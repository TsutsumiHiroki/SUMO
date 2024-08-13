#include <Arduino.h>
#include "ODriveArduino.h"
#include "ODriveEnums.h"
// #include <TaskManager.h>

ODriveArduino odrive(Serial1);

const int mr8pin_A = 2;
const int mr8pin_D = 3;

uint32_t offsetMr8 = 10;

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
  Serial.print("chA ");
  Serial.println(getPulse[0]);
}

void chDInterupt()
{
  unsigned long nowPulse = micros();
  if (digitalRead(mr8pin_D) == HIGH)
  {
    startPulse[1] = nowPulse;
  }
  else
  {
    getPulse[1] = nowPulse - startPulse[1];
  }
  Serial.print("chD ");
  Serial.println(getPulse[1]);
}

// 基板届いたら

// void chGInterupt()
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

// void chHInterupt()
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

  requested_state1 = 8;
  if (!odrive.run_state(motorNumber1, requested_state1, false))
    return;
}

// chA
void fowardAndBackfoward()
{
  if ((1496 + offsetMr8) < getPulse[0] && getPulse[0] <= 2100)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    odrive.SetVelocity(0, -2000);
    odrive.SetVelocity(1, 2000);
  }
  else if (890 >= getPulse[0] && getPulse[0] < (1496 - offsetMr8))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    odrive.SetVelocity(0, 2000);
    odrive.SetVelocity(1, -2000);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    odrive.SetVelocity(0, 0);
    odrive.SetVelocity(1, 0);
  }
}

// chD
void rotate()
{
  if ((1496 + offsetMr8) < getPulse[1] && getPulse[1] <= 2100)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    odrive.SetVelocity(0, -2000);
    odrive.SetVelocity(1, 2000);
  }
  else if (890 >= getPulse[1] && getPulse[1] < (1496 - offsetMr8))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    odrive.SetVelocity(0, 2000);
    odrive.SetVelocity(1, -2000);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    odrive.SetVelocity(0, 0);
    odrive.SetVelocity(1, 0);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(mr8pin_A, INPUT);
  pinMode(mr8pin_D, INPUT);

  attachInterrupt(digitalPinToInterrupt(mr8pin_A), chAInterupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(mr8pin_D), chDInterupt, CHANGE);

  // attachInterrupt(mr8pin_A, chAInterupt, CHANGE);
  // attachInterrupt(mr8pin_C, chCInterupt, CHANGE);
  odriveInit();
}

void loop()
{
  fowardAndBackfoward();
  rotate();
}