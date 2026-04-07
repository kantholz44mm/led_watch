# Description

It's a wristwatch using LEDs to show the current time. This repository contains one major folder for the hardware part, which contains a KiCAD project with the schematics & PCB layout, as well as a FreeCAD project with 3D-printable parts for the case.

# Prerequisites

## Software

- ARM GCC toolchain (`arm-none-eabi-xxx`)
- GNU Make

## Hardware

- KiCAD (for editing the schematics/PCB)
- FreeCAD (for editing the case)

# Getting Started

## Software

To compile the firmware, simply doing

`make`

in the `software` directory should suffice.

