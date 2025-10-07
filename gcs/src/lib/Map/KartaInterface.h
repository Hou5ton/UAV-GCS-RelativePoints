//PACKAGE : MAP.
//FILE: KartaInterface.h    
//AUTHOR:
//DESCRIPTION:

#if !defined(KARTAINTERFACE_H_)
#define KARTAINTERFACE_H_
#include <vector>
#include "Karta.h"
#include "QtDevice.h"
extern  double         axis_X,  axis_Y;
using namespace std;

// Structure for active object searcing by layer identifier and object
struct FindObjectStruct
{
	double m_LayUID;
	double m_ObjUID;
};

#define LAGRANGE_NUMBER 15
#define RAZBIENIE 15
#define STEP_M 50000

//for checking act object
#define ID_NOTHING			-1 
#define ID_NO_LAYER			0 
#define ID_EXIST_OBJECT		1 
#define ID_NO_POINT			2 
#define ID_NO_KKOD			3 
#define ID_NO_YAZ			4 
#define ID_NO_POLE			5 

//PACKAGE : MAP.
//CLASS : MKartaInterface.
//DESCRIPTION : Class, which provides operations on map.

class MKartaInterface : public QObject
{
    Q_OBJECT
public:
	bool m_test;
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MKartaInterface().
//DESCRIPTION: Constructors.
//INPUTS : NONE.
//RETURNS : NONE.
    MKartaInterface(QObject *parent = nullptr);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MKartaInterface(QWidget* widget).
//DESCRIPTION: Constructors.
//INPUTS : QWidget pointer.
//RETURNS : NONE.
	MKartaInterface(QWidget* widget);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MKartaInterface(QWidget* widget, QWidget* widgetsmall).
//DESCRIPTION: Constructors.
//INPUTS : QWidget pointer, QWidget pointer.
//RETURNS : NONE.
	MKartaInterface(QWidget* widget, QWidget* widgetsmall);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::~MKartaInterface().
//DESCRIPTION: Destructors.
//INPUTS : NONE.
//RETURNS : NONE.	
	virtual ~MKartaInterface();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::InitKartaInterface().
//DESCRIPTION: The same part of MapInterface initialization (for all constructors).
//INPUTS : NONE.
//RETURNS : NONE.
	void InitKartaInterface();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::RestoreMap().
//DESCRIPTION: Recovers the map (pixmap) without active environment.
//INPUTS : NONE.
//RETURNS : NONE.
	void RestoreMap();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::RestoreMapAct().
//DESCRIPTION: Recovers the map (pixmap) with active environment.
//INPUTS : NONE.
//RETURNS : NONE.
	void RestoreMapAct();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::RestoreMapSmall().
//DESCRIPTION: Recovers small map (pixmap) without active environment.
//INPUTS : NONE.
//RETURNS : NONE.
 	void RestoreMapSmall();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::RestoreMapActSmall().
//DESCRIPTION: Recovers small map (pixmap) with active environment.
//INPUTS : NONE.
//RETURNS : NONE.
	void RestoreMapActSmall();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawMap(bool toScreen = true).
//DESCRIPTION: Draws map.
//INPUTS : bool.
//RETURNS : NONE.
	void DrawMap(bool toScreen = true);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawAct(bool toScreen = true).
//DESCRIPTION: Recovers the map and redraw active environment.
//INPUTS : bool.
//RETURNS : NONE.
	void DrawAct(bool toScreen = true);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawMapSmall(bool toScreen = true).
//DESCRIPTION: Draws small map.
//INPUTS : bool.
//RETURNS : NONE.
	void DrawMapSmall(bool toScreen = true);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawScaleGrid(MPlotDevice* pDevice, MRect &gridrect).
//DESCRIPTION: Drawing scale ruler.
//INPUTS : MPlotDevice pointer, MRect &.
//RETURNS : NONE.
	void DrawScaleGrid(MPlotDevice* pDevice, MRect &gridrect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawCooGrid(MPlotDevice* pDevice, MRect& gridrect).
//DESCRIPTION: Drawing grid.
//INPUTS : MPlotDevice pointer, MRect& .
//RETURNS : NONE.
	void DrawCooGrid(MPlotDevice* pDevice, MRect& gridrect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawMGRSCooGrid(MPlotDevice* pDevice, MRect& gridrect).
//DESCRIPTION: Drawing MGRS grid.
//INPUTS : MPlotDevice pointer, MRect& .
//RETURNS : NONE.
    void DrawMGRSCooGrid(MPlotDevice* pDevice, MRect& gridrect);
    void DrawMGRSStepGrid(MPlotDevice* pDevice, MRect& rect, double step);    
    QVector<MPoint> GetMGRSStepLine(MPoint begin,MPoint end,double step);

    //PACKAGE : MAP.
    //FUNCTION: DrawSearchPoint(const double &lat, const double &lon).
    //DESCRIPTION: Drawing Point.
    //INPUTS : const double &lat, const double &lon
    //RETURNS : NONE.
    bool DrawSearchPoint(const double &lat, const double &lon);

    /// DESCRIPTION : Called to remove Search point from the map
    void RemoveSearchPoint();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DrawFoundByNameObject(MPlotDevice* pDevice).
//DESCRIPTION: Drawing object recently found by name.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	void DrawFoundByNameObject(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetWidget().
//DESCRIPTION: Returns 'widget' on which map is drawing.
//INPUTS : NONE.
//RETURNS : QWidget pointer.
	QWidget* GetWidget() {return m_widget;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetWidgetSizeMM().
//DESCRIPTION: returns the size of a widget- drawn map on which ( in mm ).
//INPUTS : NONE.
//RETURNS : MIntSize.
	MIntSize GetWidgetSizeMM();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetWidgetSmall().
//DESCRIPTION: Returns widget on which small map is drawing.
//INPUTS : NONE.
//RETURNS : QWidget pointer.
	QWidget* GetWidgetSmall() {return m_widgetsmall;}  

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetWidget(QWidget* widget).
//DESCRIPTION: Sets widget on which map is drawing.
//INPUTS : QWidget pointer.
//RETURNS : NONE.
	void SetWidget(QWidget* widget);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetWidgetSmall(QWidget* widget).
//DESCRIPTION: Sets widget on which small map is drawing.
//INPUTS : QWidget pointer.
//RETURNS : NONE.
	void SetWidgetSmall(QWidget* widget);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LoadMap(MString map_name).
//DESCRIPTION: Loads map from UTP-format.
//INPUTS : MString.
//RETURNS : True.	
	bool LoadMap(MString map_name);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetKlassifikator(MString kls_name).
//DESCRIPTION: Loads classifier.
//INPUTS : MString.
//RETURNS : True.
	bool SetKlassifikator(MString kls_name);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetKlassifikator().
//DESCRIPTION: Returns pointer to attached classifier.
//INPUTS : NONE.
//RETURNS : MKlassifikator pointer.
	MKlassifikator* GetKlassifikator() {return m_kls;};

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetKlassifikatorPath().
//DESCRIPTION: Returns path to classifier file 
// 				attached to current loaded map.
//INPUTS : NONE.
//RETURNS : MString .	
	MString GetKlassifikatorPath() {return m_KlsPath;};

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetKlassifikatorPath(MString newKlsPath).
//DESCRIPTION: Sets path to classifier file 
// 				attached to current loaded map.
//INPUTS : MString.
//RETURNS : NONE.	
    void SetKlassifikatorPath(MString newKlsPath) {m_KlsPath = newKlsPath;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetKarta().
//DESCRIPTION: Returns pointer to the map.
//INPUTS : NONE.
//RETURNS : MKarta pointer.
    MKarta* GetKarta() const {return m_karta;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetZoombounds().
//DESCRIPTION: Returns current rectaangle of map drawing.
//INPUTS : NONE.
//RETURNS : MRect .
	MRect GetZoombounds() {return m_zoombounds;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetGeoBounds(bool bInMapCoo = false).
//DESCRIPTION: Returns bounding rectangle of the map.
//INPUTS : bool.
//RETURNS : MRect.
	MRect GetGeoBounds(bool bInMapCoo = false);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetBackgroundColor(bool HLS = true).
//DESCRIPTION: Returns map background color.
//INPUTS : bool.
//RETURNS : MColor.
	MColor GetBackgroundColor(bool HLS = true);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetBackgroundColor(MColor& col).
//DESCRIPTION: Sets map background color.
//INPUTS : MColor&.
//RETURNS : NONE.
	void SetBackgroundColor(MColor& col);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToGeo(const QPoint& lPoint, bool bInMapCoo = false).
//DESCRIPTION: Conversion function logic coordinates into geographic.
//INPUTS : const QPoint& , bool.
//RETURNS : MPoint.
    MPoint LogicToGeo(const QPoint& lPoint, bool bInMapCoo = false);
   
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToLogic(const MPoint& gPoint).
//DESCRIPTION: Conversion function geographic coordinates into logic.
//INPUTS : const MPoint& .
//RETURNS : QPoint.
    QPoint GeoToLogic(const MPoint& gPoint);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToGeo(const QRect& lrect, bool bInMapCoo = false).
//DESCRIPTION: Conversion function logic coordinates into geographic.
//INPUTS : const QRect& , bool.
//RETURNS : MRect.
    MRect LogicToGeo(const QRect& lrect, bool bInMapCoo = false);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToLogic(const MRect& lrect).
//DESCRIPTION: Conversion function geographic coordinates into logic.
//INPUTS : const MRect& .
//RETURNS : QRect.
    QRect GeoToLogic(const MRect& lrect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToSCR(const MRect& lrect).
//DESCRIPTION: Conversion function geographic coordinates into screen.
//INPUTS : const MRect& .
//RETURNS : QRect.
    QRect GeoToSCR(const MRect& lrect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToGeo( int iVal).
//DESCRIPTION: Conversion function logic coordinates into geographic.
//INPUTS : int .
//RETURNS : double.
	double LogicToGeo( int iVal);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MKartaInterface().
//DESCRIPTION: Conversion function logic coordinates into geographic.
//INPUTS : NONE.
//RETURNS : NONE.
	int GeoToLogic( double dVal);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToGeoSmall(QPoint& lPoint, bool bInMapCoo = false).
//DESCRIPTION: Functions of transformation of screen coordinates into geographic
// 				for small map.
//INPUTS : QPoint& , bool.
//RETURNS : MPoint.
	MPoint LogicToGeoSmall(QPoint& lPoint, bool bInMapCoo = false);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToLogicSmall(MPoint& gPoint).
//DESCRIPTION: Functions of transformation of geographic coordinates into screen
// 				for small map.
//INPUTS : MPoint&.
//RETURNS : QPoint.
	QPoint GeoToLogicSmall(MPoint& gPoint);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToGeoSmall( QRect& lrect, bool bInMapCoo = false).
//DESCRIPTION: Functions of transformation of screen coordinates into geographic
// 		for small map.
//INPUTS : QRect& , bool.
//RETURNS : MRect.
    const MRect LogicToGeoSmall(const QRect& lrect, bool bInMapCoo = false);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToLogicSmall( MRect& lrect).
//DESCRIPTION: Functions of transformation of geographic coordinates into screen
// 				for small map.
//INPUTS : MRect&.
//RETURNS : QRect.
    QRect GeoToLogicSmall(const MRect& lrect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToGeoSmall( int iVal).
//DESCRIPTION: Functions of transformation of screen coordinates into geographic
// 				for small map.
//INPUTS : int.
//RETURNS : double.
	double LogicToGeoSmall( int iVal);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToLogicSmall( double dVal).
//DESCRIPTION: Functions of transformation of geographic coordinates into screen
// 				for small map.
//INPUTS : double.
//RETURNS : int.
	int GeoToLogicSmall( double dVal);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToGauss(MPoint& pt, double meredian = 1000).
//DESCRIPTION: Function of conversion from geodesic's into Gauss's.
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	static MPoint GeoToGauss(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussToGeo(MPoint& pt, double meredian = 1000).
//DESCRIPTION: Function of conversion from Gauss's into geodesic's.
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	static MPoint GaussToGeo(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToGaussStandart(MPoint& pt, double meredian = 1000).
//DESCRIPTION: Function of conversion from geodesic's into Gauss's.
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	static MPoint GeoToGaussStandart(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussStandartToGeo(MPoint& pt, double meredian = 1000).
//DESCRIPTION: Function of conversion from Gauss's into geodesic's.
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	static MPoint GaussStandartToGeo(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToGaussLocal(MPoint& pt).
//DESCRIPTION: Function of conversion from geodesic's into Gauss's.
//INPUTS : MPoint&.
//RETURNS : MPoint.
	MPoint GeoToGaussLocal(MPoint& pt);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussLocalToGeo(MPoint& pt).
//DESCRIPTION: Function of conversion from Gauss's into geodesic's.
//INPUTS : MPoint&.
//RETURNS : MPoint.
	MPoint GaussLocalToGeo(MPoint& pt);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussLocalTrueMToGeo(MPoint& pt).
//DESCRIPTION: Particular case of GaussLocalToGeo: 
// 				if local system is oriented by true meridian.
//INPUTS : MPoint&.
//RETURNS : MPoint.
	MPoint GaussLocalTrueMToGeo(MPoint& pt);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GeoToGaussLocalTrueM(MPoint& pt).
//DESCRIPTION: Particular case of GeoToGaussLocal.
//INPUTS : MPoint&.
//RETURNS : MPoint.
	MPoint GeoToGaussLocalTrueM(MPoint& pt);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussToGaussLocal(MPoint& pt, double meredian = 1000).
//DESCRIPTION:
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	MPoint GaussToGaussLocal(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussLocalToGauss(MPoint& pt, double meredian = 1000).
//DESCRIPTION: 
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	MPoint GaussLocalToGauss(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussToGaussStandart(MPoint& pt, double meredian = 1000).
//DESCRIPTION: 
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	static MPoint GaussToGaussStandart(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussStandartToGauss(MPoint& pt, double meredian = 1000).
//DESCRIPTION: 
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	static MPoint GaussStandartToGauss(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussStandartToGaussLocal(MPoint& pt, double meredian = 1000).
//DESCRIPTION: 
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	MPoint GaussStandartToGaussLocal(MPoint& pt, double meredian = 1000);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussLocalToGaussStandart(MPoint& pt, double meredian = 1000).
//DESCRIPTION: Conversion function logic coordinates into geographic.
//INPUTS : MPoint& , double.
//RETURNS : MPoint.
	MPoint GaussLocalToGaussStandart(MPoint& pt, double meredian = 1000);


	// Conversion from / to the coordinate system Georef (World Geographic Reference System)
	/*		0 for nearest degree
	*       1 for nearest ten minutes
	*       2 for nearest minute
	*       3 for nearest tenth of a minute
	*       4 for nearest hundredth of a minute
	*       5 for nearest thousandth of a minute
	*/


	MString	GeoToGeoref(MPoint pt, int precision=5 );


	MPoint	GeorefToGeo(MString georef);


	MPoint UtmToGeo(MPoint utmPt, int zone, bool nPole);


	MString GeoToUtmText(MPoint geoPt);


	MPoint GeoToUtm(MPoint geoPt, int &zone);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GaussConvergence(const MPoint& pt, int zone=0).
//DESCRIPTION: Returns the Gaussian approximation meridians in degrees relative to the zone 'zone'
//				or of "their" area if 'zone' is not defined or equal to 0.
//INPUTS : const MPoint& , int.
//RETURNS : double.
    double GaussConvergence(const MPoint& pt, int zone=0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetReperPoint(MPoint point).
//DESCRIPTION: Sets the reference point for the local coordinate system Gauss-Kruger.
//INPUTS : MPoint.
//RETURNS : NONE.
	void SetReperPoint(MPoint point) {m_reper = ConvertPoint(point);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetReperPoint.
//DESCRIPTION: Returns the set reference point for lokalShnoy coordinate system Gauss-Kruger.
//INPUTS : const MPoint& , int.
//RETURNS : MPoint.
	MPoint GetReperPoint() {return ConvertPoint(m_reper, false);}

    //double MKartaInterface::GetScale();


  //PACKAGE : MAP.
//FUNCTION: MKartaInterface::ZoomTo(MRect& rect).
//DESCRIPTION: Extends map to defined rectangle.
//INPUTS : MRect&.
//RETURNS : NONE.  
    void ZoomTo(const MRect& rect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ScrollByGeo(double x, double y).
//DESCRIPTION: Scrolls map by geographic value.
//INPUTS : double , double .
//RETURNS : NONE.
	void ScrollByGeo(double x, double y);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ScrollByLogic(int x, int y).
//DESCRIPTION: Scroll map by logical value.
//INPUTS : int , int .
//RETURNS : NONE.
	void ScrollByLogic(int x, int y);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetPixmap().
//DESCRIPTION: Returns bit image with map without active environment.
//INPUTS : NONE.
//RETURNS : QPixmap pointer.
	QPixmap* GetPixmap() {return m_pixmap;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetPixmapAct().
//DESCRIPTION: Returns bit image with map and with active environment.
//INPUTS : NONE.
//RETURNS : QPixmap pointer.
	QPixmap* GetPixmapAct() {return m_pixmapact;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetPixmapSmall().
//DESCRIPTION: Returns bit image with map without active environment for small map.
//INPUTS : NONE.
//RETURNS : QPixmap pointer.
	QPixmap* GetPixmapSmall() {return m_pixmapsmall;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetPixmapActSmall().
//DESCRIPTION: Returns bit image with map with active environment for small map.
//INPUTS : NONE.
//RETURNS : QPixmap pointer.
	QPixmap* GetPixmapActSmall() {return m_pixmapactsmall;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSnapshot(MRect &geoRect, double scale, QPixmap *&snap, bool drawCooGrid=false, bool drawScaleGrid=false).
//DESCRIPTION: Writes the specified region geoRect with ZOOM scale denominator in the snap, if successful returns true,
// 				if it is not possible to create a specified region Qpixmap / scale returns false.
//INPUTS : MRect &, double , QPixmap pointer, bool , bool .
//RETURNS : True.  
	bool GetSnapshot(MRect &geoRect, double scale, QPixmap *&snap, bool drawCooGrid=false, bool drawScaleGrid=false);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSnapshot(MRect &geoRect, QSize widgetSize, QPixmap *&snap, bool drawCooGrid=false, bool drawScaleGrid=false).
//DESCRIPTION: Writes the specified region geoRect with ZOOM scale denominator in the snap, if successful returns true,
// 				if it is not possible to create a specified region Qpixmap / scale returns false.
//INPUTS : MRect &, QSize , QPixmap *&, bool , bool .
//RETURNS : True.
	bool GetSnapshot(MRect &geoRect, QSize widgetSize, QPixmap *&snap, bool drawCooGrid=false, bool drawScaleGrid=false);

	//-= management geographic layers =-

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetTile(int zoom, int x, int y, QImage *&img)
//DESCRIPTION: Writes the web map tile
//RETURNS : True.
bool GetTile(int zoom, int x, int y, QImage *&img);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::AddGeoLayer(double uid, MLayer* pLay = NULL).
//DESCRIPTION: Adds geographic layer.
//INPUTS : double , MLayer pointer.
//RETURNS : True.	
	bool AddGeoLayer(double uid, MLayer* pLay = NULL);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteGeoLayer(double uid).
//DESCRIPTION: Deletes gejgraphic layer by it's identifier.
//INPUTS : double.
//RETURNS : True.
	bool DeleteGeoLayer(double uid);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetWorkingGeoLayer().
//DESCRIPTION: Returns identifier of active geographic layer.
//INPUTS : NONE.
//RETURNS : double.
	double GetWorkingGeoLayer();


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetWorkingGeoLayer(double uid).
//DESCRIPTION: Sets geographic layer with defined active identifier.
//INPUTS : double.
//RETURNS : NONE.
	void SetWorkingGeoLayer(double uid);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindGeoLayer(MLayer* pLay).
//DESCRIPTION: Searchs for geographic layer and return it's identifier.
//INPUTS : MLayer pointer.
//RETURNS : double.
	double FindGeoLayer(MLayer* pLay);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindGeoLayer(double uid).
//DESCRIPTION: Searchs for geographic layer by identifier and returns pointer to it.
//INPUTS : double.
//RETURNS : MLayer pointer.
	MLayer* FindGeoLayer(double uid);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerVisible(bool bVisible).
//DESCRIPTION: Sets visibility of geographic layer.
//INPUTS : bool.
//RETURNS : True.
	bool SetGeoLayerVisible(bool bVisible);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerCommon(bool bCommon).
//DESCRIPTION: Sets flag of applying/non applying of characteristics of layer at geographic layer drawing.
//INPUTS : bool .
//RETURNS : True.
	bool SetGeoLayerCommon(bool bCommon);
	
	// -=Functions of active layers control=-

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::AddActLayer(double uid, MLayer* pLay = NULL).
//DESCRIPTION: Adds active layer.
//INPUTS : double , MLayer pointer.
//RETURNS : True.
	bool AddActLayer(double uid, MLayer* pLay = NULL);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteActLayer(double uid).
//DESCRIPTION: Deletes active layer by it's identifier.
//INPUTS : double.
//RETURNS : True.
	bool DeleteActLayer(double uid);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetWorkingActLayer().
//DESCRIPTION: Returns identifier of active layer.
//INPUTS : NONE.
//RETURNS : double.
	double GetWorkingActLayer();


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetWorkingActLayer(double uid).
//DESCRIPTION: Sets active layer with defined active identifier.
//INPUTS : double.
//RETURNS : NONE.
	void SetWorkingActLayer(double uid);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindActLayer(MLayer* pLay).
//DESCRIPTION: Searchs for active layer and return it's identifier.
//INPUTS : MLayer pointer.
//RETURNS : double.
	double FindActLayer(MLayer* pLay);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindActLayer(double uid).
//DESCRIPTION: Searchs for active layer by identifier and returns pointer to it.
//INPUTS : double.
//RETURNS : MLayer pointer.
    MLayer* FindActLayer(double uid) const;


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerVisible(bool bVisible).
//DESCRIPTION: Sets visibility of active layer.
//INPUTS : bool bVisible.
//RETURNS : True.
	bool SetActLayerVisible(bool bVisible);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerVisible(double uid, bool bVisible).
//DESCRIPTION: Sets visibility of active layer.
//INPUTS : double , bool .
//RETURNS : True.
	bool SetActLayerVisible(double uid, bool bVisible);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommon(bool bCommon).
//DESCRIPTION: Sets flag of applying/non applying of characteristics of layer at active layer drawing.
//INPUTS : bool .
//RETURNS : True.
	bool SetActLayerCommon(bool bCommon);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommon(double uid, bool bCommon).
//DESCRIPTION: Sets flag of applying/non applying of characteristics of layer at active layer drawing.
//INPUTS : double , bool .
//RETURNS : True.
	bool SetActLayerCommon(double uid, bool bCommon);

	// -=Functions of characteristics setting of working layer=-

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckWorkingActLayer().
//DESCRIPTION: Checks for availability of operational active layer.
//INPUTS : NONE.
//RETURNS : True.	
	bool CheckWorkingActLayer()
	{
		if(m_actLayer) return true;
		return false;
	}


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckWorkingGeoLayer().
//DESCRIPTION: Cheks for availability of operational geographic layer.
//INPUTS : NONE.
//RETURNS : True.
	bool CheckWorkingGeoLayer()
	{
		if(m_geoLayer) return true;
		return false;
	}


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerCommonColorPen(MColor commonColor).
//DESCRIPTION: Sets line color of geographic layer.
//INPUTS : MColor.
//RETURNS : True.	
	bool SetGeoLayerCommonColorPen(MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonColorPen(MColor commonColor).
//DESCRIPTION: Sets line color of geographic layer.
//INPUTS : MColor.
//RETURNS : True.
	bool SetActLayerCommonColorPen(MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonColorPen(double uid, MColor commonColor).
//DESCRIPTION: Sets line color of geographic layer.
//INPUTS : double , MColor .
//RETURNS : True.
	bool SetActLayerCommonColorPen(double uid, MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerCommonColorBrush(MColor commonColor).
//DESCRIPTION: Sets background color of geographic layer.
//INPUTS : MColor.
//RETURNS : True.
	bool SetGeoLayerCommonColorBrush(MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonColorBrush(MColor commonColor).
//DESCRIPTION: Sets background color of active layer.
//INPUTS : MColor.
//RETURNS : True.
	bool SetActLayerCommonColorBrush(MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonColorBrush(double uid, MColor commonColor).
//DESCRIPTION: Sets background color of active layer.
//INPUTS : double , MColor .
//RETURNS : True.
	bool SetActLayerCommonColorBrush(double uid, MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerCommonColorText(MColor commonColor).
//DESCRIPTION: Sets text color of geographic layer.
//INPUTS : MColor.
//RETURNS : True.
	bool SetGeoLayerCommonColorText(MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonColorText(MColor commonColor).
//DESCRIPTION: Sets text color of active layer.
//INPUTS : MColor.
//RETURNS : True.
	bool SetActLayerCommonColorText(MColor commonColor);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonColorText(double uid, MColor commonColor).
//DESCRIPTION: Sets text color of active layer.
//INPUTS : double , MColor .
//RETURNS : True.
	bool SetActLayerCommonColorText(double uid, MColor commonColor);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerCommonPenWidth(int iPenWidth).
//DESCRIPTION: Sets line thickness (in locical units) of geographic layer.
//INPUTS : int.
//RETURNS : True.
	bool SetGeoLayerCommonPenWidth(int iPenWidth);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonPenWidth(int iPenWidth).
//DESCRIPTION: Sets line thickness (in locical units) of active layer.
//INPUTS : int.
//RETURNS : True.
	bool SetActLayerCommonPenWidth(int iPenWidth);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonPenWidth(double uid, int iPenWidth).
//DESCRIPTION: Sets line thickness (in locical units) of active layer.
//INPUTS : double , int .
//RETURNS : True.
	bool SetActLayerCommonPenWidth(double uid, int iPenWidth);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetGeoLayerCommonTextWidth(int iTextWidth).
//DESCRIPTION: Sets text dimension (in ogical units) of geographic layer.
//INPUTS : int.
//RETURNS : True.
	bool SetGeoLayerCommonTextWidth(int iTextWidth);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonTextWidth(int iTextWidth).
//DESCRIPTION: Sets text dimension (in logical units) of active layer.
//INPUTS : int.
//RETURNS : True.
	bool SetActLayerCommonTextWidth(int iTextWidth);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActLayerCommonTextWidth(double uid, int iTextWidth).
//DESCRIPTION: Sets text dimension (in logical units) of active layer.
//INPUTS : double , int .
//RETURNS : True.
	bool SetActLayerCommonTextWidth(double uid, int iTextWidth);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindTopObjectsAtPointExact(MPoint& point, MObjectList* objList, int delta = 5).
//DESCRIPTION: Searchs for topobjects in selected point by all layers (result as a list).
//INPUTS : MPoint& point, MObjectList* objList, int delta.
//RETURNS : int.
    int FindTopObjectsAtPointExact(const MPoint& point, MObjectList* objList, int delta = 5);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindActObjectsAtPointExact(MPoint& point, MObjectList* objList, int delta = 5).
//DESCRIPTION: Searchs for topobjects in selected point by all layers (result as a list).
//INPUTS : MPoint& point, MObjectList* objList, int delta = 5.
//RETURNS : int.
    int FindActObjectsAtPointExact(const MPoint& point, MObjectList* objList, int delta = 5);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::AddObject(double uid, MTopobject* pObject = NULL).
//DESCRIPTION: Add object.
//INPUTS : double uid, MTopobject* pObject = NULL.
//RETURNS : True.	
	bool AddObject(double uid, MTopobject* pObject = NULL);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteObject(double uid).
//DESCRIPTION: Delete object.
//INPUTS : double.
//RETURNS : True.
	bool DeleteObject(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteAllObjectsFromLayer(double uid).
//DESCRIPTION: Delete all objects from layer.
//INPUTS : double.
//RETURNS : NONE.
	void DeleteAllObjectsFromLayer(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteAllObjectsFromLayer().
//DESCRIPTION: Delete all objects from layer.
//INPUTS : NONE.
//RETURNS : NONE.
	void DeleteAllObjectsFromLayer();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteObjectFromLayers(double uid).
//DESCRIPTION: Delete all objects from layers.
//INPUTS : double.
//RETURNS : True.
	bool DeleteObjectFromLayers(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObject(double uid).
//DESCRIPTION: Searching object.
//INPUTS : double.
//RETURNS : MTopobject pointer.
	MTopobject* FindObject(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectAtLayers(double uid).
//DESCRIPTION: Searching object at layers.
//INPUTS : double.
//RETURNS : True.
	bool FindObjectAtLayers(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, MObjectList* objList).
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& , MObjectList pointer.
//RETURNS : int.
	int FindObjectsAtPoint(MPoint& point, MObjectList* objList);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, vector<double>* vect).
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& , vector<double> pointer.
//RETURNS : int.
	int FindObjectsAtPoint(MPoint& point, vector<double>* vect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, int delta, MObjectList* objList).
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& , int , MObjectList pointer.
//RETURNS : int.
	int FindObjectsAtPoint(MPoint& point, int delta, MObjectList* objList);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, int delta, vector<double>* vect).
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& point, int delta, vector<double>* vect.
//RETURNS : int.
	int FindObjectsAtPoint(MPoint& point, int delta, vector<double>* vect);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, MObjectList* objList, int delta = 0).
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& , MObjectList pointer, int.
//RETURNS : int.
	int FindObjectsAtPointExact(MPoint& point, MObjectList* objList, int delta = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, vector<double>* vect, int delta = 0.
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& , vector<double> pointer, int .
//RETURNS : int.
	int FindObjectsAtPointExact(MPoint& point, vector<double>* vect, int delta = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta = 0).
//DESCRIPTION: Searching objects at point.
//INPUTS : MPoint& , vector<FindObjectStruct> pointer, int .
//RETURNS : int.
	int FindObjectsAtPointExact(MPoint& point, vector<FindObjectStruct>* vect, int delta = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindObjInLayerByAttrValue(MString m_LayerName,MString m_Value, MString m_AttrName).
//DESCRIPTION: Serching for object in definite layer (m_parLayerName)
// 				by definite value (m_parValue)
// 				of given attribute (m_parAttrName).
//INPUTS : MString ,MString , MString .
//RETURNS : True.	
	bool   FindObjInLayerByAttrValue(MString m_LayerName,MString m_Value, MString m_AttrName);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::FindKvadrokodesInLayerByObjType(MString m_LayerName, int iObjType).
//DESCRIPTION: Searching for quadrocodes of objects of definite type (iObjType) in definite layer (m_LayerName)
// 				object type is defined by constant - (F_PLO, F_DOT, F_LIN [Yaz.h]), layer is defined by name
//  			returns a list of found quadrocodes.
//INPUTS : MString , int .
//RETURNS : QStringList.
	QStringList FindKvadrokodesInLayerByObjType(MString m_LayerName, int iObjType);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SelectObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta = 0).
//DESCRIPTION: Selects all object in defined point.
//INPUTS : MPoint& , vector<FindObjectStruct> pointer, int .
//RETURNS : int.	
	int SelectObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeselectObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta = 0).
//DESCRIPTION: Cancel selection for all objects of defined point.
//INPUTS : MPoint& , vector<FindObjectStruct> pointer, int.
//RETURNS : int.
	int DeselectObjectsAtPoint(MPoint& point, vector<FindObjectStruct>* vect, int delta = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeselectAll().
//DESCRIPTION: Deselect all objects. the result true, if the selected objects have been found.
//INPUTS : NONE.
//RETURNS : True.
	bool DeselectAll();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetWorkingObject(double uid).
//DESCRIPTION: Sets active object in operational active layer.
//INPUTS : double.
//RETURNS : True.
	bool SetWorkingObject(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetWorkingObject().
//DESCRIPTION: Gets operational object in operational active layer.
//INPUTS : NONE.
//RETURNS : double.
	double GetWorkingObject();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetPriv(MPoint& point, int formula_index = 0).
//DESCRIPTION: Sets snap.
//INPUTS : MPoint& , int .
//RETURNS : True.
	bool SetPriv(MPoint& point, int formula_index = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetPriv(int formula_index = 0).
//DESCRIPTION: Sets snap.
//INPUTS : int .
//RETURNS : True.
	bool SetPriv(int formula_index = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ResetAllPrivs(MPlotDevice* pDev).
//DESCRIPTION: Resets all snaps subjects to text size limit.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	void ResetAllPrivs(MPlotDevice* pDev);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteAllPriv(double uid, int formula_index = 0).
//DESCRIPTION: Deletes all snaps subjects to text size limit.
//INPUTS : double , int .
//RETURNS : NONE.
	void DeleteAllPriv(double uid, int formula_index = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteAllPriv( int formula_index = 0).
//DESCRIPTION: Deletes all snaps subjects to text size limit.
//INPUTS : int.
//RETURNS : NONE.
	void DeleteAllPriv( int formula_index = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetFormularName(MString formularname).
//DESCRIPTION: Sets formular name.
//INPUTS : MString.
//RETURNS : NONE.
	void SetFormularName(MString formularname);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetFormular(MString& formularname, MString& formular).
//DESCRIPTION: Sets formular with established name to operational object of active layer.
//INPUTS : MString& , MString& .
//RETURNS : True.
	bool SetFormular(MString& formularname, MString& formular);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetFormular(MString formular).
//DESCRIPTION: Sets formular with established name to operational object of active layer.
//INPUTS : MString.
//RETURNS : True.
	bool SetFormular(MString formular);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckWorkingObject().
//DESCRIPTION: Checks for availability of operational objects in active layer.
//INPUTS : NONE.
//RETURNS : True.
	bool CheckWorkingObject()
	{
		if(m_Object) return true;
		return false;
	}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetStatusWorkingObject().
//DESCRIPTION: Get the state of working object.
//INPUTS : NONE.
//RETURNS : True.
    bool GetStatusWorkingObject() {return m_Object->m_status;}
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetStatusWorkingObject(bool bStatus).
//DESCRIPTION: Set the state of working object.
//INPUTS : bool.
//RETURNS : NONE.
    void SetStatusWorkingObject(bool bStatus) {m_Object->m_status = bStatus;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetExcitedStatusWorkingObject().
//DESCRIPTION: Get the drawing state of working object.
//INPUTS : NONE.
//RETURNS : True.
    bool GetExcitedStatusWorkingObject() {return m_Object->m_excited;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetExcitedStatusWorkingObject(bool bExcite).
//DESCRIPTION:  Set the drawing state for working object.
//INPUTS : bool.
//RETURNS : NONE.
    void SetExcitedStatusWorkingObject(bool bExcite) {m_Object->m_excited = bExcite;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetZnak(MString& kkod).
//DESCRIPTION: Sets sign of operational object.
//INPUTS : MString&.
//RETURNS : True.	
    bool SetZnak(const MString& kkod);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeletePoint(int pos).
//DESCRIPTION: Deletes point from object.
//INPUTS : int.
//RETURNS : True.
	bool DeletePoint(int pos);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::InsertPoint(int pos, MPoint point).
//DESCRIPTION: Inserts point in active object.
//INPUTS : int , MPoint .
//RETURNS : True.
	bool InsertPoint(int pos, MPoint point);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::AddPoint(MPoint& point).
//DESCRIPTION: Adds point to active object.
//INPUTS : MPoint&.
//RETURNS : True.
	bool AddPoint(MPoint& point);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetPoints(MPoint* points, int size).
//DESCRIPTION: Sets points.
//INPUTS : MPoint pointer, int .
//RETURNS : True.
	bool SetPoints(MPoint* points, int size);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MoveTo(MPoint& delta).
//DESCRIPTION: Moves object.
//INPUTS : MPoint&.
//RETURNS : True.
	bool MoveTo(MPoint& delta);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MoveTo(double distance, double azimut).
//DESCRIPTION: Moves object.
//INPUTS : double , double .
//RETURNS : True.
	bool MoveTo(double distance, double azimut);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SelectObject().
//DESCRIPTION: Selects operational object.
//INPUTS : NONE.
//RETURNS : NONE.
	void SelectObject();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeselectObject().
//DESCRIPTION: Cancel selection of operational object.
//INPUTS : NONE.
//RETURNS : NONE.
	void DeselectObject();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SelectObject(double uid).
//DESCRIPTION: Selects object with number 'uid'.
//INPUTS : double.
//RETURNS : NONE.	
	void SelectObject(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeselectObject(double uid).
//DESCRIPTION: Cancel selection from object with number 'uid'.
//INPUTS : double.
//RETURNS : NONE.
	void DeselectObject(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SelectObjectsInLayerByUID(double layer_uid,double uid).
//DESCRIPTION: in map act_layer with defined 'layer_uid' selects objects with defined 'UID'.
//INPUTS : double ,double .
//RETURNS : NONE.
	void SelectObjectsInLayerByUID(double layer_uid,double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeselectObjectsInLayerByUID(double layer_uid,double uid).
//DESCRIPTION: in map act_layer with defined 'layer_uid' deselects objects with defined 'UID'.
//INPUTS : double ,double .
//RETURNS : NONE.
	void DeselectObjectsInLayerByUID(double layer_uid,double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckSelectedObject().
//DESCRIPTION: Cheks for availability of selected operational object.
//INPUTS : NONE.
//RETURNS : True.
	bool CheckSelectedObject();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckSelectedObject(double uid).
//DESCRIPTION: Cheks if operational object with number 'uid' is selected?
//INPUTS : double.
//RETURNS : True.
	bool CheckSelectedObject(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetSelectedColor(MColor& col).
//DESCRIPTION: Sets color of selected operational object.
//INPUTS : MColor&.
//RETURNS : NONE.
    void SetSelectedColor(const MColor& col);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetSelectedColor(double uid, MColor& col).
//DESCRIPTION: Sets color of selected object with number 'uid'.
//INPUTS : double , MColor& .
//RETURNS : NONE.
	void SetSelectedColor(double uid, MColor& col);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetZnak(double uid, MString& kkod).
//DESCRIPTION: double uid, MString& kkod.
//INPUTS : double , MString& .
//RETURNS : True.
	bool SetZnak(double uid, MString& kkod);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CorrectZnak(MTopobject* pActObj).
//DESCRIPTION: corrects pActObj depending on Znak which was set to object
// 		function is called on the end of SetZnak(double uid, MString& kkod) & SetZnak(MString& kkod).
//INPUTS : MTopobject pointer.
//RETURNS : True.	
	bool CorrectZnak(MTopobject* pActObj);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeletePoint(double uid, int pos).
//DESCRIPTION: Deletes point from object.
//INPUTS : double , int .
//RETURNS : True.
	bool DeletePoint(double uid, int pos);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::InsertPoint(double uid, int pos, MPoint point).
//DESCRIPTION: Inserts point into operational object.
//INPUTS : double , int , MPoint .
//RETURNS : True.
	bool InsertPoint(double uid, int pos, MPoint point);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::AddPoint(double uid, MPoint& point).
//DESCRIPTION: Adds point to operational object.
//INPUTS : double , MPoint& .
//RETURNS : True.
	bool AddPoint(double uid, MPoint& point);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetPoints(double uid, MPoint* points, int size).
//DESCRIPTION: Sets points.
//INPUTS : double , MPoint pointer, int .
//RETURNS : True.
	bool SetPoints(double uid, MPoint* points, int size);  

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MoveTo(double uid, MPoint& delta).
//DESCRIPTION: Moves object.
//INPUTS : double , MPoint& .
//RETURNS : True.
	bool MoveTo(double uid, MPoint& delta);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::MoveTo(double uid, double distance, double azimut).
//DESCRIPTION: Moves object.
//INPUTS : double , double , double .
//RETURNS : True.
	bool MoveTo(double uid, double distance, double azimut);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckKKod(MString& kkod).
//DESCRIPTION: Checks if sign with establlished quadrocode exists?
//INPUTS : MString&.
//RETURNS : True.
	bool CheckKKod(MString& kkod);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckPoint(MPoint pt).
//DESCRIPTION: Checks if point inside of bounding rectangle of the map?
//INPUTS : MPoint.
//RETURNS : True.
	bool CheckPoint(MPoint pt);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetMatrixFile(MString path).
//DESCRIPTION: Sets path to map file.
//INPUTS : MString.
//RETURNS : True.
	bool SetMatrixFile(MString path);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetMatrixObjectFile(MString path).
//DESCRIPTION: Sets path to file of altitude matrix of objects.
//INPUTS : MString.
//RETURNS : True.
	bool SetMatrixObjectFile(MString path);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetMatrixCorrectionFile(MString path).
//DESCRIPTION: Sets path to correction matrix file.
//INPUTS : MString.
//RETURNS : True.
	bool SetMatrixCorrectionFile(MString path);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetHight(MPoint& pt, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL).
//DESCRIPTION: Returns altitude by altitude matrix of objects and relief.
//INPUTS : MPoint& , double pointer, double pointer, double pointer.
//RETURNS : int.
	// 	int GetHight(MPoint& pt, double* hRel = NULL, double* hObj = NULL);
        int GetHight(const MPoint& pt, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);
	

	int GetHightRow(MPoint& pt,MInArray* values, int count);


	int GetHightObjects(MPoint& pt);


	int GetHightRowObjects(MPoint& pt,MInArray* values, int count);


	int GetHightRelief(MPoint& pt);


	int GetHightCorrection(MPoint& pt);


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetDistance(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y).
//DESCRIPTION: It calculates the distance
// 				finds the distance (in meters ) between the points pt1, pt2
// 				pt1, pt2 served in the geo coordinates.
//INPUTS : const MPoint& , const MPoint& , double , double .
//RETURNS : double.
    double GetDistance(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y) const;//6378245,  axis_Y  = 6356863.02

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetAzimut(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y).
//DESCRIPTION: Calculates the azimuth ( in deci - degrees) between points pt1, pt2
// 				pt1, pt2 served in the geo coordinates.
//INPUTS : const MPoint& , const MPoint& , double , double .
//RETURNS : double.
    double GetAzimut(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y) const;

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetGridAzimut(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y).
//DESCRIPTION: Calculates the azimuth ( in deci - degrees) between points pt1, pt2.
//INPUTS : const MPoint& , const MPoint& , double , double .
//RETURNS : double.	
    double GetGridAzimut(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetPointByAzimuthLength(MPoint pt1, double azimuth, double ds, double poluos1 = axis_X, double poluos2 = axis_Y).
//DESCRIPTION: calculates coordinates of a point in range and azimuth of the initial point
        /* Pt1 - the starting point in the geo coordinates ( longitude, latitude )
		azimuth - angle ( azimuth) in degrees
		ds - distance (range ) in meters
		poluos1 - semi-major axis of the reference ellipsoid
		poluos2 - minor axis of the reference ellipsoid*/	
//INPUTS : MPoint , double , double , double , double .
//RETURNS : MPoint.
	MPoint GetPointByAzimuthLength(MPoint pt1, double azimuth, double ds, double poluos1 = axis_X, double poluos2 = axis_Y);
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeltaRungeKutta(double alpha, double phi, double S0, double betta, double Pi, double& deltaB, double& deltaL, double& deltaA).
//DESCRIPTION: function for delta-parts calculating for the method of Runge-Kutt-England: is used in GetPointByAzimuthLength(..)
	/*  alpha,phi,S0,betta - input parameters
		Pi - Pi-value (3.14....)
		deltaB,deltaL,deltaA - calculated delta-parts*/	
//INPUTS : double , double , double , double , double , double& , double& , double& .
//RETURNS : NONE.	
	void DeltaRungeKutta(double alpha, double phi, double S0, double betta, double Pi, double& deltaB, double& deltaL, double& deltaA);
	

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetPogreshnost(double delta1, double delta3, double delta4, double delta5, double delta6).
//DESCRIPTION: Function for local method error calculation: is used in GetPointByAzimuthLength(..)
//				all function parameters are delta-parts calculated by the method of Runge-Kutt-England.
//INPUTS : double , double , double , double , double .
//RETURNS : double.
	double GetPogreshnost(double delta1, double delta3, double delta4, double delta5, double delta6)
		{return (-42*delta1-224*delta3-21*delta4+162*delta5+125*delta6)/336.0;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ConvertPoint(const MPoint& pt, bool bIn = true).
//DESCRIPTION: Converts a point to geodetic coordinates.
//INPUTS : const MPoint& , bool .
//RETURNS : MPoint.	
    MPoint ConvertPoint(const MPoint& pt, const bool &bIn = true) const;

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetDrawCooGrid(bool m_bFlag).
//DESCRIPTION: Set flag drawing grid.
//INPUTS : bool.
//RETURNS : NONE.	
	void SetDrawCooGrid(bool m_bFlag) {m_bDrawCooGrid = m_bFlag;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetDrawCooGrid().
//DESCRIPTION: Receive flag drawing grid.
//INPUTS : NONE.
//RETURNS : True.
	bool GetDrawCooGrid() {return m_bDrawCooGrid;}

    //PACKAGE : MAP.
    //FUNCTION: MKartaInterface::SetDrawMGRSCooGrid(bool m_bFlag).
    //DESCRIPTION: Set flag drawing mgrs grid.
    //INPUTS : bool.
    //RETURNS : NONE.
        void SetDrawMGRSCooGrid(bool m_bFlag) {m_bDrawMGRSCooGrid = m_bFlag;}

    //PACKAGE : MAP.
    //FUNCTION: MKartaInterface::GetDrawMGRSCooGrid().
    //DESCRIPTION: Receive flag drawing mgrs grid.
    //INPUTS : NONE.
    //RETURNS : True.
        bool GetDrawMGRSCooGrid() {return m_bDrawMGRSCooGrid;}


//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetDrawScaleGrid(bool m_bFlag).
//DESCRIPTION: Set flag drawing grid.
//INPUTS : bool.
//RETURNS : NONE.
	void SetDrawScaleGrid(bool m_bFlag) {m_bDrawScaleGrid = m_bFlag;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetDrawScaleGrid().
//DESCRIPTION: Receive flag drawing grid.
//INPUTS : NONE.
//RETURNS : True.
	bool GetDrawScaleGrid() {return m_bDrawScaleGrid;}

//PACKAGE : MAP.
//FUNCTION : MKartaInterface::AddSheet(MString sheet_name).
//DESCRIPTION : Adds map sheet.
//INPUTS : MString.
//RETURNS : True.
	bool AddSheet(MString sheet_name) {return m_karta->AddSheet(sheet_name);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::DeleteSheet(MString sheet_name).
//DESCRIPTION: Deletes map sheet.
//INPUTS : MString.
//RETURNS : True.
	bool DeleteSheet(MString sheet_name) {return m_karta->DeleteSheet(sheet_name);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetFilter(MString path).
//DESCRIPTION: Sets filter of objects loading.
//INPUTS : MString.
//RETURNS : True.
	bool SetFilter(MString path) {return m_karta->SetLoadFilterFile(path);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetDrawFilter(MString path).
//DESCRIPTION: Sets filter of conventional signs representation.
//INPUTS : MString.
//RETURNS : True.
	bool SetDrawFilter(MString path);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LoadSheetList(MString path).
//DESCRIPTION: Loads file with list of map sheets.
//INPUTS : MString.
//RETURNS : True.		
	bool LoadSheetList(MString path);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSheetAtPoint(MPoint pt, MRect* rec = 0).
//DESCRIPTION: Returns map sheet name in which point gets.
//INPUTS : MPoint , MRect pointer.
//RETURNS : MString.
	MString GetSheetAtPoint(MPoint pt, MRect* rec = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetDrawFilterSize(int size).
//DESCRIPTION: Sets/gets filter size of map drwing.
//INPUTS : int.
//RETURNS : NONE.
	void SetDrawFilterSize(int size) {if(m_karta) m_karta->SetDrawFilterSize(size);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetDrawFilterSize().
//DESCRIPTION: 
//INPUTS : NONE.
//RETURNS : int.
	int  GetDrawFilterSize() {if(m_karta) return m_karta->GetDrawFilterSize(); else return 0;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToBum(MPoint ptin,MPoint& ptout).
//DESCRIPTION: Function of conversion from logical coordinate system to paper.
//INPUTS : MPoint , MPoint& .
//RETURNS : NONE.
	void LogicToBum(MPoint ptin,MPoint& ptout)       { m_QtDevice-> LogicToBum( ptin, ptout);	}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::LogicToBum(MRect *pRectIn,MRect& pRectOut).
//DESCRIPTION: Function of conversion from logical coordinate system to paper.
//INPUTS : MRect pointer, MRect& .
//RETURNS : NONE.
	void LogicToBum(MRect *pRectIn,MRect& pRectOut)  { m_QtDevice-> LogicToBum( pRectIn, pRectOut); }

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::BumToLogicI(MRect *pRectIn,QRect& pRectOut).
//DESCRIPTION: Function of conversion from paper coordinate system to logical(integers).
//INPUTS : MRect pointer,QRect& .
//RETURNS : NONE.
	void BumToLogicI(MRect *pRectIn,QRect& pRectOut) { m_QtDevice-> BumToLogicI( pRectIn, pRectOut); }

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::BumToLogicI(MPoint *ptin,QPoint& ptout).
//DESCRIPTION: Function of conversion from paper coordinate system to logical(integers).
//INPUTS : MPoint opinter,QPoint& .
//RETURNS : NONE.
	void BumToLogicI(MPoint *ptin,QPoint& ptout){ m_QtDevice-> BumToLogicI( ptin, ptout); }

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetActPolygonDense(int dns).
//DESCRIPTION: Sets transparency level of area objects of active layer.
//INPUTS : int.
//RETURNS : True.
	bool SetActPolygonDense(int dns);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetActPolygonDense().
//DESCRIPTION: Returns level of transparency of area objects of active layer.
//INPUTS : NONE.
//RETURNS : int.
	int  GetActPolygonDense();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetTextRatio(float ratio = 1.0).
//DESCRIPTION: Sets coefficient of text value.
//INPUTS : float.
//RETURNS : True.
	bool   SetTextRatio(float ratio = 1.0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetTextRatio().
//DESCRIPTION: Returns coefficient of text value.
//INPUTS : NONE.
//RETURNS : float.
	float  GetTextRatio(); 

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetCentralMeridian(int mrd).
//DESCRIPTION: Sets 'mrd' value of central meridian in degrees.
//INPUTS : int .
//RETURNS : True.
	bool   SetCentralMeridian(int mrd); 
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetExtendedCentralMeridian(double meridian).
//DESCRIPTION: Sets the value of central meridian (in deci-degrees) 
// 				for a map in Gauss-Kruger coordinate system (extended or zone).
//INPUTS : double .
//RETURNS : True.
	bool   SetExtendedCentralMeridian(double meridian);

//bool SetAngle(int angel);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetAngle( int angle, double uid = 0).
//DESCRIPTION: Sets rotation angle of symbol for dotted object.
//INPUTS : int , double .
//RETURNS : True.
	bool   SetAngle( int angle, double uid = 0);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetObjAngle( int angle, double uid = 0).
//DESCRIPTION: Sets rotation angle of symbol for dotted object
//				uses the properties of ajustable classifier sign.
//INPUTS :  int , double .
//RETURNS : True.	
	bool   SetObjAngle( int angle, double uid = 0);

  //PACKAGE : MAP.
//FUNCTION: MKartaInterface::CycleGeoPoints(MPoint& pnt, int rad ).
//DESCRIPTION: Returns array of geopoints of circle with centre in 'pnt' and radius 'rad' in metres
//				Works incorrectly.
//INPUTS : MPoint& , int .
//RETURNS : MPtArray.
    MPtArray CycleGeoPoints(const MPoint& pnt, int rad );

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CycleGeoPoints(MPoint &pnt,MPtArray* arr, int radius).
//DESCRIPTION: Returns array of geopoints of circle with centre in 'pnt' and radius 'rad' in metres.
//INPUTS : MPoint &,MPtArray pointer, int .
//RETURNS : int.
    int CycleGeoPoints(const MPoint &pnt,MPtArray* arr, int radius);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CycleGeoPoints(MPoint &pnt,MPtArray* arr, int radius, double degStp).
//DESCRIPTION: Returns array of geopoints of circle with centre in 'pnt' and radius 'rad' in metres 
// 				with step degStp in degrees.
//INPUTS : MPoint & , MPtArray pointer, int , double .
//RETURNS : int.
    int CycleGeoPoints(const MPoint &pnt,MPtArray* arr, int radius, double degStp);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetObjectType(double uid=0).
//DESCRIPTION: Returns type(F_PLO, F_DOT, F_LIN) of object 'uid' or current(operational) object(uid=0).
//  			Returns -1 if object is not found.
//INPUTS : double.
//RETURNS : int.	
	int    GetObjectType(double uid=0); 

//PACKAGE : MAP.
//FUNCTION: MKartaInterface:: GetObjectType(MString &kkod).
//DESCRIPTION: Returns type of object or current(operational) object.
//  			Returns -1 if object is not found.
//INPUTS : MString &.
//RETURNS : int.
	int    GetObjectType(MString &kkod);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetWorkingObjectAtLayers(double uid).
//DESCRIPTION: Sets as operational an object with number uid .
//INPUTS : double.
//RETURNS : True.	
	bool   SetWorkingObjectAtLayers(double uid);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CheckSelectedObjectAtLayers(double uid).
//DESCRIPTION: Cheks if object with number 'uid' is operational in any active layer.
//INPUTS : double.
//RETURNS : True.
	bool   CheckSelectedObjectAtLayers(double uid);
  
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetVerboseFlag(bool flag= true).
//DESCRIPTION: Sets Indicator for outputing diag messages to stdout .
//INPUTS : bool .
//RETURNS : NONE.
	void   SetVerboseFlag(bool flag= true) {if(m_karta) m_karta->SetVerboseFlag(flag);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetVerboseFlag().
//DESCRIPTION: Returns current value of Indicator for outputing diag messages to stdout .
//INPUTS : NONE.
//RETURNS : True.
	bool   GetVerboseFlag()   {if(m_karta) return m_karta->GetVerboseFlag(); else return false;}
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetTextSizeMax(int size).
//DESCRIPTION: Sets max size of text at drawing.
//INPUTS : int.
//RETURNS : NONE.
	void SetTextSizeMax(int size) {if(m_karta) m_karta->SetTextSizeMax(size);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetTextSizeMax().
//DESCRIPTION: Gets max text size at drawing.
//INPUTS : NONE.
//RETURNS : int.
	int  GetTextSizeMax() {if(m_karta) return m_karta->GetTextSizeMax(); else return 0;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetTextSizeMin(int size).
//DESCRIPTION: Sets min text size at drawing.
//INPUTS : int.
//RETURNS : NONE.	
	void   SetTextSizeMin(int size) {if(m_karta) m_karta->SetTextSizeMin(size);}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetTextSizeMin().
//DESCRIPTION: Gets min text size at drawing.
//INPUTS : NONE.
//RETURNS : int.
	int    GetTextSizeMin() {if(m_karta) return m_karta->GetTextSizeMin(); else return 0;}

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetPixmapSize(QSize).
//DESCRIPTION: Sets mini-map size.
//INPUTS : QSize.
//RETURNS : True.	
	bool   SetPixmapSize(QSize);
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetRaster(MString filePath, MRect rect).
//DESCRIPTION: set the raster in rect.
//INPUTS : MString filePath, MRect rect.
//RETURNS : True.
	bool SetRaster(MString filePath, MRect rect);
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetDuga(MPoint* pt1, MPoint* pt2, MPoint* pt3, int index = RAZBIENIE).
//DESCRIPTION: Returns array of points, which is generate the arc by 3 points.
//INPUTS : MPoint pointer, MPoint pointer, MPoint pointer, int .
//RETURNS : MPtArray pointer.
	MPtArray* GetDuga(MPoint* pt1, MPoint* pt2, MPoint* pt3, int index = RAZBIENIE);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CreateSpline(MPtArray* pta, unsigned char vid, int number = RAZBIENIE).
//DESCRIPTION: Redefine array pta.
//INPUTS : MPtArray pointer, unsigned char , int .
//RETURNS : True.
	bool CreateSpline(MPtArray* pta, unsigned char vid, int number = RAZBIENIE);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CreateAttackDirection(MPtArray* pta, int index, int razbienie = 5).
//DESCRIPTION: Forming the attack directin arrow.
//INPUTS : MPtArray pointer, int , int .
//RETURNS : True.
	bool CreateAttackDirection(MPtArray* pta, int index, int razbienie = 5);
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CreateTask(MPtArray* pta).
//DESCRIPTION: Forming the task arrow.
//INPUTS : MPtArray pointer.
//RETURNS : True.
 	bool CreateTask(MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetEllipseQuarter(MPtArray* ptRet, MPoint pt1, MPoint pt2, double angle0, bool bChas, int number = RAZBIENIE).
//DESCRIPTION: The formation of a quarter elliptical arc, return an array ptRet.
//INPUTS : MPtArray pointer, MPoint , MPoint , double , bool , int .
//RETURNS : NONE.
	void GetEllipseQuarter(MPtArray* ptRet, MPoint pt1, MPoint pt2, double angle0, bool bChas, int number = RAZBIENIE);
  
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::CreateSubversiveGroup(MPtArray* pta).
//DESCRIPTION: Create array of points, which is forming the subversive group.
//INPUTS : MPtArray pointer.
//RETURNS : NONE.
	void CreateSubversiveGroup(MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetExcitedStatus(MPoint* point, int delta).
//DESCRIPTION: Set the drawing state to objects situated nearly <point> with accuracy <delta> (in pixels).
//INPUTS : MPoint pointer, int .
//RETURNS : NONE.
	void SetExcitedStatus(MPoint* point, int delta);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::UnExciteAll().
//DESCRIPTION: Set the drawing state of all objects in false.
//INPUTS : NONE.
//RETURNS : NONE.
	void UnExciteAll();

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::SetObjectInActLayer(double uidLay, double uidObj, MPtArray* pta, MString kkod, MString formStr, int& result).
//DESCRIPTION:  Check the object <uidObj> in operating layer <uidLay>
// 		result =
//  	 -1 - other cases
// 		 0 - incorrect <uid> of layer
// 		 1 - object <uidObj> is used in layer <uidLay>
// 		 2 - incorrect <pta>
// 		 3 - <kkod> is incorrectly (don't exist in kls)
// 		 4 - inconsistency between object type and point's quantity
// 		 5 - atribute <formStr> do not exist - in this case function returns true
// 		returns value:
// 		true - object <uidObj> added in layer <uidLay> with desired conditions
// 		false - doing nothing.
//INPUTS : double , double , MPtArray pointer, MString , MString , int& .
//RETURNS : True.
	bool SetObjectInActLayer(double uidLay, double uidObj, MPtArray* pta, MString kkod, MString formStr, int& result);
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetLine(MPoint &pnt1, MPoint &pnt2, MPtArray* arr, int step = STEP_M).
//DESCRIPTION: Redefine array <arr> by geopoints, which is generated the line
// 				with step <step>.
//INPUTS : MPoint &, MPoint &, MPtArray pointer, int .
//RETURNS : int.
        int GetLine(const MPoint &pnt1, const MPoint &pnt2, MPtArray *&arr, int step = STEP_M);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSector(MPoint* ptCenter, MPoint* pt1, double angle, MPtArray* arr, int step = STEP_M).
//DESCRIPTION: Redefine array <arr> by geopoints, which is generated the sector
//  		ptCenter = center, [ptCenter,pt1] = radius, angle = central angle of sector.
//INPUTS : MPoint pointer, MPoint pointer, double , MPtArray pointer, int .
//RETURNS : NONE.
	void GetSector(MPoint* ptCenter, MPoint* pt1, double angle, MPtArray* arr, int step = STEP_M);
	
//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ConvertGausMap(double cntrMeridian).
//DESCRIPTION: converts all objects map to the new central meridian.
//INPUTS : double.
//RETURNS : True.
	bool ConvertGausMap(double cntrMeridian);	

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ConvertWGS84_CK42(MPoint wgs84,MPoint& ck42 ).
//DESCRIPTION: Coordinate transformation between datums.
//INPUTS : MPoint ,MPoint& .
//RETURNS : NONE.
	void ConvertWGS84_CK42(MPoint wgs84,MPoint& ck42 );

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::ConvertCK42_WGS84(MPoint ck42, MPoint& wgs84 ).
//DESCRIPTION: Coordinate transformation between datums.
//INPUTS : MPoint , MPoint& .
//RETURNS : NONE.
	void ConvertCK42_WGS84(MPoint ck42, MPoint& wgs84 );

  //PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSelectedObjectInfo(MTopobject * mTpo).
//DESCRIPTION: Returns a list of object attributes mTpo.
//INPUTS : MTopobject pointer.
//RETURNS : QStringList.
	QStringList GetSelectedObjectInfo(MTopobject * mTpo);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetNauticalMiles(double dMetres).
//DESCRIPTION: Function of getting of nautical miles (NM) from metres.
//INPUTS : double.
//RETURNS : double.
	double GetNauticalMiles(double dMetres);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetMetresFromNauticalMiles(double dNauticalMiles).
//DESCRIPTION: Function of getting of metres from nautical miles (NM).
//INPUTS : double.
//RETURNS : double.
	double GetMetresFromNauticalMiles(double dNauticalMiles);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetFoots(double dMetres).
//DESCRIPTION: Function of getting of foots (Ft) from metres.
//INPUTS : double.
//RETURNS : double.
	double GetFoots(double dMetres);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetMetresFromFoots(double dFoots).
//DESCRIPTION: Function of getting of metres from foots (Ft).
//INPUTS : double.
//RETURNS : double.
	double GetMetresFromFoots(double dFoots);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetKnots(double dVelocity).
//DESCRIPTION: Function of getting of velocity in knots from velocity in metres/seconds.
//INPUTS : double.
//RETURNS : double.
	double GetKnots(double dVelocity);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetVelocityFromKnots(double dKnots).
//DESCRIPTION: Function of getting of velocity in metres/seconds from velocity in knots.
//INPUTS : double.
//RETURNS : double.
	double GetVelocityFromKnots(double dKnots);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetAccelerationKnots(double dAcceleration).
//DESCRIPTION: Function of getting of acceleration in knots/sec
//             from acceleration in metres/sec^2.
//INPUTS : double.
//RETURNS : double.
	double GetAccelerationKnots(double dAcceleration);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetAccelerationFromKnots(double dKnots).
//DESCRIPTION: Function of getting of acceleration in metres/sec^2
//             from acceleration in knots/sec.
//INPUTS : double.
//RETURNS : double.
	double GetAccelerationFromKnots(double dKnots);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSquareFoots(double dSqMetres).
//DESCRIPTION: Function of getting of square in ft^2 from  m^2.
//INPUTS : double.
//RETURNS : double.
	double GetSquareFoots(double dSqMetres);

//PACKAGE : MAP.
//FUNCTION: MKartaInterface::GetSqMetresFromSqFoots(double dSqFoots).
//DESCRIPTION: Function of getting of square in m^2 from ft^2 .
//INPUTS : double.
//RETURNS : double.
	double GetSqMetresFromSqFoots(double dSqFoots);
	
	//DMax - in meters
	void CalculateAngleClose(MPoint pointStand, double heightPhaseCenter, double DMax,
							std::map<int, std::map<int, double> > &mapAzimuthDistanceAngle,
							double discretAzimuth = 1, double discretRange = 0);

	bool GetAngleByAzimuthAndDistance(int azimuth, int distance,
									  std::map<int, std::map<int, double> > mapAngleClose,
									  double& angleClose);

	bool GetMapWithRangeAndAngleByAzimuth(int azimuth,
										  std::map<int, std::map<int, double> > mapAngleClose,
										  std::map<int, double>& mapRangeAngle);

	bool GetAngleByDistance(int distance, std::map<int, double > mapDistanceAngle,
							double& angleClose);

    ///=======================
    int GetScale();
    ///=======================

public:
	MKarta* m_karta;      //   Object map
	MRect m_zoombounds;   //   Zoom rectangle
	MObjectList* m_pFoundOblist;// list of objects found by attribute value

    bool DrawPixmapByUZN(const MString &_UZN, MKartaInterface *_pKI, QPixmap &_pmap);

    Q_INVOKABLE static QString latToString(double v);
    Q_INVOKABLE static QString lonToString(double v);
    Q_INVOKABLE QString coordinatesString(double lat, double lon, int coordinatesSystem);

    MPoint geoWgs84ToGaussSk42(MPoint& pt);
    MPoint gaussSk42ToGeoWgs84(MPoint& pt);

protected:

//	MKarta* tmp_karta;      // <

	void CreatePaintDevice();

	void CreatePaintDeviceSmall();

	bool CreateQtDevice();

	// Selects al objectswith number 'uid'
	void SelectObjectsWithUID(double uid);

	// Cancels selection of all objects with number 'uid'
	void DeselectObjectsWithUID(double uid);

	MKlassifikator* m_kls;				//  Object classifier
	MString			m_KlsPath;			//  path to classifier

	QWidget* m_widget;					//  widget for map srawing
	QWidget* m_widgetsmall;				//  widget for map drawing in a small window

	// QPaintDevice* is used for printer output
	// to calculate the size in millimeters
	QPaintDevice* m_PaintDevice;		//  Drawing device
	QPaintDevice* m_PaintDeviceSmall;	//  Drawing device for small map

	QPainter* m_Painter;				//  Drawer
	QPainter* m_PainterSmall;			//  Drawer for small map

	QPixmap * m_pixmap;					//  pixmap with map
	QPixmap * m_pixmapact;				//  pixmap with map and ground

	QPixmap * m_pixmapsmall;				//  pixmap with map for small window
	QPixmap * m_pixmapactsmall;			//  pixmap with map and ground for small window

	MPoint m_viewsize;					//  Window size(logical)
	MPoint m_viewsizesmall;				//  Window size(logical) for small map

	MLayer* m_actLayer;					//  Operational active layer
	MLayer* m_geoLayer;					//  Operational geographic layer
	MTopobject* m_Object;				//  Operational object

	double A,B1,B2;						//  Coefficients of transformation from paper to geo
	double C,D1,D2;						//  Transformation coefficients

	MString m_formularname;				//  Formular name of operational object

	MPoint m_reper;						//  Reper point

	MQtDevice* m_QtDevice;				//  Output device(screen)

	bool m_bDrawCooGrid;				//  Flag of coordinate grid drawing
    bool m_bDrawMGRSCooGrid;            //  Flag of mgrs grid drawing
    bool m_bDrawScaleGrid;				//  Flag of scale grid drawing
	
	bool m_pixmap_resizeFlag;			//  Flag 'Pixmap size measuring'
	
	struct SheetInfo					//  Additional map sheet:
	{
		MString name;					//  Map sheet name
		MRect rect;						//  Bounding rectangle
	};
	
	vector<SheetInfo> vect_sheet;		//array reloaded map sheets
 
private:
	friend class MRoute;

	//rendering route
    void RestorePixmap(QPixmap* pxmRestore);
    void DrawScaleGrid_old(MPlotDevice *pDevice, MRect &gridrect);
};

#endif // !defined(KARTAINTERFACE_H_)
