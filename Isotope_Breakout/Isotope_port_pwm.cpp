#include "Isotope_port_pwm.h"

IsotopePWM::IsotopePWM(uint8_t pin)
{
    _pin = pin;
}

void IsotopePWM::set_mode(pwm_mode_t mode)
{
    _mode = mode;
    if (_mode == PWM_MODE_SERVO)
    {
        _servo.attach(_pin);
    }
    else
    {
        _servo.detach();
        pinMode(_pin, OUTPUT);
    }
}

int IsotopePWM::get_control_value()
{
    return _value;
}

bool IsotopePWM::set_control_value(int value, pwm_value_type_t value_type)
{
    if (_mode == PWM_MODE_STANDARD && value_type == PWM_VALUE_TYPE_PWM)
    {
        if (value < 0 || value > 1024)
        {
            return false;
        }
        analogWrite(_pin, value);
    }
    else if (_mode == PWM_MODE_SERVO && value_type == PWM_VALUE_TYPE_PWM)
    {
        if (value < 0 || value > 180)
        {
            return false;
        }
        _servo.write(value);
    }
    else if (_mode == PWM_MODE_SERVO && value_type == PWM_VALUE_TYPE_MS)
    {
        if (value < 0)
        {
            return false;
        }
        _servo.writeMicroseconds(value);
    }
    else
    {
        return false;
    }
    _value = value;
    return true;
}

IsotopePWMController::IsotopePWMController() 
{
}

void IsotopePWMController::setup(uint8_t pins[4], uint8_t enable_pin)
{
    _enable_pin = enable_pin;
    for (int i = 0; i < 4; ++i)
    {
        pwm_ports[i] = new IsotopePWM(pins[i]);
    }
}

void IsotopePWMController::enable()
{
    digitalWrite(_enable_pin, LOW);
    _enabled = true;
}

void IsotopePWMController::disable()
{
    digitalWrite(_enable_pin, HIGH);
    _enabled = false;
}

void IsotopePWMController::set_enable_state(bool state)
{
    if (state)
    {
        enable();
    }
    else
    {
        disable();
    }
}

bool IsotopePWMController::is_enabled()
{
    return _enabled;
}

void IsotopePWMController::set_mode(uint8_t port, int mode, cmd_resp_t &response)
{
    if (mode < 0 || mode > 1)
    {
        response = WRONG_CMD_VALUE;
        return;
    }
    return set_mode(port, (pwm_mode_t)mode, response);
}

void IsotopePWMController::set_mode(uint8_t port, pwm_mode_t mode, cmd_resp_t &response)
{
    if (port >= 4)
    {
        response = WRONG_CMD_ITEM;
        return;
    }
    pwm_ports[port]->set_mode(mode);
    response = OK;
}

void IsotopePWMController::set_control_value(uint8_t port, int value, int value_type, cmd_resp_t &response)
{
    if (value_type < 0 || value_type > 1)
    {
        response = WRONG_CMD_VALUE;
        return;
    }
    return set_control_value(port, value, (pwm_value_type_t)value_type, response);
}

void IsotopePWMController::set_control_value(uint8_t port, int value, pwm_value_type_t value_type, cmd_resp_t &response)
{
    if (port >= 4)
    {
        response = WRONG_CMD_ITEM;
        return;
    }
    if (!pwm_ports[port]->set_control_value(value, value_type))
    {
        response = WRONG_CMD_VALUE;
        return;
    }
    response = OK;
}

int IsotopePWMController::get_control_value(uint8_t port, cmd_resp_t &response)
{
    if (port >= 4)
    {
        response = WRONG_CMD_ITEM;
        return -1;
    }
    response = OK;
    return pwm_ports[port]->get_control_value();
}
