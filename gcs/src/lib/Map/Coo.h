//PACKAGE : MAP.
//FILE: Coo.h      
//AUTHOR:
//DESCRIPTION:

#if !defined(_COO_H__)
#define _COO_H__
#include <math.h>
#include <limits>
#include <algorithm>
#include "BaseCl.h"
#include "MString.h"
#include "SysDefine.h"

class MStArray;

// Constants of coordinate system identifier
#define COO_PARENT					"CNN"
#define COO_GEODESIC				"CBL"
#define COO_TOPOCENTRIC				"CTC"
#define COO_2DTOPOCENTRIC			"C2DTC"
#define COO_EXTENDEDGAUSSKRUGER		"CGE"
#define COO_STANDARTGAUSSKRUGER		"CGS"
#define COO_ZONEGAUSSKRUGER			"CGZ"
#define COO_PSEVDOGAUSSKRUGER		"CGP"
#define COO_TRANSVERSEMERCATOR		"CTM"
#define COO_UTM						"UTM"
#define COO_LAMBERTCONIC			"CLC"//LCC
#define COO_MERCATOR				"SPM"

#if ENGVERSION == 1 // English version
// Constants of different ellipsoids
#define ELLIPSOID_KRASOVSKI_42 "Krasovskogo (1942 year)"
#define ELLIPSOID_KRASOVSKI_42_RUS "Красовского 1942 г."
#define ELLIPSOID_PZ_90 "PZ-90"
#define ELLIPSOID_PZ_90_RUS "ПЗ-90"
#define ELLIPSOID_WGS_84 "WGS-84"
#define ELLIPSOID_GRS_80 "GRS-80"

// Constants of coordinate system names
#define COO_NAME_PARENT					""
#define COO_NAME_GEODESIC				"Geodesic Coordinate System"
#define COO_NAME_EXTENDEDGAUSSKRUGER	"Extended Coordinate System of Gauss-Kruger"
#define COO_NAME_STANDARTGAUSSKRUGER	"Standart Coordinate System of Gauss-Kruger"
#define COO_NAME_ZONEGAUSSKRUGER		"Coordinate Zone System of Gauss-Kruger"
#define COO_NAME_PSEVDOGAUSSKRUGER		"Local Coordinate System of Gauss-Kruger"
#define COO_NAME_TRANSVERSEMERCATOR     "Transverse Mercator"
#define COO_NAME_UTM					"Universal Transverse Mercator"
#define COO_NAME_MERCATOR				"Standard Parallel Mercator"
#define COO_NAME_LAMBERTCONIC			"Lambert Conformal Conic System"//LCC
// Generic names of all System Gauss-Kruger
#define COO_ALLGAUSSKRUGER				"Projection of Gauss-Kruger"
#define COO_NAME_TOPOCENTRIC			"Topocentric Coordinate System"
#define COO_NAME_2DTOPOCENTRIC			"2D Topocentric Coordinate System"

// Coordinate system settings
#define IDS_CNTR_MERIDIAN "Central Meridian"
#define IDS_RETRN_CNTR_MERIDIAN "East Location of Central Meridian"
#define IDS_NORTH_CNTR_MERIDIAN "North Location of Central Meridian"
#define IDS_X_REFERENCE_POINT "X Reference Point"
#define IDS_Y_REFERENCE_POINT "Y Reference Point"
#define IDS_CONFORMITY_FLAG "Conformity Indicator(Flag)"
#define IDS_X1GK "X1 to GK"
#define IDS_X2GK "X2 to GK"
#define IDS_Y1GK "Y1 to GK"
#define IDS_Y2GK "Y2 to GK"
#define IDS_X1MGK "X1 to MGK"
#define IDS_X2MGK "X2 to MGK"
#define IDS_Y1MGK "Y1 to MGK"
#define IDS_Y2MGK "Y2 to MGK"

// kate 03.11.2010 begin
#define IDS_FIRST_STD_PARALLEL			"First Standard Parallel"
#define IDS_SECOND_STD_PARALLEL			"Second Standard Parallel"
// kate 03.11.2010 end

// Constants for geodesic coordinates display
#define IDS_SOUTH_LAT "S.lat"
#define IDS_NORTH_LAT "N.lat"
#define IDS_WEST_LONG "W.long"
#define IDS_EAST_LONG "E.long"
//===============================================
#define IDS_TM_LAT_TRUE_SCL "latitude of the centeral meridian"
#define IDS_TM_SCALE "scale coefficient"

//===============================================
#else // Russian version

// Constants of different ellipsoids
#define ELLIPSOID_KRASOVSKI_42 "Krasovskogo (1942 year)"
#define ELLIPSOID_PZ_90 "PZ-90"
#define ELLIPSOID_WGS_84 "WGS-84"
#define ELLIPSOID_GRS_80 "GRS-80"

// Constants of coordinate system names
#define COO_NAME_PARENT					""
#define COO_NAME_GEODESIC				"Geodesic Coordinate System"
#define COO_NAME_EXTENDEDGAUSSKRUGER			"Extended Coordinate System of Gauss-Kruger"
#define COO_NAME_STANDARTGAUSSKRUGER			"Standart Coordinate System of Gauss-Kruger"
#define COO_NAME_ZONEGAUSSKRUGER			"Coordinate Zone System of Gauss-Kruger"
#define COO_NAME_PSEVDOGAUSSKRUGER			"Local Coordinate System of Gauss-Kruger"
//Generic names of all System Gauss-Kruger
#define COO_ALLGAUSSKRUGER				"Projection of Gauss-Kruger"
#define COO_NAME_TRANSVERSEMERCATOR     		"Transverse Mercator"
#define COO_NAME_UTM					"Universal Transverse Mercator"
#define COO_NAME_MERCATOR				"Standard Parallel Mercator"

#define COO_NAME_LAMBERTCONIC			"Lambert Conformal Conic System"//LCC"
//Generic names of all System Gauss-Kruger
#define COO_ALLGAUSSKRUGER				"Projection of Gauss-Kruger"
#define COO_NAME_TOPOCENTRIC			"Топоцентрическая система координат"
#define COO_NAME_2DTOPOCENTRIC			"2D Топоцентрическая система координат"

//Parameters of Coordinate System
#define IDS_CNTR_MERIDIAN "Central Meridian"
#define IDS_RETRN_CNTR_MERIDIAN "East Location of Central Meridian"
#define IDS_NORTH_CNTR_MERIDIAN "North Location of Central Meridian"
#define IDS_X_REFERENCE_POINT "X Reference Point"
#define IDS_Y_REFERENCE_POINT "Y Reference Point"
#define IDS_CONFORMITY_FLAG "Conformity Indicator(Flag)"
#define IDS_X1GK "X1 to GK"
#define IDS_X2GK "X2 to GK"
#define IDS_Y1GK "Y1 to GK"
#define IDS_Y2GK "Y2 to GK"
#define IDS_X1MGK "X1 to MGK"
#define IDS_X2MGK "X2 to MGK"
#define IDS_Y1MGK "Y1 to MGK"
#define IDS_Y2MGK "Y2 to MGK"
#define IDS_TM_LAT_TRUE_SCL "Latitude Center Meridian"
#define IDS_TM_SCALE "Scale Factor"
//kate 03.11.2010 begin
#define IDS_FIRST_STD_PARALLEL			"First Standard Parallel"
#define IDS_SECOND_STD_PARALLEL			"Second Standard Parallel"
//kate 03.11.2010 end

//for geodesic coordinates display
#define IDS_SOUTH_LAT "S.lat"
#define IDS_NORTH_LAT "N.lat"
#define IDS_WEST_LONG "W.long"
#define IDS_EAST_LONG "E.long"

#endif
//kate 29.10.2010 begin 
//массив типов систем координат
const std::string COO_TYPE_AR[]=
{
    COO_PARENT,
    COO_GEODESIC,
    COO_EXTENDEDGAUSSKRUGER,
    COO_STANDARTGAUSSKRUGER,
    COO_ZONEGAUSSKRUGER,
    COO_PSEVDOGAUSSKRUGER,
    COO_TRANSVERSEMERCATOR,
    COO_UTM,
    COO_LAMBERTCONIC,
    COO_MERCATOR
};
enum 
{
    COO_PARENT_INDEX,
    COO_GEODESIC_INDEX,
    COO_EXTENDEDGAUSSKRUGER_INDEX,
    COO_STANDARTGAUSSKRUGER_INDEX,
    COO_ZONEGAUSSKRUGER_INDEX,
    COO_PSEVDOGAUSSKRUGER_INDEX,
    COO_TRANSVERSEMERCATOR_INDEX,
    COO_UTM_INDEX,
    COO_LAMBERTCONIC_INDEX,
    COO_MERCATOR_INDEX
};

#define COO_TYPE_AR_SIZE 10
//kate 29.10.2010 end 

// Array of names of coordinate systems
const std::string COO_AR[]={
    COO_NAME_PARENT,
    COO_NAME_GEODESIC,
    COO_NAME_EXTENDEDGAUSSKRUGER,
    COO_NAME_STANDARTGAUSSKRUGER,
    COO_NAME_ZONEGAUSSKRUGER,
    COO_NAME_PSEVDOGAUSSKRUGER,
    COO_ALLGAUSSKRUGER,
    COO_NAME_TRANSVERSEMERCATOR,
    COO_NAME_UTM,
    COO_NAME_LAMBERTCONIC,
    COO_NAME_MERCATOR,
    COO_NAME_TOPOCENTRIC,
    COO_NAME_2DTOPOCENTRIC
};

// List of unique indexes of coordinate systems
enum {
    COO_AR_PARENT_INDEX = 0,
    COO_AR_GEODESIC_INDEX = 1,
    COO_AR_EXTENDEDGAUSSKRUGER_INDEX = 2,
    COO_AR_STANDARTGAUSSKRUGER_INDEX = 3,
    COO_AR_ZONEGAUSSKRUGER_INDEX = 4,
    COO_AR_PSEVDOGAUSSKRUGER_INDEX = 5,
    COO_AR_ALLGAUSSKRUGER_INDEX = 6,
    COO_AR_TRANSVERSEMERCATOR_INDEX = 7,
    COO_AR_UTM_INDEX = 8,
    COO_AR_LAMBERTCONIC_INDEX = 9,
    COO_AR_MERCATOR_INDEX = 10,
    COO_AR_TOPOCENTRIC_INDEX = 11,
    COO_AR_2DTOPOCENTRIC_INDEX = 12
};

// Coordinate system array size
#define COO_AR_SIZE 13
#define ID_FI		3
// kate 29.10.2010 begin
// array names ellipsoid
const std::string ELLIPSOID_AR[] = 
{
    ELLIPSOID_KRASOVSKI_42,
    ELLIPSOID_PZ_90,
    ELLIPSOID_WGS_84,
    ELLIPSOID_GRS_80,
};

//listing of unique indexes ellipsoid
enum 
{
    ELLIPSOID_AR_KRASOVSKI_42_INDEX = 0,
    ELLIPSOID_AR_PZ_90_INDEX = 1,
    ELLIPSOID_AR_WGS_84_INDEX = 2,
    ELLIPSOID_AR_GRS_80_INDEX = 3,
};

//array size ellipsoid
#define ELLIPSOID_AR_SIZE 4
//kate 29.10.2010 end


// Structure of the coordinate transformation
struct MEllipsTransform
{
    double dX,dY,dZ;	//linear elements transform, m;
    double Wx,Wy,Wz;	//angular elements of transformation, rad;
    double m;			//differential difference scale coordinate systems;
    short direct_conv;	//straight (true) or reverse transformation (false);
};

//
struct M3DPoint
{
    double x;
    double y;
    double z;

    M3DPoint(const double& x1 = 0.0, const double& y1 = 0.0, const double& z1 = 0.0)
        : x(x1),
          y(y1),
          z(z1)
    {

    }

    void SetCoordinates(const double& x1, const double& y1, const double& z1)
    {
        x = x1;
        y = y1;
        z = z1;
    };
};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Note to the classes of coordinate systems
// ------------------------------------
// In these classes, probably not worth the designers start with the parameters
// Because as the creation of a number of parameters can be heirs
// Grow to a very large number.
// For example, in the parent - the parameters of the ellipsoid (3 pcs.)
// And in Gauss-Kruger - plus 2 (central meridian, frame), that is a total of 5
// A Pseudo-Gauss-Kruger, plus another 6 (points for preobrazvaniya to GC)
// Ie The result - already 11.
// I think that all of these options is best to install
// Using functions such SetEllipsoid, SetGaussParameters, ...
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//PACKAGE : MAP.
//CLASS	: MCoo.
//DESCRIPTION : Class - the parent of all coordinate systems.
class MCoo

{
public:
    //PACKAGE : MAP.
    //FUNCTION : MCoo::MCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    explicit MCoo(const MString& ellipsoidName = ELLIPSOID_KRASOVSKI_42);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::~MCoo();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    virtual ~MCoo();

    MString GetEllipsoidName() const
    {
        return m_ellipsoidname;
    }

    double ecentricity(void);
    double ecentricitySquare(void) const;
    double curvatureRadius(MPoint pt) const;
    double curvatureRadius(double b) const;
    double curvatureRadius(double b, double e_2) const;
    double curvatureRadius(MPoint pt, double e_2) const;
    //PACKAGE : MAP.
    //FUNCTION : MCoo::SetStandartEllipsoid(MString ellipsename);
    //DESCRIPTION : Sets parameters.
    //INPUTS : MString.
    //RETURNS : True.

    bool SetStandartEllipsoid(MString ellipsename);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::SetCustomEllipsoid(MString ellipsename, double greataxis, double pressfactor);
    //DESCRIPTION : Sets parameters.
    //INPUTS : MString , double , double .
    //RETURNS : True.
    bool SetCustomEllipsoid(MString ellipsename, double greataxis, double pressfactor);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Fills up two lines arrays with parameters names and values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Sets parameters taken from arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::GetEllipsTransform(MString fromEllips,MString toEllips,MEllipsTransform &tParam);
    //DESCRIPTION : Function to obtain the parameters the coordinate transformation when changing ellipsoid.
    //INPUTS : MString ,MString ,MEllipsTransform &.
    //RETURNS : True.
    bool GetEllipsTransform(MString fromEllips,MString toEllips,MEllipsTransform &tParam);


    // parameters: (MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
    //PACKAGE : MAP.
    //FUNCTION : MCoo::ToBLH(MPoint , double , MPoint& , double& );
    //DESCRIPTION : Virtuals functions of transformation into geodesy and back.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : False.
    virtual bool ToBLH(MPoint , double , MPoint& , double& ){return false;}

    //PACKAGE : MAP.
    //FUNCTION : MCoo::FromBLH(MPoint , double , MPoint& , double& );
    //DESCRIPTION : Virtuals functions of transformation into geodesy and back.
    //INPUTS : MPoint , double , MPoint& , double&.
    //RETURNS : False.
    virtual bool FromBLH(MPoint , double , MPoint& , double& ){return false;}

    //PACKAGE : MAP.
    //FUNCTION : MCoo::FromXYZ(M3DPoint oldpoint, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtuals functions of transformation into geodesy and back.
    //INPUTS : M3DPoint , MPoint& , double& .
    //RETURNS : True.
    bool FromXYZ(M3DPoint oldpoint, MPoint& newpoint, double& newh) const;

    //PACKAGE : MAP.
    //FUNCTION : MCoo::ToXYZ(MPoint oldpoint, double oldh, M3DPoint& newpoint);
    //DESCRIPTION : Virtuals functions of transformation into geodesy and back.
    //INPUTS : MPoint , double , M3DPoint& .
    //RETURNS : True.
    bool ToXYZ(MPoint oldpoint, double oldh, M3DPoint& newpoint) const;

    //PACKAGE : MAP.
    //FUNCTION : MCoo::ConvertToCoo(MCoo* pNewCoo, MPoint& point, double& h);
    //DESCRIPTION : Transfer point of this coordinate system to another.
    //INPUTS : MCoo pointer, MPoint& , double& .
    //RETURNS : True.
    bool ConvertToCoo(MCoo* pNewCoo, MPoint& point, double& h);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);
    //DESCRIPTION : Calculates azimuth and distance by two points.
    //INPUTS : MPoint , MPoint , double& , double& .
    //RETURNS : NONE.
    virtual void FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::WriteBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Saves to UTP-format file.
    //INPUTS : FILE pointer,  int .
    //RETURNS : True.
    virtual bool WriteBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::ReadBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Reads to UTP-format file.
    //INPUTS : FILE pointer,  int .
    //RETURNS : True.
    virtual bool ReadBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::ReadBin(MFile* file,   int version);
    //DESCRIPTION : Reads to UTP-format file.
    //INPUTS : MFile pointer,  int .
    //RETURNS : True.
    virtual bool ReadBin(MFile* file,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::Copy(MCoo* pCoo);
    //DESCRIPTION : Copies.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool Copy(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    virtual int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Compares.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool IsEqualTo(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::CreateText(MPoint point);
    //DESCRIPTION : Makes text view of point coordinates.
    //INPUTS : MPoint.
    //RETURNS : MString.
    virtual MString CreateText(MPoint point);

    //PACKAGE : MAP.
    //FUNCTION : MCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_PARENT;}

    //PACKAGE : MAP.
    //FUNCTION : MCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_PARENT_INDEX];}

    void setEllipsoidname(const MString &newEllipsoidname);

    double greataxis() const;
    void setGreataxis(double newGreataxis);

    double pressfactor() const;
    void setPressfactor(double newPressfactor);

private:

    void ConvertDatum(MPoint point, double h, MPoint& newPoint, double &newH, MCoo* pNewCoo, MEllipsTransform* tParam);
    void ConvertDatum(M3DPoint point, M3DPoint& newPoint, MCoo* pNewCoo, MEllipsTransform* tParam);

    // Ellipsoid parameters
    MString m_ellipsoidname;	// < Ellipsoid name
    double m_greataxis;			// < Large half-angle
    double m_pressfactor;		// < Compression ratio
};


//PACKAGE : MAP.
//CLASS	: MGeodesicCoo : public MCoo.
//DESCRIPTION : Class - geodesic coordinate system.
class MGeodesicCoo : public MCoo

{
public:

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::MGeodesicCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MGeodesicCoo();

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::~MGeodesicCoo();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MGeodesicCoo(){}

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual function of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual function of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);
    //DESCRIPTION : Calculates azimuth and distance by two points.
    //INPUTS : MPoint , MPoint , double& , double& .
    //RETURNS : NONE.
    virtual void FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::CreateText(MPoint point);
    //DESCRIPTION : Creates text view of coordinates.
    //INPUTS : MPoint.
    //RETURNS : MString.
    virtual MString CreateText(MPoint point);

    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_GEODESIC;}
    //PACKAGE : MAP.
    //FUNCTION : MGeodesicCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_GEODESIC_INDEX];}
};

//=======================================
//CLASS			:MTopocentricCoo : public MCoo
//
//DESCRIPTION	:class - topocentric coordinate system
class MTopocentricCoo : public MCoo
        //=======================================
{

private:
    MPoint ptCnt;
    double hCnt;

    double xCnt, yCnt,
    sinLatCnt, cosLatCnt,
    e2, n;

public:
    //!default constructor
    explicit MTopocentricCoo(const MString& ellipsoidName = ELLIPSOID_KRASOVSKI_42);
    //!constructor
    MTopocentricCoo(MPoint point, double h, const MString& ellipsoidName = ELLIPSOID_KRASOVSKI_42);
    //!destructor
    ~MTopocentricCoo(){}

    //!virtual functions of converting geodesic coordinates to topocentric and back
    virtual bool FromBLH(MPoint oldpoint, double oldh, M3DPoint& newpoint) const;
    virtual bool ToBLH(M3DPoint oldpoint, MPoint& newpoint, double& newh) const;

    //!functions of converting geocentric coordinates to topocentric and back
    bool FromGeocentricToTopo(M3DPoint oldpoint, M3DPoint& newpoint) const;
    bool FromTopoToGeocentric(M3DPoint oldpoint, M3DPoint& newpoint) const;

    //self-identificating
    //! gives a type of coordinate system
    virtual MString WhatTheCoo() const {return COO_TOPOCENTRIC;}
    //! gives a string identificator of coordinate system
    virtual MString WhatTheNameCoo() const {return COO_AR[COO_AR_TOPOCENTRIC_INDEX];}
};

//=======================================
//CLASS			:M2DTopocentricCoo : public MCoo
//
//DESCRIPTION	:class - 2D topocentric coordinate system with azimuth for the axis x
class M2DTopocentricCoo : public MCoo
        //=======================================
{

private:
    MPoint ptCnt;
    double hCnt, aziX;

    double xCnt, yCnt,
    sinLatCnt, cosLatCnt,
    aziRad,
    e2, n;

public:
    //!default constructor
    M2DTopocentricCoo(const MString& ellipsoidName = ELLIPSOID_KRASOVSKI_42);
    //!constructor
    M2DTopocentricCoo(MPoint point, double h, double azimuth, const MString & ellipsoidName = ELLIPSOID_KRASOVSKI_42);
    //!destructor
    ~M2DTopocentricCoo();

    MPoint getOrigin() {return ptCnt;}
    double getHeight() {return hCnt;}
    double getAzimuthX() {return aziX;}
    void setOrigin(MPoint neworigin);
    void setHeight(double newh) {hCnt = newh;}
    void setAzimuthX(double newazi) {aziX = newazi; aziRad = aziX * pi / 180;}
    //!virtual functions of converting geodesic coordinates to 2D topocentric and back
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint) const;
    virtual bool ToBLH(MPoint oldpoint, MPoint& newpoint, double& newh) const;

    //!functions of converting geocentric coordinates to 2D topocentric and back
    bool FromGeocentricTo2DTopo(M3DPoint oldpoint, MPoint& newpoint) const;
    bool From2DTopoToGeocentric(MPoint oldpoint, M3DPoint& newpoint) const;

    //self-identificating
    //! gives a type of coordinate system
    virtual MString WhatTheCoo(){return COO_2DTOPOCENTRIC;}
    //! gives a string identificator of coordinate system
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_2DTOPOCENTRIC_INDEX - 1];}
};

//=======================================
//CLASS			:MExtendedGaussKrugerCoo  : public MCoo
//
//DESCRIPTION	:класс - система координат Гаусса-Крюгера
class MExtendedGaussKrugerCoo : public MCoo

{
public:
    double m_meridian;		// < Central zone meridian
    double m_falseeasting;	// < Coordinates beginning shift relatively
    // meridian-equator crossing to the west
    double m_falsenorthing;	// < Coordinates beginning shift relatively
    // meridian-equator crossing to the south
    MPoint m_reper;			// < Repers coordinates for introduction relative coordinates

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::MExtendedGaussKrugerCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MExtendedGaussKrugerCoo();

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::MExtendedGaussKrugerCoo();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MExtendedGaussKrugerCoo(){}

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::SetGaussParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0, MPoint reper = MPoint(0,0));
    //DESCRIPTION : Sets parameters.
    //INPUTS : double , double , double , MPoint .
    //RETURNS : True.
    virtual bool SetGaussParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0, MPoint reper = MPoint(0,0));

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::SetBasicParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0, MPoint reper = MPoint(0,0));
    //DESCRIPTION : Sets parameters.
    //INPUTS : double , double , double , MPoint .
    //RETURNS : True.
    virtual bool SetBasicParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::SetReper(MPoint reper);
    //DESCRIPTION : Sets parameters.
    //INPUTS : MPoint.
    //RETURNS : True.
    virtual bool SetReper(MPoint reper);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Fills up two string arrays with parameters names and their values.
    //INPUTS : MStArray MString, MStArray MString.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Sets parameters taken from arrays.
    //INPUTS : MStArray MString, MStArray MString.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);
    //DESCRIPTION : Calculates azimuth and distance by two points.
    //INPUTS : MPoint , MPoint , double& , double& .
    //RETURNS : True.
    virtual void FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::WriteBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Saves from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    virtual bool WriteBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::ReadBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    virtual bool ReadBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::ReadBin(MFile* file,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : MFile pointer,   int .
    //RETURNS : True.
    virtual bool ReadBin(MFile* file,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::Copy(MCoo* pCoo);
    //DESCRIPTION : Copies.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool Copy(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    virtual int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Comparison.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool IsEqualTo(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::CreateText(MPoint point);
    //DESCRIPTION : Creates text view of coordinates.
    //INPUTS : MPoint.
    //RETURNS : MString.
    virtual MString CreateText(MPoint point);

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system.
    //INPUTS : NONE.
    //RETURNS : COO_EXTENDEDGAUSSKRUGER.
    virtual MString WhatTheCoo(){return COO_EXTENDEDGAUSSKRUGER;}

    //PACKAGE : MAP.
    //FUNCTION : MExtendedGaussKrugerCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_EXTENDEDGAUSSKRUGER_INDEX];}
};


//PACKAGE : MAP.
//CLASS	: MZoneGaussKrugerCoo  : public MExtendedGaussKrugerCoo.
//DESCRIPTION : Class - band coordinate system Gauss-Kruger.
// 				The features of this system is that ,
// 				That it may have only the standard
// 				value of the central meridian (3, 9, 15 , 21 , 27, ...).
// 				X and y coordinates are determined by the zone number and hemispheric
// 				Can be used to capture without a transition between the hemispheres
// 				Output and mouse coordinates.
class MZoneGaussKrugerCoo  : public MExtendedGaussKrugerCoo

{
public:

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::MZoneGaussKrugerCoo();
    //DESCRIPTION : Constructor by default.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MZoneGaussKrugerCoo();

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::SetGaussParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0, MPoint reper = MPoint(0,0));
    //DESCRIPTION : Sets parameters.
    //INPUTS : double , double , double , MPoint .
    //RETURNS : True.
    virtual bool SetGaussParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0, MPoint reper = MPoint(0,0));

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::SetBasicParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0);
    //DESCRIPTION : Sets parameters.
    //INPUTS : double , double , double , MPoint .
    //RETURNS : NONE.
    virtual bool SetBasicParameters(double meridian, double falseeasting = 500000.0, double falsenorthing=0.0);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::SetReper(MPoint reper);
    //DESCRIPTION : Sets parameters.
    //INPUTS : MPoint.
    //RETURNS : True.
    virtual bool SetReper(MPoint reper);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::SetZone(int zonenumber);
    //DESCRIPTION : Sets parameters.
    //INPUTS : int .
    //RETURNS : True.
    virtual bool SetZone(int zonenumber);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::GetZone();
    //DESCRIPTION : Sets parameters.
    //INPUTS : NONE.
    //RETURNS : Int.
    virtual int  GetZone();

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Fills up two string arrays with parameters names and their values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Sets parameters, taken from arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_ZONEGAUSSKRUGER;};

    //PACKAGE : MAP.
    //FUNCTION : MZoneGaussKrugerCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_ZONEGAUSSKRUGER_INDEX];};
};

//PACKAGE : MAP.
//CLASS	: MStandartGaussKrugerCoo  : public MZoneGaussKrugerCoo.
//DESCRIPTION : Class - a standard coordinate system Gauss-Kruger
// 				The features of this system is that ,
// 				That it has fixed the central meridian.
// 				Each point on the earth's surface lies in a unique
// 				Particular zone and in a hemisphere.
// 				X and y coordinates are determined based on the coordinates of this point
// 				In the zone with the zone number and hemispheric
// 				Can also be used to display the mouse coordinates
class MStandartGaussKrugerCoo  : public MZoneGaussKrugerCoo

{
public:

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::MStandartGaussKrugerCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MStandartGaussKrugerCoo();

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Fills up two line arrays with parameters names and their values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Sets parameters, taken from arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Comparison.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool IsEqualTo(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_STANDARTGAUSSKRUGER;};

    //PACKAGE : MAP.
    //FUNCTION : MStandartGaussKrugerCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_STANDARTGAUSSKRUGER_INDEX];};

};


//PACKAGE : MAP.
//CLASS	: MPsevdoGaussKrugerCoo  : public MExtendedGaussKrugerCoo.
//DESCRIPTION : Class - incomplete coordinate system Gauss-Kruger
// 				The peculiarity of this system is that ,
// 				That this coordinate space coordinate system
// 				Formed with respect to the coordinate system GK
// 				The rotation by moving +
// 				Connection is defined by 4 points
// 				In general, such a link can not be determined.
class MPsevdoGaussKrugerCoo  : public MExtendedGaussKrugerCoo

{
public:

    void SetRotation(double gamma);
    double m_rotation;
    //Parameter of Coordinate System
    MPoint m_gauss_pt1;
    MPoint m_gauss_pt2;
    MPoint m_personal_pt1;
    MPoint m_personal_pt2;
    bool   m_havelink;

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::MPsevdoGaussKrugerCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MPsevdoGaussKrugerCoo();

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::SetLink(MPoint gauss_pt1, MPoint personal_pt1, MPoint gauss_pt2, MPoint personal_pt2, bool havelink);
    //DESCRIPTION : Sets parameters.
    //INPUTS : MPoint , MPoint , MPoint , MPoint , bool .
    //RETURNS : True.
    virtual bool SetLink(MPoint gauss_pt1, MPoint personal_pt1, MPoint gauss_pt2, MPoint personal_pt2, bool havelink);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::SetLink(bool havelink);
    //DESCRIPTION : Constructor.
    //INPUTS : bool.
    //RETURNS : True.
    virtual bool SetLink(bool havelink);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Fills up two line arrays with parameters names and their values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Establishes parameters, taken from arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::WriteBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Saves from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    virtual bool WriteBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::ReadBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    virtual bool ReadBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::ReadBin(MFile* file,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : MFile pointer,   int .
    //RETURNS : True.
    virtual bool ReadBin(MFile* file,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::Copy(MCoo* pCoo);
    //DESCRIPTION : Copies.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool Copy(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    virtual int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Comparison.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool IsEqualTo(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_PSEVDOGAUSSKRUGER;};

    //PACKAGE : MAP.
    //FUNCTION : MPsevdoGaussKrugerCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_PSEVDOGAUSSKRUGER_INDEX];};
};

//PACKAGE : MAP.
//CLASS	: MTransverseMercatorCoo  : public MCoo.
//DESCRIPTION : Class - Transverse Mercator.
class MTransverseMercatorCoo : public MCoo

{
public:
    double m_meridian;		// the central meridian of the zone
    double m_latitudeOfTrueScale;
    double m_scaleFactor;
    double m_falseeasting;	// shift the origin with respect to the intersection
    //of meridian - the equator to the west
    double m_falsenorthing;	// shift the origin with respect to the intersection
    //of meridian - the equator to the south


    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::MTransverseMercatorCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MTransverseMercatorCoo();

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::~MTransverseMercatorCoo();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MTransverseMercatorCoo(){};

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::SetBasicParameters(double meridian, double latitudeOfTrueScale=0.0,double scaleFactor=0.9996L,double falseeasting=0.0, double falsenorthing=0.0);
    //DESCRIPTION : Function setting.
    //INPUTS : double , double ,double ,double , double .
    //RETURNS : True.
    virtual bool SetBasicParameters(double meridian, double latitudeOfTrueScale=0.0,double scaleFactor=0.9996L,double falseeasting=0.0, double falsenorthing=0.0);

    //virtual bool SetMercatorParameters(double meridian, double falseeasting, double falsenorthing);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Function fills the two arrays of rows parameter names and their values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : function sets the parameters taken from the arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);
    //DESCRIPTION : Calculation of azimuth and range at two points.
    //INPUTS : MPoint , MPoint , double& , double& .
    //RETURNS : NONE.
    virtual void FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::WriteBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Saves from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    virtual bool WriteBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::ReadBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    virtual bool ReadBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::ReadBin(MFile* file,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : MFile pointer,   int .
    //RETURNS : True.
    virtual bool ReadBin(MFile* file,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::Copy(MCoo* pCoo);
    //DESCRIPTION : Copies.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool Copy(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    virtual int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Comparison.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool IsEqualTo(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::CreateText(MPoint point);
    //DESCRIPTION : Formation of the text type of coordinates.
    //INPUTS : MPoint.
    //RETURNS : MString.
    virtual MString CreateText(MPoint point);

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system..
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_TRANSVERSEMERCATOR;};

    //PACKAGE : MAP.
    //FUNCTION : MTransverseMercatorCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_TRANSVERSEMERCATOR_INDEX];};

private:
    double TranMerc_es;             // Eccentricity squared
    double TranMerc_ebs;
    double TranMerc_ap;
    double TranMerc_bp;
    double TranMerc_cp;
    double TranMerc_dp;
    double TranMerc_ep;

    // Maximum variance for easting and northing values
    double TranMerc_Delta_Easting;
    double TranMerc_Delta_Northing;


    double sphtmd( double latitude );
    double sphsn( double latitude );
    double sphsr( double latitude );

};


//PACKAGE : MAP.
//CLASS	: MUtmCoo  : public MTransverseMercatorCoo.
//DESCRIPTION : Class - Transverse Mercator.
class MUtmCoo : public MTransverseMercatorCoo
{

public:
    bool nPole; //true for Northern Hemisphere

    //PACKAGE : MAP.
    //FUNCTION : MUtmCoo::MUtmCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MUtmCoo(){m_falseeasting=500000;};

    //PACKAGE : MAP.
    //FUNCTION : MUtmCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MUtmCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);

    //PACKAGE : MAP.
    //FUNCTION : MUtmCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system..
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_UTM;};
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_UTM_INDEX];};

private:
    int	StandardZone(MPoint point);

};


//PACKAGE : MAP.
//CLASS	: MLambertConicCoo  : public MCoo.
//DESCRIPTION : Class - incompleted coordinate system of Lambert Conformal Conic
class MLambertConicCoo : public MCoo 
        //=======================================
{
public:
    double ex;
    double m_originLat;
    double m_meridian;
    double m_scndStP;
    double m_frstStP;
    double m_falseeasting;
    double m_falsenorthing;

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::MLambertConicCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MLambertConicCoo();

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::~MLambertConicCoo();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    virtual ~MLambertConicCoo(){};

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::SetBasicParameters(double meridian, double originLat, double frstStP, double scndStP,double falseeasting=0.0, double falsenorthing=0.0);
    //DESCRIPTION : Function setting.
    //INPUTS : double , double , double , double ,double , double .
    //RETURNS : True.
    bool SetBasicParameters(double meridian, double originLat, double frstStP, double scndStP,double falseeasting=0.0, double falsenorthing=0.0);

    //kate 03.11.2010 begin

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Function fills the two arrays of rows parameter names and their values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Function sets the parameters taken from the arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //kate 03.11.2010 end

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    bool ToBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newH);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    bool FromBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system..
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_LAMBERTCONIC;};

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : Mstring.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_LAMBERTCONIC_INDEX];};

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::WriteBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Saves from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    bool WriteBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::ReadBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    bool ReadBin(FILE* h_nuxfile,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::ReadBin(MFile* file,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : MFile pointer,   int .
    //RETURNS : True.
    bool ReadBin(MFile* file,   int version);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::Copy(MCoo* pCoo);
    //DESCRIPTION : Copies.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    bool Copy(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MLambertConicCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Comparison.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    bool IsEqualTo(MCoo* pCoo);

private:
    double CalcT(double fi);
    double CalcM(double fi);
    double CalcDeg(double rad);
    double CalcRad(double deg);
};

//kate 03.11.2010 begin

//PACKAGE : MAP.
//CLASS	: MMercatorCoo   : public MCoo.
//DESCRIPTION : Class - incompleted coordinate system of Lambert Conformal Conic.
const double MAX_LAT =  89.5; //grad
class MMercatorCoo : public MCoo 
        //=======================================
{
public:
    double m_centrmeridian;         /* Central meridian in radians    */
    double m_falsenorthing;         /* False northing in meters          */
    double m_falseeasting;          /* False easting in meters           */
    double m_scalefactor;           /* Scale factor                      */

private:
    /* Isometric to geodetic latitude parameters */
public:

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::MMercatorCoo();
    //DESCRIPTION : Constructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    MMercatorCoo();

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::~MMercatorCoo();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    virtual ~MMercatorCoo(){};

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::SetBasicParameters(double centrmeridian, double scalefactor, double falseeasting, double falsenorthing);
    //DESCRIPTION : Function setting.
    //INPUTS : double , double , double , double .
    //RETURNS : True.
    virtual bool SetBasicParameters(double centrmeridian, double scalefactor, double falseeasting, double falsenorthing);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::GetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Function fills the two arrays of rows parameter names and their values.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool GetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::SetCooParameters(MStArray* parnames, MStArray* pardata);
    //DESCRIPTION : Function sets the parameters taken from the arrays.
    //INPUTS : MStArray pointer, MStArray pointer.
    //RETURNS : True.
    virtual bool SetCooParameters(MStArray* parnames, MStArray* pardata);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::Copy(MCoo* pCoo);
    //DESCRIPTION : Copies.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    virtual bool Copy(MCoo* pCoo);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newH);
    //DESCRIPTION : Virtual functions of transformation into geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool ToBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newH);
    //DESCRIPTION : Virtual functions of transformation from geodesy.
    //INPUTS : MPoint , double , MPoint& , double& .
    //RETURNS : True.
    virtual bool FromBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::WhatTheCoo();
    //DESCRIPTION : Self-identification.
    //  			Returns type of coordinate system..
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheCoo(){return COO_MERCATOR;};

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::WhatTheNameCoo();
    //DESCRIPTION : Returns line identifier of coordinate system.
    //INPUTS : NONE.
    //RETURNS : MString.
    virtual MString WhatTheNameCoo(){return COO_AR[COO_AR_MERCATOR_INDEX];};

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::GetBytes();
    //DESCRIPTION : Gets size in a bytes.
    //INPUTS : NONE.
    //RETURNS : Int.
    int GetBytes();

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::WriteBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Saves from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    bool WriteBin(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::ReadBin(FILE* h_nuxfile,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : FILE pointer,   int .
    //RETURNS : True.
    bool ReadBin(FILE* h_nuxfile, int version);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::ReadBin(MFile* file,   int version);
    //DESCRIPTION : Reads from UTP-format file.
    //INPUTS : MFile pointer,   int .
    //RETURNS : True.
    bool ReadBin(MFile* file, int version);

    //PACKAGE : MAP.
    //FUNCTION : MMercatorCoo::IsEqualTo(MCoo* pCoo);
    //DESCRIPTION : Comparison.
    //INPUTS : MCoo pointer.
    //RETURNS : True.
    bool IsEqualTo(MCoo* pCoo);

private:
    bool calcDelta(double &deltaeasting, double &deltanorthing);
};
//kate 03.11.2010 end


//PACKAGE : MAP.
//CLASS	: MGRS .
//DESCRIPTION : Class - Military grid reference system
//class MGRS{
//private:

//    // The smallest length s.t., 1.0e7 - eps < 1.0e7 (approx 1.9 nm)
//    static const double eps;
//    // The smallest angle s.t., 90 - eps < 90 (approx 50e-12 arcsec)
//    static const double angeps;
//    static const std::string hemispheres;
//    static const std::string utmcols[3];
//    static const std::string utmrow;
//    static const std::string upscols[4];
//    static const std::string upsrows[2];
//    static const std::string latband;
//    static const std::string upsband;
//    static const std::string digits;

//    static const int mineasting[4];
//    static const int maxeasting[4];
//    static const int minnorthing[4];
//    static const int maxnorthing[4];
//    enum {
//        base = 10,
//        // Top-level tiles are 10^5 m = 100km on a side
//        tilelevel = 5,
//        // Period of UTM row letters
//        utmrowperiod = 20,
//        // Row letters are shifted by 5 for even zones
//        utmevenrowshift = 5,
//        // Maximum precision is um
//        maxprec = 5 + 6
//    };
//    static void CheckCoords(bool utmp, bool& northp, double& x, double& y);
//    static int lookup(const std::string& s, char c) throw() {
//        std::string::size_type r = s.find(toupper(c));
//        return r == std::string::npos ? -1 : int(r);
//    }

//    static int UTMRow(int iband, int icol, int irow) throw();

//    static int LatitudeBand(double lat) throw() {
//        int ilat = int(floor(lat));
//        return (std::max)(-10, (std::min)(9, (ilat + 80)/8 - 10));
//    }
//    // UTMUPS access these enums
//    enum {
//        tile = 100000,            // Size MGRS blocks
//        minutmcol = 1,
//        maxutmcol = 9,
//        minutmSrow = 10,
//        maxutmSrow = 100,         // Also used for UTM S false northing
//        minutmNrow = 0,           // Also used for UTM N false northing
//        maxutmNrow = 95,
//        minupsSind = 8,           // These 4 ind's apply to easting and northing
//        maxupsSind = 32,
//        minupsNind = 13,
//        maxupsNind = 27,
//        upseasting = 20,          // Also used for UPS false northing
//        utmeasting = 5,           // UTM false easting
//        // Difference between S hemisphere northing and N hemisphere northing
//        utmNshift = (maxutmSrow - minutmNrow) * tile
//    };
//    MGRS();                     // Disable constructor

//public:

//    //PACKAGE : MAP.
//    //FUNCTION : MGRS::convertFromGeo( MPoint& geoCoordinates, int prec=5);
//    //DESCRIPTION : Converts from coordinates to string.
//    //INPUTS : MPoint& , int .
//    //RETURNS : MString.
//    static	MString convertFromGeo( MPoint& geoCoordinates, int prec=5);
//    static QString GetBand(MPoint& geoCoordinates);
//    static QString GetTile(MPoint& geoCoordinates);// letter designation of the square 100 km

//    ///PACKAGE : MAP.
//    //FUNCTION : MGRS::convertToGeo( MString mgrs, MPoint& geoPt, bool centerp);
//    //DESCRIPTION : Converts from string to coordinates.
//    //INPUTS : MString , MPoint& , bool .
//    //RETURNS : NONE.
//    static	void convertToGeo( MString mgrs, MPoint& geoPt, bool centerp );

//};

//PACKAGE : MAP.
//CLASS	: GEOREF .
//DESCRIPTION : 
const long GEOREF_STR_LAT_MIN_ERROR = 0x0020;
const long GEOREF_STR_LON_MIN_ERROR = 0x0040;

class GEOREF 
{
public:

    //PACKAGE : MAP.
    //FUNCTION : GEOREF::convertFromGeo( MPoint geoCoordinates, long precision);
    //DESCRIPTION : Converts from coordinates to string.
    //INPUTS : MPoint , long .
    //RETURNS : MString.
    static	MString convertFromGeo( MPoint geoCoordinates, long precision);

    //PACKAGE : MAP.
    //FUNCTION : GEOREF::convertToGeo( MString GEOREFString );
    //DESCRIPTION : Converts from string to coordinates.
    //INPUTS : MString.
    //RETURNS : MPoint.
    static	MPoint convertToGeo( MString GEOREFString );

private:
    static void extractDegrees( char *GEOREFString, double *longitude, double *latitude );
    static void extractMinutes( char *GEOREFString, long start, long length, long errorType, double *minutes );
    static long roundGEOREF( double value );
    static void convertMinutesToString( double minutes, long precision, char *str );
};


#endif // !defined(_COO_H__)
