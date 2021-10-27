//------------------------------------------------------------------------------
// File...: objTempera.cpp                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
// OBJECT CLASS: objTempera - Get temperatur and Humidity with DHT12
//------------------------------------------------------------------------------
#include "classEnable.h"
#ifdef CE_OBJ_TEMPERA
#include <Arduino.h>
#include <Wire.h>
#include "objTempera.h"

//------------------------------------------------------------------------------        
// Internal defines
//------------------------------------------------------------------------------        
#define TEMP_REG_HUMI_H      0
#define TEMP_REG_HUMI_L      1
#define TEMP_REG_TEMP_H      2
#define TEMP_REG_TEMP_L      3
#define TEMP_REG_CHECKSUM    4

//------------------------------------------------------------------------------        
// Class constructor 
//------------------------------------------------------------------------------        
objTempera::objTempera(void)
{     
    tempAddr = TEMP_I2C_ADDR;
}

//------------------------------------------------------------------------------        
//  Initialize I2C port
//------------------------------------------------------------------------------
bool objTempera::Init(U8 bTempAddr)
{   
    if (bTempAddr != 0)
    {
        tempAddr = bTempAddr;
    }  
#ifdef TEMP_OPEN_WIRE 
    Wire.begin();
#endif 
    Wire.beginTransmission(tempAddr);
    if (Wire.endTransmission() != 0) 
    {
        return false; // ERROR 
    }     
    return true; // OK
}

//------------------------------------------------------------------------------        
// Read temperatur and humidity data from i2c bus
//------------------------------------------------------------------------------        
bool objTempera::ReadData(void)
{    
    Wire.beginTransmission(tempAddr);
    Wire.write(0);
    Wire.endTransmission();
    delay(50);
    Wire.requestFrom((int)tempAddr, (int)TEMP_TXBUF_SIZE);
    for (U8 i=0; i<TEMP_TXBUF_SIZE; i++)
    {
        tempBuffer[i] = Wire.read();
    };
    delay(100);    
    Wire.endTransmission();
    
    Serial.print("Buffer: ");
    for (U8 i=0; i<TEMP_TXBUF_SIZE; i++)
    {        
        Serial.print(tempBuffer[i], DEC);
        Serial.print(", ");
    }     
    Serial.println(" ");
        
    if (tempBuffer[TEMP_REG_CHECKSUM] == checkSum()) 
    {
        return true; // OK
    }       
    return false;// CKSUM ERROR
}

//------------------------------------------------------------------------------
// Get temperatur from buffer <-1000..+1000> [215 = 21,5°C] 
//------------------------------------------------------------------------------
// Temperature:00011010 (Binary) => 26 (Decimal)00000110(Binary) => 6 (Decimal)
// =>Temperature = 26.6 degrees
//------------------------------------------------------------------------------
S16 objTempera::Temperatur(void)
{
    S16 temp = 0;
    U8 sign = tempBuffer[TEMP_REG_TEMP_L] & 0x80;
    temp += (S16)tempBuffer[TEMP_REG_TEMP_H] * 10;
    temp += (S16)tempBuffer[TEMP_REG_TEMP_L] & 0x0F;
    if (sign)
    {
        temp = -temp;
    }
    return temp;
}

//------------------------------------------------------------------------------        
// Get humidity from buffer <0..1000> [908 = 90,8%]
//------------------------------------------------------------------------------        
// Humidity:00111000 (Binary) => 56 (Decimal)00001000(Binary) => 8 (Decimal)
// =>Humidity = 56.8 % RH
//------------------------------------------------------------------------------        
U16 objTempera::Humidity(void)
{
    U16 humi = 0;
    humi += (U16)tempBuffer[TEMP_REG_HUMI_H] * 10;
    humi += (U16)tempBuffer[TEMP_REG_HUMI_L];
    return humi;
}

//------------------------------------------------------------------------------        
// PRIVATE: Calculate checksum
//------------------------------------------------------------------------------        
U8 objTempera::checkSum()
{
    U8 ckSum = 0;
    for (U8 i=0; i<TEMP_REG_CHECKSUM; i++)
    {
        ckSum += (U8)tempBuffer[i];
    }        
    return ckSum;
}

#endif // CE_OBJ_TEMPERA
// END OF objTemperacpp                 
