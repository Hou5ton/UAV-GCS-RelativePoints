
// NuxArray.cpp: implementation of the MMMMMM class.
//
#include "Array.h"
#include "SysDefine.h"
#include "Refglobal.h"
extern MArray* (*fpCreateArrayByType)(MString str);
//*********************************************************
//******                   MArArray                  ******
//*********************************************************

//=========================================================
//!Constructor by default
//!creates an empty array
MArArray::MArArray()           
{
    Size=0;//number of elements
    m_pHead=NULL;//nowhere indicates
};

//=========================================================
//!Constructor with a given number of elements
/**creates an array containing 'Cnt' elements
//constructed by default
*/
MArArray::MArArray(int Cnt)
//=========================================================
{
    //If the number of elements equal to zero ,
    //then create an empty array
	if(Cnt<0) Cnt=0;
    //Create array
	if(Cnt)
		m_pHead=(void*)new MArray*[Cnt];
	else 
		m_pHead=NULL;
    //Set the number of elements
	Size=Cnt;
};

//=========================================================
//!Destructor
//!frees the memory
MArArray::~MArArray()
//=========================================================
{
	//ClearAll();//leakDetector
	if(Size)
	{
		for (int i=0;i<Size;i++)
		{
			MArray* pArr=GetAt(i);
			pArr->FreeMemory();
			delete pArr;
		}
	}
    //Making zero setting
	m_pHead=NULL;
	Size=0;
};

//=========================================================
//!Removes all of the elements of the array
void MArArray::ClearAll()
//=========================================================
{
    //Free the memory
	if(Size)
		delete[] ((MArray**)m_pHead);
    //Making zero setting
	m_pHead=NULL;
	Size=0;
}

//===========================================
//!Free the memory
void MArArray::FreeMemory()
//===========================================
{
	for (int i=0;i<Size;i++)
	{
		MArray* pArr=GetAt(i);
		pArr->FreeMemory();
		delete pArr;
                pArr = NULL;//max
	}
	SetSize(0);
}

//==============================================
//!Returns the total size of the array in UTP- format
int MArArray::GetBytes()
//==============================================
{
    int nBytes=7;//3 ID bytes + 4 bytes for the number of elements
	for(int i=0;i<Size;i++)
	{
		MArray* par=GetAt(i);
		nBytes+=par->GetBytes();
	}
	return nBytes;
};


//=========================================================
//!Returns a reference to an element defined by the 'Index'
MArray* MArArray::GetAt(int index) const
//=========================================================
{
    //If the index is outside the array
    //returns a null element
	if(!((index>=0)&&(index<Size))) index=0;
	return ((MArray**)m_pHead)[index];
};



//=========================================================
//!Returns a reference to an element defined by the 'Index'
MArray* MArArray::operator[](int index)
//=========================================================
{
	return GetAt(index);
};

//=========================================================
//!Sets 'Element' in cell 'index'
void MArArray::SetAt(int index,MArray* Element)
//=========================================================
{
    //If the index is outside the array
    //don't do anything
	if((index>=0)&&(index<Size))
	{
		MArray** Atom=GetAtGd(index,(MArray**)m_pHead);
		Atom[0]=Element;
	}
};

//=========================================================
//!Sets a new number of elements
void MArArray::SetSize(int Cnt)
//=========================================================
{
    //If the number is less than zero or equal to the present ,
    //don't do anything
	if((Cnt<0)||(Cnt==Size))return;
	
    //Create a new array

	MArray** ptr = NULL;
	if(Cnt)
		ptr = new MArray*[Cnt];
    //Copy the matching part
	int M=Min(Size,Cnt);
	int i = 0;
	for(i=0;i<M;i++)
		GetAtGd(i,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size=Cnt;
    //Initialize a new part of the array
	for(i=M;i<Size;i++)
		SetAt(i,NULL);
};


//=========================================================
//!Adds 'Element' in the beginning of the array
void MArArray::AddHead(MArray* Element)
//=========================================================
{
    //Create a new array
	MArray** ptr=new MArray*[Size+1];
    //Copy the matching part
	for(int i=0;i<Size;i++)
		GetAtGd(i+1,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Set the first element
	SetAt(0,Element);
}


//=========================================================
//!Attaches array pointed MArr
//!to the beginning of the array
void MArArray::AppendHead(MArArray* pArr)
//=========================================================
{
        //Create a new array
		MArArray* newArr=new MArArray(pArr->GetSize()+Size);
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
//!Inserts an element into the cell , indicates by 'index'
void MArArray::InsertBefore(int index,MArray* Element)
//=========================================================
{
    //If the index is beyond the array,
    //don't do anything
	if (!((index>=0)&&(index<Size)))return;
    //Create a new array
	MArray** ptr=new MArray*[Size+1];
    //Copy the matching part
	int N=index;
	int i = 0;
	for(i=0;i<N;i++)
		(GetAtGd(i,ptr))[0]=GetAt(i);
	for(i=N;i<Size;i++)
		(GetAtGd(i+1,ptr))[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Insert an element
	SetAt(index,Element);
};

//=========================================================
//!Inserts an element 'Element' after the index 'index'
void MArArray::InsertAfter(int index,MArray* Element)
//=========================================================
{
    //If the index is beyond the array,
    //don't do anything
	if (!((index>=0)&&(index<Size)))return;
    //Create a new array
	MArray** ptr=new MArray*[Size+1];
    //Copy the matching part
	int N=index+1;
	int i = 0;
	for(i=0;i<N;i++)
		GetAtGd(i,ptr)[0]=GetAt(i);
	for(i=N;i<Size;i++)
		GetAtGd(i+1,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Insert an element
	SetAt(index+1,Element);
};

//=========================================================
//!Adds 'Element' to the end of the array
void MArArray::AddTail(MArray* Element)
//=========================================================
{
    //Create a new array
	MArray** ptr=new MArray*[Size+1];
    //Copy the matching part
	for(int i=0;i<Size;i++)
		GetAtGd(i,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size++;
    //Set the first element
	SetAt(Size-1,Element);    
};


//=========================================================
//!Add array pointed 'pArr'
//!to the end of array
void MArArray::AppendTail(MArArray* pArr)
//=========================================================
{
        //Create a new array
		MArArray* newArr=new MArArray(pArr->GetSize()+Size);
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
//!Deletes the first element of the array
void MArArray::DeleteHead()
//=========================================================
{
    //Create a new array
	MArray** ptr = NULL;
	if(Size-1)
		ptr = new MArray*[Size-1];
    //Copy the matching part
	for(int i=1;i<Size;i++)
		GetAtGd(i-1,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size--;
};

//=========================================================
//!Remove the element which is determined by the index 'index',
//!with the number of elements become less per unit
void MArArray::Delete(int index)
//=========================================================
{
    //If the index is beyond the array
    //don't do anything
    if (!((index>=0)&&(index<Size)))return;
    //Create a new array
	MArray** ptr = NULL;
	if(Size-1)
		ptr = new MArray*[Size-1];
    //Copy the matching part
	int N=index;
	int i = 0;
	for(i=0;i<N;i++)
		GetAtGd(i,ptr)[0]=GetAt(i);
	for(i=N+1;i<Size;i++)
		GetAtGd(i-1,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size--;
};

//=========================================================
//!Deletes the last element of the array
void MArArray::DeleteTail()
//=========================================================
{
    //Create a new array
	MArray** ptr= NULL;
	if(Size-1)
		ptr = new MArray*[Size-1];
    //Copy the matching part
	for(int i=0;i<Size-1;i++)
		GetAtGd(i,ptr)[0]=GetAt(i);
    //Delete the old array
	if(Size) delete[] (MArray**)m_pHead;
    //Set the pointer to a new array
	m_pHead=(void*)ptr;
    //Modify the number of
	Size--;
};


//=========================================================
//!Searches for the first element, starting from the position 'index',
//!equal 'Element'
//!and returns the index ( or -1 if the element is not found )
int MArArray::Find(int index,MArray* Element)
//=========================================================
{
	if(!((index>=0)&&(index<Size))) return -1;
	for(int i=index;i<Size;i++)
	{
		MArray* Atom=GetAt(i);
		if(Atom==Element)
			return i;
	}
	return -1;
};

//=========================================================
//!Creates an exact copy of the array
MArray* MArArray::Clone()
//=========================================================
{
    //Create a copy
	MArArray* pClone=new MArArray(Size);
    //Copy all elements
	for(int i=0; i<Size;i++)
	{
		pClone->SetAt(i,GetAt(i)->Clone());
	}
    //Returns a pointer to a copy
	return pClone;
};


//=========================================================
//!Reads array of NUX-file
bool MArArray::ReadBin(FILE* h_nuxfile, int version)
//=========================================================
{
	char id_array[]="   ";
////	char id_each[]="   ";
	switch (version)
	{
	case 0:
        //Skip the record length
		fseek(h_nuxfile, 3, SEEK_CUR);
        //Read the number of elements
		unsigned int Cnt= 0;
		fread(&Cnt, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //Read the information on points
		MArray** pAtom=(MArray**)m_pHead;  // cout <<endl;
		for(int i=0;i<Size;i++)
		{
			fread(&id_array, 3, 1, h_nuxfile);  // cout<<" ++ "<< id_array;
			fseek(h_nuxfile, -3, SEEK_CUR);
            //Depending on the type of array
			MArray* pArr=(*fpCreateArrayByType)(id_array);
			pArr->ReadBin(h_nuxfile, version);
    	pAtom[0]=pArr;
      pAtom++;
		}

	}
	return true;
};

//=========================================================
//!Reads array of NUX-file
bool MArArray::ReadBin(MFile* file, int version)
//=========================================================
{
	char id_array[]="   ";
////	char id_each[]="   ";
	switch (version)
	{
	case 0:
        //Skip the record length
		file->SetPosition(3, MAP_SEEK_CUR);
        //Read the number of elements
		unsigned int Cnt= 0;
		file->GetData(&Cnt, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //Read the information on points
		MArray** pAtom=(MArray**)m_pHead;  
		for(int i=0;i<Size;i++)
		{
			file->GetData(&id_array, 3, 1);     
			file->SetPosition(-3, MAP_SEEK_CUR);
            //Depending on the type of array
			MArray* pArr=(*fpCreateArrayByType)(id_array);
			pArr->ReadBin(file, version);
    
			pAtom[0]=pArr;
			pAtom++;
    }

	}
	return true;
};

//=========================================================
//!Writes array NUX-file
bool MArArray::WriteBin(FILE* h_nuxfile,int version)
//=========================================================
{
	switch (version)
	{
	case 0:
        //write identifier
		fwrite(ID_GDA, 3, 1, h_nuxfile);
        //Write the number of elements
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write the information on points
		MArray** pAtom=(MArray**)m_pHead;
		for(int i=0;i<Size;i++)
		{
			pAtom[0]->WriteBin(h_nuxfile,version);
			pAtom++;
		}

	}
	return true;
};

//============================================================
//!Returns a pointer to the 'index'
//!in the array to the first element HeAD
MArray** MArArray::GetAtGd(int index,MArray** Head)
//============================================================
{
	return Head+index;
};
