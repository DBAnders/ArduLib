//------------------------------------------------------------------------------
// File...: objKey.cpp                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
// OBJECT CLASS: objKey - Multi Key Manager (Taster- Entprellung)
//------------------------------------------------------------------------------
#include "classEnable.h"
#ifdef CE_OBJ_KEY
#include <Arduino.h>
#include "objKey.h"

//------------------------------------------------------------------------------
// Class constructor 
//------------------------------------------------------------------------------
objKey::objKey(void)
{
    keyCnt = 0;
    for (int i=0; i<KEY_CNT_MAX; i++)
    {        
        keyPin[i] = 0;
    }
}

//------------------------------------------------------------------------------
// Insert new KEY and connect with GPIO "pinNumber"
//------------------------------------------------------------------------------
bool objKey::Insert(U8 pinNumber)
{
    if (keyCnt < KEY_CNT_MAX)
    {         
        keyPin[keyCnt] = pinNumber;        
        pinMode(pinNumber, INPUT_PULLUP);        
        keyCnt++;
        return true; // OK
    }
    return false; // ERROR
}

//------------------------------------------------------------------------------
// return TRUE if KEY "keyIndex" pressed and wait until release
//------------------------------------------------------------------------------
bool objKey::KeyPressed(U8 keyIndex)
{
     if (isKeyRange(keyIndex))
     {
        keyIndex--;
        if (digitalRead(keyPin[keyIndex]) == LOW)
        {
            while (digitalRead(keyPin[keyIndex]) == LOW)
            {
                delay(100);
            }          
            return true; // KEY PRESSED
        }       
    }
    return false; // NO KEY PRESSED OR ERR   
}

//------------------------------------------------------------------------------
// return TRUE if KEY "keyIndex" pressed
//------------------------------------------------------------------------------
bool objKey::KeyDown(U8 keyIndex)
{
     if (isKeyRange(keyIndex))
     {
        keyIndex--;
        if (digitalRead(keyPin[keyIndex]) == LOW)
        {
            return true; // KEY PRESSED
        }       
    }
    return false; // NO KEY PRESSED OR ERR   
}

//------------------------------------------------------------------------------
// Check if KEY "keyIndex" pressed and wait until release
//------------------------------------------------------------------------------
void objKey::KeyWait(U8 keyIndex)
{
     if (isKeyRange(keyIndex))
     {
        keyIndex--;                    
        while (digitalRead(keyPin[keyIndex]) == LOW)
        {
            delay(100);
        }          
        return ;
    }    
}


//------------------------------------------------------------------------------
bool objKey::isKeyRange(U8 keyIndex)
{
    if ((keyIndex >= 1) && (keyIndex <= keyCnt))
    {
        return true;
    }
    return false;
}
#endif // CE_OBJ_KEY
// END OF objKeycpp                 