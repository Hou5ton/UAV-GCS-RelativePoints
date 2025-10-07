//PACKAGE : MAP.
//FILE: Refglobal.h   
//AUTHOR:
//DESCRIPTION:

#ifndef REFGLOBAL_H
#define REFGLOBAL_H

//#include "BaseCl.h"

// Following variables are described in varglobal.h

class MRect;

extern  char           mapCharset[];
extern  double         centralMeridian;

extern  bool           reverseReadFlag;
extern  bool           reverseValue(char*, int);
extern  bool           utpAdvancedFlag;
extern  double         axis_X,  axis_Y;
extern  float          fontValueLimit;
extern  bool           verboseFlag;
extern  bool           bHeightToZero;
extern	bool           matrixMemMap;

extern  float          zoomGeoLimit;
extern  bool           drawActFlag;
extern  int            actPolygonDense;

extern  bool           drawTextFlag;
extern  unsigned       distinguishTextSize;
extern  float          textRatio;
extern  float          actRatio;
extern  unsigned       textSizeMax;  
extern  MRect          zoomRectangle;
#endif


