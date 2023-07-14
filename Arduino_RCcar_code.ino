#include <SoftwareSerial.h>
#include <Servo.h>

Servo EduServo;

SoftwareSerial BTSerial(8, 4);  // HC-06 모듈을 연결한 소프트웨어 시리얼 포트
int trigPin = 13;
int echoPin = 12;
int Ultra_d = 0;

int val = 0;

int RightMotor_E_pin = 5;
int LeftMotor_E_pin = 6;
int RightMotor_1_pin = 3;
int RightMotor_2_pin = 9;
int LeftMotor_3_pin = 10;
int LeftMotor_4_pin = 11;

int L_MotorSpeed = 150;
int R_MotorSpeed = 130;
int L_MotorSpeed_1 = 200;
int R_MotorSpeed_1 = 200;
int L_MotorSpeed_2 = 180;
int R_MotorSpeed_2 = 160;
int L_MotorSpeed1 = 200;
int R_MotorSpeed1 = 150;
int L_MotorSpeed2 = 150;
int R_MotorSpeed2 = 200;

int L_Line = A5;
int C_Line = A4;
int R_Line = A3;

int SL = 1;
int SC = 1;
int SR = 1;

bool isStopped = true;  // 정지 상태인지 여부를 나타내는 변수
bool isAttached = false;

void setup()
{
  //EduServo.attach(2);

  Serial.begin(9600);
  Serial.println("LED control test!");
  Serial.println("no command!");

  BTSerial.begin(9600);

  pinMode(RightMotor_E_pin, OUTPUT);
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);

  Serial.println("Welcome Eduino!");
}

void loop()
{
  if (!isAttached) 
  {
    EduServo.attach(2); // 서브 모터 초기화
    isAttached = true;
  }
  if (BTSerial.available())
  {
    char toSend = (char)BTSerial.read();
    Serial.print("Command = ");
    Serial.println(toSend);

    if (toSend == '1')
    {
      isStopped = false;  // 정지 상태 해제
      motor_role(L_MotorSpeed_2 , R_MotorSpeed_2);
    }
    else if (toSend == '2')
    {
      isStopped = false;  // 정지 상태 해제
      motor_role(-L_MotorSpeed_2, -R_MotorSpeed_2);  // 후진 동작
    }
    else if (toSend == '3')
    {
      isStopped = false;  // 정지 상태 해제
      motor_role(-L_MotorSpeed_2, R_MotorSpeed_2);
    }
    else if (toSend == '4')
    {
      isStopped = false;  // 정지 상태 해제
      motor_role(L_MotorSpeed_2, -R_MotorSpeed_2);
    }
    else if (toSend == '0')
    {
      isStopped = true;  // 정지 상태로 설정
      stop_motors();     // 모터 정지
    }
    else if (toSend == '5')
    {
      isStopped = false;  // 정지 상태 해제
      while (true)
      {
        int L = digitalRead(L_Line);
        int C = digitalRead(C_Line);
        int R = digitalRead(R_Line);

        Serial.print("digital: ");
        Serial.print(L);
        Serial.print(", ");
        Serial.print(C);
        Serial.print(", ");
        Serial.print(R);
        Serial.print("   ");

        if (L == LOW && C == HIGH && R == LOW)
        {
            // 모터 정지
          motor_role(L_MotorSpeed, R_MotorSpeed);
          Serial.println("직진");
        }
        else if (L == LOW && R == HIGH)
        {
          stop_motors();  // 모터 정지
          motor_role(-L_MotorSpeed2, -R_MotorSpeed2);
          delay(100);
          motor_role(L_MotorSpeed_2, -R_MotorSpeed_2);
          Serial.println("우회전");
        }
        else if (L == HIGH && R == LOW)
        {
          stop_motors();  // 모터 정지
          motor_role(-L_MotorSpeed1, -R_MotorSpeed1);
          delay(100);
          motor_role(-L_MotorSpeed_2, R_MotorSpeed_2);
          Serial.println("좌회전");
        }
        else if (L == HIGH && R == HIGH)
        {
          stop_motors();  // 모터 정지
          Serial.println("정지");
        }

        if (BTSerial.available())
        {
          toSend = (char)BTSerial.read();
          if (toSend != '5')
            break;
        }
      }
      isStopped = true;  // 정지 상태로 설정
      stop_motors();     // 모터 정지
    }


    else if (toSend == '6')
    {
      while (toSend == '6')  // 6을 입력할 때까지 반복 실행
      {
        Ultra_d = Ultrasonic();
        Serial.println(Ultra_d);
        motor_role(L_MotorSpeed, R_MotorSpeed);

        if (Ultra_d < 250)
        {
          if (Ultra_d < 150)
          {
            Serial.println("150 이하.");
            motor_role(-L_MotorSpeed_1, -R_MotorSpeed_1);
            delay(1000);
            stop_motors();  // 모터 정지
            delay(200);
          }
          else
          {
            stop_motors();  // 모터 정지
            delay(200);
            Serial.println("150 이상.");
            val = Servo_con();
            if (val == 0)
            {
              Serial.println("우회전.");
              stop_motors();  // 모터 정지
              delay(500);
              motor_role(-L_MotorSpeed_1, -R_MotorSpeed_1);
              delay(500);
              motor_role(L_MotorSpeed_1, -R_MotorSpeed_1);
              delay(800);
            }
            else if (val == 1)
            {
              Serial.println("좌회전.");
              stop_motors();  // 모터 정지
              delay(500);
              motor_role(-L_MotorSpeed_1, -R_MotorSpeed_1);
              delay(500);
              motor_role(-L_MotorSpeed_1, R_MotorSpeed_1);
              delay(800);
            }
          }
        }

        if (BTSerial.available())
        {
          toSend = (char)BTSerial.read();
          if (toSend != '6')
            break;
        }
      }
    }
  }

  if (isStopped) {
    stop_motors();  // 정지 상태일 때 모터 정지
  }
}

void stop_motors()
{
  analogWrite(RightMotor_E_pin, 0);
  analogWrite(LeftMotor_E_pin, 0);
  delay(50);
}

void motor_role(int L_motor, int R_motor)
{
  digitalWrite(LeftMotor_3_pin, L_motor >= 0 ? HIGH : LOW);
  digitalWrite(LeftMotor_4_pin, L_motor < 0 ? HIGH : LOW);
  digitalWrite(RightMotor_1_pin, R_motor >= 0 ? HIGH : LOW);
  digitalWrite(RightMotor_2_pin, R_motor < 0 ? HIGH : LOW);

  analogWrite(LeftMotor_E_pin, abs(L_motor));
  analogWrite(RightMotor_E_pin, abs(R_motor));
}

int Ultrasonic()
{
  long duration, distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = ((float)(340 * duration) / 1000) / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
}

int Servo_con()
{
  EduServo.write(30);
  delay(300);
  int Ult_30 = Ultrasonic();
  delay(700);
  EduServo.write(150);
  delay(300);
  int Ult_150 = Ultrasonic();
  delay(700);

  if (Ult_30 > Ult_150)
  {
    val = 1;
  }
  else
  {
    val = 0;
  }
  EduServo.write(90);
  return val;
}
