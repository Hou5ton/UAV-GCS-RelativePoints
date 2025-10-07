#include <math.h>
#include "BaseCl.h"
#include "BaseFun.h"
#include "SysDefine.h"
#include "Refglobal.h"
//===============================================================================
// MPrk
//===============================================================================
//! EN: Reading Prk(Writing Style)
bool MPrk::ReadPrk(FILE* h_nuxfile, int version)
{
	short	param = 0; 
	switch( version ) {
        //version 0
		case 0:
            //Read rendering parameters
			fread(&param, 2, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&param, sizeof(param) );
            if (param & 0x0001)			// bit scalability
				scaled = true;
			else scaled = false;
            if (param & 0x0002)			// bit imaging
				visible = true;
			else visible = false;
            if (param & 0x0004)			// bit transparency
				bkmode = true;
			else bkmode = false;
            if (param & 0x0008)			// bit "Текст курсив"
				logfont.lfItalic = true;
			else logfont.lfItalic = false;
            if (param & 0x0010)			// bit "Текст жирный"
				logfont.lfWeight = FW_BOLD;
			else logfont.lfWeight = FW_NORMAL;
            if (param & 0x0020)			// bit "Текст подчеркнутый"
				logfont.lfUnderline = true;
			else logfont.lfUnderline = false;
            //reading mode of alignment
			//align = 0;

			align = (align & 0xFFC0) | ((param & 0x07C0) >> 5);

// 2005.05.28 BELOW CODE - Only for Linux/Unix
//#ifndef _WIN32
			int tmp_align = 0;

			if ((align & W_TA_BASELINE) == W_TA_BASELINE)
				tmp_align |= UT_TA_BASELINE;
			else
				if ((align & W_TA_BOTTOM) == W_TA_BOTTOM)
					tmp_align |= UT_TA_BOTTOM;
				else
					if ((align & W_TA_TOP) == W_TA_TOP)
						tmp_align |= UT_TA_TOP;

			if ((align & W_TA_CENTER) == W_TA_CENTER)
				tmp_align |= UT_TA_CENTER;
			else
				if ((align & W_TA_RIGHT) == W_TA_RIGHT)
					tmp_align |= UT_TA_RIGHT;		
				else
					if ((align & W_TA_LEFT) == W_TA_LEFT)
						tmp_align |= UT_TA_LEFT;			

			align = tmp_align;
//#endif
            //reading mode background
			if(param&0x0800)
				bkmode = UT_OPAQUE;
			else
				bkmode = UT_TRANSPARENT;
            //reading of the character set
			fread(&logfont.lfCharSet, 1, 1, h_nuxfile);
            //reading character size
			fread(&value, sizeof(float), 1, h_nuxfile);
        		if (reverseReadFlag) reverseValue((char*)&value, sizeof(value)  );
            //reading table code background color and text
			fread(&bkcolorkod, 1, 1, h_nuxfile);
			fread(&colorkod, 1, 1, h_nuxfile);
            //reading the font name
			fread(&logfont.lfFaceName, 32, 1, h_nuxfile);
		}
	return true; 
}

//! ==================================
//! EN: Reading Prk(Writing Style)
bool MPrk::ReadPrk(MFile* file, int version)
//==================================		
{
	short	param = 0;
	switch( version ) {
        //version 0
		case 0:
            //Read rendering parameters
			file->GetData(&param, 2, 1);
		if (reverseReadFlag) reverseValue((char*)&param, sizeof(param) );
            if (param & 0x0001)			// bit scalability
				scaled = true;
			else scaled = false;
            if (param & 0x0002)			// bit imaging
				visible = true;
			else visible = false;
            if (param & 0x0004)			// bit transparency
				bkmode = true;
			else bkmode = false;
            if (param & 0x0008)			// bit "Текст курсив"
				logfont.lfItalic = true;
			else logfont.lfItalic = false;
            if (param & 0x0010)			// bit "Текст жирный"
				logfont.lfWeight = FW_BOLD;
			else logfont.lfWeight = FW_NORMAL;
            if (param & 0x0020)			// bit "Текст подчеркнутый"
				logfont.lfUnderline = true;
			else logfont.lfUnderline = false;
            //reading mode of alignment
			//align = 0;

			align = (align & 0xFFC0) | ((param & 0x07C0) >> 5);

// 2005.05.28 BELOW CODE - Only for Linux/Unix
//#ifndef _WIN32
			int tmp_align = 0;

			if ((align & W_TA_BASELINE) == W_TA_BASELINE)
				tmp_align |= UT_TA_BASELINE;
			else
				if ((align & W_TA_BOTTOM) == W_TA_BOTTOM)
					tmp_align |= UT_TA_BOTTOM;
				else
					if ((align & W_TA_TOP) == W_TA_TOP)
						tmp_align |= UT_TA_TOP;

			if ((align & W_TA_CENTER) == W_TA_CENTER)
				tmp_align |= UT_TA_CENTER;
			else
				if ((align & W_TA_RIGHT) == W_TA_RIGHT)
					tmp_align |= UT_TA_RIGHT;		
				else
					if ((align & W_TA_LEFT) == W_TA_LEFT)
						tmp_align |= UT_TA_LEFT;			

			align = tmp_align;
//#endif
            //reading mode background
			if(param&0x0800)
				bkmode = UT_OPAQUE;
			else
				bkmode = UT_TRANSPARENT;
            //reading of the character set
			file->GetData(&logfont.lfCharSet, 1, 1);
            //reading character size
			file->GetData(&value, sizeof(float), 1);
        		if (reverseReadFlag) reverseValue((char*)&value, sizeof(value)  );
            //reading table code background color and text
			file->GetData(&bkcolorkod, 1, 1);
			file->GetData(&colorkod, 1, 1);
            //reading the font name
			file->GetData(&logfont.lfFaceName, 32, 1);
		}
	return true; 
}


//!==================================
//! EN: Writing Prk(Writing Style) to Kls-file
bool MPrk::WritePrk(FILE* h_nuxfile, int version) 
//==================================		
{
	short	param = 0;
	char	strbuf[32] = "                               ";

	switch( version ) {
        //version 0
		case 0:
            //Write rendering parameters
            if (scaled)						// bit scalability
				param = param | 0x0001;
            if (visible)					// bit imaging
				param = param | 0x0002;
            if (bkmode)						// bit transparency
				param = param | 0x0004;
            if (logfont.lfItalic)			// bit "Текст курсив"
				param = param | 0x0008;
            if (logfont.lfWeight == FW_BOLD)	// bit "Текст жирный"
				param = param | 0x0010;
            if (logfont.lfUnderline)		// bit "Текст подчеркнутый"
				param = param | 0x0020;
            //writing mode of alignment
			param = param | ((align & 0x003E) << 5);
			if(bkmode!=UT_TRANSPARENT)
				param|=0x0800;
			else
				param&=0xf7ff;
            //Write rendering parameters
			fwrite(&param, 2, 1, h_nuxfile);
            //Writing character set
			fwrite(&logfont.lfCharSet, 1, 1, h_nuxfile);
            //Writing character size
			fwrite(&value, sizeof(float), 1, h_nuxfile);
            //Writing table code background color and text
			fwrite(&bkcolorkod, 1, 1, h_nuxfile);
			fwrite(&colorkod, 1, 1, h_nuxfile);
            //Writing the font name
			strcpy((char*) &strbuf, (char*) &logfont.lfFaceName);
			fwrite(&strbuf, 32, 1, h_nuxfile);
	}
	return true;
}

//===============================================================================
// Functions of class MPrkExt read / write handwriting
// MPrkExt
//===============================================================================

//!==================================
//! EN: Reading MExtPrk(Writing Style) from Kls-file
bool MPrkExt::ReadPrk(FILE* h_nuxfile, int version)
//==================================		
{
	short	param = 0;
	switch( version ) {
        //version 0
		case 0:
            //read rendering parameters
			fread(&param, 2, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&param, 4 );
            if (param & 0x0001)			// bit scalability
				scaled = true;
			else scaled = false;
            if (param & 0x0002)			// bit imaging
				visible = true;
			else visible = false;
            if (param & 0x0004)			// bit transparency
				bkmode = true;
			else bkmode = false;
            if (param & 0x0008)			// bit "Текст курсив"
				logfont.lfItalic = true;
			else logfont.lfItalic = false;
			if (param & 0x0010)			// бит "Текст жирный" 
				logfont.lfWeight = FW_BOLD;
			else logfont.lfWeight = FW_NORMAL;
			if (param & 0x0020)			// бит "Текст подчеркнутый" 
				logfont.lfUnderline = true;
			else logfont.lfUnderline = false;
            //reading mode of alignment
			//align = 0;

			align = (align & 0xFFC0) | ((param & 0x07C0) >> 5);

			unsigned int tmp_align = 0;

			if ((align & W_TA_BASELINE) == W_TA_BASELINE)
				tmp_align |= UT_TA_BASELINE;
			else
				if ((align & W_TA_BOTTOM) == W_TA_BOTTOM)
					tmp_align |= UT_TA_BOTTOM;
				else
					if ((align & W_TA_TOP) == W_TA_TOP)
						tmp_align |= UT_TA_TOP;

			if ((align & W_TA_CENTER) == W_TA_CENTER)
				tmp_align |= UT_TA_CENTER;
			else
				if ((align & W_TA_RIGHT) == W_TA_RIGHT)
					tmp_align |= UT_TA_RIGHT;		
				else
					if ((align & W_TA_LEFT) == W_TA_LEFT)
						tmp_align |= UT_TA_LEFT;	

			align = tmp_align;

            //reading mode background
			if(param&0x0800)
				bkmode = UT_OPAQUE;
			else
				bkmode = UT_TRANSPARENT;
            //reading of the character set
			fread(&logfont.lfCharSet, 1, 1, h_nuxfile);
            //reading character size
			fread(&value, 8, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&value, 8 );
            //reading table code background color and text
			unsigned char t = 0;
			fread(&t, 1, 1, h_nuxfile); bkcolorkodExt.SetR(t);
			fread(&t, 1, 1, h_nuxfile); bkcolorkodExt.SetG(t);
			fread(&t, 1, 1, h_nuxfile); bkcolorkodExt.SetB(t);
			fread(&t, 1, 1, h_nuxfile); bkcolorkodExt.SetA(t);

			fread(&t, 1, 1, h_nuxfile); colorkodExt.SetR(t);
			fread(&t, 1, 1, h_nuxfile); colorkodExt.SetG(t);
			fread(&t, 1, 1, h_nuxfile); colorkodExt.SetB(t);
			fread(&t, 1, 1, h_nuxfile); colorkodExt.SetA(t);
            //reading the font name
			fread(&logfont.lfFaceName, 32, 1, h_nuxfile);
		}
	return true; 
}

//!==================================
bool MPrkExt::ReadPrk(MFile* file, int version)
//==================================		
{
	short	param = 0;
	switch( version ) {
        //version 0
		case 0:
            //read rendering parameters
			file->GetData(&param, 2, 1);
		if (reverseReadFlag) reverseValue((char*)&param, sizeof(param) );
            if (param & 0x0001)			// bit scalability
				scaled = true;
			else scaled = false;
            if (param & 0x0002)			// bit imaging
				visible = true;
			else visible = false;
            if (param & 0x0004)			// bit transparency
				bkmode = true;
			else bkmode = false;
            if (param & 0x0008)			// bit "Текст курсив"
				logfont.lfItalic = true;
			else logfont.lfItalic = false;
            if (param & 0x0010)			// bit "Текст жирный"
				logfont.lfWeight = FW_BOLD;
			else logfont.lfWeight = FW_NORMAL;
            if (param & 0x0020)			// bit "Текст подчеркнутый"
				logfont.lfUnderline = true;
			else logfont.lfUnderline = false;
            //reading mode of alignment
			//align = 0;

			align = (align & 0xFFC0) | ((param & 0x07C0) >> 5);

			unsigned int tmp_align = 0;

			if ((align & W_TA_BASELINE) == W_TA_BASELINE)
				tmp_align |= UT_TA_BASELINE;
			else
				if ((align & W_TA_BOTTOM) == W_TA_BOTTOM)
					tmp_align |= UT_TA_BOTTOM;
				else
					if ((align & W_TA_TOP) == W_TA_TOP)
						tmp_align |= UT_TA_TOP;

			if ((align & W_TA_CENTER) == W_TA_CENTER)
				tmp_align |= UT_TA_CENTER;
			else
				if ((align & W_TA_RIGHT) == W_TA_RIGHT)
					tmp_align |= UT_TA_RIGHT;		
				else
					if ((align & W_TA_LEFT) == W_TA_LEFT)
						tmp_align |= UT_TA_LEFT;	

			align = tmp_align;

            //reading mode background
            if(param&0x0800)
				bkmode = UT_OPAQUE;
			else
				bkmode = UT_TRANSPARENT;
            //reading of the character set
            file->GetData(&logfont.lfCharSet, 1, 1);
            //reading character size
            file->GetData(&value, 8, 1);
		if (reverseReadFlag) reverseValue((char*)&value, sizeof(value) );
        //reading table code background color and text
            unsigned char t = 0;
			file->GetData(&t, 1, 1); bkcolorkodExt.SetR(t);
			file->GetData(&t, 1, 1); bkcolorkodExt.SetG(t);
			file->GetData(&t, 1, 1); bkcolorkodExt.SetB(t);
			file->GetData(&t, 1, 1); bkcolorkodExt.SetA(t);

			file->GetData(&t, 1, 1); colorkodExt.SetR(t);
			file->GetData(&t, 1, 1); colorkodExt.SetG(t);
			file->GetData(&t, 1, 1); colorkodExt.SetB(t);
			file->GetData(&t, 1, 1); colorkodExt.SetA(t);
            //reading the font name
			file->GetData(&logfont.lfFaceName, 32, 1);
		}
	return true; 
}

//!==================================
bool MPrkExt::WritePrk(FILE* h_nuxfile, int version) 
//==================================		
{
	short	param = 0;
	char	strbuf[32] = "                               ";

	switch( version ) {
        //version 0
		case 0:
            //write rendering parameters
            if (scaled)						// bit scalability
				param = param | 0x0001;
            if (visible)					// bit imaging
				param = param | 0x0002;
            if (bkmode)						// bit transparency
				param = param | 0x0004;
            if (logfont.lfItalic)			// bit "Текст курсив"
				param = param | 0x0008;
            if (logfont.lfWeight == FW_BOLD)	// bit "Текст жирный"
				param = param | 0x0010;
            if (logfont.lfUnderline)		// bit "Текст подчеркнутый"
				param = param | 0x0020;
            //writing mode of alignment
			param = param | ((align & 0x003E) << 5);
			if(bkmode!=UT_TRANSPARENT)
				param|=0x0800;
			else
				param&=0xf7ff;
            //writing rendering parameters
			fwrite(&param, 2, 1, h_nuxfile);
            //writing of the character set
			fwrite(&logfont.lfCharSet, 1, 1, h_nuxfile);
            //writing character size
			fwrite(&value, 8, 1, h_nuxfile);
            //writing table code background color and text
			unsigned char t = 0;
			t = bkcolorkodExt.GetR(); fwrite(&t, 1, 1, h_nuxfile);
			t = bkcolorkodExt.GetG(); fwrite(&t, 1, 1, h_nuxfile);
			t = bkcolorkodExt.GetB(); fwrite(&t, 1, 1, h_nuxfile);
			t = bkcolorkodExt.GetA(); fwrite(&t, 1, 1, h_nuxfile);

			t = colorkodExt.GetR(); fwrite(&t, 1, 1, h_nuxfile);
			t = colorkodExt.GetG(); fwrite(&t, 1, 1, h_nuxfile);
			t = colorkodExt.GetB(); fwrite(&t, 1, 1, h_nuxfile);
			t = colorkodExt.GetA(); fwrite(&t, 1, 1, h_nuxfile);

            //writing the font name
			strcpy((char*) &strbuf, (char*) &logfont.lfFaceName);
			fwrite(&strbuf, 32, 1, h_nuxfile);
	}
	return true;
}



//===============================================================================
// MSymbol
//===============================================================================


//!==================================
bool MSymbol::ReadSymbol(FILE* h_nuxfile, int version)
//==================================		
{
////	int			param = 0;
	char sym;

	switch( version ) {
        //version 0
		case 0:
            //read handwriting
			ReadPrk(h_nuxfile,version);
            //read character stroke
			fread(&sym, 1, 1, h_nuxfile);
			str = sym;
	}
	return true; 
}

//!==================================
bool MSymbol::ReadSymbol(MFile* file, int version)
//==================================		
{
////	int			param = 0;
	char sym;

	switch( version ) {
        //version 0
		case 0:
            //read handwriting
            ReadPrk(file,version);
            //read character stroke
            file->GetData(&sym, 1, 1);
			str = sym;
	}
	return true; 
}
//!==================================
bool MSymbol::WriteSymbol(FILE* h_nuxfile, int version) 
//==================================		
{
////	int		param = 0;
////	char	strbuf[32] = "                               ";
	char	sym;
	switch( version ) {
        //version 0
		case 0:
            //write handwriting
            WritePrk(h_nuxfile,version);
            //write symbol
			sym = str.GetAt(0);
			fwrite(&sym, 1, 1, h_nuxfile);
	}
	return true;
}

//!==================================
int MSymbol::GetBytes()
//==================================	
{
	int Size = MPrk::GetBytes();
	Size += 1;//str.GetLength() + 2;

	return Size;
}


//===============================================================================
// MSymbolExt
//===============================================================================

//!==================================
bool MSymbolExt::ReadSymbol(FILE* h_nuxfile, int version)
//==================================		
{
////	int			param = 0;
	char sym;

	switch( version ) {
        //version 0
		case 0:
            //read handwriting
			ReadPrk(h_nuxfile,version);
            //read character stroke
            fread(&sym, 1, 1, h_nuxfile);
			str = sym;
	}
	return true; 
}

//!==================================
bool MSymbolExt::ReadSymbol(MFile* file, int version)
//==================================		
{
////	int			param = 0;
	char sym;

	switch( version ) {
        //version 0
		case 0:
            //read handwriting
            ReadPrk(file,version);
            //read character stroke
            file->GetData(&sym, 1, 1);
			str = sym;
	}
	return true; 
}

//!==================================
bool MSymbolExt::WriteSymbol(FILE* h_nuxfile, int version) 
//==================================		
{
////	int		param = 0;

////	char	strbuf[32] = "                               ";
	char	sym;

	switch( version ) {
        //version 0
		case 0:
            //write handwriting
			WritePrk(h_nuxfile,version);
            //write symbol
			sym = str.GetAt(0);
			fwrite(&sym, 1, 1, h_nuxfile);
	}
	return true;
}

//!==================================
int MSymbolExt::GetBytes()
//==================================	
{
	int Size = MPrkExt::GetBytes();
	Size += str.GetLength() + 2;

	return Size;
}


//=============================================================
// MPredlogenie - base class for formulas
//=============================================================
//==================================
//!constructor with the specified standard
MPredlogenie::MPredlogenie(unsigned short nompolename, unsigned char nomprefix, unsigned char nompostfix)
//==================================
{
	m_nomprefix = nomprefix;
	m_nompolename = nompolename;
	m_nompostfix = nompostfix;
}
//==================================
//!constructor with the specified standard
MPredlogenie::MPredlogenie(unsigned char nomprefix,unsigned short nompolename, unsigned char nompostfix,
		unsigned char nomprk)
//==================================	      
{
	m_nomprefix = nomprefix;
	m_nompolename = nompolename;
	m_nompostfix = nompostfix;
	m_nomprk = nomprk;
}

//==================================
//! copy function
void MPredlogenie::Copy(MPredlogenie* predlog)
//==================================
{
    //copy , if you have something to copy
	if(predlog != NULL){
		m_nomprefix = predlog->m_nomprefix;
		m_nompolename = predlog->m_nompolename;
		m_nompostfix = predlog->m_nompostfix;
		m_nomprk = predlog->m_nomprk;
	}
}

//====================================================
//!writing
bool MPredlogenie::WriteBin(FILE* h_nuxfile, int version)
//====================================================
{
    //Determine the size of the segment
	unsigned int Size=GetBytes(); 
	switch(version)
	{
	case 0:
        //Write the segment size
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write code prefix
		fwrite(&m_nomprefix, 1, 1, h_nuxfile);
        //Write the code name of the field
		fwrite(&m_nompolename, sizeof(unsigned short), 1, h_nuxfile);
        //Write code for postfix
		fwrite(&m_nompostfix, 1, 1, h_nuxfile);
		fwrite(&m_nomprk, 1, 1, h_nuxfile);
	}

	return true;
}

//====================================================
//!writing
bool MPredlogenie::ReadBin(FILE* h_nuxfile, int version)
//====================================================
{
    //Depending on the version
	switch (version){
		case 0:
        //sequentially read all terms
		unsigned int SIZE= 0;
        //Reading the record length
		fread(&SIZE, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
        //Reading vocabulary prefix code
		fread(&m_nomprefix, 1, 1, h_nuxfile);
        //Read the dictionary code name field
		fread(&m_nompolename, sizeof(m_nompolename), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_nompolename, sizeof(m_nompolename) );
        //Reading dictionary code postfix
		fread(&m_nompostfix, 1, 1, h_nuxfile);
        //Read dictionary  code of handwriting sentence
		fread(&m_nomprk, 1, 1, h_nuxfile);
	}

	return true;
}

//====================================================
//!чтение
bool MPredlogenie::ReadBin(MFile* file, int version)
//====================================================
{
    //Depending on the version
    switch (version){
		case 0:
        //sequentially read all terms
        unsigned int SIZE= 0;
        //Reading the record length
        file->GetData(&SIZE, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
        //Reading vocabulary prefix code
        file->GetData(&m_nomprefix, 1, 1);
        //Read the dictionary code name field
        file->GetData(&m_nompolename, sizeof(unsigned short), 1);
		if (reverseReadFlag) reverseValue((char*)&m_nompolename, sizeof(m_nompolename) );
        //Reading dictionary code postfix
        file->GetData(&m_nompostfix, 1, 1);
        //Read dictionary  code of handwriting sentence
        file->GetData(&m_nomprk, 1, 1);
	}

	return true;
}

//====================================================
//!cloning
MPredlogenie* MPredlogenie::Clone()
//====================================================
{
	MPredlogenie* pClone = new MPredlogenie();
	
	pClone->Copy(this);
	
	return pClone;
}

//====================================================
//!size in UTP-format
int MPredlogenie::GetBytes()
//====================================================
{
	int size = 3*sizeof(unsigned char) + sizeof(unsigned short);

	return size;
}

//====================================================
//!function of getting the index (code) handwriting sentences
unsigned char MPredlogenie::GetNomPrk()
//====================================================
{
	unsigned char nomprk;
	nomprk = m_nomprk;
	return nomprk;
}

//====================================================
//!function of establishing the index (code) handwriting sentences
bool MPredlogenie::SetNomPrk(unsigned char nomprk)
//====================================================
{
	m_nomprk = nomprk;
	return true;
}

//====================================================
//!function of getting dictionary index (code) field name
unsigned short MPredlogenie::GetNomPole()
//====================================================
{
	unsigned short nompole;
	nompole = m_nompolename;
	return nompole;
}

//====================================================
//!function of establishing dictionary index (code) field name
bool MPredlogenie::SetNomPole(unsigned short nompole)
//====================================================
{
	m_nompolename = nompole;
	return true;
}

//====================================================
//!function of getting dictionary index (code) prefix
unsigned char MPredlogenie::GetNomPrefix()
//====================================================
{
	unsigned char nomprefix;
	nomprefix = m_nomprefix;
	return nomprefix;
}

//====================================================
//!function of establishing dictionary index (code) prefix
bool MPredlogenie::SetNomPrefix(unsigned char nomprefix)
//====================================================
{
	m_nomprefix = nomprefix;
	return true;
}

//====================================================
//!function of getting dictionary index (code) postfix
unsigned char MPredlogenie::GetNomPostfix()
//====================================================
{
	unsigned char nompostfix;
	nompostfix = m_nompostfix;
	return nompostfix;
}

//====================================================
//!function of establishing dictionary index (code) postfix
bool MPredlogenie::SetNomPostfix(unsigned char nompostfix)
//====================================================
{
	m_nompostfix = nompostfix;
	return true;
}

//====================================================
//!Comparing  sentences
bool MPredlogenie::IsEqualTo(MPredlogenie* predlog)
//====================================================
{
	if((m_nomprefix==predlog->m_nomprefix)&&(m_nompolename==predlog->m_nompolename)&&
		(m_nompostfix==predlog->m_nompostfix)&&(m_nomprk==predlog->m_nomprk))
		return true;
	return false;
}

//=============================================================
//MNomValue - Class number-value - stores the numerical value of the number and characteristics ;
//contains key Characteristics of the object and Value characteristics of the object.
//=============================================================

//==================================
//!constructor by default
MNomValue::MNomValue()
//==================================	      
{
	m_nomer = 0;
	m_value = "";
}

//==================================
//!constructor with the specified standard
MNomValue::MNomValue(unsigned short nomer, MString value)
//==================================	      
{
	m_nomer = nomer;
	m_value = value;
}

//==================================
//! copy functions
void MNomValue::Copy(MNomValue* nomval)
//==================================
{
    //copy , if there is to copy.
	if(nomval != NULL){
		m_nomer = nomval->m_nomer;
		m_value = nomval->m_value;
	}
}

//====================================================
//!writing
bool MNomValue::WriteBin(FILE* h_nuxfile, int version)
//====================================================
{
    //Determine the size of the segment
	unsigned int Size=GetBytes(); 


	switch(version)
	{
	case 0:
        //Write the segment size
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write dictionary number
		fwrite(&m_nomer, sizeof(unsigned short), 1, h_nuxfile);
        //Write the value of the characteristic
		WriteString(h_nuxfile,version,m_value);
	}

	return true;
}

//====================================================
//!reading
bool MNomValue::ReadBin(FILE* h_nuxfile, int version)
//====================================================
{
    //Depending on the version
	switch (version){
		case 0:
        //sequentially read all terms
		unsigned int SIZE=0;
        //read the record length
		fread(&SIZE, 4, 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
        //read dictionary number
		fread(&m_nomer, sizeof(m_nomer), 1, h_nuxfile);
		if (reverseReadFlag) reverseValue((char*)&m_nomer, sizeof(m_nomer) );
        //read the value of the characteristic
		m_value=ReadString(h_nuxfile,version);
	}

	return true;
}

//====================================================
//!чтение
bool MNomValue::ReadBin(MFile* file, int version)
//====================================================
{
    //Depending on the version
    switch (version){
		case 0:
        //sequentially read all terms
        unsigned int SIZE= 0;
        //read the record length
        file->GetData(&SIZE, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&SIZE, sizeof(SIZE) );
        //read dictionary number
        file->GetData(&m_nomer, sizeof(m_nomer), 1);
		if (reverseReadFlag) reverseValue((char*)&m_nomer, sizeof(m_nomer) );
        //read the value of the characteristic
        m_value=ReadString(file,version);
	}

	return true;
}

//====================================================
//!cloning
MNomValue* MNomValue::Clone()
//====================================================
{
	MNomValue* pClone = new MNomValue();
	
	pClone->Copy(this);
	
	return pClone;
}


//====================================================
//!size in UTP-format
int MNomValue::GetBytes()
//====================================================
{
	int size = 4 + sizeof(unsigned short) + m_value.GetLength()+2;

	return size;
}

//====================================================
//!function of getting dictionary number
unsigned short MNomValue::GetNom()const
//====================================================
{
////	unsigned short nomer = m_nomer;  
	return m_nomer;
}

//====================================================
//!function of establishing dictionary number
bool MNomValue::SetNom(unsigned short nomer)
//====================================================
{
	m_nomer = nomer;
	return true;
}

//====================================================
//!function of getting the value of the characteristic
MString MNomValue::GetValue() const
//====================================================
{
	MString value = "";  
	value = m_value;
	return value;
}

//====================================================
//!function of establishing the value of the characteristic
bool MNomValue::SetValue(MString value)
//====================================================
{
	m_value = value;
	return true;
}

//====================================================
//!function of establishing dictionary number and  the value of the characteristic
bool MNomValue::SetNomValue(unsigned short nomer, MString value)
//====================================================
{
	m_nomer = nomer;
	m_value = value;
	return true;
}

//!====================================================
bool MNomValue::IsEqualTo(MNomValue* nomval)
//====================================================
{
	if((m_nomer==nomval->m_nomer)&&(m_value==nomval->m_value))
		return true;
	return false;
}
