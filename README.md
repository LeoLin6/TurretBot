# TurretBot for automated acoustic experiments. 

[Software section](#software).

[Hardware assembly section](#Sourcing the parts).


## Sourcing the parts


#### BOM for one set of robot: 

Note: PCB will have to be custom printed following these files (companies like JCLPCB would work): 

#### 🔩 Component List

| Part Name                                                | Quantity | Designation on Schematic         | Link                                                                 |
|----------------------------------------------------------|----------|----------------------------------|----------------------------------------------------------------------|
| Barrel Jack for power                                    | 1        | J1 (Barrel_Jack)                 | [digikey](https://www.digikey.com/en/products/detail/same-sky-formerly-cui-devices/PJ-002A/96962)           |
| PinSocket headers for Arduino Nano                       | 2        | J3, J5                            | [digikey](https://www.digikey.com/en/products/detail/sullins-connector-solutions/PPTC151LFBN-RC/810153)     |
| PinSocket headers for TMC 2208 driver chip               | 2        | J4, J6                            | [digikey](https://www.digikey.com/en/products/detail/w%C3%BCrth-elektronik/61300811821/17737805)            |
| Induction sensor terminal block                          | 1        | J7                                | [digikey](https://www.digikey.com/en/products/detail/phoenix-contact/1715734/260632)                        |
| Terminal Block for stepper motor                         | 1        | J2 (Screw_Terminal for motor)     | [digikey](https://www.digikey.com/en/products/detail/phoenix-contact/1715747/260633)                        |


PCB files for printing (supports a TMC 2208 chip controlled by arduino). Barrel jack power supply and a 4 terminal block

Follow this schematic for wiring
![Schematic Image](tmc%20pcb%20documentation/pcb_v2_schematic.png)

For motor connector reference (What's color code M1B, M1A etc)
![color code Image](tmc%20pcb%20documentation/stepper-connections.webp)

Follow this github tutorial to Solder UART bridge on tmc 2208 chip:
https://github.com/teemuatlut/TMC2208Stepper?tab=readme-ov-file


# software

Arduino files in C language to drive the TMC 2208 motor driver chips with various functionalities. Includes several testing scripts for audio experiments. Plus some files that implement UART control with motor functions. 

Open this file in Arduino IDE, flash onto arduino, note the COM port:
curr_project/softwareserial_ver_tmc_uart.ino 

Next, run this python script for calibration and positional movment functionalities:
curr_project/serial comm.py

