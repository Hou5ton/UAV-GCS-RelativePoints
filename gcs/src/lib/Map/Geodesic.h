//PACKAGE : MAP.
//FILE: Geodesic.h   
//AUTHOR:
//DESCRIPTION:

#if !defined(GEODESIC_HPP)
#define GEODESIC_HPP 


#include <limits>
#include <algorithm>
#include <math.h>
#include "Globals.h"

//#define pi 3.141592653589793238462643383279502884L
    

  class MGeodesicLine;

  class MGeodesic {
  private:
   
    friend class MGeodesicLine;
    static const int tauord = (sizeof(double) == sizeof(double) ? 5 : sizeof(double) == sizeof(float) ? 4 : 6);//GEOD_TAU_ORD;
    static const int ntau = tauord;
    static const int nsig = tauord;
    static const int zetord = (sizeof(double) == sizeof(double) ? 5 : sizeof(double) == sizeof(float) ? 4 : 6);//GEOD_TAU_ORD;
    static const int nzet = zetord;
    static const int etaord = (sizeof(double) == sizeof(double) ? 5 : sizeof(double) == sizeof(float) ? 4 : 6);//GEOD_ETA_ORD;
    // etaCoeff is multiplied by etaFactor which is O(f), so we reduce the
    // order to which etaCoeff is computed by 1.
    static const int neta = etaord > 0 ? etaord - 1 : 0;
    static const unsigned maxit = 50;

    static inline double sq(double x) throw() { return x * x; }
    void Lengths(double k1, double sig12,
                 double ssig1, double csig1, double ssig2, double csig2,
                 double cbet1, double cbet2,
                 double& s12s, double& m12a, double& m0,
                 double tc[], double zc[]) const throw();
    static void Evolute(double R, double z, double& c, double& s) throw();
    void InverseStart(double sbet1, double cbet1, double sbet2, double cbet2,
                      double lam12, double slam12, double clam12,
                      double& salp1, double& calp1,
                      double tc[], double zc[]) const throw();
    double Lambda12(double sbet1, double cbet1, double sbet2, double cbet2,
                  double salp1, double calp1,
                  double& salp2, double& calp2, double& sig12,
                  double& ssig1, double& csig1, double& ssig2, double& csig2,
                  double& k1, bool diffp, double& dlam12,
                  double tc[], double zc[], double ec[])
      const throw();

    static const double eps2, tol0, tol1, tol2, xthresh;
    const double _a, _r, _f, _f1, _e2, _ep2, _n, _b;
    static double SinSeries(double sinx, double cosx, const double c[], int n)
      throw();
    static inline double AngNormalize(double x) throw() {
      // Place angle in [-180, 180).  Assumes x is in [-540, 540).
      return x >= 180 ? x - 360 : x < -180 ? x + 360 : x;
    }
    static inline double AngRound(double x) throw() {
      // The makes the smallest gap in x = 1/16 - nextafter(1/16, 0) = 1/2^57
      // for reals = 0.7 pm on the earth if x is an angle in degrees.  (This
      // is about 1000 times more resolution than we get with angles around 90
      // degrees.)  We use this to avoid having to deal with near singular
      // cases when x is non-zero but tiny (e.g., 1.0e-200).
      const double z = double(0.0625); // 1/16
      volatile double y = fabs(x);
      // The compiler mustn't "simplify" z - (z - y) to y
      y = y < z ? z - (z - y) : y;
      return x < 0 ? -y : y;
    }
    static inline void SinCosNorm(double& sinx, double& cosx) throw() {
      double r = sqrt(pow(sinx,2)+ pow(cosx,2));
      sinx /= r;
      cosx /= r;
    }
	static inline double cbrt(double x) throw() {
      double y = pow(fabs(x), 1/double(3)); // Return the double cube root
      return x < 0 ? -y : y;
    }

    // These are Maxima generated functions to provide series approximations to
    // the integrals for the ellipsoidal geodesic.
    static double tauFactorm1(double k1) throw();
    static void tauCoeff(double k1, double t[]) throw();
    static void sigCoeff(double k1, double tp[]) throw();
    static double zetFactorm1(double k1) throw();
    static void zetCoeff(double k1, double t[]) throw();
    static double etaFactor(double f, double k1) throw();
    static void etaCoeff(double f, double k1, double h[]) throw();

  public:

    /**
     * Constructor for a ellipsoid with equatorial radius \e a (meters) and
     * reciprocal flattening \e r.  Setting \e r = 0 implies \e r = inf or
     * flattening = 0 (i.e., a sphere).  Negative \e r indicates a prolate
     * ellipsoid.  An exception is thrown if \e a is not positive.
     **********************************************************************/
    MGeodesic(double a = axis_X, double r = axis_Y);

    /**
     * Perform the direct geodesic calculation.  Given a latitude, \e lat1,
     * longitude, \e lon1, and azimuth \e azi1 (degrees) for point 1 and a
     * range, \e s12 (meters) from point 1 to point 2, return the latitude, \e
     * lat2, longitude, \e lon2, and forward azimuth, \e azi2 (degrees) for
     * point 2 and the reduced length \e m12 (meters).  If \e arcmode (default
     * false) is set to true, \e s12 is interpreted as the arc length \e a12
     * (degrees) on the auxiliary sphere.  An arc length greater that 180
     * degrees results in a geodesic which is not a shortest path.  For a
     * prolate ellipsoid, an additional condition is necessary for a shortest
     * path: the longitudinal extent must not exceed of 180 degrees.  Returned
     * value is the arc length \e a12 (degrees) if \e arcmode is false,
     * otherwise it is the distance \e s12 (meters)
     **********************************************************************/
    double Direct(double lat1, double lon1, double azi1, double s12,
                      double& lat2, double& lon2, double& azi2) const throw();

    /**
     * Perform the inverse geodesic calculation.  Given a latitude, \e lat1,
     * longitude, \e lon1, for point 1 and a latitude, \e lat2, longitude, \e
     * lon2, for point 2 (all in degrees), return the geodesic distance, \e s12
     * (meters), the forward azimuths, \e azi1 and \e azi2 (degrees) at points
     * 1 and 2, and the reduced length \e m12 (meters).  Returned value is the
     * arc length \e a12 (degrees) on the auxiliary sphere.  The routine uses
     * an iterative method.  If the method fails to converge, the negative of
     * the distances (\e s12, \e m12, and \e a12) and reverse of the azimuths
     * are returned.  This is not expected to happen with ellipsoidal models of
     * the earth.  Please report all cases where this occurs.
     **********************************************************************/
    double Inverse(double lat1, double lon1, double lat2, double lon2,
                       double& s12, double& azi1, double& azi2)
      const throw();

	double CalcSpheroidalArea(MPtArray *pta) const throw();
    /**
     * Set up to do a series of ranges.  This returns a MGeodesicLine object
     * with point 1 given by latitude, \e lat1, longitude, \e lon1, and azimuth
     * \e azi1 (degrees).  Calls to MGeodesicLine::Position return the
     * position and azimuth for point 2 a specified distance away.  Using
     * MGeodesicLine::Position is approximately 2.1 faster than calling
     * MGeodesic::Direct.
     **********************************************************************/
    MGeodesicLine Line(double lat1, double lon1, double azi1)
      const throw();

    /**
     * The major radius of the ellipsoid (meters).  This is that value of \e a
     * used in the constructor.
     **********************************************************************/
    double MajorRadius() const throw() { return _a; }

    /**
     * The inverse flattening of the ellipsoid.  This is that value of \e r
     * used in the constructor.  A value of 0 is returned for a sphere
     * (infinite inverse flattening).
     **********************************************************************/
    double InverseFlattening() const throw() { return _r; }

    /**
     * A global instantiation of MGeodesic with the parameters for the WGS84
     * ellipsoid.
     **********************************************************************/
    const static MGeodesic WGS84;
  };

  /**
   * \brief A geodesic line.
   *
   * MGeodesicLine facilitates the determination of a series of points on a
   * single geodesic.  MGeodesic.Line returns a MGeodesicLine object with the
   * geodesic defined by by \e lat1, \e lon1, and \e azi1.
   * MGeodesicLine.Position returns the \e lat2, \e lon2, \e azi2, and \e m12
   * given \e s12.  An example of use of this class is:
   \verbatim
   // Print positions on a geodesic going through latitude 30,
   // longitude 10 at azimuth 80.  Points at intervals of 10km
   // in the range [-1000km, 1000km] are given.
   GeographicLib::MGeodesicLine
     line(GeographicLib::MGeodesic::WGS84.Line(30.0, 10.0, 80.0));
   double step = 10e3;
   for (int s = -100; s <= 100; ++s) {
     double lat2, lon2, azi2, m12;
     double s12 = s * step;
     line.Position(s12, lat2, lon2, azi2, m12);
     cout << s12 << " " << lat2 << " " << lon2 << " "
          << azi2 << " " << m12 << "\n";
   }
   \endverbatim
   * The default copy constructor and assignment operators work with this
   * class, so that, for example, the previous example could start
   \verbatim
   GeographicLib::MGeodesicLine line;
   line = GeographicLib::MGeodesic::WGS84.Line(30.0, 10.0, 80.0);
   ...
   \endverbatim
   * Similarly, a vector can be used to hold MGeodesicLine objects.
   *
   * The calculations are accurate to better than 12 nm.  (See \ref geoderrors
   * for details.)
   **********************************************************************/

  class MGeodesicLine {
  private:
   
    friend class MGeodesic;
    static const int ntau = MGeodesic::ntau;
    static const int nsig = MGeodesic::nsig;
    static const int nzet = MGeodesic::nzet;
    static const int neta = MGeodesic::neta;

    double _lat1, _lon1, _azi1;
    double _a, _r,  _b, _f1, _salp0, _calp0, _u2,
      _ssig1, _csig1, _stau1, _ctau1, _somg1, _comg1,
      _taufm1, _zetfm1, _etaf, _dtau1, _dzet1, _dlam1;
    double _tauCoeff[ntau ? ntau : 1], _sigCoeff[nsig ? nsig : 1],
      _zetCoeff[nzet ? nzet : 1], _etaCoeff[neta ? neta : 1];

    static inline double sq(double x) throw() { return x * x; }
    MGeodesicLine(const MGeodesic& g, double lat1, double lon1, double azi1)
      throw();
  public:

    /**
     * A default constructor.  If MGeodesicLine::Position is called on the
     * resulting object, it returns immediately (without doing any
     * calculations).  The object should be set with a call to MGeodesic::Line.
     * Use Init() to test whether object is still in this uninitialized state.
     **********************************************************************/
    MGeodesicLine() throw() : _b(0) {};

    /**
     * Return the latitude, \e lat2, longitude, \e lon2, and forward azimuth,
     * \e azi2 (degrees) of the point 2 which is a distance, \e s12 (in
     * meters), from point 1.  Also return the reduced length \e m12 (meters).
     * \e s12 can be signed.  If \e arcmode (default false) is set to true, \e
     * s12 is interpreted as the arc length \e a12 (in degrees) on the
     * auxiliary sphere.  Returned value is the arc length \e a12 (degrees) if
     * \e arcmode is false, otherwise it is the distance \e s12 (meters).
     **********************************************************************/
    double Position(double s12, double& lat2, double& lon2, double& azi2) ;

    /**
     * Has this object been initialized so that Position can be called?
     **********************************************************************/
    bool Init() const throw() { return _b > 0; }

    /**
     * Return the latitude of point 1 (degrees).
     **********************************************************************/
    double Latitude() const throw() { return Init() ? _lat1 : 0; }

    /**
     * Return the longitude of point 1 (degrees).
     **********************************************************************/
    double Longitude() const throw() { return Init() ? _lon1 : 0; }

    /**
     * Return the azimuth (degrees) of the geodesic line as it passes through
     * point 1.
     **********************************************************************/
    double Azimuth() const throw() { return Init() ? _azi1 : 0; }

    /**
     * The major radius of the ellipsoid (meters).  This is that value of \e a
     * inherited from the MGeodesic object used in the constructor.
     **********************************************************************/
    double MajorRadius() const throw() { return Init() ? _a : 0; }

    /**
     * The inverse flattening of the ellipsoid.  This is that value of \e r
     * inherited from the MGeodesic object used in the constructor.  A value of
     * 0 is returned for a sphere (infinite inverse flattening).
     **********************************************************************/
    double InverseFlattening() const throw() { return Init() ? _r : 0; }
  };

#endif
