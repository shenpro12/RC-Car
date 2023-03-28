#define BLYNK_TEMPLATE_ID "TMPLUcPZvcKy"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "-lBUCHtfr_G4Z96C57eAAsoSKBPYMhFC"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char ssid[] = "Vo thuong";
char pass[] = "wmac3979xs";

BlynkTimer timer;
Servo servo;

const int ENA = 12;
const int IN1 = 5;
const int IN2 = 4;
const int ENB = 14;
const int IN3 = 0;
const int IN4 = 2;

int speed = 100;
bool autoDrive = false;

void goAhead() {
  analogWrite(ENA, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(ENB, speed);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void goBack() {
  analogWrite(ENA, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(ENB, speed);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

BLYNK_WRITE(V0) {
  if (autoDrive) {
    return;
  }
  servo.write(param.asInt() * -1);
}

BLYNK_WRITE(V1) {
  if (autoDrive) {
    return;
  }
  int value = param.asInt();
  if (value > 2) {
    goAhead();
  } else if (value < 2) {
    goBack();
  } else {
    stop();
  }
}

BLYNK_WRITE(V2) {
  if (autoDrive) {
    return;
  }
  speed = param.asInt();
}

BLYNK_WRITE(V3) {
  if (param.asInt() == 0) {
    autoDrive = false;
  } else {
    autoDrive = true;
    speed = 150;
  }
}

void myTimerEvent() {
  Blynk.virtualWrite(V2, speed);

  if (autoDrive) {
    Blynk.virtualWrite(V3, 1);
  } else {
    Blynk.virtualWrite(V3, 0);
  }
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  servo.attach(15, 544, 2400);  //D0(pin16)
  servo.write(90);

  timer.setInterval(100L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
}
