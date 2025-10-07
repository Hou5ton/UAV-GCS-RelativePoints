//PACKAGE : MAP.
//FILE: PlotDevice.h   
//AUTHOR:
//DESCRIPTION:

#ifndef PLOTDEVICE_H
#define PLOTDEVICE_H
#include "BaseCl.h"
#include "SysDefine.h"
#include "vector"
#include "ArrBase.h"
#include "Array.h"
#include "map"
#include "MString.h"
#include "Raster.h"
#include "MImage.h"

// File including for new bitmaps supporting

// As inside our device everything have to be moved by paper coordinate system,
// that for transfer of parameters is necessary to move
// structures similar Windows on with double dimensional values

//Coefficient, which exceeds Windows values relative to fonts
// and provides absolutely precise output on the screen
//#define SCREEN_FONT_KOEFF /*0.904*/0.1
#define SCREEN_FONT_KOEFF 0.904 //tempMax

enum RGN_TYPE{ADD_REGION = RGN_AND,INTERSECT_REGION = RGN_OR,CUT_REGION = RGN_NOT};

enum RGN_OP{PLUS,MINUS,XORING,NOT};

/// Structure, which  saves and restores regions of clipping at drawing differnt valriants of clipping
struct M_REG_OPERATION{
	MPoint* ptar;
	int		iNum;
	RGN_OP  operation;
public:
//  -
	M_REG_OPERATION(){ptar = NULL;};
//  -
	M_REG_OPERATION(MPoint* pta,int iPtNum,RGN_OP op){
							iNum=iPtNum;operation=op;
							ptar = (MPoint*)malloc(sizeof(MPoint)*iNum);
							memcpy(ptar,pta,iNum*sizeof(MPoint));};

	virtual ~M_REG_OPERATION(){free(ptar);};

//PACKAGE : MAP.
//FUNCTION : GetBytes();
//DESCRIPTION : Returns full size of array to UTP-format file.
//INPUTS : NONE.
//RETURNS : int.
	int GetBytes(){return iNum*sizeof(MPoint)+4+sizeof(RGN_OP);};

//PACKAGE : MAP.
//FUNCTION : ReadBin(FILE* , int );
//DESCRIPTION : Reads array from NUX-file.
//INPUTS : FILE pointer , int.
//RETURNS : True.
	virtual bool ReadBin(FILE* , int ){return true;};

//PACKAGE : MAP.
//FUNCTION : ReadBin(MFile* , int );
//DESCRIPTION : Reads array from NUX-file.
//INPUTS : MFile pointer , int.
//RETURNS : True.
	virtual bool ReadBin(MFile* , int){return true;};

//PACKAGE : MAP.
//FUNCTION : WriteBin(FILE* , int );
//DESCRIPTION : Saves array to NUX-file.
//INPUTS : FILE pointer , int .
//RETURNS : True.
	virtual bool WriteBin(FILE* , int ){return true;};

//PACKAGE : MAP.
//FUNCTION : Copy(M_REG_OPERATION* pOriginal);
//DESCRIPTION : Makes a clone.
//INPUTS : M_REG_OPERATION pointer.
//RETURNS : NONE.
	virtual void Copy(M_REG_OPERATION* pOriginal){
		if(!ptar) free(ptar);
		iNum = pOriginal->iNum;
		operation = pOriginal->operation;
		ptar = (MPoint*)malloc(sizeof(MPoint)*iNum);
		memcpy(ptar,pOriginal->ptar,sizeof(MPoint)*iNum);
	};
};

//PACKAGE : MAP.
//FUNCTION : MRegionOpArray:public GlobalComplexArray<M_REG_OPERATION>;
//DESCRIPTION : Array of operations of clippping regions.
class MRegionOpArray:public GlobalComplexArray<M_REG_OPERATION>
{

//PACKAGE : MAP.
//FUNCTION : MRegionOpArray::CreateOfType();
//DESCRIPTION : Functions must be inherited and redefined by inheritors
//  			and it is also have to create an object with type of class in which it is inheriting.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MRegionOpArray();};
};


//PACKAGE : MAP.
//FUNCTION : MPlotDevice.
//DESCRIPTION : This is absolutely abstract class for drawing independent of object
//				Works in logical coordinate system, which is specific for each device of drawing */
class MPlotDevice
{
protected:

	// At this place situates code, which saves condition of clipping
	std::vector<MRegionOpArray*>	region_op_stack;
	MRegionOpArray* pCurrentClipLog;

	//
	void LogClip(MPoint* pta,int iNum,RGN_OP operation){
		M_REG_OPERATION temp(pta,iNum,operation);
		pCurrentClipLog->AddTail(&temp);
    }

	// Applies region, which is saved in array of actions
	void ApplyClip(MRegionOpArray* pOld){
		for(int i = pOld->GetSize()-1;i>=0;i--){
			M_REG_OPERATION* pOp = pOld->GetAt(i);
			switch(pOp->operation){
			case MINUS:
				//DeleteClipRgn(pOp->ptar,pOp->iNum);break;
			case PLUS:
				//  Adds clippi
				AddClipRgn(pOp->ptar,pOp->iNum,ADD_REGION);break;

			case XORING:
				// Adds clipping region
				AddClipRgn(pOp->ptar,pOp->iNum,INTERSECT_REGION);break;

                    default:		AddClipRgn(pOp->ptar,pOp->iNum,ADD_REGION);break;/*exit(15)*/;
			}
		}
    }

public:

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SaveClipping;
//DESCRIPTION : Saves clipping to stack of operations.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SaveClipping(){MRegionOpArray* ArToSave = (MRegionOpArray*)pCurrentClipLog->Clone();
                    region_op_stack.push_back(ArToSave);}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::RestoreClipping();
//DESCRIPTION : Restore clipping from stack of operations.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestoreClipping(){if(!region_op_stack.size())return;
					// Pointer to the top of stack
					MRegionOpArray* pOld = region_op_stack.back();
					region_op_stack.pop_back();
					ClearClip();
					ApplyClip(pOld);
					delete pOld;
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndClipping();
//DESCRIPTION : Restore clipping from stack of operations.
//INPUTS : NONE.
//RETURNS : NONE.
    virtual void EndClipping( ){}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetClipRegionFlag(bool flag);
//DESCRIPTION : Setting clip region flag.
//INPUTS : bool.
//RETURNS : NONE.
    virtual void SetClipRegionFlag(bool flag){m_bClipRegionSelected = flag;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetClipRegionFlag();
//DESCRIPTION : Getting clip region flag.
//INPUTS : NONE.
//RETURNS : True.
    virtual bool GetClipRegionFlag(){ return m_bClipRegionSelected ;}
	


protected:
	MArArray m_clips;	//  Array of inner symply connected clippers
	bool	 m_bClipRegionSelected; //  Flag of clipping applying
protected:
	MRect m_BumBounds;	//  Rectangle of output in paper coordinate system

	MRect m_LogBounds;	//  Rectangle of drawing in logical coordinates

	bool m_bAdvocate;	//  Flag which shows if outputing directly or by dealer

	double A,B1,B2,C,D1,D2;	//  Coefficients for conversion of paper coordinate system to logical

	// Calculates coefficients of conversion
	virtual void RecalcKoeffs();

	int m_iDrawStepCount;	//  Number of steps of drawing

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::MPlotDevice();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	// 	MPlotDevice():m_bAdvocate(false),m_bClipRegionSelected(false){pCurrentClipLog = new MRegionOpArray();};
    MPlotDevice(){m_bAdvocate=false; m_bClipRegionSelected=false; pCurrentClipLog = new MRegionOpArray();}  //   2004.07.06

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::~MPlotDevice();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
    virtual ~MPlotDevice(){delete pCurrentClipLog;}

public:

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetCooSys(MRect rcLogic,MRect rcBum);
//DESCRIPTION : Sets coordinate system.
//INPUTS : MRect ,MRect .
//RETURNS : NONE.
	virtual void SetCooSys(MRect rcLogic,MRect rcBum){
		m_LogBounds = rcLogic;
		m_BumBounds = rcBum;
		RecalcKoeffs();
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetLogRect();
//DESCRIPTION : Gets visible are in logical coordinates.
//INPUTS : NONE.
//RETURNS : MRect.
    virtual MRect GetLogRect(){return m_LogBounds;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetClientRect(MRect* pRect);
//DESCRIPTION : Gets visible are in real coordinates.
//INPUTS : MRect pointer.
//RETURNS : NONE.
    virtual void GetClientRect(MRect* pRect){*pRect = m_BumBounds;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetSizeMM();
//DESCRIPTION : Gets device size in mm.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual MIntSize GetSizeMM()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetCurrentScale();
//DESCRIPTION : Gets the current geo scale.
//INPUTS : NONE.
//RETURNS : double.
	virtual double GetCurrentScale()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::BumToLogic(MPoint ptin,MPoint& ptout);
//DESCRIPTION : Converts from paper coordinate system to logical and back.
//INPUTS : MPoint ,MPoint& .
//RETURNS : NONE.
	virtual void BumToLogic(MPoint ptin,MPoint& ptout){
		ptout.x = A*ptin.x + B1;
		ptout.y = A*ptin.y + B2;
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::BumToLogic(MPoint* ptina,MPoint* ptouta,int iNum);
//DESCRIPTION : Converts from paper coordinate system to logical and back.
//INPUTS : MPoint pointer,MPoint pointer,int .
//RETURNS : NONE.
	virtual void BumToLogic(MPoint* ptina,MPoint* ptouta,int iNum){
		for(int i = 0;i<iNum;i++){
			ptouta[i].x = A*ptina[i].x +B1;
			ptouta[i].y = A*ptina[i].y +B2;
		}
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::LogicToBum(MPoint ptin,MPoint& ptout);
//DESCRIPTION : Converts from logical coordinate system to paper and back.
//INPUTS : MPoint ,MPoint& .
//RETURNS : NONE.
	virtual void LogicToBum(MPoint ptin,MPoint& ptout){
		ptout.x = C*ptin.x + D1;
		ptout.y = C*ptin.y + D2;
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::LogicToBum(MPoint* ptin,MPoint* ptout,int iNum);
//DESCRIPTION :  Converts from logical coordinate system to paper and back.
//INPUTS : MPoint pointer,MPoint pointer,int .
//RETURNS : NONE.
	virtual void LogicToBum(MPoint* ptin,MPoint* ptout,int iNum){
		for(int i = 0;i<iNum;i++){
			ptout[i].x = C*ptin[i].x + D1;
			ptout[i].y = C*ptin[i].y + D2;
		}
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::BumToLogic(MRect* pRectIn,MRect& pRectOut);
//DESCRIPTION : Converts paper rectangle to logical.
//INPUTS : MRect pointer,MRect& .
//RETURNS : NONE.
	virtual void BumToLogic(MRect* pRectIn,MRect& pRectOut);

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::LogicToBum(MRect* pRectIn,MRect& pRectOut);
//DESCRIPTION : Converts logical rectangle to paper.
//INPUTS : MRect pointer,MRect& .
//RETURNS : NONE.
	virtual void LogicToBum(MRect* pRectIn,MRect& pRectOut);

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::BumToLogic(double dVal);
//DESCRIPTION : Converts from paper coordinate system to logical and back.
//INPUTS : double.
//RETURNS : double.
    virtual double BumToLogic(double dVal){return dVal*A;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::LogicToBum(double dVal);
//DESCRIPTION : Specific transformation for availability of drawing
// 				of size in device units.
//INPUTS : double.
//RETURNS : double.
    virtual double LogicToBum(double dVal){return dVal/A;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetVirtualMode(bool bMode = true);
//DESCRIPTION : Changes output mode.
//INPUTS : bool.
//RETURNS : NONE.
    virtual void SetVirtualMode(bool bMode = true){m_bAdvocate = bMode;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Polyline(MPoint* pPoints, int iNumPoints);
//DESCRIPTION : Draws polyline.
//INPUTS : MPoint pointer, int .
//RETURNS : True.
	virtual bool Polyline(MPoint* pPoints, int iNumPoints)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Polygon(MPoint* pPoints, int iNumPoints);
//DESCRIPTION : Sraws polygon.
//INPUTS : MPoint pointer, int iNumPoints.
//RETURNS : True.
	virtual bool Polygon(MPoint* pPoints, int iNumPoints)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawImage(MImage *image, MColor color, MPoint leftTop);
//DESCRIPTION : Drawing pictures.
//INPUTS : MImage  pointer, MColor , MPoint .
//RETURNS : True.
	virtual bool DrawImage(MImage *image, MColor color, MPoint leftTop)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetZapolnenie(MImage *image, MColor color);
//DESCRIPTION : Set the brush filling.
//INPUTS : MImage  pointer, MColor .
//RETURNS : True.
	virtual bool SetZapolnenie(MImage *image, MColor color)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetZapolnenie(MColor* pColor)=0;
//DESCRIPTION : Sets brush of filling.
//INPUTS : MColor pointer.
//RETURNS : True.
    virtual bool SetZapolnenie(MColor* const pColor) = 0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetPencil(M_LOG_PEN* pLPen);
//DESCRIPTION : Sets pencil of drawing.
//INPUTS : M_LOG_PEN pointer.
//RETURNS : True.
	virtual bool SetPencil(M_LOG_PEN* pLPen)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetPencil(int iPenStyle,double dWidth,MColor* pColor);
//DESCRIPTION : Sets pencil of drawing with specific fillet.
//INPUTS : int ,double ,MColor pointer.
//RETURNS : True.
	virtual bool SetPencil(int iPenStyle,double dWidth,MColor* pColor)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetFont(M_LOG_FONT* lFont);
//DESCRIPTION : Sets font.
//INPUTS : M_LOG_FONT pointer.
//RETURNS : True.
	virtual bool SetFont(M_LOG_FONT* lFont)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetTextAlign(unsigned int uiTextAlign);
//DESCRIPTION : Sets modus of text alignment.
//INPUTS : unsigned int.
//RETURNS :  unsigned int.
	virtual unsigned int SetTextAlign(unsigned int uiTextAlign)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetTextColor(MColor crTextColor);
//DESCRIPTION : Sets text color.
//INPUTS : MColor.
//RETURNS : MColor.
	virtual MColor SetTextColor(MColor crTextColor)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetBkColor(MColor crBkColor);
//DESCRIPTION : Sets background color.
//INPUTS : MColor.
//RETURNS : MColor.
	virtual MColor SetBkColor(MColor crBkColor)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetBkMode(int nBkMode);
//DESCRIPTION : Sets background mode.
//INPUTS : int.
//RETURNS : int.
	virtual int SetBkMode(int nBkMode)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::TextOut(double dX,double dY,char * lpszString,int nCount);
//DESCRIPTION : Outputs text to device.
//INPUTS : double ,double ,char  pointer,int .
//RETURNS : True.
	virtual bool TextOut(double dX,double dY,char * lpszString,int nCount/*, bool bAligned*/)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::TextOut(double dX,double dY,MString& lpszString);
//DESCRIPTION : Outputs text to device.
//INPUTS : double ,double ,MString& .
//RETURNS : True.
	virtual bool TextOut(double dX,double dY,MString& lpszString/*, bool bAligned*/)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::TextOutPramSetka(MPtArray* pta, MRect* rect, MString* str, double step, double dSymVal);
//DESCRIPTION : Draws symbols grid.
//INPUTS : MPtArray pointer, MRect pointer, MString pointer, double , double .
//RETURNS : True.
	virtual bool TextOutPramSetka(MPtArray* pta, MRect* rect, MString* str, double step, double dSymVal)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::TextOutKosSetka(MPtArray* pta, MRect* rect, MString* str, double angle, double step, double dSymVal);
//DESCRIPTION : Getting clip region flag.
//INPUTS : True.
//RETURNS : NONE.
	virtual bool TextOutKosSetka(MPtArray* pta, MRect* rect, MString* str, double angle, double step, double dSymVal)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetClipRegionFlag();
//DESCRIPTION : Draws symbols grid.
//INPUTS : MPtArray pointer, MRect pointer, MSymbol pointer, double , double .
//RETURNS : True.
	virtual bool TextOutXaosSetka(MPtArray* pta, MRect* rect, MSymbol* sym, double step, double dSymVal)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::TextOutWindows(double dX,double dY,char * lpszString,int nCount);
//DESCRIPTION : Outputs text in windows coding to device.
//INPUTS : double ,double ,char  ,int .
//RETURNS : True.
	virtual bool TextOutWindows(double dX,double dY,char * lpszString,int nCount/*, bool bAligned*/)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::TextOutWindows(double dX,double dY,MString& lpszString);
//DESCRIPTION :  Outputs text in windows codin to device.
//INPUTS : double ,double ,MString& .
//RETURNS : True.
	virtual bool TextOutWindows(double dX,double dY,MString& lpszString/*, bool bAligned*/)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Rectangle(double dX1,double dY1,double dX2,double dY2);
//DESCRIPTION : Outputs rectangle.
//INPUTS : double ,double ,double ,double .
//RETURNS : True.
	virtual bool Rectangle(double dX1,double dY1,double dX2,double dY2)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Rectangle(MRect* lpRect);
//DESCRIPTION : Outputs rectangle.
//INPUTS : MRect pointer.
//RETURNS : True.
	virtual bool Rectangle(MRect* lpRect)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::RoundRect(MRect* lpRect,MPoint EllipsPoint);
//DESCRIPTION : Filleted rectangle.
//INPUTS : MRect pointer,MPoint .
//RETURNS : True.
	virtual bool RoundRect(MRect* lpRect,MPoint EllipsPoint)=0;

	//positioning	//  replaced by::DrawLine
	//virtual bool MoveTo( double x, double y )=0;

	//positioning	//  replaced by ::DrawLine
	//virtual bool MoveTo( MPoint point )=0;

	//Holding the line	//  replaced by ::DrawLine
	//virtual bool LineTo( double x, double y )=0;

	//Holding the line	//  replaced by ::DrawLine
	//virtual bool LineTo( MPoint point )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawLine( MPoint point1, MPoint point2  );
//DESCRIPTION : Holding the line on two points.
//INPUTS : MPoint , MPoint .
//RETURNS : True.
	virtual bool DrawLine( MPoint point1, MPoint point2  )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawLine(double x1, double y1, double x2, double y2);
//DESCRIPTION : Holding the line on two points.
//INPUTS : double , double , double , double .
//RETURNS : True.
	virtual bool DrawLine(double x1, double y1, double x2, double y2)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Arc( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
//DESCRIPTION : Draws arc.
//INPUTS : double , double , double , double , double , double , double , double .
//RETURNS : True.
	virtual bool Arc( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Arc( MRect* lpRect, MPoint ptStart, MPoint ptEnd );
//DESCRIPTION : Draws arc.
//INPUTS : MRect pointer, MPoint , MPoint .
//RETURNS : True.
	virtual bool Arc( MRect* lpRect, MPoint ptStart, MPoint ptEnd )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::FillRect( MRect* lpRect, MColor* pColor );
//DESCRIPTION : Filletsrectangle.
//INPUTS :  MRect pointer, MColor pointer.
//RETURNS : NONE.
	virtual void FillRect( MRect* lpRect, MColor* pColor )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::FillSolidRect( MRect* lpRect, MColor clr );
//DESCRIPTION : Fills rectangle.
//INPUTS : MRect pointer, MColor  .
//RETURNS : NONE.
	virtual void FillSolidRect( MRect* lpRect, MColor clr )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::FillSolidRect( double x, double y, double cx, double cy, MColor clr );
//DESCRIPTION : Fills rectangle.
//INPUTS : double , double , double , double , MColor  .
//RETURNS : NONE.
	virtual void FillSolidRect( double x, double y, double cx, double cy, MColor clr )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Ellipse( double x1, double y1, double x2, double y2 );
//DESCRIPTION : Draws ellipse.
//INPUTS : double , double , double , double .
//RETURNS : True.
	virtual bool Ellipse( double x1, double y1, double x2, double y2 )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Ellipse( MRect* lpRect );
//DESCRIPTION : Draws ellipse.
//INPUTS : MRect pointer.
//RETURNS : True.
	virtual bool Ellipse( MRect* lpRect )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Circle( MPoint ptCenter,double dRad,MColor color);
//DESCRIPTION : Draws ellipse.
//INPUTS :  MPoint ,double ,MColor .
//RETURNS : True.
	virtual bool Circle( MPoint ptCenter,double dRad,MColor color)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Pie( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
//DESCRIPTION : Draws segment.
//INPUTS : double , double , double , double , double , double , double , double .
//RETURNS : True.
	virtual bool Pie( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Pie( MRect* lpRect, MPoint ptStart, MPoint ptEnd );
//DESCRIPTION : Draws segment.
//INPUTS : MRect pointer, MPoint , MPoint  .
//RETURNS : True.
	virtual bool Pie( MRect* lpRect, MPoint ptStart, MPoint ptEnd )=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Pie( MPoint ptCenter, double dRad, double startAngle,double spanAngle);
//DESCRIPTION : Draws segment.
//INPUTS :  MPoint , double , double ,double .
//RETURNS : True.
	virtual bool Pie( MPoint ptCenter, double dRad, double startAngle,double spanAngle)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Raster(MRaster* raster);
//DESCRIPTION : Draws raster.
//INPUTS : MRaster pointer .
//RETURNS : True.
	virtual bool Raster(MRaster* raster)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::OpacityBackground(int lightness, int transparent);
//DESCRIPTION : Background rendering operational layers.
//INPUTS : int , int  .
//RETURNS : True.	
	virtual bool OpacityBackground(int lightness, int transparent)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SelectStockObject(int nIndex);
//DESCRIPTION : Selects stock object.
//INPUTS : int.
//RETURNS : NONE.
	virtual void SelectStockObject(int nIndex)=0;

// Saves different values to the stack

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SavePencil();
//DESCRIPTION : Saves pencil.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SavePencil()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::RestorePencil();
//DESCRIPTION : Restore pencil.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestorePencil()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SaveZapolnenie();
//DESCRIPTION : Saves filling.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SaveZapolnenie()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::RestoreZapolnenie();
//DESCRIPTION : Restore filling.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestoreZapolnenie()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SaveFont();
//DESCRIPTION :  Saves font.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SaveFont()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::RestoreFont();
//DESCRIPTION : Restore font.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestoreFont()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SaveAll();
//DESCRIPTION : Saves all values.
//INPUTS : NONE.
//RETURNS : NONE.
    virtual void SaveAll(){SavePencil();SaveZapolnenie();SaveFont();}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Restoreall();
//DESCRIPTION : Restore all values.
//INPUTS : NONE.
//RETURNS : NONE.
    virtual void Restoreall(){RestorePencil();RestoreZapolnenie();RestoreFont();}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Actualize(bool bToOriginal = true);
//DESCRIPTION : Function of actualization of image.
//INPUTS : bool.
//RETURNS : NONE.
	virtual void Actualize(bool bToOriginal = true)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Initialize();
//DESCRIPTION : Functions of step-by-step initialization.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Initialize()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StartSheet();
//DESCRIPTION : Beginning of new sheet.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartSheet()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StartLayer();
//DESCRIPTION : Beginning of new layer.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartLayer()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StartObject();
//DESCRIPTION :  Beginning of new object.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartObject()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StartSheetText();
//DESCRIPTION : Beginning of page text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartSheetText()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StartLayerText();
//DESCRIPTION : Beginning of layer text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartLayerText()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StartObjectText();
//DESCRIPTION : Beginning of object text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartObjectText()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::UnInitialize();
//DESCRIPTION :  The end.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void UnInitialize()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndSheet();
//DESCRIPTION : End of page.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndSheet()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndLayer();
//DESCRIPTION : End of layer.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndLayer()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndObject();
//DESCRIPTION : End of object.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndObject()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndSheetText();
//DESCRIPTION : End of page text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndSheetText()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndLayerText();
//DESCRIPTION : End of layer text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndLayerText()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::EndObjectText();
//DESCRIPTION : End of object text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndObjectText()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::BeginClipRgn();
//DESCRIPTION : Functions of operation with paths
// 				and path choosing for clipping.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool BeginClipRgn();

//PACKAGE : MAP.
//FUNCTION : MPlotDevice:: AddClipRgn(MPoint* pta,int iNum,RGN_TYPE type = ADD_REGION);
//DESCRIPTION : 
//INPUTS : MPoint pointer,int ,RGN_TYPE .
//RETURNS : True.
	virtual bool AddClipRgn(MPoint* pta,int iNum,RGN_TYPE type = ADD_REGION);

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::AddClipRgn(MRect* pRect,RGN_TYPE type = ADD_REGION);
//DESCRIPTION : 
//INPUTS : MRect pointer,RGN_TYPE.
//RETURNS : True.
	virtual bool AddClipRgn(MRect* pRect,RGN_TYPE type = ADD_REGION);


	//	virtual bool DeleteClipRgn(MPoint* pta,int iPtNum);
	//	virtual bool DeleteClipRgn(MRect* pRect);
	//	virtual bool SelectClipRgn();

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::ClearClip();
//DESCRIPTION :  Clears clip.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool ClearClip();

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::ClearClip();
//DESCRIPTION : Create clip.
//INPUTS : NONE.
//RETURNS : True.
    	virtual void CreateClipRgn(MPoint* ptaIn,int iNumPoints)=0;

//For faster work with fonts the concept of fonts pool is neccessary

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::AddFontToPool(M_LOG_FONT* lfont,int iIndex);
//DESCRIPTION : Creates font and moves it to the fonts pool.
//INPUTS : M_LOG_FONT pointer,int .
//RETURNS : True.
	virtual bool AddFontToPool(M_LOG_FONT* lfont,int iIndex)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SelectFontFromPool(int iIndex);
//DESCRIPTION : Selects font from the pool.
//INPUTS : int.
//RETURNS : True.
	virtual bool SelectFontFromPool(int iIndex)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::ClearPool();
//DESCRIPTION : Clears pool.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool ClearPool()=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetTextExtent(MString& str);
//DESCRIPTION : Gets text lenght.
//INPUTS : MString&.
//RETURNS : double.
	virtual double GetTextExtent(MString& str)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawServantPolyLine(MPoint* par,int iNumPt,int iLogicWidth,MColor crColor = MColor(0,0,0));
//DESCRIPTION :  Draws service polyline.
//INPUTS : MPoint pointer,int ,int ,MColor .
//RETURNS : NONE.
	virtual void DrawServantPolyLine(MPoint* par,int iNumPt,int iLogicWidth,MColor crColor = MColor(0,0,0))=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawServantPolyLine(MPtArray* par,int iLogicWidth,MColor crColor = MColor(0,0,0));
//DESCRIPTION :  Draws service polyline.
//INPUTS : MPtArray pointer,int ,MColor .
//RETURNS : NONE.
	virtual void DrawServantPolyLine(MPtArray* par,int iLogicWidth,MColor crColor = MColor(0,0,0)){
  	DrawServantPolyLine(par->GetHead(),par->GetSize(),iLogicWidth,crColor);
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawServantCircle(MPoint ,int , MColor );
//DESCRIPTION : Functions for service line drawing(in logical coordinates of device).
//INPUTS : MPoint ,int , MColor.
//RETURNS : NONE.
	virtual void DrawServantCircle(MPoint ,int , MColor ){
		InternalMessage("PlotDevice::DrawServantCircle not implemented");
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawServantLine(MPoint ,MPoint ,MColor ,int );
//DESCRIPTION : Functions for service line drawing(in logical coordinates of device).
//INPUTS : MPoint ,MPoint ,MColor ,int .
//RETURNS : NONE.
	virtual void DrawServantLine(MPoint ,MPoint ,MColor ,int ){
		InternalMessage("PlotDevice::DrawServantLine not implemented");
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawErasibleLine(MPoint ,MPoint );
//DESCRIPTION : Functions for service line drawing(in logical coordinates of device).
//INPUTS : MPoint ,MPoint.
//RETURNS : NONE.
	virtual void DrawErasibleLine(MPoint ,MPoint ){
		InternalMessage("PlotDevice::DrawErasibleLine not implemented");
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawErasiblePolyLine(MPtArray* );
//DESCRIPTION : Functions for service line drawing(in logical coordinates of device).
//INPUTS : MPtArray pointer.
//RETURNS : NONE.
	virtual void DrawErasiblePolyLine(MPtArray* ){
		InternalMessage("PlotDevice::DrawErasiblePolyLine not implemented");
    }

// For optimization of drawing should be availability ask if
// device is distinguished concrete value in it's own logic coordinates

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Distinguish(double dVal);
//DESCRIPTION : 
//INPUTS : double.
//RETURNS : True.
	virtual bool Distinguish(double dVal)=0;

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::Distinguish_Text(double dVal);
//DESCRIPTION :
//INPUTS : double.
//RETURNS : True.
	virtual bool Distinguish_Text(double dVal)=0;


	//	virtual CDC* GetOutputDC(){return NULL;};

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::SetROP2( int  );
//DESCRIPTION : For ROP mode using.
//INPUTS : int.
//RETURNS : int.
    virtual int SetROP2( int  ){InternalMessage("PlotDevice:: ROP-mode not implemented");return 0;}

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::PatBlt(double ,double ,double ,double ,unsigned int );
//DESCRIPTION : For PatBlt.
//INPUTS : double ,double ,double ,double ,unsigned int.
//RETURNS : True.
	virtual bool PatBlt(double ,double ,double ,double ,unsigned int ){
		InternalMessage("PlotDevice::PatBlt not implemented");return false;
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::DrawFocusRect(MRect );
//DESCRIPTION : For drawing of rectangle of focusing.
//INPUTS : MRect.
//RETURNS : True.
	virtual bool DrawFocusRect(MRect ){
		InternalMessage("PlotDevice::DrawFoucusRect not implemented");return false;
    }

// The new set of functions which allows a conclusion of images
// and receptions of images bit-by-bit from MPlotDevice
// This function creates bitmap and fills it by data from
// the concrete device, the sizes turn out from transferred
// All parameters of bitmap returns in integer values.
// The block of the data is created in the given function and should be deleted with help of
// free (pBitmapToLoad-> bmBits);

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::CreateBitmapFromDevice(UT_BITMAP* ,double ,double ,double ,double );
//DESCRIPTION : Gets image
//INPUTS : UT_BITMAP pointer ,double ,double ,double ,double.
//RETURNS : True.
	virtual bool CreateBitmapFromDevice(UT_BITMAP* ,double ,double ,
		double ,double )
	{
		return false;
    }


//PACKAGE : MAP.
//FUNCTION : MPlotDevice::StretchBitmap(UT_BITMAPINFO* ,LPVOID ,double ,double ,double ,double ,unsigned int );
//DESCRIPTION : Outputs image to the screen, even with sketching if neccessary
//				parameters (UT_BITMAPINFO* pBMPI,LPVOID lpData,double dStartX,double dStartY, double dWidth,double, dHeight,unsigned int dwRop).
//INPUTS : UT_BITMAPINFO pointer ,LPVOID ,double ,double ,double ,double ,unsigned int .
//RETURNS : True.
	virtual bool StretchBitmap(UT_BITMAPINFO* ,LPVOID ,double ,double ,double ,double ,unsigned int )
	{
		return false;
    }

//PACKAGE : MAP.
//FUNCTION : MPlotDevice::GetDeviceCaps(int ,bool );
//DESCRIPTION : Function of access to devices of representation.
//INPUTS : int ,bool.
//RETURNS : double.
	virtual double GetDeviceCaps(int ,bool )
	{
		InternalMessage("PlotDevice::GetDeviceCaps для данного устройства не поддерживается");
		return 0;
	}

};

#endif //PLOTDEVICE_H
