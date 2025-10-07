//PACKAGE : MAP.
//FILE: Mstring.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MSTRING_H
#define MSTRING_H
#include <string>
#include <stdarg.h>

//PACKAGE : MAP.
//CLASS	: MString:public std::string.
//DESCRIPTION : Class MString describes all functions of operation with strings
//				Realized by template STL - string, plus
//				a couple of additional functions from MFC CString.

class MString:public std::string
{
public:

//PACKAGE : MAP.
//FUNCTION : MString::MString();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MString();

//PACKAGE : MAP.
//FUNCTION : MString::MString(const char *str);
//DESCRIPTION : Constructor.
//INPUTS : const char pointer.
//RETURNS : NONE.
	MString(const char *str);

//PACKAGE : MAP.
//FUNCTION : MString::MString(const std::string &str);
//DESCRIPTION : Constructor.
//INPUTS : const std::string &.
//RETURNS : NONE.
	MString(const std::string &str);

//PACKAGE : MAP.
//FUNCTION : MString::MString(const MString& str);
//DESCRIPTION : Constructor.
//INPUTS : const MString& .
//RETURNS : NONE.
	MString(const MString& str);

//PACKAGE : MAP.
//FUNCTION : MString::~MString();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MString();

// Functions of operation with strings, similarly to class from MFC::CString

//PACKAGE : MAP.
//FUNCTION : MString::GetLength();
//DESCRIPTION : GetLength Returns the number of characters in a CString object. For multibyte characters, counts each 8-bit character; that is, a lead and trail byte in one multibyte character are counted as two characters.
//INPUTS : NONE.
//RETURNS : unsigned int.
	unsigned int GetLength() const
			{return length();};

//PACKAGE : MAP.
//FUNCTION : MString::IsEmpty();
//DESCRIPTION : IsEmpty Tests whether a CString object contains no characters.
//INPUTS : NONE.
//RETURNS : True.
	bool IsEmpty() const
			{return empty();};

//PACKAGE : MAP.
//FUNCTION : MString::Empty();
//DESCRIPTION : Empty Forces a string to have 0 length.
//INPUTS : NONE.
//RETURNS : NONE.
	void Empty() 
			{if(!empty()) erase(0,length());};

//PACKAGE : MAP.
//FUNCTION : MString::GetAt(int nIndex);
//DESCRIPTION : GetAt Returns the character at a given position.
//INPUTS : int.
//RETURNS : char.
	char GetAt(int nIndex) const
			{return at(nIndex);};

//PACKAGE : MAP.
//FUNCTION : MString::SetAt(int nIndex,char ch);
//DESCRIPTION : SetAt Sets a character at a given position.
//INPUTS : int ,char .
//RETURNS : NONE.
	void SetAt(int nIndex,char ch)
			{at(nIndex) = ch;};

//PACKAGE : MAP.
//FUNCTION : MString::Compare(MString lpsz);
//DESCRIPTION : Function of comparison of two strings, similarly to class from MFC::CString.
//INPUTS : MString.
//RETURNS : int.
	int Compare(MString lpsz) const
			{return compare(lpsz/*, 0, lpsz.length()*/);};

//PACKAGE : MAP.
//FUNCTION : MString::Find(char ch);
//DESCRIPTION : Functions of search, similarly to class from MFC::CString
//				Searchs for the symbol in a line.
//INPUTS : char.
//RETURNS : int.
	int Find(char ch) const
			{return find(ch,0);};

//PACKAGE : MAP.
//FUNCTION : MString::Find(MString pstr);
//DESCRIPTION : Searcs for a subline in a line.
//INPUTS : MString.
//RETURNS : int.
	int Find(MString pstr) const
			{return find(pstr,0);};

//PACKAGE : MAP.
//FUNCTION : MString::Find(char ch, int nStart );
//DESCRIPTION : Searchs for a symbol, starts with defined position.
//INPUTS : char , int .
//RETURNS : int.
	int Find(char ch, int nStart ) const
			{return find(ch,nStart);};

//PACKAGE : MAP.
//FUNCTION : MString::Find(MString pstr, int nStart );
//DESCRIPTION : Searchs for a subline, starts with defined position.
//INPUTS : MString , int .
//RETURNS : int.
	int Find(MString pstr, int nStart ) const
			{return find(pstr,nStart);};

//PACKAGE : MAP.
//FUNCTION : MString::ReverseFind(char ch);
//DESCRIPTION : ReverseFind Finds a character inside a larger string; starts from the end.
//INPUTS : char.
//RETURNS : int.
	int ReverseFind(char ch) const
			{return rfind(ch);};

//PACKAGE : MAP.
//FUNCTION : MString::FindOneOf(MString CharSet);
//DESCRIPTION : FindOneOf Finds the first matching character from a set.
//INPUTS : MString.
//RETURNS : int.
	int FindOneOf(MString CharSet) const{return find_first_of(CharSet);};	

//PACKAGE : MAP.
//FUNCTION : MString::MakeReverse();
//DESCRIPTION : Functions of remaining transformations, similarly to class from MFC::CString
// 				Turns over the string.
//INPUTS : NONE.
//RETURNS : NONE.
	void MakeReverse( );

//PACKAGE : MAP.
//FUNCTION : MString::Replace(char chOld,char chNew);
//DESCRIPTION : Replace Replaces indicated characters with other characters.
//INPUTS : char ,char .
//RETURNS : int.
	int Replace(char chOld,char chNew);
	
//PACKAGE : MAP.
//FUNCTION : MString:: Replace(MString sOld,MString sNew);
//DESCRIPTION : Replace Replaces indicated characters with other characters.
//INPUTS : MString ,MString .
//RETURNS : int.
	int Replace(MString sOld,MString sNew);

//PACKAGE : MAP.
//FUNCTION : MString::Remove(char ch);
//DESCRIPTION : Remove Removes indicated characters from a string.
//INPUTS : char.
//RETURNS : int.
	int Remove(char ch);	

//PACKAGE : MAP.
//FUNCTION : MString::Insert(int nIndex,char ch);
//DESCRIPTION : Insert Inserts a single character or a substring at the given index within the string.
//INPUTS : int ,char .
//RETURNS : int.
	int Insert(int nIndex,char ch);

//PACKAGE : MAP.
//FUNCTION : MString::Insert(int nIndex,MString pstr);
//DESCRIPTION :  Insert Inserts a single character or a substring at the given index within the string.
//INPUTS : int ,MString .
//RETURNS : int.
	int Insert(int nIndex,MString pstr);

//PACKAGE : MAP.
//FUNCTION : MString::Delete(int nIndex,int nCount = 1);
//DESCRIPTION : Delete Deletes a character or characters from a string.
//INPUTS : int ,int .
//RETURNS : int.
	int Delete(int nIndex,int nCount = 1);

//PACKAGE : MAP.
//FUNCTION : MString::TrimRight();
//DESCRIPTION : Cuts off unused blanks from the right.
//INPUTS : NONE.
//RETURNS : NONE.
	void TrimRight();

//PACKAGE : MAP.
//FUNCTION : MString::TrimLeft();
//DESCRIPTION : Cuts off unused blanks from the left.
//INPUTS : NONE.
//RETURNS : NONE.
	void TrimLeft();

//PACKAGE : MAP.
//FUNCTION : MString::Left(int nCount);
//DESCRIPTION : Returns 'nCount' of elements from the left.
//INPUTS : int.
//RETURNS : MString.
	MString Left(int nCount) const;

//PACKAGE : MAP.
//FUNCTION : MString::Right(int nCount);
//DESCRIPTION : Returns 'nCount' of elements from the right.
//INPUTS : int.
//RETURNS : MString.
	MString Right(int nCount) const;

//PACKAGE : MAP.
//FUNCTION : MString::Mid(int nFirst);
//DESCRIPTION : Returns elements from defined position.
//INPUTS : int.
//RETURNS : MString.
	MString Mid(int nFirst) const;

//PACKAGE : MAP.
//FUNCTION : MString::Mid(int nFirst, int nCount);
//DESCRIPTION : Returns 'nCount' elements from defined position.
//INPUTS : int , int .
//RETURNS : MString.
	MString Mid(int nFirst, int nCount) const;

//PACKAGE : MAP.
//FUNCTION : MString::SpanIncluding(MString lpszCharSet);
//DESCRIPTION : Extracts a substring that contains only the characters in a set.
//INPUTS : MString.
//RETURNS : MString.
	MString SpanIncluding(MString lpszCharSet) const;

//PACKAGE : MAP.
//FUNCTION : MString::SpanExcluding(MString lpszCharSet);
//DESCRIPTION : Extracts a substring that contains only the characters not in a set.
//INPUTS : MString.
//RETURNS : MString.
	MString SpanExcluding(MString lpszCharSet) const;

//PACKAGE : MAP.
//FUNCTION : MString::FmtOneValue (const char*&, va_list&);
//DESCRIPTION : Format Format the string as sprintf does.
//INPUTS : const char pointer.
//RETURNS : int.
	int FmtOneValue (const char*&, va_list&);	

//PACKAGE : MAP.
//FUNCTION : MString::Format(const char*, ...);
//DESCRIPTION : Formatted outputing to the string.
//INPUTS : const char pointer.
//RETURNS : NONE.
	void Format(const char*, ...);

//PACKAGE : MAP.
//FUNCTION : MString::operator char * ();
//DESCRIPTION : Reloads operator of appropriation for operation with 'MFC'-th class of 'CString'
//				operator LPCTSTR()
//				{return c_str();}.
//INPUTS : NONE.
//RETURNS : char pointer.
	operator char *() 
			{return (char *)c_str();};

//PACKAGE : MAP.
//FUNCTION : MString::operator[];
//DESCRIPTION : 
//INPUTS : int.
//RETURNS : char.
	char operator[](int) const;

//PACKAGE : MAP.
//FUNCTION : MString::operator = ;
//DESCRIPTION : Reloads operator of appropriation with the same
//				syntax as in base class 'string'.
//INPUTS : char.
//RETURNS : MString&.
	MString&	operator=(char &ch){assign(&ch,1);return *this;};

//PACKAGE : MAP.
//FUNCTION : MString::MString();
//DESCRIPTION : Operator of appropriation.
//INPUTS : const char pointer.
//RETURNS : MString&.
	MString&	operator=(const char *buffer){assign(buffer);return *this;};

//PACKAGE : MAP.
//FUNCTION : MString::operator = ;
//DESCRIPTION : Operator of appropriation.
//INPUTS : const MString &.
//RETURNS : MString &.
	MString&	operator=(const MString &str);
};

#endif
