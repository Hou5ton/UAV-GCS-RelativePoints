//PACKAGE: MAP.
//FILE: Array.h
//AUTHOR:
//DESCRIPTION: 

#if !defined(M_ARRAY_H)
#define M_ARRAY_H
#include <malloc.h>
#include "Id.h"
#include "ArrBase.h"
#include "MString.h"

//Creates an array of specified type strType
MArray* CreateBaseArrayByType(MString strType);

//PACKAGE : MAP.
//CLASS	: MArArray. 
//DESCRIPTION	: Class - an array of pointers to arrays .
//				  It lets you create an array of arrays of different types.
class MArArray : public MArray
{
public:
//PACKAGE : MAP.
//FUNCTION : MArArray();
//DESCRIPTION : Creates an empty array.
//INPUTS : NONE.
//RETURNS: NONE.
	MArArray();

//PACKAGE : MAP.
//FUNCTION : MArArray(int Cnt);
//DESCRIPTION : The constructor with a given number of elements
//				It creates an array containing Cnt elements  
//				constructed by default.
//INPUTS : int.
//RETURNS: NONE.
	MArArray(int Cnt);
	
//PACKAGE: MAP.
//FUNCTION: ~MArArray();
//DESCRIPTION: The destructor frees the memory.
//INPUTS: NONE.
//RETURNS: NONE.
	virtual ~MArArray();
	
//PACKAGE: MAP.
//FUNCTION: MArArray::ClearAll();
//DESCRIPTION: It removes all of the elements of the array.
//INPUTS: NONE.
//RETURNS: NONE.
	virtual void ClearAll();

//PACKAGE: MAP.
//FUNCTION: MArArray::FreeMemory();
//DESCRIPTION: Frees memory.
//INPUTS: NONE.
//RETURNS: NONE.
	virtual void FreeMemory();

//PACKAGE: MAP.
//FUNCTION: MArArray::GetBytes();
//DESCRIPTION: Returns the total size of the array in UTP-format.
//INPUTS: NONE.
//RETURNS: Int.
	virtual int GetBytes();

//PACKAGE: MAP.
//FUNCTION: MArArray::GetAt(int index);
//DESCRIPTION: It returns a reference to an element defined by the Index.
//INPUTS: int.
//RETURNS: MArray pointer.
        MArray* GetAt(int index) const;

//PACKAGE: MAP.
//FUNCTION: MArArray::operator[](int index);
//DESCRIPTION: It returns a reference to an element defined by the Index.
//INPUTS: int index.
//RETURNS: MArray pointer.
	MArray* operator[](int index);	

//PACKAGE: MAP.
//FUNCTION: MArArray::SetAt(int index,MArray* Element);
//DESCRIPTION: Set Element in cell index.
//INPUTS: int , MArray pointer.
//RETURNS: NONE.
	void SetAt(int index,MArray* Element);
	
//PACKAGE: MAP.
//FUNCTION: MArArray::SetSize(int Cnt);
//DESCRIPTION: Sets the number of array elements.
//INPUTS: int.
//RETURNS: NONE.
	void SetSize(int Cnt);

//PACKAGE: MAP.
//FUNCTION: MArArray::AppendHead(MArArray* pArr);
//DESCRIPTION: Attaches array pointed MArr to the top of the array.
//INPUTS: MArArray pointer.
//RETURNS: NONE.
	void AppendHead(MArArray* pArr);

//PACKAGE: MAP.
//FUNCTION: MArArray::AddHead(MArray* Element);
//DESCRIPTION: Adds Element in the beginning of the array.
//INPUTS: MArray pointer.
//RETURNS: NONE.
	void AddHead(MArray* Element);

//PACKAGE: MAP.
//FUNCTION: MArArray::InsertBefore(int index,MArray* Element);
//DESCRIPTION: Inserts an element into the cell , indicating the index.
//INPUTS: int , MArray pointer.
//RETURNS: NONE.
	void InsertBefore(int index,MArray* Element); 

//PACKAGE: MAP.
//FUNCTION: MArArray::InsertAfter(int index,MArray* Element);
//DESCRIPTION: Inserts an Element after index.
//INPUTS: int , MArray pointer.
//RETURNS: NONE.
	void InsertAfter(int index,MArray* Element); 

//PACKAGE: MAP.
//FUNCTION: MArArray::AddTail(MArray* Element);
//DESCRIPTION: Adds Element to the end of the array.
//INPUTS: MArray pointer.
//RETURNS: NONE.
	void AddTail(MArray* Element);

//PACKAGE: MAP.
//FUNCTION: MArArray::AppendTail(MArArray* MArr);
//DESCRIPTION: Adding array pointed pArr at the end of the array.
//INPUTS: MArArray pointer.
//RETURNS: NONE.
	void AppendTail(MArArray* MArr);

//PACKAGE: MAP.
//FUNCTION: MArArray::DeleteHead();
//DESCRIPTION: Deletes the first element of the array.
//INPUTS: NONE.
//RETURNS: NONE.
	void DeleteHead();

//PACKAGE: MAP.
//FUNCTION: MArArray::Delete(int index);
//DESCRIPTION: We remove the item that is determined by index wherein the number of elements is decremented per unit.
//INPUTS: int .
//RETURNS: NONE.	
	void Delete(int index);

//PACKAGE: MAP.
//FUNCTION: MArArray::DeleteTail();
//DESCRIPTION: Deletes the last element of the array.
//INPUTS: NONE.
//RETURNS: NONE.
	void DeleteTail();
	
//PACKAGE: MAP.
//FUNCTION: MArArray::Find(int index,MArray* Element);
//DESCRIPTION: Searches for the first element , starting from the position index equal to Element and returns his index ( or -1 if the element is not found ).
//INPUTS: int ,MArray pointer.
//RETURNS: NONE.
	int Find(int index,MArray* Element);

//PACKAGE: MAP.
//FUNCTION: MArArray::Clone();
//DESCRIPTION: It creates an exact copy of the array.
//INPUTS: NONE.
//RETURNS: MArray pointer.
	virtual MArray* Clone();

//PACKAGE: MAP.
//FUNCTION: MArArray::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION: Reads array of Nux-file.
//INPUTS: FILE pointer, int .
//RETURNS: True.
	virtual bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE: MAP.
//FUNCTION: MArArray::ReadBin(MFile* file, int version);
//DESCRIPTION: Reads array of Nux-file.
//INPUTS: MFile pointer, int .
//RETURNS: True.
	virtual bool ReadBin(MFile* file, int version);  

//PACKAGE: MAP.
//FUNCTION: MArArray::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION: Saves array NUX-file.
//INPUTS: FILE pointer, int .
//RETURNS: True.
	virtual bool WriteBin(FILE* h_nuxfile, int version);

//PACKAGE: MAP.
//FUNCTION: MArArray::ReadBin(MFile* file, int version);
//DESCRIPTION: Returns the ID array.The identifiers listed in Id.h.
//INPUTS: NONE.
//RETURNS: MString.
	virtual MString WhatTheArray(){return ID_GDA;};

protected:
	//Returns a pointer to the index
	//in the array to the first element HeAD
	MArray** GetAtGd(int index,MArray** Head);
	//Returns a pointer to an array located on the Index
	//return type of the array can be different , so used (void *)
	virtual void* GetAtVoid(int iIndex){return (void*)GetAt(iIndex);};
};

#endif // M_ARRAY_H
