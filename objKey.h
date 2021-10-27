//------------------------------------------------------------------------------
// File...: objKey.h                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
#ifndef _CPP_OBJKEY
#define _CPP_OBJKEY

/* Define number of KEY elements */
#define KEY_CNT_MAX 8

//==============================================================================
// OBJECT CLASS: objKey - Multi Key Manager (Taster- Entprellung)
//==============================================================================
class objKey
{
    public:
        /* Class constructor */ 
        objKey(void);
        
        /* Insert new KEY and connect with GPIO "pinNumber" */
        bool Insert(U8 pinNumber);
        
        /* return TRUE if KEY "keyIndex" pressed and wait until release */
        bool KeyPressed(U8 keyIndex);
        
        /* return TRUE if KEY "keyIndex" pressed */
        bool KeyDown(U8 keyIndex);        
        
         /* Check if KEY "keyIndex" pressed and wait until release */
        void KeyWait(U8 keyIndex);        

    private:        
        U8 keyPin[KEY_CNT_MAX];        
        U8 keyCnt=0;
        bool isKeyRange(U8 keyIndex);
};            

#endif // _CPP_OBJKEY

