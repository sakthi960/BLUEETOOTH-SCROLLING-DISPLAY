# Bluetooth Scrolling Display Using WS2812B

An embedded systems project that explores wireless text display using Bluetooth communication and WS2812B addressable RGB LED technology.

## About the Project

This project focuses on developing a Bluetooth-controlled scrolling LED display using WS2812B LED strips.

The concept is to receive information through Bluetooth communication and display scrolling content using individually addressable RGB LEDs.

It combines wireless communication, embedded programming, and LED display control to create a simple electronic information display system.

## Project Overview

| Parameter | Description |
|---|---|
| Project Name | Bluetooth Scrolling Display |
| Display Technology | WS2812B RGB LED |
| Communication | Bluetooth |
| Project Domain | Embedded Systems / IoT |
| Display Type | Scrolling LED Display |

## Key Features

- Bluetooth-based wireless communication.
- Scrolling text display concept.
- WS2812B addressable RGB LED integration.
- LED-based information display.
- Embedded system control.

## Technologies & Components

- WS2812B Addressable RGB LED Strip
- Bluetooth Communication Module
- Embedded Controller (refer to the source code for the exact board)
- Embedded C/C++ or the language used in the implementation

## Working Principle

1. A user sends text or display information through a Bluetooth-connected device.
2. The embedded controller receives the transmitted data.
3. The received information is processed by the controller.
4. The controller generates the required LED data.
5. WS2812B LEDs display the content as a scrolling message.

## Repository Structure

```text
BLUETOOTH-SCROLLING-DISPLAY/
│
├── BLUETOOTH/
│
└── README.md
```

## Getting Started

1. Clone the repository.

```bash
git clone https://github.com/sakthi960/BLUETOOTH-SCROLLING-DISPLAY.git
```

2. Explore the `BLUETOOTH` folder.
3. Identify the controller board and Bluetooth module used.
4. Install the required development environment and libraries.
5. Configure the Bluetooth communication and WS2812B LED connections.
6. Upload the firmware to the controller.
7. Test wireless communication and scrolling display functionality.

**Note:** Refer to the actual source code for hardware pin assignments, Bluetooth configuration, LED count and required libraries.

## Applications

- Wireless information display systems.
- Digital scrolling message boards.
- Electronic notice boards.
- Embedded display prototypes.

## Author

**SAKTHI RAJA V**

GitHub: https://github.com/sakthi960

## Project Purpose

To explore wireless communication and addressable LED control through an embedded Bluetooth-based scrolling display system.
