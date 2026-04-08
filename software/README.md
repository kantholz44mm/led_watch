# Description

The soft-/firmware is currently tailored to an STM32L071 MCU, which has a single ARM Cortex M0+ core. 

# Prerequisites

- ARM compiler toolchain (`arm-none-eabi-xxx`)
- GNU Make
- Debugger/Programmer
- Needle adapter or soldering iron & wires

You will need an ARM compiler to build the software. The build process is done using a single Makefile. To flash the code to the device, you'll also need a programmer or debugger. I personally use a JLink one within VS Code, so there's a configuration for that in the project. The hardware currently only has testpoints for attaching a debugger via SWD, but they're quite big and can easily be soldered to.

# Getting started

To compile, just go into the `software` folder and do

`make`

This should output a finished binary at `software/bin/main.elf`.

# Using the software

The software implements the following state machine for its UX:

![Image of the state machine of inputs](notes/states.png)