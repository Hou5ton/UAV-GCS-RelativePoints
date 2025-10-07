#ifndef MGRS_H
#define MGRS_H

#include "MGRS_global.h"
#include <string>
#include <cmath>


//PACKAGE : MAP.
//CLASS	: MGRS .
//DESCRIPTION : Class - Military grid reference system
class MGRS_EXPORT MGRS
{

private:

    // The smallest length s.t., 1.0e7 - eps < 1.0e7 (approx 1.9 nm)
    static const double eps;
    // The smallest angle s.t., 90 - eps < 90 (approx 50e-12 arcsec)
    static const double angeps;
    static const std::string hemispheres;
    static const std::string utmcols[3];
    static const std::string utmrow;
    static const std::string upscols[4];
    static const std::string upsrows[2];
    static const std::string latband;
    static const std::string upsband;
    static const std::string digits;

    static const int mineasting[4];
    static const int maxeasting[4];
    static const int minnorthing[4];
    static const int maxnorthing[4];
    enum {
        base = 10,
        // Top-level tiles are 10^5 m = 100km on a side
        tilelevel = 5,
        // Period of UTM row letters
        utmrowperiod = 20,
        // Row letters are shifted by 5 for even zones
        utmevenrowshift = 5,
        // Maximum precision is um
        maxprec = 5 + 6
    };
    static void CheckCoords(bool utmp, bool& northp, double& x, double& y);
    static int lookup(const std::string& s, char c) throw() {
        std::string::size_type r = s.find(toupper(c));
        return r == std::string::npos ? -1 : int(r);
    }

    static int UTMRow(int iband, int icol, int irow) throw();

    static int LatitudeBand(double lat) throw() {
        int ilat = int(floor(lat));
        return (std::max)(-10, (std::min)(9, (ilat + 80)/8 - 10));
    }
    // UTMUPS access these enums
    enum {
        tile = 100000,            // Size MGRS blocks
        minutmcol = 1,
        maxutmcol = 9,
        minutmSrow = 10,
        maxutmSrow = 100,         // Also used for UTM S false northing
        minutmNrow = 0,           // Also used for UTM N false northing
        maxutmNrow = 95,
        minupsSind = 8,           // These 4 ind's apply to easting and northing
        maxupsSind = 32,
        minupsNind = 13,
        maxupsNind = 27,
        upseasting = 20,          // Also used for UPS false northing
        utmeasting = 5,           // UTM false easting
        // Difference between S hemisphere northing and N hemisphere northing
        utmNshift = (maxutmSrow - minutmNrow) * tile
    };
    MGRS();                     // Disable constructor

public:

    //PACKAGE : MAP.
    //FUNCTION : MGRS::convertFromGeo( MPoint& geoCoordinates, int prec=5);
    //DESCRIPTION : Converts from coordinates to string.
    //INPUTS : MPoint& , int .
    //RETURNS : MString.
    static	std::string convertFromGeo( const double &xGeo, const double &yGeo, int prec=5);
    //static std::string GetBand(double &xGeo,double &yGeo);
    static std::string GetTileString(double &xGeo, double &yGeo, bool needTile = false);// letter designation of the square 100 km

    ///PACKAGE : MAP.
    //FUNCTION : MGRS::convertToGeo( MString mgrs, MPoint& geoPt, bool centerp);
    //DESCRIPTION : Converts from string to coordinates.
    //INPUTS : MString , MPoint& , bool .
    //RETURNS : NONE.
    static	void convertToGeo(std::string & mgrs, double &xGeo, double &yGeo, bool centerp );

};


#endif // MGRS_H
