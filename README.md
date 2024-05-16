# Wireless Presenter
## Introducere

Wireless Presenter se dorește a fi exact ceea ce sugerează și numele: o telecomandă wireless ce poate interacționa cu software de susținere a prezentărilor (Google Slides, OpenOffice Impress, Microsoft Office PowerPoint, Keynote etc.) într-o manieră simplă și cu un cost al materialelor redus. Acesta va putea să realizeze acțiunile uzuale de control ale prezentărilor (next slide, previous slide, start slideshow), cât și mișcarea cursorului sistemului de calcul ce susține prezentarea. Suplimentar, telecomanda va avea un pointer cu care prezentatorul să poată indica audienței diferite elemente de pe slide-uri.

Ideea de la care am pornit pentru acest proiect a fost nevoia de a susține prezentări. Deoarece a cumpăra o soluție comercială nu avea sens din punct de vedere economic pentru mine la acel moment, am creat o versiune proprie a unui presenter, folosind un Arduino, un senzor infraroșu și o telecomandă de la un radio vechi, ce interacționau împreună cu un software de tip daemon, ce primea date prin USB de la Arduino și le traducea în apăsări de taste pe sistemul țintă.

Utilitatea abordării pe care o folosesc acum, spre deosebire de cea anterioară, este că, prin crearea propriului hardware al telecomenzii, pot fi adăugate/modificate funcționalități după bunul plac, astfel încât telecomanda ar putea deveni folositoare în mai multe scenarii, nu doar în susținerea prezentărilor.

## Descriere generală

### Schema bloc

![Schema bloc](images/schema_bloc.png?raw=true "Schema bloc")

## Hardware Design

### Listă piese
  * Arduino Nano (microcontroller telecomandă)
  * Arduino Uno (microcontroller receiver)
  * Modul accelerometru + giroscop MPU6050
  * Transmițător RF 433MHz FS1000A
  * Receptor RF 433MHz XY-MK-5V
  * Butoane
  * Senzor capacitiv TTP223
  * Laser KY-008
  * Suport baterie
  * Boost Converter 5V

### Schema electrică

![Schema electrica](images/schema_electrica.png?raw=true "Schema electrica")

### Detaliere hardware
Încă de la începutul implementării hardware a telecomenzii, design-ul final pe care l-am avut în minte a fost unul bazat pe împărțirea în două categorii a componentelor proiectului: cele ce au nevoie de alimentare cu 5V și/sau să fie în partea inferioară a telecomenzii și celelalte. Astfel, pot grupa toate componentele din prima categorie în partea inferioară. În practică, pe partea superioară au rămas doar butoanele de control, care pot fi utilizate fără alimentare prin utilizarea rezistențelor de pull-up.

**Componentele și utilizarea lor:**
  * MPU6050
    * **Scop:** detectează mișcarea mâinii utilizatorului pentru a o traduce în mișcarea cursorului pe sistemul de calcul
    * **Conectare:** fiind un modul ce folosește protocolul de comunicare I²C, am folosit pinii A4 (PC4/SDA) și A5 (PC5/SCL) specifici acestui protocol

  * FS1000A
    * **Scop:** trimite comenzi către receptorul conectat la sistemul de calcul
    * **Conectare:** din motive de simplitate a organizării hardware a componentelor, am ales pinul A3 (PC3); biblioteca utilizată pentru comunicarea wireless poate fi configurată să folosească orice pin de GPIO

  * XY-MK-5V
    * **Scop:** recepționează datele transmise de telecomandă
    * **Conectare:** pinul D11 (PB3) este cel default al bibliotecii pentru recepționarea de date și nu am avut niciun motiv să îl schimb

  * Butoanele superioare
    * **Scop:** fiecare buton reprezintă o comandă pe care telecomanda o poate transmite sistemului de calcul
    * **Conectare:** întrucât numărul de butoane este foarte mic, am ales să folosesc pinii D2 - D6 (PD2 - PD6), deoarece, fiind pe același port, pot activa întreruperile de schimbare ale pinilor doar pe acest port, putând astfel să detectez în mod rapid schimbarea stării butoanelor fără a recurge la polling; suplimentar, am ales ca butoanele PREV și NEXT să fie conectate la pinii D2 și D3 (PD2/INT0 și PD3/INT1), deoarece acestea vor fi folosite cel mai mult în cadrul prezentărilor, așa că vrem ca ele să aibă prioritate față de celelalte butoane din punct de vedere al întreruperilor

  * Butonul inferior
    * **Scop:** în funcție de starea telecomenzii, acesta poate controla ori pointerul laser de pe telecomandă, ori transmiterea unui click al cursorului sistemului de calcul
    * **Conectare:** am ales pinul A2 (PC2) doar din considerente de rutare ale cablurilor, orice pin de pe această parte a Nano-ului ce nu are deja o utilizare consacrată poate fi utilizat la fel de bine

  * TTP223
    * **Scop:** detectarea degetului utilizatorului, pentru a comuta între modul normal de funcționare al telecomenzii și cel de control al cursorului: în prezența degetului, telecomanda transmite constant mișcarea telecomenzii pentru a mișca cursorul, iar butonul inferior controlează click-ul, iar în absența degetului, butonul inferior controlează laserul
    * **Conectare:** fiind echivalentul unui buton, din motive similare cu butonul inferior, am ales pinul A0 (PC0) pentru acest senzor

  * KY-008
    * **Scop:** proiectează un punct roșu laser, asemeni telecomenzilor de prezentare tradiționale, pentru a indica detalii pe slide-urile prezentării
    * **Conectare:** similar cu butonul inferior, orice pin GPIO putea fi folosit pentru laser; singurul rămas neutilizat pe partea inferioară a Nano-ului (partea stângă în schema electrică) a fost A1 (PC1), așa că pe acesta l-am utilizat

  * Boost Converter 5V
    * **Scop:** ridică tensiunea bateriilor la 5V, tensiunea nominală a componentelor utilizate
    * **Conectare:** pinii de GND și 5V sunt comuni cu cei ai tuturor celorlalte componente, iar pinul de intrare a tensiunii este conectat, printr-un switch, la terminalul pozitiv al bateriilor; terminalul negativ este conectat tot la GND

**Demo prototip:** https://www.youtube.com/watch?v=4hf4uMTWSRc


## Software Design

GitHub: https://github.com/Alexander1752/wireless_presenter

## Rezultate Obţinute

## Jurnal

  * *24.04.2024* - temă finală de proiect aleasă
  * *29.04.2024* - componentele principale cumpărate
  * *02.05.2024* - prototipare pe breadboard a schemei electrice; testare funcționalitate componente
  * *03.05.2024* - implementare incipientă a daemon-ului în Python
  * *05.05.2024* - creare pagină proiect OCW, creare schemă bloc/electrică de bază
  * *08.05.2024 - 13.05.2024* - implementare hardware a schemei electrice (fără baterii)
  * *16.05.2024* - implementare a primei versiuni de cod pentru microcontrollere, detaliere și documentare schemă electrică

## Bibliografie/Resurse

  * **Resurse Software**
    * [VirtualWire Library](https://github.com/latchdevel/VirtualWire)
    * [Logo Python utilizat în schema bloc](https://www.python.org/community/logos/)
    * [Bibliotecă EAGLE cu plăci Arduino](https://github.com/cyberlink1/Arduino-Eagle-Cad-Library/)
  * **Resurse Hardware**
    * [Datasheet ATmega328P](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
    * [Datasheet MPU6050](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
    * [Understanding the I²C Bus - Texas Instruments](https://www.ti.com/lit/an/slva704/slva704.pdf)
