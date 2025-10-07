//PACKAGE : MAP.
//FILE: Topobject.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MTOPOBJECT_H
#define MTOPOBJECT_H
#include "Object.h"
#include "Globals.h"
#include "Formula.h"
#include "privpoints.h"
#include "ZnakSistema.h"
#include "Yaz.h"
#include "Basics.h"
#include "MString.h"


class MSrez;
class MIzoline;
class MHeightPoint;
class MYaz;
class MZnak;
class MRazryv;


// ************************************************
// ****** Class of main objects of the map ********
// ************************************************


// Index of array with inscriptions (m_Komplekt)
#define F_NADPIS		0

// Index of array of snap vectors
#define F_FORMPRIV	    1

// Index of array of contour breaks of topobject
#define F_RAZRYVY	    2


// constants of topobject points types
// point lays on contour
#define KONTUR_POINT	0

// point - point of snap of inscription
#define PODPIS_POINT	1


// constants of edition modes of topobject points
// we can move point everywhere
#define MOVE_POINT_ANYWHERE 0

// we can move point only along contour
#define MOVE_POINT_KONTUR   1

// we cannot move point
#define MOVE_POINT_NOWHERE  2


// Attributes names for stripped objects
#if ENGVERSION == 1 // English version
	#define ID_PLS_ATTRIB_DIAMETR "Strip Object Diametr"
	#define ID_PLS_ATTRIB_HEIGHT  "Strip Object Altitude"
	#define ID_PLS_ATTRIB_WIDTH   "Strip Object Width"
	#define ID_PLS_ATTRIB_THICKNESS "Strip Object Thickness"
	#define IDS_MESSAGE16 "Error While Reading)Can't Read Metric of SXF-File."

#else // Russian version
	#define ID_PLS_ATTRIB_DIAMETR "ПОЛОСНОЙ ОБЪЕКТ ДИАМЕТР"
	#define ID_PLS_ATTRIB_HEIGHT  "ПОЛОСНОЙ ОБЪЕКТ ВЫСОТА"
	#define ID_PLS_ATTRIB_WIDTH   "ПОЛОСНОЙ ОБЪЕКТ ШИРИНА"
	#define ID_PLS_ATTRIB_THICKNESS "ПОЛОСНОЙ ОБЪЕКТ ТОЛЩИНА"
	#define IDS_MESSAGE16 "(Сбой чтения метрики объекта SXF-файла."

#endif

//PACKAGE : MAP.
//CLASS : MTopobject : public MObject.
//DESCRIPTION : Describes topobject class.
	class MTopobject : public MObject
{
public:
	// Object description
	// Object quadrocode by classifier
	MString m_kkod;

	//  pointer ro sign
	MZnak* m_pZnak;

	//  Index of the first part of points  (m_Komplekt)
	int F_TOCHKI;

	//  Index of formula for snapping in active object
	int m_formula_ind;

//PACKAGE : MAP.
//FUNCTION : MTopobject::MTopobject();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MTopobject();

//PACKAGE : MAP.
//FUNCTION : MTopobject::MTopobject(int kol_points);
//DESCRIPTION : Constructor with establishment of points quantity.
//INPUTS : int.
//RETURNS : NONE.
	MTopobject(int kol_points);

	// flags for operational objects
	bool m_status;//true, if object is moving (true by default in constructors) 
	bool m_excited;//true, if object will be increased while drawing
        bool bDrawColor;
	MColor mDrawColor;
        bool bDrawColorZalivka;
        MColor mDrawColorZalivka;

public:

//PACKAGE : MAP.
//FUNCTION : MTopobject::MTopobject();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MTopobject(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MTopobject::MTopobject();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual	~MTopobject();

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetBytes();
//DESCRIPTION : Function of object size definition in UTP-file.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MTopobject::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ReadBin(MFile* file,  int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MTopobject::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

// Functions of identification

//PACKAGE : MAP.
//FUNCTION : MTopobject::WhatTheObject();
//DESCRIPTION : Returns object name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_TOPOBJECT;}

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetIerarhy();
//DESCRIPTION : Returns object hierarchy.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   GetIerarhy(){MString retVal = MObject::GetIerarhy();
									return retVal+" "+ID_TOPOBJECT;}

//PACKAGE : MAP.
//FUNCTION : MTopobject::IsTopobjClass();
//DESCRIPTION : If object is inheritor of 'MTopobject'.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool	  IsTopobjClass() { return true;}

//PACKAGE : MAP.
//FUNCTION : MTopobject::Shade(MString pole,double distofaxe);
//DESCRIPTION : Creates "shadow".
//INPUTS : MString ,double .
//RETURNS : MTopobject pointer.
	virtual MTopobject*  Shade(MString pole,double distofaxe);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Draw1(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	Draw1(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Draw2(MPlotDevice* pDevice, MYaz* yaz);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer, MYaz pointer.
//RETURNS : NONE.
	virtual void	Draw2(MPlotDevice* pDevice, MYaz* yaz);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DrawLine(MYaz* pYaz, MPlotDevice* pDevice, MPtArray* pta);
//DESCRIPTION : Function of drawing.
//INPUTS : MYaz pointer, MPlotDevice pointer, MPtArray pointer.
//RETURNS : NONE.
	virtual void	DrawLine(MYaz* pYaz, MPlotDevice* pDevice, MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DrawZapolnenie(MYaz* pYaz, MPlotDevice* pDevice,MPtArray* pta);
//DESCRIPTION : Function of drawing.
//INPUTS : MYaz pointer, MPlotDevice pointer,MPtArray pointer.
//RETURNS : NONE.
	virtual void	DrawZapolnenie(MYaz* pYaz, MPlotDevice* pDevice,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MTopobject::CanDrawText(MPlotDevice* pDevice);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : True.
	virtual bool	CanDrawText(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DrawText(MPlotDevice* pDevice);
//DESCRIPTION : Draws formulas.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	DrawText(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DrawTracker(MPlotDevice* pDevice,TrackerState state);
//DESCRIPTION : Draws tracker.
//INPUTS : MPlotDevice pointer,TrackerState .
//RETURNS : NONE.
	virtual void DrawTracker(MPlotDevice* pDevice,TrackerState state);

//PACKAGE : MAP.
//FUNCTION : MTopobject::AddVektorPriv(int FIndex,MPoint pt1,MPoint pt2);
//DESCRIPTION : Functions of work with arrays of vectors- and splines-snaps of formulas
// 				adds vector of snap for the formula of number 'FIndex'
// 				also establishes this vector equal (pt1, pt2)..
//INPUTS : int ,MPoint ,MPoint .
//RETURNS : True.
	bool AddVektorPriv(int FIndex,MPoint pt1,MPoint pt2);

//PACKAGE : MAP.
//FUNCTION : MTopobject::AddPriv(int FIndex,MPtArray& pta,unsigned char bType = SIMPLE_PRIVYAZKA);
//DESCRIPTION : 
//INPUTS : int ,MPtArray& ,unsigned char .
//RETURNS : True.
	bool AddPriv(int FIndex,MPtArray& pta,unsigned char bType = SIMPLE_PRIVYAZKA);

//PACKAGE : MAP.
//FUNCTION : MTopobject::AddSplinePriv(int FIndex,MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4);
//DESCRIPTION : Adds spline of snapping wirh coordinates (pt1,pt2,pt3,pt4) for the formula of
// 				number 'FIndex' to array of snap-splines.
//INPUTS : int ,MPoint ,MPoint ,MPoint ,MPoint .
//RETURNS : True.
	bool AddSplinePriv(int FIndex,MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetVektorPriv(int FIndex, int VIndex, MPoint pt1, MPoint pt2);
//DESCRIPTION : Sets snap-vector of formula of number 'FIndex'.
//INPUTS : int , int , MPoint , MPoint .
//RETURNS : True.
	bool SetVektorPriv(int FIndex, int VIndex, MPoint pt1, MPoint pt2);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DeleteVektorPriv(int FIndex, int VIndex);
//DESCRIPTION : Deletes snap-vector of formula of number 'FIndex'.
//INPUTS : int , int .
//RETURNS : True.
	bool DeleteVektorPriv(int FIndex, int VIndex);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DeleteVektorsPriv(int FIndex);
//DESCRIPTION : Deletes all snap-vectors from array of snap-vectors of formulas
// 				for the formulas with number of number FIndex.
//INPUTS : int.
//RETURNS : True.
	bool DeleteVektorsPriv(int FIndex);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetDoubleVektorsPriv(int FIndex, MPtArray* pta);
//DESCRIPTION : Deletes all snap-vectors from array of snap-vectors of formulas
// 				for the formulas with number of number FIndex
// 				Gets all snap-vectors for formula with number 'FIndex'.
//INPUTS : int , MPtArray pointer.
//RETURNS : True.
	bool GetDoubleVektorsPriv(int FIndex, MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ChangeFormulaNomer(int oldnomer, int newnomer);
//DESCRIPTION : Changes formulas numbers from 'oldnumber' on 'newnumber' in arrays of formulas snaps.
//INPUTS : int , int .
//RETURNS : True.
	bool ChangeFormulaNomer(int oldnomer, int newnomer);

//PACKAGE : MAP.
//FUNCTION : MTopobject::HitFormulaTest(MPoint point, double dDostup,int& nomer);
//DESCRIPTION : Test: on which formula mouse cursor indicates?
//  			Serves for topobjects and their inheritors, except diagonals.
//INPUTS : MPoint , double ,int& .
//RETURNS : True.
	bool HitFormulaTest(MPoint point, double dDostup,int& nomer);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetCountPrivs(int FIndex, unsigned char btype);
//DESCRIPTION : Gets quantity of snaps of such type for defined formula.
//INPUTS : int , unsigned char .
//RETURNS : int.
	int GetCountPrivs(int FIndex, unsigned char btype);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DeletePrivs(int FIndex, unsigned char btype);
//DESCRIPTION : Deletes all snaps of such type for defined formula.
//INPUTS : int , unsigned char .
//RETURNS : NONE.
	void DeletePrivs(int FIndex, unsigned char btype);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPriv(int FIndex, int PIndex, unsigned char btype);
//DESCRIPTION : Gets pointer to any snap of any type for any formula.
//INPUTS : int , int , unsigned char .
//RETURNS : MPriv pointer.
	MPriv* GetPriv(int FIndex, int PIndex, unsigned char btype);

//Gets/Sets Name of topographical object
//Name stored in attribute with name "Name"

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetName();
//DESCRIPTION :	Gets object name.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetName();

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetName(MString str);
//DESCRIPTION : Sets object name.
//INPUTS : MString.
//RETURNS : NONE.
	void SetName(MString str);

// Functions of work with CLASSIFIER

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConnectZnak(MZnak* pZnak);
//DESCRIPTION : Connects to sign.
//INPUTS : MZnak pointer.
//RETURNS : True.
	virtual bool ConnectZnak(MZnak* pZnak);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ChangeClass(MString newclass,bool ignoreKKod = true);
//DESCRIPTION : Changes object class.
//INPUTS : MString ,bool .
//RETURNS : NONE.
	virtual void ChangeClass(MString newclass,bool ignoreKKod = true);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConnectToServKls(int typeyaz = F_LIN);
//DESCRIPTION : Connects to service classifier.
//INPUTS : int.
//RETURNS : NONE.
	virtual void ConnectToServKls(int typeyaz = F_LIN);

// Functions of work with BRAKES

//PACKAGE : MAP.
//FUNCTION : MTopobject::OrderRazryvy();
//DESCRIPTION : Sorts array of breaks by first index by increasement
// 				and by distance from the beginning of edge for the breaks, which lay on one edge.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool OrderRazryvy();

//PACKAGE : MAP.
//FUNCTION : MTopobject::DevideOverlastptRazryvy();
//DESCRIPTION : Each break,which goes through null, we splite on two breaks:
// 				The break up to the end of contour and the break from the beginning of contour up to the end of splitted break.
//INPUTS : NONE.
//RETURNS : True.
	bool DevideOverlastptRazryvy();

//PACKAGE : MAP.
//FUNCTION : MTopobject::UnionOverlappedRazryvy();
//DESCRIPTION : Unites all intersected breaks.
//INPUTS : NONE.
//RETURNS : True.
	bool UnionOverlappedRazryvy();

//PACKAGE : MAP.
//FUNCTION : MTopobject::CorrectRazryvsByInsertBeforePoint(int index, MPoint point);
//DESCRIPTION : Corrects array of breaks at addition to the contour of topobject a new point
// 				'point' before the point with index 'index'.
//INPUTS : int , MPoint .
//RETURNS : True.
	bool CorrectRazryvsByInsertBeforePoint(int index, MPoint point);

//PACKAGE : MAP.
//FUNCTION : MTopobject::CorrectRazryvsByInsertAfterPoint(int index, MPoint point);
//DESCRIPTION : Corrects array of breaks at addition to the contour of topobject a new point
// 				'point' after the point with index 'index'.
//INPUTS : int , MPoint .
//RETURNS : True.
	bool CorrectRazryvsByInsertAfterPoint(int index, MPoint point);

//PACKAGE : MAP.
//FUNCTION : MTopobject::CorrectRazryvsByDeletePoint(int index);
//DESCRIPTION : Corrects array of breaks at deleting from topobject contour poiint with index 'index'.
//INPUTS : int.
//RETURNS : True.
	bool CorrectRazryvsByDeletePoint(int index);

//PACKAGE : MAP.
//FUNCTION : MTopobject::CorrectRazryvsByMovePoint(int index, MPoint point);
//DESCRIPTION : Corrects array of breaks at moving of the point of contour topobject with index 'index'
// 				to point 'point'.
//INPUTS : int , MPoint .
//RETURNS : True.
	bool CorrectRazryvsByMovePoint(int index, MPoint point);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConstructLomlineByRazryv(MRazryv* razryv, MPtArray* pLomline);
//DESCRIPTION : For the break forms equal to it broken line.
//INPUTS : MRazryv pointer, MPtArray pointer.
//RETURNS : True.
	bool ConstructLomlineByRazryv(MRazryv* razryv, MPtArray* pLomline);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConstructRazryvForCurrentFormula(MPlotDevice* pDevice, MPoint pt, int nomer, MRazryv* razryv);
//DESCRIPTION : Forms break in a contour of topobject for the current formula.
//INPUTS : MPlotDevice pointer, MPoint , int , MRazryv pointer.
//RETURNS : True.
	bool ConstructRazryvForCurrentFormula(MPlotDevice* pDevice, MPoint pt, int nomer, MRazryv* razryv);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetFormulaOnKontur(MPlotDevice* pDevice,MPoint pt, int nomer);
//DESCRIPTION : Forms break in a contour of topobject in array of breaks for the current formula
// 		and snap-vector for the current formula in array of snaps.
//INPUTS : MPlotDevice pointer,MPoint , int .
//RETURNS : True.
	bool SetFormulaOnKontur(MPlotDevice* pDevice,MPoint pt, int nomer);

//PACKAGE : MAP.
//FUNCTION : MTopobject::AddRazryv(MPoint ptfirst, MPoint ptlast, int indfirst, int indlast);
//DESCRIPTION : Adds new break to the array of breaks.
//INPUTS : MPoint , MPoint , int , int .
//RETURNS : True.
	bool AddRazryv(MPoint ptfirst, MPoint ptlast, int indfirst, int indlast);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConvertToTopobject(MString quadrakod = "");
//DESCRIPTION : Function of conversion into another classes.
//INPUTS : MString.
//RETURNS : MTopobject pointer.
	virtual MTopobject*		ConvertToTopobject(MString quadrakod = "");

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConvertToIzoline(MString quadrakod = "");
//DESCRIPTION : Function of conversion into another classes.
//INPUTS : MString.
//RETURNS : MIzoline pointer.
	virtual MIzoline*		ConvertToIzoline(MString quadrakod = "");

//PACKAGE : MAP.
//FUNCTION : MTopobject::ConvertToHeightpt(MString quadrakod = "");
//DESCRIPTION : Function of conversion into another classes.
//INPUTS : MString.
//RETURNS : MHeightPoint pointer.
	virtual MHeightPoint*	ConvertToHeightpt(MString quadrakod = "");

//PACKAGE : MAP.
//FUNCTION : MTopobject::RecalcBounds();
//DESCRIPTION : Recalculates bound.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool      RecalcBounds();

// Functions of work with ARRAY OF POINTS of topobject

//PACKAGE : MAP.
//FUNCTION : MTopobject::AddPoint(const MPoint& geopoint);
//DESCRIPTION : Adds point to active object.
//INPUTS : const MPoint&.
//RETURNS : NONE.
	virtual void	  AddPoint(const MPoint& geopoint);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetPoints(MPtArray& geopoints);
//DESCRIPTION : Sets object points.
//INPUTS : MPtArray&.
//RETURNS : NONE.
	virtual void	  SetPoints(MPtArray& geopoints);

//PACKAGE : MAP.
//FUNCTION : MTopobject::InsertPointAfter(int num, const MPoint& geopoint);
//DESCRIPTION : Inserts point 'pt' into array of points pta after a point withindex 'index'.
//INPUTS : int , const MPoint& .
//RETURNS : NONE.
	virtual void	  InsertPointAfter(int num, const MPoint& geopoint);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DeletePoint(int num/*, MView* pView*/);
//DESCRIPTION : Deletes point.
//INPUTS : int.
//RETURNS : NONE.
	virtual void	  DeletePoint(int num/*, MView* pView*/);

//PACKAGE : MAP.
//FUNCTION : MTopobject::InsertPoint(int num, const MPoint& geopoint);
//DESCRIPTION : Inserts a point into active object.
//INPUTS : int , const MPoint& .
//RETURNS : NONE.
	virtual void	  InsertPoint(int num, const MPoint& geopoint);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DeletePoint(MPtArray* pta, int index);
//DESCRIPTION : Deletes point by it's number
//  			Deletes point from array 'pta' with index 'index'.
//INPUTS : MPtArray pointer, int .
//RETURNS : NONE.
	void			  DeletePoint(MPtArray* pta, int index);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPointForEdit(MPoint point, double dopusk, int& regim, int& kind);
//DESCRIPTION : Gets point for edition.
//INPUTS : MPoint , double , int& , int& .
//RETURNS : MPoint pointer.
	virtual MPoint*   GetPointForEdit(MPoint point, double dopusk, int& regim, int& kind);

//PACKAGE : MAP.
//FUNCTION : MTopobject::MovePointTo(MPoint* whatpoint ,int kind, MPoint whereput, int nomrebra, double nachalosdvig, double rebrosdvig);
//DESCRIPTION :  Moves point.
//INPUTS : MPoint pointer ,int , MPoint , int , double , double .
//RETURNS : NONE.
	virtual void      MovePointTo(MPoint* whatpoint ,int kind, MPoint whereput, int nomrebra, double nachalosdvig, double rebrosdvig);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DeletePt(MPoint* whatpoint, int kind);
//DESCRIPTION : Deletes point.
//INPUTS : MPoint pointer, int .
//RETURNS : NONE.
	virtual void      DeletePt(MPoint* whatpoint, int kind);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetArrayOfPoint(int &number);
//DESCRIPTION : Gets array of object points.
//INPUTS : int.
//RETURNS : MPtArray pointer.
	MPtArray*         GetArrayOfPoint(int &number);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ReverseOrder();
//DESCRIPTION : Reverses point order.
//INPUTS : NONE.
//RETURNS : NONE.
	void ReverseOrder();

	// Functions, used in work with objects

//PACKAGE : MAP.
//FUNCTION : MTopobject::DistanceToVershina(MPoint& gpt,MPoint& resultpt,int& nomrebra);
//DESCRIPTION : Searchs for the nearest vertex to the current point.
//INPUTS : MPoint& ,MPoint& ,int& .
//RETURNS : double.
	double DistanceToVershina(MPoint& gpt,MPoint& resultpt,int& nomrebra);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetLengthBetweenPointsByPerimeter(MPoint pt1,MPoint pt2,int iFirstPointSide = -1, int iSecondPointSide = -1);
//DESCRIPTION : Gets distance betweem 2 points by first contour.
//INPUTS : MPoint ,MPoint ,int , int .
//RETURNS : double.
	double GetLengthBetweenPointsByPerimeter(MPoint pt1,MPoint pt2,int iFirstPointSide = -1, int iSecondPointSide = -1);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPointByPerimeter(double dLengthFromStart,MPoint &pt,int &iSideNum);
//DESCRIPTION : Gets point which recieved distance on perimeter from the beginning as parameter.
//INPUTS : double ,MPoint &,int &.
//RETURNS : True.
	bool GetPointByPerimeter(double dLengthFromStart,MPoint &pt,int &iSideNum);

//PACKAGE : MAP.
//FUNCTION : MTopobject::MovePointToTouchFrontier(MPoint& gpt, double* dopusk = NULL);
//DESCRIPTION : Finds point in topobject, nearest to defined.
//INPUTS : MPoint& , double pointer.
//RETURNS : int.
	int               MovePointToTouchFrontier(MPoint& gpt, double* dopusk = NULL);

//PACKAGE : MAP.
//FUNCTION : MTopobject:: MovePointToTouchFrontierExept(MPoint& gpt, int exeptpt);
//DESCRIPTION : Finds point in topobject, nearest to defined, except point with any number.
//INPUTS : MPoint& , int .
//RETURNS : int.
	int               MovePointToTouchFrontierExept(MPoint& gpt, int exeptpt);

//PACKAGE : MAP.
//FUNCTION : MTopobject::FindDistanceFromPoint(MPoint& gpt, MPoint& resultpt,int* nomrebra);
//DESCRIPTION : Returns distance from defined point up to the centre  of our topobject.
//INPUTS : MPoint& , MPoint& ,int pointer.
//RETURNS : double.
	virtual double	  FindDistanceFromPoint(MPoint& gpt, MPoint& resultpt,int* nomrebra);

//PACKAGE : MAP.
//FUNCTION : MTopobject::FindDistanceFromPoint(MPoint& gpt);
//DESCRIPTION : Finds distance from point.
//INPUTS : MPoint&.
//RETURNS : double.
	virtual double	  FindDistanceFromPoint(MPoint& gpt);

//PACKAGE : MAP.
//FUNCTION : MTopobject::FindNearestIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto);
//DESCRIPTION : Finds point of intersection of our topobject with object established by two points linept1,linept2.
//INPUTS : MPoint , MPoint , MPoint& , double& .
//RETURNS : int.
	int				  FindNearestIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto);

//PACKAGE : MAP.
//FUNCTION : MTopobject::FindIntersectsWithSegment(MPoint sgpt1, MPoint sgpt2);
//DESCRIPTION : Finds point of intersection of our topobject with object established by two points sgpt1,sgpt2.
//INPUTS : MPoint , MPoint .
//RETURNS : MPtArray pointer.
	MPtArray*		  FindIntersectsWithSegment(MPoint sgpt1, MPoint sgpt2);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetFarPoint(MPoint* pt1, MPoint* pt2);
//DESCRIPTION : Finds distant point if figure from straight line (pt1,pt2).
//INPUTS : MPoint pointer, MPoint pointer.
//RETURNS : MPoint.
	MPoint			  GetFarPoint(MPoint* pt1, MPoint* pt2);

//PACKAGE : MAP.
//FUNCTION : MTopobject::CrossLineFig(MPoint* pt1, MPoint* pt2, MPtArray* pta);
//DESCRIPTION : MPtArray* - points, found at intersection of figure with straight line (pt1,pt2).
//INPUTS : MPoint pointer, MPoint pointer, MPtArray pointer.
//RETURNS : True.
	bool			  CrossLineFig(MPoint* pt1, MPoint* pt2, MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MTopobject::IsInnerPoint(MPoint* pt, int* npart);
//DESCRIPTION : Is inner point?
//INPUTS : MPoint pointer, int pointer.
//RETURNS : True.
	bool			  IsInnerPoint(MPoint* pt, int* npart);

//PACKAGE : MAP.
//FUNCTION : MTopobject::IsInnerSegment(MPoint* pt1, MPoint* pt2, int* npart);
//DESCRIPTION : Is inner segment?
//INPUTS : MPoint pointer, MPoint pointer, int pointer.
//RETURNS : True.
	bool			  IsInnerSegment(MPoint* pt1, MPoint* pt2, int* npart);

//PACKAGE : MAP.
//FUNCTION : MTopobject::IsInnerTopobject(MTopobject* pTpo);
//DESCRIPTION : Is inner topobject?
//INPUTS : MTopobject pointer.
//RETURNS : True.
	bool			  IsInnerTopobject(MTopobject* pTpo);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Cut(int n);
//DESCRIPTION : Splits array into two parts.
//INPUTS : int.
//RETURNS : MObject pointer.
	virtual MObject*  Cut(int n);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Cut(int from, int to);
//DESCRIPTION : Splits array into two parts.
//INPUTS : int , int .
//RETURNS : MObject pointer.
	virtual MObject*  Cut(int from, int to);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Add(MTopobject* mpt);
//DESCRIPTION : Addition of topobject to topobject not as like a part.
//INPUTS : MTopobject pointer.
//RETURNS : NONE.
	virtual void      Add(MTopobject* mpt);

//PACKAGE : MAP.
//FUNCTION : MTopobject::AddPart(MTopobject* mpt);
//DESCRIPTION : Addition of topobject to topobject as a part.
//INPUTS : MTopobject pointer.
//RETURNS : NONE.
	virtual void      AddPart(MTopobject* mpt);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Sew(MTopobject* pTpo,bool tohead);
//DESCRIPTION : Separation of defined parts of topobject.
//INPUTS : MTopobject pointer,bool .
//RETURNS : True.
	bool              Sew(MTopobject* pTpo,bool tohead);

//PACKAGE : MAP.
//FUNCTION : MTopobject::Area();
//DESCRIPTION : Area.
//INPUTS : NONE.
//RETURNS : double.
	double            Area();

//PACKAGE : MAP.
//FUNCTION : MTopobject::OrientInverse();
//DESCRIPTION : Orients inverse.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool      OrientInverse();  

//PACKAGE : MAP.
//FUNCTION : MTopobject::OrientLeft();
//DESCRIPTION : Orients left.
//INPUTS : NONE.
//RETURNS : True.	
	bool              OrientLeft();  

//PACKAGE : MAP.
//FUNCTION : MTopobject::OrientRight();
//DESCRIPTION : Orients right.
//INPUTS : NONE.
//RETURNS : True.
	bool              OrientRight();  	

//utilites

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPointsNumber();
//DESCRIPTION : Gets points number.
//INPUTS : NONE.
//RETURNS : int.
	int			      GetPointsNumber() ;

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPartsNumber();
//DESCRIPTION : Gets parts number.
//INPUTS : NONE.
//RETURNS : int.
	int               GetPartsNumber();

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetArea();
//DESCRIPTION : Gets area.
//INPUTS : NONE.
//RETURNS : double.
	double            GetArea();

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPerimeter();
//DESCRIPTION : Gets perimeter.
//INPUTS : NONE.
//RETURNS : double.
	double			  GetPerimeter();

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetOrientation();
//DESCRIPTION : Gets orientation.
//INPUTS : NONE.
//RETURNS : int.
	int               GetOrientation();

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetZazor();
//DESCRIPTION : Gets Zazor.
//INPUTS : NONE.
//RETURNS : double.
	double            GetZazor();

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPartPointsNumber(int ii);
//DESCRIPTION : Gets part points number.
//INPUTS : int.
//RETURNS : int.
	int               GetPartPointsNumber(int ii);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPartArea(int ii);
//DESCRIPTION : Gets part area.
//INPUTS : int.
//RETURNS : double.
	double            GetPartArea(int ii);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPartPerimeter(int ii);
//DESCRIPTION : Gets part perimeter.
//INPUTS : int.
//RETURNS : double.
	double            GetPartPerimeter(int ii);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPartOrientation(int ii);
//DESCRIPTION : Gets part orientation.
//INPUTS : int.
//RETURNS : int.
	int               GetPartOrientation(int ii);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPartZazor(int ii);
//DESCRIPTION : Gets part Zazor.
//INPUTS : int.
//RETURNS : double.
	double            GetPartZazor(int ii);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPoint(int i);
//DESCRIPTION : Gets point.
//INPUTS : int.
//RETURNS : MPoint.
	MPoint			  GetPoint(int i) ;

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetpPoint(int i);
//DESCRIPTION : Gets pointer of point.
//INPUTS : int.
//RETURNS : MPoint pointer.
	MPoint*			  GetpPoint(int i);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetPoint(int i, MPoint & p);
//DESCRIPTION : Sets point.
//INPUTS : int , MPoint & .
//RETURNS : NONE.
	void 			SetPoint(int i, MPoint & p);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetPointArray();
//DESCRIPTION : Gets point array.
//INPUTS : NONE.
//RETURNS : MPtArray pointer.
    MPtArray*         GetPointArray() const;

// FUNCTIONS OF WORK WITH CHARACTERISTICS VALUES IN TOPOBJECT

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetDataPole(unsigned short nom);
//DESCRIPTION : Returns value of characteristic by new characteristic name
// 				or empty line, if entrance number 'num' is not in array of characteristics.
//INPUTS : unsigned short.
//RETURNS : MString.
	MString           GetDataPole(unsigned short nom);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetDataPoleByName(MString polename);
//DESCRIPTION : Returns characteristic value by it's name
// 				or empty line, if input characteristic is not in array of characteristics.
//INPUTS : MString.
//RETURNS : MString.
	MString	   		  GetDataPoleByName(MString polename);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetDataPole(MString data, unsigned short polekod);
//DESCRIPTION : Sets characteristic value by characteristic code.
//INPUTS : MString , unsigned short .
//RETURNS : True.
	bool			  SetDataPole(MString data, unsigned short polekod);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetDataPoleByName(MString data, MString polename);
//DESCRIPTION : Sets characteristic value by characteristic name.
//INPUTS : MString , MString .
//RETURNS : True.
	bool			  SetDataPoleByName(MString data, MString polename);

//PACKAGE : MAP.
//FUNCTION : MTopobject::IsPole(MString polename);
//DESCRIPTION : Cheks if in array of available codes exists code equal to input characteristic.
//INPUTS : MString.
//RETURNS : True.
	bool			  IsPole(MString polename);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetIndexOfFormula(MString FormulaName);
//DESCRIPTION :  Returns index of formula of name 'FormulaName'.
//INPUTS : MString.
//RETURNS : int.
	virtual int		GetIndexOfFormula(MString FormulaName);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetDoubleDataPoleByName(MString str);
//DESCRIPTION : Gets numberic value of attribute.
//INPUTS : MString.
//RETURNS : double.
	virtual	double    GetDoubleDataPoleByName(MString str);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetDoubleDataPole(unsigned short polekod,double& val);
//DESCRIPTION : Gets numberic value if exists.
//INPUTS : unsigned short ,double& .
//RETURNS : True.
	virtual bool	  GetDoubleDataPole(unsigned short polekod,double& val);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ObtainMassivStrok(unsigned short nom, MStArray* out);
//DESCRIPTION : Gets array of strings.
//INPUTS : unsigned short , MStArray pointer.
//RETURNS : NONE.
	void			  ObtainMassivStrok(unsigned short nom, MStArray* out);

//PACKAGE : MAP.
//FUNCTION : MTopobject::GetShadePtArray(MPtArray* pta,double distofaxe);
//DESCRIPTION : Gets "shadow".
//INPUTS : MPtArray pointer,double .
//RETURNS : MPtArray pointer.
	MPtArray*		  GetShadePtArray(MPtArray* pta,double distofaxe);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetYazType(int type = F_LIN, int nom = 0);
//DESCRIPTION : Sets description language type.
//INPUTS : int , int .
//RETURNS : NONE.
	void			  SetYazType(int type = F_LIN, int nom = 0);

//PACKAGE : MAP.
//FUNCTION : MTopobject::SetYaz(MYaz* m_pYazToSet, int nom = 0);
//DESCRIPTION : Sets new description language.
//INPUTS : MYaz pointer, int .
//RETURNS : True.
	bool			  SetYaz(MYaz* m_pYazToSet, int nom = 0);

//PACKAGE : MAP.
//FUNCTION : MTopobject::DesignMaxRamkaForGrifKarta(MString grif, MString name, int mastab, MRect mapbounds);
//DESCRIPTION : 
//INPUTS : MString , MString , int , MRect .
//RETURNS : NONE.
	void			  DesignMaxRamkaForGrifKarta(MString grif, MString name, int mastab, MRect mapbounds);

//PACKAGE : MAP.
//FUNCTION : MTopobject::ReadSxfPoints( FILE* FileSxf,unsigned short int NotationDimension, unsigned short int MetricElementType,unsigned short int MetricElementSize, unsigned short int PointNumber,bool revXY);
//DESCRIPTION : Reads points of object or subobject from SXF-file.
//INPUTS : FILE pointer,unsigned short int , unsigned short int , unsigned short int , unsigned short int , bool .
//RETURNS : True.
	bool ReadSxfPoints( FILE* FileSxf,
						unsigned short int NotationDimension, // dimension
						unsigned short int MetricElementType, // type
						unsigned short int MetricElementSize, // size
						unsigned short int PointNumber,       // points quantity
						bool revXY);                          // backspacing of coordinates

    //PACKAGE : MAP.
    //FUNCTION : MTopobject::WriteSxfPoints( FILE* FileSxf,bool revXY);
    //DESCRIPTION : Writes points of object or subobject from SXF-file.
    //INPUTS : FILE pointer, bool .
    //RETURNS : True.
    virtual bool WriteSxfPoints(FILE *FileSxf, const QJsonObject &classificalCode,
                                  MKartaInterface  * pKI,bool revXY)
      {Q_UNUSED(FileSxf);Q_UNUSED(classificalCode);Q_UNUSED(revXY);Q_UNUSED(pKI);return true;} // backspacing of coordinates
    short GetSubObjectCount();// подсчет вложенных контуров(дырок)


//PACKAGE : MAP.
//FUNCTION : MTopobject::GEOM(float value);
//DESCRIPTION : Gets value dimension factor of classifier.
//INPUTS : float.
//RETURNS : double.
	double GEOM(float value){if(m_pZnak) return ((double)m_pZnak->GetMasstKoeff()*value);
								else return (double)value;}
//PACKAGE : MAP.
//FUNCTION : MTopobject::REV_GEOM(float &value,double dVal);
//DESCRIPTION : Gets value scale factor of classifier.
//INPUTS : float &value,double dVal.
//RETURNS : NONE.
	void REV_GEOM(float &value,double dVal){if(m_pZnak) value = (float)(dVal/m_pZnak->GetMasstKoeff());
								else value = (float)dVal;
	};

//PACKAGE : MAP.
//FUNCTION : MTopobject::RotateShriftNastr(double dAngle);
//DESCRIPTION : Rotation of correctly adjusted font shear.
//INPUTS : double.
//RETURNS : NONE.
	void RotateShriftNastr(double dAngle);

};
#endif //MTOPOBJECT_H
