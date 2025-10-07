//PACKAGE : MAP.
//FILE: FMT.h    
//AUTHOR:
//DESCRIPTION:

#ifndef __Format_H__
#define __Format_H__
#include "BaseCl.h"
#include "MString.h"

//PACKAGE : MAP.
//CLASS	: MShapefile.
//DESCRIPTION : Class for shape-file reading.
class MShapefile

{
public:
	bool	IsFileOpen;	// Variable of file opening
	FILE*	stream;		// Working file
	int*	p;			// Pointer to array of displacements for records from the beginning of file
	int*	q;			// Pointer to array of displacements for records from the beginning of file
	int		N;			// Records quantity of file

//PACKAGE : MAP.
//FUNCTION : MShapefile::MShapefile();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MShapefile();

//PACKAGE : MAP.
//FUNCTION : MShapefile::~MShapefile();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MShapefile(){};

//PACKAGE : MAP.
//FUNCTION : MShapefile::OpenFile(MString string);
//DESCRIPTION : Functions of file opening.
//INPUTS : MString.
//RETURNS : True.
	bool OpenFile(MString string);
	
//PACKAGE : MAP.
//FUNCTION : MShapefile::CloseFile();
//DESCRIPTION : Functions of file closing.
//INPUTS : NONE.
//RETURNS : True.
	bool CloseFile();

	//!functions of diagnostic

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetNumberOfRecords();
//DESCRIPTION : Gets records quantity of shape-file.
//INPUTS : NONE.
//RETURNS : int.
	int GetNumberOfRecords();

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetFileType();
//DESCRIPTION : Reads file type.
//INPUTS : NONE.
//RETURNS : int.
	int GetFileType();

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetRecType(int n);
//DESCRIPTION : Reads type of n-th record.
//INPUTS : int .
//RETURNS : int.
	int GetRecType(int n);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetNumberOfRecParts(int n);
//DESCRIPTION : Reads parts quantity for n-th record.
//INPUTS : int .
//RETURNS : int.
	int GetNumberOfRecParts(int n);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetNumberOfRecPoints(int n);
//DESCRIPTION : Reads points quantity for n-th record.
//INPUTS : int .
//RETURNS : int.
	int GetNumberOfRecPoints(int n);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetPartPointIndex(int n, int s);
//DESCRIPTION : Reads index of first point of s-th part for n-th record.
//INPUTS : int , int .
//RETURNS : int.
	int GetPartPointIndex(int n, int s);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetPartNumberOfPoints(int n, int s);
//DESCRIPTION : Defines points quantity of s-th part for n-th record.
//INPUTS : int , int .
//RETURNS : int.
	int GetPartNumberOfPoints(int n, int s);

	// Function of reading

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetMapRect(MRect* prect);
//DESCRIPTION : Reads boundary rectangle for shape-file.
//INPUTS : MRect pointer.
//RETURNS : int.
	int GetMapRect(MRect* prect);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetRecRect(MRect* prect, int n);
//DESCRIPTION : Reads boundary rectangle for n-th record.
//INPUTS : MRect pointer, int .
//RETURNS : int.
	int GetRecRect(MRect* prect, int n);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetPoint(MPoint* geopoint,int n, int s, int k);
//DESCRIPTION : Reads k-th point of n-th record of s-th part.
//INPUTS : MPoint pointerint , int , int .
//RETURNS : int.
	int GetPoint(MPoint* geopoint,int n, int s, int k);

//PACKAGE : MAP.
//FUNCTION : MShapefile::GetRecord(MRect* prect, int* index, MPoint* geopoints, int n);
//DESCRIPTION : Reads n-th record.
//INPUTS : MRect pointer, int pointer, MPoint pointer, int .
//RETURNS : int.
	int GetRecord(MRect* prect, int* index, MPoint* geopoints, int n);

};

//===============================================================================
// DBF (H)
//===============================================================================

//PACKAGE : MAP.
//CLASS	: MDbffile.
//DESCRIPTION : Class of Dbf-file reading.
class MDbffile

{
public:
	bool	IsFileOpen;	// Variable of file opening
	FILE*	stream;		// Working file
	int		N;			// Fields quantity of dbf-file,
	int		Z;			// Records quantity of dbf-file
	unsigned short frstRec, // first record position
                   recLen;  // record length
	
	char fieldTyp[100];		// array of field types
	short fieldLen[100];		// array of field lengthes

//PACKAGE : MAP.
//FUNCTION : MDbffile::MDbffile();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.	
	MDbffile();

//PACKAGE : MAP.
//FUNCTION : MDbffile::~MDbffile();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MDbffile(){};

//PACKAGE : MAP.
//FUNCTION : MDbffile::OpenFile(MString string);
//DESCRIPTION : Function of opening file.
//INPUTS : MString.
//RETURNS : True.
	bool OpenFile(MString string);

//PACKAGE : MAP.
//FUNCTION : MDbffile::CloseFile();
//DESCRIPTION : Function of closing file.
//INPUTS : NONE.
//RETURNS : True.
	bool CloseFile();

	
// Functions of dbf-file reading

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetFieldsNumber();
//DESCRIPTION : Reads fields quantity.
//INPUTS : NONE.
//RETURNS : int.
	int GetFieldsNumber();

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetRecordsNumber();
//DESCRIPTION : Reads records quantity.
//INPUTS : NONE.
//RETURNS : int.
	int GetRecordsNumber();

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetFieldName(int n);
//DESCRIPTION : Reads field name by number.
//INPUTS : int .
//RETURNS : MString.
	MString GetFieldName(int n);

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetFieldType(int n);
//DESCRIPTION : Reads field type by number.
//INPUTS : int .
//RETURNS : char.
	char GetFieldType(int n);

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetFieldLength(int n);
//DESCRIPTION : Reads record lenght for defined field.
//INPUTS : int .
//RETURNS : int.
	int GetFieldLength(int n);

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetString(const char* pch, int n, int z);
//DESCRIPTION : Gets symbolic field(n) value for z-th record.
//INPUTS : const char pointer, int , int .
//RETURNS : MString.
	MString GetString(const char* pch, int n, int z);

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetIntNumber(int n, int z);
//DESCRIPTION : Gets integer value of numberic fileld(n) for z-th record.
//INPUTS : int , int .
//RETURNS : int.
	int GetIntNumber(int n, int z);

//PACKAGE : MAP.
//FUNCTION : MDbffile::GetDoubleNumber(int n, int z);
//DESCRIPTION : Gets double-numberic value of n-field for z-record.
//INPUTS : int , int .
//RETURNS : double.
	double GetDoubleNumber(int n, int z);

//PACKAGE : MAP.
//FUNCTION : MDbffile::ReturnString(int n, int z);
//DESCRIPTION : Returns cell as line regardless of field type.
//INPUTS : int , int .
//RETURNS : MString.
	MString ReturnString(int n, int z);
	QString GetValueByfieldName(MString &fieldName, int z);
};


#endif // __Format_H__
