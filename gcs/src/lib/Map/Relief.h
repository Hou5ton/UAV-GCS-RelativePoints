//PACKAGE : MAP.
//FILE: Relief.h   
//AUTHOR:
//DESCRIPTION:

#ifndef __RELIEF_H__
#define __RELIEF_H__
#include <queue>
#include <QProgressBar>
#include "Topobject.h"
#include "Layer.h"
#include "MString.h"

// Line constants
#if ENGVERSION == 1
	#define IDS_RELIEF_LAYER "Relief Layer"
	#define IDS_ALTITUDE_MARKS "Altitude Marks"
	#define IDS_HORIZONTAL "Horizontal"
	#define IDS_ADT_HORIZONTAL "Additional Horizontal"
	#define IDS_FAT_HORIZONTAL "Thicker Horizontal"
// IDS_OBRYV,IDS_OTKOS,IDS_PODPSTENKA_FLL,IDS_GIDRO are defined in 'Karta.h'
	#define IDS_YAMA_VAL "Hole/Arbor"
	#define IDS_BASIC "Main"
	#define RLF_MESSAGE1 "Macroform Contour Can't be Self-Crossing"
	#define RLF_MESSAGE2 "Upper Limit of The Hole Can't Contain Less Than 3 Points!"	
	#define RLF_MESSAGE3 "Lower Limit of The Hole Can't Contain Less Than 3 Points!"
	#define RLF_MESSAGE4 "Hole Contours Can't be Self-Crossing!"
	#define RLF_MESSAGE5 "Edge is Not Belong to Adjacent Triangle!"
	#define RLF_MESSAGE6 "Edge is Not Belong to Convex Quadroangle"
	#define RLF_MESSAGE7 "Relief Contour is Not Created"

#else 
	#define IDS_RELIEF_LAYER "слой рельефа"
	#define IDS_ALTITUDE_MARKS "Отметки высот"
	#define IDS_HORIZONTAL "Горизонталь"
	#define IDS_ADT_HORIZONTAL "Доп.горизонталь"
	#define IDS_FAT_HORIZONTAL "Утолщ.горизонталь"
//	IDS_OBRYV,IDS_OTKOS,IDS_PODPSTENKA_FLL ,IDS_GIDRO - Прописаны в 'Karta.h'
//	"Обрыв", "Откос", "Подпорная стенка", "Гидрообъект"
	#define IDS_YAMA_VAL "Яма/вал"
	#define IDS_BASIC "основная"	
	#define RLF_MESSAGE1 "Контур макроформы не может самопересекатъся!"
	#define RLF_MESSAGE2 "Верхняя граница ямы не может содержатъ менъше трех точек!"	
	#define RLF_MESSAGE3 "Нижняя граница ямы не может содержатъ менъше трех точек!"
	#define RLF_MESSAGE4 "Контуры ямы не могут пересекатъся!"
	#define RLF_MESSAGE5 "Ребро не принадлежит смежным треуголъникам!"
	#define RLF_MESSAGE6 "Ребро не принадлежит выпуклому четырехуголънику!"
	#define RLF_MESSAGE7 "Не сформирован контур релъефа!"
#endif

// Index of Bergstrichs array
#define IZO_BERGSHTRIH	3

// Main horizontal
#define IZ_OSNOVNAYA	"OSN"

// Additional horizontal
#define IZ_DOPOLN		"DOP"

// reinforced horizontal
#define IZ_UTOLSCH		"UTL"

//PACKAGE : MAP.
//CLASS : MIzoline : public MTopobject.
//DESCRIPTION : Allows to create object 'isoline'.
class MIzoline : public MTopobject
{
public:

//PACKAGE : MAP.
//FUNCTION : MIzoline::MIzoline();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MIzoline();

//PACKAGE : MAP.
//FUNCTION : MIzoline::~MIzoline();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MIzoline(){};
public:

//PACKAGE : MAP.
//FUNCTION : MIzoline::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MIzoline::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MIzoline::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void		Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MIzoline::DrawTracker(MPlotDevice* pDevice,TrackerState state);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer,TrackerState .
//RETURNS : NONE.
	virtual void DrawTracker(MPlotDevice* pDevice,TrackerState state);

//PACKAGE : MAP.
//FUNCTION : MIzoline::WhatTheObject();
//DESCRIPTION : Sets name of map object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_IZOLINE;}

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetIerarhy();
//DESCRIPTION : Returns hierarchy.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MTopobject::GetIerarhy();
									return retVal+" "+ID_IZOLINE;}

//PACKAGE : MAP.
//FUNCTION : MIzoline::RecalcBounds();
//DESCRIPTION : Recalculates boundary rectangle.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool      RecalcBounds();

//PACKAGE : MAP.
//FUNCTION : MIzoline::AddBergshtrih(const MZarubka& geopoint);
//DESCRIPTION : Adds bergstrich.
//INPUTS : const MZarubka&.
//RETURNS : NONE.
	void              AddBergshtrih(const MZarubka& geopoint);

//PACKAGE : MAP.
//FUNCTION : MIzoline::AddBergshtrih(int nomrebra, MPoint pt);
//DESCRIPTION : Adds bergstrich.
//INPUTS : int , MPoint .
//RETURNS : NONE.
	void		  AddBergshtrih(int nomrebra, MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MIzoline::DeleteBergshtrih(int num);
//DESCRIPTION : Deletes bergstrich.
//INPUTS : int.
//RETURNS : NONE.
	void		  DeleteBergshtrih(int num);

//PACKAGE : MAP.
//FUNCTION : MIzoline::DeleteNearPoints(double shag);
//DESCRIPTION : Deletes close points of horizontal.
//INPUTS : double.
//RETURNS : NONE.
	void		  DeleteNearPoints(double shag);

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetBergNumber();
//DESCRIPTION : Returns quantity of bergstrichs.
//INPUTS : NONE.
//RETURNS : int.
	int		  GetBergNumber();

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetBergshtrih(int ind, MZarubka* berg);
//DESCRIPTION : Returns bergstrich by index.
//INPUTS : int , MZarubka pointer.
//RETURNS : True.
	bool              GetBergshtrih(int ind, MZarubka* berg);

//PACKAGE : MAP.
//FUNCTION : MIzoline::ConstructBergshtrih(MPoint point, MZarubka* bergshtrih);
//DESCRIPTION : Creates bergstrich of horizontal by input point.
//INPUTS : MPoint , MZarubka pointer.
//RETURNS : True.
	bool		  ConstructBergshtrih(MPoint point, MZarubka* bergshtrih);

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetBergLength();
//DESCRIPTION : Returns bergstrich length.
//INPUTS : NONE.
//RETURNS : float.
	float             GetBergLength();

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetBergLengthScaled();
//DESCRIPTION : Returns bergstrich length,multiplied by real scale coefficient.
//INPUTS : NONE.
//RETURNS : double.
	double			  GetBergLengthScaled();

//PACKAGE : MAP.
//FUNCTION : MIzoline::SetBergLength(float berglength);
//DESCRIPTION : Sets bergstrich length.
//INPUTS : float.
//RETURNS : True.
	bool			  SetBergLength(float berglength);

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetizoHeight(MString* height);
//DESCRIPTION : Returns height of horizontal.
//INPUTS : MString pointer.
//RETURNS : True.
	bool			  GetizoHeight(MString* height);

//PACKAGE : MAP.
//FUNCTION : MIzoline::SetizoHeight(double height);
//DESCRIPTION : Sets height of horizontal.
//INPUTS : double.
//RETURNS : True.
	bool			  SetizoHeight(double height);

//PACKAGE : MAP.
//FUNCTION : MIzoline::GetizoType(MString* ptype);
//DESCRIPTION : Returns type of horizontal.
//INPUTS : MString pointer.
//RETURNS : True.
	bool			  GetizoType(MString* ptype);

//PACKAGE : MAP.
//FUNCTION : MIzoline::SetizoType(MString type);
//DESCRIPTION : Sets type of horizontal.
//INPUTS : MString.
//RETURNS : True.
	bool			  SetizoType(MString type);

//PACKAGE : MAP.
//FUNCTION : MIzoline::OrientInverse();
//DESCRIPTION : Changes horizontal orientation.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool	  OrientInverse();

//PACKAGE : MAP.
//FUNCTION : MIzoline::InsertPointAfter(int num, const MPoint& geopoint);
//DESCRIPTION : Insert point before point : point in geocoordinates.
//INPUTS : int , const MPoint& .
//RETURNS : NONE.
	virtual void	  InsertPointAfter(int num, const MPoint& geopoint);

//PACKAGE : MAP.
//FUNCTION : MIzoline::DeletePoint(int num);
//DESCRIPTION : Deletes point.
//INPUTS : int.
//RETURNS : NONE.
	virtual void	  DeletePoint(int num);

//PACKAGE : MAP.
//FUNCTION : MIzoline::InsertPoint(int num, const MPoint& geopoint);
//DESCRIPTION : Adds point.
//INPUTS : int , const MPoint& .
//RETURNS : NONE.
	virtual void	  InsertPoint(int num, const MPoint& geopoint);

//PACKAGE : MAP.
//FUNCTION : MIzoline::TransformOnType(double shag);
//DESCRIPTION : Converts linear horizontal(dependly on type) to interpolation or approximation.
//INPUTS : double.
//RETURNS : NONE.
	void			  TransformOnType(double shag);

//PACKAGE : MAP.
//FUNCTION : MIzoline::SetKlsAttributes(MString type);
//DESCRIPTION : Sets language for horizontal and formular by type horizontal of relief layer
// 				if such filled up.
//INPUTS : MString.
//RETURNS : True.
	virtual bool	  SetKlsAttributes(MString type);
};


// Class description - steep

//  Index of array wtih inscriptions							(m_Komplekt)
#define OBR_NADPIS			0

//  Index of array of formulas snaps						(m_Komplekt)
#define OBR_TK_PRIV			1

// Index of array of strings for stroring relative marks of depth of
// steep or altitudes of upper/lower edge of steep       (m_Komplekt)
#define OBR_INFORM			2

// Index of array of strings of relative marks of depth of
// steep or altitudes of upper/lower edge of steep       (m_Komplekt)
#define OBR_INFORMPT		3

 // Index of points array - intersegments of weight segments with lower
// border of steep                                       (m_Komplekt)
#define OBR_VESSEGMENTS		4

// Index of index array: first two indexes - the beginning and the end of
// upper border of steep; next two indexes - the beginning and the end of
// lower border of steep                                 (m_Komplekt)
#define OBR_INDEX			5


// Index of points array - coordinates of strichs segments;
// All strichs stored by pairs- the beginning ad the end of strich	(m_Komplekt)
#define OBR_SHTRIHOVKA 		6

// Index of array of points altitudes of steep border    (m_Komplekt)
#define OBR_HEIGHTPT		7

// Index of points array, which create
// structural line                                       (m_Komplekt)
#define OBR_STRUCTLINE 		8

// Index of points array of hole border, which creates
// structural line of hole bottom					    (m_Komplekt)
#define OBR_DOWNSTRUCTLINE 	9


///Allows to create object 'slope'
/**
*					  WARNING1 : Hatching of slope defined by an arrangement of weight segments. Originally at
*								 creation of slope weight pieces are defined as a bisector of the corners, formed
*						         by adjacent edges of the top border of slope. All strokes between the next weight segments
*								 converge in a point of intersection of weight segments. stroke drawing up to
*								 crossings with the bottom border of slope. Updating of one weight segment
*								 results to updating strokes of the next shaped areas with common defined
*								 weight segment.
*					   WARNING2: the index of array of points of border of steep is defined(determined) by variable F_TOCHKI - a member of a class
*								 MTopobject. In the constructor of class MObryv we believe F_TOCHKI=5. 			 (m_Komplekt)
*					   WARNING3: All qualitative information about type, thickness and color of a line for drawing  top edge
*								 of steep and strokes is stored in m_pZnak 								 (m_pZnak)
*		 	    	   WARNING4: at creation of steep at the moment of selection of the top border of steep an array of points of a contour
*								 breakage it is renumbered so that the index of the beginning of the top border of breakage was 0;
*								 it is always chosen beginning index less than final
*					   WARNING5: at creation of steep we focus a contour clockwise
*/

//PACKAGE : MAP.
//CLASS : MObryv : public MTopobject.
//DESCRIPTION : 
class MObryv : public MTopobject
{
public:

//PACKAGE : MAP.
//FUNCTION : MObryv::MObryv();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MObryv();

//PACKAGE : MAP.
//FUNCTION : MObryv::MObryv();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	~MObryv(){};
public:

	// parameter of macroform drawing
	bool m_viewObryv;

//PACKAGE : MAP.
//FUNCTION : MObryv::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MObryv::ReadBin(MFile* file,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MObryv::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetBytes();
//DESCRIPTION : Returns class size to UTP-file.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MObryv::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MObryv::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MObryv::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void  Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MObryv::DrawTracker(MPlotDevice* pDevice,TrackerState state);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer,TrackerState .
//RETURNS : NONE.
	virtual void DrawTracker(MPlotDevice* pDevice,TrackerState state);

//PACKAGE : MAP.
//FUNCTION : MObryv::DrawMfPolygon(MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Draws all square of macroform and it's contour of sheet color.
//INPUTS : MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.
	void  DrawMfPolygon(MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MObryv::DrawUpBorder(MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Draws all square of hydroobject.
//INPUTS : MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.
	void  DrawUpBorder(MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MObryv::DrawShtrihovka(MPlotDevice* pDevice);
//DESCRIPTION : Draws hatching of steep.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void  DrawShtrihovka(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MObryv::DrawMfPodpis(MPlotDevice* pDevice);
//DESCRIPTION : Draws relative depth or height of upper/lower
//				edge of steep, slope, supporting wall.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	void  DrawMfPodpis(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MObryv::WhatTheObject();
//DESCRIPTION : Returns name of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_OBRYV;}

//PACKAGE : MAP.
//FUNCTION : MObryv::GetIerarhy();
//DESCRIPTION : Returns hierarchy of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MTopobject::GetIerarhy();
									return retVal+" "+ID_OBRYV;}

//PACKAGE : MAP.
//FUNCTION : MObryv::RecalcBounds();
//DESCRIPTION : Recalculates boundary rectangle.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool      RecalcBounds();

//PACKAGE : MAP.
//FUNCTION : MObryv::AddNadpis(MPoint nadpispt, MString nadpis);
//DESCRIPTION : Adds inscription of depth or height of upper/lower edge of steep to strings and points arrays.
//INPUTS : MPoint , MString .
//RETURNS : NONE.
	void              AddNadpis(MPoint nadpispt, MString nadpis);

//PACKAGE : MAP.
//FUNCTION : MObryv::DeleteNadpis(int num);
//DESCRIPTION : Deletes inscription of depth or or height of upper/lower edge of steep with number 'num' from
// 				strings and points arrays.
//INPUTS : int.
//RETURNS : NONE.
	void			  DeleteNadpis(int num);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetNadpisNumber();
//DESCRIPTION : Function outputs number of inscriptions.
//INPUTS : NONE.
//RETURNS : int.
	int				  GetNadpisNumber();

//PACKAGE : MAP.
//FUNCTION : MObryv::GetNadpisPrkkod();
//DESCRIPTION : Function provides handwriting.
//INPUTS : NONE.
//RETURNS : unsigned char.
	unsigned char              GetNadpisPrkkod();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetNadpisPrkkod(unsigned char prkkod);
//DESCRIPTION : Sets code of inscription handwrite of depth ot height of upper/lower steep edge.
//INPUTS : unsigned char.
//RETURNS : True.
	bool			  SetNadpisPrkkod(unsigned char prkkod);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetNadpisSym();
//DESCRIPTION : Function outputs symbol of inscription of
//				upper/lower edge of steep.
//INPUTS : NONE.
//RETURNS : MSymbol pointer.
	virtual MSymbol*          GetNadpisSym();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetNadpisSym(MSymbol* sym);
//DESCRIPTION : Sets symbol of inscription of depth or height of upper/lower steep edge.
//INPUTS : MSymbol.
//RETURNS : True.
	bool			  SetNadpisSym(MSymbol* sym);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetDistShtrih();
//DESCRIPTION : Outputs distance between steep strokes.
//INPUTS : NONE.
//RETURNS : float.
	float             GetDistShtrih();

//PACKAGE : MAP.
//FUNCTION : MObryv::GetDistShtrihScaled();
//DESCRIPTION : Outputs distance between steep strokes, multiplied by
// 				real scale coefficient.
//INPUTS : NONE.
//RETURNS : double.
	double			  GetDistShtrihScaled();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetDistShtrih(float dist);
//DESCRIPTION :  Set distance between steep strokes.
//INPUTS : float.
//RETURNS : True.
	bool			  SetDistShtrih(float dist);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetPointVesSegment(int ind, MPoint* ptsg);
//DESCRIPTION : Outputs point coordinates of crossing of weight segment with lower border of contour
// 				by number of weight segment.
//INPUTS : int , MPoint pointer.
//RETURNS : True.
	bool              GetPointVesSegment(int ind, MPoint* ptsg);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetHeight(int ind);
//DESCRIPTION : Outputs point height - borders of steep contour.
//INPUTS : int.
//RETURNS : double.
	double            GetHeight(int ind);

//PACKAGE : MAP.
//FUNCTION : MObryv::SetHeight(int ind, double height);
//DESCRIPTION : Sets point height - borders of steep contour.
//INPUTS : int , double .
//RETURNS : True.
	bool              SetHeight(int ind, double height);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetUpFirstInd();
//DESCRIPTION : Outputs index of the beginning of upper steep edge.
//INPUTS : NONE.
//RETURNS : int.
	int				  GetUpFirstInd();

//PACKAGE : MAP.
//FUNCTION : MObryv::GetUpLastInd();
//DESCRIPTION : Outputs index of the end of upper steep edge.
//INPUTS : NONE.
//RETURNS : int.
	int				  GetUpLastInd();

//PACKAGE : MAP.
//FUNCTION : MObryv::GetDownFirstInd();
//DESCRIPTION : Outputs index of the beginning of lower steep edge.
//INPUTS : NONE.
//RETURNS : int.
	int				  GetDownFirstInd();

//PACKAGE : MAP.
//FUNCTION : MObryv::GetDownLastInd();
//DESCRIPTION : Outputs index of the end of lower steep edge.
//INPUTS : NONE.
//RETURNS : int.
	int				  GetDownLastInd();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetUpFirstInd(int upfirstind);
//DESCRIPTION : Sets index of the beginning of upper steep edge.
//INPUTS : int.
//RETURNS : True.
	bool			  SetUpFirstInd(int upfirstind);

//PACKAGE : MAP.
//FUNCTION : MObryv::SetUpLastInd(int uplastind);
//DESCRIPTION : Sets index of the end of upper steep edge.
//INPUTS : int.
//RETURNS : True.
	bool			  SetUpLastInd(int uplastind);

//PACKAGE : MAP.
//FUNCTION : MObryv::SetDownFirstInd(int downfirstind);
//DESCRIPTION : Sets index of the beginning of lower steep edge.
//INPUTS : int.
//RETURNS : True.
	bool			  SetDownFirstInd(int downfirstind);

//PACKAGE : MAP.
//FUNCTION : MObryv::SetDownLastInd(int downlastind);
//DESCRIPTION : Sets index of the end of lower steep edge.
//INPUTS : int.
//RETURNS : True.
	bool			  SetDownLastInd(int downlastind);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetKontur();
//DESCRIPTION :  Returns macroform contour.
//INPUTS : NONE.
//RETURNS : MPtArray pointer.
	MPtArray*		  GetKontur();

//PACKAGE : MAP.
//FUNCTION : MObryv::GetHeights();
//DESCRIPTION : Returns heights of steep contour points.
//INPUTS : NONE.
//RETURNS : MDuArray pointer.
	MDuArray*		  GetHeights();

//PACKAGE : MAP.
//FUNCTION : MObryv:: MakeStructKontur(MPtArray* structkonturfirst, MPtArray* structkonturlast = NULL);
//DESCRIPTION : Constructs a circuit break , slope , retaining wall or hydroobject
// 				a breakline.
//INPUTS : MPtArray pointer, MPtArray pointer.
//RETURNS : True.
	virtual bool	  MakeStructKontur(MPtArray* structkonturfirst,
						MPtArray* structkonturlast = NULL);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetStructKontur();
//DESCRIPTION : Returns macroform contour as structural line.
//INPUTS : NONE.
//RETURNS : MPtArray pointer.
	MPtArray*		  GetStructKontur();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetStructKontur(MPtArray* structkontur);
//DESCRIPTION : Sets structural line of macroform.
//INPUTS : MPtArray pointer.
//RETURNS : True.
	bool			  SetStructKontur(MPtArray* structkontur);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetDownStructKontur();
//DESCRIPTION : In a case of macroform, returns hole only.
//INPUTS : NONE.
//RETURNS : MPtArray pointer.
	virtual MPtArray* GetDownStructKontur(){return NULL;};

//PACKAGE : MAP.
//FUNCTION : MObryv::SetDownStructKontur(MPtArray* );
//DESCRIPTION : In a case of macroform, sets hole only.
//INPUTS : MPtArray pointer.
//RETURNS : True.
	virtual bool	  SetDownStructKontur(MPtArray* ){return true;};

//PACKAGE : MAP.
//FUNCTION : MObryv::CompareStructKonturWithKontur(MInArray* upstructind, MInArray* downstructind = NULL);
//DESCRIPTION : Compares points of macroform contour with points of structural line , which correspond to macroform,
// 				and returns indexes of coinciding points.
//INPUTS : MInArray pointer, MInArray pointer.
//RETURNS : True.
	virtual bool	  CompareStructKonturWithKontur(MInArray* upstructind,
						MInArray* downstructind = NULL);

//PACKAGE : MAP.
//FUNCTION : MObryv::AddPoint(const MPoint& geopoint, double height);
//DESCRIPTION : Adds point to points array of steep and height of additioning point to array of altitudes.
//INPUTS : const MPoint& , double .
//RETURNS : NONE.
	void			  AddPoint(const MPoint& geopoint, double height);

//PACKAGE : MAP.
//FUNCTION : MObryv::InsertPointAfter(int num, const MPoint& geopoint, double height,bool pttype = true);
//DESCRIPTION : Inserts a point after point: point in geocoordinates.
//INPUTS : int , const MPoint& , double ,bool .
//RETURNS : NONE.
	void			  InsertPointAfter(int num, const MPoint& geopoint, double height,bool pttype = true);

//PACKAGE : MAP.
//FUNCTION : MObryv::InsertPointAfter(int num, const MPoint& geopoint,bool pttype = true);
//DESCRIPTION : Inserts a point after point: point in geocoordinates;
// 				height of the inserted point , if possible, be approximated from relefa or set manually.
//INPUTS : int , const MPoint& ,bool .
//RETURNS : NONE.
	void			  InsertPointAfter(int num, const MPoint& geopoint,bool pttype = true);

//PACKAGE : MAP.
//FUNCTION : MObryv::InsertPoint(int num, const MPoint& geopoint, double height,bool pttype = true);
//DESCRIPTION : Inserts a point before point: point in geocoordinates.
//INPUTS : int , const MPoint& , double ,bool .
//RETURNS : NONE.
	void			  InsertPoint(int num, const MPoint& geopoint, double height,bool pttype = true);

//PACKAGE : MAP.
//FUNCTION : MObryv::InsertPoint(int num, const MPoint& geopoint,bool pttype = true);
//DESCRIPTION : Inserts a point before point: point in geocoordinates;
// 				Height of inserted point(if possible) have to be approximated from relief or setted manually.
//INPUTS : int , const MPoint& ,bool .
//RETURNS : NONE.
	void			  InsertPoint(int num, const MPoint& geopoint,bool pttype = true);

//PACKAGE : MAP.
//FUNCTION : MObryv::HitTest(MPoint point, double dDostup,MPtArray*& ptarr,int& index);
//DESCRIPTION : Test: Where specifies the cursor of the mouse?
// 				Checks contour points for steep and it's inheritors.
//INPUTS : MPoint , double ,MPtArray*& ,int& .
//RETURNS : True.
	virtual bool HitTest(MPoint point, double dDostup,MPtArray*& ptarr,int& index);

//PACKAGE : MAP.
//FUNCTION : MObryv::AddHeight(double height);
//DESCRIPTION : Adds point height to array of altitudes.
//INPUTS : double.
//RETURNS : NONE.
	void			  AddHeight(double height);

//PACKAGE : MAP.
//FUNCTION : MObryv::DeletePointFromKontur(int num);
//DESCRIPTION : Delete point from kontur.
//INPUTS : int.
//RETURNS : True.
	virtual bool	  DeletePointFromKontur(int num);

//PACKAGE : MAP.
//FUNCTION : MObryv::RenumPoints(int num);
//DESCRIPTION : Renumbers array of points of steep contour so as numbering of points ciclically
// 				moved on one type backward.
//INPUTS : int.
//RETURNS : True.
	bool              RenumPoints(int num);

//PACKAGE : MAP.
//FUNCTION : MObryv::FindPoint(MPoint pt);
//DESCRIPTION : Searchs for the point with defined coordinates among points of macroform contour.
//INPUTS : MPoint.
//RETURNS : int.
	int				  FindPoint(MPoint pt);

// utilities

//PACKAGE : MAP.
//FUNCTION : MObryv::GetCrossptWithKontur(MPtArray* ptar, MPoint pt1, MPoint pt2, MPoint* pt, bool zamknut=true, MPtArray* upborder=NULL);
//DESCRIPTION : Finds point of intersection of sraight line with steep contour
// 				Straight line redefining by points pt1 and pt2; point of crossing saves to 'pt'.
//INPUTS : MPtArray pointer, MPoint , MPoint , MPoint pointer, bool , MPtArray pointer.
//RETURNS : int.
	int               GetCrossptWithKontur(MPtArray* ptar, MPoint pt1, MPoint pt2, MPoint* pt, bool zamknut=true, MPtArray* upborder=NULL);

//PACKAGE : MAP.
//FUNCTION : MObryv::GetCrossptWithDownBorder(MPoint pt1, MPoint pt2, MPoint* point);
//DESCRIPTION : Finds point of intersection of the beam, the beginning of which 'pt1' lays on a contourî with the lower border of steep.
//INPUTS : MPoint , MPoint , MPoint pointer.
//RETURNS : int.
	int				  GetCrossptWithDownBorder(MPoint pt1, MPoint pt2, MPoint* point);

//PACKAGE : MAP.
//FUNCTION : MObryv::OrientInverse();
//DESCRIPTION : Reorientates contour points of steep and array of contours points altitudes.
//INPUTS : NONE.
//RETURNS : True.
	bool			  OrientInverse();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetShtrihFirstPoints();
//DESCRIPTION : Calculates coordinates of the beginnings of all strokes.
//INPUTS : NONE.
//RETURNS : True.
	bool              SetShtrihFirstPoints();

//PACKAGE : MAP.
//FUNCTION : MObryv::MasterShtrihovka();
//DESCRIPTION : Calculates coordinates of the ends of all strokes.
//INPUTS : NONE.
//RETURNS : NOTrueNE.
	virtual bool	  MasterShtrihovka();

//PACKAGE : MAP.
//FUNCTION : MObryv::SetVesSegments;
//DESCRIPTION : Defines weight segments: first point of segment - point of steep contour, second point -
//  			point of crossing of weight stretch line with steep contour..
//INPUTS : NONE.
//RETURNS : True.
	virtual bool	  SetVesSegments();
};

//PACKAGE : MAP.
//CLASS : MOtkos : public MObryv.
//DESCRIPTION : Allows to create object of map "slope".
class MOtkos : public MObryv
{
public:

//PACKAGE : MAP.
//FUNCTION : MOtkos::MOtkos();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MOtkos();

//PACKAGE : MAP.
//FUNCTION : MOtkos::~MOtkos();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MOtkos(){};

//PACKAGE : MAP.
//FUNCTION : MOtkos::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MOtkos::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MOtkos::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MOtkos::DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Draws lower border of slope, supporting wall.
//INPUTS : MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.
	void  DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MOtkos::WhatTheObject();
//DESCRIPTION : Returns object name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_OTKOS;}

//PACKAGE : MAP.
//FUNCTION : MOtkos::GetIerarhy();
//DESCRIPTION : Returns hierarchy of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MObryv::GetIerarhy();
									return retVal+" "+ID_OTKOS;}

//PACKAGE : MAP.
//FUNCTION : MOtkos::GetDlinaShtrihaForOtkos();
//DESCRIPTION : Outputs length of small stroke for slope.
//INPUTS : NONE.
//RETURNS : float.
	float			  GetDlinaShtrihaForOtkos();

//PACKAGE : MAP.
//FUNCTION : MOtkos::SetDlinaShtrihaForOtkos(float dlina);
//DESCRIPTION : Sets length of small stroke for slope.
//INPUTS : float.
//RETURNS : True.
	bool			  SetDlinaShtrihaForOtkos(float dlina);

	// utilities

//PACKAGE : MAP.
//FUNCTION : MOtkos::MasterShtrihovka();
//DESCRIPTION : Calculates coordinates of all edges of strokes for slope.
//INPUTS : NONE.
//RETURNS : True.
	bool			  MasterShtrihovka();

//PACKAGE : MAP.
//FUNCTION : MOtkos::SetVesSegments();
//DESCRIPTION : Calculates "weight" segments: first point - point of steep contour, second point -
// 				point of intersection of weight straight line with steep contour.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool	  SetVesSegments(){bool par = MObryv::SetVesSegments();return par;};
};

//PACKAGE : MAP.
//CLASS : MYama : public MOtkos.
//DESCRIPTION : Allows to create object "hole"
class MYama : public MOtkos
{
public:

//PACKAGE : MAP.
//FUNCTION : MYama::MYama();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MYama();

//PACKAGE : MAP.
//FUNCTION : MYama::MYama();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	~MYama(){};

//PACKAGE : MAP.
//FUNCTION : MYama::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MYama::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MYama::DrawKontur(MPlotDevice* pDevice);
//DESCRIPTION :  Draws hole contour.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void  DrawKontur(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MYama::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void  Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MYama::WhatTheObject();
//DESCRIPTION : Returns name of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_YAMA;}

//PACKAGE : MAP.
//FUNCTION : MYama::GetIerarhy();
//DESCRIPTION : Returns hierarchy of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MOtkos::GetIerarhy();
									return retVal+" "+ID_YAMA;}

// utilities

//PACKAGE : MAP.
//FUNCTION : MYama::SetVesSegments();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : True.
	bool			  SetVesSegments();

//PACKAGE : MAP.
//FUNCTION : MYama::MakeStructKontur(MPtArray* structkonturfirst, MPtArray* structkonturlast);
//DESCRIPTION : Copies contour of the hole as two structutal lines.
//INPUTS : MPtArray pointer, MPtArray pointer.
//RETURNS : True.
	bool			  MakeStructKontur(MPtArray* structkonturfirst, MPtArray* structkonturlast);

//PACKAGE : MAP.
//FUNCTION : MYama::GetDownStructKontur();
//DESCRIPTION : Returns contour of the bottom of hole as structural line.
//INPUTS : NONE.
//RETURNS : MPtArray pointer.
	MPtArray*		  GetDownStructKontur();

//PACKAGE : MAP.
//FUNCTION : MYama::SetDownStructKontur(MPtArray* structkontur);
//DESCRIPTION : Sets structural line for lower edge of hole.
//INPUTS : MPtArray pointer.
//RETURNS : True.
	bool			  SetDownStructKontur(MPtArray* structkontur);

//PACKAGE : MAP.
//FUNCTION : MYama::CompareStructKonturWithKontur(MInArray* upstructind, MInArray* downstructind);
//DESCRIPTION : Compares points of contour of the hole with points of structural lines, which are correspond to
// 				the top and bottom of the hole and returns indexes of coinciding points for each structural line.
//INPUTS : MInArray pointer, MInArray pointer.
//RETURNS : True.
	bool			  CompareStructKonturWithKontur(MInArray* upstructind, MInArray* downstructind);

//PACKAGE : MAP.
//FUNCTION : MYama::DeletePointFromKontur(int num);
//DESCRIPTION : Deletes point from hole contour.
//INPUTS : int.
//RETURNS : True.
	bool			  DeletePointFromKontur(int num);
};

//PACKAGE : MAP.
//CLASS : MPodpStenka : public MObryv.
//DESCRIPTION : Allows to create object "supporting wall"
class MPodpStenka : public MObryv
{
public:

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::MPodpStenka();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MPodpStenka();

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::~MPodpStenka();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MPodpStenka(){};

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void  Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Draws lower border of supporting wall.
//INPUTS : MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.
	void  DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::DrawZubets(MPlotDevice* pDevice,int ind);
//DESCRIPTION : Draws jags of hatching for supporting wall.
//INPUTS : MPlotDevice pointer,int .
//RETURNS : NONE.
	void  DrawZubets(MPlotDevice* pDevice,int ind);

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::DrawZubetsiForPodpStenka(MPlotDevice* pDevice);
//DESCRIPTION : Draws jags of hatching for supporting wall.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	void  DrawZubetsiForPodpStenka(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::GetShirinaZubtsa();
//DESCRIPTION : Outputs jag width of supporting wall.
//INPUTS : NONE.
//RETURNS : float.
	float			  GetShirinaZubtsa();

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::SetShirinaZubtsa(float shirina);
//DESCRIPTION : Sets jag width for supporting wall.
//INPUTS : float.
//RETURNS : True.
	bool			  SetShirinaZubtsa(float shirina);

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::WhatTheObject();
//DESCRIPTION : Returns name of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_PODPSTENKA;}

//PACKAGE : MAP.
//FUNCTION : MPodpStenka::GetIerarhy();
//DESCRIPTION : Returns hierarchy of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MObryv::GetIerarhy();
									return retVal+" "+ID_PODPSTENKA;}
};

//PACKAGE : MAP.
//CLASS : MGidro : public MObryv.
//DESCRIPTION : Allows to create area objects
class MGidro : public MObryv
{
public:

//PACKAGE : MAP.
//FUNCTION : MGidro::MGidro();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MGidro();

//PACKAGE : MAP.
//FUNCTION : MGidro::~MGidro();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MGidro(){};

//PACKAGE : MAP.
//FUNCTION : MGidro::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MGidro::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MGidro::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	  Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MGidro::DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Draws lower edge of hydroobject.
//INPUTS : MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.
	void  DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MGidro::WhatTheObject();
//DESCRIPTION : Draws all square of hydroobject
// 				Returns name of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_GIDRO;}

//PACKAGE : MAP.
//FUNCTION : MGidro::GetIerarhy();
//DESCRIPTION : Returns hierarchy of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MObryv::GetIerarhy();
									return retVal+" "+ID_GIDRO;}
};


//PACKAGE : MAP.
//CLASS : MReliefLayer : public MLayer.
//DESCRIPTION : Allows to create relief layer.
class MReliefLayer : public MLayer
{
public:

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::MReliefLayer();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MReliefLayer();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::MReliefLayer(MString key);
//DESCRIPTION : Constructor.
//INPUTS : MString.
//RETURNS : NONE.
	MReliefLayer(MString key);

public:

	//  Array of triangular segments
	MSgArray* m_segments;

	// Array of triangular triangles
	MTrArray* m_triangles;

	//  Aray of structural lines
	MArArray* m_structlines;

	//  List of altitudes marks
	MObjectList m_heightpoints;

	//  List of horizontals
	MObjectList m_izolines;

	//  List of additional horizontals
	MObjectList m_dopizolines;

	// List of contours inside which draws additional horizontals
	MObjectList m_kontlist;

	// List of objects, which included steeps, slopes, holes etc.
	MObjectList m_macroforms;

	// List of hydroobjects
	MObjectList m_gidroobjects;

	// List of depths marks
	MObjectList m_depthpoints;

	//  List of lines of equal depths - isobaths
	MObjectList m_izobats;

	// List of objects, which are not taking part in calculating,
	// Service for qualitative graphical representative of the map
	MObjectList m_decorative;

	// Boundary contour of relief
	MPtArray* m_RelKonturPts;

	// Vertex heihts of boundary contour of relief layer
	MDuArray* m_RelKonturHgt;
	
	//======================================================
	//  Saveable parameters of dialogue of horizontals creating

	//  Quantity of additional points on the edge of broken horizontal
	unsigned short    m_doppts;

	//  Cut altitude of relief
	double   m_shag;

	//  Drawing modus of horizontals
	unsigned short     m_izotype;

	//  Drawing parameter of triangles list
	bool     m_viewtrg;

	//  Drawing parameter of horizontals list
	bool     m_viewizolines;

	//  Drawing parameter of isobaths list
	bool     m_viewizobats;

	//  Drawing parameter of contours of visibility of additional horizontals
	bool     m_viewkonturs;

	//  Drawing parameter of  relief contour
	bool	 m_viewRelKontur;

	//  Drawing parameter of  structural lines
	bool	 m_viewstructlines;

	//  Drawing parameter of altitudes marks number
	bool	 m_viewhptnom;

	//  Drawing parameter of altitudes marks altitude
	bool	 m_viewhptvalue;
	//=======================================================

	//  lower-range value of altitude of relief layer
	double m_minheight;

	//  max value of altitude of relief layer
	double m_maxheight;

	// Unsaved pointers to signs of classifier - base for automatic
	// constructing of main objects of relief layer lists
	MZnak* m_pKlsHeightpt;
	MZnak* m_pKlsIzoline;
	MZnak* m_pKlsDopizo;
	MZnak* m_pKlsUtlizo;
	MZnak* m_pKlsObryv;
	MZnak* m_pKlsOtkos;
	MZnak* m_pKlsPodpStenka;
	MZnak* m_pKlsGidro;
	MZnak* m_pKlsYama;
	MZnak* m_pKlsDepthpt;
	MZnak* m_pKlsIzobata;

	// Saveable quadrocodes of classifier objects - base for automatic
	// constructing of main objects of relief layer lists
	MString m_qkodHeightpt;
	MString m_qkodIzoline;
	MString m_qkodDopizo;
	MString m_qkodUtlizo;
	MString m_qkodObryv;
	MString m_qkodOtkos;
	MString m_qkodPodpStenka;
	MString m_qkodGidro;
	MString m_qkodYama;
	MString m_qkodDepthpt;
	MString m_qkodIzobata;

	
public:

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::MReliefLayer(const MRect& position);
//DESCRIPTION : Constructor.
//INPUTS : const MRect&.
//RETURNS : NONE.
	MReliefLayer(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::~MReliefLayer();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MReliefLayer();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ReadBin(MFile* file,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::WhatTheLayer();
//DESCRIPTION : Returns layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheLayer(){return ID_RELIEF_LAYER;};

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetMinHeight();
//DESCRIPTION : Finds and reminds point of min height in relief layer.
//INPUTS : NONE.
//RETURNS : MHeightPoint pointer.
	MHeightPoint* SetMinHeight();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetMaxHeight();
//DESCRIPTION : Finds and reminds point of max height in relief layer.
//INPUTS : NONE.
//RETURNS : MHeightPoint pointer.
	MHeightPoint* SetMaxHeight();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetMinMaxIzoheights(double shag, double* hmin, double* hmax);
//DESCRIPTION : Sets min and max altitudes of main horizontals
// 				at altitude of relief cut 'shag'.
//INPUTS : double , double pointer, double pointer.
//RETURNS : True.
	bool	  SetMinMaxIzoheights(double shag, double* hmin, double* hmax);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetMinMaxDopizoheights(double shag, double* hmindop, double* hmaxdop);
//DESCRIPTION : Sets min and max altitudes of additional horizontals
// 				at altitude of relief cut 'shag'.
//INPUTS : double , double pointer, double pointer.
//RETURNS : True.
	bool	  SetMinMaxDopizoheights(double shag, double* hmindop, double* hmaxdop);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ShakeTriangles(double c);
//DESCRIPTION : If altitudes in layer "triangulation" coinciding with line
// 				of level 'c', than we have to stir them a little bit for correct drawing of horizontals.
//INPUTS : double.
//RETURNS : True.
	bool	  ShakeTriangles(double c);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ShakeKonturOhvata(double c);
//DESCRIPTION : If altitudes in layer "triangulation" coinciding with line
// 				of level 'c', than we have to stir them a little bit for correct drawing of horizontals.
//INPUTS : double.
//RETURNS : True.
	bool	  ShakeKonturOhvata(double c);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetTrianglesWithLevel(double c, MTrArray* leveltrs, MBoArray* maskar);
//DESCRIPTION : Selects multitude of triangles, to which belongs line of level 'ñ', to array 'leveltrs',
// 				also creates array os masks 'maskar'.
//INPUTS : double , MTrArray pointer, MBoArray pointer.
//RETURNS : True.
	bool	  GetTrianglesWithLevel(double c, MTrArray* leveltrs, MBoArray* maskar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetLevelSgsKontOhvata(double c, MInArray* indArray);
//DESCRIPTION : In array of integers 'indArray' keeps indexes of edges of a boundary contour,
// 				through which there passes a line of a level.
//INPUTS : double , MInArray pointer.
//RETURNS : True.
	bool	  GetLevelSgsKontOhvata(double c, MInArray* indArray);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetIzolineOrientation(double c, MIzoline* pIzo, MPoint pt1, MPoint pt2, double h1, double h2);
//DESCRIPTION : Defines horizontal orientation.
//INPUTS : double , MIzoline pointer, MPoint , MPoint , double , double .
//RETURNS : True.
	bool	  SetIzolineOrientation(double c, MIzoline* pIzo, MPoint pt1, MPoint pt2, double h1, double h2);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::AddSegment(MPoint pt1, MPoint pt2, double h1, double h2);
//DESCRIPTION : Adds segment to Array of triangular segments.
//INPUTS : MPoint , MPoint , double , double .
//RETURNS : NONE.
	void      AddSegment(MPoint pt1, MPoint pt2, double h1, double h2);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::AddTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double h1, double h2, double h3);
//DESCRIPTION : Adds triangular to Array of triangular triangles.
//INPUTS : MPoint , MPoint , MPoint , double , double , double .
//RETURNS : NONE.
	void      AddTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double h1, double h2, double h3);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::AddHeightPoint(MPoint pt, double h, bool visible=true,MString signif="основная");
//DESCRIPTION : Adds new altitude mark to List of altitudes marks.
//INPUTS : MPoint , double , bool , MString .
//RETURNS : NONE.
	void	  AddHeightPoint(MPoint pt, double h, bool visible=true,MString signif="основная");

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::DeleteHeightPoint(MPoint pt);
//DESCRIPTION : Deletes mark of altitude from list of altitudes marks.
//INPUTS : MPoint.
//RETURNS : NONE.
	void	  DeleteHeightPoint(MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::AddHeightptArray(MPtArray* ptar, MDuArray* hptar, bool visible=true,MString signif="основная");
//DESCRIPTION : Adds array of points to List of altitude marks
//				parameter visible draws new altitudes marks.
//INPUTS : MPtArray pointer, MDuArray pointer, bool ,MString .
//RETURNS : NONE.
	void	  AddHeightptArray(MPtArray* ptar, MDuArray* hptar, bool visible=true,MString signif="основная");

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::DeletePointFromStructlines(MPoint pt);
//DESCRIPTION : If in a file of structural lines finded a line containing an entrance point,
//				such structural line must be paritioned into two structural lines, first of which is
// 				part of old structural line up to a point, which previous to deleting point, and the second - part of old
// 				structural line, beginning from a point, which following for the deleting up to the end.
//INPUTS : MPoint.
//RETURNS : NONE.
	void	  DeletePointFromStructlines(MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindHeightptByCoords(MPoint pt);
//DESCRIPTION : Returns pointer to altitude mark if it's coordinates coinciding
// 				with output coordinates.
//INPUTS : MPoint.
//RETURNS : MHeightPoint pointer.
	MHeightPoint* FindHeightptByCoords(MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::AddObject(MObject* pObj);
//DESCRIPTION : Deletes segment from array of triangular segments
// 				victor	bool      DeleteSegment(int ind);
// 				Deletes triangle from array of triangular triangles
// 				victor	bool      DeleteTriangle(int ind);
// 				Adds object.
//INPUTS : MObject pointer.
//RETURNS : NONE.
	void      AddObject(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::RemoveObject(MObject* pObj);
//DESCRIPTION : Deletes object.
//INPUTS : MObject pointer.
//RETURNS : NONE.
	void	  RemoveObject(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ConvertToMaket();
//DESCRIPTION : Converts to maket.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ConvertToMaket();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::MinimizeBounds();
//DESCRIPTION : Minimizes layer borders.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void	  MinimizeBounds();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetKlsZnak(MString qkod);
//DESCRIPTION : Returns pointer to classifier sign by quadrocode.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak*  GetKlsZnak(MString qkod);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FillTuneMassivsForMainReliefTypes(MStArray* pWhatTheObjArray, MStArray* pReturnKodArray, MStArray* pNameArray);
//DESCRIPTION : Fills up strings arrays to call 'MTuneClassDialog'-dialogue in document for
// 				main types of layer objects of relief.
//INPUTS : MStArray pointer, MStArray pointer, MStArray pointer.
//RETURNS : True.
	bool	  FillTuneMassivsForMainReliefTypes(MStArray* pWhatTheObjArray,
					MStArray* pReturnKodArray, MStArray* pNameArray);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetMainReliefTypes(MStArray* pReturnKodArray = NULL);
//DESCRIPTION : Sets pointers to objects of main types of relief layer
// 				to objects of classifier.
//INPUTS : MStArray pointer.
//RETURNS : NONE.
	void	  SetMainReliefTypes(MStArray* pReturnKodArray = NULL);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::Draw(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	void	  Draw(MPlotDevice* pDevice,MRect geodrawrect);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::DrawText(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Draws text.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	void	  DrawText(MPlotDevice* pDevice,MRect geodrawrect);

//  Draws boundary contour of relief layer
// 	void	  DrawTriangulationKontur(MView* pView, CDC* pDC);
//  Draws structural line
// 	void	  DrawStructLine(MPtArray* structline, MView* pView, CDC* pDC);

	//  utilities

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ClearIzolinesFromNearPoints(MObjectList* izolines);
//DESCRIPTION : Clears horizontals out of close points.
//INPUTS : MObjectList pointer.
//RETURNS : True.
	bool	  ClearIzolinesFromNearPoints(MObjectList* izolines);

// Horizontal conversion accordly to type into interpolation or approximation
// 	bool	  TransformIzolinesOnType(MObjectList* izolines);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::RemoveRelictIzolines(MObjectList* izolines);
//DESCRIPTION : Deletes relict horizontals.
//INPUTS : MObjectList pointer.
//RETURNS : True.
	bool	  RemoveRelictIzolines(MObjectList* izolines);

// Triangulation of dotted layer
// victor	bool	  Createtriangulation();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateLevelBorderIzolines(double c, MInArray* indArray, MTrArray* leveltrs, MBoArray* maskar, int count, bool type);
//DESCRIPTION : Constructs boundary altitude horizontals 'ñ' by triangular triangles.
//INPUTS : double , MInArray pointer, MTrArray pointer, MBoArray pointer, int , bool .
//RETURNS : True.
	bool	  CreateLevelBorderIzolines(double c, MInArray* indArray, MTrArray* leveltrs, MBoArray* maskar, int count, bool type);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateLevelInnerIzolines(double c, MTrArray* leveltrs, MBoArray* maskar, int count, bool type);
//DESCRIPTION : Constructs inner altitude horizontals 'ñ' by triangular triangles.
//INPUTS : double , MTrArray pointer, MBoArray pointer, int , bool .
//RETURNS : True.
	bool      CreateLevelInnerIzolines(double c, MTrArray* leveltrs, MBoArray* maskar, int count, bool type);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateLevelIzolines(double c, int count, bool type);
//DESCRIPTION : Constructs altitude horizontals 'ñ' by triangular triangles.
//INPUTS : double , int , bool .
//RETURNS : True.
	bool	  CreateLevelIzolines(double c, int count, bool type);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateIzolines(double izomin, double izomax, bool type);
//DESCRIPTION : Constructs layer horizontals by triangular triangles.
//INPUTS : double , double , bool .
//RETURNS : True.
	bool	  CreateIzolines(double izomin, double izomax, bool type);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::DeleteSegmentOfReliefKontur(int numseg);
//DESCRIPTION : circuit relief removes a rib number numseg.
//INPUTS : int.
//RETURNS : True.
	bool	  DeleteSegmentOfReliefKontur(int numseg); 

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::DeleteSegmentOfStructline(int numline, int numseg, bool& delpar);
//DESCRIPTION : Deletes segment with number 'numseg' in structural line with number 'numline'.
//INPUTS : int , int , bool& .
//RETURNS : True.
	bool	  DeleteSegmentOfStructline(int numline, int numseg, bool& delpar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::IsTriSegmentOfStructline(MSegment* pSeg, int& numline, int& numseg);
//DESCRIPTION : Defines if current segment belong to structural line by triangular segment.
//INPUTS : MSegment pointer, int& , int& .
//RETURNS : True.
	bool	  IsTriSegmentOfStructline(MSegment* pSeg, int& numline, int& numseg);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::IsTriSegmentOfMacroforma(MSegment* pSeg);
//DESCRIPTION : Defines if current segment belong to macroform by triangular segment.
//INPUTS : MSegment pointer.
//RETURNS : True.
	bool	  IsTriSegmentOfMacroforma(MSegment* pSeg);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::IsPermissibleSegment(MPoint pt1, MPoint pt2);
//DESCRIPTION : Checks, if it is possible to assign an entrance segment; for this purpose crossing of entrance segment
// 				with a structural line is allowable or a contour of macroform of relief only at vertexes
// 				(so that the point of crossing simultaneously coincided with one of the ends of a segment and vertex
// 				of structural line or a contour of macroform).
//INPUTS : MPoint , MPoint .
//RETURNS : True.
	bool	  IsPermissibleSegment(MPoint pt1, MPoint pt2);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindTrianglesByTriSegment(MSegment* pSegment, MBoArray* sgmaskar, MTrArray* sgtrar);
//DESCRIPTION : Finds triangular triangles, to which belong input segment by triangular segment.
//INPUTS : MSegment pointer, MBoArray pointer, MTrArray pointer.
//RETURNS : True.
	bool	  FindTrianglesByTriSegment(MSegment* pSegment, MBoArray* sgmaskar, MTrArray* sgtrar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetTriTrianglesPtHeight(MPoint pt, double height);
//DESCRIPTION : By coordinates of a point finds triangular triangles,
// 				to which belong point, and changes altitude of triangle vertex , coincided
// 				with an input point, on input value of altitude.
//INPUTS : MPoint , double .
//RETURNS : True.
	bool	  SetTriTrianglesPtHeight(MPoint pt, double height);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetTriSegmentsPtHeight(MPoint pt, double height);
//DESCRIPTION : By coordinates of a point finds triangular segments,
// 				to which belong point, and changes altitude of segment edge , coincided
// 				with an input point, on input value of altitude.
//INPUTS : MPoint , double .
//RETURNS : True.
	bool	  SetTriSegmentsPtHeight(MPoint pt, double height);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ChangeRebroTriangulyatsii(MSegment* pSegment);
//DESCRIPTION : Changes edge of triangulation.
//INPUTS : MSegment pointer.
//RETURNS : True.
	bool	  ChangeRebroTriangulyatsii(MSegment* pSegment);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetValueFromRelief(MPoint pt);
//DESCRIPTION : Defines altitude of the random point 'pt' of a relief layer, proceeding from triangular triangles.
//INPUTS : MPoint.
//RETURNS : double.
	double	  GetValueFromRelief(MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetValueFromReliefByIzolines(MPoint pt);
//DESCRIPTION : Defines height of random point 'pt' of a relief layer ,proceeding from horizontals.
//INPUTS : MPoint.
//RETURNS : double.
	double	  GetValueFromReliefByIzolines(MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetProfileInfo(MPtArray* pLomLine);
//DESCRIPTION : Gets information for constructing of profile by cutting line.
//INPUTS : MPtArray pointer.
//RETURNS : MPtArray pointer.
	MPtArray* GetProfileInfo(MPtArray* pLomLine);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetProfileInfoByIzolines(MPtArray* pLomLine, double dopusk=0);
//DESCRIPTION : Gets profile info by Izolines.
//INPUTS : MPtArray pointer, double .
//RETURNS : MPtArray pointer.
	MPtArray* GetProfileInfoByIzolines(MPtArray* pLomLine, double dopusk=0);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindCrosslinesWithGidro(MPtArray* plankontur, MPtArray* Gidrocoord, MPtArray* Gidroheight);
//DESCRIPTION : Funxtion finds all intersections of gas pipe-line with hydroobjects.
//INPUTS : MPtArray pointer, MPtArray pointer, MPtArray pointer.
//RETURNS : True.
	bool	  FindCrosslinesWithGidro(MPtArray* plankontur, MPtArray* Gidrocoord, MPtArray* Gidroheight);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindNearestRlfPoint(MPoint pt, double dopusk, double* rasst, MPoint* heightpt);
//DESCRIPTION : Function returns distance from point pt up to nearest altitude mark if it is not greater than tolerance 'dopusk'.
//INPUTS : MPoint , double , double pointer, MPoint pointer.
//RETURNS : True.
	bool	  FindNearestRlfPoint(MPoint pt, double dopusk, double* rasst, MPoint* heightpt);

//Operations

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindNearestIzoline(MPoint findpt);
//DESCRIPTION : Constructor.
//INPUTS : MPoint.
//RETURNS : MIzoline pointer.
	MIzoline* FindNearestIzoline(MPoint findpt);
	
//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateObjectList();
//DESCRIPTION : Unites all objects to m_oblist.
//INPUTS : NONE.
//RETURNS : MObjectList pointer.
	MObjectList* CreateObjectList();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ConnectKlassifikator(MKlassifikator* pKls);
//DESCRIPTION : connects qualifier.
//INPUTS : MKlassifikator pointer.
//RETURNS : NONE.
	virtual void ConnectKlassifikator(MKlassifikator* pKls);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::AddObjectsInObjectListByQuadrakod(MObjectList* oblist,MString quadrakod);
//DESCRIPTION : Adds objects in object list by quadrakod.
//INPUTS : MObjectList pointer,MString .
//RETURNS : NONE.
	virtual void AddObjectsInObjectListByQuadrakod(MObjectList* oblist,MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::IsObject(MObject* pObj);
//DESCRIPTION : 
//INPUTS : MObject pointer.
//RETURNS : True.
	virtual bool IsObject(MObject* pObj);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ReplaceObject(MObject* pOldObj, MObject* pNewObj);
//DESCRIPTION : Replaces object.
//INPUTS : MObject pointer, MObject pointer.
//RETURNS : NONE.
	virtual void ReplaceObject(MObject* pOldObj, MObject* pNewObj);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetObjectList(MObjectList* oblist);
//DESCRIPTION : Gets object list.
//INPUTS : MObjectList pointer.
//RETURNS : NONE.
	virtual void GetObjectList(MObjectList* oblist);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateCopyForUndo(MLayer* pLay);
//DESCRIPTION : Creates copy for undo.
//INPUTS : MLayer pointer.
//RETURNS : NONE.
	virtual void CreateCopyForUndo(MLayer* pLay);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ClearObjectList();
//DESCRIPTION : Clears object list.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ClearObjectList();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::DeleteObjects();
//DESCRIPTION : Deletes objects.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void DeleteObjects();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::GetInfoForShpWrite(MStArray& names,MInArray* dlina);
//DESCRIPTION : Returns information for saving to SHP-file.
//INPUTS : MStArray& ,MInArray pointer.
//RETURNS : NONE.
	virtual void GetInfoForShpWrite(MStArray& names,MInArray* dlina);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::Oem_To_Ansi();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Oem_To_Ansi(){};

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::Transliter();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Transliter(){};

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FormRegionForTriangulation(MObjectList* heightpoints, MTrArray* triar, MSgArray* segar,MPtArray* centerar, MDuArray* radar);
//DESCRIPTION : Forms multitude of triangular triangles and segments inside defined
// 				area, excepts triangles and segments, which are situating out of this area.
//INPUTS : MObjectList pointer, MTrArray pointer, MSgArray pointer,MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool FormRegionForTriangulation(MObjectList* heightpoints, MTrArray* triar, MSgArray* segar,
		MPtArray* centerar, MDuArray* radar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::OrientTriangleByClock(MTriangle* pTri);
//DESCRIPTION : Orientates points of triangular triangle by clockwise.
//INPUTS : MTriangle pointer.
//RETURNS : True.
	bool OrientTriangleByClock(MTriangle* pTri);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreateCircle(MTrArray* triar, MPtArray* centerar, MDuArray* radar);
//DESCRIPTION : Creates multitude of described circles for defined multitude of triangles.
//INPUTS : MTrArray pointer, MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool CreateCircle(MTrArray* triar, MPtArray* centerar, MDuArray* radar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindModregionForTript(MHeightPoint* pHpt, MTrArray* triar, MSgArray* segar,MPtArray* centerar, MDuArray* radar, MPtArray* modRegion, MDuArray* heightRegion);
//DESCRIPTION : Selects modification area - simply connected domain -  for defined point 'pt' by multitude
// 				of triangular triangles to include defined point into triangulation.
//INPUTS : MHeightPoint pointer, MTrArray pointer, MSgArray pointer,MPtArray pointer, MDuArray pointer, MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool FindModregionForTript(MHeightPoint* pHpt, MTrArray* triar, MSgArray* segar,
		MPtArray* centerar, MDuArray* radar, MPtArray* modRegion, MDuArray* heightRegion);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::TriangulationInModregion(MHeightPoint* pHpt, MPtArray* modRegion, MDuArray* heightRegion,MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar);
//DESCRIPTION : Forms multitude of triangular triangles and segments inside modified
// 				area by area contour and new(which is included in triangular net altitude mark.
//INPUTS : MHeightPoint pointer, MPtArray pointer, MDuArray pointer,MTrArray pointer, MSgArray pointer, MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool TriangulationInModregion(MHeightPoint* pHpt, MPtArray* modRegion, MDuArray* heightRegion,
		MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindModregionForStructSegment(MPoint pt1, MPoint pt2, MTrArray* triar, MSgArray* segar,MPtArray* centerar, MDuArray* radar, MPtArray* modRegion, MDuArray* heightRegion);
//DESCRIPTION : Selects modification area - simply connected domain -  for defined structural segment by multitude
// 				of triangular triangles to include defined segment into triangulation.
//INPUTS : MPoint , MPoint , MTrArray pointer, MSgArray pointer,MPtArray pointer, MDuArray pointer, MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool FindModregionForStructSegment(MPoint pt1, MPoint pt2, MTrArray* triar, MSgArray* segar,
			MPtArray* centerar, MDuArray* radar, MPtArray* modRegion, MDuArray* heightRegion);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::TriangulationInModregionForStructsg(MPoint pt1, MPoint pt2, MPtArray* modRegion,MDuArray* heightRegion,MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar);
//DESCRIPTION : Forms multitude of triangular triangles and segments inside modified
// 				area by area contour and new(which is included in triangular net structural segment.
//INPUTS : MPoint , MPoint , MPtArray pointer,MDuArray pointer,MTrArray pointer, MSgArray pointer, MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool TriangulationInModregionForStructsg(MPoint pt1, MPoint pt2, MPtArray* modRegion,MDuArray* heightRegion,
			MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindModregionForDeleteHpt(MHeightPoint* pHpt, MPtArray* modRegion, MDuArray* heightRegion);
//DESCRIPTION : Selects modification area - simply connected domain -  for deletable altitude mark by multitude of
// 				triangular triangles for changing multitude of triangular triangles and segments.
//INPUTS : MHeightPoint pointer, MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool FindModregionForDeleteHpt(MHeightPoint* pHpt, MPtArray* modRegion, MDuArray* heightRegion);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::TriangulationInModregionForDeleteHpt(MHeightPoint* pHpt, MPtArray* modRegion,MDuArray* heightRegion);
//DESCRIPTION : Forms multitude of triangular triangles and segments inside modified
// 				area by area contour.
//INPUTS : MHeightPoint pointer, MPtArray pointer,MDuArray pointer.
//RETURNS : True.
	bool TriangulationInModregionForDeleteHpt(MHeightPoint* pHpt, MPtArray* modRegion,MDuArray* heightRegion);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::RemoveIzolines();
//DESCRIPTION : Clears lists of main and additional horizontals and
// 				deletes all main and additional horizontals.
//INPUTS : NONE.
//RETURNS : NONE.
	void RemoveIzolines();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::SetTypeHeightPointsUnrelief();
//DESCRIPTION : Sets type "not reliefable" to all altitude marks.
//INPUTS : NONE.
//RETURNS : NONE.
	void SetTypeHeightPointsUnrelief();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::RemoveObjectsFromOblist(MObjectList* heightpts, MObjectList* konturhpts,MObjectList* resultlist);
//DESCRIPTION : Forms new list, which contains pointers to all objects of first list except
// 				objects, pointers to which are part of second list.
//INPUTS : MObjectList pointer, MObjectList pointer,MObjectList pointer.
//RETURNS : True.
	bool RemoveObjectsFromOblist(MObjectList* heightpts, MObjectList* konturhpts,
												MObjectList* resultlist);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FormHeightsOfPoints(MPtArray* points, MDuArray* heights);
//DESCRIPTION : Forms array of altitudes by coordinates of altitude marks, defined by input array of points.
//INPUTS : MPtArray pointer, MDuArray pointer.
//RETURNS : True.
	bool FormHeightsOfPoints(MPtArray* points, MDuArray* heights);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FormListOfHeightpoints(MPtArray* points, MObjectList* heightpts);
//DESCRIPTION : Forms list of altitude marks with coordinates equal to cooedinates of points of array by array of points.
//INPUTS : MPtArray pointer, MObjectList pointer.
//RETURNS : True.
	bool FormListOfHeightpoints(MPtArray* points, MObjectList* heightpts);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindConvexPolygonForPoints();
//DESCRIPTION : Creates prominent boundary polygon by multitude of altitude marks.
//INPUTS : NONE.
//RETURNS : True.
	bool FindConvexPolygonForPoints();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindStructLine(MPtArray* line);
//DESCRIPTION : Searchs for the structural line, which coinciding with input line,defined by array of points.
//INPUTS : MPtArray pointer.
//RETURNS : int.
	int FindStructLine(MPtArray* line);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FixStructLine(MPtArray* line);
//DESCRIPTION : Attachs structural line.
//INPUTS : MPtArray pointer.
//RETURNS : True.
	bool FixStructLine(MPtArray* line);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FixAllStructLines();
//DESCRIPTION : Attachs structural line of relief layer.
//INPUTS : NONE.
//RETURNS : True.
	bool FixAllStructLines();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::TriangulateHeightPoints();
//DESCRIPTION : Inserts all altitude marks in triangulation if they lay inside relief contour.
//INPUTS : NONE.
//RETURNS : NONE.
	void TriangulateHeightPoints();

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::TriangulationByDeloneInRegion(MPtArray* pReg);
//DESCRIPTION : Builds triangulation of Delone inside the given contour;
// 				thus deletes triangular triangles before procedure inside a contour
// 				and builds new, according to triangulation of Delone.
//INPUTS : MPtArray pointer.
//RETURNS : True.
	bool TriangulationByDeloneInRegion(MPtArray* pReg);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::CreatetriangulationByDelone(MObjectList* heightpoints);
//DESCRIPTION : Triangulation of multitude of altitude marks inside defined boundary contour,
// 				in the vertexes of which situated altitude marks of Delone.
//INPUTS : MObjectList pointer.
//RETURNS : True.
	bool CreatetriangulationByDelone(MObjectList* heightpoints);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);
//DESCRIPTION : Converts all dotted information to another coordinate system.
//INPUTS : MCoo pointer, MCoo pointer.
//RETURNS : NONE.
	virtual void ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);

private:

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindNearestHpoint(MObjectList& oblist, MPoint* point);
//DESCRIPTION : Selects altitude mark from the ,ultitude of altitudes marks, nearest to defined altitude mark.
//INPUTS : MObjectList& , MPoint pointer.
//RETURNS : MHeightPoint pointer.
	MHeightPoint* FindNearestHpoint(MObjectList& oblist, MPoint* point);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindNearestHptForSegment(MObjectList& oblist, MPoint* point1, MPoint* point2);
//DESCRIPTION : Selects altitude mark from multitude of altitudes marks, distance from which up to the end of input segment equal to min.
//INPUTS : MObjectList& , MPoint pointer, MPoint pointer.
//RETURNS : MHeightPoint pointer.
	MHeightPoint* FindNearestHptForSegment(MObjectList& oblist, MPoint* point1, MPoint* point2);

//PACKAGE : MAP.
//FUNCTION : MReliefLayer::FindIzoheightsForSegment(double h1, double h2, MDuArray* izoheights);
//DESCRIPTION : Outputs List of horizontals altitudes, which cross edge of triangular triangle, defined by altitudes of it's edges.
//INPUTS : double , double , MDuArray pointer.
//RETURNS : True.
	bool FindIzoheightsForSegment(double h1, double h2, MDuArray* izoheights);



//#ifdef _DEBUG
//	void AssertValid();
//#endif
};

//PACKAGE : MAP.
//CLASS : MHeightPoint : public MTopobject.
//DESCRIPTION : Altitude marks.
class MHeightPoint : public MTopobject
{
public:

	// Drawing parameter of  altitude mark
	bool m_bVisible;

	// Drawing parameter of  text at altitude mark
	bool m_bTextVisible;

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::MHeightPoint();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MHeightPoint();

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::MHeightPoint(MPoint point, double height = 0);
//DESCRIPTION : Constructor.
//INPUTS : MPoint , double .
//RETURNS : NONE.
	MHeightPoint(MPoint point, double height = 0);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::~MHeightPoint();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MHeightPoint(){};

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::WhatTheObject();
//DESCRIPTION : Returns name of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_HEIGHTPOINT;}

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::GetIerarhy();
//DESCRIPTION : Returns hierarchy of object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MTopobject::GetIerarhy();
									return retVal+" "+ID_HEIGHTPOINT;}

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::ReadBin(MFile* file,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to UTP.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::GetBytes();
//DESCRIPTION : Defines size of object of UTP-file.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::GetHeight();
//DESCRIPTION : Gets point altitude.
//INPUTS : NONE.
//RETURNS : double.
	double			  GetHeight();

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::SetHeight(double height);
//DESCRIPTION : Sets point altitude.
//INPUTS : double.
//RETURNS : True.
	bool			  SetHeight(double height);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::GetNomer();
//DESCRIPTION : Gets number of altitude mark.
//INPUTS : NONE.
//RETURNS : int.
	int				  GetNomer();

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::SetNomer(int nomer);
//DESCRIPTION : Sets number of altitude mark.
//INPUTS : int.
//RETURNS : True.
	bool			  SetNomer(int nomer);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::GetType();
//DESCRIPTION : Gets type of altitude mark.
//INPUTS : NONE.
//RETURNS : MString.
	MString			  GetType();

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::SetType(MString signif);
//DESCRIPTION : Sets type of altitude mark.
//INPUTS : MString.
//RETURNS : True.
	bool			  SetType(MString signif);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void		Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MHeightPoint::DrawText(MPlotDevice* pDevice);
//DESCRIPTION : Draws formulas.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void		DrawText(MPlotDevice* pDevice);

};
//PACKAGE : MAP.
//CLASS	: MFloodZone : public MTopobject.
//DESCRIPTION : Flooding area.
class MFloodZone : public MTopobject
{
  private:
	 MKarta *m_karta;        //  map object
	 MRect zoomRect;         //  search box
	 double step;            //  Step DEM
	 double levelOfWater;    //  excess water level
	 int n,m;                //  dimension of the matrix for the selected site
	 double maxDistance;     // search radius flood zone

	 QProgressBar * progressBar;

 public:

//PACKAGE : MAP.
//FUNCTION : MFloodZone::MFloodZone(MKarta *m_karta, MRect &zoomRect,double & height);
//DESCRIPTION : Constructor.
//INPUTS : MKarta pointer, MRect &,double & .
//RETURNS : NONE.
	 MFloodZone(MKarta *m_karta, MRect &zoomRect,double & height);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::~MFloodZone();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MFloodZone(){delete progressBar;};

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetIerarhy();
//DESCRIPTION : Gets ierarhy.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){return MTopobject::GetIerarhy();};

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetTriWithLessHeight(MPoint * pt1,double height, std::queue< int> &triQueue , std::vector<int> &visitedTri,std::vector<int> &resultTri );
//DESCRIPTION : Creating a list of triangles whose vertices are below the specified height.
//INPUTS : MPoint pointer,double , std::queue< int> & , std::vector<int> &,std::vector<int> &.
//RETURNS : NONE.
	void GetTriWithLessHeight(MPoint * pt1,double height, std::queue< int> &triQueue , std::vector<int> &visitedTri,std::vector<int> &resultTri );

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetFloodZone(MPtArray* piontOfSelTpo, std::vector<int> &resultTri);
//DESCRIPTION : Construction of the flood zone.
//INPUTS : MPtArray pointer, std::vector<int> &.
//RETURNS : NONE.
	void GetFloodZone(MPtArray* piontOfSelTpo, std::vector<int> &resultTri);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::CreatePtArrFloodZone(std::vector<int> &resultTri);
//DESCRIPTION : 
//INPUTS : std::vector<int> &.
//RETURNS : NONE.
	void CreatePtArrFloodZone(std::vector<int> &resultTri);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::SetMaxDistance(double  distance);
//DESCRIPTION : Sets the radius to find the flood zone.
//INPUTS : double.
//RETURNS : NONE.
	void SetMaxDistance(double  distance);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::SetProgress();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : NONE.
	void SetProgress();

//PACKAGE : MAP.
//FUNCTION : MFloodZone::SetMaxProgress(int max);
//DESCRIPTION : 
//INPUTS : int.
//RETURNS : NONE.
	void SetMaxProgress(int max);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetProgressBar();
//DESCRIPTION : Gets ierarhy.
//INPUTS : NONE.
//RETURNS : QProgressBar pointer.
	QProgressBar * GetProgressBar(){return progressBar;};

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetTriByNumber(int number);
//DESCRIPTION : Returns an array of the points of the triangle line of the triangle.
//INPUTS : int.
//RETURNS : MPtArray pointer.
	MPtArray  *GetTriByNumber(int number);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetNumReliefTriForPoint(MPoint *pt, MPoint &pt1, MPoint &pt2, MPoint &pt3, bool &pos);
//DESCRIPTION : Returns triangle relief , which gets the point.
//INPUTS : MPoint pointer, MPoint &, MPoint &, MPoint &, bool &.
//RETURNS : int.
	int GetNumReliefTriForPoint(MPoint *pt, MPoint &pt1, MPoint &pt2, MPoint &pt3, bool &pos);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::SortTriArr(std::vector<int> &resultTri);
//DESCRIPTION : Sorting the list of triangles ascending numbers.
//INPUTS : std::vector<int> &.
//RETURNS : NONE.
	void  SortTriArr(std::vector<int> &resultTri);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::ConvertPoint(MPoint& pt, bool bIn= true);
//DESCRIPTION : Converts point.
//INPUTS : MPoint& , bool .
//RETURNS : MPoint.
	MPoint ConvertPoint(MPoint& pt, bool bIn= true);

//PACKAGE : MAP.
//FUNCTION : MFloodZone::GetHight(MPoint& pt, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);
//DESCRIPTION : Gets hight.
//INPUTS : MPoint& , double pointer, double pointer, double pointer.
//RETURNS : int.
	int GetHight(MPoint& pt, double* hRel = NULL, double* hObj = NULL, double* hCor = NULL);
};


#endif // __RELIEF_H__
