#include "RTClib.h"
#include "windchime.h"

//RTC defines
RTC_DS3231 rtc;
#define RTC_INTERVAL 1000 //check RTC every 1 second
unsigned long check_rtc_millis = 0;

//WindChime defines
#define NUM_WINDCHIMES 7  //total number of windchimes
#define STRIKE_MILLIS 290 //millis of motor moving forward each strike

WindChime windchimes[7];  //instance of 7 windchimes

//motor driver input pin1
const int motor_pin_forward[] = {11, 8, 5, 24, 30, 36, 42};

//motor driver input pin2
const int motor_pin_backward[] = {10, 7, 4, 26, 32, 38, 44};

//strike direction, various on different wooden bases
const int strike_direction[] = {1, 1, 0, 0, 1, 0, 1}; //0: strike counter-clock wise, 1: strike  clock wise

//hall effect sensor input keypad
const int sensor_pin[] = {12, 9, 6, 22, 28, 34, 40};

uint8_t inBytes[8];


/*********************************ADJUSTABLE VARIABLES BEGIN HERE*********************************/
//strike forward time in millis 
const int strike_millis[] = {185, 210, 200, 200, 215, 160, 180}; //change the values here to adjust strike travel distance

//strike frequency type
// windchime -> 7 6 5 4 3 2 1 
const int strike_type[] = {2, 3, 3, 4, 4, 5, 5};//change the values here to change striking frequency

/***STRIKE TYPE***
  0: off
  1: every 10 seconds
  2: every 1 minute
  3: every 5 minutes
  4: every 30 minutes
  5: every 1 hour
*/

/*********************************ADJUSTABLE VARIABLES END HERE*********************************/

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("begin");



  //initiate RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //initiate WindChimes
  for (int i = 0; i < NUM_WINDCHIMES; i++) {
    if (strike_direction[i] == 1) {
      windchimes[i].init(i + 1, motor_pin_forward[i], motor_pin_backward[i], sensor_pin[i], strike_millis[i], strike_type[i]);
    } else {
      windchimes[i].init(i + 1, motor_pin_backward[i], motor_pin_forward[i], sensor_pin[i], strike_millis[i], strike_type[i]);
    }
    /*********************************ADJUSTABLE VARIABLES BEGIN HERE*********************************/

    //windchimes[i].reset_striker(); //uncomment this to run calibration function during setup

    /*********************************ADJUSTABLE VARIABLES END HERE*********************************/

  }

}

void loop() {
  //check RTC time
  if (millis() - check_rtc_millis >= RTC_INTERVAL) {
    DateTime now = rtc.now();
    unsigned int current_minute = now.minute();
    unsigned int current_second = now.second();
    Serial.print(current_minute);
    Serial.print(":");
    Serial.println(current_second);
    for (int i = 0; i < NUM_WINDCHIMES; i++) {
      windchimes[i].update_time(current_minute, current_second);
    }
    check_rtc_millis = millis();
  }

  for (int i = 0; i < NUM_WINDCHIMES; i++) {
    windchimes[i].update();
  }


}

void serialEvent() {
  if (Serial.available()) {
    Serial.readBytes(inBytes, 8);
    switch (inBytes[0]) {
      case 'A':
        if (inBytes[1] == '0') {
          switch (inBytes[2]) {
            case '0':
              for (int i = 0; i < NUM_WINDCHIMES; i++) {
                windchimes[i].motor_stop();
              }
              break;
            case '1':
              for (int i = 0; i < NUM_WINDCHIMES; i++) {
                windchimes[i].strike();
              }
              break;
            case '2':
              for (int i = 0; i < NUM_WINDCHIMES; i++) {
                windchimes[i].strike();
              }
              break;
            default:
              break;
          }

        }
        break;
      case 'B':
        if (inBytes[1] > 0 && inBytes[1] <= 7) {
          windchimes[(int)inBytes[1]].set_type((int)inBytes[2]);
        }
        break;
      case 'C':
        break;
      default:
        break;

    }
  }
}

void serialEvent1() {
  if (Serial1.available()) {
    Serial1.readBytes(inBytes, 8);
    Serial.println(inBytes[0]);
    switch (inBytes[0]) {
      case 'A':
        if (inBytes[1] == '0') {
          switch (inBytes[2]) {
            case '0':
              Serial.println("all reset");
              for (int i = 0; i < NUM_WINDCHIMES; i++) {
                windchimes[i].reset_striker();
              }
              break;
            case '1':
              Serial.println("all strike");
              for (int i = 0; i < NUM_WINDCHIMES; i++) {
                windchimes[i].strike();
              }
              break;
            case '2':
              Serial.println("all stop");
              for (int i = 0; i < NUM_WINDCHIMES; i++) {
                windchimes[i].motor_stop();
              }
              break;
            default:
              break;
          }

        }
        break;
      case 'B':
        int index = inBytes[1] - 48; //ASCII to int
        index = index - 1; //shift 1-7 to 0-6
        if (index >= 0 && index < 7) {
          Serial.print("windchime");
          Serial.print(index + 1);
          Serial.println(" strike");
          windchimes[index].strike();
        }
        break;
      case 'C':
        if (inBytes[1] > 0 && inBytes[1] <= 7) {
          windchimes[(int)inBytes[1]].set_type((int)inBytes[2]);
        }
        break;
      default:
        break;

    }

  }
}
