#include <SoftwareSerial.h>
#include <Servo.h>
Servo EduServo;


SoftwareSerial BTSerial (3, 4); // 2번, 3번 핀에 TX, RX 연결

int trigPin = 13;                           // 디지털 13번 핀에 연결
int echoPin = 12;                           // 디지털 12번 핀에 연결
int Ultra_d = 0;

int val = 0;

int RightMotor_E_pin = 5;                   // 오른쪽 모터의 Enable & PWM
int LeftMotor_E_pin = 6;                    // 왼쪽 모터의 Enable & PWM
int RightMotor_1_pin = 8;                   // 오른쪽 모터 제어선 IN1
int RightMotor_2_pin = 9;                   // 오른쪽 모터 제어선 IN2
int LeftMotor_3_pin = 10;                   // 왼쪽 모터 제어선 IN3
int LeftMotor_4_pin = 11;                   // 왼쪽 모터 제어선 IN4

int L_MotorSpeed = 153;                     // 왼쪽 모터 속도
int R_MotorSpeed = 153;                     // 오른쪽 모터 속도

int L_Line = A5;                                                 // 왼쪽 라인트레이서 센서는 A5 핀에 연결
int C_Line = A4;                                                 // 가운데 라인트레이서 센서는 A4 핀에 연결
int R_Line = A3;

int SL = 1;
int SC = 1;
int SR = 1;

void setup()
{
  EduServo.attach(2);

  Serial.begin (9600); // 시리얼 통신 속도 설정
  Serial.println ("LED control test!"); // 시리얼 모니터 창에 문자열 출력
  Serial.println ("no command!");
  BTSerial.begin (9600); // 블루투스데이터전송속도초기화

  pinMode(RightMotor_E_pin, OUTPUT);        // 출력모드로 설정
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Welcome Eduino!");
}
void loop()
{
  if (BTSerial.available()) // 데이터가 수신되면
  {
    char toSend = (char)BTSerial.read(); // 문자 형 데이터로 저장
    Serial.print ("Command = "); // 수신 된 데이터를
    Serial.println (toSend); // 시리얼 모니터 창에 출력
    switch (toSend) // 수신된 데이터가
    {
      case '1': // 직진 이면
        {
          motor_role(HIGH, HIGH);
          break;
        }
      case '2': // 후진 이면
        {
          motor_role(LOW, LOW);
          break;
        }
      case '3': // 좌회전 이면
        {
          motor_role(HIGH, LOW);
          break;
        }
      case '4': // 우회전 이면
        {
          motor_role(LOW, HIGH);
          break;
        }
      case '0': // 정지 이면
        {
          analogWrite(RightMotor_E_pin, 0);
          analogWrite(LeftMotor_E_pin, 0);
          break;
        }

      case '5': // 정지 이면
        {
          int L = digitalRead(L_Line);
          int C = digitalRead(C_Line);
          int R = digitalRead(R_Line);

          Serial.print("digital : ");
          Serial.print(L);
          Serial.print(", ");
          Serial.print(C);
          Serial.print(", ");
          Serial.print(R);
          Serial.print("   ");

          if ( L == LOW && C == LOW && R == LOW )
          { // 0 0 0
            L = SL; C = SC; R = SR;
          }

          if ( L == LOW && C == HIGH && R == LOW )
          { // 0 1 0
            motor_role(HIGH, HIGH);
            Serial.println("직진");
          }

          else if (L == LOW && R == HIGH )
          { // 0 0 1, 0 1 1
            motor_role(LOW, HIGH);
            Serial.println("우회전");
          }

          else if (L == HIGH && R == LOW )
          { // 1 0 0, 1 1 0
            motor_role(HIGH, LOW);
            Serial.println("좌회전");

          }
          else if ( L == HIGH && R == HIGH )
          { // 1 1 1, 1 0 1
            analogWrite(RightMotor_E_pin, 0);
            analogWrite(LeftMotor_E_pin, 0);
            Serial.println("정지");
          }
          SL = L; SC = C; SR = R;
        }
        break;
      case '6': // 정지 이면
        {
          Ultra_d = Ultrasonic();
          Serial.println(Ultra_d);
          motor_role(HIGH, HIGH);         // 직진

          if (Ultra_d < 250) {
            if (Ultra_d < 150) {
              Serial.println("150 이하.");
              motor_role(LOW, LOW);      // 후진
              delay(1000);
              analogWrite(RightMotor_E_pin, 0);
              analogWrite(LeftMotor_E_pin, 0);
              delay(200);
            }
            else {
              analogWrite(RightMotor_E_pin, 0);
              analogWrite(LeftMotor_E_pin, 0);
              delay(200);
              Serial.println("150 이상.");
              val =  Servo_con();
              if (val == 0) {
                Serial.println("우회전.");
                analogWrite(RightMotor_E_pin, 0);
                analogWrite(LeftMotor_E_pin, 0);
                delay(500);
                motor_role(LOW, LOW);    // 후진
                delay(500);
                motor_role(LOW, HIGH);   // 우회전
                delay(800);
              }
              else if (val == 1) {
                Serial.println("좌회전.");
                analogWrite(RightMotor_E_pin, 0);
                analogWrite(LeftMotor_E_pin, 0);
                delay(500);
                motor_role(LOW, LOW);   // 후진
                delay(500);
                motor_role(HIGH, LOW);  // 좌회전
                delay(800);
              }
            }
          }
          break;
        }
    }
  }
}


void motor_role(int R_motor, int L_motor)
{
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, R_MotorSpeed);                          // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);                           // 좌측 모터 속도값
}
int Ultrasonic()
{
  long duration, distance;
  digitalWrite(trigPin, HIGH);                                            // trigPin에서 초음파 발생(echoPin도 HIGH)        
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);                                      // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 1000) / 2; 

  Serial.print("DIstance:");                                              // 물체와 초음파 센서간 거리를 표시.        
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

  if(Ult_30 > Ult_150){
     val = 1;
  }
  else{
     val = 0;
  }
  EduServo.write(90);
  return val;
}
