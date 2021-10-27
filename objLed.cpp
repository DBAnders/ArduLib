//------------------------------------------------------------------------------
// File...: objLed.cpp                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
// objLed - Multi Control LED
//------------------------------------------------------------------------------
#include "classEnable.h"
#ifdef CE_OBJ_LED
#include <Arduino.h>
#include "objLed.h"

//------------------------------------------------------------------------------
// Class constructor
//------------------------------------------------------------------------------
objLed::objLed(void)
{
    ledCnt = 0;
    for (int i=0; i<LED_CNT_MAX; i++)
    {
        ledOn[i] = 0;
        ledPin[i] = 0;
    }
}

//------------------------------------------------------------------------------
// Insert new LED and connect with GPIO "pinNumber" 
//------------------------------------------------------------------------------
bool objLed::Insert(U8 pinNumber)
{
    if (ledCnt < LED_CNT_MAX)
    {
         ledOn[ledCnt] = 0;
         ledPin[ledCnt] = pinNumber;
         pinMode(pinNumber, OUTPUT);
         digitalWrite(pinNumber, 0);
         ledCnt++;
         return true; // OK
    }
    return false; // ERROR
}

//------------------------------------------------------------------------------
// Switch LED "ledIndex" ON=1 or OFF=0
//------------------------------------------------------------------------------
bool objLed::SwitchPower(U8 ledIndex, U8 ledPower)
{
    if (isLedRange(ledIndex))
    {
        ledIndex--;
        ledOn[ledIndex] = (ledPower == 1) ? 1 : 0;
        digitalWrite(ledPin[ledIndex], ledOn[ledIndex]);
        return true; // OK
    }
    return false; // ERROR
}

//------------------------------------------------------------------------------
// Toggle LED "ledIndex" ON or OFF
//------------------------------------------------------------------------------
bool objLed::SwitchToggle(U8 ledIndex)
{
    if (isLedRange(ledIndex))
    {
        ledIndex--;
        ledOn[ledIndex] = (ledOn[ledIndex] == 1) ? 0 : 1;
        digitalWrite(ledPin[ledIndex], ledOn[ledIndex]);
        return true; // OK
    }
    return false; // ERROR
}

//------------------------------------------------------------------------------
bool objLed::isLedRange(U8 ledIndex)
{
    if ((ledIndex >= 1) && (ledIndex <= ledCnt))
    {
        return true; // OK
    }
    return false; // ERROR
}
#endif // CE_OBJ_LED
// END OF objLed.cpp                 