#include "Arduino.h"
#include "windchime.h"
void WindChime::init(int _motor_index, int _input_pin_1, int _input_pin_2, int _sensor_pin, int _forward_time, int _strike_type) {

  motor_index = _motor_index;
  motor_pin_1 = _input_pin_1;
  motor_pin_2 = _input_pin_2;
  sensor_pin = _sensor_pin;
  forward_time = _forward_time;
  backward_time = forward_time + 200;//give backward more time as safety check
  strike_type = _strike_type;

  pinMode(motor_pin_1, OUTPUT);
  pinMode(motor_pin_2, OUTPUT);
  pinMode(sensor_pin, INPUT);

  motor_stop();
  
  Serial.println(motor_index);

}

void WindChime::reset_striker() {

  if (digitalRead(sensor_pin)) {
    //calibration
    //Serial.println("calibration");
    motor_backward();
    while (digitalRead(sensor_pin)) {
      ;
    }
    motor_brake();
    delay(50);
    motor_stop();
  }
}

void WindChime::update() {
  /////
//    if ((millis() - strike_start_millis) >= 10000) {
//      Serial.println("strike");
//      strike();
//      strike_start_millis = millis();
//    }
  //////


  switch (motor_state) {
    case 0:
      //motor stop
      break;
    case 1:
      //motor forward

      if ((millis() - state_start_millis) >= (long)forward_time) {
        state_start_millis = millis();
        motor_brake();
        motor_state = 2;
      }

      break;
    case 2:
      //pause
      if ((millis() - state_start_millis) >= 300) { //wait time
        state_start_millis = millis();
        motor_backward();
        motor_state = 3;
      }
      break;

    case 3:
      //motor backward
//      Serial.println(millis() - state_start_millis);
      if (!digitalRead(sensor_pin)) {
        //hall effect sensor registered, striker back in place
        motor_brake();
        motor_state = 4;
      state_start_millis = millis();
      } else if ((millis() - state_start_millis) >= (long)backward_time) {
        //exceeded timeout
        
        motor_brake();
        state_start_millis = millis();
        motor_state = 4;
        //report Error
        Serial.print("windchime#");
        Serial.print(motor_index);
        Serial.println(" error");
        Serial.println(millis() - state_start_millis);
      }
      break;
    case 4:
      //motor brake
      if ((millis() - state_start_millis) >= 100) {
        motor_stop();
        motor_state = 0;
      }

      break;
    default:
      break;
  }

}


void WindChime::strike() {
  if ((millis() - previous_strike_millis) > 1000) {
    if (motor_state == 0) {
      //motor ready to strike
      state_start_millis = millis();

      if (!digitalRead(sensor_pin)) {
        Serial.println("trigger");
        motor_forward();
        motor_state = 1;
      } else {
        Serial.println("reset");
        motor_backward();
        motor_state = 3;
      }

    } else {
      Serial.println("ongoing strike");
    }

    previous_strike_millis = millis();
  }
}

void WindChime::set_type(int _type) {

}

void WindChime::update_time(unsigned int _current_minute, unsigned int _current_second) {

  current_minute = _current_minute;
  current_second = _current_second;

  switch (strike_type) {

    case 0:
      //off
      break;
    case 1:
      //every 10 seconds
      if ((current_second != previous_second) && (current_second % 10 == 0)) {
        //10 seconds since previous strike
        strike();
        strike_start_millis = millis();
      }
      break;
    case 2:
      //every 1 minute
      if (current_minute != previous_minute) {
        //1 minute since previous strike
        strike();
        strike_start_millis = millis();
      }
      break;
    case 3:
      //every 5 minutes
      if ((current_minute != previous_minute) && (current_minute % 5 == 0)) {
        //5 minutes since previous strike
        strike();
        strike_start_millis = millis();
      }
      break;
    case 4:
      //every 30 minutes
      if ((current_minute != previous_minute) && (current_minute % 30 == 0)) {
        //30 minutes since previous strike
        strike();
        strike_start_millis = millis();
      }
      break;
    case 5:
      //every 1 hour
      if ((current_minute != previous_minute) && (current_minute == 0)) {
        //60 minutes since previous strike
        strike();
        strike_start_millis = millis();
      }
      break;
    default:
      break;
  }

  previous_minute = current_minute;
  previous_second = current_second;

}

void WindChime::motor_forward() {
  //Serial.println("forward");
  digitalWrite(motor_pin_1, HIGH);
  digitalWrite(motor_pin_2, LOW);
}

void WindChime::motor_backward() {
  //Serial.println("backward");
  digitalWrite(motor_pin_1, LOW);
  digitalWrite(motor_pin_2, HIGH);
}

void WindChime::motor_brake() {
  digitalWrite(motor_pin_1, HIGH);
  digitalWrite(motor_pin_2, HIGH);
}

void WindChime::motor_stop() {
  digitalWrite(motor_pin_1, LOW);
  digitalWrite(motor_pin_2, LOW);
}
