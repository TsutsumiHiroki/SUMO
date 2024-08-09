#include <Arduino.h>
#include "ODriveArduino.h"
#include "ODriveEnums.h"
#include <TaskManager.h>

ODriveArduino odrive(Serial1);

const int mr8pin_A = 2;
const int mr8pin_C = 3;

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
  Serial.println(getPulse[1]);
}

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

  attachInterrupt(mr8pin_A, chAInterupt, CHANGE);
  attachInterrupt(mr8pin_C, chCInterupt, CHANGE);
  odriveInit();

  // Serial1 << "Axis" << '0' << ": Requesting state" << requested_state << '\n';
  // Serial1 << "Axis" << '0' << ": Requesting state" << requested_state << '\n';
  // if (!odrive.run_state(0, requested_state, false))
  //   return;
  // if (!odrive.run_state(0, requested_state, false))
  // return;
}

void loop()
{
  // char input = Serial.read();

  // if (input == 'v')
  // {
  //   Serial1 << "r vbus_voltage\n";
  //   Serial << "Vbus voltage: " << odrive.readFloat() << "\n";
  // }

  //   if (input == 'r')
  // {
  //   Serial1.print("sr\n");
  //   Serial1.println("Reboot");
  // }

  if (getPulse[0] <= 2100 && getPulse[0] > (1497 + 200)) //&& getPulse[0] >= 890
  {
    digitalWrite(25, HIGH);
    odrive.SetVelocity(0, -2000);
    digitalWrite(LED_BUILTIN, HIGH);

    // Serial.print(odrive.GetCountVelocity(0));
    // Serial.println("count/s\n");
  }
  else if (getPulse[0] < (1495 - 200) && getPulse[0] >= 890) // getPulse[0] <= 2100 &&
  {
    odrive.SetVelocity(0, 2000);
    digitalWrite(LED_BUILTIN, HIGH);

    // Serial.print(odrive.GetCountVelocity(0));
    // Serial.println("count/s\n");
  }
  else
  {
    digitalWrite(25, LOW);
    odrive.SetVelocity(0, 0);
    digitalWrite(LED_BUILTIN, LOW);
    // Serial.println("STOP"); // Motor Stop
  }

  if (getPulse[1] <= 2100 && getPulse[1] > (1497 + 200)) //&& getPulse[0] >= 890
  {
    digitalWrite(25, HIGH);
    odrive.SetVelocity(1, 2000);
    digitalWrite(LED_BUILTIN, HIGH);

    // Serial.print(odrive.GetCountVelocity(1));
    // Serial.println("count/s\n");
  }
  else if (getPulse[1] < (1495 - 200) && getPulse[1] >= 890) // getPulse[0] <= 2100 &&
  {
    odrive.SetVelocity(1, -2000);
    digitalWrite(LED_BUILTIN, HIGH);

    // Serial.print(odrive.GetCountVelocity(1));
    // Serial.println("count/s\n");
  }
  else
  {
    digitalWrite(25, LOW);
    odrive.SetVelocity(1, 0);
    digitalWrite(LED_BUILTIN, LOW);
    // Serial.println("STOP"); // Motor Stop
  }

  // Serial.print("\033[2J"); // エスケープシーケンス
  // Serial.print("\033[H");
  // Serial.print("\033[2B");
}