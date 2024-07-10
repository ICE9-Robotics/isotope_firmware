#ifndef ISOTOPE_PWM_H
#define ISOTOPE_PWM_H

#include <Arduino.h>
#include <Servo.h>
#include "Communication_protocol.h"

enum pwm_mode_t
{
    PWM_MODE_STANDARD,
    PWM_MODE_SERVO
};

enum pwm_value_type_t
{
    PWM_VALUE_TYPE_PWM,
    PWM_VALUE_TYPE_MS
};

class IsotopePWM
{
public:
    IsotopePWM(uint8_t pin);

    void set_mode(pwm_mode_t mode);
    bool set_control_value(int value, pwm_value_type_t value_type);
    int get_control_value();

private:
    uint8_t _pin;
    pwm_mode_t _mode;
    Servo _servo;
    int _value;
};

class IsotopePWMController
{
public:
    IsotopePWMController();
    void setup(uint8_t pins[4], uint8_t enable_pin);
    void enable();
    void disable();
    void set_enable_state(bool state);
    bool is_enabled();
    void set_mode(uint8_t port, int mode, cmd_resp_t &response);
    void set_mode(uint8_t port, pwm_mode_t mode, cmd_resp_t &response);
    void set_control_value(uint8_t port, int value, int value_type, cmd_resp_t &response);
    void set_control_value(uint8_t port, int value, pwm_value_type_t value_type, cmd_resp_t &response);
    int get_control_value(uint8_t port, cmd_resp_t &response);

private:
    IsotopePWM *pwm_ports[4];
    uint8_t _enable_pin;
    bool _enabled = true; // PWM enable by default
};

#endif
