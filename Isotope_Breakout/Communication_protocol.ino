/*
  Isotope Breakout V0 firmware

  Comms Protocol for the Isotope Breakout V0 board

  This code requieres the use of ArduinoJson library,
  install it directly from the Arduino IDE.

*/
//Libraries---------------------------------------------------------------------
#include "Isotope_breakout_defs.h"
#include "Communication_protocol.h"
#include "Isotope_functions.h"
#include <ArduinoJson.h>


//JSON Definitons---------------------------------------------------------------
// Allocate the JSON document
// StaticJsonDocument<200> json_doc;
JsonDocument json_doc;
// char json_msg[] =
//       '{"type": "GET", "section": "POWER_OUTPUT", "item": 3, "value": 0}';

// https://arduinojson.org/


//Global variables--------------------------------------------------------------


//
bool handle_incoming_cmd(){
  String error_s = "0"; //Initialize variable to store ERROR
  if(Serial.available() > 0)// Is there is avaiable serial Data
  {
    const auto deser_err = deserializeJson(json_doc, Serial); //Deserialize JSON data
    if (deser_err) { //Test for errors on the JSON format
      if(Debug_flag)
      {
        Serial.print(F("Failed to deserialize, reason: \""));
        Serial.print(deser_err.c_str());
        Serial.println('"');
      }
      //Send error feedback
      send_reply("0", wrong_JSON_format);
    } else  {
      if(Debug_flag)
      {
      Serial.print(F("Recevied valid json document with "));
      Serial.print(json_doc.size());
      Serial.println(F(" elements."));
      serializeJsonPretty(json_doc, Serial);
      Serial.println();
      }

      //Brake the JSON into needed parts
      String type_s = json_doc["type"];
      String section_s = json_doc["section"];
      String item_s = json_doc["item"];
      String value_s = json_doc["value"];

      //Update the heartbeat, as comunication from the control PCB has been received
      update_comms_latency();
      //Call handle function depending on which was the request
      if(type_s.substring(0,3) == GET_S){
        if(Debug_flag)
        {
        Serial.println("Request of a GET command");
        }
        //Request is a GET command, execute it
        execute_get_cmd(section_s, item_s.toInt());
      }
      else if(type_s.substring(0,3) == SET_S){
        if(Debug_flag)
        {
        Serial.println("Request of a SET command");
        }
        //Request is a GET command, execute it
        execute_set_cmd(section_s, item_s.toInt(), value_s.toInt());
      }
      else { //If the command Type is neither a GET or SET, reply with appropiate error code
        //Commented out as its replying a wrong JSON when reading an end of line character
        //while testing with the command line
        // send_reply("0", wrong_cmd_type);

      }
    }
  }
  return 0;// No errors
}

//Handle command request functions--------------------------------------------------

//Execute GET command. Obtain the requested data and send a reply-------------------
void execute_get_cmd(String section_s, int item_i){
  String payload_s;
  if(WHO_I_AM.equals(section_s)){// Get Who_I_am request-----------------------
  send_reply(who_am_i(), CMD_ACK);
  }
  else if(POWER_OUTPUT.equals(section_s)){ //GET Power_output Request----------
    if(item_i > 2 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
    }
    else{ //Correct item range, send value
      switch (item_i) {
        case 0:
          payload_s = String(POWER_OUT_VAL_0);
          send_reply(payload_s, CMD_ACK);
          break;
        case 1:
          payload_s = String(POWER_OUT_VAL_1);
          send_reply(payload_s, CMD_ACK);
          break;
        case 2:
          payload_s = String(POWER_OUT_VAL_2);
          send_reply(payload_s, CMD_ACK);
          break;
      }
    }
  }
  else if(TEMP_SENSOR.equals(section_s)){ //GET Temp_sensor request------------
    if(item_i > 2 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
      }
    else{ //Correct item range, send value
      float tmp_val = read_temp_sensor(item_i);
      payload_s = String(tmp_val);
      send_reply(payload_s, CMD_ACK);
    }
  }
  else if(PWM_OUTPUT.equals(section_s)){ //GET PWM_Output request--------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
    }
    else{ //Correct item range, send value
      switch (item_i) {
        case 0:
          payload_s = String(PWM_OUT_VAL_0);
          send_reply(payload_s, CMD_ACK);
          break;
        case 1:
          payload_s = String(PWM_OUT_VAL_1);
          send_reply(payload_s, CMD_ACK);
          break;
        case 2:
          payload_s = String(PWM_OUT_VAL_2);
          send_reply(payload_s, CMD_ACK);
          break;
        case 3:
          payload_s = String(PWM_OUT_VAL_3);
          send_reply(payload_s, CMD_ACK);
          break;
      }
    }
  }
  else if(PWM_ENABLE.equals(section_s)){// GET PWM EN status-------------------
    if(read_pwm_en()){
      payload_s = String(1);
    }
    else{
      payload_s = String(0);
    }
    send_reply(payload_s, CMD_ACK);
  }
  else if(ANALOG_INPUT.equals(section_s)){ //GET Analog_input request----------
    if(item_i > 2 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
      }
    else{ //Correct item range, send value
      int analogue_val = read_analogue_val(item_i);
      payload_s = String(analogue_val);
      send_reply(payload_s, CMD_ACK);
    }
  }
  else if(MOTOR_RPM_SPEED.equals(section_s)){ //GET Motor_speed request------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
      }
    else{ //Correct item range, send value
      int motor_val = read_motor_rpm_speed(item_i);
      payload_s = String(motor_val);
      send_reply(payload_s, CMD_ACK);
    }
  }
  // --- Add additional commands above this line --------------------------------
  else{ // ERROR in the requested SECTION--------------------------------------
    send_reply("0", wrong_cmd_section);
  }
}

//Execute SET command. Obtain the requested data and execute ---------------------
void execute_set_cmd(String section_s, int item_i, int value_i){
  if(POWER_OUTPUT.equals(section_s)){ //Set the Power Output PWM value-----------
    if(item_i > 2 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
      }
    else{ //Correct item range, execute command
      if(value_i>1024 || value_i<0){ //Value out of range, send error
        send_reply("0", wrong_cmd_value);
      }
      else{
        set_output_power(item_i, value_i);
        send_reply("0", CMD_ACK);
      }
    }
  }
  else if(PWM_OUTPUT.equals(section_s)){//Set the PWM output value---------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
      }
    else{ //Correct item range, execute command
      if(value_i>1024 || value_i<0){ //Value out of range, send error
        send_reply("0", wrong_cmd_value);
      }
      else{
        set_pwm_output(item_i, value_i);
        send_reply("0", CMD_ACK);
      }
    }
  }
  else if(PWM_ENABLE.equals(section_s)){//Set the PWM ENABLE value---------------
    if(value_i>=1){ //If 1 or more enable, else disable
      set_pwm_en(true);
    }
    else{
      set_pwm_en(false);
    }
    send_reply("0", CMD_ACK);
  }
  else if(RGB_RED.equals(section_s)){// set the red channel value for the LED
    //Ensure value is within correct range values
    if(value_i >= 0 && value_i <256){
      set_rgb(1 , value_i);
      send_reply("0", CMD_ACK);
    }
    else{//Wrong value received
      send_reply("0", wrong_cmd_value);
    }
  }
  else if(RGB_GREEN.equals(section_s)){// set the red channel value for the LED
    //Ensure value is within correct range values
    if(value_i >= 0 && value_i <256){
      set_rgb(2 , value_i);
      send_reply("0", CMD_ACK);
    }
    else{//Wrong value received
      send_reply("0", wrong_cmd_value);
    }
  }
  else if(RGB_BLUE.equals(section_s)){// set the red channel value for the LED
    //Ensure value is within correct range values
    if(value_i >= 0 && value_i <256){
      set_rgb(3 , value_i);
      send_reply("0", CMD_ACK);
    }
  }
  else if(HEARTBEAT.equals(section_s)){//Set the heartbeat latency counter to 0---------------
      send_reply("0", CMD_ACK);
  }
  else if(MOTOR_RPM_SPEED.equals(section_s)){//Set the Motor Speed value---------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
    }
    else{
      if(value_i<=Max_motor_rpm_speed && value_i>=0){
        set_motor_rpm_speed(item_i, value_i);
        send_reply("0", CMD_ACK);
      }
      else{//Reply with wrong value (out of limits)
        send_reply("0", wrong_cmd_value);
      }
    }
  }
  else if(MOTOR_STEP.equals(section_s)){//Execute Motor steps---------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
    }
    else{
      //TODO: set a max number of steps?
      set_motor_steps(item_i, value_i);
      send_reply("0", CMD_ACK);
      }
  }
  else if (MOTOR_CURRENT_MILLIAMPS.equals(section_s)){//Set the Motor current value---------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
    }
    else{
      set_motor_current_milliamps(item_i, value_i);
      send_reply("0", CMD_ACK);
    }
  }
  else if (MOTOR_ENABLE.equals(section_s)){//Set the Motor enable value---------------
    if(item_i > 3 || item_i < 0){ //Check for error out of range
      send_reply("0", wrong_cmd_item);
    }
    set_motor_enable(item_i, value_i>=1); //If 1 or more enable, else disable
    send_reply("0", CMD_ACK);
  }
  // --- Add additional commands above this line --------------------------------
  else { // ERROR in the requested SECTION--------------------------------------
    send_reply("0", wrong_cmd_section);
  }
}

// Send repply function. Construct a JSON with the reply and send it through serial
void send_reply(String payload_s, String error_s){
  JsonDocument out_msg; //Output JSON document
  //Setup JSON sections
  out_msg["payload"] = payload_s;
  out_msg["error"] = error_s;
  serializeJson(out_msg, Serial); //Serialize JSON and send it through serial
  Serial.println();// Send an end of line since Python is waiting for it
  if(Debug_flag){ //If debug flag show the JSON message
    Serial.println("The sent JSON is");
    serializeJsonPretty(json_doc, Serial);
    Serial.println();
  }
}
