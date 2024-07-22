#ifndef ISOTOPE_MOTOR_MANAGER_H
#define ISOTOPE_MOTOR_MANAGER_H

#include "Communication_protocol.h"

struct MotorTask
{
  int seq = 0;
  int current_seq = 0;
  bool completed = true;
  bool started = true;
};

void abort_motor_task(int port_id);

void complete_motor_task(int port_id);

void add_motor_task(int seq, int port_id, int steps);

void motor_spin_once();

bool is_motor_busy(int port_id, cmd_resp_t &response);

#endif
