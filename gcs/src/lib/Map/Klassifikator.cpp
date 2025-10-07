#include "Klassifikator.h"
#include <fstream>
#include "Refglobal.h"

using namespace std;

//===========================================================
//! Constructor by default
MKlassifikator::MKlassifikator()
{
	p_sistemi = new MZsArray();
    //creation of the table of colors
	int iKey = m_ColorDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_ColorDict.SetKeyField(iKey);
	m_ColorDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_COLOR_NAME_INDEX);
	m_ColorDict.AddField(ID_CLA,KLS_TABLE_COLOR_STRING,KLS_TABLE_COLOR_VALUE_INDEX);
	m_ColorDict.SetMaxSize(UCHAR_MAX);
    //The table of colours is filled in by default
	MFixedSizeTable::InitalizeAsColorDefault(&m_ColorDict,KLS_TABLE_COLOR_VALUE_INDEX,KLS_TABLE_COLOR_NAME_INDEX);

    //creation of the table of handwrites
	m_PrkDict.SetMaxSize(UCHAR_MAX);
	iKey = m_PrkDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_PrkDict.SetKeyField(iKey);
	m_PrkDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_PRK_NAME_INDEX);
	m_PrkDict.AddField(ID_PKA,KLS_TABLE_PRK_STRING,KLS_TABLE_PRK_VALUE_INDEX);

    //creation of the table of images
	m_ImageDict.SetMaxSize(UCHAR_MAX);
	iKey = m_ImageDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_ImageDict.SetKeyField(iKey);
	m_ImageDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_IMAGE_NAME_INDEX);
	m_ImageDict.AddField(ID_IMG,KLS_TABLE_IMAGE_STRING,KLS_TABLE_IMAGE_VALUE_INDEX);

    //creation of the table of attributes
	m_AttrDict.SetMaxSize(USHRT_MAX);
	iKey = m_AttrDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_AttrDict.SetKeyField(iKey);
	m_AttrDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_ATTR_NAME_INDEX);
	m_AttrDict.AddField(ID_STA,KLS_TABLE_ATTR_ED_IZM_STRING,KLS_TABLE_ATTR_ED_IZM_INDEX);
	m_AttrDict.AddField(ID_STA,KLS_TABLE_ATTR_TYPE_STRING,KLS_TABLE_ATTR_TYPE_INDEX);

    //creation of the table of fonts
	m_FontDict.SetMaxSize(UCHAR_MAX);
	iKey = m_FontDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_FontDict.SetKeyField(iKey);
	m_FontDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_FONT_NAME_INDEX);
	m_FontDict.AddField(ID_LFA,KLS_TABLE_FONT_STRING,KLS_TABLE_FONT_VALUE_INDEX);

    //creation of the table of suffix&preffix strings
	m_MegStrDict.SetMaxSize(UCHAR_MAX);
	iKey = m_MegStrDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_MegStrDict.SetKeyField(iKey);
	m_MegStrDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_MEGSTR_NAME_INDEX);
	m_MegStrDict.AddField(ID_STA,KLS_TABLE_MEGSTR_STRING,KLS_TABLE_MEGSTR_VALUE_INDEX);
    m_klscolorkod = 1;
}


//===========================================================
//! Constructor in the set parameters
MKlassifikator::MKlassifikator(MTermin termin,MZsArray* zsa)
//===========================================================
{
	if(zsa)
	{
		m_termin.Copy(&termin);
		p_sistemi=(MZsArray*)zsa->Clone();
	}
	else
	{
		p_sistemi = new MZsArray();
	}

    //creation of the table of colors
    int iKey = m_ColorDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_ColorDict.SetKeyField(iKey);
	m_ColorDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_COLOR_NAME_INDEX);
	m_ColorDict.AddField(ID_CLA,KLS_TABLE_COLOR_STRING,KLS_TABLE_COLOR_VALUE_INDEX);
	m_ColorDict.SetMaxSize(UCHAR_MAX);
	MFixedSizeTable::InitalizeAsColorDefault(&m_ColorDict,KLS_TABLE_COLOR_VALUE_INDEX,KLS_TABLE_COLOR_NAME_INDEX);
    //creation of the table of handwrites
    m_PrkDict.SetMaxSize(UCHAR_MAX);
	iKey = m_PrkDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_PrkDict.SetKeyField(iKey);
	m_PrkDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_PRK_NAME_INDEX);
	m_PrkDict.AddField(ID_PKA,KLS_TABLE_PRK_STRING,KLS_TABLE_PRK_VALUE_INDEX);

    //creation of the table of images
    m_ImageDict.SetMaxSize(UCHAR_MAX);
	iKey = m_ImageDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_ImageDict.SetKeyField(iKey);
	m_ImageDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_IMAGE_NAME_INDEX);
	m_ImageDict.AddField(ID_IMG,KLS_TABLE_IMAGE_STRING,KLS_TABLE_IMAGE_VALUE_INDEX);

    //creation of the table of attributes
    m_AttrDict.SetMaxSize(USHRT_MAX);
	iKey = m_AttrDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_AttrDict.SetKeyField(iKey);
	m_AttrDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_ATTR_NAME_INDEX);
	m_AttrDict.AddField(ID_STA,KLS_TABLE_ATTR_ED_IZM_STRING,KLS_TABLE_ATTR_ED_IZM_INDEX);
	m_AttrDict.AddField(ID_STA,KLS_TABLE_ATTR_TYPE_STRING,KLS_TABLE_ATTR_TYPE_INDEX);

    //creation of the table of fonts
    m_FontDict.SetMaxSize(UCHAR_MAX);
	iKey = m_FontDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_FontDict.SetKeyField(iKey);
	m_FontDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_FONT_NAME_INDEX);
	m_FontDict.AddField(ID_LFA,KLS_TABLE_FONT_STRING,KLS_TABLE_FONT_VALUE_INDEX);

    //creation of the table of suffix&preffix strings
    m_MegStrDict.SetMaxSize(UCHAR_MAX);
	iKey = m_MegStrDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
	m_MegStrDict.SetKeyField(iKey);
	m_MegStrDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_MEGSTR_NAME_INDEX);
	m_MegStrDict.AddField(ID_STA,KLS_TABLE_MEGSTR_STRING,KLS_TABLE_MEGSTR_VALUE_INDEX);
    m_klscolorkod = 1;

}

//===========================================================
//!Constructor with the set standard
MKlassifikator::MKlassifikator(MKlassifikator* kls)
//===========================================================
{
	if(kls)
	{
		m_termin.Copy(&kls->m_termin);
		p_sistemi=(MZsArray*)kls->p_sistemi->Clone();

        //creation of the table of colors
        m_ColorDict.Copy(&kls->m_ColorDict);
        //creation of the table of handwrites
        m_PrkDict.Copy(&kls->m_PrkDict);
        //creation of the table of attributes
        m_AttrDict.Copy(&kls->m_AttrDict);
        //creation of the table of images
        m_ImageDict.Copy(&kls->m_ImageDict);
        //creation of the table of fonts
        m_FontDict.Copy(&kls->m_FontDict);
        //creation of the table of suffix&preffix strings
        m_MegStrDict.Copy(&kls->m_MegStrDict);
	}
	else
	{
		p_sistemi = new MZsArray();

        //creation of the table of colors
        int iKey = m_ColorDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
		m_ColorDict.SetKeyField(iKey);
		m_ColorDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_COLOR_NAME_INDEX);
		m_ColorDict.AddField(ID_CLA,KLS_TABLE_COLOR_STRING,KLS_TABLE_COLOR_VALUE_INDEX);
		m_ColorDict.SetMaxSize(UCHAR_MAX);
		MFixedSizeTable::InitalizeAsColorDefault(&m_ColorDict,KLS_TABLE_COLOR_VALUE_INDEX,KLS_TABLE_COLOR_NAME_INDEX);

        //creation of the table of handwrites
        m_PrkDict.SetMaxSize(UCHAR_MAX);
		iKey = m_PrkDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
		m_PrkDict.SetKeyField(iKey);
		m_PrkDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_PRK_NAME_INDEX);
		m_PrkDict.AddField(ID_PKA,KLS_TABLE_PRK_STRING,KLS_TABLE_PRK_VALUE_INDEX);
		
        //creation of the table of images
        m_ImageDict.SetMaxSize(UCHAR_MAX);
		iKey = m_ImageDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
		m_ImageDict.SetKeyField(iKey);
		m_ImageDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_IMAGE_NAME_INDEX);
		m_ImageDict.AddField(ID_IMG,KLS_TABLE_IMAGE_STRING,KLS_TABLE_IMAGE_VALUE_INDEX);

        //creation of the table of attributes
        m_AttrDict.SetMaxSize(USHRT_MAX);
		iKey = m_AttrDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
		m_AttrDict.SetKeyField(iKey);
		m_AttrDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_ATTR_NAME_INDEX);
		m_AttrDict.AddField(ID_STA,KLS_TABLE_ATTR_ED_IZM_STRING,KLS_TABLE_ATTR_ED_IZM_INDEX);
		m_AttrDict.AddField(ID_STA,KLS_TABLE_ATTR_TYPE_STRING,KLS_TABLE_ATTR_TYPE_INDEX);
		
        //creation of the table of fonts
        m_FontDict.SetMaxSize(UCHAR_MAX);
		iKey = m_FontDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
		m_FontDict.SetKeyField(iKey);
		m_FontDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_FONT_NAME_INDEX);
		m_FontDict.AddField(ID_LFA,KLS_TABLE_FONT_STRING,KLS_TABLE_FONT_VALUE_INDEX);
		
        //creation of the table of suffix&preffix strings
        m_MegStrDict.SetMaxSize(UCHAR_MAX);
		iKey = m_MegStrDict.AddField(ID_INA,KLS_TABLE_KEY_STRING,KLS_TABLE_KEY);
		m_MegStrDict.SetKeyField(iKey);
		m_MegStrDict.AddField(ID_STA,KLS_TABLE_NAME_STRING,KLS_TABLE_MEGSTR_NAME_INDEX);
		m_MegStrDict.AddField(ID_STA,KLS_TABLE_MEGSTR_STRING,KLS_TABLE_MEGSTR_VALUE_INDEX);

	}
}

MKlassifikator::~MKlassifikator()
{
  if (p_sistemi) delete p_sistemi;
  p_sistemi= NULL;

/*  
	 m_ColorDict.ClearTable();
	 m_PrkDict.ClearTable();		
	 m_AttrDict.ClearTable();		
	 m_FontDict.ClearTable();
	 m_MegStrDict.ClearTable();	
	 m_EdIzmDict.ClearTable();
*/
}
//===========================================================
//! Copying
void MKlassifikator::Copy(MKlassifikator* kls)
//===========================================================
{
	if(kls)
	{
		m_termin.Copy(&kls->m_termin);
		delete p_sistemi;
		p_sistemi=(MZsArray*)kls->p_sistemi->Clone();

        //Copying of tables
		m_ColorDict.Copy(&kls->m_ColorDict);
		m_PrkDict.Copy(&kls->m_PrkDict);
		m_ImageDict.Copy(&kls->m_ImageDict);
		m_AttrDict.Copy(&kls->m_AttrDict);
		m_FontDict.Copy(&kls->m_FontDict);
		m_MegStrDict.Copy(&kls->m_MegStrDict);

	}
}

//===========================================================
//! Cloning
MKlassifikator* MKlassifikator::Clone()
//===========================================================
{
	MKlassifikator* pClone=new MKlassifikator(this);
	return pClone;
}

//===========================================================
//! Considers the size
unsigned int MKlassifikator::GetBytes()
//===========================================================
{
    //Size =
    //identifier size (3 bytes) +
    //size of number of the version (2 bytes) +
    //size size (4 bytes) +
    //term size (variable) +
    //size of an index of background color of the classifier (1 byte) +
    //size of the array of sign systems (variable)
	
    //We calculate the size of all tables
	unsigned int slovsize = m_ColorDict.GetBytes()+m_PrkDict.GetBytes()+m_AttrDict.GetBytes()+m_ImageDict.GetBytes()+m_FontDict.GetBytes()+m_MegStrDict.GetBytes();

	unsigned int iRetVal = 3+2+4+m_termin.GetBytes()+slovsize+1+p_sistemi->GetBytes();
	return iRetVal;
}


//===========================================================
//! Reads from the Nux-file
/** The returned values:
//  0 - normal end
//  1 - the file isn't found
//  2 - incorrect heading of the file
*/
int MKlassifikator::ReadBin(char * filepath)
//===========================================================
{

	FILE*		h_nuxfile;
	char        id_file[] = "   ";
	short	    version = 0;

	h_nuxfile = fopen(filepath, "rb");
	if ( !h_nuxfile ) {
		return 1;
	}

	ClearZsArray();

    //reading identifier of the KLS file
	fread(&id_file, 3, 1, h_nuxfile);
	if (strcmp(id_file, ID_KLASSIFIKATOR)) {
		fclose(h_nuxfile);
		return 2;
	}
    //reading number of the version
	fread(&version, 2, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&version, sizeof(version) );

	switch( version ) {
        //version 0
		case 0:
            //we pass record length
			fseek(h_nuxfile, 4, SEEK_CUR);

            //reading term
			m_termin.ReadBin(h_nuxfile,version);

            //Reading dictionaries
			m_ColorDict.ReadBin(h_nuxfile,version);
			m_PrkDict.ReadBin(h_nuxfile,version);
			m_AttrDict.ReadBin(h_nuxfile,version);
			//m_ImageDict.ReadBin(h_nuxfile,version);
			m_FontDict.ReadBin(h_nuxfile,version);
			m_MegStrDict.ReadBin(h_nuxfile,version);
			m_EdIzmDict.ReadBin(h_nuxfile,version);

            //reading index of background color of the classifier
			fread(&m_klscolorkod, sizeof(unsigned char), 1, h_nuxfile);

            //reading array of sign systems
			p_sistemi->ReadBin(h_nuxfile,version);
			break;
		case 1:
            //we pass record length
			fseek(h_nuxfile, 4, SEEK_CUR);

            //reading term
			m_termin.ReadBin(h_nuxfile,0);

            //Reading dictionaries
            m_ColorDict.ReadBin(h_nuxfile,0);
			m_PrkDict.ReadBin(h_nuxfile,0);
			m_AttrDict.ReadBin(h_nuxfile,0);
			m_ImageDict.ReadBin(h_nuxfile,0);
			m_FontDict.ReadBin(h_nuxfile,0);
			m_MegStrDict.ReadBin(h_nuxfile,0);
			m_EdIzmDict.ReadBin(h_nuxfile,0);

            //reading index of background color of the classifier
            fread(&m_klscolorkod, sizeof(unsigned char), 1, h_nuxfile);

            //reading array of sign systems
            p_sistemi->ReadBin(h_nuxfile,0);
			break;

	}
	fclose(h_nuxfile);
	return 0;		
}

//===========================================================
//!Saves in the NUX-file
bool MKlassifikator::WriteBin(char * filepath)
//===========================================================
{
	
	FILE*		h_nuxfile;
	short		version=0;

	h_nuxfile = fopen(filepath, "wb");
	if ( !h_nuxfile ) {
		InternalMessage( "Can't open file !");
		return false;
	}

    //Calculation of the extent of object in UTP
	unsigned int SBytes = GetBytes();

    //Writing
	switch( version ) {
        //version 0
		case 0:
            //writing of the identifier
			fwrite(ID_KLASSIFIKATOR, 3, 1, h_nuxfile);

            //writing of the version of a format
			//fwrite(&version, 2, 1, h_nuxfile);
			int version_1 = 1;
			fwrite(&version_1, 2, 1, h_nuxfile);

            //writing of length of object
			fwrite(&SBytes, 4, 1, h_nuxfile);

            // terminology writing
			m_termin.WriteBin(h_nuxfile,version);

            //writing of dictionaries
			m_ColorDict.WriteBin(h_nuxfile,version);
			m_PrkDict.WriteBin(h_nuxfile,version);
			m_AttrDict.WriteBin(h_nuxfile,version);
			m_ImageDict.WriteBin(h_nuxfile,version);
			m_FontDict.WriteBin(h_nuxfile,version);
			m_MegStrDict.WriteBin(h_nuxfile,version);
			m_EdIzmDict.WriteBin(h_nuxfile,version);

            //writing of an index of background color of the classifier
			fwrite(&m_klscolorkod, sizeof(unsigned char), 1, h_nuxfile);

            //writing of the array of sign systems
			p_sistemi->WriteBin(h_nuxfile,version);


	}
	fclose(h_nuxfile);
	return true;		
	
}

//===========================================================
//! comparison of sign systems according to terms
bool MKlassifikator::IsTerminEqualTo(MKlassifikator* kls)
//===========================================================
{
	if(kls)
	{
		return m_termin.IsEqualTo(&kls->m_termin);
	}
	else return false;
}

//===========================================================
//!comparison of the term
bool MKlassifikator::IsTerminEqualTo(MTermin termin)
//===========================================================
{
	return m_termin.IsEqualTo(&termin);
}

//===========================================================
//!Getting term
MTermin* MKlassifikator::GetTermin()
//===========================================================
{
	return &m_termin;
}

//===========================================================
//!Getting copy of term
MTermin* MKlassifikator::CreateCopyTermin()
//===========================================================
{
	return m_termin.Clone();
}

//===========================================================
//!set the term
void MKlassifikator::SetTermin(MTermin termin)
//===========================================================
{
	m_termin.Copy(&termin);
}

//===========================================================
//!Give the array of sign systems
MZsArray* MKlassifikator::GetZsArray()
//===========================================================
{
	return p_sistemi;
}

//===========================================================
//!Give the copy of the array of sign systems
MZsArray* MKlassifikator::CreateCopyZsArray()
//===========================================================
{
	return (MZsArray*)p_sistemi->Clone();
}

//===========================================================
//!set the array of sign systems
void MKlassifikator::SetZsArray(MZsArray* zsa)
//===========================================================
{
	if(zsa)
	{
		delete p_sistemi;
		p_sistemi=(MZsArray*)zsa->Clone();
	}
}

//===========================================================
//!Add the array of sign systems to the beginning
void MKlassifikator::AppendZsArrayHead(MZsArray* zsa)
//===========================================================
{

	if(zsa)
	{
		p_sistemi->AppendHead(zsa);
	}
}

//===========================================================
//!Add the array of sign systems to the end
void MKlassifikator::AppendZsArrayTail(MZsArray* zsa)
//===========================================================
{
	if(zsa)
	{
		p_sistemi->AppendTail(zsa);
	}
}

//===========================================================
//!Clear the array of sign systems
void MKlassifikator::ClearZsArray()
//===========================================================
{
	p_sistemi->ClearAll();
}

//===========================================================
//!Get sign system with such term or NULL if it isn't present
MZnakSistema* MKlassifikator::GetZnakSistema(MTermin termin)
//===========================================================
{
	int cnt=p_sistemi->GetSize();
	MZnakSistema* zs;
    //we look for sign system with such temin
	for(int i=0; i<cnt;i++)
	{
		zs=p_sistemi->GetAt(i);
		if(zs->GetTermin()->IsEqualTo(&termin))
			return zs;
	}
    //if the sign system with such term isn't found, we return NULL
	return NULL;
}

//===========================================================
//!Get sign system with such code or NULL if it isn't present
MZnakSistema* MKlassifikator::GetZnakSistema(MString kod)
//===========================================================
{
	int cnt=p_sistemi->GetSize();
	MZnakSistema* zs;
    //we look for sign system with such code
	for(int i=0; i<cnt;i++)
	{
		zs=p_sistemi->GetAt(i);
		if(zs->GetTermin()->GetKod()==kod)
			return zs;
	}
    //if the sign system with such code isn't found, we return NULL
    return NULL;
}

//===========================================================
//!Get the copy of sign system with such term or NULL if it isn't present
MZnakSistema* MKlassifikator::CreateCopyZnakSistema(MTermin termin)
//===========================================================
{
	MZnakSistema* zs = GetZnakSistema(termin);
	return zs->Clone();
}

//===========================================================
//!Get the copy of sign system with such code or NULL if it isn't present
MZnakSistema* MKlassifikator::CreateCopyZnakSistema(MString kod)
//===========================================================
{
	MZnakSistema* zs = GetZnakSistema(kod);
	return zs->Clone();
}


//===========================================================
//!Remove sign system with such term
void MKlassifikator::DeleteZnakSistema(MTermin termin)
//===========================================================
{
	int cnt=p_sistemi->GetSize();
	MZnakSistema* zs;
    //we look for sign system with such temin
	int i,j=-1;
	for(i=0;i<cnt;i++)
	{
		zs=p_sistemi->GetAt(i);
		if(zs->GetTermin()->IsEqualTo(&termin))
			j=i;
	}
    //if the sign system with such term is found, we delete it
	if(j!=-1) p_sistemi->Delete(j);
}


//===========================================================
//!Delete of sign system with such code
void MKlassifikator::DeleteZnakSistema(MString dukod)
//===========================================================
{
    //we divide quad-code into four parts
	MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,dukod);
	if(kkod=="")return;
	if(kkod!=m_termin.GetKod())return;
    //code of sign system
	skod=GetPartOfString(1,dukod);
	if(skod=="")return;
    //if in line there is still something, nothing is done
	temp=GetPartOfString(2,dukod);
	if(temp!="")return;
	
    //we look for sign system
	MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return;
	
	
    //we delete for sign system
	MTermin termin = *zs->GetTermin();
	DeleteZnakSistema(termin);
}


//===========================================================
//!Addition of such sign system
MZnakSistema* MKlassifikator::AddZnakSistema(MZnakSistema* ZnakSistema)
//===========================================================
{
	if(ZnakSistema)
	{
		p_sistemi->AddTail(ZnakSistema);
		return p_sistemi->GetAt(p_sistemi->GetSize()-1);
	}
	else return NULL;
}


//===========================================================
//!Create new sign system with such term
MZnakSistema* MKlassifikator::CreateNewZnakSistema(MTermin termin)
//===========================================================
{
	MZnakSistema *zs1;
	MZnakSistema *zs = new MZnakSistema();
	zs->SetTermin(termin);
	zs1=AddZnakSistema(zs);
	delete zs;
	return zs1;
}


//===========================================================
//!Create new sign system with such code
MZnakSistema* MKlassifikator::CreateNewZnakSistema(MString kod)
//===========================================================
{
	MZnakSistema *zs1;
	MZnakSistema *zs = new MZnakSistema();
	zs->GetTermin()->SetKod(kod);
	zs1=AddZnakSistema(zs);
	delete zs;
	return zs1;
}


//===========================================================
//!Create new sign system by default
MZnakSistema* MKlassifikator::CreateNewZnakSistema()
//===========================================================
{
	MZnakSistema *zs1;
	MZnakSistema *zs = new MZnakSistema();
	zs1=AddZnakSistema(zs);
	delete zs;
	return zs1;
}


//===========================================================
//!Get a sign with such term or NULL if it isn't present
MZnak* MKlassifikator::GetZnak(MTermin termin)
//===========================================================
{
	MZnakSistema* zs;
	MZnak* zn=NULL;
	int index=p_sistemi->GetSize()-1;
    //we go on all sign systems, we won't find yet
	while((zn==NULL)&&(index>-1))
	{
		zs=p_sistemi->GetAt(index);
		zn=zs->GetZnak(termin);
		index--;
	}
	return zn;
}

//===========================================================
//!Get a sign with such fourfold code or NULL if it isn't present
MZnak* MKlassifikator::GetZnak(const MString& quadrakod,MString& TrueQuadra, bool IgnoreKKod)
//===========================================================
{
	if(GetPartOfString(0,quadrakod)==SERV_OBJECT)
		TrueQuadra = quadrakod;
	else
	{
		TrueQuadra = SERV_OBJECT;
		TrueQuadra=TrueQuadra+" "+quadrakod;
	}
	//TrueQuadra = "";
    //we divide quad-code into two parts
	MString fkod="";
	MString kkod="";
	MString skod="";
	MString rkod="";
	MString zkod="";
	MString temp="";
	char fkod1[255];
	char kkod1[255];
	char skod1[255];
	char rkod1[255];
	char zkod1[255];
	char temp1[255];
    //We try to receive codes
    //classifier code
	char buf[255];
	int f = 0;
	for(f=0; f<255;f++)buf[f]=0;
	for(f=0;f<(int)quadrakod.GetLength();f++)
		buf[f]=quadrakod[f];
	int j=0;


	j=sscanf(buf,"%s%s%s%s%s%s",fkod1,kkod1,skod1,rkod1,zkod1,temp1);
	if((j!=4)&&(j!=5)) return NULL;
	fkod=fkod1;
	kkod=kkod1;
	skod=skod1;
	rkod=rkod1;
	if(j==4) zkod="";
	else zkod=zkod1;
	temp="";

    //Check of a correctness of a quad-code
	if(fkod=="")return NULL;
//	if(fkod!=SERV_OBJECT)
	if(j==4)
	{
		temp = zkod;
		zkod = rkod;
		rkod = skod;
		skod = kkod;
		kkod = fkod;
	}
	if(kkod=="") return NULL;
	if((!IgnoreKKod)&&(kkod!=GetTermin()->GetKod())) return NULL;
	if(skod=="")return NULL;
	if(rkod=="")return NULL;
	if(zkod=="")return NULL;
	if(temp!="")return NULL;

	
    //we look for sign system
	MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return NULL;
    //we look for the section
	MRazdel* razdel=zs->GetRazdel(rkod);
	if(!razdel) return NULL;
    //we look for the sign
	MZnak* zn=razdel->GetZnak(zkod);

	TrueQuadra = GetTermin()->GetKod()+" "+skod+" "+rkod+" "+zkod;
	
	return zn;
}

//===========================================================
//!Get the copy of a sign with such term or NULL if it isn't present
MZnak* MKlassifikator::CreateCopyZnak(MTermin termin)
//===========================================================
{
	MZnak* zn = GetZnak(termin);
	if(zn)
	{
		return zn->Clone();
	}
	else
	{
		return NULL;
	}
}

//===========================================================

//!Get the copy of a sign with such fourfold code or NULL if it isn't present
MZnak* MKlassifikator::CreateCopyZnak(MString quadrakod)
//===========================================================
{
	MString str;
	MZnak* zn = GetZnak(quadrakod,str);
	if(zn)
	{
		return zn->Clone();
	}
	else
	{
		return NULL;
	}
}


//===========================================================
//!Create a new sign with such fourfold code
MZnak* MKlassifikator::CreateNewZnak(MString quadrakod)
//===========================================================
{
    //we divide quad-code into four parts
	MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,quadrakod);
	if(kkod=="")return NULL;
	if(kkod!=m_termin.GetKod())return NULL;
    //code of sign system
	skod=GetPartOfString(1,quadrakod);
	if(skod=="")return NULL;
    //code of the section
	rkod=GetPartOfString(2,quadrakod);
	if(rkod=="")return NULL;
    //code of the sign
	zkod=GetPartOfString(3,quadrakod);
	if(zkod=="")return NULL;
    //if in line there is still something, nothing is done
	temp=GetPartOfString(4,quadrakod);
	if(temp!="")return NULL;
	
    //we look for sign system
	MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return NULL;
    //we look for the section
	MRazdel* razdel=zs->GetRazdel(rkod);
	if(!razdel) return NULL;
    //we look for the sign
	return razdel->CreateNewZnak(zkod);
}


//================================================
//!Returns quantity of elements in the classifier
int MKlassifikator::GetZnCount(MString strID)
//================================================
{
	int cnt = 0;
    //cycle according to sections
	int Size = p_sistemi->GetSize();
	MZnakSistema* sistema;
	for(int i=0;i<Size;i++)
	{
		sistema = p_sistemi->GetAt(i);
		cnt+=sistema->GetZnCount(strID);
	}

	return cnt;
}

//===========================================================
//!To remove a sign with such quad-code
void MKlassifikator::DeleteZnak(MString quadrakod)
//===========================================================
{
    //we divide quad-code into four parts
	MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,quadrakod);
	if(kkod=="")return;
	if(kkod!=m_termin.GetKod())return;
    //code of sign system
	skod=GetPartOfString(1,quadrakod);
	if(skod=="")return;
    //code of the section
	rkod=GetPartOfString(2,quadrakod);
	if(rkod=="")return;
    //code of the sign
	zkod=GetPartOfString(3,quadrakod);
	if(zkod=="")return;
    //if in line there is still something, nothing is done
	temp=GetPartOfString(4,quadrakod);
	if(temp!="")return;
	
    //we look for sign system
	MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return;
    //we look for the section
	MRazdel* razdel=zs->GetRazdel(rkod);
	if(!razdel) return;
    //we look for the sign
	MZnak* znak=razdel->GetZnak(zkod);
	if(!znak) return;

	
    //we delete the sign
	razdel->DeleteZnak(zkod);
}

//===========================================================
//!To remove the section with such three-code
void MKlassifikator::DeleteRazdel(MString trikod)
//===========================================================
{
    //we divide quad-code into four parts
	MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,trikod);
	if(kkod=="")return;
	if(kkod!=m_termin.GetKod())return;
    //code of sign system
    skod=GetPartOfString(1,trikod);
	if(skod=="")return;
    //code of the section
    rkod=GetPartOfString(2,trikod);
	if(rkod=="")return;
    //if in line there is still something, nothing is done
    temp=GetPartOfString(3,trikod);
	if(temp!="")return;
	
    //we look for sign system
    MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return;
    //we look for the section
    MRazdel* razdel=zs->GetRazdel(rkod);
	if(!razdel) return;
	

    //we delete the section
    zs->DeleteRazdel(rkod);
}

//===========================================================
//!To rename a sign with such quad-code
void MKlassifikator::RenameZnak(MString quadrakod, MString newkod)
//===========================================================
{
    //We check a code correctness
	newkod.TrimLeft();
	newkod.TrimRight();
	if (newkod=="") return;
    //we divide quad-code into four parts
    MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,quadrakod);
	if(kkod=="")return;
	if(kkod!=m_termin.GetKod())return;
    //code of sign system
    skod=GetPartOfString(1,quadrakod);
	if(skod=="")return;
    //code of the section
    rkod=GetPartOfString(2,quadrakod);
	if(rkod=="")return;
    //code of the sign
    zkod=GetPartOfString(3,quadrakod);
	if(zkod=="")return;
    //if in line there is still something, nothing is done
    temp=GetPartOfString(4,quadrakod);
	if(temp!="")return;
	
    //we look for sign system
    MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return;
    //we look for the section
    MRazdel* razdel=zs->GetRazdel(rkod);
	if(!razdel) return;
    //we look for the sign
    MZnak* znak=razdel->GetZnak(zkod);
	if(!znak) return;
    //we delete the sign
    znak->GetTermin()->SetKod(newkod);
}


//===========================================================
//!To rename the section with such three-code
void MKlassifikator::RenameRazdel(MString trikod, MString newkod)
//===========================================================
{
    //We check a code correctness
    newkod.TrimLeft();
	newkod.TrimRight();
	if (newkod=="") return;
    //we divide quad-code into four parts
    MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
	MString temp1;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,trikod);
	if(kkod=="")return;
	if(kkod!=m_termin.GetKod())return;
    //code of sign system
    skod=GetPartOfString(1,trikod);
	if(skod=="")return;
    //code of the section
    rkod=GetPartOfString(2,trikod);
	if(rkod=="")return;
    //if in line there is still something, nothing is done
    temp=GetPartOfString(3,trikod);
	if(temp!="")return;
	
    //we look for sign system
    MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return;
    //we look for the section
    MRazdel* razdel=zs->GetRazdel(rkod);
	if(!razdel) return;
	

    //rename the section
	razdel->GetTermin()->SetKod(newkod);
}
//===========================================================
//!To rename sign system with such double code
void MKlassifikator::RenameZnakSistema(MString dukod, MString newkod)
//===========================================================
{
    //We check a code correctness
    newkod.TrimLeft();
	newkod.TrimRight();
	if (newkod=="") return;
    //we divide quad-code into four parts
    MString kkod;
	MString skod;
	MString rkod;
	MString zkod;
	MString temp;
	MString temp1;
    //We try to receive codes
    //classifier code
	kkod=GetPartOfString(0,dukod);
	if(kkod=="")return;
	if(kkod!=m_termin.GetKod())return;
    //code of sign system
    skod=GetPartOfString(1,dukod);
	if(skod=="")return;
    //if in line there is still something, nothing is done
    temp=GetPartOfString(2,dukod);
	if(temp!="")return;
	
    //we look for sign system
    MZnakSistema* zs= GetZnakSistema(skod);
	if(!zs)return;

    //rename the sign system
    zs->GetTermin()->SetKod(newkod);
	
}


//===========================================================
//!To rename itself
void MKlassifikator::RenameKlassifikator(MString newkod)
//===========================================================
{
    //We check a code correctness
    newkod.TrimLeft();
	newkod.TrimRight();
	if (newkod=="") return;
    //To rename itself
	GetTermin()->SetKod(newkod);
}

//!Functions of work with tables
//Table of colors
//!=================================================
bool MKlassifikator::GetDictColorName(unsigned short int ushIndex,MString& strName)
//=================================================
{
	MString *pstr = (MString*)m_ColorDict.GetCell((int)ushIndex,KLS_TABLE_COLOR_NAME_INDEX);
	if(!pstr)
		return false;
	strName = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictColorValue(unsigned short int ushIndex,MColor& colorValue)
//=================================================
{
	MColor *pclr = (MColor*)m_ColorDict.GetCell((int)ushIndex,KLS_TABLE_COLOR_VALUE_INDEX);
	if(!pclr)
		return false;
	colorValue.SetColor(pclr->GetR(), pclr->GetG(), pclr->GetB());
	return true;
}

//!=================================================
bool MKlassifikator::SetDictColorValue(unsigned short int ushIndex,MColor colorValue)
//=================================================
{
	return m_ColorDict.SetCell((int)ushIndex,KLS_TABLE_COLOR_VALUE_INDEX,&colorValue);
}

//!=================================================
bool MKlassifikator::SetDictColorName(unsigned short int ushIndex,MString strName)
//=================================================
{
	return m_ColorDict.SetCell((int)ushIndex,KLS_TABLE_COLOR_NAME_INDEX,&strName);
}

//!=================================================
bool MKlassifikator::AddDictColor(MColor clrrfValue, short& ushKey,MString strName/*=""*/)
//=================================================
{
	ushKey = m_ColorDict.CreateRecord();
	if(ushKey==-1)
		return false;
	if(m_ColorDict.SetCell((int)ushKey,KLS_TABLE_COLOR_NAME_INDEX,&strName))
		return m_ColorDict.SetCell((int)ushKey,KLS_TABLE_COLOR_VALUE_INDEX,&clrrfValue);
	else 
		return false;
}


//Table of handwrites
//!=================================================
bool MKlassifikator::GetDictPrkName(unsigned short int ushIndex,MString& strName)
//=================================================
{
	MString *pstr = (MString*)m_PrkDict.GetCell((int)ushIndex,KLS_TABLE_PRK_NAME_INDEX);
	if(!pstr)
		return false;
	strName = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictPrkValue(unsigned short int ushIndex,MPrk& prkValue)
//=================================================
{
	MPrk *pstr = (MPrk*)m_PrkDict.GetCell((int)ushIndex,KLS_TABLE_PRK_VALUE_INDEX);
	if(!pstr)
		return false;
	prkValue.Copy(pstr);
	return true;
}

//!=================================================
bool MKlassifikator::SetDictPrkValue(unsigned short int ushIndex,MPrk* prkValue)
//=================================================
{
	return m_PrkDict.SetCell((int)ushIndex,KLS_TABLE_PRK_VALUE_INDEX,prkValue);
}


//!=================================================
bool MKlassifikator::SetDictPrkName(unsigned short int ushIndex,MString strName)
//=================================================
{
	return m_PrkDict.SetCell((int)ushIndex,KLS_TABLE_PRK_NAME_INDEX,&strName);
}

//!=================================================
bool MKlassifikator::AddDictPrk(MPrk* prkValue, short& ushKey,MString strName/*=""*/)
//=================================================
{
	ushKey = m_PrkDict.CreateRecord();
	if(ushKey==-1)
		return false;
	if(m_PrkDict.SetCell((int)ushKey,KLS_TABLE_PRK_NAME_INDEX,&strName))
		return m_PrkDict.SetCell((int)ushKey,KLS_TABLE_PRK_VALUE_INDEX,prkValue);
	else 
		return false;
}



//Table of attributes
//!=================================================
bool MKlassifikator::GetDictAttrName(unsigned short int ushIndex,MString& strName)
//=================================================
{
	MString *pstr = (MString*)m_AttrDict.GetCell((int)ushIndex,KLS_TABLE_ATTR_NAME_INDEX);
	if(!pstr)
		return false;
	strName = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictAttrEdIzm(unsigned short int ushIndex,MString& edIzm)
//=================================================
{
	MString *pstr = (MString*)m_AttrDict.GetCell((int)ushIndex,KLS_TABLE_ATTR_ED_IZM_INDEX);
	if(!pstr)
		return false;
	edIzm = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictAttrType(unsigned short int ushIndex,MString& type)
//=================================================
{
	MString *pstr = (MString*)m_AttrDict.GetCell((int)ushIndex,KLS_TABLE_ATTR_TYPE_INDEX);
	if(!pstr)
		return false;
	type = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::SetDictAttrName(unsigned short int ushIndex,MString strName)
//=================================================
{
	return m_AttrDict.SetCell((int)ushIndex,KLS_TABLE_ATTR_NAME_INDEX,&strName);
}

//!=================================================
bool MKlassifikator::SetDictAttrEdIzm(unsigned short int ushIndex,MString edIzm)
//=================================================
{
	return m_AttrDict.SetCell((int)ushIndex,KLS_TABLE_ATTR_ED_IZM_INDEX,&edIzm);
}

//!=================================================
bool MKlassifikator::SetDictAttrType(unsigned short int ushIndex,MString type)
//=================================================
{
	return m_AttrDict.SetCell((int)ushIndex,KLS_TABLE_ATTR_TYPE_INDEX,&type);
}

//!=================================================
bool MKlassifikator::AddDictAttr(MString strName, short& ushKey,MString strType/* = ""*/,MString strEdIzm /*= ""*/)
//=================================================
{
	ushKey = m_AttrDict.CreateRecord();
	if(ushKey==-1)
		return false;
	if((m_AttrDict.SetCell((int)ushKey,KLS_TABLE_ATTR_NAME_INDEX,&strName))&&
		(m_AttrDict.SetCell((int)ushKey,KLS_TABLE_ATTR_TYPE_INDEX,&strType)))
		return (m_AttrDict.SetCell((int)ushKey,KLS_TABLE_ATTR_ED_IZM_INDEX,&strEdIzm));
	else 
		return false;
}

//Table of images
//!=================================================
bool MKlassifikator::GetDictImageName(unsigned short int ushIndex,MString& strName)
//=================================================
{
	MString *pstr = (MString*)m_ImageDict.GetCell((int)ushIndex,KLS_TABLE_IMAGE_NAME_INDEX);
	if(!pstr)
		return false;
	strName = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictImageValue(unsigned short int ushIndex,MImage &image)
//=================================================
{
	MImage *img = (MImage *)m_ImageDict.GetCell((int)ushIndex,KLS_TABLE_IMAGE_VALUE_INDEX);
	if(!img)
		return false;
	image.Copy(img);
	return true;
}

//!=================================================
bool MKlassifikator::SetDictImageName(unsigned short int ushIndex,MString strName)
//=================================================
{
	return m_ImageDict.SetCell((int)ushIndex,KLS_TABLE_IMAGE_NAME_INDEX,&strName);
}

//!=================================================
bool MKlassifikator::SetDictImageValue(unsigned short int ushIndex,MImage *image)
//=================================================
{
	MImage *img;
	if (image)
		img = image->Clone();
	else
		return false;
	return m_ImageDict.SetCell((int)ushIndex,KLS_TABLE_IMAGE_VALUE_INDEX,img);
}

//!=================================================
bool MKlassifikator::AddDictImage(MString strName, short& ushKey, MImage *image)
//=================================================
{
	MImage *img;
	if (image)
		img = image->Clone();
	else
		return false;

	ushKey = m_ImageDict.CreateRecord();
	if(ushKey==-1)
		return false;
	if (m_ImageDict.SetCell((int)ushKey,KLS_TABLE_IMAGE_NAME_INDEX,&strName))
		return m_ImageDict.SetCell((int)ushKey,KLS_TABLE_IMAGE_VALUE_INDEX,img);
	else 
		return false;
}





//Table of fonts
//!=================================================
bool MKlassifikator::GetDictFontName(unsigned short int ushIndex,MString& strName)
//=================================================
{
	MString *pstr = (MString*)m_AttrDict.GetCell((int)ushIndex,KLS_TABLE_FONT_NAME_INDEX);
	if(!pstr)
		return false;
	strName = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictFontValue(unsigned short int ushIndex,M_LOG_FONT& Value)
//=================================================
{
	M_LOG_FONT *pstr = (M_LOG_FONT*)m_AttrDict.GetCell((int)ushIndex,KLS_TABLE_FONT_VALUE_INDEX);
	if(!pstr)
		return false;
	Value = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::SetDictFontValue(unsigned short int ushIndex,M_LOG_FONT lfValue)
//=================================================
{
	return m_FontDict.SetCell((int)ushIndex,KLS_TABLE_FONT_VALUE_INDEX,&lfValue);
}

//!=================================================
bool MKlassifikator::SetDictFontName(unsigned short int ushIndex,MString strName)
//=================================================
{
	return m_FontDict.SetCell((int)ushIndex,KLS_TABLE_FONT_NAME_INDEX,&strName);
}

//!=================================================
bool MKlassifikator::AddDictFont(M_LOG_FONT lfValue, short& ushKey,MString strName/*=""*/)
//=================================================
{
	ushKey = m_FontDict.CreateRecord();
	if(ushKey==-1)
		return false;
	if(m_FontDict.SetCell((int)ushKey,KLS_TABLE_FONT_NAME_INDEX,&strName))
		return (m_FontDict.SetCell((int)ushKey,KLS_TABLE_FONT_VALUE_INDEX,&lfValue));
	else 
		return false;
}




//Table of suffix&preffix strings
//!=================================================
bool MKlassifikator::GetDictMegstrName(unsigned short int ushIndex,MString& strName)
//=================================================
{
	MString *pstr = (MString*)m_MegStrDict.GetCell((int)ushIndex,KLS_TABLE_MEGSTR_NAME_INDEX);
	if(!pstr)
		return false;
	strName = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::GetDictMegstrValue(unsigned short int ushIndex,MString& Value)
//=================================================
{
	MString *pstr = (MString*)m_MegStrDict.GetCell((int)ushIndex,KLS_TABLE_MEGSTR_VALUE_INDEX);
	if(!pstr)
		return false;
	Value = *pstr;
	return true;
}

//!=================================================
bool MKlassifikator::SetDictMegstrValue(unsigned short int ushIndex,MString megstrValue)
//=================================================
{
	return m_MegStrDict.SetCell((int)ushIndex,KLS_TABLE_MEGSTR_VALUE_INDEX,&megstrValue);
}

//!=================================================
bool MKlassifikator::SetDictMegstrName(unsigned short int ushIndex,MString strName)
//=================================================
{
	return m_MegStrDict.SetCell((int)ushIndex,KLS_TABLE_MEGSTR_NAME_INDEX,&strName);
}

//!=================================================
bool MKlassifikator::AddDictMegstr(MString megstrValue, short& ushKey,MString strName/*=""*/)
//=================================================
{
	ushKey = m_MegStrDict.CreateRecord();
	if(ushKey==-1)
		return false;
	if(m_MegStrDict.SetCell((int)ushKey,KLS_TABLE_MEGSTR_NAME_INDEX,&strName))
		return (m_MegStrDict.SetCell((int)ushKey,KLS_TABLE_MEGSTR_VALUE_INDEX,&megstrValue));
	else 
		return false;
}


//=============================================================================
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111
//    SET OF FUNCTIONS WHICH WILL BE REMOVED AFTER CREATION OF TABLES
//!==============================================================================
bool MKlassifikator::GetColorDictNumValue(MColor clrValue,unsigned short int& ExistKey)
{
	for(int i = m_ColorDict.GetMaxSize()-1;i>=0;i--){
		MColor* pclr = (MColor*)m_ColorDict.GetCell((unsigned short int)i,KLS_TABLE_COLOR_VALUE_INDEX);
		if(pclr)
			if(!memcmp(pclr,&clrValue,sizeof(MColor))){
				ExistKey = i;
				return true;
			}
	}
	return false;
}



//! ===
bool MKlassifikator::GetPrkDictNumValue(MPrk *clrValue,unsigned short int& ExistKey)
{
	for(int i = m_PrkDict.GetMaxSize()-1;i>=0;i--){
		MPrk* pclr = (MPrk*)m_PrkDict.GetCell((unsigned short int)i,KLS_TABLE_PRK_VALUE_INDEX);
		if(pclr)
			if(pclr->IsEqualTo(clrValue)){
				ExistKey = i;
				return true;
			}
	}
	return false;
}

bool MKlassifikator::GetPrkDictNumValue(MString clrValue,unsigned short int& ExistKey)
{
	for(int i = m_PrkDict.GetMaxSize()-1;i>=0;i--){
		MString* pclr = (MString*)m_PrkDict.GetCell((unsigned short int)i,KLS_TABLE_PRK_NAME_INDEX);
		if(pclr)
			if(*pclr == clrValue){
				ExistKey = i;
				return true;
			}
	}
	return false;
}


//! ------------
bool MKlassifikator::GetAttrDictNumValue(MString clrValue,unsigned short int& ExistKey)
{
//
	unsigned short int idx1, idx2;
	int Sz = m_AttrDict.GetMaxSize()-1;
	for(int i = Sz;i>=(int)(Sz/2)+1;i--){
	 	idx1 = (unsigned short int)i;
		MString* pclr = (MString*)m_AttrDict.GetCell(idx1,KLS_TABLE_ATTR_NAME_INDEX);
		if(pclr)
			if(*pclr==clrValue){
				ExistKey = i;
				return true;
			}
		idx2 = (unsigned short int)(Sz-i);
		pclr = (MString*)m_AttrDict.GetCell(idx2,KLS_TABLE_ATTR_NAME_INDEX);
		if(pclr)
			if(*pclr==clrValue){
				ExistKey = Sz -i;
				return true;
			}

    }

	return false;
}


//! ===========
bool MKlassifikator::GetFontDictNumValue(M_LOG_FONT clrValue,unsigned short int& ExistKey)
{
	for(int i = m_FontDict.GetMaxSize()-1;i>=0;i--){
		M_LOG_FONT* pclr = (M_LOG_FONT*)m_FontDict.GetCell((unsigned short int)i,KLS_TABLE_FONT_VALUE_INDEX);
		if(pclr)
			if(!memcmp(pclr,&clrValue,sizeof(M_LOG_FONT))){
				ExistKey = i;
				return true;
			}
	}
	return false;
}



//!  ============
bool MKlassifikator::GetMegStrDictNumValue(MString clrValue,unsigned short int& ExistKey)
{
	for(int i = m_MegStrDict.GetMaxSize()-1;i>=0;i--){
		MString* pclr = (MString*)m_MegStrDict.GetCell((unsigned short int)i,KLS_TABLE_MEGSTR_VALUE_INDEX);
		if(pclr)
			if(*pclr==clrValue){
				ExistKey = i;
				return true;
			}
	}
	return false;
}
//=============================================================================
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11111
//    THE END OF THE SET OF FUNCTIONS WHICH WILL BE REMOVED AFTER CREATION OF TABLES
//==============================================================================

//FUNCTIONS of REPLACEMENT of COLOUR of HANDWRITING of the BOUNDARY LINE AND the CHARACTERISTIC IN OBYEKTAKH KLASSIFIKATORA
//==============================================================================
//!replacement of color of handwriting in the dictionary of handwritings
bool MKlassifikator::ChangePrkColorInPrkDict(unsigned char oldcolorkod,unsigned char newcolorkod)	
//==============================================================================
{
	int kod, prevkod;
//	KLS_TABLE_PRK_VALUE_INDEX - table column index in which handwritings lie
	bool par;
	MTable* prktable = GetPrkTable();
	par = prktable->GetFirstKey(kod);
	while(par){
		MPrk* prk = (MPrk*)prktable->GetCell(kod,KLS_TABLE_PRK_VALUE_INDEX);
		if(prk->colorkod == oldcolorkod)
			prk->colorkod = newcolorkod;
		if(prk->bkcolorkod == oldcolorkod)
			prk->bkcolorkod = newcolorkod;
		prevkod = kod;
		par = prktable->GetNextKey(prevkod,kod);
	}

	return true;
}
//!==============================================================================
bool MKlassifikator::ChangeColor(unsigned char oldcolorkod,unsigned char newcolorkod)	
//==============================================================================
{
    //if it is necessary background color of the classifier is changed
	if(m_klscolorkod == oldcolorkod)
		m_klscolorkod = newcolorkod;
    //we change if it is necessary color in all signs of the classifier
	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
                //on all languages of the array of languages of a sign
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
				for(int s=0; s<pYazArray->GetSize(); s++){
					pYaz = znak->GetYaz(s);
					int type = pYaz->GetType();
					switch (type)
					{
                    //areal objects
					case F_PLO:
						{
							MPloYaz* ployaz = (MPloYaz*)pYaz->m_object;
                            //object border
							for(int i=0; i<ployaz->m_linia->GetSize(); i++){
								MProsloika* prosl = ployaz->m_linia->GetAt(i);
								int typeprosl = prosl->GetType();
								switch(typeprosl)
								{
                                //simple line
								case 0:
									{
										MLinia* linia = (MLinia*)prosl;
										if(linia->m_colorkod == oldcolorkod)
											linia->m_colorkod = newcolorkod; 
									}
									break;
                                //shaped line
								case 1:
									{
										MShtrih* shtrih = (MShtrih*)prosl;
										if(shtrih->m_colorkod == oldcolorkod)
											shtrih->m_colorkod = newcolorkod; 
									}
									break;
                                // dotted line
								case 2:
									{
										MPunktir* punktir = (MPunktir*)prosl;
										if(punktir->m_colorkod == oldcolorkod)
											punktir->m_colorkod = newcolorkod; 
									}
									break;
                                //symbol broach
								case 3:
									{
										MSbStretch* sbstretch = (MSbStretch*)prosl;
										if(sbstretch->m_symbol.colorkod == oldcolorkod)
											sbstretch->m_symbol.colorkod = newcolorkod;
										if(sbstretch->m_symbol.bkcolorkod == oldcolorkod)
											sbstretch->m_symbol.bkcolorkod = newcolorkod; 
									}
									break;
                                //boundary symbol
								case 4:
									{
										MRangeSym* rangesym = (MRangeSym*)prosl;
										if(rangesym->m_symbol.colorkod == oldcolorkod)
											rangesym->m_symbol.colorkod = newcolorkod;
										if(rangesym->m_symbol.bkcolorkod == oldcolorkod)
											rangesym->m_symbol.bkcolorkod = newcolorkod; 
									}
									break;
								}
							}

                            //fillings of areal objects
							for(int ii=0; ii<ployaz->m_plast->GetSize(); ii++){
								MPlast* plast = ployaz->m_plast->GetAt(ii);
								int typeplast = plast->GetType();
								switch(typeplast)
								{
                                //shading
								case 1:
									{
										MShtrihovka* shtrihovka = (MShtrihovka*)plast;
										if(shtrihovka->m_colorkod == oldcolorkod)
											shtrihovka->m_colorkod = newcolorkod;
									}
									break;
                                //filling
								case 2:
									{
										MZalivka* zalivka = (MZalivka*)plast;
										if(zalivka->m_colorkod == oldcolorkod)
											zalivka->m_colorkod = newcolorkod;
									}
									break;
                                //pattern
								case 3:
									{
										MUzor* uzor = (MUzor*)plast;
										if(uzor->m_symbol.colorkod == oldcolorkod)
											uzor->m_symbol.colorkod = newcolorkod;
										if(uzor->m_symbol.bkcolorkod == oldcolorkod)
											uzor->m_symbol.bkcolorkod = newcolorkod;
									}
									break;
                                //pattern
								case 11:
									{
										MPattern *pattern = (MPattern*)plast;
										if(pattern->m_colorkod == oldcolorkod)
											pattern->m_colorkod = newcolorkod;
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
										if(linia->m_colorkod == oldcolorkod)
										linia->m_colorkod = newcolorkod;
									}
									break;
                                //shaped line
								case 1:
									{
										MShtrih* shtrih = (MShtrih*)prosl;
										if(shtrih->m_colorkod == oldcolorkod)
										shtrih->m_colorkod = newcolorkod;
									}
									break;
                                // dotted line
								case 2:
									{
										MPunktir* punktir = (MPunktir*)prosl;
										if(punktir->m_colorkod == oldcolorkod)
										punktir->m_colorkod = newcolorkod;
									}
									break;
                                //symbol broach
								case 3:
									{
										MSbStretch* sbstretch = (MSbStretch*)prosl;
										if(sbstretch->m_symbol.colorkod == oldcolorkod)
											sbstretch->m_symbol.colorkod = newcolorkod;
										if(sbstretch->m_symbol.bkcolorkod == oldcolorkod)
											sbstretch->m_symbol.bkcolorkod = newcolorkod; 
									}
									break;
                                //boundary symbol
								case 4:
									{
										MRangeSym* rangesym = (MRangeSym*)prosl;
										if(rangesym->m_symbol.colorkod == oldcolorkod)
											rangesym->m_symbol.colorkod = newcolorkod;
										if(rangesym->m_symbol.bkcolorkod == oldcolorkod)
											rangesym->m_symbol.bkcolorkod = newcolorkod; 
									}
									break;
                                }//end switch(typeprosl)
                            }//end for(int i=0; i<linyaz->m_linia->GetSize(); i++)
						}
						break;
                    //dot objects
					case F_DOT:
						{
							MDotYaz* dotyaz = (MDotYaz*)pYaz->m_object;
							for(int i=0; i<dotyaz->m_tochka->GetSize(); i++){
								MSrez* srez = dotyaz->m_tochka->GetAt(i);
								int typesrez = srez->GetType();
								switch(typesrez)
								{
                                //font symbol
								case 0:
									{
										MSymbol* symbol = &((MShriftSrez*)srez)->m_tochka;
										if(symbol->colorkod == oldcolorkod)
											symbol->colorkod = newcolorkod;
										if(symbol->bkcolorkod == oldcolorkod)
											symbol->bkcolorkod = newcolorkod;
									}
									break;

                                //any symbol
								case 1:
									{
										MProizvolSrez *proizSrez = (MProizvolSrez*)srez;
										if(proizSrez->m_colorkod == oldcolorkod)
											proizSrez->m_colorkod = newcolorkod;
									}
									break;
								}
                            }//end for(int i=0; i<dotyaz->m_linia->GetSize(); i++)
						}
						break;
                    }//end switch (type)
                }//the end on languages of the array of languages of a sign
			}
		}
	}

	return true;
}


//==================================
//!replacement of handwriting by new handwriting
bool MKlassifikator::ChangePocherk(unsigned char oldprkkod,unsigned char newprkkod)
//==================================
{
	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
                //on all languages of the array of languages of a sign
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
				for(int kk=0; kk<pYazArray->GetSize(); kk++){
					pYaz = znak->GetYaz(kk);
                    //replacement of handwriting in formulas
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
									if(prkkod == oldprkkod)
										strofa->SetNomPrk(t,newprkkod);
								}
								strofa = fraction->GetZnam();
								for(t=0; t<strofa->GetPredlogenieNum(); t++){
									prkkod = strofa->GetNomPrk(t);
									if(prkkod == oldprkkod)
										strofa->SetNomPrk(t,newprkkod);
								}
							}
							if(type==ID_FRAG_STROFA){
								MStrofa* strofa = (MStrofa*)fragment->GetFragment();
								for(int t=0; t<strofa->GetPredlogenieNum(); t++){
									prkkod = strofa->GetNomPrk(t);
									if(prkkod == oldprkkod)
										strofa->SetNomPrk(t,newprkkod);
								}
							}
                        }//end for(int s=0; s<frar->GetSize(); s++)
                    }//end for(l=0; l<formuli->GetSize(); l++)
                }//the end on languages of the array of languages of a sign
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)
    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	return true;
}

//==================================
//!replacement of a boundary line
bool MKlassifikator::ChangeMegaStroka(unsigned char oldmegastrkod, unsigned char newmegastrkod)
//==================================
{
	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
                //on all languages of the array of languages of a sign
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
				for(int s=0; s<pYazArray->GetSize(); s++){
					pYaz = znak->GetYaz(s);
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
									if(megastrkod == oldmegastrkod)
										strofa->SetNomPrefix(t,newmegastrkod);
									megastrkod = strofa->GetNomPostfix(t);
									if(megastrkod == oldmegastrkod)
										strofa->SetNomPostfix(t,newmegastrkod);
								}
								strofa = fraction->GetZnam();
								for(t=0; t<strofa->GetPredlogenieNum(); t++){
									megastrkod = strofa->GetNomPrefix(t);
									if(megastrkod == oldmegastrkod)
										strofa->SetNomPrefix(t,newmegastrkod);
									megastrkod = strofa->GetNomPostfix(t);
									if(megastrkod == oldmegastrkod)
										strofa->SetNomPostfix(t,newmegastrkod);
								}
							}
							if(type==ID_FRAG_STROFA){
								MStrofa* strofa = (MStrofa*)fragment->GetFragment();
								for(int t=0; t<strofa->GetPredlogenieNum(); t++){
									megastrkod = strofa->GetNomPrefix(t);
									if(megastrkod == oldmegastrkod)
										strofa->SetNomPrefix(t,newmegastrkod);
									megastrkod = strofa->GetNomPostfix(t);
									if(megastrkod == oldmegastrkod)
										strofa->SetNomPostfix(t,newmegastrkod);
								}
							}
                        }//end for(int s=0; s<frar->GetSize(); s++)
                    }//end for(l=0; l<formuli->GetSize(); l++)
                }//the end on languages of the array of languages of a sign
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)
    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	return true;
}

//==================================
//!replacement of the characteristic
bool MKlassifikator::ChangeCharakteristika(unsigned short oldcharkod, unsigned short newcharkod)
//==================================
{
	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
                //we change a code of the characteristic in the array of admissible codes
				MShInArray* charkods = znak->GetNomsArray();
				unsigned short nom;
				int l = 0;
				for(l=0; l<charkods->GetSize(); l++){
					nom = charkods->GetAt(l)[0];
					if(nom == oldcharkod)
						charkods->SetAt(l,&newcharkod);
				}
                //on all languages of the array of languages of a sign
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
                //Also replacement of attributes in formulas corresponding is required
				for(l=0; l<pYazArray->GetSize(); l++){
					pYaz = znak->GetYaz(l);
					MFormArray* formuli = pYaz->m_formula;
					for(int ii = formuli->GetSize()-1;ii>=0;ii--){
						MFrArray* fragmenti = formuli->GetAt(ii)->fragm_arr;
						for(int kk = fragmenti->GetSize()-1;kk>=0;kk--){
							MFragment* pFrag = fragmenti->GetAt(kk);
							switch(pFrag->GetType()){
							case ID_FRAG_STROFA:
								{
									MStrofa* pStrofa = (MStrofa*)pFrag->GetFragment();
									for(int ll = pStrofa->GetPredlogenieNum()-1;ll>=0;ll--){
										if(pStrofa->GetNomPole(ll)==oldcharkod)
											pStrofa->SetNomPole(ll,newcharkod);
									}

								}break;
							case ID_FRAG_FRACTION:
								{
									MFraction* pFraction = (MFraction*)pFrag->GetFragment();

									MStrofa* pStrofa = pFraction->GetChisl();	

									int ll = 0;
									for(ll = pStrofa->GetPredlogenieNum()-1;ll>=0;ll--){
										if(pStrofa->GetNomPole(ll)==oldcharkod)
											pStrofa->SetNomPole(ll,newcharkod);
									}
									pStrofa = pFraction->GetZnam();
									for(ll = pStrofa->GetPredlogenieNum()-1;ll>=0;ll--){
										if(pStrofa->GetNomPole(ll)==oldcharkod)
											pStrofa->SetNomPole(ll,newcharkod);
									}
								}break;
							}
                        }//the end by fragments
                    }//the end by formulas
                }//the end on languages in the array of languages
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)
    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	return true;
}

//==================================
//!alignment of all dot font signs on the center
bool MKlassifikator::DotShriftZnakCenterAlign()
//==================================
{
	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
				for(int s=0; s<pYazArray->GetSize(); s++){
					pYaz = znak->GetYaz(s);
					if(pYaz->GetType()==F_DOT){
						MSzArray* szar = ((MDotYaz*)pYaz->m_object)->m_tochka;
						for(int kk=0; kk<szar->GetSize(); kk++){
							MSrez* pSrez = szar->GetAt(kk);
							if(pSrez->GetType()==IDT_SHRIFT){
								MSymbol* sym = &((MShriftSrez*)pSrez)->m_tochka;
								sym->align = UT_TA_CENTER|UT_TA_BASELINE;
							}
						}
					}
				}
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)
    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	return true;
}

//==================================
//!on all objects of the classifier looks for codes of handwritings,
//! absent in the table of handwritings
bool MKlassifikator::GetUnrealPrkKods(MShInArray* prkkodar)
//==================================
{
	prkkodar->ClearAll();

	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
				for(int s=0; s<pYazArray->GetSize(); s++){
					pYaz = znak->GetYaz(s);
                    //we look for invalid codes of handwritings in stanzas of formulas
					MFormArray* formuli = pYaz->m_formula;
					for(int l=0; l<formuli->GetSize(); l++){
						MFrArray* frar = formuli->GetAt(l)->fragm_arr;
						for(int s=0; s<frar->GetSize(); s++){
							MFragment* fragment = frar->GetAt(s);
							unsigned char type = fragment->GetType();
							unsigned short prkkod;
							MPrk prk;
							if(type==ID_FRAG_FRACTION){
								MFraction* fraction = (MFraction*)fragment->GetFragment();
								MStrofa* strofa = fraction->GetChisl();
								int t = 0;
								for(t=0; t<strofa->GetPredlogenieNum(); t++){
									prkkod = (unsigned short)strofa->GetNomPrk(t);
									if(!GetDictPrkValue(prkkod,prk)){
										if(prkkodar->Find(0,&prkkod)==-1)
											prkkodar->AddTail(&prkkod);
									}
								}
								strofa = fraction->GetZnam();
								for(t=0; t<strofa->GetPredlogenieNum(); t++){
									prkkod = (unsigned short)strofa->GetNomPrk(t);
									if(!GetDictPrkValue(prkkod,prk)){
										if(prkkodar->Find(0,&prkkod)==-1)
											prkkodar->AddTail(&prkkod);
									}
								}
							}
							if(type==ID_FRAG_STROFA){
								MStrofa* strofa = (MStrofa*)fragment->GetFragment();
								for(int t=0; t<strofa->GetPredlogenieNum(); t++){
									prkkod = (unsigned short)strofa->GetNomPrk(t);
									if(!GetDictPrkValue(prkkod,prk)){
										if(prkkodar->Find(0,&prkkod)==-1)
											prkkodar->AddTail(&prkkod);
									}
								}
							}
                        }//end for(int s=0; s<frar->GetSize(); s++)
                    }//end for(l=0; l<formuli->GetSize(); l++)
                }//the end on languages in the array of languages of a sign
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)

    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	if(prkkodar->GetSize()==0)
		return false;

	return true;
}

//==================================
//!on all objects of the classifier looks for codes of boundary lines,
//! absent in the table of boundary lines
bool MKlassifikator::GetUnrealMegstrKods(MShInArray* megstrkodar)
//==================================
{
	megstrkodar->ClearAll();

	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
				MYzArray* pYazArray = znak->GetYazArray();
				if(!pYazArray) continue;
				MYaz* pYaz;
				for(int s=0; s<pYazArray->GetSize(); s++){
					pYaz = znak->GetYaz(s);
                    //we look for invalid codes of boundary lines in stanzas of formulas
					MFormArray* formuli = pYaz->m_formula;
					for(int l=0; l<formuli->GetSize(); l++){
						MFrArray* frar = formuli->GetAt(l)->fragm_arr;
						for(int s=0; s<frar->GetSize(); s++){
							MFragment* fragment = frar->GetAt(s);
							unsigned char type = fragment->GetType();
							unsigned short megstrkod;
							MString str;
							if(type==ID_FRAG_FRACTION){
								MFraction* fraction = (MFraction*)fragment->GetFragment();
								MStrofa* strofa = fraction->GetChisl();
								int t = 0;
								for(t=0; t<strofa->GetPredlogenieNum(); t++){
									megstrkod = (unsigned short)strofa->GetNomPrefix(t);
									if(!GetDictMegstrValue(megstrkod,str)){
										if(megstrkodar->Find(0,&megstrkod)==-1)
											megstrkodar->AddTail(&megstrkod);
									}
									megstrkod = (unsigned short)strofa->GetNomPostfix(t);
									if(!GetDictMegstrValue(megstrkod,str)){
										if(megstrkodar->Find(0,&megstrkod)==-1)
											megstrkodar->AddTail(&megstrkod);
									}
								}
								strofa = fraction->GetZnam();
								for(t=0; t<strofa->GetPredlogenieNum(); t++){
									megstrkod = (unsigned short)strofa->GetNomPrefix(t);
									if(!GetDictMegstrValue(megstrkod,str)){
										if(megstrkodar->Find(0,&megstrkod)==-1)
											megstrkodar->AddTail(&megstrkod);
									}
									megstrkod = (unsigned short)strofa->GetNomPostfix(t);
									if(!GetDictMegstrValue(megstrkod,str)){
										if(megstrkodar->Find(0,&megstrkod)==-1)
											megstrkodar->AddTail(&megstrkod);
									}
								}
							}
							if(type==ID_FRAG_STROFA){
								MStrofa* strofa = (MStrofa*)fragment->GetFragment();
								for(int t=0; t<strofa->GetPredlogenieNum(); t++){
									megstrkod = (unsigned short)strofa->GetNomPrefix(t);
									if(!GetDictMegstrValue(megstrkod,str)){
										if(megstrkodar->Find(0,&megstrkod)==-1)
											megstrkodar->AddTail(&megstrkod);
									}
									megstrkod = (unsigned short)strofa->GetNomPostfix(t);
									if(!GetDictMegstrValue(megstrkod,str)){
										if(megstrkodar->Find(0,&megstrkod)==-1)
											megstrkodar->AddTail(&megstrkod);
									}
								}
							}
                        }//end for(int s=0; s<frar->GetSize(); s++)
                    }//end for(l=0; l<formuli->GetSize(); l++)
                }//the end on languages of the array of languages of a sign
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)
    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	if(megstrkodar->GetSize()==0)
		return false;

	return true;
}

//==================================
//!on all objects of the classifier looks for codes of characteristics,
//! absent in the table of characteristics
bool MKlassifikator::GetUnrealAttrKods(MShInArray* attrkodar)
//==================================
{
	attrkodar->ClearAll();

	MZsArray* znsistemi = GetZsArray();
	for(int i=0; i<znsistemi->GetSize(); i++){
		MZnakSistema* zs = znsistemi->GetAt(i);
		MRlArray* razdeli = zs->GetRazdelArray();
		for(int j=0; j<razdeli->GetSize(); j++){
			MRazdel* razdel = razdeli->GetAt(j);
			MZnArray* znaki = razdel->GetZnakArray();
			for(int k=0; k<znaki->GetSize(); k++){
				MZnak* znak = znaki->GetAt(k);
                //we look for invalid codes of characteristics in the array of admissible codes
				MShInArray* charkods = znak->GetNomsArray();
				unsigned short charkod;
				MString str;
				for(int l=0; l<charkods->GetSize(); l++){
					charkod = charkods->GetAt(l)[0];
					if(!GetDictAttrName(charkod,str)){
						if(attrkodar->Find(0,&charkod)==-1)
							attrkodar->AddTail(&charkod);
					}
				}
            }//end for(int k=0; k<znaki->GetSize(); k++)
        }//end for(int j=0; j<razdeli->GetSize(); j++)
    }//end for(int i=0; i<znsistemi->GetSize(); i++)

	if(attrkodar->GetSize()==0)
		return false;

	return true;
}



//================================================================================
//!Addition of one classifier to the current
/**The main complexity that at connection of tables shoveling is made
//second classifier
*/
bool MKlassifikator::Append(MKlassifikator *pKls)
//================================================================================
{
    //Before addition actual sign systems to the classifier
    //at first connection of tables and then shoveling in connected is required
    //the code classifier with initial on the received

    //At first table of colours
	int iKey;
	MVoidArray pva;
	MTable* pTable = pKls->GetColorTable();
	MIntSize dim = pTable->GetDimension();
	int ikeyField = pTable->GetKeyField();
	void* pvTemp;
	bool bQuit = false;
	MCodeChange ChangeRecords;
	if(pTable->GetFirstKey(iKey))do{
        //We look for compliance in the current table
		pva.SetSize(0);
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField){
				pvTemp = pTable->GetCell(iKey,i);
				pva.AddTail(&pvTemp);
			}
		int iNewKey = GetColorTable()->AddOrGetUsed(&pva);
        //Here replacement actual
		if(iNewKey!=-1){
            //Entry of replacement of color of handwriting in the dictionary of handwritings
			ChangeRecords.AddRecord(iKey,iNewKey);
		}
		else{
			InternalMessage(IDS_TABLE_ERROR1);//"Таблица цветов переполена присоединение классификаторов некорректно"
			bQuit = true;
		}
	}while((pTable->GetNextKey(iKey,iKey))&&(!bQuit)&&(((MFixedSizeTable*)pTable)->GetFilledNum()!=iKey-1));
	ChangeRecords.ApplyChangeRecords(pKls,ID_COLOR_TABLE);

    //Now table of handwritings
	pTable = pKls->GetPrkTable();
	dim = pTable->GetDimension();
	ikeyField = pTable->GetKeyField();
	bQuit = false;
	if(pTable->GetFirstKey(iKey))do{
        //We look for compliance in the current table
		pva.SetSize(0);
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField){
				pvTemp = pTable->GetCell(iKey,i);
				pva.AddTail(&pvTemp);
			}
		int iNewKey = GetPrkTable()->AddOrGetUsed(&pva);
        //Here replacement actual
		if(iNewKey!=-1)
			ChangeRecords.AddRecord(iKey,iNewKey);
		else{
			InternalMessage(IDS_TABLE_ERROR2);//"Таблица почерков переполена присоединение классификаторов некорректно"
			bQuit = true;
		}
	}while((pTable->GetNextKey(iKey,iKey))&&(!bQuit));
	ChangeRecords.ApplyChangeRecords(pKls,ID_PRK_TABLE);

    //Table of suffix&preffix strings
	pTable = pKls->GetMegStrTable();
	dim = pTable->GetDimension();
	ikeyField = pTable->GetKeyField();
	bQuit = false;
	if(pTable->GetFirstKey(iKey))do{
        //We look for compliance in the current table
		pva.SetSize(0);
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField){
				pvTemp = pTable->GetCell(iKey,i);
				pva.AddTail(&pvTemp);
			}
		int iNewKey = GetMegStrTable()->AddOrGetUsed(&pva);
        //Here replacement actual
        if(iNewKey!=-1)
			ChangeRecords.AddRecord(iKey,iNewKey);
		else{
			InternalMessage(IDS_TABLE_ERROR3);//"Таблица межевых строчек переполена присоединение классификаторов некорректно"
			bQuit = true;
		}
	}while((pTable->GetNextKey(iKey,iKey))&&(!bQuit));
	ChangeRecords.ApplyChangeRecords(pKls,ID_MEGSTR_TABLE);

    //Table of attributes
	pTable = pKls->GetAttrTable();
	dim = pTable->GetDimension();
	ikeyField = pTable->GetKeyField();
	bQuit = false;
	if(pTable->GetFirstKey(iKey))do{
        //We look for compliance in the current table
        pva.SetSize(0);
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField){
				pvTemp = pTable->GetCell(iKey,i);
				pva.AddTail(&pvTemp);
			}
		int iNewKey = GetAttrTable()->AddOrGetUsed(&pva);
        //Here replacement actual
        if(iNewKey!=-1)
			ChangeRecords.AddRecord(iKey,iNewKey);
		else{
			InternalMessage(IDS_TABLE_ERROR4);//"Таблица характеристик переполена присоединение классификаторов некорректно"
			bQuit = true;
		}
	}while((pTable->GetNextKey(iKey,iKey))&&(!bQuit));
	ChangeRecords.ApplyChangeRecords(pKls,ID_ATTR_TABLE);

/*
    //Table of fonts
	pTable = pKls->GetFontTable();
	dim = pTable->GetDimension();
	ikeyField = pTable->GetKeyField();
	bQuit = false;
	if(pTable->GetFirstKey(iKey))do{
        //We look for compliance in the current table
        pva.SetSize(0);
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField){
				pvTemp = pTable->GetCell(iKey,i);
				pva.AddTail(&pvTemp);
			}
		int iNewKey = GetFontTable()->AddOrGetUsed(&pva);
        //Here replacement actual
        if(iNewKey!=-1)
			pKls->ChangeFont((USHORT)iKey,(USHORT)iNewKey);
		else{
			InternalMessage("Таблица шрифтов переполена присоединение классификаторов некорректно");
			bQuit = true;
		}
	}while((pTable->GetNextKey(iKey,iKey))&&(!bQuit));
*/

/*
    //table of units of measure
	pTable = pKls->GetEdIzmTable();
	dim = pTable->GetDimension();
	ikeyField = pTable->GetKeyField();
	bQuit = false;
	if(pTable->GetFirstKey(iKey))do{
        //We look for compliance in the current table
        pva.SetSize(0);
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField){
				pvTemp = pTable->GetCell(iKey,i);
				pva.AddTail(&pvTemp);
			}
		int iNewKey = GetEdIzmTable()->AddOrGetUsed(&pva);
        //Here replacement actual
        if(iNewKey!=-1)
			pKls->ChangeEdIzm((USHORT)iKey,(USHORT)iNewKey);
		else{
			InternalMessage("Таблица единиц измерения переполена присоединение классификаторов некорректно");
			bQuit = true;
		}
	}while((pTable->GetNextKey(iKey,iKey))&&(!bQuit));
*/
    //Now new tables all are made
    //and codes of the connected classifier are transferred to codes of that table
    //which contains in the current classifier
	((MZsArray *)GetZsArray())->AppendTail(pKls->GetZsArray());
	return true;
}



// ==================================================================
// * Function parameters : 
// [WhatTheTable] - table type for minimization
//there can be ID_PRK_TABLE or ID_COLOR_TABLE
//! Function makes minimization of tables
void MKlassifikator::MinimizeTable(TableToChangeKod WhatTheTable)
// ==================================================================
{
	int iKey;
	MVoidArray pva;
	MTable* pTable;
	MIntSize dim;
	switch(WhatTheTable){
	case ID_COLOR_TABLE:
		{
            //Table of colors
            pTable = GetColorTable();
			dim = pTable->GetDimension();
			dim.cx = ((MFixedSizeTable*)pTable)->GetFilledNum();
			int ikeyField = pTable->GetKeyField();
			void* pvTemp;
			MCodeChange ChangeRecords;
			if(pTable->GetFirstKey(iKey))do{
                //We look for compliance in the current table
                pva.SetSize(0);
				for(int i = 0;i<dim.cy;i++)
					if(i!=ikeyField){
						pvTemp = pTable->GetCell(iKey,i);
						pva.AddTail(&pvTemp);
					}
					int iNewKey = GetColorTable()->UseRecord(&pva,KLS_TABLE_COLOR_NAME_INDEX);
                    //Here replacement actual
                    if((iNewKey!=-1)&&(iNewKey!=iKey))
						ChangeRecords.AddRecord(iKey,iNewKey);
			}while((pTable->GetNextKey(iKey,iKey))&&(iKey<dim.cx));
            //Removal of duplicating entries in the table

			ChangeRecords.ApplyAndRemove(this,ID_COLOR_TABLE);
		}break;
	case ID_PRK_TABLE:
		{
            //Table of colors
            pTable = GetPrkTable();
			dim = pTable->GetDimension();
			int ikeyField = pTable->GetKeyField();
			void* pvTemp;
			MCodeChange ChangeRecords;
			if(pTable->GetFirstKey(iKey))do{
                //We look for compliance in the current table
                pva.SetSize(0);
				for(int i = 0;i<dim.cy;i++)
					if(i!=ikeyField){
						pvTemp = pTable->GetCell(iKey,i);
						pva.AddTail(&pvTemp);
					}
					int iNewKey = GetPrkTable()->UseRecord(&pva,KLS_TABLE_PRK_NAME_INDEX);
                    //Here replacement actual
                    if((iNewKey!=-1)&&(iNewKey!=iKey))
						ChangeRecords.AddRecord(iKey,iNewKey);
			}while(pTable->GetNextKey(iKey,iKey));
			ChangeRecords.ApplyAndRemove(this,ID_PRK_TABLE);
		}break;
    	default:	break; //	exit(15);
	};
}

// ==================================================================
//!installation of the filter of visibility of signs
void MKlassifikator::SetVisibleFilter(MString filename)
// ==================================================================
{
	//SetAllZnakUnvisible();
	
	if(filename == "")
	{
		SetAllZnakVisible();
		return;
	}

	ifstream in;
	in.open(filename);
	if(!in.is_open()) return;

	SetAllZnakUnvisible();
	MString str;
	char st[30];
	MString truekkod;
	MZnak* znak;
	while(!in.eof())
	{
		in>>st;
		str = st;
		str.Replace(":"," ");

		znak = GetZnak(str, truekkod, true);
		if(znak)
			znak->GetYaz()->m_visible = true;
	}
	
	in.close();
}

// ==================================================================
//!installation of all signs the invisible
void MKlassifikator::SetAllZnakUnvisible()
// ==================================================================
{
    for(int i=0;i<GetZsArray()->GetSize();i++)
    {
//		MZnakSistema* pSys = GetZsArray()->GetAt(i);
		for(int j = 0; j<GetZsArray()->GetAt(i)->GetRazdelArray()->GetSize(); j++)
		{
			for(int k = 0; k<GetZsArray()->GetAt(i)->GetRazdelArray()->GetAt(j)->GetZnakArray()->GetSize(); k++)
				GetZsArray()->GetAt(i)->GetRazdelArray()->GetAt(j)->GetZnakArray()->GetAt(k)->GetYaz()->m_visible = false;
		}
    }	
}

// ==================================================================
//!installation of all signs the visible
void MKlassifikator::SetAllZnakVisible()
// ==================================================================
{
    for(int i=0;i<GetZsArray()->GetSize();i++)
    {
//		MZnakSistema* pSys = GetZsArray()->GetAt(i);
		for(int j = 0; j<GetZsArray()->GetAt(i)->GetRazdelArray()->GetSize(); j++)
		{
			for(int k = 0; k<GetZsArray()->GetAt(i)->GetRazdelArray()->GetAt(j)->GetZnakArray()->GetSize(); k++)
				GetZsArray()->GetAt(i)->GetRazdelArray()->GetAt(j)->GetZnakArray()->GetAt(k)->GetYaz()->m_visible = true;
		}
    }	
}
// ==================================================================
//!recalculate all m_fMasstKoeff
void MKlassifikator::RotateAllZsSystemCoeffs()
    {
        for(int i=0;i<GetZsArray()->GetSize();i++)
        {

            MZnakSistema* pSys = GetZsArray()->GetAt(i);
            if (pSys)
            {
                pSys->SetMasstKoeff(1/pSys->GetMasstKoeff());
            }

        }

    }
