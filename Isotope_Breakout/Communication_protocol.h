/*
  Isotope Breakout V0 firmware

  Communication Protocol definitions for the Isotope Braakout V0 board

*/
#ifndef COMMS_PROT_H
#define COMMS_PROT_H
#include <Arduino.h>

//String definition for the protocol-------------------------------------------
// Type Definitions
const String GET_S = "GET";
const String SET_S = "SET";

//Section Definitions
const String WHO_I_AM         =   "Who_I_am";
const String ID_VALUE         =   "ID_value";
const String HEARTBEAT        =   "HeartBeat";
const String POWER_OUTPUT     =   "Power_output";
const String TEMP_SENSOR      =   "Temp_sensor";
const String PWM_OUTPUT       =   "PWM_output";
const String PWM_ENABLE       =   "PWM_enable";
const String ANALOG_INPUT     =   "Analog_input";
const String RGB_RED          =   "RGB_red";
const String RGB_GREEN        =   "RGB_green";
const String RGB_BLUE         =   "RGB_blue";
const String MOTOR_RPM_SPEED  =   "Motor_rpm_speed";
const String MOTOR_STEP       =   "Motor_step";
const String MOTOR_CURRENT_MILLIAMPS  =   "Motor_current_milliamps";
const String MOTOR_ENABLE     =   "Motor_enable";



// Responses Definitions
String CMD_ACK =  "ACK"; //Used as a code for 'No error'

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
