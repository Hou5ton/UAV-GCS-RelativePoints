//=============================================================

#include <math.h>
#include "Basics.h"
#include "Yaz.h"
#include "Refglobal.h"
//=============================================================
// MZarubka
//=============================================================
//! constructor by default
MZarubka::MZarubka()
//==================================	
{
	m_nomrebra = 0;
	m_pt.x = 0;
	m_pt.y = 0;
}

//==================================
//!constructor for a given edge and the point
MZarubka::MZarubka(int nom, MPoint pt)
//==================================	      
{
	m_nomrebra = nom;
	m_pt.x = pt.x;
	m_pt.y = pt.y;	
}


//==================================
//!constructor with a given standard of notches
MZarubka::MZarubka(MZarubka* zarubka)
//==================================
{
	m_nomrebra = zarubka->m_nomrebra;
	m_pt.x = zarubka->m_pt.x;
	m_pt.y = zarubka->m_pt.y;	
}



//==================================
//! copy function
void MZarubka::Copy(MZarubka* zarubka)
//==================================
{
	m_nomrebra = zarubka->m_nomrebra;
	m_pt.x = zarubka->m_pt.x;
	m_pt.y = zarubka->m_pt.y;	
}

//===================================================================================
//!Setting notch
void MZarubka::SetZarubka(int nom, MPoint pt)
//===================================================================================
{
	m_nomrebra = nom;
	m_pt.x = pt.x;
	m_pt.y = pt.y;	
}


//=========================================================
//!Calculates the size that notch takes into NUX-file
/** Notch in Nux-format stored as:
//   1. Index of the rib of polyline    - integer 4 bytes
//   2. Coordinates of notch            - type MPoint - 16 bytes
*/
int MZarubka::GetSize()
//=========================================================
{
	int Size = 20; // 4+16

	return Size;
}

//===================================================================
//!Writing noches in UTP-file
bool MZarubka::WriteBin(FILE* h_nuxfile,   int version)
//====================================================================
{
    //Determine the size of the notch
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //Write the size of the notch
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write index of the rib of polyline
		fwrite(&m_nomrebra, sizeof(int), 1, h_nuxfile);
        //Write x-coordinate of notch
		fwrite(&m_pt.x, sizeof(double), 1, h_nuxfile);

        //Write y-coordinate of notch
		fwrite(&m_pt.y, sizeof(double), 1, h_nuxfile);
	}

	return true;

}

//===================================================================================
//!Reading notch from NUX-file
bool MZarubka::ReadBin(FILE* h_nuxfile,   int version)
//===================================================================================
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE=0;
            //read the record length
			fread(&SIZE, 4, 1, h_nuxfile);
            //read index of the rib of polyline
			fread(&m_nomrebra, sizeof(m_nomrebra), 1, h_nuxfile);
            //read x-coordinate of notch
			fread(&m_pt.x, sizeof(m_pt.x), 1, h_nuxfile);
            //read y-coordinate of notch
			fread(&m_pt.y, sizeof(m_pt.y), 1, h_nuxfile);
 		if (reverseReadFlag) {
		  reverseValue((char*)&SIZE, sizeof(SIZE) );
 		  reverseValue((char*)&m_nomrebra, sizeof(m_nomrebra) );
 		  reverseValue((char*)&m_pt.x, sizeof(m_pt.x) );
 		  reverseValue((char*)&m_pt.y, sizeof(m_pt.y) );
		}  
	}
	return true;
}

//===================================================================================
//!reading notch from NUX-file
bool MZarubka::ReadBin(MFile* file,   int version)
//===================================================================================
{
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE;
            //read the record length
            file->GetData(&SIZE, 4, 1);
            //read index of the rib of polyline
            file->GetData(&m_nomrebra, sizeof(m_nomrebra), 1);
            //read x-coordinate of notch
            file->GetData(&m_pt.x, sizeof(m_pt.x), 1);
            //read y-coordinate of notch
            file->GetData(&m_pt.y, sizeof(m_pt.y), 1);
 		if (reverseReadFlag) {
		  reverseValue((char*)&SIZE, sizeof(SIZE) );
 		  reverseValue((char*)&m_nomrebra, sizeof(m_nomrebra) );
 		  reverseValue((char*)&m_pt.x, sizeof(m_pt.x) );
 		  reverseValue((char*)&m_pt.y, sizeof(m_pt.y) );
		}  
	}
	return true;
}


//=============================================================
// MRazryv
//=============================================================

//==================================
//! constructor by default
MRazryv::MRazryv()
//==================================	
{
	m_indfirst = 0;
	m_indlast = 0;
	m_ptfirst.x = 0;
	m_ptfirst.y = 0;
	m_ptlast.x = 0;
	m_ptlast.y = 0;
}

//==================================
//!constructor for a given edge and the point
MRazryv::MRazryv(int indfirst, int indlast, MPoint ptfirst, MPoint ptlast)
//==================================	      
{
	m_indfirst = indfirst;
	m_indlast = indlast;
	m_ptfirst.x = ptfirst.x;
	m_ptfirst.y = ptfirst.y;
	m_ptlast.x = ptlast.x;
	m_ptlast.y = ptlast.y;
}


//==================================
//!constructor with the specified standard break
MRazryv::MRazryv(MRazryv* razryv)
//==================================
{
	m_indfirst = razryv->m_indfirst;
	m_indlast = razryv->m_indlast;
	m_ptfirst.x = razryv->m_ptfirst.x;
	m_ptfirst.y = razryv->m_ptfirst.y;
	m_ptlast.x = razryv->m_ptlast.x;
	m_ptlast.y = razryv->m_ptlast.y;
}


//==================================
//!copy function
void MRazryv::Copy(MRazryv* razryv)
//==================================
{
	m_indfirst = razryv->m_indfirst;
	m_indlast = razryv->m_indlast;
	m_ptfirst.x = razryv->m_ptfirst.x;
	m_ptfirst.y = razryv->m_ptfirst.y;
	m_ptlast.x = razryv->m_ptlast.x;
	m_ptlast.y = razryv->m_ptlast.y;
	
}

//===================================================================================
//!Setting break
void MRazryv::SetRazryv(int indfirst, int indlast, MPoint ptfirst, MPoint ptlast)
//===================================================================================
{
	m_indfirst = indfirst;
	m_indlast = indlast;
	m_ptfirst.x = ptfirst.x;
	m_ptfirst.y = ptfirst.y;
	m_ptlast.x = ptlast.x;
	m_ptlast.y = ptlast.y;
}


//=========================================================
//!Calculates the size of that break takes into NUX-file
/** Break in Nux-format stores as:
//   1. Index of the entry element polyline ,
//      corresponding to the beginning of the break        - integer 4 bytes
//   2. Index of the final element polyline,
//      corresponding to the end of the break              - integer 4 bytes
//   3. Coordinates of the segment beginning
//      that defines the break                             - type MPoint - 16 bytes
//   4. Coordinates of the segment end
//      that defines the break                             - type MPoint - 16 bytes
*/
int MRazryv::GetSize()
//=========================================================
{
	int Size = 40; // 4+16+4+16

	return Size;
}
//===================================================================
//!Writing notch in UTP-file
bool MRazryv::WriteBin(FILE* h_nuxfile,   int version)
//====================================================================
{
    //Determine the size of the break
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //Write the size of the break
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write index of initial rib of the polyline
		fwrite(&m_indfirst, sizeof(int), 1, h_nuxfile);
        //Write index of final rib of the polyline
		fwrite(&m_indlast, sizeof(int), 1, h_nuxfile);
        //Write x-coordinate of the segment beginning that defines the break
		fwrite(&m_ptfirst.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of the segment beginning that defines the break
		fwrite(&m_ptfirst.y, sizeof(double), 1, h_nuxfile);
        //Write x-coordinate of the segment end that defines the break
		fwrite(&m_ptlast.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of the segment end that defines the break
		fwrite(&m_ptlast.y, sizeof(double), 1, h_nuxfile);
	}

	return true;

}

//===================================================================================
//!Reading notch from NUX-file
bool MRazryv::ReadBin(FILE* h_nuxfile,   int version)
//===================================================================================
{
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE=0;
            //read record length
			fread(&SIZE, 4, 1, h_nuxfile);
            //read index of initial rib of the polyline
			fread(&m_indfirst, sizeof(m_indfirst), 1, h_nuxfile);
            //read index of final rib of the polyline
			fread(&m_indlast,  sizeof(m_indlast),  1, h_nuxfile);
            //read x-coordinate of the segment beginning that defines the break
			fread(&m_ptfirst.x,sizeof(m_ptfirst.x),1, h_nuxfile);
            //read y-coordinate of the segment beginning that defines the break
			fread(&m_ptfirst.y,sizeof(m_ptfirst.y),1, h_nuxfile);
            //read x-coordinate of the segment end that defines the break
			fread(&m_ptlast.x, sizeof(m_ptlast.x), 1, h_nuxfile);
            //read y-coordinate of the segment end that defines the break
			fread(&m_ptlast.y, sizeof(m_ptlast.y), 1, h_nuxfile);

 		if (reverseReadFlag) {
		  reverseValue((char*)&SIZE, sizeof(SIZE) );
 		  reverseValue((char*)&m_indfirst,  sizeof(m_indfirst) );
 		  reverseValue((char*)&m_indlast,   sizeof(m_indlast) );
 		  reverseValue((char*)&m_ptfirst.x, sizeof(m_ptfirst.x) );
 		  reverseValue((char*)&m_ptfirst.y, sizeof(m_ptfirst.y) );
 		  reverseValue((char*)&m_ptlast.x,  sizeof(m_ptlast.x) );
 		  reverseValue((char*)&m_ptlast.y,  sizeof(m_ptlast.y) );
		}  
	}
	return true;
}


//===================================================================================
//!Reading notch from NUX-file
bool MRazryv::ReadBin(MFile* file,   int version)
//===================================================================================
{
    //Depending on the version
    switch (version){
		case 0:
			unsigned int SIZE= 0;
            //read the record length
            file->GetData(&SIZE, 4, 1);
            //read index of initial rib of the polyline
            file->GetData(&m_indfirst, sizeof(m_indfirst), 1);
            //read index of final rib of the polyline
            file->GetData(&m_indlast,  sizeof(m_indlast),  1);
            //read x-coordinate of the segment beginning that defines the break
            file->GetData(&m_ptfirst.x,sizeof(m_ptfirst.x),1);
            //read y-coordinate of the segment beginning that defines the break
            file->GetData(&m_ptfirst.y,sizeof(m_ptfirst.y),1);
            //read x-coordinate of the segment end that defines the break
            file->GetData(&m_ptlast.x, sizeof(m_ptlast.x), 1);
            //read y-coordinate of the segment end that defines the break
            file->GetData(&m_ptlast.y, sizeof(m_ptlast.y), 1);

 		if (reverseReadFlag) {
		  reverseValue((char*)&SIZE, sizeof(SIZE) );
 		  reverseValue((char*)&m_indfirst,  sizeof(m_indfirst) );
 		  reverseValue((char*)&m_indlast,   sizeof(m_indlast) );
 		  reverseValue((char*)&m_ptfirst.x, sizeof(m_ptfirst.x) );
 		  reverseValue((char*)&m_ptfirst.y, sizeof(m_ptfirst.y) );
 		  reverseValue((char*)&m_ptlast.x,  sizeof(m_ptlast.x) );
 		  reverseValue((char*)&m_ptlast.y,  sizeof(m_ptlast.y) );
		}  
	}
	return true;
}

//=============================================================
// MSegment
//=============================================================

//==================================
//! constructor by default
MSegment::MSegment()
//==================================	
{
	m_sgpt1.x = m_sgpt2.x = 0;
	m_sgpt1.y = m_sgpt2.y = 0;
	m_sgheight1 = m_sgheight2 = 0;
}

//==================================
//!constructor on set ends of the segment and their heights
MSegment::MSegment(MPoint pt1, MPoint pt2, double height1, double height2)
//==================================	      
{
	m_sgpt1 = pt1;
	m_sgpt2 = pt2;
	m_sgheight1 = height1;
	m_sgheight2 = height2;
}


//==================================
//!constructor with set standart of the segment
MSegment::MSegment(MSegment* segment)
//==================================
{
	m_sgpt1 = segment->m_sgpt1;
	m_sgpt2 = segment->m_sgpt2;
	m_sgheight1 = segment->m_sgheight1;
	m_sgheight2 = segment->m_sgheight2;	
}


//==================================
//!copy function
void MSegment::Copy(MSegment* segment)
//==================================
{
	m_sgpt1 = segment->m_sgpt1;
	m_sgpt2 = segment->m_sgpt2;
	m_sgheight1 = segment->m_sgheight1;
	m_sgheight2 = segment->m_sgheight2;	
}

//===================================================================================
//!Setting of a segment
void MSegment::SetSegment(MPoint pt1, MPoint pt2, double height1, double height2)
//===================================================================================
{
	m_sgpt1 = pt1;
	m_sgpt2 = pt2;
	m_sgheight1 = height1;
	m_sgheight2 = height2;
}



//=========================================================
//!Calculates the size which the segment will occupy in the NUX-file
/** Segment stores in Nux-format as:
//   1. Coordinates of the beginning       - type MPoint - 16 bytes
//	 2. Coordinates of the end             - type MPoint - 16 bytes
//   3. Height of the beginning            - type double - 8 bytes
//	 4. Height of the end                  - type double - 8 bytes
*/
int MSegment::GetSize()
//=========================================================
{
	int Size = 48; // 16+16+8+8

	return Size;
}


//===================================================================
//!Writing segment in UTP-file
bool MSegment::WriteBin(FILE* h_nuxfile,   int version)
//====================================================================
{
    //Determine the segment size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //Write the segment size
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write x-coordinate of the segment beginning
		fwrite(&m_sgpt1.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of the segment beginning
		fwrite(&m_sgpt1.y, sizeof(double), 1, h_nuxfile);
        //Write x-coordinate of the segment end
		fwrite(&m_sgpt2.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of the segment end
		fwrite(&m_sgpt2.y, sizeof(double), 1, h_nuxfile);
        //Write height of the segment beginning
		fwrite(&m_sgheight1, sizeof(double), 1, h_nuxfile);
        //Write height of the segment end
		fwrite(&m_sgheight2, sizeof(double), 1, h_nuxfile);
	}

	return true;

}

//===================================================================================
//!Reading segment from NUX-file
bool MSegment::ReadBin(FILE* h_nuxfile,   int version)
//===================================================================================
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE=0;
            //read record length
			fread(&SIZE, 4, 1, h_nuxfile);
            //Write x-coordinate of the segment beginning
			fread(&m_sgpt1.x, sizeof(m_sgpt1.x), 1, h_nuxfile);
            //Write y-coordinate of the segment beginning
			fread(&m_sgpt1.y, sizeof(m_sgpt1.y), 1, h_nuxfile);
            //Write x-coordinate of the segment end
			fread(&m_sgpt2.x, sizeof(m_sgpt2.x), 1, h_nuxfile);
            //Write y-coordinate of the segment end
			fread(&m_sgpt2.y, sizeof(m_sgpt2.y), 1, h_nuxfile);
            //Write height of the segment beginning
			fread(&m_sgheight1, sizeof(m_sgheight1), 1, h_nuxfile);
            //Write height of the segment end
			fread(&m_sgheight2, sizeof(m_sgheight2), 1, h_nuxfile);

 		if (reverseReadFlag)  {
		  reverseValue((char*)&SIZE, sizeof(SIZE) );
 		  reverseValue((char*)&m_sgpt1.x, sizeof(m_sgpt1.x) );
 		  reverseValue((char*)&m_sgpt1.y, sizeof(m_sgpt1.y) );
 		  reverseValue((char*)&m_sgpt2.x, sizeof(m_sgpt2.x) );
 		  reverseValue((char*)&m_sgpt2.y, sizeof(m_sgpt2.y) );
 		  reverseValue((char*)&m_sgheight1, sizeof(m_sgheight1) );
 		  reverseValue((char*)&m_sgheight2, sizeof(m_sgheight2) );
		}  
	}
	return true;
}

//===================================================================================
//!Reading segment from NUX-file
bool MSegment::ReadBin(MFile* file,   int version)
//===================================================================================
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE= 0;
            //read record length
			file->GetData(&SIZE, 4, 1);
            //Write x-coordinate of the segment beginning
			file->GetData(&m_sgpt1.x, sizeof(m_sgpt1.x), 1);
            //Write y-coordinate of the segment beginning
			file->GetData(&m_sgpt1.y, sizeof(m_sgpt1.y), 1);
            //Write x-coordinate of the segment end
			file->GetData(&m_sgpt2.x, sizeof(m_sgpt2.x), 1);
            //Write y-coordinate of the segment end
			file->GetData(&m_sgpt2.y, sizeof(m_sgpt2.y), 1);
            //Write height of the segment beginning
			file->GetData(&m_sgheight1, sizeof(m_sgheight1), 1);
            //Write height of the segment end
			file->GetData(&m_sgheight2, sizeof(m_sgheight2), 1);

 		if (reverseReadFlag)  {
		  reverseValue((char*)&SIZE, sizeof(SIZE) );
 		  reverseValue((char*)&m_sgpt1.x, sizeof(m_sgpt1.x) );
 		  reverseValue((char*)&m_sgpt1.y, sizeof(m_sgpt1.y) );
 		  reverseValue((char*)&m_sgpt2.x, sizeof(m_sgpt2.x) );
 		  reverseValue((char*)&m_sgpt2.y, sizeof(m_sgpt2.y) );
 		  reverseValue((char*)&m_sgheight1, sizeof(m_sgheight1) );
 		  reverseValue((char*)&m_sgheight2, sizeof(m_sgheight2) );
		}  
	}
	return true;
}

//=============================================================
// MTriangle
//=============================================================

//==================================
//! Constructor by default
MTriangle::MTriangle()
//==================================	
{
	m_tript1.x = m_tript2.x = m_tript3.x = 0;
	m_tript1.y = m_tript2.y = m_tript3.y = 0;
	m_triheight1 = m_triheight2 = m_triheight3 = 0;
}

//==================================
//!Constructor on the set tops and their heights
MTriangle::MTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double height1, double height2, double height3)
//==================================	      
{
	m_tript1 = pt1;
	m_tript2 = pt2;
	m_tript3 = pt3;
	m_triheight1 = height1;
	m_triheight2 = height2;
	m_triheight3 = height3;
}


//==================================
//!Constructor with the set triangle standard
MTriangle::MTriangle(MTriangle* triangle)
//==================================
{
	m_tript1 = triangle->m_tript1;
	m_tript2 = triangle->m_tript2;
	m_tript3 = triangle->m_tript3;
	m_triheight1 = triangle->m_triheight1;
	m_triheight2 = triangle->m_triheight2;
	m_triheight3 = triangle->m_triheight3;
}


//==================================
//! copy function
void MTriangle::Copy(MTriangle* triangle)
//==================================
{
	m_tript1 = triangle->m_tript1;
	m_tript2 = triangle->m_tript2;
	m_tript3 = triangle->m_tript3;
	m_triheight1 = triangle->m_triheight1;
	m_triheight2 = triangle->m_triheight2;
	m_triheight3 = triangle->m_triheight3;
}

//===================================================================================
//!Setting of a triangle
void MTriangle::SetTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double height1, double height2, double height3)
//===================================================================================
{
	m_tript1 = pt1;
	m_tript2 = pt2;
	m_tript3 = pt3;
	m_triheight1 = height1;
	m_triheight2 = height2;
	m_triheight3 = height3;
}


//=========================================================
//!Calculates the size which the triangle will occupy in the NUX-file
/*
// Triangle stores in Nux-format as:
//   1. Coordinates of zero top             - type MPoint - 16 bytes
//	 2. Coordinates of the first top        - type MPoint - 16 bytes
//	 3. Coordinates of the second top       - type MPoint - 16 bytes
//   4. Height of zero top                  - type double - 8 bytes
//	 5. Height of the first top             - type double - 8 bytes
//	 6. Height of the second  top           - type double - 8 bytes
*/
int MTriangle::GetSize()
//=========================================================
{
	int Size = 72; // 16+16+16+8+8+8

	return Size;
}

//===================================================================
//!Writing of a triangle in UTP-file
bool MTriangle::WriteBin(FILE* h_nuxfile,   int version)
//====================================================================
{
    //Determine the triangle size
	unsigned int Size=GetSize(); 
	switch(version)
	{
	case 0:
        //Write the triangle size
		fwrite(&Size, 4, 1, h_nuxfile);
        //Write x-coordinate of zero top of the triangle
		fwrite(&m_tript1.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of zero top of the triangle
		fwrite(&m_tript1.y, sizeof(double), 1, h_nuxfile);
        //Write x-coordinate of the first top of the triangle
		fwrite(&m_tript2.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of the first top of the triangle
		fwrite(&m_tript2.y, sizeof(double), 1, h_nuxfile);
        //Write x-coordinate of the second top of the triangle
        fwrite(&m_tript3.x, sizeof(double), 1, h_nuxfile);
        //Write y-coordinate of the second top of the triangle
		fwrite(&m_tript3.y, sizeof(double), 1, h_nuxfile);
        //Write height of zero top of the triangle
		fwrite(&m_triheight1, sizeof(double), 1, h_nuxfile);
        //Write height of the first top of the triangle
		fwrite(&m_triheight2, sizeof(double), 1, h_nuxfile);
        //Write height of the second top of the triangle
		fwrite(&m_triheight3, sizeof(double), 1, h_nuxfile);
	}

	return true;

}

//===================================================================================
//!Reading triangle from NUX-file
bool MTriangle::ReadBin(FILE* h_nuxfile,   int version)
//===================================================================================
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE=0;
            //read record length
			fread(&SIZE, 4, 1, h_nuxfile);
            //Read x-coordinate of zero top of the triangle
			fread(&m_tript1.x, sizeof(m_tript1.x), 1, h_nuxfile);
            //Read y-coordinate of zero top of the triangle
			fread(&m_tript1.y, sizeof(m_tript1.y), 1, h_nuxfile);
            //Read x-coordinate of the first top of the triangle
			fread(&m_tript2.x, sizeof(m_tript2.x), 1, h_nuxfile);
            //Read y-coordinate of the first top of the triangle
			fread(&m_tript2.y, sizeof(m_tript2.y), 1, h_nuxfile);
            //Read x-coordinate of the second top of the triangle
			fread(&m_tript3.x, sizeof(m_tript3.x), 1, h_nuxfile);
            //Read y-coordinate of the second top of the triangle
			fread(&m_tript3.y, sizeof(m_tript3.y), 1, h_nuxfile);
            //Read height of zero top of the triangle
			fread(&m_triheight1,sizeof(m_triheight1) , 1, h_nuxfile);
            //Read height of the first top of the triangle
			fread(&m_triheight2, sizeof(m_triheight2), 1, h_nuxfile);
            //Read height of the second top of the triangle
			fread(&m_triheight3, sizeof(m_triheight3), 1, h_nuxfile);

 		if (reverseReadFlag) {
		  reverseValue((char*)&SIZE,         sizeof(SIZE) );
 		  reverseValue((char*)&m_tript1.x,   sizeof(m_tript1.x) );
 		  reverseValue((char*)&m_tript1.y,   sizeof(m_tript1.y) );
 		  reverseValue((char*)&m_tript2.x,   sizeof(m_tript2.x) );
 		  reverseValue((char*)&m_tript2.y,   sizeof(m_tript2.y) );
 		  reverseValue((char*)&m_tript3.x,   sizeof(m_tript3.x) );
 		  reverseValue((char*)&m_tript3.y,   sizeof(m_tript3.y) );
 		  reverseValue((char*)&m_triheight1, sizeof(m_triheight1) );
 		  reverseValue((char*)&m_triheight2, sizeof(m_triheight2) );
 		  reverseValue((char*)&m_triheight3, sizeof(m_triheight3) );
		}  
	}	
	return true;
}

//===================================================================================
//!Reading triangle from NUX-file
bool MTriangle::ReadBin(MFile* file,   int version)
//===================================================================================
{
    //Depending on the version
	switch (version){
		case 0:
			unsigned int SIZE= 0;
            //read record length
			file->GetData(&SIZE, 4, 1);
            //Read x-coordinate of zero top of the triangle
			file->GetData(&m_tript1.x, sizeof(m_tript1.x), 1);
            //Read y-coordinate of zero top of the triangle
			file->GetData(&m_tript1.y, sizeof(m_tript1.y), 1);
            //Read x-coordinate of the first top of the triangle
			file->GetData(&m_tript2.x, sizeof(m_tript2.x), 1);
            //Read y-coordinate of the first top of the triangle
			file->GetData(&m_tript2.y, sizeof(m_tript2.y), 1);
            //Read x-coordinate of the second top of the triangle
			file->GetData(&m_tript3.x, sizeof(m_tript3.x), 1);
            //Read y-coordinate of the second top of the triangle
			file->GetData(&m_tript3.y, sizeof(m_tript3.y), 1);
            //Read height of zero top of the triangle
			file->GetData(&m_triheight1, sizeof(m_triheight1) , 1);
            //Read height of the first top of the triangle
			file->GetData(&m_triheight2, sizeof(m_triheight2), 1);
            //Read height of the second top of the triangle
			file->GetData(&m_triheight3, sizeof(m_triheight3), 1);
 		if (reverseReadFlag) {
		  reverseValue((char*)&SIZE,         sizeof(SIZE) );
 		  reverseValue((char*)&m_tript1.x,   sizeof(m_tript1.x) );
 		  reverseValue((char*)&m_tript1.y,   sizeof(m_tript1.y) );
 		  reverseValue((char*)&m_tript2.x,   sizeof(m_tript2.x) );
 		  reverseValue((char*)&m_tript2.y,   sizeof(m_tript2.y) );
 		  reverseValue((char*)&m_tript3.x,   sizeof(m_tript3.x) );
 		  reverseValue((char*)&m_tript3.y,   sizeof(m_tript3.y) );
 		  reverseValue((char*)&m_triheight1, sizeof(m_triheight1) );
 		  reverseValue((char*)&m_triheight2, sizeof(m_triheight2) );
 		  reverseValue((char*)&m_triheight3, sizeof(m_triheight3) );
		}  
	}
	return true;
}

//*********************************************************
//******                   MRzArray                  ******
//*********************************************************

//=========================================================
//!Orders the array of breaks on the first index in ascending order
bool MRzArray::OrderByFirstIndex()
//=========================================================
{
	int n = GetSize();
    //if the array is empty or contains one element, there is nothing to order
	if(n<=1) return true;
	MRzArray* rzar = new MRzArray();
	rzar->AppendTail(this);
	MRzArray* rzarnew = new MRzArray();
	int ind;

	while(rzar->GetSize()!=0){
		MRazryv* rzmin = new MRazryv(rzar->GetAt(0));
		ind = 0;
		for(int i=1; i<n; i++){
			MRazryv* rz = rzar->GetAt(i);
			if(rzmin->m_indfirst>rz->m_indfirst){
				rzmin->Copy(rz) ;
				ind = i;
			}
		}
		rzarnew->AddTail(rzmin);
		rzar->Delete(ind);
		delete rzmin;
	}

    //clear the array and we insert the ordered
	ClearAll();
	AppendTail(rzarnew);

	delete rzar;
	delete rzarnew;

	return true;
}



//*********************************************************
//******                   MSgArray                  ******
//*********************************************************
//array of segments

//============================================
//!Search of a segment, which coordinate of the ends coincide with entrance coordinates
int MSgArray::FindByCoords(int iIndex, MPoint pt1, MPoint pt2)

//============================================
{
	if(!((iIndex>=0)&&(iIndex<Size))) return -1;
	for(int i=iIndex;i<Size;i++)
	{
		MSegment* pSeg=GetAt(i);
		if(((pSeg->m_sgpt1==pt1)&&(pSeg->m_sgpt2==pt2))||((pSeg->m_sgpt2==pt1)&&(pSeg->m_sgpt1==pt2)))
			return i;
	}
	return -1;
};


//*********************************************************
//******                   MTrArray                  ******
//*********************************************************
//Array of triangles



//============================================
//!Search of a triangle, which coordinate of the ends coincide with entrance coordinates
int MTrArray::FindByCoords(int iIndex, MPoint pt1, MPoint pt2, MPoint pt3)
//============================================
{
	if(!((iIndex>=0)&&(iIndex<Size))) return -1;
	for(int i=iIndex;i<Size;i++)
	{
		MTriangle* pTri=GetAt(i);
		if(((pTri->m_tript1==pt1)&&(pTri->m_tript2==pt2)&&(pTri->m_tript3==pt3))||
			((pTri->m_tript1==pt1)&&(pTri->m_tript2==pt3)&&(pTri->m_tript3==pt2))||
			((pTri->m_tript1==pt3)&&(pTri->m_tript2==pt2)&&(pTri->m_tript3==pt1))||
			((pTri->m_tript1==pt3)&&(pTri->m_tript2==pt1)&&(pTri->m_tript3==pt2))||
			((pTri->m_tript1==pt2)&&(pTri->m_tript2==pt1)&&(pTri->m_tript3==pt3))||
			((pTri->m_tript1==pt2)&&(pTri->m_tript2==pt3)&&(pTri->m_tript3==pt1)))
			return i;
	}
	return -1;
};

