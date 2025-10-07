//PACKAGE : MAP.
//FILE: Raster.h   
//AUTHOR:
//DESCRIPTION: interface for the MScen class

#if !defined RASTER_H
#define RASTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
#include <qimage.h>

//PACKAGE : MAP.
//CLASS : MRaster : public MObject.
//DESCRIPTION : Describes raster object.
class MRaster : public MObject  

{
public:
	QImage* pMap;//image 
	QSize pMapSz;
	MString fName;//image file name
	bool bVisible;//visible flag
	QImage* pDrawMap;//handle to the last image
	MRect 	rectDraw;//geo rect of pDrawMap
	QRect qrectDraw;//logic rect of pDrawMap

//PACKAGE : MAP.
//FUNCTION : MRaster::MRaster();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MRaster();

//PACKAGE : MAP.
//FUNCTION : MRaster::MRaster(const MPoint& point);
//DESCRIPTION : Constructor.
//INPUTS : const MPoint&.
//RETURNS : NONE.
	MRaster(const MPoint& point);

//PACKAGE : MAP.
//FUNCTION : MRaster::MRaster(const MRect& position);
//DESCRIPTION : Constructor.
//INPUTS : const MRect&.
//RETURNS : NONE.
	MRaster(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MRaster::~MRaster();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	virtual ~MRaster();

//PACKAGE : MAP.
//FUNCTION : MRaster::Clone();
//DESCRIPTION :  Makes a clone.
//INPUTS : NONE.
//RETURNS : MObject pointer.
	virtual MObject*  Clone();

//PACKAGE : MAP.
//FUNCTION : MRaster::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);
//DESCRIPTION : Convert to Coo.
//INPUTS : MCoo pointer, MCoo pointer.
//RETURNS : NONE.
	void ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo);

//PACKAGE : MAP.
//FUNCTION : MRaster::Draw(MPlotDevice* pDevice);
//DESCRIPTION : Draws .
//INPUTS : MPlotDevice pointer.
//RETURNS : NONE.
	void Draw(MPlotDevice* pDevice);

//PACKAGE : MAP.
//FUNCTION : MRaster::DrawTracker(MPlotDevice* pDevice, TrackerState  state);
//DESCRIPTION : Draws tracker.
//INPUTS : MPlotDevice pointer, TrackerState  .
//RETURNS : NONE.
	void DrawTracker(MPlotDevice* pDevice, TrackerState  state);

	//PACKAGE : MAP.
//FUNCTION : MRaster::ConnectFile(MString* fileName);
//DESCRIPTION : Load the image from file <fileName>.
//INPUTS : MString pointer.
//RETURNS : NONE.
	bool ConnectFile(MString* fileName);

//PACKAGE : MAP.
//FUNCTION : MRaster::GetBytes();
//DESCRIPTION : Gets size in a bytes.
//INPUTS : NONE.
//RETURNS : unsigned int.
	virtual unsigned int GetBytes();

//PACKAGE : MAP.
//FUNCTION : MRaster::GetImagePart(QRect prtPos, int rsz);
//DESCRIPTION : Gets part of image.
//INPUTS : QRect , int .
//RETURNS : QImage pointer.
	virtual QImage* GetImagePart(QRect prtPos, int rsz);

//PACKAGE : MAP.
//FUNCTION : MRaster::ReadBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : True.
	virtual bool  ReadBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MRaster:: ReadBin(MFile* file,  int version);
//DESCRIPTION : Read from UTP-format file.
//INPUTS : MFile pointer,  int .
//RETURNS : True.
	virtual bool  ReadBin(MFile* file,  int version);

//PACKAGE : MAP.
//FUNCTION : MRaster:: WriteBin(FILE* h_nuxfile,  int version);
//DESCRIPTION : Writes to UTP-format file.
//INPUTS : FILE pointer,  int .
//RETURNS : NONE.
	virtual bool  WriteBin(FILE* h_nuxfile,  int version);

//PACKAGE : MAP.
//FUNCTION : MRaster::WhatTheObject();
//DESCRIPTION : Returns object name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheObject() {return ID_SCENERY;};
};

#endif // !defined RASTER_H
