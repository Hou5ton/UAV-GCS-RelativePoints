#include "mgrs.h"
#include <limits>
#include <iostream>
#include "BasicCoo.h"

using namespace std;
/***************************************************************************/
//								MGRS
const double MGRS::eps = pow(double(0.5), numeric_limits<double>::digits - 25);
const double MGRS::angeps = pow(double(0.5), numeric_limits<double>::digits - 7);
const string MGRS::hemispheres = "SN";
const string MGRS::utmcols[3] =
{ "ABCDEFGH", "JKLMNPQR", "STUVWXYZ" };
const string MGRS::utmrow  = "ABCDEFGHJKLMNPQRSTUV";
const string MGRS::upscols[4] =
{ "JKLPQRSTUXYZ", "ABCFGHJKLPQR", "RSTUXYZ", "ABCFGHJ" };
const string MGRS::upsrows[2] =
{ "ABCDEFGHJKLMNPQRSTUVWXYZ", "ABCDEFGHJKLMNP" };
const string MGRS::latband = "CDEFGHJKLMNPQRSTUVWX";
const string MGRS::upsband = "ABYZ";
const string MGRS::digits  = "0123456789";

const int MGRS::mineasting[4] =
{ minupsSind, minupsNind, minutmcol, minutmcol };
const int MGRS::maxeasting[4] =
{ maxupsSind, maxupsNind, maxutmcol, maxutmcol };
const int MGRS::minnorthing[4] =
{ minupsSind, minupsNind,
  minutmSrow,  minutmSrow - (maxutmSrow - minutmNrow) };
const int MGRS::maxnorthing[4] =
{ maxupsSind, maxupsNind,
  maxutmNrow + (maxutmSrow - minutmNrow), maxutmNrow };

string MGRS::convertFromGeo(const double &xGeo, const double &yGeo/* MPoint& geoCoordinates*/, int prec)
//Forward(int zone, bool northp, double x, double y, double lat,int prec, std::string& mgrs)
{
    if ((yGeo < -80.0)||(yGeo > 84.0))
    {return std::string("");}
    std::string mgrs;

    double xLocal,yLocal,altLocal;
    MUtmProjection utm;
    utm.FromGeodetic(xGeo,yGeo,0,xLocal,yLocal,altLocal);
    int zone  = (utm.m_meridian+183)/6;
    bool northp = utm.nPole;
    xLocal -= 1000000.0*zone;

    bool utmp = zone != 0;
    CheckCoords(utmp, northp, xLocal, yLocal);
    if (zone < 0 || zone > 60)
        cout<<"Zone "<<zone<<" not in [0,60]"<<endl;
    if (!(prec >= 0 || prec <= maxprec))
        cout<<"MGRS precision " <<prec <<" not in [0, "<< int(maxprec)<< "]"<<endl;
    // Fixed char array for accumulating string.  Allow space for zone, 3 block
    // letters, easting + northing.  Don't need to allow for terminating null.
    char mgrs1[2 + 3 + 2 * maxprec];
    int
            zone1 = zone - 1,
            z = utmp ? 2 : 0,
            mlen = z + 3 + 2 * prec;
    if (utmp) {
        mgrs1[0] = digits[ zone / base ];
        mgrs1[1] = digits[ zone % base ];
        // This isn't necessary...!  Keep y non-neg
        // if (!northp) y -= maxutmSrow * tile;
    }
    int
            xh = int(floor(xLocal)) / tile,
            yh = int(floor(yLocal)) / tile;
    double
            xf = xLocal - tile * xh,
            yf = yLocal - tile * yh;
    if (utmp) {
        int
                // Correct fuzziness in latitude near equator
                iband = fabs(yGeo) > angeps ? LatitudeBand(yGeo) : (northp ? 0 : -1),
                icol = xh - minutmcol,
                irow = UTMRow(iband, icol, yh % utmrowperiod);
        if (irow != yh - (northp ? minutmNrow : maxutmSrow))
            cout<<"Latitude " <<yGeo<<" is inconsistent with UTM coordinates"<<endl;
        mgrs1[z++] = latband[10 + iband];
        mgrs1[z++] = utmcols[zone1 % 3][icol];
        mgrs1[z++] = utmrow[(yh + (zone1 & 1 ? utmevenrowshift : 0))
                % utmrowperiod];
    } else {
        bool eastp = xh >= upseasting;
        int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);
        mgrs1[z++] = upsband[iband];
        mgrs1[z++] = upscols[iband][xh - (eastp ? upseasting :
                                                  northp ? minupsNind : minupsSind)];
        mgrs1[z++] = upsrows[northp][yh - (northp ? minupsNind : minupsSind)];
    }
    double mult = pow(double(base), max(tilelevel - prec, 0));
    int
            ix = int(floor(xf / mult)),
            iy = int(floor(yf / mult));
    for (int c = min(prec, int(tilelevel)); c--;) {
        mgrs1[z + c] = digits[ ix % base ];
        ix /= base;
        mgrs1[z + c + prec] = digits[ iy % base ];
        iy /= base;
    }
    if (prec > tilelevel) {
        xf -= floor(xf / mult);
        yf -= floor(yf / mult);
        mult = pow(double(base), prec - tilelevel);
        ix = int(floor(xf * mult));
        iy = int(floor(yf * mult));
        for (int c = prec - tilelevel; c--;) {
            mgrs1[z + c + tilelevel] = digits[ ix % base ];
            ix /= base;
            mgrs1[z + c + tilelevel + prec] = digits[ iy % base ];
            iy /= base;
        }
    }
    mgrs.resize(mlen);
    copy(mgrs1, mgrs1 + mlen, mgrs.begin());
    return mgrs;
}

//std::string MGRS::GetBand(double &xGeo,double &yGeo)
//{

//    if ((yGeo < -80.0)||(yGeo > 84.0))
//        return std::string("");

//    MUtmProjection utm;
//    double xLocal,yLocal,altLocal;
//    utm.FromGeodetic(xGeo,yGeo,0,xLocal,yLocal,altLocal);

//    int zone  = (utm.m_meridian+183)/6;
//    bool northp = utm.nPole;
//    xLocal -= 1000000.0*zone;
//    bool utmp = zone != 0;
//    if (utmp)
//    {
//        int iband = fabs(yGeo) > angeps ? LatitudeBand(yGeo) : (northp ? 0 : -1);
//        return std::string(&latband[10 + iband]);
//    }
//    else {
//            bool eastp =  int(floor(xLocal)) / tile >= upseasting;
//            int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);
//            return std::string(&upsband[iband]);
//    }
//    return std::string("");
//}

std::string MGRS::GetTileString(double &xGeo, double &yGeo, bool needTile)
{

        if ((yGeo < -80.0)||(yGeo > 84.0))
        return std::string();

        MUtmProjection utm;
        double xLocal,yLocal,altLocal;
        utm.FromGeodetic(xGeo,yGeo,0,xLocal,yLocal,altLocal);

        int zone  = (utm.m_meridian+183)/6;
        bool northp = utm.nPole;
        xLocal -= 1000000.0*zone;

        std::string mgrsString = std::to_string(zone);

        bool utmp = zone != 0;

           if (utmp)
            {
                int iband = fabs(yGeo) > angeps ? LatitudeBand(yGeo) : (northp ? 0 : -1);
                char chBand = latband[10 + iband];
                mgrsString+= chBand;
            }
            else {
                bool eastp =  int(floor(xLocal)) / tile >= upseasting;
                int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);
                char chBand =upsband[iband];
                mgrsString+=chBand;
            }
        if(!needTile)
            return mgrsString;

        CheckCoords(utmp, northp, xLocal, yLocal);


        int zone1 = zone - 1;

        int
                xh = int(floor(xLocal)) / tile,
                yh = int(floor(yLocal)) / tile;
        if (utmp) {
            int
                    // Correct fuzziness in latitude near equator
                    iband = fabs(yGeo) > angeps ? LatitudeBand(yGeo) : (northp ? 0 : -1),
                    icol = xh - minutmcol,
                    irow = UTMRow(iband, icol, yh % utmrowperiod);
//            if (irow != yh - (northp ? minutmNrow : maxutmSrow))
//                cout<<"Latitude " <<yGeo<< " is inconsistent with UTM coordinates"<<endl;

            char chBand = utmcols[zone1 % 3][icol];
            mgrsString+= chBand;
            chBand =utmrow[(yh + (zone1 & 1 ? utmevenrowshift : 0))
                    % utmrowperiod];
            mgrsString+=chBand;
        } else {
            bool eastp = xh >= upseasting;
            int iband = (northp ? 2 : 0) + (eastp ? 1 : 0);
            char chBand =upscols[iband][xh - (eastp ? upseasting :
                                                      northp ? minupsNind : minupsSind)];
            mgrsString+= chBand;
            chBand = upsrows[northp][yh - (northp ? minupsNind : minupsSind)];
            mgrsString += chBand;
        }
        return mgrsString;

}


void MGRS::convertToGeo(string &mgrs, double &xGeo, double &yGeo, bool centerp)
{
    int
            p = 0,
            len = int(mgrs.size());
    int zone1 = 0;
    while (p < len) {
        int i = lookup(digits, mgrs[p]);
        if (i < 0)
            break;
        zone1 = 10 * zone1 + i;
        ++p;
    }
    if (p > 0 && (zone1 == 0 || zone1 > 60))
        cout<<"Zone " <<zone1 << " not in [1,60]"<<endl;
    if (p > 2)
        std::string("More than 2 digits at start of MGRS "
                + mgrs.substr(0, p));
    if (len - p < 3)
        cout<<"MGRS string "<<mgrs.c_str()<< " too short"<<endl;
    bool utmp = zone1 != 0;
    int zonem1 = zone1 - 1;
    const string& band = utmp ? latband : upsband;
    int iband = lookup(band, mgrs[p++]);
    if (iband < 0)
        cout<<"Band letter " <<mgrs[p-1] << " not in " << (utmp ? "UTM" : "UPS") << " set " << band.c_str()<<endl;
    bool northp1 = iband >= (utmp ? 10 : 2);
    const string& col = utmp ? utmcols[zonem1 % 3] : upscols[iband];
    const string& row = utmp ? utmrow : upsrows[northp1];
    int icol = lookup(col, mgrs[p++]);

    if (icol < 0)
        cout<<"Column letter " << mgrs[p-1] << " not in "
           << (utmp ? "zone " + mgrs.substr(0, p-2) : "UPS band " +std::string(&mgrs[p-2]))
           << " set " << col.c_str() ;
    int irow = lookup(row, mgrs[p++]);
    if (irow < 0)
        cout<<"Row letter " << mgrs[p-1] << " not in "
                << (utmp ? "UTM" :
                          "UPS " + std::string(&hemispheres[northp1]))
                << " set " << row.c_str();
    if (utmp) {
        if (zonem1 & 1)
            irow = (irow + utmrowperiod - utmevenrowshift) % utmrowperiod;
        iband -= 10;
        irow = UTMRow(iband, icol, irow);
        if (irow == maxutmSrow)
            cout<<"Block " <<mgrs.substr(p-2, 2).c_str()<<" not in zone/band " << mgrs.substr(0, p-2).c_str();

        irow = northp1 ? irow : irow + 100;
        icol = icol + minutmcol;
    } else {
        bool eastp = iband & 1;
        icol += eastp ? upseasting : northp1 ? minupsNind : minupsSind;
        irow += northp1 ? minupsNind : minupsSind;
    }
    int prec1 = (len - p)/2;
    double
            unit = tile,
            x1 = unit * icol,
            y1 = unit * irow;
    for (int i = 0; i < prec1; ++i) {
        unit /= base;
        int
                ix = lookup(digits, mgrs[p + i]),
                iy = lookup(digits, mgrs[p + i + prec1]);
        if (ix < 0 || iy < 0)
            cout<<"Encountered a non-digit in " << mgrs.substr(p).c_str();
        x1 += unit * ix;
        y1 += unit * iy;
    }
    if ((len - p) % 2) {
        if (lookup(digits, mgrs[len - 1]) < 0)
            cout<<"Encountered a non-digit in " << mgrs.substr(p).c_str();
        else
            cout<<"Not an even number of digits in "<< mgrs.substr(p).c_str();
    }
    if (prec1 > maxprec)
        cout<<"More than " << 2*maxprec<< " digits in "<<mgrs.substr(p).c_str();
    if (centerp) {
        x1 += unit/2;
        y1 += unit/2;
    }

    MUtmProjection utm;
    double altGeo;
    x1 += zone1*1000000.0;
    utm.nPole = northp1;
    utm.ToGeodetic(x1,y1,0,xGeo,yGeo,altGeo);
}

void MGRS::CheckCoords(bool utmp, bool& northp, double& x, double& y)
{
    // after rounding), it is shifted down by eps.  This also folds UTM
    // northings to the correct N/S hemisphere.
    int
            ix = int(floor(x / tile)),
            iy = int(floor(y / tile)),
            ind = (utmp ? 2 : 0) + (northp ? 1 : 0);
    if (! (ix >= mineasting[ind] && ix < maxeasting[ind]) ) {
        if (ix == maxeasting[ind] && x == maxeasting[ind] * tile)
            x -= eps;
        else
            cout<<"Easting " <<floor(x/1000)
                            << "km not in MGRS/"
                            << (utmp ? "UTM" : "UPS") << " range for "
                            << (northp ? "N" : "S" ) << " hemisphere ["
                            <<mineasting[ind]*tile/1000 << "km, "
                            <<maxeasting[ind]*tile/1000 << "km)";
    }
    if (! (iy >= minnorthing[ind] && iy < maxnorthing[ind]) ) {
        if (iy == maxnorthing[ind] && y == maxnorthing[ind] * tile)
            y -= eps;
        else
            cout<<"Northing " << floor(y/1000)
                            << "km not in MGRS/"
                            << (utmp ? "UTM" : "UPS") << " range for "
                            << (northp ? "N" : "S" ) << " hemisphere ["
                            <<minnorthing[ind]*tile/1000<< "km, "
                            <<maxnorthing[ind]*tile/1000<< "km)";
    }

    // Correct the UTM northing and hemisphere if necessary
    if (utmp) {
        if (northp && iy < minutmNrow) {
            northp = false;
            y += utmNshift;
        } else if (!northp && iy >= maxutmSrow) {
            if (y == maxutmSrow * tile)
                // If on equator retain S hemisphere
                y -= eps;
            else {
                northp = true;
                y -= utmNshift;
            }
        }
    }
}

int MGRS::UTMRow(int iband, int icol, int irow) throw() {
    // Input is MGRS (periodic) row index and output is true row index.  Band
    // index is in [-10, 10) (as returned by LatitudeBand).  Column index
    // origin is easting = 100km.  Returns maxutmSrow if irow and iband are
    // incompatible.  Row index origin is equator.



    double c = 100 * (8 * iband + 4)/double(90);
    bool northp = iband >= 0;
    int
            minrow = iband > -10 ?
                int(floor(c - double(4.3) - double(0.1) * northp)) : -90,
            maxrow = iband <   9 ?
                int(floor(c + double(4.4) - double(0.1) * northp)) :  94,
            baserow = (minrow + maxrow) / 2 - utmrowperiod / 2;
    // Add maxutmSrow = 5 * utmrowperiod to ensure operand is positive
    irow = (irow - baserow + maxutmSrow) % utmrowperiod + baserow;
    if (irow < minrow || irow > maxrow) {
        // Northing = 71*100km and 80*100km intersect band boundaries
        // The following deals with these special cases.
        int
                // Fold [-10,-1] -> [9,0]
                sband = iband >= 0 ? iband : - iband  - 1,
                // Fold [-90,-1] -> [89,0]
                srow = irow >= 0 ? irow : -irow - 1,
                // Fold [4,7] -> [3,0]
                scol = icol < 4 ? icol : -icol + 7;
        if ( ! ( (srow == 70 && sband == 8 && scol >= 2) ||
                 (srow == 71 && sband == 7 && scol <= 2) ||
                 (srow == 79 && sband == 9 && scol >= 1) ||
                 (srow == 80 && sband == 8 && scol <= 1) ) )
            irow = maxutmSrow;
    }
    return irow;
}


/***************************************************************************/


