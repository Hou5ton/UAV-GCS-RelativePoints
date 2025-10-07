//PACKAGE : MAP.
//FILE: DrawObj.h      
//AUTHOR:
//DESCRIPTION:

#ifndef _DRAWOBJ__H_
#define _DRAWOBJ__H_
#include "Topobject.h"
#include "PlotDevice.h"
#include "MString.h"

// Constants are defined here. With theirs help we perform simplyfied 
// drawing of base language primitives.
// Value sets in a logical units.
// if font less than value it just not drawing
// Following constants are used to spread boundary symbol and pattern.
#define VISIBLE_FONT_SB_STRETCH  1
#define VISIBLE_FONT_BOUND_SYM   1
#define VISIBLE_FONT_UZOR		 1
#define VISIBLE_FONT_SB_SINGULAR 1
#define VISIBLE_FONT_HANDLE		 1

// Constant of conversion hatched line to continuous
#define VISIBLE_SHTRIH_STEP_PLUS_LENGTH 1
// Constant of conversion dotted line to continuous
#define VISIBLE_PUNKTIR_STEP			1

// Constant of visibility when  hatching mustn't be drawn
#define VISIBLE_SHTRIHOVKA_STEP			0.5			

// Constant at which the dot mark is seen 
#define VISIBLE_POINT				1

// Constant of a seen font
#define VISIBLE_FORMULA				1

///Structure used for transmit between functions in a percental drawing
struct PercentParameters
{
	MSymbol FirstSym;
	MSymbol SecSym;
	MSymbol ThirdSym;
	int iFirstFont;
	int iSecFont;
	int iThirdFont;
	unsigned char iFirstPercent;
	unsigned char iSecPercent;
	unsigned char iThirdPercent;
	MColor cr1;
	MColor cr2;
	MColor cr3;
};

//PACKAGE : MAP.
//CLASS	: MDrawing.
//DESCRIPTION : Class contains functions, which ones necessary to draw different 
//				geometric primitives.
//				All members of this class are static,
//				it means cannot exist N object of this class,
//				but we can call class function at any place.
class MDrawing
{
protected:
	
	// Text drawing inside defined area
	static inline void DrawTextWithinArea(MPlotDevice* pDevice,MPoint pt,MPtArray* pta,MString* str,double dSymVal);

public:
	// To all function members we must pass:
	// pointer to function of convertion real coordinates to logical (for points).
	// Also pointer to function of convertion used sizes to logical units

	// ------------------------------------------------------------------
	// Following functions are used while drawing background of area objects 
	// by color, pattern, hatching, percental pattern.
	
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawZapolnenie(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,	MPtArray* pta);
//DESCRIPTION : Background drawing by layers.
//INPUTS : MTopobject pointer,MYaz pointer,MPlotDevice pointer, MPtArray pointer.
//RETURNS : NONE.	
	static void DrawZapolnenie(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,
		MPtArray* pta);
    static void DrawZapolnenieZone(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,
        MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPlast(MTopobject* pObj,MPlotDevice* pDevice, MPlast* pPlast,MPtArray* pta);
//DESCRIPTION : Depending on filling type
// 				calls defined functions of filling layers drawing.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MPlast pointer,MPtArray pointer.
//RETURNS : NONE.	
	static void DrawPlast(MTopobject* pObj,MPlotDevice* pDevice,
		MPlast* pPlast,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawShtrihovka(MTopobject* pObj,MPlotDevice* pDevice, MShtrihovka* pShtrihovka,MPtArray* pta);
//DESCRIPTION : Draws hatched filling.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MShtrihovka pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawShtrihovka(MTopobject* pObj,MPlotDevice* pDevice,
		MShtrihovka* pShtrihovka,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPattern(MTopobject* pObj,MPlotDevice* pDevice, MPattern* pPattern, MPtArray* pta);
//DESCRIPTION : Fills by one-color.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MPattern pointer, MPtArray pointer.
//RETURNS : NONE.
	static void DrawPattern(MTopobject* pObj,MPlotDevice* pDevice,
		MPattern* pPattern, MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawZalivka(MTopobject* pObj,MPlotDevice* pDevice, MZalivka* pZalivka,MPtArray* pta);
//DESCRIPTION : Fills by one-color.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MZalivka pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawZalivka(MTopobject* pObj,MPlotDevice* pDevice,
		MZalivka* pZalivka,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawUzor(MTopobject* pObj,MPlotDevice* pDevice,MUzor* pUzor,MPtArray* pta);
//DESCRIPTION : Fills by pattern.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MUzor pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawUzor(MTopobject* pObj,MPlotDevice* pDevice,
		MUzor* pUzor,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPramSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect, MString* str,double step,double dSymVal);
//DESCRIPTION : Fills rectangles by symbols grid.
//INPUTS : MPlotDevice pointer,MPtArray pointer,MRect pointer,MString pointer,double ,double .
//RETURNS : NONE.
	static void DrawPramSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
		MString* str,double step,double dSymVal);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawKosSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,MString* str,double angle,double step,double dSymVal);
//DESCRIPTION : Fills by oblique symbols grid.
//INPUTS : MPlotDevice pointer,MPtArray pointer,MRect pointer,MString pointer,double ,double ,double .
//RETURNS : NONE.
	static void DrawKosSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
		MString* str,double angle,double step,double dSymVal);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawXaosSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,MSymbol* sym,double step,double dSymVal);
//DESCRIPTION : Fills by symbols chaoticly.
//INPUTS : MPlotDevice pointer,MPtArray pointer,MRect pointer,MSymbol pointer,double ,double .
//RETURNS : NONE.
	static void DrawXaosSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
		MSymbol* sym,double step,double dSymVal);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPercentUzor(MTopobject* pObj,MPlotDevice* pDevice, MPercentUzor* pUzor,MPtArray* pta);
//DESCRIPTION : Fills by pattern percently.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MPercentUzor pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawPercentUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MPercentUzor* pUzor,MPtArray* pta);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPramSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,PercentParameters* params,double step,double dSymVal);
//DESCRIPTION : Fills by rectangular symbols grid percently.
//INPUTS : MPlotDevice pointer,MPtArray pointer,MRect pointer,PercentParameters pointer,double ,double .
//RETURNS : NONE.	
	static void DrawPramSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
		PercentParameters* params,double step,double dSymVal);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawKosSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,PercentParameters* params,double angle,double step,double dSymVal);
//DESCRIPTION :  Fills by oblique symbols grid percently.
//INPUTS : MPlotDevice pointer,MPtArray pointer,MRect pointer,PercentParameters pointer,double ,double ,double .
//RETURNS : NONE.
	static void DrawKosSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
		PercentParameters* params,double angle,double step,double dSymVal);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawXaosSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,PercentParameters* params,double step,double dSymVal);
//DESCRIPTION : Fills by symbols percently and chaoticly.
//INPUTS : MPlotDevice pointer,MPtArray pointer,MRect pointer,PercentParameters pointer,double ,double .
//RETURNS : NONE.
	static void DrawXaosSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
		PercentParameters* params,double step,double dSymVal);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrShtrihovka(MTopobject* pObj,MPlotDevice* pDevice,MNastrShtrihovka* pShtrihovka,MPtArray* pta);
//DESCRIPTION : Draws adjustable hatched filling.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MNastrShtrihovka pointer,MPtArray pointer.
//RETURNS : NONE.	
	static void DrawNastrShtrihovka(MTopobject* pObj,MPlotDevice* pDevice,
								MNastrShtrihovka* pShtrihovka,MPtArray* pta);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrUzor(MTopobject* pObj,MPlotDevice* pDevice,  MNastrUzor* pUzor,MPtArray* pta);
//DESCRIPTION : Draws adjustable filling by pattern.
//INPUTS : MTopobject pointer,MPlotDevice pointer,  MNastrUzor pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawNastrUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MNastrUzor* pUzor,MPtArray* pta);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrPercentUzor(MTopobject* pObj,MPlotDevice* pDevice,  MNastrPercentUzor* pUzor,MPtArray* pta);
//DESCRIPTION : Draws percentage filling by pattern.
//INPUTS : MTopobject pointer,MPlotDevice pointer,  MNastrPercentUzor pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawNastrPercentUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MNastrPercentUzor* pUzor,MPtArray* pta);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawHandle(MTopobject* pObj, MPlotDevice* pDevice, MHandleArea* pHandle, MPoint point);
//DESCRIPTION : Draws singular symbol in the area (special pattern).
//INPUTS : MTopobject pointer, MPlotDevice pointer, MHandleArea pointer, MPoint .
//RETURNS : NONE.	
	static void DrawHandle(MTopobject* pObj, MPlotDevice* pDevice, MHandleArea* pHandle, MPoint point);
	
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrHandle(MTopobject* pObj, MPlotDevice* pDevice, MNastrHandleArea* pHandle, MPoint point);
//DESCRIPTION : Draws adjustable singular symbol in the area (special pattern).
//INPUTS : MTopobject pointer, MPlotDevice pointer, MNastrHandleArea pointer, MPoint .
//RETURNS : NONE.
	static void DrawNastrHandle(MTopobject* pObj, MPlotDevice* pDevice, MNastrHandleArea* pHandle, MPoint point);

//PACKAGE : MAP.
//FUNCTION : MDrawing::GetHandlePlastPoint(MTopobject* pObj, MPtArray* pta, int index, int count);
//DESCRIPTION : Calculates the point of singular symbol placement. function is used in DrawHandle & DrawNastrHandle.
//INPUTS : MTopobject pointer, MPtArray pointer, int , int .
//RETURNS : MPoint.
	static MPoint GetHandlePlastPoint(MTopobject* pObj, MPtArray* pta, int index, int count);

	// ------------------------------------------------------------------
	// Following functions we use while draw different objects contours:
	// straight, hatched, dotted etc.

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawLine(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,MPtArray* pta,MRzArray* razryvy = NULL);
//DESCRIPTION : Draws line by layers.
//INPUTS : MTopobject pointer,MYaz pointer,MPlotDevice pointer,MPtArray pointer,MRzArray pointer.
//RETURNS : NONE.	
	static void DrawLine(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,
						MPtArray* pta,MRzArray* razryvy = NULL);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawProsloika(MTopobject* pObj,MPlotDevice* pDevice,MProsloika* prosloika,MPtArray* pta, MRzArray* razryvy);
//DESCRIPTION : Calls layer drawing depended on type.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MProsloika pointer,MPtArray pointer, MRzArray pointer.
//RETURNS : NONE.
	static void DrawProsloika(MTopobject* pObj,MPlotDevice* pDevice,
						MProsloika* prosloika,MPtArray* pta, MRzArray* razryvy);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawProsloikaOriginal(MTopobject* pObj,MPlotDevice* pDevice,MProsloika* prosloika,MPtArray* pta);
//DESCRIPTION : Calls layer drawing depended on type.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MProsloika pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawProsloikaOriginal(MTopobject* pObj,MPlotDevice* pDevice,
						MProsloika* prosloika,MPtArray* pta);
	
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawProsloikaPls(MTopobject* pObj,MPlotDevice* pDevice,MProsloika* prosloika,MPtArray* pta,MRzArray* razryvy);
//DESCRIPTION : Draws layer of stripped objects.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MProsloika pointer,MPtArray* pta,MRzArray pointer.
//RETURNS : NONE.
	static void DrawProsloikaPls(MTopobject* pObj,MPlotDevice* pDevice,
						MProsloika* prosloika,MPtArray* pta,MRzArray* razryvy);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawLinia(MTopobject* pObj,MPlotDevice* pDevice,MLinia* linia,MPtArray* ptarr);
//DESCRIPTION : Draws continuous line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MLinia pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawLinia(MTopobject* pObj,MPlotDevice* pDevice,MLinia* linia,
							MPtArray* ptarr);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawShtrih(MTopobject* pObj,MPlotDevice* pDevice,MShtrih* shtrih,MPtArray* ptarr, MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws hatched line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MShtrih pointer,MPtArray pointer, MPtArray pointer,bool .
//RETURNS : NONE.	
	static void DrawShtrih(MTopobject* pObj,MPlotDevice* pDevice,MShtrih* shtrih,
							MPtArray* ptarr, MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPunktir(MTopobject* pObj,MPlotDevice* pDevice,MPunktir* punktir,MPtArray* ptarr, MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws dotted line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MPunktir pointer,MPtArray pointer, MPtArray pointer,bool .
//RETURNS : NONE.
	static void DrawPunktir(MTopobject* pObj,MPlotDevice* pDevice,MPunktir* punktir,
							MPtArray* ptarr, MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawSbStretch(MTopobject* pObj,MPlotDevice* pDevice,MSbStretch* SbStretch,MPtArray* ptarr,MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws many equal symbols stretched along line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MSbStretch pointer,MPtArray pointer,MPtArray pointer,bool .
//RETURNS : NONE.
	static void DrawSbStretch(MTopobject* pObj,MPlotDevice* pDevice,MSbStretch* SbStretch,
							MPtArray* ptarr,MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawRangeSym(MTopobject* pObj,MPlotDevice* pDevice,MRangeSym* RangeSym,MPtArray* ptarr);
//DESCRIPTION : Draws boundary symbol.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MRangeSym pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawRangeSym(MTopobject* pObj,MPlotDevice* pDevice,MRangeSym* RangeSym,
							MPtArray* ptarr);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawSpline(MTopobject* pObj,MPlotDevice* pDevice,MSpline* spline,MPtArray* ptarr);
//DESCRIPTION : Draws spline.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MSpline pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawSpline(MTopobject* pObj,MPlotDevice* pDevice,
							MSpline* spline,MPtArray* ptarr);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawOriented(MTopobject* pObj,MPlotDevice *pDevice,MOrientedStretch* m_linia,MPtArray* ptarr,MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws oriented symbol streching.
//INPUTS : MTopobject pointer,MPlotDevice  pointer,MOrientedStretch pointer,MPtArray pointer,MPtArray pointer,bool .
//RETURNS : NONE.
	static void DrawOriented(MTopobject* pObj,MPlotDevice *pDevice,MOrientedStretch* m_linia,
							MPtArray* ptarr,MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrLinia(MTopobject* pObj,MPlotDevice* pDevice,MNastrLinia* linia,MPtArray* ptarr);
//DESCRIPTION : Draws adjustable continuous line.
//INPUTS : MTopobject* pObj,MPlotDevice* pDevice,MNastrLinia* linia,MPtArray* ptarr.
//RETURNS : NONE.
	static void DrawNastrLinia(MTopobject* pObj,MPlotDevice* pDevice,MNastrLinia* linia,
							MPtArray* ptarr);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrShtrih(MTopobject* pObj,MPlotDevice* pDevice,MNastrShtrih* shtrih,MPtArray* ptarr, MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws adjustable hatched line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MNastrShtrih pointer,MPtArray pointer, MPtArray pointer,bool .
//RETURNS : NONE.
	static void DrawNastrShtrih(MTopobject* pObj,MPlotDevice* pDevice,MNastrShtrih* shtrih,
							MPtArray* ptarr, MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrPunktir(MTopobject* pObj,MPlotDevice* pDevice,MNastrPunktir* punktir,MPtArray* ptarr, MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws adjustable dotted line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MNastrPunktir pointer,MPtArray pointer, MPtArray pointer,bool .
//RETURNS : NONE.
	static void DrawNastrPunktir(MTopobject* pObj,MPlotDevice* pDevice,MNastrPunktir* punktir,
							MPtArray* ptarr, MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrSbStretch(MTopobject* pObj,MPlotDevice* pDevice,MNastrSbStretch* SbStretch,MPtArray* ptarr,MPtArray* axearr,bool synhro);
//DESCRIPTION : Draws adjustable symbol stretching along line.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MNastrSbStretch pointer,MPtArray pointer,MPtArray pointer,bool .
//RETURNS : NONE.
	static void DrawNastrSbStretch(MTopobject* pObj,MPlotDevice* pDevice,MNastrSbStretch* SbStretch,
							MPtArray* ptarr,MPtArray* axearr,bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrSpline(MTopobject* pObj,MPlotDevice* pDevice,MNastrSpline* spline,MPtArray* ptarr);
//DESCRIPTION : Draws adjustable spline.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MNastrSpline pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawNastrSpline(MTopobject* pObj,MPlotDevice* pDevice,
							MNastrSpline* spline,MPtArray* ptarr);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawSingular(MTopobject* pObj, MPlotDevice* pDevice, MSbSingular* singular,MPtArray* ptarr, MPtArray* axearr, bool synhro);
//DESCRIPTION : Draws singular Symbol.
//INPUTS : MTopobject pointer, MPlotDevice pointer, MSbSingular pointer,MPtArray pointer, MPtArray pointer, bool .
//RETURNS : NONE.	
	static void DrawSingular(MTopobject* pObj, MPlotDevice* pDevice, MSbSingular* singular,
							MPtArray* ptarr, MPtArray* axearr, bool synhro);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrSingular(MTopobject* pObj, MPlotDevice* pDevice, MNastrSbSingular* singular,MPtArray* ptarr, MPtArray* axearr, bool synhro);
//DESCRIPTION : Draws adjustable singular Symbol.
//INPUTS : MTopobject pointer, MPlotDevice pointer, MNastrSbSingular pointer,MPtArray pointer, MPtArray pointer, bool .
//RETURNS : NONE.
	static void DrawNastrSingular(MTopobject* pObj, MPlotDevice* pDevice, MNastrSbSingular* singular,
							MPtArray* ptarr, MPtArray* axearr, bool synhro);
	
	// ------------------------------------------------------------------
	// Following functions we use when draw dotted objects:

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPoint(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Draws point by cuts.
//INPUTS : MTopobject pointer,MYaz pointer,MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.	
	// Draws point by cuts
	static void DrawPoint(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawShriftSrez(MTopobject* pObj,MPlotDevice* pDevice,MShriftSrez* pSrez,MPtArray* pta);
//DESCRIPTION : Draws font cut.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MShriftSrez pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawShriftSrez(MTopobject* pObj,MPlotDevice* pDevice,
								MShriftSrez* pSrez,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawImageSrez(MTopobject* pObj,MPlotDevice* pDevice,MProizvolSrez* pSrez,MPtArray* pta);
//DESCRIPTION : Draws arbitrary font cut.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MProizvolSrez pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawImageSrez(MTopobject* pObj,MPlotDevice* pDevice,
								MProizvolSrez* pSrez,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawNastrShriftSrez(MTopobject* pObj,MPlotDevice* pDevice,MNastrShriftSrez* pSrez,MPtArray* pta);
//DESCRIPTION : Draws  adjustable font cut.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MNastrShriftSrez pointer,MPtArray pointer.
//RETURNS : NONE.
	static void DrawNastrShriftSrez(MTopobject* pObj,MPlotDevice* pDevice,
								MNastrShriftSrez* pSrez,MPtArray* pta);
	

	// ------------------------------------------------------------------
	// Following functions we use for formulas drawing:

//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawText(MTopobject* pObj,MPlotDevice* pDevice,MYaz* pYaz,MPvArray* privs);
//DESCRIPTION : Draws text at object.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MYaz pointer,MPvArray pointer.
//RETURNS : NONE.	
	static void DrawText(MTopobject* pObj,MPlotDevice* pDevice,MYaz* pYaz,
						MPvArray* privs);
	//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawFormula(MTopobject* pObj,MPlotDevice* pDevice, MPtArray* pta, MFormula* formula);
//DESCRIPTION : Draws formulas.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MPtArray pointer, MFormula pointer.
//RETURNS : NONE.
	static void DrawFormula(MTopobject* pObj,MPlotDevice* pDevice, 
							MPtArray* pta, MFormula* formula);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawFormulaSpline(MTopobject* pObj,MPlotDevice* pDevice, MPtArray* pta, MFormula* formula);
//DESCRIPTION : Draws formula by spline: uses function 'DrawPredlogenieBySpline'.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MPtArray pointer, MFormula pointer.
//RETURNS : NONE.
	static void DrawFormulaSpline(MTopobject* pObj,MPlotDevice* pDevice, 
									MPtArray* pta, MFormula* formula);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawFragment(MTopobject* pObj,MPlotDevice* pDevice, MPtArray* pta, MFragment* fragment);
//DESCRIPTION : Draws formula fragment.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MPtArray pointer, MFragment pointer.
//RETURNS : NONE.	
	static void  DrawFragment(MTopobject* pObj,MPlotDevice* pDevice, 
							MPtArray* pta, MFragment* fragment);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPredlogenie(MTopobject* pObj,MPlotDevice* pDevice, int type, MPoint geo_pt, MStArray* sta, MString* pre_mega, MString* post_mega, MPrk* p_prk);
//DESCRIPTION : Draws sentence .
//INPUTS : MTopobject pointer,MPlotDevice pointer, int , MPoint , MStArray pointer, MString pointer, MString pointer, MPrk pointer.
//RETURNS : NONE.
	static void DrawPredlogenie(MTopobject* pObj,MPlotDevice* pDevice, int type, 
								MPoint geo_pt, MStArray* sta, MString* pre_mega, 
								MString* post_mega, MPrk* p_prk);
//PACKAGE : MAP.
//FUNCTION : MDrawing::DrawPredlogenieBySpline(MTopobject* pObj, MPlotDevice* pDevice, MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MString str, MString prefix, MString postfix, MPrk* p_prk);
//DESCRIPTION : Draws sentence along spline.
//INPUTS : MTopobject pointer, MPlotDevice pointer, MPoint , MPoint , MPoint , MPoint , MString , MString , MString , MPrk pointer.
//RETURNS : NONE.
	static void DrawPredlogenieBySpline(MTopobject* pObj, MPlotDevice* pDevice, 
									MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, 
									MString str, MString prefix, MString postfix, MPrk* p_prk);

//PACKAGE : MAP.
//FUNCTION : MDrawing::GetFormulaDlina(MTopobject* pObj,MPlotDevice* pDevice, MFormula* formula);
//DESCRIPTION :  Gets formula lenght: uses function of 'GetStrofaDlina'.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MFormula pointer.
//RETURNS : NONE.
	static double	GetFormulaDlina(MTopobject* pObj,MPlotDevice* pDevice, MFormula* formula);

//PACKAGE : MAP.
//FUNCTION : MDrawing::GetStrofaDlina(MTopobject* pObj,MPlotDevice* pDevice, MStrofa* strofa);
//DESCRIPTION : Gets strophe lenght
//  			Uses functions of 'ObtainMassivStrok' and 'GetPredlogenieDlina'.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MStrofa pointer.
//RETURNS : Double.
	static double  GetStrofaDlina(MTopobject* pObj,MPlotDevice* pDevice, MStrofa* strofa);

//PACKAGE : MAP.
//FUNCTION : MDrawing::GetPredlogenieDlina(MTopobject* pObj,MPlotDevice* pDevice, int type, MStArray* sta, MString* pre_mega, MString* post_mega, MPrk* p_prk);
//DESCRIPTION : Gets sentence lenght.
//INPUTS : MTopobject pointer,MPlotDevice pointer, int , MStArray pointer, MString pointer, MString pointer, MPrk pointer.
//RETURNS : Double.
	static double GetPredlogenieDlina(MTopobject* pObj,MPlotDevice* pDevice, 
									int type, MStArray* sta, MString* pre_mega, 
									MString* post_mega, MPrk* p_prk);

//PACKAGE : MAP.
//FUNCTION : MDrawing::GetPredloginiasDlina(MTopobject* pObj,MPlotDevice* pDevice,MStrofa* pStrofa,MDuArray& LengthArray);
//DESCRIPTION :  Gets lenght of couple sentences.
//INPUTS : MTopobject pointer,MPlotDevice pointer,MStrofa pointer,MDuArray& .
//RETURNS : NONE.
	static bool GetPredloginiasDlina(MTopobject* pObj,MPlotDevice* pDevice,
									MStrofa* pStrofa,MDuArray& LengthArray);
//PACKAGE : MAP.
//FUNCTION : MDrawing::GetFragmentVysotaUp(MTopobject* pObj,MPlotDevice* pDevice, MFragment* fragment);
//DESCRIPTION : Gets fragment altitude (higher than altitude of base font line)
//				Uses functions 'ObtainMassivStrok' and 'GetPredlogenieVysota'.
//INPUTS : MTopobject pointer,MPlotDevice pointer, MFragment pointer.
//RETURNS : Double.
	static double  GetFragmentVysotaUp(MTopobject* pObj,MPlotDevice* pDevice, 
										MFragment* fragment);

//PACKAGE : MAP.
//FUNCTION : MDrawing::GetFragmentVysotaDown(MTopobject* pObj,MPlotDevice* pDevice, MFragment* fragment);
//DESCRIPTION : Gets fragment altitude (lower than altitude of base font line).
//INPUTS : MTopobject pointer,MPlotDevice pointer, MFragment pointer.
//RETURNS : Double.
	static double  GetFragmentVysotaDown(MTopobject* pObj,MPlotDevice* pDevice, MFragment* fragment);
//PACKAGE : MAP.
//FUNCTION : MDrawing::GetPredlogenieVysota(MTopobject* pObj,MPlotDevice* pDevice,int type, MStArray* sta, MPrk* p_prk);
//DESCRIPTION : Gets sentence altitude.
//INPUTS : MTopobject pointer,MPlotDevice pointer,int , MStArray pointer, MPrk pointer.
//RETURNS : Double.
	static double GetPredlogenieVysota(MTopobject* pObj,MPlotDevice* pDevice,int type, MStArray* sta, MPrk* p_prk);
};

#endif //_DRAWOBJ__H_
