//сhange by Vadim Shashlevich
#include "Table.h"
#include "Array.h"
#include "Klassifikator.h"
#include "Refglobal.h"

extern MArray* (*fpCreateArrayByType)(MString str);

//==========================================================================
//!Constructor
MTable::MTable()
//==========================================================================
{
	m_ServInfo.SetSize(TABLE_SERVINFO_ROWS_NUM);
    //We create a line of names of columns
	MStArray* psta = new MStArray();
	m_ServInfo.SetAt(TABLE_SERVINFO_NAME_ROW,psta);
    //We create a line of types of columns
	psta = new MStArray();
	m_ServInfo.SetAt(TABLE_SERVINFO_TYPE_ROW,psta);
    //We create a line of types of columns
	MBoArray* pboa = new MBoArray();
	m_ServInfo.SetAt(TABLE_SERVINFO_KEY_ROW,pboa);
	m_iMaxSize = 0xffffffff;//UINT_MAX
}


//==========================================================================
//!Constructor
MTable::MTable(int iMaxSize)
//==========================================================================
{
	m_ServInfo.SetSize(TABLE_SERVINFO_ROWS_NUM);
    //We create a line of names of columns
	MStArray* psta = new MStArray();
	m_ServInfo.SetAt(TABLE_SERVINFO_NAME_ROW,psta);
    //We create a line of types of columns
	psta = new MStArray();
	m_ServInfo.SetAt(TABLE_SERVINFO_TYPE_ROW,psta);
    //We create a line of types of columns
	MBoArray* pboa = new MBoArray();
	m_ServInfo.SetAt(TABLE_SERVINFO_KEY_ROW,pboa);
	m_iMaxSize = iMaxSize;
}



//==========================================================
//!Reading from the file
bool MTable::ReadBin(FILE* h_nuxfile,int version)
//==========================================================
{
    //Before reading it is necessary to clear earlier got arrays
	int i = 0;
	for(i = m_Table.GetSize()-1;i>=0;i--)
		delete m_Table.GetAt(i);
	for(i = m_ServInfo.GetSize()-1;i>=0;i--)
		delete m_ServInfo.GetAt(i);
	m_Table.ClearAll();
	m_ServInfo.ClearAll();

	if((m_Table.ReadBin(h_nuxfile,version))&&
		(m_ServInfo.ReadBin(h_nuxfile,version))){
		fread(&m_iMaxSize, sizeof(m_iMaxSize), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_iMaxSize, sizeof(m_iMaxSize) );
		return true;
	}
	else 
		return false;
}

//==========================================================
//!Reading from the file
bool MTable::ReadBin(MFile* file,int version)
//==========================================================
{
    //Before reading it is necessary to clear earlier got arrays
	int i = 0;
	for(i = m_Table.GetSize()-1;i>=0;i--)
		delete m_Table.GetAt(i);
	for(i = m_ServInfo.GetSize()-1;i>=0;i--)
		delete m_ServInfo.GetAt(i);
	m_Table.ClearAll();
	m_ServInfo.ClearAll();

	if((m_Table.ReadBin(file,version))&&
		(m_ServInfo.ReadBin(file,version))){
		file->GetData(&m_iMaxSize, sizeof(m_iMaxSize), 1);
		if (reverseReadFlag) reverseValue((char*)&m_iMaxSize, sizeof(m_iMaxSize) );
		return true;
	}
	else 
		return false;
}

//==========================================================
//! file recording
bool MTable::WriteBin(FILE* h_nuxfile,int version)
//==========================================================
{
	if((m_Table.WriteBin(h_nuxfile,version))&&
		(m_ServInfo.WriteBin(h_nuxfile,version))){
		fwrite(&m_iMaxSize, sizeof(m_iMaxSize), 1, h_nuxfile);
		return true;
	}
	else 
		return false;
}


//==========================================================
//!Addition of new type in the table
//! If the field index that is specified it is added with this index
int MTable::AddField(MString strFieldType,MString strName/* = ""*/,int iIndex/* = -1*/)
//==========================================================
{
    //If at insertion try to replace a key element
	MArray* pAr = (*fpCreateArrayByType)(strFieldType);
	if(iIndex==-1){
		m_Table.AddTail(pAr);
		iIndex = m_Table.GetSize()-1;
	}
	else{
        //If the specified index doesn't exist in the table and isn't negative
        //that we adjust the table by the size to establish on this index
		if(iIndex>=m_Table.Size)
			m_Table.SetSize(iIndex+1);
		MArray* pPrevar = m_Table.GetAt(iIndex);
		if(pPrevar)
			delete pPrevar;
		m_Table.SetAt(iIndex,pAr);
	}
    //Filling of service information by default
	if(iIndex==-1){
		((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW))->AddTail(&strFieldType);
		((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_NAME_ROW))->AddTail(&strName);
		bool bTemp = false;
		((MBoArray*)m_ServInfo.GetAt(TABLE_SERVINFO_KEY_ROW))->AddTail(&bTemp);
	}
	else{
        //We set the size for all lines of the table of service information
		if(iIndex>=m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW)->GetSize())
			for(int i = m_ServInfo.GetSize()-1;i>=0;i--)
				m_ServInfo.GetAt(i)->SetSize(iIndex+1);
		((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW))->SetAt(iIndex,&strFieldType);
		((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_NAME_ROW))->SetAt(iIndex,&strName);
		bool bTemp = false;
		((MBoArray*)m_ServInfo.GetAt(TABLE_SERVINFO_KEY_ROW))->SetAt(iIndex,&bTemp);
	}
	return iIndex;
}


//==========================================================
//!To receive field type
MString MTable::GetFieldType(int iField)
//==========================================================
{
	if(iField>=m_Table.GetSize())
		return "";
	return *(((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW))->GetAt(iField));
}


//=============================================================
//!Receiving index of a key element
int MTable::GetKeyField()
//=============================================================
{
	MBoArray *pBoAr = (MBoArray*)m_ServInfo.GetAt(TABLE_SERVINFO_KEY_ROW);
	for(int i = pBoAr->GetSize()-1;i>=0;i--){
		if(*pBoAr->GetAt(i))
			return i;
	}
	return -1;
}

//==========================================================
//!Provides search of record in a key
/**It is considered that this array is sorted by increase of a key
//If record isn't found, 'm_iMaxSize' comes back
*/
int MTable::GetIndexByKey(int iKey)
//==========================================================
{
	int iKeyField = GetKeyField();
	if(iKeyField==-1)
		return m_iMaxSize;

	int iSize =m_Table.GetAt(iKeyField)->GetSize(); 
	if(!iSize)
		return m_iMaxSize;
	int iRec=(iSize-1)/2;
	int iStep = iSize/2;
	if(!iStep)
		iStep = 1;
	bool bContinue= true;
	do{
		int iCurKey= *((MInArray*)m_Table.GetAt(iKeyField))->GetAt(iRec);
        //check on an exit
		if(iStep==1){
			if(iCurKey<iKey){
				if(iRec==(iSize-1))
					return m_iMaxSize;
				else{
					int iNeihbKey = *((MInArray*)m_Table.GetAt(iKeyField))->GetAt(iRec+1);
					if (iNeihbKey>iKey)
						return m_iMaxSize;
				}
			}
			else if(iCurKey>iKey){
				if(iRec==0)
					return m_iMaxSize;
				else{
					int iNeihbKey = *((MInArray*)m_Table.GetAt(iKeyField))->GetAt(iRec+1);
					if (iNeihbKey<iKey)
						return m_iMaxSize;
				}
			}
		}

		if(iStep>1)
			iStep/=2;
		if(iCurKey>iKey){
			iRec-=iStep;
		}
		else if(iCurKey<iKey){
			iRec+=iStep;
		}
		else 
			return iRec;
	}while(bContinue);

	return m_iMaxSize;
}


//==========================================================
//!Function makes sorting of the table according to a key in the increasing order
void MTable::SortByKey()
//==========================================================
{
	int iKeyField = GetKeyField();
	if(iKeyField==-1)
		return;
	MInArray *pina = (MInArray*)m_Table.GetAt(iKeyField);
	int iSize =pina->GetSize(); 

    //Sorting of methods of a bubble
	for(int i=0;i<(iSize-1);i++)
		for(int j=i;j<(iSize-1);j++){
			if(*pina->GetAt(j)>*pina->GetAt(j+1)){
                //Replacement of two elements with places
                //Two records are interchanged the position
				for(int k = m_ServInfo.GetSize()-1;k>=0;k--)
					m_Table.GetAt(k)->Exchange(j,j+1);
			}
		}
}



//===========================================================
//!Search of the first suitable key
/**Means that the table is sorted by a key
//'m_iMaxSize' if that isn't found
//Means that the table of an otsortirovannn on increase
*/
int MTable::GetFreeKey()
//===========================================================
{
	int iKeyField = GetKeyField();
	if(iKeyField==-1)
		return m_iMaxSize;
	MInArray *pina = (MInArray*)m_Table.GetAt(iKeyField);
	if(!pina->GetSize())
		return 0;
	if((unsigned int)*pina->GetTail()==m_iMaxSize-1){
		if((unsigned int)pina->GetSize()==m_iMaxSize)
			return m_iMaxSize;
		else{
            //Search from the head in search of a free element
			int iStart = 0;
			do{

				if(*pina->GetAt(iStart)!=iStart)
					return iStart;
				iStart++;
			}while(true);
		}
	}
	else
		return (*pina->GetTail())+1;
}


//===========================================================
//!Addition of new record in the table
/**Returns -1 if in the table there is no more place
*/
int MTable::CreateRecord()
//===========================================================
{
	int iKeyField = GetKeyField();
	if(iKeyField==-1)
		return -1;
	int pinaSize = m_Table.GetAt(iKeyField)->GetSize();
	int iRetVal = GetFreeKey();
	if((unsigned int)iRetVal == m_iMaxSize)
		return -1;
	for(int i = m_Table.GetSize()-1;i>=0;i--){
		m_Table.GetAt(i)->SetSize(pinaSize+1);
	}
	MInArray* pina = (MInArray*)m_Table.GetAt(iKeyField);
	*pina->GetTail()=iRetVal;
	SortByKey();
	return iRetVal;
}

//===========================================================
//!There has to be a search of record in a cell
int MTable::FindCell(int /*iField*/, void* /*pElement*/)
//===========================================================
{
//	exit(15);
	return 0;
}

//=============================================================
//!To receive for record with a key a field with an index
void* MTable::GetCell(int iRecKey,int iField)
//=============================================================
{
	int iIndex = GetIndexByKey(iRecKey);
	if((unsigned int)iIndex == m_iMaxSize)
		return NULL;
	return m_Table.GetAt(iField)->GetAtVoid(iIndex);
}


//=============================================================
//!To set a cell on a key
bool MTable::SetCell(int iRecKey,int iField, void* pElement)
//=============================================================
{
	int iIndex = GetIndexByKey(iRecKey);
	if((unsigned int)iIndex == m_iMaxSize)
		return false;
	if(m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW)->GetSize()-1<iField)
		return false;
    //If we try to set a cell in a key field, it is required
    //special check on opportunity
	if(iField == GetKeyField())
		return SetKeyCell(iRecKey,iField,pElement);
	else
		m_Table.GetAt(iField)->SetAtVoid(iIndex,pElement);
	return true;
}

//==========================================================
//!Removal of a field
bool MTable::DeleteField(int iField)
//==========================================================
{
	if(iField>=m_Table.GetSize())
		return false;
	m_Table.Delete(iField);
	for(int i=0;i<TABLE_SERVINFO_ROWS_NUM;i++)
		m_ServInfo.GetAt(i)->Delete(iField);
	return true;
}


//==========================================================
//!Removal of record from the table on a key
void MTable::DeleteRecord(int iRecKey)
//==========================================================
{
	int iDelIndex = GetIndexByKey(iRecKey);
	if((unsigned int)iDelIndex!=m_iMaxSize){
		for(int i = m_Table.GetSize()-1;i>=0;i--){
			m_Table.GetAt(i)->Delete(iDelIndex);
		}
	}
}

//=======================================================
//!Installation of a key field
/**the established field surely owes baht the array of int
//THE array OF VALUES OF THE SPECIFIED FIELD IS CHECKED FOR IDENTITY AND REPEATED
//FIELDS ARE REMOVED
*/
bool MTable::SetKeyField(int iField)
//=======================================================
{
	if(GetFieldType(iField)!=ID_INA)
		return false;
	MInArray* pina = (MInArray*)m_Table.GetAt(iField);
	for(int i = pina->GetSize()-1;i>0;i--){
		int iCurKey = *pina->GetAt(i);
		for(int j = i-1;j>=0;j--){
            //We check the current key for identity and we delete
            //the duplicated values
			if (iCurKey == *pina->GetAt(j)){
				//Удаление
				for(int k=m_Table.GetAt(0)->GetSize()-1;k>=0;k--){
					m_Table.GetAt(k)->Delete(j);
                    //After removal of an element it is required to displace a tail
					i--;
				}
			}
		}
	}
    //Installation of bit of a key field
	bool bTemp = true;
	((MBoArray*)m_ServInfo.GetAt(TABLE_SERVINFO_KEY_ROW))->SetAt(iField,&bTemp);
	return true;

}


//=======================================================
//!Clonning
MTable*  MTable::Clone()
//=======================================================

{
	MTable *pClone = new MTable();
	pClone->Copy(this);
	return pClone;
}


//=======================================================
//!Copying
void MTable::Copy(MTable* pToCopy)
//=======================================================
{
	m_iMaxSize = pToCopy->m_iMaxSize;
	int iSize = pToCopy->m_ServInfo.GetSize();
	m_ServInfo.SetSize(iSize);
	int i = 0;
	for(i=0;i<iSize;i++){
		m_ServInfo.SetAt(i,pToCopy->m_ServInfo.GetAt(i)->Clone());
	}

	iSize = pToCopy->m_Table.GetSize();
	m_Table.SetSize(iSize);
	for(i=0;i<iSize;i++){
		m_Table.SetAt(i,pToCopy->m_Table.GetAt(i)->Clone());
	}
}

//=======================================================
//!Obtaining length
int MTable::GetBytes()
//=======================================================
{
	int iRetVal = sizeof(m_iMaxSize);
	iRetVal +=m_Table.GetBytes();
	iRetVal +=m_ServInfo.GetBytes();
	return iRetVal;
}

//=======================================================
//!Establishes the maximum size for the table
void MTable::SetMaxSize(int iMaxSize)
//=======================================================
{
	if((unsigned int)iMaxSize>m_iMaxSize){
		m_iMaxSize = iMaxSize;
		return;
	}
	int iKeyField = GetKeyField();
	if(iKeyField==-1){
		m_iMaxSize = iMaxSize;
		return;
	}

	MInArray *pina = (MInArray*)m_Table.GetAt(iKeyField);
    //We make removal of all records which code exceeds our value
	for(int i = pina->GetSize()-1;i>=0;i--){
		if(*pina->GetAt(i)>iMaxSize){
			for(int k = m_Table.GetSize();k>=0;k--)
				m_Table.GetAt(k)->Delete(i);
		}
	}
	m_iMaxSize = iMaxSize;
}




//====================================================================
//!Function establishes a key field for a cell
bool MTable::SetKeyCell(int iRecKey,int iField, void* pElement)
//====================================================================
{
	int iIndex = GetIndexByKey(iRecKey);
	if((unsigned int)iIndex == m_iMaxSize)
		return false;
	if((unsigned int)GetIndexByKey(*((int*)pElement))!=m_iMaxSize)
		return false;
	int iKey = *((int*)pElement);
	if((unsigned int)iKey>=m_iMaxSize)
		return false;
	((MInArray*)m_Table.GetAt(iField))->SetAt(iIndex,(int*)pElement);
	SortByKey();
	return true;
}



//====================================================================
//!Whether checks the field is simple
/**whether that is its elements can be transformed everything at line and back
*/
bool MTable::IsFieldSimple(unsigned int iFieldIndex)
//====================================================================
{
	MStArray* pTypeArray = (MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW);
	MString strType = *pTypeArray->GetAt(iFieldIndex);
	if((strType!=ID_STA)&&
		(strType!=ID_INA))
		return false;
	return true;
}




//====================================================================
//!Whether checks the field is simple
//!whether that is its elements can be transformed everything at line and back
bool MTable::IsSimple()
//====================================================================
{
	int iSize= m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW)->GetSize();
	for(int i =iSize-1;i>=0;i--){
		if(!IsFieldSimple(i))
			return false;
	}
	return true;
}


//====================================================================
//!Receives a field for the simple table
bool MTable::GetSimpleCell(int iRecKey,int iField,MString& strLoadString)
//====================================================================
{
	if(!IsFieldSimple(iField)) return false;
	MString strType = GetFieldType(iField);
	void* pret = GetCell(iRecKey,iField);
	if(!pret)
		return false;
	if(strType==ID_INA)
        //We transfer 'int' to a 'string'
		strLoadString.Format("%d",*((int*)pret));
	else if(strType == ID_STA)
        //the returned size is a line
		strLoadString = *((MString*)pret);
	else
		return false;    // exit(15);
	return true;
}


//====================================================================
//!Sets a field for the simple table, proceeding from rules of converting
bool MTable::SetSimpleCell(int iRecKey,int iField,MString* pElement)
//====================================================================
{
	if(!IsFieldSimple(iField)) return false;
	MString strType = GetFieldType(iField);
	if(strType==ID_INA){
        //We transfer a 'string' to 'int'
		int iSetVal = atoi((char *)*pElement);
		return SetCell(iRecKey,iField,&iSetVal);
	}
	else if(strType == ID_STA)
        //the returned size is a line
		return SetCell(iRecKey,iField,pElement);
	else
		return false;    //		exit(15);
	return false;
}

//Extension of the table functions of consecutive access (search)
//Records
//=================================================
//!Function returns the first key
bool MTable::GetFirstKey(int &iKey)
//=================================================
{
	int iKeyField = GetKeyField();
	if(!GetDimension().cx)
		return false;
	iKey = *((MInArray*)m_Table.GetAt(iKeyField))->GetHead();
	return true;
}


//=================================================
//!Function returns the next key
bool MTable::GetNextKey(int iPrevKey, int &iKey)
//=================================================
{
	int iKeyField = GetKeyField();
	int iPrevIndex = GetIndexByKey(iPrevKey);
	if((unsigned int)iPrevIndex == m_iMaxSize)
		return false;
	if(GetDimension().cx<=(iPrevIndex+1))
		return false;
	iKey = *((MInArray*)m_Table.GetAt(iKeyField))->GetAt(iPrevIndex+1);
	return true;
}


//================================================================
//!Function of addition of record to the table with use of the previous
//! in debug check on quantity of columns is made
//! actually it is very dangerous function which works with
//!void*
int MTable::UseRecord(MVoidArray* pva,int iFieldOutOfCompare)
//================================================================
{
	int ikeyField = GetKeyField();
	MIntSize dim = GetDimension();
	if(pva->GetSize()!=(dim.cy-1))
		return -1;
	int iRecExisting = -1;
    //CUNNING FOR ACCESS TO THE array ON THE SAME INDEX AS FOR ACCESS TO THE TABLE
	void* temp = NULL;
	pva->InsertBefore(ikeyField,&temp);

	for(int i = 0;i<dim.cx;i++){
        //Pro-race on all records
		for(int j=0;j<dim.cy;j++){
            //We learn type we bring obviously and we compare values
			if((j!=ikeyField)&&(j!=iFieldOutOfCompare))
				if(!CompareWith(i,j,*pva->GetAt(j)))
					break;
			if(j==dim.cy-1)
				iRecExisting = *((MInArray*)m_Table.GetAt(ikeyField))->GetAt(i);
		}
		if(iRecExisting!=-1)
			break;
	}
	pva->Delete(ikeyField);
	return iRecExisting;
}

//===================================================================
//!dobavlt record or receives a key for the already existing
int MTable::AddOrGetUsed(MVoidArray* pva)
//===================================================================
{
	int iGetUsed = UseRecord(pva);
	if(iGetUsed!=-1)
		return iGetUsed;
	int ikeyField = GetKeyField();

	int iNewRec = CreateRecord();
	if(iNewRec == -1)
        //The table is crowded
		return iNewRec;
    //CUNNING FOR ACCESS TO THE array ON THE SAME INDEX AS FOR ACCESS TO THE TABLE
    void* temp = NULL;
	pva->InsertBefore(ikeyField,&temp);

	for(int i = 0;i<pva->GetSize();i++){
		if(i!=ikeyField)
			SetCell(iNewRec,i,*pva->GetAt(i));
	}
	pva->Delete(ikeyField);
	return iNewRec;
}



//=================================================================================
//!Function of comparison of any cell of the table with transferred elemnty,
//! it is meant that the class of the transferred element coincides with a class
//! the element which is stored in a cell
bool MTable::CompareWith(int iRow,int iCol,void* pElToCompare)
//=================================================================================
{
    //We learn type we bring obviously and we compare values
    //Cells in a key field aren't exposed to comparison
	if((iCol==GetKeyField())&&(iCol!=-1))
		return false;
	MString type = *(((MStArray*)m_ServInfo.GetAt(TABLE_SERVINFO_TYPE_ROW))->GetAt(iCol));
	if(type == ID_STA){
        //Comparison can is made only nonempty lines
		if((((MString*)pElToCompare)->IsEmpty())&&((((MStArray*)m_Table.GetAt(iCol))->GetAt(iRow))->IsEmpty()))
			return true;
		if(*(MString*)pElToCompare!=*((MStArray*)m_Table.GetAt(iCol))->GetAt(iRow))
			return false;
	}
	else if(type == ID_INA){
		if(*(int*)pElToCompare!=*((MInArray*)m_Table.GetAt(iCol))->GetAt(iRow))
			return false;
	}
	else if(type == ID_CLA){
		if(*(MColor*)pElToCompare!=*((MClArray*)m_Table.GetAt(iCol))->GetAt(iRow))
			return false;
	}
	else if(type == ID_USA){
		if(*(unsigned short*)pElToCompare!=*((MShInArray*)m_Table.GetAt(iCol))->GetAt(iRow))
			return false;
	}
	else if(type == ID_LFA){
		if(!memcmp(pElToCompare,((MLfArray*)m_Table.GetAt(iCol))->GetAt(iRow),sizeof(M_LOG_FONT)))
			return false;
	}
	else if(type == ID_DUA){
		if(*(double*)pElToCompare!=*((MDuArray*)m_Table.GetAt(iCol))->GetAt(iRow))
			return false;
	}
	else if(type == ID_BOA){
		if(*(bool*)pElToCompare!=*((MBoArray*)m_Table.GetAt(iCol))->GetAt(iRow))
			return false;
	}
	else if(type == ID_PKA){
		if(!((MPrk*)pElToCompare)->IsEqualTo(((MPkArray*)m_Table.GetAt(iCol))->GetAt(iRow)))
			return false;
	else if(type == ID_IMG){
		if(!((MImage*)pElToCompare)->IsEqualTo(((MImgArray*)m_Table.GetAt(iCol))->GetAt(iRow)))
			return false;
	}
	}
	else
        InternalMessage(IDS_UNKNWN_MASSIVE);//"Unknown For MTable Massive Type"
	return true;
}

//=======================================================
//! Erases all data from the table
void MTable::ClearTable()
//=======================================================
{
	for(int i=0;i<m_Table.GetSize();i++)
		DeleteField(i);
}

//=======================================================
//! Finds record on the entered value of the set field and transfers
//! according to the link value of the specified field. In case of successful search
//! returns 'true' value
bool MTable::FindInTable(int CmpField,MString CmpElement,int RetField,MString &RetElement)
//=======================================================
{
    int tekKey = 0, prevKey = 0;  // Variables for definition of a key
    MString   LoadStr;			  // Variable for reading of a line
	GetFirstKey(tekKey);
	for(int k=1;k<=GetDimension().cx;k++)
	{
		GetSimpleCell(tekKey,CmpField,LoadStr);
        // We delete all non-significant spaces
		while(LoadStr.Find(' ')==0)
			LoadStr.Delete(0,1);
        // We check the received line for coincidence to the demanded
		if(strcmp(LoadStr,CmpElement)==0)
		{
            // We give the corresponding value
			GetSimpleCell(tekKey,RetField,RetElement);
            // If the demanded value is found, we return 'true'
			return true;
		}
		prevKey = tekKey;
		GetNextKey(prevKey,tekKey);
	}
    // If the demanded value it isn't found, we return 'false'
	return false;
}
//=======================================================
//! Finds record on the entered value of the set field and transfers
//! according to the link value of the specified field. In case of successful search
//! returns true value
bool MTable::FindInTableCMP(int CmpField,MString Cmp1pole,MString Cmp2pole,int RetField,MString &RetElement)
//=======================================================
{
    int tekKey = 0, prevKey = 0;  // Variables for definition of a key
    MString   LoadStr, PodStr;			  // Variable for reading of a line
	char *pdest;
	int iFound;
	bool bMatch;
	GetFirstKey(tekKey);
	for(int k=1;k<=GetDimension().cx;k++)
	{
		GetSimpleCell(tekKey,CmpField,LoadStr);
        // We delete all non-significant spaces
		while(LoadStr.Find(' ') == 0)
			LoadStr.Delete(0,1);
        //We check, whether the received line 'Cmp1pole' includes
		pdest = strstr( LoadStr, Cmp1pole );		
		
		bMatch = false;
        if(pdest != NULL)//if includes
		{
            //we will pair the rest of a line off < >,< >,< >
            //also we will look for each couple in 'cmp2pole'
			LoadStr.Delete(0, Cmp1pole.GetLength());
			iFound = LoadStr.Find('>');
			if (iFound == -1 )
			{
                // We give the corresponding value
				GetSimpleCell(tekKey,RetField,RetElement);
                // If the demanded value is found, we return 'true'
				return true;
			}			

			//ПЕРВАЯ ПОДСТРОКА
			PodStr = LoadStr.Left(iFound+1);
			
			pdest = strstr( Cmp2pole, PodStr );		
            //if found first <sign> from CMP in attributes of object
			if (pdest) 
			{
				bMatch = true;
				LoadStr.Delete(0,iFound+1);
                //If there were couples for comparison
				if (!LoadStr.IsEmpty())
				{
					pdest = strstr( Cmp2pole, LoadStr );		
					if (!pdest) bMatch = false;
				}
			}

			if (bMatch){
                // We give the corresponding value
				GetSimpleCell(tekKey,RetField,RetElement);
                // If the demanded value is found, we return 'true'
				return true;
			}
		}
		prevKey = tekKey;
		GetNextKey(prevKey,tekKey);
	}
    // If the demanded value it isn't found, we return 'false'
	return false;
}

//=======================================================
//! Finds record on the entered value of the set field
/**also transfers
//according to the link value of the specified field. In case of successful search
//returns 'true' value
*/
bool MTable::FindInTableExt(int CmpField1,MString CmpElement1,int CmpField2,MString CmpElement2,int RetField,MString &RetElement)
//=======================================================
{
    int tekKey = 0, prevKey = 0;  // Variables for definition of a key
    MString   LoadStr;			  // Variable for reading of a line
    MString   LastFind = "";	  // Variable for storage of latest entry
	GetFirstKey(tekKey);
	for(int k=1;k<=GetDimension().cx;k++)
	{
		GetSimpleCell(tekKey,CmpField1,LoadStr);
        // We delete all non-significant spaces
		while(LoadStr.Find(' ')==0)
			LoadStr.Delete(0,1);
        // We check the received line for coincidence to the demanded
		if(strcmp(LoadStr,CmpElement1)==0)
		{
                // Just in case we keep
					GetSimpleCell(tekKey,RetField,LastFind);
            // If the line coincides, we will check the second condition
			GetSimpleCell(tekKey,CmpField2,LoadStr);	
            // We delete all non-significant spaces
			while(LoadStr.Find(' ')==0)
				LoadStr.Delete(0,1);
            // We check the received line for coincidence to the demanded
			if(strcmp(LoadStr,CmpElement2)==0)
			{
                // We give the corresponding value
				GetSimpleCell(tekKey,RetField,RetElement);
                // If the demanded value is found, we return 'true'
				return true;
			}
		}
		prevKey = tekKey;
		GetNextKey(prevKey,tekKey);
	}

	RetElement = LastFind;
	if(LastFind!="")	return true;

    // If the demanded value it isn't found, we return 'false'
	return false;
}

//=======================================================
//! Finds record on the entered value of the set field
/** also transfers value of the specified field according to the link. In case of successful search
//returns 'true' value
*/
bool MTable::FindInTableExt2(int CmpField,MString CmpElement,int RetField1,MString &RetElement1,int RetField2,MString &RetElement2)
//=======================================================
{
    int tekKey = 0, prevKey = 0;  // Variables for definition of a key
    MString   LoadStr;			  // Variable for reading of a line
	
	GetFirstKey(tekKey);
	for(int k=1;k<=GetDimension().cx;k++)
	{
		GetSimpleCell(tekKey,CmpField,LoadStr);
        // We delete all non-significant spaces
		while(LoadStr.Find(' ')==0)
			LoadStr.Delete(0,1);
        // We check the received line for coincidence to the demanded
		if(strcmp(LoadStr,CmpElement)==0)
		{
			GetSimpleCell(tekKey,RetField1,RetElement1);
			GetSimpleCell(tekKey,RetField2,RetElement2);	
			return true;
		}
		prevKey = tekKey;
		GetNextKey(prevKey,tekKey);
	}

    // If the demanded value it isn't found, we return 'false'
	return false;
}

//**********************************************************************
//********************** MFixedSizeTable
//**********************************************************************


//=======================================================
//!Constructor
MFixedSizeTable::MFixedSizeTable():MTable()
//=======================================================
{
	m_iMaxSize = 0;
}


//=======================================================
//!Constructor
MFixedSizeTable::MFixedSizeTable(int iMaxSize):MTable(iMaxSize)
//=======================================================
{
	SetMaxSize(iMaxSize);
}
				


//=======================================================
//!Function for obtaining dimension of the table
MIntSize MFixedSizeTable::GetDimension()
//=======================================================
{
	int iColNum = m_Table.GetSize();
	if(!iColNum)
		return MIntSize(0,0);
	return MIntSize(m_iMaxSize,iColNum);
};


//=======================================================
//!Installation of a key field
/**the established field surely owes baht the array of int
//as it is the array of direct indexation, the key is renumbered
*/
bool MFixedSizeTable::SetKeyField(int iField)
//=======================================================
{
	if(GetFieldType(iField)!=ID_INA)
		return false;
	MInArray* pina = (MInArray*)m_Table.GetAt(iField);
	for(int i = pina->GetSize()-1;i>=0;i--){
		pina->SetAt(i,&i);
	}
    //Installation of bit of a key field
	bool bTemp = true;
	((MBoArray*)m_ServInfo.GetAt(TABLE_SERVINFO_KEY_ROW))->SetAt(iField,&bTemp);
	return true;
}



//=======================================================
//!Installation of the maximum size
void MFixedSizeTable::SetMaxSize(int iMaxSize)
//=======================================================
{
	int ikey = GetKeyField();
    //Installation of the table size
	int i = 0;
	for(i = m_Table.GetSize()-1;i>=0;i--)
		m_Table.GetAt(i)->SetSize(iMaxSize);
	MInArray* pina = (MInArray*)m_Table.GetAt(ikey);
	for(i = 0;i<iMaxSize;i++)
		pina->SetAt(i,&i);
	m_iMaxSize = iMaxSize;
};


//=======================================================
//!it is intended for iteration
bool MFixedSizeTable::GetFirstKey(int &iKey)
//=======================================================
{
	if(m_Table.GetAt(0)->GetSize()){
		iKey = 0;
		return true;
	}
	return false;
};

//=======================================================
//!it is intended for iteration
bool MFixedSizeTable::GetNextKey(int iPrevKey,int &iKey)
//=======================================================
{
	if(m_Table.GetAt(0)->GetSize()>iPrevKey+1){
		iKey = iPrevKey+1;
		return true;
	}
	return false;
}

//============================================================
//!Removal of record too most that installation of its value is identical with the last
void MFixedSizeTable::DeleteRecord(int iRecKey)
//============================================================
{
	MIntSize dim = GetDimension();
	for(int i = 0;i<dim.cy;i++){
			SetCell(iRecKey,i,GetCell(m_iMaxSize-1,i));
	}
}

//========================================================
//!Returns the first empty record (that is same as well as the last)
int MFixedSizeTable::CreateRecord()
//========================================================
{
	int ikeyField = GetKeyField();
	MIntSize dim = GetDimension();
	int iRecExisting = -1;
    //CUNNING FOR ACCESS TO THE array ON THE SAME INDEX AS FOR ACCESS TO THE TABLE
	for(int i = 0;i<dim.cx;i++){
        //Pro-race on all records
		for(int j=0;j<dim.cy;j++){
            //We learn type we bring obviously and we compare values
			if(j!=ikeyField)
				if(!CompareWith(i,j,m_Table.GetAt(j)->GetAtVoid(dim.cx-1)))
					break;
			if(j==dim.cy-1)
				iRecExisting = *((MInArray*)m_Table.GetAt(ikeyField))->GetAt(i);
		}
		if(iRecExisting!=-1)
			break;
	}
	if(iRecExisting!=dim.cx-1)
		return iRecExisting;
	return -1;
};

//=============================================================
//!Returns quantity of nonzero elements in the table of the classifier
int MFixedSizeTable::GetFilledNum()
//=============================================================
{
	MIntSize dim = GetDimension();
	int ikeyField = GetKeyField();
	for(int iLast = dim.cx-1;iLast>=0;iLast--){
		bool bEqual = true;
		for(int i = 0;i<dim.cy;i++)
			if(i!=ikeyField)
				if(!CompareWith(dim.cx-1,i,m_Table.GetAt(i)->GetAtVoid(iLast)))
					bEqual = false;
		if(!bEqual)
			return iLast+1;
	}
	return 0;
}



//===================================================================
//!adds entry or receives a key for the already existing
int MFixedSizeTable::AddOrGetUsed(MVoidArray* pva)
//===================================================================
{
	int iGetUsed = UseRecord(pva);
	int ikeyField = GetKeyField();

	if(iGetUsed!=-1)
		return iGetUsed;
    //If such записис isn't present in the table and the table is crowded
    //that is the last element and a penultimate element are unequal
    //that, alas the additive is impossible
	else{
        //Check on equality of the last and penultimate element
		MIntSize dim = GetDimension();
		bool bEqual = true;
		for(int i = 0;i<dim.cy;i++)
			if(!CompareWith(dim.cx-1,i,m_Table.GetAt(i)->GetAtVoid(dim.cx-2))&&
				(i!=ikeyField))
				bEqual = false;
        //If the last and penultimate records aren't identical,
        //it means the table is crowded and the place for new record is absent
		if(!bEqual)
			return -1;
	}

	int iNewRec = CreateRecord();
	if(iNewRec == -1)
        //The table is crowded
		return iNewRec;
    //CUNNING FOR ACCESS TO THE array ON THE SAME INDEX AS FOR ACCESS TO THE TABLE
	void* temp = NULL;
	pva->InsertBefore(ikeyField,&temp);

	for(int i = 0;i<pva->GetSize();i++){
		if(i!=ikeyField)
			SetCell(iNewRec,i,*pva->GetAt(i));
	}
	pva->Delete(ikeyField);
	return iNewRec;
}


//====================================================================================
//!Initializes the table as the table of colours by default
void MFixedSizeTable::InitalizeAsColorDefault(MTable* pTable,int iColorValueIndex,int iColorNameIndex)
//====================================================================================
{
	if(!(pTable->GetDimension().cx>26))   return; //     exit(15);
	MString str;
	MColor color;
	///////////////////////////////////////////////////////////////////////
    /////filling of the table of flowers with typographical colours////////
	///////////////////////////////////////////////////////////////////////
    //black 2558-01
	color.SetColor(0,0,0);
	pTable->SetCell(0,iColorValueIndex,(void*)&color);
    str = IDS_BLACK; //"black 2558-01"
	pTable->SetCell(0,iColorNameIndex,(void*)&str);
    //White
	color.SetColor(0xff,0xff,0xff);
	pTable->SetCell(1,iColorValueIndex,(void*)&color);
    str = IDS_WHITE; //"White 2558-01"
	pTable->SetCell(1,iColorNameIndex,(void*)&str);

    //Brown 2558-62
	color.SetColor(190,50,5);
	pTable->SetCell(2,iColorValueIndex,(void*)&color);
    str = IDS_BROWN; // "Brown 2558-62"
	pTable->SetCell(2,iColorNameIndex,(void*)&str);
    //Blue 2558-38
	color.SetColor(55,180,170);
	pTable->SetCell(3,iColorValueIndex,(void*)&color);
    str = IDS_DARK_BLUE; //"Blue 2558-38"
	pTable->SetCell(3,iColorNameIndex,(void*)&str);
    //Orange (weakened) 2558-10
	color.SetColor(240,60,5);
	pTable->SetCell(4,iColorValueIndex,(void*)&color);
    str = IDS_ORANGE; //"Orange (weakened) 2558-10"
	pTable->SetCell(4,iColorNameIndex,(void*)&str);
    //Green (weakened) 2558-45
	color.SetColor(220,235,130);
	pTable->SetCell(5,iColorValueIndex,(void*)&color);
    str = IDS_GREEN; //"Green (weakened) 2558-45"
	pTable->SetCell(5,iColorNameIndex,(void*)&str);
    //Brown 2558-61
	color.SetColor(245,95,15);
	pTable->SetCell(6,iColorValueIndex,(void*)&color);
    str = IDS_BROWN_NEW; //"Brown 2558-61"
	pTable->SetCell(6,iColorNameIndex,(void*)&str);
    //Blue 2558-39
	color.SetColor(5,65,125);
	pTable->SetCell(7,iColorValueIndex,(void*)&color);
    str = IDS_DARK_BLUE_NEW; //"Blue 2558-39"
	pTable->SetCell(7,iColorNameIndex,(void*)&str);
    //Turquoise  (weakened) 2558-37
	color.SetColor(140,210,210);
	pTable->SetCell(8,iColorValueIndex,(void*)&color);
    str = IDS_TURQUOISE; //"Turquoise  (weakened) 2558-37"
	pTable->SetCell(8,iColorNameIndex,(void*)&str);
    //Orange (weakened) 2558-11
	color.SetColor(250,95,5);
	pTable->SetCell(9,iColorValueIndex,(void*)&color);
    str = IDS_WEAK_ORANGE; //"Orange (weakened) 2558-11"
	pTable->SetCell(9,iColorNameIndex,(void*)&str);
    //Violet 2558-74
	color.SetColor(195,5,100);
	pTable->SetCell(10,iColorValueIndex,(void*)&color);
    str = IDS_PURPLE;//"Violet 2558-74"
	pTable->SetCell(10,iColorNameIndex,(void*)&str);
    //Dark and olive (blending) 2558-37,-39,-10
	color.SetColor(105,110,90);
	pTable->SetCell(11,iColorValueIndex,(void*)&color);
    str = IDS_DARK_OLIVE; //"Dark and olive (blending) 2558-37,-39,-10"
	pTable->SetCell(11,iColorNameIndex,(void*)&str);
    //Grey and Brown 1st(blending) 2558-62,-38
	color.SetColor(175,120,95);
	pTable->SetCell(12,iColorValueIndex,(void*)&color);
    str = IDS_TAUPE_1; //"Grey and Brown 1st(blending) 2558-62,-38"
	pTable->SetCell(12,iColorNameIndex,(void*)&str);
    //Grey and Brown 2nd(blending) 2558-20,-01,-30
	color.SetColor(120,55,55);
	pTable->SetCell(13,iColorValueIndex,(void*)&color);
    str = IDS_TAUPE_2; //"Grey and Brown 2nd(blending) 2558-20,-01,-30"
	pTable->SetCell(13,iColorNameIndex,(void*)&str);
    //Orange(blending) 2558-12,-10
	color.SetColor(250,180,60);
	pTable->SetCell(14,iColorValueIndex,(void*)&color);
    str = IDS_BLEND_ORANGE; //"Orange(blending) 2558-12,-10"
	pTable->SetCell(14,iColorNameIndex,(void*)&str);
    //black(15%,34lin/sm) 2558-01
	color.SetColor(185,160,115);
	pTable->SetCell(15,iColorValueIndex,(void*)&color);
    str = IDS_BLAK_15pc; //"black(15%,34lin/sm) 2558-01"
	pTable->SetCell(15,iColorNameIndex,(void*)&str);
    //Blue(30%,48lin/sm) 2558-38
	color.SetColor(185,205,205);
	pTable->SetCell(16,iColorValueIndex,(void*)&color);
    str = IDS_DARK_BLUE_30pc; //"Blue(30%,48lin/sm) 2558-38"
	pTable->SetCell(16,iColorNameIndex,(void*)&str);
    //Orange (weakened)(30%,48lin/sm) 2558-10
	color.SetColor(255,165,55);
	pTable->SetCell(17,iColorValueIndex,(void*)&color);
    str = IDS_WEAK_ORANGE_30pc; //"Orange (weakened)(30%,48lin/sm) 2558-10"
	pTable->SetCell(17,iColorNameIndex,(void*)&str);
    //Green (weakened)(50%,34lin/sm) 2558-45
	color.SetColor(235,240,170);
	pTable->SetCell(18,iColorValueIndex,(void*)&color);
    str = IDS_WEAK_GREEN_50pc; //"Green (weakened)(50%,34lin/sm) 2558-45"
	pTable->SetCell(18,iColorNameIndex,(void*)&str);
    //Orange (weakened)(50%,34lin/sm) 2558-10
	color.SetColor(250,190,145);
	pTable->SetCell(19,iColorValueIndex,(void*)&color);
    str = IDS_WEAK_ORANGE_50pc; //"Orange (weakened)(50%,34lin/sm) 2558-10"
	pTable->SetCell(19,iColorNameIndex,(void*)&str);
    //Brown(50%,34lin/sm) 2558-61
	color.SetColor(250,180,135);
	pTable->SetCell(20,iColorValueIndex,(void*)&color);
    str = IDS_BROWN_50pc; //"Brown(50%,34lin/sm) 2558-61"
	pTable->SetCell(20,iColorNameIndex,(void*)&str);
    //black(30%,48lin/sm) 2558-01
	color.SetColor(195,195,195);
	pTable->SetCell(21,iColorValueIndex,(void*)&color);
    str = IDS_BLACK_30pc; //"black(30%,48lin/sm) 2558-01"
	pTable->SetCell(21,iColorNameIndex,(void*)&str);
    //Brown(30%,48lin/sm) 2558-62
	color.SetColor(230,165,135);
	pTable->SetCell(22,iColorValueIndex,(void*)&color);
    str = IDS_BROWN_30pc; //"Brown(30%,48lin/sm) 2558-62"
	pTable->SetCell(22,iColorNameIndex,(void*)&str);
    //Violet(50%,34lin/sm) 2558-74
	color.SetColor(210,100,160);
	pTable->SetCell(23,iColorValueIndex,(void*)&color);
    str = IDS_PURPLE_50pc; //"Violet(50%,34lin/sm) 2558-74"
	pTable->SetCell(23,iColorNameIndex,(void*)&str);
    //Orange(blending)(24lin/sm) 2558-12,-10
	color.SetColor(250,215,160);
	pTable->SetCell(24,iColorValueIndex,(void*)&color);
    str = IDS_BLEND_ORANGE_24; //"Orange(blending)(24lin/sm) 2558-12,-10"
	pTable->SetCell(24,iColorNameIndex,(void*)&str);
    //Orange(blending)(24lin/sm) 2558-10
	color.SetColor(250,170,115);
	pTable->SetCell(25,iColorValueIndex,(void*)&color);
    str = IDS_ORANGE_24; //"Orange(24lin/sm) 2558-10"
	pTable->SetCell(25,iColorNameIndex,(void*)&str);
    //Orange (weakened) 2558-12
	color.SetColor(255,210,100);
	pTable->SetCell(26,iColorValueIndex,(void*)&color);
    str = IDS_WEAK_ORANGE_12; //"Orange (weakened) 2558-12"
	pTable->SetCell(26,iColorNameIndex,(void*)&str);

    //In total other is initialized in the white color
	color.SetColor(0xff,0xff,0xff);
	for(int i = 27;i<=pTable->GetMaxSize();i++)
		pTable->SetCell(i,iColorValueIndex,(void*)&color);
}

//====================================================================================
//!Initializes the table as the table of colours by default
void MFixedSizeTable::InitalizeAsCMYKDefault(MTable* pTable,int iColorValueIndex,int iColorNameIndex)
//====================================================================================
{
	if(!(pTable->GetDimension().cx>4)) return;  // exit(15);
	MString str;
	MColor color;
	///////////////////////////////////////////////////////////////////////
    //////filling of the table of colours with typographical colours///////
	///////////////////////////////////////////////////////////////////////
	color.SetColor(255,0,0);
	pTable->SetCell(0,iColorValueIndex,(void*)&color);
    str = "Red R-255";
	pTable->SetCell(0,iColorNameIndex,(void*)&str);

	color.SetColor(0,255,0);
	pTable->SetCell(1,iColorValueIndex,(void*)&color);
    str = "Green G-255";
	pTable->SetCell(1,iColorNameIndex,(void*)&str);

	color.SetColor(0,0,255);
	pTable->SetCell(2,iColorValueIndex,(void*)&color);
    str = "Blue B-255";
	pTable->SetCell(2,iColorNameIndex,(void*)&str);

	color.SetColor(0,174,239);
	pTable->SetCell(3,iColorValueIndex,(void*)&color);
	str = "Cyan 0,174,239";
	pTable->SetCell(3,iColorNameIndex,(void*)&str);

	color.SetColor(236,0,140);
	pTable->SetCell(4,iColorValueIndex,(void*)&color);
	str = "Magenta 236,0,140";
	pTable->SetCell(4,iColorNameIndex,(void*)&str);

	color.SetColor(255,242,0);
	pTable->SetCell(5,iColorValueIndex,(void*)&color);
	str = "Yellow 255,242,0";
	pTable->SetCell(5,iColorNameIndex,(void*)&str);

	color.SetColor(55,180,170);
	pTable->SetCell(6,iColorValueIndex,(void*)&color);
	str = "Black 55,180,170";
	pTable->SetCell(6,iColorNameIndex,(void*)&str);

	color.SetColor(0,0,0);
	pTable->SetCell(7,iColorValueIndex,(void*)&color);
    str = "Black RGB";
	pTable->SetCell(7,iColorNameIndex,(void*)&str);

    //In total other is initialized in the white color
	color.SetColor(0xff,0xff,0xff);
	for(int i = 8;i<=pTable->GetMaxSize();i++)
		pTable->SetCell(i,iColorValueIndex,(void*)&color);
}



//================================================================================
//!Function on replacement of all records made earlier
/**ALGORITHM OF REPLACEMENT FOLLOWING,
//AT FIRST REPLACEMENT OF THOSE ELEMENTS WHICH CODES AREN'T REQUIRED FOR FURTHER IS MADE
//REPLACEMENTS (THAT IS THE CODE EXISTS AS APPOINTMENT BUT DOESN'T EXIST AS THE SOURCE)
//AFTER EACH OF REPLACEMENTS THE CYCLE IS REPEATED

//THE SECOND STAGE CONSISTS IN THAT THAT ONE OF SOURCES IS OVERTAKEN IN THE FREE CODE
//IT IS THE CODE WHICH WAS REPLACED AT THE FIRST STAGE OR THE CODE WHICH HAS TO
//TO BE RECEIVED FROM THE classifier ON CONDITION THAT IT ISN'T USED
*/
void MCodeChange::ApplyChangeRecords(MKlassifikator* pKls,TableToChangeKod whatTheTable)
//================================================================================
{
	int iLastFreeKod = -1;
	CodeChangeRecord rec;
	for(int i = 0;i<m_records.GetSize();i++){
        //Basis cycle on all records
        //If the appointment code for any record isn't among codes of sources
        //that it is possible to make replacement freely
		rec = *m_records.GetAt(i);
        //If the source is equal to appointment
		if(rec.iChangeFrom==rec.iChangeTo){
			m_records.Delete(i);i=-1;continue;};
        //Check on existence of this code of appointment among source codes
		bool bSource = false;
		for(int iS = 0;iS<m_records.GetSize();iS++)
			if(m_records.GetAt(iS)->iChangeFrom == rec.iChangeTo){
				bSource = true;
			}
		if(!bSource){
			ChangeKods(pKls,whatTheTable,i);
            //Sources also changed we begin everything with the beginning
			iLastFreeKod = rec.iChangeFrom;
			i = -1;
			continue;
		}
		if(i==m_records.GetSize()-1){
            //If entered this cycle means came to such situation,
            //something that is in a source that is and in appointment
            //if the free code was received earlier
			if(iLastFreeKod!=-1){
				rec = *m_records.GetHead();
				CodeChangeRecord newRec(iLastFreeKod,rec.iChangeTo);
				rec.iChangeTo = iLastFreeKod;
                //We make replacement with participation of a code free
				ChangeKods(pKls,whatTheTable,0);
				m_records.AddHead(&newRec);
				iLastFreeKod = -1;
                //also we begin a nonsense from the very beginning
				i = -1;
				continue;
			}
			else{
                //Here receiving a free code from the classifier is necessary
                //and replacement by it
				int iKod = GetFreeCod(pKls,whatTheTable);
				if(iKod != -1){
					rec = *m_records.GetHead();
					CodeChangeRecord newRec(iKod,rec.iChangeTo);
					rec.iChangeTo = iKod;
//					iKod = -1;
					m_records.AddHead(&newRec);
					//и начинаем бодягу с самого начала
					i = -1;
					continue;
				}
			}

            //If at all here send that situation by incurably easy way
            InternalMessage(IDS_BAD_KLS_CONCT);//"INCORRECT CLASSIFIER ATTACHMENT (Code Error)"
			ChangeKods(pKls,whatTheTable,i);
		}
	}
	m_records.ClearAll();
}




//===========================================================================
//!Actually replacement, depending on the table
//! Number of entry in the table of transformations is transferred
void MCodeChange::ChangeKods(MKlassifikator* pKls,TableToChangeKod whatTheTable,int iRec)
//===========================================================================
{
	CodeChangeRecord rec = *m_records.GetAt(iRec);
	switch(whatTheTable){
	case ID_COLOR_TABLE:
			pKls->ChangeColor((unsigned char)rec.iChangeFrom,(unsigned char)rec.iChangeTo);
			pKls->ChangePrkColorInPrkDict((unsigned char)rec.iChangeFrom,(unsigned char)rec.iChangeTo);
		break;
	case ID_ATTR_TABLE:
			pKls->ChangeCharakteristika((unsigned short)rec.iChangeFrom,(unsigned short)rec.iChangeTo);
		break;
	case ID_MEGSTR_TABLE:
			pKls->ChangeMegaStroka((unsigned char)rec.iChangeFrom,(unsigned char)rec.iChangeTo);
		break;
	case ID_PRK_TABLE:
			pKls->ChangePocherk((unsigned char)rec.iChangeFrom,(unsigned char)rec.iChangeTo);
		break;
	case ID_FONT_TABLE:
			pKls->ChangeFont((unsigned short)rec.iChangeFrom,(unsigned short)rec.iChangeTo);
		break;
	case ID_EDIZM_TABLE:
			pKls->ChangeEdIzm((unsigned short)rec.iChangeFrom,(unsigned short)rec.iChangeTo);
		break;
//	default:		exit(15);
	}
	m_records.Delete(iRec);
}

//================================================================================
//!Receiving a free code for the concrete table
int MCodeChange::GetFreeCod(MKlassifikator* pKls,TableToChangeKod whatTheTable)
//================================================================================
{
	int iFreeKod = 0;
	int iStartFrom;
    //we build the general cycle on all classifier
	do{
		iStartFrom = iFreeKod;
		MZsArray* pzsar = pKls->GetZsArray();
		for(int izs = 0;izs<pzsar->GetSize();izs++){
			MRlArray* prlar = pzsar->GetAt(izs)->GetRazdelArray();
			for(int irl = 0 ;irl<prlar->GetSize();irl++){
				MZnArray* pznar = prlar->GetAt(irl)->GetZnakArray();
				for(int izn = 0;izn<pznar->GetSize();izn++){
					MZnak* pznak = pznar->GetAt(izn);
					switch(whatTheTable){
					case ID_COLOR_TABLE:
						{
							if(pznak->IsColorkod(iFreeKod))
								iFreeKod++;
							if(iFreeKod>=pKls->GetColorTable()->GetMaxSize())
								return -1;
							continue;
						}
					case ID_ATTR_TABLE:
						{
							if(pznak->IsCharkod(iFreeKod))
								iFreeKod++;
							if(iFreeKod>=pKls->GetAttrTable()->GetMaxSize())
								return -1;
							continue;
						}
					case ID_MEGSTR_TABLE:
						{
							if(pznak->IsMegstrkod(iFreeKod))
								iFreeKod++;
							if(iFreeKod>=pKls->GetMegStrTable()->GetMaxSize())
								return -1;
							continue;
						}
					case ID_PRK_TABLE:
						{
							if(pznak->IsPrkkod(iFreeKod))
								iFreeKod++;
							if(iFreeKod>=pKls->GetPrkTable()->GetMaxSize())
								return -1;
							continue;
						}
					case ID_FONT_TABLE:   break;
					case ID_EDIZM_TABLE:  break;
					default:              break; //exit(15);
  				}
				}
			}
		}	
        //Also the exit at impossibility to find the solution is just in case necessary
        //The code exceeded the table size
	}while(iStartFrom == iFreeKod);
	return iFreeKod;
}

// ==================================================================
//	Applies reassignment and udalayat initial records from the table
//! --- 
void MCodeChange::ApplyAndRemove(MKlassifikator* pKls,TableToChangeKod whatTheTable)
{
	MTable* pTable;
		switch(whatTheTable){
		case ID_COLOR_TABLE:
			pTable = pKls->GetColorTable();break;
		case ID_PRK_TABLE:
			pTable = pKls->GetPrkTable();break;
 		default:		break; //	exit(15);
		}
	for(int i = m_records.GetSize()-1;i>=0;i--)
		pTable->DeleteRecord(m_records.GetAt(i)->iChangeFrom);
	ApplyChangeRecords(pKls,whatTheTable);
}
