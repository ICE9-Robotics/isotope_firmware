/*
Hardware definitions file for the
Isotpe Breakout V0.1
*/

#ifndef ISOTOPE_DEFS_H
#define ISOTOPE_DEFS_H
#include <Arduino.h>

#define FIRMWARE_VERSION "1.0.1"

//Power Outputs
#define POWER_OUT_0       8
#define POWER_OUT_1       9
#define POWER_OUT_2       11

//Analog Inputs
#define Analog_In_0       28
#define Analog_In_1       27
#define Analog_In_2       26

//Temp Sensors
#define Temp_In_0         0
#define Temp_In_1         1
#define Temp_In_2         2

//PWM Outputs
#define PWM_EN            7
#define PWM_0             6
#define PWM_1             5
#define PWM_2             4
#define PWM_3             3

//Motor Drivers
#define SPI_MOSI          19
#define SPI_MISO          16
#define SPI_SLCK          18

#define SPI_CS_0          22
#define SPI_CS_1          21
#define SPI_CS_2          20
#define SPI_CS_3          13

#define DRV_FAULT_0       10
#define DRV_FAULT_1       15
#define DRV_FAULT_2       14
#define DRV_FAULT_3       12

//RGB LED
#define RGB_LED           17//WS2812B LED Data input

#endif
