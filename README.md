# Door_Locker_Security_System
- Created by: Jana Wael Elgendy.
- Supervised by: Eng. Mohamed Tarek.
- Description: this project is a door locker system developed to:
1. set a passcode for the door.
2. verify the passcode.
3. save the passcode for later usage to open the door.
4. open the door if the passcode entered matches.
- To develop this system I used:
1. AVR ATmega32 Microcontroller.
2. Drivers: GPIO, Keypad, LCD, Timer, UART, TWI->"I2C", EEPROM, Buzzer and DC-Motor.
T- he project designed and implemented based on the layered architectural model as followed:
1. MC1 -> HMI_ECU (Human Machine Interface) with 2x16 LCD and 4x4 keypad..
2. MC2 -> Control_ECU with EEPROM, Buzzer, and Dc-Motor..
- HMI_ECU: responsible for the interaction with the user by just taking input through keypad and displaying messages on the LCD.
- CONTROL_ECU: responsible for the processing and decisions in the system like password checking, opening the door and activating system's alarm.
