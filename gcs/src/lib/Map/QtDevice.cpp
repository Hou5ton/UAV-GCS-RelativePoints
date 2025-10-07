// QtDevice.cpp: implementation of the MQtDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "QtDevice.h"
#include "Refglobal.h"
#include <qdatetime.h>
#include <qimage.h>
#include "Karta.h"
#include "PatternConvertor.h"
#include <QPainterPath>

int drawCount;
int drawTime;
// ==================================================================
MQtDevice::MQtDevice(QPainter* pDev)
// ==================================================================
{
    m_pRealPD = (QPaintDevice*)pDev;
    m_pOutputPD = (QPaintDevice*)pDev;
    m_painter = pDev;//new QPainter((QPaintDevice*)pDev);
    width = abs((pDev->window()).right());
    height = abs((pDev->window()).bottom());
    //	m_painter->begin((QPaintDevice*)pDev);
    pPen = NULL;//new QPen;
    pBrush = NULL;//new QBrush;
    pFont = NULL;//new QFont("Courier", 10);
    pTempPen = new QPen(Qt::NoPen);
    pTempBrush = new QBrush;
    pTempFont = new QFont("Courier", 10);

    //	m_show_text_limit = 6;   // vipa 2004.06.25
    //	m_show_text_limit = 4;   // vipa 2004.06.25

    m_angle = 0;

    m_align = UT_TA_CENTER | UT_TA_BASELINE;
    codec = QTextCodec::codecForName(mapCharset);
    //	QRegion rgn(0, 0, width, height);
    //	m_painter->setClipRegion(rgn);
}

MQtDevice::~MQtDevice()
{
    //	if(m_painter)
    //	{
    //		 m_painter->end();
    //		 delete m_painter;
    //	}
    ClearPool();
    ClearClip();

    ChangePen(NULL, false);
    ChangeBrush(NULL, false);
    ChangeFont(NULL, false);

    if(pTempPen) delete pTempPen;
    pTempPen = NULL;
    if(pTempBrush) delete pTempBrush;
    pTempBrush = NULL;
    if(pTempFont) delete pTempFont;
    pTempFont = NULL;
}

//==============================================================
void MQtDevice::BumToLogicI(MPoint *ptin, QPoint& ptout)
//==============================================================
{
    /*static MPoint temp; - works incorrect when ONE QTDevice is used by many threads*/
    MPoint temp;
    BumToLogic(ptin,temp);
    ptout.setX(static_cast<int>(temp.x));
    ptout.setY(static_cast<int>(temp.y));
}

//==============================================================
void MQtDevice::BumToLogicI(MPoint* ptina, QPolygon &ptouta,int iNum)
//==============================================================
{
    auto var_1 = width / 2 + B1;
    auto var_2 = height / 2 - B2;

    for(int i = 0;i<iNum;i++)
    {
        ptouta.setPoint(i, static_cast<int>(std::round(var_1 + A*ptina[i].x)),
                        static_cast<int>(std::round(var_2 - A*ptina[i].y)));
    }

    /*	MPoint* pTemp = (MPoint*)malloc(sizeof(MPoint)*iNum);
    BumToLogic(ptina,pTemp,iNum);
    QPoint pt;
    for(int i = 0;i<iNum;i++){
        pt.setX((int)dbl_to_int(pTemp[i].x));
        pt.setY((int)dbl_to_int(pTemp[i].y));
        ptouta.setPoint(i, pt);
    }
    free(pTemp);*/
}

//==============================================================
void MQtDevice::BumToLogicI(MRect* pRectIn, QRect& pRectOut)
//==============================================================
{
    MRect rTemp;
    BumToLogic(pRectIn,rTemp);
    pRectOut.setTop(static_cast<int>(rTemp.top));
    pRectOut.setBottom(static_cast<int>(rTemp.bottom));
    pRectOut.setLeft(static_cast<int>(rTemp.left));
    pRectOut.setRight(static_cast<int>(rTemp.right));
}

//===========================================================
//Initialization of object
void MQtDevice::Initialize()
//===========================================================
{
    /*	// new context new bitmap...
    m_pixmap.CreateCompatibleDC(m_pRealDC);
    MRect clientRect = GetLogRect();

    //Initialization of devices
    InternalPrepareDC(&m_dc);
    InternalPrepareDC(m_pRealDC);*/
};

// ==================================================================
void MQtDevice::Actualize(bool /*bToOriginal*/)
// ==================================================================
{
    ///	MRect mrect = GetLogRect();
    // we draw a result


    /*	if(bToOriginal)
    {
        //bitBlt(m_pRealPD, (int)mrect.left, (int)mrect.bottom, m_pixmap, (int)mrect.left, (int)mrect.bottom, (int)mrect.Width(), (int)mrect.Height(), Qt::CopyROP, true);
        m_painter->drawPixmap((int)mrect.left, (int)mrect.bottom, m_pixmap, (int)mrect.left, (int)mrect.bottom, (int)mrect.Width(), (int)mrect.Height());
    }
        //m_pRealPD->BitBlt((int)mrect.left, (int)mrect.bottom, (int)mrect.Width(), (int)mrect.Height(),
        //	&m_dc, (int)mrect.left, (int)mrect.bottom, SRCCOPY);
    else
    {
        bitBlt(&m_pixmap, (int)mrect.left, (int)mrect.bottom, m_pRealPD, (int)mrect.left, (int)mrect.bottom, (int)mrect.Width(), (int)mrect.Height(), Qt::CopyROP, true);
    }
        //m_pixmap.BitBlt((int)mrect.left, (int)mrect.bottom, (int)mrect.Width(), (int)mrect.Height(),
        //	m_pRealDC, (int)mrect.left, (int)mrect.bottom, SRCCOPY);
*/}

// ==================================================================
bool MQtDevice::AddClipRgn(MPoint* ptaIn,int iNumPoints,RGN_TYPE type/*= ADD_REGION*/)
// ==================================================================
{
    if(!m_painter) return false;
    if(type == ADD_REGION)
        LogClip(ptaIn,iNumPoints,PLUS);
    else if(type == INTERSECT_REGION)
        LogClip(ptaIn,iNumPoints,XORING);
    else if(type == CUT_REGION)
        LogClip(ptaIn,iNumPoints,NOT);

    QPolygon pointArr(iNumPoints);
    BumToLogicI(ptaIn, pointArr, iNumPoints);

    if(!fillingPolygon.size())
    {
        fillingPolygon = pointArr;
    }
    else
        switch(type)
        {
        case CUT_REGION:
            fillingPolygon = fillingPolygon.subtracted(pointArr);
            break;

        case ADD_REGION:
            fillingPolygon = fillingPolygon.united(pointArr);
            break;

        case  INTERSECT_REGION:
            fillingPolygon = fillingPolygon.intersected(pointArr);
            break;
        }
    return true;
}

void MQtDevice::CreateClipRgn(MPoint* ptaIn,int iNumPoints)
{    
    QPolygon polygon(iNumPoints);
    BumToLogicI(ptaIn, polygon, iNumPoints);

    //painterPath = QPainterPath();
    //painterPath.addPolygon(polygon);

    fillingPolygon = polygon;

}
// ==================================================================
bool MQtDevice::AddClipRgn(MRect* pRect,RGN_TYPE type /*= ADD_REGION*/)
// ==================================================================
{
    MPoint ptar[4];
    ptar[0] = MPoint(pRect->left,pRect->top);
    ptar[1] = MPoint(pRect->right,pRect->top);
    ptar[2] = MPoint(pRect->right,pRect->bottom);
    ptar[3] = MPoint(pRect->left,pRect->bottom);

    return AddClipRgn(ptar,4,type);
}

// ==================================================================
bool MQtDevice::BeginClipRgn()
// ==================================================================
{
    return MPlotDevice::BeginClipRgn();
};

// ==================================================================
bool MQtDevice::ClearClip()
// ==================================================================
{
    if(!m_painter) return false;
    //	QRegion rgn(0, 0, 0, 0);
    //  m_painter->setClipRegion(rgn);

    return MPlotDevice::ClearClip();
};

// ==================================================================
// Creates a font and brings it in a pool of fonts
bool MQtDevice::AddFontToPool(M_LOG_FONT* lfont,int iIndex)
// ==================================================================
{
    //Check that this index of a font wasn't brought in a pool earlier
    if(font_par_pool.find(iIndex)!=font_par_pool.end())   return false;      //exit(15);
    font_par_pool.insert(FONT_PAR_POOL_TYPE::value_type(iIndex,*lfont));
    //We create a font
    QString str;//(lfont->lfFaceName);
    str = (char *)lfont->lfFaceName;
    QFont *pfont = new QFont(str, (int)((int)BumToLogicI(lfont->lfHeight)*SCREEN_FONT_KOEFF));//, lfont->lfWeight, lfont->lfItalic);
    //	QFont *pfont = new QFont(str, ((int)20*BumToLogicI(lfont->lfHeight)*SCREEN_FONT_KOEFF)); // vipa 2004.06.25
    pfont->setItalic(lfont->lfItalic);
    if(lfont->lfWeight == FW_BOLD) pfont->setBold(true);
    else pfont->setBold(false);

    font_pool.insert(FONT_POOL_TYPE::value_type(iIndex,pfont));
    return true;
};

// ==================================================================
bool MQtDevice::SelectFontFromPool(int iIndex)
// ==================================================================
{
    FONT_POOL_TYPE::iterator FI;
    FI = font_pool.find(iIndex);
    if(FI!=font_pool.end()){
        //The font is found it is only necessary to choose it
        ChangeFont(((*FI).second),false);
        m_bSelectedFromPool = true;
    }
    else{
        //If the font wasn't created in a pool, it is created independently
        //If index incorrect
        FONT_PAR_POOL_TYPE::iterator FPI;
        FPI = font_par_pool.find(iIndex);
        if(FPI==font_par_pool.end())
            return false;
        SetFont(&(*FPI).second);
    }
    return true;
};

// ==================================================================
bool MQtDevice::ClearPool()
// ==================================================================
{
    //Delete fonts

    for (const auto& kv : font_pool) {
        delete kv.second;
    }

    font_pool.clear();
    font_par_pool.clear();
    return true;
};

// ==================================================================
bool MQtDevice::SelectGDI()
// ==================================================================
{
    if(pPen)
    {
        *pTempPen = m_painter->pen();
        m_painter->setPen(*pPen);
    }
    else
        m_painter->setPen(Qt::NoPen);
    if(pBrush)
    {
        *pTempBrush = m_painter->brush();
        m_painter->setBrush(*pBrush);
    }
    if(pFont)
    {
        *pTempFont = m_painter->font();
        if (drawTextFlag)  {

            double dVal = LogicToBum((double)textSizeMax);
            double dMax = BumToLogicI(dVal);
            if (drawActFlag) dMax= actRatio * dMax;

            float size  = textRatio * pFont->pointSize();
            if (size > dMax)   size = dMax;
            pFont->setPointSize((int)size);  //
            //      if (drawActFlag) cout << "pointSize= "<< (int)size<< endl;
            m_painter->setFont(*pFont);
        }
        drawTextFlag = false;

        if ( ! drawActFlag && (pFont->pointSize()< (int)distinguishTextSize) )
            return false;
        if (   drawActFlag && (pFont->pointSize()< 3) )
            return false;

        m_painter->setFont(*pFont);
    }
    return true;
};

// ==================================================================
void MQtDevice::DeselectGDI()
// ==================================================================
{
    if(pPen)
        if(pTempPen) m_painter->setPen(*pTempPen);
    if(pFont)
        if(pTempFont) m_painter->setFont(*pTempFont);
    if(pBrush)
        if(pTempBrush) m_painter->setBrush(*pTempBrush);
    drawTextFlag = false;
};

// ==================================================================
//Drawing of the polyline
bool MQtDevice::Polyline(MPoint* pPoints, int iNumPoints)
// ==================================================================
{
    //if (iNumPoints<= 1)  return false;
    QPolygon pLogPoints(iNumPoints);
    BumToLogicI(pPoints,pLogPoints,iNumPoints);
    //Normalize(pLogPoints);   //Comment by LenaLa 19.09.2005

    //  if ( ! Intersects(pLogPoints) ) return false;
    SelectGDI();
    //if (pLogPoints.count() <= 1 ) cout <<"NULL-line"<<endl;
    //	QTime time;  time.start(); //Counting Draw-time

    m_painter->drawPolyline(pLogPoints);

    //Counting Draw-time
    //	::drawTime+= time.restart();
    //	::drawCount++;

    DeselectGDI();
    return true;

}

// ==================================================================
//Drawing of the polygone
bool MQtDevice::Polygon(MPoint* pPoints, int iNumPoints)
{
    QPolygon pLogPoints(iNumPoints);
    BumToLogicI(pPoints,pLogPoints,iNumPoints);
    //Normalize(pLogPoints);    //  2005.01.20 vipa
    SelectGDI();

    QTime time;  time.start();

    /*	if (pLogPoints.count() == 1 )  // cout <<"NULL-line"<<endl;
    {
            pDevice->Circle(pta->GetHead(), pDevice->LogicToBum(1.5), m_pLay->m_commoncolor_pen);
          Circle(pPoints[0], 0.1, MColor(0,0,0));
    }
*/
    if(m_bClipRegionSelected)
    {
        QPainterPath path;
        path.addPolygon(fillingPolygon);
        m_painter->fillPath(path, m_painter->brush());
    }
    else
        if (drawActFlag) { // transparent mode

            qreal op_backup = m_painter->opacity();
            if(actPolygonDense==1 || actPolygonDense==0)
                m_painter->setOpacity(actPolygonDense);
            else m_painter->setOpacity((qreal)actPolygonDense/10);
            m_painter->drawPolygon(pLogPoints);
            m_painter->setOpacity(op_backup);
        }
        else  // opaque mode
            m_painter->drawPolygon(pLogPoints);


    ::drawTime+= time.restart();
    ::drawCount++;

    DeselectGDI();

    m_painter->setClipping (false);


    return true;
}

bool MQtDevice::DrawImage(MImage *image, MColor color, MPoint leftTop)
{
    if (!image) return false;

    const QColor qcolor(color.GetR(), color.GetG(), color.GetB(), color.GetA());

    QImage img;
    PatternConvertor::Convert(image, img);

    QPoint leftTopLogic;

    BumToLogicI(&leftTop, leftTopLogic);

    if ((m_align & UT_TA_CENTER) == UT_TA_CENTER)
    {
        leftTopLogic.setX(leftTopLogic.x() - image->width() / 2);
    }
    else
    {
        if ((m_align & UT_TA_RIGHT) == UT_TA_RIGHT)
        {
            leftTopLogic.setX(leftTopLogic.x() - image->width());
        }
        else
            if ((m_align & UT_TA_LEFT) == UT_TA_LEFT)
            {
                //Empty
            }
    }

    if ((m_align & UT_TA_BASELINE) == UT_TA_BASELINE)
    {
        leftTopLogic.setY(leftTopLogic.y() - image->height() / 2);
    }
    else
    {
        if ((m_align & UT_TA_BOTTOM) == UT_TA_BOTTOM)
        {
            leftTopLogic.setY(leftTopLogic.y() - image->height());
        }
        else
            if ((m_align & UT_TA_TOP) == UT_TA_TOP)
            {
                //Empty
            }
    }

    if ((img.format() == QImage::Format_Mono) || (img.format() == QImage::Format_MonoLSB))
    {
        QPixmap pix(img.width(), img.height());
        pix.fill(qcolor);
        QBitmap bmp;
        bmp.convertFromImage(img);
        pix.setMask(bmp);
        m_painter->drawPixmap(leftTopLogic, pix);
    }
    else
    {
        m_painter->drawImage(leftTopLogic, img);
    }

    return true;
}

bool MQtDevice::SetZapolnenie(MImage *image, MColor color)
{
    const QColor qcolor(color.GetR(), color.GetG(), color.GetB(), color.GetA());
    QBrush* brush = new QBrush(qcolor);

    QImage img;
    PatternConvertor::Convert(image, img);

    if ((img.format() == QImage::Format_Mono) || (img.format() == QImage::Format_MonoLSB))
    {
        QBitmap bmp;
        bmp.convertFromImage(img);
        brush->setTexture(bmp);
    }
    else
        brush->setTextureImage(img);

    ChangeBrush(brush, true);
    return true;
}

// ==================================================================
//Installation of a brush of filling
bool MQtDevice::SetZapolnenie(MColor* const pColor)
// ==================================================================
{
    const QColor color(pColor->GetR(), pColor->GetG(), pColor->GetB(), pColor->GetA());
    QBrush* brush = new QBrush(color);
    ChangeBrush(brush, true);
    return true;
}
// ==================================================================	
//Installation of a pencil of drawing
bool MQtDevice::SetPencil(M_LOG_PEN* pLPen)
// ==================================================================
{
    const QColor color(pLPen->lopnColor.GetR(), pLPen->lopnColor.GetG(), pLPen->lopnColor.GetB(), pLPen->lopnColor.GetA());
    QPen* pen = new QPen(color);
    pen->setWidth(BumToLogicI(pLPen->lopnWidth.x));
    if ( pLPen->lopnStyle == PS_DOTGRID )  // drawing Grid only //vipa -ATTENTION-
        pen->setStyle(Qt::DotLine); //vipa -ATTENTION-
    ChangePen(pen, true);
    return true;
}

// ==================================================================
//Installation of a pencil of drawing with a specific curve
bool MQtDevice::SetPencil(int /*iPenStyle*/,double dWidth,MColor* pColor)
// ==================================================================
{
    int iPenWidth = BumToLogicI(dWidth);
    QColor color(pColor->GetR(), pColor->GetG(), pColor->GetB(), pColor->GetA());
    QPen* pen = new QPen(color, iPenWidth, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    ChangePen(pen, true);
    return true;
}

// ==================================================================
//Installation of a font
bool MQtDevice::SetFont(M_LOG_FONT* lFont)
// ==================================================================
{
    QFont* font = new QFont;

    m_angle = (3600+lFont->lfEscapement)/10;
    if(m_angle%360 ==  0) m_angle = 0;

    QString family = (char *)lFont->lfFaceName;
    font->setFamily(family);

    font->setItalic(lFont->lfItalic);

    if(lFont->lfWeight == FW_BOLD) font->setBold(true);
    else font->setBold(false);

    //int pointsize = int(10*BumToLogicI(lFont->lfHeight)*SCREEN_FONT_KOEFF);
    int pointsize = int(BumToLogicI(lFont->lfHeight)*SCREEN_FONT_KOEFF);//tempMax
    if (pointsize < 1)           pointsize = 1;
    font->setPointSize(pointsize);

    ChangeFont(font, true);
    m_bSelectedFromPool = false;
    return true;
}

// ==================================================================
//Installation of a mode of alignment of the text
unsigned int MQtDevice::SetTextAlign(unsigned int uiTextAlign)
// ==================================================================
{
    unsigned int old = m_align;
    m_align = uiTextAlign;
    return old;
}

// ==================================================================
//Installation of color of the text
MColor MQtDevice::SetTextColor(MColor crTextColor)
// ==================================================================
{
    MColor oldColor = font_color;

    font_color = crTextColor;

    return oldColor;
}

// ==================================================================
//Installation of color of a background
MColor MQtDevice::SetBkColor(MColor crBkColor)
// ==================================================================
{
    const QColor color(crBkColor.GetR(), crBkColor.GetG(), crBkColor.GetB(), crBkColor.GetA());
    const QColor oldColor = m_painter->background().color();
    m_painter->setBackground(QBrush(color));
    return MColor(oldColor.red(), oldColor.green(), oldColor.blue(), oldColor.alpha());
}

// ==================================================================
//Installation of the mode of a background
int MQtDevice::SetBkMode(int nBkMode)
// ==================================================================
{
    int old = m_painter->backgroundMode();

    if(nBkMode == UT_TRANSPARENT)
        m_painter->setBackgroundMode(Qt::TransparentMode);
    else
        m_painter->setBackgroundMode(Qt::OpaqueMode);

    return old;
}

// ==================================================================
//! justifies the text
QPoint MQtDevice::AlignText(QPoint& priv_pt, QString& str)
// ==================================================================
{
    //QPoint new_pt = priv_pt;
    //if(!pFont) return new_pt;
    //if(str.length() > 1) return new_pt;

    //QFontMetrics metrics(*pFont);
    ////we will receive the describing inscription rectangle
    //QRect boundingRect = metrics.boundingRect(str);
    ////we will displace a rectangle concerning a binding point
    ///*boundingRect.setLeft(boundingRect.left()+priv_pt.x());
    //boundingRect.setRight(boundingRect.right()+priv_pt.x());
    //boundingRect.setTop(boundingRect.top()+priv_pt.y());
    //boundingRect.setBottom(boundingRect.bottom()+priv_pt.y());*/
    //////we will expand a rectangle twice
    ////boundingRect.setLeft(boundingRect.left()-boundingRect.width());
    ////boundingRect.setBottom(boundingRect.bottom()+boundingRect.height());
    //
    ////we will consider shift
    //if ((m_align & UT_TA_CENTER) == UT_TA_CENTER)
    //{
    //	new_pt.setX(new_pt.x()-boundingRect.width()/2);
    //}
    //else
    //	if ((m_align & UT_TA_RIGHT) == UT_TA_RIGHT)
    //	{
    //		new_pt.setX(new_pt.x()-boundingRect.width());
    //	}
    //	else
    //		if ((m_align & UT_TA_LEFT) == UT_TA_LEFT)
    //		{
    //			;//new_pt.setX(boundingRect.left());
    //		}
    //
    //if ((m_align & UT_TA_BASELINE) == UT_TA_BASELINE)
    //{}
    //else
    //	if ((m_align & UT_TA_BOTTOM) == UT_TA_BOTTOM)
    //	{
    //		new_pt.setY(new_pt.y() - metrics.descent());
    //	}
    //	else
    //		if ((m_align & UT_TA_TOP) == UT_TA_TOP)
    //		{
    //			new_pt.setY(new_pt.y() + metrics.ascent());
    //		}

    //return new_pt;

    QPoint new_pt = priv_pt;
    if(!pFont) return new_pt;
    if(str.length() > 1) return new_pt;

    QFontMetrics metrics(*pFont);
    //we will receive the describing inscription rectangle
    QRect boundingRect = metrics.boundingRect(str);
    //we will displace a rectangle concerning a binding point
    boundingRect.setLeft(boundingRect.left()+priv_pt.x());
    boundingRect.setRight(boundingRect.right()+priv_pt.x());
    boundingRect.setTop(boundingRect.top()+priv_pt.y());
    boundingRect.setBottom(boundingRect.bottom()+priv_pt.y());
    //we will expand a rectangle twice
    boundingRect.setLeft(boundingRect.left()-boundingRect.width());
    //boundingRect.setBottom(boundingRect.bottom()+boundingRect.height());

    //we will consider shift
    if(((m_align & UT_TA_CENTER) == UT_TA_CENTER)||((m_align & W_TA_CENTER) == W_TA_CENTER))
    {
        new_pt.setX(boundingRect.left()+boundingRect.width()/4);
    }
    else
        if(((m_align & UT_TA_RIGHT) == UT_TA_RIGHT)||((m_align & W_TA_RIGHT) == W_TA_RIGHT))
        {
            new_pt.setX(boundingRect.left());
        }
        else
            if(((m_align & UT_TA_LEFT) == UT_TA_LEFT)||((m_align & W_TA_LEFT) == W_TA_LEFT))
            {
                new_pt.setX(boundingRect.left()+boundingRect.width()/2);
            }

    if(((m_align & UT_TA_BASELINE) == UT_TA_BASELINE)||((m_align & W_TA_BASELINE) == W_TA_BASELINE))
    {

    }
    else
        if(((m_align & UT_TA_BOTTOM) == UT_TA_BOTTOM)||((m_align & W_TA_BOTTOM) == W_TA_BOTTOM))
        {
            new_pt.setY(new_pt.y() - metrics.descent());
        }
        else
            if(((m_align & UT_TA_TOP) == UT_TA_TOP)||((m_align & W_TA_TOP) == W_TA_TOP))
            {
                new_pt.setY(new_pt.y() + metrics.ascent());
            }

    return new_pt;
}

// ==================================================================
//Text conclusion to the device
bool MQtDevice::TextOut(double dX,double dY,char * lpszString,int /*nCount*//*, bool bAligned*/)
// ==================================================================
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    QPoint qpt,qptC;
    MPoint pnt(dX,dY);
    BumToLogicI(&pnt,qpt);
    Normalize(qpt);     // vipa 2005.01.20
    qptC = qpt;

    //QTextCodec* codec = QTextCodec::codecForName(mapCharset);
    QString str = /*(char*)lpszString;*/codec->toUnicode((char*)lpszString);

    //QRect boundsRect = metrics.boundingRect(str, str.length());
    //boundsRect.setWidth(boundsRect.width()*2);
    //boundsRect.setHeight(boundsRect.height()*2);

    //we will consider alignment
    /*if(bAligned)*/ qpt = AlignText(qpt, str);

    QColor	col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);
    m_painter->setBackgroundMode(Qt::TransparentMode);

    QTime time;  time.start();
    if(m_angle)
    {
        m_painter->save();
        m_painter->translate( qptC.x(), qptC.y() );
        m_painter->rotate( (float)m_angle);
        m_painter->drawText(qpt-qptC, str);
        //m_painter->drawText(0, 0-boundsRect.height(), boundsRect.width(), boundsRect.height()*2, m_align, str, str.length());
        if(m_angle) m_painter->restore();
    }
    else m_painter->drawText(qpt, str);
    //m_painter->drawText(qpt.x(), qpt.y()-boundsRect.height(), boundsRect.width(), boundsRect.height()*2, m_align, str, str.length());
    /*QPen penBckp = m_painter->pen();
    m_painter->setPen(Qt::SolidLine);
    QFontMetrics metrics(*pFont);
    QRect boundingRect = metrics.boundingRect(str);
    boundingRect.translate(qpt);
    m_painter->drawRect(boundingRect);
    m_painter->setPen(penBckp);*/

    ::drawTime+= time.restart();
    ::drawCount++;
    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}

// ==================================================================
bool MQtDevice::TextOut(double dX,double dY, MString& lpszString/*, bool bAligned*/)
// ==================================================================
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    MPoint pnt(dX,dY);
    QPoint qpt, qptC;
    BumToLogicI(&pnt,qpt);
    Normalize(qpt);     // vipa 2005.01.20
    qptC = qpt;

    //QTextCodec* codec = QTextCodec::codecForName(mapCharset);

    QString str = /*(char*)lpszString;*/codec->toUnicode((char*)lpszString);

    //QString str((char*)lpszString); //Lenala 01/08/05

    //QFontMetrics metrics(m_painter->font());

    //QRect boundsRect = metrics.boundingRect(str, str.length());
    //boundsRect.setWidth(boundsRect.width()*2);
    //boundsRect.setHeight(boundsRect.height()*2);

    //we will consider alignment
    /*if(bAligned)*/
    qpt = AlignText(qpt, str);

    //we draw a binding point
    /*m_painter->setBrush(QBrush(QColor(255,0,255), Qt::SolidPattern));
    m_painter->drawEllipse(qptC, 3, 3);*/

    QColor col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);
    m_painter->setBackgroundMode(Qt::TransparentMode);

    QTime time;  time.start();
    if(m_angle)
    {
        m_painter->save();
        m_painter->translate( qptC.x(), qptC.y() );
        m_painter->rotate( (float)m_angle);
        m_painter->drawText(qpt-qptC, str);
        //m_painter->drawText(0, 0-boundsRect.height(), boundsRect.width(), boundsRect.height()*2, m_align, str, str.length());

        if(m_angle) m_painter->restore();
    }
    else m_painter->drawText(qpt, str);
    //m_painter->drawText(qpt.x(), qpt.y()-boundsRect.height(), boundsRect.width(), boundsRect.height()*2, m_align, str, str.length());
    ::drawTime+= time.restart();
    ::drawCount++;

    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}

// ==================================================================
//Text conclusion in the coding windows to the device
bool MQtDevice::TextOutWindows(double dX,double dY,char * lpszString,int /*nCount*//*, bool bAligned*/)
// ==================================================================
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    MPoint pnt(dX,dY);
    QPoint qpt, qptC;
    BumToLogicI(&pnt,qpt);
    Normalize(qpt);     // vipa 2005.01.20
    qptC = qpt;

    //QTextCodec* codec = QTextCodec::codecForName(mapCharset); //	QTextCodec* codec = QTextCodec::codecForName("cp1251");
    QString str = codec->toUnicode((char*)lpszString);

    //we will consider alignment
    /*if(bAligned)*/ qpt = AlignText(qpt, str);

    QColor col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);
    m_painter->setBackgroundMode(Qt::TransparentMode);
    //  m_painter->setBackgroundMode (Qt::OpaqueMode);

    if(m_angle)
    {
        m_painter->save();
        m_painter->translate( qptC.x(), qptC.y() );
        m_painter->rotate( (float)m_angle);
        m_painter->drawText(qpt-qptC, str);
        //m_painter->drawText(0, 0-boundsRect.height(), boundsRect.width(), boundsRect.height()*2, m_align, str, str.length());

        if(m_angle) m_painter->restore();
    }
    else m_painter->drawText(qpt, str);

    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}

// ==================================================================
bool MQtDevice::TextOutWindows(double dX,double dY,MString& lpszString/*, bool bAligned*/)
// ==================================================================
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    MPoint pnt(dX,dY);
    QPoint qpt,qptC;
    BumToLogicI(&pnt,qpt);
    Normalize(qpt);     // vipa 2005.01.20
    qptC = qpt;

    //QTextCodec* codec = QTextCodec::codecForName(mapCharset); //	QTextCodec* codec = QTextCodec::codecForName("cp1251");
    QString cstr = codec->toUnicode((char*)lpszString);
    //QString cstr((char*)lpszString);

    //we will consider alignment
    /*if(bAligned)*/ qpt = AlignText(qpt, cstr);

    QColor col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);
    m_painter->setBackgroundMode(Qt::TransparentMode);

    if(m_angle)
    {
        m_painter->save();
        m_painter->translate( qptC.x(), qptC.y() );
        m_painter->rotate( (float)m_angle);
        m_painter->drawText(qpt-qptC, cstr);
        //m_painter->drawText(0, 0-boundsRect.height(), boundsRect.width(), boundsRect.height()*2, m_align, str, str.length());

        if(m_angle) m_painter->restore();
    }
    else m_painter->drawText(qpt, cstr);
    //m_painter->drawText(qpt.x(), qpt.y()-boundsRect.height(), boundsRect.width()*2, boundsRect.height()*2, m_align, cstr, cstr.length());

    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}

// ==================================================================
//Rectangle conclusion
bool MQtDevice::Rectangle(double dX1,double dY1,double dX2,double dY2)
// ==================================================================
{
    SelectGDI();
    MRect rect(dX1,dY1,dX2,dY2);
    QRect crect;
    BumToLogicI(&rect,crect);
    Normalize(crect);     //  2005.01.20 vipa
    m_painter->drawRect(crect);
    DeselectGDI();
    return true;
}

// ==================================================================
bool MQtDevice::Rectangle(MRect* lpRect)
// ==================================================================
{
    SelectGDI();
    QRect crect;
    BumToLogicI(lpRect,crect);
    Normalize(crect);     //  2005.01.20 vipa
    m_painter->drawRect(crect);
    DeselectGDI();
    return true;
}

// ==================================================================
//The rounded-off rectangle
bool MQtDevice::RoundRect(MRect* lpRect,MPoint EllipsPoint)
// ==================================================================
{
    SelectGDI();
    QRect crect;
    BumToLogicI(lpRect,crect);
    Normalize(crect);     //  2005.01.20 vipa
    m_painter->drawRoundRect(crect, (int)EllipsPoint.x, (int)EllipsPoint.y);
    DeselectGDI();
    return true;
}

// ==================================================================
//positioning
// bool MQtDevice::MoveTo( double x, double y )		//! it is replaced on ::DrawLine(double x1, double y1, double x2, double y2)
// // ==================================================================
// {
//   MPoint pnt(x,y);
// 	QPoint qpt;
// 	BumToLogicI(&pnt,qpt);
// 	//Normalize(qpt);       //  2005.01.20 vipa ((LenaLa 16.09.2005
// 	//m_painter->moveTo(qpt);
// 	return true;
// }
// ==================================================================
// bool MQtDevice::MoveTo( MPoint point )	//! it is replaced on ::DrawLine(MPoint point1, MPoint point2)
// // ==================================================================
// {
// 	QPoint qpt;
// 	BumToLogicI(&point,qpt);
// 	Normalize(qpt);       //  2005.01.20 vipa ((LenaLa 16.09.2005
// 	m_painter->moveTo(qpt);
// 	return true;
// }

// ==================================================================
//Carrying out line
// bool MQtDevice::LineTo( double x, double y )	//! it is replaced on ::DrawLine(double x1, double y1, double x2, double y2)
// // ==================================================================
// {/* //BEFORE vipa
// 	MPoint pnt(x,y);
// 	QPoint qpt;
// 	BumToLogicI(&pnt,qpt);
// 	m_painter->lineTo(qpt);
// 	return true;
// 	*/
// 	if(!pPen) pPen = new QPen();      // vipa 2004.12.20
// 	if(!pPen)	return false;
// 	if ( ! SelectGDI() )	{
// 		DeselectGDI();
// 		return false;
// 	}
// 	MPoint pnt(x,y);
// 	QPoint qpt, prevPt;
// 	
// 	BumToLogicI(&pnt,qpt);
// 	//First: check previous QPainter position(prevPt) using next draw point (qpt)
// 	//prevPt = m_painter->pos();
// 	//if prevPt is out of Critical Rect, let's normalize it
// 	
// 	if ( (abs(prevPt.x())>4000)||(abs(prevPt.y())>4000) )
// 	{
// 		Normalize(qpt, prevPt);
// 		m_painter->moveTo(prevPt);
// 	}        
// 	//Second: Checking new draw point (qpt)
// 	if ( (abs(qpt.x())>4000)||(abs(qpt.y())>4000) )
// 		Normalize(prevPt,qpt);        
// 	
// 	//Normalize(qpt);        
// 	
// 	m_painter->lineTo(qpt);
// 
// 	DeselectGDI();
// 	return true;
// 
// }

// ==================================================================
// bool MQtDevice::LineTo( MPoint point )	//! it is replaced on ::DrawLine(MPoint point1, MPoint point2 )
// // ==================================================================
// {/* //BEFORE vipa
// 	QPoint qpt;
// 	BumToLogicI(&point,qpt);
// 	m_painter->lineTo(qpt);
// 	return true;
// 	*/
// 	if(!pPen) pPen = new QPen();      // vipa 2004.12.20
// 	if(!pPen)	return false;
// 	if ( ! SelectGDI() )	{
// 		DeselectGDI();
// 		return false;
// 	}
//   
// 	QPoint qpt, prevPt;
// 	
// 	BumToLogicI(&point,qpt);
// 	//First: check previous QPainter position(prevPt) using next draw point (qpt)
// 	prevPt = m_painter->pos();
// 	//if prevPt is out of Critical Rect, let's normalize it
// 	if ( (abs(prevPt.x())>4000)||(abs(prevPt.y())>4000) )
// 	{
// 		Normalize(qpt, prevPt);
// 		m_painter->moveTo(prevPt);
// 	}        
// 	//Second: Checking new draw point (qpt)
// 	if ( (abs(qpt.x())>4000)||(abs(qpt.y())>4000) )
// 		Normalize(prevPt,qpt);        
// 
// 	m_painter->lineTo(qpt);
// 	DeselectGDI();
// 	return true;
// 
// }


// ==================================================================
bool MQtDevice::DrawLine(MPoint point1, MPoint point2 )
// ==================================================================
{

    if(!pPen) pPen = new QPen();      // vipa 2004.12.20
    if(!pPen)	return false;
    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }
    QPoint qpt1,qpt2;
    BumToLogicI(&point1,qpt1);
    BumToLogicI(&point2,qpt2);
    //First: check previous QPainter position(prevPt) using next draw point (qpt)
    //if prevPt is out of Critical Rect, let's normalize it
    if ( (abs(qpt1.x())>4000)||(abs(qpt1.y())>4000) )
        Normalize(qpt1);//, prevPt);

    //Second: Checking new draw point (qpt)
    if ( (abs(qpt2.x())>4000)||(abs(qpt2.y())>4000) )
        Normalize(qpt2);//,qpt);

    //m_painter->setCompositionMode(QPainter::CompositionMode_Multiply);

     if(fillingPolygon.size())
    {
        m_painter->setClipping (true);
        m_painter->setClipRegion(fillingPolygon);
    }

    m_painter->drawLine(qpt1, qpt2);
    DeselectGDI();

     if( m_painter->hasClipping())
    {
        m_painter->setClipping (false);
    }

    return true;

}


// ==================================================================
bool MQtDevice::DrawLine(double x1, double y1, double x2, double y2)
// ==================================================================
{
    return DrawLine(MPoint(x1,y1), MPoint(x2,y2));
}

// ==================================================================
//Drawing of an arch
//( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 )
bool MQtDevice::Arc( double , double , double , double , double , double , double , double  )
// ==================================================================
{
    return true;
}

// ==================================================================
//( MRect* lpRect, MPoint ptStart, MPoint ptEnd )
bool MQtDevice::Arc( MRect* , MPoint , MPoint  )
// ==================================================================
{
    return true;
}

// ==================================================================
//Filling of a rectangle
void MQtDevice::FillRect( MRect* lpRect, MColor* pColor )
// ==================================================================
{
    QRect fillRect;
    BumToLogicI(lpRect,fillRect);
    QColor color(pColor->GetR(), pColor->GetG(), pColor->GetB(), pColor->GetA());
    QBrush tempBr(color);
    Normalize(fillRect);    //  2005.01.20 vipa
    m_painter->fillRect(fillRect, tempBr);
}

// ==================================================================
void MQtDevice::FillSolidRect( MRect* lpRect, MColor clr )
// ==================================================================
{
    QRect fillRect;
    BumToLogicI(lpRect,fillRect);
    QColor color(clr.GetR(), clr.GetG(), clr.GetB(),clr.GetA());
    QBrush tempBr(color);
    Normalize(fillRect);    //  2005.01.20 vipa
    m_painter->fillRect(fillRect, tempBr);
}

// ==================================================================
void MQtDevice::FillSolidRect( double x, double y, double cx, double cy, MColor clr )
// ==================================================================
{
    MRect lpRect(x, y, cx, cy);
    QRect fillRect;
    SelectGDI();
    BumToLogicI(&lpRect,fillRect);
    QColor color(clr.GetR(), clr.GetG(), clr.GetB(),clr.GetA());
    QBrush tempBr(color);
    Normalize(fillRect);    //  2005.01.20 vipa
    m_painter->fillRect(fillRect, tempBr);
    DeselectGDI();
}

// ==================================================================

//Filling of an ellipse
bool MQtDevice::Ellipse( double x1, double y1, double x2, double y2 )
// ==================================================================
{
    MRect lpRect(x1, y1, x2, y2);
    SelectGDI();
    QRect qrect;
    BumToLogicI(&lpRect,qrect);
    Normalize(qrect);         //  2005.01.20 vipa
    m_painter->drawEllipse(qrect);
    DeselectGDI();
    return true;
}

// ==================================================================
bool MQtDevice::Ellipse( MRect* lpRect )
// ==================================================================

{
    SelectGDI();
    QRect qrect;
    BumToLogicI(lpRect,qrect);
    Normalize(qrect);         //  2005.01.20 vipa
    m_painter->drawEllipse(qrect);
    DeselectGDI();
    return true;
}

// ==================================================================
bool MQtDevice::Circle( MPoint ptCenter,double dRad,MColor /*color*/)
// ==================================================================
{
    SelectGDI();
    MRect rectBum(ptCenter.x - dRad,ptCenter.y - dRad,ptCenter.x + dRad,ptCenter.y + dRad);
    QRect rect;
    BumToLogicI(&rectBum,rect);
    Normalize(rect);         //  2005.01.20 vipa
    m_painter->drawEllipse(rect);
    DeselectGDI();
    return true;
}

// ==================================================================
//Filling of an segment
//( double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4 )
bool MQtDevice::Pie( double , double , double , double , double , double , double , double  )
// ==================================================================
{
    return true;
}

// ==================================================================
//( MRect* lpRect, MPoint ptStart, MPoint ptEnd )
bool MQtDevice::Pie( MRect* , MPoint , MPoint  )
// ==================================================================
{
    return true;
}

// ==================================================================
//	Preservation of a pen
void MQtDevice::SavePencil()
// ==================================================================
{
    if(!bPen)
        return;
    pen_stack.push_back(pPen);
    pPen = NULL;
    bPen = false;
};

// ==================================================================
//( MPoint* ptCenter, double dRad, int startAngle,int spanAngle)
bool MQtDevice::Pie( MPoint ptCenter, double dRad, double startAngle,double spanAngle)
// ==================================================================
{
    SelectGDI();
    QTime time;  time.start();

    MRect rectBum(ptCenter.x - dRad,ptCenter.y - dRad,ptCenter.x + dRad,ptCenter.y + dRad);
    QRect rect;
    BumToLogicI(&rectBum,rect);
    Normalize(rect);
    m_painter->drawPie(rect,startAngle,spanAngle);
    ::drawTime+= time.restart();
    ::drawCount++;

    DeselectGDI();
    return true;
}

// ==================================================================
// Restoration of a pen
void MQtDevice::RestorePencil()
// ==================================================================
{
    if(!pen_stack.size())
        return;
    QPen* pen = pen_stack.back();
    pen_stack.pop_back();
    ChangePen(pen,true);
};


// ==================================================================
//	Preservation of a font
//The font from a pool in a stack isn't located
void MQtDevice::SaveFont()
// ==================================================================
{
    if(m_bSelectedFromPool&&(!bFont))
        return;
    font_stack.push_back(pFont);
    pFont = NULL;
    bFont = false;
};


// ==================================================================
// Restoration of a font
void MQtDevice::RestoreFont()
// ==================================================================
{
    if(!font_stack.size())
        return;
    QFont* font = font_stack.back();
    font_stack.pop_back();
    ChangeFont(font,true);
    m_bSelectedFromPool = false;
};


// ==================================================================
//	Preservation of a brush
void MQtDevice::SaveZapolnenie()
// ==================================================================
{
    if(!bBrush)
        return;
    brush_stack.push_back(pBrush);
    pBrush = NULL;
    bBrush = false;
};


// ==================================================================
// Restoration of a brysh
void MQtDevice::RestoreZapolnenie()
// ==================================================================
{
    if(!brush_stack.size())
        return;
    QBrush* brush = brush_stack.back();
    brush_stack.pop_back();
    ChangeBrush(brush,true);
};

// ==================================================================
// We receive the text size
double MQtDevice::GetTextExtent(MString& str)
// ==================================================================
{
    if(!m_painter) return 0.0;

    SelectGDI();

    QFontMetrics metrics(m_painter->font());

    //codec = QTextCodec::codecForName(mapCharset);
    QString	str0 = codec->toUnicode((char*)str);
    QRect boundsRect = metrics.boundingRect(str0);

    double dVal = LogicToBum(boundsRect.width());

    DeselectGDI();

    return dVal;
};

// ==================================================================
// We receive the drawing device size in millimeters
MIntSize MQtDevice::GetSizeMM()
// ==================================================================
{
    QPaintDevice* pd = m_painter->device();
    MIntSize sz(pd->widthMM(), pd->heightMM());
    return sz;
}
//================================================
//
bool MQtDevice::Distinguish_Text(double dVal)
{
    unsigned iVal;
    if (drawTextFlag)  iVal = BumToLogicI(textRatio * dVal);
    else               iVal = BumToLogicI(dVal);
    return (iVal >= distinguishTextSize);
}
//=======================================================================
//! Drawing of a grid of symbols
bool MQtDevice::TextOutPramSetka(MPtArray* pta,MRect* rect,
                                 MString* sss,double step,double /*dSymVal*/)
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    //QTextCodec* codec = QTextCodec::codecForName(mapCharset);
    QString str = /*codec->toUnicode*/(sss->c_str());

    QColor col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);

    //----------------------------------------------------------------
    //we define borders of drawing of a grid (that is knots which participate in
    //to drawing
    //It is meant that drawing goes from left to right and from top to down
    //Trebuttsya to find the interval of knots displayed at present
    //so that an arrangement of knots I didn't depend on a zoom
    rect->NormalizeGeoRect();

    double dMinX,dMaxX,dMinY,dMaxY;
    //We receive visible area in real coordinates
    MRect rectGeoVis;
    GetClientRect(&rectGeoVis);
    rectGeoVis.NormalizeGeoRect();


    dMinX = rect->left;
    while((dMinX<rectGeoVis.left)&&(dMinX<rect->right))
        dMinX +=step;
    dMinX-=step;
    dMaxX = dMinX;
    while((dMaxX<rectGeoVis.right)&&(dMaxX<rect->right))
        dMaxX+=step;

    dMinY = rect->bottom;
    while((dMinY<rectGeoVis.bottom)&&(dMinY<rect->top))
        dMinY +=step;
    dMinY-=step;
    dMaxY = dMinY;
    while((dMaxY<rectGeoVis.top)&&(dMaxY<rect->top))
        dMaxY+=step;
    
    MPoint draw;

    //we draw a grid of symbols parallel to axes of coordinates, providing a step
    for(double x=dMinX;x<=dMaxX;x+=step)
        for(double y=dMinY;y<=dMaxY;y+=step){
            draw = MPoint(x,y);
            QPoint qpt;
            BumToLogicI(&draw,qpt);
            Normalize(qpt);     // vipa 2005.01.20

            //      qpt = AlignText(qpt, str);
            if ( (qpt.y()<0)||(qpt.x()<0)||(qpt.x()>width)||(qpt.y()>height) )
                continue;
            QTime time;  time.start();
            if(draw.InnerPtFast(pta->GetHead(),pta->GetSize()))
            {
                if(m_angle)
                {
                    m_painter->save();
                    m_painter->translate( qpt.x(), qpt.y() );
                    m_painter->rotate( (float)m_angle);
                    QPoint pt(0, 0);
                    m_painter->drawText(pt, str);
                    if(m_angle) m_painter->restore();
                }
                else
                {
                    m_painter->drawText(qpt, str);
                }
            }
            ::drawTime+= time.restart();
            ::drawCount++;

        }   // for

    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}
//=======================================================================
//!
bool MQtDevice::TextOutKosSetka(MPtArray* pta, MRect* rect,
                                MString* sss, double angle, double step, double dSymVal)
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    //QTextCodec* codec = QTextCodec::codecForName(mapCharset);
    QString str = /*codec->toUnicode*/(sss->c_str());


    QColor col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);
    //---------------------------------------------
    //we lead a angle to a normal amount
    while(angle>=360)angle-=360;
    while(angle<0)angle+=360;

    //if a grid after all a straight line, we send drawing there
    if((angle==0)||(angle==90)||(angle==180)||(angle==270))
    {
        return TextOutPramSetka(pta,rect,sss,step,dSymVal);
    }

    //we will find grid knots
    //search is carried out so:
    //      the equations of straight lines of a type of Y=kX+b are worked out,
    //      where k1 and k2 - tangents of angle of an inclination and perpendicular to it a angle
    //      then ranges of change of the b1 and b2 parameters pay off,
    //      and also their step providing distance between grid knots.
    //      Then in a cycle in the parameters b there are points of intersection
    //      each straight line with everyone.
    //In these points of intersection the symbol is also drawn

    double b1min,b2min,b1max,b2max,db1,db2,k1,k2,b1,b2;
    {
        k1=tan(angle*pi/180);
        //we learn the range of change of members of b1 and b2 and their step
        double b1=rect->bottom-k1*rect->left;
        double b2=rect->top-k1*rect->right;
        double b3=rect->bottom-k1*rect->right;
        double b4=rect->top-k1*rect->left;
        db1=fabs(step*sqrt(k1*k1+1.0));
        b1min=Min(Min(b1,b2),Min(b3,b4));
        b1max=Max(Max(b1,b2),Max(b3,b4));

        k2=-1/k1;
        b1=rect->bottom-k2*rect->left;
        b2=rect->top-k2*rect->right;

        b3=rect->bottom-k2*rect->right;
        b4=rect->top-k2*rect->left;
        db2=fabs(step*sqrt(k2*k2+1.0));
        b2min=Min(Min(b1,b2),Min(b3,b4));
        b2max=Max(Max(b1,b2),Max(b3,b4));
    }

    //in knots of a grid we draw a symbol
    double x,y;
    MPoint draw;
    //cout <<"KosSetka- "<< (int)(b1max-b1min)/db1 * (b2max-b2min)/db2 <<endl;
    for(b1=b1min;b1<=b1max;b1+=db1)
        for(b2=b2min;b2<=b2max;b2+=db2)
        {
            //we find crossing of two straight lines
            x=(b2-b1)/(k1-k2);
            y=k2*x+b2;

            //we draw a symbol
            draw = MPoint(x,y);
            QPoint qpt;
            BumToLogicI(&draw, qpt);
            Normalize(qpt);     // vipa 2005.01.20
            //      qpt = AlignText(qpt, str);
            if ( (qpt.y()<0)||(qpt.x()<0)||(qpt.x()>width)||(qpt.y()>height) )   // vipa 2004.09.15
                continue;
            QTime time;  time.start();
            if(draw.InnerPtFast(pta->GetHead(),pta->GetSize()))
            {
                if(m_angle)
                {
                    m_painter->save();
                    m_painter->translate( qpt.x(), qpt.y() );
                    m_painter->rotate( (float)m_angle);
                    QPoint pt(0, 0);
                    m_painter->drawText(pt, str);
                    if(m_angle)
                    {
                        m_painter->restore();
                    }
                }
                else
                {
                    m_painter->drawText(qpt, str);
                }
            }
            ::drawTime+= time.restart();
            ::drawCount++;
        }

    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}
//=======================================================================
//!
bool MQtDevice::TextOutXaosSetka(MPtArray* pta,MRect* rect,
                                 MSymbol* sym,double step,double dSymVal)
{
    if(!pPen) pPen = new QPen();      // vipa 2004.09.17
    if(!pPen)	return false;

    if ( ! SelectGDI() )	{
        DeselectGDI();
        return false;
    }

    QColor col_old = pPen->color();
    pPen->setColor(QColor(font_color.GetR(), font_color.GetG(), font_color.GetB()));
    m_painter->setPen(*pPen);

    //---------------------------------------------
    //we define the maximum shift of symbols concerning the center
    double dr=(step/2-dSymVal);
    //if shift is less than zero, is accepted its equal to zero
    if(dr<0) dr=0;
    //  MString ctr= sym->str;
    //QTextCodec* codec = QTextCodec::codecForName(mapCharset);
    QString str = /*codec->toUnicode*/(sym->str.c_str());
    //we define borders of drawing of a grid
    double xmin=Min(rect->left,rect->right);
    double xmax=Max(rect->left,rect->right);
    double ymin=Min(rect->top,rect->bottom);
    double ymax=Max(rect->top,rect->bottom);
    double x,y;


    //we draw a grid of symbols also, as well as a straight line,
    //but symbols we have in a random way
    //with shift on each of axes.
    //The angle of a symbol is also casual
    M_LOG_FONT lfont = sym->logfont;
    lfont.lfHeight = dSymVal;

    //The array of various fonts for turn is created
    //it is used for drawing speed
    //as font choice significantly quicker than its creation
    for(int i = 0; i<12; i++){
        lfont.lfEscapement=i*300;
        AddFontToPool(&lfont,i);
    }
    SaveFont();
    SelectFontFromPool(0);

    //	int randv;
    double xx,yy;
    MPoint draw;
    srand(1);
    //concerning each knot of a rectangular grid
    for(x=xmin;x<=xmax;x+=step)
        for(y=ymin;y<=ymax;y+=step)
        {
            //we define shift along each axis
            xx=x+dr*((double)rand()/(double)RAND_MAX*2-1);
            yy=y+dr*((double)rand()/(double)RAND_MAX*2-1);
            //we define an angle of rotation
            SelectFontFromPool((int)((double)rand()/(double)RAND_MAX*12));

            draw = MPoint(xx,yy);
            QPoint qpt;
            BumToLogicI(&draw,qpt);
            Normalize(qpt);     // vipa 2005.01.20
            //      qpt = AlignText(qpt, str);

            if ( (qpt.y()<0)||(qpt.x()<0)||(qpt.x()>width)||(qpt.y()>height) )   // vipa 2004.09.15
                continue;
            if(draw.InnerPtFast(pta->GetHead(),pta->GetSize()))
            {
                if(m_angle)
                {
                    m_painter->save();
                    m_painter->translate( qpt.x(), qpt.y() );
                    m_painter->rotate( (float)m_angle);
                    QPoint pt(0, 0);
                    m_painter->drawText(pt, str);
                    if(m_angle)
                    {
                        m_painter->restore();
                    }
                }
                else
                {
                    m_painter->drawText(qpt, str);
                }
            }

        }

    RestoreFont();
    ClearPool();
    //---------------------------------------------

    pPen->setColor(col_old);
    m_painter->setPen(*pPen);

    DeselectGDI();
    return true;
}
//====================================
// for many PaintDevices:   
// QRect(-4000,-4000, 4000,4000) - margin(ultimate) coordinates;
void MQtDevice::Normalize(QPoint MPt, QPoint &pt) {
    int screenLimit= 3990;
    double nY, nX;
    nX = pt.x();
    nY = pt.y();

    if ( pt.x() > screenLimit ){
        nX = (double)screenLimit;//pt.setX( screenLimit);
        nY = (double)pt.y() - ( ((double)pt.y() - (double)MPt.y())*((double)pt.x() - nX) )/((double)pt.x() - (double)MPt.x());
    }
    else if ( pt.x() < -screenLimit )  {
        nX = -(double)screenLimit;//pt.setX( screenLimit);
        nY = (double)pt.y() - ( ((double)pt.y() - (double)MPt.y())*((double)pt.x() - nX) )/((double)pt.x() - (double)MPt.x());
    }
    if ( pt.y() > screenLimit ){
        nY = (double)screenLimit;
        nX = (double)pt.x() - ( ((double)pt.x() - (double)MPt.x())*((double)pt.y() - nY) )/((double)pt.y() - (double)MPt.y());
    }
    else if ( pt.y() < -screenLimit ){
        nY = -(double)screenLimit;
        nX = (double)pt.x() - ( ((double)pt.x() - (double)MPt.x())*((double)pt.y() - nY) )/((double)pt.y() - (double)MPt.y());
    }
    pt.setX(dbl_to_int(nX));
    pt.setY(dbl_to_int(nY));
}

//====================================
// for many PaintDevices:   
// QRect(-4000,-4000, 4000,4000) - margin(ultimate) coordinates;
void MQtDevice::Normalize(QPoint &pt) {
    return;
    int screenLimit= 3900;
    if ( pt.x() > screenLimit )        pt.setX( screenLimit);
    else if ( pt.x() < -screenLimit )  pt.setX(-screenLimit);
    if ( pt.y() > screenLimit )        pt.setY( screenLimit);
    else if ( pt.y() < -screenLimit )  pt.setY(-screenLimit);
}
//====================================
void MQtDevice::Normalize(QPolygon &pta) {
    //int screenLimit= 3999;

    /*
QRect limRct(QPoint(screenLimit,screenLimit),QPoint (-screenLimit,-screenLimit));
for (int i=0; i<pta.size()-1;i++)
{
    bool fl1 = (limRct.contains(pta[i]));
    bool fl2 = (limRct.contains(pta[i+1]));
    if (fl1&&fl2)
    {
        continue;
    }
    else if (fl1||fl2)
    {
      QPointF intersectPt;
      QLineF line(pta[i],pta[i+1]);
      QLineF rectItem(limRct.topLeft(),limRct.topRight());
      if (line.intersect(rectItem,&intersectPt)!=QLineF::BoundedIntersection)
        {
            rectItem = QLineF(limRct.topRight(),limRct.bottomRight());
            if (line.intersect(rectItem,&intersectPt)!=QLineF::BoundedIntersection)
            {
                rectItem = QLineF(limRct.bottomRight(),limRct.bottomLeft());
                if (line.intersect(rectItem,&intersectPt)!=QLineF::BoundedIntersection)
                {
                    rectItem = QLineF(limRct.bottomLeft(),limRct.topLeft());
                    line.intersect(rectItem,&intersectPt);
                }
            }
        }

      QPoint cros((int)intersectPt.x(),(int)intersectPt.y());
      pta.insert(i+1,cros);
      i++;
      if (! fl1)
        {pta.remove(i);i--;}

    }
    else if(!(fl1||fl2))
    {
        pta.remove(i);
        i--;
        continue;
    }
}
*/

    unsigned int i;
    const unsigned int iPtaCount = (unsigned int)pta.count();
    for(i = 0; i< /*pta.count()*/iPtaCount; i++)
        Normalize((QPoint &)pta.at(i));

    //Normalize(pta.at(pta.count()-2),pta.at(pta.count()-1));
}
//====================================
void MQtDevice::Normalize(QRect &rect) {
    return;
    int screenLimit= 3900;
    if ( rect.right()  >  screenLimit )  rect.setRight ( screenLimit);
    if ( rect.left()   < -screenLimit )  rect.setLeft  (-screenLimit);
    if ( rect.bottom() < -screenLimit )  rect.setBottom(-screenLimit);
    if ( rect.top()    >  screenLimit )  rect.setTop   ( screenLimit);
}

//====================================
bool MQtDevice::Intersects(QPolygon &pta) {
    QRect rect= QRect(0,0, width-1,height-1);  // screen Coo
    const unsigned int iPtaCount = pta.count()-1;
    for(unsigned int i=0; i</*pta.count()-1*/iPtaCount; i++){
        QRect obhv(pta.at(i), pta.at(i+1));
        obhv.normalized ();
        if ( obhv.intersects(rect) ) return true;
    }
    return false;
}

//clear all parameters & initialize
bool MQtDevice::ClearAllAndInit(QPainter* pDev)
{
    //deleting parent values
    if (pCurrentClipLog) delete pCurrentClipLog;
    
    //initing parent values
    m_bAdvocate=false;
    m_bClipRegionSelected=false;
    pCurrentClipLog = new MRegionOpArray();
    
    //clearing self parameters
    ClearPool();
    ClearClip();

    //we initialize 'painter'
    if (pDev)
    {
        m_pRealPD = (QPaintDevice*)pDev;
        m_pOutputPD = (QPaintDevice*)pDev;
        m_painter = pDev;
        width = abs((pDev->window()).right());
        height = abs((pDev->window()).bottom());
    }

    if(pTempPen) delete pTempPen;
    pTempPen = nullptr;
    if(pTempBrush) delete pTempBrush;
    pTempBrush = nullptr;
    if(pTempFont) delete pTempFont;
    pTempFont = nullptr;
    
    //Initting self params (like in constructor)
    ChangePen(nullptr, false);
    ChangeBrush(nullptr, false);
    ChangeFont(nullptr, false);

    pTempPen = new QPen(Qt::NoPen);
    pTempBrush = new QBrush;
    pTempFont = new QFont("Courier", 10);
    
    m_angle = 0;
    m_align = UT_TA_CENTER | UT_TA_BASELINE;

    return true;
}

//drawing of a raster - 'QImage' from 'MRaster'
bool MQtDevice::Raster(MRaster* raster)
{ 
    //get geo and logic bounds
    MRect rectZoom,rectInter,rectRaster;
    rectRaster = raster->m_bounds;
    QRect qrectRaster, qrectInter;
    BumToLogicI(&rectRaster, qrectRaster);
    qrectRaster = qrectRaster.normalized ();
    if((rectRaster.Width() == 0)||(rectRaster.Height() == 0))
    {
        if(raster->pMap)
        {m_painter->drawImage(qrectRaster.topLeft(), *raster->pMap);
            return true; }
        else return false;
    }

    GetClientRect(&rectZoom);
    IntersectionGeoRect(&rectZoom, &rectRaster, rectInter);
    BumToLogicI(&rectInter, qrectInter);
    qrectInter = qrectInter.normalized();

    QImage *qImage;

    //get the last drawing image
    if(raster->pDrawMap == NULL) //if not define = first drawing
    {
        //pDrawMap = new QImage(raster->pMap->copy());
        raster->rectDraw = rectRaster;
        raster->qrectDraw = qrectRaster;
    }
    //width and height of raster in pixels
    int width = qrectInter.width();
    int height = abs(qrectInter.height());

    if(rectInter == rectRaster)
    {
        //raster->gegetReadImgFromGtiff(0,0,raster->pMapSz.width(),raster->pMapSz.height(),(int)(raster->pMapSz.width()/(double)width));
        qImage = raster->GetImagePart(QRect(0,0,raster->pMapSz.width(),raster->pMapSz.height()),(int)(raster->pMapSz.width()/(double)width));
        m_painter->drawImage(qrectInter.topLeft(),qImage->scaled(width,height));
}
    else
    {
        MRect rectDraw = raster->rectDraw;
        QRect qrectDraw = raster->qrectDraw;
        int iwidth = max(1, raster->pMapSz.width()*width/qrectDraw.width());
        int iheight = max(1, abs(raster->pMapSz.height()*height/qrectDraw.height()));
        int x = (qrectInter.left() - qrectDraw.left())/(double)qrectDraw.width()*raster->pMapSz.width();
        int y = (qrectInter.top() - qrectDraw.top())/(double)qrectDraw.height()*raster->pMapSz.height();
        int rsz = (int)(iwidth/(double)width);        
        //qImage = raster->GetImagePart(QRect(x,y,iwidth,iheight),rsz==0 ? 1:rsz);
        qImage = raster->GetImagePart(QRect(0,0,raster->pMapSz.width(),raster->pMapSz.height()),rsz==0 ? 1:rsz);
        qImage->save("Img0.png");
        QImage img = qImage->copy(x,y,iwidth,iheight);
        img.save("ClipImg.png");
        m_painter->drawImage(qrectInter.topLeft(),img.scaled(width,height));
        //qImage = QImage(pDrawMap->copy(x,y,iwidth,iheight));
    }
    //scale and draw the new image
    //*pDrawMap = qImage.scale(width,height);
    //raster->rectDraw = rectInter;
    //raster->qrectDraw = qrectInter;

    //m_painter->drawImage(qrectInter.topLeft(),qImage->scaled(width,height));// !!!


    return true;
}

//! drawing of a transparent background
bool MQtDevice::OpacityBackground(int lightness, int transparent)
{
    SaveZapolnenie();
    QColor color(QColor::fromHsv(0,0,lightness,transparent));
    m_painter->setBrush(color);
    int h = m_painter->device()->height();
    int w = m_painter->device()->width();
    m_painter->drawRect(0,0,w,h);
    RestoreZapolnenie();
    return true;
}
/*
//! we consider the geoscale of the current display
double MQtDevice::GetCurrentScale()
{
    QPaintDevice *device = m_painter->device();
    if (!device) return 0.0;
    double geoWidth = m_BumBounds.right - m_BumBounds.left;
    int deviceWidth = device->widthMM();
    double temp = deviceWidth*1000/geoWidth;//return device->widthMM()*1000/geoWidth;
    qDebug(QString::number(temp).toStdString().c_str());
    return temp;
    return geo_scale;
}*/
