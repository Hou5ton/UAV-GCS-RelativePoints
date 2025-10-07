//PACKAGE : MAP.
//FILE: Yaz.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _YAZ__H_
#define _YAZ__H_
#include "BaseCl.h"
#include "ArrBase.h"
#include "Formula.h"
#include "Prosloiki.h"
#include "plasti.h"
#include "srezi.h"
#include "MString.h"

//PACKAGE : MAP.
//CLASS : MLinYaz.
//DESCRIPTION : Class describes linear object.
class MLinYaz
{
public:
	//  Pointer to array of layers
	MPrArray* m_linia;
	
//PACKAGE : MAP.
//FUNCTION : MLinYaz::MLinYaz();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MLinYaz();

//PACKAGE : MAP.
//FUNCTION : MLinYaz::MLinYaz(MPrArray* p_lin);
//DESCRIPTION : Constructor.
//INPUTS : MPrArray pointer.
//RETURNS : NONE.
	MLinYaz(MPrArray* p_lin);

//PACKAGE : MAP.
//FUNCTION : MLinYaz::Copy(MLinYaz* p_linobj);
//DESCRIPTION : Function of copying.
//INPUTS : MLinYaz pointer.
//RETURNS : NONE.
	void Copy(MLinYaz* p_linobj);

//PACKAGE : MAP.
//FUNCTION : MLinYaz::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MLinYaz::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MLinYaz::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool  ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MLinYaz::GetBytes();
//DESCRIPTION : Calculates object size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MLinYaz::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension();

//PACKAGE : MAP.
//FUNCTION : MLinYaz::~MLinYaz();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MLinYaz();
};

//PACKAGE : MAP.
//CLASS : MPloYaz.
//DESCRIPTION : Class describes area object.
class MPloYaz
{
public:
	//  pointer to array of layers of contour
	MPrArray* m_linia;

	//  pointer to array of layers
	MPlArray* m_plast;

//PACKAGE : MAP.
//FUNCTION : MPloYaz::MPloYaz();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MPloYaz();

//PACKAGE : MAP.
//FUNCTION : MPloYaz::MPloYaz(MPrArray* p_linia, MPlArray* p_plast);
//DESCRIPTION : Constructor.
//INPUTS : MPrArray pointer, MPlArray pointer.
//RETURNS : NONE.
	MPloYaz(MPrArray* p_linia, MPlArray* p_plast);

//PACKAGE : MAP.
//FUNCTION : MPloYaz::Copy(MPloYaz* ploshobj);
//DESCRIPTION : Function of copying.
//INPUTS : MPloYaz pointer.
//RETURNS : NONE.
	void Copy(MPloYaz* ploshobj);

//PACKAGE : MAP.
//FUNCTION : MPloYaz::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MPloYaz::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MPloYaz::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool  ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MPloYaz::GetBytes();
//DESCRIPTION : Calculates size of object description in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MPloYaz::~MPloYaz();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MPloYaz();

//PACKAGE : MAP.
//FUNCTION : MPloYaz::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension();
};

//PACKAGE : MAP.
//CLASS : MDotYaz.
//DESCRIPTION : Class describes dotted object.
class MDotYaz
{
public:
	//  Pointer to array of shears
	MSzArray* m_tochka;

//PACKAGE : MAP.
//FUNCTION : MDotYaz::MDotYaz();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MDotYaz();

//PACKAGE : MAP.
//FUNCTION : MDotYaz::MDotYaz(MSzArray* p_tochka);
//DESCRIPTION : Constructor.
//INPUTS : MSzArray pointer.
//RETURNS : NONE.
	MDotYaz(MSzArray* p_tochka);

//PACKAGE : MAP.
//FUNCTION : MDotYaz::Copy(MDotYaz* p_tochobj);
//DESCRIPTION : Function of copying.
//INPUTS : MDotYaz pointer.
//RETURNS : NONE.
	void Copy(MDotYaz* p_tochobj);

//PACKAGE : MAP.
//FUNCTION : MDotYaz::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MDotYaz::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MDotYaz::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool  ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MDotYaz::GetBytes();
//DESCRIPTION : Calculates size of object description in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MDotYaz::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension();

//PACKAGE : MAP.
//FUNCTION : MDotYaz::~MDotYaz();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MDotYaz();
};

//======================================================
// BASE TYPES OF LANGUAGE OF OBJECTS DESCRIPTION
//======================================================

// constants of objects types
// area
#define F_PLO	0 

// linear
#define F_LIN	1 

// dotted
#define F_DOT	2 

// stripped
#define IDL_PLS		7 

//PACKAGE : MAP.
//CLASS : MReliefRaster : public MRaster.
//DESCRIPTION : Class of language of objects descrition.
class MYaz
{
protected:
	//  object type
	short		m_type;		
	float		m_fMasstKoeff;

public:
	//  object description (by codes) and their names in a dictionary
	void*		m_object;		
									
	//  array of formulas
	MFormArray*	m_formula;		

	//  flag of drawing
	bool        m_visible;			

//PACKAGE : MAP.
//FUNCTION : MYaz::MYaz(int ntype, MFormArray* pformula=NULL, void* pobject=NULL);
//DESCRIPTION : Constructor.
//INPUTS : int , MFormArray pointer, void pointer.
//RETURNS : NONE.
	MYaz(int ntype, MFormArray* pformula=NULL, void* pobject=NULL);

//PACKAGE : MAP.
//FUNCTION : MYaz::MYaz();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MYaz();

//PACKAGE : MAP.
//FUNCTION : MYaz::GetType();
//DESCRIPTION : Gets type of description language.
//INPUTS : NONE.
//RETURNS : int.
    int GetType() const {if(m_type == IDL_PLS) return F_LIN; return m_type;};

//PACKAGE : MAP.
//FUNCTION : MYaz::IsPls();
//DESCRIPTION : Sets type of description language.
//INPUTS : NONE.
//RETURNS : True.
	bool IsPls(){if(m_type == IDL_PLS)return true;return false;};

//PACKAGE : MAP.
//FUNCTION : MYaz::ConvertObjectTo(int m_iNewType);
//DESCRIPTION : Functions of conversion of object to defined type.
//INPUTS : int.
//RETURNS : NONE.
	void ConvertObjectTo(int m_iNewType);

//PACKAGE : MAP.
//FUNCTION : MYaz::Copy(MYaz* pobject);
//DESCRIPTION : Function of copying.
//INPUTS : MYaz pointer.
//RETURNS : NONE.
	void Copy(MYaz* pobject);

//PACKAGE : MAP.
//FUNCTION : MYaz::Clone();
//DESCRIPTION : Function of cloning.
//INPUTS : NONE.
//RETURNS : MYaz pointer.
	MYaz* Clone();

//PACKAGE : MAP.
//FUNCTION : MYaz::GetBytes();
//DESCRIPTION : Calculates size in UTP-format.
//INPUTS : NONE.
//RETURNS : int.
	int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MYaz::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MYaz::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool  ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MYaz::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool  ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MYaz::~MYaz();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MYaz();

//PACKAGE : MAP.
//FUNCTION : MYaz::GetMaxDimension();
//DESCRIPTION : Returns max size as a sign.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension();

//PACKAGE : MAP.
//FUNCTION : MYaz::SetMasstKoeff(float fKoeff);
//DESCRIPTION : Sets scale factor.
//INPUTS : float.
//RETURNS : NONE.
	void SetMasstKoeff(float fKoeff){m_fMasstKoeff = fKoeff;};

//PACKAGE : MAP.
//FUNCTION : MYaz::GetMasstKoeff();
//DESCRIPTION : Sets scale factor.
//INPUTS : NONE.
//RETURNS : float.
	float GetMasstKoeff(){return m_fMasstKoeff;};
};

//PACKAGE : MAP.
//CLASS : MYzArray:public GlobalComplexArray<MYaz>.
//DESCRIPTION : Array of descriptions languages.
class MYzArray:public GlobalComplexArray<MYaz>
{
//PACKAGE : MAP.
//FUNCTION : MYzArray::CreateOfType();
//DESCRIPTION : Creates object - array of descriptions languages MYzArray.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MYzArray();};

//PACKAGE : MAP.
//FUNCTION : MYzArray::WhatTheArray();
//DESCRIPTION : Returns identifier of array of descriptions languages MYzArray.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_YZA;};

};

#endif //_YAZ__H_
