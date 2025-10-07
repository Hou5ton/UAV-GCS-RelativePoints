//PACKAGE : MAP.
//FILE: PatternConvertor.h   
//AUTHOR:
//DESCRIPTION:

#ifndef _PATTERNCONVERTOR__H_
#define _PATTERNCONVERTOR__H_

#include <qimage.h>
#include "MImage.h"

//PACKAGE : MAP.
//CLASS : PatternConvertor.
//DESCRIPTION : 
class PatternConvertor
{
public:

//PACKAGE : MAP.
//FUNCTION : PatternConvertor::Convert(MImage *mImg, QImage &qImg).
//DESCRIPTION : 
//INPUTS : MImage pointer, QImage &.
//RETURNS : NONE
	static void Convert(MImage *mImg, QImage &qImg);

//PACKAGE : MAP.
//FUNCTION : PatternConvertor::Convert(QImage qImg, MImage *mImg).
//DESCRIPTION :
//INPUTS : QImage , MImage pointer.
//RETURNS : NONE
	static void Convert(QImage qImg, MImage *mImg);
};

#endif //_PATTERNCONVERTOR__H_