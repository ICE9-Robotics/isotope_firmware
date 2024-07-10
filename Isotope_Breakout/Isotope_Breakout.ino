/*
  Isotope Breakout V0 firmware

  Code for the Isotope Breakout V0. The following features are implemented:
- Find board by reading ID value
- Turn Power Outputs On/Off
- Read Temperature Sensors
- Set and read PWM values
- Read Analogue inputs
- Set motor speed (To control magnetic stirrer)

  Libraries needed to be downloaded from the Libraries Manager in Arduino:
  - ArduinoJson
  - microDS18B20 - To communicate with the DS18B20 temperature sensors
  - DRV8434S - Stepper motor library by Pololu

*/
//Libaries needed---------------------------------------------------------------
#include <ArduinoJson.h>
#include <microDS18B20.h> // For the DS18B20 temperature sensor
#include <SPI.h>
#include <DRV8434S.h> //Stepper motor driver Library
#include <Adafruit_NeoPixel.h>// to control RGB led on board

#include "Isotope_breakout_defs.h"
#include "Communication_protocol.h"
#include "Isotope_port_pwm.h"
#include "Isotope_port_motor.h"

// Constructors-----------------------------------------------------------------
//Temp sensors, class, pin, object
MicroDS18B20<Temp_In_0> Temp_sensor_0;
MicroDS18B20<Temp_In_1> Temp_sensor_1;
MicroDS18B20<Temp_In_2> Temp_sensor_2;

// RGB LED
Adafruit_NeoPixel rgb(1, RGB_LED, NEO_GRB + NEO_KHZ800);

//Variable definition-----------------------------------------------------------
IsotopePWMController pwm_controller;
IsotopeMotorController motor_controller;

//FLAGs-------------------------------------------------------------------------
//Global flag for debug. Only use if the Isotope Board is going to be used from a terminal
bool Debug_flag = false;

//Setup-------------------------------------------------------------------------
void setup() {
  //Show start of initialization process
  pinMode(RGB_LED,       OUTPUT);
  //initialize rgb
  rgb.begin();
  rgb.clear();
  rgb.setPixelColor(0, rgb.Color(20, 0, 20));
  rgb.show();

  //Initialize SPI communications
  SPI.begin();

  //Start comms
  Serial.begin(115200);
  if(Debug_flag){
    Serial.print(">>Initializing Isotope Breakout firmware version ");
    Serial.println(FIRMWARE_VERSION);
  }
  //Pin Setup, set as outputs
  //Power outputs
  pinMode(POWER_OUT_0, OUTPUT);
  pinMode(POWER_OUT_1, OUTPUT);
  pinMode(POWER_OUT_2, OUTPUT);
  //PWM Outputs
  pinMode(PWM_EN,      OUTPUT);
  pinMode(PWM_0,       OUTPUT);
  pinMode(PWM_1,       OUTPUT);
  pinMode(PWM_2,       OUTPUT);
  pinMode(PWM_3,       OUTPUT);
  //Motor Controller Chip-Selects
  pinMode(SPI_CS_0,    OUTPUT);
  pinMode(SPI_CS_1,    OUTPUT);
  pinMode(SPI_CS_2,    OUTPUT);
  pinMode(SPI_CS_3,    OUTPUT);

  //Pin  Setup, as inputs
  //Analogue Inputs
  pinMode(Analog_In_0,  INPUT);
  pinMode(Analog_In_1,  INPUT);
  pinMode(Analog_In_2,  INPUT);
  //Temp Inputs
  pinMode(Temp_In_0,    INPUT);
  pinMode(Temp_In_1,    INPUT);
  pinMode(Temp_In_2,    INPUT);
  //Motor controllers Inputs
  pinMode(DRV_FAULT_0,  INPUT);
  pinMode(DRV_FAULT_1,  INPUT);
  pinMode(DRV_FAULT_2,  INPUT);
  pinMode(DRV_FAULT_3,  INPUT);

  //Turn Off all PWMs
  digitalWrite(PWM_EN,HIGH);

  //Turn Off all power outputs
  digitalWrite(POWER_OUT_0,LOW);
  digitalWrite(POWER_OUT_1,LOW);
  digitalWrite(POWER_OUT_2,LOW);

  //Initialize port instances
  uint8_t pwm_port_pins[4] = {PWM_0, PWM_1, PWM_2, PWM_3};
  pwm_controller.setup(pwm_port_pins, PWM_EN);
  uint8_t motor_cs_pins[4] = {SPI_CS_0, SPI_CS_1, SPI_CS_2, SPI_CS_3};
  motor_controller.setup(motor_cs_pins);

  //Setup PWM characteristics
  // analogWriteFreq(uint32_t frequency);
  analogWriteRange(1024);
  // analogWriteResolution (int resolution);

  //Setup ADC resolution
  analogReadResolution(10);// Set ADC resolution to 10 bits (max value of 1024)

  //TODO: Look for temp sensors
  if(Debug_flag){
    Serial.println(">>Initialization finished. No errors in initialization.");
  }
  //Add delay so initialization process can be seen
  delay(500);
  //Show end if initialization
  rgb.clear();
  rgb.show();

}


//Global commns structure variable
CMD_t cmd;
//Global time variables
unsigned long current_time;
unsigned long last_measure_time=0;

void loop() {
  //Get current time
  current_time = millis();
  //Take current measurements only after a constant amount of time
//   if( (current_time -last_measure_time) >= Power_sampling_period){
//     //Update last_measure_time
//     last_measure_time = current_time;
//     //Take a power sample and add it to the buffer
//     power_sample();
//   }
  //Receive, decode and execute commands
  handle_incoming_cmd();
  //Check for Comms latency. This feature is necessary to ensure the Isotope Board is
  //in constant communication with the control PC and if its not, to set all the control
  // outputs to 0
  check_comms_latency();
  motor_controller.step_once();
}
