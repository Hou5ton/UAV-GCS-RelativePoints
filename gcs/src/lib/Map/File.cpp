// MFile.cpp: implementation of the MFile class 
// and MBmhFILE class
//////////////////////////////////////////////////////////////////////

#include "File.h"
#include "SysDefine.h"
#include "Refglobal.h"
#include "Id.h"
#include <QFile> 
#include <QBuffer> 
#include <QVector>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MFile::MFile()
{
	m_data = NULL;
	m_pos = m_size = 0;
}

MFile::~MFile()
{
	if(m_data) delete[] m_data;
}
//==================================
//! installation of a way to the file, with check of existence of the file
bool MFile::SetPath(MString& path)
//==================================
{
    //we check existence of the file
	FILE* file = fopen(path, "rb");
	if(!file)
	{
        //the file doesn't exist
		MString str("Can't open file ");
		str += path;
        InternalMessage(str.c_str());
		return false;
	}
	fclose(file);

	m_path = path;
	return true;
}

//==================================
//! reading the file specified in 'm_path'
bool MFile::ReadFile()
//==================================
{
	FILE* file = fopen(m_path, "rb");

    //we check existence of the file
    if(!file)
	{
        //the file doesn't exist
        MString str("Can't open file ");
		str += m_path;
        InternalMessage(str.c_str());
		return false;
	}

    //we receive number of bytes in the file
	fseek(file, 0, SEEK_END);
	int pos = ftell(file);

	m_size = pos;

    //if earlier memory was marked out, we clear it
	if(m_data) delete[] m_data;

    //we mark out memory under data from the file
	m_data = new char[m_size];

    //we read out data from the file in memory
	fseek(file, 0, SEEK_SET);
	fread(m_data, 1, m_size, file);
	m_pos = 0;

	fclose(file);

	return true;
}

//==================================
//! reading the file with the 'm_path' installation
bool MFile::ReadFile(MString& path)
//==================================
{
    //we establish a way to the file
	if(!SetPath(path)) return false;
	
	return ReadFile();
}

//==================================
//! installation of the current position in the block of data
void MFile::SetPosition(int offset, int orign)
//==================================
{
    //if there are no data that leave
	if(!m_data) return;

	switch(orign)
	{
    case(MAP_SEEK_CUR)://from the current place
		{
			m_pos += offset;
			if(m_pos>m_size) m_pos = m_size;
		}
		break;
    case(MAP_SEEK_SET)://from the beginning
		{
			m_pos = offset;
			if(m_pos>m_size) m_pos = m_size;
		}
		break;
    case(MAP_SEEK_END)://from the end
		{
			m_pos = m_size - offset;
			if(m_pos<0) m_pos = 0;
		}
		break;
	}//end switch
}


//==================================
//! to obtain data in the 'buff' variable the 'size_t' size in number of 'count'
bool MFile::GetData(void* buff, int size_t, int count)
//==================================
{
    //if there are no data that leave
	if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
	if(m_pos+(size_t*count) > m_size) return false;

    //we read data
	for(int i = 0; i<(size_t*count); i++)
		((char*)(buff))[i] = m_data[m_pos+i];

    //we shift the pointer of the current position
	m_pos += (size_t*count);
	
	return true;
}

//==================================
//! to receive 'int'
bool MFile::GetInt(int& i)
//==================================
{
    //if there are no data that leave
	if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
    if(m_pos+sizeof(int) > m_size) return false;

	union
	{
		int ui;
		char uc[sizeof(int)];
	};
    //we read data
    for(unsigned int j = 0; j<sizeof(int); j++)
		uc[j] = m_data[m_pos+j];
 if (reverseReadFlag) reverseValue((char*)&ui, sizeof(int) );

	i = ui;
    //we shift the pointer of the current position
    m_pos += sizeof(int);

	return true;
}

//==================================
//! to receive 'short'
bool MFile::GetShort(short& s)
//==================================
{
    //if there are no data that leave
    if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
    if(m_pos+sizeof(short) > m_size) return false;

	union
	{
		short us;
		char uc[sizeof(short)];
	};
    //we read data
    for(unsigned int j = 0; j<sizeof(short); j++)
		uc[j] = m_data[m_pos+j];

 if (reverseReadFlag) reverseValue((char*)&us, sizeof(short) );
	s = us;
    //we shift the pointer of the current position
    m_pos += sizeof(short);

	return true;
}

//==================================
//! to receive
bool MFile::GetLong(int& l)
//==================================
{
    //if there are no data that leave
    if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
    if(m_pos+sizeof(int) > m_size) return false;

	union
	{
		int ul;
		char uc[sizeof(int)];
	};
    //we read data
    for(unsigned int j = 0; j<sizeof(int); j++)
		uc[j] = m_data[m_pos+j];

 if (reverseReadFlag) reverseValue((char*)&ul, sizeof(int) );
	l = ul;
    //we shift the pointer of the current position
    m_pos += sizeof(int);

	return true;
}

//==================================
//! to receive 'double'
bool MFile::GetDouble(double& d)
//==================================
{
    //if there are no data that leave
    if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
    if(m_pos+sizeof(double) > m_size) return false;

	union
	{
		double ud;
		char uc[sizeof(double)];
	};
    //we read data
    for(unsigned int j = 0; j<sizeof(double); j++)
		uc[j] = m_data[m_pos+j];

 if (reverseReadFlag) reverseValue((char*)&ud, sizeof(double) );
	d = ud;
    //we shift the pointer of the current position
    m_pos += sizeof(double);

	return true;
}

//==================================
//! to receive 'float'
bool MFile::GetFloat(float& f)
//==================================
{
    //if there are no data that leave
    if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
    if(m_pos+sizeof(float) > m_size) return false;

	union
	{
		float uf;
		char uc[sizeof(float)];
	};
    //we read data
    for(unsigned int j = 0; j<sizeof(float); j++)
		uc[j] = m_data[m_pos+j];

 if (reverseReadFlag) reverseValue((char*)&uf, sizeof(float) );
	f = uf;
    //we shift the pointer of the current position
    m_pos += sizeof(float);

	return true;
}

//==================================
//! to receive 'char'
bool MFile::GetChar(char& c)
//==================================
{
    //if there are no data that leave
    if(!m_data) return false;

    //if the readable block leaves by the size abroad of the block of data, we leave
    if(m_pos+sizeof(char) > m_size) return false;

    //we read data
    for(unsigned int j = 0; j<sizeof(char); j++)
		c = m_data[m_pos+j];

    //we shift the pointer of the current position
    m_pos += sizeof(char);

	return true;
}
