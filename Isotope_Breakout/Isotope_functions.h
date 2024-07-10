/*
  Isotope Breakout V0 firmware

  Complementary functions used in the Isotpe Breakout code

*/
#ifndef ISOTOPE_FUNC_H
#define ISOTOPE_FUNC_H
#include <Arduino.h>
#include "Isotope_breakout_defs.h"

//General variables to store current values
int power_out_val_0 = 0;
int power_out_val_1 = 0;
int power_out_val_2 = 0;

//Store general variables for RGB_LED Colour
int rgb_red = 0;
int rgb_green = 0;
int rgb_blue = 0;

//General variables for motor control
//Time delay between steps in microseconds, per motor
uint32_t Step_Period_uS_0 = 2000;
uint32_t Step_Period_uS_1 = 2000;
uint32_t Step_Period_uS_2 = 2000;
uint32_t Step_Period_uS_3 = 2000;
//Motor  continious speed values given by the user. TODO: Set the values to revs/min
// Motor continious speed can be positive or negative, which determines the direction of movement
int Motor_rpm_speed_0 =0;
int Motor_rpm_speed_1 =0;
int Motor_rpm_speed_2 =0;
int Motor_rpm_speed_3 =0;

int Max_motor_rpm_speed = 200;//Limit speed to 200 revs per minute

//Motor steps given by the user
//Motor steps can be positive or negative, which determine
int Motor_steps_0 = 0;
int Motor_steps_1 = 0;
int Motor_steps_2 = 0;
int Motor_steps_3 = 0;

//Motor step speed
//Motor steps speed is a magnitud, only positive values accepted
int Motor_steps_speed_0 = 0;
int Motor_steps_speed_1 = 0;
int Motor_steps_speed_2 = 0;
int Motor_steps_speed_3 = 0;

//Communication latency counter and max latency
unsigned long Time_since_last_comms = 0;
unsigned long Comms_max_latency = 1000000000;// Equal to 10 seconds
bool Heartbeat_alive = true;

// Complementary Functions for GET commands-------------------------------------

/// @brief Returns the information of the board and the firmware
/// @return ID of the board and firmware version in a human readable format
String who_am_i();

/// @brief Reads the value of the specified analog channel
/// @param analog_channel Analog channel ID
/// @return Analog value from 0 to 1024
int  read_analogue_val(int analog_channel);

/// @brief Reads the enable status for all PWM ports
/// @return Enable status for all PWM ports
bool read_pwm_en();

/// @brief Reads the value of the specified PWM ports
/// @param item Port ID
/// @return PWM value of the PWM port
int read_pwm_out_val(int item);

/// @brief Reads the value of the specified power output ports
/// @param item Port ID
/// @return PWM value of the power output port
int read_power_out_val(int item);

/// @brief Reads temperature sensor value in Celsius
/// @param item Port ID
/// @return Temperature value in Celsius
float read_temp_sensor(int item);

/// @brief Reads the motor speed in revolutions per minute of the specified motor port
/// @param item Port ID
/// @return Motor speed in RPM
int read_motor_rpm_speed(int item);

/// @brief Sets the PWM value of the specified power output port
/// @param item Port ID
/// @param pwm_value PWM value from 0 to 1024
void set_output_power(int item, int pwm_value);

/// @brief Sets the mode of the specified PWM output port
/// @param item Port ID
/// @param pwm_mode PWM mode, SERVO or STANDARD
///   Set to SERVO to use the servo library functions, and control the PWM output in degrees or microseconds
///   Set to STANDARD to use the standard PWM output, and control the PWM output in the range of 0 to 1024
void set_pwm_mode(int item, pwm_mode_t pwm_mode);

/// @brief Sets the PWM value of the specified PWM output port
/// @param item Port ID
/// @param pwm_value value of the PWM output
/// @param value_type type of the value, PWM or MS. 
///   In STANDARD mode, only PWM is valid and the value range from 0 to 1024.
///   In SERVO mode with PWM value type, it calls the servo.write(value) function. Please refer to the servo library for more information.
///   In SERVO mode with MS value type, it calls the servo.writeMicroseconds(value) function. Please refer to the servo library for more information.
void set_pwm_output(int item, int pwm_value, pwm_value_type_t value_type);

/// @brief Enables or disables all PWM outputs
/// @param state True to enable PWM outputs, False to disable PWM outputs
void set_pwm_en(bool state);

/// @brief Sets motor speed in revolutions per minute
/// @param item Port ID
/// @param speed_value Speed value in revolutions per minute
void set_motor_rpm_speed(int item, int speed_value);

/// @brief Sets the colors of the RGB LED on board
/// @param rgb_values values of the Red, Green and Blue channels
void set_rgb(int *rgb_values);


void set_rgb(int channel, int value);

/// @brief Sets motor steps, set the direction based on sign and the absolute value for steps
/// @param item Port ID
/// @param steps Step value, positive for forward, negative for backward
void set_motor_steps(int item, int steps);

/// @brief Sets motor current limit in milliamps
/// @param item Port ID
/// @param current Current limit in milliamps
void set_motor_current_milliamps(int item, int current);

/// @brief Enables or disables motor
/// @param item Port ID
/// @param state True to enable motor, False to disable motor
void set_motor_enable(int item, bool state);

/// @brief Updates the time of when the last communication was received
void update_comms_latency();

/// @brief Checks how long has it been since last communication and update HB_alive flag
void check_comms_latency();
#endif
