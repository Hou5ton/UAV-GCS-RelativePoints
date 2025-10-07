// Realization of drawing of various primitives
#include "Array.h"
#include "DrawObj.h"
#include "Topobject.h"
#include "Yaz.h"
#include "Karta.h"

//Pointers on function of transformation for a concrete step of drawing
double (*ConvertSizeToReal)(MTopobject* pObj,float fSize);
void RegisterConvertSizeToReal(double (*func)(MTopobject*,float)){ConvertSizeToReal = func;};
#include "Refglobal.h"

double etalon_value = 37.5;

// ==================================================================
//!	Filling drawing
/*
// [pYaz] - pointer on drawing language
// [pDC] -	DC which need to draw
*/
void MDrawing::DrawZapolnenie(MTopobject* pObj,MYaz* pYaz,
								MPlotDevice* pDevice,MPtArray* pta)
// ==================================================================
{
//  MRect georect = pObj->m_pLay->m_pKarta->GetGeoBounds();
    //receive the pointer on the array of layers of filling
    MPlArray* pla = ((MPloYaz*)pYaz->m_object)->m_plast;
    int i = pla->GetSize();
    unsigned char pltType;

    MRect clientRect;
    pDevice->GetClientRect(&clientRect);

    if(pDevice->GetClipRegionFlag())
	{
        pDevice->CreateClipRgn(pta->GetHead(), pta->GetSize());

        MPtArray* pta_next;

		for(int j1=pObj->F_TOCHKI+1;j1<pObj->m_Komplekt->GetSize();j1++)
		{
            pta_next=(MPtArray*)pObj->m_Komplekt->GetAt(j1);

            if(pObj->m_Komplekt->GetAt(j1)->WhatTheArray() == "PHL")
            {
                MRect rect = GetObhvatRect(pta_next);
                if(IntersectGeoRectFast(rect, clientRect))
                    pDevice->AddClipRgn(pta_next->GetHead(),pta_next->GetSize(), CUT_REGION);
            }
			else
            {
                pDevice->AddClipRgn(pta_next->GetHead(),pta_next->GetSize(), ADD_REGION);
            }
        }
	}

    int j, HSIZE = 0;

    for(j = 0; j < i; ++j)
	{
        pltType = pla->GetAt(j)->GetType();
        HSIZE += ((pltType == PLT_HANDLE) ||
                  (pltType == PLT_NAS_HANDLE));
	}

    //if in the form it isn't empty, we will take away a place for an text
    if(pObj->m_pLay->m_bOperLayer)
    {
        for(int i = 0; i < ((MFormArray*)pObj->m_pZnak->GetYaz()->m_formula)->GetSize(); ++i)
        {
            if(GetFormulaDlina(pObj, pDevice, ((MFormArray*)pObj->m_pZnak->GetYaz()->m_formula)->GetAt(i)) > 0)
                ++HSIZE;
		}
	}

    //draw each layer
   int HINDEX=0;
    for(j = 0; j < i; ++j)
	{
        MPlast* pplast = pla->GetAt(j);
        if(pplast->GetType() == PLT_HANDLE)
        {//drawing layer-symbol
            DrawHandle(pObj, pDevice, (MHandleArea*)pplast, GetHandlePlastPoint(pObj,pta,HINDEX, HSIZE));
			HINDEX++;
		}
        else if(pplast->GetType() == PLT_NAS_HANDLE)
        {//drawing adjustable layer-symbol
			DrawNastrHandle(pObj, pDevice, (MNastrHandleArea*)pplast, GetHandlePlastPoint(pObj,pta,HINDEX, HSIZE));
			HINDEX++;
		}
		else DrawPlast(pObj,pDevice,pplast,pta);
	}

    //drawing of forms for an operational layer
	if(pObj->m_pLay->m_bOperLayer) 
	{
        //receive the form
		MYaz* m_pYaz = pObj->m_pZnak->GetYaz();
		MFormArray* forma = (MFormArray*)m_pYaz->m_formula;
		for(int i=0; i<forma->GetSize(); i++)
		{
			MFormula* formula = forma->GetAt(i);
		double form_dlina = GetFormulaDlina(pObj, pDevice, formula);	
			if(form_dlina)
			{
				pObj->DeletePrivs(i,SIMPLE_PRIVYAZKA);
		MPoint point = GetHandlePlastPoint(pObj, pta, HINDEX, HSIZE);
		MPtArray privarr;
		privarr.ClearAll();
		MPoint tmpPt(point.x - form_dlina/2, point.y);
		privarr.AddTail(&tmpPt);
                pObj->AddPriv(i, privarr);
				HINDEX++;
			}				
		}
    }
}

#include "QtDevice.h"
void MDrawing::DrawZapolnenieZone(MTopobject *pObj, MYaz *pYaz, MPlotDevice *pDevice, MPtArray *pta)
{
    //  MRect georect = pObj->m_pLay->m_pKarta->GetGeoBounds();
    //receive the pointer on the array of layers of filling
    MPlArray* pla = ((MPloYaz*)pYaz->m_object)->m_plast;
    int i = pla->GetSize();
    MPlast* pplast;
    MFormula* formula;
    unsigned char pltType;

    MRect clientRect;
    pDevice->GetClientRect(&clientRect);

    MQtDevice* const qtD = dynamic_cast<MQtDevice* >(pDevice);
    if(!qtD) return;

    QPixmap* const pixmapAct = dynamic_cast<QPixmap* >(qtD->painter()->device());
    QPixmap pxm(pixmapAct->width(), pixmapAct->height());
    pxm.fill(Qt::transparent);

    QPainter painter(&pxm);
    painter.setBrush(Qt::black);

    for(int j1=pObj->F_TOCHKI+1; j1<pObj->m_Komplekt->GetSize(); ++j1)
    {
        MHlArray* pta_next = dynamic_cast<MHlArray*>(pObj->m_Komplekt->GetAt(j1));

        if(pta_next != nullptr)
        {
            MRect rect = GetObhvatRect((MPtArray*)pObj->m_Komplekt->GetAt(j1));
            if(IntersectGeoRectFast(rect, clientRect))
            {
                const int iNumPoints = pta_next->GetSize();
                QPolygon pointArr(iNumPoints);
                qtD->BumToLogicI(pta_next->GetHead(), pointArr, iNumPoints);

                painter.drawPolygon(pointArr);
            }
        }
    }

    painter.end();
    qtD->painter()->end();

    QBitmap const bitmap = pxm.createMaskFromColor(Qt::black);

    pxm.fill(Qt::transparent);
    qtD->painter()->begin(&pxm);

    int HINDEX=0;
    int j, HSIZE = 0;
    for(j = 0; j < i; ++j)
    {
        pplast = pla->GetAt(j);

        for(int kkk = 0; kkk < pObj->m_Komplekt->GetSize(); kkk++)
        {
            MPtArray* pta_arr = dynamic_cast<MPtArray*>(pObj->m_Komplekt->GetAt(kkk));
            if(pta_arr)
            {
                if(pplast->GetType()==PLT_HANDLE)
                {//drawing layer-symbol
                    DrawHandle(pObj, pDevice, (MHandleArea*)pplast, GetHandlePlastPoint(pObj, pta_arr, HINDEX, HSIZE));
                    HINDEX++;
                }
                else if(pplast->GetType()==PLT_NAS_HANDLE)
                {//drawing adjustable layer-symbol
                    DrawNastrHandle(pObj, pDevice, (MNastrHandleArea*)pplast, GetHandlePlastPoint(pObj, pta_arr, HINDEX, HSIZE));
                    HINDEX++;
                }
                else DrawPlast(pObj, qtD, pplast, pta_arr);
            }

            MRzArray* razryvy = NULL;
            if(pta_arr) MDrawing::DrawLine(pObj,pObj->m_pZnak->GetYaz(), qtD, pta_arr, razryvy);
        }
    }
    qtD->painter()->end();

    pxm.setMask(bitmap);

    qtD->painter()->begin(pixmapAct);
    qtD->painter()->drawPixmap(0, 0, pxm);
}

// ==================================================================
//function of calculation of a point of placement of a symbol of pHandle layer
MPoint MDrawing::GetHandlePlastPoint(MTopobject* pObj, MPtArray* pta, int index, int count)
// ==================================================================
{
	MRect m_bounds = pObj->m_bounds;
	double height = 0;
	double width = 0;
	MPoint ptLT = MPoint(m_bounds.left, m_bounds.top);
    //receive a rectangle of coverage of part of object:
    //for this purpose divide the big party of m_bounds into count of parts and take index part
	bool isHeight = true;
	if(m_bounds.Height()>m_bounds.Width())
	{
		height = m_bounds.Height()/count;
		ptLT.y -= index*height;
		width = m_bounds.Width();
	}
	else 
	{
		width = m_bounds.Width()/count;
		ptLT.x += index*width;
		height = m_bounds.Height();
		isHeight = false;
	}
    //received rectangle
	MRect rect = MRect(ptLT.x, ptLT.y, ptLT.x+width, ptLT.y-height);

    /*Further idea is as follows:
    in a rectangle of rect we will define the middle segment parallel to the longest side,
    we will calculate the array of points of intersection of this segment with object,
    the point of drawing of a symbol is the middle of the greatest of the segments consisting of points of intersection
	*/
	MPoint point1, point2;
	if(isHeight)
	{
		point1 = MPoint(rect.left, rect.CenterY());
		point2 = MPoint(rect.right, rect.CenterY());
	}
	else 
	{
		point1 = MPoint(rect.CenterX(), rect.top);
		point2 = MPoint(rect.CenterX(), rect.bottom);
	}

	MPoint ptCenter = MPoint(rect.CenterX(), rect.CenterY());

	MPtArray* ptcross = new MPtArray();
	MInArray* inarray = new MInArray();

	MPoint pt1, pt2, ptTek;
	double rasst = 0;
	if(FindCrossSegWithKontur(point1, point2, pta, ptcross, inarray))
	{
		if(!ptcross) return ptCenter;
                DeletingRepeatedPoints(ptcross);
		int iSize = ptcross->GetSize();
		for(int i=0; i<iSize-1;i++)
		{
			pt1 = ptcross->GetAt(i);
			pt2 = ptcross->GetAt(i+1);
			ptTek = MPoint((pt1.x + pt2.x)/2, (pt1.y + pt2.y)/2);
			if(ptTek.InnerPtFast((MPoint*)pta->m_pHead, pta->GetSize()))
			{
				if(DLINASG(pt1,pt2)>rasst)
				{
					ptCenter = ptTek;
					rasst = DLINASG(pt1,pt2);
				}
			}
		}
	}

	delete ptcross;
	delete inarray;

	return ptCenter;
}

// ==================================================================
//!  ------ 
/**  Returns : [void] -
// 
// * Function parameters : 
// [pDC] -
// [pPlast] -
// [pta] -
*/
void MDrawing::DrawPlast(MTopobject* pObj,MPlotDevice* pDevice,
						   MPlast* pPlast,MPtArray* pta)
// ==================================================================
{
    //check on not emptiness
	if(!pPlast) return;
	
    //depending on filling type
    //we cause the corresponding functions of drawing
	MZalivka* zalivka;
	MShtrihovka*  shtrihovka;
	MUzor* uzor;
	MPercentUzor* pPercentUzor;
	switch(pPlast->GetType())
	{
	case PLT_NESTANDART:
		break;
	case PLT_SHTRIHOVKA:
		shtrihovka=(MShtrihovka*)pPlast;
		DrawShtrihovka(pObj,pDevice,shtrihovka,pta);
		break;
	case PLT_ZALIVKA:
		zalivka=(MZalivka*)pPlast;
        DrawZalivka(pObj,pDevice,zalivka,pta);
		break;
	case PLT_UZOR:
		uzor=(MUzor*)pPlast;
		DrawUzor(pObj,pDevice,uzor,pta);
		break;
	case PLT_PERCENT_UZOR:
		pPercentUzor = (MPercentUzor*)pPlast;
		DrawPercentUzor(pObj,pDevice,pPercentUzor,pta);
		break;
	case PLT_NAS_SHTRIHOVKA:
		DrawNastrShtrihovka(pObj,pDevice,(MNastrShtrihovka*)pPlast,pta); 
		break;
	case PLT_NAS_UZOR:
		DrawNastrUzor(pObj,pDevice,(MNastrUzor*)pPlast,pta);
		break;
	case PLT_NAS_PERCENT_UZOR:
		DrawNastrPercentUzor(pObj,pDevice,(MNastrPercentUzor*)pPlast,pta);
		break;
	case PLT_PATTERN:
		DrawPattern(pObj, pDevice, (MPattern *)pPlast, pta);
		break;
	}

	pDevice->EndClipping();
}
// ==================================================================
//! Shading drawing
/**Upon transition to other devices of drawing, drawing of shading changed
//as all mathematics has to is made in the real
//coordinates time also came and here to use them
// * Returns : [void] -
*/
void MDrawing::DrawShtrihovka(MTopobject* pObj,MPlotDevice* pDevice,
								MShtrihovka* pShtrihovka,MPtArray* pta)
// ==================================================================
{
    //coverage rectangle m_bounds

    //define a step of shading and its thickness
	double step=fabs((*ConvertSizeToReal)(pObj,pShtrihovka->m_shag));
	double tols=fabs((*ConvertSizeToReal)(pObj,pShtrihovka->m_thickness));
	
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(pShtrihovka->m_colorkod, color);
	
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}

	if(step<=tols){
        //In this case we fill in the polygon
		pDevice->SaveZapolnenie();
		pDevice->SetZapolnenie(&color);
		pDevice->Polygon(pta->GetHead(),pta->GetSize());
		pDevice->RestoreZapolnenie();
		return;
	}
    //if shading itself isn't visible, its drawing isn't necessary
//	if(!pDevice->Distinguish(step/VISIBLE_SHTRIHOVKA_STEP))    
	if(!pDevice->Distinguish(step/VISIBLE_SHTRIHOVKA_STEP/12))      //vipa  2004.06.14
		return;

    //create a pen necessary to us
	M_LOG_PEN pen(PS_SOLID,MPoint(tols,tols),color);
	pDevice->SavePencil();
	pDevice->SetPencil(&pen);
	
    //the angle is expressed in degrees
	double angle=pShtrihovka->m_ugol;

    //create area in which drawing is allowed
////	pDevice->BeginClipRgn();

	

	//pDevice->SaveClipping();
	pDevice->CreateClipRgn(pta->GetHead(),pta->GetSize());
	//pDevice->AddClipRgn(pta->GetHead(),pta->GetSize(),INTERSECT_REGION);
////	pDevice->AddClipRgn(pta->GetHead(),pta->GetSize(),ADD_REGION);
////	pDevice->SelectClipRgn();
  MRect rectangle = GetObhvatRect(pta);
	DrawShtrihRect(pDevice,&rectangle,angle,step);

    //if shading double, we shade once again perpendicularly
	if(pShtrihovka->m_type==SHR_DVOINAIA)
	{
        //lead a angle to a normal amount
		while(angle>=360)angle-=360;
		while(angle<0)angle+=360;
        //establish a angle 90
		if((angle==0)||(angle==180)) angle=90;
		else if((angle==90)||(angle==270)) angle=0;
		else angle=180-angle;
        //and draw
  MRect rectangle = GetObhvatRect(pta);
		DrawShtrihRect(pDevice,&rectangle,angle,step);
	}

/*	MRect rect = pDevice->GetLogRect();
	MRect bumRect;
	pDevice->LogicToBum(&rect,bumRect);
    //restore the previous condition of a context
	pDevice->BeginClipRgn();
	pDevice->AddClipRgn(&bumRect);
	pDevice->SelectClipRgn();*/

//	pDevice->RestoreClipping();
////	pDevice->ClearClip();
	pDevice->RestorePencil();
}

//! Filling drawing by the user pattern
void MDrawing::DrawPattern(MTopobject* pObj,MPlotDevice* pDevice,
		MPattern* pPattern, MPtArray* pta)
{
	if(!pPattern) return;

    //create a brush necessary to us and a transparent pen
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take white color
	if(!pObj->GetKarta()->GetDictColorValue(pPattern->m_colorkod, color))
		color.SetColor(255,255,255);
	
	if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}

	pDevice->SaveZapolnenie();
	MImage *image = pPattern->GetImage();
	if (!image)
	{
		image = new MImage();
		pObj->GetKarta()->GetDictImageValue(pPattern->m_imagekod, *image);
		pPattern->SetImage(image);
	}
	pDevice->SetZapolnenie(image, color);


	pDevice->SavePencil();
	pDevice->SelectStockObject(NULL_PEN);

    //draw filling
	pDevice->Polygon(pta->GetHead(), pta->GetSize());

    //return an old brush and a pen
	pDevice->RestoreZapolnenie();
	pDevice->RestorePencil();
}
// ==================================================================
//!Filling drawing
void MDrawing::DrawZalivka(MTopobject* pObj,MPlotDevice* pDevice,
							 MZalivka* pZalivka,MPtArray* pta)
// ==================================================================
{
    //Drawing of usual filling
    if(!pZalivka)
        return;

    //we create a brush necessary to us and a transparent pen
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take white color
	if(!pObj->GetKarta()->GetDictColorValue(pZalivka->m_colorkod, color))
		color.SetColor(255,255,255);
        MColor whiteclr;
        pObj->GetKarta()->GetDictColorValue(1,whiteclr);

        if/*(*/(pObj->bDrawColorZalivka)//&&(color != whiteclr))
        {
            color = pObj->mDrawColorZalivka;
        }
	
	if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}

	pDevice->SaveZapolnenie();
	pDevice->SetZapolnenie(&color);

	pDevice->SavePencil();
	pDevice->SelectStockObject(NULL_PEN);

    //draw filling
    pDevice->Polygon(pta->GetHead(),pta->GetSize());

    //return an old brush and a pen
    pDevice->RestoreZapolnenie();
	pDevice->RestorePencil();
}

// ==================================================================
//! Drawing of a pattern
void MDrawing::DrawUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MUzor* pUzor,MPtArray* pta)
// ==================================================================
{
   MRect rectangle;
    //define a step of shading and its thickness
	double step=fabs((*ConvertSizeToReal)(pObj,pUzor->m_shag));
    //The step can't be zero
	if(!step)			return;
		
    //set the mode of drawing of symbols necessary to us
	MSymbol drawsym;
	drawsym.Copy(&pUzor->m_symbol);

    // symbol font
	unsigned int uiOldMode  = pDevice->SetBkMode(UT_TRANSPARENT);
  unsigned int uiOldAlign = pDevice->SetTextAlign(drawsym.align);

    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)drawsym.colorkod, color))
		color.SetColor(0,0,0);
	
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}

	MColor crOldTextColor = pDevice->SetTextColor(color);

	M_LOG_FONT lfont = drawsym.logfont;
	if(drawsym.scaled)
		lfont.lfHeight = (*ConvertSizeToReal)(pObj,drawsym.value);
	else
		lfont.lfHeight = pDevice->LogicToBum(drawsym.value);



    // it is drawn at joint performance of two conditions:
    //if it is marked as visible
    //and if it is readable in this scale
	if ((!drawsym.visible)||(!pDevice->Distinguish(lfont.lfHeight/VISIBLE_FONT_UZOR))) 
		return;

	pDevice->SaveFont();
	pDevice->SetFont(&lfont);

    //the angle of a grid is expressed in degrees
	double angle=0;
	if(pUzor->m_type==STK_KOS)
		angle=pUzor->m_ugol;



	MPtArray* ptaInternal =NULL;
    //directly drawing depending on grid type
	switch(pUzor->m_type)
	{
	case STK_PRAM:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-lfont.lfHeight);
		else
			ptaInternal = BuildObvod(pta,lfont.lfHeight);
//		DrawPramSetka(pDevice,ptaInternal,&GetObhvatRect(pta),&drawsym.str,step,lfont.lfHeight);
//  vipa 2004.09.16
    rectangle = GetObhvatRect(pta);
    pDevice-> TextOutPramSetka(ptaInternal,&rectangle,&drawsym.str,step,lfont.lfHeight);
		break;
	case STK_KOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-lfont.lfHeight);
		else
		ptaInternal = BuildObvod(pta,lfont.lfHeight);
//		DrawKosSetka(pDevice,ptaInternal,&GetObhvatRect(pta),&drawsym.str,angle,step,lfont.lfHeight);
//  vipa 2004.09.16
    rectangle = GetObhvatRect(pta);
    pDevice-> TextOutKosSetka(ptaInternal,&rectangle,&drawsym.str,angle,step,lfont.lfHeight);
		break;
	case STK_XAOS:
		if(Area(pta)<0)                 
			ptaInternal = BuildObvod(pta,-(lfont.lfHeight+step/2));
		else
			ptaInternal = BuildObvod(pta,(lfont.lfHeight+step/2));
//		  DrawXaosSetka(pDevice,ptaInternal,&GetObhvatRect(pta),&drawsym,step,lfont.lfHeight);
//  vipa 2004.09.16
    rectangle = GetObhvatRect(pta);
    pDevice-> TextOutXaosSetka(ptaInternal,&rectangle,&drawsym,step,lfont.lfHeight);
		break;
	}
	delete ptaInternal;
	pDevice->RestoreFont();
	pDevice->SetBkMode(uiOldMode);
  pDevice->SetTextAlign(uiOldAlign);
	pDevice->SetTextColor(crOldTextColor);
}

//=======================================================================
//! Drawing of a grid of symbols
void MDrawing::DrawPramSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
	MString* str,double step,double dSymVal)
//=======================================================================
{
    //define borders of drawing of a grid (that is knots which participate in
    //to drawing
    //It is meant that drawing goes from left to right and from top to down
    //It is required to find the interval of knots displayed at present
    //so that the arrangement of knots didn't depend on a zoom
	rect->NormalizeGeoRect();

	double dMinX,dMaxX,dMinY,dMaxY;
    //receive visible area in real coordinates
	MRect rectGeoVis;
	pDevice->GetClientRect(&rectGeoVis);
	rectGeoVis.NormalizeGeoRect();


	dMinX = rect->left;
	while((dMinX<rectGeoVis.left)&&(dMinX<rect->right))
		dMinX +=step;
	dMinX-=step;
	dMaxX = dMinX;
	while((dMaxX<rectGeoVis.right)&&(dMaxX<rect->right))
		dMaxX+=step;

	dMinY = rect->bottom;
	while((dMinY<rectGeoVis.bottom)&&(dMinY<rect->top))
		dMinY +=step;
	dMinY-=step;
	dMaxY = dMinY;
	while((dMaxY<rectGeoVis.top)&&(dMaxY<rect->top))
		dMaxY+=step;
		
	double x,y;
	MPoint draw;  //,pttemp;
//	int itemp,size = 1;
    // draw a grid of symbols parallel to axes of coordinates, providing a step
	for(x=dMinX;x<=dMaxX;x+=step)
		for(y=dMinY;y<=dMaxY;y+=step){
			draw = MPoint(x,y);
			DrawTextWithinArea(pDevice,draw,pta,str,dSymVal);
		}

}

//=======================================================================
//! Drawing of a grid of symbols
void MDrawing::DrawKosSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
	MString* str,double angle,double step,double dSymVal)
//=======================================================================
{
    //we lead a angle to a normal value
	while(angle>=360)angle-=360;
	while(angle<0)angle+=360;

    //if a grid after all a straight line, we send drawing there
	if((angle==0)||(angle==90)||(angle==180)||(angle==270))
	{
		DrawPramSetka(pDevice,pta,rect,str,step,dSymVal);
		return;
	}

    //find grid knots
    //search is carried out so:
    //      the equations of straight lines of a type of Y=kX+b are worked out,
    //      where k1 and k2 - tangents of angle of an inclination and perpendicular to it a angle
    //      then ranges of change of the b1 and b2 parameters pay off,
    //      and also their step providing distance between grid knots.
    //      Then in a cycle in the parameters b there are points of intersection
    //      each straight line.
    //      In these points of intersection the symbol is also drawn

	double b1min,b2min,b1max,b2max,db1,db2,k1,k2,b1,b2;
	{
		k1=tan(angle*pi/180);
        //find the range of change of members of b1 and b2 and their step
		double b1=rect->bottom-k1*rect->left;
		double b2=rect->top-k1*rect->right;
		double b3=rect->bottom-k1*rect->right;
		double b4=rect->top-k1*rect->left;
		db1=fabs(step*sqrt(k1*k1+1.0));
		b1min=Min(Min(b1,b2),Min(b3,b4));
		b1max=Max(Max(b1,b2),Max(b3,b4));
		
		k2=-1/k1;
		b1=rect->bottom-k2*rect->left;
		b2=rect->top-k2*rect->right;

		b3=rect->bottom-k2*rect->right;
		b4=rect->top-k2*rect->left;
		db2=fabs(step*sqrt(k2*k2+1.0));
		b2min=Min(Min(b1,b2),Min(b3,b4));
		b2max=Max(Max(b1,b2),Max(b3,b4));
	}



  //draw a symbol in knots of a grid
	double x,y;
//cout <<"KosSetka- "<< (int)(b1max-b1min)/db1 * (b2max-b2min)/db2 <<endl;
  for(b1=b1min;b1<=b1max;b1+=db1)
		for(b2=b2min;b2<=b2max;b2+=db2)
		{
            //find crossing of two straight lines
			x=(b2-b1)/(k1-k2);
			y=k2*x+b2;

			DrawTextWithinArea(pDevice,MPoint(x,y),pta,str,dSymVal);
            //draw a symbol
		}

}

//=======================================================================
//! Drawing of a chaotic grid of symbols
void MDrawing::DrawXaosSetka(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
	MSymbol* sym,double step,double dSymVal)
//=======================================================================
{
	
    //define the maximum shift of symbols concerning the center
	double dr=(step/2-dSymVal);
    //if shift is less than zero, is accepted its equal to zero
	if(dr<0) dr=0;


    //define borders of drawing of a grid
	double xmin=Min(rect->left,rect->right);
	double xmax=Max(rect->left,rect->right);
	double ymin=Min(rect->top,rect->bottom);
	double ymax=Max(rect->top,rect->bottom);
	double x,y;


    //draw a grid of symbols also, as well as a straight line,
    //but symbols we have in a random way
    //with shift on each of axes.
    //The angle of a symbol is also random
	M_LOG_FONT lfont = sym->logfont;
	lfont.lfHeight = dSymVal;

    //The array of various fonts for turn is created
    //it is used for drawing speed
    //as font choice significantly quicker than its creation
	for(int i = 0 ;i<12;i++){
		lfont.lfEscapement=i*300;
		pDevice->AddFontToPool(&lfont,i);
	}
	pDevice->SaveFont();
	pDevice->SelectFontFromPool(0);

//	int randv;
	double xx,yy;
	srand(1);
    //concerning each knot of a rectangular grid
	for(x=xmin;x<=xmax;x+=step)
		for(y=ymin;y<=ymax;y+=step)
		{
            //define shift along each axis
			xx=x+dr*((double)rand()/(double)RAND_MAX*2-1);
			yy=y+dr*((double)rand()/(double)RAND_MAX*2-1);
            //define an angle of rotation
			pDevice->SelectFontFromPool((int)((double)rand()/(double)RAND_MAX*12));
			DrawTextWithinArea(pDevice,MPoint(xx,yy),pta,&sym->str,step/2);
		}
	pDevice->RestoreFont();
	pDevice->ClearPool();
}

// ==================================================================
//! Drawing of a percentage pattern
void MDrawing::DrawPercentUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MPercentUzor* pUzor,MPtArray* pta)
// ==================================================================
{
  MRect rectangle;
    //define a step of shading and its thickness
	double step=fabs((*ConvertSizeToReal)(pObj,pUzor->m_shag));
		
//	MSymbol drawsym;
//	drawsym.Copy(&pUzor->m_symbol);
	
    //create fonts for drawing of all symbols in percentage drawing
	PercentParameters params;
	params.FirstSym.Copy(&pUzor->m_symbol);	
	params.SecSym.Copy(&pUzor->m_SecSym);	
	params.ThirdSym.Copy(&pUzor->m_ThirdSym);	

//For PlotDevice we create three fonts on a pool and we transfer in structure only indexes of fonts
    //in a pool

	double d1H,d2H,d3H;

	M_LOG_FONT lfont = params.FirstSym.logfont;
	d1H = lfont.lfHeight = (*ConvertSizeToReal)(pObj,params.FirstSym.value);
	pDevice->AddFontToPool(&lfont,400);
	params.iFirstFont = 400;
    //If a grid chaotic, all 12 fonts of turn are created
	if(pUzor->m_type==STK_XAOS)
		for(int i = 1;i<12;i++){
			lfont.lfEscapement +=300;
			pDevice->AddFontToPool(&lfont,params.iFirstFont+i);
		}


	lfont = params.SecSym.logfont;
	d2H = lfont.lfHeight = (*ConvertSizeToReal)(pObj,params.SecSym.value);
	pDevice->AddFontToPool(&lfont,412);
	params.iSecFont = 412;
    //If a grid chaotic, all 12 fonts of turn are created
	if(pUzor->m_type==STK_XAOS)
		for(int i = 1;i<12;i++){
			lfont.lfEscapement +=300;
			pDevice->AddFontToPool(&lfont,params.iSecFont+i);
		}

	lfont = params.ThirdSym.logfont;
	d3H = lfont.lfHeight = (*ConvertSizeToReal)(pObj,params.ThirdSym.value);
	pDevice->AddFontToPool(&lfont,424);
	params.iThirdFont = 424;
    //If a grid chaotic, all 12 fonts of turn are created
	if(pUzor->m_type==STK_XAOS)
		for(int i = 1;i<12;i++){
			lfont.lfEscapement +=300;
			pDevice->AddFontToPool(&lfont,params.iThirdFont+i);
		}

	params.iFirstPercent = pUzor->m_iPercent1;
	params.iSecPercent = pUzor->m_iPercent2;
	params.iThirdPercent = pUzor->m_iPercent3;


    //If it is invisible, it isn't displayed (on percent gets to not displayed area)
	if ((!params.FirstSym.visible)||(!pDevice->Distinguish(d1H/VISIBLE_FONT_UZOR)))
		params.iFirstPercent = 0;
	if ((!params.SecSym.visible)||(!pDevice->Distinguish(d2H/VISIBLE_FONT_UZOR)))
		params.iSecPercent = 0;
	if ((!params.ThirdSym.visible)||(!pDevice->Distinguish(d3H/VISIBLE_FONT_UZOR)))
		params.iThirdPercent = 0;


	MColor color;
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)params.FirstSym.colorkod, color))
		color.SetColor(0,0,0);
	
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}

	params.cr1 = color;
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)params.SecSym.colorkod, color))
		color.SetColor(0,0,0);
	
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}

	params.cr2   = color;
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)params.ThirdSym.colorkod, color))
		color.SetColor(0,0,0);
	
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}
	
	params.cr3 = color;


	pDevice->SaveFont();
    //set the mode of drawing of symbols necessary to us

	double symValReal = Max(Max(d1H,d2H),d3H);

    // it is drawn at joint performance of two conditions:
    //if it is marked as visible
    //and if it is readable in this scale
	if((params.iFirstPercent==0)&&(params.iSecPercent==0)&&(params.iThirdPercent==0)){
		pDevice->RestoreFont();
		pDevice->ClearPool();
		return;
	}
	pDevice->SelectFontFromPool(400);

	unsigned int uiOldMode = pDevice->SetBkMode(UT_TRANSPARENT);
	unsigned int uiOldAlign= pDevice->SetTextAlign(UT_TA_CENTER);
	MColor crOldTextColor = pDevice->SetTextColor(MColor(0,0,0));
    //the angle of a grid is expressed in degrees
	double angle=0;
	if(pUzor->m_type==STK_KOS)
		angle=pUzor->m_ugol;

	MPtArray* ptaInternal =NULL;
    //now directly drawing depending on grid type
	switch(pUzor->m_type)
	{
	case STK_PRAM:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-symValReal);
		else
			ptaInternal = BuildObvod(pta,symValReal);
   rectangle = GetObhvatRect(pta);
		DrawPramSetkaPercent(pDevice,ptaInternal,&rectangle,&params,step,symValReal);
		break;
	case STK_KOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-symValReal);
		else
			ptaInternal = BuildObvod(pta,symValReal);
   rectangle = GetObhvatRect(pta);
		DrawKosSetkaPercent(pDevice,ptaInternal,&rectangle,&params,angle,step,symValReal);
		break;

	case STK_XAOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-(symValReal+step/2));
		else
			ptaInternal = BuildObvod(pta,(symValReal+step/2));
   rectangle = GetObhvatRect(pta);
		DrawXaosSetkaPercent(pDevice,ptaInternal,&rectangle,&params,step,symValReal);
		break;
	}
	delete ptaInternal;
	pDevice->RestoreFont();
	pDevice->ClearPool();
	pDevice->SetBkMode(uiOldMode);
    pDevice->SetTextAlign(uiOldAlign);
	pDevice->SetTextColor(crOldTextColor);
}

//=======================================================================
//! Drawing of a grid of symbols
void MDrawing::DrawPramSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
	PercentParameters* params,double step,double dSymVal)
//=======================================================================
{
    //Define borders of drawing of a grid (that is knots which participate in
    //to drawing
    //It is meant that drawing goes from left to right and from top to down
    //It is required to find the interval of knots displayed at present
    //so that an arrangement of knots I didn't depend on a zoom
	rect->NormalizeGeoRect();
	
	double dMinX,dMaxX,dMinY,dMaxY;
    //Receive visible area in real coordinates
	dMinX = rect->left;
	dMaxX = rect->right;
	dMinY = rect->bottom;
	dMaxY = rect->top;

	double x,y;
	MPoint draw; //,pttemp;

//	int itemp,size = 1;
    //Draw a grid of symbols parallel to axes of coordinates, providing a step
	srand(1);
	int iPointOnPercentLine;
	int iFirstBorder = (int)(((double)params->iFirstPercent)/100*RAND_MAX);
	int iSecBorder = (int)(((double)params->iFirstPercent + params->iSecPercent)/100*RAND_MAX);
	int iThirdBorder = (int)(((double)params->iFirstPercent +params->iSecPercent+ params->iThirdPercent)/100*RAND_MAX);

    //Percentage drawing
	for(x=dMinX;x<=dMaxX;x+=step)
		for(y=dMinY;y<=dMaxY;y+=step){
			draw = MPoint(x,y);
			iPointOnPercentLine = rand();
			if(iPointOnPercentLine<=iFirstBorder){
				pDevice->SelectFontFromPool(params->iFirstFont);
				pDevice->SetTextAlign(params->FirstSym.align);
				pDevice->SetTextColor(params->cr1);
				DrawTextWithinArea(pDevice,draw,pta,&params->FirstSym.str,dSymVal);
			}
			else if(iPointOnPercentLine<=iSecBorder){
				pDevice->SelectFontFromPool(params->iSecFont);
				pDevice->SetTextAlign(params->SecSym.align);
				pDevice->SetTextColor(params->cr2);
				DrawTextWithinArea(pDevice,draw,pta,&params->SecSym.str,dSymVal);
			}
			else if(iPointOnPercentLine<=iThirdBorder){
				pDevice->SelectFontFromPool(params->iThirdFont);
				pDevice->SetTextAlign(params->ThirdSym.align);
				pDevice->SetTextColor(params->cr3);
				DrawTextWithinArea(pDevice,draw,pta,&params->ThirdSym.str,dSymVal);
			}
		}

}

//=======================================================================
//! Drawing of a grid of symbols
void MDrawing::DrawKosSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
	PercentParameters* params,double angle,double step,double dSymVal)
//=======================================================================
{
    //Lead an angle to a normal amount
	while(angle>=360)angle-=360;
	while(angle<0)angle+=360;

    //If a grid after all a straight line, we send drawing there
	if((angle==0)||(angle==90)||(angle==180)||(angle==270))
	{
		DrawPramSetkaPercent(pDevice,pta,rect,params,step,dSymVal);
		return;
	}

    //will find grid knots
    //Search is carried out so:
    //the equations of straight lines of a type of Y=kX+b are worked out,
    //where k1 and k2 - tangents of angle of an inclination and perpendicular to it an angle
    //then ranges of change of the b1 and b2 parameters are calculated,
    //and also their step providing distance between grid knots.
    //Then in a cycle in the parameters b points of intersection each straight line
    //are calculate.
    //In these points of intersection the symbol is also drawn.

	double b1min,b2min,b1max,b2max,db1,db2,k1,k2,b1,b2;
	{
		k1=tan(angle*pi/180);
        //find the range of change of members of b1 and b2 and their step
		double b1=rect->bottom-k1*rect->left;
		double b2=rect->top-k1*rect->right;
		double b3=rect->bottom-k1*rect->right;
		double b4=rect->top-k1*rect->left;
		db1=fabs(step*sqrt(k1*k1+1.0));
		b1min=Min(Min(b1,b2),Min(b3,b4));
		b1max=Max(Max(b1,b2),Max(b3,b4));
		
		k2=-1/k1;
		b1=rect->bottom-k2*rect->left;
		b2=rect->top-k2*rect->right;
		b3=rect->bottom-k2*rect->right;
		b4=rect->top-k2*rect->left;
		db2=fabs(step*sqrt(k2*k2+1.0));
		b2min=Min(Min(b1,b2),Min(b3,b4));
		b2max=Max(Max(b1,b2),Max(b3,b4));
	}


	srand(1);
	int iPointOnPercentLine;
	int iFirstBorder = (int)(((double)params->iFirstPercent)/100*RAND_MAX);
	int iSecBorder = (int)(((double)params->iFirstPercent + params->iSecPercent)/100*RAND_MAX);
	int iThirdBorder = (int)(((double)params->iFirstPercent +params->iSecPercent+ params->iThirdPercent)/100*RAND_MAX);

	MPoint draw;
    //draw a symbol in grid knots
//	double x,y;
	for(b1=b1min;b1<=b1max;b1+=db1)
		for(b2=b2min;b2<=b2max;b2+=db2)
		{
            //find crossing of two straight lines
			draw.x=(b2-b1)/(k1-k2);
			draw.y=k2*draw.x+b2;
			iPointOnPercentLine = rand();
			if(iPointOnPercentLine<=iFirstBorder){
				pDevice->SelectFontFromPool(params->iFirstFont);
				pDevice->SetTextAlign(params->FirstSym.align);
				pDevice->SetTextColor(params->cr1);
				DrawTextWithinArea(pDevice,draw,pta,&params->FirstSym.str,dSymVal);
			}
			else if(iPointOnPercentLine<=iSecBorder){
				pDevice->SelectFontFromPool(params->iSecFont);
				pDevice->SetTextAlign(params->SecSym.align);
				pDevice->SetTextColor(params->cr2);
				DrawTextWithinArea(pDevice,draw,pta,&params->SecSym.str,dSymVal);
			}
			else if(iPointOnPercentLine<=iThirdBorder){
				pDevice->SelectFontFromPool(params->iThirdFont);
				pDevice->SetTextAlign(params->ThirdSym.align);
				pDevice->SetTextColor(params->cr3);
				DrawTextWithinArea(pDevice,draw,pta,&params->ThirdSym.str,dSymVal);
			}
		}
}

//=======================================================================
//! Drawing of a chaotic grid of symbols
void MDrawing::DrawXaosSetkaPercent(MPlotDevice* pDevice,MPtArray* pta,MRect* rect,
	PercentParameters* params,double step,double dSymVal)
//=======================================================================
{
	
    //define the maximum shift of symbols concerning the center
	double dr=(step/2-dSymVal);
    //if shift is less than zero, is accepted its equal to zero
	if(dr<0) dr=0;

    //define borders of drawing of a grid
	double xmin=Min(rect->left,rect->right);
	double xmax=Max(rect->left,rect->right);
	double ymin=Min(rect->top,rect->bottom);
	double ymax=Max(rect->top,rect->bottom);
	double x,y;
	pDevice->SaveFont();

	int iPointOnPercentLine;
	int iFirstBorder = (int)(((double)params->iFirstPercent)/100*RAND_MAX);
	int iSecBorder = (int)(((double)params->iFirstPercent + params->iSecPercent)/100*RAND_MAX);
	int iThirdBorder = (int)(((double)params->iFirstPercent +params->iSecPercent+ params->iThirdPercent)/100*RAND_MAX);

//	int randv;
	double xx,yy;
	srand(1);
    //concerning each knot of a rectangular grid
	for(x=xmin;x<=xmax;x+=step)
		for(y=ymin;y<=ymax;y+=step)
		{
            //define shift along each axis
//			randv=rand();
			xx=x+dr*((double)rand()/(double)RAND_MAX*2-1);
			yy=y+dr*((double)rand()/(double)RAND_MAX*2-1);

			iPointOnPercentLine = rand();
			if(iPointOnPercentLine<=iFirstBorder){
				pDevice->SetTextAlign(params->FirstSym.align);
				pDevice->SetTextColor(params->cr1);
                //define an angle of rotation
				pDevice->SelectFontFromPool(params->iFirstFont+((int)((double)rand()/(double)RAND_MAX*11)));
				DrawTextWithinArea(pDevice,MPoint(xx,yy),pta,&params->FirstSym.str,dSymVal);
			}
			else if(iPointOnPercentLine<=iSecBorder){
				pDevice->SetTextAlign(params->SecSym.align);
				pDevice->SetTextColor(params->cr2);
                //define an angle of rotation
				pDevice->SelectFontFromPool(params->iSecFont+(int)((double)rand()/(double)RAND_MAX*11));
				DrawTextWithinArea(pDevice,MPoint(xx,yy),pta,&params->SecSym.str,dSymVal);
			}
			else if(iPointOnPercentLine<=iThirdBorder){
				pDevice->SetTextAlign(params->ThirdSym.align);
				pDevice->SetTextColor(params->cr3);
                //define an angle of rotation
				pDevice->SelectFontFromPool(params->iThirdFont+(int)((double)rand()/(double)RAND_MAX*11));
				DrawTextWithinArea(pDevice,MPoint(xx,yy),pta,&params->ThirdSym.str,dSymVal);
			}
                //define an angle of rotation
		}
	pDevice->RestoreFont();
}

//=======================================================================
void MDrawing::DrawHandle(MTopobject* pObj, MPlotDevice* pDevice, MHandleArea* pHandle, MPoint point)
//=======================================================================
{
	MRect rect = pObj->m_bounds;
	
    //set the mode of drawing of symbols necessary to us
	MSymbol drawsym;
	drawsym.Copy(&pHandle->m_symbol);
	
    // symbol font
	unsigned int uiOldMode  = pDevice->SetBkMode(UT_TRANSPARENT);
	unsigned int uiOldAlign = pDevice->SetTextAlign(drawsym.align);
	
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)drawsym.colorkod, color))
		color.SetColor(0,0,0);
	
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;

	if((pObj->m_selected)&&(color != whiteclr))      
		color = pObj->m_sel_color;
	
	MColor crOldTextColor = pDevice->SetTextColor(color);
	
	M_LOG_FONT lfont = drawsym.logfont;
	if(drawsym.scaled)
		lfont.lfHeight = (*ConvertSizeToReal)(pObj,drawsym.value);
	else
		lfont.lfHeight = pDevice->LogicToBum(drawsym.value);
	
    //it is drawn at joint performance of two conditions:
    //if it is marked as visible
    //and if it is readable in this scale
	if ((!drawsym.visible)||(!pDevice->Distinguish(lfont.lfHeight/VISIBLE_FONT_HANDLE))) 
		return;
	
    //angle
//	double angle=0;

	pDevice->SaveFont();
	pDevice->SetFont(&lfont);	
	
	pDevice->TextOut(point.x, point.y, drawsym.str, drawsym.str.GetLength());
	
	pDevice->RestoreFont();
	pDevice->SetBkMode(uiOldMode);
	pDevice->SetTextAlign(uiOldAlign);
	pDevice->SetTextColor(crOldTextColor);
}

//=======================================================================
void MDrawing::DrawNastrHandle(MTopobject* pObj, MPlotDevice* pDevice, MNastrHandleArea* pHandle, MPoint point)
//=======================================================================
{	
	MRect rect = pObj->m_bounds;
	
    //set the mode of drawing of symbols necessary to us
	MSymbol drawsym;
	drawsym.Copy(&pHandle->m_symbol);
	
	
    //we will receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)drawsym.colorkod, color))
		color.SetColor(0,0,0);
	
    //redefine a symbol
	if(pHandle->IsPereoprIndex(IDL_NAS_PLAST_HANDLE_SYMBOL))
	{
		MString str = pObj->GetDataPole(pHandle->GetPerIndex(IDL_NAS_PLAST_HANDLE_SYMBOL));
		MString strtrue;
		MZnak* pZnak = pObj->GetKarta()->m_pKls->GetZnak(str, strtrue, true);
		if((pZnak)&&(pZnak->GetYaz()->GetType() == F_DOT))
		{
			MSzArray* szArr = ((MDotYaz*)(pZnak->GetYaz())->m_object)->m_tochka;
			MShriftSrez* pSrez = (MShriftSrez*)szArr->GetTail();
			drawsym.Copy(&pSrez->m_tochka);
		}
	}
	
    // symbol font
	unsigned int uiOldMode  = pDevice->SetBkMode(UT_TRANSPARENT);
	unsigned int uiOldAlign = pDevice->SetTextAlign(drawsym.align);
	
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))      
		color = pObj->m_sel_color;
	
	MColor crOldTextColor = pDevice->SetTextColor(color);

	M_LOG_FONT lfont = drawsym.logfont;
	
    //If there is a redefinition of a symbol
    //If the sizes or an angle of rotation of a symbol are redefined
	
	double dTemp = drawsym.value;
	if(pHandle->IsPereoprIndex(IDL_NAS_PLAST_HANDLE_VAL))
		pObj->GetDoubleDataPole(pHandle->GetPerIndex(IDL_NAS_PLAST_HANDLE_VAL),dTemp);
	
	if(drawsym.scaled) lfont.lfHeight = (*ConvertSizeToReal)(pObj,dTemp);
	else lfont.lfHeight = pDevice->LogicToBum(dTemp);
    //It is drawn at joint performance of two conditions:
    //if it is marked as visible
    //and if it is readable in this scale
	if ((!drawsym.visible)||(!pDevice->Distinguish(lfont.lfHeight/VISIBLE_FONT_HANDLE))) 
		return;
	
	if(pHandle->IsPereoprIndex(IDL_NAS_PLAST_HANDLE_UGOL)){
		if(pObj->GetDoubleDataPole(pHandle->GetPerIndex(IDL_NAS_PLAST_HANDLE_UGOL),dTemp))
			lfont.lfEscapement = (int)(dTemp*10);
	}
    //angle
//	double angle=0;
	
	pDevice->SaveFont();
	pDevice->SetFont(&lfont);	
	
	pDevice->TextOut(point.x, point.y, drawsym.str, drawsym.str.GetLength());
	
	pDevice->RestoreFont();
	pDevice->SetBkMode(uiOldMode);
	pDevice->SetTextAlign(uiOldAlign);
	pDevice->SetTextColor(crOldTextColor);
}

//=======================================================================
//! Drawing of a symbol with belonging to a contour
//parameters (MPlotDevice* pDevice,MPoint pt,MPtArray* pta,MString* str,double dSymVal)
void MDrawing::DrawTextWithinArea(MPlotDevice* pDevice,MPoint pt,MPtArray* pta,MString* str,double )
//=======================================================================
{ // return;

	if(pt.InnerPtFast(pta->GetHead(),pta->GetSize())){
		pDevice->TextOut(pt.x,pt.y, str[0], str->GetLength());
	}
}

//=======================================================================
//! Drawing of the line on layers
void MDrawing::DrawLine(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,MPtArray* pta,MRzArray* razryvy)
//=======================================================================
{
	MPrArray* pra =NULL;
    //get access to the array of layers
	switch(pYaz->GetType())
	{
	case F_PLO:
		pra=((MPloYaz*)(pYaz->m_object))->m_linia;
		break;
	case F_LIN:
		pra=((MLinYaz*)(pYaz->m_object))->m_linia;
		break;
	}
	
	int Cnt=0;
	if (pra) Cnt= pra->GetSize();
	
	MInArray* ina = new MInArray();
	bool isRedef = false;
	
	int i;
	for(i=0;i<Cnt;i++)
	{
		MProsloika* prosloika=pra->GetAt(i);
		if((prosloika->GetType() == IDL_SINGULAR)||(prosloika->GetType() == IDL_NAS_SINGULAR))
		{
			ina->AddTail(&i);
			if(((MSbSingular*)prosloika)->m_pause == -1) 
			{
				isRedef=true;
				((MSbSingular*)prosloika)->m_symbol.bkmode = UT_OPAQUE;
				((MSbSingular*)prosloika)->m_symbol.bkcolorkod = pObj->GetKarta()->m_papercolorkod;
			}
		}
	}
	//redefinition m_pause
	if(isRedef)
	{
		int iSize = ina->GetSize();
		for(i=0;i<iSize;i++)
			((MSbSingular*)pra->GetAt(*ina->GetAt(i)))->m_pause = ((float)(i+1))/((float)(iSize+1));
	}
	
    //drawing
	for(i=0;i<Cnt;i++)
	{
		MProsloika* prosloika=pra->GetAt(i);
		if(!pYaz->IsPls())
			DrawProsloika(pObj,pDevice,prosloika,pta,razryvy);
		else
			DrawProsloikaPls(pObj,pDevice,prosloika,pta,razryvy);
	}

	delete ina;

    //drawing of the form for an ActLayer
	if((pObj->m_pLay->m_bOperLayer) &&(pYaz->GetType() == F_LIN))
	{
		double dlinalom = DLINALOMLINE(pta);
        //receive the form
		MYaz* m_pYaz = pObj->m_pZnak->GetYaz();
		MFormArray* forma = (MFormArray*)m_pYaz->m_formula;
		for(int i=0; i<forma->GetSize(); i++)
		{
			MFormula* formula = forma->GetAt(i);
			if(GetFormulaDlina(pObj, pDevice, formula))
			{
				for(int ii=0; ii<formula->fragm_arr->GetSize(); ii++)
				{
					MFragment* fragm = formula->fragm_arr->GetAt(i);
					if(fragm->GetType() == ID_FRAG_FRACTION)
					{
							pObj->DeletePrivs(i,SIMPLE_PRIVYAZKA);
							MPoint point = GetPointBySdvig(pta,dlinalom/2);
							MPtArray privarr;
							privarr.ClearAll();							
							MPoint tmpPt(point.x, point.y);
							privarr.AddTail(&tmpPt);
                                                        //privarr.AddTail(&MPoint(point.x, point.y));
							pObj->AddPriv(i,privarr);
						/*double vysotaUp = GetFragmentVysotaUp(pObj,pDevice,fragm);
						if(vysotaUp) 
						{
							pObj->DeletePrivs(i,SIMPLE_PRIVYAZKA);
							MPoint point = GetPointBySdvig(pta,dlinalom/2);
							MPtArray privarr;
							privarr.ClearAll();							
							privarr.AddTail(&MPoint(point.x, point.y-vysotaUp));
							pObj->AddPriv(i,privarr);
						}*/
					}
				}				
			}				
		}
	}	
}





//=======================================================================
//! Drawing of a layer
void MDrawing::DrawProsloika(MTopobject* pObj,MPlotDevice* pDevice,MProsloika* prosloika,MPtArray* pta, MRzArray* razryvy)
//=======================================================================
{
	MPtArray* ptar = (MPtArray*)pta->Clone();

    //draw a layer depending on type
    //give the axial array to synchronize layers
	switch(prosloika->GetType())
	{
	case IDL_LINIA:
	case IDL_SHTRIH:
	case IDL_PUNKTIR:
	case IDL_SBSTRETCH:
	case IDL_SPLINE:
	case IDL_ORIENTED:
	case IDL_NAS_LINIA:
	case IDL_NAS_SHTRIH:
	case IDL_NAS_PUNKTIR:
	case IDL_NAS_SBSTRETCH:
	case IDL_NAS_SPLINE:
		{
            int n = 0;//number of breaks
			if(razryvy!=NULL)
				n = razryvy->GetSize();
            //if the contour has breaks and this layer considers breaks when drawing
			if((n>0)&&(prosloika->m_drawrazryv)){
                //form the array for drawing by the line - the line is drawn to breaks
                //on a place of a break it is passed, and then renews
				MPtArray* ptarray = new MPtArray();
                //rzind - index of an edge of a contour of top object on which it breaks
				int rzind;
				MRazryv* rz = razryvy->GetAt(0);
                //ind - index of an edge of a contour of top object from which we continue to draw the line
                //we start drawing always after the first break, as the last breaks
                //can pass for a zero index for the closed top objects and contours
                //vulgar objects
				int ind = rz->m_indlast;
                //we will store the first index of the previous break in the variable 'lastind'
//				int lastind = rz->m_indfirst;
                //drawing of the line between breaks always begins with a final point
                //the previous break
				ptarray->AddTail(&rz->m_ptlast);
                //cycle on all breaks...
				for(int j=1; j<n; j++){
					MRazryv* rz = razryvy->GetAt(j);
					rzind = rz->m_indfirst;
                    //we form the array of points for drawing by the line to a break;
                    //the last in the array we include the first point of a break;
                    //if beginning of the following and end previous breaks
                    //lie on one edge, for drawing to the array of points to the last
                                   //the point of the previous break should add only the first point
                    //following break
					if(rzind!=ind-1){
						for(int i=ind; i<ptar->GetSize(); i++){
							if(i!=rzind){
								ptarray->AddTail(ptar->GetAt(i));
							}
							else{
								ptarray->AddTail(ptar->GetAt(i));
								break;
							}
						}
					}
					ptarray->AddTail(&rz->m_ptfirst);

					DrawProsloikaOriginal(pObj,pDevice,prosloika,ptarray);
                    //clean the array of points for formation of a new segment of the line
					ptarray->ClearAll();
                    //in a new segment the first point - the point noting the end of a break
					ptarray->AddTail(&rz->m_ptlast);
                    //the second point in a new segment - the first point of a contour after a point,
                    //noting the end of a break; rz->m_indlast - an index of this point
					ind = rz->m_indlast;
                    //remember the first index of the current break for comparison
                    //with the first index of the following
//					lastind = rz->m_indfirst;
				}
                //DRAWING OF THE LINE AFTER THE FINAL BREAK TO THE INITIAL BREAK
				rz = razryvy->GetAt(0);
                //compare the second index of the last break to the first index
                //initial break:
                //if the second index of the last break is less than first index
                //an initial break, in the last breaks there was a transition through
                //a zero index, it is also necessary to draw initial part of a contour
                //top object from the second point of the last break to the first point
                //initial break
				if(ind<rz->m_indfirst+1){
                    //form and draw the last segment of a contour of top object;
                    //if beginning zero and end of the last of breaks
                    //lie on one edge, for drawing to the array of points to the last
                    //the point of the last break should add only the first point
                    //zero break
					for(int i=ind; i<=rz->m_indfirst; i++){
						ptarray->AddTail(ptar->GetAt(i));
					}
					ptarray->AddTail(&rz->m_ptfirst);
					DrawProsloikaOriginal(pObj,pDevice,prosloika,ptarray);

				}
                //if initial and final breaks lie on one edge or only
                //the break settles down on one edge
				else if((ind==rz->m_indfirst+1)&&(*ptar->GetAt(0)==*ptar->GetAt(ptar->GetSize()-1))){
					if(DLINASG(*ptar->GetAt(rz->m_indfirst),rz->m_ptfirst)<=
						DLINASG(*ptar->GetAt(rz->m_indfirst),*ptarray->GetAt(0))){
						int i = 0;
						for(i=ind; i<ptar->GetSize(); i++)
							ptarray->AddTail(ptar->GetAt(i));
						for(i=0; i<=rz->m_indfirst; i++)
							ptarray->AddTail(ptar->GetAt(i));
					}
					ptarray->AddTail(&rz->m_ptfirst);
					DrawProsloikaOriginal(pObj,pDevice,prosloika,ptarray);
				}
                //if the final point of the last break lies on an edge with an index,
                //bigger than index of an edge of the initial point of the first break, transition through
                //a zero index wasn't, and it is necessary to draw a contour tail
                //from the second point of the last break and initial part of a contour
                //top object to the first point of an initial break
                //for not closed contour drawing is right and in a case, when the initial
                    //and final breaks lie on one edge or only
                    //the break settles down on one edge
				else if((ind>rz->m_indfirst+1)||
					((ind==rz->m_indfirst+1)&&(*ptar->GetAt(0)!=*ptar->GetAt(ptar->GetSize()-1)))){
                    //form and draw the last segment of a contour of top object
					int i = 0;
					for(i=ind; i<ptar->GetSize(); i++){
						ptarray->AddTail(ptar->GetAt(i));
					}
					DrawProsloikaOriginal(pObj,pDevice,prosloika,ptarray);
                    //clean the array of points for formation of a new segment of the line
					ptarray->ClearAll();
                    //form and draw an initial segment of a contour of top object
					for(i=0; i<=rz->m_indfirst; i++){
						ptarray->AddTail(ptar->GetAt(i));
					}
					ptarray->AddTail(&rz->m_ptfirst);
					DrawProsloikaOriginal(pObj,pDevice,prosloika,ptarray);
				}
				delete ptarray;
			}
            //if the contour of a top object has no breaks
			else{
				DrawProsloikaOriginal(pObj,pDevice,prosloika,ptar);
			}
		}
		break;
	case IDL_SINGULAR:
	case IDL_NAS_SINGULAR:
	case IDL_SOFRANGE:
		{
			DrawProsloikaOriginal(pObj,pDevice,prosloika,ptar);
		}
		break;
	case IDL_ALGORITM:
		break;

	}
	
	delete ptar;
}

//=======================================================================
//! Drawing of a layer
void MDrawing::DrawProsloikaOriginal(MTopobject* pObj,MPlotDevice* pDevice,MProsloika* prosloika,MPtArray* pta)
//=======================================================================
{
    //if points less than two, we draw nothing
	if(pta->GetSize()<2)return;

    //create two arrays:
    //ptar - axial
    //ptarr - covering
	//******************************************************
	MPtArray* ptar = (MPtArray*)pta->Clone();
	double dRealOffsetFromAxe = (*ConvertSizeToReal)(pObj,prosloika->m_distofaxe);
	if(prosloika->IsPereopr())
        //For the recustomized layers shift from an axis can be another
		if(prosloika->GetBaseNast()->IsPereoprIndex(IDL_NAS_PROSLOIKA_VALUE))
			pObj->GetDoubleDataPole(prosloika->GetBaseNast()->GetPerIndex(IDL_NAS_PROSLOIKA_VALUE),dRealOffsetFromAxe);
	MPtArray* ptarr = BuildObvod(ptar,dRealOffsetFromAxe);
    //draw a layer depending on type
    //we give the axial array to synchronize layers
	switch(prosloika->GetType())
	{
	case IDL_LINIA:
		DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
		break;
	case IDL_SHTRIH:
		DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptar,prosloika->m_synhro);
		break;
	case IDL_PUNKTIR:
		DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptar,prosloika->m_synhro);
		break;
	case IDL_SBSTRETCH:
		DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptar,prosloika->m_synhro);
		break;
	case IDL_SOFRANGE:
		DrawRangeSym(pObj,pDevice,(MRangeSym*)prosloika,ptarr);
		break;
	case IDL_SPLINE:
		DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
		break;
	case IDL_ALGORITM:
		break;
	case IDL_ORIENTED:
		DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptar,ptar,prosloika->m_synhro);
		break;
	case IDL_NAS_LINIA:
		DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
		break;
	case IDL_NAS_SHTRIH:
		DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptar,prosloika->m_synhro);
		break;
	case IDL_NAS_PUNKTIR:
		DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptar,prosloika->m_synhro);
		break;
	case IDL_NAS_SBSTRETCH:
		DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptar,prosloika->m_synhro);
		break;
	case IDL_NAS_SPLINE:
		DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
		break;
	case IDL_SINGULAR:
		DrawSingular(pObj, pDevice, (MSbSingular*)prosloika, ptarr, ptar, prosloika->m_synhro);
		break;
	case IDL_NAS_SINGULAR:
		DrawNastrSingular(pObj, pDevice, (MNastrSbSingular*)prosloika, ptarr, ptar, prosloika->m_synhro);
		break;
	}
	delete ptar;
	delete ptarr;
}


//=======================================================================
//! Drawing of a layer for band objects
void MDrawing::DrawProsloikaPls(MTopobject* pObj,MPlotDevice* pDevice,MProsloika* prosloika,MPtArray* pta,MRzArray* razryvy)
//=======================================================================
{
    //if points less than two, we don't draw
	if(pta->GetSize()<2)return;
	
    //receive coefficient of change of shift or a space for band objects
	double dWidth = pObj->GetDoubleDataPoleByName(ID_PLS_ATTRIB_DIAMETR);
	if(!dWidth)
		dWidth = pObj->GetDoubleDataPoleByName(ID_PLS_ATTRIB_WIDTH);
	dWidth = (dWidth>0)?dWidth:1;
	double dThick = pObj->GetDoubleDataPoleByName(ID_PLS_ATTRIB_THICKNESS);
	dThick = (dThick>0)?dThick:1;
    //Now concrete processing for a layer
	double dMul;
	if(prosloika->m_distofaxe!=0){
		dMul = dThick;
		prosloika->m_distofaxe*=(float)dWidth;
	}
	else{
		dMul = dWidth;
	}
	switch(prosloika->GetType())
	{
	case IDL_LINIA:
		((MLinia*)prosloika)->m_thickness*=(float)dMul;
		break;
	case IDL_SHTRIH:
		((MShtrih*)prosloika)->m_thickness*=(float)dMul;
		break;
	case IDL_PUNKTIR:
		((MPunktir*)prosloika)->m_diametr*=(float)dMul;
		break;
	case IDL_SPLINE:
		((MSpline*)prosloika)->m_thickness*=(float)dMul;
		break;
	case IDL_SBSTRETCH:
		((MSbStretch*)prosloika)->m_symbol.value*=(float)dMul;				
		break;
	case IDL_SOFRANGE:
		((MRangeSym*)prosloika)->m_symbol.value*=(float)dMul;				
		break;
	case IDL_ORIENTED:
		((MOrientedStretch*)prosloika)->m_symbol.value*=(float)dMul;				
		break;
	case IDL_SINGULAR:
		((MSbSingular*)prosloika)->m_symbol.value *= (float)dMul;
		break;
	case IDL_NAS_SINGULAR:
		((MNastrSbSingular*)prosloika)->m_symbol.value *= (float)dMul;
		break;
	case IDL_ALGORITM:
	default:
		break;
	}
	
    //create two arrays:
    //ptar - axial
    //ptarr - covering

	//******************************************************
	MPtArray* ptar = (MPtArray*)pta->Clone();
	double dRealOffsetFromAxe = (*ConvertSizeToReal)(pObj,prosloika->m_distofaxe);
	MPtArray* ptarr;
    //draw a layer depending on type
    //we give the axial array to synchronize layers
	switch(prosloika->GetType())
	{
	case IDL_LINIA:
	case IDL_SHTRIH:
	case IDL_PUNKTIR:
	case IDL_SBSTRETCH:
	case IDL_SPLINE:
	case IDL_ORIENTED:
	case IDL_NAS_LINIA:
	case IDL_NAS_SHTRIH:
	case IDL_NAS_PUNKTIR:
	case IDL_NAS_SBSTRETCH:
	case IDL_NAS_SPLINE:

		{
            int n = 0;//number of breaks
			if(razryvy!=NULL)
				n = razryvy->GetSize();
            //if the contour has breaks
			if(n>0){
                //form the array for drawing by the line - the line is drawn to breaks
                //on a place of a break it is passed, and then renews
				MPtArray* ptarray = new MPtArray();
                //rzind - index of an edge of a contour of top object on which it breaks
				int rzind;
				MRazryv* rz = razryvy->GetAt(0);

                //ind - index of an edge of a contour of top object from which we continue to draw the line
                //we start drawing always after the first break, as the last breaks
                //can pass for a zero index for the closed top objects and contours
                //vulgar objects
				int ind = rz->m_indlast;
                //we will store the first index of the previous space in the variable 'lastind'
//				int lastind = rz->m_indfirst;
                //drawing of the line between breaks always begins with a final point
                //the previous break
				ptarray->AddTail(&rz->m_ptlast);
                //cycle on all breaks...
				for(int j=1; j<n; j++){
					MRazryv* rz = razryvy->GetAt(j);
					rzind = rz->m_indfirst;
                    //form the array of points for drawing by the line to a break;
                    //the last in the array we include the first point of a break;
                    //if beginning of the following and end previous breaks
                    //lie on one edge, for drawing to the array of points to the last
                    //the point of the previous break should add only the first point
                    //following break
					if(rzind!=ind-1){
						for(int i=ind; i<ptar->GetSize(); i++){
							if(i!=rzind){
								ptarray->AddTail(ptar->GetAt(i));
							}
							else{
								ptarray->AddTail(ptar->GetAt(i));
								break;
							}
						}
					}
					ptarray->AddTail(&rz->m_ptfirst);
					ptarr = BuildObvod(ptarray,dRealOffsetFromAxe);
                    //draw the line to a break
					switch(prosloika->GetType())
					{
					case IDL_LINIA:
						DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
						break;
					case IDL_SHTRIH:
						DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_PUNKTIR:
						DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SBSTRETCH:
						DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SPLINE:
						DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
						break;
					case IDL_ORIENTED:
						DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptarray,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_LINIA:
						DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
						break;
					case IDL_NAS_SHTRIH:
						DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_PUNKTIR:
						DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SBSTRETCH:
						DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SPLINE:
						DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
						break;
					}
					delete ptarr;

                    //clean the array of points for formation of a new segment of the line
					ptarray->ClearAll();
                    //in a new segment the first point - the point noting the end of a break
					ptarray->AddTail(&rz->m_ptlast);
                    //the second point in a new segment - the first point of a contour after a point,
                    //noting the end of a space; rz->m_indlast - an index of this point
					ind = rz->m_indlast;
                    //remember the first index of the current break for comparison
                    //with the first index of the following
//					lastind = rz->m_indfirst;
				}
                //DRAWING OF THE LINE AFTER THE FINAL BREAK TO THE INITIAL BREAK
				rz = razryvy->GetAt(0);
                //compare the second index of the last break to the first index
                //initial break:
                //if the second index of the last break is less than first index
                //an initial break, in the last breaks there was a transition through
                //a zero index, it is also necessary to draw initial part of a contour
                //top object from the second point of the last break to the first point
                //initial break
				if(ind<rz->m_indfirst+1){
                    //form and draw the last segment of a contour of top object;
                    //if beginning zero and end of the last of breaks
                    //lie on one edge, for drawing to the array of points to the last
                                   //the point of the last break should add only the first point
                    //zero break
					for(int i=ind; i<=rz->m_indfirst; i++){
						ptarray->AddTail(ptar->GetAt(i));
					}
					ptarray->AddTail(&rz->m_ptfirst);
					ptarr = BuildObvod(ptarray,dRealOffsetFromAxe);
					switch(prosloika->GetType())
					{
					case IDL_LINIA:
						DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
						break;
					case IDL_SHTRIH:
						DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_PUNKTIR:
						DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SBSTRETCH:
						DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SPLINE:
						DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
						break;
					case IDL_ORIENTED:
						DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptarray,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_LINIA:
						DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
						break;
					case IDL_NAS_SHTRIH:
						DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_PUNKTIR:
						DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SBSTRETCH:
						DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SPLINE:
						DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
						break;
					}
					delete ptarr;
				}
                //if initial and final breaks lie on one edge or only
                //the break locates on one edge
				else if((ind==rz->m_indfirst+1)&&(*ptar->GetAt(0)==*ptar->GetAt(ptar->GetSize()-1))){
					if(DLINASG(*ptar->GetAt(rz->m_indfirst),rz->m_ptfirst)<=
						DLINASG(*ptar->GetAt(rz->m_indfirst),*ptarray->GetAt(0))){
						int i = 0;
						for(i=ind; i<ptar->GetSize(); i++)
							ptarray->AddTail(ptar->GetAt(i));
						for(i=0; i<=rz->m_indfirst; i++)
							ptarray->AddTail(ptar->GetAt(i));
					}
					ptarray->AddTail(&rz->m_ptfirst);
					ptarr = BuildObvod(ptarray,dRealOffsetFromAxe);
					switch(prosloika->GetType())
					{
					case IDL_LINIA:
						DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
						break;
					case IDL_SHTRIH:
						DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);

						break;
					case IDL_PUNKTIR:
						DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SBSTRETCH:
						DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SPLINE:
						DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
						break;
					case IDL_ORIENTED:
						DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptarray,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_LINIA:
						DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
						break;
					case IDL_NAS_SHTRIH:
						DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_PUNKTIR:
						DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SBSTRETCH:
						DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SPLINE:
						DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
						break;
					}
					delete ptarr;
				}
                //if the final point of the last break lies on an edge with an index,
                //bigger than index of an edge of the initial point of the first break, transition through
                //a zero index wasn't, and it is necessary to draw a contour tail
                //from the second point of the last break and initial part of a contour
                //top object to the first point of an initial break
                //for not closed contour drawing is right and in a case, when the initial
                //and final breaks lie on one edge or only
                //the break locates on one edge
				else if((ind>rz->m_indfirst+1)||
					((ind==rz->m_indfirst+1)&&(*ptar->GetAt(0)!=*ptar->GetAt(ptar->GetSize()-1)))){
                    //form and draw the last segment of a contour of top object
					int i = 0;
					for(i=ind; i<ptar->GetSize(); i++){
						ptarray->AddTail(ptar->GetAt(i));
					}
					ptarr = BuildObvod(ptarray,dRealOffsetFromAxe);
					switch(prosloika->GetType())
					{
					case IDL_LINIA:
						DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
						break;
					case IDL_SHTRIH:
						DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_PUNKTIR:
						DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SBSTRETCH:
						DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SPLINE:
						DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
						break;
					case IDL_ORIENTED:
						DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptarray,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_LINIA:
						DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
						break;
					case IDL_NAS_SHTRIH:
						DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_PUNKTIR:
						DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SBSTRETCH:
						DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SPLINE:
						DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
						break;
					}
					delete ptarr;
                    //clean the array of points for formation of a new segment of the line
					ptarray->ClearAll();
                    //form and draw an initial segment of a contour of top object
					for(i=0; i<=rz->m_indfirst; i++){
						ptarray->AddTail(ptar->GetAt(i));
					}
					ptarray->AddTail(&rz->m_ptfirst);
					ptarr = BuildObvod(ptarray,dRealOffsetFromAxe);
					switch(prosloika->GetType())
					{
					case IDL_LINIA:
						DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
						break;
					case IDL_SHTRIH:
						DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;

					case IDL_PUNKTIR:
						DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SBSTRETCH:
						DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_SPLINE:
						DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
						break;
					case IDL_ORIENTED:
						DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptarray,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_LINIA:
						DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
						break;
					case IDL_NAS_SHTRIH:
						DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_PUNKTIR:
						DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SBSTRETCH:
						DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptarray,prosloika->m_synhro);
						break;
					case IDL_NAS_SPLINE:
						DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
						break;
					}
					delete ptarr;
				}
				delete ptarray;
			}
            //if the contour of top object has no spaces
			else{
				ptarr = BuildObvod(ptar,dRealOffsetFromAxe);
                //draw a layer depending on type
                //we give the axial array to synchronize layers
				switch(prosloika->GetType())

				{
				case IDL_LINIA:
					DrawLinia(pObj,pDevice,(MLinia*)prosloika,ptarr);
					break;
				case IDL_SHTRIH:
					DrawShtrih(pObj,pDevice,(MShtrih*)prosloika,ptarr,ptar,prosloika->m_synhro);
					break;
				case IDL_PUNKTIR:
					DrawPunktir(pObj,pDevice,(MPunktir*)prosloika,ptarr,ptar,prosloika->m_synhro);
					break;
				case IDL_SBSTRETCH:
					DrawSbStretch(pObj,pDevice,(MSbStretch*)prosloika,ptarr,ptar,prosloika->m_synhro);
					break;
				case IDL_SPLINE:
					DrawSpline(pObj,pDevice,(MSpline*)prosloika,ptarr);
					break;
				case IDL_ORIENTED:
					DrawOriented(pObj,pDevice,(MOrientedStretch*)prosloika,ptar,ptar,prosloika->m_synhro);
					break;
				case IDL_NAS_LINIA:
					DrawNastrLinia(pObj,pDevice,(MNastrLinia*)prosloika,ptarr);
					break;
				case IDL_NAS_SHTRIH:
					DrawNastrShtrih(pObj,pDevice,(MNastrShtrih*)prosloika,ptarr,ptar,prosloika->m_synhro);
					break;
				case IDL_NAS_PUNKTIR:
					DrawNastrPunktir(pObj,pDevice,(MNastrPunktir*)prosloika,ptarr,ptar,prosloika->m_synhro);
					break;
				case IDL_NAS_SBSTRETCH:
					DrawNastrSbStretch(pObj,pDevice,(MNastrSbStretch*)prosloika,ptarr,ptar,prosloika->m_synhro);
					break;
				case IDL_NAS_SPLINE:
					DrawNastrSpline(pObj,pDevice,(MNastrSpline*)prosloika,ptarr);
					break;
				}
				delete ptarr;
			}
		}
		break;
	case IDL_SOFRANGE:
		{
			ptarr = BuildObvod(ptar,dRealOffsetFromAxe);
			DrawRangeSym(pObj,pDevice,(MRangeSym*)prosloika,ptarr);
			delete ptarr;
		}
		break;
	case IDL_SINGULAR:
		{
			ptarr = BuildObvod(ptar, dRealOffsetFromAxe);
			DrawSingular(pObj, pDevice, (MSbSingular*)prosloika, ptarr, ptar, prosloika->m_synhro);
		}
		break;
	case IDL_NAS_SINGULAR:
		{
			ptarr = BuildObvod(ptar, dRealOffsetFromAxe);
			DrawNastrSingular(pObj, pDevice, (MNastrSbSingular*)prosloika, ptarr, ptar, prosloika->m_synhro);
		}
		break;
	case IDL_ALGORITM:
		break;


	}
	
	delete ptar;
	
    //receive coefficient of change of shift or a space for band objects
	dWidth = pObj->GetDoubleDataPoleByName(ID_PLS_ATTRIB_DIAMETR);
	if(!dWidth)
		dWidth = pObj->GetDoubleDataPoleByName(ID_PLS_ATTRIB_WIDTH);
	dWidth = (dWidth>0)?dWidth:1;
	dThick = pObj->GetDoubleDataPoleByName(ID_PLS_ATTRIB_THICKNESS);
	dThick = (dThick>0)?dThick:1;
    //Now concrete processing for a layer
	if(prosloika->m_distofaxe!=0){
		dMul = dThick;
		prosloika->m_distofaxe/=(float)dWidth;
	}
	else{
		dMul = dWidth;
	}
	switch(prosloika->GetType())
	{
	case IDL_LINIA:
		((MLinia*)prosloika)->m_thickness/=(float)dMul;
		break;
	case IDL_SHTRIH:
		((MShtrih*)prosloika)->m_thickness/=(float)dMul;
		break;
	case IDL_PUNKTIR:
		((MPunktir*)prosloika)->m_diametr/=(float)dMul;
		break;
	case IDL_SPLINE:
		((MSpline*)prosloika)->m_thickness/=(float)dMul;
		break;
	case IDL_SBSTRETCH:
		((MSbStretch*)prosloika)->m_symbol.value/=(float)dMul;				
		break;
	case IDL_SOFRANGE:
		((MRangeSym*)prosloika)->m_symbol.value/=(float)dMul;				
		break;
	case IDL_ORIENTED:
		((MOrientedStretch*)prosloika)->m_symbol.value/=(float)dMul;				
		break;
	case IDL_SINGULAR:
		((MSbSingular*)prosloika)->m_symbol.value /= (float)dMul;
		break;
	case IDL_NAS_SINGULAR:
		((MNastrSbSingular*)prosloika)->m_symbol.value /= (float)dMul;
		break;
		
	case IDL_ALGORITM:
	default:
		break;
	}
	
}

//=======================================================================
//! Drawing of the continuous line
void MDrawing::DrawLinia(MTopobject* pObj,MPlotDevice* pDevice,MLinia* linia,MPtArray* ptarr)
//=======================================================================
{
    //prepare a pen
	MColor color;
	pObj->GetKarta()->GetDictColorValue(linia->m_colorkod, color);

	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
//if((pObj->bDrawColor)&&(color != pObj->GetKarta()->m_papercolorkod)&&(color != whiteclr)) 	

	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;

	if((pObj->m_selected)&&(color != whiteclr)) 
    {//If the object is chosen that we change color
		color = pObj->m_sel_color;
	}


	pDevice->SavePencil();
    //Width in real coordinates
	double dWidth = (*ConvertSizeToReal)(pObj,linia->m_thickness);
    double exVal = (pObj->m_excited)?1.5:1;
	if(pDevice->Distinguish(dWidth)) {
        //on a tabular code we will receive color from the dictionary of colours if it is possible,
        //otherwise we will take color by default
		pDevice->SetPencil(PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_ROUND,
            (*ConvertSizeToReal)(pObj,exVal*linia->m_thickness),&color);
	}
	else {
		M_LOG_PEN drawpen;
        //on a tabular code we will receive color from the dictionary of colours if it is possible,
        //otherwise we will take color by default
		drawpen.lopnColor=color;
		drawpen.lopnStyle=PS_SOLID;
        drawpen.lopnWidth.x = (*ConvertSizeToReal)(pObj,exVal*linia->m_thickness);
		pDevice->SetPencil(&drawpen);
	}
    //draw
	pDevice->Polyline(ptarr->GetHead(),ptarr->GetSize());
    //restore
	pDevice->RestorePencil();
}


//=======================================================================
//! Drawing of the shaped line
void MDrawing::DrawShtrih(MTopobject* pObj,MPlotDevice* pDevice,
				MShtrih* shtrih,MPtArray* ptarr, MPtArray* axearr,bool synhro)
//=======================================================================
{
	
	//
    //    IT DRAWS THE SEGMENT AS:
	//
	//
	//         ---------------------
	//        | =================== |
	//         ---------------------
	//
    //  There == - real segment
    //        -- - that we see on the screen
	//
//*******************************************
    //prepare basic data
	double d,s,p;
	d = (*ConvertSizeToReal)(pObj,shtrih->m_dlina);
	s = (*ConvertSizeToReal)(pObj,shtrih->m_period);
	p = s - d;

    //If the line is indiscernible, it passes into a straight line
	if(!pDevice->Distinguish((d+p)/VISIBLE_SHTRIH_STEP_PLUS_LENGTH)){
		M_LOG_PEN lpen;
		MColor color;
		pObj->GetKarta()->GetDictColorValue(shtrih->m_colorkod, color);
		MColor whiteclr;
		pObj->GetKarta()->GetDictColorValue(1, whiteclr);
		if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
		if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
		lpen.lopnStyle = PS_SOLID;
		lpen.lopnColor = color;
		lpen.lopnWidth = MPoint((double)0,(double)0);
		pDevice->SetPencil(&lpen);
		pDevice->Polyline(ptarr->GetHead(),ptarr->GetSize());
		pDevice->RestorePencil();
		return;
	}

    //temporarily we keep continuous settings for drawing of the first space
	double d1=d;
	double p1=p;
	double s1=s;
    //establish continuous settings for drawing of the first space
	s=p=(*ConvertSizeToReal)(pObj,shtrih->m_otstup);//GEOM(shtrih->m_otstup);
	d=0;
	bool first=true;

	

    //prepare a pen
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(shtrih->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
	
	pDevice->SavePencil();

 	if(shtrih->m_kvadrat)
		pDevice->SetPencil(PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_ROUND,
			(*ConvertSizeToReal)(pObj,shtrih->m_thickness),&color);
	else
		pDevice->SetPencil(PS_GEOMETRIC|PS_ENDCAP_ROUND|PS_JOIN_ROUND,
			(*ConvertSizeToReal)(pObj,shtrih->m_thickness),&color);
	
//	MIntPoint pt;
	MPoint pt4,pt5,pt6;

    double t=0; //the current length from the beginning of s;
	
    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
    MPtArray* BkArr=ptarr;//cover


    MBoArray* What=new MBoArray();//array of action
	What->SetSize(100);
    MPtArray* Where=new MPtArray();//array of movements
	Where->SetSize(100);
    int WW=0;//counter
    bool flag;//service
	

//****************************************************
    //form arrays of action and movements

    //start drawing with a zero point
	flag=false;
	pt4=BkArr->GetAt(0)[0];
	pt5=BkArr->GetAt(1)[0];
	pt6=OsArr->GetAt(0)[0];
	What->SetAt(WW,&flag);
  MPoint point= GetPointOnOtrezok(pt4,pt5,pt6);
	Where->SetAt(WW++,&point);

	int m_nPoints=OsArr->GetSize();
	
	int i;
	double dlina;
	double ostatok,ostatok1,k;
	MPoint pt1,pt2; //,pt3;
    //on all segments of top object
	for(i=0;i<m_nPoints-1;i++)
	{
		pt1=OsArr->GetAt(i)[0];
		pt2=OsArr->GetAt(i+1)[0];
		pt4=BkArr->GetAt(i)[0];
		pt5=BkArr->GetAt(i+1)[0];

		
		dlina=sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x)+(pt2.y-pt1.y)*(pt2.y-pt1.y));

        //so far length not zero
		while(dlina>0)
		{
			ostatok=s-t;
            //if the rest is more than length of a break,
            //that we draw at first a stroke
			if(ostatok>p)
			{
                //the remained stroke
				ostatok1=ostatok-p;
                //if it more than length of a segment, we also draw it up to the end
				if(ostatok1>dlina)
				{
                    //draw
					if(WW==What->GetSize())
					{
						What->SetSize(WW+100);
						Where->SetSize(WW+100);
					}
					flag=true;
					What->SetAt(WW,&flag);
					Where->SetAt(WW++,&pt5);

					t+=dlina;
					dlina=0.0;
				}
                //if it less than length of a segment,
                //that we draw it all
				else
				{
					k=ostatok1/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok1;
					t=d;
                    //draw
					if(WW==What->GetSize())
					{
						What->SetSize(WW+100);
						Where->SetSize(WW+100);
					}
					flag=true;
					What->SetAt(WW,&flag);
            MPoint point= GetPointOnOtrezok(pt4,pt5,pt1);
					Where->SetAt(WW++,&point);
				}
			}
            //if we didn't finish a space
			else
			{
                //if the space is more than length,
				if(ostatok>dlina)
				{
					t+=dlina;
					dlina=0.0;
					
				}
                //otherwise we put an end
				else
				{
					k=ostatok/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok;
					t=0;
                    //if it was the first space, restore characteristics
					if(first)
					{
						s=s1;
						d=d1;
						p=p1;
						first=false;
					}
					if(!((i==m_nPoints-2)&&(dlina==ostatok)))
					{
                        //move
						if(WW==What->GetSize())
						{
							What->SetSize(WW+100);
							Where->SetSize(WW+100);
						}
						flag=false;
						What->SetAt(WW,&flag);
            MPoint point= GetPointOnOtrezok(pt4,pt5,pt1);
						Where->SetAt(WW++,&point);
					}
				}
			}
		}
	}



//*****************************************************
//draw a stroke on codes of arrays of action and movements

    //now we will draw everything
//	int countpoints=100;
//    int cnt;//number of points in a polyline
	int ind=0;
	int bb,ee;
	bool bo,bo1;
    //we draw with polyline which we now will construct
	while(ind<WW)
	{
        //indexes of the beginning and end of a polyline are negative since we don't know them yet
		bb=-1;
		ee=-1;
        //receive a drawing flag (true - to draw, false - to move )
		bo=What->GetAt(ind)[0];
        //if to move
		if(!bo)
		{
            //if the end of the array, that will be nothing
			if(ind==WW-1) ind++;
            //else
			else
			{
                //flag of drawing of the following element
				bo1=What->GetAt(ind+1)[0];
                //if it - to draw, we received the beginning of a polyline
				if(bo1)
				{
					bb=ind;
					ee=ind;
					ind++;
                    //look for the end of a segment of a polyline
					while((ind<WW)&&(What->GetAt(ind)[0]))
					{
						ee=ind;
						ind++;
					}
                    //now, if we found a polyline, we will draw it
					if((bb!=-1)&&(ee!=-1))
					{
//						cnt=ee-bb+1;
						pDevice->Polyline(Where->GetAt(bb),(ee-bb+1));
						bb=ee=-1;
					}
				}
                //if to move,  we do anything
				else ind++;
			}
		}
        //otherwise we have some failure and we ignore it
		else ind++;
	}

	//****************************************************
    //restore
	pDevice->RestorePencil();
	delete What;
	delete Where;
}

//=======================================================================
//! Drawing of a dotted line
void MDrawing::DrawPunktir(MTopobject* pObj,MPlotDevice* pDevice,
		MPunktir* punktir,MPtArray* ptarr, MPtArray* axearr,bool synhro)

//=======================================================================
{
	double p1 = (*ConvertSizeToReal)(pObj,punktir->m_period);
    //If the dotted line isn't indiscernible it passes into the simple line
	if(!pDevice->Distinguish(p1/VISIBLE_PUNKTIR_STEP))
	{
		M_LOG_PEN lpen;
		MColor color;
		pObj->GetKarta()->GetDictColorValue(punktir->m_colorkod, color);
		MColor whiteclr;
		pObj->GetKarta()->GetDictColorValue(1, whiteclr);
		if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
		if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{	color = pObj->m_sel_color;
		}

		lpen.lopnStyle = PS_SOLID;
		lpen.lopnColor = color;
		lpen.lopnWidth = MPoint((double)0,(double)0);
		pDevice->SetPencil(&lpen);
		pDevice->Polyline(ptarr->GetHead(),ptarr->GetSize());
		pDevice->RestorePencil();
		return;
	}

	double p = (*ConvertSizeToReal)(pObj,punktir->m_nachalo);
	bool first=true;
	
	
    //dotted line radius in logical coordinates
	double d=(double)(*ConvertSizeToReal)(pObj,punktir->m_diametr)/2;

    //prepare a transparent pen
	M_LOG_PEN drawpen;
    memset(&drawpen,0x0,sizeof(drawpen));
	drawpen.lopnStyle=PS_NULL;
	pDevice->SavePencil();
	pDevice->SetPencil(&drawpen);
	
    //prepare the necessary brush
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(punktir->m_colorkod, color);

	pDevice->SaveZapolnenie();
	pDevice->SetZapolnenie(&color);


    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
    MPtArray* BkArr=ptarr;//cover

	MPoint pt4,pt5;  //,pt6;

    double t=0; //the current length from the beginning of d;

	
	int m_nPoints=OsArr->GetSize();
	
	int i;
	double dlina;
	double ostatok,k;
	MPoint pt1,pt2;
    //on all segments of top object
	for(i=0;i<m_nPoints-1;i++)
	{
		pt1=OsArr->GetAt(i)[0];
		pt2=OsArr->GetAt(i+1)[0];
		pt4=BkArr->GetAt(i)[0];
		pt5=BkArr->GetAt(i+1)[0];
		
		dlina=sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x)+(pt2.y-pt1.y)*(pt2.y-pt1.y));

		while(dlina>0)
		{
			ostatok=p-t;
            //if the rest is more than length
			if(ostatok>dlina)
			{
				t+=dlina;
				dlina=0.0;
			}
            //otherwise we put a circle
			else
			{
					k=ostatok/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok;
					t=0;
					MPoint pt = GetPointOnOtrezok(pt4,pt5,pt1);
					pDevice->Ellipse(pt.x-d,pt.y-d,pt.x+d,pt.y+d);
					if(first)
					{
						p=p1;
						first=false;
					}
			}
			
		}
	}

    //restore
	pDevice->RestorePencil();
	pDevice->RestoreZapolnenie();
}

//=======================================================================

//! Drawing of a dotted line
void MDrawing::DrawSbStretch(MTopobject* pObj,MPlotDevice* pDevice,MSbStretch* SbStretch,
		MPtArray* ptarr,MPtArray* axearr,bool synhro)
//=======================================================================
{
	if(!SbStretch->m_symbol.visible) return;
    //create a context necessary to us

	M_LOG_FONT lfont = SbStretch->m_symbol.logfont;

	pDevice->SetBkMode(SbStretch->m_symbol.bkmode);
	//pDevice->SetTextAlign(UT_TA_BASELINE|UT_TA_CENTER);
	pDevice->SetTextAlign(SbStretch->m_symbol.align);//TA_BASELINE);//TA_CENTER);//SbStretch->symbol->align);//TA_CENTER||TA_BASELINE);
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)SbStretch->m_symbol.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{	
		color = pObj->m_sel_color;
		}

	pDevice->SetTextColor(color);
	if(SbStretch->m_symbol.scaled)
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,SbStretch->m_symbol.value);
	else
		lfont.lfHeight = pDevice->LogicToBum(SbStretch->m_symbol.value);

    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FONT_SB_STRETCH))
		return;

	pDevice->SaveFont();
	pDevice->SetFont(&lfont);
	

	double p1 = (*ConvertSizeToReal)(pObj,SbStretch->m_period);

    //keep temporarily constant parameter
	double p=(*ConvertSizeToReal)(pObj,SbStretch->m_sbpause);//GEOM(SbStretch->m_sbpause);
	bool first=true;
	
    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
    MPtArray* BkArr=ptarr;//cover

	MPoint pt4,pt5; //,pt6;

    double t=0; //the current length from the beginning of d;
    //we start drawing with a zero point
	int m_nPoints=ptarr->GetSize();
	MPoint pt1,pt2;
	
	
	int i;
	double dlina;
	double ostatok,k;
	
    //on all segments of top object
	for(i=0;i<m_nPoints-1;i++)
	{
		pt1=OsArr->GetAt(i)[0];
		pt2=OsArr->GetAt(i+1)[0];
		pt4=BkArr->GetAt(i)[0];
		pt5=BkArr->GetAt(i+1)[0];
		lfont.lfEscapement=(int)(-GetVektorGrad(&pt1,&pt2)*10); 
		dlina=sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x)+(pt2.y-pt1.y)*(pt2.y-pt1.y));

		while(dlina>0)
		{
			ostatok=p-t;
            //if the rest is more than length
			if(ostatok>dlina)
			{
				t+=dlina;
				dlina=0.0;
			}
            //otherwise we put a circle
			else
			{
					k=ostatok/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok;
					t=0;
					MPoint pt = GetPointOnOtrezok(pt4,pt5,pt1);
					pDevice->SetFont(&lfont);
					pDevice->TextOut(pt.x,pt.y,SbStretch->m_symbol.str);
					if(first)
					{
						p=p1;
						first=false;
					}
			}
			
		}
	}
    //restore
	pDevice->RestoreFont();
}

//=======================================================================
//! Drawing of boundary symbols
void MDrawing::DrawRangeSym(MTopobject* pObj,MPlotDevice* pDevice,MRangeSym* RangeSym,MPtArray* ptarr)
//=======================================================================
{
	int m_nPoints=ptarr->GetSize();
    //if the array of points empty, we draw nothing
	if(m_nPoints==0) return;

	M_LOG_FONT lfont = RangeSym->m_symbol.logfont;

	pDevice->SetBkMode(RangeSym->m_symbol.bkmode);
	pDevice->SetTextAlign(RangeSym->m_symbol.align); //vipa COMMENTed this when Linux
	//pDevice->SetTextAlign(UT_TA_BASELINE|UT_TA_CENTER);//vipa UN_COMMENTed this when Linux
	//SbStretch->symbol->align);//TA_BASELINE);//TA_CENTER);//SbStretch->symbol->align);//TA_CENTER||TA_BASELINE);
    //receive color of the text on codes in the dictionary of colours

	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)RangeSym->m_symbol.colorkod, color))
		color.SetColor(0,0,0);	
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}

	pDevice->SetTextColor(color);

	if(RangeSym->m_symbol.scaled)
		lfont.lfHeight = (*ConvertSizeToReal)(pObj,RangeSym->m_symbol.value);
	else
		lfont.lfHeight =  pDevice->LogicToBum(RangeSym->m_symbol.value);
	
	//if (pDevice)
    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FONT_BOUND_SYM))
		return;
	
	pDevice->SaveFont();

	MPoint *pt1,*pt2;
//	MIntPoint pt;
    //draw an initial boundary symbol on a place of a zero point
	if((RangeSym->m_type==IDR_BEGIN)||(RangeSym->m_type==IDR_BOTH))
	{
        //translate a zero point
		
        //If the top object contains at least two points,
        //that we draw a symbol under the corresponding angle
		if(m_nPoints>1)
		{
			pt1=ptarr->GetAt(0);
			pt2=ptarr->GetAt(1);
			lfont.lfEscapement=(int)(-GetVektorGrad(pt1,pt2)*10); 
		}
        //otherwise we accept a angle equal to zero
		else
			lfont.lfEscapement=0;

        //prepare the drawing mode
		pDevice->SetFont(&lfont);
		pDevice->TextOut(ptarr->GetHead()->x,ptarr->GetHead()->y,RangeSym->m_symbol.str);
	}

    //draw a final boundary symbol on a place of the last point
	if((RangeSym->m_type==IDR_END)||(RangeSym->m_type==IDR_BOTH))
	{
        //translate the last point
        //If the top object contains at least two points,
  //that we draw a symbol under the corresponding angle
		if(m_nPoints>1)
		{
			pt1=ptarr->GetAt(m_nPoints-2);
			pt2=ptarr->GetAt(m_nPoints-1);
			lfont.lfEscapement=(int)(-GetVektorGrad(pt1,pt2)*10); 
		}
        //otherwise we accept a angle equal to zero
        else
			lfont.lfEscapement=0;

        //prepare the drawing mode
        pDevice->SetFont(&lfont);
		pDevice->TextOut(ptarr->GetTail()->x,ptarr->GetTail()->y,RangeSym->m_symbol.str);
	}

    //restore font
	pDevice->RestoreFont();
}


//=======================================================================
//! Drawing of a spline
void MDrawing::DrawSpline(MTopobject* pObj,MPlotDevice* pDevice,MSpline* spline,MPtArray* ptarr)
//=======================================================================
{
    //prepare a pen
	M_LOG_PEN drawpen;
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(spline->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}

	drawpen.lopnColor=color;
	drawpen.lopnStyle=PS_SOLID;
	drawpen.lopnWidth.x=drawpen.lopnWidth.y=(*ConvertSizeToReal)(pObj,spline->m_thickness);
	
	pDevice->SavePencil();
	pDevice->SetPencil(&drawpen);

	int m_nPoint=ptarr->GetSize();
	int type = spline->m_type;
	if(spline->m_kolpts == 0)
		type = SPT_LOMANAYA;

    //will draw horizontals one of the following types:
	switch(type)
	{
	case SPT_LOMANAYA:
        //draw
		pDevice->Polyline(ptarr->GetHead(),m_nPoint);
		break;
	case SPT_INTERP:
		{
        MPtArray* newarr = nullptr;
//        MPtArray* newarr = new MPtArray();
            // on an entrance of function of creation of an interpolation spline 'LOMTOISPL' we give the array of points 'ptarr' - an initial broken line,
            //at the exit we have the new array of points 'newarr' - a final broken line on which we carry out a spline
			newarr = LOMTOISPL(ptarr, spline->m_vid, spline->m_kolpts);
			m_nPoint = newarr->GetSize();
            //drawing
			pDevice->Polyline(newarr->GetHead(),m_nPoint);
			delete newarr;
		}
		break;
	case SPT_APPROX:
		{
			double* param;
//			int nn = m_nPoint - 1;
			
			if(spline->m_vid == SPV_ZAMKNUT)
			{
                //temporarily shag = 0.5
				double shag = 0.5;
                // boundary conditions of the third sort
				param = (double*)malloc(sizeof(double)*(m_nPoint-1));
				for(int i=0; i<m_nPoint-1; i++)
					param[i] = shag/4;
			}
            // case when we build not closed spline...
			else{
                //temporarily shag = 0.5
				double shag = 0.5;
				param = (double*)malloc(sizeof(double)*m_nPoint);
				param[0] = 0;
				param[m_nPoint-1] = 0;
				for(int i=1; i<m_nPoint - 1; i++)
					param[i] = shag/4;
			}
            MPtArray* newarr = nullptr;
//            MPtArray* newarr = new MPtArray();
            // on an entrance of function of creation of an approximating spline 'LOMTOASPL' we give the array of points 'ptarr' - an initial broken line,
            //at the exit we have the new array of points 'newarr' - a final broken line on which we carry out a spline
			newarr = LOMTOASPL(ptarr, spline->m_vid, param, spline->m_kolpts);
//			int number = 0;
//			GetArrayOfPoint(number)->ClearAll();
//			((MPtArray*)this->m_Komplekt->GetAt(F_TOCHKI))->AppendTail(ptarr);
			m_nPoint = newarr->GetSize();
            // broken line
			pDevice->Polyline(newarr->GetHead(),m_nPoint);
			delete newarr;
			free(param);
		}
			break;
	}

    //restore
	pDevice->RestorePencil();
}

// ==================================================================
//!	Drawing of the focused symbol broach
//parameters (MTopobject* pObj,MPlotDevice *pDevice, MOrientedStretch* m_linia,
//			MPtArray* ptarr,MPtArray* axearr,bool synhro)
void MDrawing::DrawOriented(MTopobject* pObj,MPlotDevice *pDevice,
		MOrientedStretch* m_linia,MPtArray* ptarr,MPtArray* ,bool )
// ==================================================================
{

	if(ptarr->GetSize()<=1)
		return;
	if(!m_linia->m_symbol.visible) return;
    //Drawing of the focused symbol broach
	M_LOG_FONT lfont = m_linia->m_symbol.logfont;

	pDevice->SetBkMode(m_linia->m_symbol.bkmode);
	pDevice->SetTextAlign(m_linia->m_symbol.align);

    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)m_linia->m_symbol.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}
	pDevice->SetTextColor(color);

	if(m_linia->m_symbol.scaled)
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,m_linia->m_symbol.value);
	else
		lfont.lfHeight = pDevice->LogicToBum(m_linia->m_symbol.value);

    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FONT_SB_STRETCH)) 
		return;

	pDevice->SaveFont();
	
	int m_nPoints=ptarr->GetSize();
//	MPoint pt1,pt2,p3;

    //Not to count the focused area
    //we use change of an angle and if summation of a angle
    //there is abroad [0,3600] that there is a reorientation

    //ALSO DRAWING IS BASED THAT THE RETURNED ANGLE IS ALWAYS POSITIVE
    //on all segments of top object
	for(int i=0;i<m_nPoints;i++)
	{
		switch(m_linia->m_type){
		case MOS_NOORIENT:
			{
				lfont.lfEscapement = 0;
			}break;
		case MOS_NEXTORIENT:
			if(i!=m_nPoints-1){
				lfont.lfEscapement = (int)(GetVektorGrad(ptarr->GetAt(i),ptarr->GetAt(i+1))*-10);
			}
			else 
				continue;
			break;
		case MOS_PREVORIENT:
			if(i!=0){
				lfont.lfEscapement = (int)(GetVektorGrad(ptarr->GetAt(i),ptarr->GetAt(i-1))*-10);
			}
			else 
				continue;
			break;
		case MOS_BISORIENT: 		
			{
                //Orientation on a bisector
                        //For the first and last point are entered voobrzhayemy continuation
                        //The basic rule for all round that the angle on following wasn't
                        //it is more, than an angle on previous (if it is that it it is necessary to turn
                        //in the similar negative
					int iNAngle,iPAngle;
					if(i==0){
						iNAngle = (int)GetVektorGrad(ptarr->GetAt(i),ptarr->GetAt(i+1))*10;
						iPAngle = iNAngle+1800;
					}
					else if(i != m_nPoints - 1){
                        //Calculation of a angle for a bisector
						iNAngle = (int)GetVektorGrad(ptarr->GetAt(i),ptarr->GetAt(i+1))*10;
						iPAngle = (int)GetVektorGrad(ptarr->GetAt(i),ptarr->GetAt(i-1))*10;
					}
					else{
						iPAngle = (int)GetVektorGrad(ptarr->GetAt(i),ptarr->GetAt(i-1))*10;
						iNAngle = iPAngle+1800;
					}
					if(iNAngle>iPAngle)iNAngle-=3600;
					lfont.lfEscapement = -1*((iNAngle+iPAngle)/2 - 900);
			}break;
//		default:			exit(15);
		};
        //It is also required to consider the general turn (which it is stored in a symbol)
		lfont.lfEscapement+=m_linia->m_symbol.logfont.lfEscapement;
		pDevice->SetFont(&lfont);
		pDevice->TextOut(ptarr->GetAt(i)->x,ptarr->GetAt(i)->y,m_linia->m_symbol.str);
	}
    //restore
	pDevice->RestoreFont();
}

//=======================================================================
//! Drawing: placement of a single symbol
void MDrawing::DrawSingular(MTopobject* pObj, MPlotDevice* pDevice, MSbSingular* singular, MPtArray* ptarr, MPtArray* axearr, bool synhro)
//=======================================================================
{
	if(!singular->m_symbol.visible) return;
    //we create a context necessary to us
	
	M_LOG_FONT lfont = singular->m_symbol.logfont;
	
	pDevice->SetBkMode(singular->m_symbol.bkmode);
	pDevice->SetTextAlign(singular->m_symbol.align);
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)singular->m_symbol.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))   color = pObj->m_sel_color;
	
	pDevice->SetTextColor(color);
	
	if(singular->m_symbol.scaled)
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,singular->m_symbol.value);
	else lfont.lfHeight = pDevice->LogicToBum(singular->m_symbol.value);
	
    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FONT_SB_SINGULAR))
		return;
	
    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial

	MPoint point = GetPointBySdvig(OsArr, DLINALOMLINE(OsArr)*singular->m_pause);
	
	if(synhro) 
	{
		int index = FindNomRebraOfPointOnLomline(OsArr, point);
		lfont.lfEscapement = (int)(-GetVektorGrad(OsArr->GetAt(index), OsArr->GetAt(index+1))*10);
	}
	
	pDevice->SaveFont();
	pDevice->SetFont(&lfont);
	
	pDevice->TextOut(point.x, point.y, singular->m_symbol.str);
	
    //restore
	pDevice->RestoreFont();
}

//=======================================================================
//! Drawing of the continuous redefined line
void MDrawing::DrawNastrLinia(MTopobject* pObj,MPlotDevice* pDevice,MNastrLinia* linia,MPtArray* ptarr)
//=======================================================================
{
    //prepare a pen
	MColor color;
	pObj->GetKarta()->GetDictColorValue(linia->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;

	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}
    //Thickness by which drawing is made (in real sizes)
	double dThickness = (*ConvertSizeToReal)(pObj,linia->m_thickness);
	if(linia->IsPereoprIndex(IDL_NAS_LINIA_VAL_TOL))
		pObj->GetDoubleDataPole(linia->GetPerIndex(IDL_NAS_LINIA_VAL_TOL),dThickness);

	pDevice->SavePencil();

//	int cptxWidth = pDevice->BumToLogic(dThickness);
	if(pDevice->Distinguish(dThickness)) {
        //on a tabular code we will receive color from the dictionary of colours if it is possible,
        //otherwise we will take color by default
		pDevice->SetPencil(PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_ROUND,
			dThickness,&color);
	}
	else {
		M_LOG_PEN drawpen;
        //on a tabular code we will receive color from the dictionary of colours if it is possible,
        //otherwise we will take color by default
		drawpen.lopnColor=color;
		drawpen.lopnStyle=PS_SOLID;
		drawpen.lopnWidth.x = dThickness;
		pDevice->SetPencil(&drawpen);
	}

    //draw
	pDevice->Polyline(ptarr->GetHead(),ptarr->GetSize());

    //restore
	pDevice->RestorePencil();
}
//=======================================================================
//! Drawing of the shaped line
void MDrawing::DrawNastrShtrih(MTopobject* pObj,MPlotDevice* pDevice,
				MNastrShtrih* shtrih,MPtArray* ptarr, MPtArray* axearr,bool synhro)
//=======================================================================
{
	
	//
    //   IT DRAWS THE SEGMENT AS:
	//
	//
	//         ---------------------
	//        | =================== |
	//         ---------------------

	//
    //  There == - real segment
    //        -- - that we see on the screen
    //
//*******************************************
    //prepare basic data

	double d,s,p;
    //receive length of a stroke and a space the geographical
	d = (*ConvertSizeToReal)(pObj,shtrih->m_dlina);
	s = (*ConvertSizeToReal)(pObj,shtrih->m_period);
	if(shtrih->IsPereoprIndex(IDL_NAS_SHTRIH_VAL_DLINA))
		pObj->GetDoubleDataPole(shtrih->GetPerIndex(IDL_NAS_SHTRIH_VAL_DLINA),d);
	if(shtrih->IsPereoprIndex(IDL_NAS_SHTRIH_VAL_PERIOD))
		pObj->GetDoubleDataPole(shtrih->GetPerIndex(IDL_NAS_SHTRIH_VAL_PERIOD),s);

	p = s - d;

    //If the line isn't indiscernible, it passes into a straight line
	if(!pDevice->Distinguish((d+p)/VISIBLE_SHTRIH_STEP_PLUS_LENGTH)){
		M_LOG_PEN lpen;
		MColor color;
		pObj->GetKarta()->GetDictColorValue(shtrih->m_colorkod, color);
		MColor whiteclr;
		pObj->GetKarta()->GetDictColorValue(1, whiteclr);
		if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
		if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
		lpen.lopnStyle = PS_SOLID;
		lpen.lopnColor = color;
		lpen.lopnWidth = MPoint((double)0,(double)0);
		pDevice->SetPencil(&lpen);
		pDevice->Polyline(ptarr->GetHead(),ptarr->GetSize());
		pDevice->RestorePencil();
		return;
	}

    //temporarily we keep continuous settings for drawing of the first space
	double d1=d;
	double p1=p;
	double s1=s;
    //establish continuous settings for drawing of the first space
    p=(*ConvertSizeToReal)(pObj,shtrih->m_otstup);//GEOM(shtrih->m_otstup);
    //If this size is redefined

	if(shtrih->IsPereoprIndex(IDL_NAS_SHTRIH_VAL_OTSTUP))
		pObj->GetDoubleDataPole(shtrih->GetPerIndex(IDL_NAS_SHTRIH_VAL_OTSTUP),p);
	s=p;

	d=0;
	bool first=true;

	

    //prepare a pen
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(shtrih->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}

	double dThickness = (*ConvertSizeToReal)(pObj,shtrih->m_thickness);
	if(shtrih->IsPereoprIndex(IDL_NAS_SHTRIH_VAL_TOL))
		pObj->GetDoubleDataPole(shtrih->GetPerIndex(IDL_NAS_SHTRIH_VAL_TOL),dThickness);

	pDevice->SavePencil();

	if(shtrih->m_kvadrat)
		pDevice->SetPencil(PS_GEOMETRIC|PS_ENDCAP_FLAT|PS_JOIN_ROUND,dThickness,&color);
	else
		pDevice->SetPencil(PS_GEOMETRIC|PS_ENDCAP_ROUND|PS_JOIN_ROUND,dThickness,&color);
	
	
	MPoint pt4,pt5,pt6;

    double t=0; //the current length from the beginning of s;
	
    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
    MPtArray* BkArr=ptarr;//cover


    MBoArray* What=new MBoArray();//array of action
	What->SetSize(100);
    MPtArray* Where=new MPtArray();//array of movements
	Where->SetSize(100);
    int WW=0;//counter
    bool flag;//service
	

//****************************************************
    //form arrays of action and movements

    //start drawing with a zero point
	flag=false;
	pt4=BkArr->GetAt(0)[0];
	pt5=BkArr->GetAt(1)[0];
	pt6=OsArr->GetAt(0)[0];
	What->SetAt(WW,&flag);
  MPoint point= GetPointOnOtrezok(pt4,pt5,pt6);
	Where->SetAt(WW++,&point);

	int m_nPoints=OsArr->GetSize();
	
	int i;
	double dlina;
	double ostatok,ostatok1,k;
	MPoint pt1,pt2; //,pt3;
    //on all segments of top object
	for(i=0;i<m_nPoints-1;i++)
	{
		pt1=OsArr->GetAt(i)[0];
		pt2=OsArr->GetAt(i+1)[0];
		pt4=BkArr->GetAt(i)[0];
		pt5=BkArr->GetAt(i+1)[0];

		
		dlina=sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x)+(pt2.y-pt1.y)*(pt2.y-pt1.y));

        //while length is not zero
		while(dlina>0)
		{
			ostatok=s-t;
            //if the rest is more than length of a space,
            //that we draw at first a stroke
			if(ostatok>p)
			{
                //the remained stroke
				ostatok1=ostatok-p;
                //if it more than length of a segment, we also draw it up to the end
				if(ostatok1>dlina)
				{
                    //draw
					if(WW==What->GetSize())
					{
						What->SetSize(WW+100);
						Where->SetSize(WW+100);
					}
					flag=true;
					What->SetAt(WW,&flag);
					Where->SetAt(WW++,&pt5);

					t+=dlina;
					dlina=0.0;
				}
                //if it less than length of a segment,
                //that we draw it all
				else
				{
					k=ostatok1/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok1;
					t=d;
                    //draw
					if(WW==What->GetSize())
					{
						What->SetSize(WW+100);
						Where->SetSize(WW+100);
					}
					flag=true;
					What->SetAt(WW,&flag);
          MPoint point = GetPointOnOtrezok(pt4,pt5,pt1);
					Where->SetAt(WW++,&point);
				}
			}
            //if we didn't finish a space
			else
			{
                //if the space is more than length,
				if(ostatok>dlina)
				{
					t+=dlina;
					dlina=0.0;
					
				}
                //otherwise we put an end
				else
				{
					k=ostatok/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok;
					t=0;
                    //if it was the first space, restore characteristics
					if(first)
					{
						s=s1;
						d=d1;
						p=p1;
						first=false;
					}
					if(!((i==m_nPoints-2)&&(dlina==ostatok)))
					{
                        //move
						if(WW==What->GetSize())
						{
							What->SetSize(WW+100);
							Where->SetSize(WW+100);
						}
						flag=false;

						What->SetAt(WW,&flag);
            MPoint point = GetPointOnOtrezok(pt4,pt5,pt1);
						Where->SetAt(WW++,&point);
					}
				}
			}
		}
	}



//*****************************************************
//draw a stroke on codes of arrays of action and movements

    //now we will draw everything
    int cnt;//number of points in a polyline
	int ind=0;
	int bb,ee;
	bool bo,bo1;
    //draw polylines which we now will construct
	while(ind<WW)
	{
        //indexes of the beginning and end of a polyline are negative since we don't know them yet
		bb=-1;
		ee=-1;
        //receive a drawing flag (true - to draw, false - to move )
		bo=What->GetAt(ind)[0];
        //if to move
		if(!bo)
		{
            //if the end of the array, is farther than nothing
			if(ind==WW-1) ind++;
            //else
			else
			{
                //flag of drawing of the following element
				bo1=What->GetAt(ind+1)[0];
                //if it - to draw, we received the beginning of a polyline
                if(bo1)
				{
					bb=ind;
					ee=ind;
					ind++;
                    //look for the termination of a segment of a polyline
					while((ind<WW)&&(What->GetAt(ind)[0]))
					{
						ee=ind;
						ind++;
					}
                    //now, if we found a polyline, we will draw it
					if((bb!=-1)&&(ee!=-1))
					{
						cnt=ee-bb+1;
                        //if the array of points is small, we increase it
						pDevice->Polyline(Where->GetAt(bb),cnt);
						bb=ee=-1;
					}
				}
                //if to move, we do nothing
				else ind++;
			}
		}
        //otherwise we have some failure and we ignore it
		else ind++;
	}

	//****************************************************
    //restore
	pDevice->RestorePencil();
	delete What;
	delete Where;
}

//=======================================================================
//! Drawing of a dotted line
void MDrawing::DrawNastrPunktir(MTopobject* pObj,MPlotDevice* pDevice,
		MNastrPunktir* punktir,MPtArray* ptarr, MPtArray* axearr,bool synhro)
//=======================================================================
{
	double p,p1;
	
	p = (*ConvertSizeToReal)(pObj,punktir->m_nachalo);
	p1 = (*ConvertSizeToReal)(pObj,punktir->m_period);
	if(punktir->IsPereoprIndex(IDL_NAS_PUNKTIR_VAL_PERIOD))
		pObj->GetDoubleDataPole(punktir->GetPerIndex(IDL_NAS_PUNKTIR_VAL_PERIOD),p1);
	if(punktir->IsPereoprIndex(IDL_NAS_PUNKTIR_VAL_OTSTUP))
		pObj->GetDoubleDataPole(punktir->GetPerIndex(IDL_NAS_PUNKTIR_VAL_OTSTUP),p);

    //If the dotted line isn't distinguishable it passes into the simple line
	if(!pDevice->Distinguish(p1/VISIBLE_PUNKTIR_STEP))
	{
		M_LOG_PEN lpen;
		MColor color;
		pObj->GetKarta()->GetDictColorValue(punktir->m_colorkod, color);
		MColor whiteclr;
		pObj->GetKarta()->GetDictColorValue(1, whiteclr);
		if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
		if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
		lpen.lopnStyle = PS_SOLID;
		lpen.lopnColor = color;
		lpen.lopnWidth = MPoint((double)0,(double)0);
		pDevice->SetPencil(&lpen);
		pDevice->Polyline(ptarr->GetHead(),ptarr->GetSize());
		pDevice->RestorePencil();
		return;
	}
 
	bool first=true;
	
//	if ((punktir->m_diametr<0.0001)||(punktir->m_diametr>p)) return;
	
    //dotted line radius in logical coordinates
	double diamReal = (*ConvertSizeToReal)(pObj,punktir->m_diametr);
	if(punktir->IsPereoprIndex(IDL_NAS_PUNKTIR_VAL_TOL))
		pObj->GetDoubleDataPole(punktir->GetPerIndex(IDL_NAS_PUNKTIR_VAL_TOL),diamReal);
	double d = diamReal/2;


    //prepare a transparent pen
	M_LOG_PEN drawpen;
    memset(&drawpen,0x0,sizeof(drawpen));
	drawpen.lopnStyle=PS_NULL;
	
	pDevice->SavePencil();
	pDevice->SetPencil(&drawpen);

	
    //prepare the necessary brush
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(punktir->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;

	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}

	pDevice->SaveZapolnenie();
	pDevice->SetZapolnenie(&color);

    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
    MPtArray* BkArr=ptarr;//cover

	MPoint pt4,pt5; //,pt6;

    double t=0; //the current length from the beginning of d;

	
	int m_nPoints=OsArr->GetSize();
	
	int i;
	double dlina;
	double ostatok,k;
	MPoint pt1,pt2;
    //on all segments of top object
	for(i=0;i<m_nPoints-1;i++)
	{
		pt1=OsArr->GetAt(i)[0];
		pt2=OsArr->GetAt(i+1)[0];
		pt4=BkArr->GetAt(i)[0];
		pt5=BkArr->GetAt(i+1)[0];
		
		dlina=sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x)+(pt2.y-pt1.y)*(pt2.y-pt1.y));

		while(dlina>0)
		{
			ostatok=p-t;
            //if the rest is more than length
			if(ostatok>dlina)
			{
				t+=dlina;
				dlina=0.0;
			}
            //otherwise we put a circle
			else
			{
					k=ostatok/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok;
					t=0;
					MPoint pt = GetPointOnOtrezok(pt4,pt5,pt1);
					pDevice->Ellipse(pt.x-d,pt.y-d,pt.x+d,pt.y+d);
					if(first)
					{
						p=p1;
						first=false;
					}
			}
			
		}

	}


    //restore
	pDevice->RestorePencil();
	pDevice->RestoreZapolnenie();
}

//=======================================================================
//! Drawing of a dotted line
void MDrawing::DrawNastrSbStretch(MTopobject* pObj,MPlotDevice* pDevice,MNastrSbStretch* SbStretch,
		MPtArray* ptarr,MPtArray* axearr,bool synhro)
//=======================================================================
{
	if(!SbStretch->m_symbol.visible) return;
    //create a context necessary to us

	M_LOG_FONT lfont = SbStretch->m_symbol.logfont;

	pDevice->SetBkMode(SbStretch->m_symbol.bkmode);
	pDevice->SetTextAlign(UT_TA_BASELINE|UT_TA_CENTER);//SbStretch->symbol->align);//TA_BASELINE);//TA_CENTER);//SbStretch->symbol->align);//TA_CENTER||TA_BASELINE);
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)SbStretch->m_symbol.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
	pDevice->SetTextColor(color);
	
	if(SbStretch->m_symbol.scaled)
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,SbStretch->m_symbol.value);
	else
		lfont.lfHeight = pDevice->LogicToBum(SbStretch->m_symbol.value);
	


    //If the sizes or an angle of rotation of a symbol are redefined
	double dTemp;
	if(SbStretch->IsPereoprIndex(IDL_NAS_SBSTRETCH_VAL_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(SbStretch->GetPerIndex(IDL_NAS_SBSTRETCH_VAL_SYM_UGOL),dTemp))
			lfont.lfEscapement = (int)(dTemp*10);
	}

	if(SbStretch->IsPereoprIndex(IDL_NAS_SBSTRETCH_VAL_SYM_VAL)){
		if(pObj->GetDoubleDataPole(SbStretch->GetPerIndex(IDL_NAS_SBSTRETCH_VAL_SYM_VAL),dTemp))
			lfont.lfHeight= dTemp;
	}


    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FONT_SB_STRETCH))
		return;

	pDevice->SaveFont();
	pDevice->SetFont(&lfont);


	double p,p1;

	p1 = (*ConvertSizeToReal)(pObj,SbStretch->m_period);
	p = (*ConvertSizeToReal)(pObj,SbStretch->m_sbpause);
	if(SbStretch->IsPereoprIndex(IDL_NAS_SBSTRETCH_VAL_PERIOD))
		pObj->GetDoubleDataPole(SbStretch->GetPerIndex(IDL_NAS_SBSTRETCH_VAL_PERIOD),p1);
	if(SbStretch->IsPereoprIndex(IDL_NAS_SBSTRETCH_VAL_OTSTUP))
		pObj->GetDoubleDataPole(SbStretch->GetPerIndex(IDL_NAS_SBSTRETCH_VAL_OTSTUP),p);

	bool first=true;
	
    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
    MPtArray* BkArr=ptarr;//cover

	MPoint pt4,pt5; //,pt6;

	
    double t=0; //the current length from the beginning of d;
    //we start drawing with a zero point
	int m_nPoints=ptarr->GetSize();
	MPoint pt1,pt2;
	
	
	int i;
	double dlina;
	double ostatok,k;
	
    //on all segments of top object
    for(i=0;i<m_nPoints-1;i++)
	{
		pt1=OsArr->GetAt(i)[0];
		pt2=OsArr->GetAt(i+1)[0];
		pt4=BkArr->GetAt(i)[0];
		pt5=BkArr->GetAt(i+1)[0];
		lfont.lfEscapement=(int)(-GetVektorGrad(&pt1,&pt2)*10); 
		dlina=sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x)+(pt2.y-pt1.y)*(pt2.y-pt1.y));

		while(dlina>0)
		{
			ostatok=p-t;
            //if the rest is more than length
            if(ostatok>dlina)
			{
				t+=dlina;
				dlina=0.0;
			}
            //otherwise we put a circle
            else
			{
					k=ostatok/dlina;
					pt1.x+=k*(pt2.x-pt1.x);
					pt1.y+=k*(pt2.y-pt1.y);
					dlina-=ostatok;
					t=0;
					MPoint pt = GetPointOnOtrezok(pt4,pt5,pt1);
					pDevice->SetFont(&lfont);
					pDevice->TextOut(pt.x,pt.y,SbStretch->m_symbol.str);
					if(first)
					{
						p=p1;
						first=false;
					}
			}
			
		}
	}
    //restore
	pDevice->RestoreFont();
}

//=======================================================================
//! Drawing of a spline
void MDrawing::DrawNastrSpline(MTopobject* pObj,MPlotDevice* pDevice,MNastrSpline* spline,MPtArray* ptarr)
//=======================================================================
{
    //prepare a pen
	M_LOG_PEN drawpen;
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(spline->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}

	drawpen.lopnColor=color;
	drawpen.lopnStyle=PS_SOLID;
	double dThick = (*ConvertSizeToReal)(pObj,spline->m_thickness);
	if(spline->IsPereoprIndex(IDL_NAS_SPLINE_VAL_TOL))
		pObj->GetDoubleDataPole(spline->GetPerIndex(IDL_NAS_SPLINE_VAL_TOL),dThick);

	drawpen.lopnWidth.x=drawpen.lopnWidth.y=dThick;


	int iKolPts = spline->m_kolpts;
	double dTemp;
	if(spline->IsPereoprIndex(IDL_NAS_SPLINE_VAL_NUMPOINTS)){
		pObj->GetDoubleDataPole(spline->GetPerIndex(IDL_NAS_SPLINE_VAL_NUMPOINTS),dTemp);
		iKolPts = (int)dTemp;
	}


	pDevice->SavePencil();
	pDevice->SetPencil(&drawpen);

	int m_nPoint=ptarr->GetSize();
	int type = spline->m_type;
	if(iKolPts == 0)
		type = SPT_LOMANAYA;

    // we will draw horizontals one of the following types:
	switch(type)
	{
	case SPT_LOMANAYA:
		pDevice->Polyline(ptarr->GetHead(),m_nPoint);
		break;
	case SPT_INTERP:
		{
        MPtArray* newarr = nullptr;
//        MPtArray* newarr = new MPtArray();
            // on an entrance of function of creation of an interpolation spline 'LOMTOISPL' we give the array of points 'ptarr' - an initial broken line,
            //at the exit we have the new array of points 'newarr' - a final broken line on which we carry out a spline
			newarr = LOMTOISPL(ptarr, spline->m_vid, iKolPts);
			m_nPoint = newarr->GetSize();

			pDevice->Polyline(newarr->GetHead(),m_nPoint);
			delete newarr;
		}
		break;
	case SPT_APPROX:
		{
			double* param;
//			int nn = m_nPoint - 1;
			
			if(spline->m_vid == SPV_ZAMKNUT)
			{
                //temporarily shag = 0.5
				double shag = 0.5;
                // boundary conditions of the third sort
				param = (double*)malloc(sizeof(double)*(m_nPoint-1));
				for(int i=0; i<m_nPoint-1; i++)
					param[i] = shag/4;
			}
            // case when we build not closed spline...
			else{
                //temporarily shag = 0.5
				double shag = 0.5;
				param = (double*)malloc(sizeof(double)*m_nPoint);
				param[0] = 0;
				param[m_nPoint-1] = 0;
				for(int i=1; i<m_nPoint - 1; i++)
					param[i] = shag/4;
			}
            MPtArray* newarr = nullptr;
//            MPtArray* newarr = new MPtArray();
            //on an entrance of function of creation of an approximating spline 'LOMTOASPL' we give the array of points 'ptarr' - an initial broken line,
            //at the exit we have the new array of points 'newarr' - a final broken line on which we carry out a spline
			newarr = LOMTOASPL(ptarr, spline->m_vid, param, iKolPts);
//			int number = 0;
//			GetArrayOfPoint(number)->ClearAll();
//			((MPtArray*)this->m_Komplekt->GetAt(F_TOCHKI))->AppendTail(ptarr);
			m_nPoint = newarr->GetSize();

			pDevice->Polyline(newarr->GetHead(),m_nPoint);
			delete newarr;
			free(param);
		}
			break;
	}

    //restore
	pDevice->RestorePencil();
}

// ==================================================================
//drawing of the redefined layer of a single symbol
void MDrawing::DrawNastrSingular(MTopobject* pObj, MPlotDevice* pDevice, MNastrSbSingular* singular,
									  MPtArray* ptarr, MPtArray* axearr, bool synhro)
// ==================================================================
{
	if(!singular->m_symbol.visible) return;
    //create a context necessary to us
	
	MSymbol smb;
	smb.Copy(&singular->m_symbol);
	pDevice->SetBkMode(smb.bkmode);

	pDevice->SetTextAlign(smb.align);//UT_TA_BASELINE|UT_TA_CENTERSbStretch->symbol->align);//TA_BASELINE);//TA_CENTER);//SbStretch->symbol->align);//TA_CENTER||TA_BASELINE);
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)smb.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;

	if((pObj->m_selected)&&(color != whiteclr))       
			color = pObj->m_sel_color;
 
	pDevice->SetTextColor(color);

	int value = smb.value;
	int iEscape = smb.logfont.lfEscapement;

    //redefine a symbol
	if(singular->IsPereoprIndex(IDL_NAS_SBSINGULAR_SYM_OPISANIE))
	{
		MString str = pObj->GetDataPole(singular->GetPerIndex(IDL_NAS_SBSINGULAR_SYM_OPISANIE));
		MString strtrue;
		MZnak* pZnak = pObj->GetKarta()->m_pKls->GetZnak(str, strtrue, true);
		if((pZnak)&&(pZnak->GetYaz()->GetType() == F_DOT))
		{
			MSzArray* szArr = ((MDotYaz*)(pZnak->GetYaz())->m_object)->m_tochka;
			MShriftSrez* pSrez = (MShriftSrez*)szArr->GetTail();
			smb = pSrez->m_tochka;
		}
	}

	M_LOG_FONT lfont = smb.logfont; //singular->m_symbol.logfont;

	
    //If the sizes or an angle of rotation of a symbol are redefined
	double dTemp;
	if(singular->IsPereoprIndex(IDL_NAS_SBSINGULAR_VAL_SYM_VAL))
	{
		if(pObj->GetDoubleDataPole(singular->GetPerIndex(IDL_NAS_SBSINGULAR_VAL_SYM_VAL),dTemp))
			lfont.lfHeight= dTemp;
		else lfont.lfHeight = value;//smb.value;
	}
	else lfont.lfHeight = value;//smb.value;

	if(singular->m_symbol.scaled)
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,lfont.lfHeight);
	else lfont.lfHeight = pDevice->LogicToBum(lfont.lfHeight);
	
    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FONT_SB_SINGULAR))
		return;	



	//int iEscape = 0;
	if(singular->IsPereoprIndex(IDL_NAS_SBSINGULAR_VAL_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(singular->GetPerIndex(IDL_NAS_SBSINGULAR_VAL_SYM_UGOL),dTemp))
			iEscape = (int)(dTemp*10);
			//lfont.lfEscapement = (int)(dTemp*10);
	}
	//else iEscape = smb.logfont.lfEscapement;


    //redefine shift concerning the beginning
	double sdvig = singular->m_pause;
	if(singular->IsPereoprIndex(IDL_NAS_SBSINGULAR_VAL_SDVIG)){
		if(pObj->GetDoubleDataPole(singular->GetPerIndex(IDL_NAS_SBSINGULAR_VAL_SDVIG), dTemp))
			sdvig = dTemp;
	}
	
    MPtArray* OsArr=(synhro)? axearr:ptarr;//axial
	
	MPoint point = GetPointBySdvig(OsArr, DLINALOMLINE(OsArr)*sdvig);
	
	if(synhro) 
	{
		int index = FindNomRebraOfPointOnLomline(OsArr, point);
		lfont.lfEscapement = (int)(-GetVektorGrad(OsArr->GetAt(index), OsArr->GetAt(index+1))*10)-iEscape;
	}
	else lfont.lfEscapement = iEscape;
	
	pDevice->SaveFont();
	pDevice->SetFont(&lfont);
	
	pDevice->TextOut(point.x, point.y, smb.str /*singular->m_symbol.str*/);
	
    //restore
	pDevice->RestoreFont();
}

// ==================================================================
//! Shading drawing
void MDrawing::DrawNastrShtrihovka(MTopobject* pObj,MPlotDevice* pDevice,
								MNastrShtrihovka* pShtrihovka,MPtArray* pta)
// ==================================================================
{
	MColor color;

    //define a step of shading and its thickness in logical coordinates
	double step=fabs((*ConvertSizeToReal)(pObj,pShtrihovka->m_shag));
	double tols=fabs((*ConvertSizeToReal)(pObj,pShtrihovka->m_thickness));


    //If redefinition of parameters is executed
	if(pShtrihovka->IsPereoprIndex(IDL_NAS_SHTRIHOVKA_VAL_STEP)){
		double dTemp;
		if(pObj->GetDoubleDataPole(pShtrihovka->GetPerIndex(IDL_NAS_SHTRIHOVKA_VAL_STEP),dTemp))
			step = dTemp;
	}

	if(pShtrihovka->IsPereoprIndex(IDL_NAS_SHTRIHOVKA_VAL_TOL)){
		double dTemp;
		if(pObj->GetDoubleDataPole(pShtrihovka->GetPerIndex(IDL_NAS_SHTRIHOVKA_VAL_TOL),dTemp))
			tols = dTemp;
	}
	if(step<=tols){
        //In this case we fill in the polygon
		pDevice->SaveZapolnenie();
		pDevice->SetZapolnenie(&color);
		pDevice->Polygon(pta->GetHead(),pta->GetSize());
		pDevice->RestoreZapolnenie();
		return;
	}

    //And if shading itself isn't visible, its drawing isn't necessary
	if(!pDevice->Distinguish(step/VISIBLE_SHTRIHOVKA_STEP))
		return;

    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take color by default
	pObj->GetKarta()->GetDictColorValue(pShtrihovka->m_colorkod, color);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}

	M_LOG_PEN pen(PS_SOLID,MPoint(tols,tols),color);
	pDevice->SavePencil();
	pDevice->SetPencil(&pen);
	
    //the angle is expressed in degrees
	double angle=pShtrihovka->m_ugol;

    //If the angle is redefined
	if(pShtrihovka->IsPereoprIndex(IDL_NAS_SHTRIHOVKA_VAL_UGOL))
		pObj->GetDoubleDataPole(pShtrihovka->GetPerIndex(IDL_NAS_SHTRIHOVKA_VAL_UGOL),angle);

    //create area in which drawing is allowed
//	pDevice->BeginClipRgn();
	pDevice->SaveClipping();
	pDevice->AddClipRgn(pta->GetHead(),pta->GetSize(),INTERSECT_REGION);
//	pDevice->SelectClipRgn();

    //shade this area
  MRect rectangle = GetObhvatRect(pta);
	DrawShtrihRect(pDevice,&rectangle,angle,step);
	
    //if shading double, we shade once again perpendicularly
	if(pShtrihovka->m_type==SHR_DVOINAIA)
	{
        //we lead an angle to a normal amount
		while(angle>=360)angle-=360;
		while(angle<0)angle+=360;
		if((angle==0)||(angle==180)) angle=90;
		else if((angle==90)||(angle==270)) angle=0;
		else angle=180-angle;
        //draw
  MRect rectangle = GetObhvatRect(pta);
		DrawShtrihRect(pDevice,&rectangle,angle,step);
	}

//	pDevice->ClearClip();
	pDevice->RestoreClipping();
	pDevice->RestorePencil();
}

// ==================================================================
//! Drawing of a pattern
void MDrawing::DrawNastrUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MNastrUzor* pUzor,MPtArray* pta)
// ==================================================================
{
  MRect rectangle;
    //define a step of shading and its thickness
	double step=fabs((*ConvertSizeToReal)(pObj,pUzor->m_shag));
    //If the step is redefined
	if(pUzor->IsPereoprIndex(IDL_NAS_UZOR_VAL_STEP))
		pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_UZOR_VAL_STEP),step);
    //The step can't be zero
	if(!step)	
		return;

    //set the mode of drawing of symbols necessary to us
	MSymbol drawsym;
	drawsym.Copy(&pUzor->m_symbol);

    // symbol font
	unsigned int uiOldMode = pDevice->SetBkMode(UT_TRANSPARENT);
    unsigned int uiOldAlign = pDevice->SetTextAlign(drawsym.align);

    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)drawsym.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}

	MColor crOldTextColor = pDevice->SetTextColor(color);
	M_LOG_FONT lfont = drawsym.logfont;
	if(drawsym.scaled)
		lfont.lfHeight = (*ConvertSizeToReal)(pObj,drawsym.value);
	else
		lfont.lfHeight = pDevice->LogicToBum(drawsym.value);

    //If there is a redefinition of a symbol
    //If the sizes or an angle of rotation of a symbol are redefined

	double dTemp;
	if(pUzor->IsPereoprIndex(IDL_NAS_UZOR_VAL_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_UZOR_VAL_SYM_UGOL),dTemp))
			drawsym.logfont.lfEscapement = (int)(dTemp*10);
	}

	if(pUzor->IsPereoprIndex(IDL_NAS_UZOR_VAL_SYM_VAL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_UZOR_VAL_SYM_VAL),dTemp))
			lfont.lfHeight= dTemp;
	}


    // it is drawn at joint performance of two conditions:
    //if it is marked as visible
    //and if it is readable in this scale
	if ((!drawsym.visible)||(!pDevice->Distinguish_Text(lfont.lfHeight/VISIBLE_FONT_UZOR))) 
		return;

	pDevice->SaveFont();
	pDevice->SetFont(&lfont);
	
    //the angle of a grid is expressed in degrees
	double angle=0;
	if(pUzor->m_type==STK_KOS){
		angle=pUzor->m_ugol;
        //And if it is redefined
		if(pUzor->IsPereoprIndex(IDL_NAS_UZOR_VAL_UGOL))
			pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_UZOR_VAL_UGOL),angle);
	}




	MPtArray* ptaInternal =NULL;
    //now directly drawing depending on grid type
	switch(pUzor->m_type)
	{
	case STK_PRAM:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-lfont.lfHeight);
		else
			ptaInternal = BuildObvod(pta,lfont.lfHeight);
    rectangle = GetObhvatRect(pta);
		DrawPramSetka(pDevice,ptaInternal,&rectangle,&drawsym.str,step,lfont.lfHeight);
		break;
	case STK_KOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-lfont.lfHeight);
		else
			ptaInternal = BuildObvod(pta,lfont.lfHeight);
    rectangle = GetObhvatRect(pta);
		DrawKosSetka(pDevice,ptaInternal,&rectangle,&drawsym.str,angle,step,lfont.lfHeight);
		break;
	case STK_XAOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-(lfont.lfHeight+step/2));
		else
			ptaInternal = BuildObvod(pta,(lfont.lfHeight+step/2));
    rectangle = GetObhvatRect(pta);
		DrawXaosSetka(pDevice,ptaInternal,&rectangle,&drawsym,step,lfont.lfHeight);
		break;
	}
	if (ptaInternal) delete ptaInternal;
  
	pDevice->RestoreFont();
	pDevice->SetBkMode(uiOldMode);
    pDevice->SetTextAlign(uiOldAlign);
	pDevice->SetTextColor(crOldTextColor);
}

// ==================================================================
//! Drawing of a percentage pattern
void MDrawing::DrawNastrPercentUzor(MTopobject* pObj,MPlotDevice* pDevice,
						  MNastrPercentUzor* pUzor,MPtArray* pta)
// ==================================================================
{
   MRect rectangle;
   //define a step of shading and its thickness
    double step=fabs((*ConvertSizeToReal)(pObj,pUzor->m_shag));

    //If the step is redefined
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_STEP))
		pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_STEP),step);
		
	
    //create fonts for drawing of all symbols in percentage drawing
	PercentParameters params;
	params.FirstSym.Copy(&pUzor->m_symbol);	
	params.SecSym.Copy(&pUzor->m_SecSym);	
	params.ThirdSym.Copy(&pUzor->m_ThirdSym);	

    //If there is a redefinition of angles and symbols
    //sizes
	double dTemp;
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_1_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_1_SYM_UGOL),dTemp))
			params.FirstSym.logfont.lfEscapement = (int)(dTemp*10);
	}
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_2_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_2_SYM_UGOL),dTemp))
			params.SecSym.logfont.lfEscapement = (int)(dTemp*10);
	}
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_3_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_3_SYM_UGOL),dTemp))
			params.ThirdSym.logfont.lfEscapement = (int)(dTemp*10);
	}

	double d1H,d2H,d3H;
	double dOverridenValue = 0;

	M_LOG_FONT lfont = params.FirstSym.logfont;
	d1H = lfont.lfHeight = (*ConvertSizeToReal)(pObj,params.FirstSym.value);

    //If the symbol size is redefined
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_1_SYM_VAL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_1_SYM_VAL),dTemp)){
			d1H = lfont.lfHeight= dTemp;
			dOverridenValue = Max(dOverridenValue,dTemp);
		}
	}

	pDevice->AddFontToPool(&lfont,400);
	params.iFirstFont = 400;
    //If a grid chaotic, all 12 fonts of turn are created
	if(pUzor->m_type==STK_XAOS)
		for(int i = 1;i<12;i++){
			lfont.lfEscapement +=300;
			pDevice->AddFontToPool(&lfont,params.iFirstFont+i);
		}


    //SECOND FONT
	lfont = params.SecSym.logfont;
	d2H = lfont.lfHeight = (*ConvertSizeToReal)(pObj,params.SecSym.value);
    //If the symbol size is redefined
    if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_2_SYM_VAL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_2_SYM_VAL),dTemp)){
			d2H = lfont.lfHeight = dTemp;
			dOverridenValue = Max(dOverridenValue,dTemp);
		}
	}
	pDevice->AddFontToPool(&lfont,412);
	params.iSecFont = 412;
    //If a grid chaotic, all 12 fonts of turn are created
    if(pUzor->m_type==STK_XAOS)
		for(int i = 1;i<12;i++){
			lfont.lfEscapement +=300;
			pDevice->AddFontToPool(&lfont,params.iSecFont+i);
		}

    //THIRD FONT
	lfont = params.ThirdSym.logfont;
	d3H = lfont.lfHeight = (*ConvertSizeToReal)(pObj,params.ThirdSym.value);
    //If the symbol size is redefined
    if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_3_SYM_VAL)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_3_SYM_VAL),dTemp)){
			d3H = lfont.lfHeight = dTemp;
			dOverridenValue = Max(dOverridenValue,dTemp);
		}
	}
	pDevice->AddFontToPool(&lfont,424);
	params.iThirdFont = 424;
    //If a grid chaotic, all 12 fonts of turn are created
    if(pUzor->m_type==STK_XAOS)
		for(int i = 1;i<12;i++){
			lfont.lfEscapement +=300;
			pDevice->AddFontToPool(&lfont,params.iThirdFont+i);
		}

	params.iFirstPercent = pUzor->m_iPercent1;
	params.iSecPercent = pUzor->m_iPercent2;
	params.iThirdPercent = pUzor->m_iPercent3;
	
    //If redefinition of percent was set
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_PERCENT_1)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_PERCENT_1),dTemp))
			params.iFirstPercent = (int)dTemp;
	}
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_PERCENT_2)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_PERCENT_2),dTemp))
			params.iSecPercent = (int)dTemp;
	}
	if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_PERCENT_3)){
		if(pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_PERCENT_3),dTemp))
			params.iThirdPercent = (int)dTemp;
	}




    //If it is invisible, it isn't displayed (on percent gets to not displayed area)
	if ((!params.FirstSym.visible)||(!pDevice->Distinguish(d1H/VISIBLE_FONT_UZOR)))
		params.iFirstPercent = 0;
	if ((!params.SecSym.visible)||(!pDevice->Distinguish(d2H/VISIBLE_FONT_UZOR)))
		params.iSecPercent = 0;
	if ((!params.ThirdSym.visible)||(!pDevice->Distinguish(d3H/VISIBLE_FONT_UZOR)))
		params.iThirdPercent = 0;


	MColor color;
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)params.FirstSym.colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	 if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
	params.cr1 = color;
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)params.SecSym.colorkod, color))
		color.SetColor(0,0,0);
	
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
	params.cr2   = color;
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)params.ThirdSym.colorkod, color))
		color.SetColor(0,0,0);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	 if((pObj->m_selected)&&(color != whiteclr))       // 28.03.2005   vipa -ATTENTION-
		{
			color = pObj->m_sel_color;
		}
	params.cr3 = color;

    //find out the maximum size of a symbol
	double symValReal = Max(Max(d1H,d2H),d3H);

	pDevice->SaveFont();
    // it is drawn at joint performance of two conditions:
    //if it is marked as visible
    //and if it is readable in this scale
	if((params.iFirstPercent==0)&&(params.iSecPercent==0)&&(params.iThirdPercent==0)){
		pDevice->RestoreFont();
		pDevice->ClearPool();
		return;
	}

	pDevice->SelectFontFromPool(400);

	unsigned int uiOldMode = pDevice->SetBkMode(UT_TRANSPARENT);
	unsigned int uiOldAlign= pDevice->SetTextAlign(UT_TA_CENTER);
	MColor crOldTextColor = pDevice->SetTextColor(MColor(0,0,0));
    //the angle of a grid is expressed in degrees
	double angle=0;
	if(pUzor->m_type==STK_KOS){
		angle=pUzor->m_ugol;
        //And if it is redefined
		if(pUzor->IsPereoprIndex(IDL_NAS_PERCENT_UZOR_VAL_UGOL))
			pObj->GetDoubleDataPole(pUzor->GetPerIndex(IDL_NAS_PERCENT_UZOR_VAL_UGOL),angle);
	}
	MPtArray* ptaInternal =NULL;
    //now directly drawing depending on grid type
	switch(pUzor->m_type)
	{
	case STK_PRAM:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-symValReal);
		else
			ptaInternal = BuildObvod(pta,symValReal);
   rectangle = GetObhvatRect(pta);
		DrawPramSetkaPercent(pDevice,ptaInternal,&rectangle,&params,step,symValReal);
		break;
	case STK_KOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-symValReal);
		else 
			ptaInternal = BuildObvod(pta,symValReal);
   rectangle = GetObhvatRect(pta);
		DrawKosSetkaPercent(pDevice,ptaInternal,&rectangle,&params,angle,step,symValReal);
		break;
	case STK_XAOS:
		if(Area(pta)<0)
			ptaInternal = BuildObvod(pta,-(symValReal+step/2));
		else
			ptaInternal = BuildObvod(pta,(symValReal+step/2));
   rectangle = GetObhvatRect(pta);
		DrawXaosSetkaPercent(pDevice,ptaInternal,&rectangle,&params,step,symValReal);
		break;
	}
	if (ptaInternal) delete ptaInternal;
	pDevice->RestoreFont();
	pDevice->ClearPool();
	pDevice->SetBkMode(uiOldMode);
    pDevice->SetTextAlign(uiOldAlign);
	pDevice->SetTextColor(crOldTextColor);
}


//=======================================================================
//! Drawing of a point on cuts
void MDrawing::DrawPoint(MTopobject* pObj,MYaz* pYaz,MPlotDevice* pDevice,MPtArray* pta)
//=======================================================================
{
	MSzArray* szar;
    //get access to the array of cuts
	szar=((MDotYaz*)(pYaz->m_object))->m_tochka;
    //ow we draw each cut
	int Cnt=szar->GetSize();
	for(int i=0;i<Cnt;i++)
	{
		MSrez* srez=szar->GetAt(i);

		switch(srez->GetType())
		{
		case IDT_SHRIFT:
			DrawShriftSrez(pObj,pDevice,(MShriftSrez*)srez,pta);
			break;
		case IDT_NAS_SHRIFT:
			DrawNastrShriftSrez(pObj,pDevice,(MNastrShriftSrez*)srez,pta);
			break;
		case IDT_PROIZVOL:
			DrawImageSrez(pObj,pDevice,(MProizvolSrez*)srez,pta);
//			exit(15);
			break;
		}
	}
}


//=======================================================================
//! Drawing of font dot object
void MDrawing::DrawShriftSrez(MTopobject* pObj,MPlotDevice* pDevice,MShriftSrez* pSrez,MPtArray* pta)
//=======================================================================
{
	MSymbol* sym = &pSrez->m_tochka;

	if(!sym->visible) return;

	int m_nPoints=pta->GetSize();
    //if the array of points empty, we draw nothing
	if(m_nPoints==0) return;

    //prepare a font
	M_LOG_FONT lfont = sym->logfont;


	pDevice->SetBkMode(sym->bkmode);
	pDevice->SetTextAlign(sym->align);
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)sym->colorkod, color))
		color.SetColor(0,0,0);

	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected)&&(color != whiteclr)) 
    {//If the object is chosen that we change color
		color = pObj->m_sel_color;
	}

	pDevice->SetTextColor(color);

	actRatio= sym->value / etalon_value;
	drawTextFlag= false;        // vipa 2004.11.05
	if(sym->scaled)
	{
		drawTextFlag= true;       // vipa 2004.11.05
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,sym->value);
    /*     vipa 2004.07.28
    //check on an text size exit for the set border
		int msize = textSizeMax;
		if(msize)                                                  
		{
			double max_size = pDevice->LogicToBum(msize);

			double etalon_size = 37.5;
			double text_size = sym->value;
			double koef = etalon_size/text_size;

			if((lfont.lfHeight*koef) > max_size) lfont.lfHeight = max_size/koef;
		}
    */
	}
	else
		lfont.lfHeight = pDevice->LogicToBum(sym->value);

    if(pObj->m_excited)	lfont.lfHeight *= 2;

  //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_POINT))
	   { drawTextFlag= false;   return; }         // vipa 2004.11.05
/*  // vipa -ATTENTION-
	pDevice->SaveFont();
	pDevice->SetFont(&lfont);

 // pObj->m_bounds.top=  pObj->m_bounds.bottom + lfont.lfHeight;  // vipa 2004.10.04  to work CheckSize for dots
	
	//Risuem vse simvoly
	for(int i=0;i<m_nPoints;i++)
		pDevice->TextOut(pta->GetAt(i)->x,pta->GetAt(i)->y,sym->str);
*/

// presumption of 2 points:   vector -> dot
// lfont.lfEscapement =-1 -> first point  with no rotation
// lfont.lfEscapement =-2 -> second point with no rotation
// otherwise              -> first point  with rotation

	pDevice->SaveFont();
	if (m_nPoints==2) {                             // 2005-03-30 vipa
	 //  cout << "m_nPoints=" <<m_nPoints<<endl;
	 double angle = GetVektorGrad(pta->GetAt(0),pta->GetAt(1) )*10;
	 if ((lfont.lfEscapement != -2)&&(lfont.lfEscapement != -1))
		lfont.lfEscapement  -= (int)dbl_to_int(angle);
	 pDevice->SetFont(&lfont);
	 if (lfont.lfEscapement==-2)
  		pDevice->TextOut(pta->GetAt(1)->x,pta->GetAt(1)->y,sym->str);
	 else
  		pDevice->TextOut(pta->GetAt(0)->x,pta->GetAt(0)->y,sym->str);
	}  
	else  {
		pDevice->SetFont(&lfont);
		for(int i=0;i<m_nPoints;i++)
			pDevice->TextOut(pta->GetAt(i)->x,pta->GetAt(i)->y,sym->str);
	}  

    //restore
	pDevice->RestoreFont();
}

//=======================================================================
//! Drawing of any dot object
void MDrawing::DrawImageSrez(MTopobject* pObj,MPlotDevice* pDevice,MProizvolSrez* pSrez,MPtArray* pta)
//=======================================================================
{
	if(!pSrez) return;
	if (!pta->GetSize()) return;

    //create a brush necessary to us and a transparent pen
	MColor color;
    //on a tabular code we will receive color from the dictionary of colours if it is possible,
    //otherwise we will take white color
	if(!pObj->GetKarta()->GetDictColorValue(pSrez->m_colorkod, color))
		color.SetColor(255,255,255);
	
	if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
	{                    
		color = pObj->m_sel_color;
	}

	pDevice->SetTextAlign(pSrez->m_align);

	MImage *image = pSrez->GetImage();
	if (!image)
	{
		image = new MImage();
		pObj->GetKarta()->GetDictImageValue(pSrez->m_imagekod, *image);
		pSrez->SetImage(image);
	}
	pDevice->DrawImage(image, color, pta->GetAt(0));	
}

//=======================================================================
//! Drawing of font dot object
void MDrawing::DrawNastrShriftSrez(MTopobject* pObj,MPlotDevice* pDevice,MNastrShriftSrez* pSrez,MPtArray* pta)
//=======================================================================
{
	MSymbol* sym = &pSrez->m_tochka;

	if(!sym->visible) return;

	int m_nPoints=pta->GetSize();
    //if the array of points empty, we draw nothing
	if(m_nPoints==0) return;

	pDevice->SetBkMode(sym->bkmode);

	pDevice->SetTextAlign(sym->align);//UT_TA_BASELINE|UT_TA_CENTERSbStretch->symbol->align);//TA_BASELINE);//TA_CENTER);//SbStretch->symbol->align);//TA_CENTER||TA_BASELINE);
    //receive color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)sym->colorkod, color))
		color.SetColor(0,0,0);
	MColor whiteclr;
 	pObj->GetKarta()->GetDictColorValue(1, whiteclr);
	if((pObj->bDrawColor)&&(color != whiteclr)) 
		color = pObj->mDrawColor;
	if((pObj->m_selected) &&(color != whiteclr))      
		color = pObj->m_sel_color;
 
	pDevice->SetTextColor(color);
  
    float value = sym->value;
	int iEscape = sym->logfont.lfEscapement;

	if(pSrez->IsPereoprIndex(IDT_NAS_SHRIFT_SYM_OPISANIE))
	{
		MString str = pObj->GetDataPole(pSrez->GetPerIndex(IDT_NAS_SHRIFT_SYM_OPISANIE));
		MString strtrue;
		MZnak* pZnak = pObj->GetKarta()->m_pKls->GetZnak(str, strtrue, true);
		if((pZnak)&&(pZnak->GetYaz()->GetType() == F_DOT))
		{
			MSzArray* szArr = ((MDotYaz*)(pZnak->GetYaz())->m_object)->m_tochka;
			MShriftSrez* pSrez1 = (MShriftSrez*)szArr->GetTail();
			sym = &pSrez1->m_tochka;
		}
	}

    //prepare a font
	M_LOG_FONT lfont = sym->logfont;

	//pDevice->SetBkMode(sym->bkmode);
	//pDevice->SetTextAlign(sym->align);
    //receive color of the text on codes in the dictionary of colours
	//MColor color;
    ////if in the dictionary there was no color, do the text black
	//if(!pObj->GetKarta()->GetDictColorValue((unsigned short)sym->colorkod, color))
	//	color.SetColor(0,0,0);
	//if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
	//{
	//	color = pObj->m_sel_color;
	//}
	//pDevice->SetTextColor(color);
 // 
	actRatio= value / etalon_value;//sym->value / etalon_value;
	drawTextFlag= false;   // vipa 2004.11.05
	if(sym->scaled)  {
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,value);//sym->value);
		drawTextFlag= true;   // vipa 2004.11.05
	}
	else
		lfont.lfHeight = pDevice->LogicToBum(value);//sym->value);

	

    //If the sizes or an angle of rotation of a symbol are redefined
	double dTemp;
	if(pSrez->IsPereoprIndex(IDT_NAS_SHRIFT_VAL_SYM_UGOL)){
		if(pObj->GetDoubleDataPole(pSrez->GetPerIndex(IDT_NAS_SHRIFT_VAL_SYM_UGOL),dTemp))
			iEscape = (int)(dTemp*10);//lfont.lfEscapement = (int)(dTemp*10);
	}

	if(pSrez->IsPereoprIndex(IDT_NAS_SHRIFT_VAL_SYM_VAL))
		if(pObj->GetDoubleDataPole(pSrez->GetPerIndex(IDT_NAS_SHRIFT_VAL_SYM_VAL),dTemp))			
			lfont.lfHeight= dTemp;

	if(pObj->m_excited) lfont.lfHeight *= 2;

    //If the size of a letter is less than zero
	if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_POINT))
		{ drawTextFlag= false;   return; }     // vipa 2004.11.05

/*
					MDotYaz *m_DotYaz = (MDotYaz *)(pObj->m_pZnak->GetYaz()->m_object);
					for(int k=0;k<m_DotYaz->m_tochka->GetSize();k++)
					{
//						if(((MSrez *)m_DotYaz->m_tochka->GetAt(k))->GetType()==IDT_NAS_SHRIFT)
//						{
							unsigned short  AngleIndex = ((MNastrShriftSrez *)m_DotYaz->m_tochka->GetAt(k))->GetPerIndex(0);
							if(AngleIndex!=-1)
							{
								MString WriteStr;
								WriteStr.Format("%f",-angle);
								pTpo->SetDataPole(WriteStr,AngleIndex);
							}
//						}
					}
//
if (m_nPoints==1)   {
					MDotYaz *m_DotYaz = (MDotYaz *)(pObj->m_pZnak->GetYaz()->m_object);
							unsigned short  AngleIndex = ((MNastrShriftSrez *)m_DotYaz->m_tochka->GetAt(0))->GetPerIndex(0);
							if(AngleIndex!=-1)
							{
								MString WriteStr =	pObj->GetDataPole(AngleIndex);
//                  lfont.lfEscapement= atoi(WriteStr.c_str())*10;
									}

}  
//  cout <<"m_nPoints= "<< m_nPoints << endl;
*/
//                MString str= (const char*)QString::number((int)angle);            // 2005-03-15 vipa
/*
                MString pole= "пУОПЧОБС";
                koi2win(pole);
                MString str=  pObj->GetDataPoleByName(pole);
                if ( strlen(str.c_str()) ) {
                  int angle= atoi(str.c_str());
                  lfont.lfEscapement= 10 * angle;
                }
*/      
                
// presumption of 2 points:   vector -> dot
// lfont.lfEscapement =-1 -> first point  with no rotation
// lfont.lfEscapement =-2 -> second point with no rotation
// otherwise              -> first point  with rotation
	pDevice->SaveFont();                        // 2005-03-30 vipa
	if (m_nPoints==2) {
//  cout << "N m_nPoints=" <<m_nPoints<<endl;
	double angle = GetVektorGrad(pta->GetAt(0),pta->GetAt(1) )*10;
	if ((iEscape = -2)&&(iEscape != -1))
     iEscape  -= (int)dbl_to_int(angle);

	lfont.lfEscapement = iEscape;
	/*if ((lfont.lfEscapement != -2)&&(lfont.lfEscapement != -1))
     lfont.lfEscapement  -= (int)dbl_to_int(angle);*/
	pDevice->SetFont(&lfont);
	if (lfont.lfEscapement==-2)
  		pDevice->TextOut(pta->GetAt(1)->x,pta->GetAt(1)->y,sym->str);
	else
  		pDevice->TextOut(pta->GetAt(0)->x,pta->GetAt(0)->y,sym->str);
	}
	else  {
		pDevice->SetFont(&lfont);
		for(int i=0;i<m_nPoints;i++)
			pDevice->TextOut(pta->GetAt(i)->x,pta->GetAt(i)->y,sym->str);
	}
	
	//pDevice->Ellipse(pta->GetAt(0)->x-10, pta->GetAt(0)->y-10, 20, 20);
    //restore
	pDevice->RestoreFont();
}

//==================================
//! Drawing of a formula: uses the GetStrofaDlina and DrawFragment functions
void MDrawing::DrawFormula(MTopobject* pObj,MPlotDevice* pDevice, MPtArray* pta, MFormula* formula)
//==================================
{

//FOR CORRECT DRAWING OF SINGLE-POINT FORMULAS WE INTRODUCE THE FOLLOWING RESTRICTION
    if(pta->GetSize()==1){
		MPoint temppoint = *pta->GetAt(0);
		pta->AddTail(&temppoint);
	}
  
    //length of a vector of a binding (geo)
	double geo_priv = DLINASG(pta->GetAt(0),pta->GetAt(1));
  //text angle of rotation in degrees (geo)
//	double angle =
  GetVektorGrad(pta->GetAt(0), pta->GetAt(1));
//	double radangle = (angle/180)*3.1415;
    //quantity of fragments in a formula
	int Size = ((MFrArray*)formula->fragm_arr)->GetSize();
    //length of the text of all formula (geo)
	double geo_text = GetFormulaDlina(pObj, pDevice, formula)/1.05;

	if(!pDevice->Distinguish_Text(geo_text)) return;//tempMax

    //distance between fragments (geo)
	double geo_probel = 0 ;
    //if length of the text is more than length of a binding vector,
    //and that fragments didn't fit at each other,
       //we remove the minimum 'probel=0.5' (it is received experimentally)
	if (Size != 1)
	{
		if ((geo_priv - geo_text) <= 0) geo_probel=0.05*geo_text/(Size-1);
		else geo_probel = (geo_priv - geo_text)/(Size-1);
	}
 
    //we consider each fragment
    //parameter of shift of a fragment
	double max_geo = 0;//(geo)
    //consider each fragment
 	for (int i=0; i<Size; i++) 
	{	
        //For drawing of each fragment before a call of functions
        //We establish to 'DrawFragment' coordinates
        //a point with which drawing begins.
		
        //we receive the i-th fragment
		MFragment* fragment = ((MFrArray*)formula->fragm_arr)->GetAt(i);
		MPtArray* geo_pta = new MPtArray();
		geo_pta->SetSize(2);
        //for correct drawing of a formula: an text we have "the head up"
/*		if (((900<(angle*10))&&((angle*10)<2700)))

		{
            //that the text was attached at the left,
   //we interchange the position of binding points
			(geo_pta->GetAt(0))->x = (pta->GetAt(1))->x - max_geo;
			(geo_pta->GetAt(0))->y = (pta->GetAt(1))->y;
			(geo_pta->GetAt(1))->x = (pta->GetAt(0))->x - max_geo;
			(geo_pta->GetAt(1))->y = (pta->GetAt(0))->y;
			
		}
		else 
		{*/
			(geo_pta->GetAt(0))->x = (pta->GetAt(0))->x + max_geo;
			(geo_pta->GetAt(0))->y = (pta->GetAt(0))->y;
			(geo_pta->GetAt(1))->x = (pta->GetAt(1))->x + max_geo;
			(geo_pta->GetAt(1))->y = (pta->GetAt(1))->y;
//		}
////////////////////////////////////////////////////////////////////////////  
//for dot objects which size more 'textsizeMax' we redefine binding points,
        //only for dot objects
if (drawActFlag) 
if (pObj->m_pZnak) 
	if(pObj->m_pZnak->GetYaz())
		if (pObj->m_pZnak->GetYaz()->GetType() == F_DOT)	
		{
		MSzArray* szar=((MDotYaz*)(pObj->m_pZnak->GetYaz()->m_object))->m_tochka;
		MShriftSrez* srez = (MShriftSrez*)szar->GetAt(0);
		MSymbol *sym = &srez->m_tochka;

		M_LOG_FONT lfont = sym->logfont;
		if(sym->scaled)
			lfont.lfHeight =(*ConvertSizeToReal)(pObj,sym->value);
		else
			lfont.lfHeight = pDevice->LogicToBum(sym->value);

    /*	int pointsize = int(pDevice->BumToLogic(lfont.lfHeight)*SCREEN_FONT_KOEFF);

		if (textRatio*pointsize > textSizeMax)
			{
				double privScale = textSizeMax/(textRatio*pointsize);
				for (int i=0; i< geo_pta->Size;i++)
				{
				double geoPriv = DLINASG(pObj->GetPoint(0),pta->GetAt(i));
                double privGrad = GetVektorGrad(pObj->GetPointArray()->GetAt(0),pta->GetAt(i));


				MPoint pt;
				double dX =  geoPriv*privScale*cos(privGrad*pi/180.0);
				double dY =  geoPriv*privScale*sin(privGrad*pi/180.0);
				pt.x = pObj->GetPoint(0).x + dX;
				pt.y = pObj->GetPoint(0).y + dY;
				geo_pta->SetAt(i,&pt);
				}
			
            }*/

		}
////////////////////////////////////////////////////////////////////////////

		switch (fragment->GetType()) {
		case ID_FRAG_STROFA:
			{
				DrawFragment(pObj, pDevice, geo_pta, fragment);
				max_geo += GetStrofaDlina(pObj, pDevice, (MStrofa*)fragment->GetFragment()) + geo_probel;
			}
			break;
            //if fragment = fraction
		case ID_FRAG_FRACTION:
			{
                //length of numerator (geo)
				double dlina_chisl = GetStrofaDlina(pObj, pDevice, ((MFraction*)fragment->GetFragment())->GetChisl());
                //length of a denominator (geo)
				double dlina_znam = GetStrofaDlina(pObj, pDevice, ((MFraction*)fragment->GetFragment())->GetZnam());
				DrawFragment(pObj, pDevice, geo_pta, fragment);
                //fragment length
				if (dlina_chisl <= dlina_znam)
					max_geo += dlina_znam+geo_probel;
				else  max_geo += dlina_chisl+geo_probel;					
			}
			break;
		}
		delete geo_pta;						
		  }
}

//==================================
//! Drawing of a formula on a spline: uses the DrawPredlogenieBySpline function
void MDrawing::DrawFormulaSpline(MTopobject* pObj, MPlotDevice* pDevice, MPtArray* pta, MFormula* formula)
//==================================
{
	if((!formula)||(!pta)||(pta->GetSize()!=4))
		return;
    //in case of a spline binding we check that the formula consisted of one
    //a fragment like ID_FRAG_STROFA, and the stanza consisted of one sentence
	bool par = true;
	MFragment* fragment =NULL;

	MStrofa* strofa =NULL;
	if(formula->fragm_arr->GetSize()!=1)
		par = false;
	else{
		fragment = formula->fragm_arr->GetAt(0);
		if(fragment->GetType()!=ID_FRAG_STROFA)
			par = false;
		else{
			strofa = (MStrofa*)fragment->GetFragment();
			if(strofa->GetPredlogenieNum()!=1)
				par = false;
			else if(strofa->GetType()!=ID_STROFA_SIMPLE)
				par = false;
				
		}
	}
	if(!par){
		InternalMessage("Too difficult formula (ignored) !");
		return;
	}
	unsigned char nomprefix, nompostfix, nomprk;
	unsigned short nompole;
	nompole = strofa->GetNomPole(0);
	nomprefix = strofa->GetNomPrefix(0);
	nompostfix = strofa->GetNomPostfix(0);
	nomprk = strofa->GetNomPrk(0);
	MStArray* strofy = new MStArray();
	pObj->ObtainMassivStrok(nompole, strofy);
	MString str = *strofy->GetAt(0);
	MPrk drawprk;
    //from the dictionary of styles on an index we will receive style if it is possible
	if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk))
		return;
	MString prefix, postfix;
    //from the dictionary of boundary lines on an index we will receive a boundary line if it is possible
    if(!pObj->GetKarta()->GetDictMegstrValue((unsigned short)nomprefix, prefix))
		return;
    //from the dictionary of boundary lines on an index we will receive a boundary line if it is possible
	if(!pObj->GetKarta()->GetDictMegstrValue((unsigned short)nompostfix, postfix))
		return;

    //we will draw the sentence on a spline
	DrawPredlogenieBySpline(pObj, pDevice, *pta->GetAt(0), *pta->GetAt(1), *pta->GetAt(2), *pta->GetAt(3), 
							 str, prefix, postfix, &drawprk);
	
}


//=========================================================================================
//!function of obtaining length of a formula: uses the 'GetStrofaDlina' function
double	MDrawing::GetFormulaDlina(MTopobject* pObj,MPlotDevice* pDevice, MFormula* formula)
//=========================================================================================
{
        //the returned size
		double geo_dlina = 0.0;
        //quantity of fragments in a formula
		int Size = ((MFrArray*)formula->fragm_arr)->GetSize();
		for (int i=0; i<Size; i++) 
		{
            //receive the i-th fragment
			MFragment* fragment = ((MFrArray*)formula->fragm_arr)->GetAt(i);
			switch (fragment->GetType()) {
			case ID_FRAG_STROFA:
				{
					geo_dlina += GetStrofaDlina(pObj, pDevice, ((MStrofa*)fragment->GetFragment()));
				}
				break;
			case ID_FRAG_FRACTION:
				{
                    //numerator length
					double geo_chisl = GetStrofaDlina(pObj, pDevice, ((MFraction*)fragment->GetFragment())->GetChisl());
                    //denominator length
					double geo_znam = GetStrofaDlina(pObj, pDevice, ((MFraction*)fragment->GetFragment())->GetZnam());
                    //receive length of all fraction
					if (geo_chisl <= geo_znam)	geo_dlina += geo_znam;
						else  geo_dlina += geo_chisl;					
				}
				break;
			}//End of switch{}
		}//End of for{MFrArray}
        //we will add distance for "spaces"
        //between fragments for initial initialization
        //(it is received experimentally)
		geo_dlina = 1.05*geo_dlina;
		return geo_dlina;
}


//========================================================
//!uses the ObtainMassivStrok and GetPredlogenieDlina functions
double  MDrawing::GetStrofaDlina(MTopobject* pObj,MPlotDevice* pDevice, MStrofa* strofa)
//========================================================
{
    //the returned size
	double geo_dlina = 0.0; 
    //we declare the new array of lines
    //also we copy in it lines
	MStArray* strofy = new MStArray();
    //cycle according to sentences
	for(int i=0; i<strofa->GetPredlogenieNum(); i++)
	{
		strofy->ClearAll();
		MString strofa_name;

        //we will receive an index (code) of the name of the characteristic of the sentence with an index 'i'
        //in an array of sentences of a stanza
		unsigned short nompolename = strofa->GetNomPole(i);
		pObj->ObtainMassivStrok(nompolename, strofy);
		MPrk drawprk;
        //we will receive an index (code) of style of the sentence with an index 'i' in an array of sentences of a stanza
		unsigned char nomprk = strofa->GetNomPrk(i);
        //from the dictionary of styles on an index we will receive style if it is possible
		if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk))
			continue;

        //receive boundary lines
		MString pre_mega, post_mega;
        //receive an index (code) of a prefix of the sentence with an index 'i'
        //in an array of sentences of a stanza
		unsigned char nomMegstr = strofa->GetNomPrefix(i);
        //from the dictionary of boundary lines on an index we will receive a boundary line if it is possible
		if(!pObj->GetKarta()->GetDictMegstrValue((unsigned short)nomMegstr, pre_mega))
			continue;
        //we will receive an index (code) of a postfix of the sentence with an index 'i'
        //in an array of sentences of a stanza
		nomMegstr = strofa->GetNomPostfix(i);
        //from the dictionary of boundary lines on an index we will receive a boundary line if it is possible
        if(!pObj->GetKarta()->GetDictMegstrValue((unsigned short)nomMegstr, post_mega))
			continue;
        //we will receive numerator length depending on type
		int type_strofa = strofa->GetType();
        //we receive length in geo-coordinates
		geo_dlina += GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk);
	}
	delete strofy;
	return geo_dlina;
}


//==============================================
//! ---------
/**type - stanza type
//sta - array of lines
//pre_mega, post_mega - boundary lines
//p_prk - style
*/
double MDrawing::GetPredlogenieDlina(MTopobject* pObj,MPlotDevice* pDevice, 
		int type, MStArray* sta, MString* pre_mega, MString* post_mega, MPrk* p_prk)
//==============================================
{
	double dlina = 0.0;    
    //shirina = length of line
	double shirina = 0.0;
    //copy style
	M_LOG_FONT lfont = p_prk->logfont;

    //create the necessary font with characteristics
	if(p_prk->scaled)
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,p_prk->value);
	else
		lfont.lfHeight = pDevice->LogicToBum(p_prk->value);
    
    if(pObj->m_pLay->m_bOperLayer)
	{
		  actRatio= p_prk->value / etalon_value;
		  if(p_prk->scaled)
			{
				lfont.lfHeight = SizeToReal(pObj,p_prk->value);
				double BumSizeMax = pDevice->LogicToBum(textSizeMax);
				double koef = etalon_value/p_prk->value;
				if((lfont.lfHeight*koef) > BumSizeMax) lfont.lfHeight = BumSizeMax/koef;
			}
			else lfont.lfHeight = pDevice->LogicToBum(p_prk->value);
	}
    
    
	pDevice->SetFont(&lfont);
    //quantity of elements of the array of lines
	int kolvo = sta->GetSize();
    //an auxiliary variable for drawing of an text
	MString str = "";
    //boundary lines
	MString pre_stroka = *pre_mega;
	MString post_stroka = *post_mega;
	for (int j=0; j<kolvo; j++)
	{
 	 	MString stroka = sta->GetAt(j)[0];
		str = pre_stroka + stroka + post_stroka;
		if ((type == ID_STROFA_STOLBEC)||(type == ID_STROFA_SIMPLE))
		{
                //receive length of a line
				shirina = pDevice->GetTextExtent(str);
                //allocate the maximum length
				if (dlina <= shirina) dlina = shirina;
		}
		else if ((type == ID_STROFA_STROKA)||(type == ID_STROFA_COMPLEX))
		{
            //receive width of a line
            shirina = pDevice->GetTextExtent(str);
			 dlina += shirina;
		}
	}//EndOfFor
    //choose an old font
	return dlina;
}


//========================================================
//!function of drawing of a fragment: uses functions
void  MDrawing::DrawFragment(MTopobject* pObj, MPlotDevice* pDevice, MPtArray* pta, MFragment* fragment)
//========================================================
{
  
    //text angle of rotation in degrees (geo)
	double angle = GetVektorGrad(pta->GetAt(0), pta->GetAt(1));
    //shift of a stanza across at alignment
	double otstup_geo = 0;
			
    if((pObj->m_pLay->m_bOperLayer)&&(pObj->m_status))
		otstup_geo += pDevice->LogicToBum(5);

	for (int j=0; j<fragment->GetStrofNum(); j++)
	{
        //stanza length
		double max_geo = 0;
		MStrofa* strofa =NULL;
        //"stanza" = one stanza, "fraction" = two stanzas
		if (fragment->GetStrofNum() == 1) 
			strofa = (MStrofa*)fragment->GetFragment();		
		if (fragment->GetStrofNum() == 2)
		{
            //receive indexes on numerator,
            //and then - on a denominator
			if (j == 0) strofa = ((MFraction*)fragment->GetFragment())->GetChisl();
			if (j == 1) strofa = ((MFraction*)fragment->GetFragment())->GetZnam();
		}
        //declare the new array of lines
        //also we copy in it lines
		MStArray* strofy = new MStArray();
    //receive parameters of shift of stanzas at alignment
	for(int i=0; i<strofa->GetPredlogenieNum(); i++)
	{
		strofy->SetSize(0);
		MString strofa_name;
        //we will receive an index (code) of the name of the characteristic of the sentence with an index 'i'
        //in an array of sentences of a stanza
		unsigned short nompolename = strofa->GetNomPole(i);
		pObj->ObtainMassivStrok(nompolename, strofy);
		MPrk drawprk;
        //we will receive an index (code) of style of the sentence with an index 'i' in an array of sentences of a stanza
		unsigned char nomprk = strofa->GetNomPrk(i);
        //from the dictionary of styles on an index we will receive style if it is possible
//		if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk)) ;
    if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk))			continue;
        //we will receive a point of drawing of a fragment
		MPoint geo_point = pta->GetAt(0);
//		double radangle = (angle/180)*3.1415;

		M_LOG_FONT lfont = drawprk.logfont;

        //we will establish a tilt angle: for drawing of an text "up the head"
        //we do a angle from-90 to 90 degrees
//		if (((900<(angle*10))&&((angle*10)<2700)))

//			lfont.lfEscapement = (int)(1800 - angle*10);			
//		else
			lfont.lfEscapement = (int)(3600 - angle*10);
		
        //For a correctness
		drawprk.logfont.lfEscapement = lfont.lfEscapement;

        //we establish style height
		if(drawprk.scaled)
		{
			lfont.lfHeight = SizeToReal(pObj,drawprk.value);
			if(pObj->m_pLay->m_bOperLayer) 
			{
				double logSizeMax = pDevice->LogicToBum(textSizeMax);
				double koef = etalon_value/drawprk.value;

				if((lfont.lfHeight*koef) > logSizeMax) lfont.lfHeight = logSizeMax/koef;
			}
		}
		else
			lfont.lfHeight = pDevice->LogicToBum(drawprk.value);
      
//    cout << drawprk.value<< endl;
		if(!pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_FORMULA)){
			delete strofy;
			return;
		}

        //we will receive boundary lines
		MString pre_mega, post_mega;
        //we will receive an index (code) of a prefix of the sentence with an index 'i'
        //in an array of sentences of a stanza
		unsigned char nomMegstr = strofa->GetNomPrefix(i);
        //from the dictionary of boundary lines on an index we will receive a boundary line if it is possible
		if(!pObj->GetKarta()->GetDictMegstrValue((unsigned short)nomMegstr, pre_mega))
			continue;
        //we will receive an index (code) of a postfix of the sentence with an index 'i'
        //in an array of sentences of a stanza
		nomMegstr = strofa->GetNomPostfix(i);
        //from the dictionary of boundary lines on an index we will receive a boundary line if it is possible
		if(!pObj->GetKarta()->GetDictMegstrValue((unsigned short)nomMegstr, post_mega))
			continue;
		int type_strofa = strofa->GetType();
				
		int kolvo = strofy->GetSize(); //cout << "kolvo= "<< kolvo <<endl;
		/*if(pObj->m_pLay->m_bOperLayer) //MAK addOn 2006.12.20 
		{
			type_strofa = ID_STROFA_COMPLEX;//;
			MString str = "PPPPP";
			strofy->AddTail(&str);
			str = "yyyyy";
			strofy->AddTail(&str);
			str = "ggggggggg";
			strofy->AddTail(&str);
			kolvo = strofy->GetSize();
		}*/
        //shift of a stanza down at alignment
		double sdvig = 0;
		switch (fragment->GetType()){
		case ID_FRAG_STROFA:
			{
                //shift across
				if ((drawprk.align & UT_TA_CENTER) == UT_TA_CENTER)
					otstup_geo = 0;
				else
					if ((drawprk.align & UT_TA_RIGHT) == UT_TA_RIGHT)
						otstup_geo = 0 - GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk)/2;
					else
						if ((drawprk.align & UT_TA_LEFT) == UT_TA_LEFT)	
						otstup_geo = GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk)/2;
				
				
				sdvig = lfont.lfHeight;
                //shift down
				if ((drawprk.align & UT_TA_BASELINE) == UT_TA_BASELINE)
					sdvig = 0;
				else
					if ((drawprk.align & UT_TA_BOTTOM) == UT_TA_BOTTOM)
						sdvig = -sdvig;
					else
					{
						if ((drawprk.align & UT_TA_TOP) == UT_TA_TOP)
						{
							if ((type_strofa == ID_STROFA_STROKA)||(type_strofa == ID_STROFA_COMPLEX))
									sdvig = sdvig/2 + sdvig/5;
							if ((type_strofa == ID_STROFA_SIMPLE)||(type_strofa == ID_STROFA_STOLBEC))
								sdvig = kolvo*sdvig/2 + sdvig/5;
						}	
					}
			}
		break;
		case ID_FRAG_FRACTION:
			{
                //choose a stanza - numerator or a denominator -
                //which we will level
                //THE CHOICE OF THAT IS MORE, NUMERATOR OR THE DENOMINATOR

				int vybor;
                //numerator length
				double geo_chisl_temp = GetStrofaDlina(pObj, pDevice, ((MFraction*)fragment->GetFragment())->GetChisl());
                //denominator length
				double geo_znam_temp = GetStrofaDlina(pObj, pDevice, ((MFraction*)fragment->GetFragment())->GetZnam());
                // receive a stanza with the minimum length
				if (geo_chisl_temp <= geo_znam_temp) vybor = 0;
				  else vybor = 1;

                //shift of numerator concerning a denominator and vice versa
                //DIFFERENCE OF LENGTHS OF NUMERATOR AND DENOMINATOR
				double max_drob;
                //receive 'max_drob' space length
				if (geo_chisl_temp<= geo_znam_temp) 	max_drob = geo_znam_temp - geo_chisl_temp;
					else 	max_drob = geo_chisl_temp - geo_znam_temp;


                //shift across
				if ((drawprk.align & UT_TA_CENTER) == UT_TA_CENTER)
				{
					//otstup_geo = GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk)/2;
					otstup_geo = 0;
					if (vybor == j)  otstup_geo += max_drob/2;
				}
				else
					if ((drawprk.align & UT_TA_RIGHT) == UT_TA_RIGHT)
					{
						//otstup_geo = GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk);
						otstup_geo = 0 - GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk)/2;
                        //WE SHIFT THAT IS LESS
						if (vybor == j)  otstup_geo += max_drob;
					}
					else
						if ((drawprk.align & UT_TA_LEFT) == UT_TA_LEFT)	
							//otstup_geo = 0;
							otstup_geo = GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk)/2;
				
				sdvig = lfont.lfHeight;
                //shift down numerator
				if ((type_strofa == ID_STROFA_STROKA)||(type_strofa == ID_STROFA_COMPLEX))
						sdvig = sdvig/2 + sdvig/5;
				if ((type_strofa == ID_STROFA_SIMPLE)||(type_strofa == ID_STROFA_STOLBEC))
						sdvig = kolvo*sdvig/2 + sdvig/5;
                //shift down denominator
				if (strofa->IsEqualTo(((MFraction*)fragment->GetFragment())->GetZnam())) sdvig = -sdvig;
			}
			break;
		}
    
		double sdvig_geo = sdvig;
		MPoint point;
        //we will receive a point of drawing of the sentence
		
		point.x = geo_point.x + (max_geo + otstup_geo);
		point.y = geo_point.y + sdvig_geo;

        //drawing of the sentence
		DrawPredlogenie(pObj, pDevice, type_strofa, point, strofy, &pre_mega, &post_mega, &drawprk);
		max_geo += GetPredlogenieDlina(pObj, pDevice, type_strofa, strofy, &pre_mega, &post_mega, &drawprk);
        //drawing of line of fraction
		if (fragment->GetType() == ID_FRAG_FRACTION)
		{
            //we copy style of a denominator
			MPrk line_prk_znam;
            //we will receive an index (code) of style of the sentence with an index 0 in an array of sentences of a stanza
			unsigned char nomprk = (((MFraction*)fragment->GetFragment())->GetZnam())->GetNomPrk(0);
            //from the dictionary of styles on an index we will receive style if it is possible
			if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, line_prk_znam))
				continue;

            //the line with the necessary parameters
			MLinia linia = MLinia(0.1f, drawprk.colorkod);
            //second point of line of fraction
			MPoint point_priv = MPoint(geo_point.x + max_geo, geo_point.y);
			MPtArray* pt_array = new MPtArray();
			pt_array->SetSize(2);
			pt_array->SetAt(0, &geo_point);
			pt_array->SetAt(1, &point_priv);
            //draw a line
			DrawLinia(pObj,pDevice,&linia,pt_array);
			delete pt_array;
		}
    
	 }   // i
	 delete strofy;
   
   }   // j
		
}


//==============================================
//! ------
void MDrawing::DrawPredlogenie(MTopobject* pObj,MPlotDevice* pDevice, int type, 
	MPoint geo_pt, MStArray* sta, MString* pre_mega, MString* post_mega, MPrk* p_prk)
//type - stanza type
//geo_pt - geo-coordinates of the beginning of drawing of a stanza
//sta - array of lines

//pre_mega, post_mega - boundary lines
//p_prk - style
//==============================================
{
	MPoint pt = geo_pt;
	M_LOG_FONT lfont = p_prk->logfont;
    //we establish style parameters:
	pDevice->SetBkMode(p_prk->bkmode);
    //we will receive color of a background of the text and color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do a background white
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)p_prk->bkcolorkod, color))
		color.SetColor(255,255,255);
	if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}
	pDevice->SetBkColor(color);
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)p_prk->colorkod, color))
		color.SetColor(0,0,0);

	if(pObj->m_selected)
    {//If the object is chosen that we change color
		color = pObj->m_sel_color;
	}
	
	pDevice->SetTextColor(color);
    //alignment: by default we establish UT_TA_BASELINE
	unsigned int m_uiNewAlign = p_prk->align;
	m_uiNewAlign&=~UT_TA_TOP;
	m_uiNewAlign&=~UT_TA_BASELINE;
	m_uiNewAlign&=~UT_TA_BOTTOM;
	m_uiNewAlign|=UT_TA_BASELINE;
	pDevice->SetTextAlign(m_uiNewAlign);
//	double angle = 3600 - lfont.lfEscapement;
    //we transfer an angle to radians
//	double radangle = (angle/3600)*2*3.1415;
	if (!p_prk->visible) 
		return;
    //we create the necessary font with characteristics of m_prk.logfont

    //establish style height
  actRatio= p_prk->value / etalon_value;
  drawTextFlag= false;
  if(p_prk->scaled)
	{
		lfont.lfHeight = SizeToReal(pObj,p_prk->value);
		if(pObj->m_pLay->m_bOperLayer)
		{
			double BumSizeMax = pDevice->LogicToBum(textSizeMax);
			double koef = etalon_value/p_prk->value;

			if((lfont.lfHeight*koef) > BumSizeMax) lfont.lfHeight = BumSizeMax/koef;
		}
	   
	   drawTextFlag= true;
	}
	else
		lfont.lfHeight = pDevice->LogicToBum(p_prk->value);
    
//  if (pObj->m_bounds.top ==  pObj->m_bounds.bottom)
//    pObj->m_bounds.top=  pObj->m_bounds.bottom + lfont.lfHeight;  // vipa 2004.10.26  to work CheckTextSize for objText

  if( !pDevice->Distinguish_Text(((double)lfont.lfHeight)/VISIBLE_POINT))
	  { drawTextFlag= false;  return;  }
    
	pDevice->SaveFont();
	pDevice->SetFont(&lfont);
    //quantity of elements of the array of lines
	int kolvo = sta->GetSize();
//  if (kolvo==0)  kolvo=1;
    //shift parameter down: for a column and a simple stanza
	int k = 0; 
    //auxiliary variable for drawing of an text
	MString str = "";
    //boundary lines
	MString pre_stroka = *pre_mega;
	MString post_stroka = *post_mega;
    //for a conclusion of a column and a simple stanza symmetrically down
	if ((type == ID_STROFA_SIMPLE)||(type == ID_STROFA_STOLBEC))
	{
	//	pt.y += ((kolvo-2)*lfont.lfHeight/2); //juli
	}
				
    //for a line and difficult stanza: we "exhaust" all lines in 'str'
    //for a column and a simple stanza: we draw every line
	for (int j=0; j<kolvo; j++)
	{
        //we receive contents a 'j'st line:
        //we attach boundary lines to 'stroka'
		//MString stroka = *sta->GetAt(j);
//    if (sta->GetAt(j))
      str += pre_stroka + *sta->GetAt(j) + post_stroka;
//    else break;
        //if a stanza like column or "simple", we draw
      //a line, previously shifting down
		if ((type == ID_STROFA_SIMPLE)||(type == ID_STROFA_STOLBEC))
		{
            //shift down
			k = (int)((double)j * lfont.lfHeight);
            //drawing of an text
			if(pObj->m_pLay->m_bOperLayer)
				pDevice->TextOut(pt.x, pt.y - k, str, str.GetLength());
			else
				pDevice->TextOutWindows(pt.x, pt.y - k, str, str.GetLength());
            //we "clean" 'str' then to receive only the current line
			str = "";
		}
		
	}//EndOfFor
//cout << kolvo<< "  " <<str<< endl;  
    //drawing of a line and difficult stanza
	if ((type == ID_STROFA_STROKA)||(type == ID_STROFA_COMPLEX))
    {
		if(pObj->m_pLay->m_bOperLayer)
        {
			pDevice->TextOut(pt.x, pt.y - lfont.lfHeight/2, str, str.GetLength());
        }
		else
        {
			pDevice->TextOutWindows(pt.x, pt.y /*- lfont.lfHeight/2*/, str, str.GetLength());
        }
    }
    //we choose an old font
	pDevice->RestoreFont();	
	
}

//==============================================
//! drawing of the sentence along a spline
//params (MTopobject* pObj, MPlotDevice* pDevice, MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, 
//		 MString str, MString prefix, MString postfix, MPrk* p_prk)
void MDrawing::DrawPredlogenieBySpline(MTopobject* pObj, MPlotDevice* pDevice, MPoint pt1, MPoint pt2, MPoint pt3, MPoint pt4, 
							 MString str, MString , MString , MPrk* p_prk)
//==============================================
{
    //forms the broken line from 10 points corresponding to the spline determined by four
    //entrance points;
	MPtArray* ptar = new MPtArray();
	FormLomlineBySimpleSpline(pt1, pt2, pt3, pt4, 10, ptar);

    //on the array of tops of the broken 'ptar' line along which we have a line, and the line 'str'
    //we form the array of points of an arrangement of letters of a line, which, generally at discrepancy
    //numbers of letters and number of tops of a broken line, doesn't coincide with the array of tops of a broken line;
	FindPointsForKrivNadpis(ptar, str);

    //calculate the array of angles of rotation of letters
	MInArray* angles = new MInArray();
	FindAnglesForKrivNadpis(ptar,  angles);

    //number of letters
	int m_nPoints = ptar->GetSize();

    //we establish style parameters:
	pDevice->SetBkMode(p_prk->bkmode);
    //we will receive color of a background of the text and color of the text on codes in the dictionary of colours
	MColor color;
    //if in the dictionary there was no color, do a background white
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)p_prk->bkcolorkod, color))
		color.SetColor(255,255,255);
	if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
	{
		color = pObj->m_sel_color;
	}
	pDevice->SetBkColor(color);
    //if in the dictionary there was no color, do the text black
	if(!pObj->GetKarta()->GetDictColorValue((unsigned short)p_prk->colorkod, color))
		color.SetColor(0,0,0);
	pDevice->SetTextColor(color);
    //alignment
	unsigned int m_uiNewAlign = p_prk->align;
	pDevice->SetTextAlign(m_uiNewAlign);

	M_LOG_FONT lfont = p_prk->logfont;
//  actRatio= sym->value / etalon_value;
  drawTextFlag= false;
  if(p_prk->scaled)
	{
		lfont.lfHeight =(*ConvertSizeToReal)(pObj,p_prk->value);
    drawTextFlag= true;
/*  vipa  2004.10.25    
        //check on an inscription size exit for the set border
		int msize = textSizeMax;
		if(msize)
		{
			double max_size = pDevice->LogicToBum(msize);
			
			double etalon_size = 37.5;
			double text_size = p_prk->value;
			double koef = etalon_size/text_size;

			if((lfont.lfHeight*koef) > max_size) lfont.lfHeight = max_size/koef;
		}
*/    
	}
	else
		lfont.lfHeight = pDevice->LogicToBum(p_prk->value);

    //check discernability of a letter
	if(!pDevice->Distinguish_Text(lfont.lfHeight)) return;

	pDevice->SaveFont();

	MPoint pt;
	for(int i=0; i<m_nPoints; i++) {
		lfont.lfEscapement =  *angles->GetAt(i);
		pDevice->SetFont(&lfont);
		char bukva = str.GetAt(i);
		pt = *ptar->GetAt(i);
		if(pObj->m_pLay->m_bOperLayer)
			pDevice->TextOut( pt.x, pt.y, &bukva ,1);
		else
			pDevice->TextOutWindows( pt.x, pt.y, &bukva ,1);
	}

	delete ptar;
    //choose an old font
	pDevice->RestoreFont();
}



//==================================
//! Drawing of the text: uses functions
void MDrawing::DrawText(MTopobject* pObj,MPlotDevice* pDevice,MYaz* pYaz,MPvArray* privs)
//==================================
{
    double val = 0.0;
	if (ConvertSizeToReal!= NULL)
     val = (*ConvertSizeToReal)(pObj, fontValueLimit);

  if(!pDevice->Distinguish_Text(val)) return;
//
    //if the flag of drawing is set in 'false', we don't draw
	if(!pYaz->m_visible)return;
    //if there is no formula
	if(privs->GetSize()==0) return;

    //if in language there is no formula, don't draw
	if(((MFormArray*)pYaz->m_formula)->GetSize()==0)
		return;

	bool oldbDrawColor = pObj->bDrawColor;
	pObj->bDrawColor = false;

	for(int i=0; i<privs->GetSize(); i++){
        MPriv* priv = privs->GetAt(i);//the pointer on a formula vector-binding
        unsigned char formnomer = priv->GetFormNomer();//number of a formula
        MPtArray* pta = new MPtArray();//formula vector-binding coordinates
		priv->GetDrawArray(pta);

        //we receive a formula
		MFormula* formula = ((MFormArray*)pYaz->m_formula)->GetAt(formnomer);
		

        //drawing depending on type
		switch(formula->GetType()){
		case ID_FORMULA_SIMPLE:
			{
				if((priv->GetType()==SIMPLE_PRIVYAZKA)||(priv->GetType()==DOUBLE_PRIVYAZKA))
					DrawFormula(pObj, pDevice, pta, formula);
				else if(priv->GetType()==SPLINE_PRIVYAZKA)
					DrawFormulaSpline(pObj,  pDevice, pta, formula);
				else{
                    //drawing text as 'callout'
                    //the array of points for drawing of the line
					MPtArray* pt_array = new MPtArray();
					pt_array->SetSize(3);
					
                    //drawing text as 'callout'
					pt_array->SetAt(0, pta->GetAt(0));
					pt_array->SetAt(1, pta->GetAt(1));
					pt_array->SetAt(2, pta->GetAt(2));
                    //receive a stanza/denominator of the first fragment
					MFragment* m_pFragment = ((MFrArray*)formula->fragm_arr)->GetAt(0);
					MStrofa* m_pStrofa =NULL;
					switch (m_pFragment->GetType()) {
					case ID_FRAG_STROFA:
						{
							m_pStrofa = (MStrofa*)m_pFragment->GetFragment();
						}
						break;
					case ID_FRAG_FRACTION:
						{
							m_pStrofa = ((MFraction*)m_pFragment->GetFragment())->GetZnam();
						}
						break;
					}
					MPrk drawprk;
                    //we will receive an index (code) of style of the sentence with an index 0 in an array of sentences of a stanza
					unsigned char nomprk = m_pStrofa->GetNomPrk(0);
                    //from the dictionary of styles on an index we will receive style if it is possible
					if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk))
						return;
                    //line of the necessary parameters
                    //receive color of the text on codes in the dictionary of colours
					MColor color;
                    //if in the dictionary there was no color, do the text black
					if(!pObj->GetKarta()->GetDictColorValue((unsigned short)drawprk.colorkod, color))
						color.SetColor(0,0,0);
				if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
				{
					color = pObj->m_sel_color;
				}
					MLinia linia = MLinia(drawprk.value/10, color.GetColor());
                    //draw a line
					DrawLinia(pObj,pDevice,&linia,pt_array);

					delete pt_array;
                    //draw a formula
					MPtArray temp;
					temp.FastSet(pta->GetSize(), pta->GetAt(0));
					temp.DeleteHead();
					DrawFormula(pObj, pDevice, &temp, formula);
				}
			}
			break;

		case ID_FORMULA_OBRAML:
			{
				MPoint pt, pt1;
				pt = *pta->GetAt(0);
				pt1 = *pta->GetAt(1);
                //binding vector length
				double dlina_priv = DLINASG(pta->GetAt(0),pta->GetAt(1));
                //formula height over a binding vector
				double vysota_Up = 0;
				double verh = 0;
                //formula height under a binding vector
				double vysota_Down = 0;
				double niz = 0;
                //inscription angle of rotation in degrees
				double angle = GetVektorGrad(pta->GetAt(0), pta->GetAt(1));
                //quantity of fragments in a formula
				int Size = ((MFrArray*)formula->fragm_arr)->GetSize();
                //			 		 =to text length if length of a binding is less than length of the text of a formula,
                // formula length = {				              or only one fragment in a formula
                //				 	 =to binding vector length, in other cases
				double dTempFormulaDlina = GetFormulaDlina(pObj, pDevice, formula);
				double geo_dlina = 0;
				if (Size == 1) geo_dlina = dTempFormulaDlina;
				else geo_dlina = dlina_priv;
				if (dlina_priv <= dTempFormulaDlina)
					geo_dlina = dTempFormulaDlina;
				else geo_dlina = dlina_priv;
				for (int i=0; i<Size; i++) 
				{
                    //we receive the i-th fragment
					MFragment* fragment = ((MFrArray*)formula->fragm_arr)->GetAt(i);
					//geo
					double verh_vysota = GetFragmentVysotaUp(pObj, pDevice, fragment);
					if (verh < verh_vysota)	verh = verh_vysota;
					double niz_vysota = GetFragmentVysotaDown(pObj, pDevice, fragment);
					if (niz < niz_vysota)  niz = niz_vysota;							
				}//End of for{MFrArray}
//                double radangle = 0;
                //tilt angle: for drawing of an inscription "up the head"
                //we do a angle from-90 to 90 degrees and we will translate
                //the received angle in radians
				if (((900<(angle*10))&&((angle*10)<2700)))
				{
//                    radangle = ((1800 - angle*10)/3600)*2*3.1415;
                    //that the inscription was attached at the left,
                    //we interchange the position of binding points
					MPoint ptTemp = pt;
					pt = pt1;
					pt1 = ptTemp;
				}
//				else
//                    radangle = ((3600 - angle*10)/3600)*2*3.1415;
//                radangle+=3.14159265358979/2;
                //add distance at the left and on the right that letters always were in a rectangle
				double geo_dopolnenie = 0.01*geo_dlina;
				geo_dlina += 2*geo_dopolnenie;	
				double dlina_form = geo_dlina;
				double dopolnenie = geo_dopolnenie;
				vysota_Up = verh;
				vysota_Down = niz;
                //we will create a brush of color of the map
				pDevice->SaveZapolnenie();				
				MColor col = pObj->GetKarta()->GetPaperColor();
			if(pObj->m_selected)      // 28.03.2005   vipa -ATTENTION-
			{
				col = pObj->m_sel_color;
			}
				pDevice->SetZapolnenie(&col);
                //the array from 4 points = rectangle tops
				MPoint pts[4];  
				pts[0].x = pt.x + vysota_Down;
				pts[0].y = pt.y - dopolnenie;   
				pts[1].x = pt.x + vysota_Down;
				pts[1].y = pt.y + dlina_form; 
				pts[2].x = pt.x - vysota_Up;
				pts[2].y = pt.y + dlina_form;  
				pts[3].x = pt.x - vysota_Up;   
				pts[3].y = pt.y - dopolnenie;
                //drawing of a rectangle
				pDevice->Polygon(pts, 4);  
				pDevice->RestoreZapolnenie();
                //drawing of a formula
				DrawFormula(pObj, pDevice, pta, formula );
			}
			break;
		}//endOfSwitch{}
		delete pta;
    }//end for(int i=0; i<vectorforms->GetSize(); i++)
	pObj->bDrawColor = oldbDrawColor;
}



//========================================================
//!uses the ObtainMassivStrok and GetPredlogenieVysota functions
double  MDrawing::GetFragmentVysotaUp(MTopobject* pObj,MPlotDevice* pDevice, MFragment* fragment)
//========================================================
{
    //the returned size
	double geo_vysotaUp = 0;
    //stanza height
//	double max_geo = 0;
	MStrofa* strofa =NULL;
	if (fragment->GetType() == ID_FRAG_STROFA) 
		strofa = (MStrofa*)fragment->GetFragment();	
    //receive the index on numerator
	if (fragment->GetType() == ID_FRAG_FRACTION)
		strofa = ((MFraction*)fragment->GetFragment())->GetChisl();
    //declare the new array of lines
    //also we copy in it lines
	MStArray* strofy = new MStArray();
    //cycle according to sentences
	for(int i=0; i<strofa->GetPredlogenieNum(); i++)
	{
		strofy->ClearAll();
//		MString strofa_name;
        //receive an index (code) of the name of the characteristic of the sentence with an index 'i'
        //in an array of sentences of a stanza
		unsigned short nompolename = strofa->GetNomPole(i);
		pObj->ObtainMassivStrok(nompolename, strofy);
		MPrk drawprk;
        //we will receive an index (code) of style of the sentence with an index 'i' in an array of sentences of a stanza
		unsigned char nomprk = strofa->GetNomPrk(i);
        //from the dictionary of styles on an index we will receive style if it is possible
		if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk))
			continue;

		unsigned int m_NewAlign = drawprk.align;
		m_NewAlign&=~UT_TA_LEFT;
		m_NewAlign&=~UT_TA_RIGHT;
		m_NewAlign&=~UT_TA_CENTER;	
        //we will receive numerator length depending on type
		int type_strofa = strofa->GetType();
		double geo_vysota = GetPredlogenieVysota(pObj,pDevice, type_strofa, strofy, &drawprk);
		switch (fragment->GetType()){
		case ID_FRAG_STROFA:
		{
			if (m_NewAlign == UT_TA_TOP)
			{
				if (geo_vysotaUp < geo_vysota)
					geo_vysotaUp = geo_vysota;
			}
			if (m_NewAlign == UT_TA_BOTTOM) 
				geo_vysotaUp = 0;
			if (m_NewAlign == UT_TA_BASELINE)
			{
				if (geo_vysotaUp < geo_vysota/2)
				geo_vysotaUp = geo_vysota/2;
			}
		}
		break;
		case ID_FRAG_FRACTION:
			{
				geo_vysotaUp = geo_vysota;
			}
			break;
		}

		double oneLineHeight;
        //we establish style height
		if(drawprk.scaled)
			oneLineHeight =(*ConvertSizeToReal)(pObj,drawprk.value);
		else
			oneLineHeight = pDevice->LogicToBum(drawprk.value);

		oneLineHeight*=0.2;

		geo_vysotaUp += oneLineHeight;		
	}
	delete strofy;
	return geo_vysotaUp;

}


//========================================================
//!uses the ObtainMassivStrok and GetPredlogenieVysota functions
double  MDrawing::GetFragmentVysotaDown(MTopobject* pObj,MPlotDevice* pDevice, MFragment* fragment)
//========================================================
{
    //the returned size
    double geo_vysotaDown = 0;
    //stanza height
//	double max_geo = 0;
	MStrofa* strofa =NULL;
	if (fragment->GetType() == ID_FRAG_STROFA) 
		strofa = (MStrofa*)fragment->GetFragment();	
    //we receive the index on a denominator
	if (fragment->GetType() == ID_FRAG_FRACTION)
		strofa = ((MFraction*)fragment->GetFragment())->GetZnam();
    //we declare the new array of lines
    //also we copy in it lines
	MStArray* strofy = new MStArray();
    //cycle according to sentences
	for(int i=0; i<strofa->GetPredlogenieNum(); i++)
	{
		strofy->ClearAll();
//		MString strofa_name;
        //receive an index (code) of the name of the characteristic of the sentence with an index 'i'
        //in an array of sentences of a stanza
		unsigned short nompolename = strofa->GetNomPole(i);
		pObj->ObtainMassivStrok(nompolename, strofy);

		MPrk drawprk;
        //we will receive an index (code) of style of the sentence with an index 'i' in an array of sentences of a stanza
		unsigned char nomprk = strofa->GetNomPrk(i);
        //from the dictionary of styles on an index we will receive style if it is possible
		if(!pObj->GetKarta()->GetDictPrkValue((unsigned short)nomprk, drawprk))
			continue;

		unsigned int m_NewAlign = drawprk.align;
		m_NewAlign&=~UT_TA_LEFT;
		m_NewAlign&=~UT_TA_RIGHT;
		m_NewAlign&=~UT_TA_CENTER;

        //we will receive numerator length depending on type
		int type_strofa = strofa->GetType();
		double geo_vysota = GetPredlogenieVysota(pObj, pDevice,type_strofa, strofy, &drawprk);
		switch (fragment->GetType()){
		case ID_FRAG_STROFA:
			{
				if (m_NewAlign == UT_TA_TOP)  
					geo_vysotaDown = 0;
				if (m_NewAlign == UT_TA_BOTTOM) 
				{
					if (geo_vysotaDown < geo_vysota)
						geo_vysotaDown = geo_vysota;		
				}
				if (m_NewAlign == UT_TA_BASELINE)
				{
					if (geo_vysotaDown < geo_vysota/2)
					geo_vysotaDown = geo_vysota/2;
				}
			}
			break;
		case ID_FRAG_FRACTION:
			{
				geo_vysotaDown = GetPredlogenieVysota(pObj,pDevice, type_strofa, strofy, &drawprk);
			}
			break;
		}
		double oneLineHeight;
        //establish style height
		if(drawprk.scaled)
			oneLineHeight =(*ConvertSizeToReal)(pObj,drawprk.value);
		else
			oneLineHeight = pDevice->LogicToBum(drawprk.value);

		oneLineHeight*=0.5;
		geo_vysotaDown += oneLineHeight;
	}
	delete strofy;
    return geo_vysotaDown;

}

//==============================================
//!  ------------
double MDrawing::GetPredlogenieVysota(MTopobject* pObj,MPlotDevice* pDevice,
										int type, MStArray* sta, MPrk* p_prk)
//type - stanza type
//sta - array of lines
//p_prk - style
//==============================================
{
	double geo_vysota =0.0;
    //quantity of elements of the array of lines
	int kolvo = sta->GetSize();

	double oneLineHeight;
    //establish style height
	if(p_prk->scaled)
		oneLineHeight =(*ConvertSizeToReal)(pObj,p_prk->value);
	else
		oneLineHeight = pDevice->LogicToBum(p_prk->value);

	if ((type == ID_STROFA_STOLBEC)||(type == ID_STROFA_SIMPLE))
				geo_vysota = kolvo*oneLineHeight;
	else if ((type == ID_STROFA_STROKA)||(type == ID_STROFA_COMPLEX))
				geo_vysota = oneLineHeight;
	return geo_vysota;
}
