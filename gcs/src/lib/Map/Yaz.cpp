
#include "Yaz.h"
#include "Refglobal.h"
//#include <math.h>
//======================================================

//======================================================
// class of the description of the line
//!constructor by default
MLinYaz::MLinYaz()
//======================================================
{
    //by default - the black line of zero thickness
	m_linia=new MPrArray();
    //the line
	MProsloika* prosloika = new MLinia();
	m_linia->AddTail(prosloika);
	delete prosloika;
}

//======================================================
//!constructor with the set parameters
MLinYaz::MLinYaz(MPrArray* p_lin)
//======================================================
{
    //if there is a standard
	if(p_lin)
		m_linia=(MPrArray*)p_lin->Clone();
    //otherwise by default
	else 
		m_linia=new MPrArray();
}

//======================================================
//!copying function
void MLinYaz::Copy(MLinYaz* p_linobj)
//======================================================
{
    //if it is, from where to copy
	if(p_linobj)
	{
		delete m_linia;
		m_linia=(MPrArray*)(p_linobj->m_linia->Clone());
	}
}


//======================================================
//!Receives the maximum distance between
//! extreme points of drawing
double MLinYaz::GetMaxDimension()
//======================================================
{
	double dMax = 0;
	for(int i=m_linia->GetSize()-1;i>=0;i--)
		dMax = Max(dMax,m_linia->GetAt(i)->GetMaxDimension());
	return dMax;
}


//======================================================
//!destructor
MLinYaz::~MLinYaz()
//======================================================
{
	if (m_linia) delete m_linia;
}

//============================================================
//!reading
bool MLinYaz::ReadBin(FILE* h_nuxfile, int version)
//============================================================
{
	m_linia->ReadBin(h_nuxfile,version);
	return true;
}

//============================================================
//!reading
bool MLinYaz::ReadBin(MFile* file, int version)
//============================================================
{
	m_linia->ReadBin(file,version);
	return true;
}

//============================================================
//!writing
bool MLinYaz::WriteBin(FILE* h_nuxfile, int version)
//============================================================
{
	m_linia->WriteBin(h_nuxfile,version);
	return true;
}

//============================================
//!calculation of the size of the description of object in bytes
int MLinYaz::GetBytes()
//============================================
{
	int Size = m_linia->GetBytes();

	return Size;
}

//======================================================
//!class MPloYaz
//======================================================

//======================================================
//!constructor by default
MPloYaz::MPloYaz()
//======================================================
{
    //line
	m_linia=new MPrArray();
	MProsloika* prosloika = new MLinia();
	m_linia->AddTail(prosloika);
	delete prosloika;
    //filling
	m_plast=new MPlArray();
	MPlast* plast = new MZalivka(1);
	m_plast->AddTail(plast);
	delete plast;
}

//======================================================
//!constructor with the set parameters
MPloYaz::MPloYaz(MPrArray* p_linia, MPlArray* p_plast)
//======================================================
{
    //we copy a contour if there is its standard
	if(p_linia)
		m_linia=(MPrArray*)p_linia->Clone();
	else
		m_linia=new MPrArray();
    //we copy filling if there is its standard
	if(p_plast)
		m_plast=(MPlArray*)p_plast->Clone();
	else
		m_plast=new MPlArray();
}

//======================================================
//!copying function
void MPloYaz::Copy(MPloYaz* ploshobj)
//======================================================
{
    //if there is nothing to copy, we don't copy
	if(!ploshobj) return;
	delete m_linia;
	delete m_plast;
	m_linia=(MPrArray*)(ploshobj->m_linia->Clone());
	m_plast=(MPlArray*)(ploshobj->m_plast->Clone());
}

//============================================
//!Receives maksimaljny distance between
//! extreme points of drawing
double MPloYaz::GetMaxDimension()
//============================================
{
	double dMax = 0;
	int i = 0;
	for(i = m_linia->GetSize()-1;i>=0;i--)
		dMax = Max(dMax,m_linia->GetAt(i)->GetMaxDimension());
	for(i = m_plast->GetSize()-1;i>=0;i--)
		dMax = Max(dMax,m_plast->GetAt(i)->GetMaxDimension());
	return dMax;
}

//======================================================
//!destructor
MPloYaz::~MPloYaz()
//======================================================
{
	delete m_linia;
	delete m_plast;
}

//============================================================
//!reading
bool MPloYaz::ReadBin(FILE* h_nuxfile, int version)
//============================================================
{
	m_linia->ReadBin(h_nuxfile,version);
	m_plast->ReadBin(h_nuxfile,version);
	return true;
}

//============================================================
//!reading
bool MPloYaz::ReadBin(MFile* file, int version)
//============================================================
{
	m_linia->ReadBin(file,version);
	m_plast->ReadBin(file,version);
	return true;
}

//============================================================
//!writing
bool MPloYaz::WriteBin(FILE* h_nuxfile, int version)
//============================================================
{
	m_linia->WriteBin(h_nuxfile,version);
	m_plast->WriteBin(h_nuxfile,version);
	return true;
}

//============================================
//!calculation of the size of the description of object in bytes
int MPloYaz::GetBytes()
//============================================
{
	int Size = m_linia->GetBytes();
	Size += m_plast->GetBytes();

	return Size;
}


//======================================================
//class of the description of dot object
//======================================================

//======================================================
//!constructor by default
MDotYaz::MDotYaz()
//======================================================
{
	m_tochka=new MSzArray();
	MSymbol sym;
	MSrez* srez = new MShriftSrez(&sym);
	m_tochka->AddTail(srez);
	delete srez;

}

//======================================================
//!constructor with the set parameters
MDotYaz::MDotYaz(MSzArray* p_tochka)
//======================================================
{
    //if there is a standard
	if(p_tochka)
		m_tochka=(MSzArray*)p_tochka->Clone();
    //otherwise by default
	else 
		m_tochka=new MSzArray();
}

//======================================================
//!copying function
void MDotYaz::Copy(MDotYaz* p_tochobj)
//======================================================
{
    //if it is, from where to copy
	if(p_tochobj)
	{
		delete m_tochka;
		m_tochka=(MSzArray*)(p_tochobj->m_tochka->Clone());
	}
}

//======================================================
//!Receives the maximum distance between
//! extreme points of drawing
double MDotYaz::GetMaxDimension()
//======================================================
{
	double dMax=0;
	for(int i=m_tochka->GetSize()-1;i>=0;i--)
		dMax = Max(m_tochka->GetAt(i)->GetMaxDimension(),dMax);
	return dMax;
}


//======================================================
//!destructor
MDotYaz::~MDotYaz()
//======================================================
{
	if (m_tochka) delete m_tochka;
}

//============================================================
//!reading
bool MDotYaz::ReadBin(FILE* h_nuxfile, int version)
//============================================================
{
	m_tochka->ReadBin(h_nuxfile,version);
	return true;
}

//============================================================
//!reading
bool MDotYaz::ReadBin(MFile* file, int version)
//============================================================
{
	m_tochka->ReadBin(file,version);
	return true;
}

//============================================================
//!writing
bool MDotYaz::WriteBin(FILE* h_nuxfile, int version)
//============================================================
{
	m_tochka->WriteBin(h_nuxfile,version);
	return true;
}

//============================================
//!calculation of the size of the description of object in bytes
int MDotYaz::GetBytes()
//============================================
{
	int Size = m_tochka->GetBytes();
	return Size;
}


//======================================================
//class MYaz
//======================================================

//======================================================
//!constructor by default
MYaz::MYaz()
//======================================================
{
	m_type=F_LIN;
	m_formula=new MFormArray();
	m_object=(void*) new MLinYaz();

	m_visible = true;
	m_fMasstKoeff = 1.0f;
}

//======================================================
//!constructor with the set parameters
MYaz::MYaz(int ntype, MFormArray* pformula, void* pobject)
//======================================================
{
	m_type=ntype;

    //we copy a formula if there is its standard
	if(pformula)
	{
		m_formula=(MFormArray*)pformula->Clone();
	}
	else
	{
		m_formula=new MFormArray();
	}

    //we copy the description of object if there is its standard
	if(pobject)
	{
        //depending on type of object we copy by rules of its class
		switch(m_type)
		{
		case F_PLO:
			m_object=(void*)new MPloYaz();
			((MPloYaz*)m_object)->Copy((MPloYaz*)pobject);
			break;
		case F_LIN:
		case IDL_PLS:
			m_object=(void*)new MLinYaz();
			((MLinYaz*)m_object)->Copy((MLinYaz*)pobject);
			break;
		case F_DOT:
			m_object=(void*)new MDotYaz();
			((MDotYaz*)m_object)->Copy((MDotYaz*)pobject);
			break;
		default:
			m_type=F_DOT;
			m_object=(void*)new MDotYaz();
			break;
		}
	}
    //otherwise we create object by default depending on type
	else
	{
		switch(m_type)
		{
		case F_PLO:
			m_object=(void*)new MPloYaz();
			break;
		case F_LIN:
		case IDL_PLS:
			m_object=(void*)new MLinYaz();
			break;
		case F_DOT:
			m_object=(void*)new MDotYaz();
			break;
		default:
			m_type=F_DOT;
			m_object=(void*)new MDotYaz();
			break;
		}
	}
	m_visible = true;
	m_fMasstKoeff = 1.0f;
}

//======================================================
//!copying function
void MYaz::Copy(MYaz* pobject)
//======================================================
{
    //we delete the existing description of object by rules of its type
	switch(m_type)
	{
	case F_PLO:
		delete (MPloYaz*)m_object;
		break;
	case F_LIN:
	case IDL_PLS:
		delete (MLinYaz*)m_object;
		break;
	case F_DOT:
		delete (MDotYaz*)m_object;
		break;
	}
	
    //we copy type
	m_type=pobject->m_type;
    //we copy a formula
	delete m_formula;
	if(pobject->m_formula != NULL)
		m_formula=(MFormArray*)pobject->m_formula->Clone();
	else
		m_formula=new MFormArray();
    //we copy the description of object by rules of its class
	switch(m_type)
	{
	case F_PLO:
		m_object=(void*)new MPloYaz();
		((MPloYaz*)m_object)->Copy((MPloYaz*)pobject->m_object);
		break;
	case F_LIN:
	case IDL_PLS:
		m_object=(void*)new MLinYaz();
		((MLinYaz*)m_object)->Copy((MLinYaz*)pobject->m_object);
		break;
	case F_DOT:
		m_object=(void*)new MDotYaz();
		((MDotYaz*)m_object)->Copy((MDotYaz*)pobject->m_object);
		break;
	}
	
	m_visible = pobject->m_visible;
	m_fMasstKoeff = pobject->m_fMasstKoeff;
}

//======================================================
//!function of cloning
MYaz* MYaz::Clone()
//======================================================
{
	MYaz* pClone=new MYaz();
	pClone->Copy(this);
	return pClone;
}

//============================================================
//!writing
bool MYaz::WriteBin(FILE* h_nuxfile, int version)
//============================================================
{
    //we count the size in an UTP format
	int Size=GetBytes();
	fwrite(&Size, 4, 1, h_nuxfile);
    //we keep object type
	fwrite(&m_type, 2, 1, h_nuxfile);
    //we keep formulas
	m_formula->WriteBin(h_nuxfile,version);

    //we keep the description of object depending on its type
	switch(m_type)
	{
	case F_PLO:
		((MPloYaz*)m_object)->WriteBin(h_nuxfile,version);
		break;
	case F_LIN:
	case IDL_PLS:
		((MLinYaz*)m_object)->WriteBin(h_nuxfile,version);
		break;
	case F_DOT:
		((MDotYaz*)m_object)->WriteBin(h_nuxfile,version);
		break;
	}
	return true;
}

//============================================================
//!reading
bool MYaz::ReadBin(FILE* h_nuxfile, int version)
//============================================================
{
	
    //We delete the old description
	switch(m_type)
	{
	case F_PLO:
		delete (MPloYaz*)m_object;
		break;
	case F_LIN:
	case IDL_PLS:
		delete (MLinYaz*)m_object;
		break;
	case F_DOT:
		delete (MDotYaz*)m_object;
		break;
	}

    //we pass size writing
	fseek(h_nuxfile, 4, SEEK_CUR);
    //we read object type
//        m_type= 0;
	fread(&m_type, 2, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_type, sizeof(m_type) );
    //we read formulas
	m_formula->ReadBin(h_nuxfile,version);
	
    //we read the description of object depending on its type
	switch(m_type)
	{
	case F_PLO:
		m_object=(void*) new MPloYaz();
		((MPloYaz*)m_object)->ReadBin(h_nuxfile,version);
		break;
	case F_LIN:
	case IDL_PLS:
		m_object=(void*) new MLinYaz();
		((MLinYaz*)m_object)->ReadBin(h_nuxfile,version);
		break;
	case F_DOT:
		m_object=(void*) new MDotYaz();
		((MDotYaz*)m_object)->ReadBin(h_nuxfile,version);
		break;
	}
	return true;
}

//============================================================
//!reading
bool MYaz::ReadBin(MFile* file, int version)
//============================================================
{
	
    //We delete the old description
	switch(m_type)
	{
	case F_PLO:
		delete (MPloYaz*)m_object;
		break;
	case F_LIN:
	case IDL_PLS:
		delete (MLinYaz*)m_object;
		break;
	case F_DOT:
		delete (MDotYaz*)m_object;
		break;
	}

    //we pass size writing
	file->SetPosition(4, MAP_SEEK_CUR);
    //we read object type
//	m_type = 0;
	file->GetData(&m_type, 2, 1);
		if (reverseReadFlag) reverseValue((char*)&m_type, sizeof(m_type) );
    //we read formulas
	m_formula->ReadBin(file,version);
	
    //we read the description of object depending on its type
	switch(m_type)
	{
	case F_PLO:
		m_object=(void*) new MPloYaz();
		((MPloYaz*)m_object)->ReadBin(file,version);
		break;
	case F_LIN:
	case IDL_PLS:
		m_object=(void*) new MLinYaz();
		((MLinYaz*)m_object)->ReadBin(file,version);
		break;
	case F_DOT:
		m_object=(void*) new MDotYaz();
		((MDotYaz*)m_object)->ReadBin(file,version);
		break;
	}
	return true;
}

//======================================================
//!destructor
MYaz::~MYaz()
//======================================================
{
	delete m_formula;
    //we delete the description of object depending on its class
	switch(m_type)
	{
	case F_PLO:
		delete (MPloYaz*)m_object;
		break;
	case F_LIN:
		delete (MLinYaz*)m_object;    //  2004.11.19  vipa
		break;                        //  2004.11.19  vipa
	case IDL_PLS:
		delete (MLinYaz*)m_object;
		break;
	case F_DOT:
		delete (MDotYaz*)m_object;
		break;
	}
}


//============================================
//!calculation of the size of the description of object in bytes
int MYaz::GetBytes()
//============================================
{
    int Size=6;//4-size of record + 2 type of object
    Size+=m_formula->GetBytes();//formulas size
    //+ the size of the description of object depending on its type
	switch(m_type)
	{
	case F_PLO:
		Size+=((MPloYaz*)m_object)->GetBytes();
		break;
	case F_LIN:
	case IDL_PLS:
		Size+=((MLinYaz*)m_object)->GetBytes();
		break;
	case F_DOT:
		Size+=((MDotYaz*)m_object)->GetBytes();
		break;
	}

	return Size;
}

//========================================================================
//!Function makes converting of the current object to the type specified
//! as parameter
void MYaz::ConvertObjectTo(int m_iNewType)
//========================================================================
{
	if(m_type == m_iNewType) return;
	switch(m_type){
	case IDL_PLS:
	case F_LIN:
		{
            //Converting from linear object
			switch(m_iNewType){
			case IDL_PLS:
			case F_LIN:
				m_type = m_iNewType;break;
			case F_PLO:
				{	
					MPloYaz* m_pPloshYazNew = new MPloYaz(((MLinYaz*)m_object)->m_linia,NULL);
					delete (MLinYaz*)m_object;
					m_object = (void *)m_pPloshYazNew;
					m_type = m_iNewType;
				} break;
			case F_DOT:
				{
					MSrez * m_pNewSrez = new MShriftSrez();
					MSzArray* m_pNewSzArray = new MSzArray();
					m_pNewSzArray->AddTail(m_pNewSrez);
					MDotYaz* m_pTochYazNew = new MDotYaz(m_pNewSzArray);
					delete m_pNewSrez;
					delete m_pNewSzArray;
					delete (MLinYaz*)m_object;
					m_object = (void*)m_pTochYazNew;
					m_type = m_iNewType;
				} break;
		default: break;
			}
		} break;
	case F_PLO:
		{
			switch(m_iNewType){
			case IDL_PLS:
			case F_LIN:
				{
					MLinYaz* m_pLinYazNew = new MLinYaz(((MPloYaz*)m_object)->m_linia);
					delete (MPloYaz *)m_object;
					m_object = (void*)m_pLinYazNew;
					m_type = m_iNewType;
				} break;
			case F_DOT:
				{
					MSrez * m_pNewSrez = new MShriftSrez();
					MSzArray* m_pNewSzArray = new MSzArray();
					m_pNewSzArray->AddTail(m_pNewSrez);
					MDotYaz* m_pTochYazNew = new MDotYaz(m_pNewSzArray);
					delete m_pNewSrez;
					delete m_pNewSzArray;
					delete (MPloYaz*)m_object;
					m_object = (void*)m_pTochYazNew;
					m_type = m_iNewType;
				} break;
			default: break;
			}
		} break;
	case F_DOT:
		{
			switch(m_iNewType){
			case IDL_PLS:
			case F_LIN:
				{
					MProsloika * m_pNewProsloika = (MProsloika*)new MLinia();
					MPrArray * m_pNewPrArray = new MPrArray();
					m_pNewPrArray->AddTail(m_pNewProsloika);
				
					MLinYaz* m_pNewLinYaz = new MLinYaz(m_pNewPrArray);

					delete (MDotYaz*)m_object;
					m_object = (void *)m_pNewLinYaz;
					m_type = m_iNewType;
					delete m_pNewProsloika;
					delete m_pNewPrArray;

				} break;
			case F_PLO:
				{
					MPlast * m_pNewPlast = (MPlast*)new MZalivka();//new MPlast(PLT_ZALIVKA);
					MPlArray * m_pNewPlArray = new MPlArray();
					m_pNewPlArray->AddTail(m_pNewPlast);

					MProsloika * m_pNewProsloika = (MProsloika*)new MLinia();
					MPrArray * m_pNewPrArray = new MPrArray();
					m_pNewPrArray->AddTail(m_pNewProsloika);

					MPloYaz* m_pPloshYazNew = new MPloYaz(m_pNewPrArray,m_pNewPlArray);
					delete (MDotYaz*)m_object;
					m_object = (void *)m_pPloshYazNew;
					m_type = m_iNewType;
					delete m_pNewPlast;
					delete m_pNewPlArray;
					delete m_pNewProsloika;
					delete m_pNewPrArray;
				} break;
			default: break;
			}
		} break;
	default: break;
	}
}


//============================================
//!Receives the maximum distance between
//! extreme points of drawing
double MYaz::GetMaxDimension()
//============================================
{
	switch(m_type){
	case F_PLO:
			return ((MPloYaz*)m_object)->GetMaxDimension();
		break;
	case F_LIN:
	case IDL_PLS:
			return ((MLinYaz*)m_object)->GetMaxDimension();
		break;
	case F_DOT:
			return ((MDotYaz*)m_object)->GetMaxDimension();
		break;
//	default:	exit(15);
	}
	return 0.0;
}
