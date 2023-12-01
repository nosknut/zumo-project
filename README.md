# Project Structure

This repository contains 3 projects:

- `IrTransponder` is a test code to verify that the IrSocket library works and is stable.
- `ChargerLink` is the code for the charging station and [ChargingStation.ino](ChargingStation/ChargingStation.ino) is feature complete, meaning that it is ready to be used and does not require any alteration. The code is written for use with the `Arduino Uno` board.
- `Zumo` is the code for the zumo robot, and [Zumo.ino](Zumo/Zumo.ino) is the main file. This is an example code to demo the capabilities of the `ChargerLink` library. To import the `ChargerLink` library, copy the ChargerLink folder into your sketch folder. The code is written for use with the `Pololu A-Star 32U4` board.

Each of these projects contain a `dependencies.txt` file which lists the libraries that are required to run the code. In addition to installing these libraries the [CarLink](ChargingStation/CarLink), [IrSocket](IrTransponder/IrSocket) or [ChargerLink](Zumo/ChargerLink) folders must be copied into your sketch folder depending on which one you intend to use.

# Docs

- [ChargerLink](Zumo/ChargerLink/README.md)
