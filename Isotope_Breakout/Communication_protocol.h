/*
  Isotope Breakout firmware

  Communication Protocol definitions for the Isotope Braakout V2.0 board
*/

#ifndef COMMS_PROT_H
#define COMMS_PROT_H
#include <Arduino.h>

//String definition for the protocol-------------------------------------------
// Type Definitions
const String GET_S = "GET";
const String SET_S = "SET";
const String PWM_MODE_STANDARD_s = "0";
const String PWM_MODE_SERVO_s = "1";
const String PWM_VALUE_TYPE_PWM_s = "0";
const String PWM_VALUE_TYPE_MS_s = "1";

//Section Definitions
const String WHO_I_AM         =   "Who_I_am";
const String ID_VALUE         =   "ID_value";
const String HEARTBEAT        =   "HeartBeat";
const String POWER_OUTPUT     =   "Power_output";
const String TEMP_SENSOR      =   "Temp_sensor";
const String PWM_OUTPUT       =   "PWM_output";
const String PWM_ENABLE       =   "PWM_enable";
const String ANALOG_INPUT     =   "Analog_input";
const String RGB              =   "RGB";
const String MOTOR_RPM_SPEED  =   "Motor_rpm_speed";
const String MOTOR_STEP_ANGLE =   "Motor_step_angle";
const String MOTOR_STEP       =   "Motor_step";
const String MOTOR_CURRENT_MILLIAMPS  =   "Motor_current_milliamps";
const String MOTOR_ENABLE     =   "Motor_enable";
const String PWM_MODE         =   "PWM_mode";

// Responses Definitions
const String CMD_ACK =  "ACK"; //Used as a code for 'No error'

//Command strcuture-------------------------------------------------------------
struct CMD_t {
  String type;
  int reg_addrs;
  int payload;
};

//Errors commands---------------------------------------------------------------
const String UNKNOWN_RESP_S =             "ERR";
const String WRONG_JSON_FORMAT_S =        "ERR0";
const String WRONG_CMD_TYPE_S =           "ERR1";
const String WRONG_CMD_SECTION_S =        "ERR2";
const String WRONG_CMD_ITEM_S =           "ERR3";
const String WRONG_CMD_VALUE_S =          "ERR4";

enum cmd_resp_t{
  OK = 0,
  WRONG_JSON_FORMAT = 1,
  WRONG_CMD_TYPE = 2,
  WRONG_CMD_SECTION = 3,
  WRONG_CMD_ITEM = 4,
  WRONG_CMD_VALUE = 5
};

#endif
