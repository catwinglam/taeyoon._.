#define BUTTON1 14
#define BUTTON2 10
#define BUTTON3 11
#define BUTTON4 12
#define BUTTON5 18
#define BUTTON6 17
#define BUTTON7 16
#define BUTTON8 15

#define LEDPIN1 3
#define LEDPIN2 4
#define LEDPIN3 6
#define LEDPIN4 8
#define LEDPIN5 2
#define LEDPIN6 5
#define LEDPIN7 7
#define LEDPIN8 9

bool trigger_flag = false; //this flag will be true when an interrupt occurred
int trigger_pin = 0; //this int captures the pin number of the button triggered
unsigned long trigger_time = 0; //this long captures the current time when it's triggered

bool sleep_flag = false;
const long interval = 1000;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  //attach interrupt to button pins
  attachInterrupt(digitalPinToInterrupt(BUTTON1), button1_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON2), button2_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON3), button3_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON4), button4_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON5), button5_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON6), button6_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON7), button7_interrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON8), button8_interrupt, FALLING);

  pinMode(LEDPIN1, OUTPUT);
  pinMode(LEDPIN2, OUTPUT);
  pinMode(LEDPIN3, OUTPUT);
  pinMode(LEDPIN4, OUTPUT);
  pinMode(LEDPIN5, OUTPUT);
  pinMode(LEDPIN6, OUTPUT);
  pinMode(LEDPIN7, OUTPUT);
  pinMode(LEDPIN8, OUTPUT);

  //enter sleep mode
  sleep_flag = true;
}

void loop() {

  
  if (trigger_flag) {
    trigger_time = millis();

    if (trigger_pin < 8) {
      Serial.print('B');
      Serial.print(trigger_pin);
      Serial.println('1');

      Serial1.print('B');
      Serial1.print(trigger_pin);
      Serial1.println('1');
    } else {
      //trigger pin 8 is for all windchimes
      Serial.print('A');
      Serial.print('0');
      Serial.println('1');

      Serial1.print('A');
      Serial1.print('0');
      Serial1.println('1');
    }

    trigger_flag = false;
    sleep_flag = false;
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

void button1_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN1, HIGH);
    trigger_flag = true;
    trigger_pin = 1;
  }
}

void button2_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN2, HIGH);
    trigger_flag = true;
    trigger_pin = 2;
  }
}

void button3_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN3, HIGH);
    trigger_flag = true;
    trigger_pin = 3;
  }
}

void button4_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN4, HIGH);
    trigger_flag = true;
    trigger_pin = 4;
  }
}

void button5_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN5, HIGH);
    trigger_flag = true;
    trigger_pin = 5;
  }
}

void button6_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN6, HIGH);
    trigger_flag = true;
    trigger_pin = 6;
  }
}

void button7_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN7, HIGH);
    trigger_flag = true;
    trigger_pin = 7;
  }
}

void button8_interrupt() {
  if (sleep_flag) {
    digitalWrite(LEDPIN8, HIGH);
    trigger_flag = true;
    trigger_pin = 8;
  }
}
