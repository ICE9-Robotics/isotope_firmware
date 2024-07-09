/*
  Isotope Breakout V0 firmware

  Communication Protocol definitions for the Isotope Braakout V0 board

*/
#ifndef COMMS_PROT_H
#define COMMS_PROT_H
#include <Arduino.h>

//String definition for the protocol-------------------------------------------
// Type Definitions
String GET_S = "GET";
String SET_S = "SET";

//Section Definitions
String WHO_I_AM         =   "Who_I_am";
String ID_VALUE         =   "ID_value";
String HEARTBEAT        =   "HeartBeat";
String POWER_OUTPUT     =   "Power_output";
String TEMP_SENSOR      =   "Temp_sensor";
String PWM_OUTPUT       =   "PWM_output";
String PWM_ENABLE       =   "PWM_enable";
String ANALOG_INPUT     =   "Analog_input";
String RGB_RED          =   "RGB_red";
String RGB_GREEN        =   "RGB_green";
String RGB_BLUE         =   "RGB_blue";
String MOTOR_RPM_SPEED  =   "Motor_rpm_speed";
String MOTOR_STEP       =   "Motor_step";
String MOTOR_CURRENT_MILLIAMPS  =   "Motor_current_milliamps";
String MOTOR_ENABLE     =   "Motor_enable";

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
