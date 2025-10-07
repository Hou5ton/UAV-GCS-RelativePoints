//PACKAGE : MAP.
//FILE: Sysdefine.h   
//AUTHOR:
//DESCRIPTION:

#ifndef SYSDEFINE_H
#define SYSDEFINE_H

// File defines som functions, whuich specific for Windows | Linux

#ifdef _WINDOWS
#include <afxwin.h>
#endif

//Specially for compiling a whole English version.
//Always make sure to change!
#ifndef ENGVERSION 
#define ENGVERSION 1
#endif

#include <QString>
#include "BaseCl.h"

#ifndef _WINDOWS
#define RGN_NOT 0
#define RGN_AND 1
#define RGN_OR  2
typedef void* LPVOID;
#endif

#ifndef PS_SOLID
#define PS_SOLID 0
#endif

#ifndef PS_DOTGRID
#define PS_DOTGRID 13
#endif

#ifndef PS_NULL
#define PS_NULL 5
#endif

#ifndef BLACKNESS
#define BLACKNESS 0x00000042
#endif

#ifndef WHITENESS
#define WHITENESS 0x00FF0062
#endif

#ifndef _WINDOWS
#define NULL_PEN 8
#define PS_ENDCAP_FLAT 0x00
#define PS_ENDCAP_ROUND 0x20
#define PS_JOIN_ROUND 0x80
#define PS_GEOMETRIC 0x00
#define FW_BOLD 700
#define FW_NORMAL 400
/*#define NULL_PEN 8
#define PS_ENDCAP_FLAT 0x00000200
#define PS_ENDCAP_ROUND 0x00000000
#define PS_JOIN_ROUND 0x00000000
#define PS_GEOMETRIC 0x00010000
#define FW_BOLD 700
#define FW_NORMAL 400*/
#endif

// int InternalMessage(char* str, unsigned nType = 0, unsigned nIDHelp = 0);

//PACKAGE : MAP.
//FUNCTION : InternalMessage(QString msg).
//DESCRIPTION : Outputs inner debugging message.
//INPUTS : QString.
//RETURNS : NONE.
void InternalMessage(QString msg);

//PACKAGE : MAP.
//FUNCTION : GetGlyphParams_SysDefine(void* dev, char ch, GLYPH_PARAMS& par, M_LOG_FONT fn).
//DESCRIPTION : Gets parameters of nomenclature.
//INPUTS : void pointer, char , GLYPH_PARAMS& , M_LOG_FONT .
//RETURNS : NONE.
void GetGlyphParams_SysDefine(void* dev, char ch, GLYPH_PARAMS& par, M_LOG_FONT fn);

//PACKAGE : MAP.
//FUNCTION : GetTextExtent_SysDefine(void* dev, char* str, int nCount, M_LOG_FONT fn).
//DESCRIPTION : Gets text string width.
//INPUTS : void pointer, char pointer, int , M_LOG_FONT .
//RETURNS : int.
int GetTextExtent_SysDefine(void* dev, char* str, int nCount, M_LOG_FONT fn);

//PACKAGE : MAP.
//FUNCTION : GetOutlineTextMetrics_SysDefine(M_LOG_FONT fn, int* otmAscent, int* otmDescent, int* otmMacAscent);
//DESCRIPTION : Gets font parameters.
//INPUTS : M_LOG_FONT , int pointer, int pointer, int pointer.
//RETURNS : NONE.
void GetOutlineTextMetrics_SysDefine(M_LOG_FONT fn, int* otmAscent, int* otmDescent, int* otmMacAscent);

#ifndef Min
#define Min(a,b)	(((a)<(b)) ? (a) : (b))
#endif

#ifndef Max
#define Max(a,b)	(((a)>(b)) ? (a) : (b))
#endif

#ifndef IS_EDIT_KLS
#define IS_EDIT_KLS 0
#endif

#endif //SYSDEFINE_H
