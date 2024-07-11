/*
  Isotope Breakout V0 firmware

  Complementary functions for the Isotope Breakout board

*/
// Libraries---------------------------------------------------------------------
#include "Isotope_breakout_defs.h"
#include "Isotope_functions.h"

// Function definitions----------------------------------------------------------
String who_am_i()
{
  String msg = "Board name=Isotope Board,Firmware version=";
  msg += FIRMWARE_VERSION;
  return msg;
}

// Function to read an analog channel and return the ADC value (0 to 1024)
int read_analogue_val(int analog_channel)
{
  // Select which PIN to use for the analogue read
  if (analog_channel == 0)
  {
    return analogRead(Analog_In_0);
  }
  else if (analog_channel == 1)
  {
    return analogRead(Analog_In_1);
  }
  else if (analog_channel == 2)
  {
    return analogRead(Analog_In_2);
  }
  return 0; // Wrong channel, return empty value
}

int read_power_out_val(int item)
{
  switch (item)
  {
  case 0:
    return power_out_val_0;
    break;
  case 1:
    return power_out_val_1;
    break;
  case 2:
    return power_out_val_2;
    break;
  }
  return 0; // In case wrong sensor index is requested
}

// Read Temperature value in C
float read_temp_sensor(int item)
{
  switch (item)
  {
  case 0:
    Temp_sensor_0.requestTemp(); // Request temp
    delay(10);
    return Temp_sensor_0.getTemp(); // Read temperature
    break;
  case 1:
    Temp_sensor_1.requestTemp(); // Request temp
    delay(10);
    return Temp_sensor_1.getTemp(); // Read temperature
    break;
  case 2:
    Temp_sensor_2.requestTemp(); // Request temp
    delay(10);
    return Temp_sensor_2.getTemp(); // Read temperature
    break;
  }
  return 0; // In case wrong sensor index is requested
}

// Set outoput power, set the PWM value of the Power Outputs (0 to 1024)
void set_output_power(int item, int pwm_value)
{
  switch (item)
  {
  case 0:
    power_out_val_0 = pwm_value;
    analogWrite(POWER_OUT_0, pwm_value);
    break;
  case 1:
    power_out_val_1 = pwm_value;
    analogWrite(POWER_OUT_1, pwm_value);
    break;
  case 2:
    power_out_val_2 = pwm_value;
    analogWrite(POWER_OUT_2, pwm_value);
    break;
  }
}

// Set the colors of the RGB LED on board
void set_rgb(const int *rgb_val)
{
  rgb_red = rgb_val[0];
  rgb_green = rgb_val[1];
  rgb_blue = rgb_val[2];
  rgb.setPixelColor(0, rgb.Color(rgb_red, rgb_green, rgb_blue));
  rgb.show();
}

void set_rgb(int channel, int value)
{
  int rgb[3];
  rgb[channel] = value;
  set_rgb(rgb);
}

// Update the time of when the last communication was received
void update_comms_latency()
{
  // Update the time of last HeartBeat
  Time_since_last_comms = millis();
  // Change the state of Heartbeat_alive flag if needed
  if (!Heartbeat_alive)
  {
    Heartbeat_alive = true;
    // Clear the RED colour of the RGB
    set_rgb(1, 0);
  }
}

// Check how long has it been since last communication and update HB_alive flag
void check_comms_latency()
{
  // Check current time against time of last HeartBeat
  unsigned long now = millis();
  if (now > (Time_since_last_comms + Comms_max_latency))
  {
    // If last heartbeat was longer ago than the max latency allowed, change state to not alive
    Heartbeat_alive = false;
    // If this happens, we are supposing that either the system is plugged into power but not
    // in use, the control program in the control PC has crashed, or the communication between
    // the Isotope Board and the control PC has been lost.
    // Based on previous possible scenarios, turn off all outputs
    pwm_controller.disable();
    set_output_power(0, 0);
    set_output_power(1, 0);
    set_output_power(2, 0);
    // TODO turn Off Power Outputs PWMs
    // Set the RGB in RED to show this
    set_rgb(0, 30);
  }
}
