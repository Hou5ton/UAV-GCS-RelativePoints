//PACKAGE : MAP.
//FILE: BaseFun.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _BASEFUN__H_
#define _BASEFUN__H_
#include "BaseCl.h"
#include "ArrBase.h"

//PACKAGE : MAP.
//FUNCTION : WriteString(FILE* h_nuxfile, int version, MString str);
//DESCRIPTION : Saves line to UTP-format file.
//INPUTS : FILE pointer, int , MString .
//RETURNS : True.
bool WriteString(FILE* h_nuxfile, int version, MString str);

//PACKAGE : MAP.
//FUNCTION : ReadString(FILE* h_nuxfile, int version);
//DESCRIPTION : Reads line from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : MString.
MString ReadString(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION :ReadString(MFile* file, int version);
//DESCRIPTION : Reads line from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : MString.
MString ReadString(MFile* file, int version);

//PACKAGE : MAP.
//FUNCTION : RotateByAngle(MPoint* pta,int iPtNum,MPoint ptRotateAround,double radangle);
//DESCRIPTION : Rotate array of point by fixed angle around fixed point.
//INPUTS : MPoint ,int ,MPoint ,double .
//RETURNS : NONE.
void RotateByAngle(MPoint* pta,int iPtNum,MPoint ptRotateAround,double radangle);

//PACKAGE : MAP.
//FUNCTION : CreateBaseArrayByType(MString strType);
//DESCRIPTION : Creates base Array of defined type.
//INPUTS : MString .
//RETURNS : MArray pointer.
MArray* CreateBaseArrayByType(MString strType);

#endif //_BASEFUN__H_

