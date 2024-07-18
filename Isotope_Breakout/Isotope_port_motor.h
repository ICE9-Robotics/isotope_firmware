#ifndef ISOTOPE_PORT_MOTOR_H
#define ISOTOPE_PORT_MOTOR_H

#include <DRV8434S.h>
#include <FireTimer.h>
#include "Communication_protocol.h"

class IsotopeMotor
{
public:
  void setup(uint8_t pin);

  bool begin();
  int stepOnce();

  inline void reset()
  {
    _steps = 0;
    _step_remaining = 0;
    _busy = false;
  }

  void enable();
  void disable();
  void set_current_milliamps(int current_value);

  bool set_steps(int steps);
  void set_rpm(int rpm);
  void set_step_angle(int step_angle);

  inline int get_rpm() { return _rpm; }
  inline int get_steps() { return _steps; }
  inline int get_step_period() { return _step_period; }
  inline int get_current_milliamps() { return _current; }
  inline bool is_busy() { return _busy; }

private:
  DRV8434S _driver;
  FireTimer _timer;
  bool _busy;
  bool _is_enabled;
  int _step_remaining;

  uint8_t _pin;
  int _rpm;
  int _step_angle;
  bool _config_valid = false;
  int _direction;
  int _steps;
  int _step_period;
  int _current;

  void _update_param();
};

class IsotopeMotorController
{
public:
  void setup(const uint8_t *pins);
  void set_rpm(uint8_t port, int rpm, cmd_resp_t &response);
  void set_step_angle(uint8_t port, int step_angle, cmd_resp_t &response);
  void set_step(uint8_t port, int step_value, cmd_resp_t &response);
  void set_current_milliamps(uint8_t port, int current_value, cmd_resp_t &response);
  void set_enable(uint8_t port, bool enable_state, cmd_resp_t &response);

  int get_rpm(uint8_t port, cmd_resp_t &response);
  int get_step(uint8_t port, cmd_resp_t &response);
  int get_current_milliamps(uint8_t port, cmd_resp_t &response);

  inline bool begin(uint8_t port) { return _motors[port]->begin(); }
  inline bool is_busy(uint8_t port) { return _motors[port]->is_busy(); }
  inline int step_once(uint8_t port) { return _motors[port]->stepOnce(); }
  void step_all_once(int *remaining_steps);

private:
  IsotopeMotor *_motors[4];
  bool _validate_port(uint8_t port, cmd_resp_t &response);
};

#endif
