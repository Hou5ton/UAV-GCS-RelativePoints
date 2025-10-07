

#include "Topobject.h"
#include "Klassifikator.h"
#include "privpoints.h"
#include "DrawObj.h"
#include "Refglobal.h"
#include<QDebug>

//Pointers on function of transformation for a concrete step of drawing
extern double (*ConvertSizeToReal)(MTopobject* pObj,float fSize);
extern MObject* (*fpCreateObjectByType)(MString str);
extern MZnak* (*fpCreateZnakByType)(MString str);

//================================================
//Constructor by default
//!******  Class of the main objects of the map   ********
MTopobject::MTopobject()
//================================================
{
    //We form structure of top object
    F_TOCHKI = 3;// Index of the first part of points  (m_Komplekt)


    m_dbarray.SetSize(ID_DBINITSIZE);
    for(int i = 0;i<ID_DBINITSIZE;i++){
        MString str= "";
        m_dbarray.SetAt(i,&str);
    }


    //We establish the set size
    m_Komplekt->FreeMemory();

    //We will set the set size

    m_Komplekt->SetSize(F_TOCHKI+1);

    //By default inscription is empty
    MNvArray* nomvals = new MNvArray();
    m_Komplekt->SetAt(F_NADPIS,(MArray*)nomvals);

    //We form the array of vectors-bindings of formulas
    MPvArray* vectpriv = new MPvArray();
    m_Komplekt->SetAt(F_FORMPRIV,(MArray*)vectpriv);

    //We form the array of ruptures of a contour of top object
    MRzArray* razryvy = new MRzArray();
    m_Komplekt->SetAt(F_RAZRYVY,(MArray*)razryvy);

    //We form the empty array of points
    MPtArray* pta =new MPtArray();
    m_Komplekt->SetAt(F_TOCHKI,(MArray*)pta);

    m_pZnak=NULL;
    m_kkod="";

    m_formula_ind = -1;

    m_status = false;
    m_excited = false;
    bDrawColor = false;
    bDrawColorZalivka = false;
}

//================================================
//!constructor with a task of number of points
MTopobject::MTopobject(int kol_points)
//================================================
{
    //We form structure of top object
    F_TOCHKI = 3;// Index of the first part of points  (m_Komplekt)


    m_dbarray.SetSize(ID_DBINITSIZE);
    for(int i = 0;i<ID_DBINITSIZE;i++){
        MString str= "";
        m_dbarray.SetAt(i,&str);
    }


    //We establish the set size
    m_Komplekt->FreeMemory();

    //We will set the set size
    m_Komplekt->SetSize(F_TOCHKI+1);

    //By default inscription is empty
    MNvArray* nomvals = new MNvArray();
    m_Komplekt->SetAt(F_NADPIS,(MArray*)nomvals);

    //We form the array of vectors-bindings of formulas
    MPvArray* vectpriv = new MPvArray();
    m_Komplekt->SetAt(F_FORMPRIV,(MArray*)vectpriv);

    //We form the array of ruptures of a contour of top object
    MRzArray* razryvy = new MRzArray();
    m_Komplekt->SetAt(F_RAZRYVY,(MArray*)razryvy);

    //We form the empty array of points
    MPtArray* pta =new MPtArray();
    pta->SetSize(kol_points);
    m_Komplekt->SetAt(F_TOCHKI,(MArray*)pta);

    m_pZnak=NULL;
    m_kkod="";

    m_formula_ind = -1;

    m_status = false;
    m_excited = false;
    bDrawColor = false;
    bDrawColorZalivka = false;
}

//!=============================================
MTopobject::MTopobject(const MRect& position)
//=============================================
{

    F_TOCHKI = 3;// Index of the first part of points  (m_Komplekt)
    m_bounds       = position;
    //We form structure of top object
    m_dbarray.SetSize(ID_DBINITSIZE);
    for(int i = 0;i<ID_DBINITSIZE;i++){
        MString str= "";
        m_dbarray.SetAt(i,&str);
    }


    //We establish the set size
    m_Komplekt->FreeMemory();

    //We will set the set size
    m_Komplekt->SetSize(F_TOCHKI+1);

    //By default inscription is empty
    MNvArray* nomvals = new MNvArray();
    m_Komplekt->SetAt(F_NADPIS,(MArray*)nomvals);

    //We form the array of vectors-bindings of formulas
    MPvArray* vectpriv = new MPvArray();
    m_Komplekt->SetAt(F_FORMPRIV,(MArray*)vectpriv);

    //We form the array of ruptures of a contour of top object
    MRzArray* razryvy = new MRzArray();
    m_Komplekt->SetAt(F_RAZRYVY,(MArray*)razryvy);




    //We form the empty array of points
    MPtArray* pta =new MPtArray();
    m_Komplekt->SetAt(F_TOCHKI,(MArray*)pta);

    m_pZnak=NULL;
    m_kkod="";
    m_pLay = NULL;

    m_formula_ind = -1;

    m_status = false;
    m_excited = false;
    bDrawColor = false;
    bDrawColorZalivka = false;
};


//!=============================================
MTopobject::~MTopobject()
//=============================================
{
    //	if(m_pZnak) delete m_pZnak;
}

//====================================================================
//!Reading from an UTP-format
bool  MTopobject::ReadBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    MObject::ReadBin(h_nuxfile,version);
    //reading quad-code of object
    //m_kkod=ReadString(h_nuxfile, version);

    return true;
}

//====================================================================
//!Reading from an UTP-format
bool  MTopobject::ReadBin(MFile* file,  int version)
//====================================================================
{
    MObject::ReadBin(file,version);
    //reading quad-code of object
    //	m_kkod=ReadString(file, version);
    //	file->SetPosition(4, MAP_SEEK_CUR);

    return true;
}

//====================================================================
//!Writing in UTP
bool MTopobject::WriteBin(FILE* h_nuxfile,  int version)
//====================================================================
{
    MObject::WriteBin(h_nuxfile,  version);
    //writing quad-code of object
    //	WriteString(h_nuxfile,version,m_kkod);

    return true;
};


//****** DRAWING OF FORMULAS *******************************************
//==========================================================
//! -----
/** name = <field name>, out = <the received array of lines>
//Upon transition to index and dictionary model to an entrance of function we submit not the name
//characteristics, and an index (code) - nome number according to the dictionary
*/
void MTopobject::ObtainMassivStrok(unsigned short nom, MStArray* out)
//==========================================================
{
    //the size of the array of lines with names of fields
    int Size = m_pZnak->GetNomsArray()->GetSize();
    int i;
    //we will check, whether the entrance nome lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        if (m_pZnak->GetNomsArray()->GetAt(i)[0]==nom){
            break;
        }
    }
    //if the entrance nome lies in the array of admissible codes
    if(i<Size){
        MString str;
        //if in an array of characteristics of top object values of the characteristic lie with
        //entrance Nome, we will receive them
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom, str);
        //if in individual characteristics the attribute isn't defined,
        //that we look for it in patrimonial characteristics
        if(index==-1)
        {
            index = m_pZnak->GetGenattrArray()->GetValue(nom, str);
        }
        //values are...
        if(index!=-1)
            //we receive the array of the lines 'out'
            MScanfArr(&str, out, separator);
    }
}

//==================================
//! Drawing of the text: uses functions
//! GetFormulaDlina, DrawFormula, ObtainMassivStrok
void MTopobject::DrawText(MPlotDevice* pDevice)
//==================================
{
    //dr 24.03.2016
    if(!m_pZnak) return;

    MDrawing::DrawText(this,pDevice,m_pZnak->GetYaz(),(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV));
};	


//==================================
//! test: to what formula points the mouse pointer?
/** serves for top objects and their successors
//INPUT PARAMETER - 'point' - a point, to which we check bindings of formulas for proximity;
//OUTPUT PARAMETERS: 'nomer' - number of a binding in the array of bindings of formulas
*/
bool MTopobject::HitFormulaTest(MPoint point, double dDostup,int& nomer)
//==================================
{
    nomer = -1;

    MPriv* pPriv;
    MPtArray* pta = new MPtArray();
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    //on all bindings of formulas we check to which the entrance point is close
    //we check proximity of a point to a binding tracker;
    //for all types of bindings, except spline, the tracker coincides with the broken line made
    //consecutive binding points, for a spline we form a tracker
    for(int i=0; i<privs->GetSize(); i++){


        pta->ClearAll();
        pPriv = privs->GetAt(i);
        pPriv->GetDrawArray(pta);
        if(pPriv->GetType()==SPLINE_PRIVYAZKA){
            MPtArray* trackpts = new MPtArray();
            //forms the broken line from 10 points corresponding to a spline
            FormLomlineBySimpleSpline(*pta->GetAt(0),*pta->GetAt(1),*pta->GetAt(2),
                                      *pta->GetAt(3),10,trackpts);
            //we form a binding tracker broken line (we add coordinates of bars to the beginning
            //and end of a broken line of a spline)
            trackpts->AddHead(pta->GetAt(2));
            trackpts->AddTail(pta->GetAt(3));
            pta->ClearAll();
            pta->AppendTail(trackpts);
            delete trackpts;
        }
        for(int k = 0;k<pta->GetSize();k++){
            //we check for proximity to all edges of a tracker
            if((pta->GetSize()>1)&&(k!=pta->GetSize()-1)){
                if(RASSTPTSG(&point,pta->GetAt(k),
                             pta->GetAt(k+1))<=dDostup){
                    nomer = i;
                    delete pta;
                    return true;
                }
            }
            //we check for proximity to the ends of a tracker
            else if((DLINASG(point,pta->GetAt(0))<=dDostup)||
                    (DLINASG(point,pta->GetAt(pta->GetSize()-1))<=dDostup)){
                nomer = i;
                delete pta;
                return true;
            }
        }

    }

    delete pta;
    return false;
}

//============================================================
//!adds a vector-binding with coordinates (pt1, pt2) for a formula number э
//! to the array of vectors-bindings.
/**If addition happened normally, true comes back
//Function doesn't add a vector for not existing formulas
*/
bool MTopobject::AddVektorPriv(int FIndex,MPoint pt1,MPoint pt2)
//============================================================
{
    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((FIndex<0)||(FIndex>=FCount))return false;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPriv* priv = new MDoublePriv(pt1, pt2, (unsigned char)FIndex);
    privs->AddTail(priv);
    delete priv;

    return true;
}


//============================================================
//!adds a binding type 'bType' for a formula number 'FIndex'
/**If addition happened normally, 'true' returns
*/
bool MTopobject::AddPriv(int FIndex,MPtArray& pta,unsigned char bType /* = SIMPLE_PRIVYAZKA*/)
//============================================================
{
    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((FIndex<0)||(FIndex>=FCount))return false;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPriv* priv = NULL;
    switch(bType)
    {
    default:

    case SIMPLE_PRIVYAZKA:
    {
        if(pta.GetSize()<1)
            return false;
        priv = new MSimplePriv(*pta.GetAt(0),(unsigned char)FIndex);
    }
        break;

    case DOUBLE_PRIVYAZKA:
    {
        if(pta.GetSize()<2)
            return false;
        priv = new MDoublePriv(*pta.GetAt(0),*pta.GetAt(1),(unsigned char)FIndex);
    }
        break;

    case VINOSKA_PRIVYAZKA:
    {
        if(pta.GetSize()<3)
            return false;
        priv = new MVinoskaPriv(*pta.GetAt(0),pta.GetAt(1),pta.GetAt(2),(unsigned char)FIndex);
    }
        break;

    case SPLINE_PRIVYAZKA:
    {
        if(pta.GetSize()<4)
            return false;
        priv = new MSplinePriv(*pta.GetAt(0), *pta.GetAt(1),*pta.GetAt(2),*pta.GetAt(3),(unsigned char)FIndex);
    }
        break;
    }
    privs->AddTail(priv);
    delete priv;
    return true;
}



//============================================================

//!adds a spline-binding with coordinates (pt1, pt2, pt3, pt4) for a formula number 'FIndex'
//! to the array of splines-bindings.
/**If addition happened normally, 'true' returns
//Function doesn't add a spline for not existing formulas
*/
bool MTopobject::AddSplinePriv(int FIndex,MPoint pt1,MPoint pt2,MPoint pt3,MPoint pt4)
//============================================================
{
    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();

    //we do nothing for nonexistent formulas
    if((FIndex<0)||(FIndex>=FCount))return false;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPriv* priv = new MSplinePriv(pt1, pt2, pt3, pt4,(unsigned char)FIndex);
    privs->AddTail(priv);
    delete priv;
    return true;
}

//=====================================================================
//!establishes a formula vector-binding with number 'FIndex', the met VIndex-th
/**in case of success we return 'true'
//aren't established nonexistent a vector
*/
bool MTopobject::SetVektorPriv(int FIndex, int VIndex, MPoint pt1, MPoint pt2)
//=====================================================================
{
    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((VIndex<0)||(FIndex<0)||(FIndex>=FCount))return false;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs=(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPriv* priv;
    unsigned char formnomer;
    int vectnomer = -1;//the counter of vectors-bindings of a formula with number 'FIndex'
    int i = 0;
    for(i=0; i<privs->GetSize(); i++){
        priv = privs->GetAt(i);
        formnomer = priv->GetFormNomer();
        if((int)formnomer == FIndex){
            vectnomer++;
            if(vectnomer==VIndex)
                break;
        }
    }
    //The vector of a binding isn't found
    if(i==privs->GetSize())
        return false;
    MDoublePriv newPriv(pt1,pt2,FIndex);
    privs->SetAt(i,&newPriv);

    return true;
}

//=====================================================================
//!to remove a formula vector-binding with number 'FIndex', the met VIndex-th
/**in case of success we return 'true'
//aren't removed nonexistent a vector
*/
bool MTopobject::DeleteVektorPriv(int FIndex, int VIndex)
//=====================================================================
{

    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((VIndex<0)||(FIndex<0)||(FIndex>=FCount))return false;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs=(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPriv* priv;
    unsigned char formnomer;
    int vectnomer = -1;//the counter of vectors-bindings of a formula with number 'FIndex'
    int i = 0;
    for(i=0; i<privs->GetSize(); i++){
        priv = privs->GetAt(i);
        formnomer = priv->GetFormNomer();
        if((int)formnomer == FIndex){
            vectnomer++;
            if(vectnomer==VIndex)
                break;
        }
    }
    //The vector of a binding isn't found
    if(i==privs->GetSize())
        return false;

    privs->Delete(i);

    return true;
}

//=====================================================================
//!To give everything a binding vector for a formula number 'FIndex'
/**Returns a binding vector only the point-to-point
*/
bool MTopobject::GetDoubleVektorsPriv(int FIndex, MPtArray* pta)
//=====================================================================
{

    if(!pta) return false;


    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((FIndex<0)||(FIndex>=FCount))return false;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs=(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    unsigned char formnomer;
    MPoint ptfirst, ptlast;
    for(int i=0; i<privs->GetSize(); i++){
        MPriv* priv = privs->GetAt(i);
        formnomer = priv->GetFormNomer();
        //if number of a formula of the current vector-binding coincided
        //with entrance number of a formula, we will add a vector binding to the 'pta' array
        if(((int)formnomer==FIndex)&&(priv->GetPointNum()==2)){
            ptfirst = priv->GetFirstPoint();
            ptlast = priv->GetLastPoint();
            pta->AddTail(&ptfirst);
            pta->AddTail(&ptlast);
        }
    }
    return true;
}

//=====================================================================
//!To receive the name
MString MTopobject::GetName()
//=====================================================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    MShInArray* noms = m_pZnak->GetNomsArray();
    int colnoms = noms->GetSize();
    unsigned short nom= 0;
    MString polename;
    MString strName;
    //we check, whether there is a number of the characteristic "Name" in the array of admissible codes
    for(int i=0; i<colnoms; i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,polename))&&(polename=="Название"))
            break;
    }
    //if is, we receive the name in an array of individual characteristics
    if(polename=="Название"){
        int index = attribs->GetValue(nom,strName);
        //if in an array of individual characteristics there was no value of the characteristic
        //"Name", we return an empty line
        if(index==-1){
            strName = "";

        }
    }
    return strName;
}

//=====================================================================
//!To set the name
void MTopobject::SetName(MString str)
//=====================================================================
{
    MNvArray* attribs = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    MShInArray* noms = m_pZnak->GetNomsArray();

    int colnoms = noms->GetSize();
    unsigned short nom= 0;
    MString polename;
    //we check, whether there is a number of the characteristic "Name" in the array of admissible codes
    for(int i=0; i<colnoms; i++){
        nom = noms->GetAt(i)[0];
        if((m_pLay->GetKarta()->GetDictAttrName(nom,polename))&&(polename=="Название"))
            break;
    }
    //if is, we establish the name in an array of individual characteristics
    if(polename=="Название"){
        int index = attribs->SetValue(nom,str);
        //if in an array of individual characteristics there was no value of the characteristic
        //"Name", we will add it
        if(index==-1){
            MNomValue* nomval = new MNomValue(nom,str);
            attribs->AddTail(nomval);

        }
    }
}

//=====================================================================
//!In the array of vectors bindings of formulas we delete everything a vector binding
//! for a formula with number 'FIndex'
bool MTopobject::DeleteVektorsPriv(int FIndex)
//=====================================================================
{
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);

    for(int i=0; i<privs->GetSize();i++){
        MPriv* priv = privs->GetAt(i);
        if((int)priv->GetFormNomer() == FIndex){
            privs->Delete(i);
            i--;
        }
    }
    return true;
}

//==============================================================
//!to give quantity of bindings of a certain type for a certain formula
int MTopobject::GetCountPrivs(int FIndex, unsigned char btype)
//==============================================================
{
    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((FIndex<0)||(FIndex>=FCount))return 0;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs=(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    int pcount = privs->GetSize();

    //now we will walk on the array of bindings we will count their quantity
    MPriv* ppriv;
    int result = 0;
    for(int i=0; i<pcount; i++)
    {
        ppriv = privs->GetAt(i);
        if((ppriv->GetType()==btype)&&(ppriv->GetFormNomer()==FIndex))
        {
            result++;
        }
    }
    return result;
}

//==============================================================
//!to remove all bindings of a certain type for a certain formula
void MTopobject::DeletePrivs(int FIndex, unsigned char btype)
//==============================================================
{
    //we receive the array of formulas
    MFormArray* fra = m_pZnak->GetYaz()->m_formula;

    int FCount = fra->GetSize();
    //we do nothing for nonexistent formulas
    if((FIndex < 0) || (FIndex >= FCount))
        return;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);

    //now we will walk on the array of bindings and we will delete all bindings of the set type
    int i = 0;
    int pcount = privs->GetSize();
    MPriv* ppriv;

    while((i < pcount) && (pcount > 0))
    {
        ppriv = privs->GetAt(i);

        if((ppriv->GetType() == btype) && (ppriv->GetFormNomer() == FIndex))
        {
            privs->Delete(i);
            --pcount;
        }
        else
        {
            ++i;
        }
    }
}

//==============================================================
//!to give the pointer on a certain binding of a certain type for a certain formula
/**if such binding doesn't exist, we return NULL
*/
MPriv* MTopobject::GetPriv(int FIndex, int PIndex, unsigned char btype)
//==============================================================
{
    //we receive the array of formulas
    MFormArray* fra=m_pZnak->GetYaz()->m_formula;
    int FCount=fra->GetSize();
    //we do nothing for nonexistent formulas
    if((FIndex<0)||(FIndex>=FCount))return NULL;

    //counter
    int nomer = -1;

    //We receive the index on the array of vectors-bindings
    MPvArray* privs=(MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);

    //now we will walk on the array of bindings and we will try to find a vector necessary to us
    int i=0;
    int pcount = privs->GetSize();
    MPriv* ppriv;
    while(i<pcount)
    {
        ppriv = privs->GetAt(i);
        if((ppriv->GetType()==btype)&&(ppriv->GetFormNomer()==FIndex))
        {
            nomer++;
            //if found that looked for, we will return
            if(nomer==PIndex)
            {
                return ppriv;
            }
        }
        ++i;
    }

    return NULL;
}

//=====================================================================
//!In arrays of bindings of formulas we change numbers of formulas with number 'oldnomer' for 'newnomer'
bool MTopobject::ChangeFormulaNomer(int oldnomer, int newnomer)
//=====================================================================
{
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    for(int i=0; i<privs->GetSize();i++){
        MPriv* priv = privs->GetAt(i);
        if((int)priv->GetFormNomer() == oldnomer)
            priv->SetFormNomer((unsigned char)newnomer);
    }
    return true;
}

//================================================
//!To be connected to a sign
bool MTopobject::ConnectZnak(MZnak* pZnak)
//================================================
{
    //If the sign is found
    if(pZnak)
    {
        //If classes coincide
        if(pZnak->GetClassType()==WhatTheObject())
            m_pZnak=pZnak;
        //If classes don't coincide
        else
        {
            if(WhatTheObject()==ID_TOPOBJECT){
                if(m_pZnak){
                    MYaz* pYaz = m_pZnak->GetYaz();
                    if(pYaz){
                        int type = pYaz->GetType();
                        ConnectToServKls(type);
                    }
                    else
                        ConnectToServKls();
                }
                else
                    ConnectToServKls();
            }
            else
                ConnectToServKls();
        }
    }
    //If the sign isn't found
    else
    {
        if(WhatTheObject()==ID_TOPOBJECT){
            if(m_pZnak){
                MYaz* pYaz = m_pZnak->GetYaz();
                if(pYaz){
                    int type = pYaz->GetType();
                    ConnectToServKls(type);
                }
                else
                    ConnectToServKls();
            }
            else
                ConnectToServKls();
        }
        else
            ConnectToServKls();
    }

    return true;
}


//==============================================
//!We change an object class
void MTopobject::ChangeClass(MString newclass,bool ignoreKKod)
//==============================================
{
    //we look for a sign with a new quad-code
    MString realklass;
    MZnak* znak = GetKarta()->m_pKls->GetZnak(newclass,realklass,ignoreKKod);
    //we will remember quad-code object
    realklass = m_kkod;
    //If the sign is found
    if(znak)
    {
        //If classes coincide
        if(znak->GetClassType()==WhatTheObject())
        {
            m_pZnak=znak;
            m_kkod = newclass;
        }
        //If classes don't coincide
        else
        {
            //in case of top object if the pointer on a sign nonzero, we find out language type
            //for drawing of top object (it always zero language in the array of languages of a sign),
            //also we will join a sign of the service classifier of the corresponding type
            if(WhatTheObject()==ID_TOPOBJECT){
                if(m_pZnak){
                    MYaz* pYaz = m_pZnak->GetYaz();
                    if(pYaz){
                        int type = pYaz->GetType();
                        ConnectToServKls(type);
                        //it is necessary to keep information on an old sign of object,
                        //to have opportunity to join an old sign
                        if(GetPartOfString(0,realklass)==SERV_OBJECT)
                            m_kkod = realklass;
                        //before quad-code an old sign we add an accession sign
                        //to the service classifier if it there wasn't earlier
                        else{
                            m_kkod = SERV_OBJECT;
                            m_kkod = m_kkod + " " + realklass;
                        }
                    }
                    //if the array of languages of a sign was empty, we will join the linear
                    //to a sign of the service classifier
                    else{
                        ConnectToServKls();
                        if(GetPartOfString(0,realklass)==SERV_OBJECT)
                            m_kkod = realklass;
                        else{
                            m_kkod = SERV_OBJECT;
                            m_kkod = m_kkod + " " + realklass;
                        }
                    }
                }
                //in case of the successor of top object we join the corresponding
                //to a sign of the service classifier
                else{
                    ConnectToServKls();
                    if(GetPartOfString(0,realklass)==SERV_OBJECT)
                        m_kkod = realklass;
                    else{
                        m_kkod = SERV_OBJECT;
                        m_kkod = m_kkod + " " + realklass;
                    }
                }
            }
            //if the pointer on a sign zero, we join the corresponding
            //to a sign of the service classifier (for top object to a linear sign)
            else{
                ConnectToServKls();
                if(GetPartOfString(0,realklass)==SERV_OBJECT)
                    m_kkod = realklass;
                else{
                    m_kkod = SERV_OBJECT;
                    m_kkod = m_kkod + " " + realklass;
                }
            }
        }
    }
    //If the sign isn't found,we join the service classifier also,
    //as in case of discrepancy of a class of object and the identifier of object in a sign
    else
    {
        if(WhatTheObject()==ID_TOPOBJECT){
            if(m_pZnak){
                MYaz* pYaz = m_pZnak->GetYaz();
                if(pYaz){
                    int type = pYaz->GetType();
                    ConnectToServKls(type);
                    if(GetPartOfString(0,realklass)==SERV_OBJECT)
                        m_kkod = realklass;
                    else{
                        m_kkod = SERV_OBJECT;
                        m_kkod = m_kkod + " " + realklass;
                    }
                }
                else{
                    ConnectToServKls();
                    if(GetPartOfString(0,realklass)==SERV_OBJECT)
                        m_kkod = realklass;
                    else{
                        m_kkod = SERV_OBJECT;
                        m_kkod = m_kkod + " " + realklass;
                    }
                }
            }
            else{
                ConnectToServKls();
                if(GetPartOfString(0,realklass)==SERV_OBJECT)
                    m_kkod = realklass;
                else{
                    m_kkod = SERV_OBJECT;
                    m_kkod = m_kkod + " " + realklass;
                }
            }
        }
        else{
            ConnectToServKls();
            if(GetPartOfString(0,realklass)==SERV_OBJECT)
                m_kkod = realklass;
            else{
                m_kkod = SERV_OBJECT;
                m_kkod = m_kkod + " " + realklass;
            }
        }
    }
}
//==============================================
//! We join the service classifier
//! language type for top objects
void MTopobject::ConnectToServKls(int typeyaz)
//==============================================
{
    //areal
    if(typeyaz==F_PLO){
        m_pZnak = GetKarta()->m_pSqrZnak;
        m_kkod = SERV_OBJECT_PLO;
    }
    //linear
    else if(typeyaz==F_LIN){
        m_pZnak = GetKarta()->m_pLinZnak;
        m_kkod = SERV_OBJECT_LIN;
    }
    //point
    else if(typeyaz==F_DOT){
        m_pZnak = GetKarta()->m_pDotZnak;
        m_kkod = SERV_OBJECT_DOT;
    }
    //if the type of language isn't defined
    else{
        m_pZnak = GetKarta()->m_pLinZnak;
        m_kkod = SERV_OBJECT_LIN;
    }
}

//======================================================================
//!Returns the closest top of top object to this point
/**The 'nomrebra' parameter accepts minimum of two possible values
//If the next top - the FIRST point of a contour, nomrebra comes back equal-1
*/
double MTopobject::DistanceToVershina(MPoint& gpt,MPoint& resultpt,int& nomrebra)
//ONLY SINGLE-PART top objects
//========================================================================
{
    MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    double rasst = -1000;
    double rasst1;

    if(pta->GetSize()==0) return rasst;


    //we will find the next top
    MPoint pt;
    nomrebra =-1;
    resultpt = *pta->GetAt(0);
    rasst=gpt.Distance(&resultpt);
    for(int i=1;i<pta->GetSize();i++)
    {
        pt = *pta->GetAt(i);
        rasst1 = gpt.Distance(&pt);
        if(rasst1<rasst)
        {
            resultpt=pt;
            nomrebra = i-1;
            rasst = rasst1;
        }
    }
    return rasst;
}

//======================================================================================
//!Returns value of the characteristic across Nome of the name of the characteristic

/**or an empty line if in an array of characteristics of entrance Nome isn't present
*/
MString MTopobject::GetDataPole(unsigned short nom)
//======================================================================================
{
    MString result = "";

    MShInArray* noms = m_pZnak->GetNomsArray();
    //the size of the array of lines from noma of names of fields
    int Size = noms->GetSize();
    int i;
    //we will check, whether the entrance nome lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        if (noms->GetAt(i)[0]==nom){
            break;

        }
    }
    //if the entrance nome lies in the array of admissible codes
    if(i<Size){
        //if in an array of characteristics of top object values of the characteristic lie with
        //entrance Nome, we will receive them
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom, result);
        //values aren't present...
        if(index==-1)
            result = "";
    }
    return result;
}

//======================================================================================
//!Returns value of the characteristic according to the name of the characteristic
/**or an empty line if in an array of characteristics there is no entrance characteristic
*/
MString MTopobject::GetDataPoleByName(MString polename)
//======================================================================================
{
    MString result = "";

    MShInArray* noms = m_pZnak->GetNomsArray();
    //the size of the array of lines from noma of names of fields
    int Size = noms->GetSize();
    int i;
    unsigned short nom;
    //we will check, whether the nome of the entrance name of the characteristic lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        MString str;
        nom = noms->GetAt(i)[0];
        if ((m_pLay->GetKarta()->GetDictAttrName(nom, str))&&(str==polename))
            break;
    }
    //if lies...
    if(i<Size){
        //if in an array of characteristics of top object values of the characteristic lie with
        //the entrance name, we will receive them
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(nom, result);
        //if the individual characteristic isn't present, we look for it among general meanings
        if(index==-1){
            index = m_pZnak->GetGenattrArray()->GetValue(nom, result);
            //isn't present among general meanings..
            if(index==-1)
                result = "";
        }
    }
    return result;
}

//======================================================================================
//!Establishes value of the characteristic on a characteristic code,
/**if this code lies in the array of admissible codes in language
*/
bool MTopobject::SetDataPole(MString data, unsigned short polekod)
//======================================================================================
{
    MShInArray* noms = m_pZnak->GetNomsArray();
    //the size of the array of lines from noma of names of fields
    int Size = noms->GetSize();
    int i;
    //we will check, whether the entrance code of the characteristic lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        if(noms->GetAt(i)[0]==polekod)
            break;
    }
    //if the code isn't present, we can't insert value of the characteristic
    //in an array of individual characteristics
    if(i==Size)
        return false;

    //in an array of individual characteristics of top object on a code we will place value
    MNvArray* pTpoNoms;
    pTpoNoms = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);
    //	int NomSize = pTpoNoms->GetSize();
    int index = pTpoNoms->SetValue(polekod, data);
    //if among individual characteristics there is no line with the polekod code,
    //we will create and will add it to the array
    if(index==-1){
        MNomValue* nomval = new MNomValue(polekod, data);
        ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        delete nomval;
    }
    return true;
}

//======================================================================================
//!Establishes value of the characteristic according to the name of the characteristic,
/**if the code of the entrance characteristic lies in the array of admissible codes in language
*/
bool MTopobject::SetDataPoleByName(MString data, MString polename)
//======================================================================================
{
    if (!m_pZnak) return false;// juli
    MShInArray* noms = m_pZnak->GetNomsArray();
    //the size of the array of lines from noma of names of fields
    int Size = noms->GetSize();
    int i;
    unsigned short nom=0;
    //we will check, whether the nome of the entrance name of the characteristic lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        MString str;
        nom = noms->GetAt(i)[0];
        if ((m_pLay->GetKarta()->GetDictAttrName(nom, str))&&(str==polename))
            break;
    }
    //if the code isn't present, we can't insert value of the characteristic
    //in an array of individual characteristics
    if(i==Size)
        return false;

    //in an array of individual characteristics of top object on the code corresponding
    //to the entrance name of the characteristic, we will place value
    int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->SetValue(nom, data);
    //if among individual characteristics there is no line with the nom code,
    //we will create and will add it to the array
    if(index==-1){
        MNomValue* nomval = new MNomValue(nom, data);
        ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->AddTail(nomval);
        delete nomval;
    }
    return true;
}

//======================================================================================
//!Checks, whether there is in the massif of admissible codes a code corresponding to the entrance characteristic
bool MTopobject::IsPole(MString polename)
//======================================================================================
{
    MShInArray* noms = m_pZnak->GetNomsArray();
    //the size of the array of lines from noma of names of fields
    int Size = noms->GetSize();
    int i;
    unsigned short nom;
    //we will check, whether the nome of the entrance name of the characteristic lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        MString str;
        nom = noms->GetAt(i)[0];
        if ((m_pLay->GetKarta()->GetDictAttrName(nom, str))&&(str==polename))
            break;
    }
    //if the code isn't present...
    if(i==Size)
        return false;
    //otherwise...
    return true;
}

//=========================================================
//!Returns a formula index,
/**at which the name coincides with 'FormulaName'.
//If such formula isn't present, returns -1.
*/
int MTopobject::GetIndexOfFormula(MString FormulaName)
//=========================================================
{
    //now we will find a formula index under the name of "Note"
    MFormArray* formar = m_pZnak->GetYaz()->m_formula;
    MFormula* formula;
    int fcnt = formar->GetSize();
    int i = 0;
    int ind = -1;
    while((i<fcnt)&&(ind==-1))
    {
        formula = formar->GetAt(i);
        if(formula->nazvanie==FormulaName)
        {
            ind = i;
        }
        i++;
    }

    return ind;
} 


//===================================================================
//!Returns the pointer on a horizontal or NULL, if failure
MIzoline*	MTopobject::ConvertToIzoline(MString quadrakod)
//===================================================================
{
    //if it not top object, and the successor, we return NULL,
    //as such converting has to be made for successors
    //or not to be made by specific ways in general

    if(WhatTheObject()!=ID_TOPOBJECT) return NULL;

    //we create result
    MIzoline* pResult = new MIzoline();
    pResult->m_bounds = m_bounds;

    //copy m_dbarray
    pResult->m_dbarray = *(MStArray *)m_dbarray.Clone();

    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    ((MPtArray*)pResult->m_Komplekt->GetAt(pResult->F_TOCHKI))->AppendTail((MPtArray*)pta->Clone());

    pResult->m_pLay = m_pLay;
    //now we will be connected to the classifier
    pResult->ChangeClass(quadrakod);

    pResult->SetizoHeight(atof(*pResult->m_dbarray.GetAt(ID_DAT)));

    return pResult;

}

//===================================================================
//!Returns the pointer on a mark of height or NULL, if failure
MHeightPoint*	MTopobject::ConvertToHeightpt(MString quadrakod)
//===================================================================
{
    //if it not top object, and the successor, we return NULL,
    //as such converting has to be made for successors
    //or not to be made by specific ways in general

    if(WhatTheObject()!=ID_TOPOBJECT) return NULL;
    //converting it is carried out only for dot objects
    if(m_pZnak->GetYaz()->GetType()!=F_DOT) return NULL;

    //we receive coordinates of the only point
    MPoint heightpt = ((MPtArray*)m_Komplekt->GetAt(F_TOCHKI))->GetAt(0)[0];
    //we receive the pointer on a data file
    MNvArray* datar = (MNvArray*)m_Komplekt->GetAt(F_NADPIS);

    MString tempstr;//quad-code object of the classifier given on a function entrance
    MHeightPoint* pResult;
    double height=0.0;//point height

    //we create result
    pResult = new MHeightPoint(heightpt);
    //if it is possible, we will receive the pointer on a classifier sign
    MZnak* pZnak = m_pLay->GetKarta()->m_pKls->GetZnak(quadrakod,tempstr);
    //We receive the pointer on active object of the classifier
    if(pZnak){
        //we receive the pointer on the array of nom of names of data fields
        MShInArray* datnazvar = pZnak->GetNomsArray();
        MString datnazv;
        unsigned short nom=0;
        //we will check, whether lies in the array of admissible codes of number,
        //corresponding to the name of the field "Height"
        for(int i=0; i<datnazvar->GetSize(); i++){
            nom = datnazvar->GetAt(i)[0];
            if((m_pLay->GetKarta()->GetDictAttrName(nom,datnazv))&&(datnazv=="Высота"))
                break;
        }
        if(datnazv=="Высота"){
            MString strheight;
            //we will receive if is, value of height in a data file across Nome of the name of a field
            int index = datar->GetValue(nom,strheight);
            //value of height is...
            if(index!=-1)
                height = atof(strheight);
            else
                height = 0;
        }
    }

    //copy m_dbarray
    pResult->m_dbarray = *(MStArray *)m_dbarray.Clone();
    pResult->m_pLay = m_pLay;
    //now we will be connected to the classifier
    pResult->ChangeClass(quadrakod);

    //in description language among formulas we look for a formula with the name "Height";
    //if such formula isn't present, we write nothing at height mark
    MFormArray* formar = pResult->m_pZnak->GetYaz()->m_formula;
    int colform = formar->GetSize();
    MString formname;
    int i = 0;
    for(i=0; i<colform; i++){
        formname = formar->GetAt(i)->nazvanie;
        if(formname=="Высота") break;
    }
    //if there is a formula with the name "Height", will remove it at distance
    //height mark symbol sizes from a point
    if(formname == "Высота"){
        pResult->SetHeight(height);
        //MSymbol* sym = &((MShriftSrez*)((MSzArray*)((MDotYaz*)pResult->m_pZnak->GetYaz()->m_object)->m_tochka)->GetAt(0))->m_tochka;
        MPoint ptnadpf, ptnadpl;
        //		ptnadpf.x = heightpt.x + GEOM(sym->value)/2;
        ptnadpf.x = heightpt.x;
        ptnadpf.y = heightpt.y;
        //		ptnadpl.x = heightpt.x + GEOM(sym->value);
        ptnadpl.x = heightpt.x;
        ptnadpl.y = heightpt.y;
        pResult->AddVektorPriv(i,ptnadpf,ptnadpl);
    }

    return pResult;
}


//===================================================================
//!Returns the pointer on top object or NULL, if failure
MTopobject*	MTopobject::ConvertToTopobject(MString /*quadrakod*/)
//===================================================================
{
    //it is impossible to convert top object in top object in that sense,
    //for what this function is started
    return NULL;
}

//=======================================================================================================================
//!Drawing of top object as simple contour without text
void MTopobject::Draw1(MPlotDevice* pDevice)
//=======================================================================================================================
{
    MYaz* pYaz=m_pZnak->GetYaz();
    //we get access to arrays of points
    int i=m_Komplekt->GetSize();
    //we draw all arrays of points separately
    MPtArray* pta =NULL;

    //we prepare a pen

    M_LOG_PEN drawpen;

    int RR, GG, BB;
    SdvigRGBbyHLS(	m_pLay->m_commoncolor_pen.R,m_pLay->m_commoncolor_pen.G,m_pLay->m_commoncolor_pen.B,
                    m_pLay->GetKarta()->m_DH,m_pLay->GetKarta()->m_DL,m_pLay->GetKarta()->m_DS,
                    RR,GG,BB);

    MColor penColor(RR, GG, BB);
    penColor.SetA(m_pLay->m_commoncolor_penTransparent);

    drawpen.lopnColor	= penColor;
    drawpen.lopnStyle	= PS_SOLID;
    drawpen.lopnWidth.x = pDevice->LogicToBum(m_pLay->m_commonpenwidth);
    drawpen.lopnWidth.y = drawpen.lopnWidth.x;

    pDevice->SavePencil();
    pDevice->SetPencil(&drawpen);

    SdvigRGBbyHLS(	m_pLay->m_commoncolor_brush.R,m_pLay->m_commoncolor_brush.G,m_pLay->m_commoncolor_brush.B,
                    m_pLay->GetKarta()->m_DH,m_pLay->GetKarta()->m_DL,m_pLay->GetKarta()->m_DS,
                    RR,GG,BB);

    MColor color(RR, GG, BB);
    color.SetA(m_pLay->m_commoncolor_brushTransparent);
    pDevice->SetZapolnenie(&color);


    for(int j1=F_TOCHKI;j1<i;j1++)
    {
        pta=(MPtArray*)m_Komplekt->GetAt(j1);
        //depending on type of object we cause the corresponding functions of drawing
        if(pta->GetSize() > 0)
            switch(pYaz->GetType())
            {
            case F_PLO:
                //				pDevice->Polyline(pta->GetHead(),pta->GetSize());   // vipa 2004.10.18
                pDevice->Polygon(pta->GetHead(),pta->GetSize());
                break;
            case F_LIN:
                pDevice->Polyline(pta->GetHead(),pta->GetSize());
                break;
            case F_DOT:
                if ( (m_pLay->m_pKarta->m_DrawFilterSizeGeo) < m_pLay->m_commonpenwidth*120.0)     // vipa 2004.10.04 to exclude invisible dots
                {                                          //  and to scale dots  ?! GeoCOO needs revise
                    if(m_pLay->GetKarta()->m_pObjectCoo->WhatTheCoo() == COO_GEODESIC)
                    {
                        pDevice->Circle(pta->GetHead(), pDevice->LogicToBum(1.5), m_pLay->m_commoncolor_pen);
                    }
                    else
                    {
                        pDevice->Circle(pta->GetHead(), pDevice->LogicToBum(1.5 + m_pLay->m_commonpenwidth*120.0/(m_pLay->m_pKarta->m_DrawFilterSizeGeo)), m_pLay->m_commoncolor_pen);
                    }
                }
                break;
            }
        pDevice->EndClipping();// does to empty 'clipRect'
    }

    //restore
    pDevice->RestoreZapolnenie();
    pDevice->RestorePencil();

    //we output the text
    MString str = "";

    if(((MFormArray*)pYaz->m_formula)->GetSize()==0) return;

    //we receive a formula
    MFormula* formula = ((MFormArray*)pYaz->m_formula)->GetAt(0);

    //quantity of fragments in a formula
    int Size = ((MFrArray*)formula->fragm_arr)->GetSize();

    for (i=0; i<Size; i++)
    {
        //we receive i-th a fragment
        MFragment* fragment = ((MFrArray*)formula->fragm_arr)->GetAt(i);

        for (int j=0; j<fragment->GetStrofNum(); j++)
        {
            MStrofa* strofa =NULL;
            //"stanza" = one stanza, "fraction" = two stanzas
            if (fragment->GetStrofNum() == 1)
                strofa = (MStrofa*)fragment->GetFragment();
            if (fragment->GetStrofNum() == 2)
            {
                //we receive pointers on numerator,
                //and then - on a denominator
                if (j == 0) strofa = ((MFraction*)fragment->GetFragment())->GetChisl();
                if (j == 1) strofa = ((MFraction*)fragment->GetFragment())->GetZnam();
            }
            //we declare the new array of lines
            //also we copy in it lines
            MStArray* strofy = new MStArray();
            for(int ii=0; ii<strofa->GetPredlogenieNum(); ii++)
            {
                strofy->SetSize(0);
                //				MString strofa_name;
                //we will receive an index (code) of the name of the characteristic of the offer with an index i
                //in an array of offers of a stanza
                unsigned short nompolename = strofa->GetNomPole(ii);
                ObtainMassivStrok(nompolename, strofy);
                int kolvo = strofy->GetSize();
                for (int jj=0; jj<kolvo; jj++)
                {
                    str += *strofy->GetAt(jj) + " ";
                }
            }
        }
    }

    //drawing of an inscription
    if(m_pLay->m_commontextwidth < 4) return;

    SdvigRGBbyHLS(	m_pLay->m_commoncolor_text.R,m_pLay->m_commoncolor_text.G,m_pLay->m_commoncolor_text.B,
                    m_pLay->GetKarta()->m_DH,m_pLay->GetKarta()->m_DL,m_pLay->GetKarta()->m_DS,
                    RR,GG,BB);

    drawpen.lopnColor	= MColor(RR, GG, BB);
    drawpen.lopnStyle	= PS_SOLID;
    pDevice->SavePencil();
    pDevice->SetPencil(&drawpen);

    M_LOG_FONT drawfont;
    //drawfont.lfFaceName = "Arial";
    strcpy((char*)&(drawfont.lfFaceName), "Arial");
    drawfont.lfHeight = textRatio * m_pLay->m_commontextwidth;

    pDevice->SaveFont();
    pDevice->SetFont(&drawfont);

    if(m_pLay->m_bOperLayer)
        switch(pYaz->GetType())
        {
        case F_PLO:
            RecalcBounds();
            pDevice->TextOut(m_bounds.CenterX(), m_bounds.CenterY(), str, str.GetLength());
            break;
        case F_LIN:
            pDevice->TextOut(pta->GetTail()->x, pta->GetTail()->y, str, str.GetLength());
            break;
        case F_DOT:
            pDevice->TextOut(pta->GetHead()->x, pta->GetHead()->y, str, str.GetLength());
            break;
        }
    else
        switch(pYaz->GetType())
        {
        case F_PLO:
            RecalcBounds();
            pDevice->TextOutWindows(m_bounds.CenterX(), m_bounds.CenterY(), str, str.GetLength());
            break;
        case F_LIN:
            if(pta->GetTail() != NULL)
                pDevice->TextOutWindows(pta->GetTail()->x, pta->GetTail()->y, str, str.GetLength());
            break;
        case F_DOT:
            if(pta->GetHead() != NULL)
                pDevice->TextOutWindows(pta->GetHead()->x, pta->GetHead()->y, str, str.GetLength());
            break;
        }

    str = "";

    pDevice->RestoreFont();
    pDevice->RestorePencil();

};

void MTopobject::DrawLine(MYaz* pYaz,MPlotDevice* pDevice,MPtArray* pta)
{
    MRect clipRect = MRect(zoomRectangle.left , zoomRectangle.bottom , zoomRectangle.right ,  zoomRectangle.top );
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);

    QList<MPtArray*> output;
    ClipPolyline(pta, output, clipRect*=1.1);

    for (QList<MPtArray*>::Iterator it = output.begin(); it != output.end(); it++)
        if ((*it)->GetSize())
            MDrawing::DrawLine(this, pYaz, pDevice, *it, razryvy);

    while (output.count())
    {
        MPtArray *first = output.first();
        output.removeFirst();
        delete first;
    }
}

void	MTopobject::DrawZapolnenie(MYaz* pYaz, MPlotDevice* pDevice,MPtArray* pta)
{
    MRect clipRect = MRect(zoomRectangle.left , zoomRectangle.bottom , zoomRectangle.right ,  zoomRectangle.top );

    MPtArray *ptPolly = new MPtArray();
    ClipPolygon(pta, ptPolly, clipRect*=1.1);
    if (ptPolly->GetSize())
        MDrawing::DrawZapolnenie(this,pYaz,pDevice,ptPolly);
    delete ptPolly;
}

//=======================================================================================================================
//!drawing
void MTopobject::Draw(MPlotDevice* pDevice)
//=======================================================================================================================
{
    actPolygonDense=this->GetTransType();  //added by back
    if (!m_pZnak)return;
    if (!m_pZnak->GetYaz()) return;
    //if the flag of drawing is set in 'false', we don't draw
    if(!m_pZnak->GetYaz()->m_visible)return;
    //we get access to arrays of points
    int i=m_Komplekt->GetSize();
    //we draw all arrays of points separately
    MPtArray* pta = nullptr;
    MYaz* pYaz=m_pZnak->GetYaz();

    //we order the array of gaps, if zero language of the description in a top object sign not dot
    MRzArray* razryvy = NULL;
    if(pYaz->GetType()!=F_DOT){
        OrderRazryvy();
        razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    }

    // MRect rectangle = GetObhvatRect(pta);
    for(int j1=F_TOCHKI;j1<i;j1++)
    {
        //MArray *arr = m_Komplekt->GetAt(j1);
        MString nWhatTheArray = m_Komplekt->GetAt(j1)->WhatTheArray();
        if(nWhatTheArray=="PHL")
        {
            pDevice->SetClipRegionFlag( true);
            break;
        }
    }
    for(int j1=F_TOCHKI;j1<i;j1++)
    {
        pta=(MPtArray*)m_Komplekt->GetAt(j1);
        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        //depending on type of object we cause the corresponding functions of drawing
        switch(pYaz->GetType())
        {
        case F_PLO:
        {
            if(j1==F_TOCHKI && pDevice->GetClipRegionFlag())
            {
                MDrawing::DrawZapolnenie(this,pYaz,pDevice,pta);
            }
            if (!pDevice->GetClipRegionFlag())
            {
                MDrawing::DrawZapolnenie(this,pYaz,pDevice,pta);
            }
            if(this->m_Komplekt->GetAt(j1)->WhatTheArray() !="PHL")
            {
                MDrawing::DrawLine(this,pYaz,pDevice,pta,razryvy);
            }


            //MDrawing::DrawZapolnenie(this,pYaz,pDevice,pta);
            // MDrawing::DrawLine(this,pYaz,pDevice,pta,razryvy);
            //            if(pDevice->GetClipRegionFlag())
            //                return;
            //            else
            break;
        }
        case F_LIN:     //  vipa 2004.12.09
            //    MDrawing::DrawLine(this,pYaz,pDevice, pta, razryvy);
            if (isRectInRect(m_bounds,zoomRectangle))
                MDrawing::DrawLine(this,pYaz,pDevice, pta, razryvy);
            else
                if (isPolylineIntersectsRect(pta, zoomRectangle))
                    MDrawing::DrawLine(this,pYaz,pDevice, pta, razryvy);
            break;
        case F_DOT:
            MDrawing::DrawPoint(this,pYaz,pDevice,pta);
            break;
        }
    }
    pDevice->SetClipRegionFlag( false);
};

void MTopobject::Draw2(MPlotDevice* pDevice, MYaz* yaz)
//=================================================================================================
{
    actPolygonDense=this->GetTransType();  //added by back
    if (!m_pZnak)return;
    if (!m_pZnak->GetYaz()) return;
    //if the flag of drawing is set in 'false', we don't draw
    if(!m_pZnak->GetYaz()->m_visible)return;
    //we get access to arrays of points
    int i=m_Komplekt->GetSize();
    //we draw all arrays of points separately
    MPtArray* pta;
    MYaz* pYaz=/*m_pZnak->GetYaz()*/yaz;

    //we order the array of gaps, if zero language of the description in a top object sign not dot
    MRzArray* razryvy = NULL;
    if(pYaz->GetType()!=F_DOT){
        OrderRazryvy();
        razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    }

    // MRect rectangle = GetObhvatRect(pta);
    for(int j1=F_TOCHKI;j1<i;j1++)
    {
        //		MArray *arr = m_Komplekt->GetAt(j1);
        MString nWhatTheArray = m_Komplekt->GetAt(j1)->WhatTheArray();
        if(nWhatTheArray=="PHL")
        {
            pDevice->SetClipRegionFlag( true);
            break;
        }
    }
    for(int j1=F_TOCHKI;j1<i;j1++)
    {
        pta=(MPtArray*)m_Komplekt->GetAt(j1);
        ConvertSizeToReal = (double(*)(MTopobject* pObj,float fVal))&SizeToReal;

        //depending on type of object we cause the corresponding functions of drawing
        switch(pYaz->GetType())
        {
        case F_PLO:
        {
            MDrawing::DrawZapolnenie(this,pYaz,pDevice,pta);
            MDrawing::DrawLine(this,pYaz,pDevice,pta,razryvy);

            if(pDevice->GetClipRegionFlag())
                return;
            else
                break;
        }
        case F_LIN:     //  vipa 2004.12.09
            //    MDrawing::DrawLine(this,pYaz,pDevice, pta, razryvy);
            if (isRectInRect(m_bounds,zoomRectangle))
                MDrawing::DrawLine(this,pYaz,pDevice, pta, razryvy);
            else
                if (isPolylineIntersectsRect(pta, zoomRectangle))
                    MDrawing::DrawLine(this,pYaz,pDevice, pta, razryvy);
            break;
        case F_DOT:
            MDrawing::DrawPoint(this,pYaz,pDevice,pta);
            break;
        }
    }
};

//====================================================
//!Gives a prompt reply to a question:
//! Whether one formula will be drawn at least?
/**This function needs to be caused if at you is
//it is a lot of identical objects (10000 and more) for this purpose,
//not to cause without results one thousand times the 'DrawText' function.
*/
bool MTopobject::CanDrawText(MPlotDevice* pDevice)
//====================================================
{
    MKarta* pKarta = GetKarta();
    //we will receive the pointer on the array of formulas
    MFormArray* farr = m_pZnak->GetYaz()->m_formula;
    int fcnt = farr->GetSize();

    //if the array of formulas is empty, anything and won't be drawn
    if(fcnt == 0) return false;


    //now in a cycle on all formulas we will check all stanzas
    MFormula* pf;
    MFragment* fr;
    MStrofa* strofa;
    MFraction* fraction;
    MPrk prk;
    unsigned char  prknum;
    int pkcnt;
    int k;
    int frcnt;
    int j;
    for(int i=0; i<fcnt; i++)
    {
        pf = farr->GetAt(i);
        //we will walk by all fragments
        frcnt = pf->fragm_arr->GetSize();
        for(j=0; i<frcnt; i++)
        {
            fr = pf->fragm_arr->GetAt(j);
            //depending on fragment type,
            //we will reach each stanza

            //If at least one offer of a stanza
            //has the text size in logical coordinates
            //it is more than one pixel,
            //that function returns true,
            //since in this case though something will be drawn.
            switch(fr->GetType())
            {
            case ID_FRAG_STROFA:
            {
                unsigned char  prknum;
                strofa = (MStrofa*)fr->GetFragment();
                pkcnt = strofa->GetPredlogenieNum();
                for(k=0; k<pkcnt; k++)
                {
                    prknum = strofa->GetNomPrk(k);
                    pKarta->GetDictPrkValue(prknum ,prk);
                    if(pDevice->BumToLogic(GEOM(prk.value))>1)
                    {
                        return true;
                    }
                }
            }
                break;
            case ID_FRAG_FRACTION:
            {
                fraction = (MFraction*)fr->GetFragment();
                strofa = fraction->GetChisl();
                pkcnt = strofa->GetPredlogenieNum();
                for(k=0; k<pkcnt; k++)
                {
                    prknum = strofa->GetNomPrk(k);
                    pKarta->GetDictPrkValue(prknum ,prk);
                    if(pDevice->BumToLogic(GEOM(prk.value))>1)
                    {
                        return true;
                    }
                }

                strofa = fraction->GetZnam();
                pkcnt = strofa->GetPredlogenieNum();
                for(k=0; k<pkcnt; k++)
                {
                    prknum = strofa->GetNomPrk(k);
                    pKarta->GetDictPrkValue(prknum ,prk);
                    if(pDevice->BumToLogic(GEOM(prk.value))>1)
                    {
                        return true;
                    }
                }
            }
                break;
            }
        }
    }

    return false;
}

//==================================
//! drawing of a marking of top object
void MTopobject::DrawTracker(MPlotDevice* pDevice,TrackerState state)
//==================================
{
    double d1Pix = pDevice->LogicToBum(1);
    double d2Pix = d1Pix*2;
    double d3Pix = d1Pix*3;
    double d5Pix = d1Pix*5;
    double d7Pix = d1Pix*7;

    for(int vv=m_Komplekt->GetSize()-1;vv>=F_TOCHKI;vv--)
    {
        MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(vv);

        pDevice->SavePencil();
        switch (state)
        {
        // usual tracker
        default:
        case normal:
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

            //we draw spaces only for top objects and his not dot successors
            if(m_pZnak->GetYaz()->GetType()!=F_DOT){
                //if the top object has spaces, will show it on a tracker
                MRzArray* rzar = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
                MPtArray* ptar = new MPtArray();
                for(int i=0; i<rzar->GetSize(); i++){
                    MRazryv* razryv = rzar->GetAt(i);
                    ptar->ClearAll();
                    //if in a space there is no transition through zero
                    if((razryv->m_indlast>razryv->m_indfirst+1)||((razryv->m_indlast==razryv->m_indfirst+1)&&
                                                                  (DLINASG(*pta->GetAt(razryv->m_indfirst),razryv->m_ptfirst)<=
                                                                   DLINASG(*pta->GetAt(razryv->m_indfirst),razryv->m_ptlast)))){
                        ptar->AddTail(&razryv->m_ptfirst);
                        for(int j=razryv->m_indfirst+1; j<razryv->m_indlast; j++)
                            ptar->AddTail(pta->GetAt(j));
                        ptar->AddTail(&razryv->m_ptlast);
                        pDevice->DrawServantPolyLine(ptar,3,MColor(0xff,0,0));
                    }
                    //if there is a transition through zero
                    else{
                        ptar->AddTail(&razryv->m_ptfirst);
                        int j = 0;
                        for(j=razryv->m_indfirst+1; j<pta->GetSize(); j++)
                            ptar->AddTail(pta->GetAt(j));
                        for(j=0; j<razryv->m_indlast; j++)
                            ptar->AddTail(pta->GetAt(j));
                        ptar->AddTail(&razryv->m_ptlast);
                        pDevice->DrawServantPolyLine(ptar,3,MColor(0xff,0,0));
                    }
                }
                delete ptar;
            }
            // points...
            for (int i = 0; i < pta->GetSize(); i++) {
                pDevice->PatBlt(pta->GetAt(i)->x - d2Pix, pta->GetAt(i)->y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                pDevice->PatBlt(pta->GetAt(i)->x - d1Pix, pta->GetAt(i)->y - d1Pix, d3Pix, d3Pix, WHITENESS);

            }
        }
            break;

            // tracker "only points"
        case points:
        {
            // points...
            for (int i = 0; i < pta->GetSize(); i++) {
                pDevice->PatBlt(pta->GetAt(i)->x - d2Pix, pta->GetAt(i)->y - d2Pix, d5Pix, d5Pix, BLACKNESS);
                pDevice->PatBlt(pta->GetAt(i)->x - d1Pix, pta->GetAt(i)->y - d1Pix, d3Pix, d3Pix, WHITENESS);
            }
        }
            break;

            // tracker "one line"
        case line:
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
        }
            break;

            // tracker "red line"
        case redline:
        {
            // we declare a pen 1
            M_LOG_PEN lpen1(PS_SOLID,MPoint(d1Pix),MColor(0xff,0xff,0));
            // we declare a pen 2
            M_LOG_PEN lpen2(PS_SOLID,MPoint(d3Pix),MColor(0xff,0,0));


            pDevice->SetPencil(&lpen2);
            pDevice->Polyline(pta->GetHead(), pta->GetSize());

            // riding broken line
            pDevice->SetPencil(&lpen1);
            pDevice->Polyline(pta->GetHead(), pta->GetSize());

        }
            break;
        }
        pDevice->RestorePencil();
    }
    //we draw a marker for splines-bindings of formulas
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
            //points: transformation to logical coordinates
            for(int j = 0;j<pta->GetSize();j++){
                pDevice->PatBlt(pta->GetAt(j)->x - d3Pix, pta->GetAt(j)->y - d3Pix, d7Pix, d7Pix, BLACKNESS);
                pDevice->PatBlt(pta->GetAt(j)->x - d2Pix, pta->GetAt(j)->y - d2Pix, d5Pix, d5Pix, WHITENESS);
            }
            delete trackpts;
        }
        delete pta;
    }

}

//=========================================================
//!Orders the array of gaps on the first index in ascending order in the array
//! and on distance from the beginning of an edge for the spaces lying on one edge
bool MTopobject::OrderRazryvy()
//=========================================================
{
    //we get access to the array of points
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if (!pta) return false;
    //we get access to the array of spaces
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    if (!razryvy) return false;

    int n = razryvy->GetSize();//number of spaces
    //if the array is empty or in it one space, it is necessary to order nothing
    if(n<=1) return true;

    MRzArray* rzar = new MRzArray();
    rzar->AppendTail(razryvy);
    MRzArray* rzarnew = new MRzArray();
    int ind;

    while(rzar->GetSize()!=0){
        MRazryv* rzmin = new MRazryv(rzar->GetAt(0));
        ind = 0;
        for(int i=1; i<n; i++){
            MRazryv* rz = rzar->GetAt(i);
            if(rzmin->m_indfirst>rz->m_indfirst){
                rzmin->Copy(rz) ;
                ind = i;
            }
            //streamlining of spaces with an identical first index
            else if(rzmin->m_indfirst==rz->m_indfirst){
                if(DLINASG(rzmin->m_ptfirst,pta->GetAt(rzmin->m_indfirst)[0])>
                        DLINASG(rz->m_ptfirst,pta->GetAt(rz->m_indfirst)[0])){
                    rzmin->Copy(rz) ;
                    ind = i;

                }
            }

        }
        rzarnew->AddTail(rzmin);
        rzar->Delete(ind);
        delete rzmin;
        n--;
    }

    //we clear the array and we insert the ordered
    razryvy->ClearAll();
    razryvy->AppendTail(rzarnew);

    delete rzar;
    delete rzarnew;

    return true;
}

//=========================================================
//!Each gap passing through zero divides into two gaps:
//! a gap until the end of a contour and a gap from the beginning of a contour until the end of the divided gap
bool MTopobject::DevideOverlastptRazryvy()
//=========================================================
{
    //we get access to the array of points
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    //we get access to the array of spaces
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);

    for(int i=0; i<razryvy->GetSize(); i++){
        MRazryv* razryv = razryvy->GetAt(i);
        //if there is a transition through zero
        if((razryv->m_indlast<razryv->m_indfirst)||((razryv->m_indlast==razryv->m_indfirst)&&
                                                    (DLINASG(GetPoint(razryv->m_indfirst),razryv->m_ptfirst)>

                                                     DLINASG(GetPoint(razryv->m_indfirst),razryv->m_ptlast)))){
            MRazryv* razryvfirst = new MRazryv();
            razryvfirst->m_indfirst = razryv->m_indfirst;
            razryvfirst->m_ptfirst = razryv->m_ptfirst;
            razryvfirst->m_indlast = pta->GetSize() - 1;
            razryvfirst->m_ptlast = *pta->GetAt(pta->GetSize()-1);
            MRazryv* razryvlast = new MRazryv();
            razryvlast->m_indfirst = 0;
            razryvlast->m_ptfirst = pta->GetAt(0);
            razryvlast->m_indlast = razryv->m_indlast;
            razryvlast->m_ptlast = razryv->m_ptlast;

            razryvy->Delete(i);
            razryvy->AddTail(razryvfirst);
            razryvy->AddTail(razryvlast);
            i--;

            delete razryvfirst;
            delete razryvlast;
        }
    }

    //we will order all gaps
    OrderRazryvy();

    return true;
}

//=========================================================
//!Unites all blocked gaps
bool MTopobject::UnionOverlappedRazryvy()
//=========================================================
{
    //we get access to the array of points
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    //we get access to the array of spaces
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);

    int n = razryvy->GetSize();//number of spaces
    //if the array is empty or in it one space, it is necessary to order nothing
    if(n<=1) return true;

    //if there are gaps with transition through zero, will break them into two gaps to zero
    //and after zero, also we will order the received array
    DevideOverlastptRazryvy();

    for(int i=0; i<razryvy->GetSize()-1; i++){
        MRazryv* razryv = razryvy->GetAt(i);
        MRazryv* rz = razryvy->GetAt(i+1);
        //if the beginning of the following gap lies on the previous gap
        if((rz->m_indfirst<razryv->m_indlast-1)||((rz->m_indfirst==razryv->m_indlast)&&(rz->m_ptfirst==razryv->m_ptlast))||
                ((rz->m_indfirst==razryv->m_indlast-1)&&(DLINASG(*pta->GetAt(razryv->m_indlast-1), rz->m_ptfirst)<=DLINASG(*pta->GetAt(razryv->m_indlast-1), razryv->m_ptlast)))){
            //if the following gap lies on the previous gap, we will remove the following gap
            if((rz->m_indlast<razryv->m_indlast)||((rz->m_indlast==razryv->m_indlast)&&
                                                   (DLINASG(*pta->GetAt(razryv->m_indlast-1), rz->m_ptlast)<=DLINASG(*pta->GetAt(razryv->m_indlast-1), razryv->m_ptlast)))){
                razryvy->Delete(i+1);
                i--;
            }
            //if the end of the following gap lies behind the previous gap, we will unite
            //two gaps
            else if((rz->m_indlast>=razryv->m_indlast)||((rz->m_indlast==razryv->m_indlast)&&
                                                         (DLINASG(*pta->GetAt(razryv->m_indlast-1), rz->m_ptlast)>DLINASG(*pta->GetAt(razryv->m_indlast-1), razryv->m_ptlast)))){
                razryv->m_indlast = rz->m_indlast;
                razryv->m_ptlast = rz->m_ptlast;
                razryvy->Delete(i+1);
                i--;
            }
        }
    }

    if(razryvy->GetSize()<=1)
        return true;

    //if final point of the last gap and first point of an initial gap
    //coincide, we will unite these gaps in one with transition through zero
    MRazryv* razryvlast = razryvy->GetAt(razryvy->GetSize()-1);
    MRazryv* razryvfirst = razryvy->GetAt(0);
    if(razryvfirst->m_ptfirst == razryvlast->m_ptlast){
        MRazryv* newrazryv = new MRazryv();
        newrazryv->m_indfirst = razryvlast->m_indfirst;
        newrazryv->m_ptfirst = razryvlast->m_ptfirst;
        newrazryv->m_indlast = razryvfirst->m_indlast;
        newrazryv->m_ptlast = razryvfirst->m_ptlast;
        razryvy->DeleteHead();
        razryvy->DeleteTail();
        razryvy->AddTail(newrazryv);
        delete newrazryv;
    }

    return true;
}

//==================================
//!for a gap forms the broken line corresponding to it
bool MTopobject::ConstructLomlineByRazryv(MRazryv* razryv, MPtArray* pLomline)
//==================================
{
    if((!pLomline)||(!razryv))
        return false;
    pLomline->ClearAll();

    //if there is no transition through zero
    if((razryv->m_indlast>razryv->m_indfirst+1)||((razryv->m_indlast==razryv->m_indfirst+1)&&
                                                  (DLINASG(GetPoint(razryv->m_indfirst),razryv->m_ptfirst)<=
                                                   DLINASG(GetPoint(razryv->m_indfirst),razryv->m_ptlast)))){
        pLomline->AddTail(&razryv->m_ptfirst);
        for(int j=razryv->m_indfirst+1; j<razryv->m_indlast; j++) {
            MPoint point=  GetPoint(j);
            pLomline->AddTail(&point);
        }
        pLomline->AddTail(&razryv->m_ptlast);
    }
    //there is a transition through zero
    else{
        pLomline->AddTail(&razryv->m_ptfirst);
        int j = 0;
        for(j=razryv->m_indfirst+1; j<GetPointsNumber(); j++)  {
            MPoint point=  GetPoint(j);
            pLomline->AddTail(&point);
        }
        for(j=0; j<razryv->m_indlast; j++)   {
            MPoint point=  GetPoint(j);
            pLomline->AddTail(&point);
        }
        pLomline->AddTail(&razryv->m_ptlast);
    }

    return true;
}


//=========================================
//!corrects the array of gaps at addition in a top object contour
//! a new point of point before a point with the 'index' index;
bool MTopobject::CorrectRazryvsByInsertBeforePoint(int index, MPoint point)
//=========================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if (!((index>=0)&&(index<ptar->GetSize())))return false;

    //if among gaps there was a gap with transition through zero, will divide it into two gaps:
    //a gap from a zero point of a contour and a gap to the last point of a contour
    DevideOverlastptRazryvy();
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    if(razryvy->GetSize()==0)
        return true;
    MRazryv* razryv = razryvy->GetAt(razryvy->GetSize()-1);
    //if the point is inserted after the end of the last gap, indexes and points began
    //and the end of all gaps don't change
    if(index>razryv->m_indlast)
        return true;
    //on all gaps we make changes from addition of a new point
    for(int i=0; i<razryvy->GetSize(); i++){
        razryv = razryvy->GetAt(i);
        //if the point is inserted prior to the beginning of a gap, beginning indexes
        //and the end of a gap increase by unit
        if(index<=razryv->m_indfirst){
            razryv->m_indfirst++;
            razryv->m_indlast++;
        }
        //we insert a point in the middle of a gap
        else if((index>razryv->m_indfirst)&&(index<=razryv->m_indlast)){
            if((index!=razryv->m_indfirst+1)&&(index!=razryv->m_indlast))
                razryv->m_indlast++;
            //we insert a point on an edge on which the initial point of a gap lies
            else if((index==razryv->m_indfirst+1)&&(index!=razryv->m_indlast)){
                //we will calculate a new point of the beginning of a gap, keeping a proportion between parts
                //a segment on which the beginning of a gap lies; the index of the beginning of a gap won't be changed
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), point, dlina);
                razryv->m_indlast++;
            }
            //we insert a point on an edge on which the final point of a gap lies
            else if((index!=razryv->m_indfirst+1)&&(index==razryv->m_indlast)){
                //we will calculate a new point of the end of a gap, keeping a proportion between parts
                //a segment on which the end of a gap lies; the index of the end of a gap won't be changed
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index-1), point, dlina);
            }
            //we insert a point on an edge on which initial and final points of a gap lie
            else if((index==razryv->m_indfirst+1)&&(index==razryv->m_indlast)){
                //we will calculate new points of the end and beginning of a gap, keeping proportions between


                //parts of a segment on which the beginning and the end of a gap lie; end indexes
                //and the beginnings of a gap won't be changed
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), point, dlina);
                dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index-1), point, dlina);
            }
        }
    }
    UnionOverlappedRazryvy();

    return true;
}

//=========================================
//!corrects the array of gaps at addition in a top object contour
//! a new point of point after a point with the 'index' index;
bool MTopobject::CorrectRazryvsByInsertAfterPoint(int index, MPoint point)
//=========================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if (!((index>=0)&&(index<ptar->GetSize())))return false;


    //if among gaps there was a gap with transition through zero, will divide it into two gaps:
    //a gap from a zero point of a contour and a gap to the last point of a contour
    DevideOverlastptRazryvy();
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    if(razryvy->GetSize()==0)
        return true;
    MRazryv* razryv = razryvy->GetAt(razryvy->GetSize()-1);
    //if the point is inserted after the end of the last gap, indexes and points began
    //and the end of all gaps don't change

    if(index>=razryv->m_indlast)
        return true;
    //on all gaps we make changes from addition of a new point
    for(int i=0; i<razryvy->GetSize(); i++){
        razryv = razryvy->GetAt(i);
        //if the point is inserted prior to the beginning of a gap, beginning indexes
        //and the end of a gap increase by unit
        if(index<razryv->m_indfirst){
            razryv->m_indfirst++;
            razryv->m_indlast++;
        }
        //we insert a point in the middle of a gap
        else if((index>=razryv->m_indfirst)&&(index<razryv->m_indlast)){
            if((index!=razryv->m_indfirst)&&(index!=razryv->m_indlast-1))
                razryv->m_indlast++;
            //we insert a point on an edge on which the initial point of a gap lies
            else if((index==razryv->m_indfirst)&&(index!=razryv->m_indlast-1)){
                //we will calculate a new point of the beginning of a gap, keeping a proportion between parts
                //a segment on which the beginning of a gap lies; the index of the beginning of a gap won't be changed
                double dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(*ptar->GetAt(index),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index), point, dlina);
                razryv->m_indlast++;
            }
            //we insert a point on an edge on which the final point of a gap lies
            else if((index!=razryv->m_indfirst)&&(index==razryv->m_indlast-1)){
                //we will calculate a new point of the end of a gap, keeping a proportion between parts

                //a segment on which the end of a gap lies; the index of the end of a gap won't be changed
                double dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(*ptar->GetAt(index),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index), point, dlina);
            }
            //we insert a point on an edge on which initial and final points of a gap lie
            else if((index==razryv->m_indfirst)&&(index==razryv->m_indlast-1)){
                //we will calculate new points of the end and beginning of a gap, keeping proportions between
                //parts of a segment on which the beginning and the end of a gap lie; end indexes
                //and the beginnings of a gap won't be changed
                double dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(*ptar->GetAt(index),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index), point, dlina);
                dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(*ptar->GetAt(index),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index), point, dlina);
            }
        }
    }
    UnionOverlappedRazryvy();

    return true;
}

//=========================================
//!corrects the array of gaps during removal from a top object contour
//! points with the 'index' index;
bool MTopobject::CorrectRazryvsByDeletePoint(int index)
//=========================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if (!((index>=0)&&(index<ptar->GetSize())))return false;

    //if among gaps there was a gap with transition through zero, will divide it into two gaps:
    //a gap from a zero point of a contour and a gap to the last point of a contour
    DevideOverlastptRazryvy();
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    if(razryvy->GetSize()==0)
        return true;
    MRazryv* razryv = razryvy->GetAt(razryvy->GetSize()-1);
    //if the point is removed after the end of the last gap, indexes and points began
    //and the end of all gaps don't change
    if(index>razryv->m_indlast)
        return true;
    //on all gaps we make changes from removal of a point
    for(int i=0; i<razryvy->GetSize(); i++){
        razryv = razryvy->GetAt(i);
        //if the point is removed prior to the beginning of a gap, beginning indexes
        //and the end of a gap we decrease by unit
        if(index<razryv->m_indfirst){
            razryv->m_indfirst--;
            razryv->m_indlast--;
        }
        //we delete a point in the middle of a gap
        else if((index>=razryv->m_indfirst)&&(index<=razryv->m_indlast)){
            //the deleted point doesn't belong to edges on which lie the beginning
            //and end of a gap
            if((index>razryv->m_indfirst+1)&&(index<razryv->m_indlast-1))
                razryv->m_indlast--;
            //the index of the deleted point is equal to an index of the beginning of a gap
            else if((index==razryv->m_indfirst)&&(index<razryv->m_indlast-1)){
                //if we delete a zero point, we believe the beginning of a gap coincides
                //with the new beginning of a contour
                if(index==0){
                    razryv->m_ptfirst = *ptar->GetAt(index+1);
                    razryv->m_indlast--;
                }
                else{
                    //we will calculate a new point of the beginning of a gap, keeping a proportion between parts
                    //of the segment on which the beginning of a gap lies;
                    double dlina = DLINASG(*ptar->GetAt(index+1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index))*
                            DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index-1));
                    razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index+1), *ptar->GetAt(index-1), dlina);
                    razryv->m_indfirst--;
                    razryv->m_indlast--;
                }
            }
            //the index of the deleted point is equal to an index of the end of an edge, on which
            //the beginning of a gap lies
            else if((index==razryv->m_indfirst+1)&&(index<razryv->m_indlast-1)){
                //we will calculate a new point of the beginning of a gap, keeping a proportion between parts
                //of the segment on which the beginning of a gap lies;
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index+1));
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), *ptar->GetAt(index+1), dlina);
                razryv->m_indlast--;
            }
            //the index of the deleted point is equal to an index of the end of a gap
            else if((index>razryv->m_indfirst+1)&&(index==razryv->m_indlast)){
                //if we delete the last point, we believe the end of a gap coincides
                //with the new end of a contour
                if(index==ptar->GetSize()-1){
                    razryv->m_ptlast = *ptar->GetAt(index-1);
                    razryv->m_indlast = index-1;
                }
                else{
                    //we will calculate a new point of the end of a gap, keeping a proportion between parts
                    //of the segment on which the end of a gap lies;
                    double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                            DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index+1));
                    razryv->m_ptlast = ExtendVector(*ptar->GetAt(index-1), *ptar->GetAt(index+1), dlina);
                }
            }
            //the index of the deleted point is equal to an index of the beginning of an edge, on which
            //the end of a gap lies
            else if((index>razryv->m_indfirst+1)&&(index==razryv->m_indlast-1)){
                //we will calculate a new point of the end of a gap, keeping a proportion between parts
                //segment, on which the end of a gap lies;
                double dlina = DLINASG(*ptar->GetAt(index+1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index+1));
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index+1), *ptar->GetAt(index-1), dlina);
                razryv->m_indlast--;
            }
            //the index of the deleted point is equal to an index of the end of an edge, on which
            //the beginning of a gap lies, and the index of the deleted point is equal to an index of the end of a gap
            else if((index==razryv->m_indfirst+1)&&(index==razryv->m_indlast)){
                //if we delete the last point, the gap vanishes
                if(index==ptar->GetSize()-1){
                    razryvy->Delete(i);
                    i--;
                }
                else{
                    //we will calculate new points of the beginning and end of a gap, keeping a proportion between
                    //parts of segments on which the beginning and the end of a gap lie;
                    double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                            DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index+1));
                    razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), *ptar->GetAt(index+1), dlina);
                    dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                            DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index+1));
                    razryv->m_ptlast = ExtendVector(*ptar->GetAt(index-1), *ptar->GetAt(index+1), dlina);
                }
            }
            //the index of the deleted point is equal to an index of the beginning of an edge, on which
            //the end of a gap lies,and the index of the deleted point is equal to an index of the beginning of a gap
            else if((index==razryv->m_indfirst)&&(index==razryv->m_indlast-1)){
                //if we delete the initial point, the gap vanishes
                if(index==0){
                    razryvy->Delete(i);
                    i--;
                }
                else{
                    //we will calculate new points of the beginning and end of a gap, keeping a proportion between
                    //parts of segments on which the beginning and the end of a gap lie;
                    double dlina = DLINASG(*ptar->GetAt(index+1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index))*
                            DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index-1));
                    razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index+1), *ptar->GetAt(index-1), dlina);
                    dlina = DLINASG(*ptar->GetAt(index+1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index))*
                            DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index-1));
                    razryv->m_ptlast = ExtendVector(*ptar->GetAt(index+1), *ptar->GetAt(index-1), dlina);
                    razryv->m_indfirst--;
                    razryv->m_indlast--;
                }
            }
            //the index of the deleted point is equal to an index of the beginning of an edge, on which
            //the end of a gap lies,and the index of the deleted point is equal to an end index
            //an edge on which the beginning of a gap lies
            else if((index==razryv->m_indfirst+1)&&(index==razryv->m_indlast-1)){
                //we will calculate new points of the beginning and end of a gap, keeping a proportion between
                //parts of segments on which the beginning and the end of a gap lie;
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index+1));
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), *ptar->GetAt(index+1), dlina);
                dlina = DLINASG(*ptar->GetAt(index+1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index-1));
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index+1), *ptar->GetAt(index-1), dlina);
                if(DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)<DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast))
                    razryv->m_indlast--;
                //we delete a gap if the received beginning and the end of a gap settle down
                //incorrectly
                else{
                    razryvy->Delete(i);
                    i--;
                }
            }
        }
    }

    UnionOverlappedRazryvy();

    return true;
}

//=========================================
//!corrects the array of gaps when moving a point with the contour 'index' index
//! top object in 'point' point;
bool MTopobject::CorrectRazryvsByMovePoint(int index, MPoint point)
//=========================================
{
    MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if (!((index>=0)&&(index<ptar->GetSize())))return false;

    //if among gaps there was a gap with transition through zero, will divide it into two gaps:
    //a gap from a zero point of a contour and a gap to the last point of a contour
    DevideOverlastptRazryvy();
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    if(razryvy->GetSize()==0)
        return true;
    MRazryv* razryv = razryvy->GetAt(razryvy->GetSize()-1);
    //if the point moves prior to the beginning of the first gap, points began
    //and the end of all gaps don't change
    if(index<razryv->m_indfirst)
        return true;
    razryv = razryvy->GetAt(razryvy->GetSize()-1);
    //if the point moves after the end of the last gap, points began
    //and the end of all gaps don't change
    if(index>razryv->m_indlast)
        return true;
    //on all gaps we make changes from movement of a point
    for(int i=0; i<razryvy->GetSize(); i++){
        razryv = razryvy->GetAt(i);
        //points of the beginning and the end of a gap change only if the point moves,
        //lying on a gap
        if((index>=razryv->m_indfirst)&&(index<=razryv->m_indlast)){
            //the index of the moved point is equal to an index of the beginning of a gap
            if((index==razryv->m_indfirst)&&(index<razryv->m_indlast-1)){
                //we will calculate a new point of the beginning of a gap, keeping a proportion between parts
                // of the segment on which the beginning of a gap lies;
                double dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(point,*ptar->GetAt(index+1));
                razryv->m_ptfirst = ExtendVector(point, *ptar->GetAt(index+1), dlina);
            }
            //the index of the moved point is equal to an index of the end of an edge, on which
            //the beginning of a gap lies
            else if((index==razryv->m_indfirst+1)&&(index<razryv->m_indlast-1)){
                //we will calculate a new point of the beginning of a gap, keeping a proportion between parts
                //of the segment on which the beginning of a gap lies;
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), point, dlina);
            }
            //the index of the moved point is equal to an index of the end of a gap
            else if((index>razryv->m_indfirst+1)&&(index==razryv->m_indlast)){
                //we will calculate a new point of the end of a gap, keeping a proportion between parts
                //of the segment on which the end of a gap lies;
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index-1), point, dlina);
            }
            //the index of the moved point is equal to an index of the beginning of an edge, on which
            //the end of a gap lies
            else if((index>razryv->m_indfirst+1)&&(index==razryv->m_indlast-1)){
                //we will calculate a new point of the end of a gap, keeping a proportion between parts
                //of the segment on which the end of a gap lies;
                double dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(point,*ptar->GetAt(index+1));
                razryv->m_ptlast = ExtendVector(point, *ptar->GetAt(index+1), dlina);
            }
            //the index of the moved point is equal to an index of the end of an edge, on which
            //the beginning of a gap lies,and the index of the deleted point is equal to an index of the end of a gap
            else if((index==razryv->m_indfirst+1)&&(index==razryv->m_indlast)){
                //we will calculate new points of the beginning and end of a gap, keeping a proportion between
                //parts of segments on which the beginning and the end of a gap lie;
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), point, dlina);
                dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index-1), point, dlina);
            }
            //the index of the moved point is equal to an index of the beginning of an edge, on which
            //the end of a gap lies,and the index of the deleted point is equal to an index of the beginning of a gap
            else if((index==razryv->m_indfirst)&&(index==razryv->m_indlast-1)){
                //we will calculate new points of the beginning and end of a gap, keeping a proportion between
                //parts of segments on which the beginning and the end of a gap lie;
                double dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(point,*ptar->GetAt(index+1));
                razryv->m_ptfirst = ExtendVector(point, *ptar->GetAt(index+1), dlina);
                dlina = DLINASG(*ptar->GetAt(index),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index),*ptar->GetAt(index+1))*
                        DLINASG(point,*ptar->GetAt(index+1));
                razryv->m_ptlast = ExtendVector(point, *ptar->GetAt(index+1), dlina);
            }
            //the index of the moved point is equal to an index of the beginning of an edge, on which
            //the end of a gap lies,and the index of the deleted point is equal to an end index
            //edges, on which the beginning of a gap lies
            else if((index==razryv->m_indfirst+1)&&(index==razryv->m_indlast-1)){
                //we will calculate new points of the beginning and end of a gap, keeping a proportion between
                //parts of segments on which the beginning and the end of a gap lie;
                double dlina = DLINASG(*ptar->GetAt(index-1),razryv->m_ptfirst)/DLINASG(*ptar->GetAt(index-1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index-1),point);
                razryv->m_ptfirst = ExtendVector(*ptar->GetAt(index-1), point, dlina);
                dlina = DLINASG(*ptar->GetAt(index+1),razryv->m_ptlast)/DLINASG(*ptar->GetAt(index+1),*ptar->GetAt(index))*
                        DLINASG(*ptar->GetAt(index+1),point);
                razryv->m_ptlast = ExtendVector(*ptar->GetAt(index+1), point, dlina);
            }
        }
    }
    UnionOverlappedRazryvy();

    return true;
}

//=========================================================
//!Forms a gap in a top object contour for the current formula;
/**ENTRANCE: 'pt' - a top object contour point - the center of a formula,
//           'nomer' - number of a formula of top object;
//The EXIT - the gap corresponding to a formula on a top object contour
*/
bool MTopobject::ConstructRazryvForCurrentFormula(MPlotDevice* pDevice, MPoint pt, int nomer, MRazryv* razryv)
//=========================================================
{
    ///	MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int index;
    MPoint point;
    if(FindDistanceFromPoint(pt,point, &index)>0.0001)
        return false;
    if(!m_pZnak) return false;

    int colforms = 0;
    double formdlina;
    colforms = m_pZnak->GetYaz()->m_formula->GetSize();
    //we will check that number of a formula was less total number of formulas of top object
    if(nomer>=colforms)
        return false;

    MFormula* formula = m_pZnak->GetYaz()->m_formula->GetAt(nomer);
    formdlina = MDrawing::GetFormulaDlina(this,pDevice,formula);
    formdlina = (double)11/(double)10*formdlina;
    //we will find coordinates of the ends of the segment defining a formula
    MPoint pt1, pt2;
    //if the point coincided with contour top
    if((pt==GetPoint(index))&&(index!=0)){
        pt1 = ExtendVector(pt, GetPoint(index-1), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+1), formdlina/2);
    }
    //if the point coincided with contour top
    else if((pt==GetPoint(index+1))&&(index != GetPointsNumber()-1)){
        pt1 = ExtendVector(pt, GetPoint(index), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+2), formdlina/2);
    }
    else{
        pt1 = ExtendVector(pt, GetPoint(index), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+1), formdlina/2);
    }
    MPoint resultpt1, resultpt2;
    int resultind1, resultind2;
    FindDistanceFromPoint(pt1,resultpt1, &resultind1);
    FindDistanceFromPoint(pt2,resultpt2, &resultind2);
    razryv->m_indfirst = resultind1;
    razryv->m_indlast = resultind2+1;
    razryv->m_ptfirst = resultpt1;
    razryv->m_ptlast = resultpt2;

    return true;
}

//=========================================================
//!Forms a gap in a top object contour in the array of gaps for the current formula
//! and a vector binding for the current formula in the array of bindings;
bool MTopobject::SetFormulaOnKontur(MPlotDevice* pDevice,MPoint pt, int nomer)
//=========================================================
{
    ///	MPtArray* ptar = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int index;
    MPoint point;
    if(FindDistanceFromPoint(pt,point, &index)>0.0001)
        return false;
    if(!m_pZnak) return false;

    int colforms = 0;
    double formdlina;
    colforms = m_pZnak->GetYaz()->m_formula->GetSize();
    //we will check that number of a formula was less total number of formulas of top object
    if(nomer>=colforms)
        return false;

    MFormula* formula = m_pZnak->GetYaz()->m_formula->GetAt(nomer);
    formdlina = MDrawing::GetFormulaDlina(this,pDevice, formula);
    //we will find coordinates of the ends of the segment defining a formula
    MPoint pt1, pt2;
    //if the point coincided with contour top
    if((pt==GetPoint(index))&&(index!=0)){
        pt1 = ExtendVector(pt, GetPoint(index-1), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+1), formdlina/2);
    }
    //if the point coincided with contour top
    else if((pt==GetPoint(index+1))&&(index != GetPointsNumber()-1)){
        pt1 = ExtendVector(pt, GetPoint(index), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+2), formdlina/2);
    }
    else{
        pt1 = ExtendVector(pt, GetPoint(index), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+1), formdlina/2);
    }
    MPoint resultpt1, resultpt2;
    int resultind1, resultind2;
    FindDistanceFromPoint(pt1,resultpt1, &resultind1);
    FindDistanceFromPoint(pt2,resultpt2, &resultind2);
    //if on a horizontal contour we put a formula Height that it was correctly developed
    //on a horizontal, it is necessary to change in a vector to a binding of a formula the beginning and the end in places
    if(WhatTheObject()==ID_IZOLINE){
        MPoint pt;
        pt = resultpt1;
        resultpt1 = resultpt2;
        resultpt2 = pt;
    }
    //we will add a vector-binding to the array of bindings of formulas
    AddVektorPriv(nomer,resultpt1,resultpt2);
    //we form the gap corresponding to an entrance formula on a contour
    //in comparison with a binding vector we will increase a gap a little
    formdlina = (double)11/(double)10*formdlina;
    //if the point coincided with contour top
    if((pt==GetPoint(index))&&(index!=0)){
        pt1 = ExtendVector(pt, GetPoint(index-1), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+1), formdlina/2);
    }
    //if the point coincided with contour top
    else if((pt==GetPoint(index+1))&&(index != GetPointsNumber()-1)){
        pt1 = ExtendVector(pt, GetPoint(index), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+2), formdlina/2);
    }
    else{
        pt1 = ExtendVector(pt, GetPoint(index), formdlina/2);
        pt2 = ExtendVector(pt, GetPoint(index+1), formdlina/2);
    }
    FindDistanceFromPoint(pt1,resultpt1, &resultind1);
    FindDistanceFromPoint(pt2,resultpt2, &resultind2);
    MRazryv* formpodp = new MRazryv(resultind1, resultind2+1, resultpt1, resultpt2);
    ((MRzArray*)m_Komplekt->GetAt(F_RAZRYVY))->AddTail(formpodp);
    delete formpodp;
    //after addition of a new gap we order the array of gaps
    OrderRazryvy();
    UnionOverlappedRazryvy();

    RecalcBounds();

    return true;
}

//==================================
//!adds a new gap to the array of gaps
bool MTopobject::AddRazryv(MPoint ptfirst, MPoint ptlast, int indfirst, int indlast)
//==================================
{
    int nPoints = GetPointsNumber();
    //if indexes are beyond contour indexes
    if((indfirst<0)||(indfirst>=nPoints)||(indlast<0)||(indlast>=nPoints))
        return false;
    MRazryv razryv(indfirst,indlast,ptfirst,ptlast);
    MRzArray* razryvy = (MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    razryvy->AddTail(&razryv);
    UnionOverlappedRazryvy();
    return true;
}


//================================================
//!function of determination of the extent of object in the UTP-file
unsigned int MTopobject::GetBytes()
//================================================
{
    unsigned int Size = MObject::GetBytes();
    //'m_kkod' variable length
    Size+=m_kkod.GetLength();//line size of a class


    return Size;
}

//============================================
//!returns the index on the array of points of a contour
MPtArray* MTopobject::GetPointArray() const
//============================================
{
    return (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
}

//==================================
//! cloning
MObject* MTopobject::Clone()
//==================================
{
    MTopobject* pClone=new MTopobject(m_bounds);
    MasterClone(pClone);
    pClone->m_kkod=m_kkod;
    pClone->m_pZnak=NULL;

    return (MObject*) pClone;
}


//!==================================
MPtArray* TestForSelfCross(MPtArray* Source)
//==================================
{
    int index = 0;
    MPoint SrcPt;
    MPoint DstPt;

    while(index<Source->GetSize()-1)
    {
        // We receive two points of a segment which we will compare
        SrcPt = Source->GetAt(index);
        DstPt = Source->GetAt(index+1);

        // We compare the received segment to all remained
        int position = index+1;
        while(position<Source->GetSize()-1)
        {
            // We receive two points of a segment to which we will compare
            MPoint SrcCmp = Source->GetAt(position);
            MPoint DstCmp = Source->GetAt(position+1);

            MPoint Cross;
            // We find a point of intersection of two straight lines if it is
            if(CROSSSECTION(SrcPt,DstPt,SrcCmp,DstCmp,&Cross))
            {

                // We check the found point
                if(Cross == SrcPt) // If the point coincides with the initial point of an initial straight line
                {
                    if((position!=Source->GetSize()-2)&&(index!=0))
                    {
                        Source->Delete(index+1);
                        // Now it is necessary to reduce the position index by quantity of remote elements
                        //if(position - 1 <=0) position = 0;
                        //else position -= 1;
                        index = 0;
                    }
                }
                else
                {
                    if(Cross != DstPt)// If the point doesn't coincide with initial and final points of an initial straight line
                        Source->SetAt(index+1,&Cross);
                }
                if(Cross == DstCmp)// If the point coincides with a final point of the looked-through straight line
                {
                    if((position!=Source->GetSize()-2)&&(index!=0))
                    {
                        Source->Delete(position);
                        // Now it is necessary to reduce the 'position' index by quantity of remote elements
                        //if(position - 1 <=0) position = 0;
                        //else position -= 1;
                        index = 0;
                    }
                }
                else
                {
                    if(Cross != SrcCmp)// If the point doesn't coincide with initial and final points of a prosmatrivamy straight line
                        Source->SetAt(position,&Cross);
                }
            }

            // We pass to the following segment
            position ++;
        }

        // We pass to the following segment
        index ++;
    }

    return Source;
}


//==================================
//! creation shadow top object
/**'pole' - a name of the field containing standard distance
// 'distofaxe'- distance from an axis
*/
MTopobject* MTopobject::Shade(MString /*pole*/,double distofaxe)
//==================================
{
    // if the zero standard distance is set, no shadow can be!
    if(distofaxe==0) return NULL;

    MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* ptarr = NULL;
    MPtArray* ptarr1 = NULL;
    MPtArray* ptarr2 = NULL;
    bool zamknut = false;

    // we create object-shadow and we tie it to a vulgar sign of the service classifier
    MTopobject* pShade=new MTopobject(m_bounds);

    // +++++++++++++++++++++++++++++++++
    // TEMPORARY ADDITIVE - ROUND
    pShade->m_pLay = m_pLay;
    // +++++++++++++++++++++++++++++++++

    pShade->ConnectToServKls(F_PLO);

    // The SHADOW is CREATED DIFFERENTLY DEPENDING ON OBJECT TYPE
    switch(m_pZnak->GetYaz()->GetType())
    {
    //SHADOW of VULGAR OBJECT
    //===============================================
    //Operations procedure:
    //1) installation of the right orientation
    //2) calculation of a contour of a shadow
    case F_PLO:
        if(pta->GetSize()==0)
            InternalMessage(QString(m_kkod.c_str()));
        else
        {
            if(Area()>0) InverseContur(pta);
            ptarr = CreateDistContur(pta, distofaxe, true);
            ptarr = TestForSelfCross(ptarr);
        }
        break;

        //SHADOW of LINEAR OBJECT
        //===============================================
        //Operations procedure:
        //1) orientation is indifferent
        //2) contours it is calculated twice - with positive and negative distance
        //(i.e. on those and other parties), then the contour is under construction total vulgar
    case F_LIN:
        //we modify type
        //if the linear object contains identical initial and final points,
        //that we build the closed leading round contour
        if(pta->GetSize()==0)
            InternalMessage(QString(m_kkod.c_str()));
        else
        {
            if((pta->GetAt(0)->x==pta->GetAt(pta->GetSize()-1)->x)&&(pta->GetAt(0)->y==pta->GetAt(pta->GetSize()-1)->y))

                zamknut = true;
            ptarr1 = CreateDistContur(pta, distofaxe, zamknut);
            ptarr2 = CreateDistContur(pta, -distofaxe, zamknut);
            InverseContur(ptarr2);//the address of an order of points in the second array
            if((ptarr1!=NULL)||(ptarr2!=NULL)) ptarr = new MPtArray;
            if(ptarr1!=NULL) ptarr->AppendTail(ptarr1);

            if(ptarr2!=NULL) ptarr->AppendTail(ptarr2);
            delete ptarr1;
            delete ptarr2;
            ptarr = TestForSelfCross(ptarr);
        }
        break;

        //SHADOW of DOT OBJECT
        //===============================================
        //Operations procedure:
        //1) concept orientation doesn't make sense
        //2) contours the circle is under construction in the form of a circle round a point,
        //it is approximated by some polygon
    case F_DOT:
        if(pta->GetSize()==0)
            InternalMessage(QString(m_kkod.c_str()));
        else
        {
            ptarr = CreateCircle(pta->GetAt(0), distofaxe, 10);//with a step of 10 degrees
            ptarr = TestForSelfCross(ptarr);
        }

        break;
    }

    if(ptarr!=NULL) {
        delete pShade->m_Komplekt->GetAt(pShade->F_TOCHKI);
        pShade->m_Komplekt->SetAt(pShade->F_TOCHKI,(MArray*)ptarr);
    }

    return (MTopobject*) pShade;
}


//!==================================
void MTopobject::SetYazType(int type,int nom)
//==================================
{
    if(!m_pZnak) return;
    //we will receive the pointer on the array of languages of a sign
    MYzArray* pYazArray = m_pZnak->GetYazArray();
    if((nom<0)||(nom>=pYazArray->GetSize())) return;

    MYaz* pYaz;
    if (type==F_DOT) {
        MSymbol sym;
        MSrez* srez = new MShriftSrez(&sym);
        MSzArray* szar = new MSzArray();
        szar->AddTail(srez);
        MDotYaz* pDot = new MDotYaz(szar);
        pYaz = new MYaz(F_DOT,NULL,(void*)pDot);
        pYazArray->SetAt(nom,pYaz);
        delete srez;
        delete szar;
        delete pDot;
        delete pYaz;
    }
    else if (type==F_LIN) {
        //the line
        MProsloika* prosloika = new MLinia();
        MPrArray* pra = new MPrArray();
        pra->AddTail(prosloika);
        MLinYaz* pLin = new MLinYaz(pra);
        pYaz = new MYaz(F_LIN,NULL,(void*)pLin);
        pYazArray->SetAt(nom,pYaz);
        delete prosloika;
        delete pra;
        delete pLin;
        delete pYaz;
    }

    else if (type==F_PLO) {
        //line
        MProsloika* prosloika = new MLinia();
        MPrArray* pra = new MPrArray();
        pra->AddTail(prosloika);
        //filling
        MPlast* plast = new MZalivka(1);
        MPlArray* pla = new MPlArray();
        pla->AddTail(plast);
        MPloYaz* pPlo = new MPloYaz(pra,pla);
        pYaz = new MYaz(F_PLO,NULL,(void*)pPlo);
        pYazArray->SetAt(nom,pYaz);
        delete pla;
        delete plast;
        delete prosloika;
        delete pra;
        delete pPlo;
        delete pYaz;
    }

}


//==================================
//! Turns top object into an external frame of the map
void MTopobject::DesignMaxRamkaForGrifKarta(MString /*grif*/, MString /*name*/, int mastab, MRect mapbounds)
//==================================
{
    MPoint pt1,pt2,pt3,pt4;

    // we form the most external frame of a leaf
    double delta = CalcBumToGauss(mastab,9.75);
    double tolst = CalcBumToGauss(mastab,0.7);
    double diampunkt = CalcBumToGauss(mastab,0.4);
    double dlprob    = CalcBumToGauss(mastab,2);
    double distofaxe    = CalcBumToGauss(mastab,0.9);

    double delta1 = CalcBumToGauss(mastab,1.8);
    double tolst1 = CalcBumToGauss(mastab,0.1);

    double delta2 = CalcBumToGauss(mastab,2.3);
    double tolst2 = CalcBumToGauss(mastab,0.1);

    double delta3 = CalcBumToGauss(mastab,2.05);
    double tolschina = CalcBumToGauss(mastab,0.5);
    double dlshtrih = CalcBumToGauss(mastab,5);
    double dlprob1 = CalcBumToGauss(mastab,10);
    bool m_kvadrat = true;
    double m_otstup = 0;

    pt1.x = mapbounds.left - delta;
    pt1.y = mapbounds.top + delta;
    pt2.x = mapbounds.right + delta;
    pt2.y = mapbounds.top + delta;
    pt3.x = mapbounds.right + delta;
    pt3.y = mapbounds.bottom - delta;
    pt4.x = mapbounds.left - delta;
    pt4.y = mapbounds.bottom - delta;
    AddPoint(&pt4);
    AddPoint(&pt1);
    AddPoint(&pt2);
    AddPoint(&pt3);
    AddPoint(&pt4);
    RecalcBounds();


    MPrArray* pra=new MPrArray();

    MProsloika* prosloika0=new MLinia((float)tolst, 0);
    pra->AddTail(prosloika0);


    MProsloika* prosloika1=new MLinia((float)tolst1, 0);
    prosloika1->m_distofaxe = (float)- delta1;
    //	pra->AddTail(prosloika1);

    MProsloika* prosloika2=new MLinia((float)tolst2, 0);
    prosloika2->m_distofaxe = (float)- delta2;
    //	pra->AddTail(prosloika2);

    MProsloika* prosloika3=new MPunktir((float)diampunkt, 0, (float)dlprob,0);
    prosloika3->m_distofaxe = (float)- distofaxe;
    prosloika3->m_synhro = false;
    //	pra->AddTail(prosloika3);

    MProsloika* prosloika4=new MShtrih((float)tolschina, 0, (float)dlshtrih, (float)dlprob1, m_kvadrat, (float)m_otstup);
    prosloika4->m_distofaxe = (float) - delta3;
    prosloika4->m_synhro = false;
    //	pra->AddTail(prosloika4);

    MLinYaz* pLin=new MLinYaz(pra);
    MYaz* pYaz = new MYaz(F_LIN,NULL,(void*)pLin);
    SetYaz(pYaz);
    delete prosloika0;
    delete prosloika1;
    delete prosloika2;
    delete prosloika3;
    delete prosloika4;
    delete pra;
    delete pLin;
    delete pYaz;
}


//==================================
//! top object section on two segments on one point
/** FOR ONE-KNEE top objects
//NB: Everyone turned out top object will include a point of a section
*/
MObject* MTopobject::Cut(int n)
//==================================
{
    if (m_Komplekt->GetSize()!=(F_TOCHKI+1)) return NULL;

    MTopobject* pClone = (MTopobject*)Clone();
    MPtArray* pta=(MPtArray*)(pClone->m_Komplekt->GetAt(F_TOCHKI));
    int i=pta->GetSize()-1;
    for(i=i; i>n; i--)pta->DeleteTail();
    GetLayer()->AddObject(pClone);
    pClone->RecalcBounds();

    pta=(MPtArray*)(m_Komplekt->GetAt(F_TOCHKI));
    i=Min(n,pta->GetSize());
    for(i=i; i<n; i++) pta->DeleteHead();
    RecalcBounds();

    return (MObject*)pClone;
}

//==================================
//! top object section on two segments on two points
/** FOR ONE-KNEE top objects
//NB: Everyone turned out top object will include a points of a section
*/
MObject* MTopobject::Cut(int from, int to)
//==================================
{
    int m=0, n=0;
    if (from == to) return NULL;
    if (from<to) {m = from; n = to;}
    if (from>to) {m = to; n = from;}

    if (m_Komplekt->GetSize()!=(F_TOCHKI+1)) return NULL;

    MTopobject* pClone = (MTopobject*)Clone();

    MPtArray* pta1=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* pta2=(MPtArray*)pClone->m_Komplekt->GetAt(F_TOCHKI);
    pta2->SetSize((n-m) + 2);
    int i = 0;
    for (i=0; i<(n-m)+2; i++) pta2->SetAt(i,pta1->GetAt(i+m));

    GetLayer()->AddObject(pClone);
    pClone->RecalcBounds();

    // new array of geopoints
    pta2 = new MPtArray();
    pta2->SetSize(pta1->GetSize() - (n-m));

    for (i=0; i<=m; i++) pta2->SetAt(i,pta1->GetAt(i));
    for (i=0; i<pta1->GetSize()-n-1; i++) pta2->SetAt(i+m+1,pta1->GetAt(i+n+1));

    m_Komplekt->SetAt(F_TOCHKI,(MArray*)pta2);
    free(pta1);

    RecalcBounds();

    return pClone;
}

//==================================
//! addition of top object to top object NOT as part
void MTopobject::Add(MTopobject* mpt)
//==================================
{
    //Check on an one-knee
    if((m_Komplekt->GetSize()!=(F_TOCHKI+1))||(mpt->m_Komplekt->GetSize()!=(F_TOCHKI+1)))
    {
        return;
    }
    //We attach one array of points to another
    MPtArray* pta1=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* pta2=(MPtArray*)mpt->m_Komplekt->GetAt(F_TOCHKI);
    pta1->AppendTail(pta2);

    // updating of a point of a tag
    RecalcBounds();
}

//==================================
//! addition of top object to top object as part
void MTopobject::AddPart(MTopobject* mpt)
//==================================
{
    //We attach all parts of 'mpt'
    int Indexs=mpt->m_Komplekt->GetSize();
    for(int i=F_TOCHKI;i<Indexs;i++)
    {
        MPtArray* pta2=(MPtArray*)mpt->m_Komplekt->GetAt(i);
        MPtArray* pta1=new MPtArray();
        pta1->SetSize(0);
        pta1->AppendTail(pta2);
        m_Komplekt->AddTail((MArray*)pta1);
    }

    //updating of the describing rectangle
    RecalcBounds();
}


//==================================
//! Recalculation of the describing rectangle
bool MTopobject::RecalcBounds()
//==================================
{
    if (GetHandleCount() == 0)
        return false;

    int Indexs=m_Komplekt->GetSize();

    MPtArray* pta = dynamic_cast<MPtArray*> (m_Komplekt->GetAt(F_TOCHKI));
    MPoint* pt1= (pta != nullptr) ? pta->GetAt(0) : nullptr;//((MPtArray*)(m_Komplekt->GetAt(F_TOCHKI)))->GetAt(0);
    if(!pt1) return false;
    //  for a start we take a rectangle round the first point...
    MRect bounds(pt1->x,
                 pt1->y,
                 pt1->x,
                 pt1->y);
    bounds.NormalizeGeoRect();

    // ...it also has to cover all the subsequent...
    for(int j=F_TOCHKI; j<Indexs; ++j)
    {
        //MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(j);
        pta = dynamic_cast<MPtArray*> (m_Komplekt->GetAt(j));
        if(pta == nullptr) continue;

        int Size = pta->GetSize();
        for(int i=0; i<Size; ++i)
        {
            pt1=pta->GetAt(i);
            bounds.ExpandToPt(*pt1);//[0]);
        }
    }

    bounds.NormalizeGeoRect();

    if (bounds == m_bounds)
        return false;

    m_bounds = bounds;
    return true;
}


//==================================
//! Addition of a point: a point in geocoordinates
void MTopobject::AddPoint(const MPoint& geopoint)
//==================================
{
    //We receive the pointer on the last part of points
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(m_Komplekt->GetSize()-1);
    MPoint* pt=NULL;
    //if the last point of the array isn't equal added,
    //that we add a point
    if(pta->GetSize()!=0)
    {
        pt=pta->GetAt(pta->GetSize()-1);
        if((pt[0]==geopoint))
            return;
    }
    pt= new MPoint;
    pt[0]=geopoint;
    pta->AddTail(pt);
    delete pt;

    RecalcBounds();
}

//==================================
//!
void MTopobject::SetPoints(MPtArray& geopoints)
//==================================
{
    int size = geopoints.GetSize();
    if(size<=0) return;

    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int i = 0;
    //  for(i = 0; i<pta->GetSize(); i++)
    //    pta->DeleteTail();
    pta->ClearAll();
    for(i = 0; i<size; i++)
        pta->AddTail(geopoints.GetAt(i));
    RecalcBounds();
}

//==============================================================
//!Looks for a point in with 'number' number
/**also returns the pointer on the array
//in which it is. Thus number becomes number
//points in this array
*/
MPtArray* MTopobject::GetArrayOfPoint(int &number)
//==============================================================
{
    MPtArray* pta;
    int count=m_Komplekt->GetSize();
    //We touch all arrays, we won't find where there is a point yet
    for(int i=F_TOCHKI;i<count;i++)
    {
        pta=(MPtArray*)m_Komplekt->GetAt(i);
        int j=pta->GetSize();
        //j=(i==F_TOCHKI)?(j-1):(j);
        if(number<j) return pta;
        number-=j;
    }
    return NULL;
}

//==================================
//! Point insert before a point: a point in geocoordinates
void MTopobject::InsertPoint(int num, const MPoint& geopoint)
//==================================
{
    CorrectRazryvsByInsertBeforePoint(num, geopoint);

    MPtArray* pta=GetArrayOfPoint(num);
    if(pta)
    {
        MPoint pt=geopoint;
        pta->InsertBefore(num,&pt);
    }

    // redrawing
    RecalcBounds();
}

//==================================
//! Point insert after a point: a point in geocoordinates
void MTopobject::InsertPointAfter(int num, const MPoint& geopoint)
//==================================
{
    CorrectRazryvsByInsertAfterPoint(num, geopoint);

    MPtArray* pta=GetArrayOfPoint(num);
    if(pta)
    {
        MPoint pt=geopoint;
        pta->InsertAfter(num,&pt);
    }

    // redrawing
    RecalcBounds();
}

//==================================
//! Removal of a point on ee to number
void MTopobject::DeletePoint(int num/*, MView* pView*/)
//==================================
{
    CorrectRazryvsByDeletePoint(num);

    int nomer=num;
    MPtArray* pta=GetArrayOfPoint(nomer);
    if(pta)
        pta->Delete(nomer);
}


//===========================================================
//!Deletes a point from the 'pta' array with the 'index' index
void MTopobject::DeletePoint(MPtArray* pta, int index)
//============================================================
{
    if((index<0)||(index>=pta->GetSize()))return;
    //We watch that there was at least one point
    if((m_Komplekt->GetSize()==(F_TOCHKI+1))&&(pta->GetSize()==1))return;
    CorrectRazryvsByDeletePoint(index);
    pta->Delete(index);
    //if it was the last point in the array, delete the array
    if(pta->GetSize()==0)
    {
        int i=m_Komplekt->Find(0,(MArray*)pta);
        m_Komplekt->Delete(i);
        delete pta;
    }
}


//==================================
//! Returns distance from the set point to the CONTOUR of our top object.
//! FOR ONE-KNEE top objects
double MTopobject::FindDistanceFromPoint(MPoint& gpt)
//==================================
{ 
    MPoint resultpt;
    int nomrebra;
    return FindDistanceFromPoint(gpt,resultpt,&nomrebra);
}

//==================================
//! Returns distance from the set point to the CONTOUR of our top object.
/** In 'resultpt' places the calculated next point.
//In nomrebra places an index of an edge on which the point was found.
//FOR ONE-KNEE top objects
*/
double MTopobject::FindDistanceFromPoint(MPoint& gpt, MPoint& resultpt,int* nomrebra)
//==================================
{
    // top object is one-knee?
    if((m_Komplekt->GetSize()-1)<F_TOCHKI)
        return -1;

    // for a start we will calculate distance to a zero point of top object
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    //if in object there is no point - for example, empty object,
    //got for formulas
    if(pta->GetSize()==0)
        return -1;
    MPoint* pt=pta->GetAt(0);
    resultpt = *pt;
    double rassto = gpt.Distance(pt);
    int m_nPoints = pta->GetSize();
    nomrebra[0]=0;

    // if top object from only one point
    if (m_nPoints<2) {
        resultpt = pt;
        return rassto;
    }

    // cycle on all segments, on each segment we find the next point
    for (int j=0; j<m_nPoints-1; j++) {
        // indexes on points of the beginning and end of the checked segment
        MPoint pt1 = pta->GetAt(j);
        MPoint pt2 = pta->GetAt(j+1);
        MPoint pt3 = gpt;
        MPoint pt4  = GetPointOnOtrezok(pt1,pt2,pt3);
        double rast = pt3.Distance(&pt4);
        if (rast<rassto) {
            rassto = rast;
            resultpt = pt4;
            nomrebra[0] = j;
        }
    }

    return rassto;
}

//==================================
//! We will find a point of intersection of ours of top object from some straight line,
/**  set by two points 'linept2', 'linept1'.
//If ours top object is crossed from a straight line in several places,
//'linept2', next to a point, gets out of all points of intersection.
//The calculated point of intersection is placed in 'resultpt'.
//The returned values:
//0 - crossing is;
//1 - crossings aren't present;
//2 - top object not one-knee;
//3 - the top object contains less than two points.
*/
int MTopobject::FindNearestIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto)
//==================================
{
    // top object is one-knee?
    if((m_Komplekt->GetSize()-1)!=F_TOCHKI)
        return 2;

    // we will receive the index on the array of points and the size of this array
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int kol=pta->GetSize();

    if(kol<2) return 3;

    // we calculate distance from 'linept2' point to the most remote point of top object
    rassto = 0;
    int j = 0;
    for (j=0; j<kol-1; j++) {
        // the pointer on the next point
        MPoint pt = pta->GetAt(j);
        double rast = linept2.Distance(&pt);
        if (rast>rassto) {
            rassto = rast;
        }
    }


    // cycle on all segments, we look for points of intersection
    int kodvozvrata = -1;
    MPoint respt;
    for (j=0; j<kol-1; j++) {
        // pointers on points of the beginning and end of the checked segment
        MPoint pt1 = pta->GetAt(j);
        MPoint pt2 = pta->GetAt(j+1);
        double rast;
        int kod = IntersectLineWithOtrezok(pt1, pt2, linept1, linept2, &respt, rast);
        // whether there was a point of intersection?
        if((kod==0)||(kod==5)||(kod==7)||(kod==9)||(kod==10)||(kod==11)) {
            // it is the next?
            if (rast<rassto) {
                rassto = rast;
                resultpt = respt;
                kodvozvrata = 0;
            }
        }
    }

    return kodvozvrata;
}

//==================================
//! We will find top object our point of intersection with a segment,
/**set by two points 'sgpt1'  'sgpt2'.
//The returned values:
//the index on the array of points of intersection if points aren't present, we return NULL
*/
MPtArray* MTopobject::FindIntersectsWithSegment(MPoint sgpt1, MPoint sgpt2)
//==================================
{
    // top object is one-knee?
    if((m_Komplekt->GetSize()-1)!=F_TOCHKI)
        return NULL;

    // we will receive the index on the array of points and the size of this array
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int kol=pta->GetSize();

    //we get the array of points of intersection
    MPtArray* resar = new MPtArray();
    //if the top object consists of one point...
    if(kol==2){
        //we check, whether the point on a segment lies
        if(!PTONSEG(pta->GetAt(0)[0], sgpt1, sgpt2)){
            delete resar;
            return NULL;
        }
        resar->AddTail(pta->GetAt(0));
    }

    // cycle on all segments of top object, we look for points of intersection of segments with a segment
    else{
        MPoint respt;
        for (int j=0; j<kol-1; j++) {
            // pointers on points of the beginning and end of the checked segment
            MPoint pt1 = pta->GetAt(j);
            MPoint pt2 = pta->GetAt(j+1);
            //we check, whether the top object segment with a segment is crossed
            bool kod = CROSSSECTION(pt1, pt2, sgpt1, sgpt2, &respt);
            // if there was a point of intersection, will add it to our array
            if(kod) {
                resar->AddTail(&respt);
            }
        }
    }

    //if there were no segment points of intersection with top object,
    if(resar->GetSize()==0){
        delete resar;
        return NULL;
    }

    return resar;
}


//==================================
//! To find (or to create) in top object a point, the next to set,
/** with the demanded admission or without it and to move the set point there
//FOR ONE-KNEE top objects
*/
int MTopobject::MovePointToTouchFrontier(MPoint& gpt, double* dopusk)
//==================================
{
    // Method such. If the admission is set, we find satisfying to the admission
    //the next point (or we generate if that falls on the midpoint).
    //If it isn't set, then simply we find it (is generated) the next.

    //NB: Function returns number of the next, existing earlier or again created,
    //points. But, BESIDES, function moves ITSELF the set point to this new place!!

    if((m_Komplekt->GetSize()-1)!=F_TOCHKI)
        return -1000000;

    double rassto;   // distance to a point
    int number;      // number of a required point

    // at first we look for, whether will be in investigated to top object of points,
    //satisfying to criterion of proximity
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPoint* pt=pta->GetAt(0);
    rassto = gpt.Distance(pt); number = 0;
    int m_nPoints=pta->GetSize();
    for (int j=1; j<m_nPoints; j++) {
        if (rassto > gpt.Distance(pta->GetAt(j))) {rassto = gpt.Distance(pta->GetAt(j)); number = j;}
    }

    //if it wasn't succeeded to find, we start proper searches
    if ((rassto>*dopusk)||(dopusk = NULL)) {
        double rasst = rassto;
        int numb = number;
        for (int j=0; j<m_nPoints-1; j++) {
            // indexes on points of the beginning and end of the checked segment
            MPoint* pt1 = pta->GetAt(j);
            MPoint* pt2 = pta->GetAt(j+1);
            // whether the perpendicular on a segment falls?
            if(((pt2->x - pt1->x)*(gpt.x - pt1->x)+(pt2->y - pt1->y)*(gpt.y - pt1->y)>=0)&&
                    ((pt1->x - pt2->x)*(gpt.x - pt2->x)+(pt1->y - pt2->y)*(gpt.y - pt2->y)>=0)) {
                // then we will check distance
                double A = pt2->x - pt1->x;
                double B = pt1->y - pt2->y;
                double C = pt1->y*(-A) + pt1->x*(-B);
                double normer = sqrt(A*A + B*B);
                A /= normer;
                B /= normer;
                C /= normer;
                double delta = fabs(A*gpt.y + B*gpt.x + C);
                if (rasst > delta) {
                    ///					double delta1 = fabs(A*pt1->y + B*pt1->x + C);
                    ///					double delta2 = fabs(A*pt2->y + B*pt2->x + C);
                    rasst = delta;
                    numb = j;
                }
            }
        }

        // if there was a distance less, it is necessary to generate a point
        if ((rasst<*dopusk)||(dopusk = NULL)) {
            MPoint* pt1 = pta->GetAt(numb);
            MPoint* pt2 = pta->GetAt(numb+1);

            double S1 = (gpt.x - pt1->x)*(gpt.x - pt1->x) + (gpt.y - pt1->y)*(gpt.y - pt1->y);
            double S2 = (gpt.x - pt2->x)*(gpt.x - pt2->x) + (gpt.y - pt2->y)*(gpt.y - pt2->y);
            MPoint newpt(pt1->x + (S1/(S1+S2))*(pt2->x - pt1->x),pt1->y + (S1/(S1+S2))*(pt2->y - pt1->y));

            // at first it...
            gpt.x = newpt.x;
            gpt.y = newpt.y;

            // ... then it!!
            InsertPointAfter(numb,newpt);

            return numb;
        }

    }
    // if the distance is less than admission, it is necessary to close edited top object
    else {
        gpt.x = pta->GetAt(number)->x;
        gpt.y = pta->GetAt(number)->y;
        return number;
    }


    //in case of failure we return -1000000
    return -1000000;
}

//==================================
//! To find (or to create) in top object a point, the next to set,
/** excepting a point with such number and to move the set point there.
//Function, unlike MovePointToTouchFrontier, serves for that case,
//when the set point belongs to top object.
//FOR ONE-KNEE top objects
*/
int MTopobject::MovePointToTouchFrontierExept(MPoint& gpt, int exeptpt)
//==================================
{
    // NB: Function returns number of the next, existing earlier or again created, points.

    //NB: BESIDES, function moves ITSELF the set point to this new place!!

    //NB: The point with number set by the second parameter is switched off from consideration!!
    //It is made in order that function made sense, even if the set point
    //belongs to top object (and differently it will find herself as the next).


    if((m_Komplekt->GetSize()-1)!=F_TOCHKI)
        return -1000000;

    // at first we look for, whether will be in investigated to top object of points,
    //satisfying to criterion of proximity
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    //MPoint* pt=pta->GetAt(0);

    double rassto;   // distance to a point
    int number;      // number of a required point

    // For the sake of processing of one exceptional case (but often meeting in
    //the Knife tool) it is necessary to take some measures. This case consists in that,
    //that a quantity of points on the right and some coincide with our point at the left.
    //To avoid tautological short circuit, we have to switch off from consideration
    //a point, all next coinciding (NEXT, but not all those!), and
    //on a point on the right and at the left.

    int m_nPoints=pta->GetSize();
    int m = exeptpt;
    int n = exeptpt;
    // lower bound
    int i = 0;
    for (i = m; i>0; i--) {
        if ((pta->GetAt(i))[0]==(pta->GetAt(m))[0]) m = i;
        else break;
    }
    // upper bound
    for (i = n; i<m_nPoints; i++) {
        if ((pta->GetAt(i))[0]==(pta->GetAt(n))[0]) n = i;
        else break;
    }


    // at first we will walk on top object points
    if (exeptpt!=0) {rassto = gpt.Distance(pta->GetAt(0)); number = 0;}
    else			 {rassto = gpt.Distance(pta->GetAt(1)); number = 1;}
    int j = 0;
    for (j=0; j<m_nPoints; j++) {
        // we switch off a point, the coinciding, and two next from consideration
        if (((j<m-1)||(j>n+1))&&(rassto > gpt.Distance(pta->GetAt(j)))) {

            rassto = gpt.Distance(pta->GetAt(j));
            number = j;
        }
    }

    // now we will walk on segments
    double rasst = rassto;
    int numb = number;
    for (j=0; j<m_nPoints; j++) {
        // the segments having the end the set point, coincident, and
        //we exclude two next from consideration
        if ((j<m-1)||(j>n+1)) {
            // pointers on points of the beginning and end of the checked segment
            MPoint* pt1;
            MPoint* pt2;
            if (j<m_nPoints-1) {
                pt1 = pta->GetAt(j);
                pt2 = pta->GetAt(j+1);
            }
            else {
                pt1 = pta->GetAt(j);
                pt2 = pta->GetAt(0);
            }
            // the perpendicular falls on a segment?
            if(((pt2->x - pt1->x)*(gpt.x - pt1->x)+(pt2->y - pt1->y)*(gpt.y - pt1->y)>=0)&&
                    ((pt1->x - pt2->x)*(gpt.x - pt2->x)+(pt1->y - pt2->y)*(gpt.y - pt2->y)>=0)) {
                // then we will check distance
                double A = pt2->x - pt1->x;
                double B = pt1->y - pt2->y;
                double C = pt1->y*(-A) + pt1->x*(-B);
                double normer = sqrt(A*A + B*B);
                A /= normer;
                B /= normer;
                C /= normer;
                double delta = fabs(A*gpt.y + B*gpt.x + C);
                if (rasst > delta) {
                    ///					double delta1 = fabs(A*pt1->y + B*pt1->x + C);
                    //double delta2 = fabs(A*pt2->y + B*pt2->x + C);
                    rasst = delta;
                    numb = j;
                }
            }
        }
    }

    // if there was a distance less, it is necessary to generate a point
    if (rasst<rassto) {
        MPoint* pt1 = pta->GetAt(numb);
        MPoint* pt2 = pta->GetAt(numb+1);

        double S1 = (gpt.x - pt1->x)*(gpt.x - pt1->x) + (gpt.y - pt1->y)*(gpt.y - pt1->y);
        double S2 = (gpt.x - pt2->x)*(gpt.x - pt2->x) + (gpt.y - pt2->y)*(gpt.y - pt2->y);
        MPoint newpt(pt1->x + (S1/(S1+S2))*(pt2->x - pt1->x),pt1->y + (S1/(S1+S2))*(pt2->y - pt1->y));

        // at first it...
        gpt.x = newpt.x;
        gpt.y = newpt.y;

        // ... then it!!
        InsertPointAfter(numb,newpt);
        return numb;
    }

    // and differently and it is necessary to generate nothing
    else {
        gpt.x = pta->GetAt(number)->x;
        gpt.y = pta->GetAt(number)->y;
        return number;
    }

    // in case of failure we return -1000000
    return -1000000;
}


//==================================
//! separation of separate parts of top object
bool MTopobject::Sew(MTopobject* pTpo,bool tohead)
//==================================
{
    //Check on an one-knee
    if(m_Komplekt->GetSize()!=(F_TOCHKI+1))return false;
    if(pTpo->m_Komplekt->GetSize()!=(F_TOCHKI+1))return false;


    //We receive pointers on arrays of points of initial top objects
    MPtArray* pta1=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* pta2=(MPtArray*)pTpo->m_Komplekt->GetAt(F_TOCHKI);

    if (tohead)
    {
        pta1->AppendHead(pta2);
    }
    else
    {
        pta1->AppendTail(pta2);
    }

    //updating of the describing rectangle
    RecalcBounds();

    return true;
}


//==================================
//! Calculation (focused) area
/** (ONLY for noncompound top objects)
*/
double MTopobject::Area()
//==================================
{
    double S =0;
    //Check on an one-knee
    if(m_Komplekt->GetSize()!=(F_TOCHKI+1))return S;
    //We receive the array of points
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int m_nPoints=pta->GetSize();
    for(int i=0; i<m_nPoints-1; i++)
        S += (pta->GetAt(i)->x - pta->GetAt(i + 1)->x)*(pta->GetAt(i)->y + pta->GetAt(i+1)->y);

    S += (pta->GetAt(m_nPoints-1)->x - pta->GetAt(0)->x)*(pta->GetAt(m_nPoints-1)->y + pta->GetAt(0)->y);
    S = S/2;
    return S;
}

//==================================
//! Change of orientation to the return
/** (ONLY for noncompound top objects)
*/
bool MTopobject::OrientInverse()
//==================================
{
    //Check on an one-knee
    if (m_Komplekt->GetSize()!=(F_TOCHKI+1)) return false;

    //We receive the pointer on the array of points
    MPtArray* pta1=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* pta2=new MPtArray();
    int	m_nPoints=pta1->GetSize();
    pta2->SetSize(m_nPoints);

    //We reorient an order of points
    for (int i=0; i<m_nPoints; i++) pta2->SetAt(i,pta1->GetAt(m_nPoints - i - 1));

    //We interchange the position of arrays
    m_Komplekt->DeleteTail();
    m_Komplekt->AddTail((MArray*)pta2);
    free(pta1);

    //We receive the pointer on the array of gaps
    MRzArray* razryvy=(MRzArray*)m_Komplekt->GetAt(F_RAZRYVY);
    int rzcol = razryvy->GetSize();
    if(rzcol != 0){
        //for each gap we interchange the position of initial and final points of a conclusion of a gap,
        //also final and initial indexes plus numbering of indexes are interchanged the position
        for(int i=0; i<rzcol; i++){
            MRazryv* razryv = razryvy->GetAt(i);
            int ind = razryv->m_indfirst;
            razryv->m_indfirst = m_nPoints - razryv->m_indlast - 1;
            razryv->m_indlast = m_nPoints - ind - 1;
            MPoint pt = razryv->m_ptfirst;
            razryv->m_ptfirst = razryv->m_ptlast;
            razryv->m_ptlast = pt;
        }
    }

    return true;
}

//==================================
//! Installation of the left orientation
/** (ONLY for noncompound hyper points)
*/
bool MTopobject::OrientLeft()
//==================================
{
    //Check on an one-knee
    if (m_Komplekt->GetSize()!=(F_TOCHKI+1)) return false;

    if(Area()<0) OrientInverse();

    return true;
}


//==================================
//! Installation of the right orientation
/** (ONLY for noncompound hyper points)
*/
bool MTopobject::OrientRight()
//==================================
{
    //Check on an one-knee
    if (m_Komplekt->GetSize()!=(F_TOCHKI+1)) return false;
    if(Area()>0) OrientInverse();
    return true;
}


//=============================
//!Returns quantity of points
int MTopobject::GetPointsNumber()
//=============================
{
    int m_nPoints=0;
    int Size=m_Komplekt->GetSize();
    MPtArray* pta;
    for(int i=F_TOCHKI;i<Size;i++)
    {
        pta=(MPtArray*)m_Komplekt->GetAt(i);
        m_nPoints+=pta->GetSize();
    }

    return m_nPoints;
}

//=============================
//!Returns quantity of parts
int MTopobject::GetPartsNumber()
//=============================
{
    int m_nParts=m_Komplekt->GetSize()-F_TOCHKI;
    return m_nParts;
}

//=============================
//! check: whether this point in any our part of top object lies
/**  (parts are considered CLOSED, but top object not necessarily single-part))
//RETURNS number of part in 'npart'
*/
bool MTopobject::IsInnerPoint(MPoint* pt, int* npart)
//=============================
{
    //=========================================
    // Idea: from 'pt' point the horizontal beam which will cross some edges of a polygon 'Plg' is carried out to the right;
    //       if number of these edges odd, a point (x, y) - internal in relation to 'Plg' polygon, if even,
    //       that 'pt' point - external in relation to 'Plg' polygon
    // parameters: (x, y) - any point

    //              'pPlg' - the index on the array of points like polygon double
    //              'n' - dimension of the array
    //the returned value: 'true' - (x, y) - an internal point, 'false' - otherwise
    //remark: for a situation when X-coordinates of an edge lie on the different parties from point X-coordinate, we look for 'pt' point
    //        crossings of the horizontal straight line passing through a point (x, y), and the straight line passing through a polygon edge;
    //        if x - the coordinate of a point of intersection lies more to the right x - point coordinates (x, y), the horizontal beam will cross an edge
    //=========================================
    bool result = false;

    // cycle in parts top object
    for(int num=F_TOCHKI;num<m_Komplekt->GetSize();num++)
    {
        // we will receive the pointer on part and number of points in part
        MPoint* pPlg=((MPtArray*)(m_Komplekt->GetAt(num)))->GetAt(0);
        int n=m_Komplekt->GetAt(num)->GetSize();

        MPoint doppt; // 'doppt' - point of intersection of the horizontal straight line and straight line passing through an edge

        int col = 0; // 'col' - shows number of crossings of a horizontal beam with 'Plg' polygon edges

        std::vector<unsigned int> indArray; // in a dynamic array 'indArray' we will keep indexes of edges of a polygon 'Plg' through which lie more to the right of 'doppt' point
        int i = 0;
        for(i=0; i<n-1; i++){
            if((pPlg[i].x>=pt->x) & (pPlg[i+1].x>=pt->x))
                indArray.push_back(i);
            // for a situation when x-coordinates of an edge lie on the different parties from point x-coordinate, we look for 'doppt1' point
            //crossings of the horizontal straight line passing through 'doppt' point, and the straight line passing through a polygon edge;
            //if x - the coordinate of a point of intersection lies more to the right x - 'doppt' point coordinates, the horizontal beam will cross
            //an edge in case of performance of a condition on at - coordinates of all points
            else if(((pPlg[i].x<pt->x) & (pPlg[i+1].x>pt->x)) || ((pPlg[i].x>pt->x) & (pPlg[i+1].x<pt->x))){
                // the formula for finding of coordinates of a point of intersection is removed according to the reference book Bronstein-Semendyaev
                doppt.x = (pt->y*(pPlg[i].x - pPlg[i+1].x) - pPlg[i].x*(pPlg[i+1].y - pPlg[i].y) + pPlg[i].y*(pPlg[i+1].x - pPlg[i].x))/(pPlg[i].y - pPlg[i+1].y);
                doppt.y = pt->y;
                if(doppt.x>pt->x)
                    indArray.push_back(i);
            }
        }
        if((pPlg[n-1].x>=pt->x) & (pPlg[0].x>=pt->x))
            indArray.push_back(n-1);
        else if(((pPlg[n-1].x<pt->x) & (pPlg[0].x>pt->x)) || ((pPlg[n-1].x>pt->x) & (pPlg[0].x<pt->x))){
            doppt.x = (pt->y*(pPlg[n-1].x - pPlg[0].x) - pPlg[n-1].x*(pPlg[0].y - pPlg[n-1].y) + pPlg[n-1].y*(pPlg[0].x - pPlg[n-1].x))/(pPlg[n-1].y - pPlg[0].y);
            doppt.y = pt->y;
            if(doppt.x>pt->x)
                indArray.push_back(n-1);
        }
        int m = indArray.size();
        // if y-coordinates of the ends of the chosen edge lie on the different parties from 'doppt' point y-coordinate, the horizontal beam crosses an edge
        for(i=0; i<m; i++) {

            if(indArray[i] != (unsigned int)(n-1)){
                if(((pPlg[indArray[i]].y > pt->y) & (pPlg[indArray[i] + 1].y < pt->y)) || ((pPlg[indArray[i]].y < pt->y) & (pPlg[indArray[i] + 1].y > pt->y)))
                    col++;
                // separately we consider cases when the horizontal beam crosses 'pPlg' polygon top
                else{
                    //					int s;
                    //					if(indArray[i]==0)
                    //						s = n-1;
                    //					else
                    //						s = indArray[i]-1;

                    if(((pPlg[indArray[i]].y > pt->y) & (pPlg[indArray[i] + 1].y == pt->y)/* & (pPlg[s].y < pt->y)*/) || ((pPlg[indArray[i]].y < pt->y) & (pPlg[indArray[i] + 1].y == pt->y)/* & (pPlg[s].y > pt->y)*/)){
                        col++;
                        i++;
                    }
                }
            }
            else{
                if(((pPlg[n-1].y > pt->y) & (pPlg[0].y < pt->y)) || ((pPlg[n-1].y < pt->y) & (pPlg[0].y > pt->y)))
                    col++;
                // separately we consider cases when the horizontal beam crosses 'pPlg' polygon top
                else if(((pPlg[n-1].y > pt->y) & (pPlg[0].y == pt->y)/* & (pPlg[n-2].y < pt->y)*/) || ((pPlg[n-1].y < pt->y) & (pPlg[0].y == pt->y)/* & (pPlg[n-2].y > pt->y)*/))
                    col++;
            }
        }
        // we check parity oddness of the 'col' parameter
        if((double)((double)col/2) != (int)((double)col/2)) {
            result = true;
            npart[0] = num;
        }

    }

    return result;

}

//=============================
//! check: whether the set segment entirely in any our part of top object lies
/** (ours is considered top object CLOSED, but not necessarily single-part)
//RETURNS number of part in npart
*/
bool MTopobject::IsInnerSegment(MPoint* pt1,MPoint* pt2, int* /*npart*/)
//=============================
{
    bool result = true;

    // cycle in parts top object
    for(int num=F_TOCHKI;num<m_Komplekt->GetSize();num++) {

        // we will receive the pointer on part and number of points in part
        MPtArray* pReg=(MPtArray*)m_Komplekt->GetAt(num);
        if(!SGINREG(pt1,pt2,pReg)) result = false;
    }

    return result;
}


//=============================
//! check: whether lies set top object in any our part of top object
/** (both top objects are considered CLOSED and SINGLE-PART)
*/
bool MTopobject::IsInnerTopobject(MTopobject* pTpo)
//=============================
{
    // GENERAL IDEA: if all segments of any part this top object lie inside
    //any our part of top object (and it is fair for all parts given top object)
    //that it lies in ours top object
    bool result = true;

    // cycle in parts set top object
    for(int num=F_TOCHKI;num<pTpo->m_Komplekt->GetSize();num++) {

        // we will receive the pointer on part and number of points in part
        MPoint* pPlg=((MPtArray*)(pTpo->m_Komplekt->GetAt(num)))->GetAt(0);
        int n=pTpo->m_Komplekt->GetAt(num)->GetSize();

        int npart;
        MPoint pt1,pt2;
        for(int i=0;i<n-1;i++){
            pt1 = pPlg[i];
            pt2 = pPlg[i+1];
            if(!IsInnerSegment(&pt1,&pt2,&npart)) result = false;
        }
    }

    return result;
}



//==================================
//! calculation of the area
double MTopobject::GetArea()

//==================================
{
    double S =0;
    int m_nParts=GetPartsNumber();
    for (int i=0; i<m_nParts; i++)
        S += GetPartArea(i);
    return S;
}


//==================================
//! calculation of the perimeter
double MTopobject::GetPerimeter()
//==================================
{
    double D =0;
    int m_nParts=GetPartsNumber();
    for (int i=0; i<m_nParts; i++)
        D += GetPartPerimeter(i);
    return D;
}

//==================================
//! orientation calculation (for one-knee)
int MTopobject::GetOrientation()
//==================================
{
    //Check on an one-knee
    if (m_Komplekt->GetSize()!=(F_TOCHKI+1)) return 0;
    return GetPartOrientation(0);
}


//==================================
//! calculation of the perimeter
double MTopobject::GetZazor()
//==================================
{

    double D = 0;
    int m_nPoints=GetPointsNumber();
    int index=0;
    MPtArray* pta=GetArrayOfPoint(index);
    if(!pta)return 0;
    MPoint* pt0=pta->GetAt(index);
    index=m_nPoints-1;
    pta=GetArrayOfPoint(index);
    if(!pta) return 0;
    MPoint* pt1=pta->GetAt(index);

    D = sqrt((pt0->x - pt1->x)*(pt0->x - pt1->x) +
             (pt0->y - pt1->y)*(pt0->y - pt1->y));
    return D;
}

//==================================
//! calculation of number of points of part
int MTopobject::GetPartPointsNumber(int ii)
//==================================
{
    int m_nParts=GetPartsNumber();
    if ((ii<0)||(ii>=m_nParts)) return 0;
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(ii+F_TOCHKI);
    return pta->GetSize();
}

//==================================
//! calculation of the area of part
double MTopobject::GetPartArea(int ii)
//==================================
{
    int m_nParts=GetPartsNumber();
    if ((ii<0)||(ii>=m_nParts)) return 0;
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(ii+F_TOCHKI);

    int Size=pta->GetSize();

    double S =0;
    for (int i=1; i<Size; i++)
        S += (pta->GetAt(i-1)->x - pta->GetAt(i)->x)*(pta->GetAt(i-1)->y + pta->GetAt(i)->y);

    S += (pta->GetAt(Size-1)->x - pta->GetAt(0)->x)*(pta->GetAt(Size-1)->y + pta->GetAt(0)->y);
    S = S/2;
    return fabs(S);
}


//==================================
//! calculation of the perimeter of part
double MTopobject::GetPartPerimeter(int ii)
//==================================
{
    int m_nParts=GetPartsNumber();
    if ((ii<0)||(ii>=m_nParts)) return 0;
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(ii+F_TOCHKI);

    int Size=pta->GetSize();

    double D = 0;
    for (int i=1; i<Size; i++)
        D += sqrt( (pta->GetAt(i)->x - pta->GetAt(i-1)->x)*(pta->GetAt(i)->x - pta->GetAt(i-1)->x) +
                   (pta->GetAt(i)->y - pta->GetAt(i-1)->y)*(pta->GetAt(i)->y - pta->GetAt(i-1)->y));
    D += sqrt((pta->GetAt(0)->x - pta->GetAt(Size-1)->x)*(pta->GetAt(0)->x - pta->GetAt(Size-1)->x) +
              (pta->GetAt(0)->y - pta->GetAt(Size-1)->y)*(pta->GetAt(0)->y - pta->GetAt(Size-1)->y));
    return D;
}

//==================================
//! calculation of the orientation of part
int MTopobject::GetPartOrientation(int ii)
//==================================
{
    int m_nParts=GetPartsNumber();
    if ((ii<0)||(ii>=m_nParts)) return 0;
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(ii+F_TOCHKI);

    int Size=pta->GetSize();

    double S =0;
    for (int i=1; i<Size; i++)
        S += (pta->GetAt(i-1)->x - pta->GetAt(i)->x)*(pta->GetAt(i-1)->y + pta->GetAt(i)->y);

    S += (pta->GetAt(Size-1)->x - pta->GetAt(0)->x)*(pta->GetAt(Size-1)->y + pta->GetAt(0)->y);

    S = S/2;

    if (S<0) return -1;
    else if (S>0) return 1;
    else return 0;
}

//==================================
//! calculation of a gap of part

double MTopobject::GetPartZazor(int ii)
//==================================
{
    int m_nParts=GetPartsNumber();
    if ((ii<0)||(ii>=m_nParts)) return 0;
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(ii+F_TOCHKI);

    int Size=pta->GetSize();

    double D = 0;
    D += sqrt((pta->GetAt(0)->x - pta->GetAt(Size-1)->x)*(pta->GetAt(0)->x - pta->GetAt(Size-1)->x) +
              (pta->GetAt(0)->y - pta->GetAt(Size-1)->y)*(pta->GetAt(0)->y - pta->GetAt(Size-1)->y));
    return D;
}

//================================
//!Returns the necessary point
MPoint MTopobject::GetPoint(int i) 
//================================
{
    int index=i;
    MPtArray* pta=GetArrayOfPoint(index);
    if(pta)
    {
        MPoint* pt=pta->GetAt(index);
        return pt[0];
    }
    else
    {
        return MPoint(0,0);
    }
}

void MTopobject::SetPoint(int i, MPoint & p)
{
    MPoint * p1 = GetpPoint(i);
    if (p1)
        *p1 = p;
}

//========================================================
//!Establishes the description of top object identically with specified for language with number nom
//! in the array of languages
bool MTopobject::SetYaz(MYaz *m_pYazToSet, int nom)
//========================================================
{
    if(!m_pYazToSet)  return false;     //exit(15);
    if(!m_pZnak)
        return false;
    if(!m_pZnak->SetYaz(m_pYazToSet, nom))
        return false;
    return true;
}

// ==================================================================
//		Returns length between two points on a contour
//Is considered only первй a contour
//! --------- 
double MTopobject::GetLengthBetweenPointsByPerimeter(MPoint pt1,MPoint pt2,int iFirstPointSide, int iSecondPointSide)
// ==================================================================
{
    double dRetVal = 0;

    MPtArray* ppta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    if((iFirstPointSide == -1)||(iSecondPointSide == -1)){
        //Finding of an edge where there are points
        int iSize = ppta->GetSize();
        for(int i=0;i<iSize-1;i++){
            if(PTONSEG(pt1,*ppta->GetAt(i),*ppta->GetAt(i+1)))
                iFirstPointSide= i;
            if(PTONSEG(pt2,*ppta->GetAt(i),*ppta->GetAt(i+1)))
                iSecondPointSide= i;
            if((iFirstPointSide!=-1)&&(iSecondPointSide!=-1))
                i = iSize;
        }
    }
    if((iFirstPointSide ==-1)||(iSecondPointSide==-1))
        return dRetVal;
    //Now we know edges on which there are our points
    MPoint pt;
    if(iFirstPointSide == iSecondPointSide){
        //Points are on one straight line
        pt = *ppta->GetAt(0);
        dRetVal = sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x)+(pt1.y - pt2.y)*(pt1.y - pt2.y));
        if(sqrt((pt1.x - pt.x)*(pt1.x - pt.x)+(pt1.y - pt.y)*(pt1.y - pt.y))>
                sqrt((pt.x - pt2.x)*(pt.x - pt2.x)+(pt.y - pt2.y)*(pt.y - pt2.y)))
            dRetVal = -dRetVal;
        return dRetVal;
    }
    //Points are on different edges
    int iStartSide = Min(iFirstPointSide,iSecondPointSide);
    int iEndSide = Max(iFirstPointSide,iSecondPointSide);

    pt = *ppta->GetAt(iStartSide);
    MPoint ptStart = iStartSide == iFirstPointSide?pt1:pt2;
    ///	MPoint ptEnd = iEndSide == iFirstPointSide?pt1:pt2;
    dRetVal = sqrt((pt.x - ptStart.x)*(pt.x - ptStart.x)+(pt.y - ptStart.y)*(pt.y - ptStart.y));
    for(int i = iStartSide;i<iEndSide;i++){
        MPoint* pptst = ppta->GetAt(i);
        MPoint* pptend = ppta->GetAt(i+1);
        dRetVal += sqrt((pptst->x - pptend->x)*(pptst->x - pptend->x)+(pptst->y - pptend->y)*(pptst->y - pptend->y));
    }
    pt = *ppta->GetAt(iEndSide);
    dRetVal+= sqrt((pt.x - ptStart.x)*(pt.x - ptStart.x)+(pt.y - ptStart.y)*(pt.y - ptStart.y));
    if(iStartSide == iFirstPointSide)
        return dRetVal;
    else
        return -dRetVal;
}

// ==================================================================
//		Counts coordinates of a point for which the way on perimeter is equal
//to the transferred value
//! ----- 
bool MTopobject::GetPointByPerimeter(double dLengthFromStart,MPoint &pt,int &iSideNum)

// ==================================================================
{
    double dLength = 0;
    MPtArray* ppta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int iSize =  ppta->GetSize();
    int iCounter = 0;
    if(iSize<2)
        return false;
    MPoint *pt1,*pt2;
    do{
        pt1 = ppta->GetAt(iCounter);
        pt2 = ppta->GetAt(iCounter+1);
        dLength += sqrt((pt1->x - pt2->x)*(pt1->x - pt2->x)+(pt1->y - pt2->y)*(pt1->y - pt2->y));
        iCounter++;
        if(iCounter>=iSize-1)
            return false;
    }while(dLength<dLengthFromStart);
    //Now we know a segment on which there is a point
    iSideNum = iCounter-1;
    double dLastSegmentLength = sqrt((pt1->x - pt2->x)*(pt1->x - pt2->x)+(pt1->y - pt2->y)*(pt1->y - pt2->y));
    double dFromStartOfLastSegmentLength = dLengthFromStart - (dLength - dLastSegmentLength);
    double koeff = dFromStartOfLastSegmentLength/dLastSegmentLength;
    pt.x = (pt1->x + (pt2->x - pt1->x)*koeff);
    pt.y = (pt1->y + (pt2->y - pt1->y)*koeff);
    return true;
}

//===================================
//!Returns the pointer on a point
MPoint* MTopobject::GetpPoint(int i)
//====================================
{
    int index=i;
    MPtArray* pta=GetArrayOfPoint(index);
    if(pta)
    {
        MPoint* pt=pta->GetAt(index);
        return pt;
    }
    else
    {
        return NULL;
    }

}

//=============================================================================
//!Returns a point for editing
/** 'point' - the center of area of search of a point
//'dopusk' - search area radius
//'regim' - the returned mode of editing a point
//'kind' - the returned point type
//result - the index on the edited point
*/
MPoint* MTopobject::GetPointForEdit(MPoint point, double dopusk, int& regim, int& kind)
//=============================================================================
{
    //we will find at first a point on a contour
    MPoint* ppoint = NULL;
    MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    int pCount = pta->GetSize();
    MPoint* pt;
    int i = 0;
    for(i=0;i<pCount;i++)
    {
        pt = pta->GetAt(i);
        if(pt->Distance(&point)<dopusk)
        {
            ppoint = pt;
        }
    }
    //if found
    if(ppoint)
    {
        regim = MOVE_POINT_ANYWHERE;
        kind = KONTUR_POINT;
        return ppoint;
    }

    //now we will look among signatures
    MPvArray* privs = (MPvArray*)m_Komplekt->GetAt(F_FORMPRIV);
    MPriv* priv;
    for(i=0; privs->GetSize(); i++){
        priv = privs->GetAt(i);
        MPoint* pta = priv->GetPoints();
        bool bQuit = false;
        for(int kk = 0;kk<priv->GetPointNum();kk++){
            pt = pta+i;
            if(pt->Distance(&point)<dopusk)
            {
                bQuit = true;
                ppoint = pt;
                break;
            }
        }
        if(bQuit) break;
    }


    //if found
    if(ppoint)
    {
        regim = MOVE_POINT_ANYWHERE;
        kind = PODPIS_POINT;
        return ppoint;
    }


    //if didn't find any point, we return NULL
    return NULL;

}

//============================================================================================================================
//!Moving of a point to the set place
/** 'whatpoint' - the index on a point which we move
//'kind' - point type
//'whereput', 'nomrebra', 'nachalosdvig', 'nomrebra', 'rebrosdvig' - ways of a binding
*/
void      MTopobject::MovePointTo(MPoint* whatpoint ,int kind, MPoint whereput, int /*nomrebra*/, double /*nachalosdvig*/, double /*rebrosdvig*/)
//============================================================================================================================
{
    if(!whatpoint)  return; //exit(15);
    //watching what type of a point
    switch(kind)
    {
    case KONTUR_POINT:
    case PODPIS_POINT:
    {
        whatpoint[0] = whereput;
        break;
    }
    }

    RecalcBounds();
}

//=========================================================
//!Removal of a point
void MTopobject::DeletePt(MPoint* whatpoint, int kind)
//=========================================================
{
    if(!whatpoint)   return; // exit(15);
    //watching what type of a point
    switch(kind)
    {
    case KONTUR_POINT:
    {
        //we will find this point
        MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
        int index = pta->GetSize()-1;
        if(index == 0)index = -1;
        bool flag = true;
        while((index>-1)&&(flag))
        {
            if(pta->GetAt(index)==whatpoint)
            {
                flag = false;
            }
            else
            {
                index--;

            }
        }
        if(index!=-1) pta->Delete(index);
    }
        break;
    case PODPIS_POINT:
        break;

    }
    RecalcBounds();
}


// ==================================================================
//!Function which receives value of numerical parameter in attributes
/**the type of the gas pipeline is transferred in a line look
//If parameter can't be transformed to number comes back 0
*/
double MTopobject::GetDoubleDataPoleByName(MString str)
// ==================================================================
{
    MString val = GetDataPoleByName(str);
    if(val.IsEmpty())
        return false;
    double dRet;
    val.TrimLeft();
    val.TrimRight();
    // We make replacement of a comma on a point (just in case)
    int iZapPos = val.Find(",");
    if(iZapPos!=-1)
        val.SetAt(iZapPos,'.');
    dRet = atof((char *)(val));
    return dRet;
}

//========================================================
//!Obtaining numerical value of a code if it exists
bool MTopobject::GetDoubleDataPole(unsigned short polekod,double& val)
//========================================================
{
    MString result = "";
    MShInArray* noms = m_pZnak->GetNomsArray();
    //the size of the array of lines from noma of names of fields
    int Size = noms->GetSize();
    int i;
    //we will check, whether the entrance nome lies in the array of admissible codes
    for (i=0; i<Size; i++)
    {
        if (noms->GetAt(i)[0]==polekod){
            break;
        }
    }

    //if the entrance nome lies in the array of admissible codes
    if(i<Size){
        //if in an array of characteristics of top object values of the characteristic lie with
        //entrance Nome, we will receive them
        int index = ((MNvArray*)m_Komplekt->GetAt(F_NADPIS))->GetValue(polekod, result);

        if((index== -1)||(result.IsEmpty()))
            return false;

        result.TrimLeft();
        result.TrimRight();
        // We make replacement of a comma by a point (just in case)
        int iZapPos = result.Find(",");
        if(iZapPos!=-1)
            result.SetAt(iZapPos,'.');
        val = atof((char *)(result));

        return true;
    }
    else
        return false;
}

//=======================================================================
//! Calculation of "shadow" of top object
MPtArray* MTopobject::GetShadePtArray(MPtArray* pta,double distofaxe)
//=======================================================================
{
    //we will create two arrays
    //'ptar' - axial
    //'ptarr' - covering

    //if points less than two, anything also we don't draw
    if(pta->GetSize()<2)return NULL;
    MPtArray* ptar=(MPtArray*)pta->Clone();
    //we modify type
    //if linear object
    //contains identical initial and final points,
    //that we draw it, as areal
    int type=m_pZnak->GetYaz()->GetType();
    if((type==F_LIN)&&
            (pta->GetAt(0)->x==pta->GetAt(pta->GetSize()-1)->x)&&
            (pta->GetAt(0)->y==pta->GetAt(pta->GetSize()-1)->y))
        type=F_PLO;

    //**************************************************************
    //we create the line for drawing

    //we will build the line of logical coordinates taking into account shift and like top object
    MPtArray* ptarr;
    //if distance from an axis zero,  we copy the initial array
    if(distofaxe==0)
    {
        ptarr=(MPtArray*)pta->Clone();
        if(m_pZnak->GetYaz()->GetType()==F_PLO)
        {
            ptarr->AddTail(pta->GetAt(0));
            ptar->AddTail(pta->GetAt(0));
        }
    }
    //otherwise we build the bypassing contour
    else
    {

        ptarr=new MPtArray();
        MPoint *pt1 =NULL,  *pt2 =NULL,  *pt3 =NULL;

        //******************************************************
        //Cleaning of the array

        //we will clean the array so that there were no in a row points equal
        //or three points lying on one straight line in a row
        //cleaning happens in spetsilno the array formed for this purpose
        //the initial array remains invariable


        int ii=0;
        //we pass all array, since a zero point
        while(ii<ptar->GetSize()-1)
        {
            if(ptar->GetSize()==1)break;
            //we receive the first two points
            pt1=ptar->GetAt(ii);
            pt2=ptar->GetAt(ii+1);
            //if the first two points are equal, we delete the second
            if((fabs(pt1->x-pt2->x)<0.01)&&(fabs(pt1->y-pt2->y)<0.01))

            {
                ptar->Delete(ii+1);
            }
            //otherwise if it is a penultimate point, we pass to the last
            else if(ii==ptar->GetSize()-2)
            {
                ii++;
            }
            //otherwise we take the third point
            else
            {
                pt3=ptar->GetAt(ii+2);
                //if three points lie on one straight line, we delete average
                if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.0001)
                {
                    ptar->Delete(ii+1);
                }
                //otherwise we pass to the subsequent point
                else
                {
                    ii++;
                }
            }
        }

        //if at us closed top object, we check still the three
        //points : (n-2,n-1,0) and (n-1,0,1)
        if(type==F_PLO)
        {
            bool bb=true;
            //we check the first three
            while(bb)
            {
                //if it is more than two points
                if(ptar->GetSize()>2)
                {
                    //we receive points
                    pt1=ptar->GetAt(ptar->GetSize()-2);
                    pt2=ptar->GetAt(ptar->GetSize()-1);
                    pt3=ptar->GetAt(0);
                    //if the tail and the head are equal, we delete a tail
                    if((fabs(pt2->x-pt3->x)<0.01)&&(fabs(pt2->y-pt3->y)<0.01))
                    {
                        ptar->DeleteTail();
                    }
                    //else
                    else
                    {
                        //if three points on one straight line, we delete a tail
                        if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.001)
                        {
                            ptar->DeleteTail();
                        }

                        //else it's OK
                        else
                        {
                            bb=false;
                        }
                    }
                }
                //if it is less than three points, we check nothing
                else
                {
                    bb=false;
                }
            }

            bb=true;

            //we check the second three
            while(bb)
            {
                //if it is more than two points
                if(ptar->GetSize()>2)
                {
                    //we receive points
                    pt1=ptar->GetAt(ptar->GetSize()-1);
                    pt2=ptar->GetAt(0);
                    pt3=ptar->GetAt(1);
                    //if the tail and the head are equal, we delete a tail
                    if((fabs(pt2->x-pt1->x)<0.01)&&(fabs(pt1->y-pt2->y)<0.01))
                    {
                        ptar->DeleteTail();
                    }
                    //else
                    else
                    {
                        //if three points on one straight line, we delete a head
                        if(fabs((pt2->x-pt1->x)*(pt3->y-pt2->y)-(pt3->x-pt2->x)*(pt2->y-pt1->y))<0.001)
                        {
                            ptar->DeleteHead();
                        }
                        //else it's OK
                        else
                        {
                            bb=false;
                        }
                    }
                }
                //if it is less than three points, we check nothing
                else
                {
                    bb=false;
                }
            }

        }

        //******************************************************

        //we build the bypassing contour

        double A1,B1,C1,A2,B2,C2;
        int m_nPoints=ptar->GetSize();
        //at once we take away the necessary quantity of elements for the bypassing contour
        if((m_nPoints==2)||(type==F_LIN))
            ptarr->SetSize(m_nPoints);
        else
            ptarr->SetSize(m_nPoints+1);

        //if we have only two points, build the segment parallel to the initial
        if(m_nPoints==2)
        {
            pt1=ptar->GetAt(0);
            pt2=ptar->GetAt(1);
            MPoint point1=  GetPerpendikular(pt1[0],pt2[0],pt1[0],distofaxe);
            ptarr->SetAt(0,&point1);
            MPoint point2=  GetPerpendikular(pt1[0],pt2[0],pt2[0],distofaxe);
            ptarr->SetAt(1,&point2);

        }
        //otherwise we find the covering contour
        else
        {
            //if the object linear, the first point lies simply on a perpendicular
            if(type==F_LIN)
            {
                pt1=ptar->GetAt(0);
                pt2=ptar->GetAt(1);
                MPoint point= GetPerpendikular(pt1[0],pt2[0],pt1[0],distofaxe);
                ptarr->SetAt(0,&point);
            }
            //further from zero to a point, the second since the end, we build contour points on crossings of straight lines
            pt1=ptar->GetAt(0);
            pt2=ptar->GetAt(1);
            GetPramuju(pt1[0],pt2[0],distofaxe,A1,B1,C1);
            double dlx,dly,dl;
            MPoint pt;

            for(int kk=0;kk<ptar->GetSize()-2;kk++)
            {
                //we receive three points
                //				pt1=ptar->GetAt(kk);
                pt2=ptar->GetAt(kk+1);
                pt3=ptar->GetAt(kk+2);
                //we find a straight line of the second couple of points
                GetPramuju(pt2[0],pt3[0],distofaxe,A2,B2,C2);
                //we find crossing of two straight lines
                dlx=B1*C2-B2*C1;
                dly=A2*C1-A1*C2;
                dl=A1*B2-A2*B1;
                pt.x=dlx/dl;
                pt.y=dly/dl;
                ptarr->SetAt(kk+1,&pt);
                //the equation of the first straight line in the following couple will be
                //same, as well as equation of the second straight line now
                A1=A2;
                B1=B2;
                C1=C2;
            }
            //if object linear, we put the last end as well as zero
            if(type==F_LIN)
            {
                MPoint point= GetPerpendikular(pt2[0],pt3[0],pt3[0],distofaxe);
                ptarr->SetAt(ptar->GetSize()-1,&point);
            }
            //otherwise we will finish finishing a contour
            else
            {
                pt1=ptar->GetAt(m_nPoints-2);
                pt2=ptar->GetAt(m_nPoints-1);
                pt3=ptar->GetAt(0);

                //three n-2,n-1,0
                GetPramuju(pt1[0],pt2[0],distofaxe,A1,B1,C1);
                GetPramuju(pt2[0],pt3[0],distofaxe,A2,B2,C2);
                //we find crossing of two straight lines
                dlx=B1*C2-B2*C1;
                dly=A2*C1-A1*C2;
                dl=A1*B2-A2*B1;
                pt.x=dlx/dl;
                pt.y=dly/dl;
                ptarr->SetAt(ptar->GetSize()-1,&pt);

                //three n-1,0,1
                pt1=ptar->GetAt(1);
                GetPramuju(pt2[0],pt3[0],distofaxe,A1,B1,C1);
                GetPramuju(pt3[0],pt1[0],distofaxe,A2,B2,C2);
                //we find crossing of two straight lines
                dlx=B1*C2-B2*C1;
                dly=A2*C1-A1*C2;
                dl=A1*B2-A2*B1;
                pt.x=dlx/dl;
                pt.y=dly/dl;
                ptarr->SetAt(0,&pt);

                //we close arrays
                pt1=ptarr->GetAt(0);
                pt.x=pt1->x;
                pt.y=pt1->y;
                ptarr->SetAt(ptarr->GetSize()-1,&pt);
                pt1=ptar->GetAt(0);
                pt.x=pt1->x;
                pt.y=pt1->y;
                ptar->AddTail(&pt);
            }
        }

    }

    delete ptar;
    return ptarr;
}

//======================================================
//!we will receive the most distant point of a figure from a straight line (pt1, pt2)
MPoint MTopobject::GetFarPoint(MPoint* pt1, MPoint* pt2)
//======================================================
{
    MPoint point;
    double perpendikular=0;
    MPoint point1 = *pt1;
    MPoint point2 = *pt2;
    for (int i=0; i<GetPointsNumber(); i++)
    {
        MPoint tekPoint = GetPoint(i);
        if ((tekPoint != point1)&&(tekPoint != point2))
        {
            if (perpendikular < RASSTPTL(tekPoint, point1, point2))
            {
                perpendikular = RASSTPTL(tekPoint, point1, point2);
                point = tekPoint;
            }
        }
    }
    return point;
}

//=======================================================
//!figure points of intersection from a straight line (pt1, pt2)
bool MTopobject::CrossLineFig(MPoint* pt1, MPoint* pt2, MPtArray* pta)
//=======================================================
{
    MPtArray* ptarr = GetPointArray();
    int kolvo = ptarr->GetSize();
    MPoint ptOne;
    MPoint ptTwo;
    for (int i=0; i<kolvo ; i++)
    {
        if (i != kolvo-1)
        {
            ptOne = GetPoint(i);
            ptTwo = GetPoint(i+1);
        }
        else
        {
            ptOne = GetPoint(i);
            ptTwo = GetPoint(0);
        }
        MPoint ptCross;
        //if the straight line crossed a figure
        double dTemp;
        int result = IntersectLineWithOtrezok(ptOne,ptTwo,pt1, pt2, &ptCross,dTemp);
        if ((result == 0)||(result == 10) || (result == 11)||(result == 3)||(result == 5)||(result == 9))
        {
            //we receive a point of intersection and we bring it to the array
            pta->AddTail(&ptCross);
        }
    }
    if (pta->GetSize() == 0) return false;
    else return true;
}

//=====================================================
//!Function overturns an order of points on the return
void MTopobject::ReverseOrder()
//=====================================================
{
    MPtArray* pta = (MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    MPtArray* ptNew = new MPtArray();
    for(int i = pta->GetSize()-1;i>=0;--i)
        ptNew->AddTail(pta->GetAt(i));
    m_Komplekt->SetAt(F_TOCHKI,ptNew);
    delete pta;
}

//!Turn of correctly adjusted font cut
void MTopobject::RotateShriftNastr(double dAngle)
{
    if(m_pZnak->GetYaz()->GetType()!=F_DOT)
        return;
    MDotYaz* pYaz = (MDotYaz*)m_pZnak->GetYaz()->m_object;
    for(int i = pYaz->m_tochka->GetSize()-1;i>=0;--i){
        //in each cut
        MSrez* pSrez = pYaz->m_tochka->GetAt(i);

        if(!pSrez->IsPereopr())
            continue;
        MNastrShriftSrez* pSz = (MNastrShriftSrez*)pSrez;
        if(!pSz->IsPereoprIndex(IDT_NAS_SHRIFT_VAL_SYM_UGOL))
            continue;
        short perIndex = pSz->GetPerIndex(IDT_NAS_SHRIFT_VAL_SYM_UGOL);
        if(perIndex == -1)
            continue;
        MString strOld = GetDataPole(perIndex);
        double old = atof((char *)strOld);
        MString str;
        str.Format("%f",old+dAngle);
        SetDataPole(str,perIndex);
    }


}

//==================================
// Reading points of object or subobject.
bool MTopobject::ReadSxfPoints( FILE* FileSxf,
                                unsigned short int NotationDimension, // dimension
                                unsigned short int MetricElementType, // type
                                unsigned short int MetricElementSize, // size
                                unsigned short int PointNumber,		// number of points
                                bool revXY)							// reverse of coordinates
//==================================
{
    MPoint* points = (MPoint*) malloc(sizeof(MPoint)*PointNumber);

    int byte_kol;

    // two-dimensional representation
    if (NotationDimension==0) {
        // integer type
        if (MetricElementType==0) {
            // size 2 bytes
            if (MetricElementSize==0) {
                byte_kol = PointNumber*sizeof(SHTPOINT);
                SHTPOINT* pts = (SHTPOINT*)malloc(byte_kol);
                //UINT uin = FileSxf->Read(pts, byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf) != byte_kol) {
                    //AfxMessageBox(IDS_MESSAGE16); //"Error While Reading)Can't Read Metric of SXF-File."
                    free(pts);
                    //                    delete pts;
                    return false;
                    //return true;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
            // size 4 bytes
            else {
                byte_kol = PointNumber*sizeof(INTPOINT);
                INTPOINT* pts = (INTPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    free(pts);
                    //                    delete pts;
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
        }
        // float type
        else {
            // size 4 bytes
            if (MetricElementSize==0) {
                byte_kol = PointNumber*sizeof(FLTPOINT);
                FLTPOINT* pts = (FLTPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    free(pts);
                    //                    delete pts;
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
            // size 8 bytes
            else {
                byte_kol = PointNumber*sizeof(DBLPOINT);
                DBLPOINT* pts = (DBLPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    free(pts);
                    //                    delete pts;
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
        }
    }//end of two-dimensional representation
    // three-dimensional representation
    else {
        // integer type
        if (MetricElementType==0) {
            // size 2 bytes
            if (MetricElementSize==0) {
                byte_kol = PointNumber*sizeof(H_SHTPOINT);
                H_SHTPOINT* pts = (H_SHTPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    free(pts);
                    //                    delete pts;
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
            // size 4 bytes
            else {
                byte_kol = PointNumber*sizeof(H_INTPOINT);
                H_INTPOINT* pts = (H_INTPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    free(pts);
                    //                    delete pts;
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
        }
        // float type
        else {
            // size 4 bytes
            if (MetricElementSize==0) {
                byte_kol = PointNumber*sizeof(H_FLTPOINT);
                H_FLTPOINT* pts = (H_FLTPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    free(pts);
                    //                    delete pts;
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
            // size 8 bytes
            else {
                byte_kol = PointNumber*sizeof(H_DBLPOINT);
                H_DBLPOINT* pts = (H_DBLPOINT*)malloc(byte_kol);
                if(fread(pts, 1, byte_kol, FileSxf)!=byte_kol) {
                    free(pts);
                    //                    delete pts;
                    //AfxMessageBox("Error While Reading)Can't Read Metric of SXF-File.");
                    return false;
                }

                if (!revXY) {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].x;
                        points[i].y = pts[i].y;
                    }
                }
                else {
                    for(int i = 0; i<PointNumber; i++) {
                        points[i].x = pts[i].y;
                        points[i].y = pts[i].x;
                    }
                }

                free(pts);
                //                delete pts;
            }
        }
    }//end of three-dimensional representation

    // installation of points at once
    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);
    //	pta->FastSet(&points[0]);
    pta->FastSet(PointNumber,&points[0]);
    free(points);
    //    delete points;
    return true;
}

//bool MTopobject::WriteSxfPoints(FILE *FileSxf, const QString &classificalCode, bool revXY)
//{
//    static unsigned short id = 0;
//    SXFHEAD_4 sxf_head;// Заголовок записи
//    sxf_head.RecordLabel =0x7FFF7FFF;// 2147450879;// ИДЕНТИФИКАТОР НАЧАЛА ЗАПИСИ (0x7FFF7FFF)
//    sxf_head.MemberNumber = id;
//    id++;
//    //sxf_head.MemberNumber = m_UID;
//    sxf_head.UnionNumber = 0;
//    sxf_head.ClassificalCode = classificalCode.toInt(); //61320000;// КЛАССИФИКАЦИОННЫЙ КОД
//    sxf_head.MultyPoligon = 0;//1 – подобъекты полигона могут быть вне объекта
//    sxf_head.CompressionSign = 0;//1 – знак графического объекта может сжиматься
//    sxf_head.PresenceOfSemantics = 0;//xххххх0х - нет семантики; хххххх1х - есть семантика
//    sxf_head.ReferenceVector = 0;//xххххх0х - нет описания вектора привязки; хххххх1х – за записью метрики идет описание вектора привязки 3D-модели.
//    sxf_head.UNICODEFlag = 0;//0 – однобайтовый текст,  1 – UNICODE.
//    sxf_head.AboveAllFlag = 0;//1 – отобразить над всеми
//    sxf_head.BelowAllFlag = 0;//1 – отобразить под всеми
//    sxf_head.SubobjectsVerticalAlignment = 1;//1 – выравнивать для векторных объектов
//    sxf_head.Bot = 0xFF;//-1
//    sxf_head.Top = 0xFF;//-1
//     // ОПИСАТЕЛЬ МЕТРИКИ
//    sxf_head.SubObjectNumber = GetSubObjectCount();// Число подобъектов
//    sxf_head.PointMetricNumber = (unsigned short)(((MPtArray*)m_Komplekt->GetAt(F_TOCHKI))->Size); // Число точек метрики
//    if(sxf_head.PointMetricNumber==0)
//        return false;
//    sxf_head.MetricElementSize = 1;// Размер элемента метрики
//    //xхххх0xх -  2 байта (для целочисленного значения);
//    //xхххх0xх -  4 байта (для плавающей точки);
//    //ххххх1xх -  4 байта (для целочисленного значения);
//    //ххххх1xх -  8 байт  (для плавающей точки).

//    sxf_head.MetricElementType =1; // Тип элемента метрики
//    //ххххх0хх - метрика представлена в виде целых чисел; ххххх1хх - представление с плавающей точкой.
//    sxf_head.MetricWithTextFlag = 0;    //ххxx0xхх - метрика  содержит  только  координаты  точек;
//                                        //ххxx1хxх - метрика  содержит  текст подписи, допускается только для объектов типа "подпись" или "шаблон подписи"
//   sxf_head.GraphicsAvailability = 0;//хxx0xххх - запись объекта не содержит графическое описание объекта;
//   //хxx1xххх - за метрикой  объекта идет графическое описание объекта                 (условного знака).
//    sxf_head.GraphicsScalability = 1;//xx0хxххх - условный знак объекта не масштабируемый; xx1хxххх - условный знак объекта масштабируется при отображении.
//    sxf_head.ConstructingSplineByMetric = 0;//00xxxххх – построение сплайна при визуализации не выполняется; 01xхxххх – сглаживающий сплайн (срезание углов); 10xхxххх – огибающий сплайн (проходит через все точки метрики).
//    switch(m_pZnak->GetYaz()->GetType())
//    {
//    case (F_LIN):
//        sxf_head.Local =0;// Характер локализации
//        //xххх0000 – линейный;
//        //ххххc - площадной;
//        //хххх0010 - точечный
//        //xxxx0011 – подпись.
//        //(для SXF версий 3.0 и старше): xххх0100 - векторный (точечный ориентированный объект, содержит две точки в метрике);
//        //xххх0101 - шаблон подписи - первая точка метрики является точкой привязки шаблона, метрика подобъектов задает расположение подписей и вспомогательных  линий ("пустые подписи").
//        break;
//    case (F_PLO):
//        sxf_head.Local = 0/*1*/;
//        sxf_head.PresenceOfSemantics =1;
////       if(sxf_head.SubObjectNumber>0)
////           sxf_head.SubObjectNumber++;
//          break;
//    case (F_DOT):
//        sxf_head.Local = 4;
//       if(sxf_head.PointMetricNumber < 2)
//           sxf_head.PointMetricNumber = 2;
//        break;
//    default:
//        sxf_head.Local = 6;
//    }



//    sxf_head.MetricRecordFormat=0; // Формат записи метрики
//    //xхххххx0 - метрика записана в линейном формате;
//    //ххххххx1 - метрика записана в векторном формате.
//    sxf_head.NotationDimension =0;  // Размерность представления
//                                    //xххххх0х - объект имеет двухмерное представление;
//                                    //xххххх1х - объект имеет трехмерное пpедстaвление.


//SEMANTIC ObjSemantic(790,4,2,0);
//    MPtArray* pta=(MPtArray*)m_Komplekt->GetAt(F_TOCHKI);//  метрики объекта

//    int byte_kol;
//    // двумерное представление
//    if (sxf_head.NotationDimension==0) {
//        // целочисленный тип
//        if (sxf_head.MetricElementType==0) {
//            // размер 2 байта
//            if (sxf_head.MetricElementSize==0) {
//                byte_kol = sxf_head.PointMetricNumber*sizeof(SHTPOINT);
//                sxf_head.MetricLength = byte_kol;// ДЛИНА МЕТРИКИ В БАЙТАХ

//                SHTPOINT* pts = (SHTPOINT*)malloc(byte_kol);
//                //UINT uin = FileSxf->Read(pts, byte_kol);


//                if (!revXY) {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                         pts[i].x = pta->GetAt(i)->x;
//                         pts[i].y = pta->GetAt(i)->y;
//                    }
//                }
//                else {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                        pts[i].x = pta->GetAt(i)->y;
//                        pts[i].y = pta->GetAt(i)->x;
//                    }
//                }

//                sxf_head.RecordLength = sizeof(SXFHEAD)+sxf_head.MetricLength;// ОБЩАЯ ДЛИНА ЗАПИСИ (с заголовком)
//                fwrite(&sxf_head,sizeof(SXFHEAD),1,FileSxf);
//                if(fwrite(pts, byte_kol,1, FileSxf)!=1)
//                {
//                    delete pts;
//                    //return false;
//                }
//                else
//                {
//                    delete pts;
//                    //return true;
//                }
//            }
//            // размер 4 байта
//            else {
//                byte_kol = sxf_head.PointMetricNumber*sizeof(INTPOINT);
//                sxf_head.MetricLength = byte_kol;//Длина метрики

//                INTPOINT* pts = (INTPOINT*)malloc(byte_kol);


//                if (!revXY) {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                         pts[i].x = pta->GetAt(i)->x;
//                         pts[i].y = pta->GetAt(i)->y;
//                    }
//                }
//                else {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                        pts[i].x = pta->GetAt(i)->y;
//                        pts[i].y = pta->GetAt(i)->x;
//                    }
//                }
//                sxf_head.RecordLength = sizeof(SXFHEAD)+sxf_head.MetricLength;
//                fwrite(&sxf_head,sizeof(SXFHEAD),1,FileSxf);
//                if(fwrite(pts, byte_kol,1, FileSxf) != byte_kol)
//                {
//                    delete pts;
//                   // return false;
//                }
//                else
//                {
//                    delete pts;
//                    //return true;
//                }
//            }
//        }
//        // плавающий тип
//        else {
//            // размер 4 байта
//            if (sxf_head.MetricElementSize==0) {
//                byte_kol = sxf_head.PointMetricNumber*sizeof(FLTPOINT);
//                sxf_head.MetricLength = byte_kol;//Длина метрики

//                FLTPOINT* pts = (FLTPOINT*)malloc(byte_kol);

//                if (!revXY) {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                         pts[i].x = pta->GetAt(i)->x;
//                         pts[i].y = pta->GetAt(i)->y;

//                    }
//                }
//                else {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                        pts[i].x = pta->GetAt(i)->y;
//                        pts[i].y = pta->GetAt(i)->x;
//                        if(m_pZnak->GetYaz()->GetType()==F_DOT && pta->Size<2)
//                        {
//                            pts[i+1].x = pta->GetAt(i)->x;
//                            pts[i+1].y = pta->GetAt(i)->y;
//                            i++;
//                        }
//                    }
//                }

//                sxf_head.RecordLength = sizeof(SXFHEAD_4)+sxf_head.MetricLength;
//                fwrite(&sxf_head,sizeof(SXFHEAD_4),1,FileSxf);
//                if(fwrite(pts, byte_kol,1, FileSxf) != byte_kol)
//                {
//                    delete pts;
//                   // return false;
//                }
//                else
//                {
//                    delete pts;
//                   // return true;
//                }
//            }
//            // размер 8 байт
//            else {
//                byte_kol = sxf_head.PointMetricNumber*sizeof(DBLPOINT);


//                /*if(sxf_head.PointMetricNumber==1)
//                            byte_kol*=2;*/

//                sxf_head.MetricLength = byte_kol;//Длина метрики

//                for (int i=1;i<m_Komplekt->GetSize();i++)// podobjekty
//                {
//                    if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PHL)
//                    {
//                        int size = (m_Komplekt->GetAt(i))->Size*sizeof(DBLPOINT);
//                       if( size >0)
//                       {
//                           sxf_head.MetricLength+=4;
//                           sxf_head.MetricLength+=size;
//                       }
//                    }
//                }

//               // sxf_head.MetricLength = byte_kol;//Длина метрики

//                DBLPOINT* pts = (DBLPOINT*)malloc(byte_kol);

//                if (!revXY) {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                         pts[i].x = pta->GetAt(i)->x;
//                         pts[i].y = pta->GetAt(i)->y;
//                    }
//                }
//                else {
//                    for(int i = 0; i<sxf_head.PointMetricNumber; i++) {
//                        pts[i].x = pta->GetAt(i)->y;
//                        pts[i].y = pta->GetAt(i)->x;
//                        if(m_pZnak->GetYaz()->GetType()==F_DOT && pta->Size<2)
//                        {
//                            pts[i+1].x = pta->GetAt(i)->x;
//                            pts[i+1].y = pta->GetAt(i)->y;
//                            i++;
//                        }
//                    }

//                }

//                sxf_head.RecordLength = sizeof(SXFHEAD_4)+sxf_head.MetricLength+sizeof(SEMANTIC);
//                fwrite(&sxf_head,sizeof(SXFHEAD_4),1,FileSxf);
//                if(fwrite(pts, byte_kol,1, FileSxf) != 1)
//                {
//                    delete pts;
//                    //return false;
//                }
//                else
//                {
//                    delete pts;
//                   // return true;
//                }
//            }
//        }
//    }//конец двумерное представление

//    // метрика подобъекта
//    qDebug()<<"===========SubObject(hole)==========";
//    for (int i=1;i<m_Komplekt->GetSize();i++)
//    {

//        if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PHL)
//        {
//            MHlArray* pta=(MHlArray*)(m_Komplekt->GetAt(i));
//            SERVICE_FIELD field(pta->Size);
//            fwrite(&field,sizeof(SERVICE_FIELD),1,FileSxf);
//            int byte_kol = field.numPoints*sizeof(DBLPOINT);
//            DBLPOINT* pts = (DBLPOINT*)malloc(byte_kol);
//            if (!revXY) {
//                for(int j = 0; j<field.numPoints; j++) {
//                    pts[j].x = pta->GetAt(j)->x;
//                    pts[j].y = pta->GetAt(j)->y;
//                    qDebug()<<pta->GetAt(j)->x<<";"<<pta->GetAt(j)->y;
//                }
//            }
//            else {
//                for(int j = 0; j<field.numPoints; j++)
//                {
//                    pts[j].x = pta->GetAt(j)->y;
//                    pts[j].y = pta->GetAt(j)->x;

//                    qDebug()<<pta->GetAt(j)->x<<";"<<pta->GetAt(j)->y;
//                }
//            }
//            fwrite(pts, byte_kol,1, FileSxf) ;
//            delete pts;

//        }
//    }

//    fwrite(&ObjSemantic,sizeof(ObjSemantic),1, FileSxf) ;
//}

short MTopobject::GetSubObjectCount()
{
    short count = 0;
    for (int i=1;i<m_Komplekt->GetSize();i++)
    {
        if(m_Komplekt->GetAt(i)->WhatTheArray()==ID_PHL)
            count++;
    }
    return count;
}
