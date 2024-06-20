/*
  Isotope Breakout V0 firmware

  Complementary functions used in the Isotpe Breakout code

*/
#ifndef ISOTOPE_FUNC_H
#define ISOTOPE_FUNC_H
#include <Arduino.h>

//General variables to store current values
int POWER_OUT_VAL_0 = 0;
int POWER_OUT_VAL_1 = 0;
int POWER_OUT_VAL_2 = 0;

int PWM_OUT_VAL_0 = 0;
int PWM_OUT_VAL_1 = 0;
int PWM_OUT_VAL_2 = 0;
int PWM_OUT_VAL_3 = 0;

bool PWM_EN_VAL = true;//PWM enable by default

//Store general variables for RGB_LED Colour
int RGB_RED = 0;
int RGB_GREEN = 0;
int RGB_BLUE = 0;

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
uint16_t Comms_max_latency = 10000;// Equal to 10 seconds
bool Heartbeat_alive = true;

// Complementary Functions for GET commands-------------------------------------
String who_am_i();

//Function to read an analog channel and return the ADC value (0 to 1024)
int  read_analogue_val(int analog_channel);

//Read PWM enable status
bool read_pwm_en();

//Read Temperature value in C
float read_temp_sensor(int item);

//Read Motor Speed
int read_motor_rpm_speed(int item);

//Set outoput power, set the PWM value of the Power Outputs (0 to 1024)
void set_output_power(int item, int pwm_value);

//Set PWM output value, se the PWM value of the PWM outputs (0 to 1024)
void set_pwm_output(int item, int pwm_value);

//Set PWM enable
void set_pwm_en(bool state);

//Set motor speed in revolutions per minute
void set_motor_rpm_speed(int item, int speed_value);

//Set the colors of the RGB LED on board
void set_rgb(int channel, int pwm_val);

//Set motor steps, set the direction based on sign and the absolute value for steps
void set_motor_steps(int item, int steps);

//Set motor current in milliamps
void set_motor_current_milliamps(int item, int current);

//Set motor enable
void set_motor_enable(int item, bool state);

//Update the time of when the last communication was received
void update_comms_latency();

//Check how long has it been since last communication and update HB_alive flag
void check_comms_latency();
#endif
