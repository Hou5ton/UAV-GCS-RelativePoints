#include "plasti.h"
#include "Refglobal.h"

//===================================================================
//! Writing of layer in the UTP file
//params (FILE* h_nuxfile,int version)
bool MPlast::WriteBin(FILE* h_nuxfile, int )
//====================================================================
{
    //We determine the layer size
	unsigned int Size=GetSize(); 
    //We write down the layer size
	fwrite(&Size, 4, 1, h_nuxfile);
    //We write down layer type
	int type =0;
	type = GetType();
	fwrite(&type, sizeof(int), 1, h_nuxfile);
	return true;
}

//===================================================================================
//! Reading layer from the UTP-file
//params (FILE* h_nuxfile,int version)
bool MPlast::ReadBin(FILE* h_nuxfile, int )
{
    //Depending on the version
    //we read record length
	int iTemp;
	fread(&iTemp, 4, 1, h_nuxfile);
    //we read layer type
	int iType;
	fread(&iType, sizeof(int), 1, h_nuxfile);
		if (reverseReadFlag) {
		   reverseValue((char*)&iTemp, sizeof(iTemp) );
		   reverseValue((char*)&iType, sizeof(iType) );
		}   
    //Check on a layer correctness
	return true;
}

//===================================================================================
//! Reading layer from the UTP-file
//params (MFile* file,int version)
bool MPlast::ReadBin(MFile* file, int )
//===================================================================================
{
    //Depending on the version
    //we read record length
	int iTemp;
	file->GetData(&iTemp, 4, 1);
    //we read layer type
	int iType;
	file->GetData(&iType, sizeof(int), 1);
    //Check on a layer correctness
		if (reverseReadFlag) {
		   reverseValue((char*)&iTemp, sizeof(iTemp) );
		   reverseValue((char*)&iType, sizeof(iType) );
		}   
	return true;
}

//=======================================================
//! For reading any of layers
MPlast* MPlast::ReadAny(FILE* h_nuxfile, int version)
//=======================================================
{
	MPlast* pPlast= NULL;
    //Depending on the version
	switch (version){
	case 0:
		int iType;
        //We write down the layer size
		fread(&iType, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
        //We write down line type
		fread(&iType, sizeof(int), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
		pPlast = CreatePlastByType(iType);
		fseek(h_nuxfile, -8, SEEK_CUR);
		pPlast->ReadBin(h_nuxfile,version);
	}
	return pPlast;
}

//=======================================================
//! For reading any of layers
MPlast* MPlast::ReadAny(MFile* file, int version)
//=======================================================
{
	MPlast* pPlast= NULL;
    //Depending on the version
	switch (version){
	case 0:
		int iType;
        //We write down the layer size
		file->GetData(&iType, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
        //We write down line type
		file->GetData(&iType, sizeof(int), 1);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
		pPlast = CreatePlastByType(iType);
		file->SetPosition(-8, MAP_SEEK_CUR);
		pPlast->ReadBin(file,version);
	}
	return pPlast;
}

//=============================================================
// MPattern
//=============================================================
//==================================

MPattern::MPattern(unsigned char colorkod, unsigned char imagekod) : m_colorkod(colorkod),  m_imagekod(imagekod), m_image(0)
{ 

}

MPattern::MPattern(MPattern* pPat)
{
	Copy(pPat);
}

//! copying function
void MPattern::Copy(MPattern* pPat)
//==================================
{
    //if is what to copy
	if(pPat != NULL){
		MPlast::Copy((MPlast*)pPat);
		m_colorkod = pPat->m_colorkod;
		m_imagekod = pPat->m_imagekod;
        if (pPat->m_image)
        {
            m_image = new MImage();
            m_image->Copy(pPat->m_image);
        }
	}
	else
	{
		m_colorkod = 0;
		m_imagekod = 0;
		m_image = 0;
	}
}

bool MPattern::WriteBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::WriteBin(h_nuxfile, version);
    // we write down the image describing a pattern
	fwrite(&m_imagekod, sizeof(m_imagekod), 1, h_nuxfile);
    // we write down a code of color of filling
	fwrite(&m_colorkod, sizeof(m_colorkod), 1, h_nuxfile);
	return true;
}

//==================================
//! reading a pattern from the UTP-file
bool MPattern::ReadBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::ReadBin(h_nuxfile,version);
    // we read the image describing a pattern
    fread(&m_imagekod, sizeof(m_imagekod), 1, h_nuxfile);
    // we read a code of color of filling
	fread(&m_colorkod, sizeof(m_colorkod), 1, h_nuxfile);
	return true;
}

//==================================
//! reading a pattern from the UTP-file
bool MPattern::ReadBin(MFile* file, int version)
//==================================
{
	MPlast::ReadBin(file,version);
    // we read the image describing a pattern
    file->GetData(&m_imagekod, sizeof(m_imagekod), 1);
    // we read a code of color of filling
    file->GetData(&m_colorkod, sizeof(m_colorkod), 1);
	return true;
}

//=============================================================
// MZalivka
//=============================================================
//==================================
//! copying function
void MZalivka::Copy(MZalivka* zalivka)
//==================================
{
	MPlast::Copy((MPlast*)zalivka);
    //if is what to copy
	if(zalivka != NULL){
		m_colorkod = zalivka->m_colorkod;
	}
}

//==================================
//! Record of layer in the UTP file
bool MZalivka::WriteBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::WriteBin(h_nuxfile,version);
    // we write down a code of color of filling
	fwrite(&m_colorkod, sizeof(unsigned char), 1, h_nuxfile);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MZalivka::ReadBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::ReadBin(h_nuxfile,version);
    // we read down a code of color of filling

	fread(&m_colorkod, 1, 1, h_nuxfile);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MZalivka::ReadBin(MFile* file, int version)
//==================================
{
	MPlast::ReadBin(file,version);
    // we read down a code of color of filling

	file->GetData(&m_colorkod, 1, 1);
	return true;
}

//=============================================================
// MUzor
//=============================================================

//==================================
//! copying function
void MUzor::Copy(MUzor* uzor)
//==================================
{
	MPlast::Copy((MPlast*) uzor);
    //if is what to copy
	if(uzor != NULL){
		m_type = uzor->m_type;
		m_shag = uzor->m_shag;
		m_ugol = uzor->m_ugol;
		m_symbol.Copy(&uzor->m_symbol);
	}
}

//==================================
//! Record of layer in the UTP file
bool MUzor::WriteBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::WriteBin(h_nuxfile,version);
    // we write down grid type
	fwrite(&m_type, sizeof(int), 1, h_nuxfile);
    // we write down a grid step
	fwrite(&m_shag, sizeof(float), 1, h_nuxfile);
    // we write down a grid tilt angle in relation to a horizontal axis
	fwrite(&m_ugol, sizeof(float), 1, h_nuxfile);
    // we write down a symbol
	m_symbol.WriteSymbol(h_nuxfile, version);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MUzor::ReadBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::ReadBin(h_nuxfile,version);
    // we read down grid type
	fread(&m_type, sizeof(int), 1, h_nuxfile); // danger length=1
    // we read down a grid step
	fread(&m_shag, sizeof(m_shag), 1, h_nuxfile);
    // we read down a grid tilt angle in relation to a horizontal axis
	fread(&m_ugol, sizeof(m_ugol), 1, h_nuxfile);
		if (reverseReadFlag) {
//		  reverseValue((char*)&m_type, sizeof(m_type) );  // vipa 2004.10.29
		  reverseValue((char*)&m_shag, sizeof(m_shag) );
		  reverseValue((char*)&m_ugol, sizeof(m_ugol) );
		}  
    // we read a symbol
	m_symbol.ReadSymbol(h_nuxfile, version);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MUzor::ReadBin(MFile* file, int version)
//==================================
{
	MPlast::ReadBin(file,version);
    // we read down grid type
    file->GetData(&m_type, sizeof(int), 1);
    // we read down a grid step
    file->GetData(&m_shag, sizeof(m_shag), 1);
    // we read down a grid tilt angle in relation to a horizontal axis
    file->GetData(&m_ugol, sizeof(m_ugol), 1);
		if (reverseReadFlag) {
//		  reverseValue((char*)&m_type, sizeof(m_type) ); // vipa 2004.10.29
		  reverseValue((char*)&m_shag, sizeof(m_shag) );
		  reverseValue((char*)&m_ugol, sizeof(m_ugol) );
		}  
        // we read a symbol
    m_symbol.ReadSymbol(file, version);
	return true;
}

//=============================================================
// MShtrihovka
//=============================================================

//==================================
//! copying function
void MShtrihovka::Copy(MShtrihovka* shtrihovka)
//==================================
{
	MPlast::Copy((MPlast*)shtrihovka);
    //if is, from where to copy
	if(shtrihovka != NULL){
		m_type = shtrihovka->m_type;
		m_ugol = shtrihovka->m_ugol;
		m_shag = shtrihovka->m_shag;
		m_thickness = shtrihovka->m_thickness;
		m_colorkod = shtrihovka->m_colorkod;
	}
}

//==================================
//! Record of layer in the UTP file
bool MShtrihovka::WriteBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::WriteBin(h_nuxfile,version);
    // we write down shading type
	fwrite(&m_type, sizeof(int), 1, h_nuxfile);
    // we write down a shading angle
	fwrite(&m_ugol, sizeof(float), 1, h_nuxfile);
    // we write down a shading step
	fwrite(&m_shag, sizeof(float), 1, h_nuxfile);
    // we write down stroke thickness
	fwrite(&m_thickness, sizeof(float), 1, h_nuxfile);
    // we write down a code of color of the line
	fwrite(&m_colorkod, sizeof(unsigned char), 1, h_nuxfile);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MShtrihovka::ReadBin(FILE* h_nuxfile, int version)
//==================================
{
	MPlast::ReadBin(h_nuxfile,version);
    // we read shading type
	fread(&m_type, sizeof(int), 1, h_nuxfile);
    // we read a shading angle
	fread(&m_ugol, sizeof(float), 1, h_nuxfile);
    // we read a shading step
	fread(&m_shag, sizeof(float), 1, h_nuxfile);
    // we read stroke thickness
	fread(&m_thickness, sizeof(float), 1, h_nuxfile);
    // we read colors of shading
	fread(&m_colorkod, 1, 1, h_nuxfile);
		if (reverseReadFlag) {
		  reverseValue((char*)&m_type, sizeof(m_type) );
		  reverseValue((char*)&m_ugol, sizeof(m_ugol) );
		  reverseValue((char*)&m_shag, sizeof(m_shag) );
		  reverseValue((char*)&m_thickness, sizeof(m_thickness) );
//		  reverseValue((char*)&m_colorkod, sizeof(m_colorkod) );
		}  
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MShtrihovka::ReadBin(MFile* file, int version)
//==================================
{
	MPlast::ReadBin(file,version);
    // we read shading type
    file->GetData(&m_type, sizeof(int), 1);
    // we read a shading angle
    file->GetData(&m_ugol, sizeof(float), 1);
    // we read a shading step
    file->GetData(&m_shag, sizeof(float), 1);
    // we read stroke thickness
    file->GetData(&m_thickness, sizeof(float), 1);
    // we read colors of shading
    file->GetData(&m_colorkod, 1, 1);
		if (reverseReadFlag) {
		  reverseValue((char*)&m_type, sizeof(m_type) );
		  reverseValue((char*)&m_ugol, sizeof(m_ugol) );
		  reverseValue((char*)&m_shag, sizeof(m_shag) );
		  reverseValue((char*)&m_thickness, sizeof(m_thickness) );
//		  reverseValue((char*)&m_colorkod, sizeof(m_colorkod) );
		}  
	return true;
}

//=================================================================================
//					MPercentUzor
//	The class which is responsible for storage of parameters for percentage filling
//=================================================================================

// ==================================================================
//! -----------
void MPercentUzor::Copy(MPercentUzor* uzor)
// ==================================================================
{
	MUzor::Copy(uzor);
	m_SecSym.Copy(&uzor->m_SecSym);
	m_ThirdSym.Copy(&uzor->m_ThirdSym);
	m_iPercent1 = uzor->m_iPercent1;
	m_iPercent2 = uzor->m_iPercent2;
	m_iPercent3 = uzor->m_iPercent3;
}

//==================================
//! Record of layer in the UTP file
bool MPercentUzor::WriteBin(FILE* h_nuxfile, int version)
//==================================
{
	MUzor::WriteBin(h_nuxfile,version);
	m_SecSym.WriteSymbol(h_nuxfile, version);
	m_ThirdSym.WriteSymbol(h_nuxfile, version);
				
	fwrite(&m_iPercent1, sizeof(unsigned char), 1, h_nuxfile);
	fwrite(&m_iPercent2, sizeof(unsigned char), 1, h_nuxfile);
	fwrite(&m_iPercent3, sizeof(unsigned char), 1, h_nuxfile);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MPercentUzor::ReadBin(FILE* h_nuxfile, int version)
//==================================
{
	MUzor::ReadBin(h_nuxfile,version);
	m_SecSym.ReadSymbol(h_nuxfile, version);
	m_ThirdSym.ReadSymbol(h_nuxfile, version);

	fread(&m_iPercent1, sizeof(unsigned char), 1, h_nuxfile);
	fread(&m_iPercent2, sizeof(unsigned char), 1, h_nuxfile);
	fread(&m_iPercent3, sizeof(unsigned char), 1, h_nuxfile);
	return true;
}

//==================================
//! Reading layer from the UTP-file
bool MPercentUzor::ReadBin(MFile* file, int version)
//==================================
{
	MUzor::ReadBin(file,version);
	m_SecSym.ReadSymbol(file, version);
	m_ThirdSym.ReadSymbol(file, version);

	file->GetData(&m_iPercent1, sizeof(unsigned char), 1);
	file->GetData(&m_iPercent2, sizeof(unsigned char), 1);
	file->GetData(&m_iPercent3, sizeof(unsigned char), 1);
	return true;
}

//========================================
//! Reading from file
bool MPlArray::ReadBin(FILE* h_nuxfile,int version)
//========================================
{
	switch (version)
	{
	case 0:
        //we pass record length
		fseek(h_nuxfile, 3, SEEK_CUR);
        //We read quantity of elements
		unsigned int Cnt = 0;
		fread(&Cnt, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
		MPlast* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MPlast::ReadAny(h_nuxfile,version);
			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//========================================
//! Reading from file
bool MPlArray::ReadBin(MFile* file,int version)
//========================================
{
	switch (version)
	{
	case 0:
        //we pass record length
        file->SetPosition(3, MAP_SEEK_CUR);
        //We read quantity of elements
        unsigned int Cnt = 0;
		file->GetData(&Cnt, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
        MPlast* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MPlast::ReadAny(file,version);
			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//========================================
//! Writing in a file
bool MPlArray::WriteBin(FILE* h_nuxfile,int version)
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
		MPlast* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = GetAt(i);
			pAtom->WriteBin(h_nuxfile,version);
		}
	}
	return true;
}

//========================================
//! Creation of a layer on type
MPlast* CreatePlastByType(int iType)
//========================================
{
	switch(iType){
	case PLT_SHTRIHOVKA:
		return (MPlast*) new MShtrihovka();
	case PLT_ZALIVKA:
		return (MPlast*) new MZalivka();
	case PLT_UZOR:
		return (MPlast*) new MUzor();
	case PLT_PATTERN:
		return (MPlast*) new MPattern();

	case PLT_PERCENT_UZOR:
		return (MPlast*) new MPercentUzor();
	case PLT_NAS_SHTRIHOVKA:
		return (MPlast*) new MNastrShtrihovka();
	case PLT_NAS_UZOR:
		return (MPlast*) new MNastrUzor();
	case PLT_NAS_PERCENT_UZOR:
		return (MPlast*) new MNastrPercentUzor();
	case PLT_HANDLE:
		return (MPlast*) new MHandleArea();
	case PLT_NAS_HANDLE:
		return (MPlast*) new MNastrHandleArea();
	default:
//		exit(15);
		return NULL;
	}
}
