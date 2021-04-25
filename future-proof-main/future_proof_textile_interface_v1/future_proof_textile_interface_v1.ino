#define LEDPIN1 3
#define LEDPIN2 4
#define LEDPIN3 6
#define LEDPIN4 8
#define LEDPIN5 2
#define LEDPIN6 5
#define LEDPIN7 7
#define LEDPIN8 9

#define BUTTON8 15


/*********************************************************
  This is a library for the MPR121 12-channel Capacitive touch sensor

  Designed specifically to work with the MPR121 Breakout in the Adafruit shop
  ----> https://www.adafruit.com/products/

  These sensors use I2C communicate, at least 2 pins are required
  to interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

//communication variables
bool trigger_flag = false; //this flag will be true when an interrupt occurred
int trigger_pin = 0; //this int captures the pin number of the button triggered
unsigned long trigger_time = 0; //this long captures the current time when it's triggered

bool sleep_flag = false;
const long interval = 1000;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  pinMode(LEDPIN4, OUTPUT);
  pinMode(LEDPIN5, OUTPUT);
  pinMode(LEDPIN6, OUTPUT);
  pinMode(LEDPIN7, OUTPUT);
  pinMode(LEDPIN8, OUTPUT);

  digitalWrite(LEDPIN1, HIGH);
  digitalWrite(LEDPIN2, HIGH);
  digitalWrite(LEDPIN3, HIGH);
  digitalWrite(LEDPIN4, HIGH);
  digitalWrite(LEDPIN5, HIGH);
  digitalWrite(LEDPIN6, HIGH);
  digitalWrite(LEDPIN7, HIGH);
  digitalWrite(LEDPIN8, HIGH);

  attachInterrupt(digitalPinToInterrupt(BUTTON8), button8_interrupt, FALLING);

  Serial.println("Adafruit MPR121 Capacitive Touch sensor");

  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  delay(500);
  digitalWrite(LEDPIN1, LOW);
  digitalWrite(LEDPIN2, LOW);
  digitalWrite(LEDPIN3, LOW);
  digitalWrite(LEDPIN4, LOW);
  digitalWrite(LEDPIN5, LOW);
  digitalWrite(LEDPIN6, LOW);
  digitalWrite(LEDPIN7, LOW);
  digitalWrite(LEDPIN8, LOW);

  sleep_flag = true;
}

void loop() {

  // Get the currently touched pads
  currtouched = cap.touched();

  for (uint8_t i = 0; i < 12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");

      if (sleep_flag) {

        switch (i) {
          case 0:
            digitalWrite(LEDPIN4, HIGH);
            trigger_flag = true;
            trigger_pin = 4;
            break;
          case 1:
            digitalWrite(LEDPIN3, HIGH);
            trigger_flag = true;
            trigger_pin = 3;
            break;
          case 3:
            digitalWrite(LEDPIN2, HIGH);
            trigger_flag = true;
            trigger_pin = 2;
            break;
          case 5:
            digitalWrite(LEDPIN1, HIGH);
            trigger_flag = true;
            trigger_pin = 1;
            break;
          case 6:
            digitalWrite(LEDPIN7, HIGH);
            trigger_flag = true;
            trigger_pin = 7;
            break;
          case 8:
            digitalWrite(LEDPIN6, HIGH);
            trigger_flag = true;
            trigger_pin = 6;
            break;
          case 10:
            digitalWrite(LEDPIN5, HIGH);
            trigger_flag = true;
            trigger_pin = 5;
            break;
          default:
            break;
        }
      }
      break;
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
    }
  }

  // reset our state
  lasttouched = currtouched;

  //communication
  if (trigger_flag) {
    Serial.println("trigger");
    trigger_time = millis();

    if (trigger_pin < 8 && trigger_pin > 0) {
      Serial.print('B');
      Serial.print(trigger_pin);
      Serial.println('1');

      Serial1.print('B');
      Serial1.print(trigger_pin);
      Serial1.println('1');
    } else if (trigger_pin == 8 ) {
      //trigger pin 8 is for all windchimes
      Serial.print('A');
      Serial.print('0');
      Serial.println('0');

      Serial1.print('A');
      Serial1.print('0');
      Serial1.println('0');
    }
    sleep_flag = false;
    trigger_flag = false;
  }

  if (sleep_flag) {
    //enter sleep mode
  } else {
    if ((millis() - trigger_time) >= interval) {

      //turn off
      digitalWrite(LEDPIN1, LOW);
      digitalWrite(LEDPIN2, LOW);
      digitalWrite(LEDPIN3, LOW);
      digitalWrite(LEDPIN4, LOW);
      digitalWrite(LEDPIN5, LOW);
      digitalWrite(LEDPIN6, LOW);
      digitalWrite(LEDPIN7, LOW);
      digitalWrite(LEDPIN8, LOW);
      sleep_flag = true;
    }
  }
}

void button8_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN8, HIGH);
    trigger_flag = true;
    trigger_pin = 8;
  }
}
