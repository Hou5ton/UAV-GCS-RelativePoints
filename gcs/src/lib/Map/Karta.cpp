#include <fstream>
#include <iostream>
#include <time.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qapplication.h>
#include <qmainwindow.h>
#include <qchar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qobject.h>
#include <QDir>
#include "Karta.h"
#include "Refglobal.h"
#include "Nadp.h" 
#include "RoadGidroLay.h"
#include "NadpKriv.h" 

using namespace std;
//for realization of a strip of progress when loading the map (when using library separately)
static void (*pOutputProgress)(const std::string&) = NULL;

//FUNCTION		:RegisterOutputProgress
//
//DESCRIPTION	:Function binding for change of reaction to progress
void RegisterOutputProgress(void(*pFunc)(const std::string&)){pOutputProgress = pFunc;};

//==================================
//the pointer on function for filling of parameters of the map from the outside
//==================================
extern void (*pOutputProgress)(const std::string&);

// for display of progress when reading the classifier
unsigned int	c_dlina_faila_kls;

MLayer* (*fpCreateLayerByType)(MString str) = &CreateLayerByType;
void RegisterCreateLayerByType(MLayer*(*func)(MString)){fpCreateLayerByType = func;};

MObject* (*fpCreateObjectByType)(MString str) = &CreateObjectByType;
void RegisterCreateObjectByType(MObject*(*func)(MString)){fpCreateObjectByType = func;};

MZnak* (*fpCreateZnakByType)(MString str) = &CreateZnakByType;
void RegisterCreateZnakByType(MZnak*(*func)(MString)){fpCreateZnakByType = func;};

MArray* (*fpCreateArrayByType)(MString str) = &CreateArrayByType;
void RegisterCreateArrayByType(MArray*(*func)(MString)){fpCreateArrayByType = func;};

//!>Globnalnye vars dlia sozdanija proressa
QProgressDialog *progressDLG= NULL;
QLabel          *progressLAB= NULL;
QString          fileName;
int              fileIndex= 1;
int              fileCount= 1;

//==================================
//! Constructor
MKarta::MKarta()
//==================================
{
    FormServKlassifikator();
    MRazdel* pRazdel = m_pServKls->GetZsArray()->GetAt(0)->GetRazdelArray()->GetAt(0);
    m_pDotZnak = pRazdel->GetZnakArray()->GetAt(0);
    m_pLinZnak = pRazdel->GetZnakArray()->GetAt(1);
    m_pSqrZnak = pRazdel->GetZnakArray()->GetAt(2);
    m_pHptZnak = pRazdel->GetZnakArray()->GetAt(3);
    m_pIzoZnak = pRazdel->GetZnakArray()->GetAt(4);
    m_pObrZnak = pRazdel->GetZnakArray()->GetAt(5);
    m_pOtkZnak = pRazdel->GetZnakArray()->GetAt(6);
    m_pYamZnak = pRazdel->GetZnakArray()->GetAt(7);
    m_pPstZnak = pRazdel->GetZnakArray()->GetAt(8);
    m_pGdrZnak = pRazdel->GetZnakArray()->GetAt(9);

    //we fill the pointer on the classifier on the service classifier
    m_pKls = m_pServKls;

    m_papercolorkod = m_pKls->m_klscolorkod;

    //model variables
    m_maketlist.left = m_maketlist.right = m_maketlist.top = m_maketlist.bottom = 0.0;
    m_geopt1.x = m_geopt1.y = 0.0;
    m_geopt2.x = m_geopt2.y = 0.0;
    m_geopt3.x = m_geopt3.y = 0.0;
    m_maketpt1.x = m_maketpt1.y = 0.0;
    m_maketpt2.x = m_maketpt2.y = 0.0;
    m_maketpt3.x = m_maketpt3.y = 0.0;
    m_ismaket = false;

    // georectangle by default:
    m_geobounds.left   = -180.0;
    m_geobounds.top    =  90.0;
    m_geobounds.right  =  180.0;
    m_geobounds.bottom = -90.0;

    // shift of a chromaticity-brightness-saturation	= 0
    m_DH = 0;
    m_DL = 0;
    m_DS = 0;

    //for drawing of a background of operational layers
    m_actBackEnable = false;
    m_actBackLight = 255;
    m_actBackTransp = 127;


    //systems of coordinates

    m_pObjectCoo = new MGeodesicCoo();

    //files for:
    m_bmhfile = NULL;    //matrix of heights
    m_bmhobjfile = NULL; //matrix of object heights
    m_bmhcorfile = NULL; //adjustment matrix

    m_DrawFilterSizeLog = 1;
    m_DrawFilterSizeGeo = 0;

    textSizeMax = 200;

    percent= -1;
    pMainMapWidget = NULL;

    layerDrawFilter = new MLayerDrawFilter();

    m_matrixExtraFile = MString();


}

//==================================
//!forms the service classifier, is caused in the designer of the map
void MKarta::FormServKlassifikator()
//==================================
{
    //FORMATION OF THE SERVISE classifier
    //====================================
    //We create the service classifier
    m_pServKls = new MKlassifikator();

    // term
    MTermin term;
    term.SetKod(SERV_KLS);
    //"Service Classifier");
    term.SetShortname(IDS_OFFICIAL_KLS_SHRT);
    //"Service Classifier For Internal Use");
    term.SetFullname(IDS_OFFICIAL_KLS_FLL);
    term.SetRemarka("");
    m_pServKls->SetTermin(term);

    //sign system
    MZsArray* zsa = m_pServKls->GetZsArray();
    term.SetKod(SERV_ZS);
    //"Service Sign System");
    term.SetShortname(IDS_OFFICIAL_SIGNSYST_SHRT);
    //"Service Sign System For Internal Use");
    term.SetFullname(IDS_OFFICIAL_SIGNSYST_FLL);
    term.SetRemarka("");
    MZnakSistema* zs = new MZnakSistema();
    zs->SetTermin(term);
    zsa->AddTail(zs);
    delete zs;

    //Partition
    zs=zsa->GetAt(0);
    MRlArray* rla = zs->GetRazdelArray();
    term.SetKod(SERV_RZ);
    //"Service Partition");
    term.SetShortname(IDS_OFFICIAL_PARTITION_SHRT);
    //"Service Partition For Internal Use");
    term.SetFullname(IDS_OFFICIAL_PARTITION_FLL);
    term.SetRemarka("");
    MRazdel* razdel = new MRazdel();
    razdel->SetTermin(term);
    rla->AddTail(razdel);
    delete razdel;
    razdel=rla->GetAt(0);
    MZnArray* zna = razdel->GetZnakArray();

    //We form signs of the service classifier
    MZnak* pZnak = NULL;


    // Point sign
    term.SetKod(SERV_DOT);
    term.SetShortname(IDS_POINT_OBJ);//"Point Object");
    term.SetFullname(IDS_POINT_OBJ);//"Point Object");
    term.SetRemarka("");
    MYaz* pDotYaz = new MYaz(F_DOT);
    MYzArray* pYazArray = new MYzArray();
    pYazArray->AddTail(pDotYaz);
    delete pDotYaz;
    pZnak = new MZnak(term,pYazArray);
    pYazArray->ClearAll();
    zna->AddTail(pZnak);
    delete pZnak;

    // Linear sign
    term.SetKod(SERV_LIN);
    term.SetShortname(IDS_LINEAR_OBJ);//"Linear Object");
    term.SetFullname(IDS_LINEAR_OBJ);//"Linear Object");
    term.SetRemarka("");
    MYaz* pLinYaz = new MYaz(F_LIN);
    pYazArray->AddTail(pLinYaz);
    pZnak = new MZnak(term,pYazArray);
    delete pLinYaz;
    pYazArray->ClearAll();
    zna->AddTail(pZnak);
    delete pZnak;

    // Areal sign
    term.SetKod(SERV_PLO);
    term.SetShortname(IDS_AREA_OBJ);//"Areal Object");
    term.SetFullname(IDS_AREA_OBJ);//"Areal Object");
    term.SetRemarka("");
    MYaz* pPloYaz = new MYaz(F_PLO);
    pYazArray->AddTail(pPloYaz);
    pZnak = new MZnak(term,pYazArray);

    delete pPloYaz;
    pYazArray->ClearAll();
    zna->AddTail(pZnak);
    delete pZnak;
    // Altitude Mark
    pZnak = CreateZnakByType(ID_HEIGHTPOINT);
    term.SetKod(SERV_HPT);
    term.SetShortname(IDS_ALTITUDE_MARK);//" Altitude Mark");
    term.SetFullname(IDS_ALTITUDE_MARK);//" Altitude Mark");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    //Isoline
    pZnak = CreateZnakByType(ID_IZOLINE);
    term.SetKod(SERV_IZO);
    term.SetShortname(IDS_ISOLINE);//"Isoline");
    term.SetFullname(IDS_ISOLINE);//"Isoline");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    //Steep
    pZnak = CreateZnakByType(ID_OBRYV);
    term.SetKod(SERV_OBR);
    term.SetShortname(IDS_OBRYV);//"Steep");
    term.SetFullname(IDS_OBRYV);//"Steep");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    //Slope
    pZnak = CreateZnakByType(ID_OTKOS);
    term.SetKod(SERV_OTK);
    term.SetShortname(IDS_OTKOS);//"Slope");
    term.SetFullname(IDS_OTKOS);//"Slope");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    //Hole
    pZnak = CreateZnakByType(ID_YAMA);
    term.SetKod(SERV_YAM);
    term.SetShortname(IDS_YAMA);//"Hole");
    term.SetFullname(IDS_YAMA);//"Hole");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    //Supporting Wall
    pZnak = CreateZnakByType(ID_PODPSTENKA);
    term.SetKod(SERV_PST);
    term.SetShortname(IDS_PODPSTENKA_SHRT);//"Supporting Wall");
    term.SetFullname(IDS_PODPSTENKA_FLL);//"Supporting Wall");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    //Hydrographical Object
    pZnak = CreateZnakByType(ID_GIDRO);
    term.SetKod(SERV_GDR);
    term.SetShortname(IDS_GIDRO);//"Hydrographical Object");
    term.SetFullname(IDS_GIDRO);//"Hydrographical Object");
    term.SetRemarka("");
    pZnak->SetTermin(term);
    zna->AddTail(pZnak);
    delete pZnak;

    delete pYazArray;
    //END OF FORMATION OF THE SERVICE classifier
    //====================================
}

//!==================================
MKarta::~MKarta()
//==================================
{
    //We delete all layers (geographical and quick)
    ClearAll();

    //We delete the service classifier
    if(m_pServKls) { delete m_pServKls;   m_pServKls= NULL;   }

    //we delete system of coordinates
    delete m_pObjectCoo;
    if(m_pOldObjectCoo)
        delete m_pOldObjectCoo;

    //we delete 'progressDLG'
    //if (progressDLG) {delete progressDLG;	progressDLG = NULL;}

    //we close the files with matrixes
    //(destructor closes the files)
    if(m_bmhfile) delete m_bmhfile;
    if(m_bmhobjfile) delete m_bmhobjfile;
    if(m_bmhcorfile) delete m_bmhcorfile;

    m_SelecedList.clear();

    m_FilterArray.ClearAll();      //  2004.11.17  vipa
    m_ShNameArray.ClearAll();      //  2004.11.17  vipa

    delete layerDrawFilter;
}

//==================================
//! addition of a layer
void MKarta::AddLayer(MLayer* pLay)
//==================================
{
    // we will get a new layer
    if(!pLay)
        pLay = new MLayer(m_geobounds);
    m_laylist.push_back(pLay);
    pLay->m_pKarta = this;
}

//==================================
//! addition of an operational layer
void MKarta::AddActLayer(MLayer* pLay)
//==================================
{
    // we will get a new layer
    if(!pLay)
        pLay = new MLayer(m_geobounds);
    m_actlaylist.push_back(pLay);
    m_actlaylist.sort(SSortLayers());
    //std::sort(m_actlaylist.begin(), m_actlaylist.end(), SSortLayers());
    pLay->m_pKarta = this;
}

//==================================
//! search of a layer in a key
MLayer* MKarta::FindLayer(MString key)
//==================================
{
    LAYITER it = m_laylist.begin();
    while (it!=m_laylist.end())
    {
        if ((*it)->m_name == key) return *it;
        it++;
    }
    return NULL;
}

//==================================
//!search of an operational layer in a key
MLayer* MKarta::FindActLayer(double uid)
//==================================
{
    LAYITER it = m_actlaylist.begin();
    while (it!=m_actlaylist.end())
    {
        if ((*it)->m_UID == uid) return *it;
        it++;
    }
    return NULL;
}

//==================================
//! search of object in all operational layers
bool MKarta::FindObjectAtActLayers(double uid)
//==================================
{
    LAYITER it = m_actlaylist.begin();
    MObject* obj;
    while (it!=m_actlaylist.end())
    {
        obj = (*it)->FindObject(uid);
        if (obj) return true;
        it++;
    }
    return false;
}

//==================================
//! removal of a layer
bool MKarta::RemoveLayer(MLayer* pLay)
//==================================
{
    //search in the list of the 'pLay' element
    LAYITER	it = find(m_laylist.begin(),m_laylist.end(),pLay);
    if (it != m_laylist.end()) {
        if(!(m_laylist.size()>1))  return false;     //exit(15);
        //At first it is required to remove objects of this layer
        pLay->DeleteObjects();
        m_laylist.erase(it);

        return true;
    }
    else return false;
}

//==================================
//! removal of an operational layer
bool MKarta::RemoveActLayer(MLayer* pLay)
//==================================
{
    //search in the list of the 'pLay' element
    LAYITER	it = find(m_actlaylist.begin(),m_actlaylist.end(),pLay);
    if(!(*it)) return false;

    //dr 21.07.2016 pLay != 0
    if(pLay)
    {
        pLay->DeleteObjects();
        m_actlaylist.erase(it);
    }

    return true;
}

//==================================
//! movement of a layer on a step up
bool MKarta::MoveLayerUp(MLayer* pLay)
//==================================
{
    //	search in the list of the 'pLay' element
    LAYITER	it, it_prev;
    it =	it_prev = find(m_laylist.begin(),m_laylist.end(),pLay);
    if((it == m_laylist.end())||(it_prev == m_laylist.begin()))
        return false;
    it_prev--;

    //replacing
    iter_swap(it,it_prev);

    return true;

}

//==================================
//! movement of an operational layer on a step up
bool MKarta::MoveActLayerUp(MLayer* pLay)
//==================================
{
    //	search in the list of the 'pLay' element
    LAYITER	it, it_prev;
    it =	it_prev = find(m_actlaylist.begin(),m_actlaylist.end(),pLay);
    if((it == m_actlaylist.end())||(it_prev == m_actlaylist.begin()))
        return false;
    it_prev--;

    //replacing
    iter_swap(it,it_prev);

    return true;
}

//==================================
//! movement of a layer on a step down
bool MKarta::MoveLayerDown(MLayer* pLay)
//==================================
{
    //	search in the list of the 'pLay' element
    LAYITER	it, it_next;
    it =	it_next = find(m_laylist.begin(),m_laylist.end(),pLay);
    if(it== m_laylist.end())
        return false;
    it_next++;
    if(it_next == m_laylist.end())
        return false;
    //replacing
    iter_swap(it,it_next);

    return true;
}

//==================================
//! movement of an operational layer on a step down
bool MKarta::MoveActLayerDown(MLayer* pLay)
//==================================
{
    //	search in the list of the 'pLay' element
    LAYITER	it, it_next;
    it =	it_next = find(m_actlaylist.begin(),m_actlaylist.end(),pLay);
    if(it== m_actlaylist.end())
        return false;
    it_next++;
    if(it_next == m_actlaylist.end())
        return false;
    //replacing
    iter_swap(it,it_next);

    return true;
}

//==============================================================
//!Makes replacement of two layers in the document
//! Indexes start from scratch
void MKarta::ExchangeLayers(int m_iFirst, int m_iSecond)
//==============================================================
{
    if(!(m_laylist.size() > (unsigned)m_iFirst))    return;     //exit(15);
    if(!(m_laylist.size() > (unsigned)m_iSecond))   return;     //exit(15);

    LAYITER	it1, it2;
    it1 = it2 = m_laylist.begin();

    int i = 0;
    for (i=0;i<m_iFirst;i++) 		it1++;
    for (i=0;i<m_iSecond;i++)		it2++;

    //replacing
    iter_swap(it1,it2);
}

//==============================================================
//!Makes replacement of two layers in the document
//! Indexes start from scratch
void MKarta::ExchangeActLayers(int m_iFirst, int m_iSecond)
//==============================================================
{
    if(!(m_actlaylist.size() > (unsigned)m_iFirst))    return;   //exit(15);
    if(!(m_actlaylist.size() > (unsigned)m_iSecond))   return;   //exit(15);

    LAYITER	it1, it2;
    it1 = it2 = m_actlaylist.begin();

    int i=0;
    for (i=0;i<m_iFirst;i++) 		it1++;
    for (i=0;i<m_iSecond;i++)		it2++;

    //replacing
    iter_swap(it1,it2);
}

//==================================
//! Removal of the lower layer
void MKarta::LayerheadDelete()
{
    MLayer* pLay = m_laylist.front();

    m_laylist.pop_front();

    delete pLay;
}

//==================================
//! Removal of the lower operational layer
void MKarta::ActLayerheadDelete()
//==================================
{
    MLayer* pLay = m_actlaylist.front();

    m_laylist.pop_front();

    delete pLay;
}

//==================================
//! removal of object
void MKarta::Remove(MObject* pObj)
//==================================
{
    // preset
    LAYITER	it = m_laylist.begin();
    // cycle
    while ( it != m_laylist.end())
    {
        (*it)->RemoveObject(pObj);
        it++;
    }
}

//==================================
//! removal of operational object
void MKarta::RemoveActor(MObject* pObj)
//==================================
{
    // preset
    LAYITER	it = m_actlaylist.begin();
    // cycle
    while ( it != m_actlaylist.end())
    {
        (*it)->RemoveObject(pObj);
        it++;
    }
}

//==================================
//! removal of operational object
void MKarta::RemoveActor(double uid)
//==================================
{
    LAYITER	it = m_actlaylist.begin();

    MObject* obj;
    while ( it != m_actlaylist.end())
    {
        obj = (*it)->FindObject(uid);
        if(obj)
            (*it)->RemoveObject(obj);
        it++;
    }
}

//!==================================
void MKarta::ClearAll()
//==================================
{
    // preset
    LAYITER	it = m_laylist.begin();
    //memory release
    while ( it != m_laylist.end())
    {
        delete (*it);
        it++;
    }
    //clarification of the list of layers
    m_laylist.clear();

    it = m_actlaylist.begin();
    //memory release
    while ( it != m_actlaylist.end())
    {
        delete (*it);
        it++;
    }
    //clarification of the list of operational layers
    m_actlaylist.clear();

    // we get the first layer by default and we do it to workers
    //MLayer* pLay = new MLayer(m_geobounds);
    //m_laylist.push_back(pLay);

    //pLay->m_pKarta = this;

}

//==================================
//! Function of reading the term of the classifier of the map from the UTP file
bool MKarta::GetKlsTermin(char * filepath, MTermin& termin)
//==================================
{
    //we will open the file for reading
    FILE* h_nuxfile = fopen(filepath, "rb");
    if ( h_nuxfile == NULL)
    {
        InternalMessage( "Can't open file !");
        return false;
    }

    char		id_file[] = "   ";
    int version = 0;

    //reading identifier of the file
    fread(&id_file, 3, 1, h_nuxfile);
    MString idfile = id_file;
    if (idfile!=ID_UTP_KARTA) {
        InternalMessage( "Unknown format !");
        return false;
    }


    //version
    fread(&version, 2, 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&version, sizeof(int) );


    //	record length
    int LPassport=0;
    fread(&LPassport, 2, 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&LPassport, sizeof(int) );

    //We read the term of the classifier
    termin.ReadBin(h_nuxfile,version);

    fclose(h_nuxfile);
    return true;
}

//==================================
//! Function of opening of the file of the map for reading in the main UTP format
FILE* MKarta::OpenFileForReading(const char * filepath)
//==================================
{
    FILE* h_nuxfile = fopen(filepath, "rb");
    if ( h_nuxfile == NULL)
        InternalMessage( "Can't open file !");

    return h_nuxfile;
}

//==================================
//! Function of map reading in the main UTP format
bool MKarta::ReadBin(FILE* h_nuxfile, MKlassifikator* pKls)
//==================================
{
    if ( h_nuxfile == NULL){
        InternalMessage( "MKarta::ReadBin >> Can't open file !");
        return false;
    }
    // first of all, we fill the index on the classifier
    if(pKls==NULL) m_pKls = m_pServKls;
    else m_pKls = pKls;

    int			version = 0;

    ClearAll();


    if (!ReadKarta(h_nuxfile, version))
    {
        InternalMessage("Error reading karta");
    }

    return true;
}

//==================================
//! Function of map reading in the main UTP format
bool MKarta::ReadBin(MString& name, MKlassifikator* pKls)
//==================================
{
    MFile file;
    if(!file.SetPath(name))
    {
        InternalMessage( "Can't open file !");
        return false;
    }
    // first of all, we fill the index on the classifier
    if(pKls==NULL) m_pKls = m_pServKls;
    else m_pKls = pKls;

    int			version = 0;

    ClearAll();

    ReadKarta(name, version);

    return true;
}

//==================================
//! Function of opening of the file of the map for record in the main UTP format
FILE* MKarta::OpenFileForWriting(char * filepath)
//================================== 
{
    FILE*	h_nuxfile = fopen(filepath, "wb");

    if ( h_nuxfile == NULL)
        InternalMessage( "Can't open file !");
    return h_nuxfile;
}
//==================================
//! Function of saving of the map in the main UTP format
/** !!! the version variable contains the version of a format and
//! !!! it is transferred in all support functions
//! !!! readings/records as one of key parameters
*/
bool MKarta::WriteBin(FILE* h_nuxfile)
//================================== 
{
    if(h_nuxfile == NULL)
        return false;

    int version = 0;
    WriteKarta(h_nuxfile,  version);

    return true;
}

//================================================
//!Closing of the file opened earlier
void MKarta::CloseFile(FILE* h_nuxfile)
//================================================
{
    fclose(h_nuxfile);
}

//================================================
//!To be connected to other classifier
void MKarta::ConnectKlassifikator(MKlassifikator* pKls)
//================================================
{
    if(!pKls) return;

    //  m_papercolorkod = pKls->m_klscolorkod;    // vipa 2004.07.20

    //We are connected to the new classifier
    LAYITER it = m_laylist.begin();
    //on layers
    while(it != m_laylist.end())
    {
        //We receive the next layer and it is connected its objects
        (*it)->ConnectKlassifikator(pKls);
        it++;
    }
    //we register the new classifier
    m_pKls = pKls;

    m_klstermin.Copy(m_pKls->GetTermin());
}

//!==================================
MColor MKarta::GetPaperColor(bool HLS)
//==================================
{
    int RR,GG,BB;
    MColor color;
    //	if(m_papercolorkod == 0)  return m_papercolor;

    //  vipa 2004.11.29       user color with HLS
    if(m_papercolorkod == 0)  {
        color= m_papercolor;   // user color
        if (HLS) {             // user color with HLS
            SdvigRGBbyHLS(m_papercolor.R,m_papercolor.G,m_papercolor.B, m_DH,m_DL,m_DS, RR,GG,BB);
            color.R = RR;
            color.G = GG;
            color.B = BB;
        }
        return color;
    }

    //we take color of paper from the table of flowers and if there didn't find,
    if(!GetDictColorValue((unsigned short)m_papercolorkod, color, HLS))
        color.SetColor(255,255,255);
    return color;
}

//!==================================
void MKarta::SetPaperColor(MColor& col)
//==================================
{
    m_papercolor = col;
    m_papercolorkod = 0;
}

//==================================
//Functions of work with tables
//==================================
//color lookup table
//!==================================
bool MKarta::GetDictColorName(unsigned short int ushIndex,MString& strName)
//==================================
{
    return m_pKls->GetDictColorName(ushIndex,strName);
}

//!==================================
bool MKarta::GetDictColorValue(unsigned short int ushIndex,MColor& colorValue, bool HLS)
//==================================
{
    int RR,GG,BB;
    //	If color is calculated with shift on HLS
    bool result = m_pKls->GetDictColorValue(ushIndex,colorValue);
    if (HLS)  {
        SdvigRGBbyHLS(colorValue.R,colorValue.G,colorValue.B,m_DH,m_DL,m_DS,RR,GG,BB);
        colorValue.R = RR;
        colorValue.G = GG;
        colorValue.B = BB;
    }
    return result;
}

//!==================================
bool MKarta::SetDictColorValue(unsigned short int ushIndex,MColor colorValue)
//==================================
{
    return m_pKls->SetDictColorValue(ushIndex,colorValue);
}

//!==================================
bool MKarta::SetDictColorName(unsigned short int ushIndex,MString strName)
//==================================
{
    return m_pKls->SetDictColorName(ushIndex,strName);
}

//!==================================
bool MKarta::AddDictColor(MColor clrrfValue, short& ushKey,MString strName)
//==================================
{
    return m_pKls->AddDictColor(clrrfValue,ushKey,strName);
}

//Table of styles
//!==================================
bool MKarta::GetDictPrkName(unsigned short int ushIndex,MString& strName)
//==================================
{
    return m_pKls->GetDictPrkName(ushIndex,strName);
}

//!==================================
bool MKarta::GetDictPrkValue(unsigned short int ushIndex,MPrk& prkValue)
//==================================
{
    return m_pKls->GetDictPrkValue(ushIndex,prkValue);
}

//!==================================
bool MKarta::SetDictPrkValue(unsigned short int ushIndex,MPrk* prkValue)
//==================================
{
    return m_pKls->SetDictPrkValue(ushIndex,prkValue);
}

//!==================================
bool MKarta::SetDictPrkName(unsigned short int ushIndex,MString strName)
//==================================
{
    return m_pKls->SetDictPrkName(ushIndex,strName);
}

//!==================================
bool MKarta::AddDictPrk(MPrk* prkValue, short& ushKey,MString strName)
//==================================
{
    return m_pKls->AddDictPrk(prkValue,ushKey,strName);
}

//Table of concepts
//!==================================
bool MKarta::GetDictAttrName(unsigned short int ushIndex,MString& strName)
//==================================
{
    return m_pKls->GetDictAttrName(ushIndex,strName);
}

//!==================================
bool MKarta::GetDictAttrEdIzm(unsigned short int ushIndex,MString& edIzm)
//==================================
{
    return m_pKls->GetDictAttrEdIzm(ushIndex,edIzm);
}

//!==================================
bool MKarta::GetDictAttrType(unsigned short int ushIndex,MString& type)
//==================================
{
    return m_pKls->GetDictAttrType(ushIndex,type);
}

//!==================================
bool MKarta::SetDictAttrName(unsigned short int ushIndex,MString strName)
//==================================
{
    return m_pKls->SetDictAttrName(ushIndex,strName);
}

//!==================================
bool MKarta::SetDictAttrEdIzm(unsigned short int ushIndex,MString edIzm)
//==================================
{
    return m_pKls->SetDictAttrEdIzm(ushIndex,edIzm);
}

//!==================================
bool MKarta::SetDictAttrType(unsigned short int ushIndex,MString type)
//==================================
{
    return m_pKls->SetDictAttrType(ushIndex,type);
}

//!==================================
bool MKarta::AddDictAttr(MString strName, short& ushKey,MString strType,MString strEdIzm)
//==================================
{
    return m_pKls->AddDictAttr(strName,ushKey,strType,strEdIzm);
}

//Table of fonts
//!==================================
//!==================================
bool MKarta::GetDictImageName(unsigned short int ushIndex,MString& strName)
//==================================
{
    return m_pKls->GetDictImageName(ushIndex,strName);
}

//!==================================
bool MKarta::GetDictImageValue(unsigned short int ushIndex,MImage &image)
//==================================
{
    return m_pKls->GetDictImageValue(ushIndex,image);
}

//!==================================
bool MKarta::SetDictImageName(unsigned short int ushIndex,MString strName)
//==================================
{
    return m_pKls->SetDictImageName(ushIndex,strName);
}

//!==================================
bool MKarta::SetDictImageValue(unsigned short int ushIndex,MImage *image)
//==================================
{
    return m_pKls->SetDictImageValue(ushIndex,image);
}

//!==================================
bool MKarta::AddDictImage(MString strName, short& ushKey, MImage *image)
//==================================
{
    return m_pKls->AddDictImage(strName,ushKey,image);
}


//Table of fonts
//!==================================
bool MKarta::GetDictFontName(unsigned short int ushIndex,MString& strName)
//==================================
{
    return m_pKls->GetDictFontName(ushIndex,strName);
}

//!==================================
bool MKarta::GetDictFontValue(unsigned short int ushIndex,M_LOG_FONT& colorValue)
//==================================
{
    return m_pKls->GetDictFontValue(ushIndex,colorValue);
}

//!==================================
bool MKarta::SetDictFontValue(unsigned short int ushIndex,M_LOG_FONT lfValue)
//==================================
{
    return m_pKls->SetDictFontValue(ushIndex,lfValue);
}

//!==================================
bool MKarta::SetDictFontName(unsigned short int ushIndex,MString strName)
//==================================
{
    return m_pKls->SetDictFontName(ushIndex,strName);
}

//!==================================
bool MKarta::AddDictFont(M_LOG_FONT lfValue, short& ushKey,MString strName)
//==================================
{
    return m_pKls->AddDictFont(lfValue,ushKey,strName);
}

//Table of boundary lines
//!==================================
bool MKarta::GetDictMegstrName(unsigned short int ushIndex,MString& strName)
//==================================
{
    return m_pKls->GetDictMegstrName(ushIndex,strName);
}

//!==================================
bool MKarta::GetDictMegstrValue(unsigned short int ushIndex,MString& colorValue)
//==================================
{
    return m_pKls->GetDictMegstrValue(ushIndex,colorValue);
}

//!==================================
bool MKarta::SetDictMegstrValue(unsigned short int ushIndex,MString megstrValue)
//==================================
{
    return m_pKls->SetDictMegstrValue(ushIndex,megstrValue);
}

//!==================================
bool MKarta::SetDictMegstrName(unsigned short int ushIndex,MString strName)
//==================================
{
    return m_pKls->SetDictMegstrName(ushIndex,strName);
}
//!==================================
bool MKarta::AddDictMegstr(MString megstrValue, short& ushKey,MString strName)
//==================================
{
    return m_pKls->AddDictMegstr(megstrValue,ushKey,strName);
}

//The following functions have to be removed
//!==================================
bool MKarta::GetColorDictNumValue(MColor clrValue,unsigned short int& ExistKey)
//==================================
{
    return m_pKls->GetColorDictNumValue(clrValue,ExistKey);
}

//!==================================
bool MKarta::GetPrkDictNumValue(MPrk *clrValue,unsigned short int& ExistKey)
//==================================
{
    return m_pKls->GetPrkDictNumValue(clrValue,ExistKey);

}


//!==================================
bool MKarta::GetAttrDictNumValue(MString clrValue,unsigned short int& ExistKey)
//==================================
{
    return m_pKls->GetAttrDictNumValue(clrValue,ExistKey);
}

//!==================================
bool MKarta::GetFontDictNumValue(M_LOG_FONT clrValue,unsigned short int& ExistKey)
//==================================
{
    return m_pKls->GetFontDictNumValue(clrValue,ExistKey);
}

//!==================================
bool MKarta::GetMegStrDictNumValue(MString clrValue,unsigned short int& ExistKey)
//==================================
{
    return m_pKls->GetMegStrDictNumValue(clrValue,ExistKey);
}

//FUNCTIONS OF REPLACEMENT STYLE COLOUR of the BOUNDARY LINE AND the CHARACTERISTIC IN OBJECTS of the DOCUMENT
//!==============================================================================
bool MKarta::ChangeColor(unsigned char oldcolorkod,unsigned char newcolorkod)	
//==============================================================================
{

    //replacement of color of handwriting in the dictionary of styles
    m_pKls->ChangePrkColorInPrkDict(oldcolorkod,newcolorkod);
    //replacement of color of objects of the classifier

    m_pKls->ChangeColor(oldcolorkod,newcolorkod);
    return true;
}

//==================================
//!replacement of style by new style
bool MKarta::ChangePocherk(unsigned char oldprkkod,unsigned char newprkkod)
//==================================
{
    m_pKls->ChangePocherk(oldprkkod,newprkkod);
    return true;
}

//==================================

//!replacement of a boundary line
bool MKarta::ChangeMegaStroka(unsigned char oldmegastrkod, unsigned char newmegastrkod)
//==================================
{
    m_pKls->ChangeMegaStroka(oldmegastrkod,newmegastrkod);
    return true;
}

//==================================
//!replacement of the characteristic
bool MKarta::ChangeCharakteristika(unsigned short oldcharkod, unsigned short newcharkod)
//==================================
{
    //we change codes in the array of admissible codes
    m_pKls->ChangeCharakteristika(oldcharkod,newcharkod);


    //we change codes in an array of individual characteristics of objects of the map
    MLayer* pLay;


    LAYITER it = m_laylist.begin();
    OBITER it_o;
    while (it != m_laylist.end())
    {
        pLay = (*it);
        MObject* pObj = NULL;
        pLay->GetObjectList(&pLay->m_oblist);
        it_o = pLay->m_oblist.begin();
        while(it_o != pLay->m_oblist.end())
        {
            pObj = *it_o;
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
            {
                MNomValue* attrib;
                MNvArray* attribs = (MNvArray*)pObj->m_Komplekt->GetAt(F_NADPIS);
                for(int i=0; i<attribs->GetSize(); i++)
                {
                    attrib = attribs->GetAt(i);
                    if(attrib->GetNom()==oldcharkod)
                        attrib->SetNom(newcharkod);
                }
            }

            it_o++;
        }
        it++;
    }
    return true;
}

//==================================
//!alignment of all dot font signs on the center
bool MKarta::DotShriftZnakCenterAlign()
//==================================
{
    m_pKls->DotShriftZnakCenterAlign();
    return true;
}

//==================================
//!Installation of a way to the file of a <Height matrix>
bool MKarta::SetMatrixFile(MString path)
//==================================
{
    if(path == "") return false;
    //to check existence of the file on the way of 'path'

    if(m_bmhfile) delete m_bmhfile;

    m_bmhfile = new MBmhFILE(path); //fopen(path, "rb");
    if(!m_bmhfile) return false;

    m_matrixfile = path;

    return true;
}

//==================================
//!Installation of a way to the file of a Matrica Vysot Objectov
bool MKarta::SetMatrixObjectFile(MString path)
//==================================
{
    if(path == "") return false;
    if(m_bmhobjfile) delete m_bmhobjfile; //fclose(m_bmhobjfile);
    m_bmhobjfile = new MBmhFILE(path);//fopen(path, "rb");
    if(!m_bmhobjfile) return false;

    m_matrixExtraFile = path;//veronika 25.04.2016

    return true;
}


//==================================
//!Installation of a way to the file of a Matrica Korrectirovki
bool MKarta::SetMatrixCorrectionFile(MString path)
//==================================
{
    if(path == "") return false;
    if(m_bmhcorfile) delete m_bmhcorfile; //fclose(m_bmhcorfile);
    m_bmhcorfile = new MBmhFILE(path);//fopen(path, "rb");
    if(!m_bmhcorfile) return false;
    return true;
}

int MKarta::GetHeight(const MPoint& pt,  MBmhFILE *file) const
{
    int value = file->GetHeight(pt);
    return ((int)value==BMH_NOVALUE)? BMH_OUTOFBOUNDS : (int)value ;
}

int MKarta::GetHeightFromMatrix(const MPoint& pt) const
{
    MBmhFILE* file = m_bmhfile;

    if(file == NULL)
        return BMH_OUTOFBOUNDS;

    return GetHeight(pt, file);
}

//==================================
//!Getting Height from <Height matrix>
int MKarta::GetHeightFromMatrix(const MPoint& pt,  const MString &path)
//==================================
{
    MBmhFILE* file = nullptr;

    if (path == "")		return GetHeightFromMatrix(pt);
    else				file = new MBmhFILE(path);//fopen(path, "rb");

    if(file == NULL)
        return BMH_OUTOFBOUNDS;

    int value =  GetHeight(pt, file);

    delete file; //ENG Close the file if we opened it
    return value;
}

//int MKarta::GetHeightFromMatrix(MPoint& pt, MString path)
////==================================
//{
//	MBmhFILE* file;
//	int str, stb, CurSeek;
//	short value = BMH_OUTOFBOUNDS; //-20000;
//
//	if (path=="")   file = m_bmhfile;
//	else            file = new MBmhFILE(path);//fopen(path, "rb");
//	
//	if((file == NULL)||(file->m_pfile == NULL))
//		return BMH_OUTOFBOUNDS;
//
//	//ENG we've read <mrect>
//	MRect mrect(file->Left(),file->Bottom(),file->Right(),file->Top());
//	mrect.NormalizeGeoRect();
//
//	if(!mrect.IsPtInRect(pt)) {
//		if (path != "") delete file;
//		return BMH_OUTOFBOUNDS;
//	}
//
//
//	CurSeek = sizeof(double)*4+sizeof(int)*2+sizeof(double);
//
//	str = (int)((pt.y - file->Bottom())/file->Step());
//	stb = (int)((pt.x - file->Left())/file->Step());
//	
//	CurSeek += (str*file->MxM() + stb)*2;
//
//	//fseek(file->m_pfile, CurSeek, SEEK_SET);
//
//	file->m_pfile->seek(CurSeek);
//	
//		//we read out the lower size
//	//fread(&value, sizeof(value), 1, file->m_pfile);
//	file->m_pfile->read((char*)&value, sizeof(value));
//
//	if (reverseReadFlag) reverseValue((char*)&value, sizeof(value) );
//
//	if (path != "") delete file; //ENG Close the file if we opened it
//	return ((int)value==BMH_NOVALUE)? BMH_OUTOFBOUNDS : (int)value ;
//}

//==================================
//!Getting Height from <Height matrix>
int MKarta::GetHeightRowFromMatrix(const MPoint& pt,MInArray* values, int count)
//==================================
{
    /*MBmhFILE* file;
    int str, stb, CurSeek, headSize, realCount, value_int;
    file = m_bmhfile;
    short value;
    int NData = BMH_OUTOFBOUNDS;//-20000;
    int read = 0;

    for (int i=0;i<count;i++)
        values->AddTail(&NData);
    if((file == NULL)||(file->m_pfile == NULL))
        return NData;

    //ENG we've read <mrect>
    MRect mrect(file->Left(),file->Bottom(),file->Right(),file->Top());
    mrect.NormalizeGeoRect();
    if(!mrect.IsPtInRect(pt)) {
        return NData;
    }

    CurSeek = headSize = sizeof(double)*4+sizeof(int)*2+sizeof(double);

    str = (int)((pt.y - file->Bottom())/file->Step());
    stb = (int)((pt.x - file->Left())/file->Step());

    CurSeek += (str*file->MxM() + stb)*2;
    fseek(file->m_pfile, CurSeek, SEEK_SET);

    realCount = file->MxM()-(CurSeek-headSize)%file->MxM();

    //we read out the lower size

    for (int n=0;n<Min(realCount,count);n++)
    {
     //read = fread(values, sizeof(short), Min(realCount,count), file->m_pfile);

        if (fread(&value, sizeof(short), 1, file->m_pfile))
        {
        read++;
        value_int = (int)value;
        values->SetAt(n,&value_int);
        }
    }

    //If 'Solaris' - to turn integers
    if (reverseReadFlag) reverseValue((char*)values, sizeof(short) );

    int trl;
    InternalMessage("was read" + QString::number(read));
    for (int k=0;k < Min(realCount, count); k++)
    {trl = *values->GetAt(k);
        InternalMessage(QString::number(trl));
    }
    return read;*/
    return 1;
}


//==================================
//!Obtaining height from a Matrica Vysot Objectov
int MKarta::GetHeightFromMatrixObject(const MPoint& pt)
{
    MBmhFILE* file = m_bmhobjfile;

    if(file == NULL)
        return BMH_OUTOFBOUNDS;

    return GetHeight(pt, file);
}

int MKarta::GetHeightFromMatrixObject(const MPoint& pt,  const MString &path)
//==================================
{
    MBmhFILE* file = nullptr;

    if (path == "")		return GetHeightFromMatrixObject(pt);
    else				file = new MBmhFILE(path);//fopen(path, "rb");

    if(file == NULL)
        return BMH_OUTOFBOUNDS;

    int value =  GetHeight(pt, file);

    delete file; //ENG Close the file if we opened it
    return value;
}

//int MKarta::GetHeightFromMatrixObject(MPoint& pt,  const MString &path)
////==================================
//{	/* //old: KOD DO BCTPOJKI MBmhFILE 27.05.2005
//	FILE* file;
//	double rect[4], step;
//	int m, n, str, stb;
//	short value = -20000;
//
//	if (path=="")   file = m_bmhobjfile;
//	else            file = fopen(path, "rb");
//	
//	if( file == NULL)   return (int)value;
//	
//	fseek(file, 0, SEEK_SET);
//	
//	for(int i=0; i<4;i++) {
//		fread(&rect[i], sizeof(double), 1, file);
//	if (reverseReadFlag) reverseValue((char*)&rect[i], sizeof(double) );
//	}
//	
//	MRect mrect(rect[0],rect[1],rect[2],rect[3]);
//	mrect.NormalizeGeoRect();
//	if(!mrect.IsPtInRect(pt)) {
//		return (int)value;
//	}
//	
//	fread(&n, sizeof(n), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&n, sizeof(n) );
//	fread(&m, sizeof(m), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&m, sizeof(m) );
//	fread(&step, sizeof(step), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&step, sizeof(step) );
//	
//	str = (int)((pt.y - rect[1])/step);
//	stb = (int)((pt.x - rect[0])/step);
//	fseek(file, (str*m + stb)*2, SEEK_CUR);
//	fread(&value, sizeof(value), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&value, sizeof(value) );
//	
//	return (int)value;
//	*/
//	MBmhFILE* file;
//	int str, stb, CurSeek;
//	short value = -20000;
//
//	if (path=="")   file = m_bmhobjfile;
//	else            file = new MBmhFILE(path);//fopen(path, "rb");
//	
//	if((file == NULL)||(file->m_pfile == NULL))
//		return BMH_OUTOFBOUNDS;
//
//	//ENG we've read <mrect>
//	MRect mrect(file->Left(),file->Bottom(),file->Right(),file->Top());
//	mrect.NormalizeGeoRect();
//	if(!mrect.IsPtInRect(pt)) {
//		if (path != "") delete file;
//		return BMH_OUTOFBOUNDS;
//	}
//
//	CurSeek = sizeof(double)*4+sizeof(int)*2+sizeof(double);
//
//	str = (int)((pt.y - file->Bottom())/file->Step());
//	stb = (int)((pt.x - file->Left())/file->Step());
//	
//	CurSeek += (str*file->MxM() + stb)*2;
//	//fseek(file->m_pfile, CurSeek, SEEK_SET);
//	file->m_pfile->seek(CurSeek);
//	
//  //we read out the lower size
//	//fread(&value, sizeof(value), 1, file->m_pfile);
//	file->m_pfile->read((char*)&value,sizeof(value));
//
//	if (reverseReadFlag) reverseValue((char*)&value, sizeof(value) );
//
//	if (path != "") delete file; //ENG Close the file if we opened it
//	return ((int)value==BMH_NOVALUE)? BMH_OUTOFBOUNDS : (int)value ;
//}

//==================================
//!Receiving a number of height from a Matrica Objectov
int MKarta::GetHeightRowFromMatrixObject(const MPoint& pt,MInArray* values, int count)
//==================================
{
    /*
    MBmhFILE* file;
    int str, stb, CurSeek, headSize, realCount, value_int;
    file = m_bmhobjfile;;
    short value;
    int NData = -20000;
    int read = 0;

    for (int i=0;i<count;i++)
        values->AddTail(&NData);
    if((file == NULL)||(file->m_pfile == NULL))
        return NData;

    //ENG we've read <mrect>
    MRect mrect(file->Left(),file->Bottom(),file->Right(),file->Top());
    mrect.NormalizeGeoRect();
    if(!mrect.IsPtInRect(pt)) {
        return NData;
    }

    CurSeek = headSize = sizeof(double)*4+sizeof(int)*2+sizeof(double);

    str = (int)((pt.y - file->Bottom())/file->Step());
    stb = (int)((pt.x - file->Left())/file->Step());

    CurSeek += (str*file->MxM() + stb)*2;
//	fseek(file->m_pfile, CurSeek, SEEK_SET);
    file->m_pfile->at(CurSeek);

    realCount = file->MxM()-(CurSeek-headSize)%file->MxM();

    //we read out the lower size
    for (int n=0;n<Min(realCount,count);n++)
    {
        if (fread(&value, sizeof(short), 1, file->m_pfile))
        {
        read++;
        value_int = (int)value;
        values->SetAt(n,&value_int);
        }
    }
    if (reverseReadFlag) reverseValue((char*)values, sizeof(short) );
    int trl;
    InternalMessage("was read" + QString::number(read));
    for (int k=0;k < Min(realCount, count); k++)
    {trl = *values->GetAt(k);
        InternalMessage(QString::number(trl));
    }
    return read;
*/
    return 1;
}

//==================================
//!Obtaining height from an Matrica Korrectirovki
int MKarta::GetHeightFromMatrixCorrection(const MPoint& pt)
{
    MBmhFILE* file = m_bmhcorfile;

    if(file == NULL)
        return BMH_OUTOFBOUNDS;

    return GetHeight(pt, file);
}

int MKarta::GetHeightFromMatrixCorrection(const MPoint& pt,  const MString &path)
//==================================
{
    MBmhFILE* file = nullptr;

    if (path == "")		return GetHeightFromMatrixCorrection(pt);
    else				file = new MBmhFILE(path);//fopen(path, "rb");

    if(file == NULL)
        return BMH_OUTOFBOUNDS;

    int value =  GetHeight(pt, file);

    delete file; //ENG Close the file if we opened it
    return value;
}

//int MKarta::GetHeightFromMatrixCorrection(MPoint& pt,  const MString &path)
////==================================
//{/*
//	FILE* file;
//	double rect[4], step;
//	int m, n, str, stb;
//	short value = -20000;
//
//	if (path=="")   file = m_bmhcorfile;
//	else            file = fopen(path, "rb");
//	
//	if( file == NULL)   return (int)value;
//
//	fseek(file, 0, SEEK_SET);
//
//	for(int i=0; i<4;i++) {
//		fread(&rect[i], sizeof(double), 1, file);
//	if (reverseReadFlag) reverseValue((char*)&rect[i], sizeof(double) );
//	}
//
//	MRect mrect(rect[0],rect[1],rect[2],rect[3]);
//	mrect.NormalizeGeoRect();
//	if(!mrect.IsPtInRect(pt)) {
//		return (int)value;
//	}
//
//	fread(&n, sizeof(n), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&n, sizeof(n) );
//	fread(&m, sizeof(m), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&m, sizeof(m) );
//	fread(&step, sizeof(step), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&step, sizeof(step) );
//
//	str = (int)((pt.y - rect[1])/step);
//	stb = (int)((pt.x - rect[0])/step);
//	fseek(file, (str*m + stb)*2, SEEK_CUR);
//	fread(&value, sizeof(value), 1, file);
//		if (reverseReadFlag) reverseValue((char*)&value, sizeof(value) );
//
//	return (int)value;
//	*/
//	MBmhFILE* file;
//	int str, stb, CurSeek;
//	short value = -20000;
//
//	if (path=="")   file = m_bmhcorfile;
//	else            file = new MBmhFILE(path);//fopen(path, "rb");
//	
//	if((file == NULL)||(file->m_pfile == NULL))
//		return (int)value;
//
//	//ENG we've read <mrect>
//	MRect mrect(file->Left(),file->Bottom(),file->Right(),file->Top());
//	mrect.NormalizeGeoRect();
//	if(!mrect.IsPtInRect(pt)) {
//		if (path != "") delete file;
//		return (int)value;
//	}
//
//	CurSeek = sizeof(double)*4+sizeof(int)*2+sizeof(double);
//
//	str = (int)((pt.y - file->Bottom())/file->Step());
//	stb = (int)((pt.x - file->Left())/file->Step());
//	
//	CurSeek += (str*file->MxM() + stb)*2;
//	//fseek(file->m_pfile, CurSeek, SEEK_SET);
//	file->m_pfile->seek(CurSeek);
//	
//  //we read out the lower size
//	//fread(&value, sizeof(value), 1, file->m_pfile);
//	file->m_pfile->read((char*)&value,sizeof(value));
//
//	if (reverseReadFlag) reverseValue((char*)&value, sizeof(value) );
//
//	if (path != "") delete file; //ENG Close the file if we opened it
//	return (int)value;
//
//}
/*
//==================================
//!Obtaining height from a <Height matrix>
int MKarta::GetHeightFromMatrixes(MPoint& pt, double* hRel, double* hObj)
//==================================
{
    short value = -20000;
    int value1 = GetHeightFromMatrix(pt);
    if(hRel) *hRel = value1;
    int value2 = GetHeightFromMatrixObject(pt);
    if(hObj) *hObj= value2;
    if(value1 != -20000) value = value1;
    if(value2 != -20000)
        if(value == -20000) value = value2;
        else value += value2;

    return (int)value;
}
*/
//==================================
//!Obtaining height from a <Height matrix>
int MKarta::GetHeightFromMatrixes(const MPoint& pt, double* hRel, double* hObj, double* hCor)
//==================================
{
    short value = -20000;
    int value1 = GetHeightFromMatrix(pt);
    //if 'NODATA' in the 'HeightMatrix'
    if ((bHeightToZero) && (value1 == 9999))
        value1 = 0;

    if(hRel) *hRel = value1;
    int value2 = GetHeightFromMatrixObject(pt);
    if(hObj) *hObj= value2;
    int value3 = GetHeightFromMatrixCorrection(pt);
    if(hCor) *hCor= value3;
    if(value1 != -20000) value = value1;
    if(value2 != -20000)
    {
        if(value == -20000) value = value2;
        else value += value2;
    }
    if(value3 != -20000)
    {
        if(value == -20000) value = value3;
        else value += value3;
    }

    return (int)value;
}
//==================================
//!Getting Values from <Height matrix> i Matrica Vysot Objectov
int MKarta::GetHeightFromMatrixes(const MPoint& pt, MString path1, MString path2, double* hRel, double* hObj)
//==================================
{
    short value = -20000;
    int value1 = GetHeightFromMatrix(pt, path1);
    //if NODATA in the HeightMatrix
    if ((bHeightToZero) && (value1 == 9999))
        value1 = 0;
    if(hRel) *hRel = value1;
    int value2 = GetHeightFromMatrixObject(pt, path2);
    if(hObj) *hObj= value2;
    if(value1 != -20000) value = value1;
    if(value2 != -20000)
    {
        if(value == -20000) value = value2;
        else value += value2;
    }

    return (int)value;
}
//==================================
//!Getting Values from Matrica vysot,Matrica Vysot Objectov i Matrica Korrectirovki
int MKarta::GetHeightFromMatrixes(const MPoint& pt, MString path1, MString path2, MString path3, double* hRel, double* hObj, double* hCor)
//==================================
{
    short value = -20000;
    int value1 = GetHeightFromMatrix(pt, path1);
    //if NODATA in the HeightMatrix
    if ((bHeightToZero) && (value1 == 9999))
        value1 = 0;
    if(hRel) *hRel = value1;
    int value2 = GetHeightFromMatrixObject(pt, path2);
    if(hObj) *hObj= value2;
    int value3 = GetHeightFromMatrixCorrection(pt, path3);
    if(hCor) *hCor= value3;
    if(value1 != -20000)  value = value1;
    if(value2 != -20000)
    {
        if(value == -20000) value = value2;
        else value += value2;
    }
    if(value3 != -20000)
    {
        if(value == -20000){ value = value3;}
        else{ value += value3;}
    }

    return (int)value;
}

//===============================================
//!function returns distance from 'pt' point to the closest mark of height, if it
//! doesn't exceed the 'dopusk' admission, on all layers of a relief
bool  MKarta::FindNearestRlfPoint(MPoint pt, double dopusk, double* rasst, MPoint* heightpt)
//===============================================
{
    rasst[0]=-1;
    double dist;
    MPoint hpt;

    //we look for the first layer of a relief in which there is height mark,
    //the distance to which is no more than admission
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())

    {

        MLayer* pLay = *it;
        if(pLay->WhatTheLayer()==ID_RELIEF_LAYER){
            if(((MReliefLayer*)pLay)->FindNearestRlfPoint(pt, dopusk, &dist, &hpt)){
                rasst[0] = dist;
                heightpt->x = hpt.x;
                heightpt->y = hpt.y;
                break;
            }
        }
        it++;
    }
    //if such layer wasn't or layers of a relief are absent...
    if(rasst[0]==-1) return false;

    //on all layers of a relief we look for height mark, distance to which minimum
    while(it != m_laylist.end())
    {
        MLayer* pLay = *it;
        if(pLay->WhatTheLayer()==ID_RELIEF_LAYER){
            if(((MReliefLayer*)pLay)->FindNearestRlfPoint(pt, dopusk, &dist, &hpt)){
                if(dist<rasst[0]){

                    rasst[0] = dist;
                    heightpt->x = hpt.x;
                    heightpt->y = hpt.y;
                }
            }
        }
        it++;
    }
    return true;
}

//==================================
//! Obtaining height of a point on all layers of a relief
double  MKarta::GetHeightFromRelief(MPoint pt) 
//==================================
{
    int kolrellayers = 0;//number of layers of a relief
    double height = -100000;

    //cycle on all layers of a relief
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        MLayer* pLay = *it;
        if(pLay->WhatTheLayer()==ID_RELIEF_LAYER){
            kolrellayers++;
            height = ((MReliefLayer*)pLay)->GetValueFromRelief(pt);
            if(height!=-100000)
                break;
        }
        it++;
    }

    return height;
}

//==================================
//! Obtaining height of a point on all layers of a relief
double  MKarta::GetHeightFromReliefByIzolines(MPoint pt) 
//==================================
{
    int kolrellayers = 0;//number of layers of a relief
    double height = -100000;

    //cycle on all layers of a relief
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        MLayer* pLay = *it;
        if(pLay->WhatTheLayer()==ID_RELIEF_LAYER){
            kolrellayers++;
            height = ((MReliefLayer*)pLay)->GetValueFromReliefByIzolines(pt);
            if(height!=-100000)
                break;
        }
        it++;
    }

    return height;
}

//==================================
//! Obtaining information for creation of a profile in the area of a section on all layers of a relief
/**GENERAL IDEA: we find points of intersection of the line of a section presented in the broken line form
//with horizontals, finally output array contains shifts from the beginning of the line
//section and height;
//'pLomLine' - the pointer on the array of coordinates of points of the line of a section presented in the broken line form
//output information - the index on the array of shifts from the beginning of the line of a profile and heights of reference points of a profile:
//generally dimension of the output array doesn't coincide with dimension of the entrance array
*/
bool  MKarta::GetProfileInfoByIzolines(MPtArray* pLomLine, MPtArray* ProfInfo, double dopusk) 
//==================================
{
    int kolrellayers = 0;//number of layers of a relief

    //we will get auxiliary arrays
    MPtArray* profar = new MPtArray();
    profar->ClearAll();
    MPtArray* relprofar;

    //cycle on all layers of a relief
    //for each layer of a relief we look for crossings of the gas pipeline with relief layer horizontals,
    //we calculate shift of a point of intersection from the beginning of the gas pipeline, and height of this point -
    //value of height of a horizontal, - so we receive each point of a profile of the gas pipeline;
    //if in a layer of a relief there are no horizontals, but there are height marks, choose height marks,
    //remote from the gas pipeline on distance, smaller the admission, we calculate shift of this point
    //from the beginning of the gas pipeline, and point height - height of the corresponding mark of height;
    //all described actions happen as 'GetProfileInfoByIzolines' relief layer
    //for each layer of a relief
    LAYITER it = m_laylist.begin();

    while (it != m_laylist.end())
    {
        MLayer* pLay = *it;
        if(pLay->WhatTheLayer()==ID_RELIEF_LAYER){
            kolrellayers++;
            relprofar = ((MReliefLayer*)pLay)->GetProfileInfoByIzolines(pLomLine,dopusk);
            profar->AppendTail(relprofar);
            delete relprofar;
        }
        it++;
    }
    //if in the map there are no relief layers...
    if(kolrellayers==0){
        // we release unnecessary memory
        delete profar;
        return false;
    }

    //nformation for the line of a profile of the gas pipeline needs to be added

    //values of height in nodal points of the gas pipeline;
    //we will calculate for a profile of value of height in nodal points of the gas pipeline;
    //if there are no data for correct calculation of height in nodal points,
    //we will give them the emergency value of height equal 0
    double dist = 0;//dist - the sum of lengths of links of a broken line to the considered link
    MPoint pt;
    for(int i=0; i<pLomLine->GetSize(); i++){
        pt.x = dist;
        pt.y = GetHeightFromReliefByIzolines(pLomLine->GetAt(i));
        if(pt.y==-100000)
            pt.y = 0;
        profar->AddTail(&pt);

        if(i!=pLomLine->GetSize()-1)
            dist += DLINASG(pLomLine->GetAt(i)[0], pLomLine->GetAt(i+1)[0]);
    }


    if(profar->GetSize()==0)
        return false;

    // we will order all elements of the output array on shift from the beginning of the line of a profile
    double mindist;
    int index;
    while(profar->GetSize() != 1){
        mindist = profar->GetAt(0)[0].x;
        index = 0;
        for(int i=1; i<profar->GetSize(); i++){
            if(profar->GetAt(i)[0].x < mindist){
                mindist = profar->GetAt(i)[0].x;
                index = i;
            }
        }
        ProfInfo->AddTail(profar->GetAt(index));
        profar->Delete(index);
    }
    ProfInfo->AddTail(profar->GetAt(0));
    profar->Delete(0);

    // we release unnecessary memory
    delete profar;

    return true;
}

//==================================
//!function finds all crossings of the gas pipeline with hydroobjects on all to relief layers
bool MKarta::FindCrosslinesWithGidro(MPtArray* plankontur, MPtArray* Gidrocoord, MPtArray* Gidroheight)
//==================================
{
    int kolrellayers = 0;//number of layers of a relief

    //we get the 'cooarray' and 'heightarray' arrays temporarily instead of the output 'Gidrocoord' arrays
    //and 'Gidroheight'
    MPtArray* cooarray =new MPtArray();
    MPtArray* heightarray =new MPtArray();
    MPtArray* relcooarray =new MPtArray();
    MPtArray* relheightarray =new MPtArray();

    //cycle on all layers of a relief
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        if((*it)->WhatTheLayer()==ID_RELIEF_LAYER){
            kolrellayers++;
            relcooarray->ClearAll();
            relheightarray->ClearAll();
            //if in a layer of a relief there are horizontals, will receive a profile where it is possible on horizontals
            FindCrosslinesWithGidro(plankontur, relcooarray, relheightarray);
            cooarray->AppendTail(relcooarray);
            heightarray->AppendTail(relheightarray);
        }
        it++;
    }
    delete relcooarray;
    delete relheightarray;

    //if in the map there are no relief layers...
    if(kolrellayers==0){
        // we release more unnecessary memory

        delete cooarray;
        delete heightarray;
        return false;
    }

    if(cooarray->GetSize()==0)
        return false;



    // we will order all elements of the output array on shift from the beginning of the line of a profile
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

    return true;
}

//==================================
//!function finds all crossings of the gas pipeline with roads on all to layers
/**ENTRANCE:
//'plankontur' - a gas pipeline broken line on the plan, 'kodarray' - the array of quad-code of objects, with which
//we look for crossing of the gas pipeline
//EXIT:
//function finds all crossings of the gas pipeline with roads and returns information in
//three arrays: 'Roadcoord', point which elements: x-coordinate is a shift of the initial point
//crossings of the road and the gas pipeline from the beginning of the gas pipeline, y-coordinate is a shift final
//points of intersection of the road and the gas pipeline from the beginning of the gas pipeline; 'Roadheight', which elements
//correspond to elements of the 'Roadcoord' array and are heights of points of intersection: 'pt.x' - height
//points of intersection on an entrance, 'pt.y' - height of a point of intersection at the exit; 'pRoadObjects' - the array
//indexes on objects of the road which are crossed by the gas pipeline, to each element of the array of indexes
//'pRoadObjects' corresponds an element of the 'Roadcoord' and 'Roadheight' arrays
//all arrays are ordered on shift of the initial point of crossing of the gas pipeline and the road
*/
bool MKarta::FindCrosslinesWithRoads(MPtArray* plankontur, MStArray* kodar, double dopusk, 
                                     MVoidArray* pRoadObjects, MPtArray* Roadcoord, MPtArray* Roadheight)
//==================================
{
    //all arrays have to be initialized
    if((!pRoadObjects)||(!Roadcoord)||(!Roadheight)) return false;
    //entrance arrays have to be not only are initialized, but also shouldn't be empty
    if((!plankontur)||(plankontur->GetSize()<2)) return false;
    if((!kodar)||(kodar->GetSize()==0)) return false;

    //we get the 'cooarray', 'heightarray' and 'objarray' arrays temporarily instead of the output 'Roadcoord' arrays,
    //'Roadheight' and 'pRoadObjects'
    MPtArray* cooarray =new MPtArray();
    MPtArray* heightarray =new MPtArray();
    MVoidArray* objarray = new MVoidArray();
    MPtArray* relcooarray =new MPtArray();
    MPtArray* relheightarray =new MPtArray();
    MVoidArray* relobjarray = new MVoidArray();

    //we will receive a gas pipeline profile
    MPtArray* profar = new MPtArray();
    GetProfileInfoByIzolines(plankontur, profar, dopusk);

    //cycle on all layers - in each layer we make the list of roads, quad-code which coincides
    //with one of quad-codes of the entrance list, on all roads of the made list we look for
    //crossings of roads with the gas pipeline;
    //we keep all crossings of a separate layer in the arrays 'relcooarray', 'relheightarray', 'relobjarray';
    //we accumulate all crossings on layers in the arrays 'cooarray', 'heightarray', 'objarray';
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        relcooarray->ClearAll();
        relheightarray->ClearAll();
        relobjarray->ClearAll();
        (*it)->FindCrosslinesWithRoads(plankontur,profar,kodar,relobjarray,relcooarray,relheightarray);
        cooarray->AppendTail(relcooarray);
        heightarray->AppendTail(relheightarray);
        objarray->AppendTail(relobjarray);
        it++;
    }
    delete relcooarray;
    delete relheightarray;
    delete relobjarray;

    delete profar;

    //if didn't find any crossing of the gas pipeline with the road, quad-code which coincides
    //with one of quad-codes of the entrance list
    if(cooarray->GetSize()==0)
        return false;

    // we will order all elements of the output array on shift from the beginning of the line of a profile
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

    return true;
}

//==================================
//! we adjust that all layers were visible
void MKarta::MinimizeGeobounds()
//==================================
{
    LAYITER it = m_laylist.begin();
    m_geobounds = (m_ismaket)? m_maketlist : (*it)->m_bounds;
    while (it != m_laylist.end())
    {
        if ((*it)!=NULL) m_geobounds.UnionGeoRect(m_geobounds,(*it)->m_bounds);
        it++;
    }
}

//==================================
//! Minimization of borders of layers
void MKarta::Minimizelayerbounds() 
//==================================
{
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        (*it)->MinimizeBounds();
        it++;
    }
}

//===========================================================
//! Drawing up the complete list of all objects of the map in all layers
//! NB: defiant function has to delete the list!
MObjectList* MKarta::GetFullListOfMapObjects()
//===========================================================
{
    MObjectList* mapoblist = new MObjectList;
    MObjectList* layoblist;

    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {	//on layers
        layoblist = (*it)->CreateObjectList();
        mapoblist->insert(mapoblist->end(),layoblist->begin(),layoblist->end());
        it++;
    }
    return mapoblist;
}

//=======================================================================
//!To Sformitrovat the list of objects with such quad-code
void MKarta::GetObjectListByQuadrakod(MObjectList* oblist,MString quadrakod)
//=======================================================================
{
    if(!oblist) return ;
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        //We look for objects of a layer with the necessary quad-code
        (*it)->AddObjectsInObjectListByQuadrakod(oblist,quadrakod);
        it++;	//transition to the following layer
    }
}

//===================================================================================
//!Deletes from objects of the map with set quad-code a vector and splines
//! formula bindings with number 'formnomer'
bool MKarta::DeleteFormulaPrivs(int formnomer, MString quadrakod)
//===================================================================================
{
    //we will collect all objects of a layer with an entrance quad-code
    MObjectList oblist;
    GetObjectListByQuadrakod(&oblist,quadrakod);

    MString str;
    MZnak* pZnak = m_pKls->GetZnak(quadrakod,str);
    //if there is no sign in the classifier with an entrance quad-code, nothing is done
    if(!pZnak)
        return false;
    MYaz* pYaz = pZnak->GetYaz();
    if(!pYaz)
        return false;
    //if the index of the deleted formula goes beyond indexes of formulas
    //object of the classifier, we do nothing
    if((formnomer<0)||(formnomer>=pYaz->m_formula->GetSize()))
        return false;

    MTopobject* pTpo;
    OBITER it = oblist.begin();
    while (it != oblist.end())
    {
        if((*it)->GetIerarhy().Find(ID_TOPOBJECT)!=-1){
            pTpo = (MTopobject*)(*it);
            pTpo->DeleteVektorsPriv(formnomer);
        }
        it++;
    }
    return true;

}

//===================================================================================
//!For objects of the map with set the quad-code changes number of a formula 'oldformnom'
//! in arrays of vectors and splines of bindings of formulas on number 'newformnom'
bool MKarta::ChangeFormulaNomer(int oldformnom, int newformnom, MString quadrakod)
//===================================================================================
{
    //we will collect all objects of a layer with an entrance quad-code
    MObjectList oblist;
    GetObjectListByQuadrakod(&oblist,quadrakod);

    MString str;
    MZnak* pZnak = m_pKls->GetZnak(quadrakod,str);
    //if there is no sign in the classifier with an entrance quad-code, nothing is done
    if(!pZnak)
        return false;
    MYaz* pYaz = pZnak->GetYaz();
    if(!pYaz)
        return false;
    //if an initial index of a formula negative, we do nothing
    if(oldformnom<0)
        return false;
    //if a final index of a formula negative, we do nothing
    if(newformnom<0)
        return false;

    MTopobject* pTpo;
    OBITER it = oblist.begin();
    while (it != oblist.end())
    {
        if((*it)->GetIerarhy().Find(ID_TOPOBJECT)!=-1){
            pTpo = (MTopobject*)(*it);
            pTpo->ChangeFormulaNomer(oldformnom, newformnom);
        }
        it++;
    }
    return true;
}

//=====================================
//!to receive amount of objects in the map
int  MKarta::GetObjectCount()
//=====================================
{
    int count = 0;
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        count+=(*it)->GetObjectCount();
        it++;
    }

    return count;
}

//==================================
//! finds in the map object, the next to the set point
MObject* MKarta::FindNearestObject(MPoint findpt)
//==================================
{
    // we will make the list of the next objects on all layers
    MObjectList oblist;

    MObject*	pObj;

    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        //We don't consider invisible layers
        if(!(*it)->x_visible){it++;continue;};

        pObj = (*it)->FindNearestObject(findpt);

        if(pObj!=NULL) oblist.push_back(pObj);
        it++;
    }

    if(!oblist.size())return NULL;
    // final: we find the champion on layers

    OBITER it_o = oblist.begin();

    double distance, newdistance;
    // cycle prolog
    /*	while (it_o != oblist.end())
    {
        distance = pObj->FindDistanceFromPoint(findpt);
        if(distance>=0){
            pFindObj = pObj;
            break;
        }
        it_o++;
    }*/
    distance = (*it_o)->FindDistanceFromPoint(findpt);
    MObject* pFindObj = *it_o;
    //a cycle on objects
    while (it_o != oblist.end())
    {
        //		if(pObj!=NULL) {
        //			if(pObj->m_bounds.IsPtInRect(findpt)) {
        newdistance = (*it_o)->FindDistanceFromPoint(findpt);
        if((newdistance>=0)&&(newdistance<distance)) {
            distance = newdistance;
            pFindObj = *it_o;
        }
        //			}
        //		}
        ++it_o;
    }


    return pFindObj;
}


//!==================================
bool MKarta::MifFileRead(MString filename)
//==================================
{
    //Files necessary for map reading (.mif, .mid)
    FILE* FileMif = fopen(filename, "r");
    char buff[100];
    MString str_fmid;
    int m=filename.GetLength();
    str_fmid = filename.Left(m - 3) + "mid";
    //	int kk=0;

    FILE* FileMid = fopen(str_fmid, "r");
    if(!FileMid)
    {
        //		if (InternalMessage("Файл .mid не найден.\nЕсли он необходим, поместите его в данный каталог.")== IDOK) return false;
        //		else kk=1;
    }
    else fseek(FileMid, 0, SEEK_SET);

    MLayer* pLay = new MLayer(m_geobounds);
    AddLayer(pLay);

    //--------------------the used variables, members of classes--------------

    MString str_mif;//line from the file .mif
    MString str_mid;//line from the file .mid
    //	int percent;
    int dl_mif; //file length .mif
    MString chset;//the code page, which character set is used
    char delimiter= 0x00;//the symbol used as a divider
    MInArray unique;//unique column
    MInArray index;//index
    MString coordsys;//coordinate system
    int columns=0;//number of columns in the file .mid
    //	int sglajiv = 0;// sign of smoothing of the polyline (it is only read)
    //****	char razdelit;//the used divider
    char buf[100];
    MTopobject* pTpo;
    //	MScen* pScn;
    MRect	mrect;
    //	int nSize=1;
    MString str_buf;
    //	MPoint center;//coordinates an area centrode (are only read)
    /****	THESE parameters aren't used further
    int pen_shirina;//width of a pen (is transferred)

    int pen_tip;//style of a pen (it is only read for each object, and the parameters set are used)
    int pen_svet;//color of a pen (it is only read)
    int br_shablon;//shading template (it is only read)

    int br_svet_osn;//primary color (it is only read)
    int br_svet_fon;//color of a background (it is only read)
    int sym_vid;// code of a symbol to which there corresponds a certain look ()
    int sym_svet;// color of a symbol (it is only read)
    int sym_razmer;// the symbol size in points ()
    int kol_data;//data file size *****/
    //	int rez=1;// flag of lack of the necessary layer

    float pointX, pointY, pointX1, pointY1;//coordinates of points of object
    float a;//extent of smoothing of angles of a polygon
    //------------------------------------------------------------


    //	int n_list = m_laylist.size();

    //	GetDoc()->SetPaneText(1,"Reading file .MIF");
    //	percent=0;
    fseek(FileMif, 0, SEEK_END);//file length .MIF
    dl_mif = ftell(FileMif);
    fseek(FileMif, 0, SEEK_SET); // the pointer for the beginning of the file .mif
    fseek(FileMid, 0, SEEK_SET); // the pointer for the beginning of the file .mid

    // Reading lines of the file .mif
    while ((fgets(buff, 100, FileMif)) != nullptr)
    {
        str_mif = buff;
        int dl=str_mif.GetLength();
        unsigned int dwPos = ftell(FileMif);
        float ty=(float)dwPos/dl_mif;
        //	percent=(int)(ty*100);
        //		GetDoc()->SetPanePercent(2,percent);

        // the name of the code page which character set is used
        if (str_mif.Find("CharSet")!=-1)
        {
            int simvol = str_mif.Find(" ");
            int dlina = str_mif.GetLength();
            chset = str_mif.Right(dlina-(simvol+1));
        }
        // the symbol used as a divider
        if (str_mif.Find("Delimiter")!=-1)
        {
            int simvol = str_mif.Find(" ");
            MString delim;
            delim = str_mif.Mid((simvol+2), 1);
            delimiter = delim.GetAt(0);
        }
        // if this line is present, data are stored NOT in a width/longitude format
        if (str_mif.Find("CoordSys")!=-1)
        {
            MString s_coord;
            coordsys = str_mif.Right(dl-9);
        }
        // the number of columns, then for each column - the name, type of data, the field size is set
        if (str_mif.Find("Columns")!=-1)
        {
            MString s_col;
            s_col = str_mif.Right(dl-8);
            columns = atoi(s_col);
            for (int k=0; k<columns; k++)
            {
                fgets(buff, 100, FileMif);
                str_mif = buff;
                MString ss;
                str_mif.TrimLeft();
                int simvol = str_mif.Find(" ");
                ss = str_mif.Left(simvol);
                if (k<3) pLay->m_dbarray.SetAt(k,&ss);
                if (k>=3) pLay->m_dbarray.SetAtGrow(k,&ss);
            }
        }

        //The used graphic objects


        //POINT//
        if (str_mif.Find("POINT")!=-1)
        {
            str_buf = str_mif.Right(dl-6);
            unsigned int q = 0;
            for(q=0; q < sizeof(buf);q++) buf[q] = 0;
            for(q=0; q <str_buf.GetLength() ; q++) buf[q] = str_buf.GetAt(q);
            int result = sscanf( buf, "%f %f", &pointX, &pointY);
            if(result==0) return false;
            pTpo = new MTopobject();
            pLay->AddObject(pTpo);

            MPoint pt;
            pt.x = pointX;
            pt.y = pointY;
            pTpo->AddPoint(&pt);

            pTpo->ConnectToServKls(F_DOT);

            str_buf.Empty();
            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {
                MString stolb;
                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                if (k<3) pTpo->m_dbarray.SetAt(k,&stolb);
                if (k>=3) pTpo->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
            //Graphic characteristics have to be described here
        }


        //AREA//
        if (str_mif.Find("Region")!=-1)
        {
            str_buf = str_mif.Right(dl-7);
            pTpo = new MTopobject;
            pTpo->m_Komplekt->DeleteTail();
            int numpolygons = atoi(str_buf);// quantity of parts in top object
            for (int sp=0; sp<numpolygons; sp++)
            {
                fgets(buff, 100, FileMif);
                str_mif = buff;
                int numpts = atoi(str_mif);// quantity of points in one part
                MPtArray* pta = new MPtArray();
                for(int s=0; s<numpts; s++)
                {
                    fgets(buff, 100, FileMif);
                    str_mif = buff;
                    unsigned int q = 0;
                    for(q=0; q < sizeof(buf);q++) buf[q] = 0;
                    for(q=0; q < str_mif.GetLength() ; q++) buf[q] = str_mif.GetAt(q);
                    int result = sscanf( buf, "%f %f", &pointX, &pointY);
                    if(result==0) return false;
                    MPoint pt;
                    pt.x = pointX;
                    pt.y = pointY;
                    pta->AddTail(&pt);
                }
                pTpo->m_Komplekt->AddTail((MArray*)pta);
            }
            pTpo->RecalcBounds();
            pLay->AddObject(pTpo);

            pTpo->ConnectToServKls(F_PLO);

            str_buf.Empty();

            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {
                MString stolb;
                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                if (k<3) pTpo->m_dbarray.SetAt(k,&stolb);
                if (k>=3) pTpo->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
        }

        //STRAIGHT LINE//
        if (str_mif.Find("Line")!=-1)
        {
            str_buf = str_mif.Right(dl-5);
            unsigned int q = 0;
            for(q=0; q < sizeof(buf);q++) buf[q] = 0;
            for(q=0; q <str_buf.GetLength() ; q++) buf[q] = str_buf.GetAt(q);
            int result = sscanf( buf, "%f %f %f %f", &pointX, &pointY, &pointX1, &pointY1);
            if(result==0) return false;

            //			pScn = new MScen(MRect(mrect.left,mrect.top,mrect.right,mrect.bottom));
            mrect.left = pointX;
            mrect.top = pointY;
            mrect.right = pointX1;
            mrect.bottom = pointY1;
            //			pScn->m_bounds = mrect;
            //			pScn->m_type = SCN_PRAMAYA;
            //			pLay->AddObject(pScn);
            str_buf.Empty();
            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {
                MString stolb;

                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                //				if (k<3) pScn->m_dbarray.SetAt(k,&stolb);
                //				if (k>=3) pScn->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
        }


        //BROKEN LINE//
        if (str_mif.Find("Pline")!=-1)

        {
            str_buf = str_mif.Right(dl-5);// that after 'Pline'
            MString str_part;// line - quantity of sections
            pTpo = new MTopobject();
            pLay->AddObject(pTpo);

            pTpo->ConnectToServKls(F_LIN);

            int numsections, numpts;// quantity of sections, quantity of points in each section
            // if some sections, we define how many, and quantity of points in the 1st section
            if (str_buf.Find("Multiple")!=-1)
            {
                pTpo->m_Komplekt->DeleteTail();
                str_part = str_buf.Right(str_buf.GetLength()-9);
                numsections = atoi(str_part);
                for (int sp=0; sp<numsections; sp++)//for all section
                {
                    fgets(buff, 100, FileMif);
                    str_mif = buff;
                    numpts = atoi(str_mif);
                    MPtArray* pta = new MPtArray();
                    for (int s=0; s<numpts; s++)// for all points in a section
                    {
                        fgets(buff, 100, FileMif);
                        str_mif = buff;
                        unsigned int q = 0;
                        for(q=0; q < sizeof(buf);q++) buf[q] = 0;
                        for(q=0; q<str_mif.GetLength() ; q++) buf[q] = str_mif.GetAt(q);

                        int result = sscanf( buf, "%f %f", &pointX, &pointY);

                        if(result==0) return false;
                        MPoint pt;
                        pt.x = pointX;
                        pt.y = pointY;
                        pta->AddTail(&pt);
                    }
                    pTpo->m_Komplekt->AddTail((MArray*)pta);
                }
                pTpo->RecalcBounds();
                str_buf.Empty();
            }
            else // if section one, we define quantity of points in section
            {
                numsections = 1;
                numpts = atoi(str_buf);
                for (int s=0; s<numpts; s++)
                {
                    fgets(buff, 100, FileMif);
                    str_mif = buff;
                    unsigned int q = 0;
                    for(q=0; q < sizeof(buf);q++) buf[q] = 0;
                    for(q=0; q < str_mif.GetLength() ; q++) buf[q] = str_mif.GetAt(q);
                    int result = sscanf( buf, "%f %f", &pointX, &pointY);
                    if(result==0) return false;
                    MPoint pt;
                    pt.x = pointX;
                    pt.y = pointY;
                    pTpo->AddPoint(&pt);
                }
                pTpo->RecalcBounds();
                str_buf.Empty();
            }
            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {
                MString stolb;
                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                if (k<3) pTpo->m_dbarray.SetAt(k,&stolb);
                if (k>=3) pTpo->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
        }

        //RECTANGLE//
        if (str_mif.Find("Rect")!=-1)
        {
            str_buf = str_mif.Right(dl-5);
            unsigned int q = 0;
            for(q=0; q < sizeof(buf);q++) buf[q] = 0;
            for(q=0; q < str_buf.GetLength() ; q++) buf[q] = str_buf.GetAt(q);
            int result = sscanf( buf, "%f %f %f %f", &pointX, &pointY, &pointX1, &pointY1);
            if(result==0) return false;
            //			pScn = new MScen(MRect(mrect.left,mrect.top,mrect.right,mrect.bottom));
            mrect.left = pointX;
            mrect.top = pointY;
            mrect.right = pointX1;
            mrect.bottom = pointY1;
            //			pScn->m_bounds = mrect;
            //			pScn->m_type = SCN_PRAMOUGOLNIK;
            //			pLay->AddObject(pScn);
            str_buf.Empty();
            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {
                MString stolb;
                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                //				if (k<3) pScn->m_dbarray.SetAt(k,&stolb);
                //				if (k>=3) pScn->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
        }

        //THE ROUNDED RECTANGLE//
        if (str_mif.Find("Roundrect")!=-1)
        {
            str_buf = str_mif.Right(dl-5);
            unsigned int q = 0;
            for(q=0; q < sizeof(buf);q++) buf[q] = 0;
            for(q=0; q <str_buf.GetLength() ; q++) buf[q] = str_buf.GetAt(q);
            int result = sscanf( buf, "%f %f %f %f", &pointX, &pointY, &pointX1, &pointY1);
            if(result==0) return false;
            //			pScn = new MScen(MRect(mrect.left,mrect.top,mrect.right,mrect.bottom));
            mrect.left = pointX;
            mrect.top = pointY;
            mrect.right = pointX1;
            mrect.bottom = pointY1;
            fgets(buff, 100, FileMif);
            str_mif = buff;
            a = atof(str_mif);
            MPoint pt;
            pt.x = pointX1-a;

            pt.y = pointY-a;
            //			((MPtArray*)pScn->m_Komplekt->GetAt(SCN_ROUNDNESS))->SetAt(0,&pt);
            //			pScn->m_bounds = mrect;
            //			pScn->m_type = SCN_KRUGLOUGOLNIK;
            //			pLay->AddObject(pScn);
            str_buf.Empty();
            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {
                MString stolb;
                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                //				if (k<3) pScn->m_dbarray.SetAt(k,&stolb);
                //				if (k>=3) pScn->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
        }

        //ELLIPSE//
        if (str_mif.Find("Ellipse")!=-1)
        {
            str_buf = str_mif.Right(dl-5);
            unsigned int q = 0;
            for(q=0; q < sizeof(buf);q++) buf[q] = 0;
            for(q=0; q <str_buf.GetLength() ; q++) buf[q] = str_buf.GetAt(q);
            int result = sscanf( buf, "%f %f %f %f", &pointX, &pointY, &pointX1, &pointY1);
            if(result==0) return false;
            //			pScn = new MScen(MRect(mrect.left,mrect.top,mrect.right,mrect.bottom));
            mrect.left = pointX;
            mrect.top = pointY;
            mrect.right = pointX1;
            mrect.bottom = pointY1;
            //			pScn->m_bounds = mrect;

            //			pScn->m_type = SCN_ELLIPS;
            //			pLay->AddObject(pScn);
            str_buf.Empty();
            int NN = pLay->m_oblist.size();
            if (NN>0)
                pLay->MinimizeBounds();

            fgets(buff, 100, FileMid);
            str_mid = buff;
            MString ss = str_mid + ',';
            for (int k=0; k<columns; k++)
            {

                MString stolb;

                int dlss = ss.GetLength();
                int n = ss.Find(delimiter);
                stolb = ss.Left(n);
                //				if (k<3) pScn->m_dbarray.SetAt(k,&stolb);
                //				if (k>=3) pScn->m_dbarray.SetAtGrow(k,&stolb);
                ss = ss.Right(dlss-n-1);
            }
        }

        MinimizeGeobounds();
    }
    return true;
}

//==================================
//SHP-file reading from filepath 'filename'
//the read file is added to map as a new layer
bool MKarta::ShapefileRead(MString filename)
//==================================
{
    MShapefile shp;
    // open file
    bool retkod = shp.OpenFile(filename);
    if(!retkod) return false;

    MLayer* pLay = new MLayer(m_geobounds);
    AddLayer(pLay);

    // we do inscriptions in a layer by default invisible
    pLay->x_textvisible = false;

    // type of top objects
    int shapetype = shp.GetFileType();

    // preparation of casual color for linear and
    //dot top objects
    MColor randcolor(rand()/211,rand()/211,rand()/211);

    // layer georectangle
    shp.GetMapRect(&pLay->m_bounds);
    pLay->m_bounds.NormalizeGeoRect();

    // for giving of random colours to top objects
    srand( (unsigned)time( NULL ) );


    int NN = shp.GetNumberOfRecords();

    // for progress indication
    this->SetProgress(filename.c_str());

    //int col = 0;
    //int lastcol = 0;

    for(int n=0; n<NN; n++) {

        // new top object
        MRect firstPosition(0,0,0,0); // the describing rectangle
        MTopobject* mpt = new MTopobject(firstPosition);
        // add to drawing
        pLay->AddObject(mpt,false);

        // number of points and parts
        int nPoints =  shp.GetNumberOfRecPoints(n);
        int nParts =  shp.GetNumberOfRecParts(n);
        // if dot, it is necessary to put these
        //values it is artificial


        if (shapetype == 1 || shapetype == 8 || shapetype == 11) {

            nParts = 1;
        }
        if (shapetype == 1 || shapetype == 11) {
            nPoints = 1;
        }


        // place for the array of points
        //if (mpt->m_points != NULL) delete[] mpt->m_points;
        //mpt->m_points = new MPoint[nPoints];
        int ii;
        MPtArray* pta=(MPtArray*)mpt->m_Komplekt->GetAt(mpt->F_TOCHKI);
        pta->SetSize(0);
        for(ii=1;ii<nParts;ii++)
        {
            pta = new MPtArray();
            mpt->m_Komplekt->AddTail((MArray*)pta);
        }

        MPoint* m_points = (MPoint*) malloc(sizeof(MPoint)*nPoints);
        // place for the array of entry points
        //if (mpt->m_parts != NULL) delete[] mpt->m_parts;
        //mpt->m_parts = new int[nParts];

        int* m_parts = (int*) malloc(sizeof(int)*nParts);
        m_parts[0]=0;

        MRect grect;

        shp.GetRecord(&grect,m_parts,m_points,n);

        for (ii=0; ii<nParts;ii++)
        {
            int Beg=m_parts[ii];
            int End=(ii==nParts-1)? nPoints : m_parts[ii+1];
            pta=(MPtArray*)mpt->m_Komplekt->GetAt(mpt->F_TOCHKI+ii);
            /*
            slow way - on one point
            for(int j=Beg;j<End;j++)
            {
                    pta->AddTail(&m_points[j]);
            }
            */
            // faster way - all points at once
            pta->FastSet(End-Beg,&m_points[Beg]);
        }

        mpt->m_bounds = grect;
        mpt->m_bounds.NormalizeGeoRect();


        // if dot, it is necessary to deliver 'bounds' artificially
        if (shapetype == 1 || shapetype == 11) {
            mpt->RecalcBounds();
        }

        // what type object?
        //we tie to the service classifier
        //areal
        if		(shapetype == 5)
        {
            mpt->ConnectToServKls(F_PLO);
            //if the first point isn't equal to the last, we will close a contour
            pta=(MPtArray*)mpt->m_Komplekt->GetAt(mpt->F_TOCHKI);
            if (pta->Size>3)
            {
                if (pta->GetAt(0)->Distance(pta->GetAt(pta->GetSize()-1)) > 0.0000001)
                {
                    MPoint geopoint;
                    geopoint = pta->GetAt(0);
                    pta->AddTail(&geopoint);
                }
            }
        }
        // linear
        else if (shapetype == 3) mpt->ConnectToServKls(F_LIN);
        // if dot or multipoints
        else if ((shapetype == 1)||(shapetype == 8)||(shapetype == 11)) mpt->ConnectToServKls(F_DOT);
        // if the type isn't distinguished, we generate linear object
        else mpt->ConnectToServKls(F_LIN);

        //progress indication

        //col++;
        //if(col >= NN/100) {
        //	lastcol += col;
        // we will bring data to the panel
        //it was: //int percent = (int)(100*((double)lastcol/(double)NN));

        //percent variable is from MKarta
        percent = (int)(100*((double)n/(double)NN));
        ShowProgress();

        //col = 0;
        //}

    }//EoFOR for (n=0;n<NN;n++)..
    shp.CloseFile();

    // ----------------------------
    //  reading attributive information from the dbf-file
    MDbffile dbf;
    int N;
    char type;

    // we define the name of a layer
    filename = filename.Left(filename.GetLength() - 4);
    int k = filename.ReverseFind(QDir::separator().toLatin1());
    pLay->m_name = filename.Right(filename.GetLength()-k-1);

    // the dbf-file corresponding to shp-file, we look for in the same place where took shp-file
    filename += ".dbf";

    // open dbf-file
    retkod = dbf.OpenFile(filename);
    // if dbf-file didn't find, we are content with reading shp-file
    if(!retkod) return true;
    // we define number of fields N in the dbf-file
    N = dbf.GetFieldsNumber();
    // we read out names of fields from the dbf-file in the layer 'dbarray' array;
    //m_dbarray[0] stores the name of a layer received from the name SHP file;
    //other elements of the 'm_dbarray' array store names of fields of the corresponding dbf-file
    pLay->m_dbarray.SetSize(N);

    for(int i=0; i<N; i++)
    {
        MString str= dbf.GetFieldName(i);
        pLay->m_dbarray.SetAt(i,&str);
    }

    //  for progress indication:
    //we define amount of objects
    NN = pLay->m_oblist.size();
    SetProgress(filename.c_str());
    int col = 0;
    //	lastcol = 0;
    int index = 0;

    OBITER it = pLay->m_oblist.begin();
    while (it != pLay->m_oblist.end())
    {

        if ((*it)->WhatTheObject()==ID_TOPOBJECT) {
            MTopobject* pFgr = (MTopobject*)(*it);


            //progress indication
            //if(col >= NN/100) {
            //lastcol += col;
            // we will bring data to the panel
            col++;
            percent = (int)(100*((double)col/(double)NN));
            ShowProgress();
            //col = 0;

            for(int j=0; j<N; j++){
                unsigned lf = dbf.GetFieldLength(j); // lf-length of the chosen field
                type = dbf.GetFieldType(j);
                char* buff;
                buff = (char*)malloc(lf+4);
                switch (type)
                {
                //if text field
                case 'C' :{
                    MString str = dbf.ReturnString(j,index);
                    pFgr->m_dbarray.SetAtGrow(j,&str);
                    break;}
                    //if integer
                case 'N' :{
                    int l = sprintf(buff, "%d ", dbf.GetIntNumber(j,index));
                    MString str= "";
                    pFgr->m_dbarray.SetAtGrow(j,&str);
                    for(int i =0;i<l;i++) *pFgr->m_dbarray[j] += " ";
                    *pFgr->m_dbarray[j] = buff;
                    break;}
                    //if rational number
                case 'F' :{
                    int l = sprintf(buff, "%.3f ", dbf.GetDoubleNumber(j,index));
                    MString str= "";
                    pFgr->m_dbarray.SetAtGrow(j,&str);
                    for(int i =0;i<l;i++) *pFgr->m_dbarray[j] += " ";
                    *pFgr->m_dbarray[j] = buff;
                    break;}
                    //by default we read out data as a line
                default :{
                    MString str = dbf.ReturnString(j,index);
                    pFgr->m_dbarray.SetAtGrow(j,&str);
                    break; }
                }
                free (buff);

            } // end for(int j=0; j<N; j++)
            index++;
        } // end if (pObj->WhatTheObject()==ID_TOPOBJECT)
        it++;
    } // end while
    dbf.CloseFile();

    MinimizeGeobounds();
    return true;
}

//====================================
//!'WriteReverseInteger' function definition - records of an integer in 'big endian' (Sun or Motorola) a byte order
bool WriteReverseInteger(FILE* file, int n)
//====================================
{
    union {
        char str[4];
        int nn;
    };
    nn = n;
    for(int i = 0; i < 4; i++)
        fwrite(&str[3-i], sizeof(char), 1, file);

    return true;
}

//==================================
//! data recording about layer hyper points in a sheyp-format

//'type' - type of the written-down hyper points, a str-full way to the written-down file
//'dlina' - the pointer on the array of integers - lengths of fields of the written-down attributive file
//'col' - number of fields
//'names' - the pointer on the array of lines with names of fields of the written-down attributive file
bool MKarta::ShapefileWrite(MLayer* pLay, int type, MString str, MInArray* dlinaarr, MStArray* names)
//==================================
{
    bool bEngVer = false;
    int col = dlinaarr->GetSize();
    if(col==0)return false;
    int* dlina = dlinaarr->GetAt(0);
    //we minimize the description of layers of a layer
    pLay->MinimizeBounds();

    MString panetext;
    panetext = "Запись в шейп-формат";
    if (bEngVer)
        panetext = "Writing to SHP-file";
    //	GetDoc()->SetPaneText(0,panetext);

    // we open for record at once three files: *.shp, *.shx, *.dbf
    FILE* shpfile = fopen(str, "wb");
    int n = str.GetLength();
    str.SetAt(n-1, 'x');
    FILE* shxfile = fopen(str, "wb");
    str.SetAt(n-3, 'd');
    str.SetAt(n-2, 'b');
    str.SetAt(n-1, 'f');
    FILE* dbffile = fopen(str, "wb");

    // record of heading of the main file and heading of the index file
    WriteReverseInteger(shpfile, 9994);
    WriteReverseInteger(shxfile, 9994);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);

    //we will make the general list of objects
    MObjectList* pWorkObjlist;
    if (type == 5)
        pWorkObjlist = pLay->CreateObjectListByObjType(F_PLO);
    else pWorkObjlist = pLay->CreateObjectList();

    OBITER it;
    int num = 0; // 'num' - number of hyper points in a layer
    int numpoints = 0; // 'numpoints' - total number of points in a layer
    int numparts = 0; // 'numparts' - total number of parts in a layer
    if(pLay->WhatTheLayer() != ID_RELIEF_LAYER)
    {
        it = pWorkObjlist->begin();
        while (it != pWorkObjlist->end())
        {
            if ((*it)->WhatTheObject()==ID_TOPOBJECT) {
                MTopobject* pHPt = (MTopobject*)(*it);
                //MYaz* pYaz = pHPt->m_pZnak->GetYaz();

                num++;
                numpoints += pHPt->GetPointsNumber();
                numparts += pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI;
            }
            else
                if ((*it)->WhatTheObject()==ID_HEIGHTPOINT) {
                    MHeightPoint* pHPt = (MHeightPoint*)(*it);
                    num++;
                    numpoints += pHPt->GetPointsNumber();
                    numparts += pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI;
                }
            it++;
        }
    }
    else //if Relief Layer
    {
        it = ((MReliefLayer*)pLay)->m_izolines.begin();
        while (it != ((MReliefLayer*)pLay)->m_izolines.end())
        {
            if ((*it)->WhatTheObject()==ID_IZOLINE) {
                MIzoline* pHPt = (MIzoline*)(*it);
                num++;
                numpoints += pHPt->GetPointsNumber();
                numparts += pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI;
            }
            it++;
        }
    }
    // length of the main 'filelength' file consists of length of heading of the main file (100 bytes),
    //sums of lengths of headings of all records and lengths of the content of all records; it is measured in 16-digit words
    //see the description of a shp-format
    if(type == 1) {
        int shplength = (100 + num*(3*sizeof(int) + sizeof(double)*2))/2;
        WriteReverseInteger(shpfile, shplength);
    }
    else if(type == 8) {
        int shplength = (100 + num*(4*sizeof(int) + 4*sizeof(double)) + numpoints*sizeof(double)*2)/2;

        WriteReverseInteger(shpfile, shplength);

    }
    //(type == 5 or type == 3)  arches or polygons
    else {
        int shplength = (100 + (5*sizeof(int) + 4*sizeof(double))*num + numparts*sizeof(int) + numpoints*sizeof(double)*2)/2;
        WriteReverseInteger(shpfile, shplength);
    }

    int shxlength = (100 + 2*sizeof(int)*num)/2;
    WriteReverseInteger(shxfile, shxlength);

    n = 1000;
    fwrite(&n, sizeof(int), 1, shpfile);
    fwrite(&n, sizeof(int), 1, shxfile);
    fwrite(&type, sizeof(int), 1, shpfile);
    fwrite(&type, sizeof(int), 1, shxfile);
    MRect rect = pLay->m_bounds;
    fwrite(&rect.left, sizeof(double), 1,shpfile );
    fwrite(&rect.left, sizeof(double), 1, shxfile);
    fwrite(&rect.bottom, sizeof(double), 1, shpfile);
    fwrite(&rect.bottom, sizeof(double), 1, shxfile);
    fwrite(&rect.right, sizeof(double), 1, shpfile);
    fwrite(&rect.right, sizeof(double), 1, shxfile);
    fwrite(&rect.top, sizeof(double), 1, shpfile);
    fwrite(&rect.top, sizeof(double), 1, shxfile);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);
    WriteReverseInteger(shpfile, 0);
    WriteReverseInteger(shxfile, 0);

    // filling of heading of the dbf-file and the structures describing each field of a database
    //dbf-file heading
    n = 3;

    fwrite(&n, sizeof(char), 1, dbffile);
    n = 98;
    fwrite(&n, sizeof(char), 1, dbffile);
    n = 10;
    fwrite(&n, sizeof(char), 1, dbffile);
    n = 22;
    fwrite(&n, sizeof(char), 1, dbffile);
    n = num;
    fwrite(&n, sizeof(long), 1, dbffile);
    n = 32 + col*32 + 1;   // 32 bytes - length of heading and still 'col' of times of the description of fields on number of fields in the dbf-file on 32 bytes plus byte - a divider -
    //shift of the beginning of information records
    fwrite(&n, sizeof(unsigned short), 1, dbffile);
    // length of record turns out from lengths of fields plus additional byte at the beginning of record
    n = dlina[0];
    int i = 0, j=0;
    MString sst;
    for(i=1; i<col; i++)
        n += dlina[i];
    n++;
    fwrite(&n, sizeof(unsigned short), 1, dbffile);
    n = 0;
    for(i=0; i<20; i++)
        fwrite(&n, sizeof(char), 1, dbffile);
    // filling of the structures describing each field of a database
    for(j=0; j<col; j++){
        if(names->GetAt(j)->GetLength() == 0){
            sst = "NO_NAME";
            names->SetAt(j,&str);
        }
        str = *names->GetAt(j);
        char ch;
        for(unsigned int ii=0; ii<str.GetLength(); ii++) {
            ch = str[ii];
            fwrite(&ch, sizeof(char), 1, dbffile);
        }
        for(unsigned int jj=0; jj<11-str.GetLength(); jj++)
            fwrite(&n, sizeof(char), 1, dbffile);
        ch = 'C';
        fwrite(&ch, sizeof(char), 1, dbffile);
        for(i=0; i<4; i++)
            fwrite(&n, sizeof(char), 1, dbffile);
        n = dlina[j];
        fwrite(&n, sizeof(unsigned short), 1, dbffile);
        n = 0;
        for(i=0; i<14; i++)
            fwrite(&n, sizeof(char), 1, dbffile);
    }
    n = 13;
    fwrite(&n, sizeof(char), 1, dbffile);

    // filling of contents of information records for shp - shx-and dbf-files
    MString strinfo;
    char ch;
    int reclength;
    strinfo.Format("%d", num);
    //	GetDoc()->SetPaneText(1,strinfo);
    unsigned long nom = 0; // 'nom' - number of record
    int offset = 50; // 'offset' - shift of entry in the main file - number 16-digit
    //words from the beginning of the file to the first byte of heading of record

    if(pLay->WhatTheLayer() != ID_RELIEF_LAYER)
    {
        it = pWorkObjlist->begin();
        while (it != pWorkObjlist->end())
        {
            if ((*it)->WhatTheObject()==ID_TOPOBJECT) {
                MTopobject* pHPt = (MTopobject*)(*it);

                // filling of heading of records of the main file
                nom++;
                WriteReverseInteger(shpfile, nom);
                // ength of the record 'reclength' depends on number of parts and points in a hyper point, see the description of the shp-file
                if(type == 1) {
                    reclength = (sizeof(int) + sizeof(double)*2)/2;
                    WriteReverseInteger(shpfile, reclength);
                }
                else if(type == 8) {
                    reclength = (sizeof(int)*2 + sizeof(double)*4 + sizeof(double)*2*pHPt->GetPointsNumber())/2;
                    WriteReverseInteger(shpfile, reclength);
                }
                else {
                    reclength = (sizeof(int)*3 + sizeof(double)*4 + sizeof(int)*(pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI) + sizeof(double)*2*pHPt->GetPointsNumber())/2;
                    WriteReverseInteger(shpfile, reclength);
                }

                // filling of records of the index file
                WriteReverseInteger(shxfile, offset);
                WriteReverseInteger(shxfile, reclength);

                offset += 4 + reclength; // calculation of shift for the following record

                // filling of records of the main file
                fwrite(&type, sizeof(int), 1, shpfile);
                if(type == 1) {
                    MPoint point;
                    point.x = pHPt->GetPoint(0).x;
                    point.y = pHPt->GetPoint(0).y;
                    fwrite(&point.x, sizeof(double), 1, shpfile);
                    fwrite(&point.y, sizeof(double), 1, shpfile);
                }
                else if(type == 8) {
                    rect = (*it)->m_bounds;
                    fwrite(&rect.left, sizeof(double), 1, shpfile);
                    fwrite(&rect.bottom, sizeof(double), 1, shpfile);
                    fwrite(&rect.right, sizeof(double), 1, shpfile);
                    fwrite(&rect.top, sizeof(double), 1, shpfile);
                    numpoints = pHPt->GetPointsNumber();
                    fwrite(&numpoints, sizeof(int), 1, shpfile);
                    MPoint point;
                    for(i=0; i<numpoints; i++) {
                        point.x = pHPt->GetPoint(i).x;
                        point.y = pHPt->GetPoint(i).y;
                        fwrite(&point.x, sizeof(double), 1, shpfile);
                        fwrite(&point.y, sizeof(double), 1, shpfile);
                    }
                }
                else {
                    rect = (*it)->m_bounds;
                    fwrite(&rect.left, sizeof(double), 1, shpfile);
                    fwrite(&rect.bottom, sizeof(double), 1, shpfile);
                    fwrite(&rect.right, sizeof(double), 1, shpfile);
                    fwrite(&rect.top, sizeof(double), 1, shpfile);
                    numparts = pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI;
                    fwrite(&numparts, sizeof(int), 1, shpfile);
                    numpoints = pHPt->GetPointsNumber();
                    fwrite(&numpoints, sizeof(int), 1, shpfile);
                    int partsind; // 'partsind' - index of the initial point of each part
                    partsind=0;
                    for(i=0; i<numparts; i++) {
                        fwrite(&partsind, sizeof(int), 1, shpfile);
                        partsind+=pHPt->m_Komplekt->GetAt(pHPt->F_TOCHKI+i)->GetSize();
                    }
                    MPoint point;
                    for(i=0; i<numpoints; i++) {
                        point.x = pHPt->GetPoint(i).x;
                        point.y = pHPt->GetPoint(i).y;
                        fwrite(&point.x, sizeof(double), 1, shpfile);
                        fwrite(&point.y, sizeof(double), 1, shpfile);
                    }
                }

                // filling of information records of the dbf-file
                ch = ' ';
                fwrite(&ch, sizeof(char), 1, dbffile);
                MString ss = "";
                for(j=0; j<col;j++){
                    ch = ' ';
                    if(pHPt->m_dbarray.GetSize()<j+1)
                        pHPt->m_dbarray.SetAtGrow(j,&ss);

                    str = *pHPt->m_dbarray[j];
                    str.TrimLeft();
                    n = str.GetLength();
                    if (n>3)
                        n = 3;
                    for(i=0; i<dlina[j]-n; i++)
                        fwrite(&ch, sizeof(char), 1, dbffile);
                    for(i=0; i<n; i++) {
                        ch = str[i];
                        fwrite(&ch, sizeof(char), 1, dbffile);
                    }
                }

                num = num-1;
                strinfo.Format("%d", num);
                //			GetDoc()->SetPaneText(1,strinfo);

            }
            else
                if ((*it)->WhatTheObject()==ID_HEIGHTPOINT) {
                    MHeightPoint* pHPt = (MHeightPoint*)(*it);

                    // filling of heading of records of the main file
                    nom++;
                    WriteReverseInteger(shpfile, nom);
                    // length of the record 'reclength' depends on number of parts and points in a hyper point, see the description of the shp-file
                    if(type == 1) {
                        reclength = (sizeof(int) + sizeof(double)*2)/2;
                        WriteReverseInteger(shpfile, reclength);
                    }
                    else if(type == 8) {
                        reclength = (sizeof(int)*2 + sizeof(double)*4 + sizeof(double)*2*pHPt->GetPointsNumber())/2;
                        WriteReverseInteger(shpfile, reclength);
                    }
                    else {
                        reclength = (sizeof(int)*3 + sizeof(double)*4 + sizeof(int)*(pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI) + sizeof(double)*2*pHPt->GetPointsNumber())/2;
                        WriteReverseInteger(shpfile, reclength);
                    }

                    // filling of records of the index file
                    WriteReverseInteger(shxfile, offset);
                    WriteReverseInteger(shxfile, reclength);

                    offset += 4 + reclength; // calculation of shift for the following record

                    // filling of records of the main file
                    fwrite(&type, sizeof(int), 1, shpfile);
                    if(type == 1) {
                        MPoint point;
                        point.x = pHPt->GetPoint(0).x;
                        point.y = pHPt->GetPoint(0).y;
                        fwrite(&point.x, sizeof(double), 1, shpfile);
                        fwrite(&point.y, sizeof(double), 1, shpfile);
                    }
                    else if(type == 8) {
                        rect = (*it)->m_bounds;
                        fwrite(&rect.left, sizeof(double), 1, shpfile);
                        fwrite(&rect.bottom, sizeof(double), 1, shpfile);
                        fwrite(&rect.right, sizeof(double), 1, shpfile);
                        fwrite(&rect.top, sizeof(double), 1, shpfile);
                        numpoints = pHPt->GetPointsNumber();
                        fwrite(&numpoints, sizeof(int), 1, shpfile);
                        MPoint point;
                        for(i=0; i<numpoints; i++) {
                            point.x = pHPt->GetPoint(i).x;
                            point.y = pHPt->GetPoint(i).y;
                            fwrite(&point.x, sizeof(double), 1, shpfile);
                            fwrite(&point.y, sizeof(double), 1, shpfile);
                        }
                    }
                    else {
                        rect = (*it)->m_bounds;
                        fwrite(&rect.left, sizeof(double), 1, shpfile);
                        fwrite(&rect.bottom, sizeof(double), 1, shpfile);
                        fwrite(&rect.right, sizeof(double), 1, shpfile);
                        fwrite(&rect.top, sizeof(double), 1, shpfile);
                        numparts = pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI;
                        fwrite(&numparts, sizeof(int), 1, shpfile);
                        numpoints = pHPt->GetPointsNumber();
                        fwrite(&numpoints, sizeof(int), 1, shpfile);
                        int partsind; // 'partsind' - index of the initial point of each part
                        partsind=0;
                        for(i=0; i<numparts; i++) {
                            fwrite(&partsind, sizeof(int), 1, shpfile);
                            partsind+=pHPt->m_Komplekt->GetAt(pHPt->F_TOCHKI+i)->GetSize();
                        }
                        MPoint point;
                        for(i=0; i<numpoints; i++) {
                            point.x = pHPt->GetPoint(i).x;
                            point.y = pHPt->GetPoint(i).y;
                            fwrite(&point.x, sizeof(double), 1, shpfile);
                            fwrite(&point.y, sizeof(double), 1, shpfile);
                        }
                    }

                    // filling of information records of the dbf-file
                    ch = ' ';
                    MString st = "";
                    fwrite(&ch, sizeof(char), 1, dbffile);
                    for(j=0; j<col;j++){
                        ch = ' ';
                        if(pHPt->m_dbarray.GetSize()<j+1)
                            pHPt->m_dbarray.SetAtGrow(j,&st);
                        str = *pHPt->m_dbarray[j];
                        n = str.GetLength();
                        for(i=0; i<dlina[j]-n; i++)
                            fwrite(&ch, sizeof(char), 1, dbffile);
                        for(i=0; i<n; i++) {
                            ch = str[i];
                            fwrite(&ch, sizeof(char), 1, dbffile);
                        }
                    }

                    num = num-1;
                    strinfo.Format("%d", num);
                    //			GetDoc()->SetPaneText(1,strinfo);

                }

            it++;
        }
    }
    else//if RELIEFlayer
    {
        it = ((MReliefLayer*)pLay)->m_izolines.begin();
        while (it != ((MReliefLayer*)pLay)->m_izolines.end())
        {
            if ((*it)->WhatTheObject()==ID_IZOLINE) {
                MIzoline* pHPt = (MIzoline*)(*it);

                // filling of heading of records of the main file
                nom++;
                WriteReverseInteger(shpfile, nom);
                // length of the record 'reclength' depends on number of parts and points in a hyper point, see the description of the shp-file
                if(type == 1) {
                    reclength = (sizeof(int) + sizeof(double)*2)/2;
                    WriteReverseInteger(shpfile, reclength);
                }
                else if(type == 8) {
                    reclength = (sizeof(int)*2 + sizeof(double)*4 + sizeof(double)*2*pHPt->GetPointsNumber())/2;
                    WriteReverseInteger(shpfile, reclength);
                }
                else {
                    reclength = (sizeof(int)*3 + sizeof(double)*4 + sizeof(int)*(pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI) + sizeof(double)*2*pHPt->GetPointsNumber())/2;
                    WriteReverseInteger(shpfile, reclength);
                }

                // filling of records of the index file
                WriteReverseInteger(shxfile, offset);
                WriteReverseInteger(shxfile, reclength);

                offset += 4 + reclength; //calculation of shift for the following record

                // filling of records of the main file
                fwrite(&type, sizeof(int), 1, shpfile);
                if(type == 1) {
                    MPoint point;
                    point.x = pHPt->GetPoint(0).x;
                    point.y = pHPt->GetPoint(0).y;
                    fwrite(&point.x, sizeof(double), 1, shpfile);
                    fwrite(&point.y, sizeof(double), 1, shpfile);
                }
                else if(type == 8) {
                    rect = (*it)->m_bounds;
                    fwrite(&rect.left, sizeof(double), 1, shpfile);
                    fwrite(&rect.bottom, sizeof(double), 1, shpfile);
                    fwrite(&rect.right, sizeof(double), 1, shpfile);
                    fwrite(&rect.top, sizeof(double), 1, shpfile);
                    numpoints = pHPt->GetPointsNumber();
                    fwrite(&numpoints, sizeof(int), 1, shpfile);
                    MPoint point;
                    for(i=0; i<numpoints; i++) {
                        point.x = pHPt->GetPoint(i).x;
                        point.y = pHPt->GetPoint(i).y;
                        fwrite(&point.x, sizeof(double), 1, shpfile);
                        fwrite(&point.y, sizeof(double), 1, shpfile);
                    }
                }
                else {
                    rect = (*it)->m_bounds;
                    fwrite(&rect.left, sizeof(double), 1, shpfile);
                    fwrite(&rect.bottom, sizeof(double), 1, shpfile);
                    fwrite(&rect.right, sizeof(double), 1, shpfile);
                    fwrite(&rect.top, sizeof(double), 1, shpfile);
                    numparts = pHPt->m_Komplekt->GetSize()-pHPt->F_TOCHKI;
                    fwrite(&numparts, sizeof(int), 1, shpfile);
                    numpoints = pHPt->GetPointsNumber();
                    fwrite(&numpoints, sizeof(int), 1, shpfile);
                    int partsind; // 'partsind' - index of the initial point of each part
                    partsind=0;
                    for(i=0; i<numparts; i++) {
                        fwrite(&partsind, sizeof(int), 1, shpfile);
                        partsind+=pHPt->m_Komplekt->GetAt(pHPt->F_TOCHKI+i)->GetSize();
                    }
                    MPoint point;
                    for(i=0; i<numpoints; i++) {
                        point.x = pHPt->GetPoint(i).x;
                        point.y = pHPt->GetPoint(i).y;
                        fwrite(&point.x, sizeof(double), 1, shpfile);
                        fwrite(&point.y, sizeof(double), 1, shpfile);
                    }
                }

                // filling of information records of the dbf-file
                ch = ' ';
                MString sr = "";
                fwrite(&ch, sizeof(char), 1, dbffile);
                for(j=0; j<col;j++){
                    ch = ' ';
                    if(pHPt->m_dbarray.GetSize()<j+1)
                        pHPt->m_dbarray.SetAtGrow(j,&sr);
                    str = *pHPt->m_dbarray[j];
                    n = str.GetLength();
                    for(i=0; i<dlina[j]-n; i++)
                        fwrite(&ch, sizeof(char), 1, dbffile);
                    for(i=0; i<n; i++) {
                        ch = str[i];
                        fwrite(&ch, sizeof(char), 1, dbffile);
                    }
                }

                num = num-1;
                strinfo.Format("%d", num);
                //			GetDoc()->SetPaneText(1,strinfo);

            }
            it++;
        }

    }
    fclose(shpfile);
    fclose(shxfile);
    fclose(dbffile);

    if ((type == 5)&&(pWorkObjlist))
    {
        pWorkObjlist->clear();
        delete pWorkObjlist;
    }

    return true;
}

//==========================================
//!Function of copying of the map
void MKarta::Risovanie(MPlotDevice* pDevice, int scale, bool bDrawSmall)
//==========================================
{
    drawActFlag = false;
    //we calculate the drawing filter size
    m_DrawFilterSizeGeo = pDevice->LogicToBum(m_DrawFilterSizeLog);
    //For filling of all area of drawing
    //It is necessary to use the return transformation

    //Filling of a background
    MRect visArea;
    MRect fillInBum;

    visArea = pDevice->GetLogRect();

    pDevice->LogicToBum(&visArea, fillInBum);
    pDevice->FillSolidRect(&fillInBum, GetPaperColor());
    pDevice->Actualize();

    //The geographical rectangle where is made a conclusion
    MRect drawBounds;
    pDevice->GetClientRect(&drawBounds);
    //	pDevice->Rectangle(&drawBounds);      // 2004.11.29  vipa

    //we will draw a leaf if it is the model
    if(m_ismaket)
    {
        //Filling of a background
        pDevice->FillSolidRect(&m_maketlist, MColor(255, 255, 255));
        pDevice->Actualize();
    }

    // REMARK: that inscriptions were always over objects, at first we draw
    //         all layers, and then inscriptions for each layer
    //DRAWING of OBJECTS
    //drawing of objects happens in a layer,

    zoomRectangle = drawBounds;
    DrawLayers(pDevice, drawBounds, scale, bDrawSmall);

    // DRAWING OF THE TEXT
    //drawing of the text at objects happens in a layer in which there are these objects
    DrawLayersText(pDevice, drawBounds);

    // we draw a result
    pDevice->Actualize();
    pDevice->SetVirtualMode(false);
};

//==========================================
//!Function of copying of an operational situation
void MKarta::RisovanieAct(MPlotDevice* pDevice, bool bDrawSmall)
//==========================================
{
    drawActFlag= true;
    //  xpma.clear();
    //  xpma_xy.clear();
    //  xpma_dxdy.clear();

    m_SelecedList.clear();

    m_DrawFilterSizeGeo = 0;

    //The geographical rectangle where is made a conclusion
    MRect drawBounds;
    pDevice->GetClientRect(&drawBounds);

    DrawActLayers(pDevice, drawBounds, bDrawSmall);
    DrawActLayersText(pDevice, drawBounds);

    DrawSelectedObj(pDevice, drawBounds);
    drawActFlag= false;
};



//============================================
//!Function creates a layer of the necessary type
MLayer* CreateLayerByType(MString IDLayer)
//============================================
{

    MLayer* pLay = NULL;
    if(IDLayer==ID_SIMPLE_LAYER)
    {
        pLay = new MLayer();
        pLay->m_type = IDLayer;
    }
    else if(IDLayer == ID_RELIEF_LAYER)
    {
        pLay = (MLayer*)new MReliefLayer();
        pLay->m_type = IDLayer;
    }
    else if(IDLayer == ID_ROAD_LAYER)
    {
        pLay = (MLayer*)new MRoadLayer();
        pLay->m_type = IDLayer;
    }
    else if(IDLayer == ID_GIDRO_LAYER)
    {
        pLay = (MLayer*)new MGidroLayer();
        pLay->m_type = IDLayer;
    }
    return pLay;
}


//=======================================
//!creates object on its identifier
MObject* CreateObjectByType(MString type)
//==========================================
{
    MObject* p_object = NULL;
    MString id_file=type;
    //if object - MTopobject
    if (id_file==ID_TOPOBJECT) {
        p_object = new MTopobject();
    }
    //if object - MRamka
    /*	else if (id_file==ID_RAMKA) {

        p_object = new MRamka();
    }*/
    //if object -
    /*	else if (id_file==ID_SCENERY) {
        p_object = new MScen();
    }*/
    //if object -
    else if (id_file==ID_NADPKRIV) {
        p_object = new MNadpKriv();
    }
    //if object - simple inscription
    else if (id_file==ID_NADGENERAL) {
        p_object = new MNadp();
    }
    //if object - MHeightPoint
    else if (id_file==ID_HEIGHTPOINT) {
        p_object = new MHeightPoint();
    }
    //if object - MIzoline
    else if (id_file==ID_IZOLINE) {
        p_object = new MIzoline();
    }

    //if object - MObryv
    else if (id_file==ID_OBRYV) {
        p_object = new MObryv();
    }
    //if object - MOtkos
    else if (id_file==ID_OTKOS) {
        p_object = new MOtkos();
    }
    //if object - MPodpStenka
    else if (id_file==ID_PODPSTENKA) {
        p_object = new MPodpStenka();
    }
    //if object - MGidro
    else if (id_file==ID_GIDRO) {
        p_object = new MGidro();
    }
    //if object - MYama
    else if (id_file==ID_YAMA) {
        p_object = new MYama();
    }
    //if object - raster
    else if (id_file == ID_SCENERY)
        p_object = new MRaster();

    //if any type
    else return NULL;//p_object=NULL;

    if ( p_object) 	return p_object;
    else            return NULL;

}


//=========================================
//!Function of creation of a sign as object
MZnak* CreateZnakByType(MString SType)
//=========================================
{
    MZnak* pZnak = NULL;
    MYaz* pYaz;

    if(SType == ID_TOPOBJECT){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_TOPOBJECT);
        pYaz = new MYaz(F_LIN);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete pYaz;
    }
    else if (SType == ID_HEIGHTPOINT){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_HEIGHTPOINT);
        pYaz = new MYaz(F_DOT);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete pYaz;
    }
    else if (SType == ID_IZOLINE){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_IZOLINE);
        //we store in the first language color and thickness of the isoline
        pYaz = new MYaz(F_LIN);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete pYaz;
        //we store length of the bergstrich of the isoline in the second language
        pYaz = new MYaz(F_LIN);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete pYaz;
    }
    else if (SType == ID_OBRYV){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_OBRYV);
        //we store in the first language color and thickness of the line of top and strokes of break
        pYaz = new MYaz(F_LIN);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete pYaz;
        //we store a symbol for the signature of depth of break in the second language, distance between
        //strokes, and also a handwriting code for the signature of depth of break in the array of formulas of language

        MPrArray* linia = new MPrArray();
        MProsloika* prosloika = new MSbStretch();
        linia->AddTail(prosloika);
        MLinYaz* pLinYaz = new MLinYaz(linia);
        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete prosloika;

        delete linia;
        delete pLinYaz;
        delete pYaz;
    }
    else if (SType == ID_OTKOS){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_OTKOS);
        //we store in the first language color and thickness of the line of top and strokes of a slope
        //in the first layer - the simple line and a symbol of the line of a bottom and distance
        //between symbols in the second layer - a symbol broach
        MPrArray* linia = new MPrArray();
        MProsloika* prosloika = new MLinia();
        linia->AddTail(prosloika);
        delete prosloika;
        prosloika = new MSbStretch();
        linia->AddTail(prosloika);
        MLinYaz* pLinYaz = new MLinYaz(linia);
        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete prosloika;
        delete linia;
        delete pLinYaz;
        delete pYaz;
        //we store distance between strokes in the second language, length of a smaller stroke,
        //and also a handwriting code for slope height in the array of formulas of language
        linia = new MPrArray();
        prosloika = new MSbStretch();
        linia->AddTail(prosloika);
        pLinYaz = new MLinYaz(linia);
        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);

        delete prosloika;
        delete linia;
        delete pLinYaz;
        delete pYaz;
    }
    else if(SType == ID_PODPSTENKA){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_PODPSTENKA);
        //in the first language we store color and thickness of the line of top in the first layer -
        //the simple line and color and thickness of the line of a bottom in the second layer - the simple line
        MPrArray* linia = new MPrArray();
        MProsloika* prosloika = new MLinia();
        linia->AddTail(prosloika);
        delete prosloika;
        prosloika = new MLinia();
        linia->AddTail(prosloika);
        MLinYaz* pLinYaz = new MLinYaz(linia);
        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete prosloika;
        delete linia;
        delete pLinYaz;
        delete pYaz;
        //we store a symbol for the signature of height of top of a retaining wall in the second language,
        //distance between teeth, tooth width in the first layer - a symbol broach,
        //a symbol for the signature of height of the basis of a retaining wall in the second layer -
        //to a symbol broach, and also a handwriting code for the signature of height of a retaining wall
        //in the array of formulas of language
        linia = new MPrArray();

        prosloika = new MSbStretch();
        linia->AddTail(prosloika);
        delete prosloika;
        prosloika = new MSbStretch();
        linia->AddTail(prosloika);
        pLinYaz = new MLinYaz(linia);

        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete prosloika;
        delete linia;

        delete pLinYaz;
        delete pYaz;
    }
    else if (SType == ID_GIDRO){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_GIDRO);
        pYaz = new MYaz(F_PLO);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete pYaz;
    }
    else if (SType == ID_YAMA){
        pZnak = new MZnak();
        pZnak->GetYazArray()->SetSize(0);
        pZnak->SetClassType(ID_YAMA);
        //we store in the first language color and thickness of the line of top and strokes of a slope
        //in the first layer - the simple line and a symbol of the line of a bottom and distance
        //between symbols in the second layer - a symbol broach
        MPrArray* linia = new MPrArray();
        MProsloika* prosloika = new MLinia();
        linia->AddTail(prosloika);
        delete prosloika;
        prosloika = new MSbStretch();
        linia->AddTail(prosloika);

        MLinYaz* pLinYaz = new MLinYaz(linia);
        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete prosloika;
        delete linia;
        delete pLinYaz;
        delete pYaz;
        //we store a symbol for the signature of depth of break in the second language, distance between
        //strokes. and also a style code for the signature of depth of break in the array of formulas of language
        linia = new MPrArray();
        prosloika = new MSbStretch();
        linia->AddTail(prosloika);
        pLinYaz = new MLinYaz(linia);
        pYaz = new MYaz(F_LIN, NULL, pLinYaz);
        pZnak->GetYazArray()->AddTail(pYaz);
        delete prosloika;
        delete linia;
        delete pLinYaz;
        delete pYaz;

    }

    return pZnak;
}


//=========================================================================
//!Creates the array on type
MArray* CreateArrayByType(MString strType)
//=========================================================================
{
    MArray* temp = CreateBaseArrayByType(strType);
    if(temp)
        return temp;

    /*	else if(!(strcmp(strType, ID_RTA)))
    {
        return new MRtArray();
    }*/
    else if(!(strcmp(strType, ID_ZNA)))
    {
        return new MZnArray();
    }
    else if(!(strcmp(strType, ID_RLA)))
    {
        return new MRlArray();
    }
    else if(!(strcmp(strType, ID_ZSA)))
    {
        return new MZsArray();
    }
    else if(!(strcmp(strType, ID_RZV)))
    {
        return new MRzArray();
    }
    else if(!(strcmp(strType, ID_ZBK)))
    {
        return new MZbArray();
    }
    else if(!(strcmp(strType, ID_SGT)))
    {
        return new MSgArray();
    }
    else if(!(strcmp(strType, ID_TRG)))
    {
        return new MTrArray();
    }
    else if(!(strcmp(strType, ID_PHL)))
    {
        return new MHlArray();
    }
    return NULL;
}

//!  ----------
double SizeToReal(MTopobject* pObj,float fVal)
{
    if (pObj->GetKarta())
    {
        if(pObj->GetKarta()->m_ismaket)
            return (double)fVal;
    }
    return pObj->GEOM(fVal);
};

//!===============================
void MKarta::Copy(MKarta* pKarta)
//===============================
{
    m_name = pKarta->m_name;       //name of the map
    m_ieroname = pKarta->m_ieroname;	//hierarchical name of the map
    m_sort = pKarta->m_sort;       //type of the map
    m_vedomstvo = pKarta->m_vedomstvo;	//department
    m_standart = pKarta->m_standart;	//standard type
    m_strana = pKarta->m_strana;     //country of the edition
    m_istochnik = pKarta->m_istochnik;	//source
    m_autor = pKarta->m_autor;      //author
    m_comment = pKarta->m_comment;	//any comment

    //map segmentation
    m_razgrafka = pKarta->m_razgrafka;  //map segmentation
    m_grif = pKarta->m_grif;		//signature stamp alphabetic
    m_grifzif = pKarta->m_grifzif;	//signature stamp digital
    m_griftop = pKarta->m_griftop;	//signature stamp of the next leaf from above
    m_grifleft = pKarta->m_grifleft;	//signature stamp of the next leaf at the left
    m_grifbottom = pKarta->m_grifbottom;	//signature stamp of the next leaf from below
    m_grifright = pKarta->m_grifright;	//signature stamp of the next leaf on the right
    m_nametop = pKarta->m_nametop;	//name of the next leaf from above
    m_nameleft = pKarta->m_nameleft;	//name of the next leaf at the left
    m_namebottom = pKarta->m_namebottom;	//name of the next leaf from below
    m_nameright = pKarta->m_nameright;	//name of the next leaf on the right

    //time
    m_survay = pKarta->m_survay;		//filming

    m_dopsurvay = pKarta->m_dopsurvay;	//additional filming (correction)
    m_sostgod = pKarta->m_sostgod;	//state on... year
    m_izdanie = pKarta->m_izdanie;	//year of the edition

    // coordinate system
    m_progectia = pKarta->m_progectia;		//projection
    m_coosyst = pKarta->m_coosyst;		// coordinate system
    m_ellipsoid = pKarta->m_ellipsoid;		//ellipsoid
    m_edinitsa = pKarta->m_edinitsa;		//map unit
    m_polusharieWE = pKarta->m_polusharieWE;	//hemisphere West east

    m_polusharieNS = pKarta->m_polusharieNS;	//hemisphere North southern
    m_geodezkomment = pKarta->m_geodezkomment;	//any comment
    m_magnsklon = pKarta->m_magnsklon;		// magnetic declination
    m_parallel1 = pKarta->m_parallel1;		//first main parallel
    m_parallel2 = pKarta->m_parallel2;		//second main parallel
    m_osmeridian = pKarta->m_osmeridian;		//axial meridian
    m_parallelgl = pKarta->m_parallelgl;		//parallel of the main point

    //system of heights
    m_hsyst = pKarta->m_hsyst;		//system of heights
    m_hedinitsa = pKarta->m_hedinitsa;	//unit of system of heights
    m_hmin = pKarta->m_hmin;		//minimum height
    m_hmax = pKarta->m_hmax;		//maximum height
    m_hsek = pKarta->m_hsek;		//contour interval
    m_hminizo = pKarta->m_hminizo;		//minimum isohypse
    m_hmaxizo = pKarta->m_hmaxizo;	//maximum isohypse

    //scale
    m_mastab = pKarta->m_mastab;

    //frame
    m_ramstyle = pKarta->m_ramstyle;
    m_contur.ClearAll();
    m_contur = *(MPtArray*)pKarta->m_contur.Clone();

    m_password = pKarta->m_password;	// password

    //the map
    MLayerList list;
    list.clear();
    LAYITER it = pKarta->m_laylist.begin();
    while(it != pKarta->m_laylist.end())
    {
        MLayer* layTek = *it;
        MLayer* newLay = new MLayer();
        newLay->Copy(layTek);
        newLay->m_pKarta = this;
        list.push_back(newLay);
        it++;
    }

    m_laylist.clear();
    m_laylist.insert(m_laylist.end(), list.begin(), list.end());

    m_geobounds = pKarta->m_geobounds;  //the describing georectangle in conventional unit.
    // attributes of paper
    m_papercolorkod = pKarta->m_papercolorkod; // color of a background - not kept parameter
    //variables for the model
    m_maketlist = pKarta->m_maketlist; //leaf of the model
    //variables for transformation of points to the geo-model
    m_geopt1 = pKarta->m_geopt1;
    m_geopt2 = pKarta->m_geopt2;
    m_geopt3 = pKarta->m_geopt3;
    m_maketpt1 = pKarta->m_maketpt1;

    m_maketpt2 = pKarta->m_maketpt2;
    m_maketpt3 = pKarta->m_maketpt3;
    m_ismaket = pKarta->m_ismaket;//the flag defining a type of data ('true' - the model, 'false' - geo)

    //Members of the classifier
    m_pKls = pKarta->m_pKls;//The pointer on the classifier
    m_klstermin.Copy(&pKarta->m_klstermin);     //term of the classifier

    //service classifier
    //(not kept, serves for import export operations)
    m_pServKls = pKarta->m_pServKls;		// The pointer on the classifier
    m_pDotZnak = pKarta->m_pDotZnak;		// The pointer on a pointer sign of the service classifier
    m_pLinZnak = pKarta->m_pLinZnak;		// The pointer on a linear sign of the service classifier
    m_pSqrZnak = pKarta->m_pSqrZnak;		// The pointer on a areal sign of the service classifier
    m_pHptZnak = pKarta->m_pHptZnak;		// The pointer on a mark of height of the service classifier
    m_pIzoZnak = pKarta->m_pIzoZnak;		// The pointer on a isoline of the service classifier
    m_pObrZnak = pKarta->m_pObrZnak;		// The pointer on a steep of the service classifier
    m_pOtkZnak = pKarta->m_pOtkZnak;		// The pointer on a slope of the service classifier
    m_pYamZnak = pKarta->m_pYamZnak;		// The pointer on a hole of the service classifier
    m_pPstZnak = pKarta->m_pPstZnak;		// The pointer on a supporting wall of the service classifier
    m_pGdrZnak = pKarta->m_pGdrZnak;		// The pointer on a hydrographical object of the service classifier

    // for color shift
    m_DH = pKarta->m_DH;
    m_DL = pKarta->m_DL;
    m_DS = pKarta->m_DS;

    //for drawing of a background of operational layers
    m_actBackEnable = pKarta->m_actBackEnable;
    m_actBackLight = pKarta->m_actBackLight;
    m_actBackTransp = pKarta->m_actBackTransp;

    //systems of coordinates
    //	m_pObjectCoo->Copy(pKarta->m_pObjectCoo);   // vipa 2004.10.22
    m_pObjectCoo = pKarta->m_pObjectCoo;

    // file of a matrix of heights
    m_matrixfile = pKarta->m_matrixfile;
    m_matrixExtraFile = pKarta->m_matrixExtraFile;// veronika 25.04.2016
    SetMatrixFile(m_matrixfile);

    //percent of performance of process for indication of the course of long utilities
    percent = pKarta->percent;

}

//!==============================================================
bool MKarta::WriteKarta(FILE* h_nuxfile, int version)
//==============================================================
{
    MString 	id_file = ID_UTP_KARTA;
    //	int			inull = 0;
    int			i;

    //writing of the identifier of the file
    fwrite((char *)id_file, 3, 1, h_nuxfile);

    //writing of the version
    fwrite(&version, 2, 1, h_nuxfile);


    //calculation of length of the record "passport of the map"
    //length of the passport of the map - 2 bytes
    //quantity of layers - 2 bytes
    //a coverage rectangle - 32 bytes
    i = 36;

    //Term of the classifier
    i += m_klstermin.GetBytes();

    //semantics
    i = i + m_name.GetLength() + 2;
    i = i + m_ieroname.GetLength() + 2;
    i = i + m_sort.GetLength() + 2;
    i = i + m_vedomstvo.GetLength() + 2;
    i = i + m_standart.GetLength() + 2;
    i = i + m_strana.GetLength() + 2;
    i = i + m_istochnik.GetLength() + 2;
    i = i + m_autor.GetLength() + 2;
    i = i + m_comment.GetLength() + 2;

    //map segmentation
    i = i + m_razgrafka.GetLength() + 2;
    i = i + m_grif.GetLength() + 2;

    i = i + m_grifzif.GetLength() + 2;
    i = i + m_griftop.GetLength() + 2;
    i = i + m_grifleft.GetLength() + 2;
    i = i + m_grifbottom.GetLength() + 2;
    i = i + m_grifright.GetLength() + 2;
    i = i + m_nametop.GetLength() + 2;
    i = i + m_nameleft.GetLength() + 2;
    i = i + m_namebottom.GetLength() + 2;
    i = i + m_nameright.GetLength() + 2;

    //time
    i = i + m_survay.GetLength() + 2;

    i = i + m_dopsurvay.GetLength() + 2;
    i = i + m_sostgod.GetLength() + 2;
    i = i + m_izdanie.GetLength() + 2;

    // coordinate system
    i = i + m_progectia.GetLength() + 2;
    i = i + m_coosyst.GetLength() + 2;
    i = i + m_ellipsoid.GetLength() + 2;
    i = i + m_edinitsa.GetLength() + 2;
    i = i + m_polusharieWE.GetLength() + 2;
    i = i + m_polusharieNS.GetLength() + 2;
    i = i + m_geodezkomment.GetLength() + 2;
    i = i + m_magnsklon.GetLength() + 2;
    i = i + m_parallel1.GetLength() + 2;
    i = i + m_parallel2.GetLength() + 2;
    i = i + m_osmeridian.GetLength() + 2;
    i = i + m_parallelgl.GetLength() + 2;

    //system of heights
    i = i + m_hsyst.GetLength() + 2;
    i = i + m_hedinitsa.GetLength() + 2;
    //minimum height  - 8 bytes
    //maximum height - 8 bytes
    //contour interval - 8 bytes
    //minimum isohypse - 8 bytes
    //maximum isohypse - 8 bytes
    i += 40;

    //Way to the file of a matrix of heights
    i = i + m_matrixfile.GetLength() + 2;

    //scale
    i = i + m_mastab.GetLength() + 2;

    //frame
    i = i + m_ramstyle.GetLength() + 2;
    //exact contour of a frame
    i += m_contur.GetBytes();

    // password
    i = i + m_password.GetLength() + 2;

    //coordinate system
    i = i + m_pObjectCoo->GetBytes();

    //model data
    //32 bytes - m_maketlist
    //16*6 = 96 bytes on six points
    //1 byte on a model flag
    i = i + 129;

    //record of length of the passport of the map
    fwrite(&i, 2, 1, h_nuxfile);

    //We keep in the map the term of the classifier (for verification at connection of classifiers)
    m_klstermin.WriteBin(h_nuxfile,version);

    //record of the describing rectangle
    WriteRect(h_nuxfile,  version, &m_geobounds);

    //Semantics
    //writing of the name of the map
    WriteString(h_nuxfile,  version, m_name);
    //hierarchical name of the map
    WriteString(h_nuxfile,  version, m_ieroname);
    //map type

    WriteString(h_nuxfile,  version, m_sort);
    //department
    WriteString(h_nuxfile,  version, m_vedomstvo);
    //standard type
    WriteString(h_nuxfile,  version, m_standart);
    //country of the edition
    WriteString(h_nuxfile,  version, m_strana);
    //source
    WriteString(h_nuxfile,  version, m_istochnik);
    //author
    WriteString(h_nuxfile,  version, m_autor);
    //any comment
    WriteString(h_nuxfile,  version, m_comment);

    //map segmentation
    WriteString(h_nuxfile,  version, m_razgrafka);

    //signature stamp alphabetic
    WriteString(h_nuxfile,  version, m_grif);
    //signature stamp digital
    WriteString(h_nuxfile,  version, m_grifzif);


    //signature stamp of the next leaf from above
    WriteString(h_nuxfile,  version, m_griftop);
    //signature stamp of the next leaf at the left
    WriteString(h_nuxfile,  version, m_grifleft);
    //signature stamp of the next leaf from below
    WriteString(h_nuxfile,  version, m_grifbottom);
    //signature stamp of the next leaf on the right
    WriteString(h_nuxfile,  version, m_grifright);
    //name of the next leaf from above
    WriteString(h_nuxfile,  version, m_nametop);
    //name of the next leaf at the left
    WriteString(h_nuxfile,  version, m_nameleft);
    //name of the next leaf from below
    WriteString(h_nuxfile,  version, m_namebottom);
    //name of the next leaf on the right
    WriteString(h_nuxfile,  version, m_nameright);

    //time
    //filming
    WriteString(h_nuxfile,  version, m_survay);
    //additional filming (correction)
    WriteString(h_nuxfile,  version, m_dopsurvay);
    //state on... year
    WriteString(h_nuxfile,  version, m_sostgod);
    //year of the edition
    WriteString(h_nuxfile,  version, m_izdanie);

    // coordinate system
    //projection
    WriteString(h_nuxfile,  version, m_progectia);
    // coordinate system
    WriteString(h_nuxfile,  version, m_coosyst);
    //ellipsoid
    WriteString(h_nuxfile,  version, m_ellipsoid);
    //map unit
    WriteString(h_nuxfile,  version, m_edinitsa);
    //hemisphere West east
    WriteString(h_nuxfile,  version, m_polusharieWE);
    //hemisphere North southern
    WriteString(h_nuxfile,  version, m_polusharieNS);
    //any comment
    WriteString(h_nuxfile,  version, m_geodezkomment);
    // magnetic declination
    WriteString(h_nuxfile,  version, m_magnsklon);

    //first main parallel
    WriteString(h_nuxfile,  version, m_parallel1);
    //second main parallel
    WriteString(h_nuxfile,  version, m_parallel2);
    //axial meridian
    WriteString(h_nuxfile,  version, m_osmeridian);

    //parallel of the main point
    WriteString(h_nuxfile,  version, m_parallelgl);

    //system of heights
    //system of heights
    WriteString(h_nuxfile,  version, m_hsyst);
    //unit of system of heights
    WriteString(h_nuxfile,  version, m_hedinitsa);
    //maximum height
    fwrite(&m_hmin, 8, 1, h_nuxfile);

    //minimum height
    fwrite(&m_hmax, 8, 1, h_nuxfile);
    //contour interval
    fwrite(&m_hsek, 8, 1, h_nuxfile);
    //minimum isohypse
    fwrite(&m_hminizo, 8, 1, h_nuxfile);
    //maximum isohypse
    fwrite(&m_hmaxizo, 8, 1, h_nuxfile);

    //record of a name of the file of a matrix of heights
    WriteString(h_nuxfile,  version, m_matrixfile);

    //scale
    WriteString(h_nuxfile,  version, m_mastab);

    //frame
    //style of a frame
    WriteString(h_nuxfile,  version, m_ramstyle);
    //exact contour of a frame
    m_contur.WriteBin(h_nuxfile,  version);

    // password
    WriteString(h_nuxfile,  version,  m_password);

    //systems of coordinates
    m_pObjectCoo->WriteBin(h_nuxfile,  version);

    //we write model data
    WriteRect(h_nuxfile,  version, &m_maketlist);
    WritePoint(h_nuxfile,  version, &m_geopt1);
    WritePoint(h_nuxfile,  version, &m_geopt2);
    WritePoint(h_nuxfile,  version, &m_geopt3);
    WritePoint(h_nuxfile,  version, &m_maketpt1);
    WritePoint(h_nuxfile,  version, &m_maketpt2);
    WritePoint(h_nuxfile,  version, &m_maketpt3);
    unsigned char maketflag = (m_ismaket)? 1:0;
    fwrite(&maketflag, sizeof(unsigned char), 1, h_nuxfile);

    i = m_laylist.size();

    //writing of quantity of layers
    fwrite(&i, 2, 1, h_nuxfile);


    //writing of layers

    LAYITER it = m_laylist.begin();
    while (it!=m_laylist.end()) { //consistently we keep each layer
        (*it)->WriteBin(h_nuxfile,  version);
        it++;
    }

    return true;
}

//!==============================================================
bool MKarta::ReadKarta(FILE* h_nuxfile, int version)
//==============================================================
{
    char		id_file[] = "   ";
    int			i,laykol;
    version = 0;
    //reading identifier of the file
    fread(&id_file, 3, 1, h_nuxfile);
    MString idfile = id_file;
    if (idfile!=ID_UTP_KARTA) {
        InternalMessage( "Unknown format !");
        return false;
    }

    //version
    fread(&version, 2, 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&version, sizeof(int) );

    //length of all file
    int p = ftell(h_nuxfile);

    fseek(h_nuxfile, 0, SEEK_END);
    int	filesize = ftell(h_nuxfile);
    fseek(h_nuxfile, p, SEEK_SET);

    //number of percent of the read file from length of all file
    int proc = 0;

    p = ftell(h_nuxfile);
    proc = (int)(100.0*(double)p/(double)filesize);
    SetProgress(0);
    ShowProgress();

    //	record length
    int LPassport=0;
    fread(&LPassport, 2, 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&LPassport, sizeof(int) );
    //We read the term of the classifier
    m_klstermin.ReadBin(h_nuxfile,version);
    //reading the describing rectangle
    ReadRect(h_nuxfile,  version, &m_geobounds);

    //semantics
    //reading name of the map
    m_name = ReadString(h_nuxfile,  version);
    //hierarchical name of the map
    m_ieroname = ReadString(h_nuxfile,  version);
    //map type
    m_sort = ReadString(h_nuxfile,  version);
    //department
    m_vedomstvo = ReadString(h_nuxfile,  version);
    //standard type

    m_standart = ReadString(h_nuxfile,  version);
    //country of the edition
    m_strana = ReadString(h_nuxfile,  version);
    //source
    m_istochnik = ReadString(h_nuxfile,  version);
    //author
    m_autor = ReadString(h_nuxfile,  version);
    //any comment
    m_comment = ReadString(h_nuxfile,  version);

    //map segmentation
    //map segmentation
    m_razgrafka = ReadString(h_nuxfile,  version);
    //signature stamp alphabetic
    m_grif = ReadString(h_nuxfile,  version);
    //signature stamp digital
    m_grifzif = ReadString(h_nuxfile,  version);
    //signature stamp of the next leaf from above
    m_griftop = ReadString(h_nuxfile,  version);
    //signature stamp of the next leaf at the left
    m_grifleft = ReadString(h_nuxfile,  version);
    //signature stamp of the next leaf from below
    m_grifbottom = ReadString(h_nuxfile,  version);
    //signature stamp of the next leaf on the right
    m_grifright = ReadString(h_nuxfile,  version);
    //name of the next leaf from above
    m_nametop = ReadString(h_nuxfile,  version);
    //name of the next leaf at the left
    m_nameleft = ReadString(h_nuxfile,  version);
    //name of the next leaf from below
    m_namebottom = ReadString(h_nuxfile,  version);
    //name of the next leaf on the right
    m_nameright = ReadString(h_nuxfile,  version);

    //time
    //filming
    m_survay = ReadString(h_nuxfile,  version);
    //additional filming (correction)
    m_dopsurvay = ReadString(h_nuxfile,  version);
    //state on... year
    m_sostgod = ReadString(h_nuxfile,  version);
    //year of the edition
    m_izdanie = ReadString(h_nuxfile,  version);

    // coordinate system
    //projection
    m_progectia = ReadString(h_nuxfile,  version);
    // coordinate system
    m_coosyst = ReadString(h_nuxfile,  version);
    //ellipsoid

    m_ellipsoid = ReadString(h_nuxfile,  version);
    //map unit
    m_edinitsa = ReadString(h_nuxfile,  version);
    //hemisphere West east
    m_polusharieWE = ReadString(h_nuxfile,  version);
    //hemisphere North southern
    m_polusharieNS = ReadString(h_nuxfile,  version);
    //any comment
    m_geodezkomment = ReadString(h_nuxfile,  version);
    // magnetic declination
    m_magnsklon = ReadString(h_nuxfile,  version);
    //first main parallel
    m_parallel1 = ReadString(h_nuxfile,  version);
    //second main parallel
    m_parallel2 = ReadString(h_nuxfile,  version);
    //axial meridian
    m_osmeridian = ReadString(h_nuxfile,  version);
    //parallel of the main point
    m_parallelgl = ReadString(h_nuxfile,  version);

    //system of heights
    //system of heights
    m_hsyst = ReadString(h_nuxfile,  version);
    //unit of system of heights
    m_hedinitsa = ReadString(h_nuxfile,  version);

    //maximum height
    fread(&m_hmin, sizeof(m_hmin), 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&m_hmin, sizeof(m_hmin) );
    //minimum height
    fread(&m_hmax, sizeof(m_hmax), 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&m_hmax, sizeof(m_hmax) );
    //contour interval
    fread(&m_hsek, sizeof(m_hsek), 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&m_hsek, sizeof(m_hsek) );
    //minimum isohypse
    fread(&m_hminizo, sizeof(m_hminizo), 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&m_hminizo, sizeof(m_hminizo) );
    //maximum isohypse
    fread(&m_hmaxizo, sizeof(m_hmaxizo), 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&m_hmaxizo, sizeof(m_hmaxizo) );

    //reading name of the file of a matrix of heights
    m_matrixfile = ReadString(h_nuxfile,  version);
    SetMatrixFile(m_matrixfile);

    //scale
    m_mastab = ReadString(h_nuxfile,  version);

    //frame
    //style of a frame
    m_ramstyle = ReadString(h_nuxfile,  version);
    //exact contour of a frame
    m_contur.ReadBin(h_nuxfile,  version);

    // password
    m_password = ReadString(h_nuxfile,  version);

    //systems of coordinates
    char idcoo[] = "   ";
    fread(&idcoo, 3, 1, h_nuxfile);
    delete m_pObjectCoo;
    m_pObjectCoo = CreateCooByType(idcoo);
    m_pObjectCoo->ReadBin(h_nuxfile,  version);

    //we read model data
    ReadRect(h_nuxfile,  version, &m_maketlist);
    ReadPoint(h_nuxfile,  version, &m_geopt1);
    ReadPoint(h_nuxfile,  version, &m_geopt2);
    ReadPoint(h_nuxfile,  version, &m_geopt3);
    ReadPoint(h_nuxfile,  version, &m_maketpt1);
    ReadPoint(h_nuxfile,  version, &m_maketpt2);
    ReadPoint(h_nuxfile,  version, &m_maketpt3);
    unsigned char maketflag;
    fread(&maketflag, sizeof(unsigned char), 1, h_nuxfile);
    m_ismaket = (maketflag==1)? true : false;

    //color of paper isn't read - it undertakes as background color of the classifier
    m_papercolorkod = m_pKls->m_klscolorkod;

    //we remove progress
    p = ftell(h_nuxfile);
    proc = (int)(100.0*(double)p/(double)filesize);
    SetProgress((int)proc);
    ShowProgress();

    //we move to the block of layers
    //SetFilePointer(h_nuxfile, 4+LPassport, NULL, FILE_BEGIN);


    //reading quantity of layers
    laykol = 0;
    fread(&laykol, 2, 1, h_nuxfile);
    if (reverseReadFlag) reverseValue((char*)&laykol, sizeof(laykol) );

    //------------reading of layers
    char id_lay[] = "   ";
    MLayer* pLay;
    for (i=laykol; i>0; i--) {

        fread(id_lay,3, 1, h_nuxfile);
        fseek(h_nuxfile, -3, SEEK_CUR);
        pLay = (*fpCreateLayerByType)(id_lay);

        if(pLay)
        {
            AddLayer(pLay);
            pLay->m_bounds = m_geobounds;
            pLay->ReadBin(h_nuxfile,version);
            pLay->m_UID = laykol-i;
            pLay->m_sheetname = m_name;
        }
        else
        {
            InternalMessage("Unknown layer !");
            return false;
        }
    }
    //LayerheadDelete();
    p = ftell(h_nuxfile);
    proc = (int)(100.0*(double)p/(double)filesize);
    SetProgress(proc);
    ShowProgress();
    return true;
}


//!==============================================================
bool MKarta::ReadKarta(MString& filename, int version)
//==============================================================
{
    QTime time;  time.start(); //we start the timer
    char		id_file[] = "   ";
    int			i,laykol;
    version = 0;

    SetProgress(QString(filename)+"  ???????? Mb");
    SetProgress(1);
    ShowProgress();

    MFile file;
    if(!file.ReadFile(filename)) {
        SetProgress(-1);
        ShowProgress();
        return false;
    }

    //reading identifier of the file
    file.GetData(&id_file, 3, 1);
    MString idfile = id_file;
    if (idfile!=ID_UTP_KARTA) {
        InternalMessage( "Unknown format !");
        SetProgress(-1);
        ShowProgress();
        return false;
    }

    //version
    file.GetData(&version, 2, 1);
    if (reverseReadFlag) reverseValue((char*)&version, sizeof(version) );

    int p = file.GetPosition();

    //length of all file
    int filesize = file.GetSize();

    //  char *filebuf = new char[filesize+2];
    //  file.GetData(filebuf, filesize, 1);
    //  delete filebuf;

    //---int msec= time.restart();
    //cout <<filename<<" (" <<(float)(filesize/1024/1024.0)<<" Mb), read in "<<(float)(msec/1000.0)<<" sec."<<endl;

    //number of percent of the read file from length of all file
    int proc = 0;
    //SetProgress(QString(filename)+" ("+QString::number((double)filesize/1024.0)+" Mb)");
    MString  mmm;
    mmm.Format(" ( %.2f Mb )", (double)filesize/1024.0/1024.0);
    SetProgress(QString(filename)+mmm.c_str());
    SetProgress(0);
    ShowProgress();

    //	record length
    int LPassport=0;
    file.GetData(&LPassport, 2, 1);
    if (reverseReadFlag) reverseValue((char*)&LPassport, sizeof(LPassport) );
    //We read the term of the classifier
    m_klstermin.ReadBin(&file,version);
    //reading the describing rectangle
    ReadRect(&file,  version, &m_geobounds);

    //semantics
    //reading name of the map
    m_name = ReadString(&file,  version);
    //hierarchical name of the map
    m_ieroname = ReadString(&file,  version);
    //map type
    m_sort = ReadString(&file,  version);
    //department
    m_vedomstvo = ReadString(&file,  version);
    //standard type

    m_standart = ReadString(&file,  version);
    //country of the edition
    m_strana = ReadString(&file,  version);
    //source
    m_istochnik = ReadString(&file,  version);
    //author
    m_autor = ReadString(&file,  version);
    //any comment
    m_comment = ReadString(&file,  version);

    //map segmentation
    m_razgrafka = ReadString(&file,  version);
    //signature stamp alphabetic
    m_grif = ReadString(&file,  version);
    //signature stamp digital
    m_grifzif = ReadString(&file,  version);
    //signature stamp of the next leaf from above
    m_griftop = ReadString(&file,  version);
    //signature stamp of the next leaf at the left
    m_grifleft = ReadString(&file,  version);
    //signature stamp of the next leaf from below
    m_grifbottom = ReadString(&file,  version);
    //signature stamp of the next leaf on the right
    m_grifright = ReadString(&file,  version);
    //name of the next leaf from above
    m_nametop = ReadString(&file,  version);
    //name of the next leaf at the left
    m_nameleft = ReadString(&file,  version);
    //name of the next leaf from below
    m_namebottom = ReadString(&file,  version);
    //name of the next leaf on the right
    m_nameright = ReadString(&file,  version);

    //time
    //filming
    m_survay = ReadString(&file,  version);
    //additional filming (correction)
    m_dopsurvay = ReadString(&file,  version);
    //state on... year
    m_sostgod = ReadString(&file,  version);
    //year of the edition
    m_izdanie = ReadString(&file,  version);

    // coordinate system
    //projection
    m_progectia = ReadString(&file,  version);
    // coordinate system
    m_coosyst = ReadString(&file,  version);
    //ellipsoid

    m_ellipsoid = ReadString(&file,  version);
    //map unit
    m_edinitsa = ReadString(&file,  version);
    //hemisphere West east
    m_polusharieWE = ReadString(&file,  version);
    //hemisphere North southern
    m_polusharieNS = ReadString(&file,  version);
    //any comment
    m_geodezkomment = ReadString(&file,  version);
    // magnetic declination
    m_magnsklon = ReadString(&file,  version);
    //first main parallel
    m_parallel1 = ReadString(&file,  version);
    //second main parallel
    m_parallel2 = ReadString(&file,  version);
    //axial meridian
    m_osmeridian = ReadString(&file,  version);
    //parallel of the main point
    m_parallelgl = ReadString(&file,  version);

    //system of heights
    //system of heights
    m_hsyst = ReadString(&file,  version);
    //unit of system of heights
    m_hedinitsa = ReadString(&file,  version);

    //maximum height
    file.GetData(&m_hmin, sizeof(m_hmin), 1);
    if (reverseReadFlag) reverseValue((char*)&m_hmin, sizeof(m_hmin) );
    //minimum height
    file.GetData(&m_hmax, sizeof(m_hmax), 1);
    if (reverseReadFlag) reverseValue((char*)&m_hmax, sizeof(m_hmax) );
    //contour interval
    file.GetData(&m_hsek, sizeof(m_hsek), 1);
    if (reverseReadFlag) reverseValue((char*)&m_hsek, sizeof(m_hsek) );
    //minimum isohypse
    file.GetData(&m_hminizo, sizeof(m_hminizo), 1);
    if (reverseReadFlag) reverseValue((char*)&m_hminizo, sizeof(m_hminizo) );
    //maximum isohypse
    file.GetData(&m_hmaxizo, sizeof(m_hmaxizo), 1);
    if (reverseReadFlag) reverseValue((char*)&m_hmaxizo, sizeof(m_hmaxizo) );

    //reading name of the file of a matrix of heights
    m_matrixfile = ReadString(&file,  version);
    SetMatrixFile(m_matrixfile);

    //scale

    m_mastab = ReadString(&file,  version);

    //frame
    //style of a frame
    m_ramstyle = ReadString(&file,  version);
    //exact contour of a frame
    m_contur.ReadBin(&file,  version);

    // password
    m_password = ReadString(&file,  version);

    //systems of coordinates
    char idcoo[] = "   ";
    file.GetData(&idcoo, 3, 1);
    delete m_pObjectCoo;
    m_pObjectCoo = CreateCooByType(idcoo);
    m_pObjectCoo->ReadBin(&file,  version);

    //we read model data
    ReadRect(&file,  version, &m_maketlist);
    ReadPoint(&file,  version, &m_geopt1);
    ReadPoint(&file,  version, &m_geopt2);
    ReadPoint(&file,  version, &m_geopt3);
    ReadPoint(&file,  version, &m_maketpt1);
    ReadPoint(&file,  version, &m_maketpt2);
    ReadPoint(&file,  version, &m_maketpt3);
    unsigned char maketflag;
    file.GetData(&maketflag, sizeof(unsigned char), 1);
    m_ismaket = (maketflag==1)? true : false;

    //color of paper isn't read - it undertakes as background color of the classifier
    m_papercolorkod = m_pKls->m_klscolorkod;

    //we remove progress
    p = file.GetPosition();
    proc = (int)(100.0*(double)p/(double)filesize);
    SetProgress((int)proc);
    ShowProgress();

    //we move to the block of layers
    //SetFilePointer(h_nuxfile, 4+LPassport, NULL, FILE_BEGIN);


    //reading quantity of layers
    laykol = 0;
    file.GetData(&laykol, 2, 1);
    if (reverseReadFlag) reverseValue((char*)&laykol, sizeof(laykol) );

    //------------reading layers
    char id_lay[] = "   ";
    MLayer* pLay;
    for (i=laykol; i>0; i--) {

        file.GetData(id_lay, 3, 1);
        file.SetPosition(-3, MAP_SEEK_CUR);
        pLay = (*fpCreateLayerByType)(id_lay);
        if(pLay)
        {
            AddLayer(pLay);
            pLay->m_bounds = m_geobounds;
            pLay->ReadBin(&file,version);
            pLay->m_UID = laykol-i;
            pLay->m_sheetname = filename;
        }
        else
        {
            InternalMessage("Unknown layer !");
            return false;
        }
    }
    //LayerheadDelete();
    p = file.GetPosition();
    proc = (int)(100.0*(double)p/(double)filesize);
    SetProgress(proc);
    ShowProgress();
    return true;
}

//!==================================
bool MKarta::AddSheet(MString& filename, int ) //(MString& filename, int version)
//==================================
{
    //  QTime time;  time.start();
    MKarta *tmp_karta = new MKarta();
    //  tmp_karta->statusText= statusText;

    //  if (! tmp_karta)  {
    //    tmp_karta = new MKarta();//    tmp_karta->m_pKls= m_pKls;
    //  }

    //  if (tmp_karta)  delete tmp_karta;
    //    tmp_karta = new MKarta();

    //	tmp_karta->m_klstermin.Copy(m_pKls->GetTermin());
    //    int n = tmp_karta->m_FilterArray.GetSize();
    //    if (n)  tmp_karta->m_FilterArray.erase(0, n-1);

    //    tmp_karta->m_FilterArray.ClearAll();

    for(int i = 0; i<m_FilterArray.GetSize(); i++) {
        MString str;
        str = *(m_FilterArray.GetAt(i));
        tmp_karta->m_FilterArray.AddTail(&str);
    }

    //  SetProgress(proc, QString(filename)+" ("+QString::number((double)filesize/1024.0)+" Mb)");

    //  tmp_karta-> StartProgress();
    //set map progress output main window
    tmp_karta->pMainMapWidget = pMainMapWidget;
    if(!tmp_karta->ReadBin(filename, NULL)){
        delete tmp_karta;
        return false;
    }

    //  int cnt= tmp_karta->GetObjectCount();
    m_ShNameArray.AddTail(&(tmp_karta->m_name));

    if(!AddKarta(*tmp_karta)) return false;

    //	tmp_karta->ClearAll();
    delete tmp_karta;

    //	ConnectKlassifikator(m_pKls);   // vipa 2004.11.17   see  MKarta::AddKarta(MKarta)

    //	int  msc;
    //	msc =  time.restart();
    //InternalMessage(filename+"   -"+QString::number(cnt)+"  objects, loaded & created in  "+QString::number((double)(msc/1000.0))+" sec.");
    return true;
}

//!==================================
bool MKarta::AddKarta(MKarta& karta)
//==================================
{
    //calculation of a new rectangle of coverage
    if(m_geobounds.left > karta.m_geobounds.left) m_geobounds.left = karta.m_geobounds.left;
    if(m_geobounds.right < karta.m_geobounds.right) m_geobounds.right = karta.m_geobounds.right;
    if(m_geobounds.top < karta.m_geobounds.top) m_geobounds.top = karta.m_geobounds.top;
    if(m_geobounds.bottom > karta.m_geobounds.bottom) m_geobounds.bottom = karta.m_geobounds.bottom;

    /*   // vipa 2004.11.17
    int count = m_laylist.size();
    //addition of layers
    LAYITER it = karta.m_laylist.begin();
    while (it != karta.m_laylist.end())
    {
//		MLayer* pLay = CreateLayerByType((*it)->m_type);    // vipa 2004.07.19 exclude "RLL"
        MLayer* pLay = CreateLayerByType("LAY");
        MLayer* lay = (*it);
        pLay->Copy(lay);
        pLay->m_pKarta = this;
        pLay->m_UID = ++count;
        m_laylist.push_back(pLay);
        it++;
    }
*/  

    // vipa 2004.11.17
    double id = 0;
    LAYITER it = karta.m_laylist.begin();
    while (it != karta.m_laylist.end())	{
        if ((*it)->m_UID > id) id= (*it)->m_UID;
        it++;
    }
    // vipa 2004.11.17  to prevent a leakage of memory and save a time
    it = karta.m_laylist.begin();
    while (it != karta.m_laylist.end())
    {
        (*it)->ConvertToCoo(karta.m_pObjectCoo, m_pObjectCoo);
        (*it)->m_pKarta = this;
        (*it)->m_UID = ++id;
        m_laylist.push_back((*it));
        if (m_pKls) (*it)->ConnectKlassifikator(m_pKls);    // vipa 2004.11.17
        it++;
    }
    karta.m_laylist.clear();
    return true;
}

//! ---------------------
bool MKarta::DeleteSheet(MString& name)
//==================================
{
    //we delete layers
    LAYITER it = m_laylist.begin();
    while (it != m_laylist.end())
    {
        MLayer* pLay = (*it);
        if ((*it)->m_sheetname == name)		{
            it++;
            RemoveLayer(pLay);
            //  		delete (*it);
            //  		it= m_laylist.erase(it);
        }
        else			it++;
    }
    //we delete the name of a leaf
    for(int i = 0; i<m_ShNameArray.GetSize(); i++)	{
        if (*(m_ShNameArray.GetAt(i)) == name)
            m_ShNameArray.Delete(i);
    }
    MinimizeGeobounds();
    return true;
}

double MKarta::GetScale(MPlotDevice* pDevice, MRect& drawBounds)
{
    MRect screen;
    pDevice->BumToLogic(&drawBounds,screen);
    int screenWidth = screen.Width();
    double ds = drawBounds.Width();/*m_ki->GetDistance(p1, p2) + m_ki->GetDistance(p2, p3)*/;
    //double M =   ds / screenWidth;

    return ds / screenWidth;
}



//==================================
//!drawing of geographical layers

void MKarta::DrawLayers(MPlotDevice* pDevice, MRect& drawBounds, int scale, bool bDrawSmall)
//==================================
{
    double currentScale = (double)scale * 1000;
    double s_max = 0.0;
    double s_min = 100000000.0;

    QString layerName;

    std::for_each(m_laylist.begin(), m_laylist.end(), [&] (MLayer* it)
    {
        layerName = QString::fromLocal8Bit(it->m_name.c_str());

        s_max = 0.0;
        s_min = 100000000.0;

        if(layerDrawFilter->layerMap.keys().contains(layerName))
        {
            if (layerDrawFilter->layerMap[layerName][0] != 0)
                s_min = layerDrawFilter->layerMap[layerName][0];

            s_max = layerDrawFilter->layerMap[layerName][1];
        }

        if (((s_min == 100000000) || (currentScale < s_min) &&
             (s_max == 0) || (currentScale > s_max)) &&
                ((!bDrawSmall) && (!it->m_bDrawSmall)))
        {
            if (!it->x_common)
            {
                it->Draw(pDevice,/*m_zoombounds*/drawBounds);
            }
            else
                it->Draw1(pDevice,/*m_zoombounds*/drawBounds);
        };
    });
}

//==================================
//!drawing of a background of operational layers
void MKarta::DrawActBackground(MPlotDevice* pDevice)
//==================================
{
    if (m_actBackEnable)
        pDevice->OpacityBackground(m_actBackLight, m_actBackTransp);

}

//==================================
//!drawing of operational layers
void MKarta::DrawActLayers(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall)
//==================================
{
    std::for_each( m_actlaylist.begin(),  m_actlaylist.end(), [&](MLayer* it)
    {
        if(!((bDrawSmall) && (!it->m_bDrawSmall)))
        {
            //  we cause either the usual, or simplified drawing
            if (!it->x_common)
            {
                it->Draw(pDevice,/*m_zoombounds*/drawBounds);
            }
            else
                it->Draw1(pDevice,/*m_zoombounds*/drawBounds);
        }
    });
}

//==================================
//!drawing of the text of geographical layers
void MKarta::DrawLayersText(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall)
//==================================
{
    std::for_each(m_laylist.begin(), m_laylist.end(),
                  [&](MLayer *it)
    {
        if(!((bDrawSmall) &&
             (!it->m_bDrawSmall)) &&
                it->x_textvisible)
        {
            it->DrawText(pDevice,drawBounds);
        }
    });
}

//==================================
//!drawing of the text of operational layers
void MKarta::DrawActLayersText(MPlotDevice* pDevice, MRect& drawBounds, bool bDrawSmall)
//==================================
{
    std::for_each(m_actlaylist.begin(), m_actlaylist.end(), [&](MLayer* it)
    {
        if(!(bDrawSmall && (!it->m_bDrawSmall)) && !it->x_common)
        {
            it->DrawText(pDevice,drawBounds);
        }
    });
}

//==================================
//!drawing of the allocated objects
void MKarta::DrawSelectedObj(MPlotDevice* pDevice, MRect& drawBounds)
//==================================
{
    std::for_each(m_SelecedList.begin(), m_SelecedList.end(), [&](MObject* pObj)
    {
        if (pObj->m_bounds.IntersectGeoRect(pObj->m_bounds,drawBounds) &&
                pObj->CheckSize(pDevice))
        {
            pObj->Draw(pDevice);
        }
    });
}

//==================================
//! ----------------
void MKarta::SetGeoBounds(MRect& rect)
//==================================
{
    m_geobounds = rect;
}

//==================================
//! ------------
MRect MKarta::GetGeoBounds()
//==================================
{
    return m_geobounds;
}

//==================================
//! establishes the file with the filter for selection of quad-codes
bool MKarta::SetLoadFilterFile(MString path)
//==================================
{
    if(path == "")
    {
        m_FilterArray.ClearAll();
        return true;
    }
    ifstream in;
    in.open(path);
    if(!in.is_open()) return false;

    MString str;
    char st[30];
    while(!in.eof())
    {
        in>>st;
        str = st;
        str.Replace(":"," ");
        m_FilterArray.AddTail(&str);
    }

    in.close();
    return true;
}

//==================================
//! looks for quad-code in the list of the filter
bool MKarta::FindKkodInLoadFilter(MString kkod)
//==================================
{
    if(m_FilterArray.GetSize() == 0) return true;
    if(m_FilterArray.Find(0, &kkod) == -1) return false;
    return true;
}

//==================================
//! GetRumb
double MKarta::GetRumb(MPoint pt1, MPoint pt2, double poluos1, double poluos2)
//==================================
{
    poluos1 = axis_X;
    poluos2 = axis_Y;
    //	double sjatie = (poluos1-poluos2)/poluos1;
    double sqrte1 = (poluos1*poluos1-poluos2*poluos2)/(poluos1*poluos1);
    double sqrte2 = (poluos1*poluos1-poluos2*poluos2)/(poluos2*poluos2);
    //	double p2shtrih = 1/(4848137*pow((double)10,(double)-12));


    double ppi = 4*atan((double)1);

    double BB1 = pt1.y*2*ppi/360;//(grad1+min1/60+sec1/3600)*2*ppi/360;
    double BB2 = pt2.y*2*ppi/360;//(grad2+min2/60+sec2/3600)*2*ppi/360;
    double LL1 = pt1.x*2*ppi/360;//(grad1+min1/60+sec1/3600)*2*ppi/360;
    double LL2 = pt2.x*2*ppi/360;//(grad2+min2/60+sec2/3600)*2*ppi/360;

    double tgB1=0.0, tgB2=0.0;
    if (BB1!=0 && BB2!=0){
        tgB1 = sin(BB1)/cos(BB1);
        tgB2 = sin(BB2)/cos(BB2);
    }
    double l = fabs(LL2-LL1);
    double Coslna2 = cos(l/2);
    double tgBsredn = (tgB1+tgB2)/(2*Coslna2);


    double cos2Bsr = 1/(1+pow(tgBsredn,2));
    double Vsredn = (4.0018 + 3*sqrte2*cos2Bsr)/(4.0018 + sqrte2*cos2Bsr);

    double V1 = (4.0018 + 3*sqrte2*pow(cos(BB1),2))/(4.0018 + sqrte2*pow(cos(BB1),2));
    double V2 = (4.0018 + 3*sqrte2*pow(cos(BB2),2))/(4.0018 + sqrte2*pow(cos(BB2),2));

    //	double V = (V1-4*Vsredn+V2)/6;

    double delta1 = Vsredn*l;

    double cosp = (sin(BB1)*sin(BB2) + cos(BB1)*cos(BB2)/sqrt(1-pow(sqrte1,2)) * cos(delta1))/V1/V2;
    double p = acos(cosp);
    double sinq = sin(delta1)*cos(BB1)*cos(BB2)/sin(p)/V1/V2;
    double cosdel2 = cos(l+0.003351*p*sinq);
    double sindel2 = sin(l+0.003351*p*sinq);
    //	double del2 = l+0.003351*p*sinq;

    double f1 = cos(BB1)*cos(BB2)*sindel2;
    double f2 = sqrt(1-pow(cos(BB1)*cos(BB2)*cosdel2 + sin(BB1)*sin(BB2),2));
    double cosBm = f1/f2;
    double Bm = acos(cosBm);
    //	double sinBm = sin(Bm);

    //	double k2 = sqrte1*pow(sinBm,2);
    //	double Wm = (1-0.75*k2)/(1-0.25*k2);
    double Vm = (4.0018+3*sqrte2*pow(cos(Bm),2))/(4.0018+sqrte2*pow(cos(Bm),2));

    //	double del3 = acos(tan(BB2)/tan(Bm))-acos(tan(BB1)/tan(Bm));


    double sinA12_ = cos(Bm)*V1/(Vm*cos(BB1));
    double A12_ = asin(sinA12_);           //  asin(cos(Bm)/Vm/(cos(B1)/V1));
    //	double sinA21_ = cos(Bm)*V2/(Vm*cos(BB2));
    //	double A21_ = asin(sinA21_);

    return A12_;
}	
//! ---------
void MKarta::SetTextSizeMax(int size) {
    textSizeMax = size;
}
//! --------
int  MKarta::GetTextSizeMax() {
    return textSizeMax;
}

//============================================
//!Initializing Progress
void MKarta::StartProgress(int cnt) 
{
    fileCount= cnt;
    fileIndex= 1;
    //  QTime *tm1= new QTime();
    //  startMsec= tm1->msecsTo(QTime::currentTime());
    //  delete tm1;
}
//============================================
//!Stopping progress
void MKarta::StopProgress() 
{
    percent= -1;
    fileIndex= 1;
    fileCount= 1;
}

//============================================
//! installation of an indicator of progress
void MKarta::SetProgress(QString filename) {
    if (!filename.isEmpty()) fileName= filename.section('/',-1);
}
//
void MKarta::SetProgress(int prc, int index) {
    if (index) fileIndex= index;
    percent= prc;
}


//============================================
//! call of display of an indicator of progress
void MKarta::ShowProgress()  
{
    if(pOutputProgress)
        //-------- call client's progressBar
        (*pOutputProgress)(fileName.toStdString());
    else {
        //-------- create built-in progressBar
        if ((percent<0)&&progressDLG)     progressDLG->hide();
        if ((percent>=0)&&(percent<=100)) {
            if (progressDLG==NULL)  {
                progressDLG = new QProgressDialog(QObject::tr("progress"),QObject::tr("Cancel"), 0, 100, pMainMapWidget);
                if (progressDLG) {
                    progressDLG->setMinimumDuration(0);
                    progressLAB = new QLabel(0);
                    if (progressLAB) progressDLG->setLabel(progressLAB);
                    if(pMainMapWidget)progressDLG->setWindowTitle(pMainMapWidget->windowTitle());
                }
            }
            if (progressDLG) {
                progressDLG->show();
                QString sss;
                if ((fileCount>1)&&(fileIndex>0)) sss= QString::number(fileIndex)+" ("+ QString::number(fileCount)+") ";
                if (progressLAB) progressLAB->setText(sss+" "+fileName);
                progressDLG->setValue(percent);
                QApplication::processEvents();
            }
        }
        //    if ((percent>100)&&progressDLG)  progressDLG->hide();
    }
}


//void MKarta::HideProgress() {
// statusText-> setText("");
//}
//==================================
//! installation of the minimum size of the visible text
void MKarta::SetTextSizeMin(int size) {
    if ((size>3) && (size<17))
        distinguishTextSize = size;
    else
        InternalMessage("MKarta::SetTextSizeMin(int)  - size "+QString::number(size)+" ignored! Should be 4..16");
}

//==================================
//! obtaining the minimum size of the visible text
int  MKarta::GetTextSizeMin(){return distinguishTextSize;};

//! to receive - to set a flag of a conclusion of debugging information on <stdout>
void MKarta::SetVerboseFlag(bool flag){verboseFlag = flag;};
bool MKarta::GetVerboseFlag(){  return verboseFlag;};

//=======================================================================
// search of the list of objects in the set layer (m_parLayerName)
//on a preset value (m_parValue)
//certain attribute (m_parAttrName)
MObjectList*   MKarta::FindOblistInLayerByAttrValue(MString m_parLayerName,MString m_parAttrName, MString m_parValue)
//=======================================================================
{
    MLayer* pWorkLayer = NULL;
    unsigned short int m_AttrKey;
    MObjectList*	pOblist = NULL;

    //we will find number of attribute according to the name
    if (!m_pKls->GetAttrDictNumValue(m_parAttrName, m_AttrKey))
        return NULL;

    //if the name of a layer in which to look for is set
    if (!m_parLayerName.IsEmpty())
    {
        pWorkLayer = FindLayer(m_parLayerName);

        if (!pWorkLayer) return NULL;

        return pWorkLayer->FindOblistByAttrValue(m_AttrKey,m_parValue);
    }
    else
        //otherwise we look for in all layers of the map in which the flag is set m_bSearchIn
    {
        LAYITER it = m_laylist.begin();
        pOblist = new MObjectList();
        pOblist->clear();
        // cycle
        MObjectList* pObjects = NULL;

        while (it != m_laylist.end())
        {
            if((*it)->m_bSearchIn)
            {
                // if found in a layer, we add to the general resultant list
                pObjects = (*it)->FindOblistByAttrValue(m_AttrKey,m_parValue);
                if (pObjects)
                {
                    OBITER o_it = pObjects->begin();
                    while(o_it != pObjects->end())
                    {
                        pOblist->push_back(*o_it);
                        o_it++;
                    }
                    pObjects->clear();
                }
            }
            it++;
        }

        if (!pOblist->size())
        {
            delete pOblist;
            pOblist = NULL;
        }

        return pOblist;

    }
}

//=======================================================================
//! search of quad-code of obkt of the set type (iObjType) in the set layer (m_LayerName)
//! the type of object is set by a constant (F_PLO, F_DOT, F_LIN [Yaz.h]), a layer - by name
//! returns the list of the found quad-code
QStringList MKarta::FindKvadrokodesInLayerByObjType(MString m_LayerName, int iObjType)
//=======================================================================
{
    MLayer* pWorkLayer = NULL;

    //resultant list of quad-code
    QStringList m_ResCodesList;
    QString sRes, sPrev;
    MObjectList* pTempObjList;

    m_ResCodesList.clear();

    //if the name of a layer in which to look for is set
    if (!m_LayerName.IsEmpty())
    {
        pWorkLayer = FindLayer(m_LayerName);
        OBITER ito;

        //to create the list of objects of the set type
        pTempObjList = pWorkLayer->CreateObjectListByObjType(iObjType);
        if (!pTempObjList) return m_ResCodesList;

        //according to the list of objects to choose quad-code (without repeating)
        ito = pTempObjList->begin();

        sPrev = "";
        while (ito != pTempObjList->end())
        {

            if ((*ito)->GetIerarhy().Find(ID_TOPOBJECT) != -1)
            {
                sRes = ((MTopobject*)(*ito))->m_kkod;

                if (sRes != sPrev)
                {
                    //if there's no sRes in 'm_ResCodeList' then add 'sRes' to 'm_ResCodeList'
                    if (m_ResCodesList.indexOf(sRes))
                        m_ResCodesList+= sRes;

                    sPrev = sRes;
                }

            }

            ito++;
        }

        delete pTempObjList;

        if (!pWorkLayer) return m_ResCodesList;


    }
    else
    {

    }

    return m_ResCodesList;
}

//=======================================================================
//appointment to the 'attrName' attribute of 'data' data of operational object of 'uid'
bool MKarta::SetAttrDataByUid(double uid, MString& attrName, MString& data)
//=======================================================================
{
    LAYITER it = m_actlaylist.begin();
    MObject* obj;
    MTopobject* tpo = NULL;
    while(it!=m_actlaylist.end())
    {
        obj = (*it)->FindObject(uid);
        if((obj)&&(obj->WhatTheObject()==ID_TOPOBJECT))
        {
            tpo = (MTopobject*)obj;
            continue;
        }
        it++;
    }
    if(!tpo) return false;

    return tpo->SetDataPoleByName(data, attrName);
}

//==============================================
// Converting of the map in new system of coordinates
//==============================================
void MKarta::ConvertKartaToCoo(MCoo* pNewCoo)
{
    //we change all document
    if(pNewCoo == NULL) return;
    MLayer* pLay;
    LAYITER it = m_laylist.begin();
    while(it!=m_laylist.end())
    {
        pLay = *it;
        pLay->ConvertToCoo(m_pObjectCoo, pNewCoo);
        it++;
    }
    it = m_actlaylist.begin();
    while(it!=m_actlaylist.end())
    {
        pLay = *it;
        pLay->ConvertToCoo(m_pObjectCoo, pNewCoo);
        it++;
    }
    //save previouse coordinate system
    if(m_pOldObjectCoo)delete m_pOldObjectCoo;
    this->m_pOldObjectCoo = CreateCooByType(m_pObjectCoo->WhatTheCoo());
    this->m_pOldObjectCoo->Copy(m_pObjectCoo);
    //=======================================
    delete m_pObjectCoo;
    this->m_pObjectCoo = CreateCooByType(pNewCoo->WhatTheCoo());
    this->m_pObjectCoo->Copy(pNewCoo);
    if ((m_pObjectCoo->WhatTheCoo()== COO_GEODESIC || pNewCoo->WhatTheCoo()==COO_GEODESIC) && (pNewCoo->WhatTheCoo()!=m_pObjectCoo->WhatTheCoo()))
        this->m_pKls->RotateAllZsSystemCoeffs();
    Minimizelayerbounds();
    MinimizeGeobounds();
}

//==============================================
//addition of a topobyekt to top object
/*we unite only the areal crossed contours,
or linear not crossed*/
//==============================================
bool MKarta::UniteTopobjects(MTopobject* tpo, MTopobject* tpoAdd, MPoint point1, MPoint point2, MPoint ptReg1, MPoint ptReg2)
{
    if((!tpo)||(!tpoAdd)) return false;

    int znakType1 = tpo->m_pZnak->GetYaz()->GetType();
    int znakType2 = tpoAdd->m_pZnak->GetYaz()->GetType();
    //if both is not a line-type or not an area-type
    if(!(((znakType1 == F_LIN)&&(znakType2 == F_LIN))||((znakType1 == F_PLO)&&(znakType2 == F_PLO))))
        return false;
    //new array
    MPtArray* pta = tpo->GetPointArray();
    MPtArray* ptaAdd = tpoAdd->GetPointArray();
    MPtArray* ptaNew = nullptr;
    //    MPtArray* ptaNew = new MPtArray();

    if((znakType1 == F_LIN)&&(znakType2 == F_LIN))//for a line-type
    {
        //if two conturs have no crossing then unite
        ptaNew = (MPtArray*)(pta->Clone());
        if(CompareTwoMPoint(&point1,pta->GetHead())) InverseContur(ptaNew);
        if(CompareTwoMPoint(&point2, ptaAdd->GetTail()))  InverseContur(ptaAdd);

        ptaNew->AppendTail(ptaAdd);
    }
    else
    {
        if((znakType1 == F_PLO)&&(znakType2 == F_PLO))//for an area-type
        {
            ptaNew->ClearAll();

            //clones for experiments
            MPtArray* ptaDop = new MPtArray();
            ptaDop = (MPtArray*)pta->Clone();
            MPtArray* ptaAddDop = new MPtArray();
            ptaAddDop = (MPtArray*)ptaAdd->Clone();

            if((LomlineInRegion(pta, ptaAdd))||(LomlineInRegion(ptaAdd, pta)))
            {
                if(LomlineInRegion(pta, ptaAdd))
                    ptaNew = (MPtArray*)ptaAddDop->Clone();
                else if(LomlineInRegion(ptaAdd, pta))
                    ptaNew = (MPtArray*)ptaDop->Clone();
            }
            else
            {
                //indexes of tops
                int iBegin1 = FindNomVershinaOfPointOnLomlineExt(ptaDop, point1);
                int iBegin2 = FindNomVershinaOfPointOnLomlineExt(ptaAddDop, point1);

                ptaDop->CircleMove(iBegin1);
                ptaAddDop->CircleMove(iBegin2);

                int iEnd1 = FindNomVershinaOfPointOnLomlineExt(ptaDop, point2);
                int iEnd2 = FindNomVershinaOfPointOnLomlineExt(ptaAddDop, point2);

                ptaNew = ptaDop->Cut(iEnd1);
                ptaNew->AddTail(ptaDop->GetHead());
                ptaDop->AddTail(ptaNew->GetHead());
                if(PointInRegion(ptReg1,ptaDop))//if vnutr
                    ptaNew = (MPtArray*)ptaDop->Clone();

                MPtArray* ptCut = new MPtArray();
                ptCut->ClearAll();
                ptCut = ptaAddDop->Cut(iEnd2);
                ptCut->AddTail(ptaAddDop->GetHead());
                ptaAddDop->AddTail(ptCut->GetHead());

                if(PointInRegion(ptReg2,ptaAddDop))//if vnutr
                    ptCut = (MPtArray*)ptaAddDop->Clone();

                //orientation
                if(!CompareTwoMPoint(ptaNew->GetTail(),ptCut->GetHead()))
                    InverseContur(ptCut);

                ptaNew->AppendTail(ptCut);
                DeletingRepeatedPoints(ptaNew);
                ptaNew->AddTail(ptaNew->GetAt(0));
            }
            delete ptaAddDop;
            delete ptaDop;
        }
        else
        {
            delete ptaNew;
            return false;
        }
    }
    //set new point array to tpo object
    tpo->SetPoints(*ptaNew);
    //delete object tpoAdd from layer
    MLayer* pLay = tpoAdd->m_pLay;
    OBITER it = pLay->m_oblist.begin();
    while(it != pLay->m_oblist.end())
    {
        MObject* pObj = (*it);
        if((pObj->WhatTheObject() == ID_TOPOBJECT)
                &&((MTopobject*)pObj == tpoAdd))
        {
            pLay->m_oblist.erase(it);
            break;
        }
        it++;
    }
    return true;
}


