//------------------------------------------------------------------------------
// File...: objFs20.cpp
// Author.: M. Anders
// Date...: 24.06.2020
//------------------------------------------------------------------------------
// objFs20 - FS20 ELV Tx868 Modul
//------------------------------------------------------------------------------
#include "classEnable.h"
#ifdef CE_OBJ_FS20
#include <Arduino.h>
#include "objFs20.h"

//------------------------------------------------------------------------------
// FS20 timing bit (BIT_0: 1=400us + 0=400us - BIT_1: 1=600us + 0=600us
//------------------------------------------------------------------------------
#define FS20_ZERO         400     //   400uS
#define FS20_ONE          600     //   600uS
#define FS20_PAUSE         10     // 10000mS

//------------------------------------------------------------------------------
#define FS20_MAX_SYNC      12
#define FS20_REP_CNT        3

//------------------------------------------------------------------------------
#define FS20_SWITCH_OFF    0x00
#define FS20_SWITCH_ON     0x10
#define FS20_SWITCH_TOGGLE 0x12

//------------------------------------------------------------------------------
// FS20 Bit Sample Telegram (Reference):
//------------------------------------------------------------------------------
#if 0 
#define BIT_BUFFER_SIZE 92
U8 bit_buffer[BIT_BUFFER_SIZE] =
{
  0,0,0,0, 0,0,0,0, 0,0,0,0 // 11 or 12 Sync
  1,                // Start FS20 telegram
  0,1,1,0, 0,0,1,1, // homeCode_H  = 0x63
  0,                // homeCode_H_Parity
  0,1,0,0, 0,0,1,0, // homeCode_L  = 0x42
  0,                // homeCode_L_Parity
  0,0,0,0, 0,0,0,1, // addrByte    = 0x01
  1,                // addrByte_Parity
  0,0,0,1, 0,0,0,1, // cmdByte     = 0x11
  0,                // cmdByte_Parity
  1,0,1,1, 1,1,0,1, // checkSum    = 0xBD
  0,                // checkSum_Parity
  0,                // End of FS20 telegram 
  0xF               // Exit telegram buffer
};               
#endif

//------------------------------------------------------------------------------
// Class Constructor
//------------------------------------------------------------------------------
objFs20::objFs20(void)
{    
    fs20Index = 0;
    fs20DataPin = 0;    
}

//------------------------------------------------------------------------------
// Initialize FS20 ELV Tx868 Modul
//------------------------------------------------------------------------------
bool objFs20::Init(U8 dataPin)
{
    fs20DataPin = dataPin;
    pinMode(fs20DataPin, OUTPUT);
    digitalWrite(fs20DataPin, 0);
    return false;
}

//------------------------------------------------------------------------------
// Send FS20 Actor Data (with other homeCode, 3x telegram)
//------------------------------------------------------------------------------
void objFs20::Send(U16 homeCode, U8 addrByte, U8 cmdByte)
{
    for (U8 iLoop=0; iLoop<FS20_REP_CNT; iLoop++)
    {        
        SendEx(homeCode, addrByte, cmdByte);
        delayMicroseconds(8000);
    }       
}

//------------------------------------------------------------------------------
// Switch FS20 Actor ON or OFF
//------------------------------------------------------------------------------
void objFs20::Switch(U16 homeCode, U8 addrByte, bool swOn)
{
    U8 cmdByte = (swOn) ? FS20_SWITCH_ON : FS20_SWITCH_OFF;
    Send(homeCode, addrByte, cmdByte);
}

//------------------------------------------------------------------------------
// Toggle FS20 Actor ON or OFF
//------------------------------------------------------------------------------
void objFs20::Switch(U16 homeCode, U8 addrByte)
{    
    Send(homeCode, addrByte, FS20_SWITCH_TOGGLE);
}

//------------------------------------------------------------------------------
// Dimm FS20 Actor (value between 0..16)
//------------------------------------------------------------------------------
void objFs20::Dimm(U16 homeCode, U8 addrByte, U8 dimmValue)
{    
    U8 cmdByte = dimmValue;    
    if (cmdByte <= FS20_SWITCH_ON)
    {
        Send(homeCode, addrByte, FS20_SWITCH_TOGGLE);
    }
}

//------------------------------------------------------------------------------
// Internal - Send complete FS20 telegram (single telegram)
//------------------------------------------------------------------------------
void objFs20::SendEx(U16 homeCode, U8 addrByte, U8 cmdByte)
{    
    U8 hCodeH = homeCode / 0x100;
    U8 hCodeL = homeCode % 0x100;    
    U8 checkSum = 0x06;
    
    /* Calculate checksum and add 0x06 for FS20 (0x0C for FHT) */
    checkSum += hCodeH;
    checkSum += hCodeL;
    checkSum += addrByte;
    checkSum += cmdByte;
     
    /* Clear bit-buffer  */
    tx868Clear();
    
    /* Synchronisation */
    for (U8 i=0; i<FS20_MAX_SYNC; i++)
    {
        tx868AddBit(0);
    }
    /* Start Bit */
    tx868AddBit(1);
    
    /* Home code */
    tx868AddByte(hCodeH);
    tx868AddByte(hCodeL);
    
    /* address, command and checksum byte */
    tx868AddByte(addrByte);
    tx868AddByte(cmdByte);
    tx868AddByte(checkSum);
    
    /* End Bit */
    tx868AddBit(0);
    
    /* End of list */  
    tx868AddBit(0xF);

    /* Send bit telegram */
    tx868Send(); 
}

//------------------------------------------------------------------------------
// TX868 - Clear bit-buffer
//------------------------------------------------------------------------------
void objFs20::tx868Clear()
{
    for (U16 i=0; i<FS20_BUFFER_LEN; i++)
    {
        fs20Buffer[i] = 0;
    }
    fs20Index = 0;
}

//------------------------------------------------------------------------------
// TX868 - Add bit to bit-buffer
//------------------------------------------------------------------------------
void objFs20::tx868AddBit(U8 aBit)
{
    /* Check overflow */
    if (fs20Index >= FS20_BUFFER_LEN) 
        return;
    
    fs20Buffer[fs20Index++] = aBit;
}

//------------------------------------------------------------------------------
// TX868 - Add byte with parity bit to bit-buffer
//------------------------------------------------------------------------------
void objFs20::tx868AddByte(U8 aByte)
{
    U8 parity = checkParity(aByte); 
    U8 bit = 0;
    
    for (U8 i=0; i<8; i++)
    {
        if ((aByte & (1 << (7 - i))) > 0) 
            bit=1; 
        else 
            bit=0;
        tx868AddBit(bit);
    }                        
    tx868AddBit(parity);
}    


//------------------------------------------------------------------------------
// TX868 - Send data-Buffer to TX-868 modul
//------------------------------------------------------------------------------
void objFs20::tx868Send(void)
{  
    /* Buffer empty */
    if (fs20Index == 0) 
        return;
    
    for (U16 i=0; i<FS20_BUFFER_LEN; i++)
    {
        switch (fs20Buffer[i])
        {
            case 1:
                digitalWrite(fs20DataPin, 1);
                delayMicroseconds(FS20_ONE);
                digitalWrite(fs20DataPin, 0);
                delayMicroseconds(FS20_ONE);
                break;

            case 0:
                digitalWrite(fs20DataPin, 1);
                delayMicroseconds(FS20_ZERO);
                digitalWrite(fs20DataPin, 0);
                delayMicroseconds(FS20_ZERO);                
                break;

            case 0xF:                
                return;
        }
    }
}

//------------------------------------------------------------------------------
// Internal - Count all HI bits
//------------------------------------------------------------------------------ 
U8 objFs20::countBit(U8 val)
{    
    U8 cnt=0;
    for (U8 i=0; i<8; i++)
    {
        if (val & (1 << (7 - i))) 
        {
            cnt++;
        }
    }    
    return cnt;
}

//------------------------------------------------------------------------------
// Internal - Check Parity Bit
//------------------------------------------------------------------------------
U8 objFs20::checkParity(U8 val)
{
    U8 c = countBit(val);  
    if (!(c%2)) 
        return 0; 
    else 
        return 1;
}

//------------------------------------------------------------------------------
 
#endif // CE_OBJ_FS20
// END OF objFs20.cpp
 
