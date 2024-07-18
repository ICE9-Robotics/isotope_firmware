#include "Isotope_port_temp.h"
#include "Communication_protocol.h"

void request_temp_sensor(int seq, int item)
{
  switch (item)
  {
  case 0:
    Temp_sensor_0.requestTemp();
    Serial.println("Requested");
    break;
  case 1:
    Temp_sensor_1.requestTemp();
    break;
  case 2:
    Temp_sensor_2.requestTemp();
    break;
  }
  temperature_data[item].sequence = seq;
}

void handle_temp_sensor()
{
  if (temperature_data[0].sequence >= 0 && Temp_sensor_0.readTemp())
  {
    temperature_data[0].temp = Temp_sensor_0.getTemp();
    send_temp_data(0);
    reset_temp_data(0);
  }
  if (temperature_data[1].sequence >= 0 && Temp_sensor_1.readTemp())
  {
    temperature_data[1].temp = Temp_sensor_1.getTemp();
    send_temp_data(1);
    reset_temp_data(1);
  }
  if (temperature_data[2].sequence >= 0 && Temp_sensor_2.readTemp())
  {
    temperature_data[2].temp = Temp_sensor_2.getTemp();
    send_temp_data(2);
    reset_temp_data(2);
  }
}

void reset_temp_data(int item)
{
  temperature_data[item].sequence = -1;
  temperature_data[item].temp = -1;
}

void send_temp_data(int item)
{
  send_reply(temperature_data[item].sequence, temperature_data[item].temp, CMD_SUCCESS_S); 
}
