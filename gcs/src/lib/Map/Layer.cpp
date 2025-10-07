//IMPLEMENTATION of functions of a layer of a map

#include <string.h>
#include "Layer.h"
#include "Karta.h"
#include "Klassifikator.h"
#include "PlotDevice.h"
#include "Refglobal.h"

using namespace std;
extern MObject* (*fpCreateObjectByType)(MString str);

char end_while[256] = "END OF THE CYCLE !!!";

//!==================================
void FmpReproject(MRect* rect, double a, double b1, double b2)
//==================================
{
    rect->left = a*rect->left + b1;
    rect->right = a*rect->right + b1;
    rect->top = a*rect->top + b2;
    rect->bottom = a*rect->bottom + b2;
}
//!==================================
void FmpReproject(MPoint* pt, double a, double b1, double b2)
//==================================
{
    pt->x = a*pt->x + b1;
    pt->y = a*pt->y + b2;
}

//===============================================================================
//===============================================================================
//==================================
//! MLayer
MLayer::MLayer()
//==================================
{

    m_dbarray.SetSize(ID_DBINITSIZE);
    m_name = IDS_LAYER;//"Layer"
    m_sort = IDS_ORDINARY ;//"Ordinary"

    // flags
    x_common                   = false;
    x_visible                  = true;
    x_textvisible              = true;
    m_trans_type				= Qt::SolidPattern; //added by back

    m_pKarta                     = NULL;
    m_oblist.clear();

    m_commoncolor_pen.SetColor(0,0,0);
    m_commoncolor_brush.SetColor(0,0,0);
    m_commoncolor_brushTransparent = 255;
    m_commoncolor_penTransparent = 255;
    m_commoncolor_text.SetColor(0,0,0);
    m_commonpenwidth  = 1;
    m_commontextwidth = 1;
    m_bDrawSmall = false;
    m_bOperLayer = false;
    m_bSearchIn = true;

}

//!==================================
MLayer::MLayer(MString key)
//==================================
{
    m_dbarray.SetSize(ID_DBINITSIZE);
    m_name = key;
    m_sort = IDS_ORDINARY;//"Ordinary"

    // flags
    x_common                   = false;
    x_visible                  = true;
    x_textvisible              = true;
    m_trans_type				= Qt::SolidPattern; //added by back

    m_pKarta                     = NULL;
    m_oblist.clear();

    m_commoncolor_pen.SetColor(0,0,0);
    m_commoncolor_brush.SetColor(0,0,0);
    m_commoncolor_brushTransparent = 255;
    m_commoncolor_penTransparent = 255;
    m_commoncolor_text.SetColor(0,0,0);
    m_commonpenwidth  = 1;
    m_commontextwidth = 1;
    m_bDrawSmall = false;
    m_bOperLayer = false;

    m_bSearchIn = true;
    //m_selectedcount = 0;
}

//!==================================
MLayer::MLayer(const MRect& bounds)
//==================================
{
    m_dbarray.SetSize(ID_DBINITSIZE);
    m_name = IDS_LAYER;//"Layer"
    m_sort = IDS_ORDINARY ;//"Ordinary"

    m_bounds = bounds;

    // flags
    x_common                   = false;
    x_visible                  = true;
    x_textvisible              = true;
    m_trans_type				= Qt::SolidPattern; //added by back

    m_pKarta                     = NULL;
    m_oblist.clear();

    m_commoncolor_pen.SetColor(0,0,0);
    m_commoncolor_brush.SetColor(0,0,0);
    m_commoncolor_brushTransparent = 255;
    m_commoncolor_penTransparent = 255;
    m_commoncolor_text.SetColor(0,0,0);
    m_commonpenwidth  = 1;
    m_commontextwidth = 1;
    m_bDrawSmall = false;
    m_bOperLayer = false;

    m_bSearchIn = true;
    //m_selectedcount = 0;
}

//!==================================
MLayer::~MLayer()
//==================================
{
    OBITER it;
    while(!m_oblist.empty())
    {
        it = m_oblist.begin();
        MObject* pObj = *it;
        delete pObj;
        m_oblist.pop_front();
    }
    m_dbarray.ClearAll();
    /*    2004.11.19  vipa
  MString pStr;
    for(int i=0; i<m_dbarray.GetSize();i++) {
        pStr = *m_dbarray.GetAt(i);
        m_dbarray.Delete(i);
        pStr.Empty();
    }
*/  
}

//!==================================
bool MLayer::MasterRead(FILE* h_nuxfile,  int version)
//==================================
{
    char		id_file[] = "   "; ///, id_eol[] = "EOL";
    int			param = 0;
    switch( version ) {
    //version 0
    case 0:
        //reading identifier of 'LAY' layer
        fread(&id_file, 3, 1, h_nuxfile);
        if (strcmp(id_file, (char *)WhatTheLayer())) {
            InternalMessage( "File structure corrupted !");
            return false;
        }
        //we pass length of record of a layer and length of the record "passport of a layer"
        fseek(h_nuxfile, 6, SEEK_CUR);
        //			SetFilePointer(h_nuxfile, 2, NULL, FILE_CURRENT);

        //name
        m_name = ReadString(h_nuxfile,  version);
        //type
        m_sort = ReadString(h_nuxfile,  version);
        //comment
        m_remark = ReadString(h_nuxfile,  version);
        //password
        m_password = ReadString(h_nuxfile,  version);
        //unique number of a layer
        unsigned int uid= 0;
        fread(&uid, sizeof(unsigned int), 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&uid, sizeof(uid) );
        m_UID = uid;
        //status of editing layer
        m_editstat= 0;
        fread(&m_editstat,sizeof(unsigned int), 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&m_editstat, sizeof(m_editstat) );

        //reading the describing rectangle
        ReadRect(h_nuxfile,  version, &m_bounds);


        //reading attributes of a layer
        param= 0;
        fread(&param, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&param, sizeof(param) );
        x_common = param & 1;
        param = param >> 1;
        x_visible = param & 1;
        param = param >> 1;
        x_textvisible = param & 1;

        ReadStrArray(h_nuxfile,  version, &m_dbarray);

        break;
    }

    return true;
}

//!==================================
bool MLayer::MasterRead(MFile* file,  int version)
//==================================
{
    char		id_file[] = "   "; ///, id_eol[] = "EOL";
    int			param = 0;
    switch( version ) {
    //version 0
    case 0:
        //reading identifier of 'LAY' layer
        file->GetData(&id_file, 3, 1);
        if (strcmp(id_file, (char *)WhatTheLayer())) {
            InternalMessage( "File structure corrupted !");
            return false;
        }
        //we pass length of record of a layer and length of the record "passport of a layer"
        file->SetPosition(6, MAP_SEEK_CUR);
        //			SetFilePointer(h_nuxfile, 2, NULL, FILE_CURRENT);

        //name
        m_name = ReadString(file,  version);
        //type
        m_sort = ReadString(file,  version);
        //comment
        m_remark = ReadString(file,  version);
        //password
        m_password = ReadString(file,  version);
        //unique number of a layer
        unsigned int uid= 0;
        file->GetData(&uid,sizeof(unsigned int), 1);
        if (reverseReadFlag) reverseValue((char*)&uid, sizeof(uid) );
        m_UID = uid;
        //status of editing layer
        m_editstat= 0;
        file->GetData(&m_editstat,sizeof(unsigned int), 1);
        if (reverseReadFlag) reverseValue((char*)&m_editstat, sizeof(m_editstat) );

        //reading the describing rectangle
        ReadRect(file,  version, &m_bounds);


        //reading attributes of a layer
        param= 0;
        file->GetData(&param, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&param, sizeof(param) );
        x_common = param & 1;
        param = param >> 1;
        x_visible = param & 1;
        param = param >> 1;
        x_textvisible = param & 1;

        ReadStrArray(file,  version, &m_dbarray);

        break;
    }

    return true;
}

//!==================================
bool MLayer::ReadBin(FILE* h_nuxfile,  int version)
//==================================
{
    MasterRead(h_nuxfile,version);

    char		id_file[] = "   ";
    MObject*    pObj;
    unsigned short    ObCnt=0;
    switch( version ) {
    //version 0
    case 0:
        //indication
        MString	panetext;
        ///			MKarta*   pKarta = m_pKarta;

        int p = ftell(h_nuxfile);

        fseek(h_nuxfile, 0, SEEK_END);
        int	filesize = ftell(h_nuxfile);
        fseek(h_nuxfile, p, SEEK_SET);

        int proc = (int)(100.0*(double)p/(double)filesize);
        GetKarta()->SetProgress(proc);
        GetKarta()->ShowProgress();

        MString kod;
        MString realkod;
        ///			MKlassifikator* pKls = pKarta->m_pKls;

        //we read amount of objects
        fread(&ObCnt, 2, 1, h_nuxfile);
        if (reverseReadFlag) reverseValue((char*)&ObCnt, sizeof(ObCnt) );

        m_oblist.clear();

        short kkod_size;
        MString kkod_str;

        for(int i = 0; i<ObCnt; i++)
        {
            //reading identifier of object
            fread(&id_file, 3, 1, h_nuxfile);

            //we will look quad-code if not that, we don't read.
            fseek(h_nuxfile, 3, SEEK_CUR);
            unsigned int rec_size=0;
            fread(&rec_size, 4, 1, h_nuxfile);
            if (reverseReadFlag) reverseValue((char*)&rec_size, sizeof(rec_size) );

            //we will read the size of a quad-code
            kkod_size= 0;
            fread(&kkod_size, 2, 1, h_nuxfile);
            if (reverseReadFlag) reverseValue((char*)&kkod_size, sizeof(kkod_size) );
            //file->GetData(&kkod_size, 2, 1);
            if(kkod_size != 0)
            {
                //we read quad-code
                char* str = new char[kkod_size+1];
                str[kkod_size]= '\0';
                memset(str, ' ', kkod_size);
                fread(str, kkod_size, 1, h_nuxfile);
                kkod_str = str;
                delete[] str;
                //                    delete str;

                //we check quad-code
                if(!m_pKarta->FindKkodInLoadFilter(kkod_str))
                {
                    fseek(h_nuxfile, rec_size-(7+2+kkod_size), SEEK_CUR);
                    continue;
                }
                else
                    fseek(h_nuxfile, -(7+2+kkod_size), SEEK_CUR);
            }
            else
                fseek(h_nuxfile, -(7+2), SEEK_CUR);

            pObj = (*fpCreateObjectByType)(id_file);
            pObj->m_pLay = this;
            if(pObj)
                pObj->ReadBin(h_nuxfile,version);
            else
                return false;
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                ((MTopobject*)pObj)->ChangeClass(((MTopobject*)pObj)->m_kkod);
            m_oblist.push_back(pObj);
        }
        break;
    }
    return true;
}

//!==================================
bool MLayer::ReadBin(MFile* file,  int version)
//==================================
{
    MasterRead(file,version);

    char		id_file[] = "   ";
    MObject*    pObj;
    unsigned short   ObCnt=0;
    switch( version ) {
    //version 0
    case 0:
        //indication
        MString	panetext;
        ///			MKarta*   pKarta = m_pKarta;

        int p = file->GetPosition();

        int	filesize = file->GetSize();

        int proc = (int)(100.0*(double)p/(double)filesize);
        GetKarta()->SetProgress(proc);
        GetKarta()->ShowProgress();

        MString kod;
        MString realkod;
        ///			MKlassifikator* pKls = pKarta->m_pKls;

        //we read amount of objects
        file->GetData(&ObCnt, 2, 1);
        if (reverseReadFlag) reverseValue((char*)&ObCnt, sizeof(ObCnt) );

        short kkod_size;
        MString kkod_str;

        m_oblist.clear();
        for(int i = 0; i<ObCnt; i++)
        {
            //we will look quad-code if not that and we don't read that.
            file->SetPosition(3, MAP_SEEK_CUR);
            unsigned int rec_size= 0;
            file->GetData(&rec_size, 4, 1);
            if (reverseReadFlag) reverseValue((char*)&rec_size, sizeof(rec_size) );

            //we will read the size of a quad-code
            kkod_size= 0;
            file->GetShort(kkod_size);
            //file->GetData(&kkod_size, 2, 1);
            if(kkod_size != 0)
            {
                //we read quad-code
                char* str = new char[kkod_size+1];
                str[kkod_size]= '\0';
                memset(str, ' ', kkod_size);
                file->GetData(str, kkod_size, 1);
                kkod_str = str;
                delete[] str;
                //                    delete str;

                //we check quad-code
                if(!m_pKarta->FindKkodInLoadFilter(kkod_str))
                {
                    //the quad-codea isn't present among selected by means of the filter
                    file->SetPosition(rec_size-(7+2+kkod_size), MAP_SEEK_CUR);
                    continue;
                }
                else
                    file->SetPosition(-(7+2+kkod_size), MAP_SEEK_CUR);
            }
            else
                file->SetPosition(-(7+2), MAP_SEEK_CUR);

            //reading identifier of object
            file->GetData(&id_file, 3, 1);
            /*
                //if object of not set type we pass it
                //we read record length
                int rec_size;
                file->GetLong(rec_size);
                //we miss record
                file->SetPosition(rec_size-7-4, MAP_SEEK_CUR);
                int kkod_size;
                file->GetLong(kkod_size);
                file->SetPosition(-(kkod_size+2), MAP_SEEK_CUR);
                //we read quad-code
                MString kkod = ReadString(file, version);
                //we check quad-code
                if(kkod == "UZN 2000 RP 185")
                {
                    //we come back and read object
                    file->SetPosition(-(rec_size-3-4), MAP_SEEK_CUR);
                }
                else //we pass object
                    continue;

*/
            pObj = (*fpCreateObjectByType)(id_file);
            pObj->m_pLay = this;
            if(pObj)
                pObj->ReadBin(file,version);
            else
                return false;

            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                ((MTopobject*)pObj)->ChangeClass(((MTopobject*)pObj)->m_kkod);

            m_oblist.push_back(pObj);
        }
        break;
    }
    return true;
}

//=============================
//!the size of the passport of a layer in UTP
unsigned short MLayer::GetPassportBytes()
//=============================
{

    unsigned short SBytes = 0;

    //calculation of length of the record "passport of a layer"
    SBytes = 74;	//2b attributes of a layer
    //32b the describing rectangle
    //4b unique number of a layer
    //4b status of editing
    //32b the describing rectangle
    //2b attributes of a layer

    //db_array array size
    SBytes += 4;
    int kolvo = m_dbarray.GetSize();
    for (int j=0; j<kolvo; j++)
        SBytes += m_dbarray[j]->GetLength() + 2;
    SBytes = SBytes + m_name.GetLength()+2;
    SBytes = SBytes + m_sort.GetLength()+2;
    SBytes = SBytes + m_remark.GetLength()+2;
    SBytes = SBytes + m_password.GetLength()+2;

    return SBytes;
}
//!==================================
bool MLayer::MasterWrite(FILE* h_nuxfile,  int version)
//==================================
{
    unsigned short	i;
    unsigned short	param = 0;

    switch( version ) {
    //version 0
    case 0:
        //record of the identifier of 'LAY' layer
        fwrite((char *)WhatTheLayer(), 3, 1, h_nuxfile);

        //calculation of length of the record "passport of a layer"
        i = GetPassportBytes();

        //writing of length of record of a layer
        unsigned int laylength = 0;
        fwrite(&laylength, 4, 1, h_nuxfile);
        //writing of length of the record "passport of a layer"
        fwrite(&i, 2, 1, h_nuxfile);
        //name
        WriteString(h_nuxfile,  version, m_name);
        //type
        WriteString(h_nuxfile,  version, m_sort);
        //commenr
        WriteString(h_nuxfile,  version, m_remark);
        //password
        WriteString(h_nuxfile,  version, m_password);
        //unique number of a layer
        unsigned int uid = (unsigned int)m_UID;
        fwrite(&uid, sizeof(unsigned int), 1, h_nuxfile);
        //status of editing layer
        fwrite(&m_editstat, sizeof(unsigned int), 1, h_nuxfile);

        //writing of the describing rectangle
        WriteRect(h_nuxfile,  version, &m_bounds);

        //writing of attributes of a layer
        param = param << 1;
        param = (param | (x_textvisible!=0)) << 1;
        param = (param | (x_visible!=0)) << 1;
        param = (param | (x_common!=0));
        fwrite(&param, 2, 1, h_nuxfile);

        //writing of the array of lines
        WriteStrArray(h_nuxfile,  version, &m_dbarray);
    }
    return true;
}


//!==================================
bool MLayer::WriteBin(FILE* h_nuxfile,  int version)
//==================================
{
    //we read out a marker of reading the file before reading a layer
    fseek(h_nuxfile, 0, SEEK_CUR);
    int pos = ftell(h_nuxfile);
    unsigned int p_begnuxfile = pos;

    MasterWrite(h_nuxfile,version);
    int			KolObj;
    MObject*	pObj;

    OBITER it;
    // REVOBITER;
    switch( version ) {
    //version 0
    case 0:
        //calculation and record of amount of objects
        KolObj = m_oblist.size();
        fwrite(&KolObj, 2, 1, h_nuxfile);

        //record of objects
        it = m_oblist.begin();

        int KolInd = 0;
        ///			MKarta*   pKarta = m_pKarta;
        MString panetext;
        //cycle of search of objects
        while(it!=m_oblist.end())
        {
            pObj = *it;
            pObj->WriteBin(h_nuxfile,  version);
            KolInd++;
            it++;
            //indication
            ///				double	ind = floor(((double) KolInd)/((double) KolObj) * 100);
            //				panetext.Format("Слой: %s %d %%", m_name, (int) ind);
        }
        //we read out a marker of reading the file after reading a layer
        fseek(h_nuxfile, 0, SEEK_CUR);
        //we calculate layer length in bytes as a difference of indications of a marker of reading the file
        //before reading a layer
        pos = ftell(h_nuxfile);
        int raznitsa = pos - p_begnuxfile;
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

//===============================================
//!Connection of objects of a layer to the classifier
void MLayer::ConnectKlassifikator(MKlassifikator* pKls)
//===============================================
{
    if(!pKls)return;
    OBITER it = m_oblist.begin();
    MObject* pObj;
    MZnak*  znak;
    MString realkod;
    //cycle on objects
    while(it != m_oblist.end())
    {
        //we connect objects
        pObj = *it;
        if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1){
            MTopobject* pTpo = (MTopobject*)pObj;
            znak = pKls->GetZnak(pTpo->m_kkod,realkod);
            pTpo->ConnectZnak(znak);
            pTpo->m_kkod=realkod;
        }
        it++;
    }
}

//===================================================================================
//!Adds to the given 'oblist' objects with set the quad-codey
void MLayer::AddObjectsInObjectListByQuadrakod(MObjectList* oblist,MString quadrakod)
//===================================================================================
{
    if(!oblist)return ;
    MObject* pObj;
    //cycle on objects
    OBITER it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        pObj = *it;
        it++;
        if((pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)&&(((MTopobject*)pObj)->m_kkod==quadrakod))
        {
            oblist->push_back(pObj);
        }
    }
}

//===================================================================================
//!Deletes from objects of a layer with set quad-codey a vector and splines
//! formula bindings with number 'formnomer'
bool MLayer::DeleteFormulaPrivs(int formnomer, MString quadrakod)
//===================================================================================
{
    //we will collect all objects of a layer with an entrance quad-code
    MObjectList oblist;
    AddObjectsInObjectListByQuadrakod(&oblist,quadrakod);

    MString str;
    MZnak* pZnak = m_pKarta->m_pKls->GetZnak(quadrakod,str);
    //if there is no sign in the classifier with an entrance quad-code, nothing is done
    if(!pZnak)
        return false;
    MYaz* pYaz = pZnak->GetYaz();
    //if the index of the deleted formula goes beyond indexes of formulas
    //object of the classifier, we do nothing
    if((formnomer<0)||(formnomer>=pYaz->m_formula->GetSize()))
        return false;

    MObject* pObj;
    MTopobject* pTpo;
    OBITER it = oblist.begin();
    while(it!=oblist.end())
    {
        pObj = *it;
        if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1){
            pTpo = (MTopobject*)pObj;
            pTpo->DeleteVektorsPriv(formnomer);
        }
        it++;
    }
    return true;
}

//===================================================================================
//!For objects of a layer with set the quad-codey changes number of a formula 'oldformnom'
//! in arrays of vectors and splines of bindings of formulas on number 'newformnom'
bool MLayer::ChangeFormulaNomer(int oldformnom, int newformnom, MString quadrakod)
//===================================================================================
{
    // we will collect all objects of a layer with an entrance quad-code

    MObjectList oblist;
    AddObjectsInObjectListByQuadrakod(&oblist,quadrakod);

    MString str;
    MZnak* pZnak = m_pKarta->m_pKls->GetZnak(quadrakod,str);
    //if there is no sign in the classifier with an entrance quad-code, nothing is done
    if(!pZnak)
        return false;
    ///	MYaz* pYaz = pZnak->GetYaz();
    //if an initial index of a formula negative, we do nothing
    if(oldformnom<0)
        return false;
    //if a final index of a formula negative, we do nothing
    if(newformnom<0)
        return false;

    MObject* pObj;
    MTopobject* pTpo;
    OBITER it = oblist.begin();
    while(it!=oblist.end())
    {
        pObj = *it;
        if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1){
            pTpo = (MTopobject*)pObj;
            pTpo->ChangeFormulaNomer(oldformnom, newformnom);
        }
        it++;
    }
    return true;
}


//==========================================
//!We look for, whether there is in a layer such object
bool MLayer::IsObject(MObject* pObj)
//==========================================
{
    OBITER it;
    it = find(m_oblist.begin(),m_oblist.end(), pObj);
    if(it!=m_oblist.end()) return true;
    else return false;
}

//======================================================
//!Replaces in the list of objects all indexes of 'pOldObj' with 'pNewObj'
void MLayer::ReplaceObject(MObject* pOldObj, MObject* pNewObj)
//======================================================
{
    //   OBITER it ;
    replace(m_oblist.begin(),m_oblist.end(),pOldObj,pNewObj) ;
}

//====================================================
//!Forms the list of the objects belonging to this layer
//! The result is located in the given 'm_oblist'
void MLayer::GetObjectList(MObjectList* oblist)
//====================================================
{
    if(oblist)
    {
        oblist->clear();
        oblist->insert(oblist->end(),m_oblist.begin(),m_oblist.end());
    }
}

//========================================================
//!To receive amount of objects
int  MLayer::GetObjectCount()
//========================================================
{
    return m_oblist.size();
}

//====================================================
//!Forms the list of objects in the set point (them can be much)
//! The result is located in the given 'findlist'
void MLayer::FindObjectsAtPoint(MPoint findpt,MObjectList* findlist)
//====================================================
{
    //we will receive the list of objects of a layer
    MObjectList oblist;
    GetObjectList(&oblist);

    OBITER it = m_oblist.begin();
    MObject* pObj;
    //cycle on objects
    while(it != m_oblist.end())
    {
        //we connect suitable objects to the submitted list
        pObj = *it;
        if(pObj->m_bounds.IsPtInRect(findpt)) findlist->push_back(pObj);
        it++;
    }

}

//====================================================
//!Forms the list of objects in the set point (them can be much)
//! The result is located in the given 'findlist'
//! More exact, but long search
void MLayer::FindObjectsAtPointExact(MPoint findpt,MObjectList* findlist, double delta)
//====================================================
{
    //we will receive the list of objects of a layer

    OBITER it = m_oblist.begin();
    MObject* pObj;
    //cycle on objects
    while(it != m_oblist.end())
    {
        //we connect suitable objects to the submitted list
        pObj = *it;
        if (pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1 && pObj->m_enabled)
        {
            MTopobject* tpo = (MTopobject*)pObj;
            if(tpo->m_pZnak)
            {
                switch(tpo->m_pZnak->GetYaz()->GetType())
                {
                case F_PLO:
                {
                    //				MPtArray ptar;
                    //				MPoint pt;
                    if (tpo->m_bounds.IsPtInRect(findpt))
                        for(int i=tpo->F_TOCHKI;i<pObj->m_Komplekt->Size;i++)
                        {
                            MPtArray* pta = (MPtArray*)(pObj->m_Komplekt->GetAt(i));

                            /*				pt = MPoint(findpt.x-delta, findpt.y+delta); ptar.AddTail(&pt);
                        pt = MPoint(findpt.x+delta, findpt.y+delta); ptar.AddTail(&pt);
                        pt = MPoint(findpt.x+delta, findpt.y-delta); ptar.AddTail(&pt);
                        pt = MPoint(findpt.x-delta, findpt.y-delta); ptar.AddTail(&pt);
                        pt = MPoint(findpt.x-delta, findpt.y+delta); ptar.AddTail(&pt);
        */
                            if	(PointInRegion(findpt, pta))
                            {findlist->push_back(pObj);break;}
                            /*				if	(	(PointInRegion(ptar[0], pta)) ||
                                (PointInRegion(ptar[1], pta)) ||
                                (PointInRegion(ptar[2], pta)) ||
                                (PointInRegion(ptar[3], pta))
                            )
                            findlist->push_back(pObj);
                        else
                            for(int i = 0; i<pta->GetSize(); i++)
                            {
                                if(PointInRegion(*(pta->GetAt(i)), &ptar))
                                {
                                    findlist->push_back(pObj);
                                    break;
                                }

                 * 		}				*/
                        }
                }
                    break;
                case F_LIN:
                {
                    int nomrebra;
                    MPoint resultpt;
                    //if(FindDistFromPointToKontur(findpt, (MPtArray*)(pObj->m_Komplekt->GetAt(((MTopobject*)pObj)->F_TOCHKI)), resultpt, &nomrebra) <= delta)
                    //	findlist->push_back(pObj);
                    if (tpo->m_bounds.IsPtInRect(findpt))
                        for(int i=tpo->F_TOCHKI;i<pObj->m_Komplekt->Size;i++)
                            if(FindDistFromPointToLomline(findpt, (MPtArray*)(pObj->m_Komplekt->GetAt(i)), resultpt, &nomrebra) <= delta)
                            {findlist->push_back(pObj);break;}
                }
                    break;
                case F_DOT:
                {
                    MRect rect;
                    MSymbol* sym = NULL;
                    unsigned int align = 0;

                    //we will receive the symbol size
                    MYaz* pYaz = tpo->m_pZnak->GetYaz();
                    MSzArray* szArr = ((MDotYaz*)pYaz->m_object)->m_tochka;
                    double sym_size = 0;
                    if(szArr)
                    {
                        if(szArr->GetSize()>0)
                        {
                            if((szArr->GetAt(0)->GetType() == IDT_SHRIFT) || (szArr->GetAt(0)->GetType() == IDT_NAS_SHRIFT))
                            {
                                sym = &(((MShriftSrez*)szArr->GetAt(0))->m_tochka);
                                sym_size = sym->value;
                                sym_size = SizeToReal(tpo, sym_size);
                                align = sym->align;
                            }
                            else if (szArr->GetAt(0)->GetType() == IDT_PROIZVOL)
                            {
                                MProizvolSrez *pSrez = (MProizvolSrez *)szArr->GetAt(0);
                                sym_size = SizeToReal(tpo, 1);
                                align = pSrez->m_align;
                            }
                        }
                    }

                    if (!sym_size) break;

                    int toch = tpo->F_TOCHKI;
                    MPtArray* pta = (MPtArray*)(pObj->m_Komplekt->GetAt(toch));
                    if(pta->GetSize() == 0) break;
                    MPoint ptpriv = pta->GetAt(0);
                    if ((align & UT_TA_CENTER) == UT_TA_CENTER)
                    {
                        rect.left = ptpriv.x-sym_size/2;
                        rect.right = ptpriv.x+sym_size/2;
                    }
                    else
                    {
                        if ((align & UT_TA_RIGHT) == UT_TA_RIGHT)
                        {
                            rect.left = ptpriv.x-sym_size;
                            rect.right = ptpriv.x;
                        }
                        else
                        {
                            if ((align & UT_TA_LEFT) == UT_TA_LEFT)
                            {
                                rect.left = ptpriv.x;
                                rect.right = ptpriv.x+sym_size;
                            }
                        }
                    }

                    if ((align & UT_TA_BASELINE) == UT_TA_BASELINE)
                    {
                        rect.top = ptpriv.y+sym_size/2;
                        rect.bottom = ptpriv.y-sym_size/2;
                    }
                    else
                        if ((align & UT_TA_BOTTOM) == UT_TA_BOTTOM)
                        {
                            rect.top = ptpriv.y+sym_size;
                            rect.bottom = ptpriv.y;
                        }
                        else
                            if ((align & UT_TA_TOP) == UT_TA_TOP)
                            {
                                rect.top = ptpriv.y;
                                rect.bottom = ptpriv.y-sym_size;
                            }

                    //rect.Inflate(delta);
                    if(rect.IsPtInRect(findpt))
                        findlist->push_back(pObj);
                }
                    //			if(DLINASG(findpt,((MPtArray*)(pObj->m_Komplekt->GetAt(((MTopobject*)pObj)->F_TOCHKI)))->GetAt(0)) <= delta)
                    //				findlist->push_back(pObj);
                    break;
                }
            }
        }
        it++;
    }
}

//====================================================
//!Forms the list of objects in the set point (them can be much)
//! The result is located in the given 'findlist'
void MLayer::FindObjectsAtRect(MRect& findrec,MObjectList* findlist)
//====================================================
{
    //
    MObjectList oblist;
    GetObjectList(&oblist);

    OBITER it = m_oblist.begin();
    MObject* pObj;
    //
    while(it != m_oblist.end())
    {
        //
        pObj = *it;
        if(MRect::IntersectGeoRect(pObj->m_bounds, findrec)) findlist->push_back(pObj);
        it++;
    }

}

//====================================================
//! to find in a layer object, the next to the set point
/** NB: at top objects the proximity to the CONTOUR, at the other is checked
//successors of 'MObject' if function isn't redefined
//FindDistanceFromPoint, to the CENTER
*/
MObject* MLayer::FindNearestObject(MPoint findpt)
//====================================================
{
    //we will receive the list of objects of a layer
    MObjectList oblist;
    GetObjectList(&oblist);

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

    return pFindObj;
}



//==================================
//! We will find the next top object and its point of intersection of ours
/** top object with some straight line set by two points of linept1, linept2.
//If our top object is crossed from a straight line in several places,
//linept2, next to a point, gets out of all points of intersection.
//The calculated point of intersection is placed in resultpt.
//The calculated distance in rassto.
*/
MObject* MLayer::FindNearestObjectIntersectWithLine(MPoint linept1, MPoint linept2, MPoint& resultpt, double& rassto)
//==================================
{
    //we will receive the list of objects of a layer
    MObjectList oblist;
    GetObjectList(&oblist);

    // cycle prolog
    OBITER it = oblist.begin();
    MObject* pObj;
    MObject* pFindObj = NULL;
    // as estimated maximum distance we set pro-deleting of a layer
    rassto = Max(m_bounds.Width(),m_bounds.Height());
    double distance;
    MPoint respt;

    //cycle on objects
    while(it != oblist.end())
    {
        pObj = *it;
        int res = pObj->FindNearestIntersectWithLine(linept1, linept2, respt, distance);
        if((res==0)&&(distance<rassto))
        {
            resultpt = respt;
            rassto = distance;
            pFindObj = pObj;
        }
        it++;
    }

    return pFindObj;
}

//=================================================================
//!In 'pLay' layer copies the members.
//! In the list of objects only indexes, but not objects are copied
void MLayer::CreateCopyForUndo(MLayer* pLay)
//=================================================================
{
    //We will check for coincidence of types and existence of pLay
    if(!pLay)return;
    if(pLay->WhatTheLayer()!=WhatTheLayer())return;
    //We copy the member m_dbarray of the class MLayer
    pLay->m_dbarray.ClearAll();
    pLay->m_dbarray = *(MStArray *)m_dbarray.Clone();

    //We copy the member m_name of the class MLayer
    pLay->m_name=m_name;

    //We copy the member m_sort of the class MLayer
    pLay->m_sort=m_sort;

    //We copy the member m_remark of the class MLayer
    pLay->m_remark=m_remark;

    //We copy the member m_password of the class MLayer
    pLay->m_password=m_password;

    //We copy the member m_bounds of the class MLayer
    pLay->m_bounds=m_bounds;

    //We copy the member x_common of the class MLayer
    pLay->x_common=x_common;

    //We copy the member x_visible of the class MLayer
    pLay->x_visible=x_visible;

    //We copy the member x_textvisible of the class MLayer
    pLay->x_textvisible=x_textvisible;

    //We copy the member m_pKarta of the class MLayer
    pLay->m_pKarta=m_pKarta;

    //We copy the member m_oblist of the class MLayer
    pLay->m_oblist.clear();
    pLay->m_oblist.insert(pLay->m_oblist.end(),m_oblist.begin(),m_oblist.end());
}

//====================================================
//!Clears the list of objects, but thus doesn't delete objects
void MLayer::ClearObjectList()
//====================================================
{
    m_oblist.clear();
}

//=================================
//!Deletes all objects of a layer
void MLayer::DeleteObjects()
//=================================
{
    /*	if objects have there own destructors -
    make SURE that they are called */
    /*
    MObject* pObj;
    OBITER it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        pObj = *it;
        delete pObj;
        it++;
    }
    m_oblist.clear();
*/

    //Thanks to < back >
    MObject * pObj;
    MTopobject *pObj2;
    while (!(m_oblist.empty()))
    {
        pObj = m_oblist.back();
        if(pObj && pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
        {
            pObj2=(MTopobject*)(pObj);
            if(pObj2)
            {
                delete pObj2;
                pObj2 = NULL;
            }
        }
        else
        {
            if(pObj)
            {
                delete pObj;
                pObj = NULL;
            }
        }
        m_oblist.pop_back();
    }
}



//======================================================
//!Returns information for record in the SHP-file
//! 'strarr' - names of fields
//! 'fieldlength' - lengths of fields
void MLayer::GetInfoForShpWrite(MStArray& names,MInArray* dlina)
//======================================================
{
    if(!dlina)return;

    int l, col, i = 0;
    MString st;

    // the number of fields is equal in the dbf-file to number of elements of the m_dbarray array of a layer minus one zero element where the name of a layer is stored
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
    OBITER it = m_oblist.begin();

    MObject* pObj;
    while(it != m_oblist.end())
    {
        pObj = *it;
        if (pObj->WhatTheObject()==ID_TOPOBJECT) {
            MTopobject* pHPt = (MTopobject*)pObj;

            //whether we will check there is an attribute "height"
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
                /*m_pKarta->m_pKls->GetDictAttrName(num, str);
                if(str == "высота")
                    str = attr_arr->GetAt(i)->GetValue();*/

                //if(((num == 1)||(num == 19))&&(str==""))//"высота"
                if((num == 19)&&(str==""))//"высота"
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
        else
            if (pObj->WhatTheObject()==ID_HEIGHTPOINT) {
                MHeightPoint* pHPt = (MHeightPoint*)pObj;
                MString str;
                double d = pHPt->GetHeight();
                str.Format("%f", d);
                *(pHPt->m_dbarray[1]) = str;
                for(i=0; i<col; i++){
                    if(pHPt->m_dbarray.GetSize()>=i+1){
                        l = pHPt->m_dbarray[i]->GetLength();
                        if(l>*dlina->GetAt(i)) dlina->SetAt(i,&l);
                    }
                }
            }
        it++;
    }
}
//=======================================
//!Allocation of "shadows" from a layer
//! 'pole' - a name of the field containing standard distance
void MLayer::DaiShades(MLayer* shadelayer, MString pole)
//=======================================
{
    MObject* pObj;
    MTopobject* pShade;
    OBITER it;

    // drawing up the list of the objects having "shadow"
    MObjectList kontlist;

    it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        pObj = *it;
        if (pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1) {
            MTopobject* pTpo = (MTopobject*)pObj;

            // If the standard distance isn't set, comes back0
            double distofaxe = pTpo->GetDoubleDataPoleByName(pole);

            // if the standard distance for this type isn't set,
            //we do nothing that
            if(distofaxe){
                pShade = (MTopobject*)pTpo->Shade(pole,distofaxe);
                shadelayer->AddObject(pShade);
            }
        }
        it++;
    }
}

//=======================================
//! Assignment of a quad-code to objects, at which corresponding fields
//! m_dbarray have preset values
void MLayer::SetKvadrokodByDbarray(MString QQ, MStArray* dbarrfieldvalues)
//=======================================
{
    // drawing up list of objects
    MObjectList* oblist = new MObjectList;
    GetObjectList(oblist);

    OBITER it = oblist->begin();
    while(it!=oblist->end())
    {
        MObject* pObj = *it;
        if (pObj->WhatTheObject()==ID_TOPOBJECT) {

            MTopobject* pTpo = (MTopobject*)pObj;

            // check of a condition
            bool result = true;
            MString str1,str2;
            for(int i=0; i<dbarrfieldvalues->GetSize();i++) {
                str1 = *dbarrfieldvalues->GetAt(i);
                if (str1!="") {
                    str2 = *pTpo->m_dbarray.GetAt(i);
                    if (str1!=str2) result = false;
                }
            }

            // if everything coincided, we appropriate quad-code
            if (result) pTpo->ChangeClass(QQ,false);
        }
        it++;
    }
}


//=======================================
//! Import of values from m_dbarray to data fields
void MLayer::SetDataByDbarray(MInArray* dbindex, MShInArray* dtnoms)
//=======================================
{
    // drawing up list of objects
    MObjectList* oblist = new MObjectList;
    GetObjectList(oblist);

    OBITER it = oblist->begin();
    while(it!=oblist->end())
    {
        MObject* pObj = *it;
        if (pObj->WhatTheObject()==ID_TOPOBJECT)
        {
            MTopobject* pTpo = (MTopobject*)pObj;
            int ii,jj;
            for(int i=0; i<dbindex->GetSize();i++)
            {
                ii = *dbindex->GetAt(i);
                jj = *dtnoms->GetAt(i);
                MNvArray* data = (MNvArray*)pTpo->m_Komplekt->GetAt(F_NADPIS);
                int ind = data->SetValue(jj,*pTpo->m_dbarray.GetAt(ii));
                if(ind==-1)
                {
                    MNomValue* nomval = new MNomValue(jj,*pTpo->m_dbarray.GetAt(ii));
                    data->AddTail(nomval);
                    delete nomval;
                }
            }

        }
        it++;
    }

}

//=======================================
//!transliteration
void MLayer::Transliter()
//=======================================
{
    MObject* pObj;

    MString str, strnew, s;

    int kol = m_oblist.size();
    int nom = 0;

    OBITER it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        pObj = *it;
        int col = pObj->m_dbarray.GetSize();
        for (int i = 0; i<col; i++)
        {
            Transliteration(pObj->m_dbarray[i]);
        }
        MString strr; strr.Format("%d",kol - nom);
        nom++;
        it++;
    }
}


//==================================
//! addition of object in a layer
void MLayer::AddObject(MObject* pObj, bool bMinimize)
//==================================
{
    m_oblist.push_back(pObj);
    pObj->m_pLay = this;

    if (bMinimize)	MinimizeBounds();
}

//==================================
//! delete of object
void MLayer::RemoveObject(MObject* pObj)
//==================================
{
    if(!pObj) return;
    // we will find and will delete object
    OBITER it = std::find(m_oblist.begin(),m_oblist.end(),pObj);
    if(it!=m_oblist.end())
    {
        //        delete pObj;
        //        pObj = NULL;
        m_oblist.erase(it);
    }
}


//==================================
//! minimization of an operational layer of a layer
void MLayer::MinimizeBounds()
//==================================
{
    if(!m_oblist.size())
        return;
    OBITER it = m_oblist.begin();
    MObject* pObj;
    pObj = *it;
    it++;
    if(pObj!=NULL) m_bounds = pObj->m_bounds;
    else return;
    while(it != m_oblist.end())
    {
        pObj = *it;
        m_bounds.UnionGeoRect(m_bounds,pObj->m_bounds);
        it++;
    }
}

//==================================
//! installation of a sign of degree of transparency
void MLayer::SetTransType(int type)
{
    if ((type>=0)&&(type<=14))
        m_trans_type=type;
} 

//==================================
//! the simplified drawing of objects of a layer

std::mutex mLayerDraw1Mu;

void MLayer::Draw1(MPlotDevice* pDevice, MRect geodrawrect)
//==================================
{
    //std::lock_guard<std::mutex> lck(mLayerDraw1Mu);

    // REMARK: at the use of this function one cunning is possible:
    //if to give it 'clib' of the zero size, drawing will happen
    //ONLY in 'memdc' context (this effect is used in MView:: Linza).
    //Otherwise at accumulation of the counter
    //count is more than 'm_DRAWPORTION', the image will be periodically splashed out
    //from 'memdc' in 'pDC' (in case 'pDC' - a screen context, it provides
    //gradualness of drawing - portions on 'm_DRAWPORTION' of segments)

    // DRAWING of OBJECTS

    if(!x_visible)
    {
        return;
    }

    std::for_each(m_oblist.begin(), m_oblist.end(), [&](MObject* pObj)
    {
        if (pObj->m_selected)
        {
           // mLayerDraw1Mu.lock();
            pObj->m_pLay->m_pKarta->m_SelecedList.push_back(pObj);
           // mLayerDraw1Mu.unlock();
        }

        if (pObj->m_enabled && pObj->CheckSize(pDevice) &&
                IntersectGeoRectFast(pObj->m_bounds, geodrawrect))
        {
           // mLayerDraw1Mu.lock();
            pObj->Draw1(pDevice);
           // mLayerDraw1Mu.unlock();
        }
    });

    pDevice->Actualize();
}
//==================================
//! drawing of objects of a layer

static std::mutex mLayerDrawMu;

void MLayer::Draw(MPlotDevice* pDevice, MRect geodrawrect)
//==================================
{
    //std::lock_guard<std::mutex> lck(mLayerDrawMu);

    // REMARK: at the use of this function one cunning is possible:
    //if to give it 'clib' of the zero size, drawing will happen
    //ONLY in 'memdc' context (this effect is used in MView:: Linza).
    //Otherwise at accumulation of the counter
    //count is more than 'm_DRAWPORTION', the image will be periodically splashed out
    //from 'memdc' in 'pDC' (in case 'pDC' - a screen context, it provides
    //gradualness of drawing - portions on 'm_DRAWPORTION' of segments)

    //
    //  objectRealSizeMin= m_pKarta->GetDrawFilterSizeGeo();

    if (!x_visible)
    {
        return;
    }

    std::for_each(m_oblist.begin(), m_oblist.end(),
                  [&] (MObject* pObj)
    {
        if(pObj->m_selected)
        {
          //  mLayerDrawMu.lock();
            pObj->m_pLay->m_pKarta->m_SelecedList.push_back(pObj);
           // mLayerDrawMu.unlock();
        }

        if (pObj-> m_enabled && pObj->CheckSize(pDevice) &&
                IntersectGeoRectFast(pObj->m_bounds, geodrawrect))
        {
         //   mLayerDrawMu.lock();
            pObj->Draw(pDevice);
          //  mLayerDrawMu.unlock();
        }

    });

    pDevice->Actualize();
}

//==================================
//! drawing of the text at objects of a layer
void MLayer::DrawText(MPlotDevice* pDevice,MRect geodrawrect)
//==================================
{
    // REMARK: at the use of this function one cunning is possible:
    //if to give it 'clib' of the zero size, drawing will happen
    //ONLY in 'memdc' context (this effect is used in MView:: Linza).
    //Otherwise at accumulation of the counter
    //count is more than 'm_DRAWPORTION', the image will be periodically splashed out
    //from 'memdc' in 'pDC' (in case 'pDC' - a screen context, it provides
    //gradualness of drawing - portions on 'm_DRAWPORTION' of segments)
    if (!x_visible) return;
    if (!x_textvisible) return;


    // DRAWING of TEXT
    MObject* pObj;
    OBITER it;

    it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        pObj = *it;
        if (pObj->m_selected)
        {
            pObj->m_pLay->m_pKarta->m_SelecedList.push_back(pObj);
            //        it++;
            //        continue;
        }
        if (pObj-> m_enabled)//if (pObj->CheckTextSize(pDevice))    // isVisible  vipa 2004.10.26
            // object in the zoom area?
            if (IntersectGeoRectFast(pObj->m_bounds,geodrawrect))
                pObj->DrawText(pDevice);

        it++;
    }

    // we draw a result
    pDevice->Actualize();
}

//==================================
#ifdef _DEBUG
void MLayer::AssertValid()
//==================================
{
    //	if(!(m_bounds.left <= m_bounds.right)) exit(15);
    //	if(!(m_bounds.bottom <= m_bounds.top)) exit(15);
}
#endif

//==============================
//!transformation to the model
void MLayer::ConvertToMaket()
//==============================
{
    //we will be run on all objects and we will cause in them the corresponding function of transformation
    MObject* pObj;

    OBITER it = m_oblist.begin();

    while(it != m_oblist.end())
    {
        pObj = *it;
        pObj->ConvertToMaket();
        it++;
    }

    MinimizeBounds();
}

//===================================================================
//!transformation to other system of coordinates
void MLayer::ConvertToCoo(MCoo* pCurrentCoo, MCoo* pNewCoo)
//===================================================================
{
    if((pCurrentCoo==NULL)||(pNewCoo==NULL))
        return;

    //we will be run on all objects and we will cause in them the corresponding function of transformation
    MObject* pObj;
    OBITER it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        pObj = *it;
        pObj->ConvertToCoo(pCurrentCoo, pNewCoo);
        it ++;
    }

    MinimizeBounds();
}

//===================================================================
//!Creates copy of Layer(this)
void MLayer::Copy(MLayer* pLay)
//===================================================================
{
    m_name = pLay->m_name;
    m_sort = pLay->m_sort;
    m_remark = pLay->m_remark;
    m_password = pLay->m_password;
    m_bounds = pLay->m_bounds;

    m_dbarray.ClearAll();
    m_dbarray = *(MStArray*)pLay->m_dbarray.Clone();

    x_visible = pLay->x_visible;
    x_textvisible = pLay->x_textvisible;
    x_common = pLay->x_common;
    m_commoncolor_pen = pLay->m_commoncolor_pen;
    m_commoncolor_brush = pLay->m_commoncolor_brush;
    m_commoncolor_brushTransparent = pLay->m_commoncolor_brushTransparent;
    m_commoncolor_penTransparent = pLay->m_commoncolor_penTransparent;
    m_commoncolor_text = pLay->m_commoncolor_text;
    m_commonpenwidth = pLay->m_commonpenwidth;
    m_commontextwidth = pLay->m_commontextwidth;

    m_pKarta = pLay->m_pKarta;
    m_sheetname = pLay->m_sheetname;

    //copying of the list of objects
    m_oblist.clear();
    //	MObjectList list;
    //	list.clear();
    OBITER it = pLay->m_oblist.begin();
    while(it != pLay->m_oblist.end())
    {
        MObject* objTek = *it;
        MObject* newObj = objTek->Clone();
        if(newObj->GetIerarhy().Find(ID_TOPOBJECT, 0)!=-1)
            ((MTopobject*)newObj)->m_pZnak = ((MTopobject*)objTek)->m_pZnak;//->Clone();
        newObj->m_pLay = this;
        //list.push_back(newObj);
        m_oblist.push_back(newObj);
        it++;
    }
    //	m_oblist.clear();
    //	m_oblist.insert(m_oblist.end(), list.begin(), list.end());
}

//!============================================
//! Creation of the list of objects of a layer depending on chosen like objects
//! iObjType = F_LIN-linear; The F_PLO-areal; F_DOT - point
//! Conditions:> calling function Must DELETE formed Objlist
//! Is used:> when Writing SHP-file
MObjectList* MLayer::CreateObjectListByObjType(short int iObjType)
//============================================
{
    MObjectList* newObList = new MObjectList;
    MObject* Obj;
    MTopobject* Tpo;

    OBITER it = m_oblist.begin();
    while(it != m_oblist.end())
    {
        Obj = *it;
        //if 'Obj' is inherited from 'Topobject'
        if ((*it)->IsTopobjClass())
        {
            Tpo = (MTopobject*)Obj;
            //if Tpo is connected to 'Znak'
            if(Tpo->m_pZnak){
                //Get pointer to 'YazArray' of 'Znak'
                MYzArray* pYazArray = Tpo->m_pZnak->GetYazArray();
                //if 'YazArray' is not empty
                if (pYazArray->GetSize()>0)
                {
                    if (pYazArray->GetAt(0)->GetType() == iObjType)
                        newObList->push_back(Obj);
                }
            }
        }
        it++;
    }
    return newObList;
}

//!============================================
/**ENTRANCE:
//'plankontur' - a gas pipeline broken line on the plan, 'kodarray' - the array of quad-code of objects, with which
//we look for crossing of the gas pipeline
//'profar' - a gas pipeline profile
//EXIT:
//function finds all crossings of the gas pipeline with roads and returns information in
//three arrays: 'Roadcoord', point which elements: 'x' - coordinate is a shift of the initial point
//crossings of the road and the gas pipeline from the beginning of the gas pipeline, 'y'-coordinate is a shift final
//points of intersection of the road and the gas pipeline from the beginning of the gas pipeline; 'Roadheight', which elements
//correspond to elements of the 'Roadcoord' array and are heights of points of intersection: 'pt.x' - height
//points of intersection on an entrance, 'pt.y' - height of a point of intersection at the exit; 'pRoadObjects' - the array
//indexes on objects of the road which are crossed by the gas pipeline, to each element of the array of indexes
//'pRoadObjects' corresponds an element of the 'Roadcoord' and 'Roadheight' arrays
//all arrays are ordered on shift of the initial point of crossing of the gas pipeline and the road
*/
bool MLayer::FindCrosslinesWithRoads(MPtArray* plankontur, MPtArray* profar, MStArray* kodar, MVoidArray* pRoadObjects,
                                     MPtArray* Roadcoord, MPtArray* Roadheight)
//============================================
{
    //all arrays have to be initialized
    if((!pRoadObjects)||(!Roadcoord)||(!Roadheight)) return false;
    //entrance arrays have to be not only are initialized, but also shouldn't be empty
    if((!plankontur)||(plankontur->GetSize()<2)) return false;
    if((!kodar)||(kodar->GetSize()==0)) return false;


    //before work, we will clean output arrays
    pRoadObjects->ClearAll();
    Roadcoord->ClearAll();
    Roadheight->ClearAll();

    //we will make the list of objects of a layer, quad-code which coincides with one of quad-code
    //from the entrance list
    MObjectList objects;
    GetObjectList(&objects);
    MObjectList crossobjects;//the list of objects with which we check crossing of the gas pipeline

    //on all objects of a layer we look for objects with an entrance quad-code
    OBITER it = objects.begin();
    while(it != objects.end())
    {
        MObject* pObj = *it;
        //only top objects have quad-codea and their successors, and all roads - top objects
        if(pObj->WhatTheObject()==ID_TOPOBJECT){
            MString kkod;
            //check on all quad-codea
            for(int i=0; i<kodar->GetSize(); i++){
                kkod = *kodar->GetAt(i);
                //if quad-codea coincided, we place object in the list of objects with which it is checked
                //crossing of the gas pipeline
                if(((MTopobject*)pObj)->m_kkod==kkod){
                    crossobjects.push_back(pObj);
                    break;
                }
            }
        }
        it++;
    }

    //if the list of objects with which we check crossing of the gas pipeline, is empty,
    //function safely fulfilled
    if(crossobjects.size()==0) return false;

    //We will get auxiliary arrays
    MPtArray* planRoadcoord = new MPtArray();//array of points of intersection of the gas pipeline
    //with the road on the plan
    MPtArray* crossseg = new MPtArray();//array of points of intersection of the gas pipeline
    //with the road on the plan
    //we get the cooarray, heightarray and objarray arrays temporarily instead of the output 'Roadcoord' array,
    //'Roadheight' and 'pRoadObjects'
    MPtArray* cooarray = new MPtArray();
    MPtArray* heightarray = new MPtArray();
    MVoidArray* objarray = new MVoidArray();

    bool parsg;

    it = crossobjects.begin();
    while(it != crossobjects.end())
    {
        MTopobject* pTpo = (MTopobject*)(*it);
        //road contour
        MPtArray* Roadkontur = (MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
        crossseg->ClearAll();
        planRoadcoord->ClearAll();
        //'parseg' - the parameter specifying whether coincided the last point of intersection of a contour
        //also gas pipeline contour edges from the end of an edge of the gas pipeline are expensive;
        //this parameter is necessary twice not to include the same point of intersection
        parsg = false;

        //for each edge of a contour of the gas pipeline we receive edge points of intersection with a contour
        //roads, excepting the duplicating points in cases, when the gas pipeline and the road
        //are crossed in points of intersection of edges of a contour of the gas pipeline;
        //we assume that total number of points of intersection the gas pipeline and are expensive -
        //even, i.e. it is couples of points: initial and final points of intersection of the gas pipeline and road
        int i = 0;
        for(i=0; i<plankontur->GetSize()-1; i++){
            if(FindCrossSegWithKontur(plankontur->GetAt(i), plankontur->GetAt(i+1), Roadkontur, crossseg)){
                if(parsg)
                    crossseg->DeleteHead();
                planRoadcoord->AppendTail(crossseg);
                if(*crossseg->GetAt(crossseg->GetSize()-1)==*plankontur->GetAt(i+1))
                    parsg = true;
                else
                    parsg = false;
            }
        }

        double h1, h2, rasst1, rasst2;
        int index1=0, index2=0;
        MPoint pt, profpt, Roadpt, heightpt;
        //now it is necessary to coordinate the 'planRoadcoord' and 'Roadheight' arrays so that
        //to couples of points an entrance-exit to hydroobject  there corresponded heights of points of intersection
        //on an entrance-exit
        for(i=0; i<planRoadcoord->GetSize()-1; i+=2){
            //we will find shift of a point of intersection of the gas pipeline and the road
            //from the beginning of the gas pipeline for an entry point
            pt = *planRoadcoord->GetAt(i);
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
            //the equation of the straight line passing through 'p1' and 'p2' segments  .
            //(x2-x1)*y + (y1-y2)*x + x1*(y2-y1) - y1*(x2-x1) = 0
            //from the equation of the straight line passing through a link with the gas pipeline broken line 'index1' index
            //on a profile, and values of shift of a point of intersection we will receive value
            //heights of a point of intersection on a profile:
            h1 = profar->GetAt(index1)->y + ((rasst1 - profar->GetAt(index1)->x)*
                                             (profar->GetAt(index1+1)->y - profar->GetAt(index1)->y))/
                    (profar->GetAt(index1+1)->x - profar->GetAt(index1)->x);
            //we will find shift of a point of intersection of the gas pipeline and the road
            //from the beginning of the gas pipeline for an exit point
            pt = *planRoadcoord->GetAt(i+1);
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
            //the equation of the straight line passing through 'p1' and 'p2' segments  .
            //(x2-x1)*y + (y1-y2)*x + x1*(y2-y1) - y1*(x2-x1) = 0
            //from the equation of the straight line passing through a link with the gas pipeline broken line 'index2' index
            //on a profile, and values of shift of a point of intersection we will receive value
            //heights of a point of intersection on a profile:
            h2 = profar->GetAt(index2)->y + ((rasst2 - profar->GetAt(index2)->x)*
                                             (profar->GetAt(index2+1)->y - profar->GetAt(index2)->y))/
                    (profar->GetAt(index2+1)->x - profar->GetAt(index2)->x);
            Roadpt.x = rasst1;
            Roadpt.y = rasst2;
            heightpt.x = h1;
            heightpt.y = h2;
            //we include the received point in the array of shifts of points of intersection of an entrance-exit
            //on a profile
            cooarray->AddTail(&Roadpt);
            heightarray->AddTail(&heightpt);
            void* pVoid = (void*)pTpo;
            objarray->AddTail(&pVoid);
        }//end for(i=0; i<Roadheight->GetSize()-1; i+=2)
        it++;
    }//end while(pos!=NULL)
    delete crossseg;
    delete planRoadcoord;

    if(cooarray->GetSize() == 0) return false;
    // we will order all elements of output arrays on shift from the beginning of the line of a profile
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
        Roadcoord->AddTail(cooarray->GetAt(index));
        Roadheight->AddTail(heightarray->GetAt(index));
        pRoadObjects->AddTail(objarray->GetAt(index));
        cooarray->Delete(index);
        heightarray->Delete(index);
        objarray->Delete(index);
    }
    Roadcoord->AddTail(cooarray->GetAt(0));
    Roadheight->AddTail(heightarray->GetAt(0));
    pRoadObjects->AddTail(objarray->GetAt(0));
    cooarray->Delete(0);
    heightarray->Delete(0);
    objarray->Delete(0);

    // we release more unnecessary memory
    delete cooarray;
    delete heightarray;
    delete objarray;

    if(Roadheight->GetSize()==0)
        return false;

    return true;
}

//============================================
//!search of object in the identifier
MObject* MLayer::FindObject(double uid)
//============================================
{
    OBITER it = m_oblist.begin();
    while (it!=m_oblist.end())
    {
        if ((*it)->m_UID == uid) return *it;
        it++;
    }
    return NULL;
}

//============================================
//Searches for topographic objects by defined value (m_parValue) 
//of defined attribute (unsigned short int m_parAttrNumber)
//all suitable objects are put in MObjectList (as a result of the function)
MObjectList*  MLayer::FindOblistByAttrValue(unsigned short int m_parAttrNumber, MString m_parValue)
//============================================
{
    OBITER it_o;
    MTopobject*	pObj = NULL;
    MString m_Value, m_LowerPar;
    unsigned int i,j;

    MObjectList* pOblist = new MObjectList();
    pOblist->clear();
    it_o = m_oblist.begin();
    while(it_o != m_oblist.end())
    {
        if((*it_o)->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
        {
            pObj = (MTopobject*)(*it_o);
            MNomValue* attrib;
            MNvArray* attribs = (MNvArray*)pObj->m_Komplekt->GetAt(F_NADPIS);
            const unsigned int iAttribsCount = (unsigned int) attribs->GetSize();
            for(i=0; i<iAttribsCount/*attribs->GetSize()*/; i++)
            {
                attrib = attribs->GetAt(i);
                //if number of attribute coincides with the required
                if(attrib->GetNom() == m_parAttrNumber)
                {
                    //we make comparison without the register

                    //we will transform in lower case value of attribute
                    m_Value = attrib->GetValue();
                    for(j=0;j<m_Value.length();j++)
                    {
                        if(isupper( m_Value.at(j) ) )
                            m_Value.at(j) = _tolower( m_Value.at(j) ) ;
                    }
                    //we will transform in lower case a line of comparison
                    m_LowerPar = m_parValue;
                    for(j=0;j<m_LowerPar.length();j++)
                    {
                        if(isupper( m_LowerPar.at(j) ) )
                            m_LowerPar.at(j) = _tolower( m_LowerPar.at(j) ) ;
                    }

                    //if value of attribute coincides with line of comparison
                    if(m_Value == m_LowerPar)
                        pOblist->push_back(pObj);
                }
            }
        }
        it_o++;
    }

    //if the resultant list was empty
    if (!pOblist->size())
    {
        delete pOblist;
        pOblist = NULL;
    }

    return pOblist;
}
