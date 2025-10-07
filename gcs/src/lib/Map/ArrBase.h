//PACKAGE: MAP
//FILE: ArrBase.h
//AUTHOR:
//DESCRIPTION: 

#if !defined(M_ARRBASE_H)
#define M_ARRBASE_H

#include <malloc.h>
#include "BaseClId.h"
#include "SysDefine.h" 
#include "MString.h"

extern  bool  reverseReadFlag;
extern  bool  reverseValue(char* , int);
class MArray;
extern MArray*  CreateArrayByType(MString strType);
//PACKAGE: MAP
//CLASS	: MArray
//DESCRIPTION : Class ia a parent for all arrays.
class  MArray
{
public:

    int Size;     // Elements quantity of array
    int reserveSize;

    void* m_pHead{nullptr};// Pointer to null element of array

    //PACKAGE: MAP.
    //FUNCTION: MArray::MArray();
    //DESCRIPTION: Creates an empty array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    MArray(){}

    //PACKAGE: MAP.
    //FUNCTION: MArray::MArray(int);
    //DESCRIPTION: Creates an array with 'Cnt' elements of random contents.
    //INPUTS: Int.
    //RETURNS: NONE.
    MArray(int){} //int Cnt

    //PACKAGE: MAP.
    //FUNCTION: MArray::~MArray();
    //DESCRIPTION: Destructor.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual ~MArray(){}

    //PACKAGE: MAP.
    //FUNCTION: MArray::ClearAll();
    //DESCRIPTION: Deletes all data from array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual void ClearAll(){}

    //PACKAGE: MAP.
    //FUNCTION: MArray::FreeMemory();
    //DESCRIPTION: Frees memory.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual void FreeMemory(){}

    // void GetAt(int index){};      // Returns element
    // void operator[](int index){}; // by index
    // void SetAt(int index){};// Records element into the given cell

    //PACKAGE: MAP.
    //FUNCTION: MArray::GetSize();
    //DESCRIPTION: Returns quantity of array elements.
    //INPUTS: NONE.
    //RETURNS: Int .
    int GetSize() const{return Size;}

    // void SetCount(int Cnt){};// Sets number of array elements

    // void AddHead(){};// Adds element to the head
    // void AddHead(MArray* MArr){};// Adds array to the head
    // void InsertBefore(int index){}; // Adds element before index
    // void InsertAfter(int index){}; // Adds element after index
    // void AddTail(){};// Adds element to the end
    // void AddTail(MArray* MArr){};// Adds array to the tail

    // void DeleteHead(){};     // Removes the head
    // void Delete(int index){};// Removes the index
    // void DeleteTail(){};     // Removes the tale

    //PACKAGE: MAP.
    //FUNCTION: MArray::Clone();
    //DESCRIPTION: Creates copy.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* Clone(){return NULL;}

    //PACKAGE: MAP.
    //FUNCTION: MArray::ReadBin(FILE*, int);
    //DESCRIPTION: Reads from UTP-file (FILE* h_nuxfile, int version).
    //INPUTS: FILE pointer, int.
    //RETURNS: True.
    virtual bool ReadBin(FILE*, int) {return true;}

    //PACKAGE: MAP.
    //FUNCTION: MArray::ReadBin(FILE*, int);
    //DESCRIPTION: Reads from UTP-file (FILE* h_nuxfile, int version).
    //INPUTS: MFile pointer, int.
    //RETURNS: True.
    virtual bool ReadBin(MFile*, int) {return true;}

    //PACKAGE: MAP.
    //FUNCTION: MArray::WriteBin(FILE*, int);
    //DESCRIPTION: Saves to UTP-file (FILE* h_nuxfile, int version).
    //INPUTS: FILE pointer, int.
    //RETURNS: True.
    virtual bool WriteBin(FILE*, int) {return true;}

    //PACKAGE: MAP.
    //FUNCTION: MArray::WhatTheArray();
    //DESCRIPTION: For child - returns the type of array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return "";}

    //PACKAGE: MAP.
    //FUNCTION: MArray::GetBytes();
    //DESCRIPTION: Returns size of array in a bytes for UTP.
    //INPUTS: NONE.
    //RETURNS: int.
    virtual int GetBytes(){return 0;}

    //Following functions are defined for using undefined function.

    //PACKAGE: MAP.
    //FUNCTION: MArray::GetAtVoid(int);
    //DESCRIPTION:
    //INPUTS: int.
    //RETURNS: void pointer.
    virtual void* GetAtVoid(int){return NULL;} // int i


    //PACKAGE: MAP.
    //FUNCTION: MArray::Exchange(int ,int );
    //DESCRIPTION:
    //INPUTS: int , int.
    //RETURNS: NONE.
    virtual void Exchange(int ,int ){}         // int i, int j


    //PACKAGE: MAP.
    //FUNCTION: MArray::Delete(int );
    //DESCRIPTION:
    //INPUTS: int.
    //RETURNS: NONE.
    virtual void Delete(int ){}                // int i


    //PACKAGE: MAP.
    //FUNCTION: MArray::SetAtVoid(int ,void* );
    //DESCRIPTION:
    //INPUTS: int, void pointer.
    //RETURNS: NONE.
    virtual void SetAtVoid(int ,void* ){}      // int i, void* pEl


    //PACKAGE: MAP.
    //FUNCTION: MArray::SetSize(int );
    //DESCRIPTION:
    //INPUTS: int.
    //RETURNS: NONE.
    virtual void SetSize(int ){}               // int i
};



//PACKAGE : MAP.
//TEMPLATE CLASS : GlobalArray:public MArray.
//DESCRIPTION : This template is intended for SIMPLE classes, which can be described as structures. For the class contained in array we need operator =.
template<class Record> class GlobalArray:public MArray
{
protected:
    // Class members, responsibles for the size of one element storing
    //	unsigned short int m_iRecordSize;   // vipa 2004.10.28
    unsigned short  m_iRecordSize;
public:


    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::GlobalArray();
    //DESCRIPTION : Constructor by default.
    //INPUTS: NONE.
    //RETURNS: NONE.
    GlobalArray()
    {
        Size = 0;
        m_pHead = NULL;
        m_iRecordSize = sizeof(Record);
    }
    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::GlobalArray();
    //DESCRIPTION : Constructor by default.
    //INPUTS: NONE.
    //RETURNS: NONE.
    GlobalArray(std::vector<Record> vect)
    {
        m_iRecordSize = sizeof(Record);

        m_pHead= realloc(m_pHead,m_iRecordSize*(vect.size()));

        //std::copy(vect.begin(), vect.end(), data_x);
        memcpy(m_pHead,&vect[0],m_iRecordSize*vect.size());


    };
    //FUNCTION : GlobalArray::~GlobalArray();
    //DESCRIPTION : Destructor.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual ~GlobalArray()
    {
        free(m_pHead);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::GetAt(int iIndex);
    //DESCRIPTION : Returns pointer to element by index 'index'.
    //INPUTS: int .
    //RETURNS: Record pointer.
    Record* GetAt(int iIndex) const
    {
        // If index is out of range
        // than return null element
        if(!((iIndex>=0)&&(iIndex<Size))) iIndex=0;
        return (Record*)(((Record*)m_pHead)+iIndex);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::AddHead(Record* pRec);
    //DESCRIPTION : Adds element to the beginning.
    //INPUTS: Record pointer.
    //RETURNS: NONE.
    void AddHead(Record* pRec)
    {
        // Creates new array
        m_pHead = realloc(m_pHead,m_iRecordSize*(++Size));
        // Copies coincident part
        memmove((void*)((Record*)m_pHead+1),m_pHead,m_iRecordSize*(Size-1));
        // Sets first element
        SetAt(0,pRec);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::AddTail(Record* pRec);
    //DESCRIPTION : Adds element to the end.
    //INPUTS: Record pointer.
    //RETURNS: NONE.
    void AddTail(Record* pRec)
    {
        m_pHead = realloc(m_pHead, m_iRecordSize*(++Size));
        // Sets last element
        SetAt(Size - 1,pRec);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::AppendHead(GlobalArray<Record>* pArr);
    //DESCRIPTION : Assigns array.
    //INPUTS: GlobalArray<Record> pointer.
    //RETURNS: NONE.
    void AppendHead(GlobalArray<Record>* pArr)
    {
        m_pHead = realloc(m_pHead,m_iRecordSize*(Size+pArr->Size));
        memmove((void*)((Record*)m_pHead+pArr->Size),m_pHead,Size*m_iRecordSize);
        memcpy((void*)m_pHead,pArr->m_pHead,pArr->Size*m_iRecordSize);
        Size+=pArr->Size;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::AppendTail(GlobalArray<Record>* pArr;
    //DESCRIPTION : Adds to the end.
    //INPUTS: GlobalArray<Record> pointer.
    //RETURNS: NONE.
    void AppendTail(GlobalArray<Record>* pArr)
    {
        m_pHead = realloc(m_pHead, m_iRecordSize*(Size+pArr->Size));
        memcpy(static_cast<void*>(static_cast<Record*>(m_pHead) + Size),pArr->m_pHead,pArr->Size*m_iRecordSize);
        //        memcpy((void*)((Record*)m_pHead+Size),pArr->m_pHead,pArr->Size*m_iRecordSize);
        Size+=pArr->Size;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::ClearAll();
    //DESCRIPTION : Clears array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void ClearAll()
    {
        free(m_pHead);
        m_pHead = NULL;
        Size = 0;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::CreateOfType();
    //DESCRIPTION : For the child we redefine this function.
    //				The function also creates an array of the same type, as it is.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType()=0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalArray::Clone();
    //DESCRIPTION : Makes a clone of array.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    MArray* Clone()
    {
        // Makes Copy
        MArray* pClone=CreateOfType();// (GlobalArray*)CreateArrayByType(WhatTheArray());
        pClone->SetSize(Size);

        // Copies all elements
        memcpy(pClone->m_pHead,m_pHead,m_iRecordSize*Size);

        // Returns pointer to the clone
        return pClone;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::Delete(int iIndex);
    //DESCRIPTION : Removes element.
    //INPUTS: int .
    //RETURNS: NONE.
    void Delete(int iIndex)
    {
        // If the index falls out the limits of array than doing nothing
        if (!((iIndex>=0)&&(iIndex<Size)))return;

        // Creates new array
        Record* ptr = (Record*)malloc(m_iRecordSize*(Size-1));

        // Copies coincident part
        memcpy((void*)ptr,m_pHead,m_iRecordSize*iIndex);
        memcpy((void*)(ptr+iIndex),(void*)((Record*)m_pHead+iIndex+1),m_iRecordSize*(Size-iIndex-1));
        free(m_pHead);
        m_pHead=(void*)ptr;

        // Modifies quantity
        Size--;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::DeleteHead();
    //DESCRIPTION : Deletes element from the head.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteHead()
    {
        // Create new array
        Record* ptr=(Record*)malloc(m_iRecordSize*(Size-1));
        // Copies coincident part
        memcpy((void*)ptr,(void*)((Record*)m_pHead+1),m_iRecordSize*(Size-1));
        // Deletes old array
        free(m_pHead);
        // Sets pointer to new array
        m_pHead=(void*)ptr;
        // Modifies quantity
        Size--;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::DeleteTail();
    //DESCRIPTION : Deletes element from the tale.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteTail()
    {
        m_pHead = realloc(m_pHead,m_iRecordSize*(Size--));
    };

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::Find(int iIndex, Record* pElement);
    //DESCRIPTION : Searchs for element.
    //INPUTS: int , Record pointer.
    //RETURNS: Int.
    int Find(int iIndex, Record* pElement)
    {
        if(!((iIndex>=0)&&(iIndex<Size))) return -1;
        for(int i=iIndex;i<Size;i++)
        {
            Record* Atom=GetAt(i);
            if(!memcmp(pElement,Atom,m_iRecordSize))
                return i;
        }
        return -1;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::FreeMemory();
    //DESCRIPTION : Frees memory.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void FreeMemory()
    {
        ClearAll();
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::operator[](int iIndex);
    //DESCRIPTION : Gets element.
    //INPUTS: int .
    //RETURNS: Record pointer.
    Record* operator[](int iIndex)
    {
        return GetAt(iIndex);
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::GetBytes();
    //DESCRIPTION : Gets total size of array.
    //INPUTS: NONE.
    //RETURNS: int.
    virtual int GetBytes()
    {
        int nBytes=7;// 3 bytes for identifier + 4 bytes for quantity of elements
        nBytes+=Size*m_iRecordSize;// Every element saves in 'sizeof(Record)' byte
        return nBytes;
    }


    //PACKAGE: MAP
    //FUNCTION : GlobalArray::InsertAfter(int iIndex,Record* pRec);
    //DESCRIPTION : Inserts in the array after element with number 'iIndex'.
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void InsertAfter(int iIndex,Record* pRec)
    {
        // If the index falls out the limits of array than doing nothing
        if (!((iIndex>=0)&&(iIndex<Size)))
            return;
        //  reates new array
        m_pHead = realloc(m_pHead,m_iRecordSize*(++Size));

        // Copies coincident part
        memmove((void*)((Record*)m_pHead+iIndex+2),(void*)((Record*)m_pHead+iIndex+1),(Size-iIndex-2)*m_iRecordSize);
        // Inserts element
        SetAt(iIndex+1,pRec);
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::InsertBefore(int iIndex,Record* pRec);
    //DESCRIPTION : Inserts in array before the element with number of 'iIndex'.
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void InsertBefore(int iIndex,Record* pRec)
    {
        // If the index falls out the limits of array than doing nothing
        if (!((iIndex>=0)&&(iIndex<Size)))return;
        // Creates new array
        m_pHead = realloc(m_pHead,m_iRecordSize*(++Size));
        // Copies coincident part
        memmove((void*)((Record*)m_pHead+iIndex+1),(void*)((Record*)m_pHead+iIndex),m_iRecordSize*(Size-iIndex-1));

        // Inserts an element
        SetAt(iIndex,pRec);
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::SetAt(int iIndex,Record* pRec);
    //DESCRIPTION : Sets element by index 'iIndex' with value, defined by pRec.
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void SetAt(int iIndex,Record* pRec)
    {
        // If the index falls out the limits of array than doing nothing
        if((iIndex>=0)&&(iIndex<Size))
        {
            Record* Atom=GetAt(iIndex);
            memcpy(Atom,pRec,m_iRecordSize);
        }
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::CircleMove(int iIndex);
    //DESCRIPTION : Shifts array elements;
    //		Element with index 'iIndex' becomes a null element,
    //		Accordingly another elements cyclically move..
    //INPUTS: int .
    //RETURNS: NONE.
    void CircleMove(int iIndex)
    {
        // If the index falls out the limits of array than doing nothing
        if (!((iIndex>=0)&&(iIndex<Size)))return;
        // Creates new array
        Record* ptr=(Record*)malloc(m_iRecordSize*(Size));
        // Copies the beginning part start with element of 'iIndex' till the end of array
        memcpy((void*)ptr,(void*)((Record*)m_pHead+iIndex),m_iRecordSize*(Size-iIndex));
        // Copies the tail part start with  element 0 index till element with the index 'iIndex-1'
        memcpy((void*)(ptr + Size - iIndex),(void*)((Record*)m_pHead),m_iRecordSize*(iIndex));
        // Deletes old array
        free(m_pHead);
        // Sets pointer to new array
        m_pHead=(void*)ptr;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::ReadBin(FILE* h_nuxfile,int version);
    //DESCRIPTION : Reads array from 'UTP - h_nuxfile' format file.
    //INPUTS: FILE pointer,int .
    //RETURNS: True.
    virtual bool ReadBin(FILE* h_nuxfile,int version)
    {
        char id_array[]="   ";
        switch (version)
        {
        case 0:
            // Skips record length
            //		fseek(h_nuxfile, 3, SEEK_CUR);
            fread(&id_array, 3, 1, h_nuxfile); //   cout<<" __ "<< id_array<< endl;
            // Reads elements quantity
            unsigned int Cnt=0;
            fread(&Cnt, 4, 1, h_nuxfile);
            if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
            SetSize(Cnt);
            // Reads data
            fread(m_pHead, m_iRecordSize, Cnt, h_nuxfile);

            if (reverseReadFlag)
            {
                if (MString(id_array)==ID_PTA)
                {
                    MPoint *pnt = (MPoint*) m_pHead;
                    for(unsigned int k=0; k< Cnt; k++)
                    {
                        reverseValue((char*)&pnt->x, sizeof(pnt->x) );
                        reverseValue((char*)&pnt->y, sizeof(pnt->y) );
                        pnt++;
                    }
                }
                else
                {
                    if (MString(id_array)==ID_INA)
                    {
                        int *pnt = (int*) m_pHead;
                        for(unsigned int k=0; k< Cnt; k++)
                        {
                            reverseValue((char*)pnt, sizeof(int) );
                            pnt++;
                        }
                    }
                    else
                    {
                        if (MString(id_array)==ID_DUA)
                        {
                            double *pnt = (double*) m_pHead;
                            for(unsigned int k=0; k< Cnt; k++)
                            {
                                reverseValue((char*)pnt, sizeof(double) );
                                pnt++;
                            }
                        }
                        else
                        {
                            if (MString(id_array)==ID_USA)
                            {
                                short *pnt = (short*) m_pHead;
                                for(unsigned int k=0; k< Cnt; k++)
                                {
                                    reverseValue((char*)pnt, sizeof(short) );
                                    pnt++;
                                }
                            }
                            else
                            {
                                if (MString(id_array)==ID_LFA)
                                {
                                    short *pnt = (short*) m_pHead;
                                    for(unsigned int k=0; k< Cnt; k++)  {
                                        reverseValue((char*)pnt, sizeof(short) );
                                        pnt++;
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
        return true;

    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::ReadBin(MFile* file,int version);
    //DESCRIPTION : Reads array from UTP-format file.
    //INPUTS: MFile pointer,int .
    //RETURNS: True.
    virtual bool ReadBin(MFile* file,int version)
    {
        char id_array[]="   ";
        switch (version)
        {
        case 0:
            // Skips record length
            // 		file->SetPosition(3, MAP_SEEK_CUR);
            file->GetData(&id_array, 3, 1);      //  cout<<" _M "<< id_array<< endl;
            // Reads elements quantity
            unsigned int Cnt=0;
            file->GetData(&Cnt, 4, 1);
            if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
            SetSize(Cnt);
            // Reads data
            file->GetData(m_pHead, m_iRecordSize, Cnt);
            //    cout << "MFile* GlobalArray::ReadBin"<<endl;

            if (reverseReadFlag)
            {
                if (MString(id_array)==ID_PTA)
                {
                    MPoint *pnt = (MPoint*) m_pHead;
                    for(unsigned int k=0; k< Cnt; k++)
                    {
                        reverseValue((char*)&pnt->x, sizeof(pnt->x) );
                        reverseValue((char*)&pnt->y, sizeof(pnt->y) );
                        pnt++;
                    }
                }
                else
                {
                    if (MString(id_array)==ID_INA)
                    {
                        int *pnt = (int*) m_pHead;
                        for(unsigned int k=0; k< Cnt; k++)
                        {
                            reverseValue((char*)pnt, sizeof(int) );
                            pnt++;
                        }
                    }
                    else
                    {
                        if (MString(id_array)==ID_DUA)
                        {
                            double *pnt = (double*) m_pHead;
                            for(unsigned int k=0; k< Cnt; k++)
                            {
                                reverseValue((char*)pnt, sizeof(double) );
                                pnt++;
                            }
                        }
                        else
                        {
                            if (MString(id_array)==ID_USA)
                            {
                                short *pnt = (short*) m_pHead;
                                for(unsigned int k=0; k< Cnt; k++)
                                {
                                    reverseValue((char*)pnt, sizeof(short) );
                                    pnt++;
                                }
                            }
                            else
                            {
                                if (MString(id_array)==ID_LFA)
                                {
                                    short *pnt = (short*) m_pHead;
                                    for(unsigned int k=0; k< Cnt; k++)
                                    {
                                        reverseValue((char*)pnt, sizeof(short) );
                                        pnt++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return true;
    }
    //PACKAGE: MAP
    //FUNCTION : GlobalArray::WriteBin(FILE* h_nuxfile,int version);
    //DESCRIPTION : Saves array to 'UTP - h_nuxfile'format file.
    //INPUTS: FILE pointer,int .
    //RETURNS: True.
    bool WriteBin(FILE* h_nuxfile,int version)
    {
        switch (version)
        {
        case 0:
            MString str = WhatTheArray();
            // Saves identifier
            fwrite((char *)str, 3, 1, h_nuxfile);
            // Saves elements quantity
            fwrite(&Size, 4, 1, h_nuxfile);
            // Saves information of points
            fwrite(m_pHead, m_iRecordSize, Size, h_nuxfile);
        }
        return true;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::SetSize(int Cnt);
    //DESCRIPTION : Sets array size.
    //INPUTS: int.
    //RETURNS: NONE.
    void SetSize(int Cnt)
    {
        if((Cnt == Size)||(Cnt<0))
            return;
        m_pHead = realloc(m_pHead,m_iRecordSize*Cnt);
        Size = Cnt;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::WhatTheArray();
    //DESCRIPTION : Identifies array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray()
    {
        return "";
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::GetAtVoid(int i);
    //DESCRIPTION : Gets void pointer to element.
    //INPUTS: int.
    //RETURNS: void pointer.
    virtual void* GetAtVoid(int i)
    {
        return (void*)GetAt(i);
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::SetAtVoid(int i,void* pEl);
    //DESCRIPTION : Sets element from void pointer.
    //INPUTS: int ,void pointer.
    //RETURNS: NONE.
    virtual void SetAtVoid(int i,void* pEl)
    {
        SetAt(i,(Record*)pEl);
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::Exchange(int i,int j);
    //DESCRIPTION : Swaps two elements with indexes i & j.
    //INPUTS: int, int.
    //RETURNS: NONE.
    virtual void Exchange(int i,int j)
    {
        Record* temp = (Record*)malloc(m_iRecordSize);
        memcpy((void*)temp,GetAtVoid(i),m_iRecordSize);
        memcpy(GetAtVoid(i),GetAtVoid(j),m_iRecordSize);
        memcpy(GetAtVoid(j),(void*)temp,m_iRecordSize);
        free(temp);
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::GetHead();
    //DESCRIPTION : Gets head (pointer to the first element of array).
    //INPUTS: NONE.
    //RETURNS: Record pointer.
    virtual Record* GetHead()
    {
        return (Record*)m_pHead;
    }

    //PACKAGE: MAP
    //FUNCTION : GlobalArray::GetTail();
    //DESCRIPTION : Gets tail (pointer to the last element of array).
    //INPUTS: NONE.
    //RETURNS: Record pointer.
    virtual Record* GetTail()
    {
        if(Size == 0) return (Record*)NULL;
        return GetAt(Size-1);
    }

    /*
// Operator of assignment
GlobalArray<Record>& operator =(GlobalArray<Record>& Src )
{
    Size=Src.Size;
    m_pHead = realloc(m_pHead,Size*m_iRecordSize);
    memcpy(m_pHead,Src.m_pHead,m_iRecordSize*Size);
    return *this;
};
*/

};// EoTEMPLATE CLASS GlobalArray:public MArray


//PACKAGE: MAP.
//TEMPLATE CLASS : GlobalComplexArray:public MArray.
//DESCRIPTION : This template is destined for COMPOUND classes,	which have POINTERS inside.
//		For class, contained in array it is  necessary to define:
//		operators = , == , >> ,<< ,
//		also GetLength() function.
template<class Record>
class GlobalComplexArray : public MArray
{

protected:
public:

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::GlobalComplexArray();
    //DESCRIPTION : Constructor by default. Creates an empty array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    GlobalComplexArray()
    {
        Size = 0;          // Quantity of elements
        reserveSize = 0;   // Allocated memory
        m_pHead = NULL;    // Directs to nowhere
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::~GlobalComplexArray();
    //DESCRIPTION : Destructor.
    //INPUTS: NONE.
    //RETURNS: NONE.
    ~GlobalComplexArray()
    {
        ClearAll();
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::ClearAll();
    //DESCRIPTION : Deletes all elements from array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void ClearAll()
    {
        // Frees memory
        if(reserveSize && m_pHead)
        {
            delete[] ((Record*)m_pHead);
        }

        // Makes null settings
        m_pHead = NULL;
        Size = 0;
        reserveSize = 0;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::GetBytes();
    //DESCRIPTION : Returns full size of array to the UTP-format file in a bytes.
    //INPUTS: NONE.
    //RETURNS: int .
    virtual int GetBytes()
    {
        int nBytes = 7;// 3 bytes for identifier + 4 bytes for elements quantity

        for(int i = 0; i < Size; ++i)
        {
            Record* str = GetAt(i);
            nBytes += str->GetBytes();
        }

        return nBytes;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::GetAt(int index);
    //DESCRIPTION : Returns pointer to element, defined by index 'Index'.
    //INPUTS: int .
    //RETURNS: Record pointer.
    Record* GetAt(const int &index)
    {
        return (((Record*)m_pHead) +
                index * ((index >= 0) && (index < Size)));
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::operator[](int index);
    //DESCRIPTION : Returns pointer to element, defined by index 'Index'.
    //INPUTS: int .
    //RETURNS: int .
    Record* operator[](const int &index)
    {
        return GetAt(index);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::SetAt(int index,Record* Element);
    //DESCRIPTION : Sets Element into a cell 'index'.
    //INPUTS: int , Record pointer.
    //RETURNS: NONE.
    void SetAt(const int &index, Record* Element)
    {
        // If the index falls out the limits of array than doing nothing
        if((index >= 0) && (index < Size))
        {
            Record* Atom=GetAt(index);
            Copy(Atom, Element);
        }
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::SetSize(int Cnt);
    //DESCRIPTION : Sets new quantity of elements.
    //INPUTS: int.
    //RETURNS: NONE.
    void SetSize(int Cnt)
    {
        if((Cnt < 0) || (Cnt == Size))
        {
            return;
        }

        if(reserveSize < Cnt)
        {
            reserveSize = Cnt;

            Record* ptr = nullptr;

            if(reserveSize)
            {
                ptr = new Record[reserveSize];
            }

            for(int i = 0; i < Min(Size, Cnt); ++i)
            {
                Record* pAtom = GetAtRec(i, ptr);
                Copy(pAtom, GetAt(i));
            }

            if(Size)
            {
                delete[] (Record*)m_pHead;
            }

            m_pHead = (void*)ptr;
        }

        Size = Cnt;
        return;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::ExpandForOne()
    //DESCRIPTION : Expand the array to add one element.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void ExpandToAddOne()
    {
        if(Size < reserveSize)
        {
            return;
        }

        reserveSize = (Size ? reserveSize * 2 : 1);

        Record* ptr = nullptr;

        if(reserveSize)
        {
            ptr = new Record[reserveSize];
        }

        for(int i = 0; i < Size; ++i)
        {
            Record* pAtom = GetAtRec(i, ptr);
            Copy(pAtom, GetAt(i));
        }

        if(Size)
        {
            delete[] (Record*)m_pHead;
        }

        m_pHead = (void*)ptr;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::AddHead(Record* Element);
    //DESCRIPTION : Adds Element to the beginning of array.
    //INPUTS: Record pointer.
    //RETURNS: NONE.
    void AddHead(Record* Element)
    {
        ExpandToAddOne();
        SetSize(Size + 1);

        for(int i = Size - 1; i >= 1; --i)
        {
            std::swap(((Record*)m_pHead)[i],
                      ((Record*)m_pHead)[i - 1]);
        }

        SetAt(0, Element);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::AppendHead(GlobalComplexArray* pArr);
    //DESCRIPTION : Assigns array, on which one indicates MArr, to the beginning of array.
    //INPUTS: GlobalComplexArray pointer.
    //RETURNS: NONE.
    void AppendHead(GlobalComplexArray* pArr)
    {
        // Creates new array
        GlobalComplexArray* newArr = dynamic_cast<GlobalComplexArray*>(CreateArrayByType(WhatTheArray()));

        newArr->SetSize(pArr->GetSize() + Size);

        int i = pArr->GetSize() - 1;

        for(; i > - 1; --i)
        {
            newArr->SetAt(i, pArr->GetAt(i));
        }

        int j = pArr->GetSize();

        for(i = 0; i < Size; ++i)
        {
            newArr->SetAt(i + j, GetAt(i));
        }


        ClearAll();

        Size = newArr->GetSize();
        reserveSize = Size;

        m_pHead = newArr->m_pHead;

        newArr->m_pHead=NULL;
        newArr->Size=0;
        newArr->reserveSize = 0;

        delete newArr;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::InsertBefore(int index,Record* Element);
    //DESCRIPTION : Inserts element into a cell, defined by index 'index'.
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void InsertBefore(const int &index, Record* Element)
    {
        if (!((index >= 0) && (index < Size)))
        {
            return;
        }

        ExpandToAddOne();
        SetSize(Size + 1);

        for(int i = Size - 1; i >= index + 1; --i)
        {
            std::swap(((Record*)m_pHead)[i],
                      ((Record*)m_pHead)[i - 1]);
        }

        SetAt(index, Element);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::IsEqualTo(GlobalComplexArray *m_pArrToCompare);
    //DESCRIPTION : Comparison of two strings arrays.
    //INPUTS: GlobalComplexArray pointer.
    //RETURNS: True.
    bool IsEqualTo(GlobalComplexArray *m_pArrToCompare)
    {
        if(Size != m_pArrToCompare->GetSize())
        {
            return false;
        }

        // Compares line by line
        for(int i = 0; i < Size; ++i)
        {
            if(!(GetAt(i)->IsEqualTo(m_pArrToCompare->GetAt(i))))
            {
                return false;
            }
        }

        return true;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::InsertAfter(int index,Record* Element);
    //DESCRIPTION : Inserts element 'Element' after index 'index'.
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void InsertAfter(const int &index, Record* Element)
    {
        // If the index falls out the limits of array,
        // than doing nothing
        if(!((index >= 0) && (index < Size)))
        {
            return;
        }

        ExpandToAddOne();
        SetSize(Size + 1);

        for(int i = Size - 1; i > index + 1; --i)
        {
            std::swap(((Record*)m_pHead)[i],
                      ((Record*)m_pHead)[i - 1]);
        }

        SetAt(index + 1, Element);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::AddTail(Record* Element);
    //DESCRIPTION : Adds Element to the end of array.
    //INPUTS: Record pointer.
    //RETURNS: NONE.
    void AddTail(Record* Element)
    {
        ExpandToAddOne();
        SetSize(Size + 1);
        SetAt(Size - 1, Element);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::AppendTail(GlobalComplexArray* pArr);
    //DESCRIPTION : Adds an array, on which one indicates pArr, to the end of array.
    //INPUTS: GlobalComplexArray pointer.
    //RETURNS: NONE.
    void AppendTail(GlobalComplexArray* pArr)
    {
        GlobalComplexArray* newArr = (GlobalComplexArray<Record>*)CreateOfType();
        newArr->SetSize(pArr->GetSize() + Size);

        int i = Size - 1;

        for(; i > -1; --i)
        {
            newArr->SetAt(i, GetAt(i));
        }

        int j = Size;

        for(i = 0; i < (pArr->GetSize()); ++i)
        {
            newArr->SetAt(i + j, pArr->GetAt(i));
        }

        ClearAll();

        Size = newArr->GetSize();
        m_pHead = newArr->m_pHead;

        newArr->m_pHead = NULL;
        newArr->Size = 0;

        delete newArr;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::DeleteHead();
    //DESCRIPTION : Deletes first element of array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteHead()
    {
        for(int i = 0; i < Size - 1; ++i)
        {
            std::swap(((Record*)m_pHead)[i],
                      ((Record*)m_pHead)[i + 1]);
        }

        SetSize(Size - 1);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::Delete(int index);
    //DESCRIPTION : Deletes element, defined by index 'index' thus quantity of elements is decreased by 1.
    //INPUTS: int .
    //RETURNS: NONE.
    void Delete(int index)
    {
        for(int i = index; i < Size - 1; ++i)
        {
            std::swap(((Record*)m_pHead)[i],
                      ((Record*)m_pHead)[i + 1]);
        }

        SetSize(Size - 1);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::DeleteTail();
    //DESCRIPTION : Deletes last element from the array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteTail()
    {
        SetSize(Size - 1);
    }


    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::Exchange(int i,int j);
    //DESCRIPTION : Swap of two elements. As it exists a possibility of a problem occurrence
    // 		in one array, therefore copies through 2 varibles.
    //INPUTS: int ,int .
    //RETURNS: NONE.
    virtual void Exchange(int i,int j)
    {
        Record* temp = new Record;
        Record* temp2 = new Record;

        Copy(temp,(Record*)GetAtVoid(i));
        Copy(temp2,(Record*)GetAtVoid(j));

        SetAt(j,temp);
        SetAt(i,temp2);

        delete temp;
        delete temp2;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::Find(int index,Record* Element);
    //DESCRIPTION : Searches for the first element, starts with the position of index equal Element
    // 		also returns it's index(or -1 if such element was not found).
    //INPUTS: int ,Record pointer.
    //RETURNS: int.
    int Find(int index, Record* Element)
    {
        if(!((index >= 0) && (index < Size)))
            return -1;

        for(int i = index; i < Size; ++i)
        {
            Record* Atom = GetAt(i);
            if(*Atom == *Element)
                return i;
        }

        return -1;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::CreateOfType();
    //DESCRIPTION : Should be redefined by inheritors. Function has to be overrided by inheritors.
    // 		it has to create an object of the class being inherited..
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType()=0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::Clone();
    //DESCRIPTION : Creates exact copy of array.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* Clone()
    {
        // Creates copy
        // 	GlobalComplexArray* pClone=(GlobalComplexArray*)CreateArrayByType(WhatTheArray());
        MArray* pClone = CreateOfType();
        pClone->SetSize(Size);
        // Copies all elements
        for(int i=0; i<Size;i++)
        {
            ((GlobalComplexArray<Record>*)pClone)->SetAt(i,GetAt(i));
        }
        // Returns pointer to copy
        return pClone;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION : Reads array from 'UTP - h_nuxfile' format file.
    //INPUTS: FILE pointer, int .
    //RETURNS: True.
    virtual bool ReadBin(FILE* h_nuxfile, int version)
    {
        switch (version)
        {
        case 0:
            // Skips record length
            fseek(h_nuxfile, 3, SEEK_CUR);
            // Reads elements quantity
            unsigned int Cnt=0;
            fread(&Cnt, 4, 1, h_nuxfile);
            if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
            SetSize(Cnt);
            // Reads lines
            Record* pAtom=(Record*)m_pHead;
            for(int i=0;i<Size;i++)
            {
                pAtom->ReadBin(h_nuxfile,version);
                pAtom++;
            }
        }
        return true;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::ReadBin(MFile* file, int version);
    //DESCRIPTION : Reads array from UTP-format file.
    //INPUTS: MFile pointer, int .
    //RETURNS: True.
    virtual bool ReadBin(MFile* file, int version)
    {
        switch (version)
        {
        case 0:
            // Skips record length
            file->SetPosition(3, MAP_SEEK_CUR);
            // Reads elements quantity
            unsigned int Cnt=0;
            file->GetData(&Cnt, 4, 1);
            if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
            SetSize(Cnt);
            // Reads lines
            Record* pAtom=(Record*)m_pHead;
            for(int i=0;i<Size;i++)
            {
                pAtom->ReadBin(file,version);
                pAtom++;
            }
        }
        return true;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::WriteBin(FILE* h_nuxfile,int version);
    //DESCRIPTION : Saves array to the UTP-format file.
    //INPUTS: FILE pointer,int .
    //RETURNS: True.
    virtual bool WriteBin(FILE* h_nuxfile,int version)
    {
        switch (version)
        {
        case 0:
            // Saves identifier
            MString str = WhatTheArray();
            fwrite((char *)str, 3, 1, h_nuxfile);
            // Saves elements quantity
            fwrite(&Size, 4, 1, h_nuxfile);
            // Saves Lines
            Record* pAtom=(Record*)m_pHead;
            for(int i=0;i<Size;i++)
            {
                pAtom->WriteBin(h_nuxfile,version);
                pAtom++;
            }
        }
        return true;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::GetAtRec(int index,Record* Head);
    //DESCRIPTION : Returns pointer to element 'index' of array with the first element of HeAD.
    //INPUTS: int ,Record pointer.
    //RETURNS: Record pointer.
    Record* GetAtRec(int index,Record* Head)
    {
        return Head+index;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::GetAtVoid(int iIndex);
    //DESCRIPTION : Gets void pointer to element.
    //INPUTS: int .
    //RETURNS: void pointer.
    virtual void* GetAtVoid(int iIndex)
    {
        return (void*)GetAt(iIndex);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::SetAtVoid(int i,void* pEl);
    //DESCRIPTION : Sets by void pointer.
    //INPUTS: int ,void pointer.
    //RETURNS: NONE.
    virtual void SetAtVoid(int i,void* pEl)
    //============================================================
    {
        SetAt(i,(Record*)pEl);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalComplexArray::Copy(Record* In,Record* Out);
    //DESCRIPTION : Redefinable for inheritors fuction of array elements copying .
    //INPUTS: Record pointer,Record pointer.
    //RETURNS: NONE.
    virtual void Copy(Record* In,Record* Out)
    //============================================================
    {
        In->Copy(Out);
    }

    // EoTEMPLATE CLASS GlobalComplexArray:public MArray
};

//PACKAGE: MAP.
//CLASS	: MPtArray : public GlobalArray<MPoint>.
//DESCRIPTION : Destined for points array storing (MPoint).
//		Class MPoint described in BaseCl.h.
class  MPtArray : public GlobalArray<MPoint>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MPtArray::CreateOfType();
    //DESCRIPTION : Creates array - object of 'MPtArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MPtArray();}

    //PACKAGE: MAP.
    //FUNCTION : MPtArray::WhatTheArray();
    //DESCRIPTION : Returns identifier of 'MPtArray' points array .
    //INPUTS: NONE.
    //RETURNS: NString.
    virtual MString WhatTheArray(){return ID_PTA;}

    //PACKAGE: MAP.
    //FUNCTION : MPtArray::FastSet(int Cnt, MPoint* points);
    //DESCRIPTION : Sets a lot of points at once by their quantity
    // 		and location adress in memory .
    //INPUTS: int , MPoint pointer.
    //RETURNS: NONE.
    virtual void FastSet(int Cnt, MPoint* points);

    //PACKAGE: MAP.
    //FUNCTION : MPtArray::FastSet(MPoint* points);
    //DESCRIPTION : Sets a lot of points at once by their quantity
    // 		and location adress in memory .
    //INPUTS: MPoint pointer.
    //RETURNS: NONE.
    virtual void FastSet(MPoint* points);

    //PACKAGE: MAP.
    //FUNCTION : MPtArray::Cut(int index);
    //DESCRIPTION : Splits array in two parts
    // 		1st part remains in the array [0;index-1]
    // 		2nd part is created and returned [index ; Size] .
    //INPUTS: int .
    //RETURNS: MPtArray pointer.
    MPtArray* Cut(int index);

    //PACKAGE: MAP.
    //FUNCTION : MPtArray::FindPoint(MPoint pt);
    //DESCRIPTION : Searchs for point pt .
    //INPUTS: MPoint.
    //RETURNS: int.
    int FindPoint(MPoint pt);
};

//PACKAGE: MAP.
//CLASS	: MHlArray : public GlobalArray<MPoint>.
//DESCRIPTION : Destined for points array storing (MPoint).
//		Class MPoint described in BaseCl.h.
class  MHlArray : public GlobalArray<MPoint>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MHlArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MHlArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MHlArray();}

    //PACKAGE: MAP.
    //FUNCTION : MHlArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MPtArray' points array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_PHL;}
};

//PACKAGE: MAP.
//CLASS	: MLfArray : public GlobalArray<M_LOG_FONT>.
//DESCRIPTION : Destined for logical fonts array storing(M_LOG_FONT).
//		Structure M_LOG_FONT described in BaseCl.h.
class  MLfArray : public GlobalArray<M_LOG_FONT>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MLfArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MLfArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MLfArray();}

    //PACKAGE: MAP.
    //FUNCTION : MLfArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MLfArray' fonts array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_LFA;}
};

//PACKAGE: MAP.
//CLASS	: MInArray : public GlobalArray<int>.
//DESCRIPTION : Used for integers array storing (int).
class  MInArray : public GlobalArray<int>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MInArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MInArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MInArray();}

    //PACKAGE: MAP.
    //FUNCTION : MInArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MInArray' array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_INA;}
};

//PACKAGE: MAP.
//CLASS	: MByArray : public GlobalArray<unsigned char>.
//DESCRIPTION : Used for single-byte integers array storing  (unsigned char).
class  MByArray : public GlobalArray<unsigned char>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MByArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MByArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MByArray();}

    //PACKAGE: MAP.
    //FUNCTION : MByArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MByArray' array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_BYA;}
};

//PACKAGE: MAP.
//CLASS	: MShInArray : public GlobalArray<unsigned short>.
//DESCRIPTION : Destined for integers array storing (unsigned short) .
class  MShInArray : public GlobalArray<unsigned short>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MShInArray::CreateOfType();
    //DESCRIPTION : Creates array - object of 'MByArray' class
    //  			Creates array - object of 'MShInArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MShInArray();}

    //PACKAGE: MAP.
    //FUNCTION : MShInArray::WhatTheArray();
    //DESCRIPTION :  Creates array - object of 'MByArray' class
    // 				 Returns identifier of 'MShInArray' array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_USA;}
};

//PACKAGE: MAP.
//CLASS	: MDuArray : public GlobalArray<double>.
//DESCRIPTION : Destined for rational numbers array storing (double)  .
class  MDuArray : public GlobalArray<double>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MDuArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MDuArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MDuArray();}

    //PACKAGE: MAP.
    //FUNCTION : MDuArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MDuArray' array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_DUA;}

    //PACKAGE: MAP.
    //FUNCTION : MDuArray::SortByGrow();
    //DESCRIPTION :  Sorts by increment(by 'bubble' method) .
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual void SortByGrow()
    {
        int i;
        int j;
        for(i=Size-1;i>0;i--)
        {
            for(j=0;j<i;j++)
            {
                if(*GetAt(j)>*GetAt(j+1))
                {
                    Exchange(j,j+1);
                }
            }
        }
    }
};

//PACKAGE: MAP.
//CLASS	: MClArray : public GlobalArray<MColor>.
//DESCRIPTION : Destined for colors array(Mcolor) storing .
//		Class MColor described in BaseCl.h.
class  MClArray : public GlobalArray<MColor>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MClArray::CreateOfType();
    //DESCRIPTION :  Create array - object of 'MClArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MClArray();}

    //PACKAGE: MAP.
    //FUNCTION : MClArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MClArray' array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_CLA;}
};

//PACKAGE: MAP.
//CLASS	: MBoArray : public GlobalArray<bool>.
//DESCRIPTION : Destined for variables(of bool type) array storing .
class  MBoArray : public GlobalArray<bool>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MBoArray::CreateOfType();
    //DESCRIPTION :  Creates array - îobject of 'MBoArray' class .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MBoArray();}

    //PACKAGE: MAP.
    //FUNCTION : MBoArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MBoArray' array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_BOA;}
};

//PACKAGE: MAP.
//CLASS	: MRkArray : public GlobalArray<MRect>.
//DESCRIPTION : Destined for rectangles array storing (class MRect) .
//		Class MRect described in BaseCl.h.
class  MRkArray : public GlobalArray<MRect>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MRkArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MRkArray' class.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MRkArray();}

    //PACKAGE: MAP.
    //FUNCTION : MRkArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MRkArray' array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_RKA;}
};

//PACKAGE: MAP.
//CLASS	: MVoidArray:public GlobalArray<void*>.
//DESCRIPTION : Destined for atypical pointers array storing .
class  MVoidArray:public GlobalArray<void*>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MVoidArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MVoidArray' class.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MVoidArray();}

    //PACKAGE: MAP.
    //FUNCTION : MVoidArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MVoidArray' array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_VDA;}

    //PACKAGE: MAP.
    //FUNCTION : MVoidArray::ReadBin(FILE* ,int );
    //DESCRIPTION :  Functions of reading an array of void pointers cannot be exactly defined.
    //INPUTS: FILE pointer ,int .
    //RETURNS: False.
    virtual bool ReadBin(FILE* ,int ){return false;}  // (FILE* h_nuxfile,int version)

    //PACKAGE: MAP.
    //FUNCTION : MVoidArray::ReadBin(MFile* ,int );
    //DESCRIPTION :  Functions of reading an array of void pointers cannot be exactly defined.
    //INPUTS: MFile pointer ,int.
    //RETURNS: False.
    virtual bool ReadBin(MFile* ,int ){return false;} // (MFile* file,int version)

    //PACKAGE: MAP.
    //FUNCTION : MVoidArray::WriteBin(FILE* ,int );
    //DESCRIPTION :  Functions of writing an array of void pointers cannot be exactly defined.
    //INPUTS: FILE pointer,int.
    //RETURNS: False.
    virtual bool WriteBin(FILE* ,int ){return false;} // (FILE* h_nuxfile,int version)

};

//PACKAGE: MAP.
//CLASS	: MPkArray:public GlobalComplexArray<MPrk>.
//DESCRIPTION : Destined for logical handwrites(MPrk) array storing.
//		Class MPrk is described in BaseCl.h  .
class  MPkArray:public GlobalComplexArray<MPrk>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MPkArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of MPkArray class.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MPkArray();}

    //PACKAGE: MAP.
    //FUNCTION : MPkArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of MPkArray array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_PKA;}
};

//PACKAGE: MAP.
//CLASS	: MSmArray:public GlobalComplexArray<MSymbol>.
//DESCRIPTION : Destined for users geosynonyms(class MSymbol) array storing. MSymbol is inheritor of MPrk .
//		Class MSymbol is described in BaseCl.h  .
class  MSmArray:public GlobalComplexArray<MSymbol>
{
    //PACKAGE: MAP.
    //FUNCTION : MSmArray::CreateOfType();
    //DESCRIPTION : Creates array - object of 'MSmArray' class.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MSmArray();}

    //PACKAGE: MAP.
    //FUNCTION : MSmArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of MSmArray class.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_SMA;}
};

//PACKAGE: MAP.
//CLASS	: MNvArray : public GlobalComplexArray<MNomValue>.
//DESCRIPTION : Destined for variables(of type MnomValue) array storing,
//		which stores a pair of "numberic code - value" .
//		Class MNomValue is described in BaseCl.h  .
class  MNvArray : public GlobalComplexArray<MNomValue>
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of MNvArray class.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MNvArray();}

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::WhatTheArray();
    //DESCRIPTION : Returns identifier of 'MNvArray' array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_NVA;}

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::GetValue(unsigned short nom, MString& str);
    //DESCRIPTION :  Gets characteristic value by numberic code.
    //INPUTS: unsigned short , MString& .
    //RETURNS: int.
    int 	GetValue(unsigned short nom, MString& str);

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::GetNom(MString str);
    //DESCRIPTION :  Returns numberic code by characteristic value.
    //INPUTS: MString .
    //RETURNS: int.
    int     GetNom(MString str);

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::SetValue(unsigned short nom, MString str);
    //DESCRIPTION :  Sets characteristic value by numberic code.
    //INPUTS:  unsigned short , MString .
    //RETURNS: int.
    int     SetValue(unsigned short nom, MString str);

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::SetValueByIndex(int index, MString str);
    //DESCRIPTION :  Sets characteristic value by index from array.
    //INPUTS: int , MString .
    //RETURNS: True.
    bool	SetValueByIndex(int index, MString str);

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::SetNomByIndex(int index, unsigned short nom);
    //DESCRIPTION :  Sets numberic code by index from array.
    //INPUTS: int , unsigned short .
    //RETURNS: True.
    bool	SetNomByIndex(int index, unsigned short nom);
};

//PACKAGE: MAP.
//CLASS	: MPgArray:public GlobalComplexArray<MPredlogenie>.
//DESCRIPTION : Destined for sentences(MPrelogenie) array storing. (MPredlogenie) is basic structure of formulas.
//		Class MPredlogenie is described in BaseCl.h  .
class  MPgArray:public GlobalComplexArray<MPredlogenie>
{
    //PACKAGE: MAP.
    //FUNCTION : MNvArray::CreateOfType();
    //DESCRIPTION :  Creates array - object of 'MPgArray' class.
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType(){return (MArray*) new MPgArray();}

    //PACKAGE: MAP.
    //FUNCTION : MNvArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MPgArray' array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_PGN;}
};

//PACKAGE: MAP
//CLASS	: MStArray : public MArray
//DESCRIPTION : Describes array of strings (class MString)
//		Class MString is described in BaseCl.h  .
class  MStArray : public MArray
{
public:

    //PACKAGE: MAP.
    //FUNCTION : MStArray::MStArray();
    //DESCRIPTION :  Creates an empty array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    MStArray();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::MStArray(int Cnt);
    //DESCRIPTION :  Consructor with established elements quantity
    // 		Creates array, that contained 'Cnt' elements created by default.
    //INPUTS: int .
    //RETURNS: NONE.
    MStArray(int Cnt);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::~MStArray();
    //DESCRIPTION :  Destructor - frees memory.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual ~MStArray();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::ClearAll();
    //DESCRIPTION : Removes all elements from array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual void ClearAll();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::FreeMemory();
    //DESCRIPTION : nalog MStArray::ClearAll();.
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual void FreeMemory(){ClearAll();}

    //PACKAGE: MAP.
    //FUNCTION : MStArray::GetBytes();
    //DESCRIPTION :  Returns full size of array to UTP-format.
    //INPUTS: NONE.
    //RETURNS: int.
    virtual int GetBytes();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::MStArray();
    //DESCRIPTION :  Creates an empty array.
    //INPUTS: int .
    //RETURNS: MString pointer.
    // Returns pointer to element, defined by index 'Index'
    MString* GetAt(int index);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::operator[](int index);
    //DESCRIPTION :  Returns pointer to element, defined by index 'Index'.
    //INPUTS: int .
    //RETURNS: MString pointer.
    MString* operator[](int index);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::SetAt(int index,MString* Element);
    //DESCRIPTION :  Sets Element into a cell 'index'.
    //INPUTS: int ,MString pointer.
    //RETURNS: NONE.
    void SetAt(int index,MString* Element);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::SetAtGrow(int nIndex, MString* newElement);
    //DESCRIPTION : Sets Element into defined cell of 'index'.
    // 		array is increased if necessary..
    //INPUTS: int , MString pointer.
    //RETURNS: NONE.
    void SetAtGrow(int nIndex, MString* newElement);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::SetSize(int Cnt);
    //DESCRIPTION :  Sets elements quantity of array.
    //INPUTS: int .
    //RETURNS: NONE.
    void SetSize(int Cnt);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::AppendHead(MStArray* pArr);
    //DESCRIPTION :  Assigns array, on which one indicates
    // 		(MArr) to the beginning of array.
    //INPUTS: MStArray pointer.
    //RETURNS: NONE.
    void AppendHead(MStArray* pArr);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::AddHead(MString* Element);
    //DESCRIPTION :  Adds Element to the beginning of array.
    //INPUTS: MString pointer.
    //RETURNS: NONE.
    void AddHead(MString* Element);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::InsertBefore(int index,MString* Element);
    //DESCRIPTION :  Inserts element into a cell, defined by 'index'.
    //INPUTS: int ,MString pointer.
    //RETURNS: NONE.
    void InsertBefore(int index,MString* Element);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::InsertAfter(int index,MString* Element);
    //DESCRIPTION :  Inserts element 'Element' after index 'index'.
    //INPUTS: int ,MString pointer.
    //RETURNS: NONE.
    void InsertAfter(int index,MString* Element);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::AddTail(MString* Element);
    //DESCRIPTION :  Adds Element to the end of array.
    //INPUTS: MString pointer.
    //RETURNS: NONE.
    void AddTail(MString* Element);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::AppendTail(MStArray* MArr);
    //DESCRIPTION :  Adds an array, on which one indicates
    // 		(pArr) to the end of array.
    //INPUTS: MStArray pointer.
    //RETURNS: NONE.
    void AppendTail(MStArray* MArr);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::DeleteHead();
    //DESCRIPTION :  Deletes first element of array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteHead();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::Delete(int index);
    //DESCRIPTION : Deletes element, defined by index 'index'
    // 		thus quantity of elements is decreased by 1.
    //INPUTS: int .
    //RETURNS: NONE.
    void Delete(int index);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::DeleteTail();
    //DESCRIPTION :  Deletes last element from the array.
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteTail();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::Find(int index,MString* Element);
    //DESCRIPTION :  Searches for the first element, starts with the position of index
    //  		 equal Element and returns it's index
    //  		 (or -1, if such element was not found).
    //INPUTS: int ,MString pointer.
    //RETURNS: int.
    int Find(int index,MString* Element);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::IsEqualTo(MStArray* m_pArrToCompare);
    //DESCRIPTION :  Identity comparison of array lines with 'm_pArrToCompare'.
    //INPUTS: MStArray pointer.
    //RETURNS: True.
    bool IsEqualTo(MStArray* m_pArrToCompare);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::Clone();
    //DESCRIPTION :  Creates exact copy of array .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* Clone();

    //PACKAGE: MAP.
    //FUNCTION : MStArray::ReadBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Read array from UTP-format file.
    //INPUTS: FILE pointer, int .
    //RETURNS: True.
    virtual bool ReadBin(FILE* h_nuxfile, int version);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::ReadBin(MFile* file, int version);
    //DESCRIPTION :  Read array from UTP-format file.
    //INPUTS: MFile pointer, int .
    //RETURNS: True.
    virtual bool ReadBin(MFile* file, int version);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::WriteBin(FILE* h_nuxfile, int version);
    //DESCRIPTION :  Saves array to UTP-format file.
    //INPUTS: FILE pointer,  int .
    //RETURNS: True.
    virtual bool WriteBin(FILE* h_nuxfile,  int version);

    //PACKAGE: MAP.
    //FUNCTION : MStArray::WhatTheArray();
    //DESCRIPTION :  Returns identifier of 'MStArray' array.
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray(){return ID_STA;}

protected:
    // Returns pointer to element 'index'
    // of array with the first element of HeAD
    MString* GetAtSt(int index,MString* Head);

    // Creates a copy of 'In' line and moves it to 'Out'
    void Copy(MString* In,MString* Out);

    //  Gets void pointer by index 'iIndex'
    virtual void* GetAtVoid(int iIndex){return (void*)GetAt(iIndex);}
    //  Sets void pointer by index 'iIndex'
    virtual void SetAtVoid(int i,void* pEl){SetAt(i,(MString*)pEl);}
    //  Exchange of two elements of array with defined indexes
    virtual void Exchange(int i,int j);

};//EoCLASS MStArray


//ATTEMPT! To create an array, that contains inheritors of defined class.

//PACKAGE: MAP.
//TEMPLATE CLASS : ParentWrapper.
//DESCRIPTION : Class participates in creation of array, that can contain inheritors from defined class.
template<class PARENT> class ParentWrapper
{
protected:
    PARENT* m_pParent;
public:

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::CheckElement();
    //DESCRIPTION :  Checks element .
    //INPUTS: NONE.
    //RETURNS: NONE.
    void CheckElement(){}

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::ParentWrapper();
    //DESCRIPTION :  Constructor .
    //INPUTS: NONE.
    //RETURNS: NONE.
    ParentWrapper():m_pParent(NULL/*(new PARENT())*/){}

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::ParentWrapper(PARENT* pParent);
    //DESCRIPTION :  Constructor from element .
    //INPUTS: PARENT pointer.
    //RETURNS: NONE.
    ParentWrapper(PARENT* pParent):m_pParent((PARENT*)pParent->Clone()){}

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::~ParentWrapper();
    //DESCRIPTION :  Destructor.
    //INPUTS: NONE.
    //RETURNS: NONE.
    ~ParentWrapper(){if(m_pParent)delete m_pParent;}

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::Copy(ParentWrapper<PARENT>* pSource);
    //DESCRIPTION :  Copies element .
    //INPUTS: ParentWrapper<PARENT> pointer.
    //RETURNS: NONE.
    void Copy(ParentWrapper<PARENT>* pSource)
    {
        if(m_pParent) delete m_pParent;
        if(pSource)
            m_pParent = (PARENT*)pSource->m_pParent->Clone();
        else m_pParent = NULL;
    }

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::SetInternals(PARENT* pParent);
    //DESCRIPTION :  Sets inner relations .
    //INPUTS: PARENT pointer.
    //RETURNS: NONE.
    void SetInternals(PARENT* pParent)
    {
        if(m_pParent)delete m_pParent;
        m_pParent = (PARENT*)pParent->Clone();
    }

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::GetInternals();
    //DESCRIPTION :  Gets inner relations .
    //INPUTS: NONE.
    //RETURNS: PARENT pointer.
    PARENT* GetInternals(){CheckElement();return m_pParent;}

    //PACKAGE: MAP.
    //FUNCTION : ParentWrapper::IsEqualTo(ParentWrapper<PARENT>* pCompareWith);
    //DESCRIPTION :  Comparison of element with passed element 'pCompareWith' .
    //INPUTS: ParentWrapper<PARENT> pointer.
    //RETURNS: True.
    bool IsEqualTo(ParentWrapper<PARENT>* pCompareWith)
    {
        CheckElement();
        return m_pParent->IsEqualTo(pCompareWith->m_pParent);
    }
};

//PACKAGE: MAP
//TEMPLATE CLASS: GlobalInheritantArray:public MArray
//DESCRIPTION : This TEMPLATE is destined for Array COMPOUND INHERITORS which contain POINTERS indside.
//				It is completely abstract class as it can't (even theoretically) 
//				write it is own 'WriteBin' and 'ReadBin'. These functions  are written
//				only at inheritors.
//				Clone() is necessary for the class contained in array.
template<class Record> class GlobalInheritantArray:public MArray
{
public:
    typedef ParentWrapper<Record> Wrapper;

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:GlobalInheritantArray();
    //DESCRIPTION :  Constructor by default .
    //INPUTS: NONE.
    //RETURNS: NONE.
    GlobalInheritantArray()
    {
        Size = 0;
        m_pHead = NULL;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:~GlobalInheritantArray();
    //DESCRIPTION :  Destructor .
    //INPUTS: NONE.
    //RETURNS: NONE.
    virtual ~GlobalInheritantArray()
    {
        delete[] (Wrapper*)m_pHead;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:GetAt(int iIndex);
    //DESCRIPTION :  Gets element by index 'iIndex' .
    //INPUTS: int .
    //RETURNS: Record pointer.
    Record* GetAt(int iIndex)
    {
        // If the index falls out the limits of array
        // than returns null element
        if(!((iIndex>=0)&&(iIndex<Size))) iIndex=0;
        return (Record*)(((Wrapper*)m_pHead)[iIndex].GetInternals());
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:AddHead(Record* pRec);
    //DESCRIPTION :  Adds element to the beginning .
    //INPUTS: Record pointer.
    //RETURNS: NONE.
    void AddHead(Record* pRec)
    {
        // Creates new array
        Wrapper* pWrap = new Wrapper[++Size];
        // Copies coincident part
        for(int i = 1;i<Size;i++)
            pWrap[i].Copy(((Wrapper*)m_pHead)+(i-1));
        delete[] (Wrapper*)m_pHead;
        m_pHead = (void*)pWrap;
        // Inserts first element
        Wrapper Wr(pRec);
        ((Wrapper*)m_pHead)->Copy(&Wr);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:AddTail(Record* pRec);
    //DESCRIPTION :  Adds element to the end .
    //INPUTS: Record pointer.
    //RETURNS: NONE.
    void AddTail(Record* pRec)
    {
        // Creates new array
        Wrapper* pWrap = new Wrapper[++Size];
        // Copies coincident part
        for(int i = 0;i<Size-1;i++)
            pWrap[i].Copy(((Wrapper*)m_pHead)+i);
        delete[] (Wrapper*)m_pHead;
        m_pHead = (void*)pWrap;

        // Establishes last element
        Wrapper Wr(pRec);
        ((Wrapper*)m_pHead)[Size-1].Copy(&Wr);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:ClearAll();
    //DESCRIPTION :  Clears array. Frees memory .
    //INPUTS: NONE.
    //RETURNS: NONE.
    void ClearAll()
    {
        delete[] (Wrapper*)m_pHead;
        m_pHead = NULL;
        Size = 0;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:Clone();
    //DESCRIPTION :  We redefines this function at inheritors.
    // 		 It is also creates an array of the same type as it is .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    virtual MArray* CreateOfType()=0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:GlobalInheritantArray();
    //DESCRIPTION :  Makes a clone of array .
    //INPUTS: NONE.
    //RETURNS: MArray pointer.
    MArray* Clone()
    {
        // Creates a copy
        GlobalInheritantArray* pClone=(GlobalInheritantArray*)CreateOfType();
        pClone->SetSize(Size);
        // Copies all elements
        for(int i = 0;i<Size;i++)
            ((Wrapper*)pClone->m_pHead)[i].Copy(((Wrapper*)m_pHead)+i);
        // Returns pointer to copy
        return pClone;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:Delete(int iIndex);
    //DESCRIPTION :  Deletes element by index 'iIndex' .
    //INPUTS: int .
    //RETURNS: NONE.
    void Delete(int iIndex)
    {
        // If the index falls out the limits of array,
        // than doing nothing
        if (!((iIndex>=0)&&(iIndex<Size)))return;

        // Creates new array
        Wrapper* newAr = new Wrapper[Size-1];
        // Copies coincident part
        int i = 0;
        for(i=0;i<iIndex;i++)
            newAr[i].Copy(((Wrapper*)m_pHead)+i);
        for(i=iIndex+1;i<Size;i++)
            newAr[i-1].Copy(((Wrapper*)m_pHead)+i);
        delete[] (Wrapper*)m_pHead;
        m_pHead=(void*)newAr;
        // Modifies quantity
        Size--;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:DeleteHead();
    //DESCRIPTION :  Deletes element from the head .
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteHead()
    {
        // Creates new array
        Wrapper* ptr=new Wrapper[Size-1];
        // Copies coincident part
        for(int i = 0;i<Size-1;i++)
            ptr[i].Copy(((Wrapper*)m_pHead)+(i+1));
        // Deletes old array
        delete[] (Wrapper*)m_pHead;
        // Sets pointer to new array
        m_pHead=(void*)ptr;
        // Modifies quantity
        Size--;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:DeleteTail();
    //DESCRIPTION :  Deletes element from the end .
    //INPUTS: NONE.
    //RETURNS: NONE.
    void DeleteTail()
    {
        // Creates new array
        Wrapper* ptr=new Wrapper[Size-1];
        // Copies coincident part
        for(int i = 0;i<Size-1;i++)
            ptr[i].Copy(((Wrapper*)m_pHead)+i);
        // Deletes old array
        delete[] (Wrapper*)m_pHead;

        // Sets pointer to new array
        m_pHead=(void*)ptr;
        // Modifies quantity
        Size--;
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:FreeMemory();
    //DESCRIPTION :  Clears array and memory .
    //INPUTS: NONE.
    //RETURNS: NONE.
    void FreeMemory()
    {
        ClearAll();
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:operator[](int iIndex);
    //DESCRIPTION :  Gets element by index 'iIndex' .
    //INPUTS: int .
    //RETURNS: Record pointer.
    Record* operator[](int iIndex)
    {
        return GetAt(iIndex);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:SetAt(int iIndex,Record* pRec);
    //DESCRIPTION :  Sets element .
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void SetAt(int iIndex,Record* pRec)
    {
        // If the index falls out the limits o array,
        //  than doing nothing
        if((iIndex>=0)&&(iIndex<Size))
        {
            Wrapper temp(pRec);
            ((Wrapper*)m_pHead)[iIndex].Copy(&temp);
        }
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:InsertAfter(int index,Record* Element);
    //DESCRIPTION :  Inserts element 'Element' after index 'index' .
    //INPUTS: int ,Record pointer.
    //RETURNS: NONE.
    void InsertAfter(int index,Record* Element)
    {
        // If the index falls out the limits of array,
        // than doing nothing
        if (!((index>=0)&&(index<Size)))  return; //   exit(15);
        // Creates new array
        Wrapper* ptr=new Wrapper[Size+1];
        // Copies coincident part
        int N=index+1;
        int i = 0;
        for(i=0;i<N;i++)
            ptr[i].Copy(((Wrapper*)m_pHead)+i);
        for(i=N;i<Size;i++)
            ptr[i+1].Copy(((Wrapper*)m_pHead)+i);

        // Deletes old array
        delete[] (Wrapper*)m_pHead;
        // Sets pointer to the new array
        m_pHead=(void*)ptr;
        // Modifies quantity
        Size++;
        // Inserts an element
        SetAt(index+1,Element);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:GetBytes();
    //DESCRIPTION :  Gets array size in a bytes .
    //INPUTS: NONE.
    //RETURNS: int.
    int GetBytes()
    {
        int counter(0);
        for(int i=0;i<Size;i++)
            counter+=GetAt(i)->GetBytes();
        return counter+sizeof(Size)+3;
    }

    // Following functions even theoretically cannot be described in a template

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:ReadBin(FILE* h_nuxfile,int version);
    //DESCRIPTION :  Reads from file .
    //INPUTS: FILE pointer,int .
    //RETURNS: True.
    virtual bool ReadBin(FILE* h_nuxfile,int version)=0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:ReadBin(MFile* h_nuxfile,int version);
    //DESCRIPTION :  Reads from file .
    //INPUTS: MFile pointer,int .
    //RETURNS: True.
    virtual bool ReadBin(MFile* file,int version)=0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:WriteBin(FILE* h_nuxfile,int version);
    //DESCRIPTION :  Saves to file .
    //INPUTS: FILE pointer,int .
    //RETURNS: True.
    virtual bool WriteBin(FILE* h_nuxfile,int version)=0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:MString WhatTheArray();
    //DESCRIPTION :  Identifies array .
    //INPUTS: NONE.
    //RETURNS: MString.
    virtual MString WhatTheArray()=0;

    // ATTENTION! At installation of sizes it is necessary to fill up an array
    // of elements by default, and it may be known only for arrays-inheritors
protected:

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:ExpandSize(int iCnt);
    //DESCRIPTION :  Increases array size by quantity of elements 'iCnt' .
    //INPUTS: int iCnt.
    //RETURNS: NONE.
    void ExpandSize(int iCnt)
    {
        if((iCnt == Size)||(iCnt<0))
            return;
        delete[] (Wrapper*)m_pHead;
        m_pHead = new Wrapper[iCnt];
        Size = iCnt;
    }

public:

    //============================================
    // FUNCTION		:GlobalInheritantArray:SetSize(int Cnt)
    //
    ///Sets array size.Virtual function.
    // 				Inheritor have to refer in a following form:
    //
    // 		Assume that MElement* pEl - element  by default
    // 		ExpandSize(Cnt);
    // 		for(int i=0;i<Cnt;i++)
    // 			SetAt(i,pEl);
    //
    //
    // 		if((Cnt == Size)||(Cnt<0))
    // 			return;
    // 		delete[] (Wrapper*)m_pHead;
    // 		m_pHead = new Wrapper[Cnt];
    // 		Size = Cnt;
    //

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:SetSize(int Cnt);
    //DESCRIPTION :  Clears array and memory .
    //INPUTS: int .
    //RETURNS: NONE.
    void SetSize(int Cnt) = 0;

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:GetHead();
    //DESCRIPTION :  Gets first element .
    //INPUTS: NONE.
    //RETURNS: Record pointer.
    virtual Record* GetHead()
    {
        return (Record*)((Wrapper*)m_pHead)->GetInternals();
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:GetTail();
    //DESCRIPTION :  Gets last element .
    //INPUTS: NONE.
    //RETURNS: Record pointer.
    virtual Record* GetTail()
    {
        return GetAt(Size-1);
    }

    //PACKAGE: MAP.
    //FUNCTION : GlobalInheritantArray:Exchange(int i,int j);
    //DESCRIPTION :  Swaps two elements .
    //INPUTS: int ,int .
    //RETURNS: NONE.
    virtual void Exchange(int i,int j)
    {
        if(!((i<Size)&&(i>=0)&&(j<Size)&&(j>=0)))  return; //  exit(15);;
        Wrapper tempWrapper;
        tempWrapper.Copy(&((Wrapper*)m_pHead)[i]);
        ((Wrapper*)m_pHead)[i].Copy(&((Wrapper*)m_pHead)[j]);
        ((Wrapper*)m_pHead)[j].Copy(&tempWrapper);
    }

};//EoTEMPLATE CLASS GlobalInheritantArray:public MArray

#endif // M_ARRBASE_H
