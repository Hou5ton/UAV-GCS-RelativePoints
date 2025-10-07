//PACKAGE : MAP.
//FILE: BaseCl.h
//AUTHOR:
//DESCRIPTION: 

#ifndef _BASECL__H_
#define _BASECL__H_
#include <math.h> 
#include <float.h>
#include <cstdio>
#include <vector>
#include <stdarg.h>
#include "MString.h"
#include "File.h"

#define FONT_10000 "UTKarta2"
#define UT_TRANSPARENT	1
#define UT_OPAQUE		2

//#ifdef _WINDOWS
#define W_TA_LEFT		0
#define W_TA_RIGHT		2
#define W_TA_CENTER		6
#define W_TA_TOP		0
#define W_TA_BOTTOM		8
#define W_TA_BASELINE	24
//#else
#define UT_TA_LEFT		0x0001
#define UT_TA_RIGHT		0x0002
#define UT_TA_CENTER	0x0004
#define UT_TA_TOP		0x0010
#define UT_TA_BOTTOM	0x0020
#define UT_TA_BASELINE	0x0040
//#endif

// At convertion DOUBLE to INT all fractional part is truncated
// But from mathematic point of view it is not correct
#define dbl_to_int(a) ((int)((a-floor(a))>=0.5)?(ceil(a)):(floor(a)))

const double pi = 3.141592653589793238462643;  // PI constant

// Defined variables(destined to work with BMP-files) are listed below:
typedef char* UT_BITMAP;
//  RGB colors and alpha-channel
struct UT_RGBQUAD{
    unsigned char    rgbBlue;
    unsigned char    rgbGreen;
    unsigned char    rgbRed;
    unsigned char    rgbReserved;
};
//  Title of MBP-file
struct UT_BITMAPFILEHEADER{
    unsigned short    bfType;
    unsigned int   bfSize;
    unsigned short    bfReserved1;
    unsigned short    bfReserved2;
    unsigned int   bfOffBits;
};
//  Information title of BMP-file
struct   UT_BITMAPINFOHEADER{
    unsigned int  biSize;
    int   biWidth;
    int   biHeight;
    unsigned short   biPlanes;
    unsigned short   biBitCount;
    unsigned int  biCompression;
    unsigned int  biSizeImage;
    int    biXPelsPerMeter;
    int    biYPelsPerMeter;
    unsigned int  biClrUsed;
    unsigned int  biClrImportant;
};

//  Work information of BMP-file
struct UT_BITMAPINFO{
    UT_BITMAPINFOHEADER bmiHeader;
    UT_RGBQUAD          bmiColors[1];
};

//===============================================================================
//  Additional classes
//===============================================================================
//  Structure for getting glyph parameters in a horizontal plane
typedef struct glyph_tag{
    double dLeftPart;
    double dRightPart;
    double dCenter;
    double dGlyphWidthInCenter;
    double dGlyphStartInCenter;
}GLYPH_PARAMS;

/// Logical (User's) font. Replaces MFC-structure LOGFONT
struct M_LOG_FONT{
    double lfHeight;
    int lfWidth;
    int lfEscapement;
    int lfOrientation;
    int lfWeight;
    unsigned char lfItalic;
    unsigned char lfUnderline;
    unsigned char lfStrikeOut;
    unsigned char lfCharSet;
    unsigned char lfOutPrecision;
    unsigned char lfClipPrecision;
    unsigned char lfQuality;
    unsigned char lfPitchAndFamily;
    char lfFaceName[32];
    /*  M_LOG_FONT(LOGFONT* lf){
      memcpy((void*)&lfWidth,(void*)(((char*)lf)+4),sizeof(LOGFONT)-4);
  };
  operator LOGFONT(){
      LOGFONT lf;
      memcpy((void*)(((char*)&lf)+4),(void*)&lfWidth,sizeof(LOGFONT)-4);
      return lf;
  };*/
    M_LOG_FONT(){memset((void*)&lfHeight,0x0,sizeof(M_LOG_FONT));};
};

//==================================
//  Structure stores size (as integers)
struct MIntSize
        //==================================
{
    // Variables
    int cx, cy;

    //PACKAGE : MAP.
    //FUNCTION : MIntSize();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MIntSize():cx(0), cy(0){};

    //PACKAGE : MAP.
    //FUNCTION : MIntSize(int X, int Y);
    //DESCRIPTION : Constructor with defined values.
    //INPUTS : int, int.
    //RETURNS : NONE.
    MIntSize(int X, int Y):cx(X), cy(Y){};
};

    /**
            CLASS       : SNumberGeo
    \brief  DESCRIPTION : Structure implements geodesic point in degree, minutes and seconds
    */
    struct SNumberGeo
    {
        int m_iDeg;///< degree
        int m_iMin;///< minutes
        int m_iSec;///< seconds

        SNumberGeo()
        {
            Clear();
        }

        explicit SNumberGeo(const double& dCoordinate)
        {
            m_iDeg = (int)(dCoordinate);
            m_iMin = (int)((dCoordinate - (double)m_iDeg)*60.0);
            m_iSec = (int)((dCoordinate - (double)m_iDeg - (double)m_iMin/60.0)*3600.0);
        }

        SNumberGeo(const int& iDeg, const int& iMin, const int& iSec)
            : m_iDeg(iDeg),
              m_iMin(iMin),
              m_iSec(iSec)
        {
        }

        ///  DESCRIPTION : Clears data
        void Clear()
        {
            m_iDeg = 0.0;
            m_iMin = 0.0;
            m_iSec = 0.0;
        }

        ///  DESCRIPTION : Returns point
        double GetDouble() const
        {
            return  m_iDeg + (double)m_iMin/60.0 + (double)m_iSec/3600.0;
        }

        ///  DESCRIPTION : Returns point
        long GetLong()
        {
            return (m_iDeg*3600 + m_iMin*60 + m_iSec)*100;
        }

        ///  DESCRIPTION : Sets point
        void SetLong(const long & value)
        {
            m_iDeg = value/360000;
            m_iMin = (value - m_iDeg*360000)/6000;
            m_iSec = (value - m_iDeg*360000 - m_iMin*6000)/100;
        }
    };


//  Point with integer coordinates
struct MIntPoint

{
    // variables
    int x, y;

    //PACKAGE : MAP.
    //FUNCTION : MIntPoint();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MIntPoint():x(0), y(0){};

    //PACKAGE : MAP.
    //FUNCTION : MIntPoint(int X, int Y);
    //DESCRIPTION : Constructor with defined values.
    //INPUTS : int, int.
    //RETURNS : NONE.
    MIntPoint(int X, int Y):x(X), y(Y){};
};

//PACKAGE : MAP.
//CLASS	: MPoint.                                                                
//DESCRIPTION : geopoint with fractional coordinate values.
class MPoint

{
public:
    //  variables
    double x;
    double y;

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MPoint():x(0),y(0){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint(int X,int Y);
    //DESCRIPTION : Constructor with defined values.
    //INPUTS : int, int.
    //RETURNS : NONE.
    MPoint(int X,int Y):x((double)X),y((double)Y){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint(const MIntPoint& pt);
    //DESCRIPTION : Constructor of integer point.
    //INPUTS : const MIntPoint& .
    //RETURNS : NONE.
    MPoint(const MIntPoint& pt):x((double)pt.x),y((double)pt.y){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint(double X,double Y);
    //DESCRIPTION : Constructor of fractional values.
    //INPUTS : double, double.
    //RETURNS : NONE.
    MPoint(double X,double Y):x(X),y(Y){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint(const MPoint& dpt);
    //DESCRIPTION : Constructor of variables.
    //INPUTS : const MPoint& .
    //RETURNS : NONE.
    MPoint(const MPoint& dpt):x(dpt.x),y(dpt.y){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint(const MPoint* dpt);
    //DESCRIPTION : Constructor of pointer.
    //INPUTS : const MPoint* .
    //RETURNS : NONE.
    MPoint(const MPoint* dpt):x(dpt->x),y(dpt->y){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MPoint(double dValue);
    //DESCRIPTION : Constructor of number.
    //INPUTS : double.
    //RETURNS : NONE.
    MPoint(double dValue):x(dValue),y(dValue){};

    //PACKAGE : MAP.
    //FUNCTION : MPoint::~MPoint();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MPoint(){};
    /*
    //  Memory distribution
    void* operator new ( size_t size) {
        return malloc( 2 * sizeof(double)* size );
    }

    //  Frees memory
    void operator delete ( void *pvTemp) {
        free(pvTemp);
    }
    */
    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator = ;
    //DESCRIPTION : Operator of assignment.
    //INPUTS : MIntPoint.
    //RETURNS : const MIntPoint.
    const MPoint& operator = (const MIntPoint pt) {
        x = (double)pt.x;
        y = (double)pt.y;
        return *this;

    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator = ;
    //DESCRIPTION : Operator of assignment.
    //INPUTS : MPoint.
    //RETURNS : const MPoint.
    const MPoint& operator = (const MPoint dpt) {
        x = dpt.x;
        y = dpt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator = ;
    //DESCRIPTION : Operator of assignment.
    //INPUTS : MPoint pointer.
    //RETURNS : const MPoint pointer.
    const MPoint& operator = (const MPoint* dpt) {
        x = dpt->x;
        y = dpt->y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator +=;
    //DESCRIPTION : Operator of addition with point.
    //INPUTS : MIntPoint.
    //RETURNS : const MIntPoint.
    const MPoint& operator += (const MIntPoint pt) {
        x += (double)pt.x;
        y += (double)pt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator +=;
    //DESCRIPTION : Operator of addition.
    //INPUTS : MPoint.
    //RETURNS : const MPoint.
    const MPoint& operator += (const MPoint dpt) {

        x += dpt.x;
        y += dpt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator -=;
    //DESCRIPTION : Operator of substraction.
    //INPUTS : MPoint.
    //RETURNS : const MPoint.
    const MPoint& operator -= (const MPoint dpt) {
        x -= dpt.x;
        y -= dpt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator *=;
    //DESCRIPTION : Operator of multiplication.
    //INPUTS : double.
    //RETURNS : const double .
    const MPoint& operator *= (const double koeff) {
        x *= koeff;
        y *= koeff;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator ==;
    //DESCRIPTION : Operators of comparison.
    //INPUTS : MIntPoint.
    //RETURNS : const int.
    bool operator == (const MIntPoint pt) {
        if ((x == pt.x)&&(y == pt.y)) return true;
        return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator ==;
    //DESCRIPTION : Operators of comparison.
    //INPUTS : MPoint.
    //RETURNS : const int.
    bool operator == (const MPoint dpt) const {
        if ((x == dpt.x)&&(y == dpt.y)) return true;
        return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator !=;
    //DESCRIPTION : Operators of comparison.
    //INPUTS :  MIntPoint.
    //RETURNS : const int.
    bool operator != (const MIntPoint pt) {
        if ((x == pt.x)&&(y == pt.y)) return false;
        return true;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::operator !=;
    //DESCRIPTION : Operators of comparison.
    //INPUTS : MPoint.
    //RETURNS : True.
    bool operator != (const MPoint dpt) {
        if ((x == dpt.x)&&(y == dpt.y)) return false;
        return true;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::Distance(MPoint* pt);
    //DESCRIPTION : Calculates distance.
    //INPUTS : MPoint pointer.
    //RETURNS : Double.
    double Distance(MPoint* pt) {
        return sqrt((x - pt->x)*(x - pt->x) + (y - pt->y)*(y - pt->y));
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::Invert();
    //DESCRIPTION : Inverses coordinates.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void Invert() {
        double a = x;
        x = y;
        y = a;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::Rotate(double A, double B, double C1, double C2);
    //DESCRIPTION : Makes rotate transformation.
    //INPUTS : double, double, double, double.
    //RETURNS : NONE.
    void Rotate(double A, double B, double C1, double C2){
        double xx,yy;
        xx=A*x-B*y+C1;

        yy=B*x+A*y+C2;
        x=xx; y=yy;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPoint::Scale( double K, double XC, double YC);
    //DESCRIPTION : Makes scale transformation.
    //INPUTS : double, double, double.
    //RETURNS : NONE.
    void Scale( double K, double XC, double YC){
        double xx,yy;
        xx=K*x+(1-K)*XC;
        yy=K*y+(1-K)*YC;
        x=xx; y=yy;
    }

    // -----------------------------------------------------------------
    //  InnerPt(MPoint* pPlg, int n) & InnerPtFast(MPoint* pPlg, int n)
    //
    //  2 functions of check: If point (x,y)
    //  is internal point of arbitrary polygon 'Plg'
    // -----------------------------------------------------------------
    //  Sense: from the point 'pt' draw to the right a horizontal beam, which one
    //  will across some edges of polygon.
    //    If number of these edges is not even, that means point (x,y) - internal
    //    attitude to polygon.
    //    If number of edges is even, that means 'pt' - external point attitude to polygon

    //  parameters: (x,y) - random point
    // 			  pPlg - pointer to polygon points array of type 'double'
    // 			  n - dimension of array
    //  Returnable value: true - if (x,y) - an internal point,
    // 						  false - otherwise
    //  The remark:  in a case of when x-coordinates of the edge lay on the different sides
    //  from x-coordinate of point, we search for a point 'pt' crossing by horizontal straight line,
    //  going through a point (x,y), and a straight line, which is going through the edge of polygon;
    //  if õ-coordinate of a point of crossing lays to the right of x-coordinate of a point(x,y),
    //  than horizontal beam will across the edge

    // --------------------------
    //  Checks: if the point (x,y)
    //  is internal point of random polygon Plg

    //PACKAGE : MAP.
    //FUNCTION : MPoint::InnerPt(MPoint* pPlg, int n);
    //DESCRIPTION : Inverses coordinates.
    //INPUTS : MPoint pointer, int.
    //RETURNS : True.
    bool InnerPt(MPoint* pPlg, int n)
    {
        MPoint pt; //  pt - point of intersection of horizontal line with line that crosses the edge
        int col = 0; //  col - Shows number of crossing of horizontal beam with polygon edges
        std::vector<unsigned int> indArray; //  In the dynamic array 'indArray' saves indexes of polygon edges, through which lays to the right points 'pt'
        // indArray.SetSize(0);
        int i = 0;
        for(i=0; i<n-1; i++){
            if((pPlg[i].x>=x) & (pPlg[i+1].x>=x))
                indArray.push_back(i);
            //  In case when x-coordinates of edge lay on different sides from x-coordinate of point, than looking for
            // crossing point 'pt1' of horizontal line , that goes through a point 'pt', and a line, that goes through the polygon edge.
            //  if o-coordinate of crossing point lays to the right from x-coorfinate of point 'pt', than horizontal beam will across
            //  the edge in a case of perfomance of a condition on y-coordinate of all points
            else if(((pPlg[i].x<x) & (pPlg[i+1].x>x)) || ((pPlg[i].x>x) & (pPlg[i+1].x<x))){
                //  Formula of estimating of point coordinates crossing. Used from guide of Bronstein-Semendiaev
                pt.x = (y*(pPlg[i].x - pPlg[i+1].x) - pPlg[i].x*(pPlg[i+1].y - pPlg[i].y) + pPlg[i].y*(pPlg[i+1].x - pPlg[i].x))/(pPlg[i].y - pPlg[i+1].y);
                pt.y = y;
                if(pt.x>x)
                    indArray.push_back(i);
            }
        }
        if((pPlg[n-1].x>=x) & (pPlg[0].x>=x))
            indArray.push_back(n-1);
        else if(((pPlg[n-1].x<x) & (pPlg[0].x>x)) || ((pPlg[n-1].x>x) & (pPlg[0].x<x))){
            pt.x = (y*(pPlg[n-1].x - pPlg[0].x) - pPlg[n-1].x*(pPlg[0].y - pPlg[n-1].y) + pPlg[n-1].y*(pPlg[0].x - pPlg[n-1].x))/(pPlg[n-1].y - pPlg[0].y);
            pt.y = y;
            if(pt.x>x)
                indArray.push_back(n-1);
        }
        int m = indArray.size();

        for(i=0
            ; i<m; i++) {
            if(indArray[i] != (unsigned int)(n-1)){
                if(((pPlg[indArray[i]].y > y) & (pPlg[indArray[i] + 1].y < y)) || ((pPlg[indArray[i]].y < y) & (pPlg[indArray[i] + 1].y > y)))
                    col++;
                //  Separately considering cases when horizontal beam crosses vertex of polygon 'pPlg'
                else{
                    //comment veronika 23.12.2016 s is not used
                    //					int s;
                    //					if(indArray[i]==0)
                    //						s = n-1;
                    //					else
                    //						s = indArray[i]-1;
                    //  commented condition leads to non correct results - must be checked
                    if(((pPlg[indArray[i]].y > y) & (pPlg[indArray[i] + 1].y == y)/* & (pPlg[s].y < y)*/) || ((pPlg[indArray[i]].y < y) & (pPlg[indArray[i] + 1].y == y)/* & (pPlg[s].y > y)*/)){
                        col++;
                        i++;
                    }
                }
            }
            else{
                if(((pPlg[n-1].y > y) & (pPlg[0].y < y)) || ((pPlg[n-1].y < y) & (pPlg[0].y > y)))
                    col++;
                //  Separately considering cases when horizontal beam crosses a vertex of polygon 'pPlg'
                else if(((pPlg[n-1].y > y) & (pPlg[0].y == y)/* & (pPlg[n-2].y < y)*/) || ((pPlg[n-1].y < y) & (pPlg[0].y == y)/* & (pPlg[n-2].y > y)*/))
                    col++;
            }
        }
        //  Checks if parameter 'col' is even or not
        if((double)((double)col/2) != (int)((double)col/2))
            return true;
        else
            return false;
    };

    //PACKAGE : MAP.
    //FUNCTION : MPoint::InnerPtFast(MPoint* pPlg, int n);
    //DESCRIPTION : Quick check: if the point (x,y)
    //  		is internal point of random polygon Plg.
    //INPUTS : MPoint pointer, int.
    //RETURNS : True.
    bool InnerPtFast(MPoint* pPlg, int n)
    {
        MPoint pt; //  pt - point of crossing of horizontal line and a line that goes through an edge of polygon
        int col = 0; //  col - shows quantity of crossing of horizontal beam with the edges of polygon
        int i =0;
        for(i=0; i<n-1; i++)
            if(((pPlg[i].y>=y)&&(pPlg[i+1].y<=y))||((pPlg[i].y<=y)&&(pPlg[i+1].y>=y))){
                if((pPlg[i].x<x) && (pPlg[i+1].x<x))
                    continue;
                else if((pPlg[i].y==y)&&(pPlg[i+1].y==y))
                    continue;
                else if((pPlg[i].x>=x) && (pPlg[i+1].x>=x))
                    col++;
                else{
                    //  Formula of estimating of crossing point coordinates is taken from guide of Bronstein-Semendiaev
                    pt.x = (y*(pPlg[i].x - pPlg[i+1].x) - pPlg[i].x*(pPlg[i+1].y - pPlg[i].y) + pPlg[i].y*(pPlg[i+1].x - pPlg[i].x))/(pPlg[i].y - pPlg[i+1].y);
                    pt.y = y;
                    if(pt.x>x)

                        col++;
                }
            };

        if(((pPlg[n-1].y>=y)&&(pPlg[0].y<=y))||((pPlg[n-1].y<=y)&&(pPlg[0].y>=y))){
            if((pPlg[i].x<x) && (pPlg[i+1].x<x));
            else if((pPlg[n-1].y==y)&&(pPlg[0].y==y));

            else if((pPlg[n-1].x>=x) && (pPlg[0].x>=x))
                col++;
            else{
                pt.x = (y*(pPlg[n-1].x - pPlg[0].x) - pPlg[n-1].x*(pPlg[0].y - pPlg[n-1].y) + pPlg[n-1].y*(pPlg[0].x - pPlg[n-1].x))/(pPlg[n-1].y - pPlg[0].y);
                pt.y = y;
                if(pt.x>x)
                    col++;
            }
        }
        if(col%2)
            return true;
        return false;
    }

};//EoCLASS MPoint

///Current structure describes polygon with integer values of edges coordinates
struct MIntRect
{
    // variables
    int left;
    int top;
    int right;
    int bottom;

    //PACKAGE : MAP.
    //FUNCTION : MIntRect();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MIntRect():left(0), top(0), right(0), bottom(0){};

    //PACKAGE : MAP.
    //FUNCTION : MIntRect(int X1, int Y1, int X2, int Y2);
    //DESCRIPTION : Constructor.
    //INPUTS : int, int, int, int.
    //RETURNS : NONE.
    MIntRect(int X1, int Y1, int X2, int Y2):left(X1), top(Y1), right(X2), bottom(Y2){};
};

//PACKAGE : MAP.
//CLASS	: MRect.
//DESCRIPTION : Georectangle (edges coordinates - double).
class MRect

{
public:
    //  variables - edges coordinates
    double left;
    double top;
    double right;
    double bottom;

    //PACKAGE : MAP.
    //FUNCTION : MPoint::MRect();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MRect(){ left = 0.0; top = 0.0; right = 0.0; bottom = 0.0;};

    //PACKAGE : MAP.
    //FUNCTION : MRect::MRect(int X1,int Y1,int X2, int Y2);
    //DESCRIPTION : Constructor.
    //INPUTS : int ,int ,int , int .
    //RETURNS : NONE.
    MRect(int X1,int Y1,int X2, int Y2) { left = (double)X1; top = (double)Y1; right = (double)X2; bottom = (double)Y2;};

    //PACKAGE : MAP.
    //FUNCTION : MRect::MRect(const MIntPoint& pt1, const MIntPoint& pt2);
    //DESCRIPTION : Constructor.
    //INPUTS : const MIntPoint& , const MIntPoint& .
    //RETURNS : NONE.
    MRect(const MIntPoint& pt1, const MIntPoint& pt2) { left = (double)pt1.x; top = (double)pt1.y; right = (double)pt2.x; bottom = (double)pt2.y;};

    //PACKAGE : MAP.
    //FUNCTION : MRect::MRect(const MIntRect& rect);
    //DESCRIPTION : Constructor.
    //INPUTS : const MIntRect& .
    //RETURNS : NONE.
    MRect(const MIntRect& rect) {left = (double)rect.left; top = (double)rect.top; right = (double)rect.right; bottom = (double)rect.bottom;};

    //PACKAGE : MAP.
    //FUNCTION : MRect::MRect(double X1,double Y1,double X2,double Y2);
    //DESCRIPTION : Constructor.
    //INPUTS : double ,double ,double ,double .
    //RETURNS : NONE.
    MRect(double X1,double Y1,double X2,double Y2) {left = X1; top = Y1; right = X2; bottom = Y2;};

    //PACKAGE : MAP.
    //FUNCTION : MRect::MRect(const MPoint& dpt1, const MPoint& dpt2);
    //DESCRIPTION : Constructor.
    //INPUTS : const MPoint& , const MPoint& .
    //RETURNS : NONE.
    MRect(const MPoint& dpt1, const MPoint& dpt2) {left = dpt1.x; top = dpt1.y; right = dpt2.x; bottom = dpt2.y;};

    //PACKAGE : MAP.
    //FUNCTION : MRect::MRect(const MRect& drect);
    //DESCRIPTION : Constructor.
    //INPUTS : const MRect& .
    //RETURNS : NONE.
    MRect(const MRect& drect) {left = drect.left; top = drect.top; right = drect.right; bottom = drect.bottom;};
    ~MRect(){};

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator = ;
    //DESCRIPTION : Operator of assignment.
    //INPUTS : MIntRect.
    //RETURNS : const MIntRect.
    const MRect& operator = (const MIntRect rect) {
        left = (double)rect.left;
        top = (double)rect.top;
        right = (double)rect.right;
        bottom = (double)rect.bottom;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator = ;
    //DESCRIPTION : Operator of assignment.
    //INPUTS : MRect.
    //RETURNS : const MRect.
    const MRect& operator = (const MRect drect) {
        left = drect.left;
        top = drect.top;
        right = drect.right;
        bottom = drect.bottom;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator = ;
    //DESCRIPTION : Operator of assignment.
    //INPUTS : MRect pointer.
    //RETURNS : const MRect pointer.
    const MRect& operator = (const MRect* drect) {
        left = drect->left;
        top = drect->top;
        right = drect->right;
        bottom = drect->bottom;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator += ;
    //DESCRIPTION : Operator of addition.
    //INPUTS : MIntPoint.
    //RETURNS : const MIntPoint.
    const MRect& operator += (const MIntPoint pt) {
        left += (double)pt.x;
        top += (double)pt.y;
        right += (double)pt.x;
        bottom += (double)pt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator += ;
    //DESCRIPTION : Operator of addition.
    //INPUTS : MIntPoint pointer.
    //RETURNS : const MIntPoint pointer.
    const MRect& operator += (const MIntPoint* pt) {
        left += (double)pt->x;
        top += (double)pt->y;
        right += (double)pt->x;
        bottom += (double)pt->y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator += ;
    //DESCRIPTION : Operator of addition.
    //INPUTS : const MPoint.
    //RETURNS : const MPoint.
    const MRect& operator += (const MPoint dpt) {
        left += dpt.x;
        top += dpt.y;
        right += dpt.x;
        bottom += dpt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator += ;
    //DESCRIPTION : Operator of addition.
    //INPUTS : MPoint pointer.
    //RETURNS : const MPoint pointer.
    const MRect& operator += (const MPoint* dpt) {
        left += dpt->x;
        top += dpt->y;
        right += dpt->x;
        bottom += dpt->y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator -= ;
    //DESCRIPTION : Operator of substraction.
    //INPUTS : MPoint.
    //RETURNS : const MPoint.
    const MRect& operator -= (const MPoint dpt) {

        left -= dpt.x;
        top -= dpt.y;
        right -= dpt.x;
        bottom -= dpt.y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator -= ;
    //DESCRIPTION : Operator of multiplication .
    //INPUTS : MPoint*.
    //RETURNS : const MPoint*.
    const MRect& operator -= (const MPoint* dpt) {
        left -= dpt->x;
        top -= dpt->y;
        right -= dpt->x;
        bottom -= dpt->y;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator *=;
    //DESCRIPTION : Operator of comparison.
    //INPUTS : val.
    //RETURNS : double val.
    const MRect& operator *= (double val){
        double width = Width()*(sqrt(fabs(val))-1)/2;
        if (right<left) width*=-1.0;
        left -= width;
        right += width;
        double height = Height()*(sqrt(fabs(val))-1)/2;
        if (top<bottom) height*=-1.0;
        top += height;
        bottom -= height;
        return *this;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::perator ==;
    //DESCRIPTION : Operator of comparison.
    //INPUTS : MIntRect.
    //RETURNS : True.
    bool operator == (const MIntRect rect) {
        if ((left == rect.left)&&(top == rect.top)&&(right == rect.right)&&(bottom == rect.bottom) ) return true;
        else return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator ==;
    //DESCRIPTION : Operator of comparison.
    //INPUTS : MRect.
    //RETURNS : True.
    bool operator == (const MRect drect) {
        if ((left == drect.left)&&(top == drect.top)&&(right == drect.right)&&(bottom == drect.bottom) ) return true;
        else return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator !=;
    //DESCRIPTION : Operator of comparison.
    //INPUTS : MIntRect.
    //RETURNS : True.
    bool operator != (const MIntRect rect) {
        if ((left == rect.left)&&(top == rect.top)&&(right == rect.right)&&(bottom == rect.bottom) ) return false;
        else return true;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::operator !=;
    //DESCRIPTION : Operator of comparison.
    //INPUTS : MRect.
    //RETURNS : True.
    bool operator != (const MRect drect) {
        if ((left == drect.left)&&(top == drect.top)&&(right == drect.right)&&(bottom == drect.bottom) ) return false;
        else return true;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::NormalizeGeoRect();
    //DESCRIPTION : Normalizes vertex order.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void NormalizeGeoRect()
    {
        if (left > right)
        {
            std::swap(left, right);
        }

        if (top < bottom)
        {
            std::swap(bottom, top);
        }
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::UnionGeoRect(MRect& grect,MRect& drect);
    //DESCRIPTION : Merges georectangles.
    //INPUTS : MRect&, MRect&.
    //RETURNS : NONE.
    void UnionGeoRect(MRect& grect,MRect& drect) {
        if (grect.left>drect.left)
        {grect.left = drect.left;
        }
        if (grect.right<drect.right)
        {grect.right = drect.right;
        }
        if (grect.top<drect.top)
        {grect.top = drect.top;
        }
        if (grect.bottom>drect.bottom)
        {grect.bottom = drect.bottom;
        }
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::IsGeoRectEmpty();
    //DESCRIPTION : Checks on null length/width.
    //INPUTS : NONE.
    //RETURNS : True.
    bool IsGeoRectEmpty() {
        if ((left==right)||
                (left>right)||
                (top==bottom)||
                (top>bottom))
            return true;
        else return false;
    }

    /*
//   2004.10.05 vipa
static bool IntersectGeoRectFast(MRect& grect,MRect& drect) {
        if (grect.left>=drect.left)
          if (grect.left<=drect.right)
            if (grect.top>=drect.top)
              if (grect.bottom<=drect.top)  return true;
          else; // return false;
            else
              if (grect.top>=drect.bottom)  return true;
          else; // return false;
      else; // return false;

        else   // grect.left < drect.left
          if (drect.left<=grect.right)
            if (drect.top>=grect.top)
              if (drect.bottom<=grect.top)  return true;
          else; // return false;
            else
          if (drect.top>=grect.bottom)  return true;
          else; // return false;
      else;  // return false;

        return false;
    }
*/ 
    //PACKAGE : MAP.
    //FUNCTION : MRect::IntersectGeoRect(MRect& grect,MRect& drect);
    //DESCRIPTION : Checks for crossing of two rectangles.
    //INPUTS : MRect&, MRect&.
    //RETURNS : True.
    static bool IntersectGeoRect(MRect& grect,MRect& drect) {
        MRect srect(grect);
        MRect prect(drect);
        srect.NormalizeGeoRect();
        prect.NormalizeGeoRect();
        //  checks if any vertexes of the first rectangle
        //  get inside of second rectangle
        if ((srect.left>=prect.left)&&
                (srect.left<=prect.right)&&
                (srect.top<=prect.top)&&
                (srect.top>=prect.bottom))
            return true;
        if ((srect.right>=prect.left)&&
                (srect.right<=prect.right)&&
                (srect.top<=prect.top)&&
                (srect.top>=prect.bottom))
            return true;
        if ((srect.left>=prect.left)&&
                (srect.left<=prect.right)&&
                (srect.bottom<=prect.top)&&
                (srect.bottom>=prect.bottom))
            return true;
        if ((srect.right>=prect.left)&&
                (srect.right<=prect.right)&&
                (srect.bottom<=prect.top)&&
                (srect.bottom>=prect.bottom))
            return true;
        //  checks if any vertexes of the second rectangle
        //  get inside of the first rectangle
        if ((prect.left>=srect.left)&&
                (prect.left<=srect.right)&&
                (prect.top<=srect.top)&&
                (prect.top>=srect.bottom))
            return true;
        if ((prect.right>=srect.left)&&
                (prect.right<=srect.right)&&
                (prect.top<=srect.top)&&
                (prect.top>=srect.bottom))
            return true;
        if ((prect.left>=srect.left)&&
                (prect.left<=srect.right)&&

                (prect.bottom<=srect.top)&&
                (prect.bottom>=srect.bottom))
            return true;
        if ((prect.right>=srect.left)&&
                (prect.right<=srect.right)&&
                (prect.bottom<=srect.top)&&
                (prect.bottom>=srect.bottom))
            return true;
        //  Checks on a case of rectangle interiors crossing
        if ((prect.left<=srect.left)&&
                (prect.right>=srect.right)&&
                (prect.top<=srect.top)&&
                (prect.bottom>=srect.bottom))
            return true;
        if ((srect.left<=prect.left)&&
                (srect.right>=prect.right)&&
                (srect.top<=prect.top)&&
                (srect.bottom>=prect.bottom))
            return true;
        return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::IsRectInRect(MRect& grect,MRect& drect);
    //DESCRIPTION : Checks if one of rectangles includes another.
    //INPUTS : MRect&, MRect&.
    //RETURNS : True.
    bool IsRectInRect(MRect& grect,MRect& drect)
    {
        MRect srect(grect);
        MRect prect(drect);
        srect.NormalizeGeoRect();
        prect.NormalizeGeoRect();
        //   checks if any vertexes of the first rectangle
        //  get inside of second rectangle: approximately
        if ((srect.left>=prect.left)&&
                (srect.left<=prect.right)&&
                (srect.top<=prect.top)&&
                (srect.top>=prect.bottom)&&
                (srect.right>=prect.left)&&
                (srect.right<=prect.right)&&
                (srect.top<=prect.top)&&
                (srect.top>=prect.bottom)&&
                (srect.left>=prect.left)&&
                (srect.left<=prect.right)&&
                (srect.bottom<=prect.top)&&
                (srect.bottom>=prect.bottom)&&
                (srect.right>=prect.left)&&
                (srect.right<=prect.right)&&
                (srect.bottom<=prect.top)&&
                (srect.bottom>=prect.bottom))
            return true;
        return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::NormalizeGeoRect();
    //DESCRIPTION : Checks if the point is inside the rectangle.
    //INPUTS : const MPoint&.
    //RETURNS : True.
    bool IsPtInRect(const MPoint& pt)
    {
        //   checks if any vertexes of the second rectangle
        //  get inside of first rectangle: approximately
        if ((pt.x>=left)&&
                (pt.x<=right)&&
                (pt.y<=top)&&
                (pt.y>=bottom))
            return true;
        return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::ExpandToPt(MPoint& pt);
    //DESCRIPTION :  Enlarges rectangle to include defined point).
    //INPUTS : MPoint&.
    //RETURNS : NONE.
    void ExpandToPt(MPoint& pt)
    {
        if (pt.x < left)
            left   = pt.x;
        if (pt.x > right)
            right  = pt.x;
        if (pt.y > top)
            top    = pt.y;
        if (pt.y < bottom)
            bottom = pt.y;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::Inflate(double rasst);
    //DESCRIPTION : Enlarges rectangle by defined length.
    //INPUTS : double.
    //RETURNS : NONE.
    void Inflate(double rasst)
    {
        NormalizeGeoRect();
        left	-= rasst;
        right	+= rasst;
        top		+= rasst;
        bottom  -= rasst;

    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::Deflate(double rasst);
    //DESCRIPTION : Constricts rectangle by defined length.
    //INPUTS : double.
    //RETURNS : NONE.
    void Deflate(double rasst)
    {
        NormalizeGeoRect();
        left	+= rasst;
        right	-= rasst;
        top		-= rasst;
        bottom  += rasst;

    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::MoveX(double shift);
    //DESCRIPTION : Offset along O.
    //INPUTS : double.
    //RETURNS : NONE.
    void MoveX(double shift)
    {
        left   += shift;
        right  += shift;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::MoveY(double shift);
    //DESCRIPTION : Offset along Y.
    //INPUTS : double.
    //RETURNS : NONE.
    void MoveY(double shift)
    {
        top    += shift;
        bottom += shift;
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::Width();
    //DESCRIPTION : Calculates width.
    //INPUTS : NONE.
    //RETURNS : Double.
    double Width() const
    {
        return fabs(left - right);
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::Height();
    //DESCRIPTION : Calculates height.
    //INPUTS : NONE.
    //RETURNS : Double.
    double Height() const
    {
        return fabs(top - bottom);
    }

    //PACKAGE : MAP.
    //FUNCTION : MRect::CenterX();
    //DESCRIPTION : Calculates X-coordinate of centre.
    //INPUTS : NONE.
    //RETURNS : Double.
    double CenterX() const {return (left+right)/2.0;}

    //PACKAGE : MAP.
    //FUNCTION : MRect::CenterY();
    //DESCRIPTION : Calculates Y-coordinate of centre.
    //INPUTS : NONE.
    //RETURNS : Double.
    double CenterY() const {return (top+bottom)/2.0;}
};

//PACKAGE : MAP.
//CLASS	: MPrk.
//DESCRIPTION : Parameters of geosymbol drawing. Geosymbol sample is dotted components of classifier.
class MPrk

{
public:
    //  attributes...
    unsigned int align;	// < modus of alignment
    bool      visible;	// < flag of visual mapping
    bool      scaled;		// < flag of scaleable
    float    value;			// < size
    int       bkmode;		// < transparency
    M_LOG_FONT   logfont;		// < font
    unsigned char	  colorkod;     // < table code of text color
    unsigned char	  bkcolorkod;   // < table code of base color

public:
    //PACKAGE : MAP.
    //FUNCTION : MPrk::MPrk();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MPrk() {
        align       = W_TA_CENTER | W_TA_BASELINE;
        visible     = true;
        scaled      = true;
        value       = 0.5;
        bkmode      = UT_TRANSPARENT;
        memset(&logfont,0x0,sizeof(M_LOG_FONT));
        strcpy((char*)&(logfont.lfFaceName), "Arial");
        logfont.lfEscapement = 0;
        logfont.lfUnderline = 0;
        logfont.lfStrikeOut = 0;
        logfont.lfItalic = 0;
        logfont.lfWidth = 0;
        logfont.lfCharSet = 32;
        //logfont.lfCharSet = RUSSIAN_CHARSET;
        colorkod       = 0;
        bkcolorkod     = 0;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrk::~MPrk();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    virtual ~MPrk() {};

    //PACKAGE : MAP.
    //FUNCTION : MPrk::GetAngle();
    //DESCRIPTION : Gets font rotation angle.
    //INPUTS : NONE.
    //RETURNS : int.
    virtual int GetAngle() { return logfont.lfEscapement;}

    //PACKAGE : MAP.
    //FUNCTION : MPrk::SetAngle(int angle);
    //DESCRIPTION : Sets font rotation angle.
    //INPUTS : int.
    //RETURNS : NONE.
    virtual void SetAngle(int angle) { logfont.lfEscapement = angle;}

    //PACKAGE : MAP.
    //FUNCTION : MPrk::IsEqualTo(MPrk* prk);
    //DESCRIPTION : Function of comparison.
    //INPUTS : prk.
    //RETURNS : True.
    bool IsEqualTo(MPrk* prk) {
        if ((align	== prk->align)&&
                (visible== prk->visible)&&
                (scaled	== prk->scaled)&&
                (value	== prk->value)&&
                (bkmode	== prk->bkmode)&&
                (logfont.lfCharSet==prk->logfont.lfCharSet)&&
                //			(logfont.lfClipPrecision==prk->logfont.lfClipPrecision)&&
                (logfont.lfEscapement==prk->logfont.lfEscapement)&&
                //			(logfont.lfHeight==prk->logfont.lfHeight)&&
                (logfont.lfItalic==prk->logfont.lfItalic)&&
                //			(logfont.lfOrientation==prk->logfont.lfOrientation)&&
                //			(logfont.lfOutPrecision==prk->logfont.lfOutPrecision)&&
                //			(logfont.lfPitchAndFamily==prk->logfont.lfPitchAndFamily)&&

                //			(logfont.lfQuality==prk->logfont.lfQuality)&&
                (logfont.lfStrikeOut==prk->logfont.lfStrikeOut)&&
                (logfont.lfUnderline==prk->logfont.lfUnderline)&&
                //			(logfont.lfWeight==prk->logfont.lfWeight)&&
                (logfont.lfWidth==prk->logfont.lfWidth)&&
                (strcmp(logfont.lfFaceName,prk->logfont.lfFaceName)==0)&&
                (colorkod==prk->colorkod)&&
                (bkcolorkod==prk->bkcolorkod)) return true;
        else return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrk::Copy(MPrk* prk);
    //DESCRIPTION : Function of copy.
    //INPUTS : MPrk pointer.
    //RETURNS : NONE.
    void Copy(MPrk* prk) {
        align       = prk->align;
        visible     = prk->visible;
        scaled      = prk->scaled;
        value       = prk->value;
        bkmode      = prk->bkmode;
        memcpy(&logfont, &prk->logfont, sizeof(M_LOG_FONT));
        colorkod    = prk->colorkod;
        bkcolorkod  = prk->bkcolorkod;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrk::SetPrkParametersFor10000();
    //DESCRIPTION : Sets symbol parameters for the map with the scale factor of 10000:1
    //				for backgrounds and contours
    //				NB! Function not sets str and value !!.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void SetPrkParametersFor10000() {
        align       = UT_TA_CENTER | UT_TA_BASELINE;
        visible     = true;
        scaled      = true;
        bkmode      = UT_TRANSPARENT;
        strcpy((char*)&(logfont.lfFaceName), FONT_10000);
        logfont.lfWidth = 0;
        logfont.lfEscapement = 0;
        logfont.lfOrientation = 0;
        logfont.lfWeight = 400;
        logfont.lfItalic = 0;
        logfont.lfUnderline = 0;
        logfont.lfStrikeOut = 0;
        logfont.lfCharSet = 2;
        logfont.lfOutPrecision = 3;
        logfont.lfClipPrecision = 2;
        logfont.lfQuality = 1;
        logfont.lfPitchAndFamily = 2;
        colorkod       = 0;

        bkcolorkod     = 0;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrk::ReadPrk(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of reading to UTP-format file.
    //INPUTS : FILE pointer, int.
    //RETURNS : True.
    virtual bool ReadPrk(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPrk::ReadPrk(MFile* file, int version);
    //DESCRIPTION : Function of reading to UTP-format file.
    //INPUTS : MFile pointer, int.
    //RETURNS : True.
    virtual bool ReadPrk(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPrk::WritePrk(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of writing to UTP-format file.
    //INPUTS : FILE pointer, int.
    //RETURNS : True.
    virtual bool WritePrk(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPrk::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of reading to UTP-format file.
    //INPUTS : FILE pointer, int.
    //RETURNS : True.
    virtual bool ReadBin(FILE* h_nuxfile, int version){return ReadPrk(h_nuxfile,version);};

    //PACKAGE : MAP.
    //FUNCTION : MPrk::ReadBin(MFILE* file, int version);
    //DESCRIPTION : Function of reading to UTP-format file.
    //INPUTS : MFile pointer, int.
    //RETURNS : True.
    virtual bool ReadBin(MFile* file, int version) {return ReadPrk(file,version);};

    //PACKAGE : MAP.
    //FUNCTION : MPrk::WriteBin(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of writing to UTP-format file.
    //INPUTS : FILE pointer, int.
    //RETURNS : True.
    virtual bool WriteBin(FILE* h_nuxfile, int version){return WritePrk(h_nuxfile,version);};

    //PACKAGE : MAP.
    //FUNCTION : MPrk::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : int.
    virtual int GetBytes(){
        // Calculates size of handwrite
        // 2b - drawing parameters
        // 1b - stack of symbols
        // 8b - symbol size
        // 2b - size of table code colors
        // 32b -Font name
        return 41;// 41;
    };
};

//PACKAGE : MAP.
//CLASS	: MColor.
//DESCRIPTION :Replaces MFC COLORREF.
class MColor

{
public:
    unsigned char R;// < Red channel
    unsigned char G;// < Green channel
    unsigned char B;// < Blue channel
    unsigned char A;// < Alpha channel

    //PACKAGE : MAP.
    //FUNCTION : MColor::MColor();
    //DESCRIPTION : Constructor by default(BLACK).
    //INPUTS : NONE.
    //RETURNS : NONE.
    MColor(){R=G=B=0; A=255;};//by default(BLACK)

    //PACKAGE : MAP.
    //FUNCTION : MColor::MColor();
    //DESCRIPTION : Constructor with the established parameters.
    //INPUTS : unsigned char, unsigned char, unsigned char, unsigned char.
    //RETURNS : NONE.
    MColor(unsigned char r, unsigned char g, unsigned char b,   unsigned char a = 255)
    {
        R = r; G= g; B = b; A = a;
    }

    /*MColor( const MColor& col)
    {
        R = col.R; G= col.G; B = col.B; A = col.A;
    }*/

    //PACKAGE : MAP.
    //FUNCTION : MColor::MColor();
    //DESCRIPTION : Constructor.
    //INPUTS : unsigned int.
    //RETURNS : NONE.
    MColor(unsigned int i)
    {
        B = i % 256; i /= 256;
        G = i % 256; i /= 256;
        R = i % 256; i /= 256;
        A = i % 256;

        //        B = i%256;
        //        G = (i/256)%256;
        //        R= (i/(256*256))%256;
        //        A = i/(256*256*256);
    }

    //PACKAGE : MAP.
    //FUNCTION : MColor::~MColor();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MColor(){};

    //PACKAGE : MAP.
    //FUNCTION : MColor::GetA();
    //DESCRIPTION : Gets alpha-channel.
    //INPUTS : NONE.
    //RETURNS : unsigned char.
    unsigned char GetA() {return A;}

    //PACKAGE : MAP.
    //FUNCTION : MColor::GetR();
    //DESCRIPTION : Gets red color.
    //INPUTS : NONE.
    //RETURNS : unsigned char.
    unsigned char GetR() {return R;}

    //PACKAGE : MAP.
    //FUNCTION : MColor::GetG();
    //DESCRIPTION : Gets green color.
    //INPUTS : NONE.
    //RETURNS : unsigned char.
    unsigned char GetG() {return G;}

    //PACKAGE : MAP.
    //FUNCTION : MColor::GetB();
    //DESCRIPTION : Gets blue color.
    //INPUTS : NONE.
    //RETURNS : unsigned char.
    unsigned char GetB() {return B;}

    //PACKAGE : MAP.
    //FUNCTION : MColor::GetColor();
    //DESCRIPTION : Gets color value in unsigned int.
    //INPUTS : NONE.
    //RETURNS : unsigned int.
    unsigned int GetColor()
    {
       return ((unsigned int) (((((A << 8) + R) << 8) + G) << 8) + B);
        //return ((unsigned int) (((unsigned char) (B) | ((unsigned) (G) << 8)) | (((unsigned int) (unsigned char) (R)) << 16))) ;
    }
    //PACKAGE : MAP.
    //FUNCTION : MColor::etA(unsigned char Alfa);
    //DESCRIPTION : Sets alpha-channel.
    //INPUTS : unsigned char.
    //RETURNS : NONE.
    void SetA(unsigned char Alfa){A=Alfa;};

    //PACKAGE : MAP.
    //FUNCTION : MColor::SetR(unsigned char Red);
    //DESCRIPTION : Sets red color.
    //INPUTS : unsigned char.
    //RETURNS : NONE.
    void SetR(unsigned char Red){R=Red;};

    //PACKAGE : MAP.
    //FUNCTION : MColor::SetG(unsigned char Green);
    //DESCRIPTION : Sets green color.
    //INPUTS : unsigned char.
    //RETURNS : NONE.
    void SetG(unsigned char Green){G=Green;};

    //PACKAGE : MAP.
    //FUNCTION : MColor::SetB(unsigned char Blue);
    //DESCRIPTION : Sets blue color.
    //INPUTS : unsigned char.
    //RETURNS : NONE.
    void SetB(unsigned char Blue){B=Blue;};

    //PACKAGE : MAP.
    //FUNCTION : MColor::SetColor(MColor col);
    //DESCRIPTION : Sets new color values.
    //INPUTS : MColor.
    //RETURNS : NONE.
    void SetColor(MColor col)
    {
        R=col.R; G=col.G; B=col.B; A=col.A;
    };

    //PACKAGE : MAP.
    //FUNCTION : MColor::SetColor(MColor* col);
    //DESCRIPTION : Sets new color values.
    //INPUTS : MColor pointer.
    //RETURNS : NONE.
    void SetColor(MColor* col)
    {
        R=col->R; G=col->G; B=col->B; A=col->A;
    };

    //PACKAGE : MAP.
    //FUNCTION : MColor::SetColor(unsigned char r, unsigned char g, unsigned char b);
    //DESCRIPTION : Sets new color values.
    //INPUTS : unsigned char, unsigned char, unsigned char.
    //RETURNS : NONE.
    void SetColor(unsigned char r, unsigned char g, unsigned char b)
    {
        R=r; G=g; B=b;
    };

    //PACKAGE : MAP.
    //FUNCTION : MColor::operator == ;
    //DESCRIPTION : Operator of comparison (equality).
    //INPUTS : MColor.
    //RETURNS : True.
    bool operator ==( MColor color ){if((R==color.R)&&(G==color.G)&&(B==color.B)&&(A==color.A))return true;
        return false;};
    //PACKAGE : MAP.
    //FUNCTION : MColor::operator != ;
    //DESCRIPTION : Operator of comparison (inequality).
    //INPUTS : MColor.
    //RETURNS : True.
    bool operator !=( MColor color){return !(operator == (color));};

    // For Windows defines COLORREF to use
#ifdef _WINDOWS
    typedef long COLORREF;

    operator COLORREF()
    {
        union col
        {
            long colorref;
            char a;
            char b;
            char g;
            char r;
        };
        col t;
        t.a = 0; t.r = R; t.g = G; t.b = B;
        return t.colorref;
    }

    void operator =( COLORREF colorref)
    {
        union col
        {
            long colorref;
            char a;
            char b;
            char g;
            char r;
        };
        col t;
        t.colorref = colorref;
        A = t.a; R = t.r; G = t.g; B = t.b;
    };
#endif
};

//PACKAGE : MAP.
//CLASS	: MPrkExt : public MPrk.
//DESCRIPTION : Extended class to work with handwrite - used in a class MNadp.
class MPrkExt : public MPrk

{
public:
    MColor	  colorkodExt;     // < text color code
    MColor	  bkcolorkodExt;   // < base color code

public:

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::MPrkExt();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MPrkExt() {
        colorkodExt.SetColor(0,0,0);
        bkcolorkodExt.SetColor(255,255,255);
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::~MPrkExt();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MPrkExt() {};

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::IsEqualTo(MPrkExt* prk);
    //DESCRIPTION : Function of comparison.
    //INPUTS : MPrkExt pointer.
    //RETURNS : True.
    bool IsEqualTo(MPrkExt* prk) {
        if ((colorkodExt==prk->colorkodExt)&&
                (bkcolorkodExt==prk->bkcolorkodExt)&&
                (MPrk::IsEqualTo(prk))) return true;
        else return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::Copy(MPrkExt* prk);
    //DESCRIPTION :  Function of copy.
    //INPUTS : MPrkExt pointer.
    //RETURNS : True.
    void Copy(MPrkExt* prk) {
        colorkodExt = prk->colorkodExt;
        bkcolorkodExt = prk->bkcolorkodExt;
        MPrk::Copy(prk);
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::SetPrkParametersFor10000();
    //DESCRIPTION : Sets symbol parameters for the map with the scale factor of 10000:1
    //				for backgrounds and contours
    //				NB! Function not defines str and value !!.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void SetPrkParametersFor10000() {
        colorkodExt.SetColor(0,0,0);
        bkcolorkodExt.SetColor(0,0,0);
        MPrk::SetPrkParametersFor10000();
    }

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::ReadPrk(FILE* h_nuxfile, int version);
    //DESCRIPTION : Functions of reading in a UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool ReadPrk(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::ReadPrk(MFile* file, int version);
    //DESCRIPTION : Functions of reading in a UTP-format file.
    //INPUTS : MFile pointer, int .
    //RETURNS : True.
    virtual bool ReadPrk(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt:: WritePrk(FILE* h_nuxfile, int version);
    //DESCRIPTION : Functions of writing in a UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool WritePrk(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPrkExt::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : int.
    virtual int GetBytes(){
        // Calculates size of handwrite
        // 2b - drawing parameters
        // 1b - stack of symbols
        // 8b - symbol size
        // 2b - size of table code colors
        // 8b - adding color sizes
        // 32b -Font name
        return 53;
    };
};

//PACKAGE : MAP.
//CLASS	: MSymbol : public MPrk.
//DESCRIPTION : Geosymbol with drawing parameters.
class MSymbol : public MPrk

{
public:
    MString   str;      // > text, actually symbol

public:

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::MSymbol();
    //DESCRIPTION : Constructors.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MSymbol() {
        str         = "o";
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::~MSymbol();
    //DESCRIPTION : Destructors.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MSymbol() {};

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::IsEqualTo(MSymbol* sym);
    //DESCRIPTION : Function of comparison.
    //INPUTS : MSymbol pointer.
    //RETURNS : True.
    bool IsEqualTo(MSymbol* sym) {
        if ((str== sym->str)&&
                (MPrk::IsEqualTo(sym))) return true;
        else return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::IsEqualByPrkTo(MSymbol* sym);
    //DESCRIPTION :  Function of handwrites comparison.
    //INPUTS : MSymbol pointer.
    //RETURNS : True.
    bool IsEqualByPrkTo(MSymbol* sym) {
        return MPrk::IsEqualTo(sym);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::SetPrk(MPrk* m_pPrk);
    //DESCRIPTION : Sets handwrite.
    //INPUTS : MPrk pointer.
    //RETURNS : NONE.
    void SetPrk(MPrk* m_pPrk){
        MPrk::Copy(m_pPrk);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::Copy(MSymbol* sym);
    //DESCRIPTION : Function of copy.
    //INPUTS : MSymbol pointer.
    //RETURNS : NONE.
    void Copy(MSymbol* sym) {
        str = sym->str;
        MPrk::Copy(sym);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::CopyPrk(MSymbol* sym);
    //DESCRIPTION : Function of handwrite copy.
    //INPUTS : MSymbol pointer.
    //RETURNS : NONE.
    void CopyPrk(MSymbol* sym) {
        MPrk::Copy(sym);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::ReadSymbol(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Function of reading to the UTP-format file of geosymbol.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool ReadSymbol(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::ReadSymbol(MFile* file, int version);
    //DESCRIPTION :  Function of reading to the UTP-format file of geosymbol.
    //INPUTS : MFile pointer, int .
    //RETURNS : True.
    virtual bool ReadSymbol(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::WriteSymbol(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Function of writing to the UTP-format file of geosymbol.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool WriteSymbol(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::WriteBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Function of writing to the UTP-format file of geosymbol.
    //INPUTS : FILE pointer, int .
    //RETURNS :  WriteSymbol(h_nuxfile,version).
    virtual bool WriteBin(FILE* h_nuxfile, int version){return WriteSymbol(h_nuxfile,version);};

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Function of reading to the UTP-format file of geosymbol.
    //INPUTS : FILE pointer, int .
    //RETURNS : ReadSymbol(h_nuxfile,version).
    virtual bool ReadBin(FILE* h_nuxfile, int version){return ReadSymbol(h_nuxfile,version);};

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::ReadBin(MFile* file, int version);
    //DESCRIPTION :  Function of reading to the UTP-format file of geosymbol.
    //INPUTS : MFile pointer, int .
    //RETURNS : ReadSymbol(file,version).
    virtual bool ReadBin(MFile* file, int version){return ReadSymbol(file,version);};

    //PACKAGE : MAP.
    //FUNCTION : MSymbol::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    int GetBytes();
};


//PACKAGE : MAP.
//CLASS	: MSymbolExt : public MPrkExt.
//DESCRIPTION : Extended class of geosymbol, uses extended class for handwrite
class MSymbolExt : public MPrkExt

{
public:
    MString   str;      // > text, actually symbol

public:

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::MSymbolExt();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MSymbolExt() {
        str         = "o";
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::MSymbolExt();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MSymbolExt() {};

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::IsEqualTo(MSymbolExt* sym);
    //DESCRIPTION : Function of comparison.
    //INPUTS : MSymbolExt pointer.
    //RETURNS : True.
    bool IsEqualTo(MSymbolExt* sym) {
        if ((str== sym->str)&&
                (MPrkExt::IsEqualTo(sym))) return true;
        else return false;
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::IsEqualByPrkTo(MSymbolExt* sym);
    //DESCRIPTION : Function of handwrites comparison.
    //INPUTS : MSymbolExt pointer.
    //RETURNS : True.
    bool IsEqualByPrkTo(MSymbolExt* sym) {
        return MPrkExt::IsEqualTo(sym);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::SetPrk(MPrkExt* m_pPrk);
    //DESCRIPTION : Sets handwrite.
    //INPUTS : MPrkExt pointer.
    //RETURNS : NONE.
    void SetPrk(MPrkExt* m_pPrk){
        MPrkExt::Copy(m_pPrk);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt:: Copy(MSymbolExt* sym);
    //DESCRIPTION : Function of copy.
    //INPUTS : MSymbolExt pointer.
    //RETURNS : NONE.
    void Copy(MSymbolExt* sym) {
        str = sym->str;
        MPrkExt::Copy(sym);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::CopyPrk(MSymbolExt* sym);
    //DESCRIPTION : Function of handwrite copy.
    //INPUTS : MSymbolExt pointer.
    //RETURNS : NONE.
    void CopyPrk(MSymbolExt* sym) {
        MPrkExt::Copy(sym);
    }

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::ReadSymbol(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of reading to the UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool ReadSymbol(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::ReadSymbol(MFile* file, int version);
    //DESCRIPTION : Function of reading to the UTP-format file.
    //INPUTS : MFile pointer, int .
    //RETURNS : True.
    virtual bool ReadSymbol(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::WriteSymbol(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of writing to the UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool WriteSymbol(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::WriteBin(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of writing to the UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool WriteBin(FILE* h_nuxfile, int version){return WriteSymbol(h_nuxfile,version);};

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION : Function of reading to the UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    virtual bool ReadBin(FILE* h_nuxfile, int version){return ReadSymbol(h_nuxfile,version);};

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::ReadBin(MFile* file, int version);
    //DESCRIPTION : Function of reading to the UTP-format file.
    //INPUTS : MFile pointer, int .
    //RETURNS : True.
    virtual bool ReadBin(MFile* file, int version){return ReadSymbol(file,version);};

    //PACKAGE : MAP.
    //FUNCTION : MSymbolExt::GetBytes();
    //DESCRIPTION :  Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    int GetBytes();
};


/*/=====================================================================
//  Класс MAlgoritm описывает алгоритмическую линии
class MAlgoritm
//Цурко Д.С.
//=====================================================================
{
public:
    double fatness; //толщина линии
    MColor color  ; //цвет линии
    MAcArray* actions; //массив действий
//конструкторы
    MAlgoritm();
    MAlgoritm(double fs, MColor* cl, MAcArray* act=NULL);
//функция копирования
    void Copy(MAlgoritm* p_Alg);
//деструктор
    ~MAlgoritm();
}
*/

//PACKAGE : MAP.
//CLASS	: MPredlogenie.
//DESCRIPTION : Class sentence - basic structure of formulas.
class MPredlogenie

{
private:
    unsigned char	m_nomprefix;	// < code from suffix&preffix strings dictionary

    unsigned short  m_nompolename;	// < Sense of sentence
    // - name of one of data field
    unsigned char	m_nompostfix;	// < code from suffix&preffix strings dictionary

    unsigned char	m_nomprk;		// < code of sentence handwrite

public:

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::MPredlogenie();
    //DESCRIPTION :  Constructor by default.
    //INPUTS : NONE.
    //RETURNS : NONE.

    MPredlogenie():m_nomprefix(0),m_nompolename(0),
        m_nompostfix(0),m_nomprk(0){};

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::MPredlogenie(unsigned short nompolename, unsigned char nomprefix=UCHAR_MAX, unsigned char nompostfix=UCHAR_MAX);
    //DESCRIPTION :  Constructors with preffix and postfix.
    //INPUTS : unsigned short, unsigned char, unsigned char.
    //RETURNS : NONE.
    MPredlogenie(unsigned short nompolename,
                 unsigned char nomprefix=UCHAR_MAX,
                 unsigned char nompostfix=UCHAR_MAX);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::MPredlogenie(unsigned char nomprefix, unsigned short nompolename, unsigned char nompostfix, unsigned char nomprk);
    //DESCRIPTION :  Constructors with preffix and postfix.
    //INPUTS : unsigned char, unsigned short, unsigned char, unsigned char.
    //RETURNS : NONE.
    MPredlogenie(unsigned char nomprefix, unsigned short nompolename,
                 unsigned char nompostfix, unsigned char nomprk);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::~MPredlogenie();
    //DESCRIPTION :  Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MPredlogenie(){};

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::Copy(MPredlogenie* predlog);
    //DESCRIPTION :  Copies.
    //INPUTS : MPredlogenie pointer.
    //RETURNS : NONE.
    void Copy(MPredlogenie* predlog);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::WriteBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Saves to UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    bool WriteBin(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Reads from UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    bool ReadBin(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::ReadBin(MFile* file, int version);
    //DESCRIPTION :  Reads from UTP-format file.
    //INPUTS : MFile pointer, int .
    //RETURNS : True.
    bool ReadBin(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::Clone();
    //DESCRIPTION :  Makes a clone.
    //INPUTS : NONE.
    //RETURNS : MPredlogenie pointer.
    MPredlogenie* Clone();

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::GetBytes();
    //DESCRIPTION :  Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::GetNomPrk();
    //DESCRIPTION :  Function of getting dictionary code of sentence handwrite.
    //INPUTS : NONE.
    //RETURNS :  unsigned char.
    unsigned char GetNomPrk();

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::SetNomPrk(unsigned char nomprk);
    //DESCRIPTION :  Function of setting dictionary code of sentence handwrite.
    //INPUTS : unsigned char .
    //RETURNS : True.
    bool  SetNomPrk(unsigned char nomprk);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::GetNomPole();
    //DESCRIPTION :  Functions of getting dictionary code of sentence sense.
    //INPUTS : NONE.
    //RETURNS : unsigned short.
    unsigned short GetNomPole();

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::SetNomPole(unsigned short nompole);
    //DESCRIPTION :  Functions of setting dictionary code of sentence sense.
    //INPUTS : unsigned short.
    //RETURNS : True.
    bool  SetNomPole(unsigned short nompole);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::GetNomPrefix();
    //DESCRIPTION :  Functions of getting preffix dictionary code.
    //INPUTS : NONE.
    //RETURNS : unsigned char.
    unsigned char GetNomPrefix();

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::SetNomPrefix(unsigned char nomprefix);
    //DESCRIPTION :  Functions of setting preffix dictionary code.
    //INPUTS : unsigned char .
    //RETURNS : True.
    bool  SetNomPrefix(unsigned char nomprefix);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::GetNomPostfix();
    //DESCRIPTION :  Functions of getting postfix dictionary code.
    //INPUTS : NONE.
    //RETURNS : unsigned char.
    unsigned char GetNomPostfix();

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::SetNomPostfix(unsigned char nompostfix);
    //DESCRIPTION :  Functions of setting postfix dictionary code.
    //INPUTS : unsigned char .
    //RETURNS : True.
    bool  SetNomPostfix(unsigned char nompostfix);

    //PACKAGE : MAP.
    //FUNCTION : MPredlogenie::IsEqualTo(MPredlogenie* predlog);
    //DESCRIPTION :  Sentence comparison.
    //INPUTS : MPredlogenie pointer.
    //RETURNS : True.
    bool  IsEqualTo(MPredlogenie* predlog);
};

//PACKAGE : MAP.
//CLASS	: MNomValue.
//DESCRIPTION : Class number-value - stores numberic value and	value of characteristic.
//		Destined for dictionary characteristic name relation. 
//		Stored in classifier, with the value of the same characteristic for object.
class MNomValue

{
private:
    unsigned short	m_nomer;	// < Index(code) of characteristic name
    MString			m_value;	// < Characteristic value

public:

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::MNomValue();
    //DESCRIPTION :  Constructor by default.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MNomValue();

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::MNomValue(unsigned short nomer, MString value);
    //DESCRIPTION :  Constructor with established values.
    //INPUTS : unsigned short , MString .
    //RETURNS : NONE.
    MNomValue(unsigned short nomer, MString value);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::~MNomValue();
    //DESCRIPTION :  Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MNomValue(){};

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::Copy(MNomValue* kodval);
    //DESCRIPTION :  Copies.
    //INPUTS : MNomValue pointer.
    //RETURNS : NONE.
    void Copy(MNomValue* kodval);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::WriteBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Saves to UTP-format file.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    bool WriteBin(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Reads from file of UTP-format.
    //INPUTS : FILE pointer, int .
    //RETURNS : True.
    bool ReadBin(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::ReadBin(MFile* file, int version);
    //DESCRIPTION :  Reads from file of UTP-format.
    //INPUTS : MFile pointer, int .
    //RETURNS : True.
    bool ReadBin(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::Clone();
    //DESCRIPTION :  Makes a clone.
    //INPUTS : NONE.
    //RETURNS : MNomValue pointer.
    MNomValue* Clone();

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::GetBytes();
    //DESCRIPTION :  Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::GetNom();
    //DESCRIPTION :  Functions of getting dictionary entry number.
    //INPUTS : NONE.
    //RETURNS : unsigned short.
    unsigned short GetNom() const;

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::SetNom(unsigned short nomer);
    //DESCRIPTION :  Functions of setting dictionary entry number.
    //INPUTS : unsigned short .
    //RETURNS : True.
    bool  SetNom(unsigned short nomer);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::GetValue();
    //DESCRIPTION :  Functions of getting characteristic value.
    //INPUTS : NONE.
    //RETURNS : MString.
    MString GetValue() const;

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::SetValue(MString value);
    //DESCRIPTION :  Functions of establishing characteristic value.
    //INPUTS : MString .
    //RETURNS : True.
    bool  SetValue(MString value);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::SetNomValue(unsigned short nomer, MString value);
    //DESCRIPTION :  Sets dictionary number and characteristic value.
    //INPUTS : unsigned short , MString .
    //RETURNS : True.
    bool  SetNomValue(unsigned short nomer, MString value);

    //PACKAGE : MAP.
    //FUNCTION : MNomValue::IsEqualTo(MNomValue* nomval);
    //DESCRIPTION :  Sets dictionary number and characteristic value.
    //INPUTS : MNomValue pointer.
    //RETURNS : True.
    bool  IsEqualTo(MNomValue* nomval);
};

/// Structure describes a logical feather replaces MFC-structure LOGPEN
struct M_LOG_PEN {
    unsigned int	lopnStyle;// style (solid, dotted etc.)
    MPoint			lopnWidth;// width
    MColor			lopnColor;// color

    M_LOG_PEN(unsigned int uiStyle,MPoint ptWidth,MColor clr):lopnStyle(uiStyle),
        lopnWidth(ptWidth),lopnColor(clr){};
    M_LOG_PEN(){};
};

#endif//__NUX_BASICS_H__
