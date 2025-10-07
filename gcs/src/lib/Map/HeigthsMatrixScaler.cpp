#include "HeigthsMatrixScaler.h"
#include "ReliefRaster.h"
#include "QtDevice.h"
#include "Globals.h"

#if QT_VERSION >= 0x050000
    #include <QtConcurrent/QtConcurrentRun>
#else
    #include <QtConcurrentRun>
#endif

#include <QFutureSynchronizer>
#include <QDebug>

#define DOUBLE_TO_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))

HeigthsMatrixScaler::HeigthsMatrixScaler(MKartaInterface *iMap,
                                         MRect rect,
                                         const double &parameter, bool bNative) :
    m_IMap(iMap),
    minRectHeight(9999),
    maxRectHeight(-32000)
{
    rect.NormalizeGeoRect();
    StartScaling(iMap, rect, parameter, bNative);
}

HeigthsMatrixScaler::HeigthsMatrixScaler()
    : m_IMap(NULL),
      minRectHeight(9999),
      maxRectHeight(-32000)
{
    //empty
}

HeigthsMatrixScaler::~HeigthsMatrixScaler()
{
    //empty
}

void HeigthsMatrixScaler::StartScaling(MKartaInterface *m_IMap,
                                       MRect rect,
                                       const double &parameter, bool bNative)
{
    MString matrixFilePath = m_IMap->m_karta->m_matrixfile;
    MBmhFILE* pBMHFile = new MBmhFILE(matrixFilePath);
    if(pBMHFile)
    {
        fileStep = pBMHFile->Step();
        const double &minStep = 100.0;
        const double &coeff = (parameter < minStep ? 1.0 : parameter/fileStep);

        const double &rowStart = rect.bottom;
        const double &rowEnd = rect.top;
        const double &columnStart = rect.left;
        const double &columnEnd = rect.right;

        //qDebug() << QString::number(columnStart, 'g', 20);
        //qDebug() << QString::number(rowEnd, 'g', 20);
        //qDebug() << QString::number(columnEnd, 'g', 20);
        //qDebug() << QString::number(rowStart, 'g', 20);

        const double &size = fileStep * coeff;
        const int &intCoeff = DOUBLE_TO_INT(coeff);

        matrixLeft = pBMHFile->Left();
        matrixTop = pBMHFile->Bottom();
        matrixRight = pBMHFile->Right();
        matrixBottom = pBMHFile->Top();

        MRect userRect = rect;
        userRect.NormalizeGeoRect();
        MRect matrixRect = MRect(matrixLeft, matrixTop, matrixRight, matrixBottom);
        matrixRect.NormalizeGeoRect();
        MRect returnRect;

        if(!IntersectionGeoRect(&userRect, &matrixRect, returnRect))
        {
            return;
        }

        rect = returnRect;

        double leftStart = columnStart;
        double topStart = rowStart;
        int stepsColumn = 0;
        int stepsRow = 0;
        int threadCounter = 0;

        QFutureSynchronizer<RestrictedAreaPolygon> synchronizer;

        while(true)
        {
            threadCounter++;

            if(topStart > rowEnd && leftStart < columnEnd)
            {
                topStart -= size;
                stepsColumn = intCoeff;
                stepsRow = DOUBLE_TO_INT((rowEnd - topStart) / fileStep);
                stepsRow = ((stepsRow == 0) ? 1 : stepsRow);
                ThreadParameters params(matrixFilePath, leftStart, topStart, stepsColumn, stepsRow);

                leftStart += size;
                topStart += size;

                if(leftStart < columnEnd)
                    synchronizer.addFuture(QtConcurrent::run(this, &HeigthsMatrixScaler::ReadHeightFromMatrix, params));
                else
                    threadCounter--;
            }
            else if(leftStart > columnEnd && topStart < rowEnd)
            {
                leftStart -= size;
                stepsColumn = DOUBLE_TO_INT((columnEnd - leftStart) / fileStep);
                stepsColumn = ((stepsColumn == 0) ? 1 : stepsColumn);
                stepsRow = intCoeff;
                ThreadParameters params(matrixFilePath, leftStart, topStart, stepsColumn, stepsRow);

                topStart += size;
                leftStart = columnStart;

                if(topStart < rowEnd)
                    synchronizer.addFuture(QtConcurrent::run(this, &HeigthsMatrixScaler::ReadHeightFromMatrix, params));
                else
                    threadCounter--;
            }
            else if(topStart > rowEnd && leftStart > columnEnd)
            {
                topStart -= size;
                leftStart -= size;
                stepsRow = DOUBLE_TO_INT((rowEnd - topStart) / fileStep);
                stepsRow = ((stepsRow == 0) ? 1 : stepsRow);
                stepsColumn = DOUBLE_TO_INT((columnEnd - leftStart) / fileStep);
                stepsColumn = ((stepsColumn == 0) ? 1 : stepsColumn);

                ThreadParameters params(matrixFilePath, leftStart, topStart, stepsColumn, stepsRow);
                synchronizer.addFuture(QtConcurrent::run(this, &HeigthsMatrixScaler::ReadHeightFromMatrix, params));
                break;
            }
            else if(topStart < rowEnd && leftStart < columnEnd)
            {
                stepsColumn = intCoeff;
                stepsRow = intCoeff;
                ThreadParameters params(matrixFilePath, leftStart, topStart, stepsColumn, stepsRow);

                leftStart += size;

                if(leftStart < columnEnd && topStart + size < rowEnd)
                    synchronizer.addFuture(QtConcurrent::run(this, &HeigthsMatrixScaler::ReadHeightFromMatrix, params));
                else
                    threadCounter--;
            }
            else
                threadCounter--;
        }

        //qDebug() << threadCounter << " segments generated.";

        futures = synchronizer.futures();
        QList<QFuture<RestrictedAreaPolygon> >::iterator it = futures.begin();
        while(it != futures.end())
        {

            RestrictedAreaPolygon polyg = it->result();
            if(bNative == true)
            {
                std::vector<MPoint> points = polyg.polygon;
                polyg.polygon.clear();
                for_each(points.begin(), points.end(), [&polyg, m_IMap](MPoint point){polyg.polygon.push_back(m_IMap->ConvertPoint(point, false));});
            }

            restrictedPolygon.push_back(polyg);
            if(polyg.maxHeight == -32000)
            {
                qDebug()<<"-32000 height added";
            }
            ++it;
        }

        delete pBMHFile;
        pBMHFile = NULL;
    }
}

HeigthsMatrixScaler::RestrictedAreaPolygon HeigthsMatrixScaler::ReadHeightFromMatrix(ThreadParameters params)
{
    HeigthsMatrixScaler::RestrictedAreaPolygon s_restrAreaPolyg;
    MBmhFILE* pBMHFile = new MBmhFILE(params.matrixFilePath);
    if(pBMHFile)
    {
        double leftStart = params.leftStart;
        double topStart = params.topStart;
        int stepsColumn = params.stepsColumn;
        int stepsRow = params.stepsRow;

        double right = params.leftStart + fileStep * params.stepsColumn;
        double bottom = params.topStart + fileStep * params.stepsRow;

        MRect userRect = MRect(leftStart, topStart, right, bottom);
        userRect.NormalizeGeoRect();
        MRect matrixRect = MRect(matrixLeft, matrixTop, matrixRight, matrixBottom);
        matrixRect.NormalizeGeoRect();
        MRect returnRect;
        IntersectionGeoRect(&userRect, &matrixRect, returnRect);

        if(!returnRect.IsGeoRectEmpty())
        {
            delete pBMHFile;
            pBMHFile = NULL;
            //qDebug()<<"if(!returnRect.IsGeoRectEmpty())";
            return s_restrAreaPolyg;
        }

        if(userRect != returnRect)
        {
            leftStart = returnRect.left;
            topStart = returnRect.top;
            stepsColumn = DOUBLE_TO_INT(returnRect.Width() / fileStep);
            stepsRow = DOUBLE_TO_INT(returnRect.Height() / fileStep);
        }

        if(stepsColumn > 0 && stepsRow > 0)
        {
            MPoint leftTop = MPoint(leftStart, topStart);
            MPoint rightTop = MPoint(leftStart + stepsColumn * fileStep, topStart);
            MPoint rightBottom = MPoint(leftStart + stepsColumn * fileStep, topStart + stepsRow * fileStep);
            MPoint leftBottom = MPoint(leftStart, topStart + stepsRow * fileStep);

            short returnHeight = -32000;
            short currentHeight = 0.0;

            for(int j = 0; j < stepsRow; j++)
            {
                MPoint crntPoint(leftStart, topStart);
                int bytesRead;
                char* arr = pBMHFile->GetHeightsRow(crntPoint, stepsColumn, bytesRead);
                int shortsRead = (int)(bytesRead/2);
                short* shortArr = (short*)arr;

                for(int i = 0; i < shortsRead; i++)
                {
                    currentHeight = (*shortArr == 9999) ? -20000 : *shortArr;
                    returnHeight = (currentHeight > returnHeight ? currentHeight : returnHeight);

                    if(currentHeight == -20000)
                    {
                        //qDebug()<<"currentHeight == -20000";
                    }
                    else
                    {
                        //qDebug()<<"minRectHeight = "<<minRectHeight;
                        minRectHeight = (minRectHeight > currentHeight ? currentHeight : minRectHeight);
                        //qDebug()<<"minRectHeight___ = "<<minRectHeight;
                        //qDebug()<<"maxRectHeight = "<<maxRectHeight;
                        maxRectHeight = (maxRectHeight < currentHeight ? currentHeight : maxRectHeight);
                        //qDebug()<<"maxRectHeight___ = "<<maxRectHeight;
                    }
                    shortArr++;
                }
                topStart += fileStep;
            }

            std::vector<MPoint> pointsVector;
            pointsVector.push_back(leftTop);
            pointsVector.push_back(rightTop);
            pointsVector.push_back(rightBottom);
            pointsVector.push_back(leftBottom);

            s_restrAreaPolyg.maxHeight = returnHeight;

            if(s_restrAreaPolyg.maxHeight == -32000)
            {
                //qDebug()<<"s_restrAreaPolyg.maxHeight = -32000";
            }
            s_restrAreaPolyg.polygon = pointsVector;
        }
        else
        {
            delete pBMHFile;
            pBMHFile = NULL;

            //qDebug()<<"else(stepsColumn > 0 && stepsRow > 0)";
            return s_restrAreaPolyg;
        }

        delete pBMHFile;
        pBMHFile = NULL;
    }
    return s_restrAreaPolyg;
}

void HeigthsMatrixScaler::DrawUserRect()
{
    const short &minRectHeight = GetMinRectHeigth();
    const short &maxRectHeight = GetMaxRectHeigth();
    int layer = 666; // ??? specify layer id here

    if (!m_IMap->AddActLayer(layer))
        m_IMap->FindActLayer(layer)->DeleteObjects();

    m_IMap->SetWorkingActLayer(layer);

    int cnt = 0;
    std::vector<RestrictedAreaPolygon> restrPolygonsVector = GetRestrictedPolygon();
    std::vector<RestrictedAreaPolygon>::iterator it = restrPolygonsVector.begin();
    while(it != restrPolygonsVector.end())
    {
        cnt++;
        MPoint privRoute(0, 0);
        RestrictedAreaPolygon polygon = *it;
        const short &height = polygon.maxHeight;

        MString kkod = "UZN 5000 RP 195";

        m_IMap->AddObject(cnt);
        m_IMap->SetWorkingObject(cnt);
        m_IMap->SetZnak(kkod);
        MTopobject* tpo = m_IMap->FindObject(cnt);

//        int count = 0;
        MPtArray pta;
        for(int i = 0; i < polygon.polygon.size(); i++)
        {
            MPoint ptRoute = polygon.polygon.at(i);
            privRoute += ptRoute;
            pta.AddTail(&ptRoute);
//            count++;
        }
        polygon.polygon.clear();
        tpo->SetPoints(pta);

//        double x = privRoute.x/count;
//        double y = privRoute.y/count;
//        privRoute = MPoint(x, y);

//        MString str(QString::number(polygon.maxHeight).toStdString());

        MTopobject* pTpo = m_IMap->FindObject(cnt);
        pTpo->bDrawColorZalivka = true;
//        pTpo->mDrawColorZalivka = MReliefRaster::GetColor(height, minRectHeight, maxRectHeight);
        pTpo->mDrawColorZalivka = MReliefRaster::GetColor1(height, minRectHeight, maxRectHeight);

//        pTpo->SetDataPoleByName(str, "formular");
//        m_IMap->SetPriv(privRoute);

        ++it;
    }
}

std::vector<HeigthsMatrixScaler::RestrictedAreaPolygon> HeigthsMatrixScaler::GetRestrictedPolygon()
{
    return restrictedPolygon;
}

short HeigthsMatrixScaler::GetMinRectHeigth()
{
    return minRectHeight;
}

short HeigthsMatrixScaler::GetMaxRectHeigth()
{
    return maxRectHeight;
}
