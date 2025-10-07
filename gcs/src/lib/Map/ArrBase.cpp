// NuxArray.cpp: implementation of the MMMMMM class.
//
//////////////////////////////////////////////////////////////////////

#include "ArrBase.h"
#include "BaseCl.h"
#include "BaseFun.h"
#include <iosfwd>
//
//*********************************************************
//******                   MPtArray                  ******
//*********************************************************

//  set a lot of points at once, specifying their number
// and a start address location in the memory
//! =========
void MPtArray::FastSet(int Cnt, MPoint* points)
//=========================================================
{
    // Clear
	ClearAll();
    //If the number of elements is equal to zero,
    //we create an empty array
	if(Cnt<0) Cnt=0;
    // Set the number of elements
    Size=Cnt;
    //Create an array
    if(Cnt){
        m_pHead=malloc(sizeof(MPoint)*Cnt);
        // Copy points
        memcpy (m_pHead, points, sizeof(MPoint)*Cnt);
    }
    else{
        m_pHead=NULL;
    }
//    if(Cnt)	m_pHead=malloc(sizeof(MPoint)*Cnt);
//	else m_pHead=NULL;
//    // Set the number of elements
//	Size=Cnt;
//    // Copy points
//	memcpy (m_pHead, points, sizeof(MPoint)*Cnt);
};

//=========================================================
// We set a lot of points at once, specifying their number
// and a start address location in the memory
//! ---------
void MPtArray::FastSet(MPoint* points)
//=========================================================
{
    // Copy points
	memcpy (m_pHead, points, sizeof(MPoint)*Size);
};


//===============================================
//!Splits an array into two parts
//Part 1 is here [0;index-1]
//Part 2 is created and returns [index ; Size]
MPtArray* MPtArray::Cut(int index)
//===============================================
{
	if((index<0)||(index>=Size))return NULL;
	int newSize=Size-index;
	MPtArray* pArr=new MPtArray();
	pArr->SetSize(newSize);
	for(int i=index;i<Size;i++)
		pArr->SetAt(i-index,GetAt(i));
	for(int j=0;j<newSize;j++)
		DeleteTail();
	return pArr;
}

//===============================================
//!Looks for a point 'pt'
int MPtArray::FindPoint(MPoint pt)
//===============================================
{
	int index = -1;
	for(int i=0; i<Size; i++){
		if(pt == *GetAt(i)){
			index = i;
			break;
		}
	}
	return index;
}


//*********************************************************
//******                   MNvArray                  ******
//*********************************************************

//============================================================
//!According to the index(code) returns the value
//!of the characteristics and the array index number 'nom'
int MNvArray::GetValue(unsigned short nom, MString& str)
//============================================================
{
	int index = -1;
	for(int i=0;i<Size;i++)
	{
		MNomValue* Atom=GetAt(i);
		if(Atom->GetNom()==nom){
			str = Atom->GetValue();
			index = i;
			break;
		}
	}
	return index;
};

//============================================================
//!According to the characteristics of the value
//!returns the index element in the array with the number nom.
//!If no such element , then returns -1.
int MNvArray::GetNom(MString str)
//============================================================
{
	for(int i=0;i<Size;i++)
	{
		MNomValue* Atom=GetAt(i);
		if(Atom->GetValue()==str)
			return Atom->GetNom();
	}
	return -1;
};

//============================================================
//!По индексу(коду)- ному устанавливает значение характеристики 
int MNvArray::SetValue(unsigned short nom, MString str)
//============================================================
{
	int index = -1;
	for(int i=0;i<Size;i++)
	{
		MNomValue* Atom=GetAt(i);
		if(Atom->GetNom()==nom){
			Atom->SetValue(str);
			index = i;
			break;
		}
	}
	return index;
};

//============================================================
//!According to the index in the array sets the characteristics
bool MNvArray::SetValueByIndex(int index, MString str)
//============================================================
{
	if((index<0)||(index>=Size))
		return false;
	GetAt(index)->SetValue(str);
	return true;
};

//============================================================
//!According to the index in the array sets the code characteristics
bool MNvArray::SetNomByIndex(int index, unsigned short nom)
//============================================================
{
	if((index<0)||(index>=Size))
		return false;
	GetAt(index)->SetNom(nom);
	return true;
};



//*********************************************************
//******                   MStArray                  ******
//*********************************************************



//=========================================================
//Constructor by default
//!creates empty array
MStArray::MStArray()
//=========================================================
{
    Size=0;//The number of elements
    m_pHead=NULL;//nowhere indicates
};

//=========================================================
//!Constructor with a given number of elements
/**Creates an array containing 'Cnt' elements
//constructed by default
*/
MStArray::MStArray(int Cnt)
//=========================================================
{
    //If the number of elements is equal to zero,
    //then create an empty array
	if(Cnt<0) Cnt=0;
    //Create an array
	if(Cnt)
		m_pHead=(void*)new MString[Cnt];
	else 
		m_pHead=NULL;
    //Set the number of elements
	Size=Cnt;
};

//=========================================================
//Destructor
//!Frees the memory
MStArray::~MStArray()
//=========================================================
{
	ClearAll();
};

//=========================================================
//!Deletes all elements of the array
void MStArray::ClearAll()
//=========================================================
{
    //Free the memory
	if(Size)
		delete [] ((MString*)m_pHead);
    //Making zero setting
	m_pHead=NULL;
	Size=0;
}

//==============================================
//!Returns the total size of the array in UTP-format
int MStArray::GetBytes()
//==============================================
{
    int nBytes=7;//3 ID bytes + 4 bytes on the number of elements
	for(int i=0;i<Size;i++)
	{
		MString* str=GetAt(i);
		nBytes+=2+str->GetLength();
	}
	return nBytes;
};

//=========================================================
//!Returns a reference to an element defined by the 'Index'
MString* MStArray::GetAt(int index)
//=========================================================
{
    //If the index is outside the array
    //returns a null element
	if(!((index>=0)&&(index<Size))) index=0;
	return (((MString*)m_pHead)+index);
};

//=========================================================
//!Returns a reference to an element defined by the 'Index'
MString* MStArray::operator[](int index)
//=========================================================
{
	return GetAt(index);
};

//=========================================================
//!Sets 'Element' in cell 'index'
void MStArray::SetAt(int index,MString* Element)
//=========================================================
{
    //If the index is outside the array ,
    // don't do anything
	if((index>=0)&&(index<Size))
	{
		MString* Atom=GetAt(index);
		Copy(Atom,Element);
	}
};

//=========================================================
//!Sets 'Element' in cell 'index' with increasing array
void MStArray::SetAtGrow(int nIndex, MString* newElement)
//=========================================================
{
    // Set the new size of the array
	SetSize(nIndex+1);
    // Set the new value ot the element
	SetAt(nIndex,newElement);
}

//=========================================================
//!Set the new number of elements
void MStArray::SetSize(int Cnt)
//=========================================================
{
    //If the number is less than zero or equal to the present,
    //don't do anything
	if((Cnt<0)||(Cnt==Size))return;
	
    //Create a new array
	MString* ptr=new MString[Cnt];
    //Copy the matching part
	int M=Min(Size,Cnt);
	for(int i=0;i<M;i++)
		Copy(GetAtSt(i,ptr),GetAt(i));
    //Delete the old array
	if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size=Cnt;
};


//=========================================================
//!Adds 'Element' in the beginning of the array
void MStArray::AddHead(MString* Element)
//=========================================================
{
    //Create a new array
	MString* ptr=new MString[Size+1];
    //Copy the matching part
	for(int i=0;i<Size;i++)
		Copy(GetAtSt(i+1,ptr),GetAt(i));
    //Delete the old array
	if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Set the first element
	SetAt(0,Element);
}


//=========================================================
//!Attaches array pointed
//! MArr to top array
void MStArray::AppendHead(MStArray* pArr)
//=========================================================
{
        //Create a new array
		MStArray* newArr=new MStArray(pArr->GetSize()+Size);
        //Fill a part of the merged array
		int i=pArr->GetSize()-1;
		for(i=i;i>-1;i--)
		{
			newArr->SetAt(i,pArr->GetAt(i));
		}
        //Fill in the former part
		int j=pArr->GetSize();
		for(i=0;i<Size;i++)
		{
			newArr->SetAt(i+j,GetAt(i));
		}
        //Transfer data from the temporary array to our
		ClearAll();
		Size=newArr->GetSize();
		m_pHead=newArr->m_pHead;
        //Correctly delete temporary array
		newArr->m_pHead=NULL;
		newArr->Size=0;
		delete newArr;
};

//=========================================================
//!Inserts the element into the cell , indicates by index
void MStArray::InsertBefore(int index,MString* Element)
//=========================================================
{
    //If the index is beyond the array,
    //don't do anything
	if (!((index>=0)&&(index<Size)))return;
    //Create a new array
	MString* ptr=new MString[Size+1];
    //Copy the matching part
	int N=index;
	int i = 0;
	for(i=0;i<N;i++)
		Copy(GetAtSt(i,ptr),GetAt(i));
	for(i=N;i<Size;i++)
		Copy(GetAtSt(i+1,ptr),GetAt(i));
    //Delete the old array
	if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Insert the element
	SetAt(index,Element);
};


//===================================================================
//!To compare two arrays of strings
bool MStArray::IsEqualTo(MStArray *m_pArrToCompare)
//===================================================================
{
	if(Size!=m_pArrToCompare->GetSize())
		return false;
    //Comparing row
	for(int i = 0;i<Size;i++){
		if((*GetAt(i))!=(*m_pArrToCompare->GetAt(i)))
			return false;
	}
	return true;
}


//=========================================================
//!Inserts the element 'Element' after the index 'index'
void MStArray::InsertAfter(int index,MString* Element)
//=========================================================
{
    //If the index is beyond the array,
    //don't do anything
	if (!((index>=0)&&(index<Size)))return;
    //Create a new array
	MString* ptr=new MString[Size+1];
    //Copy the matching part
	int N=index+1;
	int i = 0;
	for(i=0;i<N;i++)
		Copy(GetAtSt(i,ptr),GetAt(i));
	for(i=N;i<Size;i++)
		Copy(GetAtSt(i+1,ptr),GetAt(i));
    //Delete the old array
	if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Insert the element
	SetAt(index+1,Element);
};

//=========================================================
//!Adds 'Element' to the end of array
void MStArray::AddTail(MString* Element)
//=========================================================
{
    //Create a new array
	MString* ptr=new MString[Size+1];
    //Copy the matching part
    for(int i=0;i<Size;i++)
		Copy(GetAtSt(i,ptr),GetAt(i));
    //Delete the old array
    if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
    m_pHead=(void*)ptr;
    //Modify the number of
    Size++;
    //Set the first element
    SetAt(Size-1,Element);
};


//=========================================================
//!Add array pointed
//! pArr to the end of array
void MStArray::AppendTail(MStArray* pArr)
{
        //Create a new array
		MStArray* newArr=new MStArray(pArr->GetSize()+Size);
        //Fill in the former part
		int i=Size-1;
		for(i=i;i>-1;i--)
		{
			newArr->SetAt(i,GetAt(i));
		}
        //Fill a part of the merged array
		int j=Size;
		for(i=0;i<(pArr->GetSize());i++)
		{
			newArr->SetAt(i+j,pArr->GetAt(i));
		}
        //Transfer data from the temporary array to our
		ClearAll();
		Size=newArr->GetSize();
		m_pHead=newArr->m_pHead;
        //Correctly delete temporary array
		newArr->m_pHead=NULL;
		newArr->Size=0;
		delete newArr;
};


//=========================================================
//!Deletes the first elements of array
void MStArray::DeleteHead()

//=========================================================
{
    //Create a new array
    MString* ptr=new MString[Size-1];
    //Copy the matching part
    for(int i=1;i<Size;i++)
		Copy(GetAtSt(i-1,ptr),GetAt(i));
    //Delete the old array
    if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
    m_pHead=(void*)ptr;
    //Modify the number of
    Size--;
};

//=========================================================
//!Delete the element that is determined by 'index'
//!wherein the number of elements become less per unit
void MStArray::Delete(int index)
//=========================================================
{
    //If the index is beyond the array,
    //don't do anything
    if (!((index>=0)&&(index<Size)))return;
    //Create a new array
    MString* ptr=new MString[Size-1];
    //Copy the matching part
    int N=index;
	int i = 0;
	for(i=0;i<N;i++)
		Copy(GetAtSt(i,ptr),GetAt(i));
	for(i=N+1;i<Size;i++)
		Copy(GetAtSt(i-1,ptr),GetAt(i));
    //Delete the old array
    if(Size) delete [] (MString*)m_pHead;
    //Set the pointer to a new array
    m_pHead=(void*)ptr;
    //Modify the number of
    Size--;
};

//=========================================================
//!Deletes the last element of array
void MStArray::DeleteTail()
//=========================================================
{
    //Create a new array
    MString* ptr=new MString[Size-1];
    //Copy the matching part
    for(int i=0;i<Size-1;i++)
		Copy(GetAtSt(i,ptr),GetAt(i));
    //Delete the old array
    if(Size) delete[] (MString*)m_pHead;
    //Set the pointer to a new array
    m_pHead=(void*)ptr;
    //Modify the number of
    Size--;
};


//=========================================================
//!Searches for the first element , starting from the position 'index',
//!equal 'element's
//!and returns the index ( or -1 if the element is not found )
int MStArray::Find(int index,MString* Element)
//=========================================================
{
	if(!((index>=0)&&(index<Size))) return -1;
	for(int i=index;i<Size;i++)
	{
		MString* Atom=GetAt(i);
		if(Atom[0]==Element[0])
			return i;
	}
	return -1;
};

//=========================================================
//!Creates an exact copy of the array
MArray* MStArray::Clone()
//=========================================================
{
    //Create a copy
	MStArray* pClone=new MStArray(Size);
    //Copy all elements
	for(int i=0; i<Size;i++)
	{
		pClone->SetAt(i,GetAt(i));
	}
    //Returns a pointer to a copy
	return pClone;
};


//=========================================================
//!Reads array of NUX-file
bool MStArray::ReadBin(FILE* h_nuxfile, int version)
//=========================================================
{
	switch (version)
	{
	case 0:
        //skip the record length
		fseek(h_nuxfile, 3, SEEK_CUR);
        //Read the number of elements
		unsigned int Cnt= 0;
		fread(&Cnt, 4, 1, h_nuxfile);
        if (reverseReadFlag)
        {
            reverseValue((char*)&Cnt, sizeof(Cnt) );
        }
		SetSize(Cnt);   // cout << Cnt<<endl;
        //read string
		MString* pAtom=(MString*)m_pHead;
		for(int i=0;i<Size;i++)
		{      
			pAtom[0]=ReadString(h_nuxfile, version);
			pAtom++;
    }

	}
	return true;
};

//=========================================================
//!Reads array of NUX-file
bool MStArray::ReadBin(MFile* file, int version)
//=========================================================
{
	switch (version)
	{
	case 0:
        //skip the record length
		file->SetPosition(3, MAP_SEEK_CUR);
        //Read the number of elements
		unsigned int Cnt= 0;
		file->GetData(&Cnt, 4, 1);
        if (reverseReadFlag)
        {
            reverseValue((char*)&Cnt, sizeof(Cnt) );
        }
		SetSize(Cnt);
        //read string
		MString* pAtom=(MString*)m_pHead;

		for(int i=0;i<Size;i++)
		{
			pAtom[0]=ReadString(file, version);
            //Reading one string. Code is taken from the former global function
			pAtom++;
		}

	}
	return true;
};

//=========================================================
//!Writes array in NUX- file
bool MStArray::WriteBin(FILE* h_nuxfile,int version)
//=========================================================
{
	switch (version)
	{
	case 0:
        //Write identifier
		fwrite(ID_STA, 3, 1, h_nuxfile);
        //Write the number of elements
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write  strings.
		MString* pAtom=(MString*)m_pHead;
		for(int i=0;i<Size;i++)
		{
			WriteString(h_nuxfile,version,pAtom[0]);
			pAtom++;
		}

	}
	return true;
};


//============================================================
//!Returns a pointer to the 'index'
//!in the array to the first element 'HeAD'
MString* MStArray::GetAtSt(int index,MString* Head)
//============================================================
{
	return Head+index;
};

//============================================================
//!Assigns string In string Out
void MStArray::Copy(MString* In,MString* Out)
//============================================================
{
////	int Len=Out->GetLength();
	In[0]=Out[0];//.Left(Len);
};

//============================================================
//!Replacing two strings of seats
void MStArray::Exchange(int i,int j)
//============================================================
{
	MString str1 = *GetAt(i);
	MString str2 = *GetAt(j);
	SetAt(j,&str1);
	SetAt(i,&str2);
};
