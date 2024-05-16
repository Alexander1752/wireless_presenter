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
  * Butoane/Taste
  * Laser
  * Suport baterie
  * Boost Converter 5V

### Schema electrică (în lucru)

![Schema electrica](images/schema_electrica.png?raw=true "Schema electrica")

## Software Design

## Rezultate Obţinute

## Jurnal

  * *24.04.2024* - temă finală de proiect aleasă
  * *29.04.2024* - componentele principale cumpărate
  * *02.05.2024* - prototipare pe breadboard a schemei electrice; testare funcționalitate componente
  * *03.05.2024* - implementare incipientă a daemon-ului în Python
  * *05.05.2024* - creare pagină proiect OCW, creare schemă bloc/electrică de bază

## Bibliografie/Resurse

  * **Resurse Software**
    * [Logo Python utilizat în schema bloc](https://www.python.org/community/logos/)
    * [Bibliotecă EAGLE cu plăci Arduino](https://github.com/cyberlink1/Arduino-Eagle-Cad-Library/)
  * **Resurse Hardware**
    * [Datasheet ATmega328P](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf)
    * [Datasheet MPU6050](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
    * [Understanding the I²C Bus - Texas Instruments](https://www.ti.com/lit/an/slva704/slva704.pdf)
