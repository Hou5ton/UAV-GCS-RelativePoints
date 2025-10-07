//===============================================================================
// Objects (CPP)
//===============================================================================

#include <math.h>

#include "Karta.h"
#include "NadpKriv.h"
#include "Nadp.h"
#include "PlotDevice.h"
#include "Refglobal.h"

extern MObject* (*fpCreateObjectByType)(MString str);

//===============================================================================
// MNadpKriv
//===============================================================================
//==================================
//! MNadpKriv
MNadpKriv::MNadpKriv()
{
	m_dbarray.SetSize(ID_DBINITSIZE);

    //form M_Komplekt
	m_Komplekt->FreeMemory();
    //massif of points
	MPtArray* pta=new MPtArray();
	m_Komplekt->AddTail((MArray*)pta);
    //massif of angles
	MInArray* ina=new MInArray();
	m_Komplekt->AddTail((MArray*)ina);
    //Installation of attributes of handwriting
	m_prk = new MPrkExt();
	m_prk->align                = UT_TA_LEFT | UT_TA_BASELINE;
}

//!==================================
MNadpKriv::MNadpKriv(const MRect& , MPrkExt* prk)//(const MRect& position, MPrkExt* prk)
{
	m_dbarray.SetSize(ID_DBINITSIZE);

	*m_dbarray[ID_TXT] = "надписъ";

    //form M_Komplekt
	m_Komplekt->FreeMemory();
    //massif of points
    MPtArray* pta=new MPtArray();
	m_Komplekt->AddTail((MArray*)pta);
    //massif of angles
    MInArray* ina=new MInArray();
	m_Komplekt->AddTail((MArray*)ina);
	
    //Installation of attributes of handwriting
    m_prk = new MPrkExt();
	if(prk == NULL)
		m_prk->align  = UT_TA_LEFT | UT_TA_BASELINE;
	else 
	{
		delete m_prk;
		m_prk = new MPrkExt();
		m_prk->Copy(prk);
	}

	RecalcBounds();
}
//!  -----------
MNadpKriv::~MNadpKriv()
{
	if(m_prk!=NULL)
		delete m_prk;
	m_prk = NULL;
}

//==================================
//! drawing of an inscription
void MNadpKriv::DrawText(MPlotDevice* pDevice)
{
	//if(GetLayer()->x_common) return; // не рисуем!

	MPrkExt drawprk;
	drawprk.Copy(GetPrk());
	
	if(!drawprk.visible)return;

	int m_nPoints=GetHandleCount();
	MPoint*   m_points;
	if (m_nPoints==0)
		m_points=NULL;
	else
		m_points=((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0);
	int*      m_angles;
	if(m_nPoints==0)
		m_angles=NULL;
	else
		m_angles=((MInArray*)m_Komplekt->GetAt(NPK_ANGLES))->GetAt(0);
	
    // while we draw, let the line be drawn...
	if(m_angles==NULL) pDevice->Polyline(m_points, m_nPoints);
    //...and when finished - an inscription
	else {
		//pDevice->SetBkMode(drawprk.bkmode);
        //we will receive color of the text on codes in the dictionary of colours
		//MColor color;
        //if in the dictionary there was no color, do the text black
		//if(!m_pLay->GetKarta()->GetDictColorValue((unsigned short)drawprk.colorkod, color))
		//	color.SetColor(RGB(0,0,0));
		//pDevice->SetTextColor(color.GetColor());
	    //pDevice->SetTextAlign(drawprk.align);
		pDevice->SetBkMode(drawprk.bkmode);
		pDevice->SetBkColor(drawprk.bkcolorkodExt);
		pDevice->SetTextColor(drawprk.colorkodExt);
		pDevice->SetTextAlign(drawprk.align);		
		
    drawTextFlag = false;
		M_LOG_FONT lfont = drawprk.logfont;
		if (drawprk.scaled)
		{
			lfont.lfHeight = drawprk.value;
			drawTextFlag= true;
/*   vipa 2004.07.20
            //check on an inscription size exit for the set border
			int msize = textSizeMax;
			if(msize)
			{
				double max_size = pDevice->LogicToBum(msize);

				double etalon_size = 37.5;
				double text_size = drawprk.value;
				double koef = etalon_size/text_size;

				if((lfont.lfHeight*koef) > max_size) lfont.lfHeight = max_size/koef;
			}
*/
		}
		else
			lfont.lfHeight = pDevice->LogicToBum(drawprk.logfont.lfHeight);

		if(!pDevice->Distinguish_Text(lfont.lfHeight))
		   { drawTextFlag=false; return;  }

		pDevice->SaveFont();
		for (int i=0; i<m_nPoints; i++) {

			lfont.lfEscapement =  m_angles[i];

			pDevice->SetFont(&lfont);
			char bukva = m_dbarray[ID_TXT]->GetAt(i);
			pDevice->TextOut( m_points[i].x, m_points[i].y, &bukva ,1);
		}
		pDevice->RestoreFont();
	}
}

//==================================
//! drawing of a marking of top object
void MNadpKriv::DrawTracker(MPlotDevice* pDevice, TrackerState state)
{
	switch (state)
	{
	case normal:
		{
			MPtArray* pta = ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS));
			
            // usual tracker
            //we declare a pen 1
			M_LOG_PEN lpen1(PS_SOLID,MPoint(pDevice->LogicToBum(1)),MColor(0xff,0xff,0xff));
            //we declare a pen 2
            M_LOG_PEN lpen2(PS_SOLID,MPoint(pDevice->LogicToBum(3)),MColor(0,0,0));
			pDevice->SavePencil();
			pDevice->SetPencil(&lpen2);
			pDevice->Polyline(pta->GetHead(),pta->GetSize());
								
            // riding broken line
			pDevice->SetPencil(&lpen1);
			pDevice->Polyline(pta->GetHead(),pta->GetSize());

            // points...
			int nHandleCount = GetHandleCount();
			double d1Pix = pDevice->LogicToBum(1);
			double d3Pix = d1Pix*3;
			double d2Pix = d1Pix*2;
			double d5Pix = d1Pix*5;
            // points...
			for (int i = 0; i < nHandleCount; i++) 
			{
				MPoint handle = pta->GetAt(i);
				pDevice->PatBlt(handle.x - d2Pix, handle.y - d2Pix, d5Pix, d5Pix, BLACKNESS);
				pDevice->PatBlt(handle.x - d1Pix, handle.y - d1Pix, d3Pix, d3Pix, WHITENESS);
			}
			pDevice->RestorePencil();
      break;
		}
	default:	break;
	}
  
}

//==================================
//! parents are responsible for copying
void MNadpKriv::Draw(MPlotDevice* /*pDevice*/)
{
}

//==================================
//! how many points in a marking?
int MNadpKriv::GetHandleCount()
{
	return ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetSize();
}

//==================================
//! change of provision of a separate point in an inscription
void MNadpKriv::MoveHandleTo(int nHandle,MPtArray* pArray, MPoint point)
{
	pArray->GetAt(nHandle)[0]=point;
	int* m_angles=((MInArray*)m_Komplekt->GetAt(NPK_ANGLES))->GetAt(0);
	if (m_angles != NULL) RecalcAngles();
	RecalcBounds();
}



//==================================
//! clonning
MObject* MNadpKriv::Clone()
{
	MNadpKriv* pClone = new MNadpKriv(m_bounds, m_prk);
	MObject::MasterClone(pClone);
	return pClone;
}

//==================================
//! Addition of a point: a point in geocoordinates
void MNadpKriv::AddPoint(const MPoint& geopoint)
{
	MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(NPK_POINTS);
	MPoint pt111=geopoint;
	pta->AddTail(&pt111);
    // writing of a new point
	RecalcBounds();
}


//!==================================
void MNadpKriv::Rotate(double A, double B, double C1, double C2)
{
	MObject::Rotate(A, B, C1, C2);
	MasterScript();
	RecalcBounds();
	Invalidate();
}

//!==================================
void MNadpKriv::Scale(double K, double XC, double YC)
{
	MObject::Scale(K,  XC, YC);
	MasterScript();
	RecalcBounds();
	Invalidate();
}

//==================================
//! Recalculation of the describing rectangle
bool MNadpKriv::RecalcBounds()
{
	int m_nPoints=((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetSize();

	MPoint*   m_points;
	if (m_nPoints==0)
		m_points=NULL;
	else
		m_points=((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0);

	if (m_nPoints == 0)
		return false;

    // for a start we take a rectangle round the first
    //points radius...
	MRect bounds(m_points[0].x,
				    m_points[0].y, 
				    m_points[0].x, 
				    m_points[0].y);
	m_bounds.NormalizeGeoRect();

    // ...it also has to cover all the subsequent...
	for (int i=0; i<m_nPoints; i++) bounds.ExpandToPt(m_points[i]);
	bounds.NormalizeGeoRect();
	MPrkExt* p_prk = GetPrk();
    // we will give also an extra charge on the letter size if there is a need
	if (p_prk->visible == true) {
			bounds.left   = bounds.left   - p_prk->value * 1.4;
			bounds.right  = bounds.right  + p_prk->value * 1.4;
			bounds.top    = bounds.top    + p_prk->value * 1.4;
			bounds.bottom = bounds.bottom - p_prk->value * 1.4;
            //in these four lines 1.4 - the cube diagonal relation to the party,
            //that the frame covered letters even when they
            //at an angle
	}

	if (bounds == m_bounds)
		return false;

	Invalidate();

	m_bounds = bounds;
	m_bounds.NormalizeGeoRect();

	Invalidate();

	return true;
}

//==================================
//! We will make an inscription
void MNadpKriv::MasterScript(MString Text)
{
	int m_nPoints=GetHandleCount();
	MPoint*   m_points;
	if (m_nPoints==0)
		m_points=NULL;
	else
		m_points=((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0);
	int*      m_angles;
	if(m_nPoints==0)
		m_angles=NULL;
	else
		m_angles=((MInArray*)m_Komplekt->GetAt(NPK_ANGLES))->GetAt(0);
///	MPrk* p_prk = GetPrk();

    // inscription: text, font, color, size
	*m_dbarray[ID_TXT] = Text;

    // massif of the partial sums of distances
    //summ[i] - distance from 0 to point i-th
	double* summ = new double[m_nPoints];
	summ[0] = 0;
	int i = 0;
	for (i=1; i<m_nPoints; i++) 
		summ[i] = summ[i-1] + sqrt( (m_points[i].x - m_points[i-1].x)*(m_points[i].x - m_points[i-1].x) +
		                            (m_points[i].y - m_points[i-1].y)*(m_points[i].y - m_points[i-1].y));
	
    // massif of lengths of pieces
    //dlin[i] - distance from i-th to the following point
	double* dlin = new double[m_nPoints];
	for (i=1; i<m_nPoints; i++)
		dlin[i-1] = sqrt( (m_points[i].x - m_points[i-1].x)*(m_points[i].x - m_points[i-1].x) +
		                  (m_points[i].y - m_points[i-1].y)*(m_points[i].y - m_points[i-1].y));
	
    //  massif of distances to letters
    //rasst[i] - distance of companies of 0 to letter i-th
	int kol = m_dbarray[ID_TXT]->GetLength();
	double* rasst = new double[kol];
	for (i=0; i<kol; i++) rasst[i] = i*summ[m_nPoints-1]/(double)(kol-1);

    // new massif of geopoints
	MPtArray* pta1=new MPtArray();
	pta1->SetSize(kol);
	MPoint* newPoints = pta1->GetAt(0);

    //we calculate an arrangement of points of the bases of letters
    //the first (coincides)...
	newPoints[0] = m_points[0];
    // others...
	for (i=1; i<kol-1; i++) {
		int j = 0;
		int k = 0;
        // we will find such k that i-th a letter get
        //in a piece between k-th and (k+1)-th points
		while(summ[j]<rasst[i]) {
			k = j;
			j++;
		}
        //core...
		newPoints[i].x = m_points[k].x + (m_points[k+1].x - m_points[k].x)*(rasst[i] - summ[k])/dlin[k];
		newPoints[i].y = m_points[k].y + (m_points[k+1].y - m_points[k].y)*(rasst[i] - summ[k])/dlin[k];
	}
    // last...
	newPoints[kol-1] = m_points[m_nPoints-1];

	MPtArray* pta2=(MPtArray*)m_Komplekt->GetAt(NPK_POINTS);
	m_Komplekt->SetAt(NPK_POINTS,pta1);
	delete pta2;
	
    // we will create and will calculate the massif of angles of rotation
	MInArray* ina=(MInArray*)m_Komplekt->GetAt(NPK_ANGLES);
	ina->SetSize(kol);
	//m_angles = ina->GetAt(0);
	RecalcAngles();

    // we will calculate the describing rectangle
	RecalcBounds();

	delete[] summ;
	delete[] dlin;
	delete[] rasst;
}

//==================================
//! we calculate the massif of angles of rotation of letters
/**	// GENERAL IDEA IS AS FOLLOWS:
 //1) from each point we carry out a vector, perpendicular to a piece,
 //connecting the previous and subsequent points, and also we find a vector,
 //the return to it (for the first/last point we take a piece,
 //connecting it to a second/penultimate point);
 //2) we choose from these two vectors what goes to the left
 //from the line of an inscription (it becomes through calculation of the focused
 //areas);
 //3) including this vector the left normal to the line of an inscription, we calculate
 //letter angle;
*/
bool MNadpKriv::RecalcAngles()
{

	int m_nPoints=GetHandleCount();
	MPoint*   m_points;
	if (m_nPoints==0)
		m_points=NULL;
	else
		m_points=((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0);
	int*      m_angles;
	if(m_nPoints==0)
		m_angles=NULL;	
	else
		m_angles=((MInArray*)m_Komplekt->GetAt(NPK_ANGLES))->GetAt(0);

	double AT,T,X,Y;
	MPoint pr1,pr2;

	for (int i=0; i<m_nPoints; i++) {
        //first point
		if (i==0) {
			pr1.x = m_points[i].x - (-(m_points[i+1].y - m_points[i].y)/(m_points[i+1].x - m_points[i].x))*10;
			pr1.y = m_points[i].y - 10;
			pr2.x = m_points[i].x - (+(m_points[i+1].y - m_points[i].y)/(m_points[i+1].x - m_points[i].x))*10;
			pr2.y = m_points[i].y + 10;

			if (((m_points[i].x-m_points[i+1].x)*(m_points[i].y+m_points[i+1].y)
			  + (m_points[i+1].x-pr1.x)*(m_points[i+1].y+pr1.y)
			  + (pr1.x-m_points[i].x)*(pr1.y+m_points[i].y)) < 0) {
				Y = pr1.y - m_points[i].y;
				X = pr1.x - m_points[i].x;
			}
			else {
				Y = pr2.y - m_points[i].y;
				X = pr2.x - m_points[i].x;

			}
		}
        // last point
		else if (i==m_nPoints-1) {
			pr1.x = m_points[i].x - (-(m_points[i].y - m_points[i-1].y)/(m_points[i].x - m_points[i-1].x))*10;
			pr1.y = m_points[i].y - 10;
			pr2.x = m_points[i].x - (+(m_points[i].y - m_points[i-1].y)/(m_points[i].x - m_points[i-1].x))*10;
			pr2.y = m_points[i].y + 10;

			if (((m_points[i-1].x-m_points[i].x)*(m_points[i-1].y+m_points[i].y)
			  + (m_points[i].x-pr1.x)*(m_points[i].y+pr1.y)
			  + (pr1.x-m_points[i-1].x)*(pr1.y+m_points[i-1].y)) < 0) {
				Y = pr1.y - m_points[i].y;
				X = pr1.x - m_points[i].x;
			}
			else {
				Y = pr2.y - m_points[i].y;
				X = pr2.x - m_points[i].x;
			}
		}
        //other points
		else {
			pr1.x = m_points[i].x - (-(m_points[i+1].y - m_points[i-1].y)/(m_points[i+1].x - m_points[i-1].x))*10;
			pr1.y = m_points[i].y - 10;
			pr2.x = m_points[i].x - (+(m_points[i+1].y - m_points[i-1].y)/(m_points[i+1].x - m_points[i-1].x))*10;
			pr2.y = m_points[i].y + 10;

			if (((m_points[i].x-m_points[i+1].x)*(m_points[i].y+m_points[i+1].y)
			  + (m_points[i+1].x-pr1.x)*(m_points[i+1].y+pr1.y)
			  + (pr1.x-m_points[i].x)*(pr1.y+m_points[i].y)) < 0) {
				Y = pr1.y - m_points[i].y;
				X = pr1.x - m_points[i].x;
			}
			else {
				Y = pr2.y - m_points[i].y;
				X = pr2.x - m_points[i].x;
			}
		}

        //... and at last, we calculate and we write down an angle
		T = Y/X;
		AT = atan(T)*(double)3600.0/((double)2.0*(double)3.1516926);

		if((X>0)&&(Y>0)) AT = (double)2700.0 - AT;
		if((X>0)&&(Y<0)) AT = (double)2700.0 - AT;
		if((X<0)&&(Y>0)) AT = (double)900.0  - AT;
		if((X<0)&&(Y<0)) AT = (double)900.0  - AT;

		m_angles[i] = (int) AT;
		if(X==0) m_angles[i] = 0;
	}

	return true;
}

//====================================================================
//!Reading from an UTP-format
bool  MNadpKriv::ReadBin(FILE* h_nuxfile,  int version)
{
    //we will read information, general with the parent
	MObject::ReadBin(h_nuxfile,  version);

	switch( version ) {
        //version 0
		case 0:
            // Reading handwriting of object
			m_prk->ReadBin(h_nuxfile,version);
	}
	return true;
}

//====================================================================
//!Reading from an UTP-format
bool  MNadpKriv::ReadBin(MFile* file,  int version)
{
    //we will read information, general with the parent
	MObject::ReadBin(file,  version);

	switch( version ) {
        //version 0
		case 0:
            // Reading handwriting of object
			m_prk->ReadBin(file,version);
	}
	return true;
}

//======================================================================
//!Function of writing in UTP
bool  MNadpKriv::WriteBin(FILE* h_nuxfile,  int version)
{
	
    //we will write information, general with the parent
	MObject::WriteBin(h_nuxfile,  version);

    //writing
	switch( version ) {
        //version 0
		case 0:
            //writing handwriting of object
			m_prk->WriteBin(h_nuxfile,version);
	}
	return true;
};

//================================================
//!function of determination of the extent of object in the UTP-file
unsigned int MNadpKriv::GetBytes()
{
    //Calculation of the extent of object in UTP
    //The extent of object in UTP is calculated thus:
    //size of information, general with parent
    //+ extent of handwriting (m_prk)

	unsigned int SBytes = MObject::GetBytes();
    // extent of handwriting
	SBytes += m_prk->GetBytes();		

	return SBytes;
}
