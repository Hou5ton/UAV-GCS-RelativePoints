//==================================
//Realizacija MRoadLayer = Sloj Dorog i MGidroLayer = Sloj Gidrografii

#include "RoadGidroLay.h"
#include "PlotDevice.h"
#include "DrawObj.h"
//==================================
//!Road Layer
MRoadLayer::MRoadLayer()
{
    //MLayer::MLayer();
    m_name = IDS_ROAD_LAYER;// "Road Layer"
    m_sort = IDS_ROAD_LAYER;//"Road Layer"
}

//!==================================
MRoadLayer::MRoadLayer(const MRect& bounds)
{
    //MLayer::MLayer();
    m_bounds = bounds;
    //	MLayer::MLayer(bounds);
    m_name = IDS_ROAD_LAYER;//"Road Layer"
    m_sort = IDS_ROAD_LAYER;//"Road Layer"
}
//==================================
//! drawing of objects of a road layer
void MRoadLayer::Draw(MPlotDevice* pDevice, MRect geodrawrect)
{
    if(!x_visible)
    {
        return;
    }

    //we will make two auxiliary lists of objects of a layer: linear objects
    //and all other objects of a layer
    MObjectList lineoblist;
    MObjectList ostoblist;

    int maxnumprosl = 0;//the maximum number of layers in linear objects of a layer
    int numprosl = 0;

    MTopobject* pTpo = NULL;

    std::for_each(m_oblist.begin(), m_oblist.end(), [&] (MObject* pObj)
    {
        if(pObj->WhatTheObject() == ID_TOPOBJECT)
        {
            pTpo = (MTopobject*)pObj;

            if(pTpo->m_pZnak)
            {
                if(pTpo->m_pZnak->GetYaz()->GetType() == F_LIN)
                {
                    lineoblist.push_back(pObj);

                    numprosl = ((MLinYaz*)pTpo->m_pZnak->
                                GetYaz()->m_object)->m_linia->GetSize();

                    if(numprosl>maxnumprosl)
                    {
                        maxnumprosl = numprosl;
                    }
                }
                else
                    ostoblist.push_back(pObj);
            }
        }
        else
            ostoblist.push_back(pObj);
    });


    std::for_each(ostoblist.begin(), ostoblist.end(), [&](MObject* pObj)
    {
        if (pObj->m_bounds.IntersectGeoRect(pObj->m_bounds,geodrawrect))
        {
            pObj->Draw(pDevice);
        }
    });

    pDevice->Actualize();

    MPrArray* prosls;

    //we draw all linear objects on layers
    for(int i = 0; i < maxnumprosl; ++i)
    {
        std::for_each(lineoblist.begin(), lineoblist.end(), [&] (MObject* pObj)
        {
            // object in zoom area?
            if (pObj->m_bounds.IntersectGeoRect(pObj->m_bounds, geodrawrect))
            {
                pTpo = (MTopobject*)pObj;
                prosls = ((MLinYaz*)pTpo->m_pZnak->GetYaz()->m_object)->m_linia;

                if(i < prosls->GetSize())
                {
                    MDrawing::DrawProsloika(pTpo,
                                            pDevice,
                                            ((MLinYaz*)pTpo->m_pZnak->GetYaz()->m_object)->m_linia->GetAt(i),
                                            (MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI),
                                            (MRzArray*)pTpo->m_Komplekt->GetAt(F_RAZRYVY));
                }
            }
        });
    }

    // we draw a result
    pDevice->Actualize();
}


//==================================
//!Hydrographic Layer
MGidroLayer::MGidroLayer()
{
    //MLayer::MLayer();
    m_name = IDS_GIDRO_LAYER; //"Hydrographic Layer"
    m_sort = IDS_GIDRO_LAYER; //"Hydrographic Layer"
}

//!==================================
MGidroLayer::MGidroLayer(const MRect& bounds)
{
    //MLayer::MLayer();
    m_bounds = bounds;
    //	MLayer::MLayer(bounds);
    m_name = IDS_GIDRO_LAYER; //"Hydrographic Layer"
    m_sort = IDS_GIDRO_LAYER; //"Hydrographic Layer"
}

//==================================
//! drawing of objects of a layer of hydrography
void MGidroLayer::Draw(MPlotDevice* pDevice, MRect geodrawrect)
{
    if(!x_visible)
        return;
    //we will make two auxiliary lists of objects of a layer: linear objects
    //and all other objects of a layer

    MObjectList lineoblist;
    MObjectList ostoblist;

    int maxnumprosl = 0;//the maximum number of layers in linear objects of a layer
    int numprosl = 0;

    MTopobject* pTpo = NULL;

    std::for_each(m_oblist.begin(), m_oblist.end(), [&] (MObject *pObj)
    {
        if(pObj->WhatTheObject() == ID_TOPOBJECT)
        {
            pTpo = (MTopobject*)pObj;
            if(pTpo->m_pZnak)
            {
                if(pTpo->m_pZnak->GetYaz()->GetType() == F_LIN)
                {
                    lineoblist.push_back(pObj);
                    numprosl = ((MLinYaz*)pTpo->m_pZnak->
                                GetYaz()->m_object)->m_linia->GetSize();

                    if(numprosl>maxnumprosl)
                        maxnumprosl = numprosl;
                }
                else
                    ostoblist.push_back(pObj);
            }
        }
        else
            ostoblist.push_back(pObj);
    });

    std::for_each(ostoblist.begin(), ostoblist.end(), [&](MObject* it)
    {
        if (it->m_enabled &&
                it->CheckSize(pDevice) &&
                it->m_bounds.IntersectGeoRect(it->m_bounds,geodrawrect))
        {
            it->Draw(pDevice);
        }
    });

    // we draw a result
    pDevice->Actualize();

    MPrArray* prosls;

    //we draw all linear objects on layers
    for(int i = 0; i < maxnumprosl; ++i)
    {
        std::for_each(lineoblist.begin(), lineoblist.end(), [&](MObject* it)
        {
            if (it->m_enabled &&
                    it-> CheckSize(pDevice) &&
                    it->m_bounds.IntersectGeoRect(it->m_bounds,geodrawrect))
            {
                pTpo = (MTopobject*)it;
                prosls = ((MLinYaz*)pTpo->m_pZnak->GetYaz()->m_object)->m_linia;

                if(i < prosls->GetSize())
                {
                    MDrawing::DrawProsloika(pTpo,
                                            pDevice,
                                            prosls->GetAt(i),
                                            (MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI),
                                            (MRzArray*)pTpo->m_Komplekt->GetAt(F_RAZRYVY));
                }
            }
        });
    }
    // we draw a result
    pDevice->Actualize();
};
