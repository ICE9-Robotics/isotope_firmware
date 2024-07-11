/*
  Isotope Breakout V0 firmware

  Complementary functions used in the Isotpe Breakout code

*/
#ifndef ISOTOPE_FUNC_H
#define ISOTOPE_FUNC_H
#include <Arduino.h>
#include "Isotope_breakout_defs.h"

// General variables to store current values
int power_out_val_0 = 0;
int power_out_val_1 = 0;
int power_out_val_2 = 0;

// Store general variables for RGB_LED Colour
int rgb_red = 0;
int rgb_green = 0;
int rgb_blue = 0;

// General variables for motor control
// Time delay between steps in microseconds, per motor
uint32_t Step_Period_uS_0 = 2000;
uint32_t Step_Period_uS_1 = 2000;
uint32_t Step_Period_uS_2 = 2000;
uint32_t Step_Period_uS_3 = 2000;
// Motor  continious speed values given by the user. TODO: Set the values to revs/min
//  Motor continious speed can be positive or negative, which determines the direction of movement
int Motor_rpm_speed_0 = 0;
int Motor_rpm_speed_1 = 0;
int Motor_rpm_speed_2 = 0;
int Motor_rpm_speed_3 = 0;

int Max_motor_rpm_speed = 200; // Limit speed to 200 revs per minute

// Motor steps given by the user
// Motor steps can be positive or negative, which determine
int Motor_steps_0 = 0;
int Motor_steps_1 = 0;
int Motor_steps_2 = 0;
int Motor_steps_3 = 0;

// Motor step speed
// Motor steps speed is a magnitud, only positive values accepted
int Motor_steps_speed_0 = 0;
int Motor_steps_speed_1 = 0;
int Motor_steps_speed_2 = 0;
int Motor_steps_speed_3 = 0;

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

/// @brief Reads temperature sensor value in Celsius
/// @param item Port ID
/// @return Temperature value in Celsius
float read_temp_sensor(int item);

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
