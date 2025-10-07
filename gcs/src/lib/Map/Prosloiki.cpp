#include "Prosloiki.h"
#include "Refglobal.h"

//=============================================================
// MProsloika
//=============================================================
//!constructor with the set standard
MProsloika::MProsloika(MProsloika* lin)
{
	if(lin){
		m_distofaxe = lin->m_distofaxe;
		m_synhro = lin->m_synhro;
		m_drawrazryv = lin->m_drawrazryv;
	}
	else{
		m_distofaxe = 0;
		m_synhro = 0;
		m_drawrazryv = true;
	}
}

//==================================
//!copying
void MProsloika::Copy(MProsloika* lin)
{
	if(lin){
		m_distofaxe = lin->m_distofaxe;
		m_synhro = lin->m_synhro;
		m_drawrazryv = lin->m_drawrazryv;
	}
	else{
		m_distofaxe = 0;
		m_synhro = 0;
		m_drawrazryv = true;
	}
}


//==============================================
//!writing of a layer in the UTP file
bool MProsloika::WriteBin(FILE* h_nuxfile, int version)
{
    //We determine the layer size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //We write down the layer size
		fwrite(&Size, 4, 1, h_nuxfile);
		int iType = (int)GetType();
        //We write down line type
		fwrite(&iType, sizeof(int), 1, h_nuxfile);
        //We write down shift from an axis
		fwrite(&m_distofaxe, sizeof(float), 1, h_nuxfile);
        //We write down a synchronization flag
		fwrite(&m_synhro, 1, 1, h_nuxfile);
        //We write down a flag of drawing of breaks
		fwrite(&m_drawrazryv, 1, 1, h_nuxfile);
	}
	return true;
}

//=========================================
//!reading a layer from the UTP file
bool MProsloika::ReadBin(FILE* h_nuxfile, int version)
{
    //Depending on the version
	switch (version){
	case 0:
		int iTemp;
        //We write down the layer size
		fread(&iTemp, 4, 1, h_nuxfile);
        //We write down line type
		fread(&iTemp, sizeof(int), 1, h_nuxfile);
        //we read shift from an axis
		fread(&m_distofaxe, sizeof(float), 1, h_nuxfile);
        //we read a synchronization flag
		fread(&m_synhro, 1, 1, h_nuxfile);
        //we read a flag of drawing of breaks
		fread(&m_drawrazryv, 1, 1, h_nuxfile);
		if (reverseReadFlag) {
//		  reverseValue((char*)&iTemp, sizeof(iTemp) );
		  reverseValue((char*)&m_distofaxe, sizeof(m_distofaxe) );
//		  reverseValue((char*)&m_synhro, sizeof(m_synhro) );
//		  reverseValue((char*)&m_drawrazryv, sizeof(m_drawrazryv) );
		}  
        // depending on line type, we write down information on the line...
	}
	return true;
}

//=========================================
//!reading a layer from the UTP file
bool MProsloika::ReadBin(MFile* file, int version)
{
    //Depending on the version
	switch (version){
	case 0:
		int iTemp;
        //We write down the layer size
		file->GetData(&iTemp, 4, 1);
        //We write down line type
		file->GetData(&iTemp, sizeof(int), 1);
        //we read shift from an axis
		file->GetData(&m_distofaxe, sizeof(float), 1);
        //we read a synchronization flag
		file->GetData(&m_synhro, 1, 1);
        //we read a flag of drawing of breaks
		file->GetData(&m_drawrazryv, 1, 1);
		if (reverseReadFlag) {
//		  reverseValue((char*)&iTemp, sizeof(iTemp) );
		  reverseValue((char*)&m_distofaxe, sizeof(m_distofaxe) );
//		  reverseValue((char*)&m_synhro, sizeof(m_synhro) );
//		  reverseValue((char*)&m_drawrazryv, sizeof(m_drawrazryv) );
		}  
        // depending on line type, we write down information on the line...
	}
	return true;
}

//=======================================================
//!For reading any of layers
MProsloika* MProsloika::ReadAny(FILE* h_nuxfile, int version)
{
	MProsloika* pPros =NULL;
    //Depending on the version
	switch (version){
	case 0:
		int iType;
        //We write down the layer size
		fread(&iType, 4, 1, h_nuxfile);
        //We write down line type
		fread(&iType, sizeof(int), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
		pPros = CreateProsloikaByType(iType);
		fseek(h_nuxfile, -8, SEEK_CUR);
		pPros->ReadBin(h_nuxfile,version);
	}
	return pPros;
}

MProsloika* MProsloika::ReadAny(MFile* file, int version)
{
	MProsloika* pPros =NULL;
    //Depending on the version
	switch (version){
	case 0:
		int iType;
        //We write down the layer size
		file->GetData(&iType, 4, 1);
        //We write down line type
		file->GetData(&iType, sizeof(int), 1);
		if (reverseReadFlag) reverseValue((char*)&iType, sizeof(iType) );
		pPros = CreateProsloikaByType(iType);
		file->SetPosition(-8, MAP_SEEK_CUR);
		pPros->ReadBin(file,version);
	}
	return pPros;
}

//=============================================================
// MLinia
//=============================================================
//==================================
//!constructor with the set standard
MLinia::MLinia(MLinia* lin):MProsloika((MProsloika*)lin)
{
    //if there is a standard, we copy it
	if(lin)
	{
		m_thickness = lin->m_thickness;
		m_colorkod  = lin->m_colorkod;
	}
    //otherwise we create by default
	else
	{
		m_thickness = (float)0.1;
		m_colorkod  = 0;
	}
}

//==================================
//! copying function
void MLinia::Copy(MLinia* linia)
{
	MProsloika::Copy((MProsloika*)linia);
    //if is what to copy
	if(linia != NULL){
		m_thickness = linia->m_thickness;
		m_colorkod  = linia->m_colorkod;
	}
}



//==================================
//! writing of a layer in the UTP file
bool MLinia::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down line thickness
	fwrite(&m_thickness, sizeof(float), 1, h_nuxfile);
    // we write down a code of color of the line
	fwrite(&m_colorkod, sizeof(unsigned char), 1, h_nuxfile);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MLinia::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read line thickness
	fread(&m_thickness, sizeof(float), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_thickness, sizeof(m_thickness) );
    // we read a code of color of the line
	fread(&m_colorkod, sizeof(unsigned char), 1, h_nuxfile);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MLinia::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
    // we read line thickness
	file->GetData(&m_thickness, sizeof(float), 1);
		if (reverseReadFlag) reverseValue((char*)&m_thickness, sizeof(m_thickness) );
    // we read a code of color of the line
	file->GetData(&m_colorkod, sizeof(unsigned char), 1);
	return true;
}


//=============================================================
// MSbStretch
//=============================================================

//==================================
//! constructor with the set parameters
MSbStretch::MSbStretch(float pause, MSymbol* sym, float dlprob)
{
	m_sbpause = pause;
	m_symbol.Copy(sym);
	m_period = dlprob;
}

//==================================
//!constructor on the set object
MSbStretch::MSbStretch(MSbStretch* sbstr)
{
	Copy(sbstr);
}

//==================================
//! copying function
void MSbStretch::Copy(MSbStretch* sbstretch)
{
	MProsloika::Copy((MProsloika*)sbstretch);
    //we copy all members of a class if the entrance object exists
	if(sbstretch != NULL){
		m_sbpause = sbstretch->m_sbpause;
		m_symbol.Copy(&sbstretch->m_symbol);
		m_period = sbstretch->m_period;
	}
}


//==================================
//!writing of a layer in the UTP file
bool MSbStretch::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down shift of the first symbol from the beginning of the line
	fwrite(&m_sbpause, sizeof(float), 1, h_nuxfile);
    // we write down space length between symbols
	fwrite(&m_period, sizeof(float), 1, h_nuxfile);
    // we write down a symbol
	m_symbol.WriteSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MSbStretch::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read shift of the first symbol from the beginning of the line
	fread(&m_sbpause, sizeof(float), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_sbpause, sizeof(m_sbpause) );
    // we read space length between symbols
	fread(&m_period, sizeof(float), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_period, sizeof(m_period) );
    // we read a symbol
	m_symbol.ReadSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MSbStretch::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
    // we read shift of the first symbol from the beginning of the line
	file->GetData(&m_sbpause, sizeof(float), 1);
		if (reverseReadFlag) reverseValue((char*)&m_sbpause, sizeof(m_sbpause) );
    // we read space length between symbols
	file->GetData(&m_period, sizeof(float), 1);
		if (reverseReadFlag) reverseValue((char*)&m_period, sizeof(m_period) );
    // we read a symbol
	m_symbol.ReadSymbol(file, version);
	return true;
}


//=============================================================
// MShtrih
//=============================================================
//==================================
//!constructor with the set standard
MShtrih::MShtrih(MShtrih* shtr):MProsloika((MProsloika*)shtr)
{
    //if a standard not empty, we make its copy
	if(shtr)
	{
		m_thickness = shtr->m_thickness;
		m_colorkod = shtr->m_colorkod;
		m_dlina = shtr->m_dlina;
		m_period = shtr->m_period;
		m_kvadrat=shtr->m_kvadrat;
		m_otstup=shtr->m_otstup;
	}
    //otherwise the object by default is created
	else
	{
		m_thickness = 0.1f;
		m_colorkod = 0;
		m_dlina = 1;
		m_period = 1;
		m_kvadrat=true;
		m_otstup=0;
	}
}
//==================================
//! copying function
void MShtrih::Copy(MShtrih* shtrih)
{
	MProsloika::Copy((MProsloika*)shtrih);
    //we copy if is what to copy
	if(shtrih != NULL){
		m_thickness = shtrih->m_thickness;
		m_colorkod = shtrih->m_colorkod;
		m_dlina = shtrih->m_dlina;
		m_period = shtrih->m_period;
		m_kvadrat=shtrih->m_kvadrat;
		m_otstup=shtrih->m_otstup;
	}
}


//==================================
//!writing of a layer in the UTP file
bool MShtrih::WriteBin(FILE* h_nuxfile,int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down line thickness
	fwrite(&m_thickness, sizeof(float), 1, h_nuxfile);
    // we write down stroke length
	fwrite(&m_dlina, sizeof(float), 1, h_nuxfile);
    // we write down space length
	fwrite(&m_period, sizeof(float), 1, h_nuxfile);
    // we write down a code of color of the line
	fwrite(&m_colorkod, 1, 1, h_nuxfile);
    //squareness flag
	fwrite(&m_kvadrat, 1, 1, h_nuxfile);
    // we write down a stroke space
	fwrite(&m_otstup, sizeof(float), 1, h_nuxfile);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MShtrih::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read line thickness
	fread(&m_thickness, sizeof(float), 1, h_nuxfile);
    // we read stroke length
	fread(&m_dlina, sizeof(float), 1, h_nuxfile);
    // we read space length
	fread(&m_period, sizeof(float), 1, h_nuxfile);
    // we read a code of color of the line
	fread(&m_colorkod, 1, 1, h_nuxfile);
    //squareness flag
	fread(&m_kvadrat, 1, 1, h_nuxfile);
    // we read a stroke space
	fread(&m_otstup, sizeof(float), 1, h_nuxfile);
	if (reverseReadFlag) {
		reverseValue((char*)&m_thickness, sizeof(m_thickness) );
		reverseValue((char*)&m_dlina,  sizeof(m_dlina) );
		reverseValue((char*)&m_period, sizeof(m_period) );
		reverseValue((char*)&m_otstup, sizeof(m_otstup) );
	}
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MShtrih::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
    // we read line thickness
	file->GetData(&m_thickness, sizeof(float), 1);
    // we read stroke length
	file->GetData(&m_dlina, sizeof(float), 1);
    // we read space length
	file->GetData(&m_period, sizeof(float), 1);
    // we read a code of color of the line
	file->GetData(&m_colorkod, 1, 1);
    //squareness flag
	file->GetData(&m_kvadrat, 1, 1);
    // we read a stroke space
	file->GetData(&m_otstup, sizeof(float), 1);
	if (reverseReadFlag) {
		reverseValue((char*)&m_thickness, sizeof(m_thickness) );
		reverseValue((char*)&m_dlina,  sizeof(m_dlina) );
		reverseValue((char*)&m_period, sizeof(m_period) );
		reverseValue((char*)&m_otstup, sizeof(m_otstup) );
	}
	return true;
}

//=============================================================
// MPunktir
//=============================================================
//==================================
//!constructor with the set standard
MPunktir::MPunktir(MPunktir* punk):MProsloika((MProsloika*)punk)
{
    //if the standard exists, we copy it
	if(punk)
	{
		Copy(punk);
	}
    //otherwise we create by default
	else
	{
		m_diametr = (float)0.1;
		m_colorkod = 0;
		m_period = 1;
		m_nachalo = 0;
	}
}

//==================================
//! copying function
void MPunktir::Copy(MPunktir* punktir)
{
	MProsloika::Copy((MProsloika*)punktir);
    //we copy if is what to copy
	if(punktir != NULL){
		m_diametr = punktir->m_diametr;
		m_colorkod = punktir->m_colorkod;
		m_period = punktir->m_period;
		m_nachalo = punktir->m_nachalo;
	}
}


//==================================
//!writing of a layer in the UTP file

bool MPunktir::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down diameter of a point of a dotted line
	fwrite(&m_diametr, sizeof(float), 1, h_nuxfile);
    // we write down space length
	fwrite(&m_period, sizeof(float), 1, h_nuxfile);
    // we write down a code of color of the line
	fwrite(&m_colorkod, sizeof(unsigned char), 1, h_nuxfile);
    //shift of the first dotted line
	fwrite(&m_nachalo, sizeof(float), 1, h_nuxfile);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MPunktir::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read diameter of a point of a dotted line
	fread(&m_diametr, sizeof(float), 1, h_nuxfile);
    // we read space length
	fread(&m_period, sizeof(float), 1, h_nuxfile);
    // we read a code of color of the line
	fread(&m_colorkod, sizeof(unsigned char), 1, h_nuxfile);
    //shift of the first dotted line
	fread(&m_nachalo, sizeof(float), 1, h_nuxfile);
	if (reverseReadFlag) {
	  reverseValue((char*)&m_diametr, sizeof(m_diametr) );
	  reverseValue((char*)&m_period,  sizeof(m_period) );
	  reverseValue((char*)&m_nachalo, sizeof(m_nachalo) );
        }
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MPunktir::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
    // we read diameter of a point of a dotted line
	file->GetData(&m_diametr, sizeof(float), 1);
    // we read space length
	file->GetData(&m_period, sizeof(float), 1);
    // we read a code of color of the line
	file->GetData(&m_colorkod, sizeof(unsigned char), 1);
    //shift of the first dotted line
	file->GetData(&m_nachalo, sizeof(float), 1);
	if (reverseReadFlag) {
	  reverseValue((char*)&m_diametr, sizeof(m_diametr) );
	  reverseValue((char*)&m_period,  sizeof(m_period) );
	  reverseValue((char*)&m_nachalo, sizeof(m_nachalo) );
        }
	return true;
}

//======================================================
// MRangeSym
//======================================================
//==================================================
//!constructor with the set standard
MRangeSym::MRangeSym(MRangeSym* rgsym)
{
    //if the standard exists, we copy it
	if(rgsym)
	{
		Copy(rgsym);	
	}
    //otherwise we create by default
	else
	{
		m_type=0;
	}
}

//========================================
//!copying function
void MRangeSym::Copy(MRangeSym* rangesym)
{
	MProsloika::Copy((MProsloika*)rangesym);
	m_type=rangesym->m_type;
	m_symbol.Copy(&rangesym->m_symbol);
}



//==================================
//!writing of a layer in the UTP file
bool MRangeSym::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down symbol type
	fwrite(&m_type, 1, 1, h_nuxfile);
    // we write down a symbol
	m_symbol.WriteSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MRangeSym::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read symbol type
	fread(&m_type, 1, 1, h_nuxfile);
    // we read a symbol
	m_symbol.ReadSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MRangeSym::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
    // we read symbol type
	file->GetData(&m_type, 1, 1);
    // we read a symbol
	m_symbol.ReadSymbol(file, version);
	return true;
}

//======================================================
// MOrientedStretch
//======================================================
//==================================================
//!constructor with the set standard
MOrientedStretch::MOrientedStretch(MOrientedStretch* rgsym)
{
    //if the standard exists, we copy it
	if(rgsym)
	{
		
		Copy(rgsym);	
	}
    //otherwise we create by default
	else
	{
		m_type=MOS_NOORIENT;
	}
}

//========================================
//!copying function
void MOrientedStretch::Copy(MOrientedStretch* rangesym)
{
	MProsloika::Copy((MProsloika*)rangesym);
	m_type=rangesym->m_type;
	m_symbol.Copy(&rangesym->m_symbol);
}


//==================================
//!writing of a layer in the UTP file
bool MOrientedStretch::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down symbol type
	fwrite(&m_type, 1, 1, h_nuxfile);
    // we write down a symbol
	m_symbol.WriteSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MOrientedStretch::ReadBin(FILE* h_nuxfile,int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read symbol type
	fread(&m_type, 1, 1, h_nuxfile);
    // we read a symbol
	m_symbol.ReadSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MOrientedStretch::ReadBin(MFile* file,int version) 
{
	MProsloika::ReadBin(file,version);
    // we read symbol type
	file->GetData(&m_type, 1, 1);
    // we read a symbol
	m_symbol.ReadSymbol(file, version);
	return true;
}

//=============================================================
// MSpline
//=============================================================
//==================================
//!constructor with the set standard
MSpline::MSpline(MSpline* spline)
{
	
    //if a standard not empty, we make its copy
	if(spline)
	{
		Copy(spline);
	}
    //otherwise the object by default is created
	else
	{
		m_thickness = (float)0.1;
		m_colorkod = 0;
		m_type = SPT_APPROX;
		m_vid = SPV_NEZAMKNUT;
		m_kolpts = 10;
	}
}

//==================================
//! copying function
void MSpline::Copy(MSpline* spline)
{
	MProsloika::Copy((MProsloika*)spline);
    //we copy if is what to copy
	if(spline != NULL){
		m_thickness = spline->m_thickness;
		m_colorkod = spline->m_colorkod;
		m_type = spline->m_type;
		m_vid = spline->m_vid;
		m_kolpts = spline->m_kolpts;
	}
}


//==================================
//!writing of a layer in the UTP file
bool MSpline::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down line thickness
	fwrite(&m_thickness, sizeof(float), 1, h_nuxfile);
    // we write down spline type
	fwrite(&m_type, 1, 1, h_nuxfile);
    // we write down a view of a spline
	fwrite(&m_vid, 1, 1, h_nuxfile);
    // we write down number of points in splitting an edge
	fwrite(&m_kolpts, sizeof(unsigned char), 1, h_nuxfile);
    // we write down a code of color of a spline
	fwrite(&m_colorkod, 1, 1, h_nuxfile);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MSpline::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
    // we read line thickness
    fread(&m_thickness, sizeof(float), 1, h_nuxfile);
  	if (reverseReadFlag)  reverseValue((char*)&m_thickness, sizeof(m_thickness) );
    // we read spline type
    fread(&m_type, 1, 1, h_nuxfile);
    // we read a view of a spline
    fread(&m_vid, 1, 1, h_nuxfile);
    // we read number of points in splitting an edge
	fread(&m_kolpts, sizeof(unsigned char), 1, h_nuxfile);
    // we read a code of color of a spline
	fread(&m_colorkod, 1, 1, h_nuxfile);
        
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MSpline::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
    // we read line thickness
    file->GetData(&m_thickness, sizeof(float), 1);
  	if (reverseReadFlag)  reverseValue((char*)&m_thickness, sizeof(m_thickness) );
    // we read spline type
    file->GetData(&m_type, 1, 1);
    // we read a view of a spline
    file->GetData(&m_vid, 1, 1);
    // we read number of points in splitting an edge
    file->GetData(&m_kolpts, sizeof(unsigned char), 1);
    // we read a code of color of a spline
    file->GetData(&m_colorkod, 1, 1);
	return true;
}

//=============================================================
// MSbSingular
//=============================================================
//==================================
//! copying function
MSbSingular::MSbSingular(MSymbol* sym, float f_pause) 
{
	m_symbol.Copy(sym);
    //we do m_pause from 0 to 1, without changing-1
	if(f_pause>1) m_pause = 1;
	else if(f_pause>0) m_pause = f_pause;
		 else if(f_pause != -1) m_pause = 0;
}

//==================================
//! copying function
void MSbSingular::Copy(MSbSingular* sbsymbol)
{
	MProsloika::Copy((MProsloika*)sbsymbol);
    //we copy if the entrance object exists
	if(sbsymbol) 
	{
		m_symbol.Copy(&sbsymbol->m_symbol);
		m_pause = sbsymbol->m_pause;
	}
}


//==================================
//!writing of a layer in the UTP file
bool MSbSingular::WriteBin(FILE* h_nuxfile, int version)
{
	MProsloika::WriteBin(h_nuxfile,version);
    // we write down shift of the first symbol from the beginning of the line
	fwrite(&m_pause, sizeof(float), 1, h_nuxfile);	
    // we write down a symbol
	m_symbol.WriteSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MSbSingular::ReadBin(FILE* h_nuxfile, int version) 
{
	MProsloika::ReadBin(h_nuxfile,version);
	fread(&m_pause, sizeof(float), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_pause, sizeof(m_pause) );
    // we read a symbol
	m_symbol.ReadSymbol(h_nuxfile, version);
	return true;
}

//==================================
//!reading a layer from the UTP file
bool MSbSingular::ReadBin(MFile* file, int version) 
{
	MProsloika::ReadBin(file,version);
	file->GetData(&m_pause, sizeof(float), 1);
		if (reverseReadFlag) reverseValue((char*)&m_pause, sizeof(m_pause) );
    // we read a symbol
	m_symbol.ReadSymbol(file, version);
	return true;
}



//======================================================
// MAlgline
//======================================================
//========================================
//!copying function
void MAlgline::Copy(MAlgline* pAction)
{
	m_stepahead=pAction->m_stepahead;
	m_stepaside=pAction->m_stepaside;
	m_draw=pAction->m_draw;
}


//========================================
//!Reading from the file
bool MPrArray::ReadBin(FILE* h_nuxfile,int version)
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
		MProsloika* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MProsloika::ReadAny(h_nuxfile,version);
			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//========================================
//!Reading from the file
bool MPrArray::ReadBin(MFile* file,int version)
{
	switch (version)
	{
	case 0:
        //we pass record length
		file->SetPosition(3, MAP_SEEK_CUR);
        //We read quantity of elements
		int Cnt = 0;
		file->GetData(&Cnt, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
		MProsloika* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MProsloika::ReadAny(file,version);
			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//========================================
//! file recording
bool MPrArray::WriteBin(FILE* h_nuxfile,int version)
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
		MProsloika* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = GetAt(i);
			pAtom->WriteBin(h_nuxfile,version);
		}
	}
	return true;
}



//========================================================================
//========================================================================
//	MUshArray m_PereoprIndexes;
//	MStArray m_PereoprNames;
//===============================================
//!class  MBaseNastr
//!Initialization of the array
/**The class which is responsible for redefinition of various parameters for the basic
//classes This class is responsible for storage of arrays of redefinition
//their preservation and also contains the virtual interface for editing
*/
MBaseNastr::MBaseNastr(int iSize)
{
	m_PereoprIndexes.SetSize(iSize);
	unsigned short temp = PEREOPR_NOT_EXIST;
	for(int i = 0;i<iSize;i++)
		m_PereoprIndexes.SetAt(i,&temp);
}

//================================================
//!Record of those redefinitions which exist
bool MBaseNastr::WriteBin(FILE* h_nuxfile, int /*version*/)
{
    //At first we find out quantity of the redefined elements
	int iSize = m_PereoprIndexes.GetSize();
	unsigned char iWrSize=0;
	int i = 0;
	for(i = 0;i<iSize;i++){
		if(*m_PereoprIndexes.GetAt(i)!=PEREOPR_NOT_EXIST)
			iWrSize++;
	};
	fwrite(&iWrSize, sizeof(unsigned char), 1, h_nuxfile);
	for(i = 0;i<iSize;i++){
		if(*m_PereoprIndexes.GetAt(i)!=PEREOPR_NOT_EXIST){
			fwrite(&i, sizeof(unsigned char), 1, h_nuxfile);
			fwrite(m_PereoprIndexes.GetAt(i), sizeof(unsigned short), 1, h_nuxfile);
		}
	}
	return true;
}

//======================================================
//!Reading those redefinitions which exist
bool MBaseNastr::ReadBin(FILE* h_nuxfile, int /*version*/)
{
    //At first we find out quantity of the redefined elements
	int iSize = m_PereoprIndexes.GetSize();
	unsigned char iRdSize;
	fread(&iRdSize, 1, 1, h_nuxfile);
	unsigned char bIndex;
	unsigned short ushValue;
	for(int i = 0;i<iRdSize;i++){
		fread(&bIndex, 1, 1, h_nuxfile);
		fread(&ushValue, sizeof(unsigned short), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&ushValue, sizeof(ushValue) );
		if(bIndex<iSize)
			m_PereoprIndexes.SetAt(bIndex,&ushValue);
	}
	return true;
}

//======================================================
//!Reading those redefinitions which exist
bool MBaseNastr::ReadBin(MFile* file, int /*version*/)
{
    //At first we find out quantity of the redefined elements
	int iSize = m_PereoprIndexes.GetSize();
	unsigned char iRdSize;
	file->GetData(&iRdSize, 1, 1);
	unsigned char bIndex;
	unsigned short ushValue;
	for(int i = 0;i<iRdSize;i++){
		file->GetData(&bIndex, 1, 1);
		file->GetData(&ushValue, sizeof(unsigned short), 1);
		if (reverseReadFlag) reverseValue((char*)&ushValue, sizeof(ushValue) );
		if(bIndex<iSize)
			m_PereoprIndexes.SetAt(bIndex,&ushValue);
	}
	return true;
}

//======================================================
//!To receive an index of the redefined parameter
unsigned short  MBaseNastr::GetPerIndex(int index)
{
	return *m_PereoprIndexes.GetAt(index);
}


//======================================================
//!Whether parameter is redefined
bool MBaseNastr::IsPereoprIndex(int iIndex)
{
	if(*m_PereoprIndexes.GetAt(iIndex)==PEREOPR_NOT_EXIST)
		return false;
	return true;
}

//!========================================================
void MBaseNastr::Copy(MBaseNastr* pBase)
{
	int iSize = pBase->m_PereoprIndexes.GetSize();
	m_PereoprIndexes.SetSize(iSize);
	for(int i = 0;i<iSize;i++)
		m_PereoprIndexes.SetAt(i,pBase->m_PereoprIndexes.GetAt(i));
}

//==========================================================
//!We write down the size in the file
int  MBaseNastr::GetSize()
{
	int iWrSize = 0;
	int iSize = m_PereoprIndexes.GetSize();
    //At first we find out quantity of the redefined elements
	for(int i = 0;i<iSize;i++)
		if(*m_PereoprIndexes.GetAt(i)!=PEREOPR_NOT_EXIST)
			iWrSize++;
    //The written quantity consists from
    //1b - num
    //3b - everyone record
	return 1+iWrSize*(sizeof(unsigned short)+sizeof(unsigned char));
}




//!Creation of a layer on type
MProsloika* CreateProsloikaByType(int iType)
{
	switch(iType){
	case IDL_LINIA:
		return (MProsloika*) new MLinia();
	case IDL_SHTRIH:
		return (MProsloika*) new MShtrih();
	case IDL_PUNKTIR:
		return (MProsloika*) new MPunktir();
	case IDL_SBSTRETCH:
		return (MProsloika*) new MSbStretch();
	case IDL_SOFRANGE:
		return (MProsloika*) new MRangeSym();
	case IDL_ALGORITM:
		return (MProsloika*) new MAlgline();
	case IDL_SPLINE:
		return (MProsloika*) new MSpline();
	case IDL_ORIENTED:
		return (MProsloika*) new MOrientedStretch();
	case IDL_NAS_LINIA:
		return (MProsloika*) new MNastrLinia();
	case IDL_NAS_SHTRIH:
		return (MProsloika*) new MNastrShtrih();
	case IDL_NAS_PUNKTIR:
		return (MProsloika*) new MNastrPunktir();
	case IDL_NAS_SPLINE:
		return (MProsloika*) new MNastrSpline();
	case IDL_NAS_SBSTRETCH:
		return (MProsloika*) new MNastrSbStretch();
	case IDL_SINGULAR:
		return (MProsloika*) new MSbSingular();
	case IDL_NAS_SINGULAR:
		return (MProsloika*) new MNastrSbSingular();
		
	default:
//		exit(15);
		return NULL;
	}
}
