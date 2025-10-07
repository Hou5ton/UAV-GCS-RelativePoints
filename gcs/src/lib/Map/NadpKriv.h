//PACKAGE : MAP.
//FILE: NadpKriv.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _KRIV_NADP_H_
#define _KRIV_NADP_H_
#include <math.h>
#include "Array.h"
#include "Object.h"
#include "MString.h"

class MLayer;

//PACKAGE : MAP.
//CLASS	: MNadpKriv : public MObject.
//DESCRIPTION : Class - figured inscription.
class MNadpKriv : public MObject
{
public:

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::MNadpKriv();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MNadpKriv();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::MNadpKriv(const MRect& position, MPrkExt* prk = NULL);
//DESCRIPTION : Constructor.
//INPUTS : const MRect& , MPrkExt pointer.
//RETURNS : NONE.
	MNadpKriv(const MRect& position, MPrkExt* prk = NULL);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::~MNadpKriv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNadpKriv();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::GetPrk();
//DESCRIPTION : Returns pointer to handwrite.
//INPUTS : NONE.
//RETURNS : MPrkExt pointer.
	MPrkExt* GetPrk(){return m_prk;};

	// Handwrite of curvilinear inscription
	MPrkExt*  m_prk;
 
//PACKAGE : MAP.
//FUNCTION : MNadpKriv::Scale(double K, double XC, double YC);
//DESCRIPTION : Makes scale transformation.
//INPUTS : double , double , double .
//RETURNS : NONE.
	virtual void Scale(double K, double XC, double YC);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::Rotate(double A, double B, double C1, double C2);
//DESCRIPTION : Makes turn transformation.
//INPUTS : double , double , double , double .
//RETURNS : NONE.
	virtual void Rotate(double A, double B, double C1, double C2);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::AddPoint(const MPoint& geopoint);
//DESCRIPTION : Adds point to operation object.
//INPUTS : const MPoint&.
//RETURNS : NONE.
	void              AddPoint(const MPoint& geopoint);
	
//PACKAGE : MAP.
//FUNCTION : MNadpKriv::DrawText(MPlotDevice* pDevice);
//DESCRIPTION : Draws text.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	  DrawText(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv:: Draw(MPlotDevice* pDevice);
//DESCRIPTION :  Draws layer objects.
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	virtual void	  Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::DrawTracker(MPlotDevice* pDevice, TrackerState state);
//DESCRIPTION : Draws topobject marking.
//INPUTS : MPlotDevice pointer, TrackerState .
//RETURNS : NONE.
	virtual void      DrawTracker(MPlotDevice* pDevice, TrackerState state);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::GetHandleCount();
//DESCRIPTION : How many oints inside the frame?
//INPUTS : NONE.
//RETURNS : int.
	virtual int       GetHandleCount();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::MoveHandleTo(int nHandle,MPtArray* pArray, MPoint point);
//DESCRIPTION : Changes position of a point in the inscription.
//INPUTS : int ,MPtArray pointer, MPoint .
//RETURNS : NONE.
	virtual void      MoveHandleTo(int nHandle,MPtArray* pArray, MPoint point);

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::GetBytes();
//DESCRIPTION : Returns full size of array to UTP-format file.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int   GetBytes();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION :  Reads inscription from UTP-file.
//INPUTS : FILE pointer,  int .
//RETURNS : False.
	virtual bool	  ReadBin(FILE* h_nuxfile,  int version);//! {return false;}
	
//PACKAGE : MAP.
//FUNCTION : MNadpKriv::ReadBin(MFile* file,  int version);
//DESCRIPTION :  Reads inscription from UTP-file.
//INPUTS : MFile pointer,  int .
//RETURNS : False.
	virtual bool	  ReadBin(MFile* file,  int version);//! {return false;}

//PACKAGE : MAP.
//FUNCTION : MNadpKriv:: WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves inscription to UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : False.
	virtual bool	  WriteBin(FILE* h_nuxfile,  int version);//!{return false;}

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::Clone();
//DESCRIPTION : Creates object copy.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::WhatTheObject();
//DESCRIPTION : Returns object name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString   WhatTheObject() {return ID_NADPKRIV;}

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::RecalcBounds();
//DESCRIPTION : Refreshs bounding rectangle.
//INPUTS : NONE.
//RETURNS : True.
	bool              RecalcBounds();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv::RecalcAngles();
//DESCRIPTION : Refreshs rectangle.
//INPUTS : NONE.
//RETURNS : True.
	bool              RecalcAngles();

//PACKAGE : MAP.
//FUNCTION : MNadpKriv:: MasterScript(MString Text = "TEKCT");
//DESCRIPTION : Construct inscription! (TEXT - written by Latin letters).
//INPUTS : MString.
//RETURNS : NONE.
	void              MasterScript(MString Text = "TEKCT");
};

#endif // _KRIV_NADP_H_
