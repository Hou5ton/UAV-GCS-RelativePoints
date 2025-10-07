//PACKAGE : MAP.
//FILE: Globals.h   
//AUTHOR:
//DESCRIPTION:

#ifndef NUXGLOBALS_H
#define NUXGLOBALS_H

//===============================================================================
//  Global functions (H)
//  This file includes all additional static functions
//===============================================================================

#include <iosfwd>
#include "SysDefine.h"
#include "Array.h"
#include "Layer.h"
#include "MString.h"
#include "PlotDevice.h"

// Functions uses pointers to objects of the following calls:
class MPoint;
class MRect;
class MArArray;
class MPtArray;
class MDuArray;
class MInArray;
class MCoo;
class MPrk;
class MStArray;
class MRkArray;
class MPlotDevice;
class MTopobject;

// Splitter between lines
const char separator = 0x09;

#define EPSILON 0.000001

extern  double         axis_X,  axis_Y;

//  Structure describes geographic coordinates
typedef struct{
	 int znak;
	 double grad;
	 double min;
     double sec;
}Coord;

// ------------------------------------------
//  READING-WRITING BASE TYPES
// (Static funcions of saving)
// ------------------------------------------

//PACKAGE : MAP.
//FUNCTION: WriteString(FILE* h_nuxfile, int version, MString str).
//DESCRIPTION: Saves line to UTP-file.
//INPUTS : FILE pointer, int , MString .
//RETURNS : True.
bool WriteString(FILE* h_nuxfile, int version, MString str);

//PACKAGE : MAP.
//FUNCTION: ReadString(FILE* h_nuxfile, int version).
//DESCRIPTION: Reads line from UTP-format file.
//INPUTS : FILE pointer, int .
//RETURNS : MString.
MString ReadString(FILE* h_nuxfile, int version);

//PACKAGE : MAP.
//FUNCTION: ReadString(MFile* file, int version).
//DESCRIPTION: Reads line from UTP-format file.
//INPUTS : MFile pointer, int .
//RETURNS : MString.
MString ReadString(MFile* file, int version);


// Functions block of array and bse type reading/writing
bool WriteStrArray(FILE* h_nuxfile,  int version, MStArray* strar);
bool ReadStrArray(FILE* h_nuxfile,  int version, MStArray* strar);
bool ReadStrArray(MFile* file,  int version, MStArray* strar);
bool WritePoint(FILE* h_nuxfile,  int version, MPoint* pnt);
bool ReadPoint(FILE* h_nuxfile,  int version, MPoint* pnt);
bool ReadPoint(MFile* file,  int version, MPoint* pnt);
bool WriteRect(FILE* h_nuxfile,  int version, MRect* rect);
bool ReadRect(FILE* h_nuxfile,  int version, MRect* rect);
bool ReadRect(MFile* file,  int version, MRect* rect);
bool WriteArrayPoints(FILE* h_nuxfile,  int version, int *pkol, MPoint* pnt);
bool ReadArrayPoints(FILE* h_nuxfile,  int version, int *pkol, MPoint** pnt);
bool ReadArrayPoints(MFile* file,  int version, int *pkol, MPoint** pnt);
bool WriteArrayNambers(FILE* h_nuxfile,  int version, int *nkol, int* nbr);
bool ReadArrayNambers(FILE* h_nuxfile,  int version, int *nkol, int** nbr);
bool ReadArrayNambers(MFile* file,  int version, int *nkol, int** nbr);
bool WriteColor(FILE* h_nuxfile,  int version, MColor* color);
bool ReadColor(FILE* h_nuxfile,  int version, MColor* color);
bool ReadColor(MFile* file,  int version, MColor* color);
bool WritePen(FILE* h_nuxfile,  int version, M_LOG_PEN* lpen);
bool ReadPen(FILE* h_nuxfile,  int version, M_LOG_PEN* lpen);
bool ReadPen(MFile* file,  int version, M_LOG_PEN* lpen);
// ------------------------------------------
//  The end. (Static functions of saving)
// ------------------------------------------

//PACKAGE : MAP.
//FUNCTION : FindM3DPointsDistance(M3DPoint firstPoint, M3DPoint secondPoint)
//DESCRIPTION : Сalculates distance between two M3DPoints.
//INPUTS : M3DPoint, M3DPoint.
//RETURNS : Double.
double FindM3DPointsDistance(M3DPoint firstPoint, M3DPoint secondPoint);

//PACKAGE : MAP.
//FUNCTION : ConvertDegreesToRadians(double value)
//DESCRIPTION : Сonverts degrees to radians.
//INPUTS : Double.
//RETURNS : Double.
double ConvertDegreesToRadians(double value);

//PACKAGE : MAP.
//FUNCTION : ConvertDegreeToRadian(double value)
//DESCRIPTION : Сonverts radians to degrees.
//INPUTS : Double.
//RETURNS : Double.
double ConvertRadiansToDegrees(double value);

//PACKAGE : MAP.
//FUNCTION : FindTriangleAngle(double firstAngleSide, double secondAngleSide, double thirdSide).
//DESCRIPTION : Сalculates triangle's angle in degrees.
//INPUTS : Double, Double, Double.
//RETURNS : Double.
double FindTriangleAngle(double firstAngleSide, double secondAngleSide, double thirdSide);

//PACKAGE : MAP.
//FUNCTION: RegisterCreateArrayByType(MArray*(*func)(MString)).
//DESCRIPTION: Binds functions of special type array creating.
//INPUTS : MArray*(*func)(MString).
//RETURNS : NONE.
void RegisterCreateArrayByType(MArray*(*func)(MString));

//PACKAGE : MAP.
//FUNCTION: RegisterCreateLayerByType(MLayer*(*func)(MString)).
//DESCRIPTION: Binds functions of special type layer creating.
//INPUTS : MLayer*(*func)(MString).
//RETURNS : NONE.
void RegisterCreateLayerByType(MLayer*(*func)(MString));

//PACKAGE : MAP.
//FUNCTION: RegisterCreateObjectByType(MObject*(*func)(MString)).
//DESCRIPTION: Binds functions of special type object creating.
//INPUTS : MObject*(*func)(MString).
//RETURNS : NONE.
void RegisterCreateObjectByType(MObject*(*func)(MString));

//PACKAGE : MAP.
//FUNCTION: RegisterCreateZnakByType(MZnak*(*func)(MString)).
//DESCRIPTION: Binds functions of special type sign creating.
//INPUTS : MZnak*(*func)(MString).
//RETURNS : NONE.
void RegisterCreateZnakByType(MZnak*(*func)(MString));

//PACKAGE : MAP.
//FUNCTION: RegisterConvertSizeToReal(double (*func)(MTopobject*,float)).
//DESCRIPTION: Binds function of size conversion.
//INPUTS : double (*func)(MTopobject*,float).
//RETURNS : NONE.
void RegisterConvertSizeToReal(double (*func)(MTopobject*,float));

//PACKAGE : MAP.
//FUNCTION: SizeToReal(MTopobject* pObj,float fVal).
//DESCRIPTION: Converts size stored in classifier to real size.
//INPUTS : MTopobject* pObj,float fVal.
//RETURNS : NONE.
double SizeToReal(MTopobject* pObj,float fVal);

//PACKAGE : MAP.
//FUNCTION: RegisterOutputProgress(void(*pFunc)()).
//DESCRIPTION: Set function for progress output (implementation in Karta.cpp).
//INPUTS : void(*pFunc)().
//RETURNS : NONE.
void RegisterOutputProgress(void(*pFunc)(const std::string&));

// ******************************************
//  ALGEBRAIC ADDITIONAL FUNCTIONS
// ******************************************

//PACKAGE : MAP.
//FUNCTION: ConvertGaussPoint(MPoint* point).
//DESCRIPTION: Converts point coordinates to view as system requesting
// 				(in Gauss-Cruger -- swap X & Y).
//INPUTS : MPoint pointer.
//RETURNS : NONE.
void ConvertGaussPoint(MPoint* point);

//PACKAGE : MAP.
//FUNCTION: ConvertGaussPoint(MPoint& point).
//DESCRIPTION: Converts point coordinates to view as system requesting
// 				(in Gauss-Cruger -- swap X & Y).
//INPUTS : MPoint& .
//RETURNS : MPoint.
MPoint ConvertGaussPoint(MPoint& point);

//PACKAGE : MAP.
//FUNCTION: RotateByAngle(MPoint* pta,int iPtNum,MPoint ptRotateAround,double radangle).
//DESCRIPTION: Rotates points array around fixed point by defined angle.
// 				Using for rotation of formulas snaps.
//INPUTS : MPoint pointer,int ,MPoint ,double .
//RETURNS : NONE.
void RotateByAngle(MPoint* pta,int iPtNum,MPoint ptRotateAround,double radangle);

//PACKAGE : MAP.
//FUNCTION: CROSSLINES(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt).
//DESCRIPTION: Definition of point of two straight lines crossing coordinates.
//INPUTS : MPoint , MPoint , MPoint , MPoint , MPoint pointer.
//RETURNS : True.
bool   CROSSLINES(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt);

//PACKAGE : MAP.
//FUNCTION: CROSSLS(double a1, double b1, double c1, double a2, double b2, double c2, MPoint* pt).
//DESCRIPTION: Definition of point of two straight lines crossing coordinates defined by
//				  equations : A1*x + B1*y + C1 = 0 I A2*x + B2*y + C2 = 0.
//INPUTS : double , double , double , double , double , double , MPoint pointer.
//RETURNS : True.
bool   CROSSLS(double a1, double b1, double c1, double a2, double b2, double c2, MPoint* pt);

//PACKAGE : MAP.
//FUNCTION:  OTS(MPoint pt1, MPoint pt2, MPoint pt3).
//DESCRIPTION: Definition of oriented square of triangle.
//INPUTS : MPoint , MPoint , MPoint .
//RETURNS : Double.
double    OTS(MPoint pt1, MPoint pt2, MPoint pt3);

//PACKAGE : MAP.
//FUNCTION: COOPT(MPoint pt1, MPoint pt2, double h1, double h2, double c).
//DESCRIPTION: Definition of coordinates of point with height c, laying on a segment
// 				with the ends pt1 and pt2 and heights of the ends of a segment h1 and h2 accordingly.
//INPUTS : MPoint , MPoint , double , double , double .
//RETURNS : MPoint.
MPoint    COOPT(MPoint pt1, MPoint pt2, double h1, double h2, double c);

//PACKAGE : MAP.
//FUNCTION:  HGTPT(MPoint pt1, MPoint pt2, MPoint pt, double h1, double h2).
//DESCRIPTION: Definition of height of a point pt with coordinates pt.x and pt.y, laying on a segment
// 				with the ends pt1 and pt2 and heights of the ends of a segment h1 and h2 accordingly.
//INPUTS : MPoint , MPoint , MPoint , double , double .
//RETURNS : Double.
double	  HGTPT(MPoint pt1, MPoint pt2, MPoint pt, double h1, double h2);

//PACKAGE : MAP.
//FUNCTION:  COS3Points(MPoint pt1, MPoint pt2, MPoint pt3).
//DESCRIPTION: Definition of cosinus of angle by three points on the plane.
//INPUTS : MPoint , MPoint , MPoint .
//RETURNS : Double.
double COS3Points(MPoint pt1, MPoint pt2, MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  COS2Vector(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4).
//DESCRIPTION: Definition of cosinus of angle between two vector on the plane
// 				Every vector defined by two ends.
//INPUTS : MPoint , MPoint , MPoint , MPoint .
//RETURNS : Double.
double COS2Vector(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4);

//PACKAGE : MAP.
//FUNCTION:  ZNAK(double d).
//DESCRIPTION: Definition of number sign.
//INPUTS : double .
//RETURNS : Int.
int       ZNAK(double d);

//PACKAGE : MAP.
//FUNCTION:  TANGV(MPoint pt1, MPoint pt2, MPoint pt3, double cos, int p, int q, int s).
//DESCRIPTION: Definition of coordinates of the vector laying on a perpendicular
// 				to a bisector between two given vectors	with the common beginning in a point pt1..
//INPUTS : MPoint , MPoint , MPoint , double , int , int , int .
//RETURNS : MPoint.
MPoint    TANGV(MPoint pt1, MPoint pt2, MPoint pt3, double cos, int p, int q, int s);

//PACKAGE : MAP.
//FUNCTION:  DERIV2(MTopobject* pTpo).
//DESCRIPTION: Definition of values of the second derivative functions,
// 				interpolating horizontals by cubic splines
// 				in nodes ptj.x, where j varies from 0 up to points quantity
// 				in a horizontal minus unit (see realization of function).
//INPUTS : MTopobject pointer.
//RETURNS : Double pointer.
double*   DERIV2(MTopobject* pTpo);

//PACKAGE : MAP.
//FUNCTION:  RASSTPTL(MPoint pt, MPoint pt1, MPoint pt2).
//DESCRIPTION: Definition of distance from point pt till the straight,	going through two points: pt1 and pt2.
//INPUTS : MPoint , MPoint , MPoint .
//RETURNS : Double.
double    RASSTPTL(MPoint pt, MPoint pt1, MPoint pt2);

//PACKAGE : MAP.
//FUNCTION:  DLINASG(MPoint pt1, MPoint pt2).
//DESCRIPTION: Definition of distance between two points : pt1 and pt2.
//INPUTS : MPoint , MPoint .
//RETURNS : Double.
double    DLINASG(const MPoint &pt1, const MPoint &pt2) ;

//PACKAGE : MAP.
//FUNCTION:  DLINALOMLINE(MPtArray* pLomline).
//DESCRIPTION: Definition of lenght of broken line.
//INPUTS : MPtArray pointer.
//RETURNS : Double.
double DLINALOMLINE(const MPtArray* pLomline);

//PACKAGE : MAP.
//FUNCTION:  NORMTOPLANE(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, //		double z3, double* norm).
//DESCRIPTION: Definition of perpendicular coordinates to the plane defined
// 				by three points pt1, pt2, pt3.
//INPUTS : double , double , double , double , double , double , double , double , double , double pointer.
//RETURNS : True.
bool NORMTOPLANE(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double* norm);

//PACKAGE : MAP.
//FUNCTION:  HLINSYST(double* pA, double* utvector, int n).
//DESCRIPTION: Definition of the decision of linear equations system with symmetric
// 				five diagonal matrix by Hmelnitski method
// 				for construction approximated cubic splines
// 				with boundary conditions of the first or second type.
//INPUTS : double pointer, double pointer, int .
//RETURNS : Double pointer.

double* HLINSYST(double* pA, double* utvector, int n);

//PACKAGE : MAP.
//FUNCTION:  HLINSYST3(double* pA, double* utvector, int n).
//DESCRIPTION: Definition of decision of linear equations with symmetric
// 				matrix by Hmelnitski method
// 				for construction approximated cubic splines
// 				with boundary conditions of third type.
//INPUTS : double pointer, double pointer, int .
//RETURNS : Double pointer.
double* HLINSYST3(double* pA, double* utvector, int n);

//PACKAGE : MAP.
//FUNCTION:  PointsAreSame(const MPoint *pt1, const MPoint *pt2, const double &eps = EPSILON)
//DESCRIPTION: Return true if the distance between two points less then eps
//INPUTS : MPoint , MPoint , MPoint , MPoint pointer, double pointer.
//RETURNS : bool.
bool PointsAreSame(const MPoint *pt1, const MPoint *pt2, const double &eps = EPSILON);
//FUNCTION:  CIRCLEPT(MPoint pt1, MPoint pt2, MPoint pt3, MPoint* pt, double* rad).
//DESCRIPTION: Definition of coordinates circle centre, described around triangle
// 				with vertexes at points P1=(x1,y1), P2=(x2,y2), P3=(x3,y3).
//INPUTS : MPoint , MPoint , MPoint , MPoint pointer, double pointer.
//RETURNS : True.
bool CIRCLEPT(MPoint pt1, MPoint pt2, MPoint pt3, MPoint* pt, double* rad);

//PACKAGE : MAP.
//FUNCTION:  SGINREG(MPoint pt1, MPoint pt2, MPtArray* pReg).
//DESCRIPTION: Chekcs if the segment lays inside the random messenger area
// 				(set as the closed broken line)
//INPUTS : MPoint , MPoint , MPtArray pointer.
//RETURNS : True.
bool SGINREG(MPoint pt1, MPoint pt2, MPtArray* pReg);

//PACKAGE : MAP.
//FUNCTION:  SGINREG_EXT(MPoint pt1, MPoint pt2, MPtArray* pReg).
//DESCRIPTION: Chekcs if the segment lays inside the random messenger area
// 				(set as the closed broken line) :
// 				We consider a case when the ends of a segment may lay on a contour
// 				or the segment laying inside a contour may touch contour vertex.
//INPUTS : MPoint , MPoint , MPtArray pointer.
//RETURNS : True.
bool SGINREG_EXT(MPoint pt1, MPoint pt2, MPtArray* pReg);

//PACKAGE : MAP.
//FUNCTION:  LomlineInRegion(MPtArray* pLomline, MPtArray* pRegion).
//DESCRIPTION: Checks if the random straight line lays inside the random messenger area
// 				(set as the closed broken line).
//INPUTS : MPtArray pointer, MPtArray pointer.
//RETURNS : True.
bool LomlineInRegion(MPtArray* pLomline, MPtArray* pRegion);

//PACKAGE : MAP.
//FUNCTION:  LomlineInRegion_EXT(MPtArray* pLomline, MPtArray* pRegion).
//DESCRIPTION: Checks if the random straight line lays inside the random messenger area
// 				(set as the closed broken line).
//INPUTS : MPtArray pointer, MPtArray pointer.
//RETURNS : True.
bool LomlineInRegion_EXT(MPtArray* pLomline, MPtArray* pRegion);

//PACKAGE : MAP.
//FUNCTION:  PTONSEG(MPoint pt, MPoint pt1, MPoint pt2, double ldelta = 0.01).
//DESCRIPTION: Checks if the random point py lays on a segment with the ends of pt1 and pt2.
//INPUTS : MPoint , MPoint , MPoint , double .
//RETURNS : True.
bool PTONSEG(MPoint point, MPoint segPt1, MPoint segPt2, double ldelta = EPSILON);

//PACKAGE : MAP.
//FUNCTION:  PTONSEG_DOSTUP(MPoint pt, MPoint pt1, MPoint pt2, double dostup).
//DESCRIPTION: Checks if the random point py lays on a segment with the ends of pt1 and pt2.
//INPUTS : MPoint , MPoint , MPoint , double .
//RETURNS : True.
bool PTONSEG_DOSTUP(MPoint pt, MPoint pt1, MPoint pt2, double dostup);

//PACKAGE : MAP.
//FUNCTION:  PTONLOMLINE(MPoint pt, MPtArray* pLomline, int& index, int& vertexind).
//DESCRIPTION: Checks if the random point py lays on a straight line.
//INPUTS : MPoint , MPtArray pointer, int& , int& .
//RETURNS : True.
bool PTONLOMLINE(MPoint pt, MPtArray* pLomline, int& index, int& vertexind);

//PACKAGE : MAP.
//FUNCTION:  PTONKONTUR(MPoint pt, MPtArray* pKontur, int& index, int& vertexind, double delta=0.01).
//DESCRIPTION: Checks if the random point py lays on a contour.
// 				REMARK:The contour is set so, that zero and last points do not coincide.
//INPUTS : MPoint , MPtArray pointer , int& , int& , double .
//RETURNS : True.
bool PTONKONTUR(MPoint point, MPtArray* pKontur, int& index, int& vertexind, double delta=EPSILON);

//PACKAGE : MAP.
//FUNCTION:  FindDistFromBeginningToLompt(MPoint pt, MPtArray* pLomline, double& rasst).
//DESCRIPTION: Definition of distance from the beginning of straight line to the point, 
//				laying on this straight line.
//INPUTS : MPoint , MPtArray pointer, double& .
//RETURNS : True.
bool FindDistFromBeginningToLompt(MPoint pt, MPtArray* pLomline, double& rasst);

//PACKAGE : MAP.
//FUNCTION:  FindLomptOnRasstFromBeginning(MPtArray* pLomline, double rasst, MPoint& pt).
//DESCRIPTION: Definines contour point of distance 'rasst' from the beginning of contour
//INPUTS : MPtArray pointer, double , MPoint& .
//RETURNS : True.
bool FindLomptOnRasstFromBeginning(MPtArray* pLomline, double rasst, MPoint& pt);

//PACKAGE : MAP.
//FUNCTION: LOMTOASPL(MPtArray* ptar, unsigned char vid, double* vesarray, int kolpts).
//DESCRIPTION: Straight line transformation to approximated spline
// 				with established weighting coefficient
// 				and defined points quantity of edge partition of initial straight line.
//INPUTS : MPtArray pointer, unsigned char , double pointer, int .
//RETURNS : MPtArray pointer.
MPtArray* LOMTOASPL(MPtArray* ptar, unsigned char vid, double* vesarray, int kolpts);

//PACKAGE : MAP.
//FUNCTION:  LOMTOISPL(MPtArray* ptar, unsigned char vid, int kolpts).
//DESCRIPTION: Straight line transformation to approximated spline with
// 				defined points quantity of edge partition of initial straight line.
//INPUTS : MPtArray pointer, unsigned char , int .
//RETURNS : MPtArray pointer.
MPtArray* LOMTOISPL(MPtArray* ptar, unsigned char vid, int kolpts);

QVector<MPoint> LOMTOISPL(QVector<MPoint> vector, unsigned char vid, int kolpts);

//PACKAGE : MAP.
//FUNCTION:  GetPramuju(MPoint pt1,MPoint pt2,double d,double& A,double&B, double& C).
//DESCRIPTION: Gives out equation of Ax+By+C=0 view, which one is parallel to vector(pt1,pt2),
//				taking place on distance d from it and laying to the left, if d > 0,
//				or to the right, if d<0.
//INPUTS : MPoint ,MPoint ,double ,double& ,double&, double& .
//RETURNS : NONE.
void GetPramuju(MPoint pt1,MPoint pt2,double d,double& A,double&B, double& C);

//PACKAGE : MAP.
//FUNCTION:  GetPerpendikular(MPoint pt1,MPoint pt2,MPoint pt3,double d).
//DESCRIPTION: Gives out coordinates of a point, laying on a straight line,
// 				which one is perpendicular to a straight line defined by segment pt1, pt2,
// 				taking place through a point pt3.
// 				Total point should be lay on distance d from straight line [pt1, pt2]
// 				and lay to the left, if d > 0, or to the right, if d < 0.
//INPUTS : MPoint ,MPoint ,MPoint ,double .
//RETURNS : MPoint.
MPoint GetPerpendikular(MPoint pt1,MPoint pt2,MPoint pt3,double d);

//PACKAGE : MAP.
//FUNCTION:  GetPerpComponents(MPoint pt1,MPoint pt2,MPoint pt3).
//DESCRIPTION: Gives out coordinates of a point, laying on a straight line,
// 				which one is perpendicular to a straight line defined by vector pt1, pt2,
// 				taking place through a point pt2, and laying on the same side
// 				and on the same distance from a straight line, as a point pt3.
//INPUTS : MPoint ,MPoint ,MPoint .
//RETURNS : MPoint.
MPoint GetPerpComponents(MPoint pt1,MPoint pt2,MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  GetProdComponents(MPoint pt1,MPoint pt2,MPoint pt3).
//DESCRIPTION: Gives out coordinates of a point-projection pt3 to straight line,
// 				defined by vector pt1,pt2,p3.
//INPUTS : MPoint ,MPoint ,MPoint .
//RETURNS : MPoint.
MPoint GetProdComponents(MPoint pt1,MPoint pt2,MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  GetVektorGrad(MPoint* pt1,MPoint* pt2).
//DESCRIPTION: Returns angle between vector and OX axis in a degrees.
//INPUTS : MPoint pointer,MPoint pointer.
//RETURNS : Double.
double GetVektorGrad(MPoint* pt1,MPoint* pt2);

//PACKAGE : MAP.
//FUNCTION:  GetPointOnOtrezok(MPoint pt1,MPoint pt2,MPoint pt3).
//DESCRIPTION: segment (pt1,pt2) and point pt3 are defined.
// 				Returns point of perpendicular crossing of
// 				lowered from a point on a segment and a segment.
// 				If the point does not lay on a segment,
// 				than comes back the nearest end of a segment.
//INPUTS : MPoint ,MPoint ,MPoint .
//RETURNS : MPoint.
MPoint GetPointOnOtrezok(MPoint pt1,MPoint pt2,MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  IntersectLineWithOtrezok(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint* pt5,double& rassto).
//DESCRIPTION: Calculates point of crossing with a segment.
// 				segment defined by points pt1 Í pt2, staright  line - by points pt3 Í pt4.
// 				Calculates point of crossing of two straight lines. This calculated point
//               we can find in pt5. Additionally calculating distance from pt4 to point of crossing
// 				(Saves to rassto).
// 				Returnable values:
// 				0 - point of crossing lays on a picece (pt1,pt2):
// 				1 - point of crossing does not lay on a picece (pt1,pt2);
// 				2 - Straight lines are not crossing;
// 				3 - Straight lines are coinciding;
// 				4 - The segment is incorrect because the beginning and the end located in one point
// 					and straight line are not going through this point
// 				5 - The segment is incorrect because the beginning and the end located in one point
// 					but straight line are going through this point
// 				6 - Straight line is defined incorrect,both points pt3 and pt4 coincided and
// 					this point doesn't lay on a segment
// 				7 - Straight line is defined incorrect,both points pt3 and pt4 coinided but
// 					this point lays on a segment
// 				8 - Straight line is defined incorrect and segment is defined incorrect too
// 				9 - Straight line is defined incorrect and segment is defined incorrect too, but
// 					intersection may be found correctly
// 				10 - First point of points of straight line is belong to segment
// 				11 - Second point of points of straight line is belong to segment.
//INPUTS : MPoint ,MPoint ,MPoint ,MPoint ,MPoint pointer,double& .
//RETURNS : Int.
int IntersectLineWithOtrezok(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt5, double& rassto, double delta = EPSILON);

//PACKAGE : MAP.
//FUNCTION:  ExtendVector(MPoint pt1,MPoint pt2, double d).
//DESCRIPTION: Returns point coordinates, which is located on a defined distance
// 				and in a defined direction from this point
// 				Sets direction by two points: pt1 - the beginning of vector
// 				pt2 - end of vector.
//INPUTS : MPoint ,MPoint , double .
//RETURNS : MPoint.
MPoint ExtendVector(MPoint pt1,MPoint pt2, double d);

//PACKAGE : MAP.
//FUNCTION:  GetBisectrissa(MPoint pt1, MPoint pt2, MPoint pt3).
//DESCRIPTION: Defines coordinates of unit vector which lays on a bissectre of angle
// 				between two defined vectors with the common beginning in pt1.
//INPUTS : MPoint , MPoint , MPoint .
//RETURNS : MPoint.
MPoint GetBisectrissa(MPoint pt1, MPoint pt2, MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  GetPartOfString(int nPart, MString str).
//DESCRIPTION: Returns substring with number 'nIndex' from string str,
// 				which one has parted by blanks substrings.
//INPUTS : int , MString .
//RETURNS : MString.
MString GetPartOfString(int nPart, MString str);

//PACKAGE : MAP.
//FUNCTION:  MScanf(MString* in,MString* out, char separator, int &number).
//DESCRIPTION: Gets separators array for formulas lines creating
// 				Outputs array of elements indexes after which separator has been added
// 				Also returns 'MString' with distant separators
// 				Returns dimension of array
// 				// must be deleted
// 				if(pointer = MScanf(MString* in,MString* out, char separator, int &number))
// 				free(pointer).
//INPUTS : MString pointer,MString pointer, char , int &.
//RETURNS : Int pointer.
int* MScanf(MString* in,MString* out, char separator, int &number);

//PACKAGE : MAP.
//FUNCTION:  MScanfArrWithoutEmpty(MString* in,MStArray* outArr, char separator).
//DESCRIPTION: ets strings array for formulas lines creating
// 				Outputs strings array after which separator has been added
// 				Array is creating and deleting out of this function
// 				Also adds string 'WITHOUT BLANK STRINGS'.
//INPUTS : MString pointer,MStArray pointer, char .
//RETURNS : NONE.
void MScanfArrWithoutEmpty(MString* in,MStArray* outArr, char separator);

//PACKAGE : MAP.
//FUNCTION:  MScanfArr(MString* in,MStArray* outArr, char separator).
//DESCRIPTION: Gets strings array for formulas lines creating
// 				Outputs strings array after which separator has been added
// 				Array is creating and deleting out of this function
// 				Also adds string.
//INPUTS : MString pointer,MStArray pointer, char .
//RETURNS : NONE.
void MScanfArr(MString* in,MStArray* outArr, char separator);

//PACKAGE : MAP.
//FUNCTION:  CalcLinPreobrBy3pt(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint pt5,MPoint pt6).
//DESCRIPTION: Gets linear transformation if defined 3 points
// 				pt1,pt2,pt3 and equal to them images pt4,pt5,pt6,
// 				obtained after adding to this 3 points pt1,pt2,pt3
// 				required linear transformation. Function gets
// 				coefficients A1,B1,C1,A2,B2,C2 of surface transformation of following view:
// 				       A1*(x*)+B1*(y*)+C1=y
// 				       A2*(x*)+B2*(y*)+C2=x
// 				by defined 6 points,from wich 3 points- points befor transformation
// 				another 3 - points obtained after linear transformation
// 				Accordingly (x *, y*-old coordinates of a point, x, y-new)
// 				Recoverable values:
// 				0 - points pt1, pt2, pt3 are various and do not lie on one straight line
// 					(linear transformation is determined exaxtly and it is not depends
// 					on arrangement of points pt4, pt5, pt6 on a plane);
// 				1 - points pt1, pt2, pt3 are various and lie on one straight line or 2 of
// 					3 points pt1, pt2, pt3 coincide
// 					or 3 points pt1, pt2, pt3 coincide (linear transformation
// 					does not exist or it is determined not exactly
// 					it is not depends on arrangement of points pt4, pt5, pt6 on a plane).
//INPUTS : MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint pt5,MPoint pt6.
//RETURNS : Int.
int CalcLinPreobrBy3pt(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint pt5,MPoint pt6);

//PACKAGE : MAP.
//FUNCTION:  MakeLinPreobr(MPoint *pt).
//DESCRIPTION: Given function gets an image of a point at obtained above
// 				 linear transformation by function int CalcLinPreobrBy3pt ().
//INPUTS : MPoint pointer.
//RETURNS : NONE.
void MakeLinPreobr(MPoint *pt);

//PACKAGE : MAP.
//FUNCTION:  SetKompozParameters(double a,double b,double k,double l,double alfa).
//DESCRIPTION: This function gets an image of a point pt at linear trandformation of the plane
// 				of global view :
// 				       y = A1*(x*)+B1*(y*)+C1
// 				       x = A2*(x*)+B2*(y*)+C2
// 				( x *, y*-old coordinates of point; x, y-new) about which it is known following:
// 				 - centre of coordinate system at given linear transfarmation by parallel
// 					carry displaces to point with coordinates (a, b)
// 				 - coordinate axises Ox and Oy of obtained new coordinate system are expanded
// 					(contract) in k and l times accordingly
// 				 - coordinate system obtained in result of first two transformations is
// 					turning on angle alpha
// 				 Given linear transformation of plane with allowance for 3 conditions looks like above:
// 				 y = (-k*sin(alfa))*(x*) + (l*cos(alfa))*(y*) + (k*a*sin(alfa)-l*b*cos(alfa))
// 				 x = (k*cos(alfa))*(x*) + (l*sin(alfa))*(y*) + (-k*a*cos(alfa)-l*b*sin(alfa)).
//INPUTS : double ,double ,double ,double ,double .
//RETURNS : NONE.
void SetKompozParameters(double a,double b,double k,double l,double alfa);

//PACKAGE : MAP.
//FUNCTION:  KompozLinPreobr(MPoint *pt).
//DESCRIPTION: 
//INPUTS : MPoint pointer.
//RETURNS : NONE.
void KompozLinPreobr(MPoint *pt);

//PACKAGE : MAP.
//FUNCTION:  SetLinPreobrByCoefParameters(double a1,double b1,double c1,double a2,double b2,double c2).
//DESCRIPTION: The given function gets an image of a point pt at a linear transformation of plane of view:
// 				       y = A1*(x*)+B1*(y*)+C1
// 				       x = A2*(x*)+B2*(y*)+C2
// 				( x *, y*-aged coordinates of points of a plane, and x, y-new coordinates in a case if
// 				 coefficients Ai and Bi (i=1,2) are given.
//INPUTS : double ,double ,double ,double ,double ,double .
//RETURNS : NONE.
void SetLinPreobrByCoefParameters(double a1,double b1,double c1,double a2,double b2,double c2);

//PACKAGE : MAP.
//FUNCTION:  LinPreobrByCoef(MPoint *pt).
//DESCRIPTION: 
//INPUTS : MPoint pointer.
//RETURNS : NONE.
void LinPreobrByCoef(MPoint *pt);

//PACKAGE : MAP.
//FUNCTION:  GetAzimutAndLength(MPoint pt1,MPoint pt2,double &dAzimut,double &dLength).
//DESCRIPTION: Obtaining of azimuth and distance from the first point up to the second.
//INPUTS : MPoint ,MPoint ,double &,double &.
//RETURNS : NONE.
void		GetAzimutAndLength(MPoint pt1,MPoint pt2,double &dAzimut,double &dLength);

//PACKAGE : MAP.
//FUNCTION:  RectIntersection(MRect mrc1,MRect mrc2,MRect* pmrcResult = NULL).
//DESCRIPTION: Calculates rectangle of intersection.
//INPUTS : MRect ,MRect ,MRect pointer.
//RETURNS : True.
bool RectIntersection(MRect mrc1,MRect mrc2,MRect* pmrcResult = NULL);

//PACKAGE : MAP.
//FUNCTION:  ClipPolygon(MPtArray *input, MPtArray *output, MRect rect).
//DESCRIPTION: The cut points of the polygon lying beyond the borders of the rectangle.
//INPUTS : MPtArray  pointer, MPtArray  pointer, MRect .
//RETURNS : NONE.
void ClipPolygon(MPtArray *input, MPtArray *output, MRect rect);

//PACKAGE : MAP.
//FUNCTION:  ClipPolyline(MPtArray *input, QList<MPtArray*> &output, MRect rect).
//DESCRIPTION: Clipping of broken , lying beyond the borders of the rectangle.
//INPUTS : MPtArray  pointer, QList<MPtArray*> &, MRect .
//RETURNS : NONE.
void ClipPolyline(MPtArray *input, QList<MPtArray*> &output, MRect rect);

//PACKAGE : MAP.
//FUNCTION:  GetBitInArray(void* pArray,int iIndex).
//DESCRIPTION: Functions work with bits arrays
//				Gets bit in array.
//INPUTS : void pointer,int .
//RETURNS : True.
bool GetBitInArray(void* pArray,int iIndex);

//PACKAGE : MAP.
//FUNCTION:  SetBitInArray(void* pArray,int iIndex,bool bval).
//DESCRIPTION: Sets bit in a bits array.
//INPUTS : void pointer,int ,bool .
//RETURNS : NONE.
void SetBitInArray(void* pArray,int iIndex,bool bval);

//PACKAGE : MAP.
//FUNCTION:  CROSSSECTION(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt).
//DESCRIPTION: Intersection of two segments.
//INPUTS : MPoint , MPoint , MPoint , MPoint , MPoint pointer.
//RETURNS : True.
bool   CROSSSECTION(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, MPoint* pt);

//FUNCTION		:CalculateIntersection
//
//DESCRIPTION	://Intersection of a ray specified by a point with a stator from standPoint to vectPoint, with a contour arZoneCoord
//                 Return true if there are intersections. False if there is no
//                 In the case of several intersections, returns the result point maximally remote from the beam origin
bool CalculateIntersection(const MPoint& standPoint, const MPoint& vectPoint,  MPtArray& arZoneCoord, MPoint& result);

//FUNCTION		:CalculateIntersection
//
//DESCRIPTION	://Intersection of a ray specified by a point with a stator from standPoint to vectPoint, with a contour arZoneCoord
//                 Return true if there are intersections. False if there is no
//                 In the case of several intersections, returns the result point maximally remote from the beam origin
bool CalculateIntersection(const MPoint& standPoint, int angle,  MPtArray& arZoneCoord, MPoint& result);

//PACKAGE : MAP.
//FUNCTION:  FindCrossSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts, MInArray* indexes = NULL).
//DESCRIPTION: Intersection of segment with contour.
//INPUTS : MPoint , MPoint , MPtArray pointer, MPtArray pointer, MInArray pointer.
//RETURNS : True.
bool   FindCrossSegWithKontur(MPoint pt1, MPoint pt2, const MPtArray* kontur, MPtArray* crosspts, MInArray* indexes = NULL);

//PACKAGE : MAP.
//FUNCTION:  FindCrossOpenSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts).
//DESCRIPTION: Intersection of segment(which is not included the beginning point) with contour.
//INPUTS : MPoint , MPoint , MPtArray pointer, MPtArray pointer.
//RETURNS : True.
bool   FindCrossOpenSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts);

//PACKAGE : MAP.
//FUNCTION:  FindNotVertexCrossSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts).
//DESCRIPTION: Gets inner points of intersection of segment with contour.
//INPUTS : MPoint , MPoint , MPtArray pointer, MPtArray pointer.
//RETURNS : True.
bool   FindNotVertexCrossSegWithKontur(MPoint pt1, MPoint pt2, MPtArray* kontur, MPtArray* crosspts);

//PACKAGE : MAP.
//FUNCTION:  FindCrossptsLomlineWithLomline(MPtArray* lomline1, MPtArray* lomline2, MPtArray* crosspts).
//DESCRIPTION: Intersection of contour with contour.
//INPUTS : MPtArray pointer, MPtArray pointer, MPtArray pointer.
//RETURNS : True.
bool   FindCrossptsLomlineWithLomline(MPtArray* lomline1, MPtArray* lomline2, MPtArray* crosspts);

//PACKAGE : MAP.
//FUNCTION:  IsCrossptLomline(MPtArray* lomline).
//DESCRIPTION: Checks if contour has intersection?.
//INPUTS : MPtArray pointer.
//RETURNS : True.
bool   IsCrossptLomline(MPtArray* lomline);

//PACKAGE : MAP.
//FUNCTION:  Convex(MPtArray* points).
//DESCRIPTION: .
//INPUTS : MPtArray pointer.
//RETURNS : 1 - convex -1 non convex.
int Convex(MPtArray* points);

//PACKAGE : MAP.
//FUNCTION:  HORS_VERT_OTREZOK_CROSS(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint* pt).
//DESCRIPTION: Intersection of vertical and horizontal segments.
//INPUTS : MPoint ,MPoint ,MPoint ,MPoint ,MPoint pointer.
//RETURNS : True.
bool HORS_VERT_OTREZOK_CROSS(MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4,MPoint* pt);

//PACKAGE : MAP.
//FUNCTION:  GetPointBySdvig(MPtArray* pta, double s).
//DESCRIPTION: Defines point coordinates, that located on distance s from the beginning of pta array.
//INPUTS : MPtArray pointer, double .
//RETURNS : MPoint.
MPoint GetPointBySdvig(const MPtArray* pta, double s);

//PACKAGE : MAP.
//FUNCTION:  GetSdvigByPoint(MPtArray* pta, MPoint pt).
//DESCRIPTION: Defines offset of point from the beginning of break line, if it coordinates are known
// 				point exaxtly must lay on the break line.
//INPUTS : MPtArray pointer, MPoint .
//RETURNS : Double.
double GetSdvigByPoint(MPtArray* pta, MPoint pt);

//PACKAGE : MAP.
//FUNCTION:  GetDlinaLista(int visota, int dlina).
//DESCRIPTION: Align length of a sheet to the multiple values
// 				if the altitude of a sheet and the necessary length are defined.
//INPUTS : int , int .
//RETURNS : Int.
int GetDlinaLista(int visota, int dlina);

//PACKAGE : MAP.
//FUNCTION:  StrToInt(MString str).
//DESCRIPTION: Converts line into integer.
//INPUTS : MString.
//RETURNS : Int.
int StrToInt(MString str);

//PACKAGE : MAP.
//FUNCTION:  StrToDouble(MString str).
//DESCRIPTION: Converts number into line.
//INPUTS : MString.
//RETURNS : Double.
double StrToDouble(MString str);

//PACKAGE : MAP.
//FUNCTION:  ExtStrToDouble(MString str, double& result).
//DESCRIPTION: Converts line into double.
// 				 If transformation has passed successfully, than returns true,
// 				 and result of transformation lies in result
// 				 If the line does not contain number, than returns false.
//INPUTS : MString , double& .
//RETURNS : True.
bool ExtStrToDouble(MString str, double& result);

//PACKAGE : MAP.
//FUNCTION:  CalcBumToGauss(int mastab, double rasst).
//DESCRIPTION: Converts millimetres of paper into metres of map in Gauss projection.
//INPUTS : int , double .
//RETURNS : Double.
double CalcBumToGauss(int mastab, double rasst);

//PACKAGE : MAP.
//FUNCTION:  CalcGaussZoneNumberByCentralMeridian(int meridian).
//DESCRIPTION: Gets zone number by central meridian.
//INPUTS : int.
//RETURNS : int.
int CalcGaussZoneNumberByCentralMeridian(int meridian);

//PACKAGE : MAP.
//FUNCTION:  CalcCentralMeridianByGaussZoneNumber(int zonenumb).
//DESCRIPTION: Gets central meridian by zone number.
//INPUTS : int.
//RETURNS : int.
int CalcCentralMeridianByGaussZoneNumber(int zonenumb);

//PACKAGE : MAP.
//FUNCTION:  CalcGaussZoneNumberByGrif(MString str).
//DESCRIPTION: Gets zone number by layer numbering.
// 				Also checks if first two nominals have been defined correctly.
// 				Returns -1 if negative.
//INPUTS : MString.
//RETURNS : int.
int CalcGaussZoneNumberByGrif(MString str);

//PACKAGE : MAP.
//FUNCTION:  CalcMapGradboundsByGrif1(MString mas,MString str, MRect& gradbounds).
//DESCRIPTION: Calculates frame of layer in degrees
// 				by it's numbering
// 				Input: mas - scale, str - nomenclature
// 				Output: gradbounds - frame in degrees
// 				Returns code of success.
//INPUTS : MString ,MString , MRect& .
//RETURNS : True.
bool CalcMapGradboundsByGrif1(MString mas,MString str, MRect& gradbounds);

//PACKAGE : MAP.
//FUNCTION:  CalcMapGradboundsByGrif(unsigned mastab, MString str, MRect& gradbounds).
//DESCRIPTION: Calculates frame of layer in degrees
// 				by it's numbering
// 				Input: mas - scale, str - nomenclature
// 				Output: gradbounds - frame in degrees
// 				Returns code of success.
//INPUTS : unsigned , MString , MRect& .
//RETURNS : True.
bool CalcMapGradboundsByGrif(unsigned mastab, MString str, MRect& gradbounds);

//PACKAGE : MAP.
//FUNCTION:  Transliteration(MString* str).
//DESCRIPTION: Transliteration of English word by Russian letters.
//INPUTS : MString pointer.
//RETURNS : NONE.
void Transliteration(MString* str);

//PACKAGE : MAP.
//FUNCTION:  GetPointsBySdvigs(MPtArray* kontur, MDuArray* sdvigarr, MPtArray* respta).
//DESCRIPTION: Calculates coordinates of points in a massif respta, which lie
// 				 on broken line 'kontur' with shifts sdvigarr.
// 				 1) if any shift less than null, than resultative point
// 				 will be beginning of broken 'kontur'
// 				 2) if any shift greater than length of a contour, than resultative
// 				 point will be the end of a contour
// 				 in these cases shifts will be updated in a massif sdvigarr
// 				 (there will be brought real shifts of points)
// 				 all shifts in a massif sdvigarr should represent
// 				 nondecreasing order.
//INPUTS : MPtArray pointer, MDuArray pointer, MPtArray pointer.
//RETURNS : True.
bool GetPointsBySdvigs(MPtArray* kontur, MDuArray* sdvigarr, MPtArray* respta);

//PACKAGE : MAP.
//FUNCTION:  GetYkoordsByXkoords(MPtArray* profil, MDuArray* Xkoords, MDuArray* Ykoords).
//DESCRIPTION: Array profil is defined, where Ì - offset, y - height
// 				(Ì - sorted by increment
// 				and array Xkoord - Ì-coordinates(sorted by increment),
// 				in which we need to get altitudes
// 				Obtained altitudes save to Ykoord array.
//INPUTS : MPtArray pointer, MDuArray pointer, MDuArray pointer.
//RETURNS : True.
bool GetYkoordsByXkoords(MPtArray* profil, MDuArray* Xkoords, MDuArray* Ykoords);

//PACKAGE : MAP.
//FUNCTION:  FindDistFromPointToLomline(MPoint& gpt, MPtArray* pLomLine, MPoint& resultpt,int* nomrebra).
//DESCRIPTION: Returns distance from the given point up to the given broken line
// 				 In resultpt places calculated nearest point on broken line.
// 				 In nomrebra places an index of a crossbar, on which one the point was retrieved.
//INPUTS : MPoint& , MPtArray pointer, MPoint& ,int pointer.
//RETURNS : Double.
double FindDistFromPointToLomline(MPoint& gpt, MPtArray* pLomLine, MPoint& resultpt,int* nomrebra);

//PACKAGE : MAP.
//FUNCTION:  FindDistFromPointToKontur(MPoint& gpt, MPtArray* pKontur, MPoint& resultpt,int* nomrebra).
//DESCRIPTION: Returns distance from the given point up to the given contour
// 				In resultpt places calculated nearest point on broken line.
// 				In nomrebra places an index of edge, on which one the point was retrieved.
//INPUTS : MPoint& , MPtArray pointer, MPoint& ,int pointer.
//RETURNS : Double.
double FindDistFromPointToKontur(MPoint& gpt, MPtArray* pKontur, MPoint& resultpt,int* nomrebra);

//PACKAGE : MAP.
//FUNCTION:  DistanceToVershinaOfLomline(MPoint point, MPtArray* pLomline, MPoint& resultpt, int& nomvertex).
//DESCRIPTION: Returns nearest vertex of broken line to current point.
//INPUTS : MPoint , MPtArray pointer, MPoint& , int& .
//RETURNS : Double.
double DistanceToVershinaOfLomline(MPoint point, MPtArray* pLomline, MPoint& resultpt, int& nomvertex);

//PACKAGE : MAP.
//FUNCTION:  FindNomRebraOfPointOnLomline(MPtArray* pLomline, MPoint point).
//DESCRIPTION: Gets number of edge of broken line, on which lies point 'point'
// 				Input characteristics:
// 						pLomline - broken line
// 						point - point
// 				Output characteristic: number of edge, on which lies point, or -1,
// 						if pLomline = NULL, or quantity of points less than 2
// 				Conditions:
// 				1. Point must be lies on a contour
// 				(otherwise result will be incorrect).
//INPUTS : MPtArray pointer, MPoint .
//RETURNS : int.
int FindNomRebraOfPointOnLomline(MPtArray* pLomline, MPoint point);

//PACKAGE : MAP.
//FUNCTION:  FindNomVershinaOfPointOnLomline(MPtArray* pLomline, MPoint point).
//DESCRIPTION: Gets index of vertex of broken line, on which lies point
// 				Input characteristics:
// 					pLomline - broken line
// 					point    - point
// 				Result:
// 					NUmber of vertex, on which lies point,
// 					or -1, if point does not lie on vertex
// 				Conditions:
// 				1. Point must be lies on a contour.
//INPUTS : MPtArray pointer, MPoint .
//RETURNS : int.
int FindNomVershinaOfPointOnLomline(MPtArray* pLomline, MPoint point);

//PACKAGE : MAP.
//FUNCTION:  FindVisotaBySdvig(MPtArray* pProfil,double sdvig).
//DESCRIPTION: Defines altitude of point on a profile with displacement from the beginning of profile.
//INPUTS : MPtArray pointer,double .
//RETURNS : Double.
double FindVisotaBySdvig(MPtArray* pProfil,double sdvig);

//PACKAGE : MAP.
//FUNCTION:  RASSTPTSG(MPoint* point, MPoint* pt1, MPoint* pt2).
//DESCRIPTION: Distance from point up to segment.
//INPUTS : MPoint pointer, MPoint pointer, MPoint pointer.
//RETURNS : Double.
double RASSTPTSG(MPoint* point, MPoint* pt1, MPoint* pt2);

//PACKAGE : MAP.
//FUNCTION:  SetBitInByte(unsigned char& Byte, int nomer, bool value).
//DESCRIPTION: Sets in the byte 'Byte' bit with number of 'number' into value 'value'.
//INPUTS : char& , int , bool .
//RETURNS : NONE.
void SetBitInByte(unsigned char& Byte, int nomer, bool value);

//PACKAGE : MAP.
//FUNCTION:  GetBitInByte(unsigned char Byte, int nomer).
//DESCRIPTION: Ouputs value of bit with number of 'nomer' in byte 'Byte'.
//INPUTS : char , int .
//RETURNS : True.
bool GetBitInByte(unsigned char Byte, int nomer);

//PACKAGE : MAP.
//FUNCTION:  RGBtoHLS(int R,int G,int B, int& H,int& L,int& S).
//DESCRIPTION: Color transformation from RGB to HLS.
//INPUTS : int ,int ,int , int& ,int& ,int& .
//RETURNS : True.
bool  RGBtoHLS(int R,int G,int B, int& H,int& L,int& S);

//PACKAGE : MAP.
//FUNCTION:  HLStoRGB(int H,int L,int S, int& R,int& G,int& B).
//DESCRIPTION: Color transformation from HLS to RGB.
//INPUTS : int ,int ,int , int& ,int& ,int& .
//RETURNS : True.
bool HLStoRGB(int H,int L,int S, int& R,int& G,int& B);

//PACKAGE : MAP.
//FUNCTION:  SdvigRGBbyHLS(int R,int G,int B, int dH,int dL,int dS, int& nR,int& nG,int& nB).
//DESCRIPTION: Color transformation from HLS to RGB.
//INPUTS : int ,int ,int , int ,int ,int , int& ,int& ,int& .
//RETURNS : NONE.
void SdvigRGBbyHLS(int R,int G,int B, int dH,int dL,int dS, int& nR,int& nG,int& nB);

//PACKAGE : MAP.
//FUNCTION:  IS_VNUTRI_STROBA(MPtArray* pta1,MPoint* pt1, MPoint* pt2, double dopusk).
//DESCRIPTION: Checks if points of topobject (pta1) gets in strobe link [pt1, pt2].
//INPUTS : MPtArray pointer,MPoint pointer, MPoint pointer, double .
//RETURNS : True.
bool IS_VNUTRI_STROBA(MPtArray* pta1,MPoint* pt1, MPoint* pt2, double dopusk);

//PACKAGE : MAP.
//FUNCTION:  GetRasst(double poluos1,double poluos2,Coord shir1,Coord shir2,Coord dolg1,Coord dolg2).
//DESCRIPTION: Returns distance between two points
// 				on ellipsoid along geodesic line.
//INPUTS : double ,double ,Coord ,Coord ,Coord ,Coord .
//RETURNS : Double.
double GetRasst(double poluos1,double poluos2,Coord shir1,Coord shir2,Coord dolg1,Coord dolg2);

//PACKAGE : MAP.
//FUNCTION:  GetAzimut(double poluos1,double poluos2,Coord shir1,Coord shir2,Coord dolg1,Coord dolg2).
//DESCRIPTION: Returns azimuth of direction (Q1 Q2) in radians,
// 				if two point on ellipsoid are defined.
//INPUTS : double ,double ,Coord ,Coord ,Coord ,Coord .
//RETURNS : Double.
double GetAzimut(double poluos1,double poluos2,Coord shir1,Coord shir2,Coord dolg1,Coord dolg2);

double GetAzimuthByTwoPoints(const MPoint& ppt1, const MPoint& ppt2, double poluos1 = axis_X, double poluos2 = axis_Y);

int ConvertAzimuthToFold(const double& dAzimuth, const int& azimuthDiscrete);
int BringToMultiple(const int iNumber, const int iMultiple);

//PACKAGE : MAP.
//FUNCTION:  CreateDistContur(MPtArray* pta, double distofaxe, bool zamknut).
//DESCRIPTION: Creates line (contour), which goes on a defined distance
// 				from defined contour.
//INPUTS : MPtArray pointer, double , bool .
//RETURNS : MPtArray pointer.
MPtArray* CreateDistContur(MPtArray* pta, double distofaxe, bool zamknut);

//PACKAGE : MAP.
//FUNCTION:  InverseContur(MPtArray* pta).
//DESCRIPTION: Inverses order of points in points array.
//INPUTS : MPtArray pointer.
//RETURNS : True.
bool InverseContur(MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION:  CreateCircle(MPoint* pt, double radius, double grad).
//DESCRIPTION: Creates polygon, which approximates circle
// 				with defined centre and radius.
//INPUTS : MPoint pointer, double , double .
//RETURNS : MPtArray pointer.
MPtArray* CreateCircle(MPoint* pt, double radius, double grad);

// ---------------------------------------------------------------------------
// ADDITIONAL FUNCTIONS FOR IMPLEMENTATION of ALGORITHM of CONSTRUCTION TRIANGULATION
// TRIANGLES BY A QUANTITY OF POINTS AND DEFINED CONTOUR BY DELAUNEU

//PACKAGE : MAP.
//FUNCTION:  FindBoundRect(MPtArray* pReg, MRect* boundrect).
//DESCRIPTION: Calculates rectangle for input contour.
//INPUTS : MPtArray pointer, MRect pointer.
//RETURNS : True.
bool FindBoundRect(MPtArray* pReg, MRect* boundrect);

//PACKAGE : MAP.
//FUNCTION:  IsTriangleInRegion(MPoint pt1, MPoint pt2, MPoint pt3, MPtArray* pReg).
//DESCRIPTION: Checks , if triangle lies inside simply connected domain.
//INPUTS : MPoint , MPoint , MPoint , MPtArray pointer.
//RETURNS : True.
bool IsTriangleInRegion(MPoint pt1, MPoint pt2, MPoint pt3, MPtArray* pReg);

//PACKAGE : MAP.
//FUNCTION:  IsPointInCircle(MPoint pt, MPoint center, double radius).
//DESCRIPTION: Checks ,if defined point lies inside circle,
// 				which is defining by centre and radius.
//INPUTS : MPoint , MPoint , double .
//RETURNS : True.
bool IsPointInCircle(MPoint pt, MPoint center, double radius);

//PACKAGE : MAP.
//FUNCTION:  IsPointInTriangle(MPoint pt, MPoint pt1, MPoint pt2, MPoint pt3).
//DESCRIPTION: Checks,if defined point lies inside triangle,
// 				which is defining by 3 points.
//INPUTS : MPoint , MPoint , MPoint , MPoint .
//RETURNS : True.
bool IsPointInTriangle(MPoint pt, MPoint pt1, MPoint pt2, MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  PointInRegion(MPoint pt, MPtArray* pReg).
//DESCRIPTION: Cheks, if the point pt is inner point of random polygon 'pReg'.
//INPUTS : MPoint , MPtArray pointer.
//RETURNS : True.
bool PointInRegion(MPoint point, MPtArray* polygon, double ldelta = EPSILON, bool retTrueIfPtOnCountur = false);

//PACKAGE : MAP.
//FUNCTION:  FindConvexPolygonForPoints(MPtArray* ptar, MPtArray* convexplg).
//DESCRIPTION: For defined multitude of points creates prominent boundary polygon.
//INPUTS : MPtArray pointer, MPtArray pointer.
//RETURNS : True.
bool FindConvexPolygonForPoints(MPtArray* ptar, MPtArray* convexplg, double delta = 0.00001);

//PACKAGE : MAP.
//FUNCTION:  OrientTriangleByClock(MPoint& pt1, MPoint& pt2, MPoint& pt3).
//DESCRIPTION: Orientation of triangle points  clockwise;
// 				passed points are numerated as triangle vertexes.
//INPUTS : MPoint& , MPoint& , MPoint& .
//RETURNS : True.
bool OrientTriangleByClock(MPoint& pt1, MPoint& pt2, MPoint& pt3);

//PACKAGE : MAP.
//FUNCTION:  Area(MPtArray* ptar).
//DESCRIPTION: Calculation of (oriented) square of polygon.
//INPUTS : MPtArray pointer.
//RETURNS : Double.
double Area(MPtArray* ptar);

//PACKAGE : MAP.
//FUNCTION:  OrientInverse(MPtArray* ptar).
//DESCRIPTION: Changes orientation from broken to reverse.
//INPUTS : MPtArray pointer.
//RETURNS : True.
bool OrientInverse(MPtArray* ptar);

//PACKAGE : MAP.
//FUNCTION:  OrientLeft(MPtArray* ptar).
//DESCRIPTION: Sets left orientation of broken line.
//INPUTS : MPtArray pointer.
//RETURNS : True.
bool OrientLeft(MPtArray* ptar);

//PACKAGE : MAP.
//FUNCTION:  OrientRight(MPtArray* ptar).
//DESCRIPTION: Sets right orientation of broken line.
//INPUTS : MPtArray pointer.
//RETURNS : True.
bool OrientRight(MPtArray* ptar);

//PACKAGE : MAP.
//FUNCTION:  ConstructUnionStructKontur(MArArray* konturs, MPtArray* reskontur).
//DESCRIPTION: For the given set of contours builds the bounding contour which includes
// 				 all entrance contours so that the border of the received contour consist
// 				 from parts of borders of entrance contours also belonged to messenger area.
//INPUTS : MArArray pointer, MPtArray pointer.
//RETURNS : True.
bool ConstructUnionStructKontur(MArArray* konturs, MPtArray* reskontur);

//PACKAGE : MAP.
//FUNCTION:  RemoveAllCrossptsOfKontur(MPtArray* kontur).
//DESCRIPTION: Returns contour ,removed from it all selfcrossing pieces, if such were.
//INPUTS : MPtArray pointer.
//RETURNS : True.
bool RemoveAllCrossptsOfKontur(MPtArray* kontur);

//PACKAGE : MAP.
//FUNCTION:  TwoContursRelationships(MPtArray* kontur1, MPtArray* kontur2, bool &smegnost, MInArray* indexes1, MInArray* indexes2).
//DESCRIPTION: Function finds out relations of two contours among themselves.
//INPUTS : MPtArray pointer, MPtArray pointer, bool &, MInArray pointer, MInArray pointer.
//RETURNS : int.
int TwoContursRelationships(MPtArray* kontur1, MPtArray* kontur2, bool &smegnost,
							MInArray* indexes1, MInArray* indexes2);

//PACKAGE : MAP.
//FUNCTION:  GetPointByAzD(MPoint reper, double azimut, double dalnost).
//DESCRIPTION: The point reper, an azimuth azimut (degrees), distance (dalnost) are given
// 				 returns a point which is defined by a reference point, azimuth and distance
// 				 if distance < =0 than returns coordinates of reper.
//INPUTS : MPoint , double , double .
//RETURNS : MPoint.
MPoint GetPointByAzD(MPoint reper, double azimut, double dalnost);

//PACKAGE : MAP.
//FUNCTION:  GetAzimutByPoints(MPoint pt1,MPoint pt2).
//DESCRIPTION: Two points pt1 and pt2 are given.
// 				Returns azimuth of vector pt1->pt2 .
// 				If points coincide, than azimuth equal to  0.
//INPUTS : MPoint ,MPoint .
//RETURNS : Double.
double GetAzimutByPoints(MPoint pt1,MPoint pt2);

//PACKAGE : MAP.
//FUNCTION:  GetAzByUp(MPoint pt1, MPoint pt2, double& up).
//DESCRIPTION: Vector pt1->pt2 and angle of turn 'up' to third point is given
// 				returns azimuth
// 				If points coincide, than azimuth equal to  0.
//INPUTS : MPoint , MPoint , double& .
//RETURNS : Double.
double GetAzByUp(MPoint pt1, MPoint pt2, double& up);

//PACKAGE : MAP.
//FUNCTION:  GetUpByAz(MPoint pt1, MPoint pt2, double& az).
//DESCRIPTION: Vector pt1->pt2 and azimuth to third point az(degrees) is given
// 				returns angle of turn
// 				If points coincide, than azimuth equal to  0.
//INPUTS : MPoint , MPoint , double& .
//RETURNS : Double.
double GetUpByAz(MPoint pt1, MPoint pt2, double& az);

//PACKAGE : MAP.
//FUNCTION:  FindUgolBetweenVectors(MPoint pt1, MPoint pt2, MPoint pt3).
//DESCRIPTION: Three points: pt1, pt2, pt3 are given
// 				Find min angle between vectors (pt2; pt1) (pt2, pt3).
//INPUTS : MPoint , MPoint , MPoint .
//RETURNS : Double.
double FindUgolBetweenVectors(MPoint pt1, MPoint pt2, MPoint pt3);

//PACKAGE : MAP.
//FUNCTION:  BuildObvod(MPtArray* ptaIN,double dOffset).
//DESCRIPTION: Constructs array for leading round contour.
//INPUTS : MPtArray pointer,double .
//RETURNS : MPtArray pointer.
MPtArray* BuildObvod(MPtArray* ptaIN,double dOffset);

//PACKAGE : MAP.
//FUNCTION:  GetObhvatRect(MPtArray* pta).
//DESCRIPTION: Constructs bounding rectangle by input array.
//INPUTS : MPtArray pointer.
//RETURNS : MRect.
MRect GetObhvatRect(MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION:  BuildRudezh(MPtArray* pta,double dOffset, int splineCount=0).
//DESCRIPTION: Construction of the array circuit pta stroke at a distance dOffset.
//		The resulting circuit splineCount again performs smoothing sharp edges.
//INPUTS : MPtArray pointer,double , int .
//RETURNS : MPtArray.
MPtArray* BuildRudezh(MPtArray* pta,double dOffset, int splineCount=0); 

//PACKAGE : MAP.
//FUNCTION:  CalcZoomBound(MPtArray* pta, double delta).
//DESCRIPTION: Construction of the array circuit stroke pta.
//INPUTS : MPtArray pointer, double .
//RETURNS : True.
bool CalcZoomBound(MPtArray* pta, double delta);

//PACKAGE : MAP.
//FUNCTION:  CleanArr(MPtArray* pta,double dist).
//DESCRIPTION: It removes the array points the distance between which is less than dist.
//INPUTS : MPtArray pointer .
//RETURNS : NONE.
void CleanArr(MPtArray* pta,double dist);

//PACKAGE : MAP.
//FUNCTION:  SplineAngels(MPtArray* pta, double dist).
//DESCRIPTION: It smoothes out the sharp corners of the contour.
//				For every 3 points of the contour builds inscribed circle of radius dist
//				replaces triangle trapeze .
//				When you dist> 0 handles only prominent peaks at dist < 0 - only concave ..
//INPUTS : MPtArray pointer, double .
//RETURNS : True.
bool SplineAngels(MPtArray* pta, double dist);

//PACKAGE : MAP.
//FUNCTION:  RemoveIntersection(MPtArray* pta).
//DESCRIPTION: It removes the array pta cause self-intersection point circuit.
//INPUTS : MPtArray pointer.
//RETURNS : NONE.
void RemoveIntersection(MPtArray* pta);

//----------------------------------------------------
//FUNCTIONS FOR SPLINE INSCRIPTIONS CREATION - are taken from MNadpKriv class

//PACKAGE : MAP.
//FUNCTION:  FindPointsForKrivNadpis(MPtArray* ptar, MString str).
//DESCRIPTION: By entrance array of vertexes of a broken line(along which
// 				we have a line) and a line we form array of points
// 				arrangements of letters of line, which generally at discrepancy
// 				numbers of letters and numbers of vertexes of a broken line, does not
// 				coincide with array of vertexes of broken line.
//INPUTS : MPtArray pointer, MString .
//RETURNS : True.
bool FindPointsForKrivNadpis(MPtArray* ptar, MString str);

//PACKAGE : MAP.
//FUNCTION:  FindAnglesForKrivNadpis(MPtArray* ptar, MInArray* angles).
//DESCRIPTION: Calculates array of rotation letters angles.
//INPUTS : MPtArray pointer, MInArray pointer.
//RETURNS : True.
bool FindAnglesForKrivNadpis(MPtArray* ptar, MInArray* angles);

//PACKAGE : MAP.
//FUNCTION:  DrawShtrihRect(MPlotDevice* pDevice,MRect* rect,double angle,double step).
//DESCRIPTION: Hatched rectanle drawing under defined angle.
//INPUTS : MPlotDevice* pDevice,MRect* rect,double angle,double step.
//RETURNS : NONE.
void DrawShtrihRect(MPlotDevice* pDevice,MRect* rect,double angle,double step);

//PACKAGE : MAP.
//FUNCTION:  FormLomlineBySimpleSpline(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, int num, MPtArray* ptar).
//DESCRIPTION: Creates broken line(which equal to spline, which defined
// 				by 4 input points) of tipical points.
//INPUTS : MPoint , MPoint , MPoint , MPoint , int , MPtArray pointer.
//RETURNS : True.
bool FormLomlineBySimpleSpline(MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, int num,
								MPtArray* ptar);

// Creates coordinate system, layer, sign, object, array
// by given type

//PACKAGE : MAP.
//FUNCTION:  CreateCooByType(MString cootype).
//DESCRIPTION: Static function of coordinate system creation by it's type.
//INPUTS : MString.
//RETURNS : MCoo pointer.
MCoo* CreateCooByType(MString cootype);

//PACKAGE : MAP.
//FUNCTION:  CreateLayerByType(MString IDLayer).
//DESCRIPTION: Creates layer of special type.
//INPUTS : MString.
//RETURNS : MLayer pointer.
MLayer* CreateLayerByType(MString IDLayer);

//PACKAGE : MAP.
//FUNCTION:  CreateObjectByType(MString type).
//DESCRIPTION: Creates an object by it's identifier.
//INPUTS : MString.
//RETURNS : MObject pointer.
MObject*  CreateObjectByType(MString type);

//PACKAGE : MAP.
//FUNCTION:  CreateZnakByType(MString SType).
//DESCRIPTION: Creates a sign by it's identifier.
//INPUTS : MString.
//RETURNS : MZnak pointer.
MZnak*  CreateZnakByType(MString SType);

//PACKAGE : MAP.
//FUNCTION: CreateArrayByType(MString strType).
//DESCRIPTION: Creates array by specified type.
//INPUTS : MString.
//RETURNS : MArray pointer.
MArray* CreateArrayByType(MString strType);

//--------------------------------------
//
QVector<MPoint>  SimpleLine( QVector<MPoint> ptList, double epsilon);

//PACKAGE : MAP.
//FUNCTION:  GetEllipsoidParameters(MString ellipsoidname, double& greataxis, double& pressfactor).
//DESCRIPTION: Defines parameters of one of standart ellipsoids.
//INPUTS : MString , double& , double& .
//RETURNS : True.
bool GetEllipsoidParameters(MString ellipsoidname, double& greataxis, double& pressfactor);

//PACKAGE : MAP.
//FUNCTION:  GetGaussNomenklatura(MPoint geopt, unsigned mashtab, MString& nomenklatura, MString& cifrnomenklatura).
//DESCRIPTION: Searchs for nomenclatures of map sheet by known point
// 				 in geocoordinates and scale map sheet.
//INPUTS : MPoint , unsigned , MString& , MString& .
//RETURNS : NONE.
void GetGaussNomenklatura(MPoint geopt, unsigned mashtab, MString& nomenklatura,
						  MString& cifrnomenklatura);
//PACKAGE : MAP.
//FUNCTION:  GetSheetAtPoint(int scale, MPoint &pt, MRect &rect).
//DESCRIPTION: Returns the item number of the sheet scale maps 'scale' 
//				( 'scale' = 1000 1 : 1,000,000 , 'scale' = 500 to 1 : 500,000 ... ).
//INPUTS : int , MPoint &, MRect &.
//RETURNS : MString.
MString GetSheetAtPoint(int scale, const MPoint &pt, MRect &rect);

//PACKAGE : MAP.
//FUNCTION: GetSheetAtPoint1(MPoint &pt, MRect &rect).
//DESCRIPTION: Returns number of map sheet of scale 1:100000.
//INPUTS : MPoint &, MRect &.
//RETURNS : MString.
MString GetSheetAtPoint1(MPoint &pt, MRect &rect);

//PACKAGE : MAP.
//FUNCTION:  GetSheetAtPoint2(MPoint &pt, MRect &rect).
//DESCRIPTION: Returns number of map sheet of scale 1:200000.
//INPUTS : MPoint &, MRect &.
//RETURNS : MString.
MString GetSheetAtPoint2(MPoint &pt, MRect &rect);

//PACKAGE : MAP.
//FUNCTION:  GetSheetAtPoint5(MPoint &pt, MRect &rect).
//DESCRIPTION: Returns number of map sheet of scale 1:500000.
//INPUTS : MPoint &, MRect &.
//RETURNS : MString.
MString GetSheetAtPoint5(MPoint &pt, MRect &rect);

//PACKAGE : MAP.
//FUNCTION: SetRoundedNumber(double number,int col).
//DESCRIPTION: Approximates number 'number' to the given quantity of marks 'col' after a point.
//INPUTS : double ,int .
//RETURNS : Double.
double SetRoundedNumber(double number,int col);

//PACKAGE : MAP.
//FUNCTION:  reverseValue(char* val, int len).
//DESCRIPTION: Replaces bytes of variable 'val' of length 'len' in reverse order.
//INPUTS : char pointer, int .
//RETURNS : True.
bool reverseValue(char* val, int len);

//PACKAGE : MAP.
//FUNCTION:  isRectInRect(MRect&, MRect& ).
//DESCRIPTION: Defines, if geographical rectangles intersects.
//INPUTS : MRect&, MRect& .
//RETURNS :True.
bool IntersectGeoRectFast(MRect&, MRect& );

//PACKAGE : MAP.
//FUNCTION:  isRectInRect(MRect&, MRect& ).
//DESCRIPTION: Defines, if geographical rectangles intersects.
//INPUTS : MRect&, MRect& .
//RETURNS :True.
// FUNCTION		:isRectInRect
// 
///Defines if geographical rectangles includes each other
bool isRectInRect(MRect&, MRect& );

//PACKAGE : MAP.
//FUNCTION:  isPolylineIntersectsRect(MPtArray*, MRect& ).
//DESCRIPTION: Defines, if polyline intersects geographical rectangle.
//INPUTS : MPtArray pointer, MRect& .
//RETURNS :True.
bool isPolylineIntersectsRect(MPtArray*, MRect& );

//PACKAGE : MAP.
//FUNCTION:  Lagrange(MPtArray* pta, int number = 10).
//DESCRIPTION: Returns array of points representing a part of curveapproximated 
//                by Lagrange method. pta - array of points(nodes) which are used to 
//                calculate a curve. LAGRANGE_NUMBER = 10.
//INPUTS : MPtArray pointer, int  .
//RETURNS :MPtArray pointer.
MPtArray* Lagrange(MPtArray* pta, int number = 10);

//PACKAGE : MAP.
//FUNCTION:  UzelLagrange(double x, MPtArray* pta).
//DESCRIPTION: Calculates Lagrange coefficients (is used in GetLagrange()).
//INPUTS : double , MPtArray pointer.
//RETURNS : Double.
double UzelLagrange(double x, MPtArray* pta);

//PACKAGE : MAP.
//FUNCTION:  IntersectionGeoRect(MRect* rect1, MRect* rect2, MRect& rectReturn).
//DESCRIPTION: Returns in rectReturn the Rectangle of Intersection of rect1 & rect2 .
//INPUTS : MRect pointer, MRect pointer, MRect&  .
//RETURNS :True.
bool IntersectionGeoRect(MRect* rect1, MRect* rect2, MRect& rectReturn);

//PACKAGE : MAP.
//FUNCTION:  FindNomVershinaOfPointOnLomlineExt(MPtArray* pLomline, MPoint point, double eps = EPSILON).
//DESCRIPTION: Return number of pLomline vertex, which agree closely with point .
//INPUTS : MPtArray pointer, MPoint , double  .
//RETURNS : int.
int FindNomVershinaOfPointOnLomlineExt(MPtArray* pLomline, MPoint point, double eps = EPSILON);

//PACKAGE : MAP.
//FUNCTION:  CompareTwoMPoint(MPoint* pt1, MPoint* pt2, double eps = EPSILON).
//DESCRIPTION: Return true, if points are congruent with eps accuracy.
//INPUTS : MPoint pointer, MPoint pointer, double .
//RETURNS :True.
bool CompareTwoMPoint(const MPoint* pt1, const MPoint* pt2, double eps = EPSILON);

//PACKAGE : MAP.
//FUNCTION:  DeletingRepeatedPoints(MPtArray* pta).
//DESCRIPTION: Delete repeating points.
//INPUTS : MPtArray pointer .
//RETURNS : NONE.
void DeletingRepeatedPoints(MPtArray* pta);
void DeletingRepeatedPoints(MHlArray* pta);
void DeletingRepeatedPoints(std::vector<MPoint>& points);

void ConvertVectorMPointToMPtArray(const std::vector<MPoint>& vectorMPoint, MPtArray& ptArray);

//PACKAGE : MAP.
//FUNCTION:  Letter (int nomer, int koef).
//DESCRIPTION: Returns the sheet number for the scale of 1 : 50,000, 1 : 25,000.
//INPUTS : int , int  .
//RETURNS : int.
int Letter (int nomer, int koef);

//PACKAGE : MAP.
//FUNCTION:  FindIndexOfNearestPoint(MPtArray* pta, MPoint* point).
//DESCRIPTION: return the nearest to point number of pta vertex, nearest to point.
//INPUTS : MPtArray pointer, MPoint pointer .
//RETURNS : int.
int FindIndexOfNearestPoint(MPtArray* pta, MPoint* point);

//---------------------------------------------------------
// Functions for converting OEM -to- WIN and Win-to-OEM 
//(used to convert text files & data strings in SXF-files) 
 unsigned char dos2win(unsigned char);
 unsigned char win2dos(unsigned char);
 unsigned char koi2dos(unsigned char);
 unsigned char dos2koi(unsigned char);
 unsigned char koi2win(unsigned char);
 unsigned char win2koi(unsigned char);

 void dos2win( char* );
 void win2dos( char* );
 void koi2dos( char* );
 void dos2koi( char* );
 void koi2win( char* );
 void win2koi( char* );


 bool isNaN( double r );

//PACKAGE : MAP.
//FUNCTION:  round(double x, int decimals).
//DESCRIPTION: Round double value for specified decimals after comma.
//INPUTS : double , int .
//RETURNS : Double.
 double round(double x, int decimals);

 //******************************************
//  STRUCTURES AND FUNCTIONS FOR SXF-format FILE READING
//******************************************
#define IDSXF		0x00465853			// SXF
#define EDITION		0x0231				// 2.31
#define EDITION3	0x0300				// 3.0
#define IDSXFDATA	0x00544144			// DAT


// 	PASSPORT OF "SXF3_0"-FORMAT FILE
// 	EN: Passport of SXF-file
//=======================================
#ifdef _WINDOWS			
#pragma pack(push, 1)
#endif
#pragma pack(push, 1)
///Structure describes SXF format
typedef  struct {
										//  FILE IDENTIFIER
	int				SxfLabel;			//  0x00465853 (SXF)
	int				PassportLength;		//  LENGTH OF PASSPORT IN A BYTES
	unsigned short	Edition;			//  0x0231 // 0300
// 	unsigned long  int	 CheckSum;		// 
	unsigned char	CheckSum[4];		//  TOTAL SUMM OF FILE
	char			CreationDate[10];	//  CREATION DATE (DD/MM/YY)
	char			Nomenclature[24];	//  NOMENCLATURE OF SHEET (ASCIIZ)
	int				Scale;				//  SHEET SCALE (DENOMINATOR)
	char			SheetName[26];		//  CONVENTIONAL NAME OF SHEET (ASCIIZ)

										//  INFORMATIONAL FLAGS
	unsigned short	StateFlag		:2; //  Flag of data condition
	unsigned short	ProjectionFlag	:1; //  Flag of projections conformity
	unsigned short	CoordinateFlag	:2; //  Flag of real coordinates availability
	unsigned short	MethodFlag		:2; //  Flag of encoding method
	unsigned short	Placeholder1	:1; //  Reserve = 0
	unsigned short	FrameFlag		:8; //  Sign of summary by frame
	unsigned short	Placeholder2;		//  Reserve = 0

									//  NUMBER OF CLASSIFIER
	short	ObjectsClassificator;	//  Objects classifier
	short	SemanticClassificator;	//  Semantic classifier

	unsigned char	Mask[8];		//  Mask of elements of content availability

	//  Rectangular coordinates of rotation angles of sheet(in decimetres)

	char   XSouthWestCoordinate[4]; //  X of south-west angle (vertical)
	char   YSouthWestCoordinate[4]; //  Y of south-west angle (horizontal)
	char   XNorthWestCoordinate[4]; //  X of north-west angle
	char   YNorthWestCoordinate[4]; //  Y of north-west angle
	char   XNorthEastCoordinate[4]; //  X of north-east angle
	char   YNorthEastCoordinate[4]; //  Y of north-east angle
	char   XSouthEastCoordinate[4]; //  X of south-east angle
	char   YSouthEastCoordinate[4]; //  Y of south-east angle

	//  GEODESIC COORDINATES OF SHEET ANGLES
	//  (multiplied by 100 000 000 in radians)

	char   BSouthWestCoordinate[4]; //  B of south-west angle
	char   LSouthWestCoordinate[4]; //  L of south-west angle
	char   BNorthWestCoordinate[4]; //  B of north-west angle
	char   LNorthWestCoordinate[4]; //  L of north-west angle
	char   BNorthEastCoordinate[4]; //  B of north-east angle
	char   LNorthEastCoordinate[4]; //  L of north-east angle
	char   BSouthEastCoordinate[4]; //  B of south-east angle
	char   LSouthEastCoordinate[4]; //  L of south-east angle

	//  MATHEMATIC BASE OF SHEET

	unsigned char EllipsoideKind    ; //  Ellipsoid view
	unsigned char HeightSystem      ; //  System of altitudes
	unsigned char MaterialProjection; //  Projection of source material
	unsigned char CoordinateSystem  ; //  Coordinate system
	unsigned char PlaneUnit         ; //  Measuring unit of plane
	unsigned char HeightUnit        ; //  Measuring unit of altitude
	unsigned char FrameKind         ; //  Frame type
	unsigned char MapType           ; //  Global map view

	//  REFERENCE DATA OF SOURCE MATERIAL

	char           Date[10]           ; //  Area photo date(DD/MM/YY)
	unsigned char  MaterialKind       ; //  View of source material
	unsigned char  MaterialType       ; //  Type of source material
	char       MagneticAngle[4]       ; //  Magnition angle
	char       MeridianAngle[4]       ; //  Meridian angle
										//  (Ì 100 000 000 in radians)
	//  REFERENCE DATA OF SOURCE ALTITUDES (in decimetres)

	short     ReliefHeight        ; //  Altitude of relief section
	int       MaxReliefHeight     ; //  Max altitude of relief
	int       MinReliefHeight     ; //  Min altitude of relief
	int       MaxHorizonHeight    ; //  Max altitude of horizontal
	int       MinHorizonHeight    ; //  Min altitude of horizontal

	//  REFERENCE DATA OF FRAME(in 1/10 of millimetre)

	unsigned short  NorthSideFrameSize; //  Size of north side
	unsigned short  SouthSideFrameSize; //  Size of south side
	unsigned short  SideviewFrameSize;  //  Size of cheek
	unsigned short  DiagonalFrameSize;  //  Size of diagonal

	//  CHARACTERISTIC OF DEVICE (points/discrete to metre)

	int    DeviceCapability         ; //  Device resolution

	//  ARRANGEMENT OF THE FRAMEWORK ON THE DEVICE
	//  (inside device system in discretes)
	short     XSouthWestLocation     ; //  X of south-west angle  (vertical)
	short     YSouthWestLocation     ; //  Y of south-west angle (horisontal)
	short     XNorthWestLocation     ; //  X of north-west angle
	short     YNorthWestLocation     ; //  Y of north-west angle
	short     XNorthEastLocation     ; //  X of north-east angle
	short     YNorthEastLocation     ; //  Y of north-east angle
	short     XSouthEastLocation     ; //  X of south-east angle
	short     YSouthEastLocation     ; //  Y of south-east angle

	//  CLASSIFICATION CODE OF OBJECT FRAME

	int   ClassificationCodeFrame; //  From objects classifier

	//  REFERENCE DATA OF SOURCE MATERIAL PROJECTION
	//  (Multiplied by 100 000 000 in radians )

	int   FirstMainParallel      ; //  First main parallel
	int   SecondMainParallel     ; //  Second main parallel
	int   AxisMeridian           ; //  Axis meridian
	int   MainPointParallel      ; //  Parallel of main point
	int   Placeholder3           ; //  Reserve = 0

} SXFPASP;

///Structure describes "SXF4_0" format
typedef  struct {
							//  FILE IDENTIFIER
    int				SxfLabel         ;  //  0x00465853 (SXF) 4
    int				PassportLength   ;  //  //  LENGTH OF PASSPORT IN A BYTES  4
    unsigned  int	Edition ;           //  0x0231  4
    unsigned  int	CheckSum;           //  TOTAL SUMM OF FILE 4
    char			CreationDate[12] ;  //  CREATION DATE (DD/MM/YY) 12
    char			Nomenclature[32] ;  //  SHEET SCALE (DENOMINATOR) (ASCIIZ) 32
    int				Scale            ;  //  Sheet scale 4
    char			SheetName[32]    ;  //  CONVENTIONAL NAME OF SHEET (ASCIIZ) 32

										//  INFORMATIONAL FLAGS
	unsigned short	StateFlag		:2; //  Flag of data condition
    unsigned short	ProjectionFlag	:1; //  Flag of projections conformity // Флаг автоматического ведения GUID объектов
	unsigned short	CoordinateFlag	:2; //  Flag of real coordinates availability
	unsigned short	MethodFlag		:2; //  Flag of encoding method
    unsigned short	GeneralizationTable	:1; //  Reserve = 0 //Таблица генерализации
    char  SignatureEncodingFlag;// Флаг кодировки подписи  1
    char  CoordinatePrecisionFlag;//Флаг точности координат 1
    unsigned short SpetialSorting       :1;//Признак специальной  сортировки данных
   // unsigned short	FrameFlag		:8; //  Sign of summary by frame 1
    unsigned short	Placeholder2        :7;		//  Reserve = 0

								   //  CLASSIFIER NUMBER
    int   ObjectsClassificator   ; //  Objects classifier 4 Код EPSG для системы координат или 0

	//  Rectangular coordinates of rotation angles of sheet(in decimetres)

	double   XSouthWestCoordinate  ; //  X of south-west angle (vertical)
	double   YSouthWestCoordinate  ; //  Y of south-west angle (horizontal)
	double   XNorthWestCoordinate  ; //  X of north-west angle
	double   YNorthWestCoordinate  ; //  Y of north-west angle
	double   XNorthEastCoordinate  ; //  X of north-east angle
	double   YNorthEastCoordinate  ; //  Y of north-east angle
	double   XSouthEastCoordinate  ; //  X of south-east angle
	double   YSouthEastCoordinate  ; //  Y of south-east angle

	//  GEODESIC COORDINATES OF ANGLES OF SHEET
	//  (multiplied by 100 000 000 in radians )

	double   BSouthWestCoordinate   ; //  B of south-west angle
	double   LSouthWestCoordinate   ; //  L of south-west angle
	double   BNorthWestCoordinate   ; //  B of north-west angle
	double   LNorthWestCoordinate   ; //  L of north-west angle
	double   BNorthEastCoordinate   ; //  B of north-east angle
	double   LNorthEastCoordinate   ; //  L of north-east angle
	double   BSouthEastCoordinate   ; //  B of south-east angle
	double   LSouthEastCoordinate   ; //  L of south-east angle

	//  MATHEMATIC BASE OF SHEET

    unsigned char EllipsoideKind       ; //  Ellipsoid view 1
    unsigned char HeightSystem         ; //  Altitudes system   1
    unsigned char MaterialProjection   ; //  Source material projection 1
    unsigned char CoordinateSystem     ; //  Coordinate system 1
    unsigned char PlaneUnit            ; //  Plane measuring unit 1
    unsigned char HeightUnit           ; //  Measuring unit of altitude 1
    unsigned char FrameKind            ; //  Frame view 1
    unsigned char MapType              ; //  Global map type 1


	//  REFERENCE DATA OF SOURCE MATERIAL

    char           Date[12]            ; //  Area photo date(DD/MM/YY) 12
    unsigned char  MaterialKind        ; //  View of source material 1
    unsigned char  MaterialType        ; //  Type of source material 1
    unsigned short  Reserv_1         ; //  Reserved 2
    double       MagneticAngle       ; //  Magnition angle 8
    double       MeridianAngle       ; //  Average rapproachement of meridians 8
                                       //  (Ì 100 000 000 in radians)
	// Additional
    double			YearSklonenie		;//  Annual magnetic declination 8
	char			DateSklonenie[12]	;//  Date of declination mark
    int				Reserv_2			;//  Reserved(2 bytes) 4
    double			ReliefSech			;//  Relief section altitude 8
    double			Reserv_4			;//  Reserved (4 bytes) 8

	//  DEVICE CHARACTERISTICS (points/discretes on metre)
    int    DeviceCapability         ; //  Device resolution 4

	//  ARRANGEMENT OF THE FRAMEWORK ON THE DEVICE
	//  (inside device system in discretes)
	int   XSouthWestLocation     ; //  X of south-west angle  (vertical)
	int   YSouthWestLocation     ; //  Y of south-west angle (horizontal)
	int   XNorthWestLocation     ; //  X of north-west angle
	int   YNorthWestLocation     ; //  Y of north-west angle
	int   XNorthEastLocation     ; //  X of north-east angle
	int   YNorthEastLocation     ; //  Y of north-east angle
	int   XSouthEastLocation     ; //  X of south-east angle
	int   YSouthEastLocation     ; //  Y of south-east angle

	//  CLASSIFICATION CODE OF OBJECT FRAME

    int   ClassificationCodeFrame; //  From objects classifier 4

    //Справочные данные о     проекции исходного материала:
    long long FirstMajorParallel; //Первая главная параллель
    long long SecondMajorParallel;//Вторая главная параллель
    long long Meridian;// Осевой меридиан
    long long ParallelMajorPoint;//Параллель главной точки
    long long OffsetNorth;//Смещение на север
    long long OffsetEast; //Смещение на восток
   // char Reserv_Last[48];

} SXFPASP_4_0;


///Structure describes descriptor of  "SXF3_0" format data file
typedef struct
{
	int   DataLabel         ; //  Data identifier (0x00544144 (DAT))
	int   DescriptorLength  ; //  Descriptor length
	char  Nomenclature[24]  ; //  Nomenclature of sheet (ASCIIZ)
	int   DataRecordAmount  ; //  Data records quantity

										//  INFORMATIONAL FLAGS
	unsigned short   StateFlag      :2; //  Flag of data condition
	unsigned short   ProjectionFlag :1; //  Flag of projections conformity
	unsigned short   CoordinateFlag :2; //  Flag of real coordinates availability
	unsigned short   MethodFlag     :2; //  Flag of encoding method
	unsigned short   GeneralTable   :1; //  Generalization table
	unsigned short   FrameFlag      :8; //  Sign of summary by frame
	unsigned short   Placeholder2     ; //  Reserve = 0

									//  CLASSIFIER NUMBER
	short   ObjectsClassificator   ; //  Objects classifier
	short   SemanticClassificator  ; //  Semantic classifier

} SXFDESCRIPTOR;


/////Structure describes descriptor of  "SXF4_0" format data file
//typedef struct
//{
//	long  int   DataLabel         ; //  Data identifier (0x00544144 (DAT))
//	long  int   DescriptorLength  ; //  Descriptor length
//	char        Nomenclature[32]  ; //  Nomenclature of sheet (ASCIIZ)
//	long  int   DataRecordAmount  ; //  Data records quantity

//											//  INFORMATIONAL FLAGS
//	unsigned short int  StateFlag      :2; //  Flag of data condition
//	unsigned short int  ProjectionFlag :1; //  Flag of projections conformity
//	unsigned short int  CoordinateFlag :2; //  Flag of real coordinates availability
//	unsigned short int  MethodFlag     :2; //  Flag of encoding method
//	unsigned short int  GeneralTable   :1; //  Generalization table
//	unsigned short int  FrameFlag      :8; //  Sign of summary by frame
//	char  Placeholder2[3]     ; //  Reserve = 0

//										 //  CLASSIFIER NUMBER
//	short int   ObjectsClassificator   ; //  Objects classifier
//	short int   SemanticClassificator  ; //  Semantic classifier

//} SXFDESCRIPTOR_4_0;

 //  ДЕСКРИПТОР ДАННЫХ ФАЙЛА ФОРМАТА  "SXF4_0"
 //  ============================ 4.0
 typedef struct
 {
     long  int   DataLabel         ; // Идентификатор данных (0x00544144 (DAT)) 4 байта
     long  int   DescriptorLength  ; // Длина дескриптора 4 байта =52
     char        Nomenclature[32]  ; // Номенклатура листа (ASCIIZ) 32 байта
     long  int   DataRecordAmount  ; // Число записей данных 4 байта

                                             // ИНФОРМАЦИОННЫЕ ФЛАЖКИ (4 байта)
     unsigned short int  StateFlag      :2;  // Флаг состояния данных
     unsigned short int  ProjectionFlag :1;  // Флаг соответствия проекции
     unsigned short int  CoordinateFlag :2;  // Флаг наличия реальных координат
     unsigned short int  MethodFlag     :2;  // Флаг метода кодирования
     unsigned short int  GeneralTable   :1;  // Таблица генерализации
     char  SignatureEncoding;                //Флаг кодирования подписей  1 байт

     //=======

     char  SecrecyLabel;         //Гриф секретности  1 байт
     char  Placeholder;          //Резерв = 0 1 байт
     int  Placeholder2 ;     //Резерв  (4 байта)


     //=======
     //unsigned short int  FrameFlag      :8; // Признак сводки по рамке
     //char  Placeholder2[3]     ; // Резерв = 0

                                          // НОМЕР КЛАССИФИКАТОРА
     //short int   ObjectsClassificator   ; // Классификатор объектов
     //short int   SemanticClassificator  ; // Классификатор семантики

 } SXFDESCRIPTOR_4_0;


///Structure describes header record of  "SXF"-file format
typedef  struct {

	int   RecordLabel   ;  //  Identifier of record begin (0x7FFF7FFF)
	int   RecordLength  ;  //  Whole record lenght (with header)
	int   MetricLength  ;  //  Metric length (in bytes)

	unsigned  int  ClassificalCode       ; //  CLASSIFICATION CODE

											//  PERSONAL OBJECT NUMBER
	unsigned short  MemberNumber          ; //  Lower part
	unsigned short  UnionNumber           ; //  Upper part

												//  REFERENCE DATA
    // Справочные данные
	unsigned short   Local              : 4; //  Character of localisation
												//  (in SXF version lower than 3.0
												//  used lower two bytes)
    unsigned short   Limit              : 4; //  outgoing to bounds
    unsigned short   Abridge            : 1; //  Sign of lock-in
    unsigned short   SemanticOccurrence : 1; //  Semantic availability
    unsigned short   MetricElementSize  : 1; //  Metric element size
    unsigned short   GroupFlag          : 1; //  Sign of group object
    unsigned short   Reserve1           : 4; //  Reserve
    unsigned short   MetricRecordFormat : 1; //  Format of metric saving
    unsigned short   NotationDimension  : 1; //  Dimension of submission
    unsigned short   MetricElementType  : 1; //  Metric element type
    unsigned short   MetricWithTextFlag : 1; //  Sign of metric with text
                                                //  (only for SXF of version 3.0)
    unsigned short   Reserve2           : 4; //  Reserve
                                                //  GENERALIZATION LEVEL
    unsigned short    Bot               : 4; //  Lower limit of visibility
    unsigned short    Top               : 4; //  Upper limit of visibility
                                            //  DESCRIPTOR OF GROUP
    int            GroupNumber          ; //  Number of group(for SXF of version lower than 3.0)

                                                //  Metric descriptor
    short             SubObjectNumber      ; //  Subobjects quantity
    unsigned short    PointMetricNumber    ; //  Number of metric's points

} SXFHEAD;

 ///Structure describes header record of  "SXF"-file format
 typedef  struct {

     int   RecordLabel   ;  //  Identifier of record begin (0x7FFF7FFF)
     int   RecordLength  ;  //  Whole record lenght (with header)
     int   MetricLength  ;  //  Metric length (in bytes)

     unsigned  int  ClassificalCode       ; //  CLASSIFICATION CODE

                                             //  PERSONAL OBJECT NUMBER
     unsigned short  MemberNumber          ; //  Lower part
     unsigned short  UnionNumber           ; //  Upper part

                                                 //  REFERENCE DATA
     // Справочные данные
     unsigned short   Local              : 4; //  Character of localisation
                                                 //  (in SXF version lower than 3.0
                                                 //  used lower two bytes)

     unsigned short   MultyPoligon       : 1; //  Мультиполигон
     unsigned short   Reserv             : 3; //  Должен быть ноль

     unsigned short   CompressionSign     : 1; //Признак сжатия для графики
     unsigned short   PresenceOfSemantics : 1;// Наличие семантики
     unsigned short   MetricElementSize   : 1;// Размер элемента метрики
     unsigned short   ReferenceVector     : 1;//Наличие вектора привязки
     unsigned short   UNICODEFlag         : 1;//Признак текста в UNICODE
     unsigned short   AboveAllFlag        : 1;// Признак отображения объекта выше всех
     unsigned short   BelowAllFlag        : 1;// Признак отображения объекта ниже всех
     unsigned short   SubobjectsVerticalAlignment :1;// Признак выравнивания подобъектов по вертикали
     unsigned short   MetricRecordFormat : 1; // Формат записи метрики
     unsigned short   NotationDimension  : 1; //Размерность представления
     unsigned short   MetricElementType  : 1; //Тип элемента метрики
     unsigned short   MetricWithTextFlag : 1; //Признак метрики с текстом
     unsigned short   GraphicsAvailability  : 1;//Наличие графики (знака)
     unsigned short   GraphicsScalability   : 1; // Масштабируемость   графики
     unsigned short   ConstructingSplineByMetric :2;// Признак построения     сплайна по метрике

     // Уровень генерализации
     unsigned short    Bot               : 4; //  Lower limit of visibility
     unsigned short    Top               : 4; //  Upper limit of visibility

     unsigned int   PointMetricNumberBig; //Число точек метрики   для  больших объектов
                                       //Если в следующем поле значение 65535

     //Описатель метрики
     short             SubObjectNumber      ;// Число подобъектов
     unsigned short    PointMetricNumber    ; //Число точек метрики

 } SXFHEAD_4;

struct SERVICE_FIELD{

    SERVICE_FIELD(unsigned short num):reserv(0),numPoints(num){}
    unsigned short reserv ;      // 2 байта - резерв (желательно записывать ноль);
    unsigned short numPoints ;   // 2 байта  -   число точек метрики в подобъекте.

 } ;

struct TEXT_METRIC{

    TEXT_METRIC(QString & text):text(text)
    {
        lenght = text.length();
        lastSymbol = '\0';
    }
    ~TEXT_METRIC()
    {

    }
    unsigned short lenght       :8;      // 1 байт
   QString text;
    char lastSymbol   ;      // 1 байт

 } ;
struct SEMANTIC_head
{
    unsigned short kod :16; //Код характеристики (2 байта)

                        //Код длины блока (2 байта)
   unsigned short type :8;           //Тип характеристики  (1 байт)
   unsigned short scale :8;
};
struct SEMANTIC{
    SEMANTIC(unsigned short m_kod,char m_type, const char * m_value,unsigned short m_scale):
        kod(m_kod), type(m_type),scale(m_scale)//,value(nullptr)
    {

      // int length =(strlen(m_value))*sizeof(char);
       value = (char*)malloc(scale+1);
       strcpy(value,m_value);
    }

    SEMANTIC(SEMANTIC && other) noexcept :
        kod(other.kod), type(other.type),scale(other.scale),value(nullptr)
    {
        value = other.value;
        other.scale = 0;
        other.value = nullptr;


    }
    SEMANTIC& operator=(SEMANTIC&& other)noexcept
    {
        if (this != &other)
        {

                free(value);
            kod=other.kod;
            type=other.type;
            scale=other.scale;
            value = other.value;
            other.value = nullptr;
        }
        return *this;
    }
    ~SEMANTIC(){   
        free(value);
        value = nullptr;
    }


    int getLenght(){
        int lenght =0;
        if(type==4)
            lenght = sizeof(int)+4;
        else
            lenght =strlen(value)*sizeof(char)+4;
        return lenght;
    }

    unsigned short kod :16; //Код характеристики (2 байта)

                        //Код длины блока (2 байта)
   unsigned short type :8;           //Тип характеристики  (1 байт)
                        //   0 - символьное поле в формате ASCIIZ (DOS),
                        //   1 - цифровое поле длиной 1 байт, целочисленное,
                        //   2 - цифровое поле длиной 2 байта, целочисленное,
                        //   4 - цифровое поле длиной 4 байта, целочисленное,
                        //   8 - цифровое поле длиной 8 байт, с плавающей точкой в стандарте IEEE,
                        //   126 - символьное поле в формате ANSI (WINDOWS),
                        //   127 - символьное поле в формате UNICODE (UNIX).
   unsigned short scale :8;          //Масштабный коэффициент (1 байт)
                        //   Для символьного поля - число символов без учета замыкающего ноля (не более 255 символов в строке).
                        //   ­Для цифрового  целочисленного  поля - степень числа 10,
                        //   масштабный множитель для записи чисел с дробной частью или больших чисел.
                        //   Масштабный коэффициент может принимать значения от -127 до +127.
   //int value;      //Значение характеристики
   char*  value;      //Значение характеристики


};
#pragma pack(pop)
#ifdef _WINDOWS
#pragma pack(pop)
#endif


//=============================
//  Basic structures for 2-d geographic points (geopoints)
//=============================

// geopoint. x, y - type double
typedef struct {
	double x;
	double y;
} DBLPOINT;

// geopoint. x, y - type float
typedef struct {
	float x;
	float y;
} FLTPOINT;

// integer geopoint. x, y - type int
typedef struct {
	int x;// long int x; -ATTENTION-
	int y;// long int y;
} INTPOINT;

// integer geopoint. x, y - type short int
typedef struct {
	short int x;
	short int y;
} SHTPOINT;

//=========
// Base structures for 3d-points
//=========
// geopoint. x, y, h - type double
typedef struct {
	double x;
	double y;
	double h;
} H_DBLPOINT;

// geopoint. x, y, h - type float
typedef struct {
	float x;
	float y;
	float h;
} H_FLTPOINT;

// geopoint. x, y, h - type int
typedef struct {
	int x;//long int x; -ATTENTION-
	int y;//long int y;
	int h;//long int h;
} H_INTPOINT;

// geopoint. x, y, h - type short int
typedef struct {
	short int x;
	short int y;
	short int h;
} H_SHTPOINT;


// --------------------------------------
// Variables and Structures for conversion
// of matrix (MTR) and raster (RST) formats of the programm "Panorama"

#define   LABEL_MTR    0x0052544D
#define   VERSION_MTR  0x0401



///Structure describes pasport of altitude matrix (MTR)
typedef struct {

	unsigned long  int  Label          ; //  IDENTIFIER OFTHE BEGINNING OF FILE

	unsigned long  int  MBord_X1       ; //  MATRIX FRAME
	unsigned long  int  MBord_Y1       ; //     X, Y ( ÄÒ )
	unsigned long  int  MBord_X2       ;
	unsigned long  int  MBord_Y2       ;

	unsigned short int  Num_Blks_Vert  ; //  BLOCKS BY VERTICAL
	unsigned short int  Num_Blks_Horz  ; //  BLOCKS BY HORIZONTAL

	unsigned short int  Blksize_Vert   ; //  LINES IN COLUMN
	unsigned short int  Blksize_Horz   ; //  COLUMNS IN BLOCK

	unsigned short int  Blksize_Vert_tr; //  LINES IN TRUNCATED BLOCK
	unsigned short int  Blksize_Horz_tr; //  COLUMNS IN TRUNCATED BLOCK

	unsigned short int  Matr_Size_Vert ; //  LINES IN MATRIX
	unsigned short int  Matr_Size_Horz ; //  COLUMNS IN MATRIX

	unsigned short int  Elem_Size      ; //  ELEMENT SIZE : 1,2,4 bytes
	unsigned long  int  Elem_Size_mm   ; //  SIZE ON AREA IN MILLIMETRES

	unsigned short int  Num_Diskr_Elem ; //  DISCRETE IN ELEMENT (from SXF)

										 //  FLAGS FIELD
	unsigned short int  TypeHeight :  1; //  ALTITUDES TYPE
	unsigned short int  TruncBlocks:  1; //  TRUNCATED BLOCKS AVAILABILITY
	unsigned short int  MtrType    :  1; //  MATRIX STRUCTURE TYPE
										 //  0 - regular,1 - list
	unsigned short int  Free       : 13; //  RESERVE

	unsigned short int  Measure        ; //  MEASURING UNIT OF ALTITUDE

	signed   long  int  H_min          ; //  MIN MATRIX HEIGHT
	signed   long  int  H_max          ; //  MAX MATRIX HEIGHT

	unsigned long  int  PseudoCode     ; 

	unsigned long  int  Num_Diskr_m    ; //   DISCRETENESS (points per metre from SXF)
	signed   long  int  Scale          ; //   SCALE (DENOMINATOR)
	unsigned short int  Type           ; //   SOURCE MATERIAL TYPE
	unsigned char       Reserve [ 6]   ; //   RESERVE
	unsigned short int  Version        ; //   STRUCTURE VERSION
	unsigned short int  Reserve2	   ; //   RESERVE

} PSPM;

#define RSTIDN 0x00545352L  // "RST" 

#define RSTVEROLD 0x0201
#define RSTVER    0x0202

///Structure describes raster object
typedef struct RST  {

	unsigned long  int IDN;     //  IDENTIFIER IF THE BEGINNING OF FILE  0x00545352
	unsigned long  int LEN;     //  FILE LENGTH WITH TITLE
	unsigned short int AVR;     //  TOTAL QUANTITY OF LINES
	unsigned short int AHR;     //  TOTAL QUANTITY OF COLUMNS
	unsigned short int BVR;     //  QUANTITY OF BLOCKS IN VERTICAL PLANE
	unsigned short int BHR;     //  QUANTITY OF BLOCKS IN HORIZONTAL PLANE
	unsigned short int EVR;     //  QUANTITY OF LINES IN BLOCK
	unsigned short int EHR;     //  QUANTITY OF COLUMNS IN BLOCK
	unsigned short int DVR;     //  QUANTITY OF LINES IN BLOCK OF LAST ROW
	unsigned short int DHR;     //  QUANTITY COLUMNS IN BLOCK OF LAST COLUMN
	unsigned short int BSZ;     //  BLOCK SIZE (BYTES)
	unsigned short int ESZ;     //  ELEMENT SIZE (BITS)
	unsigned short int DSZ;     //  BLOCK SIZE OF LAST ROW
	unsigned short int EDZ;     //  ELEMENTS QUANTITY PER INCH

	unsigned short int FLG_BORD : 1;  //  FLAG OF FRAME AVAILABILITY
	unsigned short int FLG_STPR : 1;  //  FLAG OF PROJECTION CONDITION
	unsigned short int FLG_COLR : 1;  //  FLAG OF REPRESENTATION OF COLOR
	unsigned short int FLG_PRCN : 1;  //  FLAG OF VALIDATY OF FIELD EDZ
	unsigned short int FLG_RESR : 12; //  FLAGS OF PROCESS CONDITION

	signed long  int RMX;     //  ì OF SHEET FRAME
	signed long  int RMY;     //  Y OF SHEET FRAME
	unsigned short int VER;     //  STRUCTURE VERSION is 2.2
	unsigned short int SER;     //  SERVICE FIELD
	unsigned short int X1 ;     //  COORDINATE X (|) OF LOWER LEFT ANGLE
	unsigned short int Y1 ;     //  COORDINATE Y (->) OF LOWER LEFT ANGLE
	unsigned short int X2 ;     //  COORDINATE X OF UPPER LEFT ANGLE
	unsigned short int Y2 ;     //  COORDINATE Y OF UPPER LEFT ANGLE
	unsigned short int X3 ;     //  COORDINATE X OF UPPER RIGHT ANGLE
	unsigned short int Y3 ;     //  COORDINATE Y OF UPPER RIGHT ANGLE
	unsigned short int X4 ;     //  COORDINATE X OF LOWER RIGHT ANGLE
	unsigned short int Y4 ;     //  COORDINATE Y OF LOWER RIGHT ANGLE
	unsigned char  COLOR  ;     //  COLOR OF REPRESENTATION OF B/W RASTER
	unsigned char      REZ;     //  RESERVE

} RST ;

///Structure describes pasport of raster
typedef struct {

	RST  Rst;      //  RASTER PASSPORT
	char Pal[48];  //  PALETTE FOR 16 COLORS
} RSTPAL ;

#endif //NUXGLOBALS_H
