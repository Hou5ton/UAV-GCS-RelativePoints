#ifndef MBASICCOO_H
#define MBASICCOO_H

#include <math.h>
#include <limits>
#include <algorithm>
#include <string>


const std::string g_ELLIPSOID_KRASOVSKI_42 = "Krasovskogo (1942 year)";
const std::string g_ELLIPSOID_PZ_90 = "PZ-90";
const std::string g_ELLIPSOID_WGS_84 = "WGS-84";
const std::string g_ELLIPSOID_GRS_80 = "GRS-80";
const double g_pi = 3.141592653589793238462643;

class MCooBasic
{
public:

    double m_greataxis;			// < Large half-angle
    double m_pressfactor;		// < Compression ratio

    explicit MCooBasic(const std::string & ellipsoidname = g_ELLIPSOID_KRASOVSKI_42);
    virtual ~MCooBasic(){}


    virtual bool ToGeodetic(const double& xLocal, const double& yLocal, const double& altLocal,
                               double& xGeo, double& yGeo, double& altGeo)  {return false;}

    virtual bool FromGeodetic(const double& xGeo, const double& yGeo, const double& altGeo,
                                 double& xLocal, double& yLocal, double& altLocal)  {return false;}
};

class MTransverseMercatorProjection : public MCooBasic
{
public:

    MTransverseMercatorProjection();
    ~MTransverseMercatorProjection(){}
    virtual bool SetBasicParameters(double meridian, double latitudeOfTrueScale=0.0,double scaleFactor=0.9996L,double falseeasting=0.0, double falsenorthing=0.0);

    double m_meridian;		// the central meridian of the zone
    double m_latitudeOfTrueScale;
    double m_scaleFactor;
    double m_falseeasting;	// shift the origin with respect to the intersection
    //of meridian - the equator to the west
    double m_falsenorthing;	// shift the origin with respect to the intersection
    //of meridian - the equator to the south

    virtual bool ToGeodetic(const double& xLocal, const double& yLocal, const double& altLocal,
                               double& xGeo, double& yGeo, double& altGeo) ;

    virtual bool FromGeodetic(const double& xGeo, const double& yGeo, const double& altGeo,
                                 double& xLocal, double& yLocal, double& altLocal)  ;

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

class MUtmProjection : public MTransverseMercatorProjection
{
public:

    bool nPole; //true for Northern Hemisphere

    MUtmProjection();

    virtual bool ToGeodetic(const double& xLocal, const double& yLocal, const double& altLocal,
                               double& xGeo, double& yGeo, double& altGeo)  final;

    virtual bool FromGeodetic(const double& xGeo, const double& yGeo, const double& altGeo,
                                 double& xLocal, double& yLocal, double& altLocal)  final;
private:
    int	StandardZone(const double& xGeo, const double& yGeo);
};
#endif // MBASICCOO_H
