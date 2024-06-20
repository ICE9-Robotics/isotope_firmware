/*
  Isotope Breakout V0 firmware

  Communication Protocol definitions for the Isotope Braakout V0 board

*/
#ifndef COMMS_PROT_H
#define COMMS_PROT_H
#include <Arduino.h>

//String definition for the protocol-------------------------------------------
// Type Definitions
String get_s = "GET";
String set_s = "SET";

//Section Definitions
String Who_I_am         =   "Who_I_am";
String ID_value         =   "ID_value";
String HeartBeat        =   "HeartBeat";
String Power_output     =   "Power_output";
String Temp_sensor      =   "Temp_sensor";
String PWM_output       =   "PWM_output";
String PWM_enable       =   "PWM_enable";
String Analog_input     =   "Analog_input";
String RGB_red          =   "RGB_red";
String RGB_green        =   "RGB_green";
String RGB_blue         =   "RGB_blue";
String Motor_rpm_speed  =   "Motor_rpm_speed";
String Motor_step       =   "Motor_step";
String Motor_current_milliamps  =   "Motor_current_milliamps";
String Motor_enable     =   "Motor_enable";

// Responses Definitions
String cmd_ack =  "ACK"; //Used as a code for 'No error'

//Command strcuture-------------------------------------------------------------
struct CMD_t {
  String type;
  int reg_addrs;
  int payload;
};

//Errors commands---------------------------------------------------------------
String error =                    "ERR";
String wrong_JSON_format =        "ERR0";
String wrong_cmd_type =           "ERR1";
String wrong_cmd_section =        "ERR2";
String wrong_cmd_item =           "ERR3";
String wrong_cmd_value =          "ERR4";



#endif
