#include "BaseFun.h"
#include "Prosloiki.h"
#include "plasti.h"
#include "Formula.h"
#include "srezi.h"
#include "privpoints.h"
#include "Refglobal.h"

//! записъ MString
bool WriteString(FILE* h_nuxfile, int version, MString str)
{
	unsigned short i=0;
	switch( version ) {
        //version 0
		case 0:
			i = str.GetLength();
            //writing a byte with the length of the string and the string itself
			fwrite(&i, 2, 1, h_nuxfile);
			fwrite(str, 1, i, h_nuxfile);
	}
	return true;
}
//==================================		
//!reading  MString
MString ReadString(FILE* h_nuxfile, int version)
{
	unsigned short	i = 0;
	MString		str_ret="";
	char*		str;
	switch( version ) {
        //version 0
		case 0:
            //reading a byte with the length of the string and the string itself
			fread(&i, 2, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&i, sizeof(i) );
            //creates an array of length equal to the reading length
			str = new char[i+1];
			str[i]='\0';
			if(!str)	return str_ret;
			memset(str,' ',i);
			fread(str, 1, i, h_nuxfile);
            //conversion to MString with removing breaks
			str_ret = str;
			str_ret.TrimRight();
			delete[] str;
	}
	return str_ret;
}

//==================================		
//!reading  MString
MString ReadString(MFile* file, int version)
//==================================		
{
	unsigned short	i = 0;
	MString		str_ret="";
	char*		str;
	switch( version ) {
        //version 0
		case 0:
            //reading a byte with the length of the string and the string itself
			file->GetData(&i, 2, 1);
		if (reverseReadFlag) reverseValue((char*)&i, sizeof(i) );
            //creates an array of length equal to the reading length
			str = new char[i+1];
			str[i]='\0';
			if(!str)	return str_ret;
			memset(str,' ',i);
			file->GetData(str, 1, i);
            //conversion to MString with removing breaks
			str_ret = str;
			str_ret.TrimRight();
			delete[] str;
	}
	return str_ret;
}

//===========================================================================
//!Function of rotation of the array around some point
//! at a predetermined angle clockwise
void RotateByAngle(MPoint* pta,int iPtNum,MPoint ptRotateAround,double radangle)
//===========================================================================
{
	double dX,dY,x1,y1;
//Make the rotation for each point
	for(int i = 0;i<iPtNum;i++){
        //difference of distances on both axes
		dX = (pta+i)->x - ptRotateAround.x;
		dY = (pta+i)->y - ptRotateAround.y;
        //rotation
		x1 = dX*cos(radangle)+dY*sin(radangle);
		y1 = -dX*sin(radangle)+dY*cos(radangle);
        //after rotation
		(pta+i)->x = ptRotateAround.x +x1;
		(pta+i)->y = ptRotateAround.y +y1;
	}
}


// ==================================================================
//! Create arrays that are stored in the module
MArray* CreateBaseArrayByType(MString strType)
// ==================================================================
{
	if(!(strcmp(strType, ID_PTA)))
	{
		return new MPtArray();
	}

	else if(!(strcmp(strType, ID_STA)))
	{
		return new MStArray();
	}
	else if(!(strcmp(strType, ID_SMA)))
	{
		return new MSmArray();
	}

	else if(!(strcmp(strType, ID_PKA)))
	{
		return new MPkArray();
	}
	else if(!(strcmp(strType, ID_IMG)))
	{
		return new MImgArray();
	}
	else if(!(strcmp(strType, ID_INA))){
		return new MInArray();
	}
	else if(!(strcmp(strType, ID_BYA))){
		return new MByArray();
	}
	else if(!(strcmp(strType, ID_DUA)))
	{
		return new MDuArray();
	}
	else if(!(strcmp(strType, ID_CLA)))
	{
		return new MClArray();
	}
	else if(!(strcmp(strType, ID_BOA)))
	{
		return new MBoArray();
	}
	else if(!(strcmp(strType, ID_ALA)))
	{
		return new MAlArray();
	}
	else if(!(strcmp(strType, ID_PLA)))
	{
		return new MPlArray();
	}
	else if(!(strcmp(strType, ID_PRA)))
	{
		return new MPrArray();
	}
	else if(!(strcmp(strType, ID_FRA)))
	{
		return new MFrArray();
	}
	else if(!(strcmp(strType, ID_FMA)))
	{
		return new MFormArray();
	}
	else if(!(strcmp(strType, ID_RKA)))
	{
		return new MRkArray();
	}
	else if(!(strcmp(strType, ID_LFA)))
	{
		return new MLfArray();
	}
	else if(!(strcmp(strType, ID_SRZ)))
	{
		return new MSzArray();
	}
	else if(!(strcmp(strType, ID_NVA)))
	{
		return new MNvArray();
	}
	else if(!(strcmp(strType, ID_USA)))
	{
		return new MShInArray();
	}
	else if(!(strcmp(strType, ID_PGN)))
	{
		return new MPgArray();
	}
	else if(!(strcmp(strType, ID_PVA)))
	{
		return new MPvArray();
	}
	return NULL;
}
