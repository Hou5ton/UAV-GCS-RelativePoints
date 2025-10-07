#include "Coo.h"
#include "ArrBase.h"
#include "Globals.h"
#include "Refglobal.h"
#include <QTextCodec>

using namespace std;

//==========================================================================================
//!Function of determination of parameters of one of standard ellipsoids
/**input parameters;
//        ellipsoidname - name of an ellipsoid
//output parameters
//        greataxis - big semiaxis
//        pressfactor - compression coefficient
//returns 'true',
//if information on a required ellipsoid exists.
//else returns 'false'
*/
bool GetEllipsoidParameters(MString ellipsoidname, double& greataxis, double& pressfactor)
//==========================================================================================
{
    //if the required ellipsoid is supported by us
    if(ellipsoidname==ELLIPSOID_KRASOVSKI_42)
    {
        greataxis = 6378245.0;
        pressfactor = 1.0/298.3;
        return true;
    }
    else if(ellipsoidname==ELLIPSOID_PZ_90)
    {
        greataxis = 6378136.0;
        pressfactor = 1.0/298.257839303;
        return true;
    }
    else if(ellipsoidname==ELLIPSOID_WGS_84)
    {
        greataxis = 6378137.0;
        pressfactor = 1.0/298.257223563;
        return true;
    }
    else if(ellipsoidname==ELLIPSOID_GRS_80)
    {
        greataxis = 6378137.0;
        pressfactor = 1.0/298.257222101;
        return true;
    }
    else
    {
        return false;
    }
}

//==========================================================================================
//!Function of determination of parameters of transformation of system of coordinates
//
double MCoo::ecentricity(void)
{
    return sqrt(ecentricitySquare());
}
double MCoo::ecentricitySquare(void) const
{
    return (2 * m_pressfactor - pow(m_pressfactor, 2));
}
double MCoo::curvatureRadius(MPoint pt) const
{
    return curvatureRadius(pt.y);
}
double MCoo::curvatureRadius(double b) const
{
    double e_2 = ecentricitySquare();
    return curvatureRadius(b, e_2);
}
double MCoo::curvatureRadius(double b, double e_2) const
{
    return (m_greataxis / sqrt(1 - e_2 * pow(sin(b), 2)));
}
double MCoo::curvatureRadius(MPoint pt, double e_2) const
{
    return curvatureRadius(pt.y, e_2);
}

bool MCoo::GetEllipsTransform(MString fromEllips,MString toEllips,MEllipsTransform &tParam)
{
    MString sum = fromEllips+toEllips;
    //SK42 -> WGS84 //GOST 51794-2001
    if((sum == MString(ELLIPSOID_KRASOVSKI_42)+ELLIPSOID_WGS_84)||(sum == MString(ELLIPSOID_WGS_84)+ELLIPSOID_KRASOVSKI_42))
    {
        tParam.dX = 23.57;
        tParam.dY = -141.95;
        tParam.dZ = -79.8;
        tParam.Wx = 0.0;
        tParam.Wy = -0.35;
        tParam.Wz = -0.79;
        tParam.m = -0.22e-6;
        MString dtm(ELLIPSOID_KRASOVSKI_42);
        tParam.direct_conv = (sum.Left(dtm.size())==dtm)?true:false;
        return true;
    }
    //SK-42 -> PZ-90 //GOST 51794-2008
    else if ((sum ==MString(ELLIPSOID_KRASOVSKI_42)+ELLIPSOID_PZ_90)||(sum ==MString(ELLIPSOID_PZ_90)+ELLIPSOID_KRASOVSKI_42))
    {
        tParam.dX = 25;
        tParam.dY = -141;
        tParam.dZ = -80;
        tParam.Wx = 0.0;
        tParam.Wy = -0.35;
        tParam.Wz = -0.66;
        tParam.m = 0.0;
        MString dtm(ELLIPSOID_KRASOVSKI_42);
        tParam.direct_conv = (sum.Left(dtm.size())==dtm)?true:false;
        return true;
    }
    //WGS84 -> PZ-90 //GOST 51794-2001
    else if((sum ==MString(ELLIPSOID_WGS_84)+ELLIPSOID_PZ_90)||(sum ==MString(ELLIPSOID_PZ_90)+ELLIPSOID_WGS_84))
    {
        tParam.dX =1.08 ;
        tParam.dY =0.27 ;
        tParam.dZ =0.9 ;
        tParam.Wx =0.0 ;
        tParam.Wy =0.0 ;
        tParam.Wz =0.16 ;
        tParam.m =0.12e-6 ;
        MString dtm(ELLIPSOID_WGS_84);
        tParam.direct_conv = (sum.Left(dtm.size())==dtm)?true:false;
        return true;
    }

    return false;
}

//=========================================================
//!static function of creation of system of coordinates on its type
MCoo* CreateCooByType(MString cootype)
//=========================================================
{
    if(cootype==COO_PARENT)
    {
        return new MCoo();
    }
    else if(cootype==COO_GEODESIC)
    {
        return (MCoo*)new MGeodesicCoo();
    }
    else if(cootype==COO_TOPOCENTRIC)
    {
        return (MCoo*)new MTopocentricCoo();
    }
    else if(cootype==COO_EXTENDEDGAUSSKRUGER)
    {
        return (MCoo*)new MExtendedGaussKrugerCoo();
    }
    else if(cootype==COO_STANDARTGAUSSKRUGER)
    {
        return (MCoo*)new MStandartGaussKrugerCoo();
    }
    else if(cootype==COO_ZONEGAUSSKRUGER)
    {
        return (MCoo*)new MZoneGaussKrugerCoo();

    }
    else if(cootype==COO_PSEVDOGAUSSKRUGER)
    {
        return (MCoo*)new MPsevdoGaussKrugerCoo();
    }

    else if (cootype == COO_TRANSVERSEMERCATOR)
    {
        return (MCoo*)new MTransverseMercatorCoo();
    }
    else if (cootype == COO_UTM)
    {
        return (MCoo*)new MUtmCoo();
    }

    else if (cootype == COO_MERCATOR)
    {
        return (MCoo*)new MMercatorCoo();
    }

    else if (cootype == COO_LAMBERTCONIC)
    {
        return (MCoo*)new MLambertConicCoo();
    }

    else
    {
        return NULL;
    }

}


//*****************************************************
//MCoo
//*****************************************************


//===============================================================
//!Constructor by default sets an ellipsoid of Krasovsky 1942
MCoo::MCoo(const MString &ellipsoidName)
//===============================================================
{
    SetStandartEllipsoid(ellipsoidName);
}

MCoo::~MCoo()
{

}
//========================================================
//!Setting of parameters of one of standard ellipsoids
bool MCoo::SetStandartEllipsoid(MString ellipsename)
//========================================================
{
    //if the name of a required ellipsoid isn't known,
    //we change nothing that, and we return 'false'
    //in case of success we return 'true'
    double greataxis;
    double pressfactor;
    if(!GetEllipsoidParameters(ellipsename, greataxis, pressfactor))
    {
        return false;
    }

    return SetCustomEllipsoid(ellipsename, greataxis, pressfactor);
}

//=============================================================================================
//!Setting of an ellipsoid with any parameters
bool MCoo::SetCustomEllipsoid(MString ellipsename, double greataxis, double pressfactor)
//==============================================================================================
{
    //if big semiaxis is equal 0, we will establish nothing
    if(greataxis==0.0)
        return false;

    //now we will establish a new ellipsoid
    m_ellipsoidname = ellipsename;

    m_greataxis = greataxis;
    m_pressfactor = pressfactor;

    return true;
}

//======================================================================
//!function filling two arrays of lines with names of parameters
//! and their values
bool MCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    if((parnames==NULL)||(pardata==NULL))
    {
        return false;
    }

    parnames->SetSize(0);
    pardata->SetSize(0);
    return true;
}

//==========================================================================
//!function establishing the parameters taken from arrays
bool MCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
//==========================================================================
{
    //we receive nothing from arrays
    if((parnames==NULL)||(pardata==NULL))
        return false;

    if(parnames->GetSize()!=pardata->GetSize())
        return false;

    return true;
}


//==============================================================================================
//!returns the size in bytes at record in the file
int MCoo::GetBytes()
//==============================================================================================
{
    //the size of coordinates consists of the following a component:
    //1. identifier 3b size.
    //2. name size: 2b + length of a line
    //3. size of a boljshy half shaft 8b
    //4. compression coefficient size 8b

    return 21+m_ellipsoidname.GetLength();
}

//==============================================================================================
//!Saving in the file
bool MCoo::WriteBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //depending on the version
    switch(version)
    {
    case 0:
    {
        //save identifier
        fwrite((char *)WhatTheCoo(), 3, 1, h_nuxfile);
        //name
        WriteString(h_nuxfile,version,m_ellipsoidname);
        //big semiaxis
        fwrite(&m_greataxis, sizeof(double), 1, h_nuxfile);
        //compression coefficient
        fwrite(&m_pressfactor, sizeof(double), 1, h_nuxfile);
    }
        break;
    default:
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from the file
bool MCoo::ReadBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //depending on the version
    switch(version)
    {
    case 0:
    {
        //name
        m_ellipsoidname = ReadString(h_nuxfile,version);

        const QTextCodec *codecWindows1251 = QTextCodec::codecForName(mapCharset);
        const MString ellipsoidnameFromUtp = codecWindows1251->toUnicode(m_ellipsoidname).toStdString();
        const MString ellipsoidnameFromSrc_KRAS = ELLIPSOID_KRASOVSKI_42_RUS;
        const MString ellipsoidnameFromSrc_PZ90 = ELLIPSOID_PZ_90_RUS;
        if(ellipsoidnameFromUtp.Compare(ellipsoidnameFromSrc_KRAS) == 0)
        {
            m_ellipsoidname = ELLIPSOID_KRASOVSKI_42;
        }
        else if(ellipsoidnameFromUtp.Compare(ellipsoidnameFromSrc_PZ90) == 0)
        {
            m_ellipsoidname = ELLIPSOID_PZ_90_RUS;
        }

        //big semiaxis
        fread(&m_greataxis, sizeof(m_greataxis), 1, h_nuxfile);
        //compression coefficient
        fread(&m_pressfactor, sizeof(m_pressfactor), 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_greataxis, sizeof(m_greataxis) );
        if (reverseReadFlag) reverseValue((char*)&m_pressfactor, sizeof(m_pressfactor) );
    }
        break;
    default:
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from the file
bool MCoo::ReadBin(MFile* file,   int version)
//==============================================================================================
{
    //depending on the version
    switch(version)
    {
    case 0:
    {
        //name
        m_ellipsoidname = ReadString(file,version);

        const QTextCodec *codecWindows1251 = QTextCodec::codecForName(mapCharset);
        const MString ellipsoidnameFromUtp = codecWindows1251->toUnicode(m_ellipsoidname).toStdString();
        const MString ellipsoidnameFromSrc_KRAS = ELLIPSOID_KRASOVSKI_42_RUS;
        const MString ellipsoidnameFromSrc_PZ90 = ELLIPSOID_PZ_90_RUS;
        if(ellipsoidnameFromUtp.Compare(ellipsoidnameFromSrc_KRAS) == 0)
        {
            m_ellipsoidname = ELLIPSOID_KRASOVSKI_42;
        }
        else if(ellipsoidnameFromUtp.Compare(ellipsoidnameFromSrc_PZ90) == 0)
        {
            m_ellipsoidname = ELLIPSOID_PZ_90_RUS;
        }

        //big semiaxis
        file->GetData(&m_greataxis, sizeof(m_greataxis), 1);
        //compression coefficient
        file->GetData(&m_pressfactor, sizeof(m_pressfactor), 1);
        if (reverseReadFlag) reverseValue((char*)&m_greataxis, sizeof(m_greataxis) );
        if (reverseReadFlag) reverseValue((char*)&m_pressfactor, sizeof(m_pressfactor) );
    }
        break;
    default:
        return false;
    }

    return true;
}

//!==============================================================================================
bool MCoo::Copy(MCoo* pCoo)
//==============================================================================================
{
    if(!pCoo)
        return false;

    //we can make the copy of other system of coordinates only,
    //if it too the parental
    if(WhatTheCoo()==pCoo->WhatTheCoo())
    {
        return SetCustomEllipsoid(pCoo->m_ellipsoidname, pCoo->m_greataxis, pCoo->m_pressfactor);
    }

    return false;
}

//==============================================================================================
//!comparing

bool MCoo::IsEqualTo(MCoo* pCoo)
//==============================================================================================
{
    return (pCoo  && (WhatTheCoo() == pCoo->WhatTheCoo()) &&
            (m_ellipsoidname == pCoo->m_ellipsoidname) &&
            (m_greataxis == pCoo->m_greataxis) &&
            (m_pressfactor == pCoo->m_pressfactor));
}

//=========================================================================================
//!the translation of a point from this system of coordinates in another
bool MCoo::ConvertToCoo(MCoo* pNewCoo, MPoint& point, double& h)
{
    //check indexes
    if(!pNewCoo)
        return false;

    //if systems of coordinates are equivalent,
    //that is no need to spoil type Double
    if(IsEqualTo(pNewCoo))
        return true;

    //transfer to BLH from old system
    //and then from BLH in new system
    MPoint newpt(0,0);
    double newh=0;
    MPoint oldpt = point;
    double oldh = h;
    if(!ToBLH(oldpt, oldh, newpt, newh))
        return false;

    //if ellipsoids are different, we pass from one system of coordinates to another
    if (m_ellipsoidname!=pNewCoo->m_ellipsoidname)
    {
        MEllipsTransform tParam;
        if (GetEllipsTransform(m_ellipsoidname,pNewCoo->m_ellipsoidname,tParam))
        {
            ConvertDatum(newpt, h, newpt, newh,pNewCoo, &tParam);
            point = newpt;
            h = newh;
        }
    }

    return pNewCoo->FromBLH(newpt, newh, point, h);
}

void MCoo::ConvertDatum(MPoint point, double h, MPoint& newPoint, double &newH, MCoo* pNewCoo, MEllipsTransform* tParam)
{
    M3DPoint xyzPoint, newxyzPoint;

    this->ToXYZ(point, h, xyzPoint);

    this->ConvertDatum(xyzPoint, newxyzPoint, pNewCoo, tParam);
    MString oldDatum = this->m_ellipsoidname;
    this->SetStandartEllipsoid(pNewCoo->m_ellipsoidname);
    this->FromXYZ(newxyzPoint, newPoint, newH);
    this->SetStandartEllipsoid(oldDatum);
}

//===============================================================================================
//transformation of coordinates of XYZ from this system of coordinates (A) in other system of coordinates (B)
void MCoo::ConvertDatum(M3DPoint point, M3DPoint& newPoint, MCoo* pNewCoo, MEllipsTransform* tParam)
{
    double wx = tParam->Wx * pi / (3600 * 180);
    double wy = tParam->Wy * pi / (3600 * 180);
    double wz = tParam->Wz * pi / (3600 * 180);

    double TrMatrix[3][3] = { {1, wz, -wy},
                              {-wz, 1, wx},
                              {wy, -wx, 1}};

    //correction of a matrix for performance of the return transformation
    if (!tParam->direct_conv)
    {
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                if ( i != j )
                    TrMatrix[i][j] *= -1;

    }

    double delta[3] = {tParam->dX, tParam->dY, tParam->dZ};
    double oldCoo[3] = {point.x, point.y, point.z};
    double newCoo[3];
    double m = tParam->m;


    //realization of transformation of matrixes
    //at direct transformation:    (1 + m) * TrMatrix[l x n] * oldCoo[n] + delta[n]
    //at the return perobrazovaniye: (1 - m) * TrMatrix[l x n] * oldCoo[n] - delta[n]
    int l = 3;
    int n = 3;
    for(int i = 0; i < l; ++i)
    {
        double s = 0;
        for(int k = 0; k < n; ++k)
            s = s + TrMatrix[i][k] * oldCoo[k];
        if (tParam->direct_conv)
            newCoo[i] = (1 + m) * s + delta[i]; //direct
        else
            newCoo[i] = (1 - m) * s - delta[i]; //return
    }
    newPoint.x = newCoo[0];
    newPoint.y = newCoo[1];
    newPoint.z = newCoo[2];

}

double MCoo::pressfactor() const
{
    return m_pressfactor;
}

void MCoo::setPressfactor(double newPressfactor)
{
    m_pressfactor = newPressfactor;
}

double MCoo::greataxis() const
{
    return m_greataxis;
}

void MCoo::setGreataxis(double newGreataxis)
{
    m_greataxis = newGreataxis;
}

void MCoo::setEllipsoidname(const MString &newEllipsoidname)
{
    m_ellipsoidname = newEllipsoidname;
}

bool MCoo::FromXYZ(M3DPoint oldpoint, MPoint& newpoint, double& newh) const
{
    double x = oldpoint.x, y = oldpoint.y, z = oldpoint.z;
    // calculating D
    double D = sqrt(x*x + y*y);
    double B, L, H;
    double a = this->m_greataxis;
    double e_2 = this->ecentricitySquare();//2 * this->m_pressfactor - pow(this->m_pressfactor, 2);

    if (D == 0)
    {
        B = pi * z / (2.0 * fabs(z));
        L = 0;
        H = z * sin(B) - a * sqrt(1 - e_2 * pow( sin(B), 2));
    }
    else // D!=0
    {
        double La = fabs( asin(y / D));
        if (y < 0 && x > 0)
        { L = 2 * pi - La; }
        if (y < 0 && x < 0)
        { L = pi + La; }
        if (y > 0 && x < 0)
        { L = pi - La; }
        if (y > 0 && x > 0)
        { L = La; }

        if (z == 0)
        {
            B = 0;
            H = D - a;
        }
        else
        {
            double r = sqrt( x*x + y*y + z*z);
            double c = asin(z / r);
            double p = e_2 * a / (2 * r);
            double s1 = 0;
            double b = 0, s2 = 0;
            double d = 0;
            //the admission of accuracy makes 10^(-4) seconds, then calculation of geodetic height is made with an accuracy 0.003 in m
            //below a transfer of seconds in radians is made
            double delta = 0.0001/3600 * pi/180;

            do
            {
                s1 = s2;
                b = c + s1;
                s2 = asin(p * sin(2 * b) / sqrt(1 - e_2 * pow(sin(b), 2)));
                d = fabs(s2 - s1);
            }
            while( d >= delta);
            B = b;
            H = D * cos(B) + z * sin(B) - a * sqrt(1 - e_2 * pow(sin(B), 2));
        }

    }
    newpoint.x = L * 180 / pi;
    newpoint.y = B * 180 / pi;
    newh = H;
    return true;
}

bool MCoo::ToXYZ(MPoint oldpoint, double oldh, M3DPoint& newpoint) const
{
    double B = oldpoint.y * pi / 180, L = oldpoint.x * pi / 180, H = oldh;
    //double a = this->m_greataxis;
    double e_2 = this->ecentricitySquare();//2 * this->m_pressfactor - pow(this->m_pressfactor, 2);
    double N = this->curvatureRadius(B);//a / sqrt( 1 - e_2 * pow(sin(B), 2));

    newpoint.x = (N + H) * cos(B) * cos(L);
    newpoint.y = (N + H) * cos(B) * sin(L);
    newpoint.z = ((1 - e_2) * N + H) * sin(B);

    return true;
}
//===============================================================================================
//!calculation of an azimuth and range between points
//parameters (MPoint pt1, MPoint pt2, double& azimut, double& distance)
void MCoo::FindAzimutAndDistance(MPoint , MPoint , double& azimut, double& distance)
//===============================================================================================
{
    distance = 0;
    azimut = 0;
}

//=========================================
//!formation of a text type of coordinates
MString MCoo::CreateText(MPoint point)
//=========================================
{
    MString result;
    result.Format("X:  %.3f ; Y:  %.3f", point.x, point.y);
    return result;
}

//***************************************************************************
//MGeodesicCoo
//***************************************************************************

//==============================================================================================
//!Constructor by default
MGeodesicCoo::MGeodesicCoo()
//==============================================================================================
    :MCoo()
{
    //do nothing more since this system of coordinates has no other parameters in comparison with
    //parent
}

//==============================================================================================
//!translation of coordinates in this system of coordinates in longitude-width-height
bool MGeodesicCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//==============================================================================================
{
    //anything it isn't necessary to translate anywhere,
    //since points in this system are also BLH
    newpoint = oldpoint;
    newh = oldh;
    return true;
}

//==============================================================================================
//!the translation of coordinates of a point in longitude-width-height in our system of coordinates
bool MGeodesicCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//==============================================================================================
{
    //anything it isn't necessary to translate anywhere,
    //since points in this system are also BLH
    newpoint = oldpoint;
    newh = oldh;
    return true;
}

//===============================================================================================
//!calculation of an azimuth and range between points
void MGeodesicCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance)
//===============================================================================================
{
    distance = 0;
    azimut  = pt2.x - pt1.x;
}

//=========================================
//!formation of a text type of coordinates
MString MGeodesicCoo::CreateText(MPoint point)
//=========================================
{
    MString strB = (point.y<0)? IDS_SOUTH_LAT : IDS_NORTH_LAT ;//"S.lat" : "N.lat";
    MString strL = (point.x<0)? IDS_WEST_LONG : IDS_EAST_LONG ;//"W.long" : "E.long";

    point.x = fabs(point.x);
    point.y = fabs(point.y);

    int Bg = (int)floor(point.y);
    int Bm = (int)floor((point.y-(double)Bg)*60.0);
    int Bs = (int)floor((point.y-(double)Bg-(double)Bm/60.0)*3600.0);

    int Lg = (int)floor(point.x);
    int Lm = (int)floor((point.x-(double)Lg)*60.0);
    int Ls = (int)floor((point.x-(double)Lg-(double)Lm/60.0)*3600.0);

    MString result;
    result.Format("B: %d\u00B0%d'%d'' %s ; L: %d\u00B0%d'%d'' %s" ,Bg, Bm, Bs, strB.c_str(), Lg, Lm, Ls, strL.c_str());
    return result;
}

//***************************************************************************
//MTopocentricCoo
//***************************************************************************

//==============================================================================================
//!constructor
MTopocentricCoo::MTopocentricCoo(const MString &ellipsoidName)
    : MCoo(ellipsoidName)
{
    ptCnt.x = ptCnt.y = hCnt = 0;
}

MTopocentricCoo::MTopocentricCoo(MPoint point, double h, const MString &ellipsoidName)
    : MCoo(ellipsoidName)
//==============================================================================================
{
    ptCnt = point;
    hCnt = h;

    xCnt = ptCnt.x * pi / 180;
    yCnt = ptCnt.y * pi / 180;
    sinLatCnt = sin(yCnt);
    cosLatCnt = cos(yCnt);
    e2 = pressfactor() * (2 - pressfactor());
    n = greataxis() / sqrt(1 - e2 * pow(sinLatCnt, 2));
}

//==============================================================================================
//!converting geodesic coordinates to topocentric
bool MTopocentricCoo::FromBLH(MPoint oldpoint, double oldh, M3DPoint& newpoint) const
//==============================================================================================
{	
    ToXYZ(oldpoint, oldh, newpoint);
    FromGeocentricToTopo(newpoint, newpoint);
    return true;
}

//==============================================================================================
//!converting topocentric coordinates to geodesic
bool MTopocentricCoo::ToBLH(M3DPoint oldpoint, MPoint& newpoint, double& newh) const
//==============================================================================================
{	
    FromTopoToGeocentric(oldpoint, oldpoint);
    FromXYZ(oldpoint, newpoint, newh);
    if(newpoint.x > 180)
        newpoint.x -= 360;
    return true;
}

//==============================================================================================
//!converting geocentric coordinates to topocentric
bool MTopocentricCoo::FromGeocentricToTopo(M3DPoint oldpoint, M3DPoint& newpoint) const
//==============================================================================================
{		
    double xTemp, yTemp;

    newpoint = oldpoint;
    newpoint.z += e2 * n * sinLatCnt;
    xTemp = newpoint.x * cos(xCnt) + newpoint.y * sin(xCnt);
    yTemp = -newpoint.x * sin(xCnt) + newpoint.y * cos(xCnt);
    newpoint.x = xTemp;
    newpoint.y = yTemp;
    yTemp = newpoint.z * sinLatCnt + newpoint.x * cosLatCnt;
    xTemp = -newpoint.z * cosLatCnt + newpoint.x * sinLatCnt;
    newpoint.z = yTemp;
    newpoint.x = xTemp;
    newpoint.z -= n + hCnt;
    newpoint.x = -newpoint.x;
    return true;
}

//==============================================================================================
//!converting topocentric coordinates to geocentric
bool MTopocentricCoo::FromTopoToGeocentric(M3DPoint oldpoint, M3DPoint& newpoint) const
//==============================================================================================
{	
    double xTemp, yTemp;

    newpoint = oldpoint;
    newpoint.x = -newpoint.x;
    newpoint.z += n + hCnt;
    yTemp = newpoint.z * sinLatCnt - newpoint.x * cosLatCnt;
    xTemp = newpoint.z * cosLatCnt + newpoint.x * sinLatCnt;
    newpoint.z = yTemp;
    newpoint.x = xTemp;
    xTemp = newpoint.x * cos(xCnt) - newpoint.y * sin(xCnt);
    yTemp = newpoint.x * sin(xCnt) + newpoint.y * cos(xCnt);
    newpoint.x = xTemp;
    newpoint.y = yTemp;
    newpoint.z -= e2 * n * sinLatCnt;
    return true;
}

//***************************************************************************
//M2DTopocentricCoo
//***************************************************************************

//==============================================================================================
//!constructor
M2DTopocentricCoo::M2DTopocentricCoo(const MString& ellipsoidName)
    : MCoo(ellipsoidName)
{
    ptCnt.x = ptCnt.y = hCnt = aziX = 0;
}
M2DTopocentricCoo::M2DTopocentricCoo(MPoint point, double h, double azimuth, const MString & ellipsoidName)
    : MCoo(ellipsoidName)
//==============================================================================================
{
    ptCnt = point;
    hCnt = h;
    aziX = azimuth;

    xCnt = ptCnt.x * pi / 180;
    yCnt = ptCnt.y * pi / 180;
    sinLatCnt = sin(yCnt);
    cosLatCnt = cos(yCnt);
    aziRad = aziX * pi / 180;
    e2 = pressfactor() * (2 - pressfactor());
    n = greataxis() / sqrt(1 - e2 * pow(sinLatCnt, 2));
}

M2DTopocentricCoo::~M2DTopocentricCoo()
{
}
//==============================================================================================
void M2DTopocentricCoo::setOrigin(MPoint neworigin)
{
    ptCnt = neworigin;
    xCnt = ptCnt.x * pi / 180;
    yCnt = ptCnt.y * pi / 180;
    sinLatCnt = sin(yCnt);
    cosLatCnt = cos(yCnt);
    e2 = pressfactor() * (2 - pressfactor());
    n = greataxis() / sqrt(1 - e2 * pow(sinLatCnt, 2));
}
//!converting geodesic coordinates to 2D topocentric
bool M2DTopocentricCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint) const
//==============================================================================================
{	
    M3DPoint geocentric;
    ToXYZ(oldpoint, oldh, geocentric);
    FromGeocentricTo2DTopo(geocentric, newpoint);
    return true;
}

//==============================================================================================
//!converting 2D topocentric coordinates to geodesic
bool M2DTopocentricCoo::ToBLH(MPoint oldpoint, MPoint& newpoint, double& newh) const
//==============================================================================================
{	
    M3DPoint geocentric;
    From2DTopoToGeocentric(oldpoint, geocentric);
    FromXYZ(geocentric, newpoint, newh);
    if(newpoint.x > 180)
        newpoint.x -= 360;
    return true;
}

//==============================================================================================
//!converting geocentric coordinates to 2D topocentric
bool M2DTopocentricCoo::FromGeocentricTo2DTopo(M3DPoint oldpoint, MPoint& newpoint) const
//==============================================================================================
{		
    double xTemp, yTemp;

    newpoint.y = oldpoint.z + e2 * n * sinLatCnt;
    newpoint.x = oldpoint.x * cos(xCnt) + oldpoint.y * sin(xCnt);
    yTemp = newpoint.y * sinLatCnt + newpoint.x * cosLatCnt;
    xTemp = - newpoint.y * cosLatCnt + newpoint.x * sinLatCnt;
    newpoint.y = yTemp;
    newpoint.x = xTemp;
    newpoint.y -= n + hCnt;
    newpoint.x = - newpoint.x * cos(aziRad) + (- oldpoint.x * sin(xCnt) + oldpoint.y * cos(xCnt)) * sin(aziRad);
    return true;
}

//==============================================================================================
//!converting 2D topocentric coordinates to geocentric
bool M2DTopocentricCoo::From2DTopoToGeocentric(MPoint oldpoint, M3DPoint& newpoint) const
//==============================================================================================
{	
    double xTemp, yTemp;

    newpoint.x = - oldpoint.x * cos(aziRad);
    newpoint.y = oldpoint.x * sin(aziRad);
    newpoint.z = oldpoint.y + n + hCnt;
    yTemp = newpoint.z * sinLatCnt - newpoint.x * cosLatCnt;
    xTemp = newpoint.z * cosLatCnt + newpoint.x * sinLatCnt;
    newpoint.z = yTemp;
    newpoint.x = xTemp;
    xTemp = newpoint.x * cos(xCnt) - newpoint.y * sin(xCnt);
    yTemp = newpoint.x * sin(xCnt) + newpoint.y * cos(xCnt);
    newpoint.x = xTemp;
    newpoint.y = yTemp;
    newpoint.z -= e2 * n * sinLatCnt;
    return true;
}

//************************************************************************************
//MExtendedGaussKrugerCoo
//************************************************************************************


//==============================================================================================
//!Constructor by default
MExtendedGaussKrugerCoo::MExtendedGaussKrugerCoo()
//==============================================================================================
    :MCoo()
{
    //by default we will establish a zero meridian and standard settings
    SetGaussParameters(0.0, 500000.0, 0.0, MPoint(0,0));
}

//==============================================================================================
//!function of setting of parameters
bool MExtendedGaussKrugerCoo::SetGaussParameters(double meridian, double falseeasting, double falsenorthing, MPoint reper)
//==============================================================================================
{
    //it is impossible to establish a meridian which is less than-180 or more than 180 degrees,
    //therefore at first we will normalize value of a meridian
    while(meridian<-180.0)
        meridian+=360.0;
    while(meridian>180)
        meridian-=360.0;

    m_meridian = meridian;
    m_falseeasting = falseeasting;
    m_falsenorthing = falsenorthing;
    m_reper = reper;

    return true;
}

//!==============================================================================================
bool MExtendedGaussKrugerCoo::SetBasicParameters(double meridian, double falseeasting, double falsenorthing)
//==============================================================================================
{
    //it is impossible to establish a meridian which is less than-180 or more than 180 degrees,
    //therefore at first we will normalize value of a meridian
    while(meridian<-180.0)
        meridian+=360.0;
    while(meridian>180)
        meridian-=360.0;

    m_meridian = meridian;
    m_falseeasting = falseeasting;
    m_falsenorthing = falsenorthing;

    return true;
}

//!==============================================================================================
bool MExtendedGaussKrugerCoo::SetReper(MPoint reper)
//=============================================================================================
{
    m_reper = reper;
    return true;
}
//==============================================================================================
//! revised 2005.01 
bool MExtendedGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//==============================================================================================
{
    //consider a reference point
    oldpoint.x += m_reper.x;
    oldpoint.y += m_reper.y;

    //consider shifts of the beginning of coordinates
    oldpoint.x -= m_falseeasting;
    oldpoint.y -= m_falsenorthing;

    // interchange the position of X and Y
    /* BEFORE vipa
    double beta = oldpoint.y/6367558.4869;
    double Bx = beta + (50221746 + (293622 + (2350 + 22*cos(beta)*cos(beta))*cos(beta)*cos(beta))*cos(beta)*cos(beta))*0.0000000001*sin(beta)*cos(beta);
    double Nx = 6399698.902 - (21562.267 - (108.973 - 0.612*cos(Bx)*cos(Bx))*cos(Bx)*cos(Bx))*cos(Bx)*cos(Bx);
    double b2 = (0.5 + 0.0033692*cos(Bx)*cos(Bx))*sin(Bx)*cos(Bx);
    double b3 = 0.333333 - (0.166667 - 0.001123*cos(Bx)*cos(Bx))*cos(Bx);
    double b4 = 0.25 + (0.16161 + 0.00562*cos(Bx)*cos(Bx))*cos(Bx)*cos(Bx);
    double b5 = 0.2 - (0.1667 - 0.0088*cos(Bx)*cos(Bx))*cos(Bx)*cos(Bx);
    double Z  = oldpoint.x/(Nx*cos(Bx));
    */
    //NEXT = vipa
    /*
    double beta = oldpoint.y/6367558.4969;
    double cosbeta= cos(beta);            // 2005.01.13  vipa (to improve performance)
    double Bx = beta + (50221746 + (293622 + (2350 + 22* cosbeta*cosbeta)* cosbeta*cosbeta) * cosbeta*cosbeta)*sin(beta)* cosbeta*0.0000000001;
    double cosBx=   cos(Bx);              // 2005.01.13  vipa (to improve performance)
    double cosBxcosBx= cosBx * cosBx;
    //double Nx = 6399698.902 - (21562.267 - (108.973 - 0.612* cosBx*cosBx)* cosBx*cosBx)* cosBx*cosBx;
    double Nx = 6399698.902 - (21562.267 - (108.973 - 0.612* cosBxcosBx)* cosBxcosBx)* cosBxcosBx;  // error: 108,973 instead of 108.973

    double b2 = (0.5 + 0.0033692* cosBxcosBx)*sin(Bx)*cosBx;

    double b3 = 0.333333 - (0.166667 - 0.001123* cosBxcosBx)*cosBxcosBx;   //  2005.01.19 vipa
    double b4 = 0.25 + (0.16161 + 0.00562* cosBxcosBx)* cosBxcosBx;
    double b5 = 0.2 - (0.1667 - 0.0088* cosBxcosBx)* cosBxcosBx;
    double Z  = oldpoint.x/(Nx*cosBx);
    // Eof vipa

    double B = Bx - (1 - (b4 - 0.12*Z*Z)*Z*Z)*Z*Z*b2;
    double l = (1 - (b3 - b5*Z*Z)*Z*Z)*Z;
    // transfer to degrees
    newpoint.x = 360.0*l/(2*pi) + m_meridian;
    newpoint.y  = 360.0*B/(2*pi);
    */

    //	int nZone;
    double a2, a4, a6, b2, b4, b6;
    double fi0, fi1, et, t, RRR, nnn, ks, ks1, et1, l, ex2;
    double xxx, yyy, eee=0.000000001;
    bool flag=false;

    xxx = oldpoint.y;
    yyy = oldpoint.x;

    nnn=(axis_X - axis_Y)/(axis_X + axis_Y);

    RRR = axis_X * (1 + nnn*nnn/4 + nnn*nnn*nnn*nnn/64) / (1+nnn);

    a2 = nnn/2 - 2*nnn*nnn/3 + 5*nnn*nnn*nnn/16;
    a4 = 13*nnn*nnn/48 - 3*nnn*nnn*nnn/5;
    a6 = 61*nnn*nnn*nnn/240;

    ks1 = 0;
    et1 = 0;
    ks=999.0;
    et=999.0;
    int counter=0;
    while (((fabs(ks-ks1)>eee) || (fabs(et-et1)>eee) )&& counter<100)
    {
        if (flag)
        {
            ks1=ks;
            et1=et;
        }
        else
            flag=true;

        ks = xxx/RRR - (a2*sin(2*ks1)*cosh(2*et1) + a4*sin(4*ks1)*cosh(4*et1) + a6*sin(6*ks1)*cosh(6*et1));
        et = yyy/RRR - (a2*cos(2*ks1)*sinh(2*et1) + a4*cos(4*ks1)*sinh(4*et1) + a6*cos(6*ks1)*sinh(6*et1));

        counter ++;
    }

    t=tanh(et);
    fi1 = atan(tan(ks)*sqrt(1-t*t) / sqrt(1+t*t*tan(ks)*tan(ks)));
    l=asin(t / cos(fi1));

    ex2 = 1-axis_Y*axis_Y/(axis_X*axis_X);
    b2 = (ex2/2 + 5*ex2*ex2/24 + ex2*ex2*ex2/12);
    b4 = (7*ex2*ex2/48 + 29*ex2*ex2*ex2/240);
    b6 = (7*ex2*ex2*ex2/120);

    fi0 = fi1 + b2*sin(2*fi1) + b4*sin(4*fi1) + b6*sin(6*fi1);

    //	if (m_meridian > 0) nZone = (int)((m_meridian - 3)/6 + 1.5);//maax
    //	else nZone = (int)((360  + m_meridian + 3)/6);

    double ro = 180 / pi;
    newpoint.x = l*ro + m_meridian;//(6*(nZone-1)+3);//maax
    newpoint.y = fi0*ro;

    // correction of values of longitude ranging from-180 to 180
    if ((newpoint.x>360) || (newpoint.x<-360))
        newpoint.x = newpoint.x - (int)(newpoint.x/360)*360;
    if (newpoint.x>180) newpoint.x-=360;	else if (newpoint.x<-180) newpoint.x+=360;

    //and height
    newh = oldh;

    return true;
}

//==============================================================================================
//parameters (MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
bool MExtendedGaussKrugerCoo::FromBLH(MPoint oldpoint, double , MPoint& newpoint, double& )
{
    //consider the central meridian
    //double l = oldpoint.x - m_meridian;
    //double B = oldpoint.y;

    //transfer to radians
    //l = (2*pi)*l/360.0;
    //B = (2*pi)*B/360.0;

    /*double cosB = cos(B);       // 2005.01.13  vipa   (to improve performance)
    double cosBcosB = cosB*cosB;
    double N = 6399698.902 - (21562.267 - (108.973 - 0.612* cosBcosB) *cosBcosB) *cosBcosB;
    double a0 = 32140.4040 - (135.3302 - (0.7092 - 0.0040*  cosBcosB ) *cosB*cosB) *cosBcosB;
    //- 0.04166 - LAT:
    double a4 = (0.25 + 0.00252* cosBcosB)* cosBcosB - 0.04166;
    double a6 = (0.166* cosBcosB - 0.084)* cosBcosB;
    double a3 = (0.3333333 + 0.001123* cosBcosB)* cosBcosB - 0.1666667;
    double a5 = 0.0083 - (0.1667 - (0.1968 + 0.0040* cosBcosB)* cosBcosB)* cosBcosB;

    newpoint.x = (1 + (a3 + a5*l*l)*l*l)*l*N*cosB;//*cos(B);  vipa
    newpoint.y = 6367558.4969*B - (a0 - (0.5 + (a4 + a6*l*l)*l*l)*l*l*N)*sin(B)*cos(B);*/
    int nZone;
    if (m_meridian > 0) nZone = (int)((m_meridian - 3)/6 + 1.5);//maax
    else nZone = (int)((360  + m_meridian + 3)/6);
    double a2, a4, a6, b2, b4, b6;
    double fi0, fi1, lam, et, t, RRR, nnn, ks;

    if ((oldpoint.y < -80) || (oldpoint.y >84))
        return false;

    // correction of entrance data of L
    if ((oldpoint.x >360) || (oldpoint.x < -360))
        oldpoint.x = oldpoint.x - (int)(oldpoint.x/360)*360;
    // Check of coordinates for a wide zone
    if ((abs((int)(oldpoint.x)- (ID_FI*2*(nZone-1)+ID_FI))>90) &&
            (abs((int)(oldpoint.x+360)- (ID_FI*2*(nZone-1)+ID_FI))>90) &&
            (abs((int)(oldpoint.x-360)- (ID_FI*2*(nZone-1)+ID_FI))>90))
        return false;

    lam = oldpoint.x - m_meridian;//(ID_FI*2*(nZone-1)+ID_FI); //maax

    double ro = 180 / pi;
    fi0 = oldpoint.y/ro;
    lam = lam/ro;

    nnn = (axis_X - axis_Y)/(axis_X + axis_Y);
    a2 = 2*(nnn - nnn*nnn/3 - 2*nnn*nnn*nnn/3);
    a4 = 5*nnn*nnn/3-16*nnn*nnn*nnn/15;
    a6 = 26*nnn*nnn*nnn/15;
    fi1=fi0 - a2*sin(2*fi0) + a4*sin(4*fi0) - a6*sin(6*fi0);

    t = cos(fi1)*sin(lam);

    ks=atan(tan(fi1)/cos(lam));

    et=log((1+t)/(1-t))/2;

    RRR = axis_X * (1 + nnn*nnn/4 + nnn*nnn*nnn*nnn/64) / (1+nnn);

    b2 = nnn/2 - 2*nnn*nnn/3 + 5*nnn*nnn*nnn/16;
    b4 = 13*nnn*nnn/48 - 3*nnn*nnn*nnn/5;
    b6 = 61*nnn*nnn*nnn/240;

    newpoint.y = RRR * (ks + b2*sin(2*ks)*cosh(2*et) + b4*sin(4*ks)*cosh(4*et) + b6*sin(6*ks)*cosh(6*et));
    newpoint.x = RRR * (et + b2*cos(2*ks)*sinh(2*et) + b4*cos(4*ks)*sinh(4*et) + b6*cos(6*ks)*sinh(6*et));


    //consider transfer of the beginning of coordinates
    newpoint.x += m_falseeasting;
    newpoint.y += m_falsenorthing;

    //consider a reference point
    newpoint.x-=m_reper.x;
    newpoint.y-=m_reper.y;

    return true;
}

//===============================================================================================
//!calculation of an azimuth and range between points
void MExtendedGaussKrugerCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance)
//===============================================================================================
{
    //NB to subtract a reference point there is no need
    azimut  = atan2(pt2.x - pt1.x, pt2.y - pt1.y);
    azimut = 360.0*azimut/(2*pi);
    distance = sqrt(((pt1.x - pt2.x)*(pt1.x - pt2.x)) + ((pt1.y - pt2.y)*(pt1.y - pt2.y)));
}

//======================================================================
//!the function filling two arrays of lines with names of parameters
//! and their values
bool MExtendedGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    //call the parent
    if(!MCoo::GetCooParameters(parnames, pardata))
        return false;

    MString str;
    str = IDS_CNTR_MERIDIAN; //"Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.2f", m_meridian);
    pardata->AddTail(&str);
    str = IDS_RETRN_CNTR_MERIDIAN;//"East Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falseeasting);
    pardata->AddTail(&str);
    str = IDS_NORTH_CNTR_MERIDIAN;//"Notrh Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falsenorthing);
    pardata->AddTail(&str);
    str = IDS_X_REFERENCE_POINT;//"X Reference Point"
    parnames->AddTail(&str);
    str.Format("%.2f", m_reper.y);
    pardata->AddTail(&str);
    str = IDS_Y_REFERENCE_POINT;//"Y Reference Point"
    parnames->AddTail(&str);
    str.Format("%.2f", m_reper.x);
    pardata->AddTail(&str);
    return true;
}

//==========================================================================
//! function establishing the parameters taken from arrays
bool MExtendedGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
//==========================================================================
{
    if(!MCoo::SetCooParameters(parnames, pardata))
        return false;

    //run on all array of names and take all parameters
    MString str;
    double meridian = m_meridian;
    double falseeasting = m_falseeasting;
    double falsenorthing = m_falsenorthing;

    MPoint reper = m_reper;
    for(int i=parnames->GetSize()-1; i>-1; i--)
    {
        str = *parnames->GetAt(i);
        if(str==IDS_CNTR_MERIDIAN)//"Central Meridian"//
        {
            meridian = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_RETRN_CNTR_MERIDIAN)//"East Location of Central Meridian"
        {
            falseeasting = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_NORTH_CNTR_MERIDIAN)//"Notrh Location of Central Meridian"
        {
            falsenorthing = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_X_REFERENCE_POINT)//"X Reference Point"
        {
            reper.y = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_Y_REFERENCE_POINT)//"Y Reference Point"
        {
            reper.x = StrToDouble(*pardata->GetAt(i));
        }
    }

    //set parameters
    return SetGaussParameters(meridian, falseeasting, falsenorthing, reper);
}

//==============================================================================================
//!number of bytes in the UTP format
int MExtendedGaussKrugerCoo::GetBytes()
//==============================================================================================
{

    //except the parent's variables are added here:
    //1. m_meridian(8б)
    //2. m_falseeasting(8б)
    //3. m_falsenorthing(8б)
    //4. m_reper(16б)

    return MCoo::GetBytes()+3*sizeof(double)+sizeof(MPoint);
}

//!==============================================================================================
bool MExtendedGaussKrugerCoo::WriteBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //at first we write down that, as in the parent
    if(MCoo::WriteBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fwrite(&m_meridian, sizeof(double), 1, h_nuxfile);
            //shift to the west
            fwrite(&m_falseeasting, sizeof(double), 1, h_nuxfile);
            //shift to the south
            fwrite(&m_falsenorthing, sizeof(double), 1, h_nuxfile);
            //reference point
            WritePoint(h_nuxfile,  version, &m_reper);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from UTP
bool MExtendedGaussKrugerCoo::ReadBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //at first we write down that, as in the parent
    if(MCoo::ReadBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fread(&m_meridian, sizeof(m_meridian), 1, h_nuxfile);
            //shift to the west
            fread(&m_falseeasting, sizeof(m_falseeasting), 1, h_nuxfile);
            //shift to the south
            fread(&m_falsenorthing, sizeof(m_falsenorthing), 1, h_nuxfile);
            if (reverseReadFlag) {
                reverseValue((char*)&m_meridian,      sizeof(m_meridian) );
                reverseValue((char*)&m_falseeasting,  sizeof(m_falseeasting) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
            }
            //reference point
            ReadPoint(h_nuxfile,  version, &m_reper);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from UTP
bool MExtendedGaussKrugerCoo::ReadBin(MFile* file,   int version)
//==============================================================================================
{
    //at first we write down that, as in the parent
    if(MCoo::ReadBin(file,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            file->GetData(&m_meridian, sizeof(m_meridian), 1);
            //shift to the west
            file->GetData(&m_falseeasting, sizeof(m_falseeasting), 1);
            //shift to the south
            file->GetData(&m_falsenorthing, sizeof(m_falsenorthing), 1);
            if (reverseReadFlag) {
                reverseValue((char*)&m_meridian,      sizeof(m_meridian) );
                reverseValue((char*)&m_falseeasting,  sizeof(m_falseeasting) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
            }
            //reference point
            ReadPoint(file,  version, &m_reper);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!copy
bool MExtendedGaussKrugerCoo::Copy(MCoo* pCoo)
//==============================================================================================
{
    if(MCoo::Copy(pCoo))
    {
        MExtendedGaussKrugerCoo* pGCoo = (MExtendedGaussKrugerCoo*)pCoo;
        return SetGaussParameters(pGCoo->m_meridian, pGCoo->m_falseeasting, pGCoo->m_falsenorthing, pGCoo->m_reper);
    }
    else
    {
        return false;
    }
}


//==============================================================================================
//!comparing
bool MExtendedGaussKrugerCoo::IsEqualTo(MCoo* pCoo)

//==============================================================================================
{
    //if at the parent everything coincides
    if(!MCoo::IsEqualTo(pCoo))
        return false;

    MExtendedGaussKrugerCoo* pGCoo = (MExtendedGaussKrugerCoo*)pCoo;

    //check term by term
    return ((m_falseeasting==pGCoo->m_falseeasting)&&
            (m_falsenorthing==pGCoo->m_falsenorthing)&&
            (m_meridian==pGCoo->m_meridian)&&
            (m_reper==pGCoo->m_reper));

}


//=========================================
//!formation of a text type of coordinates
MString MExtendedGaussKrugerCoo::CreateText(MPoint point)
//=========================================
{
    MString result;
    result.Format("X:  %.3f ; Y:  %.3f", point.y, point.x);
    return result;
}

//************************************************************************
//MZoneGaussKrugerCoo
//************************************************************************

//!==============================================================================================
MZoneGaussKrugerCoo::MZoneGaussKrugerCoo()
//==============================================================================================
    :MExtendedGaussKrugerCoo()
{
    //establish characteristics of the first zone
    SetGaussParameters(3.0, 500000.0, 0.0, MPoint(0,0));
}

//==============================================================================================
//!установка параметров ГК
bool MZoneGaussKrugerCoo::SetGaussParameters(double meridian, double falseeasting, double falsenorthing, MPoint reper)
//==============================================================================================
{
    //establish basic characteristics
    if(!SetBasicParameters(meridian, falseeasting, falsenorthing))
        return false;

    //establish reference point
    return SetReper(reper);
}

//==============================================================================================
//!setting of basic parameters of Gauss-Kryuger
bool MZoneGaussKrugerCoo::SetBasicParameters(double meridian, double falseeasting, double falsenorthing)
//==============================================================================================
{
    //the meridian will be brought to that central meridian of a zone
    //in which there is a required meridian
    //transfer of system of coordinates will be established in 500000 and 0

    //it is impossible to establish a meridian which is less than-180 or more than 180 degrees,
    //therefore at first we will normalize value of a meridian
    while(meridian<-180.0)
        meridian+=360.0;
    while(meridian>180)
        meridian-=360.0;

    //round a meridian
    //meridian = 6.0*floor(meridian/6.0)+3.0; //maax

    //establish shift of the beginning of coordinates
    falseeasting = 500000.0;
    falsenorthing = 0.0;
    return MExtendedGaussKrugerCoo::SetBasicParameters(meridian, falseeasting, falsenorthing);
}

//=============================================================================
//!establish reference point
bool MZoneGaussKrugerCoo::SetReper(MPoint ) //(MPoint reper)
//=============================================================================
{
    //we can't establish any reference point except the zero
    return MExtendedGaussKrugerCoo::SetReper(MPoint(0,0));
}

//==============================================================================================
//!setting of the central meridian proceeding from number of a zone
bool MZoneGaussKrugerCoo::SetZone(int zonenumber)
//==============================================================================================
{
    //number of a zone can be in the range from 1 to 60
    //if number of a zone is incorrect, returns false
    if((zonenumber<1)||(zonenumber>60))
        return false;

    //define the central meridian
    int mer = zonenumber*6-3;

    //establish the central meridian
    SetBasicParameters((double)mer, m_falseeasting, m_falsenorthing);

    return true;
}

//==============================================================================================
//!establish number of a zone on the central meridian
int  MZoneGaussKrugerCoo::GetZone()
//==============================================================================================
{
    //normalize the central meridian in value(0:360)
    double mer;
    if(m_meridian<0)
        mer = m_meridian+360.0;
    else
        mer = m_meridian;

    //calculate number of a zone
    return (int)floor(mer/6.0)+1;
}

//==============================================================================================
//!transformation to geodetic coordinates
bool MZoneGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//==============================================================================================
{
    //remove number of a zone from coordinate
    int zone = GetZone();
    double dobavka = (double)(zone*1000000);
    oldpoint.x-=dobavka;


    //remove the southern hemisphere
    // it isn't necessary during the work on the map of the whole world of it
    /*
    if(oldpoint.y>=10000000.0)
    {
        oldpoint.y-=10000000.0;
    }*/

    //execute parental function
    return MExtendedGaussKrugerCoo::ToBLH(oldpoint, oldh, newpoint, newh);
}

//!==============================================================================================
bool MZoneGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//==============================================================================================
{
    //execute parental function
    if(!MExtendedGaussKrugerCoo::FromBLH(oldpoint, oldh, newpoint, newh))
        return false;

    //add number of a zone and a hemisphere
    int zone = GetZone();
    double dobavka = (double)(zone*1000000);
    newpoint.x+=dobavka;

    //consider the southern hemisphere
    // it isn't necessary during the work on the map of the whole world of it
    /*
    if(newpoint.y<0)
        newpoint.y+=10000000.0;
    */

    return true;
}

//======================================================================
//!function filling two arrays of lines with names of parameters
//! and their values
bool MZoneGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    //call the parent
    if(!MCoo::GetCooParameters(parnames, pardata))
        return false;

    MString str;
    str = IDS_CNTR_MERIDIAN;//"Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.2f", m_meridian);
    pardata->AddTail(&str);

    return true;
}

//======================================================================
//!function filling two arrays of lines with names of parameters
//! and their values
bool MZoneGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    //call the parent
    return MExtendedGaussKrugerCoo::SetCooParameters(parnames, pardata);
}


//**********************************************************************
//MStandartGaussKrugerCoo
//**********************************************************************

//!=============================================================================
MStandartGaussKrugerCoo::MStandartGaussKrugerCoo()
//=============================================================================
    :MZoneGaussKrugerCoo()
{

}

//=============================================================================
//!transfer to geodetic systems of coordinates
bool MStandartGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//=============================================================================
{
    //isolate number of a zone
    double x = oldpoint.x;
    int zone = (int)floor(x/1000000.0);
    if(!SetZone(zone))
        return false;

    //call the parent
    return MZoneGaussKrugerCoo::ToBLH(oldpoint, oldh, newpoint, newh);
}

//=============================================================================
//!transfer from geodetic system to our
bool MStandartGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//=============================================================================
{
    //establish the central meridian
    if(!SetBasicParameters((int)((oldpoint.x - 3)/6 + 1.5)*6-3))//maax
        return false;

    //call transformation of coordinates of the parent
    return MZoneGaussKrugerCoo::FromBLH(oldpoint, oldh, newpoint, newh);
}

//==============================================================================================
//!comparing
bool MStandartGaussKrugerCoo::IsEqualTo(MCoo* pCoo)
//==============================================================================================
{
    //as in this system doesn't matter
    //anything from gauss, we will cause the MCoo function
    return MCoo::IsEqualTo(pCoo);
}


//======================================================================
//!function filling two arrays of lines with names of parameters
//! and their values
bool MStandartGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    //completely all that at the parent
    return MCoo::GetCooParameters(parnames, pardata);
}

//==========================================================================
//!the function establishing the parameters taken from arrays
bool MStandartGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
//==========================================================================
{
    return MCoo::SetCooParameters(parnames, pardata);
}


//**********************************************************************
//MPsevdoGaussKrugerCoo
//**********************************************************************


//=============================================================================
//!Constructor by default
MPsevdoGaussKrugerCoo::MPsevdoGaussKrugerCoo() 
//=============================================================================
    :MExtendedGaussKrugerCoo()
{
    //make system inappropriate by default
    SetLink(MPoint(0,0), MPoint(0,0), MPoint(0,0), MPoint(0,0), false);
}

//=============================================================================
//!establishes compliance between the coordinates and coordinates GK
bool MPsevdoGaussKrugerCoo::SetLink(MPoint gauss_pt1, MPoint personal_pt1, MPoint gauss_pt2, MPoint personal_pt2, bool havelink)
//=============================================================================
{
    m_gauss_pt1 = gauss_pt1;
    m_gauss_pt2 = gauss_pt2;
    m_personal_pt1 = personal_pt1;
    m_personal_pt2 = personal_pt2;
    SetLink(havelink);
    return true;
}
//=============================================================================
//!sets a flag of connectivity of own coordinates with coordinates of GK
bool MPsevdoGaussKrugerCoo::SetLink(bool havelink)
//=============================================================================
{
    m_havelink = havelink;
    return true;
}

//=============================================================================
//!transition to geodetic coordinates
bool MPsevdoGaussKrugerCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//=============================================================================
{
    //if communication with Gauss isn't present, we can't find geodetic coordinates
    if(m_havelink)
    {
        //receive coordinates in expanded Gauss
        double dx = m_personal_pt1.x-m_gauss_pt1.x;
        double dy = m_personal_pt1.y-m_gauss_pt1.y;

        //find an angle of rotation
        double ag = GetVektorGrad(&m_gauss_pt1, &m_gauss_pt2);
        double ap = GetVektorGrad(&m_personal_pt1, &m_personal_pt2);
        double da = ag - ap;
        while(da<0.0)
            da+=360.0;
        while(da>=360.0)
            da-=360.0;
        da = pi*da/180.0;
        if (da==0)
            da=pi*m_rotation/180;
        //execute parallel translation with turn
        SetKompozParameters(dx, dy, 1, 1, da);
        KompozLinPreobr(&oldpoint);

        //call parent's function
        return MExtendedGaussKrugerCoo::ToBLH(oldpoint, oldh, newpoint, newh);
    }

    return false;
}

//=============================================================================
//!transformation from geodetic coordinates
bool MPsevdoGaussKrugerCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//=============================================================================
{
    //if communication with Gauss isn't present, we can't find geodetic coordinates
    if(m_havelink)
    {
        //call parent's function
        if(!MExtendedGaussKrugerCoo::FromBLH(oldpoint, oldh, newpoint, newh))
            return false;

        //receive coordinates in expanded Gauss
        double dx = m_gauss_pt1.x-m_personal_pt1.x;
        double dy = m_gauss_pt1.y-m_personal_pt1.y;

        //find an angle of rotation
        double ag = GetVektorGrad(&m_gauss_pt1, &m_gauss_pt2);
        double ap = GetVektorGrad(&m_personal_pt1, &m_personal_pt2);
        double da = ap - ag;
        while(da<0.0)
            da+=360.0;
        while(da>=360.0)
            da-=360.0;
        da = pi*da/180.0;
        if (da==0)
            da=pi*m_rotation/180;
        //execute parallel translation with turn
        SetKompozParameters(dx, dy, 1, 1, da);
        KompozLinPreobr(&newpoint);
    }

    return false;
}

//======================================================================
//!function filling two arrays of lines with names of parameters
//! and their values
bool MPsevdoGaussKrugerCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    //call the parent
    if(!MExtendedGaussKrugerCoo::GetCooParameters(parnames, pardata))
        return false;

    MString str;
    str = IDS_CONFORMITY_FLAG;//"Conformity Indicator(Flag)";
    parnames->AddTail(&str);
    str = (m_havelink)? "+":"-";
    //str.Format("%.2f", m_meridian);
    pardata->AddTail(&str);

    str = IDS_X1GK;//"X1 to GK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_gauss_pt1.y);
    pardata->AddTail(&str);
    str = IDS_Y1GK;//"Y1 to GK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_gauss_pt1.x);
    pardata->AddTail(&str);

    str = IDS_X2GK;//"X2 to GK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_gauss_pt2.y);
    pardata->AddTail(&str);
    str = IDS_Y2GK;//"Y2 to GK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_gauss_pt2.x);
    pardata->AddTail(&str);

    str = IDS_X1MGK;//"X1 to LGK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_personal_pt1.y);
    pardata->AddTail(&str);

    str = IDS_Y1MGK;//"Y1 to LGK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_personal_pt1.x);
    pardata->AddTail(&str);

    str = IDS_X2MGK;//"X2 to LGK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_personal_pt2.y);
    pardata->AddTail(&str);
    str = IDS_Y2MGK;//"Y2 to LGK"
    parnames->AddTail(&str);
    str.Format("%.2f", m_personal_pt2.x);
    pardata->AddTail(&str);

    return true;
}

//==========================================================================
//!function establishing the parameters taken from arrays
bool MPsevdoGaussKrugerCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
//==========================================================================
{
    if(!MExtendedGaussKrugerCoo::SetCooParameters(parnames, pardata))
        return false;

    //run on all array of names and we will take all parameters
    MString str;
    MPoint gauss_pt1 = m_gauss_pt1;
    MPoint gauss_pt2 = m_gauss_pt2;
    MPoint personal_pt1 = m_personal_pt1;
    MPoint personal_pt2 = m_personal_pt2;
    bool havelink = m_havelink;

    for(int i=parnames->GetSize()-1; i>-1; i--)
    {
        str = *parnames->GetAt(i);
        if(str=="флаг соответствия")//IDS_CONFORMITY_FLAG
        {
            havelink = (*pardata->GetAt(i)=="+")? true:false;
        }
        else if(str=="Х1 в ГК")//IDS_X1GK
        {
            gauss_pt1.y = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="У1 в ГК")//IDS_Y1GK
        {
            gauss_pt1.x = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="Х2 в ГК")//IDS_X2GK
        {
            gauss_pt2.y = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="У2 в ГК")//IDS_Y2GK
        {
            gauss_pt2.x = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="Х1 в МГК")//IDS_X1MGK
        {
            personal_pt1.y = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="У1 в МГК")//IDS_Y1MGK
        {
            personal_pt1.x = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="Х2 в МГК")//IDS_X2MGK
        {
            personal_pt2.y = StrToDouble(*pardata->GetAt(i));
        }
        else if(str=="У2 в МГК")//IDS_Y2MGK
        {
            personal_pt2.x = StrToDouble(*pardata->GetAt(i));
        }

    }

    return SetLink(gauss_pt1, personal_pt1, gauss_pt2, personal_pt2, havelink);

}

//=============================================================================
//!returns number of bytes in the UTP format
int MPsevdoGaussKrugerCoo::GetBytes()
//=============================================================================
{
    //besides that is available in the parent,
    //this class contains communication
    //these are 4 points on 16b + a flag 1b

    return MExtendedGaussKrugerCoo::GetBytes()+4*sizeof(MPoint)+1;
}
//!=============================================================================
bool MPsevdoGaussKrugerCoo::WriteBin(FILE* h_nuxfile,   int version)
//=============================================================================
{
    //write down that, as in the parent
    if(MExtendedGaussKrugerCoo::WriteBin(h_nuxfile,  version))
    {
        unsigned char byte;
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //communication flag
            byte = (m_havelink)? 1:0;
            fwrite(&byte, 1, 1, h_nuxfile);
            //compliance points
            WritePoint(h_nuxfile,  version, &m_gauss_pt1);
            WritePoint(h_nuxfile,  version, &m_gauss_pt2);
            WritePoint(h_nuxfile,  version, &m_personal_pt1);
            WritePoint(h_nuxfile,  version, &m_personal_pt2);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//=============================================================================
//!reading from UTP
bool MPsevdoGaussKrugerCoo::ReadBin(FILE* h_nuxfile,   int version)
//=============================================================================
{
    //read down that, as in the parent
    if(MExtendedGaussKrugerCoo::ReadBin(h_nuxfile,  version))
    {
        unsigned char byte;
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //communication flag
            fread(&byte, 1, 1, h_nuxfile);
            bool havelink = (byte==1)? true:false;
            //compliance points
            MPoint gpt1, gpt2, ppt1, ppt2;
            ReadPoint(h_nuxfile,  version, &gpt1);
            ReadPoint(h_nuxfile,  version, &gpt2);
            ReadPoint(h_nuxfile,  version, &ppt1);
            ReadPoint(h_nuxfile,  version, &ppt2);
            return SetLink(gpt1, ppt1, gpt2, ppt2, havelink);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}	

//=============================================================================
//!readinf from UTP
bool MPsevdoGaussKrugerCoo::ReadBin(MFile* file,   int version)
//=============================================================================
{
    //read down that, as in the parent
    if(MExtendedGaussKrugerCoo::ReadBin(file,  version))
    {
        unsigned char byte;
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //communication flag
            file->GetData(&byte, 1, 1);
            bool havelink = (byte==1)? true:false;
            //compliance points
            MPoint gpt1, gpt2, ppt1, ppt2;
            ReadPoint(file,  version, &gpt1);
            ReadPoint(file,  version, &gpt2);
            ReadPoint(file,  version, &ppt1);
            ReadPoint(file,  version, &ppt2);
            return SetLink(gpt1, ppt1, gpt2, ppt2, havelink);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//=============================================================================
//!coping
bool MPsevdoGaussKrugerCoo::Copy(MCoo* pCoo)
//=============================================================================
{
    if(MExtendedGaussKrugerCoo::Copy(pCoo))
    {
        MPsevdoGaussKrugerCoo* pGCoo = (MPsevdoGaussKrugerCoo*)pCoo;
        return SetLink(pGCoo->m_gauss_pt1, pGCoo->m_personal_pt1, pGCoo->m_gauss_pt2, pGCoo->m_personal_pt2, pGCoo->m_havelink);
    }
    else
    {
        return false;
    }
}



//==============================================================================================
//!comparing
bool MPsevdoGaussKrugerCoo::IsEqualTo(MCoo* pCoo)
//==============================================================================================
{
    //if at the parent everything coincides
    if(!MExtendedGaussKrugerCoo::IsEqualTo(pCoo))
        return false;

    MPsevdoGaussKrugerCoo* pGCoo = (MPsevdoGaussKrugerCoo*)pCoo;

    //check term by term
    return ((m_gauss_pt1==pGCoo->m_gauss_pt1)&&
            (m_gauss_pt2==pGCoo->m_gauss_pt2)&&
            (m_personal_pt1==pGCoo->m_personal_pt1)&&
            (m_personal_pt2==pGCoo->m_personal_pt2)&&
            (m_havelink==pGCoo->m_havelink));
}

void MPsevdoGaussKrugerCoo::SetRotation(double gamma)
{
    m_rotation = gamma;
}



//***************************************************************************
//MTransverseMercatorCoo
//***************************************************************************

//==============================================================================================
//!Constructor by default
MTransverseMercatorCoo::MTransverseMercatorCoo()
//==============================================================================================
    :MCoo()
{

    SetBasicParameters(0.0);

}


//!==============================================================================================
bool MTransverseMercatorCoo::SetBasicParameters(double meridian, double latitudeOfTrueScale,double scaleFactor,double falseeasting, double falsenorthing)
//==============================================================================================
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


//==============================================================================================
//! revised 2005.01 
bool MTransverseMercatorCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
//==============================================================================================
{
    double tn;         /* True Meridianal distance constant  */
    double tn2;
    double tn3;
    double tn4;
    double tn5;
    
    double semiMajorAxis = this->greataxis();
    double flattening = this->pressfactor();
    double TranMerc_Origin_Lat = m_latitudeOfTrueScale*pi/180;
    if (m_meridian > 180)
        m_meridian -= (2*180);
    double TranMerc_Origin_Long = m_meridian*pi/180;
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


    double easting = oldpoint.x;
    double northing = oldpoint.y;

    if ((easting < (TranMerc_False_Easting - TranMerc_Delta_Easting))
            ||(easting > (TranMerc_False_Easting + TranMerc_Delta_Easting)))
    { /* easting out of range  */
        InternalMessage("easting out of range");
    }
    if ((northing < (TranMerc_False_Northing - TranMerc_Delta_Northing))
            || (northing > (TranMerc_False_Northing + TranMerc_Delta_Northing)))
    { /* northing out of range */
        InternalMessage("northing out of range");
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

    if((longitude) > (pi))
    {
        longitude -= (2 * pi);
    }
    else if((longitude) < (-pi))
    {
        longitude += (2 * pi);

    }

    if (fabs(dlam) > (9.0 * pi / 180) * cos(latitude))
    { /* Distortion will result if longitude is more than 9 degrees from the Central Meridian at the equator */
        /* and decreases to 0 degrees at the poles */
        /* As you move towards the poles, distortion will become more significant */
        InternalMessage("TransvrseMercator: Warning: big distorsion");
    }
    newpoint.x = longitude*180/pi;
    newpoint.y = latitude*180/pi;
    return true;
}

//==============================================================================================
//! revised 2005.01 by vipa)
//parameters (MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
bool MTransverseMercatorCoo::FromBLH(MPoint oldpoint, double , MPoint& newpoint, double& )
{

    double tn;         /* True Meridianal distance constant  */
    double tn2;
    double tn3;
    double tn4;
    double tn5;
    
    double semiMajorAxis = this->pressfactor();
    double flattening = this->pressfactor();
    double TranMerc_Origin_Lat = m_latitudeOfTrueScale*pi/180;
    if (m_meridian > 180)
        m_meridian -= (2*180);
    double TranMerc_Origin_Long = m_meridian*pi/180;
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

    double longitude = oldpoint.x*pi/180;
    double latitude = oldpoint.y*pi/180;

    if ((latitude < -((pi * 89.99)/180.0)) || (latitude > ((pi * 89.99)/180.0)))
    {  /* Latitude out of range */
        InternalMessage("Latitude out of range");
    }
    if (longitude > pi)
        longitude -= (2 * pi);
    if ((longitude < (TranMerc_Origin_Long - ((pi * 90)/180.0)))
            || (longitude > (TranMerc_Origin_Long + ((pi * 90)/180.0))))
    {
        if (longitude < 0)
            temp_Long = longitude + 2 * pi;
        else
            temp_Long = longitude;
        if (TranMerc_Origin_Long < 0)
            temp_Origin = TranMerc_Origin_Long + 2 * pi;
        else
            temp_Origin = TranMerc_Origin_Long;
        if ((temp_Long < (temp_Origin - ((pi * 90)/180.0)))
                || (temp_Long > (temp_Origin + ((pi * 90)/180.0))))
            InternalMessage("Longitude out of range");;
    }



    /*
   *  Delta Longitude
   */
    dlam = longitude - TranMerc_Origin_Long;

    if (fabs(dlam) > (9.0 * pi / 180))
    { /* Distortion will result if Longitude is more than 9 degrees from the Central Meridian */
        InternalMessage("Distortion will result if Longitude is more than 9 degrees from the Central Meridian");
    }

    if (dlam > pi)
        dlam -= (2 * pi);
    if (dlam < -pi)
        dlam += (2 * pi);
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

    newpoint.y = TranMerc_False_Northing + t1 + pow(dlam, 2.e0) * t2
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

    newpoint.x = TranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7
            + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;


    return true;
}

//===============================================================================================
//!calculation of an azimuth and range between points
void MTransverseMercatorCoo::FindAzimutAndDistance(MPoint pt1, MPoint pt2, double& azimut, double& distance)
//===============================================================================================
{
    //NB to subtract a reference point there is no need
    azimut  = atan2(pt2.x - pt1.x, pt2.y - pt1.y);
    azimut = 360.0*azimut/(2*pi);
    distance = sqrt(((pt1.x - pt2.x)*(pt1.x - pt2.x)) + ((pt1.y - pt2.y)*(pt1.y - pt2.y)));
}

//======================================================================
//!function fills two arrays of lines with names of parameters
//! and their values
bool MTransverseMercatorCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
//======================================================================
{
    //call a parent
    if(!MCoo::GetCooParameters(parnames, pardata))
        return false;

    MString str;
    str = IDS_CNTR_MERIDIAN; //"Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.2f", m_meridian);
    pardata->AddTail(&str);
    str = IDS_RETRN_CNTR_MERIDIAN;//"East Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falseeasting);
    pardata->AddTail(&str);
    str = IDS_NORTH_CNTR_MERIDIAN;//"Notrh Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falsenorthing);
    pardata->AddTail(&str);
    return true;
}

//==========================================================================
//!function establishing the parameters taken from arrays
bool MTransverseMercatorCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
//==========================================================================
{
    if(!MCoo::SetCooParameters(parnames, pardata))
        return false;

    //run on all array of names and we will take all parameters
    MString str;
    double meridian = m_meridian;
    double falseeasting = m_falseeasting;
    double falsenorthing = m_falsenorthing;
    double latitudeOfTrueScale = m_latitudeOfTrueScale;
    double scaleFactor = m_scaleFactor;


    for(int i=parnames->GetSize()-1; i>-1; i--)
    {
        str = *parnames->GetAt(i);
        if(str==IDS_CNTR_MERIDIAN)//"Central Meridian"//
        {
            meridian = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_TM_LAT_TRUE_SCL)
        {
            latitudeOfTrueScale = StrToDouble(*pardata->GetAt(i));
        }
        else if (str==IDS_TM_SCALE)
        {
            scaleFactor = StrToDouble(*pardata->GetAt(i));
        }

        else if(str==IDS_RETRN_CNTR_MERIDIAN)//"East Location of Central Meridian"
        {
            falseeasting = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_NORTH_CNTR_MERIDIAN)//"Notrh Location of Central Meridian"
        {
            falsenorthing = StrToDouble(*pardata->GetAt(i));
        }

    }

    //set parameters
    return SetBasicParameters(meridian,latitudeOfTrueScale,scaleFactor,falseeasting,falsenorthing);
}

//==============================================================================================
//!number of bytes in the UTP format
int MTransverseMercatorCoo::GetBytes()
//==============================================================================================
{

    //except the parent's variables are added here:
    //1. m_meridian(8б)
    //2. m_latitudeOfTrueScale (8б)
    //3. m_scaleFactor (8б)
    //4. m_falseeasting(8б)
    //5. m_falsenorthing(8б)

    return MCoo::GetBytes()+5*sizeof(double);
}

//!==============================================================================================
bool MTransverseMercatorCoo::WriteBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //write down that, as in the parent
    if(MCoo::WriteBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fwrite(&m_meridian, sizeof(double), 1, h_nuxfile);
            //
            fwrite(&m_latitudeOfTrueScale, sizeof(double), 1, h_nuxfile);
            //
            fwrite(&m_scaleFactor, sizeof(double), 1, h_nuxfile);
            //shift to the west
            fwrite(&m_falseeasting, sizeof(double), 1, h_nuxfile);
            //shift to the south
            fwrite(&m_falsenorthing, sizeof(double), 1, h_nuxfile);

        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from UTP
bool MTransverseMercatorCoo::ReadBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //write down that, as in the parent
    if(MCoo::ReadBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fread(&m_meridian, sizeof(m_meridian), 1, h_nuxfile);
            //
            fread(&m_latitudeOfTrueScale, sizeof(m_latitudeOfTrueScale), 1, h_nuxfile);
            //
            fread(&m_scaleFactor, sizeof(m_scaleFactor), 1, h_nuxfile);
            //shift to the west
            fread(&m_falseeasting, sizeof(m_falseeasting), 1, h_nuxfile);
            //shift to the south
            fread(&m_falsenorthing, sizeof(m_falsenorthing), 1, h_nuxfile);
            if (reverseReadFlag) {
                reverseValue((char*)&m_meridian,      sizeof(m_meridian) );
                reverseValue((char*)&m_latitudeOfTrueScale,      sizeof(m_latitudeOfTrueScale) );
                reverseValue((char*)&m_scaleFactor,      sizeof(m_scaleFactor) );
                reverseValue((char*)&m_falseeasting,  sizeof(m_falseeasting) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
            }

        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from UTP
bool MTransverseMercatorCoo::ReadBin(MFile* file,   int version)
//==============================================================================================
{
    //write down that, as in the parent
    if(MCoo::ReadBin(file,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            file->GetData(&m_meridian, sizeof(m_meridian), 1);
            //
            file->GetData(&m_latitudeOfTrueScale, sizeof(m_latitudeOfTrueScale), 1);
            //
            file->GetData(&m_scaleFactor, sizeof(m_scaleFactor), 1);
            //shift to the west
            file->GetData(&m_falseeasting, sizeof(m_falseeasting), 1);
            //shift to the south
            file->GetData(&m_falsenorthing, sizeof(m_falsenorthing), 1);
            if (reverseReadFlag) {
                reverseValue((char*)&m_meridian,      sizeof(m_meridian) );
                reverseValue((char*)&m_falseeasting,  sizeof(m_falseeasting) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
            }

        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!copy
bool MTransverseMercatorCoo::Copy(MCoo* pCoo)
//==============================================================================================
{
    if(MCoo::Copy(pCoo))
    {
        MTransverseMercatorCoo* pGCoo = (MTransverseMercatorCoo*)pCoo;
        return SetBasicParameters(pGCoo->m_meridian,pGCoo->m_latitudeOfTrueScale,pGCoo->m_scaleFactor,pGCoo->m_falseeasting, pGCoo->m_falsenorthing);
    }
    else
    {
        return false;
    }
}


//==============================================================================================
//!comparing
bool MTransverseMercatorCoo::IsEqualTo(MCoo* pCoo)

//==============================================================================================
{
    //if at the parent everything coincides
    if(!MCoo::IsEqualTo(pCoo))
        return false;

    MTransverseMercatorCoo* pTMCoo = (MTransverseMercatorCoo*)pCoo;

    //check term by term
    return ((m_falseeasting==pTMCoo->m_falseeasting)&&
            (m_falsenorthing==pTMCoo->m_falsenorthing)&&
            (m_meridian==pTMCoo->m_meridian)&&
            (m_latitudeOfTrueScale == pTMCoo->m_latitudeOfTrueScale)&&
            (m_scaleFactor == pTMCoo->m_scaleFactor));


}


//=========================================
//!formation of a text type of coordinates
MString MTransverseMercatorCoo::CreateText(MPoint point)
//=========================================
{
    MString result;
    result.Format("X:  %.3f ; Y:  %.3f", point.y, point.x);
    return result;
}

double MTransverseMercatorCoo::sphtmd( double latitude )
{
    return TranMerc_ap * latitude
            - TranMerc_bp * sin(2.e0 * latitude) + TranMerc_cp * sin(4.e0 * latitude)
            - TranMerc_dp * sin(6.e0 * latitude) + TranMerc_ep * sin(8.e0 * latitude);
}


double MTransverseMercatorCoo::sphsn( double  latitude ) 
{
    return this->greataxis() / sqrt( 1.e0 - TranMerc_es * pow(sin(latitude), 2.0));
}


double MTransverseMercatorCoo::sphsr( double latitude ) 
{
    double denom = sqrt(1.e0 - TranMerc_es * pow(sin(latitude), 2.0));
    return this->greataxis() * (1.e0 - TranMerc_es) / pow(denom, 3.0);
}






int MUtmCoo::StandardZone(MPoint point) {

    double lat = point.y, lon = point.x;

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


bool MUtmCoo::FromBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
{
    if ((oldpoint.y < -80) || (oldpoint.y >84))
        return false;
    double nhgt;
    int zone = StandardZone(oldpoint);
    m_meridian = (6 * zone - 183);
    if(oldpoint.y < 0)
    {
        nPole = false;
        m_falsenorthing = 10000000;
    }
    bool res = MTransverseMercatorCoo::FromBLH(oldpoint,0.0, newpoint, nhgt);
    //
    newpoint.x += zone*1000000;
    return res;
}
bool MUtmCoo::ToBLH(MPoint oldpoint, double oldh, MPoint& newpoint, double& newh)
{
    int zone = (int)floor(oldpoint.x/1000000.0);
    oldpoint.x -= 1000000.0*(int)floor(oldpoint.x/1000000.0);
    m_meridian = (6 * zone - 183);
    if (!nPole) m_falsenorthing = 10000000;
    return MTransverseMercatorCoo::ToBLH(oldpoint, oldh, newpoint, newh);
}
//**********************************************************************
//MLambertConicCoo
//**********************************************************************
bool MLambertConicCoo::GetCooParameters(MStArray *parnames, MStArray *pardata)
{
    //call a parent
    if(!MCoo::GetCooParameters(parnames, pardata))
        return false;

    MString str;
    str = IDS_CNTR_MERIDIAN; //"Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.2f", m_meridian);
    pardata->AddTail(&str);
    str = IDS_RETRN_CNTR_MERIDIAN;//"East Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falseeasting);
    pardata->AddTail(&str);
    str = IDS_NORTH_CNTR_MERIDIAN;//"North Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falsenorthing);
    pardata->AddTail(&str);

    str = IDS_TM_LAT_TRUE_SCL;
    parnames->AddTail(&str);
    str.Format("%.2f", m_originLat);
    pardata->AddTail(&str);

    str = IDS_FIRST_STD_PARALLEL;
    parnames->AddTail(&str);
    str.Format("%.2f", m_frstStP);
    pardata->AddTail(&str);

    str = IDS_SECOND_STD_PARALLEL;
    parnames->AddTail(&str);
    str.Format("%.2f", m_scndStP);
    pardata->AddTail(&str);

    return true;
}
bool MLambertConicCoo::SetCooParameters(MStArray *parnames, MStArray *pardata)
{

    if(!MCoo::SetCooParameters(parnames, pardata))
        return false;

    //run on all array of names and we will take all parameters
    MString str;
    double meridian = m_meridian;
    double falseeasting = m_falseeasting;
    double falsenorthing = m_falsenorthing;

    double originLat = m_originLat;
    double scndStP = m_scndStP;
    double frstStP = m_frstStP;

    for(int i=parnames->GetSize()-1; i>-1; i--)
    {
        str = *parnames->GetAt(i);
        if(str==IDS_CNTR_MERIDIAN)//"Central Meridian"//
        {
            meridian = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_TM_LAT_TRUE_SCL)
        {
            originLat = StrToDouble(*pardata->GetAt(i));
        }
        else if (str==IDS_FIRST_STD_PARALLEL)
        {
            frstStP= StrToDouble(*pardata->GetAt(i));
        }
        else if (str==IDS_SECOND_STD_PARALLEL)
        {
            scndStP= StrToDouble(*pardata->GetAt(i));
        }

        else if(str==IDS_RETRN_CNTR_MERIDIAN)//"East Location of Central Meridian"
        {
            falseeasting = StrToDouble(*pardata->GetAt(i));
        }
        else if(str==IDS_NORTH_CNTR_MERIDIAN)//"North Location of Central Meridian"
        {
            falsenorthing = StrToDouble(*pardata->GetAt(i));
        }

    }

    //set parameters
    return SetBasicParameters(meridian, originLat, frstStP, scndStP, falseeasting, falsenorthing);

}
bool MLambertConicCoo::SetBasicParameters(double meridian, double originLat, double frstStP, double scndStP,double falseeasting, double falsenorthing)
//==============================================================================================
{
    //it is impossible to establish a meridian which is less than-180 or more than 180 degrees,
    //therefore at first we will normalize value of a meridian
    while(meridian<-180.0)
        meridian+=360.0;
    while(meridian>180)
        meridian-=360.0;

    m_meridian = meridian;
    m_originLat = originLat;
    m_frstStP = frstStP;
    m_scndStP = scndStP;
    m_falseeasting = falseeasting;
    m_falsenorthing = falsenorthing;

    return true;
}

MLambertConicCoo::MLambertConicCoo()
{
    ex = 0.081819199;
    SetStandartEllipsoid(ELLIPSOID_WGS_84);
    SetBasicParameters(0, 45, 40, 50);
}


bool MLambertConicCoo::FromBLH(MPoint oldpoint, double oldH , MPoint& newpoint, double& newH)
{
    double n, F, Ro,R, lmb;
    if (m_frstStP == m_scndStP)
        n=sin(CalcRad(m_frstStP));
    else
        n = log(CalcM(m_frstStP)/CalcM(m_scndStP))/log(CalcT(m_frstStP)/CalcT(m_scndStP));
    if(n*CalcRad(oldpoint.y)<=0)
        return false;

    F = CalcM(m_frstStP)/(n*pow(CalcT(m_frstStP),n));
    Ro = greataxis()*F*pow(CalcT(m_originLat),n);
    R = greataxis()*F*pow(CalcT(oldpoint.y),n);
    double dLng = CalcRad(oldpoint.x-m_meridian);
    if ((dLng>-pi)&&(dLng<=pi))
        lmb = dLng;
    else if (dLng>pi)
        lmb = dLng-2*pi;
    else if (dLng<=-pi)
        lmb = dLng+2*pi;
    newpoint.x=m_falseeasting+R*sin(n*lmb);
    newpoint.y=m_falsenorthing+Ro-R*cos(n*lmb);
    return true;
}

bool MLambertConicCoo::ToBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH)
{
    double n, F, dU, dV,R, Ro, t, th;
    if (m_frstStP == m_scndStP)
        n=sin(CalcRad(m_frstStP));
    else
        n = log(CalcM(m_frstStP)/CalcM(m_scndStP))/log(CalcT(m_frstStP)/CalcT(m_scndStP));
    F = CalcM(m_frstStP)/(n*pow(CalcT(m_frstStP),n));
    char sgn = n/fabs(n);
    dU= sgn*(oldpoint.x-m_falseeasting);
    dV= sgn*(oldpoint.y-m_falsenorthing);
    Ro = sgn*greataxis()*F*pow(CalcT(m_originLat),n);
    R = sgn*sqrt(pow(dU,2)+pow(Ro-dV,2));
    t= pow(R/(greataxis()*F),1/n);
    th = atan(dU/(sgn*(Ro-dV)));
    double dLng = th/n+CalcRad(m_meridian);
    if ((dLng>-pi)&&(dLng<=pi))
        newpoint.x = CalcDeg(dLng);
    else if (dLng>pi)
        newpoint.x = CalcDeg(dLng-2*pi);
    else if (dLng<=-pi)
        newpoint.x = CalcDeg(dLng+2*pi);
    double Q1 = 0.5*pi-2*atan(t);
    double Q2 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q1))/(1+ex*sin(Q1)),ex/2));
    double Q3 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q2))/(1+ex*sin(Q2)),ex/2));
    double Q4 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q3))/(1+ex*sin(Q3)),ex/2));
    double Q5 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q4))/(1+ex*sin(Q4)),ex/2));
    double Q6 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q5))/(1+ex*sin(Q5)),ex/2));
    double Q7 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q6))/(1+ex*sin(Q6)),ex/2));
    double Q8 = 0.5*pi-2*atan(t*pow((1-ex*sin(Q7))/(1+ex*sin(Q7)),ex/2));
    newpoint.y = CalcDeg(Q8);



    return true;

}

double MLambertConicCoo::CalcM(double fi)
{
    return cos(CalcRad(fi))/sqrtf(1-pow(ex,2)*pow(sin(CalcRad(fi)),2));
}

double MLambertConicCoo::CalcT(double fi)
{
    return tan(0.25*pi-CalcRad(fi)/2)/pow((1-ex*sin(CalcRad(fi)))/(1+ex*sin(CalcRad(fi))),ex/2);
}

double MLambertConicCoo::CalcDeg(double rad)
{
    while(rad<-2*pi)
        rad+=2*pi;
    while(rad>2*pi)
        rad-=2*pi;
    return rad*180/pi;
}

double MLambertConicCoo::CalcRad(double deg)
{
    if (fabs(deg) > 360)
    {
        int c = deg / 360;
        deg -= c * 360;
    }
    /*while (deg < -360.0)
        deg+=360;
    while (deg > 360)
        deg-=360;*/
    return deg*pi/180;
}

//==============================================================================================
//!number of bytes in the UTP format
int MLambertConicCoo::GetBytes()
//==============================================================================================
{

    //except the parent's variables are added here:
    //1. m_meridian(8б)
    //2. m_falseeasting(8б)
    //3. m_falsenorthing(8б)
    //4. m_originLat(8б)
    //5. m_frstStP(8б)
    //6. m_scndStP(8б)

    return MCoo::GetBytes()+6*sizeof(double);
}

//!==============================================================================================
bool MLambertConicCoo::WriteBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //write down that, as in the parent
    if(MCoo::WriteBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fwrite(&m_meridian, sizeof(double), 1, h_nuxfile);
            //shift to the west
            fwrite(&m_falseeasting, sizeof(double), 1, h_nuxfile);
            //shift to the south
            fwrite(&m_falsenorthing, sizeof(double), 1, h_nuxfile);
            //latitude of a zero reference point
            fwrite(&m_originLat, sizeof(double), 1, h_nuxfile);
            //first standard parallel
            fwrite(&m_frstStP, sizeof(double), 1, h_nuxfile);
            //second standard parallel
            fwrite(&m_scndStP, sizeof(double), 1, h_nuxfile);
        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from UTP
bool MLambertConicCoo::ReadBin(FILE* h_nuxfile,   int version)
//==============================================================================================
{
    //write down that, as in the parent
    if(MCoo::ReadBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {

        case 0:
        {
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!why fwrite is used here? this function should perform reading
            ////Central Meridian
            //fwrite(&m_meridian, sizeof(double), 1, h_nuxfile);
            ////shift to the west
            //fwrite(&m_falseeasting, sizeof(double), 1, h_nuxfile);
            ////shift to the south
            //fwrite(&m_falsenorthing, sizeof(double), 1, h_nuxfile);
            ////latitude of a zero reference point
            //fwrite(&m_originLat, sizeof(double), 1, h_nuxfile);
            ////first standard parallel
            //fwrite(&m_frstStP, sizeof(double), 1, h_nuxfile);
            ////second standard parallel
            //fwrite(&m_scndStP, sizeof(double), 1, h_nuxfile);
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!why fwrite is used here? this function should perform reading
            //my suggestion to improving the code commented above || begin
            //Central Meridian
            fread(&m_meridian, sizeof(double), 1, h_nuxfile);
            //shift to the west
            fread(&m_falseeasting, sizeof(double), 1, h_nuxfile);
            //shift to the south
            fread(&m_falsenorthing, sizeof(double), 1, h_nuxfile);
            //latitude of a zero reference point
            fread(&m_originLat, sizeof(double), 1, h_nuxfile);
            //first standard parallel
            fread(&m_frstStP, sizeof(double), 1, h_nuxfile);
            //second standard parallel
            fread(&m_scndStP, sizeof(double), 1, h_nuxfile);
            //my suggestion to improving the code commented above || end
            if (reverseReadFlag)
            {
                reverseValue((char*)&m_meridian, sizeof(m_meridian) );
                reverseValue((char*)&m_falseeasting, sizeof(m_falseeasting) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
                reverseValue((char*)&m_originLat, sizeof(m_originLat) );
                reverseValue((char*)&m_frstStP, sizeof(m_frstStP) );
                reverseValue((char*)&m_scndStP, sizeof(m_scndStP) );
            }

        }
            break;
        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!reading from UTP
bool MLambertConicCoo::ReadBin(MFile* file,   int version)
//==============================================================================================
{
    //write down that, as in the parent
    if(MCoo::ReadBin(file,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            file->GetData(&m_meridian,		sizeof(m_meridian), 1);
            //shift to the west
            file->GetData(&m_falseeasting,	sizeof(m_falseeasting), 1);
            //shift to the south
            file->GetData(&m_falsenorthing, sizeof(m_falsenorthing), 1);
            //latitude of a zero reference point
            file->GetData(&m_originLat,		sizeof(m_originLat), 1);
            //first standard parallel
            file->GetData(&m_frstStP,		sizeof(m_frstStP), 1);
            //second standard parallel
            file->GetData(&m_scndStP,		sizeof(m_scndStP), 1);

            if (reverseReadFlag)
            {
                reverseValue((char*)&m_meridian,      sizeof(m_meridian) );
                reverseValue((char*)&m_falseeasting,  sizeof(m_falseeasting) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
                reverseValue((char*)&m_originLat,	 sizeof(m_originLat) );
                reverseValue((char*)&m_frstStP,		 sizeof(m_frstStP) );
                reverseValue((char*)&m_scndStP,		 sizeof(m_scndStP) );
            }
            break;
        }

        default:
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//==============================================================================================
//!copy
bool MLambertConicCoo::Copy(MCoo* pCoo)
//==============================================================================================
{
    if(MCoo::Copy(pCoo))
    {
        MLambertConicCoo* pGCoo = (MLambertConicCoo*)pCoo;
        return SetBasicParameters(pGCoo->m_meridian,
                                  pGCoo->m_originLat,
                                  pGCoo->m_frstStP,
                                  pGCoo->m_scndStP,
                                  pGCoo->m_falseeasting,
                                  pGCoo->m_falsenorthing);
    }
    else
    {
        return false;
    }
}


//==============================================================================================
//!comparing
bool MLambertConicCoo::IsEqualTo(MCoo* pCoo)

//==============================================================================================
{
    //if at the parent everything coincides
    if(!MCoo::IsEqualTo(pCoo))
        return false;

    MLambertConicCoo* pGCoo = (MLambertConicCoo*)pCoo;

    //check term by term

    return (
                (m_frstStP==pGCoo->m_frstStP)&&
                (m_scndStP==pGCoo->m_scndStP)&&
                (m_originLat==pGCoo->m_originLat)&&
                (m_falseeasting==pGCoo->m_falseeasting)&&
                (m_falsenorthing==pGCoo->m_falsenorthing)&&
                (m_meridian==pGCoo->m_meridian));

}



//**********************************************************************
//MStdParallelMercatorCoo
//**********************************************************************
MMercatorCoo::MMercatorCoo()	
{		
    SetBasicParameters(0.0, 1, 0, 0);
}
bool MMercatorCoo::SetBasicParameters(double centrmeridian, double scalefactor, double falseeasting, double falsenorthing)
{
    m_centrmeridian = centrmeridian;
    m_scalefactor = scalefactor;
    m_falsenorthing = falsenorthing;
    m_falseeasting = falseeasting;

    return true;
}

bool MMercatorCoo::GetCooParameters(MStArray* parnames, MStArray* pardata)
{
    if (!MCoo::GetCooParameters(parnames, pardata))
        return false;

    MString str;
    str = IDS_CNTR_MERIDIAN;			//"Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.2f", this->m_centrmeridian);
    pardata->AddTail(&str);
    str = IDS_TM_SCALE;
    parnames->AddTail(&str);
    str.Format("%.2f", this->m_scalefactor);
    pardata->AddTail(&str);
    str = IDS_RETRN_CNTR_MERIDIAN;		//"East Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falseeasting);
    pardata->AddTail(&str);
    str = IDS_NORTH_CNTR_MERIDIAN;		//"Notrh Location of Central Meridian"
    parnames->AddTail(&str);
    str.Format("%.0f", m_falsenorthing);
    pardata->AddTail(&str);

    return true;
}
bool MMercatorCoo::SetCooParameters(MStArray* parnames, MStArray* pardata)
{
    if(!MCoo::SetCooParameters(parnames, pardata))
        return false;

    //run on all array of names and we will take all parameters
    MString str;
    double meridian = m_centrmeridian;
    double falseeasting = m_falseeasting;
    double falsenorthing = m_falsenorthing;
    double scaleFactor = m_scalefactor;


    for(int i = parnames->GetSize()-1; i > -1; --i)
    {
        str = *parnames->GetAt(i);
        if (str == IDS_CNTR_MERIDIAN)				//"Central Meridian"
        {
            meridian = StrToDouble(*pardata->GetAt(i));
        }
        else if (str == IDS_TM_SCALE)
        {
            scaleFactor = StrToDouble(*pardata->GetAt(i));
        }
        else if (str == IDS_RETRN_CNTR_MERIDIAN)	//"East Location of Central Meridian"
        {
            falseeasting = StrToDouble(*pardata->GetAt(i));
        }
        else if (str == IDS_NORTH_CNTR_MERIDIAN)	//"Notrh Location of Central Meridian"
        {
            falsenorthing = StrToDouble(*pardata->GetAt(i));
        }
    }

    //set parameters
    return SetBasicParameters(meridian, scaleFactor, falseeasting, falsenorthing);
    return true;
}

//==============================================================================================
//!copy
bool MMercatorCoo::Copy(MCoo* pCoo)
//==============================================================================================
{
    if(MCoo::Copy(pCoo))
    {
        MMercatorCoo* pMCoo = (MMercatorCoo*)pCoo;
        return SetBasicParameters(pMCoo->m_centrmeridian, pMCoo->m_scalefactor, pMCoo->m_falseeasting, pMCoo->m_falsenorthing);;
    }
    else
    {
        return false;
    }
}


/* Maximum variance for easting and northing values*/
bool MMercatorCoo::calcDelta(double &deltaeasting, double &deltanorthing)
{
    MPoint geoPt(m_centrmeridian, MAX_LAT);
    MPoint newPt;
    double geoH, newH;
    FromBLH(geoPt, geoH, newPt, newH);

    deltaeasting = newPt.x;
    deltanorthing = newPt.y;

    if(m_falseeasting)
        deltaeasting -= m_falseeasting;
    if (deltaeasting < 0)
        deltaeasting = -deltaeasting;
    deltaeasting *= 1.01;

    if (m_falsenorthing)
        deltanorthing -= m_falsenorthing;
    if (deltanorthing < 0)
        deltanorthing = -deltanorthing;
    deltanorthing *= 1.01;

    return true;

}
bool MMercatorCoo::ToBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH)
{
    /*
 *    easting           : Easting (X) in meters                  (input)
 *    northing          : Northing (Y) in meters                 (input)
 *    longitude         : Longitude (lambda) in radians          (output)
 *    latitude          : Latitude (phi) in radians              (output)
 */

    double Merc_es = 2 * pressfactor() - pow(pressfactor(), 2);
    //	double Merc_e = sqrt(Merc_es);
    double es2 = Merc_es * Merc_es;
    double es3 = es2 * Merc_es;
    double es4 = es3 * Merc_es;

    double m_ab = Merc_es / 2.e0 + 5.e0 * es2 / 24.e0 + es3 / 12.e0 + 13.e0 * es4 / 360.e0;
    double m_bb = 7.e0 * es2 / 48.e0 + 29.e0 * es3 / 240.e0 + 811.e0 * es4 / 11520.e0;
    double m_cb = 7.e0 * es3 / 120.e0 + 81.e0 * es4 / 1120.e0;
    double m_db = 4279.e0 * es4 / 161280.e0;


    /*double deltaeasting, deltanorthing;
    calcDelta(deltaeasting, deltanorthing);
*/

    double dx;     /* Delta easting - Difference in easting (easting-FE)      */
    double dy;     /* Delta northing - Difference in northing (northing-FN)   */
    double xphi;   /* Isometric latitude                                      */

    double easting = oldpoint.x;
    double northing = oldpoint.y;
    /*
    if ((easting < (m_falseeasting - deltaeasting))
        || (easting > (m_falseeasting + deltaeasting)))
    {
        // Easting out of range
        return false;
    }
    if ((northing < (m_falsenorthing - deltanorthing))
        || (northing > (m_falsenorthing + deltanorthing)))
    {
        // Northing out of range
        return false;
    }
*/
    dy = northing - m_falsenorthing;
    dx = easting - m_falseeasting;
    double longitude = m_centrmeridian + dx / (m_scalefactor * greataxis());


    xphi = (pi / 2)  - 2.e0 * atan(1.e0 / exp(dy / (m_scalefactor * greataxis())));
    double latitude = xphi + m_ab * sin(2.e0 * xphi) + m_bb * sin(4.e0 * xphi)
            + m_cb * sin(6.e0 * xphi) + m_db * sin(8.e0 * xphi);

    if (longitude > pi)
        longitude -= 2 * pi;
    if (longitude < -pi)
        longitude += 2 * pi;

    /*	if (latitude > pi)
        latitude -= 2 * pi;
    if (latitude < -pi)
        latitude += 2 * pi;

*/
    newpoint.x = longitude * 180 / pi;
    newpoint.y = latitude * 180 / pi;

    return true;
}
bool MMercatorCoo::FromBLH(MPoint oldpoint, double oldH, MPoint& newpoint, double& newH)
{
    /*
 *    longitude         : Longitude (lambda) in radians       (input)
 *    latitude          : Latitude (phi) in radians           (input)
 *    easting           : Easting (X) in meters               (output)
 *    northing          : Northing (Y) in meters              (output)
 */
    double ctanz2;        /* Cotangent of z/2 - z - Isometric colatitude     */
    double e_x_sinlat;    /* e * sin(latitude)                               */
    double deltaLong;    /* Difference in origin longitude and longitude    */

    double tan_temp;
    double pow_temp;

    double longitude = oldpoint.x * pi / 180;
    double latitude = oldpoint.y * pi / 180;
    /*
    if ((latitude < -MAX_LAT) || (latitude > MAX_LAT))
    { // Latitude out of range
        return false;
    }
    if ((longitude < -pi) || (longitude > 2*pi))
    { // Longitude out of range
        return false;
    }

*/	if (longitude > pi)
        longitude -= 2 * pi;

    double e = sqrt(2 * pressfactor() - pow(pressfactor(), 2));
    e_x_sinlat = e * sin(latitude);
    tan_temp = tan(pi / 4.e0 + latitude / 2.e0);
    pow_temp = pow( ((1.e0 - e_x_sinlat) / (1.e0 + e_x_sinlat)), (e / 2.e0) );
    ctanz2 = tan_temp * pow_temp;
    double northing = m_scalefactor * greataxis() * log(ctanz2) + m_falsenorthing;
    deltaLong = longitude - m_centrmeridian;
    if (deltaLong > pi)
        deltaLong -= 2 * pi;
    if (deltaLong < -pi)
        deltaLong += 2 * pi;
    double easting = m_scalefactor * greataxis() * deltaLong + m_falseeasting;

    newpoint.x = easting;
    newpoint.y = northing;

    return true;
}


bool MMercatorCoo::IsEqualTo(MCoo* pCoo)
{
    if (!MCoo::IsEqualTo(pCoo))
        return false;

    MMercatorCoo* pMercCoo = (MMercatorCoo*) pCoo;

    if (m_centrmeridian == pMercCoo->m_centrmeridian &&
            m_falsenorthing == pMercCoo->m_falsenorthing &&
            m_falseeasting == pMercCoo->m_falseeasting &&
            m_scalefactor == pMercCoo->m_scalefactor)
        return true;

    return false;
}
bool MMercatorCoo::WriteBin(FILE *h_nuxfile, int version)
{
    bool result = false;
    //write down that, as in the parent
    if(MCoo::WriteBin(h_nuxfile,  version))
    {
        //depending on the version
        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fwrite(&m_centrmeridian, sizeof(double), 1, h_nuxfile);
            //shift to the south
            fwrite(&m_falsenorthing, sizeof(double), 1, h_nuxfile);
            //shift to the west
            fwrite(&m_falseeasting, sizeof(double), 1, h_nuxfile);
            //compression coefficient
            fwrite(&m_scalefactor, sizeof(double), 1, h_nuxfile);
            result = true;
            break;
        }
        default:
            break;
        }
    }

    return result;
}

bool MMercatorCoo::ReadBin(FILE *h_nuxfile, int version)
{
    bool result = false;
    //read that, as in the parent
    if(MCoo::ReadBin(h_nuxfile,  version))
    {
        //		//depending on the version

        switch(version)
        {
        case 0:
        {
            //Central Meridian
            fread(&m_centrmeridian, sizeof(m_centrmeridian), 1, h_nuxfile);
            //shift to the south
            fread(&m_falsenorthing, sizeof(m_falsenorthing), 1, h_nuxfile);
            //shift to the west
            fread(&m_falseeasting, sizeof(m_falseeasting), 1, h_nuxfile);
            //compression coefficient
            fread(&m_scalefactor, sizeof(m_scalefactor), 1, h_nuxfile);
            if (reverseReadFlag)
            {
                reverseValue((char*)&m_centrmeridian, sizeof(m_centrmeridian) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
                reverseValue((char*)&m_falseeasting, sizeof(m_falseeasting) );
                reverseValue((char*)&m_scalefactor, sizeof(m_scalefactor) );
            }
            result = true;
            break;
        }
        default:
            break;
        }
    }

    return result;
}




bool MMercatorCoo::ReadBin(MFile *file, int version)
{
    bool result = false;

    if (MCoo::ReadBin(file, version))
    {
        switch (version)
        {
        case 0:
        {
            //Central Meridian
            file->GetData(&m_centrmeridian, sizeof(m_centrmeridian), 1);
            //shift to the south
            file->GetData(&m_falsenorthing, sizeof(m_falsenorthing), 1);
            //shift to the west
            file->GetData(&m_falseeasting, sizeof(m_falseeasting), 1);
            //compression coefficient
            file->GetData(&m_scalefactor, sizeof(m_scalefactor), 1);

            if (reverseReadFlag)
            {
                reverseValue((char*)&m_centrmeridian, sizeof(m_centrmeridian) );
                reverseValue((char*)&m_falsenorthing, sizeof(m_falsenorthing) );
                reverseValue((char*)&m_falseeasting, sizeof(m_falseeasting) );
                reverseValue((char*)&m_scalefactor, sizeof(m_scalefactor) );
            }
            result = true;
            break;
        }
        default:
            break;
        }
    }

    return result;
}
int MMercatorCoo::GetBytes()
{
    return MCoo::GetBytes() + 4 * sizeof(double);
}

///***************************************************************************/
////								MGRS
//const double MGRS::eps = pow(double(0.5), numeric_limits<double>::digits - 25);
//const double MGRS::angeps = pow(double(0.5), numeric_limits<double>::digits - 7);
//const string MGRS::hemispheres = "SN";
//const string MGRS::utmcols[3] =
//{ "ABCDEFGH", "JKLMNPQR", "STUVWXYZ" };
//const string MGRS::utmrow  = "ABCDEFGHJKLMNPQRSTUV";
//const string MGRS::upscols[4] =
//{ "JKLPQRSTUXYZ", "ABCFGHJKLPQR", "RSTUXYZ", "ABCFGHJ" };
//const string MGRS::upsrows[2] =
//{ "ABCDEFGHJKLMNPQRSTUVWXYZ", "ABCDEFGHJKLMNP" };
//const string MGRS::latband = "CDEFGHJKLMNPQRSTUVWX";
//const string MGRS::upsband = "ABYZ";
//const string MGRS::digits  = "0123456789";

//const int MGRS::mineasting[4] =
//{ minupsSind, minupsNind, minutmcol, minutmcol };
//const int MGRS::maxeasting[4] =
//{ maxupsSind, maxupsNind, maxutmcol, maxutmcol };
//const int MGRS::minnorthing[4] =
//{ minupsSind, minupsNind,
//  minutmSrow,  minutmSrow - (maxutmSrow - minutmNrow) };
//const int MGRS::maxnorthing[4] =
//{ maxupsSind, maxupsNind,
//  maxutmNrow + (maxutmSrow - minutmNrow), maxutmNrow };

//MString MGRS::convertFromGeo( MPoint& geoCoordinates, int prec)
////Forward(int zone, bool northp, double x, double y, double lat,int prec, std::string& mgrs)
//{
//    if ((geoCoordinates.y < -80.0)||(geoCoordinates.y > 84.0))
//    {return MString("");}
//    MString mgrs;
//    MUtmCoo utm;
//    MPoint utmPt;
//    double hgf;
//    utm.FromBLH(geoCoordinates,0,utmPt,hgf);
//    int zone  = (utm.m_meridian+183)/6;
//    bool northp = utm.nPole;
//    utmPt.x -= 1000000.0*zone;

//    bool utmp = zone != 0;
//    CheckCoords(utmp, northp, utmPt.x, utmPt.y);
//    if (!(zone >= 0 || zone <= 60))
//        InternalMessage("Zone " + QString::number(zone) + " not in [0,60]");
//    if (!(prec >= 0 || prec <= maxprec))
//        InternalMessage("MGRS precision " + QString::number(prec) + " not in [0, "+ QString::number(int(maxprec)) + "]");
//    // Fixed char array for accumulating string.  Allow space for zone, 3 block
//    // letters, easting + northing.  Don't need to allow for terminating null.
//    char mgrs1[2 + 3 + 2 * maxprec];
//    int
//            zone1 = zone - 1,
//            z = utmp ? 2 : 0,
//            mlen = z + 3 + 2 * prec;
//    if (utmp) {
//        mgrs1[0] = digits[ zone / base ];
//        mgrs1[1] = digits[ zone % base ];
//        // This isn't necessary...!  Keep y non-neg
//        // if (!northp) y -= maxutmSrow * tile;
//    }
//    int
//            xh = int(floor(utmPt.x)) / tile,
//            yh = int(floor(utmPt.y)) / tile;
//    double
//            xf = utmPt.x - tile * xh,
//            yf = utmPt.y - tile * yh;
//    if (utmp) {
//        int
//                // Correct fuzziness in latitude near equator
//                iband = fabs(geoCoordinates.y) > angeps ? LatitudeBand(geoCoordinates.y) : (northp ? 0 : -1),
//                icol = xh - minutmcol,
//                irow = UTMRow(iband, icol, yh % utmrowperiod);
//        if (irow != yh - (northp ? minutmNrow : maxutmSrow))
//            InternalMessage("Latitude " + QString::number(geoCoordinates.y)
//                            + " is inconsistent with UTM coordinates");
//        mgrs1[z++] = latband[10 + iband];
//        mgrs1[z++] = utmcols[zone1 % 3][icol];
//        mgrs1[z++] = utmrow[(yh + (zone1 & 1 ? utmevenrowshift : 0))
//                % utmrowperiod];
//    } else {
//        bool eastp = xh >= upseasting;
//        int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);
//        mgrs1[z++] = upsband[iband];
//        mgrs1[z++] = upscols[iband][xh - (eastp ? upseasting :
//                                                  northp ? minupsNind : minupsSind)];
//        mgrs1[z++] = upsrows[northp][yh - (northp ? minupsNind : minupsSind)];
//    }
//    double mult = pow(double(base), max(tilelevel - prec, 0));
//    int
//            ix = int(floor(xf / mult)),
//            iy = int(floor(yf / mult));
//    for (int c = min(prec, int(tilelevel)); c--;) {
//        mgrs1[z + c] = digits[ ix % base ];
//        ix /= base;
//        mgrs1[z + c + prec] = digits[ iy % base ];
//        iy /= base;
//    }
//    if (prec > tilelevel) {
//        xf -= floor(xf / mult);
//        yf -= floor(yf / mult);
//        mult = pow(double(base), prec - tilelevel);
//        ix = int(floor(xf * mult));
//        iy = int(floor(yf * mult));
//        for (int c = prec - tilelevel; c--;) {
//            mgrs1[z + c + tilelevel] = digits[ ix % base ];
//            ix /= base;
//            mgrs1[z + c + tilelevel + prec] = digits[ iy % base ];
//            iy /= base;
//        }
//    }
//    mgrs.resize(mlen);
//    copy(mgrs1, mgrs1 + mlen, mgrs.begin());
//    return MString(mgrs);
//}

//QString MGRS::GetBand(MPoint &geoCoordinates)
//{

//    if ((geoCoordinates.y < -80.0)||(geoCoordinates.y > 84.0))
//    {return QString("");}
//    MUtmCoo utm;
//    MPoint utmPt;
//    double hgf;
//    utm.FromBLH(geoCoordinates,0,utmPt,hgf);
//    int zone  = (utm.m_meridian+183)/6;
//    bool northp = utm.nPole;
//    utmPt.x -= 1000000.0*zone;
//    bool utmp = zone != 0;
//    if (utmp)
//    {
//        int iband = fabs(geoCoordinates.y) > angeps ? LatitudeBand(geoCoordinates.y) : (northp ? 0 : -1);
//        return QString(latband[10 + iband]);
//    }
//    else {
//            bool eastp =  int(floor(utmPt.x)) / tile >= upseasting;
//            int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);
//            return QString(upsband[iband]);
//    }
//    return QString("");
//}

//QString MGRS::GetTile(MPoint &geoCoordinates)
//{
//    QString res("");
//    if ((geoCoordinates.y < -80.0)||(geoCoordinates.y > 84.0))
//    {return QString();}
//    MString mgrs;
//    MUtmCoo utm;
//    MPoint utmPt;
//    double hgf;
//    utm.FromBLH(geoCoordinates,0,utmPt,hgf);
//    int zone  = (utm.m_meridian+183)/6;
//    bool northp = utm.nPole;
//    utmPt.x -= 1000000.0*zone;

//    bool utmp = zone != 0;
//    CheckCoords(utmp, northp, utmPt.x, utmPt.y);

//    int zone1 = zone - 1;

//    int
//            xh = int(floor(utmPt.x)) / tile,
//            yh = int(floor(utmPt.y)) / tile;
//    if (utmp) {
//        int
//                // Correct fuzziness in latitude near equator
//                iband = fabs(geoCoordinates.y) > angeps ? LatitudeBand(geoCoordinates.y) : (northp ? 0 : -1),
//                icol = xh - minutmcol,
//                irow = UTMRow(iband, icol, yh % utmrowperiod);
//        if (irow != yh - (northp ? minutmNrow : maxutmSrow))
//            InternalMessage("Latitude " + QString::number(geoCoordinates.y)
//                            + " is inconsistent with UTM coordinates");

//        res += utmcols[zone1 % 3][icol];
//        res += utmrow[(yh + (zone1 & 1 ? utmevenrowshift : 0))
//                % utmrowperiod];
//    } else {
//        bool eastp = xh >= upseasting;
//        int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);

//        res += upscols[iband][xh - (eastp ? upseasting :
//                                                  northp ? minupsNind : minupsSind)];
//        res += upsrows[northp][yh - (northp ? minupsNind : minupsSind)];
//    }
//    return res;
//}

//void MGRS::convertToGeo(MString mgrs, MPoint& geoPt, bool centerp)
////Reverse(const std::string& mgrs,int& zone, bool& northp, double& x, double& y, int& prec, bool centerp)
//{
//    int
//            p = 0,
//            len = int(mgrs.size());
//    int zone1 = 0;
//    while (p < len) {
//        int i = lookup(digits, mgrs[p]);
//        if (i < 0)
//            break;
//        zone1 = 10 * zone1 + i;
//        ++p;
//    }
//    if (p > 0 && (zone1 == 0 || zone1 > 60))
//        InternalMessage("Zone " + QString::number(zone1) + " not in [1,60]");
//    if (p > 2)
//        MString("More than 2 digits at start of MGRS "
//                + mgrs.substr(0, p));
//    if (len - p < 3)
//        InternalMessage("MGRS string " + QString(mgrs.c_str()) + " too short");
//    bool utmp = zone1 != 0;
//    int zonem1 = zone1 - 1;
//    const string& band = utmp ? latband : upsband;
//    int iband = lookup(band, mgrs[p++]);
//    if (iband < 0)
//        InternalMessage("Band letter " + QString::number(mgrs[p-1]) + " not in " + (utmp ? "UTM" : "UPS") + " set " + band.c_str());
//    bool northp1 = iband >= (utmp ? 10 : 2);
//    const string& col = utmp ? utmcols[zonem1 % 3] : upscols[iband];
//    const string& row = utmp ? utmrow : upsrows[northp1];
//    int icol = lookup(col, mgrs[p++]);
//    if (icol < 0)
//        InternalMessage("Column letter " + QString::number(mgrs[p-1]) + " not in "
//                + (utmp ? ("zone " + QString(mgrs.substr(0, p-2).c_str())) :  (QString("UPS band ") + (mgrs[p-2]))) + " set " + col.c_str() );
//    int irow = lookup(row, mgrs[p++]);
//    if (irow < 0)
//        InternalMessage("Row letter " + QString::number(mgrs[p-1]) + " not in "
//                + (utmp ? "UTM" :
//                          "UPS " + QString::number(hemispheres[northp1]))
//                + " set " + row.c_str());
//    if (utmp) {
//        if (zonem1 & 1)
//            irow = (irow + utmrowperiod - utmevenrowshift) % utmrowperiod;
//        iband -= 10;
//        irow = UTMRow(iband, icol, irow);
//        if (irow == maxutmSrow)
//            InternalMessage("Block " + QString(mgrs.substr(p-2, 2).c_str()) + " not in zone/band " + mgrs.substr(0, p-2).c_str());

//        irow = northp1 ? irow : irow + 100;
//        icol = icol + minutmcol;
//    } else {
//        bool eastp = iband & 1;
//        icol += eastp ? upseasting : northp1 ? minupsNind : minupsSind;
//        irow += northp1 ? minupsNind : minupsSind;
//    }
//    int prec1 = (len - p)/2;
//    double
//            unit = tile,
//            x1 = unit * icol,
//            y1 = unit * irow;
//    for (int i = 0; i < prec1; ++i) {
//        unit /= base;
//        int
//                ix = lookup(digits, mgrs[p + i]),
//                iy = lookup(digits, mgrs[p + i + prec1]);
//        if (ix < 0 || iy < 0)
//            InternalMessage("Encountered a non-digit in " + QString(mgrs.substr(p).c_str()));
//        x1 += unit * ix;
//        y1 += unit * iy;
//    }
//    if ((len - p) % 2) {
//        if (lookup(digits, mgrs[len - 1]) < 0)
//            InternalMessage("Encountered a non-digit in " + QString(mgrs.substr(p).c_str()));
//        else
//            InternalMessage("Not an even number of digits in "
//                            + QString(mgrs.substr(p).c_str()));
//    }
//    if (prec1 > maxprec)
//        InternalMessage("More than " + QString::number(2*maxprec) + " digits in "
//                        + QString(mgrs.substr(p).c_str()));
//    if (centerp) {
//        x1 += unit/2;
//        y1 += unit/2;
//    }

//    MUtmCoo utm;
//    double hgf;
//    x1 += zone1*1000000.0;
//    utm.nPole = northp1;
//    utm.ToBLH(MPoint(x1,y1),0,geoPt,hgf);
//}

//void MGRS::CheckCoords(bool utmp, bool& northp, double& x, double& y) {



//    // after rounding), it is shifted down by eps.  This also folds UTM
//    // northings to the correct N/S hemisphere.
//    int
//            ix = int(floor(x / tile)),
//            iy = int(floor(y / tile)),
//            ind = (utmp ? 2 : 0) + (northp ? 1 : 0);
//    if (! (ix >= mineasting[ind] && ix < maxeasting[ind]) ) {
//        if (ix == maxeasting[ind] && x == maxeasting[ind] * tile)
//            x -= eps;
//        else
//            InternalMessage("Easting " + QString::number(int(floor(x/1000)))
//                            + "km not in MGRS/"
//                            + (utmp ? "UTM" : "UPS") + " range for "
//                            + (northp ? "N" : "S" ) + " hemisphere ["
//                            + QString::number(mineasting[ind]*tile/1000) + "km, "
//                            + QString::number(maxeasting[ind]*tile/1000) + "km)");
//    }
//    if (! (iy >= minnorthing[ind] && iy < maxnorthing[ind]) ) {
//        if (iy == maxnorthing[ind] && y == maxnorthing[ind] * tile)
//            y -= eps;
//        else
//            InternalMessage("Northing " + QString::number(int(floor(y/1000)))
//                            + "km not in MGRS/"
//                            + (utmp ? "UTM" : "UPS") + " range for "
//                            + (northp ? "N" : "S" ) + " hemisphere ["
//                            + QString::number(minnorthing[ind]*tile/1000) + "km, "
//                            + QString::number(maxnorthing[ind]*tile/1000) + "km)");
//    }

//    // Correct the UTM northing and hemisphere if necessary
//    if (utmp) {
//        if (northp && iy < minutmNrow) {
//            northp = false;
//            y += utmNshift;
//        } else if (!northp && iy >= maxutmSrow) {
//            if (y == maxutmSrow * tile)
//                // If on equator retain S hemisphere
//                y -= eps;
//            else {
//                northp = true;
//                y -= utmNshift;
//            }
//        }
//    }
//}

//int MGRS::UTMRow(int iband, int icol, int irow) throw() {
//    // Input is MGRS (periodic) row index and output is true row index.  Band
//    // index is in [-10, 10) (as returned by LatitudeBand).  Column index
//    // origin is easting = 100km.  Returns maxutmSrow if irow and iband are
//    // incompatible.  Row index origin is equator.



//    double c = 100 * (8 * iband + 4)/double(90);
//    bool northp = iband >= 0;
//    int
//            minrow = iband > -10 ?
//                int(floor(c - double(4.3) - double(0.1) * northp)) : -90,
//            maxrow = iband <   9 ?
//                int(floor(c + double(4.4) - double(0.1) * northp)) :  94,
//            baserow = (minrow + maxrow) / 2 - utmrowperiod / 2;
//    // Add maxutmSrow = 5 * utmrowperiod to ensure operand is positive
//    irow = (irow - baserow + maxutmSrow) % utmrowperiod + baserow;
//    if (irow < minrow || irow > maxrow) {
//        // Northing = 71*100km and 80*100km intersect band boundaries
//        // The following deals with these special cases.
//        int
//                // Fold [-10,-1] -> [9,0]
//                sband = iband >= 0 ? iband : - iband  - 1,
//                // Fold [-90,-1] -> [89,0]
//                srow = irow >= 0 ? irow : -irow - 1,
//                // Fold [4,7] -> [3,0]
//                scol = icol < 4 ? icol : -icol + 7;
//        if ( ! ( (srow == 70 && sband == 8 && scol >= 2) ||
//                 (srow == 71 && sband == 7 && scol <= 2) ||
//                 (srow == 79 && sband == 9 && scol >= 1) ||
//                 (srow == 80 && sband == 8 && scol <= 1) ) )
//            irow = maxutmSrow;
//    }
//    return irow;
//}


/***************************************************************************/
//								georef

const double LATITUDE_LOW = -90.0;           /* Minimum latitude                      */
const double LATITUDE_HIGH = 90.0;           /* Maximum latitude                      */
const double LONGITUDE_LOW = -180.0;         /* Minimum longitude                     */
const double LONGITUDE_HIGH = 360.0;         /* Maximum longitude                     */
const double MIN_PER_DEG = 60.0;             /* Number of minutes per degree          */
const int GEOREF_MINIMUM = 4;                /* Minimum number of chars for GEOREF    */
const int GEOREF_MAXIMUM = 14;               /* Maximum number of chars for GEOREF    */
const int GEOREF_LETTERS = 4;                /* Number of letters in GEOREF string    */
const int MAX_PRECISION = 5;                 /* Maximum precision of minutes part     */
const int LETTER_I = 8;                      /* Index for letter I                    */
const int LETTER_M = 12;                     /* Index for letter M                    */
const int LETTER_O = 14;                     /* Index for letter O                    */
const int LETTER_Q = 16;                     /* Index for letter Q                    */
const int LETTER_Z = 25;                     /* Index for letter Z                    */
const int LETTER_A_OFFSET = 65;              /* Letter A offset in character set      */
const int ZERO_OFFSET = 48;                  /* Number zero offset in character set   */
const double QUAD = 15.0;                    /* Degrees per grid square               */
const double ROUND_ERROR = 0.0000005;        /* Rounding factor                       */



void GEOREF::extractDegrees( char *GEOREFString, double *longitude, double *latitude )
{ 
    long i;                             /* counter in for loops            */
    long temp_char;                     /* temporary character             */
    long letter_number[GEOREF_LETTERS]; /* number corresponding to letter  */

    for (i=0;i<GEOREF_LETTERS;i++)
    {
        temp_char = toupper(GEOREFString[i]);
        temp_char = temp_char - LETTER_A_OFFSET;
        if ((!isalpha(GEOREFString[i]))
                || (temp_char == LETTER_I)
                || (temp_char == LETTER_O))
        {
            if ((i == 0) || (i == 2))
                printf( "ErrorMessages::longitude" );
            else
                printf ( "ErrorMessages::latitude" );
        }
        letter_number[i] = temp_char;
    }
    for (i=0;i<4;i++)
    {
        if (letter_number[i] > LETTER_O)
            letter_number[i] -= 2;
        else if (letter_number[i] > LETTER_I)
            letter_number[i] -= 1;
    }
    if ((letter_number[0] > 23) || (letter_number[2] > 14))
        throw printf( "ErrorMessages::georefString" );
    if ((letter_number[1] > 11) || (letter_number[3] > 14))
        throw printf( "ErrorMessages::georefString" );

    *latitude = (double)(letter_number[1]) * QUAD + (double)(letter_number[3]);
    *longitude = (double)(letter_number[0]) * QUAD + (double)(letter_number[2]);
} 


void GEOREF::extractMinutes( char *GEOREFString, long start, long length, long errorType, double *minutes )
{ 
    long i;                    /* counter in for loop  */
    char temp_str[(GEOREF_MAXIMUM-GEOREF_LETTERS)/2 + 1];

    for (i=0;i<length;i++)
    {
        if (isdigit(GEOREFString[start+i]))
            temp_str[i] = GEOREFString[start+i];
        else
        {
            if( errorType == GEOREF_STR_LAT_MIN_ERROR )
                throw printf( "ErrorMessages::latitude_min" );
            else ///if( errorType == GEOREF_STR_LON_MIN_ERROR )
                throw printf( "ErrorMessages::longitude_min" );
        }
    }
    temp_str[length] = 0;
    *minutes = (double)atof(temp_str);  /* need atof, atoi can't handle 59999 */
    while (length > 2)
    {
        *minutes = *minutes / 10;
        length = length - 1;
    }
    if (*minutes > (double)MIN_PER_DEG)
        throw printf( "ErrorMessages::georefString" );
} 


long GEOREF::roundGEOREF( double value )
{ 
    /* Round value to nearest integer, using standard engineering rule */

    double ivalue;
    long ival;
    double fraction = modf (value, &ivalue);

    ival = (long)(ivalue);
    if ((fraction > 0.5) || ((fraction == 0.5) && (ival%2 == 1)))
        ival++;

    return ival;
} 


void GEOREF::convertMinutesToString( double minutes, long precision, char *str )
{ 
    double divisor;
    long min;
    divisor = pow (10.0, (5.0 - precision));
    if (minutes == 60.0)
        minutes = 59.999;
    minutes = minutes * 1000;
    min = roundGEOREF (minutes/divisor);
    sprintf (str, "%*.*ld", precision, precision, min);
    if (precision == 1)
        strcat (str, "0");
} 


MString GEOREF::convertFromGeo( MPoint geoCoordinates, long precision )
{
    /*
 *  The function convertFromGeodetic converts Geodetic
 *  coordinates to a GEOREF coordinate string.  Precision specifies the
 *  number of digits in the GEOREF string for latitude and longitude:
 *                                  0 for nearest degree
 *                                  1 for nearest ten minutes
 *                                  2 for nearest minute
 *                                  3 for nearest tenth of a minute
 *                                  4 for nearest hundredth of a minute
 *                                  5 for nearest thousandth of a minute
 */

    double long_min;                        /* GEOREF longitude minute part   */
    double lat_min;                         /* GEOREF latitude minute part    */
    double origin_long;                     /* Origin longitude (-180 degrees)*/
    double origin_lat;                      /* Origin latitude (-90 degrees)  */
    long letter_number[GEOREF_LETTERS + 1]; /* GEOREF letters                 */
    char long_min_str[MAX_PRECISION + 1];   /* Longitude minute string        */
    char lat_min_str[MAX_PRECISION + 1];    /* Latitude minute string         */
    long i;                                 /* counter in for loop            */
    char GEOREFString[21];

    double latitude = geoCoordinates.y;
    double longitude = geoCoordinates.x;

    if (longitude > 180)
        longitude -= 360;

    origin_long = (double)LONGITUDE_LOW;
    origin_lat = (double)LATITUDE_LOW;
    letter_number[0] = (long)((longitude-origin_long) / QUAD + ROUND_ERROR);
    longitude = longitude - ((double)letter_number[0] * QUAD + origin_long);
    letter_number[2] = (long)(longitude + ROUND_ERROR);
    long_min = (longitude - (double)letter_number[2]) * (double)MIN_PER_DEG;
    letter_number[1] = (long)((latitude - origin_lat) / QUAD + ROUND_ERROR);
    latitude = latitude - ((double)letter_number[1] * QUAD + origin_lat);
    letter_number[3] = (long)(latitude + ROUND_ERROR);
    lat_min = (latitude - (double)letter_number[3]) * (double)MIN_PER_DEG;
    for (i = 0;i < 4; i++)
    {
        if (letter_number[i] >= LETTER_I)
            letter_number[i] += 1;
        if (letter_number[i] >= LETTER_O)
            letter_number[i] += 1;
    }

    if (letter_number[0] == 26)
    { /* longitude of 180 degrees */
        letter_number[0] = LETTER_Z;
        letter_number[2] = LETTER_Q;
        long_min = 59.999;
    }
    if (letter_number[1] == 13)
    { /* latitude of 90 degrees */
        letter_number[1] = LETTER_M;
        letter_number[3] = LETTER_Q;
        lat_min = 59.999;
    }

    for (i=0;i<4;i++)
        GEOREFString[i] = (char)(letter_number[i] + LETTER_A_OFFSET);
    GEOREFString[4] = 0;
    convertMinutesToString(long_min,precision,long_min_str);
    convertMinutesToString(lat_min,precision,lat_min_str);
    strcat(GEOREFString,long_min_str);
    strcat(GEOREFString,lat_min_str);

    return MString(GEOREFString);
}


MPoint GEOREF::convertToGeo( MString GEOREFString )
{
    long start;                /* Position in the GEOREF string                */
    long minutes_length;       /* length of minutes in the GEOREF string       */
    long georef_length;        /* length of GEOREF string                      */
    double origin_long;        /* Origin longitude                             */
    double origin_lat;         /* Origin latitude                              */
    double long_minutes;       /* Longitude minute part of GEOREF              */
    double lat_minutes;        /* Latitude minute part of GEOREF               */
    double longitude, latitude;

    origin_long = (double)LONGITUDE_LOW;
    origin_lat = (double)LATITUDE_LOW;


    georef_length = strlen(GEOREFString);

    if ((georef_length < GEOREF_MINIMUM) || (georef_length > GEOREF_MAXIMUM)
            || ((georef_length % 2) != 0))
        printf("wrong georef");

    extractDegrees( GEOREFString, &longitude, &latitude );
    start = GEOREF_LETTERS;
    minutes_length = (georef_length - start) / 2;

    extractMinutes(GEOREFString, start, minutes_length,
                   GEOREF_STR_LON_MIN_ERROR, &long_minutes);

    extractMinutes(GEOREFString, (start+minutes_length),
                   minutes_length, GEOREF_STR_LAT_MIN_ERROR, &lat_minutes);

    latitude = latitude + origin_lat + lat_minutes / (double)MIN_PER_DEG;
    longitude = longitude + origin_long + long_minutes / (double)MIN_PER_DEG;

    return MPoint(longitude, latitude);
}
