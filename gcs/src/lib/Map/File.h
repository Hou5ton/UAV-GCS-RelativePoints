//PACKAGE : MAP.
//FILE: File.h.       
//AUTHOR:
//DESCRIPTION:

#ifndef MFILE_H_
#define MFILE_H_
#include "MString.h"
#include <qiodevice.h> 

#include "Demming/MBmhFILE.h"

// Lists availeble locations of pointer when map file reading
enum UT_FILE_ORIGN{MAP_SEEK_SET = 0, MAP_SEEK_CUR = 1, MAP_SEEK_END = 2};

//PACKAGE : MAP.
//CLASS	: MFile .
//DESCRIPTION : Class reads file to memory and works with it
class MFile
{
	char*		m_data;		//  pointer to data readed from file
	MString		m_path;		//  file path
	unsigned	int m_pos;	//  present location in a data block
	unsigned	int m_size;	//  data block size

public:

//PACKAGE : MAP.
//FUNCTION : MFile::MFile();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MFile();

//PACKAGE : MAP.
//FUNCTION : MFile::~MFile();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MFile();

//PACKAGE : MAP.
//FUNCTION : MFile::SetPath(MString& path);
//DESCRIPTION : Sets path to file, also checks if file exists.
//INPUTS : MString& .
//RETURNS : True.
	bool SetPath(MString& path);

//PACKAGE : MAP.
//FUNCTION : MFile::GetPath();
//DESCRIPTION : Gets file path.
//INPUTS : NONE.
//RETURNS : MString .
	MString GetPath() {return m_path;}

//PACKAGE : MAP.
//FUNCTION : MFile::ReadFile();
//DESCRIPTION :  Reads file defined in 'm_path'.
//INPUTS : NONE.
//RETURNS : True.
	bool ReadFile();

//PACKAGE : MAP.
//FUNCTION : MFile::ReadFile(MString& path);
//DESCRIPTION : Reads file with installation 'm_path'.
//INPUTS : MString& .
//RETURNS : True.
	bool ReadFile(MString& path);

//PACKAGE : MAP.
//FUNCTION : MFile::GetPosition();
//DESCRIPTION : Gets current position of data block.
//INPUTS : NONE.
//RETURNS : int .
	int GetPosition() {return m_pos;}

//PACKAGE : MAP.
//FUNCTION : MFile::GetSize();
//DESCRIPTION : Gets data block size.
//INPUTS : NONE.
//RETURNS : int m_size.
	int GetSize() {return m_size;}

//PACKAGE : MAP.
//FUNCTION : MFile::SetPosition(int offset, int orign)();
//DESCRIPTION : Sets current position of data block.
//INPUTS : int , int .
//RETURNS : NONE.
	void SetPosition(int offset, int orign);

//PACKAGE : MAP.
//FUNCTION : MFile::GetData(void* buff, int size_t, int count);
//DESCRIPTION : Gets data to variable 'buff' of size 'size_t' with quantity of 'count'.
//INPUTS : void pointer , int , int .
//RETURNS : True.
	bool GetData(void* buff, int size_t, int count);

//PACKAGE : MAP.
//FUNCTION : MFile::GetInt(int& i);
//DESCRIPTION : Getting int from file .
//INPUTS : int& .
//RETURNS : True.
	bool GetInt(int& i);

//PACKAGE : MAP.
//FUNCTION : MFile::GetShort(short& s);
//DESCRIPTION : Getting short from file .
//INPUTS : short& .
//RETURNS : True.
	bool GetShort(short& s);

//PACKAGE : MAP.
//FUNCTION : MFile::GetLong(int& l);
//DESCRIPTION : Getting Long from file .
//INPUTS : int& .
//RETURNS : True.
	bool GetLong(int& l);

//PACKAGE : MAP.
//FUNCTION : MFile::GetDouble(double& d);
//DESCRIPTION : Getting double from file .
//INPUTS : double& .
//RETURNS : True.
	bool GetDouble(double& d);

//PACKAGE : MAP.
//FUNCTION : MFile::GetFloat(float& f);
//DESCRIPTION :  Getting float from file.
//INPUTS : float& .
//RETURNS : True.
	bool GetFloat(float& f);

//PACKAGE : MAP.
//FUNCTION : MFile::GetChar(char& c);
//DESCRIPTION :  Getting char from file.
//INPUTS : char& .
//RETURNS : True.
	bool GetChar(char& c);
};

#endif // ifndef(MFILE_H_)
