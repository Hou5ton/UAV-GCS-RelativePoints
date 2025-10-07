//PACKAGE : MAP.
//FILE: Plasti.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _PLASTI__H_
#define _PLASTI__H_
#include "BaseCl.h"
#include "ArrBase.h"
#include "Prosloiki.h"
#include "MString.h"
#include "MImage.h"

//===============================================================================
// CONSTANTS FOR TYPES OF AREA OBJECTS
// Names and identifiers of plasts
//===============================================================================
#define PLT_NESTANDART	 0 //  identifier of unforeseen filling of area object
#define PLT_SHTRIHOVKA   1 //  identifier of hatching type
#define PLT_ZALIVKA      2 //  identifier of filling type
#define PLT_UZOR         3 //  identifier of pattern type
#define PLT_PERCENT_UZOR 4 //  percentage filling
#define PLT_HANDLE	 9 //  single symbol per filling (inside contour)

#define PLT_NAS_SHTRIHOVKA   5 //  identifier of adjustable hatching type
#define PLT_NAS_ZALIVKA      6 //  identifier of adjustable filling type
#define PLT_NAS_UZOR         7 //  identifier of adjustable pattern type
#define PLT_NAS_PERCENT_UZOR 8 //  adjustable percentage filling
#define PLT_NAS_HANDLE	     10 //  adjustable single symbol per filling (inside contour)

#define PLT_PATTERN			 11 // fill pattern

#if ENGVERSION == 1 //English version
	//Constant names of simple seams
	#define PLT_SHTRIHOVKA_NAME			"Hatching"
	#define PLT_ZALIVKA_NAME			"Background Color"
	#define PLT_UZOR_NAME				"Pattern"
	#define PLT_PERCENT_UZOR_NAME		"Compound Pattern"
	#define PLT_HANDLE_NAME				"Symbolic Handle"
	// Constants of adjustable layers names
	#define PLT_NAS_SHTRIHOVKA_NAME		"Hatching (Adjustable)"
	#define PLT_NAS_ZALIVKA_NAME		"Background Color (Adjustable)"  
	#define PLT_NAS_UZOR_NAME			"Pattern (Adjustable)"
	#define PLT_NAS_PERCENT_UZOR_NAME	"Compound Pattern (Adjustable)"
	#define PLT_NAS_HANDLE_NAME			"Symbolic Handle (Adjustable)"
	#define PLT_PATTERN_NAME			"User Pattern"

#else //Russion version
	//Constant names of simple seams
	#define PLT_SHTRIHOVKA_NAME			"Штриховка"
	#define PLT_ZALIVKA_NAME			"Заливка"  
	#define PLT_UZOR_NAME				"Узор"
	#define PLT_PERCENT_UZOR_NAME		"Смешанный узор"
	#define PLT_HANDLE_NAME				"Символ-указатель"
	//Constants of adjustable layers names
	#define PLT_NAS_SHTRIHOVKA_NAME		"Штриховка (нас.)"
	#define PLT_NAS_ZALIVKA_NAME		"Заливка (нас.)"  
	#define PLT_NAS_UZOR_NAME			"Узор (нас.)"
	#define PLT_NAS_PERCENT_UZOR_NAME	"Смешанный узор (нас.)" 
	#define PLT_NAS_HANDLE_NAME			"Символ-указатель (нас.)"
	#define PLT_PATTERN_NAME			"Паттерн"
#endif

//PACKAGE : MAP.
//CLASS	: MPlast.
//DESCRIPTION : Class MPlast describes one of basic graphic fillings types, which can be used to draw any area object without border.
class MPlast
{
public:

//PACKAGE : MAP.
//FUNCTION : MPlast::MPlast();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MPlast(){};

//PACKAGE : MAP.
//FUNCTION : MPlast::~MPlast();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MPlast(){};

//PACKAGE : MAP.
//FUNCTION : MPlast::GetBytes();
//DESCRIPTION : Gets syze in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int  GetBytes(){return GetSize();};

//PACKAGE : MAP.
//FUNCTION : MPlast::GetType();
//DESCRIPTION : Functions for definition
//  			Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType()=0;
	
//PACKAGE : MAP.
//FUNCTION : MPlast::GetName();
//DESCRIPTION :  Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName() = 0;

//PACKAGE : MAP.
//FUNCTION : MPlast::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone()=0;

//PACKAGE : MAP.
//FUNCTION : MPlast::Copy(MPlast* );
//DESCRIPTION : Makes a copy (MPlast* plast).
//INPUTS : MPlast pointer.
//RETURNS : NONE.
	virtual void Copy(MPlast* ){};

//PACKAGE : MAP.
//FUNCTION : MPlast::GetSize();
//DESCRIPTION :  Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return 0;};

//PACKAGE : MAP.
//FUNCTION : MPlast::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 
	
//PACKAGE : MAP.
//FUNCTION : MPlast::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 
	
//PACKAGE : MAP.
//FUNCTION : MPlast::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MPlast::GetMaxDimension();
//DESCRIPTION : Gets max distance between ends point of drawing.
//INPUTS : NONE.
//RETURNS : double.
	virtual double GetMaxDimension(){return 0;};

//PACKAGE : MAP.
//FUNCTION : MPlast::ReadAny(FILE* h_nuxfile, int version);
//DESCRIPTION :  Reads layer of any type from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : MPlast pointer.
	static MPlast* ReadAny(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MPlast::ReadAny(MFile* file, int version);
//DESCRIPTION :  Reads layer of any type from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : MPlast pointer.
	static MPlast* ReadAny(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : MPlast::IsPereopr();
//DESCRIPTION : Base layer - non redefinable.
//INPUTS : NONE.
//RETURNS : False.
	virtual bool IsPereopr(){return false;};

//PACKAGE : MAP.
//FUNCTION : MPlast::GetBaseNast();
//DESCRIPTION : AS Base layer is non redefinable
//  			it does not have adjustments.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return NULL;};
};

//PACKAGE : MAP.
//CLASS	: MPattern:public MPlast.
//DESCRIPTION : Class MPattern describes filling area object to some pattern.
class MPattern : public MPlast
{
public:
	unsigned char	m_colorkod;  // < table fill color code
	unsigned char	m_imagekod;	 // < table code pattern fill
	MImage *m_image;			 // < cached image

//PACKAGE : MAP.
//FUNCTION : MPattern::MPattern(unsigned char colorkod = 0, unsigned char imagekod = 0);
//DESCRIPTION : Constructor.
//INPUTS : unsigned char , unsigned char .
//RETURNS : NONE.
	MPattern(unsigned char colorkod = 0, unsigned char imagekod = 0);

//PACKAGE : MAP.
//FUNCTION : MPattern::MPattern(MPattern* pPat);
//DESCRIPTION : Constructor for a given object.
//INPUTS : MPattern pointer.
//RETURNS : NONE.
	MPattern(MPattern* pPat);
	
//PACKAGE : MAP.
//FUNCTION : MPattern::~MPattern();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MPattern(){ if (m_image) delete m_image; };

//PACKAGE : MAP.
//FUNCTION : MPattern::Copy(MPattern* pPat);
//DESCRIPTION : Copy function.
//INPUTS : MPattern .
//RETURNS : NONE.
	void Copy(MPattern* pPat);

//PACKAGE : MAP.
//FUNCTION : MPattern::Clone();
//DESCRIPTION : Cloning.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MPattern(this);};
	
//PACKAGE : MAP.
//FUNCTION : MPattern::GetMaxDimension();
//DESCRIPTION :  Gets maximal distance between the extreme points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MPlast::GetMaxDimension(), double(0));};
	
//PACKAGE : MAP.
//FUNCTION : MPattern::GetSize();
//DESCRIPTION : Options to override.
// 				Specifies the size in bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MPlast::GetSize()+sizeof(m_colorkod)+sizeof(m_imagekod);};

//PACKAGE : MAP.
//FUNCTION : MPattern::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Write to a file format UTP.
//INPUTS : FILE pointer, int .
//RETURNS : NONE.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MPattern::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from a file format UTP.
//INPUTS : FILE pointer, int .
//RETURNS : NONE.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MPattern::WriteBin(MFile* file, int version);
//DESCRIPTION : Read from a file format UTP.
//INPUTS : MFile pointer, int .
//RETURNS : NONE.
	virtual bool ReadBin(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MPattern::GetType;
//DESCRIPTION : Getting the type of formation.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_PATTERN;};

//PACKAGE : MAP.
//FUNCTION : MPattern::GetName;
//DESCRIPTION : Getting the name of formation.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_PATTERN_NAME;};

//PACKAGE : MAP.
//FUNCTION : MPattern::GetImage();
//DESCRIPTION : Getting image.
//INPUTS : NONE.
//RETURNS : MImage pointer.	
	MImage *GetImage() { return m_image; };

//PACKAGE : MAP.
//FUNCTION : MPattern::SetImage(MImage *image);
//DESCRIPTION : Setting image.
//INPUTS : MImage pointer.
//RETURNS : NONE.
	void SetImage(MImage *image) { m_image = image; };
};

//PACKAGE : MAP.
//CLASS	: MZalivka:public MPlast.
//DESCRIPTION : Class MZalivka describes area object filling.
class MZalivka : public MPlast
{
public:
	unsigned char	m_colorkod;  // < Table code of filling color

//PACKAGE : MAP.
//FUNCTION : MZalivka::MZalivka();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MZalivka():m_colorkod(1){};

//PACKAGE : MAP.
//FUNCTION : MZalivka::MZalivka(unsigned char colorkod);
//DESCRIPTION : Constructor with defined code of color.
//INPUTS : unsigned char.
//RETURNS : NONE.
	MZalivka(unsigned char colorkod):m_colorkod(colorkod){};

//PACKAGE : MAP.
//FUNCTION : MZalivka::MZalivka(MZalivka* pZal);
//DESCRIPTION : Constructor by defined object.
//INPUTS : MZalivka pointer.
//RETURNS : NONE.
	MZalivka(MZalivka* pZal){Copy(pZal);};

//PACKAGE : MAP.
//FUNCTION : MZalivka::~MZalivka();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MZalivka(){};

//PACKAGE : MAP.
//FUNCTION : MZalivka::Copy(MZalivka* zalivka);
//DESCRIPTION : Function of copy.
//INPUTS : MZalivka pointer.
//RETURNS : NONE.
	void Copy(MZalivka* zalivka);

//PACKAGE : MAP.
//FUNCTION : MZalivka::Clone();
//DESCRIPTION : Cloning.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MZalivka(this);};

//PACKAGE : MAP.
//FUNCTION : MZalivka::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MPlast::GetMaxDimension(),double(0));};

//PACKAGE : MAP.
//FUNCTION : MZalivka::GetSize();
//DESCRIPTION : Functions for defining
// 				Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MPlast::GetSize()+sizeof(m_colorkod);};

//PACKAGE : MAP.
//FUNCTION : MZalivka::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MZalivka::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MZalivka::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);  

//PACKAGE : MAP.
//FUNCTION : MZalivka::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_ZALIVKA;};

//PACKAGE : MAP.
//FUNCTION : MZalivka::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_ZALIVKA_NAME;};
	
};

//===============================================================================
// CONSTANTS FOR TYPES OF GRID
//===============================================================================

#define STK_PRAM 0 //  identifier of direct grid
#define STK_KOS  1 //  identifier of skew grid
#define STK_XAOS 2 //  identifier of chaotic grid

//PACKAGE : MAP.
//CLASS	: MUzor : public MPlast.
//DESCRIPTION : Class MUzor describes filling of area object by symbol of defined grid.
class MUzor : public MPlast
{
public:
	// variables
	unsigned char	m_type;   // < grid type
	float			m_shag;   // < grid step
	float			m_ugol;   // < angle of grid slope between horizontal axis
	MSymbol			m_symbol; // < symbol

//PACKAGE : MAP.
//FUNCTION : MUzor::MUzor();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MUzor():m_type(STK_PRAM),m_shag(1),m_ugol(0){};

//PACKAGE : MAP.
//FUNCTION : MUzor::MUzor(int tip, float shg, float angle, MSymbol* sb);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : int , float , float , MSymbol pointer.
//RETURNS : NONE.
	MUzor(int tip, float shg, float angle, MSymbol* sb):m_type(tip),m_shag(shg),m_ugol(angle){m_symbol.Copy(sb);};

//PACKAGE : MAP.
//FUNCTION : MUzor::MUzor(MUzor* pUz);
//DESCRIPTION : Constructor by defined object.
//INPUTS : MUzor pointer.
//RETURNS : NONE.
	MUzor(MUzor* pUz){Copy(pUz);};

//PACKAGE : MAP.
//FUNCTION : MUzor::~MUzor();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MUzor(){};

//PACKAGE : MAP.
//FUNCTION : MUzor::Copy(MUzor* uzor);
//DESCRIPTION : Function of coying.
//INPUTS : MUzor pointer.
//RETURNS : NONE.
	void Copy(MUzor* uzor);

//PACKAGE : MAP.
//FUNCTION : MUzor::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MUzor(this);};

//PACKAGE : MAP.
//FUNCTION : MUzor::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MPlast::GetMaxDimension(),Max(fabs(m_shag*3),fabs(m_symbol.value)));};

//PACKAGE : MAP.
//FUNCTION : MUzor::GetSize();
//DESCRIPTION : Functions for redifining
//  			Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MPlast::GetSize()+sizeof(m_type)+sizeof(m_shag)+sizeof(m_ugol)+m_symbol.GetBytes();};

//PACKAGE : MAP.
//FUNCTION : MUzor::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION :  Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : double.
	virtual bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MUzor::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION :  Reads from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : double.
	virtual bool ReadBin(FILE* h_nuxfile, int version);  

//PACKAGE : MAP.
//FUNCTION : MUzor::ReadBin(MFile* file, int version);
//DESCRIPTION :  Reads from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : double.
	virtual bool ReadBin(MFile* file, int version);  

//PACKAGE : MAP.
//FUNCTION : MUzor::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_UZOR;};

//PACKAGE : MAP.
//FUNCTION : MUzor::GetName();
//DESCRIPTION :  Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_UZOR_NAME;};
};

//===============================================================================
// CONSTANTS FOR HATCHING TYPE
//===============================================================================
#define SHR_ODINARNAIA 0 //  identifier of single hatching
#define SHR_DVOINAIA   1 //  identifier of double hatching

//PACKAGE : MAP.
//CLASS	: MShtrihovka : public MPlast.
//DESCRIPTION : Class MShtrihovka describes hatching of area object.
class MShtrihovka : public MPlast
{
public:
	// variables
	int				m_type;      // < hatching type
	float			m_ugol;      // < hatching angle
	float			m_shag;      // < hatching step
	float			m_thickness; // < hatch thickness
	unsigned char	m_colorkod;  // < Table code of hatching color

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::MShtrihovka();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MShtrihovka():m_type(SHR_ODINARNAIA),m_ugol(0),m_shag(1),m_thickness(0.1f),m_colorkod(0){};

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::MShtrihovka(int tip, float angle, float shg, float tolschina, unsigned char colorkod);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : int , float , float , float , unsigned char .
//RETURNS : NONE.
	MShtrihovka(int tip, float angle, float shg, float tolschina, unsigned char colorkod):m_type(tip),
			m_ugol(angle),m_shag(shg),m_thickness(tolschina),m_colorkod(colorkod){};

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::MShtrihovka(MShtrihovka* pSh);
//DESCRIPTION : Constructor by defined object.
//INPUTS : MShtrihovka pointer.
//RETURNS : NONE.
	MShtrihovka(MShtrihovka* pSh){Copy(pSh);};

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::~MShtrihovka();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MShtrihovka(){};

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::Copy(MShtrihovka* shtrihovka);
//DESCRIPTION : Function of copying.
//INPUTS : MShtrihovka pointer.
//RETURNS : NONE.
	void Copy(MShtrihovka* shtrihovka);

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MShtrihovka(this);};

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::GetMaxDimension();
//DESCRIPTION : Gets max size between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MPlast::GetMaxDimension(),Max(fabs(m_shag*3),fabs(m_thickness)));};

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::GetSize();
//DESCRIPTION : Function for redefining
// 				Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MPlast::GetSize()+sizeof(m_type)+sizeof(m_shag)+
			sizeof(m_thickness)+sizeof(m_ugol)+sizeof(m_colorkod);};	
//PACKAGE : MAP.
//FUNCTION : MShtrihovka:: WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MShtrihovka:: ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);  

//PACKAGE : MAP.
//FUNCTION : MShtrihovka:: ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MShtrihovka::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.	
	virtual unsigned char GetType(){return PLT_SHTRIHOVKA;};
	
//PACKAGE : MAP.
//FUNCTION : MShtrihovka::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_SHTRIHOVKA_NAME;};
};

//PACKAGE : MAP.
//CLASS	: MPercentUzor : public MUzor.
//DESCRIPTION : Class MPercentUzor describes filling of area object by couple symbols (from 1 to 3x) by defined grid, with the ability of setting % of using any of symbols
class MPercentUzor : public MUzor
{
public:
	// variables
	MSymbol m_SecSym;	// < second symbol of pattern
	MSymbol m_ThirdSym;	// < third symbol of pattern

	unsigned char m_iPercent1; // < % of pattern filling by 1st symbol
	unsigned char m_iPercent2; // < % of pattern filling by 2nd symbol
	unsigned char m_iPercent3; // < % % of pattern filling by 3rd symbol


//PACKAGE : MAP.
//FUNCTION : MPercentUzor::MPercentUzor();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MPercentUzor():m_iPercent1(30),m_iPercent2(20),m_iPercent3(10){};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::MPercentUzor(int tip, float shg, float angle, MSymbol* sb);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : int , float , float , MSymbol pointer.
//RETURNS : NONE.
	MPercentUzor(int tip, float shg, float angle, MSymbol* sb):MUzor(tip,shg,angle,sb){
		m_SecSym.Copy(sb);
		m_ThirdSym.Copy(sb);
	};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::MPercentUzor(MPercentUzor* pUz);
//DESCRIPTION : Constructor by defined object.
//INPUTS : MPercentUzor pointer.
//RETURNS : NONE.
	MPercentUzor(MPercentUzor* pUz){Copy(pUz);};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::~MPercentUzor();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MPercentUzor(){};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::Copy(MPercentUzor* uzor);
//DESCRIPTION : Function of copying.
//INPUTS : MPercentUzor pointer.
//RETURNS : NONE.
	void Copy(MPercentUzor* uzor);

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MPercentUzor(this);};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MUzor::GetMaxDimension(),Max(Max(fabs(m_SecSym.value),fabs(m_SecSym.value)),Max(fabs(m_shag*3),fabs(m_symbol.value))));};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::GetSize();
//DESCRIPTION : Function for redefining
//  			Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize(){return MUzor::GetSize()+sizeof(m_iPercent1)+sizeof(m_iPercent2)+sizeof(m_iPercent3)+
		m_SecSym.GetBytes()+m_ThirdSym.GetBytes();};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);  
	
//PACKAGE : MAP.
//FUNCTION : MPercentUzor::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_PERCENT_UZOR;};

//PACKAGE : MAP.
//FUNCTION : MPercentUzor::GetName();
//DESCRIPTION :  Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_PERCENT_UZOR_NAME;};
};

//PACKAGE : MAP.
//CLASS	: MHandleArea : public MPlast.
//DESCRIPTION : Class MHandleArea describes filling of area where single symbol is placed inside object area
class MHandleArea : public MPlast
{
public:
	
	MSymbol m_symbol;// Symbol
//	float partX;// placement point symbol = offset in width from direct coverage
//	float partY;//placement point symbol = offset in length from direct coverage

//	MHandleArea():partX(-1),partY(-1){};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::MHandleArea();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MHandleArea(){};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::MHandleArea(MSymbol* simvol);
//DESCRIPTION : Constructor.
//INPUTS : MSymbol pointer.
//RETURNS : NONE.
	MHandleArea(MSymbol* simvol){m_symbol.Copy(simvol);};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::MHandleArea(MHandleArea* pHandle);
//DESCRIPTION : Constructor.
//INPUTS : MHandleArea pointer.
//RETURNS : NONE.
	MHandleArea(MHandleArea* pHandle) { Copy(pHandle);};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::~MHandleArea();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MHandleArea(){};
	
//PACKAGE : MAP.
//FUNCTION : MHandleArea::Copy(MHandleArea* pHandle);
//DESCRIPTION : Copy function.
//INPUTS : MHandleArea pointer.
//RETURNS : NONE.
	void Copy(MHandleArea* pHandle)
	{
		MPlast::Copy((MPlast*)pHandle);
		if(pHandle) m_symbol.Copy(&pHandle->m_symbol);
	};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MHandleArea(this);};
	
//PACKAGE : MAP.
//FUNCTION : MHandleArea::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.	
	double GetMaxDimension(){return Max(MPlast::GetMaxDimension(),fabs(m_symbol.value));};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::GetSize();
//DESCRIPTION : Gets size in bytes.
//INPUTS : NONE.
//RETURNS : int.	
	virtual int  GetSize(){return MPlast::GetSize()+m_symbol.GetBytes();};	

//PACKAGE : MAP.
//FUNCTION : MHandleArea::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.	
	virtual bool WriteBin(FILE* h_nuxfile, int version) 
	{
		MPlast::WriteBin(h_nuxfile, version);
		m_symbol.WriteSymbol(h_nuxfile, version);
		return true;		
	};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version) 
	{
		MPlast::ReadBin(h_nuxfile, version);
		m_symbol.ReadSymbol(h_nuxfile, version);
		return true;		
	};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MPlast::ReadBin(file, version);
		m_symbol.ReadSymbol(file, version);
		return true;
		
	};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::GetType();
//DESCRIPTION : Gets type of layer.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_HANDLE;};

//PACKAGE : MAP.
//FUNCTION : MHandleArea::GetName();
//DESCRIPTION :  Gets name of layer.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_HANDLE_NAME;};
};


//===========================================================================
// Constants for class of adjustable pattern MNastrUzor
#define IDL_NAS_UZOR_SIZE 4			// quantity of adjustable parameters

#define IDL_NAS_UZOR_VAL_STEP 0		// step of pattern symbol placement
#define IDL_NAS_UZOR_VAL_UGOL 1		// angle
#define IDL_NAS_UZOR_VAL_SYM_UGOL 2	// angle of symbol rotation
#define IDL_NAS_UZOR_VAL_SYM_VAL  3	// symbol size


//PACKAGE : MAP.
//CLASS	: MNastrUzor:public MUzor, public MBaseNastr.
//DESCRIPTION : Class MNastrUzor describes filling of areal object
//				by symbol by defined grid with the ability of adjustment:
//				step of pattern symbols placement, angle, angle of symbol rotation, symbol size.
class MNastrUzor:public MUzor, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::MNastrUzor();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrUzor():MBaseNastr(IDL_NAS_UZOR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::MNastrUzor(int typ, float shg,float angle,MSymbol* sb);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : int , float ,float ,MSymbol pointer.
//RETURNS : NONE.
	MNastrUzor(int typ, float shg,float angle,MSymbol* sb)
		:MUzor(typ,shg,angle,sb),MBaseNastr(IDL_NAS_UZOR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::MNastrUzor(MNastrUzor* pl);
//DESCRIPTION : Constructor by defined object.
//INPUTS : MNastrUzor pointer.
//RETURNS : NONE.
	MNastrUzor(MNastrUzor* pl):MUzor((MUzor*)pl),MBaseNastr((MBaseNastr*)pl){};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::~MNastrUzor();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrUzor(){};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::IsPereopr();
//DESCRIPTION :  If adjustable? - YES.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::GetBaseNast();
//DESCRIPTION :  Casts to MBaseNast. As layer can be redefined.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefining parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_UZOR_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::Copy(MNastrUzor* pl);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrUzor pointer.
//RETURNS : NONE.
	void Copy(MNastrUzor* pl)
	{
		MUzor::Copy((MUzor*)pl);
		MBaseNastr::Copy((MBaseNastr*)pl);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MNastrUzor(this);};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::GetSize();
//DESCRIPTION : Functions for redefining
//  			Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MUzor::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MUzor::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,int version)
	{
		MUzor::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrUzor::ReadBin(MFile* file,int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,int version)
	{
		MUzor::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::GetPerName(int iIndex);
//DESCRIPTION : Gets redefining parameter name.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_UZOR_VAL_STEP:
			return "Шаг";
		case IDL_NAS_UZOR_VAL_UGOL:
			return "Угол";
		case IDL_NAS_UZOR_VAL_SYM_UGOL:
			return "Угол поворота символа";
		case IDL_NAS_UZOR_VAL_SYM_VAL:
			return "Размер симола";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_NAS_UZOR;};

//PACKAGE : MAP.
//FUNCTION : MNastrUzor::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_NAS_UZOR_NAME;};
};


//===========================================================================
// Constants for class of adjustable hatching MNastrShtrihovka
#define IDL_NAS_SHTRIHOVKA_SIZE 3		// quantity of adjustable parameters

#define IDL_NAS_SHTRIHOVKA_VAL_TOL  0	// hatching thickness
#define IDL_NAS_SHTRIHOVKA_VAL_STEP 1	// step
#define IDL_NAS_SHTRIHOVKA_VAL_UGOL 2	// angle


//PACKAGE : MAP.
//CLASS	: MNastrShtrihovka:public MShtrihovka, public MBaseNastr.
////DESCRIPTION	:
//DESCRIPTION : Class MNastrShtrihovka describes hatching of area object with the ability of adjusting: 
//		thickness, step, amgle of hatching
class MNastrShtrihovka:public MShtrihovka, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::MNastrShtrihovka();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrShtrihovka():MBaseNastr(IDL_NAS_SHTRIHOVKA_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::MNastrShtrihovka(int typ, float angle,float shg,float tolschina,unsigned char colorkod);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : int , float ,float ,float ,unsigned char .
//RETURNS : NONE.
	MNastrShtrihovka(int typ, float angle,float shg,float tolschina,unsigned char colorkod)
		:MShtrihovka(typ,angle,shg,tolschina,colorkod),MBaseNastr(IDL_NAS_SHTRIHOVKA_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::MNastrShtrihovka(MNastrShtrihovka* pl);
//DESCRIPTION : Constructor by defined object.
//INPUTS : MNastrShtrihovka pointer.
//RETURNS : NONE.
	MNastrShtrihovka(MNastrShtrihovka* pl):MShtrihovka((MShtrihovka*)pl),MBaseNastr((MBaseNastr*)pl){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::~MNastrShtrihovka();
//DESCRIPTION : Destructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrShtrihovka(){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::IsPereopr();
//DESCRIPTION : If adjustable? - YES.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::GetBaseNast();
//DESCRIPTION : Casts to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefining parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_SHTRIHOVKA_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::Copy(MNastrShtrihovka* pl);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrShtrihovka pointer.
//RETURNS : NONE.
	void Copy(MNastrShtrihovka* pl)
	{
		MShtrihovka::Copy((MShtrihovka*)pl);
		MBaseNastr::Copy((MBaseNastr*)pl);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MNastrShtrihovka(this);};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::GetSize();
//DESCRIPTION : Functions for redefining
//  			Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MShtrihovka::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MShtrihovka::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MShtrihovka::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MShtrihovka::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::GetPerName(int iIndex);
//DESCRIPTION : Gets redefining parameter name.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_SHTRIHOVKA_VAL_TOL:
			return "Толщина";
		case IDL_NAS_SHTRIHOVKA_VAL_STEP:
			return "Шаг";
		case IDL_NAS_SHTRIHOVKA_VAL_UGOL:
			return "Угол";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_NAS_SHTRIHOVKA;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrShtrihovka::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_NAS_SHTRIHOVKA_NAME;};
};

//===========================================================================
// Constants for class of adjustable percentage pattern MNastrPercentUzor
#define IDL_NAS_PERCENT_UZOR_SIZE 11           // quantity of adjustable parameters

#define IDL_NAS_PERCENT_UZOR_VAL_PERCENT_1  0  // % of pattern filling by 1st symbol
#define IDL_NAS_PERCENT_UZOR_VAL_PERCENT_2  1  // % of pattern filling by 2nd symbol
#define IDL_NAS_PERCENT_UZOR_VAL_PERCENT_3  2  // % % of pattern filling by 3rd symbol
#define IDL_NAS_PERCENT_UZOR_VAL_STEP		3  // step of pattern symbol placement
#define IDL_NAS_PERCENT_UZOR_VAL_UGOL		4  // angle
#define IDL_NAS_PERCENT_UZOR_VAL_1_SYM_UGOL 5  // angle of symbol rotation 1
#define IDL_NAS_PERCENT_UZOR_VAL_2_SYM_UGOL 6  // angle of symbol rotation 2
#define IDL_NAS_PERCENT_UZOR_VAL_3_SYM_UGOL	7  // angle of symbol rotation 3
#define IDL_NAS_PERCENT_UZOR_VAL_1_SYM_VAL	8  // symbol size 1
#define IDL_NAS_PERCENT_UZOR_VAL_2_SYM_VAL	9  // symbol size 2
#define IDL_NAS_PERCENT_UZOR_VAL_3_SYM_VAL	10 // symbol size 3


//PACKAGE : MAP.
//CLASS	: MNastrPercentUzor:public MPercentUzor, public MBaseNastr.
//DESCRIPTION : Class MPercentUzor describes filling of area object
//				by couple symbols(from 1 to 3) by defined grid,
//				with the ability of adjustment:
//				% of usinf each of symbols, step of symbols placement,
//				angle, rotation angle of each symbol, symbols size.
class MNastrPercentUzor:public MPercentUzor, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::MNastrPercentUzor();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrPercentUzor():MBaseNastr(IDL_NAS_PERCENT_UZOR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::MNastrPercentUzor(int typ, float shg,float angle,MSymbol* sm);
//DESCRIPTION : Constructor with defined parameters.
//INPUTS : int , float ,float ,MSymbol pointer.
//RETURNS : NONE.
	MNastrPercentUzor(int typ, float shg,float angle,MSymbol* sm)
		:MPercentUzor(typ,shg,angle,sm),MBaseNastr(IDL_NAS_PERCENT_UZOR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::MNastrPercentUzor(MNastrPercentUzor* pl);
//DESCRIPTION : Constructor by default.
//INPUTS : MNastrPercentUzor pointer.
//RETURNS : NONE.
	MNastrPercentUzor(MNastrPercentUzor* pl):MPercentUzor((MPercentUzor*)pl),MBaseNastr((MBaseNastr*)pl){};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::~MNastrPercentUzor();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrPercentUzor(){};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::IsPereopr();
//DESCRIPTION : If adjustable? - YES.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::GetBaseNast();
//DESCRIPTION : Casts to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefining parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_PERCENT_UZOR_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::Copy(MNastrPercentUzor* pl);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrPercentUzor pointer.
//RETURNS : NONE.
	void Copy(MNastrPercentUzor* pl)
	{
		MPercentUzor::Copy((MPercentUzor*)pl);
		MBaseNastr::Copy((MBaseNastr*)pl);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MNastrPercentUzor(this);};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::GetSize();
//DESCRIPTION : Functions for redefining
//  			Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MPercentUzor::GetSize()+MBaseNastr::GetSize();};


//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.	
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MPercentUzor::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MPercentUzor::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MPercentUzor::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::GetPerName(int iIndex);
//DESCRIPTION : Gets redefining parameter name.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PERCENT_UZOR_VAL_PERCENT_1:
			return "Процентное содержание превого символа";
		case IDL_NAS_PERCENT_UZOR_VAL_PERCENT_2:
			return "Процентное содержание второго символа";
		case IDL_NAS_PERCENT_UZOR_VAL_PERCENT_3:
			return "Процентное содержание третъего символа";
		case IDL_NAS_PERCENT_UZOR_VAL_STEP:
			return "Шаг";
		case IDL_NAS_PERCENT_UZOR_VAL_UGOL:
			return "Угол";
		case IDL_NAS_PERCENT_UZOR_VAL_1_SYM_UGOL:
			return "Угол повората 1-го символа";
		case IDL_NAS_PERCENT_UZOR_VAL_2_SYM_UGOL:
			return "Угол повората 2-го символа";
		case IDL_NAS_PERCENT_UZOR_VAL_3_SYM_UGOL:
			return "Угол повората 3-го символа";
		case IDL_NAS_PERCENT_UZOR_VAL_1_SYM_VAL:
			return "Величина 1-го символа";
		case IDL_NAS_PERCENT_UZOR_VAL_2_SYM_VAL:
			return "Величина 2-го символа";
		case IDL_NAS_PERCENT_UZOR_VAL_3_SYM_VAL:
			return "Величина 3-го символа";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_NAS_PERCENT_UZOR;};

//PACKAGE : MAP.
//FUNCTION : MNastrPercentUzor::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_NAS_PERCENT_UZOR_NAME;};
};

#define IDL_NAS_PLAST_HANDLE_SIZE	3
#define IDL_NAS_PLAST_HANDLE_VAL 0
#define IDL_NAS_PLAST_HANDLE_SYMBOL 1
#define IDL_NAS_PLAST_HANDLE_UGOL 2

//PACKAGE : MAP.
//CLASS	: MNastrHandleArea : public MHandleArea, public MBaseNastr.
//DESCRIPTION : Class MNastrHandleArea describes filling of area where adjustable single symbol is placed inside object area.
class MNastrHandleArea : public MHandleArea, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::MNastrHandleArea();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrHandleArea():MHandleArea(),MBaseNastr(IDL_NAS_PLAST_HANDLE_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::MNastrHandleArea(MSymbol* simvol);
//DESCRIPTION : Constructor.
//INPUTS : MSymbol pointer.
//RETURNS : NONE.
	MNastrHandleArea(MSymbol* simvol)
		:MHandleArea(simvol),MBaseNastr(IDL_NAS_PLAST_HANDLE_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::MNastrHandleArea(MNastrHandleArea* pHandle);
//DESCRIPTION : Constructor.
//INPUTS : MNastrHandleArea pointer.
//RETURNS : NONE.
	MNastrHandleArea(MNastrHandleArea* pHandle)
		:MHandleArea((MHandleArea*)pHandle),MBaseNastr((MBaseNastr*)pHandle){};

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::~MNastrHandleArea();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.	
	~MNastrHandleArea(){};
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::IsPereopr();
//DESCRIPTION : If adjustable? - YES.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::GetBaseNast();
//DESCRIPTION : Casts to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefining parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_PLAST_HANDLE_SIZE;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::Copy(MNastrHandleArea* pHandle);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrHandleArea pointer.
//RETURNS : NONE.
	void Copy(MNastrHandleArea* pHandle)
	{
		MHandleArea::Copy((MHandleArea*)pHandle);
		MBaseNastr::Copy((MBaseNastr*)pHandle);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MPlast pointer.
	virtual MPlast* Clone(){return (MPlast*) new MNastrHandleArea(this);};
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::GetSize();
//DESCRIPTION : Functions for redefining
//  			Defines size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MHandleArea::GetSize()+MBaseNastr::GetSize();};
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MHandleArea::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MHandleArea::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MHandleArea::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::GetPerName(int iIndex);
//DESCRIPTION : Gets redefining parameter name.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PLAST_HANDLE_VAL:
			return "Величина символа";
			//		default:	exit(15);
		case IDL_NAS_PLAST_HANDLE_SYMBOL:
			return "Символ";
		case IDL_NAS_PLAST_HANDLE_UGOL:
			return "Угол поворота";

		}
		return strEmpty;
	};
	
//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return PLT_NAS_HANDLE;};

//PACKAGE : MAP.
//FUNCTION : MNastrHandleArea::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return PLT_NAS_HANDLE_NAME;};
};

//PACKAGE : MAP.
//CLASS	: MPlArray:public GlobalInheritantArray<MPlast>.
//DESCRIPTION : Class - array of pointers to MPlast.
class MPlArray:public GlobalInheritantArray<MPlast>
{
public:

//PACKAGE : MAP.
//FUNCTION : MPlArray::CreateOfType();
//DESCRIPTION : Creates array - object of MPlArray class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MPlArray();};

//PACKAGE : MAP.
//FUNCTION : MPlArray::WhatTheArray();
//DESCRIPTION : Returns identifier of layers 'MPlArray' array.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_PLA;};

//PACKAGE : MAP.
//FUNCTION : MPlArray::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MPlArray::ReadBin(MFile* file,int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,int version);

//PACKAGE : MAP.
//FUNCTION : MPlArray::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MPlArray::SetSize(int Cnt);
//DESCRIPTION : Sets layers array size.
//INPUTS : int.
//RETURNS : NONE.
	void SetSize(int Cnt){
		MPlast* pEl = new MZalivka();
		ExpandSize(Cnt);
		for(int i=0;i<Cnt;i++)
			SetAt(i,pEl);
		delete pEl;
	}
};

//PACKAGE : MAP.
//FUNCTION : MPlArray::CreatePlastByType(int iType);
//DESCRIPTION : Creates layer by type.
//INPUTS : int.
//RETURNS : MPlast pointer.
MPlast* CreatePlastByType(int iType);

#endif //_PLASTI__H_
