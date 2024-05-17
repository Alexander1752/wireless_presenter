#!/usr/bin/env python3

import PySimpleGUI as sg
import os
from enum import Enum

from threading import Thread
from queue import Queue, Empty
import serial
import serial.tools.list_ports as list_ports
import logging as log
from pynput.mouse import Button, Controller as MController
from pynput.keyboard import Key, Controller as KController
from subprocess import Popen as cmd
# log.basicConfig(level = log.INFO)

keyboard = KController()
mouse = MController()

COMMANDS = {
    "P": lambda: keyboard.press(Key.space) or keyboard.release(Key.space),
    "e": lambda: keyboard.press(Key.esc) or keyboard.release(Key.esc),
    "l": lambda: keyboard.press(Key.left) or keyboard.release(Key.left),
    "r": lambda: keyboard.press(Key.right) or keyboard.release(Key.right),
    "x": lambda: exec("with keyboard.pressed(Key.alt): keyboard.press(Key.f4) or keyboard.release(Key.f4)"),
    "u": lambda: keyboard.press(Key.media_volume_up) or keyboard.release(Key.media_volume_up),
    "d": lambda: keyboard.press(Key.media_volume_down) or keyboard.release(Key.media_volume_down),
    "p": lambda: exec("with keyboard.pressed(Key.shift): keyboard.press('p') or keyboard.release('p')"),
    "n": lambda: exec("with keyboard.pressed(Key.shift): keyboard.press('n') or keyboard.release('n')"),
    "f": lambda: exec("with keyboard.pressed(Key.shift): keyboard.press(Key.f5) or keyboard.release(Key.f5)"),
    "POWERPOINT": lambda: cmd(['C:/Program Files/Microsoft Office/root/Office16/POWERPNT.EXE'])
}

PROCENT = 0.001

FONT = ("Arial", 20)

class Status(Enum):
    RUNNING = 1
    STOPPED = 2

logo = os.path.abspath("logo.png")

if not os.path.exists(logo):
    logo = ""

refresh = os.path.abspath("refresh_white_30x30.png")

if not os.path.exists(refresh):
    refresh = ""

refresh_args = {"image_filename": refresh} if refresh != "" else {"button_text": "Refresh"}

def update_serial_ports():
    global ports
    ports = list_ports.comports()
    
    global combo_list
    combo_list = list(map(lambda x: f"{x[0]}: {x[1]}", ports))

update_serial_ports()

queue = Queue()

# First the window layout in 2 columns

parameter_list_column = [
    [
        sg.Text("Select a port:", expand_x = True, expand_y = True),
        
        sg.Button(**refresh_args, button_color = "#808080", key = "-REFRESH-")
    ],

    [
        sg.Combo(combo_list, key = "-LIST-", expand_x = True, readonly = True),
    ],

    [
        sg.Button(button_text = "Connect", button_color = "#275DA4", size = 10, key = "-CONNECT-", expand_x = True),

        sg.Button(button_text = "Disconnect", button_color = "red", size = 10, key = "-DISCONNECT-")
    ],

    [
        sg.Text("Status:"),

        sg.Text("STOPPED", expand_x = True, text_color = "red", key = "-STATUS-"),
    ],
    
    [
        sg.Text("", expand_x = True),

        sg.Button(button_text = "Quit", button_color = "#808080", key = "-QUIT-")
    ],
]


# Show the logo

image_viewer_column = [
    [sg.Image(key = "-IMAGE-", filename = logo)]
]


# ----- Full layout -----

layout = [
    [
        sg.Column(image_viewer_column),

        sg.VSeperator(),

        sg.Column(parameter_list_column)
    ],
]

def serial_loop(port: str):
    if port == "":
        queue.put(lambda: sg.popup("Please select a port from the list", font = FONT))
        window["-DISCONNECT-"].click()
        return

    try:
        ser = serial.Serial(
            port, 9600,
            bytesize = 8,
            parity = "N",
            stopbits = 1,
            timeout = 0.001
        )

    except:
        queue.put(lambda: sg.popup("Invalid port or port busy", font = FONT))
        window["-DISCONNECT-"].click()
        return

    command = b''
    first = True

    while True:
        try:
            queue.get_nowait()
            log.info("received message to end thread")
            ser.close()
            return

        except Empty:
            pass

        buf = ser.readline()
        command += buf
        
        if b'\n' in command:
            command = command.decode('ascii').strip()
            print(command)

            # Process command
            if command in COMMANDS:
                COMMANDS[command]()
            else:
                cmd_list = command.split()
                if len(cmd_list) >= 3 and cmd_list[0][0] == 'A':
                    try:
                        x = int(cmd_list[1])
                        y = int(cmd_list[2])

                        if first:
                            first = False
                            prev_x = x
                            prev_y = y

                        mouse.move(PROCENT *(x - prev_x), PROCENT * (y - prev_y))
                        
                        prev_x = x
                        prev_y = y
                    except:
                        pass
            command = b''

def disconnect():
    global thread
    try:
        queue.put(None)
        thread.join()
    except:
        log.info("disconnect error")

    while True:
        try:
            queue.get_nowait()
        except Empty:
            break

if __name__ == "__main__":
    # for port, desc, hwid in sorted(ports):
    #     print("{}: {} [{}]".format(port, desc, hwid))

    window = sg.Window(
        "Remote Daemon",
        layout,
        return_keyboard_events = True,
        icon = "arduino.ico",
        font = FONT
    )
    
    status = Status.STOPPED

    # Run the Event Loop
    while True:

        event, values = window.read()

        try:
            fn = queue.get_nowait()
            fn()
        except Empty:
            pass

        if (isinstance(event, str) and "ESCAPE" in event.upper())  or\
                event == sg.WIN_CLOSED or event in ["-QUIT-", "q",  "Q"]:
            disconnect()
            break

        elif event == "-CONNECT-":
            global thread
            disconnect()

            try:
                thread = Thread(target = serial_loop, args = (values["-LIST-"].split(':')[0],),  daemon = True)
                thread.start()

                status = Status.RUNNING

            except:
                pass

        elif event == "-DISCONNECT-":
            disconnect()

            status = Status.STOPPED

        elif event == "-PASSWORD-":
            password = values["-PASSWORD-"]

        elif event == "-REFRESH-":
            update_serial_ports()
            window["-LIST-"].update(value = '', values = combo_list)

        else:
            print(f"Received: {event}", event)

        if status == Status.STOPPED:
            window["-STATUS-"].update(value = "STOPPED", text_color = "red")
            window["-CONNECT-"].update(text = "Connect")
        else:
            window["-STATUS-"].update(value = "RUNNING", text_color = "#5DF455")
            window["-CONNECT-"].update(text = "Re-connect")

    window.close()
