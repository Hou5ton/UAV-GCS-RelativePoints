//PACKAGE : MAP.
//FILE: Varglobal.h   
//AUTHOR:
//DESCRIPTION:

#ifndef VARGLOBAL_H
#define VARGLOBAL_H

//********************************************************************************//
//                 compilation-time section
//
// preferably to set proper parameters in this section before creating libMap.so, 
// otherwise you have to do that in your application using MKartaInterface class. 
//********************************************************************************//


// charset of map files
 char mapCharset[20]="Windows-1251"; //UTF-8
     
// mandatory if the map based on global(geodesic) coordinate system
double  centralMeridian= 27;//105

//--- binary data reverse read flag  
//--- set true if the map files were created on another platform (e.g. Intel<->SPARC )  

//false for WINDOWS,LiNUX (Intel)
//bool reverseReadFlag = true;      //  !!!SOLARIS
 unsigned short reverseTestVar = 1; /* 0x0001 */
 bool reverseReadFlag = *((unsigned char *) &reverseTestVar) == 0 ? true /*sparc big-endian*/: false/*x86 little-endian*/;


//set this value to true if need to work with memory-mapped matrix file
bool matrixMemMap = false;

//--- set your ellipsoid parameters
// Pulkovo-42, Belarus
double  axis_X  = 6378245,  axis_Y  = 6356863.02;
// WGS-84, 
//double  axis_X  = 6378137,  axis_Y  = 6356752.314;
   

//--- set true if  UTP-file contains objects and subobjects 
bool        utpAdvancedFlag   = false;

//--- OLD horizontal zoom-in limit (grads)
//float       zoomGeoLimit = 0.08;      // Belarus
//float       zoomGeoLimit = 0.25;      // 

//--- text size ratio limit
// Belarus
float       fontValueLimit = 12.0;
//float       fontValueLimit    = 2.0;      // 

//--- to log messages into the standard output?
//  false - no messages
bool        verboseFlag = true;  

//--- true, if KartaInterface functions
//GetHightRelief();GetHightObjects();GetHightCorrection();
//should return 0 (zero) instead of -20000
//when there's no value in the AltitudeMatrix
bool	bHeightToZero = true/*false*/;

//******************** end of section *************************//


//========================================//
// leave everything below untouched
//========================================//
//  EACH SIZE BELOW IN PIXELS 

//--- horizontal zoom-in limit (degrees)
// sredniaja (constant) 
 float  zoomGeoLimit = 0.0027F;      


// drawing actLayer polygons with brush DENSE
// runtime variable
bool   drawActFlag;             

// runtime variable
 int    actPolygonDense = 0;//7     

// drawing text with font SIZE(height)
// runtime variable
bool        drawTextFlag;             

// parameter
 unsigned    distinguishTextSize = 6;  

// parameter
 unsigned    textSizeMax = 200;

// parameter
 float       textRatio = 1.0;          

// runtime variable
float       actRatio;                 

// drawing objects with size objectSizeMin and more
// parameter
unsigned    objectSizeMin = 3;        

// current zoom geo rectangle
// runtime variable
MRect       zoomRectangle;            

#endif //VARGLOBAL_H

