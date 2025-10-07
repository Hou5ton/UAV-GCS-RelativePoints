#include "srezi.h"
#include "Refglobal.h"

//===================================================================
//! Writing of layer in the UTP-file
//params (FILE* h_nuxfile, int version)
bool MSrez::WriteBin(FILE* h_nuxfile, int )
//====================================================================
{
    //We determine the cut size
	unsigned int Size=GetSize(); 
    //We write down the cut size
	fwrite(&Size, 4, 1, h_nuxfile);
    //We write down cut type
	int type = GetType();
	fwrite(&type, sizeof(type), 1, h_nuxfile);
	return true;
}
//===================================================================================
//! Reading layer from the UTP-file
//params (FILE* h_nuxfile, int version)
bool MSrez::ReadBin(FILE* h_nuxfile, int )
//===================================================================================
{
    //Depending on the version
    //we read record length
	int iTemp;
	fread(&iTemp, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iTemp, sizeof(iTemp) );
    //we read cut type
	int iType;
	fread(&iType, sizeof(int), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
	return true;
}

//===================================================================================
//! Reading layer from the UTP-file
//params (MFile* file, int version)
bool MSrez::ReadBin(MFile* file, int )
//===================================================================================
{
    //Depending on the version
    //we read record length
	int iTemp;
	file->GetData(&iTemp, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&iTemp, sizeof(iTemp) );
    //we read cut type
	int iType;
	file->GetData(&iType, sizeof(int), 1);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
	return true;
}

//=======================================================
//! For reading any of layers
MSrez* MSrez::ReadAny(FILE* h_nuxfile, int version)
//=======================================================
{
	MSrez* pSrez =NULL;
    //Depending on the version
	switch (version){
	case 0:
		int iType;
        //cut size
		fread(&iType, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
        //cut type
		fread(&iType, sizeof(int), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
		pSrez = CreateSrezByType(iType);
		fseek(h_nuxfile, -8, SEEK_CUR);
		pSrez->ReadBin(h_nuxfile,version);
	}
	return pSrez;
}

//=======================================================
//! For reading any of layers
MSrez* MSrez::ReadAny(MFile* file, int version)
//=======================================================
{
	MSrez* pSrez =NULL;
    //Depending on the version
	switch (version){
	case 0:
		int iType;
        //cut size
		file->GetData(&iType, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
        //cut type
		file->GetData(&iType, sizeof(int), 1);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
		pSrez = CreateSrezByType(iType);
		file->SetPosition(-8, MAP_SEEK_CUR);
		pSrez->ReadBin(file,version);
	}
	return pSrez;
}

//========================================
//! Reading from the file
bool MSzArray::ReadBin(FILE* h_nuxfile,int version)
//========================================
{
	switch (version)
	{
	case 0:
        //we pass record length
		fseek(h_nuxfile, 3, SEEK_CUR);
        //We read quantity of elements
		int Cnt = 0;
		fread(&Cnt, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
		MSrez* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MSrez::ReadAny(h_nuxfile,version);
			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//========================================
//! Reading from the file
bool MSzArray::ReadBin(MFile* file,int version)
//========================================
{
	switch (version)
	{
	case 0:
        //we pass record length
		file->SetPosition(3, SEEK_CUR);
        //We read quantity of elements
		int Cnt = 0;
		file->GetData(&Cnt, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
		MSrez* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MSrez::ReadAny(file,version);
			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//========================================
//!  file recording
bool MSzArray::WriteBin(FILE* h_nuxfile,int version)
//========================================
{
	switch (version)
	{
	case 0:
        //we write down the identifier
		MString str = WhatTheArray();
		fwrite((char *)str, 3, 1, h_nuxfile);
        //We write down quantity of elements
		fwrite(&Size, 4, 1, h_nuxfile);
        //We write down lines
		MSrez* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = GetAt(i);
			pAtom->WriteBin(h_nuxfile,version);
		}
	}
	return true;
}

//========================================
//Creation of a cut on type
MSrez* CreateSrezByType(int iType)
//========================================
{
	switch(iType){
	case IDT_SHRIFT:
		return (MSrez*) new MShriftSrez();
	case IDT_PROIZVOL:
		return (MSrez*) new MProizvolSrez();
	case IDT_NAS_SHRIFT:
		return (MSrez*)new MNastrShriftSrez();
	default:
//		exit(15);
		return NULL;
	}
}
