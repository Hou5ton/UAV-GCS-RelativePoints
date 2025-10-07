//PACKAGE : MAP.
//FILE: ZnakSistema.h   
//AUTHOR:
//DESCRIPTION:


#ifndef MZNAKSISTEMA_H
#define MZNAKSISTEMA_H
#include "Yaz.h"
#include "Id.h"
#include "MString.h"

// constants of attributes of new classifier
#if ENGVERSION == 1
	#define KLS_NAZV "Name"
	#define KLS_FULL_NAME "Full Name"
	#define KLS_COMMENT "Comment"

#else
	#define KLS_NAZV "Название"
	#define KLS_FULL_NAME "Полное название"
	#define KLS_COMMENT "Комментарий"
#endif

//PACKAGE : MAP.
//CLASS : MTermin.
//DESCRIPTION : Class for the work with unique four-line structure of identification of classifier, sign system,	parts and conventional signs.
class MTermin
{
private:
	//  code
	MString m_kod;

	//  name
	MString m_shortname;

	//  full name
	MString m_fullname;

	//  comment
	MString m_remarka;

public:

//PACKAGE : MAP.
//FUNCTION : MTermin::MTermin();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MTermin();

//PACKAGE : MAP.
//FUNCTION : MTermin::MTermin(MString kod, MString shortname, MString fullname, MString remarka);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MString , MString , MString , MString .
//RETURNS : NONE.
	MTermin(MString kod, MString shortname, MString fullname, MString remarka);

//PACKAGE : MAP.
//FUNCTION : MTermin::~MTermin();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MTermin(){};

//PACKAGE : MAP.
//FUNCTION : MTermin::operator ==;
//DESCRIPTION : Redefinition of operator of equal.
//INPUTS : const MTermin&.
//RETURNS : True.
	bool operator ==( const MTermin& toComp ) const;

//PACKAGE : MAP.
//FUNCTION : MTermin::GetBytes();
//DESCRIPTION : Function of size definition in UTP.
//INPUTS : NONE.
//RETURNS : int.
	int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MTermin::Copy(MTermin* pTermin);
//DESCRIPTION : Function copies to itself fields of 'pTermin'.
//INPUTS : MTermin pointer.
//RETURNS : NONE.
	void Copy(MTermin* pTermin);

//PACKAGE : MAP.
//FUNCTION : MTermin::Clone();
//DESCRIPTION : Function creates exact copy of term.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* Clone();

//PACKAGE : MAP.
//FUNCTION : MTermin::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MTermin::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MTermin::ReadBin(MFile* file,  int version);
//DESCRIPTION : Constructor.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MTermin::IsEqualTo(MTermin* pTermin);
//DESCRIPTION : Returns true, if 'pTermun' the same as it is.
//INPUTS : MTermin pointer.
//RETURNS : True.
	bool IsEqualTo(MTermin* pTermin);

//PACKAGE : MAP.
//FUNCTION : MTermin::SetKod(MString kod);
//DESCRIPTION : Function of felds initialization.
//INPUTS : MString.
//RETURNS : NONE.
	void SetKod(MString kod)
	{
		kod.TrimLeft();
		kod.TrimRight();

		// The code cannot be empty in a term
		if(kod!="")
		{
			// During the test we need code of termn to don't contain blanks
			while(kod.Find(" ",0)!=-1){
				int iIndex = kod.Find(" ",0);
				kod.Delete(iIndex);
			}
			m_kod=kod;
		}
		else

			m_kod="NEW";
	};

//PACKAGE : MAP.
//FUNCTION : MTermin::SetShortname(MString shortname);
//DESCRIPTION : Enters short name in a term.
//INPUTS : MString.
//RETURNS : NONE.
	void SetShortname(MString shortname){m_shortname=shortname;};

//PACKAGE : MAP.
//FUNCTION : MTermin::SetFullname(MString fullname);
//DESCRIPTION : Enters full name in a term.
//INPUTS : MString.
//RETURNS : NONE.
	void SetFullname(MString fullname){m_fullname=fullname;};

//PACKAGE : MAP.
//FUNCTION : MTermin::SetRemarka(MString remarka);
//DESCRIPTION : Enters comment in a term.
//INPUTS : MString.
//RETURNS : NONE.
	void SetRemarka(MString remarka){m_remarka=remarka;};
	
	// Functions returns fields values

//PACKAGE : MAP.
//FUNCTION : MTermin::GetKod();
//DESCRIPTION : Returns code from term.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetKod(){return m_kod;};

//PACKAGE : MAP.
//FUNCTION : MTermin::GetShortname();
//DESCRIPTION : Returns short name from term.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetShortname(){return m_shortname;};

//PACKAGE : MAP.
//FUNCTION : MTermin::GetFullname();
//DESCRIPTION : Returns full name from term.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetFullname(){return m_fullname;};

//PACKAGE : MAP.
//FUNCTION : MTermin::GetRemarka();
//DESCRIPTION : Returns comment from term.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetRemarka(){return m_remarka;};
};

//PACKAGE : MAP.
//CLASS : MZnak.
//DESCRIPTION : Classifier sign. The class works with a set of variables and functions,
//				allowing to create and store in the classifier
//				the unique description of a conventional sign (a way of drawing and
//				a set of attributes). Created (or available) conventional sign
//				subsequently contacts to object (MTopobject) on a map.
class MZnak
{
	friend class MZnArray;
private:
	//  sign term
	MTermin		m_termin;

	//  Identifier of object class, for which we are creating a sign
	MString		m_class;

	//  Array of indexes of fields data names in a dictionary
	MShInArray* m_noms;

	//  Array of group characteristics and their names in a dictionary
	MNvArray*	m_genattrs;

	//  Pointer to array of languages
	MYzArray*	m_YazArray;

public:

//PACKAGE : MAP.
//FUNCTION : MZnak::SetMasstKoeff(float fKoeff, int nomyaz = 0);
//DESCRIPTION : Sets scale factor.
//INPUTS : float , int .
//RETURNS : NONE.
	void SetMasstKoeff(float fKoeff, int nomyaz = 0){if((nomyaz<0)||(nomyaz>=m_YazArray->GetSize())) return;
		m_YazArray->GetAt(nomyaz)->SetMasstKoeff(fKoeff);};

//PACKAGE : MAP.
//FUNCTION : MZnak::GetMasstKoeff(int nomyaz = 0);
//DESCRIPTION : Returns scale factor.
//INPUTS : int.
//RETURNS : float.
	float GetMasstKoeff(int nomyaz = 0){if((nomyaz<0)||(nomyaz>=m_YazArray->GetSize())) return -1;
		return m_YazArray->GetAt(nomyaz)->GetMasstKoeff();};

//PACKAGE : MAP.
//FUNCTION : MZnak::MZnak();
//DESCRIPTION : Constructor by default creates
// 				description of linear object with null thickness.
//INPUTS : NONE.
//RETURNS : NONE.
	MZnak();

//PACKAGE : MAP.
//FUNCTION : MZnak::MZnak(MTermin termin, MYzArray* YazArray,MShInArray* pNomAttrNames = NULL,MNvArray* pGroupAttr = NULL);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : MTermin , MYzArray pointer,MShInArray pointer, MNvArray pointer.
//RETURNS : NONE.
	MZnak(MTermin termin, MYzArray* YazArray,MShInArray* pNomAttrNames = NULL,
		MNvArray* pGroupAttr = NULL);

//PACKAGE : MAP.
//FUNCTION : MZnak::MZnak(MZnak* znak);
//DESCRIPTION : Constructor with established standart.
//INPUTS : MZnak pointer.
//RETURNS : NONE.
	MZnak(MZnak* znak);

//PACKAGE : MAP.
//FUNCTION : MZnak::~MZnak();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MZnak();

//PACKAGE : MAP.
//FUNCTION : MZnak::GetBytes();
//DESCRIPTION : Returns record length to NUX.
//INPUTS : NONE.
//RETURNS : unsigned int.
	unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MZnak::Copy(MZnak* znak);
//DESCRIPTION : Function copies to itself containing of 'znak'.
//INPUTS : MZnak pointer.
//RETURNS : NONE.
	void Copy(MZnak* znak);

//PACKAGE : MAP.
//FUNCTION : MZnak::IsTerminEqualTo(MZnak* znak);
//DESCRIPTION : Checks if term of 'znak' coinciding.
//INPUTS : MZnak pointer.
//RETURNS : True.
	bool IsTerminEqualTo(MZnak* znak);

//PACKAGE : MAP.
//FUNCTION : MZnak::Clone();
//DESCRIPTION : Makes a copy.
//INPUTS : NONE.
//RETURNS : MZnak pointer.
	MZnak* Clone();

//PACKAGE : MAP.
//FUNCTION : MZnak::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MZnak::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MZnak::ReadBin(MFile* file,  int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	bool ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MZnak::GetClassType();
//DESCRIPTION : Returns class type.
//INPUTS : NONE.
//RETURNS : MString.
	MString GetClassType(){return m_class;};

//PACKAGE : MAP.
//FUNCTION : MZnak::SetClassType(MString classtype);
//DESCRIPTION : Sets class type.
//INPUTS : MString.
//RETURNS : NONE.
	void SetClassType(MString classtype){m_class = classtype;};

//PACKAGE : MAP.
//FUNCTION : MZnak::CreateCopyTermin();
//DESCRIPTION : Returns copy of term.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* CreateCopyTermin();

//PACKAGE : MAP.
//FUNCTION : MZnak::GetTermin();
//DESCRIPTION : Returns terms.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
        MTermin* GetTermin();

//PACKAGE : MAP.
//FUNCTION : MZnak::GetYaz(int nomyaz = 0);
//DESCRIPTION : Returns pointer to language by input number.
//INPUTS : int.
//RETURNS : MYaz pointer.
        MYaz* GetYaz(int nomyaz = 0)const;

//PACKAGE : MAP.
//FUNCTION : MZnak::SetTermin(MTermin termin);
//DESCRIPTION : Sets term.
//INPUTS : MTermin.
//RETURNS : NONE.
	void SetTermin(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnak::GetNomsArray();
//DESCRIPTION : Returns pointer to array of characteristics.
//INPUTS : NONE.
//RETURNS : MShInArray pointer.
        MShInArray* GetNomsArray() const;

//PACKAGE : MAP.
//FUNCTION : MZnak::GetGenattrArray();
//DESCRIPTION :  Returns pointer to array of values of similar characteristics.
//INPUTS : NONE.
//RETURNS : MNvArray pointer.
        MNvArray* GetGenattrArray()const;

//PACKAGE : MAP.
//FUNCTION : MZnak::GetYazArray();
//DESCRIPTION : Returns pointer to array of languages.
//INPUTS : NONE.
//RETURNS : MYzArray pointer.
        MYzArray* GetYazArray()const;

//PACKAGE : MAP.
//FUNCTION : MZnak::SetNomsArray(MShInArray* noms);
//DESCRIPTION : Sets array of characteristics.
//INPUTS : MShInArray pointer.
//RETURNS : NONE.
	void SetNomsArray(MShInArray* noms);

//PACKAGE : MAP.
//FUNCTION : MZnak::SetGenattrArray(MNvArray* genattrs);
//DESCRIPTION : Sets array of similar values of characteristics.
//INPUTS : MNvArray pointer.
//RETURNS : NONE.
	void SetGenattrArray(MNvArray* genattrs);

//PACKAGE : MAP.
//FUNCTION : MZnak::SetYaz(MYaz* pYaz, int nom = 0);
//DESCRIPTION : Sets language with number 'nom' in array of languages.
//INPUTS : MYaz pointer, int .
//RETURNS : True.
	bool SetYaz(MYaz* pYaz, int nom = 0);

//PACKAGE : MAP.
//FUNCTION : MZnak::SetYazArray(MYzArray* YazArray);
//DESCRIPTION : Sets array of languages.
//INPUTS : MYzArray pointer.
//RETURNS : NONE.
	void SetYazArray(MYzArray* YazArray);

//PACKAGE : MAP.
//FUNCTION : MZnak::IsColorkod(unsigned char colorkod);
//DESCRIPTION : Analyzes using of color in a sign.
//INPUTS : unsigned char.
//RETURNS : True.
	bool IsColorkod(unsigned char colorkod);

//PACKAGE : MAP.
//FUNCTION : MZnak::IsPrkkod(unsigned char pocherkkod);
//DESCRIPTION : Analyzes using of handwrite in a sign.
//INPUTS : unsigned char.
//RETURNS : True.
	bool IsPrkkod(unsigned char pocherkkod);

//PACKAGE : MAP.
//FUNCTION : MZnak::IsMegstrkod(unsigned char megstrkod);
//DESCRIPTION : Analyzes using of suffix and preffix string in a sign.
//INPUTS : unsigned char.
//RETURNS : True.
	bool IsMegstrkod(unsigned char megstrkod);

//PACKAGE : MAP.
//FUNCTION : MZnak::IsCharkod(unsigned short charkod);
//DESCRIPTION : Analyzes using of characteristic in a sign.
//INPUTS : unsigned char.
//RETURNS : True.
	bool IsCharkod(unsigned short charkod);
};

//PACKAGE : MAP.
//CLASS : MZnArray:public GlobalComplexArray<MZnak>.
//DESCRIPTION : Class - array of signs.
class MZnArray:public GlobalComplexArray<MZnak>
{
public:

//PACKAGE : MAP.
//FUNCTION : MZnArray::CreateOfType();
//DESCRIPTION : Object creation - array of signs 'MZnArray'.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MZnArray();};

//PACKAGE : MAP.
//FUNCTION : MZnArray::WhatTheArray();
//DESCRIPTION : Identification of array of signs.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_ZNA;};

//PACKAGE : MAP.
//FUNCTION : MZnArray::Copy(MZnak* In,MZnak* Out);
//DESCRIPTION : Makes a copy of array of signs.
//INPUTS : MZnak pointer,MZnak pointer.
//RETURNS : NONE.
	virtual void Copy(MZnak* In,MZnak* Out);

//PACKAGE : MAP.
//FUNCTION : MZnArray::Clone();
//DESCRIPTION : Clones array of signs.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* Clone();
};

//PACKAGE : MAP.
//CLASS : MRazdel.
//DESCRIPTION : Class - Partition of classifier.
//				The section serves as storehouse for marks of the classifier.
//				At constructing of classifier by user,
//				as a rule, in one section we unite signs, concerning
//				to the certain subject domain (for example, conventional signs
//				for drawing of vegetation).

class MRazdel
{
	friend class MRlArray;

private:
	float m_fMasstKoeff;	// scale factor of conventional signs of the partition

	//  Term of partition
	MTermin m_termin;

	//  Pointer to array of signs
	MZnArray* p_znaki;

public:

//PACKAGE : MAP.
//FUNCTION : MRazdel::UpdateKoeff();
//DESCRIPTION : Functions of work with scale coefficients.
//INPUTS : NONE.
//RETURNS : NONE.
	void UpdateKoeff();

//PACKAGE : MAP.
//FUNCTION : MRazdel::SetMasstKoeff(float fKoeff);
//DESCRIPTION : Sets scale factor.
//INPUTS : float.
//RETURNS : NONE.
	void SetMasstKoeff(float fKoeff){m_fMasstKoeff = fKoeff;UpdateKoeff();};

//PACKAGE : MAP.
//FUNCTION : MRazdel::MRazdel();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MRazdel();

//PACKAGE : MAP.
//FUNCTION : MRazdel::MRazdel(MTermin termin,MZnArray* zna);
//DESCRIPTION : Constructor by established parameters.
//INPUTS : MTermin ,MZnArray pointer.
//RETURNS : NONE.
	MRazdel(MTermin termin,MZnArray* zna);

//PACKAGE : MAP.
//FUNCTION : MRazdel::MRazdel(MRazdel* razdel);
//DESCRIPTION : Constructor with established standart.
//INPUTS : MRazdel pointer.
//RETURNS : NONE.
	MRazdel(MRazdel* razdel);

//PACKAGE : MAP.
//FUNCTION : MRazdel::~MRazdel();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MRazdel();

//PACKAGE : MAP.
//FUNCTION : MRazdel::Copy(MRazdel* razdel);
//DESCRIPTION : Copying.
//INPUTS : MRazdel pointer.
//RETURNS : NONE.
	void Copy(MRazdel* razdel);

//PACKAGE : MAP.
//FUNCTION : MRazdel::Clone();
//DESCRIPTION : Clones.
//INPUTS : NONE.
//RETURNS : MRazdel pointer.
	MRazdel* Clone();

//PACKAGE : MAP.
//FUNCTION : MRazdel::GetBytes();
//DESCRIPTION : Calculates size.
//INPUTS : NONE.
//RETURNS : unsigned int.
	unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MRazdel::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from NUX-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MRazdel::ReadBin(MFile* file,int version);
//DESCRIPTION : Reads from NUX-file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	bool ReadBin(MFile* file,int version);

//PACKAGE : MAP.
//FUNCTION : MRazdel::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to NUX-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MRazdel::IsTerminEqualTo(MRazdel* razdel);
//DESCRIPTION : Compares partitions by terms.
//INPUTS : MRazdel pointer.
//RETURNS : True.
	bool IsTerminEqualTo(MRazdel* razdel);

//PACKAGE : MAP.
//FUNCTION : MRazdel::IsTerminEqualTo(MTermin termin);
//DESCRIPTION : Term comparison.
//INPUTS : MTermin.
//RETURNS : True.
	bool IsTerminEqualTo(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MRazdel::GetTermin();
//DESCRIPTION : Gets term.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
        MTermin* GetTermin();

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateCopyTermin();
//DESCRIPTION : Gets term copy.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* CreateCopyTermin();

//PACKAGE : MAP.
//FUNCTION : MRazdel::SetTermin(MTermin termin);
//DESCRIPTION : Sets term.
//INPUTS : MTermin.
//RETURNS : NONE.
	void SetTermin(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MRazdel::GetZnakArray();
//DESCRIPTION : Gets array of signs.
//INPUTS : NONE.
//RETURNS : MZnArray pointer.
	MZnArray* GetZnakArray();

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateCopyZnakArray();
//DESCRIPTION : Gets copy of array of signs.
//INPUTS : NONE.
//RETURNS : MZnArray pointer.
	MZnArray* CreateCopyZnakArray();

//PACKAGE : MAP.
//FUNCTION : MRazdel::SetZnakArray(MZnArray* zna);
//DESCRIPTION : Sets array of signs.
//INPUTS : MZnArray pointer.
//RETURNS : NONE.
	void SetZnakArray(MZnArray* zna);

//PACKAGE : MAP.
//FUNCTION : MRazdel::AppendZnakArrayHead(MZnArray* zna);
//DESCRIPTION : Adds array of signs to the beginning.
//INPUTS : MZnArray pointer.
//RETURNS : NONE.
	void AppendZnakArrayHead(MZnArray* zna);

//PACKAGE : MAP.
//FUNCTION : MRazdel::AppendZnakArrayTail(MZnArray* zna);
//DESCRIPTION : Adds array of signs to the end.
//INPUTS : MZnArray pointer.
//RETURNS : NONE.
	void AppendZnakArrayTail(MZnArray* zna);

//PACKAGE : MAP.
//FUNCTION : MRazdel::ClearZnakArray();
//DESCRIPTION : Frees array of signs.
//INPUTS : NONE.
//RETURNS : NONE.
	void ClearZnakArray();

//PACKAGE : MAP.
//FUNCTION : MRazdel::GetZnak(MTermin termin);
//DESCRIPTION : Gets sign with such term or NULL if it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnak pointer.
	MZnak* GetZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MRazdel::GetZnak(MString kod);
//DESCRIPTION : Gets sign with such code or NULL if it does not exist.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* GetZnak(MString kod);

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateCopyZnak(MTermin termin);
//DESCRIPTION : Gets copy of sign with such term or NULL if it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnak pointer.
	MZnak* CreateCopyZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateCopyZnak(MString kod);
//DESCRIPTION : Gets copy of sign with such code or NULL if it does not exist.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* CreateCopyZnak(MString kod);

//PACKAGE : MAP.
//FUNCTION : MRazdel::DeleteZnak(MTermin termin);
//DESCRIPTION : Deletes sign with such term.
//INPUTS : MTermin.
//RETURNS : NONE.
	void DeleteZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MRazdel::DeleteZnak(MString kod);
//DESCRIPTION : Deletes sign with such code.
//INPUTS : MString.
//RETURNS : NONE.
	void DeleteZnak(MString kod);

//PACKAGE : MAP.
//FUNCTION : MRazdel::AddZnak(MZnak* znak);
//DESCRIPTION : Adds such sign.
//INPUTS : MZnak pointer.
//RETURNS : MZnak pointer.
	MZnak* AddZnak(MZnak* znak);

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateNewZnak(MTermin termin,MString type);
//DESCRIPTION : Creates new sign with such term.
//INPUTS : MTermin ,MString .
//RETURNS : MZnak pointer.
	MZnak* CreateNewZnak(MTermin termin,MString type);

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateNewZnak(MString kod);
//DESCRIPTION : Creates new sign with such code.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* CreateNewZnak(MString kod);

//PACKAGE : MAP.
//FUNCTION : MRazdel::CreateNewZnak();
//DESCRIPTION : Creates new sign by deafult.
//INPUTS : NONE.
//RETURNS : MZnak pointer.
	MZnak* CreateNewZnak();

//PACKAGE : MAP.
//FUNCTION : MRazdel::GetZnCount(MString strID = "");
//DESCRIPTION : Returns signs quantity of partition.
//INPUTS : MString.
//RETURNS : int.
	int GetZnCount(MString strID = "");
};

//PACKAGE : MAP.
//CLASS : MRlArray:public GlobalComplexArray<MRazdel>.
//DESCRIPTION : Class -  array of partitions.
class MRlArray:public GlobalComplexArray<MRazdel>
{
public:

//PACKAGE : MAP.
//FUNCTION : MRlArray::CreateOfType();
//DESCRIPTION : Object creation - array of partitions 'MRlArray'.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MRlArray();};

//PACKAGE : MAP.
//FUNCTION : MRlArray::WhatTheArray();
//DESCRIPTION : Identification of array of partitions 'MRlArray'.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_RLA;};

//PACKAGE : MAP.
//FUNCTION : MRlArray::Copy(MRazdel* In,MRazdel* Out);
//DESCRIPTION : Function of copying.
//INPUTS : MRazdel pointer,MRazdel pointer.
//RETURNS : NONE.
	virtual void Copy(MRazdel* In,MRazdel* Out);

//PACKAGE : MAP.
//FUNCTION : MRlArray::Clone();
//DESCRIPTION : Function of cloning.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* Clone();
};


//PACKAGE : MAP.
//CLASS : MZnakSistema.
//DESCRIPTION : Class - sign system.
//				Serves as storehouse for sections of the classifier. In sign system we
//				unite the sections containing conventional signs of one
//				scale. For example, " sign system for scale of 1:500 000 "
//				or " sign system for scale of 1:25 000 "

class MZnakSistema
{
	friend class MZsArray;
private:
	//  Scale of sign system
	int m_iRealMasstab;

	// Conversion coefficient of millimetre of the map to geographic unit
	float m_fMasstKoeff;

	//  Line, which contain projection name
	MString m_strProection;

private:
	//  Term of partition
	MTermin m_termin;

	//  Pointer to array of signs
	MRlArray* p_razdeli;

public:

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::MZnakSistema();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MZnakSistema();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::MZnakSistema(MTermin termin,MRlArray* rla);
//DESCRIPTION : Constructor by established parameters.
//INPUTS : MTermin ,MRlArray pointer.
//RETURNS : NONE.
	MZnakSistema(MTermin termin,MRlArray* rla);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::MZnakSistema(MZnakSistema* znsys);
//DESCRIPTION : Constructor with established standart.
//INPUTS : MZnakSistema pointer.
//RETURNS : NONE.
	MZnakSistema(MZnakSistema* znsys);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::~MZnakSistema();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MZnakSistema(){if(p_razdeli)delete p_razdeli;};

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::Copy(MZnakSistema* znsys);
//DESCRIPTION : Copying.
//INPUTS : MZnakSistema pointer.
//RETURNS : NONE.
	void Copy(MZnakSistema* znsys);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::Clone();
//DESCRIPTION : Clones.
//INPUTS : NONE.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* Clone();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetBytes();
//DESCRIPTION : Calculates size.
//INPUTS : NONE.
//RETURNS : unsigned int.
	unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from NUX-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::ReadBin(MFile* file,int version);
//DESCRIPTION : Reads from NUX-file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	bool ReadBin(MFile* file,int version);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Saves to NUX-file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::IsTerminEqualTo(MZnakSistema* znsys);
//DESCRIPTION : Comparison of sign systems by terms.
//INPUTS : MZnakSistema pointer.
//RETURNS : True.
	bool IsTerminEqualTo(MZnakSistema* znsys);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::IsTerminEqualTo(MTermin termin);
//DESCRIPTION : Term comparison.
//INPUTS : MTermin.
//RETURNS : True.
	bool IsTerminEqualTo(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetTermin();
//DESCRIPTION : Gets term.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* GetTermin();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateCopyTermin();
//DESCRIPTION : Gets term copy.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* CreateCopyTermin();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::SetTermin(MTermin termin);
//DESCRIPTION : Sets term.
//INPUTS : MTermin.
//RETURNS : NONE.
	void SetTermin(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetRazdelArray();
//DESCRIPTION : Gets array of partitions.
//INPUTS : NONE.
//RETURNS : MRlArray pointer.
	MRlArray* GetRazdelArray();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateCopyRazdelArray();
//DESCRIPTION : Gets copy of array of partitions.
//INPUTS : NONE.
//RETURNS : MRlArray pointer.
	MRlArray* CreateCopyRazdelArray();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::SetRazdelArray(MRlArray* rla);
//DESCRIPTION : Sets array of partitions.
//INPUTS : MRlArray pointer.
//RETURNS : NONE.
	void SetRazdelArray(MRlArray* rla);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::AppendRazdelArrayHead(MRlArray* rla);
//DESCRIPTION : Adds array of partitions to the beginning.
//INPUTS : MRlArray pointer.
//RETURNS : NONE.
	void AppendRazdelArrayHead(MRlArray* rla);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::AppendRazdelArrayTail(MRlArray* rla);
//DESCRIPTION : Adds array of partitions to the end.
//INPUTS : MRlArray pointer.
//RETURNS : NONE.
	void AppendRazdelArrayTail(MRlArray* rla);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::ClearRazdelArray();
//DESCRIPTION : Frees array of partitions.
//INPUTS : NONE.
//RETURNS : NONE.
	void ClearRazdelArray();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetRazdel(MTermin termin);
//DESCRIPTION : Gets partition woth such term or NULL if it doesn't exist.
//INPUTS : MTermin.
//RETURNS : MRazdel pointer.
	MRazdel* GetRazdel(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetRazdel(MString kod);
//DESCRIPTION : Gets partition woth such code or NULL if it doesn't exist.
//INPUTS : MString.
//RETURNS : MRazdel pointer.
	MRazdel* GetRazdel(MString kod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateCopyRazdel(MTermin termin);
//DESCRIPTION : Gets copy of partition woth such term or NULL if it doesn't exist.
//INPUTS : MTermin.
//RETURNS : MRazdel pointer.
	MRazdel* CreateCopyRazdel(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateCopyRazdel(MString kod);
//DESCRIPTION : Gets copy of partition woth such code or NULL if it doesn't exist.
//INPUTS : MString.
//RETURNS : MRazdel pointer.
	MRazdel* CreateCopyRazdel(MString kod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::DeleteRazdel(MTermin termin);
//DESCRIPTION : Deletes partition with such term.
//INPUTS : MTermin.
//RETURNS : NONE.
	void DeleteRazdel(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::DeleteRazdel(MString kod);
//DESCRIPTION : Deletes partition with such code.
//INPUTS : MString.
//RETURNS : NONE.
	void DeleteRazdel(MString kod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::AddRazdel(MRazdel* Razdel);
//DESCRIPTION : Adds such partition.
//INPUTS : MRazdel pointer.
//RETURNS : MRazdel pointer.
	MRazdel* AddRazdel(MRazdel* Razdel);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateNewRazdel(MTermin termin);
//DESCRIPTION : Creates new partition with such term.
//INPUTS : MTermin.
//RETURNS : MRazdel pointer.
	MRazdel* CreateNewRazdel(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateNewRazdel(MString kod);
//DESCRIPTION : Creates new partition with such code.
//INPUTS : MString.
//RETURNS : MRazdel pointer.
	MRazdel* CreateNewRazdel(MString kod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateNewRazdel();
//DESCRIPTION : Creates new partition by default.
//INPUTS : NONE.
//RETURNS : MRazdel pointer.
	MRazdel* CreateNewRazdel();

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetZnak(MTermin termin);
//DESCRIPTION : Gets sign with such term or NULL if it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnak pointer.
	MZnak* GetZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetZnak(MString dukod);
//DESCRIPTION : Gets sign with such double code or NULL if it doesn't exist.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* GetZnak(MString dukod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateCopyZnak(MTermin termin);
//DESCRIPTION : Gets copy of sign with such term or NULL if it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnak pointer.
	MZnak* CreateCopyZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateCopyZnak(MString dukod);
//DESCRIPTION : Gets copy of sign with such double code or NULL if it doesn't exist.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* CreateCopyZnak(MString dukod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::CreateNewZnak(MString dukod);
//DESCRIPTION : Creates new sign with such double code.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* CreateNewZnak(MString dukod);

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetZnCount(MString strID = "");
//DESCRIPTION : Returns elements quantity in sign system.
//INPUTS : MString.
//RETURNS : int.
	int GetZnCount(MString strID = "");

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::UpdateKoeff();
//DESCRIPTION : Work with scale factor.
//INPUTS : NONE.
//RETURNS : NONE.
	void UpdateKoeff(){for(int i =p_razdeli->GetSize()-1;i>=0;i--)
						p_razdeli->GetAt(i)->SetMasstKoeff(m_fMasstKoeff);}

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::SetMasstKoeff(float fKoeff);
//DESCRIPTION : Sets scale factor.
//INPUTS : float.
//RETURNS : NONE.
	void SetMasstKoeff(float fKoeff){m_fMasstKoeff = fKoeff;UpdateKoeff();};

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetMasstKoeff();
//DESCRIPTION : Gets scale factor.
//INPUTS : NONE.
//RETURNS : float.
	float GetMasstKoeff(){return m_fMasstKoeff;};

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::SetRealMasst(int iRelaMasst);
//DESCRIPTION : Sets real scale.
//INPUTS : int.
//RETURNS : NONE.
	void SetRealMasst(int iRelaMasst){m_iRealMasstab = iRelaMasst;};

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetRealMasst;
//DESCRIPTION : Gets real scale.
//INPUTS : NONE.
//RETURNS : int.
	int GetRealMasst(){return m_iRealMasstab;};

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::SetProection(MString& strProection);
//DESCRIPTION : Sets prohection (of Gauss-Kruger or metric).
//INPUTS : MString&.
//RETURNS : NONE.
	void SetProection(MString& strProection){m_strProection = strProection;};

//PACKAGE : MAP.
//FUNCTION : MZnakSistema::GetProection();
//DESCRIPTION : Gets prohection (of Gauss-Kruger or metric).
//INPUTS : NONE.
//RETURNS : MString.
	MString GetProection(){return m_strProection;};
};

//PACKAGE : MAP.
//CLASS : MZsArray:public GlobalComplexArray<MZnakSistema>.
//DESCRIPTION : Class -  array of sign systems.
class MZsArray:public GlobalComplexArray<MZnakSistema>
{
public:

//PACKAGE : MAP.
//FUNCTION : MZsArray::CreateOfType();
//DESCRIPTION : Object creation - array of sign system 'MZsArray'.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MZsArray();};

//PACKAGE : MAP.
//FUNCTION : MZsArray::WhatTheArray();
//DESCRIPTION : Returns line identifier of array of sign systems 'MZsArray'.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_ZSA;};

//PACKAGE : MAP.
//FUNCTION : MZsArray::Copy(MZnakSistema* In,MZnakSistema* Out);
//DESCRIPTION : Function of copying.
//INPUTS : MZnakSistema pointer,MZnakSistema pointer.
//RETURNS : NONE.
	virtual void Copy(MZnakSistema* In,MZnakSistema* Out);

//PACKAGE : MAP.
//FUNCTION : MZsArray::Clone();
//DESCRIPTION : Makes a clone for 'Undo' operation.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* Clone();
};

#endif //MZNAKSISTEMA_H
