//------------------------------------------------------------------------------
// File...: objRadio.h                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
// OBJECT CLASS: objRadioo - TEA5767, RDA5807, etc..
//------------------------------------------------------------------------------
#ifndef _CPP_OBJRADIO
#define _CPP_OBJRADIO

//------------------------------------------------------------------------------
// Select your "Radio Tuner Chip"
//------------------------------------------------------------------------------
//#define RADIO_TEA_5767  // I2C Chip ID = 0x60
#define RADIO_RDA_5807M  // I2C Chip ID = 0x10
//#define RADIO_SI4705 <-- not supported!

//------------------------------------------------------------------------------
// carrier circuit board (Please check your radio board first)
//------------------------------------------------------------------------------
/*            TEA-5767                           RDA-5807M
 *         +------------+                     +------------+
 *   SDA --| 1       10 +-- ANT         SDA --| 1       10 +-- ANT
 *   SCL --| 2        9 +-- NC          SCL --| 2        9 +-- NC
 *   GND --| 3        8 +-- R_OUT       NC  --| 3        8 +-- R_OUT 
 *   NC  --| 4        7 +-- L_OUT       NC  --| 4        7 +-- L_OUT
 *   5V  --| 5        6 +-- GND       3,3V  --| 5        6 +-- GND
 *         +------------+                     +------------+
 * Not chip pinout! */ 
//------------------------------------------------------------------------------

/* Initialize constructor with "Wire.begin();" */
//#define RADIO_OPEN_WIRE

/* Enabe Debugmode -> Use "DumpBuffer()" */
//#define RADIO_DEBUG_MODE

//------------------------------------------------------------------------------
/* Change datatyp for radio register */
#ifdef RADIO_TEA_5767
  #define RADIO_DAT       U8
  #define RADIO_BUF_SIZE  10
  #define RADIO_ICC_ADDR  0x60  
#else
#ifdef RADIO_RDA_5807M
  #define RADIO_DAT       U8
  #define RADIO_BUF_SIZE  22
  #define RADIO_ICC_ADDR  0x10  
#else
#ifdef RADIO_SI4705
  #pragma message ("+++WARNING: This CHIP is not supported!+++")
#else  
  #pragma message ("+++WARNING: No Radio CHIP selected!+++")
#endif  
#endif    
#endif

//------------------------------------------------------------------------------
/* Frequence range on FM band */
#define RADIO_FMIN  8700
#define RADIO_FMAX 10800

/* Step for "Tune_UP/N" */
#define RADIO_STEP_5   5
#define RADIO_STEP_10 10

/* Tune frequence up or down in 50 kHz steps */
#define RADIO_TUNE_UP(fq,st) if((fq)<RADIO_FMAX) (fq)+=(st);else (fq)=RADIO_FMIN
#define RADIO_TUNE_DN(fq,st) if((fq)>RADIO_FMIN) (fq)-=(st);else (fq)=RADIO_FMAX

//==============================================================================
// OBJECT CLASS: objRadio - TEA5767, RDA5807M, ... 
//==============================================================================
class objRadio
{
    public:
        objRadio(void);
        
        /* Initialize radio chip (TEA5767, RDA5807M, .. ) */
        /* Use I2C address define -> deviceAddress=RADIO_ICC_ADDRC */
        void Init(U8 deviceAddress);                       
        
        /* Set FM frequence (87,6 MHz -> iFrequence=8760) */
        void SetFrequence(U16 iFrequence);
        
        /* (only RDA5807M) Set volume (100% -> iVolume=15) */
        void SetVolume(U8 iVolume);
        
        /* Enable or disable sound (ON->bMute=false; OFF->bMute=true ) */
        void SetMute(bool bMute);
        
        /* (only RDA5807M) Activate Bass-Boost (ON->bBass=true; OFF->bBass=false) */
        void SetBass(bool bBass);
        
        /* (only RDA5807M) Activate Mono (ON->bBass=true; OFF->bBass=false) */        
        void SetMono(bool bMono);
        
        /* (only TEA5767) Get signal level */
        U8 GetSignalLevel(U8 *sigLevel, U8 *bStereo);
        
        /* Print internal regBuffer (ONLY Serial Debug) */
        void DumpBuffer(void);

    private:                     
        /* Internal I2C device address */
        U8 devAddr;
        
        /* Internal register buffer */
        RADIO_DAT regBuffer[RADIO_BUF_SIZE];
        
        /* Internal frequence as backup */
        U16 intFreq;
        
        /* Internal communication functions */
        void SendRegister(U8 iRegister);        
        void SendMessage(U8 msgLen);        
        void ReceiveMessage(U8 recLen);
};            

#endif // _CPP_OBJRADIO

