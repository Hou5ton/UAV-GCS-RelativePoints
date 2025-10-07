
#include "MString.h"
#include <algorithm>
#include <cstdio>
#include <string.h>

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// Functions of the class MString
//!  MString
MString::MString():string()
{ 
}
//!  MString
MString::MString(const char *str):string(str)
{
}
//!  MString
MString::MString(const string &str):string(str)
{
}
//!  MString
MString::MString(const MString &str):string(str)
{
}
//!  MString
MString::~MString()
{
}

//!  --
void MString::MakeReverse( )
{
	string str;
	for(int i=length()-1;i>=0;i--)
		str += at(i);
	assign(str);
}

//!  --
/** Since function works with symbols, replaced
//and replacing lines respectively are equal, and it is possible
//to carry out replacement simply passing on a cycle
*/
int MString::Replace(char chOld,char chNew)
{
	int nReplace = 0;
	for(unsigned int i=0;i<length();i++)
		if(at(i)==chOld)
		{
			at(i) = chNew;
			nReplace ++;
		}
	return nReplace;
}

//!  -------
/** Since function works from a strings, replaced
//and replacing lines respectively can differ
//dimension and not to carry out simple replacement on a cycle.
*/
int MString::Replace(MString sOld,MString sNew)
{
	int nReplace = 0;
	int nFind = find(sOld,0);
	while(nFind!=-1)
	{
		erase(nFind,sOld.length());
		insert(nFind,sNew);
		nReplace ++;
		nFind = find(sOld,nFind+sNew.length());
	}
	
	return nReplace;
}

//!  -------
int MString::Remove(char ch)	
{
	int nLBefore = Find(ch,0);
	int nLAfter = 0;
	while(nLBefore!=-1)
	{
		erase(nLBefore,1);
		nLAfter++;
		nLBefore = Find(ch,0);
	}

	return nLBefore - nLAfter;
}

//!  -------
int MString::Insert(int nIndex,char ch)
{
	string str;
	str += ch;
	insert(nIndex,str);
	return length();
}

//!  -------
int MString::Insert(int nIndex,MString pstr)
{
	insert(nIndex,pstr);
	return length();
}

//!  -------
void MString::TrimRight()
{
	if(!empty())
	while(GetAt(GetLength()-1)==' ')
	{
		Delete(GetLength()-1,1);
		if(empty()) break;
	}
}

//!  -------
void MString::TrimLeft()
{
	if(!empty()) 
	while(GetAt(0)==' ')
	{
		Delete(0,1);
		if(empty()) break;
	}
}

// A substring that contains characters in the string that are in lpszCharSet, 
// beginning with the first character in the string and ending when a character 
// is found in the string that is not in lpszCharSet. SpanIncluding returns an 
// empty substring if the first character in the string is not in the specified set.

//!  -------
MString MString::SpanIncluding(MString lpszCharSet) const
{
	MString ret = "";
	int iPos = ret.find_first_not_of(lpszCharSet,0);
	if(iPos!=-1)
	{
		ret = c_str();
		ret.erase(iPos,GetLength()-iPos);
	}
	return ret;
}

// A substring that contains characters in the string that are not in lpszCharSet, 
// beginning with the first character in the string and ending with the first 
// character found in the string that is also in lpszCharSet (that is, starting with 
// the first character in the string and up to but excluding the first character in 
// the string that is found lpszCharSet). It returns the entire string if no character 
// in lpszCharSet is found in the string. 
//!  -------
MString MString::SpanExcluding(MString lpszCharSet) const
{
	MString ret = c_str();
	int iPos = ret.find_first_of(lpszCharSet,0);
	if(iPos!=-1)
		ret.erase(iPos,GetLength()-iPos);
	return ret;
}

//!  -------
MString MString::Left(int nCount) const
{
	int Count = (nCount<(int)GetLength())?nCount:GetLength();
	return substr(0,Count);
}

//!  -------
MString MString::Right(int nCount) const
{
	int Count = (nCount<(int)GetLength())?nCount:GetLength();
	return substr(GetLength()-Count,Count);
}

//!  -------
MString MString::Mid(int nFirst) const
{
	return substr(nFirst,GetLength());
}

//!  -------
MString MString::Mid(int nFirst, int nCount) const
{
	int Count = (nCount<(int)GetLength())?nCount:GetLength();
	return substr(nFirst,Count);
}

//!  -------
int MString::Delete(int nIndex,int nCount)
{
	erase(nIndex,nCount);
	return length();
}

//!  -------
int MString::FmtOneValue (const char*& x, va_list& marker)
{
	// Start copying format specifier to a local buffer
	char fsbuf[64];
	fsbuf[0] = '%';
	int fsp = 1;
GetMoreSpecifiers:
	// Get one character
	char ch = x[0];
	if (ch == 0)
		return 0;		// unexpected end of format string
	x++;
	// Chars that may exist in the format prefix
	const char fprefix[] = "-+0 #*.123456789hlL";
	if (strchr (fprefix, ch) != NULL) {
		fsbuf[fsp] = ch;
		fsp++;
		goto GetMoreSpecifiers;
	}
	// 's' is the most important parameter specifier type
	if (ch == 's') {
		// Find out how many characters should we actually print.
		//   To do this, get the string length, but also try to
		//   detect a .precision field in the format specifier prefix.
		const char* value = va_arg (marker, const char*);
		int slen = strlen(value);
		fsbuf[fsp] = 0;
		char* precis = strchr (fsbuf, '.');
		if (precis != NULL  &&  precis[1] != 0) {
			// Convert value after dot, put within 0 and slen
			char* endptr;
			int result = (int) strtol (precis+1, &endptr, 10);
			if (result >= 0  &&  result < int(slen))
				slen = (int) result;
		}
		// Copy the appropriate number of characters
		if (slen > 0)
			append(value,slen);
		return 1;
	}
	// '!' is our private extension, allows direct passing of CStr*
	if (ch == '!') {
		// No precision characters taken into account here.
		const MString* value = va_arg (marker, const MString*);
		*this += *value;
		return 1;
	}
	// Chars that format an integer value
	const char intletters[] = "cCdiouxX";
	if (strchr (intletters, ch) != NULL) {
		fsbuf[fsp] = ch;
		fsbuf[fsp+1] = 0;
		char valbuf[64];
		int value = va_arg (marker, int);
		sprintf (valbuf, fsbuf, value);
		append(valbuf,strlen(valbuf));
		
		return 1;
	};
	// Chars that format a double value
	const char dblletters[] = "eEfgG";
	if (strchr (dblletters, ch) != NULL) {
		fsbuf[fsp] = ch;
		fsbuf[fsp+1] = 0;
		char valbuf[256];
		double value = va_arg (marker, double);
		sprintf (valbuf, fsbuf, value);
		*this += valbuf;
		return 1;
	}
	// 'Print pointer' is supported
	if (ch == 'p') {
		fsbuf[fsp] = ch;
		fsbuf[fsp+1] = 0;
		char valbuf[64];
		void* value = va_arg (marker, void*);
		sprintf (valbuf, fsbuf, value);
		*this += valbuf;
		return 1;
	};
	// 'store # written so far' is obscure and unsupported
	if (ch == 'n') {
		return 0;
	}
	// 'Print unicode string' is not supported also
	if (ch == 'S') {
		return 0;
	}
	// If we fall here, the character does not represent an item
	append(ch,1);

	return 1;
}

//!  -------
void MString::Format(const char* fmt, ...)
{
	erase(0,length());
	// Walk the string
	va_list marker;
	va_start(marker, fmt);
	
	for(;;) 
	{
		// Locate next % sign, copy chunk, and exit if no more
		char* next_p = strchr((char*)fmt, '%');
		if(next_p == NULL)
			break;
		if(next_p>fmt)
			append(fmt,int(next_p-fmt));
		fmt = next_p+1;
		// We're at a parameter
		if(!FmtOneValue(fmt,marker))
			break;		// Copy rest of string and return
	}
	if(fmt[0] != 0)
		*this += fmt;	
	
	va_end(marker);
}

//!  -------
char MString::operator[](int idx) const
{
	return at(idx);
}


//!  -------
MString& MString::operator=(const MString &str)
{
	assign(str,0,str.length());
	return *this;
}
