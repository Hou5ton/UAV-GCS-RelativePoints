//===============================================================================
// Global functions (CPP)
//realization of all support static functions
//===============================================================================
#include "Globals.h"
#include "BaseCl.h"
#include "Topobject.h"
#include "PlotDevice.h"
#include "Refglobal.h"
#include "Clipping.h"
#include "Geodesic.h"
#include <QtCore/qmath.h>

typedef QVector<MPoint> PointContainer;
typedef QVector<PointContainer::ConstIterator> KeyContainer;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// READING-WRITING BASIC TYPES
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//! writing MStringArray
bool WriteStrArray(FILE* h_nuxfile,   int version, MStArray* strar)
{
	int			i;

	switch( version ) {
        //version 0
		case 0:
			i = strar->GetSize();
            //writing of byte with a length of a line and the line
			fwrite(&i, 4, 1, h_nuxfile);
			for(int j=0; j<i; j++)
				WriteString(h_nuxfile,  version,*strar->GetAt(j));
	}
	return true;
}

//!reading  MStArray
bool ReadStrArray(FILE* h_nuxfile,   int version, MStArray*	strar)
//==================================		
{
	int			i = 0;
    MString		str;

	switch( version ) {
        //version 0
		case 0:
            //reading byte with a length of a line and the line
			fread(&i, 4, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)&i, sizeof(int) );
			strar->SetSize(i);
			for(int j=0;j<i;j++) {
				str = ReadString(h_nuxfile,  version);
				strar->SetAt(j,&str);
			}
	}
	return true;
}

 
//!reading  MStArray
bool ReadStrArray(MFile* file,   int version, MStArray*	strar)
//==================================		
{
	int			i = 0;
    MString		str;

	switch( version ) {
        //version 0
		case 0:
        //reading byte with a length of a line and the line
            file->GetData(&i, 4, 1);
		if (reverseReadFlag) reverseValue((char*)&i, sizeof(i) );
			strar->SetSize(i);
			for(int j=0;j<i;j++) {
				str = ReadString(file,  version);
				strar->SetAt(j,&str);
			}
	}
	return true;
}

//!writing of a point
bool WritePoint(FILE* h_nuxfile,   int version, MPoint* pnt)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
            //writing of coordinates of X and Y
			fwrite(pnt, 16, 1, h_nuxfile);
	}
	return true;
}

//==================================		
//!reading of a point
bool ReadPoint(FILE* h_nuxfile,   int version, MPoint* pnt)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
            //reading of coordinates of X and Y
			fread(pnt, 16, 1, h_nuxfile);
 		if (reverseReadFlag)  { 
		  reverseValue((char*)&pnt->x, sizeof(pnt->x) );
 		  reverseValue((char*)&pnt->y, sizeof(pnt->y) );
		}  

	}
	return true;
}

//==================================		
//!reading of a point
bool ReadPoint(MFile* file,   int version, MPoint* pnt)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
        //reading of coordinates of X and Y
            file->GetData(pnt, 16, 1);

 		if (reverseReadFlag)  { 
		  reverseValue((char*)&pnt->x, sizeof(pnt->x) );
 		  reverseValue((char*)&pnt->y, sizeof(pnt->y) );
		}  
	}
	return true;
}

//==================================		
//!writing of a rectangle
bool WriteRect(FILE* h_nuxfile,   int version, MRect* rect)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
            //writing of coordinates of angles
			fwrite(rect, 32, 1, h_nuxfile);
	}
	return true;
}

//==================================		
//!reading of a rectangle
bool ReadRect(FILE* h_nuxfile,   int version, MRect* rect)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
        //reading of coordinates of angles
            fread(rect, 32, 1, h_nuxfile);
 		if (reverseReadFlag) {
		   reverseValue((char*)&rect->left ,  sizeof(rect->left) );
 		   reverseValue((char*)&rect->right , sizeof(rect->right) );
 		   reverseValue((char*)&rect->bottom, sizeof(rect->bottom) );
 		   reverseValue((char*)&rect->top ,   sizeof(rect->top) );
		}   
	}
	return true;
}

//==================================		
//!reading of a rectangle
bool ReadRect(MFile* file,   int version, MRect* rect)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
        //reading of coordinates of angles
            file->GetData(rect, 32, 1);
 		if (reverseReadFlag) {
		   reverseValue((char*)&rect->left ,  sizeof(rect->left) );
 		   reverseValue((char*)&rect->right , sizeof(rect->right) );
 		   reverseValue((char*)&rect->bottom, sizeof(rect->bottom) );
 		   reverseValue((char*)&rect->top ,   sizeof(rect->top) );
		}   
	}
	return true;
}

//==================================		
//!writing of the array of points
bool WriteArrayPoints(FILE* h_nuxfile,   int version, int *pkol, MPoint* pnt)
//==================================		
{
	unsigned int		pnt_size;

	switch( version ) {
        //version 0
		case 0:
            //writing numbers of points
			fwrite(pkol, 2, 1, h_nuxfile);
            //writing array of points
            if (*pkol != 0) {

				pnt_size = sizeof(MPoint) * (*pkol);
				fwrite(pnt, pnt_size, 1, h_nuxfile);
			}
	}
	return true;
}

//==================================		
//!reading of the array of points
bool ReadArrayPoints(FILE* h_nuxfile,   int version, int *pkol, MPoint** pnt)
//==================================		
{
	unsigned int		pnt_size;

	switch( version ) {
        //version 0
		case 0:
            //reading numbers of points
			*pkol= 0;  // vipa 
			fread(pkol, 2, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)pkol, sizeof(int) );
            //reading array of points
            if (*pkol != 0) {
				pnt_size = sizeof(MPoint) * (*pkol);
				MPoint* points = (MPoint*) malloc(pnt_size);
				fread(points, pnt_size, 1, h_nuxfile);
				*pnt = points;
    			if (reverseReadFlag) 
			  for(int j=0; j< (*pkol); j++ ) { 
    			    reverseValue((char*)&points->x, sizeof(points->x) );
    			    reverseValue((char*)&points->y, sizeof(points->y) );
			    points++;
			  }	
 			}
	}
	return true;
}

//==================================		
//!reading of the array of points
bool ReadArrayPoints(MFile* file,   int version, int *pkol, MPoint** pnt)
//==================================		
{
	unsigned int		pnt_size;

	switch( version ) {
        //version 0
		case 0:
        //reading numbers of points
            *pkol= 0;  // vipa
			file->GetData(pkol, 2, 1);
 		if (reverseReadFlag) reverseValue((char*)pkol, sizeof(int) );
            //reading array of points
            if (*pkol != 0) {
				pnt_size = sizeof(MPoint) * (*pkol);
				MPoint* points = (MPoint*) malloc(pnt_size);
				file->GetData(points, pnt_size, 1);
				*pnt = points;
    			if (reverseReadFlag) 
			  for(int j=0; j< (*pkol); j++ ) { 
    			    reverseValue((char*)&points->x, sizeof(points->x) );
    			    reverseValue((char*)&points->y, sizeof(points->y) );
			    points++;
			  }	
								
			}
	}
	return true;
}

//==================================		
//!writing of the array of numbers
bool WriteArrayNambers(FILE* h_nuxfile,   int version, int *nkol, int* nbr)
//==================================		
{
	unsigned int		n_size;

	switch( version ) {
        //version 0
		case 0:
            //writing numbers
			fwrite(nkol, 2, 1, h_nuxfile);
            //writing array of numbers
			if (*nkol > 1) {

				n_size = sizeof(*nbr) * (*nkol);
				fwrite(nbr, n_size, 1, h_nuxfile);
			}
	}
	return true;
}

//==================================		
//!reading of the array of numbers
bool ReadArrayNambers(FILE* h_nuxfile,   int version, int *nkol, int** nbr)
//==================================		
{
	unsigned int		n_size;

	switch( version ) {
        //version 0
		case 0:
        //reading numbers
            *nkol= 0;  // vipa
			fread(nkol, 2, 1, h_nuxfile);
 		if (reverseReadFlag) reverseValue((char*)nkol, sizeof(int) );
        //reading array of numbers
            if (*nkol > 1) {

				n_size = sizeof(*nbr) * (*nkol);
				int* new_nbr = (int*) malloc(n_size);
				fread(new_nbr, n_size, 1, h_nuxfile);
				*nbr = new_nbr;
		if (reverseReadFlag) 		
		  for(int j=0; j< *nkol; j++ ) {  
    		     reverseValue((char*)new_nbr, sizeof(int) );
		     new_nbr++;
		 }		
			}
			else *nbr[0] = 0;
	}
	return true;
}

//==================================		
//!reading of the array of numbers
bool ReadArrayNambers(MFile* file,   int version, int *nkol, int** nbr)
//==================================		
{
	unsigned int		n_size;

	switch( version ) {
        //version 0
		case 0:
        //reading numbers
            *nkol= 0;  // vipa
			file->GetData(nkol, 2, 1);
 		if (reverseReadFlag) reverseValue((char*)nkol, sizeof(int) );
        //reading array of numbers
            if (*nkol > 1) {

				n_size = sizeof(*nbr) * (*nkol);
				int* new_nbr = (int*) malloc(n_size);
				file->GetData(new_nbr, n_size, 1);
				*nbr = new_nbr;
		if (reverseReadFlag) 		
		  for(int j=0; j< *nkol; j++ ) {  
    		     reverseValue((char*)new_nbr, sizeof(int) );
		     new_nbr++;
		 }		
				
			}
			else *nbr[0] = 0;
	}
	return true;
}

//==================================		
//!writing Colour type
bool WriteColor(FILE* h_nuxfile,   int version, MColor* color)
//==================================		
{
	switch( version ) {
        //version 0
		case 0:
            //writing red component
			unsigned char k = 0;
			k = color->GetR();
			fwrite(&k, 1, 1, h_nuxfile);
            //writing green component
            k = color->GetG();
			fwrite(&k, 1, 1, h_nuxfile);
            //writing blue component
            k = color->GetB();
			fwrite(&k, 1, 1, h_nuxfile);
            //writing alpha
            k = color->GetA();
			fwrite(&k, 1, 1, h_nuxfile);
	}
	return true;
}

//==================================		
//!reading Colour type
bool ReadColor(FILE* h_nuxfile,   int version, MColor* color)
//==================================		
{
	unsigned char		r,g,b,a;
	a = 0;

	switch( version ) {
        //version 0
		case 0:
            //reading colour components

			fread(&r, 1, 1, h_nuxfile);
			fread(&g, 1, 1, h_nuxfile);
			fread(&b, 1, 1, h_nuxfile);
			fread(&a, 1, 1, h_nuxfile);
			//color->SetA(a);
			color->SetColor(r,g,b);
	}
	return true;
}

//==================================		
//!reading Colour type
bool ReadColor(MFile* file,   int version, MColor* color)
//==================================		
{
	unsigned char		r,g,b,a;
	a = 0;

	switch( version ) {
        //version 0
		case 0:
            //reading colour components
            file->GetData(&r, 1, 1);
			file->GetData(&g, 1, 1);
			file->GetData(&b, 1, 1);
			file->GetData(&a, 1, 1);
			//color->SetA(a);
			color->SetColor(r,g,b);
	}
	return true;
}

//==================================		
//!writing expanded pen type
bool WritePen(FILE* h_nuxfile,   int version, M_LOG_PEN* lpen)
//==================================		
{
	MColor*	color;

	switch( version ) {
        //version 0
		case 0:
            //writing style and type of a pen
			fwrite(&lpen->lopnStyle, 4, 1, h_nuxfile);
			fwrite(&lpen->lopnWidth.x, sizeof(double), 1, h_nuxfile);
            //writing colour of the pen
			color = &lpen->lopnColor;
			WriteColor(h_nuxfile,  version, color);
	}
	return true;
}

//==================================		
//!reading expanded pen type
bool ReadPen(FILE* h_nuxfile,   int version, M_LOG_PEN* lpen)
//==================================		
{
	MColor*	color;

	switch( version ) {
        //version 0
		case 0:
            //reading style, feather type
            double Width;
			fread(&lpen->lopnStyle, 4, 1, h_nuxfile);
    			if (reverseReadFlag) reverseValue((char*)&lpen->lopnStyle, sizeof(lpen->lopnStyle) );
			Width=0;
			fread(&Width, sizeof(double), 1, h_nuxfile);
    			if (reverseReadFlag) reverseValue((char*)&Width, sizeof(Width) );
			lpen->lopnWidth.x=Width;
			lpen->lopnWidth.y = 1;
            //reading colour of the pen
			color = &lpen->lopnColor;
			ReadColor(h_nuxfile,  version, color);
			lpen->lopnColor = *color;
	}
	return true;
}

//==================================		
//!reading expanded pen type
bool ReadPen(MFile* file,   int version, M_LOG_PEN* lpen)
//==================================		
{
	MColor*	color;

	switch( version ) {
        //version 0
		case 0:
            //reading style, feather type
			double Width;
			file->GetData(&lpen->lopnStyle, 4, 1);
    			if (reverseReadFlag) reverseValue((char*)&lpen->lopnStyle, sizeof(lpen->lopnStyle) );
			Width=0;
			file->GetData(&Width, sizeof(double), 1);
    			if (reverseReadFlag) reverseValue((char*)&Width, sizeof(Width) );
			lpen->lopnWidth.x=Width;
			lpen->lopnWidth.y = 1;
            //reading colour of the pen
            color = &lpen->lopnColor;
			ReadColor(file,  version, color);
			lpen->lopnColor = *color;
	}
	return true;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// End. (Static functions of Preservation. Records of the document)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//!====================================
void ConvertGaussPoint(MPoint* point)
//====================================
{
    if(!point) return;
    double t = point->x;
    point->x = point->y;
    point->y = t;
}

//! =========================
MPoint ConvertGaussPoint(MPoint& point)
//====================================
{
    MPoint t_point(point.y, point.x);
    return t_point;
}

//******************************************
// ALGEBRAIC SUPPORT FUNCTIONS
//******************************************

//==================================
//! Determination of coordinates of a point of intersection of two straight lines,
//! passing through points: pt1, pt2 - the first straight line, pt3, pt4 - the second straight line,
//! pt - the pointer on a point of intersection of two straight lines
bool CROSSLINES(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt)
//==================================
{
	double z;
    // the formula for finding of coordinates of a point of intersection is removed according to the reference book Bronstein-Semendyaev
	z = (pt1.y - pt2.y)*(pt4.x - pt3.x) - (pt3.y - pt4.y)*(pt2.x - pt1.x);

    // if straight lines are parallel

	if(z == 0) {
		return false;
	}
    // if straight lines aren't parallel
	else{
		pt->x = ((pt2.x - pt1.x)*(pt3.x*(pt4.y - pt3.y) - pt3.y*(pt4.x - pt3.x)) -
			(pt4.x - pt3.x)*(pt1.x*(pt2.y - pt1.y) - pt1.y*(pt2.x - pt1.x)))/z;
		pt->y = ((pt3.y - pt4.y)*(pt1.x*(pt2.y - pt1.y) - pt1.y*(pt2.x - pt1.x)) -
			(pt1.y - pt2.y)*(pt3.x*(pt4.y - pt3.y) - pt3.y*(pt4.x - pt3.x)))/z;
	}

	return true;
}

//==================================
//! Determination of coordinates of a point of intersection of two straight lines,
//! A1*x set by the equations + B1*y + C1 = 0 and A2*x + B2*y + C2 = 0;
//! pt - the pointer on a point of intersection of two straight lines
bool CROSSLS(double A1, double B1, double C1, double A2, double B2, double C2, MPoint* pt)
//==================================
{
	double z;
    // the formula for finding of coordinates of a point of intersection is removed according to the reference book Bronstein-Semendyaev
    z = A1*B2 - A2*B1;
	if(z == 0) {
		return false;
	}
	else{
		pt->x = (B1*C2 - B2*C1)/z;
		pt->y = (C1*A2 - C2*A1)/z;
	}

	return true;
}

//==================================
//! Determination of the focused area of a triangle
double OTS(MPoint pt1, MPoint pt2, MPoint pt3)
//==================================		
{
	double s = pt1.x * (pt2.y - pt3.y) - pt1.y * (pt2.x - pt3.x) + pt2.x * pt3.y - pt3.x * pt2.y;
	return s;
}

//==================================
//! Determination of coordinates of a point with height of c lying on a segment
//! from the ends of pt1 and pt2 and heights of the ends of a segment of h1 and h2 respectively
MPoint COOPT(MPoint pt1, MPoint pt2, double h1, double h2, double c)
//==================================		
{ 
	MPoint pt;





	pt.x = pt1.x + (c - h1)*(pt2.x - pt1.x)/(h2 - h1);
	pt.y = pt1.y + (c - h1)*(pt2.y - pt1.y)/(h2 - h1);
	return pt;
}

//==================================
//! Determination of height of a point of pt with coordinates of pt.x and pt.y lying on a segment
//! from the ends of pt1 and pt2 and heights of the ends of a segment of h1 and h2 respectively
double HGTPT(MPoint pt1, MPoint pt2, MPoint pt, double h1, double h2)
//==================================		
{ 
	double c;
	c = h1 + (h2 - h1)*(pt.x - pt1.x)/(pt2.x - pt1.x);
    c = h1 + (h2 - h1)*(pt.y - pt1.y)/(pt2.y - pt1.y); // serves function check
	return c;
}

//==================================
//! Definition of a cosine of the angle on three points on the plane
//! the second point - vertex of angle
double COS3Points(MPoint pt1, MPoint pt2, MPoint pt3)
//==================================		
{
	double cos;
	double sqroot;

	sqroot = sqrt(((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y))*((pt3.x - pt1.x)*(pt3.x - pt1.x) + (pt3.y - pt1.y)*(pt3.y - pt1.y)));

    // in that specific case, if length of the first vector is equal to zero, and the cosine is equal to zero
	if (sqroot==0) cos = 0;
    // generally
	else cos = (double)((pt2.x - pt1.x)*(pt3.x - pt1.x) + (pt2.y - pt1.y)*(pt3.y - pt1.y))/sqroot;
	return cos;
}

//==================================
//! Definition of a cosine of the angle between two vectors on the plane
//! Each vector is set by two ends
double COS2Vector(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4)
//==================================		
{





    // pt1 and pt2 - respectively beginning and end of the first vector
    // pt3 and pt4 - respectively beginning and end of the second vector
	double cos;
	double sqroot;

    // length of the first vector
	sqroot = sqrt(((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y))*((pt3.x - pt1.x)*(pt3.x - pt1.x) + (pt3.y - pt1.y)*(pt3.y - pt1.y)));

    // in that specific case, if length of the first vector is equal to zero, and the cosine is equal to zero
	if (sqroot==0) cos = 0;
    // generally
	else cos = (double)((pt2.x - pt1.x)*(pt4.x - pt3.x) + (pt2.y - pt1.y)*(pt4.y - pt3.y))/sqroot;

	return cos;
}
//==================================
//! Definition of a sign of number
int ZNAK(double d)
//==================================		
{
	if(d>0)
		return 1;
	return (d<0)?-1:0;
}

//==================================
//! Determination of coordinates of the vector lying on a perpendicular to a bisector
//! between two set vectors with the general beginning in 'pt1' point.
/**'cos' parameter
// is defined by a cosine of the angle between a vector perpendicular to a bisector
//and one of the set vectors (see drawing of Bezier curves in 'Draw' functuion for hyper points)
//GENERAL IDEA: we normalize the set two vectors, we take their difference and we consider transfer
//the beginnings of coordinates in a point of 'pt1' is and there will be a required vector; its direction depends
//from an order of the normalized vectors participating in a difference
*/
MPoint TANGV(MPoint pt1, MPoint pt2, MPoint pt3, double cos, int p, int q, int s)
//==================================
{ 
	MPoint pt;
	pt.x = (pt2.x - pt1.x)/sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)) - (pt3.x - pt1.x)/sqrt((pt3.x - pt1.x)*(pt3.x - pt1.x) + (pt3.y - pt1.y)*(pt3.y - pt1.y));
	pt.y = (pt2.y - pt1.y)/sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)) - (pt3.y - pt1.y)/sqrt((pt3.x - pt1.x)*(pt3.x - pt1.x) + (pt3.y - pt1.y)*(pt3.y - pt1.y));
    double distpt = sqrt(pt.x * pt.x + pt.y * pt.y); // distpt - length of a vector difference of two normalized vectors (including the beginning of coordinates in 'pt1' point)
    double distsg = sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)); // distsg - segment length between points of 'pt1' and 'pt2'
    double par = (distsg/distpt); // ratio of lengths of segments of 'distsg' and 'distpt'
	

    // all constant coefficients are picked up in the empirical way
    //a case when length of a vector difference is more than length of a segment between points of 'pt1' and 'pt2'
    //we reduce length of a tangential vector for avoidance of loops in Bezier's spline
	if(par < 1) {
        pt.x = pt.x/((distpt/distsg)*p) + pt1.x; // p - parameter of small segments
		pt.y = pt.y/((distpt/distsg)*p) + pt1.y;
	}
    // case when length of a vector difference is no more than length of a segment between points of 'pt1' and 'pt2'
    //we increase length of a tangential vector depending on value of the 'par' parameter and a cosine of the angle between a tangent vector in 'pt1' point a vector and the 'pt1' 'pt2' segment for avoidance of loops in Bezier's spline
	else {
		if(cos <= 0.5) {
            pt.x = par/q*pt.x + pt1.x; // q - parameter of acute angles
			pt.y = par/q*pt.y + pt1.y;
		}
		else {
            pt.x = (cos/s)*par*pt.x + pt1.x; // s - roundness parameter
			pt.y = (cos/s)*par*pt.y + pt1.y;
		}
	}
	return pt;
}

//==================================
//! Determination of values of the second derivatives of the function interpolating horizontals the cubic
//! splines in the 'ptj.x' hubs where 'j' changes from 0 to quantity of points in a horizontal minus unit
double* DERIV2(MTopobject* pTpo)
//==================================
 {
     //at first we will express all second derivatives of required function in nodal points through the second derivative in point 'm_points[GetPointsNumber ()-1]'
     double* derivx; // in the 'derivx' array we will store coefficients at the second derivative in point 'm_points[GetPointsNumber ()-1]'
     double* derivc; // we will store in the 'derivc' array free members

     double A, B, C, D, b; // the coefficients participating in recursive formulas for determination of coefficients in formulas for the second derivatives
     //second experience//
	 A = 1;
	 B = 0;
	 C = -2;
	 D = (3*pTpo->GetPoint(pTpo->GetPointsNumber()-1).y - pTpo->GetPoint(pTpo->GetPointsNumber()-2).y)/(pTpo->GetPoint(pTpo->GetPointsNumber()-1).x - pTpo->GetPoint(pTpo->GetPointsNumber()-2).x);

	 derivx = (double*)malloc(sizeof(double)*pTpo->GetPointsNumber());
	 derivc = (double*)malloc(sizeof(double)*pTpo->GetPointsNumber());
     // for point 'pTpo->m_points[pTpo->GetPointsNumber()-2]' we fill arrays of coefficients out of a cycle
	 derivx[pTpo->GetPointsNumber()-2] = -2;
	 derivc[pTpo->GetPointsNumber()-2] = D;
	
	 int j = 0;

	 for(j=pTpo->GetPointsNumber()-3; j>=0; j--) {
		derivx[j] = (-1)/(pTpo->GetPoint(j+2).x - pTpo->GetPoint(j+1).x)*(2*C*(pTpo->GetPoint(j+2).x - pTpo->GetPoint(j).x) + A*(pTpo->GetPoint(j+1).x - pTpo->GetPoint(j).x));
		b = 3*(pTpo->GetPoint(j+1).x - pTpo->GetPoint(j).x)*((pTpo->GetPoint(j+1).y - pTpo->GetPoint(j).y)/((pTpo->GetPoint(j+1).x - pTpo->GetPoint(j).x)*(pTpo->GetPoint(j+1).x - pTpo->GetPoint(j).x)) + (pTpo->GetPoint(j+2).y - pTpo->GetPoint(j+1).y)/((pTpo->GetPoint(j+2).x - pTpo->GetPoint(j+1).x)*(pTpo->GetPoint(j+2).x - pTpo->GetPoint(j+1).x)));
		derivc[j] = b - (1/(pTpo->GetPoint(j+2).x - pTpo->GetPoint(j+1).x))*(2*D*(pTpo->GetPoint(j+2).x - pTpo->GetPoint(j).x) + B*(pTpo->GetPoint(j+1).x - pTpo->GetPoint(j).x));
	 }

     // we find the second derivative of required function in a point pTpo->m_points[pTpo->GetPointsNumber()-1].x
	 double s = (3*(pTpo->GetPoint(1).y - pTpo->GetPoint(0).y)/(pTpo->GetPoint(1).x - pTpo->GetPoint(0).x) - 2*derivc[0] - derivc[1])/(2*derivx[0] + derivx[1]);
	 derivx[pTpo->GetPointsNumber()-1] = s;
     // we find the second derivatives of required function in all nodal points
	 for(j=0; j<pTpo->GetPointsNumber()-1; j++) {
		 derivx[j] = derivx[j]*s + derivc[j];
	 }
     //second experience//

	 free (derivc);
	 return (derivx);
 }

//==================================
//! Determination of distance from pt point to the straight line passing through two points: 'pt1' and 'pt2'
double RASSTPTL(MPoint pt, MPoint pt1, MPoint pt2)
//==================================
{
	double A, B, C;

    // coefficients and free member of the equation of a straight line

	A = pt1.y - pt2.y;
	B = pt2.x - pt1.x;
	C = pt1.x*(pt2.y - pt1.y) - pt1.y*(pt2.x - pt1.x);
	int zn = ZNAK(C);
	double norma = 1/sqrt((pt2.y - pt1.y)*(pt2.y - pt1.y) + (pt2.x - pt1.x)*(pt2.x - pt1.x));
	if(zn == 1)
		norma = (-1)*norma;
	if((zn == 0) || (zn == -1))
		norma = norma;

    // normal equation of a straight line: A*norma*x + B*norma*y + C*norma = 0
    // distance  'rasstpt' from 'pt' point to a straight line - the number module  A*norma*pt.x + B*norma*pt.y + C*norma
	double rasstpt = fabs(A*norma*pt.x + B*norma*pt.y + C*norma);

	return rasstpt;
}

//==================================
//! Determination of distance between two points of 'pt1' and 'pt2'
double DLINASG(const MPoint& pt1, const MPoint& pt2)
//==================================
{
	double dlina = sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y));
	return dlina;
}

//==================================
//! Determination of length of the broken line
double DLINALOMLINE(const MPtArray* pLomline)
//==================================
{
	double dlina = 0;

	for(int i=0; i<pLomline->GetSize()-1; i++){
		dlina += DLINASG(*pLomline->GetAt(i), *pLomline->GetAt(i+1));
	}

	return dlina;
}

//==================================
//! Determination of coordinates of a normal to the plane set by three points of 'pt1', 'pt2', 'pt3'
//! For the plane set by the equation of  A*x+B*y+C*z+D=0, normal vector coordinate N=(A,B,C).
//! From the equation of the plane set by three points 'pt1(x1,y1,z1)', 'pt2(x2,y2,z2)', 'pt3(x3,y3,z3)' we have:
//! A=y1*(z2-z3)+z1*(y3-y2)+(y2*z3-y3*z2), B=z1*(x2-x3)+x1*(z3-z2)+(z2*x3-x2*z3), C=x1*(y2-y3)+y1*(x3-x2)+(x2*y3-x3*y2)
bool NORMTOPLANE(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double* norm)
//==================================
{
	if(((x1==x2)&&(y1==y2)&&(z1==z2))||((x1==x3)&&(y1==y3)&&(z1==z3))||((x2==x3)&&(y2==y3)&&(z2==z3)))
		return false;
    //check, whether three points on one straight line lie
	if((y2-y1)*x3+(x1-x2)*y3+y1*(x2-x1)-x1*(y2-y1)==0)
		return false;
	norm[0]=y1*(z2-z3)+z1*(y3-y2)+(y2*z3-y3*z2);
	norm[1]=z1*(x2-x3)+x1*(z3-z2)+(z2*x3-x2*z3);
	norm[2]=x1*(y2-y3)+y1*(x3-x2)+(x2*y3-x3*y2);
	return true;
}

//==================================
//! Definition of the decision of system of the linear equations with a symmetric five-diagonal matrix Haletsky's by method

/** for creation of approximating cubic splines with boundary conditions of the first or second sort
//'pA' - the index on the array for storage of coefficients of a matrix 'A' - contains '3*n' elements, considering symmetry and five-diagonal matrixes 'A'

// utvector - the pointer on the array for storage of a column of free members of system of the linear equations A*x=b
// n+1 - dimension of a matrix 'A'
// GENERAL IDEA: a five-diagonal symmetric matrix 'A' it is displayed on work of the three-diagonal lower-triangular
 //matrixes 'B' and the three-diagonal upper-triangular matrix 'C' with units on the main diagonal.
 //'B' and 'C' we determine coefficients of matrixes by formulas of algorithm of Haletsky - see B.P.Demidovich, I. A. Maron of "Fundamentals of calculus mathematics"
 // REMARK: coefficients of a symmetric matrix 'A' it is stored a look (a00,a01,a02,a11,a12,a13,...,an-2n-2,an-2n-1,an-2n,an-1n-1,an-1n,ann)
 // coefficients of a matrix 'B' it is stored a look (on columns) (b00,b10,b20,b11,b21,b31,...,bn-2n-2,bn-1n-2,bnn-2,bn-1n-1,bnn-1,bnn)
 // coefficients of a matrix 'C' it is stored a look (in the lines) (c01,c02,c12,c13,...,cn-2n-1,cn-2n,cn-1n)
*/
double* HLINSYST(double* pA, double* utvector, int n)
//==================================
{

    double* pB  =NULL;  //the pointer on the array for storage of coefficients of a matrix 'B' - contains '3*n' elements
    double* pC  =NULL;  //the pointer on the array for storage of coefficients of a matrix 'C' - contains '2*(n-1)+1' elements
    double* res =NULL; //the pointer on the array for storage of the decision of system of the linear equations of 'A*x=b' - contains 'n+1' elements
    double* helper =NULL; //the pointer on the array for storage of the decision of system of the linear equations of 'B*y=b' - contains 'n+1' elements
	if(n>3){
		pB = (double*)malloc(sizeof(double)*3*n);
		pC = (double*)malloc(sizeof(double)*(2*(n-1)+1));
		res = (double*)malloc(sizeof(double)*(n+1));
		helper = (double*)malloc(sizeof(double)*(n+1));
        // we will fill initial elements of three diagonals of a matrix 'B': 'b00', 'b10', 'b20'
		int i = 0;
		for(i=0; i<3; i++){
			pB[i] = pA[i];
		}
        // we will fill initial elements of two top collateral diagonals of a matrix 'C': 'c01', 'c02'
		for(i=0; i<2; i++){
			pC[i] = pA[i+1]/pB[0];
		}
        // we will fill the first column of a matrix 'B' - the elements 'b11', 'b21', 'b31'
		pB[3] = pA[3] - pB[1]*pC[0];
		pB[4] = pA[4] - pB[2]*pC[0];
		pB[5] = pA[5];
        // we will fill the first line of a matrix 'C' - elements 'c12', 'c13'
		pC[2] = pB[4]/pB[3];
		pC[3] = pB[5]/pB[3];
		for(int j=2; j<n-1; j++){
            // we will fill j-th a matrix column 'B' - elements 'bjj', 'bj+1j', 'bj+2j'
			pB[3*j] = pA[3*j] - pB[3*j-4]*pC[2*j-3] - pB[3*j-2]*pC[2*j-2];
			pB[3*j+1] = pA[3*j+1] - pB[3*j-1]*pC[2*j-2];
			pB[3*j+2] = pA[3*j+2];
            // we will fill j-th a line of a matrix 'C' - elements 'сjj+1', 'сjj+2'
			pC[2*j] = pB[3*j+1]/pB[3*j];
			pC[2*j+1] = pB[3*j+2]/pB[3*j];

		}
        // we will fill (n-1)-th column of a matrix 'B' - the elements 'bn-1n-1', 'bnn-1'
		pB[3*(n-1)] = pA[3*(n-1)] - pB[3*(n-1)-4]*pC[2*(n-1)-3] - pB[3*(n-1)-2]*pC[2*(n-1)-2];
		pB[3*(n-1)+1] = pA[3*(n-1)+1] - pB[3*(n-1)-1]*pC[2*(n-1)-2];
        // we will fill (n-1)-th line of a matrix 'C' - an element  'сn-1n'
		pC[2*(n-1)] = pB[3*(n-1)+1]/pB[3*(n-1)];
        // we will fill n-th a matrix column 'B' - the 'bnn' element
		pB[3*n-1] = pA[3*n-1] - pB[3*n-4]*pC[2*n-5] - pB[3*n-2]*pC[2*n-4];

        // Now we will look for the decision 'A*x=b' system in the form of '(В*С)*x=b (C*x=y)', therefore, 'B*y=b'

        // From a type of a matrix 'B' we find 'y' and we keep it in the 'helper' array:
		helper[0] = utvector[0]/pB[0];
		helper[1] = (1/pB[3])*(utvector[1] - pB[1]*helper[0]); 

		for(i=2; i<n; i++){
			helper[i] = (1/pB[3*i])*(utvector[i] - pB[3*i-4]*helper[i-2] - pB[3*i-2]*helper[i-1]); 
		}
		helper[n] = (1/pB[3*n-1])*(utvector[n] - pB[3*n-4]*helper[n-2] - pB[3*n-2]*helper[n-1]); 

        // At last, we find a required vector 'x' from system with the top triangular matrix with units on diagonal 'C*x=y'
        //We keep 'x' in the 'res' array:
		res[n] = helper[n];
		res[n-1] = helper[n-1] - pC[2*n-2]*res[n];
		for(i=n-2; i>=0; i--){
			res[i] = helper[i] - pC[2*i]*res[i+1] - pC[2*i+1]*res[i+2];
		}
    } // end if(n>3)

	if(n==3){
		pB = (double*)malloc(sizeof(double)*9);
		pC = (double*)malloc(sizeof(double)*5);
		res = (double*)malloc(sizeof(double)*(n+1));
		helper = (double*)malloc(sizeof(double)*(n+1));
        // we will fill initial elements of three diagonals of a matrix 'B': 'b00', 'b10', 'b20'
		int i = 0;
		for(i=0; i<3; i++){

			pB[i] = pA[i];
		}
        // we will fill initial elements of two top collateral diagonals of a matrix 'C': 'c01', 'c02'
		for(i=0; i<2; i++){
			pC[i] = pB[i+1]/pB[0];
		}
        // we will fill the first column of a matrix 'B' - the elements 'b11', 'b21', 'b31'
		pB[3] = pA[3] - pB[1]*pC[0];
		pB[4] = pA[4] - pB[2]*pC[0];
		pB[5] = pA[5];
        // we will fill the first line of a matrix 'C' - the elements 'с12', 'с13'
		pC[2] = pB[4]/pB[3];
		pC[3] = pB[5]/pB[3];
        // the elements 'b22', 'b32'
		pB[6] = pA[6] - pB[2]*pC[1] - pB[4]*pC[2];
		pB[7] = pA[7] - pB[5]*pC[2];
        // the element 'c23'
		pC[4] = pB[7]/pB[6];


        // the element 'b33'
		pB[8] = pA[8] - pB[5]*pC[3] - pB[7]*pC[4];

        // Now we will look for the decision of 'A*x=b' system in the form of '(В*С)*x=b (C*x=y)', therefore, 'B*y=b'
        // From a type of a matrix 'B' we find 'y' and we keep it in the 'helper' array:
        helper[0] = utvector[0]/pB[0];
		helper[1] = (1/pB[3])*(utvector[1] - pB[1]*helper[0]); 
		helper[2] = (1/pB[6])*(utvector[2] - pB[2]*helper[0] - pB[4]*helper[1]); 
		helper[3] = (1/pB[8])*(utvector[3] - pB[5]*helper[1] - pB[7]*helper[2]); 


        // At last, we find a required vector 'x' from system with the top triangular matrix with units on diagonal 'C*x=y'
        //We keep 'x' in the 'res' array:
		res[3] = helper[3];
		res[2] = helper[2] - pC[4]*res[3];
		res[1] = helper[1] - pC[3]*res[3] - pC[2]*res[2];
		res[0] = helper[0] - pC[1]*res[2] - pC[0]*res[1];
    } // end if(n==3)

	if(n==2){
		pB = (double*)malloc(sizeof(double)*6);
		pC = (double*)malloc(sizeof(double)*3);
		res = (double*)malloc(sizeof(double)*(n+1));
		helper = (double*)malloc(sizeof(double)*(n+1));
        // we will fill initial elements of three diagonals of a matrix 'B' : 'b00', 'b10', 'b20'
		int i = 0;
		for(i=0; i<3; i++){
			pB[i] = pA[i];

		}
        // we will fill initial elements of two top collateral diagonals of a matrix 'C': 'c01', 'c02'
		for(i=0; i<2; i++){
			pC[i] = pB[i+1]/pB[0];
		}
        // we will fill the first column of a matrix 'В' - elements 'b11', 'b21'
		pB[3] = pA[3] - pB[1]*pC[0];
		pB[4] = pA[4] - pB[2]*pC[0];
        // we will fill the first line of a matrix 'С' - the element 'с12'
		pC[2] = pB[4]/pB[3];
        // the element 'b22'
		pB[5] = pA[5] - pB[2]*pC[1] - pB[4]*pC[2];

        // Now we will look for the decision of 'A*x=b' system in the form of '(В*С)*x=b (C*x=y)', therefore, 'B*y=b'
        // From a type of a matrix In we find 'y' and we keep it in the 'helper' array:
		helper[0] = utvector[0]/pB[0];
		helper[1] = (1/pB[3])*(utvector[1] - pB[1]*helper[0]); 
		helper[2] = (1/pB[5])*(utvector[2] - pB[2]*helper[0] - pB[4]*helper[1]); 

        // At last, we find a required vector 'x' from system with the top triangular matrix with units on diagonal 'C*x=y'
        //We keep 'x' in the 'res' array:
		res[2] = helper[2];
		res[1] = helper[1] - pC[2]*res[2];
		res[0] = helper[0] - pC[1]*res[2] - pC[0]*res[1];
    } // end if(n==2)
    // we release auxiliary memory
	free (pB);
	free (pC);
	free (helper);

	return res;}

//==================================
//! Definition of the decision of system of the linear equations with a symmetric matrix by Haletsky's  method

/** for creation of approximating cubic splines with boundary conditions of the third sort
// pA - the pointer on the array for storage of coefficients of a matrix 'A' - contains '3*n' elements, considering symmetry of a matrix 'A'
// utvector - the pointer on the array for storage of a column of free members of system of the linear equations of 'A*x=b'
// n - dimension of a matrix 'A'
// GENERAL IDEA: a symmetric matrix 'A' it is displayed on work lower-triangular
 //matrixes 'B' and a upper-triangular matrix 'C' with units on the main diagonal.
 //In and With we determine coefficients of matrixes by formulas of algorithm of Haletsky - see B.P.Demidovich, I. A. Maron of "Fundamentals of calculus mathematics"
 // REMARK: coefficients of a symmetric matrix 'A' it is stored a look (a11,a12,a13,a1n-1,a1n,a22,a23,a24,a2n,a33,a34,a35,...,an-2n-2,an-2n-1,an-2n,an-1n-1,an-1n,ann)
 // coefficients of a matrix 'B' it is stored a look (on columns) (b11,b21,b31,bn-11,bn1,b22,b32,b42,bn-12,bn2,...bn-4n-4,bn-3n-4,bn-2n-4,bn-1n-4,bnn-4,bn-3n-3,bn-2n-3,bn-1n-3,bnn-3,bn-2n-2,bn-1n-2,bnn-2,bn-1n-1,bnn-1,bnn)
 // coefficients of a matrix 'C' it is stored a look (in the lines) (c12,c13,с1n-1,c1n,c23,c24,c2n-1,c2n,...cn-4n-3,cn-4n-2,cn-4n-1,cn-4n.cn-3n-2,cn-3n-1,cn-3n,cn-2n-1,cn-2n,cn-1n)
*/
double* HLINSYST3(double* pA, double* utvector, int n)
//==================================
{ 
    double* pB =NULL;  //the pointer on the array for storage of coefficients of a matrix 'B' - contains '5*(n-2)' elements
    double* pC =NULL;  //the pointer on the array for storage of coefficients of a matrix 'C' - contains '2*(2*n-5)' elements
    double* res =NULL; //the pointer on the array for storage of the decision of system of the linear equations of 'A*x=b' - contains 'n' elements
    double* helper =NULL; //the pointer on the array for storage of the decision of system of the linear equations of 'B*y=b' - contains 'n' elements
	if(n>=5){
		pB = (double*)malloc(sizeof(double)*5*(n-2));
		pC = (double*)malloc(sizeof(double)*2*(2*n-5));
		res = (double*)malloc(sizeof(double)*n);
		helper = (double*)malloc(sizeof(double)*n);
        // we will fill nonzero elements of the first column of a matrix 'В': 'b11', 'b21', 'b31', 'bn-11', 'bn1'
		int i = 0;
		for(i=0; i<5; i++){
			pB[i] = pA[i];
		}
        // we will fill nonzero elements of the first line of a matrix 'C': 'c11', 'c12', 'c1n-1', 'c1n'
		for(i=0; i<4; i++){

			pC[i] = pB[i+1]/pB[0];
		}
        // we will fill the second column of a matrix 'В' - elements 'b22', 'b32', 'b42', 'bn-12', 'bn2'
		pB[5] = pA[5] - pB[1]*pC[0];
		pB[6] = pA[6] - pB[2]*pC[0];
		pB[7] = pA[7];
		pB[8] = -pB[3]*pC[0];
		pB[9] = pA[8] - pB[4]*pC[0];
        // we will fill the second line of a matrix 'С' - elements 'с23', 'с24', 'c2n-1', 'c2n'
		pC[4] = pB[6]/pB[5];
		pC[5] = pB[7]/pB[5];
		pC[6] = pB[8]/pB[5];
		pC[7] = pB[9]/pB[5];
		int j = 0;
		for(j=3; j<n-3; j++){

            // we will fill j-th a matrix column 'В' - elements 'bjj', 'bj+1j', 'bj+2j', 'bn-1j', 'bnj'
			pB[5*(j-1)] = pA[3*(j-1)+3] - pB[5*(j-1)-8]*pC[4*(j-1)-7] - pB[5*(j-1)-4]*pC[4*(j-1)-4];
			pB[5*(j-1)+1] = pA[3*(j-1)+4] - pB[5*(j-1)-3]*pC[4*(j-1)-4];
			pB[5*(j-1)+2] = pA[3*(j-1)+5];
			pB[5*(j-1)+3] = - pB[5*(j-1)-7]*pC[4*(j-1)-7] - pB[5*(j-1)-2]*pC[4*(j-1)-4];
			pB[5*(j-1)+4] = - pB[5*(j-1)-6]*pC[4*(j-1)-7] - pB[5*(j-1)-1]*pC[4*(j-1)-4];
            // we will fill j-th a matrix line 'С' - elements 'сjj+1', 'сjj+2', 'cjn-1', 'cjn'
			pC[4*(j-1)] = pB[5*(j-1)+1]/pB[5*(j-1)];
			pC[4*(j-1)+1] = pB[5*(j-1)+2]/pB[5*(j-1)];
			pC[4*(j-1)+2] = pB[5*(j-1)+3]/pB[5*(j-1)];
			pC[4*(j-1)+3] = pB[5*(j-1)+4]/pB[5*(j-1)];

		}

		if(n>=6){
        // we will fill (n-3)-th column of a matrix 'В' - elements 'bn-3n-3', 'bn-2n-3', 'bn-1n-3', 'bnn-3'
		pB[5*(n-4)] = pA[3*(n-4)+3] - pB[5*(n-4)-8]*pC[4*(n-4)-7] - pB[5*(n-4)-4]*pC[4*(n-4)-4];
		pB[5*(n-4)+1] = pA[3*(n-4)+4] - pB[5*(n-4)-3]*pC[4*(n-4)-4];
		pB[5*(n-4)+2] = pA[3*(n-4)+5] - pB[5*(n-4)-7]*pC[4*(n-4)-7] - pB[5*(n-4)-2]*pC[4*(n-4)-4];
		pB[5*(n-4)+3] = - pB[5*(n-4)-6]*pC[4*(n-4)-7] - pB[5*(n-4)-1]*pC[4*(n-4)-4];
        // we will fill (n-3)-th line of a matrix 'С' - elements 'сn-3n-2', 'сn-3n-1', 'cn-3n'
		pC[4*(n-4)] = pB[5*(n-4)+1]/pB[5*(n-4)];
		pC[4*(n-4)+1] = pB[5*(n-4)+2]/pB[5*(n-4)];
		pC[4*(n-4)+2] = pB[5*(n-4)+3]/pB[5*(n-4)];
		}
		
        // we will fill (n-2)-th column of a matrix 'В' - elements 'bn-2n-2', 'bn-1n-2', 'bnn-2'
		pB[5*(n-3)-1] = pA[3*(n-3)+3] - pB[5*(n-3)-8]*pC[4*(n-3)-7] - pB[5*(n-3)-4]*pC[4*(n-3)-4];
		pB[5*(n-3)] = pA[3*(n-3)+4] - pB[5*(n-3)-7]*pC[4*(n-3)-7] - pB[5*(n-3)-3]*pC[4*(n-3)-4];
		pB[5*(n-3)+1] = pA[3*(n-3)+5] - pB[5*(n-3)-6]*pC[4*(n-3)-7] - pB[5*(n-3)-2]*pC[4*(n-3)-4];
        // we will fill (n-2)-th line of a matrix 'С' - elements 'сn-2n-1', 'сn-2n'
		pC[4*(n-3)-1] = pB[5*(n-3)]/pB[5*(n-3)-1];
		pC[4*(n-3)] = pB[5*(n-3)+1]/pB[5*(n-3)-1];

        // we will fill (n-1)-th column of a matrix 'В' :
        //elements 'bn-1n-1'
		double summa = pB[3]*pC[2] + pB[8]*pC[6];
		for(j=3; j<n-3; j++){
			summa += pB[5*(j-1)+3]*pC[4*(j-1)+2];
		}
		summa += (pB[5*(n-4)+2]*pC[4*(n-4)+1] + pB[5*(n-3)]*pC[4*(n-3)-1]);
		pB[5*(n-2)-3] = pA[3*(n-2)+3] - summa;
        //element 'bnn-1'
		summa = pB[4]*pC[2] + pB[9]*pC[6];
		for(j=3; j<n-3; j++){
			summa += pB[5*(j-1)+4]*pC[4*(j-1)+2];
		}
		summa += (pB[5*(n-4)+3]*pC[4*(n-4)+1] + pB[5*(n-3)+1]*pC[4*(n-3)-1]);
		pB[5*(n-2)-2] = pA[3*(n-2)+4] - summa;
        // we will fill (n-1)-th line of a matrix 'С' - elements 'сn-1n'
		pC[4*(n-2)-3] = pB[5*(n-2)-2]/pB[5*(n-2)-3];

        // we will fill n-th column of a matrix 'В' :
        //element 'bnn'
		summa = pB[4]*pC[2] + pB[9]*pC[6];
		for(j=3; j<n-3; j++){
			summa += pB[5*(j-1)+4]*pC[4*(j-1)+2];
		}
		summa += (pB[5*(n-4)+3]*pC[4*(n-4)+1] + pB[5*(n-3)+1]*pC[4*(n-3)-1] + pB[5*(n-2)-2]*pC[4*(n-2)-3]);
		pB[5*(n-2)-1] = pA[3*(n-2)+5] - summa;
		
        // Now we will look for the decision of 'A*x=b' system in the form of '(В*С)*x=b (C*x=y)', therefore, 'B*y=b'
        // From a type of a matrix 'B' we find 'y' and we keep it in the 'helper' array:
		helper[0] = utvector[0]/pB[0];
		helper[1] = (1/pB[5])*(utvector[1] - pB[1]*helper[0]); 
		for(i=3; i<n-2; i++){
			helper[i-1] = (1/pB[5*(i-1)])*(utvector[i-1] - pB[5*(i-1)-8]*helper[i-3] - pB[5*(i-1)-4]*helper[i-2]); 
		}
		helper[n-3] = (1/pB[5*(n-3)-1])*(utvector[n-3] - pB[5*(n-3)-8]*helper[n-5] - pB[5*(n-3)-4]*helper[n-4]); 
		summa = pB[3]*helper[0];
		for(i=1; i<n-4; i++){
			summa += pB[5*i+3]*helper[i];
		}
		summa += pB[5*(n-4)+2]*helper[n-4] + pB[5*(n-3)]*helper[n-3];
		helper[n-2] = (1/pB[5*(n-3)-1])*(utvector[n-2] - summa); 
		summa = pB[4]*helper[0];
		for(i=1; i<n-4; i++){
			summa += pB[5*i+4]*helper[i];
		}
		summa += pB[5*(n-4)+3]*helper[n-4] + pB[5*(n-3)+1]*helper[n-3] + pB[5*(n-2)-2]*helper[n-2]; 
		helper[n-1] = (1/pB[5*(n-2)-1])*(utvector[n-1] - summa); 

        // At last, we find a required vector 'x' from system with the top triangular matrix with units on diagonal 'C*x=y'
        //We keep 'x' in the 'res' array:
		res[n-1] = helper[n-1];
		res[n-2] = helper[n-2] - pC[4*(n-2)-3]*res[n-1];
		res[n-3] = helper[n-3] - pC[4*(n-3)-1]*res[n-2] - pC[4*(n-3)]*res[n-1];
		res[n-4] = helper[n-4] - pC[4*(n-4)]*res[n-3] - pC[4*(n-4)+1]*res[n-2] - pC[4*(n-4)+2]*res[n-1];
		for(i=n-5; i>=0; i--){
			res[i] = helper[i] - pC[4*i]*res[i+1] - pC[4*i+1]*res[i+2] - pC[4*i+2]*res[n-2] - pC[4*i+3]*res[n-1];
		}
    } // end if(n>=5)

	if(n == 4){
		pB = (double*)malloc(sizeof(double)*10);
		pC = (double*)malloc(sizeof(double)*6);
		res = (double*)malloc(sizeof(double)*n);
		helper = (double*)malloc(sizeof(double)*n);
        // we will fill elements of the first column of a matrix 'В': 'b11', 'b21', 'b31', 'b41'
		int i = 0;
		for(i=0; i<4; i++){
			pB[i] = pA[i];
		}
        // we will fill elements of the first line of a matrix 'C': 'c12', 'c13', 'c14'
		for(i=0; i<3; i++){
			pC[i] = pB[i+1]/pB[0];
		}
        // we will fill elements of the second column of a matrix 'В' - elements 'b22', 'b32', 'b42'
		pB[4] = pA[4] - pB[1]*pC[0];
		pB[5] = pA[5] - pB[2]*pC[0];
		pB[6] = pA[6] - pB[3]*pC[0];
        // we will fill elements of the second line of a matrix 'С' - elements 'с23', 'с24'
		pC[3] = pB[5]/pB[4];
		pC[4] = pB[6]/pB[4];
        // we will fill elements of the third column of a matrix 'В' - elements 'b33', 'b43'

		pB[7] = pA[7] - pB[2]*pC[1] - pB[5]*pC[3];
		pB[8] = pA[8] - pB[3]*pC[1] - pB[6]*pC[3];
        // we will fill elements of the third line of a matrix 'С' - element 'с34'
		pC[5] = pB[8]/pB[7];
        // we will fill elements of the fourth column of a matrix 'В' - element 'b44'
		pB[9] = pA[9] - pB[3]*pC[2] - pB[6]*pC[4] - pB[8]*pC[5];

        // Now we will look for the decision of 'A*x=b' system in the form of '(В*С)*x=b (C*x=y)', therefore, 'B*y=b'
        // From a type of a matrix 'B' we find 'y' and we keep it in the 'helper' array:
		helper[0] = utvector[0]/pB[0];
		helper[1] = (1/pB[4])*(utvector[1] - pB[1]*helper[0]); 
		helper[2] = (1/pB[7])*(utvector[2] - pB[2]*helper[0] - pB[5]*helper[1]); 
		helper[3] = (1/pB[9])*(utvector[3] - pB[3]*helper[0] - pB[6]*helper[1] - pB[8]*helper[1]); 
        // At last, we find a required vector 'x' from system with the top triangular matrix with units on diagonal 'C*x=y'
        //We keep 'x' in the 'res' array:
		res[3] = helper[3];
		res[2] = helper[2] - pC[5]*res[3];
		res[1] = helper[1] - pC[4]*res[3] - pC[3]*res[2];
		res[0] = helper[0] - pC[2]*res[3] - pC[1]*res[2] - pC[0]*res[1];
	}


	if(n == 3){
		pB = (double*)malloc(sizeof(double)*6);
		pC = (double*)malloc(sizeof(double)*3);
		res = (double*)malloc(sizeof(double)*n);
		helper = (double*)malloc(sizeof(double)*n);
        // we will fill elements of the first column of a matrix 'В': 'b11', 'b21', 'b31'
		int i = 0;
		for(i=0; i<3; i++){
			pB[i] = pA[i];
		}
        // we will fill elements of the first line of a matrix 'C': 'c12', 'c13'
		for(i=0; i<2; i++){
			pC[i] = pB[i+1]/pB[0];
		}
        // we will fill elements of the second column of a matrix 'В' - elements 'b22', 'b32'
		pB[3] = pA[3] - pB[1]*pC[0];
		pB[4] = pA[4] - pB[2]*pC[0];
        // we will fill elements of the second line of a matrix 'С' - element 'с23'
		pC[2] = pB[4]/pB[3];
        // we will fill elements of the third column of a matrix 'В' - element 'b33'
		pB[5] = pA[5] - pB[2]*pC[1] - pB[4]*pC[2];

        // Now we will look for the decision of 'A*x=b' system in the form of '(В*С)*x=b (C*x=y)', therefore, 'B*y=b'
        // From a type of a matrix 'B' we find 'y' and we keep it in the 'helper' array:
		helper[0] = utvector[0]/pB[0];
		helper[1] = (1/pB[3])*(utvector[1] - pB[1]*helper[0]); 
		helper[2] = (1/pB[5])*(utvector[2] - pB[2]*helper[0] - pB[4]*helper[1]); 
        // At last, we find a required vector 'x' from system with the top triangular matrix with units on diagonal 'C*x=y'
        //We keep 'x' in the 'res' array:
		res[2] = helper[2];
		res[1] = helper[1] - pC[2]*res[2];
		res[0] = helper[0] - pC[1]*res[2] - pC[0]*res[1];
	}
    // we release auxiliary memory
	free (pB);
	free (pC);
	free (helper);

	return res;}


bool PointsAreSame(const MPoint *pt1, const MPoint *pt2, const double &eps)
{
    bool answer = false;

    if(pt1 != NULL && pt2 != NULL)
    {
        const double dx = pt2->x - pt1->x;
        const double dy = pt2->y - pt1->y;
        answer = (dx * dx + dy * dy <= eps * eps);
    }

    return answer;
}


//==================================
//! Determination of coordinates of the center and radius of the circle described round a triangle with tops in points P1=(x1,y1), P2=(x2,y2), P3=(x3,y3)
/** GENERAL IDEA: the center of a circumscribed circle is in a point of intersection of the perpendiculars restored to the parties of a triangle
//in their middle; it is enough to find the equations of two normal straight lines passing through the middle of two parties of a triangle
//the center of the circle described round a triangle - a point of intersection of these straight lines.
// The equation of a straight line, perpendicular to the straight line set by two points P1=(x1,y1), P2=(x2,y2), and passing through the midpoint 'P1P2':
// (x1-x2)*x + (y1-y2)*y + 0.5*((y1+y2)*(y2-y1) + (x1+x2)*(x2-x1)) = 0 - conclusion according to the reference book I. N. Bronstein, K. A. Semendyaev
// Radius of a circumscribed circle easily is defined as distance from the center of a circle to one of triangle tops.
*/
bool CIRCLEPT(MPoint pt1, MPoint pt2, MPoint pt3, MPoint* pt, double* rad)
//==================================		
{
	double A1, A2, B1, B2, C1, C2;
	bool par;
	A1 = pt1.x - pt2.x;
	A2 = pt2.x - pt3.x;
	B1 = pt1.y - pt2.y;
	B2 = pt2.y - pt3.y;
	C1 = 0.5*((pt1.y + pt2.y)*(pt2.y - pt1.y) + (pt1.x + pt2.x)*(pt2.x - pt1.x));
	C2 = 0.5*((pt2.y + pt3.y)*(pt3.y - pt2.y) + (pt2.x + pt3.x)*(pt3.x - pt2.x));
	par = CROSSLS(A1, B1, C1, A2, B2, C2, pt);
	if(!par) 
		return false;
	rad[0] = DLINASG(pt1, pt);
	return true;
}

//==================================
//! Check, whether lies a segment in any coherent area (set as the closed broken line)
/** GENERAL IDEA: 1) we check, whether the ends of a segment in area lie;
//                2) we check that the segment didn't cross any of area contour edges;
//  pt1, pt2 - the ends of a segment, pReg - the index on a polygon, n - number of tops of a polygon
*/

bool SGINREG(MPoint pt1, MPoint pt2, MPtArray* pReg)
//==================================		
{
    //the area can't be less segment
	if(!pReg) return false;
	if(pReg->GetSize()<=1)
		return false;

    //if the area - a segment, and a segment belongs to area
    if((pReg->GetSize()==2)&&((PTONSEG(pt1,*pReg->GetAt(0),*pReg->GetAt(1)),0.00000001)&&(PTONSEG(pt2,*pReg->GetAt(0),*pReg->GetAt(1)),0.00000001)))
		return true;
	else if(pReg->GetSize()==2) return false;

	MPoint pt;
	double r;
	int res=-1;

	if((PointInRegion(pt1,pReg))&&(PointInRegion(pt2,pReg))){
		for(int i=0; i<pReg->GetSize()-1; i++){
			res = IntersectLineWithOtrezok(pt1,pt2,*pReg->GetAt(i),*pReg->GetAt(i+1),&pt,r);
			if(res==0){
				res = IntersectLineWithOtrezok(*pReg->GetAt(i),*pReg->GetAt(i+1),pt1,pt2,&pt,r);
				if(res==0)
					return false;
			}
		}
		res = IntersectLineWithOtrezok(pt1,pt2,*pReg->GetAt(pReg->GetSize()-1),*pReg->GetAt(0),&pt,r);
		if(res==0){
			res = IntersectLineWithOtrezok(*pReg->GetAt(pReg->GetSize()-1),*pReg->GetAt(0),pt1,pt2,&pt,r);
			if(res==0)
				return false;
		}
	}
	else
		return false;

	return true;
}

//==================================
//! Check, whether lies a segment in any coherent area (set as the closed broken line):
/** we consider a case when the ends of a segment can lie on a contour or the segment lying
//in a contour can concern contour tops
// pt1, pt2 - the ends of a segment, pReg - the array of points of a contour
*/
bool SGINREG_EXT(MPoint pt1, MPoint pt2, MPtArray* pReg)
//==================================		
{
	if(!pReg)
		return false;

    //if the segment lies strictly in area
	if(SGINREG(pt1, pt2, pReg))
		return true;

	int n = pReg->GetSize();
	int index, index1, vertexind, vertexind1, res;
	MPoint pt;
	double r;
	if((PTONKONTUR(pt1, pReg, index, vertexind))&&(PointInRegion(pt2, pReg))){
        //if the first point of a segment coincides with the beginning of an edge of a contour with the 'index' index,
        //that the segment is crossed with a contour on an edge with the 'index' index and with the previous
        //contour edge
		if(pt1 == *pReg->GetAt(index)){
			if(index==0)
				index1 = n-2;
			else
				index1 = index-1;
		}
        //if the first point of a segment coincides with the end of an edge of a contour with the 'index' index,
        //that the segment is crossed with a contour on an edge with the 'index' index and with the subsequent
        //contour edge
		else if(pt1==*pReg->GetAt(index+1)){
			if(index==n-2)
				index1 = 0;
			else
				index1 = index+1;

		}
        //the first point lies on a contour edge with the 'index' index, therefore a segment
        //it is crossed only with this edge of a contour
		else
			index1 = -1;
        //we check a segment for crossing with contour edges, excepting edges with numbers
        //'index' and 'index1'
		for(int i=0; i<n-1; i++){
			if((i!=index)&&(i!=index1)){
				res = IntersectLineWithOtrezok(pt1,pt2,*pReg->GetAt(i),*pReg->GetAt(i+1),&pt,r);
				if(res==0){
					res = IntersectLineWithOtrezok(*pReg->GetAt(i),*pReg->GetAt(i+1),pt1,pt2,&pt,r);
					if(res==0)
						return false;
				}
			}
		}
		return true;
	}
	if((PTONKONTUR(pt2, pReg, index, vertexind))&&(PointInRegion(pt1, pReg))){
        //if the second point of a segment coincides with the beginning of an edge of a contour with the 'index' index,
        //that the segment is crossed with a contour on an edge with the 'index' index and with the previous
        //contour edge
		if(pt2 == *pReg->GetAt(index)){
			if(index==0)
				index1 = n-2;
			else
				index1 = index-1;
		}
        //if the second point of a segment coincides with the beginning of an edge of a contour with the 'index' index,
        //that the segment is crossed with a contour on an edge with the 'index' index and with the previous
        //contour edge
		else if(pt2==*pReg->GetAt(index+1)){
			if(index==n-2)
				index1 = 0;
			else
				index1 = index+1;
		}
        //the second point lies on a contour edge with the 'index' index, therefore a segment
        //it is crossed only with this edge of a contour
		else
			index1 = -1;
        //we check a segment for crossing with contour edges, excepting edges with numbers
        //'index' and 'index1'
		for(int i=0; i<n-1; i++){
			if((i!=index)&&(i!=index1)){
				res = IntersectLineWithOtrezok(pt1,pt2,*pReg->GetAt(i),*pReg->GetAt(i+1),&pt,r);
				if(res==0){
					res = IntersectLineWithOtrezok(*pReg->GetAt(i),*pReg->GetAt(i+1),pt1,pt2,&pt,r);
					if(res==0)
						return false;
				}
			}
		}
		return true;
	}
	int index2, index3;
    //both ends of a segment lie on a contour

	if((PTONKONTUR(pt1, pReg, index, vertexind))&&(PTONKONTUR(pt2, pReg, index1, vertexind1))){

        //the ends of a segment lie on one edge of a contour, all segment means
        //lies on this edge
		if(index==index1)
			return true;
        //the segment coincides with one of edges of a contour or one of the ends of a segment
        //coincides with contour top
		if((index==vertexind1)||(vertexind==index1)||(vertexind==vertexind1))
			return true;

		pt.x = (pt1.x + pt2.x)/2;
		pt.y = (pt1.y + pt2.y)/2;
		if(!PointInRegion(pt, pReg))
			return false;
        //if the first point of a segment coincides with the beginning of an edge of a contour with the 'index' index,

        //that the segment is crossed with a contour on an edge with the index 'index' index and with the previous
        //contour edge
		if(pt1 == *pReg->GetAt(index)){
			if(index==0)
				index2 = n-2;
			else
				index2 = index-1;
		}
        //if the first point of a segment coincides with the end of an edge of a contour with the 'index' index,
        //that the segment is crossed with a contour on an edge with the 'index' index and with the subsequent
        //contour edge
		else if(pt1==*pReg->GetAt(index+1)){
			if(index==n-2)
				index2 = 0;
			else
				index2 = index+1;
		}
        //the first point lies on a contour edge with the index 'index', therefore a segment
        //it is crossed only with this edge of a contour
		else
			index2 = -1;
        //if the second point of a segment coincides with the beginning of an edge of a contour with the 'index1' index,
        //that the segment is crossed with a contour on an edge with the index1 index and with the previous
        //contour edge
		if(pt2 == *pReg->GetAt(index1)){
			if(index1==0)
				index3 = n-2;
			else
				index3 = index1-1;
		}
        //if the second point of a segment coincides with the end of an edge of a contour with the 'index' index,
        //that the segment is crossed with a contour on an edge with the 'index' index and with the subsequent
        //contour edge
		else if(pt2==*pReg->GetAt(index1+1)){
			index3 = index1+1;
		}
        //the second point lies on a contour edge with the 'index' index, therefore a segment
        //it is crossed only with this edge of a contour
		else
			index3 = -1;
        //we check a segment for crossing with contour edges, excepting edges with numbers
        //'index', 'index1', 'index2', 'index3'
		for(int i=0; i<n-1; i++){
			if(((i!=index)&&(i!=index1))&&((i!=index2)&&(i!=index3))){
				res = IntersectLineWithOtrezok(pt1,pt2,*pReg->GetAt(i),*pReg->GetAt(i+1),&pt,r);
				if(res==0){
					res = IntersectLineWithOtrezok(*pReg->GetAt(i),*pReg->GetAt(i+1),pt1,pt2,&pt,r);

					if(res==0)
						return false;
				}
			}
		}
		if((index!=n-1)&&(index1!=n-1)&&(index2!=n-1)&&(index3!=n-1)){
			res = IntersectLineWithOtrezok(pt1,pt2,*pReg->GetAt(n-1),*pReg->GetAt(0),&pt,r);
			if(res==0){
				res = IntersectLineWithOtrezok(*pReg->GetAt(n-1),*pReg->GetAt(0),pt1,pt2,&pt,r);
				if(res==0)
					return false;
			}
		}
		return true;
	}

	return false;
}

//==================================
//!Check, whether lies any broken line inside (strict inclusion) any coherent
//! the area (set as the closed broken line)
//! The GENERAL Ideya:proveryaem each segment of a broken line
bool LomlineInRegion(MPtArray* pLomline, MPtArray* pRegion)
//==================================		
{
	if((!pLomline)||(!pRegion))
		return false;

	for(int i=0; i<pLomline->GetSize()-1; i++){
		if(!SGINREG(*pLomline->GetAt(i), *pLomline->GetAt(i+1), pRegion))
			return false;
	}
	return true;
}

//==================================
//!Check, whether lies any broken line inside (mild  inclusion) any coherent
//! the area (set as the closed broken line)
//! The GENERAL Ideya:proveryaem each segment of a broken line
bool LomlineInRegion_EXT(MPtArray* pLomline, MPtArray* pRegion)
//==================================		
{
	if((!pLomline)||(!pRegion))
		return false;

	for(int i=0; i<pLomline->GetSize()-1; i++){
		if(!SGINREG_EXT(*pLomline->GetAt(i), *pLomline->GetAt(i+1), pRegion))
			return false;
	}
	return true;
}

//==================================
//! Check, whether lies any 'pt' point on a segment from the ends 'pt1' and 'pt2'
/** GENERAL IDEA: 1) we check, whether the 'pt' point of the straight line passing through 'pt1pt2' segment belongs;
//2) we check, whether 'pt' point belongs to 'pt1pt2' segment;
//pt - any point, pt1, pt2 - the ends of a segment
// (x2-x1)*y + (y1-y2)*x + x1*(y2-y1) - y1*(x2-x1) = 0 - the equation of the straight line passing through a segment of' p1, p2'.
*/
bool PTONSEG(MPoint point, MPoint segPt1, MPoint segPt2, double ldelta)
//==================================		
{
    bool answer = true;

        const bool ptIsPt1 = PointsAreSame(&point, &segPt1, ldelta);
        const bool ptIsPt2 = PointsAreSame(&point, &segPt2, ldelta);
        if((ptIsPt1 == false) && (ptIsPt2 == false))
        {
			if(ldelta < 0)
			{
				ldelta = -ldelta;
			}

            const double dx = segPt2.x - segPt1.x;
            const double dxAbs = fabs(dx);
            if(dxAbs <= ldelta)
            {
                const double absDif = fabs(point.x - (segPt1.x + segPt2.x) / 2);
                answer = (absDif <= ldelta) && ((point.y >= segPt1.y) == (point.y <= segPt2.y));
            }
            else
            {
                const double dy = segPt2.y - segPt1.y;
                const double dyAbs = fabs(dy);
                if(dyAbs <= ldelta)
                {
                    const double absDif = fabs(point.y - (segPt1.y + segPt2.y) / 2);
                    answer = (absDif <= ldelta) && ((point.x >= segPt1.x) == (point.x <= segPt2.x));
                }
                else
                {
                    if(dxAbs > dyAbs)
                    {
                        answer = ((point.x >= segPt1.x) == (point.x <= segPt2.x));
                    }
                    else
                    {
                        answer = ((point.y >= segPt1.y) == (point.y <= segPt2.y));
                    }
                    if(answer == true)
                    {
                        const double absDif = fabs((point.x - segPt1.x) / dx - (point.y - segPt1.y) / dy);
                        answer = (absDif <= ldelta);
                        if(answer == false)
                        {
                            const MPoint closestPt = GetPointOnOtrezok(segPt1, segPt2, point);
                            answer = PointsAreSame(&point, &closestPt, ldelta);
                        }
                    }
                }
            }
        }

        return answer;
}

//==================================
//!Check, whether lies any 'pt' point on the broken line;
/**for each link of a broken line it is checked, whether 'pt' point belongs to it;
//in case the point belongs to a broken line, we return an edge index,
//on which the point lies; if the point coincides with one of tops,
//we return still an index of an adjacent edge which also possesses a point,
//if doesn't coincide, we believe 'vertexind =-1';
//in a case, the point coincides with one of the ends of a broken line, 'vertexind' specifies
//on a contour top index
*/
bool PTONLOMLINE(MPoint pt, MPtArray* pLomline, int& index, int& vertexind)
//==================================		
{
	if(!pLomline) return false;
	if(pLomline->GetSize() == 0) return false;

	vertexind = -1;
	int i = 0;
	for(i = 0; i<pLomline->GetSize()-1; i++){
		if(PTONSEG(pt, *pLomline->GetAt(i), *pLomline->GetAt(i+1))){
			index = i;
			if(pt==*pLomline->GetAt(i)){
				if(i!=0)
					vertexind = i-1;
				else if(i==0)
					vertexind = i;
			}
			else if(pt==*pLomline->GetAt(i+1)){
				if(i!=pLomline->GetSize()-2)
					vertexind = i+1;
				else if(i==pLomline->GetSize()-2)
					vertexind = i+1;
			}
			break;
		}
	}

	if(i==pLomline->GetSize()-1) return false;


	return true;
}

//==================================
//!Check, whether lies any 'pt' point on the contour;
/**for each link of a contour it is checked, whether 'pt' point belongs to it;
//in case the point belongs to a contour, we return an edge index,
//on which the point lies; if the point coincides with one of tops,
//we return still an index of an adjacent edge which also possesses a point,
//if doesn't coincide, we believe 'vertexind =-1'
//REMARK: the contour is set so that initial and final points don't coincide,
//therefore if an initial and final point coincide in an entrance contour,
//on operating time of function we will exclude the last point
//on a contour top index
*/
bool PTONKONTUR(MPoint point, MPtArray* polygon, int& index, int& vertexind,double delta)
//==================================		
{
    bool answer = false;
            vertexind = -1;
            index = -1;

        if(polygon != NULL)
        {
            const int polSize = polygon->GetSize();
            if(polSize < 2)
            {
                if(polSize == 1)
                {
                    const MPoint *polHead = polygon->GetAt(0);
                    answer = PointsAreSame(&point, polHead, delta);
                }
            }
            else
            {
                MPoint pt1 = *polygon->GetAt(0);
                MPoint pt2;
                bool ptIsOnSeg;
                for(int iPolPtLoop = 1; iPolPtLoop < polSize; ++iPolPtLoop)
                {
                    pt2 = *polygon->GetAt(iPolPtLoop);
                    ptIsOnSeg = PTONSEG(point, pt1, pt2);
                    if(ptIsOnSeg == true)
                    {
                        answer = true;
                        index = iPolPtLoop - 1;
                        if (iPolPtLoop==1)
                        {
                            if (PointsAreSame(&pt1, &point, delta))
                                vertexind=0;
                        }
                        if (PointsAreSame(&pt2, &point, delta))
                            vertexind=iPolPtLoop;

                        break;
                    }
                    pt1 = pt2;
                }
            }
        }

        return answer;
}

//==================================
//!Determination of distance from the beginning of a broken line to the point lying on this broken line;
/**function returns 'false' if the entrance point doesn't lie on a broken line;
//otherwise function returns 'true' and distance from the beginning of a broken line to a broken line point
*/
bool FindDistFromBeginningToLompt(MPoint pt, MPtArray* pLomline, double& rasst)
//==================================		
{
	int index;
	int vertexind;
    //if the point doesn't belong to a broken line
	if(!PTONLOMLINE(pt, pLomline, index, vertexind))
		return false;

	rasst = 0;
	for(int i=0; i<index; i++){
		rasst += DLINASG(*pLomline->GetAt(i), *pLomline->GetAt(i+1));
	}
	rasst += DLINASG(*pLomline->GetAt(index), pt);

	return true;

}

//==================================
//!we find a point on a contour at 'rasst' distance from the beginning of a contour
bool FindLomptOnRasstFromBeginning(MPtArray* pLomline, double rasst, MPoint& pt)
//==================================		
{
	double dlina = 0;
    //we find a broken line edge index on which the required point lies
	int i = 0;
	for(i=0; i<pLomline->GetSize(); i++){
		dlina += DLINASG(*pLomline->GetAt(i), *pLomline->GetAt(i+1));

		if(dlina>=rasst)
			break;
	}
    //the entrance distance exceeds the total length of a broken line
	if(i==pLomline->GetSize())
		return false;
    //we will find distance from the beginning of the found edge on which the required point lies

	double rebrorasst = rasst - (dlina - DLINASG(*pLomline->GetAt(i), *pLomline->GetAt(i+1)));
    //we will find a required point
	pt = ExtendVector(*pLomline->GetAt(i),*pLomline->GetAt(i+1), rebrorasst);

	
	return true;
}


//==================================
//! Transformation of the broken line to an interpolation spline with the set number of points in splitting an edge of an initial broken line
/** INPUT PARAMETERS: 1) the array of points of MPtArray - an initial broken line;
//                    2) a type of a spline (will close / won't close);
//                    3) number of points in splitting an edge of an initial broken line
// OUTPUT PARAMETER: the array of points of 'MPtArray' - the resultant broken line defining a spline;
// we will consider a case of interpolation horizontals when the spline passes precisely through tops of an initial broken line
// REASONS://cubic splines of Bezier; vector tangents to Bezier's splines in the main points of a horizontal it is directed on a perpendicular to a bisector
// between two adjacent parties of a broken horizontal with the general beginning in the main top of a broken horizontal; the size of a tangent vector depends
// from length of the party of a broken horizontal to which we build a cubic spline and sizes of a angle between a tangent vector and the party of a broken horizontal;
// we use global algebraic function 'TANGV'.
*/
MPtArray* LOMTOISPL(MPtArray* ptar, unsigned char vid, int kolpts)
//==================================
{
    MPtArray* psplom = new MPtArray();// the array of points for a final broken line
	MPoint pt1, pt2, ptt; ////, ptt1;
    double cos = 1; // cos - cosine of the angle between tangent to a Bezier curve in a point of 'm_points [s]' a vector and a vector 'segment m_points[s+1]' (or 'm_points[s-1]')
	double shag = (double)1/(double)kolpts;

	double t;
    int p, q, l; // drawing parameters are chosen empirically

	p = 10;
	q = 10;

	l = 5;
	int m_nPoint = ptar->GetSize();

	if(m_nPoint == 2){
		pt2.x = ptar->GetAt(1)->x;
		pt2.y = ptar->GetAt(1)->y;
	}
	else
		pt2 = TANGV((ptar->GetAt(1))[0], (ptar->GetAt(0))[0], (ptar->GetAt(2))[0], cos, p, q, l);
	if((vid == SPV_NEZAMKNUT) || (m_nPoint == 2))
		pt1 = (ptar->GetAt(0))[0];
	else{
		pt1 = TANGV((ptar->GetAt(0))[0], (ptar->GetAt(1))[0], (ptar->GetAt(m_nPoint-2))[0], cos, p, q, l);
		cos = fabs(COS3Points((ptar->GetAt(0))[0], pt1, (ptar->GetAt(1))[0]));
		pt1 = TANGV((ptar->GetAt(0))[0], (ptar->GetAt(1))[0], (ptar->GetAt(m_nPoint-2))[0], cos, p, q, l);
		cos = fabs(COS3Points((ptar->GetAt(1))[0], pt2, (ptar->GetAt(0))[0]));
		pt2 = TANGV((ptar->GetAt(1))[0], (ptar->GetAt(0))[0], (ptar->GetAt(2))[0], cos, p, q, l);
	}
	for(t=0; t<=1; t+=shag) {
		ptt.x = (1 - t)*(1 - t)*(1 - t)*ptar->GetAt(0)->x + 3*t*(1 - t)*(1 - t)*pt1.x + 3*t*t*(1 - t)*pt2.x + t*t*t*ptar->GetAt(1)->x;
		ptt.y = (1 - t)*(1 - t)*(1 - t)*ptar->GetAt(0)->y + 3*t*(1 - t)*(1 - t)*pt1.y + 3*t*t*(1 - t)*pt2.y + t*t*t*ptar->GetAt(1)->y;
		psplom->AddTail(&ptt);
	}
    // if the edge of a broken horizontal doesn't share precisely on 'colpt' of segments,
    //we complete the last small segment
	if(t!=1+shag){
		ptt = ptar->GetAt(1);
		psplom->AddTail(&ptt);
	}
	for(int s=1; s<m_nPoint-2; s++) {
		pt1 = TANGV((ptar->GetAt(s))[0], (ptar->GetAt(s+1))[0], (ptar->GetAt(s-1))[0], cos, p, q, l);
		pt2 = TANGV((ptar->GetAt(s+1))[0], (ptar->GetAt(s))[0], (ptar->GetAt(s+2))[0], cos, p, q, l);
		cos = fabs(COS3Points((ptar->GetAt(s))[0],pt1,(ptar->GetAt(s+1))[0]));
		pt1 = TANGV((ptar->GetAt(s))[0], (ptar->GetAt(s+1))[0], (ptar->GetAt(s-1))[0], cos, p, q, l);
		cos = fabs(COS3Points((ptar->GetAt(s+1))[0], pt2, (ptar->GetAt(s-1))[0]));
		pt2 = TANGV((ptar->GetAt(s+1))[0], (ptar->GetAt(s))[0], (ptar->GetAt(s+2))[0], cos, p, q, l);
		for(t=0; t<=1; t+=shag) {
			ptt.x = (1 - t)*(1 - t)*(1 - t)*ptar->GetAt(s)->x + 3*t*(1 - t)*(1 - t)*pt1.x + 3*t*t*(1 - t)*pt2.x + t*t*t*ptar->GetAt(s+1)->x;
			ptt.y = (1 - t)*(1 - t)*(1 - t)*ptar->GetAt(s)->y + 3*t*(1 - t)*(1 - t)*pt1.y + 3*t*t*(1 - t)*pt2.y + t*t*t*ptar->GetAt(s+1)->y;
			psplom->AddTail(&ptt);
		}
        // if the edge of a broken horizontal doesn't share precisely on 'colpt' of segments,
        //we complete the last small segment
		if(t!=1+shag){
			ptt = ptar->GetAt(s+1);
			psplom->AddTail(&ptt);
		}
	}
    //  if to isolines only two points, it is carried already out
	if(m_nPoint > 2) {
		pt1 = TANGV((ptar->GetAt(m_nPoint-2))[0], (ptar->GetAt(m_nPoint-1))[0], (ptar->GetAt(m_nPoint-3))[0],cos, p, q, l);
		if(vid == SPV_NEZAMKNUT)
			pt2 = (ptar->GetAt(m_nPoint-1))[0];


		else{
			pt2 = TANGV((ptar->GetAt(0))[0],( ptar->GetAt(m_nPoint-2))[0], (ptar->GetAt(1))[0],cos, p, q, l);
			cos = fabs(COS3Points((ptar->GetAt(m_nPoint-2))[0], pt1, (ptar->GetAt(0))[0]));
			pt1 = TANGV((ptar->GetAt(m_nPoint-2))[0],( ptar->GetAt(m_nPoint-1))[0], (ptar->GetAt(m_nPoint-3))[0], cos, p, q, l);
			cos = fabs(COS3Points((ptar->GetAt(0))[0], pt2, (ptar->GetAt(m_nPoint-2))[0]));
			pt2 = TANGV((ptar->GetAt(0))[0], (ptar->GetAt(m_nPoint-2))[0], (ptar->GetAt(1))[0], cos, p, q, l);
		}
		for(t=0; t<=1; t+=shag) {
			ptt.x = (1 - t)*(1 - t)*(1 - t)*ptar->GetAt(m_nPoint-2)->x + 3*t*(1 - t)*(1 - t)*pt1.x + 3*t*t*(1 - t)*pt2.x + t*t*t*ptar->GetAt(m_nPoint-1)->x;
			ptt.y = (1 - t)*(1 - t)*(1 - t)*ptar->GetAt(m_nPoint-2)->y + 3*t*(1 - t)*(1 - t)*pt1.y + 3*t*t*(1 - t)*pt2.y + t*t*t*ptar->GetAt(m_nPoint-1)->y;
			psplom->AddTail(&ptt);
		}
        //  if the edge of a broken horizontal doesn't share precisely on 'colpt' of segments,
        //we complete the last small segment
		if(t!=1+shag){
			ptt = ptar->GetAt(m_nPoint-1);
			psplom->AddTail(&ptt);
		}
    }// end m_nPoint > 2

	return (psplom);
}
//====
QVector<MPoint> LOMTOISPL(QVector<MPoint> vector, unsigned char vid, int kolpts)
//==================================
{
    QVector<MPoint> psplom;// = new QVector<MPoint>();// the array of points for a final broken line
	MPoint pt1, pt2, ptt; ////, ptt1;
    double cos = 1; // cos - cosine of the angle between tangent to a Bezier curve in a point of 'm_points [s]' a vector and a vector m_points[s+1] (or m_points[s-1])
	double shag = (double)1/(double)kolpts;

	double t;
    int p, q, l; // drawing parameters are chosen empirically

	p = 10;
	q = 10;

	l = 5;
	int m_nPoint = vector.size();
	if(m_nPoint == 2){
		pt2.x = vector[1].x;
		pt2.y = vector[1].y;
	}
	else
		pt2 = TANGV(vector[1], vector[0], vector[2], cos, p, q, l);
	if((vid == SPV_NEZAMKNUT) || (m_nPoint == 2))
		pt1 = vector[0];
	else{
		pt1 = TANGV(vector[0], vector[1], vector[m_nPoint-2], cos, p, q, l);
		cos = fabs(COS3Points(vector[0], pt1, vector[1]));
		pt1 = TANGV(vector[0], vector[1], vector[m_nPoint-2], cos, p, q, l);
		cos = fabs(COS3Points(vector[0], pt2, vector[0]));
		pt2 = TANGV(vector[1], vector[0], vector[2], cos, p, q, l);
	}
	for(t=0; t<=1; t+=shag) {
		ptt.x = (1 - t)*(1 - t)*(1 - t)*vector[0].x + 3*t*(1 - t)*(1 - t)*pt1.x + 3*t*t*(1 - t)*pt2.x + t*t*t*vector[1].x;
		ptt.y = (1 - t)*(1 - t)*(1 - t)*vector[0].y + 3*t*(1 - t)*(1 - t)*pt1.y + 3*t*t*(1 - t)*pt2.y + t*t*t*vector[1].y;
		psplom.append(ptt);
	}
    // if the edge of a broken horizontal doesn't share precisely on 'colpt' of segments,
    //we complete the last small segment
	if(t!=1+shag){
		ptt = vector[1];
		psplom.append(ptt);
	}
	for(int s=1; s<m_nPoint-2; s++) {
		pt1 = TANGV(vector[s], vector[s+1], vector[s-1], cos, p, q, l);
		pt2 = TANGV(vector[s+1], vector[s], vector[s+2], cos, p, q, l);
		cos = fabs(COS3Points(vector[s], pt1, vector[s+1]));
		pt1 = TANGV(vector[s], vector[s+1], vector[s-1], cos, p, q, l);
		cos = fabs(COS3Points(vector[s+1], pt2, vector[s-1]));
		pt2 = TANGV(vector[s+1], vector[s], vector[s+2], cos, p, q, l);
		for (t = 0; t <= 1; t += shag) {
			ptt.x = (1 - t)*(1 - t)*(1 - t)*vector[s].x + 3*t*(1 - t)*(1 - t)*pt1.x + 3*t*t*(1 - t)*pt2.x + t*t*t*vector[s+1].x;
			ptt.y = (1 - t)*(1 - t)*(1 - t)*vector[s].y + 3*t*(1 - t)*(1 - t)*pt1.y + 3*t*t*(1 - t)*pt2.y + t*t*t*vector[s+1].y;
			psplom.append(ptt);
		}
        // if the edge of a broken horizontal doesn't share precisely on 'colpt' of segments,
        //we complete the last small segment
		if(t != 1+shag){
			ptt = vector[s+1];
			psplom.append(ptt);
		}
	}
    //  if to isolines only two points, it is carried already out
	if (m_nPoint > 2) {
		pt1 = TANGV(vector[m_nPoint-2], vector[m_nPoint-1], vector[m_nPoint-3],cos, p, q, l);
		if (vid == SPV_NEZAMKNUT)
			pt2 = vector[m_nPoint-1];
		else{
			pt2 = TANGV(vector[0], vector[m_nPoint-2], vector[1],cos, p, q, l);
			cos = fabs(COS3Points(vector[m_nPoint-2], pt1, vector[0]));
			pt1 = TANGV(vector[m_nPoint-2],vector[m_nPoint-1], vector[m_nPoint-3], cos, p, q, l);
			cos = fabs(COS3Points(vector[0], pt2, vector[m_nPoint-2]));
			pt2 = TANGV(vector[0], vector[m_nPoint-2], vector[1], cos, p, q, l);
		}
		for(t=0; t<=1; t+=shag) {
			ptt.x = (1 - t)*(1 - t)*(1 - t)*vector[m_nPoint-2].x + 3*t*(1 - t)*(1 - t)*pt1.x + 3*t*t*(1 - t)*pt2.x + t*t*t*vector[m_nPoint-1].x;
			ptt.y = (1 - t)*(1 - t)*(1 - t)*vector[m_nPoint-2].y + 3*t*(1 - t)*(1 - t)*pt1.y + 3*t*t*(1 - t)*pt2.y + t*t*t*vector[m_nPoint-1].y;
			psplom.append(ptt);
		}
        // if the edge of a broken horizontal doesn't share precisely on 'colpt' of segments,
        //we complete the last small segment
		if(t!=1+shag){
			ptt = vector[m_nPoint-1];
			psplom.append(ptt);
		}
    }// end m_nPoint > 2

	return (psplom);
}

//==================================
//! Transformation of the broken line to an approximating spline with the set weight coefficients
//! and the set number of points in splitting an edge of an initial broken line
/** INPUT PARAMETERS: 1) the array of points of 'MPtArray' - an initial broken line;
//2) a type of a spline (will close / won't close);
//3) array of weight coefficients of 'double *';
//4) number of points in splitting an edge of an initial broken line
// OUTPUT PARAMETER: the array of points of 'MPtArray' - the resultant broken line defining a spline;
//we will consider a case of approximating horizontals when the spline not necessarily passes through tops of an initial broken line
//all theory is taken from the book Shikin E.V., A.I Cotton velvet. "Curves and surfaces on the computer screen. The guide to splines for users."
// REASONS: to smooth a broken horizontal of an approximating curve, it is necessary to solve 'm_nPoint' system of the linear equations from the symmetric
//'A' matrix for determination of coefficients of 'ni' (i=0..., m_nPoint-1); then on coefficients 'ni' and weight coefficients 'ri'
//(we believe for any 'i' 'ri=r' below) we define coefficients 'zi' and further by means of all coefficients we build an approximating spline;
//system of the linear equations we solve Haletsky's by method (detailed algorithm - see the global 'HLINSYST' function (double * pA, double * utvector, int n));
//coefficients of a symmetric matrix 'A' it is stored a look (a00, a01, a02, a11, a12, a13..., an-2n-2, an-2n-1, an-2n, an-1n-1, an-1n, ann).
// REMARK: length of the array of weight coefficients is equal in a case of not closed spline or is one unit less in case of the closed spline of number of tops of the initial
//broken line; equality to zero any weight coefficient means that the spline precisely passes through the corresponding top of an initial broken line
*/
MPtArray* LOMTOASPL(MPtArray* ptar, unsigned char vid, double* vesarray, int kolpts)
//==================================		
{
    // we will build parametrical splines
    //we enter parametrization on a segment [0, m_nPoint-1];we will build on a segment [0, m_nPoint-1] cubic approximating splines x (t), y (t);
    //we break a segment [0, m_nPoint-1] into 'm_nPoint-1' of equal parts (0, 1, 2..., m_nPoint-1 - splitting points);
    //at such parametrization all formulas become simpler and assume an air - see in the text of the program lower;
    //filling of a matrix 'A' and a vector column of free members for the decision of system  of the linear equations for determination of 'ni' values
    MPtArray* psplom = new MPtArray();// the array of points for a final broken line
	double* pA =NULL;
	double* vectorx =NULL;
	double* vectory =NULL;
////	int i = 0;

	int m_nPoint = ptar->GetSize();
	int nn = m_nPoint - 1;
			
	if(vid == SPV_ZAMKNUT)
	{
		if(nn<=2){
			psplom->AppendTail(ptar);
			return psplom;
		}
		if(nn>4){
			pA = (double*)malloc(sizeof(double)*3*nn);
			vectorx = (double*)malloc(sizeof(double)*(m_nPoint-1));
			vectory = (double*)malloc(sizeof(double)*(m_nPoint-1));

			vectorx[0] = ptar->GetAt(2)->x - 2* ptar->GetAt(1)->x + ptar->GetAt(0)->x;
			vectory[0] = ptar->GetAt(2)->y - 2*ptar->GetAt(1)->y + ptar->GetAt(0)->y;
			vectorx[1] = ptar->GetAt(3)->x - 2* ptar->GetAt(2)->x + ptar->GetAt(1)->x;
			vectory[1] = ptar->GetAt(3)->y - 2*ptar->GetAt(2)->y + ptar->GetAt(1)->y;
			int i = 0;
			for(i=3; i<m_nPoint-2; i++){
				vectorx[i-1] = ptar->GetAt(i+1)->x - 2* ptar->GetAt(i)->x + ptar->GetAt(i-1)->x;
				vectory[i-1] = ptar->GetAt(i+1)->y - 2*ptar->GetAt(i)->y + ptar->GetAt(i-1)->y;
			}
			vectorx[m_nPoint-3] = ptar->GetAt(m_nPoint-1)->x - 2*ptar->GetAt(m_nPoint-2)->x + ptar->GetAt(m_nPoint-3)->x;
			vectory[m_nPoint-3] = ptar->GetAt(m_nPoint-1)->y - 2*ptar->GetAt(m_nPoint-2)->y + ptar->GetAt(m_nPoint-3)->y;
			vectorx[m_nPoint-2] = ptar->GetAt(1)->x - 2*ptar->GetAt(m_nPoint-1)->x + ptar->GetAt(m_nPoint-2)->x;

			vectory[m_nPoint-2] = ptar->GetAt(1)->y - 2*ptar->GetAt(m_nPoint-1)->y + ptar->GetAt(m_nPoint-2)->y;
	
			pA[0] = (double)2/(double)3 + vesarray[m_nPoint-2] + 4*vesarray[0] + vesarray[1];
			pA[1] = (double)1/(double)6 - 2*vesarray[0] - 2*vesarray[1];
			pA[2] = vesarray[1];
			pA[3] = vesarray[m_nPoint-2];
			pA[4] = (double)1/(double)6 - 2*vesarray[m_nPoint-2] - 2*vesarray[0];
			pA[5] = (double)2/(double)3 + vesarray[0] + 4*vesarray[1] + vesarray[2];
			pA[6] = (double)1/(double)6 - 2*vesarray[1] - 2*vesarray[2];
			pA[7] = vesarray[2];
			pA[8] = vesarray[0];

			for(i=3; i<m_nPoint-2; i++){
				pA[3*(i-1)+3] =(double)2/(double)3 + vesarray[i-2] + 4*vesarray[i-1] + vesarray[i];
				pA[3*(i-1)+4] = (double)1/(double)6 - 2*vesarray[i-1] - 2*vesarray[i];
				pA[3*(i-1)+5] = vesarray[i];
			}
			pA[3*(m_nPoint-3)+3] = (double)2/(double)3 + vesarray[m_nPoint-5] + 4*vesarray[m_nPoint-4] + vesarray[m_nPoint-3];
			pA[3*(m_nPoint-3)+4] = (double)1/(double)6 - 2*vesarray[m_nPoint-4] - 2*vesarray[m_nPoint-3];
			pA[3*(m_nPoint-2)+2] = (double)2/(double)3 + vesarray[m_nPoint-4] + 4*vesarray[m_nPoint-3] + vesarray[m_nPoint-2];;
        } // end if(nn>4)

		else if(nn==4){
			pA = (double*)malloc(sizeof(double)*10);
			vectorx = (double*)malloc(sizeof(double)*(m_nPoint-1));
			vectory = (double*)malloc(sizeof(double)*(m_nPoint-1));
			vectorx[0] = ptar->GetAt(2)->x - 2* ptar->GetAt(1)->x + ptar->GetAt(0)->x;
			vectory[0] = ptar->GetAt(2)->y - 2*ptar->GetAt(1)->y + ptar->GetAt(0)->y;
			vectorx[1] = ptar->GetAt(3)->x - 2* ptar->GetAt(2)->x + ptar->GetAt(1)->x;
			vectory[1] = ptar->GetAt(3)->y - 2*ptar->GetAt(2)->y + ptar->GetAt(1)->y;
			vectorx[2] = ptar->GetAt(m_nPoint-1)->x - 2*ptar->GetAt(m_nPoint-2)->x + ptar->GetAt(m_nPoint-3)->x;
			vectory[2] = ptar->GetAt(m_nPoint-1)->y - 2*ptar->GetAt(m_nPoint-2)->y + ptar->GetAt(m_nPoint-3)->y;
			vectorx[3] = ptar->GetAt(1)->x - 2*ptar->GetAt(m_nPoint-1)->x + ptar->GetAt(m_nPoint-2)->x;
			vectory[3] = ptar->GetAt(1)->y - 2*ptar->GetAt(m_nPoint-1)->y + ptar->GetAt(m_nPoint-2)->y;

			pA[0] = (double)2/(double)3 + vesarray[3] + 4*vesarray[0] + vesarray[1];
			pA[1] = (double)1/(double)6 - 2*vesarray[0] - 2*vesarray[1];
			pA[2] = vesarray[1] + vesarray[3];
			pA[3] = (double)1/(double)6 - 2*vesarray[3] - 2*vesarray[0];
			pA[4] = (double)2/(double)3 + vesarray[0] + 4*vesarray[1] + vesarray[2];

			pA[5] = (double)1/(double)6 - 2*vesarray[1] - 2*vesarray[2];
			pA[6] = vesarray[2] + vesarray[0];
			pA[7] = (double)2/(double)3 + vesarray[1] + 4*vesarray[2] + vesarray[3];
			pA[8] = (double)1/(double)6 - 2*vesarray[2] - 2*vesarray[3];
			pA[9] = (double)2/(double)3 + vesarray[2] + 4*vesarray[3] + vesarray[0];


        } // end else if(nn==4)

		else if(nn==3){
			pA = (double*)malloc(sizeof(double)*6);
			vectorx = (double*)malloc(sizeof(double)*(m_nPoint-1));
			vectory = (double*)malloc(sizeof(double)*(m_nPoint-1));
			vectorx[0] = ptar->GetAt(2)->x - 2* ptar->GetAt(1)->x + ptar->GetAt(0)->x;
			vectory[0] = ptar->GetAt(2)->y - 2*ptar->GetAt(1)->y + ptar->GetAt(0)->y;
			vectorx[1] = ptar->GetAt(3)->x - 2* ptar->GetAt(2)->x + ptar->GetAt(1)->x;
			vectory[1] = ptar->GetAt(3)->y - 2*ptar->GetAt(2)->y + ptar->GetAt(1)->y;
			vectorx[2] = ptar->GetAt(1)->x - 2*ptar->GetAt(m_nPoint-1)->x + ptar->GetAt(m_nPoint-2)->x;
			vectory[2] = ptar->GetAt(1)->y - 2*ptar->GetAt(m_nPoint-1)->y + ptar->GetAt(m_nPoint-2)->y;

			pA[0] = (double)2/(double)3 + vesarray[2] + 4*vesarray[0] + vesarray[1];
			pA[1] = (double)1/(double)6 - 2*vesarray[0] - 2*vesarray[1] + vesarray[2];
			pA[2] = (double)1/(double)6 - 2*vesarray[2] - 2*vesarray[0] + vesarray[1];
			pA[3] = (double)2/(double)3 + vesarray[0] + 4*vesarray[1] + vesarray[2];

			pA[4] = (double)1/(double)6 - 2*vesarray[1] - 2*vesarray[2] +vesarray[0];
			pA[5] = (double)2/(double)3 + vesarray[1] + 4*vesarray[2] + vesarray[0];
        } // end else if(nn==3)

        // we receive the decision of linear system - 'ni'
		double* pLx;
		double* pLy;
		pLx = HLINSYST3(pA, vectorx, nn);
		pLy = HLINSYST3(pA, vectory, nn);
		double* pNx;
		double* pNy;
		pNx = (double*)malloc(sizeof(double)*m_nPoint);

		pNy = (double*)malloc(sizeof(double)*m_nPoint);
		pNx[0] = pLx[m_nPoint-2];	
		pNy[0] = pLy[m_nPoint-2];
		int i = 0;
		for(i=1; i<m_nPoint; i++){
			pNx[i] = pLx[i-1];
			pNy[i] = pLy[i-1];
		}
		free(pLx);
		free(pLy);
		double* pDx;
		double* pDy;
		pDx = (double*)malloc(sizeof(double)*m_nPoint);
		pDy = (double*)malloc(sizeof(double)*m_nPoint);
        pDx[0] = pDx[m_nPoint-1] = pNx[1] - 2*pNx[m_nPoint-1] + pNx[m_nPoint-2]; // in a case the curve passes through a point 0, isn't necessary
        pDy[0] = pDy[m_nPoint-1] = pNy[1] - 2*pNy[m_nPoint-1] + pNy[m_nPoint-2]; // in a case the curve passes through a point 0, isn't necessary
		for(i=1; i<m_nPoint-1; i++){
			pDx[i] = pNx[i+1] - 2*pNx[i] + pNx[i-1];
			pDy[i] = pNy[i+1] - 2*pNy[i] + pNy[i-1];
		}

        // now it is necessary to determine the sizes 'zi'
		double* pZx;
		double* pZy;
		pZx = (double*)malloc(sizeof(double)*m_nPoint);
		pZy = (double*)malloc(sizeof(double)*m_nPoint);

        // we build an interpolation curve in a point 0 - the curve passes through a point 0
//		pZx[0] = ptar->GetAt(0)->x;
//		pZx[m_nPoint-1] = ptar->GetAt(m_nPoint-1)->x;
        // we build an approximating curve in a point 0 - the curve doesn't pass through a point 0
		pZx[0] = ptar->GetAt(0)->x - vesarray[m_nPoint-2]*pDx[0];
		pZx[m_nPoint-1] = ptar->GetAt(m_nPoint-1)->x - vesarray[m_nPoint-2]*pDx[m_nPoint-1];
        // we build an interpolation curve in a point 0 - the curve passes through a point 0
//		pZy[0] = ptar->GetAt(0)->y;
//		pZy[m_nPoint-1] = ptar->GetAt(m_nPoint-1)->y;
        // we build an approximating curve in a point 0 - the curve doesn't pass through a point 0
		pZy[0] = ptar->GetAt(0)->y - vesarray[m_nPoint-2]*pDy[0];
		pZy[m_nPoint-1] = ptar->GetAt(m_nPoint-1)->y - vesarray[m_nPoint-2]*pDy[m_nPoint-1];
		for(i=1; i<m_nPoint-1; i++){
			pZx[i] = ptar->GetAt(i)->x - vesarray[i-1]*pDx[i];

			pZy[i] = ptar->GetAt(i)->y - vesarray[i-1]*pDy[i];
		}

        // now we will calculate the array of points for a required broken line - an approximating spline
        //we receive points from formulas for approximating splines and the calculated 'ni' and 'zi' parameters
		double dist, t;
		MPoint pt;
		pt.x = pZx[0];
		pt.y = pZy[0];
		psplom->AddTail(&pt);
		for(i=0; i<m_nPoint-1; i++) {
			dist = (double)1/(double)kolpts;
			for(int j=1; j<kolpts + 1; j++) {
				double x = i + dist*j;
				t = x - i;
				pt.x = pZx[i]*(1 - t) + pZx[i+1]*t - (double)1/(double)6*t*(1 - t)*((2 - t)*pNx[i] + (1 + t)*pNx[i+1]);
				pt.y = pZy[i]*(1 - t) + pZy[i+1]*t - (double)1/(double)6*t*(1 - t)*((2 - t)*pNy[i] + (1 + t)*pNy[i+1]);

				psplom->AddTail(&pt);
			}
		}
		free(pDx);
		free(pZx);
		free(pNx);
		free(pDy);
		free(pZy);
		free(pNy);

	}

    // case when we build not closed spline...
	else if(vid == SPV_NEZAMKNUT){
		if(nn<=1) return psplom;
		if(nn>3){
			pA = (double*)malloc(sizeof(double)*3*nn);
			vectorx = (double*)malloc(sizeof(double)*m_nPoint);
			vectory = (double*)malloc(sizeof(double)*m_nPoint);
            // boundary conditions of the first type
			vectorx[0] = 0;
			vectory[0] = 0;
			int i = 0;
			for(i=1; i<m_nPoint-3; i++){
				vectorx[i] = ptar->GetAt(i+1)->x - 2* ptar->GetAt(i)->x + ptar->GetAt(i-1)->x;
				vectory[i] = ptar->GetAt(i+1)->y - 2*ptar->GetAt(i)->y + ptar->GetAt(i-1)->y;
			}
			vectorx[m_nPoint-3] = ptar->GetAt(m_nPoint-2)->x - 2*ptar->GetAt(m_nPoint-3)->x + ptar->GetAt(m_nPoint-4)->x;
			vectory[m_nPoint-3] = ptar->GetAt(m_nPoint-2)->y - 2*ptar->GetAt(m_nPoint-3)->y + ptar->GetAt(m_nPoint-4)->y;
			vectorx[m_nPoint-2] = ptar->GetAt(m_nPoint-1)->x - 2*ptar->GetAt(m_nPoint-2)->x + ptar->GetAt(m_nPoint-3)->x;
			vectory[m_nPoint-2] = ptar->GetAt(m_nPoint-1)->y - 2*ptar->GetAt(m_nPoint-2)->y + ptar->GetAt(m_nPoint-3)->y;
			vectorx[m_nPoint-1] = 0;
			vectory[m_nPoint-1] = 0;
			pA[0] = (double)1/(double)3 + vesarray[1];
			pA[1] = (double)1/(double)6 - 2*vesarray[1];
			pA[2] = vesarray[1];
			pA[3*nn-1] = (double)1/(double)3 + vesarray[m_nPoint-2];
			pA[3*nn-2] = (double)1/(double)6 - 2*vesarray[m_nPoint-3];
			pA[3*nn-4] = vesarray[m_nPoint-2];
					
			for(i=1; i<m_nPoint-3; i++){
				pA[3*i] =(double)2/(double)3 + vesarray[i-1] + 4*vesarray[i] + vesarray[i+1];
				pA[3*i+1] = (double)1/(double)6 - 2*vesarray[i] - 2*vesarray[i+1];
				pA[3*i+2] = vesarray[i+1];
			}
			pA[3*(m_nPoint-3)] = (double)2/(double)3 + vesarray[m_nPoint-4] + 4*vesarray[m_nPoint-3] + vesarray[m_nPoint-2];
			pA[3*(m_nPoint-3)+1] = (double)1/(double)6 - 2*vesarray[m_nPoint-3] - 2*vesarray[m_nPoint-2];
			pA[3*(m_nPoint-2)] = (double)2/(double)3 + vesarray[m_nPoint-3] + 4*vesarray[m_nPoint-2];
		}
		if(nn==3){
			pA = (double*)malloc(sizeof(double)*9);
			vectorx = (double*)malloc(sizeof(double)*m_nPoint);
			vectory = (double*)malloc(sizeof(double)*m_nPoint);
			vectorx[0] = 0;
			vectory[0] = 0;
			vectorx[1] = ptar->GetAt(2)->x - 2*ptar->GetAt(1)->x + ptar->GetAt(0)->x;
			vectory[1] = ptar->GetAt(2)->y - 2*ptar->GetAt(1)->y + ptar->GetAt(0)->y;


			vectorx[2] = ptar->GetAt(3)->x - 2*ptar->GetAt(2)->x + ptar->GetAt(1)->x;
			vectory[2] = ptar->GetAt(3)->y - 2*ptar->GetAt(2)->y + ptar->GetAt(1)->y;
			vectorx[3] = 0;
			vectory[3] = 0;
			pA[0] = (double)1/(double)3 + vesarray[1];
			pA[1] = (double)1/(double)6 - 2*vesarray[1];
			pA[2] = vesarray[1];
			pA[3] = (double)2/(double)3 + 4*vesarray[1] + vesarray[2];
			pA[4] = (double)1/(double)6 - 2*vesarray[1] - 2*vesarray[2];
			pA[5] = vesarray[2];
			pA[6] = (double)2/(double)3 + vesarray[1] + 4*vesarray[2];
			pA[7] = (double)1/(double)6 - 2*vesarray[1];
			pA[8] = (double)1/(double)3 + vesarray[2];
		}

		if(nn==2){
			pA = (double*)malloc(sizeof(double)*6);
			vectorx = (double*)malloc(sizeof(double)*m_nPoint);
			vectory = (double*)malloc(sizeof(double)*m_nPoint);
			vectorx[0] = 0;
			vectory[0] = 0;
			vectorx[1] = ptar->GetAt(2)->x - 2*ptar->GetAt(1)->x + ptar->GetAt(0)->x;
			vectory[1] = ptar->GetAt(2)->y - 2*ptar->GetAt(1)->y + ptar->GetAt(0)->y;
			vectorx[2] = 0;

			vectory[2] = 0;
			pA[0] = (double)1/(double)3 + vesarray[1];
			pA[1] = (double)1/(double)6 - 2*vesarray[1];
			pA[2] = vesarray[1];

			pA[3] = (double)2/(double)3 + 4*vesarray[1];
			pA[4] = (double)1/(double)6 - 2*vesarray[1];

			pA[5] = (double)1/(double)3 + vesarray[1];
		}

        // we receive the decision of linear system - 'ni'
		double* pNx;
		double* pNy;
		pNx = HLINSYST(pA, vectorx, nn);
		pNy = HLINSYST(pA, vectory, nn);
		double* pDx;
		double* pDy;
		pDx = (double*)malloc(sizeof(double)*m_nPoint);
		pDy = (double*)malloc(sizeof(double)*m_nPoint);
		pDx[0] = pDx[m_nPoint-1] = 0;
		pDy[0] = pDy[m_nPoint-1] = 0;
		int i = 0;
		for(i=1; i<m_nPoint-1; i++){
			pDx[i] = pNx[i+1] - 2*pNx[i] + pNx[i-1];
			pDy[i] = pNy[i+1] - 2*pNy[i] + pNy[i-1];
		}

        // now it is necessary to determine the sizes 'zi'
		double* pZx;
		double* pZy;
		pZx = (double*)malloc(sizeof(double)*m_nPoint);
		pZy = (double*)malloc(sizeof(double)*m_nPoint);
		pZx[0] = ptar->GetAt(0)->x;
		pZy[0] = ptar->GetAt(0)->y;
		for(i=1; i<m_nPoint-1; i++){
			pZx[i] = ptar->GetAt(i)->x - vesarray[i]*pDx[i];
			pZy[i] = ptar->GetAt(i)->y - vesarray[i]*pDy[i];

		}
		pZx[m_nPoint-1] = ptar->GetAt(m_nPoint-1)->x;
		pZy[m_nPoint-1] = ptar->GetAt(m_nPoint-1)->y;

        // we will build approximating splines, proceeding from formulas now for
        //approximating splines and the calculated 'ni' and 'zi' parameters
		double t, dist;
		MPoint pt;
		pt.x = pZx[0];
		pt.y = pZy[0];
		psplom->AddTail(&pt);
		for(i=0; i<m_nPoint-1; i++) {
			dist = (double)1/(double)kolpts;
			for(int j=1; j<kolpts + 1; j++) {
				double x = i + dist*j;
				t = x - i;
				pt.x = pZx[i]*(1 - t) + pZx[i+1]*t - (double)1/(double)6*t*(1 - t)*((2 - t)*pNx[i] + (1 + t)*pNx[i+1]);
				pt.y = pZy[i]*(1 - t) + pZy[i+1]*t - (double)1/(double)6*t*(1 - t)*((2 - t)*pNy[i] + (1 + t)*pNy[i+1]);
				psplom->AddTail(&pt);
			}
		}
		free(pZx);
		free(pNx);
		free(pZy);
		free(pNy);
	}
	free(vectorx);
	free(vectory);
	free(pA);
	

	return (psplom);
}


//=====================================================================
//! To give out coordinates of the point lying on a straight line
//! the perpendicular straight line set by a vector of 'pt1', 'pt2'
//! 'pt3' passing through a point.
/**  The resultant point has to lie at d distance from a straight line [pt1, pt2] and
//to lie at the left,if d>0, or on the right, if d<0
*/
MPoint GetPerpendikular(MPoint pt1,MPoint pt2,MPoint pt3,double d)
//===============================================================
{
			
            //we determine vector coordinates
		    double xr=pt2.x-pt1.x;
			double yr=pt2.y-pt1.y;

			MPoint pt4;


            // if the vector is parallel to OX axis

			if(fabs(yr)<0.001)
			{
				pt4.x=pt3.x;
				pt4.y=pt3.y+d*xr/fabs(xr);
			}
			else
			{
                //we define a point, as the decision of system of the equations
                // let 'r1' - a vector 'pt1,pt2'
                // let 'r2' - a vector (0,0),(x,y)
                // then 'x', 'y' are from system of the equations:
                // r1x*r2x+r1y*r2y=0 -  they are perpendicular
                // r1x*r2y-r1y*r2x=d - vector work

				double r=sqrt(xr*xr+yr*yr);
				pt4.x=-d*yr/r+pt3.x;
				pt4.y=d*xr/r+pt3.y;
			}

			return pt4;
}


//=====================================================================
//! To give out coordinates of the point lying on a straight line
//! the perpendicular  straight line set by a vector of 'pt1, pt2'
//! 'pt2' passing through a point and lying on the same party
//! and at the same distance from a straight line, as 'pt3' point
MPoint GetPerpComponents(MPoint pt1,MPoint pt2,MPoint pt3)
//===============================================================
{

    // if 'pt1' and 'pt2' coincide, or 'pt2' and 'pt3' coincide,
    //we return as the answer of 'p2'
	if ((pt1==pt2)||(pt2==pt3)) return pt2;
			

    //we determine coordinates of a single vector from 'pt1' to 'pt2'
    double xr=pt2.x-pt1.x;
	double yr=pt2.y-pt1.y;

	
	if(COS2Vector(pt1,pt2,pt2,pt3)<0) {
		xr = -xr;
		yr = -yr;
	}

	double rn = fabs(RASSTPTL(pt3, pt1, pt2));
	double rd = sqrt(((pt3.x - pt2.x)*(pt3.x - pt2.x)+(pt3.y - pt2.y)*(pt3.y - pt2.y))-rn*rn);

	MPoint pt4;

	pt4.x = pt3.x - xr*rd/sqrt(xr*xr + yr*yr);
	pt4.y = pt3.y - yr*rd/sqrt(xr*xr + yr*yr);

	return pt4;
}

//=====================================================================
//! To give out 'pt3' point point projection coordinates on a straight line,
//! set by a vector 'pt1pt2',
MPoint GetProdComponents(MPoint pt1,MPoint pt2,MPoint pt3)
//===============================================================
{

    // if 'pt1' and 'pt2' coincide, or 'pt2' and 'pt3' coincide,
    //we return as the answer of 'p2'
	if ((pt1==pt2)||(pt2==pt3)) return pt2;
			
    //we determine coordinates of a single vector from 'pt1' to 'pt2'
    double xr=pt2.x-pt1.x;
	double yr=pt2.y-pt1.y;

	
	if(COS2Vector(pt1,pt2,pt2,pt3)<0) {
		xr = -xr;
		yr = -yr;
	}

	double rn = fabs(RASSTPTL(pt3, pt1, pt2));
	double rd = sqrt(((pt3.x - pt2.x)*(pt3.x - pt2.x)+(pt3.y - pt2.y)*(pt3.y - pt2.y))-rn*rn);

	MPoint pt4;

	pt4.x = pt2.x + xr*rd/sqrt(xr*xr + yr*yr);
	pt4.y = pt2.y + yr*rd/sqrt(xr*xr + yr*yr);

	return pt4;
}


//=====================================================================
//!To give out the equation of a straight line of a type of Ax+By+C=0 parallel to a vector (to pt1, pt2),
//!passing at distance 'd' from it and lying at the left if  d>0, or on the right if d<0
void GetPramuju(MPoint pt1,MPoint pt2,double d,double& A,double &B, double& C)
//===============================================================
{
	
            //we define 'A','B' (at parallel straight lines they are equal respectively)
			A=pt2.y-pt1.y;
			B=pt1.x-pt2.x;
			
			
			
            //we determine vector coordinates
			double xr=pt2.x-pt1.x;
			double yr=pt2.y-pt1.y;

			MPoint pt3;

            //if the vector is parallel to OX axis
			if(fabs(yr)<0.001)
			{
				pt3.x=pt2.x;
				pt3.y=pt2.y+d*xr/fabs(xr);
			}
			else
			{
                //we define a point, as the decision of system of the equations
                //let r1 - a vector of pt1, pt2
                //let r2 - a vector (0,0), (x, y)
                //then x, y are from system of the equations:
                //r1x*r2x+r1y*r2y=0 - they are perpendicular
                //r1x*r2y-r1y*r2x=d - vector work
                //this point lies on a required straight line

				double r=sqrt(xr*xr+yr*yr);
				pt3.x=-d*yr/r+pt2.x;
				pt3.y=d*xr/r+pt2.y;

			}
            //we define the free member
			C=-(A*pt3.x+B*pt3.y);
}


//====================================================
//!Returns an angle between a vector and an axis OX in degrees
double GetVektorGrad(MPoint* pt1,MPoint* pt2)
//====================================================
{
	if(pt1==pt2)
		return 0;
    //we determine vector length
	double dlina=sqrt((pt2->x-pt1->x)*(pt2->x-pt1->x)+(pt2->y-pt1->y)*(pt2->y-pt1->y));
	if(dlina==0)return 0;
    //we define a sine and a cosine of the angle
	double X=pt2->x-pt1->x;
	double Y=pt2->y-pt1->y;
	double cs=X/dlina;
	double ss=Y/dlina;
    //if the sine is less than zero, the angle defined from an arccosine will be symmetric relatively 0
	double angle=acos(cs);
	if(ss<0)
	{
		angle=-angle;
	}
    //we transfer to degrees
	angle=angle/pi*180;
    //normalize
	while(angle<0)angle+=360;
	while(angle>=360)angle-=360;
	return angle;

}


//!Returns a perpendicular point of intersection,
//! lowered from a point on a segment and a segment.
/**the segment (pt1, pt2) and 'pt3' point is set.
//If the point doesn't lie on a segment,
//that comes back the closest end of a segment
*/
MPoint GetPointOnOtrezok(MPoint pt1,MPoint pt2,MPoint pt3)
{
	MPoint pt;
	if((pt1.x==pt2.x)&&(pt1.y==pt2.y))return pt1;
	double A1,A2,B1,B2,C1,C2,d,dx,dy,t;
    //we build the equations of direct (pt1-pt2) and a perpendicular through 'pt3'
	A1=pt1.y-pt2.y;
	B1=pt2.x-pt1.x;
	C1=pt1.x*pt2.y-pt2.x*pt1.y;
	A2=-B1;
	B2=A1;
	C2=B1*pt3.x-A1*pt3.y;

    //we find their crossing
	d=A1*B2-A2*B1;
	dx=B1*C2-B2*C1;
	dy=A2*C1-A1*C2;


	pt.x=dx/d;
	pt.y=dy/d;

    //we present direct pt1-pt2 in the parametrical form
	A1=pt2.x-pt1.x;
	A2=pt2.y-pt1.y;
	B1=pt1.x;
	B2=pt1.y;

    //we find the parameter of a point of intersection
	if(fabs(A1)<0.0000001)
	{
		t=(pt.y-B2)/A2;
	}
	else
	{
		t=(pt.x-B1)/A1;
	}
    //if parameter goes beyond a segment, we find the next end
	if(t<0)pt=pt1;
	else if(t>1) pt=pt2;
	
	return pt;
}



//! Calculates a straight line point of intersection with a segment.
/** The segment is set by points of 'pt1' and 'pt2', and a straight line - points of 'pt3' and 'pt4'.

// Calculates a point of intersection of two straight lines, the calculated point can be found in 'pt5'.
//The distance from 'pt4' to a point of intersection is in addition calculated (registers in rassto).
//The returned values:
//0 - the point of intersection lies on a segment (pt1, pt2):
//1 - the point of intersection doesn't lie on a segment (pt1, pt2);
//2 - straight lines aren't crossed;
//3 - straight lines coincide;
//4 - a segment incorrect, the beginning and the end - in one point, and the straight line doesn't pass through this point
//5 - a segment incorrect, the beginning and the end - in one point, but the straight line passes through this point
//6 - the straight line is set incorrectly, both points of 'pt3' and 'pt4' coincide and this point doesn't lie on a segment
//7 - the straight line is set incorrectly, both points of 'pt3' and 'pt4' coincide but this point lies on a segment
//8 - the straight line is set incorrectly, and the segment is set incorrectly
//9 - the straight line is set incorrectly, and the segment is set incorrectly, but crossing (by miracle!) it can be found correctly since all four (!) points coincided
// 10 - the first point from points by which the straight line is set, belongs to a segment
//11 - the second point from points by which the straight line is set, belongs to a segment

*/
int IntersectLineWithOtrezok(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint* pt5,double& rassto,double delta)
{
////	MPoint pt;
    // if the straight line is set incorrectly, and the segment is set incorrectly
	if((pt1==pt2)&&(pt3==pt4)) {
        // all four points coincided
		if(pt2==pt3) {
			pt5[0] = pt1;
			rassto = 0;
			return 9;
		}
		else return 8;
	}

	if(PTONSEG(pt3, pt1, pt2)){
		pt5[0] = pt3;
		rassto = DLINASG(pt3, pt4);
		return 10;
	}

	if(PTONSEG(pt4, pt1, pt2)){
		pt5[0] = pt4;
		rassto = 0;
		return 11;
	}

    //we build the equations of straight lines
	double A1,A2,B1,B2,C1,C2,d,dx,dy,t;
	A1=pt1.y-pt2.y;
	B1=pt2.x-pt1.x;
	C1=pt1.x*pt2.y-pt2.x*pt1.y;
	A2=pt3.y-pt4.y;
	B2=pt4.x-pt3.x;
	C2=pt3.x*pt4.y-pt4.x*pt3.y;

    // if the segment is set incorrectly
	if(pt1==pt2) {
        //the straight line passes through this point

		if((A2*pt1.x + B2*pt1.y + C2)==0) {
			pt5[0] = pt1;
			rassto = pt5->Distance(&pt4);

			return 5;
		}
		else return 4;
	}

    // if the straight line is set incorrectly
	if(pt3==pt4) {
		double scalarpr = (pt3.x - pt1.x)*(pt3.x - pt2.x) + (pt3.y - pt1.y)*(pt3.y - pt2.y);
        //the segment contains this point
		if(((A1*pt3.x + B1*pt3.y + C1)==0)&&(scalarpr<0)) {
			pt5[0] = pt3;
			rassto = 0;
			return 7;
		}
		else return 6;
	}

    // straight lines are parallel
	if((A1==A2)&&(B1==B2)) {
        // straight lines coincide
		if(C1==C2) return 3;
		else return 2;

	}

    //we find their crossing
	d=A1*B2-A2*B1;
	dx=B1*C2-B2*C1;
	dy=A2*C1-A1*C2;


	pt5->x=dx/d;
	pt5->y=dy/d;

    //we present direct pt1-pt2 in the parametrical form
	A1=pt2.x-pt1.x;
	A2=pt2.y-pt1.y;
	B1=pt1.x;
	B2=pt1.y;

    //we find the parameter of a point of intersection
    if(fabs(A1)<delta)
	{
		t=(pt5->y-B2)/A2;
	}
	else
	{
		t=(pt5->x-B1)/A1;
	}
    //if parameter goes beyond a segment, we find the next end
    if((t<delta)||(t>1+delta)) return 1;
	else {
		rassto = pt5->Distance(&pt4);
		return 0;

	}
}

//====================================================
//!Returns coordinates of a point remote from this point on the set distance
//! in the set direction.
/**The direction is set by two points: pt1 - the beginning of a vector
//pt2 - the end of a vector.
*/
MPoint ExtendVector(MPoint pt1,MPoint pt2, double d)
//====================================================
{
    //We will determine length of the directing vector
	double dlina = sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y));

    //if the direction isn't defined, we don't shift a point
	if(dlina==0)
		return pt1;

    //We will determine coordinates of the end of the "stretched" vector
	MPoint pt;
	pt.x = pt1.x + d*(pt2.x - pt1.x)/dlina;
	pt.y = pt1.y + d*(pt2.y - pt1.y)/dlina;
	return pt;
}

//==================================
//!Determination of coordinates of the single vector lying on a angle bisektrissa
//! between two set vectors with the general beginning in 'pt1' point.
/**GENERAL IDEA: we normalize the set two vectors, we take their sum, we normalize and we consider transfer
//the beginnings of coordinates in 'pt1' point is and there will be a required vector;
//the single vector lying on a angle bisektrissa is directed in a angle, smaller, than developed
*/
MPoint GetBisectrissa(MPoint pt1, MPoint pt2, MPoint pt3)
//==================================
{ 
	MPoint pt;
	pt.x = (pt2.x - pt1.x)/sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)) + (pt3.x - pt1.x)/sqrt((pt3.x - pt1.x)*(pt3.x - pt1.x) + (pt3.y - pt1.y)*(pt3.y - pt1.y));
	pt.y = (pt2.y - pt1.y)/sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y)) + (pt3.y - pt1.y)/sqrt((pt3.x - pt1.x)*(pt3.x - pt1.x) + (pt3.y - pt1.y)*(pt3.y - pt1.y));
    //if two vectors aren't opposite directed
	if((pt.x!=0)&&(pt.y!=0)){
        double distpt = sqrt(pt.x * pt.x + pt.y * pt.y); // distpt - length of a vector-sum of two normalized vectors (including the beginning of coordinates in 'pt1' point)
        //we normalize a vector
		pt.x = pt.x/distpt;
		pt.y = pt.y/distpt;
        //we consider transfer of the beginning of coordinates
		pt.x += pt1.x;
		pt.y += pt1.y;
	}
    //if two vectors are opposite directed (a angle between vectors developed)
	else if((pt.x==0)&&(pt.y==0)){
        //we look for coordinates of the end of a single vector from the beginning in 'pt1' point,
        //lying perpendicular to the direction of the set vectors;
              //the direction of a single vector we choose so that a triangle of 'pt1', 'pt3', 'pt'
        //it is focused clockwise
		pt = GetPerpendikular(pt1, pt3, pt1, -1);
	}
	return pt;
}


//========================================================
//!Function returns a subline number 'nIndex' from the line 'str',
//! at which sublines are divided by spaces
MString GetPartOfString(int nPart, MString str)
//=========================================================
{
	MString s2,s3;
	if(nPart<0) 
	{
		s2="";
		return s2;
	}
    //we prepare a line for processing
	str.TrimLeft();
	str.TrimRight();
	str+=" ";
	
	bool bo=true;
	int i=0;
	while (bo)
	{
        //We look for a space
		int j=str.Find(' ');
        //If the space isn't found, we return an empty line
		if(j==-1) 

		{
			bo = false;
			s2="";
		}
        //otherwise we divide a line into parts
		else
		{
			s2=str.Left(j);
			s3=str.Right(str.GetLength()-j);

			str=s3;
			str.TrimLeft();
		}
		if(i==nPart) bo=false;
		i++;
	}
	return s2;
}


//=========================================================================
//!Function of receiving the array of dividers for formatting of lines of formulas
/**Removes the array of indexes of elements after which there was an inclusion of a divider
//Also returns 'MString' with remote dividers
//Removal has to is carried out in a look
//if(pointer = MScanf(MString* in,MString* out, char separator, int& number))
//{
//	free(pointer);
//}
*/
int* MScanf(MString* in,MString* out, char separator, int& number)
{
	if(*in==""){
		*out = "";
		number =0;
		return NULL;

	}
	*out=*in;
	int sep = separator;
	int length = in->GetLength();
	char* buffer = *in;
	char* m_pStart = buffer;
	int* m_pSeparators = (int*)malloc(length*sizeof(int));//new int[length];
	number = 0;
    //finding of a symbol
	while((m_pStart=strchr(m_pStart,sep))){
        //preparation of an output line and array
		m_pSeparators[number]=(m_pStart-buffer)-number;
		out->Delete(m_pSeparators[number]);
		number++;
		m_pStart++;
	}
	*in = buffer;//->ReleaseBuffer();
	if(number)
		return m_pSeparators;

	
	free(m_pSeparators);
	return NULL;
}

//===================================================================
//!Parse of a line with removal of the empty lines which are
//! between dividers
void MScanfArrWithoutEmpty(MString* in,MStArray* outArr, char separator)
//=====================================================================
{
	MScanfArr(in,outArr,separator);
	int iSize = outArr->GetSize();
	for(int i=0;i<iSize;i++){
		if(outArr->GetAt(i)->IsEmpty()){
			iSize--;
			outArr->Delete(i);
			i--;
		}
	}
}

//=========================================================================
//!Function of receiving the array of lines for formatting of lines of formulas
/**Removes the array of lines after which there was an inclusion of a divider
//The array is created and destroyed out of this function
*/
void MScanfArr(MString* in,MStArray* outArr, char separator)
{
	MString m_strWithoutSep;
	int m_iNumOfSeparators;
	int *m_pSepArrPointer;
	m_pSepArrPointer = MScanf(in,&m_strWithoutSep,separator,m_iNumOfSeparators);

	int m_iLastSeparatorIndex = 0;
//	char *m_pDestBuf = NULL;
	MString str = "";
	char *m_pSourseBuf = m_strWithoutSep;//.GetBuffer(m_strWithoutSep.GetLength());

	if(m_iNumOfSeparators){
//		m_pDestBuf = NULL;
        //further there is a copying of a line
		for(int i = 0;i<m_iNumOfSeparators;i++){
			outArr->AddTail(&str);
			int m_iSubStringLength = m_pSepArrPointer[i]-m_iLastSeparatorIndex;
			MString destString = *outArr->GetAt(i);
			//m_pDestBuf = destString.GetBuffer(m_iSubStringLength);
			destString.assign(m_pSourseBuf+m_iLastSeparatorIndex,m_iSubStringLength);
//			memcpy(m_pDestBuf,(void*)(m_pSourseBuf+m_iLastSeparatorIndex),m_iSubStringLength);
			m_iLastSeparatorIndex = m_pSepArrPointer[i];
			*outArr->GetAt(i) = destString;
		}
		//m_strWithoutSep.ReleaseBuffer();
//        delete[] m_pSepArrPointer;
        free(m_pSepArrPointer);
    }
    //Loading of the last line (which doesn't come to an end a divider)
	int m_iStrLength = m_strWithoutSep.GetLength();
	if((m_iLastSeparatorIndex != m_iStrLength)&&
	  (m_iStrLength)){
		outArr->AddTail(&str);
		int i = outArr->GetSize()-1;
		MString destString = *outArr->GetAt(i);
		int m_iSubStringLength = m_iStrLength-m_iLastSeparatorIndex;
		//m_pDestBuf = destString.GetBuffer(m_iSubStringLength);
		//memcpy(m_pDestBuf,(void*)(m_pSourseBuf+m_iLastSeparatorIndex),m_iSubStringLength);
		destString.assign(m_pSourseBuf+m_iLastSeparatorIndex,m_iSubStringLength);
		//destString.ReleaseBuffer();
		*outArr->GetAt(i) = destString;
	}
	

}


//==============================================================================
//
static  double A1,B1,C1,A2,B2,C2;
//!This function finds linear transformation,
/**if 3 some points of 'pt1', 'pt2', 'pt3' and corresponding are given
//to them images of 'pt4', 'pt5', 'pt6' received after application to these 3 points of 'pt1', 'pt2', 'pt3'
//required linear transformation, In other words function finds 'A1','B1','C1','A2','B2','C2' coefficients
//transformations of the plane of a look
//       A1*(x*)+B1*(y*)+C1=y
//       A2*(x*)+B2*(y*)+C2=x
//on known 6 points from which 3 points are points before transformation, and others 3 are the points received
//from data this linear transformation respectively (x*, y*-old coordinates of a point, x, y - new)
//The returned values:
//  0 - points of 'pt1', 'pt2', 'pt3' are various and don't lie on one straight line (linear transformation is defined unambiguously it isn't dependent
//      from an arrangement of points of 'pt4', 'pt5', 'pt6' on the plane);
//  1 - points of 'pt1', 'pt2', 'pt3' are various and lie on one straight line or 2 of 3 points of 'pt1', 'pt2', 'pt3' coincide
//      or 3 points of 'pt1', 'pt2', 'pt3' coincide (linear transformation either doesn't exist or is defined not unambiguously
//      it isn't dependent on an arrangement of points of 'pt4', 'pt5', 'pt6' on the plane)
*/
int CalcLinPreobrBy3pt(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint pt5,MPoint pt6)

//==============================================================================
 {
      double d1,d2,d3,a[3][4],b[3],t,q;
	  int    i,j,k,l;

      d1=DLINASG( pt1 , pt2 );
      d2=DLINASG( pt1 , pt3 ); 
      d3=DLINASG( pt2 , pt3 );

      if((d1+d2!=d3)&&(d1+d3!=d2)&&(d2+d3!=d1)) 
	  {  
  //we set an expanded matrix of system of the linear equations of the 3rd order of a look
  //	 a[i][0]*A1 + a[i][1]*B1 + a[i][2]*C1 = a[i][3] (i=0,1,2)

  //concerning 3 unknown A1,B1,C1 which after their search fill in the same order an array b []
	      a[0][0]=pt1.x; a[0][1]=pt1.y; a[0][2]=1; a[0][3]=pt4.y;
	      a[1][0]=pt2.x; a[1][1]=pt2.y; a[1][2]=1; a[1][3]=pt5.y;
	      a[2][0]=pt3.x; a[2][1]=pt3.y; a[2][2]=1; a[2][3]=pt6.y;

		  for (i=0;i<3;i++) b[i]=0;
          
  //we solve system of the linear equations of the 3rd order of a type of an [i][0]*A1 + a[i][1]*B1 + a[i][2]*C1 = an [i][3] (i=0,1,2)
          //concerning 3 unknown A1,B1,C1 which after their search fill in the same order an array b []
		  int i = 0;
          for(i=0;i<3;i++)
		  {  j=0;
             for(l=0;l<2;l++)
                if(fabs(a[i][l])<fabs(a[i][l+1])) j=l+1;
             t=a[i][j];
             for(l=0;l<4;l++) a[i][l]=a[i][l]/t;
             for(k=0;k<3;k++)
                if(i!=k) 
				{  q=a[k][j];
                   for(l=0;l<4;l++)
				   {  t=a[i][l];
	                  a[k][l]=a[k][l]-t*q;  
				   }
				} 
		  } 
          for(i=0;i<3;i++)
           for(int j=0;j<3;j++)
             if(a[i][j]==1) b[j]=a[i][3];
  
			 
          A1=b[0]; B1=b[1]; C1=b[2];

  //we set an expanded matrix of system of the linear equations of the 3rd order of a type of an [i][0]*A2 + a[i][1]*B2 + a[i][2]*C2 = an [i][3] (i=0,1,2)
          //concerning 3 unknown A2,B2,C2 which after their search fill in the same order an array b []
		  a[0][0]=pt1.x; a[0][1]=pt1.y; a[0][2]=1; a[0][3]=pt4.x;
	      a[1][0]=pt2.x; a[1][1]=pt2.y; a[1][2]=1; a[1][3]=pt5.x;
	      a[2][0]=pt3.x; a[2][1]=pt3.y; a[2][2]=1; a[2][3]=pt6.x;

  //we solve system of the linear equations of the 3rd order of a type of an [i][0]*A2 + a[i][1]*B2 + a[i][2]*C2 = an [i][3] (i=0,1,2)
          //concerning 3 unknown A2,B2,C2 which after their search fill in the same order an array b []
	     for(i=0;i<3;i++)
		 {  j=0;
            for(l=0;l<2;l++)
               if(fabs(a[i][l])<fabs(a[i][l+1])) j=l+1;
            t=a[i][j];
            for(l=0;l<4;l++) a[i][l]=a[i][l]/t;
            for(k=0;k<3;k++)
               if(i!=k) 
			   {  q=a[k][j];

                  for(l=0;l<4;l++)
				  {  t=a[i][l];
	                 a[k][l]=a[k][l]-t*q;  
				  }
			   } 
		 } 

         for(i=0;i<3;i++)
           for(int j=0;j<3;j++)
             if(a[i][j]==1) b[j]=a[i][3];

         A2=b[0]; B2=b[1]; C2=b[2];



	    return 0;
	  } else return 1;
 }
 

//===============================================================================
//!This function finds an image of a point at the linear transformation received above
void MakeLinPreobr(MPoint *pt)
//===============================================================================
{
    MPoint new_pt;

    new_pt.y = A1*(pt->x) + B1*(pt->y) + C1;
    new_pt.x = A2*(pt->x) + B2*(pt->y) + C2;
   
	pt[0] = new_pt;
}



//===============================================================================
static double PPX,PPY,RX,RY,UGOLPOV;
//! ----------
void SetKompozParameters(double a,double b,double k,double l,double alfa)
{//===============================================================================

	PPX = a;
	PPY = b;
	RX  = k;
	RY  = l;
	UGOLPOV = alfa;

}

//===============================================================================
//!Given function finds an image of a point of pt at linear transformation of the plane
//!general view
/**       y = A1*(x*)+B1*(y*)+C1
//       x = A2*(x*)+B2*(y*)+C2
//('x*','y*'-sharpen old coordinates; 'x', 'y' - new) about which is known the following:
//      -the center of system of coordinates at this linear transformation by the parallel
//       by transfer it is displaced in a point with coordinates (a, b)
//      -coordinate axes of Ox and Oy of the received new system of coordinates stretch
//       (contract) in 'k' and 'l' of times respectively
//      -the system of coordinates received as a result of the first two transformations
//       turns on 'alfa' angle
//This linear transformation of the plane taking into account 3 conditions has an appearance above:
//       y = (-k*sin(alfa))*(x*) + (l*cos(alfa))*(y*) + (k*a*sin(alfa)-l*b*cos(alfa))   
//       x = (k*cos(alfa))*(x*) + (l*sin(alfa))*(y*) + (-k*a*cos(alfa)-l*b*sin(alfa))
//
*/
void KompozLinPreobr(MPoint *pt)
//===============================================================================
{
    MPoint new_pt;

		new_pt.y = (-RX*sin(UGOLPOV))*(pt->x) + (RY*cos(UGOLPOV))*(pt->y) + (RX*PPX*sin(UGOLPOV)-RY*PPY*cos(UGOLPOV));
	    new_pt.x = (RX*cos(UGOLPOV))*(pt->x) + (RY*sin(UGOLPOV))*(pt->y) + (-RX*PPX*cos(UGOLPOV)-RY*PPY*sin(UGOLPOV));


	pt->x = new_pt.x;
	pt->y = new_pt.y;
}


//===============================================================================
//!  -----------
void SetLinPreobrByCoefParameters(double a1,double b1,double c1,double a2,double b2,double c2)
//===============================================================================
{
	A1 =a1 ; B1 =b1 ; C1 =c1 ;
	A2 =a2 ; B2 =b2 ; C2 =c2 ;
}


//===============================================================================
//!This function finds an image of a point of 'pt' at linear transformation of the plane of a look
/**       y = A1*(x*)+B1*(y*)+C1
//       x = A2*(x*)+B2*(y*)+C2
//('x*', 'y*' - old coordinates of points of the plane, and 'x', 'y' - new coordinates) in case
//coefficients of 'Aai' and 'Bbi' (i=1,2) of this linear transformation are set
//
*/
void LinPreobrByCoef(MPoint *pt)
//===============================================================================
{
	 MPoint new_pt;

		new_pt.y = A1*(pt->x) + B1*(pt->y) + C1;
	    new_pt.x = A2*(pt->x) + B2*(pt->y) + C2;


	pt->x = new_pt.x;
	pt->y = new_pt.y;

}


//=====================================
//!Receiving an azimuth are also long from the first point to the second
void GetAzimutAndLength(MPoint pt1,MPoint pt2,double &dAzimut,double &dLength)
//=====================================
{
	dLength = sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x)+(pt1.y - pt2.y)*(pt1.y - pt2.y));
	dAzimut = 360 - GetVektorGrad(&pt1,&pt2) + 90;
	while(dAzimut>360){
		dAzimut-=360;
	}
}


//!==============================================
/**On the entrance array of tops of the broken line along which we have a line, and a line
//we form the array of points of an arrangement of letters of a line, which, generally at discrepancy
//numbers of letters and number of tops of a broken line, doesn't coincide with the array of tops of a broken line;
//REMARK: during function performance the entrance ptar array is substituted for the output array
//        coordinates of letters therefore if the entrance array needs to be kept, we give on an entrance
//        functions the duplicated array
*/
bool FindPointsForKrivNadpis(MPtArray* ptar, MString str)
//==============================================
{
	if(!ptar)
		return false;

	int nPoints = ptar->GetSize();
	
	if(nPoints==0)
		return false;
	
	if(str.GetLength()==0)
		return true;

    // array of the partial sums of distances
    // i-th a array element - distance from 0 to point i-th
	MDuArray* summ = new MDuArray();
	summ->SetSize(nPoints);

    //for a zero element of the array the distance is equal to zero
	double d = 0;
	summ->SetAt(0,&d);

	int i = 0;
	for(i=1; i<nPoints; i++){
		d = *summ->GetAt(i-1) + DLINASG(*ptar->GetAt(i-1), *ptar->GetAt(i));
		summ->SetAt(i, &d);
	}

    // array of lengths of segments
    //i-th a array element - length i-th broken line edges
	MDuArray* dlin = new MDuArray();
	dlin->SetSize(nPoints-1);
	for(i=0; i<nPoints-1; i++){
		d = DLINASG(*ptar->GetAt(i),*ptar->GetAt(i+1)); 
		dlin->SetAt(i, &d);
	}

	int kol = str.GetLength();
    // the array of distances to letters

    // i-th a array element - distance from 0 to letter i-th
	MDuArray* rasst = new MDuArray();
	rasst->SetSize(kol);
	d = 0;
	for(i=0; i<kol; i++){
		d = i*(*summ->GetAt(nPoints-1))/(double)(kol-1);
		rasst->SetAt(i, &d);
	}

    // new array of geopoints
	MPtArray* newPoints=new MPtArray();
	newPoints->SetSize(kol);

    //we calculate an arrangement of points of the bases of letters
    //the first (coincides)...
	newPoints->SetAt(0,ptar->GetAt(0));
    // others...
	MPoint pt, pt1, pt2;
	double r, s;
	for (i=1; i<kol-1; i++) {
		int j = 0;
		int k = 0;
        // we will find such k that i-th a letter get
        //in a segment between k-th and (k+1)-th points
		while(*summ->GetAt(j)<*rasst->GetAt(i)) {
			k = j;
			j++;
		}
        //core...
		pt1 = *ptar->GetAt(k);
		pt2 = *ptar->GetAt(k+1);
		r = *rasst->GetAt(i);
		s = *summ->GetAt(k);
		d = *dlin->GetAt(k);
		pt.x = pt1.x + (pt2.x - pt1.x)*(r - s)/d;
		pt.y = pt1.y + (pt2.y - pt1.y)*(r - s)/d;
		newPoints->SetAt(i, &pt);
	}
    // last...
	newPoints->SetAt(kol-1,ptar->GetAt(nPoints-1));

	ptar->ClearAll();
	ptar->AppendTail(newPoints);

	delete summ;
	delete dlin;
	delete rasst;

	return true;
}

//==================================
//!we calculate the array of angles of rotation of letters
//! 'ptar' - the entrance array of coordinates of letters of an inscription;
//! 'angles' - the output array of angles of rotation of letters
bool FindAnglesForKrivNadpis(MPtArray* ptar, MInArray* angles)
//==================================
{
    // GENERAL IDEA IS AS FOLLOWS:
    //1) from each point we carry out a vector, perpendicular to a segment,
    //   connecting the previous and subsequent points, and also we find a vector,
    //   the return to it (for the first/last point we take a segment,
    //   connecting it to a second/penultimate point);
    //2) we choose from these two vectors what goes to the left
    //   from the line of an inscription at the movement along the line (it becomes through calculation
    //   the focused area);
    //3) including this vector the left normal to the line of an inscription, we calculate
    //   letter angle;

	if((!ptar)||(!angles))
		return false;
	angles->ClearAll();
	int kol = ptar->GetSize();
	if(kol==0)
		return false;
	angles->SetSize(kol);

	double AT,T,X,Y;
	MPoint pr1,pr2,pt1,pt2,pt3;

	for (int i=0; i<kol; i++) {
		pt1 = *ptar->GetAt(i);
		pt2 = *ptar->GetAt(i+1);
		pt3 = *ptar->GetAt(i-1);
        //the first point
		if (i==0) {
			pr1.x = pt1.x - (-(pt2.y - pt1.y)/(pt2.x - pt1.x))*10;
			pr1.y = pt1.y - 10;
			pr2.x = pt1.x - (+(pt2.y - pt1.y)/(pt2.x - pt1.x))*10;
			pr2.y = pt1.y + 10;

			if (((pt1.x-pt2.x)*(pt1.y+pt2.y) + (pt2.x-pr1.x)*(pt2.y+pr1.y)
			  + (pr1.x-pt1.x)*(pr1.y+pt1.y)) < 0) {
				Y = pr1.y - pt1.y;
				X = pr1.x - pt1.x;
			}
			else {
				Y = pr2.y - pt1.y;
				X = pr2.x - pt1.x;
			}
		}
        // last point
		else if (i==kol-1) {
			pr1.x = pt1.x - (-(pt1.y - pt3.y)/(pt1.x - pt3.x))*10;
			pr1.y = pt1.y - 10;
			pr2.x = pt1.x - (+(pt1.y - pt3.y)/(pt1.x - pt3.x))*10;
			pr2.y = pt1.y + 10;

			if (((pt3.x-pt1.x)*(pt3.y+pt1.y) + (pt1.x-pr1.x)*(pt1.y+pr1.y)
			  + (pr1.x-pt3.x)*(pr1.y+pt3.y)) < 0) {
				Y = pr1.y - pt1.y;
				X = pr1.x - pt1.x;
			}
			else {
				Y = pr2.y - pt1.y;
				X = pr2.x - pt1.x;
			}
		}
        //others points
		else {
			pr1.x = pt1.x - (-(pt2.y - pt3.y)/(pt2.x - pt3.x))*10;
			pr1.y = pt1.y - 10;
			pr2.x = pt1.x - (+(pt2.y - pt3.y)/(pt2.x - pt3.x))*10;
			pr2.y = pt1.y + 10;

			if (((pt1.x-pt2.x)*(pt1.y+pt2.y) + (pt2.x-pr1.x)*(pt2.y+pr1.y)
			  + (pr1.x-pt1.x)*(pr1.y+pt1.y)) < 0) {
				Y = pr1.y - pt1.y;
				X = pr1.x - pt1.x;

			}
			else {
				Y = pr2.y - pt1.y;
				X = pr2.x - pt1.x;
			}
		}

        //... and at last, we calculate and we write down a angle
		T = Y/X;
		AT = atan(T)*(double)3600.0/((double)2.0*(double)3.1516926);

		if((X>0)&&(Y>0)) AT = (double)2700.0 - AT;
		if((X>0)&&(Y<0)) AT = (double)2700.0 - AT;
		if((X<0)&&(Y>0)) AT = (double)900.0  - AT;
		if((X<0)&&(Y<0)) AT = (double)900.0  - AT;

		int angle = (int)AT;
		if(X==0)
			angle = 0;
		angles->SetAt(i,&angle);
	}

	return true;

}

//==============================================
//!forms the broken line from 'num' of points corresponding to the spline determined by four
//! entrance points;
//! 'pt1' and 'pt2' - the ends of a spline, 'pt3' and 'pt4' - the ends of bars of a spline
bool FormLomlineBySimpleSpline(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, int num,
							   MPtArray* ptar)
//==============================================
{
	if(!ptar)
		return false;

	ptar->ClearAll();
	ptar->AddTail(&pt1);
    //we break a parametrical segment length unit into 'num' of parts
	double shag = (double)1/(double)num;
	MPoint pt;
    //using the parametrical equations of the cubic spline constructed on four
    //to points, we will calculate coordinates of each of points of the broken line corresponding to a spline
	for(double t=shag; t<=1; t+=shag){
		pt.x = (1-t)*(1-t)*(1-t)*pt1.x + 3*t*(1-t)*(1-t)*pt3.x + 3*t*t*(1-t)*pt4.x + t*t*t*pt2.x;
		pt.y = (1-t)*(1-t)*(1-t)*pt1.y + 3*t*(1-t)*(1-t)*pt3.y + 3*t*t*(1-t)*pt4.y + t*t*t*pt2.y;
		ptar->AddTail(&pt);
	}
    //if the segment doesn't share totally on num of parts, we will add the end of a spline
	if(pt!=pt2)
		ptar->AddTail(&pt2);
	
	return true;
}


//====================================================
//!Calculates a crossing rectangle if the pointer of result is set

/**the parties of rectangles for this function HAVE TO be PARALLEL
//TO AXES OF COORDINATES
//'true' - if was crossing
*/
bool RectIntersection(MRect mrc1,MRect mrc2,MRect* pmrcResult)
//====================================================
{
	if(!mrc1.IntersectGeoRect(mrc1,mrc2))
		return false;
	if(!pmrcResult)
		return true;
	MPtArray ptar;
	ptar.SetSize(1);
	MPtArray *pptar = &ptar;
    // Finding of a rectangle of crossing
	
    //Top with the right
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.left,mrc1.top),MPoint(mrc1.right,mrc1.top),
		MPoint(mrc2.right,mrc2.top),MPoint(mrc2.right,mrc2.bottom),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}
    //Top with the left
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.left,mrc1.top),MPoint(mrc1.right,mrc1.top),

		MPoint(mrc2.left,mrc2.top),MPoint(mrc2.left,mrc2.bottom),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}
    //Lower with the right
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.left,mrc1.bottom),MPoint(mrc1.right,mrc1.bottom),
		MPoint(mrc2.right,mrc2.top),MPoint(mrc2.right,mrc2.bottom),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}
    //Lower with the left
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.left,mrc1.bottom),MPoint(mrc1.right,mrc1.bottom),
		MPoint(mrc2.left,mrc2.top),MPoint(mrc2.left,mrc2.bottom),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}


    //Right with the top
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.right,mrc1.top),MPoint(mrc1.right,mrc1.bottom),
		MPoint(mrc2.right,mrc2.top),MPoint(mrc2.left,mrc2.top),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}
    //Right with the lower
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.right,mrc1.bottom),MPoint(mrc1.right,mrc1.top),
		MPoint(mrc2.right,mrc2.bottom),MPoint(mrc2.left,mrc2.bottom),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}

    //Left with the top
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.left,mrc1.top),MPoint(mrc1.left,mrc1.bottom),
		MPoint(mrc2.right,mrc2.top),MPoint(mrc2.left,mrc2.top),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}
    //Left with the lower
	if(HORS_VERT_OTREZOK_CROSS(MPoint(mrc1.left,mrc1.top),MPoint(mrc1.left,mrc1.bottom),
		MPoint(mrc2.right,mrc2.bottom),MPoint(mrc2.left,mrc2.bottom),pptar->GetAt(pptar->GetSize()-1))){
		pptar->SetSize(pptar->GetSize()+1);
	}

	pptar->DeleteTail();
    //Now we have points of intersection
	int iSize = pptar->GetSize();
	if(iSize == 0){
        // If there are no points of intersection that one of rectangles is included
		MPoint point(mrc2.left,mrc2.top);
		if(mrc1.IsPtInRect(point)){
			*pmrcResult = mrc2;
			return true;
		}
		else{
			*pmrcResult = mrc1;
			return true;
		}
	}
	else if(iSize == 4){
        //If that unambiguity is only required to find 4 points of intersection
		double dXMax = Max(Max(pptar->GetAt(0)->x,pptar->GetAt(1)->x),Max(pptar->GetAt(2)->x,pptar->GetAt(3)->x));
		double dXMin = Min(Min(pptar->GetAt(0)->x,pptar->GetAt(1)->x),Min(pptar->GetAt(2)->x,pptar->GetAt(3)->x));
		double dYMax = Max(Max(pptar->GetAt(0)->y,pptar->GetAt(1)->y),Max(pptar->GetAt(2)->y,pptar->GetAt(3)->y));
		double dYMin = Max(Max(pptar->GetAt(0)->y,pptar->GetAt(1)->y),Max(pptar->GetAt(2)->y,pptar->GetAt(3)->y));
		*pmrcResult = MRect(dXMin,dYMax,dXMax,dYMin);
		return true;

	}
	else if(iSize == 2){
        //In this case or the angle is included and then points unambiguously set a rectangle
		if((pptar->GetAt(0)->x!=pptar->GetAt(1)->x)&&(pptar->GetAt(0)->y!=pptar->GetAt(1)->y)){
			*pmrcResult = MRect(Min(pptar->GetAt(0)->x,pptar->GetAt(1)->x),
								Max(pptar->GetAt(0)->y,pptar->GetAt(1)->y),

								Max(pptar->GetAt(0)->x,pptar->GetAt(1)->x),
								Min(pptar->GetAt(0)->y,pptar->GetAt(1)->y));
			return true;
		}
		else{

            //Or 2 points of inclusions and a point of intersection are on one line
            //We find the points included in a rectangle
			pptar->SetSize(pptar->GetSize()+1);
			//Rect2 in Rect1
			MPoint point(mrc2.left,mrc2.top);
			if(mrc1.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc2.left,mrc2.top);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			point = MPoint(mrc2.right,mrc2.top);
			if(mrc1.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc2.right,mrc2.top);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			point = MPoint(mrc2.right,mrc2.bottom);
			if(mrc1.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc2.right,mrc2.bottom);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			point = MPoint(mrc2.left,mrc2.bottom);
			if(mrc1.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc2.left,mrc2.bottom);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			//Rect1 in Rect2;

			point = MPoint(mrc1.left,mrc1.top);
			if(mrc2.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc1.left,mrc1.top);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}

			point = MPoint(mrc1.right,mrc1.top);
			if(mrc2.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc1.right,mrc1.top);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			point = MPoint(mrc1.right,mrc1.bottom);
			if(mrc2.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc1.right,mrc1.bottom);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			point = MPoint(mrc1.left,mrc1.bottom);
			if(mrc2.IsPtInRect(point)){
        MPoint pnt= MPoint(mrc1.left,mrc1.bottom);
				pptar->SetAt(pptar->GetSize()-1,&pnt);
				pptar->SetSize(pptar->GetSize()+1);
			}
			double dXMax = Max(Max(pptar->GetAt(0)->x,pptar->GetAt(1)->x),Max(pptar->GetAt(2)->x,pptar->GetAt(3)->x));
			double dXMin = Min(Min(pptar->GetAt(0)->x,pptar->GetAt(1)->x),Min(pptar->GetAt(2)->x,pptar->GetAt(3)->x));
			double dYMax = Max(Max(pptar->GetAt(0)->y,pptar->GetAt(1)->y),Max(pptar->GetAt(2)->y,pptar->GetAt(3)->y));
			double dYMin = Min(Min(pptar->GetAt(0)->y,pptar->GetAt(1)->y),Min(pptar->GetAt(2)->y,pptar->GetAt(3)->y));
			*pmrcResult = MRect(dXMin,dYMax,dXMax,dYMin);
			return true;
		}
	}
	return false;
}

void ClipPolygon(MPtArray *input, MPtArray *output, MRect rect)
{
	SutherlandHodgman sh(rect);
	sh.Clip(input, output);
}

void ClipPolyline(MPtArray *input, QList<MPtArray*> &output, MRect rect)
{
	CohenSutherland cs(rect);
	cs.Clip(input, output);
}

//! --------
union byteInArray{
	char fullByte;
//! -----	
	struct{
		bool b0:1;
		bool b1:1;
		bool b2:1;
		bool b3:1;
		bool b4:1;
		bool b5:1;
		bool b6:1;
		bool b7:1;
	}bit;
};

//Functions of work with bit arrays
//=======================================================
//!To receive bit in the array
//! RESPONSIBILITY FOR DIMENSION LIES ON THE DEFIANT
bool GetBitInArray(void* pArray,int iIndex)
//=======================================================
{
	int iLeft = iIndex%8;
	char*  pByte = (char*)pArray + (iIndex-iLeft)/8;
	bool bRetVal;
	switch(iLeft){
	case 0:
		bRetVal = ((byteInArray*)pByte)->bit.b0;
		break;
	case 1:
		bRetVal = ((byteInArray*)pByte)->bit.b1;
		break;
	case 2:
		bRetVal = ((byteInArray*)pByte)->bit.b2;
		break;
	case 3:
		bRetVal = ((byteInArray*)pByte)->bit.b3;
		break;
	case 4:
		bRetVal = ((byteInArray*)pByte)->bit.b4;
		break;
	case 5:
		bRetVal = ((byteInArray*)pByte)->bit.b5;
		break;
	case 6: 
		bRetVal = ((byteInArray*)pByte)->bit.b6;
		break;
	case 7:
		bRetVal = ((byteInArray*)pByte)->bit.b7;
		break;
//	default: exit(15);
	}
	if(bRetVal)
		return true;
	return false;
}


//=======================================================
//!To receive bit in the array
//! RESPONSIBILITY FOR DIMENSION LIES ON THE DEFIANT
void SetBitInArray(void* pArray,int iIndex,bool bval)
//=======================================================
{
	int iLeft = iIndex%8;
	char*  pByte = (char*)pArray + (iIndex-iLeft)/8;
	switch(iLeft){
	case 0:
		((byteInArray*)pByte)->bit.b0 = bval;
		break;
	case 1:
		((byteInArray*)pByte)->bit.b1 = bval;
		break;
	case 2:
		((byteInArray*)pByte)->bit.b2 = bval;
		break;
	case 3:
		((byteInArray*)pByte)->bit.b3 = bval;
		break;
	case 4:
		((byteInArray*)pByte)->bit.b4 = bval;
		break;
	case 5:
		((byteInArray*)pByte)->bit.b5 = bval;
		break;
	case 6: 
		((byteInArray*)pByte)->bit.b6 = bval;
		break;
	case 7:
		((byteInArray*)pByte)->bit.b7 = bval;
		break;
//	default:		exit(15);
	}
}

//==============================================================================
//!Crossing of two segments
bool   CROSSSECTION(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt)
//==============================================================================
{
	double dTemp;

	int res = IntersectLineWithOtrezok(pt1,pt2,pt3,pt4,pt,dTemp);
	if(res==0){
		if(PTONSEG(*pt,pt3,pt4))
			return true;
	}
    //if one of the ends of one segment coincides with one of the ends of other segment
	else if((res==10)||(res==11))
	{
		if(PTONSEG(*pt,pt1,pt2,EPSILON)) return true;
	}

	return false;
}

//==============================================================================
//!Crossing of a segment with a contour
/**ENTRANCE: 'pt1', 'pt2' - coordinates of the ends of a segment, 'kontur' - the coordinate of tops of a contour;
//EXIT: 'crosspts' - coordinates of points of intersection of a segment with a contour,
//'indexes' - the array of indexes of the edges of a contour corresponding to points of intersection
*/
bool FindCrossSegWithKontur(MPoint pt1, MPoint pt2, const MPtArray* kontur, MPtArray* crosspts, MInArray* indexes)
//==============================================================================
{
	MPoint res;
	crosspts->ClearAll();
	if(indexes!=NULL)
		indexes->ClearAll();

	for(int i=0; i<kontur->GetSize()-1; i++){
		if(CROSSSECTION(pt1, pt2, *kontur->GetAt(i), *kontur->GetAt(i+1), &res)){
			crosspts->AddTail(&res);
			if(indexes!=NULL)
				indexes->AddTail(&i);
		}
	}

  if(crosspts->GetSize()==0)
  return false;

  return true;
}

//Intersection of a ray specified by a point with a stator from standPoint to vectPoint, with a contour arZoneCoord
//Return true if there are intersections. False if there is no
//In the case of several intersections, returns the result point maximally remote from the beam origin
bool CalculateIntersection(const MPoint& standPoint, const MPoint& vectPoint,  MPtArray& arZoneCoord, MPoint& result)
{
	MPtArray crosspts;
	crosspts.ClearAll();
	double maxDist=0;
	int index=-1;

	for(int i=0; i<arZoneCoord.GetSize(); i++){
		MPoint res, seg1, seg2;
		seg1 = arZoneCoord.GetAt(i);
		if (i == arZoneCoord.GetSize()-1)
			seg2 = arZoneCoord.GetAt(0);
		else seg2 = arZoneCoord.GetAt(i+1); 
		
		if(CROSSLINES(standPoint, vectPoint, seg1, seg2, &res))
			// if there was a point of intersection of an edge of a contour with a segment,
			// check that the starting point of the segment does not lie on the contour edge
			if(PTONSEG(res,seg1, seg2))
			// check that the intersection point belongs to the ray
			if(fabs(FindUgolBetweenVectors(vectPoint, standPoint, res))<0.01)
			{
				crosspts.AddTail(&res);
				double dist = DLINASG(standPoint, res);
				if (crosspts.Size==1)
				{
					maxDist = dist;
					index = 0;
				}		
				else if (dist>maxDist)
				{
					maxDist = dist;				
					index = crosspts.Size-1;
				}
			
			}
	}
	
	if (index!=-1)
	{
		result = crosspts.GetAt(index);
		return true;
	}
	else return false;
}

//Intersection of a ray specified by a point with a stator from standPoint to vectPoint, with a contour arZoneCoord
//Return true if there are intersections. False if there is no
//In the case of several intersections, returns the result point maximally remote from the beam origin
bool CalculateIntersection(const MPoint& standPoint, int angle,  MPtArray& arZoneCoord, MPoint& result)
{
	MPoint vectPoint = GetPointByAzD(standPoint, angle,100);
	return CalculateIntersection(standPoint,vectPoint,arZoneCoord,result);
}
//==============================================================================
//!Crossing of the segment which isn't including the initial point with a contour
bool   FindCrossOpenSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts)
//==============================================================================
{
	MPoint res;
	crosspts->ClearAll();

	for(int i=0; i<kontur->GetSize()-1; i++){
		if(CROSSSECTION(pt1, pt2, *kontur->GetAt(i), *kontur->GetAt(i+1), &res))
            //if there was a contour edge point of intersection with a segment,
            //we will check that the initial point of a segment didn't lie on a contour edge
			if(!PTONSEG(pt1,*kontur->GetAt(i), *kontur->GetAt(i+1)))
				crosspts->AddTail(&res);
	}

  if(crosspts->GetSize()==0)
  return false;

  return true;
}

//==============================================================================
//!Finding of internal points of intersection of a segment with a contour (such, that a point of intersection
//! I wasn't at the same time top of a contour and one of the ends of a segment)
bool   FindNotVertexCrossSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts)
//==============================================================================
{
	MPoint res;
	crosspts->ClearAll();

	MPoint segpt1, segpt2;
	for(int i=0; i<kontur->GetSize()-1; i++){
		segpt1 = *kontur->GetAt(i);
		segpt2 = *kontur->GetAt(i+1);
		if(CROSSSECTION(pt1, pt2, segpt1, segpt2, &res))
            //if there was a contour edge point of intersection with a segment,
            //we will check that the segment of a contour and a segment weren't crossed on the ends
			if(!((res==pt1)&&(res==segpt1))&&!((res==pt1)&&(res==segpt2))&&
				!((res==pt2)&&(res==segpt1))&&!((res==pt2)&&(res==segpt2)))
				crosspts->AddTail(&res);
	}

  if(crosspts->GetSize()==0)
  return false;

  return true;
}

//==============================================================================
//!Crossing of a contour with a contour
bool   FindCrossptsLomlineWithLomline(MPtArray* lomline1, MPtArray* lomline2, MPtArray* crosspts)
//==============================================================================
{
	if((!lomline1)||(!lomline2)||(!crosspts))
		return false;

	crosspts->ClearAll();

	MPtArray* segcrosspts = new MPtArray();
	FindCrossSegWithKontur(*lomline1->GetAt(0), *lomline1->GetAt(1),lomline2, segcrosspts);
	int crptCount = segcrosspts->GetSize();
	MPoint temp;
	if(crptCount != 0)
	{
		for (int j = 0; j < crptCount - 1; j++)
			for (int k = 0; k < crptCount - j - 1; k++)
			{
				if (DLINASG(segcrosspts->GetAt(k), *lomline1->GetAt(0)) > DLINASG(segcrosspts->GetAt(k + 1), *lomline1->GetAt(0)))
				{
					temp = segcrosspts->GetAt(k);
					segcrosspts->SetAt(k, segcrosspts->GetAt(k + 1));
					segcrosspts->SetAt(k + 1, &temp);
				}
			}     
		crosspts->AppendTail(segcrosspts);
	}
    //if the segment is crossed from a broken line in the point equal to the beginning of a segment, this point already
    //noted on previous to a step
	for(int i=1; i< lomline1->GetSize()-1; i++){
		segcrosspts->ClearAll();

		FindCrossOpenSegWithKontur(*lomline1->GetAt(i), *lomline1->GetAt(i+1),lomline2, segcrosspts);
		crptCount = segcrosspts->GetSize();
		if(crptCount != 0)
		{
			for (int j = 0; j < crptCount - 1; j++)
				for (int k = 0; k < crptCount - j - 1; k++)
				{
					if (DLINASG(segcrosspts->GetAt(k), *lomline1->GetAt(i)) > DLINASG(segcrosspts->GetAt(k + 1), *lomline1->GetAt(i)))
					{
						temp = segcrosspts->GetAt(k);
						segcrosspts->SetAt(k, segcrosspts->GetAt(k + 1));
						segcrosspts->SetAt(k + 1, &temp);
					}
				}     
			crosspts->AppendTail(segcrosspts);
	}
	}

	if(crosspts->GetSize()==0)
		return false;
	return true;
}

//==============================================================================
//!Is at a self-crossing contour?
bool IsCrossptLomline(MPtArray* lomline)
//==============================================================================
{
	if(!lomline)
		return false;

	MPoint res, pt1, pt2;
    //for an initial segment of a broken line it is checked, whether it crosses a broken line;
    //we exclude adjacent segments
	pt1 = *lomline->GetAt(0);

	pt2 = *lomline->GetAt(1);
	int colpts;
    //if it is a contour, that is initial and final points coincide, the last segment of a broken line
    //is adjacent for initial
	if(pt1==*lomline->GetAt(lomline->GetSize()-1) || DLINASG(pt1, *lomline->GetAt(lomline->GetSize() - 1)) < EPSILON)
		colpts = lomline->GetSize()-2;
	else
		colpts = lomline->GetSize()-1;
	int j = 0;
	for(j=2; j<colpts; j++){
        //if there was a broken line segment point of intersection from a broken line not in an adjacent segment
		if(CROSSSECTION(pt1, pt2, *lomline->GetAt(j), *lomline->GetAt(j+1), &res))
			return true;
	}
    //for each segment of a broken line, except initial and final, we check, whether crosses
    //it a broken line, we exclude adjacent segments
	for(int i=1; i< lomline->GetSize()-2; i++){
		pt1 = *lomline->GetAt(i);
		pt2 = *lomline->GetAt(i+1);
		for(int j=0; j<lomline->GetSize()-1; j++){
            //adjacent segments and the checked broken line segment
			if((j<i-1)||(j>i+1)){
                //if there was a broken line segment point of intersection from a broken line not in an adjacent segment
				if(CROSSSECTION(pt1, pt2, *lomline->GetAt(j), *lomline->GetAt(j+1), &res))
					return true;
			}
		}
	}
    //for a final segment of a broken line it is checked, whether it crosses a broken line;
    //we exclude adjacent segments
	pt1 = *lomline->GetAt(lomline->GetSize()-2);
	pt2 = *lomline->GetAt(lomline->GetSize()-1);
    //if it is a contour, that is initial and final points coincide, an initial segment of a broken line
    //is adjacent for the last
	if(pt2!=*lomline->GetAt(0) && DLINASG(pt2, *lomline->GetAt(0)) > EPSILON){
		if(CROSSSECTION(pt1, pt2, *lomline->GetAt(0), *lomline->GetAt(1), &res))
			return true;
	}
	for(j=1; j<lomline->GetSize()-3; j++){
        //if there was a broken line segment point of intersection from a broken line not in an adjacent segment
		if(CROSSSECTION(pt1, pt2, *lomline->GetAt(j), *lomline->GetAt(j+1), &res))
			return true;
	}
	return false;
}

int Convex(MPtArray* points)
{
   int i,j,k;
   int flag = 0;
   double z;
   int n = points->Size;

   if (n < 3)
      return(0);

   for (i=0;i<n;i++) {
      j = (i + 1) % n;
      k = (i + 2) % n;
      z  = (points->GetAt(j)->x - points->GetAt(i)->x) * (points->GetAt(k)->y - points->GetAt(j)->y);
      z -= (points->GetAt(j)->y - points->GetAt(i)->y) * (points->GetAt(k)->x - points->GetAt(j)->x);
      if (z < 0)
         flag |= 1;
      else if (z > 0)
         flag |= 2;
      if (flag == 3)
         return(-1);
   }
   if (flag != 0)
      return(1);
   else
      return(0);
}


//===============================================================================
//!Crossing of vertical and horizontal segments
//! pt1, pt2 - the first segment
//! pt3, pt4 - the second segment
bool HORS_VERT_OTREZOK_CROSS(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint* pt)
//===============================================================================
{
    //We will make sure about horizontal position or vertical position of lines
//	if(!((pt1.x == pt2.x)||(pt1.y == pt2.y))) exit(15);
//	if(!((pt3.x == pt4.x)||(pt3.y == pt3.y))) exit(15);
	if(pt1.x == pt2.x){
        //first horizontal line
		if(pt3.y == pt4.y){
            //FINDING OF CROSSING
			if(((pt1.y<=pt3.y)&&(pt3.y<=pt2.y))||((pt1.y>=pt3.y)&&(pt3.y>=pt2.y))){
                //'pt3.y' is between 'pt1' and 'pt2 (y)'
				if(((pt1.x<=pt3.x)&&(pt1.x>=pt4.x))||((pt1.x>=pt3.x)&&(pt1.x<=pt4.x))){
				
					pt->x = pt1.x;
					pt->y = pt3.y;
					return true;
				}
			}
		}
	}
	else{
        //first vertical line
		if(pt3.x == pt4.x){
            //FINDING OF CROSSING
            if(((pt1.x<=pt3.x)&&(pt3.x<=pt2.x))||((pt1.x>=pt3.x)&&(pt3.x>=pt2.x))){
                //'pt3.y' is between 'pt1' and 'pt2'
                if(((pt1.y<=pt3.y)&&(pt1.y>=pt4.y))||((pt1.y>=pt3.y)&&(pt1.y<=pt4.y))){
					pt->y = pt1.y;
					pt->x = pt3.x;
					return true;
				}

			}
		}
	}
	return false;
}

//=========================================================
//!Returns coordinates of a point which lies at 's' distance
//! from the beginning of a broken line of 'pta'
MPoint GetPointBySdvig(const MPtArray* pta, double s)
//=========================================================
{
	
    //we will get some variables
	MPoint pt;
	MPoint pt1,pt2;
	if(!pta)  return pt;    //exit(15);
	
    //size of the array of points

	int pcount = pta->GetSize();

	if(pcount == 0)  return pt;  //exit(15);

    //if the space is less than zero, we return the first point
	if(s<=0)
	{
		return *pta->GetAt(0); 
	}

    //we will walk on the array for definition of an edge on which there will be a point
	int     minindex = 1;
	double  minrasst = 0;
	double  maxrasst = 0;
	int i=0;
	while((i<pcount-1)&&(maxrasst<=s))
	{
		minindex = i;
		minrasst = maxrasst;
		pt1 = *pta->GetAt(i);
		pt2 = *pta->GetAt(i+1);
		maxrasst+=DLINASG(pt1,pt2);
		i++;
	}
	
    //if the distance is more than length of a broken line, we return the last point
	if(maxrasst<=s) 

	{
		return *pta->GetAt(pcount-1);
	}

    //we will determine point coordinates
	pt1 = pta->GetAt(minindex);
	pt2 = pta->GetAt(minindex+1);
	double k = (s-minrasst)/DLINASG(pt1,pt2);
	pt.x = pt1.x+k*(pt2.x-pt1.x);

	pt.y = pt1.y+k*(pt2.y-pt1.y);

	return pt;

}


//=====================================================================================================
//!Determination of height of points on them x-coordinates and the array of a profile
bool GetYkoordsByXkoords(MPtArray* profil, MDuArray* Xkoords, MDuArray* Ykoords)
//=====================================================================================================
{
    //at first we will check arrays for NULL
	if((profil==NULL)||(Xkoords==NULL)||(Ykoords==NULL))
		return false;

	int prcnt = profil->GetSize();
	if(prcnt<2)
		return false;
	
	int Xcnt = Xkoords->GetSize();
	if(Xcnt==0)

	{
		Ykoords->ClearAll();
		return true;
	}

    //we will pass across the array of coordinates
	Ykoords->SetSize(Xcnt);
	int profl = 0;
	int profr = 1;
	double xl = profil->GetAt(0)->x;
	double yl = profil->GetAt(0)->y;
	double yr = profil->GetAt(1)->y;
	double xr = profil->GetAt(1)->x;
	double x;
	double y;
	for(int i = 0; i<Xcnt; i++)
	{
		x = *Xkoords->GetAt(i);
        //if the first points are more left than the beginning of a profile, their heights become equal to height of the first point of a profile
		if((profl==0)&&(x<xl))
		{

			y = yl;
		}
        //if the last points are more right than the last point of a profile, their heights become equal to height of the last point of a profile
		else if((profr==prcnt-1)&&(x>xr))

		{
			y = yr;
		}
        //if everything is normal
		else
		{
            //if next point
            //I went beyond
            //current segment of a profile,
            //that we look for a profile site,
            //to which our point gets
			if(x>xr)
			{
				do
				{
					profr++;
					xr = profil->GetAt(profr)->x;

				}
				while((x>xr)&&(profr!=prcnt-1));
				
				profl = profr-1;
				xl = profil->GetAt(profl)->x;
				yl = profil->GetAt(profl)->y;
				yr = profil->GetAt(profr)->y;
			}

            //if x-coordinate of the point after all is more, than the last point of a profile
			if(x>xr)
			{
				y = yr;
			}
			else
			{
				if(xl!=xr)
				{
					y = (x-xl)*(yr-yl)/(xr-xl)+yl;
				}
				else
				{
					y = yl;
				}
			}
		}
		Ykoords->SetAt(i,&y);
	}
	return true;
}


//================================================================================
//!definition of shift of a point from the beginning of a broken line if its coordinates are known
//! the point has to lie authentically on a broken line
double GetSdvigByPoint(MPtArray* pta, MPoint pt)
//================================================================================
{
	if(!pta)   return 0.0;   //exit(15);


    //quantity of points on a broken line
	int pcount = pta->GetSize();

	if(pcount==0)  return 0.0;        //exit(15);

    //if a point in a broken line only one
	if(pcount == 1) return 0;

    //we will find an edge on which the point lies
	int nrebra = -1;
	double minrasst = -1;
	double rasst;
	MPoint pt1,pt2,pt3;

	int i = 0;
	for(i = 0; i<pcount-1;i++)
	{
		pt1 = *pta->GetAt(i);
		pt2 = *pta->GetAt(i+1);

		pt3 = GetPointOnOtrezok(pt1,pt2,pt);
		rasst = DLINASG(pt,pt3);
		if((rasst<minrasst)||(minrasst==-1))
		{
			minrasst = rasst;
			nrebra = i;
		}
	}

    //now we will define shift
	rasst = 0;
	for(i = 0;i<nrebra;i++)
	{
		pt1 = *pta->GetAt(i);
		pt2 = *pta->GetAt(i+1);
		rasst+=DLINASG(pt1,pt2);
	}
	pt2 = *pta->GetAt(nrebra);
	rasst+=DLINASG(pt2,pt);

	return rasst;
}





//==================================
//! Check, whether lies any point of pt on a segment from the ends of 'pt1' and 'pt2'
/** GENERAL IDEA: 1) we check, whether the point of 'pt' of the straight line passing through 'pt1pt2' segment belongs;
//2) we check, whether 'pt' point belongs to 'pt1pt2' segment;
//'pt' - any point, 'pt1', 'pt2' - the ends of a segment
// (x2-x1)*y + (y1-y2)*x + x1*(y2-y1) - y1*(x2-x1) = 0 -  the equation of the straight line passing through a segment of 'p1', 'p2'.
*/
bool PTONSEG_DOSTUP(MPoint pt, MPoint pt1, MPoint pt2, double dostup)
//==================================		
{
    //at first we will check, whether the point coincides with one of the ends of a segment
	if((pt==pt1)||(pt==pt2))
		return true;
    // we check, whether the 'pt' point of the straight line passing through 'pt1pt2' segment belongs
	if(fabs((pt2.x-pt1.x)*pt.y + (pt1.y-pt2.y)*pt.x + pt1.x*(pt2.y-pt1.y) - pt1.y*(pt2.x-pt1.x)) > dostup)
		return false;

    // if the straight line passes vertically...
	if((pt1.x==pt2.x)&&(fabs(pt1.x - pt.x)<dostup)){
		if(ZNAK(pt1.y-pt.y) == ZNAK(pt2.y-pt.y))
		return false;
	}
	else{
		if(ZNAK(pt1.x-pt.x) == ZNAK(pt2.x-pt.x))
			return false;
	}

	return true;
}

//===========================================================
//!levels leaf length to multiple values
//! if height of a leaf and the necessary length is set
int GetDlinaLista(int /*visota*/, int dlina)
//===========================================================
{
	return dlina;
}

//===========================================================
//! To turn a line into an integer
int StrToInt(MString str)
//===========================================================
{
	int retInt =0;

	char buf[200];

	for(unsigned	int i=0; i < sizeof(buf);i++) buf[i] = 0;
	for(unsigned	int j=0; j < str.GetLength(); j++) buf[j] = str.GetAt(j);

    retInt = atoi(buf);

    return retInt;
}

//===========================================================
//! To turn a line into number

double StrToDouble(MString str)
//===========================================================
{
	float retFloat= 0.0;

	char buf[200];
	for(unsigned	int i=0; i < sizeof(buf);i++) buf[i] = 0;
	for(unsigned	int j=0; j < str.GetLength(); j++) buf[j] = str.GetAt(j);

    retFloat = atof(buf);

    return (double)retFloat;
}

//======================================================
//!will transform a line to double
/** if transformation took place successfully, 'true' comes back,
//and the result of transformation lies in 'result'
//if the line - doesn't contain number, 'false' comes back
*/
bool ExtStrToDouble(MString str, double& result)
//======================================================
{
    //we will consider that our lines don't contain degrees

	MString s = str;
    //at first we will clean all spaces and symbols, to them equivalent
	str.TrimLeft();
	str.TrimRight();
	if(str.IsEmpty())
		return false;
	int lenght = str.GetLength();
	
    //we start sorting a line on symbols
	int  sign = str.FindOneOf("+-");
	if(sign!=-1)

	{
		if(sign==0)
		{
			str.Delete(0);
			lenght--;
		}
		else
		{
			return false;
		}
	}
	
	if(lenght==0)
	{
		return false;
	}

    //we will find a point
	int point = str.Find(".");
	if(point!=-1)
	{
		str.Delete(point);
		lenght--;
		if(lenght==0)

			return false;
	}
	
    //we will check all symbols now
	char ch;

	for(int i=0; i<lenght; i++)
	{
		ch = str.GetAt(i);
		if(!((ch=='0')||
		   (ch=='1')||
		   (ch=='2')||
		   (ch=='3')||
		   (ch=='4')||
		   (ch=='5')||
		   (ch=='6')||
		   (ch=='7')||
		   (ch=='8')||
		   (ch=='9')))
		{
			return false;
		}
	}
	

    //now we will transform to 'double'
	result = StrToDouble(s);

	return true;
}


//==================================
//! Transfers paper millimeters to map meters in Gauss's projection.
double CalcBumToGauss(int mastab, double rasst)
//==================================
{
	return rasst*mastab/1000;
}



//===========================================================
//! To get a number of a zone on the central meridian

int CalcGaussZoneNumberByCentralMeridian(int meridian)
//===========================================================
{
	return meridian/6 + 1;
}

//===========================================================
//! To receive the central meridian according to number of a zone
int CalcCentralMeridianByGaussZoneNumber(int zonenumb)
//===========================================================
{
	return 6*zonenumb - 3;
}

//===========================================================
//! To get a number of a zone on a leaf signature stamp.
//! Checks also a correctness of a task of the first two face values.
//! Returns-1 in case of failure
int CalcGaussZoneNumberByGrif(MString str)
//===========================================================
{
    // we break the submitted nomenclature for the sublines
	MStArray* starr = new MStArray();
	MScanfArr(&str,starr,'-');

    // transformation to a digital form of the first face value
	int NML1 = -1;
	if((*starr->GetAt(0)=="A")||(*starr->GetAt(0)=="1"))  NML1 = 0;

	if((*starr->GetAt(0)=="B")||(*starr->GetAt(0)=="2"))  NML1 = 4;
	if((*starr->GetAt(0)=="C")||(*starr->GetAt(0)=="3"))  NML1 = 8;
	if((*starr->GetAt(0)=="D")||(*starr->GetAt(0)=="4"))  NML1 = 12;
	if((*starr->GetAt(0)=="E")||(*starr->GetAt(0)=="5"))  NML1 = 16;
	if((*starr->GetAt(0)=="F")||(*starr->GetAt(0)=="6"))  NML1 = 20;
	if((*starr->GetAt(0)=="G")||(*starr->GetAt(0)=="7"))  NML1 = 24;
	if((*starr->GetAt(0)=="H")||(*starr->GetAt(0)=="8"))  NML1 = 28;
	if((*starr->GetAt(0)=="I")||(*starr->GetAt(0)=="9"))  NML1 = 32;
	if((*starr->GetAt(0)=="J")||(*starr->GetAt(0)=="10")) NML1 = 36;
	if((*starr->GetAt(0)=="K")||(*starr->GetAt(0)=="11")) NML1 = 40;
	if((*starr->GetAt(0)=="L")||(*starr->GetAt(0)=="12")) NML1 = 44;
	if((*starr->GetAt(0)=="M")||(*starr->GetAt(0)=="13")) NML1 = 48;
	if((*starr->GetAt(0)=="N")||(*starr->GetAt(0)=="14")) NML1 = 52;
	if((*starr->GetAt(0)=="O")||(*starr->GetAt(0)=="15")) NML1 = 56;
	if((*starr->GetAt(0)=="P")||(*starr->GetAt(0)=="16")) NML1 = 60;
	if((*starr->GetAt(0)=="Q")||(*starr->GetAt(0)=="17")) NML1 = 64;
	if((*starr->GetAt(0)=="R")||(*starr->GetAt(0)=="18")) NML1 = 68;
	if((*starr->GetAt(0)=="S")||(*starr->GetAt(0)=="19")) NML1 = 72;
	if((*starr->GetAt(0)=="T")||(*starr->GetAt(0)=="20")) NML1 = 76;
	if((*starr->GetAt(0)=="U")||(*starr->GetAt(0)=="21")) NML1 = 80;
	if((*starr->GetAt(0)=="V")||(*starr->GetAt(0)=="22")) NML1 = 84;
	if((*starr->GetAt(0)=="Z")||(*starr->GetAt(0)=="23")) NML1 = 88;

    // check of a correctness of the first face value
	if(NML1 == -1) {
		delete starr;

		return -1;

	}

    // obtaining the second face value
	int NML2 = StrToInt(*starr->GetAt(1));

    // check of a correctness of the second face value
	if((NML2 < 1)||(NML2 > 60)) {
		delete starr;
		return -1;
	}

    // if everything is OK, we return number of a zone
	return NML2 - 30;


}


//===========================================================
//! Calculates a leaf frame in-degree expression
//! on its signature stamp
/** Entrance: 'mas' - the scale, 'str' - the nomenclature
//Exit: 'gradbounds' - a frame in degrees
//Returns a success code
*/

bool CalcMapGradboundsByGrif1(MString mas,MString str, MRect& gradbounds)
//===========================================================
{
    // we will transfer scale to integers
unsigned mastab = atoi((char *)mas);
    //call
	return CalcMapGradboundsByGrif(mastab, str, gradbounds);
}

//===========================================================
//! Calculates a leaf frame in-degree expression
//! on its signature stamp
/** Entrance: 'mas' - the scale, 'str' - the nomenclature
//Exit: 'gradbounds' - a frame in degrees
//Returns a success code
*/
bool CalcMapGradboundsByGrif(unsigned  mastab,MString str, MRect& gradbounds)
//===========================================================
{
    // we break the submitted nomenclature for the sublines
	MStArray* starr = new MStArray();
	MScanfArr(&str,starr,'-');

    // we will transfer scale to integers
    //with simultaneous check of a correctness of dimension of the array
	int result = 0;
	if ((mastab==MASSTAB_AR[MASSTAB_1000000_INDEX])&&(starr->GetSize()==2))	result = 1;
	if ((mastab==MASSTAB_AR[MASSTAB_500000_INDEX])&&(starr->GetSize()==3))	result = 1;
	if ((mastab==MASSTAB_AR[MASSTAB_200000_INDEX])&&(starr->GetSize()==3))	result = 1;
	if ((mastab==MASSTAB_AR[MASSTAB_100000_INDEX])&&(starr->GetSize()==3))	result = 1;
	if ((mastab==MASSTAB_AR[MASSTAB_50000_INDEX])&&(starr->GetSize()==4))	result = 1;
	if ((mastab==MASSTAB_AR[MASSTAB_25000_INDEX])&&(starr->GetSize()==5))	result = 1;
	if ((mastab==MASSTAB_AR[MASSTAB_10000_INDEX])&&(starr->GetSize()==6))	result = 1;

    // if scale is set incorrectly or dimension of the array not
    //it is correct, we calculate nothing
	if(result != 1) {
		delete starr;
		return false;
	}

    // transformation to a digital form of the first face value
	int NML1 = -1;
	if((*starr->GetAt(0)=="A")||(*starr->GetAt(0)=="1"))  NML1 = 0;
	if((*starr->GetAt(0)=="B")||(*starr->GetAt(0)=="2"))  NML1 = 4;
	if((*starr->GetAt(0)=="C")||(*starr->GetAt(0)=="3"))  NML1 = 8;
	if((*starr->GetAt(0)=="D")||(*starr->GetAt(0)=="4"))  NML1 = 12;

	if((*starr->GetAt(0)=="E")||(*starr->GetAt(0)=="5"))  NML1 = 16;
	if((*starr->GetAt(0)=="F")||(*starr->GetAt(0)=="6"))  NML1 = 20;
	if((*starr->GetAt(0)=="G")||(*starr->GetAt(0)=="7"))  NML1 = 24;
	if((*starr->GetAt(0)=="H")||(*starr->GetAt(0)=="8"))  NML1 = 28;
	if((*starr->GetAt(0)=="I")||(*starr->GetAt(0)=="9"))  NML1 = 32;
	if((*starr->GetAt(0)=="J")||(*starr->GetAt(0)=="10")) NML1 = 36;
	if((*starr->GetAt(0)=="K")||(*starr->GetAt(0)=="11")) NML1 = 40;
	if((*starr->GetAt(0)=="L")||(*starr->GetAt(0)=="12")) NML1 = 44;
	if((*starr->GetAt(0)=="M")||(*starr->GetAt(0)=="13")) NML1 = 48;
	if((*starr->GetAt(0)=="N")||(*starr->GetAt(0)=="14")) NML1 = 52;
	if((*starr->GetAt(0)=="O")||(*starr->GetAt(0)=="15")) NML1 = 56;
	if((*starr->GetAt(0)=="P")||(*starr->GetAt(0)=="16")) NML1 = 60;
	if((*starr->GetAt(0)=="Q")||(*starr->GetAt(0)=="17")) NML1 = 64;
	if((*starr->GetAt(0)=="R")||(*starr->GetAt(0)=="18")) NML1 = 68;
	if((*starr->GetAt(0)=="S")||(*starr->GetAt(0)=="19")) NML1 = 72;
	if((*starr->GetAt(0)=="T")||(*starr->GetAt(0)=="20")) NML1 = 76;
	if((*starr->GetAt(0)=="U")||(*starr->GetAt(0)=="21")) NML1 = 80;
	if((*starr->GetAt(0)=="V")||(*starr->GetAt(0)=="22")) NML1 = 84;
	if((*starr->GetAt(0)=="Z")||(*starr->GetAt(0)=="23")) NML1 = 88;

    // check of a correctness of the first face value
	if(NML1 == -1) {
		delete starr;
		return false;
	}

    // obtaining the second face value
	int NML2 = StrToInt(*starr->GetAt(1));

    // check of a correctness of the second face value
	if((NML2 < 1)||(NML2 > 60)) {
		delete starr;
		return false;
	}


    // formation of sheet 1:1 000 000
	gradbounds.top		= NML1 + 4;
	gradbounds.bottom	= NML1;
	if (NML2 <= 30) {
		gradbounds.left		= 180 + NML2*6 - 6;
		gradbounds.right	= 180 + NML2*6;
	}
	else {
		gradbounds.left		= (NML2 - 30)*6 - 6;
		gradbounds.right	= (NML2 - 30)*6;
	}

    // if million scale was ordered,
    //it is possible to come back - the answer is ready
	if (mastab == MASSTAB_AR[MASSTAB_1000000_INDEX]) {

		return true;
	}

    // if the 500th scale was ordered...
	if (mastab == MASSTAB_AR[MASSTAB_500000_INDEX]) {
        // transformation to a digital form of the third face value
		int NML3G = -1;
		int NML3V = -1;
		if((*starr->GetAt(2)=="А")||(*starr->GetAt(2)=="1")) {NML3G = 0; NML3V = 0;}
		if((*starr->GetAt(2)=="Б")||(*starr->GetAt(2)=="2")) {NML3G = 1; NML3V = 0;}
		if((*starr->GetAt(2)=="В")||(*starr->GetAt(2)=="3")) {NML3G = 0; NML3V = 1;}
		if((*starr->GetAt(2)=="Г")||(*starr->GetAt(2)=="4")) {NML3G = 1; NML3V = 1;}

        // check of a correctness of the third face value
		if((NML3G == -1)||(NML3V == -1)) {
			delete starr;
			return false;
		}

		if (NML3G == 0) gradbounds.right	-= 3;

		if (NML3G == 1) gradbounds.left		+= 3;
		if (NML3V == 0) gradbounds.bottom	+= 2;
		if (NML3V == 1) gradbounds.top		-= 2;

        // it is possible to come back - the answer is ready
		delete starr;
		return true;
	}

    // if scale 200 000 was ordered...
	if (mastab == MASSTAB_AR[MASSTAB_200000_INDEX]) {

        // transformation to a digital form of the third face value
		int NML3G = -1;
		int NML3V = -1;
		if((*starr->GetAt(2)=="I")		||(*starr->GetAt(2)=="1"))  {NML3G = 0; NML3V = 0;}
		if((*starr->GetAt(2)=="II")		||(*starr->GetAt(2)=="2"))  {NML3G = 1; NML3V = 0;}
		if((*starr->GetAt(2)=="III")	||(*starr->GetAt(2)=="3"))  {NML3G = 2; NML3V = 0;}
		if((*starr->GetAt(2)=="IV")		||(*starr->GetAt(2)=="4"))  {NML3G = 3; NML3V = 0;}
		if((*starr->GetAt(2)=="V")		||(*starr->GetAt(2)=="5"))  {NML3G = 4; NML3V = 0;}
		if((*starr->GetAt(2)=="VI")		||(*starr->GetAt(2)=="6"))  {NML3G = 5; NML3V = 0;}
		if((*starr->GetAt(2)=="VII")	||(*starr->GetAt(2)=="7"))  {NML3G = 0; NML3V = 1;}
		if((*starr->GetAt(2)=="VIII")	||(*starr->GetAt(2)=="8"))  {NML3G = 1; NML3V = 1;}
		if((*starr->GetAt(2)=="IX")		||(*starr->GetAt(2)=="9"))  {NML3G = 2; NML3V = 1;}
		if((*starr->GetAt(2)=="X")		||(*starr->GetAt(2)=="10")) {NML3G = 3; NML3V = 1;}
		if((*starr->GetAt(2)=="XI")		||(*starr->GetAt(2)=="11")) {NML3G = 4; NML3V = 1;}
		if((*starr->GetAt(2)=="XII")	||(*starr->GetAt(2)=="12")) {NML3G = 5; NML3V = 1;}
		if((*starr->GetAt(2)=="XIII")	||(*starr->GetAt(2)=="13")) {NML3G = 0; NML3V = 2;}
		if((*starr->GetAt(2)=="XIV")	||(*starr->GetAt(2)=="14")) {NML3G = 1; NML3V = 2;}
		if((*starr->GetAt(2)=="XV")		||(*starr->GetAt(2)=="15")) {NML3G = 2; NML3V = 2;}
		if((*starr->GetAt(2)=="XVI")	||(*starr->GetAt(2)=="16")) {NML3G = 3; NML3V = 2;}
		if((*starr->GetAt(2)=="XVII")	||(*starr->GetAt(2)=="17")) {NML3G = 4; NML3V = 2;}
		if((*starr->GetAt(2)=="XVIII")	||(*starr->GetAt(2)=="18")) {NML3G = 5; NML3V = 2;}
		if((*starr->GetAt(2)=="XIX")	||(*starr->GetAt(2)=="19")) {NML3G = 0; NML3V = 3;}
		if((*starr->GetAt(2)=="XX")		||(*starr->GetAt(2)=="20")) {NML3G = 1; NML3V = 3;}
		if((*starr->GetAt(2)=="XXI")	||(*starr->GetAt(2)=="21")) {NML3G = 2; NML3V = 3;}
		if((*starr->GetAt(2)=="XXII")	||(*starr->GetAt(2)=="22")) {NML3G = 3; NML3V = 3;}
		if((*starr->GetAt(2)=="XXIII")	||(*starr->GetAt(2)=="23")) {NML3G = 4; NML3V = 3;}
		if((*starr->GetAt(2)=="XXIV")	||(*starr->GetAt(2)=="24")) {NML3G = 5; NML3V = 3;}
		if((*starr->GetAt(2)=="XXV")	||(*starr->GetAt(2)=="25")) {NML3G = 0; NML3V = 4;}
		if((*starr->GetAt(2)=="XXVI")	||(*starr->GetAt(2)=="26")) {NML3G = 1; NML3V = 4;}
		if((*starr->GetAt(2)=="XXVII")	||(*starr->GetAt(2)=="27")) {NML3G = 2; NML3V = 4;}
		if((*starr->GetAt(2)=="XXVIII")	||(*starr->GetAt(2)=="28")) {NML3G = 3; NML3V = 4;}
		if((*starr->GetAt(2)=="XXIX")	||(*starr->GetAt(2)=="29")) {NML3G = 4; NML3V = 4;}
		if((*starr->GetAt(2)=="XXX")	||(*starr->GetAt(2)=="30")) {NML3G = 5; NML3V = 4;}
		if((*starr->GetAt(2)=="XXXI")	||(*starr->GetAt(2)=="31")) {NML3G = 0; NML3V = 5;}
		if((*starr->GetAt(2)=="XXXII")	||(*starr->GetAt(2)=="32")) {NML3G = 1; NML3V = 5;}
		if((*starr->GetAt(2)=="XXXIII")	||(*starr->GetAt(2)=="33")) {NML3G = 2; NML3V = 5;}
		if((*starr->GetAt(2)=="XXXIV")	||(*starr->GetAt(2)=="34")) {NML3G = 3; NML3V = 5;}
		if((*starr->GetAt(2)=="XXXV")	||(*starr->GetAt(2)=="35")) {NML3G = 4; NML3V = 5;}
		if((*starr->GetAt(2)=="XXXVI")	||(*starr->GetAt(2)=="36")) {NML3G = 5; NML3V = 5;}

        // check of a correctness of the third face value

		if((NML3G == -1)||(NML3V == -1)) {
			delete starr;
			return false;
		}

		gradbounds.left		= gradbounds.left + (double)NML3G;
		gradbounds.right	= gradbounds.left + 1;
		gradbounds.top		= gradbounds.top - (double)NML3V*(4.0/6.0);
		gradbounds.bottom	= gradbounds.top - (4.0/6.0);

        // it is possible to come back - the answer is ready
		delete starr;
		return true;
	}


    // if scale 100 000 was ordered more largely...
	if (mastab <= MASSTAB_AR[MASSTAB_100000_INDEX]) {

        // obtaining the third face value
		int NML3 = StrToInt(*starr->GetAt(2));

        // check of a correctness of the third face value
		if((NML3 <= 1)||(NML3 > 144)) {
			delete starr;
			return false;
		}


		int NML3V = (NML3-1)/12;
		int NML3G = NML3%12;

		if(NML3G==0) NML3G = 12;
		NML3G -=1;

		gradbounds.left		= gradbounds.left + (double)NML3G*0.5;
		gradbounds.right	= gradbounds.left + 0.5;
		gradbounds.top		= gradbounds.top - (double)NML3V*(4.0/12.0);
		gradbounds.bottom	= gradbounds.top - (4.0/12.0);

        // if scale 100 000 was ordered...
        //it is possible to come back - the answer is ready
		if (mastab == MASSTAB_AR[MASSTAB_100000_INDEX]) {
			delete starr;
			return true;
		}


        // if scale 50 000 was ordered more largely...
        if (mastab <= MASSTAB_AR[MASSTAB_50000_INDEX]) {
            // transformation to a digital form of the fourth face value
			int NML4G = -1;
			int NML4V = -1;
			if((*starr->GetAt(3)=="А")||(*starr->GetAt(3)=="A")||(*starr->GetAt(3)=="1")) {NML4G = 0; NML4V = 0;}
			if((*starr->GetAt(3)=="Б")||(*starr->GetAt(3)=="2")) {NML4G = 1; NML4V = 0;}
			if((*starr->GetAt(3)=="В")||(*starr->GetAt(3)=="B")||(*starr->GetAt(3)=="3")) {NML4G = 0; NML4V = 1;}
			if((*starr->GetAt(3)=="Г")||(*starr->GetAt(3)=="4")) {NML4G = 1; NML4V = 1;}

            // check of a correctness of the fourth face value
            if((NML4G == -1)||(NML4V == -1)) {
				delete starr;
				return false;
			}

			if (NML4G == 0) gradbounds.right	-= 0.25;
			if (NML4G == 1) gradbounds.left		+= 0.25;
			if (NML4V == 0) gradbounds.bottom	+= (1.0/6.0);
			if (NML4V == 1) gradbounds.top		-= (1.0/6.0);

            // if scale 50 000 was ordered...
            //it is possible to come back - the answer is ready

			if (mastab == MASSTAB_AR[MASSTAB_50000_INDEX]) {
				delete starr;
				return true;
			}

            // if scale 25 000 was ordered more largely...
            if (mastab <= MASSTAB_AR[MASSTAB_25000_INDEX]) {
                // transformation to a digital form of the fifth  face value
                int NML5G = -1;
				int NML5V = -1;
				if((*starr->GetAt(4)=="а")||(*starr->GetAt(4)=="a")||(*starr->GetAt(4)=="1")) {NML5G = 0; NML5V = 0;}
				if((*starr->GetAt(4)=="б")||(*starr->GetAt(4)=="2")) {NML5G = 1; NML5V = 0;}
				if((*starr->GetAt(4)=="в")||(*starr->GetAt(4)=="3")) {NML5G = 0; NML5V = 1;}
				if((*starr->GetAt(4)=="г")||(*starr->GetAt(4)=="4")) {NML5G = 1; NML5V = 1;}

                // check of a correctness of the fifth face value
                if((NML5G == -1)||(NML5V == -1)) {
					delete starr;
					return false;
				}

				if (NML5G == 0) gradbounds.right	-= 0.125;
				if (NML5G == 1) gradbounds.left		+= 0.125;
				if (NML5V == 0) gradbounds.bottom	+= (1.0/12.0);
				if (NML5V == 1) gradbounds.top		-= (1.0/12.0);

                // if scale 25 000 was ordered...
                //it is possible to come back - the answer is ready

				if (mastab == MASSTAB_AR[MASSTAB_25000_INDEX]) {
					delete starr;
					return true;
				}

                // if scale 10 000 was ordered more largely...
                if (mastab <= MASSTAB_AR[MASSTAB_10000_INDEX]) {
                    // transformation to a digital form of the sixth  face value
                    int NML6G = -1;
					int NML6V = -1;
					if(*starr->GetAt(5)=="1") {NML6G = 0; NML6V = 0;}
					if(*starr->GetAt(5)=="2") {NML6G = 1; NML6V = 0;}
					if(*starr->GetAt(5)=="3") {NML6G = 0; NML6V = 1;}
					if(*starr->GetAt(5)=="4") {NML6G = 1; NML6V = 1;}

                    // check of a correctness of the sixth face value
                    if((NML6G == -1)||(NML6V == -1)) {
						delete starr;
						return false;
					}

					if (NML6G == 0) gradbounds.right	-= (0.125/2.0);
					if (NML6G == 1) gradbounds.left		+= (0.125/2.0);
					if (NML6V == 0) gradbounds.bottom	+= (1.0/24.0);
					if (NML6V == 1) gradbounds.top		-= (1.0/24.0);

                    // if scale 10 000 was ordered...
                    //it is possible to come back - the answer is ready
					if (mastab == MASSTAB_AR[MASSTAB_10000_INDEX]) {
						delete starr;
						return true;
					}


				}//if (mastab <= 10000)


			}//if (mastab <= 25000)

		}//if (mastab <= 50000)

	}//if (mastab <= 100000)

    // sorting
	delete starr;
	return false;
}




//===========================================================
//! Transliteration of the English word in Russian
void Transliteration(MString* str)
//===========================================================
{
return;


}



//===================================================================================================
//!to calculate coordinates of points in the 'respta' array which lie
//! on broken 'kontur' with shifts of 'sdvigarr'
/**if any shift is less than zero, the beginning of broken 'kontur'/will be a resultant point
//if any shift is more than length of a contour, the end of a contour will be a resultant point
//in these cases shifts will be modified in the 'sdvigarr' array (real shifts of points will be brought there)
//all shifts in the 'sdvigarr' array have to represent not decreasing sequence
*/
bool GetPointsBySdvigs(MPtArray* kontur, MDuArray* sdvigarr, MPtArray* respta)
//=====================================================================================
{
	if(!kontur)      return false;   //exit(15);
	if(!sdvigarr)    return false;   //exit(15);
	if(!respta)      return false;   //exit(15);

    //quantity of points in a broken line
	int Cntpt = kontur->GetSize();
	if(Cntpt==0)return false;

    //quantity of resultant points
	int Cnt = sdvigarr->GetSize();
	respta->SetSize(Cnt);

    //the first and last point in a contour
	MPoint firstpt = *kontur->GetAt(0);
	MPoint lastpt = *kontur->GetAt(Cntpt-1);

    //if the quantity of points is equal in a contour 1
    //that at once we calculate result
	if(Cntpt==1)

	{
		double s = 0;
		for(int j=0;j<Cnt;j++)

		{
			sdvigarr->SetAt(j,&s);
			respta->SetAt(j,&firstpt);
		}
		return true;
	}

    //now we will walk on all shifts
	double sdvig;
	int index = 0;
	double summa1 = 0;
	double summa2 = 0;
	MPoint pt;
	MPtArray* pta = new MPtArray();
	pta->SetSize(2);
	for(int i=0;i<Cnt;i++)
	{
        //we will receive the next shift
		sdvig = *sdvigarr->GetAt(i);
        //if shift is less than zero
		if(sdvig<=0)
		{
			sdvig = 0;
			respta->SetAt(i,&firstpt);
			sdvigarr->SetAt(i,&sdvig);
		}	
        //else
		else
		{
            //if the current shift lies on the same segment, as previous
			if(sdvig<=summa2)
			{
				pt = GetPointBySdvig(pta,sdvig-summa1);
				respta->SetAt(i,&pt);
			}
            //otherwise we move further, yet we don't find the necessary segment
			else
			{
                //if there is no place to move already further, we write down the last point
				if(index == Cntpt-1)
				{
					sdvig = summa2;
					respta->SetAt(i,&lastpt);
					sdvigarr->SetAt(i,&sdvig);
				}
                //else

				else
				{
                    //we look for an edge

					while((sdvig>summa2)&&(index<Cntpt-1))
					{
						summa1 = summa2;
						index++;
						pta->SetAt(0,kontur->GetAt(index-1));
						pta->SetAt(1,kontur->GetAt(index));
						summa2 = summa2+DLINASG(*pta->GetAt(0),*pta->GetAt(1));
					}
                    //if nevertheless shift is more, than 'summa2', we came to an end of a broken line
					if(sdvig>summa2)
					{
						sdvig = summa2;
						respta->SetAt(i,&lastpt);
						sdvigarr->SetAt(i,&sdvig);
					}
                    //otherwise we find a point on a contour
					else

					{
						pt = GetPointBySdvig(pta,sdvig-summa1);
						respta->SetAt(i,&pt);
					}
				}
			}
		}
	}
	return true;
}

//==================================
//! Returns distance from the set point to the set line broken line
/** In 'resultpt' places the calculated next point on a broken line.
//In 'nomrebra' places an index of an edge on which the point was found.
*/
double FindDistFromPointToLomline(MPoint& gpt, MPtArray* pLomLine, MPoint& resultpt,int* nomrebra)
//==================================
{
	if(pLomLine->GetSize()==0)
		return -1;

    // for a start we will calculate distance to a zero point of a broken line
	MPoint pt = *pLomLine->GetAt(0);
	resultpt = pt;
	double rassto = gpt.Distance(&pt);
	*nomrebra=0;

	int nPoints = pLomLine->GetSize();

    // if top object from only one point
	if (nPoints<2) {
		resultpt = pt;
		return rassto;
	}

	MPoint pt1,pt2,pt3,pt4;
	double rast;

    // cycle on all segments, on each segment we find the next point
	for (int j=0; j<nPoints-1; j++) {
        // indexes on points of the beginning and end of the checked segment
		pt1 = pLomLine->GetAt(j);
		pt2 = pLomLine->GetAt(j+1);
		pt3 = gpt;
		pt4  = GetPointOnOtrezok(pt1,pt2,pt3);
		rast = pt3.Distance(&pt4);
		if (rast<rassto) {
			rassto = rast;
			resultpt = pt4;
			*nomrebra = j;
		}
	}

	return rassto;
}

//==================================
//! Returns distance from the set point to the set contour
/** In 'resultpt' places the calculated next point on a contour.
//In 'nomrebra' places an index of an edge on which the point was found.
*/
double FindDistFromPointToKontur(MPoint& gpt, MPtArray* pKontur, MPoint& resultpt,int* nomrebra)
//==================================
{
	double rassto = FindDistFromPointToLomline(gpt, pKontur, resultpt, nomrebra);

	MPoint pt1,pt2,pt3,pt4;
	double rast;

    // indexes on points of the beginning and end of the checked segment
	pt1 = pKontur->GetAt(pKontur->GetSize()-1);
	pt2 = pKontur->GetAt(0);
	pt3 = gpt;
	pt4  = GetPointOnOtrezok(pt1,pt2,pt3);
	rast = pt3.Distance(&pt4);
	if (rast<rassto) {
		rassto = rast;
		resultpt = pt4;
		*nomrebra = pKontur->GetSize()-1;
	}

	return rassto;
}

//======================================================================
//!Returns the closest top of a broken line to this point
double DistanceToVershinaOfLomline(MPoint point, MPtArray* pLomline, MPoint& resultpt, int& nomvertex)
//========================================================================
{
	if(!pLomline)
		return -1;

	if(pLomline->GetSize()==0) return -1;

	double rasst = -1;
	double rasst1;
	MPoint pt;
	
    //we will find the next top
	nomvertex = 0;
	resultpt = *pLomline->GetAt(0);
	rasst=point.Distance(&resultpt);
	for(int i=1;i<pLomline->GetSize();i++)
	{
		pt = *pLomline->GetAt(i);
		rasst1 = point.Distance(&pt);
		if(rasst1 < rasst)
		{
			resultpt=pt;
			nomvertex = i;
			rasst = rasst1;
		}
	}
	return rasst;
}



//===============================================================================
//!to find number of an edge of a broken line on which 'point' point lies
/**Entrance characteristics: 'pLomline' - a broken line
//                           'point' - a point
//Output characteristic: number of an edge on which the point, or-1 lies,
//                       if 'pLomline' = NULL,
//                       or quantity of points less than two
//Conditions:
//              1. the point has to lie with guarantee on a contour
//                 (differently the result will be incorrect)
*/
int FindNomRebraOfPointOnLomline(MPtArray* pLomline, MPoint point)

//================================================================================
{
    //we will check a contour task correctness
	if(pLomline == NULL)
	{
		return -1;
	}
	int cnt = pLomline->GetSize();
	if(cnt<2)

	{
		return -1;
	}

    //if a broken line from only two points,
    //that is clear,
    //that the point lies on a zero edge
	if(cnt==2)
	{
		return 0;
	}

    //we will find an edge on which the point lies
	int nrebra = -1;
	double minrasst = -1;
	double rasst;
	MPoint pt1,pt2,pt3;
	for(int i = 0; i<cnt-1;i++)
	{
		pt1 = *pLomline->GetAt(i);
		pt2 = *pLomline->GetAt(i+1);
		pt3 = GetPointOnOtrezok(pt1,pt2,point);
		rasst = DLINASG(point,pt3);
		if((rasst<minrasst)||(minrasst==-1))
		{
			minrasst = rasst;

			nrebra = i;
		}
	}

	return nrebra;
}

//=====================================================================
//!to find a broken line top index, in on which the point lies
/**Entrance data: 'pLomline' - a broken line
//                'point' - a point
//Result:
//         number of top at which the point lies,
//         or-1 if the point doesn't lie at one top
//Conditions:
//             1. the point has to lie with guarantee on a broken line
*/
int FindNomVershinaOfPointOnLomline(MPtArray* pLomline, MPoint point)
//=====================================================================
{
	if(pLomline == NULL)
		return -1;

	int cnt = pLomline->GetSize();

	if(cnt==0)
		return -1;

	for(int i=0; i<cnt;i++)
	{
		if(point==*pLomline->GetAt(i))
			return i;
	}


	return -1;
}

//=======================================================
//!function returns height of the point located with shift of 'sdvig' from the beginning of a profile
/** 1. if the profile doesn't consist of one point, comes back 0
// 2. if shift coincides with a place of vertical difference of a profile, the most right comes back
// 3. height is defined proceeding from the following scheme
//
//
//                                            *--------
//                                            |
//               *                            |
//             /   \                *---------*
//           /       \            /    
//  -------*           \        /
//                       *-----*
//
//  - - interpretation of a profile
//  * - points of the array of a profile
// 
//'pProfil' - the array of a profile something. ('x' - shift from the beginning, 'y' - height)
//points in a profile have to be located in not decreasing order of coordinate 'x'
*/
double FindVisotaBySdvig(MPtArray* pProfil,double sdvig)
//=======================================================
{
    //if the profile isn't present
	if(!pProfil) return 0;	

    //profile size
	int Cnt = pProfil->GetSize();
	
    //if profile empty
	if(Cnt==0) return 0;

    //auxiliary point
    MPoint* ppt,*ppt1;
	
	//!!!!!!!!!
    //for performance of a condition 2
    //it is necessary to check extreme points
    //in such sequence:
 //at first on the right border, and then on the left

 //if the point lies more to the right of the last point on a profile
	ppt = pProfil->GetAt(Cnt-1);
	if(sdvig>=ppt->x) return ppt->y;


    //if the point lies more to the left of the first point of a profile
	ppt = pProfil->GetAt(0);
	if(sdvig<=ppt->x) return ppt->y;

	
    //if we didn't find height yet, we look for the range of shifts
	
    int il = 0; //left border of range
    int ir = Cnt-1;//right border of range
    int index; //current index
	
    //so far range isn't equal 1
	while(ir-il>1)
	{
        //middle of range
		index = (ir+il)/2;
		ppt = pProfil->GetAt(index);
        //if shift is more left than the middle
		if(sdvig<ppt->x)
			ir = index;
        //if shift is more right than the middle
		else if(sdvig>ppt->x)
			il = index;
        //if shift got on the middle
		else 
			il = ir = index;
	}

    //now we will carry out the analysis

    //if indexes aren't equal
	if(il!=ir)
	{
        // linear interpolation
		ppt = pProfil->GetAt(il);
		ppt1 = pProfil->GetAt(ir);
		return ppt->y+(ppt1->y-ppt->y)*(sdvig-ppt->x)/(ppt1->x-ppt->x);
	}
    //if indexes are equal, perhaps, we got on a vertical site
	else
	{
		ppt = pProfil->GetAt(ir);
		while(ppt->x==sdvig)
		{
			ir++;
			ppt = pProfil->GetAt(ir);
		}
		ir--;
		ppt = pProfil->GetAt(ir);
		return ppt->y;
	}
}

//==============================================================================
//!distance from 'point' point to a segment [pt1, pt2]
double RASSTPTSG(MPoint* point, MPoint* pt1, MPoint* pt2)
//==============================================================================
{
	double rasstoanie;

	MPoint pt_sg;
    //we will receive a perpendicular point of intersection from 'point' on
    //segment [pt1, pt2]; if the received point doesn't lie
    //on a segment, we return the closest end of a segment
	pt_sg = GetPointOnOtrezok(pt1, pt2, point);
    //length from point to a pochenny point
	rasstoanie = DLINASG(pt_sg, *point);


	return rasstoanie;
}

//=========================================================
//!to establish in 'Byte' byte of bits number nomer in value value
void SetBitInByte(unsigned char& Byte, int nomer, bool value)
//=========================================================
{
    //if number of bit doesn't get to range 0..7

	if((nomer<0)||(nomer>7))return;
    //if it is necessary to establish bit in unit
	unsigned char b;
	if(value)
	{
		switch (nomer)
		{
		case 0:
			b = 0x01;
			break;
		case 1:

			b = 0x02;
			break;
		case 2:
			b = 0x04;
			break;
		case 3:
			b = 0x08;
			break;
		case 4:
			b = 0x10;
			break;
		case 5:
			b = 0x20;
			break;
		case 6:
			b = 0x40;
			break;
		case 7:
			b = 0x80;
			break;
		}
		Byte = Byte|b;
	}
    //if it is necessary to establish bit in zero
	else
	{
		switch (nomer)
		{
		case 0:
			b = 0xFE;
			break;
		case 1:
			b = 0xFD;
			break;
		case 2:
			b = 0xFB;
			break;
		case 3:
			b = 0xF7;
			break;
		case 4:
			b = 0xEF;
			break;
		case 5:
			b = 0xDF;
			break;
		case 6:
			b = 0xBF;
			break;
		case 7:
			b = 0x7E;
			break;
		}
		Byte = Byte&b;
	}
}

//=========================================================
//!to give out value of bit number 'nomer' in 'Byte' byte
bool GetBitInByte(unsigned char Byte, int nomer)
//=========================================================
{
	unsigned char b;
	switch(nomer)
	{
		case 0:
			b = 0x01;
			break;
		case 1:
			b = 0x02;
			break;
		case 2:
			b = 0x04;
			break;
		case 3:
			b = 0x08;
			break;
		case 4:
			b = 0x10;
			break;
		case 5:
			b = 0x20;
			break;
		case 6:
			b = 0x40;
			break;
		case 7:
			b = 0x80;
			break;
		default:
			return false;
	}
	if((Byte&b)==b)
		return true;
	else 

		return false;
}



// 'RGBMAX', 'HLSMAX' have to occupy 1 byte.
#define  HLSMAX   360  // that shared totally on 6
#define  RGBMAX   255   
// Color tone isn't defined if a saturation = 0 (a gray scale)
//for calculation of provision of a scrollbar in this case:
#define UNDEFINED_HUE (HLSMAX*2/3) 
//=========================================================
//! Color RGB transformation to HLS
bool  RGBtoHLS(int R,int G,int B, int& H,int& L,int& S)
//=========================================================
{
// act layer -> nothing to do
if (drawActFlag) {
  H= R;  L= G;  S= B;
  return true;
}    


   unsigned char   cMax,cMin;				// maximum and minimum of the sizes RGB
   unsigned short  Rdelta,Gdelta,Bdelta;	// intermediate size: % differences from a maximum
    
   // brightness
   cMax = Max( Max(R,G), B);
   cMin = Min( Min(R,G), B);
   L = ( ((cMax+cMin)*HLSMAX) + RGBMAX )/(2*RGBMAX);

    // achromatic case
   if (cMax == cMin) {           // r=g=b --> achromatic case
      S = 0;                     // saturation
      H = UNDEFINED_HUE;             // Color tone
	  return false;
   }
   //  general case
   else {                         
      // saturation
      if (L <= (HLSMAX/2))	S = ( ((cMax-cMin)*HLSMAX) + ((cMax+cMin)/2) ) / (cMax+cMin);
      else					S = ( ((cMax-cMin)*HLSMAX) + ((2*RGBMAX-cMax-cMin)/2)) / (2*RGBMAX-cMax-cMin);

      // Color tone
	  Rdelta = ( ((cMax-R)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
	  Gdelta = ( ((cMax-G)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);
	  Bdelta = ( ((cMax-B)*(HLSMAX/6)) + ((cMax-cMin)/2) ) / (cMax-cMin);

      if (R == cMax)		H = Bdelta - Gdelta;
      else if (G == cMax)	H = (HLSMAX/3) + Rdelta - Bdelta;
      else					H = ((2*HLSMAX)/3) + Gdelta - Rdelta;// B == cMax  

      if (H < 0)		H += HLSMAX;
      if (H > HLSMAX)	H -= HLSMAX;
	  return true;
   }
}
//=========================================================
//!The office utility for HLStoRGB
unsigned short HueToRGB(int n1,int n2,int hue)
//=========================================================
{
   // range check: note values passed add/subtract thirds of range  
   if (hue < 0) hue += HLSMAX;
   if (hue > HLSMAX) hue -= HLSMAX;

   // return r,g, or b value from this tridrant  
   if (hue < (HLSMAX/6))		return ( n1 + (((n2-n1)*hue+(HLSMAX/12))/(HLSMAX/6)) );
   if (hue < (HLSMAX/2))		return ( n2 );
   if (hue < ((HLSMAX*2)/3))	return ( n1 +    (((n2-n1)*(((HLSMAX*2)/3)-hue)+(HLSMAX/12))/(HLSMAX/6)));
   else							return ( n1 );
}

//=========================================================
//! Color HLS transformation to RGB
bool HLStoRGB(int H,int L,int S, int& R,int& G,int& B)
//=========================================================
{
// act layer -> nothing to do
if (drawActFlag) {
  R= H;  G= L;  B= S;
  return true;
}

  unsigned short  Magic1,Magic2;       // the calculated magic numbers (sic!)

    // achromatic case
    if (S == 0) {            
      R=G=B=(L*RGBMAX)/HLSMAX;
      if (H != UNDEFINED_HUE) {
         return false;  

       }
    }

    // general case
	else  {                    
      // we will calculate magic numbers
      if (L <= (HLSMAX/2))	Magic2 = (L*(HLSMAX + S) + (HLSMAX/2))/HLSMAX;
      else						Magic2 = L + S - ((L*S) + (HLSMAX/2))/HLSMAX;
      Magic1 = 2*L-Magic2;

      // we receive RGB, we change norm from HLSMAX for RGBMAX
      R = (HueToRGB(Magic1,Magic2,H+(HLSMAX/3))*RGBMAX + (HLSMAX/2))/HLSMAX;
      G = (HueToRGB(Magic1,Magic2,H)*RGBMAX + (HLSMAX/2)) / HLSMAX;
      B = (HueToRGB(Magic1,Magic2,H-(HLSMAX/3))*RGBMAX + (HLSMAX/2))/HLSMAX;
    }

	return true;
} 


//=========================================================
//! Shift of RGB-color on a HLS-Scale
void SdvigRGBbyHLS(int R,int G,int B, int dH,int dL,int dS, int& nR,int& nG,int& nB)
//=========================================================
{
	int H,L,S;

	RGBtoHLS(R,G,B,H,L,S);


	H +=dH;
	if (dL>=0) L += (int) ( (double)(HLSMAX-L)*((double)dL/(double)HLSMAX) );
	if (dL< 0) L += (int) ( (double)L*((double)dL/(double)HLSMAX) );
	if (dS>=0) S += (int) ( (double)(HLSMAX-S)*((double)dS/(double)HLSMAX) ) ;
	if (dS< 0) S += (int) ( (double)S*((double)dS/(double)HLSMAX) );

	if (H>HLSMAX) H = H - HLSMAX;
	if (L>HLSMAX) L = HLSMAX;
	if (S>HLSMAX) S = HLSMAX;

	if (H<0) H = HLSMAX - H;
	if (L<0) L = 0;
	if (S<0) S = 0;

	HLStoRGB(H,L,S,nR,nG,nB);
}


//=========================================================
//!=========================================================
bool IS_VNUTRI_STROBA(MPtArray* pta1,MPoint* pt1, MPoint* pt2, double dopusk) 
{ 
    // we will check: whether the top object point gets to a link strobe [pt1, pt2]
    bool booolean = false;

	int size = pta1->GetSize();
	for (int i=0; i<size; i++)
	{
		if(RASSTPTSG(pta1->GetAt(i), pt1, pt2)<=dopusk)
		{
           booolean = true;
		   break;
		}

	}
    // we will check: whether the broken line link crosses top object
    if(size>1)
	{
	   for (int k=0; k<size-1; k++)

	   {
			MPoint pt;
			if(CROSSSECTION(pta1->GetAt(k), pta1->GetAt(k+1), pt1, pt2, &pt))
			{
			   booolean = true;
			   break;
			}
	   }
	}

	return booolean;
}
//////////////////////////////////////////////////////////////////////////////////////
//!   Function returns distance between two points
//!   on an ellipsoid along the geodetic
double GetRasst(double poluos1,double poluos2,Coord shir1,Coord shir2,Coord dolg1,Coord dolg2)
//////////////////////////////////////////////////////////////////////////
{

  poluos1 = axis_X;
  poluos2 = axis_Y;
    //  finding of distance between two points
    //  along the geodetic
    //  Danye: small and big half shafts of an ellipsoid,
    //  width longitude of each point.
    //  It is a so-called return geodetic task.

    //  big half shaft of 'poluos1'
    //  small half shaft of 'poluos2'

    //  the first parameter in 'Coord' structure is a sign of number
    //  it can be 1, if number positive, and -1, if
    //  negative number

	// B1
	double grad1   = shir1.grad*shir1.znak;
	double min1    = shir1.min*shir1.znak;
	double sec1    = shir1.sec*shir1.znak;


	// B2
	double grad2   = shir2.grad*shir2.znak;
	double min2    = shir2.min*shir2.znak;
	double sec2    = shir2.sec*shir2.znak;

	// L1
	double grad3   = dolg1.grad*dolg1.znak;
	double min3    = dolg1.min*dolg1.znak;
	double sec3    = dolg1.sec*dolg1.znak;

	// L2
	double grad4   = dolg2.grad*dolg2.znak;
	double min4    = dolg2.min*dolg2.znak;
	double sec4    = dolg2.sec*dolg2.znak;

    // compression coefficient (polar compression)
////	double sjatie = (poluos1-poluos2)/poluos1;
    //  square of the first eccentricity
	double sqrte1 = (poluos1*poluos1-poluos2*poluos2)/(poluos1*poluos1);
    //  square of the second eccentricity
	double sqrte2 = (poluos1*poluos1-poluos2*poluos2)/(poluos2*poluos2);
    //  p2shtrih - middle latitude of an arch
////	double p2shtrih = 1/(4848137*pow(10,-12)); 

    ///////////////////////////////////////////////////////
    //  calculation of a middle 'tgBsredn' latitude
	///////////////////////////////////////////////////////

	//double pi = 4*atan(1);

	double B1 = (grad1+min1/60+sec1/3600)*2*pi/360;
	double B2 = (grad2+min2/60+sec2/3600)*2*pi/360;
////	double L1 = (grad1+min1/60+sec1/3600)*2*pi/360;
////	double L2 = (grad2+min2/60+sec2/3600)*2*pi/360;

	double tgB1=0.0, tgB2=0.0;

	if (B1!=0 && B2!=0){
        tgB1 = sin(B1)/cos(B1);
        tgB2 = sin(B2)/cos(B2);
	}
	double l = fabs(grad4-grad3+(min4-min3)/60+(sec4-sec3)/3600)*2*pi/360;
	double Coslna2 = cos(fabs(grad4-grad3+(min4-min3)/60+(sec4-sec3)/3600)*pi/360);
    double tgBsredn = (tgB1+tgB2)/(2*Coslna2);

    ///////////////////////////////////////////////////////
    //  calculation of the spheroidal 'Vsredn' function
	///////////////////////////////////////////////////////

    double cos2Bsr = 1/(1+pow(tgBsredn,2));
	double Vsredn = (4.0018 + 3*sqrte2*cos2Bsr)/(4.0018 + sqrte2*cos2Bsr);

    ///////////////////////////////////////////////////////
    //  calculation of the spheroidal 'Vi' functions
	///////////////////////////////////////////////////////

	double V1 = (4.0018 + 3*sqrte2*pow(cos(B1),2))/(4.0018 + sqrte2*pow(cos(B1),2));  
	double V2 = (4.0018 + 3*sqrte2*pow(cos(B2),2))/(4.0018 + sqrte2*pow(cos(B2),2)); 
	
    ///////////////////////////////////////////////////////
    //  calculation of 'V'
	///////////////////////////////////////////////////////

////    double V = (V1-4*Vsredn+V2)/6;

    ///////////////////////////////////////////////////////
    //  calculation of a spherical difference of longitudes of 'delta1'
    //in the second approach
	///////////////////////////////////////////////////////


    double delta1 = Vsredn*l;

    ///////////////////////////////////////////////////////
    //  calculation of a spherical difference of longitudes of delta2
    //as a first approximation
	///////////////////////////////////////////////////////

	double cosp = (sin(B1)*sin(B2) + cos(B1)*cos(B2)/sqrt(1-pow(sqrte1,2)) * cos(delta1))/V1/V2;
	double p = acos(cosp); 
	double sinq = sin(delta1)*cos(B1)*cos(B2)/sin(p)/V1/V2; 
    double cosdel2 = cos(l+0.003351*p*sinq);
    double sindel2 = sin(l+0.003351*p*sinq);
    double del2 = l+0.003351*p*sinq;
    ///////////////////////////////////////////////////////
    //  calculation of 'Bm'
	///////////////////////////////////////////////////////

	double f1 = cos(B1)*cos(B2)*sindel2;
    double f2 = sqrt(1-pow(cos(B1)*cos(B2)*cosdel2 + sin(B1)*sin(B2),2));
	double cosBm = f1/f2;
	double Bm = acos(cosBm);
	double sinBm = sin(Bm);


    ///////////////////////////////////////////////////////

    //  calculation of 's12shtrih' distance
	///////////////////////////////////////////////////////

    double k2 = sqrte1*pow(sinBm,2);
    double Wm = (1-0.75*k2)/(1-0.25*k2);
    double sinfi1 = sin (B1)/sinBm;
    double sinfi2 = sin (B2)/sinBm;
    double fi1 = asin(sinfi1);
    double fi2 = asin(sinfi2);
////	double tanfi1 = tan(fi1);//
    double qw = sqrt(1-1.25*k2);

	double a1 = atan(sqrt(1-1.25*k2)*tan(fi1));
	double a2 = atan(sqrt(1-1.25*k2)*tan(fi2));
    // for debugging
////	double tana1 = sqrt(1-1.25*k2)*tan(fi1);

//    double a1n = (-73-57/60-50.96/3600)*2*pi/360;
//    double a2n = (-47-24/60-11.11/3600)*2*pi/360;
////	double asd = a1n-a2n;
	double f76 = (a1-a2)*1.2/sqrt(1-1.25*k2);
    double f77 = f76+0.2*(fi2-fi1);//not precisely f76
    // f78 true f77 false
	double f78 = poluos2*Wm;
	double f79 = f78*f77;
	double s12shtrih = fabs(f79);

    ///////////////////////////////////////////////////////
    //  calculation of 'del3' distance
	///////////////////////////////////////////////////////

	double del3 = acos(tan(B2)/tan(Bm))-acos(tan(B1)/tan(Bm));
    // 'del' for debugging: the following values are taken from the table
    // 'del' has to coincide with 'del3'
////    double del = (30+14/60+58.34/3600)*2*pi/360;
    ///////////////////////////////////////////////////////
    //  calculation of 's01' distance and 's02'
    //  distance
	///////////////////////////////////////////////////////

	double s01 = poluos2*Wm*(0.2*(fabs(fi1)-pi/2) + 1.2/qw*(pi/2-fabs(a1)));
	double s02 = poluos2*Wm*(0.2*(pi/2-fabs(fi2)) + 1.2/qw*(fabs(a2)-pi/2));

    ///////////////////////////////////////////////////////

    //  calculation of 's12' distance
	///////////////////////////////////////////////////////

	double s12= 0.0;
	if(fabs(fabs(del3)-fabs(del2))<0.01*2*pi/360) s12 = s12shtrih;
	if(fabs(fabs(del3)-fabs(del2))>0.01*2*pi/360 && B1>B2) s12 = s12shtrih+2*s01;
	if(fabs(fabs(del3)-fabs(del2))>0.01*2*pi/360 && B1<B2) s12 = s12shtrih+2*s02;

	return s12;

}
////////////////////////////////////////////////////////////////////////////////////////
//! Function returns
//! an azimuth of the direction (Q1 Q2) in radians.
//! if two points on an ellipsoid are given.
double GetAzimut(double poluos1,double poluos2,Coord shir1,Coord shir2,Coord dolg1,Coord dolg2)
////////////////////////////////////////////////////////////////////////////////////////
{
  poluos1 = axis_X;
  poluos2 = axis_Y;
  
    //  finding of an azimuth between two points
    //  along the geodetic
    //  Data: small and big half shafts of an ellipsoid,
    //  width longitude of each point.
    //  It is a so-called return geodetic task.

    //  big half shaft of 'poluos1'
    //  small half shaft of 'poluos2'

    //  the first parameter in 'Coord' structure is a sign of number
    //  it can be 1, if number positive, and-1, if
    //  negative number

	// B1
	double grad1   = shir1.grad*shir1.znak;
	double min1    = shir1.min*shir1.znak;
	double sec1    = shir1.sec*shir1.znak;

	// B2
	double grad2   = shir2.grad*shir2.znak;
	double min2    = shir2.min*shir2.znak;
	double sec2    = shir2.sec*shir2.znak;

	// L1
	double grad3   = dolg1.grad*dolg1.znak;
	double min3    = dolg1.min*dolg1.znak;
	double sec3    = dolg1.sec*dolg1.znak;

	// L2
	double grad4   = dolg2.grad*dolg2.znak;
	double min4    = dolg2.min*dolg2.znak;
	double sec4    = dolg2.sec*dolg2.znak;

    //  compression coefficient (polar compression)
////	double sjatie = (poluos1-poluos2)/poluos1;
    //  square of the first eccentricity
	double sqrte1 = (poluos1*poluos1-poluos2*poluos2)/(poluos1*poluos1);
    //  square of the second eccentricity
	double sqrte2 = (poluos1*poluos1-poluos2*poluos2)/(poluos2*poluos2);
    //  p2shtrih - middle latitude of an arch
////	double p2shtrih = 1/(4848137*pow(10,-12)); 

    ///////////////////////////////////////////////////////
    //  'p2shtrih' - middle latitude of an arch
	///////////////////////////////////////////////////////

	//double pi = 4*atan(1);

	double B1 = (grad1+min1/60+sec1/3600)*2*pi/360;
	double B2 = (grad2+min2/60+sec2/3600)*2*pi/360;
////	double L1 = (grad1+min1/60+sec1/3600)*2*pi/360;
////	double L2 = (grad2+min2/60+sec2/3600)*2*pi/360;

	double tgB1=0.0, tgB2=0.0;
	if (B1!=0 && B2!=0){
        tgB1 = sin(B1)/cos(B1);
        tgB2 = sin(B2)/cos(B2);
	}
	double l = fabs(grad4-grad3+(min4-min3)/60+(sec4-sec3)/3600)*2*pi/360;
	double Coslna2 = cos(fabs(grad4-grad3+(min4-min3)/60+(sec4-sec3)/3600)*pi/360);
    double tgBsredn = (tgB1+tgB2)/(2*Coslna2);

    ///////////////////////////////////////////////////////
    //  calculation of the spheroidal 'Vsredn' function
	///////////////////////////////////////////////////////

    double cos2Bsr = 1/(1+pow(tgBsredn,2));
	double Vsredn = (4.0018 + 3*sqrte2*cos2Bsr)/(4.0018 + sqrte2*cos2Bsr);

    ///////////////////////////////////////////////////////
    //  calculation of the spheroidal 'Vi' functions
	///////////////////////////////////////////////////////

	double V1 = (4.0018 + 3*sqrte2*pow(cos(B1),2))/(4.0018 + sqrte2*pow(cos(B1),2));  
	double V2 = (4.0018 + 3*sqrte2*pow(cos(B2),2))/(4.0018 + sqrte2*pow(cos(B2),2)); 
	
    ///////////////////////////////////////////////////////
    //  calculation of  'V'

	///////////////////////////////////////////////////////

////    double V = (V1-4*Vsredn+V2)/6;

    ///////////////////////////////////////////////////////
    //  calculation of a spherical difference of longitudes of 'delta1'
    //in the second approach
	///////////////////////////////////////////////////////

    double delta1 = Vsredn*l;

    ///////////////////////////////////////////////////////
    //  calculation of a spherical difference of longitudes of 'delta2'
    //in the second approach
	///////////////////////////////////////////////////////

	double cosp = (sin(B1)*sin(B2) + cos(B1)*cos(B2)/sqrt(1-pow(sqrte1,2)) * cos(delta1))/V1/V2;
	double p = acos(cosp); 
	double sinq = sin(delta1)*cos(B1)*cos(B2)/sin(p)/V1/V2; 
    double cosdel2 = cos(l+0.003351*p*sinq);
    double sindel2 = sin(l+0.003351*p*sinq);
////    double del2 = l+0.003351*p*sinq;
    ///////////////////////////////////////////////////////
    //  calculation of  'Bm'
	///////////////////////////////////////////////////////

	double f1 = cos(B1)*cos(B2)*sindel2;
    double f2 = sqrt(1-pow(cos(B1)*cos(B2)*cosdel2 + sin(B1)*sin(B2),2));
	double cosBm = f1/f2;
	double Bm = acos(cosBm);
//	double sinBm = sin(Bm);

    ///////////////////////////////////////////////////////
    //  calculation of 's12shtrih' distance
	///////////////////////////////////////////////////////

//    double k2 = sqrte1*pow(sinBm,2);
////    double Wm = (1-0.75*k2)/(1-0.25*k2);
	double Vm = (4.0018+3*sqrte2*pow(cos(Bm),2))/(4.0018+sqrte2*pow(cos(Bm),2));
    double A12 = asin(cos(Bm)/Vm/(cos(B1)/V1)); 
	return A12;
}

//===================================================================
//! Creation of the line (contour) passing at some ordered distance
//! from the set contour.
/** NB: the sign of zamknut indicates the need to construct the CLOSED contour.
//NB: if the distance zero, function simply copies the initial array.
//NB: RELEASE OF THE MEMORY OCCUPIED UNDER THE array LIES ON CONSCIENCE OF DEFIANT FUNCTION!!!
*/
MPtArray* CreateDistContur(MPtArray* pta, double distofaxe, bool zamknut)
//===================================================================
{
    // ptar - axial
    // ptarr - cover
	MPtArray* ptar=(MPtArray*)pta->Clone();
	MPtArray* ptarr;

    //if distance from an axis zero, simply we copy the initial array
	if(distofaxe==0)
	{
		ptarr=(MPtArray*)pta->Clone();
        //if it is required to construct the closed contour, it is necessary to close still
		if(zamknut)	ptarr->AddTail(pta->GetAt(0));
	}
    //otherwise we build the bypassing contour
	else
	{
		

		ptarr=new MPtArray();
		MPoint* pt1,*pt2,*pt3 =NULL;

		//******************************************************
        //CLEANING OF THE INITIAL array
		//******************************************************
        //we will clean the array so that there were no in a row points equal
        //or three points lying on one straight line in a row
        //cleaning happens in spetsilno the array formed for this purpose
        //the initial array remains invariable


        //CLEANING OF COINCIDING POINTS
        //AND THE THREE OF THE POINTS LYING ON ONE STRAIGHT LINE
		int ii=0;
        //we pass all array, since a zero point
		while(ii<ptar->GetSize()-1)
		{
			if(ptar->GetSize()==1)break;
            //we receive the first two points
			pt1=ptar->GetAt(ii);
			pt2=ptar->GetAt(ii+1);
            //if the first two points are equal, we delete the second
			if((fabs(pt1->x-pt2->x)<0.01)&&(fabs(pt1->y-pt2->y)<0.01))
			{
				ptar->Delete(ii+1);
			}
            //otherwise if it is a penultimate point, we pass to the last
			else if(ii==ptar->GetSize()-2)
			{
				ii++;
			}
            //otherwise we take the third point
			else
			{
				pt3=ptar->GetAt(ii+2);
                //if three points lie on one straight line, we delete average
				if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.0001)
				{
					ptar->Delete(ii+1);
				}
                //otherwise we pass to the subsequent point
				else
				{
					ii++;
				}
			}
		}

        // IN ADDITION FOR THE CLOSED CONTOUR
        //if at us closed top object, we check still the three
        //points: (n-2, n-1,0) and (n-1,0,1)
		if(zamknut)
		{
			bool bb=true;
            //we check the first three
			while(bb)
			{
                //if it is more than two points
				if(ptar->GetSize()>2)
				{
                    //we receive points
					pt1=ptar->GetAt(ptar->GetSize()-2);
					pt2=ptar->GetAt(ptar->GetSize()-1);
					pt3=ptar->GetAt(0);
                    //if the tail and the head are equal, we delete a tail
					if((fabs(pt2->x-pt3->x)<0.01)&&(fabs(pt2->y-pt3->y)<0.01))
					{
						ptar->DeleteTail();
					}
                    //else
					else
					{
                        //if three points on one straight line, we delete a tail
						if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.001)
						{
							ptar->DeleteTail();
						}
                        //everything is otherwise normal
						else
						{
							bb=false;
						}
					}
				}
                //if it is less than three points, we check nothing
				else
				{
					bb=false;
				}
			}

			bb=true;
            //we check the second three
			while(bb)
			{
                //if it is more than two points
                if(ptar->GetSize()>2)
				{
                    //we receive points
                    pt1=ptar->GetAt(ptar->GetSize()-1);
					pt2=ptar->GetAt(0);
					pt3=ptar->GetAt(1);
                    //if the tail and the head are equal, we delete a tail
                    if((fabs(pt2->x-pt1->x)<0.01)&&(fabs(pt1->y-pt2->y)<0.01))
					{
						ptar->DeleteTail();
					}
                    //else
					else
					{
                        //if three points on one straight line, we delete a head
                        if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.001)
						{
							ptar->DeleteHead();
						}
                        //everything is otherwise normal
                        else
						{
							bb=false;
						}
					}
				}
                //if it is less than three points, we check nothing
                else
				{
					bb=false;
				}
			}

		}
		
		//******************************************************
        // CREATION OF THE LEADING ROUND CONTOUR
		//******************************************************
		
        //we build the bypassing contour

		double A1,B1,C1,A2,B2,C2;
		int m_nPoints=ptar->GetSize();
        //at once we take away the necessary quantity of elements for the bypassing contour
		if((m_nPoints==2)||(!zamknut))
			ptarr->SetSize(m_nPoints);

		else
			ptarr->SetSize(m_nPoints+1);

        //if we have only two points, build the segment parallel to the initial
		if(m_nPoints==2)
		{
			pt1=ptar->GetAt(0);
			pt2=ptar->GetAt(1);
      MPoint pnt1=  GetPerpendikular(pt1[0],pt2[0],pt1[0],distofaxe);
			ptarr->SetAt(0,&pnt1);
      MPoint pnt2=  GetPerpendikular(pt1[0],pt2[0],pt2[0],distofaxe);
			ptarr->SetAt(1,&pnt2);

		}
        //otherwise we find the covering contour
		else
		{
            // if the contour which isn't closed, the first point lies simply on a perpendicular
			if(!zamknut)
			{
				pt1=ptar->GetAt(0);
				pt2=ptar->GetAt(1);
        MPoint pnt= GetPerpendikular(pt1[0],pt2[0],pt1[0],distofaxe);
				ptarr->SetAt(0,&pnt);
			}
            //further from zero to a point, the second since the end, we build contour points on crossings of straight lines
			pt1=ptar->GetAt(0);
			pt2=ptar->GetAt(1);
			GetPramuju(pt1[0],pt2[0],distofaxe,A1,B1,C1);
			double dlx,dly,dl;
			MPoint pt;
			for(int kk=0;kk<ptar->GetSize()-2;kk++)
			{
                //we receive three points
				pt1=ptar->GetAt(kk);
				pt2=ptar->GetAt(kk+1);
				pt3=ptar->GetAt(kk+2);
                //we find a straight line of the second couple of points
				GetPramuju(pt2[0],pt3[0],distofaxe,A2,B2,C2);
                //we find crossing of two straight lines
				dlx=B1*C2-B2*C1;
				dly=A2*C1-A1*C2;
				dl=A1*B2-A2*B1;
				pt.x=dlx/dl;
				pt.y=dly/dl;
				ptarr->SetAt(kk+1,&pt);
                //on the following step the equation of the first straight line will be
                //same, as well as the equation of the second straight line on this step
				A1=A2;
				B1=B2;
				C1=C2;
			}
            // if the contour which isn't closed, the last point, as well as the first,

            //lies simply on a perpendicular
			if(!zamknut)
			{
        MPoint pnt= GetPerpendikular(pt2[0],pt3[0],pt3[0],distofaxe);
				ptarr->SetAt(ptar->GetSize()-1,&pnt);
			}
            // if the contour closed, we finish finishing a contour
			else
			{
				pt1=ptar->GetAt(m_nPoints-2);
				pt2=ptar->GetAt(m_nPoints-1);
				pt3=ptar->GetAt(0);

                //three n-2,n-1,0
				GetPramuju(pt1[0],pt2[0],distofaxe,A1,B1,C1);
				GetPramuju(pt2[0],pt3[0],distofaxe,A2,B2,C2);
                //we find crossing of two straight lines
				dlx=B1*C2-B2*C1;
				dly=A2*C1-A1*C2;
				dl=A1*B2-A2*B1;
				pt.x=dlx/dl;
				pt.y=dly/dl;
				ptarr->SetAt(ptar->GetSize()-1,&pt);

                //three n-1,0,1
				pt1=ptar->GetAt(1);
				GetPramuju(pt2[0],pt3[0],distofaxe,A1,B1,C1);
				GetPramuju(pt3[0],pt1[0],distofaxe,A2,B2,C2);
                //we find crossing of two straight lines
				dlx=B1*C2-B2*C1;
				dly=A2*C1-A1*C2;
				dl=A1*B2-A2*B1;
				pt.x=dlx/dl;
				pt.y=dly/dl;
				ptarr->SetAt(0,&pt);

                //we close arrays
				pt1=ptarr->GetAt(0);
				pt.x=pt1->x;
				pt.y=pt1->y;
				ptarr->SetAt(ptarr->GetSize()-1,&pt);
				pt1=ptar->GetAt(0);
				pt.x=pt1->x;
				pt.y=pt1->y;
				ptar->AddTail(&pt);
			}
		}
		
	}


	delete ptar;
	return ptarr;

}

//===================================================================
//! inversion of an order of points in the array of points (contour)
bool InverseContur(MPtArray* pta)
//===================================================================
{
	if(pta==NULL) return false;
 
	int	kol = pta->GetSize();
	for (int i=0; i<(kol/2); i++) pta->Exchange(i,kol - i - 1);

	return true;
}


//===================================================================
//! Creation of the polygon approximating a circle
//! with the set center and radius
/** NB: 'grad' - the parameter which is giving a task through to mold how many degrees the next point
//NB: RELEASE OF THE MEMORY OCCUPIED UNDER THE array LIES ON CONSCIENCE OF DEFIANT FUNCTION!!!
*/
MPtArray* CreateCircle(MPoint* pt, double radius, double grad)
//===================================================================
{

	if(radius==0) return NULL;

	MPtArray* ptarr = new MPtArray;
	ptarr->SetSize(360/(int)grad);

	double ugol = 0;
	for(int i=0; i<360/(int)grad; i++) {
		MPoint pto;
		pto.x = pt->x + radius*sin((2*pi)*ugol/360.0);
		pto.y = pt->y + radius*cos((2*pi)*ugol/360.0);
		ptarr->SetAt(i,&pto);
		ugol += grad;
	}

	return ptarr;
}


//SUPPORT FUNCTIONS FOR REALIZATION OF ALGORITHM OF CREATION OF THE TRIANGULABLE
//TRIANGLES ON THE SET OF POINTS AND THE SET CONTOUR AFTER DELON
//===================================================================
//!Check, whether lies the set point in the circle set by the center and radius
bool IsPointInCircle(MPoint pt, MPoint center, double radius)
//===================================================================
{
    //radius has to be non-negative;
	if(radius<0)
		return false;

	if((pt.x - center.x)*(pt.x - center.x) + (pt.y - center.y)*(pt.y - center.y)>radius*radius)
		return false;
	return true;
}

//===================================================================
//!Check, whether lies the set point in the triangle set by three points
bool IsPointInTriangle(MPoint pt, MPoint pt1, MPoint pt2, MPoint pt3)
//===================================================================
{
    //if triangle degenerate
	if((OTS(pt1, pt2, pt3)==0)&&(PTONSEG(pt, pt1, pt2)))
		return true;

    //if the triangle is focused clockwise, the focused areas of triangles,
    //leaning on edges of the set triangle with top in pt point, have to be positive,
    //that is triangles are focused counterclockwise
	if(OTS(pt1, pt2, pt3)<0){
		if((OTS(pt1, pt, pt2)>=0)&&(OTS(pt2, pt, pt3)>=0)&&(OTS(pt3, pt, pt1)>=0))
			return true;
	}
    //if the triangle is focused against an hour hand, the focused areas of triangles,
    //leaning on edges of the set triangle with top in pt point, have to be negative,
    //that is triangles are focused clockwise shooters
	else if(OTS(pt1, pt2, pt3)>0){
		if((OTS(pt1, pt, pt2)<=0)&&(OTS(pt2, pt, pt3)<=0)&&(OTS(pt3, pt, pt1)<=0))
			return true;
	}
	
	return false;

}

//===================================================================
//!Orientation of points of a triangle clockwise;
//! on an entrance of a point move on numbering of tops in a triangle
bool OrientTriangleByClock(MPoint& pt1, MPoint& pt2, MPoint& pt3)
//===================================================================
{
    //if any of points coincide, the focused area of a triangle is equal
    //to zero, it is a degenerate case
	if(((pt1==pt2)||(pt1==pt3))||(pt2==pt3))
		return false;

	MPoint pt;

	if(OTS(pt1, pt2, pt3)>0){
		pt = pt2;
		pt2 = pt3;
		pt3 = pt;
	}
	return true;
}


//==================================
//!Calculation of the describing rectangle for an entrance contour
bool FindBoundRect(MPtArray* pReg, MRect* boundrect)
//==================================
{
    if((!pReg) || (!boundrect))
		return false;

    int size = pReg->GetSize();

    if(size > 0)
    {
        MPoint* pt1 = pReg->GetAt(0);
        //  for a start we take a rectangle round the first point.
        boundrect->left = pt1->x;
        boundrect->right = pt1->x;

        boundrect->top = pt1->y;
        boundrect->bottom = pt1->y;

        boundrect->NormalizeGeoRect();

        // ...it also has to cover all the subsequent...
        for(int i = 1; i < size; ++i)
        {
            pt1 = pReg->GetAt(i);
            boundrect->ExpandToPt(*pt1);
        }
    }

	boundrect->NormalizeGeoRect();

	return true;
}

//==================================
//!Check, whether lies a triangle in one-coherent area;

/**for each edge of a triangle, we check, whether the edge in a contour in expanded sense lies
*/
bool IsTriangleInRegion(MPoint pt1, MPoint pt2, MPoint pt3, MPtArray* pReg)
//==================================
{
	if(!pReg)
		return false;

	if((SGINREG_EXT(pt1, pt2, pReg))&&(SGINREG_EXT(pt2, pt3, pReg))&&(SGINREG_EXT(pt1, pt3, pReg)))
		return true;

	return false;
}


//==================================
//!check, whether an internal 'pt' point for any polygon of 'pReg'
/**IDEA: from 'pt' point the horizontal beam which will cross some edges is carried out to the right
//'pReg' polygon; if number of these edges odd, 'pt' point - internal on the relation
//to 'pReg' polygon, if even, 'pt' point - external
*/
bool PointInRegion(MPoint point, MPtArray* polygon, double ldelta,bool retTrueIfPtOnCountur)
//==================================
{
    bool answer = false;

    if(polygon != NULL)
    {
        int polSize = polygon->GetSize();
        if(polSize > 2)
        {
            MPtArray region;
            region.AppendTail(polygon);
            MPoint point0 = *region.GetHead();
            MPoint point1 = *region.GetTail();
            if(point0 != point1)
            {
                region.AddTail(&point0);
                ++polSize;
            }
            int sIndex, iVertex;
            if (PTONKONTUR(point, &region,sIndex,iVertex,ldelta) )
                return retTrueIfPtOnCountur;

            MPoint crosspt;
            int intersectCount = 0;
            double x0 = point0.x;
            double y0 = point0.y;
            double x1;
            double y1;
            double deltaY;

            for(int iPolPtLoop = 1; iPolPtLoop < polSize; ++iPolPtLoop)
            {
                point1 = *region.GetAt(iPolPtLoop);
                x1 = point1.x;
                y1 = point1.y;
                deltaY = fabs(y0 - y1);
                if(deltaY != 0)
                {
                    crosspt.x = x0 + (point.y - y0) * (x0 - x1) / (y0 - y1);
                    crosspt.y = point.y;
                    if(crosspt.x > point.x)
                    {
                        if((y0 < crosspt.y) == (y1 >= crosspt.y))
                        {
                            ++intersectCount;
                        }
                    }
                }
                point0 = point1;
                x0 = x1;
                y0 = y1;
            }

            const int rem = intersectCount % 2;
            answer = (rem != 0);
        }
    }

    return answer;
}

//===================================================================
//!for the set set of points builds the convex covering polygon,
/**which tops - points from a set
//IDEA: 1) we find 'pt1' point with the maximum value x - coordinates;
//2) we find such 'pt2' point that all points of a set lie on the right side from a straight line
//(pt1, pt2):
//3) we find 'pt3' point, that all points of a set lie on the right side from a straight line
//(pt2, pt3) and so on, we won't return to initial 'pt1' point yet.
*/
bool FindConvexPolygonForPoints(MPtArray* ptar, MPtArray* convexplg,double delta)
//===================================================================
{
	if((!ptar)||(!convexplg))
		return false;

	int colpts = ptar->GetSize();
	if(colpts==0)
		return false;

	convexplg->ClearAll();

    //if the set consists of one point
	if(colpts==1){
		convexplg->AddTail(ptar->GetAt(0));
		return true;
	}

    //we find 'pt1' point with the maximum value x - coordinates
	double xmax = ptar->GetAt(0)->x;
	int nomer = 0;
//	int i = 0;
//	int j = 0;

	for(int i=1; i<colpts; i++){
		if(xmax < ptar->GetAt(i)->x){
			xmax = ptar->GetAt(i)->x;

			nomer = i;
		}
	}
    //first point of a convex polygon...
	convexplg->AddTail(ptar->GetAt(nomer));
	
    //if the set consists of two points
	int jj = ((nomer==0) ? 1 : 0);
	if(colpts==2){
		convexplg->AddTail(ptar->GetAt(jj));
		return true;
	}
	
	MPoint pt1, pt2, pt;
	bool isLeft = false;

	pt2 = *ptar->GetAt(jj);
    //we build a contour of a convex polygon, we won't return to the initial point yet
    while(pt2 != *convexplg->GetHead() && DLINASG(pt2, *convexplg->GetHead()) > delta){
		pt1 = *convexplg->GetTail();
        //we find such point of pt2 that all points of a set lie on the right side
        //from a straight line (pt1, pt2):
		for(int i=0; i<colpts; i++){
			pt2 = *ptar->GetAt(i);
			for(int j=0; j<colpts; j++){
				pt = *ptar->GetAt(j);
                if((pt!=pt1 && DLINASG(pt, pt1) > delta)&&(pt!=pt2 && DLINASG(pt, pt2) > delta)){
                    //as soon as there is a point to the left of a straight line (pt1, pt2)
					if(OTS(pt1, pt, pt2)<=0)
					{
						isLeft = true;
						break;
					}
				}
			}
            //if all points lie to the right of a straight line (pt1, pt2), we include pt2 point
            //in a contour of a convex polygon
			//if(jj==colpts){
			if(!isLeft)
			{
				convexplg->AddTail(&pt2);
				break;
			}
			else
				isLeft = false;
		}
	}

    //we will remove the final point of a convex contour coinciding with initial
	convexplg->DeleteTail();

	return true;
}

//==================================
//! Calculation (focused) area of a polygon
double Area(MPtArray* ptar)
//==================================
{
	if(!ptar)
		return -1;
	double S =0;
	int nPoints = ptar->GetSize();
	for(int i=0; i<nPoints-1; i++)
		S += (ptar->GetAt(i)->x - ptar->GetAt(i + 1)->x)*(ptar->GetAt(i)->y + ptar->GetAt(i+1)->y);

	S += (ptar->GetAt(nPoints-1)->x - ptar->GetAt(0)->x)*(ptar->GetAt(nPoints-1)->y + ptar->GetAt(0)->y);
	S = S/2;
	return S;
}

//==================================
//!Change of orientation of a broken line on the return
bool OrientInverse(MPtArray* ptar)
//==================================
{
    if(!ptar)
		return false;

    int	nPoints=ptar->GetSize();
	if(nPoints==0)

		return true;


	MPtArray* ptarnew = new MPtArray();
	ptarnew->SetSize(nPoints);

    //We reorient an order of points
	for (int i=0; i<nPoints; i++)
		ptarnew->SetAt(i,ptar->GetAt(nPoints - i - 1));
	
	ptar->ClearAll();
	ptar->AppendTail(ptarnew);
	delete ptarnew;
	return true;
}

//==================================
//!Installation of the left orientation of a broken line
bool OrientLeft(MPtArray* ptar)
//==================================
{
	if(!ptar)
		return false;

    int	nPoints=ptar->GetSize();
	if(nPoints==0)
		return true;

	if(Area(ptar)<0) OrientInverse(ptar);

	return true;
}

//==================================
//! Installation of the right orientation of a broken line

bool OrientRight(MPtArray* ptar)
//==================================
{
	if(!ptar)
		return false;

    int	nPoints=ptar->GetSize();
	if(nPoints==0)
		return true;

	if(Area(ptar)>0) OrientInverse(ptar);

	return true;
}

//===================================================================
//!for the set set of contours builds the covering contour,
/**the including
//all entrance contours so that the border of the received contour consisted of parts
//borders of entrance contours also I belonged to coherent area
//ASSUMPTION: for any of contours there will be a contour in which the first lies
//a contour or, on the contrary, lying in the first contour, or with which
//the first contour is crossed on some broken line
*/
bool ConstructUnionStructKontur(MArArray* structkonturs, MPtArray* reskontur)
//===================================================================
{
	if((!structkonturs)||(!reskontur))
		return false;

    //we will check a correctness of the entrance array of contours
	int i = 0;
	for(i=0; i<structkonturs->GetSize(); i++){
		if(structkonturs->GetAt(i)->WhatTheArray() != ID_PTA)
			return false;
	}

	reskontur->ClearAll();

    //we will make copies of all entrance contours to have opportunity to change them
	MArArray* konturs = new MArArray();
	for(i=0; i<structkonturs->GetSize(); i++){
		MPtArray* kontur = new MPtArray();
		if(((MPtArray*)structkonturs->GetAt(i))->GetSize()>2){
			kontur->AppendTail((MPtArray*)structkonturs->GetAt(i));
            //if contours aren't closed, we will close them
			if(((MPtArray*)kontur)->GetAt(0)!=((MPtArray*)kontur)->GetAt(kontur->GetSize()-1) && DLINASG(((MPtArray*)kontur)->GetAt(0), ((MPtArray*)kontur)->GetAt(kontur->GetSize()-1)) > 0.00001)
				((MPtArray*)kontur)->AddTail(((MPtArray*)kontur)->GetAt(0));
			konturs->AddTail((MArray*)kontur);
		}

	}

    //finally in the 'borderkonturs' array only contours will lie,
    //participating in formation of the covering contour
	MArArray* borderkonturs = new MArArray();
	borderkonturs->AppendTail(konturs);
	MPtArray* firstkontur;
	MPtArray* secondkontur;
    //we will exclude internal contours for any contour, including, concerning a contour
    //them containing in some points or adjacent to a contour them containing on
    //some broken line
	for(i=0; i<borderkonturs->GetSize(); i++){
		firstkontur = (MPtArray*)borderkonturs->GetAt(i);
		for(int j=0; j<borderkonturs->GetSize(); j++){
			if(j!=i){
				secondkontur = (MPtArray*)borderkonturs->GetAt(j);
				if(LomlineInRegion_EXT(secondkontur, firstkontur)){
					borderkonturs->Delete(j);
					j--;
				}
				else if(LomlineInRegion_EXT(firstkontur, secondkontur)){
					borderkonturs->Delete(i);
					i--;
					break;
				}
			}
		}
	}

	MArray* kontur;
    //we focus all contours which will take part in designing
    //the output covering contour, clockwise
	for(i=0; i<borderkonturs->GetSize(); i++){
		kontur = borderkonturs->GetAt(i);
		OrientRight((MPtArray*)kontur);
	}

    //we will check that tops of each of boundary contours didn't coincide, except
    //initial and final points of a contour
	for(i=0; i<borderkonturs->GetSize(); i++){
		firstkontur = (MPtArray*)borderkonturs->GetAt(i);
		MPoint pt, pt1;
        //for each point of a contour, we check, whether it coincides about any another
        //contour point
		for(int j=0; j<firstkontur->GetSize()-1; j++){
			pt = *firstkontur->GetAt(j);
			for(int k=0; k<firstkontur->GetSize()-1; k++){
				if(k!=j){
					pt1 = *firstkontur->GetAt(k);
                    //if there were coinciding points, form two contours: one such, that
                    //coinciding points were for it initial and final: the second is made
                    //from the contour rests
					if(pt==pt1 || DLINASG(pt, pt1) < EPSILON){
						MPtArray* partkontur1 = new MPtArray();
						MPtArray* partkontur2 = new MPtArray();
                        //we form the second contour...
						int l = 0;
						for(l=0; l<firstkontur->GetSize()-1; l++){
							if(l==j)
								break;
							partkontur2->AddTail(firstkontur->GetAt(l));
						}
						for(l=k; l<firstkontur->GetSize()-1; l++){
							partkontur2->AddTail(firstkontur->GetAt(l));
						}
                        //we close a contour
						partkontur2->AddTail(partkontur2->GetAt(0));
                        //we form the first contour...
						for(l=j; l<k; l++){
							partkontur1->AddTail(firstkontur->GetAt(l));
						}
                        //we close a contour
						partkontur1->AddTail(partkontur1->GetAt(0));
                        //one of two made contours lies in another,
                        //as boundary we will leave covering from two contours
						if(LomlineInRegion_EXT(partkontur2, partkontur1)){
							borderkonturs->Delete(i);
							OrientRight(partkontur1);
							borderkonturs->AddTail((MArray*)partkontur1);
							delete partkontur2;
							firstkontur = partkontur1;
							j = 0;
							i--;
							break;
						}
						else if(LomlineInRegion_EXT(partkontur1, partkontur2)){
							borderkonturs->Delete(i);
							OrientRight(partkontur2);
							borderkonturs->AddTail((MArray*)partkontur2);
							delete partkontur1;
							firstkontur = partkontur2;
							j = 0;
							i--;
							break;
						}
					}
				}
			}
		}
	}


    //if boundary contour is one

	if(borderkonturs->GetSize()==1){
		reskontur->AppendTail((MPtArray*)borderkonturs->GetAt(0));
		delete borderkonturs;
		konturs->FreeMemory();
		delete konturs;
		return true;
	}

    //PROCEDURE of formation of the covering contour from a set of boundary contours:
    //we compare two contours if they are adjacent, of borders of two contours it is formed
    //a contour them covering
	MPoint pt;
	int index, vertexind;
//	int col = 0;//the auxiliary parameter serving to procedure debugging
    MInArray* firstindar = new MInArray();  //the array of indexes of the points of the first contour which are
                                            //also points of the second contour
    MInArray* secondindar = new MInArray(); //array of indexes of points of the second contour,
                                            //the points of the first contour corresponding to indexes
    MInArray* orderdfindar = new MInArray();//the array of indexes of points of the first contour describing
                                            //the general broken line for two contours
    MInArray* orderdsindar = new MInArray();//the array of indexes of the points of the second contour respective
                                            //to indexes of points of the first contour of orderdfindar

	for(i=0; i<borderkonturs->GetSize(); i++){
		firstkontur = (MPtArray*)borderkonturs->GetAt(i);
		for(int j=0; j<borderkonturs->GetSize(); j++){
			if(i!=j){
				firstindar->ClearAll();
				secondindar->ClearAll();
				orderdfindar->ClearAll();
				orderdsindar->ClearAll();
				secondkontur = (MPtArray*)borderkonturs->GetAt(j);
                //for each point of the first contour, except the last as it coincides
                //with initial, we check, whether it belongs to the second contour, and we will make
    //two arrays of the indexes corresponding each other for coinciding points
				int k = 0;
				for( k=0; k<firstkontur->GetSize()-1; k++){
					pt = *firstkontur->GetAt(k);
					if(PTONKONTUR(pt, secondkontur, index, vertexind)){
						firstindar->AddTail(&k);
						if(vertexind==index+1)
							index += 1;
						secondindar->AddTail(&index);
					}
				}
                //if the general points of contours are found...
                //if general points more than one, it is necessary to allocate the general broken line for
    //both contours
				if(firstindar->GetSize()>1){
					index = *firstindar->GetAt(0);
					orderdfindar->AddTail(&index);
					orderdsindar->AddTail(secondindar->GetAt(0));
//					for(int k=1; k<firstindar->GetSize(); k++){     2004.10.06 vipa
					for( k=1; k<firstindar->GetSize(); k++){
						index = *firstindar->GetAt(k);
						if(index == *firstindar->GetAt(k-1) + 1){
							if((*secondindar->GetAt(k) != *secondindar->GetAt(k-1) - 1)&&(*secondindar->GetAt(k-1) != 0))
								break;
							orderdfindar->AddTail(&index);
							orderdsindar->AddTail(secondindar->GetAt(k));
						}
						else
							break;
					}
					int l = 0;
					for(l=firstindar->GetSize()-1; l>=k; l--){
						orderdfindar->AddHead(firstindar->GetAt(l));
						orderdsindar->AddHead(secondindar->GetAt(l));
					}
				
					firstkontur->Delete(firstkontur->GetSize()-1);
					secondkontur->Delete(secondkontur->GetSize()-1);
                    //it is necessary to check that indexes of the general broken line go consistently
					bool smegnost = true;
					for(l=0; l<orderdfindar->GetSize()-1; l++){

                        //if there is no transition through a zero point, check, that the subsequent
                        //the index was one unit more than the previous
						if(*orderdfindar->GetAt(l)!=firstkontur->GetSize()-1){
							if(*orderdfindar->GetAt(l) != *orderdfindar->GetAt(l+1)-1){
								smegnost = false;
								break;
							}
						}
                        //if there is a transition through a zero point, check, that the subsequent
                        //the index was zero
						else if(*orderdfindar->GetAt(l)==firstkontur->GetSize()-1){
							if(*orderdfindar->GetAt(l+1) != 0){
								smegnost = false;
								break;
							}
						}
					}
                    //if from indexes of the first contour the coherent broken line turned out,
                    //belonging to a contour, we will check the same for the second contour
					if(smegnost){
						for(int l=0; l<orderdsindar->GetSize()-1; l++){
                            //if there is no transition through a zero point, check, that the subsequent
                            //the index was one unit less than the previous
							if(*orderdsindar->GetAt(l)!=0){
								if(*orderdsindar->GetAt(l) != *orderdsindar->GetAt(l+1)+1){
									smegnost = false;
									break;
								}
							}
                            //if there is a transition through a zero point, check, that the subsequent
                            //the index was equal to an index of the last point of the second contour
							else if(*orderdsindar->GetAt(l)==0){
								if(*orderdsindar->GetAt(l+1) != secondkontur->GetSize()-1){
									smegnost = false;
									break;
								}
							}
						}
					}
					if(smegnost){
                        //we will renumber points in contours so that a final point of the general broken line,
                        //I coincided with the initial points of the first and second contours;
                        //it is necessary  that to carry points of the general broken line for the first contour
                        //in a tail of the first contour, and for the second contour in the contour head
						firstkontur->CircleMove(*orderdfindar->GetAt(orderdfindar->GetSize()-1));
						secondkontur->CircleMove(*orderdsindar->GetAt(orderdsindar->GetSize()-1));
                        //we will close contours
						firstkontur->AddTail(firstkontur->GetAt(0));
						secondkontur->AddTail(secondkontur->GetAt(0));
						
                        //now we form the contour uniting borders of two contours
                        //we delete points of the general broken line from the first contour
						for(k=0; k<orderdfindar->GetSize(); k++)
							firstkontur->DeleteTail();
                        //we will add to the end of the first contour consistently points of the second contour,
                        //following the points belonging to the general broken line
						for(k=orderdfindar->GetSize()-1; k<secondkontur->GetSize(); k++)
							firstkontur->AddTail(secondkontur->GetAt(k));
                        //if the new created contour wasn't closed, we will close it
						if(*firstkontur->GetAt(0) != *firstkontur->GetAt(firstkontur->GetSize()-1) && DLINASG(*firstkontur->GetAt(0), *firstkontur->GetAt(firstkontur->GetSize()-1)) > EPSILON)
							firstkontur->AddTail(firstkontur->GetAt(0));
                        //the first of the considered contours changed and we keep for the further
                        //participations in formation of the covering contour, and the second contour we delete
                        //from the list of boundary contours as it already took part in procedure
						borderkonturs->Delete(j);
						i--;
//						col++;
						break;

					}
					else{
						firstkontur->AddTail(firstkontur->GetAt(0));
						secondkontur->AddTail(secondkontur->GetAt(0));
					}
				}
			}
		}
	}

	delete firstindar;
	delete secondindar;
	delete orderdfindar;
	delete orderdsindar;

	index = 0;
	double area;
    //from all contours which remained after working off of algorithm - and there were only contours,
    //from which parts it is impossible to create the general contour of coverage: non-adjacent contours,
    //the contours having one general point of a contact and not lying one in other, contours, not
    //having the general broken line and not lying one in other, - we choose a contour with maximum
    //the area also we take it for a coverage contour
	double maxarea = fabs(Area((MPtArray*)borderkonturs->GetAt(0)));
	for(i=0; i<borderkonturs->GetSize(); i++){
		area = fabs(Area((MPtArray*)borderkonturs->GetAt(i)));
		if(area>maxarea){
			index = i;
			maxarea = area;
		}
	}
	reskontur->AppendTail((MPtArray*)borderkonturs->GetAt(index));
	reskontur->DeleteTail();

	delete borderkonturs;
	konturs->FreeMemory();
	delete konturs;

	return true;
}

//===================================================================
//!function returns a contour, having removed from it all self-crossed segments,
//! if such were
/**REMARK: we believe, initial and final points of a contour coincide
*/
bool RemoveAllCrossptsOfKontur(MPtArray* kontur)

//===================================================================
{
	if(!kontur) return false;

	MPoint pt1, pt2, pt3, pt4, pt;
	int index1=0, index2=0, ind1, ind2;
    //for each edge of a contour we check, whether it is crossed with any another
	for(int j=0; j<kontur->GetSize()-1; j++){
		pt1 = *kontur->GetAt(j);
		pt2 = *kontur->GetAt(j+1);
		for(int k=0; k<kontur->GetSize()-1; k++){
			if((j != 0)&&(j != kontur->GetSize()-2)){
				ind1 = j+1;
				ind2 = j-1;
			}
			else if(j == 0){
				ind1 = kontur->GetSize()-2;
				ind2 = j+1;
			}
			else if(j == kontur->GetSize()-2){
				ind1 = j-1;
				ind2 = 0;
			}
			if((k!=j)&&(k!=ind1)&&(k!=ind2)){
				pt3 = *kontur->GetAt(k);
				pt4 = *kontur->GetAt(k+1);
                //if there was a broken line segment point of intersection from a broken line not in an adjacent segment
				if(CROSSSECTION(pt1, pt2, pt3, pt4, &pt)){
                    //the point of intersection doesn't coincide with one of the ends of a segment;
                    //then we will add a point of intersection to a contour on both segments
					if((pt != pt1)&&(pt != pt2)&&(pt != pt3)&&(pt != pt4)){
						kontur->InsertAfter(k,&pt);
						kontur->InsertAfter(j,&pt);
						index1 = j+1;
						index2 = k+1;
					}
                    //the point of intersection coincided with one of the ends j-th of a segment;
                    //then we will add a point of intersection to a contour on k-th a segment
					else if(((pt == pt1)||(pt == pt2))&&((pt != pt3)&&(pt != pt4))){
						kontur->InsertAfter(k,&pt);
						index1 = j;
						index2 = k+1;
					}
                    //the point of intersection coincided with one of the ends k-th of a segment;
                    //then we will add a point of intersection to a contour on j-th a segment
					else if(((pt == pt3)||(pt == pt4))&&((pt != pt1)&&(pt != pt2))){
						kontur->InsertAfter(j,&pt);
						index1 = j+1;
						index2 = k;
					}
                    //the point of intersection coincided with j-th and k-th contour tops
					else if(((pt == pt3)||(pt == pt4))&&((pt == pt1)||(pt == pt2))){
						index1 = j;
						index2 = k;
					}
                    //if there were coinciding points, form two contour: firsth such, that
                    //coinciding points were for it initial and final: second is made
                    //from the contour rests
					MPtArray* partkontur1 = new MPtArray();
					MPtArray* partkontur2 = new MPtArray();
                    //we form the second contour...
					int l = 0;
					for(l=0; l<kontur->GetSize()-1; l++){
						if(l==index1)
							break;
						partkontur2->AddTail(kontur->GetAt(l));
					}
					for(l=index2; l<kontur->GetSize()-1; l++){
						partkontur2->AddTail(kontur->GetAt(l));
					}
                    //we close a contour
					partkontur2->AddTail(partkontur2->GetAt(0));
                    //we form the first contour...
					for(l=index1; l<index2; l++){
						partkontur1->AddTail(kontur->GetAt(l));
					}
                    //we close a contour
					partkontur1->AddTail(partkontur1->GetAt(0));
                    //one of two made contours lies in another,
                    //as boundary we will leave covering from two contours
					if(LomlineInRegion_EXT(partkontur2, partkontur1)){
						OrientRight(partkontur1);
						kontur->ClearAll();
						kontur->AppendTail(partkontur1);
						delete partkontur2;
						delete partkontur1;
						j--;
						break;
					}
					else if(LomlineInRegion_EXT(partkontur1, partkontur2)){
						OrientRight(partkontur2);
						kontur->ClearAll();
						kontur->AppendTail(partkontur2);
						delete partkontur2;
						delete partkontur1;
						j--;
						break;
					}
				}
			}
		}
	}
	return true;
}

//===================================================================
//!function sorts out the relations of two contours among themselves;

/**'indexes1' and 'indexes2' - the returned arrays of indexes of the general tops ordered so
//that on indexes it was possible to build the general broken line for two contours,
//if such is available:
//the 'smegnost' parameter shows, whether there is such only (!) general broken line;
//function returns the return code defining the relations between contours
//RETURN CODES: - 1 - a nekorrktny exit,
//                0 - the first contour lies strictly inside another,
//                1 - the second contour lies strictly inside the first,
//                2 - the first contour lies in the second and concerns it in one point,
//                3 - the second contour lies in the first and concerns it in one point,
//                4 - contours concern in one point,
//                5 - the first contour lies in the second and has with it the general tops,
//                    if the 'smegnost' parameter == true, contours have the general broken line,
//                6 - the second contour lies in the first and has with it the general tops,
//                    if the 'smegnost' parameter == true, contours have the general broken line,
//                7 - contours don't lie one in another and have the general tops,
//                    if the 'smegnost' parameter == true, contours have the general broken line,
//                8 - all other cases
*/
int TwoContursRelationships(MPtArray* kontur1, MPtArray* kontur2, bool &smegnost, 
							MInArray* indexes1, MInArray* indexes2)
//===================================================================
{
	if((!kontur1)||(!kontur2)||(!indexes1)||(!indexes2))
		return -1;

	indexes1->ClearAll();
	indexes2->ClearAll();

	smegnost = false;

	OrientRight(kontur1);
	OrientRight(kontur2);

    //we will check that contours were closed
	if(*kontur1->GetAt(kontur1->GetSize() - 1) != *kontur1->GetAt(0)){
		MPoint pt = *kontur1->GetAt(0);
		kontur1->AddTail(&pt);
	}
	if(*kontur2->GetAt(kontur2->GetSize() - 1) != *kontur2->GetAt(0))
		kontur2->AddTail(kontur2->GetAt(0));

    //if the first contour lies strictly in the second contour
	if(LomlineInRegion(kontur1, kontur2))
		return 0;
    //if the second contour lies strictly in the first contour
    if(LomlineInRegion(kontur2, kontur1))

		return 1;


	MPoint pt;
	int index, vertexind;
    MInArray* firstindar = new MInArray();  //the array of indexes of the points of the first contour which are

                                            //also points of the second contour
    MInArray* secondindar = new MInArray(); //array of indexes of points of the second contour,
    //the points of the first contour corresponding to indexes

    //for each point of the first contour, except the last as it coincides
    //with initial, we check, whether it belongs to the second contour, and we will make
    //two arrays of the indexes corresponding each other for coinciding points
	for(int k=0; k<kontur1->GetSize()-1; k++){
		pt = *kontur1->GetAt(k);
		if(PTONKONTUR(pt, kontur2, index, vertexind)){
			firstindar->AddTail(&k);
			if(vertexind==index+1)
				index += 1;
			secondindar->AddTail(&index);
		}
	}
	int konturspar;
    //if the first contour lies in the second...
	if(LomlineInRegion_EXT(kontur1, kontur2))
		konturspar = 0;
    //if the second contour lies in the first...
	else if(LomlineInRegion_EXT(kontur2, kontur1))
		konturspar = 1;
    //if contours don't lie one in another...
	else
		konturspar = 2;
    //if the general points of contours are found...
    //one general point
	if(firstindar->GetSize()==1){
		if(konturspar==0)
			return 2;
		else if(konturspar==1)
			return 3;
		else if(konturspar==2)
			return 4;
	}
    //if general points more than one, it is necessary to allocate the general broken line for
    //both contours
	if(firstindar->GetSize()>1){
        //if contours don't lie one in other, but have the general points

		if(konturspar==2){
            //we will order arrays of indexes of the general points so that on them it is continuous
            //the general broken line if such exists was under construction
			index = *firstindar->GetAt(0);
			indexes1->AddTail(&index);
			indexes2->AddTail(secondindar->GetAt(0));
			int k = 0;
			for(k=1; k<firstindar->GetSize(); k++){
				index = *firstindar->GetAt(k);
                //if indexes of the general points of the first contour grow, for the second contour
                //they decrease, when konturspar=2, as the movement on contours lengthways
                //to the general broken line it is opposite sent
				if(index == *firstindar->GetAt(k-1) + 1){
					if((*secondindar->GetAt(k) != *secondindar->GetAt(k-1) - 1)&&(*secondindar->GetAt(k-1) != 0))
						break;
					indexes1->AddTail(&index);
					indexes2->AddTail(secondindar->GetAt(k));
				}

				else
					break;
			}
			for(int l=firstindar->GetSize()-1; l>=k; l--){
				indexes1->AddHead(firstindar->GetAt(l));
				indexes2->AddHead(secondindar->GetAt(l));
			}
		}
        //if contours lie one in other and have the general points
		if((konturspar==0)||(konturspar==1)){
            //we will order arrays of indexes of the general points so that on them it is continuous
            //the general broken line if such exists was under construction
			index = *firstindar->GetAt(0);
			indexes1->AddTail(&index);
			indexes2->AddTail(secondindar->GetAt(0));
			int k = 0;
			for(k=1; k<firstindar->GetSize(); k++){
				index = *firstindar->GetAt(k);
                //if indexes of the general points of the first contour grow, for the second contour
                //they also grow, when konturspar=0(=1), as the movement on contours lengthways
                //general broken line codirectional
				if(index == *firstindar->GetAt(k-1) + 1){
					if((*secondindar->GetAt(k) != *secondindar->GetAt(k-1) + 1)&&(*secondindar->GetAt(k-1) != kontur2->GetSize()-2))
						break;
					indexes1->AddTail(&index);

					indexes2->AddTail(secondindar->GetAt(k));
				}
				else
					break;
			}
			for(int l=firstindar->GetSize()-1; l>=k; l--){
				indexes1->AddHead(firstindar->GetAt(l));
				indexes2->AddHead(secondindar->GetAt(l));
			}
		}
	
		kontur1->Delete(kontur1->GetSize()-1);
        //it is necessary to check that indexes of the general broken line go consistently
		smegnost = true;
		for(int l=0; l<indexes1->GetSize()-1; l++){
            //if there is no transition through a zero point, check, that the subsequent
            //the index was one unit more than the previous
			if(*indexes1->GetAt(l)!=kontur1->GetSize()-1){
				if(*indexes1->GetAt(l) != *indexes1->GetAt(l+1)-1){
					smegnost = false;
					kontur1->AddTail(kontur1->GetAt(0));
					return 8;

				}
			}
            //if there is a transition through a zero point, check, that the subsequent
            //the index was zero
			else if(*indexes1->GetAt(l)==kontur1->GetSize()-1){
				if(*indexes1->GetAt(l+1) != 0){
					smegnost = false;
					kontur1->AddTail(kontur1->GetAt(0));

					return 8;
				}
			}
		}
        //if from indexes of the first contour the coherent broken line turned out,
        //belonging to a contour, we will check the same for the second contour
		if((smegnost)&&(konturspar==2)){
			kontur2->Delete(kontur2->GetSize()-1);
			for(int l=0; l<indexes2->GetSize()-1; l++){
                //if there is no transition through a zero point, check, that the subsequent
                //the index was one unit less than the previous
				if(*indexes2->GetAt(l)!=0){
					if(*indexes2->GetAt(l) != *indexes2->GetAt(l+1)+1){
						smegnost = false;
						kontur2->AddTail(kontur2->GetAt(0));
						return 8;
					}
				}
                //if there is a transition through a zero point, check, that the subsequent
                //the index was equal to an index of the last point of the second contour
				else if(*indexes2->GetAt(l)==0){
					if(*indexes2->GetAt(l+1) != kontur2->GetSize()-1){
						smegnost = false;
						kontur2->AddTail(kontur2->GetAt(0));

						return 8;
					}
				}
			}
        }//end if(smegnost)
        //if from indexes of the first contour the coherent broken line turned out,
        //belonging to a contour, we will check the same for the second contour
		if((smegnost)&&((konturspar==0)||(konturspar==1))){
			kontur2->Delete(kontur2->GetSize()-1);
			for(int l=0; l<indexes2->GetSize()-1; l++){
                //if there is no transition through a zero point, check, that the subsequent
                //the index was one unit more than the previous
				if(*indexes2->GetAt(l)!=kontur2->GetSize()-1){
					if(*indexes2->GetAt(l) != *indexes2->GetAt(l+1)-1){
						smegnost = false;
						kontur2->AddTail(kontur2->GetAt(0));
						return 8;
					}
				}
                //if there is a transition through a zero point, check, that the subsequent
                //the index was equal to an index of the last point of the second contour
				else if(*indexes2->GetAt(l)==kontur2->GetSize()-1){
					if(*indexes2->GetAt(l+1) != 0){
						smegnost = false;
						kontur2->AddTail(kontur2->GetAt(0));
						return 8;
					}
				}
			}
        }//end if(smegnost)
		if(smegnost){
			if(konturspar==0)
				return 5;
			else if(konturspar==1)
				return 6;
			else if(konturspar==2)
				return 7;
		}
    }//end if(firstindar->GetSize()>1)

	delete firstindar;
	delete secondindar;

	return 8;
}


//'reper' point, 'azimuth' azimut (degrees), range ('dalnost') is set
//! returns a point which is defined by a reference point, an azimuth and range
//! if range <=0, reference point coordinates come back
MPoint GetPointByAzD(MPoint reper, double azimut, double dalnost)
{
    //if radius <=0
	if(dalnost<=0)
		return reper;

    //we will normalize an azimuth

	while(azimut<0) azimut+=360;
	while(azimut>=360)azimut-=360;

    //in radians
	azimut = azimut*pi/180.0;

    //find result
	MPoint result;
	result.x = reper.x + dalnost*sin(azimut);
	result.y = reper.y + dalnost*cos(azimut);
	

	return result;
}

//two points of 'pt1' and 'pt2' are set
//!returns azimuth of the vector pt1->pt2
//! if points coincide, the azimuth is equal 0
double GetAzimutByPoints(MPoint pt1, MPoint pt2)
{
    //if points coincide
	if(pt1==pt2)
	{
		return 0;
	}

    //if points are located along an axis Y
	if(pt1.x==pt2.x)
	{
		if(pt1.y<pt2.y)
			return 0;
		else
			return 180;
	}
    //if points are located along an axis X
	else if(pt1.y==pt2.y)
	{
		if(pt1.x<pt2.x)
			return 90;
		else
			return 270;
	}
	else
	{
		double dlina = DLINASG(pt1,pt2);
		double azimut;
		if(dlina==0)
			return 0;
        //differently if points are in the left part
		if(pt1.x<pt2.x)
		{
            //first quarter
			if(pt1.y<pt2.y)
			{
				azimut = acos((pt2.y-pt1.y)/dlina);
			}
            //second quarter
			else
			{
				azimut = pi/2+acos((pt2.x-pt1.x)/dlina);
			}
		}
        // right part
		else
		{
            // third part
			if(pt1.y>pt2.y)
			{
				azimut = pi+ acos((pt1.y-pt2.y)/dlina);
			}
            // fourth part
			else
			{

				azimut = 3*pi/2+acos((pt1.x-pt2.x)/dlina);
			}
		}
		azimut = 180*azimut/pi;
		return azimut;
	}
}


//!vector pt1->pt2,and 'up' angle of rotation is set
//! returns an azimuth
//! if points coincide, the azimuth will be 0
double GetAzByUp(MPoint pt1, MPoint pt2, double& up)
{
    //if points coincide
    if(pt1==pt2)
		return 0;

    //we will execute normalization of an angle of rotation
	if(up<0)
	{
		while(up<-180)
			up+=360;
		if(up>0)
			up = 180-up;
		else if(up == -180) 
			up = 180;
	}
	else
	{
		while(up>180)
			up-=360;

		if(up<0)
			up = -180-up;
	}

    //we will transfer up to a normal look,
    // that it was from 0 to 360
	double up1 = up;
	if(up1!=0)
	{
		if(up1<0)
			up1 = fabs(up1);
		else 
			up1 = 360-up1;
	}

    //we will define an azimuth of two points
	double azim = GetAzimutByPoints(pt1, pt2);

    //we will calculate result
	azim = azim+up1+180;

    //we normalize an azimuth
	while (azim<0) azim+=360;
	while (azim>=360) azim-=360;

    //we will return result
	return azim;
}


//!vector pt1->pt2,  and an azimuth to the third point of 'az' (degrees) is set
//! returns an angle of rotation
//! if points coincide, the angle of rotation will be 0
double GetUpByAz(MPoint pt1, MPoint pt2, double& az)
{
    //if points coincide
    if(pt1==pt2)
		return 0;

    //we normalize an azimuth
    while(az<0) az+=360;

	while(az>=360) az-=360;

    //we will define an azimuth of two points
    double result = GetAzimutByPoints(pt1,pt2);

    //we will count an angle of rotation in a look from 0 to 360
	result = az-result+180;

    //normalize
	while(result<0) result+=360;
	while(result>=360) result-=360;

    //we will transfer result to a normal look
	if(result!=0)
	{
		if(result<180)
			result = -result;
		else if(result>180)
			result = 360-result;
	}


    //we will return result
	return result;
}



//!three points are set: 'pt1', 'pt2', 'pt3'
//!to find the minimum angle between vectors (pt2; pt1) (pt2, pt3)
double FindUgolBetweenVectors(MPoint pt1, MPoint pt2, MPoint pt3)
{
	double result;
	
    //vector pt2-pt1
	MPoint v1;
	v1.x = pt1.x-pt2.x;
	v1.y = pt1.y-pt2.y;

    //vector pt2-pt3
	MPoint v2;
	v2.x = pt3.x-pt2.x;
	v2.y = pt3.y-pt2.y;

    // scalar product
	double scal = v1.x*v2.x+v1.y*v2.y;

    //lengths of vectors
	double d1 = sqrt(v1.x*v1.x+v1.y*v1.y);
	double d2 = sqrt(v2.x*v2.x+v2.y*v2.y);

    //cosine
	double cosinus = scal/(d1*d2);

	if (fabs(cosinus)>1.0)
		cosinus = cosinus/fabs(cosinus);

    //result

	result = acos(cosinus);

    //in degrees
	result = result*180/pi;

    //normalize
	while(result<0)
		result+=360;
	while(result>=360)
		result-=360;

	return result;
}

//=================================================================
//!Rectangle sketch straight lines current brush
void DrawShtrihRect(MPlotDevice* pDevice,MRect* rect,double angle,double step)
//=================================================================
{
    //we lead a angle to a normal amount
	while(angle>=360)angle-=360;

	while(angle<0)angle+=360;

    //if angle = 90 degrees
	if(angle==90)
	{
        //we draw parallel to axis Y with 'step' step
		double y1=rect->bottom;
		double y2=rect->top;
		double x1,x2;
		double x;
		if(rect->left<rect->right)
		{
			x1=rect->left;
			x2=rect->right;
		}
		else
		{
			x1=rect->left;
			x2=rect->right;
		}
		for(x=x1;x<x2;x+=step)
		{
			//since qt3
			//pDevice->MoveTo(c);
			//pDevice->LineTo(x2,y2);
			pDevice->DrawLine((int)x,y1,(int)x,y2);
		}
	}
    //else
	else
	{
        // here we proceed from Y=kX+b straight line equation

        //1. we learn coefficient of an inclination of shadings
        //2. we determine the range of change of the free member
        //   at least and at most on straight lines,
        //   passing through the covering rectangle
        //3. we define a step of the free member,
        //   at which the necessary distance is provided
        //   between the next strokes
        //4. we determine the ends of a segment by the equation of a straight line
        //   for carrying out a straight line:
        //   the first point is defined
        //   at equality of X-coordinate of one of the parties of a rectangle,
        //   and the second - another
        //5. we draw a segment


        //k - inclination tangent of angle
		angle=tan(angle*pi/180);
        //we learn the range of change of the member of 'b'
        //and its step
		double b1=rect->bottom-angle*rect->left;
		double b2=rect->top-angle*rect->right;
		double b3=rect->bottom-angle*rect->right;
		double b4=rect->top-angle*rect->left;
		double db=fabs(step*sqrt(angle*angle+1.0));
		double b,bmin,bmax;

		bmin=Min(Min(b1,b2),Min(b3,b4));
		bmax=Max(Max(b1,b2),Max(b3,b4));
        //we draw segments which ends lie on the parties of our rectangle
		for(b=bmin;b<bmax;b+=db)
		{
			double x1=rect->left;
			double y1=angle*x1+b;
			double x2=rect->right;
			double y2=angle*x2+b;
			//since qt3
			//pDevice->MoveTo(c);
			//pDevice->LineTo(x2,y2);
			pDevice->DrawLine(x1,y1,x2,y2);
		}
	}
}


// ==================================================================
//!	Function builds the leading round contour for the given
MPtArray* BuildObvod(MPtArray* pta,double dOffset)
// ==================================================================
{ //return (MPtArray*)pta->Clone();
	if(!dOffset)
		return (MPtArray*)pta->Clone();
	bool bBounded = (*pta->GetHead()==*pta->GetTail())?true:false;
	
	MPtArray* ptarr=new MPtArray();
	MPoint* pt1,*pt2,*pt3 =NULL;
	
	MPtArray* ptar = (MPtArray*)pta->Clone();
	//******************************************************
    //Cleaning of the array
    //we will clean the array so that there were no in a row points equal
    //or three points lying on one straight line in a row
    //cleaning happens in spetsilno the array formed for this purpose
    //the initial array remains invariable
	
	int ii=0;
    //we pass all array, since a zero point
	while(ii<ptar->GetSize()-1)
	{
		if(ptar->GetSize()==1)break;

        //we receive the first two points
		pt1=ptar->GetAt(ii);
		pt2=ptar->GetAt(ii+1);
        //if the first two points are equal, we delete the second
		if((fabs(pt1->x-pt2->x)<0.01)&&(fabs(pt1->y-pt2->y)<0.01))
		{
			ptar->Delete(ii+1);
		}
        //otherwise if it is a penultimate point, we pass to the last
		else if(ii==ptar->GetSize()-2)
		{
			ii++;
		}
        //otherwise we take the third point
		else
		{
			pt3=ptar->GetAt(ii+2);

            //if three points lie on one straight line, we delete average
			if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.0001)
			{
				ptar->Delete(ii+1);
			}
            //otherwise we pass to the subsequent point
			else
			{
				ii++;
			}
		}
	}

	
    //if at us closed top object, we check still the three
    //points : (n-2,n-1,0) and (n-1,0,1)
	if(bBounded)
	{
		bool bb=true;
        //we check the first three
		while(bb)
		{
            //if it is more than two points
			if(ptar->GetSize()>2)
			{

                //we receive points
				pt1=ptar->GetAt(ptar->GetSize()-2);
				pt2=ptar->GetAt(ptar->GetSize()-1);

				pt3=ptar->GetAt(0);
                //if the tail and the head are equal, we delete a tail
				if((fabs(pt2->x-pt3->x)<0.01)&&(fabs(pt2->y-pt3->y)<0.01))
				{
					ptar->DeleteTail();
				}
                //else
				else
				{

                    //if three points on one straight line, we delete a tail
					if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.001)
					{
						ptar->DeleteTail();
					}
                    //differently everything is normal
					else
					{

						bb=false;
					}

				}
			}
            //if it is less than three points, we check nothing
			else
			{
				bb=false;
			}
		}
		
		bb=true;
        //we check the second three
		while(bb)
		{

            //if it is more than two points
			if(ptar->GetSize()>2)
			{
                //we receive points
				pt1=ptar->GetAt(ptar->GetSize()-1);
				pt2=ptar->GetAt(0);
				pt3=ptar->GetAt(1);
                //if the tail and the head are equal, we delete a tail
				if((fabs(pt2->x-pt1->x)<0.01)&&(fabs(pt1->y-pt2->y)<0.01))
				{
					ptar->DeleteTail();
				}
                //else
				else
				{
                    //if three points on one straight line, we delete the head
					if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.001)
					{
						ptar->DeleteHead();
					}
                    //everything is otherwise normal
					else
					{
						bb=false;
					}
				}
			}
            //if it is less than three points, we check nothing
			else
			{
				bb=false;
			}
		}

		
	}
	
	
	
	
	//******************************************************
	
    //we build the bypassing contour
	double A1,B1,C1,A2,B2,C2;
	int m_nPoints=ptar->GetSize();


	

    //at once we take away the necessary quantity of elements for the bypassing contour
	if((m_nPoints==2)||(!bBounded))
		ptarr->SetSize(m_nPoints);
	else

		ptarr->SetSize(m_nPoints+1);
	
    //if we have only two points, build the segment parallel to the initial
	if(m_nPoints==2)
	{
		pt1=ptar->GetAt(0);
		pt2=ptar->GetAt(1);
    MPoint pnt1= GetPerpendikular(pt1[0],pt2[0],pt1[0],dOffset);
		ptarr->SetAt(0,&pnt1);
    MPoint pnt2= GetPerpendikular(pt1[0],pt2[0],pt2[0],dOffset);
		ptarr->SetAt(1,&pnt2);
		
	}
    //otherwise we find the covering contour
	else

	{
        //if the object linear, the first point lies simply on a perpendicular
		if(!bBounded)
		{
			pt1=ptar->GetAt(0);
			pt2=ptar->GetAt(1);
      MPoint pnt= GetPerpendikular(pt1[0],pt2[0],pt1[0],dOffset);
			ptarr->SetAt(0,&pnt);
		}
		
        //further from zero to a point, the second since the end, we build contour points on crossings of straight lines
		pt1=ptar->GetAt(0);
		pt2=ptar->GetAt(1);
		GetPramuju(pt1[0],pt2[0],dOffset,A1,B1,C1);
		double dlx,dly,dl;
		MPoint pt;
		for(int kk=0;kk<ptar->GetSize()-2;kk++)
		{
            //we receive three points
			pt1=ptar->GetAt(kk);
			pt2=ptar->GetAt(kk+1);
			pt3=ptar->GetAt(kk+2);
			
            if (pt3 == NULL) pt3 = pt2;

            //we find a straight line of the second couple of points
			GetPramuju(pt2[0],pt3[0],dOffset,A2,B2,C2);
            //we find crossing of two straight lines
			dlx=B1*C2-B2*C1;
			dly=A2*C1-A1*C2;
			dl=A1*B2-A2*B1;
			pt.x=dlx/dl;
			pt.y=dly/dl;
			ptarr->SetAt(kk+1,&pt);
            //the equation of the first straight line in the following couple will be

            //same, as well as equation of the second straight line now
			A1=A2;
			B1=B2;
			C1=C2;
		}
        //if object linear, we put the last end as well as zero
		if(!bBounded)
		{
			if (pt3 == NULL)
				pt3 = pt2; //temp
      MPoint pnt= GetPerpendikular(pt2[0],pt3[0],pt3[0],dOffset);
			ptarr->SetAt(ptar->GetSize()-1,&pnt);
		}

        //otherwise we will finish finishing a contour
		else
		{
			pt1=ptar->GetAt(m_nPoints-2);
			pt2=ptar->GetAt(m_nPoints-1);
			pt3=ptar->GetAt(0);
			
            //three n-2,n-1,0
			GetPramuju(pt1[0],pt2[0],dOffset,A1,B1,C1);
			GetPramuju(pt2[0],pt3[0],dOffset,A2,B2,C2);
            //we find crossing of two straight lines
			dlx=B1*C2-B2*C1;
			dly=A2*C1-A1*C2;
			dl=A1*B2-A2*B1;
			pt.x=dlx/dl;
			pt.y=dly/dl;
			ptarr->SetAt(ptar->GetSize()-1,&pt);
			
            //three n-1,0,1
			pt1=ptar->GetAt(1);
			GetPramuju(pt2[0],pt3[0],dOffset,A1,B1,C1);

			GetPramuju(pt3[0],pt1[0],dOffset,A2,B2,C2);

            //we find crossing of two straight lines
			dlx=B1*C2-B2*C1;
			dly=A2*C1-A1*C2;
			dl=A1*B2-A2*B1;
			pt.x=dlx/dl;
			pt.y=dly/dl;
			ptarr->SetAt(0,&pt);
			
            //we close arrays
			pt1=ptarr->GetAt(0);
			pt.x=pt1->x;
			pt.y=pt1->y;
			ptarr->SetAt(ptarr->GetSize()-1,&pt);
			pt1=ptar->GetAt(0);
			pt.x=pt1->x;
			pt.y=pt1->y;
			ptar->AddTail(&pt);
		}
	}
	delete ptar;
	return ptarr;
}



//=================================================================
//!Creation of the leading round contour
MPtArray* BuildRudezh(MPtArray* pta,double dOffset, int splineCount) 
{
MPtArray* ptarr = new MPtArray();
ptarr->AppendTail(pta);
double step = dOffset/100;
double crntDist=0.0;
while(fabs(dOffset)>fabs(crntDist))
{
		CalcZoomBound(ptarr,step);
		if(dOffset<0)
			RemoveIntersection(ptarr);
		CleanArr(ptarr,step);
		crntDist += step;
}


for (int k=0;k<splineCount;k++)
{
		SplineAngels(ptarr,dOffset);
		CleanArr(ptarr, fabs(dOffset/10));
}
if (ptarr->Size)
	ptarr->AddTail(ptarr->GetAt(0));

return ptarr;
}



bool CalcZoomBound(MPtArray* pta, double delta)
{
	if(!pta->Size) return false;
	MPtArray tempArr;
	tempArr.AppendTail(pta);
	pta->ClearAll();

	MPoint pt1, pt2, pt3, crosPoint, pt4,pt5,pt6,pt7,centerPoint;
	MPoint prevPoint(0,0);
	double znak;

	for(int k=0;k<tempArr.Size;k++)
	{
		centerPoint.x += tempArr.GetAt(k)->x;
		centerPoint.y += tempArr.GetAt(k)->y;
	}
	centerPoint.x = centerPoint.x/tempArr.Size;
	centerPoint.y = centerPoint.y/tempArr.Size;

	for (int i=0;i<tempArr.Size;i++)
	{
			pt1 = tempArr.GetAt(i);
			if (i == tempArr.Size-2)
			{
				pt2 = tempArr.GetAt(i+1);
				pt3 = tempArr.GetAt(0);
			}
			else if(i == tempArr.Size-1)
			{
				pt2 = tempArr.GetAt(0);
				pt3 = tempArr.GetAt(1);
			}
			else 
			{
				pt2 = tempArr.GetAt(i+1);
				pt3 = tempArr.GetAt(i+2);
			}
			znak = pt1.y*(pt1.x-pt2.x)/pt2.y-pt1.y + pt1.x;
			if(znak < centerPoint.x)
				GetPramuju(pt1,pt2,delta,A1,B1,C1);
			else
				GetPramuju(pt1,pt2,-delta,A1,B1,C1);

			znak = pt2.y*(pt2.x-pt3.x)/pt3.y-pt2.y + pt2.x;
			if(znak < centerPoint.x )
				GetPramuju(pt2,pt3,delta,A2,B2,C2);
			else
				GetPramuju(pt2,pt3,-delta,A2,B2,C2);


			if ( CROSSLS(A1,B1,C1,A2,B2,C2, &crosPoint))
			{		
				if(pt2.Distance(&crosPoint)<fabs(100*delta))
					pta->AddTail(&crosPoint);
			}
	}
return true;
}

void CleanArr(MPtArray* pta,double dist)
{
	if(!pta->Size) return;
	double distance;
	MPoint pt1, pt2;
	for (int i=0;i<pta->Size;i++)
	{
		pt1 =  pta->GetAt(i);
		if(i== pta->Size-1)
			pt2 =  pta->GetAt(0);
		pt2 =  pta->GetAt(i+1);
		distance = pt1.Distance(&pt2);
		double re = dist;
		if (distance < re)
		{
			pta->Delete(i);
		}
	}	
}

bool SplineAngels(MPtArray* pta, double dist)
{
	if(!pta->Size) return false;
	MPtArray tempArr;
	tempArr.AppendTail(pta);
	pta->ClearAll();

	MPoint pt1, pt2, pt3, crosPoint, pt4,pt5,pt6,pt7,cross1,cross2;
	//double A1,B1,C1,A2,B2,C2,A3,B3,C3, znak;
	
	for (int i=0;i<tempArr.Size;i++)
	{
		pt1 = tempArr.GetAt(i);
		if (i == tempArr.Size-2)
		{
			pt2 = tempArr.GetAt(i+1);
			pt3 = tempArr.GetAt(0);
		}
		else if(i == tempArr.Size-1)
		{
			pt2 = tempArr.GetAt(0);
			pt3 = tempArr.GetAt(1);
		}
		else 
		{
			pt2 = tempArr.GetAt(i+1);
			pt3 = tempArr.GetAt(i+2);
		}
		
		//double dist1 = pMapI->GetDistance(m_initBound.GetCenterPoint(), pt2);
		//double dist2 = pMapI->GetDistance(m_initBound.GetCenterPoint(), MPoint((pt1.x+pt3.x)/2,(pt1.y+pt3.y)/2));
		bool sign;
		(dist>0)?sign = true:sign=false;
		bool isConvex = PointInRegion(MPoint((pt1.x+pt3.x)/2,(pt1.y+pt3.y)/2), &tempArr);
		double angl1 = GetVektorGrad(&pt2,&pt1);
		double angl2 = GetVektorGrad(&pt2,&pt3);
		double cosBs = cos((angl1-angl2)*pi/180);
		
			
		if((sign==isConvex)&&(cosBs>-0.8))
				{
                    pt4 = GetBisectrissa(pt2,pt1,pt3);//bisector vector
					angl1 = GetVektorGrad(&pt2,&pt4);
					angl2 = GetVektorGrad(&pt2,&pt3);
					double sinBs = sin((angl1-angl2)*pi/180);
					double dlt = fabs(dist)*(1/fabs(sinBs)-1);
					pt5 = ExtendVector(pt2,pt4, dlt);//
                    pt6 = GetPerpendikular(pt2,pt4, pt5, dist);//perpendicular to a bisector
					if (CROSSLINES(pt1,pt2,pt5,pt6, &cross1) && CROSSLINES(pt2,pt3,pt5,pt6, &cross2))
					{
						if(PTONSEG(cross1,pt1,pt2))
							pta->AddTail(&cross1);
						else 
							pta->AddTail(&pt5);
						pta->AddTail(&pt5);
						if(PTONSEG(cross2,pt2,pt3))
							pta->AddTail(&cross2);
						else 
							pta->AddTail(&pt5);
												
					}
					else
						pta->AddTail(&pt2);
				}
		else pta->AddTail(&pt2);
	}
	
	return true;
}


void RemoveIntersection(MPtArray* pta)
{
	if(!pta->Size) return;
	MPoint pnt1,pnt2,pnt3,pnt4,cross;
	int arrSize = pta->Size;
	for (int i=0;i<arrSize-2;i++)
	{	
		pnt1 = pta->GetAt(i);
		pnt2 = pta->GetAt(i+1);
			for (int j=i+1;j<arrSize-2;j++)
			{
				pnt3 = pta->GetAt(j);
				pnt4 = pta->GetAt(j+1);
				if(CROSSSECTION(pnt1,pnt2,pnt3,pnt4, &cross))
				{
					for (int k=i+1;k<j;k++)
					{
						pta->Delete(k);
					}
					pta->SetAt(i+1,&cross);
				}			
			}
	}
}

//==========================================================================================================
//!search of the nomenclature of a leaf in a known point in geocoordinates and to leaf scale
void DivideSelfIntersectPolygon(MPtArray *pta, QVector<MPtArray*> *cuttings, QVector<MPtArray*> *polygons)
{
	if(!pta->Size) return;
	MPoint pnt1, pnt2, pnt3, pnt4, cross;
	MPtArray partOfPta;
	bool crossIsPt1, crossIsPt2, crossIsPt3, crossIsPt4;
	int delInd;
	for(int i = 0; i < pta->Size - 3; i++)
	{	
		pnt1 = *pta->GetAt(i);
		pnt2 = *pta->GetAt(i + 1);
		for(int j = i + 2; j < pta->Size - 1; j++)
		{
			pnt3 = *pta->GetAt(j);
			pnt4 = *pta->GetAt(j + 1);
			if(CROSSSECTION(pnt1, pnt2, pnt3, pnt4, &cross))
			{
				crossIsPt1 = CompareTwoMPoint(&pnt1, &cross);
				crossIsPt2 = CompareTwoMPoint(&pnt2, &cross);
				crossIsPt3 = CompareTwoMPoint(&pnt3, &cross);
				crossIsPt4 = CompareTwoMPoint(&pnt4, &cross);
				if(!crossIsPt1 && !crossIsPt2 && !crossIsPt3 && !crossIsPt4)
				{					
					partOfPta.AddTail(&cross);
					delInd = i + 1;
					for(int k = delInd; k <= j; k++)
					{
						partOfPta.AddTail(pta->GetAt(delInd));
						pta->Delete(delInd);
					}
					pta->InsertAfter(i, &cross);
					partOfPta.AddTail(&cross);
					cuttings->push_back((MPtArray *) partOfPta.Clone());
					partOfPta.ClearAll();
					pnt2 = cross;
					j = i + 1;
				}
				else if(crossIsPt2 && crossIsPt4)
				{
					delInd = i + 1;
					for(int k = delInd; k <= j; k++)
					{
						partOfPta.AddTail(pta->GetAt(delInd));
						pta->Delete(delInd);
					}					
					partOfPta.AddTail(&cross);
					cuttings->push_back((MPtArray *) partOfPta.Clone());
					partOfPta.ClearAll();
					j = i + 1;
				}
			}			
		}
	}
	polygons->push_back((MPtArray *) pta->Clone());
	if(cuttings->size() > 0)
	{
		pta = cuttings->at(0);
		cuttings->pop_front();
		DivideSelfIntersectPolygon(pta, cuttings, polygons);
	}
}
void GetGaussNomenklatura(MPoint geopt, unsigned int mashtab, MString& nomenklatura, MString& cifrnomenklatura)
//==========================================================================================================
{

    //here latitudes more than 60 and 76 degrees aren't considered
	nomenklatura = "";
	cifrnomenklatura = "";

    //we will normalize longitude
	geopt.x+= 180.0;
	while(geopt.x<0)
		geopt.x+=360.0;
	while(geopt.x>=360.0)
		geopt.x-=360;


    //we will check a point correctness
	if((geopt.y<-88.0)||(geopt.y>88.0))
		return;

    //if unknown scale
	bool bExistMasst(false);
	for(int i = 0 ;i<MASSTAB_AR_SIZE;++i)
		if(MASSTAB_AR[i]==mashtab){
			bExistMasst = true;break;
		};
	if((!bExistMasst)||(mashtab<50000)) return;

    //at first we will look, what hemisphere
	MString polusharie = "";
	MString cpolusharie = "";

	MString nom = "";
	MString cnom = "";


	MString part;
	MString cpart;

	if(geopt.y<0)
	{
		polusharie = "(Ю. П.)";
		cpolusharie = "9.";
	}

	geopt.y = fabs(geopt.y);
	
	
	

    //we will proceed from this,
    //that the width and longitude can be sampled
    //by number of sheets 25000 which are located in 1 hemisphere

    //in one column 22 sheets 1000000
    //in one sheet 1 000 000 on a vertical 12 sheets 100 000 are located
    //in one sheet 100 000 on a vertical 2 sheets 50 000 are located
    //in one sheet 50 000 2 sheets 25 000 are located
    //thus in one column 1056 sheets 25 000 on 88 degrees are located
 int iy = (int) (fabs (1056.0*geopt.y/88.0));

    //in the same row 60 sheets 1000000
    //in one sheet 1 000 000 on 12 sheets 100 000 are located
    //in one sheet 100 000 on 2 sheets 50 000 are located
    //in one sheet 50 000 2 sheets 25 000 are located
    //thus in one column 2880 sheets 25 000 on 360 degrees are located
	int ix = (int)(fabs(2880.0*geopt.x/360.0));


	MStArray radname;
	radname.SetSize(22);
	MString str = "A"; radname.SetAt(0, &str); 
	        str = "B"; radname.SetAt(1, &str);
			str = "C"; radname.SetAt(2, &str);
	        str = "D"; radname.SetAt(3, &str);
			str = "E"; radname.SetAt(4, &str);
	        str = "F"; radname.SetAt(5, &str);
			str = "G"; radname.SetAt(6, &str);
	        str = "H"; radname.SetAt(7, &str);
			str = "I"; radname.SetAt(8, &str);
	        str = "J"; radname.SetAt(9, &str);
			str = "K"; radname.SetAt(10, &str);
	        str = "L"; radname.SetAt(11, &str);
			str = "M"; radname.SetAt(12, &str);
	        str = "N"; radname.SetAt(13, &str);
			str = "O"; radname.SetAt(14, &str);
	        str = "P"; radname.SetAt(15, &str);
			str = "Q"; radname.SetAt(16, &str);
	        str = "R"; radname.SetAt(17, &str);
			str = "S"; radname.SetAt(18, &str);
	        str = "T"; radname.SetAt(19, &str);
			str = "U"; radname.SetAt(20, &str);
	        str = "V"; radname.SetAt(21, &str);
			
			
    //we will define, to what row we got
    //We won't forget that in the same row 48 sheets 25 000
	int ny = div(iy, 48).quot;
	iy = div(iy, 48).rem;
	if(polusharie!="")
	{
		iy = 47-iy;
	}
	part = *radname.GetAt(ny);
	cpart.Format("%d", ny+1);
	if(cpart.GetLength()==1)
		cpart = "0"+cpart;

    //in what column (too in the same row 48 sheets 25 000)
	int nx = div(ix, 48).quot;
	ix = div(ix, 48).rem;
	str.Format("%d", nx+1);
	part = part+"-"+str;
	if(str.GetLength()==1)
		str = "0"+str;
	cpart = cpart+"-"+str;

    //we already have the million nomenclature
	nom = part;
	cnom = cpart;

    //if ordered 1000 000, everything is already ready
	if(mashtab == MASSTAB_AR[MASSTAB_1000000_INDEX])
	{
		nomenklatura = nom + polusharie;
		cifrnomenklatura = cpolusharie + cnom;
		return;
	}

    //now, if it is necessary to us 500 000
	if(mashtab == MASSTAB_AR[MASSTAB_500000_INDEX])
	{
        //radname.SetSize(4);
        // Преодразовать в windows1251, сейчас utf8

       const QTextCodec *codecWindows1251 = QTextCodec::codecForName("Windows-1251");
       QByteArray ba = codecWindows1251->fromUnicode("АБВГ");//Windows-1251
//        str = "А"; radname.SetAt(0, &str);
//        str = "Б"; radname.SetAt(1, &str);
//        str = "В"; radname.SetAt(2, &str);
//        str = "Г"; radname.SetAt(3, &str);

        //in one leaf of scale 500 000 on a vertical and a horizontal
        //24 sheets 25 000 are located
		nx = 1-div(ix, 24).quot;
		//nx = div(ix, 24).quot;
		ny = div(iy, 24).quot;
        //nomenklatura = nom + "-" + *radname.GetAt(nx*2+ny)+polusharie;
        std::string sss((size_t)1, ba.at(nx*2+ny));
        nomenklatura = nom + "-" + MString(sss)+polusharie;
        ////nomenklatura = nom + "-" + *radname.GetAt((1-ny)*2+nx)+polusharie;
		str.Format("-%d", nx*2+ny+1);
		cifrnomenklatura = polusharie+cnom+str;
		return;
	}

    //now, if it is necessary to us 200 000
	if(mashtab == MASSTAB_AR[MASSTAB_200000_INDEX])
	{
		radname.SetSize(36);

		str = "I"; radname.SetAt(0, &str); 
	    str = "II"; radname.SetAt(1, &str);
		str = "III"; radname.SetAt(2, &str);
	    str = "IV"; radname.SetAt(3, &str);
		str = "V"; radname.SetAt(4, &str);
	    str = "VI"; radname.SetAt(5, &str);
		str = "VII"; radname.SetAt(6, &str);
	    str = "VIII"; radname.SetAt(7, &str);
		str = "IX"; radname.SetAt(8, &str);
	    str = "X"; radname.SetAt(9, &str);
		str = "XI"; radname.SetAt(10, &str);
	    str = "XII"; radname.SetAt(11, &str);
		str = "XIII"; radname.SetAt(12, &str);
	    str = "XIV"; radname.SetAt(13, &str);
		str = "XV"; radname.SetAt(14, &str);
	    str = "XVI"; radname.SetAt(15, &str);
		str = "XVII"; radname.SetAt(16, &str);
	    str = "XVIII"; radname.SetAt(17, &str);
		str = "XIX"; radname.SetAt(18, &str);
	    str = "XX"; radname.SetAt(19, &str);
		str = "XXI"; radname.SetAt(20, &str);
	    str = "XXII"; radname.SetAt(21, &str);
		str = "XXIII"; radname.SetAt(22, &str);
	    str = "XXIV"; radname.SetAt(23, &str);
		str = "XXV"; radname.SetAt(24, &str);
	    str = "XXVI"; radname.SetAt(25, &str);
		str = "XXVII"; radname.SetAt(26, &str);
	    str = "XXVIII"; radname.SetAt(27, &str);
		str = "XXIX"; radname.SetAt(28, &str);
	    str = "XXX"; radname.SetAt(29, &str);
		str = "XXXI"; radname.SetAt(30, &str);
	    str = "XXXII"; radname.SetAt(31, &str);
		str = "XXXIII"; radname.SetAt(32, &str);
	    str = "XXXIV"; radname.SetAt(33, &str);
		str = "XXXV"; radname.SetAt(34, &str);
	    str = "XXXVI"; radname.SetAt(35, &str);
				
        //in one leaf of scale 200 000 on a vertical and a horizontal
        //8 sheets 25 000 are located
        //in one million - on 6 sheets 25 000
		nx = div(ix, 8).quot;
		ny = div(iy, 8).quot;
		nomenklatura = nom + "-" + *radname.GetAt((5-ny)*6+nx)+polusharie;
		str.Format("%d", (5-ny)*6+nx+1);
		if(str.GetLength()==1) 
			str = "0"+str;
		str = "-"+str;
		cifrnomenklatura = polusharie+cnom+str;
		return;
	}

    //now anyway we need to learn the nomenclature 100 000 leaves
    //in one sheet 100 000 is located on a vertical and a horizontal
    //on 4 sheets 25 000
    //and in one million map - on 12 sheets on 100 000
	ny = div(iy, 4).quot;
	iy = div(iy, 4).rem;
	nx = div(ix, 4).quot;
	ix = div(ix, 4).rem;
	str.Format("%d", 12*(11-ny)+nx+1);
	nom = nom + "-"+str;
	if(str.GetLength()==1)
		str = "00"+str;
	else if(str.GetLength()==2)

		str = "0"+str;
	cnom = cnom + "-" +str;

    //if we needed the map on 100 000, it already is
	if(mashtab==MASSTAB_AR[MASSTAB_100000_INDEX])
	{

		nomenklatura = nom+polusharie;
		cifrnomenklatura = cpolusharie+cnom;
		return;

	}

    //now anyway we need to learn the nomenclature 50 000 leaves
    //in one sheet 50 000 is located on a vertical and a horizontal
    //on 2 sheets 25 000
    //and in one map on 100 000 - on 2 sheets on 50 000
	radname.SetSize(4);
	str = "А"; radname.SetAt(0, &str); 
	str = "Б"; radname.SetAt(1, &str);
	str = "В"; radname.SetAt(2, &str);
	str = "Г"; radname.SetAt(3, &str);
	
	ny = div(iy, 2).quot;
	iy = div(iy, 2).rem;
	nx = div(ix, 2).quot;
	ix = div(ix, 2).rem;

	part = *radname.GetAt(2*(1-ny)+nx);
	cpart.Format("%d", 2*(1-ny)+nx+1);
	
	nom = nom + "-"+part;
	cnom = cnom + "-" +cpart;

    //if we needed the map on 50 000, it already is
	if(mashtab==MASSTAB_AR[MASSTAB_50000_INDEX])
	{
		nomenklatura = nom+polusharie;
		cifrnomenklatura = cpolusharie+cnom;
		return;
	}

    //if we lived still, we need the map on 25 000
    //in one sheet 50 000 is located on a vertical and a horizontal
    //on 2 sheets 25 000
	
	radname.SetSize(4);
	str = "а"; radname.SetAt(0, &str); 
	str = "б"; radname.SetAt(1, &str);
	str = "в"; radname.SetAt(2, &str);
	str = "г"; radname.SetAt(3, &str);
	
	part = *radname.GetAt(2*(1-iy)+ix);
	cpart.Format("%d", 2*(1-iy)+ix+1);
	
	nom = nom + "-"+part;
	cnom = cnom + "-" +cpart;

	nomenklatura = nom+polusharie;
	cifrnomenklatura = cpolusharie+cnom;
}

//===================================================
//!Builds the clasping rectangle on the basis of the entrance array
MRect GetObhvatRect(MPtArray* pta)
//===================================================
{
	MRect obhv(MPoint((double)0),MPoint((double)0));
	if(!pta->GetSize())
		return obhv;
	obhv.left = obhv.right = pta->GetHead()->x;
	obhv.top =  obhv.bottom= pta->GetHead()->y;
	for(int i=pta->GetSize()-1;i>0;i--)
		obhv.ExpandToPt(*pta->GetAt(i));
	return obhv;
}

//===================================================
//!Rounds 'number' number to the set quantity of the signs 'col' after a comma
double SetRoundedNumber(double number,int col)
//===================================================
{
	MString str;
	str.Format("%f", number);
    //we will leave significant figures after a comma
	MString strdop = str.SpanExcluding(".");
	str.Delete(0, strdop.GetLength());
	MString lastsym = str.Right(1);
	MString dopsym = lastsym;
    //rounding from a number tail
	for(int i=str.GetLength(); i>col+1; i--){
		str = str.Left(str.GetLength()-1);
		dopsym = str.Right(1);
		if((lastsym=="5")||(lastsym=="6")||(lastsym=="7")||
			(lastsym=="8")||(lastsym=="9")||(lastsym=="10")){
			int n2 = atoi((char *)dopsym);
			n2++;
			lastsym.Format("%d",n2);
		}
		else
			lastsym = str.Right(1);
	}

    //if it is necessary, we round the last removed number figure
	if((lastsym != dopsym)&&(lastsym!="10")){

		str = str.Left(str.GetLength()-1);
		str += lastsym;
	}

	str = strdop + str;

	double roundednum = atof(str);
	return roundednum;
}

int Letter (int nomer, int koef)
{
	int lett = 0;
	int z = nomer/koef;
	int r = nomer%koef;
	if (r != 0)
		z = z+1;

    if (z%2 == 0) //whole part is even number
		{
			if (r%2 == 0)
				lett = 3;
			else
				lett = 2;
		}
	else
		{
			if (r%2 == 0)
				lett = 1;
			else
				lett = 0;
		}
	return lett;
}

MString GetSheetAtPoint(int scale, const MPoint &pt, MRect &rect)
{
MString str = "";
	if (pt.x < -180 || pt.x > 180.0 || pt.y < -90 || pt.y > 90.0)
		return str;

	int sideCnt; //2- for 1:500000 6- for 
	switch (scale)
	{
		case 1000:  sideCnt = 1; break;
		case 500:	sideCnt = 2; break;
		case 200:	sideCnt = 6; break;
		case 100:	sideCnt = 12; break;
		case 50:	sideCnt = 24; break;
		case 25:	sideCnt = 48; break;
		default:	sideCnt = 1;
	}
	const int secInDeg = 3600;
	const MIntSize mlnSz(6,4);
	MIntSize ptSec((int)(pt.x*secInDeg),(int)(pt.y*secInDeg));
	
	int y1 = (int)fabs(pt.y / mlnSz.cy);
	int x1 = (int)fabs((pt.x+180) / mlnSz.cx);	 
	
	double dstToLeft = ptSec.cx>0 ? ptSec.cx%(mlnSz.cx*secInDeg) : mlnSz.cx*secInDeg+ptSec.cx%(mlnSz.cx*secInDeg);
	double dstToBottom = ptSec.cy>0 ? ptSec.cy%(mlnSz.cy*secInDeg): mlnSz.cy*secInDeg+ptSec.cy%(mlnSz.cy*secInDeg);//double dstToTop  = mlnSz.cy*secInDeg - ptSec.cy%(mlnSz.cy*secInDeg);
	
	int ny = (int)(dstToBottom/(mlnSz.cy*secInDeg/sideCnt)); 
	int nx = (int)(dstToLeft/(mlnSz.cx*secInDeg/sideCnt));
	
	rect.bottom = floor(pt.y / mlnSz.cy)*mlnSz.cy + ny*mlnSz.cy/(double)sideCnt; // 
	rect.top = rect.bottom + mlnSz.cy/(double)sideCnt;
	rect.left = floor(pt.x / mlnSz.cx)*mlnSz.cx + nx*mlnSz.cx/(double)sideCnt;
	rect.right = rect.left + mlnSz.cx/(double)sideCnt;
	
	str.Format("%c%d%03d", y1 + 'A', x1 + 1, (sideCnt-1-abs(ny))*sideCnt+abs(nx)+1);
	switch (scale)
	{
		case 1000:  
			str.Format("%c%d", y1 + 'A', x1 + 1);
			break;
		case 500:	
			str.Format("%c%d%01d", y1 + 'A', x1 + 1, (sideCnt-1-abs(ny))*sideCnt+abs(nx)+1);
			break;
		case 200:	
			str.Format("%c%d%02d", y1 + 'A', x1 + 1, (sideCnt-1-abs(ny))*sideCnt+abs(nx)+1);
			break;
		case 100:	
			str.Format("%c%d%03d", y1 + 'A', x1 + 1, (sideCnt-1-abs(ny))*sideCnt+abs(nx)+1);			
			break;
			case 50:	
			{
			int ny100 = (int)(dstToBottom/(mlnSz.cy*secInDeg/(sideCnt/2))); 
			int nx100 = (int)(dstToLeft/(mlnSz.cx*secInDeg/ (sideCnt/2)));
								
			int ny = (int)(dstToBottom/(mlnSz.cy*secInDeg/sideCnt)); 
			int nx = (int)(dstToLeft/(mlnSz.cx*secInDeg/sideCnt));

			rect.bottom = floor(pt.y / mlnSz.cy)*mlnSz.cy + ny*mlnSz.cy/(double)(sideCnt);
			rect.top = rect.bottom + mlnSz.cy/(double)(sideCnt);
			rect.left = floor(pt.x / mlnSz.cx)*mlnSz.cx + nx*mlnSz.cx/(double)(sideCnt);
			rect.right = rect.left + mlnSz.cx/(double)(sideCnt);
	
			str.Format("%c%d%03d%c",y1 + 'A', x1 + 1, ((sideCnt/2)-1-abs(ny100))*(sideCnt/2)+abs(nx100)+1,
														Letter((sideCnt-1-abs(ny))*sideCnt+abs(nx)+1, sideCnt) + 'A');
		
			}
		break;

		case 25:
			{
					
			int ny100 = (int)(dstToBottom/(mlnSz.cy*secInDeg/(sideCnt/4))); 
			int nx100 = (int)(dstToLeft/(mlnSz.cx*secInDeg/(sideCnt/4)));

			int ny50 = (int)(dstToBottom/(mlnSz.cy*secInDeg/(sideCnt/2))); 
			int nx50 = (int)(dstToLeft/(mlnSz.cx*secInDeg/(sideCnt/2)));

			int ny = (int)(dstToBottom/(mlnSz.cy*secInDeg/sideCnt)); 
			int nx = (int)(dstToLeft/(mlnSz.cx*secInDeg/sideCnt));

			rect.bottom = floor(pt.y / mlnSz.cy)*mlnSz.cy + ny*mlnSz.cy/(double)(sideCnt); 
			rect.top = rect.bottom + mlnSz.cy/(double)(sideCnt);
			rect.left = floor(pt.x / mlnSz.cx)*mlnSz.cx + nx*mlnSz.cx/(double)(sideCnt);
			rect.right = rect.left + mlnSz.cx/(double)(sideCnt);	

			str.Format("%c%d%03d%c%c",y1 + 'A', x1 + 1, ((sideCnt/4)-1-abs(ny100))*(sideCnt/4)+abs(nx100)+1,
															Letter(((sideCnt/2)-1-abs(ny50))*(sideCnt/2)+abs(nx50)+1, (sideCnt/2)) + 'A',
																Letter((sideCnt-1-abs(ny))*sideCnt+abs(nx)+1, sideCnt) + 'a');
			
			}
		break;
	}
	return str;


}

//===================================================
//! Returns number 'MString' of a leaf of the map of scale 1:100000
//! and its describing 'MRect' rectangle (geo) for 'MPoint' point (geo)
MString GetSheetAtPoint1(MPoint &pt, MRect &rect)
{
	return GetSheetAtPoint(100, pt, rect);
}

//===================================================
//! Returns number 'MString' of a leaf of the map of scale 1:200000
//! and its describing 'MRect' rectangle (geo) for 'MPoint' point (geo)
MString GetSheetAtPoint2(MPoint &pt, MRect &rect)
{
	return GetSheetAtPoint(200, pt, rect);
}

//===================================================
//! Returns number 'MString' of a leaf of the map of scale 1:500000
//! and its describing 'MRect' rectangle (geo) for 'MPoint' point (geo)
MString GetSheetAtPoint5(MPoint &pt, MRect &rect)
{
	return GetSheetAtPoint(500, pt, rect);
}

//====================================
//! Rearranges bytes at the 'val' variable 'len' length upside-down
//! Returns 'true' in case of success, 'false' - differently.
//

bool reverseValue(char* val, int len) {

  if (len <2)   return false;
  if (len >256) return false;
  if ( ! val)   return false;
  int n = len >>1;
  for (int j= 0; j<n; j++ )  {
    char ch  = (char) *(val+j);
    *(val+j) = (char) *(val+ len -j -1);
    *(val+ len -j -1) = ch;   
  }
  return true;
}
//====================================
bool IntersectGeoRectFast(MRect& grect,MRect& drect)
{
    grect.NormalizeGeoRect();
    drect.NormalizeGeoRect();
    if (grect.left>=drect.left)
    {
        if (grect.left<=drect.right)
        {
            if (grect.top>=drect.top)
            {
                if (grect.bottom<=drect.top)
                {
                    return true;
                }
                else{} // return false;
            }
            else
            {
                if (grect.top>=drect.bottom)
                {
                    return true;
                }
                else{} // return false;
            }
        }
        else{} // return false;
    }
    else   // grect.left < drect.left
    {
        if (drect.left<=grect.right)
        {
            if (drect.top>=grect.top)
            {
                if (drect.bottom<=grect.top)
                {
                    return true;
                }
                else{} // return false;
            }
            else
            {
                if (drect.top>=grect.bottom)
                {
                    return true;
                }
                else{} // return false;
            }
        }
        else{}  // return false;
    }

    return false;
}

//====================================
bool isRectInRect(MRect& grect,MRect& drect) {
		if (grect.left >= drect.left)
		  if (grect.right <= drect.right)
		    if (grect.top <= drect.top)
		      if (grect.bottom >= drect.bottom)
             return true;
		return false;
	}
  
//====================================
bool isPolylineIntersectsRect(MPtArray *pta, MRect& rect) {
	if(!pta->GetSize())  return false;
  MPoint pt1, pt2;
	for(int i=0; i<pta->GetSize()-1; i++){
    pt1= *pta->GetAt(i);
    pt2= *pta->GetAt(i+1);
  	MRect obhv(pt1, pt2);
    obhv.NormalizeGeoRect();
    if (IntersectGeoRectFast(obhv, rect)) return true;
	}
  return false;
}

//========================================================
//FUNCTION		:Lagrange
//
//DESCRIPTION	:Returns the array of the points setting a segment of an approximating curve Lagrange's by method
//               'pta' - the array of knots on which the curve is under construction (in to Gauss-Krueger)
//               LAGRANGE_NUMBER = 10
//Author        :MAK
MPtArray* Lagrange(MPtArray* pta, int number)
//========================================================
{
	MPtArray* ptNew = new MPtArray();
		
	if(pta == NULL) return ptNew;
	int size = pta->GetSize();
	if(size < 2) return ptNew;
	MPoint* pt1 = pta->GetAt(0);
	ptNew->AddHead(pt1);

	 
	MPoint* pt2; 
	double shag;
	MPoint point;
//	point.y = pt1->y;
		
	for(int i=0; i<size-1; i++)
	{
		pt1 = pta->GetAt(i);
		pt2 = pta->GetAt(i+1);
		point.x = pt1->x;
		shag = (pt2->x - pt1->x)/number;
		for(int j=1; j<number-1; j++)
		{			
			point.x += shag;
			point.y = UzelLagrange(point.x, pta);
			ptNew->AddTail(&point);
		}
		ptNew->AddTail(pt2);		
	}
	
	return ptNew;
}

//========================================================
//FUNCTION		:UzelLagrange
//
//DESCRIPTION	:Lagrange's coefficients(is used in GetLagrange())
//
//Author        :MAK
double UzelLagrange(double x, MPtArray* pta)
{
	double L=0;
	int pSize = pta->GetSize();
    //cycle on coefficients
	for(int j=0; j<pSize;j++)
	{
        double Lj = 1;//each coefficient
		for(int i=0;i<pSize;i++)
		{
			if(i==j) continue;
			Lj = Lj*(x - pta->GetAt(i)->x)/(pta->GetAt(j)->x - pta->GetAt(i)->x);		
		}
		L += Lj*pta->GetAt(j)->y;
	}	

	return L;
}

//========================================================
//returns 'rectReturn' = a rectangle of crossing of 'rect1' and 'rect2'
bool IntersectionGeoRect(MRect* rect1, MRect* rect2, MRect& rectReturn)
//========================================================
{
	if(!(rect1->IntersectGeoRect(*rect1,*rect2))) return false;
	else 
	{
		if(rect1->left>rect2->left) rectReturn.left = rect1->left;
		else rectReturn.left = rect2->left;
		if(rect1->top>rect2->top) rectReturn.top = rect2->top;
		else rectReturn.top = rect1->top;
		if(rect1->right>rect2->right) rectReturn.right = rect2->right;
		else rectReturn.right = rect1->right;
		if(rect1->bottom>rect2->bottom) rectReturn.bottom = rect1->bottom;
		else rectReturn.bottom = rect2->bottom;
	}	
	return true;
}

//=======================================================
//returns an index of top of a broken line of 'pLomline' coinciding with 'point' with the set accuracy
int FindNomVershinaOfPointOnLomlineExt(MPtArray* pLomline, MPoint point, double eps)
//=======================================================
{
	if(pLomline == NULL) return -1;
	
	int cnt = pLomline->GetSize();
	
	if(cnt==0) return -1;
	
	for(int i=0; i<cnt;i++)
		if(CompareTwoMPoint(&point,pLomline->GetAt(i),eps))	return i;
		
		return -1;
}

//=======================================================
//true, if two points lie in the 'eps' vicinity
bool CompareTwoMPoint(const MPoint* pt1, const MPoint* pt2, double eps)
//=======================================================
{
	//	if((fabs(pt1->x-pt2->x)<eps)&&(fabs(pt1->y-pt2->y)<eps)) return true;
	//	else return false;
	if(DLINASG(*pt1,*pt2)<eps) return true;
	else return false;
}

//=======================================================
//returns 'pta' broken line top index, the next to 'point'
int FindIndexOfNearestPoint(MPtArray* pta, MPoint* point)
//=======================================================
{
	if(!pta) return -1;
	
	int iSize = pta->GetSize();
	if(iSize == 0) return -1;
	
	double rasst = DLINASG(*pta->GetAt(0), *point);
	int result = 0;
	for(int i=1; i<iSize; i++)
	{
		double dlina = DLINASG(*pta->GetAt(i), *point);
		if(dlina<rasst)
		{
			rasst = dlina;
			result = i;
		}
	}
	return result;
}

//=======================================================
//cleaning of the array from the duplicated points
void DeletingRepeatedPoints(MPtArray* pta)
//=======================================================
{
	if(!pta) return;
	
	int iSize = pta->GetSize();	
	for(int i=0; i<iSize; i++)
	{
		MPoint point = *pta->GetAt(i);
		for(int j=iSize-1; j>i; j--)
		{			
			if(CompareTwoMPoint(&point, pta->GetAt(j)))
			{
				pta->Delete(j);
				iSize--;
			}
		}		
	}
}

void DeletingRepeatedPoints(MHlArray* pta)
//=======================================================
{
    if(!pta) return;

    int iSize = pta->GetSize();
    for(int i=0; i<iSize; i++)
    {
        MPoint point = *pta->GetAt(i);
        for(int j=iSize-1; j>i; j--)
        {
            if(CompareTwoMPoint(&point, pta->GetAt(j)))
            {
                pta->Delete(j);
                iSize--;
            }
        }
    }
}
//
unsigned char dos2win(unsigned char c)
{
 unsigned char result= c;
 if ( (c>=224) && (c<240) )    result +=16;
 if ( (c>=128) && (c<176) )    result +=64;
 if (  c==0xf8 )               result  = 0xb0; // degree
 return result;
}
// -------------------------
unsigned char win2dos(unsigned char c)  // And what about other cyrrilic letters (BY,UA)
{
 unsigned char result= c;
 if ( (c>=192) && (c<240) )    result -= 64;
 if ( (c>=240) && (c<256) )    result -= 16;
 if (  c==0xa8 )               result  = 0x85; // YO -> E
 if (  c==0xb8 )               result  = 0xa5; // yo -> e
 if (  c==0xb0 )               result  = 0xf8; // degree
return result;
}
// -------------------------
unsigned char dos2koi(unsigned char c) {
// degree: f8->9c  
unsigned char DosToKoi[]
     ={
       0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
       0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
       0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
       0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
       0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
       0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f, // ff->5e
       0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
       0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f, // ff->7e
       0xE1,0xE2,0xF7,0xE7,0xE4,0xE5,0xF6,0xFA,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,0xF0,
       0xF2,0xF3,0xF4,0xF5,0xE6,0xE8,0xE3,0xFE,0xFB,0xFD,0xFF,0xF9,0xF8,0xFC,0xE0,0xF1,
       0xC1,0xC2,0xD7,0xC7,0xC4,0xC5,0xD6,0xDA,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,
       0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
       0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
       0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
       0xD2,0xD3,0xD4,0xD5,0xC6,0xC8,0xC3,0xDE,0xDB,0xDD,0xDF,0xD9,0xD8,0xDC,0xC0,0xD1,
       0xE5,0xC5,0x20,0x20,0x20,0x20,0x20,0x20,0x9c,0x20,0x20,0x20,0x20,0x24,0x20,0x20,  // і Ј -?
			};
return  DosToKoi [c];
}
// -------------------------
//
unsigned char koi2dos(unsigned char c) {
// degree: 9c->f8  
unsigned char KoiToDos[]   ={
       0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
       0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
			 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
			 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
			 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
			 0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
			 0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
			 0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
			 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
			 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0xf8,0x20,0x20,0x20,
			 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
			 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
			 0xEE,0xA0,0xA1,0xE6,0xA4,0xA5,0xE4,0xA3,0xE5,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,
			 0xAF,0xEF,0xE0,0xE1,0xE2,0xE3,0xA6,0xA2,0xEC,0xEB,0xA7,0xE8,0xED,0xE9,0xE7,0xEA,
       0x9E,0x80,0x81,0x96,0x84,0x85,0x94,0x83,0x95,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,
       0x8F,0x9F,0x90,0x91,0x92,0x93,0x86,0x82,0xEC,0x9B,0x87,0x98,0x9D,0x99,0x97,0x9C,
			};
return KoiToDos [c];
}
//
unsigned char koi2win(unsigned char c) {
  unsigned char result= koi2dos(c);
  return dos2win(result);
}    
unsigned char win2koi(unsigned char c) {
  unsigned char result= win2dos(c);
  return dos2koi(result);
}
//-------------------------
void dos2win( char* ptr) {
  while ((unsigned char)*ptr != 0x00) {
    *ptr= dos2win((unsigned char)*ptr);
    ptr++;
  }
}
void win2dos( char* ptr) {
  while ((unsigned char)*ptr != 0x00) {
    *ptr= win2dos((unsigned char)*ptr);
    ptr++;
  }
}
void koi2dos( char* ptr) {
  while ((unsigned char)*ptr != 0x00) {
    *ptr= koi2dos((unsigned char)*ptr);
    ptr++;
  }
}
void dos2koi( char* ptr) {
  while ((unsigned char)*ptr != 0x00) {
    *ptr= dos2koi((unsigned char)*ptr);
    ptr++;
  }
}
void koi2win( char* ptr) {
  while ((unsigned char)*ptr != 0x00) {
    *ptr= koi2win((unsigned char)*ptr);
    ptr++;
  }
}
void win2koi( char* ptr) {
  while ((unsigned char)*ptr != 0x00) {
    *ptr= win2koi((unsigned char)*ptr);
    ptr++;
  }
}

bool isNaN( double r )
{
   return r != r;
}

double round(double x, int decimals)
{
	double power = pow(10.0, decimals);
	return ((x < 0) ? ceil(x * power - 0.5) : floor(x * power + 0.5)) / power;
}


QVector<MPoint> SimpleLine(QVector<MPoint> ptList, double epsilon)
{
	///
	if(ptList.size() < 3)
		return ptList;

	if(epsilon == 0)
	{
		int ptCount = ptList.size();
		double dist = DLINASG(ptList.first(), ptList.last());
		for(int i = 1; i < ptCount; ++i)
			dist += DLINASG(ptList[i - 1], ptList[i]);
		epsilon = dist / 500;
	}
	///

	QVector<MPoint> result;
	double dmax = 0.0;
    int index = 0;
	int end = ptList.size() - 1;

	double d;
    for (int i = 1; i < end; i++)
	{
        //d = shortestDistanceToSegment(PointList[i], Line(PointList[1], PointList[end])) 
		d = DLINASG(ptList[i], GetProdComponents(ptList[0], ptList[end], ptList[i]));
		if (d > dmax)
		{
            index = i;
            dmax = d;
        }
    }
    // If max distance is greater than epsilon, recursively simplify
    if (dmax >= epsilon)
	{
		QVector<MPoint> vect1(ptList);
		QVector<MPoint> vect2(ptList);
		vect1.remove(index + 1, vect1.size() - index - 1);
		vect2.remove(0, index);
        // Recursive call
		vect1 = SimpleLine(vect1, epsilon);
		vect2 = SimpleLine(vect2, epsilon);
        // Build the result list
		result = vect1;
		result.pop_back();
		result += vect2;
    } 
	else 
	{
		result.append(ptList.first());
		result.append(ptList.last());
    }
    // Return the result
	return result;
}
double GetAzimuthByTwoPoints(const MPoint &ppt1, const MPoint &ppt2, double poluos1, double poluos2)
{
    const MGeodesic geod(poluos1, poluos1/(poluos1-poluos2));
    double s12,azi1,azi2;
    geod.Inverse(ppt1.y, ppt1.x, ppt2.y, ppt2.x, s12, azi1, azi2);
    return azi1;
}
int ConvertAzimuthToFold(const double& dAzimuth, const int& azimuthDiscrete)
{
    const double dAzD = (double)azimuthDiscrete;
    double azimuthC;
    double azimuthD;
    azimuthD = modf(dAzimuth, &azimuthC);
    int c = 0;
    if(azimuthD < 0.5)
    {
        c = (int)azimuthC;
    }
    else
    {
        c = (int)azimuthC + 1;
    }
    int azres = 0;
    if(c % azimuthDiscrete == 0)
    {
        azres = c;
    }
    else
    {
        const double dres = azimuthC/dAzD;
        double cc;
        const double ost = modf(dres, &cc);
        if(ost < dAzD/2)
        {
            azres = cc*azimuthDiscrete;
        }
        else
        {
            azres = cc*azimuthDiscrete + azimuthDiscrete;
        }
    }
    return azres;
}

int BringToMultiple(const int iNumber, const int iMultiple)
{
    int result = 0;

    int integerPart = iNumber/iMultiple;
    int fractionPart = iNumber%iMultiple;

    result = integerPart * iMultiple;

    if(fractionPart > iMultiple/2)
    {
        result += iMultiple;
    }
    return result;
}
double FindM3DPointsDistance(M3DPoint firstPoint, M3DPoint secondPoint)
{
  double distance;
  distance = sqrt(pow(secondPoint.x - firstPoint.x, 2) +
                  pow(secondPoint.y - firstPoint.y, 2) +
                  pow(secondPoint.z - firstPoint.z, 2));
  return distance;
}
double FindTriangleAngle(double firstAngleSide, double secondAngleSide, double thirdSide)
{
  double angle;
  angle = acos((pow(firstAngleSide, 2) + pow(secondAngleSide, 2) - pow(thirdSide, 2)) /
                    (2 * firstAngleSide * secondAngleSide)) * 180 / pi;
  return angle;
}
double ConvertDegreesToRadians(double value)
{
  return value * pi / 180.0;
}

double ConvertRadiansToDegrees(double value)
{
    return value * 180 / pi;
}

void ConvertVectorMPointToMPtArray(const std::vector<MPoint> &vectorMPoint, MPtArray &ptArray)
{
    for(int i = 0; i < vectorMPoint.size(); i++)
    {
        MPoint pt = vectorMPoint.at(i);
        ptArray.AddTail(&pt);
    }
}

void DeletingRepeatedPoints(std::vector<MPoint>& points)
{
    if(points.size() == 0) return;

    int iSize = points.size();
    for(int i=0; i<iSize; i++)
    {
        MPoint point = points.at(i);
        for(int j=iSize-1; j>i; j--)
        {
            if(CompareTwoMPoint(&point, &points.at(j)))
            {
                points.erase(points.begin() + j);
                iSize--;
            }
        }
    }
}
