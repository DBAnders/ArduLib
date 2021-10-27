//------------------------------------------------------------------------------
// File...: defGlobal.h                 
// Author.: M. Anders
// Date...: 17.02.2020  
//------------------------------------------------------------------------------
#ifndef _CPP_DEFGLOBAL
#define _CPP_DEFGLOBAL


/* My easy standard data types */
/*   = 0 : Standard types defined in "Arduino.h" system header file */
/*   = 1 : It's not defined in "Arduino.h" system header file */
#define STANDARD_TYPES_ENABLE   0

/*-----------------------------------------------------------------------------*/
/* I use my own easy data types: 8,16,32 Bit (signed and unsigned) */
/*-----------------------------------------------------------------------------*/
#if STANDARD_TYPES_ENABLE == 1
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef int8_t   I8;
typedef int16_t  I16;
typedef int32_t  I32;
/* Old pascal types */
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
#endif

/*-----------------------------------------------------------------------------*/
/* STANDARD BOOLEAN DEFINES */
/*-----------------------------------------------------------------------------*/
#define FALSE       (U8)0x00
#define TRUE        (U8)0x01
#define RESET       (U8)0xFF

/*-----------------------------------------------------------------------------*/
/* STANDARD FUNCTION RETURN VALUE */
/*-----------------------------------------------------------------------------*/
#define RET_OK      (U8)0x00
#define RET_ERROR   (U8)0x01
#define RET_BUSY    (U8)0x02
#define RET_TIMEOUT (U8)0x03

//------------------------------------------------------------------------------
// USER TOOLS
//------------------------------------------------------------------------------
/* rotate 'v' from 'a' to 'b' */
#define G_ROTATE(v,a,b)   ((v)<(b))?(v)+1:(a)
#define G_ROTREV(v,a,b)   ((v)>(a))?(v)-1:(b)

/* get absolut value from 'x' */
#define G_ABS(x)          ((int)(x)>0?(x):-(x))

/* size of elements 'x' */
#define G_LENGTH_OF(x)    (sizeof(x)/sizeof(x[0]))

/* limit value 'v' from 'l' */
#define G_LIMIT(v,l)      if ((v)>(l)) { (v) = (l); }

#endif // _CPP_DEFGLOBAL
