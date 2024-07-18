#include <Arduino.h>
#include "Isotope_port_motor.h"

void IsotopeMotor::setup(uint8_t pin)
{
  _pin = pin;

  _driver.setChipSelectPin(pin);
  // Delay to allow the motor controllers to initialize
  delay(5);

  // Reset the driver settings to their default values.
  _driver.resetSettings();
  _driver.clearFaults();

  _driver.setCurrentMilliamps(1);
  _driver.setStepMode(DRV8434SStepMode::MicroStep1); // Set the number of microsteps that correspond to one full step.

  // Enable direction and step control through SPI.
  _driver.enableSPIDirection();
  _driver.enableSPIStep();

  reset();
}

bool IsotopeMotor::begin()
{
  if (_steps == 0 || !_is_enabled)
  {
    return false;
  }
  _step_remaining = abs(_steps);

  if (_steps > 0)
  {
    _driver.setDirection(0);
  }
  else
  {
    _driver.setDirection(1);
  }
  _timer.begin(_step_period, true);
  return true;
}

int IsotopeMotor::stepOnce()
{
  if (_step_remaining <= 0)
  {
    return _step_remaining;
  }
  if (_timer.fire(true))
  {
    _driver.step();
    _step_remaining--;
  }
  return _step_remaining;
}

void IsotopeMotor::enable()
{
  _driver.enableDriver();
  _is_enabled = true;
}

void IsotopeMotor::disable()
{
  _driver.disableDriver();
  _is_enabled = false;
}

void IsotopeMotor::set_current_milliamps(int current_value)
{
  _current = current_value;
  _driver.setCurrentMilliamps(current_value);
}

bool IsotopeMotor::set_steps(int steps)
{
  if (!_config_valid)
  {
    return false;
  }
  _steps = steps;
  {
    return true;
  }
}

void IsotopeMotor::set_rpm(int rpm)
{
  _rpm = rpm;
  _update_param();
}

void IsotopeMotor::set_step_angle(int step_angle)
{
  _step_angle = step_angle;
  _update_param();
}

void IsotopeMotor::_update_param()
{
  _config_valid = _rpm > 0 && _step_angle > 0;
  if (!_config_valid)
  {
    return;
  }
  _step_period = 60L * 1000L * 1000L / (_rpm * (360 / _step_angle));
  if (_busy)
  {
    _timer.update(_step_period);
  }
}

void IsotopeMotorController::setup(const uint8_t *pins)
{
  for (int i = 0; i < 4; ++i)
  {
    _motors[i] = new IsotopeMotor();
    _motors[i]->setup(pins[i]);
  }
}

void IsotopeMotorController::set_rpm(uint8_t port, int rpm, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return;
  _motors[port]->set_rpm(rpm);
  response = CMD_SUCCESS;
}

void IsotopeMotorController::set_step_angle(uint8_t port, int step_angle, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return;
  _motors[port]->set_step_angle(step_angle);
  response = CMD_SUCCESS;
}

void IsotopeMotorController::set_step(uint8_t port, int step, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return;
  if (_motors[port]->set_steps(step))
  {
    response = CMD_ACK;
  }
  else
  {
    response = HARDWARE_ERROR;
  }
  return;
}

void IsotopeMotorController::set_current_milliamps(uint8_t port, int current_value, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return;
  _motors[port]->set_current_milliamps(current_value);
  response = CMD_SUCCESS;
}

void IsotopeMotorController::set_enable(uint8_t port, bool enable_state, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return;
  if (enable_state)
  {
    _motors[port]->enable();
  }
  else
  {
    _motors[port]->disable();
  }
  response = CMD_SUCCESS;
}

int IsotopeMotorController::get_rpm(uint8_t port, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return 0;
  response = CMD_SUCCESS;
  return _motors[port]->get_rpm();
}

int IsotopeMotorController::get_step(uint8_t port, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return 0;
  response = CMD_SUCCESS;
  return _motors[port]->get_steps();
}

int IsotopeMotorController::get_current_milliamps(uint8_t port, cmd_resp_t &response)
{
  if (!_validate_port(port, response))
    return 0;
  response = CMD_SUCCESS;
  return _motors[port]->get_current_milliamps();
}

void IsotopeMotorController::step_all_once(int *remaining_steps)
{
  for (int i = 0; i < 4; ++i)
  {
    remaining_steps[i] = step_once(i);
  }
}

bool IsotopeMotorController::_validate_port(uint8_t port, cmd_resp_t &response)
{
  if (port >= 4)
  {
    response = WRONG_CMD_ITEM;
    return false;
  }
  return true;
}
