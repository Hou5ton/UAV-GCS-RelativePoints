//The file providing realization of various forms of a binding
#include "privpoints.h"
#include "BaseFun.h"
#include "Refglobal.h"
//===================================================================
//! Writing of the general form of a binding
//params (MFile* h_nuxfile,int version)
bool MPriv::WriteBin(FILE* h_nuxfile,int )
//===================================================================
{
	unsigned char bType = GetType();
	fwrite(&bType, sizeof(bType), 1, h_nuxfile);
	fwrite(&ush_formNumber, sizeof(ush_formNumber), 1, h_nuxfile);
	MPoint* pPoints = GetPoints();
	unsigned short ush_size = GetPointNum();
	fwrite(&ush_size, sizeof(ush_size), 1, h_nuxfile);
	fwrite(pPoints, sizeof(MPoint), ush_size, h_nuxfile);
	return true;
}

//===================================================================
//!Reading the general form of a binding
//params (MFile* h_nuxfile,int version)
bool MPriv::ReadBin(FILE* h_nuxfile,int )
//===================================================================
{
	fread(&ush_formNumber, sizeof(ush_formNumber), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&ush_formNumber, sizeof(ush_formNumber) );
	unsigned short ush_size = 0;
	fread(&ush_size, sizeof(ush_size), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&ush_size, sizeof(ush_size) );
	MPoint* pPoints = (MPoint*)malloc(ush_size*sizeof(MPoint));
	fread(pPoints, sizeof(MPoint), ush_size, h_nuxfile);
		if (reverseReadFlag)  {
      MPoint *pnt = pPoints;
		  for(int j=0; j<ush_size; j++) {
		    reverseValue((char*)&pnt->x, sizeof(pnt->x) );
		    reverseValue((char*)&pnt->y, sizeof(pnt->y) );
        pnt++;
      }
    }
	
	SetPoints(pPoints,ush_size);
	free(pPoints);
	return true;
}

//===================================================================
//!Reading the general form of a binding
//params (MFile* file,int version)
bool MPriv::ReadBin(MFile* file,int )
//===================================================================
{
	file->GetData(&ush_formNumber, sizeof(ush_formNumber), 1);
		if (reverseReadFlag) reverseValue((char*)&ush_formNumber, sizeof(ush_formNumber) );
	unsigned short ush_size;
	file->GetData(&ush_size, sizeof(ush_size), 1);
		if (reverseReadFlag) reverseValue((char*)&ush_size, sizeof(ush_size) );
	MPoint* pPoints = (MPoint*)malloc(ush_size*sizeof(MPoint));
	file->GetData(pPoints, sizeof(MPoint), ush_size);
		if (reverseReadFlag)  {
      MPoint *pnt = pPoints;
		  for(int j=0; j<ush_size; j++) {
		    reverseValue((char*)&pnt->x, sizeof(pnt->x) );
		    reverseValue((char*)&pnt->y, sizeof(pnt->y) );
        pnt++; 
      }
    }  
	SetPoints(pPoints,ush_size);
	free(pPoints);
	return true;
}

//===================================================================
//!Reading the general form of a binding
MPriv* MPriv::ReadAny(FILE* h_nuxfile,int version)
//===================================================================
{
	unsigned char bType;
	fread(&bType, sizeof(bType), 1, h_nuxfile);
    //Creation of a binding on type
	MPriv* pPriv = CreatePrivByType(bType);
	pPriv->ReadBin(h_nuxfile,version);
	return pPriv;
}

//===================================================================
//!Reading the general form of a binding
MPriv* MPriv::ReadAny(MFile* file,int version)
//===================================================================
{
	unsigned char bType;
	file->GetData(&bType, sizeof(bType), 1);
    //Creation of a binding on type
	MPriv* pPriv = CreatePrivByType(bType);
	pPriv->ReadBin(file,version);
	return pPriv;
}

//! --
MPriv::operator MSimplePriv()
{
	if(GetType()==SIMPLE_PRIVYAZKA)
		return *this;
	MPoint* pta = GetPoints();
	return MSimplePriv(pta[0],GetFormNomer());
}
//! --
MPriv::operator MDoublePriv()
{
	if(GetType()==DOUBLE_PRIVYAZKA)
		return *this;
	MPoint* pta = GetPoints();
	if(GetPointNum()>=2)
		return MDoublePriv(pta[0],pta[1],GetFormNomer());
	else
		return MDoublePriv(pta[0],pta[0],GetFormNomer());
}

//! --
MPriv::operator MVinoskaPriv()
{
	if(GetType()==VINOSKA_PRIVYAZKA)
		return *this;
	MPoint* pta = GetPoints();
	if(GetPointNum()>=3)
		return MVinoskaPriv(pta[0],pta[1],pta[2],GetFormNomer());
	else if(GetPointNum()>=2)
		return MVinoskaPriv(pta[0],pta[1],pta[1],GetFormNomer());
	else
		return MVinoskaPriv(pta[0],pta[0],pta[0],GetFormNomer());
}
//! --
MPriv::operator MSplinePriv()
{
	if(GetType()==SPLINE_PRIVYAZKA)
		return *this;
	MPoint* pta = GetPoints();
	if(GetPointNum()>=4)
		return MSplinePriv(pta[0],pta[1],pta[2],pta[3],GetFormNomer());
	else if(GetPointNum()>=3)
		return MSplinePriv(pta[0],pta[1],pta[2],pta[2],GetFormNomer());
	else if(GetPointNum()>=2)
		return MSplinePriv(pta[0],pta[1],pta[1],pta[1],GetFormNomer());
	else
		return MSplinePriv(pta[0],pta[0],pta[0],pta[0],GetFormNomer());
}




//=========================================================
//! Reads the array from the NUX file
bool MPvArray::ReadBin(FILE* h_nuxfile, int version)
{
	switch (version)
	{
	case 0:
        //we pass record length
		fseek(h_nuxfile, 3, SEEK_CUR);
        //We read quantity of elements
		unsigned int Cnt = 0;
		fread(&Cnt, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
		MPriv* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MPriv::ReadAny(h_nuxfile,version);

			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//=========================================================
//! Reads the array from the NUX file
bool MPvArray::ReadBin(MFile* file, int version)
{
	switch (version)
	{
	case 0:
        //we pass record length
		file->SetPosition(3, MAP_SEEK_CUR);
        //We read quantity of elements
		unsigned int Cnt = 0;
		file->GetData(&Cnt, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&Cnt, sizeof(Cnt) );
		SetSize(Cnt);
        //We read lines
		MPriv* pAtom;
		for(int i=0;i<Size;i++)
		{
			pAtom = MPriv::ReadAny(file,version);

			SetAt(i,pAtom);
			delete pAtom;
		}
	}
	return true;
}

//===============================================================
//!Turns on the specified angle concerning the center
void MPriv::RotateAroundCenter(double dRadangle)
//===============================================================
{
	MPoint* pta = GetPoints();
    //We find the center
	MPoint ptCenter;
	unsigned short ptNum = GetPointNum();
	for(int i = ptNum-1;i>=0;i--)
		ptCenter+=*(pta+i);
	ptCenter.x = ptCenter.x/ptNum;
	ptCenter.y = ptCenter.y/ptNum;
	RotateByAngle(pta,ptNum,ptCenter,dRadangle);
}

//===================================================================
//!Sets turn of a vector of a binding
//! generally the angle of the first vector of a binding is established
void MPriv::SetAtAnAngle(double dRadangle)
//===================================================================
{
    //We find a angle, under which otnositeljno the center
    //there is the first segment of a binding
	MPoint* pta = GetPoints();
	int iNum = GetPointNum();
	if(iNum<2)
		return;
	MPoint ptCenter = pta[0];
	ptCenter+=pta[1];
	ptCenter.x/=2;
	ptCenter.y/=2;
    //vector angle
	double dCurAngle = atan((pta[1].y - ptCenter.y)/(pta[1].x-ptCenter.x));
	RotateByAngle(pta,iNum,ptCenter,-(dRadangle - dCurAngle));
}




//! Function of creation of a binding on type
MPriv* CreatePrivByType(unsigned char bType){
	switch(bType){
	case SIMPLE_PRIVYAZKA:
		return (MPriv*)new MSimplePriv();
	case DOUBLE_PRIVYAZKA:
		return (MPriv*)new MDoublePriv();
	case VINOSKA_PRIVYAZKA:
		return (MPriv*)new MVinoskaPriv();
	case SPLINE_PRIVYAZKA:
		return (MPriv*)new MSplinePriv();
//	default:		exit(15);
	}
	return NULL;
}
