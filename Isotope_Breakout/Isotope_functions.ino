/*
  Isotope Breakout V0 firmware

  Complementary functions for the Isotope Breakout board

*/
//Libraries---------------------------------------------------------------------
#include "Isotope_breakout_defs.h"
#include "Isotope_functions.h"


//Function definitions----------------------------------------------------------
String who_am_i(){
  String msg = "Board name=Isotope Board,Firmware version=";
  msg += FIRMWARE_VERSION;
  return msg;
}

//Function to read an analog channel and return the ADC value (0 to 1024)
int  read_analogue_val(int analog_channel){
  //Select which PIN to use for the analogue read
  if(analog_channel  == 0){
    return analogRead(Analog_In_0);
  }
  else if(analog_channel == 1){
    return analogRead(Analog_In_1);
  }
  else if(analog_channel == 2){
    return analogRead(Analog_In_2);
  }
  else {
    return 0; //Wrong channel, return empty value
  }
}

//Read Temperature value in C
float read_temp_sensor(int item){
  switch (item) {
    case 0:
      Temp_sensor_0.requestTemp();//Request temp
      delay(10);
      return Temp_sensor_0.getTemp(); //Read temperature
      break;
    case 1:
      Temp_sensor_1.requestTemp();//Request temp
      delay(10);
      return Temp_sensor_1.getTemp(); //Read temperature
      break;
    case 2:
      Temp_sensor_2.requestTemp();//Request temp
      delay(10);
      return Temp_sensor_2.getTemp(); //Read temperature
      break;
  }
  return 0; //In case wrong sensor index is requested
}

//Read Motor Speed
int read_motor_rpm_speed(int item){
  switch (item) {
    case 0:
      return Motor_rpm_speed_0;
      break;
    case 1:
      return Motor_rpm_speed_1;
      break;
    case 2:
      return Motor_rpm_speed_2;
      break;
    case 3:
      return Motor_rpm_speed_3;
      break;
  }
  return 0; //In case wrong sensor index is requested
}

//Set outoput power, set the PWM value of the Power Outputs (0 to 1024)
void set_output_power(int item, int pwm_value){
  switch (item) {
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

//Set motor speed in revolutions per minute
void set_motor_rpm_speed(int item, int speed_value){
  //Clamp speed value
  if(speed_value < 0){ //Speed can't be negative
    speed_value = 0;
  }
  if(speed_value > Max_motor_rpm_speed){ //Don't allow speed to be more than max speed
    speed_value = Max_motor_rpm_speed;
  }

  int total_steps = (360/7.5);
  switch (item) {
    case 0:
      Motor_rpm_speed_0 = speed_value;
      //Calculate from revs/min to step period
      Step_Period_uS_0 = 60000000/(total_steps*speed_value);
      break;
    case 1:
      Motor_rpm_speed_1 = speed_value;
      //Calculate from revs/min to step period
      Step_Period_uS_1 = 60000000/(total_steps*speed_value);
      break;
    case 2:
      Motor_rpm_speed_2 = speed_value;
      //Calculate from revs/min to step period
      Step_Period_uS_2 = 60000000/(total_steps*speed_value);
      break;
    case 3:
      Motor_rpm_speed_3 = speed_value;
      //Calculate from revs/min to step period
      Step_Period_uS_3 = 60000000/(total_steps*speed_value);
      break;
      break;
  }
}

void set_motor_current_milliamps(int item, int current_value){
  switch (item) {
    case 0:
      motor_0.setCurrentMilliamps(current_value);
      break;
    case 1:
      motor_1.setCurrentMilliamps(current_value);
      break;
    case 2:
      motor_2.setCurrentMilliamps(current_value);
      break;
    case 3:
      motor_3.setCurrentMilliamps(current_value);
      break;
  }
}

void set_motor_enable(int item, bool state){
  switch (item) {
    case 0:
      if(state){
        motor_0.enableDriver();
      }
      else{
        motor_0.disableDriver();
      }
      break;
    case 1:
      if(state){
        motor_1.enableDriver();
      }
      else{
        motor_1.disableDriver();
      }
      break;
    case 2:
      if(state){
        motor_2.enableDriver();
      }
      else{
        motor_2.disableDriver();
      }
      break;
    case 3:
      if(state){
        motor_3.enableDriver();
      }
      else{
        motor_3.disableDriver();
      }
      break;
  }
}

//Set the colors of the RGB LED on board
void set_rgb(int *rgb_val){
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

//Set motor steps, set the direction based on sign and the absolute value for steps
void set_motor_steps(int item, int steps){
  set_rgb(2, 50);
  //Set direction based on steps sign
  switch (item)
  {
  case 0: //Set dir for motor 0
    if(steps > 0){
      motor_0.setDirection(0);
    }
    else{
      motor_0.setDirection(1);
    }
    break;
  case 1: //Set dir for motor 1
    if(steps > 0){
      motor_1.setDirection(0);
    }
    else{
      motor_1.setDirection(1);
    }
    break;
  case 2: //Set dir for motor 2
    if(steps > 0){
      motor_2.setDirection(0);
    }
    else{
      motor_2.setDirection(1);
    }
    break;
  case 3: //Set dir for motor 3
    if(steps > 0){
      motor_3.setDirection(0);
    }
    else{
      motor_3.setDirection(1);
    }
    break;
  }
  //Use the absolute value of steps to count how many steps to take
  steps = abs(steps);
  //Move the amout of steps requested, and send ACK when finished
  switch (item)
  {
  case 0: //Move the amount of steps for motor 0
    for(int i=0; i<=steps; i++){
      motor_0.step();
      delayMicroseconds(Step_Period_uS_0);
    }
    break;
  case 1: //Move the amount of steps for motor 1
    for(int i=0; i<=steps; i++){
      motor_1.step();
      delayMicroseconds(Step_Period_uS_1);
    }
    break;
  case 2: //Move the amount of steps for motor 2
    for(int i=0; i<=steps; i++){
      motor_2.step();
      delayMicroseconds(Step_Period_uS_2);
    }
    break;
  case 3: //Move the amount of steps for motor 3
    for(int i=0; i<=steps; i++){
      motor_3.step();
      delayMicroseconds(Step_Period_uS_3);
    }
    break;
  }
  set_rgb(2, 0);
}

//Update the time of when the last communication was received
void update_comms_latency(){
  //Update the time of last HeartBeat
  Time_since_last_comms = millis();
  //Change the state of Heartbeat_alive flag if needed
  if(!Heartbeat_alive){
    Heartbeat_alive = true;
    //Clear the RED colour of the RGB
    set_rgb(1,0);
  }
}

//Check how long has it been since last communication and update HB_alive flag
void check_comms_latency(){
  //Check current time against time of last HeartBeat
  unsigned long now = millis();
  if( now > (Time_since_last_comms + Comms_max_latency) ){
    //If last heartbeat was longer ago than the max latency allowed, change state to not alive
    Heartbeat_alive = false;
    //If this happens, we are supposing that either the system is plugged into power but not
    //in use, the control program in the control PC has crashed, or the communication between
    //the Isotope Board and the control PC has been lost.
    //Based on previous possible scenarios, turn off all outputs
    pwm_controller.disable();
    set_output_power(0, 0);
    set_output_power(1, 0);
    set_output_power(2, 0);
    //TODO turn Off Power Outputs PWMs
    //Set the RGB in RED to show this
    int rgb_val[3] = {30,0,0};
    set_rgb(rgb_val);
  }
}
