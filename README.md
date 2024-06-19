# Isotope Breakout firmware

## Compiling and uploading code

To compile and upload code to the Isotope Board, the Arduino IDE is used.

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

To install them, in the Arduino IDE go to Sketch> Include Libary> Manage Libaries and select and install the previous Libraries
# isotope_firmware
