/*
  Isotope Breakout V0 firmware

  Complementary functions used in the Isotpe Breakout code

*/
#ifndef ISOTOPE_FUNC_H
#define ISOTOPE_FUNC_H
#include <Arduino.h>
#include "Isotope_breakout_defs.h"

// I/O pin values
int power_out_vals[3] = {0, 0, 0};
int led_rgb_vals[3] = {0, 0, 0};

// I/O pin arrays
int analogue_pins[3] = {Analog_In_0, Analog_In_1, Analog_In_2};
int power_out_pins[3] = {POWER_OUT_0, POWER_OUT_1, POWER_OUT_2};

// Communication latency counter and max latency
unsigned long Time_since_last_comms = 0;
unsigned long Comms_max_latency = 10000; // Equal to 10 seconds
bool Heartbeat_alive = true;

// Complementary Functions for GET commands-------------------------------------

/// @brief Returns the information of the board and the firmware
/// @return ID of the board and firmware version in a human readable format
String who_am_i();

/// @brief Reads the value of the specified analog channel
/// @param analog_channel Analog channel ID
/// @return Analog value from 0 to 1024
int read_analogue_val(int analog_channel);

/// @brief Reads the value of the specified power output ports
/// @param item Port ID
/// @return PWM value of the power output port
int read_power_out_val(int item);

/// @brief Sets the PWM value of the specified power output port
/// @param item Port ID
/// @param pwm_value PWM value from 0 to 1024
void set_output_power(int item, int pwm_value);

/// @brief Sets the colors of the RGB LED on board
/// @param rgb_values values of the Red, Green and Blue channels
void set_rgb(const int *rgb_values);

void set_rgb(int channel, int value);

/// @brief Updates the time of when the last communication was received
void update_comms_latency();

/// @brief Checks how long has it been since last communication and update HB_alive flag
void check_comms_latency();
#endif
