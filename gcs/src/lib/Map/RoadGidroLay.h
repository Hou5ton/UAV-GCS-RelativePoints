//PACKAGE : MAP.
//FILE: RoadGidroLay.h   
//AUTHOR:
//DESCRIPTION:

#ifndef ROADGIDROLAY_H
#define ROADGIDROLAY_H
#include "Layer.h"
#include "MString.h"

// Strings constants
#if ENGVERSION == 1
	#define IDS_ROAD_LAYER "Road Layer"
	#define IDS_GIDRO_LAYER "Hydrographic Layer"
#else
	#define IDS_ROAD_LAYER "слой дорог"
	#define IDS_GIDRO_LAYER "слой гидрографии"
#endif

//PACKAGE : MAP.
//CLASS : MRoadLayer : public MLayer.
//DESCRIPTION : Class roads layer differs from usual layer only by drawing of linear objects.
class MRoadLayer : public MLayer
{

public:

//PACKAGE : MAP.
//FUNCTION : MRoadLayer::MRoadLayer();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MRoadLayer();

//PACKAGE : MAP.
//FUNCTION : MRoadLayer::~MRoadLayer();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MRoadLayer(){};

//PACKAGE : MAP.
//FUNCTION : MRoadLayer::MRoadLayer(const MRect& position);
//DESCRIPTION : Constructor.
//INPUTS : const MRect&.
//RETURNS : NONE.
	MRoadLayer(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MRoadLayer::Draw(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	virtual void Draw(MPlotDevice* pDevice,MRect geodrawrect);

//PACKAGE : MAP.
//FUNCTION : MRoadLayer::WhatTheLayer();
//DESCRIPTION : Returns layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual   MString WhatTheLayer() { return ID_ROAD_LAYER;}
};


//PACKAGE : MAP.
//CLASS : MGidroLayer : public MLayer.
//DESCRIPTION : Class - hydrography layer differs from usual layer only by drawing of linear objects.
class MGidroLayer : public MLayer
{

public:

//PACKAGE : MAP.
//FUNCTION : MGidroLayer::MGidroLayer();
//DESCRIPTION : Constructor by default.
//INPUTS : NONE.
//RETURNS : NONE.
	MGidroLayer();

//PACKAGE : MAP.
//FUNCTION : MGidroLayer::~MGidroLayer();
//DESCRIPTION : Destructor.
//INPUTS : NONE.
//RETURNS : NONE.
	~MGidroLayer(){};

//PACKAGE : MAP.
//FUNCTION : MGidroLayer::MGidroLayer(const MRect& position);
//DESCRIPTION : Constructor.
//INPUTS : const MRect&.
//RETURNS : NONE.
	MGidroLayer(const MRect& position);

//PACKAGE : MAP.
//FUNCTION : MGidroLayer::Draw(MPlotDevice* pDevice,MRect geodrawrect);
//DESCRIPTION : Function of drawing.
//INPUTS : MPlotDevice pointer,MRect .
//RETURNS : NONE.
	virtual void Draw(MPlotDevice* pDevice,MRect geodrawrect);

//PACKAGE : MAP.
//FUNCTION : MGidroLayer::WhatTheLayer();
//DESCRIPTION : Returns layer name.
//INPUTS : NONE.
//RETURNS : MString.
	virtual   MString WhatTheLayer() { return ID_GIDRO_LAYER;}
};

#endif
