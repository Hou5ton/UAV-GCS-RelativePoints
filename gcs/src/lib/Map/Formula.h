//PACKAGE : MAP.
//FILE: Formula.h   
//AUTHOR:
//DESCRIPTION:

#ifndef NUXFORMULA_H
#define NUXFORMULA_H
#include "ArrBase.h"
#include "MString.h"

//=================================================================================
//!Base classes for formulas
//==================================================================================

//Identifiers for formula's. Types of MStrofa.

#define ID_STROFA_STROKA	0	// Inscription identifier
#define ID_STROFA_STOLBEC	1	// Column identifier
#define	ID_STROFA_COMPLEX	2	// Compound strophe identifier
#define ID_STROFA_SIMPLE	3	// Simple strophe identifier

#define ID_STROFA_DEFAULT_MEGA "" //!Suffix&preffix string by default

//!Attribute name which included in a strophe by default
//!In this case - first attribute from existing attributes dictionary
#if ENGVERSION == 1
	#define ID_STROFA_DEFAULT_POLE "Name" 
#else
	#define ID_STROFA_DEFAULT_POLE "Название" 
#endif

//PACKAGE : MAP.
//CLASS	: MStrofa.
//DESCRIPTION : Constituent part of formula (MFormula).
//				One of two types of fragments (MFragment),	which ones may be used in a formulas.
class MStrofa

{
private:
	MPgArray*		m_pPgArray;		// Sentences array of strophe
	unsigned char	m_typeStrofa;	// Strophe type

public:

//PACKAGE : MAP.
//FUNCTION : MStrofa::MStrofa();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MStrofa();

//PACKAGE : MAP.
//FUNCTION : MStrofa::MStrofa(unsigned char type_str);
//DESCRIPTION : Constructor by type.
//INPUTS : unsigned char .
//RETURNS : NONE.
	MStrofa(unsigned char type_str);

//PACKAGE : MAP.
//FUNCTION : MStrofa::MStrofa(unsigned char type_str, MPredlogenie* predlog);
//DESCRIPTION : Constructor by type and by one sentence.
//INPUTS : unsigned char , MPredlogenie pointer.
//RETURNS : NONE.
	MStrofa(unsigned char type_str, MPredlogenie* predlog);

//PACKAGE : MAP.
//FUNCTION : MStrofa::~MStrofa();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MStrofa();

//PACKAGE : MAP.
//FUNCTION : MStrofa::GetNomPrk(int index);
//DESCRIPTION : Returns dictionary index of sentences handwrite with number of 'index'.
//INPUTS : int .
//RETURNS : char.
	unsigned char GetNomPrk(int index);
	
//PACKAGE : MAP.
//FUNCTION : MStrofa::GetNomPole(int index);
//DESCRIPTION : Returns dictionary index(code) of sentence characteristic name with number of 'index'.
//INPUTS : int .
//RETURNS : short.
	unsigned short GetNomPole(int index);

//PACKAGE : MAP.
//FUNCTION : MStrofa::GetNomPrefix(int index);
//DESCRIPTION : Returns dictionary index(code) of sentence preffix with number of 'index'.
//INPUTS : int .
//RETURNS : char.
	unsigned char GetNomPrefix(int index);

//PACKAGE : MAP.
//FUNCTION : MStrofa::GetNomPostfix(int index);
//DESCRIPTION : Returns dictionary index(code) of sentence postfix with number of 'index'.
//INPUTS : int .
//RETURNS : char.
	unsigned char GetNomPostfix(int index);

//PACKAGE : MAP.
//FUNCTION : MStrofa::SetNomPrk(int index, unsigned char nomprk);
//DESCRIPTION : Sets dictionary index(code) of sentence handwrite with number of 'index'.
//INPUTS : int , unsigned char .
//RETURNS : NONE.
	void SetNomPrk(int index, unsigned char nomprk);

//PACKAGE : MAP.
//FUNCTION : MStrofa::SetNomPole(int index, unsigned short nompolename);
//DESCRIPTION : Sets dictionary index(code) of attribute with number of 'index'.
//INPUTS : int , unsigned short .
//RETURNS : NONE.
	void SetNomPole(int index, unsigned short nompolename);

//PACKAGE : MAP.
//FUNCTION : MStrofa::SetNomPrefix(int index, unsigned char nomprefix);
//DESCRIPTION : Sets dictionary index(code) of sentence preffix with number of 'index'.
//INPUTS : int , unsigned char .
//RETURNS : NONE.
	void SetNomPrefix(int index, unsigned char nomprefix);

//PACKAGE : MAP.
//FUNCTION : MStrofa::SetNomPostfix(int index, unsigned char nompostfix);
//DESCRIPTION : Sets dictionary index(code) of sentence postfix with number of 'index'.
//INPUTS : int , unsigned char .
//RETURNS : NONE.
	void SetNomPostfix(int index, unsigned char nompostfix);

//PACKAGE : MAP.
//FUNCTION : MStrofa::GetPredlogenieNum();
//DESCRIPTION : Returns sentences quantity of 'm_pPgArray' array.
//INPUTS : NONE.
//RETURNS : int.
	int GetPredlogenieNum();

//PACKAGE : MAP.
//FUNCTION : MStrofa::SetPredlogenie(int index, MPredlogenie* pPredlog);
//DESCRIPTION : Sets sentence data with established index.
//INPUTS : int , MPredlogenie pointer.
//RETURNS : True.
	bool SetPredlogenie(int index, MPredlogenie* pPredlog);

//PACKAGE : MAP.
//FUNCTION : MStrofa::DeletePredlogenie(int index);
//DESCRIPTION : Deletes sentence with established index.
//INPUTS : int .
//RETURNS : True.
	bool DeletePredlogenie(int index);

//PACKAGE : MAP.
//FUNCTION : MStrofa::AddPredlogenie(MPredlogenie* pPredlog);
//DESCRIPTION : Adds sentence to the strophe.
//INPUTS : MPredlogenie pointer.
//RETURNS : True.
	bool AddPredlogenie(MPredlogenie* pPredlog);

//PACKAGE : MAP.
//FUNCTION : MStrofa::SetType(unsigned char typenew);
//DESCRIPTION : Set strophe type.
//INPUTS : unsigned char .
//RETURNS : NONE.
	void SetType(unsigned char typenew);

//PACKAGE : MAP.
//FUNCTION : MStrofa::GetType();
//DESCRIPTION : Returns strophe type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	unsigned char GetType();

//PACKAGE : MAP.
//FUNCTION : MStrofa::IsEqualTo(MStrofa * pStrofaToCompare);
//DESCRIPTION : Strophe comparison with 'pStrofaToCompare'.
//INPUTS : MStrofa pointer.
//RETURNS : True.
	bool IsEqualTo(MStrofa * pStrofaToCompare);

//PACKAGE : MAP.
//FUNCTION : MStrofa::Copy(MStrofa* strofa);
//DESCRIPTION : Copies strophe.
//INPUTS : MStrofa pointer.
//RETURNS : NONE.
	void Copy(MStrofa* strofa);

//PACKAGE : MAP.
//FUNCTION : MStrofa::GetSize();
//DESCRIPTION : Gets strophe size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetSize();

//PACKAGE : MAP.
//FUNCTION : MStrofa::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MStrofa::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MStrofa::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool ReadBin(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MStrofa::operator = ;
//DESCRIPTION : Undefened operator =.
//INPUTS : MStrofa&.
//RETURNS : const MStrofa&.
	MStrofa& operator=(const MStrofa& in);
};


//PACKAGE : MAP.
//CLASS	: MFraction.
//DESCRIPTION : Class MFraction describes fractional fragment - constituent part of formula (MFormula).
//				One of two types of fragments (MFragment), which ones may be used in a formulas.
class MFraction

{
private:
	MStrofa		m_chisl;	//!< numenator
	MStrofa		m_znam;		//!< denominator

public:
	
//PACKAGE : MAP.
//FUNCTION : MFraction::MFraction();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MFraction();

//PACKAGE : MAP.
//FUNCTION : MFraction::MFraction(MStrofa* chisl, MStrofa* znam);
//DESCRIPTION : Constructor with established numenator and denominator.
//INPUTS : MStrofa pointer, MStrofa pointer.
//RETURNS : NONE.
	MFraction(MStrofa* chisl, MStrofa* znam);

//PACKAGE : MAP.
//FUNCTION : MFraction::~MFraction();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MFraction(){};

//PACKAGE : MAP.
//FUNCTION : MFraction::IsEqualTo(MFraction* m_pFractionToCompare);
//DESCRIPTION : Fractional fragment comparison with 'm_pFractionToCompare'.
//INPUTS : MFraction pointer.
//RETURNS : True.
	bool IsEqualTo(MFraction* m_pFractionToCompare);

//PACKAGE : MAP.
//FUNCTION : MFraction::Copy(MFraction* fraction);
//DESCRIPTION : Copies fractional fragment.
//INPUTS : MFraction pointer.
//RETURNS : NONE.
	void Copy(MFraction* fraction);

//PACKAGE : MAP.
//FUNCTION : MFraction::GetChisl();
//DESCRIPTION : Gets strophe - numenator.
//INPUTS : NONE.
//RETURNS : MStrofa pointer.
	MStrofa* GetChisl();

//PACKAGE : MAP.
//FUNCTION : MFraction::GetZnam();
//DESCRIPTION : Gets strophe - denominator.
//INPUTS : NONE.
//RETURNS : MStrofa pointer.
	MStrofa* GetZnam();

//PACKAGE : MAP.
//FUNCTION : MFraction::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetSize();

//PACKAGE : MAP.
//FUNCTION : MFraction::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Functions of writing to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MFraction::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Functions of reading to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version);
	
//PACKAGE : MAP.
//FUNCTION : MFraction::ReadBin(MFile* file, int version);
//DESCRIPTION : Functions of reading to UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MFraction::operator=;
//DESCRIPTION : Redefined operator = .
//INPUTS : MFraction&.
//RETURNS : const MFraction&.
	MFraction& operator=(const MFraction& in)
	{
		if(this!=&in)
			Copy((MFraction*)&in);
		return *this;
	};
};

// Identifiers for formula's. Types of MFragment.

#define ID_FRAG_STROFA	  0	// strophe identifier
#define ID_FRAG_FRACTION  1	// fractional part identifier

// Fragment name by default
#if ENGVERSION == 1
	#define ID_FRAG_NAZVANIE  "Fragment"	
#else
	#define ID_FRAG_NAZVANIE  "фрагмент"	
#endif

//PACKAGE : MAP.
//CLASS	: MFragment.
//DESCRIPTION : Describes fragment - constituent part of formula.
//				Fragment may be simple (MStrofa) or fractional (MFraction).
class MFragment

{
private:
	unsigned char	type_fragm;	// fragment type
	void*			m_fragm;	// pointer to fragment
public:
	
//PACKAGE : MAP.
//FUNCTION : MFragment::MFragment();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MFragment();

//PACKAGE : MAP.
//FUNCTION : MFragment::MFragment(unsigned char type_fragm);
//DESCRIPTION : Constructor by fragment type.
//INPUTS : unsigned char .
//RETURNS : NONE.
	MFragment(unsigned char type_fragm);

//PACKAGE : MAP.
//FUNCTION : MFragment::MFragment(MStrofa* strof);
//DESCRIPTION : Constructor by strophe.
//INPUTS : MStrofa pointer.
//RETURNS : NONE.
	MFragment(MStrofa* strof);

//PACKAGE : MAP.
//FUNCTION : MFragment::MFragment(MFraction* frac);
//DESCRIPTION : Constructor by fractional fragment.
//INPUTS : MFraction pointer.
//RETURNS : NONE.
	MFragment(MFraction* frac);

//PACKAGE : MAP.
//FUNCTION : MFragment::~MFragment();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MFragment();

//PACKAGE : MAP.
//FUNCTION : MFragment::GetFragment();
//DESCRIPTION : Returns pointer to fragment.
//INPUTS : NONE.
//RETURNS : void pointer.
	void* GetFragment();

//PACKAGE : MAP.
//FUNCTION : MFragment::Clone();
//DESCRIPTION : Creates fragment clone.
//INPUTS : NONE.
//RETURNS : MFragment pointer.
	MFragment * Clone();

//PACKAGE : MAP.
//FUNCTION : MFragment::Copy(MFragment* fragment);
//DESCRIPTION : Creates fragment copy.
//INPUTS : MFragment pointer.
//RETURNS : NONE.
	void Copy(MFragment* fragment);

//PACKAGE : MAP.
//FUNCTION : MFragment::GetStrofNum();
//DESCRIPTION : Returns strophe quantity(1 - for simple and 2 - for fraction).
//INPUTS : NONE.
//RETURNS : int.
	int GetStrofNum();

//PACKAGE : MAP.
//FUNCTION : MFragment::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetSize();

//PACKAGE : MAP.
//FUNCTION : MFragment::GetBytes();
//DESCRIPTION : Get bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetBytes(){return GetSize();};

//PACKAGE : MAP.
//FUNCTION : MFragment::SetType(unsigned char typenew);
//DESCRIPTION : Sets fragment type.
//INPUTS : unsigned char typenew.
//RETURNS : NONE.
	void SetType(unsigned char typenew);

//PACKAGE : MAP.
//FUNCTION : MFragment::GetType();
//DESCRIPTION : Gets fragment type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	unsigned char GetType();

//PACKAGE : MAP.
//FUNCTION : MFragment::IsEqualTo(MFragment * m_pFragmentToCompare);
//DESCRIPTION : Fragment comparison with 'm_pFragmentToCompare'.
//INPUTS : MFragment pointer.
//RETURNS : True.
	bool IsEqualTo(MFragment * m_pFragmentToCompare);

//PACKAGE : MAP.
//FUNCTION : MFragment::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MFragment::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MFragment::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads to UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool ReadBin(MFile* file, int version);  

//PACKAGE : MAP.
//FUNCTION : MFragment::Free();
//DESCRIPTION : Deletes fragment and frees memory.
//INPUTS : NONE.
//RETURNS : NONE.
	void Free();

};


//PACKAGE : MAP.
//CLASS : MFrArray.
//DESCRIPTION : Fragments array (for formulas).
class MFrArray:public GlobalComplexArray<MFragment>
{
public:
	
//PACKAGE : MAP.
//FUNCTION : MFrArray::CreateOfType();
//DESCRIPTION : Creates an array - object of 'MFrArray' class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MFrArray();};

//PACKAGE : MAP.
//FUNCTION : MFrArray::WhatTheArray();
//DESCRIPTION : Returns identifier of 'MFrArray' fragments array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_FRA;};

//PACKAGE : MAP.
//FUNCTION : MFrArray::GetStrofNum();
//DESCRIPTION : Returns strophes quantity of array.
//INPUTS : NONE.
//RETURNS : int.
	int GetStrofNum();
};

class MFormula;


//PACKAGE : MAP.
//CLASS : MFormArray.
//DESCRIPTION : Class - formulas array
class MFormArray:public GlobalComplexArray<MFormula>
{
public:

//PACKAGE : MAP.
//FUNCTION : MFormArray::CreateOfType();
//DESCRIPTION : Creates array - object of 'MFormArray' class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MFormArray();};

//PACKAGE : MAP.
//FUNCTION : MFormArray::WhatTheArray();
//DESCRIPTION : Returns identifier of 'MFormArray' formulas array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_FMA;};

//PACKAGE : MAP.
//FUNCTION : MFormArray::GetStrofNum();
//DESCRIPTION : Returns strophe quantity of array.
//INPUTS : NONE.
//RETURNS : int.
	int GetStrofNum();

//PACKAGE : MAP.
//FUNCTION : MFormArray::GetFormNum();
//DESCRIPTION : Returns formulas quantity of array.
//INPUTS : NONE.
//RETURNS : int.
	int GetFormNum();

//PACKAGE : MAP.
//FUNCTION : MFormArray::GetFormulaRange(int m_iFormulaIndex,int &minStrofa, int &maxStrofa);
//DESCRIPTION : Parameters 'minStrofa' and 'maxStrofa' are filled
//				by number of first and last strophe for the current formula accordingly.
//INPUTS : int ,int &, int &.
//RETURNS : NONE.
	void GetFormulaRange(int m_iFormulaIndex,int &minStrofa, int &maxStrofa);

//PACKAGE : MAP.
//FUNCTION : MFormArray::Copy(MFormArray* pArray)();
//DESCRIPTION : Creates copy of formulas array.
//INPUTS : MFormArray pointer.
//RETURNS : NONE.
	void Copy(MFormArray* pArray);
};


//!Identifiers for formula's. Types of MFormula.

#define ID_FORMULA_SIMPLE	0	//!Simple formula
//!#define ID_FORMULA_VINOSKA 1	//!Formula - marginal note
#define ID_FORMULA_OBRAML	2	//!Frame formula

//!Formulas name by default
//!Are setted for defined formula
#if ENGVERSION == 1
	#define ID_FORMULA_NAZVANIE "Formula"
#else	
	#define ID_FORMULA_NAZVANIE "Формула"
#endif

//PACKAGE : MAP.
//CLASS	: MFormula.
//DESCRIPTION : Formula defines how to draw inscriptions at map objects.
class MFormula

{
protected:
	int m_iType;			//!< Formula type
public:
	MString		nazvanie;	//!< Formula name
	MFrArray*	fragm_arr;	//!< Formula fragments array

//PACKAGE : MAP.
//FUNCTION : MFormula::MFormula();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MFormula();

//PACKAGE : MAP.
//FUNCTION : MFormula::MFormula(MFrArray* fr_arr);
//DESCRIPTION : Constructor with established fragments array.
//INPUTS : MFrArray pointer.
//RETURNS : NONE.
	MFormula(MFrArray* fr_arr);

//PACKAGE : MAP.
//FUNCTION : MFormula::~MFormula();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MFormula();

//PACKAGE : MAP.
//FUNCTION : MFormula::GetType();
//DESCRIPTION : Gets formula type.
//INPUTS : NONE.
//RETURNS : int.
	int GetType();

//PACKAGE : MAP.
//FUNCTION : MFormula::SetType(int m_iNewType);
//DESCRIPTION : Sets formula type.
//INPUTS : int .
//RETURNS : NONE.
	void SetType(int m_iNewType);

//PACKAGE : MAP.
//FUNCTION : MFormula::IsStructureEqualTo(MFormula* m_pMFormula);
//DESCRIPTION : Formula structure comparison with 'm_pMFormula' structure.
//INPUTS : MFormula pointer.
//RETURNS : True.
	bool IsStructureEqualTo(MFormula* m_pMFormula);

//PACKAGE : MAP.
//FUNCTION : MFormula::IsEqualTo(MFormula* m_pFormulaToCompare);
//DESCRIPTION : Formula comparison with 'm_pMFormula'.
//INPUTS : MFormula pointer.
//RETURNS : True.
	bool IsEqualTo(MFormula* m_pFormulaToCompare);

//PACKAGE : MAP.
//FUNCTION : MFormula::SetNazvanie(MString m_strNazv);
//DESCRIPTION : Sets formula name.
//INPUTS : MString.
//RETURNS : NONE.
	void SetNazvanie(MString m_strNazv);

//PACKAGE : MAP.
//FUNCTION : MFormula::GetStrofNum();
//DESCRIPTION : Returns strophe quantity of formula.
//INPUTS : NONE.
//RETURNS : int.
	int GetStrofNum();

//PACKAGE : MAP.
//FUNCTION : MFormula::Copy(MFormula* formula);
//DESCRIPTION : Copies formula.
//INPUTS : MFormula pointer.
//RETURNS : NONE.
	void Copy(MFormula* formula);

//PACKAGE : MAP.
//FUNCTION : MFormula::GetSize();
//DESCRIPTION :  GEts size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetSize();

//PACKAGE : MAP.
//FUNCTION : MFormula::GetBytes();
//DESCRIPTION : Get bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetBytes(){return GetSize();};

//PACKAGE : MAP.
//FUNCTION : MFormula::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile, int version);
//PACKAGE : MAP.
//FUNCTION : MFormula::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MFormula::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads to UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool ReadBin(MFile* file, int version);  

//PACKAGE : MAP.
//FUNCTION : MFormula::Free();
//DESCRIPTION : Deletes formula and frees memory.
//INPUTS : NONE.
//RETURNS : NONE.
	void Free();
};

#endif //NUXFORMULA_H

