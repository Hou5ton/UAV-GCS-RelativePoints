//PACKAGE : MAP.
//FILE: ReliefRaster.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MRELIEFRASTER_H
#define MRELIEFRASTER_H

#include <QObject>

#include "Raster.h"
#include "BaseCl.h"
#include "KartaInterface.h"

class MReliefRaster : public MRaster
{
private:

	//PACKAGE : MAP.
	//CLASS : MHeightMap.
	//DESCRIPTION :
	class MHeightMap
	{
		struct ParamsThread
        {
            ParamsThread(MString _matrixPath,
                         double _leftStart,
                         double _topStart,
                         double _rowCount,
                         double _stepWidth)
            {
                matrixPath = _matrixPath;
                leftStart = _leftStart;
                topStart = _topStart;
                rowCount = _rowCount;
                stepWidth = _stepWidth;
            }

            MString matrixPath;
            double leftStart;
            double topStart;
            double rowCount;
            double stepWidth;
        };


    private:
        int **m_heights;
        QSize m_size;
    private:
        void Realloc(QSize newSize);
    public:

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::MHeightMap();
        //DESCRIPTION : Constructor.
        //INPUTS : NONE.
        //RETURNS : NONE.
        MHeightMap();

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::~MHeightMap();
        //DESCRIPTION : Destructor.
        //INPUTS : NONE.
        //RETURNS : NONE.
        ~MHeightMap();

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::Update(MPlotDevice *plotDevice, MRect m_bounds, MKartaInterface *IMap, bool drawShader);
        //DESCRIPTION : Update
        //INPUTS : MPlotDevice pointer, MRect, MKartaInterface pointer, bool .
        //RETURNS : NONE.
        bool Update(MPlotDevice *plotDevice, /*MRect m_bounds, MRect geoBounds,*/ MKartaInterface *IMap, bool drawShader, MRect logIntersection);

		void ReadHeightFromMatrixes(const ParamsThread &params, MPlotDevice *plotDevice, const int &j, int **result);

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::GetHeight(int row, int column);
        //DESCRIPTION : Gets height.
        //INPUTS : int , int .
        //RETURNS : int.
        int GetHeight(int row, int column);

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::GetMaxHeight();
        //DESCRIPTION : Gets maximum height.
        //INPUTS :
        //RETURNS : int.
        int GetMaxHeight();

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::GetHeight(int row, int column);
        //DESCRIPTION : Gets height.
        //INPUTS : int , int .
        //RETURNS : int.
        int GetMinHeight();

        //PACKAGE : MAP.
        //FUNCTION : MHeightMap::GetSize();
        //DESCRIPTION : Gets size.
        //INPUTS : NONE.
        //RETURNS : QSize.
        QSize GetSize();
    };

public:

    //PACKAGE : MAP.
    //FUNCTION : MReliefRaster::MReliefRaster(const MRect& position, MKartaInterface *IMap, bool drawShader = false);
    //DESCRIPTION : Constructor.
    //INPUTS : const MRect& , MKartaInterface pointer, bool .
    //RETURNS : NONE.
    MReliefRaster(const MRect& position, MKartaInterface *IMap, bool drawShader = false);

    //PACKAGE : MAP.
    //FUNCTION : MColor MReliefRaster::GetColor(double height, double minHght, double maxHght, int scale = 1);
    //DESCRIPTION : Gets color pixel.
    //INPUTS : double, double, double, int.
    //RETURNS : MColor.
    static MColor GetColor(double height, double minHght, double maxHght, int scale = 1);

    static MColor GetColor1(double height, double minHght, double maxHght);

	//PACKAGE : MAP.
	//FUNCTION : MColor MReliefRaster::GetTranspColor(double height, double minHght, double maxHght, double transp = 1.0, int scale = 1);
	//DESCRIPTION : Gets color of pixel with transparancy.
	//INPUTS : double, double, double, double, int.
	//RETURNS : MColor.
	static MColor GetTranspColor(double height, double minHght, double maxHght, double transp = 1.0, int scale = 1);

	//PACKAGE : MAP.
	//FUNCTION : MReliefRaster::~MReliefRaster();
	//DESCRIPTION : Destructor.
	//INPUTS : NONE.
	//RETURNS : NONE.
	~MReliefRaster();

    //PACKAGE : MAP.
    //FUNCTION : MReliefRaster::DrawImagePart(int rowStart, int rowEnd, double sqrStepWidthInMetres);
    //DESCRIPTION : Draws part of image.
    //INPUTS : int , int , double .
    //RETURNS : NONE.
    void DrawImagePart(/*MPlotDevice* pDevice, */int rowStart, int rowEnd, double sqrStepWidthInMetres, double minHeight, double maxHeight);

    //PACKAGE : MAP.
    //FUNCTION : MReliefRaster::Draw(MPlotDevice* pDevice);
    //DESCRIPTION : Draws.
    //INPUTS : MPlotDevice pointer.
    //RETURNS : NONE.
    void Draw(MPlotDevice* pDevice);

    //PACKAGE : MAP.
    //FUNCTION : MReliefRaster::setShader(bool draw);
    //DESCRIPTION : Sets shader.
    //INPUTS : bool.
    //RETURNS : NONE.
    void setShader(bool draw);

    double dMinHeight();
    double dMaxHeight();

    bool getShaderState();

    void setAlpha(int a)
    {this->alpha = a; m_WillBeUpdated = true;}
	int getAlpha(){return this->alpha;}

	bool m_WillBeUpdated;

private:
	MHeightMap m_heights;
	MKartaInterface *m_IMap;

	MRect saveRect;
	MRect saveRect_pixel;

	bool drawShader; // It indicates whether the shader render relief
	int alpha;
	void MYsetColor(double minHeight, double maxHeight, QColor& rgb, int ht);
	void MYsetTranspColor(double minHeight, double maxHeight, QColor &rgb, double transp, int ht);

	static const int m_minH = 0; // minimum height of the card (hereinafter - rendering green )
	static const int m_maxH = 9000; // maximum height map ( above - rendering red)
	static const int i_minH = -500; // minimum possible height card
};

#endif // MRELIEFRASTER_H
