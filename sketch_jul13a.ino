#include <SoftwareSerial.h>

SoftwareSerial btSerial(3,4);
int BTstates=0;

const int sw = 2;
const int joystickXPin = A0;
const int joystickYPin = A1;
const int bluebuttonPin = 5;
const int redbuttonPin = 6;

void setup()
{
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(joystickXPin,INPUT);
  pinMode(joystickYPin,INPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(bluebuttonPin, INPUT_PULLUP);
  pinMode(redbuttonPin, INPUT_PULLUP);
}

void loop()
{
  int bluebuttonPinValue = digitalRead(bluebuttonPin);
  int redbuttonPinValue = digitalRead(redbuttonPin);
  int swValue = digitalRead(sw);
  int joyXVal = analogRead(joystickXPin);
  int joyYVal = analogRead(joystickYPin);

  if(bluebuttonPinValue == LOW)
  {
    Serial.print("5");
    btSerial.print(String(5));
    delay(50);
  }
  
  if(bluebuttonPinValue == LOW)
  {
    Serial.print("6");
    btSerial.print(String(6));
    delay(50);
  }
  
  
  if(joyXVal>850)
  {
    Serial.print("1");
    btSerial.print(String(1));
  }
  else if(joyXVal<250)
  {
    Serial.print("2");
    btSerial.print(String(2));
  }
  else if(joyYVal<250)
  {
    Serial.print("3");
    btSerial.print(String(3));
  }
  else if(joyYVal>850)
  {
    Serial.print("4");
    btSerial.print(String(4));
  }
  else
  {
    Serial.print("0");
    btSerial.print(String(0));
  }
  
}
