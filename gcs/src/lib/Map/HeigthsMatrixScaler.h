#ifndef HEIGTHSMATRIXSCALER_H
#define HEIGTHSMATRIXSCALER_H

#include "BaseCl.h"
#include "KartaInterface.h"

#include <QObject>
#include <QVector>
#include <QFuture>

class HeigthsMatrixScaler
{
public:
    HeigthsMatrixScaler(MKartaInterface *iMap,
                        MRect rect,
                        const double &parameter, bool bNative = false);

    HeigthsMatrixScaler();
    ~HeigthsMatrixScaler();

    double matrixLeft;
    double matrixTop;
    double matrixRight;
    double matrixBottom;
    double fileStep;

    short minRectHeight;
    short maxRectHeight;

    struct RestrictedAreaPolygon
    {
        RestrictedAreaPolygon()
            : polygon(),
              maxHeight(-20000.0),
              minHeight(-20000.0),
              radius(0.0),
              dftProb(0.0)
        {
        }

        RestrictedAreaPolygon(std::vector<MPoint> _polygon,
                              short _maxHeight,
                              short _minHeight = 0.0,
                              double _radius = 0.0,
                              double _dftProb = 0.0)
        {
            polygon = _polygon;
            maxHeight = _maxHeight;
            minHeight = _minHeight;
            radius = _radius;
            dftProb = _dftProb;
        }

        std::vector<MPoint> polygon;
        short maxHeight;
        short minHeight;
        double radius;
        double dftProb;
    };

    struct ThreadParameters
    {
        ThreadParameters()
        {
            matrixFilePath = MString();
            leftStart = 0.0;
            topStart = 0.0;
            stepsColumn = 0;
            stepsRow = 0;
        }

        ThreadParameters(MString _matrixFilePath,
                         double _leftStart,
                         double _topStart,
                         int _stepsColumn,
                         int _stepsRow)
        {
            matrixFilePath = _matrixFilePath;
            leftStart = _leftStart;
            topStart = _topStart;
            stepsColumn = _stepsColumn;
            stepsRow = _stepsRow;
        }

        MString matrixFilePath;
        double leftStart;
        double topStart;
        int stepsColumn;
        int stepsRow;
    };

    void StartScaling(MKartaInterface *m_IMap,
                      MRect rect,
                      const double &parameter,
                      bool bNative = false);

    HeigthsMatrixScaler::RestrictedAreaPolygon ReadHeightFromMatrix(ThreadParameters params);
    std::vector<RestrictedAreaPolygon> GetRestrictedPolygon();
    short GetMinRectHeigth();
    short GetMaxRectHeigth();
    void DrawUserRect();

private:
    MKartaInterface *m_IMap;

    std::vector<RestrictedAreaPolygon> restrictedPolygon;
    QList<QFuture<RestrictedAreaPolygon> > futures;
};

#endif // HEIGTHSMATRIXSCALER_H
