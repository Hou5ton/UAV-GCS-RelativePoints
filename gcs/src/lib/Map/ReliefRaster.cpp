#include "ReliefRaster.h"
#include "QtDevice.h"
#include "Globals.h"
//#include <QDebug>
#if QT_VERSION >= 0x050000
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtConcurrentRun>
#endif
#include "BaseCl.h"
#include <QDebug>
#include <QFutureSynchronizer>



MReliefRaster::MHeightMap::MHeightMap() : m_size(0, 0), m_heights(NULL)
{
}

MReliefRaster::MHeightMap::~MHeightMap()
{
    for (int i = 0; i < m_size.height(); i++)
        delete m_heights[i];
    delete m_heights;
}

void MReliefRaster::MHeightMap::Realloc(QSize newSize)
{
    for (int i = 0; i < m_size.height(); i++)
        delete m_heights[i];
    delete m_heights;

    m_size = newSize;
    m_heights = new int *[m_size.height()];
    for (int i = 0; i < m_size.height(); i++)
        m_heights[i] = new int[m_size.width()];
}

QSize MReliefRaster::MHeightMap::GetSize()
{
    return m_size;
}

bool MReliefRaster::MHeightMap::Update(MPlotDevice *plotDevice,/* MRect m_bounds, MRect geoBounds,*/
                                       MKartaInterface *m_IMap, bool drawShader, /*MRect &m_userRect, */MRect logIntersection)//logIntersection
{

    bool bResult = false;
    //QTime t;
    //t.start();

    /*MRect preIntersection;
    IntersectionGeoRect(&geoBounds, &m_bounds, preIntersection);

    double left = m_IMap->GetKarta()->m_bmhfile->Left();
    double top = m_IMap->GetKarta()->m_bmhfile->Bottom();
    double right = m_IMap->GetKarta()->m_bmhfile->Right();
    double bottom = m_IMap->GetKarta()->m_bmhfile->Top();

    MRect drawBounds = MRect(left, top, right, bottom);

    drawBounds.NormalizeGeoRect();

    MRect intersection;
    IntersectionGeoRect(&drawBounds, &preIntersection, intersection);*/
    if(true)
    {
        bResult = true;

        // m_userRect = intersection;

        //MRect intersection = m_userRect;

        //MRect logIntersection;
        //plotDevice->BumToLogic(&m_userRect, logIntersection);

        QRect scrBounds = QRect(logIntersection.left, logIntersection.bottom, logIntersection.right, logIntersection.top);
        QSize size = QSize(fabs(logIntersection.left - logIntersection.right), fabs(logIntersection.top - logIntersection.bottom));

        QSize allBounds = QSize(m_IMap->GetKarta()->m_bmhfile->MxM(), m_IMap->GetKarta()->m_bmhfile->MxN()); // size of a matrix of heights
        // step in pixels for receiving the following point from a matrix of heights
        double stepWidth = max(2.0 - drawShader, (double)(abs(scrBounds.width())*1.0 / allBounds.width()));
        double stepHeight = max(2.0 - drawShader, (double)(abs(scrBounds.height())*1.0 / allBounds.height()));

        // the total size of the image to which the relief is brought
        size.setWidth(size.width() / stepWidth);
        size.setHeight(size.height() / stepHeight);

        Realloc(size);

        MString matrixFilePath = m_IMap->m_karta->m_matrixfile;
        double topStart = logIntersection.top;
        double leftStart = logIntersection.left;
        int rowCount = size.width();
        //-----------------------------------------------------------------------
        const int threadCount = size.height();

        QFutureSynchronizer<void> synchronizer;

        for (int i = 0; i < threadCount; i++)
        {
            ParamsThread params(matrixFilePath, leftStart, topStart, rowCount, stepWidth);
            synchronizer.addFuture(QtConcurrent::run(this, &MReliefRaster::MHeightMap::ReadHeightFromMatrixes,
                                                     params,
                                                     plotDevice,
                                                     i,
                                                     m_heights));
            topStart += stepHeight;
        }

        synchronizer.waitForFinished();
    }

    return bResult;
}

void MReliefRaster::MHeightMap::ReadHeightFromMatrixes(const ParamsThread &params,
                                                       MPlotDevice *plotDevice,
                                                       const int &j,
                                                       int **result)
{
    MBmhFILE* pBMHFile = new MBmhFILE(params.matrixPath);
    if(pBMHFile)
    {
        double m = params.topStart;
        double k = params.leftStart;

        for(int i = 0; i < params.rowCount; i++)
        {
            double returnHeight = 0.0;
            MPoint crntPoint(k, m);
            MPoint outPoint;
            plotDevice->LogicToBum(crntPoint, outPoint);

            int value = pBMHFile->GetHeight(outPoint);

            ((int)value==BMH_NOVALUE)? returnHeight =
                    BMH_OUTOFBOUNDS : returnHeight = (int)value;

            result[j][i] = returnHeight;
            k += params.stepWidth;
        }

        delete pBMHFile;
        pBMHFile = NULL;
    }
}

int MReliefRaster::MHeightMap::GetHeight(int row, int column)
{
    if ((row >= 0) && (row < m_size.height()) && (column >= 0) && (column < m_size.width()))
        return m_heights[row][column];
    return -20000;
}

int MReliefRaster::MHeightMap::GetMaxHeight()
{
    int temp = -999;
    for (int i = 0; i < m_size.height(); i++)
        for (int j = 0; j < m_size.width(); j++)
            if (m_heights[i][j] > temp && m_heights[i][j] != 9999)
                temp = m_heights[i][j];
    return temp;
}

int MReliefRaster::MHeightMap::GetMinHeight()
{
    int temp = 10000;
    for (int i = 0; i < m_size.height(); i++)
        for (int j = 0; j < m_size.width(); j++)
            if (m_heights[i][j] < temp && m_heights[i][j] != 9999 && m_heights[i][j] != -20000)
                temp = m_heights[i][j];
    return temp;
}

MReliefRaster::MReliefRaster(const MRect& position, MKartaInterface *IMap, bool drawShader)	: MRaster(position)
{
    saveRect = MRect();
    saveRect_pixel = MRect();
    this->m_IMap = IMap;
    this->drawShader = drawShader;
    this->alpha = 100;
    m_WillBeUpdated = true;
}

MReliefRaster::~MReliefRaster()
{
}

void MReliefRaster::MYsetColor(double minHeight, double maxHeight, QColor& rgb,int ht)
{
    //    MColor color = GetColor(ht, minHeight, maxHeight);
    MColor color = GetColor1(ht, minHeight, maxHeight);
    double red = color.GetR()*1.0/255.0;
    double green = color.GetG()*1.0/255.0;
    double blue = color.GetB()*1.0/255.0;

    rgb.setRgbF(red, green, blue);
}

void MReliefRaster::MYsetTranspColor(double minHeight, double maxHeight, QColor& rgb, double transp, int ht)
{
    MColor color = GetTranspColor(ht, minHeight, maxHeight, transp);
    double red = color.GetR()*1.0/255.0;
    double green = color.GetG()*1.0/255.0;
    double blue = color.GetB()*1.0/255.0;
    double alpha = color.GetA()*1.0/255.0;

    rgb.setRgbF(red, green, blue, alpha);
}

MColor MReliefRaster::GetColor(double height, double minHght, double maxHght, int scale)
{
    //   int hue = (int) 200*(fabs(maxHght-height)/fabs(maxHght-minHght));
    //   QColor cHsv = QColor::fromHsv(hue, 200, 200);
    //   return MColor(cHsv.red(),cHsv.green(),cHsv.blue());

    int m_maxH = maxHght + 50;
    int i_minH = minHght - 10;

    QColor rgb;

    int ht = (int)height / scale;

    if((m_maxH-ht) <0)
    {
        rgb.setRgb(255, 0, 0);
        return MColor(rgb.red(), rgb.green(), rgb.blue());
    }

    int d33=(m_maxH-i_minH);
    double doR=0.50 + 0.10 *(d33/2000);
    double doG = 1.0-doR;
    int colorR,colorG;

    if ((m_maxH-ht)<=(d33*doR))
    {
        double colR=0;
        colR=(doR-0.4)*100.0;
        colorR=240-(int)((double)(ht-i_minH)*colR/(d33*doR));
        colorG=(int)((double)(m_maxH-ht)*220.0/(d33*doR));
        rgb.setRgb(colorR,colorG,70);
    }
    else
    {
        if ((m_maxH-ht)<=(d33*0.993))
        {
            colorG=180 + (int)((double)(ht-i_minH)*60.0/(d33*doG));
            colorR=(int)((double)(ht-i_minH)*240.0/(d33*doG));
            rgb.setRgb(colorR,colorG,50);
        }
        else
        {
            //colorR= 1;
            colorG=180 + (int)((double)(ht-i_minH)*60.0/(d33*doG));
            colorR=(int)((double)(ht-i_minH)*240.0/(d33*doG));
            //rgb.setRgb(0, 70, 0);
            rgb.setRgb(colorR,colorG,50);
        }
    }
    return MColor(rgb.red(), rgb.green(), rgb.blue());
}

MColor MReliefRaster::GetColor1(double height, double minHght, double maxHght)
{
    int dR0 = -255;
    int dG0 = 0;
    int dB0 = 0;

    int dR1 = 0;
    int dG1 = 255;
    int dB1 = 0;

    double k = (height - minHght) / (maxHght - minHght + 1) * 2;

    if(k < 1)
        return MColor(0 - dR0*k, 255, 0);
    else
        return MColor(255, 255 - dG1*(k-1), 0);


}

MColor MReliefRaster::GetTranspColor(double height, double minHght, double maxHght, double transp, int scale)
{
    MColor resultColor = MColor();

    transp = transp > 1.0 ? 1.0 : transp;
    transp = transp < 0.1 ? 0.1 : transp;
    transp = (int)(transp * 255);

    //    MColor color = GetColor(height, minHght, maxHght, scale);
    MColor color = GetColor1(height, minHght, maxHght);
    resultColor = MColor(color.GetR(), color.GetG(), color.GetB(), transp);
    return resultColor;
}

void MReliefRaster::setShader(bool draw)
{
    drawShader = draw;
    m_WillBeUpdated = true;
}

double MReliefRaster::dMinHeight()
{
    return m_heights.GetMinHeight();
}

double MReliefRaster::dMaxHeight()
{
    return m_heights.GetMaxHeight();
}

bool MReliefRaster::getShaderState()
{
    return drawShader;
}

void MReliefRaster::DrawImagePart(/*MPlotDevice* pDevice, */int rowStart, int rowEnd, double sqrStepWidthInMetres, double minHeight, double maxHeight)
{
    int hPrev;
    //    double minHeight = m_heights.GetMinHeight();
    //    double maxHeight = m_heights.GetMaxHeight();

    const int dWidth = pMapSz.width();

    for (int j = rowStart; j < rowEnd; j++)
    {
        int height = 9999;
        bool redrawPrev = false;
        double deltaH = 0;
        uint *scanLine = (uint *)pMap->scanLine(j);

        for (int i = 0; i < dWidth; i++)
        {
            hPrev = height;
            height = m_heights.GetHeight(j, i);
            if ((height != -20000) && (height != 9999))
            {
                QColor rgb;
                MYsetColor(minHeight, maxHeight, rgb, height);
                //MYsetTranspColor(minHeight, maxHeight, rgb, (alpha*1.0)/255.0, height);
                // We check, whether copying of the previous pixel is required (for removal of strips on a joint of 2 columns of a matrix of heights)
                if (deltaH == 0)
                    redrawPrev = true;
                else
                    redrawPrev = false;

                // We calculate a lighting inclination cosine of the angle to the current piece of a relief
                if ((drawShader) && (hPrev != -20000) && (hPrev != 9999))
                {
                    deltaH = height - hPrev;
                    double cs = deltaH / sqrt(deltaH * deltaH + sqrStepWidthInMetres);

                    // Depending on value of a cosine illumination of pixel is established
                    if (cs > 0)
                    {
                        int f = cs * 1000;
                        if (f > 0) rgb = rgb.darker(f + 100);
                    }
                    else
                    {
                        int f = -cs * 50;
                        if (f > 0) rgb = rgb.lighter(f + 100);
                    }
                }

                rgb.setAlpha(alpha/100.0*255.0);


                // We get rid of strips on a joint of two columns of a matrix of heights
                if ((redrawPrev) && (drawShader) &&  (i > 1))
                {
                    QColor prevRgb = QColor(scanLine[i-2]/*pMap->pixel(i - 2, j)*/);
                    QColor c = QColor(rgb.red() / 2 + prevRgb.red() / 2, rgb.green() / 2 + prevRgb.green() / 2,
                                      rgb.blue() / 2 + prevRgb.blue() / 2/*, 200*/);
                    //pMap->setPixel(i - 1, j, c.rgba());
                    scanLine[i-1] = c.rgba();
                }
                // We draw the current pixel of the picture
                //pMap->setPixel(i, j, rgb.rgba());
                scanLine[i] = rgb.rgba();

            }
        }
    }
}

void MReliefRaster::Draw(MPlotDevice* pDevice)
//================================
{
    //if(pMap) delete pMap;

    MRect geoBounds;
    pDevice->GetClientRect(&geoBounds); // rectangle of drawing of the map

    MRect preIntersection;
    IntersectionGeoRect(&geoBounds, &m_bounds, preIntersection);

    double left = m_IMap->GetKarta()->m_bmhfile->Left();
    double top = m_IMap->GetKarta()->m_bmhfile->Bottom();
    double right = m_IMap->GetKarta()->m_bmhfile->Right();
    double bottom = m_IMap->GetKarta()->m_bmhfile->Top();

    MRect drawBounds = MRect(left, top, right, bottom);

    drawBounds.NormalizeGeoRect();

    MRect m_userRect;
    IntersectionGeoRect(&drawBounds, &preIntersection, m_userRect);

    MRect logIntersection;
    pDevice->BumToLogic(&m_userRect, logIntersection);

    bool bRedrawImageRelief = false;

    if(m_userRect != saveRect || logIntersection != saveRect_pixel || m_WillBeUpdated)
    {
        bool bIsIntersects = m_heights.Update(pDevice, m_IMap, drawShader, logIntersection);
        if(!bIsIntersects) return;

        saveRect = m_userRect;
        saveRect_pixel = logIntersection;

        bRedrawImageRelief = true;

        if(pMap)
        {
            delete pMap;
            pMap = NULL;
        }
    }


    MRect tempRect;
    tempRect = m_bounds;
    this->SetMbounds(m_userRect);

    if(bRedrawImageRelief)
    {
        //double minHeight = m_heights.GetMinHeight();
        //double maxHeight = m_heights.GetMaxHeight();

        pMapSz = m_heights.GetSize();
        pMap = new QImage(pMapSz, QImage::Format_ARGB32);
        pMap->fill(Qt::transparent); // we fill with a transparent background

        double stepWidthInMetres = pDevice->LogicToBum(pDevice->GetLogRect().Width() / pMapSz.width());
        if (m_IMap->m_karta->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC)
            stepWidthInMetres *= 111144;
        double sqrStepWidthInMetres = stepWidthInMetres * sqrt(fabs(stepWidthInMetres));

        const int threadCount = pMapSz.height();
        double minHeight = m_heights.GetMinHeight();
        double maxHeight = m_heights.GetMaxHeight();

        QFutureSynchronizer<void> synchronizer;
        for (int i = 0; i < threadCount; i++)
        {
            synchronizer.addFuture(QtConcurrent::run(this,
                                                     &MReliefRaster::DrawImagePart,
                                                     i * pMapSz.height() / threadCount,
                                                     (i + 1) * pMapSz.height() / threadCount,
                                                     sqrStepWidthInMetres,
                                                     minHeight, maxHeight));
        }


        synchronizer.waitForFinished();

    }
    pDevice->Raster(this);
    m_WillBeUpdated = false;

    this->SetMbounds(tempRect);

}
