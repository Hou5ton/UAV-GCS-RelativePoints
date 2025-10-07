//PACKAGE : MAP.
//FILE: Table.h   
//AUTHOR:
//DESCRIPTION:

#ifndef TEMPLARRAY_H
#define TEMPLARRAY_H
#include "Array.h"
#include "MString.h"

#define TABLE_SERVINFO_ROWS_NUM 3
#define TABLE_SERVINFO_TYPE_ROW 0
#define TABLE_SERVINFO_NAME_ROW 1
#define TABLE_SERVINFO_KEY_ROW 2

#if ENGVERSION == 1  // English version

#define IDS_UNKNWN_MASSIVE "Unknown For MTable Massive Type"
#define IDS_BAD_KLS_CONCT "INCORRECT CLASSIFIER ATTACHMENT (Code Error)"

#define IDS_BLACK "Black 2558-01"
#define IDS_WHITE "White 2558-01"
#define IDS_BROWN "Brown 2558-62"
#define IDS_DARK_BLUE "Blue 2558-38"
#define IDS_ORANGE "Orange(Blend) 2558-10"
#define IDS_GREEN "Green(Blend) 2558-45"
#define IDS_BROWN_NEW "Brown 2558-61"
#define IDS_DARK_BLUE_NEW "Blue 2558-39"
#define IDS_TURQUOISE "Turquoise(Blend) 2558-37"
#define IDS_WEAK_ORANGE "Orange(Blend) 2558-11"
#define IDS_PURPLE "Purple 2558-74"
#define IDS_DARK_OLIVE "Olive(Mixed) 2558-37,-39,-10"
#define IDS_TAUPE_1 "Taupe 1st(Mixed) 2558-62,-38"
#define IDS_TAUPE_2 "Taupe 2nd(Mixed) 2558-20,-01,-30"
#define IDS_BLEND_ORANGE "Orange(Mixed) 2558-12,-10"
#define IDS_BLAK_15pc "Black(15%,34) 2558-01"
#define IDS_DARK_BLUE_30pc "Blue(30%,48) 2558-38"
#define IDS_WEAK_ORANGE_30pc "Orange(Blend)(30%,48) 2558-10"
#define IDS_WEAK_GREEN_50pc "Green(Blend)(50%,34) 2558-45"
#define IDS_WEAK_ORANGE_50pc "Orange(Blend)(50%,34) 2558-10"
#define IDS_BROWN_50pc "Brown(50%,34) 2558-61"
#define IDS_BLACK_30pc "Black(30%,48) 2558-01"
#define IDS_BROWN_30pc "Brown(30%,48) 2558-62"
#define IDS_PURPLE_50pc "Purple(50%,34) 2558-74"
#define IDS_BLEND_ORANGE_24 "Orange(Mixed)(24) 2558-12,-10"
#define IDS_ORANGE_24 "Orange(Mixed)(24) 2558-10"
#define IDS_WEAK_ORANGE_12 "Orange(Blend) 2558-12"


#else	// russian version
#define IDS_UNKNWN_MASSIVE "Неизвестный для MTable тип массива"
#define IDS_BAD_KLS_CONCT "Некорректное присоединение классификатора (неверный код)"

#define IDS_BLACK "Черная 2558-01"
#define IDS_WHITE "Белая 2558-01"
#define IDS_BROWN "Коричневая 2558-62"
#define IDS_DARK_BLUE "Синяя 2558-38"
#define IDS_ORANGE "Оранжевая(расслабленная) 2558-10"
#define IDS_GREEN "Зеленая(расслабленная) 2558-45"
#define IDS_BROWN_NEW "Коричневая 2558-61"
#define IDS_DARK_BLUE_NEW "Синяя 2558-39"
#define IDS_TURQUOISE "Бирюзовая(расслабленная) 2558-37"
#define IDS_WEAK_ORANGE "Оранжевая(расслабленная) 2558-11"
#define IDS_PURPLE "Фиолетовая 2558-74"
#define IDS_DARK_OLIVE "Темно-оливковая(смесевая) 2558-37,-39,-10"
#define IDS_TAUPE_1 "Серо-коричневая 1-я(смесевая) 2558-62,-38"
#define IDS_TAUPE_2 "Серо-коричневая 2-я(смесевая) 2558-20,-01,-30"
#define IDS_BLEND_ORANGE "Оранжевая(смесевая) 2558-12,-10"
#define IDS_BLAK_15pc "Черная(15%,34лин/см) 2558-01"
#define IDS_DARK_BLUE_30pc "Синяя(30%,48лин/см) 2558-38"
#define IDS_WEAK_ORANGE_30pc "Оранжевая(расслабленная)(30%,48лин/см) 2558-10"
#define IDS_WEAK_GREEN_50pc "Зеленая(расслабленная)(50%,34лин/см) 2558-45"
#define IDS_WEAK_ORANGE_50pc "Оранжевая(расслабленная)(50%,34лин/см) 2558-10"
#define IDS_BROWN_50pc "Коричневая(50%,34лин/см) 2558-61"
#define IDS_BLACK_30pc "Черная(30%,48лин/см) 2558-01"
#define IDS_BROWN_30pc "Коричневая(30%,48лин/см) 2558-62"
#define IDS_PURPLE_50pc "Фиолетовая(50%,34лин/см) 2558-74"
#define IDS_BLEND_ORANGE_24 "Оранжевая(смесевая)(24лин/см) 2558-12,-10"
#define IDS_ORANGE_24 "Оранжевая(смесевая)(24лин/см) 2558-10"
#define IDS_WEAK_ORANGE_12 "Оранжевая(расслабленная) 2558-12"

#endif

class MKlassifikator;

//PACKAGE : MAP.
//CLASS : MTable.
//DESCRIPTION : Class tables used for realization of databases creation. Also used for realization of classifier tables.
class MTable
{
public:
	//  The table
	MArArray m_Table;

	//  Table of additional information
	MArArray m_ServInfo;

	//  Max tsable size
	unsigned int m_iMaxSize;

//PACKAGE : MAP.
//FUNCTION : MTable::GetIndexByKey(int iKey);
//DESCRIPTION : Service functions  accessible only to this class.
//INPUTS : int.
//RETURNS : int.
	virtual int GetIndexByKey(int iKey);

//PACKAGE : MAP.
//FUNCTION : MTable::SortByKey();
//DESCRIPTION : Sorts by key.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void SortByKey();

//PACKAGE : MAP.
//FUNCTION : MTable::GetFreeKey();
//DESCRIPTION : Gets free key.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetFreeKey();

//PACKAGE : MAP.
//FUNCTION : MTable::SetKeyCell(int iRecKey,int iField, void* pElement);
//DESCRIPTION : Function sets key field for a cell.
//INPUTS : int ,int , void pointer.
//RETURNS : True.
	virtual bool SetKeyCell(int iRecKey,int iField, void* pElement);

//PACKAGE : MAP.
//FUNCTION : MTable::CompareWith(int iRow,int iCol,void* pElToCompare);
//DESCRIPTION : Function of comparison of element with pointer to the same element by indexes
// 				Function is not safety.
//INPUTS : int ,int ,void pointer.
//RETURNS : True.
	virtual bool CompareWith(int iRow,int iCol,void* pElToCompare);

public:

//PACKAGE : MAP.
//FUNCTION : MTable::IsFieldSimple(unsigned int iFieldIndex);
//DESCRIPTION : Checks if pole simple or not.
//INPUTS : unsigned int.
//RETURNS : True.
	virtual bool IsFieldSimple(unsigned int iFieldIndex);

//PACKAGE : MAP.
//FUNCTION : MTable::MTable();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MTable();

//PACKAGE : MAP.
//FUNCTION : MTable::MTable(int iMaxSize);
//DESCRIPTION : Constructor.
//INPUTS : int.
//RETURNS : NONE.
	MTable(int iMaxSize);

//PACKAGE : MAP.
//FUNCTION : MTable::~MTable();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MTable(){m_Table.FreeMemory();
				m_ServInfo.FreeMemory();};

//PACKAGE : MAP.
//FUNCTION : MTable::ReadBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Reads from file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	// Reads from file
	virtual bool ReadBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MTable::ReadBin(MFile* file,int version);
//DESCRIPTION : Reads from file.
//INPUTS : MFile pointer,int .
//RETURNS : True.
	virtual bool ReadBin(MFile* file,int version);

//PACKAGE : MAP.
//FUNCTION : MTable::WriteBin(FILE* h_nuxfile,int version);
//DESCRIPTION : Saves to file.
//INPUTS : FILE pointer,int .
//RETURNS : True.
	virtual bool WriteBin(FILE* h_nuxfile,int version);

//PACKAGE : MAP.
//FUNCTION : MTable::AddField(MString strFieldType,MString strName = "",int iIndex = -1);
//DESCRIPTION : Adds field to the table.
//INPUTS : MString ,MString ,int .
//RETURNS : int.
	virtual int AddField(MString strFieldType,MString strName = "",int iIndex = -1);

//PACKAGE : MAP.
//FUNCTION : MTable::GetFieldType(int iField);
//DESCRIPTION : Gets field type.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetFieldType(int iField);

//PACKAGE : MAP.
//FUNCTION : MTable::MString GetFieldName(int iField);
//DESCRIPTION : Defines functions of field name reading by number.
//INPUTS : int.
//RETURNS : MString.
	virtual MString GetFieldName(int iField)
			{return *((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_NAME_ROW))->GetAt(iField);};

//PACKAGE : MAP.
//FUNCTION : MTable::DeleteField(int iField);
//DESCRIPTION : Deletes field.
//INPUTS : int.
//RETURNS : True.
	virtual bool DeleteField(int iField);

//PACKAGE : MAP.
//FUNCTION : MTable::DeleteRecord(int iRecKey);
//DESCRIPTION : Operations with records in a table.
//INPUTS : int.
//RETURNS : NONE.
	virtual void DeleteRecord(int iRecKey);

//PACKAGE : MAP.
//FUNCTION : MTable::CreateRecord();
//DESCRIPTION : Creates record.
//INPUTS : NONE.
//RETURNS : int.
	virtual int CreateRecord();

//PACKAGE : MAP.
//FUNCTION : MTable::FindCell(int iField, void* pElement);
//DESCRIPTION : Operations with cells in a table.
//INPUTS : int , void pointer.
//RETURNS : int.
	virtual int FindCell(int iField, void* pElement);

//PACKAGE : MAP.
//FUNCTION : MTable::GetCell(int iRecKey,int iField);
//DESCRIPTION : Gets cell.
//INPUTS : int ,int .
//RETURNS : void pointer.
	virtual void* GetCell(int iRecKey,int iField);

//PACKAGE : MAP.
//FUNCTION : MTable::SetCell(int iRecKey,int iField, void* pElement);
//DESCRIPTION : Sets cell.
//INPUTS : int ,int , void pointer.
//RETURNS : True.
	virtual bool SetCell(int iRecKey,int iField, void* pElement);

	// Additional operations

//PACKAGE : MAP.
//FUNCTION : MTable::Clone();
//DESCRIPTION : Makes a clone.
//INPUTS : NONE.
//RETURNS : MTable pointer.
	virtual MTable* Clone();

//PACKAGE : MAP.
//FUNCTION : MTable::Copy(MTable* pToCopy);
//DESCRIPTION : Copying.
//INPUTS : MTable pointer.
//RETURNS : NONE.
	virtual void Copy(MTable* pToCopy);

//PACKAGE : MAP.
//FUNCTION : MTable::GetBytes();
//DESCRIPTION : Gets length.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MTable::GetKeyField();
//DESCRIPTION : Gets number of key element.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetKeyField();

//PACKAGE : MAP.
//FUNCTION : MTable::SetKeyField(int iField);
//DESCRIPTION : Sets key field.
//INPUTS : int.
//RETURNS : True.
	virtual bool SetKeyField(int iField);

//PACKAGE : MAP.
//FUNCTION : MTable::SetMaxSize(int iMaxSize);
//DESCRIPTION : Sets max size.
//INPUTS : int.
//RETURNS : NONE.
	virtual void SetMaxSize(int iMaxSize);

//PACKAGE : MAP.
//FUNCTION : MTable::GetMaxSize();
//DESCRIPTION : Gets max size.
//INPUTS : NONE.
//RETURNS : int.
	virtual int GetMaxSize(){return m_iMaxSize;};

//PACKAGE : MAP.
//FUNCTION : MTable::GetDimension();
//DESCRIPTION : Gets table dimension.
//INPUTS : NONE.
//RETURNS : MIntSize.
	virtual MIntSize GetDimension(){
						int iColNum = m_Table.GetSize();
						if(!iColNum)
							return MIntSize(0,0);
						return MIntSize(m_Table.GetAt(0)->GetSize(),iColNum);
					};

//PACKAGE : MAP.
//FUNCTION : MTable::ClearTable();
//DESCRIPTION : Deletes all data from the table.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual void ClearTable();

//PACKAGE : MAP.
//FUNCTION : MTable::FindInTable(int CmpField,MString CmpElement,int RetField,MString &RetElement);
//DESCRIPTION : Finds a record by inserted avalue of defined field.
//INPUTS : int ,MString ,int ,MString &.
//RETURNS : True.
	virtual bool FindInTable(int CmpField,MString CmpElement,int RetField,MString &RetElement);

//PACKAGE : MAP.
//FUNCTION : MTable::FindInTableCMP(int CmpField,MString Cmp1part,MString Cmp2part,int RetField,MString &RetElement);
//DESCRIPTION :  Finds a record by inserted avalue of defined field(specifically for import from SXF).
//INPUTS : int ,MString ,MString ,int ,MString &.
//RETURNS : True.
	virtual bool FindInTableCMP(int CmpField,MString Cmp1part,MString Cmp2part,int RetField,MString &RetElement);

//PACKAGE : MAP.
//FUNCTION : MTable::FindInTableExt(int CmpField1,MString CmpElement1,int CmpField2,MString CmpElement2,int RetField,MString &RetElement);
//DESCRIPTION : Finds a record by inserted values of two key fields.
//INPUTS : int ,MString ,int ,MString ,int ,MString &.
//RETURNS : True.
	virtual bool FindInTableExt(int CmpField1,MString CmpElement1,int CmpField2,MString CmpElement2,int RetField,MString &RetElement);

//PACKAGE : MAP.
//FUNCTION : MTable::FindInTableExt2(int CmpField,MString CmpElement,int RetField1,MString &RetElement1,int RetField2,MString &RetElement2);
//DESCRIPTION : Finds a record.
//INPUTS : int ,MString ,int ,MString &,int ,MString &.
//RETURNS : True.
	virtual bool FindInTableExt2(int CmpField,MString CmpElement,int RetField1,MString &RetElement1,int RetField2,MString &RetElement2);

//PACKAGE : MAP.
//FUNCTION : MTable::IsSimple();
//DESCRIPTION : Functions used for edition of simple tables
// 				Simple tables it is those which all elements may be transformed
// 				into lines and back.
//INPUTS : NONE.
//RETURNS : True.
	virtual bool     IsSimple();

//PACKAGE : MAP.
//FUNCTION : MTable:: GetSimpleCell(int iRecKey,int iField,MString &strLoadString);
//DESCRIPTION : Gets simple cell.
//INPUTS : int ,int ,MString &.
//RETURNS : True.
	virtual bool	 GetSimpleCell(int iRecKey,int iField,MString &strLoadString);

//PACKAGE : MAP.
//FUNCTION : MTable::SetSimpleCell(int iRecKey,int iField,MString* pElemene);
//DESCRIPTION : Sets simple cell.
//INPUTS : int ,int ,MString pointer.
//RETURNS : True.
	virtual bool	 SetSimpleCell(int iRecKey,int iField,MString* pElemene);

//PACKAGE : MAP.
//FUNCTION : MTable::GetFirstKey(int &iKey);
//DESCRIPTION : Enhancements table by funtions of sequential access of the records.
//INPUTS : int.
//RETURNS : True.
	virtual bool GetFirstKey(int &iKey);

//PACKAGE : MAP.
//FUNCTION : MTable::GetNextKey(int iPrevKey,int &iKey);
//DESCRIPTION : Gets next key.
//INPUTS : int ,int &.
//RETURNS : True.
	virtual bool GetNextKey(int iPrevKey,int &iKey);

//PACKAGE : MAP.
//FUNCTION : MTable::UseRecord(MVoidArray* pva,int iFieldOutOfCompare = -1);
//DESCRIPTION : Gets record index with the same values
// 				Necessarily to use with the similar tables.
//INPUTS : MVoidArray pointer,int .
//RETURNS : int.
	int UseRecord(MVoidArray* pva,int iFieldOutOfCompare = -1);

//PACKAGE : MAP.
//FUNCTION : MTable::AddOrGetUsed(MVoidArray* pva);
//DESCRIPTION : Adds or gets used.
//INPUTS : MVoidArray pointer.
//RETURNS : int.
	virtual int AddOrGetUsed(MVoidArray* pva);

//PACKAGE : MAP.
//FUNCTION : MTable::IsFixed();
//DESCRIPTION : Is table size fixed?.
//INPUTS : NONE.
//RETURNS : False.
	virtual bool IsFixed(){return false;};
};

//PACKAGE : MAP.
//CLASS : MFixedSizeTable : public MTable.
//DESCRIPTION : Class table for realization of the fast table of the fixed size of the table of colors
//		ATTENTION LAST RECORD IN THE TABLE DESIGNATES THE EMPTY ELEMENT
//		ALL ELEMENTS FROM THE END WHICH HAVE SAME VALUE ARE EMPTY

class MFixedSizeTable : public MTable
{
public:

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::InitalizeAsColorDefault(MTable* pTable,int iColorValueIndex,int iColorNameIndex);
//DESCRIPTION : Functions of initialization of table of colors(in values by default).
//INPUTS : MTable pointer,int ,int .
//RETURNS : NONE.
	static void InitalizeAsColorDefault(MTable* pTable,int iColorValueIndex,int iColorNameIndex);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::InitalizeAsCMYKDefault(MTable* pTable,int iColorValueIndex,int iColorNameIndex);
//DESCRIPTION : Functions of initialization of table of CMYK(in values by default).
//INPUTS : MTable pointer,int ,int .
//RETURNS : NONE.
	static void InitalizeAsCMYKDefault(MTable* pTable,int iColorValueIndex,int iColorNameIndex);

// Service functions  accessible only to this class

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetIndexByKey(int iKey);
//DESCRIPTION : Searchs for the record by key.
//INPUTS : int.
//RETURNS : int.
	int GetIndexByKey(int iKey){return iKey;};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::SortByKey();
//DESCRIPTION : Sorts table by increasement by key.
//INPUTS : NONE.
//RETURNS : NONE.
	void SortByKey(){};

	//	int GetFreeKey(){return m_iMaxSize;};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::SetKeyCell(int iRecKey,int , void* pElement);
//DESCRIPTION : Constructor.
//INPUTS : int ,int , void pointer.
//RETURNS : True.
	bool SetKeyCell(int iRecKey,int , void* pElement){
		if(*((int*)pElement)!=iRecKey)
			return false;
		return true;
	};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::MFixedSizeTable();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MFixedSizeTable();

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::MFixedSizeTable(int iMaxSize);
//DESCRIPTION : Constructor.
//INPUTS : int.
//RETURNS : NONE.
	MFixedSizeTable(int iMaxSize);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::~MFixedSizeTable();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MFixedSizeTable(){m_Table.FreeMemory();
						m_ServInfo.FreeMemory();};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::DeleteRecord(int iRecKey);
//DESCRIPTION : Operations with records in a table.
//INPUTS : int.
//RETURNS : NONE.
	virtual void DeleteRecord(int iRecKey);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::CreateRecord();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : int.
	virtual int CreateRecord();

	// Operations with cells in a table
	// 	virtual int FindCell(int iField, void* pElement){exit(15);return false;};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetCell(int iRecKey,int iField);
//DESCRIPTION : Gets a cell.
//INPUTS : int ,int .
//RETURNS : void pointer.
	virtual void* GetCell(int iRecKey,int iField){
	return m_Table.GetAt(iField)->GetAtVoid(iRecKey);};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::SetCell(int iRecKey,int iField, void* pElement);
//DESCRIPTION : Sets a cell.
//INPUTS : int ,int , void pointer.
//RETURNS : False.
	virtual bool SetCell(int iRecKey,int iField, void* pElement){
		if(iField!=GetKeyField()){
			m_Table.GetAt(iField)->SetAtVoid(iRecKey,pElement);
			return true;
		}
		return false;
	};

	// Additional work

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::SetKeyField(int iField);
//DESCRIPTION : Gets number of key element.
//INPUTS : int.
//RETURNS : True.
	bool SetKeyField(int iField);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::SetMaxSize(int iMaxSize);
//DESCRIPTION : Sets max size for a table.
//INPUTS : int.
//RETURNS : NONE.
	void SetMaxSize(int iMaxSize);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetMaxSize();
//DESCRIPTION : Sets max size for a table.
//INPUTS : NONE.
//RETURNS : int.
	int GetMaxSize(){return m_iMaxSize;};

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetDimension();
//DESCRIPTION : Gets table dimension.
//INPUTS : NONE.
//RETURNS : MIntSize.
	MIntSize GetDimension();

	// Functions used for edition of simple tables
	// Simple tables it is those which all elements may be transformed
	// into lines and back

	// Functions of simple table edition

// Enhancements table by funtions of sequential access of the records

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetFirstKey(int &iKey);
//DESCRIPTION : Gets first key.
//INPUTS : int.
//RETURNS : True.
	bool GetFirstKey(int &iKey);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetNextKey(int iPrevKey,int &iKey);
//DESCRIPTION : Gets next key.
//INPUTS : int ,int &.
//RETURNS : True.
	bool GetNextKey(int iPrevKey,int &iKey);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::AddOrGetUsed(MVoidArray* pva);
//DESCRIPTION : Adds or gets used.
//INPUTS : MVoidArray .
//RETURNS : int.
	virtual int AddOrGetUsed(MVoidArray* pva);

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::GetFilledNum();
//DESCRIPTION : Returns completed quantity of elements.
//INPUTS : NONE.
//RETURNS : int.
	int GetFilledNum();

//PACKAGE : MAP.
//FUNCTION : MFixedSizeTable::IsFixed();
//DESCRIPTION : Is table size fixed?
//INPUTS : NONE.
//RETURNS : True.
	virtual bool IsFixed(){return true;};
};

enum TableToChangeKod{ID_COLOR_TABLE,ID_ATTR_TABLE,ID_MEGSTR_TABLE,ID_PRK_TABLE,
				ID_FONT_TABLE,ID_EDIZM_TABLE};

//PACKAGE : MAP.
//CLASS : MCodeChange.
//DESCRIPTION : Class - an array of Indexes on arrays.
//			 	 ATTENTION DURING THE SPELLING OF CLASIIFIER ATTACHMENT TO EXISTING
//				 THERE WAS A FOLLOWING PROBLEM
//				 AT MASS REASSIGNMENT OF CODES WE RECEIVE FOLLOWING POSITION
//				 ONE CODE IS REASSIGNED TO ANOTHER, AND ANOTHER IS REASSIGNED TO FIRST
//				 THE UTTERMOST LOSS OF THE INFORMATION OF ONE OF CODES FOLLOWS FROM HERE
//				 THE SOLVING OF THIS PROBLEM CONSISTS IN FOLLOWING
//				 FOR THE FIRST WE MAKE FULL ANALYSIS FOR RECEPTION OF ALL REPLACEMENTS OF CODES
//				 AND THEN REASSIGN PAIRS BY FREE CODE IF THOSE EXISTS.
//
//				 ALL RECORDS IN THIS ARE INDEPENDENT THAT IS EACH CHANGE IS NECESSARILY CARRIED OUT
//				 for this work the following class also was created.

	class MCodeChange{
	struct CodeChangeRecord{
		int iChangeFrom;
		int iChangeTo;
		CodeChangeRecord(int iFrom,int iTo){
			iChangeFrom = iFrom;iChangeTo = iTo;};
		CodeChangeRecord(){iChangeFrom = 0;iChangeTo = 0;};
	};
	
//PACKAGE : MAP.
//CLASS : MRecordArray : public GlobalArray<CodeChangeRecord>.
//DESCRIPTION : Array of records.
class MRecordArray : public GlobalArray<CodeChangeRecord>
	{

//PACKAGE : MAP.
//FUNCTION : MRecordArray::CreateOfType();
//DESCRIPTION : This functions redefining at inheritors. Also creates array of the same type as it is.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MRecordArray();};

	};
	MRecordArray m_records;
	public:

//PACKAGE : MAP.
//FUNCTION : MRecordArray::AddRecord(int iChangeFrom,int iChangeTo);
//DESCRIPTION : Adds a record.
//INPUTS : int ,int .
//RETURNS : NONE.
	void AddRecord(int iChangeFrom,int iChangeTo){MCodeChange::CodeChangeRecord temp(iChangeFrom,iChangeTo);
											m_records.AddTail(&temp);};

//PACKAGE : MAP.
//FUNCTION : MRecordArray::ApplyChangeRecords(MKlassifikator* pKls,TableToChangeKod whatTheTable);
//DESCRIPTION : Functions of old records replacement.
//INPUTS : MKlassifikator pointer,TableToChangeKod .
//RETURNS : NONE.
	void ApplyChangeRecords(MKlassifikator* pKls,TableToChangeKod whatTheTable);

//PACKAGE : MAP.
//FUNCTION : MRecordArray::ApplyAndRemove(MKlassifikator* pKls,TableToChangeKod whatTheTable);
//DESCRIPTION : Applies reassignments  and deletes old records from the table.
//INPUTS : MKlassifikator pointer,TableToChangeKod .
//RETURNS : NONE.
	void ApplyAndRemove(MKlassifikator* pKls,TableToChangeKod whatTheTable);

protected:

//PACKAGE : MAP.
//FUNCTION : MRecordArray::ChangeKods(MKlassifikator* pKls,TableToChangeKod whatTheTable,int iRec);
//DESCRIPTION : The replacement.
//INPUTS : MKlassifikator pointer,TableToChangeKod ,int .
//RETURNS : NONE.
	void ChangeKods(MKlassifikator* pKls,TableToChangeKod whatTheTable,int iRec);

//PACKAGE : MAP.
//FUNCTION : MRecordArray::GetFreeCod(MKlassifikator* pKls,TableToChangeKod whatTheTable);
//DESCRIPTION : Gets free code for the current table.
//INPUTS : MKlassifikator pointer,TableToChangeKod .
//RETURNS : int.
	int GetFreeCod(MKlassifikator* pKls,TableToChangeKod whatTheTable);
};

#endif //TEMPLARRAY_H

