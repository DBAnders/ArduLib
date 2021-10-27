//------------------------------------------------------------------------------
// File...: objFs20.h
// Author.: M. Anders
// Date...: 24.06.2020
//------------------------------------------------------------------------------
#ifndef _CPP_OBJFS20
#define _CPP_OBJFS20

//------------------------------------------------------------------------------
/* TX Modul (ELV/eQ-3) with FS20 (ST-3) protocol:
 *      ------------+
 *    /  +--------+ |     Class:        Short Range Device (SRD) Class 1 
 *   |   |        | |     Type:         TX868-75
 *   |   +--------+ |     Frequence:    868,35 MHz 
 *   |              |     Tolerance:    max. +/-75 kHz
 *   |              |     Drift:        max. 100 ppm
 *   |              |     Modulation:   ASK, 100%
 *   |              |     Transferrate: max. 10 kbit/s
 *   +-*--*--*------+     Voltage:      2,0 .. 3,0 Volt
 *     |  |  |            Current:      14mA (50% Grade); 28mA (100% Grade)
 *     |  |  | 
 *     1  2  3 ------ Data Input (0 / 3V)
 *     |  +---------- Ground /GND
 *     +------------- UB+ 3V
 * 
 * Protocol for FS20 (Wireless Power Switch, ELV / eQ-3)  
 * -----------------------------------------------------------------------------
 * U6 homecode : 0x0000 .. 0xFFFF
 * -----------------------------------------------------------------------------
 * U8 addrByte : High-Nibble -> Function Group (F=Master; 0..E=Single)
 *               Low-Nibble -> Under Address (F=All Groups; 0..E=Single)
 * -----------------------------------------------------------------------------
 * U8 cmdbyte  : State/Information
 *     0x00    : OFF   0,00%
 *     0x01    : Dimm  6,25%
 *     0x02    : Dimm 12,50%
 *    ........ : ..........
 *     0x0F    : Dimm 93,75%
 *     0x10    : ON  100,00%
 *     0x11    : ON, old dimmer value
 *     0x12    : Toggle Mode
 *     0x13    : Dimm up ++
 *     0x14)   : Dimm down --
 *     0x15    : Dim up and down..
 *    ........ : .......... 
 *     0x1B    : Reset (Delivery State)
 *
 * Checksum: 0x06 + homeCode_H + omeCode_L + addrByte + cmdByte
 *
 * "Synchr:13 -- HC1:8 -- P:1 -- HC2:8 -- P:1 -- Adr:8 -- P:1 -- Cmd:8 -- P:1"
 *
 * Data Bit: (Send / Receive)
 *     0: 400us High,  400us Low  =  600..1000 us Period length
 *     1: 600us High,  600us Low  = 1000..1450 us Period length
 */
//------------------------------------------------------------------------------

/* Internal bit buffer */
#define FS20_BUFFER_LEN  96

//==============================================================================
// OBJECT CLASS: objFs20 - FS20 ELV Tx868 Modul
//==============================================================================
class objFs20
{
    public:
        /* Class constructor */
        objFs20(void);

        /* Initialize FS20 ELV Tx868 Modul */ 
        bool Init(U8 dataPin);
                
        /* Send FS20 Actor Data */
        void Send(U16 homeCode, U8 addrByte, U8 cmdByte);

        /* Switch FS20 Actor ON or OFF */
        void Switch(U16 homeCode, U8 addrByte, bool swOn);
        
        /* Toggle FS20 Actor ON or OFF */
        void Switch(U16 homeCode, U8 addrByte);

        /* Dimm FS20 Actor (value between 0..16) */
        void Dimm(U16 homeCode, U8 addrByte, U8 dimmValue);
   
    private:        
        U8 fs20Buffer[FS20_BUFFER_LEN];        
        U8 fs20DataPin;        
        U16 fs20Index;        
        void SendEx(U16 homeCode, U8 addrByte, U8 cmdByte);
        void tx868Clear(void);
        void tx868AddBit(U8 aBit);
        void tx868AddByte(U8 aByte);
        void tx868Send(void);
        U8 countBit(U8 val);
        U8 checkParity(U8 val);

};
 
#endif // _CPP_OBJFS20
 
