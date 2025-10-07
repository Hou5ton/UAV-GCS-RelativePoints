//PACKAGE : MAP.
//FILE: Klassifikator.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MKLASSIFIKATOR_H
#define MKLASSIFIKATOR_H
#include "Karta.h"
#include "Table.h"
#include "MString.h"
#include "SysDefine.h"

// Constants which used in tables

#if ENGVERSION == 1

	#define KLS_TABLE_KEY_STRING "Key"
	#define KLS_TABLE_NAME_STRING "Name"
	#define KLS_TABLE_COLOR_STRING "Color"
	#define KLS_TABLE_PRK_STRING "HandWrite"
	#define KLS_TABLE_IMAGE_STRING "Image"
	#define KLS_TABLE_ATTR_ED_IZM_STRING "Unit of Measure"
	#define KLS_TABLE_ATTR_TYPE_STRING "Type"
	#define KLS_TABLE_FONT_STRING "Font"
	#define KLS_TABLE_MEGSTR_STRING "Suffix & Preffix String Table"
	#define IDS_TABLE_ERROR1 "Colors Table is Full(Overfill). Classifier Attachment is Incorrect"
	#define IDS_TABLE_ERROR2 "Handwrites Table is Full(Overfill). Classifier Attachment is Incorrect"
	#define IDS_TABLE_ERROR3 "Suffix & Preffix Strings Table is Full(Overfill). Classifier Attachment is Incorrect"
	#define IDS_TABLE_ERROR4 "Descriptions Table is Full(Overfill). Classifier Attachment is Incorrect"


#else

	#define KLS_TABLE_KEY_STRING "Ключ"
	#define KLS_TABLE_NAME_STRING "Название"
	#define KLS_TABLE_COLOR_STRING "Цвет"
	#define KLS_TABLE_PRK_STRING "Почерк"
        #define KLS_TABLE_IMAGE_STRING "Изображение"
	#define KLS_TABLE_ATTR_ED_IZM_STRING "Единица измерения"
	#define KLS_TABLE_ATTR_TYPE_STRING "Тип"
	#define KLS_TABLE_FONT_STRING "Шрифт"
	#define KLS_TABLE_MEGSTR_STRING "Межевая строка"
	#define IDS_TABLE_ERROR1 "Таблица цветов переполена присоединение классификаторов некорректно"
	#define IDS_TABLE_ERROR2 "Таблица почерков переполена присоединение классификаторов некорректно"
	#define IDS_TABLE_ERROR3 "Таблица межевых строчек переполена присоединение классификаторов некорректно"
	#define IDS_TABLE_ERROR4 "Таблица характеристик переполена присоединение классификаторов некорректно"

#endif

//Field index "key" in classifier tables
#define KLS_TABLE_KEY 0

//Table of colors
#define KLS_TABLE_COLOR_NAME_INDEX	1 //Field number "Name"
#define KLS_TABLE_COLOR_VALUE_INDEX	2 //Field number 'value'

//Table of images
#define KLS_TABLE_IMAGE_NAME_INDEX	1 //Field number "Name"
#define KLS_TABLE_IMAGE_VALUE_INDEX	2 //Field number 'value'

//Table of handwrites
#define KLS_TABLE_PRK_NAME_INDEX	1 //Field number "Name"
#define KLS_TABLE_PRK_VALUE_INDEX	2 //Field number 'value'

//Table of attributes
#define KLS_TABLE_ATTR_NAME_INDEX	1 //Field number "Name"
#define KLS_TABLE_ATTR_ED_IZM_INDEX	2 //Measure unit
#define KLS_TABLE_ATTR_TYPE_INDEX	3 //Attribute type

//Table of fonts
#define KLS_TABLE_FONT_NAME_INDEX	1 //Field number "Name"
#define KLS_TABLE_FONT_VALUE_INDEX	2 //Field number 'value'

//Table of suffix&preffix strings
#define KLS_TABLE_MEGSTR_NAME_INDEX	 1 //Field number "Name"
#define KLS_TABLE_MEGSTR_VALUE_INDEX 2 //Field number 'value'

//PACKAGE : MAP.
//CLASS	: MKlassifikator.
//DESCRIPTION : Class describes classifier structure.
class MKlassifikator
{
private:
	MTermin m_termin;      // Term of section
	MZsArray* p_sistemi;   //  Pointer to sign systems array


public: 
	// Dictionaries
	MFixedSizeTable m_ColorDict; //of colors
    
	MTable m_ImageDict; // of images
    
	MTable m_PrkDict;		// of handwrites
	MTable m_AttrDict;		// of attributes
	MTable m_FontDict;		// of fonts
	MTable m_MegStrDict;	// of suffix&and preffix strings
	MTable m_EdIzmDict;		// of measuring units
        
	MString	m_masshtabkls;	// classifier scale
	MString	m_projectionkls;// clasifier projection
	MString	m_edizmkls;		// Measuring units of classifier
public:
	unsigned char	m_klscolorkod;	// index of background color of KLS in the table of colors
public:

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::MKlassifikator();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MKlassifikator();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::MKlassifikator(MTermin termin,MZsArray* zsa);
//DESCRIPTION : Constructor by established parameters.
//INPUTS : MTermin , MZsArray pointer.
//RETURNS : NONE.
	MKlassifikator(MTermin termin,MZsArray* zsa);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::MKlassifikator();
//DESCRIPTION : Constructor with defined standart.
//INPUTS : MKlassifikator pointer.
//RETURNS : NONE.
	MKlassifikator(MKlassifikator* kls);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::~MKlassifikator();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MKlassifikator();// {delete p_sistemi;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::Copy(MKlassifikator* kls);
//DESCRIPTION : Copies.
//INPUTS : MKlassifikator pointer.
//RETURNS : NONE.
	void Copy(MKlassifikator* kls);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::Clone();
//DESCRIPTION : Makes clone.
//INPUTS : NONE.
//RETURNS : MKlassifikator pointer.
	MKlassifikator* Clone();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetBytes();
//DESCRIPTION : Calculates size.
//INPUTS : NONE.
//RETURNS : unsigned int.
	unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ReadBin(char * filepath);
//DESCRIPTION : Reads from UTP-format file.
//INPUTS : char pointer.
//RETURNS : int.
	int ReadBin(char * filepath);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::WriteBin(char * filepath);
//DESCRIPTION : Saves to UTP-format file.
//INPUTS : char pointer.
//RETURNS : True.
	bool WriteBin(char * filepath);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::IsTerminEqualTo(MKlassifikator* kls);
//DESCRIPTION : Comparison of sign systems by terms.
//INPUTS : MKlassifikator pointer.
//RETURNS : True.
	bool IsTerminEqualTo(MKlassifikator* kls);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::IsTerminEqualTo(MTermin termin);
//DESCRIPTION : Term comparison.
//INPUTS : MTermin.
//RETURNS : True.
	bool IsTerminEqualTo(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetTermin();
//DESCRIPTION : Gets term.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* GetTermin();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateCopyTermin();
//DESCRIPTION : Get copy of term.
//INPUTS : NONE.
//RETURNS : MTermin pointer.
	MTermin* CreateCopyTermin();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetTermin(MTermin termin);
//DESCRIPTION : Sets term.
//INPUTS : MTermin.
//RETURNS : NONE.
	void SetTermin(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetZsArray();
//DESCRIPTION : Gets array of sign system.
//INPUTS : NONE.
//RETURNS : MZsArray pointer.
	MZsArray* GetZsArray();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateCopyZsArray();
//DESCRIPTION : Gets copy of sign systems.
//INPUTS : NONE.
//RETURNS : MZsArray pointer.
	MZsArray* CreateCopyZsArray();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetZsArray(MZsArray* zsa);
//DESCRIPTION : Sets array of sign system.
//INPUTS : MZsArray pointer.
//RETURNS : NONE.
	void SetZsArray(MZsArray* zsa);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AppendZsArrayHead(MZsArray* zsa);
//DESCRIPTION : Adds array of sign systems to the beginning.
//INPUTS : MZsArray pointer.
//RETURNS : NONE.
	void AppendZsArrayHead(MZsArray* zsa);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AppendZsArrayTail(MZsArray* zsa);
//DESCRIPTION : dds array of sign systems to the end.
//INPUTS : MZsArray pointer.
//RETURNS : NONE.
	void AppendZsArrayTail(MZsArray* zsa);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ClearZsArray();
//DESCRIPTION : Clears array of sign systems.
//INPUTS : NONE.
//RETURNS : NONE.
	void ClearZsArray();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetZnakSistema(MTermin termin);
//DESCRIPTION : Gets sign system with such term or NULL it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* GetZnakSistema(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetZnakSistema(MString kod);
//DESCRIPTION : Gets sign system with such code or NULL it does not exist.
//INPUTS : MString.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* GetZnakSistema(MString kod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateCopyZnakSistema(MTermin termin);
//DESCRIPTION : Gets copy of sign system with such term or NULL it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* CreateCopyZnakSistema(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateCopyZnakSistema(MString kod);
//DESCRIPTION : Gets copy of sign system with such code or NULL it does not exist.
//INPUTS : MString.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* CreateCopyZnakSistema(MString kod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::DeleteZnakSistema(MTermin termin);
//DESCRIPTION : Deletes sign system with such term.
//INPUTS : DeleteZnakSistema(MTermin termin).
//RETURNS : NONE.
	void DeleteZnakSistema(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddZnakSistema(MZnakSistema* ZnakSistema);
//DESCRIPTION : Adds such sign system.
//INPUTS : MZnakSistema pointer.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* AddZnakSistema(MZnakSistema* ZnakSistema);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateNewZnakSistema(MTermin termin);
//DESCRIPTION : Creates new sign system with such term.
//INPUTS : MTermin.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* CreateNewZnakSistema(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateNewZnakSistema(MString kod);
//DESCRIPTION : Creates new sign system with such code.
//INPUTS : MString.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* CreateNewZnakSistema(MString kod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateNewZnakSistema();
//DESCRIPTION : Creates new sign system by default.
//INPUTS : NONE.
//RETURNS : MZnakSistema pointer.
	MZnakSistema* CreateNewZnakSistema();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetZnak(MTermin termin);
//DESCRIPTION : Gets sign with such term or null if it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnak pointer.
	MZnak* GetZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetZnak(MString quadrakod,MString& TrueQuadra ,bool ignoreKKod = true);
//DESCRIPTION : Gets sign with such code or null if it does not exist.
//INPUTS : MString ,MString&  ,bool .
//RETURNS : MZnak pointer.
    MZnak* GetZnak(const MString &quadrakod, MString& TrueQuadra , bool ignoreKKod = true);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateCopyZnak(MTermin termin);
//DESCRIPTION : Gets copy of sign with such term or null if it does not exist.
//INPUTS : MTermin.
//RETURNS : MZnak pointer.
	MZnak* CreateCopyZnak(MTermin termin);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateCopyZnak(MString quadrakod);
//DESCRIPTION : Gets copy of sign with such code or null if it does not exist.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* CreateCopyZnak(MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::CreateNewZnak(MString quadrakod);
//DESCRIPTION : Creates new sign with such quadrocode.
//INPUTS : MString.
//RETURNS : MZnak pointer.
	MZnak* CreateNewZnak(MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetZnCount(MString strID = "");
//DESCRIPTION : Returns elements quantity of classifier.
//INPUTS : MString.
//RETURNS : int.
	int GetZnCount(MString strID = "");

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::DeleteZnak(MString quadrakod);
//DESCRIPTION : Deletes sign with such quadrocode.
//INPUTS : MString.
//RETURNS : NONE.
	void DeleteZnak(MString quadrakod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::DeleteRazdel(MString trikod);
//DESCRIPTION : Deletes part with such triplecode.
//INPUTS : MString.
//RETURNS : NONE.
	void DeleteRazdel(MString trikod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::DeleteZnakSistema(MString dukod);
//DESCRIPTION : Deletes part with such dualcode.
//INPUTS : MString.
//RETURNS : NONE.
	void DeleteZnakSistema(MString dukod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::RenameZnak(MString quadrakod, MString newkod);
//DESCRIPTION : Rename sign with such quadrocode.
//INPUTS : MString quadrakod, MString newkod.
//RETURNS : NONE.
	void RenameZnak(MString quadrakod, MString newkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::RenameRazdel(MString trikod, MString newkod);
//DESCRIPTION : Rename part with such triplecode.
//INPUTS : MString trikod, MString newkod.
//RETURNS : NONE.
	void RenameRazdel(MString trikod, MString newkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::RenameZnakSistema(MString dukod, MString newkod);
//DESCRIPTION : Rename part with such dualcode.
//INPUTS : MString dukod, MString newkod.
//RETURNS : NONE.
	void RenameZnakSistema(MString dukod, MString newkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::RenameKlassifikator(MString newkod);
//DESCRIPTION : Rename itself.
//INPUTS : MString.
//RETURNS : NONE.
	void RenameKlassifikator(MString newkod);

// Funcions of operation with tables

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictColorName(unsigned short int ushIndex,MString& strName);
//DESCRIPTION : Table of colors.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictColorName(unsigned short int ushIndex,MString& strName);
	
//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictColorValue(unsigned short int ushIndex,MColor& colorValue);
//DESCRIPTION : Table of colors.
//INPUTS : unsigned short int ,MColor& .
//RETURNS : True.
	bool GetDictColorValue(unsigned short int ushIndex,MColor& colorValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictColorValue(unsigned short int ushIndex,MColor colorValue);
//DESCRIPTION : Table of colors.
//INPUTS : unsigned short int ,MColor .
//RETURNS : True.
	bool SetDictColorValue(unsigned short int ushIndex,MColor colorValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictColorName(unsigned short int ushIndex,MString strName);
//DESCRIPTION : Table of colors.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictColorName(unsigned short int ushIndex,MString strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddDictColor(MColor clrrfValue, short& ushKey,MString strName="");
//DESCRIPTION : Table of colors.
//INPUTS : MColor , short& , MString .
//RETURNS : True.
	bool AddDictColor(MColor clrrfValue, short& ushKey,MString strName="");

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictPrkName(unsigned short int ushIndex,MString& strName);
//DESCRIPTION : Table of handwrites.
//INPUTS : unsigned short int , MString& .
//RETURNS : True.
	bool GetDictPrkName(unsigned short int ushIndex,MString& strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictPrkValue(unsigned short int ushIndex,MPrk& prkValue);
//DESCRIPTION : Table of handwrites.
//INPUTS : unsigned short int ,MPrk& .
//RETURNS : True.
	bool GetDictPrkValue(unsigned short int ushIndex,MPrk& prkValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictPrkValue(unsigned short int ushIndex,MPrk* prkValue);
//DESCRIPTION : Table of handwrites.
//INPUTS : unsigned short int ,MPrk pointer .
//RETURNS : True.
	bool SetDictPrkValue(unsigned short int ushIndex,MPrk* prkValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictPrkName(unsigned short int ushIndex,MString strName);
//DESCRIPTION : Table of handwrites.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictPrkName(unsigned short int ushIndex,MString strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddDictPrk(MPrk* prkValue, short& ushKey,MString strName="");
//DESCRIPTION : Table of handwrites.
//INPUTS : MPrk pointer, short& ,MString .
//RETURNS : True.
	bool AddDictPrk(MPrk* prkValue, short& ushKey,MString strName="");

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictAttrName(unsigned short int ushIndex,MString& strName);
//DESCRIPTION : Table of notions.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictAttrName(unsigned short int ushIndex,MString& strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictAttrEdIzm(unsigned short int ushIndex,MString& edIzm);
//DESCRIPTION : Table of notions.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictAttrEdIzm(unsigned short int ushIndex,MString& edIzm);
	
//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictAttrType(unsigned short int ushIndex,MString& type);
//DESCRIPTION : Table of notions.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictAttrType(unsigned short int ushIndex,MString& type);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictAttrName(unsigned short int ushIndex,MString strName);
//DESCRIPTION : Table of notions.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictAttrName(unsigned short int ushIndex,MString strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictAttrEdIzm(unsigned short int ushIndex,MString edIzm);
//DESCRIPTION : Table of notions.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictAttrEdIzm(unsigned short int ushIndex,MString edIzm);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictAttrType(unsigned short int ushIndex,MString type);
//DESCRIPTION : Table of notions.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictAttrType(unsigned short int ushIndex,MString type);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddDictAttr(MString strName, short& ushKey,MString strType = "",MString strEdIzm = "");
//DESCRIPTION : Table of notions.
//INPUTS : MString , short& ,MString ,MString .
//RETURNS : True.
	bool AddDictAttr(MString strName, short& ushKey,MString strType = "",MString strEdIzm = "");

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictImageName(unsigned short int ushIndex,MString& strName);
//DESCRIPTION : Table of images.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictImageName(unsigned short int ushIndex,MString& strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictImageValue(unsigned short int ushIndex,MImage &image);
//DESCRIPTION : Table of images.
//INPUTS : unsigned short int ,MImage &.
//RETURNS : True.
	bool GetDictImageValue(unsigned short int ushIndex,MImage &image);
	
//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictImageName(unsigned short int ushIndex,MString strName);
//DESCRIPTION : Table of images.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictImageName(unsigned short int ushIndex,MString strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictImageValue(unsigned short int ushIndex,MImage *image);
//DESCRIPTION : Table of images.
//INPUTS : unsigned short int ,MImage pointer.
//RETURNS : True.
	bool SetDictImageValue(unsigned short int ushIndex,MImage *image);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddDictImage(MString strName, short& ushKey, MImage *image);
//DESCRIPTION : Table of images.
//INPUTS : MString , short& , MImage pointer.
//RETURNS : True.
	bool AddDictImage(MString strName, short& ushKey, MImage *image);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictFontName(unsigned short int ushIndex,MString& strName);
//DESCRIPTION :  Table of fonts.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictFontName(unsigned short int ushIndex,MString& strName);
	
//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictFontValue(unsigned short int ushIndex,M_LOG_FONT& colorValue);
//DESCRIPTION :  Table of fonts.
//INPUTS : unsigned short int , M_LOG_FONT& .
//RETURNS : True.
	bool GetDictFontValue(unsigned short int ushIndex,M_LOG_FONT& colorValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictFontValue(unsigned short int ushIndex,M_LOG_FONT lfValue);
//DESCRIPTION :  Table of fonts.
//INPUTS : unsigned short int ,M_LOG_FONT .
//RETURNS : True.
	bool SetDictFontValue(unsigned short int ushIndex,M_LOG_FONT lfValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictFontName(unsigned short int ushIndex,MString strName);
//DESCRIPTION :  Table of fonts.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictFontName(unsigned short int ushIndex,MString strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddDictFont(M_LOG_FONT lfValue, short& ushKey,MString strName="");
//DESCRIPTION :  Table of fonts.
//INPUTS : M_LOG_FONT , short& ,MString .
//RETURNS : True.
	bool AddDictFont(M_LOG_FONT lfValue, short& ushKey,MString strName="");

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictMegstrName(unsigned short int ushIndex,MString& strName);
//DESCRIPTION :  Suffix&preffix strings table.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictMegstrName(unsigned short int ushIndex,MString& strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetDictMegstrValue(unsigned short int ushIndex,MString& colorValue);
//DESCRIPTION :  Suffix&preffix strings table.
//INPUTS : unsigned short int ,MString& .
//RETURNS : True.
	bool GetDictMegstrValue(unsigned short int ushIndex,MString& colorValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictMegstrValue(unsigned short int ushIndex,MString megstrValue);
//DESCRIPTION :  Suffix&preffix strings table.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictMegstrValue(unsigned short int ushIndex,MString megstrValue);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetDictMegstrName(unsigned short int ushIndex,MString strName);
//DESCRIPTION :  Suffix&preffix strings table.
//INPUTS : unsigned short int ,MString .
//RETURNS : True.
	bool SetDictMegstrName(unsigned short int ushIndex,MString strName);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::AddDictMegstr(MString megstrValue, short& ushKey,MString strName="");
//DESCRIPTION :  Suffix&preffix strings table.
//INPUTS : MString , short& ,MString .
//RETURNS : True.
	bool AddDictMegstr(MString megstrValue, short& ushKey,MString strName="");

// Folowing functions must be deleted:
	// But paradox! Suddenly they became very useful!

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetColorDictNumValue(MColor clrValue,unsigned short int& ExistKey);
//DESCRIPTION :  Gets key by inscription name.
//INPUTS : MColor ,unsigned short int& .
//RETURNS : True.
	bool GetColorDictNumValue(MColor clrValue,unsigned short int& ExistKey);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator:: GetPrkDictNumValue(MPrk *clrValue,unsigned short int& ExistKey);
//DESCRIPTION :  Gets key by inscription name.
//INPUTS : MPrk pointer,unsigned short int& .
//RETURNS : True.
	bool GetPrkDictNumValue(MPrk *clrValue,unsigned short int& ExistKey);
	
//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetPrkDictNumValue(MString clrValue,unsigned short int& ExistKey);
//DESCRIPTION :  Gets key by inscription name.
//INPUTS : MString ,unsigned short int& .
//RETURNS : True.
	bool GetPrkDictNumValue(MString clrValue,unsigned short int& ExistKey);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetAttrDictNumValue(MString clrValue,unsigned short int& ExistKey);
//DESCRIPTION :  Gets key by inscription name.
//INPUTS : MString ,unsigned short int& .
//RETURNS : True.
	bool GetAttrDictNumValue(MString clrValue,unsigned short int& ExistKey);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetFontDictNumValue(M_LOG_FONT clrValue,unsigned short int& ExistKey);
//DESCRIPTION :  Gets key by inscription name.
//INPUTS : M_LOG_FONT ,unsigned short int& .
//RETURNS : True.
	bool GetFontDictNumValue(M_LOG_FONT clrValue,unsigned short int& ExistKey);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetMegStrDictNumValue(MString clrValue,unsigned short int& ExistKey);
//DESCRIPTION :  Gets key by inscription name.
//INPUTS : MString ,unsigned short int& .
//RETURNS : True.
	bool GetMegStrDictNumValue(MString clrValue,unsigned short int& ExistKey);

// Functions which provide access to the tables

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetColorTable();
//DESCRIPTION : Table of colors.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetColorTable(){return &m_ColorDict;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetPrkTable();
//DESCRIPTION : Table of handwrites.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetPrkTable(){return &m_PrkDict;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetAttrTable();
//DESCRIPTION : Table of attributes.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetAttrTable(){return &m_AttrDict;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetImgTable();
//DESCRIPTION : Table of images.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetImgTable(){return &m_ImageDict;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetFontTable();
//DESCRIPTION : Table of fonts.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetFontTable(){return &m_FontDict;};		

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetMegStrTable();
//DESCRIPTION : Suffix&preffix string table.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetMegStrTable(){return &m_MegStrDict;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetEdIzmTable();
//DESCRIPTION : Table of measuring units.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	MTable* GetEdIzmTable(){return &m_EdIzmDict;};

// FUNCTIONS OF HANDWRITE COLOR REPLACEMENT OF
// SUFFIX&PREFFIX STRING AND CHARACTERISTIC IN CLASSIFIER OBJECTS

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangeColor(unsigned char oldcolorkod,unsigned char newcolorkod);
//DESCRIPTION : Handwrite color.
//INPUTS : unsigned char ,unsigned char .
//RETURNS : True.
	bool ChangeColor(unsigned char oldcolorkod,unsigned char newcolorkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangePrkColorInPrkDict(unsigned char oldcolorkod,unsigned char newcolorkod);
//DESCRIPTION : Handwrite color replacement in handwrite dictionary.
//INPUTS : unsigned char ,unsigned char .
//RETURNS : True.
	bool ChangePrkColorInPrkDict(unsigned char oldcolorkod,unsigned char newcolorkod);
	
//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangePocherk(unsigned char oldprkkod,unsigned char newprkkod);
//DESCRIPTION : Change pocherk.
//INPUTS : unsigned char ,unsigned char .
//RETURNS : True.
	bool ChangePocherk(unsigned char oldprkkod,unsigned char newprkkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangeMegaStroka(unsigned char oldmegastrkod, unsigned char newmegastrkod);
//DESCRIPTION : Change MegaStroka.
//INPUTS : unsigned char , unsigned char .
//RETURNS : True.
	bool ChangeMegaStroka(unsigned char oldmegastrkod, unsigned char newmegastrkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangeCharakteristika(unsigned short oldcharkod, unsigned short newcharkod);
//DESCRIPTION : Change Charakteristic.
//INPUTS : unsigned short , unsigned short .
//RETURNS : True.
	bool ChangeCharakteristika(unsigned short oldcharkod, unsigned short newcharkod);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangeFont(unsigned short , unsigned short );
//DESCRIPTION : If we will work with next arrays, thus we need to add these functions:
//  			Replaces font
// 				parameters (unsigned short oldcharkod, unsigned short newcharkod).
//INPUTS : unsigned short , unsigned short .
//RETURNS : False.
	bool ChangeFont(unsigned short , unsigned short ){return false;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::ChangeEdIzm(unsigned short , unsigned short );
//DESCRIPTION : Replaces measuring unit
// 		parameters (unsigned short oldcharkod, unsigned short newcharkod).
//INPUTS : unsigned short , unsigned short.
//RETURNS : False.
	bool ChangeEdIzm(unsigned short , unsigned short ){return false;};

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::DotShriftZnakCenterAlign();
//DESCRIPTION : Aligns all dotted font signs by centre.
//INPUTS : NONE.
//RETURNS : True.
	bool DotShriftZnakCenterAlign();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetUnrealPrkKods(MShInArray* prkkodar);
//DESCRIPTION : Searchs for the codes of handwrites which don't exist in handwrite dictionary by all objects of classifier.
//INPUTS : MShInArray pointer.
//RETURNS : True.
	bool GetUnrealPrkKods(MShInArray* prkkodar);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetUnrealMegstrKods(MShInArray* megstrkodar);
//DESCRIPTION : Searchs for the codes of suffix &preffix strings which don't exist in suffix &preffix strings dictionary by all objects of classifier.
//INPUTS : MShInArray pointer.
//RETURNS : True.
	bool GetUnrealMegstrKods(MShInArray* megstrkodar);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::GetUnrealAttrKods(MShInArray* attrkodar);
//DESCRIPTION : Searchs for the codes of characteristics which don't exist in characteristics dictionary by all objects of classifier.
//INPUTS : MShInArray pointer.
//RETURNS : True.
	bool GetUnrealAttrKods(MShInArray* attrkodar);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::Append(MKlassifikator* pKls);
//DESCRIPTION : Classifier attachment.
//INPUTS : MKlassifikator pointer.
//RETURNS : True.
	bool Append(MKlassifikator* pKls);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::MinimizeTable(TableToChangeKod WhatTheTable);
//DESCRIPTION : Table minimization.
//INPUTS : TableToChangeKod.
//RETURNS : NONE.
	void MinimizeTable(TableToChangeKod WhatTheTable);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetVisibleFilter(MString filename);
//DESCRIPTION : Sets filter of signs visibility.
//INPUTS : MString.
//RETURNS : NONE.
	void SetVisibleFilter(MString filename);

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetAllZnakUnvisible();
//DESCRIPTION : Sets all signs to invisible.
//INPUTS : NONE.
//RETURNS : NONE.
	void SetAllZnakUnvisible();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::SetAllZnakVisible();
//DESCRIPTION : Sets all signs to visible.
//INPUTS : NONE.
//RETURNS : NONE.
	void SetAllZnakVisible();

//PACKAGE : MAP.
//FUNCTION : MKlassifikator::RotateAllZsSystemCoeffs();
//DESCRIPTION : recalculate all m_fMasstKoeff.
//INPUTS : NONE.
//RETURNS : NONE.
    void RotateAllZsSystemCoeffs();
};

#endif //MKLASSIFIKATOR_H
