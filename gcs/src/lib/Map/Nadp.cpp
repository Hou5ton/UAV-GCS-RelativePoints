// Nadp.cpp : implementation file
//
#include "Nadp.h"
#include "Id.h"
#include "PlotDevice.h"
#include "Layer.h"
#include "Globals.h"
#include "Karta.h"
#include "Refglobal.h"

//==================================
//! MNadp
MNadp::MNadp()
{
    // Installation of an inscription
	m_Text = "Надписъ";
    // Installation of an scale
	m_Mashtab = 1;

    // Installation of an points
	m_dbarray.SetSize(ID_DBINITSIZE);
	
    //We form M_Komplekt
	m_Komplekt->FreeMemory();
    //array of points
	MPtArray* pta = new MPtArray();
	m_Komplekt->AddTail((MArray*)pta);
    // Installation of attributes of handwriting
	m_Prk = new MPrkExt();
	m_Prk->align                = UT_TA_LEFT | UT_TA_BASELINE;
}

//!==================================
MNadp::~MNadp()
{
	if(m_Prk!=NULL)
		delete m_Prk;
	m_Prk = NULL;
//  m_Text.Empty();
}

//!==================================
MNadp::MNadp(MRect position, MString Text, MPrkExt* Attr, float Mashtab)
	: MObject(position)
{

    // Installation of an inscription
    m_Text = Text;
    // Installation of an scale
    m_Mashtab = Mashtab;

	m_dbarray.SetSize(ID_DBINITSIZE);

    //We form M_Komplekt
    m_Komplekt->FreeMemory();
    //array of points
    MPtArray* pta=new MPtArray();
	m_Komplekt->AddTail((MArray*)pta);

    // Installation of attributes of handwriting
	m_Prk = new MPrkExt();
	if(Attr == NULL)
	{
		m_Prk->align  = UT_TA_LEFT | UT_TA_BASELINE;
	}
	else 
	{
		delete m_Prk;
		m_Prk = new MPrkExt();
		m_Prk->Copy(Attr);
	}
}

//==================================
//! how many points in a marking?
int MNadp::GetHandleCount()
{
	return ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetSize();
}

//==================================
//! parents are responsible for copying
void MNadp::Draw(MPlotDevice*)//(MPlotDevice* pDevice)
{
}

//==================================
//! drawing of an inscription
void MNadp::DrawText(MPlotDevice* pDevice)
{
    //if(GetLayer()->x_common) return; //we don't draw!

	if(!m_Prk->visible) return;


    // We set an inscription angle of rotation at a conclusion
//	m_Prk->logfont.lfEscapement = (int)- dbl_to_int(angle);

	M_LOG_FONT lfont = m_Prk->logfont;

  drawTextFlag = false;
	if(m_Prk->scaled)
	{
    drawTextFlag = true;
    lfont.lfHeight = m_Prk->value * m_Mashtab ;
    /*   //  vipa 2004.07.07
    //check on an inscription size exit for the set border
		int msize = textSizeMax;
		if(msize)
		{
			double max_size =  pDevice->LogicToBum(msize);
			
			double etalon_size =  37.5;
//    	double text_size =  m_Prk->value;
			double text_size = lfont.lfHeight;           
			double koef = etalon_size/text_size;

			if((lfont.lfHeight*koef) > max_size) lfont.lfHeight = max_size/koef;
		}
    */
	}
	else
		lfont.lfHeight = pDevice->LogicToBum(m_Prk->logfont.lfHeight);//m_Prk->value);

	if (!pDevice->Distinguish_Text(lfont.lfHeight))
	  { drawTextFlag= false;	return; }
    
    //We calculate an inscription angle of rotation in degrees (geo)
	double angle = GetVektorGrad(((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0),
								((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(1))*10;

	pDevice->SetBkMode(m_Prk->bkmode);
	pDevice->SetBkColor(m_Prk->bkcolorkodExt);
	pDevice->SetTextColor(m_Prk->colorkodExt);
        pDevice->SetTextAlign(m_Prk->align);
  // We set an inscription angle of rotation at a conclusion
	lfont.lfEscapement = (int)- dbl_to_int(angle);


	pDevice->SaveFont();

	pDevice->SetFont(&lfont);
	if(m_pLay->m_bOperLayer)
		pDevice->TextOut(((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->x,
						((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->y, m_Text, m_Text.GetLength());
	else
		pDevice->TextOutWindows(((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->x,
						((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->y, m_Text, m_Text.GetLength());
    // Return to old type of drawing
	pDevice->RestoreFont();
}


//==================================
//! drawing of a marking of top object
void MNadp::DrawTracker(MPlotDevice* pDevice,TrackerState)//(MPlotDevice* pDevice,TrackerState state)
{
	MPtArray* pta = ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS));
	
    // usual tracker
    // we declare a pen 1
	M_LOG_PEN lpen1(PS_SOLID,MPoint(pDevice->LogicToBum(1)),MColor(0xff,0xff,0xff));
    // we declare a pen 2
	M_LOG_PEN lpen2(PS_SOLID,MPoint(pDevice->LogicToBum(3)),MColor(0,0,0));
	pDevice->SavePencil();
	pDevice->SetPencil(&lpen2);
	pDevice->Polyline(pta->GetHead(),pta->GetSize());
						
    // riding broken line
	pDevice->SetPencil(&lpen1);
	pDevice->Polyline(pta->GetHead(),pta->GetSize());	
					
	double d1Pix = pDevice->LogicToBum(1);
	double d2Pix = d1Pix*2;
	double d3Pix = d1Pix*3;
	double d5Pix = d1Pix*5;
    // points...
	for (int i = 0; i < pta->GetSize(); i++) 
	{
		pDevice->PatBlt(pta->GetAt(i)->x - d2Pix, pta->GetAt(i)->y - d2Pix, d5Pix, d5Pix, BLACKNESS);
		pDevice->PatBlt(pta->GetAt(i)->x - d1Pix, pta->GetAt(i)->y - d1Pix, d3Pix, d3Pix, WHITENESS);
	}
	pDevice->RestorePencil();	
}



//==================================
//! change of provision of a separate point in an inscription
void MNadp::MoveHandleTo(int nHandle,MPtArray* pArray, MPoint point)
{
	pArray->GetAt(nHandle)[0]=point;
	RecalcBounds();
}



//==================================
//! clonning
MObject* MNadp::Clone()
{
	MNadp* pClone = new MNadp(m_bounds,m_Text,m_Prk,m_Mashtab);
	MObject::MasterClone(pClone);
	return pClone;
}

//==================================
//! Installation of basic points for an inscription binding vector
void MNadp::SetBasePoint(MPoint SrcPnt, MPoint DstPnt, double *Angle)
{
	double alpha = 0.0;
	if(Angle == NULL)
		alpha = GetVektorGrad(&SrcPnt,&DstPnt);
	else
		alpha = *Angle;
	double Metrica = DLINASG(SrcPnt,DstPnt);
	DstPnt.x = SrcPnt.x+Metrica*cos(alpha*pi/*_MW*//180.0);
	DstPnt.y = SrcPnt.y+Metrica*sin(alpha*pi/*_MW*//180.0);
    // The necessary check excluding record of an inscription head over heels
    //We check existence of points if they aren't present, we will add
		if(((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetSize()==0)
		{
			((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->AddTail(&SrcPnt);
			((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->AddTail(&DstPnt);
		}
		else
		{
			((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->SetAt(0,&SrcPnt);
			((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->SetAt(1,&DstPnt);
		}
}

//==================================
//! Addition of a point: a point in geocoordinates
void MNadp::AddPoint(MPoint geopoint)
{
	MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(NPK_POINTS);
	pta->AddTail(&geopoint);
    // writing of a new point
	RecalcBounds();
}

//!==================================
void MNadp::Rotate(double A, double B, double C1, double C2)
{
	MObject::Rotate(A, B, C1, C2);
	RecalcBounds();
	Invalidate();
}

//!==================================
void MNadp::Scale(double K, double XC, double YC)
{
	MObject::Scale(K,  XC, YC);
    // we calculate font height
	if(m_Prk->scaled) 
		m_Prk->value *=K;
	RecalcBounds();
	Invalidate();
}


//==================================
//! Recalculation of the describing rectangle
bool MNadp::RecalcBounds()
{
	if(((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetSize()== 0)
		return false;

    // for a start we take a rectangle round the first
    //points radius...
	MRect bounds(((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->x,
				    ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->y, 
				    ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->x, 
				    ((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(0)->y);
	m_bounds.NormalizeGeoRect();

    // ... it also has to cover all the subsequent...
	for (int i=0; i<((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetSize(); i++) bounds.ExpandToPt(*((MPtArray*)m_Komplekt->GetAt(NPK_POINTS))->GetAt(i));
	bounds.NormalizeGeoRect();
    // we will give also an extra charge on the letter size if there is a need
	if (m_Prk->visible == true) 
	{
		bounds.left   = bounds.left   - m_Prk->value * 1.4;
		bounds.top    = bounds.top    + m_Prk->value * 1.4;
		bounds.right  = bounds.right  + m_Prk->value * 1.4;
		bounds.bottom = bounds.bottom - m_Prk->value * 1.4;
	}

	if (bounds == m_bounds)
		return false;

	Invalidate();

	m_bounds = bounds;
	m_bounds.NormalizeGeoRect();
	Invalidate();
	return true;
}

//====================================================================
//!Reading from an UTP-format
bool  MNadp::ReadBin(FILE* h_nuxfile,  int version)
{
    //we will read information, general with the parent
	MObject::ReadBin(h_nuxfile,  version);

	switch( version ) {
        //version 0
		case 0:
            // Reading handwriting of object
			m_Prk->ReadBin(h_nuxfile,version);
            // Reading scale of object
			fread(&m_Mashtab, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_Mashtab, sizeof(m_Mashtab) );
            // Reading text
			m_Text=ReadString(h_nuxfile, version);
	}
	return true;
}

//====================================================================
//!Reading from an UTP-format
bool  MNadp::ReadBin(MFile* file,  int version)
{
    //we will read information, general with the parent
    MObject::ReadBin(file,  version);

	switch( version ) {
        //version 0
		case 0:
            // Reading handwriting of object
            m_Prk->ReadBin(file,version);
            // Reading scale of object
            file->GetData(&m_Mashtab, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&m_Mashtab, sizeof(m_Mashtab) );
            // Reading text
            m_Text=ReadString(file, version);
	}
	return true;
}

//======================================================================
//!Function of writing in UTP
bool  MNadp::WriteBin(FILE* h_nuxfile,  int version)
{
	
    //Calculation of the extent of object in UTP
///	unsigned int SBytes = GetBytes();

    //we will write information, general with the parent
    MObject::WriteBin(h_nuxfile,  version);
	
    //Writing of own information
	switch( version ) {
        //version 0
		case 0:
            // Writing handwriting of object
            m_Prk->WriteBin(h_nuxfile,version);
            // Writing scale of object
			fwrite(&m_Mashtab, 4, 1, h_nuxfile);
            // Writing text
			WriteString(h_nuxfile,version,m_Text);
	}
	return true;
};

//===============================
//!transformation to the model
void MNadp::ConvertToMaket()
{
	MObject::ConvertToMaket();

    //we will establish scale in 1
	m_Mashtab = 0.005f;
	
	RecalcBounds();
}


//================================================
//!function of determination of the extent of object in the UTP-file
/**	//The extent of object in UTP is calculated thus:
         // size of the part, general with parent
         // + extent of handwriting (m_Prk)
         // + 4 bytes on writing scale
         // + size of an inscription (m_Text)
*/
unsigned int MNadp::GetBytes()
{
    //Calculation of the extent of object in UTP

	unsigned int SBytes = MObject::GetBytes();
    // extent of handwriting
	SBytes += m_Prk->GetBytes();		
    // the size for writing of scale
	SBytes += 4; 
    // the size for writing of an inscription with its size
	SBytes += m_Text.GetLength()+2; 

	return SBytes;
}
