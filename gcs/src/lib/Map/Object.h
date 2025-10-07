//PACKAGE : MAP.
//FILE: Object.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MOBJECT_H
#define MOBJECT_H

// TITLE FOR CLASS MObject

#include <list>
#include "Array.h"
#include "Coo.h"
#include "MString.h"

//===============================================================================
// CONSTANTS
//===============================================================================
#define ID_DBINITSIZE	3	//  Beginning dimension of bd-array
#define ID_DBINITSTEP	1	//  Beginning increasing step of bd-array
#define ID_KEY			0	//  Line number of bd-array, which contain key
#define ID_TXT			1	//  Line number of bd-array, which contain text
#define ID_DAT			2	//  Line number of bd-array, which contain data

class MKarta;
class MLayer;
class MZnak;
class MObject;
class MPlotDevice;

class MKartaInterface;
// List of available statuses of tracker
enum    TrackerState { normal, complex, simple, signale, redline, points, line};


class MObject;

//PACKAGE : MAP.
//CLASS	: MObjectList : public std::list<MObject*>.
//DESCRIPTION : List of objects.
class MObjectList : public std::list<MObject*>
{
public:

//PACKAGE : MAP.
//FUNCTION : MObjectList::Serialize(MString strFileName, bool bCopy);
//DESCRIPTION : Redefining of our own serialization.
//INPUTS : MString , bool .
//RETURNS : NONE.
	void Serialize(MString strFileName, bool bCopy);
};

// Direct iterator by objects list
typedef std::list<MObject*>::iterator OBITER;
// Direct const_iterator by objects list
typedef std::list<MObject*>::const_iterator CONSTOBITER;

// Reverse iterator by objects list
typedef	std::list<MObject*>::reverse_iterator REVOBITER;

//PACKAGE : MAP.
//CLASS	: MObject.
//DESCRIPTION : Class object - describes standart map object.
//				This class is parent for almost all of remains objects	which can place on the map .
class MObject
{
protected:

//PACKAGE : MAP.
//FUNCTION : MObject::MObject();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MObject();

public:
	double m_UID;				// < unique object number
	double m_UID_master;		// < unique number of main object

	unsigned int m_editstat;	// < status of object edition

    //  attributes
	MStArray m_dbarray;			// < Contains imported object data (from DBF)

	// spatial data...
	MRect     m_bounds;			// < Bounding rectangle

	MLayer*     m_pLay;			// < index of layer

	MArArray* m_Komplekt;		// < Pointer to own object data

	int m_trans_type;			// < transparency type (of background) // added by back

	bool m_selected;			// < flag of object selection

	bool m_enabled;				// < access allowing (non using)

	MColor m_sel_color;			// < object color, if it is selected

public:

//PACKAGE : MAP.
//FUNCTION : MObject::MObject(const MRect& position);
//DESCRIPTION : Constructor.
//INPUTS : const MRect&.
//RETURNS : NONE.
	MObject(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MObject::~MObject();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MObject();

//PACKAGE : MAP.
//FUNCTION : MObject::GetKarta();
//DESCRIPTION : Gets pointer to map.
//INPUTS : NONE.
//RETURNS : MKarta pointer.
	MKarta*		GetKarta();

//PACKAGE : MAP.
//FUNCTION : MObject::GetLayer();
//DESCRIPTION : Gets pointer to layer.
//INPUTS : NONE.
//RETURNS : MLayer pointer.
	MLayer*		GetLayer(){return m_pLay;};

//PACKAGE : MAP.
//FUNCTION : MObject::Createstruktura();
//DESCRIPTION : Creates correct structure of all it's members.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Createstruktura(){};

//PACKAGE : MAP.
//FUNCTION : MObject::Scale(double K, double XC, double YC);
//DESCRIPTION :  Makes sclae transformation.
//INPUTS : double , double , double .
//RETURNS : NONE.
	virtual void Scale(double K, double XC, double YC);

//PACKAGE : MAP.
//FUNCTION : MObject::Rotate(double A, double B, double C1, double C2);
//DESCRIPTION : Makes sclae transformation.
//INPUTS : double , double , double , double .
//RETURNS : NONE.
	virtual void Rotate(double A, double B, double C1, double C2);

//PACKAGE : MAP.
//FUNCTION : MObject:: ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : REads array from NUX-file.
//INPUTS : FILE pointer,  int .
//RETURNS : False.
	virtual bool  ReadBin(FILE* h_nuxfile,  int version);//  {return false;}

//PACKAGE : MAP.
//FUNCTION : MObject:: ReadBin(MFile* file,  int version);
//DESCRIPTION : REads array from NUX-file.
//INPUTS : MFile pointer,  int .
//RETURNS : False.
	virtual bool  ReadBin(MFile* file,  int version);//  {return false;}

//PACKAGE : MAP.
//FUNCTION : MObject::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves array to NUX-file.
//INPUTS : FILE pointer,  int .
//RETURNS : False.
	virtual bool  WriteBin(FILE* h_nuxfile,  int version);// {return false;}

//PACKAGE : MAP.
//FUNCTION : MObject::GetHandleCount();
//DESCRIPTION : How many points in a marking?
//INPUTS : NONE.
//RETURNS : int.
	virtual int      GetHandleCount();

//PACKAGE : MAP.
//FUNCTION : MObject::GetHandle(int nHandle);
//DESCRIPTION :  Returns geocoordinates of n-th point if marking.
//INPUTS : int.
//RETURNS : MPoint.
	virtual MPoint	 GetHandle(int nHandle);

//PACKAGE : MAP.
//FUNCTION : MObject::GetBytes();
//DESCRIPTION : Returns full array size to UTP-format.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MObject::GetChildList(MObjectList& utlist);
//DESCRIPTION :  Returns list of subordinate objects.
//INPUTS : MObjectList&.
//RETURNS : NONE.
	virtual void      GetChildList(MObjectList& utlist);

//  Operations

//PACKAGE : MAP.
//FUNCTION : MObject::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Draws layer objects.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MObject::Draw1(MPlotDevice* pDevice);
//DESCRIPTION : Simplified drawing of layer objects.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void Draw1(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MObject::DrawText(MPlotDevice* pDevice);
//DESCRIPTION : Outputs text.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void DrawText(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MObject::DrawTracker(MPlotDevice* pDevice,TrackerState state);
//DESCRIPTION :  Draws inscription marking.
//INPUTS : MPlotDevice pointer,TrackerState .
//RETURNS : NONE.
	virtual void DrawTracker(MPlotDevice* pDevice,TrackerState state);

//PACKAGE : MAP.
//FUNCTION : MObject::MoveTo(MPoint& delta);
//DESCRIPTION :  Moves active object.
//INPUTS : MPoint&.
//RETURNS : NONE.
	virtual void MoveTo(MPoint& delta);

//PACKAGE : MAP.
//FUNCTION : MObject::MoveTo(double distance, double azimut);
//DESCRIPTION :  Moves active object.
//INPUTS : double , double .
//RETURNS : NONE.
	virtual void MoveTo(double distance, double azimut);

//PACKAGE : MAP.
//FUNCTION : MObject::HitTest(MPoint point, double dDostup,MPtArray*& ptarr,int& index);
//DESCRIPTION :  Test	: Where specifies the cursor of the mouse?
//INPUTS : MPoint , double ,MPtArray*& ,int& .
//RETURNS : True.
	virtual bool HitTest(MPoint point, double dDostup,MPtArray*& ptarr,int& index);

//PACKAGE : MAP.
//FUNCTION : MObject::InsertPointBefore(MPoint pt,MPtArray* pta, int index);
//DESCRIPTION : Inserts point 'pt' into points array 'pta' before a point with index 'index'.
//INPUTS : MPoint ,MPtArray pointer, int .
//RETURNS : NONE.
	virtual void InsertPointBefore(MPoint pt,MPtArray* pta, int index);

//PACKAGE : MAP.
//FUNCTION : MObject::InsertPointAfter(MPoint pt,MPtArray* pta, int index);
//DESCRIPTION : Inserts point pt into points array 'pta' after a point with index 'index'.
//INPUTS : MPoint ,MPtArray pointer, int .
//RETURNS : NONE.
	virtual void InsertPointAfter(MPoint pt,MPtArray* pta, int index);

//PACKAGE : MAP.
//FUNCTION : MObject::DeletePoint(MPtArray* pta, int index);
//DESCRIPTION : Deletes point from active object.
//INPUTS : MPtArray pointer, int .
//RETURNS : NONE.
	virtual void DeletePoint(MPtArray* pta, int index);

//PACKAGE : MAP.
//FUNCTION : MObject::AddPoint(MPoint pt,MPtArray* pta);
//DESCRIPTION : Adds point to active layer.
//INPUTS : MPoint ,MPtArray pointer.
//RETURNS : NONE.
	virtual void AddPoint(MPoint pt,MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION : MObject::Intersects(const MPoint& point);
//DESCRIPTION : Test: If defined point is inside our object?
//INPUTS : const MPoint&.
//RETURNS : True.
	virtual bool Intersects(const MPoint& point);

//PACKAGE : MAP.
//FUNCTION : MObject::Intersects(const MRect& Rect);
//DESCRIPTION : Test: If defined point is inside our object?
//INPUTS : const MRect& .
//RETURNS : True.
	virtual bool Intersects(const MRect& Rect);

//PACKAGE : MAP.
//FUNCTION : MObject::FindDistanceFromPoint(MPoint& gpt);
//DESCRIPTION : Returns distance from defined point till the CONTOUR of our topobject.
//INPUTS : MPoint&.
//RETURNS : double.
	virtual double	FindDistanceFromPoint(MPoint& gpt);

//PACKAGE : MAP.
//FUNCTION : MObject::FindNearestIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto);
//DESCRIPTION : Finds point of intersection of our topobject with straight line which is defined by 2 points linept1,linept2.
//INPUTS : MPoint , MPoint , MPoint& , double& .
//RETURNS : int.
	virtual int     FindNearestIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto);

//PACKAGE : MAP.
//FUNCTION : MObject::RecalcBounds();
//DESCRIPTION : Refreshs bounding rectangle.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool RecalcBounds();

//PACKAGE : MAP.
//FUNCTION : MObject::SetMbounds(MRect m_userRect);
//DESCRIPTION : Setting m_bounds.
//INPUTS : MRect.
//RETURNS : None.
    void SetMbounds(MRect m_userRect);

//FUNCTION : MObject::Clone();
//DESCRIPTION :  Creates object copy.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject* Clone();

//PACKAGE : MAP.
//FUNCTION : MObject::Invalidate();
//DESCRIPTION : Refreshs an object.
//INPUTS : NONE.
//RETURNS : NONE.
	void         Invalidate();

//PACKAGE : MAP.
//FUNCTION : MObject::WhatTheObject();
//DESCRIPTION : Returns object name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheObject() { return "";}

//PACKAGE : MAP.
//FUNCTION : MObject::GetIerarhy();
//DESCRIPTION : Returns object hierarchy.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetIerarhy() {return "";};

//PACKAGE : MAP.
//FUNCTION : MObject::MasterClone(MObject* pClone);
//DESCRIPTION : Makes all neccessary operations for cloning.
//INPUTS : MObject pointer.
//RETURNS : NONE.
	void MasterClone(MObject* pClone);

//PACKAGE : MAP.
//FUNCTION : MObject::LinPr();
//DESCRIPTION : Linear transformations.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void LinPr();

//PACKAGE : MAP.
//FUNCTION : MObject::LinPrKompoz();
//DESCRIPTION :
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void LinPrKompoz();

//PACKAGE : MAP.
//FUNCTION : MObject::LinPrByCoef();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void LinPrByCoef();

//PACKAGE : MAP.
//FUNCTION : MObject::ConvertToMaket();
//DESCRIPTION : Converts into model.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ConvertToMaket();

//PACKAGE : MAP.
//FUNCTION : MObject::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);
//DESCRIPTION : Converts to another coordinate system.
//INPUTS : MCoo pointer, MCoo pointer.
//RETURNS : NONE.
	virtual void ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);

//PACKAGE : MAP.
//FUNCTION : MObject::IsTopobjClass();
//DESCRIPTION : If current object is inheritor of 'MTopobject'.
//INPUTS : NONE.
//RETURNS : False.
	virtual bool IsTopobjClass() { return false;}

//PACKAGE : MAP.
//FUNCTION : MObject::MObject();
//DESCRIPTION :  Selects object.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Select() {m_selected = true;}

//PACKAGE : MAP.
//FUNCTION : MObject::Deselect();
//DESCRIPTION : Cancel object selection.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void Deselect() {m_selected = false; m_sel_color = MColor(0, 255, 0);}

//PACKAGE : MAP.
//FUNCTION : MObject::SetSelectedColor(MColor& col);
//DESCRIPTION : Sets selected color.
//INPUTS : MColor&.
//RETURNS : NONE.
    virtual void SetSelectedColor(const MColor& col) {m_sel_color = col;}

//PACKAGE : MAP.
//FUNCTION : MObject::GetSelectedColor();
//DESCRIPTION : Gets selected color.
//INPUTS : NONE.
//RETURNS : MColor.
	virtual MColor GetSelectedColor() {return m_sel_color;}

//PACKAGE : MAP.
//FUNCTION : MObject::SetTransType(int type);
//DESCRIPTION : Sets sign transparency value//  added by back.
//INPUTS : int.
//RETURNS : NONE.
	void	SetTransType(int type);

//PACKAGE : MAP.
//FUNCTION : MObject::GetTransType();
//DESCRIPTION : Gets sign transparency value.
//INPUTS : NONE.
//RETURNS : int.
	int		GetTransType() {return m_trans_type;};

//PACKAGE : MAP.
//FUNCTION : MObject::CheckSize(MPlotDevice* pDev);
//DESCRIPTION : Checks object size for drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : True.
	virtual bool CheckSize(MPlotDevice* pDev);

//PACKAGE : MAP.
//FUNCTION : MObject::CheckTextSize(MPlotDevice* pDev);
//DESCRIPTION : Checks text size for drawing.
//INPUTS : MPlotDevice pointer.
//RETURNS : True.
	virtual bool CheckTextSize(MPlotDevice* pDev);

    //PACKAGE : MAP.
    //FUNCTION : MTopobject::WriteSxfPoints( FILE* FileSxf,bool revXY);
    //DESCRIPTION : Writes points of object or subobject from SXF-file.
    //INPUTS : FILE pointer, bool .
    //RETURNS : True.
   virtual bool WriteSxfPoints(FILE *FileSxf, const QJsonObject &classificalCode,
                                MKartaInterface  * pKI,bool revXY)
    {Q_UNUSED(FileSxf);Q_UNUSED(classificalCode);Q_UNUSED(revXY);Q_UNUSED(pKI);return true;} // backspacing of coordinates

#ifdef _DEBUG
	void AssertValid();
#endif
	
};

#endif //MOBJECT_H
