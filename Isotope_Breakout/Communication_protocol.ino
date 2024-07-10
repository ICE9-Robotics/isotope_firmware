/*
  Isotope Breakout V0 firmware

  Comms Protocol for the Isotope Breakout V0 board

  This code requieres the use of ArduinoJson library,
  install it directly from the Arduino IDE.

*/
// Libraries---------------------------------------------------------------------
#include "Isotope_breakout_defs.h"
#include "Communication_protocol.h"
#include "Isotope_functions.h"
#include <ArduinoJson.h>
#include <Arduino.h>

// JSON Definitons---------------------------------------------------------------
//  Allocate the JSON document
JsonDocument json_doc;
// char json_msg[] =
//       '{"type": "GET", "section": "POWER_OUTPUT", "item": 3, "value": [0, 0]}';
// Serial commands for test.
// {"type": "SET", "section": "PWM_enable", "item": 0, "value": [1,0]}
// {"type": "SET", "section": "PWM_mode", "item": 0, "value": [1,0]}
// {"type": "SET", "section": "PWM_output", "item": 0, "value": [180,0]}

// https://arduinojson.org/

bool validate_cmd_value(int value, int min, int max)
{
  if (value >= min && value <= max)
  {
    return true;
  }
  else
  {
    send_reply("0", WRONG_CMD_VALUE_S);
    return false;
  }
}

bool validate_item_value(int item, int min, int max)
{
  if (item >= min && item <= max)
  {
    return true;
  }
  else
  {
    send_reply("0", WRONG_CMD_ITEM_S);
    return false;
  }
}

bool handle_incoming_cmd()
{
  String error_s = "0";       // Initialize variable to store ERROR
  if (Serial.available() > 0) // Is there is avaiable serial Data
  {
    const auto deser_err = deserializeJson(json_doc, Serial); // Deserialize JSON data
    if (deser_err)
    { // Test for errors on the JSON format
      if (Debug_flag)
      {
        Serial.print(F("Failed to deserialize, reason: \""));
        Serial.print(deser_err.c_str());
        Serial.println('"');
      }
      // Send error feedback
      send_reply("0", WRONG_JSON_FORMAT_S);
    }
    else
    {
      if (Debug_flag)
      {
        Serial.print(F("Recevied valid json document with "));
        Serial.print(json_doc.size());
        Serial.println(F(" elements."));
        serializeJsonPretty(json_doc, Serial);
        Serial.println();
      }

      // Break the JSON into needed parts
      String type_s = json_doc["type"];
      String section_s = json_doc["section"];
      int item_i = json_doc["item"];

      // Update the heartbeat, as comunication from the control PCB has been received
      update_comms_latency();
      // Call handle function depending on which was the request
      if (type_s.substring(0, 3) == GET_S)
      {
        if (Debug_flag)
        {
          Serial.println("Request of a GET command");
        }
        // Request is a GET command, execute it
        execute_get_cmd(section_s, item_i);
      }
      else if (type_s.substring(0, 3) == SET_S)
      {
        if (Debug_flag)
        {
          Serial.println("Request of a SET command");
        }
        // Request is a SET command, execute it
        int value_a[2];
        value_a[0] = json_doc["value"][0];
        value_a[1] = json_doc["value"][1];
        execute_set_cmd(section_s, item_i, value_a);
      }
      else
      {
      }
    }
  }
  return 0; // No errors
}

// Handle command request functions--------------------------------------------------

// Execute GET command. Obtain the requested data and send a reply-------------------
void execute_get_cmd(String section_s, int item_i)
{
  String payload_s;
  cmd_resp_t response = OK;
  if (WHO_I_AM == section_s)
  {
    payload_s = who_am_i();
  }
  else if (POWER_OUTPUT == section_s)
  {
    if (!validate_item_value(item_i, 0, 2))
    {
      return;
    }
    switch (item_i)
    {
    case 0:
      payload_s = String(power_out_val_0);
      break;
    case 1:
      payload_s = String(power_out_val_1);
      break;
    case 2:
      payload_s = String(power_out_val_2);
      break;
    }
  }
  else if (TEMP_SENSOR == section_s)
  {
    if (!validate_item_value(item_i, 0, 2))
    {
      return;
    }
    payload_s = String(read_temp_sensor(item_i));
  }
  else if (PWM_OUTPUT == section_s)
  {
    if (!validate_item_value(item_i, 0, 3))
    {
      return;
    }
    payload_s = String(pwm_controller.get_control_value(item_i, response));
  }
  else if (PWM_ENABLE == section_s)
  {
    payload_s = String(int(pwm_controller.is_enabled()));
  }
  else if (ANALOG_INPUT == section_s)
  {
    if (!validate_item_value(item_i, 0, 2))
    {
      return;
    }
    payload_s = String(read_analogue_val(item_i));
  }
  else if (MOTOR_RPM_SPEED == section_s)
  {
    if (!validate_item_value(item_i, 0, 3))
    {
      return;
    }
    payload_s = String(read_motor_rpm_speed(item_i));
  }

  // --- Add additional commands above this line --------------------------------
  else
  { // ERROR in the requested SECTION--------------------------------------
    send_reply("0", WRONG_CMD_SECTION_S);
    return;
  }

  String response_s = get_cmd_resp_string(response);
  send_reply(payload_s, response_s);
}

// Execute SET command. Obtain the requested data and execute ---------------------
void execute_set_cmd(String section_s, int item_i, int *value_a)
{
  cmd_resp_t response = OK;
  if (POWER_OUTPUT == section_s)
  {
    if (!validate_item_value(item_i, 0, 2) || !validate_cmd_value(value_a[0], 0, 1024))
    {
      return;
    }
    set_output_power(item_i, value_a[0]);
  }
  else if (PWM_OUTPUT == section_s)
  {
    pwm_controller.set_control_value(item_i, value_a[0], value_a[1], response);
  }
  else if (PWM_ENABLE == section_s)
  {
    pwm_controller.set_enable_state(value_a[0] >= 1);
  }
  else if (PWM_MODE == section_s)
  {
    pwm_controller.set_mode(item_i, value_a[0], response);
  }
  else if (RGB == section_s)
  {
    if (!validate_cmd_value(value_a[0], 0, 255) || !validate_cmd_value(value_a[1], 0, 255) || !validate_cmd_value(value_a[2], 0, 255))
    {
      return;
    }
    set_rgb(value_a);
  }
  else if (HEARTBEAT == section_s)
  {
    // do nothing
  }
  else if (MOTOR_RPM_SPEED == section_s)
  {
    if (!validate_item_value(item_i, 0, 3) || !validate_cmd_value(value_a[0], 0, Max_motor_rpm_speed))
    {
      return;
    }
    set_motor_rpm_speed(item_i, value_a[0]);
  }
  else if (MOTOR_STEP == section_s)
  {
    if (!validate_item_value(item_i, 0, 3))
    {
      return;
    }
    set_motor_steps(item_i, value_a[0]);
  }
  else if (MOTOR_CURRENT_MILLIAMPS == section_s)
  {
    if (!validate_item_value(item_i, 0, 3))
    {
      return;
    }
    set_motor_current_milliamps(item_i, value_a[0]);
  }
  else if (MOTOR_ENABLE == section_s)
  {
    if (!validate_item_value(item_i, 0, 3))
    {
      return;
    }
    set_motor_enable(item_i, value_a[0] >= 1);
  }

  // --- Add additional commands above this line --------------------------------
  else
  { // ERROR in the requested SECTION
    send_reply("0", WRONG_CMD_SECTION_S);
    return;
  }

  String response_s = get_cmd_resp_string(response);
  send_reply("0", response_s);
}

// Send repply function. Construct a JSON with the reply and send it through serial
void send_reply(String payload_s, String error_s)
{
  JsonDocument out_msg; // Output JSON document
  // Setup JSON sections
  out_msg["payload"] = payload_s;
  out_msg["error"] = error_s;
  serializeJson(out_msg, Serial); // Serialize JSON and send it through serial
  Serial.println();               // Send an end of line since Python is waiting for it
  if (Debug_flag)
  { // If debug flag show the JSON message
    Serial.println("The sent JSON is");
    serializeJsonPretty(json_doc, Serial);
    Serial.println();
  }
}

String get_cmd_resp_string(cmd_resp_t resp)
{
  switch (resp)
  {
  case OK:
    return CMD_ACK;
  case WRONG_JSON_FORMAT:
    return WRONG_JSON_FORMAT_S;
  case WRONG_CMD_TYPE:
    return WRONG_CMD_TYPE_S;
  case WRONG_CMD_SECTION:
    return WRONG_CMD_SECTION_S;
  case WRONG_CMD_ITEM:
    return WRONG_CMD_ITEM_S;
  case WRONG_CMD_VALUE:
    return WRONG_CMD_VALUE_S;
  default:
    return UNKNOWN_RESP_S;
  }
}