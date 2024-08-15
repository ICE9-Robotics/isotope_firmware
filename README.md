# Isotope Breakout firmware

[Latest release](https://github.com/ICE9-Robotics/isotope_firmware/releases)

## Preparation

To compile and upload the firmware to the Isotope Board, the Arduino IDE is used and necessary dependencies such as Arduino core and several external libraries need to be installed.

### Arduino IDE

To install the Arduino IDE please follow the instructions [here](https://docs.arduino.cc/software/ide-v1/tutorials/Linux/)

### Arduino-Pico core and board selection

To compile for the RaspberryPi Pico W (the micontroller used in the Isotope Board), its necessary to download the specific board package. Please follow the guide found [here](https://arduino-pico.readthedocs.io/en/latest/install.html) to instal it.

Once the Arduino-Pico core has been installed, select the Pico W board. To do that, in the Arduino IDE go to Tools>Board>RaspberryPi RP2040> RaspberryPi Pico W

### Libraries

To compile this code, the following libraries are necessary:
- ArduinoJson
- microDS18B20
- DRV8434S
- Adafruit_NeoPixel
- FireTimer

To install them, in the Arduino IDE go to Sketch> Include Libary> Manage Libaries, search the names, select and install the latest versions.

## Upload isotope firmware

Once preparation work is completed, open Isotope_Breakout.ino in Arduino IDE.

Select the correct port and board from Tools menu and then select Upload.
