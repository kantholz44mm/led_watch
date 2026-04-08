# Schematics

The whole watch is built around an STM32L071 low power MCU. There's also a Bosch BMA400 accelerometer used for detecting taps and the absolute orientation for setting the time, bubble-level syle. The MCU has a 32KHz for precise timekeeping. The LED ring is segmented into 4 groups of 15 to drastically reduce routing complexity and part count. As a consequence, only 1 LED per group can be active at a time. In the current software, only 1 LED in total is ever on at a time. The accelerometer communicates with the MCU via I2C for configuration and has a shared interrupt line for wakeup. You can find the full schematics under `hardware/export/watch.pdf`.

# Case

There's two case designs, which are very similar: There's a Unibody with only one outer shell, as well as a bottom lid for easy battery replacement, and one body with two separate handle pieces, which makes the body part rotationally symmetric for those who might want to machine the parts on a lathe. For both cases, you'll need 4 1.6x8mm screws to hold everything together, as well as a 38mm watch-glass/crystal. You'll also need two gaskets, both of which can either be purchased, or, as I did, 3D printed from TPU. There's the FreeCAD project file, as well as STEP files for all parts. The STEP files can be found under `hardware/export/`.

# PCB

The PCB was created using KiCAD. The project includes part numbers for ordering with automated assembly from JLCPCB. You can find the exported gerber files (ready to order) in the folder `hardware/export`.