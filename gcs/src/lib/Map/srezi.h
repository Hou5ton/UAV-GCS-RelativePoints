//PACKAGE : MAP.
//FILE: Srezi.h   
//AUTHOR:
//DESCRIPTION:

#ifndef SREZI_H
#define SREZI_H
#include "BaseCl.h"
#include "ArrBase.h"
#include "Prosloiki.h"
#include "MString.h"
#include "MImage.h"

// BASE TYPE FOR DRAWING OF DOTTED OBJECT
//=========================================================
// CONSTANTS FOR DOTTED OBJECTS TYPES
//  identifier of dotted object
#define IDT_SHRIFT		0

//  indentifier of random dotted object
#define IDT_PROIZVOL	1

#define IDT_NAS_SHRIFT	2

#if ENGVERSION == 1 // English version
	#define IDT_SHRIFT_NAME	   "Symbol"	//  identifier of dotted object
	#define IDT_PROIZVOL_NAME  "Icon"	//  indentifier of random dotted object
	#define IDT_NAS_SHRIFT_NAME "Symbol(Adjustable)"

#else //russian version
	#define IDT_SHRIFT_NAME	   "Символ"	// identifier of dotted object
	#define IDT_PROIZVOL_NAME  "Пиктограмма"	// indentifier of random dotted object
	#define IDT_NAS_SHRIFT_NAME "Символ (нас.)"
#endif

//PACKAGE : MAP.
//CLASS : MSrez.
//DESCRIPTION : Class MSrez is vase class for description of graphic primitives, which ones help to representative dotted object.
class MSrez
{
public:

//PACKAGE : MAP.
//FUNCTION : MSrez::MSrez();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MSrez(){};

//PACKAGE : MAP.
//FUNCTION : MSrez::GetBytes();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetBytes(){return GetSize();};

//PACKAGE : MAP.
//FUNCTION : MSrez::GetType();
//DESCRIPTION : Function of redefining.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType()=0;

//PACKAGE : MAP.
//FUNCTION : MSrez::GetName();
//DESCRIPTION : Function of redefining.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName() = 0;

//PACKAGE : MAP.
//FUNCTION : MSrez::Clone();
//DESCRIPTION : Function of redefining.
//INPUTS : NONE.
//RETURNS : MSrez pointer.
	virtual MSrez* Clone()=0;

//PACKAGE : MAP.
//FUNCTION : MSrez::Copy(MSrez* );
//DESCRIPTION : Function of redefining.
//INPUTS : MSrez pointer.
//RETURNS : NONE.
	virtual void Copy(MSrez* ){};

//PACKAGE : MAP.
//FUNCTION : MSrez::GetSize();
//DESCRIPTION : Defines size as nux-format.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return 4;};

//PACKAGE : MAP.
//FUNCTION : MSrez::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves layer to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MSrez::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads shear from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MSrez::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads shear from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MSrez::GetMaxDimension();
//DESCRIPTION : Gets min distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	virtual double GetMaxDimension(){return 0;};

//PACKAGE : MAP.
//FUNCTION : MSrez::ReadAny(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads.
//INPUTS : FILE pointer, int .
//RETURNS : MSrez pointer.
	static MSrez* ReadAny(FILE* h_nuxfile, int version);
	
//PACKAGE : MAP.
//FUNCTION : MSrez::ReadAny(MFile* file, int version);
//DESCRIPTION : Reads.
//INPUTS : MFile pointer, int .
//RETURNS : MSrez pointer.
	static MSrez* ReadAny(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MSrez::~MSrez();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MSrez(){};

//PACKAGE : MAP.
//FUNCTION : MSrez::IsPereopr();
//DESCRIPTION : Base shear - not redefinable.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return false;};

//PACKAGE : MAP.
//FUNCTION : MSrez::GetBaseNast();
//DESCRIPTION : As shear is not redefinable.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return NULL;};
};


//PACKAGE : MAP.
//CLASS : MShriftSrez : public MSrez.
//DESCRIPTION : Class MShriftSrez describes graphic primitive, which is symbol of some font.
class MShriftSrez : public MSrez

{
public:
	//  pointer to sigh description of dotted object
	MSymbol  m_tochka;

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::MShriftSrez();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MShriftSrez(){};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::MShriftSrez(MSymbol* sym);
//DESCRIPTION : Constructor  creates dotted font object by symbol 'sym'.
//INPUTS : MSymbol pointer.
//RETURNS : NONE.
	MShriftSrez(MSymbol* sym){m_tochka.Copy(sym);};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::MShriftSrez(MShriftSrez* srez);
//DESCRIPTION : Constructor  creates dotted object by defined template.
//INPUTS : MShriftSrez pointer.
//RETURNS : NONE.
	MShriftSrez(MShriftSrez* srez){Copy(srez);};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::~MShriftSrez();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MShriftSrez(){};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::GetType();
//DESCRIPTION : Returns identifier of dotted font object.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDT_SHRIFT;};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::GetName();
//DESCRIPTION : Returns identifier of dotted font object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDT_SHRIFT_NAME;};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::Clone();
//DESCRIPTION : Function of redefining.
//INPUTS : NONE.
//RETURNS : MSrez pointer.
	virtual MSrez* Clone(){return (MSrez*) new MShriftSrez(this);};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::Copy(MShriftSrez* srez);
//DESCRIPTION : Function of copying.
//INPUTS : MShriftSrez pointer.
//RETURNS : NONE.
	void Copy(MShriftSrez* srez){MSrez::Copy((MSrez*)srez);m_tochka.Copy(&srez->m_tochka);};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::GetSize();
//DESCRIPTION : Defines size as nux-format.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MSrez::GetSize()+m_tochka.GetBytes();};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::GetMaxDimension();
//DESCRIPTION : Gets min distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MSrez::GetMaxDimension(),fabs(m_tochka.value));};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves shear to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version){
		MSrez::WriteBin(h_nuxfile,version);
		return m_tochka.WriteSymbol(h_nuxfile, version);};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads shear from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version){
			MSrez::ReadBin(h_nuxfile,version);
			return m_tochka.ReadSymbol(h_nuxfile, version);};

//PACKAGE : MAP.
//FUNCTION : MShriftSrez:: ReadBin(MFile* file, int version);
//DESCRIPTION : Reads shear from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version){
			MSrez::ReadBin(file,version);
			return m_tochka.ReadSymbol(file, version);};

};


//PACKAGE : MAP.
//CLASS :  MProizvolSrez : public MSrez.
//DESCRIPTION : Class MProizvolSrez describes graphic primitive.
class MProizvolSrez : public MSrez

{
public:

	unsigned char	m_colorkod;  // table fill color code
	unsigned char	m_imagekod;	 // table code pattern fill
	MImage *m_image;	// cached link to the image from the dictionary
	unsigned int m_align;	// modus alignment 

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::MProizvolSrez();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
    MProizvolSrez() : m_colorkod(0), m_imagekod(0), m_image(0), m_align(0)  {};
	
//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::MProizvolSrez(MProizvolSrez* srez);
//DESCRIPTION : Constructor  creates dotted object by defined template.
//INPUTS : MProizvolSrez pointer.
//RETURNS : NONE.
    MProizvolSrez(MProizvolSrez* srez) : m_colorkod(0), m_imagekod(0), m_image(0), m_align(0)  {Copy(srez);};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::~MProizvolSrez();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MProizvolSrez(){};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::GetType();
//DESCRIPTION : Returns identifier of random dotted object.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDT_PROIZVOL;};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::GetName();
//DESCRIPTION : Returns identifier of random dotted object.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDT_PROIZVOL_NAME;};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::Clone();
//DESCRIPTION : Function of redefining.
//INPUTS : NONE.
//RETURNS : MSrez pointer.
	virtual MSrez* Clone(){return (MSrez*) new MProizvolSrez(this);};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::Copy(MProizvolSrez* srez);
//DESCRIPTION : function of copying.
//INPUTS : MProizvolSrez pointer.
//RETURNS : NONE.
	void Copy(MProizvolSrez* srez){
		MSrez::Copy((MSrez*)srez);
		m_align = srez->m_align;
		m_colorkod = srez->m_colorkod;
		m_image = srez->m_image;
		m_imagekod = srez->m_imagekod;
	};      

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::GetSize();
//DESCRIPTION : Defines size as nux-format.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MSrez::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::GetMaxDimension();
//DESCRIPTION : Defines size as nux-format.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return MSrez::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves shear to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		bool result = MSrez::WriteBin(h_nuxfile,version);
		// reading the image code , describing the pattern
		fwrite(&m_imagekod, sizeof(m_imagekod), 1, h_nuxfile);
		// write code fill color
		fwrite(&m_colorkod, sizeof(m_colorkod), 1, h_nuxfile);
		// write leveling
		fwrite(&m_align, sizeof(m_align), 1, h_nuxfile);
		return result;
	}; 

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads shear from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		bool result = MSrez::ReadBin(h_nuxfile,version);
		// reading the image code , describing the pattern
		fread(&m_imagekod, sizeof(m_imagekod), 1, h_nuxfile);
		// reading code fill color
		fread(&m_colorkod, sizeof(m_colorkod), 1, h_nuxfile);
		// reading leveling
		fread(&m_align, sizeof(m_align), 1, h_nuxfile);
		return result;
	};  

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads shear from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		bool result = MSrez::ReadBin(file,version);
		// reading the image code , describing the pattern
		file->GetData(&m_imagekod, sizeof(m_imagekod), 1);
		// reading fill color
		file->GetData(&m_colorkod, sizeof(m_colorkod), 1);
		// reading leveling
		file->GetData(&m_align, sizeof(m_align), 1);
		return result;
	};  

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::GetImage();
//DESCRIPTION : Gets image.
//INPUTS : NONE.
//RETURNS : MImage pointer.
	MImage *GetImage() { return m_image; };

//PACKAGE : MAP.
//FUNCTION : MProizvolSrez::SetImage(MImage *image);
//DESCRIPTION : Sets image.
//INPUTS : MImage pointer.
//RETURNS : NONE.
	void SetImage(MImage *image) { m_image = image; };
};

// constants of adjustable shear MNastrShriftSrez

#define IDT_NAS_SHRIFT_SIZE			3
#define IDT_NAS_SHRIFT_VAL_SYM_UGOL	0
#define IDT_NAS_SHRIFT_VAL_SYM_VAL	1
#define IDT_NAS_SHRIFT_SYM_OPISANIE	2

//PACKAGE : MAP.
//CLASS :  MNastrShriftSrez:public MShriftSrez, public MBaseNastr.
//DESCRIPTION : Class MNastrShriftSrez describes graphic primitive which is symbol of some font, with the availability of adjusting of: rotation angle of symbol and it's size.
class MNastrShriftSrez:public MShriftSrez, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::MNastrShriftSrez();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrShriftSrez():MBaseNastr(IDT_NAS_SHRIFT_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::MNastrShriftSrez(MSymbol* sym);
//DESCRIPTION : Constructor.
//INPUTS : MSymbol pointer.
//RETURNS : NONE.
	MNastrShriftSrez(MSymbol* sym):MShriftSrez(sym),MBaseNastr(IDT_NAS_SHRIFT_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::MNastrShriftSrez(MNastrShriftSrez* psz);
//DESCRIPTION : Constructor.
//INPUTS : MNastrShriftSrez pointer.
//RETURNS : NONE.
	MNastrShriftSrez(MNastrShriftSrez* psz):MShriftSrez((MShriftSrez*)psz),MBaseNastr((MBaseNastr*)psz){};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::GetType();
//DESCRIPTION : Gets shear type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDT_NAS_SHRIFT;};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::GetName();
//DESCRIPTION : Gets shear name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDT_NAS_SHRIFT_NAME;};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::IsPereopr();
//DESCRIPTION : MNastrShriftSrez - redefinable.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDT_NAS_SHRIFT_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::Copy(MNastrShriftSrez* psz);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrShriftSrez pointer.
//RETURNS : NONE.
	void Copy(MNastrShriftSrez* psz)
	{
		MShriftSrez::Copy((MShriftSrez*)psz);
		MBaseNastr::Copy((MBaseNastr*)psz);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MSrez pointer.
	virtual MSrez* Clone(){return (MSrez*) new MNastrShriftSrez(this);};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MShriftSrez::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MShriftSrez::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MShriftSrez::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MShriftSrez::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}	

//PACKAGE : MAP.
//FUNCTION : MNastrShriftSrez::GetPerName(int iIndex);
//DESCRIPTION : Gets for redefinable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDT_NAS_SHRIFT_VAL_SYM_UGOL:
			return "Угол поворота символа";
		case IDT_NAS_SHRIFT_VAL_SYM_VAL:
			return "Размер символа";
		case IDT_NAS_SHRIFT_SYM_OPISANIE:
			return "Символ";
		//	default:	exit(15);
		}
		return strEmpty;
	};

};

//PACKAGE : MAP.
//CLASS : MSzArray:public GlobalInheritantArray<MSrez>.
//DESCRIPTION : Class - array of pointers to 'MSrez'.
class MSzArray:public GlobalInheritantArray<MSrez>
{
public:

//PACKAGE : MAP.
//FUNCTION : MSzArray::CreateOfType();
//DESCRIPTION : Object creation - array of shears 'MSzArray'.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MSzArray();};

//PACKAGE : MAP.
//FUNCTION : MSzArray::WhatTheArray();
//DESCRIPTION : Identifies an array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_SRZ;};
public:

//PACKAGE : MAP.
//FUNCTION : MSzArray::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MSzArray::ReadBin(MFile* file,int version);
//DESCRIPTION : Reads from file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,int version);

//PACKAGE : MAP.
//FUNCTION : MSzArray::WriteBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Saves to file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MSzArray::SetSize(int Cnt);
//DESCRIPTION : Sets array size.
//INPUTS : int.
//RETURNS : NONE.
	void SetSize(int Cnt){
		MSrez* pEl = new MShriftSrez();
		ExpandSize(Cnt);
		for(int i=0;i<Cnt;i++)
			SetAt(i,pEl);
		delete pEl;
	}
};

//PACKAGE : MAP.
//FUNCTION : CreateSrezByType(int iType);
//DESCRIPTION : Creates shear by type.
//INPUTS : int.
//RETURNS : MSrez pointer.
MSrez* CreateSrezByType(int iType);

#endif //SREZI_H
