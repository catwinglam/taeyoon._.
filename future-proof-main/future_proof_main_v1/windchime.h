#ifndef windchime_h
#define windchime_h

#include "Arduino.h"

class WindChime
{
  public:
    void init(int _motor_index, int _input_pin_1, int _input_pin_2, int _sensor_pin, int _forward_time, int _strike_frequency);
    void reset_striker();
    void update();
    void strike();
    void motor_stop();
    void motor_brake();
    void set_type(int _type);
    void update_time(unsigned int _current_minute, unsigned int _current_second);
  private:
    void motor_forward();
    void motor_backward();

    int motor_index;
    int motor_pin_1;
    int motor_pin_2;
    int sensor_pin;
    int forward_time;
    int backward_time;
    int strike_type;
    unsigned int current_second;
    unsigned int current_minute;
    unsigned int previous_second;
    unsigned int previous_minute;

    int motor_state = 0; //0:stop, 1: forward, 2: backward

    unsigned long strike_start_millis = 0;
    unsigned long state_start_millis;
    unsigned long previous_strike_millis = 0;
};

#endif
