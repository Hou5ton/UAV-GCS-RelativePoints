#include <iostream>
#include "SysDefine.h"
#include "Refglobal.h"
using namespace std;
/*
int InternalMessage(char* str, unsigned nType, unsigned nIDHelp)
{
#ifndef _WINDOWS
  cout << str << "\n"; return 1;
#else
  return 0;
//return AfxMessageBox(str, nType, nIDHelp);
#endif
}
*/

void InternalMessage(QString msg) {
	if (verboseFlag)
	#ifdef WIN32 
		cout <<"***libMap***: "<< msg.toLocal8Bit().constData()<< endl;
	#else
		cout <<"***libMap***: "<< msg.toLocal8Bit().constData() << endl;
	#endif
}
//! -------------
//
void GetGlyphParams_SysDefine(void* dev, char ch, GLYPH_PARAMS& par,  M_LOG_FONT fn)
{
#ifdef _WINDOWS
	CDC* dc = new CDC;
	dc->CreateCompatibleDC(NULL);

	LOGFONT lf;
	lf.lfCharSet = fn.lfCharSet;
	lf.lfClipPrecision = fn.lfClipPrecision;
	lf.lfEscapement = fn.lfEscapement;
	memcpy(lf.lfFaceName, fn.lfFaceName, 32);
	lf.lfHeight = 1000;
	lf.lfItalic = fn.lfItalic;
	lf.lfOrientation = fn.lfOrientation;
	lf.lfOutPrecision = fn.lfOutPrecision;
	lf.lfPitchAndFamily = fn.lfPitchAndFamily;
	lf.lfQuality = fn.lfQuality;
	lf.lfStrikeOut = fn.lfStrikeOut;
	lf.lfUnderline = fn.lfUnderline;
	lf.lfWeight = fn.lfWeight;
	lf.lfWidth = fn.lfWidth;

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = dc->SelectObject(&Font);

	GLYPHMETRICS GLTEMP;

	MAT2 HITER; 
	HITER.eM11.fract = 0;
	HITER.eM11.value = 1;
	HITER.eM12.fract = 0;
	HITER.eM12.value = 0;
	HITER.eM21.fract = 0;
	HITER.eM21.value = 0;
	HITER.eM22.fract = 0;
	HITER.eM22.value = 1;

	if(dc->GetGlyphOutline(
		ch,          // character to query
		GGO_METRICS,        // data format
		&GLTEMP, // glyph metrics
		NULL,      // size of data buffer
		NULL,    // data buffer
		&HITER   // transformation matrix
		)==GDI_ERROR)  return;  //    exit(15);

	OUTLINETEXTMETRIC TM;
	dc->GetOutlineTextMetrics(sizeof(TM),&TM);
	MPlotDevice* pdev = (MPlotDevice*)dev;
    //'TM.otmMacAscent' is used as an indicator of a single piece in this font
    //A constant for transfer to logical coordinates of our font
	double toFont = 1*fabs(pdev->BumToLogic(fn.lfHeight))/TM.otmMacAscent;
    //We fill structure for delivery of parameters from the outside
    //Left part
	par.dLeftPart = (GLTEMP.gmptGlyphOrigin.x<0)?abs(GLTEMP.gmptGlyphOrigin.x):0;
	par.dLeftPart*=toFont;
    //Beginning of a glyph inside
	par.dGlyphStartInCenter = (GLTEMP.gmptGlyphOrigin.x>0)?GLTEMP.gmptGlyphOrigin.x:0;
	par.dGlyphStartInCenter*=toFont;
	par.dGlyphWidthInCenter=GLTEMP.gmCellIncX;
	par.dGlyphWidthInCenter*=toFont;

    //Right part of a glyph
	double dTemp = GLTEMP.gmBlackBoxX;
	if(GLTEMP.gmptGlyphOrigin.x<0)
		dTemp +=GLTEMP.gmptGlyphOrigin.x;
	else
		dTemp -=GLTEMP.gmptGlyphOrigin.x;
	par.dRightPart = ((dTemp - GLTEMP.gmCellIncX)>0)?(dTemp - GLTEMP.gmCellIncX):0;
	par.dRightPart*=toFont;
    //The center of a glyph in the middle
	par.dCenter = par.dGlyphWidthInCenter/2;

	dc->SelectObject(pOldFont);
	delete dc;
#else
#endif
}

int GetTextExtent_SysDefine(void* dev, char* str, int nCount,  M_LOG_FONT fn)
{
#ifdef _WINDOWS
	GLYPH_PARAMS par;
	double dVal =0;

	for(int i = nCount-1;i>=0;i--){
		GetGlyphParams_SysDefine(dev, str[i], par, fn);
		dVal+=par.dGlyphWidthInCenter;
	}
	return dVal;
#else
	return 1;	
#endif
}

//!  ----------------
void GetOutlineTextMetrics_SysDefine(M_LOG_FONT fn, int* otmAscent, int* otmDescent, int* otmMacAscent)
{
#ifdef _WINDOWS
	CDC* dc = new CDC;
	dc->CreateCompatibleDC(NULL);

	LOGFONT lf;
	lf.lfCharSet = fn.lfCharSet;
	lf.lfClipPrecision = fn.lfClipPrecision;
	lf.lfEscapement = fn.lfEscapement;
	memcpy(lf.lfFaceName, fn.lfFaceName, 32);
	lf.lfHeight = 1000;
	lf.lfItalic = fn.lfItalic;
	lf.lfOrientation = fn.lfOrientation;
	lf.lfOutPrecision = fn.lfOutPrecision;
	lf.lfPitchAndFamily = fn.lfPitchAndFamily;
	lf.lfQuality = fn.lfQuality;
	lf.lfStrikeOut = fn.lfStrikeOut;
	lf.lfUnderline = fn.lfUnderline;
	lf.lfWeight = fn.lfWeight;
	lf.lfWidth = fn.lfWidth;

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = dc->SelectObject(&Font);

	OUTLINETEXTMETRIC TM;
	dc->GetOutlineTextMetrics(sizeof(TM),&TM);
	*otmAscent = TM.otmAscent;
	*otmDescent = TM.otmDescent;
	*otmMacAscent = TM.otmMacAscent;

	dc->SelectObject(pOldFont);
	delete dc;
#else
#endif
}
