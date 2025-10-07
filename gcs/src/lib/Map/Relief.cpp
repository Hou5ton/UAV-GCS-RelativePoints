//===============================================================================
// Рельеф (CPP)
//===============================================================================

#include "Relief.h"
#include "Klassifikator.h"
#include "privpoints.h"
#include "DrawObj.h"
#include "Refglobal.h"

extern MObject* (*fpCreateObjectByType)(MString str);
extern MZnak* (*fpCreateZnakByType)(MString str);

//Pointers on function of transformation for a concrete step of drawing
extern double (*ConvertSizeToReal)(MTopobject* pObj,float fSize);

extern void (*pSetPercent)();

//===============================================================================
// MIzoline
//===============================================================================
//================================================
//!Constructor by default
MIzoline::MIzoline()
//================================================
{
    F_TOCHKI = 4;
    //We form structure of the isoline
    m_dbarray.SetSize(ID_DBINITSIZE);

    //We set the set size
    m_Komplekt->FreeMemory();
    m_Komplekt->SetSize(F_TOCHKI+1);

    //By default inscription is empty
    MNvArray* nomvals = new MNvArray();
    m_Komplekt->SetAt(F_NADPIS,(MArray*)nomvals);

    //We form the array of vectors bindings of formulas
    MPvArray* vectpriv = new MPvArray();
    m_Komplekt->SetAt(F_FORMPRIV,(MArray*)vectpriv);

    //We form the array of ruptures of a contour of a horizontal
    MRzArray* razryvy = new MRzArray();
    m_Komplekt->SetAt(F_RAZRYVY,(MArray*)razryvy);

    //array of hachure
    MZbArray* zba = new MZbArray();
    m_Komplekt->SetAt(IZO_BERGSHTRIH, (MArray*)zba);

    //array of points of a horizontal
    MPtArray* pta = new MPtArray();
    m_Komplekt->SetAt(F_TOCHKI, (MArray*)pta);
}


//==================================
//! cloning
MObject* MIzoline::Clone()
//==================================
{
    MIzoline* pClone=new MIzoline();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_kkod=m_kkod;
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}


//==============================================
//!We join the service classifier
void MIzoline::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pIzoZnak;
    m_kkod = SERV_OBJECT_IZO;
    /*
    if(typeyaz==F_PLO){
        m_pZnak = GetKarta()->m_pSqrZnak;
        m_kkod = SERV_OBJECT_PLO;
    }
    //
    else if(typeyaz==F_LIN){
        m_pZnak = GetKarta()->m_pLinZnak;
        m_kkod = SERV_OBJECT_LIN;
    }
    //
    else if(typeyaz==F_DOT){
        m_pZnak = GetKarta()->m_pDotZnak;
        m_kkod = SERV_OBJECT_DOT;
    }
    //
    else{
        m_pZnak = GetKarta()->m_pLinZnak;
        m_kkod = SERV_OBJECT_LIN;
    }
*/
}


//==================================
//! Recalculation of the describing rectangle
bool MIzoline::RecalcBounds()
//==================================
{
    MTopobject::RecalcBounds();

    // we will give also an extra charge on hachure if there is a need
    MZbArray* zba=(MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    double berglength = 0;
    if(zba->GetSize() != 0)
        berglength = GetBergLengthScaled();

    m_bounds.left   -= berglength;
    m_bounds.right  += berglength;
    m_bounds.top    += berglength;
    m_bounds.bottom -= berglength;

    return true;
}

//==================================
//! Addition of the hachure in geocoordinates
void MIzoline::AddBergshtrih(const MZarubka& bergshtrih)
//==================================
{
    //We receive the index on the array of hachure
    MZbArray* bergar=(MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    MZarubka* berg= new MZarubka(bergshtrih);
    bergar->AddTail(berg);
    delete berg;

    // copying
    if (!RecalcBounds())
    {
        Invalidate();
    }

}

//==================================
//! Addition of the hachure in geocoordinates
void MIzoline::AddBergshtrih(int nomrebra, MPoint pt)
//==================================
{
    //We receive the pointer on the array of hachure
    MZbArray* bergar=(MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    MZarubka* berg= new MZarubka(nomrebra, pt);
    bergar->AddTail(berg);
    delete berg;

    // copying
    if (!RecalcBounds())
    {
        Invalidate();
    }

}

//==================================
//! Removal of the hachure according to his number
void MIzoline::DeleteBergshtrih(int num)
//==================================
{
    MZbArray* bergar=(MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    if(bergar)
        bergar->Delete(num);
}

//==================================
//! Removal of close points of a horizontal
void MIzoline::DeleteNearPoints(double shag)
//==================================
{
    if(shag<0) return;

    for(int i=1; i<GetPointsNumber()-1; i++){
        if((sqrt((GetPoint(i).x - GetPoint(i-1).x)*(GetPoint(i).x - GetPoint(i-1).x) + (GetPoint(i).y - GetPoint(i-1).y)*(GetPoint(i).y - GetPoint(i-1).y))<4*shag) &&
                (sqrt((GetPoint(i+1).x - GetPoint(i).x)*(GetPoint(i+1).x - GetPoint(i).x) + (GetPoint(i+1).y - GetPoint(i).y)*(GetPoint(i+1).y - GetPoint(i).y))<4*shag)){
            DeletePoint(i);
            i--;
        }
    }
    int n = GetPointsNumber();
    if((sqrt((GetPoint(0).x - GetPoint(n-2).x)*(GetPoint(0).x - GetPoint(n-2).x) + (GetPoint(0).y - GetPoint(n-2).y)*(GetPoint(0).y - GetPoint(n-2).y))<4*shag) &&
            (sqrt((GetPoint(1).x - GetPoint(0).x)*(GetPoint(1).x - GetPoint(0).x) + (GetPoint(1).y - GetPoint(0).y)*(GetPoint(1).y - GetPoint(0).y))<4*shag)){
        MPoint ptt;
        ptt = GetPoint(1);
        DeletePoint(n-1);
        DeletePoint(0);
        AddPoint(&ptt);
    }
}


//==================================


//!Transformation of a linear horizontal, depending on type,
//! in interpolation or approximating
void MIzoline::TransformOnType(double shag)
//==================================
{
    if(shag<0) return;

    int vid; //the type of a spline - will be closed / not be closed
    MPtArray* psplom = nullptr;
    //    MPtArray* psplom = new MPtArray();
    MString izotype;
    GetizoType(&izotype);
    if(izotype == "внутренняя")
        vid = SPV_ZAMKNUT;
    else vid = SPV_NEZAMKNUT;

    // case of interpolation horizontals...
    if(*m_dbarray.GetAt(3) == "1"){
        psplom = LOMTOISPL(((MPtArray*)m_Komplekt->GetAt(F_TOCHKI)), vid, atoi(*m_dbarray[4]));
        if(psplom != NULL){
            ((MPtArray*)m_Komplekt->GetAt(F_TOCHKI))->ClearAll();
            ((MPtArray*)m_Komplekt->GetAt(F_TOCHKI))->AppendTail(psplom);
        }
    }
    // case of approximating horizontals...
    if(*m_dbarray.GetAt(3) == "2"){
        double* param;
        int m_nPoint = GetPointsNumber();
        if(vid == SPV_ZAMKNUT)
        {
            // boundary conditions of the third sort
            param = (double*)malloc(sizeof(double)*(m_nPoint-1));
            for(int i=0; i<m_nPoint-1; i++)
                param[i] = shag/4;
        }
        // case when we build not closed spline...
        else{
            param = (double*)malloc(sizeof(double)*m_nPoint);
            param[0] = 0;
            param[m_nPoint-1] = 0;
            for(int i=1; i<m_nPoint - 1; i++)
                param[i] = shag/4;
        }//the end of a case of not closed spline.
        psplom = LOMTOASPL(((MPtArray*)m_Komplekt->GetAt(F_TOCHKI)), vid, param, atoi(*m_dbarray[4]));
        if(psplom != NULL){
            ((MPtArray*)m_Komplekt->GetAt(F_TOCHKI))->ClearAll();
            ((MPtArray*)m_Komplekt->GetAt(F_TOCHKI))->AppendTail(psplom);
        }
    }//end of creation of an approximating spline...
}

//==================================
//! Change of orientation to the return
bool MIzoline::OrientInverse()
//==================================
{
    MTopobject::OrientInverse();

    //We receive the pointer on the array of points
    MPtArray* pta1=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int	m_nPoints=pta1->GetSize();

    //We receive the pointer on the array of hachure of a horizontal
    MZbArray* bergar=(MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    int nberg = bergar->GetSize();
    if(nberg != 0){
        //for each hachure we change numbering of indexes
        for(int i=0; i<nberg; i++){
            MZarubka* bshtrih = bergar->GetAt(i);
            bshtrih->m_nomrebra = m_nPoints - bshtrih->m_nomrebra - 1;
        }
    }

    //We receive the index on the array of vectors-bindings
    //for each vector we interchange the position of the beginning and the end
    MPvArray* privs=(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPoint pt;
    for(int i=0; i<privs->GetSize(); i++){
        MPriv* priv = privs->GetAt(i);
        if(priv->GetPointNum()==2){
            MPoint* pts = priv->GetPoints();
            pt = pts[0];
            pts[0] = pts[1];
            pts[1] = pt;
        }
    }

    return true;
}

//==================================
//! Point insert before a point: a point in geocoordinates
void MIzoline::InsertPoint(int num, const MPoint& geopoint)
//==================================
{
    MTopobject::InsertPoint(num, geopoint);
}

//==================================
//! Point insert after a point: a point in geocoordinates
void MIzoline::InsertPointAfter(int num, const MPoint& geopoint)
//==================================
{
    MTopobject::InsertPointAfter(num, geopoint);
}

//==================================
//! Removal of a point according to its number
void MIzoline::DeletePoint(int num)
//==================================
{
    MTopobject::DeletePoint(num);
}

//=============================
//!Returns quantity of hachure
int MIzoline::GetBergNumber()
//=============================
{
    int m_nBerg=0;
    MZbArray* bergar;
    bergar=(MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    m_nBerg+=bergar->GetSize();

    return m_nBerg;
}

//=========================================
//!The hachure returns on an index
bool MIzoline::GetBergshtrih(int ind, MZarubka* berg)
//=========================================
{
    MZbArray* bergar = (MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    int n = bergar->GetSize();
    if(((n-1)<ind)||(ind<0)) return false;
    berg->Copy(bergar->GetAt(ind));
    return true;

}

//=========================================
//!The hachure of a horizontal on an entrance point forms;
//! we project an entrance point on a horizontal and on the received point we form a horizontal notch
bool MIzoline::ConstructBergshtrih(MPoint point, MZarubka* bergshtrih)
//=========================================
{
    if(!bergshtrih)
        return false;

    MPoint resultpt;
    int resultind;
    FindDistanceFromPoint(point,resultpt, &resultind);
    bergshtrih->SetZarubka(resultind, resultpt);

    return true;
}

//=========================================
//!Returns the hachure's length
float MIzoline::GetBergLength()
//=========================================
{
    if(!m_pZnak) return -1;
    if(!m_pZnak->GetYaz(IDY_SECOND)) return -1;
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    float berglength = ((MLinia*)linia->GetAt(0))->m_thickness;
    return berglength;
}

//=========================================
//!Returns the hachure's length increased by true large-scale coefficient
double MIzoline::GetBergLengthScaled()
//=========================================
{
    ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;
    float bergln = GetBergLength();
    if(bergln==-1) return -1;
    double berglength  =(*ConvertSizeToReal)((MTopobject*)this,bergln);
    return berglength;
}

//=========================================
//!Sets the hachure's length
bool MIzoline::SetBergLength(float berglength)
//=========================================
{
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    ((MLinia*)linia->GetAt(0))->m_thickness = berglength;
    return true;
}

//=========================================
//!Returns horizontal height
bool MIzoline::GetizoHeight(MString* height)
//=========================================
{
    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "Height"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Высота"))
            break;
    }
    //across Nome names of the characteristic "Height" we receive value of height of a horizontal
    //in a horizontal data file if those are available
    if(datnazv=="Высота"){
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom,*height);
        if(index==-1)
            return false;
    }
    else
        return false;

    return true;
}

//=========================================
//!Sets horizontal height
bool MIzoline::SetizoHeight(double height)
//=========================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    if(!attribs) return false;

    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "Height"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Высота"))
            break;
    }
    //across Nome names of the characteristic "Height" we set value of height of a horizontal
    //in a horizontal data file if earlier there was no value of height in a data file
    //horizontals, we will add an element to the array for storage of height of a horizontal
    if(datnazv=="Высота"){
        MString strheight;
        strheight.Format("%f", height);
        MString str = strheight.SpanExcluding(".");
        strheight.Delete(0, str.GetLength());
        strheight = strheight.SpanExcluding("0");
        if(strheight.GetLength()==1)
            strheight = str;
        else
            strheight = str + strheight;
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->SetValue(nom,strheight);
        if(index==-1){
            MNomValue* nomval = new MNomValue(nom,strheight);
            ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        }
    }
    else
        return false;

    return true;
}


//=========================================
//!Returns horizontal type
bool MIzoline::GetizoType(MString* ptype)
//=========================================
{
    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "View"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Вид"))
            break;
    }
    //across Nome names of the characteristic "View" we receive a type of a horizontal
    //in a horizontal data file if those are available
    if(datnazv=="Вид"){
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom,*ptype);
        if(index==-1)
            return false;
    }
    else
        return false;

    return true;
}

//=========================================
//!Sets horizontal type
bool MIzoline::SetizoType(MString type)
//=========================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    if(!attribs) return false;

    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "View"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Вид"))
            break;
    }
    //across Nome names of the characteristic "View" we set a type of a horizontal
    //in a horizontal data file if earlier the type of a horizontal wasn't specified
    //in a data file, we will add an element to the array for storage of a type of a horizontal
    if(datnazv=="Вид"){
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->SetValue(nom,type);
        if(index==-1){
            MNomValue* nomval = new MNomValue(nom,type);
            ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        }
    }
    else
        return false;

    return true;
}

//=======================================================================================================================
//!drawing
void MIzoline::Draw(MPlotDevice* pDevice)
//=======================================================================================================================
{
    MTopobject::Draw(pDevice);

    //we get access to the array of points
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);

    //now, if the horizontal has hachure, will draw them
    MZbArray* bergar = (MZbArray*)m_Komplekt->GetAt(IZO_BERGSHTRIH);
    int nberg = bergar->GetSize();// number of hachure
    //hachure aren't present
    if(nberg<=0) return;
    //we will find the hachure's length increased by true large-scale coefficient
    double berglength = GetBergLengthScaled();
    //length of the hachure isn't determined
    if(berglength==-1) return;
    MPtArray* ptar = new MPtArray();//the array for storage of two points of the hachure
    // cycle on all hachure...
    for(int j=0; j<nberg; j++){
        MZarubka* zb = bergar->GetAt(j);
        MPoint bergf = zb->m_pt;//the beginning of the hachure - lies on a horizontal
        //the hachure we draw perpendicular to a horizontal edge on which he lies
        //the directions of hachure should be focused down a slope!!!
        MPoint bergl = GetPerpendikular(pta->GetAt(zb->m_nomrebra),pta->GetAt(zb->m_nomrebra+1),bergf,berglength);
        ptar->AddTail(&bergf);
        ptar->AddTail(&bergl);
        //we draw the hachure
        MDrawing::DrawLine(this,m_pZnak->GetYaz(),pDevice,ptar);
        //we clean the array of points for formation of the new hachure
        ptar->ClearAll();
    }
    delete ptar;
    return;
}

//==================================
//! drawing of a marking of a horizontal
void MIzoline::DrawTracker(MPlotDevice* pDevice,TrackerState state)
//==================================
{

    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    // points: transformation to logical coordinates
    double d1Pix = pDevice->LogicToBum(1);
    double d2Pix = d1Pix*2;
    double d3Pix = d1Pix*3;
    double d5Pix = d1Pix*5;
    pDevice->SavePencil();
    switch (state)
    {
    // usual tracker
    case simple:
    {
        // we declare a pen 1
        M_LOG_PEN lpen1(PS_SOLID,MPoint(d1Pix),MColor(0xff,0xff,0xff));
        // we declare a pen 2
        M_LOG_PEN lpen2(PS_SOLID,MPoint(d3Pix),MColor(0,0,0));

        pDevice->SetPencil(&lpen2);
        pDevice->Polyline(pta->GetHead(), pta->GetSize());

        // riding broken line
        pDevice->SetPencil(&lpen1);
        pDevice->Polyline(pta->GetHead(), pta->GetSize());

        // points...
        for (int i = 0; i < pta->GetSize(); i++) {
            pDevice->PatBlt(pta->GetAt(i)->x - d2Pix, pta->GetAt(i)->y - d2Pix, d5Pix, d5Pix, BLACKNESS);
            pDevice->PatBlt(pta->GetAt(i)->x - d1Pix, pta->GetAt(i)->y - d1Pix, d3Pix, d3Pix, WHITENESS);
        }
        //we choose a system pen
    }
        break;
        // the main tracker of a horizontal which specifies provisions of hachure and signatures
    default:
    case normal:
    {
        MTopobject::DrawTracker(pDevice,normal);

        //we draw hachure if they are
        int colberg = GetBergNumber();
        for(int i=0; i<colberg; i++){
            MZarubka* berg = new MZarubka();
            GetBergshtrih(i, berg);
            //we will draw hachure black-and-white small squares, just as taps
            //top object points

            pDevice->PatBlt(berg->m_pt.x - d2Pix, berg->m_pt.y - d2Pix, d5Pix, d5Pix, BLACKNESS);
            pDevice->PatBlt(berg->m_pt.x - d1Pix, berg->m_pt.y - d1Pix, d3Pix, d3Pix, WHITENESS);
            delete berg;
        }
    }
        break;
    }

    pDevice->RestorePencil();
}

//==================================
//!Sets for a horizontal language and the form on a standard horizontal of a layer of a relief,
//! if such is filled
bool MIzoline::SetKlsAttributes(MString type)
//==================================
{
    if(!m_pLay) return false;
    if(m_pLay->WhatTheLayer()!=ID_RELIEF_LAYER) return false;

    if(type==IZ_OSNOVNAYA){
        ChangeClass(((MReliefLayer*)m_pLay)->m_qkodIzoline);
    }
    else if(type==IZ_DOPOLN){
        ChangeClass(((MReliefLayer*)m_pLay)->m_qkodDopizo);
    }
    else if(type==IZ_UTOLSCH){
        ChangeClass(((MReliefLayer*)m_pLay)->m_qkodUtlizo);
    }

    return true;
}

//===============================================================================
// MObryv
//===============================================================================
//================================================
//!Constructor by default
MObryv::MObryv()
//================================================
{
    F_TOCHKI = 9;
    //We form structure of break
    m_dbarray.SetSize(ID_DBINITSIZE);

    //by default we draw macroforms
    m_viewObryv = true;

    //We set the set size
    m_Komplekt->FreeMemory();

    m_Komplekt->SetSize(F_TOCHKI+1);

    //By default inscription empty
    MNvArray* nomvals = new MNvArray();
    m_Komplekt->SetAt(F_NADPIS,(MArray*)nomvals);

    //We form the array of vectors-bindings of formulas
    MPvArray* vectpriv = new MPvArray();
    m_Komplekt->SetAt(F_FORMPRIV,(MArray*)vectpriv);

    //By default the array of lines for storage of relative marks of depth of break is empty
    MStArray* infar =new MStArray();
    m_Komplekt->SetAt(OBR_INFORM,(MArray*)infar);
    //By default the array of points of relative marks of depth of break is empty
    MPtArray* infptar =new MPtArray();
    m_Komplekt->SetAt(OBR_INFORMPT,(MArray*)infptar);
    //The array of points - crossings of weight segments
    MPtArray* vessgptar =new MPtArray();
    m_Komplekt->SetAt(OBR_VESSEGMENTS,(MArray*)vessgptar);
    //array of indexes of the beginning and end of upper/bottom edge of break
    MInArray* indar = new MInArray();
    m_Komplekt->SetAt(OBR_INDEX,(MArray*)indar);
    //array of shading of break
    MPtArray* shtrihar = new MPtArray();
    m_Komplekt->SetAt(OBR_SHTRIHOVKA,(MArray*)shtrihar);
    //array of heights of points of border of break
    MDuArray* heightar = new MDuArray();
    m_Komplekt->SetAt(OBR_HEIGHTPT,(MArray*)heightar);
    //The array of the points of border of break forming the structural line
    MPtArray* structline = new MPtArray();
    m_Komplekt->SetAt(OBR_STRUCTLINE,(MArray*)structline);
    //array of points of border of break
    MPtArray* pta = new MPtArray();
    m_Komplekt->SetAt(F_TOCHKI,(MArray*)pta);
}

//====================================================================
//!Reading in UTP
bool MObryv::ReadBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    MTopobject::ReadBin(h_nuxfile,  version);

    //we read a sign of drawing of a macroform
    fread(&m_viewObryv, 1, 1, h_nuxfile);

    return true;
}

//====================================================================
//!Reading in UTP
bool MObryv::ReadBin(MFile* file,  int version)
//====================================================================
{
    MTopobject::ReadBin(file,  version);

    //we read a sign of drawing of a macroform
    file->GetData(&m_viewObryv, 1, 1);

    return true;
}

//====================================================================
//!Writing in UTP
bool MObryv::WriteBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    MTopobject::WriteBin(h_nuxfile,  version);

    //we write a sign of drawing of a macroform
    fwrite(&m_viewObryv, 1, 1, h_nuxfile);

    return true;
}

//=======================================
//!Returns the class size in UTP
unsigned int MObryv::GetBytes()
//=======================================
{
    //Calculation of the extent of object in UTP
    //The extent of object in UTP is calculated thus:
    //Parent's size
    //+ sign of drawing of a macroform (1 byte)

    unsigned int SBytes = MTopobject::GetBytes()+1;


    return SBytes;
}

//==================================
//! cloning
MObject* MObryv::Clone()
//==================================
{
    MObryv* pClone=new MObryv();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}

//==============================================
//!We join the service classifier
void MObryv::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pObrZnak;
    m_kkod = SERV_OBJECT_OBR;
}


//==================================
//! Recalculation of the describing rectangle
bool MObryv::RecalcBounds()
//==================================
{
    if (GetHandleCount() == 0)
        return false;

    MPoint* pt1=((MPtArray*)(m_Komplekt->GetAt(F_TOCHKI)))->GetAt(0);
    //  for a start we take a rectangle round the first point...
    if(!pt1) return false;
    MRect bounds(pt1->x,
                 pt1->y,
                 pt1->x,
                 pt1->y);
    bounds.NormalizeGeoRect();

    // ... it also has to cover all the subsequent...
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int Size=pta->GetSize();
    for(int i=0;i<Size;i++)
    {
        pt1=pta->GetAt(i);
        bounds.ExpandToPt(pt1[0]);
    }
    bounds.NormalizeGeoRect();

    // we will give also an extra charge on a break depth inscription if there is a need
    int col = GetNadpisNumber();
    if(col != 0){
        unsigned char prkkod = GetNadpisPrkkod();
        MPrk prk;
        m_pLay->GetKarta()->GetDictPrkValue((unsigned short)prkkod, prk);
        double value = GEOM(prk.value);
        bounds.left   -= 2*value;
        bounds.right  += 2*value;
        bounds.top    += 2*value;
        bounds.bottom -= 2*value;
    }

    if (bounds == m_bounds)
        return false;

    m_bounds = bounds;
    return true;
}


//==================================
//!adds an inscription of depth or height of upper/bottom edge of break to the line
//! and dot arrays
void MObryv::AddNadpis(MPoint nadpispt, MString nadpis)
//==================================
{
    MStArray* infar = (MStArray*)m_Komplekt->GetAt(OBR_INFORM);
    infar->AddTail(&nadpis);
    MPtArray* infptar = (MPtArray*)m_Komplekt->GetAt(OBR_INFORMPT);
    infptar->AddTail(&nadpispt);

    // copying
    if (!RecalcBounds())
    {
        Invalidate();
    }
}

//==================================
//!deletes an inscription of depth or height of upper/bottom edge of break with number num from
//! line and dot arrays
void MObryv::DeleteNadpis(int num)
//==================================
{
    int col = GetNadpisNumber();

    if((num>=col)||(num<0)) return;

    MStArray* infar = (MStArray*)m_Komplekt->GetAt(OBR_INFORM);
    infar->Delete(num);
    MPtArray* infptar = (MPtArray*)m_Komplekt->GetAt(OBR_INFORMPT);
    infptar->Delete(num);

    // copying
    if (!RecalcBounds())
    {
        Invalidate();
    }
}

//==================================
//!gives out number of inscriptions of depth or height of upper/bottom edge of break
int	MObryv::GetNadpisNumber()
//==================================
{
    MStArray* infar = (MStArray*)m_Komplekt->GetAt(OBR_INFORM);
    int col = infar->GetSize();
    return col;
}

//==================================
//!gives out a code of handwriting of an inscription of depth or height of upper/bottom edge of break
/** from the array of formulas of additional language of a sign takes the first, in it the first fragment,
//if it is fraction, we take a code of handwriting of the first offer of numerator,
//if a stanza, we take a code of handwriting of the first offer of a stanza
*/
unsigned char MObryv::GetNadpisPrkkod()
//==================================
{
    MFormArray* formuli = m_pZnak->GetYaz(1)->m_formula;
    unsigned char prkkod = 0;
    if(formuli->GetSize()!=0){

        MFormula* formula = formuli->GetAt(0);
        MFrArray* frar = formula->fragm_arr;
        MFragment* fragment = frar->GetAt(0);
        unsigned char type = fragment->GetType();
        if(type==ID_FRAG_FRACTION){
            MFraction* fraction = (MFraction*)fragment->GetFragment();
            MStrofa* strofa = fraction->GetChisl();
            prkkod = strofa->GetNomPrk(0);
        }
        if(type==ID_FRAG_STROFA){
            MStrofa* strofa = (MStrofa*)fragment->GetFragment();
            prkkod = strofa->GetNomPrk(0);
        }
    }
    return prkkod;
}

//==================================
//!sets a code of handwriting of an inscription of depth or height of upper/bottom edge of break
bool MObryv::SetNadpisPrkkod(unsigned char prkkod)
//==================================
{
    MFormArray* formuli = m_pZnak->GetYaz(1)->m_formula;
    if(formuli->GetSize()==0)
        return false;

    MFormula* formula = formuli->GetAt(0);
    MFrArray* frar = formula->fragm_arr;
    MFragment* fragment = frar->GetAt(0);
    unsigned char type = fragment->GetType();
    if(type==ID_FRAG_FRACTION){
        MFraction* fraction = (MFraction*)fragment->GetFragment();
        MStrofa* strofa = fraction->GetChisl();
        strofa->SetNomPrk(0,prkkod);
    }
    if(type==ID_FRAG_STROFA){
        MStrofa* strofa = (MStrofa*)fragment->GetFragment();
        strofa->SetNomPrk(0,prkkod);
    }
    return true;
}

//==================================
//!gives out a symbol of an inscription of depth or height of upper/bottom edge of break
MSymbol* MObryv::GetNadpisSym()
//==================================
{
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    MSymbol* sym = &((MSbStretch*)linia->GetAt(0))->m_symbol;
    return sym;
}

//==================================
//!sets a symbol of an inscription of depth or height of upper/bottom edge of break
bool MObryv::SetNadpisSym(MSymbol* sym)
//==================================
{
    if(!sym)
        return false;
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    ((MSbStretch*)linia->GetAt(0))->m_symbol.Copy(sym);
    return true;
}

//==================================
//!gives out distance between break strokes
float MObryv::GetDistShtrih()
//==================================
{
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    float shtrihdist = ((MSbStretch*)linia->GetAt(0))->m_period;
    return shtrihdist;
}

//==================================
//!gives out the distance between break strokes increased
//! on true large-scale coefficient
double MObryv::GetDistShtrihScaled()
//==================================
{
    ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;
    float rasst = GetDistShtrih();//distance between strokes in mm
    //the distance between strokes in mm increased by true large-scale coefficient
    double shtrihdist = (*ConvertSizeToReal)((MTopobject*)this,rasst);
    return shtrihdist;
}

//==================================
//!sets distance between break strokes
bool MObryv::SetDistShtrih(float dist)
//==================================
{
    //the distance between strokes can't be negative size
    if(dist<0)
        return false;

    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    ((MSbStretch*)linia->GetAt(0))->m_period = dist;
    return true;
}

//==================================
//!gives out coordinates of a point of intersection of a weight segment with the lower bound of break
//! according to number of a weight segment
bool MObryv::GetPointVesSegment(int ind, MPoint* ptsg)
//==================================
{
    MPtArray* vessgptar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
    int col = vessgptar->GetSize();
    if((ind>=col)||(ind<-1)) return false;
    ptsg = vessgptar->GetAt(ind);
    return true;
}

//==================================
//!gives out point height - break contour borders
double MObryv::GetHeight(int ind)
//==================================
{
    MDuArray* heightar = (MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    int col = heightar->GetSize();
    if((ind>=col)||(ind<-1)) return (-1);
    double height = heightar->GetAt(ind)[0];
    return height;
}

//==================================
//!gives out point height - break contour borders
bool MObryv::SetHeight(int ind, double height)
//==================================
{
    MDuArray* heightar = (MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    int col = heightar->GetSize();
    if((ind>=col)||(ind<-1)) return false;
    heightar->SetAt(ind, &height);
    return true;
}

//==================================
//!gives out an index of the beginning of the upper edge of break
int MObryv::GetUpFirstInd()
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return (-1);
    int col = indar->GetSize();
    if(col==0) return (-1);
    int ind = indar->GetAt(0)[0];
    return ind;
}

//==================================
//!gives out an index of the end of the upper edge of break
int	MObryv::GetUpLastInd()
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return (-1);
    int col = indar->GetSize();
    if(col<2) return (-1);
    int ind = indar->GetAt(1)[0];
    return ind;
}

//==================================
//!gives out an index of the beginning of the bottom edge of break
int	MObryv::GetDownFirstInd()
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return (-1);
    int col = indar->GetSize();
    if(col<3) return (-1);
    int ind = indar->GetAt(2)[0];
    return ind;
}

//==================================
//!gives out an index of the end of the bottom edge of break
int	MObryv::GetDownLastInd()
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return (-1);
    int col = indar->GetSize();
    if(col<4) return (-1);
    int ind = indar->GetAt(3)[0];
    return ind;
}

//==================================
//!sets an index of the beginning of the upper edge of break
bool MObryv::SetUpFirstInd(int upfirstind)
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return false;
    int col = indar->GetSize();
    if(col == 0)
        indar->SetSize(1);
    indar->SetAt(0, &upfirstind);
    return true;
}

//==================================
//!sets an index of the end of the upper edge of break
bool MObryv::SetUpLastInd(int uplastind)
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return false;
    int col = indar->GetSize();
    if(col <= 1)
        indar->SetSize(2);
    indar->SetAt(1, &uplastind);
    return true;
}

//==================================
//!sets an index of the beginning of the bottom edge of break
bool MObryv::SetDownFirstInd(int downfirstind)
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return false;
    int col = indar->GetSize();
    if(col <= 2)
        indar->SetSize(3);
    indar->SetAt(2, &downfirstind);
    return true;
}

//==================================
//!sets an index of the end of the bottom edge of break
bool MObryv::SetDownLastInd(int downlastind)
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar == NULL)
        return false;
    int col = indar->GetSize();
    if(col <= 3)
        indar->SetSize(4);
    indar->SetAt(3, &downlastind);
    return true;
}

//==================================
//! Returns a break contour
MPtArray* MObryv::GetKontur()
//==================================
{
    MPtArray* kontur = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);

    return kontur;
}

//==================================
//! Returns heights of points of a contour of break
MDuArray* MObryv::GetHeights()
//==================================
{
    MDuArray* ptsheights = (MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);

    return ptsheights;
}

//==================================
//!Designs a break contour in the form of the structural line
bool MObryv::MakeStructKontur(MPtArray* structkonturfirst, MPtArray* /*structkonturlast*/)
//==================================
{
    if(!structkonturfirst)
        return false;
    structkonturfirst->ClearAll();
    //if the contour isn't defined
    MPtArray* kontur = GetKontur();
    if(!kontur)
        return false;
    if(*m_dbarray.GetAt(0)=="неопределена"){
        structkonturfirst->AppendTail(kontur);
        return true;
    }
    //if one of indexes isn't defined
    int upfirstind = GetUpFirstInd();
    int uplastind = GetUpLastInd();
    int downfirstind = GetDownFirstInd();
    int downlastind = GetDownLastInd();
    if((upfirstind==-1)||(uplastind==-1)||(downfirstind==-1)||(downlastind==-1))
        return false;
    structkonturfirst->AppendTail(kontur);
    if(*structkonturfirst->GetAt(upfirstind) == *structkonturfirst->GetAt(downfirstind))
        structkonturfirst->Delete(downfirstind);
    if(*structkonturfirst->GetAt(uplastind) == *structkonturfirst->GetAt(downlastind))
        structkonturfirst->Delete(downlastind);
    //to receive the closed contour, we will add the last point equal to the initial
    structkonturfirst->AddTail(structkonturfirst->GetAt(0));
    return true;
}

//==================================
//!returns a macroform contour (for a hole of the upper edge) in the form of the structural line
MPtArray* MObryv::GetStructKontur()
//==================================
{
    MPtArray* structline = (MPtArray*)m_Komplekt->GetAt(OBR_STRUCTLINE);
    return structline;
}

//==================================
//!sets the structural line (for a hole of the upper edge) macroforms
bool  MObryv::SetStructKontur(MPtArray* structkontur)
//==================================
{
    if(!structkontur)
        return false;
    m_Komplekt->SetAt(OBR_STRUCTLINE, (MArray*)structkontur);
    return true;
}

//==================================
//! Addition of a point: a point in geocoordinates
void MObryv::AddPoint(const MPoint& geopoint, double height)
//==================================
{
    MTopobject::AddPoint(geopoint);

    //We receive the pointer on the array of heights
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    //to the array of heights we add 'geopoint' point height
    heightar->AddTail(&height);
}

//==================================
//!Point insert after a point: a point in geocoordinates
/**Height of the inserted point is set
//REMARK: 1) numbering of points of the upper bound of break begins with a zero index,
//an index of the first point of the lower bound of break of kolpts-1, where kolpts -
//number of points of a contour of break;
//2) the point insert in a contour of break leads to change of indexes of extreme points
//the upper and lower bound of break, except the initial point of the upper bound of break;
//3) points can't be inserted between break borders;
//4) if the pttype=true parameter, the inserted point relief, and we owe it also
//to insert into the structural contour corresponding to a macroform contour.
*/
void MObryv::InsertPointAfter(int num, const MPoint& geopoint, double height,bool pttype)
//==================================
{
    int uplastind = GetUpLastInd(); //index of the last point of the upper bound of break
    int downlastind = GetDownLastInd(); //index of the last point of the bottom bound of break
    //if indexes of the top and lower parts aren't defined
    if((uplastind==-1)||(downlastind==-1)){
        //we cause function of the parent
        MTopobject::InsertPointAfter(num, geopoint);
        return;
    }

    int kolpts = GetPointsNumber(); //number of points of a contour of break
    //points can't be inserted between break borders;
    //the index shouldn't overstep the bounds of indexes of points of a contour
    if((num==uplastind)||(num>=kolpts-1)||(num<0))
        return;

    MPoint pt=geopoint;

    //if we insert the situational point which isn't participating in designing of the relief
    //surfaces, the structural line of a contour of a macroform won't change,
    //otherwise the point, but, generally will also be added to the structural line
    //the index on which it needs to be added, doesn't coincide with an index of points of a contour
    //macroforms
    if(pttype){
        MPtArray* upstructline = GetStructKontur();
        MPtArray* downstructline = GetDownStructKontur();
        if(WhatTheObject()!=ID_YAMA){
            MInArray* structind = new MInArray();
            //we will receive indexes of coinciding points of a contour and corresponding to it
            //structural line
            CompareStructKonturWithKontur(structind);
            int index=0;
            //we will define a point index in the structural line after which we insert a point
            for(int i=0; i<structind->GetSize(); i++){
                if(*structind->GetAt(i)>num){
                    index = i-1;
                    break;
                }
            }
            upstructline->InsertAfter(index, &pt);
            delete structind;
        }
        //for a hole we will add two contours
        if(WhatTheObject()==ID_YAMA){
            MInArray* upstructind = new MInArray();
            MInArray* downstructind = new MInArray();
            //we will receive indexes of coinciding points of contours of top and a bottom of a hole
            //and corresponding to them structural lines
            CompareStructKonturWithKontur(upstructind, downstructind);
            int index = *upstructind->GetTail();
            //we insert a point into the structural line of top of a hole
            if(num<index){
                //we will define a point index in the structural line after which we insert a point
                for(int i=0; i<upstructind->GetSize(); i++){
                    if(*upstructind->GetAt(i)>num){
                        index = i-1;
                        break;
                    }
                }
                upstructline->InsertAfter(index, &pt);
            }
            //we insert a point into the structural line of a bottom of a hole
            else{
                //we will define a point index in the structural line after which we insert a point
                for(int i=0; i<downstructind->GetSize(); i++){
                    if(*downstructind->GetAt(i)>num){
                        index = i-1;
                        break;
                    }
                }
                downstructline->InsertAfter(index, &pt);
            }
            delete upstructind;
            delete downstructind;
        }
    }

    //we will receive the index on the array of marks of heights of points of a contour of break
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    if(!heightar)
        return;
    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if(!ptar)
        return;
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);

    //if we add a point to the upper bound of break, indexes of extreme points of parts
    //increase by unit, except for an index of the initial point of the upper bound
    if((num>=0)&&(num<uplastind)){
        MPoint crosspt, vespoint, perppt;
        //we update the array of weight segments - we insert a new weight segment
        if(vesar){
            //crosspt - a point of intersection of weight straight lines between which it is inserted
            //new weight segment
            //if weight straight lines are crossed...
            if(CROSSLINES(*ptar->GetAt(num), *vesar->GetAt(num+1), *ptar->GetAt(num), *vesar->GetAt(num), &crosspt)){
                ptar->InsertAfter(num,&pt);
                //we will change indexes of extreme points of borders of break
                uplastind++;
                downlastind++;
                SetUpLastInd(uplastind);
                SetDownLastInd(downlastind);
                SetDownFirstInd(kolpts);
                //point of intersection of the straight line passing through new top of the upper bound
                //a contour and a point of intersection of weight straight lines, with the lower bound of a contour -
                //it is the end of a new weight segment
                int crosspar = GetCrossptWithDownBorder(pt, crosspt, &vespoint);
                //if there are no crossings with the lower bound, a straight line point of intersection,
                //the upper bound of a contour passing through new top and a point of intersection
                //weight straight lines, with a contour is the end of a new weight segment
                if(crosspar == -1)
                    crosspar = GetCrossptWithKontur(ptar, pt, crosspt, &vespoint);
                //if the "weight" straight line doesn't cross a contour anywhere, except 'pt' point
                if(crosspar == -1)
                    vespoint = pt;
                vesar->InsertAfter(num, &vespoint);
            }
        }
        //if "weight" straight lines are parallel...
        else{
            //we look for coordinates of the end of a single vector from the beginning in new top top
            //borders of the contour lying on a perpendicular to an edge of the upper bound of a contour and directed
            //in a contour
            perppt = GetPerpendikular(ptar->GetAt(num), ptar->GetAt(num+1), pt, -1);
            ptar->InsertAfter(num,&pt);
            //we will change indexes of extreme points of borders of break
            uplastind++;
            downlastind++;
            SetUpLastInd(uplastind);
            SetDownLastInd(downlastind);
            SetDownFirstInd(kolpts);
            GetCrossptWithKontur(ptar, pt, perppt, &vespoint);
            vesar->InsertAfter(num, &vespoint);
        }
    }
    //if we add a point to the lower bound of break, an index of the last point lower
    //borders increases by unit
    else if((num>=downlastind)&&(num<kolpts-1)){
        ptar->InsertAfter(num,&pt);
        //we will change indexes of extreme points of borders of break
        SetDownFirstInd(kolpts);
    }
    else return;
    heightar->InsertAfter(num,&height);

    if(WhatTheObject()!=ID_GIDRO){
        SetShtrihFirstPoints();
        MasterShtrihovka();
    }
}

//==================================
//!Point insert after a point: a point in geocoordinates
/**Height of the inserted point if it is possible, is approximated from a relief or
//sets manually
//REMARK: 1) numbering of points of the upper bound of break begins with a zero index,
//an index of the first point of the lower bound of break of kolpts-1, where kolpts -
//number of points of a contour of break;
//2) the point insert in a contour of break leads to change of indexes of extreme points
//the upper and lower bound of break, except the initial point of the upper bound of break;
//3) points can't be inserted between break borders;
//4) if the pttype=true parameter, the inserted point relief, and we owe it also
//to insert into the structural contour corresponding to a macroform contour.
*/
void MObryv::InsertPointAfter(int num, const MPoint& geopoint,bool pttype)
//==================================
{
    int uplastind = GetUpLastInd(); //index of the last point of the upper bound of break
    int downlastind = GetDownLastInd(); //index of the last point of the lower bound of break
    //if indexes of the top and lower parts aren't defined
    if((uplastind==-1)||(downlastind==-1)){
        //we cause function of the parent
        MTopobject::InsertPointAfter(num, geopoint);
        return;
    }
    int kolpts = GetPointsNumber(); //number of points of a contour of break
    //points can't be inserted between break borders;
    //the index shouldn't overstep the bounds of indexes of points of a contour
    if((num==uplastind)||(num>=kolpts-1)||(num<0))
        return;

    MPoint pt=geopoint;
    //if we insert the situational point which isn't participating in designing of the relief
    //surfaces, the structural line of a contour of a macroform won't change,
    //otherwise the point, but, generally will also be added to the structural line
    //the index on which it needs to be added, doesn't coincide with an index of points of a contour
    //macroforms
    if(pttype){
        MPtArray* upstructline = GetStructKontur();
        MPtArray* downstructline = GetDownStructKontur();
        if(WhatTheObject()!=ID_YAMA){
            MInArray* structind = new MInArray();
            //we will receive indexes of coinciding points of a contour and corresponding to it
            //structural line
            CompareStructKonturWithKontur(structind);
            int index=0;
            //we will define a point index in the structural line after which we insert a point
            for(int i=0; i<structind->GetSize(); i++){
                if(*structind->GetAt(i)>num){

                    index = i-1;
                    break;
                }
            }
            upstructline->InsertAfter(index, &pt);
            ((MReliefLayer*)GetLayer())->FixStructLine(upstructline);
            delete structind;
        }
        //for a hole we will add two contours
        if(WhatTheObject()==ID_YAMA){
            MInArray* upstructind = new MInArray();
            MInArray* downstructind = new MInArray();
            //we will receive indexes of coinciding points of contours of top and a bottom of a hole
            //and corresponding to them structural lines
            CompareStructKonturWithKontur(upstructind, downstructind);
            int index = *upstructind->GetTail();
            //we insert a point into the structural line of top of a hole
            if(num<index){
                //we will define a point index in the structural line after which we insert a point
                for(int i=0; i<upstructind->GetSize(); i++){
                    if(*upstructind->GetAt(i)>num){
                        index = i-1;
                        break;
                    }
                }
                upstructline->InsertAfter(index, &pt);
                ((MReliefLayer*)GetLayer())->FixStructLine(upstructline);
            }
            //we insert a point into the structural line of a bottom of a hole
            else{
                //we will define a point index in the structural line after which we insert a point
                for(int i=0; i<downstructind->GetSize(); i++){
                    if(*downstructind->GetAt(i)>num){
                        index = i-1;
                        break;
                    }
                }
                downstructline->InsertAfter(index, &pt);
                ((MReliefLayer*)GetLayer())->FixStructLine(downstructline);
            }
            delete upstructind;
            delete downstructind;
        }
    }

    //we will receive the index on the array of marks of heights of points of a contour of break
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    if(!heightar)
        return;
    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if(!ptar)
        return;
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);

    //if we add a point to the upper bound of break, indexes of extreme points of parts
    //increase by unit, except for an index of the initial point of the upper bound
    if((num>=0)&&(num<uplastind)){
        MPoint crosspt, vespoint, perppt;
        //we update the array of weight segments - we insert a new weight segment
        if(vesar){
            //'crosspt' - a point of intersection of weight straight lines between which it is inserted
            //new weight segment
            //if weight straight lines are crossed...
            if(CROSSLINES(*ptar->GetAt(num), *vesar->GetAt(num+1), *ptar->GetAt(num), *vesar->GetAt(num), &crosspt)){
                ptar->InsertAfter(num,&pt);
                //we will change indexes of extreme points of borders of break
                uplastind++;
                downlastind++;
                SetUpLastInd(uplastind);
                SetDownLastInd(downlastind);
                SetDownFirstInd(kolpts);
                //point of intersection of the straight line passing through new top of the upper bound
                //a contour and a point of intersection of weight straight lines, with the lower bound of a contour -
                //it is the end of a new weight segment
                int crosspar = GetCrossptWithDownBorder(pt, crosspt, &vespoint);
                //if there are no crossings with the lower bound, a straight line point of intersection,
                //the upper bound of a contour passing through new top and a point of intersection
                //weight straight lines, with a contour is the end of a new weight segment
                if(crosspar == -1)
                    crosspar = GetCrossptWithKontur(ptar, pt, crosspt, &vespoint);
                //if the "weight" straight line doesn't cross a contour anywhere, except 'pt' point
                if(crosspar == -1)
                    vespoint = pt;
                vesar->InsertAfter(num, &vespoint);
            }
        }
        //if "weight" straight lines are parallel...
        else{
            //we look for coordinates of the end of a single vector from the beginning in new top top
            //borders of the contour lying on a perpendicular to an edge of the upper bound of a contour and directed
            //in a contour
            perppt = GetPerpendikular(ptar->GetAt(num), ptar->GetAt(num+1), pt, -1);
            ptar->InsertAfter(num,&pt);
            //we will change indexes of extreme points of borders of break
            uplastind++;
            downlastind++;
            SetUpLastInd(uplastind);
            SetDownLastInd(downlastind);
            SetDownFirstInd(kolpts);
            GetCrossptWithKontur(ptar, pt, perppt, &vespoint);
            vesar->InsertAfter(num, &vespoint);
        }
    }
    //if we add a point to the lower bound of break, an index of the last point lower
    //borders increases by unit
    else if((num>=downlastind)&&(num<kolpts-1)){
        ptar->InsertAfter(num,&pt);
        //we will change indexes of extreme points of borders of break
        SetDownFirstInd(kolpts);
    }
    //we will receive height of the inserted point linear interpolation on heights of the ends of an edge,
    //on which we insert a point
    double height = HGTPT(ptar->GetAt(num), ptar->GetAt(num+1), geopoint, *heightar->GetAt(num), *heightar->GetAt(num+1));
    heightar->InsertAfter(num,&height);

    if(WhatTheObject()!=ID_GIDRO){
        SetShtrihFirstPoints();
        MasterShtrihovka();
    }
}


//==================================
//!Point insert before a point: a point in geocoordinates
/**Height of the inserted point is set
//REMARK: 1) numbering of points of the upper bound of break begins with a zero index,
//an index of the first point of the lower bound of break of kolpts-1, where kolpts -
//number of points of a contour of break;
//2) the point insert in a contour of break leads to change of indexes of extreme points
//the upper and lower bound of break, except the initial point of the upper bound of break;
//3) points can't be inserted between break borders;
//4) if the pttype=true parameter, the inserted point relief, and we owe it also
//to insert into the structural contour corresponding to a macroform contour.
*/
void MObryv::InsertPoint(int num, const MPoint& geopoint, double height,bool pttype)
//==================================
{
    int uplastind = GetUpLastInd(); //index of the last point of the upper bound of break
    int downlastind = GetDownLastInd(); //index of the last point of the lower bound of break
    //if indexes of the top and lower parts aren't defined
    if((uplastind==-1)||(downlastind==-1)){
        //we cause function of the parent
        MTopobject::InsertPoint(num, geopoint);
        return;
    }
    int kolpts = GetPointsNumber(); //number of points of a contour of break
    //points can't be inserted between break borders;
    //the index shouldn't overstep the bounds of indexes of points of a contour
    if((num==downlastind)||(num<=0)||(num>=kolpts))
        return;

    MPoint pt=geopoint;
    //if we insert the situational point which isn't participating in designing of the relief
    //surfaces, the structural line of a contour of a macroform won't change,
    //otherwise the point, but, generally will also be added to the structural line
    //the index on which it needs to be added, doesn't coincide with an index of points of a contour
    //macroforms
    if(pttype){
        MPtArray* upstructline = GetStructKontur();
        MPtArray* downstructline = GetDownStructKontur();
        if(WhatTheObject()!=ID_YAMA){
            MInArray* structind = new MInArray();
            //we will receive indexes of coinciding points of a contour and corresponding to it
            //structural line
            CompareStructKonturWithKontur(structind);
            int index=0;
            //we will define a point index in the structural line before which we insert a point
            for(int i=0; i<structind->GetSize(); i++){
                if(*structind->GetAt(i)>num){
                    index = i;
                    break;
                }
            }
            upstructline->InsertBefore(index, &pt);
            delete structind;
        }
        //for a hole we will add two contours
        if(WhatTheObject()==ID_YAMA){
            MInArray* upstructind = new MInArray();
            MInArray* downstructind = new MInArray();
            //we will receive indexes of coinciding points of contours of top and a bottom of a hole
            //and corresponding to them structural lines
            CompareStructKonturWithKontur(upstructind, downstructind);
            int index = *upstructind->GetTail();
            //we insert a point into the structural line of top of a hole
            if(num<index){
                //we will define a point index in the structural line before which we insert a point
                for(int i=0; i<upstructind->GetSize(); i++){
                    if(*upstructind->GetAt(i)>num){
                        index = i;
                        break;
                    }
                }
                upstructline->InsertBefore(index, &pt);
            }
            //we insert a point into the structural line of a bottom of a hole
            else{
                //we will define a point index in the structural line before which we insert a point
                for(int i=0; i<downstructind->GetSize(); i++){
                    if(*downstructind->GetAt(i)>num){
                        index = i;
                        break;
                    }
                }
                downstructline->InsertBefore(index, &pt);
            }
            delete upstructind;
            delete downstructind;
        }
    }

    //we will receive the index on the array of marks of heights of points of a contour of break
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    if(!heightar)
        return;
    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if(!ptar)
        return;
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);

    //if we add a point to the upper bound of break, indexes of extreme points of parts
    //increase by unit, except for an index of the initial point of the upper bound
    if((num>0)&&(num<=uplastind)){
        MPoint crosspt, vespoint, perppt;
        //we update the array of weight segments - we insert a new weight segment
        if(vesar){
            //'crosspt' - a point of intersection of weight straight lines between which it is inserted
            //new weight segment
            //if weight straight lines are crossed...
            if(CROSSLINES(*ptar->GetAt(num-1), *vesar->GetAt(num-1), *ptar->GetAt(num), *vesar->GetAt(num), &crosspt)){
                //we will add a point to a contour
                ptar->InsertBefore(num,&pt);
                //we will change indexes of extreme points of borders of break
                uplastind++;
                downlastind++;
                SetUpLastInd(uplastind);
                SetDownLastInd(downlastind);
                SetDownFirstInd(kolpts);
                //point of intersection of the straight line passing through new top of the upper bound
                //a contour and a point of intersection of weight straight lines, with the lower bound of a contour -
                //it is the end of a new weight segment
                int crosspar = GetCrossptWithDownBorder(pt, crosspt, &vespoint);
                //if there are no crossings with the lower bound, a straight line point of intersection,
                //the upper bound of a contour passing through new top and a point of intersection
                //weight straight lines, with a contour is the end of a new weight segment
                if(crosspar == -1)
                    crosspar = GetCrossptWithKontur(ptar, pt, crosspt, &vespoint);
                //if the "weight" straight line doesn't cross a contour anywhere, except 'pt' point
                if(crosspar == -1)
                    vespoint = pt;
                vesar->InsertBefore(num, &vespoint);
            }
        }
        //if "weight" straight lines are parallel...
        else{
            //we look for coordinates of the end of a single vector from the beginning in new top top
            //borders of the contour lying on a perpendicular to an edge of the upper bound of a contour and directed
            //in a contour
            perppt = GetPerpendikular(ptar->GetAt(num-1), ptar->GetAt(num), pt, -1);
            //we will add a point to a contour
            ptar->InsertBefore(num,&pt);
            //we will change indexes of extreme points of borders of break
            uplastind++;
            downlastind++;
            SetUpLastInd(uplastind);
            SetDownLastInd(downlastind);
            SetDownFirstInd(kolpts);
            GetCrossptWithKontur(ptar, pt, perppt, &vespoint);
            vesar->InsertBefore(num, &vespoint);
        }
    }
    //if we add a point to the lower bound of break, an index of the last point lower
    //borders increases by unit
    else if((num>downlastind)&&(num<=kolpts-1)){
        ptar->InsertBefore(num,&pt);
        //we will change indexes of extreme points of borders of break
        SetDownFirstInd(kolpts);
    }
    heightar->InsertBefore(num,&height);

    if(WhatTheObject()!=ID_GIDRO){
        SetShtrihFirstPoints();
        MasterShtrihovka();
    }
}

//==================================
//!Point insert before a point: a point in geocoordinates
/**Height of the inserted point if it is possible, is approximated from a relief or
//it is set manually
//REMARK: 1) numbering of points of the upper bound of break begins with a zero index,
//an index of the first point of the lower bound of break of kolpts-1, where kolpts -
//number of points of a contour of break;
//2) the point insert in a contour of break leads to change of indexes of extreme points
//the upper and lower bound of break, except the initial point of the upper bound of break;
//3) points can't be inserted between break borders;
//4) if the pttype=true parameter, the inserted point relief, and we owe it also
//to insert into the structural contour corresponding to a macroform contour.
*/
void MObryv::InsertPoint(int num, const MPoint& geopoint,bool pttype)
//==================================
{
    int uplastind = GetUpLastInd(); //index of the last point of the upper bound of break
    int downlastind = GetDownLastInd(); //index of the last point of the lower bound of break
    //if indexes of the top and lower parts aren't defined
    if((uplastind==-1)||(downlastind==-1)){
        //we cause function of the parent
        MTopobject::InsertPoint(num, geopoint);
        return;
    }
    int kolpts = GetPointsNumber(); //number of points of a contour of break
    //points can't be inserted between break borders;
    //the index shouldn't overstep the bounds of indexes of points of a contour
    if((num==downlastind)||(num<=0)||(num>=kolpts))
        return;

    MPoint pt=geopoint;
    //if we insert the situational point which isn't participating in designing of the relief
    //surfaces, the structural line of a contour of a macroform won't change,
    //otherwise the point, but, generally will also be added to the structural line
    //the index on which it needs to be added, doesn't coincide with an index of points of a contour
    //macroforms
    if(pttype){
        MPtArray* upstructline = GetStructKontur();
        MPtArray* downstructline = GetDownStructKontur();
        if(WhatTheObject()!=ID_YAMA){
            MInArray* structind = new MInArray();
            //we will receive indexes of coinciding points of a contour and corresponding to it
            //structural line
            CompareStructKonturWithKontur(structind);
            int index=0;
            //we will define a point index in the structural line before which we insert a point
            for(int i=0; i<structind->GetSize(); i++){
                if(*structind->GetAt(i)>=num){
                    index = i;
                    break;
                }
            }
            upstructline->InsertBefore(index, &pt);
            delete structind;
        }
        //for a hole we will add two contours
        if(WhatTheObject()==ID_YAMA){
            MInArray* upstructind = new MInArray();
            MInArray* downstructind = new MInArray();
            //we will receive indexes of coinciding points of contours of top and a bottom of a hole
            //and corresponding to them structural lines
            CompareStructKonturWithKontur(upstructind, downstructind);
            int index = *upstructind->GetTail();
            //we insert a point into the structural line of top of a hole
            if(num<index){
                //we will define a point index in the structural line before which we insert a point
                for(int i=0; i<upstructind->GetSize(); i++){
                    if(*upstructind->GetAt(i)>num){
                        index = i;
                        break;
                    }
                }
                upstructline->InsertBefore(index, &pt);
            }
            //we insert a point into the structural line of a bottom of a hole
            else{
                //we will define a point index in the structural line before which we insert a point
                for(int i=0; i<downstructind->GetSize(); i++){
                    if(*downstructind->GetAt(i)>num){
                        index = i;
                        break;
                    }
                }
                downstructline->InsertBefore(index, &pt);
            }
            delete upstructind;
            delete downstructind;
        }
    }

    //we will receive the index on the array of marks of heights of points of a contour of break
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    if(!heightar)
        return;
    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if(!ptar)
        return;
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);

    //if we add a point to the upper bound of break, indexes of extreme points of parts
    //increase by unit, except for an index of the initial point of the upper bound
    if((num>0)&&(num<=uplastind)){
        MPoint crosspt, vespoint, perppt;
        //we update the array of weight segments - we insert a new weight segment
        if(vesar){
            //'crosspt' - a point of intersection of weight straight lines between which it is inserted
            //new weight segment
            //if weight straight lines are crossed...
            if(CROSSLINES(*ptar->GetAt(num-1), *vesar->GetAt(num-1), *ptar->GetAt(num), *vesar->GetAt(num), &crosspt)){
                ptar->InsertBefore(num,&pt);
                //we will change indexes of extreme points of borders of break
                uplastind++;
                downlastind++;
                SetUpLastInd(uplastind);
                SetDownLastInd(downlastind);
                SetDownFirstInd(kolpts);
                //point of intersection of the straight line passing through new top of the upper bound
                //a contour and a point of intersection of weight straight lines, with the lower bound of a contour -
                //it is the end of a new weight segment
                int crosspar = GetCrossptWithDownBorder(pt, crosspt, &vespoint);
                //if there are no crossings with the lower bound, a straight line point of intersection,
                //the upper bound of a contour passing through new top and a point of intersection
                //weight straight lines, with a contour is the end of a new weight segment
                if(crosspar == -1)
                    crosspar = GetCrossptWithKontur(ptar, pt, crosspt, &vespoint);
                //if the "weight" straight line doesn't cross a contour anywhere, except 'pt' point
                if(crosspar == -1)
                    vespoint = pt;
                vesar->InsertBefore(num, &vespoint);
            }
        }
        //if "weight" straight lines are parallel...
        else{
            //we look for coordinates of the end of a single vector from the beginning in new top top
            //borders of the contour lying on a perpendicular to an edge of the upper bound of a contour and directed
            //in a contour
            perppt = GetPerpendikular(ptar->GetAt(num-1), ptar->GetAt(num), pt, -1);
            ptar->InsertBefore(num,&pt);
            //we will change indexes of extreme points of borders of break
            uplastind++;
            downlastind++;
            SetUpLastInd(uplastind);
            SetDownLastInd(downlastind);
            SetDownFirstInd(kolpts);
            GetCrossptWithKontur(ptar, pt, perppt, &vespoint);
            vesar->InsertBefore(num, &vespoint);
        }
    }
    //if we add a point to the lower bound of break, an index of the last point lower
    //borders increases by unit
    else if((num>downlastind)&&(num<=kolpts-1)){
        ptar->InsertBefore(num,&pt);
        //we will change indexes of extreme points of borders of break
        SetDownFirstInd(kolpts);
    }
    //we will receive height of the inserted point linear interpolation on heights of the ends of an edge,
    //on which we insert a point
    double height = HGTPT(ptar->GetAt(num-1), ptar->GetAt(num+1), geopoint, *heightar->GetAt(num-1), *heightar->GetAt(num));
    heightar->InsertBefore(num,&height);

    if(WhatTheObject()!=ID_GIDRO){
        SetShtrihFirstPoints();
        MasterShtrihovka();
    }
}

//==================================
//!test: where specifies the mouse pointer?
//! for break and its successors we check only contour points
bool MObryv::HitTest(MPoint point, double dDostup,MPtArray*& ptarr,int& index)
//==================================
{

    MPoint* pt;

    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int Count=ptar->GetSize();
    for(int j=0;j<Count;j++)
    {
        pt=ptar->GetAt(j);
        if ((fabs(point.x - pt->x)<= dDostup)&&
                (fabs(point.y - pt->y) <= dDostup))
        {
            index=j;
            ptarr=ptar;
            return true;
        }
    }
    index=-1;
    ptarr=NULL;
    return false;
}


//==================================
//!adds point height to the array of heights
void MObryv::AddHeight(double height)
//==================================
{
    //We receive the index on the array of heights
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    //to the array of heights we add 'geopoint' point height
    heightar->AddTail(&height);
}

//==================================
//!Removal of a point from a macroform contour
/**REMARK: 1) numbering of points of the upper bound of break begins with a zero index,
//an index of the first point of the lower bound of break of kolpts-1, where kolpts -
//number of points of a contour of break;
//2) removal of a point from a contour of break leads to change of indexes of extreme points
//the upper and lower bound of break, except the initial point of the upper bound of break;
//3) it is impossible to delete extreme points of borders of break.
*/
bool MObryv::DeletePointFromKontur(int num)
//==================================
{
    //we will receive the index on the array of marks of heights of points of a contour of break
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    if(!heightar)
        return false;

    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if(!ptar)
        return false;

    int uplastind = GetUpLastInd(); //index of the last point of the upper bound of break
    int downlastind = GetDownLastInd(); //index of the last point of the lower bound of break
    //if indexes of the top and lower parts aren't defined
    if((uplastind==-1)||(downlastind==-1)){
        //we cause function of the parent
        MTopobject::DeletePoint(num);
        return true;
    }

    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
    if(!vesar)
        return false;

    int kolpts = GetPointsNumber(); //number of points of a contour of break
    //it is impossible to delete extreme points of borders of break;
    //the index shouldn't overstep the bounds of indexes of points of a contour
    if((num==uplastind)||(num==downlastind)||(num<=0)||(num>=kolpts-1))
        return false;

    //if we delete the situational point which isn't participating in designing relief
    //surfaces, the structural line of a contour of a macroform won't change,
    //otherwise from the structural line the point, but, generally will also be deleted
    //the index on which it needs to be deleted, doesn't coincide with an index of points of a contour
    //macroforms
    MPtArray* structline = GetStructKontur();
    MInArray* structind = new MInArray();
    //we will receive indexes of coinciding points of a contour and corresponding to it
    //structural line
    CompareStructKonturWithKontur(structind);
    int index=0;
    //if in the structind array there is a 'num' element, will delete a point from the structural line
    for(int i=0; i<structind->GetSize(); i++){
        if(*structind->GetAt(i)==num){
            index = i;
            break;
        }
    }
    MPtArray* line = new MPtArray();
    line->AppendTail(structline);
    line->Delete(index);
    if(!IsCrossptLomline(line)){

        structline->ClearAll();
        structline->AppendTail(line);
        delete line;
    }
    else{
        delete line;
        InternalMessage(RLF_MESSAGE1);
        return false;
    }
    delete structind;

    //if we delete a point in the upper bound of break, indexes of extreme points of parts
    //decrease by unit, except for an index of the initial point of the upper bound
    if((num>0)&&(num<=uplastind)){
        if(vesar){
            vesar->Delete(num);
        }
        //we will delete a point from a contour
        ptar->Delete(num);
        //we will change indexes of extreme points of borders of break
        uplastind--;
        downlastind--;
        SetUpLastInd(uplastind);
        SetDownLastInd(downlastind);
        SetDownFirstInd(kolpts-2);
    }
    //if we delete a point from the lower bound of break, an index of the last point lower
    //borders decreases by unit
    else if((num>downlastind)&&(num<=kolpts-1)){
        //we will delete a point from a contour
        ptar->Delete(num);
        //we will change indexes of extreme points of borders of break
        SetDownFirstInd(kolpts-2);
    }
    //we will delete height from the array of heights of points of a contour
    heightar->Delete(num);

    if(WhatTheObject()!=ID_GIDRO){
        SetShtrihFirstPoints();
        MasterShtrihovka();
    }

    return true;
}

//==================================
//!Compares macroform contour points to points of the structural line, the corresponding macroform,
//! also returns indexes of coinciding points
bool MObryv::CompareStructKonturWithKontur(MInArray* upstructind, MInArray* /*downstructind*/)
//==================================
{
    if(!upstructind)
        return false;

    upstructind->ClearAll();

    MPtArray* kontur = GetKontur();
    MPtArray* structkontur = GetStructKontur();
    MPoint pt;
    int index = -1;
    for(int i=0; i<structkontur->GetSize(); i++){
        pt = *structkontur->GetAt(i);
        for(int j=index+1; j<kontur->GetSize(); j++){
            if(pt==*kontur->GetAt(j)){
                upstructind->AddTail(&j);
                index = j;
                break;
            }
        }
    }
    return true;
}

//==================================
//!Looks for a point with the set coordinates among macroform contour points
int MObryv::FindPoint(MPoint pt)
//==================================
{
    int index = -1;
    MPtArray* kontur = GetKontur();
    if(!kontur)
        return index;

    index = kontur->FindPoint(pt);

    return index;
}


//==================================
//! Renumbering of the array of points and array of heights of points
bool MObryv::RenumPoints(int num)
//==================================
{
    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MDuArray* har=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);

    int col = ptar->GetSize();
    if((num<0)||(num>col)) return false;

    MPtArray* novar=new MPtArray();
    MDuArray* novhar=new MDuArray();

    int i = 0;
    for(i=num; i<col; i++)
    {
        novar->AddTail(ptar->GetAt(i));
        novhar->AddTail(har->GetAt(i));
    }
    for(i=0; i<num; i++)
    {
        novar->AddTail(ptar->GetAt(i));
        novhar->AddTail(har->GetAt(i));
    }

    ptar->ClearAll();
    har->ClearAll();

    ptar->AppendTail(novar);
    har->AppendTail(novhar);

    delete novar;
    delete novhar;

    return true;
}

//==================================
//!to find a beam point of intersection which beginning of 'pt1' lies on a contour, with the lower bound of break;
/**the beam is defined by points of 'pt1' and 'pt2'; we will keep a point of intersection in 'pt';
//return code: 0 - the beam is crossed with the lower bound of break;
//             1 - the beam is crossed with the lower bound of break; and the second point of 'pt2' setting a beam lies on a contour,
//             -1 - the beam isn't crossed with the lower bound of break;
//             -2 - the contour of break or its lower bound isn't defined
//REMARK: from all crossings of a beam with a contour it is chosen such that distance from the beginning of a beam
//to a point of intersection the smallest
*/
int MObryv::GetCrossptWithDownBorder(MPoint pt1, MPoint pt2, MPoint* point)
//==================================
{
    MPtArray* ptar=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if(!ptar)
        return -2;
    MInArray* indar=(MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if(indar->GetSize()!=4)
        return -2;
    MPtArray* indptar = new MPtArray();
    int downlastind = GetDownLastInd();
    int downfirstind = GetDownFirstInd();
    int i = 0;
    for(i=downlastind; i<=downfirstind; i++){
        indptar->AddTail(ptar->GetAt(i));
    }
    MPtArray* upborder = new MPtArray();
    int upfirstind = GetUpFirstInd();
    int uplastind = GetUpLastInd();

    for(i=upfirstind; i<=uplastind; i++){
        upborder->AddTail(ptar->GetAt(i));
    }
    //We set the upborder parameter as the upper bound of break;
    //we check, that the segment received by the found beam point of intersection from the lower
    //border of break and the beginning of a beam, I didn't cross thus the upper bound of break
    //(we consider a half-open segment, excluding from it the end - the beginning of a beam,
    //since the beginning of a beam lies on the upper bound of break)
    int crosspar = GetCrossptWithKontur(indptar, pt1, pt2, point, false, upborder);

    delete indptar;
    delete upborder;

    return crosspar;
}

//==================================
//!to find a beam point of intersection which beginning of 'pt1' lies on a contour, with a break contour;
/** the beam is defined by points of 'pt1' and 'pt2'; we will keep a point of intersection in 'pt';
//the Boolean 'zamknut' variable defines whether to check crossing of a beam with a segment,
//connecting the last and zero tops, - zamknut=true;
//return code: 0 - the beam is crossed with a contour,
//             1 - the beam is crossed with a contour and the second point of 'pt2' setting a beam lies on a contour,
//             -1 - the beam isn't crossed with a contour
//REMARK 1: from all crossings of a beam with a contour it is chosen such that distance from the beginning of a beam
//          to a point of intersection the smallest
//REMARK 2: if the array of points of 'upborder' is set, we check, that the found beam point of intersection
//          with a contour of 'ptar' I didn't form the segment which is crossed together with a point of the beginning of a beam
//          with the upper bound of a contour of break (since the beginning of a beam lies on the upper bound, it is excluded
//          from a segment). We set the 'upborder' parameter as the upper bound of break by a call of this function
//          from the 'GetCrossptWithDownBorder' function.
*/
int MObryv::GetCrossptWithKontur(MPtArray* ptar, MPoint pt1, MPoint pt2, MPoint* point, bool zamknut, MPtArray* upborder)
//==================================

{
    MPoint pt;
    int index = -1;	//index of an edge of a contour with which the beam is crossed
    double dlina= -999.9;	//distance from the beginning of a beam to a point of intersection with a contour
    int kontur = -1;//parameter of crossing of a beam with a contour
    int parcross = -1;//parameter of crossing of a beam with a contour edge
    double d;//serves nothing, it is necessary to have as the input parameter of the IntersectLineWithOtrezok function
    MPtArray* crosspts = new MPtArray();

    //we will find the first crossing of a beam with a break contour edge if it is possible
    int i = 0;
    for(i=0; i<ptar->GetSize()-1; i++){
        parcross = IntersectLineWithOtrezok(ptar->GetAt(i)[0], ptar->GetAt(i+1)[0], pt1, pt2, &pt, d);
        //if the beam is crossed with a contour edge
        if(parcross==0){
            if((!upborder)||(!FindCrossOpenSegWithKontur(pt1, pt, upborder, crosspts))){
                kontur = 0;
                index = i;
                dlina = DLINASG(pt1, pt);//distance from the beginning of a beam to a point of intersection with an edge
                point[0] = pt;
                break;
            }
        }
        //if the beam is crossed with a contour edge and the second point of a beam lies on an edge
        else if(parcross==11){
            if((!upborder)||(!FindCrossOpenSegWithKontur(pt1, pt, upborder, crosspts))){
                kontur = 1;
                index = i;
                dlina = DLINASG(pt1, pt);
                point[0] = pt;
                break;
            }
        }
    }
    //we will find crossing of a beam with a break contour edge such that distance from the beginning of a beam
    //to a point of intersection with an edge the smallest
    for(i=index+1; i<ptar->GetSize()-1; i++){
        parcross = IntersectLineWithOtrezok(ptar->GetAt(i)[0], ptar->GetAt(i+1)[0], pt1, pt2, &pt, d);
        //if the beam is crossed with a contour edge
        if(parcross==0){
            if((!upborder)||(!FindCrossOpenSegWithKontur(pt1, pt, upborder, crosspts))){
                d = DLINASG(pt1, pt);
                if(d<dlina){
                    kontur = 0;
                    index = i;
                    dlina = d;
                    point[0] = pt;
                }
            }
        }
    }
    //separately we check for the segment connecting the last and zero tops of a contour
    //if didn't find crossing of a beam with a break contour in case of the closed contour...
    if(zamknut){
        if(index==-1){
            parcross = IntersectLineWithOtrezok(ptar->GetAt(ptar->GetSize()-1)[0], ptar->GetAt(0)[0], pt1, pt2, &pt, d);
            //if the beam is crossed with a contour edge
            if(parcross==0){
                if((!upborder)||(!FindCrossOpenSegWithKontur(pt1, pt, upborder, crosspts))){
                    kontur = 0;
                    point[0] = pt;
                }
            }
            //if the beam is crossed with a contour edge and the second point of a beam lies on an edge
            else if(parcross==11){
                if((!upborder)||(!FindCrossOpenSegWithKontur(pt1, pt, upborder, crosspts))){
                    kontur = 1;
                    point[0] = pt;
                }
            }
        }
        //if earlier found crossing of a beam with a break contour...
        else {
            parcross = IntersectLineWithOtrezok(ptar->GetAt(ptar->GetSize()-1)[0], ptar->GetAt(0)[0], pt1, pt2, &pt, d);

            //if the beam is crossed with a contour edge
            if(parcross==0){
                if((!upborder)||(!FindCrossOpenSegWithKontur(pt1, pt, upborder, crosspts))){
                    d = DLINASG(pt1, pt);
                    if(d<dlina){
                        kontur = 0;
                        point[0] = pt;
                    }
                }
            }
        }
    }
    //if the beam isn't crossed with a contour, a code of return-1 and we return the second point of a beam
    if(kontur!=0)
        point[0]= pt2;

    delete crosspts;
    return kontur;
}

//==================================
//!we will calculate coordinates of the beginning of all strokes
/**distance between strokes 1 millimeter on paper, therefore at the scale
//1:1000 in meters it is 1
*/
bool MObryv::SetShtrihFirstPoints()

//==================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    if((ptar->GetSize()==0)||(indar->GetSize()==0))
        return false;

    int uplastind = GetUpLastInd();//index of a final point of the upper bound of a contour of break
    double delta = 0;//shift from the beginning of an edge for the first stroke on an edge
    MPoint pt, pt1, pt2, utvector, deltapt;
    int colshtrih;
    double dist;//edge length
    double distshtrih;//length of all strokes on an edge plus a space from the beginning
    //edges on 'delta' distance
    double rasst = GetDistShtrihScaled();//distance between strokes
    //we will get the array in which we will store the beginnings of segments of shading
    MPtArray* shtrihar = new MPtArray();
    int i = 0;
    for(i=0; i<uplastind; i++){
        pt2 = ptar->GetAt(i+1)[0];
        pt1 = ptar->GetAt(i)[0];
        dist = sqrt((pt2.x - pt1.x)*(pt2.x - pt1.x) + (pt2.y - pt1.y)*(pt2.y - pt1.y));//length of the i-th edges
        if(dist>delta){
            colshtrih = (int)((dist-delta)/rasst) + 1;//number of strokes on an edge
            //we will find a shift vector in the direction from the beginning of an edge by the end on distance,
            //equal to distance between strokes
            utvector.x = (pt2.x - pt1.x)/dist*rasst;
            utvector.y = (pt2.y - pt1.y)/dist*rasst;
            //we will find a shift vector for the first stroke of an edge in the direction from the beginning
            //edges by the end on the distance equal to shift from the beginning of an edge of 'delta'
            deltapt.x = (pt2.x - pt1.x)/dist*delta;
            deltapt.y = (pt2.y - pt1.y)/dist*delta;
            pt.x = pt1.x + deltapt.x;
            pt.y = pt1.y + deltapt.y;
            for(int j=0; j<colshtrih; j++){
                shtrihar->AddTail(&pt);
                pt.x += utvector.x;
                pt.y += utvector.y;
            }

            distshtrih = rasst*(colshtrih-1) + delta;
            //we will find shift for the following edge
            delta = rasst - (dist - distshtrih);
        }
        else
            delta -= dist;

    }
    if(shtrihar->GetAt(shtrihar->GetSize()-1)[0]!=ptar->GetAt(uplastind)[0])
        shtrihar->AddTail(ptar->GetAt(uplastind));
    //we will define total number of strokes
    colshtrih = shtrihar->GetSize();
    MPtArray* shtrihovka = (MPtArray*)m_Komplekt->GetAt(OBR_SHTRIHOVKA);
    shtrihovka->SetSize(colshtrih*2);
    for(i=0; i<colshtrih*2-1; i+=2){
        shtrihovka->SetAt(i, shtrihar->GetAt(i/2));
        shtrihovka->SetAt(i+1, shtrihar->GetAt(i/2));//temporarily the end of a stroke coincides with the beginning
    }
    delete shtrihar;

    return true;
}

//==================================
//!we will calculate coordinates of the ends of all strokes
/**IDEA: we find points of intersection of the next weight segments; the ends of strokes of the fixed edge
//lie on the straight lines passing through the beginning of a stroke and the calculated point of intersection; end
//a stroke - crossing of such straight line with a break contour; if next weight segments

//are parallel among themselves, the straight line is perpendicular to an edge, the end of a stroke - crossing with a contour
*/
bool MObryv::MasterShtrihovka()
//==================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
    MPtArray* shtrihar = (MPtArray*)m_Komplekt->GetAt(OBR_SHTRIHOVKA);
    if((ptar->GetSize()==0)||(indar->GetSize()==0)||(vesar->GetSize()==0)||(shtrihar->GetSize()==0))
        return false;
    //in total strokes
    int colshtrih = shtrihar->GetSize();

    //'crosspt' - a point of intersection of "weight" straight lines, 'upshtrihpt' - the beginning of a stroke,
    //'downshtrihpt' - the end of a stroke
    MPoint crosspt, upshtrihpt, downshtrihpt, pt;

    int uplastind = GetUpLastInd();
    int shtrihind = 0;//index of the current stroke

    upshtrihpt = shtrihar->GetAt(shtrihind)[0];
    //we find the end of a stroke for each stroke for each edge of the upper bound of a contour
    for(int i=0; i<uplastind; i++){
        //if "weight" straight lines are crossed...
        if(CROSSLINES(ptar->GetAt(i)[0], vesar->GetAt(i)[0], ptar->GetAt(i+1)[0], vesar->GetAt(i+1)[0], &crosspt)){
            //for all strokes lying on an edge
            while(PTONSEG(upshtrihpt, ptar->GetAt(i)[0], ptar->GetAt(i+1)[0])){
                //point of intersection of the straight line passing through the beginning of a stroke and a point of intersection
                //"weight" straight lines, with the lower bound of a contour is the end of a stroke
                int crosspar = GetCrossptWithDownBorder(upshtrihpt, crosspt, &downshtrihpt);
                //if there are no crossings with the lower bound, a straight line point of intersection,
                //passing through the beginning of a stroke and a point of intersection of "weight" straight lines,
                //with a contour is the end of a stroke
                if(crosspar == -1)
                    crosspar = GetCrossptWithKontur(ptar, upshtrihpt, crosspt, &downshtrihpt);
                //if the "weight" straight line doesn't cross a contour anywhere, except 'upshtrihpt' point
                if(crosspar == -1)
                    downshtrihpt = upshtrihpt;
                shtrihar->SetAt(shtrihind+1, &downshtrihpt);
                shtrihind += 2;
                if(shtrihind>=colshtrih)

                    break;
                upshtrihpt = shtrihar->GetAt(shtrihind)[0];
            }
        }
        //if "weight" straight lines are parallel...
        else{
            //for all strokes lying on an edge
            while(PTONSEG(upshtrihpt, ptar->GetAt(i)[0], ptar->GetAt(i+1)[0])){
                //we look for coordinates of the end of a single vector from the beginning in a point of the beginning of a stroke,
                //the contour lying on a perpendicular to an edge of the upper bound and directed
                //in a contour
                pt = GetPerpendikular(ptar->GetAt(i), ptar->GetAt(i+1), upshtrihpt, -1);
                GetCrossptWithKontur(ptar, upshtrihpt, pt, &downshtrihpt);
                shtrihar->SetAt(shtrihind+1, &downshtrihpt);
                shtrihind += 2;
                upshtrihpt = shtrihar->GetAt(shtrihind)[0];
                if(shtrihind>=colshtrih)
                    break;
            }
        }
    }

    return true;

}


//==================================
//!we find "weight" segments
/**the first point of a segment - a break contour point, the second point
//a segment - a point of intersection of a "weight" straight line with a break contour;
//IDEA: "weight" straight lines are bisektrissa of angles in break contour tops, except extreme points
//upper bound of a contour of break; in extreme points "weight" straight lines are defined so:
//if extreme points of the top and lower borders of a contour are various, the ends of a "weight" segment -
//these are extreme points of the top and lower borders of a contour; if points coincide, we build
//a perpendicular to the zero/last edge of the upper bound of a contour also we look for its crossing
//with a contour
*/
bool MObryv::SetVesSegments()
//==================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
    int uplastind = GetUpLastInd();
    //the number of "weight" segments coincides with number of points of the upper bound of a contour
    vesar->SetSize(uplastind+1);

    //we will find "weight" segments for initial and final points of the upper bound of a contour
    //initial "weight" segment:
    MPoint pt1, pt2, pt, vespt;
    int colpt = ptar->GetSize();//number of points of a contour
    pt1 = ptar->GetAt(0);
    pt2 = ptar->GetAt(colpt-1);
    if(pt1!=pt2)
        vesar->SetAt(0, &pt2);
    else if(pt1==pt2){
        //we look for coordinates of the end of the single vector lying on a perpendicular to an initial edge
        //the upper bound of a contour and the contour directed inside
        pt = GetPerpendikular(pt1, ptar->GetAt(1), pt1, -1);
        //second end of an initial "weight" segment
        vesar->SetAt(0, &pt);

    }
    //final "weight" segment:
    pt1 = ptar->GetAt(uplastind);
    pt2 = ptar->GetAt(uplastind+1);
    if(pt1!=pt2)
        vesar->SetAt(uplastind, &pt2);
    else if(pt1==pt2){
        //we look for coordinates of the end of the single vector lying on a perpendicular to the last to an edge
        //the upper bound of a contour and the contour directed inside
        pt = GetPerpendikular(pt1, ptar->GetAt(uplastind-1), pt1, 1);
        //second end of the last "weight" segment
        vesar->SetAt(uplastind, &pt);
    }
    //we find other "weight" segments
    for(int i=1; i<uplastind; i++){
        //we look for coordinates of the end of the single vector lying on a bisector
        //in top of i of the upper bound of a contour
        pt = GetBisectrissa(ptar->GetAt(i), ptar->GetAt(i-1), ptar->GetAt(i+1));
        //we look for the second end of a "weight" segment as crossing of a "weight" straight line
        //with the lower bound of a contour
        int downcross = GetCrossptWithDownBorder(ptar->GetAt(i), pt, &vespt);
        //if with the lower bound there are no crossings, look for the second end of a "weight" segment
        //as crossing of a "weight" straight line with contour border
        if(downcross==-1)
            GetCrossptWithKontur(ptar, ptar->GetAt(i), pt, &vespt);
        vesar->SetAt(i, &vespt);
    }
    return true;
}

//==================================
//! Change of orientation of a contour of break on the return
/**REMARK: we focus break clockwise at the time of creation of a contour of break
//until a choice of the upper bound of break
*/
bool MObryv::OrientInverse()
//==================================
{
    //We receive the pointer on the array of points
    MPtArray* pta1=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* pta2=new MPtArray();
    int	m_nPoints=pta1->GetSize();
    pta2->SetSize(m_nPoints);

    //We receive the index on the array of points heights
    MDuArray* har=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    MDuArray* novhar=new MDuArray();
    int	colhpts=har->GetSize();
    novhar->SetSize(colhpts);

    //We reorient an order of points
    int i = 0;
    for (i=0; i<m_nPoints; i++){
        pta2->SetAt(i,pta1->GetAt(m_nPoints - i - 1));
    }
    //We reorient an order of heights of points
    for (i=0; i<colhpts; i++){
        novhar->SetAt(i,har->GetAt(colhpts - i - 1));
    }

    //We interchange the position of arrays
    pta1->ClearAll();
    m_Komplekt->SetAt(F_TOCHKI, (MArray*)pta2);
    free(pta1);

    har->ClearAll();
    m_Komplekt->SetAt(OBR_HEIGHTPT, (MArray*)novhar);
    free(har);

    return true;
}

//==================================
//! Drawing of all area of a macroform and its contour of color of a sheet of paper
void  MObryv::DrawMfPolygon(MPlotDevice* pDevice,MPtArray* pta)
//==================================
{
    //if the contour of a macroform isn't closed, we will close it
    MPtArray* kontur = new MPtArray();
    kontur->AppendTail(pta);
    if(*kontur->GetAt(0)!=*kontur->GetAt(kontur->GetSize()-1))
        kontur->AddTail(kontur->GetAt(0));

    //we will define color of paper
    MColor papercolor = GetKarta()->GetPaperColor();

    //we will stock up with necessary brushes and pens
    M_LOG_PEN pen;
    pen.lopnStyle = PS_NULL;
    pDevice->SavePencil();
    pDevice->SaveZapolnenie();
    //we choose a pen
    pDevice->SetPencil(&pen);


    //brush
    pDevice->SetZapolnenie(&papercolor);

    pDevice->Polygon(kontur->GetHead(), kontur->GetSize());

    delete kontur;

    //we restore a system brush
    pDevice->RestorePencil();
    //we restore a system pen
    pDevice->RestoreZapolnenie();
}

//==================================
//! Drawing of the upper bound of break, slope, retaining wall
void  MObryv::DrawUpBorder(MPlotDevice* pDevice,MPtArray* pta)
//==================================
{
    //we receive the index on the array of indexes of the beginning and end of the top
    //and lower break contour borders
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);

    //we will get the array where we will store the line for drawing
    MPtArray* ptar = new MPtArray();

    //if the top and lower borders of break are defined
    if(indar->GetSize() == 4){
        int upfirstind = indar->GetAt(0)[0];//index of the beginning of the upper bound
        int uplastind = indar->GetAt(1)[0];//index of the end of the upper bound

        int ind = upfirstind;
        MPoint pt;

        //we form the upper bound of break
        while(ind<=uplastind){
            pt = pta->GetAt(ind)[0];
            ptar->AddTail(&pt);
            ind++;
        }

        //we take the line for drawing of the upper bound and strokes of a break/slope
        MLinia* linia = (MLinia*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(0);

        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        MDrawing::DrawLinia(this,pDevice,linia,ptar);
        //we clean the array of points
        ptar->ClearAll();
    }

    delete ptar;
}

//==================================
//! Drawing of shading of break, slope
void  MObryv::DrawShtrihovka(MPlotDevice* pDevice)
//==================================
{
    //we take the line for drawing of strokes of a break/slope
    MLinia* linia = (MLinia*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(0);

    //it is necessary to update the array of strokes for a case of change of distance between strokes
    SetShtrihFirstPoints();
    MasterShtrihovka();

    MPtArray* shtrihar = (MPtArray*)m_Komplekt->GetAt(OBR_SHTRIHOVKA);
    int colshtrih = shtrihar->GetSize();
    MPtArray temp;
    temp.SetSize(2);
    for(int i=0; i<colshtrih-1;i+=2){
        temp.SetAt(0,shtrihar->GetAt(i));
        temp.SetAt(1,shtrihar->GetAt(i+1));
        MDrawing::DrawLinia(this,pDevice,linia,&temp);
    }
}

//==================================
//! Drawing of relative depth or height of the top/lower
//! edges of steep, slope, retaining wall
void  MObryv::DrawMfPodpis(MPlotDevice* /*pDevice*/)
//==================================
{
}

//==================================
//! Break drawing
void  MObryv::Draw(MPlotDevice* pDevice)
//==================================
{
    //we draw a macroform in full volume only if its construction is fully complete
    if(*m_dbarray.GetAt(0)=="обрыв"){
        //we receive the pointer on break contour points
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
        //we draw all area of break in the color of a sheet of paper
        DrawMfPolygon(pDevice,pta);

        //we draw the upper bound of break
        DrawUpBorder(pDevice,pta);

        //we draw shading for break
        DrawShtrihovka(pDevice);

        //we draw marks of relative depth or height of upper/bottom edge of break
        DrawMfPodpis(pDevice);
    }
    //at incomplete construction we draw only a macroform contour
    else
        pDevice->DrawServantPolyLine((MPtArray*)m_Komplekt->GetAt(F_TOCHKI),2,MColor(0,0xff,0));
}

//==================================
//! drawing of a marking of break
void MObryv::DrawTracker(MPlotDevice* pDevice,TrackerState state)
//==================================
{
    double d1Pix = pDevice->LogicToBum(1);
    double d2Pix = d1Pix*2;
    double d3Pix = d1Pix*3;
    double d5Pix = d1Pix*5;
    double d7Pix = d1Pix*7;

    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);

    switch (state)
    {
    //usual tracker
    case normal:
    {
        if((*m_dbarray.GetAt(0)=="контур")||(*m_dbarray.GetAt(0)=="неопределена")){
            pDevice->DrawServantPolyLine(pta,6,MColor(0,0,0));
            pDevice->DrawServantPolyLine(pta,4,MColor(0xff,0xff,0xff));
        }
        if(((*m_dbarray.GetAt(0)=="обрыв")||(*m_dbarray.GetAt(0)=="яма"))||
                (*m_dbarray.GetAt(0)=="второй контур")){

            int uplastind = GetUpLastInd();

            MPtArray* upptar = new MPtArray();
            int i = 0;
            for(i=0; i<=uplastind; i++)
                upptar->AddTail(pta->GetAt(i));

            pDevice->DrawServantPolyLine(upptar,6,MColor(0,0,0));
            pDevice->DrawServantPolyLine(upptar,4,MColor(0xff,0xff,0xff));
            delete upptar;

            MPtArray* downptar = new MPtArray();
            MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);

            if(indar->GetSize()==4){
                int downfirstind = GetDownFirstInd();
                int downlastind = GetDownLastInd();

                for(int i=downlastind; i<=downfirstind; i++)
                    downptar->AddTail(pta->GetAt(i));
            }
            else{

                for(int i=uplastind+1; i<pta->GetSize(); i++)
                    downptar->AddTail(pta->GetAt(i));
            }

            //if we allocate hydroobject, the lower bound as well as top
            if(WhatTheObject()==ID_GIDRO){
                pDevice->DrawServantPolyLine(downptar,6,MColor(0,0,0));
                pDevice->DrawServantPolyLine(downptar,4,MColor(0xff,0xff,0xff));
            }
            else{
                pDevice->DrawServantPolyLine(downptar,6,MColor(0,0,0));
                pDevice->DrawServantPolyLine(downptar,4,MColor(0xff,0xff,0xff));
                pDevice->DrawServantPolyLine(downptar,2,MColor(0,0,0));
            }
            delete downptar;

            //we transfer to logical coordinates
            //also we draw a tracker of shading of break for each "weight" segment
            MPtArray* dwaptar = new MPtArray();
            dwaptar->SetSize(2);
            MPtArray* shtrihar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
            int colshtrih = shtrihar->GetSize();
            for(i=0; i<colshtrih;i++){
                dwaptar->SetAt(0,pta->GetAt(i));
                dwaptar->SetAt(1,shtrihar->GetAt(i));
                pDevice->DrawServantPolyLine(dwaptar,6,MColor(0,0,0));
                pDevice->DrawServantPolyLine(dwaptar,4,MColor(0xff,0xff,0xff));
            }

            //we draw trackers of tops of a contour for the lower bound of a contour;
            //in case of hydroobject for all tops
            if(WhatTheObject()==ID_GIDRO){
                for (int i = 0; i < pta->GetSize(); i++) {
                    pDevice->PatBlt(pta->GetAt(i)->x - d2Pix, pta->GetAt(i)->y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                    pDevice->PatBlt(pta->GetAt(i)->x - d1Pix, pta->GetAt(i)->y - d1Pix, d3Pix, d3Pix, WHITENESS);
                }
            }
            else{
                for (int i = 0; i < pta->GetSize()-uplastind-1; i++) {
                    pDevice->PatBlt(pta->GetAt(i)->x - d2Pix, pta->GetAt(i)->y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                    pDevice->PatBlt(pta->GetAt(i)->x - d1Pix, pta->GetAt(i)->y - d1Pix, d3Pix, d3Pix, WHITENESS);
                }
            }
            delete dwaptar;
        }
        if(*m_dbarray.GetAt(0)=="неопределена"){
            pDevice->DrawServantPolyLine(pta,4,MColor(0,0,0));
            pDevice->DrawServantPolyLine(pta,3,MColor(222,43,3));
        }
        if(*m_dbarray.GetAt(0)=="выбор индексов"){
            pDevice->DrawServantPolyLine(pta,4,MColor(0,0,0));
            pDevice->DrawServantPolyLine(pta,3,MColor(0,0xff,0));
        }
        if(*m_dbarray.GetAt(0)=="верхняя граница"){
            int uplastind = GetUpLastInd();
            int downlastind = GetDownLastInd();

            MPtArray* upptar = new MPtArray();
            int i = 0;
            for(i=0; i<=uplastind; i++)
                upptar->AddTail(pta->GetAt(i));

            pDevice->DrawServantPolyLine(upptar,4,MColor(0,0,0));
            pDevice->DrawServantPolyLine(upptar,3,MColor(0,0xff,0));

            delete upptar;

            MPtArray* downptar = new MPtArray();
            for(i=downlastind; i<pta->GetSize(); i++)
                downptar->AddTail(pta->GetAt(i));

            pDevice->DrawServantPolyLine(downptar,4,MColor(0,0,0));
            pDevice->DrawServantPolyLine(downptar,3,MColor(0,0xff,0));

            delete downptar;

            //we draw a tracker of the edges and tops corresponding to indexes of the beginning and end
            //top and lower edges of a contour of break
            //tracker of edges
            MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
            int colind = indar->GetSize();
            MString macroforma = WhatTheObject();
            if((colind==4)&&(macroforma!=ID_YAMA)){
                pDevice->DrawServantLine(pta->GetAt(GetUpFirstInd()), pta->GetAt(GetDownFirstInd()), MColor(0,0,0),4);
                pDevice->DrawServantLine(pta->GetAt(GetUpFirstInd()), pta->GetAt(GetDownFirstInd()), MColor(238, 225, 32),3);
                pDevice->DrawServantLine(pta->GetAt(GetUpLastInd()), pta->GetAt(GetDownLastInd()), MColor(0,0,0),4);
                pDevice->DrawServantLine(pta->GetAt(GetUpLastInd()), pta->GetAt(GetDownLastInd()), MColor(238, 225, 32),3);

                MPoint cpt;
                //Tracer of the tops
                cpt = *pta->GetAt(GetUpFirstInd());
                pDevice->PatBlt(cpt.x - d2Pix, cpt.y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                pDevice->PatBlt(cpt.x - d1Pix, cpt.y - d1Pix, d3Pix, d3Pix, WHITENESS);
                cpt = *pta->GetAt(GetDownFirstInd());
                pDevice->PatBlt(cpt.x - d2Pix, cpt.y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                pDevice->PatBlt(cpt.x - d1Pix, cpt.y - d1Pix, d3Pix, d3Pix, WHITENESS);
                cpt = *pta->GetAt(GetUpLastInd());
                pDevice->PatBlt(cpt.x - d2Pix, cpt.y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                pDevice->PatBlt(cpt.x - d1Pix, cpt.y - d1Pix, d3Pix, d3Pix, WHITENESS);
                cpt = *pta->GetAt(GetDownLastInd());
                pDevice->PatBlt(cpt.x - d2Pix, cpt.y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                pDevice->PatBlt(cpt.x - d1Pix, cpt.y - d1Pix, d3Pix, d3Pix, WHITENESS);
            }
        }
        break;
    }
    default: break;
    }

    //we draw a marker for splines bindings of formulas
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);

    for(int tt=privs->GetSize()-1;tt>-1;tt-=1)
    {
        MPriv* priv = privs->GetAt(tt);
        MPtArray* pta=new MPtArray();
        //we will receive spline coordinates
        priv->GetDrawArray(pta);
        unsigned char privtype = priv->GetType();
        if(privtype != SPLINE_PRIVYAZKA){
            pDevice->DrawServantPolyLine(pta,3,MColor(0,0,0));
            pDevice->DrawServantPolyLine(pta,1,MColor(0xff,0xff,0xff));
            // points: transformation to logical coordinates
            for(int j = 0;j<pta->GetSize();j++){
                pDevice->PatBlt(pta->GetAt(j)->x - d3Pix, pta->GetAt(j)->y - d3Pix, d7Pix, d7Pix, BLACKNESS);
                pDevice->PatBlt(pta->GetAt(j)->x - d2Pix, pta->GetAt(j)->y - d2Pix, d5Pix, d5Pix, WHITENESS);

            }
        }
        else if(privtype == SPLINE_PRIVYAZKA){
            MPtArray* trackpts = new MPtArray();
            //forms the broken line from 10 points corresponding to a spline
            FormLomlineBySimpleSpline(*pta->GetAt(0),*pta->GetAt(1),*pta->GetAt(2),
                                      *pta->GetAt(3),10,trackpts);
            //we draw a spline
            pDevice->DrawServantPolyLine(trackpts,3,MColor(0,0,0));
            pDevice->DrawServantPolyLine(trackpts,1,MColor(0xff,0xff,0xff));
            //we draw the first bar
            pDevice->DrawServantLine(*pta->GetAt(0), *pta->GetAt(2), MColor(0, 0, 0), 3);
            pDevice->DrawServantLine(*pta->GetAt(0), *pta->GetAt(2), MColor(255, 255, 255), 1);
            //we draw the second bar
            pDevice->DrawServantLine(*pta->GetAt(1), *pta->GetAt(3), MColor(0, 0, 0), 3);
            pDevice->DrawServantLine(*pta->GetAt(1), *pta->GetAt(3), MColor(255, 255, 255), 1);
            //we draw spline points
            // points: transformation to logical cords
            for(int j = 0;j<pta->GetSize();j++){
                pDevice->PatBlt(pta->GetAt(j)->x - d3Pix, pta->GetAt(j)->y - d3Pix, d7Pix, d7Pix, BLACKNESS);
                pDevice->PatBlt(pta->GetAt(j)->x - d2Pix, pta->GetAt(j)->y - d2Pix, d5Pix, d5Pix, WHITENESS);
            }
            delete trackpts;
        }
        delete pta;
    }
}

//===============================================================================
// MOtkos
//===============================================================================
//================================================
//!Constructor by default
MOtkos::MOtkos()
//================================================
{
}

//==================================
//! cloning
MObject* MOtkos::Clone()
//==================================
{
    MOtkos* pClone=new MOtkos();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}

//==============================================
//!We join the service classifier
void MOtkos::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pOtkZnak;
    m_kkod = SERV_OBJECT_OTK;
}

//==================================
//!gives out length of a small stroke for a slope
float MOtkos::GetDlinaShtrihaForOtkos()
//==================================
{
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    float dlinashtriha = ((MSbStretch*)linia->GetAt(0))->m_sbpause;
    return dlinashtriha;
}

//==================================
//!sets length of a small stroke for a slope
bool MOtkos::SetDlinaShtrihaForOtkos(float dlina)
//==================================
{
    //length of a small stroke can't be negative size
    if(dlina<0)
        return false;

    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    ((MSbStretch*)linia->GetAt(0))->m_sbpause = dlina;
    return true;
}


//==================================
//!we will calculate coordinates of the ends of all strokes for a slope
/**IDEA: we find coordinates of the ends of all strokes for a slope,
//starting with zero we reduce every second stroke to length 2mm,
//starting with the first we draw every second stroke length on 1mm less than the corresponding
//a stroke for break
*/
bool MOtkos::MasterShtrihovka()
//==================================
{
    MObryv::MasterShtrihovka();

    ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

    MPtArray* shtrihar = (MPtArray*)m_Komplekt->GetAt(OBR_SHTRIHOVKA);
    double dlina;
    int colshtrih = shtrihar->GetSize();
    //we take length of a short stroke
    double smalldlina = GetDlinaShtrihaForOtkos();
    //we will receive length of a short stroke multiplied by true large-scale coefficient
    double dlinaForOtkos = ConvertSizeToReal((MTopobject*)this,smalldlina);
    //starting with zero we draw every second stroke the short
    //if length adjacent with short a long stroke is less than doubled length of a short stroke,
    //we draw a short stroke with a length equal to a half of a long stroke
    int i = 0;
    for(i=0; i<shtrihar->GetSize(); i=i+4){
        //if the short stroke finishes shading...
        if(i+2>=colshtrih)
            dlina = DLINASG(shtrihar->GetAt(i-2)[0], shtrihar->GetAt(i-1)[0]);
        //if the long stroke finishes shading...
        else
            dlina = DLINASG(shtrihar->GetAt(i+2)[0], shtrihar->GetAt(i+3)[0]);

        MPoint pt;
        //if length adjacent with short a long stroke is less than doubled length of a short stroke...
        if(dlina<=dlinaForOtkos*2){
            pt = ExtendVector(shtrihar->GetAt(i)[0], shtrihar->GetAt(i+1)[0], dlina/2);
        }
        //otherwise...
        else{
            pt = ExtendVector(shtrihar->GetAt(i)[0], shtrihar->GetAt(i+1)[0], dlinaForOtkos);
        }
        shtrihar->SetAt(i+1, &pt);

    }
    for(i=2; i<shtrihar->GetSize(); i=i+4){
        dlina = DLINASG(shtrihar->GetAt(i)[0], shtrihar->GetAt(i+1)[0]);
        if(dlina>dlinaForOtkos/2){
            //we will reduce a long stroke by the distance equal to the doubled thickness of the line of a stroke,
            //to note the lower bound of a slope points
            double cut = GEOM(((MLinia*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(0))->m_thickness);
            cut = cut*2;
            dlina = dlina - cut;
            MPoint pt = ExtendVector(shtrihar->GetAt(i)[0], shtrihar->GetAt(i+1)[0], dlina);
            shtrihar->SetAt(i+1, &pt);
        }

    }

    return true;

}

//==================================
//! Drawing of the lower bound of a slope
void  MOtkos::DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta)
//==================================
{
    //we receive the index on the array of indexes of the beginning and end of the top
    //and lower slope contour borders
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);

    //we will get the array where we will store the line for drawing
    MPtArray* ptar = new MPtArray();

    //if the top and lower borders of a slope are defined
    if(indar->GetSize() == 4){
        int downfirstind = indar->GetAt(2)[0];//index of the beginning of the lower bound
        int downlastind = indar->GetAt(3)[0];//index of the end of the lower bound

        int ind;
        MPoint pt;

        //in case of a slope it is necessary to draw the lower bound points
        MSbStretch* sbstretch = (MSbStretch*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(1);
        //if the second layer in the array of layers of linear language not a broach of a symbol,
        //the lower bound we have no opportunity to draw
        if(sbstretch->GetType()!=IDL_SBSTRETCH) return;
        ind = downlastind;

        //we form the lower bound of a slope
        while(ind<=downfirstind){
            pt = pta->GetAt(ind)[0];
            ptar->AddTail(&pt);
            ind++;
        }
        //we draw the lower bound
        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        MDrawing::DrawSbStretch(this,pDevice,sbstretch,ptar,ptar,true);

        //we clean the array of points
        ptar->ClearAll();
    }
    delete ptar;
}

//==================================
//! Drawing of a slope
void  MOtkos::Draw(MPlotDevice* pDevice)
//==================================
{
    //we draw a macroform in full volume only if its construction is fully complete
    if(*m_dbarray.GetAt(0)=="обрыв"){
        //we receive the pointer on slope contour points
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
        //we draw all area of a slope in the color of a sheet of paper
        DrawMfPolygon(pDevice,pta);
        //we draw the upper bound of a slope
        DrawUpBorder(pDevice,pta);
        //we draw shading for a slope
        DrawShtrihovka(pDevice);
        //we draw marks of relative depth or height of upper/bottom edge of a slope
        DrawMfPodpis(pDevice);
        //we draw the bottom bound of a slope
        DrawDownBorder(pDevice,pta);
    }
    //at incomplete construction we draw only a macroform contour
    else
        //DrawKontur(pView,pDC);
        pDevice->DrawServantPolyLine((MPtArray*)m_Komplekt->GetAt(F_TOCHKI),2,MColor(0,0xff,0));
}

//===============================================================================
// MYama
//===============================================================================
//================================================
//!Constructor by default
MYama::MYama()
//================================================
{
    F_TOCHKI = 10;
    MPtArray* downstructline = new MPtArray();
    m_Komplekt->InsertAfter(OBR_STRUCTLINE, (MArray*)downstructline);
}

//==================================
//! cloning
MObject* MYama::Clone()
//==================================
{
    MYama* pClone=new MYama();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}

//==============================================
//!We join the service classifier

void MYama::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pYamZnak;
    m_kkod = SERV_OBJECT_YAM;
}

//==================================
//!we find "weight" segments: the first point of a segment - a point of the top contour of a hole, the second point
//! a segment - a point of intersection of a "weight" straight line with a break contour;
bool MYama::SetVesSegments()
//IDEA: we build all "weight" segments, as internal "weight" segments for break
//==================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
    int uplastind = GetUpLastInd();
    //the number of "weight" segments are one less than number of points of the upper bound of a contour,
    //since weight segments in zero and last points coincide
    vesar->SetSize(uplastind);

    //we will find a "weight" segment for an initial (final) point of the upper bound of a contour
    MPoint pt, vespt;
    //we look for coordinates of the end of the single vector lying on a bisector
    //in top 0 upper bounds of a contour
    pt = GetBisectrissa(ptar->GetAt(0), ptar->GetAt(uplastind-1), ptar->GetAt(1));
    //we look for the second end of a "weight" segment as crossing of a "weight" straight line
    //with the lower bound of a contour
    int downcross = GetCrossptWithDownBorder(ptar->GetAt(0), pt, &vespt);
    //if with the lower bound there are no crossings, look for the second end of a "weight" segment
    //as crossing of a "weight" straight line with contour border
    if(downcross==-1)
        GetCrossptWithKontur(ptar, ptar->GetAt(0), pt, &vespt);
    vesar->SetAt(0, &vespt);

    //we will find a "weight" segment for a penultimate point of the upper bound of a contour
    //we look for coordinates of the end of the single vector lying on a bisector
    //in top uplastind-1 upper bounds of a contour
    pt = GetBisectrissa(ptar->GetAt(uplastind-1), ptar->GetAt(uplastind-2), ptar->GetAt(0));
    //we look for the second end of a "weight" segment as crossing of a "weight" straight line
    //with the lower bound of a contour
    downcross = GetCrossptWithDownBorder(ptar->GetAt(uplastind-1), pt, &vespt);
    //if with the lower bound there are no crossings, look for the second end of a "weight" segment
    //as crossing of a "weight" straight line with contour border
    if(downcross==-1)
        GetCrossptWithKontur(ptar, ptar->GetAt(uplastind-1), pt, &vespt);
    vesar->SetAt(uplastind-1, &vespt);

    //we find other "weight" segments
    for(int i=1; i<uplastind-1; i++){
        //we look for coordinates of the end of the single vector lying on a bisector
        //in top i-th upper bounds of a contour
        pt = GetBisectrissa(ptar->GetAt(i), ptar->GetAt(i-1), ptar->GetAt(i+1));
        //we look for the second end of a "weight" segment as crossing of a "weight" straight line
        //with the lower bound of a contour
        downcross = GetCrossptWithDownBorder(ptar->GetAt(i), pt, &vespt);
        //if with the lower bound there are no crossings, look for the second end of a "weight" segment
        //as crossing of a "weight" straight line with contour border
        if(downcross==-1)
            GetCrossptWithKontur(ptar, ptar->GetAt(i), pt, &vespt);
        vesar->SetAt(i, &vespt);
    }
    return true;
}


//==================================
//!Designs a hole contour in the form of two structural lines
bool MYama::MakeStructKontur(MPtArray* structkonturfirst, MPtArray* structkonturlast)
//==================================
{
    if((!structkonturfirst)||(!structkonturlast))
        return false;
    structkonturfirst->ClearAll();
    structkonturlast->ClearAll();
    //if the contour isn't defined
    MPtArray* kontur = GetKontur();
    if(!kontur)
        return false;
    //if one of indexes isn't defined
    int upfirstind = GetUpFirstInd();
    int uplastind = GetUpLastInd();
    int downfirstind = GetDownFirstInd();
    int downlastind = GetDownLastInd();
    if((upfirstind==-1)||(uplastind==-1)||(downfirstind==-1)||(downlastind==-1))
        return false;
    //structural line of top of a hole
    int i = 0;
    for(i=upfirstind; i<=uplastind; i++){
        structkonturfirst->AddTail(kontur->GetAt(i));
    }
    //structural line of a bottom of a hole
    for(i=downlastind; i<=downfirstind; i++){
        structkonturlast->AddTail(kontur->GetAt(i));
    }
    return true;
}

//==================================
//!returns a contour of bottom edge of a hole in the form of the structural line
MPtArray* MYama::GetDownStructKontur()
//==================================
{
    MPtArray* structline = (MPtArray*)m_Komplekt->GetAt(OBR_DOWNSTRUCTLINE);
    return structline;
}

//==================================
//!sets the structural line for bottom edge of a hole
bool  MYama::SetDownStructKontur(MPtArray* structkontur)
//==================================
{
    if(!structkontur)
        return false;
    m_Komplekt->SetAt(OBR_DOWNSTRUCTLINE, (MArray*)structkontur);
    return true;
}

//==================================
//!Compares points of contours of a hole to points structural lines, the corresponding top and a bottom
//! holes, also returns indexes of coinciding points for each structural line
bool MYama::CompareStructKonturWithKontur(MInArray* upstructind, MInArray* downstructind)
//==================================
{
    if((!upstructind)||(!downstructind))
        return false;

    upstructind->ClearAll();
    downstructind->ClearAll();

    MPtArray* kontur = GetKontur();
    MPtArray* upstructkontur = GetStructKontur();
    MPtArray* downstructkontur = GetDownStructKontur();
    MPoint pt;
    int index = -1;
    int i = 0;
    for(i=0; i<upstructkontur->GetSize(); i++){
        pt = *upstructkontur->GetAt(i);
        for(int j=index+1; j<kontur->GetSize(); j++){
            if(pt==*kontur->GetAt(j)){
                upstructind->AddTail(&j);
                index = j;
                break;
            }
        }
    }

    for(i=0; i<downstructkontur->GetSize(); i++){
        pt = *downstructkontur->GetAt(i);
        for(int j=index+1; j<kontur->GetSize(); j++){
            if(pt==*kontur->GetAt(j)){
                downstructind->AddTail(&j);
                index = j;
                break;
            }
        }
    }
    return true;
}

//==================================
//!Removal of a point from a hole contour
/**REMARK: 1) numbering of points of the upper bound of a hole begins with a zero index,
//an index of the first point of the lower bound of a hole of kolpts-1, where kolpts -
//number of points of a contour of a hole;
//2) removal of a point from a contour of a hole leads to change of indexes of extreme points
//the upper and lower bound of a hole, except the initial point of the upper bound of a hole;
*/
bool MYama::DeletePointFromKontur(int num)
//==================================
{
    //we will receive the index on the array of marks of heights of points of a contour of a hole
    MDuArray* heightar=(MDuArray*)m_Komplekt->GetAt(OBR_HEIGHTPT);
    if(!heightar)
        return false;
    MPtArray* ptar=GetKontur();
    if(!ptar)
        return false;

    int uplastind = GetUpLastInd(); //index of the last point of the upper bound of a hole
    int downlastind = GetDownLastInd(); //index of the initial point of the lower bound of a hole
    //if indexes of the top and lower parts aren't defined
    if((uplastind==-1)||(downlastind==-1)){
        //we cause function of the parent
        MTopobject::DeletePoint(num);
        return true;
    }

    MPtArray* vesar = (MPtArray*)m_Komplekt->GetAt(OBR_VESSEGMENTS);
    if(!vesar)
        return false;

    int kolpts = GetPointsNumber(); //number of points of a contour of a hole
    //it is impossible to delete extreme points of borders of break;
    //the index shouldn't overstep the bounds of indexes of points of a contour
    if((num<0)||(num>kolpts-1))
        return false;

    if((uplastind==3)&&(num<=3)){
        InternalMessage(RLF_MESSAGE2);//"The upper bound of a hole doesn't may contain less than three points!"
        return false;
    }

    if((downlastind==kolpts-4)&&(num>=kolpts-4)){
        InternalMessage(RLF_MESSAGE3);//"The bottom bound of a hole doesn't may contain less than three points!"
        return false;
    }

    //if we delete the situational point which isn't participating in designing relief
    //surfaces, the structural line of a contour of a hole won't change,
    //otherwise from the structural line the point, but, generally, will also be deleted
    //the index on which it needs to be deleted, doesn't coincide with an index of points of a contour of a hole
    MPtArray* upstructline = GetStructKontur();
    MPtArray* downstructline = GetDownStructKontur();
    MInArray* upstructind = new MInArray();
    MInArray* downstructind = new MInArray();
    //we will receive indexes of coinciding points of contours of top and a bottom of a hole
    //and corresponding to them structural lines
    CompareStructKonturWithKontur(upstructind, downstructind);
    int index = *upstructind->GetTail();
    //we will delete a point from the structural line of top of a hole
    if(num<index){
        //if in the structind array there is a 'num' element, will delete a point from the structural line
        for(int i=0; i<upstructind->GetSize(); i++){
            if(*upstructind->GetAt(i)==num){
                index = i;
                break;
            }
        }
        delete upstructind;
        delete downstructind;

        MPtArray* kontur = new MPtArray();
        kontur->AppendTail(upstructline);
        kontur->Delete(index);
        //if the beginning and the end of the structural line coincide, it is necessary to change the end of the line
        if(index==0)
            kontur->SetAt(kontur->GetSize()-1, kontur->GetAt(0));
        //we will check that contours of top and a bottom of a hole weren't crossed
        if((LomlineInRegion(kontur, downstructline))||(LomlineInRegion(downstructline, kontur))){
            upstructline->ClearAll();
            upstructline->AppendTail(kontur);
            delete kontur;
        }
        else{
            delete kontur;
            InternalMessage(RLF_MESSAGE4);//"Contours of a hole can't be crossed!"
            return false;
        }
    }
    //we will delete a point from the structural line of a bottom of a hole
    else{
        //if in the structind array there is a 'num' element, will delete a point from the structural line
        for(int i=0; i<downstructind->GetSize(); i++){
            if(*downstructind->GetAt(i)==num){
                index = i;
                break;
            }
        }
        delete upstructind;
        delete downstructind;

        MPtArray* kontur = new MPtArray();
        kontur->AppendTail(downstructline);
        kontur->Delete(index);
        //if the beginning and the end of the structural line coincide, it is necessary to change the end of the line
        if(index==0)
            kontur->SetAt(kontur->GetSize()-1, kontur->GetAt(0));
        //we will check that contours of top and a bottom of a hole weren't crossed
        if((LomlineInRegion(kontur, upstructline))||(LomlineInRegion(upstructline, kontur))){
            downstructline->ClearAll();
            downstructline->AppendTail(kontur);
            delete kontur;
        }
        else{
            delete kontur;
            InternalMessage("Hole Contours Can't be Self-Crossing!");
            return false;
        }
    }

    //if we delete a point in the upper bound of break, indexes of extreme points of parts
    //decrease by unit, except for an index of the initial point of the upper bound
    if((num>=0)&&(num<=uplastind)){
        //we will delete a point from a contour
        ptar->Delete(num);
        //we will change indexes of extreme points of borders of break
        uplastind--;
        downlastind--;
        SetUpLastInd(uplastind);
        SetDownLastInd(downlastind);
        SetDownFirstInd(kolpts-2);
        //if we delete the initial point of top of a hole
        if(num==0)
            ptar->SetAt(uplastind, ptar->GetAt(0));
        vesar->Delete(num);
        SetShtrihFirstPoints();
        MasterShtrihovka();
    }
    //if we delete a point from the lower bound of break, an index of the last point lower
    //borders decreases by unit
    else if((num>=downlastind)&&(num<=kolpts-1)){
        //we will delete a point from a contour
        ptar->Delete(num);
        //we will change indexes of extreme points of borders of break
        SetDownFirstInd(kolpts-2);
        //if we delete the initial point of bottom of a hole
        if(num==downlastind)
            ptar->SetAt(kolpts-2, ptar->GetAt(downlastind));
    }

    return true;
}

//==================================
//! Drawing of a contour of a hole
void  MYama::DrawKontur(MPlotDevice* pDevice)
//==================================
{
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);
    //if the first contour isn't created yet...
    if(indar->GetSize()<2)
        pDevice->DrawServantPolyLine((MPtArray*)m_Komplekt->GetAt(F_TOCHKI),2,MColor(0,0xff,0));
    else{
        //we receive the pointer on break contour points
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
        //we form and draw the top contour
        int upfirstind = GetUpFirstInd();
        int uplastind = GetUpLastInd();
        pDevice->DrawServantPolyLine(pta->GetAt(upfirstind),uplastind-upfirstind+1,2,MColor(0,0xff,0));
        //we form and draw the bottom contour
        int downfirstind = uplastind+1;
        int downlastind = pta->GetSize()-1;
        pDevice->DrawServantPolyLine(pta->GetAt(downfirstind),downlastind-downfirstind+1,2,MColor(0,0xff,0));
    }
}

//==================================
//! Drawing of a hole
void  MYama::Draw(MPlotDevice* pDevice)
//==================================
{
    //we draw a macroform in full volume only if its construction is fully complete
    if(*m_dbarray.GetAt(0)=="яма"){
        //we receive the pointer on hole contour points
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);

        //for a hole we draw everything, as for a slope, only in addition
        //we fill a hole interior
        MPtArray* upptar = new MPtArray();
        for(int i=0; i<=GetUpLastInd(); i++)
            upptar->AddTail(pta->GetAt(i));
        DrawMfPolygon(pDevice,upptar);
        delete upptar;

        //we draw all area of a hole in the color of a sheet of paper
        DrawMfPolygon(pDevice,pta);

        //we draw the upper bound of a hole
        DrawUpBorder(pDevice,pta);

        //we draw shading for a slope
        DrawShtrihovka(pDevice);

        //we draw marks of relative depth of a hole
        DrawMfPodpis(pDevice);

        //we draw the bottom bound of a hole
        DrawDownBorder(pDevice,pta);
    }
    //at incomplete construction we draw only a macroform contour
    else
        DrawKontur(pDevice);
}

//===============================================================================
// MPodpStenka
//===============================================================================
//================================================
//!Constructor by default
MPodpStenka::MPodpStenka()
//================================================
{
}

//==================================
//! cloning
MObject* MPodpStenka::Clone()
//==================================
{
    MPodpStenka* pClone=new MPodpStenka();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}

//==============================================
//!We join the service classifier
void MPodpStenka::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pPstZnak;
    m_kkod = SERV_OBJECT_PST;
}

//==================================
//!in case of a retaining wall gives out notch width
float MPodpStenka::GetShirinaZubtsa()
//==================================
{
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    float shirinazubtsa = ((MSbStretch*)linia->GetAt(0))->m_sbpause;
    return shirinazubtsa;
}

//==================================
//!sets notch width for a retaining wall
bool MPodpStenka::SetShirinaZubtsa(float shirina)
//==================================
{
    //width of a notch can't be negative size
    if(shirina<0)
        return false;
    MPrArray* linia = ((MLinYaz*)(m_pZnak->GetYaz(IDY_SECOND)->m_object))->m_linia;
    ((MSbStretch*)linia->GetAt(0))->m_sbpause = shirina;
    return true;
}

//==================================
//! Drawing of a notch of shading for a retaining wall
//! 'ind' - an index of the drawn notch in the array of shading of a retaining wall
void  MPodpStenka::DrawZubets(MPlotDevice* pDevice,int ind)
//==================================
{
    MPtArray* shtrihar = (MPtArray*)m_Komplekt->GetAt(OBR_SHTRIHOVKA);
    int colshtrih = shtrihar->GetSize();

    if(colshtrih==0)
        return;

    //on a place of zero and last strokes we don't draw notches
    if((ind<2)||(ind>=colshtrih-3))
        return;

    MColor color;
    //if on a code of color of the line found in the dictionary color, we choose it,
    //otherwise we take color by default
    m_pLay->GetKarta()->GetDictColorValue(((MLinia*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(2))->m_colorkod, color);

    //brush
    pDevice->SaveZapolnenie();
    pDevice->SetZapolnenie(&color);

    MPoint* ipoints=new MPoint[3];

    double dist = GetShirinaZubtsa()/2;

    ipoints[0] = ExtendVector(shtrihar->GetAt(ind)[0],shtrihar->GetAt(ind-2)[0],dist);
    ipoints[1] = ExtendVector(shtrihar->GetAt(ind)[0],shtrihar->GetAt(ind+2)[0],dist);
    ipoints[2] = *shtrihar->GetAt(ind+1);
    pDevice->Polygon(ipoints, 3);

    //we restore a system brush
    pDevice->RestoreZapolnenie();
    delete[] ipoints;
}

//==================================
//! Drawing of teeth of shading for a retaining wall
void  MPodpStenka::DrawZubetsiForPodpStenka(MPlotDevice* pDevice)

//==================================
{
    //it is necessary to update the array of strokes for a case of change of distance between teeth
    SetShtrihFirstPoints();
    MasterShtrihovka();
    MPtArray* shtrihar = (MPtArray*)m_Komplekt->GetAt(OBR_SHTRIHOVKA);
    int colshtrih = shtrihar->GetSize();

    if(colshtrih==0)
        return;

    for(int i=2; i<=colshtrih-4; i+=2)
        DrawZubets(pDevice,i);
}

//==================================
//! Drawing of the lower bound of a retaining wall
void  MPodpStenka::DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta)
//==================================
{
    //we receive the index on the array of indexes of the beginning and end of the top
    //and lower break contour borders
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);

    //we will get the array where we will store the line for drawing
    MPtArray* ptar = new MPtArray();

    //if the top and lower borders of break are defined
    if(indar->GetSize() == 4){
        int downfirstind = indar->GetAt(2)[0];//index of the beginning of the lower bound
        int downlastind = indar->GetAt(3)[0];//index of the end of the lower bound

        int ind;
        MPoint pt;
        //in case of a slope it is necessary to draw the lower bound points
        MLinia* plinia;
        plinia = (MLinia*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(1);
        ind = downlastind;

        //we form the lower bound of a retaining wall
        while(ind<=downfirstind){
            pt = pta->GetAt(ind)[0];
            ptar->AddTail(&pt);
            ind++;
        }

        //we draw the lower bound
        //DrawLinia(pView,pDC,plinia,ptar);
        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        MDrawing::DrawLinia(this,pDevice,plinia,ptar);

        //we clean the array of points
        ptar->ClearAll();
    }
    delete ptar;
}

//==================================
//! Drawing of a retaining wall
void  MPodpStenka::Draw(MPlotDevice* pDevice)
//==================================
{
    //we draw a macroform in full volume only if its construction is fully complete
    if(*m_dbarray.GetAt(0)=="обрыв"){
        //we receive the pointer on points of a contour of a retaining wall
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);

        //we draw all area of a retaining wall in the color of a sheet of paper
        DrawMfPolygon(pDevice,pta);

        //we draw the upper bound of a retaining wall
        DrawUpBorder(pDevice,pta);

        //we draw notches for a retaining wall
        DrawZubetsiForPodpStenka(pDevice);

        //we draw marks of height of upper/bottom edge of a retaining wall
        DrawMfPodpis(pDevice);

        //we draw the bottom bound of a retaining wall
        DrawDownBorder(pDevice, pta);
    }
    //at incomplete construction we draw only a macroform contour
    else
        pDevice->DrawServantPolyLine((MPtArray*)m_Komplekt->GetAt(F_TOCHKI),2,MColor(0,0xff,0));
}

//===============================================================================
// MGidro
//===============================================================================
//================================================
//!Constructor by default
MGidro::MGidro()
//================================================
{
}

//==================================
//! cloning
MObject* MGidro::Clone()
//==================================
{
    MGidro* pClone=new MGidro();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}

//==============================================
//!We join the service classifier

void MGidro::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pGdrZnak;
    m_kkod = SERV_OBJECT_GDR;
}


//==================================
//! Drawing of the lower bound of hydroobject
void  MGidro::DrawDownBorder(MPlotDevice* pDevice,MPtArray* pta)
//==================================
{
    //we receive the pointer on the array of indexes of the beginning and end of the top
    //and lower hydroobject contour borders
    MInArray* indar = (MInArray*)m_Komplekt->GetAt(OBR_INDEX);

    //we will get the array where we will store the line for drawing
    MPtArray* ptar = new MPtArray();

    //if the top and lower borders of hydroobject are defined
    if(indar->GetSize() == 4){
        int downfirstind = indar->GetAt(2)[0];//index of the beginning of the lower bound
        int downlastind = indar->GetAt(3)[0];//index of the end of the lower bound

        int ind;
        MPoint pt;

        MLinia* plinia;
        plinia = (MLinia*)((MLinYaz*)m_pZnak->GetYaz()->m_object)->m_linia->GetAt(0);
        ind = downlastind;

        //we form the lower bound of hydroobject
        while(ind<=downfirstind){
            pt = pta->GetAt(ind)[0];
            ptar->AddTail(&pt);
            ind++;
        }

        //we draw the lower bound
        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        MDrawing::DrawLinia(this,pDevice,plinia,ptar);

        //we clean the array of points
        ptar->ClearAll();
    }
    delete ptar;
}

//==================================
//! Drawing of hydroobjects
void  MGidro::Draw(MPlotDevice* pDevice)
//==================================
{
    //we draw a macroform in full volume only if its construction is fully complete
    if(*m_dbarray.GetAt(0)=="обрыв"){
        //we receive the pointer on points of a contour of a retaining wall
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);

        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        MZalivka* zal = (MZalivka*)((MPloYaz*)m_pZnak->GetYaz()->m_object)->m_plast->GetAt(0);
        MDrawing::DrawZalivka(this,pDevice, zal, pta);

        //we draw the upper bound of hydroobject
        DrawUpBorder(pDevice,pta);

        //we draw the bottom bound of hydroobject
        DrawDownBorder(pDevice,pta);
    }
    //at incomplete construction we draw only a macroform contour
    else{
        pDevice->DrawServantPolyLine((MPtArray*)m_Komplekt->GetAt(F_TOCHKI),2,MColor(0,0xff,0));

    }
}

//===============================================================================
//===============================================================================
//==================================
//! MReliefLayer - relief layer
MReliefLayer::MReliefLayer()
//==================================
{
    //	MLayer::MLayer();
    m_name = IDS_RELIEF_LAYER  ;//"relief layer"
    m_sort = IDS_RELIEF_LAYER ;//"relief layer"

    m_segments = new MSgArray();
    m_triangles = new MTrArray();
    m_structlines = new MArArray();
    m_RelKonturPts = new MPtArray();
    m_RelKonturHgt = new MDuArray();

    // for initialization of dialogue of drawing of horizontals
    m_doppts = 10;
    m_izotype = 2;
    m_shag = 0.5;
    m_viewtrg = true;
    m_viewizolines = true;
    m_viewizobats = false;
    m_viewkonturs = false;
    m_viewRelKontur = false;
    m_viewstructlines = false;
    m_viewhptnom = false;
    m_viewhptvalue = false;

    m_minheight = m_maxheight = -1000000;

    m_pKlsHeightpt = NULL;
    m_pKlsIzoline = NULL;
    m_pKlsDopizo = NULL;
    m_pKlsUtlizo = NULL;
    m_pKlsObryv = NULL;
    m_pKlsOtkos = NULL;
    m_pKlsPodpStenka = NULL;
    m_pKlsGidro = NULL;
    m_pKlsYama = NULL;
    m_pKlsDepthpt = NULL;
    m_pKlsIzobata = NULL;

    m_qkodHeightpt = "";
    m_qkodIzoline = "";
    m_qkodDopizo = "";
    m_qkodUtlizo = "";
    m_qkodObryv = "";
    m_qkodOtkos = "";
    m_qkodPodpStenka = "";
    m_qkodGidro = "";
    m_qkodYama = "";
    m_qkodDepthpt = "";
    m_qkodIzobata = "";
}

//!==================================
MReliefLayer::MReliefLayer(MString key)
//==================================
{
    //	MLayer::MLayer();
    m_name = key;
    m_sort = IDS_RELIEF_LAYER;//"relief layer"

    m_segments = new MSgArray();
    m_triangles = new MTrArray();
    m_structlines = new MArArray();
    m_RelKonturPts = new MPtArray();
    m_RelKonturHgt = new MDuArray();

    // for initialization of dialogue of drawing of horizontals
    m_doppts = 10;
    m_izotype = 2;
    m_shag = 0.5;
    m_viewtrg = true;
    m_viewizolines = true;
    m_viewizobats = false;
    m_viewkonturs = false;
    m_viewRelKontur = false;
    m_viewstructlines = false;

    m_minheight = m_maxheight = -1000000;

    m_pKlsHeightpt = NULL;
    m_pKlsIzoline = NULL;
    m_pKlsDopizo = NULL;
    m_pKlsUtlizo = NULL;
    m_pKlsObryv = NULL;

    m_pKlsOtkos = NULL;
    m_pKlsPodpStenka = NULL;
    m_pKlsGidro = NULL;
    m_pKlsYama = NULL;
    m_pKlsDepthpt = NULL;
    m_pKlsIzobata = NULL;

    m_qkodHeightpt = "";
    m_qkodIzoline = "";
    m_qkodDopizo = "";
    m_qkodUtlizo = "";
    m_qkodObryv = "";
    m_qkodOtkos = "";
    m_qkodPodpStenka = "";
    m_qkodGidro = "";
    m_qkodYama = "";
    m_qkodDepthpt = "";
    m_qkodIzobata = "";
}

//!==================================
MReliefLayer::~MReliefLayer()
//==================================
{
    OBITER it;
    while(!m_oblist.empty())
        m_oblist.pop_front();

    it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        delete (*it);
        it++;
    }
    m_heightpoints.clear();

    it = m_izolines.begin();
    while(it != m_izolines.end())
    {
        delete (*it);
        it++;
    }
    m_izolines.clear();

    it = m_depthpoints.begin();
    while(it != m_depthpoints.end())
    {
        delete (*it);
        it++;
    }
    m_depthpoints.clear();

    it = m_izobats.begin();
    while(it != m_izobats.end())
    {
        delete (*it);
        it++;
    }
    m_izobats.clear();

    it = m_dopizolines.begin();
    while(it != m_dopizolines.end())
    {
        delete (*it);
        it++;
    }
    m_dopizolines.clear();

    it = m_kontlist.begin();
    while(it != m_kontlist.end())
    {
        delete (*it);
        it++;
    }
    m_kontlist.clear();

    it = m_macroforms.begin();
    while(it != m_macroforms.end())
    {
        delete (*it);
        it++;
    }
    m_macroforms.clear();

    it = m_gidroobjects.begin();
    while(it != m_gidroobjects.end())
    {
        delete (*it);

        it++;
    }
    m_gidroobjects.clear();

    it = m_decorative.begin();
    while(it != m_decorative.end())
    {
        delete (*it);
        it++;
    }
    m_decorative.clear();

    m_segments->ClearAll();
    m_triangles->ClearAll();
    m_RelKonturPts->ClearAll();
    m_RelKonturHgt->ClearAll();

}

//!==================================
MReliefLayer::MReliefLayer(const MRect& bounds)
//==================================
{
    //MLayer::MLayer();
    m_bounds = bounds;
    m_name = IDS_RELIEF_LAYER;//relief layer
    m_sort = IDS_RELIEF_LAYER;//relief layer

    m_segments = new MSgArray();
    m_triangles = new MTrArray();
    m_structlines = new MArArray();
    m_RelKonturPts = new MPtArray();
    m_RelKonturHgt = new MDuArray();

    // for initialization of dialogue of drawing of horizontals
    m_doppts = 10;
    m_izotype = 2;
    m_shag = 0.5;
    m_viewtrg = true;
    m_viewizolines = true;
    m_viewizobats = false;
    m_viewkonturs = false;
    m_viewRelKontur = false;
    m_viewstructlines = false;

    m_minheight = m_maxheight = -1000000;

    m_pKlsHeightpt = NULL;
    m_pKlsIzoline = NULL;
    m_pKlsDopizo = NULL;

    m_pKlsUtlizo = NULL;
    m_pKlsObryv = NULL;
    m_pKlsOtkos = NULL;
    m_pKlsPodpStenka = NULL;
    m_pKlsGidro = NULL;
    m_pKlsYama = NULL;
    m_pKlsDepthpt = NULL;
    m_pKlsIzobata = NULL;

    m_qkodHeightpt = "";
    m_qkodIzoline = "";
    m_qkodDopizo = "";
    m_qkodUtlizo = "";
    m_qkodObryv = "";
    m_qkodOtkos = "";
    m_qkodPodpStenka = "";
    m_qkodGidro = "";
    m_qkodYama = "";
    m_qkodDepthpt = "";
    m_qkodIzobata = "";
}

//!==================================
bool MReliefLayer::WriteBin(FILE* h_nuxfile,  int version)
//==================================
{
    //we read out a marker of reading the file before reading a layer
    fseek(h_nuxfile, 0, SEEK_CUR);
    unsigned int p_begnuxfile = ftell(h_nuxfile);

    MLayer::MasterWrite(h_nuxfile,version);

    unsigned short		KolObj;
    //	int			param = 0;

    switch( version ) {
    //version 0
    case 0:
        //record of quad-code of objects of the classifier - basic for automatic
        //creation of the main objects of lists of a layer of a relief
        WriteString(h_nuxfile,  version, m_qkodHeightpt);
        WriteString(h_nuxfile,  version, m_qkodIzoline);
        WriteString(h_nuxfile,  version, m_qkodDopizo);
        WriteString(h_nuxfile,  version, m_qkodUtlizo);
        WriteString(h_nuxfile,  version, m_qkodObryv);
        WriteString(h_nuxfile,  version, m_qkodOtkos);
        WriteString(h_nuxfile,  version, m_qkodPodpStenka);
        WriteString(h_nuxfile,  version, m_qkodGidro);
        WriteString(h_nuxfile,  version, m_qkodYama);
        WriteString(h_nuxfile,  version, m_qkodDepthpt);
        WriteString(h_nuxfile,  version, m_qkodIzobata);

        //total number of objects
        KolObj = m_heightpoints.size() + m_izolines.size() + m_dopizolines.size() +
                m_kontlist.size() + m_macroforms.size() + m_gidroobjects.size() +
                m_depthpoints.size() + m_izobats.size() + m_decorative.size() +
                m_segments->GetSize() + m_triangles->GetSize();
        fwrite(&KolObj, 2, 1, h_nuxfile);

        //record of objects
        //			MKarta*   pKarta = m_pKarta;
        MString panetext;
        int KolInd = 0;
        unsigned short kol;
        //calculation and writing of quantity of marks of heights

        kol = 0;
        OBITER it = m_heightpoints.begin();
        while (it != m_heightpoints.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_heightpoints.begin();
        //cycle of search of objects
        while(it != m_heightpoints.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;

            //indication
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of quantity of horizontals
        it = m_izolines.begin();
        kol = 0;
        while (it != m_izolines.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_izolines.begin();
        while (it != m_izolines.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;

            //indication
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }
        //calculation and record of quantity of additional horizontals
        kol = 0;
        it = m_dopizolines.begin();
        while (it != m_dopizolines.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_dopizolines.begin();
        while (it != m_dopizolines.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of quantity of contours of visibility
        kol = 0;
        it = m_kontlist.begin();
        while (it != m_kontlist.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_kontlist.begin();
        while (it != m_kontlist.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of quantity of macroforms of a relief
        kol = 0;
        it = m_macroforms.begin();
        while (it != m_macroforms.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_macroforms.begin();
        while (it != m_macroforms.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of amount of hydroobjects
        kol = 0;
        it = m_gidroobjects.begin();
        while (it != m_gidroobjects.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_gidroobjects.begin();
        while (it != m_gidroobjects.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of amount of decorative objects
        kol = 0;
        it = m_decorative.begin();
        while (it != m_decorative.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_decorative.begin();
        while (it != m_decorative.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				double	ind = floor(((double) KolInd)/((double) KolObj) * 100);
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of quantity of marks of depths
        kol = 0;
        it = m_depthpoints.begin();
        while (it != m_depthpoints.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_depthpoints.begin();
        while (it != m_depthpoints.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				double	ind = floor(((double) KolInd)/((double) KolObj) * 100);
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }

        //calculation and record of quantity of isobaths
        kol = 0;
        it = m_izobats.begin();
        while (it != m_izobats.end())
        {
            kol++;
            it++;
        }
        fwrite(&kol, 2, 1, h_nuxfile);
        it = m_izobats.begin();

        while (it != m_izobats.end())
        {
            (*it)->WriteBin(h_nuxfile,  version);
            KolInd++;
            //indication
            //				double	ind = floor(((double) KolInd)/((double) KolObj) * 100);
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
            it++;
            //				pDoc->SetPaneText(0,(char *)panetext);
        }
        //we remember the array of segments
        m_segments->WriteBin(h_nuxfile,  version);
        //we remember the array of triangles
        m_triangles->WriteBin(h_nuxfile,  version);
        //we remember a coverage contour
        m_RelKonturPts->WriteBin(h_nuxfile,  version);
        //we remember coverage contour heights
        m_RelKonturHgt->WriteBin(h_nuxfile,  version);
        //Data recording for dialogue of creation of horizontals
        fwrite(&m_doppts, 2, 1, h_nuxfile);
        fwrite(&m_shag, 8, 1, h_nuxfile);
        fwrite(&m_izotype, 2, 1, h_nuxfile);
        fwrite(&m_viewtrg, 1, 1, h_nuxfile);
        fwrite(&m_viewizolines, 1, 1, h_nuxfile);
        fwrite(&m_viewizobats, 1, 1, h_nuxfile);
        //flag of visibility of structural lines
        fwrite(&m_viewstructlines, 1, 1, h_nuxfile);
        //flag of visibility of a contour of a relief
        fwrite(&m_viewRelKontur, 1, 1, h_nuxfile);
        //conclusion flag in a formula of number of marks of heights
        fwrite(&m_viewhptnom, 1, 1, h_nuxfile);
        //conclusion flag in a formula of height of marks of heights
        fwrite(&m_viewhptvalue, 1, 1, h_nuxfile);
        // Record of the minimum and maximum heights of a layer of a relief
        fwrite(&m_minheight, sizeof(double), 1, h_nuxfile);
        fwrite(&m_maxheight, sizeof(double), 1, h_nuxfile);

        //we read out a marker of reading the file after reading a layer
        fseek(h_nuxfile, 0, SEEK_CUR);
        //we calculate layer length in bytes as a difference of indications of a marker of reading the file
        //before reading a layer
        int raznitsa = ftell(h_nuxfile) - p_begnuxfile;
        //we will get up instead of record of real length of a layer in bytes -
        //this variable registers right after the layer identifier
        fseek(h_nuxfile, -raznitsa+3, SEEK_CUR);
        //we write down layer length in bytes
        fwrite(&raznitsa, 4, 1, h_nuxfile);
        //we come back to a point of the end of reading a layer
        fseek(h_nuxfile, raznitsa-7, SEEK_CUR);

    }

    return true;
}

//!==================================
bool MReliefLayer::ReadBin(FILE* h_nuxfile,  int version)
//==================================
{
    MLayer::MasterRead(h_nuxfile,version);

    char		  id_file[] = "   ";
    unsigned short    ObCnt=0,  kolvo=0;
    MTopobject*   pTpo;
    MIzoline*	  pIzo;
    MObject*      pObj;
    MHeightPoint* pHtPt;

    switch( version ) {
    //version 0
    case 0:
        //reading quad-code of objects of the classifier - basic for automatic
        //creation of the main objects of lists of a layer of a relief
        m_qkodHeightpt = ReadString(h_nuxfile,  version);
        m_qkodIzoline = ReadString(h_nuxfile,  version);
        m_qkodDopizo = ReadString(h_nuxfile,  version);
        m_qkodUtlizo = ReadString(h_nuxfile,  version);
        m_qkodObryv = ReadString(h_nuxfile,  version);
        m_qkodOtkos = ReadString(h_nuxfile,  version);
        m_qkodPodpStenka = ReadString(h_nuxfile,  version);
        m_qkodGidro = ReadString(h_nuxfile,  version);
        m_qkodYama = ReadString(h_nuxfile,  version);
        m_qkodDepthpt = ReadString(h_nuxfile,  version);
        m_qkodIzobata = ReadString(h_nuxfile,  version);

        //indication
        MString	panetext;
        //			MKarta*   pKarta = m_pKarta;
        fseek(h_nuxfile, 0, SEEK_CUR);

        int p = ftell(h_nuxfile);


        fseek(h_nuxfile, 0, SEEK_END);
        int	filesize = ftell(h_nuxfile);
        fseek(h_nuxfile, p, SEEK_SET);

        double	ind = floor(((double) p)/((double) filesize) * 100);
        panetext.Format("Чтение карты: %d %%", (int) ind);
        GetKarta()->SetProgress((int)ind);
        GetKarta()->ShowProgress();
        //			pDoc->SetPaneText(0,(char *)panetext);

        //we read amount of objects
        //	ObCnt= 0;
        fread(&ObCnt, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&ObCnt, sizeof(ObCnt) );

        //cycle of reading marks of heights
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        unsigned short i = 0;
        for(i=0; i<kolvo; i++){
            fseek(h_nuxfile, 3, SEEK_CUR);
            pHtPt = new MHeightPoint();
            pHtPt->m_pLay = this;
            m_heightpoints.push_back(pHtPt);
            pHtPt->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pHtPt->ChangeClass(pHtPt->m_kkod);

        }
        //cycle of reading horizontals
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            //horizontal type: the main \reinforced
            //reading identifier of object
            fseek(h_nuxfile, 3, SEEK_CUR);
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            m_izolines.push_back(pIzo);
            pIzo->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pIzo->ChangeClass(pIzo->m_kkod);
        }
        //cycle of reading additional horizontals
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            fseek(h_nuxfile, 3, SEEK_CUR);
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            m_dopizolines.push_back(pIzo);
            pIzo->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pIzo->ChangeClass(pIzo->m_kkod);
        }
        //cycle of reading contours of visibility
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            fseek(h_nuxfile, 3, SEEK_CUR);
            pTpo = new MTopobject();
            pTpo->m_pLay = this;
            m_kontlist.push_back(pTpo);
            pTpo->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pTpo->ChangeClass(pTpo->m_kkod);
        }
        //cycle of reading macroforms of a relief
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            //reading identifier of object
            fread(&id_file, 3, 1, h_nuxfile);
            pObj = (*fpCreateObjectByType)(id_file);
            pObj->m_pLay = this;
            m_macroforms.push_back(pObj);
            pObj->ReadBin(h_nuxfile,  version);
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                ((MTopobject*)pObj)->ChangeClass(((MTopobject*)pObj)->m_kkod);
        }
        //cycle of reading hydroobjects
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            //reading identifier of object
            fread(&id_file, 3, 1, h_nuxfile);
            pObj = (*fpCreateObjectByType)(id_file);
            pObj->m_pLay = this;
            m_gidroobjects.push_back(pObj);
            pObj->ReadBin(h_nuxfile,  version);
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                ((MTopobject*)pObj)->ChangeClass(((MTopobject*)pObj)->m_kkod);
        }
        //cycle of reading dekorativ
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            fseek(h_nuxfile, 3, SEEK_CUR);
            pTpo = new MTopobject();
            pTpo->m_pLay = this;
            m_decorative.push_back(pTpo);
            pTpo->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pTpo->ChangeClass(pTpo->m_kkod);
        }
        //cycle of reading marks of depths
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            fseek(h_nuxfile, 3, SEEK_CUR);
            pHtPt = new MHeightPoint();
            pHtPt->m_pLay = this;
            m_depthpoints.push_back(pHtPt);
            pHtPt->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pHtPt->ChangeClass(pHtPt->m_kkod);
        }
        //cycle of reading isobaths
        //kolvo= 0;
        fread(&kolvo, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            fseek(h_nuxfile, 3, SEEK_CUR);
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            m_izobats.push_back(pIzo);
            pIzo->ReadBin(h_nuxfile,  version);
            //We are connected to the classifier
            pIzo->ChangeClass(pIzo->m_kkod);
        }
        //reading array of segments
        m_segments->ReadBin(h_nuxfile,  version);
        //reading array of triangles
        m_triangles->ReadBin(h_nuxfile,  version);
        //reading points of a contour of coverage
        m_RelKonturPts->ReadBin(h_nuxfile,  version);
        //reading heights of a contour of coverage
        m_RelKonturHgt->ReadBin(h_nuxfile,  version);
        //number of additional points
        //			m_doppts= 0;
        fread(&m_doppts, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_doppts, sizeof(m_doppts) );
        //relief section height
        m_shag= 0;
        fread(&m_shag, 8, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_shag, sizeof(m_shag) );
        //type of a curve horizontal
        m_izotype= 0;
        fread(&m_izotype, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_izotype, sizeof(m_izotype) );
        //flag of visibility of triangles

        fread(&m_viewtrg, 1, 1, h_nuxfile);
        //flag of visibility of horizontals
        fread(&m_viewizolines, 1, 1, h_nuxfile);
        //flag of visibility of isobaths
        fread(&m_viewizobats, 1, 1, h_nuxfile);
        //flag of visibility of structural lines
        fread(&m_viewstructlines, 1, 1, h_nuxfile);
        //flag of visibility of a contour of a relief
        fread(&m_viewRelKontur, 1, 1, h_nuxfile);
        //conclusion flag in a formula of number of marks of heights
        fread(&m_viewhptnom, 1, 1, h_nuxfile);
        //conclusion flag in a formula of height of marks of heights
        fread(&m_viewhptvalue, 1, 1, h_nuxfile);
        //minimum height of a relief
        //			m_minheight= 0;
        fread(&m_minheight, 8, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_minheight, sizeof(m_minheight) );
        //maximum height of a relief
        //			m_maxheight= 0;
        fread(&m_maxheight, 8, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_maxheight, sizeof(m_maxheight) );

        //we will set indexes on the main types of objects of a layer of a relief
        SetMainReliefTypes();
    }
    return true;
}

//!==================================
bool MReliefLayer::ReadBin(MFile* file,  int version)
//==================================
{
    MLayer::MasterRead(file,version);

    char		  id_file[] = "   ";
    unsigned short    ObCnt=0, kolvo=0;
    MTopobject*   pTpo;
    MIzoline*	  pIzo;
    MObject*      pObj;
    MHeightPoint* pHtPt;


    switch( version ) {
    //version 0
    case 0:
        //reading quad-code of objects of the classifier - basic for automatic
        //creation of the main objects of lists of a layer of a relief
        m_qkodHeightpt = ReadString(file,  version);
        m_qkodIzoline = ReadString(file,  version);
        m_qkodDopizo = ReadString(file,  version);
        m_qkodUtlizo = ReadString(file,  version);
        m_qkodObryv = ReadString(file,  version);
        m_qkodOtkos = ReadString(file,  version);
        m_qkodPodpStenka = ReadString(file,  version);
        m_qkodGidro = ReadString(file,  version);
        m_qkodYama = ReadString(file,  version);
        m_qkodDepthpt = ReadString(file,  version);
        m_qkodIzobata = ReadString(file,  version);

        //indication
        MString	panetext;

        file->SetPosition(0, MAP_SEEK_CUR);

        int p = file->GetPosition();

        int	filesize = file->GetSize();

        double	ind = floor(((double) p)/((double) filesize) * 100);
        panetext.Format("Чтение карты: %d %%", (int) ind);
        GetKarta()->SetProgress((int)ind);
        GetKarta()->ShowProgress();
        //			pDoc->SetPaneText(0,(char *)panetext);

        //we read amount of objects
        //			ObCnt = 0;
        file->GetData(&ObCnt, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&ObCnt, sizeof(ObCnt) );

        //cycle of reading marks of heights
        //kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        unsigned short i = 0;
        for(i=0; i<kolvo; i++){
            file->SetPosition(3, MAP_SEEK_CUR);
            pHtPt = new MHeightPoint();
            pHtPt->m_pLay = this;
            m_heightpoints.push_back(pHtPt);
            pHtPt->ReadBin(file,  version);
            //We are connected to the classifier
            pHtPt->ChangeClass(pHtPt->m_kkod);
        }
        //cycle of reading horizontals  //kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            //horizontal type: the main \reinforced
            //reading identifier of object
            file->SetPosition(3, MAP_SEEK_CUR);
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            m_izolines.push_back(pIzo);
            pIzo->ReadBin(file,  version);
            //We are connected to the classifier
            pIzo->ChangeClass(pIzo->m_kkod);
            if(!m_pKarta->FindKkodInLoadFilter(pIzo->m_kkod)) {    // vipa 2005.03.30
                m_izolines.pop_back();
                delete  pIzo;
            }
        }
        //cycle of reading additional horizontals //kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            file->SetPosition(3, MAP_SEEK_CUR);
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            m_dopizolines.push_back(pIzo);
            pIzo->ReadBin(file,  version);
            //We are connected to the classifier
            pIzo->ChangeClass(pIzo->m_kkod);
            if(!m_pKarta->FindKkodInLoadFilter(pIzo->m_kkod)) {    // vipa 2005.03.30
                m_izolines.pop_back();
                delete  pIzo;
            }
        }
        //cycle of reading contours of visibility //kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            file->SetPosition(3, MAP_SEEK_CUR);
            pTpo = new MTopobject();
            pTpo->m_pLay = this;
            m_kontlist.push_back(pTpo);
            pTpo->ReadBin(file,  version);
            //We are connected to the classifier
            pTpo->ChangeClass(pTpo->m_kkod);
        }
        //cycle of reading macroforms of a relief //kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            //reading identifier of object
            file->GetData(&id_file, 3, 1);
            pObj = (*fpCreateObjectByType)(id_file);
            pObj->m_pLay = this;
            m_macroforms.push_back(pObj);
            pObj->ReadBin(file,  version);
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                ((MTopobject*)pObj)->ChangeClass(((MTopobject*)pObj)->m_kkod);
        }
        //cycle of reading hydroobjects // kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            //reading identifier of object
            file->GetData(&id_file, 3, 1);
            pObj = (*fpCreateObjectByType)(id_file);
            pObj->m_pLay = this;
            m_gidroobjects.push_back(pObj);
            pObj->ReadBin(file,  version);
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                ((MTopobject*)pObj)->ChangeClass(((MTopobject*)pObj)->m_kkod);
        }
        //cycle of reading dekorativ // kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            file->SetPosition(3, MAP_SEEK_CUR);
            pTpo = new MTopobject();
            pTpo->m_pLay = this;
            m_decorative.push_back(pTpo);
            pTpo->ReadBin(file,  version);
            //We are connected to the classifier
            pTpo->ChangeClass(pTpo->m_kkod);
        }
        //cycle of reading marks of depths // kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            file->SetPosition(3, SEEK_CUR);
            pHtPt = new MHeightPoint();
            pHtPt->m_pLay = this;
            m_depthpoints.push_back(pHtPt);
            pHtPt->ReadBin(file,  version);
            //We are connected to the classifier
            pHtPt->ChangeClass(pHtPt->m_kkod);
        }
        //cycle of reading isobaths // kolvo = 0;
        file->GetData(&kolvo, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&kolvo, sizeof(kolvo) );
        for(i=0; i<kolvo; i++){
            file->SetPosition(3, MAP_SEEK_CUR);
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            m_izobats.push_back(pIzo);
            pIzo->ReadBin(file,  version);
            //We are connected to the classifier
            pIzo->ChangeClass(pIzo->m_kkod);
        }
        //reading array of segments
        m_segments->ReadBin(file,  version);
        //reading array of triangles
        m_triangles->ReadBin(file,  version);
        //reading points of a contour of coverage
        m_RelKonturPts->ReadBin(file,  version);
        //reading heights of a contour of coverage
        m_RelKonturHgt->ReadBin(file,  version);
        //number of additional points
        //	m_doppts= 0;
        file->GetData(&m_doppts, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&m_doppts, sizeof(m_doppts) );
        //relief section height
        //	m_shag= 0;
        file->GetData(&m_shag, 8, 1);
        if (reverseReadFlag) reverseValue((char*)&m_shag, sizeof(m_shag) );
        //type of a curve horizontal
        //	m_izotype= 0;
        file->GetData(&m_izotype, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&m_izotype, sizeof(m_izotype) );
        //flag of visibility of triangles
        file->GetData(&m_viewtrg, 1, 1);
        //flag of visibility of horizontals
        file->GetData(&m_viewizolines, 1, 1);
        //flag of visibility of isobaths
        file->GetData(&m_viewizobats, 1, 1);
        //flag of visibility of structural lines
        file->GetData(&m_viewstructlines, 1, 1);
        //flag of visibility of a contour of a relief
        file->GetData(&m_viewRelKontur, 1, 1);
        //conclusion flag in a formula of number of marks of heights
        file->GetData(&m_viewhptnom, 1, 1);
        //conclusion flag in a formula of height of marks of heights
        file->GetData(&m_viewhptvalue, 1, 1);
        //minimum height of a relief
        //	m_minheight= 0;
        file->GetData(&m_minheight, 8, 1);
        if (reverseReadFlag) reverseValue((char*)&m_minheight, sizeof(m_minheight) );
        //maximum height of a relief
        //	m_maxheight= 0;
        file->GetData(&m_maxheight, 8, 1);
        if (reverseReadFlag) reverseValue((char*)&m_maxheight, sizeof(m_maxheight) );

        //we will set indexes on the main types of objects of a layer of a relief
        SetMainReliefTypes();
    }
    return true;
}


//==================================
//!o fill line arrays for a MTuneClassDialog-dialogue call in the document for
//! main types of objects of a layer of a relief
bool MReliefLayer::FillTuneMassivsForMainReliefTypes(MStArray* pWhatTheObjArray,
                                                     MStArray* pReturnKodArray, MStArray* pNameArray)
//==================================
{
    if((!pWhatTheObjArray)||(!pReturnKodArray)||(!pNameArray))
        return false;

    pWhatTheObjArray->ClearAll();
    pReturnKodArray->ClearAll();
    pNameArray->ClearAll();

    //we fill the array of line identifiers of class types of objects
    MString str = ID_HEIGHTPOINT;

    pWhatTheObjArray->AddTail(&str);
    str = ID_IZOLINE;
    pWhatTheObjArray->AddTail(&str);
    str = ID_IZOLINE;
    pWhatTheObjArray->AddTail(&str);
    str = ID_IZOLINE;
    pWhatTheObjArray->AddTail(&str);
    str = ID_OBRYV;
    pWhatTheObjArray->AddTail(&str);
    str = ID_OTKOS;
    pWhatTheObjArray->AddTail(&str);
    str = ID_PODPSTENKA;
    pWhatTheObjArray->AddTail(&str);
    str = ID_GIDRO;
    pWhatTheObjArray->AddTail(&str);
    str = ID_YAMA;
    pWhatTheObjArray->AddTail(&str);

    //we fill the array of quad-code of objects
    str = m_qkodHeightpt;
    pReturnKodArray->AddTail(&str);
    str = m_qkodIzoline;
    pReturnKodArray->AddTail(&str);
    str = m_qkodDopizo;
    pReturnKodArray->AddTail(&str);
    str = m_qkodUtlizo;
    pReturnKodArray->AddTail(&str);
    str = m_qkodObryv;
    pReturnKodArray->AddTail(&str);
    str = m_qkodOtkos;
    pReturnKodArray->AddTail(&str);
    str = m_qkodPodpStenka;
    pReturnKodArray->AddTail(&str);
    str = m_qkodGidro;
    pReturnKodArray->AddTail(&str);
    str = m_qkodYama;
    pReturnKodArray->AddTail(&str);

    //we fill an array of names of objects
    str = IDS_ALTITUDE_MARKS;// "Altitude Marks"
    pNameArray->AddTail(&str);
    str = IDS_HORIZONTAL;//"Horizontal"
    pNameArray->AddTail(&str);
    str = IDS_ADT_HORIZONTAL;//"Additional Horizontal"
    pNameArray->AddTail(&str);
    str = IDS_FAT_HORIZONTAL;//"Thicker Horizontal"
    pNameArray->AddTail(&str);
    str = IDS_OBRYV;//"Steep"
    pNameArray->AddTail(&str);
    str = IDS_OTKOS;//"Slope"
    pNameArray->AddTail(&str);
    str = IDS_PODPSTENKA_FLL;//"Supporting Wall"
    pNameArray->AddTail(&str);
    str = IDS_GIDRO;//"Hydrographical Object"
    pNameArray->AddTail(&str);
    str = IDS_YAMA_VAL;//"Hole/Arbor"
    pNameArray->AddTail(&str);

    return true;
}

//==================================
//!to set pointers of the main types of objects of a layer of a relief
//! on classifier signs
/** ENTRANCE: the array of quad-code for the main types of objects of a layer of a relief
*/
void MReliefLayer::SetMainReliefTypes(MStArray* pReturnKodArray)
//==================================
{
    if(pReturnKodArray!=NULL){
        //we appropriate quad-code to standard objects
        if(pReturnKodArray->GetSize()>0)
            m_qkodHeightpt = pReturnKodArray->GetAt(0)[0];
        if(pReturnKodArray->GetSize()>1)
            m_qkodIzoline = pReturnKodArray->GetAt(1)[0];
        if(pReturnKodArray->GetSize()>2)
            m_qkodDopizo = pReturnKodArray->GetAt(2)[0];
        if(pReturnKodArray->GetSize()>3)
            m_qkodUtlizo = pReturnKodArray->GetAt(3)[0];
        if(pReturnKodArray->GetSize()>4)
            m_qkodObryv = pReturnKodArray->GetAt(4)[0];
        if(pReturnKodArray->GetSize()>5)
            m_qkodOtkos = pReturnKodArray->GetAt(5)[0];
        if(pReturnKodArray->GetSize()>6)
            m_qkodPodpStenka = pReturnKodArray->GetAt(6)[0];
        if(pReturnKodArray->GetSize()>7)
            m_qkodGidro = pReturnKodArray->GetAt(7)[0];
        if(pReturnKodArray->GetSize()>8)
            m_qkodYama = pReturnKodArray->GetAt(8)[0];
    }

    MZnak* pZnak;

    pZnak = GetKlsZnak(m_qkodHeightpt);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_HEIGHTPOINT))
        m_pKlsHeightpt = pZnak;
    else
        m_pKlsHeightpt = NULL;

    pZnak = GetKlsZnak(m_qkodDepthpt);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_HEIGHTPOINT))
        m_pKlsDepthpt = pZnak;
    else
        m_pKlsDepthpt = NULL;

    pZnak = GetKlsZnak(m_qkodIzoline);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_IZOLINE))
        m_pKlsIzoline = pZnak;
    else
        m_pKlsIzoline = NULL;

    pZnak = GetKlsZnak(m_qkodUtlizo);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_IZOLINE))
        m_pKlsUtlizo = pZnak;
    else
        m_pKlsUtlizo = NULL;

    pZnak = GetKlsZnak(m_qkodDopizo);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_IZOLINE))
        m_pKlsDopizo = pZnak;
    else
        m_pKlsDopizo = NULL;

    pZnak = GetKlsZnak(m_qkodObryv);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_OBRYV))

        m_pKlsObryv = pZnak;
    else
        m_pKlsObryv = NULL;

    pZnak = GetKlsZnak(m_qkodOtkos);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_OTKOS))
        m_pKlsOtkos = pZnak;
    else
        m_pKlsOtkos = NULL;

    pZnak = GetKlsZnak(m_qkodPodpStenka);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_PODPSTENKA))
        m_pKlsPodpStenka = pZnak;
    else
        m_pKlsPodpStenka = NULL;

    pZnak = GetKlsZnak(m_qkodYama);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_YAMA))
        m_pKlsYama = pZnak;
    else
        m_pKlsYama = NULL;

    pZnak = GetKlsZnak(m_qkodGidro);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_GIDRO))
        m_pKlsGidro = pZnak;
    else
        m_pKlsGidro = NULL;

    pZnak = GetKlsZnak(m_qkodIzobata);
    if((pZnak!=NULL)&&(pZnak->GetClassType()==ID_IZOLINE))
        m_pKlsIzobata = pZnak;
    else
        m_pKlsIzobata = NULL;
}


//==================================
//! minimization of an operational rectangle of a layer
void MReliefLayer::MinimizeBounds()
//==================================
{
    //We form the general list and we cause parental function
    CreateObjectList();
    MLayer::MinimizeBounds();
}


//==================================
//! finds and remembers a point of the minimum height in a relief layer;
/** output parameter - a class MHeightPoint copy - height point with the minimum height
*/
MHeightPoint* MReliefLayer::SetMinHeight()
//==================================
{
    //if there are no marks of heights...
    if(m_heightpoints.empty()) return NULL;

    // we choose a point of the minimum height
    OBITER it = m_heightpoints.begin();

    MHeightPoint* heightpt = (MHeightPoint*)(*it);
    double hmin = heightpt->GetHeight();
    MHeightPoint* hpt;
    double h;
    it++;
    while (it != m_heightpoints.end())
    {
        hpt = (MHeightPoint*)(*it);
        h = hpt->GetHeight();
        if(h<hmin) {
            heightpt = hpt;
            hmin = h;
        }
        it++;
    }
    m_minheight = hmin; //in a layer of marks of heights we place information on the minimum point of height

    return heightpt;

}

//==================================
//! finds and remembers a point of the maximum height in a relief layer;
/**output parameter - a class MHeightPoint copy - height point with the maximum height
*/
MHeightPoint* MReliefLayer::SetMaxHeight()
//==================================
{
    //if there are no marks of heights...
    if(m_heightpoints.empty()) return nullptr;

    // we choose a point of the minimum height
    OBITER it = m_heightpoints.begin();

    MHeightPoint* heightpt = (MHeightPoint*)(*it);
    double hmax = heightpt->GetHeight();
    MHeightPoint* hpt;
    double h;
    it++;
    while (it != m_heightpoints.end())
    {
        hpt = (MHeightPoint*)(*it);
        h = hpt->GetHeight();
        if(h>hmax) {
            heightpt = hpt;
            hmax = h;
        }
        it++;
    }
    m_maxheight = hmax;// in a layer of marks of heights we place information on the maximum point of height

    return heightpt;
}

//==================================
//!sets the minimum and maximum heights of horizontals with 'shag' relief section height;
/**input parameter: double shag
//output  parameters: double* hmin, double* hmax
*/
bool MReliefLayer::SetMinMaxIzoheights(double shag, double* hmin, double* hmax)
//==================================
{
    //height of section of a relief can't be negative or zero
    if(shag<=0) return false;

    double rem;

    //we care that the top and lower borders of drawing of isolines were multiple to relief section height
    //we find the lower bound
    if(shag<1) {
        rem = shag*((double)(((double)m_minheight - (int)m_minheight)/shag) - (int)(((double)m_minheight - (int)m_minheight)/shag));
        hmin[0] = m_minheight - rem;
    }
    else {
        rem = shag*((double)(m_minheight/shag) - (int)(m_minheight/shag));
        hmin[0] = m_minheight - rem;
    }

    //we find the upper bound
    if(m_shag<1) {
        rem = shag*((double)(((double)m_maxheight - (int)m_maxheight)/shag) - (int)(((double)m_maxheight - (int)m_maxheight)/shag));
        hmax[0] = m_maxheight - rem + shag;
    }
    else {
        rem = shag*((double)(m_maxheight/shag) - (int)(m_maxheight/shag));
        hmax[0] = m_maxheight - rem + shag;
    }

    return true;
}


//==================================
//!sets the minimum and maximum heights of additional horizontals
//! with 'shag' relief section height;
/**input parameter: double shag
//output  parameters: double* hmindop, double* hmaxdop
*/
bool MReliefLayer::SetMinMaxDopizoheights(double shag, double* hmindop, double* hmaxdop)
//==================================
{
    //height of section of a relief can't be negative or zero
    if(shag<=0) return false;

    double hmin, hmax;
    //we find borders of drawing of the main horizontals
    SetMinMaxIzoheights(shag, &hmin, &hmax);

    //we find the lower bound of drawing of additional horizontals
    hmindop[0] = hmin - (double)(shag/2);
    if(hmindop[0]<m_minheight)
        hmindop[0] = hmin + (double)(shag/2);

    //we find the upper bound of drawing of additional horizontals
    hmaxdop[0] = hmax + (double)(shag/2);
    if(hmaxdop[0]>m_maxheight)
        hmaxdop[0] = hmax - (double)(shag/2);

    return true;
}


//==================================
//!if heights in a layer "triangulation" coincide with the line
//! level 'c', we slightly move them for correct drawing of horizontals
bool MReliefLayer::ShakeTriangles(double c)
//==================================
{
    if(m_triangles->GetSize() == 0) return false;

    for(int i=0; i<m_triangles->GetSize(); i++){
        //we receive the index on a triangulable triangle
        MTriangle* pTri = m_triangles->GetAt(i);

        if(pTri->m_triheight1 == c){
            pTri->m_triheight1 = c + 0.000001;
        }
        if(pTri->m_triheight2 == c){
            pTri->m_triheight2 = c + 0.000001;
        }

        if(pTri->m_triheight3 == c){
            pTri->m_triheight3 = c + 0.000001;
        }
    }

    return true;
}


//==================================
//!if heights of tops of a contour of coverage coincide with the line of level 'c',
//! we them slightly move that for correct drawing of horizontals
bool MReliefLayer::ShakeKonturOhvata(double c)
//==================================
{
    if(m_RelKonturHgt->GetSize() == 0) return false;

    for(int i=0; i<m_RelKonturHgt->GetSize(); i++) {
        if(m_RelKonturHgt->GetAt(i)[0] == c){
            double h = c + 0.000001;
            m_RelKonturHgt->SetAt(i, &h);
        }
    }

    return true;
}


//==================================
//!we allocate a set of triangles which possess the line of level with, in the 'leveltrs' array,
//! also creates  'maskar' array of masks:
/**dimension of the array of masks 3 * (dimension of the array of triangles), since a position 3*i,
//to the array of masks there are three Boolean variables, if value of a variable j (j=0,1,2) of true, to i-th
//a triangle the line of level with passes through 'j' edge, otherwise, if value
//the j=false variable, the line of level c doesn't pass through 'j' edge;
//input parameter: double c,
//output parameters: MTrArray* leveltrs, MBoArray* maskar;
*/
bool MReliefLayer::GetTrianglesWithLevel(double c, MTrArray* leveltrs, MBoArray* maskar)
//==================================
{
    if(m_triangles->GetSize() == 0) return false;

    for(int i=0; i<m_triangles->GetSize(); i++){
        MTriangle* pTri = m_triangles->GetAt(i);
        double h = pTri->m_triheight1;
        double h1 = pTri->m_triheight2;
        double h2 = pTri->m_triheight3;

        bool rebro1 = false;
        bool rebro2 = false;
        bool rebro3 = false;

        //we mask those sides which possess the line of level, and triangles with these sides it is included in the array
        if((c - h)*(c - h1)<0)
            rebro1 = true;
        if((c - h1)*(c - h2)<0)
            rebro2 = true;
        if((c - h)*(c - h2)<0)
            rebro3 = true;
        if(rebro1||rebro2||rebro3){
            leveltrs->AddTail(pTri);
            maskar->AddTail(&rebro1);
            maskar->AddTail(&rebro2);
            maskar->AddTail(&rebro3);
        }
    }

    return true;
}


//==================================
//!in the array of integers of indArray keeps indexes of edges of a contour of coverage,
//! through which there passes the line of level
/**input parameter: double c,
//output parameters: MTrArray* leveltrs, MBoArray* maskar;
*/
bool MReliefLayer::GetLevelSgsKontOhvata(double c, MInArray* indArray)
//==================================
{
    if(m_RelKonturHgt->GetSize() == 0) return false;

    for(int i=0; i<m_RelKonturHgt->GetSize(); i++) {
        int j;
        if(i == m_RelKonturHgt->GetSize()-1)
            j = 0;
        else
            j = i+1;
        double h = m_RelKonturHgt->GetAt(i)[0];
        double h1 = m_RelKonturHgt->GetAt(j)[0];
        if((c - h)*(c - h1)<0)
            indArray->AddTail(&i);
    }

    return true;
}


//==================================
//!We define orientation of a horizontal.
/**It is auxiliary funktsiya: 'pt1' and 'pt2' points - top of a triangulable triangle,
//through which passes a zero edge of a horizontal
//IDEA: the horizontal already consists of two points and, therefore, has orientation;
//we check, on right or on the left side from a horizontal the top of a triangulable triangle lies;
//height of top is known; on the right side from a horizontal value of height of top has to be more
//horizontal heights; if it not so, we change orientation of a horizontal on opposite after
//completion of full formation of a horizontal.
//We will keep the parameter of orientation of a horizontal in the 'm_dbarray' array: "1" - the horizontal is focused
//clockwise, "0" - the horizontal is focused counterclockwise.
*/
bool MReliefLayer::SetIzolineOrientation(double c, MIzoline* pIzo, MPoint pt1, MPoint pt2, double h1, double h2)
//==================================
{

    if(!pIzo) return false;
    //in a degenerate case we don't change orientation
    if(pIzo->GetPointsNumber()<2){
        MString str= "1";
        pIzo->m_dbarray.SetAtGrow(5,&str);
        return true;
    }

    if(OTS(pIzo->GetPoint(0), pt2, pIzo->GetPoint(1))<0){
        if(c<h2)   {
            MString str= "0";
            pIzo->m_dbarray.SetAtGrow(5,&str);
        }
        if(c>h2)   {
            MString str= "1";
            pIzo->m_dbarray.SetAtGrow(5,&str);
        }
        if(c==h2){

            if(OTS(pIzo->GetPoint(0), pt1, pIzo->GetPoint(1))<0){
                if(c<h1)  {
                    MString str= "0";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
                if(c>h1)  {
                    MString str= "1";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
            }
            else{
                if(c<h1)  {
                    MString str= "1";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
                if(c>h1)  {
                    MString str= "0";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
            }
        }//end if(c==h2)
    }

    else{
        if(c<h2)   {
            MString str= "1";
            pIzo->m_dbarray.SetAtGrow(5,&str);
        }
        if(c>h2)   {
            MString str= "0";
            pIzo->m_dbarray.SetAtGrow(5,&str);
        }

        if(c==h2) {
            if(OTS(pIzo->GetPoint(0), pt1, pIzo->GetPoint(1)) <0)  {
                if(c<h1)   {
                    MString str= "0";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
                if(c>h1)   {
                    MString str= "1";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
            }
            else{
                if(c<h1)  {
                    MString str= "1";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
                if(c>h1)  {
                    MString str= "0";
                    pIzo->m_dbarray.SetAtGrow(5,&str);
                }
            }

        } //end if(c==h2)

    }

    return true;
}


//==================================
//!adds a segment to the array of segments
void MReliefLayer::AddSegment(MPoint pt1, MPoint pt2, double h1, double h2)
//==================================
{
    if(!m_segments) return;

    MSegment* pSeg = new MSegment(pt1, pt2, h1, h2);
    m_segments->AddTail(pSeg);
    delete pSeg;
}

//==================================
//!adds a triangle to the array of triangles
void MReliefLayer::AddTriangle(MPoint pt1, MPoint pt2, MPoint pt3, double h1, double h2, double h3)
//==================================
{
    if(!m_triangles) return;

    MTriangle* pTri = new MTriangle(pt1, pt2, pt3, h1, h2, h3);
    m_triangles->AddTail(pTri);
    delete pTri;
}

//==================================
//!adds a new mark of height to the list of marks of heights
/**the 'visible' parameter is responsible for drawing of a new mark of height
//the 'signif' variable means height mark type: the main/additional
//'pt' - point coordinates
//'h' - point height
*/
void MReliefLayer::AddHeightPoint(MPoint pt, double h, bool visible,
                                  MString signif)
//==================================
{
    if(!m_pKlsHeightpt)
        return;
    //if in a point where we are going to give height mark already
    //there is height mark, new don't put
    if(FindHeightptByCoords(pt)!=NULL)
        return;
    //we form height mark
    MHeightPoint* pHtPt = new MHeightPoint(pt);
    if(!visible)
        pHtPt->m_bVisible = false;
    pHtPt->m_pLay = this;
    pHtPt->ChangeClass(m_qkodHeightpt,false);
    //if it wasn't succeeded to set to a mark height,
    //there is no sense it to place in the list of marks of heights
    if(!pHtPt->SetHeight(h)){
        delete pHtPt;
        return;
    }
    //we give to a new mark of height number equal to number of all marks of heights, believing
    //that numbers of marks of heights go consistently
    pHtPt->SetNomer(m_heightpoints.size());
    pHtPt->SetType(signif);

    //in description language among formulas we look for formulas with the name
    //"Height" and "Height/number";
    //if such formulas aren't present, we don't form the corresponding binding
    MFormArray* formar = pHtPt->m_pZnak->GetYaz()->m_formula;
    int colform = formar->GetSize();
    MString formname;
    int indhform = -1;//index of a formula "Height"
    int indhnform = -1;//index of a formula "Height/number"
    for(int i=0; i<colform; i++){
        formname = formar->GetAt(i)->nazvanie;
        if(formname=="Высота")
            indhform = i;
        else if(formname=="Высота/номер")
            indhnform = i;
        //as soon as found both formulas...
        if((indhform!=-1)&&(indhnform!=-1))
            break;
    }
    MSymbol* sym;
    MPoint ptnadpf, ptnadpl;
    //if there was at least one of formulas, will delete it at distance
    //height mark symbol sizes from a point
    if((indhform != -1)||(indhnform != -1)){
        sym = &((MShriftSrez*)((MSzArray*)((MDotYaz*)pHtPt->m_pZnak->GetYaz()->m_object)->m_tochka)->GetAt(0))->m_tochka;
        ptnadpf.x = pt.x + pHtPt->GEOM(sym->value/2);
        ptnadpf.y = pt.y;
        ptnadpl.x = pt.x + pHtPt->GEOM(sym->value);
        ptnadpl.y = pt.y;
    }
    //we form a vector binding for a formula "Height"
    if(indhform != -1)
        pHtPt->AddVektorPriv(indhform,ptnadpf,ptnadpl);
    //we form a vector binding for a formula "Height/number"
    if((indhnform != -1)&&(pHtPt->GetDataPoleByName("номер")!=""))
        pHtPt->AddVektorPriv(indhnform,ptnadpf,ptnadpl);

    m_heightpoints.push_back(pHtPt);

    //we include a new mark of height in the existing triangulation
    if(m_triangles->GetSize()!=0){
        MPtArray* centerar = new MPtArray();
        MDuArray* radar = new MDuArray();
        CreateCircle(m_triangles, centerar, radar);
        MPtArray* modRegion = new MPtArray();
        MDuArray* heightRegion = new MDuArray();
        FindModregionForTript(pHtPt, m_triangles, m_segments, centerar, radar, modRegion, heightRegion);
        TriangulationInModregion(pHtPt, modRegion, heightRegion, m_triangles, m_segments,
                                 centerar, radar);
        MString str= "нерельефная";
        pHtPt->m_dbarray.SetAt(0,&str);
        delete modRegion;
        delete heightRegion;
        delete centerar;
        delete radar;
    }
}

//!==================================
/**if in the array of structural lines there was a line containing an entrance point
//such structural line breaks into two structural lines, first of which is
//part of the old structural line to the point previous deleted, and the second - part old
//the structural line, beginning from the point following for deleted up to the end
*/
void MReliefLayer::DeletePointFromStructlines(MPoint pt)
//'pt' - coordinates of the deleted point
//==================================
{
    MPtArray* structline;
    int index;
    for(int i=0; i<m_structlines->GetSize(); i++){
        structline = (MPtArray*)m_structlines->GetAt(i);
        index = structline->FindPoint(pt);
        if(index!=-1){
            //if the point belongs to a structural segment, we will delete this segment
            if(structline->GetSize()>2){
                MPtArray* line1 = new MPtArray();
                MPtArray* line2 = new MPtArray();
                int j = 0;
                for(j=0; j<index; j++){
                    line1->AddTail(structline->GetAt(j));
                }
                for(j=index+1; j<structline->GetSize(); j++){
                    line2->AddTail(structline->GetAt(j));
                }
                m_structlines->AddTail((MArray*)line1);
                m_structlines->AddTail((MArray*)line2);
            }
            m_structlines->Delete(i);
            delete structline;
        }
    }
}

//==================================
//!deletes height mark from the list of marks of heights
/**'pt' - coordinates of the deleted point
 */
void MReliefLayer::DeleteHeightPoint(MPoint pt)
//==================================
{
    //we will find height mark with the set coordinates
    MHeightPoint* pHpt = FindHeightptByCoords(pt);
    //the main mark of height can't be deleted
    if(pHpt->GetType()==IDS_BASIC) // IDS_BASIC="основная"
        return;
    //during removal of a mark of height, the point corresponding to it from a contour and structural is deleted
    //lines of a macroform if is
    int index;
    OBITER it = m_macroforms.begin();
    while(it != m_macroforms.end())
    {
        index = ((MObryv*)(*it))->FindPoint(pt);
        if(index!=-1){
            if(!((MObryv*)(*it))->DeletePointFromKontur(index))
                return;
        }
        it++;
    }

    it = m_gidroobjects.begin();
    while(it != m_gidroobjects.end())
    {
        if((*it)->WhatTheObject()==ID_GIDRO){
            index = ((MGidro*)(*it))->FindPoint(pt);
            if(index!=-1)
                if(!((MGidro*)(*it))->DeletePointFromKontur(index))
                    return;
        }
        it++;
    }

    //during removal of a mark of height, the point corresponding to it from a contour and structural is deleted
    //lines of a macroform if is
    DeletePointFromStructlines(pt);

    int vertexind;
    it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        pHpt = (MHeightPoint*)(*it);
        if(pt==pHpt->GetPoint(0)){
            MString str = *pHpt->m_dbarray.GetAt(0);
            //if the deleted point didn't participate in formation of a relief,
            //we will delete it only from the list of marks of heights
            if(*pHpt->m_dbarray.GetAt(0)!=MString("рельефная")){
                m_heightpoints.erase(it);
                delete pHpt;
                break;
            }
            //if the deleted point participates in formation of a relief, we change a set
            //triangulable segments and triangles also we delete a point from the list
            //marks of heights
            else{
                //if the deleted mark of height lies on a relief contour
                if(PTONLOMLINE(pt, m_RelKonturPts, index, vertexind)){
                    //we delete the relief contour point corresponding to the deleted height mark
                    m_RelKonturPts->Delete(vertexind);
                    m_RelKonturHgt->Delete(vertexind);
                    //we will clear everything that is connected with an old contour of a relief
                    m_triangles->ClearAll();
                    m_segments->ClearAll();
                    //we delete height mark from the list of marks of heights and we release memory
                    m_heightpoints.erase(it);
                    delete pHpt;
                    MObjectList konturhpts;
                    FormListOfHeightpoints(m_RelKonturPts, &konturhpts);
                    //we break all area in a new contour of a relief into the triangulable
                    //triangles according to Delon
                    CreatetriangulationByDelone(&konturhpts);
                }
                else{
                    MPtArray* modRegion = new MPtArray();
                    MDuArray* heightRegion = new MDuArray();
                    FindModregionForDeleteHpt(pHpt, modRegion, heightRegion);
                    TriangulationInModregionForDeleteHpt(pHpt, modRegion, heightRegion);
                    m_heightpoints.erase(it);
                    delete pHpt;
                    delete modRegion;
                    delete heightRegion;
                }
                break;
            }
        }
        it++;
    }
    //we fix all structural lines
    FixAllStructLines();
}

//==================================
//!adds the array of points to the list of marks of heights
/**the 'visible' parameter is responsible for drawing of new marks of height
//the 'signif' variable means height mark type: the main/additional
//'pt' - point coordinates
//'h' - point height
*/
void MReliefLayer::AddHeightptArray(MPtArray* ptar,MDuArray* hptar,bool visible,
                                    MString signif)
//==================================
{
    if(!m_pKlsHeightpt)
        return;

    for(int i=0; i<ptar->GetSize(); i++)
    {
        OBITER it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            if(ptar->GetAt(i)[0]==((MHeightPoint*)(*it))->GetPoint(0))
                break;
            it++;
        }
        if(it == m_heightpoints.end())
            AddHeightPoint(ptar->GetAt(i)[0],hptar->GetAt(i)[0],visible,signif);
    }
}

//==================================
//!eturns the index on height mark, if its coordinates
//! coincide with entrance coordinates
MHeightPoint* MReliefLayer::FindHeightptByCoords(MPoint pt)
//==================================
{
    MHeightPoint* pHpt;
    OBITER it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        if((*it)->WhatTheObject() == ID_HEIGHTPOINT){
            pHpt = (MHeightPoint*)(*it);
            if(pt==pHpt->GetPoint(0))
                return pHpt;
        }
        it++;
    }
    return NULL;
}

//==============================
//!transformation to the model
void MReliefLayer::ConvertToMaket()
//==============================
{
    //we will be run on all objects and we will cause in them the corresponding function of transformation
    OBITER it = m_kontlist.begin();
    while(it != m_kontlist.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_decorative.begin();
    while(it !=m_decorative .end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_izobats.begin();
    while(it != m_izobats.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_depthpoints.begin();
    while(it != m_depthpoints.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_gidroobjects.begin();
    while(it != m_gidroobjects.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_macroforms.begin();
    while(it != m_macroforms.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_dopizolines.begin();
    while(it != m_dopizolines.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_izolines.begin();
    while(it != m_izolines.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        (*it)->ConvertToMaket();
        it++;
    }

    MinimizeBounds();
}


//==================================
//! drawing of objects of a layer

//static std::mutex mReliefLayerDrawMu;

void MReliefLayer::Draw(MPlotDevice* pDevice, MRect geodrawrect)
//==================================
{
    // std::lock_guard<std::mutex> lck(mReliefLayerDrawMu);

    if(!x_visible)
        return;

    // REMARK: at the use of this function one cunning is possible:
    //if to give it 'clib' of the zero size, drawing will happen
    //ONLY in 'memdc' context (this effect is used in MView:: Linza).
    //Otherwise at accumulation of the counter
    //count is more than 'm_DRAWPORTION', the image will be periodically splashed out
    //from 'memdc' in 'pDC' (in case 'pDC' - a screen context, it provides
    //gradualness of drawing - portions on 'm_DRAWPORTION' of segments)

    //DRAWING of OBJECTS
    //presets
    //we learn a visible rectangle

    MPtArray* ptar;
    int m_nPoints;

    MRect rectCurrentKontur;
    MRect temp1;
    MRect temp2;

    if((m_viewizolines) && (m_kontlist.size() != 0))
    {
        std::for_each(m_kontlist.begin(), m_kontlist.end(), [&](MObject *itKont)
        {
            ptar = (MPtArray*)itKont->m_Komplekt->GetAt(((MTopobject*)itKont)->F_TOCHKI);

            m_nPoints = ptar->GetSize();
            pDevice->SaveClipping();
            pDevice->AddClipRgn(ptar->GetHead(),m_nPoints,INTERSECT_REGION);

            temp1 = itKont->m_bounds;
            temp2 = geodrawrect;

            temp1.NormalizeGeoRect();
            temp2.NormalizeGeoRect();

            RectIntersection(temp1, temp2, &rectCurrentKontur);

            std::for_each(m_dopizolines.begin(), m_dopizolines.end(), [&] (MObject *it)
            {
                if (it-> m_enabled &&
                        it-> CheckSize(pDevice) &&
                        it->m_bounds.IntersectGeoRect(it->m_bounds, rectCurrentKontur))
                {
                    it->Draw(pDevice);
                }
            });

            pDevice->RestoreClipping();
            ++itKont;
        });

        pDevice->Actualize();
    }

    //we draw contours of visibility of additional horizontals if it is necessary
    if (m_viewkonturs)
    {
        std::for_each(m_kontlist.begin(), m_kontlist.end(), [&](MObject *it)
        {
            if (it-> m_enabled &&
                    it->CheckSize(pDevice) &&
                    it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect))
            {
                pDevice->DrawServantPolyLine((MPtArray*)it->m_Komplekt->GetAt(((MTopobject*)it)->F_TOCHKI),
                                             2,
                                             MColor(0xff, 0x0, 0x0));
            }
        });
    }

    //we draw horizontals if it is necessary
    if (m_viewizolines)
    {
        std::for_each(m_izolines.begin(), m_izolines.end(), [&] (MObject* it)
        {
            if (it-> m_enabled && it-> CheckSize(pDevice))
                // Is the object in zoom area?
                if (it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect))
                {
                    it->Draw(pDevice);
                }
        });
    }

    //we draw isobaths if it is necessary
    if (m_viewizobats)
    {
        std::for_each(m_izobats.begin(), m_izobats.end(), [&] (MObject* it)
        {
            if (it-> m_enabled && it-> CheckSize(pDevice))
                // Is the object in zoom area?
                if (it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect))
                {
                    it->Draw(pDevice);
                }
        });
    }

    //we draw relief macroforms: breaks, slopes, holes, etc.
    std::for_each(m_macroforms.begin(), m_macroforms.end(), [&] (MObject* it)
    {
        if (it->m_enabled && it-> CheckSize(pDevice) &&
                it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect))
        {
            it->Draw(pDevice);

            if (m_viewstructlines)
            {
                pDevice->DrawServantPolyLine(((MObryv*)it)->GetStructKontur(),
                                             2, MColor(0, 0xff, 0xff));

                if(it->WhatTheObject() == ID_YAMA)
                {
                    pDevice->DrawServantPolyLine(((MYama*)it)->GetDownStructKontur(),
                                                 2, MColor(0, 0xff, 0xff));
                }
            }
        }
    });

    //we draw hydroobjects
    std::for_each(m_gidroobjects.begin(), m_gidroobjects.end(), [&] (MObject* it)
    {
        if (it->m_enabled &&
                it-> CheckSize(pDevice) &&
                (it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect)))
        {
            it->Draw(pDevice);
            if (m_viewstructlines)
                pDevice->DrawServantPolyLine(((MObryv*)it)->GetStructKontur(), 2,
                                             MColor(0, 0xff, 0xff));
        }
    });

    //we draw triangles if it is necessary
    if (m_viewtrg)
    {
        MPtArray* pReg;

        int coltri = m_triangles->GetSize();
        MTriangle* pTri;

        for(int i=0; i < coltri; ++i)
        {
            pTri = m_triangles->GetAt(i);
            pReg = new MPtArray();
            pReg->AddTail(&pTri->m_tript1);
            pReg->AddTail(&pTri->m_tript2);
            pReg->AddTail(&pTri->m_tript3);
            MRect boundrect;
            //we will check, whether the segment with 'geodrawrect' is crossed
            FindBoundRect(pReg, &boundrect);
            boundrect.NormalizeGeoRect();
            //there has to be a check of visibility of a triangle from the point of view of the size further
            //if (pTri-> m_enabled)             // 2005.01.18 vipa
            //if (pTri-> CheckSize(pDevice))    // isVisible
            // Is the object in zoom area?
            if (boundrect.IntersectGeoRect(boundrect, geodrawrect))
            {
                pDevice->DrawLine(pTri->m_tript1,pTri->m_tript2);
                pDevice->DrawLine(pTri->m_tript2,pTri->m_tript3);
                pDevice->DrawLine(pTri->m_tript3,pTri->m_tript1);
                //!Sinse qt3
                //pDevice->MoveTo(pTri->m_tript1);
                //pDevice->LineTo(pTri->m_tript2);
                //pDevice->LineTo(pTri->m_tript3);
                //pDevice->LineTo(pTri->m_tript1);
            }

            delete pReg;
        }
    }

    //we draw structural lines if it is necessary
    if (m_viewstructlines)
    {
        MPtArray* structline;
        MRect boundrect;

        for(int i=0; i < m_structlines->GetSize(); ++i)
        {
            structline = (MPtArray*)m_structlines->GetAt(i);

            //we will check, whether the structural line with 'geodrawrect' is crossed
            FindBoundRect(structline, &boundrect);
            boundrect.NormalizeGeoRect();
            //there has to be a check of visibility of a 'structline' from the point of view of the size further
            //if (structline-> m_enabled)             // 2005.01.18 vipa
            //if (structline-> CheckSize(pDevice))    // isVisible
            // Is the object in zoom area?
            if (boundrect.IntersectGeoRect(boundrect, geodrawrect))
            {
                pDevice->DrawServantPolyLine(structline, 2, MColor(0, 0xff, 0xff));
            }
        }
    }

    //we draw декоратив
    std::for_each(m_decorative.begin(), m_decorative.end(), [&] (MObject* it)
    {
        if (it-> m_enabled && it-> CheckSize(pDevice) &&
                it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect))
        {
            it->Draw(pDevice);
        }
    });

    //we draw marks of heights.
    std::for_each(m_heightpoints.begin(), m_heightpoints.end(), [&] (MObject* it)
    {
        if (it-> m_enabled && it-> CheckSize(pDevice) &&
                (it->m_bounds.IntersectGeoRect(it->m_bounds, geodrawrect)))
        {
            it->Draw(pDevice);
        }
    });

    //we draw marks of depths.
    std::for_each(m_depthpoints.begin(), m_depthpoints.end(), [&] (MObject* it)
    {
        if (it->m_enabled && it->CheckSize(pDevice)
                && (it->m_bounds.IntersectGeoRect(it->m_bounds,geodrawrect)))
        {
            it->Draw(pDevice);
        }
    });

    if(m_viewRelKontur)
    {
        //for drawing of area of a triangulation we will close a triangulation area contour
        MPtArray* trikontur = new MPtArray();
        trikontur->AppendTail(m_RelKonturPts);
        trikontur->AddTail(m_RelKonturPts->GetAt(0));
        pDevice->DrawServantPolyLine(trikontur, 3, MColor(0, 0xff, 0xff));
        delete trikontur;
    }
    // we draw a result
    pDevice->Actualize();
}

//==================================
//! drawing of the text at objects of a layer
void MReliefLayer::DrawText(MPlotDevice* pDevice,MRect geodrawrect)
//==================================
{
    // REMARK: at the use of this function one cunning is possible:
    //if to give it 'clib' of the zero size, drawing will happen
    //ONLY in 'memdc' context (this effect is used in MView:: Linza).
    //Otherwise at accumulation of the counter
    //count is more than 'm_DRAWPORTION', the image will be periodically splashed out
    //from 'memdc' in 'pDC' (in case 'pDC' - a screen context, it provides
    //gradualness of drawing - portions on 'm_DRAWPORTION' of segments)

    //DRAWING OF THE TEXT
    OBITER it;
    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_izolines.begin();
        while(it != m_izolines.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }

    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_dopizolines.begin();
        while(it != m_dopizolines.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }

    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }

    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_depthpoints.begin();
        while(it != m_depthpoints.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }
    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_macroforms.begin();
        while(it != m_macroforms.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }

    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_gidroobjects.begin();
        while(it != m_gidroobjects.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }

    if ((x_visible)&&((!x_common)||(x_textvisible))) {
        it = m_decorative.begin();
        while(it != m_decorative.end())
        {
            // Is the object in zoom area?
            if ((*it)->m_bounds.IntersectGeoRect((*it)->m_bounds,geodrawrect))
                (*it)->DrawText(pDevice);
            it++;
        }
        // we draw a result
        pDevice->Actualize();
    }

}


//==================================
//!Removal of degenerate horizontals
bool MReliefLayer::RemoveRelictIzolines(MObjectList* izolines)
//==================================
{	
    MObjectList newizolines;
    MObjectList delizolines;

    OBITER it = izolines->begin();
    while(it != izolines->end())
    {
        if(((MIzoline*)(*it))->GetPointsNumber() > 1)
            newizolines.push_back(*it);
        else delizolines.push_back(*it);
        it++;
    }
    izolines->clear();
    izolines->swap(newizolines);

    it = delizolines.begin();
    while(it != delizolines.end())
    {
        delete (*it);
        it++;
    }
    delizolines.clear();

    return true;
}


//==================================
//!Cleaning of horizontals from close points
bool MReliefLayer::ClearIzolinesFromNearPoints(MObjectList* izolines)
//==================================
{
    OBITER it = izolines->begin();
    while(it != izolines->end())
    {
        ((MIzoline*)(*it))->DeleteNearPoints(m_shag);
        it++;
    }
    return true;
}

//==================================
//! To choose height mark from a set of marks of heights, the next to the set height mark
/**It is support function for function of creation of a network of triangulable triangles
// INPUT PARAMETERS: 'oblist' - the list of marks of heights from which we make a choice,
//                   changes in function - the chosen height mark is excluded from it;
//                   'point' - the index on coordinates of a mark of height to which we look for
//                   the next mark;
// OUTPUT PARAMETER - the pointer on height mark, the next to set
*/
MHeightPoint* MReliefLayer::FindNearestHpoint(MObjectList& oblist, MPoint* point)
//==================================
{
    if((oblist.size()==0)||(!point))
        return NULL;

    OBITER it = oblist.begin();
    MObject* pObj = *it;
    MPoint pt = ((MTopobject*)pObj)->GetPoint(0);
    double h = point->Distance(&pt);

    MObjectList oblistdop;//auxiliary list of marks of heights
    double h1;
    OBITER it1 = it++;

    while (it1 != oblist.end())
    {
        pt = ((MHeightPoint*)(*it1))->GetPoint(0);
        h1 = point->Distance(&pt);
        if(h1<h) {
            h = h1;
            oblistdop.push_back(pObj);
            pObj = *it1;
        }
        else oblistdop.push_back(*it1);
        it1++;
    }

    oblist.swap(oblistdop);

    return (MHeightPoint*)(*it);
}


//==================================
//!To choose height mark from a set of marks of heights, distance from which till the ends of the entrance
//! segment the minimum
/**It is support function for function of creation of a network of triangulable triangles
// INPUT PARAMETERS: 'oblist' - the list of marks of heights from which we make a choice,
//                   changes in function - the chosen height mark is excluded from it;
//                   'pt1' and 'pt2' - indexes for the ends of a segment to which we look for the next mark;
//OUTPUT PARAMETER - the pointer on height mark, the next to the set segment
*/
MHeightPoint* MReliefLayer::FindNearestHptForSegment(MObjectList& oblist, MPoint* point1, MPoint* point2)
//==================================
{
    if((oblist.size()==0)||(!point1)||(!point2))
        return NULL;

    MObjectList oblistdop;//auxiliary list of marks of heights
    MObject* pObj;
    //	MObject* pObj1;
    MPoint pt,pt1;
    double h=-999.9, h1;

    //	POSITION pos = oblist.GetHeadPosition();
    OBITER it = oblist.begin();
    //on a required mark of height we impose the condition following from definition
    //networks of triangulable triangles, - triangles have to be approached
    //to equilateral therefore we will watch that the ends of a segment and a required mark of height
    //formed a triangle with angles at a segment >30 degrees and <150 degrees;
    //we look for the first such mark of height
    while (it != oblist.end())
    {
        pObj = *it;
        pt = ((MTopobject*)pObj)->GetPoint(0);
        h = point1->Distance(&pt) + point2->Distance(&pt);
        // for each point we check the following condition - in the triangle formed by a point and the ends of a segment angles at a segment have to be >30 degrees and <150 degrees,
        // 'pt' - a point, the next on the sum of distances till the ends of a segment; 'str' - 'pt' height; 'h' - the sum of distances from the ends of a segment to 'pt'
        if((fabs(COS3Points(point2, point1, pt))<(double)(sqrt(3.0)/2)) &&
                (fabs(COS3Points(point1, point2, pt))<(double)(sqrt(3.0)/2))) {
            break;
        }
        else oblistdop.push_back(pObj);
        it++;
    }
    OBITER it1 = it++;
    while(it1 != oblist.end())
    {
        pt = ((MTopobject*)(*it1))->GetPoint(0);
        h1 = point1->Distance(&pt) + point2->Distance(&pt);
        if((fabs(COS3Points(point1, point2, pt1))<(double)(sqrt(3.0)/2)) &&
                (fabs(COS3Points(point2, point1, pt1))<(double)(sqrt(3.0)/2))){
            if(h1<h) {
                h = h1;
                oblistdop.push_back(pObj);
                pObj = *it1;
            }
            else oblistdop.push_back(*it1);
        }
        else oblistdop.push_back(*it1);
    }

    //if there was no mark of height meeting the imposed condition
    if(oblistdop.size()==oblist.size())
        return NULL;

    oblist.swap(oblistdop);

    return (MHeightPoint*)pObj;
}

//==================================
//!Creation of boundary horizontals of height with on triangulable triangles
/**REMARK 1: height with is present here indirectly through arrays: 'indArray' - in the indArray array
//           kept indexes of edges of a contour of coverage through which there passes the line of level 'c';
//           'leveltrs' - a set of triangles which possess the line of level 'c';
//           'maskar'-//the array of masks a set of triangles of 'leveltrs';
//           the type=true parameter means creation of the main horizontals: if type=false,
//           we build additional horizontals.
//REMARK 2: orientation of triangles in a layer "triangulation" plays an essential role!!! It is impossible to change!!!
//          at 'Createtriangulation' function working off all triangulable triangles are guided clockwise
//REMARK 3: at construction all horizontals are guided so that at round of the line on increase of tops,
//          on the right there were horizontals with bigger height; it is essential to correct drawing
//          hachure and correct signature of horizontals
//REASONS: orientation is seted in the course of creation of horizontals by situation definition (inside/outside)
//         one of tops of a triangulable triangle in relation to a horizontal under construction;
//         on the sixth place in the 'm_dbarray' array of a horizontal we bring "1" if orientation doesn't be changed,
//         and "0" otherwise; upon completion of creation of a horizontal, we change in it orientation if it is necessary
*/
bool MReliefLayer::CreateLevelBorderIzolines(double c, MInArray* indArray, MTrArray* leveltrs, MBoArray* maskar, int count, bool type)
//==================================
{
    if(leveltrs==NULL) return false;
    if(maskar==NULL) return false;
    if(indArray==NULL)return false;

    double h1, h2;
    MPoint pt1, pt2, pt;
    int i, j;
    bool param = false;
    //	int nom = -1;
    MIzoline* pIzo;

    //in a triangle we look for a side which coincides with the first side of a contour of coverage,
    //through which there passes this line of level
    for(int s=0; s<indArray->GetSize(); s++) {
        i = indArray->GetAt(s)[0];
        if(i == indArray->GetSize() - 1)
            j = 0;
        else
            j = i+1;

        //'coltrg' - number of the remained triangles in the 'leveltrs' array at the time of construction
        //next boundary horizontal
        int coltrg = leveltrs->GetSize();
        for(int l = 0; l<coltrg; l++){
            MTriangle* pTriHpt = leveltrs->GetAt(l);
            //the zero point of a triangle coincided with the beginning of an edge of a contour of coverage
            if(pTriHpt->m_tript1 == m_RelKonturPts->GetAt(i)[0]) {
                //the zero edge of a triangle coincides with a contour edge
                if(pTriHpt->m_tript2 == m_RelKonturPts->GetAt(j)[0]) {
                    pIzo = new MIzoline();
                    pIzo->m_pLay = this;
                    if(type){
                        if(count==3)
                            pIzo->SetKlsAttributes(IZ_UTOLSCH);
                        else
                            pIzo->SetKlsAttributes(IZ_OSNOVNAYA);
                        m_izolines.push_back(pIzo);
                    }
                    else if(!type){
                        pIzo->SetKlsAttributes(IZ_DOPOLN);
                        m_dopizolines.push_back(pIzo);
                    }
                    MString str;
                    str.Format("%d", m_izotype);
                    pIzo->m_dbarray.SetAtGrow(3,&str);
                    str.Format("%d", m_doppts);
                    pIzo->m_dbarray.SetAtGrow(4,&str);
                    pIzo->SetizoHeight(c);
                    pIzo->SetizoType("граничная");
                    h1 = pTriHpt->m_triheight2;
                    h2 = pTriHpt->m_triheight1;
                    //we find coordinates of a zero point of a horizontal
                    pt = COOPT(pTriHpt->m_tript2, pTriHpt->m_tript1, h1, h2, c);
                    pIzo->AddPoint(pt);
                    maskar->SetAt(3*l, &param);
                    //the horizontal passes through the first edge of a triangle
                    if(maskar->GetAt(3*l+1)[0]) {
                        h1 = pTriHpt->m_triheight3;
                        h2 = pTriHpt->m_triheight2;
                        pt1 = pTriHpt->m_tript3;
                        pt2 = pTriHpt->m_tript2;
                        pt = COOPT(pt1, pt2, h1, h2, c);
                        pIzo->AddPoint(pt);
                        maskar->SetAt(3*l+1, &param);
                        //we define orientation of a horizontal
                        SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                        //we exclude the considered triangle from the 'leveltrs' array
                        leveltrs->Delete(l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        break;
                    }
                    //the horizontal passes through the second edge of a triangle
                    else {
                        h1 = pTriHpt->m_triheight1;
                        h2 = pTriHpt->m_triheight3;
                        pt1 = pTriHpt->m_tript1;
                        pt2 = pTriHpt->m_tript3;
                        pt = COOPT(pt1, pt2, h1, h2, c);
                        pIzo->AddPoint(pt);
                        maskar->SetAt(3*l+2, &param);
                        //we define orientation of a horizontal
                        SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                        //we exclude the considered triangle from the 'leveltrs' array
                        leveltrs->Delete(l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        break;
                    }
                } // end internal if
            } // end if
            //the first point of a triangle coincided with the beginning of an edge of a contour of coverage
            else if(pTriHpt->m_tript2 == m_RelKonturPts->GetAt(i)[0]) {
                //the first edge of a triangle coincides with a contour edge
                if(pTriHpt->m_tript3 == m_RelKonturPts->GetAt(j)[0]) {
                    pIzo = new MIzoline();
                    pIzo->m_pLay = this;
                    if(type){
                        if(count==3)
                            pIzo->SetKlsAttributes(IZ_UTOLSCH);
                        else
                            pIzo->SetKlsAttributes(IZ_OSNOVNAYA);
                        m_izolines.push_back(pIzo);
                    }
                    else if(!type){
                        pIzo->SetKlsAttributes(IZ_DOPOLN);
                        m_dopizolines.push_back(pIzo);
                    }
                    MString str;
                    str.Format("%d", m_izotype);
                    pIzo->m_dbarray.SetAtGrow(3,&str);
                    str.Format("%d", m_doppts);
                    pIzo->m_dbarray.SetAtGrow(4,&str);
                    pIzo->SetizoHeight(c);
                    pIzo->SetizoType("граничная");
                    h1 = pTriHpt->m_triheight3;
                    h2 = pTriHpt->m_triheight2;
                    //we find coordinates of a zero point of a horizontal
                    pt = COOPT(pTriHpt->m_tript3, pTriHpt->m_tript2, h1, h2, c);
                    pIzo->AddPoint(pt);
                    maskar->SetAt(3*l+1, &param);
                    //the horizontal passes through a zero edge of a triangle
                    if(maskar->GetAt(3*l)[0]) {
                        h1 = pTriHpt->m_triheight2;
                        h2 = pTriHpt->m_triheight1;
                        pt1 = pTriHpt->m_tript2;
                        pt2 = pTriHpt->m_tript1;
                        pt = COOPT(pt1, pt2, h1, h2, c);
                        pIzo->AddPoint(pt);
                        maskar->SetAt(3*l, &param);
                        //we define orientation of a horizontal
                        SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                        //we exclude the considered triangle from the 'leveltrs' array
                        leveltrs->Delete(l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        break;
                    }
                    //the horizontal passes through the second edge of a triangle
                    else {
                        h2 = pTriHpt->m_triheight3;
                        h1 = pTriHpt->m_triheight1;
                        pt2 = pTriHpt->m_tript3;
                        pt1 = pTriHpt->m_tript1;
                        pt = COOPT(pt1, pt2, h1, h2, c);
                        pIzo->AddPoint(pt);
                        maskar->SetAt(3*l+2, &param);
                        //we define orientation of a horizontal

                        SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                        //we exclude the considered triangle from the 'leveltrs' array
                        leveltrs->Delete(l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        break;
                    }
                } // end internal if
            } // end else if
            //the second point of a triangle coincided with the beginning of an edge of a contour of coverage
            else if(pTriHpt->m_tript3 == m_RelKonturPts->GetAt(i)[0]) {
                //the second edge of a triangle coincides with a coverage contour edge
                if(pTriHpt->m_tript1 == m_RelKonturPts->GetAt(j)[0]) {
                    pIzo = new MIzoline();
                    pIzo->m_pLay = this;
                    if(type){
                        if(count==3)
                            pIzo->SetKlsAttributes(IZ_UTOLSCH);
                        else
                            pIzo->SetKlsAttributes(IZ_OSNOVNAYA);
                        m_izolines.push_back(pIzo);
                    }
                    else if(!type){
                        pIzo->SetKlsAttributes(IZ_DOPOLN);
                        m_dopizolines.push_back(pIzo);
                    }
                    MString str;
                    str.Format("%d", m_izotype);
                    pIzo->m_dbarray.SetAtGrow(3,&str);
                    str.Format("%d", m_doppts);
                    pIzo->m_dbarray.SetAtGrow(4,&str);
                    pIzo->SetizoHeight(c);
                    pIzo->SetizoType("граничная");
                    h1 = pTriHpt->m_triheight3;
                    h2 = pTriHpt->m_triheight1;
                    //we find coordinates of a zero point of a horizontal
                    pt = COOPT(pTriHpt->m_tript3, pTriHpt->m_tript1, h1, h2, c);
                    pIzo->AddPoint(pt);
                    maskar->SetAt(3*l+2, &param);
                    //the horizontal passes through a zero edge of a triangle
                    if(maskar->GetAt(3*l)[0]) {
                        h1 = pTriHpt->m_triheight2;
                        h2 = pTriHpt->m_triheight1;
                        pt1 = pTriHpt->m_tript2;
                        pt2 = pTriHpt->m_tript1;
                        pt = COOPT(pt1, pt2, h1, h2, c);
                        pIzo->AddPoint(pt);
                        maskar->SetAt(3*l, &param);
                        //we define orientation of a horizontal
                        SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                        //we exclude the considered triangle from the 'leveltrs' array
                        leveltrs->Delete(l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        break;
                    }
                    //the horizontal passes through the first edge of a triangle
                    else {
                        h1 = pTriHpt->m_triheight3;;
                        h2 = pTriHpt->m_triheight2;
                        pt1 = pTriHpt->m_tript3;
                        pt2 = pTriHpt->m_tript2;
                        pt = COOPT(pt1, pt2, h1, h2, c);
                        pIzo->AddPoint(pt);
                        maskar->SetAt(3*l+1, &param);
                        //we define orientation of a horizontal
                        SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                        //we exclude the considered triangle from the 'leveltrs' array
                        leveltrs->Delete(l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        maskar->Delete(3*l);
                        break;
                    }
                } // end internal if
            } // end else if
        }//end for

        bool par;
        if(leveltrs->GetSize() == coltrg)
            par = false;
        else
            par = true;

        // creation of a horizontal; we go on the adjacent triangles containing
        //the line of level, we won't reach border yet
        //if after pass of a cycle the 'par' parameter is equal to zero, means the adjacent
        //triangles aren't any more, and we reached border
        while(par) {
            MObject* pObj;
            if(type)
                pObj = *m_izolines.rbegin();
            else if(!type)
                pObj = *m_dopizolines.rbegin();
            par = false; // the par parameter is equal to unit as soon as there is a first triangle adjacent to previous, containing the line of level
            coltrg = leveltrs->GetSize();
            for(int l=0; l<coltrg; l++){
                MTriangle* pTriHpt = leveltrs->GetAt(l);
                if(pTriHpt->m_tript1 == pt1) {
                    //found the triangle adjacent on a zero edge
                    if(pTriHpt->m_tript2 == pt2) {
                        maskar->SetAt(3*l, &param); // unmask a triangle distinction through which already drew a horizontal
                        //if the horizontal passes through the first edge of a triangle
                        if(maskar->GetAt(3*l+1)[0]) {
                            h1 = pTriHpt->m_triheight3;
                            h2 = pTriHpt->m_triheight2;
                            pt1 = pTriHpt->m_tript3;
                            pt2 = pTriHpt->m_tript2;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+1, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                        //if the horizontal passes through the second edge of a triangle
                        else {
                            h1 = pTriHpt->m_triheight1;
                            h2 = pTriHpt->m_triheight3;
                            pt1 = pTriHpt->m_tript1;
                            pt2 = pTriHpt->m_tript3;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+2, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                    } // end internal if
                } // end if
                else if(pTriHpt->m_tript2 == pt1) {
                    //found the triangle adjacent on the first edge
                    if(pTriHpt->m_tript3 == pt2) {
                        maskar->SetAt(3*l+1, &param);
                        //if the horizontal passes through a zero edge of a triangle
                        if(maskar->GetAt(3*l)[0]) {
                            h1 = pTriHpt->m_triheight2;
                            h2 = pTriHpt->m_triheight1;
                            pt1 = pTriHpt->m_tript2;
                            pt2 = pTriHpt->m_tript1;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                        //if the horizontal passes through the second edge of a triangle
                        else {
                            h1 = pTriHpt->m_triheight1;
                            h2 = pTriHpt->m_triheight3;
                            pt1 = pTriHpt->m_tript1;
                            pt2 = pTriHpt->m_tript3;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+2, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                    } // end internal if
                } // end if
                else if(pTriHpt->m_tript3 == pt1) {
                    //found the triangle adjacent on the second edge
                    if(pTriHpt->m_tript1 == pt2) {
                        maskar->SetAt(3*l+2, &param);
                        //if the horizontal passes through a zero edge of a triangle
                        if(maskar->GetAt(3*l)[0]) {
                            h1 = pTriHpt->m_triheight2;
                            h2 = pTriHpt->m_triheight1;
                            pt1 = pTriHpt->m_tript2;
                            pt2 = pTriHpt->m_tript1;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                        //if the horizontal passes through the first edge of a triangle
                        else {
                            h1 = pTriHpt->m_triheight3;
                            h2 = pTriHpt->m_triheight2;
                            pt1 = pTriHpt->m_tript3;
                            pt2 = pTriHpt->m_tript2;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+1, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                    } // end internal if
                } // end if
            } // end for
            //if from all list of triangles there was no adjacent, means we reached
            //to border, it is also possible to change orientation of again constructed horizontal, if
            //it is necessary
            if(!par){
                MObject* pObj;
                if(type)
                    pObj = *m_izolines.rbegin();
                else if(!type)
                    pObj = *m_dopizolines.rbegin();
                //we will delete a trivial horizontal
                if(((MIzoline*)pObj)->GetPointsNumber() <= 1){
                    m_izolines.pop_back();
                    delete pObj;
                }
                else{
                    //we change orientation if it is necessary
                    if(*pObj->m_dbarray[5] == "0"){
                        ((MIzoline*)pObj)->OrientInverse();
                    }
                    //we will delete close points on a horizontal
                    ((MIzoline*)pObj)->DeleteNearPoints(m_shag);
                    //if after removal of close points the number of points of a horizontal became
                    //less than two, we will delete such horizontal
                    if(((MIzoline*)pObj)->GetPointsNumber() < 2){
                        RemoveObject(pObj);
                        delete pObj;
                    }
                    else{
                        //we will transform a horizontal according to type
                        ((MIzoline*)pObj)->TransformOnType(m_shag);

                        pObj->m_dbarray.Delete(5);
                    }
                }
            }
        } // end while(par == true)
    } // end for
    return true;
}


//==================================
//!Creation of internal horizontals of height with on triangulable triangles.
/**the type=true parameter means creation of the main horizontals: if type=false,
//we build additional horizontals.
//REMARK 1: height with is present here indirectly through arrays:
//          'leveltrs' - a set of triangles which possess the line of level with;
//          'maskar' - the array of masks a set of triangles of leveltrs.
//REMARK 2: orientation of triangles in a layer "triangulation" plays an essential role!!! It is impossible to change!!!
//          at 'Createtriangulation' function working off all triangulable triangles are guided clockwise
//REMARK 3: at construction all horizontals are guided so that at round of the line on increase of tops,
//          on the right there were horizontals with bigger height; it is essential to correct drawing
//          hachure and correct signature of horizontals
//REASONS: orientation is seted in the course of creation of horizontals by situation definition (inside/outside)
//         one of tops of a triangulable triangle in relation to a horizontal under construction;
//         on the sixth place in the 'm_dbarray' array of a horizontal we bring "1" if orientation doesn't be changed,
//         and "0" otherwise; upon completion of creation of a horizontal, we change in it orientation if it is necessary
*/
bool MReliefLayer::CreateLevelInnerIzolines(double c, MTrArray* leveltrs, MBoArray* maskar, int count, bool type)
//==================================

{
    if(leveltrs==NULL) return false;
    if(maskar==NULL) return false;

    MPoint pt, pt1, pt2;
    double h1, h2;
    //	int nom;

    //	if(type)
    //		nom = m_izolines.size() - 1;
    //	if(!type)
    //		nom = m_dopizolines.size() - 1;
    bool param = false;
    MIzoline* pIzo;

    // we look for the first triangle through which side there passes the line of level 'c'
    while(leveltrs->GetSize() != 0){
        MTriangle* pTriHpt = leveltrs->GetAt(0);
        //if the horizontal passes through a zero edge...
        if(maskar->GetAt(0)[0]){
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            if(type){
                if(count==3)
                    pIzo->SetKlsAttributes(IZ_UTOLSCH);
                else
                    pIzo->SetKlsAttributes(IZ_OSNOVNAYA);
                m_izolines.push_back(pIzo);
            }
            else if(!type){
                pIzo->SetKlsAttributes(IZ_DOPOLN);
                m_dopizolines.push_back(pIzo);
            }
            MString str;
            str.Format("%d", m_izotype);
            pIzo->m_dbarray.SetAtGrow(3,&str);
            str.Format("%d", m_doppts);
            pIzo->m_dbarray.SetAtGrow(4,&str);
            pIzo->SetizoHeight(c);
            pIzo->SetizoType("внутренняя");
            h1 = pTriHpt->m_triheight2;
            h2 = pTriHpt->m_triheight1;
            pt = COOPT(pTriHpt->m_tript2, pTriHpt->m_tript1, h1, h2, c);
            pIzo->AddPoint(pt);
            maskar->SetAt(0, &param); // unmask a triangle distinction through which already drew a horizontal
            //... and through the first edge
            if(maskar->GetAt(1)[0]) {
                h1 = pTriHpt->m_triheight3;
                h2 = pTriHpt->m_triheight2;
                pt1 = pTriHpt->m_tript3;
                pt2 = pTriHpt->m_tript2;
                pt = COOPT(pt1, pt2, h1, h2, c);
                pIzo->AddPoint(pt);
                maskar->SetAt(1, &param);
                //we define orientation of a horizontal
                SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                //we exclude the considered triangle from the 'leveltrs' array
                leveltrs->Delete(0);
                maskar->Delete(0);
                maskar->Delete(0);
                maskar->Delete(0);
            }
            //... and through the second edge
            else{
                h1 = pTriHpt->m_triheight1;
                h2 = pTriHpt->m_triheight3;
                pt1 = pTriHpt->m_tript1;
                pt2 = pTriHpt->m_tript3;
                pt = COOPT(pt1, pt2, h1, h2, c);
                pIzo->AddPoint(pt);
                maskar->SetAt(2, &param);
                //we define orientation of a horizontal
                SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
                //we exclude the considered triangle from the 'leveltrs' array
                leveltrs->Delete(0);
                maskar->Delete(0);
                maskar->Delete(0);
                maskar->Delete(0);
            }
        }
        //if the horizontal passes through the first edge...
        else{
            pIzo = new MIzoline();
            pIzo->m_pLay = this;
            if(type){
                if(count==3)
                    pIzo->SetKlsAttributes(IZ_UTOLSCH);
                else
                    pIzo->SetKlsAttributes(IZ_OSNOVNAYA);
                m_izolines.push_back(pIzo);
            }
            else if(!type){
                pIzo->SetKlsAttributes(IZ_DOPOLN);
                m_dopizolines.push_back(pIzo);
            }
            MString str;
            str.Format("%d", m_izotype);
            pIzo->m_dbarray.SetAtGrow(3,&str);
            str.Format("%d", m_doppts);
            pIzo->m_dbarray.SetAtGrow(4,&str);
            pIzo->SetizoHeight(c);
            pIzo->SetizoType("внутренняя");
            h1 = pTriHpt->m_triheight3;
            h2 = pTriHpt->m_triheight2;
            pt = COOPT(pTriHpt->m_tript3, pTriHpt->m_tript2, h1, h2, c);
            pIzo->AddPoint(pt);
            maskar->SetAt(1, &param); // unmask a triangle distinction through which already drew a horizontal
            //... and through the second edge
            h1 = pTriHpt->m_triheight1;
            h2 = pTriHpt->m_triheight3;
            pt1 = pTriHpt->m_tript1;
            pt2 = pTriHpt->m_tript3;
            pt = COOPT(pt1, pt2, h1, h2, c);
            pIzo->AddPoint(pt);
            maskar->SetAt(2, &param);
            //we define orientation of a horizontal
            SetIzolineOrientation(c, pIzo, pt1, pt2, h1, h2);
            //we exclude the considered triangle from the 'leveltrs' array
            leveltrs->Delete(0);
            maskar->Delete(0);
            maskar->Delete(0);
            maskar->Delete(0);
        }

        bool par = true;

        // creation of a horizontal; we go on the adjacent triangles containing
        //the line of level, so far those are
        //if after pass of a cycle the 'par' parameter is equal to zero, means the adjacent
        //triangles aren't any more, and we finished creation of a horizontal
        while(par) {
            MObject* pObj;
            if(type)
                pObj = *m_izolines.rbegin();
            if(!type)
                pObj = *m_dopizolines.rbegin();
            par = false; // the par parameter is equal to unit as soon as there is a first triangle adjacent to previous, containing the line of level
            for(int l=0; l<leveltrs->GetSize(); l++){
                MTriangle* pTriHpt = leveltrs->GetAt(l);
                if(pTriHpt->m_tript1 == pt1) {
                    //found the triangle adjacent on a zero edge
                    if(pTriHpt->m_tript2 == pt2) {
                        maskar->SetAt(3*l, &param); // unmask a triangle distinction through which already drew a horizontal
                        //if the horizontal passes through the first edge of a triangle
                        if(maskar->GetAt(3*l+1)[0]) {
                            h1 = pTriHpt->m_triheight3;
                            h2 = pTriHpt->m_triheight2;
                            pt1 = pTriHpt->m_tript3;
                            pt2 = pTriHpt->m_tript2;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+1, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                        //if the horizontal passes through the second edge of a triangle
                        else {
                            h1 = pTriHpt->m_triheight1;
                            h2 = pTriHpt->m_triheight3;
                            pt1 = pTriHpt->m_tript1;
                            pt2 = pTriHpt->m_tript3;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+2, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                    } // end internal if
                } // end if
                else if(pTriHpt->m_tript2 == pt1) {
                    //found the triangle adjacent on the first edge
                    if(pTriHpt->m_tript3 == pt2) {
                        maskar->SetAt(3*l+1, &param);
                        //if the horizontal passes through a zero edge of a triangle
                        if(maskar->GetAt(3*l)[0]) {
                            h1 = pTriHpt->m_triheight2;
                            h2 = pTriHpt->m_triheight1;
                            pt1 = pTriHpt->m_tript2;
                            pt2 = pTriHpt->m_tript1;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                        //if the horizontal passes through the second edge of a triangle
                        else {
                            h1 = pTriHpt->m_triheight1;
                            h2 = pTriHpt->m_triheight3;
                            pt1 = pTriHpt->m_tript1;
                            pt2 = pTriHpt->m_tript3;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+2, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                    } // end internal if
                } // end if
                else if(pTriHpt->m_tript3 == pt1) {
                    //found the triangle adjacent on the second edge
                    if(pTriHpt->m_tript1 == pt2) {
                        maskar->SetAt(3*l+2, &param);
                        //if the horizontal passes through a zero edge of a triangle
                        if(maskar->GetAt(3*l)[0]) {
                            h1 = pTriHpt->m_triheight2;
                            h2 = pTriHpt->m_triheight1;
                            pt1 = pTriHpt->m_tript2;
                            pt2 = pTriHpt->m_tript1;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);

                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                        //if the horizontal passes through the first edge of a triangle
                        else {
                            h1 = pTriHpt->m_triheight3;
                            h2 = pTriHpt->m_triheight2;
                            pt1 = pTriHpt->m_tript3;
                            pt2 = pTriHpt->m_tript2;
                            pt = COOPT(pt1, pt2, h1, h2, c);
                            ((MIzoline*)pObj)->AddPoint(pt);
                            maskar->SetAt(3*l+1, &param);
                            //we exclude the considered triangle from the 'leveltrs' array
                            leveltrs->Delete(l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            maskar->Delete(3*l);
                            par = true;
                            break;
                        }
                    } // end internal if
                } // end if
            } // end for
            //if from all list of triangles there was no adjacent, means we reached
            //to border, it is also possible to change orientation of again constructed horizontal, if
            //it is necessary
            if(!par){
                MObject* pObj;
                if(type)
                    pObj = *m_izolines.rbegin();
                else if(!type)
                    pObj = *m_dopizolines.rbegin();
                //we will delete a trivial horizontal
                if(((MIzoline*)pObj)->GetPointsNumber() <= 1){
                    m_izolines.pop_back();
                    delete pObj;
                }
                else{
                    //we change orientation if it is necessary
                    if(*pObj->m_dbarray[5] == "0"){
                        ((MIzoline*)pObj)->OrientInverse();
                    }
                    //we will delete close points on a horizontal
                    ((MIzoline*)pObj)->DeleteNearPoints(m_shag);
                    //if after removal of close points the number of points of a horizontal became
                    //less than two, we will delete such horizontal
                    if(((MIzoline*)pObj)->GetPointsNumber() < 2){
                        RemoveObject(pObj);
                        delete pObj;
                    }
                    else{
                        //we will transform a horizontal according to type
                        ((MIzoline*)pObj)->TransformOnType(m_shag);

                        pObj->m_dbarray.Delete(5);
                    }
                }
            }
        } // end while(par == true)
    } // end while(leveltrs->GetSize() != 0)

    return true;
}


//==================================
//!Creation of horizontals of height with on triangulable triangles.
/**the 'type'=true parameter means creation of the main horizontals: if 'type'=false,
//we build additional horizontals.
*/
bool MReliefLayer::CreateLevelIzolines(double c, int count, bool type)
//==================================
{
    //we move triangulable triangles if it is necessary
    ShakeTriangles(c);
    //we move a coverage contour if it is necessary
    ShakeKonturOhvata(c);

    //in the 'indArray' array we will keep indexes of edges of a contour of coverage,
    //through which there passes the line of level
    MInArray* indArray = new MInArray();
    //set of triangles which possess the line of level 'c'
    MTrArray* leveltrs = new MTrArray();
    //array of masks set of triangles of 'leveltrs'
    MBoArray* maskar = new MBoArray();

    //we allocate a set of triangles which possess the line of level 'c', in the 'leveltrs' array,
    //also we create the array of maskar masks:
    GetTrianglesWithLevel(c, leveltrs, maskar);
    //in the 'indArray' array we keep indexes of edges of a contour of coverage,
    //through which there passes the line of level with
    GetLevelSgsKontOhvata(c, indArray);
    //we will construct boundary horizontals
    if(!CreateLevelBorderIzolines(c, indArray, leveltrs, maskar, count, type))
        return false;
    //we will construct internal horizontals
    if(!CreateLevelInnerIzolines(c, leveltrs, maskar, count, type))
        return false;

    // we release memory
    delete indArray;
    delete leveltrs;
    delete maskar;

    return true;
}


//==================================
//!Creation of horizontals of a layer on triangulable triangles.
/**the 'type'=true parameter means creation of the main horizontals: if 'type'=false,
//we build additional horizontals.
*/
bool MReliefLayer::CreateIzolines(double izomin, double izomax, bool type)
//==================================
{
    if(type) m_izolines.clear();
    else m_dopizolines.clear();

    double c; //  level line

    // three parameters are necessary for progress indexation below
    int collev = int((m_maxheight - m_minheight)/m_shag);
    if(collev==0) collev = 1;
    int k = 1;
    //'count' - the counter of number of level - changes from 0 to 3, if count=3,
    //all horizontals of this level - reinforced
    //we install the counter so that height of a reinforced horizontal was an integer
    int count= 0;
    double ostatok = izomin - (int)izomin;
    if(ostatok==0) count = 3;
    else if(ostatok==0.5) count = 2;
    else if(ostatok==0.25) count = 0;


    for(c=izomin; c<=izomax; c+=m_shag) {
        if(!CreateLevelIzolines(c, count, type))
            return false;

        //we care of information output in a state window
        GetKarta()->SetProgress(k*100/collev);
        GetKarta()->ShowProgress();
        k++;

        count++;
        if(count==4)
            count = 0;
    }

    return true;
}

//==================================
//!to issue the list of heights of the horizontals passing through an edge of the triangulable
//! a triangle, set by heights of the ends
/**INPUT PARAMETERS: 'h1' and 'h2' - heights of the ends of a segment;
//OUTPUT PARAMETER - MDuArray * izoheights - the array of heights of horizontals, which
//can pass through an entrance segment
*/
bool MReliefLayer::FindIzoheightsForSegment(double h1, double h2, MDuArray* izoheights)
//==================================
{
    //	izoheights->ClearAll();
    double ostatok = h1/m_shag - (int)h1/m_shag;//m_shag - relief section height
    double izlishek = ostatok*m_shag;//broke 'h1' into the part sharing totally on 'm_shag'
    //and 'izlishek'
    //we will find the first height of a horizontal
    double izoheight = h1 + m_shag - izlishek;
    izoheights->AddTail(&izoheight);
    izoheight += m_shag;
    //we will collect all heights on a segment
    while(izoheight<=h2){
        izoheights->AddTail(&izoheight);
        izoheight += m_shag;
    }
    if(izoheights->GetSize()==0)
        return false;
    return true;
}

//==================================
//!in a contour of a relief deletes an edge with 'number' numseg;
/** to any edge of a contour of a relief there corresponds the only boundary triangulable
//segment and, therefore, only triangulable triangle, which we and
//we will delete from triangulation area
*/
bool MReliefLayer::DeleteSegmentOfReliefKontur(int numseg)
{
    if((numseg<0)||(numseg>m_RelKonturPts->GetSize()-1))
        return false;

    MPoint ptseg1(m_RelKonturPts->GetAt(numseg));
    MPoint ptseg2(m_RelKonturPts->GetAt(numseg+1));
    int trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
    if(trisegind==-1)
        return false;

    MBoArray* sgmaskar = new MBoArray();
    MTrArray* sgtrar = new MTrArray();
    //we look for the only triangulable triangle corresponding to the boundary
    //to a triangulable segment;
    //we store tags of accessory of top of the triangulable in the 'sgmaskar' array
    //triangle to a triangulable segment (sequence of tags
    //corresponds to following of tops in a triangle, 'true' means accessory
    //tops to a triangulable segment
    FindTrianglesByTriSegment(m_segments->GetAt(trisegind), sgmaskar, sgtrar);
    if(sgtrar->GetSize()!=1){
        delete sgmaskar;
        delete sgtrar;
        return false;
    }

    int triind = m_triangles->Find(0,sgtrar->GetAt(0));
    if(triind==-1){
        delete sgmaskar;
        delete sgtrar;
        return false;
    }
    MTriangle* pTri = sgtrar->GetAt(0);

    //for the found triangulable triangle we find out, what his edge coincides
    //with the deleted relief contour edge, height mark belonging to a triangle
    //and not belonging to the deleted edge, we insert into a relief contour after a point
    //with number numseg, from the list the tringulyatsionnykh of triangles and segments
    //we delete the found triangle and a segment, respectively;
    //chance, when all three points of a triangulable triangle
    //belong to a contour; in this case we don't add to a contour of a relief any
    //points, and we delete the point belonging to two triangulable segments of a contour
    //a relief, from the list of triangulable segments to darings the second segment belonging
    //to a relief contour

    //we will consider all cases
    MPoint pt;
    int index, vertexind;
    //the zero edge of a triangle coincides with a relief contour edge
    if((*sgmaskar->GetAt(0))&&(*sgmaskar->GetAt(1))){
        //we will check, whether the third point of the triangulable belongs to a relief contour
        //triangle
        if(PTONKONTUR(pTri->m_tript3, m_RelKonturPts,index,vertexind)){
            //we will check on an average point, what of the remained triangle edges
            //coincides with a relief contour edge;
            pt.x = (pTri->m_tript3.x + pTri->m_tript1.x)/2;
            pt.y = (pTri->m_tript3.y + pTri->m_tript1.y)/2;
            //if the average point of an edge of a triangle lies on a relief contour,
            //that and all edge coincides with one of relief contour edges
            if(PTONKONTUR(pt, m_RelKonturPts,index,vertexind)){
                //we will find an index of the point of a contour of a relief belonging adjacent
                //to edges of a triangulable triangle which coincide with
                //relief contour edges, also we will delete it from a contour
                index = FindNomVershinaOfPointOnLomline(m_RelKonturPts, pTri->m_tript1);
                m_RelKonturPts->Delete(index);
                m_RelKonturHgt->Delete(index);
                //to mark of height deleted from triangulation area, we appoint
                //sign "not relief"
                MHeightPoint* pHpt = FindHeightptByCoords(pTri->m_tript1);
                MString str= "нерельефная";
                pHpt->m_dbarray.SetAt(0,&str);
                //we will delete the second found from the list of triangulable segments
                //the segment coinciding with a relief contour edge
                index = m_segments->FindByCoords(0,pTri->m_tript3,pTri->m_tript1);
                m_segments->Delete(index);
                trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
            }
            else{
                //we will check on an average point, what of the remained triangle edges
                //coincides with a relief contour edge;
                pt.x = (pTri->m_tript3.x + pTri->m_tript2.x)/2;
                pt.y = (pTri->m_tript3.y + pTri->m_tript2.y)/2;
                //we will check on an average point, what of the remained triangle edges
                //coincides with a relief contour edge;
                if(PTONKONTUR(pt, m_RelKonturPts,index,vertexind)){
                    //we will find an index of the point of a contour of a relief belonging adjacent
                    //to edges of a triangulable triangle which coincide with
                    //relief contour edges, also we will delete it from a contour
                    index = FindNomVershinaOfPointOnLomline(m_RelKonturPts, pTri->m_tript2);
                    m_RelKonturPts->Delete(index);
                    m_RelKonturHgt->Delete(index);
                    //to mark of height deleted from triangulation area, we appoint
                    //sign "not relief"
                    MHeightPoint* pHpt = FindHeightptByCoords(pTri->m_tript2);
                    MString str= "нерельефная";
                    pHpt->m_dbarray.SetAt(0,&str);
                    //we will delete the second found from the list of triangulable segments
                    //the segment coinciding with a relief contour edge
                    index = m_segments->FindByCoords(0,pTri->m_tript2,pTri->m_tript1);
                    m_segments->Delete(index);
                    trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
                }
                //case of an outstanding quadrangle in a relief contour,
                //removal of an edge will lead to an incorrect contour of a relief
                else{
                    delete sgmaskar;
                    delete sgtrar;
                    return false;
                }
            }
        }
        //the third point of a triangulable triangle doesn't belong
        //to a relief contour
        else{
            m_RelKonturPts->InsertAfter(numseg,&pTri->m_tript3);
            m_RelKonturHgt->InsertAfter(numseg,&pTri->m_triheight3);
        }
    }
    //the second edge of a triangle coincides with a relief contour edge
    else if((*sgmaskar->GetAt(0))&&(*sgmaskar->GetAt(2))){
        //we will check, whether the third point of the triangulable belongs to a relief contour
        //triangle
        if(PTONKONTUR(pTri->m_tript2, m_RelKonturPts,index,vertexind)){
            //we will check on an average point, what of the remained triangle edges
            //coincides with a relief contour edge;
            pt.x = (pTri->m_tript2.x + pTri->m_tript1.x)/2;
            pt.y = (pTri->m_tript2.y + pTri->m_tript1.y)/2;
            //if the average point of an edge of a triangle lies on a relief contour,
            //that and all edge coincides with one of relief contour edges
            if(PTONKONTUR(pt, m_RelKonturPts,index,vertexind)){
                //we will find an index of the point of a contour of a relief belonging adjacent
                //to edges of a triangulable triangle which coincide with
                //relief contour edges, also we will delete it from a contour
                index = FindNomVershinaOfPointOnLomline(m_RelKonturPts, pTri->m_tript1);
                m_RelKonturPts->Delete(index);
                m_RelKonturHgt->Delete(index);
                //to mark of height deleted from triangulation area, we appoint
                //sign "not relief"
                MHeightPoint* pHpt = FindHeightptByCoords(pTri->m_tript1);
                MString str= "нерельефная";
                pHpt->m_dbarray.SetAt(0,&str);
                //we will delete the second found from the list of triangulable segments
                //the segment coinciding with a relief contour edge
                index = m_segments->FindByCoords(0,pTri->m_tript2,pTri->m_tript1);
                m_segments->Delete(index);
                trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
            }
            else{
                //we will check on an average point, what of the remained triangle edges
                //coincides with a relief contour edge;
                pt.x = (pTri->m_tript3.x + pTri->m_tript2.x)/2;
                pt.y = (pTri->m_tript3.y + pTri->m_tript2.y)/2;
                //if the average point of an edge of a triangle lies on a relief contour,
                //that and all edge coincides with one of relief contour edges
                if(PTONKONTUR(pt, m_RelKonturPts,index,vertexind)){
                    //we will find an index of the point of a contour of a relief belonging adjacent
                    //to edges of a triangulable triangle which coincide with
                    //relief contour edges, also we will delete it from a contour
                    index = FindNomVershinaOfPointOnLomline(m_RelKonturPts, pTri->m_tript3);
                    m_RelKonturPts->Delete(index);
                    m_RelKonturHgt->Delete(index);
                    //to mark of height deleted from triangulation area, we appoint
                    //sign "not relief"
                    MHeightPoint* pHpt = FindHeightptByCoords(pTri->m_tript3);
                    MString str= "нерельефная";
                    pHpt->m_dbarray.SetAt(0,&str);
                    //we will delete the second found from the list of triangulable segments
                    //the segment coinciding with a relief contour edge
                    index = m_segments->FindByCoords(0,pTri->m_tript3,pTri->m_tript2);
                    m_segments->Delete(index);
                    trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
                }
                //case of an outstanding quadrangle in a relief contour,
                //removal of an edge will lead to an incorrect contour of a relief
                else{
                    delete sgmaskar;
                    delete sgtrar;
                    return false;
                }
            }
        }
        //the third point of a triangulable triangle doesn't belong
        //to a relief contour
        else{
            m_RelKonturPts->InsertAfter(numseg,&pTri->m_tript2);
            m_RelKonturHgt->InsertAfter(numseg,&pTri->m_triheight2);
        }
    }
    //the zero edge of a triangle coincides with a relief contour edge
    else if((*sgmaskar->GetAt(1))&&(*sgmaskar->GetAt(2))){
        //we will check, whether the third point of the triangulable belongs to a relief contour
        //triangle
        if(PTONKONTUR(pTri->m_tript1, m_RelKonturPts,index,vertexind)){
            //we will check on an average point, what of the remained triangle edges
            //coincides with a relief contour edge;
            pt.x = (pTri->m_tript1.x + pTri->m_tript2.x)/2;
            pt.y = (pTri->m_tript1.y + pTri->m_tript2.y)/2;
            //if the average point of an edge of a triangle lies on a relief contour,
            //that and all edge coincides with one of relief contour edges
            if(PTONKONTUR(pt, m_RelKonturPts,index,vertexind)){
                //we will find an index of the point of a contour of a relief belonging adjacent
                //to edges of a triangulable triangle which coincide with
                //relief contour edges, also we will delete it from a contour
                index = FindNomVershinaOfPointOnLomline(m_RelKonturPts, pTri->m_tript2);
                m_RelKonturPts->Delete(index);
                m_RelKonturHgt->Delete(index);
                //to mark of height deleted from triangulation area, we appoint
                //sign "not relief"
                MHeightPoint* pHpt = FindHeightptByCoords(pTri->m_tript2);
                MString str= "нерельефная";
                pHpt->m_dbarray.SetAt(0,&str);
                //we will delete the second found from the list of triangulable segments
                //the segment coinciding with a relief contour edge
                index = m_segments->FindByCoords(0,pTri->m_tript1,pTri->m_tript2);
                m_segments->Delete(index);
                trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
            }
            else{
                //we will check on an average point, what of the remained triangle edges
                //coincides with a relief contour edge;
                pt.x = (pTri->m_tript1.x + pTri->m_tript3.x)/2;
                pt.y = (pTri->m_tript1.y + pTri->m_tript3.y)/2;
                //if the average point of an edge of a triangle lies on a relief contour,
                //that and all edge coincides with one of relief contour edges
                if(PTONKONTUR(pt, m_RelKonturPts,index,vertexind)){
                    //we will find an index of the point of a contour of a relief belonging adjacent
                    //to edges of a triangulable triangle which coincide with
                    //relief contour edges, also we will delete it from a contour
                    index = FindNomVershinaOfPointOnLomline(m_RelKonturPts, pTri->m_tript3);
                    m_RelKonturPts->Delete(index);
                    m_RelKonturHgt->Delete(index);
                    //to mark of height deleted from triangulation area, we appoint
                    //sign "not relief"
                    MHeightPoint* pHpt = FindHeightptByCoords(pTri->m_tript3);
                    MString str= "нерельефная";
                    pHpt->m_dbarray.SetAt(0,&str);
                    //we will delete the second found from the list of triangulable segments
                    //the segment coinciding with a relief contour edge
                    index = m_segments->FindByCoords(0,pTri->m_tript1,pTri->m_tript3);
                    m_segments->Delete(index);
                    trisegind = m_segments->FindByCoords(0,ptseg1,ptseg2);
                }
                //case of an outstanding quadrangle in a relief contour,
                //removal of an edge will lead to an incorrect contour of a relief
                else{
                    delete sgmaskar;
                    delete sgtrar;
                    return false;
                }
            }
        }
        //the third point of a triangulable triangle doesn't belong
        //to a relief contour

        else{
            m_RelKonturPts->InsertAfter(numseg,&pTri->m_tript1);
            m_RelKonturHgt->InsertAfter(numseg,&pTri->m_triheight1);
        }
    }
    m_segments->Delete(trisegind);
    m_triangles->Delete(triind);

    delete sgmaskar;
    delete sgtrar;
    return true;
}

//==================================
//!in the structural line with number 'numline' deletes a segment with number 'numseg',
/**thus the structural line or is broken off on two, or from it is deleted
//an initial segment, or final, or if the structural line consisted
//from one this segment, she leaves from the array of structural lines
//the 'delpar' parameter accepts true value, if together with an entrance segment
//also the structural line is deleted, i.e. the structural line consists from
//one segment
*/
bool MReliefLayer::DeleteSegmentOfStructline(int numline, int numseg, bool& delpar)
//==================================
{
    if((numline<0)||(numline>m_structlines->GetSize()-1))
        return false;
    MPtArray* structline = (MPtArray*)m_structlines->GetAt(numline);
    if((numseg<0)||(numseg>structline->GetSize()-2))
        return false;
    //initially we assume that removal of an edge won't entail removal
    //structural line
    delpar = false;
    //after removal from the structural line of a segment, structural
    //the line will break into two new: to a remote segment and after
    //remote segment; if the deleted segment belongs
    //by the beginning or by the end of the structural line, it is enough from it
    //to delete an initial or final point
    //initial edge
    if(numseg==0){
        structline->Delete(0);
        //if in the structural line there was only one point...
        if(structline->GetSize() == 1){
            m_structlines->Delete(numline);
            delpar = true;
        }
    }
    //final edge
    else if(numseg==structline->GetSize()-2)
        structline->Delete(structline->GetSize()-1);
    else{
        MPtArray* finline =new MPtArray();
        for(int i=numseg+1; i<structline->GetSize(); i++)
            finline->AddTail(structline->GetAt(i));
        //we delete a tail from the initial structural line
        while(structline->GetSize()>numseg+1)
            structline->DeleteTail();
        //we will add the new structural line to the array (to fix it
        //it isn't necessary since all its segments are already  fixed)
        m_structlines->AddTail((MArray*)finline);
    }

    return true;
}

//==================================
//!whether on a triangulable segment answers this segment belongs
//! structural line
/**ENTRANCE: 'pSeg' - a triangulable segment;
// EXIT: 'numline' - an index of the structural line to which belongs
//       a triangulable segment, in the array of structural lines,
//       'numseg' - number of a segment of the structural line, coincident
//       with an entrance triangulable segment
*/
bool MReliefLayer::IsTriSegmentOfStructline(MSegment* pSeg, int& numline,
                                            int& numseg)
//==================================
{
    if(!pSeg)
        return true;

    MPtArray* structkontur;

    for(int i=0; i<m_structlines->GetSize(); i++){
        structkontur = (MPtArray*)m_structlines->GetAt(i);
        for(int j=0; j<structkontur->GetSize()-1; j++){
            if(*structkontur->GetAt(j) == pSeg->m_sgpt1){
                if(*structkontur->GetAt(j+1) == pSeg->m_sgpt2){
                    numline = i;
                    numseg = j;
                    return true;
                }
            }
            else if(*structkontur->GetAt(j) == pSeg->m_sgpt2){
                if(*structkontur->GetAt(j+1) == pSeg->m_sgpt1){
                    numline = i;
                    numseg = j;
                    return true;
                }
            }
        }
    }

    return false;
}


//==================================
//!whether on a triangulable segment answers the macroform possesses this segment
bool MReliefLayer::IsTriSegmentOfMacroforma(MSegment* pSeg)
//==================================
{
    if(!pSeg)
        return true;

    MPtArray* structkontur;
    OBITER it = m_macroforms.begin();
    while(it != m_macroforms.end())
    {
        structkontur = ((MObryv*)(*it))->GetStructKontur();
        for(int i=0; i<structkontur->GetSize()-1; i++){
            if(*structkontur->GetAt(i) == pSeg->m_sgpt1){
                if(*structkontur->GetAt(i+1) == pSeg->m_sgpt2)
                    return true;
            }
            if(*structkontur->GetAt(i) == pSeg->m_sgpt2){
                if(*structkontur->GetAt(i+1) == pSeg->m_sgpt1)
                    return true;
            }
        }
        if((*it)->WhatTheObject() == ID_YAMA){
            structkontur = ((MYama*)(*it))->GetDownStructKontur();
            for(int i=0; i<structkontur->GetSize()-1; i++){
                if(*structkontur->GetAt(i) == pSeg->m_sgpt1){
                    if(*structkontur->GetAt(i+1) == pSeg->m_sgpt2)
                        return true;
                }
                if(*structkontur->GetAt(i) == pSeg->m_sgpt2){
                    if(*structkontur->GetAt(i+1) == pSeg->m_sgpt1)
                        return true;
                }
            }
        }
        it++;
    }

    it = m_gidroobjects.begin();
    while(it != m_gidroobjects.end())
    {
        if((*it)->WhatTheObject()==ID_GIDRO){
            structkontur = ((MGidro*)(*it))->GetStructKontur();
            for(int i=0; i<structkontur->GetSize()-1; i++){
                if(*structkontur->GetAt(i) == pSeg->m_sgpt1){
                    if(*structkontur->GetAt(i+1) == pSeg->m_sgpt2)
                        return true;
                }
                if(*structkontur->GetAt(i) == pSeg->m_sgpt2){
                    if(*structkontur->GetAt(i+1) == pSeg->m_sgpt1)
                        return true;
                }
            }
        }
        it++;
    }

    return false;
}

//==================================
//!checks, whether it is possible to fix an entrance segment;
/**crossing of the entrance is for this purpose admissible
//a segment with the structural line or a contour of macroforms of a relief only in tops
//(so that the point of intersection at the same time coincided with one of the ends of a segment and top
//structural line or contour of a macroform)
*/
bool MReliefLayer::IsPermissibleSegment(MPoint pt1, MPoint pt2)
//==================================
{
    MPtArray* structkontur{nullptr};
    MPtArray* crosspts = new MPtArray();

    //we check crossing with structural lines
    for(int i=0; i<m_structlines->GetSize(); i++){
        structkontur = (MPtArray*)m_structlines->GetAt(i);
        if(FindNotVertexCrossSegWithKontur(pt1,pt2,structkontur,crosspts))
            return false;
    }

    //we check crossing with contours of macroforms
    OBITER it = m_macroforms.begin();
    while(it != m_macroforms.end())
    {
        structkontur = ((MObryv*)*it)->GetStructKontur();
        if(FindNotVertexCrossSegWithKontur(pt1,pt2,structkontur,crosspts))
            return false;
        if((*it)->WhatTheObject()==ID_YAMA){
            structkontur = ((MYama*)*it)->GetDownStructKontur();
            if(FindNotVertexCrossSegWithKontur(pt1,pt2,structkontur,crosspts))
                return false;
        }
        it++;
    }

    it = m_gidroobjects.begin();
    while(it != m_gidroobjects.end())
    {
        structkontur = ((MGidro*)*it)->GetStructKontur();
        if(FindNotVertexCrossSegWithKontur(pt1,pt2,structkontur,crosspts))
            return false;
        it++;
    }

    delete crosspts;
    return true;
}


//==================================
//!on a triangulable segment finds triangulable triangles,
//! which possess an entrance segment
/**ENTRANCE: 'pSegment' - a triangulable segment;
//EXIT: 'sgtrar' - the array of triangulable triangles to which belongs
//      an entrance segment (these triangles can be or two adjacent
//      on an entrance segment, or one boundary, for which entrance
//      the segment belongs to border)
//      'sgmaskar' - the array of Boolean variables - on three variables on everyone
//      the triangle-true means coincidence of an entrance segment with
//      corresponding edge of a triangle
*/
bool MReliefLayer::FindTrianglesByTriSegment(MSegment* pSegment, MBoArray* sgmaskar,
                                             MTrArray* sgtrar)
//==================================
{
    //in case of zero pointers
    if((!pSegment)||(!sgmaskar)||(!sgtrar))
        return false;

    //we look for the triangles containing an entrance triangulable segment
    //we include them in the 'sgtrar' array, we mark numbers of the tops belonging
    //to an entrance segment by means of the Boolean 'sgmaskar' array
    for(int i=0; i<m_triangles->GetSize(); i++){
        MTriangle* pTriHpt = m_triangles->GetAt(i);
        //if the segment coincided with a zero edge of a triangle...
        if(((pSegment->m_sgpt1 == pTriHpt->m_tript1)&&(pSegment->m_sgpt2 == pTriHpt->m_tript2))||
                ((pSegment->m_sgpt1 == pTriHpt->m_tript2)&&(pSegment->m_sgpt2 == pTriHpt->m_tript1))){
            sgtrar->AddTail(pTriHpt);
            bool par = true;
            sgmaskar->AddTail(&par);
            sgmaskar->AddTail(&par);
            par = false;
            sgmaskar->AddTail(&par);
            if(sgtrar->GetSize() == 2)
                break;
        }
        //if the segment coincided with the first edge of a triangle...
        else if(((pSegment->m_sgpt1 == pTriHpt->m_tript1)&&(pSegment->m_sgpt2 == pTriHpt->m_tript3))||
                ((pSegment->m_sgpt1 == pTriHpt->m_tript3)&&(pSegment->m_sgpt2 == pTriHpt->m_tript1))){
            sgtrar->AddTail(pTriHpt);
            bool par = true;
            sgmaskar->AddTail(&par);
            par = false;
            sgmaskar->AddTail(&par);
            par = true;
            sgmaskar->AddTail(&par);
            if(sgtrar->GetSize() == 2)
                break;
        }
        //if the segment coincided with the second edge of a triangle...
        else if(((pSegment->m_sgpt1 == pTriHpt->m_tript2)&&(pSegment->m_sgpt2 == pTriHpt->m_tript3))||
                ((pSegment->m_sgpt1 == pTriHpt->m_tript3)&&(pSegment->m_sgpt2 == pTriHpt->m_tript2))){
            sgtrar->AddTail(pTriHpt);
            bool par = false;
            sgmaskar->AddTail(&par);
            par = true;
            sgmaskar->AddTail(&par);
            sgmaskar->AddTail(&par);
            if(sgtrar->GetSize() == 2)
                break;
        }
    }

    //if the triangulable segment was set incorrectly
    if(sgtrar->GetSize()==0)
        return false;

    return true;
}


//==================================
//!on coordinates of a point finds triangulable triangles,
//! which possess a point, and changes height of top of a triangle, coincident
//! with an entrance point 'pt', on entrance value of 'height' height
bool MReliefLayer::SetTriTrianglesPtHeight(MPoint pt, double height)
//ENTRANCE: 'pt' - a point,
//'height' - the new height of top of a triangle
//==================================
{
    //we look for the triangles containing an entrance point, and we change top height
    //a triangle, coinciding with an entrance point
    for(int i=0; i<m_triangles->GetSize(); i++){
        MTriangle* pTriHpt = m_triangles->GetAt(i);
        //if the point coincided with zero top of a triangle...
        if(pt == pTriHpt->m_tript1)
            pTriHpt->m_triheight1 = height;
        //if the point coincided with the first top of a triangle...
        else if(pt == pTriHpt->m_tript2)
            pTriHpt->m_triheight2 = height;
        //if the point coincided with the second top of a triangle...
        else if(pt == pTriHpt->m_tript3)
            pTriHpt->m_triheight3 = height;
    }

    return true;
}

//==================================
//!on coordinates of a point finds triangulable segments,
//! which possess a point, and changes height of the end of a segment, coincident
//! with an entrance point, on entrance value of height
bool MReliefLayer::SetTriSegmentsPtHeight(MPoint pt, double height)
//ENTRANCE: 'pt' - a point,
//'height' - the new height of top of a triangle
//==================================
{
    //we look for the segments containing an entrance point, and we change height of the end of a segment,
    //coinciding with an entrance point
    for(int i=0; i<m_segments->GetSize(); i++){
        MSegment* pSgHpt = m_segments->GetAt(i);
        //if the point coincided with zero top of a segment...
        if(pt == pSgHpt->m_sgpt1)
            pSgHpt->m_sgheight1 = height;
        //if the point coincided with the first top of a segment...
        else if(pt == pSgHpt->m_sgpt2)
            pSgHpt->m_sgheight2 = height;
    }

    return true;
}

//==================================
//!Change of an edge of a triangulation
bool MReliefLayer::ChangeRebroTriangulyatsii(MSegment* pSegment)
//==================================
{
    if(m_triangles == NULL) return false;

    if(pSegment == NULL) return false;

    //'sgtrar' - the array in which we will store the triangles containing a changeable edge of a triangulation
    MTrArray* sgtrar = new MTrArray();
    //'sgmaskar' - the Boolean array in which we will store masks on edges of triangles,
    //coinciding with a changeable edge
    MBoArray* sgmaskar = new MBoArray();
    //we look for the triangles containing a changeable edge; as soon as two such
    //a triangle are found, we include them in the 'sgtrar' array, we mark numbers of the tops containing an edge
    //by means of the Boolean 'sgmaskar' array
    FindTrianglesByTriSegment(pSegment,sgmaskar,sgtrar);

    // situation when the edge belonging to a boundary triangle is allocated
    if(sgtrar->GetSize() == 1) {
        delete sgtrar;
        delete sgmaskar;
        InternalMessage(RLF_MESSAGE5);//"The edge doesn't belong to adjacent triangles!"
        return false;
    }

    //the array of heights of horizontals which can pass through a quadrangle contour,
    //containing an entrance edge
    MDuArray* izoheights = new MDuArray();
    //the array of heights of horizontals which can pass through a quadrangle contour,
    //containing an entrance edge, crude; the same height can enter it
    //more than once
    MDuArray* heights = new MDuArray();

    int i = 0;
    for(i=0; i<sgtrar->GetSize(); i++){
        MTriangle* pTri = sgtrar->GetAt(i);
        //we will add to the heights array of height of horizontals which can pass
        //through the first edge of a triangle
        FindIzoheightsForSegment(pTri->m_triheight2,pTri->m_triheight3,heights);
        //we will add to the heights array of height of horizontals which can pass
        //through the second edge of a triangle
        FindIzoheightsForSegment(pTri->m_triheight3,pTri->m_triheight1,heights);
        //to the final array of the reconstructed heights each height has to be
        //it is included no more once
        for(int j=0; j<heights->GetSize(); j++){
            if(izoheights->Find(0, heights->GetAt(j))==-1)
                izoheights->AddTail(heights->GetAt(j));
        }
        //we will exclude the found triangle from triangulable triangles
        int ind = m_triangles->Find(0, pTri);
        m_triangles->Delete(ind);
    }

    delete heights;

    // we change an edge in the 'm_segments' array, the ends of a new edge - "not marked" tops of the chosen adjacent triangles
    MSegment* pNewHpt = new MSegment();

    for(i=0; i<2; i++){
        for(int j=0; j<3; j++){
            if(!sgmaskar->GetAt(3*i+j)[0]){
                MPoint pt;
                double h= -999.9;
                if(j==0){
                    pt = sgtrar->GetAt(i)->m_tript1;
                    h = sgtrar->GetAt(i)->m_triheight1;
                }
                if(j==1){
                    pt = sgtrar->GetAt(i)->m_tript2;
                    h = sgtrar->GetAt(i)->m_triheight2;
                }
                if(j==2){
                    pt = sgtrar->GetAt(i)->m_tript3;
                    h = sgtrar->GetAt(i)->m_triheight3;
                }
                if(i==0){
                    pNewHpt->m_sgpt1 = pt;
                    pNewHpt->m_sgheight1 = h;
                    break;
                }
                if(i==1){
                    pNewHpt->m_sgpt2 = pt;
                    pNewHpt->m_sgheight2 = h;
                    break;
                }
            }
        }
    }

    // check that the quadrangle formed by two triangles containing the changed edge was convex;
    //if it not so, we return everything on places

    if(ZNAK(OTS(pNewHpt->m_sgpt1, pSegment->m_sgpt1, pNewHpt->m_sgpt2)) == ZNAK(OTS(pNewHpt->m_sgpt1, pSegment->m_sgpt2, pNewHpt->m_sgpt2))){
        InternalMessage(RLF_MESSAGE6);//"The edge doesn't belong to a convex quadrangle!"
        m_triangles->AppendTail(sgtrar);
        sgtrar->ClearAll();
        sgmaskar->ClearAll();
        delete sgtrar;
        delete sgmaskar;
        return false;
    }

    // we add two new triangles containing a new edge instead of two excluded to the 'm_triangles' array
    AddTriangle(pSegment->m_sgpt1, pNewHpt->m_sgpt1, pNewHpt->m_sgpt2, pSegment->m_sgheight1, pNewHpt->m_sgheight1, pNewHpt->m_sgheight2);
    AddTriangle(pSegment->m_sgpt2, pNewHpt->m_sgpt1, pNewHpt->m_sgpt2, pSegment->m_sgheight2, pNewHpt->m_sgheight1, pNewHpt->m_sgheight2);

    // we focus all new triangles clockwise
    double h;
    MPoint pt;
    MTriangle* pTriHpt = m_triangles->GetAt(m_triangles->GetSize()-1);

    if(OTS(pTriHpt->m_tript1, pTriHpt->m_tript2, pTriHpt->m_tript3)>0){
        pt = pTriHpt->m_tript2;
        h = pTriHpt->m_triheight2;
        pTriHpt->m_tript2 = pTriHpt->m_tript3;
        pTriHpt->m_tript3 = pt;
        pTriHpt->m_triheight2 = pTriHpt->m_triheight3;
        pTriHpt->m_triheight3 = h;
    }

    pTriHpt = m_triangles->GetAt(m_triangles->GetSize()-2);
    if(OTS(pTriHpt->m_tript1, pTriHpt->m_tript2, pTriHpt->m_tript3)>0){
        pt = pTriHpt->m_tript2;
        h = pTriHpt->m_triheight2;
        pTriHpt->m_tript2 = pTriHpt->m_tript3;
        pTriHpt->m_tript3 = pt;
        pTriHpt->m_triheight2 = pTriHpt->m_triheight3;
        pTriHpt->m_triheight3 = h;
    }

    //we will find an index of the deleted edge
    int delsgind = m_segments->Find(0, pSegment);
    m_segments->Delete(delsgind);
    m_segments->AddTail(pNewHpt);

    sgtrar->ClearAll();
    sgmaskar->ClearAll();
    delete sgtrar;
    delete sgmaskar;
    delete pNewHpt;

    //we will allocate horizontals with height which isn't entering the 'izoheights' list
    //the others for correct removal in the 'delizolines' list
    OBITER it = m_izolines.begin();
    MString strheight;
    double height;
    MObjectList newizolines;
    MObjectList delizolines;
    while (it != m_izolines.end())
    {
        ((MIzoline*)(*it))->GetizoHeight(&strheight);
        height = atof(strheight);
        if(izoheights->Find(0,&height)==-1)
            newizolines.push_back(*it);
        else delizolines.push_back(*it);
        it++;
    }
    m_izolines.swap(newizolines);

    //we delete the reconstructed horizontals
    it = delizolines.begin();
    while (it != delizolines.end())
    {
        delete (*it);
        it++;
    }
    delizolines.clear();

    //we reconstruct a picture of horizontals
    for(i=0; i<izoheights->GetSize();i++) {
        height = *izoheights->GetAt(i);
        if(!CreateLevelIzolines(height, 0, true))
            return false;
    }

    delete izoheights;

    return true;
}

//==================================
//! determination of height of any point 'pt' in a relief layer;
/** ALGORITHM: 1) we define a triangulable triangle to which the point got;
//             2) in a triangle we choose tops from the minimum 'h1' and the maximum 'h2' as height;
//             3) from top with the minimum height 'h1' we draw a straight line before crossing with
//                an opposite edge in a point with the same height (i.e. we restore
//                the isoline with 'h1' height);
//REMARK: all lines of identical heights in a triangle will be parallel to the constructed straight line
//        (follows from similarity of triangles);
//             4) from the set point 'pt' it is restored the straight line parallel received
//                the 'h1' isolines before crossing with opposite to triangle top with 'h1' height an edge;
//             5) we determine height of the received point of intersection is and there will be 'pt' point height.
*/
double MReliefLayer::GetValueFromRelief(MPoint pt)
//==================================
{	
    if(m_triangles->GetSize()==0){
        //		InternalMessage("Слой рельефа не сформирован!", MB_OK, 0);
        return -100000;
    }

    MSegment* pSeg =NULL;
    MTriangle* pTrg =NULL;
    double height= -999.0;
    int index=-1;

    // we will check, whether our point on an edge triangulable triangle lies and we will calculate its height if lies
    MPoint perppt;
    int i = 0;
    for(i=0; i<m_segments->GetSize(); i++){
        pSeg = m_segments->GetAt(i);
        // if our point lies on a triangulable segment...
        if(DLINASG(pt, pSeg->m_sgpt1) + DLINASG(pt, pSeg->m_sgpt2) - DLINASG(pSeg->m_sgpt1, pSeg->m_sgpt2) == 0){
            index = i;
            perppt = GetPointOnOtrezok(pSeg->m_sgpt1, pSeg->m_sgpt2, pt);
            break;
        }
    }

    if(index!=-1){
        // we will determine 'pt' point height
        height = HGTPT(pSeg->m_sgpt1, pSeg->m_sgpt2, perppt, pSeg->m_sgheight1, pSeg->m_sgheight2);
        return height;
    }

    //	if our point doesn't lie on a triangulable segment...
    for(i=0; i<m_triangles->GetSize(); i++){
        pTrg = m_triangles->GetAt(i);
        MPoint trg[3];
        trg[0] = pTrg->m_tript1;
        trg[1] = pTrg->m_tript2;
        trg[2] = pTrg->m_tript3;
        // we touch all triangulable triangles, we won't find yet to what the point got
        if(pt.InnerPt(trg, 3)){
            index = i;
            break;
        }
    }

    if(index==-1){
        return -100000;
    }

    double hmin, hmax;
    int indmin, indmax;
    MPoint ptmin, ptmax, ptindex;

    ptmin = pTrg->m_tript1;
    ptmax = pTrg->m_tript1;
    hmin = hmax = pTrg->m_triheight1;
    indmin = indmax = 0;
    for(i=1; i<3; i++){
        if(i==1){
            ptindex = pTrg->m_tript2;
            height = pTrg->m_triheight2;
            index = 1;
        }
        if(i==2){
            ptindex = pTrg->m_tript3;
            height = pTrg->m_triheight3;
            index = 2;
        }
        if(height <= hmin) {
            ptmin = ptindex;
            hmin = height;
            indmin = index;
        }
        if(height > hmax) {
            ptmax = ptindex;
            hmax = height;
            indmax = index;
        }
    }
    for(i=0; i<3; i++){
        if((i != indmin) && (i != indmax)){
            index = i;
            switch (index){
            case 0:
                height = pTrg->m_triheight1;
                ptindex = pTrg->m_tript1;
                break;
            case 1:
                height = pTrg->m_triheight2;
                ptindex = pTrg->m_tript2;
                break;
            case 2:
                height = pTrg->m_triheight3;
                ptindex = pTrg->m_tript3;
                break;
            }
        }
    }

    // we will determine 'pth1' point coordinates with 'height' height lying on an edge, opposite to 'index' top
    MPoint pth1;
    pth1 = COOPT(ptmax, ptmin, hmax, hmin, height);

    double A1, B1, C1, A2, B2, C2;
    // we will determine coefficients of the equation of a straight line of A1, B1, C1 from point 4) ALGORITHM (all calculations by Bronshteyna-Semendyaev's reference book)
    A1 = (pth1.y - ptindex.y)/(pth1.x - ptindex.x);
    B1 = -1;
    C1 = pt.y - pt.x*A1;

    // we will determine coefficients of the equation of a straight line of A2, B2, C2 passing through opposite to top with intermediate height an edge (all calculations by Bronshteyna-Semendyaev's reference book)
    A2 = ptmin.y - ptmax.y;
    B2 = (-1)*(ptmin.x - ptmax.x);
    C2 = (-1)*ptmax.x*A2 - ptmax.y*B2;

    MPoint pt1;
    // we will define a point of intersection of 'pt1' of the straight lines set by coefficients of A1, B1, C1, A2, B2, C2
    CROSSLS(A1, B1, C1, A2, B2, C2, &pt1);

    // we will determine 'pt1' point height, and also 'pt'
    height = HGTPT(ptmax, ptmin, &pt1, hmax, hmin);


    return height;
}


//==================================
//! determination of height of any point 'pt' in a relief layer, proceeding from horizontals;
double MReliefLayer::GetValueFromReliefByIzolines(MPoint pt)
//==================================
{	
    if(m_izolines.size()==0)
        return -100000;

    //if there is a relief contour, will check that the point got inside or belonged
    //to a contour
    int index, vertexind;
    if(m_RelKonturPts->GetSize() != 0){
        if((!PointInRegion(pt, m_RelKonturPts))&&(!PTONKONTUR(pt, m_RelKonturPts, index, vertexind)))
            return -100000;
    }
    double height;

    //we will check, whether our point coincides with any mark of height
    MHeightPoint* pHpt;
    pHpt = FindHeightptByCoords(pt);
    if(pHpt != NULL){
        height = pHpt->GetHeight();
        return height;
    }

    //if additional horizontals are constructed, we will unite both lists of horizontals in one
    MObjectList izolines;
    izolines.insert(izolines.end(), m_izolines.begin(), m_izolines.end());
    izolines.insert(izolines.end(), m_dopizolines.begin(), m_dopizolines.end());

    //we will find the horizontal, next to a point
    MObject* pObj;
    MPtArray* pLomline;
    MPoint resultpt, izopt1;
    double dist, mindist;
    int minizoind;
    MIzoline* pIzo1;
    OBITER it = izolines.begin();
    pObj = *it;
    pIzo1 = (MIzoline*)pObj;
    mindist = ((MIzoline*)pObj)->FindDistanceFromPoint(pt, izopt1, &minizoind);
    it++;
    while(it != izolines.end())
    {
        pObj = *it;
        dist = ((MIzoline*)pObj)->FindDistanceFromPoint(pt, resultpt, &index);
        if(dist<mindist){
            pIzo1 = (MIzoline*)pObj;
            mindist = dist;
            izopt1 = resultpt;
            minizoind = index;
        }
        it++;
    }

    //we will find a horizontal, the second for proximity, to a point such that the segment restored
    //from a point to a horizontal, doesn't cross the found horizontal, next to a point;
    //at first we will find the first horizontal meeting a condition in the list of horizontals
    pLomline = (MPtArray*)pIzo1->m_Komplekt->GetAt(((MIzoline*)pObj)->F_TOCHKI);
    MIzoline* pIzo2 = NULL;
    MPoint izopt2;
    it = izolines.begin();
    MPtArray* crosspts = new MPtArray();
    while(it != izolines.end())
    {
        //we exclude the found next horizontal from consideration
        if(*it != pIzo1){
            dist = ((MIzoline*)(*it))->FindDistanceFromPoint(pt, resultpt, &index);
            crosspts->ClearAll();
            if(!FindCrossSegWithKontur(pt, resultpt, pLomline, crosspts, NULL)){
                pIzo2 = (MIzoline*)(*it);
                izopt2 = resultpt;
                mindist = dist;
                break;
            }
        }
        it++;
    }
    //from all horizontals meeting a condition, we choose the next to a point
    while(it != izolines.end())
    {
        //we exclude the found next horizontal from consideration
        if(*it != pIzo1){
            dist = ((MIzoline*)(*it))->FindDistanceFromPoint(pt, resultpt, &index);
            crosspts->ClearAll();
            if(!FindCrossSegWithKontur(pt, resultpt, pLomline, crosspts, NULL)){
                if(dist<mindist){
                    pIzo2 = (MIzoline*)(*it);
                    izopt2 = resultpt;
                    mindist = dist;
                }
            }
        }
        it++;
    }

    //if there was a horizontal meeting a condition
    if(pIzo2 != NULL){
        //we will determine heights of horizontals
        MString str1, str2;
        pIzo1->GetizoHeight(&str1);
        double izoheight1 = atof(str1);
        pIzo2->GetizoHeight(&str2);
        double izoheight2 = atof(str2);
        //if heights of horizontals were identical, so the entrance point lies on
        //saddle; we will find the next to it height mark which is also lying between two
        //the found horizontals
        if(izoheight1 == izoheight2){
            it = m_heightpoints.begin();
            MPtArray* crosspts = new MPtArray();
            MHeightPoint* pHpt =NULL;
            MPtArray* pLomline1 = (MPtArray*)pIzo2->m_Komplekt->GetAt(pIzo2->F_TOCHKI);
            while(it != m_heightpoints.end())
            {
                if((!FindCrossSegWithKontur(pt, ((MHeightPoint*)(*it))->GetPoint(0), pLomline, crosspts, NULL))&&
                        (!FindCrossSegWithKontur(pt, ((MHeightPoint*)(*it))->GetPoint(0), pLomline1, crosspts, NULL))){

                    pHpt = (MHeightPoint*)(*it);
                    mindist = DLINASG(pt, pHpt->GetPoint(0));
                    break;
                }
                it++;
            }
            //from all marks of heights meeting a condition, we choose the next to a point
            while(it != m_heightpoints.end())
            {
                if((!FindCrossSegWithKontur(pt, ((MHeightPoint*)(*it))->GetPoint(0), pLomline, crosspts, NULL))&&
                        (!FindCrossSegWithKontur(pt, ((MHeightPoint*)(*it))->GetPoint(0), pLomline1, crosspts, NULL))){
                    dist = DLINASG(pt, ((MHeightPoint*)(*it))->GetPoint(0));
                    if(dist<mindist){
                        pHpt = (MHeightPoint*)pObj;
                        mindist = dist;
                    }
                }
                it++;
            }
            //we will restore a perpendicular from height mark on the segment connecting the bases of segments
            //restored from a point to the next found horizontals;
            //height of the received point same, as well as at height mark
            MPoint perphpt = GetPointOnOtrezok(izopt1, izopt2, pHpt->GetPoint(0));
            //we will restore a perpendicular from a point on the segment connecting the bases of segments
            //restored from a point to the next found horizontals
            MPoint perppt = GetPointOnOtrezok(izopt1, izopt2, pt);
            //we determine height of a point, proceeding from on what segment the basis got
            //a perpendicular from a point
            if(PTONSEG(perppt, izopt1, perphpt)){
                height = HGTPT(izopt1, perphpt, perppt, izoheight1, pHpt->GetHeight());
                return height;
            }
            else{
                height = HGTPT(izopt2, perphpt, perppt, izoheight2, pHpt->GetHeight());
                return height;
            }

        }
        //if heights of horizontals were various
        else{
            //we will restore a perpendicular from a point on the segment connecting the bases of segments
            //restored from a point to the next found horizontals
            MPoint perppt = GetPointOnOtrezok(izopt1, izopt2, pt);
            //we will determine perpendicular basis height, height of an entrance point coincides with it
            height = HGTPT(izopt1, izopt2, perppt, izoheight1, izoheight2);
            return height;
        }
    }

    //if the point lies in the area limited by the closed horizontal, and wasn't
    //any horizontal meeting a condition, we will find height mark, next to a point,
    //lying also in area, a limited horizontal
    MString str;
    MPoint perppt, hpt;
    double izoheight1, izoheight2;
    MObjectList innerpts;
    pIzo1->GetizoType(&str);
    if(str == "внутренняя"){
        MPoint hpt;
        MHeightPoint* pHpt;
        it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            hpt = ((MHeightPoint*)(*it))->GetPoint(0);
            if(PointInRegion(hpt, pLomline))
                innerpts.push_back(*it);
            it++;
        }
        //if in area there was no mark of height
        if(innerpts.size()==0){
            pIzo1->GetizoHeight(&str);
            height = atof(str);
            return height;
        }
        it = innerpts.begin();
        pHpt = (MHeightPoint*)(*it);
        mindist = DLINASG(pt, pHpt->GetPoint(0));
        it++;
        while(it != innerpts.end())
        {
            dist = DLINASG(pt, ((MHeightPoint*)(*it))->GetPoint(0));
            if(dist<mindist){
                pHpt = (MHeightPoint*)(*it);
                mindist = dist;
            }
            it++;
        }
        //we will restore a perpendicular from a point on the segment connecting the segment basis
        //restored from a point to the next horizontal, and height mark
        perppt = GetPointOnOtrezok(izopt1, pHpt->GetPoint(0), pt);
        //we will determine heights of points of a segment
        pIzo1->GetizoHeight(&str);
        izoheight1 = atof(str);
        izoheight2 = pHpt->GetHeight();
        //we will determine perpendicular basis height, height of an entrance point coincides with it
        height = HGTPT(izopt1, pHpt->GetPoint(0), perppt, izoheight1, izoheight2);
        return height;
    }

    //if the point lies between a horizontal and border of area of a relief, and wasn't
    //any horizontal meeting a condition, we will find height mark, next to a point,
    //lying on the same party from a horizontal, as an entrance point
    it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        hpt = ((MHeightPoint*)(*it))->GetPoint(0);
        if(OTS(*pLomline->GetAt(minizoind), hpt, *pLomline->GetAt(minizoind+1)) ==
                OTS(*pLomline->GetAt(minizoind), pt, *pLomline->GetAt(minizoind+1)))
            innerpts.push_back(*it);
        it++;
    }
    //if in area there was no mark of height
    if(innerpts.size()==0){
        pIzo1->GetizoHeight(&str);
        height = atof(str);
        return height;
    }
    it = innerpts.begin();
    pHpt = (MHeightPoint*)(*it);
    mindist = DLINASG(pt, pHpt->GetPoint(0));
    it++;
    while(it != innerpts.end())
    {
        dist = DLINASG(pt, ((MHeightPoint*)(*it))->GetPoint(0));
        if(dist<mindist){
            pHpt = (MHeightPoint*)(*it);
            mindist = dist;
        }
        it++;
    }
    //we will restore a perpendicular from a point on the segment connecting the segment basis
    //restored from a point to the next horizontal, and height mark
    perppt = GetPointOnOtrezok(izopt1, pHpt->GetPoint(0), pt);
    //we will determine heights of points of a segment
    pIzo1->GetizoHeight(&str);
    izoheight1 = atof(str);
    izoheight2 = pHpt->GetHeight();
    //we will determine perpendicular basis height, height of an entrance point coincides with it
    height = HGTPT(izopt1, pHpt->GetPoint(0), perppt, izoheight1, izoheight2);
    return height;

    return -100000;
}



//==================================
//! Obtaining information for creation of a profile in the area of a section
/**GENERAL IDEA: we find points of intersection of the line of a section presented in the broken line form with triangulable segments;
//               we go consistently on broken line links;
//               dimension of the output array is equal to the sum of tops of a broken line and a set of points of intersection of a broken line with triangulable segments;
//               then we find shift of each of points of the output array concerning the beginning of a broken line and height, linearly interpolating
//               from triangulable segments:
//               finally output array contains shifts from the beginning of the line of a section and height
//'pLomLine' - the pointer on the array of coordinates of points of the line of a section presented in the broken line form
//output information - the pointer on the array of shifts from the beginning of the line of a profile and heights of reference points of a profile:
//generally dimension of the output array is more than dimension of the entrance array
*/
MPtArray* MReliefLayer::GetProfileInfo(MPtArray* pLomLine)
//==================================
{
    if(m_triangles->GetSize()==0) return NULL;
    //We form the empty array of points for storage of information necessary for creation of a profile of a section
    MPtArray* array =new MPtArray();
    bool crossptl, crosspts, par;
    //	int i;
    MPoint pt;
    MPoint profpt, lompt1, lompt2, pt1, pt2;
    int numpts = pLomLine->GetSize(); //'numpts' - number of tops in a broken line
    double dist = 0; //'dist' - the sum of lengths of links of a broken line to the considered link
    double h1, h2;

    int i = 0;
    for(i=0; i<m_triangles->GetSize(); i++){
        MTriangle* pTrg = m_triangles->GetAt(i);
        MPoint trg[3];
        trg[0] = pTrg->m_tript1;
        trg[1] = pTrg->m_tript2;
        trg[2] = pTrg->m_tript3;
        if((pLomLine->GetAt(0)->InnerPt(trg, 3) == true)){
            profpt.x = 0; //shift from the beginning of the line of a profile of the initial point of a broken line the zero
            profpt.y = GetValueFromRelief(pLomLine->GetAt(0)); //linear interpolation we receive broken line top height from a triangulable triangle
            //we add a point to the output array of points of a profile
            array->AddTail(&profpt);
            break;
        }
    }

    //for each link of a broken line we find a point of intersection with a triangulable segment;
    //for the received point, and also the ends of a link of a broken line, we find height and shift from the beginning of a broken line;
    //consistently for each point we bring the received information in the output 'profar' array
    for(i=0; i<numpts-1; i++){
        // 'lompt1' and 'lompt2' - ends of a link of a broken line
        lompt1 = pLomLine->GetAt(i);
        lompt2 = pLomLine->GetAt(i+1);

        for(int j=0; j<m_segments->GetSize(); j++){
            MSegment* pSeg = m_segments->GetAt(j);
            pt1 = pSeg->m_sgpt1;
            pt2 = pSeg->m_sgpt2;

            // we look for a point of intersection of an edge of a triangle and a link of a broken line
            par = CROSSLINES(lompt1, lompt2, pt1, pt2, &pt);
            h1 = pSeg->m_sgheight1;
            h2 = pSeg->m_sgheight2;
            // further we keep the analysis, only if straight lines aren't parallel
            if(par){
                // we check, whether the point of intersection belongs to a link of a broken line and a triangulable edge
                crossptl = PTONSEG(&pt, lompt1, lompt2);
                crosspts = PTONSEG(&pt, pt1, pt2);
                if((crossptl) && (crosspts)){
                    profpt.x = DLINASG(&pt, lompt1) + dist; //shift of a point from the beginning of the line of a profile
                    profpt.y = HGTPT(pt1, pt2, &pt, h1, h2);// point height
                    //we add a point to the output array of points of a profile
                    array->AddTail(&profpt);
                }
            }
        }//the end of a cycle on all segments
        dist += DLINASG(lompt1, lompt2);
        //we receive information on the points of a profile corresponding to broken line tops
        for(int l=0; l<m_triangles->GetSize(); l++){
            MTriangle* pTrg = m_triangles->GetAt(l);

            MPoint trg[3];
            trg[0] = pTrg->m_tript1;
            trg[1] = pTrg->m_tript2;
            trg[2] = pTrg->m_tript3;
            if((pLomLine->GetAt(i+1)->InnerPt(trg, 3) == true)){
                profpt.x = dist; //shift from the beginning of the line of a profile of top of a link of a broken line is equal to the sum of lengths of the previous links
                profpt.y = GetValueFromRelief(pLomLine->GetAt(i+1)); //linear interpolation we receive broken line top height from a triangulable triangle
                //we add a point to the output array of points of a profile
                array->AddTail(&profpt);
                break;
            }
        }
    }// the end of a cycle on broken line links

    // we will order all elements of the output array on shift from the beginning of the line of a profile
    MPtArray* profar =new MPtArray();
    double mindist;
    int index;
    while(array->GetSize() != 1){
        mindist = array->GetAt(0)[0].x;
        index = 0;
        for(int i=1; i<array->GetSize(); i++){
            if(array->GetAt(i)[0].x < mindist){
                mindist = array->GetAt(i)[0].x;
                index = i;
            }
        }
        profar->AddTail(array->GetAt(index));
        array->Delete(index);
    }
    profar->AddTail(array->GetAt(0));
    array->Delete(0);

    // we release more unnecessary memory
    array->ClearAll();

    return profar;
}

//==================================
//! Obtaining information for creation of a profile in the area of a section on all layers of a relief
/**GENERAL IDEA: we find points of intersection of the line of a section presented in the broken line form
//               with horizontals, finally output array contains shifts from the beginning of the line
//               section and height;
//'pLomLine' - the pointer on the array of coordinates of points of the line of a section presented in the broken line form
//output information - the pointer on the array of shifts from the beginning of the line of a profile and heights of reference points of a profile:
//generally dimension of the output array doesn't coincide with dimension of the entrance array
*/
MPtArray* MReliefLayer::GetProfileInfoByIzolines(MPtArray* pLomLine, double dopusk)
//==================================
{
    //We form the empty array of points for storage of information necessary for creation of a profile of a section
    MPtArray* array =new MPtArray();
    MPoint pt;
    OBITER it;

    if(m_izolines.size()!=0){
        //		int numpts = pLomLine->GetSize(); //'numpts' - number of tops in a broken line
        double dist; //'dist' - the sum of lengths of links of a broken line to the considered link
        MPtArray* ptar;
        it = m_izolines.begin();
        while(it != m_izolines.end())
        {
            MString strheight;
            double izoheight;
            ((MIzoline*)(*it))->GetizoHeight(&strheight);
            izoheight = atof(strheight);
            dist = 0;
            for(int i=0; i<pLomLine->GetSize()-1; i++){
                ptar = ((MIzoline*)(*it))->FindIntersectsWithSegment(pLomLine->GetAt(i)[0], pLomLine->GetAt(i+1)[0]);
                if(ptar!=NULL){
                    for(int j=0; j<ptar->GetSize(); j++){
                        pt.x = dist + DLINASG(pLomLine->GetAt(i)[0], ptar->GetAt(j)[0]);
                        pt.y = izoheight;
                        array->AddTail(&pt);
                    }
                }
                dist += DLINASG(pLomLine->GetAt(i)[0], pLomLine->GetAt(i+1)[0]);
            }
            it++;
        }
    }
    //if horizontals aren't present, but there are marks of heights, build a profile on marks of heights,
    //being from the gas pipeline at distance, smaller the admission
    else if(m_heightpoints.size()!=0){
        MPoint resultpt;
        int nomrebra;
        it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            pt = ((MHeightPoint*)(*it))->GetPoint(0);
            if(FindDistFromPointToLomline(pt, pLomLine, resultpt,&nomrebra)<dopusk){
                FindDistFromBeginningToLompt(resultpt, pLomLine, pt.x);
                pt.y = ((MHeightPoint*)(*it))->GetHeight();
                array->AddTail(&pt);
            }
            it++;
        }
    }

    if(array->GetSize()<=1)
        return array;

    // we will order all elements of the output array on shift from the beginning of the line of a profile
    MPtArray* profar =new MPtArray();
    double mindist;
    int index;
    while(array->GetSize() != 1){
        mindist = array->GetAt(0)[0].x;
        index = 0;
        for(int i=1; i<array->GetSize(); i++){
            if(array->GetAt(i)[0].x < mindist){
                mindist = array->GetAt(i)[0].x;
                index = i;
            }
        }
        profar->AddTail(array->GetAt(index));
        array->Delete(index);
    }
    profar->AddTail(array->GetAt(0));
    array->Delete(0);

    // we release more unnecessary memory
    delete array;

    return profar;
}


//============================================
//!function finds all crossings of the gas pipeline with hydroobjects and returns information in
/**two arrays: 'Gidrocoord', point which elements: 'x' - coordinate is a shift of a point
//crossings at a gas pipeline entrance to hydroobject from the beginning of the gas pipeline, 'y' - coordinate -
//this shift of a point of intersection at a gas pipeline exit from hydroobject from the beginning of the gas pipeline;
//'Gidroheight' which elements correspond to elements of the Gidrocoord array and are heights
//points of intersection: 'pt.x' - height of a point of intersection on an entrance, 'pt.y' - height of a point of intersection
//at the exit;
//both arrays are ordered on shift of a point of intersection of an entrance of the gas pipeline to hydroobject;
*/
bool MReliefLayer::FindCrosslinesWithGidro(MPtArray* plankontur, MPtArray* Gidrocoord, MPtArray* Gidroheight)
//============================================
{
    MPtArray* planGidrocoord = new MPtArray();//array of points of intersection of the gas pipeline
    //with hydroobject on the plan
    MPtArray* crossseg = new MPtArray();//array of points of intersection of the gas pipeline
    //with hydroobject on the plan
    //we get the 'cooarray' and 'heightarray' arrays temporarily instead of the output 'Gidrocoord'
    //and 'Gidroheight' arrays
    MPtArray* cooarray =new MPtArray();
    MPtArray* heightarray =new MPtArray();

    Gidrocoord->ClearAll();
    Gidroheight->ClearAll();

    bool parsg;
    //we will receive a gas pipeline profile
    MPtArray* profar;
    profar = GetProfileInfoByIzolines(plankontur);

    OBITER it = m_gidroobjects.begin();

    while(it != m_gidroobjects.end())
    {
        if((*it)->WhatTheObject()==ID_GIDRO){
            MGidro* pGidro = (MGidro*)(*it);
            MPtArray* gidrokontur = (MPtArray*)pGidro->m_Komplekt->GetAt(pGidro->F_TOCHKI);
            crossseg->ClearAll();
            planGidrocoord->ClearAll();
            //'parseg' - the parameter specifying whether coincided a last contour point of intersection
            //hydroobject and an edge of a contour of the gas pipeline from the end of an edge of the gas pipeline;
            //this parameter is necessary twice not to include the same point of intersection
            parsg = false;

            //for each edge of a contour of the gas pipeline we receive edge points of intersection with a contour
            //hydroobject, excepting the duplicating points in cases, when the gas pipeline and hydroobject
            //are crossed in points of intersection of edges of a contour of the gas pipeline;
            //we assume that total number of points of intersection the gas pipeline and hydroobject -
            //even, i.e. it is couples of points: an entrance to hydroobject and an exit from hydroobject
            int i = 0;
            for(i=0; i<plankontur->GetSize()-1; i++){
                if(FindCrossSegWithKontur(plankontur->GetAt(i), plankontur->GetAt(i+1), gidrokontur, crossseg, NULL)){
                    if(parsg)
                        crossseg->DeleteHead();
                    planGidrocoord->AppendTail(crossseg);
                    if(*crossseg->GetAt(crossseg->GetSize()-1)==*plankontur->GetAt(i+1))
                        parsg = true;
                    else
                        parsg = false;
                }
            }

            double h1, h2, rasst1, rasst2;
            int index1=0, index2=0;
            MPoint pt, profpt, gidropt, heightpt;
            //now it is necessary to coordinate the 'planGidrocoord' and 'Gidroheight' arrays so that
            //to couples of points an entrance exit to hydroobject there corresponded heights of points of intersection
            //on an entrance-exit
            for(i=0; i<planGidrocoord->GetSize()-1; i+=2){
                //we will find shift of a point of intersection of the gas pipeline and hydroobject
                //from the beginning of the gas pipeline for an entry point
                pt = *planGidrocoord->GetAt(i);
                FindDistFromBeginningToLompt(pt, plankontur, rasst1);
                //now we will find an index of an edge of a broken line of a profile on which lies
                //point of intersection;
                //in the array of points of the gas pipeline on a profile of a point are ordered on shift
                //from the beginning
                int j = 0;
                for(j=0; j<profar->GetSize(); j++){
                    profpt = *profar->GetAt(j);
                    if(profpt.x>=rasst1){
                        index1 = j - 1;
                        break;
                    }
                }
                //the equation of the straight line passing through a segment of 'p1', 'p2'.
                //(x2-x1)*y + (y1-y2)*x + x1*(y2-y1) - y1*(x2-x1) = 0
                //from the equation of the straight line passing through a link with the gas pipeline broken line 'index1' index
                //on a profile, and values of shift of a point of intersection we will receive value
                //heights of a point of intersection on a profile:
                h1 = profar->GetAt(index1)->y + ((rasst1 - profar->GetAt(index1)->x)*
                                                 (profar->GetAt(index1+1)->y - profar->GetAt(index1)->y))/
                        (profar->GetAt(index1+1)->x - profar->GetAt(index1)->x);
                //we will find shift of a point of intersection of the gas pipeline and hydroobject
                //from the beginning of the gas pipeline for an exit point
                pt = *planGidrocoord->GetAt(i+1);
                FindDistFromBeginningToLompt(pt, plankontur, rasst2);
                //now we will find an index of an edge of a broken line of a profile on which lies
                //point of intersection;
                //in the array of points of the gas pipeline on a profile of a point are ordered on shift
                //from the beginning
                for(j=0; j<profar->GetSize(); j++){
                    profpt = *profar->GetAt(j);
                    if(profpt.x>=rasst2){
                        index2 = j - 1;
                        break;
                    }
                }
                //the equation of the straight line passing through a segment 'p1', 'p2'.
                //(x2-x1)*y + (y1-y2)*x + x1*(y2-y1) - y1*(x2-x1) = 0
                //from the equation of the straight line passing through a link with the gas pipeline broken line index2 index
                //on a profile, and values of shift of a point of intersection we will receive value
                //heights of a point of intersection on a profile:
                h2 = profar->GetAt(index2)->y + ((rasst2 - profar->GetAt(index2)->x)*
                                                 (profar->GetAt(index2+1)->y - profar->GetAt(index2)->y))/
                        (profar->GetAt(index2+1)->x - profar->GetAt(index2)->x);
                gidropt.x = rasst1;
                gidropt.y = rasst2;
                heightpt.x = h1;
                heightpt.y = h2;
                //we include the received point in the array of shifts of points of intersection of an entrance exit
                //on a profile
                cooarray->AddTail(&gidropt);
                heightarray->AddTail(&heightpt);
            }//end for(int i=0; i<Gidroheight->GetSize()-1; i+=2)
        }//end if(pObj->WhatTheObject()==ID_GIDRO)
        it++;
    }//end while(pos!=NULL)
    delete crossseg;
    delete profar;
    delete planGidrocoord;

    if(cooarray->GetSize() == 0) return false;
    //we will order all elements of output arrays on shift from the beginning of the line of a profile
    double mindist;
    int index;
    while(cooarray->GetSize() != 1){
        mindist = cooarray->GetAt(0)[0].x;
        index = 0;
        for(int i=1; i<cooarray->GetSize(); i++){
            if(cooarray->GetAt(i)[0].x < mindist){
                mindist = cooarray->GetAt(i)[0].x;
                index = i;
            }
        }
        Gidrocoord->AddTail(cooarray->GetAt(index));
        Gidroheight->AddTail(heightarray->GetAt(index));
        cooarray->Delete(index);
        heightarray->Delete(index);
    }
    Gidrocoord->AddTail(cooarray->GetAt(0));
    Gidroheight->AddTail(heightarray->GetAt(0));
    cooarray->Delete(0);
    heightarray->Delete(0);

    // we release more unnecessary memory
    delete cooarray;
    delete heightarray;

    if(Gidroheight->GetSize()==0)
        return false;

    return true;
}

//============================================
//!Unites all objects in 'm_oblist'
MObjectList* MReliefLayer::CreateObjectList()
//============================================
{
    m_oblist.clear();
    m_oblist.insert(m_oblist.end(), m_heightpoints.begin(), m_heightpoints.end());
    m_oblist.insert(m_oblist.end(), m_izolines.begin(), m_izolines.end());
    m_oblist.insert(m_oblist.end(), m_dopizolines.begin(), m_dopizolines.end());
    m_oblist.insert(m_oblist.end(), m_kontlist.begin(), m_kontlist.end());
    m_oblist.insert(m_oblist.end(), m_macroforms.begin(), m_macroforms.end());
    m_oblist.insert(m_oblist.end(), m_gidroobjects.begin(), m_gidroobjects.end());
    m_oblist.insert(m_oblist.end(), m_depthpoints.begin(), m_depthpoints.end());
    m_oblist.insert(m_oblist.end(), m_izobats.begin(), m_izobats.end());
    m_oblist.insert(m_oblist.end(), m_decorative.begin(), m_decorative.end());
    return &m_oblist;
}

//===============================================
//!Connection of objects of a layer to the classifier
void MReliefLayer::ConnectKlassifikator(MKlassifikator* pKls)
//===============================================
{
    //We form the general list and we cause parental function
    CreateObjectList();
    MLayer::ConnectKlassifikator(pKls);
}
//===================================================================================
//!Adds to the given 'oblist' objects with set the quad-code
void MReliefLayer::AddObjectsInObjectListByQuadrakod(MObjectList* oblist,MString quadrakod)
//===================================================================================
{
    //We form the general list and we cause parental function
    CreateObjectList();
    MLayer::AddObjectsInObjectListByQuadrakod(oblist,quadrakod);
}

//==========================================
//!We look for, whether there is in a layer such object
bool MReliefLayer::IsObject(MObject* pObj)
//==========================================
{
    OBITER it = std::find(m_heightpoints.begin(), m_heightpoints.end(), pObj);
    if(it != m_heightpoints.end()) return true;

    it = std::find(m_izolines.begin(), m_izolines.end(), pObj);
    if(it != m_izolines.end()) return true;

    it = std::find(m_dopizolines.begin(), m_dopizolines.end(), pObj);
    if(it != m_dopizolines.end()) return true;

    it = std::find(m_kontlist.begin(), m_kontlist.end(), pObj);
    if(it != m_kontlist.end()) return true;

    it = std::find(m_macroforms.begin(), m_macroforms.end(), pObj);
    if(it != m_macroforms.end()) return true;

    it = std::find(m_gidroobjects.begin(), m_gidroobjects.end(), pObj);
    if(it != m_gidroobjects.end()) return true;

    it = std::find(m_depthpoints.begin(), m_depthpoints.end(), pObj);
    if(it != m_depthpoints.end()) return true;

    it = std::find(m_izobats.begin(), m_izobats.end(), pObj);
    if(it != m_izobats.end()) return true;

    it = std::find(m_decorative.begin(), m_decorative.end(), pObj);
    if(it != m_decorative.end()) return true;

    else return false;
}

//======================================================
//!Replaces in the list of objects all pointers 'pOldObj' with 'pNewObj'
void MReliefLayer::ReplaceObject(MObject* pOldObj, MObject* pNewObj)
//======================================================
{
    std::replace(m_heightpoints.begin(), m_heightpoints.end(), pOldObj, pNewObj);
    std::replace(m_izolines.begin(), m_izolines.end(), pOldObj, pNewObj);
    std::replace(m_dopizolines.begin(), m_dopizolines.end(), pOldObj, pNewObj);
    std::replace(m_kontlist.begin(), m_kontlist.end(), pOldObj, pNewObj);
    std::replace(m_macroforms.begin(), m_macroforms.end(), pOldObj, pNewObj);
    std::replace(m_gidroobjects.begin(), m_gidroobjects.end(), pOldObj, pNewObj);
    std::replace(m_decorative.begin(), m_decorative.end(), pOldObj, pNewObj);
    std::replace(m_depthpoints.begin(), m_depthpoints.end(), pOldObj, pNewObj);
    std::replace(m_izobats.begin(), m_izobats.end(), pOldObj, pNewObj);
}

//====================================================
//!Forms the list of the objects belonging to a relief layer
//! The result is located in the given 'oblist'
void MReliefLayer::GetObjectList(MObjectList* oblist)
//====================================================
{
    if(oblist)
    {
        oblist->clear();
        oblist->insert(oblist->end(), m_heightpoints.begin(), m_heightpoints.end());
        oblist->insert(oblist->end(), m_izolines.begin(), m_izolines.end());
        oblist->insert(oblist->end(), m_dopizolines.begin(), m_dopizolines.end());
        oblist->insert(oblist->end(), m_kontlist.begin(), m_kontlist.end());
        oblist->insert(oblist->end(), m_macroforms.begin(), m_macroforms.end());
        oblist->insert(oblist->end(), m_gidroobjects.begin(), m_gidroobjects.end());
        oblist->insert(oblist->end(), m_depthpoints.begin(), m_depthpoints.end());
        oblist->insert(oblist->end(), m_izobats.begin(), m_izobats.end());
        oblist->insert(oblist->end(), m_decorative.begin(), m_decorative.end());
    }
}

MIzoline* MReliefLayer::FindNearestIzoline(MPoint findpt)
//====================================================
{
    //we will receive the list of isolines of a layer
    MObjectList oblist;
    oblist.insert(oblist.end(), m_izolines.begin(), m_izolines.end());


    OBITER it = oblist.begin();
    MObject* pObj;
    MObject* pFindObj = NULL;
    double distance= -1.0;
    // cycle prolog
    while(it != oblist.end())
    {
        pObj = *it;
        distance = pObj->FindDistanceFromPoint(findpt);
        if(distance>=0){
            pFindObj = pObj;
            break;
        }
        it++;
    }

    //cycle on objects
    while(it != oblist.end())
    {
        pObj = *it;
        double newdistance = pObj->FindDistanceFromPoint(findpt);
        if((newdistance>=0)&&(newdistance<distance))
        {
            distance = newdistance;
            pFindObj = pObj;
        }
        it ++;
    }

    return (MIzoline*)pFindObj;
}


//===================================================================
//!transformation to other system of coordinates
void MReliefLayer::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo)
//===================================================================
{
    if((pCurrentCoo==NULL)||(pNewCoo==NULL))
        return;

    //we will be run on all objects and we will cause in them the corresponding function of transformation
    MObjectList utlist;
    GetObjectList(&utlist);
    OBITER it = utlist.begin();
    while(it != utlist.end())
    {
        (*it)->ConvertToCoo(pCurrentCoo, pNewCoo);
        it++;
    }

    // array of triangulable segments
    double h=0;
    MSegment* pSeg;
    int i = 0;
    for(i=m_segments->GetSize()-1;i>-1;i--)
    {
        pSeg = m_segments->GetAt(i);
        pCurrentCoo->ConvertToCoo(pNewCoo, pSeg->m_sgpt1, h);
        pCurrentCoo->ConvertToCoo(pNewCoo, pSeg->m_sgpt2, h);
    }
    // array of triangulable triangles
    MTriangle* pTrian;
    for(i=m_triangles->GetSize()-1;i>-1;i--)
    {
        pTrian = m_triangles->GetAt(i);
        pCurrentCoo->ConvertToCoo(pNewCoo, pTrian->m_tript1, h);
        pCurrentCoo->ConvertToCoo(pNewCoo, pTrian->m_tript2, h);
        pCurrentCoo->ConvertToCoo(pNewCoo, pTrian->m_tript3, h);
    }
    // array of structural lines
    MPtArray* pta;
    int j;
    MPoint* ppt;
    for(i=m_structlines->GetSize()-1;i>-1;i--)
    {
        pta = (MPtArray*)m_structlines->GetAt(i);
        for(j=pta->GetSize()-1; j>-1; j--)
        {
            ppt = pta->GetAt(j);
            pCurrentCoo->ConvertToCoo(pNewCoo, ppt[0], h);
        }
    }

    //contour of coverage of a layer of a relief
    for(j=m_RelKonturPts->GetSize()-1; j>-1; j--)
    {
        ppt = m_RelKonturPts->GetAt(j);
        pCurrentCoo->ConvertToCoo(pNewCoo, ppt[0], h);
    }

    MinimizeBounds();
}

//=================================================================
//!In a layer 'pLay' copies the members.
//! In the list of objects only pointers, but not objects are copied
void MReliefLayer::CreateCopyForUndo(MLayer* pLay)
//=================================================================
{
    //We will check for coincidence of types and existence 'pLay'
    if(!pLay)return;
    if(pLay->WhatTheLayer()!=ID_RELIEF_LAYER)return;
    //We copy the member of 'm_dbarray' of the class 'MLayer'
    pLay->m_dbarray.ClearAll();
    pLay->m_dbarray = *(MStArray *)m_dbarray.Clone();

    //We copy the member of 'm_name' of the class 'MLayer'
    pLay->m_name=m_name;

    //We copy the member of 'm_sort' of the class 'MLayer'
    pLay->m_sort=m_sort;

    //We copy the member of 'm_remark' of the class 'MLayer'
    pLay->m_remark=m_remark;

    //We copy the member of 'm_password' of the class 'MLayer'
    pLay->m_password=m_password;

    //We copy the member of 'm_bounds' of the class 'MLayer'
    pLay->m_bounds=m_bounds;

    //We copy the member of 'x_common' of the class 'MLayer'
    pLay->x_common=x_common;

    //We copy the member of 'x_visible' of the class 'MLayer'
    pLay->x_visible=x_visible;

    //We copy the member of 'x_textvisible' of the class 'MLayer'
    pLay->x_textvisible=x_textvisible;

    //We copy indexes on the main types of objects
    ((MReliefLayer*)pLay)->m_pKlsHeightpt = m_pKlsHeightpt;
    ((MReliefLayer*)pLay)->m_pKlsDepthpt = m_pKlsDepthpt;
    ((MReliefLayer*)pLay)->m_pKlsIzoline = m_pKlsIzoline;
    ((MReliefLayer*)pLay)->m_pKlsIzobata = m_pKlsIzobata;
    ((MReliefLayer*)pLay)->m_pKlsDopizo = m_pKlsDopizo;
    ((MReliefLayer*)pLay)->m_pKlsUtlizo = m_pKlsUtlizo;
    ((MReliefLayer*)pLay)->m_pKlsObryv = m_pKlsObryv;
    ((MReliefLayer*)pLay)->m_pKlsOtkos = m_pKlsOtkos;
    ((MReliefLayer*)pLay)->m_pKlsPodpStenka = m_pKlsPodpStenka;
    ((MReliefLayer*)pLay)->m_pKlsGidro = m_pKlsGidro;
    ((MReliefLayer*)pLay)->m_pKlsYama = m_pKlsYama;

    //We copy quad-code of the main types of objects
    ((MReliefLayer*)pLay)->m_qkodHeightpt = m_qkodHeightpt;
    ((MReliefLayer*)pLay)->m_qkodDepthpt = m_qkodDepthpt;
    ((MReliefLayer*)pLay)->m_qkodIzoline = m_qkodIzoline;
    ((MReliefLayer*)pLay)->m_qkodIzobata = m_qkodIzobata;
    ((MReliefLayer*)pLay)->m_qkodDopizo = m_qkodDopizo;
    ((MReliefLayer*)pLay)->m_qkodUtlizo = m_qkodUtlizo;
    ((MReliefLayer*)pLay)->m_qkodObryv = m_qkodObryv;
    ((MReliefLayer*)pLay)->m_qkodOtkos = m_qkodOtkos;
    ((MReliefLayer*)pLay)->m_qkodPodpStenka = m_qkodPodpStenka;
    ((MReliefLayer*)pLay)->m_qkodGidro = m_qkodGidro;
    ((MReliefLayer*)pLay)->m_qkodYama = m_qkodYama;

    //We copy the list of triangles
    ((MReliefLayer*)pLay)->m_triangles->ClearAll();
    ((MReliefLayer*)pLay)->m_triangles->AppendTail(m_triangles);

    //We copy the list of segments
    ((MReliefLayer*)pLay)->m_segments->ClearAll();
    ((MReliefLayer*)pLay)->m_segments->AppendTail(m_segments);

    //We copy the list of structural lines
    ((MReliefLayer*)pLay)->m_structlines->ClearAll();
    MArArray* newstructlines = new MArArray();
    MPtArray* ptar;
    for(int i=0; i<m_structlines->GetSize(); i++){
        ptar = new MPtArray();
        ptar->AppendTail((MPtArray*)m_structlines->GetAt(i));
        newstructlines->AddTail((MArray*)ptar);
    }
    ((MReliefLayer*)pLay)->m_structlines->AppendTail(newstructlines);

    //We copy a coverage contour
    ((MReliefLayer*)pLay)->m_RelKonturPts->ClearAll();
    ((MReliefLayer*)pLay)->m_RelKonturPts->AppendTail(m_RelKonturPts);

    //We copy the array of heights for a coverage contour
    ((MReliefLayer*)pLay)->m_RelKonturHgt->ClearAll();
    ((MReliefLayer*)pLay)->m_RelKonturHgt->AppendTail(m_RelKonturHgt);

    //We copy the member of 'm_doppts' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_doppts = m_doppts;
    //We copy the member of 'm_shag' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_shag = m_shag;
    //We copy the member of 'm_izotype' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_izotype = m_izotype;
    //We copy the member of 'm_viewtrg' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_viewtrg = m_viewtrg;
    //We copy the member of 'm_viewizolines' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_viewizolines = m_viewizolines;
    //We copy the member of 'm_viewstructlines' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_viewstructlines = m_viewstructlines;
    //We copy the member of 'm_viewRelKontur' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_viewRelKontur = m_viewRelKontur;
    //We copy the member of 'm_viewhptnom' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_viewhptnom = m_viewhptnom;
    //We copy the member of 'm_viewhptvalue' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_viewhptvalue = m_viewhptvalue;
    //We copy the member of 'm_minheight' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_minheight = m_minheight;
    //We copy the member of 'm_maxheight' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_maxheight = m_maxheight;

    //We copy the member of 'm_pDoc' of the class 'MLayer'
    pLay->m_pKarta=m_pKarta;
    //We copy the member of 'm_heightpoints' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_heightpoints.clear();
    ((MReliefLayer*)pLay)->m_heightpoints.insert(((MReliefLayer*)pLay)->m_heightpoints.end(),
                                                 m_heightpoints.begin(), m_heightpoints.end());
    //We copy the member of 'm_izolines' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_izolines.clear();
    ((MReliefLayer*)pLay)->m_izolines.insert(((MReliefLayer*)pLay)->m_izolines.end(),
                                             m_izolines.begin(), m_izolines.end());
    //We copy the member of 'm_dopizolines' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_dopizolines.clear();
    ((MReliefLayer*)pLay)->m_dopizolines.insert(((MReliefLayer*)pLay)->m_dopizolines.end(),
                                                m_dopizolines.begin(), m_dopizolines.end());
    //We copy the member of 'm_kontlist' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_kontlist.clear();
    ((MReliefLayer*)pLay)->m_kontlist.insert(((MReliefLayer*)pLay)->m_kontlist.end(),
                                             m_kontlist.begin(), m_kontlist.end());

    //We copy the member of 'm_macroforms' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_macroforms.clear();
    ((MReliefLayer*)pLay)->m_macroforms.insert(((MReliefLayer*)pLay)->m_macroforms.end(),
                                               m_macroforms.begin(), m_macroforms.end());
    //We copy the member of 'm_gidroobjects' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_gidroobjects.clear();
    ((MReliefLayer*)pLay)->m_gidroobjects.insert(((MReliefLayer*)pLay)->m_gidroobjects.end(),
                                                 m_gidroobjects.begin(), m_gidroobjects.end());
    //We copy the member of 'm_depthpoints' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_depthpoints.clear();
    ((MReliefLayer*)pLay)->m_depthpoints.insert(((MReliefLayer*)pLay)->m_depthpoints.end(),
                                                m_depthpoints.begin(), m_depthpoints.end());
    //We copy the member of 'm_izobats' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_izobats.clear();
    ((MReliefLayer*)pLay)->m_izobats.insert(((MReliefLayer*)pLay)->m_izobats.end(),
                                            m_izobats.begin(), m_izobats.end());
    //We copy the member of 'm_decorative' of the class 'MReliefLayer'
    ((MReliefLayer*)pLay)->m_decorative.clear();
    ((MReliefLayer*)pLay)->m_decorative.insert(((MReliefLayer*)pLay)->m_decorative.end(),
                                               m_decorative.begin(), m_decorative.end());
}

//====================================================
//!Clears the list of objects, but thus doesn't delete objects
void MReliefLayer::ClearObjectList()
//====================================================
{
    m_oblist.clear();
    m_heightpoints.clear();
    m_izolines.clear();
    m_dopizolines.clear();
    m_kontlist.clear();
    m_macroforms.clear();
    m_gidroobjects.clear();
    m_depthpoints.clear();
    m_izobats.clear();
    m_decorative.clear();
}


//=================================
//!Deletes all objects of a layer
void MReliefLayer::DeleteObjects()
//=================================
{
    GetObjectList(&m_oblist);
    MLayer::DeleteObjects();
    ClearObjectList();

    m_segments->ClearAll();
    m_triangles->ClearAll();
    m_structlines->ClearAll();
    m_RelKonturPts->ClearAll();
    m_RelKonturHgt->ClearAll();
}

//==================================
//! addition of object
void MReliefLayer::AddObject(MObject* pObj)
//==================================
{
    MString str = pObj->WhatTheObject();
    if(str == ID_IZOLINE)
        m_izolines.push_back(pObj);
    if((str == ID_OBRYV)||(str == ID_OTKOS)||(str == ID_PODPSTENKA)||(str == ID_YAMA))
        m_macroforms.push_back(pObj);
    if(str == ID_GIDRO)
        m_gidroobjects.push_back(pObj);
    if(str == ID_TOPOBJECT)
        m_decorative.push_back(pObj);
    pObj->m_pLay = this;
}

//==================================
//!delete object
void MReliefLayer::RemoveObject(MObject* pObj)
//==================================
{
    // we will find and will clean from object
    OBITER it = std::find(m_heightpoints.begin(), m_heightpoints.end(), pObj);
    if(it!=m_heightpoints.end()) m_heightpoints.erase(it);
    it = std::find(m_izolines.begin(), m_izolines.end(), pObj);
    if(it!=m_izolines.end()) m_izolines.erase(it);
    it = std::find(m_dopizolines.begin(), m_dopizolines.end(), pObj);
    if(it!=m_dopizolines.end()) m_dopizolines.erase(it);
    it = std::find(m_kontlist.begin(), m_kontlist.end(), pObj);
    if(it!=m_kontlist.end()) m_kontlist.erase(it);
    it = std::find(m_macroforms.begin(), m_macroforms.end(), pObj);
    if(it!=m_macroforms.end()) m_macroforms.erase(it);
    it = std::find(m_gidroobjects.begin(), m_gidroobjects.end(), pObj);
    if(it!=m_gidroobjects.end()) m_gidroobjects.erase(it);
    it = std::find(m_gidroobjects.begin(), m_gidroobjects.end(), pObj);
    if(it!=m_gidroobjects.end()) m_gidroobjects.erase(it);
    it = std::find(m_decorative.begin(), m_decorative.end(), pObj);
    if(it!=m_decorative.end()) m_decorative.erase(it);
    it = std::find(m_depthpoints.begin(), m_depthpoints.end(), pObj);
    if(it!=m_depthpoints.end()) m_depthpoints.erase(it);
    it = std::find(m_izobats.begin(), m_izobats.end(), pObj);
    if(it!=m_izobats.end()) m_izobats.erase(it);
}



//===============================================
//!according to the pointer on the classifier and a quad-code function returns the pointer on a classifier sign
MZnak*  MReliefLayer::GetKlsZnak(MString qkod)
//===============================================
{
    MString tempstr;
    MZnak* pZnak = GetKarta()->m_pKls->GetZnak(qkod,tempstr);
    return pZnak;
}

//===============================================
//!function returns distance from 'pt' point to the closest mark of height if it doesn't exceed the admission of 'dopusk'
bool  MReliefLayer::FindNearestRlfPoint(MPoint pt, double dopusk, double* rasst, MPoint* heightpt)
//===============================================
{
    if(m_heightpoints.size()==0) return false;

    rasst[0] = -1;
    MPoint hpt;
    double dist;

    //we look for the first mark of height the distance to which doesn't exceed the admission
    OBITER it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        hpt = ((MHeightPoint*)(*it))->GetPoint(0);
        dist = DLINASG(pt, hpt);
        if(dist<=dopusk){
            rasst[0] = dist;
            heightpt->x = hpt.x;
            heightpt->y = hpt.y;
            break;
        }
        it++;
    }
    //if the distance to all marks of heights exceeds the admission
    if(rasst[0]==-1) return false;

    //we look for height mark, distance to which minimum
    while(it != m_heightpoints.end())
    {
        hpt = ((MHeightPoint*)(*it))->GetPoint(0);
        dist = DLINASG(pt, hpt);
        if(dist<rasst[0]){
            rasst[0] = dist;
            heightpt->x = hpt.x;
            heightpt->y = hpt.y;
        }
        it++;
    }
    return true;
}

//===================================================================
//!Creation of a set of circumscribed circles for the set of triangles
bool MReliefLayer::CreateCircle(MTrArray* triar, MPtArray* centerar, MDuArray* radar)
//===================================================================
{
    if((!triar)||(triar->GetSize()==0))
        return false;

    MTriangle* pTri;
    MPoint center;
    double radius;

    for(int i=0; i<triar->GetSize(); i++){
        pTri = triar->GetAt(i);
        CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
        centerar->AddTail(&center);
        radar->AddTail(&radius);
    }
    return true;
}

//===================================================================
//!Orientation of points of a triangulable triangle clockwise;
bool MReliefLayer::OrientTriangleByClock(MTriangle* pTri)
//===================================================================
{
    //if any of points coincide, the focused area of a triangle is equal
    //to zero, it is a degenerate case
    if(((pTri->m_tript1==pTri->m_tript2)||(pTri->m_tript1==pTri->m_tript3))||
            (pTri->m_tript2==pTri->m_tript3))
        return false;

    MPoint pt;
    double h;
    if(OTS(pTri->m_tript1, pTri->m_tript2, pTri->m_tript3)>0){
        pt = pTri->m_tript2;
        h = pTri->m_triheight2;
        pTri->m_tript2 = pTri->m_tript3;
        pTri->m_tript3 = pt;
        pTri->m_triheight2 = pTri->m_triheight3;
        pTri->m_triheight3 = h;
    }
    return true;
}

//===================================================================
//!Allocation of area of modification - one-coherent area - for the set 'pHpt' height mark
//! on a set of triangulable triangles for inclusion of the set height mark
//! in a triangulation
bool MReliefLayer::FindModregionForTript(MHeightPoint* pHpt, MTrArray* triar, MSgArray* segar,
                                         MPtArray* centerar,MDuArray* radar, MPtArray* modRegion, MDuArray* heightRegion)
//===================================================================
{
    if(((!triar)||(!centerar))||(!radar))
        return false;
    int ntriangles = triar->GetSize();
    int ncircles = centerar->GetSize();
    int nradius = radar->GetSize();

    if((ntriangles!=ncircles)||(ntriangles!=nradius))
        return false;

    modRegion->ClearAll();
    heightRegion->ClearAll();

    //deltris - the array for the triangles deleted from a triangulable network
    MTrArray* deltris = new MTrArray();

    //we check, whether the set poin getst in spun for i-th triangle;
    //we form the array of the deleted triangles
    int i = 0;
    for(i=0; i<centerar->GetSize(); i++){
        if(IsPointInCircle(pHpt->GetPoint(0), *centerar->GetAt(i), *radar->GetAt(i))){
            deltris->AddTail(triar->GetAt(i));
        }
    }
    //NOW WE WILL WORK WITH THE array OF THE DELETED deltris TRIANGLES!
    //1) at first we believe, the area of modification coincides with a triangle, in which
    //there is a set point
    //we focus modification area clockwise
    //we delete a triangle in which there is a set point, from a triangulable network,
    //and a circumscribed circle from a set of the circumscribed circles corresponding
    //to triangulable triangles;
    MTriangle* pTri;
    int delsgind, deltrind;
    for(i=0; i<deltris->GetSize();i++){
        pTri = deltris->GetAt(i);
        if(IsPointInTriangle(pHpt->GetPoint(0), &pTri->m_tript1, &pTri->m_tript2, &pTri->m_tript3)){
            OrientTriangleByClock(pTri);
            modRegion->AddTail(&pTri->m_tript1);
            modRegion->AddTail(&pTri->m_tript2);
            modRegion->AddTail(&pTri->m_tript3);
            heightRegion->AddTail(&pTri->m_triheight1);
            heightRegion->AddTail(&pTri->m_triheight2);
            heightRegion->AddTail(&pTri->m_triheight3);
            deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
            triar->Delete(deltrind);
            centerar->Delete(deltrind);
            radar->Delete(deltrind);
            deltris->Delete(i);
            break;
        }
    }

    if(modRegion->GetSize()==0)
        return false;

    //in case of very difficult contours there can be triangles, a circumscribed circle
    //which contains a point, but the triangle isn't joined to modification area on
    //to an edge; we don't consider these triangles;
    //the count parameter is equal to unit if the next triangle is joined to area
    //modifications on an edge
    bool count = true;

    //2) from the list of other deleted triangles we look for such that one of his edges
    //coincides with an edge of the modified area;
    //we believe, the area of modification and all triangulable triangles are focused
    //clockwise, so the set of checks is narrowed;
    //we expand modification area, we won't include in it all deleted triangles yet
    //	while(deltris->GetSize()!=0){
    while((count)&&(deltris->GetSize()!=0)){
        for(int j=0; j<deltris->GetSize(); j++){
            pTri = deltris->GetAt(j);
            count = false;
            int i = 0;
            for(i=0; i<modRegion->GetSize()-1;i++){
                if((*modRegion->GetAt(i)==pTri->m_tript2)&&(*modRegion->GetAt(i+1)==pTri->m_tript1)){
                    modRegion->InsertAfter(i,&pTri->m_tript3);
                    heightRegion->InsertAfter(i,&pTri->m_triheight3);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
                    segar->Delete(delsgind);
                    //we delete a triangle from a triangulable network, and a circumscribed circle
                    //from a set of the circumscribed circles corresponding to the triangulable
                    //to triangles;
                    deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
                    triar->Delete(deltrind);
                    centerar->Delete(deltrind);
                    radar->Delete(deltrind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                    i--;
                    count = true;
                    break;
                }
                else if((*modRegion->GetAt(i)==pTri->m_tript1)&&(*modRegion->GetAt(i+1)==pTri->m_tript3)){
                    modRegion->InsertAfter(i,&pTri->m_tript2);
                    heightRegion->InsertAfter(i,&pTri->m_triheight2);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript3);
                    segar->Delete(delsgind);
                    //we delete a triangle from a triangulable network, and a circumscribed circle
                    //from a set of the circumscribed circles corresponding to the triangulable
                    //to triangles;
                    deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
                    triar->Delete(deltrind);
                    centerar->Delete(deltrind);
                    radar->Delete(deltrind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                    i--;
                    count = true;
                    break;
                }
                else if((*modRegion->GetAt(i)==pTri->m_tript3)&&(*modRegion->GetAt(i+1)==pTri->m_tript2)){
                    modRegion->InsertAfter(i,&pTri->m_tript1);
                    heightRegion->InsertAfter(i,&pTri->m_triheight1);
                    //we delete the considered triangle from the list of the deleted triangles
                    delsgind = segar->FindByCoords(0, pTri->m_tript3, pTri->m_tript2);
                    segar->Delete(delsgind);
                    //we delete a triangle from a triangulable network, and a circumscribed circle
                    //from a set of the circumscribed circles corresponding to the triangulable
                    //to triangles;
                    deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
                    triar->Delete(deltrind);
                    centerar->Delete(deltrind);
                    radar->Delete(deltrind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                    i--;
                    count = true;
                    break;
                }
            }
            //if the triangle for expansion of area of modification is found, we interrupt a cycle on
            //to all deleted triangles also we begin check with the beginning already inside
            //expanded area of modification
            if(i<modRegion->GetSize()-1)
                break;
            if((*modRegion->GetAt(modRegion->GetSize()-1)==pTri->m_tript2)&&(*modRegion->GetAt(0)==pTri->m_tript1)){
                modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript3);
                heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight3);
                //we delete the found segment from the list of triangulable segments
                delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
                segar->Delete(delsgind);
                //we delete a triangle from a triangulable network, and a circumscribed circle
                //from a set of the circumscribed circles corresponding to the triangulable
                //to triangles;
                deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
                triar->Delete(deltrind);
                centerar->Delete(deltrind);
                radar->Delete(deltrind);
                //we delete the considered triangle from the list of the deleted triangles
                deltris->Delete(j);
                count = true;
                break;
            }
            else if((*modRegion->GetAt(modRegion->GetSize()-1)==pTri->m_tript1)&&(*modRegion->GetAt(0)==pTri->m_tript3)){
                modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript2);
                heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight2);
                //we delete the found segment from the list of triangulable segments
                delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript3);
                segar->Delete(delsgind);
                //we delete a triangle from a triangulable network, and a circumscribed circle
                //from a set of the circumscribed circles corresponding to the triangulable
                //to triangles;
                deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
                triar->Delete(deltrind);
                centerar->Delete(deltrind);

                radar->Delete(deltrind);
                //we delete the considered triangle from the list of the deleted triangles
                deltris->Delete(j);
                count = true;
                break;
            }
            else if((*modRegion->GetAt(modRegion->GetSize()-1)==pTri->m_tript3)&&(*modRegion->GetAt(0)==pTri->m_tript2)){
                modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript1);
                heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight1);
                //we delete the found segment from the list of triangulable segments
                delsgind = segar->FindByCoords(0, pTri->m_tript3, pTri->m_tript2);
                segar->Delete(delsgind);
                //we delete a triangle from a triangulable network, and a circumscribed circle
                //from a set of the circumscribed circles corresponding to the triangulable
                //to triangles;
                deltrind = triar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3);
                triar->Delete(deltrind);
                centerar->Delete(deltrind);
                radar->Delete(deltrind);
                //we delete the considered triangle from the list of the deleted triangles
                deltris->Delete(j);
                count = true;
                break;
            }
        }
    }

    return true;
}

//===================================================================
//!forms sets of triangulable triangles and segments in the modified
/**areas on a contour of area and new, included in a triangulable network, height mark;
//we receive new triangulable segments connecting the entered height mark to tops
//the modified area, and new triangulable triangles lean on the new
//triangulable segments and contour of area of modification.
//ENTRANCE: the mark of height 'pHpt' entered into a triangulable network, area of modification 'modRegion'
//          heights of tops of area of modification 'heightRegion';
//EXIT: sets of the triangulable triangles 'm_triangles' focused clockwise
//      and the triangulable 'm_segments' segments in modification area
*/
bool MReliefLayer::TriangulationInModregion(MHeightPoint* pHpt, MPtArray* modRegion, MDuArray* heightRegion,
                                            MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar)
//===================================================================
{
    if((!modRegion)||(!pHpt)||(!heightRegion)||(!triar)||(!segar))
        return false;
    if(modRegion->GetSize()!=heightRegion->GetSize())
        return false;

    MTriangle* pTri;
    MSegment* pSeg;
    //coordinates of the entered height mark
    MPoint pt = pHpt->GetPoint(0);
    //height of the entered height mark
    double h = pHpt->GetHeight();
    MPoint center;
    double radius;

    for(int i=0; i<modRegion->GetSize()-1; i++){
        pTri = new MTriangle(pt, *modRegion->GetAt(i), *modRegion->GetAt(i+1),
                             h, *heightRegion->GetAt(i), *heightRegion->GetAt(i+1));
        triar->AddTail(pTri);
        CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
        centerar->AddTail(&center);
        radar->AddTail(&radius);
        delete pTri;
        if(segar->FindByCoords(0, pt, *modRegion->GetAt(i))==-1){
            pSeg = new MSegment(pt, *modRegion->GetAt(i), h, *heightRegion->GetAt(i));
            segar->AddTail(pSeg);
            delete pSeg;
        }
        if(segar->FindByCoords(0, pt, *modRegion->GetAt(i+1))==-1){
            pSeg = new MSegment(pt, *modRegion->GetAt(i+1), h, *heightRegion->GetAt(i+1));
            segar->AddTail(pSeg);
            delete pSeg;
        }
    }
    if(modRegion->GetHead() && heightRegion->GetHead())//tempNata
    {
        pTri = new MTriangle(pt, *modRegion->GetAt(modRegion->GetSize()-1), *modRegion->GetAt(0),
                             h, *heightRegion->GetAt(modRegion->GetSize()-1), *heightRegion->GetAt(0));
        CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
        centerar->AddTail(&center);
        radar->AddTail(&radius);
        triar->AddTail(pTri);
        delete pTri;


        if(segar->FindByCoords(0, pt, *modRegion->GetAt(modRegion->GetSize()-1))==-1){
            pSeg = new MSegment(pt, *modRegion->GetAt(modRegion->GetSize()-1), h, *heightRegion->GetAt(modRegion->GetSize()-1));
            segar->AddTail(pSeg);
            delete pSeg;
        }
        if(segar->FindByCoords(0, pt, *modRegion->GetAt(0))==-1){
            pSeg = new MSegment(pt, *modRegion->GetAt(0), h, *heightRegion->GetAt(0));
            segar->AddTail(pSeg);
            delete pSeg;
        }

    }
    return true;
}


//===================================================================
//!Allocation of area of modification - one-coherent area - for the set structural segment
//! on a set of triangulable triangles for inclusion of the set structural segment
//! in a triangulation
bool MReliefLayer::FindModregionForStructSegment(MPoint pt1, MPoint pt2, MTrArray* triar, MSgArray* segar,
                                                 MPtArray* centerar,MDuArray* radar, MPtArray* modRegion, MDuArray* heightRegion)
//===================================================================
{
    if(((!triar)||(!centerar))||(!radar))
        return false;
    int ntriangles = triar->GetSize();
    int ncircles = centerar->GetSize();
    int nradius = radar->GetSize();

    if((ntriangles!=ncircles)||(ntriangles!=nradius))
        return false;

    modRegion->ClearAll();
    heightRegion->ClearAll();

    //'deltris' - the array for the triangles deleted from a triangulable network
    MTrArray* deltris = new MTrArray();
    MTriangle* pTri;
    MPoint pt;

    //we check, whether the triangle on any of segments crosses the set i-th segment,
    //and not just in top;
    //if crosses, we delete i-th a triangle from a triangulable network, and described
    //a circle from a set of the circumscribed circles corresponding to the triangulable
    //to triangles;
    //we form the array of the deleted triangles
    for(int i=0; i<triar->GetSize(); i++){
        pTri = triar->GetAt(i);
        if((CROSSSECTION(pt1,pt2,pTri->m_tript1,pTri->m_tript2,&pt))&&((pTri->m_tript1!=pt)&&
                                                                       (pTri->m_tript2!=pt))){

            deltris->AddTail(pTri);
            triar->Delete(i);
            centerar->Delete(i);
            radar->Delete(i);
            i--;
        }
        else if((CROSSSECTION(pt1,pt2,pTri->m_tript2,pTri->m_tript3,&pt))&&
                ((pTri->m_tript2!=pt)&&(pTri->m_tript3!=pt))){
            deltris->AddTail(pTri);
            triar->Delete(i);
            centerar->Delete(i);
            radar->Delete(i);
            i--;
        }
        else if((CROSSSECTION(pt1,pt2,pTri->m_tript1,pTri->m_tript3,&pt))&&
                ((pTri->m_tript1!=pt)&&(pTri->m_tript3!=pt))){
            deltris->AddTail(pTri);
            triar->Delete(i);
            centerar->Delete(i);
            radar->Delete(i);
            i--;
        }
    }

    if(deltris->GetSize()==0){
        delete deltris;
        return true;
    }

    //NOW WE WILL WORK WITH THE array OF THE DELETED 'deltris' TRIANGLES!
    //1) at first we believe, the area of modification coincides with the zero deleted triangle
    //we focus modification area clockwise
    OrientTriangleByClock(deltris->GetAt(0));
    modRegion->AddTail(&deltris->GetAt(0)->m_tript1);
    modRegion->AddTail(&deltris->GetAt(0)->m_tript2);
    modRegion->AddTail(&deltris->GetAt(0)->m_tript3);
    heightRegion->AddTail(&deltris->GetAt(0)->m_triheight1);
    heightRegion->AddTail(&deltris->GetAt(0)->m_triheight2);
    heightRegion->AddTail(&deltris->GetAt(0)->m_triheight3);


    deltris->Delete(0);
    int delsgind;

    //2) from the list of other deleted triangles we look for such that one of his edges
    //coincides with an edge of the modified area;
    //we believe, the area of modification and all triangulable triangles are focused
    //clockwise, so the set of checks is narrowed;
    //we expand modification area, we won't include in it all deleted triangles yet
    while(deltris->GetSize()!=0){
        for(int j=0; j<deltris->GetSize(); j++){
            pTri = deltris->GetAt(j);
            int i = 0;
            for(i=0; i<modRegion->GetSize()-1;i++){
                if((*modRegion->GetAt(i)==pTri->m_tript2)&&(*modRegion->GetAt(i+1)==pTri->m_tript1)){
                    modRegion->InsertAfter(i,&pTri->m_tript3);
                    heightRegion->InsertAfter(i,&pTri->m_triheight3);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
                    segar->Delete(delsgind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                    j--;
                    break;
                }
                else if((*modRegion->GetAt(i)==pTri->m_tript1)&&(*modRegion->GetAt(i+1)==pTri->m_tript3)){
                    modRegion->InsertAfter(i,&pTri->m_tript2);
                    heightRegion->InsertAfter(i,&pTri->m_triheight2);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript3);
                    segar->Delete(delsgind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                    j--;
                    break;
                }
                else if((*modRegion->GetAt(i)==pTri->m_tript3)&&(*modRegion->GetAt(i+1)==pTri->m_tript2)){
                    modRegion->InsertAfter(i,&pTri->m_tript1);
                    heightRegion->InsertAfter(i,&pTri->m_triheight1);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript3, pTri->m_tript2);
                    segar->Delete(delsgind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                    j--;
                    break;
                }
            }
            if(i==modRegion->GetSize()-1){
                if((*modRegion->GetAt(modRegion->GetSize()-1)==pTri->m_tript2)&&(*modRegion->GetAt(0)==pTri->m_tript1)){
                    modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript3);
                    heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight3);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
                    segar->Delete(delsgind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                }
                else if((*modRegion->GetAt(modRegion->GetSize()-1)==pTri->m_tript1)&&(*modRegion->GetAt(0)==pTri->m_tript3)){
                    modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript2);
                    heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight2);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript1, pTri->m_tript3);
                    segar->Delete(delsgind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                }
                else if((*modRegion->GetAt(modRegion->GetSize()-1)==pTri->m_tript3)&&(*modRegion->GetAt(0)==pTri->m_tript2)){
                    modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript1);
                    heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight1);
                    //we delete the found segment from the list of triangulable segments
                    delsgind = segar->FindByCoords(0, pTri->m_tript3, pTri->m_tript2);
                    segar->Delete(delsgind);
                    //we delete the considered triangle from the list of the deleted triangles
                    deltris->Delete(j);
                }
            }
        }
    }

    delete deltris;

    return true;
}

//===================================================================
//!forms sets of triangulable triangles and segments in the modified
/**areas on a contour of area and the new, included in a triangulable network, structural segment;
//we receive new triangulable segments connecting the entered height mark to tops
//the modified area, and new triangulable triangles lean on the new
//triangulable segments and contour of area of modification.
//ENTRANCE: the mark of height of pHpt entered into a triangulable network, area of modification of modRegion
//heights of tops of area of modification of heightRegion;
//EXIT: sets of the triangulable triangles of m_triangles focused clockwise
//and the triangulable m_segments segments in modification area
*/
bool MReliefLayer::TriangulationInModregionForStructsg(MPoint pt1, MPoint pt2, MPtArray* modRegion,MDuArray* heightRegion,
                                                       MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar)
//===================================================================
{
    if((!modRegion)||(!heightRegion)||(!triar)||(!segar))
        return false;
    if(modRegion->GetSize()!=heightRegion->GetSize())
        return false;

    MTriangle* pTri;
    MSegment* pSeg;
    MPoint center;
    double radius;
    MPoint pt;

    //we will find numbers of tops in which the structural segment crosses modification area
    int index1 = -1;
    int index2 = -1;
    int i = 0;
    for(i=0; i<modRegion->GetSize(); i++){
        pt = *modRegion->GetAt(i);
        if(pt1 == pt)
            index1 = i;
        if(pt2 == pt)
            index2 = i;
        //as soon as found numbers of tops, we interrupt check
        if((index1!=-1)&&(index2!=-1))
            break;
    }

    //we will renumber modification area tops so that the beginning of a structural segment
    //coincided with zero top of area of modification;
    //respectively, we will renumber elements in the array of heights
    modRegion->CircleMove(index1);
    heightRegion->CircleMove(index1);
    if(index2>=index1)
        index2 -= index1;
    else
        index2 = modRegion->GetSize() - index1 + index2;

    index1 = 0;
    for(i=1; i<index2; i++){
        //if orientation clockwise

        if(OTS(*modRegion->GetAt(0),*modRegion->GetAt(i),*modRegion->GetAt(i+1))<0){
            pTri = new MTriangle(pt1, *modRegion->GetAt(i), *modRegion->GetAt(i+1),
                                 *heightRegion->GetAt(0), *heightRegion->GetAt(i), *heightRegion->GetAt(i+1));
            triar->AddTail(pTri);
            CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
            centerar->AddTail(&center);
            radar->AddTail(&radius);
            delete pTri;
            if(segar->FindByCoords(0, pt1, *modRegion->GetAt(i+1))==-1){
                pSeg = new MSegment(pt1, *modRegion->GetAt(i+1), *heightRegion->GetAt(0),
                                    *heightRegion->GetAt(i+1));
                segar->AddTail(pSeg);
                delete pSeg;
            }
        }
        else{
            pTri = new MTriangle(pt1, *modRegion->GetAt(i), pt2, *heightRegion->GetAt(0),
                                 *heightRegion->GetAt(i), *heightRegion->GetAt(index2));
            CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
            centerar->AddTail(&center);
            radar->AddTail(&radius);
            triar->AddTail(pTri);
            delete pTri;
            break;
        }
    }
    int j= 0;
    for(j=index2-1; j>i; j--){
        //if orientation counterclockwise
        if(OTS(*modRegion->GetAt(index2),*modRegion->GetAt(j-1),*modRegion->GetAt(j))<0){
            pTri = new MTriangle(pt2, *modRegion->GetAt(j-1), *modRegion->GetAt(j),
                                 *heightRegion->GetAt(index2), *heightRegion->GetAt(j-1), *heightRegion->GetAt(j));
            triar->AddTail(pTri);
            CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
            centerar->AddTail(&center);
            radar->AddTail(&radius);
            delete pTri;
            if(segar->FindByCoords(0, pt2, *modRegion->GetAt(j-1))==-1){
                pSeg = new MSegment(pt2, *modRegion->GetAt(j-1), *heightRegion->GetAt(index2),
                                    *heightRegion->GetAt(j-1));
                segar->AddTail(pSeg);
                delete pSeg;
            }
        }
    }

    //now we will construct triangulable triangles in the second part of area of modification
    //from the 'index2' index to the 'index1' index equal 0
    for(i=index2+1; i<modRegion->GetSize()-1; i++){
        //if orientation clockwise
        if(OTS(*modRegion->GetAt(index2),*modRegion->GetAt(i),*modRegion->GetAt(i+1))<0){
            pTri = new MTriangle(pt2, *modRegion->GetAt(i), *modRegion->GetAt(i+1),
                                 *heightRegion->GetAt(index2), *heightRegion->GetAt(i), *heightRegion->GetAt(i+1));
            triar->AddTail(pTri);
            CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
            centerar->AddTail(&center);
            radar->AddTail(&radius);
            delete pTri;
            if(segar->FindByCoords(0, pt2, *modRegion->GetAt(i+1))==-1){
                pSeg = new MSegment(pt2, *modRegion->GetAt(i+1), *heightRegion->GetAt(index2),
                                    *heightRegion->GetAt(i+1));
                segar->AddTail(pSeg);
                delete pSeg;
            }
        }
        else{
            pTri = new MTriangle(pt2, *modRegion->GetAt(i), pt1, *heightRegion->GetAt(index2),
                                 *heightRegion->GetAt(i), *heightRegion->GetAt(0));
            CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
            centerar->AddTail(&center);
            radar->AddTail(&radius);
            triar->AddTail(pTri);
            delete pTri;
            if(segar->FindByCoords(0, pt1, *modRegion->GetAt(i))==-1){
                pSeg = new MSegment(pt1, *modRegion->GetAt(i), *heightRegion->GetAt(index1),
                                    *heightRegion->GetAt(i));
                segar->AddTail(pSeg);
                delete pSeg;
            }
            break;
        }
    }
    if(i==modRegion->GetSize()-1){
        pTri = new MTriangle(pt2, *modRegion->GetAt(modRegion->GetSize()-1), *modRegion->GetAt(0),
                             *heightRegion->GetAt(index2), *heightRegion->GetAt(modRegion->GetSize()-1), *heightRegion->GetAt(0));
        triar->AddTail(pTri);
        CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
        centerar->AddTail(&center);
        radar->AddTail(&radius);
        delete pTri;
        if(segar->FindByCoords(0, pt2, *modRegion->GetAt(modRegion->GetSize()-1))==-1){
            pSeg = new MSegment(pt2, *modRegion->GetAt(modRegion->GetSize()-1),
                                *heightRegion->GetAt(index2), *heightRegion->GetAt(modRegion->GetSize()-1));
            segar->AddTail(pSeg);
            delete pSeg;
        }
    }
    else if(i==modRegion->GetSize()-2){
        pTri = new MTriangle(*modRegion->GetAt(modRegion->GetSize()-2), *modRegion->GetAt(modRegion->GetSize()-1),
                             *modRegion->GetAt(0), *heightRegion->GetAt(modRegion->GetSize()-2),
                             *heightRegion->GetAt(modRegion->GetSize()-1), *heightRegion->GetAt(0));
        triar->AddTail(pTri);
        CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
        centerar->AddTail(&center);
        radar->AddTail(&radius);
        delete pTri;
    }
    for(j=modRegion->GetSize()-2; j>i; j--){
        //if orientation counterclockwise
        if(OTS(*modRegion->GetAt(0),*modRegion->GetAt(j-1),*modRegion->GetAt(j))<0){
            pTri = new MTriangle(pt1, *modRegion->GetAt(j-1), *modRegion->GetAt(j),
                                 *heightRegion->GetAt(0), *heightRegion->GetAt(j-1), *heightRegion->GetAt(j));
            triar->AddTail(pTri);
            CIRCLEPT(pTri->m_tript1,pTri->m_tript2, pTri->m_tript3, &center, &radius);
            centerar->AddTail(&center);
            radar->AddTail(&radius);
            delete pTri;
            if(segar->FindByCoords(0, pt2, *modRegion->GetAt(j-1))==-1){
                pSeg = new MSegment(pt2, *modRegion->GetAt(j-1), *heightRegion->GetAt(index2),
                                    *heightRegion->GetAt(j-1));
                segar->AddTail(pSeg);
                delete pSeg;
            }
        }
    }

    return true;
}

/*
//===================================================================
//forms sets of triangulable triangles and segments in the modified
//areas on a contour of area and the new, included in a triangulable network, structural segment;
//we receive new triangulable segments connecting the entered height mark to tops
//the modified area, and new triangulable triangles lean on the new
//triangulable segments and contour of area of modification.
//ENTRANCE: the mark of height of pHpt entered into a triangulable network, area of modification of modRegion
//          heights of tops of area of modification of heightRegion;
//EXIT: sets of the triangulable triangles of m_triangles focused clockwise
//      and the triangulable m_segments segments in modification area
bool MReliefLayer::TriangulationInModregionForStructsg(MPoint pt1, MPoint pt2, MPtArray* modRegion,MDuArray* heightRegion,
                        MTrArray* triar, MSgArray* segar, MPtArray* centerar, MDuArray* radar)
//===================================================================
{
    if((!modRegion)||(!heightRegion)||(!triar)||(!segar))
        return false;
    if(modRegion->GetSize()!=heightRegion->GetSize())
        return false;

    MTriangle* pTri;
    MSegment* pSeg;
    MPoint center;
    double radius;
    MPoint pt;

    //we will find numbers of tops in which the structural segment crosses modification area
    int index1 = -1;
    int index2 = -1;
    for(int i=0; i<modRegion->GetSize(); i++){
        pt = *modRegion->GetAt(i);
        if(pt1 == pt)
            index1 = i;
        if(pt2 == pt)
            index2 = i;
        //as soon as found numbers of tops, we interrupt check
        if((index1!=-1)&&(index2!=-1))
            break;
    }

    //we will renumber modification area tops so that the beginning of a structural segment
 //coincided with zero top of area of modification;
 //respectively, we will renumber elements in the array of heights
    modRegion->CircleMove(index1);
    heightRegion->CircleMove(index1);
    if(index2>=index1)
        index2 -= index1;
    else
        index2 = modRegion->GetSize() - index1 + index2;

    index1 = 0;

    MPtArray* ptar1 = new MPtArray();
    for(int i=0; i<=index2; i++)
        ptar1->AddTail(modRegion->GetAt(i));
    MObjectList heightpts;
    FormListOfHeightpoints(ptar1, &heightpts);
    MTrArray* triar1 = new MTrArray();
    MSgArray* segar1 = new MSgArray();
    MPtArray* centerar1 = new MPtArray();
    MDuArray* radar1 = new MDuArray();
    FormRegionForTriangulation(&heightpts, triar1, segar1, centerar1, radar1);

    MPtArray* ptar2 = new MPtArray();
    for(int i=index2; i<modRegion->GetSize(); i++)
        ptar2->AddTail(modRegion->GetAt(i));
    ptar2->AddTail(modRegion->GetAt(0));
    heightpts.clear();;
    FormListOfHeightpoints(ptar2, &heightpts);
    MTrArray* triar2 = new MTrArray();
    MSgArray* segar2 = new MSgArray();
    MPtArray* centerar2 = new MPtArray();
    MDuArray* radar2 = new MDuArray();
    FormRegionForTriangulation(&heightpts, triar2, segar2, centerar2, radar2);


    delete ptar1;
    delete ptar2;
    delete centerar1;
    delete centerar2;
    delete radar1;
    delete radar2;

    int ind = segar1->FindByCoords(0,pt1,pt2);
    if(ind!=-1)
        segar1->Delete(ind);
    for(int i=0; i<segar1->GetSize(); i++){
        MSegment* pSeg = segar1->GetAt(i);
        if(!SGINREG(pSeg->m_sgpt1, pSeg->m_sgpt2, modRegion))
            segar1->Delete(i);
    }
    m_segments->AppendTail(segar1);
    for(int i=0; i<triar1->GetSize(); i++){
        MTriangle* pTri = triar1->GetAt(i);
        if(!IsTriangleInRegion(pTri->m_tript1, pTri->m_tript2, pTri->m_tript3, modRegion))
            triar1->Delete(i);
    }
    m_triangles->AppendTail(triar1);

    for(int i=0; i<segar2->GetSize(); i++){
        MSegment* pSeg = segar2->GetAt(i);
        if(!SGINREG(pSeg->m_sgpt1, pSeg->m_sgpt2, modRegion))
            segar1->Delete(i);
    }
    m_segments->AppendTail(segar2);
    for(int i=0; i<triar2->GetSize(); i++){
        MTriangle* pTri = triar2->GetAt(i);
        if(!IsTriangleInRegion(pTri->m_tript1, pTri->m_tript2, pTri->m_tript3, modRegion))
            triar1->Delete(i);
    }
    m_triangles->AppendTail(triar2);

    return true;
}
*/
//===================================================================
//!Allocation of area of modification - one-coherent area - for the deleted height mark
//! on a set of triangulable triangles for change of a set of the triangulable
//! triangles and segments
bool MReliefLayer::FindModregionForDeleteHpt(MHeightPoint* pHpt, MPtArray* modRegion, MDuArray* heightRegion)
//===================================================================
{
    //	int ntriangles = m_triangles->GetSize();

    modRegion->ClearAll();
    heightRegion->ClearAll();

    //'deltris' - the array for the triangles deleted from a triangulable network
    MTrArray* deltris = new MTrArray();
    MTriangle* pTri;
    MPoint pt;

    //we check, whether is the deleted height mark triangle top;
    //if is, we delete i-th a triangle from a triangulable network, and described
    //a circle from a set of the circumscribed circles corresponding to the triangulable
    //to triangles;
    pt = pHpt->GetPoint(0);
    //we form the array of the deleted triangles
    for(int i=0; i<m_triangles->GetSize(); i++){
        pTri = m_triangles->GetAt(i);
        if((pt == pTri->m_tript1)||(pt == pTri->m_tript2)||(pt == pTri->m_tript3)){
            if(deltris->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3)==-1){
                deltris->AddTail(pTri);
                m_triangles->Delete(i);
                i--;
            }
        }
    }
    //NOW WE WILL WORK WITH THE array OF THE DELETED 'deltris' TRIANGLES!
    //1) at first we believe, the area of modification coincides with the zero deleted triangle
    //we focus modification area clockwise
    OrientTriangleByClock(deltris->GetAt(0));

    MPoint ptfirst, ptlast; //initial and final points of area of modification
    if(pt==deltris->GetAt(0)->m_tript1){
        modRegion->AddTail(&deltris->GetAt(0)->m_tript1);
        modRegion->AddTail(&deltris->GetAt(0)->m_tript2);
        modRegion->AddTail(&deltris->GetAt(0)->m_tript3);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight1);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight2);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight3);
        ptfirst = deltris->GetAt(0)->m_tript2;
        ptlast = deltris->GetAt(0)->m_tript3;
    }
    else if(pt==deltris->GetAt(0)->m_tript2){
        modRegion->AddTail(&deltris->GetAt(0)->m_tript2);
        modRegion->AddTail(&deltris->GetAt(0)->m_tript3);
        modRegion->AddTail(&deltris->GetAt(0)->m_tript1);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight2);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight3);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight1);
        ptfirst = deltris->GetAt(0)->m_tript3;
        ptlast = deltris->GetAt(0)->m_tript1;
    }
    else if(pt==deltris->GetAt(0)->m_tript3){
        modRegion->AddTail(&deltris->GetAt(0)->m_tript3);
        modRegion->AddTail(&deltris->GetAt(0)->m_tript1);
        modRegion->AddTail(&deltris->GetAt(0)->m_tript2);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight3);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight1);
        heightRegion->AddTail(&deltris->GetAt(0)->m_triheight2);
        ptfirst = deltris->GetAt(0)->m_tript1;
        ptlast = deltris->GetAt(0)->m_tript2;
    }


    deltris->Delete(0);
    int delsgind;

    //2) from the list of other deleted triangles we look for such that one of his edges
    //coincides with an edge of the modified area;
    //we believe, the area of modification and all triangulable triangles are focused
    //clockwise, so the set of checks is narrowed;
    //we expand modification area, we won't include in it all deleted triangles yet
    while(deltris->GetSize()!=0){
        for(int j=0; j<deltris->GetSize(); j++){
            pTri = deltris->GetAt(j);
            if((pt==pTri->m_tript2)&&(ptfirst==pTri->m_tript1)){
                modRegion->InsertAfter(0,&pTri->m_tript3);
                heightRegion->InsertAfter(0,&pTri->m_triheight3);
                //we delete the found segment from the list of triangulable segments
                delsgind = m_segments->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
                m_segments->Delete(delsgind);
                //we delete the considered triangle from the list of the deleted triangles
                ptfirst = pTri->m_tript3;
                deltris->Delete(j);
                break;
            }

            else if((pt==pTri->m_tript1)&&(ptfirst==pTri->m_tript3)){
                modRegion->InsertAfter(0,&pTri->m_tript2);
                heightRegion->InsertAfter(0,&pTri->m_triheight2);
                //we delete the found segment from the list of triangulable segments
                delsgind = m_segments->FindByCoords(0, pTri->m_tript1, pTri->m_tript3);
                m_segments->Delete(delsgind);
                //we delete the considered triangle from the list of the deleted triangles
                ptfirst = pTri->m_tript2;
                deltris->Delete(j);
                break;
            }
            else if((pt==pTri->m_tript3)&&(ptfirst==pTri->m_tript2)){
                modRegion->InsertAfter(0,&pTri->m_tript1);
                heightRegion->InsertAfter(0,&pTri->m_triheight1);
                //we delete the found segment from the list of triangulable segments
                delsgind = m_segments->FindByCoords(0, pTri->m_tript3, pTri->m_tript2);
                m_segments->Delete(delsgind);
                //we delete the considered triangle from the list of the deleted triangles
                ptfirst = pTri->m_tript1;
                deltris->Delete(j);
                break;
            }
            else if((pt==pTri->m_tript1)&&(ptlast==pTri->m_tript2)){
                modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript3);
                heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight3);
                //we delete the found segment from the list of triangulable segments
                delsgind = m_segments->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
                m_segments->Delete(delsgind);
                //we delete the considered triangle from the list of the deleted triangles
                ptlast = pTri->m_tript3;
                deltris->Delete(j);
                break;
            }
            else if((pt==pTri->m_tript2)&&(ptlast==pTri->m_tript3)){
                modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript1);
                heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight1);
                //we delete the found segment from the list of triangulable segments
                delsgind = m_segments->FindByCoords(0, pTri->m_tript2, pTri->m_tript3);
                m_segments->Delete(delsgind);
                //we delete the considered triangle from the list of the deleted triangles
                ptlast = pTri->m_tript1;
                deltris->Delete(j);
                break;
            }
            else if((pt==pTri->m_tript3)&&(ptlast==pTri->m_tript1)){
                modRegion->InsertAfter(modRegion->GetSize()-1,&pTri->m_tript2);
                heightRegion->InsertAfter(heightRegion->GetSize()-1,&pTri->m_triheight2);
                //we delete the found segment from the list of triangulable segments
                delsgind = m_segments->FindByCoords(0, pTri->m_tript3, pTri->m_tript1);
                m_segments->Delete(delsgind);
                //we delete the considered triangle from the list of the deleted triangles
                ptlast = pTri->m_tript2;
                deltris->Delete(j);
                break;
            }
        }
    }
    if(*modRegion->GetAt(1) == *modRegion->GetAt(modRegion->GetSize()-1)){
        //we delete the found segment from the list of triangulable segments
        delsgind = m_segments->FindByCoords(0, pt, *modRegion->GetAt(1));
        m_segments->Delete(delsgind);
        modRegion->DeleteHead();
        modRegion->DeleteTail();
    }
    else{
        delsgind = m_segments->FindByCoords(0, pt, *modRegion->GetAt(1));
        m_segments->Delete(delsgind);
        delsgind = m_segments->FindByCoords(0, pt, *modRegion->GetAt(modRegion->GetSize()-1));
        m_segments->Delete(delsgind);
    }

    return true;
}

//===================================================================
//!forms sets of triangulable triangles and segments in the modified
//! areas on an area contour;
/**we receive new triangulable segments connecting tops of a contour of the modified
//areas, and new triangulable triangles lean on the new
//triangulable segments and contour of area of modification.
//REMARK: if the deleted mark of height is one of modification area tops,
//        sets of triangulable triangles and segments are already created.
//ENTRANCE: the mark of height 'pHpt' deleted from a triangulable network, area of modification 'modRegion'
//          heights of tops of area of modification 'heightRegion';
//EXIT: sets of the triangulable triangles 'm_triangles' focused clockwise
//      and the triangulable 'm_segments' segments in modification area
*/
bool MReliefLayer::TriangulationInModregionForDeleteHpt(MHeightPoint* pHpt, MPtArray* modRegion,MDuArray* /*heightRegion*/)

//===================================================================
{
    //if the deleted mark of height is one of modification area tops,
    //sets of triangulable triangles and segments are already created
    int index;
    int vertexind;
    if(PTONKONTUR(pHpt->GetPoint(0), modRegion, index, vertexind))
        return true;

    MTrArray* triar = new MTrArray();
    MSgArray* segar = new MSgArray();
    MPtArray* centerar = new MPtArray();
    MDuArray* radar = new MDuArray();

    MObjectList konturhpts;
    MPoint pt;
    //we will make the list of marks of heights forming a relief contour
    int i = 0;
    for(i=0; i<modRegion->GetSize(); i++)
    {
        pt = *modRegion->GetAt(i);
        OBITER it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            if(pt == ((MHeightPoint*)(*it))->GetPoint(0)){
                konturhpts.push_back(*it);
                break;
            }
            it++;
        }
    }
    MSegment* pSeg;
    MTriangle* pTri;

    FormRegionForTriangulation(&konturhpts, triar, segar, centerar, radar);
    for(i=0; i<triar->GetSize(); i++){
        pTri = triar->GetAt(i);
        if(m_triangles->FindByCoords(0, pTri->m_tript1, pTri->m_tript2, pTri->m_tript3)!=-1){
            triar->Delete(i);
            i--;
        }
    }
    m_triangles->AppendTail(triar);

    for(i=0; i<segar->GetSize(); i++){
        pSeg = segar->GetAt(i);
        if(m_segments->FindByCoords(0, pSeg->m_sgpt1, pSeg->m_sgpt2)!=-1){
            segar->Delete(i);
            i--;
        }
    }
    m_segments->AppendTail(segar);
    delete triar;
    delete segar;
    delete centerar;
    delete radar;

    return true;
}

//===================================================================
//!forms sets of triangulable triangles and segments in the set
//! areas, excepting the triangles and segments which aren't entering the set area
/**ENTRANCE: the contour of area of a triangulation of pReg set by the points
//EXIT: sets of the triangulable triangles and segments constructed on a basis
//      tops of the set contour, a set of circumscribed circles for the constructed
//      triangulable triangles, set by arrays of the centers of circles and them
//      radiuses
*/
bool MReliefLayer::FormRegionForTriangulation(MObjectList* heightpoints, MTrArray* triar,MSgArray* segar,
                                              MPtArray* centerar, MDuArray* radar)
//===================================================================
{
    if(heightpoints->size()==0)
        return false;
    if((!triar)||(!segar)||(!centerar)||(!radar))
        return false;


    triar->ClearAll();
    segar->ClearAll();
    centerar->ClearAll();
    radar->ClearAll();

    MRect* pRect = new MRect();
    MPtArray* pReg = new MPtArray();

    MString str;
    MPoint pt;
    //we form a contour of coordinates of boundary marks of heights
    OBITER it = heightpoints->begin();
    while(it != heightpoints->end())
    {
        pt = ((MHeightPoint*)(*it))->GetPoint(0);
        pReg->AddTail(&pt);
        it++;
    }
    //we will find a coverage rectangle for a contour
    FindBoundRect(pReg, pRect);

    //we will expand a few rectangle of coverage that all points of a contour were internal
    //for a rectangle
    pRect->left -= 2;
    pRect->right += 2;
    pRect->bottom -= 2;
    pRect->top += 2;

    //on the first step we will include two triangles in a set of triangulable triangles,
    //the diagonals received from a coverage rectangle by carrying out;
    //initial triangulable segments correspond to the first triangulable
    //to triangles
    MPoint pt1(pRect->left,pRect->bottom);
    MPoint pt2(pRect->left,pRect->top);
    MPoint pt3(pRect->right,pRect->top);
    MPoint pt4(pRect->right,pRect->bottom);

    MTriangle* pTri = new MTriangle(pt1, pt2, pt3, 0, 0, 0);
    triar->AddTail(pTri);
    pTri = new MTriangle(pt3, pt4, pt1, 0, 0, 0);
    triar->AddTail(pTri);
    MSegment* pSeg = new MSegment(pt1, pt3, 0, 0);
    segar->AddTail(pSeg);

    //we will find circumscribed circles for triangulable triangles
    CreateCircle(triar, centerar, radar);
    //we will include all points of a contour in triangulation area
    MPtArray* modRegion = new MPtArray();
    MDuArray* heightRegion = new MDuArray();
    //	int num = 0;
    it = heightpoints->begin();
    while(it != heightpoints->end())
    {
        modRegion->ClearAll();
        heightRegion->ClearAll();
        //for the included mark of height we calculate modification area
        FindModregionForTript((MHeightPoint*)(*it), triar, segar, centerar, radar, modRegion, heightRegion);
        //we add to a set of triangulable triangles and segments
        //triangulable triangles and segments, and in a set of the described
        //circles circumscribed circles for again included triangulable
        //triangles from modification area,
        TriangulationInModregion((MHeightPoint*)(*it), modRegion, heightRegion, triar, segar, centerar, radar);
        MString str= "рельефная";
        (*it)->m_dbarray.SetAt(0,&str);
        it++;
    }

    modRegion->ClearAll();
    heightRegion->ClearAll();

    //we will fix a triangulation contour; for this purpose for each edge of a contour we will check, whether enters
    //it in a set of triangulable segments if doesn't enter, we will change the triangulable
    //segments and triangles so that the edge of a contour entered a set of the triangulable
    //segments
    int index;
    int size = pReg->GetSize();
    int i = 0;
    for(i=0; i<pReg->GetSize()-1; i++){
        index = segar->FindByCoords(0,*pReg->GetAt(i), *pReg->GetAt(i+1));
        if(index == -1){
            //we will find area modification for a structural segment, that is a polygon,
            //the including structural segment so that the ends of a segment coincide with tops
            //an area contour, and all triangulable triangles which are crossed by the structural
            //a segment not only in the points coinciding with its ends
            FindModregionForStructSegment(*pReg->GetAt(i), *pReg->GetAt(i+1), triar, segar, centerar, radar,
                                          modRegion, heightRegion);
            //we will construct new triangulable triangles and segments in modification area,
            //which will be is among also by the set structural segment
            TriangulationInModregionForStructsg(*pReg->GetAt(i), *pReg->GetAt(i+1), modRegion,
                                                heightRegion, triar, segar, centerar, radar);
        }
    }
    //we make check for the last link of the contour of a triangulation connecting the initial
    //and final contour points
    index = segar->FindByCoords(0,*pReg->GetAt(size-1), *pReg->GetAt(0));
    if(index == -1){
        FindModregionForStructSegment(*pReg->GetAt(size-1), *pReg->GetAt(0), triar, segar, centerar, radar,
                                      modRegion, heightRegion);
        TriangulationInModregionForStructsg(*pReg->GetAt(size-1), *pReg->GetAt(0), modRegion,
                                            heightRegion, triar, segar, centerar, radar);
    }

    delete modRegion;
    delete heightRegion;

    //now it was necessary to clear a set of triangulable triangles of triangles,
    //not lying in a boundary contour
    for(i=0; i<triar->GetSize(); i++){
        pTri = triar->GetAt(i);
        //if the triangle doesn't lie in a boundary contour
        if(!IsTriangleInRegion(pTri->m_tript1, pTri->m_tript2, pTri->m_tript3, pReg)){
            //if the zero edge of a triangle doesn't lie on a boundary contour,
            //we will delete it from the list of triangulable segments
            if(!SGINREG_EXT(pTri->m_tript1, pTri->m_tript2, pReg)){
                index = segar->FindByCoords(0,pTri->m_tript1, pTri->m_tript2);
                segar->Delete(index);
            }
            //if the first edge of a triangle doesn't lie on a boundary contour,
            //we will delete it from the list of triangulable segments
            if(!SGINREG_EXT(pTri->m_tript2, pTri->m_tript3, pReg)){
                index = segar->FindByCoords(0,pTri->m_tript2, pTri->m_tript3);
                segar->Delete(index);
            }
            //if the second edge of a triangle doesn't lie on a boundary contour,
            //we will delete it from the list of triangulable segments
            if(!SGINREG_EXT(pTri->m_tript1, pTri->m_tript3, pReg)){
                index = segar->FindByCoords(0,pTri->m_tript1, pTri->m_tript3);
                segar->Delete(index);
            }
            //we will delete a triangle and the circumscribed circle corresponding to it
            triar->Delete(i);
            centerar->Delete(i);
            radar->Delete(i);
            i--;
        }
    }

    delete pRect;
    delete pReg;

    return true;
}

//===================================================================
//!clears lists of the main and additional horizontals and
//! deletes all main and additional horizontals
void MReliefLayer::RemoveIzolines()
//===================================================================
{
    OBITER it = m_izolines.begin();
    while(it != m_izolines.end())
    {
        delete (*it);
        it++;
    }
    m_izolines.clear();

    it = m_dopizolines.begin();
    while(it != m_dopizolines.end())
    {
        delete (*it);
        it++;
    }
    m_dopizolines.clear();
}

//===================================================================
//!sets to all marks of height not relief type
void MReliefLayer::SetTypeHeightPointsUnrelief()
//===================================================================
{
    OBITER it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        MString str= "нерельефная";
        (*it)->m_dbarray.SetAt(0,&str);
        it++;
    }
}

//===================================================================
//!forms the new list containing pointers on all objects of the first list behind an exception
//! objects pointers on which are included into the second list
bool MReliefLayer::RemoveObjectsFromOblist(MObjectList* heightpts, MObjectList* konturhpts,
                                           MObjectList* resultlist)
//===================================================================
{
    if((!heightpts)||(!konturhpts)||(!resultlist))
        return false;

    resultlist->clear();

    if(konturhpts->size()==0){
        resultlist->insert(resultlist->end(), heightpts->begin(), heightpts->end());
        return true;
    }

    resultlist->insert(resultlist->end(), heightpts->begin(), heightpts->end());
    OBITER it = konturhpts->begin();
    while(it != konturhpts->end())
    {
        OBITER it1 = std::find(resultlist->begin(), resultlist->end(), *it);
        if(it1 != resultlist->end())
            resultlist->erase(it1);
        it++;
    }

    return true;
}

//===================================================================
//!on coordinates of marks of heights set by the entrance array of points forms
//! the array of heights corresponding to points
bool MReliefLayer::FormHeightsOfPoints(MPtArray* points, MDuArray* heights)
//===================================================================
{
    if(!(points)||!(heights))
        return false;
    MPoint pt;
    double height;
    for(int i=0; i<points->GetSize(); i++){
        pt = *points->GetAt(i);
        OBITER it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            if(pt == ((MHeightPoint*)(*it))->GetPoint(0)){
                height = ((MHeightPoint*)(*it))->GetHeight();
                heights->AddTail(&height);
                break;
            }
            it++;
        }
    }
    if(points->GetSize() != heights->GetSize())
        return false;
    return true;
}

//===================================================================
//!on the array of points, forms the list of marks of heights, with the coordinates corresponding
//! to coordinates of points of the array
bool MReliefLayer::FormListOfHeightpoints(MPtArray* points, MObjectList* heightpts)
//===================================================================
{
    if((!points)||(!heightpts))
        return false;
    MPoint pt;
    //we will make the list of marks of heights
    for(int j=0; j<points->GetSize(); j++)
    {
        pt = *points->GetAt(j);
        OBITER it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            if(pt == ((MHeightPoint*)(*it))->GetPoint(0)){
                heightpts->push_back(*it);
                break;
            }
            it++;
        }
    }
    return true;
}

//===================================================================
//!triangulation of a mozhestvo of marks of heights in the set boundary contour,
//! in which tops there are marks of heights, according to Delone
bool MReliefLayer::CreatetriangulationByDelone(MObjectList* heightpoints)
//===================================================================
{
    MTrArray* triar = new MTrArray();
    MSgArray* segar = new MSgArray();
    MPtArray* centerar = new MPtArray();
    MDuArray* radar = new MDuArray();

    FormRegionForTriangulation(heightpoints, triar, segar, centerar, radar);

    //height marks coinciding with tops of a contour of a triangulation are already included in a set
    //triangulable points; we will allocate the remained height marks
    MObjectList innerhpts;
    RemoveObjectsFromOblist(&m_heightpoints, heightpoints, &innerhpts);

    MPtArray* modRegion = new MPtArray();
    MDuArray* heightRegion = new MDuArray();
    ///	MPoint pt;
    int k = 0;
    int colpoints = innerhpts.size();
    OBITER it = innerhpts.begin();
    while(it != innerhpts.end())
    {
        //if the point lies in a triangulation contour, we will include it in a triangulyatsionnoka
        //a set on Watson's algorithm (see materials on a Delone's triangulation )
        if(PointInRegion(((MHeightPoint*)(*it))->GetPoint(0), m_RelKonturPts)){
            //for the included mark of height we calculate modification area
            if(FindModregionForTript((MHeightPoint*)(*it), triar, segar, centerar, radar, modRegion, heightRegion))
                //we add to a set of triangulable triangles and segments
                //triangulable triangles and segments, and in a set of the described
                //circles circumscribed circles for again included triangulable
                //triangles from modification area,
                TriangulationInModregion((MHeightPoint*)(*it), modRegion, heightRegion,
                                         triar, segar, centerar, radar);
            MString str= "рельефная";
            (*it)->m_dbarray.SetAt(0,&str);
            //we care of information output in a state window
            GetKarta()->SetProgress(k*100/colpoints);
            GetKarta()->ShowProgress();
            k++;
        }
        it++;
    }
    delete modRegion;
    delete heightRegion;

    m_segments->ClearAll();
    m_triangles->ClearAll();
    m_segments->AppendTail(segar);
    m_triangles->AppendTail(triar);

    return true;
}

//===================================================================
//!includes all marks of height which aren't included earlier in a triangulation if they lie inside
//! relief contour
void MReliefLayer::TriangulateHeightPoints()
//===================================================================
{
    if(m_RelKonturPts->GetSize()==0){
        InternalMessage(RLF_MESSAGE7);//"The relief contour isn't created!"
        return;
    }

    MPtArray* centerar = new MPtArray();
    MDuArray* radar = new MDuArray();
    MPtArray* modRegion = new MPtArray();
    MDuArray* heightRegion = new MDuArray();
    //we will find circumscribed circles for triangulable triangles
    CreateCircle(m_triangles, centerar, radar);
    OBITER it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        //we include in a relief only those marks of height, which earlier
        //didn't participate in formation of a relief
        if(*(*it)->m_dbarray.GetAt(0)=="нерельефная"){
            modRegion->ClearAll();
            heightRegion->ClearAll();
            //if the point lies in a triangulation contour, we will include it in a triangulyatsionnoka
            //a set on Watson's algorithm (see materials on a Delone's triangulation )
            if(PointInRegion(((MHeightPoint*)(*it))->GetPoint(0), m_RelKonturPts)){
                //for the included mark of height we calculate modification area
                if(FindModregionForTript((MHeightPoint*)(*it), m_triangles, m_segments, centerar, radar, modRegion, heightRegion))
                    //we add to a set of triangulable triangles and segments
                    //triangulable triangles and segments, and in a set of the described
                    //circles circumscribed circles for again included triangulable
                    //triangles from modification area,
                    TriangulationInModregion((MHeightPoint*)(*it), modRegion, heightRegion,
                                             m_triangles, m_segments, centerar, radar);
                MString str= "рельефная";
                (*it)->m_dbarray.SetAt(0,&str);
            }
        }
        it++;
    }
    delete modRegion;
    delete heightRegion;
    delete centerar;
    delete radar;
}

//===================================================================
//!builds a Delone's triangulation  in the set contour;
/**thus triangulable triangles before procedure in a contour are deleted
//also are under construction new, according to a Delone's triangulation
//triangles from modification area,
*/
bool MReliefLayer::TriangulationByDeloneInRegion(MPtArray* pReg)
//===================================================================
{
    if(!pReg)
        return false;
    //we will allocate height marks which are in a contour
    MObjectList innerhpts;
    OBITER it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {
        if(PointInRegion(((MHeightPoint*)(*it))->GetPoint(0), pReg))
            innerhpts.push_back(*it);
        it++;
    }
    //we will make the list of marks of heights forming an entrance contour
    MObjectList konturhpts;
    MPoint pt;
    for(int j=0; j<pReg->GetSize(); j++)
    {
        pt = *pReg->GetAt(j);
        it = m_heightpoints.begin();
        while(it != m_heightpoints.end())
        {
            if(pt == ((MHeightPoint*)(*it))->GetPoint(0)){
                konturhpts.push_back(*it);
                break;
            }
            it++;
        }
    }
    //we will delete all triangulable triangles and segments in a contour
    MTrArray* newtriar = new MTrArray();
    MTriangle* pTri;
    int sgind;
    int i = 0;
    for(i=0; i<m_triangles->GetSize(); i++){
        pTri = m_triangles->GetAt(i);

        if(IsTriangleInRegion(pTri->m_tript1, pTri->m_tript2, pTri->m_tript3, pReg)){
            sgind = m_segments->FindByCoords(0, pTri->m_tript1, pTri->m_tript2);
            m_segments->Delete(sgind);
            sgind = m_segments->FindByCoords(0, pTri->m_tript2, pTri->m_tript3);
            m_segments->Delete(sgind);
            sgind = m_segments->FindByCoords(0, pTri->m_tript3, pTri->m_tript1);
            m_segments->Delete(sgind);
        }
        else
            newtriar->AddTail(pTri);
    }
    m_triangles->ClearAll();
    m_triangles->AppendTail(newtriar);
    delete newtriar;

    MTrArray* triar = new MTrArray();
    MSgArray* segar = new MSgArray();
    MPtArray* centerar = new MPtArray();
    MDuArray* radar = new MDuArray();

    //we will include a contour in a triangulation
    FormRegionForTriangulation(&konturhpts, triar, segar, centerar, radar);

    //we will include internal points of a contour in a triangulation
    MPtArray* modRegion = new MPtArray();
    MDuArray* heightRegion = new MDuArray();
    it = innerhpts.begin();
    while(it != innerhpts.end())

    {
        if(FindModregionForTript((MHeightPoint*)(*it), triar, segar, centerar, radar, modRegion, heightRegion)){
            //we add to a set of triangulable triangles and segments
            //triangulable triangles and segments, and in a set of the described
            //circles circumscribed circles for again included triangulable
            //triangles from modification area,
            TriangulationInModregion((MHeightPoint*)(*it), modRegion, heightRegion,
                                     triar, segar, centerar, radar);
        }
        it++;
    }
    delete modRegion;
    delete heightRegion;

    //we will make the array of new triangulable segments so,
    //that segments in it weren't duplicated
    MSgArray* newsgar = new MSgArray();
    newsgar->AddTail(segar->GetAt(0));
    MSegment* pSeg;
    for(i=1; i<segar->GetSize(); i++){
        pSeg = segar->GetAt(i);
        if(newsgar->FindByCoords(0, pSeg->m_sgpt1, pSeg->m_sgpt2)==-1)
            newsgar->AddTail(pSeg);
    }
    delete segar;
    m_segments->AppendTail(newsgar);
    m_triangles->AppendTail(triar);
    delete newsgar;
    delete triar;
    delete centerar;
    delete radar;

    return true;
}

//===================================================================
//!for a set of marks of heights builds the convex covering polygon,
//! which tops - height marks from a set
/**IDEA: 1) we find 'pt1' point with the maximum value 'x' - coordinates;
//       2) we find such 'pt2' point that all points of a set lie on the right side from a straight line
//          (pt1, pt2):
//       3) we find 'pt3' point, that all points of a set lie on the right side from a straight line
//          (pt2, pt3) and so on, we won't return to initial 'pt1' point yet.
*/
bool MReliefLayer::FindConvexPolygonForPoints()
//===================================================================
{
    if(m_RelKonturPts->GetSize()!=0){
        m_RelKonturPts->ClearAll();
        m_RelKonturHgt->ClearAll();
        m_segments->ClearAll();
        m_triangles->ClearAll();
    }
    MString str;
    double height;
    MPoint point;
    MPtArray* coordptar = new MPtArray();
    MDuArray* heightptar = new MDuArray();
    OBITER it = m_heightpoints.begin();
    while(it != m_heightpoints.end())
    {

        point = ((MHeightPoint*)(*it))->GetPoint(0);
        height = ((MHeightPoint*)(*it))->GetHeight();
        coordptar->AddTail(&point);
        heightptar->AddTail(&height);
        it++;
    }

    int colpts = coordptar->GetSize();
    if(colpts==0)
        return false;

    //if the set consists of one point
    if(colpts==1){
        m_RelKonturPts->AddTail(coordptar->GetAt(0));
        m_RelKonturHgt->AddTail(heightptar->GetAt(0));
        return true;
    }

    //we find 'pt1' point with the maximum value 'x' - coordinates
    double xmax = coordptar->GetAt(0)->x;
    int nomer = 0;
    //	int i = 0;
    int j;

    for(int i=1; i<colpts; i++){
        if(xmax < coordptar->GetAt(i)->x){
            xmax = coordptar->GetAt(i)->x;
            nomer = i;
        }
    }
    //first point of a convex polygon...
    m_RelKonturPts->AddTail(coordptar->GetAt(nomer));
    m_RelKonturHgt->AddTail(heightptar->GetAt(nomer));

    //if the set consists of two points
    j = ((nomer==0) ? 1 : 0);
    if(colpts==2){
        m_RelKonturPts->AddTail(coordptar->GetAt(j));
        m_RelKonturHgt->AddTail(heightptar->GetAt(j));
        return true;
    }

    MPoint pt1, pt2, pt, ptt;

    pt2 = *coordptar->GetAt(j);
    ptt = *m_RelKonturPts->GetAt(0);
    //we build a contour of a convex polygon, we won't return to the initial point yet
    while(pt2!=ptt){
        pt1 = *m_RelKonturPts->GetTail();
        //we find such point of pt2 that all points of a set lie on the right side
        //from a straight line (pt1, pt2):
        for(int i=0; i<colpts; i++){
            pt2 = *coordptar->GetAt(i);
            height = *heightptar->GetAt(i);
            int j = 0;
            for(j=0; j<colpts; j++){
                pt = *coordptar->GetAt(j);
                if((pt!=pt1)&&(pt!=pt2)){
                    //as soon as there is a point to the left of a straight line (pt1, pt2)
                    if(OTS(pt1, pt, pt2)<=0)
                        break;
                }
            }
            //if all points lie to the right of a straight line (pt1, pt2), we include 'pt2' point
            //in a contour of a convex polygon
            if(j==colpts){
                m_RelKonturPts->AddTail(&pt2);
                m_RelKonturHgt->AddTail(&height);
                break;
            }
        }
    }

    //we will delete the final point of a convex contour coinciding with initial
    m_RelKonturPts->DeleteTail();
    m_RelKonturHgt->DeleteTail();

    return true;
}

//===================================================================
//!in the array of structural lines looks for coinciding on coordinates with the input line set
//! array of points;
/**returns an index of the structural line in the array if the line was, and-1 otherwise
*/
int MReliefLayer::FindStructLine(MPtArray* line)
//===================================================================

{
    int index = -1;

    if(m_structlines->GetSize()==0)
        return -1;
    if(!line)
        return -1;
    int colpts = line->GetSize();
    MPtArray* structline;
    for(int i=0; i<m_structlines->GetSize(); i++){
        structline = (MPtArray*)m_structlines->GetAt(i);
        if(structline->GetSize()==colpts){
            int j = 0;
            for(j=0; j<colpts; j++){
                if(*structline->GetAt(j) != *line->GetAt(j))
                    break;
            }
            //if all points coincided, we found an index of the structural line
            if(j==colpts){
                index = i;
                break;
            }
        }
    }
    return index;
}

//===================================================================
//!fixing of the structural line
bool MReliefLayer::FixStructLine(MPtArray* line)
//===================================================================
{
    if(!line)
        return false;
    if(m_triangles->GetSize()==0)
        return false;

    MPtArray* centerar = new MPtArray();
    MDuArray* radar = new MDuArray();
    CreateCircle(m_triangles, centerar, radar);
    MPtArray* modRegion = new MPtArray();
    MDuArray* heightRegion = new MDuArray();

    //we don't consider degenerate edges when the beginning and the end of an edge coincide,
    for(int i=0; i<line->GetSize()-1; i++){
        if((m_segments->FindByCoords(0, *line->GetAt(i), *line->GetAt(i+1))== -1)&&
                (*line->GetAt(i) != *line->GetAt(i+1))){
            if(!FindModregionForStructSegment(*line->GetAt(i), *line->GetAt(i+1),
                                              m_triangles, m_segments, centerar, radar, modRegion, heightRegion))
                return false;
            if(!TriangulationInModregionForStructsg(*line->GetAt(i), *line->GetAt(i+1),
                                                    modRegion, heightRegion, m_triangles, m_segments, centerar, radar))
                return false;
        }
    }
    delete modRegion;
    delete heightRegion;
    delete centerar;
    delete radar;

    return true;
}

//===================================================================
//!fixing structural lines of a layer of a relief
bool MReliefLayer::FixAllStructLines()
//===================================================================
{
    if(m_triangles->GetSize()==0)
        return false;

    int k = 0;
    int colstructlines = m_structlines->GetSize() + m_macroforms.size() + m_gidroobjects.size();
    MPtArray* structline;
    for(int i=0; i<m_structlines->GetSize(); i++){
        structline = (MPtArray*)m_structlines->GetAt(i);
        FixStructLine(structline);
        //we care of information output in a state window
        GetKarta()->SetProgress(k*100/colstructlines);
        GetKarta()->ShowProgress();
        k++;
    }

    OBITER it =  m_macroforms.begin();
    while(it !=  m_macroforms.end())
    {
        structline = ((MObryv*)(*it))->GetStructKontur();
        FixStructLine(structline);
        if((*it)->WhatTheObject()==ID_YAMA){
            structline = ((MYama*)(*it))->GetDownStructKontur();
            FixStructLine(structline);
            //we care of information output in a state window
            GetKarta()->SetProgress(k*100/colstructlines);
            GetKarta()->ShowProgress();
            k++;
        }
        it++;
    }

    it =  m_gidroobjects.begin();
    while(it !=  m_gidroobjects.end())
    {
        structline = ((MGidro*)(*it))->GetStructKontur();
        FixStructLine(structline);
        //we care of information output in a state window
        GetKarta()->SetProgress(k*100/colstructlines);
        GetKarta()->ShowProgress();
        k++;
        it++;
    }

    return true;
}

void MReliefLayer::GetInfoForShpWrite(MStArray& names,MInArray* dlina)
{
    if(!dlina)return;

    int l, col, i = 0;
    MString st;

    // the number of fields is equal in the dbf-file to number of elements of the 'm_dbarray' array of a layer minus one zero element where the name of a layer is stored
    col = m_dbarray.GetSize();
    *m_dbarray[0] = "Field_1";
    *m_dbarray[1] = "Field_2";
    *m_dbarray[2] = "Field_3";
    names.SetSize(col);

    for(i=0; i<col; i++)
    {
        st = *m_dbarray.GetAt(i);
        names.SetAt(i, &st);
    }
    dlina->SetSize(col);
    for(i=0; i<col; i++)
        //	dlina->GetAt(i)[0] = 0;
    {
        l = m_dbarray[i]->GetLength();
        if(l > *dlina->GetAt(i))
            dlina->SetAt(i,&l);
    }

    //we will calculate length of fields
    OBITER it = m_izolines.begin();

    MObject* pObj;
    // for a layer of a relief we go according to the list of isolines
    while(it != m_izolines.end())
    {
        pObj = *it;
        if (pObj->WhatTheObject()==ID_IZOLINE)
        {
            //MTopobject* pHPt = (MTopobject*)pObj;
            MIzoline* pHPt = (MIzoline*)(*it);
            //whether we will check there is a 'height' attribute
            MNvArray* attr_arr = (MNvArray*)pHPt->m_Komplekt->GetAt(F_NADPIS);
            for(i = pHPt->m_pZnak->GetNomsArray()->GetSize()-1;i>=0;i--){
                MString str,strEmpty = "";
                unsigned short ushCurNum = *pHPt->m_pZnak->GetNomsArray()->GetAt(i);
                if(attr_arr->GetValue(ushCurNum,str)==-1){
                    MNomValue temp = MNomValue(ushCurNum,strEmpty);
                    attr_arr->AddTail(&temp);
                }
            }
            int attr_size = attr_arr->GetSize();
            MString str = "";
            for(i=0; i<attr_size; i++)
            {
                unsigned short num = attr_arr->GetAt(i)->GetNom();

                if((num == 19)&&(str==""))//"height"
                    str = attr_arr->GetAt(i)->GetValue();
            }

            if(str != "")
            {
                *(pHPt->m_dbarray[1]) = str;

                for(i=0; i<col; i++){
                    if(pHPt->m_dbarray.GetSize()>=i+1){
                        l = pHPt->m_dbarray[i]->GetLength();
                        if(l>*dlina->GetAt(i))  dlina->SetAt(i,&l);
                    }
                }
            }
        }

        it++;
    }
}

//************************************************
//******  Class of marks of heights   ********
//************************************************
//================================================
//!Constructor by default
MHeightPoint::MHeightPoint()
//================================================
{
    m_Komplekt->Delete(F_RAZRYVY);
    F_TOCHKI = 2;
    m_bVisible = true;//we draw height mark by default
    m_bTextVisible = true;//we draw the text at height mark by default

    m_dbarray.SetSize(ID_DBINITSIZE);
    MString str= "нерельефная";
    m_dbarray.SetAt(0,&str);

    m_bounds.left = 0;
    m_bounds.right = 0;
    m_bounds.top = 0;
    m_bounds.bottom = 0;

    //We form the array of points with one point - height mark
    MPoint point;
    point.x = 0;
    point.y = 0;
    MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    pta->AddTail(&point);
}

//!================================================
MHeightPoint::MHeightPoint(MPoint point, double /*height*/)
//================================================
{
    m_Komplekt->Delete(F_RAZRYVY);
    F_TOCHKI = 2;
    m_bVisible = true;//we draw height mark by default
    m_bTextVisible = true;//we draw the text at height mark by default

    m_dbarray.SetSize(ID_DBINITSIZE);
    MString str= "нерельефная";
    m_dbarray.SetAt(0,&str);

    m_bounds.left = point.x;
    m_bounds.right = point.x;
    m_bounds.top = point.y;
    m_bounds.bottom = point.y;

    //We form the array of points with one point - height mark
    MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    pta->AddTail(&point);
}

//====================================================================
//!Reading in UTP
bool MHeightPoint::ReadBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    MTopobject::ReadBin(h_nuxfile,  version);

    //we read a sign of drawing of a mark of height
    fread(&m_bVisible, 1, 1, h_nuxfile);
    //we read a sign of drawing of the text at height mark
    fread(&m_bTextVisible, 1, 1, h_nuxfile);

    return true;
}

//====================================================================
//!Reading in UTP
bool MHeightPoint::ReadBin(MFile* file,  int version)
//====================================================================
{
    MTopobject::ReadBin(file,  version);

    //we read a sign of drawing of a mark of height
    file->GetData(&m_bVisible, 1, 1);
    //we read a sign of drawing of the text at height mark
    file->GetData(&m_bTextVisible, 1, 1);

    return true;
}

//====================================================================
//!Writing in UTP
bool MHeightPoint::WriteBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    MTopobject::WriteBin(h_nuxfile,  version);

    //we write a sign of drawing of a mark of height
    fwrite(&m_bVisible, 1, 1, h_nuxfile);
    //we write a sign of drawing of the text at height mark
    fwrite(&m_bTextVisible, 1, 1, h_nuxfile);

    return true;
}

//=======================================
//!Returns the class size in UTP
unsigned int MHeightPoint::GetBytes()
//=======================================
{
    //Calculation of the extent of object in UTP
    //The extent of object in UTP is calculated thus:
    //Parent's size
    //+ sign of drawing of a mark of height (1 byte)

    unsigned int SBytes = MTopobject::GetBytes()+2;

    return SBytes;
}


//==================================
//! cloning
MObject* MHeightPoint::Clone()
//==================================
{
    MHeightPoint* pClone=new MHeightPoint();
    pClone->m_bounds=m_bounds;
    MasterClone(pClone);
    pClone->m_pZnak=NULL;
    return (MObject*) pClone;
}

//==============================================
//!We join the service classifier
void MHeightPoint::ConnectToServKls(int /*typeyaz*/)
//==============================================
{
    m_pZnak = GetKarta()->m_pHptZnak;
    m_kkod = SERV_OBJECT_HPT;
}

//==================================
//!function of obtaining height of a point
double MHeightPoint::GetHeight()
//==================================
{
    MString datnazv;
    MString strheight;
    double height;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "Height"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        m_pLay->GetKarta()->GetDictAttrName(nom,datnazv);
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Высота"))
            break;
    }
    //across Nome names of the characteristic "Height" we receive value of height of a point
    //in a point data file if that is available
    if((datnazv=="Высота") ||  (datnazv=="Абс.Высота")){
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom,strheight);
        if(index==-1)
            height = -1000000;
        else
            height = atof(strheight);
    }
    else
        height = -1000000;

    return height;
}

//==================================
//!function of installation of height of a point
bool MHeightPoint::SetHeight(double height)
//==================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    if(!attribs) return false;

    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "Height"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Высота"))
            break;
    }
    //across Nome names of the characteristic "Height" we set value of height of a point
    //in a point data file if that is available
    if((datnazv=="Высота") || (datnazv=="Абс.Высота") )//tempNata
    {
        //we will round value of height to two signs after a comma
        double roundedh = SetRoundedNumber(height,2);
        MString str;
        str.Format("%.2f",roundedh);

        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->SetValue(nom,str);
        if(index==-1){
            MNomValue* nomval = new MNomValue(nom,str);
            ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        }
    }

    else
        return false;

    return true;
}


//==================================
//!function of obtaining number of a mark of height
int MHeightPoint::GetNomer()
//==================================
{
    MString datnazv;
    MString strnomer;
    int nomer;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "number"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="номер"))
            break;
    }
    //across Nome names of the characteristic "number" we get a number of a mark of height
    //in height mark data file if that is available
    if(datnazv=="номер"){
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom,strnomer);
        if(index==-1)
            nomer = -1;
        else
            nomer = atoi(strnomer);
    }
    else
        nomer = -1;

    return nomer;
}

//==================================
//!function of obtaining number of a mark of height
bool MHeightPoint::SetNomer(int nomer)
//==================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    if(!attribs) return false;

    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "number"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="номер"))
            break;
    }
    //across Nome names of the characteristic "number" we get a number of a mark of height
    //in height mark data file if that is available
    if(datnazv=="номер"){
        MString str;
        str.Format("%d", nomer);
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->SetValue(nom,str);
        if(index==-1){
            MNomValue* nomval = new MNomValue(nom,str);
            ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        }
    }
    else
        return false;

    return true;
}

//==================================
//!function of receiving type of height mark
MString MHeightPoint::GetType()
//==================================
{
    MString datnazv;
    MString strsignif = "";
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom;
    //in the array we look for number of the name of the characteristic "type"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="тип"))
            break;
    }
    //across Nome names of the characteristic "type" we receive the importance of a mark of height
    //in height mark data file if that is available
    //	if(datnazv=="тип")
    //		int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom,strsignif);

    return strsignif;
}

//==================================
//!function of receiving type of height mark
bool MHeightPoint::SetType(MString signif)
//==================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    if(!attribs) return false;

    MString datnazv;
    //we receive the pointer on the array of admissible codes
    MShInArray* noms = (MShInArray*)m_pZnak->GetNomsArray();
    unsigned short nom=0;
    //in the array we look for number of the name of the characteristic "type"
    for(int i=0; i< noms->GetSize(); i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="тип"))
            break;
    }
    //across Nome names of the characteristic "type" we receive the importance of a mark of height
    //in height mark data file if that is available
    if(datnazv=="тип"){
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->SetValue(nom,signif);
        if(index==-1){
            MNomValue* nomval = new MNomValue(nom,signif);
            ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        }
    }
    else
        return false;

    return true;
}
//==================================
//!Drawing functions
void  MHeightPoint::Draw(MPlotDevice* pDevice)
//==================================
{
    if(!m_bVisible) return;
    MTopobject::Draw(pDevice);
}
//==================================
//!Functions of drawing of the text
/**for height marks from two types of formulas "Height" and "Height/number"
//only one type of a formula is deleted
*/
void  MHeightPoint::DrawText(MPlotDevice* pDevice)
//==================================
{
    if((!m_bVisible)||(!m_bTextVisible)) return;
    MLayer* pLay = GetLayer();
    if(!pLay) return;
    MReliefLayer* pReliefLay = NULL;
    if(pLay->WhatTheLayer()==ID_RELIEF_LAYER)
        pReliefLay = (MReliefLayer*)pLay;

    MPtArray* privs = new MPtArray();
    MFormArray* formar;
    int colform = 0;
    MString formname;
    int indhform = -1;//index of a formula "Height"
    int indhnform = -1;//index of a formula "Height/number"
    //for a relief layer if the tag 'm_viewhptnom' is included, we delete at a mark
    //heights more informative formula "Height/number", otherwise
    //we delete a formula "Height"
    if(pReliefLay!=NULL){
        //in description language among formulas we look for formulas with the name
        //"Height" and "Height/number";
        formar = m_pZnak->GetYaz()->m_formula;
        colform = formar->GetSize();
        for(int i=0; i<colform; i++){
            formname = formar->GetAt(i)->nazvanie;
            if(formname=="Высота")
                indhform = i;
            else if(formname=="Высота/номер")
                indhnform = i;
            //as soon as found both formulas...
            if((indhform!=-1)&&(indhnform!=-1))
                break;
        }
        //we will temporarily delete all bindings of a formula "Height" if they are
        if((pReliefLay->m_viewhptnom)&&(indhform!=-1)){
            //we will keep bindings in the 'privs' array
            GetDoubleVektorsPriv(indhform,privs);
            DeleteVektorsPriv(indhform);
        }
        //we will temporarily delete all bindings of a formula "Height/number" if they are
        else if((!pReliefLay->m_viewhptnom)&&(indhnform!=-1)){
            //we will keep bindings in the 'privs' array
            GetDoubleVektorsPriv(indhnform,privs);
            DeleteVektorsPriv(indhnform);
        }
    }

    //we draw the text
    MTopobject::DrawText(pDevice);

    //for a layer of a relief we restore all bindings if is what to restore
    if((pReliefLay!=NULL)&&(privs->GetSize()!=0)){
        //we will restore bindings of a formula "Height" if they are
        if((pReliefLay->m_viewhptnom)&&(indhform!=-1)){
            for(int i=0; i<=privs->GetSize()-2; i++)
                AddVektorPriv(indhform,*privs->GetAt(i),*privs->GetAt(i+1));
        }
        //we will restore bindings of a formula "Height/number" if they are
        else if((!pReliefLay->m_viewhptnom)&&(indhnform!=-1)){
            for(int i=0; i<=privs->GetSize()-2; i++){
                AddVektorPriv(indhnform,*privs->GetAt(i),*privs->GetAt(i+1));
            }
        }
    }

    delete privs;
}

//zone of flooding
MFloodZone::MFloodZone(MKarta *m_karta, MRect &zoomRect,double &height)
{
    this->m_karta = m_karta;
    this->zoomRect = zoomRect;
    this->levelOfWater = height;

    step = fabs(zoomRect.right - zoomRect.left) / 500;

    if (step < m_karta->m_bmhfile->Step())
        step = m_karta->m_bmhfile->Step();

    maxDistance = 1000;
    n = (int)zoomRect.Width()/step;
    m = (int)zoomRect.Height()/step;

    progressBar = new QProgressBar();
}

//installation of the maximum 'QProgressBar' value
void MFloodZone::SetMaxProgress(int max)
{
    progressBar->setMaximum(max);
    progressBar->setValue(0);
}
//increase in 'QProgressBar' value at unit
void MFloodZone::SetProgress()
{
    progressBar->setValue(progressBar->value()+1);
}

//creation of a zone of flooding
void MFloodZone::GetFloodZone(MPtArray* piontOfSelTpo, std::vector<int> &resultTri)
{
    if(piontOfSelTpo)
    {
        int size =  piontOfSelTpo->Size;
        for(int ind = 0; ind<size;++ind)
        {
            MPoint pt = piontOfSelTpo->GetAt(ind);
            double height = GetHight(pt) + levelOfWater;
            bool pos = true;
            MPoint pt1,pt2,pt3;

            int currentPos = GetNumReliefTriForPoint(&pt,pt1,pt2,pt3, pos);

            std::vector<int> visitedTri;
            visitedTri.resize(2*m*n);

            std::queue<int> triQueue;
            triQueue.push(currentPos);

            vector <int>::iterator it = find(resultTri.begin(),resultTri.end(),currentPos);
            if(it==resultTri.end())
                resultTri.push_back(currentPos);
            visitedTri[currentPos] = 1;

            GetTriWithLessHeight(&pt,height,triQueue,visitedTri,resultTri);
            SetProgress();
        }
    }
}

// formation of the list of triangles which tops are below the set height
void MFloodZone::GetTriWithLessHeight(MPoint * pt1,double height, std::queue< int> &triQueue , std::vector<int> &visitedTri,std::vector<int> &resultTri )
{
    vector <int>::iterator it;
    while(!triQueue.empty())
    {
        int triPos  = triQueue.front();
        triQueue.pop();

        int borderTriNum1,borderTriNum2,borderTriNum3;
        if(triPos%2)
        {
            borderTriNum1 = triPos+1;
            borderTriNum2 = triPos-2*n+1;
            borderTriNum3 = triPos-1;
        }
        else
        {
            borderTriNum1 =  triPos-1;
            borderTriNum2 =  triPos+1;
            borderTriNum3 =  triPos+2*n-1;
        }

        if(borderTriNum1 > 0 && borderTriNum1 <= 2*n*m)
        {
            if(!visitedTri[borderTriNum1])
            {
                MPtArray * ptArr = GetTriByNumber(borderTriNum1);
                if(ptArr->GetSize()>0)
                {
                    MPoint pt = ptArr->GetAt(1);
                    double L = DLINASG(pt,pt1);
                    if(L<maxDistance)
                    {
                        double h = GetHight(pt);
                        if( h <=height )
                        {
                            triQueue.push(borderTriNum1);
                            it = find(resultTri.begin(),resultTri.end(),borderTriNum1);
                            if(it==resultTri.end())
                                resultTri.push_back(borderTriNum1);
                        }
                    }
                }
                visitedTri[borderTriNum1] = 1;
                delete ptArr;//temp
            }
        }

        if(borderTriNum2 > 0 && borderTriNum2 <= 2*n*m)
        {
            if(!visitedTri[borderTriNum2])
            {
                MPtArray * ptArr = GetTriByNumber(borderTriNum2);
                if(ptArr->GetSize()>0)
                {
                    MPoint pt = ptArr->GetAt(2);
                    double L = DLINASG(pt,pt1);
                    if(L<maxDistance)
                    {
                        double h = GetHight(pt);
                        if( h <=height )
                        {
                            triQueue.push(borderTriNum2);
                            it = find(resultTri.begin(),resultTri.end(),borderTriNum2);
                            if(it==resultTri.end())
                                resultTri.push_back(borderTriNum2);
                        }
                    }
                }
                visitedTri[borderTriNum2] = 1;
                delete ptArr;//temp
            }
        }

        if(borderTriNum3 > 0 && borderTriNum3 <= 2*n*m)
        {
            if(!visitedTri[borderTriNum3])
            {
                MPtArray * ptArr = GetTriByNumber(borderTriNum3);
                if(ptArr->GetSize()>0)
                {
                    MPoint pt = ptArr->GetAt(0);
                    double L = DLINASG(pt,pt1);
                    if(L<maxDistance)
                    {
                        double h = GetHight(pt);
                        if( h <=height )
                        {
                            triQueue.push(borderTriNum3);
                            it = find(resultTri.begin(),resultTri.end(),borderTriNum3);
                            if(it==resultTri.end())
                            {
                                resultTri.push_back(borderTriNum3);
                            }
                        }
                    }
                }
                visitedTri[borderTriNum3] = 1;
                delete ptArr;//temp
            }
        }
    }
};

// receiving a set of coordinates of a treygolnik according to number
MPtArray  *  MFloodZone::GetTriByNumber(int number)
{
    MPtArray  * ptArr = new MPtArray();
    int div  = number %2;
    for( int i = 0; i<n-1;++i )
        for(int j = 0; j<m-1; ++j)
        {
            if(div==0)
            {
                if(2*(j-1)*n+2*i==number)
                {
                    ptArr->AddTail(new MPoint(zoomRect.left+i*step,zoomRect.top-(j+1)*step));
                    ptArr->AddTail(new MPoint(zoomRect.left+(i+1)*step,zoomRect.top-(j+1)*step));
                    ptArr->AddTail(new MPoint(zoomRect.left+(i+1)*step,zoomRect.top-j*step));
                    return ptArr;
                }
            }
            else
                if(2*(j-1)*n+2*i-1==number)
                {
                    ptArr->AddTail(new MPoint(zoomRect.left+i*step,zoomRect.top-(j+1)*step));
                    ptArr->AddTail(new MPoint(zoomRect.left+i*step,zoomRect.top-j*step));
                    ptArr->AddTail(new MPoint(zoomRect.left+(i+1)*step,zoomRect.top-j*step));
                    return ptArr;
                }
        }
    return ptArr;
}

//obtaining number of a triangle on its coordinates
int MFloodZone::GetNumReliefTriForPoint(MPoint *pt,MPoint &pt1,MPoint &pt2, MPoint &pt3, bool &pos)
{
    //	MPtArray * triArr = new MPtArray();
    double x1 = 0.0,x2 = 0.0,y1 = 0.0,y2 = 0.0;

    MPoint tempPT(zoomRect.left, zoomRect.top);
    MRect curRect;
    int i,j;
    for( i = 0; i<n-1;++i )
    {
        x1 = tempPT.x+i*step;
        x2 = tempPT.x+(i+1)*step;
        if(pt->x>=x1 && pt->x<=x2)
        {
            curRect.left = x1;
            curRect.right = x2;
            break;
        }
    }
    for(j = 0; j<m-1; ++j)
    {
        y1 = tempPT.y-j*step;
        y2 = tempPT.y-(j+1)*step;
        if(pt->y<=y1 && pt->y>=y2)
        {
            curRect.top = y1;
            curRect.bottom = y2;
            break;
        }
    }
    if(IsPointInTriangle(pt,pt1,pt2,pt3))
    {
        pt1 =  MPoint(x1,y2);
        pt2 =  MPoint(x1,y1);
        pt3 =  MPoint(x2,y1);
        pos = true;
    }
    else
    {
        pt1 =  MPoint(x1,y2);
        pt2 =  MPoint(x2,y2);
        pt3 =  MPoint(x2,y1);
        pos = false;
    }
    int currentPos;
    if(pos)
        currentPos = 2*j*n+2*i-1;
    else
        currentPos = 2*j*n+2*i;

    return currentPos;
}
//sets the radius of search of a zone of flooding
void MFloodZone::SetMaxDistance(double distance)
{
    maxDistance = distance;
}

void  MFloodZone::CreatePtArrFloodZone(std::vector<int> &resultTri)
{
    MPtArray * ptArr = new MPtArray();
    const int iResultTriSize = (int)resultTri.size();
    for(int i=0; i<iResultTriSize;++i)
    {
        int num1 = resultTri[i];
        MPtArray *arr = GetTriByNumber(num1);
        if(arr->Size>0)
        {
            if(num1%2)
            {
                if(ptArr->Size==0)
                {
                    /*if(find(resultTri.begin(),resultTri.end(),num1+2*n-2)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n-3)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n-4)!=resultTri.end())
                    {
                    MPoint newPt(arr->GetAt(0)->x-step,arr->GetAt(0)->y);
                    ptArr->AddTail(&newPt);
                    }
                    else*/
                    ptArr->AddTail(arr->GetAt(0));

                    if(find(resultTri.begin(),resultTri.end(),num1-2*n-1)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-2*n-2)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-2*n-3)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-4)!=resultTri.end())
                    {
                        MPoint newPt(arr->GetAt(1)->x-step,arr->GetAt(1)->y);
                        ptArr->AddTail(&newPt);
                    }
                    else
                        ptArr->AddTail(arr->GetAt(1));
                }

                if(find(resultTri.begin(),resultTri.end(),num1+1)==resultTri.end()
                        && find(resultTri.begin(),resultTri.end(),num1+2)==resultTri.end()
                        && find(resultTri.begin(),resultTri.end(),num1+3)==resultTri.end())
                {
                    /*if(find(resultTri.begin(),resultTri.end(),num1-2*n+3)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1-2*n+4)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1-2*n+5)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+4)!=resultTri.end())
                    {
                    MPoint newPt(arr->GetAt(2)->x+2*step,arr->GetAt(2)->y);
                    ptArr->AddTail(&newPt);
                    }
                    else*/

                    ptArr->AddTail(arr->GetAt(2));

                    /*if(find(resultTri.begin(),resultTri.end(),num1+2*n)==resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n+1)==resultTri.end())
                    {
                    MPoint newPt(arr->GetAt(0)->x+step,arr->GetAt(0)->y);
                    ptArr->AddTail(&newPt);
                    }
                    else
                    if(find(resultTri.begin(),resultTri.end(),num1+2*n+2)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n+3)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n+4)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+4)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+5)!=resultTri.end())
                    {
                    MPoint newPt(arr->GetAt(0)->x+2*step,arr->GetAt(0)->y);
                    ptArr->AddTail(&newPt);
                    }
                    else*/
                    ptArr->AddTail(arr->GetAt(0));

                    m_Komplekt->AddTail(ptArr->Clone());
                    ptArr->ClearAll();
                }
            }
            else
            {
                if(ptArr->Size==0)
                {
                    /*if( find(resultTri.begin(),resultTri.end(),num1+2*n-3)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n-4)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1+2*n-5)!=resultTri.end()||
                    find(resultTri.begin(),resultTri.end(),num1-4)!=resultTri.end()	)
                    {
                    MPoint newPt(arr->GetAt(0)->x-step,arr->GetAt(0)->y);
                    ptArr->AddTail(&newPt);
                    }
                    else*/
                    ptArr->AddTail(arr->GetAt(0));

                    if( find(resultTri.begin(),resultTri.end(),num1-2*n)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-2*n-1)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-2*n-2)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-3)!=resultTri.end()	)
                    {
                        MPoint newPt(arr->GetAt(2)->x-step,arr->GetAt(2)->y);
                        ptArr->AddTail(&newPt);
                    }
                    else
                        ptArr->AddTail(arr->GetAt(2));
                }

                if(find(resultTri.begin(),resultTri.end(),num1+1)==resultTri.end()
                        && find(resultTri.begin(),resultTri.end(),num1+2)==resultTri.end()
                        && find(resultTri.begin(),resultTri.end(),num1+3)==resultTri.end())
                {
                    if(find(resultTri.begin(),resultTri.end(),num1-2*n+2)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-2*n+3)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1-2*n+4)!=resultTri.end())
                    {
                        MPoint newPt(arr->GetAt(2)->x+step,arr->GetAt(2)->y);
                        ptArr->AddTail(&newPt);
                    }
                    else
                        ptArr->AddTail(arr->GetAt(2));

                    if(find(resultTri.begin(),resultTri.end(),num1+2*n+1)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1+2*n+2)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1+2*n+3)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1+2*n+4)!=resultTri.end()||
                            find(resultTri.begin(),resultTri.end(),num1+4)!=resultTri.end())
                    {
                        MPoint newPt(arr->GetAt(1)->x+step,arr->GetAt(1)->y);
                        ptArr->AddTail(&newPt);
                    }
                    else
                        ptArr->AddTail(arr->GetAt(1));

                    m_Komplekt->AddTail(ptArr->Clone());
                    ptArr->ClearAll();
                }
            }
            delete arr;
        }
    }
    delete ptArr;
}

//sorting of the list of triangles according to increase of number
void  MFloodZone::SortTriArr(std::vector<int> &resultTri)
{
    int temp=0;
    int count = resultTri.size();
    for (int i=1;  i<count  ;  i++)
    {
        for (int j=0;  j<count-i;  j++)
        {
            if (resultTri [j]>resultTri [j+1])
            {
                temp=resultTri[j];
                resultTri [j]=resultTri [j+1];
                resultTri [j+1]=temp;
            }
        }
    }
}

MPoint MFloodZone::ConvertPoint(MPoint& pt, bool bIn)
{
    if(!m_karta) return pt;
    if(!m_karta->m_pObjectCoo) return pt;

    MPoint ret_pt = pt;
    if ( m_karta->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC )
        return  ret_pt;           // native = global

    if(bIn)	//  to native coordinates  (from global)
    {
        //the point is drawn from a map
        if((ret_pt.x<1000)&&(ret_pt.y<1000))      // really grads ?
        {
            MGeodesicCoo coo;        //MExtendedGaussKrugerCoo
            coo.SetStandartEllipsoid(m_karta->m_pObjectCoo->GetEllipsoidName());
            double h = 0.0F;
            coo.ConvertToCoo(m_karta->m_pObjectCoo, ret_pt, h);
        }
    }
    else //  to global coordinates  (from native)
    {//the point will be transformed for display on the map
        if(!((ret_pt.x<1000)&&(ret_pt.y<1000)))     // really no grads ?
        {
            MGeodesicCoo coo;
            coo.SetStandartEllipsoid(m_karta->m_pObjectCoo->GetEllipsoidName());
            double h = 0.0F;
            m_karta->m_pObjectCoo->ConvertToCoo(&coo, ret_pt, h);
        }
    }
    return ret_pt;
}

int MFloodZone::GetHight(MPoint& pt, double* hRel, double* hObj, double* hCor)
//==================================
{
    if(!m_karta) return -1000000;
    MPoint tmp_pt = ConvertPoint(pt);
    return m_karta->GetHeightFromMatrixes(tmp_pt, hRel, hObj, hCor);
}
