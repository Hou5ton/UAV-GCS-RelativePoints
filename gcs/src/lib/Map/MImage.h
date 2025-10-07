//PACKAGE : MAP.
//FILE: MImage.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _MIMAGE__H_
#define _MIMAGE__H_

#include "ArrBase.h"

//PACKAGE : MAP.
//CLASS	: MImage.
//DESCRIPTION : 
class MImage
{
private:
	int w, h, d, bitordr, dpmx, dpmy, alpha;
	MByArray *data;
	MInArray *ctbl;

public:

//PACKAGE : MAP.
//FUNCTION : MImage::MImage();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : NONE.
	MImage () ;

//PACKAGE : MAP.
//FUNCTION : MImage::MImage ( int w, int h, int depth, int bitOrder = 0 );
//DESCRIPTION : Constructor.
//INPUTS : int , int , int , int .
//RETURNS : NONE.
	MImage ( int w, int h, int depth, int bitOrder = 0 ) ;

//PACKAGE : MAP.
//FUNCTION : MImage::MImage ( int w, int h, int format );
//DESCRIPTION : Constructor.
//INPUTS : int , int , int.
//RETURNS : NONE.
	MImage ( int w, int h, int format ) ;

//PACKAGE : MAP.
//FUNCTION : MImage::MImage();
//DESCRIPTION : Constructor for a given object.
//INPUTS : MImage pointer.
//RETURNS : NONE.
	MImage(MImage* image);

//PACKAGE : MAP.
//FUNCTION : MImage::~MImage();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
    virtual ~MImage () ;

//PACKAGE : MAP.
//FUNCTION : MImage::width();
//DESCRIPTION : Width calculation.
//INPUTS : NONE.
//RETURNS : int.
	int width () { return w; }  

//PACKAGE : MAP.
//FUNCTION : MImage::setWidth ( int width );
//DESCRIPTION : Sets width.
//INPUTS : int.
//RETURNS : NONE.
	void setWidth ( int width ) { if (width > 0) w = width; else w = 0; }

//PACKAGE : MAP.
//FUNCTION : MImage::height();
//DESCRIPTION : Height calculation.
//INPUTS : NONE.
//RETURNS : int.
	int height () { return h; }

//PACKAGE : MAP.
//FUNCTION : MImage::setHeight ( int height );
//DESCRIPTION : Sets height.
//INPUTS : int.
//RETURNS : NONE.
	void setHeight ( int height ) { if (height > 0) h = height; else h = 0; }

//PACKAGE : MAP.
//FUNCTION : MImage::MImage();
//DESCRIPTION : Depth calculation.
//INPUTS : NONE.
//RETURNS : NONE.
	int depth () { return d; }

//PACKAGE : MAP.
//FUNCTION : MImage::MImage();
//DESCRIPTION : Sets depth.
//INPUTS : NONE.
//RETURNS : NONE.
	void setDepth ( int depth ) { if (depth > 0) d = depth; else d = 0; }
	
//PACKAGE : MAP.
//FUNCTION : MImage::bits();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : MByArray pointer.
	MByArray * bits () { return  data; } 	

//PACKAGE : MAP.
//FUNCTION : MImage::getBits(uchar* bits);
//DESCRIPTION : Gets bits.
//INPUTS : uchar pointer.
//RETURNS : NONE.
	void getBits (uchar* bits);

//PACKAGE : MAP.
//FUNCTION : MImage::setBits (uchar* bits, int numBytes);
//DESCRIPTION : Gets bits.
//INPUTS : uchar pointer, int .
//RETURNS : NONE.
	void setBits (uchar* bits, int numBytes);

//PACKAGE : MAP.
//FUNCTION : MImage::numBytes();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : int.
	int numBytes () { return data->GetSize(); }

//PACKAGE : MAP.
//FUNCTION : MImage::colorTable();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : MInArray pointer.
	MInArray * colorTable () { return  ctbl; }

//PACKAGE : MAP.
//FUNCTION : MImage::getColorTable (uint* colors);
//DESCRIPTION : Gets table of color.
//INPUTS : uint pointer.
//RETURNS : NONE.
	void getColorTable (uint* colors);

//PACKAGE : MAP.
//FUNCTION : MImage::setColorTable (uint* colors, int numColors);
//DESCRIPTION : Sets table of color.
//INPUTS : uint pointer, int .
//RETURNS : NONE.
	void  setColorTable (uint* colors, int numColors);

//PACKAGE : MAP.
//FUNCTION : MImage::numColors();
//DESCRIPTION :
//INPUTS : NONE.
//RETURNS : int.
	int numColors () { return ctbl->GetSize(); }

//PACKAGE : MAP.
//FUNCTION : MImage::bitOrder();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : int.	
	int bitOrder () { return bitordr; }

//PACKAGE : MAP.
//FUNCTION : MImage::setBitOrder ( int bitOrder );
//DESCRIPTION : 
//INPUTS : int.
//RETURNS : NONE.
	void setBitOrder ( int bitOrder ) { if ((bitOrder > 0) && (bitOrder < 3)) bitordr = bitOrder; else bitordr = 0; }
	
//PACKAGE : MAP.
//FUNCTION : MImage::hasAlphaBuffer();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : True.
	bool hasAlphaBuffer () { return alpha; }

//PACKAGE : MAP.
//FUNCTION : MImage::setAlphaBuffer ( bool enable );
//DESCRIPTION : 
//INPUTS : bool.
//RETURNS : NONE.
	void setAlphaBuffer ( bool enable ) { alpha = enable; }

//PACKAGE : MAP.
//FUNCTION : MImage::dotsPerMeterX();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : int.
	int dotsPerMeterX () { return dpmx; }

//PACKAGE : MAP.
//FUNCTION : MImage::dotsPerMeterY();
//DESCRIPTION : Constructor.
//INPUTS : NONE.
//RETURNS : int.
	int dotsPerMeterY () { return dpmy; }

//PACKAGE : MAP.
//FUNCTION : MImage::setDotsPerMeterX ( int x );
//DESCRIPTION : 
//INPUTS : int.
//RETURNS : NONE.
	void setDotsPerMeterX ( int x ) { if (x > 0) dpmx = x; else dpmx = 0; } 

//PACKAGE : MAP.
//FUNCTION : MImage::setDotsPerMeterY ( int y );
//DESCRIPTION : 
//INPUTS : int.
//RETURNS : NONE.
	void setDotsPerMeterY ( int y ) { if (y > 0) dpmy = y; else dpmy = 0; }
	
//PACKAGE : MAP.
//FUNCTION : MImage::GetSize();
//DESCRIPTION : Getting the size in bytes.
//INPUTS : NONE.
//RETURNS : int.
	int GetSize() { return sizeof(w) + sizeof(h) + sizeof(d) + sizeof(bitordr) + sizeof(dpmx) + sizeof(dpmy) + sizeof(alpha) + data->GetSize() + ctbl-> GetSize(); }

//PACKAGE : MAP.
//FUNCTION : MImage::GetBytes();
//DESCRIPTION :
//INPUTS : NONE.
//RETURNS : int.
	int GetBytes() {return GetSize(); }

//PACKAGE : MAP.
//FUNCTION : MImage::IsEqualTo(MImage *image);
//DESCRIPTION : 
//INPUTS : MImage pointer.
//RETURNS : True.
	bool IsEqualTo(MImage *image);

//PACKAGE : MAP.
//FUNCTION : MImage::Copy(MImage* image);
//DESCRIPTION : Copies image.
//INPUTS : MImage pointer.
//RETURNS : NONE.
	void Copy(MImage* image);

//PACKAGE : MAP.
//FUNCTION : MImage::WriteBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Writes on UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool WriteBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MImage::ReadBin(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : FILE pointer, int .
//RETURNS : True.
	bool ReadBin(FILE* h_nuxfile, int version); 

//PACKAGE : MAP.
//FUNCTION : MImage::ReadBin(MFile* file, int version);
//DESCRIPTION : Reads from UTP-file.
//INPUTS : MFile pointer, int.
//RETURNS : True.
	bool ReadBin(MFile* file, int version); 

//PACKAGE : MAP.
//FUNCTION : MImage::Clone();
//DESCRIPTION : Clones image.
//INPUTS : NONE.
//RETURNS : MImage pointer.
	virtual MImage* Clone(){ return new MImage(this); };

//PACKAGE : MAP.
//FUNCTION : MImage::format();
//DESCRIPTION : 
//INPUTS : NONE.
//RETURNS : int.
	int format();

//PACKAGE : MAP.
//FUNCTION : MImage::setFormat(int format);
//DESCRIPTION : 
//INPUTS : int.
//RETURNS : NONE.
	void setFormat(int format);

};

//PACKAGE : MAP.
//CLASS	: MImgArray:public GlobalComplexArray<MImage>.
//DESCRIPTION : Designed to hold an array of custom images MImage.
class  MImgArray:public GlobalComplexArray<MImage>
{
public:

//PACKAGE : MAP.
//FUNCTION : MImgArray::CreateOfType();
//DESCRIPTION : It creates an array - an object of class MImgArray.
//INPUTS : NONE.
//RETURNS : MArray pointer.
	virtual MArray* CreateOfType(){return (MArray*) new MImgArray();};

//PACKAGE : MAP.
//FUNCTION : MImgArray::WhatTheArray();
//DESCRIPTION : Returns the ID array MImgArray.
//INPUTS : NONE.
//RETURNS : MString.
	virtual MString WhatTheArray(){return ID_IMG;};
};

#endif //_MIMAGE__H_
