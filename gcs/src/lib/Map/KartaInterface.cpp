// KartaInterface.cpp: implementation of the MKartaInterface class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
#include <QPen>
#include <qdatetime.h>
#include "MercatorTile.h"
#include "KartaInterface.h"
#include "Varglobal.h"
#include "Geodesic.h"
#include <math.h>
#include "../MGRS/mgrs.h"
#include <QMessageBox>
#include "../MGRS/mgrsgridform.h"

extern int drawCount;
extern int drawTime;

const int ID_ACTLAYER_FindPoint = 8888;
const int ID_ACTOBJECT_FindPoint = 1;
#ifdef BOR_CHEMISTRY
const std::string kkod_FindPoint = "UZN 170 SRV 1";
#else
const std::string kkod_FindPoint = "UZN 5000 SRV 5";
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//! Default Contructor with no params
MKartaInterface::MKartaInterface(QObject *parent) : QObject(parent)
{
    InitKartaInterface();
}

//! constructor with main map widget defined
MKartaInterface::MKartaInterface(QWidget* widget)
{
    InitKartaInterface();

    if (widget)  {
        m_widget = widget;
        m_karta->pMainMapWidget = widget;
        QSize sz= widget->size();
        m_pixmap = new QPixmap(sz);
        m_pixmapact = new QPixmap(sz);
        CreatePaintDevice();
    }

}

//! constructor with main map widget and preview map widget defined
MKartaInterface::MKartaInterface(QWidget* widget, QWidget* widgetsmall)
{
    InitKartaInterface();

    if (widget)  {
        m_widget = widget;
        m_karta->pMainMapWidget = widget;
        QSize sz= widget->size();
        m_pixmap = new QPixmap(sz);
        m_pixmapact = new QPixmap(sz);
        CreatePaintDevice();
    }

    if (widgetsmall)  {
        m_widgetsmall = widgetsmall;
        QSize sz= widgetsmall->size();
        m_pixmapsmall = new QPixmap(sz);
        m_pixmapactsmall = new QPixmap(sz);
        CreatePaintDeviceSmall();
    }

}

//! ------------------
MKartaInterface::~MKartaInterface()
{
    if(m_karta)   delete m_karta;
    //	if(tmp_karta) delete tmp_karta;
    if(m_kls) delete m_kls;
    if(m_QtDevice)
    {
        delete m_QtDevice;
        m_QtDevice = NULL;
    }
    if(m_Painter)
    {
        m_Painter->end();
        delete m_Painter;
    }
    if(m_PainterSmall)
    {
        m_PainterSmall->end();
        delete m_PainterSmall;
    }

    if (m_pixmap) delete m_pixmap;
    if (m_pixmapact) delete m_pixmapact;
    if (m_pixmapsmall) delete m_pixmapsmall;
    if (m_pixmapactsmall) delete m_pixmapactsmall;

}

//! Common part of all constructors
//! Initiates main variables to NULL & 0
void MKartaInterface::InitKartaInterface()
{
    m_karta = new MKarta();
    //m_karta->  statusText = NULL;
    //tmp_karta = NULL;
    m_zoombounds = m_karta->m_geobounds;
    m_QtDevice = NULL;
    m_kls = new MKlassifikator();
    m_KlsPath = ""; //way to the new classifier - empty

    m_widget = m_widgetsmall = NULL;
    m_karta->pMainMapWidget = NULL;
    m_pixmap = NULL;
    m_pixmapact = NULL;
    m_pixmapsmall = NULL;
    m_pixmapactsmall = NULL;
    m_PaintDevice = m_PaintDeviceSmall = NULL;
    m_Painter = m_PainterSmall = NULL;
    A = C = B1 = B2 = D1 = D2 = 0;

    m_actLayer = m_geoLayer = NULL;
    m_Object = NULL;
    m_pFoundOblist = NULL;
    m_formularname = "";
    m_bDrawCooGrid = false;
    m_bDrawScaleGrid = true;
    m_bDrawMGRSCooGrid = false;
    //

    m_test = false;
    m_pixmap_resizeFlag = true;

}

//! loads the classifier
bool MKartaInterface::SetKlassifikator(MString kls_name)
{
    int iRet;
    if(!m_karta) return false;

    MKlassifikator* new_kls = new MKlassifikator();

    iRet = new_kls->ReadBin(kls_name);
    //if there was an error of reading the file of the classifier
    if (iRet!=0)
    {
        delete new_kls;
        //cout<<"MKartaInterface::SetKlassifikator -(Error Reading Kls #"<<iRet<<endl;
        return false;
    }

    if(m_kls) delete m_kls;

    m_kls  = new_kls;

    m_karta->m_papercolorkod = m_kls->m_klscolorkod;   // vipa 2004.07.20

    m_karta->ConnectKlassifikator(m_kls);

    m_KlsPath = kls_name;
    return true;
}

//! loads the map in an UTP format
bool MKartaInterface::LoadMap(MString map_name)
{
    if(m_karta) delete m_karta;
    m_karta = new MKarta;
    m_actLayer = NULL;
    //set map output window
    m_karta->pMainMapWidget = m_widget;

    m_karta->ReadBin(map_name, NULL);

    if(m_kls) m_karta->ConnectKlassifikator(m_kls);

    m_zoombounds = m_karta->m_geobounds;

    ZoomTo(m_zoombounds);      // 2004.12.29  vipa

    return true;
}

//! establishes 'widget' on which the map is drawn
void MKartaInterface::SetWidget(QWidget* widget)
{
    m_widget = widget;
    m_karta->pMainMapWidget = widget;
    CreatePaintDevice();
}

//! establishes 'widget' on which the small map is drawn
void MKartaInterface::SetWidgetSmall(QWidget* widget)
{
    m_widgetsmall = widget;

    CreatePaintDeviceSmall();
}

//! to draw the map
void MKartaInterface::DrawMap(bool toScreen)
{
    drawCount = 0;
    drawTime =  0;

    if((!m_karta) || (!m_Painter))
    {
        return;
    }

    ResetAllPrivs(m_QtDevice);

    if (m_Painter->isActive())
        m_Painter->end();

    m_Painter->begin(m_pixmapact);

    if (!m_QtDevice) // INDY
        CreateQtDevice();
    else
        m_QtDevice->ClearAllAndInit(NULL);//Painter for QtDevice is NOT changed
    //	MQtDevice* qtDev = new MQtDevice(m_Painter);
    //	MRect logRect(0.0F, 0.0F, m_viewsize.x, m_viewsize.y);
    //	MRect rec(m_zoombounds.left, m_zoombounds.top, m_zoombounds.right, m_zoombounds.bottom);
    //	qtDev->SetCooSys(logRect, rec);

    //QTime time;
    //time.start();
    //
    m_karta->Risovanie(m_QtDevice, this->GetScale());

    m_karta->DrawActBackground(m_QtDevice);

    *m_pixmap = m_pixmapact->copy();


    m_karta->RisovanieAct(/*qtDev*/m_QtDevice);

    if(m_bDrawCooGrid)
    {
        if(m_bDrawMGRSCooGrid)
            DrawMGRSCooGrid(m_QtDevice, m_zoombounds);
        else
            DrawCooGrid(m_QtDevice, m_zoombounds);
    }

    if(m_bDrawScaleGrid)
        DrawScaleGrid(m_QtDevice, m_zoombounds);


    //Show foundbyName object
    if(m_pFoundOblist)
        DrawFoundByNameObject(m_QtDevice);

    if(toScreen)
    {	//bitBlt( m_widget, 0, 0, m_pixmapact);//, 0, 0, m_viewsize.x, m_viewsize.y); //since qt3
        RestoreMapAct();
        //m_widget->repaint();

    }

    //	int cnt= m_karta->GetObjectCount();
    //	int  msc=  time.restart();

    //InternalMessage(QString::number(cnt)+" objects,  painted in  "+QString::number((double)(msc/1000.0))+" sec.");
    ///InternalMessage(QString::number(cnt)+" objects,  painted in  "+QString::number((double)(msc/1000.0))+" sec. (in fact "+QString::number(drawCount)+" objects, XWindow time- "+QString::number(100*drawTime/msc)+"%)");
    //++msc;

    //
    //cout <<"  ( "<<m_karta->GetDrawFilterSize()<<" pixels= "<< m_karta->GetDrawFilterSizeGeo() <<" m )"<< endl;
}

//! to restore the map and to redraw a operational
//
void MKartaInterface::DrawAct(bool toScreen)
{
    if((!m_karta) || (!m_Painter)) return;

    ResetAllPrivs(m_QtDevice);

    if (m_Painter->isActive())
        m_Painter->end();
    //we restore the map
    //bitBlt(m_pixmapact, 0, 0, m_pixmap);//, 0, 0, m_viewsize.x, m_viewsize.y); //since qt3
    *m_pixmapact = m_pixmap->copy();
    m_Painter->begin(m_pixmapact);

    //	MQtDevice* qtDev = new MQtDevice(m_Painter);
    //
    //	MRect logRect(0.0F, 0.0F, m_viewsize.x, m_viewsize.y);
    //	MRect rec(m_zoombounds.left, m_zoombounds.top, m_zoombounds.right, m_zoombounds.bottom);
    //	qtDev->SetCooSys(logRect, rec);

    //we draw a operational
    m_karta->RisovanieAct(m_QtDevice);

    if(m_bDrawCooGrid)
    {
        if(m_bDrawMGRSCooGrid)
            DrawMGRSCooGrid(m_QtDevice, m_zoombounds);
        else
            DrawCooGrid(m_QtDevice, m_zoombounds);
    }
    if(m_bDrawScaleGrid)
        DrawScaleGrid(m_QtDevice, m_zoombounds);


    if(toScreen)
    { //bitBlt( m_widget, 0, 0, m_pixmapact);//, 0, 0, m_viewsize.x, m_viewsize.y); //since qt3
        RestoreMapAct();
        //	m_widget->repaint();
    }
    //    delete qtDev;
}

//! to draw the small map
void MKartaInterface::DrawMapSmall(bool toScreen)
{
    if((!m_karta) || (!m_PainterSmall))
        return;

    if (m_PainterSmall->isActive())
        m_PainterSmall->end();

    m_PainterSmall->begin(m_pixmapactsmall);

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);

    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);

    qtDev->SetCooSys(logRect, rec);

    //
    m_karta->Risovanie(qtDev, this->GetScale(), true);
    //
    //bitBlt(m_pixmapsmall , 0, 0, m_pixmapactsmall ); //since qt3
    *m_pixmapsmall = *m_pixmapactsmall;
    //
    m_karta->RisovanieAct(qtDev, true);

    if(toScreen)
    { //bitBlt( m_widgetsmall, 0, 0, m_pixmapactsmall ); //since qt3
        RestoreMapActSmall();
    }

    delete qtDev;
}

//! creation of the device of drawing
void MKartaInterface::CreatePaintDevice()
{
    if(m_widget && m_pixmap_resizeFlag )
    {
        if(m_Painter)
        {
            if(m_Painter->isActive())
                m_Painter->end();
            delete m_Painter;
        }

        if (m_pixmap) delete m_pixmap;
        if (m_pixmapact) delete m_pixmapact;

        QSize sz = m_widget->size();
        m_pixmap = new QPixmap(sz);
        m_pixmapact = new QPixmap(sz);
        //to put check, whether it turned out to create Pixmaps
        /*...*/

        m_Painter = new QPainter();
        m_Painter->begin(m_pixmapact);

        //	DELETE	QRect w = m_Painter->window();
        //	DELETE	m_Painter->setWindow(int(-w.width()/2), int(w.height()/2), w.width(), -w.height());

        m_viewsize.x = sz.width();
        m_viewsize.y = sz.height();

        //now we create 'QtDevice'
        if (!m_QtDevice)
            CreateQtDevice();
        else
            m_QtDevice->ClearAllAndInit(m_Painter);

        //following with next:-->
        MRect logRect_qt(0.0F, 0.0F, m_viewsize.x- 1.0, m_viewsize.y- 1.0);    // 2004.12.30  vipa

        MRect rec_qt(m_zoombounds.left, m_zoombounds.top, m_zoombounds.right, m_zoombounds.bottom);
        m_QtDevice->SetCooSys(logRect_qt, rec_qt);
        //<--

    }
}

//! ------------------
void MKartaInterface::CreatePaintDeviceSmall()
{
    if(m_widgetsmall)
    {
        if(m_PainterSmall)
        {
            if (m_PainterSmall->isActive())
                m_PainterSmall->end();
            delete m_PainterSmall;
        }

        if (m_pixmapsmall) delete m_pixmapsmall;
        if (m_pixmapactsmall) delete m_pixmapactsmall;

        QSize siz = m_widgetsmall->size();
        m_pixmapsmall = new QPixmap(siz);
        m_pixmapactsmall = new QPixmap(siz);

        m_PainterSmall = new QPainter();
        m_PainterSmall->begin(m_pixmapactsmall);

        //	DELETE	QRect w = m_Painter->window();
        //	DELETE	m_Painter->setWindow(int(-w.width()/2), int(w.height()/2), w.width(), -w.height());

        m_viewsizesmall.x = siz.width();
        m_viewsizesmall.y = siz.height();
    }
}

//! ------------------
bool MKartaInterface::CreateQtDevice()
{
    if(!m_Painter) return false;

    if(m_QtDevice)
    {
        delete m_QtDevice;
        m_QtDevice = NULL;
    }

    m_QtDevice = new MQtDevice(m_Painter);

    //MRect logRect(0.0F, 0.0F, m_viewsize.x, m_viewsize.y); //when WINDOWS -ATTENTION-
    MRect logRect(0.0F, 0.0F, m_viewsize.x- 1.0, m_viewsize.y- 1.0);    // 2004.12.30  vipa

    MRect rec(m_zoombounds.left, m_zoombounds.top, m_zoombounds.right, m_zoombounds.bottom);
    m_QtDevice->SetCooSys(logRect, rec);

    return true;
}

//==================================
//! to restore the map ('pixmap') without operational situation
void MKartaInterface::RestoreMap()
//==================================
{
    if(m_widget)
    {
        if(m_Painter->isActive())
            m_Painter->end();
        m_Painter->begin(m_widget);
        m_Painter->drawPixmap(0,0,*m_pixmap);
        m_Painter->end();
    }
}

//==================================
//! to restore the map ('pixmap') with operational situation
void MKartaInterface::RestoreMapAct()
//==================================
{
    if(m_widget)
    {
        if(m_Painter->isActive())
            m_Painter->end();

        m_Painter->begin(m_widget);
        m_Painter->drawPixmap(0,0,*m_pixmapact);
        m_Painter->end();
    }
}

//==================================
//! to restore the small map ('pixmap') without operational situation
void MKartaInterface::RestoreMapSmall()
//==================================
{
    if(m_widgetsmall)
    { //bitBlt( m_widgetsmall, 0, 0, m_pixmapsmall ); //since qt3
        if(m_PainterSmall->isActive())
            m_PainterSmall->end();
        m_PainterSmall->begin(m_widgetsmall);
        m_PainterSmall->drawPixmap(0,0,*m_pixmapsmall);
        m_PainterSmall->end();

    }
}

//==================================
//! to restore the small map ('pixmap') with operational situation
void MKartaInterface::RestoreMapActSmall()
//==================================
{
    if(m_widgetsmall)
    { //bitBlt( m_widgetsmall, 0, 0, m_pixmapactsmall ); //since qt3
        if(m_PainterSmall->isActive())
            m_PainterSmall->end();
        m_PainterSmall->begin(m_widgetsmall);
        m_PainterSmall->drawPixmap(0,0,*m_pixmapactsmall);
        m_PainterSmall->end();
    }
}

//==================================
//! ------------------
QPoint MKartaInterface::GeoToLogic(const MPoint& gPoint)
//==================================
{
    if((!m_karta) || (!m_Painter)) return QPoint(-10000, -10000);

    QPoint lpt;
    MPoint mPt = ConvertPoint(gPoint);
    m_QtDevice->BumToLogicI(&mPt, lpt);

    return lpt;
}

//==================================
//! ------------------
MPoint MKartaInterface::LogicToGeo(const QPoint& lPoint, bool bInMapCoo)
//==================================
{
    if((!m_karta) || (!m_Painter)) return MPoint(-10000, -10000);

    MPoint gpt;
    MPoint lpt(lPoint.x(), lPoint.y());
    m_QtDevice->LogicToBum(lpt, gpt);

    if(!bInMapCoo)
    {
        return ConvertPoint(gpt, false);
    }
    else return gpt;
}

//==================================
//! logical coordinates in geographical (rectangle)
MRect MKartaInterface::LogicToGeo(const QRect& lrect, bool bInMapCoo)
//==================================
{
    if((!m_karta) || (!m_Painter))
        return MRect(-10000, -10000, 10000, 10000);

    MRect grect;
    MRect lrec(lrect.left(), lrect.top(), lrect.right(), lrect.bottom());
    m_QtDevice->LogicToBum(&lrec, grect);

    if(!bInMapCoo)
    {
        return MRect(ConvertPoint(MPoint(grect.left, grect.top), false),
                     ConvertPoint(MPoint(grect.right, grect.bottom), false));
    }

    return grect;
}

//==================================
//! geographical coordinates in logical (rectangle)
QRect MKartaInterface::GeoToLogic(const MRect& grect)
//==================================
{
    if((!m_karta) || (!m_Painter))
        return QRect(QPoint(-10000, -10000), QPoint(10000, 10000));

    QRect lrect;

    MRect grec(ConvertPoint(MPoint(grect.left, grect.top)),
               ConvertPoint(MPoint(grect.right, grect.bottom)));

    m_QtDevice->BumToLogicI(&grec, lrect);

    return lrect;
}

//==================================
//! geographical coordinates in logical (rectangle)
QRect MKartaInterface::GeoToSCR(const MRect& grect)
//==================================
{
    if((!m_karta) || (!m_Painter)) return QRect(QPoint(-10000, -10000), QPoint(10000, 10000));;

    QRect lrect;
    
    MPoint pt1(grect.left, grect.top);
    MPoint pt2(grect.right, grect.bottom);

    //  pt1 = ConvertPoint(pt1, false);
    //  pt2 = ConvertPoint(pt2, false);

    MRect grec(pt1.x, pt1.y, pt2.x, pt2.y);

    //  m_QtDevice->BumToLogicI(&grec, lrect);

    BumToLogicI(&grec, lrect);
    //delete qtDev;
    return lrect;
}
//==================================
//! value in logical coordinates in the geographical
double MKartaInterface::LogicToGeo( int iVal)
//==================================
{
    if((!m_karta) || (!m_Painter)) return 0;

    double d = m_QtDevice->LogicToBum((double)iVal);

    return d;
}

//==================================
//! value in geographical coordinates in the logical
int MKartaInterface::GeoToLogic( double dVal)
//==================================
{
    if((!m_karta) || (!m_Painter)) return 0;

    int i = m_QtDevice->BumToLogicI(dVal);

    return i;
}

//==================================
//! geographical coordinates in logical for the small map
QPoint MKartaInterface::GeoToLogicSmall(MPoint& gPoint)
//==================================
{
    if((!m_karta) || (!m_PainterSmall)) return QPoint(-10000, -10000);

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);
    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);
    qtDev->SetCooSys(logRect, rec);

    QPoint lpt;
    MPoint mPt = ConvertPoint(gPoint);
    qtDev->BumToLogicI(&mPt, lpt);

    delete qtDev;
    return lpt;
}


//==================================
//! logical coordinates in geographical for the small map
MPoint MKartaInterface::LogicToGeoSmall(QPoint& lPoint, bool bInMapCoo)
//==================================
{
    if((!m_karta) || (!m_PainterSmall)) return MPoint(-10000, -10000);

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);
    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);
    qtDev->SetCooSys(logRect, rec);

    MPoint gpt;
    MPoint lpt(lPoint.x(), lPoint.y());
    qtDev->LogicToBum(lpt, gpt);

    delete qtDev;

    if(!bInMapCoo)
    {
        return ConvertPoint(gpt, false);
    }
    else return gpt;
}

//==================================
//! logical coordinates in geographical for the small map (rectangle)
const MRect MKartaInterface::LogicToGeoSmall(const QRect& lrect, bool bInMapCoo)
//==================================
{
    if((!m_karta) || (!m_PainterSmall)) return MRect(-10000, -10000, 10000, 10000);

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);
    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);
    qtDev->SetCooSys(logRect, rec);

    MRect grect;
    MRect lrec(lrect.left(), lrect.top(), lrect.right(), lrect.bottom());
    qtDev->LogicToBum(&lrec, grect);

    delete qtDev;

    if(!bInMapCoo)
    {
        MPoint pt1(grect.left, grect.top);
        MPoint pt2(grect.right, grect.bottom);
        pt1 = ConvertPoint(pt1, false);
        pt2 = ConvertPoint(pt2, false);
        return MRect(pt1.x, pt1.y, pt2.x, pt2.y);
    }

    return grect;
}

//==================================
//! geographical coordinates in logical for the small map (rectangle)
QRect MKartaInterface::GeoToLogicSmall(const MRect& grect)
//==================================
{
    if((!m_karta) || (!m_PainterSmall)) return QRect(QPoint(-10000, -10000), QPoint(10000, 10000));;

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);
    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);
    qtDev->SetCooSys(logRect, rec);

    QRect lrect;
    
    MPoint pt1(grect.left, grect.top);
    MPoint pt2(grect.right, grect.bottom);

    pt1 = ConvertPoint(pt1);
    pt2 = ConvertPoint(pt2);

    MRect grec(pt1.x, pt1.y, pt2.x, pt2.y);

    qtDev->BumToLogicI(&grec, lrect);

    delete qtDev;
    return lrect;
}

//==================================
//! value in logical coordinates in geographical for the small map
double MKartaInterface::LogicToGeoSmall( int iVal)
//==================================
{
    if((!m_karta) || (!m_PainterSmall)) return 0;

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);
    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);
    qtDev->SetCooSys(logRect, rec);

    double d = qtDev->LogicToBum((double)iVal);
    delete qtDev;

    return d;
}

//==================================
//! value in geographical coordinates in logical for the small map
int MKartaInterface::GeoToLogicSmall( double dVal)
//==================================
{
    if((!m_karta) || (!m_PainterSmall)) return 0;

    MQtDevice* qtDev = new MQtDevice(m_PainterSmall);

    MRect logRect(0.0F, 0.0F, m_viewsizesmall.x, m_viewsizesmall.y);
    MRect rec(m_karta->m_geobounds.left, m_karta->m_geobounds.top,
              m_karta->m_geobounds.right, m_karta->m_geobounds.bottom);
    qtDev->SetCooSys(logRect, rec);

    int i = qtDev->BumToLogicI(dVal);
    delete qtDev;

    return i;
}

//==================================
//! --------------------- 
MPoint MKartaInterface::GeoToGauss(MPoint& pt, double meredian)
//==================================
{
    MPoint gauss_pt;
    MZoneGaussKrugerCoo coo;

    //  int zone;
    //  if(meredian == 1000) meredian = pt.x;
    if(meredian == 1000)   meredian = centralMeridian;
    /*{
      meredian = pt.x;
      if(m_karta)
          if(m_karta->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC)
          {
            MRect bounds = m_karta->GetGeoBounds();
            meredian = bounds.left+((bounds.right-bounds.left)/2);
          }
          else
            meredian = ((MExtendedGaussKrugerCoo*)m_karta->m_pObjectCoo)->m_meridian;
  }*/

    if (meredian == 360) meredian = 0;
    //  if (meredian >= 0) zone = (int)dbl_to_int((meredian - 3.0)/6.0 + 1.5);
    //  else zone = (int)dbl_to_int((360.0  + meredian + 3.0)/6.0);

    coo.SetBasicParameters(meredian);
    //coo.SetBasicParameters(meredian);

    double oldh, newh;
    oldh = newh = 0;
    coo.FromBLH(pt, oldh, gauss_pt, newh);
    return gauss_pt;
}

//==================================
//! ---------------------
MPoint MKartaInterface::GaussToGeo(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    MZoneGaussKrugerCoo coo;

    if(meredian == 1000)   meredian = centralMeridian;
    /*{
      if(m_karta)
          if(m_karta->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC)
          {
            MRect bounds = m_karta->GetGeoBounds();
            meredian = bounds.left+((bounds.right-bounds.left)/2);
          }
          else
            meredian = ((MExtendedGaussKrugerCoo*)m_karta->m_pObjectCoo)->m_meridian;
  }*/

    int zone;
    if (meredian ==360) meredian = 0;
    if (meredian >=0) zone = (int)dbl_to_int((meredian - 3.0)/6.0 + 1.0);
    else zone = (int)dbl_to_int((360.0  + meredian + 3.0)/6.0);

    coo.SetZone(zone);

    double oldh, newh;
    oldh = newh = 0;
    coo.ToBLH(pt, oldh, geo_pt, newh);

    return geo_pt;
}

//==================================
//! --------------------------- 
MPoint MKartaInterface::GeoToGaussStandart(MPoint& pt, double meredian)
//==================================
{
    MPoint gauss_pt;
    MStandartGaussKrugerCoo coo;

    int zone;

    if(meredian == 1000)	meredian = centralMeridian;
    /*{
      meredian = pt.x;
      if(m_karta)
          if(m_karta->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC)
          {
            MRect bounds = m_karta->GetGeoBounds();
            meredian = bounds.left+((bounds.right-bounds.left)/2);
          }
          else
            meredian = ((MExtendedGaussKrugerCoo*)m_karta->m_pObjectCoo)->m_meridian;
  }*/
    if (meredian > 0) zone = (int)((meredian - 3)/6 + 1.5);
    else zone = (int)((360  + meredian + 3)/6);

    coo.SetZone(zone);

    double oldh, newh;
    oldh = newh = 0;
    coo.FromBLH(pt, oldh, gauss_pt, newh);
    return gauss_pt;
}

//==================================
//! -------------------------
MPoint MKartaInterface::GaussStandartToGeo(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    MStandartGaussKrugerCoo coo;

    //  if(meredian == 1000)   meredian = centralMeridian;
    /*{
      if(m_karta)
          if(m_karta->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC)
          {
            MRect bounds = m_karta->GetGeoBounds();
            meredian = bounds.left+((bounds.right-bounds.left)/2);
          }
          else
            meredian = ((MExtendedGaussKrugerCoo*)m_karta->m_pObjectCoo)->m_meridian;
  }*/
    meredian = centralMeridian;

    int zone;
    if (meredian > 0) zone = (int)((meredian - 3)/6 + 1);
    else zone = (int)((360  + meredian + 3)/6);

    //coo.SetBasicParameters(meredian);
    coo.SetZone(zone);
    //coo.SetReper(m_reper);

    double oldh, newh;
    oldh = newh = 0;
    coo.ToBLH(pt, oldh, geo_pt, newh);
    return geo_pt;
}

//==================================
//! ----------------- 
MPoint MKartaInterface::GeoToGaussLocal(MPoint& pt)
//==================================
{
    MPoint gauss_pt;
    MPsevdoGaussKrugerCoo coo;

    int meredian = int(m_reper.x/1000000)*6-3;

    coo.SetBasicParameters(meredian);

    coo.m_havelink = true;

    MPoint temp_reper(m_reper.x - int(m_reper.x/1000000)*1000000, m_reper.y);

    coo.SetReper(temp_reper);
    //coo.SetReper(m_reper);
    coo.SetRotation(0.0);
    double oldh, newh;
    oldh = newh = 0;
    coo.FromBLH(pt, oldh, gauss_pt, newh);
    return gauss_pt;
}

//==================================
//! ----------------- 
MPoint MKartaInterface::GeoToGaussLocalTrueM(MPoint& pt)
//==================================
{
    //////////////////////////////////////////////////////
    double az = ConvertDegreesToRadians(GetAzimut(m_reper,pt));
    double ds = GetDistance(m_reper,pt);
    double x_loc = ds*sin(az);
    double y_loc = ds*cos(az);
    return MPoint(x_loc,y_loc);
    /////////////////////////////////////////////////////
    MPoint gauss_pt;
    MPsevdoGaussKrugerCoo coo;

    int meredian = int(m_reper.x/1000000)*6-3;

    coo.SetBasicParameters(meredian);

    coo.m_havelink = true;

    MPoint temp_reper(m_reper.x - int(m_reper.x/1000000)*1000000, m_reper.y);
    double gamma = GaussConvergence(m_reper,(int)(coo.m_meridian/6.0)+1);
    coo.SetReper(temp_reper);
    //coo.SetReper(m_reper);
    MPoint ptgeo = GaussStandartToGeo(m_reper);
    // double ppi = 4*atan((double)1);
    // double gamma = (coo.m_meridian - ptgeo.x)*sin(ptgeo.y*2*ppi/360);
    coo.SetRotation(gamma);
    double oldh, newh;
    oldh = newh = 0;
    coo.FromBLH(pt, oldh, gauss_pt, newh);
    return gauss_pt;
}

//==================================
//! -----------------------
MPoint MKartaInterface::GaussLocalToGeo(MPoint& pt)
//==================================
{
    MPoint geo_pt;
    MPsevdoGaussKrugerCoo coo;

    int meredian = int(m_reper.x/1000000)*6-3;

    coo.SetBasicParameters(meredian);
    coo.m_havelink = true;

    MPoint temp_reper(m_reper.x - int(m_reper.x/1000000)*1000000, m_reper.y);

    coo.SetReper(temp_reper);
    coo.SetRotation(0.0);
    double oldh, newh;
    oldh = newh = 0;
    coo.ToBLH(pt, oldh, geo_pt, newh);
    return geo_pt;
}

//==================================
//! -----------------------
MPoint MKartaInterface::GaussLocalTrueMToGeo(MPoint& pt)
//==================================
{
    ////////////////////////////////////////////////////////////////////////////////////////
    double az = ConvertRadiansToDegrees(atan(pt.x/pt.y));
    return GetPointByAzimuthLength(m_reper,az,sqrt(pow(pt.x,2)+pow(pt.y,2)));
    ////////////////////////////////////////////////////////////////////////////////////////
    MPoint geo_pt;
    MPsevdoGaussKrugerCoo coo;

    int meredian = int(m_reper.x/1000000)*6-3;

    coo.SetBasicParameters(meredian);
    coo.m_havelink = true;

    MPoint temp_reper(m_reper.x - int(m_reper.x/1000000)*1000000, m_reper.y);

    coo.SetReper(temp_reper);
    MPoint ptgeo = GaussStandartToGeo(m_reper);
    double ppi = 4*atan((double)1);
    double gamma = (coo.m_meridian - ptgeo.x)*sin(ptgeo.y*2*ppi/360);
    coo.SetRotation(gamma);
    double oldh, newh;
    oldh = newh = 0;
    coo.ToBLH(pt, oldh, geo_pt, newh);
    return geo_pt;
}


//==================================
//! ------------------
MPoint MKartaInterface::GaussToGaussLocal(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    geo_pt = GaussToGeo(pt, meredian);
    return GeoToGaussLocal(geo_pt);
}

//==================================
//! ------------------
MPoint MKartaInterface::GaussLocalToGauss(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    geo_pt = GaussLocalToGeo(pt);
    return GeoToGauss(geo_pt, meredian);
}

//==================================
//! ------------------
MPoint MKartaInterface::GaussToGaussStandart(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    geo_pt = GaussToGeo(pt, meredian);
    return GeoToGaussStandart(geo_pt, meredian);
}

//==================================
//! ------------------
MPoint MKartaInterface::GaussStandartToGauss(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    geo_pt = GaussStandartToGeo(pt, meredian);
    return GeoToGauss(geo_pt, meredian);
}


//==================================
//! ------------------
MPoint MKartaInterface::GaussStandartToGaussLocal(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    geo_pt = GaussStandartToGeo(pt, meredian);
    return GeoToGaussLocal(geo_pt);
}

//==================================
//! ------------------
MPoint MKartaInterface::GaussLocalToGaussStandart(MPoint& pt, double meredian)
//==================================
{
    MPoint geo_pt;
    geo_pt = GaussLocalToGeo(pt);
    return GeoToGaussStandart(geo_pt, meredian);
}
//==================================
//! ------------------
MString MKartaInterface::GeoToGeoref(MPoint pt, int precision )
{
    return GEOREF::convertFromGeo(pt, precision);
}

//==================================
//! ------------------
MPoint MKartaInterface::GeorefToGeo(MString georef)
{
    return GEOREF::convertToGeo(georef);
}

//==================================
//! ------------------
MPoint MKartaInterface::GeoToUtm(MPoint geoPt, int &zone)
{
    MUtmCoo utm;
    utm.SetStandartEllipsoid(m_karta->m_pObjectCoo->GetEllipsoidName());
    MPoint utmPt;
    double hgf;
    utm.FromBLH(geoPt,0,utmPt,hgf);
    zone  = (utm.m_meridian+183)/6;
    utmPt.x -= 1000000.0*zone;
    return utmPt;
}

//==================================
//! ------------------
MString MKartaInterface::GeoToUtmText(MPoint geoPt)
{
    if (geoPt.y<-80 || geoPt.y>84)
        return "";
    char latBand[] = "CDEFGHJKLMNPQRSTUVWX";//excluding I and O
    char utmString[23];
    int ilat = int(floor(geoPt.y));
    int latZone = (std::max)(0, (std::min)(19, (ilat + 80)/8));
    int lngZone;
    MPoint utm = GeoToUtm(geoPt,lngZone);
    sprintf(utmString,"%.2i%c %.6imE %.7imN",lngZone,latBand[latZone],int(floor(utm.x)),int(floor(utm.y)));
    return utmString;
}

//==================================
//! ------------------
MPoint MKartaInterface::UtmToGeo(MPoint utmPt, int zone, bool nPole)
{
    MUtmCoo utm;
    utm.SetStandartEllipsoid(m_karta->m_pObjectCoo->GetEllipsoidName());
    MPoint geoPt;
    double hgf;
    utmPt.x += zone*1000000.0;
    utm.nPole = nPole;
    utm.ToBLH(utmPt,0,geoPt,hgf);
    return geoPt;
}

//==================================
//!returns Gaussian rapprochement of meridians in degrees concerning the zone 'zone' or concerning "its" zone if 'zone' isn't set or equal 0
double MKartaInterface::GaussConvergence(const MPoint& pt, int zone )
{

    MPoint geoPt = ConvertPoint(pt,false);
    double l0;
    if (zone==0)
        if ((geoPt.x >=0)&&(geoPt.x <180.0))
            l0 = (int)(geoPt.x/6)*6.0+3.0;
        else
            l0 = (int)(geoPt.x/6)*6.0-3.0;
    else
        if ((zone>0)&&(zone <=30))
            l0 = zone*6.0-3.0;
        else
            l0 = zone*6.0-3.0-360;

    double l0rad = l0*pi/180.0;
    double lrad = geoPt.x*pi/180.0;
    double brad = geoPt.y*pi/180.0;
    double dL = lrad-l0rad;


    double e2 = (pow(axis_X,2)-pow(axis_Y,2))/pow(axis_Y,2);

    double c1 = sin(brad);
    double c3 = sin(brad)*pow(cos(brad),2)*(1+3*e2*pow(cos(brad),2))/3.0;
    double c5 = sin(brad)*pow(cos(brad),2)*(2-pow(tan(brad),2))/15.0;
    double alph =c1*dL + c3*pow(dL,3)+c5*pow(dL,5);
    return alph*180/pi;
}

//==================================
//! ------------------
void MKartaInterface::ZoomTo(const MRect& rect)
//==================================
{
    if(!m_karta)
        return;

    MPoint ptLT(rect.left,  rect.top);
    MPoint ptRB(rect.right, rect.bottom);

    MPoint pt1 = ConvertPoint(MPoint(rect.left, rect.top));
    MPoint pt2 = ConvertPoint(MPoint(rect.right, rect.bottom));

    m_zoombounds.left = pt1.x;
    m_zoombounds.top = pt1.y;

    m_zoombounds.right = pt2.x;
    m_zoombounds.bottom = pt2.y;

    m_zoombounds.NormalizeGeoRect();

    //проверим пустой ли зум в инишке
    bool isEmptyZoomRect = (rect.Width() <= 0.0 ||
                            rect.Height() <= 0.0);

    if(!isEmptyZoomRect)
    {
        if (m_zoombounds.left < m_karta->m_geobounds.left)
        {
            m_zoombounds.left = m_karta->m_geobounds.left;
        }

        if (m_zoombounds.right > m_karta->m_geobounds.right)
        {
            m_zoombounds.right = m_karta->m_geobounds.right;
        }

        if (m_zoombounds.top > m_karta->m_geobounds.top)
        {
            m_zoombounds.top = m_karta->m_geobounds.top;
        }

        if (m_zoombounds.bottom < m_karta->m_geobounds.bottom)
        {
            m_zoombounds.bottom = m_karta->m_geobounds.bottom;
        }
    }
    else
    {
        m_zoombounds = m_karta->m_geobounds;

        ptLT = MPoint(m_zoombounds.left, m_zoombounds.top);
        ptRB = MPoint(m_zoombounds.right, m_zoombounds.bottom);
    }

    if ((fabs(ptRB.x - ptLT.x) < zoomGeoLimit) &&
            (fabs(ptLT.y - ptRB.y) < zoomGeoLimit))
    {
        double dx = zoomGeoLimit / 2.0;

        m_zoombounds = MRect(ConvertPoint(MPoint((ptLT.x + ptRB.x) /
                                                 2.0 - dx, (ptLT.y + ptRB.y) /
                                                 2.0 + 0.005)),
                             ConvertPoint(MPoint((ptLT.x + ptRB.x) /
                                                 2.0 + dx,(ptLT.y + ptRB.y) /
                                                 2.0 - 0.005)));
    }
    //----- end vipa

    m_zoombounds.NormalizeGeoRect();

    if (!m_QtDevice)
        CreateQtDevice();
    else
        m_QtDevice->ClearAllAndInit(NULL); //painter for QtDevice is not changed
    //following with next:-->
    MRect logRect_qt(0.0F, 0.0F, m_viewsize.x - 1.0, m_viewsize.y - 1.0);    // 2004.12.30  vipa

    MRect rec_qt(m_zoombounds.left,
                 m_zoombounds.top,
                 m_zoombounds.right,
                 m_zoombounds.bottom);

    m_QtDevice->SetCooSys(logRect_qt, rec_qt);
    //<--

    //-ATTENTION-
    //QRect lRect(0, 0, (int)m_viewsize.x, (int)m_viewsize.y); //  2004.12.30  vipa
    int ix = ((int)m_viewsize.x % 2) ? ((int)m_viewsize.x) :  ((int)m_viewsize.x - 1);
    int iy = ((int)m_viewsize.y % 2) ? ((int)m_viewsize.y) :  ((int)m_viewsize.y -1);
    QRect lRect(0, 0, ix, iy);

    m_zoombounds = LogicToGeo(lRect, true);
    m_zoombounds.NormalizeGeoRect();

    //CreateQtDevice(); - replacing with next:-->
    MRect logRect_nqt(0.0F, 0.0F, m_viewsize.x - 1.0, m_viewsize.y - 1.0);    // 2004.12.30  vipa

    MPoint p1(m_zoombounds.left, m_zoombounds.top);
    MPoint p2(m_zoombounds.right, m_zoombounds.bottom);
    //MRect rec_nqt(m_zoombounds.left, m_zoombounds.top, m_zoombounds.right, m_zoombounds.bottom);
    MRect rec_nqt(ConvertPoint(p1), ConvertPoint(p2));
    m_QtDevice->SetCooSys(logRect_nqt, rec_nqt);
    //<--
}

//==================================
//! ------------------
void MKartaInterface::ScrollByGeo(double x, double y)
//==================================
{
    if(!m_karta) return;
    MPoint pt(x,y);
    pt = ConvertPoint(pt);
    MRect rect = m_zoombounds;
    m_zoombounds += pt;

    // if there is an attempt to be removed further, than allow
    // 'geobounds',
    if (m_zoombounds.left<m_karta->m_geobounds.left)
        m_zoombounds.left = m_karta->m_geobounds.left;
    if (m_zoombounds.right>m_karta->m_geobounds.right)
        m_zoombounds.right = m_karta->m_geobounds.right;
    if (m_zoombounds.top>m_karta->m_geobounds.top)
        m_zoombounds.top = m_karta->m_geobounds.top;
    if (m_zoombounds.bottom<m_karta->m_geobounds.bottom)
        m_zoombounds.bottom = m_karta->m_geobounds.bottom;
    //	if (m_zoombounds.left<m_karta->m_geobounds.left)
    //	{
    //		m_zoombounds.left = m_karta->m_geobounds.left;
    //		m_zoombounds.right = m_karta->m_geobounds.left+rect.Width();
    //	}
    //	if (m_zoombounds.right>m_karta->m_geobounds.right)
    //	{
    //		m_zoombounds.right = m_karta->m_geobounds.right;
    //		m_zoombounds.left = m_karta->m_geobounds.right-rect.Width();
    //	}
    //	if (m_zoombounds.top>m_karta->m_geobounds.top)
    //	{
    //		m_zoombounds.top = m_karta->m_geobounds.top;
    //		m_zoombounds.bottom = m_karta->m_geobounds.top - rect.Height();
    //	}
    //	if (m_zoombounds.bottom<m_karta->m_geobounds.bottom)
    //	{
    //		m_zoombounds.bottom = m_karta->m_geobounds.bottom;
    //		m_zoombounds.top = m_karta->m_geobounds.bottom + rect.Height();
    //	}

    m_zoombounds.NormalizeGeoRect();
    ZoomTo(m_zoombounds);
}

//==================================
//! ------------------                 
void MKartaInterface::ScrollByLogic(int x, int y)
//==================================
{
    if(!m_karta)
        return;

    QRect lrec = GeoToLogic(m_zoombounds);
    lrec.translate(-x, -y);

    MRect grec;
    grec = LogicToGeo(lrec, true);

    // if there is an attempt to be removed further, than allow
    // 'geobounds',
    if (grec.left < m_karta->m_geobounds.left)
    {
        grec.left = m_karta->m_geobounds.left;
        grec.right = m_karta->m_geobounds.left+m_zoombounds.Width();
    }

    if (grec.right > m_karta->m_geobounds.right)
    {
        grec.right = m_karta->m_geobounds.right;
        grec.left = m_karta->m_geobounds.right-m_zoombounds.Width();
    }

    if (grec.top > m_karta->m_geobounds.top)
    {
        grec.top = m_karta->m_geobounds.top;
        grec.bottom = m_karta->m_geobounds.top - m_zoombounds.Height();
    }

    if (grec.bottom < m_karta->m_geobounds.bottom)
    {
        grec.bottom = m_karta->m_geobounds.bottom;
        grec.top = m_karta->m_geobounds.bottom + m_zoombounds.Height();
    }

    m_zoombounds = grec;
    m_zoombounds.NormalizeGeoRect();
    ZoomTo(m_zoombounds);
}

//==================================
//!to add an operational layer
bool MKartaInterface::AddActLayer(double uid, MLayer* pLay)
//==================================
{
    if(!m_karta) return false;
    if (FindActLayer(uid))
        return false;

    //if the pointer isn't transferred to a layer, we create the new
    if(!pLay)
    {
        MLayer* pLayer = new MLayer();
        pLayer->m_UID = uid;
        pLayer->m_bOperLayer = true;
        m_karta->AddActLayer(pLayer);
    }
    else m_karta->AddActLayer(pLay);

    return true;
}

//==================================
//!to find an operational layer
MLayer* MKartaInterface::FindActLayer(double uid) const
//==================================
{
    if(!m_karta) return NULL;

    MLayer* pLay = m_karta->FindActLayer(uid);

    return pLay;
}

//==================================
//!to find an operational layer
double MKartaInterface::FindActLayer(MLayer* pLay)
//==================================
{
    if(!m_karta) return -1.0;

    double uid = pLay->m_UID;

    MLayer* play = m_karta->FindActLayer(uid);
    if(!play) uid = 0;

    return uid;
}

//==================================
//!to establish a working operational layer
void MKartaInterface::SetWorkingActLayer(double uid)
//==================================
{
    m_actLayer = FindActLayer(uid);
    m_Object = NULL;
}

//==================================
//!to receive a working operational layer
double MKartaInterface::GetWorkingActLayer()
//==================================
{
    return m_actLayer->m_UID;
}

//==================================
//!to delete an operational layer
bool MKartaInterface::DeleteActLayer(double uid)
//==================================
{
    if(!m_karta) return false;

    m_karta->RemoveActLayer(FindActLayer(uid));

    return true;
}

//==================================
//!to add a geographical layer
bool MKartaInterface::AddGeoLayer(double, MLayer* ) //(double uid, MLayer* pLay)
//==================================
{
    return true;
}

//==================================
//!to find a geographical layer
MLayer* MKartaInterface::FindGeoLayer(double ) //(double uid)
//==================================
{
    return NULL;
}

//==================================
//!to find a geographical layer
double MKartaInterface::FindGeoLayer(MLayer* ) //(MLayer* pLay)
//==================================
{
    return 0;
}

//==================================
//!to set a geographical layer
void MKartaInterface::SetWorkingGeoLayer(double ) //(double uid)
//==================================
{
}

//==================================
//!to get a geographical layer
double MKartaInterface::GetWorkingGeoLayer()
//==================================
{
    return 0;
}

//==================================
//!to delete a geographical layer
bool MKartaInterface::DeleteGeoLayer(double ) //(double uid)
//==================================
{
    return true;
}

//==================================
//!to establish visibility of a geographical layer
bool MKartaInterface::SetGeoLayerVisible(bool bVisible)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->SetVisible(bVisible);
    return true;
}

//==================================
//!to establish visibility of a operational layer
bool MKartaInterface::SetActLayerVisible(bool bVisible)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->SetVisible(bVisible);
    return true;
}

//==================================
//!to establish visibility of a operational layer
bool MKartaInterface::SetActLayerVisible(double uid, bool bVisible)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->SetVisible(bVisible);
    return true;
}

//==================================
//!to set a flag of use/non-use of characteristics of a layer when drawing a geographical layer
bool MKartaInterface::SetGeoLayerCommon(bool bCommon)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->SetCommon(bCommon);
    return true;
}

//==================================
//!to set a flag of use/non-use of characteristics of a layer when drawing a operational layer
bool MKartaInterface::SetActLayerCommon(bool bCommon)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->SetCommon(bCommon);
    return true;
}

//==================================
//!to set a flag of use/non-use of characteristics of a layer when drawing a operational layer
bool MKartaInterface::SetActLayerCommon(double uid, bool bCommon)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->SetCommon(bCommon);
    return true;
}

//==================================
//!to set color of the line of a geographical layer
bool MKartaInterface::SetGeoLayerCommonColorPen(MColor commonColor)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->m_commoncolor_pen = commonColor;
    return true;
}

//==================================
//!to set color of the line of a operational layer
bool MKartaInterface::SetActLayerCommonColorPen(MColor commonColor)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->m_commoncolor_pen = commonColor;
    return true;
}

//==================================
//!to set color of the line of a operational layer
bool MKartaInterface::SetActLayerCommonColorPen(double uid, MColor commonColor)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->m_commoncolor_pen = commonColor;
    return true;
}

//==================================
//!to set color of filling of a geographical layer
bool MKartaInterface::SetGeoLayerCommonColorBrush(MColor commonColor)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->m_commoncolor_brush = commonColor;
    return true;
}

//==================================
//!to set color of filling of a operational layer
bool MKartaInterface::SetActLayerCommonColorBrush(MColor commonColor)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->m_commoncolor_brush = commonColor;
    return true;
}

//==================================
//!to set color of filling of a operational layer
bool MKartaInterface::SetActLayerCommonColorBrush(double uid, MColor commonColor)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->m_commoncolor_brush = commonColor;
    return true;
}

//==================================
//!to set color of the text of a geographical layer
bool MKartaInterface::SetGeoLayerCommonColorText(MColor commonColor)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->m_commoncolor_text = commonColor;
    return true;
}

//==================================
//!to set color of the text of a operational layer
bool MKartaInterface::SetActLayerCommonColorText(MColor commonColor)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->m_commoncolor_text = commonColor;
    return true;
}

//==================================
//!to set color of the text of a operational layer
bool MKartaInterface::SetActLayerCommonColorText(double uid, MColor commonColor)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->m_commoncolor_text = commonColor;
    return true;
}

//==================================
//!to set line thickness (in logical units) a geographical layer
bool MKartaInterface::SetGeoLayerCommonPenWidth(int iPenWidth)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->m_commonpenwidth = iPenWidth;
    return true;
}

//==================================
//!to set line thickness (in logical units) a operational layer
bool MKartaInterface::SetActLayerCommonPenWidth(int iPenWidth)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->m_commonpenwidth = iPenWidth;
    return true;
}

//==================================
//!to set line thickness (in logical units) a operational layer
bool MKartaInterface::SetActLayerCommonPenWidth(double uid, int iPenWidth)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->m_commonpenwidth = iPenWidth;
    return true;
}

//==================================
//!to set the text size (in logical units) geographical layer
bool MKartaInterface::SetGeoLayerCommonTextWidth(int iTextWidth)
//==================================
{
    if(!CheckWorkingGeoLayer()) return false;
    m_geoLayer->m_commontextwidth = iTextWidth;
    return true;
}

//==================================
//!to set the text size (in logical units) operational layer
bool MKartaInterface::SetActLayerCommonTextWidth(int iTextWidth)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->m_commontextwidth = iTextWidth;
    return true;
}

//==================================
//!to set the text size (in logical units) operational layer
bool MKartaInterface::SetActLayerCommonTextWidth(double uid, int iTextWidth)
//==================================
{
    MLayer* play = FindActLayer(uid);
    if(!play) return false;

    play->m_commontextwidth = iTextWidth;
    return true;
}

//==================================
//!to add object to a working operational layer
bool MKartaInterface::AddObject(double uid, MTopobject* pObject)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    if(!pObject)
    {
        //if the object with such identifier already exists, we leave
        if(FindObject(uid)) return false;

        MTopobject* pobj = new MTopobject();
        pobj->m_UID = uid;
        pobj->SetTransType(7);
        m_actLayer->AddObject(pobj);
    }
    else
    {
        //if the object with such identifier already exists, we leave
        if(FindObject(pObject->m_UID)) return false;

        m_actLayer->AddObject(pObject);
    }
    return true;
}

//==================================
//!to find and receive object from a working operational layer
MTopobject* MKartaInterface::FindObject(double uid)
//==================================
{
    if(!CheckWorkingActLayer()) return NULL;
    return (MTopobject*)m_actLayer->FindObject(uid);
}

//==================================
//!to find object in all operational layers
bool MKartaInterface::FindObjectAtLayers(double uid)
//==================================
{
    if(!m_karta) return false;
    return m_karta->FindObjectAtActLayers(uid);
}

//==================================
//!to find and receive the list of the objects in a working operational layer which are in the set point
int MKartaInterface::FindObjectsAtPoint(MPoint& point, MObjectList* objList)
//==================================
{
    if(!CheckWorkingActLayer()) return 0;
    m_actLayer->FindObjectsAtPoint(ConvertPoint(point), objList);
    return objList->size();
}

//==================================
//! -----------------
int MKartaInterface::FindObjectsAtPoint(MPoint& point, vector<double>* vect)
//==================================
{
    if(!CheckWorkingActLayer()) return 0;
    MObjectList* objList = new MObjectList;
    m_actLayer->FindObjectsAtPoint(ConvertPoint(point), objList);

    OBITER it = objList->begin();
    MObject* pObj;

    while(it != objList->end())
    {
        //
        pObj = *it;
        vect->push_back(pObj->m_UID);
        it++;
    }
    objList->clear();
    delete objList;

    return vect->size();
}

//==================================
//! to find and receive the list of the objects in a working operational layer which are in the set point
int MKartaInterface::FindObjectsAtPointExact(MPoint& point, MObjectList* objList, int delta)
//==================================
{
    if(!CheckWorkingActLayer()) return 0;

    m_actLayer->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));
    return objList->size();
}

//==================================
//! to allocate all objects in the set point
int MKartaInterface::SelectObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta)
//==================================
{
    if(!m_karta) return 0;
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        MObjectList* objList = new MObjectList;
        (*itl)->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));

        OBITER it = objList->begin();

        while(it != objList->end())
        {
            //
            FindObjectStruct fobj;

            fobj.m_LayUID = (*itl)->m_UID;
            fobj.m_ObjUID = (*it)->m_UID;
            (*it)->Select();

            vect->push_back(fobj);

            it++;
        }
        objList->clear();
        delete objList;

        itl++;
    }

    return vect->size();
}

//==================================
//!to remove allocation for all objects in the set point
int MKartaInterface::DeselectObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta)
//==================================
{
    if(!m_karta) return 0;
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        MObjectList* objList = new MObjectList;
        (*itl)->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));

        OBITER it = objList->begin();

        while(it != objList->end())
        {
            //
            FindObjectStruct fobj;

            fobj.m_LayUID = (*itl)->m_UID;
            fobj.m_ObjUID = (*it)->m_UID;
            (*it)->Deselect();

            vect->push_back(fobj);

            it++;
        }
        objList->clear();
        delete objList;

        itl++;
    }

    return vect->size();
}

//==================================
//! to remove allocation for all objects
bool MKartaInterface::DeselectAll()
//==================================
{
    bool result = false;
    LAYITER layit = m_karta->m_laylist.begin();
    while (layit != m_karta->m_laylist.end())
    {
        OBITER obit = (*layit)->m_oblist.begin();
        while(obit != (*layit)->m_oblist.end())
        {
            if ((*obit)->m_selected) result = true;
            (*obit)->Deselect();
            obit++;
        }
        layit++;
    }
    layit = m_karta->m_actlaylist.begin();
    while (layit != m_karta->m_actlaylist.end())
    {
        OBITER obit = (*layit)->m_oblist.begin();
        while(obit != (*layit)->m_oblist.end())
        {
            if ((*obit)->m_selected) result = true;
            (*obit)->Deselect();
            obit++;
        }
        layit++;
    }
    return result;
}

//==================================
//! to find and receive the list of the objects in a working operational layer which are in the set point
int MKartaInterface::FindObjectsAtPointExact(MPoint& point, vector<double>* vect, int delta)
//==================================
{
    if(!CheckWorkingActLayer()) return 0;

    MObjectList* objList = new MObjectList;
    m_actLayer->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));

    OBITER it = objList->begin();
    MObject* pObj;

    while(it != objList->end())
    {
        //
        pObj = *it;
        vect->push_back(pObj->m_UID);
        it++;
    }
    objList->clear();
    delete objList;

    return vect->size();
}

//==================================
//!to find and receive the list of the objects in a working operational layer which are in the set point
int MKartaInterface::FindObjectsAtPointExact(MPoint& point, vector<FindObjectStruct>* vect, int delta)
//==================================
{
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        MObjectList* objList = new MObjectList;
        (*itl)->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));

        OBITER it = objList->begin();

        while(it != objList->end())
        {
            //
            FindObjectStruct fobj;

            fobj.m_LayUID = (*itl)->m_UID;
            fobj.m_ObjUID = (*it)->m_UID;

            vect->push_back(fobj);

            it++;
        }
        objList->clear();
        delete objList;

        itl++;
    }

    return vect->size();
}
//==================================  
//! to find and create the list of the objects of the map which are in the set point (in a radius of 5 pixels)
//! 'objList' it is created and destroyed OUT OF function
int MKartaInterface::FindTopObjectsAtPointExact(const MPoint& point, MObjectList* objList, int delta)
//==================================
{
    if(!m_karta) return 0;
    LAYITER itl = m_karta->m_laylist.begin();
    while(itl != m_karta->m_laylist.end())
    {
        if ((*itl)->x_visible)
            (*itl)->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));
        itl++;
    }

    return objList->size();
}

//==================================  
//! to find and create the list of the objects of the map which are in the set point (in a radius of 5 pixels)
//! 'objList' it is created and destroyed OUT OF function
int MKartaInterface::FindActObjectsAtPointExact(const MPoint& point, MObjectList* objList, int delta)
//==================================
{
    if(!m_karta) return 0;
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        if ((*itl)->x_visible)
            (*itl)->FindObjectsAtPointExact(ConvertPoint(point), objList, LogicToGeo(delta));
        itl++;
    }

    return objList->size();
}

//==================================
//! to find and receive the list of the objects in a working operational layer which are in the set point
int MKartaInterface::FindObjectsAtPoint(MPoint& point, int delta, MObjectList* objList)
//==================================
{
    if(!CheckWorkingActLayer()) return 0;

    MRect gRect;
    QRect lRect;

    QPoint lPoint = GeoToLogic(point);

    lRect.setLeft(lPoint.x()-delta);
    lRect.setRight(lPoint.x()+delta);
    lRect.setTop(lPoint.y()-delta);
    lRect.setBottom(lPoint.y()+delta);

    gRect = LogicToGeo(lRect, true);

    m_actLayer->FindObjectsAtRect(gRect, objList);
    return objList->size();
}

//==================================
//! -----------------
int MKartaInterface::FindObjectsAtPoint(MPoint& point, int delta, vector<double>* vect)
//==================================
{
    if(!CheckWorkingActLayer()) return 0;

    MRect gRect;
    QRect lRect;

    QPoint lPoint = GeoToLogic(point);

    lRect.setLeft(lPoint.x()-delta);
    lRect.setRight(lPoint.x()+delta);
    lRect.setTop(lPoint.y()-delta);
    lRect.setBottom(lPoint.y()+delta);

    gRect = LogicToGeo(lRect, true);

    MObjectList* objList = new MObjectList;
    m_actLayer->FindObjectsAtRect(gRect, objList);

    OBITER it = objList->begin();
    MObject* pObj;

    while(it != objList->end())
    {
        //
        pObj = *it;
        vect->push_back(pObj->m_UID);
        it++;
    }
    objList->clear();
    delete objList;

    return vect->size();
}

//==================================
//!to set working object in a working operational layer
bool MKartaInterface::SetWorkingObject(double uid)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_Object = FindObject(uid);
    if(m_Object) return true;
    else return false;
    //return true;
}

//==================================
//!to get working object in a working operational layer
double MKartaInterface::GetWorkingObject()
//==================================
{
    if(!m_Object) return 0;
    return m_Object->m_UID;
}

//==================================
//!to delete working object from a working operational layer
bool MKartaInterface::DeleteObject(double uid)
//==================================
{
    if(!CheckWorkingActLayer()) return false;
    m_actLayer->RemoveObject(FindObject(uid));
    return true;
}

//==================================
//!to delete working object from a operational layer
void MKartaInterface::DeleteAllObjectsFromLayer(double uid)
//==================================
{
    MLayer* pLay = FindActLayer(uid);
    if(!pLay) return;

    MObject* pObj;
    OBITER it = pLay->m_oblist.begin();
    //cycle on objects
    while(it != pLay->m_oblist.end())
    {
        //we receive the next object
        pObj = *it;
        //we delete object with this identifier from all layers
        // double uid = pObj->m_UID;

        //we will check other operational layers
        /*  LAYITER	layit = m_karta->m_actlaylist.begin();
          MObject* obj;
          while ( layit != m_karta->m_actlaylist.end())
          {
              if(*layit == pLay)
              {
                  layit++;
                  continue;
              }
              obj = (*layit)->FindObject(uid);
              if(obj)
                  (*layit)->RemoveObject(obj);
              layit++;
          }*/

        delete pObj;
        pObj = NULL;
        it++;
    }
    //we clean the list of objects
    pLay->m_oblist.clear();
}

//==================================
//!to remove all objects from a working operational layer
void MKartaInterface::DeleteAllObjectsFromLayer()
//==================================
{
    if(!CheckWorkingActLayer()) return;

    MObject* pObj;
    OBITER it = m_actLayer->m_oblist.begin();
    //cycle on objects
    while(it != m_actLayer->m_oblist.end())
    {
        //we receive the next object
        pObj = *it;
        //we delete object with this identifier from all layers
        double uid = pObj->m_UID;

        //we will check other operational layers
        LAYITER	layit = m_karta->m_actlaylist.begin();
        MObject* obj;
        while ( layit != m_karta->m_actlaylist.end())
        {
            if(*layit == m_actLayer)
            {
                layit++;
                continue;
            }
            obj = (*layit)->FindObject(uid);
            if(obj)
                (*layit)->RemoveObject(obj);
            layit++;
        }

        delete pObj;
        it++;
    }
    //we clean the list of objects
    m_actLayer->m_oblist.clear();
}

//==================================
//!to remove object from all operational layers
bool MKartaInterface::DeleteObjectFromLayers(double uid)
//==================================
{
    if(!m_karta) return false;
    if(!FindObjectAtLayers(uid)) return false;
    m_karta->RemoveActor(uid);
    return true;
}

//==================================
//!to add a point to working object
bool MKartaInterface::AddPoint(MPoint& point)
//==================================
{
    if(!CheckWorkingObject()) return false;

    if(!CheckPoint(point))
    {
        DeleteObjectFromLayers(m_Object->m_UID);
        m_Object = NULL;
        return false;
    }

    MPtArray* pta = m_Object->GetPointArray();
    if(pta->GetSize() > 0)
    {
        MPtArray* ptNew = new MPtArray();
        /*int iSize = */(void)GetLine(*pta->GetTail(),point,ptNew);
        pta->AppendTail(ptNew);
        m_Object->RecalcBounds();
        delete ptNew;
    }
    else  m_Object->AddPoint(ConvertPoint(point));
    //===========
    // m_Object->AddPoint(ConvertPoint(point));

    return true;
}

//==================================
//! -----------
bool MKartaInterface::SetPoints(MPoint* points, int size)
//==================================
{
    if(!CheckWorkingObject()) return false;
    MPtArray pta;
    MPoint pt;
    for(int i = 0; i<size; i++)
    {
        if(!CheckPoint(points[i])) return false;
        pt = ConvertPoint(points[i]);
        pta.AddTail(&pt);
    }
    m_Object->SetPoints(pta);

    if(m_Object->m_pZnak->GetYaz()->GetType() == F_PLO)
    {
        MPtArray* pta;
        for(int i = m_Object->F_TOCHKI; i<m_Object->m_Komplekt->GetSize(); i++)
        {
            pta=(MPtArray*)m_Object->m_Komplekt->GetAt(i);

            if(*(pta->GetAt(0)) != *(pta->GetAt(pta->GetSize()-1)))
            {
                MPoint pt = *(pta->GetAt(0));
                pta->AddTail(&pt);
            }
        }
    }

    if(m_Object->m_formula_ind != -1) SetPriv(m_Object->m_formula_ind);
    return true;
}

//==================================
//!to insert a point into working object
bool MKartaInterface::InsertPoint(int pos, MPoint point)
//==================================
{
    if(!CheckWorkingObject()) return false;
    m_Object->InsertPoint(pos, ConvertPoint(point));
    return true;
}

//==================================
//!to delete a point from working object
bool MKartaInterface::DeletePoint(int pos)
//==================================
{
    if(!CheckWorkingObject()) return false;
    m_Object->DeletePoint(pos);
    return true;
}

//==================================
//!o check existence of a sign with set the quad-codey
bool MKartaInterface::CheckKKod(MString& kkod)
//==================================
{
    if(!m_kls) return false;

    MString truekkod;
    MZnak* znak = m_kls->GetZnak(kkod, truekkod, true);
    if(!znak) return false;

    return true;
}

//==================================
//!to establish a sign of working object
bool MKartaInterface::SetZnak(const MString& kkod)
//==================================
{
    //  bool result= true;

    if(!CheckWorkingObject())  {
        InternalMessage("MKartaInterface::SetZnak(MString&)  -Object not found!");
        return false;
    }
    MString truekkod;
    MZnak* znak = m_kls->GetZnak(kkod, truekkod, true);

    if(!znak)  {        //  2004.12.13  vipa
        double uid= GetWorkingObject();
        DeleteObject(uid);
        m_Object= NULL;
        InternalMessage("MKartaInterface::SetZnak(MString&)  -`"+QString(kkod.c_str())+"` not found! Object removed");
        //      znak = m_kls->GetZnak(SERV_OBJECT_DOT, truekkod, true);
        //      m_Object->ConnectZnak(znak);
        //			m_Object->m_kkod= truekkod;
        //      result= false;
        return false;
    }

    m_Object->m_kkod  = truekkod;  // 2005.03.14  vipa
    m_Object->m_pZnak = znak;

    //call of check of a correctness of a sign
    CorrectZnak(m_Object);

    return true;
}

//==================================
//!to set an object sign
bool MKartaInterface::SetZnak(double uid, MString& kkod)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj)  {
        InternalMessage("MKartaInterface::SetZnak(double, MString&)  -Object not found!");
        return false;
    }

    MString truekkod;
    MZnak* znak = m_kls->GetZnak(kkod, truekkod, true);

    if(!znak)  {           //  2004.12.13  vipa
        DeleteObject(uid);   //    m_Object= NULL;
        InternalMessage("MKartaInterface::SetZnak(double, MString&)  -`"+QString(kkod.c_str())+"` not found! Object removed");
        return false;
    }

    obj->m_pZnak = znak;
    obj->m_kkod  = truekkod;  // 2005.03.14  vipa

    CorrectZnak(obj);

    return true;
}

//corrects object depending on the established sign
//is caused at the end of 'SetZnak'(double uid, MString& kkod) and SetZnak(MString& kkod)
bool MKartaInterface::CorrectZnak(MTopobject* pActObj)
{

    if (!pActObj) return false;
    if (!pActObj->m_pZnak) return false;

    if (!pActObj->m_Komplekt) return false;
    if (!pActObj->m_Komplekt->GetSize()) return false;

    MPtArray* pta = (MPtArray*)pActObj->m_Komplekt->GetAt(pActObj->F_TOCHKI);

    /*check and formation of a new contour*/
    if (pActObj->m_kkod == "UZN 5000 NZ 11")
    {
        int iC = dbl_to_int(pta->GetSize()/2.0);
        CreateAttackDirection(pta, iC-1);

    }
    else if ((pActObj->m_kkod == "UZN 5000 NZ 20")||(pActObj->m_kkod == "UZN 5000 NZ 200"))
    {
        CreateTask(pta);
        int isize = pta->GetSize();
        pActObj->AddRazryv(*pta->GetAt(0),*pta->GetAt(14),0,14);
        pActObj->AddRazryv(*pta->GetAt(22),*pta->GetAt(isize-1),22,isize-1);

    }
    else if ((pActObj->m_kkod == "UZN 5000 NZ 21")||(pActObj->m_kkod == "UZN 5000 NZ 201")||
             (pActObj->m_kkod == "UZN 5000 NZ 22")||(pActObj->m_kkod == "UZN 5000 NZ 202")||
             (pActObj->m_kkod == "UZN 5000 NZ 23")||(pActObj->m_kkod == "UZN 5000 NZ 203"))
    {
        CreateTask(pta);
        pActObj->AddRazryv(*pta->GetAt(16),*pta->GetAt(22),16,22);
    }
    else if (pActObj->m_kkod == "UZN 5000 NZ 7")
    {
        int isize = pta->GetSize();
        pActObj->AddRazryv(*pta->GetAt(35),*pta->GetAt(isize-3),36,isize-2);

    }
    else if (pActObj->m_kkod == "UZN 5000 NZ 8")
    {
        pActObj->AddRazryv(*pta->GetAt(0),*pta->GetAt(35),0,35);
    }
    /*
  else if (truekkod == "UZN 5000 NZ 11")
  {
  }

  */

    switch(pActObj->m_pZnak->GetYaz()->GetType())
    {
    case F_PLO:
        if(pta->GetSize() < 3) return false;
        else
            if(pta->GetSize() == 3)
            {
                if((*(pta->GetAt(0)) == *(pta->GetAt(1))) ||
                        (*(pta->GetAt(0)) == *(pta->GetAt(2))) ||
                        (*(pta->GetAt(1)) == *(pta->GetAt(2)))) return false;
            }
        break;
    case F_LIN:
        if(pta->GetSize() < 2) return false;
        else
            if((pta->GetSize() == 2) && (*(pta->GetAt(0)) == *(pta->GetAt(1)))) return false;
        break;
    case F_DOT:
        if(pta->GetSize() != 1) return false;
        break;
    }

    if(pActObj->m_pZnak->GetYaz()->GetType() == F_PLO)
    {
        for(int i = pActObj->F_TOCHKI; i<pActObj->m_Komplekt->GetSize(); i++)
        {
            pta=(MPtArray*)pActObj->m_Komplekt->GetAt(i);

            if(*(pta->GetAt(0)) != *(pta->GetAt(pta->GetSize()-1)))
            {
                MPoint pt;
                pt.x = (pta->GetAt(0))->x;
                pt.y = (pta->GetAt(0))->y;
                pta->AddTail(&pt);
            }
        }
    }

    return true;

}

//==================================
//!to move working object
bool MKartaInterface::MoveTo(MPoint& delta)
//==================================
{
    if(!CheckWorkingObject()) return false;
    //MPoint pt = ConvertPoint(delta);
    m_Object->MoveTo(/*pt*/delta);
    return true;
}

//==================================
//!to move working object
bool MKartaInterface::MoveTo(double distance, double azimut)
//==================================
{
    if(!CheckWorkingObject()) return false;
    m_Object->MoveTo(distance, azimut);
    return true;
}

//==================================
//!to add a point to object
bool MKartaInterface::AddPoint(double uid, MPoint& point)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    if(!CheckPoint(point))
    {
        DeleteObjectFromLayers(obj->m_UID);
        return false;
    }

    MPtArray* pta = obj->GetPointArray();
    if((pta->GetSize() > 0)&&(m_karta->m_pObjectCoo->WhatTheCoo() != COO_GEODESIC))
    {
        MPoint* ptLast = pta->GetTail();
        MPtArray* ptNew = new MPtArray();
        int iSize = GetLine(*ptLast,point,ptNew);
        for(int i=1; i<iSize; i++)
            obj->AddPoint(ConvertPoint(*ptNew->GetAt(i)));

        delete ptNew;
    }
    else  obj->AddPoint(ConvertPoint(point));
    //===========
    //obj->AddPoint(ConvertPoint(point));

    return true;
}

//==================================
//! ---------
bool MKartaInterface::SetPoints(double uid, MPoint* points, int size)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    MPtArray pta;
    MPoint pt;
    for(int i = 0; i<size; i++)
    {
        if(!CheckPoint(points[i])) return false;
        pt = ConvertPoint(points[i]);
        pta.AddTail(&pt);
    }
    obj->SetPoints(pta);

    if(obj->m_pZnak->GetYaz()->GetType() == F_PLO)
    {
        MPtArray* pta;
        for(int i = obj->F_TOCHKI; i<obj->m_Komplekt->GetSize(); i++)
        {
            pta=(MPtArray*)obj->m_Komplekt->GetAt(i);

            if(*(pta->GetAt(0)) != *(pta->GetAt(pta->GetSize()-1)))
                pta->AddTail(pta->GetAt(0));
        }
    }

    if(obj->m_formula_ind != -1) SetPriv(obj->m_formula_ind);
    return true;
}

//==================================
//!to insert a point into object
bool MKartaInterface::InsertPoint(double uid, int pos, MPoint point)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    obj->InsertPoint(pos, ConvertPoint(point));
    return true;
}

//==================================
//!to delete a point from the object
bool MKartaInterface::DeletePoint(double uid, int pos)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    obj->DeletePoint(pos);
    return true;
}

//==================================
//!to check a point for entry into a rectangle of coverage of the map
bool MKartaInterface::CheckPoint(MPoint pt)
//==================================
{
    if(!m_karta) return false;
    MRect rec = m_karta->GetGeobounds();
    pt = ConvertPoint(pt);
    if(!(rec.IsPtInRect(pt))) return false;
    return true;
}

//==================================
//!to move object
bool MKartaInterface::MoveTo(double uid, MPoint& delta)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    MPoint pt = ConvertPoint(delta);
    obj->MoveTo(pt);
    return true;
}

//==================================
//!to move working object
bool MKartaInterface::MoveTo(double uid, double distance, double azimut)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    obj->MoveTo(distance, azimut);
    return true;
}

//==================================
//! on all act_layers of the map - allocates objects with the set UID
void MKartaInterface::SelectObjectsWithUID(double uid)
//==================================
{
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        OBITER it = (*itl)->m_oblist.begin();
        while(it != (*itl)->m_oblist.end())
        {
            if((*it)->m_UID == uid) (*it)->Select();
            it++;
        }
        itl++;
    }
}

//==================================
//! on all act_layers of the map - removes allocation of objects with the set UID
void MKartaInterface::DeselectObjectsWithUID(double uid)
//==================================
{
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        OBITER it = (*itl)->m_oblist.begin();
        while(it != (*itl)->m_oblist.end())
        {
            if((*it)->m_UID == uid) (*it)->Deselect();
            it++;
        }
        itl++;
    }
}
//==================================
//! in map act_layer with the set layer_uid - allocates objects with the set UID
void MKartaInterface::SelectObjectsInLayerByUID(double layer_uid,double uid)
//==================================
{
    MLayer* m_pTheLayer = FindActLayer(layer_uid);

    if (m_pTheLayer == NULL) return;

    OBITER it = m_pTheLayer->m_oblist.begin();
    while(it != m_pTheLayer->m_oblist.end())
    {
        if((*it)->m_UID == uid) (*it)->Select();
        it++;
    }
}

//==================================
//! in map act_layer with the set layer_uid - removes allocation of objects with the set UID
void MKartaInterface::DeselectObjectsInLayerByUID(double layer_uid,double uid)
//==================================
{
    MLayer* m_pTheLayer = FindActLayer(layer_uid);

    if (m_pTheLayer == NULL) return;

    OBITER it = m_pTheLayer->m_oblist.begin();
    while(it != m_pTheLayer->m_oblist.end())
    {
        if((*it)->m_UID == uid) (*it)->Deselect();
        it++;
    }
}

//==================================
//! ------------------
void MKartaInterface::SelectObject()
//==================================
{
    if(!CheckWorkingObject()) return;
    SelectObjectsWithUID(m_Object->m_UID);
}

//==================================
//! ------------------
void MKartaInterface::DeselectObject()
//==================================
{
    if(!CheckWorkingObject()) return;
    DeselectObjectsWithUID(m_Object->m_UID);
}

//==================================
//! ------------------
bool MKartaInterface::CheckSelectedObject()
//==================================
{
    if(!CheckWorkingObject()) return false;
    return m_Object->m_selected;
}

//==================================
//! ------------------
void MKartaInterface::SetSelectedColor(const MColor& col)
//==================================
{
    if(!CheckWorkingObject()) return;

    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        OBITER it = (*itl)->m_oblist.begin();
        while(it != (*itl)->m_oblist.end())
        {
            if((*it)->m_UID == m_Object->m_UID)(*it)->SetSelectedColor(col);
            it++;
        }
        itl++;
    }
    //m_Object->SetSelectedColor(col);
}

//==================================
//! ------------------
void MKartaInterface::SetSelectedColor(double uid, MColor& col)
//==================================
{
    LAYITER itl = m_karta->m_actlaylist.begin();
    while(itl != m_karta->m_actlaylist.end())
    {
        OBITER it = (*itl)->m_oblist.begin();
        while(it != (*itl)->m_oblist.end())
        {
            if((*it)->m_UID == uid)(*it)->SetSelectedColor(col);
            it++;
        }
        itl++;
    }
    /*	MTopobject* obj = FindObject(uid);
    if(!obj) return;
    obj->SetSelectedColor(col);*/
}

//==================================
//! ------------------
void MKartaInterface::SelectObject(double uid)
//==================================
{
    SelectObjectsWithUID(uid);
    /*MTopobject* obj = FindObject(uid);
    if(!obj) return;
    obj->Select();*/
}

//==================================
//! ------------------
void MKartaInterface::DeselectObject(double uid)
//==================================
{
    DeselectObjectsWithUID(uid);
    /*MTopobject* obj = FindObject(uid);
    if(!obj) return;
    obj->Deselect();*/
}
//==================================
//! ------------------
bool MKartaInterface::CheckSelectedObject(double uid)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    return obj->m_selected;
}

//==================================
//! ------------------
void MKartaInterface::SetFormularName(MString formularname)
//==================================
{
    m_formularname = formularname;
}

//==================================
//
//! ------------------
bool MKartaInterface::SetFormular(MString formular)
//==================================
{
    if(m_formularname == "") return false;

    if(!CheckWorkingObject()) return false;

    //change \\t to \t
    formular.Replace("\\t", "\t");

    m_Object->SetDataPoleByName(formular, m_formularname);

    //we do stanzas like COLUMN
    for(int i = 0; i<m_Object->m_pZnak->GetYaz()->m_formula->GetSize(); i++)
    {
        MFormula* formula = m_Object->m_pZnak->GetYaz()->m_formula->GetAt(i);
        for(int j = 0; j<formula->fragm_arr->GetSize(); j++)
        {
            MFragment* frag = formula->fragm_arr->GetAt(j);
            if(frag->GetType() == ID_FRAG_STROFA)
            {
                MStrofa* strofa = (MStrofa*)frag->GetFragment();
                if(strofa->GetType() != ID_STROFA_STOLBEC)
                    strofa->SetType(ID_STROFA_STOLBEC);
            }
        }
    }
    return true;
}

//==================================
//
//! ------------------
bool MKartaInterface::SetFormular(MString& formularname,MString& formular)
//==================================
{
    if(!CheckWorkingObject()) return false;

    //change \\t to \t
    formular.Replace("\\t", "\t");

    m_Object->SetDataPoleByName(formular, formularname);

    //we do stanzas like COLUMN
    for(int i = 0; i<m_Object->m_pZnak->GetYaz()->m_formula->GetSize(); i++)
    {
        MFormula* formula = m_Object->m_pZnak->GetYaz()->m_formula->GetAt(i);
        for(int j = 0; j<formula->fragm_arr->GetSize(); j++)
        {
            MFragment* frag = formula->fragm_arr->GetAt(j);
            if(frag->GetType() == ID_FRAG_STROFA)
            {
                MStrofa* strofa = (MStrofa*)frag->GetFragment();
                if(strofa->GetType() != ID_STROFA_STOLBEC)
                    strofa->SetType(ID_STROFA_STOLBEC);
            }
        }
    }

    return true;
}

//==================================
//
//! ------------------
bool MKartaInterface::SetPriv(MPoint& point, int formula_index)
//==================================
{
    if(!CheckWorkingObject()) return false;
    MPtArray pta;
    pta.ClearAll();
    MPoint pt = ConvertPoint(point);
    pta.AddTail(&pt);
    m_Object->m_formula_ind = formula_index;
    m_Object->DeletePrivs(formula_index, SIMPLE_PRIVYAZKA);
    m_Object->AddPriv(formula_index, pta, SIMPLE_PRIVYAZKA);
    return true;
}

//==================================
//! ------------------
bool MKartaInterface::SetPriv(int formula_index)
//==================================
{
    if(!CheckWorkingObject()) return false;
    MPtArray* pta;
    MPtArray pta2;
    MPoint pt;

    MYaz* pYaz=m_Object->m_pZnak->GetYaz();
    if(!pYaz) return false;

    switch(pYaz->GetType())
    {
    case F_PLO:
    {
        MPoint p1(m_Object->m_bounds.left, m_Object->m_bounds.top);
        MPoint p2(m_Object->m_bounds.right, m_Object->m_bounds.bottom);
        p1 = ConvertPoint(p1, false);
        p2 = ConvertPoint(p2, false);
        pt.x = p1.x+fabs((p1.x - p2.x)/2);
        pt.y = p1.y-fabs((p1.y - p2.y)/2);
        pt = ConvertPoint(pt);
    }
        break;
    case F_LIN:
        pta = (MPtArray*)m_Object->m_Komplekt->GetAt(m_Object->m_Komplekt->GetSize()-1);
        if(pta->GetSize() == 0) return false;
        pt = GetPointBySdvig(pta, DLINALOMLINE(pta)/2);//Mak 05.2010 for kotra
        //pt = pta->GetAt(pta->GetSize()-1);
        break;
    case F_DOT:
        pta = (MPtArray*)m_Object->m_Komplekt->GetAt(m_Object->m_Komplekt->GetSize()-1);
        if(pta->GetSize() == 0) return false;
        pt = pta->GetAt(pta->GetSize()-1);
        //we will receive the symbol size
        MSzArray* szArr = ((MDotYaz*)pYaz->m_object)->m_tochka;
        double sym_size = 0;
        if(szArr)
        {
            for(int i = 0; i<szArr->GetSize(); i++)
            {
                if((szArr->GetAt(i)->GetType() == IDT_SHRIFT) || (szArr->GetAt(i)->GetType() == IDT_NAS_SHRIFT))
                {
                    double tmp_size = ((MShriftSrez*)szArr->GetAt(i))->m_tochka.value;

                    tmp_size = SizeToReal(m_Object,tmp_size);
                    if(tmp_size>sym_size) sym_size = tmp_size;
                }
            }
        }

        MSrez* last_sz = szArr->GetAt(szArr->GetSize()-1);
        if((last_sz->GetType() == IDT_SHRIFT) || (last_sz->GetType() == IDT_NAS_SHRIFT))
        {
            unsigned int align = ((MShriftSrez*)last_sz)->m_tochka.align;

            //we will consider shift
            if ((align & UT_TA_CENTER) == UT_TA_CENTER)
                pt.x += sym_size/2 + sym_size/4;
            else
                if ((align & UT_TA_RIGHT) == UT_TA_RIGHT)
                    pt.x += sym_size/4;
                else
                    if ((align & UT_TA_LEFT) == UT_TA_LEFT)
                        pt.x += sym_size + sym_size/4;

            /*if ((align & UT_TA_BASELINE) == UT_TA_BASELINE)
                {
                    pt.y += sym_size/2;
                }
                else
                    if ((align & UT_TA_BOTTOM) == UT_TA_BOTTOM)
                    {
                        pt.y += sym_size;
                    }
                    else
                        if ((align & UT_TA_TOP) == UT_TA_TOP)
                        {
                            pt.y -= sym_size/2;
                        }*/

        }
        break;
    }

    pta2.ClearAll();
    pta2.AddTail(&pt);
    m_Object->m_formula_ind = formula_index;
    m_Object->DeletePrivs(formula_index, SIMPLE_PRIVYAZKA);
    m_Object->AddPriv(formula_index, pta2, SIMPLE_PRIVYAZKA);
    return true;
}

//==================================
//! to reinstall all bindings, taking into account restrictions on the text size
void MKartaInterface::ResetAllPrivs(MPlotDevice* pDev)
//==================================
{
    MTopobject* pObj;
    MYaz* pYaz;
    MPtArray* pta;
    MPtArray pta2;
    MPoint pt;

    MSzArray* szArr;
    MSrez* last_sz;

    double sym_size = 0;
    double sym_size_log = 0;

    double tmp_size;
    double tmp_size_log;

    unsigned int align;

    double etalon_size = 37.5;

    double max_size;
    double koef;

    std::for_each(m_karta->m_actlaylist.begin(),
                  m_karta->m_actlaylist.end(), [&] (MLayer *it)
    {
        std::for_each(it->m_oblist.begin(),
                      it->m_oblist.end(), [&] (MObject* obit)
        {
            pObj = dynamic_cast<MTopobject*>(obit);

            if (!pObj || !pObj->m_pZnak)
            {
                return;
            }

            pYaz = pObj->m_pZnak->GetYaz();

            if(!pYaz || pYaz->GetType() != F_DOT ||
                    !pObj->m_status ||
                    !pObj->m_Komplekt->GetSize())
            {
                return;
            }

            pta = (MPtArray*)pObj->m_Komplekt->
                    GetAt(pObj->m_Komplekt->GetSize() - 1);

            if (!pta->GetSize())
            {
                return;
            }

            pt = pta->GetAt(pta->GetSize() - 1);

            //we will receive the symbol size
            szArr = ((MDotYaz*)pYaz->m_object)->m_tochka;

            sym_size = 0;
            sym_size_log = 0;

            if(szArr)
            {
                for(int i = 0; i < szArr->GetSize(); ++i)
                {
                    if((szArr->GetAt(i)->GetType() == IDT_SHRIFT) ||
                            (szArr->GetAt(i)->GetType() == IDT_NAS_SHRIFT))
                    {
                        tmp_size_log = ((MShriftSrez*)szArr->GetAt(i))->m_tochka.value;
                        tmp_size = SizeToReal(pObj, tmp_size_log);

                        if(tmp_size > 0)
                        {
                            sym_size = tmp_size;
                            sym_size_log = tmp_size_log;
                        }
                    }
                }
            }

            if(textSizeMax)
            {
                max_size =
                        pDev->LogicToBum(static_cast<int>(textSizeMax));

                koef = etalon_size / sym_size_log;

                if((sym_size * koef) > max_size) sym_size = max_size / koef;
            }

            last_sz = szArr->GetAt(szArr->GetSize() - 1);

            if((last_sz->GetType() == IDT_SHRIFT) ||
                    (last_sz->GetType() == IDT_NAS_SHRIFT))
            {
                align = ((MShriftSrez*)last_sz)->m_tochka.align;

                //we will consider shift
                if ((align & UT_TA_CENTER) == UT_TA_CENTER)
                    pt.x += sym_size / 2 ;//+ sym_size/4;
                else
                    if ((align & UT_TA_RIGHT) == UT_TA_RIGHT)
                        pt.x += 0; //sym_size/4;
                    else
                        if ((align & UT_TA_LEFT) == UT_TA_LEFT)
                            pt.x += sym_size ;//+ sym_size/4;
            }

            pta2.ClearAll();
            pta2.AddTail(&pt);
            pObj->m_formula_ind = 0;//formula_index;
            pObj->DeletePrivs(0/*formula_index*/, SIMPLE_PRIVYAZKA);
            pObj->AddPriv(0/*formula_index*/, pta2, SIMPLE_PRIVYAZKA);
        });
    });
}

//==================================
//! ------------------
void MKartaInterface::DeleteAllPriv(double uid, int formula_index)
//==================================
{
    MTopobject* obj = FindObject(uid);
    if(!obj) return;
    obj->DeletePrivs(formula_index, SIMPLE_PRIVYAZKA);
    obj->m_formula_ind = -1;
}

//==================================
//! ------------------
void MKartaInterface::DeleteAllPriv(int formula_index)
//==================================
{
    if(!CheckWorkingObject()) return;
    m_Object->DeletePrivs(formula_index, SIMPLE_PRIVYAZKA);
    m_Object->m_formula_ind = -1;
}

//==================================
//! Will transform a point to map coordinates (to bIn = false)
//! or to geodetic coordinates (to bIn = true)
// presumption: MPoint &pt coordinates are global or native.
// converts pt coordinates into native(bIn=true, by default) or global(bIn=false) format.
MPoint MKartaInterface::ConvertPoint(const MPoint& pt, const bool &bIn) const
//==================================
{
    if(!m_karta) return pt;
    if(!m_karta->m_pObjectCoo) return pt;

    MPoint ret_pt = pt;
    if (dynamic_cast<MGeodesicCoo *> (m_karta->m_pObjectCoo)  )
        return  ret_pt;           // native = global

    /*COO_PARENT
    COO_GEODESIC
    COO_EXTENDEDGAUSSKRUGER
    COO_STANDARTGAUSSKRUGER
    COO_ZONEGAUSSKRUGER
    COO_PSEVDOGAUSSKRUGER*/
    if(bIn)	//  to native coordinates  (from global)
    {
        //the point is drawn from a map
        if((ret_pt.x<1000)&&(ret_pt.y<1000))
        {
            MGeodesicCoo coo;        //MExtendedGaussKrugerCoo
            coo.SetStandartEllipsoid(m_karta->m_pObjectCoo->GetEllipsoidName());
            double h = 0.0F;
            coo.ConvertToCoo(m_karta->m_pObjectCoo, ret_pt, h);
            //m_karta->m_pObjectCoo->ConvertToCoo(&coo, ret_pt, h);
        }
    }
    else //  to global coordinates  (from native)
    {//the point will be transformed for display on the map
        if(!((ret_pt.x<1000)&&(ret_pt.y<1000)))
        {
            MGeodesicCoo coo;
            coo.SetStandartEllipsoid(m_karta->m_pObjectCoo->GetEllipsoidName());
            double h = 0.0F;
            m_karta->m_pObjectCoo->ConvertToCoo(&coo, ret_pt, h);
        }
    }

    return ret_pt;
}

//==================================
//! Establishes the file of a matrix of heights of a relief
bool MKartaInterface::SetMatrixFile(MString path)
//==================================
{
    if(!m_karta) return false;
    return m_karta->SetMatrixFile(path);
}

//==================================
//! Establishes the file of a matrix of excess of objects
bool MKartaInterface::SetMatrixObjectFile(MString path)
//==================================
{
    if(!m_karta) return false;
    return m_karta->SetMatrixObjectFile(path);
}
//==================================
//!Sets a file of Correction Matrix
bool MKartaInterface::SetMatrixCorrectionFile(MString path)
//==================================
{
    if(!m_karta) return false;
    return m_karta->SetMatrixCorrectionFile(path);
}
/*
//==================================
//! Returns height on a matrix of heights of objects and a relief
int MKartaInterface::GetHight(MPoint& pt, double* hRel, double* hObj)
//==================================
{
  if(!m_karta) return -1000000;
  MPoint tmp_pt = ConvertPoint(pt);
  return m_karta->GetHeightFromMatrixes(tmp_pt, hRel, hObj);
}
*/
//==================================
//! Returns Heights on a matrix of heights, relief, objects i adjustment
int MKartaInterface::GetHight(const MPoint& pt, double* hRel, double* hObj, double* hCor)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);
    return m_karta->GetHeightFromMatrixes(tmp_pt, hRel, hObj, hCor);
}

//==================================
//! Returns Row of Heights on a matrix of heights
int MKartaInterface::GetHightRow(MPoint& pt,MInArray* values, int count)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);
    return m_karta->GetHeightRowFromMatrix(tmp_pt, values, count);
}

//==================================
//! Returns height on a matrix of excess of objects
int MKartaInterface::GetHightObjects(MPoint& pt)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);

    int iRet = m_karta->GetHeightFromMatrixObject(tmp_pt);

    if ((bHeightToZero) &&
            ((iRet == -20000)||(iRet==9999)))
        iRet = 0;

    return iRet;
}

//==================================
//! Returns Row of Heights on a matrix of objects heights
int MKartaInterface::GetHightRowObjects(MPoint& pt,MInArray* values, int count)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);
    return m_karta->GetHeightRowFromMatrixObject(tmp_pt, values, count);
}

//==================================
//!Returns Height on an adjustment matrix
int MKartaInterface::GetHightCorrection(MPoint& pt)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);

    int iRet = m_karta->GetHeightFromMatrixCorrection(tmp_pt);

    if ((bHeightToZero) &&
            ((iRet == -20000)||(iRet == 9999)) )
        iRet = 0;

    return iRet;
}

//==================================
//! Returns Height on a matrix of heights
int MKartaInterface::GetHightRelief(MPoint& pt)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);
    int iRet = m_karta->GetHeightFromMatrix(tmp_pt);

    if ((bHeightToZero) &&
            ((iRet == -20000)||(iRet == 9999)) )
        iRet = 0;

    return iRet;
}

//====================================
//! Returns a geographical rectangle of the map
MRect MKartaInterface::GetGeoBounds(bool bInMapCoo)
//====================================
{
    if(!m_karta) return MRect(0, 0, 0, 0);
    if(!bInMapCoo)
    {
        MRect grect = m_karta->GetGeoBounds();
        MPoint pt1(grect.left, grect.top);
        MPoint pt2(grect.right, grect.bottom);
        pt1 = ConvertPoint(pt1, false);
        pt2 = ConvertPoint(pt2, false);
        return MRect(pt1.x, pt1.y, pt2.x, pt2.y);
    }
    return m_karta->GetGeoBounds();
}

//====================================
//! Returns background color of the map
MColor MKartaInterface::GetBackgroundColor(bool HLS)
//====================================
{
    if(!m_karta) return MColor(255, 255, 255);
    return m_karta->GetPaperColor(HLS);
}

//====================================
//! Sets background color of the map
void MKartaInterface::SetBackgroundColor(MColor& col)
//====================================
{
    if(!m_karta) return;
    m_karta->SetPaperColor(col);
}

//====================================
//! counts distance
double MKartaInterface::GetDistance(const MPoint& ppt1, const MPoint& ppt2, double poluos1, double poluos2) const
//====================================
{


    MPoint pt1 = ConvertPoint(ppt1, false);
    MPoint pt2 = ConvertPoint(ppt2, false);
    const MGeodesic geod(poluos1, poluos1/(poluos1-poluos2));
    double s12,azi1,azi2;
    geod.Inverse(pt1.y, pt1.x, pt2.y, pt2.x, s12, azi1, azi2);
    return s12;

    //	poluos1 = axis_X;
    //	poluos2 = axis_Y;

    //	pt1 = ConvertPoint(pt1, false);
    //	pt2 = ConvertPoint(pt2, false);

    ////	double sjatie = (poluos1-poluos2)/poluos1;
    //	double sqrte1 = (poluos1*poluos1-poluos2*poluos2)/(poluos1*poluos1);
    //	double sqrte2 = (poluos1*poluos1-poluos2*poluos2)/(poluos2*poluos2);
    ////	double p2shtrih = 1/(4848137*pow((double)10,(double)-12));

    //	double ppi = 4.0*atan(1.0);//double ppi = 4*atan((double)1); //-ATTENTION-

    //	double BB1 = pt1.y * 2.0 *ppi/360.0;//*2*ppi/360;//(grad1+min1/60+sec1/3600)*2*ppi/360; //-ATTENTION-
    //	double BB2 = pt2.y * 2.0 *ppi/360.0;//*2*ppi/360;//(grad2+min2/60+sec2/3600)*2*ppi/360;//-ATTENTION-
    //	double LL1 = pt1.x * 2.0 *ppi/360.0;//*2*ppi/360;//(grad1+min1/60+sec1/3600)*2*ppi/360;//-ATTENTION-
    //	double LL2 = pt2.x * 2.0 *ppi/360.0;//*2*ppi/360;//(grad2+min2/60+sec2/3600)*2*ppi/360;//-ATTENTION-

    //	double tgB1=0.0, tgB2=0.0;
    //	if (BB1!=0 && BB2!=0)
    //	  {
    //		tgB1 = sin(BB1)/cos(BB1);
    //		tgB2 = sin(BB2)/cos(BB2);
    //	  }
    //	double l = fabs(LL2-LL1);//grad4-grad3+(min4-min3)/60+(sec4-sec3)/3600)*2*ppi/360;

    //	double Coslna2 = cos(l/2);//fabs(grad4-grad3+(min4-min3)/60+(sec4-sec3)/3600)*ppi/360);
    //	double tgBsredn = (tgB1+tgB2)/(2*Coslna2);

    //	double cos2Bsr = 1/(1+pow(tgBsredn,2));
    //	double Vsredn = (4.0018 + 3*sqrte2*cos2Bsr)/(4.0018 + sqrte2*cos2Bsr);

    //	double V1 = (4.0018 + 3*sqrte2*pow(cos(BB1),2))/(4.0018 + sqrte2*pow(cos(BB1),2));
    //	double V2 = (4.0018 + 3*sqrte2*pow(cos(BB2),2))/(4.0018 + sqrte2*pow(cos(BB2),2));
    //
    //	double V = (V1+4*Vsredn+V2)/6;

    //	double delta1 = V*l;//Vsredn*l;

    //	double cosp = (sin(BB1)*sin(BB2) + cos(BB1)*cos(BB2)/sqrt(1-pow(sqrte1,2)) * cos(delta1))/V1/V2;
    //	double p = acos(cosp);
    //	double sinq = sin(delta1)*cos(BB1)*cos(BB2)/sin(p)/V1/V2;
    //	double cosdel2 = cos(l+0.003351*p*sinq);
    //	double sindel2 = sin(l+0.003351*p*sinq);
    //	double del2 = l+0.003351*p*sinq;

    //	double f1 = cos(BB1)*cos(BB2)*sindel2;
    //	double f2 = sqrt(1-pow(cos(BB1)*cos(BB2)*cosdel2 + sin(BB1)*sin(BB2),2));
    //	double cosBm = f1/f2;
    //	double Bm = acos(cosBm);
    //	double sinBm = sin(Bm);

    //	double k2 = sqrte1*pow(sinBm,2);
    //	double Wm = (1-0.75*k2)/(1-0.25*k2);
    //	double sinfi1 = sin (BB1)/sinBm;
    //	double sinfi2 = sin (BB2)/sinBm;
    //	double fi1 = asin(sinfi1);
    //	double fi2 = asin(sinfi2);
    ////	double tanfi1 = tan(fi1);
    //	double qw = sqrt(1-1.25*k2);

    //	double a1 = atan(sqrt(1-1.25*k2)*tan(fi1));
    //	double a2 = atan(sqrt(1-1.25*k2)*tan(fi2));
    ////	double tana1 = sqrt(1-1.25*k2)*tan(fi1);

    ////	double a1n = (-73-57/60-50.96/3600)*2*ppi/360;
    ////	double a2n = (-47-24/60-11.11/3600)*2*ppi/360;
    ////	double asd = a1n-a2n;
    //	double f76 = (a1-a2)*1.2/sqrt(1-1.25*k2);
    //	double f77 = f76+0.2*(fi2-fi1);
    //	double f78 = poluos2*Wm;
    //	double f79 = f78*f77;
    //	double s12shtrih = fabs(f79);

    //	double del3 = acos(tan(BB2)/tan(Bm))-acos(tan(BB1)/tan(Bm));
    ////	double del = (30+14/60+58.34/3600)*2*ppi/360;

    //	double s01 = poluos2*Wm*(0.2*(fabs(fi1)-ppi/2) + 1.2/qw*(ppi/2-fabs(a1)));
    //	double s02 = poluos2*Wm*(0.2*(ppi/2-fabs(fi2)) + 1.2/qw*(fabs(a2)-ppi/2));

    //	double s12= 0.0;

    //	if(fabs(fabs(del3)-fabs(del2))<=0.01*2*pi/360)
    //	  s12 = fabs(s12shtrih);
    //	else
    //	  if(fabs(fabs(del3)-fabs(del2))>0.01*2*pi/360 && BB1>=BB2)
    //		s12 = fabs(s12shtrih)+2*fabs(s01);
    //	  else
    //		if(fabs(fabs(del3)-fabs(del2))>0.01*2*pi/360 && BB1<BB2)
    //		  s12 = fabs(s12shtrih)+2*fabs(s02);
    //	return s12;
}


//====================================
//! counts azimuth
double MKartaInterface::GetAzimut(const MPoint& ppt1, const MPoint& ppt2, double poluos1, double poluos2) const
//====================================
{
    MPoint pt1 = ConvertPoint(ppt1, false);
    MPoint pt2 = ConvertPoint(ppt2, false);
    const MGeodesic geod(poluos1, poluos1/(poluos1-poluos2));
    double s12,azi1,azi2;
    geod.Inverse(pt1.y, pt1.x, pt2.y, pt2.x, s12, azi1, azi2);
    return azi1;
    //  poluos1 = axis_X;
    //  poluos2 = axis_Y;
    //
    //	pt1 = ConvertPoint(pt1, false);
    //	pt2 = ConvertPoint(pt2, false);


    ////	double sjatie = (poluos1-poluos2)/poluos1;
    //	double sqrte1 = (poluos1*poluos1-poluos2*poluos2)/(poluos1*poluos1);
    //	double sqrte2 = (poluos1*poluos1-poluos2*poluos2)/(poluos2*poluos2);
    ////	double p2shtrih = 1/(4848137*pow((double)10,(double)-12));


    //	double ppi = 4*atan((double)1);

    //	double BB1 = pt1.y*2*ppi/360;//(grad1+min1/60+sec1/3600)*2*ppi/360;
    //	double BB2 = pt2.y*2*ppi/360;//(grad2+min2/60+sec2/3600)*2*ppi/360;
    //	double LL1 = pt1.x*2*ppi/360;//(grad1+min1/60+sec1/3600)*2*ppi/360;
    //	double LL2 = pt2.x*2*ppi/360;//(grad2+min2/60+sec2/3600)*2*ppi/360;

    //	double tgB1=0.0, tgB2=0.0;
    //	if (BB1!=0 && BB2!=0){
    //		tgB1 = sin(BB1)/cos(BB1);				//[4]
    //		tgB2 = sin(BB2)/cos(BB2);				//[5]
    //	}
    //	double l = fabs(LL2-LL1);					//[1] (radians)
    //	double Coslna2 = cos(l/2);					//[3]
    //	double tgBsredn = (tgB1+tgB2)/(2*Coslna2);

    //	//-------------------------------------------
    //	// 2. Calculating V_cp
    //	double cos2Bsr = 1/(1+pow(tgBsredn,2));		//[15]
    //	double Vsredn = (4.0018 + 3.0*sqrte2*cos2Bsr)/(4.0018 + sqrte2*cos2Bsr);

    //	double V1 = (4.0018 + 3.0*sqrte2*pow(cos(BB1),2))/(4.0018 + sqrte2*pow(cos(BB1),2));
    //	double V2 = (4.0018 + 3.0*sqrte2*pow(cos(BB2),2))/(4.0018 + sqrte2*pow(cos(BB2),2));

    //	double V = (V1+4*Vsredn+V2)/6;

    //	double delta1 = V*l;//Vsredn*l;

    //	double cosp = (sin(BB1)*sin(BB2) + cos(BB1)*cos(BB2)/sqrt(1-pow(sqrte1,2)) * cos(delta1))/V1/V2;
    //	double p = acos(cosp);
    //	double sinq = sin(delta1)*cos(BB1)*cos(BB2)/sin(p)/V1/V2;
    //	double cosdel2 = cos(l+0.003351*p*sinq);
    //	double sindel2 = sin(l+0.003351*p*sinq);
    //	double del2 = l+0.003351*p*sinq;

    //	double f1 = cos(BB1)*cos(BB2)*sindel2;
    //	double f2 = sqrt(1-pow(cos(BB1)*cos(BB2)*cosdel2 + sin(BB1)*sin(BB2),2));
    //	double cosBm = f1/f2;
    //	double Bm = acos(cosBm);
    ////	double sinBm = sin(Bm);


    ////	double k2 = sqrte1*pow(sinBm,2);
    ////	double Wm = (1-0.75*k2)/(1-0.25*k2);
    //	double Vm = (4.0018+3*sqrte2*pow(cos(Bm),2))/(4.0018+sqrte2*pow(cos(Bm),2));

    //	double del3 = acos(tan(BB2)/tan(Bm))-acos(tan(BB1)/tan(Bm));
    //
    //	//double A12_ = asin(cos(Bm)/Vm/(cos(B1)/V1));
    //
    //	double sinA12_ = cos(Bm)*V1/(Vm*cos(BB1));
    //	double A12_ = asin(sinA12_);
    //	double sinA21_ = cos(Bm)*V2/(Vm*cos(BB2));
    //	double A21_ = asin(sinA21_);
    //
    //	double Az12 = 0.0;
    //	double Az21 = 0.0;

    //	if(!m_karta) return -1.0;
    //	double az = m_karta->GetRumb(pt1, pt2, poluos1, poluos2);

    ////	if ((fabs(del3) - fabs(del2)) <= /*0.01*/0.00001*pi/180)
    //	if (fabs(fabs(del3) - fabs(del2)) <= /*0.01*/0.00001*pi/180)
    //	{
    //		//Calculating A_12 And A_21
    //		if (BB1>BB2)
    //		{
    //			if (LL1>=LL2)
    //			{
    //				double az_tmp = m_karta->GetRumb(MPoint(pt1.x, pt1.y), MPoint(pt2.x, pt2.y-0.000001), poluos1, poluos2);
    //				if ((az_tmp>az) && (fabs(az-az_tmp) < 45.0))
    //					Az12 = 2*pi - az;
    //				else
    //					Az12 = pi + az;
    //				//Az12 = pi + A12_;
    //				//Az21 = A21_;
    //			}
    //			if (LL1<LL2)
    //			{
    ////				double az_tmp = m_karta->GetRumb(MPoint(pt1.x, pt1.y), MPoint(pt1.x, pt1.y-0.01), poluos1, poluos2);
    ////				if ((az_tmp<az) && (fabs(az-az_tmp) < 45.0))
    ////					Az12 = pi + az;
    //				double az_tmp = m_karta->GetRumb(MPoint(pt1.x, pt1.y), MPoint(pt2.x, pt2.y+0.000001), poluos1, poluos2);
    //				if ((az_tmp<az) && (fabs(az-az_tmp) < 45.0))
    //					Az12 = az;
    //
    //				else
    //					Az12 = pi - az;
    //				//Az12 = pi - A12_;
    //				//Az21 = 2*pi - A21_;
    //			}
    //		}
    //		if (BB1<=BB2)
    //		{
    //			if (LL1>=LL2)
    //			{
    ////				double az_tmp = m_karta->GetRumb(MPoint(pt1.x, pt1.y), MPoint(pt1.x, pt1.y-0.01), poluos1, poluos2);
    ////				if ((az_tmp<az) && (fabs(az-az_tmp) < 45.0))
    ////					Az12 = az;
    //				double az_tmp = m_karta->GetRumb(MPoint(pt1.x, pt1.y), MPoint(pt2.x, pt2.y+0.000001), poluos1, poluos2);
    //				if ((az_tmp>az) && (fabs(az-az_tmp) < 45.0))
    //					Az12 = az;//Az12 = 2*pi - az;
    //				else
    //					Az12 = 2*pi - az;//Az12 = pi + az;
    //				//Az12 = 2*pi - A12_;
    //				//Az21 = pi - A21_;
    //			}
    //			if (LL1<LL2)
    //			{
    //				Az12 = az;
    //				//Az12 = A12_;
    //				//Az21 = pi + A21_;
    //			}
    //		}
    //	}
    //	else
    //	{
    //		//Calculating A_12 And A_21
    //		if (BB1>BB2)
    //		{
    //			if (LL1>=LL2)
    //			{Az12 = 2.0*pi - A12_;
    //			 Az21 = A21_;}
    //			if (LL1<LL2)
    //			{Az12 = A12_;
    //			 Az21 = 2.0*pi - A21_;}
    //		}
    //		if (BB1<=BB2)
    //		{
    //			if (LL1>=LL2)
    //			{Az12 = 2.0*pi - A12_;
    //			 Az21 = A21_;}
    //			if (LL1<LL2)
    //			{Az12 = A12_;
    //			 Az21 = 2.0*pi - A21_;}
    //		}
    //
    //	}

    //	return (Az12*180.0/pi);
}


//====================================
//! counts a directional angle
double MKartaInterface::GetGridAzimut(const MPoint& ppt1, const MPoint& ppt2, double poluos1, double poluos2)
//====================================
{
    double azim = GetAzimut(ppt1,ppt2,poluos1,poluos2);
    MString cooType(m_karta->m_pObjectCoo->WhatTheCoo());
    if ((cooType == COO_EXTENDEDGAUSSKRUGER)||(cooType == COO_PSEVDOGAUSSKRUGER)||(cooType == COO_ZONEGAUSSKRUGER))
        azim -=	 GaussConvergence(ppt1,floor(((MExtendedGaussKrugerCoo*)m_karta->m_pObjectCoo)->m_meridian/6.0)+1);

    while(azim<0.0)
        azim+=360.0;
    while(azim>360)
        azim-=360.0;
    return azim;
}

//====================================
//calculation of Runge-Kutta-Inglanda which are delta making for a method
void MKartaInterface::DeltaRungeKutta(double alpha, double phi, double S0, double betta, double Pi, \
                                      double& deltaB, double& deltaL, double& deltaA)
//====================================
{
    double gamma = betta*pow(cos(phi),2);
    double V = (1+0.6*gamma)/(1+0.2*gamma);
    deltaB = S0*pow(V, 3)*cos(alpha);
    deltaL = S0*V*sin(alpha)/cos(phi);
    deltaA = deltaL*sin(phi);
    deltaB *= Pi/648000.0;// the translation from {" } in radians
    deltaL *= Pi/648000.0;
    deltaA *= Pi/648000.0;
}

//====================================
//! counts point coordinates on range and an azimuth from a starting point
MPoint MKartaInterface::GetPointByAzimuthLength(MPoint pt1, double azimuth, double ds, double poluos1, double poluos2)
//====================================
//MPoint poin(23.75373055556,50.02304722222); ///////////////Temp
//MPoint pt = GetPointByAzimDaln(poin, 3.496063888889, 281260.08, 6378245, 6356863);//Temp
//supposed result: B: 52^39'03,91" (52,65) L: 24^00'25,45"
{

    pt1 = ConvertPoint(pt1, false);
    const MGeodesic geod(poluos1, poluos1/(poluos1-poluos2));
    double lat2, lon2,azi2;
    geod.Direct(pt1.y, pt1.x, azimuth, ds, lat2, lon2, azi2);
    return MPoint(lon2,lat2);
    //	poluos1 = axis_X;
    //	poluos2 = axis_Y;
    //
    //	pt1 = ConvertPoint(pt1, false);

    //	//first and second eccentricities of a meridian ellipse
    //	double sqrte1 = (poluos1*poluos1-poluos2*poluos2)/(poluos1*poluos1);
    //	double sqrte2 = (poluos1*poluos1-poluos2*poluos2)/(poluos2*poluos2);
    //	double ppi = 4.0*atan(1.0);

    //		//Radius of curvature of a meridian of an ellipsoid in poles
    //		double C = poluos1/(sqrt(1-sqrte1)); //6399698.8 ;
    //	double betta = 1.25*sqrte2; //0.00842316;
    //	double ro_shtrih = 180.0*60.0*60.0/ppi;
    //	double S0 = ds*ro_shtrih/C;
    //
    //	//to RAD
    //	double BB1 = pt1.y * ppi/180.0;
    //	double LL1 = pt1.x * ppi/180.0;
    //	azimuth = azimuth * ppi/180.0;

    //	//auxiliary distances for the cycle 'for'
    //	double deltaS = S0;//current step
    //	double sigmaS = 0;//the "passable" distance

    //	double dA1,dA2,dA3,dA4,dA5,dA6,dB1,dB2,dB3,dB4,dB5,dB6,dL1,dL2,dL3,dL4,dL5,dL6,MdeltaB,MdeltaL;
    //	//beginning initialization
    //	double  BB2 = BB1;
    //	double  LL2 = LL1;
    ////	double  AA2 = azimuth;

    //	/*method of Runge-Kutty-Inglanda with check of a local mistake:
    //  if an error big, we divide distance into intervals since the accepted
    //  a local mistake for each interval,
    //  previously applying a method to the current step*/
    //	//for(;;)
    //	{
    //		//Method of Runge-Kutta-Inglanda from 6 stages:
    //		DeltaRungeKutta(azimuth, BB1, deltaS, betta, ppi, dB1, dL1, dA1);
    //		DeltaRungeKutta(azimuth+dA1/2.0, BB1+dB1/2.0, deltaS, betta, ppi, dB2, dL2, dA2);
    //		DeltaRungeKutta(azimuth + (dA1+dA2)/4.0, BB1 + (dB1+dB2)/4.0, deltaS, betta, ppi,
    //						dB3, dL3, dA3);
    //		DeltaRungeKutta(azimuth - dA2 + 2*dA3, BB1 - dB2 + 2*dB3, deltaS, betta, ppi, dB4, dL4, dA4);
    //		DeltaRungeKutta(azimuth + (7*dA1+10*dA2+dA3)/27.0, BB1 + (7*dB1+10*dB2+dB3)/27.0,
    //						deltaS, betta, ppi, dB5, dL5, dA5);
    //		DeltaRungeKutta(azimuth + (28*dA1-125*dA2+546*dA3+54*dA4-378*dA5)/625.0,
    //			BB1 + (28*dB1-125*dB2+546*dB3+54*dB4-378*dB5)/625.0, deltaS, betta, ppi, dB6, dL6, dA6);
    //		//required data
    //		BB2 = BB1 + (dB1+4*dB3+dB4)/6.0;
    //		LL2 = LL1 + (dL1+4*dL3+dL4)/6.0;
    //	//	AA2 = azimuth + (dA1+4*dA3+dA4)/6.0;
    //		//local error
    //	/*	MdeltaB = GetPogreshnost(dB1, dB3, dB4, dB5, dB6);
    //		MdeltaL = GetPogreshnost(dL1, dL3, dL4, dL5, dL6);
    //		//if the error suits us
    //		if((MdeltaB<0.000000001)&&(MdeltaL<0.000000001))
    //		{
    //			//and the current step is the last (or the only thing)
    //			if((deltaS == S0)||(sigmaS == S0)) break;
    //			else//we redefine 'BB1' and 'LL1' and 'deltaS'
    //			{
    //				//we redefine initial data
    //				BB1 = BB2;
    //				LL1 = LL2;
    //				//we redefine a step
    //				sigmaS += deltaS;
    //				deltaS = S0-sigmaS;
    //			}
    //		}
    //		//if an error big, we continue to reduce a method step, and we don't change initial data
    //		else deltaS = deltaS/2 ;*/
    //	}
    //
    //	return MPoint( LL2*180.0/ppi,BB2*180.0/ppi);
}

//====================================
//! sets text size coefficient
//void MKartaInterface::SetTextRatio(float koef) {	textRatio = koef; }

//====================================
//! returns text size coefficient
//float MKartaInterface::GetTextRatio() { return textRatio; }

//====================================
//! returns the drawing window size in millimeters
MIntSize MKartaInterface::GetWidgetSizeMM()
//====================================
{
    if(!m_Painter) return MIntSize(0, 0);

    QPaintDevice* pd = m_Painter->device();
    MIntSize sz(pd->widthMM(), pd->heightMM());

    return sz;
}

//==================================
//! Loading of the file with the list of sheets
bool MKartaInterface::LoadSheetList(MString path)
//==================================
{
    ifstream in;
    in.open(path);
    if(!in.is_open()) return false;
    vect_sheet.clear();
    char st[20];
    while(!in.eof())
    {
        SheetInfo sinf;
        //reading name
        in >> st;
        sinf.name = st;
        //reading the left border
        in >> sinf.rect.left;
        //reading the right border
        in >> sinf.rect.right;
        //reading the upper border
        in >> sinf.rect.top;
        //reading the below border
        in >> sinf.rect.bottom;

        vect_sheet.push_back(sinf);
    }
    in.close();
    return true;
}

//==================================
//! Returns the name of a leaf to which the point got
MString MKartaInterface::GetSheetAtPoint(MPoint pt, MRect* rec)
//==================================
{
    for(unsigned int i = 0; i<vect_sheet.size(); i++)
        if(vect_sheet[i].rect.IsPtInRect(pt))
        {
            if(rec) *rec = vect_sheet[i].rect;
            return vect_sheet[i].name;
        }

    return "";
}

//==================================
//! Installs the filter of display of conventional signs
bool MKartaInterface::SetDrawFilter(MString path)
//==================================
{
    if(m_kls)
    {
        m_kls->SetVisibleFilter(path);
        return true;
    }
    else return false;
}

//////////////////////////////////////
//double MKartaInterface::GetScale()
//{
//    MRect geoRect = this->GetGeoBounds();
//    MRect logRect = this->GetZoombounds(); //  this->m_viewsize;

//    MPoint ptLT(geoRect.left,geoRect.top);
//    MPoint ptRB(geoRect.right,geoRect.bottom);
//    double d1 = this->GetDistance(ptLT, ptRB);

//    MPoint ptLT2(logRect.left,logRect.top);
//    MPoint ptRB2(logRect.right,logRect.bottom);
//    double d2 = this->GetDistance(ptLT2, ptRB2);

//    return d2 / d1;
//}



//writes down the specified region of 'geoRect' with 'scale' scale denominator in 'snap', at success returns 'true',
//if it isn't possible to create 'Qpixmap' under the specified region/scale returns 'false'
bool MKartaInterface::GetSnapshot(MRect &geoRect, double scale, QPixmap *&snap, bool drawCooGrid, bool drawScaleGrid)
{
    MRect zoomBackp = zoomRectangle;
    QPixmap	tmp_snap = QPixmap(10,10);
    int ptInM = tmp_snap.logicalDpiX()/0.0254;

    MPoint ptLT(geoRect.left,geoRect.top);
    MPoint ptRT(geoRect.right,geoRect.top);
    MPoint ptLB(geoRect.left,geoRect.bottom);

    double ltrt = GetDistance(ptLT, ptRT)/scale;
    double lblt = GetDistance(ptLB, ptLT)/scale;

    int pixWdth  = (int)(ltrt*ptInM);
    int pixHght  = (int)(lblt*ptInM);


    double  mPixLimit = 30.0;
    double nPix = double(pixWdth*pixHght/(mPixLimit*(1<<20)));
    if (false/*nPix>1.0*/)
    {
        pixWdth =(int)pixWdth/sqrt(nPix);
        pixHght = (int)pixHght/sqrt(nPix);
    }

    snap = new QPixmap(pixWdth,pixHght);
    if (snap->isNull())
        return false;

    QPainter m_Paint(snap);
    MQtDevice *m_SnapDevice = new MQtDevice(&m_Paint);
    MRect logRect(0.0F, 0.0F, pixWdth- 1.0,pixHght- 1.0);
    m_SnapDevice->SetCooSys(logRect, geoRect);
    m_SnapDevice->ClearAllAndInit(&m_Paint);
    m_karta->Risovanie(m_SnapDevice, this->GetScale());
    m_karta->DrawActBackground(m_SnapDevice);
    m_karta->RisovanieAct(m_SnapDevice);
    if(drawCooGrid)
        DrawCooGrid(m_SnapDevice,geoRect);
    if(drawScaleGrid)
        DrawScaleGrid(m_SnapDevice,geoRect);


    zoomRectangle = zoomBackp;
    delete m_SnapDevice;
    return true;
}

bool MKartaInterface::GetSnapshot(MRect &geoRect, QSize widgetSize, QPixmap *&snap, bool drawCooGrid, bool drawScaleGrid)
{
    MRect zoomBackp = zoomRectangle;

    int pixWdth = widgetSize.width();
    int pixHght = widgetSize.height();

    snap = new QPixmap(pixWdth,pixHght);
    if (snap->isNull())
        return false;

    QPainter m_Paint(snap);
    MQtDevice *m_SnapDevice = new MQtDevice(&m_Paint);
    MRect logRect(0.0F, 0.0F, pixWdth- 1.0,pixHght- 1.0);
    m_SnapDevice->SetCooSys(logRect, geoRect);
    m_SnapDevice->ClearAllAndInit(&m_Paint);
    m_karta->Risovanie(m_SnapDevice, this->GetScale());
    m_karta->DrawActBackground(m_SnapDevice);
    m_karta->RisovanieAct(m_SnapDevice);
    if(drawCooGrid)
        DrawCooGrid(m_SnapDevice,geoRect);
    if(drawScaleGrid)
        DrawScaleGrid(m_SnapDevice,geoRect);
    zoomRectangle = zoomBackp;
    delete m_SnapDevice;
    return true;
}

bool MKartaInterface::GetTile(int zoom, int x, int y, QImage *&img)
{
    MRect zoomBackp = zoomRectangle;

    int pixWdth = 256;
    int pixHght = 256;

    img = new QImage(pixWdth,pixHght,QImage::Format_ARGB32);
    if (img->isNull())
        return false;

    mercatortile::Tile stile{x,y,zoom};
    mercatortile::Bbox rbbox = xy_bounds(stile);
    MRect geoRect(rbbox.left,rbbox.top,rbbox.right,rbbox.bottom);
    QPainter m_Paint(img);
    MQtDevice *m_SnapDevice = new MQtDevice(&m_Paint);
    MRect logRect(0.0F, 0.0F, pixWdth- 1.0,pixHght- 1.0);
    m_SnapDevice->SetCooSys(logRect, geoRect);
    m_SnapDevice->ClearAllAndInit(&m_Paint);
    m_karta->Risovanie(m_SnapDevice, this->GetScale());
    m_karta->DrawActBackground(m_SnapDevice);
    m_karta->RisovanieAct(m_SnapDevice);
    zoomRectangle = zoomBackp;
    delete m_SnapDevice;
    return true;

}

void MKartaInterface::DrawScaleGrid(MPlotDevice* pDevice, MRect &gridrect)
{
    if(!m_karta) return;

    M_LOG_FONT   logfont;  // font (actually, only the name is used)
    //    strcpy((logfont.lfFaceName), "Arial");
    //    strcpy((logfont.lfFaceName), "Courier");
    logfont.lfEscapement = 0;
    logfont.lfUnderline = 0;
    logfont.lfStrikeOut = 0;
    logfont.lfItalic = 0;
    logfont.lfHeight = pDevice->LogicToBum(12);

    if (distinguishTextSize>8)
        logfont.lfHeight = pDevice->LogicToBum(distinguishTextSize); // 2005.01.26 vipa

    logfont.lfWeight = 0;
    logfont.lfWidth = 0;
    pDevice->SetTextColor(MColor(0, 0, 100));
    pDevice->SaveFont();
    pDevice->SetFont(&logfont);
    pDevice->SavePencil();

    double wMM = (double)pDevice->GetSizeMM().cx;
    MPoint p1(gridrect.left,  (gridrect.bottom+gridrect.top)/2.0);
    MPoint p2((gridrect.right+gridrect.left)/2.0, (gridrect.bottom+gridrect.top)/2.0);
    MPoint p3(gridrect.right, (gridrect.bottom+gridrect.top)/2.0);
    p1= this-> ConvertPoint(p1, false);
    p2= this-> ConvertPoint(p2, false);
    p3= this-> ConvertPoint(p3, false);
    double ds= this->GetDistance(p1, p2) + this->GetDistance(p2, p3);
    bool bkmFlag = true;
    double M0=  10.0 * ds/wMM/1000;// km in 1 sm

    double curSale  = 0.0;
    if(M0>1000)
        curSale = ceil(M0/1000)*1000;// длину линейки приводить к десяткам\сотням\тысячам километров
    else if(M0>100)
        curSale = ceil(M0/100)*100;
    else if(M0>10)
        curSale = ceil(M0/10)*10;
    else
        curSale = ceil(M0);


    double top=gridrect.bottom+gridrect.Height()/20;
    double right = gridrect.right-gridrect.Width()/50;
    double bottom = gridrect.bottom+gridrect.Height()/50;

    double left = 0;

    if(curSale*1000*4>ds)//максимальный размер линейки до четверти ширины виджета с картой
    {
        bkmFlag = false;
        left= right-gridrect.Width()/4;

        double dist = GetDistance(MPoint(left,bottom),MPoint(right,bottom));// m
        if(dist>1000)
            curSale = floor(dist/1000)*1000;// длину линейки приводить к десяткам\сотням\тысячам метров
        else if(dist>100)
            curSale = floor(dist/100)*100;
        else if(dist>10)
            curSale = floor(dist/10)*10;
        else
            curSale = floor(dist);
        curSale/=1000;
    }

    MPoint lb = GetPointByAzimuthLength(MPoint(right,bottom),270,curSale*1000);
    lb = ConvertPoint(lb);
    left=lb.x;
    MRect rect(left,top, right,bottom);
    double h = rect.Height();

    MColor color(39,61,63,100);
   // pDevice->FillRect(&rect,&color);

    MColor m_gridColor(0, 0, 0);
    M_LOG_PEN lpen_solid(PS_SOLID,   MPoint(0.0), m_gridColor);
    pDevice->SetPencil(&lpen_solid);
    pDevice->DrawLine(MPoint(left,bottom),MPoint(right,bottom));

    MString temp;
    if(bkmFlag)
        temp = (QString::number(curSale).toStdString()+"km");
    else
        temp = (QString::number(curSale*1000).toStdString()+"m");

    pDevice->TextOut(left, bottom+h/4 ,temp );

    pDevice->RestorePencil();
}
//drawing of a large-scale ruler
void MKartaInterface::DrawScaleGrid_old(MPlotDevice* pDevice, MRect &gridrect)
{  
    MColor m_gridColor(0, 0, 0);

    if(!m_karta) return;

    M_LOG_FONT   logfont;  // font (actually, only the name is used)
    //    strcpy((logfont.lfFaceName), "Arial");
    //    strcpy((logfont.lfFaceName), "Courier");
    logfont.lfEscapement = 0;
    logfont.lfUnderline = 0;
    logfont.lfStrikeOut = 0;
    logfont.lfItalic = 0;
    logfont.lfHeight = pDevice->LogicToBum(12);

    if (distinguishTextSize>8)
        logfont.lfHeight = pDevice->LogicToBum(distinguishTextSize); // 2005.01.26 vipa
    
    logfont.lfWeight = 0;
    logfont.lfWidth = 0;
    pDevice->SetTextColor(MColor(0, 0, 100));
    pDevice->SaveFont();
    pDevice->SetFont(&logfont);

    pDevice->SavePencil();


    //scaling
    //////////////////////////////

    double wMM = (double)pDevice->GetSizeMM().cx;        // double hMM = (double)pdm.heightMM();
    MRect zoom = this->GetZoombounds();
    MPoint p1(zoom.left,  (zoom.bottom+zoom.top)/2.0);
    MPoint p2((zoom.right+zoom.left)/2.0, (zoom.bottom+zoom.top)/2.0);
    MPoint p3(zoom.right, (zoom.bottom+zoom.top)/2.0);
    p1= this-> ConvertPoint(p1, false);
    p2= this-> ConvertPoint(p2, false);
    p3= this-> ConvertPoint(p3, false);
    double ds= this->GetDistance(p1, p2) + this->GetDistance(p2, p3);
    double M0=  10.0 * ds/wMM;
    int M = (int)(M0+5)/10;
    /////////////////////////////////

    int M1=M;
    while (div(M1,50).rem !=0)
    {  M1=M1+1;
    }
    float k =(float) M1*10/M;

    double left=gridrect.left+gridrect.Width()/50;
    double top=gridrect.bottom+gridrect.Height()/15;
    double right = gridrect.left+gridrect.Width()/4;
    double bottom = gridrect.bottom+gridrect.Height()/50;
    MRect rect(left,top, right,bottom);
    MColor color(230,230,230);
    pDevice->FillRect(&rect,&color);

    int x= gridrect.left+gridrect.Width()/40;
    int y=gridrect.bottom+gridrect.Height()/35;

    int x1=k*gridrect.Width()/pDevice->GetSizeMM().cx;
    int h=gridrect.Height()/150;
    pDevice->SavePencil();
    M_LOG_PEN lpen_solid(PS_SOLID,   MPoint((double)0), m_gridColor);
    pDevice->SetPencil(&lpen_solid);
    pDevice->DrawLine(MPoint(x, y+h),MPoint(x, y));
    MString temp("0 m");
    pDevice->TextOut(x, y + h * 2, temp, 5);
    int i=0;
    do{
        i++;
        QString str;
        if (M1*i/100.0>=1)
            str= QString::number(M1*i/100)+ " " + QObject::tr("km");
        else
            str= QString::number(M1*i*10)+ " " + QObject::tr("m");

        if (x+x1>right)
        {

            rect.left = rect.right;
            rect.right = x+x1+gridrect.Width()/40;
            pDevice->FillRect(&rect,&color);
        }

        MString s(str.toLatin1());
        pDevice->DrawLine(MPoint(x, y),MPoint((x+x1), y));
        pDevice->DrawLine(MPoint((x+x1), y+h),MPoint((x+x1), y));

        if (div(i,2).rem==0)
            pDevice->TextOut(x+x1-h*3, y+h*2,s);
        if(x+2*x1>=right)
            if (i<=2)pDevice->TextOut(x+x1-h*3, y+h*2,s);
        x+=x1;   }while(x+x1<right);

    pDevice->RestorePencil();
}


//! drawing of a grid is changed
void MKartaInterface::DrawCooGrid(MPlotDevice* pDevice, MRect &gridrect)
{
    if(!m_karta) return;

    MString str, str1, str2;
    MColor m_gridColor(0, 0, 0);

    M_LOG_FONT   logfont;  // font (actually, only the name is used)
    //    strcpy((logfont.lfFaceName), "Arial");
    //    strcpy((logfont.lfFaceName), "Courier");
    logfont.lfEscapement = 0;
    logfont.lfUnderline = 0;
    logfont.lfStrikeOut = 0;
    logfont.lfItalic = 0;
    logfont.lfHeight = pDevice->LogicToBum(8);

    if (distinguishTextSize>8)
        logfont.lfHeight = pDevice->LogicToBum(distinguishTextSize); // 2005.01.26 vipa
    
    logfont.lfWeight = 0;
    logfont.lfWidth = 0;
    pDevice->SetTextColor(MColor(0, 0, 100));
    pDevice->SaveFont();
    pDevice->SetFont(&logfont);

    pDevice->SavePencil();
    /*
    //'Ramka' Drawing

    M_LOG_PEN lpen1(PS_SOLID,MPoint(pDevice->LogicToBum(20)),MColor(0xff,0xff,0xff));
    M_LOG_PEN lpen2(PS_SOLID,MPoint(pDevice->LogicToBum(30)),MColor(0xff,0xff,0xff));

    MColor oldColor = pDevice->SetBkColor(MColor(155,155,155));

    MPoint ramkaLB, ramkaLT, ramkaRB, ramkaRT;

    ramkaLB.x = gridrect.left;
    ramkaLB.y = gridrect.bottom;
    ramkaLT.x = gridrect.left;
    ramkaLT.y = gridrect.top;
    ramkaRB.x = gridrect.right;
    ramkaRB.y = gridrect.bottom;
    ramkaRT.x = gridrect.right;
    ramkaRT.y = gridrect.top;

    pDevice->MoveTo(ramkaLB);
    pDevice->LineTo(ramkaLT);
    pDevice->SetPencil(&lpen1);
    pDevice->LineTo(ramkaRT);
    pDevice->SetPencil(&lpen2);
    pDevice->LineTo(ramkaRB);
    pDevice->SetPencil(&lpen1);
    pDevice->LineTo(ramkaLB);
*/
    MPoint ptLB, ptLT, ptRB, ptRT;

    ptLB.x = gridrect.left;
    ptLB.y = gridrect.bottom;
    ptLT.x = gridrect.left;
    ptLT.y = gridrect.top;
    ptRB.x = gridrect.right;
    ptRB.y = gridrect.bottom;
    ptRT.x = gridrect.right;
    ptRT.y = gridrect.top;

    // transfer of tops of a rectangle of 'm_zoombounds' to geographical coordinates
    double height=0;
    m_karta->m_pObjectCoo->ToBLH(ptLB, height, ptLB, height);
    m_karta->m_pObjectCoo->ToBLH(ptLT, height, ptLT, height);
    m_karta->m_pObjectCoo->ToBLH(ptRB, height, ptRB, height);
    m_karta->m_pObjectCoo->ToBLH(ptRT, height, ptRT, height);

    // calculation of the describing 'rect' rectangle
    MRect rect(ptLT.x,ptLT.y, ptRB.x,ptRB.y);
    rect.NormalizeGeoRect();

    double  w;
    double  stepx, stepy, x, y;
    int     n, m;
    double* grptx;
    double* grpty;
    stepx = 0.25;
    do
    {
        w= rect.Width() / stepx;
        if ( (int)w<10 ) break;
        stepx *=2;
    }
    while (true);

    double step= stepx;
    double bold_stepx= 2.0 * step;
    if (step == 1.0) { stepx = 1.0;  bold_stepx= 3.0;  }
    if (step == 2.0) { stepx = 1.0;  bold_stepx= 3.0;  }
    if (step == 4.0) { stepx = 3.0;  bold_stepx= 6.0;  }
    if (step >  4.0) { stepx = 6.0;  bold_stepx= 30.0; }
    if (step >  8.0) { stepx = 30.0; bold_stepx= 30.0; }

    /*
  if (stepx == 2.0) stepx = 1.0;   if (stepx == 4.0) stepx = 3.0;    if (stepx >  4.0) stepx = 6.0;
*/
    // alignment of knots of a grid on points, multiple to a grid step on axis X
    int i = 0, j = 0;

    int ix = (int)(rect.left / stepx);

    /*if   (ix*stepx == rect.left) x=rect.left; //-ATTENTION-
    else x= ix*stepx + stepx;
     */
    x= ix*stepx; // + stepx;
    if (ix>0) x +=stepx;

    n = (int)(rect.Width()/stepx) + 1; // 'n' - number of points of a geographical grid on axis X
    grptx = (double*)malloc(sizeof(double)*n);
    // filling of the 'grptx' array of knots of a grid on axis X
    for(i=0; i<n; i++) {
        grptx[i] = x;
        x += stepx;
        if ( x > rect.right ) {
            n = i+1;
            break;
        }
    }

    // calculation of a step of a grid on axis Y
    stepy = (double)1.0/6.0;
    /* //-ATTENTION-
  if (stepx == 0.5) stepy = (double)1/3;
  if (stepx == 1.0) stepy = (double)2/3;
  if (stepx == 3.0) stepy = 2.0;
  if (stepx == 6.0) stepy = 4.0;
  */
    double bold_stepy= 2.0 * stepy;
    if (stepx == 0.5)  { stepy = 1.0/3.0;      bold_stepy= 2.0/3.0;  }
    if (stepx == 1.0)  { stepy = 2.0/3.0;      bold_stepy= 2.0;      }
    if (stepx == 3.0)  { stepy = 2.0;          bold_stepy= 4.0;      }
    if (stepx == 6.0)  { stepy = 4.0;          bold_stepy= 30.0;     }
    if (stepx == 30.0) { stepy = 30.0;         bold_stepy= 30.0;     }

    //
    int iy = (int)(rect.bottom / stepy);
    if   (iy*stepy == rect.bottom) y=rect.bottom;
    else y= iy*stepy + stepy;
    m = (int)(rect.Height()/stepy) + 1; // 'n' - number of points of a geographical grid on axis X
    grpty = (double*)malloc(sizeof(double)*m);
    // filling of the 'grptx' array of knots of a grid on axis X
    for(i=0; i<m; i++) {
        grpty[i] = y;
        y += stepy;
        if ( y > rect.top ) {
            m = i+1;
            break;
        }
    }

    ptLT.x = gridrect.left;
    ptLT.y = gridrect.top;
    ptRB.x = gridrect.right;
    ptRB.y = gridrect.bottom;

    MPoint* grpt;
    grpt = (MPoint*)malloc(sizeof(MPoint)*(n+2));

    //	pDevice->SetROP2(0x00b906e6);
    //	pDevice->SetBkMode(UT_TRANSPARENT);
    /* //-ATTENTION-
    MColor cl = m_gridColor;
    M_LOG_PEN lpen(Qt::DotLine, MPoint((double)0), cl);
    pDevice->SetPencil(&lpen);
    */
    M_LOG_PEN lpen_solid(PS_SOLID,   MPoint((double)0), m_gridColor);
    M_LOG_PEN lpen_dash (PS_DOTGRID, MPoint((double)0), m_gridColor);

    // drawing of parallels

    for(j=0; j<m; j++) {
        grpt[0].x = rect.left;
        grpt[0].y = grpty[j];
        m_karta->m_pObjectCoo->FromBLH(grpt[0], height, grpt[0],height);

        for(i=0; i<n; i++) {
            grpt[i+1].x = grptx[i];
            grpt[i+1].y = grpty[j];
            m_karta->m_pObjectCoo->FromBLH(grpt[i+1], height, grpt[i+1],height);
        }
        grpt[n+1].x = rect.right;
        grpt[n+1].y = grpty[j];
        m_karta->m_pObjectCoo->FromBLH(grpt[n+1], height, grpt[n+1],height);

        //new --------->
        int  id = (int)(fabs(grpty[j])+0.0001);

        if (abs(id) <= 90)  {
            int k;
            if (grpty[j] >=0.0)  k= (int)(grpty[j]/bold_stepy+0.01);
            else                 k= (int)(grpty[j]/bold_stepy-0.01);
            if ( fabs(grpty[j] - bold_stepy*(double)k )< 0.1 )
                pDevice->SetPencil(&lpen_solid);
            else  pDevice->SetPencil(&lpen_dash);
            //<--------------



            //! since qt3
            //pDevice->MoveTo(grpt[0]);
            //for(i=0; i<n+1; i++)
            //	pDevice->LineTo(grpt[i]);
            for(i=0; i<n+1; i++)
                pDevice->DrawLine(grpt[i],grpt[i+1]);

            //int  id = (int)(grpty[j]+0.01); -ATTENTION-
            str1.Format("%2d", id);
            str1 +=  "\xb0";//"\u00B0";
            //str2.Format("%3d", (int) (100.01 + 60.0 *(grpty[j]-id) ) ); --ATTENTION
            if (grpty[j] <0)  str1= "-"+str1;
            str2.Format("%3d", (int) (100.001 + 60.0 *(fabs(grpty[j])-id) ) );
            str2 = str2.Right(2);
            str2 += "\'";

            double dBum10 = pDevice->LogicToBum(14);
            //		pDevice->SetTextAlign(UT_TA_LEFT);
            pDevice->TextOut(ptLT.x+dBum10/2, grpt[0].y, str1, str1.GetLength());
            pDevice->TextOut(ptLT.x+dBum10/2, grpt[0].y-dBum10, str2, str2.GetLength());
            //		pDevice->SetTextAlign(UT_TA_RIGHT);
            pDevice->TextOut(ptRB.x-3*dBum10/2, grpt[n+1].y, str1, str1.GetLength());
            pDevice->TextOut(ptRB.x-3*dBum10/2, grpt[n+1].y-dBum10, str2, str2.GetLength());

        }//EndOf - if (abs(id) <= 90)

    }
    free(grpt);
    grpt = (MPoint*)malloc(sizeof(MPoint)*(m+2));

    // drawing of meridians
    for(j=0; j<n; j++) {
        grpt[0].x = grptx[j];
        grpt[0].y = rect.bottom;
        m_karta->m_pObjectCoo->FromBLH(grpt[0], height, grpt[0],height);

        for(i=0; i<m; i++) {
            grpt[i+1].x = grptx[j];
            grpt[i+1].y = grpty[i];
            m_karta->m_pObjectCoo->FromBLH(grpt[i+1], height, grpt[i+1],height);
        }
        grpt[m+1].x = grptx[j];
        grpt[m+1].y = rect.top;
        m_karta->m_pObjectCoo->FromBLH(grpt[m+1], height, grpt[m+1],height);
        //new ---->
        int k;
        if (grptx[j] >=0.0)  k= (int)(grptx[j]/bold_stepx+0.01);
        else             k= (int)(grptx[j]/bold_stepx-0.01);
        if ( fabs(grptx[j] - bold_stepx*(double)k )< 0.1 )
            pDevice->SetPencil(&lpen_solid);
        else  pDevice->SetPencil(&lpen_dash);
        //<---------


        for(i=0; i<m+1; i++)
            pDevice->DrawLine(grpt[i],grpt[i+1]);


        //int  id = (int)(grptx[j]+0.01); //-ATTENTION-
        //str1.Format("%2d", id);         //-ATTENTION-
        int  id = (int)(fabs(grptx[j])+0.0001);
        str1.Format("%d", id);
        str1 = str1 + "\u00B0";
        if (grptx[j] <0)  str1= "-"+str1; //NEW
        str2.Format("%3d", (int) (100.01 + 60.0 *(fabs(grptx[j])-id) ) );//NEW
        //str2.Format("%3d", (int) (100.01 + 60.0 *(grptx[j]-id) ) );

        str2 = str2.Right(2) + "\'";
        str1 += str2;

        double dBum12 = pDevice->LogicToBum(14);
        //		pDevice->SetTextAlign(UT_TA_RIGHT|UT_TA_TOP);
        pDevice->TextOut(grpt[0].x-dBum12 , ptRB.y+dBum12, str1, str1.GetLength());
        ////		pDevice->SetTextAlign(UT_TA_LEFT|UT_TA_TOP);
        ////		pDevice->TextOut(grpt[0].x, ptRB.y+dBum12, str2, str2.GetLength());
        //		pDevice->SetTextAlign(UT_TA_RIGHT|UT_TA_BOTTOM);
        pDevice->TextOut(grpt[m+1].x-dBum12, ptLT.y-dBum12, str1, str1.GetLength());
        //		pDevice->SetTextAlign(UT_TA_LEFT|UT_TA_BOTTOM);
        //		pDevice->TextOut(grpt[m+1].x, ptLT.y-dBum12, str2, str2.GetLength());
    }

    pDevice->RestoreFont();
    pDevice->RestorePencil();
    //	pDevice->SetBkColor(oldColor); //-ATTENTION-

    free(grptx);
    free(grpty);
    free(grpt);
}
#include <iostream>
//#include "Coo.h"

void MKartaInterface::DrawMGRSCooGrid(MPlotDevice *pDevice, MRect &gridrect)
{
    int scale = GetScale();
    M_LOG_FONT   logfont;  // font (actually, only the name is used)
    //    strcpy((logfont.lfFaceName), "Arial");
    //    strcpy((logfont.lfFaceName), "Courier");
    logfont.lfEscapement = 0;
    logfont.lfUnderline = 0;
    logfont.lfStrikeOut = 0;
    logfont.lfItalic = 0;
    logfont.lfHeight = pDevice->LogicToBum(8);

    if (distinguishTextSize>8)
        logfont.lfHeight = pDevice->LogicToBum(distinguishTextSize); // 2005.01.26 vipa

    logfont.lfWeight = 1;
    logfont.lfWidth = 0;
    pDevice->SetTextColor(MColor(0, 0, 0));
    pDevice->SaveFont();
    pDevice->SetFont(&logfont);

    pDevice->SavePencil();
    double dBum12 = pDevice->LogicToBum(14);
    MColor m_gridColor(0, 0, 0);
    M_LOG_PEN lpen_dash (PS_DOTGRID, MPoint((double)0.0), m_gridColor);
    M_LOG_PEN lpen_solid(PS_SOLID,   MPoint((double)0.0), m_gridColor);
    pDevice->SavePencil();

    QVector<int> vScales = {10000,1000,400};
    QVector<int> vSteps = {100000,10000,1000};

   // MUtmCoo utm;
   // MPoint utmPt;
   // MPoint utmPtbot;
   // double hgf;

    double stepXDegree = 6;
    double stepYDegree = 8;

    int Xbegin = (int(floor(gridrect.left/stepXDegree))+1)*stepXDegree;
    int Xend = int(floor(gridrect.right/stepXDegree))*stepXDegree;
    int numStepX = (Xend-Xbegin)/stepXDegree;

    int Ybegin = (int(floor(gridrect.bottom/stepYDegree))+1)*stepYDegree;
    int Yend = int(floor(gridrect.top/stepYDegree))*stepYDegree;
    int numStepY = (Yend-Ybegin)/stepYDegree;

    QVector<double> XDegree, YDegree; // grid by degree

    XDegree.push_back(Xbegin-stepXDegree);
    YDegree.push_back(Ybegin-stepYDegree);
    for(int i = 0; i<=numStepX+1;++i)
    {
        double left = Xbegin+stepXDegree*(i-1);
        double right = Xbegin+stepXDegree*i;
        XDegree.push_back(right);

        for(int j = 0; j<=numStepY+1;++j)
        {
            double top = Ybegin+stepYDegree*j;
            double bottom = Ybegin+stepYDegree*(j-1);
            YDegree.push_back(top);
            MRect currentRect(left,top,right,bottom);

            for(int k = 0; k<vScales.count();++k)
            {
                int curScale = vScales.at(k);
                if(scale < curScale)
                {
                    if(scale<curScale/10)
                        pDevice->SetPencil(&lpen_solid);
                    else
                      pDevice->SetPencil(&lpen_dash);

                    DrawMGRSStepGrid(pDevice,currentRect,vSteps.at(k));
                    pDevice->RestorePencil();
                }
                else
                    break;
            }


        }
    }
   pDevice->SetPencil(&lpen_solid);
   //meridians 6 degrees
    for(int i = 0; i<=numStepX+1;++i)
    {
        for(int j = 0; j<=numStepY+1;++j)
        {
            MPoint pt1m (XDegree.at(i),YDegree.at(j)) ;           
            QString str(MGRS::GetTileString(pt1m.x,pt1m.y).c_str());
            MString str1(str.toStdString());
            pDevice->TextOut(pt1m.x+dBum12 , pt1m.y+dBum12, str1);
            MPoint pt2m (XDegree.at(i),YDegree.at(j+1));
            pDevice->DrawLine(pt1m,pt2m);
        }
    }
    // parallels 8 degrees
    for(int j = 0; j<=numStepY+1;++j )
    {
        for(int i = 0; i<=numStepX+1;++i)
        {
            MPoint pt1p (XDegree.at(i),YDegree.at(j)) ;
            MPoint pt2p (XDegree.at(i+1),YDegree.at(j));
            pDevice->DrawLine(pt1p,pt2p);
        }
    }

}

void MKartaInterface::DrawMGRSStepGrid(MPlotDevice *pDevice, MRect &rect, double step)
{
    // work for nothen hemisphere
    MRect intersectionRect;
    IntersectionGeoRect(&rect,&m_zoombounds,intersectionRect);

    QVector<QVector<MPoint>> vPointsStep100Km;
    MPoint pt1 (intersectionRect.left,intersectionRect.top);
    MPoint pt2 (intersectionRect.right,intersectionRect.top);
    MPoint pt1bot (intersectionRect.left,intersectionRect.bottom);
    MPoint pt2bot (intersectionRect.right,intersectionRect.bottom);
    MPoint equatorLeft(pt1.x,0.0);
    double dDistanceBottom = GetDistance(equatorLeft,pt1bot);
    double dDistanceTop = GetDistance(equatorLeft,pt1);
    int newB = (int(floor(dDistanceBottom/step)))*step;
    int newT = (int(floor(dDistanceTop/step)))*step;
    int iStepY = (newT-newB)/step;// колличество целых шагов по Y

    vPointsStep100Km.push_back(GetMGRSStepLine(pt1bot,pt2bot,step));
    for(int i = 1; i<=iStepY;++i)
    {      
        MPoint pt1p = GetPointByAzimuthLength(MPoint (intersectionRect.left,0.0),0,(int(floor(dDistanceBottom/step))+i)*step);
        MPoint pt2p = GetPointByAzimuthLength(MPoint (intersectionRect.right,0.0),0,(int(floor(dDistanceBottom/step))+i)*step);
        vPointsStep100Km.push_back(GetMGRSStepLine(pt1p,pt2p,step));
    }

   //draw

    double dBum12 = pDevice->LogicToBum(14);

    for(int i = 1; i<vPointsStep100Km.count();++i)
    {
        QVector<MPoint> vecPoint = vPointsStep100Km.at(i);

        // horizont
        pDevice->DrawLine(vecPoint.first(),vecPoint.back());
        if(step>10000)
            for(int j = 1;j<vecPoint.count();++j)
            {
                MPoint cross = vecPoint.at(j);
                double move =0.01;
                cross = MPoint(cross.x+move,cross.y+move);// move the intersection point to the first quarter                
                QString str = MGRS::GetTileString(cross.x,cross.y,true).c_str();
                MString str1(str.toStdString());
                pDevice->TextOut(cross.x+dBum12/4 , cross.y+dBum12/4, str1);
            }
    }

    //vertical

    vPointsStep100Km.push_back(GetMGRSStepLine(pt1,pt2,step));// add only for drawing
    for(int i = 0; i<vPointsStep100Km.count()-1;++i)
    {
        QVector<MPoint> top = vPointsStep100Km.at(i+1);
        QVector<MPoint> bottom  = vPointsStep100Km.at(i);
        double nColTop = top.count();
        double nColBot = bottom.count();

        double distTop = GetDistance(top.at(0),top.at(1));
        double distBot = GetDistance(bottom.at(0),bottom.at(1));

        if(nColTop==nColBot)
        {
            if(step< distBot*10 && distTop> distBot*3)
            {
                for(int j= 0;j<nColTop-1;++j)
                    pDevice->DrawLine(top.at(j),bottom.at(j+1));
            }
            else if(step< distTop *10 && distBot>distTop*5)
            {
                for(int j= 0;j<nColTop-1;++j)
                    pDevice->DrawLine(top.at(j+1),bottom.at(j));
            }
            else
            {
                for(int j= 1;j<nColTop-1;++j)
                        pDevice->DrawLine(top.at(j),bottom.at(j));
            }
        }
        if(nColBot-nColTop==2)
        {
            for(int j= 0;j<nColTop-1;++j)          
               pDevice->DrawLine(top.at(j),bottom.at(j+1));
        }
        if( nColBot-nColTop==1)
        {
            bool bottomPlus = distTop> distBot*3;
            for(int j= bottomPlus?0:1;j<nColTop;++j)          
               pDevice->DrawLine(top.at(j),bottom.at(bottomPlus?j+1:j));
        }
        if( nColTop-nColBot==1)
        {
            for(int j = 1;j<nColBot;++j)           
               pDevice->DrawLine(top.at(j),bottom.at((step< distBot*10 && distBot> distTop*3)?j-1:j));
        }
    }

       pDevice->RestorePencil();
}

QVector<MPoint> MKartaInterface::GetMGRSStepLine(MPoint begin, MPoint end, double step)
{
    QVector<MPoint> resultVector;
    MUtmCoo utm;
    MPoint utmPt;
    double hgf;
    utm.FromBLH(begin,0,utmPt,hgf);
    int zone  = (utm.m_meridian+183)/6;
    utmPt.x -= 1000000.0*zone;
    double distForFirstPoint =((int)floor(utmPt.x / step)+1)*step-utmPt.x ;
    double rectWidth = GetDistance(begin,end);
    double nCol = int(floor(rectWidth/step));
    resultVector.push_back(begin);

    if(distForFirstPoint>rectWidth)
    {
        resultVector.push_back(end);
        return resultVector;
    }

    MPoint firstTopLeft= GetPointByAzimuthLength(begin,90,distForFirstPoint);
    firstTopLeft.y = begin.y;
    resultVector.push_back(firstTopLeft);

    for(int i = 1;i<=nCol;++i)
    {
        MPoint next = GetPointByAzimuthLength(firstTopLeft,90,step);
        if(next.x > end.x)
        {
            resultVector.push_back(end);
            return resultVector;
        }
        else
        {
            next.y = firstTopLeft.y;
            firstTopLeft = next;
        }
        resultVector.push_back(firstTopLeft);       
    }

    resultVector.push_back(end);
    return resultVector;
}

bool MKartaInterface::DrawSearchPoint(const double &lat, const double &lon)
{
    MPoint pt(lon,lat);
    pt = ConvertPoint(pt);
     if(!m_zoombounds.IsPtInRect(pt))
     {
         // message         
         QMessageBox::information(nullptr,QObject::tr("Search point"),QObject::tr("Entered coordinates outside the map."));
         return false;
     }
    if(FindActLayer(ID_ACTLAYER_FindPoint))
    {
        SetWorkingActLayer(ID_ACTLAYER_FindPoint);
        DeleteObject(ID_ACTOBJECT_FindPoint);
    }
    else
    {
        AddActLayer(ID_ACTLAYER_FindPoint);
        SetWorkingActLayer(ID_ACTLAYER_FindPoint);
    }
    AddObject(ID_ACTOBJECT_FindPoint);
    SetWorkingObject(ID_ACTOBJECT_FindPoint);

    AddPoint(pt);

    if(!SetZnak(kkod_FindPoint))
    {
        SetWorkingActLayer(ID_ACTLAYER_FindPoint);
        DeleteObject(ID_ACTOBJECT_FindPoint);
    }
    return true;
}

void MKartaInterface::RemoveSearchPoint()
{
    if(FindActLayer(ID_ACTLAYER_FindPoint))
    {
        SetWorkingActLayer(ID_ACTLAYER_FindPoint);
        DeleteObject(ID_ACTOBJECT_FindPoint);
    }
}



//===============================================================
// drawing of object - a search result according to the name
void MKartaInterface::DrawFoundByNameObject(MPlotDevice* pDevice)
//===============================================================
{
    if (!m_pFoundOblist) return;

    MPoint centrePt, newPt;
    double h;
    double d1Pix = pDevice->LogicToBum(1);
    double d2Pix = pDevice->LogicToBum(8);

    if (m_pFoundOblist->size() < 1) return;

    //find centers of the objects

    MTopobject* tpo;
    MColor colorRed(255, 0, 200);

    std::for_each(m_pFoundOblist->begin(), m_pFoundOblist->end(), [&] (MObject* it)
    {
        tpo = (MTopobject*)it;

        //find center of the object
        centrePt = tpo->GetPoint(0);

        if (tpo->GetPointsNumber() > 1)
            centrePt = MPoint(tpo->m_bounds.CenterX(), tpo->m_bounds.CenterY());

        pDevice->SaveZapolnenie();
        pDevice->SavePencil();

        M_LOG_PEN lpen_solid(PS_SOLID, MPoint(d1Pix), colorRed);

        pDevice->SetZapolnenie(&colorRed);
        pDevice->SetPencil(&lpen_solid);

        m_karta->m_pObjectCoo->FromBLH(centrePt, 0, newPt,h);

        pDevice->Circle(centrePt, d2Pix, colorRed);

        pDevice->RestoreZapolnenie();
        pDevice->RestorePencil();
    });
}

//============================================
//! Sets degree of transparency of vulgar objects of an operational layer
bool  MKartaInterface::SetActPolygonDense(int dns) 
{

    if ((dns>=0)&&(dns<=14)) actPolygonDense= dns;
    else return false;
    return true;
}
//! Returns degree of transparency of vulgar objects of an operational layer
int   MKartaInterface::GetActPolygonDense() { return actPolygonDense; }

//! Sets correction coefficient on the size of inscriptions
bool  MKartaInterface::SetTextRatio(float ratio) 
{
    //if ((ratio>=1.0)&&(ratio<=6.0)) //-ATTENTION-
    if ((ratio>=0.5)&&(ratio<=10.0))
        textRatio= ratio;
    else return false;

    return true;
}

//! Returns correction coefficient on the size of inscriptions
float MKartaInterface::GetTextRatio()   { return textRatio; }

//! Establishes 'mrd' value of the central meridian in degrees
bool  MKartaInterface::SetCentralMeridian(int mrd) 
{
    if (mrd>180)  return false;
    if (mrd<-180) return false;  //-ATTENTION- //earlier: if (mrd<0)
    centralMeridian= mrd;
    return true;
}

//==============================================
// Sets value of the central meridian for Gauss-Krueger's system
//==============================================
bool MKartaInterface::SetExtendedCentralMeridian(double meridian)
{
    /*if(!m_karta->m_pObjectCoo->WhatTheNameCoo().Compare(COO_NAME_ZONEGAUSSKRUGER))
    {
        MExtendedGaussKrugerCoo* m_extCoo = new MExtendedGaussKrugerCoo();
        m_extCoo->SetStandartEllipsoid(m_karta->m_pObjectCoo->m_ellipsoidname);
        m_extCoo->m_meridian = meridian;
        m_karta->ConvertKartaToCoo(m_extCoo);
        return true;
    }

    return false;*/

    //if system Zonal Gauss-Krueger, we transform to the expanded
    if(!m_karta->m_pObjectCoo->WhatTheNameCoo().Compare(COO_NAME_ZONEGAUSSKRUGER))
    {
        //we create a clone of the current system of coordinates with a new meridian
        MExtendedGaussKrugerCoo* m_extCoo = new MExtendedGaussKrugerCoo();
        m_extCoo->Copy(m_karta->m_pObjectCoo);
        //we change the document
        m_karta->ConvertKartaToCoo(m_extCoo);
        //we redefine the current system of coordinates
        delete m_karta->m_pObjectCoo;
        m_karta->m_pObjectCoo = m_extCoo;
    }
    //we change a meridian only for expanded system of Gauss-Krueger
    if(m_karta->m_pObjectCoo->WhatTheNameCoo().Compare(COO_NAME_EXTENDEDGAUSSKRUGER)) return false;
    else
    {
        //we create a clone of the current system of coordinates with a new meridian
        MExtendedGaussKrugerCoo* m_extCoo = new MExtendedGaussKrugerCoo();
        m_extCoo->Copy(m_karta->m_pObjectCoo);
        m_extCoo->m_meridian = meridian;

        m_karta->ConvertKartaToCoo(m_extCoo);

        //we redefine the current system of coordinates
        delete m_karta->m_pObjectCoo;
        m_karta->m_pObjectCoo = m_extCoo;
        SetCentralMeridian(meridian);

        return true;
    }
}

//==============================================
//! we establish a symbol angle of rotation for dot object
//! affects all objects drawn by the same sign
bool MKartaInterface::SetAngle(int angle, double uid)
//==============================================
{
    MTopobject* obj;
    if (uid) obj = FindObject(uid);
    else     obj = m_Object;

    if(!obj)          return false;
    if(!obj->m_pZnak) return false;

    MYaz* pYaz=obj->m_pZnak->GetYaz();
    if(!pYaz)         return false;

    if(pYaz->GetType() != F_DOT) return false;

    MSzArray* szar = ((MDotYaz*)(pYaz->m_object))->m_tochka;
    MSrez* srez=szar->GetAt(0);
    if(srez->GetType() != IDT_SHRIFT) return false;

    MSymbol* sym = &((MShriftSrez*)srez)->m_tochka;
    /*sym->logfont.lfEscapement = angle;*/

    MPtArray* pta = (MPtArray*)obj->m_Komplekt->GetAt(obj->F_TOCHKI);
    int iSize = pta->GetSize();

    if(iSize==0) return false;

    if(iSize>1)
        for(int i=iSize-1; i>0; i--)
            pta->DeleteTail();

    double radangle = pi*angle/180.0;
    MPoint point = pta->GetAt(0);
    MPoint pointNew = MPoint(point);
    pointNew.x += sym->value*sin(radangle);
    pointNew.y += sym->value*cos(radangle);

    pta->AddTail(&pointNew);

    return true;
}

//==============================================
//! we establish a symbol angle of rotation for dot object
//! uses the 'angle of rotation of dot object' attribute [43]
bool   MKartaInterface::SetObjAngle( int angle, double uid)
{
    MTopobject* obj;
    MSzArray* szar;
    MSrez* srez;
    MNastrShriftSrez* pNShSr;
    double dTemp;
    short perIndex, iSize, i;
    MString str;

    if (uid)
        obj = FindObject(uid);
    else
        obj = m_Object;

    if(!obj)          return false;
    if(!obj->m_pZnak) return false;

    MYaz* pYaz=obj->m_pZnak->GetYaz();
    if(!pYaz)         return false;

    if(pYaz->GetType() != F_DOT) return false;

    szar = ((MDotYaz*)(pYaz->m_object))->m_tochka;
    srez = szar->GetAt(0);
    if(srez->GetType() != IDT_NAS_SHRIFT) return false;
    //TR: po vsem srezam . vse oni dolzhny byt' IDT_NAS_SHRIFT
    iSize = szar->GetSize();
    for (i =0;i<iSize;i++)
    {
        srez=szar->GetAt(i);
        pNShSr = (MNastrShriftSrez*)srez;

        if(!pNShSr->IsPereoprIndex(IDT_NAS_SHRIFT_VAL_SYM_UGOL))
            continue;

        dTemp = angle*10;

        perIndex = pNShSr->GetPerIndex(IDT_NAS_SHRIFT_VAL_SYM_UGOL);
        if(perIndex == -1)
            continue;
        str.Format("%f",dTemp);
        obj->SetDataPole(str,perIndex);
    }
    return true;
}

//! Returns the array of geo-points of a circle with the 'pnt' center and 'rad' radius of meters
MPtArray MKartaInterface::CycleGeoPoints(const MPoint &pnt, int radius)  {

    MPtArray arr;
    MRect geoBounds(-180.0,90.0,180.0,-90.0);
    MPoint   geoCpt = ConvertPoint(pnt,false);
    MPoint geoPt;

    for(int i=0; i<360; i+5)	{
        geoPt = GetPointByAzimuthLength(geoCpt,i,radius);
        if (geoBounds.IsPtInRect(geoPt))
            arr.AddTail((MPoint*) &geoPt );
    }
    return arr;
}
//! Fills array 'arr' with GEO-points (to make circle) with centre at 'pnt'
//! and radius = 'radius'.
//! Make sure to Create and Delete 'arr' - Array of geo-points OUTSIDE this function.
int MKartaInterface::CycleGeoPoints(const MPoint &pnt,MPtArray* arr, int radius)
{ 
    return  CycleGeoPoints(pnt,arr,radius,5);

}

//! Fills the 'arr' array of geo-points of a circle with the 'pnt' center, 'rad' radius of meters
//! and 'degStp' step in degrees. Returns quantity of points in the array.
int MKartaInterface::CycleGeoPoints(const MPoint &pnt,MPtArray* arr, int radius, double degStp)
{ 

    MRect glob(-180.0,90.0,180.0,-90.0);
    MPoint  g_pnt = ConvertPoint(pnt,false);
    MPoint nextPt;
    int nStp = (int)360/degStp;
    for(int i=0; i<nStp+1; i++)
    {
        nextPt = GetPointByAzimuthLength(pnt,i*degStp,radius);
        if (glob.IsPtInRect(nextPt))
            arr->AddTail(&nextPt);
    }
    return arr->Size;
}

//! ----------------------------------------
//! Returns type(F_PLO, F_DOT, F_LIN)of object with uid = 'uid' or uid = 0 (current selected object).
//! returns -1, if object wasn't found.
int MKartaInterface::GetObjectType(double uid) {

    MTopobject* obj;
    if (uid == 0) obj = m_Object;
    else          obj = FindObject(uid);
    if(!obj)  {
        InternalMessage("MKartaInterface::GetObjectType(double)  -Object  not found!");
        return -1;
    }
    if(!obj->m_pZnak) return -1;
    return (int) obj-> m_pZnak-> GetYaz()-> GetType();
    //  MPtArray* pta = (MPtArray*)obj->m_Komplekt->GetAt(obj->F_TOCHKI);
}

// 2005.01.12  vipa
//! Returns object type(F_PLO, F_DOT, F_LIN) from given quad-code 'kkod'.
//! returns -1, if quad-code wasn't found in Klassifikator
int MKartaInterface::GetObjectType(MString &kkod)
{
    MString truekkod;
    MZnak* znak = m_kls->GetZnak(kkod, truekkod, true);
    if(!znak)  {
        InternalMessage("MKartaInterface::GetObjectType(MString&)  -`"+QString(kkod.c_str())+"` not found!");
        return -1;
    }
    return (int)(znak->GetYaz()->GetType());
}

//!
bool MKartaInterface::SetWorkingObjectAtLayers(double uid)
{
    if(!m_karta) return false;
    LAYITER layit = m_karta->m_actlaylist.begin();
    while(layit != m_karta->m_actlaylist.end())
    {
        OBITER obit = (*layit)->m_oblist.begin();
        while(obit != (*layit)->m_oblist.end())
        {
            if ((*obit)->m_UID == uid) {
                m_actLayer = (*layit);
                m_Object   = (MTopobject*)(*obit);
                return true;
            }
            obit++;
        }
        layit++;
    }
    InternalMessage("MKartaInterface::SetWorkingObjectAtLayers(double)  -Object  not found!");
    return false;
}

//! ==================================
bool MKartaInterface::CheckSelectedObjectAtLayers(double uid) {
    if(!m_karta) return false;
    LAYITER layit = m_karta->m_actlaylist.begin();
    while(layit != m_karta->m_actlaylist.end())  {
        OBITER obit = (*layit)->m_oblist.begin();
        while(obit != (*layit)->m_oblist.end())  {
            if ((*obit)->m_UID == uid) return (*obit)->m_selected;
            obit++;
        }
        layit++;
    }
    return false;
}

//! To set the fixed size (in pixels) an image of the map
//! It is used for preservation of scale of the image of the map at the press
bool MKartaInterface::SetPixmapSize(QSize sz)
{
    //	CreatePaintDevice();
    if ((sz.width() ==0)&&(sz.height()==0))  {
        m_pixmap_resizeFlag= true;
        CreatePaintDevice();
        return true;
    }

    //	if(m_widget)
    //	{
    if(m_Painter)
    {
        m_Painter->end();
        delete m_Painter;
    }
    m_Painter = new QPainter();
    m_Painter->setRenderHint(QPainter::Antialiasing,false);//max


    if (m_pixmap) delete m_pixmap;
    if (m_pixmapact) delete m_pixmapact;

    m_pixmap = new QPixmap(sz);
    m_pixmapact = new QPixmap(sz);

    m_pixmap_resizeFlag= false;

    m_Painter->begin(m_pixmapact);

    m_viewsize.x = sz.width();
    m_viewsize.y = sz.height();
    CreateQtDevice();
    //	}
    //  else  return false;

    return true;
}

//==================================
//! search of objects in the set layer (m_parLayerName) - the Name of a layer, a line
//! on a preset value (m_parValue) - a line
//! a certain attribute (m_parAttrName) - the name of attribute, a line
bool MKartaInterface::FindObjInLayerByAttrValue(MString m_LayerName,MString m_Value, MString m_AttrName)
//==================================
{
    if (!m_karta) return false;

    m_pFoundOblist = m_karta->FindOblistInLayerByAttrValue(m_LayerName, m_Value, m_AttrName);

    if (!m_pFoundOblist) return false;

    return true;
}

//==================================
//! search of quad-code of objkt of the set type (iObjType) in the set layer (m_LayerName)
//! the type of object is set by a constant (F_PLO, F_DOT, F_LIN [Yaz.h]), a layer - by name
//! returns the list of the found quad-code
QStringList MKartaInterface::FindKvadrokodesInLayerByObjType(MString m_LayerName, int iObjType)
//==================================
{
    if (!m_karta) return QStringList();

    return m_karta->FindKvadrokodesInLayerByObjType(m_LayerName, iObjType);
}

//========================================================
//Returns the array of the points forming an arch with a development angle in degrees
MPtArray* MKartaInterface::GetDuga(MPoint* pt1, MPoint* pt2, MPoint* pt3, int index)
//========================================================
{	
    bool bRev = false;
    if(OTS(pt1, pt2, pt3)<0)
    {
        MPoint pt = MPoint(pt1);
        pt1 = pt3;
        pt3 = &pt;
        bRev = true;
    }

    MPtArray* pta = new MPtArray();
    pta->AddHead(pt1);

    double radius;
    MPoint centr;
    MPoint ptTek;
    //def center and radius
    CIRCLEPT(pt1, pt2, pt3, &centr, &radius);
    double grad1 = GetVektorGrad(&centr,pt1);
    double grad2 = GetVektorGrad(&centr,pt2);
    double grad3 = GetVektorGrad(&centr,pt3);
    double razv = fabs(grad3 - grad1);
    if((grad3<grad1)&&((grad2>grad3)||(grad2<grad1))) razv = 360.0 - razv;
    double shag = razv/index;

    int i;
    for (i=1; i<index; i++)
    {
        grad1+=shag;
        if(grad1 > 360.0) grad1 -= 360.0;
        ptTek.x = centr.x + radius*cos(grad1*pi/180.0);
        ptTek.y = centr.y + radius*sin(grad1*pi/180.0);
        if(bRev) pta->AddHead(&ptTek);
        else pta->AddTail(&ptTek);
    }

    if(bRev) pta->AddHead(pt3);
    else pta->AddTail(pt3);

    return pta;
}


//==================================
//Building spline function
bool MKartaInterface:: CreateSpline(MPtArray* pta, unsigned char vid, int number)
//==================================
{
    if(pta->GetSize() <= 2) return false;

    MPtArray* ptNew;

    if(vid == SPV_ZAMKNUT) pta->AddTail(pta->GetAt(0));

    MDuArray* ar = new MDuArray();//array of height koeff
    double d = 0.0;
    for(int i=0; i<pta->GetSize(); i++)
        ar->AddTail(&d);

    ptNew = LOMTOASPL(pta, vid, ar->GetHead(), number);

    delete ar;

    pta->ClearAll();
    pta->AppendTail(ptNew);

    delete ptNew;

    return true;
}

//==================================
//index - edge point index
//==================================
bool MKartaInterface::CreateAttackDirection(MPtArray* pta, int index, int razbienie)
//==================================
{
    MPtArray* ptNew = new MPtArray();

    //we break the array on an index
    MPtArray* pta2 = pta->Cut(index);
    pta->AddTail(pta2->GetHead());

    //we form curves
    MPtArray* spta1 = (MPtArray*)pta->Clone();
    MPtArray* spta2 = (MPtArray*)pta2->Clone();

    CreateSpline(spta1, SPV_NEZAMKNUT, razbienie);
    CreateSpline(spta2, SPV_NEZAMKNUT, razbienie);

    //ends of an arrow
    MPoint point1,point2;
    double A,B,C;//coefficients of the straight line passing through 'point1' and 'point2' points
    double relation;//part of an edge
    double d = 0.0;

    relation = (pta->GetTail()->x - pta->GetHead()->x)/5;
    point1.x = pta->GetTail()->x - relation;
    GetPramuju(pta->GetHead(), pta->GetTail(), d, A, B, C);
    point1.y = (-A * point1.x - C)/B;
    relation = (pta2->GetHead()->x - pta2->GetTail()->x)/5;
    point2.x = pta2->GetHead()->x - relation;
    GetPramuju(pta2->GetHead(), pta2->GetTail(), d, A, B, C);
    point2.y = (-A * point2.x - C)/B;

    //we will calculate points of intersection of curves with an edge and we will remove "unnecessary" points
    MPoint ptCross1, ptCross2;
    int nomrebra = -1;
    FindDistFromPointToLomline(point1, spta1, ptCross1, &nomrebra);
    if(nomrebra > -1)
    {
        for(int i=spta1->GetSize()-1; i>nomrebra; i--)
            spta1->DeleteTail();
        spta1->AddTail(&ptCross1);
    }
    nomrebra = -1;
    FindDistFromPointToLomline(point2, spta2, ptCross2, &nomrebra);
    if(nomrebra > -1)
    {
        for(int i=0; i<nomrebra+1; i++)
            spta2->DeleteHead();
        spta2->AddHead(&ptCross2);
    }

    MPoint cnt = MPoint((ptCross1.x + ptCross2.x)/2, (ptCross1.y + ptCross2.y)/2);//center
    ptCross1.Scale(2.0, cnt.x, cnt.y);
    ptCross2.Scale(2.0, cnt.x, cnt.y);
    //formation of an exit
    ptNew->AppendHead(spta1);//1 arch
    ptNew->AddTail(&ptCross1);//end of an arrow
    ptNew->AddTail(pta->GetTail());//edge
    ptNew->AddTail(&ptCross2);//end of an arrow
    ptNew->AppendTail(spta2);//2d arch

    delete spta1;
    delete spta2;
    delete pta2;

    pta->ClearAll();
    pta->AppendTail(ptNew);

    delete ptNew;

    return true;
}

//==================================
//function of formation of an arrow task ONLY FROM 4-TH POINTS
//the received array is copied in the initial
bool MKartaInterface::CreateTask(MPtArray* pta)
//==================================
{
    if(pta->GetSize() != 4) return false;

    MPtArray* ptRet = new MPtArray();

    double orient = Area(pta);//>0 clockwise

    //all points
    MPoint pt0 = pta->GetAt(0);
    MPoint pt1 = pta->GetAt(1);
    MPoint pt2 = pta->GetAt(2);
    MPoint pt3 = pta->GetAt(3);

    //we will receive a point of an attachment of an arrow - we consider the middle
    MPoint point = MPoint((pt1.x + pt2.x)/2, (pt1.y + pt2.y)/2);
    MPoint ptTek = MPoint((pt0.x + pt3.x)/2, (pt0.y +pt3.y)/2);

    double dist = DLINASG(point, ptTek);
    int znak = ZNAK(ptTek.x - point.x);//for drawing on the right/at the left
    if(PointInRegion(GetPerpendikular(pt1, pt2, point, dist*znak/2), pta))
        znak = -znak;

    //initial angle and the direction for drawing of the first arch of an ellipse
    double angle0;
    bool bChas = true;
    if(orient<0)
    {
        if((pt0.x-pt1.x)>0)
            if((pt0.y-pt1.y)>0)  angle0 = 0.0;
            else  angle0 = 270.0;
        else if((pt0.y-pt1.y)>0) angle0 = 90.0;
        else angle0 = 180.0;
    }
    else
    {
        bChas = false;
        if((pt0.x-pt1.x)>0)
            if((pt0.y-pt1.y)>0) angle0 = 90.0;
            else  angle0 = 0.0;
        else if((pt0.y-pt1.y)>0) angle0 = 180.0;
        else angle0 = 270.0;
    }
    //array of points of an arch
    MPtArray* ptEllipse = new MPtArray();
    GetEllipseQuarter(ptEllipse, pt0, pt1, angle0, bChas);
    ptRet->AppendHead(ptEllipse);
    //perpendicular
    ptRet->AddTail(&point);
    MPoint ptPerp = GetPerpendikular(pt1, pt2, point, 2*dist*znak);
    ptRet->AddTail(&ptPerp);
    //arrow
    double sdvig = GetSdvigByPoint(pta, point);
    MPoint pt_vspom = GetPointBySdvig(pta, sdvig+dist/2);
    MPoint ptStr = GetPerpendikular(pt1, pt2, pt_vspom, 3*dist*znak/2);
    ptRet->AddTail(&ptStr);
    ptRet->AddTail(&ptPerp);
    pt_vspom = GetPointBySdvig(pta, sdvig-dist/2);
    ptStr = GetPerpendikular(pt1, pt2, pt_vspom, 3*dist*znak/2);
    ptRet->AddTail(&ptStr);
    //return perpendicular
    ptRet->AddTail(&ptPerp);
    ptRet->AddTail(&point);

    //initial angle and the direction for drawing of the last arch
    bChas = true;
    if(orient<0)
    {
        if((pt3.x-pt2.x)>0)
            if((pt3.y-pt2.y)>0) angle0 = 180.0;
            else angle0 = 90.0;
        else if((pt3.y-pt2.y)>0) angle0 = 270.0;
        else angle0 = 0.0;
    }
    else
    {
        bChas = false;
        if((pt3.x-pt2.x)>0)
            if((pt3.y-pt2.y)>0) angle0 = 270.0;
            else angle0 = 180.0;
        else if((pt3.y-pt2.y)>0) angle0 = 0.0;
        else angle0 = 90.0;
    }
    GetEllipseQuarter(ptEllipse, pt2, pt3, angle0, bChas);
    ptRet->AppendTail(ptEllipse);

    //redefinition of the existing array
    pta->ClearAll();
    pta->AppendTail(ptRet);

    //removal of auxiliary arrays
    delete ptEllipse;
    delete ptRet;

    return true;
}

//==================================
//function of formation of a quarter of an arch of an ellipse of a point 1 to a point 2
void MKartaInterface::GetEllipseQuarter(MPtArray* ptRet, MPoint pt1, MPoint pt2, double angle0, bool bChas, int number)
//==================================
{
    ptRet->ClearAll();
    ptRet->AddHead(&pt1);
    MPoint point;// current point
    //splitting
    double delta = 90.0/number;
    //if clockwise, we take away
    if(bChas) delta = -delta;

    MPoint centr;//relative center of an ellipse
    //we will determine coordinates of the center
    if((angle0 == 0.0)||(angle0 == 180.0))
    {
        centr.x = pt2.x;
        centr.y = pt1.y;
    }
    if((angle0 == 90.0)||(angle0 == 270.0))
    {
        centr.x = pt1.x;
        centr.y = pt2.y;
    }
    double a = fabs(pt1.x - pt2.x);
    double b = fabs(pt1.y - pt2.y);
    for(int i=1; i<number; i++)
    {
        double radangle = pi*(angle0+delta*i)/180.0;
        point.x = centr.x + a*cos(radangle);
        point.y = centr.y + b*sin(radangle);

        ptRet->AddTail(&point);
    }

    ptRet->AddTail(&pt2);
}

#define ID_SUBVERSIVE_DUGA_RAZB 7//number of the points forming an arch
#define ID_SUBVERSIVE_RAZVERTKA	30//solution of a angle of an arrow in degrees
//==================================
//function of formation of the area of diversionary and prospecting group
void MKartaInterface::CreateSubversiveGroup(MPtArray* pta)
//==================================
{
    if(!pta) return;

    int iSize = pta->GetSize();
    if(iSize < 3)
        return;

    MPtArray* ptRet = new MPtArray();
    ptRet->ClearAll();

    if(pta->GetHead() != pta->GetTail()) pta->AddTail(pta->GetAt(0));

    MPoint point1, point2, point3, pt1, pt2, pt, ptSered, ptPerp;

    //we will find a segment with the minimum length
    iSize = pta->GetSize();
    double min = -1;
    int i;
    for(i=0; i<iSize-1; i++)
    {
        point1 = pta->GetAt(i);
        point2 = pta->GetAt(i+1);
        double dlina = DLINASG(point1,point2);
        if((min<0)||(dlina<min))
            min = dlina;
    }
    min /= 5;

    double rad = pi*ID_SUBVERSIVE_RAZVERTKA/360;
    double tangence = tan(rad);
    double d;
    double sdvig = 0;
    for(i=0; i<iSize-1; i++)
    {
        pt1 = pta->GetAt(i);
        pt2 = pta->GetAt(i+1);
        //we define points for formation of an arch
        point1 = GetPointBySdvig(pta,min+sdvig);
        sdvig += DLINASG(pt1,pt2);
        point2 = GetPointBySdvig(pta, sdvig-min);
        ptSered = MPoint((point1.x+point2.x)/2, (point1.y+point2.y)/2);
        ptPerp = GetPerpendikular(point1,point2,ptSered,min);
        if(!ptPerp.InnerPtFast((MPoint*)pta->GetHead(),iSize))
            ptPerp = GetPerpendikular(point1,point2,ptSered,-min);
        //we add an arch to the array
        ptRet->AppendTail(GetDuga(&point1,&ptPerp,&point2,ID_SUBVERSIVE_DUGA_RAZB));
        //formation of arrows
        if(i==iSize-2) point3 = GetPointBySdvig(pta, min);
        else point3 = GetPointBySdvig(pta, sdvig+min);
        ptSered = MPoint((point2.x+point3.x)/2,(point2.y+point3.y)/2);
        ptRet->AddTail(&ptSered);
        pt = MPoint((point2.x+ptSered.x)/2,(point2.y+ptSered.y)/2);
        d = tangence*DLINASG(ptSered,pt);
        ptPerp = GetPerpendikular(ptSered,point2, pt,d);
        ptRet->AddTail(&ptPerp);
        ptRet->AddTail(&ptSered);
        MPoint tmpPt = GetPerpendikular(ptSered,point2, pt,-d);
        ptRet->AddTail(&tmpPt);
        ptRet->AddTail(&ptSered);

        pt = MPoint((point3.x+ptSered.x)/2,(point3.y+ptSered.y)/2);
        tmpPt = GetPerpendikular(ptSered,point3, pt,d);
        ptRet->AddTail(&tmpPt);
        ptRet->AddTail(&ptSered);
        tmpPt = GetPerpendikular(ptSered,point3, pt,-d);
        ptRet->AddTail(&tmpPt);
        ptRet->AddTail(&ptSered);
    }
    //filling the array
    pta->ClearAll();
    pta->AppendTail(ptRet);
    delete ptRet;
}

//==================================
//Placement of a substrate
//==================================
bool MKartaInterface:: SetRaster(MString filePath, MRect rect)
{
    MRaster* raster = new MRaster(rect);
    if(!raster->ConnectFile(&filePath)) return false;

    MLayer* pLay = new MLayer(m_karta->m_geobounds);
    pLay->m_name = "RASTER";
    pLay->AddObject(raster);
    m_karta->AddLayer(pLay);

    return true;
}

//==================================
//sets the status of working operational object to objects in this point with 'delta' accuracy
void MKartaInterface::SetExcitedStatus(MPoint* point, int delta)
//==================================
{	
    UnExciteAll();

    vector<double> vect;
    FindObjectsAtPoint(*point,delta,&vect);
    vector<double>::iterator it = vect.begin();
    while(it != vect.end())
    {
        MTopobject* tpo = FindObject(*it);
        if(tpo) tpo->m_excited = true;
        it++;
    }
}

//==================================
//sets the statuses of all operational objects of the map in 'false'
void MKartaInterface::UnExciteAll()
//================================== 
{
    OBITER itObj = m_actLayer->m_oblist.begin();
    while(itObj != m_actLayer->m_oblist.end())
    {
        if((*itObj)->WhatTheObject() == ID_TOPOBJECT)
            ((MTopobject*)(*itObj))->m_excited = false;
        itObj++;
    }
}

//==================================
bool MKartaInterface::SetObjectInActLayer(double uidLay, double uidObj, MPtArray* pta, MString kkod, MString formStr, int& result)
//==================================
{
    result = ID_NOTHING;

    //check on existence of this layer
    MLayer* pLay = FindActLayer(uidLay);
    if(!pLay)
    {
        result = ID_NO_LAYER;
        return false;
    }

    //we will check, whether there is already such object
    if(pLay->FindObject(uidObj))
    {
        result = ID_EXIST_OBJECT;
        return false;
    }

    //check of points
    if((!pta)||(pta->GetSize() == 0))
    {
        result = ID_NO_POINT;
        return false;
    }

    int iSize = pta->GetSize();
    for(int i=0; i<iSize; i++)
    {
        if(!CheckPoint(pta->GetAt(i)))
        {
            result = ID_NO_POINT;
            return false;
        }
    }

    //check of a quad-code
    if(!CheckKKod(kkod))
    {
        result = ID_NO_KKOD;
        return false;
    }

    //check on compliance of quantity of points to object type
    MString truekkod = kkod;
    MZnak* znak = m_kls->GetZnak(kkod, truekkod, true);
    int type = znak->GetYaz()->GetType();
    if(((type == F_DOT)&&(iSize >1))
            ||((iSize == 1)&&(type != F_DOT)))
    {
        result = ID_NO_YAZ;
        return false;
    }

    //if everything is correct, we can add object and to appoint to it data
    MTopobject* pobj = new MTopobject();
    pobj->m_UID = uidObj;
    pobj->SetTransType(7);
    pLay->AddObject(pobj);

    pobj->SetPoints(*pta);

    pobj->m_pZnak = znak;
    pobj->m_kkod  = truekkod;

    //verification of attribute
    if(!pobj->IsPole(formStr)) result = ID_NO_POLE;

    return true;
}

//==================================
//Converts a point from system of coordinates of CK42 in WGS84
void MKartaInterface::ConvertCK42_WGS84(MPoint ck42, MPoint& wgs84 )
//==================================
{
    double h2=0.0f;
    MPoint pt1 = ConvertPoint(ck42,false);
    //MEllipsTransform tParam;
    MCoo *crntCoo = new MGeodesicCoo();
    crntCoo->SetStandartEllipsoid(ELLIPSOID_KRASOVSKI_42);
    MCoo *newCoo = new MGeodesicCoo();
    newCoo->SetStandartEllipsoid(ELLIPSOID_WGS_84);
    crntCoo->ConvertToCoo(newCoo,pt1,h2);

    delete crntCoo;
    delete newCoo;
    wgs84 = ConvertPoint(pt1);

}

//==================================
//Converts a point from system of coordinates of WGS84 in CK42
void MKartaInterface::ConvertWGS84_CK42(MPoint wgs84, MPoint& ck42 )
//==================================
{
    double h2=0.0f;
    MPoint pt1 = ConvertPoint(wgs84,false);
    MCoo *crntCoo = new MGeodesicCoo();
    crntCoo->SetStandartEllipsoid(ELLIPSOID_WGS_84 );
    MCoo *newCoo = new MGeodesicCoo();
    newCoo->SetStandartEllipsoid(ELLIPSOID_KRASOVSKI_42);
    crntCoo->ConvertToCoo(newCoo,pt1,h2);

    delete crntCoo;
    delete newCoo;
    ck42 = ConvertPoint(pt1);

}


//==================================
//Converts all objects of the map for a new central meridian
bool MKartaInterface::ConvertGausMap(double cntrMeridian)
//==================================
{
    if(m_karta->m_pObjectCoo->WhatTheCoo()!= COO_GEODESIC)
    {

        /*MRect mapRect = m_karta->GetGeoBounds();
        MPoint tmpPtL(mapRect.left, mapRect.bottom);
        MPoint geoPointL = ConvertPoint(tmpPtL, false);
        MPoint tmpPtR(mapRect.right, mapRect.bottom);
        MPoint geoPointR = ConvertPoint(tmpPtR, false);
        int leftCM = (int)geoPointL.x - ((int)geoPointL.x)%6 + 3;
        int rightCM = (int)geoPointR.x - ((int)geoPointR.x)%6 + 3;
        if (cntrMeridian < leftCM)
            cntrMeridian = leftCM;
        else if (cntrMeridian > rightCM)
            cntrMeridian = rightCM;
        else cntrMeridian = cntrMeridian - cntrMeridian%6 + 3;*/

        MCoo* coo = CreateCooByType(m_karta->m_pObjectCoo->WhatTheCoo());
        MExtendedGaussKrugerCoo * extGK  = (MExtendedGaussKrugerCoo *) coo;
        extGK->SetBasicParameters(cntrMeridian);
        centralMeridian = cntrMeridian;
        MPoint pt;
        double h=0;
        pt.x = (m_karta->m_geobounds.left+m_karta->m_geobounds.right)/2;
        pt.y = (m_karta->m_geobounds.bottom+m_karta->m_geobounds.top)/2;
        if(m_karta->m_pObjectCoo->ConvertToCoo(extGK, pt, h))
        {
            //????????? ?? ???? ?????
            MLayer* pLay;
            LAYITER it = m_karta->m_laylist.begin();
            while(it!=m_karta->m_laylist.end())
            {
                pLay = *it;
                pLay->ConvertToCoo(m_karta->m_pObjectCoo, extGK);
                it++;
            }

            MCoo* pNewCoo = CreateCooByType(extGK->WhatTheCoo());
            if(pNewCoo)
            {
                pNewCoo->Copy(extGK);
                delete m_karta->m_pObjectCoo;
                m_karta->m_pObjectCoo = pNewCoo;
            }
            //???????????? ???????????? ?????????????
            m_karta->MinimizeGeobounds();
            //delete coo;

        }
        else return false;
        return true;
    }
    return true;
}

//==================================
//Fills the arr array of geo-points of a straight line of the set 'pnt1' and 'pnt2' with 'step' step in meters
int MKartaInterface::GetLine(const MPoint &pnt1, const MPoint &pnt2, MPtArray *&arr, int step)
//==================================
{ 

    if (arr!=nullptr) arr->ClearAll();
    else arr = new MPtArray();
    MPoint pt;
    MPoint pntg1 = ConvertPoint(pnt1, false);
    MPoint pntg2 = ConvertPoint(pnt2, false);
    const MGeodesic geod(axis_X, axis_X/(axis_X-axis_Y));
    double s12,azi1,azi2;
    geod.Inverse(pntg1.y, pntg1.x, pntg2.y, pntg2.x, s12, azi1, azi2);

    if (s12<step)
    {
        pt = ConvertPoint(pntg1);
        arr->AddTail(&pt);
        pt = ConvertPoint(pntg2);
        arr->AddTail(&pt);
        return 2;
    }

    MGeodesicLine l;
    l = geod.Line(pntg1.y, pntg1.x, azi1);
    pt = ConvertPoint(pntg1);
    arr->AddTail(&pt);
    MPoint iPt;
    double curPos = step;
    while(curPos<s12)
    {
        double mz;
        l.Position(curPos, iPt.y, iPt.x, mz);
        pt = ConvertPoint(iPt);
        arr->AddTail(&pt);
        curPos+=step;
    }
    pt = ConvertPoint(pntg2);
    arr->AddTail(&pt);

    return arr->Size;
}


//==================================
//Fills the 'arr' array of geo-points of sector.
//'ptCenter' = the center, 'pt1' = a point forming radius, 'angle' = a development angle
void MKartaInterface::GetSector(MPoint* ptCenter, MPoint* pt1, double angle, MPtArray* arr, int step)
//==================================
{
    if (arr) arr->ClearAll();
    else arr = new MPtArray();

    while(angle>360) angle -= 360;

    MPtArray* ptaTemp = nullptr;
    //    MPtArray* ptaTemp = new MPtArray();

    GetLine(*ptCenter, *pt1, ptaTemp, step);
    arr->AppendHead(ptaTemp);

    double radius = GetDistance(*ptCenter,*pt1);
    double azimut = GetAzimut(*ptCenter, *pt1);
    double azAngle = azimut+angle;
    while((azAngle)>360) azAngle -= 360;
    MPoint pt2 = GetPointByAzimuthLength(*ptCenter, azAngle, radius);

    double dlina = 2*pi*radius*angle/360;
    int count = (int)dlina/step;
    double azStep = angle/count;
    for(int i=1; i<count; i++)
    {
        azAngle = azimut+i*azStep;
        if(azAngle>360) azAngle -= 360;
        MPoint pt3 = GetPointByAzimuthLength(*ptCenter, azAngle, radius);
        arr->AddTail(&pt3);
    }


    ptaTemp->ClearAll();
    GetLine(pt2, *ptCenter, ptaTemp, step);
    arr->AppendTail(ptaTemp);

    delete ptaTemp;

}

void MKartaInterface::RestorePixmap(QPixmap* pxmRestore)
{
    if(!pxmRestore) return;
    if(m_widget)
    {
        if(m_Painter->isActive())
            m_Painter->end();
        m_Painter->begin(m_widget);
        m_Painter->drawPixmap(0,0,*pxmRestore);
        m_Painter->end();
    }
}

//  Function of getting of nautical miles (NM) from metres
double MKartaInterface::GetNauticalMiles(double dMetres)
{
    double dNauticalMiles;
    //dNauticalMiles = 0.00054*dMetres;
    dNauticalMiles = dMetres/1852.0;
    return dNauticalMiles;
}

//  Function of getting of metres from nautical miles (NM)
double MKartaInterface::GetMetresFromNauticalMiles(double dNauticalMiles)
{
    double dMetres;
    dMetres = 1852.0*dNauticalMiles;
    return dMetres;
}

//  Function of getting of foots (Ft) from metres
double MKartaInterface::GetFoots(double dMetres)
{
    double dFoots;
    //dFoots = 3.281*dMetres;
    dFoots = dMetres/0.3048;
    return dFoots;
}

//  Function of getting of metres from foots (Ft)
double MKartaInterface::GetMetresFromFoots(double dFoots)
{
    double dMetres;
    dMetres = 0.3048*dFoots;
    return dMetres;
}

//  Function of getting of velocity in knots from velocity in metres/seconds
double MKartaInterface::GetKnots(double dVelocity)
{
    double dKnots;
    dKnots = dVelocity*3600.0/1852.0;
    return dKnots;
}

//  Function of getting of velocity in metres/seconds from velocity in knots
double MKartaInterface::GetVelocityFromKnots(double dKnots)
{
    double dVelocity;
    dVelocity = dKnots*1852.0/3600.0;
    return dVelocity;
}

//  Function of getting of acceleration in knots/sec
//                   from acceleration in metres/sec^2
double MKartaInterface::GetAccelerationKnots(double dAcceleration)
{
    double dKnots;
    dKnots = dAcceleration*3600.0/1852.0;
    return dKnots;
}

//  Function of getting of acceleration in metres/sec^2
//                   from acceleration in knots/sec
double MKartaInterface::GetAccelerationFromKnots(double dKnots)
{
    double dAcceleration;
    dAcceleration = dKnots*1852.0/3600.0;
    return dAcceleration;
}

//  Function of getting of square in ft^2 from  m^2
double MKartaInterface::GetSquareFoots(double dSqMetres)
{
    double dSqFoots;
    dSqFoots = dSqMetres/(0.09290304);
    return dSqFoots;
}

//  Function of getting of square in m^2 from ft^2 
double MKartaInterface::GetSqMetresFromSqFoots(double dSqFoots)
{
    double dSqMetres;
    dSqMetres = dSqFoots*(0.09290304);
    return dSqMetres;
}




void MKartaInterface::CalculateAngleClose(MPoint pointStand, double heightPhaseCenter,
                                          double DMax,
                                          std::map<int, std::map<int, double> >& mapAzimuthDistanceAngle,
                                          double discretAzimuth, double discretRange)
{
    mapAzimuthDistanceAngle.clear();

    if(discretRange == 0)
    {
        discretRange = DMax/1000;
    }


    int countAzimuth= (int)(360/discretAzimuth);
    int countRange = (int)(DMax/discretRange);
    M2DTopocentricCoo *coo{nullptr};
    for(int i = 0; i < countAzimuth; i++)
    {
        double azimuth = i*discretAzimuth;

        if(countRange){
            coo = new  M2DTopocentricCoo(pointStand, heightPhaseCenter,
                                         (double)azimuth);
        }


        std::map<int, double> mapDistanceAngel;

        double previousAngleClose = 0.0;
        for(int j = 1; j <= countRange; j++)
        {
            int distance = j*discretRange;
            MPoint analisysPoint = this->GetPointByAzimuthLength(pointStand,
                                                                 (double) azimuth,
                                                                 (double) distance);
            double heightAnalisysPoint = this->GetHight(analisysPoint);
            MPoint analisysPointTopocentric;
            coo->FromBLH(analisysPoint, heightAnalisysPoint, analisysPointTopocentric);
            double angleClose = atan(analisysPointTopocentric.y/analisysPointTopocentric.x);
            if(j == 1)
                previousAngleClose = angleClose;
            else
            {
                if(angleClose < previousAngleClose)
                    angleClose = previousAngleClose;
                //			double hhhh = pBMH->GetHeight(pointStand);
                else
                    previousAngleClose = angleClose;
            }

            mapDistanceAngel.insert(std::make_pair(distance, angleClose));
        }
        mapAzimuthDistanceAngle.insert(std::make_pair(azimuth, mapDistanceAngel));
        if(coo){
            delete coo;
        }
    }
}

bool MKartaInterface::GetMapWithRangeAndAngleByAzimuth(
        int azimuth, std::map<int, std::map<int, double> > mapAngleClose,
        std::map<int, double>& mapRangeAngle)
{
    if(mapAngleClose.size() == 0)
        return false;

    std::map<int, std::map<int, double> >::iterator it1 = mapAngleClose.find(azimuth);

    if(it1 == mapAngleClose.end())
        return false;

    mapRangeAngle = it1->second;
    return true;
}

bool MKartaInterface::GetAngleByDistance(
        int distance, std::map<int, double > mapDistanceAngle,
        double& angleClose)
{
    if(mapDistanceAngle.size() == 0)
        return false;

    std::map<int, double>::iterator it1 = mapDistanceAngle.find(distance);

    if(it1 == mapDistanceAngle.end())
        return false;

    angleClose = it1->second;
    return true;
}

bool MKartaInterface::GetAngleByAzimuthAndDistance(int azimuth, int distance,
                                                   std::map<int, std::map<int, double> > mapAngleClose,
                                                   double& angleClose)
{
    double angle = 0.0;

    std::map<int, double> mapRangeAngle;
    if(!GetMapWithRangeAndAngleByAzimuth(azimuth, mapAngleClose, mapRangeAngle))
        return false;

    if(!GetAngleByDistance(distance, mapRangeAngle, angle))
        return false;

    angleClose = angle;
    return true;
}

/*
//! We establish a symbol angle of rotation for dot object
bool MKartaInterface::SetAngle(int angle)
//==================================
{
    if(!CheckWorkingObject()) return false;

    if(!m_Object->m_pZnak) return false;

    MYaz* pYaz = m_Object->m_pZnak->GetYaz();

    if(pYaz->GetType() != F_DOT) return false;

    MSzArray* szar = ((MDotYaz*)(pYaz->m_object))->m_tochka;
    MSrez* srez=szar->GetAt(0);
    if(srez->GetType() != IDT_SHRIFT) return false;

    MSymbol* sym = &((MShriftSrez*)srez)->m_tochka;
    sym->logfont.lfEscapement = angle;

    return true;
}


//! We establish a symbol angle of rotation for dot object
bool MKartaInterface::SetAngle(double uid, int angle){

    MTopobject* obj = FindObject(uid);
    if(!obj) return false;
    if(!obj->m_pZnak) return false;

    MYaz* pYaz=obj->m_pZnak->GetYaz();

    if(pYaz->GetType() != F_DOT) return false;

    MSzArray* szar = ((MDotYaz*)(pYaz->m_object))->m_tochka;
    MSrez* srez=szar->GetAt(0);
    if(srez->GetType() != IDT_SHRIFT) return false;

    MSymbol* sym = &((MShriftSrez*)srez)->m_tochka;
    sym->logfont.lfEscapement = angle;

    return true;
}
*/

//getting altitude 
extern "C" int Get_Altitude(  int Y_gauss, short iY_gauss ,
                              int X_gauss,  short  iX_gauss ,
                              char* path, short ipath,
                              short lenpath ,
                              short* piH )
{
    FILE *f;
    int H=0;
    if( (f=fopen("/tmp/Get_Altitude.txt","a" ) )!=NULL)
    {
        fprintf(f,"\n Get_Altitude X_gauss=%d, iX_gauss=%d , Y_gauss=%d,  iY_gauss=%d, path=%s,ipath=%d, lenpath=%d \n",
                X_gauss,  iX_gauss, Y_gauss, iY_gauss, path,  ipath, lenpath) ;
        fclose(f);
    }
    MPoint pt_gauss ( X_gauss , Y_gauss ) ;  // revert
    MKarta m ;
    if( (f=fopen("/tmp/Get_Altitude.txt","a" ) )!=NULL)
    {
        fprintf(f,"\n Get_Altitude pt_gauss.x=%f, , pt_gauss.y=%f ,  path=%s  \n",pt_gauss.x,  pt_gauss.y, path ) ;
        fclose(f);
    }
    /*     H=m.GetHeightFromMatrix( pt_gauss ,   path )    ;*/
    MString path2="/opt/oracle/product/db/lib/matrix.bmh";
    H=m.GetHeightFromMatrix( pt_gauss ,  path2 )    ;


    if ( H !=-20000 )
        *piH=0 ;

    if( (f=fopen("/tmp/Get_Altitude.txt","a" ) )!=NULL)
    {
        fprintf(f,"\n   H=%d,  indicator=%d \n",H, *piH) ;
        fclose(f);
    }

    return H;
}


QStringList MKartaInterface::GetSelectedObjectInfo(MTopobject * mTpo)
{
    MTopobject * m_pSelectedTpo = mTpo;
    QStringList list;//TR:the list which is output in a new window
    if (m_pSelectedTpo == NULL) return list;

    //Output List to a new window
    //QStringList list;//TR:the list which is output in a new window
    QTextCodec *codec = QTextCodec::codecForName(mapCharset);
    QString     mes;
    //QStringList coo;
    float len = 0.0;
    //float sqt = 0.0;
    QString   sss;
    //MString  kkod= m_pSelectedTpo->m_kkod;
    QByteArray  ccc;
    ccc= m_pSelectedTpo->m_pZnak->GetTermin()->GetShortname().c_str();
    sss = codec->toUnicode(ccc);
    list.push_back(QObject::tr("Object: ") + sss);
    ccc = m_pSelectedTpo->m_kkod.c_str();
    sss= codec->toUnicode(ccc);
    list.push_back(QObject::tr("Code:   ") + sss);
    MYaz* pYaz= m_pSelectedTpo->m_pZnak->   GetYaz();
    int N =  m_pSelectedTpo->m_Komplekt->GetSize();
    MPtArray* pta =NULL;
    int j;

    for(j= m_pSelectedTpo->F_TOCHKI; j<N; j++)
    {
        pta=(MPtArray*) m_pSelectedTpo->m_Komplekt->GetAt(j);
        if ( pta->GetSize() )
            switch(pYaz->GetType())
            {
            case F_PLO:
                if (j == m_pSelectedTpo->F_TOCHKI) mes = QObject::tr("Area");
                for(int i=0;i<pta->Size-1;++i)
                    len += GetDistance(*pta->GetAt(i),*pta->GetAt(i+1));

                //len += DLINALOMLINE(pta);
                break;
            case F_LIN:
                if (j == m_pSelectedTpo->F_TOCHKI) mes = QObject::tr("Line");
                for(int i=0;i<pta->Size-1;++i)
                    len += GetDistance(*pta->GetAt(i),*pta->GetAt(i+1));
                //len += DLINALOMLINE(pta);
                break;

            case F_DOT:
                if (j == m_pSelectedTpo->F_TOCHKI) mes = QObject::tr("Dot");
                break;
            }
    }//EoFOR

    list.push_back(QObject::tr("Type:   ") + mes);

    //Calculated Length doen't seem to be right
    if (len)
    {
        MString mss;
        mss.Format("  %.3f ", len/1000.0);
        list.push_back(QObject::tr("Length:") + mss.c_str() +QObject::tr("km"));
    }

    //Creating Section with Attributes description
    list.push_back(QObject::tr("Attributes:"));
    MNvArray *m_pAttrArray = (MNvArray *)m_pSelectedTpo->m_Komplekt->GetAt(F_NADPIS);
    MString str, name;
    unsigned short nm;
    MNomValue *temp;
    //Going through object attributes
    for(int i = 0; i<m_pAttrArray->GetSize();i++)
    {
        temp =   m_pAttrArray->GetAt(i);
        nm = temp->GetNom();
        GetKlassifikator()->GetDictAttrName(nm,name);
        str="";
        str = temp->GetValue();
        ccc= name.c_str();
        sss= codec->toUnicode(ccc);
        ccc= str.c_str();
        sss += " : " + codec->toUnicode(ccc);
        GetKlassifikator()->GetDictAttrEdIzm(nm,name);
        sss += " "+codec->toUnicode(name.c_str());
        list.push_back( sss );
    }

    MLayer *layer = m_pSelectedTpo->GetLayer();
    for(int i=0;i<m_pSelectedTpo->m_dbarray.Size;++i)
    {
        MString *s = m_pSelectedTpo->m_dbarray.GetAt(i);
        MString *name = NULL;
        if(!s->IsEmpty())
        {
            name = layer->m_dbarray.GetAt(i);

            sss = " "+codec->toUnicode(name->c_str());
            sss += " : " + codec->toUnicode(s->c_str());
            list.push_back(sss);
        }
    }


    //Creating Section with Object Coordinates
    list.push_back(QObject::tr("Coordinates:"));
    int k=0;
    bool co_isGaus = (GetKarta()->m_pObjectCoo->WhatTheCoo() != COO_GEODESIC );
    for(j= m_pSelectedTpo->F_TOCHKI; j<N; j++)
    {
        pta=(MPtArray*) m_pSelectedTpo->m_Komplekt->GetAt(j);
        if (pta)
        {
            MPoint pt, geoPt;
            for(int i=0; i<pta->GetSize(); i++)
            {
                if (co_isGaus)
                {
                    geoPt = ConvertPoint(*pta->GetAt(i),false);
                    pt= GeoToGaussStandart(geoPt);
                }
                else pt =*pta->GetAt(i);

                MString xy;
                if (pt.y + pt.x> 0.1)
                {
                    k++;
                    if (pt.y + pt.x> 1000.0)
                        xy.Format("%3d. %7.0f   %7.0f", k, pt.y, pt.x );
                    else
                        xy.Format("%3d. %3.7f   %3.7f", k, pt.y, pt.x );
                    //qDebug() << xy.c_str();
                    list.push_back( xy.c_str() );
                }
            }
        }
    }
    return list;
}

int MKartaInterface::GetScale()
{
    //scaleOnlyForCentralPoint = false;
    double wMM = GetWidgetSizeMM().cx;
    MRect zoom = GetZoombounds();
    MPoint p1(zoom.left, (zoom.bottom + zoom.top) / 2.0);
    MPoint p2((zoom.right + zoom.left) / 2.0, (zoom.bottom + zoom.top) / 2.0);
    MPoint p3(zoom.right, (zoom.bottom + zoom.top) / 2.0);
    p1 = ConvertPoint(p1, false);
    p2 = ConvertPoint(p2, false);
    p3 = ConvertPoint(p3, false);
    double ds = GetDistance(p1, p2) + GetDistance(p2, p3);
    double M = 10.0 * ds / wMM;
    int scale = (int)(M + 5) / 10;
    m_QtDevice->SetGeoScale(1000*scale);
    return scale;
}

bool MKartaInterface::DrawPixmapByUZN(const MString& _UZN, MKartaInterface* _pKI, QPixmap& _pmap)
{
    if(!_pKI) return false;
    if(!_pKI->m_karta) return false;
    if(!_pKI->m_karta->m_pKls) return false;

    MString truekkod = "";
    const MZnak* pZnak = _pKI->m_karta->m_pKls->GetZnak(_UZN, truekkod);
    if(!pZnak) return false;

    const  MYzArray* pYzArr = pZnak->GetYazArray();
    if(!pYzArr) return false;

    for(int i = 0; i<pYzArr->GetSize(); i++)
    {
        const MYaz* pYaz = pZnak->GetYaz(i);
        if(pYaz->GetType() != F_DOT) continue;

        const MDotYaz* dotyaz = (MDotYaz*)pYaz->m_object;
        if(!dotyaz) continue;
        if(!dotyaz->m_tochka) continue;

        for(int j = 0; j < dotyaz->m_tochka->GetSize(); j++)
        {
            MSrez* const srez = dotyaz->m_tochka->GetAt(j);
            if(srez && srez->GetType() == 0)
            {
                const MSymbol* pSymbol = &((MShriftSrez*)srez)->m_tochka;
                if(pSymbol)
                {
                    const MString s = pSymbol->str;
                    QFont font = QFont(pSymbol->logfont.lfFaceName,14);
                    font.setBold(true);
                    const unsigned char colorKod = pSymbol->colorkod;
                    MColor color(0, 0, 0, 255);
                    (void)_pKI->m_karta->m_pKls->GetDictColorValue(colorKod, color);
                    const QColor fontColor(color.R, color.G, color.B);
                    const QFontMetrics fontMt(font);
                    const int recWH = std::max(fontMt.width(s.c_str()), fontMt.height()) + 2;
                    const QRect mapRect(0,0,recWH,recWH);
                    QPixmap pmap(mapRect.size());
                    pmap.fill(QColor(255,255,255,0));
                    QPainter * const painter = new QPainter();
                    if(painter)
                    {
                        (void)painter->begin(&pmap);
                        painter->setFont(font);
                        painter->setPen(fontColor);
                        painter->setRenderHint(QPainter::TextAntialiasing);
                        painter->setBackgroundMode(Qt::TransparentMode);

                        const int iCharWidth = fontMt.width(s.c_str());
                        const int iCharHeight = fontMt.height();
                        const int iMapRectWidth = mapRect.width();
                        const int iMapRectHeight = mapRect.height();

                        const QPoint pos((iMapRectWidth-iCharWidth)/2,
                                         iMapRectHeight-(iMapRectHeight-iCharHeight)/2);
                        painter->drawText(pos, s.c_str());
                        (void)painter->end();

                        delete painter;
                    }
                    _pmap = pmap;
                    return true;
                }
            }
        }
    }
    return false;
}

QString MKartaInterface::coordinatesString(double lat, double lon, int coordinatesSystem)
{
    QString coorString{""};

    if(coordinatesSystem == 1){
        MPoint geoCorrdWGS84(lon, lat);
        MPoint geoCorrdSK42;
        ConvertWGS84_CK42(geoCorrdWGS84, geoCorrdSK42);
        auto gaussCoord = GeoToGaussStandart(geoCorrdSK42);
        coorString = QString("X %1 Y %2").arg(static_cast<int>(gaussCoord.y)).arg(static_cast<int>(gaussCoord.x));
    }
    else{
        coorString = QString("%1 %2").arg(latToString(lat)).arg(lonToString(lon));
    }

    return coorString;
}

MPoint MKartaInterface::geoWgs84ToGaussSk42(MPoint &pt)
{
    MPoint geoCorrdSK42;
    ConvertWGS84_CK42(pt, geoCorrdSK42);
    return GeoToGaussStandart(geoCorrdSK42);
}

MPoint MKartaInterface::gaussSk42ToGeoWgs84(MPoint &pt)
{
    MPoint geoCorrdSK42 = GaussStandartToGeo(pt);
    MPoint geoCorrdWgs84;
    ConvertCK42_WGS84(geoCorrdSK42, geoCorrdWgs84);
    return geoCorrdWgs84;
}

QString MKartaInterface::latToString(double v)
{
    double a = std::abs(v);
    double a_m = 60 * (a - std::floor(a)), a_s = 60 * (a_m - std::floor(a_m)),
           a_ss = 100 * (a_s - std::floor(a_s));
    return QString("%1 %2%3%4'%5.%6\"")
        .arg((v >= 0) ? 'N' : 'S') //
        .arg(static_cast<quint16>(std::floor(a)), 2, 10, QChar('0'))
        .arg(QChar(176))
        .arg(static_cast<quint16>(std::floor(a_m)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_s)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_ss)), 2, 10, QChar('0'));
}

QString MKartaInterface::lonToString(double v)
{
    double a = std::abs(v);
    double a_m = 60 * (a - std::floor(a)), a_s = 60 * (a_m - std::floor(a_m)),
           a_ss = 100 * (a_s - std::floor(a_s));
    return QString("%1 %2%3%4'%5.%6\"")
        .arg((v >= 0) ? 'E' : 'W') //
        .arg(static_cast<quint16>(std::floor(a)), 2, 10, QChar('0'))
        .arg(QChar(176))
        .arg(static_cast<quint16>(std::floor(a_m)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_s)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_ss)), 2, 10, QChar('0'));
}
