//PACKAGE : MAP.
//FILE: Nadp.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _NADP_H_
#define _NADP_H_
#include "Object.h"
#include "MString.h"

// Constants MNadpKriv
#define NPG_ID_FREE "NPG_ID_FREE"
#define NPK_ID_FREE "NPK_ID_FREE"

// For saving to m_Komplekt
#define NPK_POINTS  0 // Index of points array
#define NPK_ANGLES  1 // Index of angles array

//PACKAGE : MAP.
//CLASS	:  MNadp : public MObject.
//DESCRIPTION : Class - generalized inscription
//				Points for snap vector of inscription storing are situated in m_Komplect (from MObject).
class MNadp : public MObject
{
public:

//PACKAGE : MAP.
//FUNCTION : MNadp::MNadp();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MNadp();

//PACKAGE : MAP.
//FUNCTION : MNadp::MNadp(MRect position,MString Text = "", MPrkExt* Attr = NULL,float Mashtab = 1);
//DESCRIPTION : Constructor by parametrs.
//INPUTS : MRect ,MString , MPrkExt pointer,float .
//RETURNS : NONE.
	MNadp(MRect position,MString Text = "", MPrkExt* Attr = NULL,float Mashtab = 1);

//PACKAGE : MAP.
//FUNCTION : MNadp::~MNadp();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNadp();

	//  Map scale in which deduced inscription
	float  m_Mashtab;

	MPrkExt*  m_Prk;

	//  Text of inscription
	MString m_Text;

public:

//PACKAGE : MAP.
//FUNCTION : MNadp::SetKartMashtab(int Value);
//DESCRIPTION : Sets map scale.
//INPUTS : int
//RETURNS : NONE.
	void    SetKartMashtab(int Value) { m_Mashtab = Value;};// /1000.0;}

//PACKAGE : MAP.
//FUNCTION : MNadp::SetBaseText(MString SetText);
//DESCRIPTION : Sets inscription text.
//INPUTS : MString.
//RETURNS : NONE.
	void    SetBaseText(MString SetText) { m_Text = SetText;}

//PACKAGE : MAP.
//FUNCTION : MNadp::SetBasePrk(MPrkExt*  m_NewPrk);
//DESCRIPTION : Sets handwrite of inscription.
//INPUTS : MPrkExt pointer.
//RETURNS : NONE.
	void    SetBasePrk(MPrkExt*  m_NewPrk) { m_Prk->Copy(m_NewPrk);}

//PACKAGE : MAP.
//FUNCTION : MNadp::SetAlign(unsigned int align);
//DESCRIPTION : Sets modus of alignment.
//INPUTS : unsigned int.
//RETURNS : NONE.
	void    SetAlign(unsigned int align){ m_Prk->align = align;}

//PACKAGE : MAP.
//FUNCTION : MNadp::SetBasePoint(MPoint SrcPnt, MPoint DstPnt, double *Angle = NULL);
//DESCRIPTION :  Sets points of inscription snap.
//INPUTS : MPoint , MPoint , double pointer.
//RETURNS : NONE.
	void	SetBasePoint(MPoint SrcPnt, MPoint DstPnt, double *Angle = NULL);

//PACKAGE : MAP.
//FUNCTION : MNadp::AddPoint(MPoint geopoint);
//DESCRIPTION : Adds point of inscription snap.
//INPUTS : MPoint.
//RETURNS : NONE.
	void    AddPoint(MPoint geopoint);

//PACKAGE : MAP.
//FUNCTION : MNadp::GetBaseText();
//DESCRIPTION : Get inscription text.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetBaseText() { return m_Text; }

//PACKAGE : MAP.
//FUNCTION : MNadp::GetBasePrk();
//DESCRIPTION :  Gets handwrite.
//INPUTS : NONE.
//RETURNS : MPrkExt pointer
	MPrkExt* GetBasePrk() { return m_Prk; }

//PACKAGE : MAP.
//FUNCTION : MNadp::DrawText(MPlotDevice* pDevice);
//DESCRIPTION : Outputs text.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void DrawText(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MNadp::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Draws.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MNadp::DrawTracker(MPlotDevice* pDevice,TrackerState state);
//DESCRIPTION : Draws inscription marking.
//INPUTS : MPlotDevice pointer,TrackerState .
//RETURNS : NONE.
	virtual void DrawTracker(MPlotDevice* pDevice,TrackerState state);

//PACKAGE : MAP.
//FUNCTION : MNadp::ConvertToMaket();
//DESCRIPTION : Converts into model.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ConvertToMaket();

//PACKAGE : MAP.
//FUNCTION : MNadp::MoveHandleTo(int nHandle,MPtArray* pArray, MPoint point);
//DESCRIPTION : Cursor changement during poining on a point
//  			Position changement of a point in a inscription.
//INPUTS : int ,MPtArray pointer, MPoint .
//RETURNS : NONE.
	void MoveHandleTo(int nHandle,MPtArray* pArray, MPoint point);

//PACKAGE : MAP.
//FUNCTION : MNadp::Clone();
//DESCRIPTION : Creates object copy.
//INPUTS : NONE.
//RETURNS : MObject pointer
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MNadp::WhatTheObject();
//DESCRIPTION : Returns identifier of object as generalized inscription.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_NADGENERAL;}

//PACKAGE : MAP.
//FUNCTION : MNadp::GetHandleCount();
//DESCRIPTION :  Returns points quantity.
//INPUTS : NONE.
//RETURNS : int.
	int GetHandleCount();

//PACKAGE : MAP.
//FUNCTION : MNadp::RecalcBounds();
//DESCRIPTION : Refreshs of bounding rectangle.
//INPUTS : NONE.
//RETURNS : True.
	bool   RecalcBounds();

//PACKAGE : MAP.
//FUNCTION : MNadp::Scale(double K, double XC, double YC);
//DESCRIPTION :  Makes scale transformation.
//INPUTS : double , double , double .
//RETURNS : NONE.
	virtual void  Scale(double K, double XC, double YC);

//PACKAGE : MAP.
//FUNCTION : MNadp::Rotate(double A, double B, double C1, double C2);
//DESCRIPTION : Makes rotation transformation.
//INPUTS : double , double , double , double .
//RETURNS : NONE.
	virtual void  Rotate(double A, double B, double C1, double C2);

//PACKAGE : MAP.
//FUNCTION : MNadp::GetBytes();
//DESCRIPTION : Returns full array size to UTP-format file.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int  GetBytes();

//PACKAGE : MAP.
//FUNCTION : MNadp::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads inscription from UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool  ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MNadp::ReadBin(MFile* file,  int version);
//DESCRIPTION : Reads inscription from UTP-format file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool  ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MNadp::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves inscription to UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool  WriteBin(FILE* h_nuxfile,  int version);
};
#endif //_NADP_H_
