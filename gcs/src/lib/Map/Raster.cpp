// Raster.cpp: implementation of the MScen class.
//
//////////////////////////////////////////////////////////////////////

#include "Raster.h"
#include "Globals.h"
#include "QtDevice.h"
#include "Karta.h"

//================================
MRaster::MRaster()
//================================
{
	m_Komplekt->FreeMemory();
	bVisible = false;
	fName = "";
	pMap = NULL;
	pDrawMap = NULL;
}

//================================
MRaster::MRaster(const MRect& position) : MObject(position)
//================================
{
	m_bounds = position;
	m_Komplekt->FreeMemory();
	bVisible= false;
	fName = "";	
	pMap = NULL;
	pDrawMap = NULL;
}

//================================
MRaster::MRaster(const MPoint& point)
//================================
{
	m_bounds = MRect(point.x,point.y,point.x,point.y);
	m_Komplekt->FreeMemory();
	bVisible= false;
	fName = "";	
	pMap = NULL;
	pDrawMap = NULL;
}

MRaster::~MRaster()
{
     if(pMap) delete pMap;
	 if(pDrawMap) delete pDrawMap;
}


//================================
//connect with file
bool MRaster::ConnectFile(MString* fileName)
//================================
{
	if(!fileName) return false;
	if(fileName->IsEmpty()) 
	{
		if(fName.IsEmpty()) return false;
		fileName = &fName;
	}
	FILE* hFile = fopen(*fileName,"r");
	if(!hFile)
	{ 
		InternalMessage("Incorrect file name");
		return false;
	} 
	else fclose(hFile);
	if(pMap) delete pMap;
	if(pDrawMap) delete pDrawMap;
	
	pMap = new QImage(fileName->c_str());	
	#ifndef _WIN32
	*pMap = pMap->mirrored();
	#endif
	
	bVisible = true;
	fName = *fileName;
	
	return true;
}

//================================
MObject* MRaster::Clone()
//================================
{
	MRaster* pClone = new MRaster(m_bounds);	
	MObject::MasterClone(pClone);	
	pClone->ConnectFile(&fName);
	
	return pClone;
}

//================================
void MRaster::Draw(MPlotDevice* pDevice)
//================================
{
	if(!bVisible) return;
	//отрисуем 
	pDevice->Raster(this);	
	DrawTracker(pDevice, normal);
}

//==================================
void MRaster::DrawTracker(MPlotDevice* pDevice, TrackerState state)
//==================================
{
	M_LOG_PEN lpen1(PS_SOLID,MPoint(pDevice->LogicToBum(1)),MColor(0xff,0xff,0xff));
	
	M_LOG_PEN lpen2(PS_SOLID,MPoint(pDevice->LogicToBum(3)),MColor(0,0,0));	
	
    // points: transformation to logical coordinates
	MPoint ipoints[5];
	ipoints[0].x = m_bounds.left;
	ipoints[0].y = m_bounds.top;
	ipoints[1].x = m_bounds.right;
	ipoints[1].y = m_bounds.top;
	ipoints[2].x = m_bounds.right;
	ipoints[2].y = m_bounds.bottom;
	ipoints[3].x = m_bounds.left;
	ipoints[3].y = m_bounds.bottom;
	ipoints[4].x = m_bounds.left;
	ipoints[4].y = m_bounds.top;
	
	pDevice->SavePencil();
	switch (state)
	{
	case normal:
		{
            // shadow broken line
			pDevice->SetPencil(&lpen2);
			pDevice->Polyline(&ipoints[0], 5);
            // riding broken line
			pDevice->SetPencil(&lpen1);
			pDevice->Polyline(&ipoints[0], 5);
			
		}
		break;
	}	
	pDevice->RestorePencil();	
	MObject::DrawTracker(pDevice,state);
}

//====================================================================
//Reading from an UTP format
bool  MRaster::ReadBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    //we will read information, general with the parent
	MObject::ReadBin(h_nuxfile,  version);

	switch( version ) {
		case 0:
			fread(&bVisible,sizeof(bool),1,h_nuxfile);
			fName = ReadString(h_nuxfile,version);
			//ReadRect(h_nuxfile,version,&rectDraw);
			//fread(&qrectDraw,sizeof(4*Q_INT32),1,h_nuxfile);
			MString str = "";
			ConnectFile(&str);
	}
	return true;
}

//====================================================================
bool MRaster::ReadBin(MFile* file,  int version)
//====================================================================
{
    //we will read information, general with the parent
	MObject::ReadBin(file,  version);

	switch( version ) {
		case 0:
			file->GetData(&bVisible,sizeof(bool),1);
			fName = ReadString(file,version);
			MString str = "";
			ConnectFile(&str);
	}
	return true;
}

//==================================
bool MRaster::WriteBin(FILE* h_nuxfile,  int version)
//==================================
{
	MObject::WriteBin(h_nuxfile,  version);

	switch( version ) {
		case 0:
			fwrite(&bVisible,sizeof(bool),1,h_nuxfile);
			WriteString(h_nuxfile,  version, fName);
			//WriteRect(h_nuxfile,version,&rectDraw);
			//fwrite(&qrectDraw,sizeof(4*Q_INT32),1,h_nuxfile);
		}
	return true;
}

//================================================
//function of determination of the extent of object in the UTP file
unsigned int MRaster::GetBytes()
//================================================
{
	unsigned long SBytes = MObject::GetBytes();
	
	SBytes += sizeof(bool);
	SBytes += fName.GetLength()+2; 

	return SBytes;
}

void MRaster::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo)
{
MRect rect = this->m_bounds;
MPoint ptLT(rect.left,rect.top);
MPoint ptRB(rect.right,rect.bottom);
MPoint ptLB(rect.left,rect.bottom);
MPoint ptRT(rect.right,rect.top);

MPoint cptLT, cptRB,cptLB,cptRT;
double hgt;

pCurrentCoo->ToBLH(ptLT,0,cptLT,hgt);
pNewCoo->FromBLH(cptLT,0,ptLT,hgt);
pCurrentCoo->ToBLH(ptRB,0,cptRB,hgt);
pNewCoo->FromBLH(cptRB,0,ptRB,hgt);
pCurrentCoo->ToBLH(ptLB,0,cptLB,hgt);
pNewCoo->FromBLH(cptLB,0,ptLB,hgt);
pCurrentCoo->ToBLH(ptRT,0,cptRT,hgt);
pNewCoo->FromBLH(cptRT,0,ptRT,hgt);

MRect resRect(ptLT,ptRB);
resRect.ExpandToPt(ptLB);
resRect.ExpandToPt(ptRT);
this->m_bounds = resRect;


}

QImage* MRaster::GetImagePart(QRect prtPos, int rsz)//ReadImgFromGtiff(int x_bgn,int y_bgn,int iwidth,int iheight, int rsz)
{
	if (pDrawMap)
	{       
		delete pDrawMap;
		pDrawMap = new QImage(pMap->copy(prtPos.x(),prtPos.y(),prtPos.width(),prtPos.height()));
	}

    return pMap;
}
