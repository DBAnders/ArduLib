//------------------------------------------------------------------------------
// File...: objRadio.cpp                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
// OBJECT CLASS: objRadioo - TEA5767, RDA5807, etc..
//------------------------------------------------------------------------------
#include "classEnable.h"
#ifdef CE_OBJ_RADIO
#include <Arduino.h>
#include <Wire.h>
#include "objRadio.h"

//------------------------------------------------------------------------------
#ifdef RADIO_TEA_5767
/* Radio ChipConfiguration TEA5767 */
//-----------------------------------------------------------------------------
// REGISTER: MUTE / TUNING 1
//-----------------------------------------------------------------------------
#define RADIO_00_REG     0x00 
#define RADIO_00_VALUE   0b00000000 
    /* DMUTE (7) - mute enable
     * 0 = normal operation
     * 1 = L and R audio are muted
     *
     * SM (6) - Search mode
     * 1 = in search mode
     * 0 = not in search mode
     *
     * PLL[5:0] (13:8) - synthesizer programmable counter for search/preset     
     * 00 0000
     */
//-----------------------------------------------------------------------------
// REGISTER: TUNING 2
//-----------------------------------------------------------------------------
#define RADIO_01_REG     0x01 
#define RADIO_01_VALUE   0b00000000
    /* PLL[7:0] (7:0) - synthesizer programmable counter for search/preset     
     * 0000 0000
     */
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 1
//-----------------------------------------------------------------------------
#define RADIO_02_REG     0x02 
#define RADIO_02_VALUE   0b10110000
    /* SUD (7) - Search Up/Down
     * 1 = search up
     * 0 = search down
     *
     * SSL[1:0] (6:5) - Search Stop Level
     * 00 = not allowed in search mode
     * 01 = low; level ADC output = 5
     * 10 = mid; level ADC output = 7
     * 11 = high; level ADC output = 10
     *
     * HLSI (4) - High/Low Side Injection
     * 1 = high side LO injection
     * 0 = low side LO injection
     *
     * MS (3) - Mono to Stereo
     * 1 = forced mono
     * 0 = stereo ON
     *
     * MR (2) - Mute Right
     * 1 = the right audio channel is muted and forced mono
     * 0 = the right audio channel is not muted
     *
     * ML (1) - Mute Left
     * 1 = the left audio channel is muted and forced mono
     * 0 = the left audio channel is not muted
     *
     * SWP1 (0) - Software programmable port 1
     * 1 = port 1 is HIGH
     * 0 = port 1 is LOW
     */  
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 2
//-----------------------------------------------------------------------------
#define RADIO_03_REG     0x03 
#define RADIO_03_VALUE   0b00010000
    /* SWP2 (7) - Software programmable port 2
     * 1 = port 2 is HIGH
     * 0 = port 2 is LOW
     *
     * STBY (6) - Standby
     * 1 = Standby mode
     * 0 = not in Standby mode
     *
     * BL (5) - Band Limits
     * 1 = Japanese FM band
     * 0 = US/Europe FM band
     *
     * XTAL (4) - Clock frequency
     * 00 = 13 MHz
     * 01 = 32.768 kHz
     * 10 = 6.5 MHz
     * 11 = not allowed
     *
     * SMUTE (3) - Soft Mute
     * 1 = soft mute is ON
     * 0 = soft mute is OFF
     *
     * HCC (2) - High Cut Control
     * 1 = high cut control is ON
     * 0 = high cut control is OFF
     *
     * SNC (1) - Stereo Noise Cancelling
     * 1 = stereo noise cancelling is ON
     * 0 = stereo noise cancelling is OFF
     *
     * SI (0) - Search Indicator
     * 1 = pin SWPORT1 is output for the ready flag
     * 0 = pin SWPORT1 is software programmable port 1
     */
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 3
//-----------------------------------------------------------------------------
#define RADIO_04_REG     0x04
#define RADIO_04_VALUE   0x00000000
    /* PLLREF (7) 
     * 1 = 6.5 MHz reference frequency for the PLL is enabled
     * 0 = 6.5 MHz reference frequency for the PLL is disabled (see XTAL)
     *
     * DTC (6) 
     * 1 = the de-emphasis time constant is 75 ms
     * 0 = the de-emphasis time constant is 50 ms
     *
     * RESERVED (5:0) - not used; position is don’t care
     */
//-----------------------------------------------------------------------------
#define RADIO_READ_OFS   5
#define RADIO_SEND_LEN   5
#define RADIO_READ_LEN   5
#endif

//------------------------------------------------------------------------------
#ifdef RADIO_RDA_5807M
/* Radio ChipConfiguration RDA5807M */
#define RADIO_00_REG    0x00 // Register not writable: CHIP ID
#define RADIO_01_REG    0x01 // Register not writable: unused!
//-----------------------------------------------------------------------------
// ### WRITE REGISTER ###
//-----------------------------------------------------------------------------
// REGISTER: MODE and OPTIONS
//-----------------------------------------------------------------------------
#define RADIO_02_REG    0x02
#define RADIO_H2_BUF    0x00
#define RADIO_H2_VALUE  0b10010000
    /* DHIZ (15) - audio output high-z disable
     * 1 = normal operation
     *
     * DMUTE (14) - mute disable 
     * 1 = normal operation
     *
     * MONO (13) - mono select
     * 0 = stereo
     *
     * BASS (12) - bass boost
     * 0 = disabled
     *
     * RCLK NON CALIBRATE MODE (11)
     * 0 = RCLK is always supplied
     *
     * RCLK DIRECT INPUT MODE (10)
     * 0 = ??? not certain what this does
     *
     * SEEKUP (9)
     * 0 = seek in down direction
     *
     * SEEK (8)
     * 0 = disable / stop seek (i.e. don't seek)
     */                      
#define RADIO_L2_BUF    0x01
#define RADIO_L2_VALUE  0b00000000
    /* SKMODE (7) - seek mode: 
     * 0 = wrap at upper or lower band limit and contiue seeking
     *
     * CLK_MODE (6:4) - clock mode
     * 000 = 32.768kHZ clock rate (match the watch cystal on the module) 
     *
     * RDS_EN (3) - radio data system enable
     * 0 = disable radio data system
     *
     * NEW_METHOD (2) - use new demodulate method for improved sensitivity
     * 0 = presumably disabled 
     *
     * SOFT_RESET (1)
     * 1 = perform a reset
     *
     * ENABLE (0) - power up enable 
     * 0 = enabled
     */     
//-----------------------------------------------------------------------------
// REGISTER: TUNING
//-----------------------------------------------------------------------------
#define RADIO_03_REG    0x03
#define RADIO_H3_BUF    0x02
#define RADIO_H3_VALUE  0b00000000
    /* CHAN (15:8) - channel select 8 most significant bits of 10 in total
     * 0000 0000 = don't boher to program a channel at this time
     */  
#define RADIO_L3_BUF    0x03
#define RADIO_L3_VALUE  0b00000000
    /* CHAN (7:6) - two least significant bits of 10 in total 
     * 00 = don't bother to program a channel at this time
     *
     * DIRECT MODE (5) - used only when test
     * 0 = presumably disabled
     *
     * TUNE (4) - commence tune operation 
     * 0 = disable (i.e. don't tune to selected channel)
     *
     * BAND (3:2) - band select
     * 00 = select the 87-108MHz band
     *
     * SPACE (1:0) - channel spacing
     * 00 = select spacing of 100kHz between channels     
     * 01 = 200 kHz
     * 10 = 50kHz
     * 11 = 25KHz
     */                   
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 1
//-----------------------------------------------------------------------------
#define RADIO_04_REG    0x04
#define RADIO_H4_BUF    0x04
#define RADIO_H4_VALUE  0b00000000 
    /* RESERVED (15)
     *
     * STCIEN (14) - Seek/Tune Complete Interrupt Enable
     * 0 = Disable Interrupt
     * 1 = Enable Interrupt
     *
     * RBDS (13)
     * 1 = RBDS mode enable
     * 0 = RDS mode only
     *
     * RDS_FIFO_EN (12)
     * 1 = RDS fifo mode enable
     *
     * DE de-emphasis (11)
     * 0 = 75 μs 
     * 1 = 50 μs
     *
     * RDS_FIFO_CLR (10)
     * 1 = clear RDS fifoRE
     *
     * SOFTMUTE_EN (9)
     * 1 = soft mute enabled
     *
     * AFCD (8) - AFC disable
     * 0 = AFC enabled
     */               
#define RADIO_L4_BUF    0x05
#define RADIO_L4_VALUE  0b01000000
    /* RSVD (7) 
     * Read as 0!
     *
     * I2S_ENABLE (6)    
     * 0 = disabled
     * 1 = enabled
     *
     * GPIO3[1:0] (5:4) - General Purpose I/O 3
     * 00 = High impedance
     * 01 = Mono/Stereo indicator (ST)
     * 10 = Low
     * 11 = High
     *
     * GPIO2[1:0] (3:2) - General Purpose I/O 2
     * 00 = High impedance
     * 01 = Interrupt (INT)
     * 10 = Low
     * 11 = High
     *
     * GPIO1[1:0] (1:0) - General Purpose I/O 1
     * 00 = High impedance
     * 01 = Reserved
     * 10 = Low
     * 11 = High
     */ 
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 2
//-----------------------------------------------------------------------------
#define RADIO_05_REG    0x05
#define RADIO_H5_BUF    0x06
#define RADIO_H5_VALUE  0b10010000
    /* INT_MODE (15)
     * 1 = interrupt last until read reg 0x0C
     *
     * SEEK:MODE[1:0] (14:13) 
     * 00=Default; 10=will add the RSSI seek mode
     *
     * RSVD (12)
     *
     * SEEKTH[3:0] (11:8) - Seek signal to noise ratio threshold
     * 1000 = suggested defaultSeek SNR threshold
     */   
#define RADIO_L5_BUF    0x07
#define RADIO_L5_VALUE  0b11101000
    /* LNA_PORT_SEL[1:0] (7:6) - LNA input port selection bit
     * 00 = no input
     * 01 = LNAN
     * 10 = LNAP
     * 11 = dual port input
     *
     * LNA_ICSEL_BIT[1:0] (5:4) - Lna working current bit
     * 00 = 1.8mA
     * 01 = 2.1mA
     * 10 = 2.5mA
     * 11 = 3.0mA    
     *
     * VOLUME[3:0] (3:0) - Changel output volume
     * 1111 = loudest volume
     */                 
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 3
//-----------------------------------------------------------------------------
#define RADIO_06_REG    0x06
#define RADIO_H6_BUF    0x08
#define RADIO_H6_VALUE  0b00000000
    /* RESERVED (15)
     * 0
     *
     * OPEN_MODE[1:0] (14:13) - open reserved registers mode
     * 00 = suggested default
     
     * SLAVE_MASTER (12) - slave_master I2S slave or master
     * 1 = slave; 0 = master
     *
     * WS_LR (11) - Ws relation to l/r channel
     * 0 = ws=0 ->r, ws=1 ->l
     * 1 = ws=0 ->l, ws=1 ->r
     * 
     * SCLK_I_EDGE (10)
     * 0 = use normal sclk internally
     * 1 = inverte sclk internally
     *
     * DATA_SIGNED (9)
     * 0 = I2S output unsigned 16-bit audio data
     * 1 = I2S output signed 16-bit audio data
     *
     * WS_I_EDGE (8)
     * 0 = use normal ws internally
     * 1 = inverte ws internally
     */     
#define RADIO_L6_BUF    0x09
#define RADIO_L6_VALUE  0b00000000
    /* I2S_SW_CNT[3:0] (7:4) - Only valid in master mode
     * 4'b1000 = WS_STEP = 48
     * 4'b0111 = WS_STEP = 44.1kbps
     * 4'b0110 = WS_STEP = 32kbps
     * 4'b0101 = WS_STEP = 24kbps
     * 4'b0100 = WS_STEP = 22.05kbps
     * 4'b0011 = WS_STEP = 16kbps
     * 4'b0010 = WS_STEP = 12kbps
     * 4'b0001 = WS_STEP = 11.025kbps
     * 4'b0000 = WS_STEP = 8kbps
     *
     * SW_O_EDGE (3)
     * 1 = invert ws output when as master 
     *
     * SCLK_O_EDGE (2)
     * 1 = invert sclk output when as master
     *
     * L_DELY (1)
     * 1 = L channel data delay 1T
     *
     * R_DELY (0)
     * 1 = R channel data delay 1T     
     */          
//-----------------------------------------------------------------------------
// REGISTER: CONFIGURATION 4
//-----------------------------------------------------------------------------
#define RADIO_07_REG    0x07
#define RADIO_H7_BUF    0x0A
#define RADIO_H7_VALUE  0b01000010
    /* RESERVED (15) 
     * 0
     *
     * TH_SOFRBLEND[4:0] (14:10) - threshhold for noise soft blend setting
     * 10000 = using default value
     *
     * 65M_50M MODE (9) - Valid when band[1:0] = 2’b11 (0x03H_bit<3:2>)      
     * 1 = 65~76 MHz
     * 0 = 50~76 MHz
     *
     * RESERVED (8)
     * 0
     */        
#define RADIO_L7_BUF    0x0B
#define RADIO_L7_VALUE  0b00000010
    /* SEEK_TH_OLD seek[5:0] (7:2) - threshold for old seek mode
     * 000000
     *
     * SOFTBLEND_EN (1) - soft blend enable
     * 1 = using default value
     *
     * FREQ_MODE (0)
     * 0 = using defualt value
     * 1 = then freq setting changed
     * Freq = 76000(or 87000) kHz + freq_direct (08H) kHz
     */        
//-----------------------------------------------------------------------------
// REGISTER: FREQUENCE SELECT
//-----------------------------------------------------------------------------
#define RADIO_08_REG    0x08
#define RADIO_H8_BUF    0x0C
#define RADIO_H8_VALUE  0b00000000
    /* FREQ_DIRECT (15:8) - HI-BYTE - Valid when FREQ_MODE = 1
     * 0000 0000 
     */    
#define RADIO_L8_BUF    0x0D
#define RADIO_L8_VALUE  0b00000000
    /* FREQ_DIRECT (7:0) - HI-BYTE - Valid when FREQ_MODE = 1
     * 0000 0000 
     */    
//-----------------------------------------------------------------------------
// REGISTER: 09 UNUSED II
//-----------------------------------------------------------------------------
#define RADIO_09_REG    0x09 // Register not used
//-----------------------------------------------------------------------------
// ### READ REGISTER ###
//-----------------------------------------------------------------------------
// REGISTER: STATUS I
//-----------------------------------------------------------------------------
#define RADIO_10_REG    0x0A
#define RADIO_H0_STATUS 0x10
#define RADIO_L0_STATUS 0x11
    /* RDS READY (15) 
     * 0 = No RDS/RBDS group ready(default)
     * 1 = New RDS/RBDS group ready
     *
     * STC Seek/Tune Complete  (14)
     * 0 = Not complete
     * 1 = Complete
     *
     * SF Seek Fail (13)
     * 0 = Seek successful
     * 1 = Seek failure
     *
     * RDSS RDS Synchronization (12)
     * 0 = RDS decoder not synchronized(default)
     * 1 = RDS decoder synchronized
     *
     * BLK_E When RDS enable (11)
     * 1 = Block E has been found
     * 0 = no Block E has been found
     * 
     * ST Stereo Indicator (10)
     * 0 = Mono
     * 1 = Stereo
     * 
     * READCHAN[9:0] (9:0) Read Channel
     * Frequency = Channel Spacing (kHz) x READCHAN[9:0] + 87.0 MHz
     */        
//-----------------------------------------------------------------------------
// REGISTER: STATUS II
//-----------------------------------------------------------------------------
#define RADIO_11_REG    0x0B
#define RADIO_H1_STATUS 0x12
#define RADIO_L1_STATUS 0x13          
    /* RSSI[6:0] (15:9)
     * 0000000 = min
     * 1111111 = max -> RSSI scale is logarithmic.
     * 
     * FM TRUE (8)
     * 1 = the current channel is a station
     * 0 = the current channel is not a station
     *      
     * FM_READY (7) 
     * 1=ready;  0=not ready
     * 
     * RESERVED (6:5) 
     * 00
     *
     * ABCD_E (4) 
     * 1 = the block id of register 0cH, 0dH, 0eH, 0fH is E
     * 0 = the block id of register 0cH, 0dH, 0eH, 0fH is A, B, C, D     
     *     
     * BLERA[1:0] (3:2)
     * Block Errors Level of RDS_DATA_0
     * BLERB[1:0] (1:0)
     * Block Errors Level of RDS_DATA_1
     * (see documentation of RDA5708)     
     */         
//-----------------------------------------------------------------------------
// REGISTER: RDS - RADIO DATA SYSTEM 
//-----------------------------------------------------------------------------
    /* register 0x0C = READ REGEISTER 3 - RDS BLOCK A / E */  
#define RADIO_12_REG    0x0C
#define RADIO_H2_STATUS 0x14
#define RADIO_L2_STATUS 0x15          
    /* register 0x0D = READ REGEISTER 4 - RDS BLOCK B / E */  
#define RADIO_13_REG    0x0D
#define RADIO_H3_STATUS 0x14
#define RADIO_L3_STATUS 0x15          
    /* register 0x0E = READ REGEISTER 5 - RDS BLOCK C / E */       
#define RADIO_14_REG    0x0E
#define RADIO_H4_STATUS 0x14
#define RADIO_L4_STATUS 0x15            
    /* register 0x0F = READ REGEISTER 6 - RDS BLOCK D / E */  
#define RADIO_15_REG    0x0F
#define RADIO_H5_STATUS 0x14
#define RADIO_L5_STATUS 0x15          
//-----------------------------------------------------------------------------
// END OF CONFIG AND DATA STRUCTURE
//-----------------------------------------------------------------------------
/* End of marker and Read offset */
#define RADIO_READ_OFS   16
#define RADIO_INIT_LEN    8
#define RADIO_TUNE_LEN    4
#define RADIO_READ_LEN    2
#endif
 
//------------------------------------------------------------------------------
// CONSTRUCTOR
//------------------------------------------------------------------------------
objRadio::objRadio(void)
{
    devAddr = 0;   
    intFreq = 0;
#ifdef RADIO_TEA_5767    
    U8 reg = RADIO_00_REG;
    /* Configure TEA 5767 regBuffer */
    regBuffer[reg++] = RADIO_00_VALUE;                                                    
    regBuffer[reg++] = RADIO_01_VALUE; 
    regBuffer[reg++] = RADIO_02_VALUE;
    regBuffer[reg++] = RADIO_03_VALUE;
    regBuffer[reg++] = RADIO_04_VALUE;         
#else
#ifdef RADIO_RDA_5807M 
    U8 reg = RADIO_H2_BUF;
    /* Configure RDA 5807 regBuffer */
    regBuffer[reg++] = RADIO_H2_VALUE;
    regBuffer[reg++] = RADIO_L2_VALUE;
    regBuffer[reg++] = RADIO_H3_VALUE;
    regBuffer[reg++] = RADIO_L3_VALUE;
    regBuffer[reg++] = RADIO_H4_VALUE;
    regBuffer[reg++] = RADIO_L4_VALUE;
    regBuffer[reg++] = RADIO_H5_VALUE;
    regBuffer[reg++] = RADIO_L5_VALUE;
#else
#pragma message ("+++WARNING: This CHIP is not supported!+++")
#endif
#endif   
}

//------------------------------------------------------------------------------
void objRadio::Init(U8 deviceAddress)
{    
    devAddr = deviceAddress;          
    //TWBR=12;
#ifdef RADIO_OPEN_WIRE        
    Wire.begin();
#endif          
    delay(500);
    
#ifdef RADIO_TEA_5767
    Wire.beginTransmission(devAddr);  
    Wire.write(regBuffer, RADIO_SEND_LEN);
    Wire.endTransmission(); 
#else
#ifdef RADIO_RDA_5807M                        
    U16 channel = 166;
 
    regBuffer[RADIO_H2_BUF] = 0x00;
    regBuffer[RADIO_L2_BUF] = 0x02;
    Wire.beginTransmission(devAddr);  
    Wire.write(regBuffer, RADIO_INIT_LEN);
    Wire.endTransmission();
    
    delay(500);
    
    regBuffer[RADIO_H2_BUF] = 0xD0; 
    regBuffer[RADIO_L2_BUF] = 0x01;
    Wire.beginTransmission(devAddr);  
    Wire.write(regBuffer, RADIO_INIT_LEN);
    Wire.endTransmission();
         
    delay(10);
    regBuffer[RADIO_H2_BUF] = 0b11000000;
    regBuffer[RADIO_L2_BUF] = 0b00000001;    
    regBuffer[RADIO_H3_BUF] = (channel >> 2); 
    regBuffer[RADIO_L3_BUF] = ((channel & 0b11) << 6 ) | 0b00010000;    
   
    Wire.beginTransmission(devAddr);
    Wire.write(regBuffer, RADIO_TUNE_LEN);
    Wire.endTransmission();    
   
#endif
#endif                  
    delay(100);    
}

//------------------------------------------------------------------------------
void objRadio::SendRegister(U8 iRegister)
{    
#ifdef RADIO_TEA_5767
    Wire.beginTransmission(devAddr);     
    Wire.write(iRegister); 
    Wire.write(regBuffer[iRegister]);
    Wire.endTransmission();        
    delay(100);  
#else
#ifdef RADIO_RDA_5807M                        
    U8 bufOffset = (iRegister * 2) - 4;
    Wire.beginTransmission(devAddr + 1);
    Wire.write(iRegister);
    Wire.write(regBuffer[bufOffset]);
    Wire.write(regBuffer[bufOffset + 1]);
    Wire.endTransmission(); 
#endif
#endif         
}

//------------------------------------------------------------------------------
void objRadio::SendMessage(U8 msgLen)
{
    Wire.beginTransmission(devAddr);
    Wire.write(regBuffer, msgLen);
    Wire.endTransmission();    
}

//------------------------------------------------------------------------------
void objRadio::ReceiveMessage(U8 recLen)
{
    /* reading TEA5767 or RDA6807 */   
    regBuffer[RADIO_READ_OFS] = 0;
    Wire.requestFrom((uint8_t)devAddr, (uint8_t)recLen); 
    if (Wire.available()) 
    {
        for (int i=0; i<recLen; i++)
        {
            regBuffer[RADIO_READ_OFS + i] = Wire.read();
        }
    }    
    Wire.endTransmission();    
 }    

//------------------------------------------------------------------------------
void objRadio::SetFrequence(U16 iFrequence)
{                
#ifdef RADIO_TEA_5767    
    intFreq = ((U32)iFrequence * 10000 + 225000) / 8192; 
    regBuffer[RADIO_00_REG] = ((intFreq >> 8) & 0x3F);
    regBuffer[RADIO_01_REG] = intFreq & 0XFF;                      
    SendMessage(5);    
#else  
#ifdef RADIO_RDA_5807M        
    U16 channel = (iFrequence - 8700) / 10;       
    
    regBuffer[RADIO_H3_BUF] = (channel >> 2); 
    regBuffer[RADIO_L3_BUF] = ((channel & 0b11) << 6 ) | 0b00010000;     
        
    SendRegister(RADIO_03_REG); 
#endif
#endif    
}

//------------------------------------------------------------------------------
void objRadio::SetVolume(U8 iVolume)
{
#ifdef RADIO_TEA_5767  
// NONE
#else
#ifdef RADIO_RDA_5807M
    if (iVolume <= 15)
    {       
        regBuffer[RADIO_L5_BUF] &= (~0x0F);
        regBuffer[RADIO_L5_BUF] |= iVolume;       
        SendRegister(RADIO_05_REG);
    }   
#endif
#endif  
}   
  
//------------------------------------------------------------------------------
void objRadio::SetMute(bool bMute)
{ 
#ifdef RADIO_TEA_5767      
    // Mute_On:MR[2]=1,ML[1]=1; 
    // Mute_Off:MR[2]=00,ML[1]=0 (0x06)
    if (bMute == 0)         
        regBuffer[RADIO_02_REG] &= (~0x06);     
    else
        regBuffer[RADIO_02_REG] |= 0x06;     
    SendMessage(RADIO_SEND_LEN);      
#else   
#ifdef RADIO_RDA_5807M
    if (bMute == 0)         
        regBuffer[RADIO_H2_BUF] |= 0x40;
    else
        regBuffer[RADIO_H2_BUF] &= (~0x40);    
    SendRegister(RADIO_02_REG);    
#endif
#endif      
}

//------------------------------------------------------------------------------
void objRadio::SetBass(bool bBass)
{
#ifdef RADIO_TEA_5767  
// NONE
#else
#ifdef RADIO_RDA_5807M         
    if (bBass == 0)                 
        regBuffer[RADIO_H2_BUF] &= (~0x10);    
    else
        regBuffer[RADIO_H2_BUF] |= 0x10;
        
    SendRegister(RADIO_02_REG);  
#endif
#endif 
}

//------------------------------------------------------------------------------
void objRadio::SetMono(bool bMono)
{
#ifdef RADIO_TEA_5767  
    // Mono_On:MS[3]=1; 
    // Mono_Off/Stereo:MS[3]=0; (0x08)
    if (bMono == 0)         
        regBuffer[RADIO_02_REG] &= (~0x06);    
    else
        regBuffer[RADIO_02_REG] |= 0x06;     
#else
#ifdef RADIO_RDA_5807M         
    if (bMono == 0)         
        regBuffer[RADIO_H2_BUF] |= 0x20;
    else
        regBuffer[RADIO_H2_BUF] &= (~0x20);    
    SendRegister(RADIO_02_REG); 
#endif
#endif     
}


//------------------------------------------------------------------------------
U8 objRadio::GetSignalLevel(U8 *sigLevel, U8 *bStereo)
{    
    U8 ret = 0;   
#ifdef RADIO_TEA_5767      
    /* Receive message fail */
    ReceiveMessage(RADIO_READ_LEN);    
    ret = (regBuffer[RADIO_READ_OFS + RADIO_00_REG] == 0) ? 0 : 1;
    *bStereo = (regBuffer[RADIO_READ_OFS + RADIO_02_REG] & 0x80) ?  1 : 0;
    *sigLevel = (regBuffer[RADIO_READ_OFS + RADIO_03_REG] >> 4);
#else    
#ifdef RADIO_RDA_5807M         
    /*  Not supported! */
    *bStereo = 0;
    *sigLevel = 0;
#endif
#endif      
    return ret;
}    

//------------------------------------------------------------------------------
void objRadio::DumpBuffer(void)
{
#ifdef RADIO_DEBUG_MODE    
    Serial.print("DeviceID: ");
    Serial.println(devAddr, HEX);
    
    ReceiveMessage(RADIO_READ_LEN);
    
    for (U8 i=0; i<RADIO_BUF_SIZE; i++)
    {
        Serial.print(i, DEC);
        Serial.print(": ");
        Serial.println(regBuffer[i], BIN);
    }    
#endif 
}
#endif // CE_OBJ_RADIO
// END OF objRadioCpp                 