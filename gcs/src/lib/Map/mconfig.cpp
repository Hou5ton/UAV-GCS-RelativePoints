static char *xpmOK[]={
    "20 11 2 1",
    "# c #000000",
    ". c #ffffff",
    "................#...",
    "...............##...",
    ".............###....",
    ".......##...###.....",
    "......###..###......",
    "......###.###.......",
    "......######........",
    "......#####.........",
    "......####..........",
    ".......##...........",
    "...................."};
//
const char * xpmUP[] =
{
    "16 27 3 1",
    "       c None",
    ".      c #000000000000",
    "X      c #0FFFFFFF0FFF",
    "                ",
    "                ",
    "       ..       ",
    "      .XX.      ",
    "     .XXXX.     ",
    "    .XXXXXX.    ",
    "   .XXXXXXXX.   ",
    "  .XXXXXXXXXX.  ",
    " .XXXXXXXXXXXX. ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     ......     ",
    "                "};

const char * xpmDOWN[] =
{
    "16 27 3 1",
    "       c None",
    ".      c #000000000000",
    "X      c #0FFFFFFF0FFF",
    "                ",
    "     ......     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    "     .XXXX.     ",
    " .XXXXXXXXXXXX. ",
    "  .XXXXXXXXXX.  ",
    "   .XXXXXXXX.   ",
    "    .XXXXXX.    ",
    "     .XXXX.     ",
    "      .XX.      ",
    "       ..       ",
    "                ",
    "                "};

const char * x_folder[] =
{
    "16 16 3 1",
    "       c None",
    ".      c #000000000000",
    "X      c #FFFFFFFF0000",
    "                ",
    "                ",
    "  ....          ",
    " .XXXX.         ",
    " .............. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .XXXXXXXXXXXX. ",
    " .............. ",
    "                "};
const char *utpDirPath = "./";
const char *onFlag          = "X";
const char *onPartFlag		= "P";
const char *XXappendix      = ".ini";   //! extension of the file of the project (by default)
const char *SPLITTER        = "-";      //!  divider NOT the SPACE, NOT FIGURE
//const char *sxfCharset      = "cp866";  //!   coding of the UTP file
const char *filterFileName  =  "utp-filter";//! the file - the filter of objects
//! Names of tags in the file of the project
const char *XXutp               = "utpFile";   //! main UTP-file
const char *XXutp_extra         = "utp";       //!
const char *XXkls               = "klsFile";   //!Classifikator files
const char *XXmtx               = "mtxFile";      //!
const char *XXmtxobj            = "mtxObjFile";   //!
const char *XXmtx3              = "mtx3File";   //!
const char *XXlayer             = "layerList";    //!
const char *XXvisible           = "visibleList";     //
const char *XXsignature         = "signatureList";   //
const char *XXtransparent       = "transparentList"; //
const char *XXnavigator         = "navigatorList";   //
const char *XXscheme            = "schemeList";      //
const char *XXcGround           = "groundColor";         //
const char *XXobjsize           = "objSize";         //
const char *XXtextmax           = "textMax";         //
const char *XXtextmin           = "textMin";         //
const char *XXobjLoad           = "objLoad";             //
const char *XXobjDraw           = "objDraw";             //
const char *XXhls               = "HLS";             //
const char *XXratio             = "ratio";             //
const char *XXzoom              = "zoom";
const char *XXactBackground     = "ActBackground";//
const char *XXmgrslevel         = "MGRSTileLavel";//

//! Names of fields in tags of the file of the project
const char *XXtext           = "text";               //
const char *XXname           = "name";  
const char *XXminScale       = "minScale";     //
const char *XXmaxScale       = "maxScale";     //
const char *XXcLine          = "lineColor";
const char *XXcLineTransparent          = "lineTransparent";   //
const char *XXcText          = "textColor";              //
const char *XXcFiller        = "fillerColor";            //
const char *XXcFillerTransparent = "fillerTransparent";
const char *XXwLine          = "lineWidth";              //
const char *XXwText          = "textWidth";              //
const char *XXflag           = "flag";      //!
const char *XXfilter         = "filter";      //!
const char *XXp1             = "p1";             //  hls
const char *XXp2             = "p2";             //  hls
const char *XXp3             = "p3";             //  hls
const char *XXactBack		 = "enable";
const char *XXp5			 = "p5";
const char *XXp6			 = "p6";

const char *XXleft           = "left";
const char *XXtop            = "top";
const char *XXright          = "right";
const char *XXbottom         = "bottom";

#include <QGroupBox>
#include <math.h>
#include <cstdio>
#include <QTextStream>
#include <QLineEdit>
#include <QtXml>
#include <QComboBox>
#include <QTableWidget>
#include "mconfig.h"
#include "Refglobal.h"
#include "BaseCl.h"
#include <iostream>

#if IS_EDIT_KLS
#include "ModelInterface.h"
#endif
#include <QHeaderView>


MConfig::MConfig(QWidget *par, MKartaInterface *kartaInt, bool screenFl )
{
    parent= par;
    IMap=   kartaInt;
    karta=  IMap->GetKarta();
    KLS=    IMap->GetKlassifikator();
    w1=  w2=  w3=  w5=  w7=  NULL;
    w10=  w30=  NULL;
    tab= NULL;
    tabDialog = NULL;
    mki= mki30= NULL;
    matrixFlag=  false;
    matrix2Flag=  false;
    screenFlag=  screenFl;
    codec =       QTextCodec::codecForName(mapCharset);
    // codecSXF =    QTextCodec::codecForName(sxfCharset);
    configFile= QCoreApplication::arguments().at(0);
    configFile += XXappendix;    // by default
    id1=   1;    id2= 1;
    id4 = 1;
    node1 = node2 = node3 = node4 = NULL;
    list= list2= list3=  NULL;
    lab52 = lab53 = lab54 = NULL;
    sl51 = sl52 = sl53 = sl54 = sl55 = sl56 = NULL;
    pbReset5 = NULL;
    codecSXF = NULL;
    simple = visible = signature = transparent = navigator = false;
    findIn = filter = false;
    wLine = wText = wLine0 = wText0 = 0;
    tabID = 0;
    objLoadList.clear();
    objDrawList.clear();
    layerNameList.clear();
    objSize=  3;
    textMin= distinguishTextSize;
    textMax= 200;
    p1= p2= p3= 0;
    p5=karta->m_actBackLight;
    p6=karta->m_actBackTransp;
    p4= 10;
    actBack= false;
    htmlFlag= false;
    QDir utpDir(utpDirPath);
    if (!utpDir.exists())
        utpDirPath = QDir::currentPath().toLocal8Bit().data();

    sliderScaleMinEnable = true;
    sliderScaleMaxEnable = true;
    scaleList.push_back(1000000);
    scaleList.push_back(500000);
    scaleList.push_back(100000);
    scaleList.push_back(50000);
    scaleList.push_back(20000);
    scaleList.push_back(10000);
    scaleList.push_back(5000);
    scaleList.push_back(2000);
    scaleList.push_back(1000);
    scaleList.push_back(500);
    scaleList.push_back(200);
    scaleList.push_back(100);
    scaleList.push_back(50);
    scaleList.push_back(20);
    scaleList.push_back(0);

    isEnabledByGB = true;
    workCooArray.push_back(COO_GEODESIC_INDEX);
    workCooArray.push_back(COO_MERCATOR_INDEX);
    workCooArray.push_back(COO_TRANSVERSEMERCATOR_INDEX);
    workDatumArray.push_back(ELLIPSOID_AR_WGS_84_INDEX);
    workDatumArray.push_back(ELLIPSOID_AR_PZ_90_INDEX);
    isNewMap = false;


}
//----------------------------------------------------------------------
//! Destructor
MConfig::~MConfig()
{
    delete tabDialog;
    delete mki;
}

//on Tab changed in MapConfig dialog
void MConfig::tabChanged(int index)
{
    //if (w10) w10-> hide();

    QWidget *pTab = tab->widget(index);
    if (pTab!= NULL)
    {
        if (pTab == w1) {

            refreshForm1();
        }
        if (pTab == w2) {

            refreshForm2();
        }
        if (pTab == w3) {

            refreshForm3();
            //pbOK3->     setEnabled(false);
        }
        if (pTab == w5) {
            refreshForm5();
            pbOK5->     setEnabled(false);
        }
        if (pTab == w6) {

        }
        if (pTab == w7) {
            refreshForm7();
        }
    }
    if (isNewMap)
    {
        tab-> setWindowTitle(tr("Map project")+" <...>");
        /*added*/tabDialog-> setWindowTitle(tr("Map project")+" <...>");
    }
    else
    {
        tab-> setWindowTitle(tr("Map project")+" < "+configFile+ " >");
        /*added*/tabDialog-> setWindowTitle(tr("Map project")+" < "+configFile+ " >");
    }
}

//----------------------------------------------------------------------
//!on Calling  MapConfig dialog
void  MConfig::setup(bool screenFl)  {
    screenFlag=  screenFl;
    if (! IMap)  return;
    karta= IMap-> GetKarta();
    if (! karta) return;
    if (! tabDialog)   createForm();

    switch (tab->currentIndex())
    {
    case 0: refreshForm1(); break;
    case 1: refreshForm2(); break;
    case 2: refreshForm3(); break;
    case 3: refreshForm5(); break;
    case 4: refreshForm7(); break;
    }
    if (screenFl)
    {
        tabDialog-> show();
        tabDialog-> raise();
    }
    if (isNewMap)
    {
        tab-> setWindowTitle(tr("Map project")+" <...>");
        /*added*/tabDialog-> setWindowTitle(tr("Map project")+" <...>");
    }
    else
    {
        tab-> setWindowTitle(tr("Map project")+" < "+configFile+ " >");
        /*added*/tabDialog-> setWindowTitle(tr("Map project")+" < "+configFile+ " >");
    }
}

//!on Calling  MapConfig dialog
void  MConfig::setupGeoLayers(bool screenFl){
    setup(screenFl);
}

//!MapConfig dialog: Check of needed vars existance
bool MConfig::exists() {
    if (!IMap)   return false;
    karta= IMap-> GetKarta();
    if (! karta) return false;

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    int kk=0;
    while (it != m_laylist->end()){
        it++;
        kk++;
    }
    return (bool)kk;
}

//-------------------------------------------------------------------------
//! To load and display the map on the basis of data of the file of the fileName project
//! including a matrix of heights (mtxFl=true) and a matrix of excess (mtx2fl=true)
bool MConfig::initMap(bool mtxFl,   const QString &fileName, bool mtx2Fl)
{
    if (!IMap)   return false;
    karta= IMap-> GetKarta();
    if (! karta) return false;

    if (IMap->m_pFoundOblist) delete IMap->m_pFoundOblist;
    IMap->m_pFoundOblist = NULL;

    if (exists())  {
        int kexit=QMessageBox::information(IMap->GetWidget(), tr("Loading map"),
                                           tr("Map already loaded.\nLoad new map?"),
                                           tr("OK"),
                                           tr("Cancel"),
                                           0, 1);
        if (kexit==0) {        // to be a new map
            loadFileList.clear();
            loadFlagList.clear();
            objLoadList.clear();
            objDrawList.clear();
            layerNameFilterList.clear();
            emit kartaToBeCleared();
            karta->ClearAll();
            drawMap();
        }
        if (kexit==1 || kexit==-1)
            return true;   // OLD MAP
    }

    //  --------- new map config ------------------------
    configFile= fileName;
    matrixFlag= mtxFl;
    matrix2Flag= mtx2Fl;
    if ( ! loadConfig(mtxFl, mtx2Fl) )  return newMap(mtxFl, mtx2Fl);


    int i=0;
    int k=0;
    while( (i<loadFileList.count()) && (i<loadFlagList.count()) )
    {
        if (loadFlagList[i].length()>0)  k++;
        i++;
    }

    int fileCount= k;
    QMap<QString,QVector<int> > layMap = karta->layerDrawFilter->layerMap;

    //setLoadFilterEnabled(true);
    // ---------- contour map --------------------------
    if (loadMap()==false) return false;
    refreshLayerList();

    //karta-> SetProgress(0, 1);
    // -------- to set off(on) filter or cancel map.
    //setLoadFilterEnabled(false);
    if (objLoadList.count()) {
        int kexit=QMessageBox::information(IMap->GetWidget(), tr("Loading map"),
                                           tr("to load all map objects \n   or \n selected only (using load filter)?"),
                                           tr("All"), tr("Selected"), tr("Cancel"),
                                           0, 2) ;

        if (kexit==1)  setLoadFilterEnabled(true); // to load with filter
        if (kexit==2)  {                           // NO MAP and EXIT
            loadFileList.clear();
            loadFlagList.clear();
            objLoadList.clear();
            objDrawList.clear();
            layerNameFilterList.clear();
            karta->ClearAll();
            drawMap();
            return false;
        }
    }//EoIF if(objLoadList.count())

    //--------- load extra-maps
    karta->layerDrawFilter->SetLayerMap(layMap);
    karta-> StartProgress(fileCount);
    i=0; k=0;
    while( (i<loadFileList.count()) && (i<loadFlagList.count()) ) {
        if (loadFlagList[i].length()>0)  {
            k++;
            karta-> SetProgress(0, k);
            loadMapExtra(loadFileList[i].toUtf8().data());
            refreshLayerList();
        }
        i++;
    }
    karta-> StopProgress();

    //--------   new map setup  ------------------------
    setClassificator();
    if (matrixFlag || matrix2Flag)
    {
        bool isMatrixLoaded = setMatrixFiles();
        if(pbShowRelief) pbShowRelief->setEnabled(isMatrixLoaded);
        if(drawReliefShadow) drawReliefShadow->setEnabled(isMatrixLoaded);
    }
    IMap-> SetDrawFilterSize(objSize);
    IMap-> SetTextSizeMax(textMax);
    IMap-> SetTextSizeMin(textMin);
    IMap-> SetBackgroundColor(cGround);
    karta->m_DH = p1;
    karta->m_DL = p2;
    karta->m_DS = p3;
    textRatio= (float)p4/10.0;
    karta->m_actBackEnable = actBack;
    karta->m_actBackLight = p5;
    karta->m_actBackTransp = p6;
    setupMap(configFile);              //  IMap->GetKarta()-> ClearAll();
    setDrawFilter();                   //  removeOrphans();
    IMap-> ZoomTo(zoom);
    drawMap();
    emit configFileSaved(fileName);
    return true;
}
//-----------------------------------------------
//
bool MConfig::resetConfig(  const char * fileName)  {

    if (!IMap)   return false;
    karta= IMap-> GetKarta();
    if (! karta) return false;

    matrixFlag = true;

    if ( ! exists()) return initMap(matrixFlag, fileName, matrix2Flag);
    QFile  *file = new QFile(fileName);  if ( ! file-> exists())  return false;

    getCurrentConfig();

    QString  configFile0= configFile;
    QString  utpFile0= utpFile;
    QString  klsFile0= klsFile;
    QString  mtxFile0= mtxFile;
    QString  mtx2File0= mtx2File;
    QString  mtx3File0= mtx3File;
    QString  layerLine0     = layerLine;
    QString  visibleLine0   = visibleLine;
    QString  signatureLine0 = signatureLine;
    QString  navigatorLine0   = navigatorLine;
    QString  schemeLine0      = schemeLine;
    QStringList   layerNameList0=       layerNameList;          //
    QStringList   layerNameFilterList0= layerNameFilterList;    //
    QStringList   loadFileList0 =loadFileList;
    QStringList   loadFlagList0 =loadFlagList;
    QStringList   objLoadList0  =objLoadList;
    QStringList   objDrawList0  =objDrawList;
    int  p10= p1;
    int  p20= p2;
    int  p30= p3;
    int  p40= p4;
    int  p50= p5;
    int  p60= p6;
    bool actBack0 = actBack;
    int  textMin0= textMin;
    int  textMax0= textMax;
    int  objSize0= objSize;
    MColor   cGround0= cGround;
    MRect zoom0=  zoom;

    configFile= fileName;
    if ( ! loadConfig(matrixFlag, matrix2Flag) )  return false;

    if ((utpFile0 != utpFile)
            ||  (klsFile0 != klsFile)) {
        configFile = configFile0;
        utpFile    = utpFile0;
        klsFile    = klsFile0;
        mtxFile    = mtxFile0;
        mtx2File   = mtx2File0;
        mtx3File   = mtx3File0;
        layerLine     = layerLine0;
        visibleLine   = visibleLine0;
        signatureLine = signatureLine0;
        navigatorLine = navigatorLine0;
        schemeLine    = schemeLine0;
        layerNameList =       layerNameList0;
        layerNameFilterList= layerNameFilterList0;
        loadFileList = loadFileList0;
        loadFlagList = loadFlagList0;
        objLoadList  = objLoadList0;
        objDrawList  = objDrawList0;
        p1 = p10;
        p2 = p20;
        p3 = p30;
        p4 = p40;
        p5 = p50;
        p6 = p60;
        actBack = actBack0;
        textMin = textMin0;
        textMax = textMax0;
        objSize = objSize0;
        cGround = cGround0;
        zoom =  zoom0;
        InternalMessage("MConfig::resetConfig(char*)  - project file  "+QString(fileName)+" is incompatible and ignored!");
        return false;
    }

    int i=0;
    while ( i <loadFileList0.count() )  {
        int k= loadFileList.indexOf(loadFileList0[i]);
        if  ( k<0)
            eraseMapExtra(loadFileList0[i].toUtf8().data());
        else
            if (loadFlagList[k].length() ==0)
                eraseMapExtra(loadFileList0[i].toUtf8().data());

        i++;
    }

    i=0;
    while ( i <loadFileList.count())  {
        if (loadFlagList[i].length() >0)  {
            int k= loadFlagList0.indexOf(loadFileList[i]);
            if ( (k<0) || (loadFlagList0[k].length() ==0) ) {
                loadMapExtra(loadFileList[i].toUtf8().data());
                refreshLayerList();
            }
        }
        i++;
    }

    if (matrixFlag || matrix2Flag)
    {
        bool isMatrixLoaded = setMatrixFiles();
        pbShowRelief->setEnabled(isMatrixLoaded);
        drawReliefShadow->setEnabled(isMatrixLoaded);
    }

    setupMap(configFile);

    IMap-> SetDrawFilterSize(objSize);
    IMap-> SetTextSizeMax(textMax);
    IMap-> SetTextSizeMin(textMin);
    IMap-> SetBackgroundColor(cGround);
    karta->m_DH = p1;
    karta->m_DL = p2;
    karta->m_DS = p3;
    textRatio= (float)p4/10.0;
    karta->m_actBackEnable = actBack;
    karta->m_actBackLight = p5;
    karta->m_actBackTransp = p6;

    setDrawFilter();  //  removeOrphans();
    IMap-> ZoomTo(zoom);
    drawMap();

    //  emit loadChanged();
    return true;
}
//----------------------------
//
bool MConfig::setLoadFilterEnabled(bool flag) {
    karta= IMap-> GetKarta();
    if (! karta) return false;
    if (flag==false) {
        IMap-> SetFilter("");
        return true;
    }

    QString buf="";
    objLoadList.sort();
    const unsigned int iObjLoadListCount = (unsigned int)objLoadList.count();
    for(unsigned int i=0; i</*objLoadList.count()*/iObjLoadListCount; i++)
        buf+=  objLoadList[i]+"\n";
    QFile *file = new QFile(filterFileName);
    file -> open(QIODevice::WriteOnly);
    buf.replace(QRegExp(" "),":");
    QByteArray ccc= codec->fromUnicode(buf);
    file -> write(ccc);
    file -> close();
    IMap-> SetFilter(filterFileName);
    return true;
}


//Installation of the filter of drawing for a layer
bool MConfig::setDrawFilter() {

    karta= IMap-> GetKarta();
    if (! karta) return false;
    setAnyEnabled(true);

    if ( layerNameFilterList.count() == 0)    return true;
    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())   {
        QString name= codec->toUnicode((*it)->m_name.c_str());
        if ( layerNameFilterList.indexOf(name) >= 0 )
        {
            MObjectList *m_oblist= &(*it)->m_oblist;
            OBITER io = m_oblist->begin();
            while (io != m_oblist->end())  {
#ifdef WIN32//_WINDOWS
                MTopobject  *to= (MTopobject *)  (*io);
#else
                MTopobject  *to= dynamic_cast<MTopobject *>  (*io);
#endif
                if (to) {
                    QByteArray kkod=  to->m_kkod.c_str();
                    QString  qkod=  codec->toUnicode(kkod);
                    // vipa        if (( lst.findIndex(qkod) < 0)
                    if (( objDrawList.indexOf(qkod) < 0)
                            && (qkod.left(4)!="SERV") )   to-> m_enabled = false;
                }
                io++;
            }
        }
        it++;
    }
    return true;
}

// ------------------------------------
// to include/switch off objects of all layers of the map
bool MConfig::setAnyEnabled(bool flag) {
    karta= IMap-> GetKarta();
    if (! karta) return false;
    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())   {
        MObjectList *m_oblist= &(*it)->m_oblist;
        OBITER io = m_oblist->begin();
        while (io != m_oblist->end())  {
            (*io)-> m_enabled = flag;
            io++;
        }
        it++;
    }
    return true;
}

//------------------------------------------------------
//!Creation of the new draft of the map (assumes loading of other map)
//! 'mtxFl' - a flag of need of inquiry of matrixes of heights
//! 'mtx2Fl' - a flag of need of inquiry of matrixes of excess
bool MConfig::newMap(bool mtxFl, bool mtx2Fl)
{
    if (!IMap)   return false;
    karta= IMap-> GetKarta();
    if (! karta) return false;

    if (IMap->m_pFoundOblist) delete IMap->m_pFoundOblist;
    IMap->m_pFoundOblist = NULL;

    //If already there is a loaded map
    if (exists())
    {
        /*int kexit=QMessageBox::information(IMap->GetWidget(), tr("Loading new map"),
                                           tr("Map already loaded.\nLoad new map?"),
                                           tr("OK"),
                                           tr("Cancel"),
                                           0, 1);*/        //if in dialogue chose "OK"
        int kexit=QMessageBox::information(IMap->GetWidget(),
                                           tr("Loading new map"),
                                           tr("Map already loaded.\nLoad new map?"),
                                           QMessageBox::Ok,
                                           QMessageBox::Cancel);
        if (kexit==QMessageBox::Ok)
        {
            loadFileList.clear();
            loadFlagList.clear();
            objLoadList.clear();
            objDrawList.clear();
            layerNameFilterList.clear();
            drawMap();
        }
        //If cancelled creation of the new project (loading of other map)
        else if (kexit==1 || kexit==-1 )  return false;
        else return false;//example, when close information window
    }

    matrixFlag= mtxFl;
    matrix2Flag= mtx2Fl;

    //If successfully I passed a choice of files of the map (UTP, KLS, BMH)
    if ( selectMapFiles(mtxFl, mtx2Fl))
    {
        if (! loadMap()) return false;
        if (! setClassificator()) return false;

        //that the map was loaded even in the absence of a matrix of heights,
        //instead of return false; to use bool bb = false;
        if (matrixFlag || matrix2Flag)
        {
            bool isMatrixLoaded = setMatrixFiles();
            if(pbShowRelief) pbShowRelief->setEnabled(isMatrixLoaded);
            if(drawReliefShadow) drawReliefShadow->setEnabled(isMatrixLoaded);
            if (!isMatrixLoaded)
            {
                return false; //bool bb = false;
            }
        }
        cGround= IMap-> GetBackgroundColor(false);
        IMap-> SetDrawFilterSize(objSize);
        IMap-> SetTextSizeMax(textSizeMax);
        //  =+=
        if (! drawMap()) return false;
        layerNameList.clear();
        layerNameFilterList.clear();
        refreshLayerList();
          isNewMap = true;
        return true;
    }
    else return false;
}

// ---------------------------------------------------
//! To load the map from the 'utpFile' file
//
bool MConfig::loadMap()
{
    ///QTime  tim= QTime::currentTime(); cout <<"loadMap:  " <<tim.toString("hh:mm:ss.zzz") ;
    QFile file(utpFile);
    if ( ! file. exists())  return false;

    QByteArray ccc= codec->fromUnicode(utpFile);
    MString msUtpFileName= ccc.data();

    emit kartaToBeCleared();
    if (IMap) IMap->LoadMap(utpFile.toUtf8().data());//utpFile.ascii());
    else return false;

    //-- Installation of the central meridian of the map (it is necessary for calculations) --
    //if Map coordinate system is GEODESIC
    if (IMap->GetKarta()->m_pObjectCoo->WhatTheCoo()== COO_GEODESIC)
    {

        //take centralMeridian from MKarta::m_osmeridian
        if (IMap->GetKarta()->m_osmeridian != "")
            centralMeridian = atoi(IMap->GetKarta()->m_osmeridian.c_str());
        else
            InternalMessage("Map Coo System is Geodesic. Central Meridian may be incorrect");

    } //if Map coordinate system is _not_ GEODESIC
    else
    {
#ifdef WIN32//_WINDOWS
        MExtendedGaussKrugerCoo  *coo = (MExtendedGaussKrugerCoo *)  (IMap->GetKarta()->m_pObjectCoo);
#else
        MExtendedGaussKrugerCoo  *coo = dynamic_cast<MExtendedGaussKrugerCoo *>  (IMap->GetKarta()->m_pObjectCoo);
#endif

        if (coo) centralMeridian = (int)coo->m_meridian;
        else  {  // is it world map?
            MRect rct= IMap->GetGeoBounds();
            if ((int)(rct.Width()+0.5) == 360) centralMeridian = 0;
        }
    }

    return true;
}
//--------------------------------------------------------
//! To appoint the 'klsFile' classifier for the map
//
bool MConfig::setClassificator()  {
    if (! IMap) return false;
    KLS=  IMap-> GetKarta()-> m_pKls;
    if (klsFile.length()==0) {
        InternalMessage("kls-file  not defined -> have a contour map!");
        MColor clr(250,250,250);
        IMap-> SetBackgroundColor(clr);
        return true;
    }
    QFile file(klsFile);
    if ( ! file. exists()) {
        InternalMessage(klsFile +"  not found -> have a contour map!");
        MColor clr(250,250,250);
        IMap-> SetBackgroundColor(clr);
        return true;
    }

    QByteArray asciiKLSFile = codec->fromUnicode(klsFile);
    MString msKLSFileName= asciiKLSFile.data();

    IMap->SetKlassifikator(klsFile.toUtf8().data());
    KLS=  IMap-> GetKarta()-> m_pKls;
    return true;
}
//--------------------------------------------------------
//! To appoint for the 'mtxFile' map
//
bool MConfig::setMatrixFiles() {
    bool rez= false;
    if (! IMap) return false;
    if (/*mtxFile.length()*/!mtxFile.isEmpty() && !mtxFile.isNull())
    {
        rez= IMap->SetMatrixFile(mtxFile.toUtf8().data());
    }
    if (/*mtx2File.length()*/!mtx2File.isEmpty() && !mtx2File.isNull())
    {
        IMap->SetMatrixObjectFile(mtx2File.toUtf8().data());
    }
    if (/*mtx3File.length()*/!mtx3File.isEmpty() && !mtx3File.isNull())
    {
        IMap->SetMatrixCorrectionFile(mtx3File.toUtf8().data());
    }
    return rez;
}
//--------------------------------------------------------
//!
//
bool MConfig::drawMap() {
    if (! IMap) return false; // if (parent)  IMap->SetWidget(parent);
    IMap->DrawMap(false);
    IMap->DrawMapSmall(false);
    //IMap->GetWidget()->update();
    emit mapPainted();
    return true;
}
//---------------------------------------------------
//! Dialogue of a choice of files of the map, including the classifier and a matrix of heights (mtx=true))
//
bool MConfig::selectMapFiles(bool mtx, bool mtx2)
{
    matrixFlag= mtx;
    matrix2Flag= mtx2;
    //utpFile = QFileDialog::getOpenFileName(parent, tr("Select utp-file"), QDir::homePath (),"*.utp *.UTP");
    utpFile = QFileDialog::getOpenFileName(parent, tr("Select utp-file"), QDir::homePath (),tr("Maps *.utp *UTP (*.utp *UTP)"));
    if (utpFile.isEmpty())  return  false;

    QString prePath = utpFile.section(QChar(0x2F),0,-2);
    
    //klsFile = QFileDialog::getOpenFileName(parent, tr("Select kls-file"), prePath+"//","*.kls *.KLS");
    klsFile = QFileDialog::getOpenFileName(parent, tr("Select kls-file"), prePath+"//",tr("Klassificator *.kls *.KLS (*.kls *.KLS)"));
    if (mtx)
    {
        //mtxFile  = QFileDialog::getOpenFileName(parent, tr("Select elevation matrix"), prePath,"*.bmh *.BMH");
        mtxFile  = QFileDialog::getOpenFileName(parent, tr("Select elevation matrix"), prePath, tr("Matrix elevation *.bmh *.BMH (*.bmh *.BMH)"));

        //TR:Only for Belarus
        //mtx3File = QFileDialog::getOpenFileName(0, "*.bmh3 *.BMH3", parent, 0, tr("Select correction matrix"));

        //that the map was loaded even in the absence of a matrix of heights,
        //to comment out the next line:
        if (mtxFile.isEmpty() || mtxFile.isNull())  return  false;
    }
    if (mtx2)
    {
        //mtx2File = QFileDialog::getOpenFileName(parent,tr("Select extra-elevation matrix"), prePath /*utpFile*/,"*.bmh2 *.BMH2");
        mtx2File = QFileDialog::getOpenFileName(parent,tr("Select extra-elevation matrix"), prePath /*utpFile*/ ,tr("Extra-elevation matrix *.bmh2 *.BMH2 (*.bmh2 *.BMH2)"));
        //that the map was loaded even in the absence of an expanded matrix of heights,
        //to comment out the next line:
        if (mtx2File.isEmpty() || mtx2File.isNull())  matrix2Flag = false;
    }
    return true;
}

//-------------------------
//
MLayer*  MConfig::findLayer(const int uid)  {
    karta= IMap-> GetKarta();
    if (karta == 0) return 0;

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())
    {
        if ((*it)-> m_UID == uid)   return (*it);
        it++;
    }
    return 0;
}
//-------------------
//
MLayer*  MConfig::findLayerByName(QString  strSearch, int indexFrom) {
    karta= IMap-> GetKarta();
    if (! karta) return 0;
    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    int i=0;
    while (it != m_laylist->end()) {
        if ( i==indexFrom ) break;
        it++;
        i++;
    }
    while (it != m_laylist->end()) {
        QString sss= codec->toUnicode((*it)->m_name.c_str());
        if ( sss == strSearch )   return (*it);
        it++;
    }
    return 0;
}
//-------------------
//
int  MConfig::getLayerObjectCount(QString  strSearch) {
    int total= 0;
    karta= IMap-> GetKarta();
    if (! karta) return 0;

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end()) {
        QString sss= codec->toUnicode((*it)->m_name.c_str());
        if ( sss == strSearch )   total += (*it)-> GetObjectCount();
        it++;
    }
    return total;
}
// ------------------------------
//
MLayer*  MConfig::findLayerAt(const int idx)  {
    karta= IMap-> GetKarta();
    if (! karta) return 0;

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    int i=0;
    while (it != m_laylist->end()) {
        if (i == idx)   return (*it);
        it++;
        i++;
    }
    return 0;
}
// ------------------------------
//
void  MConfig::fillListForm1()
{
    if (! IMap)  return;
    karta= IMap-> GetKarta();
    if (! karta) return;
    QPixmap okPix(xpmOK);
    const QIcon okIcon(okPix);

    refreshLayerList();

    setupMapExtra();

    //layerCount= 0;
    if (! list) return;
    list-> clear();
    //QString  strSearch;

    for (int i=0; i<layerNameList.count(); i++)
    {
        QString strSearch= layerNameList[i];
        MLayer *layer= findLayerByName(strSearch); //  layer=  findLayer(real[i]);
        if (layer == 0 )    continue;

        //QString sid= QString::number(layer->m_UID);
        QTreeWidgetItem *lv = new QTreeWidgetItem(list);

        lv-> setText(0, QString::number(i+100+1).right(2) ); //QCString ccc= layer->m_name.c_str() ;
        lv-> setText(1, strSearch );

        lv->setCheckState(2,Qt::Unchecked);
        if (layer->x_visible)
            lv->setCheckState(2,Qt::Checked);
        lv->setCheckState(3,Qt::Unchecked);
        if (layer->x_common)
            lv->setCheckState(3,Qt::Checked);
        lv->setCheckState(4,Qt::Unchecked);
        if (layerNameFilterList.indexOf(strSearch) >=0 )
            lv->setCheckState(4,Qt::Checked);
        lv->setCheckState(5,Qt::Unchecked);
        if (layer->m_bDrawSmall)
            lv->setCheckState(5,Qt::Checked);
        lv->setCheckState(6,Qt::Unchecked);
        if (layer->m_bSearchIn)
            lv->setCheckState(6,Qt::Checked);
        lv->setCheckState(7,Qt::Unchecked);
        if (layer->x_textvisible)
            lv->setCheckState(7,Qt::Checked);

        MPoint p1 = IMap->ConvertPoint(MPoint(layer->m_bounds.left,layer->m_bounds.top), false);
        MPoint p2 = IMap->ConvertPoint(MPoint(layer->m_bounds.right,layer->m_bounds.bottom), false);
        int b1d = (int)floor(p1.y);
        int b1m = (int)floor((p1.y-(double)b1d)*60.0);
        int b1s = (int)floor((p1.y-(double)b1d-(double)b1m/60.0)*3600.0);
        int l1d = (int)floor(p1.x);
        int l1m = (int)floor((p1.x-(double)l1d)*60.0);
        int l1s = (int)floor((p1.x-(double)l1d-(double)l1m/60.0)*3600.0);
        int b2d = (int)floor(p2.y);
        int b2m = (int)floor((p2.y-(double)b2d)*60.0);
        int b2s = (int)floor((p2.y-(double)b2d-(double)b2m/60.0)*3600.0);
        int l2d = (int)floor(p2.x);
        int l2m = (int)floor((p2.x-(double)l2d)*60.0);
        int l2s = (int)floor((p2.x-(double)l2d-(double)l2m/60.0)*3600.0);
        QString l1 = QString("%1%2%3'%4''").arg(QString::number(l1d),QChar(176),QString::number(l1m),QString::number(l1s));
        QString b1 = QString("%1%2%3'%4''").arg(QString::number(b1d),QChar(176),QString::number(b1m),QString::number(b1s));
        QString l2 = QString("%1%2%3'%4''").arg(QString::number(l2d),QChar(176),QString::number(l2m),QString::number(l2s));
        QString b2 = QString("%1%2%3'%4''").arg(QString::number(b2d),QChar(176),QString::number(b2m),QString::number(b2s));
        for (int i = 0; i < lv->columnCount(); i++)
            lv->setToolTip(i, QString("LT(%1:%2) RB(%3:%4)").arg(l1,b1,l2,b2));
        if ( getLayerObjectCount(strSearch) )
            lv-> setDisabled(false);
        else                                    lv-> setDisabled(true);
    }
    for (int n=0;n<list->columnCount();n++)
        list->resizeColumnToContents(n);
    //  ckObject-> setEnabled(false);
    if(IMap->GetWidgetSmall()==NULL)
        list->hideColumn(5);
    pbUp->    setEnabled(false);
    pbDown->  setEnabled(false);
}
// ------------------------------
//
//QStringList
void  MConfig::createLayerObjectList(QString strSearch) {

    objectLayerList.clear();

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())   {
        QString sss= codec->toUnicode((*it)->m_name.c_str());
        if ( sss == strSearch )
        {
            MObjectList *m_oblist= &(*it)->m_oblist;
            OBITER io = m_oblist->begin();
            while (io != m_oblist->end())
            {

                if (((MObject*)*io)->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
                {
#ifdef WIN32//_WINDOWS
                    MTopobject  *to= (MTopobject *)  (*io);
#else
                    MTopobject  *to= dynamic_cast<MTopobject *>  (*io);
#endif
                    if (to) {
                        QByteArray kkod=  to->m_kkod.c_str();
                        QString  qkod=  codec->toUnicode(kkod);
                        //if ( qkod.find("000") >0)
                        if ( objectLayerList.indexOf(qkod) <0 )
                            objectLayerList.append(qkod);
                    }
                } //if topobject class
                io++;
            }
        }

        it++;
    }
    //return  objectLayerList;
}
// ------------------------------
//
void  MConfig::fillListForm2()  {
    if (IMap == 0)  return;
    karta= IMap-> GetKarta();
    if (karta == 0) return;
    if (list2 == 0) return;
    list2-> clear();

    /*QTreeWidgetItem *lvRoot1 = new QTreeWidgetItem(list2);
  lvRoot1->setText(0, tr("Maps of scale 1:100 000"));
  lvRoot1->setExpanded(true);

  QTreeWidgetItem *lvRoot2 = new QTreeWidgetItem(list2);
  lvRoot2->setText(0, tr("Maps of scale 1:200 000"));
  lvRoot2->setExpanded(true);
  QTreeWidgetItem *lvRoot5 = new QTreeWidgetItem(list2);
  lvRoot5->setText(0, tr("Maps of scale 1:500 000"));
  lvRoot5->setExpanded(true);
  QTreeWidgetItem *lvRootUndef = new QTreeWidgetItem(list2);
  lvRootUndef->setText(0, tr("Maps of undef scale"));
  lvRootUndef->setExpanded(true);
  lvRootUndef->setHidden(true);*/
    QTreeWidgetItem *lvRootUtp = new QTreeWidgetItem(list2);
    lvRootUtp->setText(0, tr("Maps"));
    lvRootUtp->setExpanded(true);
    QTreeWidgetItem *lvRootShp = new QTreeWidgetItem(list2);
    lvRootShp->setText(0, tr("Shapes"));
    lvRootShp->setExpanded(true);
    lvRootShp->setHidden(true);
    for (int nCol=0;nCol<list2->columnCount();nCol++)
    {
        lvRootUtp->setBackground (nCol, Qt::gray);
        lvRootShp->setBackground (nCol, Qt::gray);
    }

    int i=0;
    while ((i < loadFileList.count())&& (i < loadFlagList.count()) )
    {
        QString filePath = loadFileList[i];
        QString fileName = filePath.section('/',-1);
        QTreeWidgetItem *lv;
        if (fileName.endsWith(".utp", Qt::CaseInsensitive))
        {
            lv = new QTreeWidgetItem(lvRootUtp);
        }
        else if (fileName.endsWith(".shp", Qt::CaseInsensitive))
        {
            lv = new QTreeWidgetItem(lvRootShp);
            lvRootShp->setHidden(false);
        }


        lv-> setText(3, QString::number(i+100+1).right(2) );
        lv->setText(0, fileName);
        lv->setFlags(Qt::ItemIsSelectable |Qt::ItemIsEnabled);
        if (loadFlagList[i]==onFlag){
            lv->setCheckState(1,Qt::Checked);
            lv->setCheckState(2,Qt::Unchecked);
        }
        if (loadFlagList[i]==onPartFlag){
            lv->setCheckState(2,Qt::Checked);
            lv->setCheckState(1,Qt::Unchecked);
        }
        if (loadFlagList[i].length()==0) {
            lv->setCheckState(1,Qt::Unchecked);
            lv->setCheckState(2,Qt::Unchecked);
        }

        i++;
    }
    if (objLoadList.count()==0)
        list2->hideColumn(2);
    else list2->showColumn(2);

    for (int i=0; i<list2->columnCount();i++)
        list2->resizeColumnToContents(i);
    list2->setEnabled(isEnabledByGB);
    pbInsert2-> setEnabled(isEnabledByGB);
    pbDelete2-> setEnabled(isEnabledByGB);
}

// ------------------------------
//
void  MConfig::createObjMapList() {
    objMapList.clear();
    cntMapList.clear();

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())   {
        MObjectList *m_oblist= &(*it)->m_oblist;
        OBITER io = m_oblist->begin();
        while (io != m_oblist->end())
        {
            if (((MObject*)*io)->GetIerarhy().Find(ID_TOPOBJECT)!=-1)
            {

#ifdef WIN32//_WINDOWS
                MTopobject  *to= (MTopobject *)  (*io);
#else
                MTopobject  *to= dynamic_cast<MTopobject *>  (*io);
#endif
                if (to) {
                    QByteArray kkod=  to->m_kkod.c_str();
                    QString  qkod=  codec->toUnicode(kkod);
                    int i=  objMapList.indexOf(qkod);
                    if ( i<0) {    // not found
                        objMapList.append(qkod);
                        cntMapList.push_back(1);
                    }
                    else cntMapList[i]++;    // QValueList<int>::iterator ic= cntMapList.at(i);    (*ic)++;
                }
            } //if topobject class
            io++;
        }
        it++;
    }
}

// ------------------------------
//
void  MConfig::fillListForm3()  
{
    QString HTML; //= "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
    HTML+="<HTML><HEAD> <TITLE>Topographic Map Symbols</TITLE><META HTTP-EQUIV=\"Content-type\" CONTENT=\"text/html; charset=UTF-8\"></HEAD>";
    HTML+="<BODY>";
    HTML+="<CENTER><p2>Topographic Map Symbols</p2></CENTER><hr>";
    QString HTML1, HTML2;
    QString imgName, imgName1, imgName2;

    //<META HTTP-EQUIV=\"Content-type\" CONTENT=\"text/html; charset=UTF-8\">
    QTreeWidgetItem *lv2 = NULL;
    createObjMapList();

    QString pss0, pss1, pss2;
    int id=0;
    if (! IMap )   return;
    karta= IMap-> GetKarta();
    if (! karta )  return;
    if (! KLS )    return;
    if (! list3 )  return;
    list3-> clear();

    int dense;
    dense = IMap->GetActPolygonDense();
    IMap->SetActPolygonDense(Qt::SolidPattern);

    QPixmap folderPix(x_folder);
    QIcon folderIcon(folderPix);
    QByteArray  ccc, pcc;
    QString     sss, pss;
    MTermin  *trm= KLS-> GetTermin();
    ccc=  trm->GetKod().c_str();
    pss0= codec->toUnicode(ccc);
    MZsArray* znsistemi = KLS-> GetZsArray();
    for(int i=0; i<znsistemi->GetSize(); i++)
    {
        MZnakSistema* zs = znsistemi->GetAt(i);
        MRlArray* razdeli = zs->GetRazdelArray();
        trm=   zs->  GetTermin();
        ccc=  trm-> GetKod().c_str();
        sss=  codec->toUnicode(ccc);
        if ( true/*(sss=="2000")	||	(sss=="5000") || (sss=="1000")||(sss == "2500")*/)
        {
            imgName1= sss;
            pss1= pss0+" "+ sss;
            QTreeWidgetItem *lv1 = new QTreeWidgetItem(list3);
            lv1->   setExpanded(true);
            for (int nCol=0;nCol<list3->columnCount();nCol++)
                lv1->setBackground (nCol, Qt::gray);

            lv1->   setText(0, tr("Symbols of scale")+" 1:"+sss+"00" );
            if (htmlFlag)  {
                HTML1 += "<br>Symbols of scale  1:" +sss+"00"+"";
                HTML2 += "<br><center>Symbols of scale  1:" +sss+"00"+"</center>";
            }
            for(int j=0; j<razdeli->GetSize(); j++)
            {
                MRazdel* razdel = razdeli->GetAt(j);
                MZnArray* znaki = razdel->GetZnakArray();
                trm=   razdel->  GetTermin();
                ccc=  trm-> GetKod().c_str();
                sss=  codec->toUnicode(ccc);
                imgName2 = sss;
                lv2 = new QTreeWidgetItem(lv1);
                lv2-> setText(TID_UZN, pss1+ " " +sss  );
                lv2-> setIcon(0, folderIcon );
                ccc=  trm->GetShortname().c_str();
                lv2-> setText(0,  codec->toUnicode(ccc)  );
                ccc=  trm->GetFullname().c_str();
                lv2-> setText(4,  codec->toUnicode(ccc)  );
                lv2-> setText(TID_MASTER, pss  );

                if (htmlFlag)  {
                    QString anchor=
                            HTML1+= "<li><a href=\"#"+imgName1+imgName2+"\" >"+codec->toUnicode(ccc)+"</a>";     // razdel
                    HTML2+= "<P2><center><a id=\""+imgName1+imgName2+"\" name=\""+imgName1+imgName2+"\" >"+codec->toUnicode(ccc)+"</a></P2></center>";     // razdel
                    HTML2+= "<TABLE BORDER=1>";
                }
                for(int k=0; k<znaki->GetSize(); k++)
                {
                    MZnak* znak = znaki->GetAt(k); //cout << "        знак="<<k <<endl;
                    QPixmap imgPXM;

                    pss=  lv2->  text(TID_UZN);
                    trm=  znak-> GetTermin();
                    ccc=  trm->  GetKod().c_str();
                    sss=  codec->toUnicode(ccc);
                    QTreeWidgetItem *lv3 = new QTreeWidgetItem(lv2);
                    lv3-> setText(TID_UZN, pss+ " " +sss  );
                    ccc=  trm->GetShortname().c_str();
                    lv3-> setText(0,  codec->toUnicode(ccc)  );
                    QString sname =codec->toUnicode(ccc);
                    ccc=  trm->GetFullname().c_str();
                    lv3-> setText(4,  codec->toUnicode(ccc)  );
                    lv3-> setText(TID_MASTER, pss  );
                    QString fname =codec->toUnicode(ccc);

                    QByteArray ccc= codec->fromUnicode(pss+ " " +sss);
                    MString kkod= ccc.data();

                    id++;  lv3-> setText(i_ID, QString::number(id));
                    QString fg=lv3->text(TID_UZN);
                    if (objLoadList.indexOf(fg)!= -1)
                        lv3->setCheckState(0,Qt::Checked);
                    else lv3->setCheckState(0,Qt::Unchecked);

                    QString sName=pss+" "+sss +"               ";
                    QString sTip="    ";
                    sName= sName.left(20)+codec->toUnicode(ccc);

                    MYzArray* pYazArray = znak->GetYazArray();
                    if(!pYazArray) continue;
                    MYaz* pYaz;
                    for(int s=0; s<pYazArray->GetSize(); s++)
                    {
                        pYaz = znak->GetYaz(s);
                        if(pYaz->GetType()==F_DOT)
                        {
                            lv3-> setText(2,  tr("Dot")   );
                            sTip="dot";
                            if (htmlFlag)
                            {
                                MSzArray* szar = ((MDotYaz*)pYaz->m_object)->m_tochka;
                                for(int kk=0; kk<szar->GetSize(); kk++)
                                {
                                    MSrez* pSrez = szar->GetAt(kk);
                                    if(pSrez->GetType()==IDT_SHRIFT)
                                    {
                                        MSymbol* sym = &((MShriftSrez*)pSrez)->m_tochka;
                                        QFont font(QString(sym->logfont.lfFaceName), 32);
                                        lab33-> setFont(font);
                                        QColor col(255, 255, 255);
                                        lab33->setPalette(QPalette(col));
                                        unsigned int color=sym->colorkod;
                                        MColor colorValue;
                                        KLS->GetDictColorValue(color,colorValue);
                                        QColor clr(colorValue.R, colorValue.G, colorValue.B);
                                        //lab33->setPaletteForegroundColor (clr);
                                        lab33-> setText(QString(sym->str) );
                                        imgPXM = QPixmap(0, 0);
                                        imgPXM = QPixmap(lab33->size());
                                        //bitBlt(&imgPXM, 0,0, lab33);
                                        imgPXM = *lab33->pixmap ();
                                    }
                                }//EoFOR for(int kk=0; kk<szar->GetSize(); kk++)
                            }//EoIF if (htmlFlag)
                        }//EoIF if(pYaz->GetType()==F_DOT)

                        //Linear sign
                        if(pYaz->GetType()==F_LIN)
                        {
                            lv3-> setText(2,  tr("Line")  );
                            sTip="line";

                            if (htmlFlag)
                            {
                                if (mki->FindObjectAtLayers(1))
                                {
                                    mki-> SetWorkingActLayer(1);
                                    mki-> DeleteObject(1);
                                }
                                else
                                {
                                    mki-> AddActLayer(1);
                                    mki-> SetWorkingActLayer(1);
                                }
                                mki-> SetWorkingActLayer(1);
                                mki-> AddObject(1);
                                mki-> SetWorkingObject(1);

                                MRect  rct;
                                rct = mki->GetGeoBounds();
                                rct.NormalizeGeoRect();
                                mki-> ZoomTo(rct);
                                double hr= (rct.right -rct.left)/12.0;
                                double vr=  (rct.top -rct.bottom)/12.0;
                                MPoint ptg1(rct.left+ hr , rct.bottom + vr);
                                mki-> AddPoint(ptg1);
                                MPoint ptg2(rct.right- hr , rct.top- vr );
                                mki-> AddPoint(ptg2);
                                mki-> SetZnak(kkod);
                                mki-> SetWidget(lab33);
                                mki-> DrawMap(false);
                                imgPXM =  *(mki->GetPixmapAct());
                            }
                        }

                        if(pYaz->GetType()==F_PLO)
                        {
                            lv3-> setText(2,  tr("Area")  );
                            sTip="area";
                            if (htmlFlag)
                            {
                                if (mki->FindObjectAtLayers(1))
                                {
                                    mki-> SetWorkingActLayer(1);
                                    mki->DeleteObject(1);
                                }
                                else
                                {
                                    mki-> AddActLayer(1);
                                    mki-> SetWorkingActLayer(1);
                                }
                                mki-> SetWorkingActLayer(1);
                                mki-> AddObject(1);
                                mki-> SetWorkingObject(1);
                                MRect  rct = mki->GetGeoBounds();
                                rct.NormalizeGeoRect();
                                mki-> ZoomTo(rct);
                                double hr= (rct.right -rct.left)/12.0;
                                double vr=  (rct.top -rct.bottom)/12.0;
                                MPoint ptg1(rct.left + hr, rct.bottom + vr);
                                mki-> AddPoint(ptg1);
                                MPoint ptg2(rct.left + hr, rct.top - vr);
                                mki-> AddPoint(ptg2);
                                MPoint ptg3(rct.right - hr, rct.top - vr);
                                mki-> AddPoint(ptg3);
                                MPoint ptg4(rct.right - hr, rct.bottom + vr);
                                mki-> AddPoint(ptg4);
                                mki-> SetZnak(kkod);
                                mki-> SetWidget(lab33);
                                mki-> DrawMap(false);
                                imgPXM =  *(mki->GetPixmapAct());
                            }
                        }
                        if(pYaz->GetType()==IDL_PLS) { lv3-> setText(2,  "strip" );  cout <<"STRIP"<<endl; }

                    }

                    //int ii= objMapList.indexOf(pss+ " " +sss);


                    if (htmlFlag)  {
                        QImage img = imgPXM.toImage() ;
                        imgName    = "/tmp/png/"+imgName1+imgName2+QString::number(k+1) +".png";
                        img.save(imgName,"PNG");
                        HTML2 += "<tr><td>"+QString::number(k+1)+"</td><td><img src=\""+imgName+"\"></td><td>"+sname+"</td><td>"+sTip+"</td><td>"+pss+" "+sss+"</td><td>"+fname+"</td></tr>";
                    }
                }//end for(int k=0; k<znaki->GetSize(); k++)

                HTML2+= "</TABLE>";

                if (lv2) {
                    int total, cnt;
                    cnt= 0;
                    total = lv2->childCount();
                    for (int i=0;i<total;i++)
                        if (lv2->child(i)->checkState(0)==Qt::Checked) cnt++;

                    lv2->setText(1, QString::number(cnt)+ QString('/')+ QString::number(total));

                    if (cnt<total && cnt!=0) lv2->setCheckState(0, Qt::PartiallyChecked);
                    if (cnt ==0)lv2->setCheckState(0, Qt::Unchecked);
                    if (cnt == total) lv2->setCheckState(0, Qt::Checked);
                }

            }//end for(int j=0; j<razdeli->GetSize(); j++)
        } // razdelList
    }//end for(int i=0; i<znsistemi->GetSize(); i++)
    //if (mki)  mki->DrawMap(true);

    list3->resizeColumnToContents(0);
    //list3->hideColumn(TID_UZN);
    if (htmlFlag)  {
        HTML +=HTML1;
        HTML +=HTML2;
        HTML +="</BODY></HTML>";
        QString hmlf= klsFile;
        hmlf= hmlf.left(hmlf.indexOf('.')) + ".hml";
        QFile *file = new QFile(hmlf);
        file -> open(QIODevice::WriteOnly); //FILE *fl; fl=fopen(hmlf.ascii(), "w");
        file -> write( HTML.toUtf8().data(), strlen(HTML.toUtf8().data()) );
        file -> close();
    }
    IMap->SetActPolygonDense(dense);
}

// ------------------------------
// dialogue filling "the filter for a layer" (Select)
void  MConfig::fillListForm10()
{
    createObjMapList();
    int id=0;
    if (! IMap ) return;
    karta= IMap-> GetKarta();
    if (! karta) return;
    if (! KLS)   return;
    if (! list10) return;
    list10-> clear();
    QPixmap  folderPix(x_folder);
    QIcon folderIcon(folderPix);

    QByteArray  ccc, pcc;
    QString     sss, pss;
    MTermin  *trm= KLS-> GetTermin();

    ccc=  trm->GetShortname().c_str();
    ccc=  trm->GetFullname().c_str();
    ccc=  trm->GetKod().c_str();
    QString pssT=  codec->toUnicode(ccc);

    MZsArray* znsistemi = KLS-> GetZsArray();
    for(int i=0; i<znsistemi->GetSize(); i++)
    {
        MZnakSistema* zs = znsistemi->GetAt(i);
        MRlArray* razdeli = zs->GetRazdelArray();
        trm=   zs->  GetTermin();
        ccc=  trm-> GetKod().c_str();
        sss=  codec->toUnicode(ccc);
        if (true/*sss=="2000" || sss=="5000" || sss=="1000" || sss == "2500"*/)
        {
            QTreeWidgetItem *lv1 = new QTreeWidgetItem(list10);
            lv1-> setText(TID_UZN, pssT+ " " +sss  );
            lv1->setExpanded(true);
            ccc=  trm->GetShortname().c_str();
            lv1-> setText(0,  codec->toUnicode(ccc)  );
            ccc=  trm->GetFullname().c_str();
            lv1-> setText(5,  codec->toUnicode(ccc)  );
            lv1-> setText(TID_MASTER, pss  );

            for(int j=0; j<razdeli->GetSize(); j++)
            {
                MRazdel* razdel = razdeli->GetAt(j);
                MZnArray* znaki = razdel->GetZnakArray();
                pss=  lv1-> text(TID_UZN);
                trm=   razdel->  GetTermin();
                ccc=  trm-> GetKod().c_str();
                sss=  codec->toUnicode(ccc);

                QTreeWidgetItem *lv2 = new QTreeWidgetItem(lv1);
                lv2-> setText(TID_UZN, pss+ " " +sss  );
                ccc=  trm->GetShortname().c_str();
                lv1-> setIcon(0, folderIcon );
                lv2-> setText(0,  codec->toUnicode(ccc)  );
                ccc=  trm->GetFullname().c_str();
                lv2-> setText(4,  codec->toUnicode(ccc)  );
                lv2-> setText(TID_MASTER, pss  );

                for(int k=0; k<znaki->GetSize(); k++)
                {
                    MZnak* znak = znaki->GetAt(k); //cout << "        знак="<<k <<endl;

                    pss=  lv2-> text(TID_UZN);
                    trm=   znak->  GetTermin();
                    ccc=  trm-> GetKod().c_str();
                    sss=  codec->toUnicode(ccc);
                    int ii= objMapList.indexOf(pss+ " " +sss);
                    int jj= objectLayerList.indexOf(pss+ " " +sss);
                    if ( (ii>=0)&&(jj>=0) )
                    {
                        QTreeWidgetItem *lv3 = new QTreeWidgetItem(lv1); // +++++++
                        lv3-> setText(TID_UZN, pss+ " " +sss  );
                        ccc=  trm->GetShortname().c_str();
                        lv2-> setIcon(0, folderIcon );
                        lv3-> setText(0,  codec->toUnicode(ccc)  );
                        ccc=  trm->GetFullname().c_str();
                        lv3-> setText(5,  codec->toUnicode(ccc)  );
                        lv3-> setText(TID_MASTER, pss  );

                        id++;
                        lv3-> setText(i_ID, QString::number(id));
                        if (objDrawList.indexOf(lv3->text(TID_UZN))!=-1)
                        {
                            lv3->setCheckState(0,Qt::Checked);
                            lv2->setCheckState(0,Qt::Checked);
                        }
                        else
                        {
                            lv3->setCheckState(0,Qt::Unchecked);
                            lv2->setCheckState(0,Qt::Unchecked);
                        }
                        QString sName=pss+" "+sss +"               ";
                        QString sTip="    ";
                        sName= sName.left(20)+codec->toUnicode(ccc);

                        MYzArray* pYazArray = znak->GetYazArray();
                        if(!pYazArray) continue;
                        MYaz* pYaz;
                        for(int s=0; s<pYazArray->GetSize(); s++)
                        {
                            pYaz = znak->GetYaz(s);
                            if(pYaz->GetType()==F_DOT)   { lv3-> setText(2,  tr("Dot")   );  sTip="dot";  }
                            if(pYaz->GetType()==F_LIN)   { lv3-> setText(2,  tr("Line")  );  sTip="line"; }
                            if(pYaz->GetType()==F_PLO)   { lv3-> setText(2,  tr("Area")  );  sTip="area"; }
                            if(pYaz->GetType()==IDL_PLS) { lv3-> setText(2,  "strip" );  cout <<"STRIP"<<endl; }
                        }
                        lv3->setText(4,  QString::number(cntMapList[ii]));
                    }
                }//end for(int k=0; k<znaki->GetSize(); k++)

                if (lv2) {
                    int total, cnt;
                    total= cnt= 0;
                    int childCount = lv2->childCount();
                    for (int i=0;i<childCount;i++)
                    {
                        total++;
                        if (((QCheckBox *)list3->itemWidget(lv2->child(i),0))->isChecked()) cnt++;
                    }

                    if (cnt)         lv2->setText(1, QString::number(cnt));
                    if (cnt== total) lv2->setCheckState(0,Qt::Checked);
                    if (cnt== 0)     lv2->setCheckState(0,Qt::Checked);
                    if (total == 0)  lv2-> parent()-> removeChild(lv2);
                }

            }//end for(int j=0; j<razdeli->GetSize(); j++)
        }
    }//end for(int i=0; i<znsistemi->GetSize(); i++)
    for (int ii=0;ii<list10->columnCount();ii++)
        if(list10->columnWidth(ii)!=0)
            list10->resizeColumnToContents(ii);
}

void MConfig::saveMinScale(int value)
{
    if(sliderScaleMinEnable)
    {
        if (value == sliderScaleMin->maximum())
        {
            value--;
            sliderScaleMin->setValue(value);
        }
        else if (value > sliderScaleMax->value())
            sliderScaleMax->setSliderPosition(value+1);
        QString text = list->currentItem ()->text (1);
        karta->layerDrawFilter->SetFilterScale(text,scaleList.at(value) * 100,scaleList.at(sliderScaleMax->value()) * 100);
        setScaleSpanLabel(scaleList.at(value) * 100,scaleList.at(sliderScaleMax->value()) * 100);
    }
    else
        sliderScaleMin->setSliderPosition(0);
    pbOK->setEnabled(true);
    return;
}

void MConfig::saveMaxScale(int value)
{
    if(sliderScaleMaxEnable)
    {
        if (value == 0)
        {
            value++;
            sliderScaleMax->setValue(value);
        }
        else
            if (value < sliderScaleMin->value())
                sliderScaleMin->setSliderPosition(value-1);
        QString text = list->currentItem ()->text (1);
        karta->layerDrawFilter->SetFilterScale(text,scaleList.at(sliderScaleMin->value()) * 100,scaleList.at(value) * 100);
        setScaleSpanLabel(scaleList.at(sliderScaleMin->value()) * 100,scaleList.at(value) * 100);
    }
    else
        sliderScaleMax->setSliderPosition(sliderScaleMax->maximum());
    pbOK->setEnabled(true);
    return;
}

void MConfig::sliderEnableSlot()
{
    if (!list->currentItem())
        return;
    QString name = list->currentItem ()->text (1);
    if(karta->layerDrawFilter->layerMap.keys().contains(name))
    {
        int pos_min = scaleList.indexOf(karta->layerDrawFilter->layerMap[name][0] / 100);
        if (pos_min == scaleList.length()-1)
            pos_min = 0;
        int pos_max = scaleList.indexOf(karta->layerDrawFilter->layerMap[name][1] / 100);
        sliderScaleMin->setSliderPosition(pos_min);
        sliderScaleMax->setSliderPosition(pos_max);
        setScaleSpanLabel(scaleList.at(pos_min) * 100,scaleList.at(pos_max) * 100);
    }
    else
    {
        sliderScaleMin->setSliderPosition(0);
        sliderScaleMax->setSliderPosition(sliderScaleMax->maximum());
        labScaleSpan->setText(tr("Full range"));
    }

    if (!sliderScaleMinEnable) sliderScaleMinEnable = true;
    if (!sliderScaleMaxEnable) sliderScaleMaxEnable = true;
}

void MConfig::sliderToZeroSlot()
{
    sliderScaleMin->setSliderPosition(0);
    sliderScaleMax->setSliderPosition(scaleList.length()-1);
}
void MConfig::setScaleSpanLabel(int minScale, int maxScale)
{
    QString s1,s2;
    if (minScale == scaleList.first() * 100)
        s1 = "";
    else s1 = QString("from 1:%1 ").arg(QString::number(minScale));
    if (maxScale == 0/*scaleList.last()*/)
        s2 = "";
    else s2 = QString("to 1:%1").arg(QString::number(maxScale));
    if ((s1 == "")&&(s2 == ""))
        labScaleSpan->setText(tr("Full range"));
    else labScaleSpan->setText(s1 + s2);
}
//---------------------------------------------------
//
void MConfig::applyButton()
{
    if (node1== 0) return;

    setDrawFilter();

    refreshForm1();
    refreshForm5();
    drawMap();
    pbOK->      setEnabled(false);
    emit  drawClicked();
}

void MConfig::setSadowOnRelief(int state)
{
    //MLayer* pLay = IMap->m_karta->FindLayer(MString(tr("Relief"). toLatin1()));
    QString sName = tr("Terrain");
    MLayer* pLay = IMap->m_karta->FindLayer(MString(codec->fromUnicode(sName).toStdString()));
    if (!pLay)
        return;

    MObjectList *m_objList = new MObjectList();
    pLay->GetObjectList(m_objList);
    OBITER it;
    for(it = m_objList->begin();it!=m_objList->end();it++)
    {
        MReliefRaster* raster = dynamic_cast<MReliefRaster*>(*it);
        if(raster)
        {
            raster->setShader(state);
            pbOK->setEnabled(true);
        }
    }
}

void MConfig::showReliefButton()
{
    if(mtxFile.isEmpty() || mtxFile.isNull() || !IMap->m_karta->m_bmhfile)
    {
        return;
    }

    QString sName = tr("Terrain");
    MString namelayer = MString(codec->fromUnicode(sName).toStdString());
    if (!IMap->m_karta->FindLayer(namelayer))
    {
        MReliefRaster* raster = new MReliefRaster(IMap->GetGeoBounds(true), IMap);
        // reliefKeyPanel = new MReliefKeyPanel(w1, 9000, -500);
        // grid->addWidget(reliefKeyPanel,9,0,2,5);

        //raster->setShader(true);
        raster->setShader(drawReliefShadow->checkState());
        MLayer* pLay = new MLayer(IMap->GetGeoBounds(true));
        pLay->m_name = namelayer;
        pLay->AddObject(raster);
        IMap->m_karta->AddLayer(pLay);
        refreshForm1();
        //drawMap();
        //emit  drawClicked();
    }

}

//---------------------------------------------------
//
void MConfig::refreshLayerItem()
{
    if (node1== 0) return;

    QString strSearch=  node1->text(1);

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())
    {     // i++;
        QString sss= codec->toUnicode((*it)->m_name.c_str());
        if ( sss == strSearch )
        {
            (*it)->  x_visible     = visible;
            (*it)->  x_textvisible = signature;
            //transparent
            (*it)->  m_bDrawSmall  = navigator;
            (*it)->  m_bSearchIn = findIn;
            (*it)->  x_common      = simple;
            (*it)->  m_commoncolor_pen=    cLine;
            (*it)->  m_commoncolor_penTransparent=iLineTransparent;
            (*it)->  m_commoncolor_brush=  cFiller;
            (*it)->  m_commoncolor_brushTransparent = iFillerTransparent;
            (*it)->  m_commoncolor_text=   cText;
            (*it)->  m_commonpenwidth=     wLine;
            (*it)->  m_commontextwidth=    wText;
        }
        it++;
    }
    pbOK->      setEnabled(true);

}

//---------------------------------------------------
//
void MConfig::ok5Button()
{
    IMap->    SetDrawFilterSize(sbSize->value());
    IMap->    SetTextSizeMin(sbTextMin->value());
    IMap->    SetTextSizeMax(sbTextMax->value());
    IMap->    SetBackgroundColor(cGround);
    drawMap();
    pbOK5->      setEnabled(false);
    emit  drawClicked();
}


//@@@@@///////////////////////////////////////
void MConfig::destCooChanged(int index)
{	
    //	if (comboDestinationCoo->itemText(index) != lineCurrentCoo->text())
    //	{
    //		this->btnConvertCoo->setEnabled(true);
    //	}
    //	else if (comboDestinationElipsoid->currentText() == lineCurrentElipsoid->text())
    //	{
    //		//this->btnConvertCoo->setEnabled(false);
    //	}
    //ad destination params to list

    int sz = parnames[index].GetSize();
    if (sz < 0)
    {
        cooParamTable->setRowCount(0);
        return;
    }
    cooParamTable->setRowCount(sz);
    for(int i = 0; i < sz; ++i)
    {
        QTableWidgetItem* itemname = new QTableWidgetItem();
        itemname->setFlags(Qt::ItemIsEnabled);
        itemname->setText(codec->toUnicode(parnames[index][i]->c_str()));
        cooParamTable->setItem(i, 0, itemname);

        QTableWidgetItem* itemdata = new QTableWidgetItem();
        itemdata->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled |Qt::ItemIsEditable);
        itemdata->setText(codec->toUnicode(pardata[index][i]->c_str()));
        cooParamTable->setItem(i, 1, itemdata);
    }
}
void MConfig::destElipsoidChanged(int index)
{
    //if (comboDestinationElipsoid->itemText(index) != lineCurrentElipsoid->text())
    //{
    //	this->btnConvertCoo->setEnabled(true);
    //}
    //else if (comboDestinationCoo->currentText() == lineCurrentCoo->text())
    //{
    //	//this->btnConvertCoo->setEnabled(false);
    //}
}
void MConfig::convertCooClicked(bool)
{
    int idxCoo = this->comboDestinationCoo->currentIndex();
    MCoo* coo = CooArray[idxCoo];
    QString elipsoid_name = this->comboDestinationElipsoid->currentText();
    QString* names;
    QString* data;
    int paramNumber = this->cooParamTable->rowCount();
    if (paramNumber > 0)
    {
        names = new QString[paramNumber];
        data = new QString[paramNumber];
    }

    for(int i = 0; i < paramNumber; ++i)
    {
        names[i] = cooParamTable->item(i, 0)->text();
        data[i] = cooParamTable->item(i, 1)->text();
    }
    this->ConvertToCoo(coo, elipsoid_name, paramNumber, names, data);
    if (paramNumber > 0)
    {
        delete [] names;
        delete [] data;
    }
    this->lineCurrentCoo->setText( comboDestinationCoo->currentText());
    this->lineCurrentElipsoid->setText(comboDestinationElipsoid->currentText());
    //this->btnConvertCoo->setEnabled(false);


}
/////////////////////////////////////////
//---------------------------------------------------
//
void MConfig::load()
{
    if (list2->currentItem()) {
        setLoadFilterEnabled(false);
        loadFlagList[id2-1] =  onFlag;
        loadMapExtra(loadFileList[id2-1].toUtf8().data());
        if (list2->currentItem())list2->currentItem()->setCheckState(1, Qt::Checked);
        if (list2->currentItem())list2->currentItem()->setCheckState(2, Qt::Unchecked);
        refreshLayerList();
        setupMapExtra();
        emit typesWinToBeRefreshed();
    }
    setDrawFilter();
    drawMap();
    emit  drawClicked();
}
//---------------------------------------------------
//
void MConfig::loadSelected()
{
    if (list2->currentItem ()) {
        setLoadFilterEnabled(true);
        loadFlagList[id2-1] =  onPartFlag;
        loadMapExtra(loadFileList[id2-1].toUtf8().data());
        list2->currentItem()->setCheckState(1,Qt::Unchecked);
        list2->currentItem()->setCheckState(2,Qt::Checked);
        refreshLayerList();
        setupMapExtra();
        emit typesWinToBeRefreshed();
    }
    setDrawFilter();
    refreshForm2();
    drawMap();
    emit  drawClicked();
}
//---------------------------------------------------
//
void MConfig::unload()
{
    if (list2->currentItem()) {
        if (list2->currentItem()->checkState(1)==Qt::Unchecked &&
                list2->currentItem()->checkState(2)==Qt::Unchecked) return;
        loadFlagList[id2-1] = "";
        QFileInfo fInfo(loadFileList[id2-1]);
        if (fInfo.suffix().toLower()=="utp")
        {
            eraseMapExtra(loadFileList[id2-1].toUtf8().data());
            list2->currentItem()->setCheckState(1, Qt::Unchecked);
        }
        refreshLayerList();
        setupMapExtra();
        emit typesWinToBeRefreshed();
    }
    setDrawFilter();
    drawMap();
    emit  drawClicked();
}
//---------------------------------------------------
//
void MConfig::ok10Button() {
    w10-> hide();
    refreshForm1();
}
//-----------------------------------------
//! To move the current layer up on 1
void MConfig::upReal() {
    QString name= layerNameList[id1-1];
    if (id1>1) {
        layerNameList[id1-1]= layerNameList[id1-2];
        layerNameList[id1-2]= name;
        id1--;
    }
    sortLayers();
    refreshForm1();
    pbOK->     setEnabled(true);
}
//-----------------------------------------
//! To move the current layer up on 1
void MConfig::downReal() {
    QString name= layerNameList[id1-1];
    if (id1<layerNameList.count()) {
        layerNameList[id1-1]= layerNameList[id1];
        layerNameList[id1]= name;
        id1++;
    }
    sortLayers();
    refreshForm1();
    pbOK->     setEnabled(true);
}
//--------------------------------------------
//!
void MConfig::cancelButton() {
    tabDialog-> close();
    emit closeClicked();
}
void MConfig::OKButtonGlobal() {
    applyButton();
    //saveButton();
    cancelButton();
}
// -------------------------------------------
//! To keep settings in the current
void MConfig::saveButton() {

    if (isNewMap)
        saveAsButton();
    else
    {
        if (saveConfig(configFile.toLocal8Bit().data()))
            emit configFileSaved(configFile.toLocal8Bit().data());
    }
    //saveConfig(configFile. toLatin1());
    refreshForm1();
    refreshForm2();
    tabDialog->show();
    tabDialog->raise();
}
// -------------------------------------------
//!
void MConfig::saveAsButton()
{
    if (!IMap)   return;
    karta= IMap-> GetKarta();
    if (! karta) return;

    /*     int i;
cout << endl;
    cmpList.sort();
    for (i=0; i< cmpList.count(); i++)
        cout << cmpList.at(i).toLocal8Bit().constData()<<"  -not found in CMP-file" << endl;//toLocal8Bit().constData()
cout << endl;
    cmfList.sort();
    for (i=0; i< cmfList.count(); i++)
        cout << cmfList.at(i).toLocal8Bit().constData() <<"  -not found in CMF-file"<< endl;
cout << endl;
    klsList.sort();
    for (i=0; i< klsList.count(); i++)
        cout << klsList.at(i).toLocal8Bit().constData() <<"  -not found in KLS"<< endl;
cout << endl;
    codeSXF.sort();
    for (i=0; i< codeSXF.count(); i++)
        cout << codeSXF.at(i).toLocal8Bit().constData() << endl;

    FILE *fl; fl=fopen("/tmp/united.utp", "w");
    karta-> WriteBin(fl);
*/
    QString fn = QFileDialog::getSaveFileName(tabDialog, tr("Enter project name"), utpDirPath,"Map Projects (*.ini *.INI);;All Map Projects (*.*)");
    if (!fn.isEmpty())
    {

        configFile = fn.right(4);
        configFile = configFile.toLower ();
        if (configFile != ".ini")
            fn = fn + ".ini";

        configFile= fn;
        if (saveConfig(configFile.toLocal8Bit().data()))
        {
            isNewMap = false;
            emit configFileSaved(configFile.toLocal8Bit().data());
        }
        // saveConfig(configFile. toLatin1());
        refreshForm1();
        refreshForm2();
    }

    tabDialog-> setWindowTitle(tr("Map project")+" < "+configFile+ " >");
    //tab-> setCaption("Map project  < "+configFile+ " >");
}
//--------------------------------------------
//
void MConfig::objectsClicked() {
    //pbOK3->     setEnabled(true);
}
//------------------------------------
//

void MConfig::insertButton()
{
    QStringList fList = QFileDialog::getOpenFileNames(
                tab,
                "Select one or more files to open",
                "/home",
                "Map files (*.utp *.UTP);;Shape files (*.shp *.SHP)");

    for (int i=0;i<fList.count();i++)
    {
        QString fn(fList[i]);
        if (!fn.isEmpty()) {
            if ((loadFileList.indexOf(fn)==-1)
                    &&  ( utpFile != fn  ))     {
                loadFileList.append(fn);
                loadFlagList.append("");
                if (id2==0) id2=1;
            }
            refreshForm2();
        }
    }
    refreshForm2();
}
//--------------------------------------------
//!
void MConfig::deleteButton() {
    QList<QTreeWidgetItem *> toBeDeleted(list2->selectedItems());
    QStringList toBeDeletedNames;
    for (int i=0;i<toBeDeleted.size();i++)
        toBeDeletedNames.append(toBeDeleted[i]->text(0));
    for (int j=0;j<toBeDeletedNames.size();j++)
    {
        QList<QTreeWidgetItem *> fndList(list2->findItems(toBeDeletedNames[j],Qt::MatchRecursive));
        if (fndList.size()>0)
        {
            list2->setCurrentItem(fndList[0]);
            id2 = list2->currentItem()->text(3).toInt();
            unload();
            loadFileList.removeAt(id2-1);
            loadFlagList.removeAt(id2-1);
            refreshForm2();
        }

    }
}
//------------------------------
void MConfig::visibleClicked() {
    //  visible= ckVisible->isChecked();
    refreshLayerItem();
}
//------------------------------
void MConfig::signatureClicked() {
    //signature = ckTextVisible->isChecked();
    refreshLayerItem();
}
//
void MConfig::navigatorClicked() {
    //  navigator= ckNavigator->isChecked();
    refreshLayerItem();
}
//
void MConfig::FindInClicked() {

    refreshLayerItem();
}

//--------------------------------------------
void MConfig::currentLayer(QTreeWidgetItem *nd){
    if (nd==0) return;
    node1 = nd;
    if (! IMap)  return;
    QString  name =  node1->text(1);
    MLayer *layer =findLayerByName(name);
    if (layer== 0) return;

    pbUp->   setEnabled(true);
    pbDown-> setEnabled(true);
    if (node1)
    {
        if (node1->text(0).toInt()== 1)           pbUp->   setEnabled(false);
        if (node1->text(0).toInt()==(int)layerNameList.count())  pbDown-> setEnabled(false);

        id1=  node1->text(0).toInt();  //cout << " on id1=  "<< id1<< endl;
        node1->setSelected(true);

        visible =node1->checkState(2)==Qt::Checked?true:false;
        simple=node1->checkState(3)==Qt::Checked?true:false;
        filter=node1->checkState(4)==Qt::Checked?true:false;
        navigator= node1->checkState(5)==Qt::Checked?true:false;
        findIn=node1->checkState(6)==Qt::Checked?true:false;
        signature = node1->checkState(7)==Qt::Checked?true:false;
    }
    if ((filter) && (layerNameFilterList.indexOf(name)==-1))
        layerNameFilterList.append(name);
    else
        layerNameFilterList.removeAt(layerNameFilterList.indexOf(name));


    wLine=   layer-> m_commonpenwidth;
    wText=   layer-> m_commontextwidth;
    cLine=   layer-> m_commoncolor_pen;
    cFiller= layer-> m_commoncolor_brush;
    iFillerTransparent = layer->m_commoncolor_brushTransparent;
    iLineTransparent = layer->m_commoncolor_penTransparent;
    cText=   layer-> m_commoncolor_text;

    createLayerObjectList(name);

    int i, cnt = 0;
    int total   = objectLayerList.count();
    for (i=0; i< total; i++) {
        if ( objDrawList.indexOf(objectLayerList[i]) >= 0) cnt++;
    }

    if ( total == 0)  {
        pbSelect->  setText("");
        labSelect-> setEnabled(false);
        pbSelect->  setEnabled(false);
        layerNameFilterList.removeAt(layerNameFilterList.indexOf(name));
        if (node1)
        {
            node1->setCheckState(4,Qt::Unchecked);
        }
        filter = false;
    }
    else     {
        labSelect-> setEnabled(true);
        pbSelect->  setEnabled(true);
        if ( cnt == 0 )   {
            pbSelect->  setText("-/"+ QString::number(total));
            layerNameFilterList.removeAt(layerNameFilterList.indexOf(name));
            if(node1)
            {
                node1->setCheckState(4,Qt::Unchecked);
            }
            filter = false;
        }
        else
            pbSelect->  setText( QString::number(cnt)+"/"+ QString::number(total));
    }

    refreshLayerItem();
    fillSubForm();
    if (w10)   w10-> hide();
}
//--------------------------------------------
void MConfig::currentFile(QTreeWidgetItem *nd, int column)  {
    if (!nd) return;
    if(GetTreeItemDepth(nd)==0) {
        pbDelete2->setEnabled(false);
        return;
    }
    id2=nd->text(3).toInt();
    if ((column ==1) && (nd->checkState(1)==Qt::Unchecked))
    {
        if(nd->checkState(2)==Qt::Checked)
            unload();
        load();
    }
    else if ((column ==1) && (nd->checkState(1)==Qt::Checked))
    {
        unload();
    }

    if ((column==2) && (nd->checkState(2)==Qt::Unchecked) && (objLoadList.count()>0))
    {
        if (nd->checkState(1)==Qt::Checked)
            unload();
        loadSelected();
    }
    else if ((column==2) && (nd->checkState(2)==Qt::Checked))
    {
        unload();
    }

    pbDelete2->setEnabled(isEnabledByGB);

}
//--------------------------------------------
//click in "Load filter" ListView
void MConfig::currentObject(QTreeWidgetItem *nd, int column)
{
    QPixmap okPix(xpmOK);
    QIcon okIcon(okPix);
    lab33-> setAlignment(Qt::AlignCenter);
    if (! nd)  return;
    if (! mki) return;
    if (! KLS) return;
    int dense= IMap->GetActPolygonDense();
    IMap->SetActPolygonDense(Qt::SolidPattern);

    node3 = nd;
    node3->   setSelected(true);
    int depth = GetTreeItemDepth(node3);

    if (depth==1) {
        enum Qt::CheckState chkState = node3->checkState(0);
        if (chkState != Qt::PartiallyChecked)
        {
            int childCount = node3->childCount();
            for(int i=0;i<childCount;i++)
            {
                if(chkState!=node3->child(i)->checkState(0))
                {
                    if (chkState== Qt::Checked)
                    {
                        objLoadList.append(node3->child(i)->text(TID_UZN));
                    }
                    else
                    {
                        objLoadList.removeAt(objLoadList.indexOf(node3->child(i)->text(TID_UZN)));
                    }
                }

                node3->child(i)->setCheckState(0,chkState);
            }
            if(chkState== Qt::Checked)
                node3->setText(1, QString::number(childCount)+ QString('/')+ QString::number(childCount));
            else
                node3->setText(1, QString::number(0)+ QString('/')+ QString::number(childCount));
        }
    }
    if (depth==2)
    {

        if (node3->checkState(0)==Qt::Unchecked)
            objLoadList.removeAt(objLoadList.indexOf(node3->text(TID_UZN)));
        else
            objLoadList.append(node3->text(TID_UZN));

        //cEm->repaint();

        int total, cnt;
        total= cnt= 0;
        QTreeWidgetItem *prn = node3->parent();
        total = prn->childCount();
        for(int i=0;i<total;i++ )
        {
            if (prn->child(i)->checkState(0)==Qt::Checked)
                cnt++;
        }

        prn->setText(1, QString::number(cnt)+ QString('/')+ QString::number(total));
        if (cnt<total && cnt!=0) prn->setCheckState(0, Qt::PartiallyChecked);
        if (cnt ==0)prn->setCheckState(0, Qt::Unchecked);
        if (cnt == total) prn->setCheckState(0, Qt::Checked);

        QByteArray ccc= codec->fromUnicode(node3->text(TID_UZN));
        MString kkod= ccc.data();
        MString  true_kod;
        MZnak* znak =  KLS-> GetZnak(kkod, true_kod);

        if (znak)
        {
            MYzArray* pYazArray = znak->GetYazArray();
            if(pYazArray)                {
                MYaz* pYaz;
                for(int s=0; s<pYazArray->GetSize(); s++)
                {
                    pYaz = znak->GetYaz(s);
                    QPixmap str = CreatePixmap(KLS, pYaz, znak);
                    lab33->setPixmap(str);
                }  // if (pYazArray)

            }  // if (znak)
        }   // depth() == 2
        IMap->SetActPolygonDense(dense);
    }
}


//--------------------------------------------------------------------

QPixmap MConfig::CreatePixmap(MKlassifikator* kls, MYaz* c_yaz, MZnak* znak)
{
    CreateTopObject();
    GetPointForObject(c_yaz);

    QPixmap str_image;

    MTopobject* pointer;

    bool centerPoint;
    if(c_yaz->GetType() == F_DOT)
    {
        pointer = DOT_topObj;
        centerPoint = true;
    }
    else if(c_yaz->GetType() == F_LIN)
    {
        pointer = LIN_topObj;
        centerPoint = false;
    }
    else if(c_yaz->GetType() == F_PLO)
    {
        pointer = PLO_topObj;
        centerPoint = false;
    }
    str_image = CreatePixmapImage(pointer, centerPoint, znak, c_yaz);

    return str_image;
}


QPixmap MConfig::CreatePixmapImage(MTopobject* pointer, bool CenterPoint, MZnak* znak, MYaz* yaz)
{
    MZnak* currentZnak;
    currentZnak = znak;

    QSize svgSize(120, 120);
    MTopobject *tpoPointer;
    tpoPointer = pointer;
    tpoPointer->m_pZnak = currentZnak;

    QRect rect(0, 0, 120, 120);
    QPixmap pixmap(rect.size());

    QPainter my_Painter;
    my_Painter.begin(&pixmap);
    MQtDevice *my_QtDevice = new MQtDevice(&my_Painter);
    MRect logRect(0.0, 0.0,(float)svgSize.width(),(float)svgSize.height());
    MRect m_zoombounds = tpoPointer->m_bounds;
    m_zoombounds *=1.1;
    MRect temp = zoomRectangle;
    zoomRectangle = m_zoombounds;
    my_QtDevice->SetCooSys(logRect, m_zoombounds);

    MPoint dotPt = tpoPointer->GetPoint(0);
    MPoint logPt;
    my_QtDevice->BumToLogic(&dotPt,logPt);

    tpoPointer->Draw2(my_QtDevice, yaz);
    zoomRectangle = temp;

    my_Painter.end();

    return pixmap;
}

void MConfig::CreateTopObject()
{
    MLayer* layer = new MLayer();
    karta = new MKarta();
    karta->ConnectKlassifikator(KLS);

    if (DOT_topObj)
    {
        DOT_topObj = NULL;
        DOT_topObj = new MTopobject();
    }
    else
        DOT_topObj = new MTopobject();

    if (LIN_topObj)
    {
        LIN_topObj = NULL;
        LIN_topObj = new MTopobject();
    }
    else
        LIN_topObj = new MTopobject();

    if (PLO_topObj)
    {
        PLO_topObj = NULL;
        PLO_topObj = new MTopobject();
    }
    else
        PLO_topObj = new MTopobject();

    layer->AddObject(DOT_topObj);
    layer->AddObject(LIN_topObj);
    layer->AddObject(PLO_topObj);
    karta->AddLayer(layer);
}

void MConfig::readJson(const MString &fileName)
{
    //1. Open the QFile and write it to a byteArray and close the file
    QFile file;
    file.setFileName(QString(fileName.c_str()));
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Json filef couldn't be opened/found";
        return;
    }

    QByteArray byteArray;
    byteArray = file.readAll();
    file.close();

    //2. Format the content of the byteArray as QJsonDocument
    //and check on parse Errors
    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);
    if(parseError.error != QJsonParseError::NoError){
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return;
    }
    //3. Create a jsonObject and fill it with the byteArray content, formatted
    //and holding by the jsonDocument Class
    QJsonObject jsonObj;
    jsonObj = jsonDoc.object();
    //QVariantMap jsonMap =jsonObj.toVariantMap();
    foreach(QJsonValue element, jsonObj["cmpTable"].toArray())
    {
        QJsonObject node = element.toObject();
        qDebug()<< node["quadrocode"] << " "<< node["codersc"];
        QString key = node["quadrocode"].toString();
        uznToSxfCodeMap[key] = node["codersc"].toObject();
    }
}

bool MConfig::saveSXF(QString sxfName)
{
    MString   sxfFile(sxfName.toStdString());
    MString   filename = sxfFile;

    if (!IMap)  return false;
    IMap-> SetVerboseFlag(false);
    karta = IMap-> GetKarta();
    if (!karta) return false;

    //======================================
    //TR: SXF
    //EN: Begin SXF writing
    //======================================
    FILE* FileSxf = fopen(filename, "wb");
    if(!FileSxf)
        return false;

    SXFPASP_4_0  * sxf_pasp = (SXFPASP_4_0  *)calloc(1,sizeof(SXFPASP_4_0));
    if(!sxf_pasp)
        return false;
    sxf_pasp->Edition = 262144;// 0x00040000  "SXF 4_0"
    sxf_pasp->CheckSum = 0;// КОНТРОЛЬНАЯ СУММА ФАЙЛА
    strcpy(&sxf_pasp->CreationDate[0],QDate::currentDate().toString("yyyyMMdd").toUtf8()+"\0 ");
    MString name = karta->m_name;
    QString s(name.c_str());
    QStringList list =  s.split(" ");
    if(list.count()>1)
    {
        strcpy(&sxf_pasp->SheetName[0],list.first().toUtf8());// Условное название
        strcpy(&sxf_pasp->Nomenclature[0],list.at(1).toUtf8());// Код номенклатуры
    }
    else
    {
        strcpy(&sxf_pasp->SheetName[0],"List");// Условное название
        strcpy(&sxf_pasp->Nomenclature[0],list.first().toUtf8());// Код номенклатуры
    }

    QString sMashtab(karta->m_mastab.c_str());
    sxf_pasp->Scale = sMashtab.split(":").at(1).toInt();// знаменатель масштаба

    sxf_pasp->StateFlag =  3; // Флаг состояния данных
    sxf_pasp->ProjectionFlag =1; // Флаг соответсвия проекции
    sxf_pasp->CoordinateFlag =1; // Флаг наличия реальных координат
    sxf_pasp->MethodFlag     =0; // Флаг метода кодирования
    sxf_pasp->GeneralizationTable=0;    // 0xххxxхх - уровень генерализации задан по таблице мелкомасштабных карт ;
                                        //1xхxxхxх - уровень генерализации задан по таблице крупномасштабных карт

    sxf_pasp->CoordinatePrecisionFlag = 1;
    sxf_pasp->SpetialSorting = 0;
    sxf_pasp->Placeholder2  = 0    ; // Резерв = 0
    sxf_pasp->DeviceCapability = -1;

    //  ПРЯМОУГОЛЬНЫЕ КООРДИНАТЫ УГЛОВ ЛИСТА  в метрах
    MRect geoRect = IMap->GetZoombounds();

    MPoint tl = IMap->ConvertPoint( MPoint(geoRect.left,geoRect.top),false);
    MPoint rb = IMap->ConvertPoint(MPoint(geoRect.right,geoRect.bottom),false);

    MPoint leftTop = MPoint(geoRect.left,geoRect.top);//IMap->GeoToGauss(tl);
    MPoint rigtBottom = MPoint(geoRect.right,geoRect.bottom);//IMap->GeoToGauss(rb);
//    MPoint leftTop(geoRect.left,geoRect.top);
//    MPoint rigtBottom (geoRect.right,geoRect.bottom);

    sxf_pasp->YNorthWestCoordinate = leftTop.x;
    sxf_pasp->XNorthWestCoordinate = leftTop.y;
    sxf_pasp->YNorthEastCoordinate = rigtBottom.x;
    sxf_pasp->XNorthEastCoordinate = leftTop.y;
    sxf_pasp->YSouthEastCoordinate = rigtBottom.x;
    sxf_pasp->XSouthEastCoordinate = rigtBottom.y;
    sxf_pasp->YSouthWestCoordinate = leftTop.x;
    sxf_pasp->XSouthWestCoordinate  = rigtBottom.y;

    // ГЕОДЕЗИЧЕСКИЕ КООРДИНАТЫ УГЛОВ ЛИСТА

    sxf_pasp->LNorthWestCoordinate = tl.x*pi/180 ; // L северо-западного угла
    sxf_pasp->BNorthWestCoordinate = tl.y*pi/180 ; // B северо-западного угла
    sxf_pasp->LNorthEastCoordinate = rb.x*pi/180 ; // L северо-восточного угла
    sxf_pasp->BNorthEastCoordinate = tl.y*pi/180 ; // B северо-восточного угла
    sxf_pasp->LSouthEastCoordinate = rb.x*pi/180 ; // L юго-восточного угла
    sxf_pasp->BSouthEastCoordinate = rb.y*pi/180 ; // B юго-восточного угла
    sxf_pasp->LSouthWestCoordinate = tl.x*pi/180 ; // L юго-западного угла
    sxf_pasp->BSouthWestCoordinate = rb.y*pi/180 ; // B юго-западного угла

    // Формируем рамку листа в логических координатах устройства
//    QRect logicRect = IMap->GeoToLogic(geoRect);
//    sxf_pasp->XSouthWestLocation   =  logicRect.bottom()  ; // X юго-западного угла  (вертикаль)
//    sxf_pasp->YSouthWestLocation   =  logicRect.left()  ; // Y юго-западного угла (горизонталь)
//    sxf_pasp->XNorthWestLocation   =  logicRect.top()  ; // X северо-западного угла
//    sxf_pasp->YNorthWestLocation   =  logicRect.left()  ; // Y северо-западного угла
//    sxf_pasp->XNorthEastLocation   =  logicRect.top() ; // X северо-восточного угла
//    sxf_pasp->YNorthEastLocation   =  logicRect.right()  ; // Y северо-восточного угла
//    sxf_pasp->XSouthEastLocation   =  logicRect.bottom()  ; // X юго-восточного угла
//    sxf_pasp->YSouthEastLocation   =  logicRect.right()  ; // Y юго-восточного угла

    sxf_pasp->SxfLabel = 0x00465853;//4610131;//Идентификатор файла  0x00465853 (SXF)
    sxf_pasp->PassportLength = sizeof(SXFPASP_4_0);// 400

    // МАТЕМАТИЧЕСКАЯ ОСНОВА ЛИСТА
    sxf_pasp->EllipsoideKind  =1      ; // Вид эллипсоида, Красовского
    sxf_pasp->HeightSystem   = 1       ; // Система высот,Балтийская 0 - не установлено
    sxf_pasp->MaterialProjection  =1; // Проекция исх. материала,Гаусса-Крюгера
    sxf_pasp->CoordinateSystem  = 1 ; // 6 - прямоугольная условная для обзорных карт; 1- Система координат,Система 42 года
    sxf_pasp->PlaneUnit   = 0       ; // Единица измерения в плане,0  –  метры (или дискреты), 64  –  радианы, 65  –  градусы.
    sxf_pasp->HeightUnit  = 0       ; // Единица измерения по высоте, метры
    sxf_pasp->FrameKind   = 0      ; // Вид рамки, прямоугольная 0 – карта не ограничена рамкой
    sxf_pasp->MapType = 1           ; //2- обзорно-географическая; 1- Обобщенный тип карты,Топографическая 42 года

   sxf_pasp->MagneticAngle = 0;
   sxf_pasp->MeridianAngle = 0;

    if(fwrite(sxf_pasp,sizeof(SXFPASP_4_0),1,FileSxf)!=1)
    {
        free(sxf_pasp);
        return false;
    }
    free(sxf_pasp);
    SXFDESCRIPTOR_4_0 sxf_descriptor; //Дескриптор данных
    sxf_descriptor.DescriptorLength = sizeof(SXFDESCRIPTOR_4_0);//52;//Длина дескриптора
    sxf_descriptor.DataLabel = 0x00544144;//5521732;//Идентификатор данных 0х00544144  (DAT)

    MObjectList savingObjectsList;
    long RecordAmount = 0;
    LAYITER it = karta->m_actlaylist.begin();
    while (it != karta->m_actlaylist.end())
    {
        MLayer * actLayer = *it;
        it++;
        if(!actLayer->x_visible)// сохраняем только объекты оперативной обстановки
            continue;

        MObjectList objectList;
        actLayer->GetObjectList(&objectList);

        OBITER iter = objectList.begin();
        for(iter; iter != objectList.end(); ++iter)
        { 
            MTopobject* object = dynamic_cast<MTopobject*>(*iter);
            if(object && object->m_enabled && object->m_Komplekt->GetAt(object->F_TOCHKI)->Size>0 /*&&
                    uznToSxfCodeMap.find(QString(object->m_kkod.c_str()).trimmed())!=uznToSxfCodeMap.end()*/)
            {
                savingObjectsList.push_back(*iter);
                RecordAmount++;
            }
            else if(object->m_Komplekt->GetAt(object->F_TOCHKI)->Size==0)
            {
                qDebug()<<"Object without points "<<QString(object->m_kkod)<<" "<<object->m_UID;
            }
            else if(uznToSxfCodeMap.find(QString(object->m_kkod.c_str()).trimmed())==uznToSxfCodeMap.end())
            {
                 qDebug()<<"Json doesn't contain "<< object->m_kkod.c_str();
            }

        }

    }
    sxf_descriptor.DataRecordAmount = RecordAmount;//Число записей данных

    // ИНФОРМАЦИОННЫЕ ФЛАЖКИ
    sxf_descriptor.StateFlag      =3 ; // Флаг состояния данных хххххх11  -  данные в Состоянии 3 - формат обмена
    sxf_descriptor.ProjectionFlag =1; // Флаг соответствия проекции ххххх0xx  -  данные не соответствуют проекции
    // (т.е. карта может иметь поворот относительно истинного положения и некоторую деформацию);
    // xxxxx1xx  -  данные соответствуют проекции.
    sxf_descriptor.CoordinateFlag  = 1; // Флаг наличия реальных координат
    //ххх00xxx  -  вся метрика объектов представлена в условной системе координат   (в дискретах);
    //ххх11xxx  -  вся  метрика   объектов   представлена  в  реальных координатах на местности
    //в соответствии с паспортными данными листа (проекция, система    координат, единица измерения),
    //данные о масштабе и  дискретности цифрования носят справочный характер.

    sxf_descriptor.MethodFlag    =0 ; // Флаг метода кодирования
    //x00ххххх  -  классификационные коды объектов и семантических
    //характеристик представлены десятичными числами, записанными в двоичном виде
    //(например: код объекта "32100000" запишется в виде 0x01E9CEA0, код семантики "253" - в виде 0x00FD);
    //x01xxxxx  -  коды объектов и семантики являются шестнадцатеричными числами,
    //записанными в двоичном виде (например: "A200FE01" в виде 0xA200FE01, "E59A" в виде 0xE59A);
    sxf_descriptor.GeneralTable   =0; // Таблица генерализации
    //0xххxxхх - уровень генерализации задан по таблице мелкомасштабных карт (описан в таблице 4);
    //1xхxxхxх - уровень   генерализации   задан по    таблице    крупномасштабных   карт (описан в таблице 5).

    sxf_descriptor.SignatureEncoding = 1;// 0 - в кодировке ASCIIZ (Dos); 1 - в кодировке ANSI (Windows); 2 - в кодировке KOI-8 (Unix).

    sxf_descriptor.SecrecyLabel = 0;        //Гриф секретности
                                        //0 - не установлено;
                                        //1 - открытая информация;
                                        //2 - информация с ограниченным доступом;
                                        //3 - информация для служебного пользования;
                                        //4 - секретная информация;
                                        //5 - совершенно секретная информация.
    sxf_descriptor.Placeholder= 0;          //Резерв = 0 1 байт
    sxf_descriptor.Placeholder2 =0 ;     //Резерв  (4 байта)

    if(fwrite(&sxf_descriptor,sizeof(SXFDESCRIPTOR_4_0),1,FileSxf)!=1)
        return false;


// Для каждого объекта оперативного слоя создаем  Записи метрики и семантики
    bool returnValue = true;
    for(auto object:savingObjectsList)
    {
        MTopobject* tpo = dynamic_cast<MTopobject*>(object);  
       QJsonObject codeRSCObject = uznToSxfCodeMap[QString(tpo->m_kkod.c_str()).trimmed()];
       if(object->WriteSxfPoints(FileSxf,codeRSCObject,IMap,true))
           continue;
       else
       {
           returnValue = false;
           break;
       }
    }
fclose(FileSxf);

FILE* f = fopen(filename, "r+b");
if(!f)
    return false;
fseek(f, 0, SEEK_END);
int  lSize = ftell (f);
rewind (f);
char*buffer = (char*) malloc (sizeof(char)*lSize);
if (buffer != NULL)
{
     size_t res = fread (buffer,1,lSize,f);
     int checkSum=  0;
     for(int i = 0;i<lSize;++i)
     {
         if(i==12) i+=4;
         checkSum+=buffer[i];
     }
     fseek(f, 12, SEEK_SET);
     if(fwrite(&checkSum,4,1,f)!=1)
         returnValue = false;
 }
 else
     returnValue = false;

fclose(f);
return returnValue;

}

void MConfig::GetPointForObject(MYaz* yaz)
{
    float scale = yaz->GetMasstKoeff();

    MPtArray *pointArrDOT = new MPtArray();
    MPtArray *pointArrLIN = new MPtArray();
    MPtArray *pointArrPLO = new MPtArray();

    int kof = 4;
    int otst = 1000;

    //point object
    DOT_topObj->GetPointArray()->ClearAll();
    MPoint* pt = new MPoint();
    pt->x = (-1)*(scale*kof-2*scale*kof/otst);
    pt->y = (-1)*(scale*kof-2*scale*kof/otst);
    pointArrDOT->AddTail(pt);
    DOT_topObj->SetPoints(*pointArrDOT);

    //linear object
    LIN_topObj->GetPointArray()->ClearAll();
    MPoint* pt_1 = new MPoint();
    pt_1->x = scale*kof/otst;
    pt_1->y = (-1)*(scale*kof-scale*kof/otst);
    pointArrLIN->AddTail(pt_1);
    MPoint* pt_2 = new MPoint();
    pt_2->x = (scale*kof-scale*kof/otst);
    pt_2->y = (-1)*scale*kof/otst;
    pointArrLIN->AddTail(pt_2);
    LIN_topObj->SetPoints(*pointArrLIN);

    //areal object
    PLO_topObj->GetPointArray()->ClearAll();

    MPoint* pt_3 = new MPoint();
    pt_3->x = (scale*kof)/otst;
    pt_3->y = (-1)*(scale*kof)/otst;
    pointArrPLO->AddTail(pt_3);

    MPoint* pt_4 = new MPoint();
    pt_4->x = (scale*kof)/otst;
    pt_4->y = (-1)*((scale*kof)-(scale*kof)/otst);
    pointArrPLO->AddTail(pt_4);

    MPoint* pt_5 = new MPoint();
    pt_5->x = (scale*kof)-(scale*kof)/otst;
    pt_5->y = (-1)*((scale*kof)-(scale*kof)/otst);
    pointArrPLO->AddTail(pt_5);

    MPoint* pt_6 = new MPoint();
    pt_6->x = (scale*kof)-(scale*kof)/otst;
    pt_6->y = (-1)*(scale*kof)/otst;
    pointArrPLO->AddTail(pt_6);

    MPoint* pt_7 = new MPoint();
    pt_7->x = (scale*kof)/otst;
    pt_7->y = (-1)*(scale*kof)/otst;
    pointArrPLO->AddTail(pt_7);
    PLO_topObj->SetPoints(*pointArrPLO);
}


//--------------------------------------------
//
void MConfig::currentObject10(QTreeWidgetItem *nd, int column) {
    if (nd==0) return;
    node4 =   nd;
    node4->setSelected(true);

    if (GetTreeItemDepth(node4)==1)
    {
        if (node4->checkState(0)==Qt::Unchecked)
            objDrawList.removeAt(objDrawList.indexOf(node4->text(TID_UZN)));
        else if (node4->checkState(0)==Qt::Checked)
            objDrawList.append(node4->text(TID_UZN));
    }
    if (node4->checkState(0)==Qt::Checked)
        pbOK-> setEnabled(true);
}
//--------------------------------------------
//
void MConfig::refreshForm1() {
    fillListForm1();
    QList<QTreeWidgetItem *> nodeList = list->findItems(QString::number(id1+100).right(2), Qt::MatchRecursive,0);
    if(nodeList.size()>0)
        node1 = nodeList[0];
    else node1 =NULL;

    if (node1) {
        list->   setCurrentItem(node1);
        if (id1==1)  currentLayer(node1);
    }

    if(IMap->GetWidgetSmall()==NULL)
        list->hideColumn(5);
    if (w10)  w10-> hide();

}
//--------------------------------------------
void MConfig::refreshForm2() {
    fillListForm2();
    node2 = list2->currentItem();
}
//--------------------------------------------
void MConfig::refreshForm3() {
    fillListForm3();
}
//---------------------------------------------
void MConfig::refreshForm7() {
    box7->clear();
    QStringList lst;
    box7-> addItem(tr("Loaded files"));
    box7-> addItem(utpFile);
    lst.append(utpFile);
    box7-> addItem(klsFile);
    if (!mtxFile.isEmpty() && !mtxFile.isNull())  box7-> addItem(mtxFile);
    if (!mtx2File.isEmpty() && !mtx2File.isNull()) box7-> addItem(mtx2File);
    if (!mtx3File.isEmpty() && !mtx3File.isNull()) box7-> addItem(mtx3File);
    MKarta *karta= IMap-> GetKarta();
    if (! karta) return;
    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())
    {
        QString search= (*it)->m_sheetname.c_str();
        if ( lst.indexOf(search) ==-1 ) {
            lst.append(search);
            box7-> addItem(search);
        }
        it++;
    }

    MRect rct= IMap->GetGeoBounds();
    box7-> addItem("");
    box7-> addItem(tr("Map bounds"));
    box7-> addItem(tr("Longitude: ")+QString::number(rct.left)+" - "+QString::number(rct.right) );
    box7-> addItem(tr("Lattitude: ")+QString::number(rct.bottom)+" - "+QString::number(rct.top) );

    if(IMap->m_karta && IMap->m_karta->m_bmhfile && !mtxFile.isEmpty() && !mtxFile.isNull())
    {
        double altLeft = IMap->m_karta->m_bmhfile->Left();
        double altTop = IMap->m_karta->m_bmhfile->Bottom();
        double altRight = IMap->m_karta->m_bmhfile->Right();
        double altBottom = IMap->m_karta->m_bmhfile->Top();
        box7-> addItem("");
        box7-> addItem(tr("Altitude matrix bounds"));
        box7-> addItem(tr("Longitude: ")+QString::number(altLeft,'f',2)+" - "+QString::number(altRight,'f',2) );
        box7-> addItem(tr("Lattitude: ")+QString::number(altBottom,'f',2)+" - "+QString::number(altTop,'f',2) );
    }

    if (false/*objLoadList.count()*/) {
        box7-> addItem("");
        box7-> addItem(tr("Load filter"));
        objLoadList.sort();
        for( int i=0; i<objLoadList.count(); i++)
        {
            QList<QTreeWidgetItem *> nodeList(list3->findItems(objLoadList[i],Qt::MatchRecursive,TID_UZN));
            if(nodeList.size()>0)
            {
                QTreeWidgetItem *node= nodeList[0];
                box7->addItem(objLoadList[i] +"  -"+node->text(4));
            }
        }
    }

    if (false/*objDrawList.count()*/) {
        box7->addItem("");
        box7->addItem(tr("Draw filter"));
        objDrawList.sort();
        for( int i=0; i<objDrawList.count(); i++)
        {
            QList<QTreeWidgetItem *> nodeList2 = list3-> findItems(objDrawList[i],Qt::MatchRecursive,TID_UZN);
            if(nodeList2.size()>0)
            {
                QTreeWidgetItem *node= nodeList2[0];
                box7->addItem(objDrawList[i] +"  -"+node->text(4));
            }


        }
    }

    box7->addItem("");
    box7->addItem(tr("Contents"));
    int totalObj= 0;
    int totalNadp=0;

    lst.clear();
    m_laylist= &karta->m_laylist;
    it = m_laylist->begin();
    while (it != m_laylist->end())   {
        QByteArray name=   (*it)-> m_name.c_str();
        QString  qname=  codec-> toUnicode(name);
        if ( lst.indexOf(qname) < 0)   lst.append(qname);
        it++;
    }


    for(int  k=0; k < lst.count(); k++)  {
        QString name = lst[k];
        box7->addItem("      "+name);
        QStringList kodList;
        vector <int> cntList;
        int objCount=  0;
        int nadpCount= 0;
        MLayerList *m_laylist= &karta->m_laylist;
        LAYITER it = m_laylist->begin();
        while (it != m_laylist->end())   {
            QByteArray cname=  (*it)->m_name.c_str();
            QString  qname=  codec->toUnicode(cname);
            if (qname == name)  {

                MObjectList *m_oblist= &(*it)->m_oblist;
                OBITER io = m_oblist->begin();

                while (io != m_oblist->end())  {

                    MTopobject  *to= dynamic_cast<MTopobject *>  (*io);
                    if (to) {
                        QByteArray kkod=  to->m_kkod.c_str();
                        QString  qkod=  codec->toUnicode(kkod);
                        int i= kodList.indexOf(qkod);
                        if (i < 0) {
                            kodList.append(qkod);
                            cntList.push_back(1);
                        }
                        else  cntList[i]++;
                    }
                    else   nadpCount++;
                    io++;
                }
            }
            it++;
        }

        for(unsigned  j=0; j < cntList.size(); j++)  {
            objCount+= cntList[j];
            QString str= QString::number(cntList[j])+ "  -    "+ kodList[j];
            MString out;
            MZnak * p_znak = IMap->GetKarta()->m_pKls->GetZnak(kodList[j].toStdString(), out, true);
            if (p_znak)
            {
                str += "  -  "  + codec->toUnicode(p_znak->GetTermin()->GetShortname().c_str());
            }

            box7->addItem(str);

        }
        QString str= tr("layer total:");
        str += "  "+QString::number(objCount+nadpCount);
        box7->addItem(str);
        totalObj+=  objCount;
        totalNadp+= nadpCount;
    } // k

    QString str= tr("map total:");
    str += "  "+QString::number(totalObj+totalNadp);
    box7->addItem(str);
}
//---------------
//
void  MConfig::fillSubForm()
{
    sb6-> setValue(wLine);
    QColor c7, c8;
    c7.setRgb (cLine.GetR(),   cLine.GetG(),    cLine.GetB());
    c7.setAlpha(iLineTransparent);
    c8.setRgb (cFiller.GetR(), cFiller.GetG(),  cFiller.GetB());
    c8.setAlpha(iFillerTransparent);
    QPalette palette;
    palette.setColor(pb7->backgroundRole(), c7);
    pb7->setPalette(palette);
    palette.setColor(pb8->backgroundRole(), c8);
    pb8->setPalette(palette);

    if (simple)  {
        lab6->    setEnabled(true);
        lab7->    setEnabled(true);
        lab8->    setEnabled(true);
        sb6->     setEnabled(true);
        pb7->     setEnabled(true);
        pb8->     setEnabled(true);
    }
    else  {
        lab6->    setEnabled(false);
        lab7->    setEnabled(false);
        lab8->    setEnabled(false);
        sb6->     setEnabled(false);
        pb7->     setEnabled(false);
        pb8->     setEnabled(false);
    }
}
//-----------------------------------
//!
bool MConfig::loadMapExtra(const char *fl) {
    MString flpath= fl;
    QFile  file(QString::fromUtf8(fl));
    if ( ! file. exists())  return false;
    QFileInfo flInfo(file);
    if (flInfo.suffix()=="utp"||flInfo.suffix()=="UTP")
        IMap-> AddSheet(flpath);
    else if (flInfo.suffix()=="shp"||flInfo.suffix()=="SHP")
        karta->ShapefileRead(flpath);
    file.close();
    return true;
}
//-----------------------------------
//!
bool MConfig::eraseMapExtra(const char *fl) {
    MString flpath= fl;
    QFile * file = new QFile(codec -> toUnicode(fl));
    if ( ! file-> exists())  return false;
    IMap-> DeleteSheet(flpath);
    return true;
}
//-----------------------------------
//!
bool MConfig::loadConfig(bool mtxFl, bool mtx2Fl)
{
    matrixFlag= mtxFl;
    matrix2Flag= mtx2Fl;
    QFile file(configFile);
    if ( ! file. exists())  return false;
    readConfigMain(configFile);
    if (utpFile.isEmpty() || utpFile.isNull())  return false;
    if (klsFile.isEmpty() || klsFile.isNull())  return false;
    if ( matrixFlag &&(mtxFile.isEmpty() || mtxFile.isNull())) return false;
    if ( matrix2Flag &&(mtx2File.isEmpty() || mtx2File.isNull())) matrix2Flag = false;

    QFile utpfile(utpFile);  if ( ! utpfile. exists()) return false;
    QFile klsfile(klsFile);  if ( ! klsfile. exists()) return false;
    if (matrixFlag)  {
        QFile mtxfile(mtxFile);
        if ( ! mtxfile. exists()) return false;
    }
    if (matrix2Flag)  {
        QFile mtx2file (mtx2File);
        if ( ! mtx2file. exists())  return false;
    }
    return true;
}

// -----------------------------
//
void MConfig::refreshLoadList() {
    MKarta *karta= IMap-> GetKarta();
    if (! karta) return;

    int i=0;
    while( (i<loadFileList.count()) && (i<loadFlagList.count()) )
    {
        QString name=  loadFileList[i];
        bool found = false;
        MLayerList *m_laylist= &karta->m_laylist;
        LAYITER it = m_laylist->begin();
        while (it != m_laylist->end())  {
            QString sss= (*it)->m_sheetname.c_str();
            if ( sss == codec ->fromUnicode (name) ) {
                found = true;
                break;
            }
            it++;
        }

        if (found)   loadFlagList[i] = onFlag;
        else         loadFlagList[i] = "";
        i++;
    }

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())  {
        QString sss= codec->toUnicode((*it)->m_sheetname.c_str());
        if ( sss != utpFile )
            if ( loadFileList.indexOf(sss) == -1) {
                loadFileList.append(sss);
                loadFlagList.append(onFlag);
            }
        it++;
    }

    refreshLayerList();
    setupMapExtra();
    setDrawFilter();
    drawMap();
}
// -----------------------------
//
void MConfig::refreshLayerList() {
    karta= IMap-> GetKarta();
    if (! karta) return;

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())  {
        QString name= codec->toUnicode((*it)->m_name.c_str());
        if ( layerNameList.indexOf(name)== -1)
            layerNameList.append(name);
        it++;
    }
}

//-----------------------------------
//!
//
void MConfig::getCurrentConfig() {
    karta= IMap-> GetKarta();
    if (! karta) return;
    cGround= IMap-> GetBackgroundColor(false);

    layerLine     = SPLITTER;
    visibleLine   = SPLITTER;
    signatureLine = SPLITTER;
    navigatorLine   = SPLITTER;
    schemeLine      = SPLITTER;

    const unsigned int iLayerNameListCount = (unsigned int)layerNameList.count();
    for (unsigned int i=0; i< iLayerNameListCount; i++) {
        QString  name= layerNameList[i];
        MLayer *layer= findLayerByName(name); //  layer=  findLayer(real[i]);
        if (layer == 0 )    continue;
        QString sid= QString::number(i);
        layerLine+= sid +SPLITTER;
        if (layer->x_visible)     visibleLine+=     sid +SPLITTER;
        if (layer->x_textvisible) signatureLine+=   sid +SPLITTER;
        if (layer->m_bDrawSmall)  navigatorLine+=   sid +SPLITTER;
        if (layer->x_common)      schemeLine+=      sid +SPLITTER;
    }
    textMin= karta-> GetTextSizeMin();
    textMax= karta-> GetTextSizeMax();
    objSize= karta-> GetDrawFilterSize();
    p1=  karta->m_DH ;
    p2=  karta->m_DL ;
    p3=  karta->m_DS ;
    p4=  (int)(10.0*textRatio) ;
    p5 = karta->m_actBackLight;
    p6 = karta->m_actBackTransp;
    actBack = karta->m_actBackEnable;
    zoom = IMap->GetZoombounds();
}
//--------------------------------------------
//!
//
void    MConfig::setupMapExtra()
{
    karta= IMap-> GetKarta();
    if (! karta) return;
    getCurrentConfig();

    for(unsigned int i=0; i<layerNameList.count(); i++) {
        QString  strSearch= layerNameList[i];
        MLayer  *layer= findLayerByName(strSearch); //  layer=  findLayer(real[i]);
        if (layer == 0 )    continue;
        QString str= SPLITTER+QString::number(i)+SPLITTER;

        MLayerList *m_laylist= &karta->m_laylist;
        LAYITER it = m_laylist->begin();
        while (it != m_laylist->end())
        {
            QString  name= codec->toUnicode((*it)->m_name.c_str());
            if ( (name == strSearch) && ( (*it) != layer ) )
            {
                (*it)->x_visible     = false;
                (*it)->x_textvisible = false;
                (*it)->m_bDrawSmall  = false;
                (*it)->m_bSearchIn   = false;
                (*it)->x_common      = false;
                if (visibleLine.indexOf(str) >=0)      (*it)->x_visible     = true;
                if (signatureLine.indexOf(str) >=0)    (*it)->x_textvisible = true;
                if (navigatorLine.indexOf(str) >=0)    (*it)->m_bDrawSmall  = true;
                if (schemeLine.indexOf(str) >=0)       (*it)->x_common      = true;
                (*it)-> m_commonpenwidth=    layer-> m_commonpenwidth;
                (*it)-> m_commontextwidth=   layer-> m_commontextwidth;
                (*it)-> m_commoncolor_pen=   layer-> m_commoncolor_pen;
                (*it)-> m_commoncolor_brush= layer-> m_commoncolor_brush;
                (*it)-> m_commoncolor_brushTransparent = layer-> m_commoncolor_brushTransparent;
                (*it)-> m_commoncolor_penTransparent = layer-> m_commoncolor_penTransparent;

                (*it)-> m_commoncolor_text=  layer-> m_commoncolor_text;
            }
            it++;
        }

    } // for i

}
//-------------------------
//
void    MConfig::setupMap(const QString fileName)
{
    karta= IMap-> GetKarta();
    if (! karta) return;

    setupFastDraw(fileName);

    MLayerList *m_laylist= &karta->m_laylist;
    LAYITER it = m_laylist->begin();
    while (it != m_laylist->end())
    {
        QString  name= codec->toUnicode((*it)->m_name.c_str());
        int i= layerNameList.indexOf(name);
        if (i<0) {it++; continue;}

        QString str= SPLITTER+QString::number(i)+SPLITTER;  ///==
        (*it)->x_visible = false;
        (*it)->x_textvisible  = false;
        (*it)->m_bDrawSmall  = false;
        (*it)->x_common  = false;
        if (visibleLine.indexOf(str) >=0)      (*it)->x_visible     = true;
        if (signatureLine.indexOf(str) >=0)    (*it)->x_textvisible = true;
        if (navigatorLine.indexOf(str) >=0)    (*it)->m_bDrawSmall  = true;
        if (schemeLine.indexOf(str) >=0)       (*it)->x_common      = true;
        it++;
    }

    sortLayers();
}
// ---------------------
//
void    MConfig::sortLayers()
{
    MLayer* layer;
    int j=0;
    const unsigned int iLayerNameListCount = (unsigned int)layerNameList.count();
    for(unsigned int i=0; i<iLayerNameListCount; i++) {
        QString  strSearch= layerNameList[i];   //
        while(1==1)  {
            layer= findLayerAt(j);
            if (layer == 0 )    break;
            QString sss= codec->toUnicode(layer->m_name.c_str());
            if (sss == strSearch)
                j++;
            else
            {
                layer= findLayerByName(strSearch, j+1);
                if (layer == 0 )    break;
                karta-> MoveLayerUp(layer);
            }
        }
    }

}

// ------------------------------
//
void    MConfig::setupFastDraw(const QString fileName )
{
    QString str;


    QDomDocument doc( "initMap" );
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly ) ) return;

    if ( !doc.setContent( &file ) ) {
        file.close();   cout << fileName. toLatin1().data() << ":  XML-file content error" << endl;
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList nl;// = root.elementsByTagName(QString( XXutp));
    QDomNode nd;
    QDomElement e;
    int xc;


    // ----------

    MLayerList  *m_laylist= &karta->m_laylist;
    LAYITER  it = m_laylist->begin();
    while (it != m_laylist->end()) {
        QString  name= codec->toUnicode((*it)->m_name.c_str());
        int i= layerNameList.indexOf(name);
        if (i<0) {it++; continue;}

        QString str=  "ID_"+QString::number(i);   ///==
        nl = root.elementsByTagName(str);
        if( nl.count() )
        {  nd = nl.item(0);   e = nd.toElement();
            if( !e.isNull() ) {

                str=  e.attribute( XXflag);
                if ( str.length()) (*it)->x_common = true;
                str=  e.attribute( XXwLine);
                xc= str.toInt();
                if ( xc ) (*it)-> m_commonpenwidth = xc;
                str=  e.attribute( XXwText);
                xc= str.toInt();
                if ( xc ) (*it)-> m_commontextwidth = xc;
                str=  e.attribute( XXcLine);
                xc= str.toInt();
                if ( xc ) {
                    (*it)-> m_commoncolor_pen.SetR( xc/0x10000 );
                    (*it)-> m_commoncolor_pen.SetG( xc/0x100 );
                    (*it)-> m_commoncolor_pen.SetB( xc%0x100 );
                }
                str= e.attribute( XXcLineTransparent);
                xc= str.toInt();
                if ( xc ) {
                    (*it)-> m_commoncolor_penTransparent = xc;
                }
                str=  e.attribute( XXcFiller);
                xc= str.toInt();
                if ( xc ) {
                    (*it)-> m_commoncolor_brush.SetR( xc/0x10000 );
                    (*it)-> m_commoncolor_brush.SetG( xc/0x100 );
                    (*it)-> m_commoncolor_brush.SetB( xc%0x100 );
                }
                str=  e.attribute( XXcFillerTransparent);
                xc= str.toInt();
                if ( xc ) {
                    (*it)-> m_commoncolor_brushTransparent = xc;
                }
                str=  e.attribute( XXcText);
                xc= str.toInt();
                if ( xc ) {
                    (*it)-> m_commoncolor_text.SetR( xc/0x10000 );
                    (*it)-> m_commoncolor_text.SetG( xc/0x100 );
                    (*it)-> m_commoncolor_text.SetB( xc%0x100 );
                }
            }
        }
        it++;
    }
}
// ----------------------------------------------
//!
//
void    MConfig::readConfigMain(const QString fileName)
{
    QString str;
    utpFile = QString();
    klsFile = QString();
    mtxFile = QString();
    mtx2File = QString();
    mtx3File = QString();
    loadFileList.clear();
    loadFlagList.clear();
    layerNameList.clear();
    layerNameFilterList.clear();
    objLoadList.clear();
    objDrawList.clear();
    layerLine     = SPLITTER;
    visibleLine   = SPLITTER;
    signatureLine = SPLITTER;
    navigatorLine   = SPLITTER;
    schemeLine      = SPLITTER;
    cGround= IMap-> GetBackgroundColor(false);
    p1= p2= p3= 0;
    p4= 10;
    actBack=false;
    p5=karta->m_actBackLight;
    p6=karta->m_actBackTransp;
    QDomDocument doc( "initMap" );

    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly ) )     return;

    QByteArray  file_content = file.readAll();

    if ( !doc.setContent( file_content ) )
    {
        file.close();
        cout << fileName.toLocal8Bit().constData()<< ":  XML-file content error" << endl;
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList nl;// = root.elementsByTagName(QString( XXutp));
    QDomNode nd;
    QDomElement e;
    int xc;

    nl = root.elementsByTagName( XXcGround);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) {
            str=  e.attribute( XXtext);
            xc= str.toInt();
            if ( xc ) {
                cGround.SetR( xc/0x10000 );
                cGround.SetG( xc/0x100 );
                cGround.SetB( xc%0x100 );
            }
        }
    }

    nl = root.elementsByTagName( XXutp);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) utpFile=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXkls);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) klsFile=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXmtx);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) mtxFile=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXmtxobj);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) mtx2File=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXmtx3);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) mtx3File=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXlayer);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) layerLine=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXvisible);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) visibleLine=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXsignature);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) signatureLine=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXnavigator);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) navigatorLine=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXscheme);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) schemeLine=  e.attribute( XXtext);
    }
    nl = root.elementsByTagName( XXobjsize);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) objSize=  e.attribute( XXtext).toInt();
    }
    nl = root.elementsByTagName( XXtextmax);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) textMax=  e.attribute( XXtext).toInt();
    }
    nl = root.elementsByTagName( XXtextmin);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) textMin=  e.attribute( XXtext).toInt();
    }
    nl = root.elementsByTagName( XXhls);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) {
            p1=  e.attribute( XXp1).toInt();
            p2=  e.attribute( XXp2).toInt();
            p3=  e.attribute( XXp3).toInt();
            if (karta) {
            }
        }
    }

    nl = root.elementsByTagName( XXactBackground);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) {
            actBack = e.attribute( XXactBack).toInt();
            p5 = e.attribute( XXp5).toInt();
            p6 = e.attribute( XXp6).toInt();
        }
    }
    nl = root.elementsByTagName( XXmgrslevel);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) {
            mgrsTilelevel=  e.attribute( XXtext).toInt();
        }
    }

    nl = root.elementsByTagName( XXratio);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() ) p4=  e.attribute( XXtext).toInt();
    }

    nl = root.elementsByTagName( XXzoom);
    if( nl.count() ){  nd = nl.item(0);   e = nd.toElement();
        if( !e.isNull() )  {
            zoom.left=  e.attribute( XXleft).toDouble();
            zoom.top=   e.attribute( XXtop).toDouble();
            zoom.right= e.attribute( XXright).toDouble();
            zoom.bottom=e.attribute( XXbottom).toDouble();
        }
    }

    QFileInfo fileInfo(fileName);
    QDir dir = fileInfo.dir();
    QFileInfo utpFileInf(utpFile);
    if (!utpFileInf.isAbsolute())
    {
        QFileInfo utpInfo(fileInfo.absolutePath()+QDir::separator()+utpFile);
        utpInfo.makeAbsolute();
        utpFile = utpInfo.absoluteFilePath();
    }

    QFileInfo klsFileInfo(klsFile);
    if (!klsFileInfo.isAbsolute())
    {
        QFileInfo klsInfo(fileInfo.absolutePath()+QDir::separator()+klsFile);
        klsInfo.makeAbsolute();
        klsFile = klsInfo.absoluteFilePath();
    }

    //utpFile = dir.absoluteFilePath(utpFile);
    QFileInfo mtxFileInfo(mtxFile);
    if (!mtxFileInfo.isAbsolute())
    {
        QFileInfo mtxInfo(fileInfo.absolutePath()+QDir::separator()+mtxFile);
        mtxInfo.makeAbsolute();
        mtxFile = mtxInfo.absoluteFilePath();
    }

    QFileInfo mtx2Info(fileInfo.absolutePath()+QDir::separator()+mtx2File);
    mtx2Info.makeAbsolute();

    if(!mtx2File.isEmpty() && mtx2Info.exists())
    {
        mtx2File = mtx2Info.absoluteFilePath();
    }

    QFileInfo mtx3Info(fileInfo.absolutePath()+QDir::separator()+mtx3File);
    mtx3Info.makeAbsolute();
    if(!mtx3File.isEmpty() && mtx3Info.exists())
    {
        mtx3File = mtx3Info.absoluteFilePath();
    }

    nl = root.elementsByTagName( XXutp_extra);
    unsigned int i=0;
    while( i< nl.count() )
    {  nd = nl.item(i);
        e = nd.toElement();
        if( !e.isNull() )
            loadFileList.append (dir.absoluteFilePath(e.attribute( XXtext)) );
        loadFlagList.append (e.attribute( XXflag) );
        i++;
    }

    nl = root.elementsByTagName( XXobjLoad);
    i=0;
    while( i< nl.count() )
    {  nd = nl.item(i);
        e = nd.toElement();
        if( !e.isNull() )
            objLoadList.append (e.attribute( XXtext) );
        i++;
    }

    nl = root.elementsByTagName( XXobjDraw);
    i=0;
    while( i< nl.count() )
    {  nd = nl.item(i);
        e = nd.toElement();
        if( !e.isNull() )
            objDrawList.append (e.attribute( XXtext) );
        i++;
    }

    // ----------  layer order  + filter

    QString order= layerLine;
    order= order.right(order.length()-1 );
    while (order.length()>1) {
        int  j= order.indexOf(SPLITTER);
        if (j<0)  break;
        else  {
            int uid= order.left(j).toInt();
            order= order.right(order.length()-j-1 );

            QString str=  "ID_"+QString::number(uid);   ///==
            nl = root.elementsByTagName(str);
            if( nl.count() ){
                nd = nl.item(0);
                e = nd.toElement();
                if( !e.isNull() ) {
                    QString name= e.attribute( XXname);
                    layerNameList.append(name);
                    QString str=  e.attribute( XXfilter);
                    if ( str.length()>0 )  layerNameFilterList.append(name);
                    QString s_minscale = e.attribute( XXminScale);
                    QString s_maxscale = e.attribute( XXmaxScale);
                    karta->layerDrawFilter->SetFilterScale(name,s_minscale.toInt(),s_maxscale.toInt());
                }
            }
        }  // else
    }    // while


}

// ----------------------------------------------
//!
//
bool    MConfig::saveConfig(const char  *fileName)
{
    //MLayer *layer;
    if (fileName) configFile= QString(fileName);
    QString  XXnameXML = configFile.isEmpty()?"configFile.ini":configFile;

    karta= IMap-> GetKarta();
    if (! karta) return false;
    getCurrentConfig();
    QFile *file;
    QFileInfo fInfo(XXnameXML);
    if (fInfo.exists() && !fInfo.isWritable())
    {
        QMessageBox::warning(this->tab,"",tr("You do not have permission to write to the file"));
        return false;
    }

    QDomDocument doc( "initMAP" );

    QDomElement root = doc.createElement( "initMAP" );
    doc.appendChild( root );
    QDomElement tag;

    QFileInfo *fileInfo = new QFileInfo(configFile);
    QDir dir = fileInfo -> dir();

    tag = doc.createElement( XXutp);
    tag.setAttribute( XXtext,  dir.relativeFilePath(utpFile)  );
    root.appendChild( tag );
    tag = doc.createElement( XXkls);
    tag.setAttribute( XXtext,  dir.relativeFilePath(klsFile)  );
    root.appendChild( tag );
    tag = doc.createElement( XXmtx);
    tag.setAttribute( XXtext,  dir.relativeFilePath(mtxFile)  );
    root.appendChild( tag );
    tag = doc.createElement( XXmtxobj);
    tag.setAttribute( XXtext,  dir.relativeFilePath(mtx2File) );
    root.appendChild( tag );
    tag = doc.createElement( XXmtx3);
    tag.setAttribute( XXtext,  dir.relativeFilePath(mtx3File) );
    root.appendChild( tag );
    tag = doc.createElement( XXlayer);
    tag.setAttribute( XXtext,  layerLine );
    root.appendChild( tag );
    tag = doc.createElement( XXvisible);
    tag.setAttribute( XXtext,  visibleLine );
    root.appendChild( tag );
    tag = doc.createElement( XXsignature);
    tag.setAttribute( XXtext,  signatureLine );
    root.appendChild( tag );
    tag = doc.createElement( XXnavigator);
    tag.setAttribute( XXtext,  navigatorLine );
    root.appendChild( tag );
    tag = doc.createElement( XXscheme);
    tag.setAttribute( XXtext,  schemeLine );
    root.appendChild( tag );
    tag = doc.createElement( XXcGround);
    int bc= 0x10000*cGround.GetR() + 0x100*cGround.GetG() +cGround.GetB();
    tag.setAttribute( XXtext, QString::number(bc)   );
    root.appendChild( tag );
    tag = doc.createElement( XXobjsize);
    tag.setAttribute( XXtext,  objSize );
    root.appendChild( tag );
    tag = doc.createElement( XXtextmax);
    tag.setAttribute( XXtext,  textMax );
    root.appendChild( tag );
    tag = doc.createElement( XXtextmin);
    tag.setAttribute( XXtext,  textMin );
    root.appendChild( tag );
    tag = doc.createElement( XXhls);
    tag.setAttribute( XXp1,  p1 );
    tag.setAttribute( XXp2,  p2 );
    tag.setAttribute( XXp3,  p3 );
    root.appendChild( tag );
    tag = doc.createElement( XXratio);
    tag.setAttribute( XXtext,  p4 );
    root.appendChild( tag );

    tag = doc.createElement( XXactBackground);
    tag.setAttribute( XXactBack,  actBack );
    tag.setAttribute( XXp5, p5 );
    tag.setAttribute( XXp6, p6);
    root.appendChild( tag );

    tag = doc.createElement( XXmgrslevel);
    tag.setAttribute( XXtext,  mgrsTilelevel );
    root.appendChild( tag );

    tag = doc.createElement( XXzoom);
    tag.setAttribute( XXleft,  zoom.left );
    tag.setAttribute( XXtop,   zoom.top );
    tag.setAttribute( XXright, zoom.right );
    tag.setAttribute( XXbottom,zoom.bottom );
    root.appendChild( tag );

    //nl = root.elementsByTagName(QString( XXutp_extra));
    unsigned int i=0;
    const unsigned int iLoadFlagListCount = (unsigned int)loadFlagList.count();
    while( (i<iLoadFlagListCount) && (i<iLoadFlagListCount) )
    {
        tag = doc.createElement( XXutp_extra);
        tag.setAttribute( XXtext, dir.relativeFilePath(loadFileList[i])   );
        tag.setAttribute( XXflag, loadFlagList[i]   );
        root.appendChild( tag );
        i++;
    }

    int xc;
    QString str, str1 = QString::number(0.0);
    QMap<QString,QVector<int> > laymap = karta->layerDrawFilter->layerMap;

    const unsigned int iLayerNameListCount = (unsigned int)layerNameList.count();
    for ( i=0; i< iLayerNameListCount; i++)
    {
        QString  name= layerNameList[i];
        MLayer *layer= findLayerByName(name); //  layer=  findLayer(real[i]);
        if (layer == 0 )    continue;
        tag = doc.createElement("ID_"+ QString::number(i) );
        str="";
        if ( layer->x_common)
            str=onFlag;
        tag.setAttribute( XXflag, str );
        tag.setAttribute( XXwLine, QString::number(layer-> m_commonpenwidth) );
        xc= 0x10000* layer->m_commoncolor_pen.GetR() + 0x100* layer->m_commoncolor_pen.GetG() + layer->m_commoncolor_pen.GetB();
        tag.setAttribute( XXcLine, QString::number(xc) );
        tag.setAttribute( XXcLineTransparent, QString::number(layer-> m_commoncolor_penTransparent) );
        xc= 0x10000* layer->m_commoncolor_brush.GetR() + 0x100* layer->m_commoncolor_brush.GetG() + layer->m_commoncolor_brush.GetB();
        tag.setAttribute( XXcFiller, QString::number(xc) );
        tag.setAttribute( XXcFillerTransparent, QString::number(layer-> m_commoncolor_brushTransparent) );
        tag.setAttribute( XXwText, QString::number( layer-> m_commontextwidth) );
        xc= 0x10000* layer->m_commoncolor_text.GetR() + 0x100* layer->m_commoncolor_text.GetG() + layer->m_commoncolor_text.GetB();
        tag.setAttribute( XXcText, QString::number(xc) );
        //str=  layer-> m_name.c_str();
        tag.setAttribute( XXname, name);//codec->toUnicode( str. toLatin1()) );
        str="";
        if ( layerNameFilterList.indexOf(name) >= 0)
            str=onFlag;
        tag.setAttribute( XXfilter, str );
        if(laymap.keys().contains(name))
        {
            str = QString::number(laymap[name][0]);
            str1 = QString::number(laymap[name][1]);
        }
        else
            str = QString::number(0.0);
        tag.setAttribute( XXminScale, str );
        tag.setAttribute( XXmaxScale, str1 );

        root.appendChild( tag );
    }

    i=0;
    const unsigned int iObjLoadListCount =(unsigned int)objLoadList.count();
    while( i<iObjLoadListCount )
    {
        tag = doc.createElement( XXobjLoad);
        tag.setAttribute( XXtext, objLoadList[i]   );
        root.appendChild( tag );
        i++;
    }

    i=0;

    const unsigned int iObjDrawListCount =(unsigned int)objDrawList.count();
    while( i<iObjDrawListCount )
    {
        tag = doc.createElement( XXobjDraw);
        tag.setAttribute( XXtext, objDrawList[i]   );
        root.appendChild( tag );
        i++;
    }


    QByteArray xml = doc.toByteArray();


    file = new QFile(XXnameXML);
    file -> open(QIODevice::WriteOnly);
    file -> write( xml, xml.length() );
    file -> close();

    return true;
}


//-------------------------------------------------------------
//!
void  MConfig::createForm()
{                     //   0, Qt::WType_Dialog
    QPixmap upPix(xpmUP);
    QPixmap downPix(xpmDOWN);
    QIcon upIcon(upPix);
    QIcon downIcon(downPix);

    if(tab)
    {
        delete tab;
    }
    if(tabDialog)
    {
        delete tabDialog;
    }

    tabDialog = new QDialog(parent);
    tabDialog->setModal(true);
    tab = new QTabWidget(tabDialog);


    QFont ff= tab->font();
    ff.setPointSize(9);
    tab->setFont(ff);

    w1 = new QWidget(tab);
    QVBoxLayout* topLayout = new QVBoxLayout (w1);// 2- border
    topLayout->setSpacing(5);
    grid = new QGridLayout();
    topLayout->addLayout(grid, 10);

    list = new QTreeWidget(w1);
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setRootIsDecorated(false);
    list->setAlternatingRowColors(true);

    pbShowRelief = new QPushButton(w1);
    drawReliefShadow = new QCheckBox(w1);
    if(karta && karta->m_bmhfile)
    {
        pbShowRelief->setEnabled(true);
        drawReliefShadow->setEnabled(true);
    }
    else
    {
        pbShowRelief->setEnabled(false);
        drawReliefShadow->setEnabled(false);
    }
    pbOK = new QPushButton(w1);
    pbUp = new QPushButton(w1);
    pbDown = new QPushButton(w1);
    pb7 = new QPushButton(w1);
    pb8 = new QPushButton(w1);
    pbSelect = new QPushButton(w1);
    sb6 = new QSpinBox(w1);
    sb6->setMaximum(20);//Line width
    sb6->setMinimum(1);
    sb6-> setSingleStep(1);
    labSelect = new QLabel( w1 );
    lab6 = new QLabel(w1);
    lab7  = new QLabel(w1);
    lab8 = new QLabel(w1);

    connect(tab, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
    connect(list, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), SLOT(currentLayer(QTreeWidgetItem*)));
    connect(list, SIGNAL(itemClicked(QTreeWidgetItem *, int)), SLOT(currentLayer(QTreeWidgetItem*)));
    connect(pbOK, SIGNAL(clicked()), this, SLOT(applyButton()));
    connect(pbShowRelief, SIGNAL(clicked()),this, SLOT(showReliefButton()));
    connect(drawReliefShadow, SIGNAL(stateChanged(int)),this, SLOT(setSadowOnRelief(int)));
    connect(pbUp, SIGNAL(clicked()),this, SLOT(upReal()));
    connect(pbDown, SIGNAL(clicked()),this, SLOT(downReal()));
    connect(sb6, SIGNAL(valueChanged(int)), this, SLOT(widthClicked(int)));
    connect(pb7, SIGNAL(clicked()),this, SLOT(linecolorButton()));
    connect(pb8, SIGNAL(clicked()),this, SLOT(fillcolorButton()));
    connect(pbSelect, SIGNAL(clicked()),this, SLOT(selectObjectsDialog()));
    grid->addWidget(list, 0, 0, 9, 6);
    QHBoxLayout *hbox5 = new QHBoxLayout();
    hbox5->addWidget(pbShowRelief);
    pbShowRelief->setText(tr("Add terrain layer") );
    hbox5->addWidget(drawReliefShadow);
    drawReliefShadow->setCheckState(Qt::Unchecked);
    drawReliefShadow->setText(tr("Draw shadow"));
    grid->addLayout(hbox5, 9, 0, 1, 3);
    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->setSpacing(8);
    grid->addLayout(vbox, 0, 7, 8, 1);
    QVBoxLayout* vbox2 = new QVBoxLayout();
    vbox2->setSpacing(8);
    grid->addLayout(vbox2, 0, 8, 8, 1);
    QHBoxLayout* hbox10 = new QHBoxLayout();
    hbox10->setSpacing(8);
    grid->addLayout(hbox10, 9, 7, 1, 1);
    QHBoxLayout* hbox11 = new QHBoxLayout();
    hbox11->setSpacing(8);
    grid->addLayout(hbox11, 8, 6, 1, 3);
    QVBoxLayout *vboxp = new QVBoxLayout();
    vboxp->setSpacing(8);
    grid->addLayout(vboxp, 1, 6, 4, 1);

    vbox->setAlignment(Qt::AlignCenter);
    vbox2->setAlignment(Qt::AlignCenter);
    vboxp->setAlignment(Qt::AlignCenter);

    pbUp->setIcon(upIcon);
    pbUp->setFixedHeight(pbUp->sizeHint().height());
    vboxp->addWidget(pbUp);
    pbUp->setEnabled(false);
    pbDown->setIcon(downIcon);
    pbDown->setFixedHeight(pbDown->sizeHint().height());
    vboxp->addWidget(pbDown);
    pbDown->setEnabled(false);
    pbUp->setMaximumWidth(20);
    pbDown->setMaximumWidth(20);
    lab6->setText(tr("Line width"));
    lab7->setText(tr("Line color"));
    lab8->setText(tr("Brush color"));
    labSelect->setText(tr("Objects filter"));
    vbox->addWidget(lab6);
    vbox->addWidget(lab7);
    vbox->addWidget(lab8);
    vbox->addWidget(labSelect);
    lab6->setFixedHeight(lab6->sizeHint().height());
    lab7->setFixedHeight(lab7->sizeHint().height());
    lab8->setFixedHeight(lab8->sizeHint().height());
    labSelect->setFixedHeight(labSelect->sizeHint().height());
    sb6->setFixedHeight(sb6->sizeHint().height());
    pb7->setFixedHeight(lab7->sizeHint().height());
    pb8->setFixedHeight(lab8->sizeHint().height());
    pbSelect->setFixedHeight(labSelect->sizeHint().height());
    sb6->setFixedWidth(40);
    pb7-> setFixedWidth( 40);
    pb8-> setFixedWidth( 40);
    pbSelect->setFixedWidth( 40);
    vbox2-> addWidget( sb6 );
    vbox2-> addWidget( pb7 );
    vbox2-> addWidget( pb8 );
    vbox2-> addWidget( pbSelect );
    list->headerItem()->setText(0, tr("№"));

    list->headerItem()->setText(1, tr("Layer name"));

    list->headerItem()->setText(2, tr("Show"));
    list->headerItem()->setToolTip(2, tr("Layer features will be displayed on the map"));

    list->headerItem()->setText(3, tr("Simpl."));
    list->headerItem()->setToolTip(3, tr("Simplified rendering will be applied to objects in the layer"));

    list->headerItem()->setText(4, tr("Obj. filter"));
    list->headerItem()->setToolTip(4, tr("Apply a filter to objects in a layer"));

    list->headerItem()->setText(5, tr("Small map"));
    list->headerItem()->setToolTip(5, tr("Layer objects will be displayed on a small(navigation) map"));

    list->headerItem()->setText(6, tr("Search"));
    list->headerItem()->setToolTip(6, tr("Permission to search for objects in the selected layer"));

    list->headerItem()->setText(7, tr("Labels"));
    list->headerItem()->setToolTip(7, tr("Labels will be displayed next to the objects in the layer"));

    list->setAllColumnsShowFocus(true);
    list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    list->adjustSize();
    list->hideColumn(6);
#ifdef MYDEBUG
    //list->addColumn("");
#endif

    scaleGroupBox = new QGroupBox(tr("Set scale limits"));
    scaleGroupBox->setMinimumHeight (100);
    QVBoxLayout *svbox = new QVBoxLayout();
    sliderScaleMin = new QSlider(Qt::Horizontal);
    sliderScaleMin->setMinimum(0);
    sliderScaleMin->setMaximum(scaleList.length()-1);
    sliderScaleMin->setTickPosition(QSlider::TicksBelow);
    sliderScaleMin->setTickInterval(1);
    sliderScaleMin->setSingleStep(1);
    sliderScaleMin->setSliderPosition(0);
    sliderScaleMin->setTracking(true);
    //
    QHBoxLayout* slider1Layout = new QHBoxLayout();
    slider1Layout->setSpacing(6);
    slider1Layout->setObjectName(QString::fromUtf8("slider1Layout"));

    labMinScale = new QLabel();
    labMinScale->setObjectName(QString::fromUtf8("labMinScale"));
    labMinScale->setText(tr("Min."));

    slider1Layout->addWidget(sliderScaleMin);
    slider1Layout->addWidget(labMinScale);
    //
    svbox->addLayout(slider1Layout);//(sliderScaleMin);

    sliderScaleMax = new QSlider(Qt::Horizontal);
    sliderScaleMax->setMinimum(0);
    sliderScaleMax->setMaximum(scaleList.length()-1);
    sliderScaleMax->setTickPosition(QSlider::TicksAbove);
    sliderScaleMax->setTickInterval(1);
    sliderScaleMax->setSingleStep(1);
    sliderScaleMax->setSliderPosition(sliderScaleMax->maximum());
    sliderScaleMax->setTracking(true);
    //
    QHBoxLayout* slider2Layout = new QHBoxLayout();
    slider2Layout->setSpacing(6);
    slider2Layout->setObjectName(QString::fromUtf8("slider2Layout"));

    labMaxScale = new QLabel();
    labMaxScale->setObjectName(QString::fromUtf8("labMaxScale"));
    labMaxScale->setText(tr("Max."));

    slider2Layout->addWidget(sliderScaleMax);
    slider2Layout->addWidget(labMaxScale);
    //
    svbox->addLayout(slider2Layout);//(sliderScaleMax);
    labScaleSpan = new QLabel();
    labScaleSpan->setObjectName(QString::fromUtf8("labScaleSpan"));
    labScaleSpan->setText(tr("Full range"));
    svbox->addWidget(labScaleSpan);
    scaleGroupBox->setLayout(svbox);

    hbox11->addWidget(scaleGroupBox);
    //connect(sliderScaleMin,SIGNAL(sliderReleased()),this,SLOT(saveMinScale()));
    connect(sliderScaleMin,SIGNAL(sliderMoved(int)),this,SLOT(saveMinScale(int)));
    connect(sliderScaleMax,SIGNAL(sliderMoved(int)),this,SLOT(saveMaxScale(int)));
    scaleGroupBox->setEnabled(false);
    scaleGroupBox->setVisible(false);

    connect(list, SIGNAL(itemClicked(QTreeWidgetItem *,int)), SLOT(sliderEnableSlot()));

    pbOK->setText( tr("Apply") );
    pbOK->setFixedHeight( pbOK->sizeHint().height());
    hbox10->addWidget( pbOK );


    tab ->addTab( w1, tr("Layers") );

    if(mTabSetup.fTabSheets)
    {
        w2 = new QWidget( tab );

        QVBoxLayout *topLayout2 = new QVBoxLayout (w2); // 2- border
        topLayout2->setSpacing(5);
        QGridLayout * grid2     = new QGridLayout();
        grid2->setHorizontalSpacing(9);
        grid2->setVerticalSpacing(10);
        topLayout2-> addLayout (grid2, 10);

        list2        = new QTreeWidget( w2 );   // list->  setSelectionMode(QTreeWidget::Single);
        list2->setSelectionMode(QAbstractItemView::ExtendedSelection);
        list2->setAlternatingRowColors (true);
        pbInsert2    = new QPushButton( w2 );
        pbDelete2    = new QPushButton( w2 );
        connect(list2,     SIGNAL(itemClicked(QTreeWidgetItem *, int)),this, SLOT(currentFile(QTreeWidgetItem *,int)));
        connect(pbInsert2, SIGNAL(clicked()), this, SLOT(insertButton()));
        connect(pbDelete2, SIGNAL(clicked()), this, SLOT(deleteButton()));

        grid2->addWidget( list2 ,0,0,9,7 );
        QVBoxLayout *vbox21 = new QVBoxLayout();
        vbox21->setSpacing(8);
        grid2->addItem( vbox21,  0,8,9,1);//( vbox21,  0,9, 8,8 )
        vbox21->  setAlignment(Qt::AlignTop);

        list2->headerItem()->setText(0, tr("File name"));
        list2->headerItem()->setText(1, tr("Load"));
        list2->headerItem()->setText(2, tr("Filter Load"));
        list2->headerItem()->setText(3, tr("id"));
        list2->setColumnHidden (TID_UZN,true);
        list2->setAllColumnsShowFocus(true);

        vbox21->  addWidget( pbInsert2 );
        pbInsert2->setText( tr("Add") );
        pbInsert2-> setEnabled(false);
        vbox21->  addWidget( pbDelete2 );
        pbDelete2->setText( tr("Delete") );
        pbDelete2-> setEnabled(false);
        pbOK-> setEnabled(false);
        tab ->addTab( w2, tr("Sheets") );
    }

    //--------------------------
    if (mTabSetup.fTabLoadFilter)
    {
        w3 = new QWidget( tab);

        QVBoxLayout *topLayout3 = new QVBoxLayout (w3); // 2- border
        topLayout3->setSpacing(5);
        QGridLayout *grid3      = new QGridLayout();
        topLayout3-> addLayout (grid3, 10);
        list3       = new QTreeWidget( w3 );   // list->  setSelectionMode(QTreeWidget::Single);
        list3->setAlternatingRowColors (true);
        lab33       = new QLabel(w3);
        grid3->addWidget( list3,0,0,9,6 );
        QVBoxLayout *vbox31 = new QVBoxLayout();
        vbox31->setSpacing(8);
        QVBoxLayout *vbox32 = new QVBoxLayout();
        vbox32->setSpacing(8);
        grid3->addLayout( vbox31,  0,7, 9,1 );
        grid3->addLayout( vbox32,  0,8, 9,1 );
        vbox31->  setAlignment(Qt::AlignTop);
        vbox32->  setAlignment(Qt::AlignTop);


        connect(list3,  SIGNAL(itemSelectionChanged()),this, SLOT(itemSelection()));

        list3->headerItem()->setText(0, tr("Name"));
        list3->headerItem()->setText(1, tr("On"));
        list3->headerItem()->setText(2, tr("Type"));
        list3->headerItem()->setText(3, tr("Id"));
        list3->headerItem()->setText(4, tr("Name complete"));
        list3->setColumnHidden (TID_UZN,true);

        vbox31-> addWidget( lab33 );
        lab33 -> setEnabled(true);
        lab33 -> setFixedHeight(150);
        lab33 -> setFixedWidth(150);

        if (! klsFile.isEmpty())  {
            mki   =  new MKartaInterface(lab33);
            MRect  rct = IMap-> GetKarta()->GetGeoBounds();//    MRect  rct = IMap->GetGeoBounds();
            MRect  rct0;
            rct0.left  = (rct.right + rct.left)/2.0 ;
            rct0.right =  rct0.left + 3240.05;    //2160.03;
            rct0.bottom= (rct.bottom + rct.top)/2.0 ;
            rct0.top   =  rct0.bottom + 3240.05;  //2160.03;
            MString kls(klsFile. toLatin1());
            mki-> SetKlassifikator(kls);
            mki   -> GetKarta()->SetGeoBounds(rct0);
            mki   -> ZoomTo(rct0);
            MColor col(245, 245, 245);
            mki   -> SetBackgroundColor(col);
            mki   -> SetWidget( lab33 );
            mki   -> DrawMap();
        }

        tab ->addTab( w3, tr("Load filter") );
    } // add tab Load filter

    //--------------------------
    if(mTabSetup.fTabAdvanced)
    {
        w5 = new QWidget( tab );

        QVBoxLayout *topLayout5 = new QVBoxLayout (w5);
        topLayout5->setSpacing(5);
        QGroupBox *gbMapColor = new QGroupBox(w5);
        gbMapColor->setTitle (tr("Map color") );
        lab51       = new QLabel(gbMapColor);
        lab52       = new QLabel(gbMapColor);
        lab53       = new QLabel(gbMapColor);
        lab54       = new QLabel(gbMapColor);
        sl51        = new QSlider(Qt::Horizontal, gbMapColor);
        sl51->setMinimum(-25);
        sl51->setMaximum(25);
        sl51-> setPageStep(25);
        sl51->setSliderPosition(0);
        sl52        = new QSlider(Qt::Horizontal, gbMapColor);
        sl52->setMinimum(-250);
        sl52->setMaximum(100);
        sl52-> setPageStep(25);
        sl52->setSliderPosition(-75);
        sl53        = new QSlider(Qt::Horizontal, gbMapColor);
        sl53->setMinimum(-75);
        sl53->setMaximum(75);
        sl53-> setPageStep(25);
        sl53->setSliderPosition(0);
        sl54        = new QSlider(Qt::Horizontal, gbMapColor);
        sl54->setMinimum(5);
        sl54->setMaximum(100);
        sl54-> setPageStep(5);
        sl54->setSliderPosition(10);


        QGroupBox *gbMapOpacity = new QGroupBox(w5);
        gbMapOpacity->setTitle (tr("Map opacity") );
        gbMapOpacity->setCheckable(true);
        gbMapOpacity->setChecked(actBack);
        QLabel *lab55       = new QLabel(gbMapOpacity);
        sl55  = new QSlider(Qt::Vertical, gbMapOpacity);
        sl55->setMinimum(0);
        sl55->setMaximum(255);
        sl55-> setPageStep(1);
        QLabel *lab56       = new QLabel(gbMapOpacity);
        sl56        = new QSlider(Qt::Vertical, gbMapOpacity);
        sl56->setMinimum(0);
        sl56->setMaximum(255);
        sl56-> setPageStep(1);


        labTextMin = new QLabel(w5);
        labTextMax = new QLabel(w5);
        labSize    = new QLabel(w5);
        labBG      = new QLabel(w5);
        sbTextMin  = new QSpinBox(w5);
        sbTextMin->setMaximum(16);
        sbTextMin->setMinimum(4);
        sbTextMin->setSingleStep(1);
        sbTextMax  = new QSpinBox( w5 );
        sbTextMax->setMaximum(500);
        sbTextMax->setMinimum(10);
        sbTextMax->setSingleStep(5);
        sbSize     = new QSpinBox(w5);
        sbSize->setMaximum(20);
        sbSize->setMinimum(1);
        sbSize->setSingleStep(1);
        pbBG       = new QPushButton( w5);
        pbOK5      = new QPushButton( w5);
        pbReset5    = new QPushButton(w5);

        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->setSpacing(8);
        QVBoxLayout *vbox51 = new QVBoxLayout(gbMapColor);
        vbox51->setSpacing(8);
        QVBoxLayout *vbox53 = new QVBoxLayout(gbMapOpacity);
        vbox53->setSpacing(8);
        QHBoxLayout *hbox52 = new QHBoxLayout();
        hbox52->setSpacing(8);
        hbox->addWidget(gbMapColor);
        hbox->addWidget(gbMapOpacity);
        topLayout5->addItem(hbox);
        topLayout5->addItem(hbox52);


        //grid5->addItem( vbox51,  0,0,8,7);
        //grid5->addItem( vbox53,  0,9,8,7);
        //grid5->addItem( hbox52,  9,0,1,8);

        vbox51->  setAlignment(Qt::AlignCenter);
        vbox53->  setAlignment(Qt::AlignCenter);
        hbox52->  setAlignment(Qt::AlignCenter);

        connect( sl51, SIGNAL(valueChanged(int)), this, SLOT(palette1Changed(int)) );
        connect( sl52, SIGNAL(valueChanged(int)), this, SLOT(palette2Changed(int)) );
        connect( sl53, SIGNAL(valueChanged(int)), this, SLOT(palette3Changed(int)) );
        connect( sl54, SIGNAL(valueChanged(int)), this, SLOT(fontsizeChanged(int)) );
        connect( sl51, SIGNAL(sliderReleased()),  this, SLOT(sliderReleased()) );
        connect( sl52, SIGNAL(sliderReleased()),  this, SLOT(sliderReleased()) );
        connect( sl53, SIGNAL(sliderReleased()),  this, SLOT(sliderReleased()) );
        connect( sl54, SIGNAL(sliderReleased()),  this, SLOT(sliderReleased()) );
        connect(pbReset5,     SIGNAL(clicked()),  this, SLOT(paletteReset()));

        connect( gbMapOpacity, SIGNAL(toggled(bool)),this,SLOT(mapOpacityTooggled(bool)));
        connect( gbMapOpacity, SIGNAL(toggled(bool)),this,SLOT(sliderReleased()));
        connect( sl55, SIGNAL(valueChanged(int)),  this, SLOT(lightnessChanged(int)));
        connect( sl56, SIGNAL(valueChanged(int)),  this, SLOT(transparentChanged(int)));
        connect( sl55, SIGNAL(sliderReleased()),  this, SLOT(sliderReleased()) );
        connect( sl56, SIGNAL(sliderReleased()),  this, SLOT(sliderReleased()) );


        connect(sbSize,    SIGNAL(valueChanged(int)), this, SLOT(objsizeChanged(int)) );
        connect(sbTextMin, SIGNAL(valueChanged(int)), this, SLOT(textMinChanged(int)) );
        connect(sbTextMax, SIGNAL(valueChanged(int)), this, SLOT(textMaxChanged(int)) );
        connect(pbBG,      SIGNAL(clicked()),         this, SLOT(groundcolorButton()));
        connect(pbOK5,     SIGNAL(clicked()),         this, SLOT(ok5Button()));


        lab51  ->setText(tr("Color"));
        lab52  ->setText(tr("Brightness"));
        lab53  ->setText(tr("Contrast"));
        lab54  ->setText(tr("Font"));
        sl51-> setValue(p1);
        sl52-> setValue(p2);
        sl53-> setValue(p3);
        sl54-> setValue(p4);
        sl51->setTickPosition( QSlider::TicksAbove );
        sl52->setTickPosition( QSlider::TicksAbove );
        sl53->setTickPosition( QSlider::TicksAbove );
        sl54->setTickPosition( QSlider::TicksAbove );
        vbox51 ->addWidget( lab51 );
        vbox51 ->addWidget( sl51 );
        vbox51 ->addWidget( lab52 );
        vbox51 ->addWidget( sl52 );
        vbox51 ->addWidget( lab53 );
        vbox51 ->addWidget( sl53 );
        vbox51 ->addWidget( lab54 );
        vbox51 ->addWidget( sl54 );



        lab55->setText(tr("Tone"));
        lab56->setText(tr("Opacity"));
        sl55-> setValue(p5);
        sl56-> setValue(p6);
        QHBoxLayout *vbox531 = new QHBoxLayout();
        vbox531->setSpacing(5);
        vbox531->  setAlignment(Qt::AlignCenter);
        vbox531->addWidget(lab55);
        vbox531->addWidget(lab56);
        QHBoxLayout *vbox532 = new QHBoxLayout();
        vbox532->setSpacing(24);
        vbox532->  setAlignment(Qt::AlignCenter);
        vbox532->addWidget(sl55);
        vbox532->addWidget(sl56);
        vbox53 ->addItem(vbox531);
        vbox53 ->addItem(vbox532);



        pbReset5->setText( tr("Reset") );
        pbReset5->setEnabled(true);
        pbOK5->setText( tr("Apply") );
        pbOK5->setFixedHeight( pbOK5->sizeHint().height());
        pbOK5-> setEnabled(false);


        labTextMin->setText(tr("Text min:"));
        labTextMax->setText("- "+tr("max:"));
        labSize    ->setText(tr("Min.object:"));
        labBG      ->setText(tr("Background:"));
        sbSize->    setFixedWidth(60);
        //sbTextMin-> setFixedWidth(60);
        //sbTextMax-> setFixedWidth(60);
        pbBG->      setFixedWidth( 40);
        pbBG->setFixedHeight( sbSize->sizeHint().height());
        hbox52-> addWidget( labTextMin );
        hbox52-> addWidget( sbTextMin );
        hbox52-> addWidget( labTextMax );
        hbox52-> addWidget( sbTextMax );
        hbox52-> addWidget( labSize );
        hbox52-> addWidget( sbSize );
        hbox52->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        hbox52-> addWidget( labBG );
        hbox52-> addWidget( pbBG );
        hbox52-> addWidget( pbOK5 );
        hbox52-> addWidget( pbReset5 );
        tab ->addTab( w5, tr("Advanced") );
    }

    if (mTabSetup.fTabCoordinateSystem)
    {
        MCoo* coo = IMap->GetKarta()->m_pObjectCoo;

        QString cooName  = codec->toUnicode(coo->WhatTheNameCoo());
        QString elipsoidName = codec->toUnicode(coo->GetEllipsoidName());


        w6 = new QWidget(tab);
        QGridLayout *mainLayout6 = new QGridLayout (w6);
        mainLayout6->setSpacing(5);

        QGroupBox* groupCurrent = new QGroupBox("Current coordinate system", w6);
        QGridLayout * gridCurrent = new QGridLayout(groupCurrent);
        gridCurrent->setHorizontalSpacing(10);

        QLabel* lblCurrentCoo = new QLabel("Coordinate system:", w6);
        gridCurrent->addWidget(lblCurrentCoo, 0, 0, 1, 3);

        QLabel* lblCurrentElipsoid = new QLabel("Datum", w6);
        gridCurrent->addWidget(lblCurrentElipsoid, 0, 3, 1, 2);

        lineCurrentCoo = new QLineEdit(w6);
        lineCurrentCoo->setReadOnly(true);
        lineCurrentCoo->setText(cooName);
        gridCurrent->addWidget(lineCurrentCoo, 1, 0, 1, 3);

        lineCurrentElipsoid = new QLineEdit(w6);
        lineCurrentElipsoid->setReadOnly(true);
        lineCurrentElipsoid->setText(elipsoidName);
        gridCurrent->addWidget(lineCurrentElipsoid, 1, 3, 1, 2);

        ////
        QGroupBox* groupDestination = new QGroupBox("Destination coordinate system", w6);
        QGridLayout * gridDestination = new QGridLayout(groupDestination);
        gridDestination->setHorizontalSpacing(10);

        QLabel* lblDestinationCoo = new QLabel("Coordinate system:", w6);
        gridDestination->addWidget(lblDestinationCoo, 0, 0, 1, 3);

        QLabel* lblDestinationElipsoid = new QLabel("Datum", w6);
        gridDestination->addWidget(lblDestinationElipsoid, 0, 3, 1, 2);

        comboDestinationCoo = new QComboBox(w6);
        gridDestination->addWidget(comboDestinationCoo, 1, 0, 1, 3);

        const int iWorkCooArraySize = (int)workCooArray.size();
        CooArray = new MPtCoo [iWorkCooArraySize];
        parnames = new MStArray [iWorkCooArraySize];
        pardata = new MStArray [iWorkCooArraySize];
        for(int i = 0 ; i < iWorkCooArraySize; ++i)
        {
            int key = workCooArray[i];
            MString cooType = COO_TYPE_AR[key];

            CooArray[i] = CreateCooByType(cooType);
            if (CooArray[i])
            {
                MString cooName = CooArray[i]->WhatTheNameCoo();
                comboDestinationCoo->addItem(codec->toUnicode(cooName.c_str()));
                CooArray[i]->GetCooParameters(&(parnames[i]), &(pardata[i]));
            }
        }
        comboDestinationElipsoid = new QComboBox(w6);
        gridDestination->addWidget(comboDestinationElipsoid, 1, 3, 1, 2);

        const int iWorkDatumArraySize = (int)workDatumArray.size();
        for(int i = 0 ; i < iWorkDatumArraySize; ++i)
        {
            comboDestinationElipsoid->addItem(codec->toUnicode(ELLIPSOID_AR[workDatumArray[i]].c_str()));

        }
        cooParamTable = new QTableWidget(w6);
        cooParamTable->setColumnCount(2);
        QTableWidgetItem *nameHeaderItem = new QTableWidgetItem(tr("Parameneter name"));
        nameHeaderItem->setTextAlignment(Qt::AlignVCenter);
        cooParamTable->setHorizontalHeaderItem(0, nameHeaderItem);
        QTableWidgetItem *valueHeaderItem = new QTableWidgetItem(tr("Parameneter value"));
        valueHeaderItem->setTextAlignment(Qt::AlignVCenter);
        cooParamTable->setHorizontalHeaderItem(1, valueHeaderItem);


        btnConvertCoo = new QPushButton(w6);
        btnConvertCoo->setText("Apply");
        btnResetCoo = new QPushButton(w6);
        btnResetCoo->setText("Reset");
        mainLayout6->addWidget(groupCurrent, 0, 0, 3, 3);
        mainLayout6->addWidget(groupDestination, 3, 0, 3, 3);
        mainLayout6->addWidget(cooParamTable, 6, 0, 3, 3);
        mainLayout6->addWidget(btnConvertCoo, 0, 4, 1, 1, Qt::AlignTop | Qt::AlignRight);
        mainLayout6->addWidget(btnResetCoo, 1, 4, 9, 1, Qt::AlignTop | Qt::AlignRight);

        //btnConvertCoo->setEnabled(false);
        //btnResetCoo->setEnabled(false);
        comboDestinationCoo->setCurrentIndex(1);
        connect(comboDestinationCoo, SIGNAL(currentIndexChanged(int )), this, SLOT(destCooChanged( int )));
        connect(comboDestinationElipsoid, SIGNAL(currentIndexChanged(int )), this, SLOT(destElipsoidChanged( int )));

        connect(btnConvertCoo, SIGNAL(clicked (bool)), this, SLOT(convertCooClicked( bool )));
        comboDestinationCoo->setCurrentIndex(0);


        tab->addTab( w6, tr("Coordinate system") );
    }
    //---------------------------------------
    if (mTabSetup.fTabInfo)
    {
        w7 = new QWidget( tab );

        QVBoxLayout *topLayout7 = new QVBoxLayout (w7); // 2- border
        topLayout7->setSpacing(5);
        QGridLayout *grid7      = new QGridLayout();
        grid7->setHorizontalSpacing(9);
        grid7->setVerticalSpacing(10);
        topLayout7-> addLayout (grid7, 10);

        box7        = new QListWidget(w7);
        QHBoxLayout *hbox73  = new QHBoxLayout();
        hbox73->setSpacing(8);
        grid7->  addItem( hbox73, 0,0, 9,8 );
        hbox73 ->addWidget( box7 );
        tab ->addTab( w7, tr("Info") );
    }

    // ---------------------
    QVBoxLayout  * mainVbox = new QVBoxLayout(tabDialog);
    QHBoxLayout *mainButtons  = new QHBoxLayout();
    QPushButton * mainOk = new QPushButton(tr("Ok"));
    QPushButton * mainSave = new QPushButton(tr("Save"));
    QPushButton * mainSaveAs = new QPushButton(tr("Save As"));
    QPushButton * mainCancel = new QPushButton(tr("Cancel"));
    connect(mainOk, SIGNAL(clicked()),this, SLOT(OKButtonGlobal()));
    connect(mainSave, SIGNAL(clicked()), this, SLOT(saveButton()));
    connect(mainSaveAs, SIGNAL(clicked()), this, SLOT(saveAsButton()));
    connect(mainCancel, SIGNAL(clicked()),  this, SLOT(cancelButton()));
    mainButtons->addSpacing(200);
    mainButtons->addWidget(mainOk);
    mainButtons->addWidget(mainSave);
    mainButtons->addWidget(mainSaveAs);
    mainButtons->addWidget(mainCancel);
    mainVbox->addWidget(tab);
    mainVbox->addLayout(mainButtons);

#if IS_EDIT_KLS
    ModelInterface* mi = new ModelInterface();
    mi->SetupKls(KLS);
    mi->ForMConfig();
    tab ->addTab(/*new ModelInterface()*/mi,tr("Edit KLS"));
#endif
}

void MConfig::itemSelection()
{
    lab33->clear();
    QList<QTreeWidgetItem*> item_list = list3->selectedItems ();
    if(item_list.size() != 1)
        return;
    else
    {
        QTreeWidgetItem* item  = item_list.at(0);
        currentObject(item, 0);
    }
}

//-------------------------------------
//
void	MConfig::textMinChanged(int val){
    textMin= val;
    if (textMin>textMax) {
        textMin= textMax;
        sbTextMin->   setValue(textMin);//  sbTextMax->   setValue(textMax);
    }
    pbOK5->     setEnabled(true);
}
void	MConfig::textMaxChanged(int val){
    textMax= val;
    if (textMin>textMax) {
        textMin= textMax;
        sbTextMin->   setValue(textMin);//  sbTextMax->   setValue(textMax);
    }
    pbOK5->     setEnabled(true);
}
//-------------------
void	MConfig::objsizeChanged(int val){
    objSize=  val;
    pbOK5->     setEnabled(true);
}
//---------
void MConfig::groundcolorButton()  {
    QColor c11;
    c11.setRgb (cGround.GetR(),   cGround.GetG(),    cGround.GetB());
    QColor c = QColorDialog::getColor(c11, tab);
    if ( c.isValid() )  {
        cGround.SetColor(c.red(), c.green(), c.blue());
        QPixmap  pbBGPix(pbBG->size());
        pbBGPix.fill(c);
        pbBG->setIcon(pbBGPix);
        pbOK5->    setEnabled(true);
    }
}
//---------
void MConfig::simpleClicked() {
    refreshLayerItem();
    fillSubForm();
}
//---------
void MConfig::widthClicked( int w ) {
    wLine=  w;
    refreshLayerItem();
}
//---------
void MConfig::linecolorButton()  {
    QColor c7;
    c7.setRgb (cLine.GetR(),   cLine.GetG(),    cLine.GetB());
    c7.setAlpha(iLineTransparent);
    QColor c = QColorDialog::getColor(c7, tab,  QString(), QColorDialog::ShowAlphaChannel);
    if ( c.isValid() )  {
        cLine.SetColor(c.red(), c.green(), c.blue());
        iLineTransparent = c.alpha();
//        QPixmap  pb7Pix(pb7->size());
//        pb7Pix.fill(c);
//        pb7->setIcon(pb7Pix);
        pb7->setPalette(QPalette(c));
        refreshLayerItem();

    }
}
//---------
void MConfig::fillcolorButton() {
    QColor c8;
    c8.setRgb(cFiller.GetR(),   cFiller.GetG(),    cFiller.GetB());
    c8.setAlpha(iFillerTransparent);
    QColor c = QColorDialog::getColor(c8, tab, QString(), QColorDialog::ShowAlphaChannel);
    if ( c.isValid() )  {
        cFiller.SetColor(c.red(), c.green(), c.blue());
        iFillerTransparent = c.alpha();
       // QPixmap  pb8Pix(pb8->size());
        //pb8Pix.fill(c);
        //pb8->setIcon(pb8Pix);
        pb8->setPalette(QPalette(c));
        refreshLayerItem();
    }
}

//
//
void MConfig::selectObjectsDialog(){

    QSize w10Size(0,0);
    QPoint   pnt;
    if (w10)  {
        w10Size= w10->size();
        pnt=    w10->pos();
        delete w10;
    }

    w10= new QDialog(IMap->GetWidget());
    QFont ff= w10-> font();
    ff.setPointSize(9);
    w10-> setFont(ff);

    QVBoxLayout *topLayout = new QVBoxLayout (w10); // 2- border
    topLayout->setSpacing(10);
    list10       = new QTreeWidget( w10 );
    list10->setSelectionMode(QAbstractItemView::SingleSelection);
    lab10       = new QLabel(w10);
    pbOK10       = new QPushButton( w10 );
    topLayout->addWidget( list10);

    connect(list10,     SIGNAL(itemClicked(QTreeWidgetItem *, int)),this, SLOT(currentObject10(QTreeWidgetItem *, int)));
    connect(pbOK10,     SIGNAL(clicked()), this, SLOT(ok10Button()));

    list10->headerItem()->setText(0, tr("Name"));
    list10->headerItem()->setText(1, tr("On"));
    list10->headerItem()->setText(2, tr("Type"));
    list10->headerItem()->setText(3, tr("ID"));
    list10->headerItem()->setText(4, tr("Total"));
    list10->headerItem()->setText(5, tr("Name complete"));
    list10->hideColumn(TID_UZN);

    pbOK10-> setText( tr("Close") );
    pbOK10-> setFixedHeight( pbOK10->sizeHint().height());
    topLayout->  addWidget( pbOK10); //, 0, Qt::AlignBottom);
    pbOK10-> setEnabled(true);

    if (node1)  w10->  setWindowTitle(node1-> text(1));
    if (w10Size.width() != 0) {
        w10-> move(pnt);
        w10-> resize(w10Size);
    }

    fillListForm10();
    w10-> exec();
}

//--------------------------------------------
//
void MConfig::refreshForm5() {
    sl51->setValue(p1);
    sl52->setValue(p2);
    sl53->setValue(p3);
    sl54->setValue(p4);
    sbTextMin->setValue(textMin);
    sbTextMax->setValue(textMax);
    sbSize->setValue(objSize);

    //MColor col = IMap->GetKarta()->GetPaperColor();
    QColor qcol;
    qcol.setRgb(cGround.GetR(), cGround.GetG(),  cGround.GetB());
    QPixmap  pbBGPix(pbBG->size());
    pbBGPix.fill(qcol);
    pbBG->setIcon(pbBGPix);
}
//
void MConfig::palette1Changed( int val ) {
    p1= val;
    karta= IMap->GetKarta();
    if (! karta) return;
    karta->m_DH = val;
}
//
void MConfig::palette2Changed( int val ) {
    p2= val;
    karta= IMap-> GetKarta();
    if (! karta) return;
    karta->m_DL = val;
}
//
void MConfig::palette3Changed( int val ) {
    p3= val;
    karta= IMap-> GetKarta();
    if (! karta) return;
    karta->m_DS = val;
}
void MConfig::fontsizeChanged( int val ) {
    p4= val;
    textRatio = (float)val/10.0;
}
//
void MConfig::sliderReleased( ) {
    karta= IMap-> GetKarta();
    if (! karta) return;
    drawMap();
    emit  drawClicked();
}

//
void MConfig::mapOpacityTooggled(bool enable) 
{
    actBack = enable;
    karta= IMap-> GetKarta();
    if (! karta) return;
    karta->m_actBackEnable = enable;
}

//
void MConfig::lightnessChanged(int val)
{
    p5= val;
    karta= IMap-> GetKarta();
    if (! karta) return;
    karta->m_actBackLight = val;
}
//
void MConfig::transparentChanged(int val)
{
    p6= val;
    karta= IMap-> GetKarta();
    if (! karta) return;
    karta->m_actBackTransp = val;
}

void MConfig::paletteReset() {
    karta= IMap-> GetKarta();
    if (! karta) return;

    p1=  p2=  p3= 0;
    p4=  10;
    p5=255;
    p6=180;
    sl51-> setValue(p1);
    sl52-> setValue(p2);
    sl53-> setValue(p3);
    sl54-> setValue(p4);
    sl55-> setValue(p5);
    sl56-> setValue(p6);
    karta->m_DH = p1;
    karta->m_DL = p2;
    karta->m_DS = p3;
    karta->m_actBackLight= p5;
    karta->m_actBackTransp = p6;
    textRatio = (float)p4/10.0;

    sbSize->setValue(4);
    sbTextMin->setValue(4);
    sbTextMax->setValue(100);

    karta->m_papercolorkod = karta->m_pKls->m_klscolorkod;
    cGround= IMap-> GetBackgroundColor(false);
    refreshForm5();
    drawMap();
}
//
void MConfig::cancel30Button(){
    currentZnak="";
    if (w30) w30->hide();
}
void MConfig::ok30Button(){
    if (w30) w30->hide();
}
//---------------------------------------------
//
QString MConfig::selectZnakDialog(){

    QSize w30Size(0,0);
    QPoint   pnt;
    if (w30)  {
        w30Size= w30->size();
        pnt=    w30->pos();
        w30->~QDialog();
    }

    w30= new QDialog(IMap->GetWidget());
    QFont ff= w30-> font();
    ff.setPointSize(9);
    w30-> setFont(ff);

    QVBoxLayout *topLayout = new QVBoxLayout (w30); // 2- border
    topLayout->setSpacing(5);
    QGridLayout *grid      = new QGridLayout();
    grid->setHorizontalSpacing(9);
    grid->setVerticalSpacing(10);
    topLayout-> addLayout (grid, 10);

    //==========================

    list30       = new QTreeWidget( w30 );   // list->  setSelectionMode(QTreeWidget::Single);
    labMap       = new QLabel(w30);
    pbOK30       = new QPushButton( w30);
    pbCancel30   = new QPushButton( w30);

    grid->addWidget( list30 ,0,0,8,6 );
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(8);
    QHBoxLayout *hbox  = new QHBoxLayout();
    hbox->setSpacing(8);
    grid->addItem( vbox,    0,7, 9,1 );
    grid->addItem( hbox,    9,0, 0,6 );
    vbox->  setAlignment(Qt::AlignCenter);

    connect(list30,  SIGNAL(clicked(QTreeWidgetItem *)),this, SLOT(currentObject30(QTreeWidgetItem *)));
    connect(pbOK30,  SIGNAL(clicked()), this, SLOT(ok30Button()));
    connect(pbCancel30,  SIGNAL(clicked()), this, SLOT(cancel30Button()));

    QStringList columns;
    columns <<tr("Name")
           <<tr("Type")
          <<tr("ID");
    list30->setHeaderLabels(columns);
    pbOK30->setText( tr("OK") );
    pbOK30->setFixedHeight( pbOK30->sizeHint().height());
    hbox-> addWidget( pbOK30 );
    pbOK30->  setEnabled(true);
    pbCancel30->setText( tr("Cancel") );
    pbCancel30->setFixedHeight( pbCancel30->sizeHint().height());
    hbox-> addWidget( pbCancel30 );
    pbCancel30->  setEnabled(true);

    vbox->   addWidget( labMap );
    labMap-> setText("MAP SYMBOL");
    labMap-> setEnabled(true);
    labMap-> setFixedHeight(labMap->width());
    labMap-> setFixedWidth (labMap->width());

    if (! klsFile.isEmpty())  {
        mki30   =  new MKartaInterface(labMap);
        MRect  rct = IMap-> GetKarta()->GetGeoBounds();//    MRect  rct = IMap->GetGeoBounds();
        MRect  rct0;
        rct0.left  = (rct.right + rct.left)/2.0 ;
        rct0.right =  rct0.left + 3240.05;    //2160.03;
        rct0.bottom= (rct.bottom + rct.top)/2.0 ;
        rct0.top   =  rct0.bottom + 3240.05;  //2160.03;
        MString kls(klsFile. toLatin1());
        mki30->  SetKlassifikator(kls);
        mki30->  GetKarta()->SetGeoBounds(rct0);
        mki30->  ZoomTo(rct0);
        MColor col(245, 245, 245);
        mki30->  SetBackgroundColor(col);
        mki30->  SetWidget( labMap );
        mki30->  DrawMap();
    }

    if (w30Size.width() != 0) {
        w30-> move(pnt);
        w30-> resize(w30Size);
    }

    fillListForm30();

    currentZnak= "";
    w30-> exec();

    if (mki30) delete mki30;

    return currentZnak;
}

//--------------------------------------------
//
void MConfig::currentObject30(QTreeWidgetItem *nd)
{
    labMap-> setAlignment(Qt::AlignCenter);
    labMap-> setText("");
    labMap-> hide();
    if (! nd)    return;
    if (! mki30) return;
    if (! KLS)   return;

    int dense= IMap->GetActPolygonDense();
    IMap->SetActPolygonDense(Qt::SolidPattern);

    currentZnak= nd-> text(2);
    QByteArray ccc= codec->fromUnicode(currentZnak);
    MString kkod= ccc.data();
    MString  true_kod;
    MZnak* znak =  KLS-> GetZnak(kkod, true_kod);  //cout << "  znak="<< currentZnak <<endl;

    if (znak)
    {
        MYzArray* pYazArray = znak->GetYazArray();
        if(pYazArray)                {
            MYaz* pYaz;
            for(int s=0; s<pYazArray->GetSize(); s++){
                pYaz = znak->GetYaz(s);

                if(pYaz->GetType()==F_DOT)
                {

                    MSzArray* szar = ((MDotYaz*)pYaz->m_object)->m_tochka;
                    for(int kk=0; kk<szar->GetSize(); kk++){
                        MSrez* pSrez = szar->GetAt(kk);
                        if(pSrez->GetType()==IDT_SHRIFT){
                            MSymbol* sym = &((MShriftSrez*)pSrez)->m_tochka;
                            QFont font(QString(sym->logfont.lfFaceName), 32);
                            labMap-> setFont(font);
                            QColor col(255, 255, 255);

                            QPalette paletteB;
                            paletteB.setColor(labMap->backgroundRole(), col);
                            labMap->setPalette(paletteB);
                            unsigned int color=sym->colorkod;
                            MColor colorValue;
                            KLS->GetDictColorValue(color,colorValue);
                            QColor clr(colorValue.R, colorValue.G, colorValue.B);
                            QPalette paletteF;
                            paletteF.setColor(labMap->foregroundRole(), clr);
                            labMap->setPalette(paletteF);
                            labMap-> setText(QString(sym->str) );
                            labMap-> show();
                        }
                    }


                }

                if(pYaz->GetType()==F_LIN)
                {
                    if (mki30->FindObjectAtLayers(1)) { mki30-> SetWorkingActLayer(1); mki30-> DeleteObject(1); }
                    else                            { mki30-> AddActLayer(1);  mki30-> SetWorkingActLayer(1); }
                    mki30-> SetWorkingActLayer(1);
                    mki30-> AddObject(1);
                    mki30-> SetWorkingObject(1);

                    MRect  rct = mki30->GetGeoBounds();
                    rct.NormalizeGeoRect();
                    mki30-> ZoomTo(rct);
                    double hr= (rct.right -rct.left)/12.0;
                    double vr=  (rct.top -rct.bottom)/12.0;
                    MPoint ptg1(rct.left+ hr , rct.bottom + vr);
                    mki30-> AddPoint(ptg1);
                    MPoint ptg2(rct.right- hr , rct.top- vr );
                    mki30-> AddPoint(ptg2);
                    mki30-> SetZnak(kkod);
                    mki30-> SetWidget(labMap);
                    mki30-> DrawMap(false);
                    QPixmap *pxm = mki30->GetPixmapAct();
                    labMap->setPixmap(*pxm);
                    labMap-> show();
                }

                if(pYaz->GetType()==F_PLO)
                {
                    if (mki30->FindObjectAtLayers(1)) {mki30-> SetWorkingActLayer(1); mki30->DeleteObject(1); }
                    else  { mki30-> AddActLayer(1);  mki30-> SetWorkingActLayer(1); }
                    mki30-> SetWorkingActLayer(1);
                    mki30-> AddObject(1);
                    mki30-> SetWorkingObject(1);

                    MRect  rct = mki30->GetGeoBounds();
                    rct.NormalizeGeoRect();
                    mki30-> ZoomTo(rct);
                    double hr= (rct.right -rct.left)/12.0;
                    double vr=  (rct.top -rct.bottom)/12.0;
                    MPoint ptg1(rct.left + hr, rct.bottom + vr);
                    mki30-> AddPoint(ptg1);
                    MPoint ptg2(rct.left + hr, rct.top - vr);
                    mki30-> AddPoint(ptg2);
                    MPoint ptg3(rct.right - hr, rct.top - vr);
                    mki30-> AddPoint(ptg3);
                    MPoint ptg4(rct.right - hr, rct.bottom + vr);
                    mki30-> AddPoint(ptg4);
                    mki30-> SetZnak(kkod);
                    mki30-> SetWidget(labMap);
                    mki30-> DrawMap(false);
                    QPixmap *pxm = mki30->GetPixmapAct();
                    labMap->setPixmap(*pxm);
                    labMap-> show();
                }
            }
        }
    }

    IMap->SetActPolygonDense(dense);
}

// ------------------------------
//
void  MConfig::fillListForm30() {

    QString pss0, pss1, pss2;
    if (! IMap )    return;
    karta= IMap-> GetKarta();
    if (! karta )   return;
    if (! KLS )     return;
    if (! list30 )  return;
    list30-> clear();
    QPixmap folderPix(x_folder);
    QIcon folderIcon(folderPix);

    QByteArray  ccc, pcc;
    QString     sss, pss;
    MTermin  *trm= KLS-> GetTermin();
    ccc=  trm->GetKod().c_str();
    pss0= codec->toUnicode(ccc);

    QTreeWidgetItem *lv1 = new QTreeWidgetItem(list30);
    lv1-> setText(0, tr("Symbols of Ground Situation")  );
    lv1-> setIcon(0, folderIcon );
    lv1-> setExpanded( true );

    MZsArray* znsistemi = KLS-> GetZsArray();
    for(int i=0; i<znsistemi->GetSize(); i++){
        MZnakSistema* zs = znsistemi->GetAt(i);
        MRlArray* razdeli = zs->GetRazdelArray();
        trm=   zs->  GetTermin();
        ccc=  trm-> GetKod().c_str();
        sss=  codec->toUnicode(ccc);

        pss1= pss0+" "+ sss;
        for(int j=0; j<razdeli->GetSize(); j++){
            MRazdel* razdel = razdeli->GetAt(j);
            MZnArray* znaki = razdel->GetZnakArray();
            trm=   razdel->  GetTermin();
            ccc=  trm-> GetKod().c_str();
            sss=  codec->toUnicode(ccc);
            pss= pss1+" "+ sss;

            if (
                    (pss.indexOf("5000 NO")>0)
                    ||
                    (pss.indexOf("5000 FR")>0)
                    ||
                    (pss.indexOf("5000 EN")>0)
                    )
            {
                QTreeWidgetItem *lv2 = new QTreeWidgetItem(lv1);
                ccc=  trm->GetShortname().c_str();
                lv2-> setText(0,  codec->toUnicode(ccc)  );
                lv2-> setText(2, pss  );

                for(int k=0; k<znaki->GetSize(); k++){
                    MZnak* znak = znaki->GetAt(k);

                    pss=  lv2->  text(2);
                    trm=  znak-> GetTermin();
                    ccc=  trm->  GetKod().c_str();
                    sss=  codec->toUnicode(ccc);

                    QTreeWidgetItem *lv3 = new QTreeWidgetItem(lv2);
                    lv3-> setText(2, pss+ " " +sss  );
                    ccc=  trm->GetShortname().c_str();
                    lv2-> setIcon(0, folderIcon );
                    lv3-> setText(0,  codec->toUnicode(ccc)  );

                    MYzArray* pYazArray = znak->GetYazArray();
                    if(!pYazArray) continue;
                    MYaz* pYaz;
                    for(int s=0; s<pYazArray->GetSize(); s++){
                        pYaz = znak->GetYaz(s);
                        if(pYaz->GetType()==F_DOT)    lv3-> setText(1,  tr("Dot")   );
                        if(pYaz->GetType()==F_LIN)    lv3-> setText(1,  tr("Line")  );
                        if(pYaz->GetType()==F_PLO)    lv3-> setText(1,  tr("Area")  );
                        if(pYaz->GetType()==IDL_PLS)  lv3-> setText(1,  "strip" );
                    }


                }//end for(int k=0; k<znaki->GetSize(); k++)
            }
        }//end for(int j=0; j<razdeli->GetSize(); j++)

    }//end for(int i=0; i<znsistemi->GetSize(); i++)

    if (mki30)  mki30->DrawMap(true);
}


//==================================
MString MConfig::GetPartOfStringExt(int Source,int Dest,MString BaseString)
{
    MString Result = "";
    if((unsigned)Dest>BaseString.GetLength()-1)
        return Result;
    for(int i= Source;i<=Dest;i++)
        Result += BaseString.GetAt(i);
    return Result;
}

//-----------------------------------
bool MConfig::SetObjectCoo(MCoo* pCoo, MKarta * p_karta)
{
    if(!pCoo) return false;
    MKarta * _karta = p_karta != nullptr ? p_karta : karta;
    MCoo* pNewCoo = CreateCooByType(pCoo->WhatTheCoo());
    if(pNewCoo)
    {
        pNewCoo->Copy(pCoo);
        delete _karta->m_pObjectCoo;
        _karta->m_pObjectCoo = pNewCoo;
    }
    else	return false;

    return true;
}
//-----------------------------------
//@@@@@
void MConfig::ConvertToCoo(MCoo* coo, QString elipsoid_name, int paramCount, QString* names, QString* data)
{
    MCoo* newCoo = CreateCooByType(coo->WhatTheCoo());
    QByteArray tmp = codec->fromUnicode(elipsoid_name);
    MString ellipsoid = MString(tmp.data());
    newCoo->SetStandartEllipsoid(ellipsoid);
    MStArray arrNames;
    MStArray arrData;
    MString str;
    for(int i = 0; i < paramCount; ++i)
    {
        str = codec->fromUnicode(names[i]).data();
        arrNames.AddTail(&str);
        str = codec->fromUnicode(data[i]).data();
        arrData.AddTail(&str);
    }

    if (paramCount > 0)
    {
        newCoo->SetCooParameters(&arrNames, &arrData);
    }

    MRect zoomRect = IMap->m_zoombounds;
    MPoint lt(zoomRect.left, zoomRect.top);
    MPoint rb(zoomRect.right, zoomRect.bottom);
    lt = IMap->ConvertPoint(lt,false);
    rb = IMap->ConvertPoint(rb,false);
    zoomRect = MRect(lt,rb);
    MPoint boundsLT(karta->m_geobounds.left, karta->m_geobounds.top);
    MPoint boundsRB(karta->m_geobounds.right, karta->m_geobounds.bottom);
    boundsLT = IMap->ConvertPoint(boundsLT,false);
    boundsRB = IMap->ConvertPoint(boundsRB,false);
    MPoint currLT, currRB;
    double h;
    newCoo->FromBLH(boundsLT,0,currLT, h);
    newCoo->FromBLH(boundsRB,0,currRB, h);
    IMap->GetKarta()->ConvertKartaToCoo(newCoo);
    IMap->GetKarta()->m_geobounds.left = currLT.x;
    IMap->GetKarta()->m_geobounds.bottom = currRB.y;
    IMap->GetKarta()->m_geobounds.right = currRB.x;
    IMap->GetKarta()->m_geobounds.top = currLT.y;
    IMap->ZoomTo(zoomRect);
}
void MConfig::ConvertToCoo(MCoo* pCoo)
{
    if(pCoo==NULL) return;

    IMap->GetKarta()->ConvertKartaToCoo(pCoo);



}

int MConfig::GetTreeItemDepth( QTreeWidgetItem *item)
{
    int depth=-1;
    if(!item)return depth;
    QTreeWidgetItem *lUp = item;
    while(lUp)
    {
        lUp =  lUp->parent();
        depth++;
    }
    return depth;
}


//****************************************************************************************
//MAKaddOn
bool   MConfig::readCmp(QString cmpName, MTable& m_CompareTable, MTable& m_AtrValueSooTable, MInArray& m_AtrSooTable, MInArray& m_OrdAtrList, int& CmpFileVer)
//****************************************************************************************
{
    MString   ReadString, TempStr;
    int       i, inte;

    if (!IMap)  return false;
    karta = IMap-> GetKarta();
    if (!karta) return false;
    //karta->ClearAll();
    //=====================================
    m_CompareTable.AddField(ID_INA,"KEY",0);
    m_CompareTable.SetKeyField(0);
    m_CompareTable.AddField(ID_STA,"OOF_CODE",1);
    m_CompareTable.AddField(ID_STA,"OOF_NAME",2);
    m_CompareTable.AddField(ID_STA,"KLS_CODE",3);
    m_CompareTable.AddField(ID_STA,"KLS_NAME",4);

    // ---> Chtenie Tablicy sootv Znakov
    QFile  cmpFile(cmpName);
    if (!cmpFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return false;

    char line[1000];
    cmpFile.readLine(line, sizeof(line)-1);
    CmpFileVer = atoi(line);
    line[strlen(line)-1]= 0x00;
    if(CmpFileVer!=1) // esli CmpFileVer ne 1 i ne 2
    {
        if(CmpFileVer!=2){
            cmpFile.close();
            return false;
        }
    }

    cmpFile.readLine(line, sizeof(line)-1);//Signatura
    line[strlen(line)-1]= 0x00;
    cmpFile.readLine(line, sizeof(line)-1);//Polnoe nazvanie tablicy (Kommentarij)
    line[strlen(line)-1]= 0x00;
    //Schityvaem path k Klassifikatoru
    cmpFile.readLine(line, sizeof(line)-1);
    line[strlen(line)-1]= 0x00;
    //Schityvaem Kod Klassifikatora
    cmpFile.readLine(line, sizeof(line)-1);
    line[strlen(line)-1]= 0x00;
    //Schityvaem Polnoe nazvanie Klassifikatora
    cmpFile.readLine(line, sizeof(line)-1);

    KLS = IMap-> GetKarta()-> m_pKls;
    MString sLine = karta->m_pKls->GetTermin()->GetKod();
    //=============================================================
    //Chtenie tablicy sootv atributov ->
    //=============================================================
    if (CmpFileVer == 2)
    {
        m_AtrSooTable.ClearAll();
        //Propuskaem 5 strok
        for (i=0;i<5;i++)
            cmpFile.readLine(line, sizeof(line)-1);

        line[strlen(line)-1]= 0x00;
        ReadString  = line;
        if (ReadString == "COMPARE TABLE ATRIBUTES (SXF!UTP)")
        {
            cmpFile.readLine(line, sizeof(line)-1);
            line[strlen(line)-1]= 0x00;
            ReadString  = line;
            while (ReadString.Left(15)!= "===============")
            {
                inte = 0;
                i = ReadString.Find('!');
                if (i !=-1){
                 TempStr = ReadString.Left(i);
                 inte = atoi(TempStr);
                 m_AtrSooTable.AddTail(&inte);
                 ReadString.Delete(0,i+1);

                 inte = 0;
                 i = ReadString.Find('\t');
                 if (i !=-1){
                     TempStr = ReadString.Left(i);
                     inte = atoi(TempStr);
                    }
                 m_AtrSooTable.AddTail(&inte);
                }
                cmpFile.readLine(line, sizeof(line)-1);
                line[strlen(line)-1]= 0x00;
                ReadString  = line;
            }
        }//if (ReadString == "ТАБЛ...
        //propuskaem stroku-Nazvanie
        cmpFile.readLine(line, sizeof(line)-1);
        line[strlen(line)-1]= 0x00;
        ReadString  = line;
    }//if (CmpFileVer ==2)
    //<-
    m_AtrValueSooTable.AddField(ID_INA,"KEY",0);
    m_AtrValueSooTable.SetKeyField(0);
    m_AtrValueSooTable.AddField(ID_STA,"OOF_CODE",1);
    m_AtrValueSooTable.AddField(ID_STA,"OOF_VALUE",2);
    m_OrdAtrList.ClearAll();

        if (CmpFileVer == 2)
        {
            if (ReadString == "SEMANTIC SYMBOL TABLE VALUES")
            {
                cmpFile.readLine(line, sizeof(line)-1);
                line[strlen(line)-1]= 0x00;
                ReadString  = line;
                while (ReadString.Left(15)!= "===============")
                {
                    int rKey = m_AtrValueSooTable.CreateRecord();
                    inte = 0;
                    i = ReadString.Find('_');
                    if (i !=-1)
                    {
                        TempStr = ReadString.Left(i);
                        inte = atoi(TempStr);
                        if (m_OrdAtrList.Size !=0)
                        {
                            if(inte != *m_OrdAtrList.GetAt(m_OrdAtrList.Size-1))
                                m_OrdAtrList.AddTail(&inte);
                        }
                        else m_OrdAtrList.AddTail(&inte);
                    }

                    i = ReadString.Find('\t');
                    if (i !=-1){

                        TempStr = ReadString.Left(i);
                        MString tmp_str(TempStr);
                        m_AtrValueSooTable.SetSimpleCell(rKey,1,&tmp_str);
                        ReadString.Delete(0,i+1);
                        tmp_str = ReadString;
                        m_AtrValueSooTable.SetSimpleCell(rKey,2,&tmp_str);
                    }
                    cmpFile.readLine(line, sizeof(line)-1);
                    line[strlen(line)-1]= 0x00;
                    ReadString  = line;
                }
                cmpFile.readLine(line, sizeof(line)-1);
                line[strlen(line)-1]= 0x00;
                ReadString  = line;
            }//if (ReadString == "ТАБЛ...
        }//if (iVerCMP ==2)

    //Chtenie sootv Znakov
    bool ErrorOccure= false;
    int j=6;
    while(cmpFile.readLine(line, sizeof(line)-1) >0 )
    {
        line[strlen(line)-1]= 0x00;
        int rKey = m_CompareTable.CreateRecord();
        MString s0= line;
        m_CompareTable.SetSimpleCell(rKey,1,&s0);
        //---------------------------
        if (cmpFile.readLine(line, sizeof(line)-1) <0 )  ErrorOccure = true;
        line[strlen(line)-1]= 0x00;   s0= line;
        m_CompareTable.SetSimpleCell(rKey,2,&s0);
        //---------------------------
        if (cmpFile.readLine(line, sizeof(line)-1) <0 )  ErrorOccure = true;
        line[strlen(line)-1]= 0x00;   s0=line;
        m_CompareTable.SetSimpleCell(rKey,3,&s0);
        //---------------------------
        if (cmpFile.readLine(line, sizeof(line)-1) <0 )  ErrorOccure = true;
        line[strlen(line)-1]= 0x00;   s0= line;
        m_CompareTable.SetSimpleCell(rKey,4,&s0);
        //---- chitaem stroku-razdelitel, esli CmpFileVer=2 ------------------
        if (CmpFileVer == 2){
            if (cmpFile.readLine(line, sizeof(line)-1) <0 )  ErrorOccure = true;
        }

        if(ErrorOccure)
        {
            cmpFile.close();
            return false;
        }
        j += 4;
    }//EndOf - while(cmpFile.readLine(line, sizeof(line)-1) >0 )
    cmpFile.close();
    return true;
}

//****************************************************************************************
//MAKaddOn
bool MConfig::readCmf(QString cmfName, MTable &m_CompareFontTable)
//****************************************************************************************
{
    bool ErrorOccure= false;
    int j=3;
    char line[1000];

    //Sozdanie i Chtenie Tablicy sootv pocherkov
    m_CompareFontTable.AddField(ID_INA,"KEY",0);
    m_CompareFontTable.SetKeyField(0);
    m_CompareFontTable.AddField(ID_STA,"SXF_CODE",1);
    m_CompareFontTable.AddField(ID_STA,"FONT_NAME",2);
    m_CompareFontTable.AddField(ID_STA,"FONT_COLOR",3);

    QFile  cmfFile(cmfName);
    if (!cmfFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
        return false;

    cmfFile.readLine(line, sizeof(line)-1);
    if(atoi(line)!=1)
     return false;

    cmfFile.readLine(line, sizeof(line)-1);
    cmfFile.readLine(line, sizeof(line)-1);

    while(cmfFile.readLine(line, sizeof(line)-1) >0 )
    {
        line[strlen(line)-1]= 0x00;
        MString s0= line;
        int rKey = m_CompareFontTable.CreateRecord();

        m_CompareFontTable.SetSimpleCell(rKey,1,&s0);
        if (cmfFile.readLine(line, sizeof(line)-1) <0 )  ErrorOccure = true;
        line[strlen(line)-1]= 0x00;     s0=line;
        m_CompareFontTable.SetSimpleCell(rKey,2,&s0);
        if (cmfFile.readLine(line, sizeof(line)-1) <0 )  ErrorOccure = true;
        line[strlen(line)-1]= 0x00;     s0= line;
        m_CompareFontTable.SetSimpleCell(rKey,3,&s0);

        if(ErrorOccure)
        {
            cmfFile.close();
            return false;
        }

        j +=3;
    }//EndOf - while(cmfFile.readLine(line, sizeof(line)-1) >0 )

    cmfFile.close();
    //<< Konets Chtenija Tablicy sootv pocherkov
    return true;
}

//****************************************************************************************
//конвертация  sxf
bool MConfig::loadSXF(QString sxfName, MTable* m_CompareTable, MTable* m_AtrValueSooTable, MInArray* m_AtrSooTable, MInArray* m_OrdAtrList,
                MTable* m_CompareFontTable, int CmpFileVer, double centralMeridian, MKarta *p_karta)
//****************************************************************************************
{
    QString bartext = "";
    int meridian;

    float     nadpRatio = 0.85;
    int       Mashtab;
    bool      prAsGeneral= true;

    MString   sxfFile= sxfName.toStdString();
    MString   filename = sxfFile;

    //======================================
    //TR: npicmynaeM K chTeHIju SXF
    //EN: Begin SXF reading
    //======================================
    FILE* FileSxf = fopen(filename, "rb");
    if(!FileSxf) return false;

    int percent=0;
    fseek(FileSxf, 0, SEEK_END);
    int pos= ftell(FileSxf);
    int dl_sxf = pos;//RU:длина файла //EN: file length

    //заводим ...
    //TR:BPEMEHHO!!
    MTopobject  *pTpo;
    MLayer      *pLay;
    MLayer* m_Lastlayer = NULL;

    MLayer* m_Generallayer = new MLayer();
    QString str=  tr("General Layer");//"Обобщенный слой");
    m_Generallayer->m_name = codec->fromUnicode(str);
    p_karta-> AddLayer(m_Generallayer);				//  vipa
    //RU: Создаем слой для записи в него надписей
    //TR: Sozdaem Sloj dlia zapisi v nego Nadpisej
    MLayer* m_Nadplayer = new MLayer();
    p_karta-> AddLayer(m_Nadplayer);  // Добавляем слой надписей в массив слоев
    str = tr("Toponyms Layer");//"Слой надписей");
    m_Nadplayer->m_name = codec->fromUnicode(str);//str.toAscii();//codec->toUnicode(str);
    //TR: Sozdaem Sloj dlia Reljefa
    MReliefLayer* pReliefLayer = NULL;//new MReliefLayer();

    //TR: Spisok objectov-simvolov
    MObjectList m_SymbolList;	// EN: List of symbol objects

    // SXF version
    short edition;
    fseek(FileSxf, 8, SEEK_SET);
    fread ((char *)&edition, 1, sizeof(edition), FileSxf);
    fseek(FileSxf, 0, SEEK_SET);

    if (edition == EDITION3 )
    {
    // -------------------  VERSION 3.0  ( scale 1:500000 )-------------------
        // Чтение паспорта карты
        SXFPASP sxf_pasp;
        if (fread((char *)(&sxf_pasp), 1, 256, FileSxf) <256)
            return false;

        // ====================
        MString m_nomenclature;

        // Условное название
        // Код номенклатуры
        dos2win((char*)sxf_pasp.SheetName);
        p_karta->m_name.Format("%s %s",sxf_pasp.SheetName,sxf_pasp.Nomenclature);
        // знаменатель масштаба
        p_karta->m_mastab.Format("1:%d",sxf_pasp.Scale);
                // Обобщенный тип карты
                // B  L  юго-западного угла
                // B  L  северо-западного угла
                // B  L  северо-восточного угла
                // B  L  юго-восточного угла
                // X  Y  юго-западного угла
                // X  Y  северо-западного угла
                // X  Y  северо-восточного угла
                // X  Y  юго-восточного угла
                // Система координат
                // Система высот
                // Проекция карты
    Mashtab = sxf_pasp.Scale;

    // Считываем из паспорта и формируем рамку листа в географических координатах
    pTpo = new MTopobject();
    // Запись в обычный слой
    pTpo->m_pLay = m_Generallayer;
    // Добавим объект в список объектов слоя
    m_Generallayer->m_oblist.push_back((MObject*)pTpo);
    // привязываем к служебному классификатору
    pTpo->ConnectToServKls(F_LIN);
    // Записываем координаты рамки
    MPoint NWPnt;
    int ix, iy;
  memcpy(&ix,sxf_pasp.YNorthWestCoordinate, sizeof(ix));
  memcpy(&iy,sxf_pasp.XNorthWestCoordinate, sizeof(iy));
  NWPnt = MPoint((double)ix/10.0, (double)iy/10.0);
    pTpo->AddPoint(&NWPnt); // северо-западного угла
    MPoint NEPnt;
  memcpy(&ix,sxf_pasp.YNorthEastCoordinate, sizeof(ix));
  memcpy(&iy,sxf_pasp.XNorthEastCoordinate, sizeof(iy));
  NEPnt = MPoint((double)ix/10.0, (double)iy/10.0);
    pTpo->AddPoint(&NEPnt); // северо-восточного угла
    MPoint SEPnt;
  memcpy(&ix,sxf_pasp.YSouthEastCoordinate, sizeof(ix));
  memcpy(&iy,sxf_pasp.XSouthEastCoordinate, sizeof(iy));
  SEPnt = MPoint((double)ix/10.0, (double)iy/10.0);
    pTpo->AddPoint(&SEPnt); // юго-восточного угла
    MPoint SWPnt;
  memcpy(&ix,sxf_pasp.YSouthWestCoordinate, sizeof(ix));
  memcpy(&iy,sxf_pasp.XSouthWestCoordinate, sizeof(iy));
  SWPnt = MPoint((double)ix/10.0, (double)iy/10.0);
    pTpo->AddPoint(&SWPnt); // юго-западного угла
    pTpo->AddPoint(&NWPnt); // северо-западного угла
    // перевычислим охват
    pTpo->RecalcBounds();

    MRect GeoBounds= pTpo->m_bounds;  // pTpo->m_bounds.NormalizeGeoRect();
  meridian= (int)(pTpo->m_bounds.CenterX()/1000000)*6 - 3;

    // Считываем из паспорта и формируем рамку листа в логических координатах устройства
    MPoint NWPntR;
    NWPntR = MPoint(sxf_pasp.YNorthWestLocation,sxf_pasp.XNorthWestLocation);

    MPoint NEPntR;
    NEPntR = MPoint(sxf_pasp.YNorthEastLocation,sxf_pasp.XNorthEastLocation);

    MPoint SEPntR;
    SEPntR = MPoint(sxf_pasp.YSouthEastLocation,sxf_pasp.XSouthEastLocation);

    MPoint SWPntR;
    SWPntR = MPoint(sxf_pasp.YSouthWestLocation,sxf_pasp.XSouthWestLocation);

    //CalcProektPreobrBy4pt(NWPntR,NEPntR,SEPntR,SWPntR,NWPnt,NEPnt,SEPnt,SWPnt);
    // Получаем линейное преобразование логических координатах устройства
    // в географические координаты
    if (DLINASG(NWPntR,SEPntR)==0.0)
        CalcLinPreobrBy3pt(NWPnt,NEPnt,SEPnt,NWPnt,NEPnt,SEPnt);
    else
        CalcLinPreobrBy3pt(NWPntR,NEPntR,SEPntR,NWPnt,NEPnt,SEPnt);

    // Чтение дескриптора карты
    fseek(FileSxf, sxf_pasp.PassportLength, SEEK_SET);
    SXFDESCRIPTOR sxf_descriptor;
    if (fread(&sxf_descriptor, 1, 44, FileSxf) < 44)
        return false;

    // Чтение секции данных
    fseek(FileSxf,  sxf_pasp.PassportLength + sxf_descriptor.DescriptorLength, SEEK_SET );
}
else
// -------------------  VERSION 4.0  ( scale 1:200000 )   -------------------
{
    SXFPASP_4_0 sxf_pasp;
    if (fread(&sxf_pasp, 1, 400,FileSxf) <400) {
        //InternalMessage("Сбой чтения паспорта SXF-файла.");
        return false;
    }

    // Переменные, которых не хватает в MDoc'e по сравнению с паспортоь SXF
    MString m_nomenclature;

    // Условное название
    dos2win((char*)sxf_pasp.SheetName);
    // Код номенклатуры
    p_karta->m_name.Format("%s %s",sxf_pasp.SheetName,sxf_pasp.Nomenclature);
    // знаменатель масштаба
    p_karta->m_mastab.Format("1:%d",sxf_pasp.Scale);
                // Обобщенный тип карты
                // B  L  юго-западного угла
                // B  L  северо-западного угла
                // B  L  северо-восточного угла
                // B  L  юго-восточного угла
                // X  Y  юго-западного угла
                // X  Y  северо-западного угла
                // X  Y  северо-восточного угла
                // X  Y  юго-восточного угла
                // Система координат
                // Система высот
                // Проекция карты
  Mashtab = sxf_pasp.Scale;

    // Считываем из паспорта и формируем рамку листа в географических координатах
    pTpo = new MTopobject();
    // Запись в обычный слой
    pTpo->m_pLay = m_Generallayer;
    // Добавим объект в список объектов слоя
    m_Generallayer->m_oblist.push_back((MObject*)pTpo);
    // привязываем к служебному классификатору
    pTpo->ConnectToServKls(F_LIN);
    // Записываем координаты рамки
    MPoint NWPnt;
    //NWPnt = MPoint((double)(sxf_pasp.YNorthWestCoordinate/10+sxf_pasp.YNorthWestLocation*DEVICE_KOEFF),(double)(sxf_pasp.XNorthWestCoordinate/10.0+sxf_pasp.XNorthWestLocation*DEVICE_KOEFF));
    NWPnt = MPoint((double)sxf_pasp.YNorthWestCoordinate,(double)sxf_pasp.XNorthWestCoordinate);
    pTpo->AddPoint(&NWPnt); // северо-западного угла
    MPoint NEPnt;
    //HelpPnt = MPoint((double)(sxf_pasp.YNorthEastCoordinate/10.0+sxf_pasp.YNorthEastLocation*DEVICE_KOEFF),(double)(sxf_pasp.XNorthEastCoordinate/10.0+sxf_pasp.XNorthEastLocation*DEVICE_KOEFF));
    NEPnt = MPoint((double)sxf_pasp.YNorthEastCoordinate,(double)sxf_pasp.XNorthEastCoordinate);
    pTpo->AddPoint(&NEPnt); // северо-восточного угла
    MPoint SEPnt;
    //HelpPnt = MPoint((double)(sxf_pasp.YSouthEastCoordinate/10.0+sxf_pasp.YSouthEastLocation*DEVICE_KOEFF),(double)(sxf_pasp.XSouthEastCoordinate/10.0+sxf_pasp.XSouthEastLocation*DEVICE_KOEFF));
    SEPnt = MPoint((double)sxf_pasp.YSouthEastCoordinate,(double)sxf_pasp.XSouthEastCoordinate);
    pTpo->AddPoint(&SEPnt); // юго-восточного угла
    MPoint SWPnt;
    //HelpPnt = MPoint((double)(sxf_pasp.YSouthWestCoordinate/10.0+sxf_pasp.YSouthWestLocation*DEVICE_KOEFF),(double)(sxf_pasp.XSouthWestCoordinate/10.0+sxf_pasp.XSouthWestLocation*DEVICE_KOEFF));
    SWPnt = MPoint((double)sxf_pasp.YSouthWestCoordinate,(double)sxf_pasp.XSouthWestCoordinate);
    pTpo->AddPoint(&SWPnt); // юго-западного угла
    pTpo->AddPoint(&NWPnt); // северо-западного угла
    // перевычислим охват
    pTpo->RecalcBounds();
  meridian= (int)(pTpo->m_bounds.CenterX()/1000000)*6 - 3;

    // Считываем из паспорта и формируем рамку листа в логических координатах устройства
    MPoint NWPntR;
    NWPntR = MPoint(sxf_pasp.YNorthWestLocation,sxf_pasp.XNorthWestLocation);

    MPoint NEPntR;
    NEPntR = MPoint(sxf_pasp.YNorthEastLocation,sxf_pasp.XNorthEastLocation);

    MPoint SEPntR;
    SEPntR = MPoint(sxf_pasp.YSouthEastLocation,sxf_pasp.XSouthEastLocation);

    MPoint SWPntR;
    SWPntR = MPoint(sxf_pasp.YSouthWestLocation,sxf_pasp.XSouthWestLocation);

    // Получаем линейное преобразование логических координатах устройства
    // в географические координаты
    //CalcLinPreobrBy3pt(NWPntR,NEPntR,SEPntR,NWPnt,NEPnt,SEPnt);
    if (DLINASG(NWPntR,SEPntR)==0.0)
        CalcLinPreobrBy3pt(SWPnt,NWPnt,NEPnt,SWPnt,NWPnt,NEPnt);//tempMax for corect reading SIT files
    else
        CalcLinPreobrBy3pt(SWPntR,NWPntR,NEPntR,SWPnt,NWPnt,NEPnt);//SEPntR,NWPnt,NEPnt,SEPnt);
//	CalcProektPreobrBy4pt(NWPntR,NEPntR,SEPntR,SWPntR,NWPnt,NEPnt,SEPnt,SWPnt);

    // Чтение дескриптора карты
    fseek(FileSxf, sxf_pasp.PassportLength, SEEK_SET );
    SXFDESCRIPTOR_4_0 sxf_descriptor;
    if (fread(&sxf_descriptor, 1, 52, FileSxf) < 52)
        return false;

    // ====================
    // Чтение секции данных
    // ====================
    fseek(FileSxf, sxf_pasp.PassportLength + sxf_descriptor.DescriptorLength, SEEK_SET );
}

    p_karta-> SetGeoBounds(pTpo->m_bounds);
    if (prAsGeneral == false) m_Generallayer->ClearObjectList();

    MZoneGaussKrugerCoo* pCoo = new MZoneGaussKrugerCoo();
    pCoo->SetBasicParameters(meridian);
    delete p_karta->m_pObjectCoo;
    p_karta->m_pObjectCoo = pCoo;

    Mashtab= (int)( (float)nadpRatio*(float)Mashtab );

    SXFHEAD sxf_head;
    // Заведём вспомогательный объект для чтения в него координат
    MTopobject* pTpoHelp = new MTopobject();
    // Вспомогательный массив точек
    MPtArray* ptaHelp;
    // Заведем надпись
    MNadp* pNadp;
    // Классификационный код последнего объекта
    unsigned long  int LastClassificalCode = 0;
    // Характер локализации последнего объекта
    unsigned short int LastLocal = 100;
    // Указатель на последний знак
    MZnak *LastZnak = NULL;
    // Переменные для хранения квадрокодов
    MString QuadraKod, TrueQuadra;
    //Vars для хранения состояния набора Значимых атрибутов
    MString AtrState, LastAtrState;

    unsigned long parentUID= 0;
    unsigned int sxfUID= 0;
    char* typeSXF="LPTNVS67????";

    AtrState.Empty();
    LastAtrState.Empty();
    //KOMMEHTARIJ KYCKAMI-nachalo2
    //ГЛАВНЫЙ ЦИКЛ ПО ВСЕМ ОБЪЕКТАМ SXF-file
    while (fread(&sxf_head, 1, 32, FileSxf) == 32)
    {
        //заботимся о выводе информации на полосу прогресса
        pos= ftell(FileSxf);
        int dwPos = pos;
        percent= 2*dwPos/dl_sxf;

        //SetPanePercent(2,percent);
        sxfUID= 256*256*sxf_head.UnionNumber + sxf_head.MemberNumber;

        //УНИКАЛЬНЫЙ номер
        parentUID++;

        // заранее вычислим смещение до следующей записи
        long ZapOffset = dwPos - 32 + sxf_head.RecordLength;
        // заранее вычислим смещение до семантики
        long SemOffset = dwPos + sxf_head.MetricLength;

        //========================
        // прочитаем точки объекта
        //========================
        pTpoHelp->ReadSxfPoints(FileSxf,
                    sxf_head.NotationDimension,
                    sxf_head.MetricElementType,
                    sxf_head.MetricElementSize,
                    sxf_head.PointMetricNumber,
                    true);

        // Определим код типа объекта
        MString str,nstr;
        str.Format("%d",sxf_head.ClassificalCode);
        QString codeItem= QString::number(sxf_head.ClassificalCode)+"  "+ typeSXF[sxf_head.Local];
        bool bAdd = true;
        //если ненужный код пропускаем объект
        bool CreateNadp = false;

        // Берем точки из вспомогательного топобъекта
        ptaHelp=(MPtArray*)pTpoHelp->m_Komplekt->GetAt(pTpoHelp->F_TOCHKI);
        // Преобразуем их координаты в координаты Гаусса-Крюгера:
        for(int i=0;i<ptaHelp->GetSize();i++)
        {
            MPoint pt = ptaHelp->GetAt(i);
            MakeLinPreobr(&pt);
            //MakeProektPreobr(&pt);
            ptaHelp->SetAt(i,&pt);
        }


        // В таблице соответствия естественно не будут указаны такие объекты, как
        // надписи и подписи, однако они должны присутствовать обязательно, поэтому их
        // будем обрабатывать отдельно.
        // 1) Надпись
        if(sxf_head.Local == 3)
        {
            MString m_FontName = "UTArial";
            MString m_ColorStr;
            // Код цвета текста по умолчанию
            MColor m_ColorKod;

            // Просматриваем таблицу соответствия почерков, и выясняем,
            // имеется ли в таблице указанный почерк
            if(m_CompareFontTable->FindInTableExt2(1,str,2,m_FontName,3,m_ColorStr))
            {
                   unsigned int xc= atoi(m_ColorStr); //DWORD
                   m_ColorKod.SetB( xc/0x10000 );
                   m_ColorKod.SetG((xc%0x10000)/0x100 );
                   m_ColorKod.SetR( xc%0x100 );

            }
            else
                {
                    qDebug(str.c_str());
                }

            // Проверяем, установлен ли флаг метрика с текстом
            if(sxf_head.MetricWithTextFlag == 1)
            {
                char temp;
                fread(&temp,1,1,FileSxf);
                int iLe;
                if(temp==0)	iLe = 3;
                else iLe = (int)temp+1;
                MString m_Text = "";
                for(int i = iLe;i>1;i--)
                {
                    fread(&temp,1,1,FileSxf);
                    if((temp==0x0a)||(temp==0x0d))
                        temp=separator;
                    m_Text += temp;
                }
                fread(&temp,1,1,FileSxf);

                // Если текст не является пустой строкой, то создаем надпись
                if(m_Text.GetLength()>0)
                {
                    // Если в служебном комплекте имеются точки
                    if(ptaHelp->GetSize()>0)
                    {
                        // Установка текста надписи
                        MString WriteString;
                        WriteString.Format("%s",m_Text.c_str());
                        if (edition == EDITION3)  dos2win( (char*)WriteString.c_str());

                        // Количество отдельных строк
                        int ind = 0;
                        // Индекс симола-разделителя в строке
                        int NextSearch = 0;
                        // Проверяем, являеся ли надпись многострочной
                        while(WriteString.GetLength()>0)
                        {
                            // Ищем следующий элемент
                            NextSearch = WriteString.Find(separator,0);
                            if(NextSearch==-1)
                                NextSearch = WriteString.GetLength();
                            // Получаем первую строку
                            MString HelpString;
                            HelpString = GetPartOfStringExt(0,NextSearch-1,WriteString);

                            if((HelpString!="")&&(HelpString!=" "))
                            {
                                pNadp = new MNadp();
                                pNadp-> m_UID = sxfUID;
                                pNadp-> m_UID_master = 0;//parentUID;
                                // Добавляем объект во вспомогательный слой для сохранения семантики
                                if(bAdd) m_SymbolList.push_back((MObject*)pNadp);
                                // Точечный объект добавляем в слой
                                if(bAdd) m_Nadplayer->AddObject(pNadp);
                                pNadp->SetBaseText(HelpString);
                                // Установка масштаба карты
                                pNadp->SetKartMashtab(1);//Mashtab);
                                // Вычисляем высоту шрифта
                                pNadp->GetBasePrk()->scaled = true;
                                pNadp->GetBasePrk()->value = Mashtab*1/1000;
                                // Устанавливаем шрифт отрисовки
                                strcpy((char*)&(pNadp->GetBasePrk()->logfont.lfFaceName),m_FontName);
                                // Устанавливаем цвет шрифт отрисовки
                                pNadp->GetBasePrk()->colorkodExt = m_ColorKod;
                                // Добавляем вектор привязки формулы
                                if(ptaHelp->GetSize()>1)
                                {
                                    MPoint pt1 = ptaHelp->GetAt(0);
                                    pt1.y -= pNadp->GetBasePrk()->value*ind*1;
                                    MPoint pt2 = ptaHelp->GetAt(1);
                                    pt2.y -= pNadp->GetBasePrk()->value*ind*1;
                                    pNadp->SetBasePoint(pt1,pt2);
                                }
                                else
                                {
                                    MPoint pt = ptaHelp->GetAt(0);
                                    pt.y -= pNadp->GetBasePrk()->value*ind*1;
                                    pNadp->SetBasePoint(pt,pt);
                                }
                            }
                            ind ++;
                            // Чистим исходную строку
                            WriteString.Delete(0,NextSearch+1);
                        }
                        CreateNadp = true;
                    }
                }
            }//end of - if(sxf_head.MetricWithTextFlag == 1)
        }
        // 2) Шаблон подписи
        if(sxf_head.Local == 5)
        {
            MString m_FontName = "UTArial";
            MString m_ColorStr;
            // Код цвета текста по умолчанию
            MColor m_ColorKod;

            // Просматриваем таблицу соответствия почерков, и выясняем,
            // имеется ли в таблице указанный почерк
            if(m_CompareFontTable->FindInTableExt2(1,str,2,m_FontName,3,m_ColorStr))
            {
                   unsigned int xc= atoi(m_ColorStr); //DWORD
                   m_ColorKod.SetB( xc/0x10000 );
                   m_ColorKod.SetG((xc%0x10000)/0x100 );
                   m_ColorKod.SetR( xc%0x100 );
            }
            else
                {
                    qDebug(str.c_str());
                }

            char temp;
            fread(&temp,1,1,FileSxf);
            int iLe;
            if(temp==0)	iLe = 3;
            else iLe = (int)temp+1;
            MString m_Text = "";
            for(int i = iLe;i>1;i--)
            {
                fread(&temp,1,1,FileSxf);
                if((temp==0x0a)||(temp==0x0d))
                    temp=separator;
                m_Text += temp;
            }
            fread(&temp,1,1,FileSxf);

            // Если текст не является пустой строкой, то создаем надпись
            if(m_Text.GetLength()>0)
            {
                // Если в служебном комплекте имеются точки
                if(ptaHelp->GetSize()>0)
                {
                    // Установка текста надписи
                    MString WriteString;
                    WriteString.Format("%s",m_Text.c_str());
                    if (edition == EDITION3)  dos2win( (char*)WriteString.c_str());

                    // Количество отдельных строк
                    int ind = 0;
                    // Индекс симола-разделителя в строке
                    int NextSearch = 0;
                    // Проверяем, являеся ли надпись многострочной
                    while(WriteString.GetLength()>0)
                    {
                        // Ищем следующий элемент
                        NextSearch = WriteString.Find(separator,0);
                        if(NextSearch==-1)
                            NextSearch = WriteString.GetLength();
                        // Получаем первую строку
                        MString HelpString;
                        HelpString = GetPartOfStringExt(0,NextSearch-1,WriteString);

                        if((HelpString!="")&&(HelpString!=" "))
                        {
                            pNadp = new MNadp();
                                pNadp-> m_UID = 0;
                                pNadp-> m_UID_master = parentUID;
                            // Добавляем объект во вспомогательный слой для сохранения семантики
                            if(bAdd) m_SymbolList.push_back((MObject*)pNadp);
                            // Точечный объект добавляем в слой
                            if(bAdd) m_Nadplayer->AddObject(pNadp);
                            pNadp->SetBaseText(HelpString);
                            // Установка масштаба карты
                            pNadp->SetKartMashtab(1);//Mashtab);
                            // Вычисляем высоту шрифта
                            pNadp->GetBasePrk()->scaled = true;
                            pNadp->GetBasePrk()->value = Mashtab*1/1000;
                            // Устанавливаем шрифт отрисовки
                            strcpy((char*)&(pNadp->GetBasePrk()->logfont.lfFaceName),m_FontName);
                            // Устанавливаем цвет шрифт отрисовки
                            pNadp->GetBasePrk()->colorkodExt = m_ColorKod;
                            // Добавляем вектор привязки формулы
                            if(ptaHelp->GetSize()>1)
                            {
                                MPoint pt1 = ptaHelp->GetAt(0);
                                pt1.y -= pNadp->GetBasePrk()->value*ind;
                                MPoint pt2 = ptaHelp->GetAt(1);
                                pt2.y -= pNadp->GetBasePrk()->value*ind;
                                pNadp->SetBasePoint(pt1,pt2);
                            }
                            else
                            {
                                MPoint pt = ptaHelp->GetAt(0);
                                pt.y -= pNadp->GetBasePrk()->value*ind;
                                pNadp->SetBasePoint(pt,pt);
                            }
                        }//endOf- if((HelpString!="")&&(HelpString!=" "))
                        ind ++;
                        // Чистим исходную строку
                        WriteString.Delete(0,NextSearch+1);
                    }//endOf- while(WriteString.GetLength()>0)
                    CreateNadp = true;
                }//endOf- if(ptaHelp->GetSize()>0)
            }//endOf- if(m_Text.GetLength()>0)
        }//endOf- if((sxf_head.Local == 5)

        // Просматриваем все остальные объекты, кроме nadpisej
        if((sxf_head.Local!=5)&&(sxf_head.Local!=3))
        {
            // Проверяем флаг замкнутости объекта, если объект замкнут, то добавим
            // ему его же первую точку
            if((sxf_head.Abridge==1)&&  (sxf_head.Local == 1))//vipa
                ptaHelp->AddTail(ptaHelp->GetAt(0));

            //(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
            //Формирование состояния Значимых атрибутов
            if (CmpFileVer == 2)
            {
                long dwCurPos, dwSemP;
                int inum = 1;

                // код характеристики, тип и масштабный коэффициент
                unsigned short int ikod = 0;
                unsigned short int itip = 0;
                short int imsb = 0;
                union
                {
                    short tmp_sh;
                    char tmp_ch[2];
                };
                char charx = 0;
                short shortx = 0;
                long int intx = 0;
                char slovo[255];
                double dblx = 0;

                //remember Offset
                pos= ftell(FileSxf);
                dwCurPos = pos;
                // скачем на начало семантики
                fseek(FileSxf,SemOffset,SEEK_SET);
                pos= ftell(FileSxf);
                // общая длина всех оставшихся записей семантики
                dwSemP = pos;

                AtrState.Empty();

                while((dwSemP+4)<ZapOffset) {
                    ikod = 0;
                    itip = 0;
                    imsb = 0;
                    tmp_sh = 0;
                    if(fread(&ikod,1,2,FileSxf)!=2)
                        break;

                    if(fread(&tmp_sh, 1, 2, FileSxf)!=2)
                        break;
                    itip = tmp_ch[0];
                    imsb = tmp_ch[1];

                    charx = 0;
                    shortx = 0;
                    intx = 0;
                    dblx = 0;
                    nstr.Empty();

                    switch (itip)
                    {
                        case 0:
                            if(imsb>255) break;
                            //slovo zanimaet kol-vo simvolov +1(tipa konec stroki)
                            if (fread(&slovo, 1, abs(imsb)+1, FileSxf)!=abs(imsb)+1) break;
                            break;
                        case 126:
                            if(imsb>255) break;
                            if(fread(&slovo, 1, abs(imsb)+1, FileSxf)!=abs(imsb)+1) break;
                            break;
                        case 1:
                            if(fread(&charx,1,1,FileSxf)!=1) break;
                            nstr.Format("<%d %d>", ikod, charx);
                            break;
                        case 2:
                            if(fread(&shortx, 1, 2,FileSxf)!=2) break;
                            nstr.Format("<%d %.0f>", ikod, shortx*pow(10.0,imsb));
                            break;
                        case 4:
                            if(fread(&intx, 1, 4,FileSxf)!=4) break;
                            nstr.Format("<%d %.0f>", ikod, intx*pow(10.0,imsb));
                            break;
                        case 8:
                            if(fread(&dblx, 1, 8, FileSxf)!=8) break;
                            break;
                        default:
                            break;
                    }
                    if (!nstr.IsEmpty())
                        AtrState += nstr;

                    pos = ftell(FileSxf);
                    dwSemP = pos;
                    inum++;
                }
                // переходим ОБРАТНО
                fseek(FileSxf,dwCurPos, SEEK_SET );
                //Добавим локализацию
                nstr = AtrState;
                AtrState.Format("!%d!",sxf_head.Local);
                //содержит кодSXF!Локализация!
                str += AtrState;
                //содержит атрибуты для сравнения
                AtrState = nstr;
            }//EndOf - it CmpFileVer ==2
            //----------------------------------------------------------------

            // Проверяем, имеет ли считанный объект такой же классификационный
            // код и характер локализации, как и предидущий
            if((LastClassificalCode!=sxf_head.ClassificalCode)||
                (LastLocal!=sxf_head.Local)||
                (LastAtrState!=AtrState))
            {
                // Запоминаем новый классификационный код как последний
                LastClassificalCode = sxf_head.ClassificalCode;
                // Запоминаем новый характер локализации как последний
                LastLocal = sxf_head.Local;
                // Запоминаем новое состояние Значимых атрибутов как последнее
                LastAtrState = AtrState;

                // Переменная, указывающая состояние объекта
                MString State = "";
                // Определим состояние объекта по характеру его локализации
                // 1) Линейный:
                if(sxf_head.Local == 0)
                    State = "Линейный";
                // 2) Площадной:
                if(sxf_head.Local == 1)
                    State = "Площадной";
                // 3) Точечный:
                if(sxf_head.Local == 2)
                    State = "Точечный";
                // 4) Векторный:
                if(sxf_head.Local == 4)
                    State = "Векторный";

                bool bFound = false;
                 // По полученному коду находим в таблице соответствия объект Унитопографа
                if (CmpFileVer == 1)
                 bFound = m_CompareTable->FindInTableExt(1,str,2,State,3,QuadraKod);
                //2 версия CMP
                if (CmpFileVer == 2)
                    bFound = m_CompareTable->FindInTableCMP(1,str,AtrState,3,QuadraKod);

                if (bFound)
                {
                    // Преобразуем квадрокод в необходимый формат
                    QuadraKod.Replace("_:_"," ");
                    // Сперва проверим, имеется ли в классификаторе такой знак
                    LastZnak = p_karta->m_pKls->GetZnak(QuadraKod,TrueQuadra);
                    if(LastZnak==NULL)
                    {
                        // Если такого знака не существует
                        fseek(FileSxf,ZapOffset, SEEK_SET); // Двигаем указатель на начало следующей записи
                        continue;							   // Продолжаем цикл
                    }

                    // По полученному квадрокоду определяем принадлежность элемента
                    if(LastZnak->GetClassType()==ID_IZOLINE)
                    {
                        pTpo = new MIzoline();
                                pTpo-> m_UID = parentUID;
                                pTpo-> m_UID_master = 0;
                        // Проверяем наличия слоя рельефа
                        if(pReliefLayer == NULL)
                        {
                            pReliefLayer = new MReliefLayer();  // Создаем слой рельефа
                            p_karta->AddLayer(pReliefLayer);             // Добавляем слой рельефа в массив слоев
                        }
                        pTpo->m_pLay = pReliefLayer;
                        // Добавим объект в список изолиний слоя
                        if(bAdd) pReliefLayer->m_izolines.push_back((MIzoline*)pTpo);
                        pTpo->ChangeClass(TrueQuadra);
                        // Установка всех точек разом
                        MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                        pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                        // перевычислим охват
                        pTpo->RecalcBounds();
                    }
                    else // Все остальные объекты
                    {
                        // Все остальные объекты
                        pTpo = (MTopobject *)CreateObjectByType(LastZnak->GetClassType());
                                pTpo-> m_UID = parentUID;
                                pTpo-> m_UID_master = 0;

                        // Установим объекту слой
                        pTpo->m_pLay = m_Generallayer;
                        // Добавим объект в список объектов слоя
                        if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);
                        pTpo->ChangeClass(TrueQuadra);

                        // Нужно выяснить, является ли объект точечным
                        // Проверяем, является ли объект точечным через проверку его
                        // иерархии на наличие топобъекта
                        if((LastZnak->GetYaz()->GetType()==F_DOT)&&// Проверка точечного объекта
                            (sxf_head.Local==15)) // Проверка признака векторизованного точечного топобъекта
                        {
                            // Если объект является точечным векторизованным объектом, то
                            // в комплект помещаем одну единственную точку - центр масс
                            MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                            MPoint pnt = ptaHelp->GetAt(0);

                            pnt += ptaHelp->GetAt(1);
                            pnt *= 0.5;
                            pta->AddTail(&pnt);
                            // Получаем угол поворота объекта
                            double angle = GetVektorGrad(ptaHelp->GetAt(0),ptaHelp->GetAt(1));

                            MDotYaz *m_DotYaz = (MDotYaz *)(pTpo->m_pZnak->GetYaz()->m_object);
                            for(int k=0;k<m_DotYaz->m_tochka->GetSize();k++)
                            {
                                if(((MSrez *)m_DotYaz->m_tochka->GetAt(k))->GetType()==IDT_NAS_SHRIFT)
                                {
                                    unsigned short AngleIndex = ((MNastrShriftSrez *)m_DotYaz->m_tochka->GetAt(k))->GetPerIndex(0);
                                    if(AngleIndex!=-1)
                                    {
                                        MString WriteStr;
                                        WriteStr.Format("%f",-angle);
                                        pTpo->SetDataPole(WriteStr,AngleIndex);
                                    }
                                }
                            }
                        }//если точечный(UTP) и векторный(SXF)
                        else
                        {
                            // Установка всех точек разом
                            MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                            pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                        }

                        // перевычислим охват
                        pTpo->RecalcBounds();
                    }
                }
                // Если в таблице соответствия элемента не было обнаружено
                else
                {
                    // Если пользователь задал включение элемента, не содержащегося в таблице
                    if(prAsGeneral)
                    {
                        pTpo = new MTopobject();
                                pTpo-> m_UID = parentUID;
                                pTpo-> m_UID_master = 0;
                        // Запись в обычный слой
                        pTpo->m_pLay = m_Generallayer;
                        // Добавим объект в список объектов слоя
                        if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);

                        // Определим какого типа объект по характеру его локализации
                        // 1) Линейный:
                        if(sxf_head.Local == 0)
                            pTpo->ConnectToServKls(F_LIN);
                        // 2) Полощадной:
                        if(sxf_head.Local == 1)
                            pTpo->ConnectToServKls(F_PLO);
                        // 3) Точечный:
                        if(sxf_head.Local == 2)
                            pTpo->ConnectToServKls(F_DOT);
                        // 4) Векторный
                        if(sxf_head.Local == 4)
                            pTpo->ConnectToServKls(F_LIN);

                        // Установка всех точек разом
                        MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                        pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                        // перевычислим охват
                        pTpo->RecalcBounds();
                    }
                    // Указывает на то, что последний знак импортировался как обобщенный
                    LastZnak = NULL;
                }
            }
            else // = TR:Obobwennyj object
            {
                // Проверяем, был ли предидущий объект обобщенным, или для него имеется знак
                if(LastZnak!=NULL)
                {
                    // Если это была изолиния
                    if(LastZnak->GetClassType()==ID_IZOLINE)
                    {
                        pTpo = new MIzoline();
                                pTpo-> m_UID = parentUID;
                                pTpo-> m_UID_master = 0;
                        pTpo->m_pLay = pReliefLayer;
                        // Добавим объект в список изолиний слоя
                        if(bAdd) pReliefLayer->m_izolines.push_back((MIzoline*)pTpo);
                        pTpo->ChangeClass(TrueQuadra);
                        // Установка всех точек разом
                        MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                        pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                        // перевычислим охват
                        pTpo->RecalcBounds();
                    }
                    else // Если это все остальные объекты
                    {
                        // Все остальные объекты
                        pTpo = (MTopobject *)CreateObjectByType(LastZnak->GetClassType());
                                pTpo-> m_UID = sxfUID;//parentUID;
                                pTpo-> m_UID_master = 0;
                        // Установим объекту слой
                        pTpo->m_pLay = m_Generallayer;
                        // Добавим объект в список объектов слоя
                        if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);
                        pTpo->ChangeClass(TrueQuadra);

                        // Проверяем, является ли объект точечным через проверку его
                        // иерархии на наличие топобъекта
                        if((LastZnak->GetYaz()->GetType()==F_DOT)&&// Проверка точечного объекта
                            (sxf_head.Local==15))//||(sxf_head.Local==4)) // Проверка признака векторизованного точечного топобъекта

                        {
                            // Если объект является точечным векторизованным объектом, то
                            // в комплект помещаем одну единственную точку - центр масс
                            MPoint pnt = ptaHelp->GetAt(0);
                            MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                            pnt += ptaHelp->GetAt(1);
                            pnt *= 0.5;
                            pta->AddTail(&pnt);
                            // Получаем угол поворота объекта
                            double angle = GetVektorGrad(ptaHelp->GetAt(0),ptaHelp->GetAt(1));

                            MDotYaz *m_DotYaz = (MDotYaz *)(pTpo->m_pZnak->GetYaz()->m_object);
                            for(int k=0;k<m_DotYaz->m_tochka->GetSize();k++)
                            {
                                if(((MSrez *)m_DotYaz->m_tochka->GetAt(k))->GetType()==IDT_NAS_SHRIFT)
                                {
                                    unsigned short AngleIndex = ((MNastrShriftSrez *)m_DotYaz->m_tochka->GetAt(k))->GetPerIndex(0);
                                    if(AngleIndex!=-1)
                                    {
                                        MString WriteStr;
                                        WriteStr.Format("%f",-angle);
                                        pTpo->SetDataPole(WriteStr,AngleIndex);
                                    }
                                }
                            }
                        }
                        else
                        {
                            // Установка всех точек разом
                            MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                            pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                        }

                        // перевычислим охват
                        pTpo->RecalcBounds();
                    }
                }
                // Если последний прочитанный элемент был обобщенным
                else
                {
                    // Если пользователь задал включение элемента, не содержащегося в таблице
                    if(prAsGeneral)
                    {
                        pTpo = new MTopobject();
                                pTpo-> m_UID = sxfUID;//parentUID;
                                pTpo-> m_UID_master = 0;
                        // Запись в обычный слой
                        pTpo->m_pLay = m_Generallayer;
                        // Добавим объект в список объектов слоя
                        if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);

                        // Определим какого типа объект по характеру его локализации
                        // 1) Линейный:
                        if(LastLocal == 0)
                            pTpo->ConnectToServKls(F_LIN);
                        // 2) Полощадной:
                        if(LastLocal == 1)
                            pTpo->ConnectToServKls(F_PLO);
                        // 3) Точечный:
                        if(LastLocal == 2)
                            pTpo->ConnectToServKls(F_DOT);
                        // 4) Векторный
                        if(LastLocal == 4)
                            pTpo->ConnectToServKls(F_LIN);

                        // Установка всех точек разом
                        MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                        pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                        // перевычислим охват
                        pTpo->RecalcBounds();
                    }
                }
            }
        }
        //***********************************************************************

        //========================
        // прочитаем подобъекты
        //========================
        pTpo-> m_UID = parentUID;
        pTpo-> m_UID_master = 0;

        unsigned long tmp_uid = pTpo->m_UID;

        MObjectList tmp_oblist;
        tmp_oblist.push_back(pTpo);

        for (int ii = 0; ii<sxf_head.SubObjectNumber ;ii++)
        {
            // Заводим переменную для определения числа точек подобъекта
            unsigned int kolpt = 0;

            // Пытаемся прочесть число точек подобъекта
            if(fread(&kolpt, 1, 4, FileSxf)!=4)
                break;

            // Определяем общее число точек метрики подобъекта
            unsigned int maska = 0xffff0000;
            kolpt = kolpt & maska;
            kolpt = kolpt>>16;

            //========================
            // прочитаем точки объекта
            //========================
            pTpoHelp->ReadSxfPoints(FileSxf,
                        sxf_head.NotationDimension,
                        sxf_head.MetricElementType,
                        sxf_head.MetricElementSize,
                        kolpt,
                        true);

            // Берем точки из вспомогательного топобъекта
            ptaHelp=(MPtArray*)pTpoHelp->m_Komplekt->GetAt(pTpoHelp->F_TOCHKI);
            // Преобразуем их координаты в координаты Гаусса-Крюгера:
            for(int i=0;i<ptaHelp->GetSize();i++)
            {
                MPoint pt = ptaHelp->GetAt(i);
                MakeLinPreobr(&pt);
                //MakeProektPreobr(&pt);
                ptaHelp->SetAt(i,&pt);
            }
            // Проверяем флаг замкнутости объекта, если объект замкнут, то добавим
            // ему его же первую точку
            if ((sxf_head.Abridge==1)     //  for PLO
                &&  (sxf_head.Local == 1) ) ptaHelp->AddTail(ptaHelp->GetAt(0));

            // Определим какого типа объект по характеру его локализации
            // 1) Линейный:
            if((sxf_head.Local == 0)&&prAsGeneral)// Если пользователь задал включение элемента
            {
                pTpo = new MTopobject();
                                pTpo-> m_UID = 0;
                                pTpo-> m_UID_master = parentUID; // y vipa = 0;
                // Запись в обычный слой
                pTpo->m_pLay = m_Generallayer;
                // Добавим объект в список объектов слоя
                if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);
                pTpo->ConnectToServKls(F_LIN);
                // Установка всех точек разом
                MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                // перевычислим охват
                pTpo->RecalcBounds();
            }
            // 2) Полощадной:
            if((sxf_head.Local == 1)&&prAsGeneral)// Если пользователь задал включение элемента
            {
                pTpo = new MTopobject();
                                pTpo-> m_UID = 0;
                                pTpo-> m_UID_master = parentUID;// y vipa = 0;
                // Запись в обычный слой
                pTpo->m_pLay = m_Generallayer;
                // Добавим объект в список объектов слоя
                if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);
                pTpo->ConnectToServKls(F_PLO);
                // Установка всех точек разом
                MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                // перевычислим охват
                pTpo->RecalcBounds();
            }
            // 3) Точечный:
            if((sxf_head.Local == 2)&&prAsGeneral)// Если пользователь задал включение элемента
            {
                pTpo = new MTopobject();
                                pTpo-> m_UID = 0;
                                pTpo-> m_UID_master = parentUID;// y vipa = 0;
                // Запись в обычный слой
                pTpo->m_pLay = m_Generallayer;
                // Добавим объект в список объектов слоя
                if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);
                pTpo->ConnectToServKls(F_DOT);
                // Установка всех точек разом
                MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                // перевычислим охват
                pTpo->RecalcBounds();
            }
            // 4) Надпись
            if(sxf_head.Local == 3)
            {
                MString m_FontName = "UTArial";
                MString m_ColorStr;
                // Код цвета текста по умолчанию
                MColor m_ColorKod;

                // Просматриваем таблицу соответствия почерков, и выясняем,
                // имеется ли в таблице указанный почерк
                if(m_CompareFontTable->FindInTableExt2(1,str,2,m_FontName,3,m_ColorStr))
                {
                   unsigned int xc= atoi(m_ColorStr); //DWORD
                   m_ColorKod.SetB( xc/0x10000 );
                   m_ColorKod.SetG((xc%0x10000)/0x100 );
                   m_ColorKod.SetR( xc%0x100 );
                }
                else
                {
                    qDebug(str.c_str());
                }
                // Проверяем, установлен ли флаг метрика с текстом
                if(sxf_head.MetricWithTextFlag == 1)
                {
                    char temp;
                    fread(&temp,1,1,FileSxf);
                    int iLe;
                    if(temp==0)	iLe = 3;
                    else iLe = (int)temp+1;
                    MString m_Text = "";
                    for(int i = iLe;i>1;i--)
                    {
                        fread(&temp,1,1,FileSxf);
                        if((temp==0x0a)||(temp==0x0d))
                            temp=separator;
                        m_Text += temp;
                    }
                    fread(&temp,1,1,FileSxf);

                    // Если текст не является пустой строкой, то создаем надпись
                    if(m_Text.GetLength()>0)
                    {
                        // Если в служебном комплекте имеются точки
                        if(ptaHelp->GetSize()>0)
                        {
                            // Сперва проверим, есть ли уже созданный слой надписей
                            if(m_Nadplayer==NULL)
                            {
                                // Если слоя нет, то создадим его с именем рассматриваемого топобъекта
                                m_Nadplayer = new MLayer();  // Создаем слой рельефа
                                p_karta->AddLayer(m_Nadplayer);       // Добавляем слой рельефа в массив слоев
                                QString constTitle = tr("Toponyms Layer");
                                m_Nadplayer->m_name = codec->fromUnicode(constTitle);//constTitle.toAscii();//"Слой надписей"
                            }

                            // Установка текста надписи
                            MString WriteString;
                            WriteString.Format("%s",m_Text.c_str());
                            if (edition == EDITION3)  dos2win( (char*)WriteString.c_str());

                            // Количество отдельных строк
                            int ind = 0;
                            // Индекс симола-разделителя в строке
                            int NextSearch = 0;
                            // Проверяем, являеся ли надпись многострочной
                            while(WriteString.GetLength()>0)
                            {
                                // Ищем следующий элемент
                                NextSearch = WriteString.Find(separator,0);
                                if(NextSearch==-1)
                                    NextSearch = WriteString.GetLength();
                                // Получаем первую строку
                                MString HelpString;
                                HelpString = GetPartOfStringExt(0,NextSearch-1,WriteString);

                                if((HelpString!="")&&(HelpString!=" "))
                                {
                                    pNadp = new MNadp();
                                pNadp-> m_UID = 0;
                                pNadp-> m_UID_master = parentUID;//y vipa =0;
                                    // Добавляем объект во вспомогательный слой для сохранения семантики
                                    if(bAdd) m_SymbolList.push_back((MObject*)pNadp);
                                    // Точечный объект добавляем в слой
                                    if(bAdd) m_Nadplayer->AddObject(pNadp);
                                    pNadp->SetBaseText(HelpString);
                                    // Установка масштаба карты
                                    pNadp->SetKartMashtab(1);//Mashtab);
                                    // Вычисляем высоту шрифта
                                    double lfHeight = 0;
                                    pNadp->GetBasePrk()->scaled = true;
                                    pNadp->GetBasePrk()->value = Mashtab*1/1000;
                                    // Устанавливаем шрифт отрисовки
                                    strcpy((char*)&(pNadp->GetBasePrk()->logfont.lfFaceName),m_FontName);
                                    // Устанавливаем цвет шрифт отрисовки
                                    pNadp->GetBasePrk()->colorkodExt = m_ColorKod;
                                    // Добавляем вектор привязки формулы
                                    if(ptaHelp->GetSize()>1)
                                    {
                                        MPoint pt1 = ptaHelp->GetAt(0);
                                        pt1.y -= pNadp->GetBasePrk()->value*ind;
                                        MPoint pt2 = ptaHelp->GetAt(1);
                                        pt2.y -= pNadp->GetBasePrk()->value*ind;
                                        pNadp->SetBasePoint(pt1,pt2);
                                    }
                                    else
                                    {
                                        MPoint pt = ptaHelp->GetAt(0);
                                        pt.y -= pNadp->GetBasePrk()->value*ind;
                                        pNadp->SetBasePoint(pt,pt);
                                    }
                                }
                                ind ++;
                                // Чистим исходную строку
                                WriteString.Delete(0,NextSearch+1);
                            }
                            CreateNadp = true;
                        }
                    }
                }
            }
            // 5) Векторный
            if((sxf_head.Local == 4)&&prAsGeneral)// Если пользователь задал включение элемента
            {
                pTpo = new MTopobject();
                                pTpo-> m_UID = 0;
                                pTpo-> m_UID_master = parentUID;//y vipa =0;
                // Запись в обычный слой
                pTpo->m_pLay = m_Generallayer;
                // Добавим объект в список объектов слоя
                if(bAdd) m_Generallayer->m_oblist.push_back((MObject*)pTpo);
                pTpo->ConnectToServKls(F_LIN);
                // Установка всех точек разом
                MPtArray* pta=(MPtArray*)pTpo->m_Komplekt->GetAt(pTpo->F_TOCHKI);
                pta->FastSet(ptaHelp->GetSize(),ptaHelp->GetHead());
                // перевычислим охват
                pTpo->RecalcBounds();
            }
            // 6) Шаблон подписи
            if(sxf_head.Local == 5)
            {
                break;// 10.07.2004 DimaLa
                MString m_FontName = "UTArial";
                MString m_ColorStr;
                // Код цвета текста по умолчанию
                MColor m_ColorKod;

                // Просматриваем таблицу соответствия почерков, и выясняем,
                // имеется ли в таблице указанный почерк
                if(m_CompareFontTable->FindInTableExt2(1,str,2,m_FontName,3,m_ColorStr))
                {
                   unsigned int xc= atoi(m_ColorStr); //DWORD
                   m_ColorKod.SetB( xc/0x10000 );
                   m_ColorKod.SetG((xc%0x10000)/0x100 );
                   m_ColorKod.SetR( xc%0x100 );
                }
                char temp;
                fread(&temp,1,1,FileSxf);
                int iLe;
                if(temp==0)	iLe = 3;
                else iLe = (int)temp+1;
                MString m_Text = "";
                for(int i = iLe;i>1;i--)
                {
                    fread(&temp,1,1,FileSxf);
                    if((temp==0x0a)||(temp==0x0d))
                        temp=separator;
                    m_Text += temp;
                }
                fread(&temp,1,1,FileSxf);

                // Если текст не является пустой строкой, то создаем надпись
                if(m_Text.GetLength()>0)
                {
                    // Если в служебном комплекте имеются точки
                    if(ptaHelp->GetSize()>0)
                    {
                        // Сперва проверим, есть ли уже созданный слой надписей
                        if(m_Nadplayer==NULL)
                        {
                            // Если слоя нет, то создадим его с именем рассматриваемого топобъекта
                            m_Nadplayer = new MLayer();  // Создаем слой рельефа
                            p_karta->AddLayer(m_Nadplayer);       // Добавляем слой рельефа в массив слоев
                            QString constTitle = tr("Toponyms Layer");//"Слой надписей");
                            m_Nadplayer->m_name = codec->fromUnicode(constTitle);//constTitle.toAscii();//"Слой надписей"

                        }
                        // Установка текста надписи
                        MString WriteString;
                        WriteString.Format("%s",m_Text.c_str());
                        if (edition == EDITION3)  dos2win( (char*)WriteString.c_str());

                        // Количество отдельных строк
                        int ind = 0;
                        // Индекс симола-разделителя в строке
                        int NextSearch = 0;
                        // Проверяем, являеся ли надпись многострочной
                        while(WriteString.GetLength()>0)
                        {
                            // Ищем следующий элемент
                            NextSearch = WriteString.Find(separator,0);
                            if(NextSearch==-1)
                                NextSearch = WriteString.GetLength();
                            // Получаем первую строку
                            MString HelpString;
                            HelpString = GetPartOfStringExt(0,NextSearch-1,WriteString);

                            if((HelpString!="")&&(HelpString!=" "))
                            {
                                pNadp = new MNadp();
                                pNadp-> m_UID = 0;
                                pNadp-> m_UID_master = parentUID;
                                // Добавляем объект во вспомогательный слой для сохранения семантики
                                if(bAdd) m_SymbolList.push_back((MObject*)pNadp);
                                // Точечный объект добавляем в слой
                                if(bAdd) m_Nadplayer->AddObject(pNadp);
                                pNadp->SetBaseText(HelpString);
                                // Установка масштаба карты
                                pNadp->SetKartMashtab(1);//Mashtab);
                                // Вычисляем высоту шрифта
                                double lfHeight = 0;
                                pNadp->GetBasePrk()->scaled = true;
                                pNadp->GetBasePrk()->value = Mashtab*1/1000;
                                // Устанавливаем шрифт отрисовки
                                strcpy((char*)&(pNadp->GetBasePrk()->logfont.lfFaceName),m_FontName);
                                // Устанавливаем цвет шрифт отрисовки
                                pNadp->GetBasePrk()->colorkodExt = m_ColorKod;//MColor(GetRValue(m_ColorKod), GetGValue(m_ColorKod), GetBValue(m_ColorKod));
                                // Добавляем вектор привязки формулы
                                if(ptaHelp->GetSize()>1)
                                {
                                    MPoint pt1 = ptaHelp->GetAt(0);
                                    pt1.y -= pNadp->GetBasePrk()->value*ind;
                                    MPoint pt2 = ptaHelp->GetAt(1);
                                    pt2.y -= pNadp->GetBasePrk()->value*ind;
                                    pNadp->SetBasePoint(pt1,pt2);
                                }
                                else
                                {
                                    MPoint pt = ptaHelp->GetAt(0);
                                    pt.y -= pNadp->GetBasePrk()->value*ind;
                                    pNadp->SetBasePoint(pt,pt);
                                }
                            }
                            ind ++;
                            // Чистим исходную строку
                            WriteString.Delete(0,NextSearch+1);
                        }
                        CreateNadp = true;
                    }
                }
            }
            pTpo->m_UID_master = tmp_uid;
            tmp_oblist.push_back(pTpo);
        }

        //***********************************************************************
        // прочитаем семантику
        //***********************************************************************
        // Для начала будем читать семантику только для подписей
        fseek(FileSxf,SemOffset,SEEK_SET);
        // общая длина всех оставшихся записей семантики
        pos= ftell(FileSxf);
        int dwSemPos = pos;

        QStringList kod_tip;
        int num = 1, AtrNum;
        while((dwSemPos+4)<ZapOffset)
        {
            // код характеристики, тип и масштабный коэффициент
            unsigned short int kod = 0;
            unsigned short int tip = 0;
            short int msb = 0;
            union
            {
                short temp_sh;
                char temp_ch[2];
            };
            temp_sh = 0;

            if(fread(&kod,1,2,FileSxf)!=2)
                break;

            if(fread(&temp_sh, 1, 2, FileSxf)!=2)
                break;

            tip = temp_ch[0];
            msb = temp_ch[1];

            MString sss=" ";  // vipa
            MString str="";  // vipa
            char charxarka = 0;
            MString tmpStr1 = "", tmpStr2 = "";
            short shortxarka = 0;
            long int intxarka = 0;
            double dblxarka = 0;
            char slovo[256];  memset(slovo, 0x00, sizeof(slovo));
            switch (tip)
            {
                case 0:
                    if(msb>255) break;
                    if(fread(&slovo, 1, abs(msb)+1, FileSxf)!=abs(msb)+1) break;
                    if (edition == EDITION3)  dos2win(slovo);
                    str.Format("%s", slovo);
                    break;
                case 126:
                    if(msb>255) break;
                    if(fread(&slovo, 1, abs(msb)+1, FileSxf)!=abs(msb)+1) break;
                    if (edition == EDITION3)  dos2win(slovo);
                    str.Format("%s",slovo);
                    break;
                case 1:
                    if(fread(&charxarka,1,1,FileSxf)!=1) break;
                    for (int iAtr = 0; iAtr<m_OrdAtrList->Size;iAtr++)
                    {
                        if (*m_OrdAtrList->GetAt(iAtr) == kod)
                        {

                            tmpStr1.Format("%i",kod);
                            tmpStr1.append ("_");
                            tmpStr2.Format("%d",charxarka);
                            tmpStr1.append (tmpStr2);
                            if (!m_AtrValueSooTable->FindInTable(1,tmpStr1,2,str))
                                str.Format("%d",charxarka);
                            break;
                        }
                        else str.Format("%d",charxarka);
                    }
                    sss.Format("%d",charxarka);
                    break;
                case 2:
                    if(fread(&shortxarka, 1, 2,FileSxf)!=2) break;
                    for (int iAtr = 0; iAtr<m_OrdAtrList->Size;iAtr++)
                    {
                        if (*m_OrdAtrList->GetAt(iAtr) == kod)
                        {
                            tmpStr1.Format("%i",kod);
                            tmpStr1.append ("_");
                            tmpStr2.Format("%i",shortxarka);
                            tmpStr1.append (tmpStr2);
                            if(!m_AtrValueSooTable->FindInTable(1,tmpStr1,2,str))
                                str.Format("%f",shortxarka*pow(10.0,msb));
                            break;
                        }
                        else
                            str.Format("%f",shortxarka*pow(10.0,msb));
                    }
                    sss.Format("%d",(int)(shortxarka*pow(10.0,msb)));
                    break;
                case 4:
                    if(fread(&intxarka, 1, 4,FileSxf)!=4) break;
                    str.Format("%f",intxarka*pow(10.0,msb));
                    sss.Format("%d",(int)(intxarka*pow(10.0,msb)) );
                    break;
                case 8:
                    if(fread(&dblxarka, 1, 8, FileSxf)!=8) break;
                    if (dblxarka > INT_MAX)
                        dblxarka = 3.14;
                    str.Format("%f",dblxarka);
                    sss.Format("%d",(int)(dblxarka));//tempmax
                    break;
                default:
                    str = "unknown type";
                    break;
            }

            OBITER it = tmp_oblist.begin();
            while(it != tmp_oblist.end())
            {
                pTpo = (MTopobject*)(*it);
              //TR: Esli 1st ver of CMP-file
                if (CmpFileVer == 1)
                switch (kod)
                {
                    case 214:
                    {
                            // Теперь необходимо пройти по списку объектов вспомогательного
                            // слоя и изменить их атрибут "высота шрифта" на значение, полученное из семантики
                            // Находим первый объект в списке объектов
                            OBITER its = m_SymbolList.begin();
                            MNadp* pSymb;
                            // Просматриваем все объекты в списке
                            while(its!=m_SymbolList.end())
                            {
                                pSymb = (MNadp*)*its;
                                pSymb->GetBasePrk()->value = Mashtab*(atof(str)+9)/10000;
                                its++;
                            }
                            // После изменения атрибутотв всех объектов слоя, его нужно очистить
                            m_SymbolList.clear();
                        break;
                    }
                    case 4:
                        {//Считали характеристику АБСОЛЮТНАЯ ВЫСОТА
                            if(!pTpo) break;
                            pTpo->SetDataPole(str,19);//"Высота"
                        break;}
                    case 9:
                        {//Считали характеристику "название"
                            if(!pTpo) break;
                            pTpo->SetDataPole(str,0);//"название"
                        break;}
                    case 1:
                        {//Считали характеристику "относит.высота"
                            if(!pTpo) break;
                            pTpo->SetDataPole(str,1);//"относит.высота"
                        break;}
                    default: break;
                }
                //если 2 версия CMP
                else if (CmpFileVer == 2)
                switch (kod)
                {
                    case 214:
                        {
                            // Теперь необходимо пройти по списку объектов вспомогательного
                            // слоя и изменить их атрибут "высота шрифта" на значение, полученное из семантики
                            // Находим первый объект в списке объектов
                            OBITER it = m_SymbolList.begin();
                            MNadp* pSymb;
                            // Просматриваем все объекты в списке
                            while(it!=m_SymbolList.end())
                            {
                                pSymb = (MNadp*)*it;
                                pSymb->GetBasePrk()->value = Mashtab*(atof(str)+9)/10000;
                                it++;
                            }
                            // После изменения атрибутотв всех объектов слоя, его нужно очистить
                            m_SymbolList.clear();
                        }break;

                    default://sopostavlenie SXF-xarakteristik UTP-atributam po tablice
                        if(!pTpo) break;
                        AtrNum = -1;
                        for (num = 0; num<m_AtrSooTable->GetSize();num+=2)
                        {
                            if (*m_AtrSooTable->GetAt(num) == kod)
                            {
                                AtrNum = *m_AtrSooTable->GetAt(num+1);
                                break;
                            }
                        }

                        if (AtrNum != -1)
                            pTpo->SetDataPole(str,AtrNum);//"относит.высота"

                        break;
                    }//switch(kod)
                it++;
            }
            pos= ftell(FileSxf);
            dwSemPos = pos;
            num++;
        }
        kod_tip.sort();
        QString strItem= codeItem + "  ";
        for (unsigned int is=0; is< kod_tip.count(); is++)
            strItem+= kod_tip.at(is);

        if (codeSXF.indexOf(strItem)==-1) codeSXF.append(strItem);

                // Если семантика не читается, то переходим на начало следующей записи
                fseek(FileSxf,ZapOffset, SEEK_SET );
    }//while (FileSxf.Read(&sxf_head,

    fclose(FileSxf);

//====================================================================

    // Далее для одного генерального слоя, в котором лежат все объекты, будем
    // выполнять процедуру растаскивания по слоям. Слои будем определять по
    // принадлежности объекта разделам классификатора

    MObjectList m_list;
    // Получаем список объектов слоя
    m_Generallayer->GetObjectList(&m_list);
    int iSize = m_list.size();
    if(iSize>0)
    {
        // Находим первый объект в списке объектов слоя
        OBITER it = m_list.begin();
        MObject* pObj;
        // Просматриваем все объекты в списке
        while(it!=m_list.end())
        {
            // Получаем следующий объект в списке
            pObj = *it;
            if(pObj->GetIerarhy().Find(ID_TOPOBJECT)!=-1){
                MTopobject* pTpo = (MTopobject*)pObj;
                // Далее нужно определить, к какому разделу
                // классификатора относится выбранный объект
                QuadraKod = pTpo->m_kkod; // Получаем полный квадрокод объекта
                // Ищем 1 часть квадрокода
                int NextSearch = QuadraKod.Find(' ',0);
                // Чистим строку
                QuadraKod.Delete(0,NextSearch+1);

                // Ищем 2 часть квадрокода
                NextSearch = QuadraKod.Find(' ',0);
                MString ZnakSistemaKod = GetPartOfStringExt(0,NextSearch-1,QuadraKod);
                // Чистим строку
                QuadraKod.Delete(0,NextSearch+1);

                // Ищем 3 часть квадрокода
                NextSearch = QuadraKod.Find(' ',0);
                MString RazdelKod = GetPartOfStringExt(0,NextSearch-1,QuadraKod);

                MZnakSistema *ZnSystema = p_karta->m_pKls->GetZnakSistema(ZnakSistemaKod);
                if(ZnSystema!= NULL)
                {
                    MRazdel *Razdel = ZnSystema->GetRazdel(RazdelKod);
                    if(Razdel!=NULL)
                    {
                        MString Name = Razdel->GetTermin()->GetShortname();
                        // Сперва проверим, есть ли уже созданный слой
                        if(m_Lastlayer==NULL)
                        {
                            // Если слоя нет, то создадим его с именем рассматриваемого топобъекта
                            m_Lastlayer = new MLayer();  // Создаем слой рельефа
                            p_karta->AddLayer(m_Lastlayer);       // Добавляем слой рельефа в массив слоев
                            m_Lastlayer->m_name = Name;
                        }
                        else // Если слой уже есть, то проверим, не соответствует ли он рассматриваемому топобъекту
                        {
                            // Проверяем, совпадает ли квадрокод объекта с именем последнегo слоя
                            if(m_Lastlayer->m_name != Name)
                            {
                                // Просматриваем все слои из имеющегося списка слоев
                                LAYITER it2 = p_karta->m_laylist.begin();
                                bool find = false;
                                while(it2!= p_karta->m_laylist.end())
                                {
                                    pLay = *it2;
                                    if(pLay->m_name == Name)
                                    {
                                        m_Lastlayer = pLay;
                                        find = true;
                                        break;
                                    }
                                    it2++;
                                }
                                // Если слоя не было найдено
                                if(!find)
                                {
                                    // Если слоя нет, то создадим его с именем рассматриваемого топобъекта
                                    m_Lastlayer = new MLayer();  // Создаем слой рельефа
                                    p_karta->AddLayer(m_Lastlayer);       // Добавляем слой рельефа в массив слоев
                                    m_Lastlayer->m_name = Name;
                                }
                            }
                        }

                        // Теперь перенесем найденный объект в соответствующий слой
                        pTpo->m_pLay = m_Lastlayer;
                        m_Lastlayer->m_oblist.push_back((MObject*)pTpo);
                        // Не забудем однако, удалить его из рассматриваемого слоя
                        m_Generallayer->RemoveObject((MObject*)pTpo);
                    }
                }
            }
            it++;
        }
    }

    //удаляем  m_Generallayer т.к.
    p_karta->RemoveLayer(m_Generallayer);
    //если в слое надписей ничего нет, то удаляем его
    if (m_Nadplayer->GetObjectCount()==0)
        p_karta->RemoveLayer(m_Nadplayer);

    // Минимизируем границы по каждому слою
    LAYITER it = p_karta->m_laylist.begin();
    while(it!=p_karta->m_laylist.end())
    {
        // Получаем указатель на следующий слой в списке
        pLay = *it;
        // Получаем список объектов слоя
        pLay->GetObjectList(&m_list);
            OBITER it2 = m_list.begin();
            // Просматриваем все объекты в списке
            while(it2!=m_list.end())
            {
                pTpo = (MTopobject*)*it2;
                pTpo->RecalcBounds();
                it2++;
            }
        pLay->MinimizeBounds();
        it++;
    }

    qApp->processEvents();

    p_karta->ConnectKlassifikator(KLS);
    p_karta->MinimizeGeobounds();
    p_karta->m_geobounds.Inflate(p_karta->m_geobounds.Width()/10);

    if ( meridian != centralMeridian )
    {
        MZoneGaussKrugerCoo* m_pCoo = new MZoneGaussKrugerCoo();
        m_pCoo->SetBasicParameters(centralMeridian);
        p_karta->ConvertKartaToCoo(m_pCoo);
        delete m_pCoo;
    }

    LAYITER itt = p_karta->m_laylist.begin();
    while (itt!=p_karta->m_laylist.end())
    {
        SplitLayer(*itt);
        itt++;
    }

    return true;
}

void MConfig::SplitLayer(MLayer * m_lay)
{
int maxShrt = 0xffff;
if (m_lay->m_oblist.size()<maxShrt)
        return;
    MLayer *nweOneLay = NULL;
    int layCnt = 2;
        while (m_lay->m_oblist.size()>maxShrt)
        {
            nweOneLay = new MLayer();
            nweOneLay->m_name = m_lay->m_name + MString(QString::number(layCnt).toStdString());
            OBITER m_obj_bgn = m_lay->m_oblist.begin();
            for (int i=0;i<maxShrt;i++)
                m_obj_bgn++;
            OBITER m_obj_end = m_obj_bgn;
            int cnt=0;
            while((cnt<maxShrt)&&(m_obj_end!=m_lay->m_oblist.end()))
                {m_obj_end++;cnt++;}

            nweOneLay->m_oblist.splice(nweOneLay->m_oblist.begin(),m_lay->m_oblist,m_obj_bgn,m_obj_end);
            nweOneLay->MinimizeBounds();
            m_lay->m_pKarta->m_laylist.push_back(nweOneLay);
            layCnt++;
        }
    m_lay->MinimizeBounds();
    m_lay->m_name.append(QString::number(1).toStdString());
}

