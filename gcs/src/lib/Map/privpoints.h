//PACKAGE : MAP.
//FILE: Privpoints.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _PRIVPOINTS_H_
#define _PRIVPOINTS_H_

// File contains class descriptions of different snaps types
// for formulas

#include "BaseCl.h"
#include "ArrBase.h"
#include "MString.h"

// Identifiers of different snaps types
#define SIMPLE_PRIVYAZKA  1
#define DOUBLE_PRIVYAZKA  2
#define VINOSKA_PRIVYAZKA 3
#define SPLINE_PRIVYAZKA  4

// Prior declaration of calsses
class MSimplePriv;
class MDoublePriv;
class MVinoskaPriv;
class MSplinePriv;

//PACKAGE : MAP.
//CLASS : MPriv.
//DESCRIPTION : Base abstarct class for snaps forms.
//				NUMBER OF FORMULAS per one snap cannot be more than 64000 (unsigned short).
class MPriv
{
protected:
	unsigned short ush_formNumber; // < Number of snapped formula

public:

//PACKAGE : MAP.
//FUNCTION : MPriv::MPriv();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MPriv():ush_formNumber(0){};

//PACKAGE : MAP.
//FUNCTION : MPriv::~MPriv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MPriv(){};

//PACKAGE : MAP.
//FUNCTION : MPriv::GetFormNomer();
//DESCRIPTION : Gets number of snapped formula.
//INPUTS : NONE.
//RETURNS : unsigned short .
	unsigned short GetFormNomer(){return ush_formNumber;};

//PACKAGE : MAP.
//FUNCTION : MPriv::SetFormNomer(unsigned short formNum);
//DESCRIPTION : Gets number of snapped formula.
//INPUTS : unsigned short.
//RETURNS : NONE.
	void SetFormNomer(unsigned short formNum){ush_formNumber = formNum;};

//PACKAGE : MAP.
//FUNCTION : MPriv::GetBytes();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int .
	int GetBytes(){
		// SNAP TYPE + formula number + points quantity and a point
		int iRet = sizeof(unsigned char)+sizeof(ush_formNumber)+sizeof(unsigned short)+GetPointNum()*sizeof(MPoint);
		return iRet;};

	// Following functions must be redefined of inheritors

//PACKAGE : MAP.
//FUNCTION : MPriv::GetType();
//DESCRIPTION : Gets type of snap.
//INPUTS : NONE.
//RETURNS : unsigned char.
		virtual unsigned char GetType(){return 0;};

//PACKAGE : MAP.
//FUNCTION : MPriv::GetPointNum();
//DESCRIPTION : Gets quantity of points in a snap.
//INPUTS : NONE.
//RETURNS : unsigned short.
		virtual unsigned short GetPointNum(){return 0;};

//PACKAGE : MAP.
//FUNCTION : MPriv::GetPoints();
//DESCRIPTION : Gets pointer to array of points.
//INPUTS : NONE.
//RETURNS : MPoint pointer.
		virtual MPoint* GetPoints(){return NULL;};

//PACKAGE : MAP.
//FUNCTION : MPriv::SetPoints(MPoint* ,unsigned short );
//DESCRIPTION : Sets new array of points.
//INPUTS : MPoint pointer ,unsigned short.
//RETURNS : NONE.
		virtual void SetPoints(MPoint* ,unsigned short ){};

//PACKAGE : MAP.
//FUNCTION : MPriv::Clone();
//DESCRIPTION : Clones snap.
//INPUTS : NONE.
//RETURNS : MPriv pointer.
		virtual MPriv* Clone(){return NULL;};

//PACKAGE : MAP.
//FUNCTION : MPriv::RotateAroundCenter(double dRadangle);
//DESCRIPTION : Rotates on defined angle relatively to centre.
//INPUTS : double.
//RETURNS : NONE.
		virtual void RotateAroundCenter(double dRadangle);

//PACKAGE : MAP.
//FUNCTION : MPriv::SetAtAnAngle(double dRadangle);
//DESCRIPTION : Sets rotation of snap vector.
//INPUTS : double.
//RETURNS : NONE.
		virtual void SetAtAnAngle(double dRadangle);

//PACKAGE : MAP.
//FUNCTION : MPriv::GetDrawArray(MPtArray* pta);
//DESCRIPTION : Gets array of points for drawing.
//INPUTS : MPtArray pointer.
//RETURNS : NONE.
		virtual void GetDrawArray(MPtArray* pta){pta->FastSet(GetPointNum(),GetPoints());};

//PACKAGE : MAP.
//FUNCTION : MPriv::GetFirstPoint();
//DESCRIPTION : Gets first point of snap.
//INPUTS : NONE.
//RETURNS : MPoint.
		virtual MPoint GetFirstPoint(){return *GetPoints();};

//PACKAGE : MAP.
//FUNCTION : MPriv::GetLastPoint();
//DESCRIPTION : Gets last point of snap.
//INPUTS : NONE.
//RETURNS : MPoint.
		virtual MPoint GetLastPoint(){return *(GetPoints()+(GetPointNum()-1));};

//PACKAGE : MAP.
//FUNCTION : MPriv::MSimplePriv();
//DESCRIPTION : Operators of type conversion.
//INPUTS : NONE.
//RETURNS : NONE.
		virtual operator MSimplePriv();


		virtual operator MDoublePriv();


		virtual operator MVinoskaPriv();


		virtual operator MSplinePriv();

//PACKAGE : MAP.
//FUNCTION : MPriv::WriteBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,int version);

protected:

//PACKAGE : MAP.
//FUNCTION : MPriv::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MPriv::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool ReadBin(MFile* file, int version);

public:

//PACKAGE : MAP.
//FUNCTION : MPriv::ReadAny(FILE* h_nuxfile,int version);
//DESCRIPTION : Utility of different snaps types reading
//  			Returns pointer to base type.
//INPUTS : FILE pointer,int .
//RETURNS : MPriv pointer.
	static MPriv* ReadAny(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MPriv::ReadAny(MFile* file,int version);
//DESCRIPTION : Utility of different snaps types reading
//  			Returns pointer to base type.
//INPUTS : MFile pointer,int .
//RETURNS :  MPriv pointer.
	static MPriv* ReadAny(MFile* file,int version);
};

//PACKAGE : MAP.
//CLASS : MSimplePriv: public MPriv.
//DESCRIPTION : Class for exact snapping.
class MSimplePriv: public MPriv
{
protected:
	MPoint point;	// < single point of snap

public:

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::MSimplePriv();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MSimplePriv():point(0.0){};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::MSimplePriv(MPoint pt,unsigned short form);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MPoint ,unsigned short .
//RETURNS : NONE.
	MSimplePriv(MPoint pt,unsigned short form):point(pt){
		ush_formNumber=form;};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::~MSimplePriv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MSimplePriv(){};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::GetType();
//DESCRIPTION : Gets snap type.
//INPUTS : NONE.
//RETURNS : unsigned char .
	virtual unsigned char GetType(){return SIMPLE_PRIVYAZKA;};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::GetPointNum();
//DESCRIPTION : Gets points quantity.
//INPUTS : NONE.
//RETURNS : unsigned short.
	virtual unsigned short GetPointNum(){return 1;};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::GetPoints();
//DESCRIPTION : Gets pointer to array of points.
//INPUTS : NONE.
//RETURNS : MPoint pointer.
	virtual MPoint* GetPoints(){return &point;};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::SetPoints(MPoint* pPoints,unsigned short iNum);
//DESCRIPTION : Sets new array of points.
//INPUTS : MPoint pointer,unsigned short .
//RETURNS : NONE.
	virtual void SetPoints(MPoint* pPoints,unsigned short iNum){if(!(iNum==1)) return; point = *pPoints;};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::Clone();
//DESCRIPTION : Makes a clone of snap.
//INPUTS : NONE.
//RETURNS : MPriv pointer.
	virtual MPriv* Clone(){return (MPriv*)new MSimplePriv(point,ush_formNumber);};

//PACKAGE : MAP.
//FUNCTION : MSimplePriv::RotateAroundCenter(int );
//DESCRIPTION : One-shot snap cannot be rotated around centre.
//INPUTS : int.
//RETURNS : NONE.
	virtual void RotateAroundCenter(int ){};

};

//PACKAGE : MAP.
//CLASS : MDoublePriv:public MPriv.
//DESCRIPTION : Class describes double-point snap.
class MDoublePriv:public MPriv
{
protected:
	// Successive arrangement of points is very critical here
	MPoint point1; // < first point of snap
	MPoint point2; // < second point of snap
public:

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::MDoublePriv();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MDoublePriv(){};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::MDoublePriv();
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MPoint ,MPoint ,unsigned short .
//RETURNS : NONE.
	MDoublePriv(MPoint pt1,MPoint pt2,unsigned short form):point1(pt1),point2(pt2){
				ush_formNumber=form;};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::~MDoublePriv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MDoublePriv(){};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::GetType();
//DESCRIPTION : Gets type of snap.
//INPUTS : NONE.
//RETURNS : unsigned char .
	virtual unsigned char GetType(){return DOUBLE_PRIVYAZKA;};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::GetPointNum();
//DESCRIPTION : Gets points quantity.
//INPUTS : NONE.
//RETURNS : unsigned short.
	virtual unsigned short GetPointNum(){return 2;};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::GetPoints();
//DESCRIPTION : Gets pointer to array.
//INPUTS : NONE.
//RETURNS : MPoint pointer.
	virtual MPoint* GetPoints(){return &point1;};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::SetPoints(MPoint* pPoints,unsigned short iNum);
//DESCRIPTION : Sets new array of points.
//INPUTS : MPoint pointer,unsigned short .
//RETURNS : NONE.
	virtual void SetPoints(MPoint* pPoints,unsigned short iNum){if(!(iNum==2)) return;point1 = *pPoints;point2 = *++pPoints;};

//PACKAGE : MAP.
//FUNCTION : MDoublePriv::Clone();
//DESCRIPTION : Makes a clone of snap.
//INPUTS : NONE.
//RETURNS : MPriv pointer.
	virtual MPriv* Clone(){return (MPriv*)new MDoublePriv(point1,point2,ush_formNumber);};
};

//PACKAGE : MAP.
//CLASS : MVinoskaPriv:public MPriv.
//DESCRIPTION : Class describes leader-snap (3 points needed).
class MVinoskaPriv:public MPriv
{
protected:
	// Successive arrangement of points is very critical here
	MPoint point1; // < first point of snap
	MPoint point2; // < second point of snap
	MPoint point3; // < third point of snap
public:

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::MVinoskaPriv();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MVinoskaPriv(){};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::MVinoskaPriv(MPoint pt1,MPoint pt2,MPoint pt3,unsigned short form);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : MPoint ,MPoint ,MPoint ,unsigned short .
//RETURNS : NONE.
	MVinoskaPriv(MPoint pt1,MPoint pt2,MPoint pt3,unsigned short form):point1(pt1),point2(pt2),point3(pt3){
		ush_formNumber=form;};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::~MVinoskaPriv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MVinoskaPriv(){};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::GetType();
//DESCRIPTION :  Gets type of snap.
//INPUTS : NONE.
//RETURNS : unsigned char .
	virtual unsigned char GetType(){return VINOSKA_PRIVYAZKA;};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::GetPointNum();
//DESCRIPTION :  Gets points quantity.
//INPUTS : NONE.
//RETURNS : unsigned short.
	virtual unsigned short GetPointNum(){return 3;};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::GetPoints();
//DESCRIPTION : Gets pointer to array.
//INPUTS : NONE.
//RETURNS : MPoint pointer.
	virtual MPoint* GetPoints(){return &point1;};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::SetPoints(MPoint* pPoints,unsigned short iNum);
//DESCRIPTION : Sets new array of points.
//INPUTS : MPoint pointer,unsigned short .
//RETURNS : NONE.
	virtual void SetPoints(MPoint* pPoints,unsigned short iNum){if(!(iNum==3)) return;memcpy(&point1,pPoints,sizeof(MPoint)*3);};

//PACKAGE : MAP.
//FUNCTION : MVinoskaPriv::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPriv pointer.
	virtual MPriv* Clone(){return (MPriv*)new MVinoskaPriv(point1,point2,point3,ush_formNumber);};
};

//PACKAGE : MAP.
//CLASS : MSplinePriv:public MPriv.
//DESCRIPTION : Class for spline snapping.
class MSplinePriv:public MPriv
{
protected:
	MPoint pt1; // < first point of snap
	MPoint pt2; // < second point of snap
	MPoint pt3; // < third point of snap
	MPoint pt4; // < fourth point of snap

public:

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::MSplinePriv();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MSplinePriv(){};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::MSplinePriv(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,unsigned short form);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : MPoint ,MPoint ,MPoint ,MPoint ,unsigned short .
//RETURNS : NONE.
	MSplinePriv(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,unsigned short form):pt1(pt1),pt2(pt2),pt3(pt3),pt4(pt4){
		ush_formNumber=form;};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::~MSplinePriv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MSplinePriv(){};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::GetType();
//DESCRIPTION : Gets type of snap.
//INPUTS : NONE.
//RETURNS : unsigned char .
	virtual unsigned char GetType(){return SPLINE_PRIVYAZKA;};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::GetPointNum();
//DESCRIPTION : Gets points quantity.
//INPUTS : NONE.
//RETURNS : unsigned short.
	virtual unsigned short GetPointNum(){return 4;};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::GetPoints();
//DESCRIPTION : Gets pointer to array.
//INPUTS : NONE.
//RETURNS : MPoint pointer.
	virtual MPoint* GetPoints(){return &pt1;};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::SetPoints(MPoint* pPoints,unsigned short iNum);
//DESCRIPTION : Sets new array of points.
//INPUTS : MPoint pointer,unsigned short .
//RETURNS : NONE.
	virtual void SetPoints(MPoint* pPoints,unsigned short iNum){if(!(iNum==4)) return;memcpy(&pt1,pPoints,sizeof(MPoint)*4);};

//PACKAGE : MAP.
//FUNCTION : MSplinePriv::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS :  Makes a clone pointer.
	virtual MPriv* Clone(){return (MPriv*)new MSplinePriv(pt1,pt2,pt3,pt4,ush_formNumber);};
};

//PACKAGE : MAP.
//CLASS : MPvArray:public GlobalInheritantArray<MPriv>.
//DESCRIPTION : Class - array of snaps.  GlobalInheritantArray provides access to work with array of classes, which inherited from one.
class MPvArray:public GlobalInheritantArray<MPriv>
{
public:

//PACKAGE : MAP.
//FUNCTION : MPvArray::CreateOfType();
//DESCRIPTION : Creates an array - object of class MPvArray.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MPvArray;};

//PACKAGE : MAP.
//FUNCTION : MPvArray::WhatTheArray();
//DESCRIPTION : Returns identifier of 'MPvArray' array of snaps.
//INPUTS : NONE.
//RETURNS : MString .
	virtual MString WhatTheArray(){return ID_PVA;};

//PACKAGE : MAP.
//FUNCTION : MPvArray::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MPvArray::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MPvArray::WriteBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,int version)
	{
		switch (version)
		{
		case 0:
			// Saves identifier
			MString str = WhatTheArray();
			fwrite((char *)str, 3, 1, h_nuxfile);
			// Saves elements quantity
			fwrite(&Size, 4, 1, h_nuxfile);
			// Saves lines
			MPriv* pAtom;
			for(int i=0;i<Size;i++)
			{
				pAtom = GetAt(i);
				pAtom->WriteBin(h_nuxfile,version);
			}
		}
		return true;
	}
	
//PACKAGE : MAP.
//FUNCTION : MPvArray::SetSize(int Cnt);
//DESCRIPTION : Sets array size.
//INPUTS : int.
//RETURNS : NONE.
	void SetSize(int Cnt)
	{
		MPriv* pEl = new MSimplePriv();
		ExpandSize(Cnt);
		for(int i=0;i<Cnt;i++)
			SetAt(i,pEl);
		delete pEl;
	}

};

//PACKAGE : MAP.
//FUNCTION : MPvArray::CreatePrivByType(unsigned char bType);
//DESCRIPTION :  Creates a snap by type.
//INPUTS : unsigned char.
//RETURNS : MPriv pointer.
MPriv* CreatePrivByType(unsigned char bType);

#endif //_PRIVPOINTS_H_

