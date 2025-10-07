#include "Formula.h"
#include "BaseFun.h"
#include "Refglobal.h"
//======================================================================================
// Realization of basic classes for formulas
//=========================================================================================
//=============================================================
// MStrofa
//=============================================================
//! Constructor by default
//! all parameters are set conditionally; parameters depend on the scale of the map, type...
MStrofa::MStrofa()
{
	m_typeStrofa = ID_STROFA_STROKA;
	m_pPgArray = new MPgArray();
	m_pPgArray->SetSize(1);
	MPredlogenie* predlog = new MPredlogenie();
	m_pPgArray->SetAt(0, predlog);
	delete predlog;
}

//!==================================
MStrofa::MStrofa(unsigned char type_str)
{
	m_typeStrofa = type_str;
	m_pPgArray = new MPgArray();

	m_pPgArray->SetSize(1);
	MPredlogenie* predlog = new MPredlogenie();
	m_pPgArray->SetAt(0, predlog);
	delete predlog;
}

//!==================================
MStrofa::MStrofa(unsigned char type_str, MPredlogenie* predlog)
{
	m_typeStrofa = type_str;
	m_pPgArray = new MPgArray();
	m_pPgArray->SetSize(1);
	m_pPgArray->SetAt(0, predlog);
}

//==================================
//! copy function
void MStrofa::Copy(MStrofa* strofa)
{
	m_typeStrofa = strofa->m_typeStrofa;
	m_pPgArray->ClearAll();
	m_pPgArray->AppendTail(strofa->m_pPgArray);
}

//==================================
//!The redefined operator '='
MStrofa& MStrofa::operator=(const MStrofa& in)
{
	if(&in!=this){
		Copy((MStrofa*)&in);
	}
	return *this;
}

//==================================
//!sizing
int MStrofa::GetSize()
{   
	int Size;
    Size = sizeof(unsigned char);//stanza type
	Size+=m_pPgArray->GetBytes();
	return Size;	
}

//===================================================================
//!Writing in the UTP-file
bool MStrofa::WriteBin(FILE* h_nuxfile, int version)
{
    //We determine the stanza size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //We write down the stanza size
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write down the stanza type
		fwrite(&m_typeStrofa, sizeof(m_typeStrofa), 1, h_nuxfile);
        //write down an array of sentences
		m_pPgArray->WriteBin(h_nuxfile,version);
	}
	return true;
}

//===================================================================================
//!Reading from the NUX-file
bool MStrofa::ReadBin(FILE* h_nuxfile, int version)
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE=0;
            //we read record length
			fread(&SIZE, 4, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
            //read type
			fread(&m_typeStrofa, 1, 1, h_nuxfile);
			m_pPgArray->ReadBin(h_nuxfile,version);
	}
	return true;
}

//===================================================================================
//!Reading from the NUX-file
bool MStrofa::ReadBin(MFile* file, int version)
{
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE=0;
            //we read record length
            file->GetData(&SIZE, 4, 1);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
            //read type
            file->GetData(&m_typeStrofa, 1, 1);
			m_pPgArray->ReadBin(file,version);
	}
	return true;
}

//!===================================================================================
void MStrofa::SetType(unsigned char typenew)
{
    //If the array of sentences exceeds the admissible
    //the size for this type,it is cleared
	if(m_typeStrofa==typenew)
		return;
    //only the difficult stanza may contain more than one sentence, therefore, if
    //we change type to a difficult stanza for another, we delete all sentences, except the initial
	if(m_typeStrofa==ID_STROFA_COMPLEX){
		int Size = m_pPgArray->GetSize();
        //Removal of a tail of arrays
		for (int i=Size-1;i>=1;i--){
			m_pPgArray->Delete(i);
		}
	}
	m_typeStrofa=typenew;
}

//============================================================
//!Comparison of stanzas
bool MStrofa::IsEqualTo(MStrofa* pStrofaToCompare)
{
	if((m_typeStrofa==pStrofaToCompare->m_typeStrofa)&&
		(m_pPgArray->IsEqualTo(pStrofaToCompare->m_pPgArray)))
		return true;
	return false;
}


//!==================================
unsigned char MStrofa::GetType()
{   
	unsigned char Type;
	Type = m_typeStrofa; 
	return Type;	
}

//==================================
//!Destructor
MStrofa::~MStrofa()
{ 
	m_pPgArray->ClearAll();
	delete m_pPgArray;
}


//=================================================================
//!Addition of the sentence in a stanza
bool MStrofa::AddPredlogenie(MPredlogenie* predlog)
{
    //if index zero
	if(!predlog) return false;
    //in all types of stanzas, except difficult, one sentence is always stored,
    //therefore it is impossible to add more sentences
	if(m_typeStrofa!=ID_STROFA_COMPLEX)
		return false;
	m_pPgArray->AddTail(predlog);
	return true;
}

//============================================================
//!Removal of the sentence with the set index
bool MStrofa::DeletePredlogenie(int index)
{
    //in a stanza always there has to be, at least, one sentence
	if((m_typeStrofa!=ID_STROFA_COMPLEX)||(index<=0))
		return false;
	m_pPgArray->Delete(index);
	return true;
}

//===============================================================
//!Establishes these sentences with this index
bool MStrofa::SetPredlogenie(int index,MPredlogenie* predlog)
{
	if(!predlog)
		return false;
    //only the difficult stanza may contain more than one sentence
	if((index>0)&&(m_typeStrofa!= ID_STROFA_COMPLEX))
		return false;
    //if the index exceeds an index of the last element of the array of sentences
	if(index>=m_pPgArray->GetSize()){
		return false;
	}
	m_pPgArray->SetAt(index,predlog);
	return true;
}


//==================================================================
//!Returns number of sentences in a stanza
int MStrofa::GetPredlogenieNum()
{
	return m_pPgArray->GetSize();
}

//==========================================================
//!returns a dictionary index (code) of a prefix of the sentence with number 'index'
unsigned char MStrofa::GetNomPrefix(int index)
{
	return m_pPgArray->GetAt(index)->GetNomPrefix();
}

//==========================================================
//!returns a dictionary index (code) of a postfix of the sentence with number 'index'
unsigned char MStrofa::GetNomPostfix(int index)
{
	return m_pPgArray->GetAt(index)->GetNomPostfix();
}

//==========================================================
//!returns a dictionary index (code) of the name of the characteristic of the sentence with number 'index'
unsigned short MStrofa::GetNomPole(int index)
{
	return m_pPgArray->GetAt(index)->GetNomPole();
}

//==============================================================
//!returns a dictionary index (code) of a style of the sentence with number 'index'
unsigned char MStrofa::GetNomPrk(int index)
{
	return m_pPgArray->GetAt(index)->GetNomPrk();
}

//=====================================================
//!establishes a dictionary index (code) of the name of the characteristic of the sentence with number 'index'
void MStrofa::SetNomPole(int index, unsigned short nompolename)
{
	m_pPgArray->GetAt(index)->SetNomPole(nompolename);
}

//=====================================================
//!establishes a dictionary index (code) of a prefix of the sentence with number 'index'
void MStrofa::SetNomPrefix(int index, unsigned char nomprefix)
{
	m_pPgArray->GetAt(index)->SetNomPrefix(nomprefix);
}

//=====================================================
//!establishes a dictionary index (code) of a postfix of the sentence with number 'index'
void MStrofa::SetNomPostfix(int index, unsigned char nompostfix)
{
	m_pPgArray->GetAt(index)->SetNomPostfix(nompostfix);
}

//=====================================================
//!establishes a dictionary index (code) of a style of the sentence with number 'index'
void MStrofa::SetNomPrk(int index, unsigned char nomprk)
{
	m_pPgArray->GetAt(index)->SetNomPrk(nomprk);
}



//=============================================================
// MFraction
//=============================================================

//==================================
//! Constructor by default
//! all parameters are set conditionally; parameters depend on the scale of the map, type...
MFraction::MFraction()
//==================================	
{
}

//!==================================
MFraction::MFraction(MStrofa*  chisl, MStrofa* znam)
{
	m_chisl=*chisl;
	m_znam=*znam;
}

//!==================================
//MFraction::~MFraction()
//{
//}

//==================================
//! copy function
void MFraction::Copy(MFraction* fraction)
{
	m_chisl=fraction->m_chisl;
	m_znam=fraction->m_znam;
}
//==================================
//!sizing
int MFraction::GetSize()
{   
	int Size;
	Size = m_chisl.GetSize() + m_znam.GetSize();//размер 
	return Size;	
}

//!==================================
MStrofa* MFraction::GetChisl()
{   
	return &m_chisl;	
}

//!==================================
MStrofa* MFraction::GetZnam()
{   
	return &m_znam;
}



//===================================================================
//!Writing in UTP-file
bool MFraction::WriteBin(FILE* h_nuxfile, int version)
{
    //determine the stanza size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //writing the stanza size
		fwrite(&Size, 4, 1, h_nuxfile);
        //writing
		m_chisl.WriteBin(h_nuxfile, version);
		m_znam.WriteBin(h_nuxfile, version);
		
	}
	return true;
}

//===================================================================================
//!Reading fron NUX-file
bool MFraction::ReadBin(FILE* h_nuxfile, int version)
{
	//Free();
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE= 0;
            //we read record length
			fread(&SIZE, 4, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
            //read
			m_chisl.ReadBin(h_nuxfile, version);
			m_znam.ReadBin(h_nuxfile, version);
	}
	return true;
}

//===================================================================================
//!Reading fron NUX-file
bool MFraction::ReadBin(MFile* file, int version)
{
	//Free();
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE= 0;
            //we read record length
            file->GetData(&SIZE, 4, 1);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
            //read
			m_chisl.ReadBin(file, version);
			m_znam.ReadBin(file, version);
	}
	return true;
}

//==============================================================
//!Comparison of fractions
bool MFraction::IsEqualTo(MFraction *m_pFractionToCompare)
{
	if(m_chisl.IsEqualTo(&m_pFractionToCompare->m_chisl))
		return m_znam.IsEqualTo(&m_pFractionToCompare->m_znam);
	return false;
}



//=============================================================
// MFragment
//=============================================================

//==================================
//! Constructor by default
//! all parameters are set conditionally; parameters depend on the scale of the map, type...
MFragment::MFragment()
{
	type_fragm = ID_FRAG_STROFA;
	m_fragm = (void*) new MStrofa;
}

//!==================================
MFragment::MFragment(unsigned char type_str)
{
	type_fragm = type_str;
	switch (type_fragm) {
	case ID_FRAG_STROFA:
		{
			m_fragm = (void*) new MStrofa();		
		}
		break;
	case ID_FRAG_FRACTION:
		{
			m_fragm = (void*) new MFraction();
		}
		break;
	}
}

//!==================================
MFragment::MFragment(MStrofa* strof)
{
	type_fragm = ID_FRAG_STROFA;
	m_fragm = (void*) new MStrofa;
	((MStrofa*)m_fragm)->Copy(strof);
}

//!==================================
MFragment::MFragment(MFraction* fraction)
{
	type_fragm = ID_FRAG_FRACTION;
	m_fragm = (void*) new MFraction();
	((MFraction*)m_fragm)->Copy(fraction);
}

//! ---------
void MFragment::Free()
{
	switch (type_fragm) {
	case ID_FRAG_STROFA:
		{
			delete (MStrofa*) m_fragm;
		}
		break;
	case ID_FRAG_FRACTION:
		{
			delete (MFraction*)m_fragm;
		}
		break;
	}
	
}

//! ----------
MFragment::~MFragment()
{
	Free();	
}


//==================================
//! Copy function
void MFragment::Copy(MFragment* fragment)
{
	Free();	
   type_fragm = fragment->type_fragm;
 
   switch (type_fragm)
	{
	case ID_FRAG_STROFA:
		{

			m_fragm = (void*) new MStrofa();
			((MStrofa*)m_fragm)->Copy((MStrofa*)fragment->m_fragm);						
		}
		break;
	case ID_FRAG_FRACTION:
		{
			m_fragm = (void*) new MFraction();
			((MFraction*)m_fragm)->Copy((MFraction*)fragment->m_fragm);						
		}
		break;	
	}
}
//==================================
//!Sizing
int MFragment::GetSize()
{   
	int Size = sizeof(unsigned char);
	
	switch(type_fragm){
	case ID_FRAG_STROFA:
		{   
			//m_fragm = (void*) new MStrofa();
			Size += ((MStrofa*)m_fragm)->GetSize();  
		}
		break;
	case ID_FRAG_FRACTION:
		{   
			//m_fragm = (void*) new MFraction();
			Size += ((MFraction*)m_fragm)->GetSize();  
		}
		break;	
	
	}
	return Size;	
}

//!================================================
void* MFragment::GetFragment()
{
	return m_fragm;
}


//===================================================================
//!Writing in UTP-file
bool MFragment::WriteBin(FILE* h_nuxfile, int version)
{
    //We determine the size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //Write the size
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write the stanza size
		fwrite(&type_fragm, sizeof(type_fragm), 1, h_nuxfile);
		//
		switch(type_fragm){
		case ID_FRAG_STROFA:
			{   
				//m_fragm = (void*) new MStrofa();
				((MStrofa*)m_fragm)->WriteBin(h_nuxfile, version);  
			}
			break;
		case ID_FRAG_FRACTION:
			{   
				//m_fragm = (void*) new MFraction();
				((MFraction*)m_fragm)->WriteBin(h_nuxfile, version);  
			}
			break;	
	
		}
	}
	return true;
}

//===================================================================================
//!Reading from the NUX-file
bool MFragment::ReadBin(FILE* h_nuxfile, int version)
{
	Free();
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE= 0;
            //we read record length
			fread(&SIZE, 4, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
            //read type
			fread(&type_fragm, 1, 1, h_nuxfile);
			switch(type_fragm){
			case ID_FRAG_STROFA:
				{
					m_fragm = (void*) new MStrofa();
					((MStrofa*)m_fragm)->ReadBin(h_nuxfile, version);  
				}
				break;
			case ID_FRAG_FRACTION:
				{   
					m_fragm = (void*) new MFraction();
					((MFraction*)m_fragm)->ReadBin(h_nuxfile, version);  
				}
				break;	
	
			}

	}
	return true;
}

//===================================================================================
//!Reading from the NUX-file
bool MFragment::ReadBin(MFile* file, int version)
{
	Free();
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE= 0;
            //we read record length
            file->GetData(&SIZE, 4, 1);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
        //read type
            file->GetData(&type_fragm, 1, 1);
			switch(type_fragm){
			case ID_FRAG_STROFA:
				{
					m_fragm = (void*) new MStrofa();
					((MStrofa*)m_fragm)->ReadBin(file, version);  
				}
				break;
			case ID_FRAG_FRACTION:
				{   
					m_fragm = (void*) new MFraction();
					((MFraction*)m_fragm)->ReadBin(file, version);  
				}
				break;	
	
			}

	}
	return true;
}

//!===================================================================================
void MFragment::SetType(unsigned char typenew)
{
	if (type_fragm == typenew) return;
	switch (type_fragm) {
	case ID_FRAG_STROFA:
		{
			delete (MStrofa*)m_fragm;
		}
		break;
	case ID_FRAG_FRACTION:
		{
			delete (MFraction*)m_fragm;
		}
		break;	
	}


	switch(typenew)
	{
		case ID_FRAG_STROFA:
		{
			m_fragm = (void*) new MStrofa();
		}
		break;
	case ID_FRAG_FRACTION:
		{
			m_fragm = (void*) new MFraction();
		}
		break;	
	}
	type_fragm=typenew;
}


//!===================================================================================
unsigned char MFragment::GetType()
{
	unsigned char Type;
	Type = type_fragm;
	return Type;

}

//================================================================
//!Comparison of the current fragment with the specified
bool MFragment::IsEqualTo(MFragment *m_pFragmentToCompare)
{
	if(type_fragm != m_pFragmentToCompare->type_fragm) 
		return false;
	switch(type_fragm){
	case ID_FRAG_STROFA:
		{
			return((MStrofa*)m_fragm)->IsEqualTo((MStrofa*)(m_pFragmentToCompare->m_fragm));
		} break;
	case ID_FRAG_FRACTION:
		{
			return((MFraction*)m_fragm)->IsEqualTo((MFraction*)(m_pFragmentToCompare->m_fragm));
		} break;
	default:  // exit(15); 
	        break;
	}
	return false;
}

//=======================================================
//! Receive quantity of stanzas
int MFragment::GetStrofNum()
{
	switch(type_fragm){
	case ID_FRAG_FRACTION: return 2;
	case ID_FRAG_STROFA: return 1;
//	default :exit(15);
	}
	return 0;
}

//=========================================================
//!Function of cloning
MFragment * MFragment::Clone()
{
	MFragment* m_pRetPointer = new MFragment();
	m_pRetPointer->Copy(this);
	return m_pRetPointer;
}


//*********************************************************
//******                   MFrArray                  ******
//*********************************************************
//============================================================
//!Returns quantity of stanzas in the array of fragments
int MFrArray::GetStrofNum()
{
	int m_iResult = 0;
	int m_iStrofaType;
	for(int i = 0 ;i<Size;i++){
		m_iStrofaType = GetAt(i)->GetType();
		switch(m_iStrofaType){
		case ID_STROFA_STROKA:
			m_iResult++; break;
		case ID_STROFA_STOLBEC:
			m_iResult+=2; break;
//		default:			exit(15);
		}
	}
	return m_iResult;
};


//*********************************************************
//******                   MFormArray                  ******
//*********************************************************


//============================================================
//!Copying of the array entirely
void MFormArray::Copy(MFormArray* pArray)
{
	int iNewSize = pArray->GetSize();
	SetSize(iNewSize);
	for(int i=0;i<iNewSize;i++){
		SetAt(i,pArray->GetAt(i));
	}
}

//============================================================
//!Receiving quantity of stanzas in the array of formulas
int MFormArray::GetStrofNum()
{
	int m_iResult=0;
	for(int i=0;i<Size;i++)
		m_iResult+=GetAt(i)->GetStrofNum();
	return m_iResult;
}

//============================================================
//!Returns quantity of formulas in the array
int MFormArray::GetFormNum()
{
	return Size+1;
}

//======================================================================
//!Parameters  'minStrofa' and 'maxStrofa' are filled respectively
//! number of the first and last stanza for this formula
void MFormArray::GetFormulaRange(int m_iFormulaIndex, int &minStrofa, int &maxStrofa)
{
	if(!((m_iFormulaIndex>=0)&&(m_iFormulaIndex<Size))){
		minStrofa = 0;
		maxStrofa = 0;
		return;
	};
	int m_iCounter = 0;
	for (int i=0;i<m_iFormulaIndex;i++)
		m_iCounter+=GetAt(i)->GetStrofNum();
	minStrofa = m_iCounter;
	maxStrofa = m_iCounter + GetAt(m_iFormulaIndex)->GetStrofNum();
}



//================MFormula======================================

//!==============================================================
MFormula::MFormula()
{
/*	int Size;
    //Constructors:
	Size = fragm_arr->GetSize();
	for (int i=0; i < Size; i++) 
	{	
		MFragment* fragment = fragm_arr->GetAt(i); 
		fragment->SetType(ID_FRAG_STROFA);
		delete fragment;
	}*/
	fragm_arr = new MFrArray();
	fragm_arr->SetSize(1);
	nazvanie = ID_FORMULA_NAZVANIE;
	m_iType = ID_FORMULA_SIMPLE;
}

///============================
MFormula::~MFormula()
{
	if (fragm_arr) delete fragm_arr;
}

//!==============================================================
MFormula::MFormula(MFrArray* fr_arr)
{
	fragm_arr = new MFrArray();
	int Size;
	Size = fr_arr->GetSize();
	fragm_arr->SetSize(Size);
	for (int i=0; i < Size; i++) 
		(fragm_arr->GetAt(i))->Copy(fr_arr->GetAt(i));
	nazvanie = ID_FORMULA_NAZVANIE;
	m_iType = ID_FORMULA_SIMPLE;
}

//!==============================================================
void MFormula::Copy(MFormula* formula)
{
	delete fragm_arr;
	fragm_arr = (MFrArray*)(formula->fragm_arr->Clone());
	nazvanie = formula->nazvanie;
	m_iType = formula->m_iType;
}

//==================================
//!sizing
int MFormula::GetSize()
{   
	int Size;
	Size = 4;
	Size += nazvanie.GetLength() + 2;
	Size += fragm_arr->GetBytes();
	return Size;	
}

//!=======================================================
void MFormula::SetType(int m_iNewType)
{
	m_iType = m_iNewType;
}

//!=========================================================
int MFormula::GetType()
{
	return m_iType;
}


//===================================================================
//!Writing in the UTP-file
bool MFormula::WriteBin(FILE* h_nuxfile, int version)
{
    //We determine the size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //Write the size
		fwrite(&Size, 4, 1, h_nuxfile);

        //Write the stanza type
		fwrite(&m_iType, sizeof(m_iType), 1, h_nuxfile);

        //Write
		fragm_arr->WriteBin(h_nuxfile, version);
		WriteString(h_nuxfile, version, nazvanie);
	}
	return true;
}

//===================================================================================
//!Reading from the NUX-file
bool MFormula::ReadBin(FILE* h_nuxfile, int version)
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE= 0;
            //we read record length
			fread(&SIZE, 4, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
//			//read type
			fread(&m_iType, sizeof(m_iType), 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)&m_iType, sizeof(m_iType) );
            //read
			fragm_arr->ReadBin(h_nuxfile, version);
		 nazvanie = ReadString(h_nuxfile, version);	
	}
	return true;
}

//===================================================================================
//!Reading from the NUX-file
bool MFormula::ReadBin(MFile* file, int version)
{
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE= 0;
            //we read record length
            file->GetData(&SIZE, 4, 1);
 		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
            //read type
            file->GetData(&m_iType, sizeof(m_iType), 1);
 		if (reverseReadFlag) reverseValue((char*)&m_iType, sizeof(m_iType) );
            //read
			fragm_arr->ReadBin(file, version);
			nazvanie = ReadString(file, version);	
	}
	return true;
}

//============================================================
//!Comparison of structures of two formulas on identity is made
//! Only structures and also names of formulas are checked
bool MFormula::IsStructureEqualTo(MFormula *m_pMFormula)
{
	if((fragm_arr->Size!=m_pMFormula->fragm_arr->Size)||
		(nazvanie!=m_pMFormula->nazvanie))
		return false;
	if(m_iType!=m_pMFormula->m_iType)
		return false;
	MFragment *m_pThisFragment;
	MFragment *m_pCompareFragment;
	for(int i=0;i<fragm_arr->Size;i++){
        //Comparison of structures of fragments
		m_pThisFragment = fragm_arr->GetAt(i);
		m_pCompareFragment = m_pMFormula->fragm_arr->GetAt(i);
		if(m_pThisFragment->GetType()!= m_pCompareFragment->GetType())
			return false;
	}
	return true;
}

//!=============================================================
void MFormula::Free()
{
	delete fragm_arr;
}

//=========================================================
//!Comparison of two formulas
bool MFormula::IsEqualTo(MFormula *m_pFormulaToCompare)
{
	if((nazvanie==m_pFormulaToCompare->nazvanie)&&
		(m_iType == m_pFormulaToCompare->m_iType))
		return fragm_arr->IsEqualTo(m_pFormulaToCompare->fragm_arr);
	return false;
}

//=======================================================
//!Returns quantity of stanzas
int MFormula::GetStrofNum()
{
	return fragm_arr->GetStrofNum();
}

//=====================================================
//! To establish the name
void MFormula::SetNazvanie(MString m_strNazv)
{
	nazvanie = m_strNazv;	
}

