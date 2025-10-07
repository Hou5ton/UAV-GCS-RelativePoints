//PACKAGE : MAP.
//FILE: Karta.h    
//AUTHOR:
//DESCRIPTION:

#ifndef __NUXKARTA_H__
#define __NUXKARTA_H__
#include <qobject.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qprogressdialog.h> 
#include <qlayout.h>
#include <qstring.h>
#include <qdatetime.h>
#include <algorithm>
#include <list>
#include "Table.h"
#include "Relief.h"
#include "Klassifikator.h"
#include "fmt.h"
#include "MString.h"

//=======================================================================
// Strings constants - objects identifiers of service classifier

#if ENGVERSION == 1 // if English version
	#define IDS_OFFICIAL_KLS_SHRT "Service Classifier"
	#define IDS_OFFICIAL_KLS_FLL "Service Classifier For Internal Use"
	#define IDS_OFFICIAL_SIGNSYST_SHRT "Service Sign System"
	#define IDS_OFFICIAL_SIGNSYST_FLL "Service Sign System For Internal Use"
	#define IDS_OFFICIAL_PARTITION_SHRT "Service Partition"
	#define IDS_OFFICIAL_PARTITION_FLL "Service Partition For Internal Use"
	#define IDS_POINT_OBJ "Point Object"
	#define IDS_LINEAR_OBJ "Linear Object"
	#define IDS_AREA_OBJ "Areal Object"
	#define IDS_ALTITUDE_MARK "Altitude Mark"
	#define IDS_ISOLINE "Isoline"
	#define IDS_OBRYV "Steep"
	#define IDS_OTKOS "Slope"
	#define IDS_YAMA "Hole"
	#define IDS_PODPSTENKA_SHRT "Supporting Wall"
	#define IDS_PODPSTENKA_FLL "Supporting Wall"
	#define IDS_GIDRO "Hydrographical Object"

#else//Russian versian
	#define IDS_OFFICIAL_KLS_SHRT "Сл.классификатор"
	#define IDS_OFFICIAL_KLS_FLL "Служебный классификатор для внутреннего полъзования"
	#define IDS_OFFICIAL_SIGNSYST_SHRT "Сл.зн.сист."
	#define IDS_OFFICIAL_SIGNSYST_FLL "Служебная знаковая система для внутреннего полъзования"
	#define IDS_OFFICIAL_PARTITION_SHRT "Сл.раздел"
	#define IDS_OFFICIAL_PARTITION_FLL "Служебный раздел для внутреннего полъзования"
	#define IDS_POINT_OBJ "Точечный объект"
	#define IDS_LINEAR_OBJ "Линейный объект"
	#define IDS_AREA_OBJ "Площадной объект"
	#define IDS_ALTITUDE_MARK "Отметка высоты"
	#define IDS_ISOLINE "Изолиния"
	#define IDS_OBRYV "Обрыв"
	#define IDS_OTKOS "Откос"
	#define IDS_YAMA "Яма"
	#define IDS_PODPSTENKA_SHRT "Подп.стенка"
	#define IDS_PODPSTENKA_FLL "Подпорная стенка"
	#define IDS_GIDRO "Гидрообъект"
#endif
//==========================================

using namespace std;

//PACKAGE : MAP.
//FUNCTION : CreateLayerByType(MString IDLayer);
//DESCRIPTION : Function creates layer of requaired type.
//INPUTS : MString.
//RETURNS : MLayer pointer.
MLayer* CreateLayerByType(MString IDLayer);

//PACKAGE : MAP.
//FUNCTION : CreateObjectByType(MString type);
//DESCRIPTION : Creates object by it's identifier.
//INPUTS : MString.
//RETURNS : MObject pointer.
MObject* CreateObjectByType(MString type);

//PACKAGE : MAP.
//FUNCTION : CreateZnakByType(MString SType);
//DESCRIPTION : Creates sign by object identifier.
//INPUTS : MString.
//RETURNS : MZnak pointer.
MZnak* CreateZnakByType(MString SType);

//PACKAGE : MAP.
//FUNCTION : CreateArrayByType(MString strType);
//DESCRIPTION : Creates array by type.
//INPUTS : MString.
//RETURNS : MArray pointer.
MArray* CreateArrayByType(MString strType);

//PACKAGE : MAP.
//FUNCTION : SizeToReal(MTopobject* pObj,float fVal);
//DESCRIPTION : Converts size which storing in classifier to real size.
//INPUTS : MTopobject pointer,float .
//RETURNS : Double.
double SizeToReal(MTopobject* pObj,float fVal);

//PACKAGE : MAP.
//CLASS	: MKarta.
//DESCRIPTION : Main class which realizing functions of digital map
class MKarta
{
public:

//PACKAGE : MAP.
//FUNCTION : MKarta::MKarta();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MKarta();

//PACKAGE : MAP.
//FUNCTION : MKarta::FormServKlassifikator();
//DESCRIPTION :  Creates service classifier.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void FormServKlassifikator();

//PACKAGE : MAP.
//FUNCTION : MKarta::~MKarta();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MKarta();

public:
	// MAP PASSPORT
	// Semantic
	MString       m_name;       // < Map name
	MString	      m_ieroname;	// < hierarchic Map name
	MString       m_sort;       // < map type
	MString	      m_vedomstvo;	// < department
	MString	      m_standart;	// < type of standart
	MString       m_strana;     // < producing country
	MString	      m_istochnik;	// < source
	MString       m_autor;      // < author
	MString	      m_comment;	// < comment

    // map segmentation
    MString       m_razgrafka;  // < map segmentation
	MString		  m_grif;		// < nomenclature in letters
	MString       m_grifzif;	// < numberic nomenclature
	MString		  m_griftop;	// < nomenclature of next sheet from up
	MString		  m_grifleft;	// < nomenclature of next sheet from left
	MString       m_grifbottom;	// < nomenclature of next sheet from down
	MString		  m_grifright;	// < nomenclature of next sheet from right
	MString		  m_nametop;	// < name of next sheet from up
	MString		  m_nameleft;	// < name of next sheet from left
	MString       m_namebottom;	// < name of next sheet from down
	MString		  m_nameright;	// < name of next sheet from right

	// Time of:
	MString		  m_survay;		// < shooting
	MString		  m_dopsurvay;	// < additional shooting (correction)
	MString		  m_sostgod;	// < condition at ... year
	MString		  m_izdanie;	// < date of birth

	// Coordinate system
	MString       m_progectia;		// < projection
	MString		  m_coosyst;		// < coordinate system
	MString       m_ellipsoid;		// < ellipsoid
	MString		  m_edinitsa;		// < unit of the map
	MString		  m_polusharieWE;	// < west-east hemisphere
	MString       m_polusharieNS;	// < south-north hemisphere
	MString		  m_geodezkomment;	// < comment
	MString		  m_magnsklon;		// < magnetic declination
	MString		  m_parallel1;		// < first main parallel
	MString       m_parallel2;		// < second main parallel
	MString		  m_osmeridian;		// < axled meridian
	MString       m_parallelgl;		// < parallel of main point

	// System of altitudes
	MString       m_hsyst;			// < system of altitudes
	MString		  m_hedinitsa;		// < unit of system of altitudes
	MBmhFILE*	  m_bmhfile;		// < file of altitude matrix
	MBmhFILE*	  m_bmhobjfile;		// < file of altitude matrix of objects
	MBmhFILE*	  m_bmhcorfile;		// < file of matrix of correction
	double		  m_hmin;			// < min altitude
	double		  m_hmax;			// < max altitude
	double		  m_hsek;			// < relief section
	double        m_hminizo;		// < min isohypse
	double		  m_hmaxizo;		// < max isohypse

	// scale
	MString       m_mastab;			// < scale

	// Frame
	MString       m_ramstyle;		// < frame style
	MPtArray	  m_contur;			// < exaxt contour of frame

	MString		  m_password;		// <  password

	// the map
	MLayerList    m_laylist;		// < list of layers
    MLayerList    m_actlaylist;		// < list of active layers

	MRect         m_geobounds;		// < bounding georectangle in conventional units

	// paper attributes
	unsigned char m_papercolorkod;	// <  background color code - non saving parameter
	MColor m_papercolor;			// <  background color - non saving parameter

	// variables for model
	MRect m_maketlist;				// < list of model

	// variables for conversion points to geo-model
	MPoint m_geopt1;
	MPoint m_geopt2;
	MPoint m_geopt3;
	MPoint m_maketpt1;
	MPoint m_maketpt2;
	MPoint m_maketpt3;
	bool   m_ismaket;		// < flag, which defines view of data(true - model, false - geo)

	// Classifier members
	MKlassifikator* m_pKls;   // < Pointer to classifier
	MTermin  m_klstermin;     // < term of classifier

	// Service classifier (non saving, used for operations of import-export)
	MKlassifikator* m_pServKls;	// <  Pointer to classifier
	MZnak*	m_pDotZnak;			// <  Pointer to dotted sign of service classifier
	MZnak*	m_pLinZnak;			// <  Pointer to linear sign of service classifier
	MZnak*	m_pSqrZnak;			// <  Pointer to areal sign of service classifier
	MZnak*	m_pHptZnak;			// <  Pointer to altitude mark of service classifier
	MZnak*	m_pIzoZnak;			// <  Pointer to isoline of service classifier
	MZnak*	m_pObrZnak;			// <  Pointer to steep of service classifier
	MZnak*	m_pOtkZnak;			// <  Pointer to slope of service classifier
	MZnak*	m_pYamZnak;			// <  Pointer to hole of service classifier
	MZnak*	m_pPstZnak;			// <  Pointer to supporting wall of service classifier
	MZnak*	m_pGdrZnak;			// <  Pointer to hydroobject of service classifier

	//  for color displacement by parameters:
	int m_DH; // <saturateon
	int m_DL; // <brightness
	int m_DS; // <hue

	// Background rendering operational layers
	bool m_actBackEnable;
	int m_actBackLight;
	int m_actBackTransp;

    MCoo* m_pObjectCoo = nullptr;                     // < current coordinate system
    MCoo* m_pOldObjectCoo = nullptr;		// < previous coordinate system
    bool b_converted = false;               // true - if it has been converted

	MString m_matrixfile;		// < altitude matrix file
    MString m_matrixExtraFile;  // <extra altitude matrix file  //veronika 25.04.2016
	int percent;				// < the percentage of completion of the process 
						//for indicating progress dlitelnyh utilities
	int startMsec;
	//int m_progress;

	MStArray m_ShNameArray;
	MStArray m_FilterArray;

	int m_DrawFilterSizeLog;
	double m_DrawFilterSizeGeo;

// 	int m_TextSizeMaxLog;		// < max size of inscriptions in pixels
	list<MObject*> m_SelecedList; // <list of selecteed objects
        
	QWidget* pMainMapWidget;  // <main widget for map output

	MLayerDrawFilter *layerDrawFilter;

    QProgressDialog *progressDLG_1{nullptr};
    QLabel          *progressLAB_1{nullptr};

public:

//PACKAGE : MAP.
//FUNCTION : GetGeobounds();
//DESCRIPTION : Functions "get".
//INPUTS : NONE.
//RETURNS : MRect.
	MRect	      GetGeobounds(){ return m_geobounds; }

//PACKAGE : MAP.
//FUNCTION : AddLayer(MLayer* pLay = NULL);
//DESCRIPTION : Add layer.
//INPUTS : MLayer pointer.
//RETURNS : NONE.
	void          AddLayer(MLayer* pLay = NULL);

//PACKAGE : MAP.
//FUNCTION : FindLayer(MString key);
//DESCRIPTION : Find layer.
//INPUTS : MString.
//RETURNS : MLayer pointer.
	MLayer*       FindLayer(MString key);

//PACKAGE : MAP.
//FUNCTION : MoveLayerDown(MLayer* pLay);
//DESCRIPTION : Move layer down.
//INPUTS : MLayer pointer.
//RETURNS : True.
	bool          MoveLayerDown(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : MoveLayerDown(MLayer* pLay);
//DESCRIPTION : Move layer up.
//INPUTS : MLayer pointer.
//RETURNS : True.
	bool          MoveLayerUp(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : MoveLayerDown(MLayer* pLay);
//DESCRIPTION : Remove layer.
//INPUTS : MLayer pointer.
//RETURNS : True.
	bool          RemoveLayer(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : GetScale(MPlotDevice* pDevice, MRect& drawBounds);
//DESCRIPTION : 
//INPUTS : MPlotDevice pointer, MRect& .
//RETURNS : Double.
	double GetScale(MPlotDevice* pDevice, MRect& drawBounds);
	
//PACKAGE : MAP.
//FUNCTION : ExchangeLayers(int m_iFirst,int m_iSecond);
//DESCRIPTION : replacement of layers with places.
//INPUTS : int ,int .
//RETURNS : NONE.
	void ExchangeLayers(int m_iFirst,int m_iSecond);

//PACKAGE : MAP.
//FUNCTION : LayerheadDelete();
//DESCRIPTION : Deletes lower layer.
//INPUTS : NONE.
//RETURNS : NONE.
	void		  LayerheadDelete();

//PACKAGE : MAP.
//FUNCTION : AddActLayer(MLayer* pLay = NULL);
//DESCRIPTION :  Addition of active layer.
//INPUTS : MLayer pointer.
//RETURNS : NONE.
	void          AddActLayer(MLayer* pLay = NULL);

//PACKAGE : MAP.
//FUNCTION : FindActLayer(double uid);
//DESCRIPTION : Searchs for active layer by key.
//INPUTS : Double.
//RETURNS : MLayer pointer.
    MLayer*       FindActLayer(double uid);

//PACKAGE : MAP.
//FUNCTION : FindObjectAtActLayers(double uid);
//DESCRIPTION : Searchs for object by all operative layers.
//INPUTS : Double.
//RETURNS : True.
	bool          FindObjectAtActLayers(double uid);

//PACKAGE : MAP.
//FUNCTION :  RemoveActLayer(MLayer* pLay);
//DESCRIPTION : Deletes active layer.
//INPUTS : MLayer pointer.
//RETURNS : True.
	bool          RemoveActLayer(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : MoveActLayerDown(MLayer* pLay);
//DESCRIPTION : Moves active layer on one step downward.
//INPUTS : MLayer pointer.
//RETURNS : True.
	bool          MoveActLayerDown(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION :  MoveActLayerUp(MLayer* pLay);
//DESCRIPTION :  Moves active layer on one step upward.
//INPUTS : MLayer pointer.
//RETURNS : True.
	bool          MoveActLayerUp(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : ExchangeActLayers(int m_iFirst,int m_iSecond);
//DESCRIPTION : Replaces two active layers in document.
//INPUTS : int ,int .
//RETURNS : NONE.
	void          ExchangeActLayers(int m_iFirst,int m_iSecond);

//PACKAGE : MAP.
//FUNCTION : ActLayerheadDelete();
//DESCRIPTION : Deletes lowerest active layer.
//INPUTS : NONE.
//RETURNS : NONE.
	void	      ActLayerheadDelete();

//PACKAGE : MAP.
//FUNCTION : Remove(MObject* pObj);
//DESCRIPTION : Deletes object.
//INPUTS : MObject pointer.
//RETURNS : NONE.
	void		  Remove(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION :  RemoveActor(MObject* pObj);
//DESCRIPTION : Deletes active object.
//INPUTS : MObject pointer.
//RETURNS : NONE.
	void		  RemoveActor(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION : RemoveActor(double uid);
//DESCRIPTION : Deletes active object.
//INPUTS : Double.
//RETURNS : NONE.
	void		  RemoveActor(double uid);  

//PACKAGE : MAP.
//FUNCTION : ClearAll();
//DESCRIPTION : Remove layer.
//INPUTS : MLayer pointer.
//RETURNS : True.
	void      ClearAll();

//PACKAGE : MAP.
//FUNCTION : FindOblistInLayerByAttrValue(MString m_parLayerName,MString m_parAttrName, MString m_parValue);
//DESCRIPTION : Searching for objects in definite layer (m_parLayerName)
//				by definite value (m_parValue)
//				of given attribute (m_parAttrName)
//				returns as a result a list of all objects that satisfy the input requirements.
//INPUTS : MLayer pointer.
//RETURNS : True.
	MObjectList*  FindOblistInLayerByAttrValue(MString m_parLayerName,MString m_parAttrName, MString m_parValue);

//PACKAGE : MAP.
//FUNCTION : FindKvadrokodesInLayerByObjType(MString m_LayerName, int iObjType);
//DESCRIPTION : Searching for quadrocodes of objects of definite type (iObjType) in definite layer (m_LayerName)
//				object type is defined by constant - (F_PLO, F_DOT, F_LIN [Yaz.h]), layer is defined by name
//				returns a list of found quadrocodes.
//INPUTS : MString , int .
//RETURNS : QStringList.
	QStringList FindKvadrokodesInLayerByObjType(MString m_LayerName, int iObjType);

//PACKAGE : MAP.
//FUNCTION : GetKlsTermin(char * filepath, MTermin& termin);
//DESCRIPTION : Reads term of map classifier from UTP-format file.
//INPUTS : char pointer, MTermin& .
//RETURNS : True.	
	bool GetKlsTermin(char * filepath, MTermin& termin);

//PACKAGE : MAP.
//FUNCTION : OpenFileForReading(const char * filepath);
//DESCRIPTION : Opens map file for reading of main UTP-format.
//INPUTS : const char pointer.
//RETURNS : FILE pointer.
	FILE* OpenFileForReading(const char * filepath);

//PACKAGE : MAP.
//FUNCTION : ReadBin(FILE* h_nuxfile, MKlassifikator* pKls);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer, MKlassifikator pointer.
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, MKlassifikator* pKls);

//PACKAGE : MAP.
//FUNCTION : ReadBin(MString& name, MKlassifikator* pKls);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : MString& , MKlassifikator pointer.
//RETURNS : True.
	virtual bool ReadBin(MString& name, MKlassifikator* pKls);

//PACKAGE : MAP.
//FUNCTION : OpenFileForWriting(char * filepath);
//DESCRIPTION : Opens map file for ssaving of main format UTP.
//INPUTS : char pointer.
//RETURNS : FILE pointer.
	FILE* OpenFileForWriting(char * filepath);

//PACKAGE : MAP.
//FUNCTION : WriteBin(FILE* h_nuxfile);
//DESCRIPTION : Saves to UTP-format file.
//INPUTS : FILE pointer.
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile);

//PACKAGE : MAP.
//FUNCTION : CloseFile(FILE* h_nuxfile);
//DESCRIPTION : Close opened file.
//INPUTS : FILE pointer.
//RETURNS : NONE.
	void CloseFile(FILE* h_nuxfile);

//PACKAGE : MAP.
//FUNCTION : ConnectKlassifikator(MKlassifikator* pKls);
//DESCRIPTION : Connect to another classifier.
//INPUTS : MKlassifikator pointer.
//RETURNS : NONE.
	void ConnectKlassifikator(MKlassifikator* pKls);

//PACKAGE : MAP.
//FUNCTION : GetPaperColor(bool HLS= true);
//DESCRIPTION : Returns background color of the map.
//INPUTS : bool.
//RETURNS : MColor.
	MColor      GetPaperColor(bool HLS= true);

//PACKAGE : MAP.
//FUNCTION : SetPaperColor(MColor& col);
//DESCRIPTION : Sets background color of the map.
//INPUTS : MColor& .
//RETURNS : NONE.
	void		SetPaperColor(MColor& col);


	// Functions of table handling of classifier which attached to the map

	// Table of colors
	bool GetDictColorName(unsigned short int ushIndex,MString& strName);
	bool GetDictColorValue(unsigned short int ushIndex,MColor& colorValue, bool HLS= true);
	bool SetDictColorValue(unsigned short int ushIndex,MColor colorValue);
	bool SetDictColorName(unsigned short int ushIndex,MString strName);
	bool AddDictColor(MColor clrrfValue, short& ushKey,MString strName="");

	// Table of handwrites
	bool GetDictPrkName(unsigned short int ushIndex,MString& strName);
	bool GetDictPrkValue(unsigned short int ushIndex,MPrk& prkValue);
	bool SetDictPrkValue(unsigned short int ushIndex,MPrk* prkValue);
	bool SetDictPrkName(unsigned short int ushIndex,MString strName);
	bool AddDictPrk(MPrk* prkValue, short& ushKey,MString strName="");

	// Table of notions
	bool GetDictAttrName(unsigned short int ushIndex,MString& strName);
	bool GetDictAttrEdIzm(unsigned short int ushIndex,MString& edIzm);
	bool GetDictAttrType(unsigned short int ushIndex,MString& type);
	bool SetDictAttrName(unsigned short int ushIndex,MString strName);
	bool SetDictAttrEdIzm(unsigned short int ushIndex,MString edIzm);
	bool SetDictAttrType(unsigned short int ushIndex,MString type);
	bool AddDictAttr(MString strName, short& ushKey,MString strType = "",MString strEdIzm = "");

        //Table image
	bool GetDictImageName(unsigned short int ushIndex,MString& strName);
	bool GetDictImageValue(unsigned short int ushIndex,MImage &image);
	bool SetDictImageName(unsigned short int ushIndex,MString strName);
	bool SetDictImageValue(unsigned short int ushIndex,MImage *image);
	bool AddDictImage(MString strName, short& ushKey, MImage *image);

	//Table font
	bool GetDictFontName(unsigned short int ushIndex,MString& strName);
	bool GetDictFontValue(unsigned short int ushIndex,M_LOG_FONT& colorValue);
	bool SetDictFontValue(unsigned short int ushIndex,M_LOG_FONT lfValue);
	bool SetDictFontName(unsigned short int ushIndex,MString strName);
	bool AddDictFont(M_LOG_FONT lfValue, short& ushKey,MString strName="");

	// Table of suffix&preffix strings
	bool GetDictMegstrName(unsigned short int ushIndex,MString& strName);
	bool GetDictMegstrValue(unsigned short int ushIndex,MString& colorValue);
	bool SetDictMegstrValue(unsigned short int ushIndex,MString megstrValue);
	bool SetDictMegstrName(unsigned short int ushIndex,MString strName);
	bool AddDictMegstr(MString megstrValue, short& ushKey,MString strName="");

	// Following functions must be removed :
	bool GetColorDictNumValue(MColor clrValue,unsigned short int& ExistKey);
	bool GetPrkDictNumValue(MPrk *clrValue,unsigned short int& ExistKey);
	bool GetAttrDictNumValue(MString clrValue,unsigned short int& ExistKey);
	bool GetFontDictNumValue(M_LOG_FONT clrValue,unsigned short int& ExistKey);
	bool GetMegStrDictNumValue(MString clrValue,unsigned short int& ExistKey);

	// FUNCTION OF COLOR REPLACEMENT OF HANDWRITE OF SUFFIX&PREFFIX STRING AND CHARACTERISTIC IN OBJECTS OF THE DOCUMENT
	bool ChangeColor(unsigned char oldcolorkod,unsigned char newcolorkod);
	bool ChangePocherk(unsigned char oldprkkod,unsigned char newprkkod);
	bool ChangeMegaStroka(unsigned char oldmegastrkod, unsigned char newmegastrkod);
	bool ChangeCharakteristika(unsigned short oldcharkod, unsigned short newcharkod);

//PACKAGE : MAP.
//FUNCTION : DotShriftZnakCenterAlign();
//DESCRIPTION : Aligns all dotted script signs by centre.
//INPUTS : NONE.
//RETURNS : True.
	bool DotShriftZnakCenterAlign();

//PACKAGE : MAP.
//FUNCTION : MinimizeGeobounds();
//DESCRIPTION : Minimizes map boundaries.
//INPUTS : NONE.
//RETURNS : NONE.
	void          MinimizeGeobounds();

//PACKAGE : MAP.
//FUNCTION : Minimizelayerbounds();
//DESCRIPTION : Minimizes layers boundaries.
//INPUTS : NONE.
//RETURNS : NONE.
	void		  Minimizelayerbounds();

//PACKAGE : MAP.
//FUNCTION : GetFullListOfMapObjects();
//DESCRIPTION : Creates full list of all objects of the map.
//INPUTS : NONE.
//RETURNS : MObjectList pointer.
	virtual MObjectList* GetFullListOfMapObjects();

//PACKAGE : MAP.
//FUNCTION : GetObjectListByQuadrakod(MObjectList* oblist,MString quadrakod);
//DESCRIPTION : Creates list of objects of following quadrant.
//INPUTS : MObjectList pointer,MString .
//RETURNS : NONE.
	void GetObjectListByQuadrakod(MObjectList* oblist,MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : DeleteFormulaPrivs(int formnomer, MString quadrakod);
//DESCRIPTION : Deletes vectors and splines of formula snaps with bumber 'formnomer'
// 		from objects with following quadrant.
//INPUTS : int , MString .
//RETURNS : True.
	bool DeleteFormulaPrivs(int formnomer, MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : ChangeFormulaNomer(int oldformnom, int newformnom, MString quadrakod);
//DESCRIPTION : Changes number of formula 'oldformnom' for objects with defined quadrant in vectors
// 		and splines arrays of formulas snaps on number 'newformnom'.
//INPUTS : int , int , MString .
//RETURNS : True.
	bool ChangeFormulaNomer(int oldformnom, int newformnom, MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : GetObjectCount();
//DESCRIPTION : Gets quantity of objects of the map.
//INPUTS : NONE.
//RETURNS : int.
	int  GetObjectCount();

//PACKAGE : MAP.
//FUNCTION : FindNearestObject(MPoint findpt);
//DESCRIPTION : Searchs for object of the map which is nearest to defined point.
//INPUTS : MPoint.
//RETURNS : MObject pointer.
	MObject*	  FindNearestObject(MPoint findpt);

//PACKAGE : MAP.
//FUNCTION : FindNearestRlfPoint(MPoint pt, double dopusk, double* rasst, MPoint* heightpt);
//DESCRIPTION : Returns distance from point 'pt' to nearest mark of altitude by all layers of relief only if
// 				it is not bigger than tolerance 'dopusk'.
//INPUTS : MPoint , double , double pointer, MPoint pointer.
//RETURNS : True.
	bool FindNearestRlfPoint(MPoint pt, double dopusk, double* rasst, MPoint* heightpt);

//PACKAGE : MAP.
//FUNCTION : GetHeightFromRelief(MPoint pt);
//DESCRIPTION : Gets altitude of point by all layers of relief.
//INPUTS : MPoint.
//RETURNS : Double.
	double GetHeightFromRelief(MPoint pt);
 
//PACKAGE : MAP.
//FUNCTION : GetHeight(const MPoint& pt, MBmhFILE *file);
//DESCRIPTION : Get height.
//INPUTS : const MPoint& pt, MBmhFILE pointer.
//RETURNS : int. 
    int GetHeight(const MPoint& pt, MBmhFILE *file) const;

//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrix(const MPoint& pt);
//DESCRIPTION : Get height of DEM.
//INPUTS : const MPoint& .
//RETURNS : int. 
    int GetHeightFromMatrix(const MPoint& pt) const;
 
//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrix(const MPoint& pt, const MString &path);
//DESCRIPTION : Get height of DEM.
//INPUTS : const MPoint& , const MString &.
//RETURNS : int. 
   int GetHeightFromMatrix(const MPoint& pt, const MString &path);

//PACKAGE : MAP.
//FUNCTION : GetHeightRowFromMatrix(const MPoint& pt, MInArray* values, int count);
//DESCRIPTION : Getting a number of altitudes DEM
// 				pt - Starting point , values ??- the values ??of the output array ( created from the outside )
// 				count - the number of values ??that need to get
// 				returns the actual number of read values.
//INPUTS :const MPoint& , MInArray pointer, int .
//RETURNS : int.
    int GetHeightRowFromMatrix(const MPoint& pt, MInArray* values, int count);

//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixObject(const MPoint& pt);
//DESCRIPTION : Get height of DEM facilities.
//INPUTS : MPoint&.
//RETURNS : int.
    int GetHeightFromMatrixObject(const MPoint& pt);

//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixObject(const MPoint& pt,  const MString &path);
//DESCRIPTION : Get height of DEM facilities.
//INPUTS : const MPoint& ,  const MString &.
//RETURNS : int.
    int GetHeightFromMatrixObject(const MPoint& pt,  const MString &path);

//PACKAGE : MAP.
//FUNCTION : GetHeightRowFromMatrixObject(const MPoint& pt,MInArray* values, int count);
//DESCRIPTION : Getting a number of altitudes DEM facilities
// 				(see . GetHeightRowFromMatrix ()).
//INPUTS : const MPoint& ,MInArray pointer, int .
//RETURNS : int.	
    int GetHeightRowFromMatrixObject(const MPoint& pt,MInArray* values, int count);

//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixCorrection(const MPoint& pt);
//DESCRIPTION : Getting a number of altitudes DEM facilities
// 				(see . GetHeightRowFromMatrix ()).
//INPUTS : MPoint&.
//RETURNS : int.
    int GetHeightFromMatrixCorrection(const MPoint& pt);
 
//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixCorrection(const MPoint& pt,  const MString &path).
//DESCRIPTION : Getting a number of altitudes DEM facilities
// 				(see . GetHeightRowFromMatrix ()).
//INPUTS : const MPoint& ,  const MString &.
//RETURNS : int.
   int GetHeightFromMatrixCorrection(const MPoint& pt,  const MString &path);

//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixes(const MPoint& pt, MString path1, MString path2, double* hRel = NULL, double* hObj = NULL);
//DESCRIPTION : Get height of DEM.
//INPUTS : const MPoint& , MString , MString , double pointer, double pointer.
//RETURNS : int.
    int GetHeightFromMatrixes(const MPoint& pt, MString path1, MString path2, double* hRel = NULL, double* hObj = NULL);
 
//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixes(const MPoint& pt, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);
//DESCRIPTION : Get height of DEM.
//INPUTS : const MPoint& , double pointer, double pointer , double pointer.
//RETURNS : int.
   int GetHeightFromMatrixes(const MPoint& pt, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);
 
//PACKAGE : MAP.
//FUNCTION : GetHeightFromMatrixes(const MPoint& pt, MString path1, MString path2, MString path3, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);
//DESCRIPTION : Get height of DEM.
//INPUTS : const MPoint& , MString , MString , MString , double pointer, double pointer , double pointer.
//RETURNS : int.
   int GetHeightFromMatrixes(const MPoint& pt, MString path1, MString path2, MString path3, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);

//PACKAGE : MAP.
//FUNCTION : SetMatrixFile(MString path);
//DESCRIPTION : Sets path to altitude matrix file.
//INPUTS : MString.
//RETURNS : True.
	bool SetMatrixFile(MString path);

//PACKAGE : MAP.
//FUNCTION : SetMatrixObjectFile(MString path);
//DESCRIPTION : Sets path to altitude matrix of objects file.
//INPUTS : MString.
//RETURNS : True.
	bool SetMatrixObjectFile(MString path);

//PACKAGE : MAP.
//FUNCTION : SetMatrixCorrectionFile(MString path);
//DESCRIPTION : Sets path to correction matrix file.
//INPUTS : MString.
//RETURNS : True.
	 bool SetMatrixCorrectionFile(MString path);

//PACKAGE : MAP.
//FUNCTION : GetHeightFromReliefByIzolines(MPoint pt);
//DESCRIPTION : Gets altitude of point by all layers of relief.
//INPUTS : MPoint.
//RETURNS : Double.
	double GetHeightFromReliefByIzolines(MPoint pt);

//PACKAGE : MAP.
//FUNCTION : GetProfileInfoByIzolines(MPtArray* pLomLine, MPtArray* ProfInfo, double dopusk=0);
//DESCRIPTION : Gets informationf for profile creation by cutting line by all layers.
//INPUTS : MPtArray pointer, MPtArray pointer, double .
//RETURNS : True.
	bool		  GetProfileInfoByIzolines(MPtArray* pLomLine, MPtArray* ProfInfo, double dopusk=0);

//PACKAGE : MAP.
//FUNCTION : FindCrosslinesWithGidro(MPtArray* plankontur, MPtArray* Gidrocoord, MPtArray* Gidroheight);
//DESCRIPTION : Finds all intersections of gas pipeline with hydroobjects by all layers of relief.
//INPUTS : MPtArray pointer, MPtArray pointer, MPtArray pointer.
//RETURNS : True.
	bool		  FindCrosslinesWithGidro(MPtArray* plankontur, MPtArray* Gidrocoord, MPtArray* Gidroheight);

//PACKAGE : MAP.
//FUNCTION : FindCrosslinesWithRoads(MPtArray* plankontur, MStArray* kodar, double dopusk, MVoidArray* pRoadObjects, MPtArray* Roadcoord, MPtArray* Roadheight);
//DESCRIPTION : Finds all intersections of gas pipeline with roads by all layers of relief.
//INPUTS : MPtArray pointer, MStArray pointer, double , MVoidArray pointer, MPtArray pointer, MPtArray pointer.
//RETURNS : True.
	bool		  FindCrosslinesWithRoads(MPtArray* plankontur, MStArray* kodar, double dopusk,
						MVoidArray* pRoadObjects, MPtArray* Roadcoord, MPtArray* Roadheight);

//PACKAGE : MAP.
//FUNCTION : StartProgress(int fileCount=1);
//DESCRIPTION : Initializing Progress.
//INPUTS : int.
//RETURNS : NONE.
	void StartProgress(int fileCount=1); //(); //when WINDOWS

//PACKAGE : MAP.
//FUNCTION : StopProgress();
//DESCRIPTION : Stopping progress.
//INPUTS : NONE.
//RETURNS : NONE.
	void StopProgress();
//	void CreateProgress(QWidget *parent, int cnt=1);

//PACKAGE : MAP.
//FUNCTION : SetProgress(QString filename);
//DESCRIPTION : Sets progress index.
//INPUTS : QString.
//RETURNS : NONE.
	void SetProgress(QString filename);

//PACKAGE : MAP.
//FUNCTION : SetProgress(int percent, int fileIndex=0);
//DESCRIPTION : Sets progress index.
//INPUTS : int , int .
//RETURNS : NONE.
	void SetProgress(int percent, int fileIndex=0);//, int fileCount=0); //when WINDOWS

//PACKAGE : MAP.
//FUNCTION : ShowProgress();
//DESCRIPTION : Calls progress index.
//INPUTS : NONE.
//RETURNS : NONE.
	void ShowProgress();
	//void HideProgress();//when WINDOWS

//PACKAGE : MAP.
//FUNCTION : Risovanie(MPlotDevice* pDevice, bool bDrawSmall = false);
//DESCRIPTION : Redraws map.
//INPUTS : MPlotDevice pointer, bool .
//RETURNS : NONE.
    void Risovanie(MPlotDevice* pDevice, int scale, bool bDrawSmall = false);

//PACKAGE : MAP.
//FUNCTION : RisovanieAct(MPlotDevice* pDevice, bool bDrawSmall = false);
//DESCRIPTION : Redraws active environment.
//INPUTS : MPlotDevice pointer, bool .
//RETURNS : NONE.
	void RisovanieAct(MPlotDevice* pDevice, bool bDrawSmall = false);

//PACKAGE : MAP.
//FUNCTION : DrawLayers(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);
//DESCRIPTION : Draws geographic layers.
//INPUTS : MPlotDevice pointer, MRect& , bool .
//RETURNS : NONE.
    void DrawLayers(MPlotDevice* pDevice, MRect& drawBounds, int scale, bool bDrawSmall = false);

//PACKAGE : MAP.
//FUNCTION : DrawActBackground(MPlotDevice* pDevice);
//DESCRIPTION : Background rendering operational layers.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	void DrawActBackground(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : DrawActLayersText(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);
//DESCRIPTION : Text rendering operational layers.
//INPUTS : MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall.
//RETURNS : NONE.
	void DrawActLayersText(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);

//PACKAGE : MAP.
//FUNCTION : DrawLayersText(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);
//DESCRIPTION :  Draws text of geographic layers.
//INPUTS : MPlotDevice pointer, MRect& , bool .
//RETURNS : NONE.
	void DrawLayersText(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);

//PACKAGE : MAP.
//FUNCTION : DrawActLayers(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);
//DESCRIPTION : Draws text of active layers.
//INPUTS : MPlotDevice pointer, MRect& , bool .
//RETURNS : NONE.
	void DrawActLayers(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall = false);

//PACKAGE : MAP.
//FUNCTION : DrawSelectedObj(MPlotDevice* pDevice, MRect& drawBounds);
//DESCRIPTION : Draws selected objects.
//INPUTS : MPlotDevice pointer, MRect& .
//RETURNS : NONE.
	void DrawSelectedObj(MPlotDevice* pDevice, MRect& drawBounds);

//PACKAGE : MAP.
//FUNCTION : ShapefileRead(MString filename);
//DESCRIPTION : Reads Shape-file.
//INPUTS : MString.
//RETURNS : True.
	bool      ShapefileRead(MString filename);

//PACKAGE : MAP.
//FUNCTION : MifFileRead(MString filename);
//DESCRIPTION : Reads mif-file.
//INPUTS : MString.
//RETURNS : True.
	bool      MifFileRead(MString filename);

//PACKAGE : MAP.
//FUNCTION : ShapefileWrite(MLayer* pLay, int type, MString str, MInArray* dlinaarr,  MStArray* names);
//DESCRIPTION : Saves to Shape-file.
//INPUTS : MLayer pointer, int , MString , MInArray pointer,  MStArray pointer.
//RETURNS : True.
	bool      ShapefileWrite(MLayer* pLay, int type, MString str, MInArray* dlinaarr,  MStArray* names);

//PACKAGE : MAP.
//FUNCTION :  WriteKarta(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves map.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool	  WriteKarta(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : ReadKarta(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads map.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool	  ReadKarta(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : ReadKarta(MString& filename, int version);
//DESCRIPTION : Reads map.
//INPUTS : MString& , int .
//RETURNS : True.
	bool	  ReadKarta(MString& filename, int version);

//PACKAGE : MAP.
//FUNCTION : Copy(MKarta* pKarta);
//DESCRIPTION : Function of copying.
//INPUTS : MKarta pointer.
//RETURNS : NONE.
	void    Copy(MKarta* pKarta);

	// next functions are used in LINUX (and Sample)

//PACKAGE : MAP.
//FUNCTION : AddSheet(MString& filename, int version = 0);
//DESCRIPTION : Adds map sheet.
//INPUTS : MString& , int .
//RETURNS : True.
	bool AddSheet(MString& filename, int version = 0);

//PACKAGE : MAP.
//FUNCTION : AddKarta(MKarta& karta);
//DESCRIPTION : Adds map.
//INPUTS : MKarta&.
//RETURNS : True.
	bool AddKarta(MKarta& karta);

//PACKAGE : MAP.
//FUNCTION : DeleteSheet(MString& name);
//DESCRIPTION : Deletes map sheet.
//INPUTS : MString&.
//RETURNS : True.
	bool DeleteSheet(MString& name);

//PACKAGE : MAP.
//FUNCTION : GetGeoBounds();
//DESCRIPTION : Returns geographic rectangle of the map.
//INPUTS : NONE.
//RETURNS : MRect.
	MRect GetGeoBounds();

//PACKAGE : MAP.
//FUNCTION : SetGeoBounds(MRect& rect);
//DESCRIPTION : Sets geographic rectangle of the map.
//INPUTS : MRect&.
//RETURNS : NONE.
	void SetGeoBounds(MRect& rect);

//PACKAGE : MAP.
//FUNCTION : SetLoadFilterFile(MString path);
//DESCRIPTION : Sets file with filter for quadrant selection during loading.
//INPUTS : MString.
//RETURNS : True.
	bool SetLoadFilterFile(MString path);

//PACKAGE : MAP.
//FUNCTION : FindKkodInLoadFilter(MString kkod);
//DESCRIPTION : Searchs for quadrocode in filter list of loading.
//INPUTS : MString.
//RETURNS : True.
	bool FindKkodInLoadFilter(MString kkod);

//PACKAGE : MAP.
//FUNCTION : SetDrawFilterSize(int size);
//DESCRIPTION : Sets min size of representative symbol
//				in logil units (pixels).
//INPUTS : int.
//RETURNS : NONE.
	void SetDrawFilterSize(int size) {m_DrawFilterSizeLog = size;}

//PACKAGE : MAP.
//FUNCTION : GetDrawFilterSize();
//DESCRIPTION : Gets min size of representative symbol
// 				in logil units (pixels).
//INPUTS : NONE.
//RETURNS : int .
	int  GetDrawFilterSize() {return m_DrawFilterSizeLog;}

//PACKAGE : MAP.
//FUNCTION : SetTextSizeMax(int size);
//DESCRIPTION : Sets max size of representative text.
//INPUTS : int.
//RETURNS : NONE.
	void SetTextSizeMax(int size);

//PACKAGE : MAP.
//FUNCTION : GetTextSizeMax();
//DESCRIPTION : Gets max size of representative text.
//INPUTS : NONE.
//RETURNS : int.
	int  GetTextSizeMax();

//PACKAGE : MAP.
//FUNCTION : SetTextSizeMin(int size);
//DESCRIPTION : Sets min size of visible text.
//INPUTS : int.
//RETURNS : NONE.
    void   SetTextSizeMin(int size);

//PACKAGE : MAP.
//FUNCTION : GetTextSizeMin();
//DESCRIPTION : Gets min size of visible text.
//INPUTS : NONE.
//RETURNS : int.
	int    GetTextSizeMin();

//PACKAGE : MAP.
//FUNCTION : GetRumb(MPoint pt1, MPoint pt2, double poluos1 = 6378245, double poluos2 = 6356862.02);
//DESCRIPTION : Used for azimuth calculation between two points of the map.
//INPUTS : MPoint , MPoint , double , double .
//RETURNS : Double.
	double GetRumb(MPoint pt1, MPoint pt2, double poluos1 = 6378245, double poluos2 = 6356862.02);

//PACKAGE : MAP.
//FUNCTION : SetVerboseFlag(bool flag= true);
//DESCRIPTION : R:ustanovit' flag vyvoda otladochnoj Info na <stdout>
// 				Set flag of debug information output to <stdout>.
//INPUTS : bool .
//RETURNS : NONE.	
	void   SetVerboseFlag(bool flag= true);

//PACKAGE : MAP.
//FUNCTION : GetVerboseFlag();
//DESCRIPTION : R:Poluchit' flag vyvoda otladochnoj Info na <stdout>
// 				Get flag of debug information output to <stdout>.
//INPUTS : MColor& .
//RETURNS : True.
	bool   GetVerboseFlag();

//PACKAGE : MAP.
//FUNCTION : ConvertKartaToCoo(MCoo* pNewCoo);
//DESCRIPTION : Map conversion into the new map coordinate system 
// 				Warning: current map coordinate system 'm_pObjectCoo' is not replaced with pNewCoo.
//INPUTS : MCoo pointer.
//RETURNS : NONE.	
	void ConvertKartaToCoo(MCoo* pNewCoo);

//PACKAGE : MAP.
//FUNCTION : SetAttrDataByUid(double uid, MString& attrName, MString& data);
//DESCRIPTION : For the operational object 'uid' the function puts 'data' into attribute 'attrName'.
//INPUTS : double , MString& , MString& .
//RETURNS : True.	
	bool SetAttrDataByUid(double uid, MString& attrName, MString& data);

//PACKAGE : MAP.
//FUNCTION : UniteTopobjects(MTopobject* tpo, MTopobject* tpoAdd, MPoint point1, MPoint point2, MPoint ptReg1, MPoint ptReg2);
//DESCRIPTION : Unite two objects by segment [point1,point2], 
// 				ptReg1 - point in tpo region, ptReg2 - point in tpoAdd region.
//INPUTS : MTopobject pointer, MTopobject pointer, MPoint , MPoint , MPoint , MPoint .
//RETURNS : True.	
	bool UniteTopobjects(MTopobject* tpo, MTopobject* tpoAdd, MPoint point1, MPoint point2, MPoint ptReg1, MPoint ptReg2);
	
    struct SSortLayers
    {
        bool operator()(const MLayer* first, const MLayer* second) const
        {
          return !(first->m_UID < second->m_UID);
        }
    };
};


#endif //__NUXKARTA_H__
