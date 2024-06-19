# MXCHIP AZ1366 Example Project: RGB Fade and IoT Message Display

Welcome to the **RGB Fade and IoT Message Display** example project for the MXCHIP AZ1366 development board! This project demonstrates how to control the RGB LED to create a fading effect and display a message on the board's screen, providing a fun and interactive way to learn about embedded systems development.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Requirements](#requirements)
- [Known Issues](#known-issues)
- [Credits](#credits)

## Overview

The RGB Fade and IoT Message Display example project is designed to showcase the capabilities of the MXCHIP AZ1366 development board. This project demonstrates how to use the board's RGB LED and display to create a visually engaging effect and show a message.

## Features

- **RGB LED Control**: Create a smooth fading effect using the onboard RGB LED.
- **Message Display**: Display the message "IoT Tuesday" on the onboard screen.
- **Interactive Learning**: Understand how to control hardware components using embedded C.

## Installation

### For Windows Users

1. **First Time Setup**:
   - Run the following command in PowerShell as an Admin:
     ```powershell
     Set-ExecutionPolicy -ExecutionPolicy RemoteSigned
     ```
   - Type **Y** and press Enter.

2. **Install the MXCHIP AZ1366 Toolbox Extension on Vscode**
    
3. **Install Drivers**:
   - Press `F1`, type the command `Mxchip Install Drivers`, and select it.
   - Confirm the prompt to install `cmake`.
   - (Note: This step may take some time.)

4. **Upload Project**:
   - Restart VSCode.
   - Press `F1`, type the command `Mxchip Upload Project`, and select it.
   - Ensure your MXCHIP is connected to the host machine.

### For Linux Users

1. **Install the MXCHIP AZ1366 Toolbox Extension on Vscode**

2. **Install Drivers**:
   - Press `F1`, type the command `Mxchip Install Drivers`, and select it.
   - Enter your password to install `cmake`.

3. **Upload Project**:
   - Restart VSCode.
   - Press `F1`, type the command `Mxchip Upload Project`, and select it.
   - Ensure your MXCHIP is connected to the host machine.

## Usage

1. **Build the Project**:
   - Open the project in VSCode.
   - Press `F1`, type and select `Mxchip Build Project`.

2. **Upload the Project**:
   - Connect your MXCHIP AZ1366 to your computer.
   - Press `F1`, type and select `Mxchip Upload Project`.

3. **Run the Project**:
   - The RGB LED will start fading through different colors.
   - The message "IoT Tuesday" will be displayed on the screen.

## Requirements

- Visual Studio Code version 1.87.0 or higher.
- Arduino IDE.
- **For Windows Users**:
  - Ensure the execution policy is set to RemoteSigned.
- **For Linux Users**:
  - Ensure you have the necessary permissions to install software.

## Known Issues

- **Long Path Issues on Windows**: Enabling long paths may have security implications. Refer to [this Microsoft documentation](https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file) for more information.
- **CMake Deprecation Warnings**: Compatibility with CMake < 3.5 will be removed from a future version of CMake. Update the VERSION argument <min> value or use a ...<max> suffix.

## Credits

This project is based on the original work by Microsoft and Eclipse ThreadX. 

- **Microsoft**: [Azure IoT DevKit MXChip](https://learn.microsoft.com/en-us/azure/iot/tutorial-devkit-mxchip-az3166-iot-hub)
- **Eclipse ThreadX**: [Getting Started with ThreadX](https://github.com/eclipse-threadx/getting-started)

**Enjoy your development with the RGB Fade and IoT Message Display project on MXCHIP AZ1366!**
