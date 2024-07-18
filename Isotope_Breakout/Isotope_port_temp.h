#ifndef _ISOTOPE_PORT_TEMP_H_
#define _ISOTOPE_PORT_TEMP_H_

#include "Communication_protocol.h"

struct TemperatureData
{
  int sequence = -1;
  float temp = -1;
};

TemperatureData temperature_data[3];

void request_temp_sensor(int seq, int item);

void handle_temp_sensor();

void reset_temp_data(int item);

void send_temp_data(int item);

#endif
