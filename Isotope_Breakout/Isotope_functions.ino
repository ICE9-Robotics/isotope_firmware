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
  return analogRead(analogue_pins[analog_channel]);
}

int read_power_out_val(int item)
{
  return power_out_vals[item];
}

// Set outoput power, set the PWM value of the Power Outputs (0 to 1024)
void set_output_power(int item, int pwm_value)
{
  power_out_vals[item] = pwm_value;
  analogWrite(power_out_pins[item], pwm_value);
}

// Set the colors of the RGB LED on board
void set_rgb()
{
  rgb.setPixelColor(0, rgb.Color(led_rgb_vals[0], led_rgb_vals[1], led_rgb_vals[2]));
  rgb.show();
}

void set_rgb(const int *rgb_val)
{
  memcpy(led_rgb_vals, rgb_val, sizeof(led_rgb_vals));
  set_rgb();
}

void set_rgb(int channel, int value)
{
  led_rgb_vals[channel] = value;
  set_rgb();
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
    set_rgb(0, 0);
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
