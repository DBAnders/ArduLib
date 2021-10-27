//------------------------------------------------------------------------------
// File...: objLed.h                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
#ifndef _CPP_OBJLED
#define _CPP_OBJLED

/* Define number of LED elements */
#define LED_CNT_MAX 8

//==============================================================================
// OBJECT CLASS: objLed - Multi Control LED
//==============================================================================
class objLed
{
    public:
        /* Class constructor */ 
        objLed(void);
        
        /* Insert new LED and connect with GPIO "pinNumber" */
        bool Insert(U8 PinNumber);
        
        /* Switch LED "ledIndex" ON=1 or OFF=0 */
        bool SwitchPower(U8 ledIndex, U8 ledPower);
        
        /* Switch LED "ledIndex" OFF */
        bool SwitchOff(U8 ledIndex) { SwitchPower(ledIndex, 0); }
        
        /* Switch LED "ledIndex" ON */
        bool SwitchOn(U8 ledIndex) { SwitchPower(ledIndex, 1); }
        
        /* Toggle LED "ledIndex" ON or OFF */
        bool SwitchToggle(U8 ledIndex);        

    private:
        U8 ledOn[LED_CNT_MAX];
        U8 ledPin[LED_CNT_MAX];
        U8 ledCnt=0;
        bool isLedRange(U8 ledIndex);
};            

#endif // _CPP_OBJLED

