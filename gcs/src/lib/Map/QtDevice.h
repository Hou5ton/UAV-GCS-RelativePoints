//PACKAGE : MAP.
//FILE: QtDevice.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _H_QT_DEVICE
#define _H_QT_DEVICE
#include <qpaintdevice.h> 
#include <qpen.h>
#include <qbrush.h>
#include <qfont.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qpoint.h>
#include <qregion.h>

#include <qstring.h>
#include <qwidget.h>
#include <qrect.h>
#include <qtextcodec.h>
#include <iosfwd>
#include <QPolygon>
#include "PlotDevice.h"


void WinToUnicod(QByteArray & str, QString* ret_str);

//PACKAGE : MAP.
//CLASS : MQtDevice.
//DESCRIPTION : Class to draw the unit QT - QPainter. It used to draw on Linux.
class MQtDevice : public MPlotDevice  
{
public:

//PACKAGE : MAP.
//FUNCTION : MQtDevice::MQtDevice(QPainter* pDev);
//DESCRIPTION : Constructor.
//INPUTS : QPainter pointer.
//RETURNS : NONE.
	MQtDevice(QPainter* pDev);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::~MQtDevice();
//DESCRIPTION : Destructor.
//INPUTS : QPainter pointer.
//RETURNS : NONE.
	virtual ~MQtDevice();

protected:
	//Stacks to save values
	//Stacks are made using STL
	//Stack feathers
	std::vector<QPen*>	pen_stack;
	//Stack brushes
	std::vector<QBrush*> brush_stack;
	//Stack fonts
	std::vector<QFont*>  font_stack;

	//Pools of fonts
	std::map<int,QFont*>	 font_pool;
	std::map<int,M_LOG_FONT> font_par_pool; 

	//Flag Specifies the target which has been selected from a pool of the font or set up
	bool m_bSelectedFromPool;

	typedef std::map<int,QFont*> FONT_POOL_TYPE;
	typedef std::map<int,M_LOG_FONT> FONT_PAR_POOL_TYPE;


	//mediator for outputting
	QPixmap		m_pixmap;//m_dc

	//Bitmap intermediary device
//	CBitmap			m_Bitmap;
    QPaintDevice*	m_pRealPD{nullptr};		// Device is made ​​on the basis of output ( original)
    QPaintDevice*	m_pOutputPD{nullptr};	// Device that is currently produced output

    int width{0}, height{0};		// size of painter-а

    bool m_bPathStarted{false};	// It is impossible to produce a switch without finishing path

	//Variables for storing flags GDI objects and their installation or removal
    QPen* pPen{nullptr};
    bool bPen{false};
    QBrush* pBrush{nullptr};
    bool bBrush{false};
    QFont* pFont{nullptr};
    bool bFont{false};
    QPen* pTempPen{nullptr};
    QFont* pTempFont{nullptr};
    QBrush* pTempBrush{nullptr};
    QTextCodec *codec{nullptr};

	MColor font_color;

    QPainter* m_painter{nullptr};

    int m_show_text_limit{0};

    int m_angle{0};

    int m_align{0};

	// aligns text
	QPoint AlignText(QPoint& priv_pr, QString& str);
    QPolygon fillingPolygon;
    //QPainterPath painterPath;

private:
    int geo_scale;

public:

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Initialize();
//DESCRIPTION : Initialization.
//INPUTS : NONE.
//RETURNS : NONE.
	void Initialize();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::ClearAllAndInit(QPainter* pDev = NULL);
//DESCRIPTION : Clear all parameters and initialized.
//INPUTS : QPainter pointer.
//RETURNS : True.
	bool ClearAllAndInit(QPainter* pDev = NULL);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Actualize(bool bToOriginal = true);
//DESCRIPTION : Updating feature image ( bringing it to a specific device
//				if the conclusion was made through an intermediary.
//INPUTS : bool.
//RETURNS : NONE.
	void Actualize(bool bToOriginal = true);

//Functions ways
// and sample path for clipping

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BeginClipRgn();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : True.
	bool BeginClipRgn();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::AddClipRgn(MPoint* ptaIn,int iNumPoints,RGN_TYPE type = ADD_REGION);
//DESCRIPTION : 
//INPUTS : MPoint pointer,int ,RGN_TYPE .
//RETURNS : True.
	bool AddClipRgn(MPoint* ptaIn,int iNumPoints,RGN_TYPE type = ADD_REGION);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::AddClipRgn(MRect* pRect,RGN_TYPE type = ADD_REGION);
//DESCRIPTION : Initialization.
//INPUTS : MRect pointer, RGN_TYPE .
//RETURNS : True.
	bool AddClipRgn(MRect* pRect,RGN_TYPE type = ADD_REGION);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::ClearClip();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : True.
	bool ClearClip();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::CreateClipRgn(MPoint* ptaIn,int iNumPoints);
//DESCRIPTION : 
//INPUTS : MPoint pointer,int .
//RETURNS : NONE.
	void CreateClipRgn(MPoint* ptaIn,int iNumPoints);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SaveClipping();
//DESCRIPTION : Keep the area with rendering.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SaveClipping()
	{
		if(!m_painter) return;
		m_painter->save();
		MPlotDevice::SaveClipping();
	}

//PACKAGE : MAP.
//FUNCTION : MQtDevice::RestoreClipping();
//DESCRIPTION : Restoring the area with rendering.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestoreClipping()
	{
		if(!m_painter) return;
		m_painter->restore();
		delete pCurrentClipLog;
		pCurrentClipLog = region_op_stack.back();
		region_op_stack.pop_back();
	};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndClipping();
//DESCRIPTION : End clipping.
//INPUTS : NONE.
//RETURNS : NONE.
	void  EndClipping()
	{
        fillingPolygon = QPolygon();
	};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogic(MPoint* ptina,MPoint* ptouta,int iNum);
//DESCRIPTION : Transfer function of the paper coordinate system into a logical.
//INPUTS : MPoint pointer , MPoint pointer,int .
//RETURNS : NONE.
	virtual void BumToLogic(MPoint* ptina,MPoint* ptouta,int iNum){
		for(int i = 0;i<iNum;i++){
			ptouta[i].x = width/2+(A*ptina[i].x +B1);
			ptouta[i].y = height/2-(A*ptina[i].y +B2);
		}
	};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogic(MPoint *ptin,MPoint& ptout);
//DESCRIPTION : Transfer function of the paper coordinate system into a logical.
//INPUTS : MPoint pointer,MPoint& .
//RETURNS : NONE.
	virtual void BumToLogic(MPoint *ptin,MPoint& ptout){
		ptout.x = width/2+(A*ptin->x + B1);
		ptout.y = height/2-(A*ptin->y + B2);
	};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogic(MRect* pRectIn,MRect& pRectOut);
//DESCRIPTION : Transfer function of the paper coordinate system into a logical.
//INPUTS : MRect pointer,MRect&.
//RETURNS : NONE.
	virtual void BumToLogic(MRect* pRectIn,MRect& pRectOut)
	{
		pRectOut.left = width/2+(A*pRectIn->left + B1);
		pRectOut.top  = height/2-(A*pRectIn->top + B2);
		pRectOut.right = width/2+(A*pRectIn->right + B1);
		pRectOut.bottom  = height/2-(A*pRectIn->bottom + B2);
	}

//PACKAGE : MAP.
//FUNCTION : MQtDevice::LogicToBum(MPoint* ptin,MPoint* ptout,int iNum);
//DESCRIPTION : The transfer function from the logical coordinates into paper.
//INPUTS : MPoint pointer,MPoint pointer,int.
//RETURNS : NONE.
	virtual void LogicToBum(MPoint* ptin,MPoint* ptout,int iNum){
		for(int i = 0;i<iNum;i++){
			ptout[i].x = C*(ptin[i].x-width/2) + D1;
			ptout[i].y = C*(height/2-ptin[i].y) + D2;
		}
	};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::LogicToBum(MPoint ptin,MPoint& ptout);
//DESCRIPTION : The transfer function from the logical coordinates into paper.
//INPUTS : MPoint ,MPoint&.
//RETURNS : NONE.
	virtual void LogicToBum(MPoint ptin,MPoint& ptout){
		ptout.x = C*(ptin.x-width/2) + D1;
		ptout.y = C*(height/2-ptin.y) + D2;
	};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::LogicToBum(MRect* pRectIn,MRect& pRectOut);
//DESCRIPTION : The transfer function from the logical coordinates into paper.
//INPUTS : MRect pointer,MRect&.
//RETURNS : NONE.
	virtual void LogicToBum(MRect* pRectIn,MRect& pRectOut)
	{
	pRectOut.left = C*(pRectIn->left-width/2) + D1;
		pRectOut.top  = C*(height/2-pRectIn->top) + D2;
		pRectOut.right = C*(pRectIn->right-width/2) + D1;
		pRectOut.bottom  = C*(height/2-pRectIn->bottom) + D2;
	}

//PACKAGE : MAP.
//FUNCTION : MQtDevice::LogicToBum(double dVal);
//DESCRIPTION : The transfer function from the logical coordinates into paper.
//INPUTS : double.
//RETURNS : double.
	double LogicToBum(double dVal){return double(dVal)/A;}

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogicI(MPoint *ptin,QPoint& ptout);
//DESCRIPTION : For the purity of the code in this class there is a conversion function
//				the coordinates of the paper into logical integer.
//INPUTS : MPoint pointer,QPoint&.
//RETURNS : NONE.
	void BumToLogicI(MPoint *ptin,QPoint& ptout);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogicI(MPoint* ptina,QPolygon &ptouta,int iNum);
//DESCRIPTION : Conversion function the coordinates of the paper into logical integer.
//INPUTS : MPoint pointer,QPolygon &,int .
//RETURNS : NONE.
	void BumToLogicI(MPoint* ptina,QPolygon &ptouta,int iNum);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogicI(MRect* pRectIn,QRect& pRectOut);
//DESCRIPTION : Conversion function the coordinates of the paper into logical integer.
//INPUTS : MRect pointer,QRect& .
//RETURNS : NONE.
	void BumToLogicI(MRect* pRectIn,QRect& pRectOut);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogicI(double dVal);
//DESCRIPTION : Conversion function the coordinates of the paper into logical integer.
//INPUTS : double.
//RETURNS : int.
	int  BumToLogicI(double dVal){return (int)dbl_to_int(A*dVal);};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::BumToLogic(double dVal);
//DESCRIPTION : Transfer function of the paper coordinate system into a logical.
//INPUTS : double.
//RETURNS : double.
	double BumToLogic(double dVal){return dVal*A;}

//PACKAGE : MAP.
//FUNCTION : MQtDevice::AddFontToPool(M_LOG_FONT* lfont,int iIndex);
//DESCRIPTION : For fast work with fonts must be the concept of a pool
//				Font ( He created it added value of
//				and it cleans.
//INPUTS : M_LOG_FONT pointer,int .
//RETURNS : True.
	bool AddFontToPool(M_LOG_FONT* lfont,int iIndex);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SelectFontFromPool(int iIndex);
//DESCRIPTION : Selects font from pool.
//INPUTS : int.
//RETURNS : True.
	bool SelectFontFromPool(int iIndex);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::ClearPool();
//DESCRIPTION : Clears pool.
//INPUTS : NONE.
//RETURNS : True.
	bool ClearPool();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::ChangePen(QPen* pen,bool bDel);
//DESCRIPTION : Set pencil drawing.
//INPUTS : QPen pointer,bool.
//RETURNS : NONE.
	void ChangePen(QPen* pen,bool bDel){if(bPen&&(pPen))delete pPen;pPen = pen;bPen = bDel;};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::ChangeFont(QFont* font,bool bDel);
//DESCRIPTION : Set font.
//INPUTS : QFont pointer,bool.
//RETURNS : NONE.
	void ChangeFont(QFont* font,bool bDel){if(bFont&&(pFont&&(!m_bSelectedFromPool)))delete pFont;pFont = font;bFont = bDel;};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::ChangeBrush(QBrush* brush,bool bDel);
//DESCRIPTION : Set brush.
//INPUTS : QBrush pointer , bool.
//RETURNS : NONE.
	void ChangeBrush(QBrush* brush,bool bDel){if(bBrush&&(pBrush))delete pBrush;pBrush = brush;bBrush = bDel;};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SelectGDI();
//DESCRIPTION : Set GDI object.
//INPUTS : NONE.
//RETURNS : True.
	bool SelectGDI();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::DeselectGDI();
//DESCRIPTION : Deselect GDI object.
//INPUTS : NONE.
//RETURNS : NONE.
	void DeselectGDI();

//To optimize the rendering should be able to ask
// Device distinguishes whether it is in their logical coordinates
// Specific value

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Distinguish_Text(double dVal);
//DESCRIPTION : 
//INPUTS : double.
//RETURNS : True.  
	bool Distinguish_Text(double dVal);
//  {
//    unsigned iVal = BumToLogicI(dVal);
//    if (drawTextFlag)  iVal = BumToLogicI(textSizeRatio * dVal); 
//    return (iVal >= distinguishTextSize);
//  }

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Distinguish(double dVal);
//DESCRIPTION : 
//INPUTS : double.
//RETURNS : True.
	bool Distinguish(double dVal){int iVal = BumToLogicI(dVal);
		return (iVal>=1);} 

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Polyline(MPoint* pPoints, int iNumPoints);
//DESCRIPTION : Drawing polylines.
//INPUTS : MPoint pointer, int.
//RETURNS : True.
	bool Polyline(MPoint* pPoints, int iNumPoints);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Polygon(MPoint* pPoints, int iNumPoints);
//DESCRIPTION : Drawing polygon.
//INPUTS : MPoint pointer, int .
//RETURNS : True.
	bool Polygon(MPoint* pPoints, int iNumPoints);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::DrawImage(MImage *image, MColor color, MPoint leftTop);
//DESCRIPTION : Drawing pictures.
//INPUTS : MImage pointer, MColor , MPoint.
//RETURNS : True.
	bool DrawImage(MImage *image, MColor color, MPoint leftTop);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetZapolnenie(MImage *image, MColor color);
//DESCRIPTION : Set the brush filling.
//INPUTS : MImage pointer, MColor.
//RETURNS : True.
	bool SetZapolnenie(MImage *image, MColor color);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetZapolnenie(MColor* pColor);
//DESCRIPTION : Set the brush filling.
//INPUTS : MColor pointer.
//RETURNS : True.
    bool SetZapolnenie(MColor* const pColor);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetPencil(M_LOG_PEN* pLPen);
//DESCRIPTION : Set pencil drawing.
//INPUTS : M_LOG_PEN pointer.
//RETURNS : True.
	bool SetPencil(M_LOG_PEN* pLPen);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetPencil(int iPenStyle,double dWidth,MColor* pColor);
//DESCRIPTION : Set a pencil drawing with a specific rounding.
//INPUTS : int ,double ,MColor pointer.
//RETURNS : True.
	bool SetPencil(int iPenStyle,double dWidth,MColor* pColor);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetFont(M_LOG_FONT* lFont);
//DESCRIPTION : Set font.
//INPUTS : M_LOG_FONT pointer.
//RETURNS : True.
	bool SetFont(M_LOG_FONT* lFont);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetTextAlign(unsigned int uiTextAlign);
//DESCRIPTION : Set of text alignment.
//INPUTS : unsigned int.
//RETURNS : unsigned int.
	unsigned int SetTextAlign(unsigned int uiTextAlign);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetTextColor(MColor crTextColor);
//DESCRIPTION : Set the text color.
//INPUTS : MColor.
//RETURNS : MColor.
	MColor SetTextColor(MColor crTextColor);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetBkColor(MColor crBkColor);
//DESCRIPTION : Set the background color.
//INPUTS : MColor.
//RETURNS : MColor.
	MColor SetBkColor(MColor crBkColor);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SetBkMode(int nBkMode);
//DESCRIPTION : Setting the background.
//INPUTS : int.
//RETURNS : int.
	int SetBkMode(int nBkMode);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOut(double dX,double dY,MString& lpszString);
//DESCRIPTION : Text output device.
//INPUTS : double ,double ,MString&.
//RETURNS : True.
    bool TextOut(double dX,double dY, MString& lpszString/*, bool bAligned*/);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOut(double dX,double dY,char * lpszString,int nCount);
//DESCRIPTION : Text output device.
//INPUTS : double ,double ,char pointer,int.
//RETURNS : True.
	bool TextOut(double dX,double dY,char * lpszString,int nCount/*, bool bAligned*/);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOutPramSetka(MPtArray* pta, MRect* rect, MString* str, double step, double dSymVal);
//DESCRIPTION : 
//INPUTS : MPtArray pointer, MRect pointer, MString pointer, double , double.
//RETURNS : NONE.
	bool TextOutPramSetka(MPtArray* pta, MRect* rect, MString* str, double step, double dSymVal);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOutKosSetka(MPtArray* pta, MRect* rect, MString* str, double angle, double step, double dSymVal);
//DESCRIPTION : 
//INPUTS : MPtArray pointer, MRect pointer, MString pointer, double , double , double .
//RETURNS : True.
	bool TextOutKosSetka(MPtArray* pta, MRect* rect, MString* str, double angle, double step, double dSymVal);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOutXaosSetka(MPtArray* pta, MRect* rect, MSymbol* sym, double step, double dSymVal);
//DESCRIPTION : 
//INPUTS : MPtArray pointer, MRect pointer, MSymbol pointer, double , double.
//RETURNS : True.
	bool TextOutXaosSetka(MPtArray* pta, MRect* rect, MSymbol* sym, double step, double dSymVal);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOutWindows(double dX,double dY,char * lpszString,int nCount);
//DESCRIPTION : Output text encoding windows device.
//INPUTS : double ,double ,char  pointer,int .
//RETURNS : True.
	bool TextOutWindows(double dX,double dY,char * lpszString,int nCount/*, bool bAligned*/);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::TextOutWindows(double dX,double dY,MString& lpszString);
//DESCRIPTION :  Output text encoding windows device.
//INPUTS : double ,double ,MString& .
//RETURNS : True.
	bool TextOutWindows(double dX,double dY,MString& lpszString/*, bool bAligned*/);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Rectangle(double dX1,double dY1,double dX2,double dY2);
//DESCRIPTION :  Output of the rectangle.
//INPUTS : double ,double ,double ,double.
//RETURNS : True.
	bool Rectangle(double dX1,double dY1,double dX2,double dY2);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Rectangle(MRect* lpRect);
//DESCRIPTION : 
//INPUTS : MRect pointer.
//RETURNS : True.
	bool Rectangle(MRect* lpRect);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::RoundRect(MRect* lpRect,MPoint EllipsPoint);
//DESCRIPTION : Rounded rectangle.
//INPUTS : MRect pointer,MPoint .
//RETURNS : True.
	bool RoundRect(MRect* lpRect,MPoint EllipsPoint);

	//Позиционирование //!Заменено на ::DrawLine()
	//bool MoveTo( double x, double y );
	//bool MoveTo( MPoint point );

	//Проведение линии //!Заменено на ::DrawLine()
	//bool LineTo( double x, double y );
	//bool LineTo( MPoint point );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::DrawLine( double x1, double y1,double x2, double y2);
//DESCRIPTION : Holding the line.
//INPUTS : double , double ,double , double.
//RETURNS : True.
	bool DrawLine( double x1, double y1,double x2, double y2);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::DrawLine( MPoint point1, MPoint point2 );
//DESCRIPTION : Holding the line.
//INPUTS : MPoint , MPoint.
//RETURNS : True.
	bool DrawLine( MPoint point1, MPoint point2 );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Arc( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
//DESCRIPTION : Drawing an arc.
//INPUTS : double , double , double , double , double , double , double , double .
//RETURNS : True.
	bool Arc( double x1, double y1, double x2, double y2,
		double x3, double y3, double x4, double y4 );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Arc( MRect* lpRect, MPoint ptStart, MPoint ptEnd );
//DESCRIPTION : Drawing an arc.
//INPUTS : MRect pointer, MPoint , MPoint.
//RETURNS : True.
	bool Arc( MRect* lpRect, MPoint ptStart, MPoint ptEnd );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::FillRect( MRect* lpRect, MColor* pColor );
//DESCRIPTION : Filling the rectangle.
//INPUTS : MRect pointer, MColor pointer.
//RETURNS : NONE.
	void FillRect( MRect* lpRect, MColor* pColor );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::FillSolidRect( MRect* lpRect, MColor clr );
//DESCRIPTION : Filling the rectangle.
//INPUTS :  MRect pointer, MColor.
//RETURNS : NONE.
	void FillSolidRect( MRect* lpRect, MColor clr );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::FillSolidRect( double x, double y, double cx, double cy, MColor clr );
//DESCRIPTION : Filling the rectangle.
//INPUTS : double , double , double , double , MColor.
//RETURNS : NONE.
	void FillSolidRect( double x, double y, double cx, double cy,
						MColor clr );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Ellipse( double x1, double y1, double x2, double y2 );
//DESCRIPTION : Drawing an ellipse.
//INPUTS : double , double , double , double.
//RETURNS : True.
	bool Ellipse( double x1, double y1, double x2, double y2 );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Ellipse( MRect* lpRect );
//DESCRIPTION : Drawing an ellipse.
//INPUTS : MRect pointer.
//RETURNS : True.
	bool Ellipse( MRect* lpRect );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Circle( MPoint ptCenter,double dRad,MColor color);
//DESCRIPTION : Drawing a circle.
//INPUTS : MPoint ,double ,MColor.
//RETURNS : True.
	bool Circle( MPoint ptCenter,double dRad,MColor color);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Pie( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );
//DESCRIPTION : Drawing segment.
//INPUTS : double , double , double , double , double , double , double , double .
//RETURNS : True.
	bool Pie( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Pie( MRect* lpRect, MPoint ptStart, MPoint ptEnd );
//DESCRIPTION : Drawing segment.
//INPUTS : MRect pointer, MPoint , MPoint.
//RETURNS : True.
	bool Pie( MRect* lpRect, MPoint ptStart, MPoint ptEnd );

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Pie( MPoint ptCenter, double dRad, double startAngle, double spanAngle);
//DESCRIPTION : Drawing segment.
//INPUTS : MPoint , double , double , double.
//RETURNS : True.
	bool Pie( MPoint ptCenter, double dRad, double startAngle, double spanAngle);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Raster(MRaster* raster);
//DESCRIPTION : Drawing raster = QImage from MRaster-а.
//INPUTS : MRaster pointer.
//RETURNS : True.
	//MAK 05/2007
	bool Raster(MRaster* raster);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::OpacityBackground(int lightness, int transparent);
//DESCRIPTION : Drawing transparent background.
//INPUTS : int, int.
//RETURNS : True.
	//max 02/2009
	bool OpacityBackground(int lightness, int transparent);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SelectStockObject(int );
//DESCRIPTION : Select stock object
//INPUTS : int.
//RETURNS : NONE.
	virtual void SelectStockObject(int ){}; //(int nIndex)
	
//PACKAGE : MAP.
//FUNCTION : MQtDevice::SavePencil();
//DESCRIPTION : Saving pen.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SavePencil();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::RestorePencil();
//DESCRIPTION : Restore pen.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestorePencil();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SaveZapolnenie();
//DESCRIPTION : Saving brush.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SaveZapolnenie();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::RestoreZapolnenie();
//DESCRIPTION : Restore brush.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestoreZapolnenie();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::SaveFont();
//DESCRIPTION : Saving font.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SaveFont();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::RestoreFont();
//DESCRIPTION : Restore font.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void RestoreFont();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::StartSheet();
//DESCRIPTION : Start a new sheet.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartSheet(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::StartLayer();
//DESCRIPTION : Start a new layer.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartLayer(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::StartObject();
//DESCRIPTION : Start a new object.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartObject(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::StartSheetText();
//DESCRIPTION : Start the sheet text.
//INPUTS : NONE.
//RETURNS : NONE. 
	virtual void StartSheetText(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::StartLayerText();
//DESCRIPTION : Start the layer text .
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartLayerText(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::StartObjectText();
//DESCRIPTION : Start the object text .
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void StartObjectText(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::UnInitialize();
//DESCRIPTION : Ending.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void UnInitialize(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndSheet();
//DESCRIPTION : End of sheet.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndSheet(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndLayer();
//DESCRIPTION : End of layer.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndLayer(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndObject();
//DESCRIPTION : End object.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndObject(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndSheetText();
//DESCRIPTION : End sheet text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndSheetText(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndLayerText();
//DESCRIPTION : End layer text.
//INPUTS : NONE.
//RETURNS : NONE.  
	virtual void EndLayerText(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::EndObjectText();
//DESCRIPTION : End object text.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void EndObjectText(){};

//PACKAGE : MAP.
//FUNCTION : MQtDevice::GetTextExtent(MString& str);
//DESCRIPTION : Getting the length of the text.
//INPUTS : MString&.
//RETURNS : double.
	virtual double GetTextExtent(MString& str);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::DrawServantPolyLine(MPoint* ,int ,int ,MColor );
//DESCRIPTION : Drawing polylines service.
//INPUTS : MPoint* ,int ,int ,MColor.
//RETURNS : NONE.
	//params (MPoint* par,int iNumPt,int iLogicWidth,MColor crColor = MColor(0,0,0))
	virtual void DrawServantPolyLine(MPoint* ,int ,int ,MColor ){};


//	void SetTextLimit(int i) {m_show_text_limit = i;}    // vipa 2004.06.25
//	int GetTextLimit() {return m_show_text_limit;}       // vipa 2004.06.25

//PACKAGE : MAP.
//FUNCTION : MQtDevice::GetSizeMM();
//DESCRIPTION : Getting the size in millimeters.
//INPUTS : NONE.
//RETURNS : MIntSize.
	virtual MIntSize GetSizeMM();

//PACKAGE : MAP.
//FUNCTION : MQtDevice::GetCurrentScale();
//DESCRIPTION : Getting the current scale.
//INPUTS : NONE.
//RETURNS : double.
    virtual double GetCurrentScale() { return geo_scale; }

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Normalize (QPoint &pt);
//DESCRIPTION : Function normalization. Needed in QT due to restriction
//				rectangle O ( -4000 ; -4000 ; 4000 ; 4000).
//INPUTS : QPoint &.
//RETURNS : NONE.
	inline  void Normalize (QPoint &pt);       // vipa 2005.01.20

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Normalize (QRect  &rect);
//DESCRIPTION : Function normalization. Needed in QT due to restriction
//				rectangle O ( -4000 ; -4000 ; 4000 ; 4000).
//INPUTS : QRect  &.
//RETURNS : NONE.
	inline  void Normalize (QRect  &rect);     // vipa 2005.01.20

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Normalize(QPoint MPt, QPoint &pt);
//DESCRIPTION : Function normalization. Needed in QT due to restriction
//				rectangle O ( -4000 ; -4000 ; 4000 ; 4000).
//INPUTS : QPoint , QPoint&.
//RETURNS : NONE.
	inline  void Normalize(QPoint MPt, QPoint &pt);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Normalize (QPolygon &pta);
//DESCRIPTION : Function normalization. Needed in QT due to restriction
//				rectangle O ( -4000 ; -4000 ; 4000 ; 4000).
//INPUTS : QPolygon &.
//RETURNS : NONE.
	void Normalize (QPolygon &pta);

//PACKAGE : MAP.
//FUNCTION : MQtDevice::Intersects(QPolygon &pta);
//DESCRIPTION : 
//INPUTS : QPolygon &.
//RETURNS : True.
	bool Intersects(QPolygon &pta);      // vipa 2005.01.20 

    void SetGeoScale(int scale) { geo_scale = scale; }
    
    QPainter * painter() {return m_painter;}
};

#endif // _H_QT_DEVICE
