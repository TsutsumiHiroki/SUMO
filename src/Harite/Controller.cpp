#include "Arduino.h"
#include "Controller.h"

Controller::Controller(uint8_t leftPinNumber, uint8_t lightPinNumber)
{
    leftPin = leftPinNumber;
    lightPin = lightPinNumber;
}

void Controller::initialize()
{
    pulseIn(leftPin, HIGH);
    pulseIn(lightPin, HIGH);
}