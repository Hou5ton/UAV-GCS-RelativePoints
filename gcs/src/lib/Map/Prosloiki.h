//PACKAGE : MAP.
//FILE: Prosloiki.h   
//AUTHOR:
//DESCRIPTION:

#ifndef PROSLOIKI_H
#define PROSLOIKI_H
#include "BaseCl.h"
#include "ArrBase.h"
#include "MString.h"


// CONSTANTS FOR LINE TYPES
#define IDL_LINIA	  0	//  identifier of continuous line
#define IDL_SHTRIH	  1	//  identifier of hatched line
#define IDL_PUNKTIR	  2	//  identifier of dotted line
#define IDL_SBSTRETCH 3	//  identifier of line from outstretched symbol
#define IDL_SOFRANGE  4 //  identifier of line from bounadry symbol
#define IDL_ALGORITM  5 //  identifier of algorithmic line
#define IDL_SPLINE    6 //  identifier of spline
#define IDL_ORIENTED  7 //  oriented symbol outstretching
#define IDL_SINGULAR  16 //  single symbol outstretching

// Adjustable types of lines
#define IDL_NAS_LINIA		8  // adjustable continuous line
#define IDL_NAS_SHTRIH		9  // adjustable hatched line
#define IDL_NAS_PUNKTIR		10 // adjustable dotted line
#define IDL_NAS_SBSTRETCH	11 // adjustable line from outstretched symbol
#define IDL_NAS_SOFRANGE	12 // adjustable line from bounadry symbol
#define IDL_NAS_ALGORITMA	13 // adjustable algorithmic line
#define IDL_NAS_SPLINE		14 // adjustable spline
#define IDL_NAS_ORIENTED	15 // adjustable symbol outstretching
#define IDL_NAS_SINGULAR	17 // adjustable single symbol outstretching

#if ENGVERSION == 1 // English version
	// Constants of nsme of regular sublayers
	#define	IDL_LINIA_NAME			"Continuous"
	#define IDL_SHTRIH_NAME			"Hatched"
	#define IDL_PUNKTIR_NAME		"Dotted"
	#define IDL_SBSTRETCH_NAME		"Symbolic"
	#define IDL_SOFRANGE_NAME		"Boundary Symbol"
	#define IDL_ALGORITM_NAME		"Algorithmic"
	#define IDL_SPLINE_NAME			"Spline"
	#define IDL_ORIENTED_NAME		"Node Symbol"
	#define IDL_SINGULAR_NAME		"Singular Symbol"
	// constants of names of adjustable sublayers
	#define IDL_NAS_LINIA_NAME		"Continuous(Adjustable)"
	#define IDL_NAS_SHTRIH_NAME		"Hatched(Adjustable)"
	#define IDL_NAS_PUNKTIR_NAME	"Dotted(Adjustable)"	
	#define IDL_NAS_SBSTRETCH_NAME	"Symbolic(Adjustable)"
	#define IDL_NAS_SOFRANGE_NAME	"Boundary Symbol(Adjustable)"
	#define IDL_NAS_ALGORITMA_NAME	"Algorithmic(Adjustable)"
	#define IDL_NAS_SPLINE_NAME		"Spline(Adjustable)"
	#define IDL_NAS_ORIENTED_NAME	"Node Symbol(Adjustable)"
	#define IDL_NAS_SINGULAR_NAME	"Singular Symbol"

#else  //Русская версия 
	//константы названий обычных прослоек
	#define	IDL_LINIA_NAME			"Сплошная"
	#define IDL_SHTRIH_NAME			"Штриховая"
	#define IDL_PUNKTIR_NAME		"Пунктирная"
	#define IDL_SBSTRETCH_NAME		"Символьная"
	#define IDL_SOFRANGE_NAME		"Гран. символ"
	#define IDL_ALGORITM_NAME		"Алгоритмическая"
	#define IDL_SPLINE_NAME			"Сплайновая"
	#define IDL_ORIENTED_NAME		"Узловой символ"
	#define IDL_SINGULAR_NAME		"Единичный символ"
	//константы названий настраиваемых прослоек
 	#define IDL_NAS_LINIA_NAME		"Сплошная (нас.)"
	#define IDL_NAS_SHTRIH_NAME		"Штриховая (нас.)"
	#define IDL_NAS_PUNKTIR_NAME	"Пунктирная (нас.)"	
	#define IDL_NAS_SBSTRETCH_NAME	"Символьная (нас.)"
	#define IDL_NAS_SOFRANGE_NAME	"Гран. символ (нас.)"
	#define IDL_NAS_ALGORITMA_NAME	"Алгоритмическая (нас.)"
	#define IDL_NAS_SPLINE_NAME		"Сплайновая (нас.)"
	#define IDL_NAS_ORIENTED_NAME	"Узловой символ (нас.)"
	#define IDL_NAS_SINGULAR_NAME	"Единичный символ (нас.)"
#endif

class MBaseNastr;

//PACKAGE : MAP.
//CLASS : MProsloika.
//DESCRIPTION : Class MProsloika is parent for all layers to locate them into one array.
class MProsloika
{
public:
	// variables
	float	m_distofaxe;	// < Offset from contour
	bool	m_synhro;		// < Synchronize with contour
	bool	m_drawrazryv;	// < Drawing at breaks
	
//PACKAGE : MAP.
//FUNCTION : MProsloika::MProsloika();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MProsloika():m_distofaxe(0),m_synhro(false),m_drawrazryv(true){};

//PACKAGE : MAP.
//FUNCTION : MProsloika::MProsloika(float distofaxe,bool synhro,bool drawrazryv);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float ,bool ,bool .
//RETURNS : NONE.
	MProsloika(float distofaxe,bool synhro,bool drawrazryv):m_distofaxe(distofaxe),
		m_synhro(synhro),m_drawrazryv(drawrazryv){};

//PACKAGE : MAP.
//FUNCTION : MProsloika::MProsloika(MProsloika* pProsloika);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MProsloika pointer.
//RETURNS : NONE.
	MProsloika(MProsloika* pProsloika);

//PACKAGE : MAP.
//FUNCTION : MProsloika::~MProsloika();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MProsloika(){};

//PACKAGE : MAP.
//FUNCTION : MProsloika::GetBytes();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	int GetBytes(){return GetSize();};

// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MProsloika::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType()=0;

//PACKAGE : MAP.
//FUNCTION : MProsloika::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName()=0;

//PACKAGE : MAP.
//FUNCTION : MProsloika::Clone();
//DESCRIPTION :  Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone()=0;

//PACKAGE : MAP.
//FUNCTION : MProsloika::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return sizeof(float)+sizeof(bool)+sizeof(bool);};

//PACKAGE : MAP.
//FUNCTION : MProsloika::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	virtual double GetMaxDimension(){return fabs(m_distofaxe);};

//PACKAGE : MAP.
//FUNCTION : MProsloika::Copy(MProsloika* prosloika);
//DESCRIPTION :  Makes a copy.
//INPUTS : MProsloika pointer.
//RETURNS : NONE.	
	virtual void Copy(MProsloika* prosloika);

//PACKAGE : MAP.
//FUNCTION : MProsloika::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MProsloika::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);  

//PACKAGE : MAP.
//FUNCTION : MProsloika::ReadBin(MFile* file, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);    

//PACKAGE : MAP.
//FUNCTION : MProsloika::ReadAny(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads layer of different type from UTP-file .
//INPUTS : FILE pointer, int .
//RETURNS : MProsloika pointer.
	static MProsloika* ReadAny(FILE* h_nuxfile, int version);  

//PACKAGE : MAP.
//FUNCTION : MProsloika::ReadAny(MFile* file, int version);
//DESCRIPTION : Reads layer of different type from UTP-file .
//INPUTS : MFile pointer, int .
//RETURNS : MProsloika pointer.
	static MProsloika* ReadAny(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MProsloika::IsPereopr();
//DESCRIPTION : Base layer - non redefinable.
//INPUTS : NONE.
//RETURNS : False.
	virtual bool IsPereopr(){return false;};

//PACKAGE : MAP.
//FUNCTION : MProsloika::GetBaseNast();
//DESCRIPTION : As layer is not redefinable.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return NULL;};
};


//PACKAGE : MAP.
//CLASS : MLinia:public MProsloika.
//DESCRIPTION : Class MLinia describes continuous line.
class  MLinia:public MProsloika
{
public:

	// variables
	float			m_thickness; // < Line thickness
	unsigned char	m_colorkod;  // < Table code of line color
	
//PACKAGE : MAP.
//FUNCTION : MLinia::MLinia();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MLinia():MProsloika(0,0,true),m_thickness(0),m_colorkod(0){};   

   //PACKAGE : MAP.
//FUNCTION : MLinia::MLinia(float tolschina, unsigned char colorkod);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char .
//RETURNS : NONE.
	MLinia(float tolschina, unsigned char colorkod):MProsloika(0,0,true),
			m_thickness(tolschina),m_colorkod(colorkod){};

//PACKAGE : MAP.
//FUNCTION : MLinia::MLinia(MLinia* lin);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MLinia pointer.
//RETURNS : NONE.
	MLinia(MLinia* lin);
	
//PACKAGE : MAP.
//FUNCTION : MLinia::~MLinia();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MLinia(){};
	
//PACKAGE : MAP.
//FUNCTION : MLinia::Copy(MLinia* linia);
//DESCRIPTION :  Function of copying.
//INPUTS : MLinia pointer.
//RETURNS : NONE.
	void Copy(MLinia* linia);

//PACKAGE : MAP.
//FUNCTION : MLinia::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MLinia(this);};

//PACKAGE : MAP.
//FUNCTION : MLinia::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),fabs(m_thickness));};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MLinia::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(float)+sizeof(m_colorkod);};
	
//PACKAGE : MAP.
//FUNCTION : MLinia::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE* h_nuxfile, int version.
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MLinia::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MLinia::ReadBin(MFile* file, int version);
//DESCRIPTION : Read from to UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);      

//PACKAGE : MAP.
//FUNCTION : MLinia::GetType();
//DESCRIPTION :  Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_LINIA;};

//PACKAGE : MAP.
//FUNCTION : MLinia::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_LINIA_NAME;};
};


//PACKAGE : MAP.
//CLASS : MSbStretch:public MProsloika.
//DESCRIPTION : Class MSbStretch describes outstretching of symbol along line.
class  MSbStretch:public MProsloika
{
public:

	// variables
	float    m_sbpause;  // < Offet of symbol from the beginning of line
	MSymbol  m_symbol;   // < Symbol, outstreched along line
	float    m_period;   // < Period of outstretching of symbols
	

//PACKAGE : MAP.
//FUNCTION : MSbStretch::MSbStretch();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MSbStretch():MProsloika(0,0,true),m_sbpause(0),m_period(1){};

//PACKAGE : MAP.
//FUNCTION : MSbStretch::MSbStretch(float pause, MSymbol* sym, float dlprob);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , MSymbol pointer, float .
//RETURNS : NONE.
	MSbStretch(float pause, MSymbol* sym, float dlprob);

//PACKAGE : MAP.
//FUNCTION : MSbStretch::MSbStretch(MSbStretch* sbstr);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MSbStretch pointer.
//RETURNS : NONE.
	MSbStretch(MSbStretch* sbstr);
	
//PACKAGE : MAP.
//FUNCTION : MSbStretch::~MSbStretch();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MSbStretch(){};
	
//PACKAGE : MAP.
//FUNCTION : MSbStretch::Copy(MSbStretch* sbstretch);
//DESCRIPTION : Function of copying.
//INPUTS : MSbStretch pointer.
//RETURNS : NONE.
	void Copy(MSbStretch* sbstretch);

//PACKAGE : MAP.
//FUNCTION : MSbStretch::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MSbStretch(this);};

//PACKAGE : MAP.
//FUNCTION : MSbStretch::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),Max(Max(fabs(m_sbpause),fabs(m_period)),fabs(m_symbol.value)));};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MSbStretch::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(float)+sizeof(float)+m_symbol.GetBytes();};
	
//PACKAGE : MAP.
//FUNCTION : MSbStretch::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION :  Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 
//PACKAGE : MAP.
//FUNCTION : MSbStretch::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION :  Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MSbStretch::ReadBin(MFile* file, int version);
//DESCRIPTION :  Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);    

//PACKAGE : MAP.
//FUNCTION : MSbStretch::GetType();
//DESCRIPTION :  Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_SBSTRETCH;};

//PACKAGE : MAP.
//FUNCTION : MSbStretch::GetName();
//DESCRIPTION :  Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_SBSTRETCH_NAME;};
};

//PACKAGE : MAP.
//CLASS : MShtrih:public MProsloika.
//DESCRIPTION : Class MShtrih describes hatched line.
class  MShtrih:public MProsloika
{
public:
	// variables
	float			m_thickness; // < line thickness
	unsigned char	m_colorkod;  // < table code of hatch color
	float			m_dlina;     // < hatch length
	float			m_period;    // < period of hatched line
	bool			m_kvadrat;   // < flag of squareness of hatch
	float			m_otstup ;   // < offset of the first hatch from the beginning
	
//PACKAGE : MAP.
//FUNCTION : MShtrih::MShtrih();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MShtrih():MProsloika(0,0,true),m_thickness(0.1f),m_colorkod(0),
		m_dlina(1),m_period(1),m_kvadrat(true),m_otstup(0){};   

//PACKAGE : MAP.
//FUNCTION : MShtrih::MShtrih(float tolschina, unsigned char colorkod, float dlshtrih, float dlprob,bool kvadrat, float otstup);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char , float , float ,bool , float .
//RETURNS : NONE.      
	MShtrih(float tolschina, unsigned char colorkod, float dlshtrih, float dlprob,bool kvadrat, float otstup):MProsloika(0,0,true),
		m_thickness(tolschina),m_colorkod(colorkod),m_dlina(dlshtrih),m_period(dlprob),m_kvadrat(kvadrat),m_otstup(otstup){};

//PACKAGE : MAP.
//FUNCTION : MShtrih::MShtrih(MShtrih* shtr);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MShtrih pointer.
//RETURNS : NONE.
	MShtrih(MShtrih* shtr);
	
//PACKAGE : MAP.
//FUNCTION : MShtrih::~MShtrih();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MShtrih(){};

//PACKAGE : MAP.
//FUNCTION : MShtrih::Copy(MShtrih* shtrih);
//DESCRIPTION : Function of copying.
//INPUTS : MShtrih pointer.
//RETURNS : NONE.
	void Copy(MShtrih* shtrih);

//PACKAGE : MAP.
//FUNCTION : MShtrih::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MShtrih(this);};

//PACKAGE : MAP.
//FUNCTION : MShtrih::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),Max(Max(fabs(m_dlina),fabs(m_period)),Max(fabs(m_otstup),fabs(m_thickness))));};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MShtrih::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(m_thickness)
		+sizeof(m_colorkod)+sizeof(m_dlina)+sizeof(m_period)+sizeof(m_kvadrat)+sizeof(m_otstup);};
	
//PACKAGE : MAP.
//FUNCTION : MShtrih::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION :  Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MShtrih::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION :  Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MShtrih::ReadBin(MFile* file, int version);
//DESCRIPTION :  Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);   

//PACKAGE : MAP.
//FUNCTION : MShtrih::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_SHTRIH;};

//PACKAGE : MAP.
//FUNCTION : MShtrih::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_SHTRIH_NAME;};
};

//PACKAGE : MAP.
//CLASS : MPunktir:public MProsloika.
//DESCRIPTION : Class MPunktir describes dotted line.
class  MPunktir : public MProsloika
{
public:
	unsigned char	m_colorkod; // < Table code of line color
	float			m_diametr;  // < diameter of point of dotted line
	float			m_period;   // < period of dotted line
	float			m_nachalo;  // < offset of the first hatch from the beginning ëèíèè
	
//PACKAGE : MAP.
//FUNCTION : MPunktir::MPunktir();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MPunktir():MProsloika(0,0,true),m_colorkod(0),m_diametr(0.1f),m_period(1),m_nachalo(0){};    

//PACKAGE : MAP.
//FUNCTION : MPunktir::MPunktir(float diampunkt, unsigned char colorkod, float dlprob,float nachalo);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char , float ,float .
//RETURNS : NONE.       
	MPunktir(float diampunkt, unsigned char colorkod, float dlprob,float nachalo):MProsloika(0,0,true),
		m_colorkod(colorkod),m_diametr(diampunkt),m_period(dlprob),m_nachalo(nachalo){};

//PACKAGE : MAP.
//FUNCTION : MPunktir::MPunktir(MPunktir* punk);
//DESCRIPTION : Constructor defined type.
//INPUTS : MPunktir pointer.
//RETURNS : NONE.
	MPunktir(MPunktir* punk);
	
//PACKAGE : MAP.
//FUNCTION : MPunktir::~MPunktir();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MPunktir(){};
	
//PACKAGE : MAP.
//FUNCTION : MPunktir::Copy(MPunktir* punktir);
//DESCRIPTION : Function of copying.
//INPUTS : MPunktir pointer.
//RETURNS : NONE.
	void Copy(MPunktir* punktir);

//PACKAGE : MAP.
//FUNCTION : MPunktir::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MPunktir(this);};

//PACKAGE : MAP.
//FUNCTION : MPunktir::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),Max(fabs(m_diametr),Max(fabs(m_period),fabs(m_nachalo))));};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MPunktir::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(m_colorkod)
		+sizeof(m_diametr)+sizeof(m_period)+sizeof(m_nachalo);};
	
//PACKAGE : MAP.
//FUNCTION : MPunktir::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MPunktir::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MPunktir::ReadBin(MFile* file, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);    

//PACKAGE : MAP.
//FUNCTION : MPunktir::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_PUNKTIR;};

//PACKAGE : MAP.
//FUNCTION : MPunktir::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_PUNKTIR_NAME;};
};


// constants of boundary symbol types
#define IDR_BEGIN 0
#define IDR_END   1
#define IDR_BOTH  2

//PACKAGE : MAP.
//CLASS : MRangeSym:public MProsloika.
//DESCRIPTION : Class MRangeSym describes boundary symbol.
class  MRangeSym:public MProsloika
{
public:
	// variables
	unsigned char	m_type ;	// <boundary symbol type
	MSymbol			m_symbol;	// <boundary type

//PACKAGE : MAP.
//FUNCTION : MRangeSym::MRangeSym();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MRangeSym():m_type(0){};

//PACKAGE : MAP.
//FUNCTION : MRangeSym::MRangeSym(int stype, MSymbol* ssymbol);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : int , MSymbol pointer.
//RETURNS : NONE.
	MRangeSym(int stype, MSymbol* ssymbol):m_type(stype){if(ssymbol) m_symbol.Copy(ssymbol);};

//PACKAGE : MAP.
//FUNCTION : MRangeSym::MRangeSym(MRangeSym* rgsym);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MRangeSym pointer.
//RETURNS : NONE.
	MRangeSym(MRangeSym* rgsym);

//PACKAGE : MAP.
//FUNCTION : MRangeSym::~MRangeSym();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MRangeSym(){};

//PACKAGE : MAP.
//FUNCTION : MRangeSym::Copy(MRangeSym* rangesym);
//DESCRIPTION : Function of copying.
//INPUTS : MRangeSym pointer.
//RETURNS : NONE.
	void Copy(MRangeSym* rangesym);

//PACKAGE : MAP.
//FUNCTION : MRangeSym::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MRangeSym(this);};

//PACKAGE : MAP.
//FUNCTION : MRangeSym::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),fabs(m_symbol.value));};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MRangeSym::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(m_type)+m_symbol.GetBytes();};
	
//PACKAGE : MAP.
//FUNCTION : MRangeSym::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MRangeSym::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MRangeSym::ReadBin(MFile* file, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);     

//PACKAGE : MAP.
//FUNCTION : MRangeSym::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_SOFRANGE;};

//PACKAGE : MAP.
//FUNCTION : MRangeSym::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_SOFRANGE_NAME;};
};

// constants of types of oriented outstraching
#define MOS_NOORIENT	0
#define MOS_NEXTORIENT  1
#define MOS_PREVORIENT  2
#define MOS_BISORIENT   3

//PACKAGE : MAP.
//CLASS : MOrientedStretch:public MProsloika.
//DESCRIPTION : Class describes a line along which stretched the symbol.
class  MOrientedStretch:public MProsloika
{
public:
	// variables
	unsigned char	m_type ;  // <boundary symbol type
	MSymbol			m_symbol; // <boundary type

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::MOrientedStretch();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MOrientedStretch():m_type(MOS_NOORIENT){};

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::MOrientedStretch(int stype, MSymbol* ssymbol);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : int , MSymbol pointer.
//RETURNS : NONE.
	MOrientedStretch(int stype, MSymbol* ssymbol):m_type(stype){if(ssymbol) m_symbol.Copy(ssymbol);};

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::MOrientedStretch(MOrientedStretch* rgsym);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MOrientedStretch pointer.
//RETURNS : NONE.
	MOrientedStretch(MOrientedStretch* rgsym);
	
//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::~MOrientedStretch();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MOrientedStretch(){};
	
//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::Copy(MOrientedStretch* rangesym);
//DESCRIPTION : Function of copying.
//INPUTS : MOrientedStretch pointer.
//RETURNS : NONE.
	void Copy(MOrientedStretch* rangesym);

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MOrientedStretch(this);};

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),fabs(m_symbol.value));};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(m_type)+m_symbol.GetBytes();};
	
//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);  

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::ReadBin(MFile* file, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);      

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_ORIENTED;};

//PACKAGE : MAP.
//FUNCTION : MOrientedStretch::GetName();
//DESCRIPTION :  Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_ORIENTED_NAME;};
};


// constants of spline types
#define SPT_LOMANAYA 0
#define SPT_INTERP	1
#define SPT_APPROX  2

// constants of spline views
#define SPV_ZAMKNUT   0
#define SPV_NEZAMKNUT 1

//PACKAGE : MAP.
//CLASS : MSpline:public MProsloika.
//DESCRIPTION : Class MSpline describes spline.
class  MSpline:public MProsloika
{
public:
	float			m_thickness; // < line thickness
	unsigned char	m_colorkod;  // < table code of spline color
	unsigned char	m_type;      // < spline type
	unsigned char	m_vid;       // < spline view
	int				m_kolpts;    // < Points quantity at edge partitioning of broken line which is interpolated by spline
	
//PACKAGE : MAP.
//FUNCTION : MSpline::MSpline();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MSpline():m_thickness(0.1f),m_colorkod(0),m_type(SPT_APPROX),m_vid(SPV_NEZAMKNUT),m_kolpts(10){}; 

 //PACKAGE : MAP.
//FUNCTION : MSpline::MSpline(float tolschina, unsigned char colorkod, int type, int vid, int kolpts);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char , int , int , int .
//RETURNS : NONE.        
	MSpline(float tolschina, unsigned char colorkod, int type, int vid, int kolpts):m_thickness(tolschina),
		m_colorkod(colorkod),m_type(type),m_vid(vid),m_kolpts(kolpts){};

//PACKAGE : MAP.
//FUNCTION : MSpline::MSpline(MSpline* spline);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MSpline pointer.
//RETURNS : NONE.
	MSpline(MSpline* spline);
	
//PACKAGE : MAP.
//FUNCTION : MSpline::~MSpline();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MSpline(){};

//PACKAGE : MAP.
//FUNCTION : MSpline::Copy(MSpline* spline);
//DESCRIPTION : Function of copying.
//INPUTS : MSpline pointer.
//RETURNS : NONE.
	void Copy(MSpline* spline);

//PACKAGE : MAP.
//FUNCTION : MSpline::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MSpline(this);};

//PACKAGE : MAP.
//FUNCTION : MSpline::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(),fabs(m_thickness));};
	
//PACKAGE : MAP.
//FUNCTION : MSpline::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MSpline::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version);  

//PACKAGE : MAP.
//FUNCTION : MSpline::ReadBin(MFile* file, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version);   

//PACKAGE : MAP.
//FUNCTION : MSpline::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_SPLINE;};

//PACKAGE : MAP.
//FUNCTION : MSpline::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_SPLINE_NAME;};
};

//PACKAGE : MAP.
//CLASS : MSbSingular:public MProsloika.
//DESCRIPTION : Class MSbSingular describes symbol. 
class  MSbSingular:public MProsloika
{
public:
	// variables
	MSymbol  m_symbol;   // < symbol
	float	 m_pause;	// < indent from beginning of line
	
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::MSbSingular();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MSbSingular():MProsloika(0,true,true),m_pause(-1){};

//PACKAGE : MAP.
//FUNCTION : MSbSingular::MSbSingular(MSymbol* sym, float f_pause);
//DESCRIPTION : Constructor.
//INPUTS : MSymbol pointer, float .
//RETURNS : NONE.
	MSbSingular(MSymbol* sym, float f_pause);

//PACKAGE : MAP.
//FUNCTION : MSbSingular::MSbSingular(MSbSingular* sbsym);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MSbSingular.
//RETURNS : NONE.
	MSbSingular(MSbSingular* sbsym) { Copy(sbsym);};
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::~MSbSingular();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MSbSingular(){};
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::Copy(MSbSingular* sbsymbol);
//DESCRIPTION : Function of copying.
//INPUTS : MSbSingular pointer.
//RETURNS : NONE.
	void Copy(MSbSingular* sbsymbol);

//PACKAGE : MAP.
//FUNCTION : MSbSingular::Clone();
//DESCRIPTION : Makes a clone of layer.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MSbSingular(this);};
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::GetMaxDimension();
//DESCRIPTION : Gets max distance between edge points of drawing.
//INPUTS : NONE.
//RETURNS : double.
	double GetMaxDimension(){return Max(MProsloika::GetMaxDimension(), Max(fabs(m_pause),fabs(m_symbol.value)));};
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MProsloika::GetSize()+sizeof(float)+m_symbol.GetBytes();};
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MSbSingular::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Read  from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 
//PACKAGE : MAP.
//FUNCTION : MSbSingular::WriteBin(MFile* file, int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version); 
	
//PACKAGE : MAP.
//FUNCTION : MSbSingular::GetType();
//DESCRIPTION :  Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_SINGULAR;};

//PACKAGE : MAP.
//FUNCTION : MSbSingular::GetName();
//DESCRIPTION :  Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_SINGULAR_NAME;};
};

//PACKAGE : MAP.
//CLASS : MAlgline.
//DESCRIPTION : Class MAlgline describes action for drawing algorithmic line.
class  MAlgline
{
public:
	// variables
	float m_stepahead ;// < step in the line of (+ forward, - backward)
	float m_stepaside ;// < step in the line of forward direction (+ to the right, - to the left)
	bool  m_draw      ;// < flag of drawing (true - draw, false - do not draw)
	
//PACKAGE : MAP.
//FUNCTION : MAlgline::MAlgline();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MAlgline():m_stepahead(0),m_stepaside(0),m_draw(false){};

//PACKAGE : MAP.
//FUNCTION : MAlgline::MAlgline(float sahead, float saside, bool fdraw);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , float , bool .
//RETURNS : NONE.
	MAlgline(float sahead, float saside, bool fdraw):m_stepahead(sahead),m_stepaside(saside),m_draw(fdraw){};
	
//PACKAGE : MAP.
//FUNCTION : MAlgline::~MAlgline();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MAlgline(){};

//PACKAGE : MAP.
//FUNCTION : MAlgline::Copy(MAlgline* pAlgline);
//DESCRIPTION : Function of copying.
//INPUTS : MAlgline pointer.
//RETURNS : NONE.
	void Copy(MAlgline* pAlgline);
};


// Constant checks for absence of parameter redefinition
#define PEREOPR_NOT_EXIST USHRT_MAX 

//PACKAGE : MAP.
//CLASS : MBaseNastr.
//DESCRIPTION : Class MBaseNastr redefines different parameters for base classes. Also stores and saves arrays of redefinition and contains virtual interface for editing.
class  MBaseNastr
{
	MShInArray m_PereoprIndexes;

public:

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::MBaseNastr();
//DESCRIPTION : Constructor.
//INPUTS : int.
//RETURNS : NONE.
	MBaseNastr(int iSize);

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::MBaseNastr(MBaseNastr* pBase);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MBaseNastr pointer.
//RETURNS : NONE.
	MBaseNastr(MBaseNastr* pBase){Copy(pBase);};

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::~MBaseNastr();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MBaseNastr(){};

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::Copy(MBaseNastr* pBase);
//DESCRIPTION : Copies.
//INPUTS : MBaseNastr pointer.
//RETURNS : NONE.
	virtual void Copy(MBaseNastr* pBase);
	
//PACKAGE : MAP.
//FUNCTION : MBaseNastr::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int  GetSize();
	
//PACKAGE : MAP.
//FUNCTION : MBaseNastr::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes settings of redefinition to UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads settings of redefinition from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads settings of redefinition from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	bool ReadBin(MFile* file, int version);
	
//PACKAGE : MAP.
//FUNCTION : MBaseNastr::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefined parameter.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)=0;

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::GetPerIndex(int index);
//DESCRIPTION : Gets index of redefined parameter.
//INPUTS : int.
//RETURNS : unsigned short.
	unsigned short  GetPerIndex(int index);

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::IsPereoprIndex(int iIndex);
//DESCRIPTION : Is parameter with index 'index' redefined?
//INPUTS : int.
//RETURNS : True.
	bool IsPereoprIndex(int iIndex);

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::ClearPerIndex(int iIndex);
//DESCRIPTION : Deletes values of redefined parameter with index 'iIndex'.
//INPUTS : int.
//RETURNS : NONE.
	void ClearPerIndex(int iIndex){unsigned short temp = PEREOPR_NOT_EXIST; m_PereoprIndexes.SetAt(iIndex,&temp);};

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::SetPerIndex(int iIndex,unsigned short val);
//DESCRIPTION : Sets value of redefined parameter with index 'iIndex'.
//INPUTS : int.
//RETURNS : NONE.
	void SetPerIndex(int iIndex,unsigned short val){m_PereoprIndexes.SetAt(iIndex,&val);};

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::IsPereopr();
//DESCRIPTION : Virtual function.
// 				If redefinitable?
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr()=0;

//PACKAGE : MAP.
//FUNCTION : MBaseNastr::GetPereoprDim();
//DESCRIPTION :  Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim()=0;
};

// Constants for class of adjustable line 'MNastrLinia'

#define IDL_NAS_LINIA_SIZE		2	// quantity of adjustable parameters

#define IDL_NAS_PROSLOIKA_VALUE	0	// offset from axis
#define IDL_NAS_LINIA_VAL_TOL	1	// line thickness

//PACKAGE : MAP.
//CLASS : MNastrLinia:public MLinia, public MBaseNastr.
//DESCRIPTION : Class MNastrLinia describes adjustable continuous line with the availability of adjusting : 	line thickness, offset from axis.
class  MNastrLinia:public MLinia, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::MNastrLinia();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrLinia():MBaseNastr(IDL_NAS_LINIA_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::MNastrLinia(float tolschina, unsigned char colorkod);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char .
//RETURNS : NONE.
	MNastrLinia(float tolschina, unsigned char colorkod):MLinia(tolschina,colorkod),MBaseNastr(IDL_NAS_LINIA_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::MNastrLinia(MNastrLinia* lin);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MNastrLinia pointer.
//RETURNS : NONE.
	MNastrLinia(MNastrLinia* lin):MLinia((MLinia*)lin),MBaseNastr((MBaseNastr*)lin){};
	
//PACKAGE : MAP.
//FUNCTION : MNastrLinia::~MNastrLinia();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrLinia(){};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::Copy(MNastrLinia* linia);
//DESCRIPTION :  Function of copying.
//INPUTS : MNastrLinia pointer.
//RETURNS : NONE.
	void Copy(MNastrLinia* linia)
	{
		MLinia::Copy((MLinia*)linia);
		MBaseNastr::Copy((MBaseNastr*)linia);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MNastrLinia(this);};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::GetSize();
//DESCRIPTION :  Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MLinia::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MLinia::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MLinia::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MLinia::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrLinia::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefinitable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PROSLOIKA_VALUE:
			return "Смещение от оси";
		case IDL_NAS_LINIA_VAL_TOL:
			return "Толщина";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::IsPereopr();
//DESCRIPTION :  Redefinitable? - Yes.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_LINIA_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_NAS_LINIA;};

//PACKAGE : MAP.
//FUNCTION : MNastrLinia::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_NAS_LINIA_NAME;};

};

// Constants for class of adjustable hatched line MNastrShtrih

#define IDL_NAS_SHTRIH_SIZE			5 // quantity of adjustable parameters

#define IDL_NAS_SHTRIH_VAL_TOL		1 // line thickness
#define IDL_NAS_SHTRIH_VAL_DLINA	2 // hatch length
#define IDL_NAS_SHTRIH_VAL_PERIOD	3 // period
#define IDL_NAS_SHTRIH_VAL_OTSTUP	4 // offset from the beginning

//PACKAGE : MAP.
//CLASS : MNastrShtrih:public MShtrih, public MBaseNastr.
//DESCRIPTION : Class MNastrShtrih describes hatched line, with the availability of adjusting of: line thickness, offset from axis, hatch lengths, period, offest from the beginning.
class  MNastrShtrih:public MShtrih, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::MNastrShtrih();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrShtrih():MBaseNastr(IDL_NAS_SHTRIH_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::MNastrShtrih(float tolschina, unsigned char colorkod,float fShtr,float fProb,bool kvadrat,float otstup);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char ,float ,float ,bool ,float .
//RETURNS : NONE.
	MNastrShtrih(float tolschina, unsigned char colorkod,float fShtr,float fProb,bool kvadrat,float otstup)
		:MShtrih(tolschina,colorkod,fShtr,fProb,kvadrat,otstup),MBaseNastr(IDL_NAS_SHTRIH_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::MNastrShtrih(MNastrShtrih* lin);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MNastrShtrih pointer.
//RETURNS : NONE.
	MNastrShtrih(MNastrShtrih* lin):MShtrih((MShtrih*)lin),MBaseNastr((MBaseNastr*)lin){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::~MNastrShtrih();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrShtrih(){};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::Copy(MNastrShtrih* linia);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrShtrih pointer.
//RETURNS : NONE.
	void Copy(MNastrShtrih* linia)
	{
		MShtrih::Copy((MShtrih*)linia);
		MBaseNastr::Copy((MBaseNastr*)linia);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MNastrShtrih(this);};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MShtrih::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MShtrih::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MShtrih::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MShtrih::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefinitable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PROSLOIKA_VALUE:
			return "Смещение от оси";
		case IDL_NAS_SHTRIH_VAL_TOL:
			return "Толщина";
		case IDL_NAS_SHTRIH_VAL_DLINA:
			return "Длина штриха";
		case IDL_NAS_SHTRIH_VAL_PERIOD:
			return "Период";
		case IDL_NAS_SHTRIH_VAL_OTSTUP:
			return "Смещение от начала";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::IsPereopr();
//DESCRIPTION : Redefinitable? - Yes.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_SHTRIH_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.	
	virtual unsigned char GetType(){return IDL_NAS_SHTRIH;};

//PACKAGE : MAP.
//FUNCTION : MNastrShtrih::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_NAS_SHTRIH_NAME;};

};

// Constants of class of adjustable dotted line MNastrPunktir

#define IDL_NAS_PUNKTIR_SIZE		4 // quantity of adjustable parameters

#define IDL_NAS_PUNKTIR_VAL_TOL		1 // diameter
#define IDL_NAS_PUNKTIR_VAL_PERIOD	2 // period
#define IDL_NAS_PUNKTIR_VAL_OTSTUP	3 // offset from the beginning

//PACKAGE : MAP.
//CLASS : MNastrPunktir:public MPunktir, public MBaseNastr.
//DESCRIPTION : Class MNastrPunktir describes dotted line with the availability of adjusting of: offset from axis, diameter, offset from the beginning.
class  MNastrPunktir:public MPunktir, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::MNastrPunktir();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrPunktir():MBaseNastr(IDL_NAS_SHTRIH_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::MNastrPunktir(float tolschina,unsigned char colorkod,float fProb,float otstup);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float ,unsigned char ,float ,float .
//RETURNS : NONE.
	MNastrPunktir(float tolschina,unsigned char colorkod,float fProb,float otstup)
		:MPunktir(tolschina,colorkod,fProb,otstup),MBaseNastr(IDL_NAS_PUNKTIR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::MNastrPunktir(MNastrPunktir* lin);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MNastrPunktir pointer.
//RETURNS : NONE.
	MNastrPunktir(MNastrPunktir* lin):MPunktir((MPunktir*)lin),MBaseNastr((MBaseNastr*)lin){};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::~MNastrPunktir();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrPunktir(){};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::Copy(MNastrPunktir* linia);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrPunktir pointer.
//RETURNS : NONE.
	void Copy(MNastrPunktir* linia)
	{
		MPunktir::Copy((MPunktir*)linia);
		MBaseNastr::Copy((MBaseNastr*)linia);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MNastrPunktir(this);};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MPunktir::GetSize()+MBaseNastr::GetSize();};


//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MPunktir::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MPunktir::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::ReadBin(FMFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MPunktir::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefinitable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PROSLOIKA_VALUE:
			return "Смещение от оси";
		case IDL_NAS_PUNKTIR_VAL_TOL:
			return "Диаметр";
		case IDL_NAS_PUNKTIR_VAL_PERIOD:
			return "Период";
		case IDL_NAS_PUNKTIR_VAL_OTSTUP:
			return "Смещение от начала";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::IsPereopr();
//DESCRIPTION : Redefinitable? - Yes.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_PUNKTIR_SIZE;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_NAS_PUNKTIR;};

//PACKAGE : MAP.
//FUNCTION : MNastrPunktir::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_NAS_PUNKTIR_NAME;};

};

// Constants of class of adjustable line with outstrectching of symbol MNastrSbStretch

#define IDL_NAS_SBSTRETCH_SIZE			5 // quantity of adjustable parameters

#define IDL_NAS_SBSTRETCH_VAL_PERIOD	1 // period ïðîòÿæêè
#define IDL_NAS_SBSTRETCH_VAL_OTSTUP	2 // offset from the beginning
#define IDL_NAS_SBSTRETCH_VAL_SYM_UGOL	3 // óãîë ïîâîðîòà ñèìâîëà
#define IDL_NAS_SBSTRETCH_VAL_SYM_VAL	4 // ðàçìåð ñèìâîëà

//PACKAGE : MAP.
//CLASS : MNastrSbStretch:public MSbStretch, public MBaseNastr.
//DESCRIPTION : Class MNastrSbStretch describes outstretching of symbol along line with the availability of adjustment of:	offset from axis, period of outstretching, offset from the beginning,	rotation angle, symbol size.
class  MNastrSbStretch:public MSbStretch, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::MNastrSbStretch();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrSbStretch():MBaseNastr(IDL_NAS_SBSTRETCH_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::MNastrSbStretch(float pause, MSymbol* symbol,float dlProb);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , MSymbol pointer,float .
//RETURNS : NONE.
	MNastrSbStretch(float pause, MSymbol* symbol,float dlProb)
		:MSbStretch(pause,symbol,dlProb),MBaseNastr(IDL_NAS_SBSTRETCH_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::MNastrSbStretch(MNastrSbStretch* lin);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MNastrSbStretch pointer.
//RETURNS : NONE.
	MNastrSbStretch(MNastrSbStretch* lin):MSbStretch((MSbStretch*)lin),MBaseNastr((MBaseNastr*)lin){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::~MNastrSbStretch();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrSbStretch(){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::Copy(MNastrSbStretch* linia);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrSbStretch pointer.
//RETURNS : NONE.
	void Copy(MNastrSbStretch* linia)
	{
		MSbStretch::Copy((MSbStretch*)linia);
		MBaseNastr::Copy((MBaseNastr*)linia);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MNastrSbStretch(this);};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::GetSize();
//DESCRIPTION :  Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MSbStretch::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : NONE.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MSbStretch::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : NONE.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MSbStretch::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : NONE.
	virtual bool ReadBin(MFile* file, int version)
	{
		MSbStretch::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefinitable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PROSLOIKA_VALUE:
			return "Смещение от оси";
		case IDL_NAS_SBSTRETCH_VAL_PERIOD:
			return "Период";
		case IDL_NAS_SBSTRETCH_VAL_OTSTUP:
			return "Смещение от начала";
		case IDL_NAS_SBSTRETCH_VAL_SYM_UGOL:
			return "Угол поворота символа";
		case IDL_NAS_SBSTRETCH_VAL_SYM_VAL:
			return "Размер символа";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::IsPereopr();
//DESCRIPTION : Redefinitable? - Yes.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_SBSTRETCH_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_NAS_SBSTRETCH;};

//PACKAGE : MAP.
//FUNCTION : MNastrSbStretch::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_NAS_SBSTRETCH_NAME;};

};

// Constants of class of adjustable spline MNastrSpline

#define IDL_NAS_SPLINE_SIZE				3 // quantity of adjustable parameters

#define IDL_NAS_SPLINE_VAL_TOL			1 // line thickness
#define IDL_NAS_SPLINE_VAL_NUMPOINTS	2 // points quantity

//PACKAGE : MAP.
//CLASS : MNastrSpline:public MSpline, public MBaseNastr.
//DESCRIPTION : Class MNastrSpline describes spline, with the availability of adjusting of: line thickness, offset from axis, points quantity of spline.
class  MNastrSpline:public MSpline, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::MNastrSpline();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrSpline():MBaseNastr(IDL_NAS_SPLINE_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::MNastrSpline(float tol, unsigned char colorkod,int iType,int iVid,int iKolpts);
//DESCRIPTION : Constructor with established parameters.
//INPUTS : float , unsigned char ,int ,int ,int .
//RETURNS : NONE.
	MNastrSpline(float tol, unsigned char colorkod,int iType,int iVid,int iKolpts)
		:MSpline(tol,colorkod,iType,iVid,iKolpts),MBaseNastr(IDL_NAS_SPLINE_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::MNastrSpline(MNastrSpline* lin);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MNastrSpline pointer.
//RETURNS : NONE.
	MNastrSpline(MNastrSpline* lin):MSpline((MSpline*)lin),MBaseNastr((MBaseNastr*)lin){};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::~MNastrSpline();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MNastrSpline(){};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::Copy(MNastrSpline* linia);
//DESCRIPTION : Function of copying.
//INPUTS : MNastrSpline pointer.
//RETURNS : NONE.
	void Copy(MNastrSpline* linia)
	{
		MSpline::Copy((MSpline*)linia);
		MBaseNastr::Copy((MBaseNastr*)linia);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MNastrSpline(this);};

	// Functions for redefining

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::GetSize();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MSpline::GetSize()+MBaseNastr::GetSize();};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MSpline::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MSpline::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MSpline::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrSpline::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefinitable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_PROSLOIKA_VALUE:
			return "Смещение от оси";
		case IDL_NAS_SPLINE_VAL_TOL:
			return "Толщина";
		case IDL_NAS_SPLINE_VAL_NUMPOINTS:
			return "Количество точек";
	//		default:	exit(15);
		}
		return strEmpty;
	};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::IsPereopr();
//DESCRIPTION : Redefinitable? - Yes.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_SPLINE_SIZE;};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_NAS_SPLINE;};

//PACKAGE : MAP.
//FUNCTION : MNastrSpline::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_NAS_SPLINE_NAME;};

};

// The constants for the class with a single line custom symbol MNastrSbSingular

#define IDL_NAS_SBSINGULAR_SIZE			4 // quantity of adjustable parameters

#define IDL_NAS_SBSINGULAR_VAL_SDVIG	0 // indent from beginning
#define IDL_NAS_SBSINGULAR_VAL_SYM_UGOL	1 // angle
#define IDL_NAS_SBSINGULAR_VAL_SYM_VAL	2 // value
#define IDL_NAS_SBSINGULAR_SYM_OPISANIE 3 // symbol

//PACKAGE : MAP.
//CLASS : MNastrSbSingular:public MSbSingular, public MBaseNastr.
//DESCRIPTION : Class with a single line custom symbol.
class  MNastrSbSingular:public MSbSingular, public MBaseNastr
{
public:

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::MNastrSbSingular();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MNastrSbSingular():MSbSingular(),MBaseNastr(IDL_NAS_SBSINGULAR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::MNastrSbSingular(MSymbol* symbol, float pause);
//DESCRIPTION : Constructor.
//INPUTS : MSymbol pointer, float .
//RETURNS : NONE.
	MNastrSbSingular(MSymbol* symbol, float pause)
		:MSbSingular(symbol, pause),MBaseNastr(IDL_NAS_SBSINGULAR_SIZE){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::MNastrSbSingular(MNastrSbSingular* sym);
//DESCRIPTION : Constructor by defined type.
//INPUTS : MNastrSbSingular pointer.
//RETURNS : NONE.
	MNastrSbSingular(MNastrSbSingular* sym):MSbSingular((MSbSingular*)sym),MBaseNastr((MBaseNastr*)sym){};

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::~MNastrSbSingular();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.	
	~MNastrSbSingular(){};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::Copy(MNastrSbSingular* symbol);
//DESCRIPTION :  Function of copying.
//INPUTS : MNastrSbSingular pointer.
//RETURNS : NONE.
	void Copy(MNastrSbSingular* symbol)
	{
		MSbSingular::Copy((MSbSingular*)symbol);
		MBaseNastr::Copy((MBaseNastr*)symbol);
	}
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MProsloika pointer.
	virtual MProsloika* Clone(){return (MProsloika*) new MNastrSbSingular(this);};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::GetSize();
//DESCRIPTION :  Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetSize(){return MSbSingular::GetSize()+MBaseNastr::GetSize();};
		
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile, int version)
	{
		MSbSingular::WriteBin(h_nuxfile,version);
		return MBaseNastr::WriteBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile, int version)
	{
		MSbSingular::ReadBin(h_nuxfile,version);
		return MBaseNastr::ReadBin(h_nuxfile,version);
	}

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file, int version)
	{
		MSbSingular::ReadBin(file,version);
		return MBaseNastr::ReadBin(file,version);
	}
	

//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::GetPerName(int iIndex);
//DESCRIPTION : Gets name of redefinitable parameter.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetPerName(int iIndex)
	{
		MString strEmpty = "";
		switch(iIndex){
		case IDL_NAS_SBSINGULAR_VAL_SDVIG:
			return "Сдвиг от начала контура";
		case IDL_NAS_SBSINGULAR_VAL_SYM_UGOL:
			return "Угол поворота символа";
		case IDL_NAS_SBSINGULAR_VAL_SYM_VAL:
			return "Размер символа";
		case IDL_NAS_SBSINGULAR_SYM_OPISANIE:
			return "Символ";
			//		default:	exit(15);
		}
		return strEmpty;
	};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::IsPereopr();
//DESCRIPTION : Redefinitable? - Yes.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsPereopr(){return true;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::GetBaseNast();
//DESCRIPTION : Cast to MBaseNast.
//INPUTS : NONE.
//RETURNS : MBaseNastr pointer.
	virtual MBaseNastr* GetBaseNast(){return (MBaseNastr*)this;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::GetPereoprDim();
//DESCRIPTION : Gets quantity of redefinitable parameters.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetPereoprDim(){return IDL_NAS_SBSINGULAR_SIZE;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::GetType();
//DESCRIPTION : Gets layer type.
//INPUTS : NONE.
//RETURNS : unsigned char.
	virtual unsigned char GetType(){return IDL_NAS_SINGULAR;};
	
//PACKAGE : MAP.
//FUNCTION : MNastrSbSingular::GetName();
//DESCRIPTION : Gets layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString GetName(){return IDL_NAS_SINGULAR_NAME;};
	
};

//PACKAGE : MAP.
//CLASS : MAlArray : public GlobalArray<MAlgline>.
//DESCRIPTION : Array of objects 'MAlgline' - of algorithmic lines.
class  MAlArray : public GlobalArray<MAlgline>
{
public:

//PACKAGE : MAP.
//FUNCTION : MAlArray::CreateOfType();
//DESCRIPTION : Creates array - object of 'MAlArray' class.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MAlArray();};

//PACKAGE : MAP.
//FUNCTION : MPunktir::WhatTheArray();
//DESCRIPTION : Returns identifier of array of algorithmic lines 'MAlArray'.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_ALA;};
};

//PACKAGE : MAP.
//CLASS : MPrArray:public GlobalInheritantArray<MProsloika>.
//DESCRIPTION : Array of objects 'MProsloika'.
class  MPrArray:public GlobalInheritantArray<MProsloika>
{
public:

//PACKAGE : MAP.
//FUNCTION : MPrArray::WhatTheArray();
//DESCRIPTION : Creates an array - object of 'MPrArray' class.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_PRA;};

//PACKAGE : MAP.
//FUNCTION : MPrArray::CreateOfType();
//DESCRIPTION : Returns identifier of array of layers 'MPrArray'.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MPrArray();};
	
//PACKAGE : MAP.
//FUNCTION : MPrArray::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool ReadBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MPrArray::ReadBin(MFile* file,int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,int version);

//PACKAGE : MAP.
//FUNCTION : MPrArray::WriteBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Saves to UTP-file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,int version);
	
//PACKAGE : MAP.
//FUNCTION : MPrArray::SetSize(int Cnt);
//DESCRIPTION : Sets array size.
//INPUTS : int.
//RETURNS : NONE.
	void SetSize(int Cnt){
		MProsloika* pEl = new MLinia();
		ExpandSize(Cnt);
		for(int i=0;i<Cnt;i++)
			SetAt(i,pEl);

		delete pEl;
	}
};

//PACKAGE : MAP.
//FUNCTION : CreateProsloikaByType(int iType);
//DESCRIPTION : Creates layer by type.
//INPUTS : int.
//RETURNS : MProsloika pointer.
MProsloika* CreateProsloikaByType(int iType);

#endif //PROSLOIKI_H
