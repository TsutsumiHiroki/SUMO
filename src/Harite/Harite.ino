#include <Arduino.h>
#include "ODriveArduino.h"
#include "ODriveEnums.h"
// #include <TaskManager.h>

ODriveArduino odrive(Serial1);

const int mr8pin_A = 2;
const int mr8pin_C = 3;

// 基板届いたら
// const int mr8pin_B = 4;
// const int mr8pin_D = 5;


volatile unsigned long startPulse[8];
volatile int getPulse[8];

template <class T>
inline Print &operator<<(Print &obj, T arg)
{
  obj.print(arg);
  return obj;
}
template <>
inline Print &operator<<(Print &obj, float arg)
{
  obj.print(arg, 4);
  return obj;
}

void chAInterupt()
{
  unsigned long nowPulse = micros(); // 現在のマイコン内の時間をnowPulseに代入
  if (digitalRead(mr8pin_A) == HIGH) // 端子がHIGHの場合
  {
    startPulse[0] = nowPulse; // 現在の時間をstartPulseに代入
  }
  else // 端子がLOWの場合
  {
    getPulse[0] = nowPulse - startPulse[0]; // 現在の時間(NowPulse)とstartPulseの差を求める=>パルス幅
  }
}


void chCInterupt()
{
  unsigned long nowPulse = micros();
  if (digitalRead(mr8pin_C) == HIGH)
  {
    startPulse[1] = nowPulse;
  }
  else
  {
    getPulse[1] = nowPulse - startPulse[1];
  }
  Serial.print("chC ");
  Serial.println(getPulse[1]);
}


// 基板届いたら
// void chBInterupt()
// {
//   unsigned long nowPulse = micros();
//   if (digitalRead(mr8pin_B) == HIGH)
//   {
//     startPulse[2] = nowPulse;
//   }
//   else
//   {
//     getPulse[2] = nowPulse - startPulse[2];
//   }
//   Serial.print("chB ");
//   Serial.println(getPulse[2]);
// }


// void chDInterupt()
// {
//   unsigned long nowPulse = micros();
//   if (digitalRead(mr8pin_D) == HIGH)
//   {
//     startPulse[3] = nowPulse;
//   }
//   else
//   {
//     getPulse[3] = nowPulse - startPulse[3];
//   }
//   Serial.print("chD ");
//   Serial.println(getPulse[3]);
// }


void odriveInit()
{
  int requested_state0;
  int requested_state1;

  int motorNumber0 = 0;
  int motorNumber1 = 1;

  requested_state0 = 8;
  if (!odrive.run_state(motorNumber0, requested_state0, false))
    return;

  delay(10);
  requested_state1 = 8;
  if (!odrive.run_state(motorNumber1, requested_state1, false))
    return;
  delay(10);
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(mr8pin_A, INPUT);
  pinMode(mr8pin_C, INPUT);

  attachInterrupt(digitalPinToInterrupt(mr8pin_A), chAInterupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mr8pin_C), chCInterupt, CHANGE);

  // attachInterrupt(mr8pin_A, chAInterupt, CHANGE);
  // attachInterrupt(mr8pin_C, chCInterupt, CHANGE);
  odriveInit();
}


void loop()
{
  if (getPulse[0] <= 2100 && getPulse[0] > (1497 + 100))
  {
    digitalWrite(25, HIGH);
    odrive.SetVelocity(0, -2000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (getPulse[0] < (1495 - 100) && getPulse[0] >= 890)
  {
    odrive.SetVelocity(0, 2000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(25, LOW);
    odrive.SetVelocity(0, 0);
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (getPulse[1] <= 2100 && getPulse[1] > (1497 + 100))
  {
    digitalWrite(25, HIGH);
    odrive.SetVelocity(1, 2000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (getPulse[1] < (1495 - 100) && getPulse[1] >= 890)
  {
    odrive.SetVelocity(1, -2000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(25, LOW);
    odrive.SetVelocity(1, 0);
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.print("\033[2J"); // エスケープシーケンス
  Serial.print("\033[H");
  Serial.print("\033[2B");
}