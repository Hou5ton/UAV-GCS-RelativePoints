//File of realization of a conclusion to the concrete device
//For suppression of 'Warning'
#include "PlotDevice.h"

//The coefficient blocking Windows values otnositeljno of fonts
//and allowing absolutely exact conclusion to the screen
//====================================================================
//						MPlotDevice			
//====================================================================
//The virtual device for an image conclusion

//==================================================================
//!Function for recalculation of coefficients of the translation
void MPlotDevice::RecalcKoeffs(){
    //==================================================================
    double rGx = m_BumBounds.right - m_BumBounds.left;
    double rGy = m_BumBounds.bottom - m_BumBounds.top;
    double sGx = m_BumBounds.right + m_BumBounds.left;
    double sGy = m_BumBounds.bottom + m_BumBounds.top;


    // we assume that the beginning of logical coordinates in the center of the page
    double rLx = m_LogBounds.Width();
    double rLy = -m_LogBounds.Height();
    double sLx = 0;
    double sLy = 0;

    // at alphastation at initial loading the program glitches,
    //if not to put protection from zero:
    if( rGx == 0 ) rGx = 1;
    if( rGy == 0 ) rGy = 1;
    if( rLx == 0 ) rLx = 1;
    if( rLy == 0 ) rLy = 1;

    //paper in the logical
    if (fabs(rLx/rGx)>fabs(rLy/rGy)) { A = rLy/rGy; C = rGy/rLy; }
    else                     	     { A = rLx/rGx; C = rGx/rLx; }
    B1 = 0.5*(sLx - sGx*A);
    B2 = 0.5*(sLy - sGy*A);

    //logical in the paper
    if (fabs(rGx/rLx)<fabs(rGy/rLy)) C = rGy/rLy;
    else                             C = rGx/rLx;
    D1 = 0.5*(sGx - sLx*C);
    D2 = 0.5*(sGy - sLy*C);
}


// ==================================================================
// * Function parameters : 
// [pRect] - rectangle for the translation
//[pCRect] - a rectangle where to place
//! ------------------- 
void MPlotDevice::BumToLogic(MRect* pRectIn,MRect& pRectOut)
// ==================================================================
{
    pRectOut.left = A*pRectIn->left + B1;
    pRectOut.top  = A*pRectIn->top + B2;
    pRectOut.right = A*pRectIn->right + B1;
    pRectOut.bottom  = A*pRectIn->bottom + B2;
}

// ==================================================================
// * Function parameters : 
// [pRect] -
// [pCRect] -
//! ------ 
void MPlotDevice::LogicToBum(MRect* pRectIn,MRect& pRectOut)
// ==================================================================
{
    pRectOut.left = C*pRectIn->left + D1;
    pRectOut.top  = C*pRectIn->top + D2;
    pRectOut.right = C*pRectIn->right + D1;
    pRectOut.bottom  = C*pRectIn->bottom + D2;
}



//CLIPPING FUNCTIONS THE GENERAL FOR ALL VECTOR DEVICES


// ==================================================================
//! Actually the beginning of clipping (an ochitsk of all of that was earlier established)
bool MPlotDevice::BeginClipRgn()
// ==================================================================
{
    //	return ClearClip();
    //	MRect bumRect;
    //	MRect logRect = GetLogRect();
    //	LogicToBum(&logRect,bumRect);
    //	AddClipRgn(&bumRect,ADD_REGION);
    //	return true;
    AddClipRgn(&m_BumBounds/*bumRect*/,ADD_REGION);
    return true;
}


//=================================================================
//!Addition of the clipping region
bool MPlotDevice::AddClipRgn(MPoint* pta,int iNum,RGN_TYPE type)
//=================================================================
{
    // it is possible толъко addition of regions (connection)
    /*	if(type!=ADD_REGION){
        InternalMessage("Временно не поддерживаемые регионы");
        return false;
    }*/
    if(type == ADD_REGION)
        LogClip(pta,iNum,PLUS);
    else if(type == INTERSECT_REGION)
        LogClip(pta,iNum,XORING);
    else if(type == CUT_REGION)
        LogClip(pta,iNum,NOT);

    MPoint* ptnewArr = (MPoint*)malloc(sizeof(MPoint)*iNum);
    BumToLogic(pta, ptnewArr, iNum);
    MPtArray* ptNewClip = new MPtArray;
    ptNewClip->FastSet(iNum, ptnewArr);
    m_clips.AddTail(ptNewClip);
    free(ptnewArr);
    return true;
}


//================================================================
//!Addition of the clipping rectangle
bool MPlotDevice::AddClipRgn(MRect* pRect,RGN_TYPE type)
//================================================================
{
    MPoint ptar[4];
    ptar[0] = MPoint(pRect->left,pRect->top);
    ptar[1] = MPoint(pRect->right,pRect->top);
    ptar[2] = MPoint(pRect->right,pRect->bottom);
    ptar[3] = MPoint(pRect->left,pRect->bottom);
    AddClipRgn(ptar,4,type);
    return true;
}


//===================================================================
//!Cleaning of the clipping region
bool MPlotDevice::ClearClip()
//===================================================================
{
    pCurrentClipLog->SetSize(0);

    for(int i = m_clips.GetSize() - 1; i>=0; i--)
    {
        delete (MPtArray*)m_clips.GetAt(i);
    }

    m_clips.SetSize(0);
    m_bClipRegionSelected = false;
    return true;
}

//END OF FUNCTIONS OF CLIPPING OF THE REGION
