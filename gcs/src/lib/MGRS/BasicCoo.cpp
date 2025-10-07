#include "BasicCoo.h"
#include <iostream>
using namespace std;
MCooBasic::MCooBasic(const std::string &ellipsoidname)
{
    if(ellipsoidname == g_ELLIPSOID_KRASOVSKI_42)
       {
           m_greataxis = 6378245.0;
           m_pressfactor = 1.0/298.3;
       }
       else if(ellipsoidname == g_ELLIPSOID_PZ_90)
       {
           m_greataxis = 6378136.0;
           m_pressfactor = 1.0/298.257839303;
       }
       else if(ellipsoidname == g_ELLIPSOID_WGS_84)
       {
           m_greataxis = 6378137.0;
           m_pressfactor = 1.0/298.257223563;
       }
       else if(ellipsoidname == g_ELLIPSOID_GRS_80)
       {
           m_greataxis = 6378137.0;
           m_pressfactor = 1.0/298.257222101;
       }
}

MUtmProjection::MUtmProjection():MTransverseMercatorProjection()
{
m_falseeasting=500000;
}

bool MUtmProjection::ToGeodetic(const double &xLocal, const double &yLocal, const double &altLocal, double &xGeo, double &yGeo, double &altGeo)
{
    int zone = (int)floor(xLocal/1000000.0);
    double newLocalX = xLocal - 1000000.0*(int)floor(xLocal/1000000.0);
    m_meridian = (6 * zone - 183);
    if (!nPole) m_falsenorthing = 10000000;

    return MTransverseMercatorProjection::ToGeodetic(newLocalX,yLocal, altLocal, xGeo, yGeo, altGeo);

}

bool MUtmProjection::FromGeodetic(const double &xGeo, const double &yGeo, const double &altGeo, double &xLocal, double &yLocal, double &altLocal)
{
    if ((yGeo < -80) || (yGeo >84))
        return false;
    int zone = StandardZone(xGeo, yGeo);
    m_meridian = (6 * zone - 183);
    if(yGeo < 0)
    {
        nPole = false;
        m_falsenorthing = 10000000;
    }
    bool res = MTransverseMercatorProjection::FromGeodetic(xGeo, yGeo,altGeo, xLocal,yLocal, altLocal);
    //
    xLocal += zone*1000000;
    return res;
}

int MUtmProjection::StandardZone(const double &xGeo, const double &yGeo)
{
    double lat = yGeo, lon = xGeo;

    // Assume lon is in [-180, 360].
    if (lat >= -80 && lat < 84) {
        // Assume lon is in [-180, 360].
        int ilon = int(floor(lon));
        if (ilon >= 180)
            ilon -= 360;
        int zone = (ilon + 186)/6;
        int ilat = int(floor(lat));
        int band = (std::max)(-10, (std::min)(9, (ilat + 80)/8 - 10));
        if (band == 7 && zone == 31 && ilon >= 3)// The part of zone 31 covering western Norway is transferred to zone 32
            zone = 32;
        else if (band == 9 && ilon >= 0 && ilon < 42)
            zone = 2 * ((ilon + 183)/12) + 1;
        return zone;
    }
    return 0;
}

MTransverseMercatorProjection::MTransverseMercatorProjection():MCooBasic()
{
    SetBasicParameters(0.0);
}

bool MTransverseMercatorProjection::SetBasicParameters(double meridian, double latitudeOfTrueScale, double scaleFactor, double falseeasting, double falsenorthing)
{
    //it is impossible to establish a meridian which is less than-180 or more than 180 degrees,
    //therefore at first we will normalize value of a meridian
    while(meridian<-180.0)
        meridian+=360.0;
    while(meridian>180)
        meridian-=360.0;

    m_latitudeOfTrueScale = latitudeOfTrueScale;
    m_scaleFactor = scaleFactor;
    m_meridian = meridian;
    m_falseeasting = falseeasting;
    m_falsenorthing = falsenorthing;

    return true;
}

bool MTransverseMercatorProjection::ToGeodetic(const double &xLocal, const double &yLocal, const double &altLocal, double &xGeo, double &yGeo, double &altGeo)
{
    double tn;         /* True Meridianal distance constant  */
    double tn2;
    double tn3;
    double tn4;
    double tn5;

    double semiMajorAxis = this->m_greataxis;
    double flattening = this->m_pressfactor;
    double TranMerc_Origin_Lat = m_latitudeOfTrueScale*g_pi/180;
    if (m_meridian > 180)
        m_meridian -= (2*180);
    double TranMerc_Origin_Long = m_meridian*g_pi/180;
    double TranMerc_False_Northing = m_falsenorthing;
    double TranMerc_False_Easting = m_falseeasting;
    double TranMerc_Scale_Factor = m_scaleFactor;

    /* Eccentricity Squared */
    TranMerc_es = 2 * flattening - flattening * flattening;
    /* Second Eccentricity Squared */
    TranMerc_ebs = (1 / (1 - TranMerc_es)) - 1;

    double TranMerc_b = semiMajorAxis * (1 - flattening);
    /*True meridianal constants  */
    tn = (semiMajorAxis - TranMerc_b) / (semiMajorAxis + TranMerc_b);
    tn2 = tn * tn;
    tn3 = tn2 * tn;
    tn4 = tn3 * tn;
    tn5 = tn4 * tn;

    TranMerc_ap = semiMajorAxis * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
                                   + 81.e0 * (tn4 - tn5)/64.e0 );
    TranMerc_bp = 3.e0 * semiMajorAxis * (tn - tn2 + 7.e0 * (tn3 - tn4)
                                          /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
    TranMerc_cp = 15.e0 * semiMajorAxis * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
    TranMerc_dp = 35.e0 * semiMajorAxis * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
    TranMerc_ep = 315.e0 * semiMajorAxis * (tn4 - tn5) / 512.e0;

    double c;       /* Cosine of latitude                          */
    double de;      /* Delta easting - Difference in Easting (easting-Fe)    */
    double dlam;    /* Delta longitude - Difference in Longitude       */
    double eta;     /* constant - TranMerc_ebs *c *c                   */
    double eta2;
    double eta3;
    double eta4;
    double ftphi;   /* Footpoint latitude                              */
    int    i;       /* Loop iterator                   */
    //  double s;       /* Sine of latitude                        */
    double sn;      /* Radius of curvature in the prime vertical       */
    double sr;      /* Radius of curvature in the meridian             */
    double t;       /* Tangent of latitude                             */
    double tan2;
    double tan4;
    double t10;     /* Term in coordinate conversion formula - GP to Y */
    double t11;     /* Term in coordinate conversion formula - GP to Y */
    double t12;     /* Term in coordinate conversion formula - GP to Y */
    double t13;     /* Term in coordinate conversion formula - GP to Y */
    double t14;     /* Term in coordinate conversion formula - GP to Y */
    double t15;     /* Term in coordinate conversion formula - GP to Y */
    double t16;     /* Term in coordinate conversion formula - GP to Y */
    double t17;     /* Term in coordinate conversion formula - GP to Y */
    double tmd;     /* True Meridianal distance                        */
    double tmdo;    /* True Meridianal distance for latitude of origin */


    double easting = xLocal;
    double northing = yLocal;

    if ((easting < (TranMerc_False_Easting - TranMerc_Delta_Easting))
            ||(easting > (TranMerc_False_Easting + TranMerc_Delta_Easting)))
    { /* easting out of range  */
        cout <<"easting out of range"<<endl;
    }
    if ((northing < (TranMerc_False_Northing - TranMerc_Delta_Northing))
            || (northing > (TranMerc_False_Northing + TranMerc_Delta_Northing)))
    { /* northing out of range */
        cout <<"northing out of range"<<endl;
    }



    /* True Meridianal Distances for latitude of origin */
    tmdo = sphtmd(TranMerc_Origin_Lat);

    /*  Origin  */
    tmd = tmdo +  (northing - TranMerc_False_Northing) / TranMerc_Scale_Factor;

    /* First Estimate */
    sr = sphsr(0.e0);
    ftphi = tmd/sr;

    for (i = 0; i < 5 ; i++)
    {
        t10 = sphtmd (ftphi);
        sr = sphsr(ftphi);
        ftphi = ftphi + (tmd - t10) / sr;
    }

    /* Radius of Curvature in the meridian */
    sr = sphsr(ftphi);

    /* Radius of Curvature in the meridian */
    sn = sphsn(ftphi);

    /* Sine Cosine terms */
    //  s = sin(ftphi);
    c = cos(ftphi);

    /* Tangent Value  */
    t = tan(ftphi);
    tan2 = t * t;
    tan4 = tan2 * tan2;
    eta = TranMerc_ebs * pow(c, 2.0);
    eta2 = eta * eta;
    eta3 = eta2 * eta;
    eta4 = eta3 * eta;
    de = easting - TranMerc_False_Easting;
    if (fabs(de) < 0.0001)
        de = 0.0;

    /* Latitude */
    t10 = t / (2.e0 * sr * sn * pow(TranMerc_Scale_Factor, 2.0));
    t11 = t * (5.e0  + 3.e0 * tan2 + eta - 4.e0 * pow(eta, 2.0)
               - 9.e0 * tan2 * eta) / (24.e0 * sr * pow(sn, 3.0)
                                       * pow(TranMerc_Scale_Factor, 4.0));
    t12 = t * (61.e0 + 90.e0 * tan2 + 46.e0 * eta + 45.E0 * tan4
               - 252.e0 * tan2 * eta  - 3.e0 * eta2 + 100.e0
               * eta3 - 66.e0 * tan2 * eta2 - 90.e0 * tan4
               * eta + 88.e0 * eta4 + 225.e0 * tan4 * eta2
               + 84.e0 * tan2* eta3 - 192.e0 * tan2 * eta4)
            / ( 720.e0 * sr * pow(sn,5.0) * pow(TranMerc_Scale_Factor, 6.0) );
    t13 = t * ( 1385.e0 + 3633.e0 * tan2 + 4095.e0 * tan4 + 1575.e0
                * pow(t, 6.0))/ (40320.e0 * sr * pow(sn, 7.0) * pow(TranMerc_Scale_Factor, 8.0));
    double latitude = ftphi - pow(de, 2.0) * t10 + pow(de, 4.0) * t11 - pow(de, 6.0) * t12
            + pow(de, 8.0) * t13;

    t14 = 1.e0 / (sn * c * TranMerc_Scale_Factor);

    t15 = (1.e0 + 2.e0 * tan2 + eta) / (6.e0 * pow(sn, 3.0) * c *
                                        pow(TranMerc_Scale_Factor, 3.0));

    t16 = (5.e0 + 6.e0 * eta + 28.e0 * tan2 - 3.e0 * eta2
           + 8.e0 * tan2 * eta + 24.e0 * tan4 - 4.e0
           * eta3 + 4.e0 * tan2 * eta2 + 24.e0
           * tan2 * eta3) / (120.e0 * pow(sn, 5.0) * c
                             * pow(TranMerc_Scale_Factor, 5.0));

    t17 = (61.e0 +  662.e0 * tan2 + 1320.e0 * tan4 + 720.e0
           * pow(t, 6.0)) / (5040.e0 * pow(sn, 7.0) * c
                             * pow(TranMerc_Scale_Factor, 7.0));

    /* Difference in Longitude */
    dlam = de * t14 - pow(de, 3.0) * t15 + pow(de, 5.0) * t16 - pow(de, 7.0) * t17;

    /* Longitude */
    double longitude = TranMerc_Origin_Long + dlam;

    if((longitude) > (g_pi))
    {
        longitude -= (2 * g_pi);
    }
    else if((longitude) < (-g_pi))
    {
        longitude += (2 * g_pi);

    }

    if (fabs(dlam) > (9.0 * g_pi / 180) * cos(latitude))
    { /* Distortion will result if longitude is more than 9 degrees from the Central Meridian at the equator */
        /* and decreases to 0 degrees at the poles */
        /* As you move towards the poles, distortion will become more significant */
        cout <<"TransvrseMercator: Warning: big distorsion"<<endl;
    }
    xGeo = longitude*180/g_pi;
    yGeo = latitude*180/g_pi;
    return true;
}

bool MTransverseMercatorProjection::FromGeodetic(const double &xGeo, const double &yGeo, const double &, double &xLocal, double &yLocal, double &)
{
    double tn;         /* True Meridianal distance constant  */
    double tn2;
    double tn3;
    double tn4;
    double tn5;

    double semiMajorAxis = this->m_greataxis;
    double flattening = this->m_pressfactor;
    double TranMerc_Origin_Lat = m_latitudeOfTrueScale*g_pi/180;
    if (m_meridian > 180)
        m_meridian -= (2*180);
    double TranMerc_Origin_Long = m_meridian*g_pi/180;
    double TranMerc_False_Northing = m_falsenorthing;
    double TranMerc_False_Easting = m_falseeasting;
    double TranMerc_Scale_Factor = m_scaleFactor;

    /* Eccentricity Squared */
    TranMerc_es = 2 * flattening - flattening * flattening;
    /* Second Eccentricity Squared */
    TranMerc_ebs = (1 / (1 - TranMerc_es)) - 1;

    double TranMerc_b = semiMajorAxis * (1 - flattening);
    /*True meridianal constants  */
    tn = (semiMajorAxis - TranMerc_b) / (semiMajorAxis + TranMerc_b);
    tn2 = tn * tn;
    tn3 = tn2 * tn;
    tn4 = tn3 * tn;
    tn5 = tn4 * tn;

    TranMerc_ap = semiMajorAxis * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
                                   + 81.e0 * (tn4 - tn5)/64.e0 );
    TranMerc_bp = 3.e0 * semiMajorAxis * (tn - tn2 + 7.e0 * (tn3 - tn4)
                                          /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
    TranMerc_cp = 15.e0 * semiMajorAxis * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
    TranMerc_dp = 35.e0 * semiMajorAxis * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
    TranMerc_ep = 315.e0 * semiMajorAxis * (tn4 - tn5) / 512.e0;

    double c;       /* Cosine of latitude                          */
    double c2;
    double c3;
    double c5;
    double c7;
    double dlam;    /* Delta longitude - Difference in Longitude       */
    double eta;     /* constant - TranMerc_ebs *c *c                   */
    double eta2;
    double eta3;
    double eta4;
    double s;       /* Sine of latitude                        */
    double sn;      /* Radius of curvature in the prime vertical       */
    double t;       /* Tangent of latitude                             */
    double tan2;
    double tan3;
    double tan4;
    double tan5;
    double tan6;
    double t1;      /* Term in coordinate conversion formula - GP to Y */
    double t2;      /* Term in coordinate conversion formula - GP to Y */
    double t3;      /* Term in coordinate conversion formula - GP to Y */
    double t4;      /* Term in coordinate conversion formula - GP to Y */
    double t5;      /* Term in coordinate conversion formula - GP to Y */
    double t6;      /* Term in coordinate conversion formula - GP to Y */
    double t7;      /* Term in coordinate conversion formula - GP to Y */
    double t8;      /* Term in coordinate conversion formula - GP to Y */
    double t9;      /* Term in coordinate conversion formula - GP to Y */
    double tmd;     /* True Meridianal distance                        */
    double tmdo;    /* True Meridianal distance for latitude of origin */
    double temp_Origin;
    double temp_Long;

    double longitude = xGeo*g_pi/180;
    double latitude = yGeo*g_pi/180;

    if ((latitude < -((g_pi * 89.99)/180.0)) || (latitude > ((g_pi * 89.99)/180.0)))
    {  /* Latitude out of range */
        cout<<"Latitude out of range"<<endl;
    }
    if (longitude > g_pi)
        longitude -= (2 * g_pi);
    if ((longitude < (TranMerc_Origin_Long - ((g_pi * 90)/180.0)))
            || (longitude > (TranMerc_Origin_Long + ((g_pi * 90)/180.0))))
    {
        if (longitude < 0)
            temp_Long = longitude + 2 * g_pi;
        else
            temp_Long = longitude;
        if (TranMerc_Origin_Long < 0)
            temp_Origin = TranMerc_Origin_Long + 2 * g_pi;
        else
            temp_Origin = TranMerc_Origin_Long;
        if ((temp_Long < (temp_Origin - ((g_pi * 90)/180.0)))
                || (temp_Long > (temp_Origin + ((g_pi * 90)/180.0))))
            cout<<"Longitude out of range"<<endl;
    }



    /*
   *  Delta Longitude
   */
    dlam = longitude - TranMerc_Origin_Long;

    if (fabs(dlam) > (9.0 * g_pi / 180))
    { /* Distortion will result if Longitude is more than 9 degrees from the Central Meridian */
        cout<<"Distortion will result if Longitude is more than 9 degrees from the Central Meridian"<<endl;
    }

    if (dlam > g_pi)
        dlam -= (2 * g_pi);
    if (dlam < -g_pi)
        dlam += (2 * g_pi);
    if (fabs(dlam) < 2.e-10)
        dlam = 0.0;

    s = sin(latitude);
    c = cos(latitude);
    c2 = c * c;
    c3 = c2 * c;
    c5 = c3 * c2;
    c7 = c5 * c2;
    t = tan (latitude);
    tan2 = t * t;
    tan3 = tan2 * t;
    tan4 = tan3 * t;
    tan5 = tan4 * t;
    tan6 = tan5 * t;
    eta = TranMerc_ebs * c2;
    eta2 = eta * eta;
    eta3 = eta2 * eta;
    eta4 = eta3 * eta;

    /* radius of curvature in prime vertical */
    sn = sphsn(latitude);

    /* True Meridianal Distances */
    tmd = sphtmd(latitude);

    /*  Origin  */
    tmdo = sphtmd (TranMerc_Origin_Lat);

    /* northing */
    t1 = (tmd - tmdo) * TranMerc_Scale_Factor;
    t2 = sn * s * c * TranMerc_Scale_Factor/ 2.e0;
    t3 = sn * s * c3 * TranMerc_Scale_Factor * (5.e0 - tan2 + 9.e0 * eta
                                                + 4.e0 * eta2) /24.e0;

    t4 = sn * s * c5 * TranMerc_Scale_Factor * (61.e0 - 58.e0 * tan2
                                                + tan4 + 270.e0 * eta - 330.e0 * tan2 * eta + 445.e0 * eta2
                                                + 324.e0 * eta3 -680.e0 * tan2 * eta2 + 88.e0 * eta4
                                                -600.e0 * tan2 * eta3 - 192.e0 * tan2 * eta4) / 720.e0;

    t5 = sn * s * c7 * TranMerc_Scale_Factor * (1385.e0 - 3111.e0 *
                                                tan2 + 543.e0 * tan4 - tan6) / 40320.e0;

    yLocal = TranMerc_False_Northing + t1 + pow(dlam, 2.e0) * t2
            + pow(dlam,4.e0) * t3 + pow(dlam,6.e0) * t4
            + pow(dlam,8.e0) * t5;

    /* Easting */
    t6 = sn * c * TranMerc_Scale_Factor;
    t7 = sn * c3 * TranMerc_Scale_Factor * (1.e0 - tan2 + eta ) /6.e0;
    t8 = sn * c5 * TranMerc_Scale_Factor * (5.e0 - 18.e0 * tan2 + tan4
                                            + 14.e0 * eta - 58.e0 * tan2 * eta + 13.e0 * eta2 + 4.e0 * eta3
                                            - 64.e0 * tan2 * eta2 - 24.e0 * tan2 * eta3 )/ 120.e0;
    t9 = sn * c7 * TranMerc_Scale_Factor * ( 61.e0 - 479.e0 * tan2
                                             + 179.e0 * tan4 - tan6 ) /5040.e0;

    xLocal = TranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7
            + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;


    return true;
}
double MTransverseMercatorProjection::sphtmd( double latitude )
{
    return TranMerc_ap * latitude
            - TranMerc_bp * sin(2.e0 * latitude) + TranMerc_cp * sin(4.e0 * latitude)
            - TranMerc_dp * sin(6.e0 * latitude) + TranMerc_ep * sin(8.e0 * latitude);
}


double MTransverseMercatorProjection::sphsn( double  latitude )
{
    return this->m_greataxis / sqrt( 1.e0 - TranMerc_es * pow(sin(latitude), 2.0));
}


double MTransverseMercatorProjection::sphsr( double latitude )
{
    double denom = sqrt(1.e0 - TranMerc_es * pow(sin(latitude), 2.0));
    return this->m_greataxis * (1.e0 - TranMerc_es) / pow(denom, 3.0);
}
