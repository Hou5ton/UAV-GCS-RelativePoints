#include "ZnakSistema.h"
#include "Karta.h"
#include "Globals.h"
#include "Refglobal.h"
/////////////////////////////////////////////////////////////
//*********************************************************//
//*********************************************************//
//**                                                     **//
//**                  Classifier classes                 **//
//**                                                     **//
//*********************************************************//
//*********************************************************//
/////////////////////////////////////////////////////////////

//for display of progress when reading the classifier
extern unsigned int	c_dlina_faila_kls;
extern void (*pSetPercent)();

extern MZnak* (*fpCreateZnakByType)(MString str);
//===========================================================
//!constructor of the term by default initializes all fields
//! the empty
MTermin::MTermin()
//===========================================================
{
	
	m_kod="KOD";
    m_shortname = KLS_NAZV; //"Name"
    m_fullname = KLS_FULL_NAME; //"Full Name"
    m_remarka = KLS_COMMENT; //"Comment"
}

//===========================================================
//!constructor of the term with the set fields
MTermin::MTermin(MString kod, MString shortname, MString fullname, MString remarka)
//===========================================================
{
	kod.TrimLeft();
	kod.TrimRight();
    //In the term the code can't be empty
	if(kod!="")
	{
        //During check it is required that the code of the term didn't contain
        //spaces
		while(kod.Find(" ",0)!=-1){
			int iIndex = kod.Find(" ",0);
			kod.Delete(iIndex);
		};

		m_kod=kod;
		m_shortname=shortname;
		m_fullname=fullname;
		m_remarka=remarka;
	}
	else
	{
		m_kod="NEW";
		m_shortname="Новый";
        m_fullname="Новый обьект";
        m_remarka="Новый обьект с неопределенными параметрами";
	}
}

//================================================
//!comparing
bool MTermin::operator ==( const MTermin& toComp ) const
//================================================
{
	if((m_fullname==toComp.m_fullname)&&
		(m_kod == toComp.m_kod)&&
		(m_remarka == toComp.m_remarka)&&
		(m_shortname == toComp.m_shortname))
		return true;
	return false;
}


//===========================================================
//!function of determination of the size in UTP
int MTermin::GetBytes()
//===========================================================
{
    //term size =
    //      quantity of characters in m_kod + 2
    //      quantity of characters in m_shortname + 2
    //      quantity of characters in m_fullname + 2
    //      quantity of characters in m_remarka + 2
	return m_kod.GetLength()+m_shortname.GetLength()+
		   m_fullname.GetLength()+m_remarka.GetLength()+8;
}

//==========================================================
//!function copies in itself the  fields of 'pTermin'
void MTermin::Copy(MTermin* pTermin)
//===========================================================
{
    //if the term exists, is copied its fields in our term
	if(pTermin)
	{
        //During check it is required that the code of the term didn't contain
        //spaces
		while(pTermin->m_kod.Find(" ",0)!=-1){
			int iIndex = pTermin->m_kod.Find(" ",0);
			pTermin->m_kod.Delete(iIndex);
		};
		m_kod=pTermin->m_kod;
		m_shortname=pTermin->m_shortname;
		m_fullname=pTermin->m_fullname;
		m_remarka=pTermin->m_remarka;
	}
}


//===========================================================
//!function creates the exact copy of the term
MTermin* MTermin::Clone()
//===========================================================
{
    //we create the copy
	MTermin* pTerm=new MTermin();
    //we copy in it our term
	pTerm->Copy(this);
    //return
	return pTerm;
}

//===========================================================
//!writing in a NUX format
bool MTermin::WriteBin(FILE* h_nuxfile,  int version)
//===========================================================
{
	if(!WriteString(h_nuxfile,  version, m_kod))return false;
	if(!WriteString(h_nuxfile,  version, m_shortname))return false;
	if(!WriteString(h_nuxfile,  version, m_fullname))return false;
	if(!WriteString(h_nuxfile,  version, m_remarka))return false;
	return true;
}

//===========================================================
//!reading from a NUX format
bool MTermin::ReadBin(FILE* h_nuxfile,  int version)
//===========================================================
{
	m_kod=ReadString(h_nuxfile,version);
	m_shortname=ReadString(h_nuxfile,version);
	m_fullname=ReadString(h_nuxfile,version);
	m_remarka=ReadString(h_nuxfile,version);
	return true;
}

//===========================================================
//!reading from a NUX format
bool MTermin::ReadBin(MFile* file,  int version)
//===========================================================
{
	m_kod=ReadString(file,version);
	m_shortname=ReadString(file,version);
	m_fullname=ReadString(file,version);
	m_remarka=ReadString(file,version);
	return true;
}

//===========================================================
//!returns 'true', if to 'pTermin' same, as well as he
bool MTermin::IsEqualTo(MTermin* pTermin)
//===========================================================
{
    // Terms are equal if are equal a code, a short name and a polgy name.
    //The comment isn't taken into account
	if(pTermin)
		return ((m_kod==pTermin->m_kod)&&
			    (m_shortname==pTermin->m_shortname)&&
				(m_fullname==pTermin->m_fullname));
	else return false;
}



//===========================================================
//!constructor by default creates top object by default
MZnak::MZnak()
//===========================================================
{
	m_class = ID_TOPOBJECT;
	m_noms = new MShInArray();
	m_genattrs = new MNvArray();
	m_YazArray = new MYzArray();
	MYaz* pYaz = new MYaz(F_LIN);
	m_YazArray->AddTail(pYaz);
	delete pYaz;
}

//===========================================================
//!constructor with the given parameters
MZnak::MZnak(MTermin termin, MYzArray* YazArray, MShInArray* pNomAttrNames, MNvArray* pGroupAttr)
//===========================================================
{
	m_termin.Copy(&termin);
	m_class = ID_TOPOBJECT;

    //Initialization of data
	if((pNomAttrNames)&&(pNomAttrNames->GetSize()>=1))
		m_noms = (MShInArray*)pNomAttrNames->Clone();
	else{
		m_noms = new MShInArray();
	}
	if((pGroupAttr)&&(pGroupAttr->GetSize()>=1))
		m_genattrs = (MNvArray*)pGroupAttr->Clone();
	else{
		m_genattrs = new MNvArray();
	}

	m_YazArray = new MYzArray();
    //if the object-pointer on an array of languages isn't empty, we copy it
	if(YazArray)
		m_YazArray->AppendTail(YazArray);
    //otherwise we create an array of languages by default
	else{
		MYaz* pYaz = new MYaz(F_LIN);
		m_YazArray->AddTail(pYaz);
		delete pYaz;
	}
}


//===========================================================
//!constructor with the given standard
MZnak::MZnak(MZnak* znak)
//===========================================================
{
    //if the standard is
	if(znak)
	{
		m_termin.Copy(&znak->m_termin);
		m_class = znak->m_class;
		m_noms = (MShInArray*)znak->m_noms->Clone();
		m_genattrs = (MNvArray*)znak->m_genattrs->Clone();
		m_YazArray = (MYzArray*)znak->m_YazArray->Clone();
	}
	else
	{
		m_class = ID_TOPOBJECT;
		m_noms = new MShInArray();
		m_genattrs = new MNvArray();
		m_YazArray = new MYzArray();
		MYaz* pYaz = new MYaz(F_LIN);
		m_YazArray->AddTail(pYaz);
		delete pYaz;
	}
}

//===========================================================
//!destructor
MZnak::~MZnak()
//===========================================================
{
	delete m_YazArray;
	delete m_noms;
	delete m_genattrs;
}


//===========================================================
//!Returns the record length NUX
unsigned int MZnak::GetBytes()
//===========================================================
{
    //Length =
    //      identifier (3 bytes) +
    //      amount of record (4 bytes) +
    //      term size (variable) +
    //      size of a line of the name of a class (variable) + 2 bytes
    //      size of an array of characteristics (variable)
    //      size of an array of values of patrimonial characteristics (variable)
    //      size of an array of languages (variable)
	unsigned int Size=3+4+m_termin.GetBytes()+m_class.GetLength()+2+m_noms->GetBytes()+
		m_genattrs->GetBytes()+m_YazArray->GetBytes();
	return Size;
}

//===========================================================
//!function copies in itself 'znak' contents
void MZnak::Copy(MZnak* znak)
//===========================================================
{
	if(znak)
	{
		m_termin.Copy(&znak->m_termin);
		m_class = znak->m_class;

        //we copy an array of admissible codes
		delete m_noms;
		if(znak->m_noms != NULL)
			m_noms=(MShInArray*)znak->m_noms->Clone();
		else
			m_noms=new MShInArray();
        //we copy an array of group characteristics
		delete m_genattrs;
		if(znak->m_genattrs != NULL)
			m_genattrs=(MNvArray*)znak->m_genattrs->Clone();
		else
			m_genattrs=new MNvArray();

		m_YazArray->ClearAll();
		delete m_YazArray;
		m_YazArray = (MYzArray*)znak->m_YazArray->Clone();
	}
}

//===========================================================
//!checks, whether the term at 'znak' matches
bool MZnak::IsTerminEqualTo(MZnak* znak)
//===========================================================
{
	if(znak)
		return m_termin.IsEqualTo(&znak->m_termin);
	else return false;
}

//===========================================================
//!we create the copy
MZnak* MZnak::Clone()
//===========================================================
{
    //we create a new sign
	MZnak* pZnak=new MZnak();
    //we copy in it ourselves
	pZnak->Copy(this);
    //return
	return pZnak;
}


//===========================================================
//!writing in a NUX format
bool MZnak::WriteBin(FILE* h_nuxfile,  int version)
//===========================================================
{
    //Calculation of the extent of object in UTP
	unsigned int SBytes = GetBytes();

    //Writing
	switch( version ) {
        //version 0
		case 0:
            //writing of the identifier of a sign
			fwrite(ID_ZNAK, 3, 1, h_nuxfile);
            //writing of length of object
			fwrite(&SBytes, 4, 1, h_nuxfile);
            //terminology writing
			m_termin.WriteBin(h_nuxfile,version);
            // writing of the identifier of a class
			WriteString(h_nuxfile,  version, m_class);
            //we save data on indexes (codes) of names of admissible characteristics in the dictionary
			m_noms->WriteBin(h_nuxfile,version);
            //we save data on group characteristics
			m_genattrs->WriteBin(h_nuxfile,version);
            //writing of an array of languages
			m_YazArray->WriteBin(h_nuxfile,version);
	}
	return true;
}

//===========================================================
//!reading from a NUX format
bool MZnak::ReadBin(FILE* h_nuxfile,  int version)
//===========================================================
{
	switch( version ) {
        //version 0
		case 0:

            //we pass length of record and the identifier
			fseek(h_nuxfile, 7, SEEK_CUR);
            //reading term
			m_termin.ReadBin(h_nuxfile,version);
            //reading identifier of a class
			m_class = ReadString(h_nuxfile,  version);
            //we read indexes (codes) of names of admissible characteristics
			m_noms->ReadBin(h_nuxfile,version);
            //we read group characteristics
			m_genattrs->ReadBin(h_nuxfile,version);
            //reading array of languages
			m_YazArray->ReadBin(h_nuxfile,version);
	}
	return true;
}

//===========================================================
//!reading from a NUX format
bool MZnak::ReadBin(MFile* file,  int version)
//===========================================================
{
	switch( version ) {
        //version 0
		case 0:

            //we pass length of record and the identifier
			file->SetPosition(7, MAP_SEEK_CUR);
            //reading term
			m_termin.ReadBin(file,version);
            //reading identifier of a class
			m_class = ReadString(file,  version);
            //we read indexes (codes) of names of admissible characteristics
			m_noms->ReadBin(file,version);
            //we read group characteristics
			m_genattrs->ReadBin(file,version);
            //reading array of languages
			m_YazArray->ReadBin(file,version);
	}
	return true;
}

//===========================================================
//!Returns the term
MTermin* MZnak::CreateCopyTermin()
//===========================================================
{
	return m_termin.Clone();
}



//===========================================================
//!Returns the term
MTermin* MZnak::GetTermin()
//===========================================================
{
	return &m_termin;
}

//===========================================================
//!sets the term
void MZnak::SetTermin(MTermin termin)
//===========================================================
{
	m_termin.Copy(&termin);
}

MShInArray *MZnak::GetNomsArray()const
{
    return m_noms;
}

MNvArray *MZnak::GetGenattrArray() const
{
    return m_genattrs;
}

MYzArray *MZnak::GetYazArray() const
{
    return m_YazArray;
}

//===========================================================
//!sets an array of characteristics
void MZnak::SetNomsArray(MShInArray* noms)
//===========================================================
{
	if(noms)
	{
		delete m_noms;
		m_noms = (MShInArray*)noms->Clone();
	}
}

//===========================================================
//!sets an array of patrimonial values of characteristics
void MZnak::SetGenattrArray(MNvArray* genattrs)
//===========================================================
{
	if(genattrs)
	{
		delete m_genattrs;
		m_genattrs = (MNvArray*)genattrs->Clone();
	}
}

//===========================================================
//!sets language with number 'nom' in an array of languages
bool MZnak::SetYaz(MYaz* pYaz, int nom)
//===========================================================
{
	if(!pYaz)
		return false;
	if(!m_YazArray)
		return false;
	if((nom<0)||(nom>=m_YazArray->GetSize()))
		return false;

	m_YazArray->SetAt(nom,pYaz);
	return true;
}

//===========================================================
//!sets an array of languages
void MZnak::SetYazArray(MYzArray* YazArray)
//===========================================================
{
	if(YazArray)
	{
		delete m_YazArray;
		m_YazArray = (MYzArray*)YazArray->Clone();
	}
}

//==============================================================================
//Input parameter - color index;
//! function returns 'true' if in a sign the color appropriate is used
//! to an input index, and 'false' otherwise
bool MZnak::IsColorkod(unsigned char colorkod)	
//==============================================================================
{
	MYaz* pYaz = GetYaz();
	int type = pYaz->GetType();
	switch (type)
	{
    //areal objects
	case F_PLO:
		{
			MPloYaz* ployaz = (MPloYaz*)pYaz->m_object;
            //object boundary
			int i = 0;
			for(i=0; i<ployaz->m_linia->GetSize(); i++){
				MProsloika* prosl = ployaz->m_linia->GetAt(i);
				int typeprosl = prosl->GetType();
				switch(typeprosl)
				{
                //simple line
				case 0:
					{
						MLinia* linia = (MLinia*)prosl;
						if(linia->m_colorkod == colorkod)
							return true; 
					}
					break;
                //shaped line
				case 1:
					{
						MShtrih* shtrih = (MShtrih*)prosl;
						if(shtrih->m_colorkod == colorkod)
							return true;
					}
					break;
                // dotted line
				case 2:
					{
						MPunktir* punktir = (MPunktir*)prosl;
						if(punktir->m_colorkod == colorkod)
							return true; 
					}
					break;
				}
			}
            //fillings of areal objects
			for(i=0; i<ployaz->m_plast->GetSize(); i++){
				MPlast* plast = ployaz->m_plast->GetAt(i);
				int typeplast = plast->GetType();
				switch(typeplast)
				{
                //shading
				case 1:
					{
						MShtrihovka* shtrihovka = (MShtrihovka*)plast;
						if(shtrihovka->m_colorkod == colorkod)
						return true;
					}
					break;
                //filling
				case 2:
					{
						MZalivka* zalivka = (MZalivka*)plast;
						if(zalivka->m_colorkod == colorkod)
						return true;
					}
					break;
                //pattern
				case 11:
					{
						MPattern* pattern = (MPattern*)plast;
						if(pattern->m_colorkod == colorkod)
						return true;
					}
					break;
                }//end switch(typeplast)
            }//end for(int i=0; i<ployaz->m_plast->GetSize(); i++)
		}
		break;
    //linear objects
	case F_LIN:
	case IDL_PLS:
		{
			MLinYaz* linyaz = (MLinYaz*)pYaz->m_object;
			for(int i=0; i<linyaz->m_linia->GetSize(); i++){
				MProsloika* prosl = linyaz->m_linia->GetAt(i);
				int typeprosl = prosl->GetType();
				switch(typeprosl)
				{
                //simple line
				case 0:
					{
						MLinia* linia = (MLinia*)prosl;
						if(linia->m_colorkod == colorkod)
						return true;
					}
					break;
                //shaped line
				case 1:
					{
						MShtrih* shtrih = (MShtrih*)prosl;
						if(shtrih->m_colorkod == colorkod)
						return true;
					}
					break;
                    // dotted line
                case 2:
					{
						MPunktir* punktir = (MPunktir*)prosl;
						if(punktir->m_colorkod == colorkod)
						return true;
					}
					break;
                }//end switch(typeprosl)
            }//end for(int i=0; i<linyaz->m_linia->GetSize(); i++)
		}
		break;
    }//end switch (type)
	return false;
}


//==================================
//Input parameter - a handwriting code;
//! function returns 'true' if in a sign the handwriting appropriate is used
//! to an input code, and 'false' - otherwise
bool MZnak::IsPrkkod(unsigned char pocherkkod)	
//==================================
{
	MYaz* pYaz = GetYaz();
	MFormArray* formuli = pYaz->m_formula;
	unsigned char prkkod;
	for(int l=0; l<formuli->GetSize(); l++){
		MFrArray* frar = formuli->GetAt(l)->fragm_arr;
		for(int s=0; s<frar->GetSize(); s++){
			MFragment* fragment = frar->GetAt(s);
			unsigned char type = fragment->GetType();
			if(type==ID_FRAG_FRACTION){
				MFraction* fraction = (MFraction*)fragment->GetFragment();
				MStrofa* strofa = fraction->GetChisl();
				int t = 0;
				for(t=0; t<strofa->GetPredlogenieNum(); t++){
					prkkod = strofa->GetNomPrk(t);
					if(prkkod == pocherkkod)
						return true;
				}
				strofa = fraction->GetZnam();
				for(t=0; t<strofa->GetPredlogenieNum(); t++){
					prkkod = strofa->GetNomPrk(t);
					if(prkkod == pocherkkod)
						return true;
				}
			}
			if(type==ID_FRAG_STROFA){
				MStrofa* strofa = (MStrofa*)fragment->GetFragment();
				for(int t=0; t<strofa->GetPredlogenieNum(); t++){
					prkkod = strofa->GetNomPrk(t);
					if(prkkod == pocherkkod)
						return true;
				}
			}
        }//end for(int s=0; s<frar->GetSize(); s++)
    }//end for(l=0; l<formuli->GetSize(); l++)

	return false;
}

//==================================
//Input parameter - a code of a boundary line;
//! function returns 'true' if in a sign the boundary line is used,
//! appropriate to an input code, and 'false' - otherwise
bool MZnak::IsMegstrkod(unsigned char megstrkod)	
//==================================
{
	MYaz* pYaz = GetYaz();
    //the boundary line changes in stanzas of formulas
	MFormArray* formuli = pYaz->m_formula;
	for(int l=0; l<formuli->GetSize(); l++){
		MFrArray* frar = formuli->GetAt(l)->fragm_arr;
		for(int s=0; s<frar->GetSize(); s++){
			MFragment* fragment = frar->GetAt(s);
			unsigned char type = fragment->GetType();
			unsigned char megastrkod;
				if(type==ID_FRAG_FRACTION){
				MFraction* fraction = (MFraction*)fragment->GetFragment();
				MStrofa* strofa = fraction->GetChisl();
				int t = 0;
				for(t=0; t<strofa->GetPredlogenieNum(); t++){
					megastrkod = strofa->GetNomPrefix(t);
					if(megastrkod == megstrkod)
						return true;
					megastrkod = strofa->GetNomPostfix(t);
					if(megastrkod == megstrkod)
						return true;
				}
				strofa = fraction->GetZnam();
				for(t=0; t<strofa->GetPredlogenieNum(); t++){
					megastrkod = strofa->GetNomPrefix(t);
					if(megastrkod == megstrkod)
						return true;
					megastrkod = strofa->GetNomPostfix(t);
					if(megastrkod == megstrkod)
						return true;
				}
			}
			if(type==ID_FRAG_STROFA){
				MStrofa* strofa = (MStrofa*)fragment->GetFragment();
				for(int t=0; t<strofa->GetPredlogenieNum(); t++){
					megastrkod = strofa->GetNomPrefix(t);
					if(megastrkod == megstrkod)
						return true;
					megastrkod = strofa->GetNomPostfix(t);
					if(megastrkod == megstrkod)
						return true;
				}
			}
        }//end for(int s=0; s<frar->GetSize(); s++)
    }//end for(l=0; l<formuli->GetSize(); l++)

	return false;
}

//==================================
//Input parameter - a characteristic code;
//! function returns 'true' if in a sign the characteristic is used,
//! appropriate to an input code, and 'false' - otherwise
bool MZnak::IsCharkod(unsigned short charkod)	
//==================================
{
    //we look for a code of the characteristic in an array of admissible codes
	unsigned short nom;
	for(int l=0; l<m_noms->GetSize(); l++){
		nom = m_noms->GetAt(l)[0];
		if(nom == charkod)
			return true;
	}

	return false;
}


//===========================================================
//!constructor by default
//! creates the term by default and an empty array of signs
MRazdel::MRazdel()
//===========================================================
{
	m_fMasstKoeff=1;
	p_znaki = new MZnArray();
}

//===========================================================
//!constructor in the given parameters
MRazdel::MRazdel(MTermin termin,MZnArray* zna)
//===========================================================
{
	m_fMasstKoeff=1;
    //if 'zna' not of NULL, we create the new section
	if(zna)
	{
		m_termin.Copy(&termin);
		p_znaki=(MZnArray*)zna->Clone();
	}
    //otherwise we create the section by default
	else
	{
		p_znaki=new MZnArray();
	}
}


//===========================================================
//!constructor with the given standard
MRazdel::MRazdel(MRazdel* razdel)
//===========================================================
{
	m_fMasstKoeff=1;
    //if the standard is set, we create the section by its example
	if(razdel)
	{
		m_termin.Copy(&razdel->m_termin);
		m_fMasstKoeff = razdel->m_fMasstKoeff;
		p_znaki=(MZnArray*)razdel->p_znaki->Clone();
	}
    //otherwise we create the section by default
	else
	{
		p_znaki=new MZnArray();
	}
}

//===========================================================
//!destructor
MRazdel::~MRazdel()
//===========================================================
{
	if(p_znaki)   delete p_znaki;
}


//===========================================================
//!Copying
void MRazdel::Copy(MRazdel* razdel)
//===========================================================
{
    //we copy only when is, from where
	if(razdel)
	{
		m_termin.Copy(&razdel->m_termin);
		delete p_znaki;
		p_znaki=(MZnArray*)razdel->p_znaki->Clone();
		m_fMasstKoeff = razdel->m_fMasstKoeff;
	}
}

//===========================================================
//!Cloning
MRazdel* MRazdel::Clone()
//===========================================================
{
	MRazdel* pClone=new MRazdel(this);
	return pClone;
}

//===========================================================
//!Calculate the size
unsigned int MRazdel::GetBytes()
//===========================================================
{
    //The size of the section consists =
    //      identifier size (3 bytes) +
    //      size size (4 bytes) +
    //      term size (variable) +
    //      size of an array of signs (variable)
	unsigned int bytes=3+4+m_termin.GetBytes()+p_znaki->GetBytes();
	return bytes;
}


//===========================================================
//!Reads from the Nux-file
bool MRazdel::ReadBin(FILE* h_nuxfile,int version)
//===========================================================
{
	switch( version ) {
        //version 0
		case 0:
            //we pass length of record and the identifier
			fseek(h_nuxfile, 7, SEEK_CUR);
            //reading term
			m_termin.ReadBin(h_nuxfile,version);
            //reading array of signs
			p_znaki->ReadBin(h_nuxfile,version);
	}

//	(*pSetPercent)();
	return true;
}

//===========================================================
//!Reads from the Nux-file
bool MRazdel::ReadBin(MFile* file,int version)
//===========================================================
{
	switch( version ) {
        //version 0
		case 0:
            //we pass length of record and the identifier
			file->SetPosition(7, MAP_SEEK_CUR);
            //reading term
			m_termin.ReadBin(file,version);
            //reading array of signs
			p_znaki->ReadBin(file,version);
	}

//	(*pSetPercent)();
	return true;
}

//===================================================================
//!Coercion of all scaling ratios of signs in compliance
void MRazdel::UpdateKoeff()
//===================================================================
{
	for(int i=p_znaki->GetSize()-1;i>=0;i--)
		p_znaki->GetAt(i)->SetMasstKoeff(m_fMasstKoeff);
}

//===========================================================
//!Saves in the NUX file
bool MRazdel::WriteBin(FILE* h_nuxfile,  int version)
//===========================================================
{
    //Calculation of the extent of object in UTP
	unsigned int SBytes = GetBytes();
    //Writing
	switch( version ) {
        //version 0
		case 0:
            //Writing of the identifier of the section
			fwrite(ID_RAZDEL, 3, 1, h_nuxfile);
            //Writing of length of object
			fwrite(&SBytes, 4, 1, h_nuxfile);
            //Writing termin
			m_termin.WriteBin(h_nuxfile,version);
            //Writing of an array of signs
			p_znaki->WriteBin(h_nuxfile,version);
	}
	return true;
}

//===========================================================
//!comparing of sections according to terms
bool MRazdel::IsTerminEqualTo(MRazdel* razdel)
//===========================================================
{
    //if is to what to compare
	if(razdel)
		return m_termin.IsEqualTo(&razdel->m_termin);
    //else return false
	else
		return false;
}


//===========================================================
//!comparing the term
bool MRazdel::IsTerminEqualTo(MTermin termin)
//===========================================================
{
	return m_termin.IsEqualTo(&termin);
}

//===========================================================
//!give the term
MTermin* MRazdel::GetTermin()
//===========================================================
{
	return &m_termin;
}

//===========================================================
//!give the term copy
MTermin* MRazdel::CreateCopyTermin()
//===========================================================
{
	return m_termin.Clone();
}

//===========================================================
//!set the term
void MRazdel::SetTermin(MTermin termin)
//===========================================================
{
	m_termin.Copy(&termin);
}

//===========================================================
//!give the array of signs
MZnArray* MRazdel::GetZnakArray()
//===========================================================
{
	return p_znaki;
}

//===========================================================
//!Give the copy of an array of signs
MZnArray* MRazdel::CreateCopyZnakArray()
//===========================================================
{
	return (MZnArray*)p_znaki->Clone();
}

//===========================================================
//!Set an array of signs
void MRazdel::SetZnakArray(MZnArray* zna)
//===========================================================
{
    //we set if the array exists
	if(zna)
	{
		delete p_znaki;
		p_znaki=(MZnArray*)zna->Clone();
		UpdateKoeff();
	}
}

//===========================================================
//!Add an array of signs to the beginning
void MRazdel::AppendZnakArrayHead(MZnArray* zna)
//===========================================================
{
	if(zna)
	{
		p_znaki->AppendHead(zna);
		UpdateKoeff();
	}
}


//===========================================================
//!Add an array of signs to the end
void MRazdel::AppendZnakArrayTail(MZnArray* zna)
//===========================================================
{
	if(zna)
	{
		p_znaki->AppendTail(zna);
		UpdateKoeff();
	}
}

//===========================================================
//!Clear an array of signs
void MRazdel::ClearZnakArray()
//===========================================================
{
	p_znaki->ClearAll();
}

//===========================================================
//!Give a sign with such term or NULL if it isn't present
MZnak* MRazdel::GetZnak(MTermin termin)
//===========================================================
{
	
	int cnt=p_znaki->GetSize();
	MZnak* zn;
    //we look for a sign with such term
	for(int i=0; i<cnt;i++)
	{
		zn=p_znaki->GetAt(i);
		if(zn->GetTermin()->IsEqualTo(&termin))
			return zn;
	}
    //if the sign with such term isn't found, return NULL
	return NULL;
}

//===========================================================
//!Give a sign with such code or NULL if it isn't present
MZnak* MRazdel::GetZnak(MString kod)
//===========================================================
{
	int cnt=p_znaki->GetSize();
	MZnak* zn;
    //we look for a sign with such code
	for(int i=0; i<cnt;i++)
	{
		zn=p_znaki->GetAt(i);
		if(zn->GetTermin()->GetKod()==kod)
			return zn;
	}
    //if the sign with such code isn't found, return NULL
	return NULL;
}

//===========================================================
//!Give the copy of a sign with such term or NULL if it isn't present
MZnak* MRazdel::CreateCopyZnak(MTermin termin)
//===========================================================
{
    //We look for the necessary sign
	MZnak* zn=GetZnak(termin);
	if(zn) 
		return zn->Clone();
	else 
		return NULL;
}

//===========================================================
//!Give the copy of a sign with such code or NULL if it isn't present
MZnak* MRazdel::CreateCopyZnak(MString kod)
//===========================================================
{
    //We look for the necessary sign
	MZnak* zn=GetZnak(kod);
	if(zn) 
		return zn->Clone();
	else 
		return NULL;
}

//===========================================================
//!Delete a sign with such term
void MRazdel::DeleteZnak(MTermin termin)
//===========================================================
{
	int cnt=p_znaki->GetSize();
	MZnak* zn;
    //we look for a sign with such term
	int i,j=-1;
	for(i=0;i<cnt;i++)
	{
		zn=p_znaki->GetAt(i);
		if(zn->GetTermin()->IsEqualTo(&termin))
			j=i;
	}
    //if the sign with such term is found, we delete it
	if(j!=-1) p_znaki->Delete(j);
}

//===========================================================
//!Delete a sign with such code
void MRazdel::DeleteZnak(MString kod)
//===========================================================
{
	int cnt=p_znaki->GetSize();
	MZnak* zn;
    //we look for a sign with such code
	int i,j=-1;
	for(i=0;i<cnt;i++)
	{
		zn=p_znaki->GetAt(i);

		if(zn->GetTermin()->GetKod()==kod)
			j=i;
	}
    //if the sign with such code is found, we delete it
	if(j!=-1) p_znaki->Delete(j);
}

//===========================================================
//!Add such sign
MZnak* MRazdel::AddZnak(MZnak* znak)
//===========================================================
{
    //we add only not a zero sign
	if(znak)
	{
		p_znaki->AddTail(znak);
		UpdateKoeff();
		return p_znaki->GetAt(p_znaki->GetSize()-1);
	}
	else return NULL;
}

//===========================================================
//!Create a new sign with such term
MZnak* MRazdel::CreateNewZnak(MTermin termin,MString type)
//===========================================================
{
    //we create a new sign by default and
    //we set to it our term
	MZnak* zn= (*fpCreateZnakByType)(type);
	zn->SetTermin(termin);
	p_znaki->AddTail(zn);
	UpdateKoeff();
	delete zn;
	return p_znaki->GetAt(p_znaki->GetSize()-1);
}

//===========================================================
//!Create a new sign with such code
MZnak* MRazdel::CreateNewZnak(MString kod)
//===========================================================
{
    //we create a new sign by default and
    //we set to it our code
	MZnak* zn= new MZnak();
	zn->GetTermin()->SetKod(kod);
	p_znaki->AddTail(zn);
	UpdateKoeff();
	return p_znaki->GetAt(p_znaki->GetSize()-1);
}

//===========================================================
//!Create a new sign by default
MZnak* MRazdel::CreateNewZnak()
//===========================================================
{
	int i=p_znaki->GetSize();
	p_znaki->SetSize(i+1);
	UpdateKoeff();
	return p_znaki->GetAt(p_znaki->GetSize()-1);
}

//================================================
//!Returns quantity of elements in the section
int MRazdel::GetZnCount(MString strID)
//================================================
{
	if(strID.IsEmpty())
		return p_znaki->GetSize();
	else{
		int iCounter = 0 ;
		for(int i = p_znaki->GetSize()-1;i>=0;i--)
			if(p_znaki->GetAt(i)->GetClassType()==strID)
				iCounter++;
		return iCounter;
	}
}

//===========================================================
//!constructor by default
MZnakSistema::MZnakSistema()
//===========================================================
{
    //we create an empty array of sections
	p_razdeli=new MRlArray();
	m_fMasstKoeff = 1;
	m_iRealMasstab = MASSTAB_AR[MASSTAB_1000000_INDEX];
	m_strProection = COO_AR[COO_AR_ALLGAUSSKRUGER_INDEX];
}	

//===========================================================
//!constructor in the given parameters
MZnakSistema::MZnakSistema(MTermin termin,MRlArray* rla)
//===========================================================
{
	m_fMasstKoeff = 1;
	m_termin.Copy(&termin);
    //if there is an array of sections, create its copy
	if(rla)
		p_razdeli=(MRlArray*)rla->Clone();
    //otherwise we create an empty array of sections
	else
		p_razdeli=new MRlArray();
	m_iRealMasstab = MASSTAB_AR[MASSTAB_1000000_INDEX];
	m_strProection = COO_AR[COO_AR_ALLGAUSSKRUGER_INDEX];
	UpdateKoeff();
}

//===========================================================
//!constructor with the given standard
MZnakSistema::MZnakSistema(MZnakSistema* znsys)
//===========================================================
{
	m_fMasstKoeff=1;
    //if there is a standard
	if(znsys)
	{
		m_termin.Copy(&znsys->m_termin);
		p_razdeli=(MRlArray*)znsys->p_razdeli->Clone();
		m_fMasstKoeff = znsys->m_fMasstKoeff;
		m_iRealMasstab = znsys->m_iRealMasstab;
		m_strProection = znsys->m_strProection;
	}
    //if there is no standard, create everything by default
	else
	{
		p_razdeli=new MRlArray();
	}
}

//===========================================================
//!Copying
void MZnakSistema::Copy(MZnakSistema* znsys)
//===========================================================
{
    //we copy only when is from where
	if(znsys)
	{
		m_termin.Copy(&znsys->m_termin);
		delete p_razdeli;
		p_razdeli=(MRlArray*)znsys->p_razdeli->Clone();
		m_fMasstKoeff = znsys->m_fMasstKoeff;
		m_iRealMasstab = znsys->m_iRealMasstab;
		m_strProection = znsys->m_strProection;
	}
}


//===========================================================
//!Cloning
MZnakSistema* MZnakSistema::Clone()
//===========================================================
{
	return new MZnakSistema(this);
}

//===========================================================
//!Calculates the size
unsigned int MZnakSistema::GetBytes()
//===========================================================
{
    // Size =
    //      Identifier (3 bytes) +
    //      Size size (4 bytes) +
    //      Term size (variable) +
    //      Size of an array of sections (variable)
	unsigned int iBytes  =  3+4+m_termin.GetBytes()+p_razdeli->GetBytes();
    //real scale
    //scaling ratio
    //name of a projection
	iBytes += sizeof(m_iRealMasstab) + sizeof(m_fMasstKoeff) + m_strProection.GetLength() + 2;
	return iBytes;
}

//===========================================================
//!Reads from the Nux-file
bool MZnakSistema::ReadBin(FILE* h_nuxfile,int version)
//===========================================================
{
	switch( version ) {
        //version 0
		case 0:
            //we pass length of record and the identifier
			fseek(h_nuxfile, 7, SEEK_CUR);
            //reading term
			m_termin.ReadBin(h_nuxfile,version);

             //Scale of sign system
			fread(&m_iRealMasstab,sizeof(m_iRealMasstab), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_iRealMasstab, sizeof(m_iRealMasstab) );
            //Translation coefficient from map millimeter in unit geographical
			fread(&m_fMasstKoeff,sizeof(m_fMasstKoeff), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_fMasstKoeff, sizeof(m_fMasstKoeff) );
            //The line containing sign system
			m_strProection = ReadString(h_nuxfile,version);

            //reading array of signs
			p_razdeli->ReadBin(h_nuxfile,version);
			UpdateKoeff();
	}
	return true;
}

//===========================================================
//!Reads from the Nux-file
bool MZnakSistema::ReadBin(MFile* file,int version)
//===========================================================
{
	switch( version ) {
        //version 0
		case 0:
            //we pass length of record and the identifier
			file->SetPosition(7, MAP_SEEK_CUR);
            //reading term
			m_termin.ReadBin(file,version);

             //Scale of sign system
			file->GetData(&m_iRealMasstab,sizeof(m_iRealMasstab), 1);
		if (reverseReadFlag) reverseValue((char*)&m_iRealMasstab, sizeof(m_iRealMasstab) );
            //Translation coefficient from map millimeter in unit geographical
			file->GetData(&m_fMasstKoeff,sizeof(m_fMasstKoeff), 1);
		if (reverseReadFlag) reverseValue((char*)&m_fMasstKoeff, sizeof(m_fMasstKoeff) );
            //The line containing sign system
			m_strProection = ReadString(file,version);

            //reading array of signs
			p_razdeli->ReadBin(file,version);
			UpdateKoeff();
	}
	return true;
}

//===========================================================
//!Saves in the NUX file
bool MZnakSistema::WriteBin(FILE* h_nuxfile,  int version)
//===========================================================
{
    //Calculation of the extent of object in UTP
	unsigned int SBytes = GetBytes();
    //Writing
	switch( version ) {
        //version 0
		case 0:
            //Writing of the identifier of the section
			fwrite(ID_ZNAKSISTEMA, 3, 1, h_nuxfile);
            //Writing of length of object
			fwrite(&SBytes, 4, 1, h_nuxfile);
            //Writing termin
			m_termin.WriteBin(h_nuxfile,version);
			
             //Scale of sign system
			fwrite(&m_iRealMasstab, sizeof(m_iRealMasstab), 1, h_nuxfile);
            //Translation coefficient from map millimeter in unit geographical
			fwrite(&m_fMasstKoeff, sizeof(m_fMasstKoeff), 1, h_nuxfile);
            //The line containing sign system
			WriteString(h_nuxfile,version,m_strProection);

            //Writing of an array of signs
			p_razdeli->WriteBin(h_nuxfile,version);
	}
	return true;
}

//===========================================================
//!comparing of sign systems according to terms
bool MZnakSistema::IsTerminEqualTo(MZnakSistema* znsys)
//===========================================================
{
    //If there is nothing to compare
	if(! znsys) return false;
	
	return m_termin.IsEqualTo(&znsys->m_termin);
}

//===========================================================
//!comparing the term
bool MZnakSistema::IsTerminEqualTo(MTermin termin)
//===========================================================
{
	return m_termin.IsEqualTo(&termin);
}

//===========================================================
//!Give the term
MTermin* MZnakSistema::GetTermin()
//===========================================================
{
	return &m_termin;
}

//===========================================================
//!Give the term copy
MTermin* MZnakSistema::CreateCopyTermin()
//===========================================================
{
	return m_termin.Clone();
}


//===========================================================
//!Set the term
void MZnakSistema::SetTermin(MTermin termin)
//===========================================================
{
	m_termin.Copy(&termin);
}

//===========================================================
//!Give an array of sections
MRlArray* MZnakSistema::GetRazdelArray()
//===========================================================
{
	return p_razdeli;
}

//===========================================================
//!Give the copy of an array of sections
MRlArray* MZnakSistema::CreateCopyRazdelArray()
//===========================================================
{
	return (MRlArray*)p_razdeli->Clone();
}

//===========================================================
//!Set an array of sections
void MZnakSistema::SetRazdelArray(MRlArray* rla)
//===========================================================
{
	if(rla)
	{
		delete p_razdeli;
		p_razdeli=(MRlArray*)rla->Clone();
		UpdateKoeff();
	}
}

//===========================================================
//!Add an array of sections to the beginning
void MZnakSistema::AppendRazdelArrayHead(MRlArray* rla)
//===========================================================
{
	if(rla)
	{
		p_razdeli->AppendHead(rla);
		UpdateKoeff();
	}
}

//===========================================================
//!Add an array of sections to the end
void MZnakSistema::AppendRazdelArrayTail(MRlArray* rla)
//===========================================================
{
	if(rla)
	{
		p_razdeli->AppendTail(rla);
		UpdateKoeff();
	}
}

//===========================================================
//!Clear an array of sections
void MZnakSistema::ClearRazdelArray()
//===========================================================
{
	p_razdeli->ClearAll();
}

//===========================================================
//!Give the section with such term or NULL if it isn't present
MRazdel* MZnakSistema::GetRazdel(MTermin termin)
//===========================================================
{
	int cnt=p_razdeli->GetSize();
	MRazdel* rl;
    //we look for the section with such term
	for(int i=0; i<cnt;i++)
	{
		rl=p_razdeli->GetAt(i);
		if(rl->GetTermin()->IsEqualTo(&termin))
			return rl;
	}
    //if the section with such term isn't found, return NULL
	return NULL;
}

//===========================================================
//!Give the section with such code or NULL if it isn't present
MRazdel* MZnakSistema::GetRazdel(MString kod)
//===========================================================
{
	int cnt=p_razdeli->GetSize();
	MRazdel* rl;
    //we look for the section with such code
    for(int i=0; i<cnt;i++)
	{
		rl=p_razdeli->GetAt(i);
		if(rl->GetTermin()->GetKod()==kod)
			return rl;
	}
    //if the section with such code isn't found, return NULL
	return NULL;
}

//===========================================================
//!Give the copy of the section with such term or NULL if it isn't present
MRazdel* MZnakSistema::CreateCopyRazdel(MTermin termin)
//===========================================================
{
    //we look for the section
	MRazdel* rl = GetRazdel(termin);
    //if it is found, we make its copy
	if(rl)
	{
		return rl->Clone();
	}
    //else return NULL
	else
	{
		return NULL;
	}
}

//===========================================================
//!Give the copy of the section with such code or NULL if it isn't present
MRazdel* MZnakSistema::CreateCopyRazdel(MString kod)
//===========================================================
{
    //we look for the section
	MRazdel* rl = GetRazdel(kod);
    //if it is found, we make its copy
	if(rl)
	{
		return rl->Clone();
	}
    //else return NULL
	else
	{
		return NULL;
	}
}

//===========================================================
//!Delete the section with such term
void MZnakSistema::DeleteRazdel(MTermin termin)
//===========================================================
{
	int cnt=p_razdeli->GetSize();
	MRazdel* rl;
    //we look for the section with such term
	int i,j=-1;
	for(i=0;i<cnt;i++)
	{
		rl=p_razdeli->GetAt(i);
		if(rl->GetTermin()->IsEqualTo(&termin))
			j=i;
	}
    //if the section with such term is found, we delete it
	if(j!=-1) p_razdeli->Delete(j);
}

//===========================================================
//!Delete the section with such code
void MZnakSistema::DeleteRazdel(MString kod)
//===========================================================
{
	int cnt=p_razdeli->GetSize();
	MRazdel* rl;
    //we look for the section with such cose
    int i,j=-1;
	for(i=0;i<cnt;i++)
	{
		rl=p_razdeli->GetAt(i);
		if(rl->GetTermin()->GetKod()==kod)
			j=i;
	}
    //if the section with such code is found, we delete it
	if(j!=-1) p_razdeli->Delete(j);
}

//===========================================================
//!Add such section
MRazdel* MZnakSistema::AddRazdel(MRazdel* Razdel)
//===========================================================
{
	if(Razdel)
	{
		p_razdeli->AddTail(Razdel);
		return p_razdeli->GetAt(p_razdeli->GetSize()-1);
		UpdateKoeff();
	}
	else return NULL;
}

//===========================================================
//!Create the new section with such term
MRazdel* MZnakSistema::CreateNewRazdel(MTermin termin)
//===========================================================
{
    //We create the new section by default and
    //we set to it our term
	MRazdel* rl= new MRazdel();
	rl->SetTermin(termin);
	p_razdeli->AddTail(rl);
	UpdateKoeff();
	return p_razdeli->GetAt(p_razdeli->GetSize()-1);
}

//===========================================================
//!Create the new section with such code
MRazdel* MZnakSistema::CreateNewRazdel(MString kod)
//===========================================================
{
    //We create the new section by default and
    //we set to it our code
	MRazdel* rl= new MRazdel();
	rl->GetTermin()->SetKod(kod);
	p_razdeli->AddTail(rl);
	UpdateKoeff();
	return p_razdeli->GetAt(p_razdeli->GetSize()-1);
}

//===========================================================
//!Create the new section by default
MRazdel* MZnakSistema::CreateNewRazdel()
//===========================================================
{
    //Create the new section by default
	MRazdel* rl= new MRazdel();
	p_razdeli->AddTail(rl);
	UpdateKoeff();
	return p_razdeli->GetAt(p_razdeli->GetSize()-1);
}


//===========================================================
//!Give a sign with such term or NULL if it isn't present
MZnak* MZnakSistema::GetZnak(MTermin termin)
//===========================================================
{
	MRazdel* rl;
	MZnak* zn=NULL;
	int index=p_razdeli->GetSize()-1;
    //we go according to all sections, we won't find yet
	while((zn==NULL)&&(index>-1))
	{
		rl=p_razdeli->GetAt(index);
		zn=rl->GetZnak(termin);
		index--;
	}
	return zn;
}

//===========================================================
//!Give a sign with such double code or NULL if it isn't present
MZnak* MZnakSistema::GetZnak(MString dukod)
//===========================================================
{
    //we divide 'dukod' into two parts

	MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //section code
	rkod=GetPartOfString(0,dukod);
	if(rkod=="")return NULL;
    //sign code
	zkod=GetPartOfString(1,dukod);
	if(zkod=="")return NULL;
    //if in line there is still something, nothing is done
	temp=GetPartOfString(2,dukod);
	if(temp!="")return NULL;
    //we look for the section
	MRazdel* razdel=GetRazdel(rkod);
	if(!razdel) return NULL;
    //if the section is found, we look for a sign in this section
	MZnak* znak=razdel->GetZnak(zkod);
	return znak;
}

//===========================================================
//!Give a sign copy with such term or NULL if it isn't present
MZnak* MZnakSistema::CreateCopyZnak(MTermin termin)
//===========================================================
{
    //we look for a sign
	MZnak* zn=GetZnak(termin);
    //if found, return its copy
	if(zn) return zn->Clone();
	else return NULL;
}

//===========================================================
//!Give a copy of the sign with such double code or NULL if it isn't present
MZnak* MZnakSistema::CreateCopyZnak(MString dukod)
//===========================================================
{
    //we look for a sign
	MZnak* zn=GetZnak(dukod);
    //if found, return its copy
	if(zn) return zn->Clone();
	else return NULL;
}


//===========================================================
//!Create a new sign with such double code
MZnak* MZnakSistema::CreateNewZnak(MString dukod)
//===========================================================
{
    //we divide 'dukod' into two parts
    MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //section code
	rkod=GetPartOfString(0,dukod);
	if(rkod=="")return NULL;
    //sign code
	zkod=GetPartOfString(1,dukod);
	if(zkod=="")return NULL;
    //if in line there is still something, nothing is done
	temp=GetPartOfString(2,dukod);
	if(temp!="")return NULL;
    //we look for the section
	MRazdel* razdel=GetRazdel(rkod);
	if(!razdel) return NULL;

    //if the section is found, we create a sign in this section
	MZnak* zn = razdel->CreateNewZnak(zkod);
	return zn;
}


//================================================
//!Returns quantity of elements in sign system
int MZnakSistema::GetZnCount(MString strID)
//================================================
{
	int cnt = 0;
    //cycle according to sections
	int Size = p_razdeli->GetSize();
	MRazdel* razdel;
	for(int i=0;i<Size;i++)
	{
		razdel = p_razdeli->GetAt(i);
		cnt+=razdel->GetZnCount(strID);
	}

	return cnt;
}


//*******************************************************************
//******************************* MZnArray **************************
//*******************************************************************
//============================================================
//!Copies 'Out' contents in 'In'
/**for an array of signs, and also for an array of sections and an array of sign systems
//function doesn't generate the copy of an array of languages (signs/sections), and rearranges
//on them pointers; before killing the pointer, it is necessary to delete
//the array of languages (signs/sections) lying according to the old pointer
*/
/*void MZnArray::Copy(MZnak* In,MZnak* Out)
//============================================================
{
	In->SetTermin(*Out->GetTermin());
	In->SetClassType(Out->GetClassType());
	delete In->m_YazArray;
	In->m_YazArray = Out->m_YazArray;
	Out->m_YazArray = NULL;
};*/
void MZnArray::Copy(MZnak* In,MZnak* Out)
//============================================================
{
	In->SetTermin(*Out->GetTermin());
	In->SetClassType(Out->GetClassType());

	delete In->m_YazArray;
	In->m_YazArray = Out->m_YazArray;
	delete In->m_noms;
	In->m_noms = Out->m_noms;
	delete In->m_genattrs;
	In->m_genattrs = Out->m_genattrs;
	Out->m_YazArray = NULL;
	Out->m_noms = NULL;
	Out->m_genattrs = NULL;
};

//=========================================================
//!Creates the exact copy of an array
MArray* MZnArray::Clone()
//=========================================================
{
    //we create the copy
//	GlobalComplexArray* pClone=(GlobalComplexArray*)CreateArrayByType(WhatTheArray());
	MZnArray* pClone = (MZnArray*)CreateOfType();
	pClone->SetSize(Size);
    //We copy all elements
	for(int i=0; i<Size;i++){
		MZnak* pElClone = GetAt(i)->Clone();
		pClone->SetAt(i,pElClone/*GetAt(i)*/);
		delete pElClone;
	}
    //return the pointer on the copy
	return pClone;
};


//*******************************************************************
//******************************* MRlArray **************************
//*******************************************************************
//============================================================
//!Appropriates the line 'Out' to the line 'In'
void MRlArray::Copy(MRazdel* In,MRazdel* Out)
//============================================================
{
	In->SetTermin(*Out->GetTermin());
	delete In->p_znaki;
	In->p_znaki = Out->p_znaki;
	Out->p_znaki=NULL;
};
//=========================================================
//!Creates the exact copy of an array
MArray* MRlArray::Clone()
//=========================================================
{
    //we create the copy
//	GlobalComplexArray* pClone=(GlobalComplexArray*)CreateArrayByType(WhatTheArray());
	MRlArray* pClone = (MRlArray*)CreateOfType();
	pClone->SetSize(Size);
    //We copy all elements
	for(int i=0; i<Size;i++){
		MRazdel* pElClone = GetAt(i)->Clone();
		pClone->SetAt(i,pElClone/*GetAt(i)*/);
		delete pElClone;
	}
    //return the pointer on the copy
	return pClone;
};

//*******************************************************************
//******************************* MZsArray **************************
//*******************************************************************
//!Appropriates the line 'Out' to the line 'In'
void MZsArray::Copy(MZnakSistema* In,MZnakSistema* Out)
//============================================================
{
	In->SetTermin(*Out->GetTermin());
	delete In->p_razdeli;
	In->p_razdeli = Out->p_razdeli;
	Out->p_razdeli=NULL;
};

//=========================================================
//!Creates the exact copy of an array
MArray* MZsArray::Clone()
//=========================================================
{
    //we create the copy
//	GlobalComplexArray* pClone=(GlobalComplexArray*)CreateArrayByType(WhatTheArray());
	MZsArray* pClone = (MZsArray*)CreateOfType();
	pClone->SetSize(Size);
    //We copy all elements
	for(int i=0; i<Size;i++){
		MZnakSistema* pElClone = GetAt(i)->Clone();
		pClone->SetAt(i,pElClone/*GetAt(i)*/);
		delete pElClone;
	}
    //return the pointer on the copy
	return pClone;
}


MYaz *MZnak::GetYaz(int nomyaz) const
{
    if (!m_YazArray) return NULL;
    if((nomyaz<0)||(nomyaz>=m_YazArray->GetSize())) return NULL;
    return m_YazArray->GetAt(nomyaz);
}
