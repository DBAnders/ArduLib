//------------------------------------------------------------------------------
// File...: objTempera.h                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
#ifndef _CPP_OBJTEMPERA
#define _CPP_OBJTEMPERA

//------------------------------------------------------------------------------
/* Modul DHT12               M5Stick-C ENV-Hut [DHT12,BMM150,BMP280]
 *   +------------------+      +-------------------------------+
 *   |  ##  ##  ##  ##  |      |    ##     ##     ##     ##    |
 *   |                  |      |    ##     ##     ##     ##    |
 *   |  ##  ##  ##  ##  |      |        ##     ##     ##       |
 *   |                  |      |    ##     ##     ##     ##    |
 *   |  ##  ##  ##  ##  |      |    ##     ##     ##     ##    |
 *   +-*----*----*----*-+      +-------------------------------+
 *     |    |    |    |          |   |   |   |   |   |   |   |
 *     |    |    |    |          |   |   |   |   |   |   |   | 
 *    VCC  SDA  GND  SCL         -  VCC  -  SDA  -  SCL  -  GND
 *------------------------------------------------------------------------------
 * Arduino Uno Pins:
 *   VCC -> (DHT12) 3,3 Volt
 *   SDA -> A4
 *   SCL -> A5
 *   GND
 *---------------------------------------------------------------------------- 
 *  Supply Voltage: 2,7 .. 5,5 Volt (Attention: M5Stick-C ENV Hut only 3,3V)
 *  Relative humidity: 20 .. 95% (Resolution 0,1%)
 *  Relative Temperature: -20  .. 60°C (Resolution 0,2°C) 
 */ 
//------------------------------------------------------------------------------
/* Initialize constructor with "Wire.begin();" */
#define TEMP_OPEN_WIRE

/* DHT12 I2C Address */
#define TEMP_I2C_ADDR      0x5C
#define TEMP_TXBUF_SIZE       5

//==============================================================================
// OBJECT CLASS: objTempera - Get temperatur and Humidity with DHT12
//==============================================================================
class objTempera
{
    public:
        /* Class constructor */
        objTempera(void);
        
        /* Initialize I2C port */
        bool Init(U8 bTempCAddr);
               
        /* Read temperatur and humidity data from i2c bus */
        bool ReadData(void);
        
        /* Get temperatur from buffer <-1000..+1000> [215 = 21,5°C] */
        S16 Temperatur(void);
                
        /* Get humidity from buffer <0..1000> [908 = 90,8%] */
        U16 Humidity(void);
        
    private:                
        U8 tempAddr;                
        U8 tempBuffer[TEMP_TXBUF_SIZE];                
        U8 checkSum();
};            

#endif // _CPP_OBJTEMPERA

