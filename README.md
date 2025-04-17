TurretBot for acoustic experiments. 

[Software section](#software).

[Hardware assembly section](#hardware).


# software

Arduino files in C language to drive the TMC 2208 motor driver chips with various functionalities. Includes several testing scripts for audio experiments. Plus some files that implement UART control with motor functions. 

Open this file in Arduino IDE, flash onto arduino, note the COM port:
curr_project/softwareserial_ver_tmc_uart.ino 

Next, run this python script for calibration and positional movment functionalities:
curr_project/serial comm.py


# hardware assembly
PCB files for printing (supports a TMC 2208 chip controlled by arduino). Barrel jack power supply and a 4 terminal block

Follow this schematic for wiring
![Schematic Image](tmc%20pcb%20documentation/pcb_v2_schematic.png)

For motor connector reference (What's color code M1B, M1A etc)
![color code Image](tmc%20pcb%20documentation/stepper-connections.webp)

Follow this github tutorial to Solder UART bridge on tmc 2208 chip:
https://github.com/teemuatlut/TMC2208Stepper?tab=readme-ov-file

BOM: 
| Id   | Designator                                               | Footprint                              | Quantity | Designation              | Supplier and ref |   |
|:-----|:---------------------------------------------------------|:----------------------------------------|----------:|:--------------------------|:------------------|--:|
| J1   | BarrelJack_Horizontal                                    | 1                                       | Barrel_Jack              | -                | - |
| J3   | PinSocket_1x15_P2.54mm_Vertical                          | 1                                       | left_Socket              | -                | - |
| J6   | PinSocket_1x08_P2.54mm_Vertical                          | 1                                       | right_Socket             | -                | - |
| J5   | PinSocket_1x15_P2.54mm_Vertical                          | 1                                       | right-Socket             | -                | - |
| J2   | TerminalBlock_Phoenix_MKDS-1,5-4_1x04_P5.00mm_Horizontal | 1                                       | Screw_Terminal for motor | -                | - |
| J4   | PinSocket_1x08_P2.54mm_Vertical                          | 1                                       | left_Socket              | -                | - |
