#include "Object.h"
#include "Karta.h"
#include "Klassifikator.h"
#include "privpoints.h"
#include "PlotDevice.h"
#include "Refglobal.h"

extern MObject* (*fpCreateObjectByType)(MString str);
//===============================================================================
// MObject
//===============================================================================
//! MObject
MObject::MObject()
//==================================
{
    //by default
	m_Komplekt=new MArArray();
	m_pLay = NULL;
	m_selected = false;
	m_sel_color = MColor(0, 255, 0);
	m_trans_type = Qt::SolidPattern; //added by back
	m_enabled    = true;
 
	m_UID        = 0;
  	m_UID_master = 0;
}
 
//!==================================
MObject::~MObject()
//==================================
{
	m_dbarray.ClearAll();

    //delete 'm_Komplekt'
	if(m_Komplekt) {
		m_Komplekt->FreeMemory();
		delete m_Komplekt;
	}
}

//====================================================================
//!Reading from an UTP-format
bool  MObject::ReadBin(FILE* h_nuxfile,  int version)
//====================================================================
{

	switch( version ) {
        //version 0
		case 0:
            //we pass record length
			fseek(h_nuxfile, 4, SEEK_CUR);

			if(GetIerarhy().Find(ID_TOPOBJECT)!=-1)
			{	
                //we read quad-code
				((MTopobject*)this)->m_kkod = ReadString(h_nuxfile, version);
			}
			else
				fseek(h_nuxfile, 2, SEEK_CUR);

			unsigned int uid= 0;
      
			fread(&uid,sizeof(uid), 1, h_nuxfile);
if (reverseReadFlag) reverseValue((char*)&uid, sizeof(uid) );
			m_UID = uid;
if (utpAdvancedFlag)  {
			fread(&uid,sizeof(uid), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&uid, sizeof(uid) );
			m_UID_master = uid;
}      
			m_editstat = 0;
			fread(&m_editstat,sizeof(m_editstat), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_editstat, sizeof(m_editstat) );
            //we read  'db_array'
			int kolvo = 0;
			fread(&kolvo, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
            m_dbarray.SetSize(kolvo); // for each object dimension of the 'm_dbarray' array
                                      // has to correspond to number of the lines which are read out in it
            //we come back to read the array of lines
			fseek(h_nuxfile, -4, SEEK_CUR);
            // reading array of lines
			ReadStrArray(h_nuxfile,  version, &m_dbarray);

            //reading the describing rectangle
			ReadRect(h_nuxfile,  version, &m_bounds);
			
            //reading 'm_Komplekt'
			m_Komplekt->FreeMemory();
			m_Komplekt->ReadBin(h_nuxfile,version);
	}
	return true;
}

//====================================================================
//!Reading from an UTP-format
bool  MObject::ReadBin(MFile* file,  int version)
//====================================================================
{

	switch( version ) {
        //version 0
		case 0:
            //we pass record length
			file->SetPosition(4, MAP_SEEK_CUR);

			if(GetIerarhy().Find(ID_TOPOBJECT)!=-1)
			{	
                //we read quad-code
				((MTopobject*)this)->m_kkod = ReadString(file, version);
			}
			else
				file->SetPosition(2, MAP_SEEK_CUR);

			unsigned int uid= 0;
			file->GetData(&uid, sizeof(uid), 1);
if (reverseReadFlag) reverseValue((char*)&uid, sizeof(uid) );
			m_UID = uid;
if (utpAdvancedFlag)  {
			file->GetData(&uid, sizeof(uid), 1);
		if (reverseReadFlag) reverseValue((char*)&uid, sizeof(uid) );
			m_UID_master = uid;
}
      
			m_editstat = 0;
			file->GetData(&m_editstat,sizeof(m_editstat), 1);
		if (reverseReadFlag) reverseValue((char*)&m_editstat, sizeof(m_editstat) );

            //we read  'db_array'
			int kolvo = 0;
			file->GetData(&kolvo, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
            m_dbarray.SetSize(kolvo); // for each object dimension of the 'm_dbarray' array
                                         // has to correspond to number of the lines which are read out in it
            //we come back to read the array of lines
			file->SetPosition(-4, MAP_SEEK_CUR);
            // reading array of lines
			ReadStrArray(file,  version, &m_dbarray);

            //reading the describing rectangle
			ReadRect(file,  version, &m_bounds);
			
            //reading 'm_Komplekt'
			m_Komplekt->FreeMemory();
			m_Komplekt->ReadBin(file, version);
	}
	return true;
}

//======================================================================
//!Function of writing in UTP
bool  MObject::WriteBin(FILE* h_nuxfile,  int version)
//======================================================================
{
	
    //int			i;
    //Calculation of the extent of object in UTP
	unsigned int SBytes = GetBytes();

    //writing
	switch( version ) {
        //version 0
		case 0:
            //at first we will write down the object identifier
			fwrite((char *)WhatTheObject(), 3, 1, h_nuxfile);
            //i=0;
            //writing object lengths
			fwrite(&SBytes, 4, 1, h_nuxfile);

			if(GetIerarhy().Find(ID_TOPOBJECT)!=-1)
			{	
                //we write down quad-code
				WriteString(h_nuxfile, version, ((MTopobject*)this)->m_kkod);
			}
			else
			{
				short int j = 0;
				fwrite(&j, 2, 1, h_nuxfile);
			}
//      cout << (int) m_UID<< " - " << (int)m_UID_master<< endl;

			unsigned int uid = (unsigned int)m_UID;
			fwrite(&uid, 4, 1, h_nuxfile);
if (utpAdvancedFlag)  {
      uid = (unsigned int)m_UID_master;
			fwrite(&uid, 4, 1, h_nuxfile);
}
			fwrite(&m_editstat, 4, 1, h_nuxfile);
            //writing db_array'a
			WriteStrArray(h_nuxfile,  version, &m_dbarray);
            //writing of the describing rectangle
			WriteRect(h_nuxfile,  version, &m_bounds);
            //writing m_Komplekt'a
			m_Komplekt->WriteBin(h_nuxfile,version);
	}
	return true;
};

//!==================================
MObject::MObject(const MRect& bounds)
//==================================
{
	m_dbarray.SetSize(ID_DBINITSIZE);
	m_bounds = bounds;
	m_Komplekt=new MArArray();
	m_pLay = NULL;
	m_selected = false;
	m_trans_type = Qt::SolidPattern; //added by back
	m_sel_color = MColor(0, 255, 0);
    m_enabled = true;
}

//!==================================
MKarta* MObject::GetKarta() 
//==================================
{
	if (m_pLay==NULL) return NULL;
	else return m_pLay->GetKarta();
}

//==================================
//! installation of a sign of degree of transparency
void MObject::SetTransType(int type)
{
	if ((type>=0)&&(type<=14))
		m_trans_type=type;
} 

//==================================
//!Drawing of object the simplified
void MObject::Draw1(MPlotDevice* pDevice)
{
    // at whom from successors this function isn't replaced,
    //at that usual function of drawing is caused
	Draw(pDevice);
};


//!==================================
/** affiliated classes are responsible for copying
//MObject does only drawing of the text at
//to a tag (see the DrawText function below...)
*/
void MObject::Draw(MPlotDevice* /*pDevice*/)
//==================================
{
//	exit(15);
}

//==================================
//! affiliated classes are responsible for copying
void MObject::DrawText(MPlotDevice* /*pDevice*/)
//==================================
{  
}
//==================================
//! drawing of a marking of top object
void MObject::DrawTracker(MPlotDevice* pDevice,TrackerState state)
//==================================
{
	switch (state)
	{
	case normal:
		{
			MPtArray* pta;
			MPoint* pt;
			MArray* par;
			int nCount=m_Komplekt->GetSize();
			double dOnePix = pDevice->LogicToBum(1);
			double dTwoPix = dOnePix*2;
			double dThreePix = dOnePix*3;
			double dFivePix = dOnePix*5;
			for(int i=0;i<nCount;i++)
			{
				par=m_Komplekt->GetAt(i);
				if(par->WhatTheArray()==ID_PTA)
				{
					pta=(MPtArray*)par;
					int Count=pta->GetSize();
					for(int j=0;j<Count;j++)
					{
						pt=pta->GetAt(j);
						pDevice->PatBlt(pt->x - dTwoPix, pt->y - dTwoPix, dFivePix, dFivePix, BLACKNESS);
						pDevice->PatBlt(pt->x - dOnePix, pt->y - dOnePix, dThreePix, dThreePix, WHITENESS);		
					}
				}	
			}
		break;
		}
	default: break;
	}
}
//==================================
//! movement of object as whole
//! (or its brands)
void MObject::MoveTo(MPoint& delta)
//==================================
{
		Invalidate();
		m_bounds += delta;
		Invalidate();

    //We move all dot arrays
	int i;
	for (i=m_Komplekt->GetSize()-1;i>-1;i--)
	{
		if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PTA || m_Komplekt->GetAt(i)->WhatTheArray()==ID_PHL)
		{
			MPtArray* pta=(MPtArray*)(m_Komplekt->GetAt(i));
			int j;
			for(j=pta->GetSize()-1;j>-1;j--)
			{
				MPoint* pt = pta->GetAt(j);
				pt[0]+=delta;
			}
		}
		else if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_ZBK)
		{
			MZbArray* PAr=(MZbArray*)m_Komplekt->GetAt(i);
			MZarubka* zbk;
            //we scale each point of this array
			for(int j=PAr->GetSize()-1;j>-1;j--)
			{
				zbk=PAr->GetAt(j);
				zbk->m_pt+=delta;
			}
		}
		else if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_RZV)
		{
			MRzArray* PAr=(MRzArray*)m_Komplekt->GetAt(i);
			MRazryv* razr;
            //we scale each point of this array
			for(int j=PAr->GetSize()-1;j>-1;j--)
			{
				razr=PAr->GetAt(j);
				razr->m_ptfirst+=delta;
				razr->m_ptlast+=delta;
			}
		}
		else if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PVA){
            //Shift of all points at a certain size
				MPvArray* pvar= (MPvArray*)m_Komplekt->GetAt(i);
				for(int pvi = pvar->GetSize()-1;pvi>=0;pvi--){
					MPriv* pPriv = pvar->GetAt(pvi);
					unsigned short iPtNum = pPriv->GetPointNum();
					MPoint *pta = pPriv->GetPoints();
					for(unsigned short pvj = 0;pvj<iPtNum;pvj++)
						pta[pvj]+=delta;
				}
		}
	}
}

//==================================
//! movement of object as whole
//! (or its brands)
void MObject::MoveTo(double distance, double azimut)
//==================================
{
	MPoint pt1(m_bounds.left, m_bounds.top);
	MPoint pt2(m_bounds.right, m_bounds.bottom);
	pt1 = GetPointByAzD(pt1, azimut, distance);
	pt2 = GetPointByAzD(pt2, azimut, distance);
	m_bounds = MRect(pt1.x, pt1.y, pt2.x, pt2.y);
	

    //We move all dot arrays
	int i;
	for (i=m_Komplekt->GetSize()-1;i>-1;i--)
	{
		if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PTA || m_Komplekt->GetAt(i)->WhatTheArray()==ID_PHL)
		{
			MPtArray* pta=(MPtArray*)(m_Komplekt->GetAt(i));
			int j;
			for(j=pta->GetSize()-1;j>-1;j--)
			{
				MPoint* pt = pta->GetAt(j);
				*pt = GetPointByAzD(*pt, azimut, distance);
			}
		}
		else if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_ZBK)
		{
			MZbArray* PAr=(MZbArray*)m_Komplekt->GetAt(i);
			MZarubka* zbk;
            //we scale each point of this array
            for(int j=PAr->GetSize()-1;j>-1;j--)
			{
				zbk=PAr->GetAt(j);
				zbk->m_pt = GetPointByAzD(zbk->m_pt, azimut, distance);
			}
		}
		else if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_RZV)
		{
			MRzArray* PAr=(MRzArray*)m_Komplekt->GetAt(i);
			MRazryv* razr;
            //we scale each point of this array
            for(int j=PAr->GetSize()-1;j>-1;j--)
			{
				razr=PAr->GetAt(j);
				razr->m_ptfirst = GetPointByAzD(razr->m_ptfirst, azimut, distance);
				razr->m_ptlast = GetPointByAzD(razr->m_ptlast, azimut, distance);
			}
		}
		else if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PVA){
            //Shift of all points at a certain size
			MPvArray* pvar= (MPvArray*)m_Komplekt->GetAt(i);
			for(int pvi = pvar->GetSize()-1;pvi>=0;pvi--){
				MPriv* pPriv = pvar->GetAt(pvi);
				unsigned short iPtNum = pPriv->GetPointNum();
				MPoint *pta = pPriv->GetPoints();
				for(unsigned short pvj = 0;pvj<iPtNum;pvj++)
					pta[pvj]=GetPointByAzD(pta[pvj], azimut, distance);
			}
		}
	}
}

//==================================
//! test: where specifies the mouse pointer?
//! serves for all types of top objects, except hyper points
bool MObject::HitTest(MPoint point, double dDostup,MPtArray*& ptarr,int& index)
//==================================
{
	MPtArray* pta;
	MPoint* pt;
	MArray* par;
	int nCount=m_Komplekt->GetSize();
	for(int i=0;i<nCount;i++)
	{
		par=m_Komplekt->GetAt(i);
		if(par->WhatTheArray()==ID_PTA)
		{
			pta=(MPtArray*)par;
			int Count=pta->GetSize();
			for(int j=0;j<Count;j++)
			{
				pt=pta->GetAt(j);
				if ((fabs(point.x - pt->x)<= dDostup)&&
					(fabs(point.y - pt->y) <= dDostup))
				{
					index=j;
					ptarr=pta;
					return true;
				}
			}
		}
	}
	index=-1;
	ptarr=NULL;
	return false;
}

//=========================================================================
//!Inserts 'pt' point into the array of points of 'pta' before a point with the 'index' index
void MObject::InsertPointBefore(MPoint pt,MPtArray* pta, int index)
//=========================================================================
{
	if((index<0)||(index>=pta->GetSize()))return;
	pta->InsertBefore(index,&pt);
}
//=========================================================================
//!Inserts 'pt' point into the array of points of 'pta' after a point with the 'index' index
void MObject::InsertPointAfter(MPoint pt,MPtArray* pta, int index)
//=========================================================================
{
	if((index<0)||(index>=pta->GetSize()))return;
	pta->InsertAfter(index,&pt);
}
//===========================================================
//!Deletes a point from the 'pta' array with the 'index' index
void MObject::DeletePoint(MPtArray* pta, int index)
//============================================================
{
	if((index<0)||(index>=pta->GetSize()))return;
	pta->Delete(index);
}

//===========================================================
//!Adds 'pt' point to the end of the 'pta' array
void MObject::AddPoint(MPoint pt,MPtArray* pta)
//============================================================
{
	pta->AddTail(&pt);
}

//==================================
//! test: whether inside this point of a rectangle of our object?
bool MObject::Intersects(const MPoint& point)
//==================================
{
	if ( (m_bounds.left <= point.x)&&
	     (m_bounds.right >= point.x)&&
	     (m_bounds.top >= point.y)&&
	     (m_bounds.bottom <= point.y))
	return true;
	else return false;
}

//==================================
//! Returns distance from the set point to the CENTER of ours of top object.
double MObject::FindDistanceFromPoint(MPoint& gpt)
//==================================
{ 
	return sqrt((gpt.x - m_bounds.CenterX())*(gpt.x - m_bounds.CenterX())+(gpt.y - m_bounds.CenterY())*(gpt.y - m_bounds.CenterY()));
}

//==================================
//! Fictitious function for MObject,
/** actually it makes sense only for MTopobject and descendants.
//Allegedly we will find a point of intersection of ours of top object from some straight line,
// set by two points 'linept1' and 'linept2'.
//Actually for MObject always returns a bad code of return.
*/
//(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto)
int MObject::FindNearestIntersectWithLine(MPoint , MPoint , MPoint& , double& )
//==================================
{
	return 1;
}

//==================================
//! test: whether covers the rectangle of our object given a rectangle?
bool MObject::Intersects(const MRect& rect)
//==================================
{
	if ( (m_bounds.left >= rect.left)&&
	     (m_bounds.right <= rect.right)&&
	     (m_bounds.top <= rect.top)&&
	     (m_bounds.bottom >= rect.bottom))
	return true;
	else return false;
}

//=============================================
//! Returns the list of the subordinated objects
void MObject::GetChildList(MObjectList& utlist)

//=============================================
{
	utlist.clear();
}

//================================================
//! function of determination of the extent of object in the UTP file
unsigned int MObject::GetBytes()
//================================================
{
    //Calculation of the extent of object in UTP
    //The extent of object in UTP is calculated thus:
    //3 bytes on record of the identifier
    //+ 4 bytes on record of the size
    //+ size of the array of lines
    //+ 4 bytes size of unique number
    //+ 4 bytes size of the status of editing
    //+ 32 bytes on the size of the describing rectangle
    //+ array of personal information (m_Komplekt)

    int SBytes = 3;//3 bytes on record of the identifier
    SBytes += 4;//4 bytes on record of the size

    SBytes += 2;//2 bytes on the size of a quad-code
	SBytes+=4;
	SBytes+=4;
    //'db_array' array size
	SBytes+=4;
	for(int i=0; i<m_dbarray.GetSize(); i++)
		SBytes += m_dbarray.GetAt(i)->GetLength() + 2;
    //'m_bounds' size
	SBytes+=32;
    //size of individual information
	SBytes+=m_Komplekt->GetBytes();//Размер m_Komplekt'а
	return SBytes;
}

//==================================
//! how many points in a marking?
int MObject::GetHandleCount()
//==================================
{
    // by default eight - angles and the middle of the parties
	return 8;
}

//==================================
//! returns geocoordinates of a n-th marking point
MPoint MObject::GetHandle(int nHandle)
//==================================
{
	double x, y, xCenter, yCenter;

	xCenter = (m_bounds.left + m_bounds.right) / 2;
	yCenter = (m_bounds.top + m_bounds.bottom) / 2;

	switch (nHandle)
	{
	default:
//		exit(15);

	case 0:
		x = m_bounds.left;
		y = m_bounds.top;
		break;

	case 1:
		x = xCenter;
		y = m_bounds.top;
		break;

	case 2:
		x = m_bounds.right;
		y = m_bounds.top;
		break;

	case 3:
		x = m_bounds.right;
		y = yCenter;
		break;

	case 4:
		x = m_bounds.right;
		y = m_bounds.bottom;
		break;

	case 5:
		x = xCenter;
		y = m_bounds.bottom;
		break;

	case 6:
		x = m_bounds.left;
		y = m_bounds.bottom;
		break;

	case 7:
		x = m_bounds.left;
		y = yCenter;
		break;
	}

	return MPoint(x, y);
}



//==================================
//! carries out transformation of turn in affiliated classes
void MObject::Rotate(double A, double B, double C1, double C2)
//==================================
{	
    //if m_Komplekt is not NULL
	if(m_Komplekt)
	{
		MArray* ar;
		int i, j;
        //We are run on all arrays
		for(i=m_Komplekt->GetSize()-1;i>-1;i--)
		{
			ar=m_Komplekt->GetAt(i);
            //if the next array - the array of points
			if(ar->WhatTheArray()==ID_PTA)
			{
				MPtArray* PAr=(MPtArray*)ar;
				MPoint* pt;
                //we turn each point of this array
				for(j=PAr->GetSize()-1;j>-1;j--)
				{
					pt=PAr->GetAt(j);
					pt->Rotate( A, B, C1, C2);
				}
			}
			else if(ar->WhatTheArray()==ID_RZV)
			{
				MRzArray* PAr=(MRzArray*)ar;
				MRazryv* razr;
                //we scale each point of this array
				for(j=PAr->GetSize()-1;j>-1;j--)
				{
					razr=PAr->GetAt(j);
					razr->m_ptfirst.Rotate( A, B, C1, C2);
					razr->m_ptlast.Rotate( A, B, C1, C2);
				}
			}
			else if(ar->WhatTheArray()==ID_ZBK)
			{
				MZbArray* PAr=(MZbArray*)ar;
				MZarubka* zbk;
                //we scale each point of this array
				for(j=PAr->GetSize()-1;j>-1;j--)
				{
					zbk=PAr->GetAt(j);
					zbk->m_pt.Rotate( A, B, C1, C2);
				}
			}
			else if(ar->WhatTheArray() == ID_PVA){
				MPvArray* par=(MPvArray*)ar;
				for(int pvi = par->GetSize()-1;pvi>=0;pvi--){
					MPriv* pPriv = par->GetAt(pvi);
					unsigned short pointNum = pPriv->GetPointNum();
					MPoint* pta = pPriv->GetPoints();
					for(int pvj = 0;pvj<pointNum;pvj++)
						pta[pvj].Rotate(A,B,C1,C2);
				}
			}
		}
		RecalcBounds();
	}	
	
	//if(WhatTheObject() != ID_SCR)	
	//		m_mark.Rotate( A, B, C1, C2);
}

//==================================
//! carries out transformation of scaling in affiliated classes
void MObject::Scale(double K, double XC, double YC)
//==================================
{
    //if m_Komplekt is not NULL
	if(m_Komplekt)
	{
		MArray* ar;
		int i, j;
        //We are run on all arrays
        for(i=m_Komplekt->GetSize()-1;i>-1;i--)
		{
			ar=m_Komplekt->GetAt(i);
            //if the next array - the array of points
            if(ar->WhatTheArray()==ID_PTA)
			{
				MPtArray* PAr=(MPtArray*)ar;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					pt=PAr->GetAt(j);
					pt->Scale(K,XC,YC);
				}
			}
			else if(ar->WhatTheArray()==ID_RZV)
			{
				MRzArray* PAr=(MRzArray*)ar;
				MRazryv* razr;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					razr=PAr->GetAt(j);
					razr->m_ptfirst.Scale(K,XC,YC);
					razr->m_ptlast.Scale(K,XC,YC);
				}
			}
			else if(ar->WhatTheArray()==ID_ZBK)
			{
				MZbArray* PAr=(MZbArray*)ar;
				MZarubka* zbk;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					zbk=PAr->GetAt(j);
					zbk->m_pt.Scale(K,XC,YC);
				}
			}
			else if(ar->WhatTheArray() == ID_PVA){
				MPvArray* par=(MPvArray*)ar;
				for(int pvi = par->GetSize()-1;pvi>=0;pvi--){
					MPriv* pPriv = par->GetAt(pvi);
					unsigned short pointNum = pPriv->GetPointNum();
					MPoint* pta = pPriv->GetPoints();
					for(int pvj = 0;pvj<pointNum;pvj++)
						pta[pvj].Scale(K,XC,YC);
				}
			}
		}
		RecalcBounds();
	}
	
}



//==================================
//! affiliated classes are responsible for recounting of the describing rectangle
bool MObject::RecalcBounds()

//==================================
{
	return true;
}

//==================================
//! updating of object
void MObject::SetMbounds(MRect m_userRect)
//==================================
{
    m_bounds = m_userRect;
}


//==================================
//! updating of object
void MObject::Invalidate()
//==================================
{
}

//==================================
//! clonning
MObject* MObject::Clone()
//==================================
{
	MObject* pClone = new MObject(m_bounds);
	pClone->m_dbarray = *(MStArray *)m_dbarray.Clone();
	delete pClone->m_Komplekt;
	
	pClone->m_Komplekt = (MArArray*)m_Komplekt->Clone();
	
	return pClone;
}

//===================================================
//!We carry out all necessary operations for cloning
void MObject::MasterClone(MObject* pClone)
//====================================================
{
	pClone->m_dbarray = *(MStArray *)m_dbarray.Clone();
	pClone->m_Komplekt->FreeMemory();
	delete pClone->m_Komplekt;
	
	pClone->m_Komplekt=(MArArray*)m_Komplekt->Clone();
}

//==================================
//! carries out linear transformation in affiliated classes
void MObject::LinPr()
//==================================
{
    //if m_Komplekt is not NULL
	if(m_Komplekt)
	{
		MArray* ar;
		int i, j;
        //We are run on all arrays
        for(i=m_Komplekt->GetSize()-1;i>-1;i--)
		{
			ar=m_Komplekt->GetAt(i);
            //if the next array - the array of points
            if(ar->WhatTheArray()==ID_PTA)
			{
				MPtArray* PAr=(MPtArray*)ar;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					pt=PAr->GetAt(j);
					MakeLinPreobr(pt);
				}
			}
			else if(ar->WhatTheArray()==ID_RZV)
			{
				MRzArray* PAr=(MRzArray*)ar;
				MRazryv* razr;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					razr=PAr->GetAt(j);
					MakeLinPreobr(&razr->m_ptfirst);
					MakeLinPreobr(&razr->m_ptlast);
				}
			}
			else if(ar->WhatTheArray()==ID_PVA)
			{
				MPvArray* PAr=(MPvArray*)ar;
				MPriv* priv;
				int cnt;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					priv=PAr->GetAt(j);
					pt = priv->GetPoints();
					for(cnt = priv->GetPointNum()-1;cnt>-1;cnt--)
					{
						MakeLinPreobr(&pt[cnt]);
					}
				}
			}
			else if(ar->WhatTheArray()==ID_ZBK)
			{
				MZbArray* PAr=(MZbArray*)ar;
				MZarubka* zarubka;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					zarubka=PAr->GetAt(j);
					MakeLinPreobr(&zarubka->m_pt);
				}
			}

		}

		RecalcBounds();
	}
	
}

//==================================
//! carries out linear transformation in affiliated classes
void MObject::LinPrKompoz()
//==================================
{
    //if m_Komplekt is not NULL
	if(m_Komplekt)
	{
		MArray* ar;
		int i, j;
        //We are run on all arrays
		for(i=m_Komplekt->GetSize()-1;i>-1;i--)
		{
			ar=m_Komplekt->GetAt(i);
            //if the next array - the array of points
			if(ar->WhatTheArray()==ID_PTA || ar->WhatTheArray()==ID_PHL)
			{
				MPtArray* PAr=(MPtArray*)ar;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					pt=PAr->GetAt(j);
					KompozLinPreobr(pt);
				}
			}
			else if(ar->WhatTheArray()==ID_RZV)
			{
				MRzArray* PAr=(MRzArray*)ar;
				MRazryv* razr;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					razr=PAr->GetAt(j);
					KompozLinPreobr(&razr->m_ptfirst);					
					KompozLinPreobr(&razr->m_ptlast);
				}
			}
			else if(ar->WhatTheArray()==ID_PVA)

			{
				MPvArray* PAr=(MPvArray*)ar;
				MPriv* priv;
				int cnt;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					priv=PAr->GetAt(j);
					pt = priv->GetPoints();
					for(cnt = priv->GetPointNum()-1;cnt>-1;cnt--)
					{
						KompozLinPreobr(pt+cnt);
					}
				}
			}
			else if(ar->WhatTheArray()==ID_ZBK)
			{
				MZbArray* PAr=(MZbArray*)ar;
				MZarubka* zarubka;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					zarubka=PAr->GetAt(j);
					KompozLinPreobr(&zarubka->m_pt);
				}
			}
		}
		RecalcBounds();
	}
	
}

//! ------------
void MObject::LinPrByCoef()
//==================================
{
    //if m_Komplekt is not NULL
	if(m_Komplekt)
	{
		MArray* ar;
		int i, j;
        //We are run on all arrays
		for(i=m_Komplekt->GetSize()-1;i>-1;i--)
		{
			ar=m_Komplekt->GetAt(i);
            //if the next array - the array of points
			if(ar->WhatTheArray()==ID_PTA || ar->WhatTheArray()==ID_PHL)
			{
				MPtArray* PAr=(MPtArray*)ar;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					pt=PAr->GetAt(j);
					LinPreobrByCoef(pt);
				}
			}
			else if(ar->WhatTheArray()==ID_RZV)
			{
				MRzArray* PAr=(MRzArray*)ar;
				MRazryv* razr;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					razr=PAr->GetAt(j);
					LinPreobrByCoef(&razr->m_ptfirst);
					LinPreobrByCoef(&razr->m_ptlast);
				}
			}
			else if(ar->WhatTheArray()==ID_PVA)
			{
				MPvArray* PAr=(MPvArray*)ar;
				MPriv* priv;
				int cnt;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					priv=PAr->GetAt(j);
					pt = priv->GetPoints();
					for(cnt = priv->GetPointNum()-1;cnt>-1;cnt--)
					{
						LinPreobrByCoef(pt+cnt);
					}
				}
			}
			else if(ar->WhatTheArray()==ID_ZBK)
			{
				MZbArray* PAr=(MZbArray*)ar;
				MZarubka* zarubka;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					zarubka=PAr->GetAt(j);
					LinPreobrByCoef(&zarubka->m_pt);
				}
			}
		}
		RecalcBounds();
	}
	
}

//==================================
#ifdef _DEBUG
void MObject::AssertValid()
//==================================
{
//	if(!(m_bounds.left <= m_bounds.right)) exit(15);
//	if(!(m_bounds.bottom <= m_bounds.top)) exit(15);
}
#endif

//===============================
//!transformation to the model
void MObject::ConvertToMaket()
//===============================
{
    //this usual linear transformation
	LinPr();
	
	RecalcBounds();
}

//==================================
//!transformation to other system of coordinates
void MObject::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo)
//==================================
{
	if((pCurrentCoo==NULL)||(pNewCoo==NULL))
		return;
	
    //if m_Komplekt is not NULL
	if(m_Komplekt)
	{
		double h=0;
		MArray* ar;
		int i, j;
        //We are run on all arrays
		for(i=m_Komplekt->GetSize()-1;i>-1;i--)
		{
			ar=m_Komplekt->GetAt(i);
            //we scale each point of this array
            if(ar->WhatTheArray()==ID_PTA || ar->WhatTheArray()==ID_PHL)
			{
				MPtArray* PAr=(MPtArray*)ar;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
                    pt=PAr->GetAt(j);
                    /*if (fabs(pt->x)>=179.9)
                    {
                        pt->x = 179.9*(int)(fabs(pt->x)/pt->x);
                    }
                    if(fabs(pt->y)>=86.0)
                    {
                        pt->y = 86.0*(int)(fabs(pt->y)/pt->y);
                    }*/
					pCurrentCoo->ConvertToCoo(pNewCoo, pt[0], h);
				}
			}
			else if(ar->WhatTheArray()==ID_RZV)
			{
				MRzArray* PAr=(MRzArray*)ar;
				MRazryv* razr;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					razr=PAr->GetAt(j);
					pCurrentCoo->ConvertToCoo(pNewCoo,razr->m_ptfirst, h);
					pCurrentCoo->ConvertToCoo(pNewCoo,razr->m_ptlast, h);
				}
			}
			else if(ar->WhatTheArray()==ID_PVA)
			{
				MPvArray* PAr=(MPvArray*)ar;
				MPriv* priv;
				int cnt;
				MPoint* pt;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					priv=PAr->GetAt(j);
					pt = priv->GetPoints();
					for(cnt = priv->GetPointNum()-1;cnt>-1;cnt--)
					{
						pCurrentCoo->ConvertToCoo(pNewCoo, pt[cnt], h);
					}
				}
			}
			else if(ar->WhatTheArray()==ID_ZBK)
			{
				MZbArray* PAr=(MZbArray*)ar;
				MZarubka* zarubka;
                //we scale each point of this array
                for(j=PAr->GetSize()-1;j>-1;j--)
				{
					zarubka=PAr->GetAt(j);
					pCurrentCoo->ConvertToCoo(pNewCoo,zarubka->m_pt, h);
				}
			}

		}

		RecalcBounds();
	}
	
}

//==================================
//!Check of the extent of object for drawing
bool MObject::CheckSize(MPlotDevice* /*pDev*/)
//==================================
{
// vipa 2004.10.01
  if (drawActFlag)  return true;
	if (m_bounds.Width()  > m_pLay->m_pKarta->m_DrawFilterSizeGeo )  return true;
  if (m_bounds.Height() > m_pLay->m_pKarta->m_DrawFilterSizeGeo )  return true;

	if((fabs(m_bounds.left - m_bounds.right) < EPSILON)||(fabs(m_bounds.top - m_bounds.bottom)< EPSILON)) return true;  // dot object
//  if(m_pLay->m_pKarta-> m_DrawFilterSizeGeo == 0) return true;  // maybe actLayer

  return false;
}

//==================================
//! =============
bool MObject::CheckTextSize(MPlotDevice* /*pDev*/)
//==================================
{

  if (drawActFlag)  return true;
//	if (m_bounds.Width()  > m_pLay->m_pKarta->m_DrawFilterSizeGeo )  return true;
  if (m_pLay->m_pKarta-> m_DrawFilterSizeGeo == 0) return true;  // maybe actLayer
	if (m_bounds.top == m_bounds.bottom)  return true;  // text(object) height not defined
  if (m_bounds.Height() > m_pLay->m_pKarta->m_DrawFilterSizeGeo )  return true;
  return false;
}
//==================================
//!Redefinition of our own serialization
void MObjectList::Serialize( MString strFileName, bool bCopy )
//==================================
{
	if(bCopy){
		if(empty())
			return;
        //We count the size necessary for record in the clip board

		FILE* h_nuxfile = fopen(strFileName, "wb");
		if(!h_nuxfile){
			InternalMessage( "Can't open file !");
			return;
		}

        //installation of the index of record
		fseek(h_nuxfile, 0, SEEK_CUR);
		int version = 0;
		
        //We write down amount of objects
		int iSize = size();
		fwrite(&iSize, sizeof(iSize), 1, h_nuxfile);
        //Writing of all objects in turn in the file of a pocket

		OBITER it = begin();
		while(it!=end()){
			(*it)->WriteBin(h_nuxfile,version);
			++it;
		}
        //We close the file
		fclose(h_nuxfile);
	}
	else{
        //Reading from 'Clipboard'
		FILE* h_nuxfile = fopen(strFileName, "rb");
		
		if ( !h_nuxfile ) {
			InternalMessage( "Can't open file !");
			return;
		}
        //installation of the index of record
		fseek(h_nuxfile, 0, SEEK_CUR);
		
		clear();
        //We read amount of objects
		int iSize=0;
		char id_file[] = "   ";
		int version = 0;
		MObject* pObj = NULL;
		fread(&iSize, sizeof(iSize), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&iSize, sizeof(iSize) );
		for(int i = 0;i<iSize;i++){
            //reading identifier of object
			fread(&id_file, 3, 1, h_nuxfile);
			pObj = (*fpCreateObjectByType)(id_file);
			if(pObj){
				pObj->ReadBin(h_nuxfile,version);
				push_back(pObj);
			}
            //installation of the index of record
			fseek(h_nuxfile, 0, SEEK_CUR);
		}
		fclose(h_nuxfile);
	}
}
