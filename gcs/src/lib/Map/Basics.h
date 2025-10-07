//PACKAGE : MAP.
//FILE: Basics.h    
//AUTHOR:
//DESCRIPTION:

#ifndef __NUX_BASICSPLUS_H__
#define __NUX_BASICSPLUS_H__
#include <math.h>
#include "Globals.h"
#include "Array.h"
#include "MString.h"

//PACKAGE : MAP.
//CLASS	: MZarubka.
//DESCRIPTION : Usually class objects stores link index of broken line and
//				coordinates of point where we want to place a 'mark'.
//				Base Type 'MARK' - useful when draw lines of any element at random place
class MZarubka

{
public:
	int     m_nomrebra; //!< Link index of broken line
	MPoint  m_pt;       //!< 'Mark' coordinates

//PACKAGE : MAP.
//FUNCTION : MZarubka::MZarubka();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MZarubka();

//PACKAGE : MAP.
//FUNCTION : MZarubka::MZarubka(int nom, MPoint pt);
//DESCRIPTION : Constructor.
//INPUTS : int , MPoint .
//RETURNS : NONE.				 
	MZarubka(int nom, MPoint pt);

//PACKAGE : MAP.
//FUNCTION : MZarubka::MIntSize();
//DESCRIPTION : Constructor.
//INPUTS : MZarubka pointer.
//RETURNS : NONE.
	MZarubka(MZarubka* zarubka);
	
//PACKAGE : MAP.
//FUNCTION : MZarubka::MIntSize();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MZarubka(){};
	
//PACKAGE : MAP.
//FUNCTION : MZarubka::Copy(MZarubka* zarubka);
//DESCRIPTION : Constructor by default.
//INPUTS : MZarubka pointer.
//RETURNS : NONE.
	void Copy(MZarubka* zarubka);
	
//PACKAGE : MAP.
//FUNCTION : MZarubka::SetZarubka(int nom, MPoint pt);
//DESCRIPTION : Sets parametrs.
//INPUTS : int , MPoint .
//RETURNS : NONE.
	void SetZarubka(int nom, MPoint pt);
	
//PACKAGE : MAP.
//FUNCTION : MZarubka::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int GetSize();
	
//PACKAGE : MAP.
//FUNCTION : MZarubka::WriteBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Saves Mark to UTP-format file    .
//INPUTS : FILE pointer,   int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,   int version);
	
//PACKAGE : MAP.
//FUNCTION : MZarubka::ReadBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Reads Mark from UTP-format file.
//INPUTS : FILE pointer,   int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,   int version);

//PACKAGE : MAP.
//FUNCTION : MZarubka::ReadBin(MFile* file,   int version);
//DESCRIPTION : Reads Mark from UTP-format file.
//INPUTS : MFile pointer,   int .
//RETURNS : True.
	bool ReadBin(MFile* file,   int version);
};

//PACKAGE : MAP.
//CLASS	: MRazryv.
//DESCRIPTION : Objects of class usually store indexes of first and last broken line links.
//				Appropriated to the beginning and to the end of break, and coordinates of the ends of line,
//				determining 'break'.
//				BASE TYPE "LINE BREAK" - is useful in cases when we don't need to draw 
//				part of line at some place of conture
class MRazryv

{
public:
	int     m_indfirst; //!< Broken line link index of the beginning of 'break'
	int     m_indlast;  //!< Broken line link index of the end of 'break'
	MPoint  m_ptfirst;  //!< Coordinates of the beginning of 'break'
	MPoint  m_ptlast;   //!< Coordinates of the end of 'break'


//PACKAGE : MAP.
//FUNCTION : MRazryv::MRazryv();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MRazryv();

//PACKAGE : MAP.
//FUNCTION : MRazryv::MRazryv();
//DESCRIPTION : Constructor with established parameters.
//INPUTS : int , int , MPoint , MPoint .
//RETURNS : NONE.
	MRazryv(int indfirst, int indlast, MPoint ptfirst, MPoint ptlast);


//PACKAGE : MAP.
//FUNCTION : MRazryv::MRazryv(MRazryv* razryv);
//DESCRIPTION : Constructor.
//INPUTS : MRazryv pointer.
//RETURNS : NONE.
	MRazryv(MRazryv* razryv);
	
//PACKAGE : MAP.
//FUNCTION : MRazryv::~MRazryv();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MRazryv(){};
	
//PACKAGE : MAP.
//FUNCTION : MRazryv::Copy(MRazryv* razryv);
//DESCRIPTION : Function of copy.
//INPUTS : MRazryv pointer.
//RETURNS : NONE.
	void Copy(MRazryv* razryv);

//PACKAGE : MAP.
//FUNCTION : MRazryv::SetRazryv(int indfirst, int indlast, MPoint ptfirst, MPoint ptlast);
//DESCRIPTION : Sets parametrs.
//INPUTS : int , int , MPoint , MPoint .
//RETURNS : NONE.
	void SetRazryv(int indfirst, int indlast, MPoint ptfirst, MPoint ptlast);
	
//PACKAGE : MAP.
//FUNCTION : MRazryv::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int GetSize();
	
//PACKAGE : MAP.
//FUNCTION : MRazryv::WriteBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Saves break to UTP-format file.
//INPUTS : FILE pointer,   int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,   int version);
	
//PACKAGE : MAP.
//FUNCTION : MRazryv::ReadBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Reads break from UTP-format file.
//INPUTS : FILE pointer,   int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,   int version);  

//PACKAGE : MAP.
//FUNCTION : MRazryv::ReadBin(MFile* file,   int version);
//DESCRIPTION : Reads break from UTP-format file.
//INPUTS : MFile pointer,   int .
//RETURNS : True.
	bool ReadBin(MFile* file,   int version);
};

//PACKAGE : MAP.
//CLASS	: MSegment.
//DESCRIPTION : Objects of class usually store coordinates of the ends of segment
//				and points altitudes - ends of line
//				ASE TYPE "SEGMENT" - is used to create 
//				triangular lines appropriate to relief creating
class MSegment

{
public:
	MPoint  m_sgpt1;       //!< Beginning coordinates of segment
	MPoint  m_sgpt2;       //!< Coordinates of the end of segment
	double  m_sgheight1;   //!< Point altitude of the beginning of segment
	double  m_sgheight2;   //!< Point altitude of the end of segment

//PACKAGE : MAP.
//FUNCTION : MSegment::MSegment();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MSegment();
	
//PACKAGE : MAP.
//FUNCTION : MSegment::MSegment(MPoint pt1, MPoint pt2, double height1, double height2);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MPoint , MPoint , double , double .
//RETURNS : NONE.				 
	MSegment(MPoint pt1, MPoint pt2, double height1, double height2);

//PACKAGE : MAP.
//FUNCTION : MSegment::MSegment(MSegment* segment);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MSegment pointer.
//RETURNS : NONE.				 
	MSegment(MSegment* segment);
	
//PACKAGE : MAP.
//FUNCTION : MSegment::~MSegment()();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MSegment(){};
	
//PACKAGE : MAP.
//FUNCTION : MSegment::Copy(MSegment* segment);
//DESCRIPTION : Copy Function.
//INPUTS : MSegment pointer.
//RETURNS : NONE.
	void Copy(MSegment* segment);
	
//PACKAGE : MAP.
//FUNCTION : MSegment::SetSegment(MPoint pt1, MPoint pt2, double height1, double height2);
//DESCRIPTION : Parameters establishment.
//INPUTS : MPoint , MPoint , double , double .
//RETURNS : NONE.
	void SetSegment(MPoint pt1, MPoint pt2, double height1, double height2);
	
//PACKAGE : MAP.
//FUNCTION : MSegment::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int GetSize();
	
//PACKAGE : MAP.
//FUNCTION : MSegment::WriteBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Saves segment to UTP-format file.
//INPUTS : FILE pointer,   int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,   int version);

//PACKAGE : MAP.
//FUNCTION : MSegment::ReadBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Reads segment from UTP-format file.
//INPUTS : FILE pointer,   int .
//RETURNS : True.	
	bool ReadBin(FILE* h_nuxfile,   int version);

//PACKAGE : MAP.
//FUNCTION : MSegment::ReadBin(MFile* file,   int version);
//DESCRIPTION : Reads segment from UTP-format file.
//INPUTS : MFile pointer,   int .
//RETURNS : True.
	bool ReadBin(MFile* file,   int version);
};


//PACKAGE : MAP.
//CLASS	: MTriangle.
//DESCRIPTION : Objects of this class usually store coordinates and altitudes 	of triangle vertexes	
//				BASE TYPE "TRIANGLE" - useful at formatting of triangulation triangles class MTriangle
class MTriangle

{
public:
	MPoint  m_tript1;       //!< Coordinates of null vertex of triangle
	MPoint  m_tript2;       //!< Coordinates of first vertex of triangle
	MPoint  m_tript3;       //!< Coordinates of second vertex of triangle
	double  m_triheight1;   //!< Altitude of null vertex
	double  m_triheight2;   //!< Altitude of first vertex
	double  m_triheight3;   //!< Altitude of second vertex

//PACKAGE : MAP.
//FUNCTION : MTriangle::MTriangle();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MTriangle();

//PACKAGE : MAP.
//FUNCTION : MTriangle::MTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double height1, double height2, double height3);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MPoint , MPoint , MPoint , double , double , double .
//RETURNS : NONE.					 
	MTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double height1, double height2, double height3);

//PACKAGE : MAP.
//FUNCTION : MTriangle::MTriangle(MTriangle* triangle);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MTriangle pointer.
//RETURNS : NONE.
	MTriangle(MTriangle* triangle);
	
//PACKAGE : MAP.
//FUNCTION : MTriangle::~MTriangle();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MTriangle(){};
	
//PACKAGE : MAP.
//FUNCTION : MTriangle::Copy(MTriangle* triangle);
//DESCRIPTION : Function of copy.
//INPUTS : MTriangle pointer.
//RETURNS : NONE.
	void Copy(MTriangle* triangle);
	
//PACKAGE : MAP.
//FUNCTION : MTriangle::SetTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double height1, double height2, double height3);
//DESCRIPTION : Sets parametrs.
//INPUTS : MPoint , MPoint , MPoint , double , double , double .
//RETURNS : NONE.
	void SetTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double height1, double height2, double height3);
	
//PACKAGE : MAP.
//FUNCTION : MTriangle::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int GetSize();
	
//PACKAGE : MAP.
//FUNCTION : MTriangle::WriteBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Saves triangular to UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,   int version);

//PACKAGE : MAP.
//FUNCTION : MTriangle::ReadBin(FILE* h_nuxfile,   int version);
//DESCRIPTION : Reads triangular from UTP-format file.
//INPUTS : FILE pointer,   int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,   int version);  

//PACKAGE : MAP.
//FUNCTION : MTriangle::ReadBin(MFile* file,   int version);
//DESCRIPTION : Reads triangular from UTP-format file.
//INPUTS : MFile pointer,   int .
//RETURNS : True.
	bool ReadBin(MFile* file,   int version);
};


//PACKAGE : MAP.
//CLASS	: MZbArray:public GlobalArray<MZarubka>.
//DESCRIPTION : Class - array of pointers to MZarubka.
class MZbArray:public GlobalArray<MZarubka>
{
public:

//PACKAGE : MAP.
//FUNCTION : MZbArray::CreateOfType();
//DESCRIPTION : Creates an array - object of MZbArray class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MZbArray();};

//PACKAGE : MAP.
//FUNCTION : MZbArray::WhatTheArray();
//DESCRIPTION : Returns identifier of MZbArray array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_ZBK;};
};


//PACKAGE : MAP.
//CLASS	: MRzArray:public GlobalArray<MRazryv>.
//DESCRIPTION : Class - array of pointers to MRazryv.
class MRzArray:public GlobalArray<MRazryv>
{
public: 

//PACKAGE : MAP.
//FUNCTION : MRzArray::CreateOfType();
//DESCRIPTION : Creates an array - object of MRzArray class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MRzArray();};

//PACKAGE : MAP.
//FUNCTION : MRzArray::CreateOfType();
//DESCRIPTION : Creates an array - object of MRzArray class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MString WhatTheArray(){return ID_RZV;};
	
//PACKAGE : MAP.
//FUNCTION : MRzArray::OrderByFirstIndex();
//DESCRIPTION : Sorts breaks array by first index in ascending order.
//INPUTS : NONE.
//RETURNS : True.
	bool    OrderByFirstIndex();
};


//PACKAGE : MAP.
//CLASS	: MSgArray:public GlobalArray<MSegment>.
//DESCRIPTION : Class - array of pointers to MSegment array.
class MSgArray:public GlobalArray<MSegment>
{
public:

//PACKAGE : MAP.
//FUNCTION : MSgArray::CreateOfType();
//DESCRIPTION : Creates an array - object of MSgArray class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MSgArray();};

//PACKAGE : MAP.
//FUNCTION : MSgArray::WhatTheArray();
//DESCRIPTION : Returns identifier of MSgArray array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_SGT;};
	
//PACKAGE : MAP.
//FUNCTION : MSgArray::FindByCoords(int iIndex, MPoint pt1, MPoint pt2);
//DESCRIPTION : Searchs for segment,which coordenates of ends equals input coordinates.
//INPUTS :int , MPoint , MPoint .
//RETURNS : int.
	int FindByCoords(int iIndex, MPoint pt1, MPoint pt2);
};


//PACKAGE : MAP.
//CLASS	: MTrArray:public GlobalArray<MTriangle>.
//DESCRIPTION : Class - array of pointers to MTriangle array.
class MTrArray:public GlobalArray<MTriangle>
{
public:

//PACKAGE : MAP.
//FUNCTION : MTrArray::CreateOfType();
//DESCRIPTION : Creates an array - object of MTrArray class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MTrArray();};

//PACKAGE : MAP.
//FUNCTION : MTrArray::WhatTheArray();
//DESCRIPTION : Returns identifier of MTrArray array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_TRG;};

//PACKAGE : MAP.
//FUNCTION : MTrArray::FindByCoords(int iIndex, MPoint pt1, MPoint pt2, MPoint pt3);
//DESCRIPTION : Searchs for triangle, which coordinates of ends equal input coordinates.
//INPUTS : int , MPoint , MPoint , MPoint .
//RETURNS : int.	
	int FindByCoords(int iIndex, MPoint pt1, MPoint pt2, MPoint pt3);
};

#endif//__NUX_BASICSPLUS_H__
