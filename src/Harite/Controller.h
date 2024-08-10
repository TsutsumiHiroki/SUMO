#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>

class Controller{
    public:
    Controller(uint8_t leftPinNumber, uint8_t lightPinNumber);
    
    void initialize();


    private:
    uint8_t leftPin;
    uint8_t lightPin;


};

#endif