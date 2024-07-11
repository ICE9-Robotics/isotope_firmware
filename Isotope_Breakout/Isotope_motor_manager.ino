#include "Isotope_breakout_defs.h"
#include "Isotope_motor_manager.h"

MotorTask motor_tasks[4];

void abort_motor_task(int port_id)
{
  if (motor_tasks[port_id].completed)
  {
    return;
  }
  send_reply(motor_tasks[port_id].seq, port_id, CMD_ABORT_S);
  motor_tasks[port_id].completed = true;
}

void complete_motor_task(int port_id)
{
  send_reply(motor_tasks[port_id].seq, port_id, CMD_SUCCESS_S);
  motor_tasks[port_id].completed = true;
}

void add_motor_task(int seq, int port_id, int steps)
{
  abort_motor_task(port_id);
  motor_tasks[port_id].seq = seq;
  motor_tasks[port_id].completed = false;
  motor_tasks[port_id].current_seq = -1;
}

int remaining_steps = 0;
void motor_spin_once()
{
  for (int i = 0; i < 4; i ++)
  {
    if (motor_tasks[i].completed)
    {
      continue;
    }
    if (motor_tasks[i].current_seq != motor_tasks[i].seq)
    { // a new task
      if (! motor_controller.begin(i))
      { // fail to begin the new task, due to zero steps or motor being disabled.
        delay(1);
        abort_motor_task(i);
        continue;
      }
      motor_tasks[i].current_seq = motor_tasks[i].seq;
    }
    remaining_steps = motor_controller.step_once(i);

    if (remaining_steps <= 0)
    {
      complete_motor_task(i);
    }
  }
}