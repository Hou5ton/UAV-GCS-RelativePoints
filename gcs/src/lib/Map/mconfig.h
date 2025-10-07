//PACKAGE : MAP.
//FILE: mconfig.h   
//AUTHOR:
//DESCRIPTION:

#ifndef MCONFIG_H
#define MCONFIG_H

#include <iosfwd>
#include <qmath.h>
#include <qobject.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <QTreeWidget>
#include <QTableWidget>
#include <qcheckbox.h>
#include <QGroupBox>
#include <qtextcodec.h>
#include <qfile.h>
#include <qimage.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qspinbox.h>
#include <qcolordialog.h>
#include <qprocess.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qslider.h>
#include <qregexp.h>
#include <qcursor.h>
//#include <qdom.h>
#include <QListWidget>
#include <qdialog.h>
#include <QProgressBar>

#include "Klassifikator.h"
#include "Karta.h"
#include "KartaInterface.h"
#include "Layer.h"
#include "ReliefRaster.h"
#include "Object.h"
#include "Nadp.h"
#include "Coo.h"

typedef MCoo* MPtCoo ;
#define   i_ID           7            //   for debug (layer number)
#define   TID_UZN            3            //   id of object
#define   TID_MASTER     6            //   id

class MReliefKeyPanel : public QWidget
{
    Q_OBJECT

public:
    MReliefKeyPanel(QWidget *parent, int maxH, int minH) : QWidget(parent)
    {
        _hmax = maxH;
        _hmin = minH;
    }
    ~MReliefKeyPanel() { }

private:
    int _hmin;
    int _hmax;

    virtual void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);

        QColor rgb;
        double tmp0 = 9000, /*tmp500 = 9000, */tmp2k=9000, tmp5k = 9000, tmp8k = 9000;
        int i0,/*i500,*/i2k,i5k,i8k;
        for (int i = 0; i < this->width(); i++)
        {
            double ht = (_hmax-_hmin)*pow(fabs(sin(i*M_PI/(2*this->width()))),3.0)+_hmin;
            if (fabs(ht)< tmp0) {
                tmp0 = fabs(ht);
                i0 = i;
            }

            if ((ht >= -500.0)&&(ht <= 0))
            {
                double r = fabs(cos(ht*M_PI/1000));
                double g = 0.39+0.61*(ht + 500.0)/500.0;
                rgb.setRgbF(r,g,0.0);
            }
            else if ((ht > 0)&&(ht <= 2000))
            {
                if (fabs(2000.0-ht) < tmp2k) {
                    tmp2k = fabs(2000.0-ht);
                    i2k = i;
                }
                double r = 1.0;
                double g = sqrt(1-fabs(sin(0.5*ht*M_PI/2000.0)));
                rgb.setRgbF(r,g,0.0);
            }
            else if (ht > 2000)
            {
                if (fabs(5000.0-ht)< tmp5k) {
                    tmp5k = fabs(5000.0-ht);
                    i5k = i;
                }if (fabs(8000.0-ht)< tmp8k) {
                    tmp8k = fabs(8000.0-ht);
                    i8k = i;
                }
                double r = 0.5 + 0.5*sqrt(fabs(cos(0.5*((double)ht-2000.0)*M_PI/((double)_hmax-2000.0))));
                rgb.setRgbF(r,0.0,0.0);
            }
            painter.setPen(rgb);
            painter.drawLine(i, 0, i, 30);
        }
        painter.setPen(QColor::fromRgb(0,0,0));
        painter.drawText(QPoint(1,20),"-500");
        painter.drawText(QPoint(this->width()-28,20),"9000");
        painter.drawLine(i0,0,i0,6);
        painter.drawText(QPoint(i0-3,20),"0");
        painter.drawLine(i2k,0,i2k,6);
        painter.drawText(QPoint(i2k-13,20),"2000");
        painter.drawLine(i5k,0,i5k,6);
        painter.drawText(QPoint(i5k-13,20),"5000");
        painter.drawLine(i8k,0,i8k,6);
        painter.drawText(QPoint(i8k-13,20),"8000");
    }
};

class QComboBox;
class QTableWidget;

//CLASS			:MConfig : public QObject
//
//DESCRIPTION	: Outputs dialog for viewing of parameters of the map
//				 creates new map project, loads existing project
class MConfig : public QObject  {
    Q_OBJECT
public:

    //PACKAGE : MAP.
    //FUNCTION : MConfig::MConfig(QWidget *parent, MKartaInterface *ki,  bool screenFl=1);
    //DESCRIPTION : Constructor
    //  parent-  window of parent class, ki- object MKartaInterface,
    //  screenFl=true   -outputs created pixmap-image of the map to the screen during map drawing
    //  screenFl=false  -creates pixmap-image of the map during the map drawing but do not represent it.
    //INPUTS : QWidget pointer, MKartaInterface pointer,  bool .
    //RETURNS : NONE.
    MConfig(QWidget *parent, MKartaInterface *ki,  bool screenFl=1);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::~MConfig();
    //DESCRIPTION : Destructor.
    //INPUTS : NONE.
    //RETURNS : NONE.
    ~MConfig();

    //PACKAGE : MAP.
    struct TabSetup
    {
        TabSetup(bool tabSheets=true,
                 bool tabLoadFilter=false,
                 bool tabAdvanced=true,
                 bool tabInfo=true,
                 bool tabCoordinateSystem=false):
            fTabSheets(tabSheets),
            fTabLoadFilter(tabLoadFilter),
            fTabAdvanced(tabAdvanced),
            fTabInfo(tabInfo),
            fTabCoordinateSystem(tabCoordinateSystem){};
        ~TabSetup(){};
        bool  fTabSheets;
        bool  fTabLoadFilter;
        bool  fTabAdvanced;
        bool  fTabInfo;
        bool  fTabCoordinateSystem;
    }mTabSetup;
    //FUNCTION : MConfig::setup(bool screenFl=1);
    //DESCRIPTION : Dialogue - Set up current map roject
    //				screenFl=false  - creates pixmap-image of the map during the map drawing but do not represent it.
    //INPUTS : bool.
    //RETURNS : NONE.
    void    setup(bool screenFl=1);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::setMatrixFlag(bool mtxFl=1);
    //DESCRIPTION :  Sets sign of altitudes matrix handling.
    //INPUTS : bool.
    //RETURNS : NONE.
    void    setMatrixFlag(bool mtxFl=1)      {matrixFlag= mtxFl;}

    //PACKAGE : MAP.
    //FUNCTION : MConfig::setMatrix2Flag(bool mtx2Fl=1);
    //DESCRIPTION : Sets sign of exceeding matrix handling.
    //INPUTS : bool.
    //RETURNS : NONE.
    void    setMatrix2Flag(bool mtx2Fl=1)      {matrix2Flag= mtx2Fl;}

    //PACKAGE : MAP.
    //FUNCTION : MConfig::setScreenFlag(bool screenFl=1);
    //DESCRIPTION :  Sets sign of visibility of pixmap-image of the map.
    //INPUTS :  bool.
    //RETURNS : NONE.
    void    setScreenFlag(bool screenFl=1)   {screenFlag= screenFl;}

    //PACKAGE : MAP.
    //FUNCTION : MConfig::setupGeoLayers(bool screenFl=1);
    //DESCRIPTION : Synonim setup(), look at description setup(bool screenFl).
    //INPUTS : bool.
    //RETURNS : NONE.
    void    setupGeoLayers(bool screenFl=1);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::newMap (bool mtxFl=0, bool mtx2Fl=0);
    //DESCRIPTION : Dialogue- Create new map project
    //				using altitude matrixes (if mtxFl=true)
    //				and exceeding matrixes (if mtx2Fl=true).
    //INPUTS : bool , bool .
    //RETURNS : True.
    bool    newMap (bool mtxFl=0, bool mtx2Fl=0);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::loadMapExtra( const char *fileName);
    //DESCRIPTION : Loads map sheet from 'fileName' and represent it.
    //INPUTS : const char pointer.
    //RETURNS : True.
    bool    loadMapExtra( const char *fileName);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::eraseMapExtra( const char *fileName);
    //DESCRIPTION : Clears map sheet, which is loaded from 'fileName'.
    //INPUTS : const char pointer.
    //RETURNS : True.
    bool    eraseMapExtra( const char *fileName);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::saveConfig(const char *fileName=0);
    //DESCRIPTION : Save map project to 'fileName'.
    //  File by default - "<name of location>.ini".
    //INPUTS : const char pointer.
    //RETURNS : True.
    bool    saveConfig(const char *fileName=0);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::initMap(bool mtxFl=0, const char *fileName=0, bool mtx2Fl=0);
    //DESCRIPTION :  Loads map project from 'fileName'.
    /*! Loads altitudes matrix from 'fileName', if mtxFl=true.
    Loads exceeding matrix from 'fileName', if mtx2Fl=true.
    fileName by default - "<name of location>.ini".*/
    //INPUTS : bool , const char pointer, bool mtx2Fl.
    //RETURNS : True.
    bool    initMap(bool mtxFl=0, const QString &fileName=0, bool mtx2Fl=0);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::exists();
    //DESCRIPTION : Returns true if map is loaded.
    //INPUTS : NONE.
    //RETURNS : True.
    bool    exists();

    //PACKAGE : MAP.
    //FUNCTION : MConfig::resetConfig( const char *fileName);
    //DESCRIPTION : Resetting of loaded map by data from project 'fileName'.
    /*! Returns true if successful
    Returns false if project 'fileName' is not conform to current
    fileName must be created by function 'saveConfig()'.
*/
    //INPUTS : const char pointer.
    //RETURNS : True.
    bool    resetConfig( const char *fileName);

    //  Creates(Refreshs) list of loaded files of project.
    /*! refreshLoadList() Recommended to call after downloading/uploading map sheets with the help of
    functions of another classes (MKarta, MKartaInterface).
*/

    //PACKAGE : MAP.
    //FUNCTION : MConfig::refreshLoadList();
    //DESCRIPTION :   Creates(Refreshs) list of loaded files of project.
    /*! refreshLoadList() Recommended to call after downloading/uploading map sheets with the help of
    functions of another classes (MKarta, MKartaInterface).
*/
    //INPUTS : NONE.
    //RETURNS : NONE.
    void    refreshLoadList();

    //PACKAGE : MAP.
    //FUNCTION : MConfig::getUtpFile();
    //DESCRIPTION : Gets Utp-File.
    //INPUTS : NONE.
    //RETURNS : QString .
    QString  getUtpFile()    {return utpFile;};

    //PACKAGE : MAP.
    //FUNCTION : MConfig::getKlsFile();
    //DESCRIPTION : Gets Kls-File.
    //INPUTS : NONE.
    //RETURNS : QString .
    QString  getKlsFile()    {return klsFile;};

    //PACKAGE : MAP.
    //FUNCTION : MConfig::getMtxFile();
    //DESCRIPTION : Gets Mtx-File.
    //INPUTS : NONE.
    //RETURNS : QString .
    QString  getMtxFile()    {return mtxFile;};

    //PACKAGE : MAP.
    //FUNCTION : MConfig::getObjMtxFile();
    //DESCRIPTION : Gets Mtx-File.
    //INPUTS : NONE.
    //RETURNS : QString .
    QString  getObjMtxFile() {return mtx2File;};

    //PACKAGE : MAP.
    //FUNCTION : MConfig::getCorMtxFile();
    //DESCRIPTION : Get Mtx-File.
    //INPUTS : NONE.
    //RETURNS : QString .
    QString  getCorMtxFile() {return mtx3File;};

    //PACKAGE : MAP.
    //FUNCTION : MConfig::selectZnakDialog();
    //DESCRIPTION : Dialogue - Object selection of active ground environment from classifier list
    //				Returns qaadrocode of selected object or empty line..
    //INPUTS : NONE.
    //RETURNS : QString .
    QString  selectZnakDialog();

    //  Returns pointers to map object, nearest to the point 'pnt'.
    //MObject* findNearestGeoObject(MPoint pnt);


    //PACKAGE : MAP.
    //FUNCTION : MConfig::CreatePixmap(MKlassifikator* kls, MYaz* c_yaz, MZnak* znak);
    //DESCRIPTION : Create pixmap.
    //INPUTS : MKlassifikator pointer, MYaz pointer, MZnak pointer.
    //RETURNS : QPixmap.
    QPixmap CreatePixmap(MKlassifikator* kls, MYaz* c_yaz, MZnak* znak);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::CreatePixmapImage(MTopobject* pointer, bool CenterPoint, MZnak* znak, MYaz* yaz);
    //DESCRIPTION : Create pixmap image.
    //INPUTS : MTopobject pointer, bool , MZnak pointer, MYaz pointer.
    //RETURNS : QPixmap.
    QPixmap CreatePixmapImage(MTopobject* pointer, bool CenterPoint, MZnak* znak, MYaz* yaz);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::GetPointForObject(MYaz* yaz);
    //DESCRIPTION : Get point for object.
    //INPUTS : MYaz pointer.
    //RETURNS : NONE.
    void GetPointForObject(MYaz* yaz);

    //PACKAGE : MAP.
    //FUNCTION : MConfig::CreateTopObject();
    //DESCRIPTION : Create top object.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void CreateTopObject();


    MTopobject *LIN_topObj;
    MTopobject *PLO_topObj;
    MTopobject *DOT_topObj;

signals:

    //PACKAGE : MAP.
    //FUNCTION : MConfig::closeClicked();
    //DESCRIPTION : The button 'CLOSE' is presed in dialogue of map project setting. Absolete, use mapPainted().
    //INPUTS : NONE.
    //RETURNS : NONE.
    void	closeClicked();

    //PACKAGE : MAP.
    //FUNCTION : MConfig::drawClicked();
    //DESCRIPTION : The button 'APPLY' is presed in dialogue of map project setting. Absolete, use mapPainted().
    //INPUTS : NONE.
    //RETURNS : NONE.
    void	drawClicked();

    //PACKAGE : MAP.
    //FUNCTION : MConfig::mapPainted();
    //DESCRIPTION : Pixmap-image of the map has been created. The image is displayed if  screenFlag= true..
    //INPUTS : NONE.
    //RETURNS : NONE.
    void	mapPainted();

    //PACKAGE : MAP.
    //FUNCTION : MConfig::kartaToBeCleared();
    //DESCRIPTION : Signal is emitted before clearing old m_karta and loading new .
    //INPUTS : NONE.
    //RETURNS : NONE.
    void kartaToBeCleared();

    //PACKAGE : MAP.
    //FUNCTION : MConfig::typesWinToBeRefreshed()
    //DESCRIPTION : A signal that is emitted after loading extramaps to refresh windows of allowable elements
    //              for shortest path and graph building.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void typesWinToBeRefreshed();
    //PACKAGE : MAP.
    //FUNCTION : MConfig::configFileSaved(QString)
    //DESCRIPTION : Signal that is emitted when the project file is changed.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void configFileSaved(QString);

protected:
private:

    bool    setLoadFilterEnabled(bool);
    bool    setAnyEnabled(bool);
    bool    loadConfig(bool mtx=0, bool mtx2=0);
    void    setupMapExtra();
    void    setupMap(const QString);
    bool    selectMapFiles(bool mtx=0, bool mtx2=0);
    bool    loadMap();
    bool    setClassificator();
    bool    setMatrixFiles();
    bool    drawMap();
    //bool    removeOrphans();
    //bool    refreshOrphans();

    void    getCurrentConfig();  //void    getOriginConfig();
    void    readConfigMain(const QString);
    void    setupFastDraw (const QString);
    void    sortLayers();
    void    createObjMapList();
    bool    setDrawFilter();
    int     getLayerObjectCount(QString);

    void    createLayerObjectList(QString);

    void createForm();
    void fillListForm1();
    void fillListForm2();
    void fillListForm3();
    void fillListForm10();
    void fillListForm30();
    void fillCheckForm();
    void fillSubForm();

    void    refreshForm1();
    void    refreshForm2();
    void    refreshForm3();
    void    refreshForm5();
    void    refreshForm7();
    void    refreshLayerList();
    void    refreshLayerItem();

    MLayer* findLayer(const int);
    MLayer* findLayerAt(const int);
    MLayer* findLayerByName(QString, int=0);

    MString GetPartOfStringExt(int Source,int Dest,MString BaseString);
    bool SetObjectCoo(MCoo* pCoo, MKarta* p_karta = nullptr);
    void ConvertToCoo(MCoo* coo, QString elipsoid_name, int paramCount, QString* names, QString* data);
    void ConvertToCoo(MCoo* pCoo);

    void setScaleSpanLabel(int minScale, int maxScale);

private slots:

    // ----------  1. Layers  ---------------------------

    void visibleClicked();       //<  Changes sign of layer displaying
    void signatureClicked();     //<  Changes sign of inscriptions displaying
    //   void transparentClicked();   //<  Changes sign of transparency(50%) displaying
    void navigatorClicked();       //<  Changes sign of Displaying on a creen
    void FindInClicked();		  // < Changes flag of Searching in current Layer


    void simpleClicked();
    void widthClicked(int);
    void linecolorButton();
    void fillcolorButton();
    void currentLayer(QTreeWidgetItem *nd);  // < current layer
    void upReal();
    void downReal();
    void applyButton();      // <  Change signs of he current layer
    void showReliefButton();
    void setSadowOnRelief(int state);

    // --------     draw Filter ( 1. Layers  )
    void selectObjectsDialog();
    void currentObject10(QTreeWidgetItem *nd, int column);
    void ok10Button();

    // ------------- 3. Load filter ------------
    void currentObject(QTreeWidgetItem *nd, int column);
    void itemSelection();
    void	objectsClicked();

    // -------------- getZnakDialog ------------
    void currentObject30(QTreeWidgetItem *nd);
    void ok30Button();
    void cancel30Button();

    // ----------- global buttons --------------
    void OKButtonGlobal();
    void saveButton();
    void saveAsButton();
    void cancelButton();
    void tabChanged(int index);

    // ----------- 2. Sheets  ------------------
    void currentFile(QTreeWidgetItem *nd, int column);
    void load();
    void loadSelected();
    void unload();
    void insertButton();
    void deleteButton();
    //PACKAGE : MAP.
    //FUNCTION : MConfig::slotDisableByGB()
    //DESCRIPTION : Set false to a logical variable to disable actions that are incompatible with graph building.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void slotDisableByGB() {isEnabledByGB = false;}

    //PACKAGE : MAP.
    //FUNCTION : MConfig::slotEnableByGB()
    //DESCRIPTION : Set true to the logical variable to enable actions that are incompatible with graph building.
    //INPUTS : NONE.
    //RETURNS : NONE.
    void slotEnableByGB() {isEnabledByGB = true;}
    // ------------ 4. Advanced  ---------------
    void palette1Changed(int);
    void palette2Changed(int);
    void palette3Changed(int);
    void fontsizeChanged(int);
    void sliderReleased();
    void mapOpacityTooggled(bool);
    void lightnessChanged(int);
    void transparentChanged(int);
    void paletteReset();
    void	textMaxChanged(int);
    void	textMinChanged(int);
    void	objsizeChanged(int);
    void groundcolorButton();
    void ok5Button();
    void destCooChanged(int);
    void destElipsoidChanged(int);
    void convertCooClicked(bool);
    // ------------ 6. Scale  ---------------
    void saveMinScale(int);
    void saveMaxScale(int);
    void sliderToZeroSlot();
    void sliderEnableSlot();


public:
    // ------ Public map project parameters  ------
    QString configFile;   // project name

    void readJson(const MString &fileName);
    bool saveSXF(QString sxfName);

    /// DESCRIPTION : Sets tilelevel for MGRS
    void setMGRSTileLevel(int level){mgrsTilelevel = level;}
    /// DESCRIPTION : Returns tilelevel for MGRS
    int getMGRSTileLevel(){return mgrsTilelevel;}

    //for sxf-converter
    bool   readCmp(QString cmpName, MTable& m_CompareTable, MTable& m_AtrValueSooTable, MInArray& m_AtrSooTable, MInArray& m_OrdAtrList, int& CmpFileVer);
    bool	readCmf(QString cmfName, MTable& m_CompareFontTable);
    bool    loadSXF(QString sxfName, MTable* m_CompareTable, MTable* m_AtrValueSooTable, MInArray* m_AtrSooTable, MInArray* m_OrdAtrList,
                    MTable* m_CompareFontTable, int CmpFileVer, double centralMeridian, MKarta *p_karta);
    void SplitLayer(MLayer * m_lay);

private:
    QWidget          *parent = nullptr;
    QDialog			 *tabDialog = nullptr;
    QTabWidget       *tab = nullptr;
    QWidget          *w1 = nullptr, *w2 = nullptr, *w3 = nullptr, *w5 = nullptr, *w6 = nullptr, *w7 = nullptr;
    QDialog          *w10 = nullptr, *w30 = nullptr;
    QWidget          *objWin = nullptr;
    MKartaInterface  *IMap = nullptr;
    MKarta           *karta = nullptr;
    QGridLayout     *grid = nullptr;
    QTreeWidget        *list = nullptr, *list2 = nullptr, *list3 = nullptr, *list10 = nullptr, *list30 = nullptr;
    QSlider          *slider = nullptr;
    MKlassifikator   *KLS = nullptr;
    QPushButton		 *pbShowRelief = nullptr;
    QCheckBox		 *drawReliefShadow = nullptr;
    //QSlider          *sliderScale;
    QSlider          *sliderScaleMin = nullptr, *sliderScaleMax = nullptr;
    MReliefKeyPanel     *reliefKeyPanel = nullptr;

    MKartaInterface  *mki = nullptr, *mki30 = nullptr;

    QMap<QString,QJsonObject> uznToSxfCodeMap;

    bool screenFlag, matrixFlag, matrix2Flag;
    QPushButton   *pbUp = nullptr,  *pbDown = nullptr;   //QPushButton   *pbCancel;
    QPushButton   *pbOK = nullptr,  *pbSave = nullptr;
    QPushButton   *pbOK10 = nullptr;
    QPushButton   *pbOK30 = nullptr, *pbCancel30 = nullptr;
    QPushButton   *pbOK5 = nullptr;
    QGroupBox     *scaleGroupBox = nullptr;
    bool           sliderScaleMinEnable, sliderScaleMaxEnable;
    QList<int>     scaleList;

    QPushButton   *pbInsert2 = nullptr, *pbDelete2 = nullptr;
    QPushButton   *pbInsert4 = nullptr, *pbDelete4 = nullptr;

    QLabel *lab33 = nullptr,  *lab10 = nullptr,  *labMap = nullptr;
    QLabel *labSize = nullptr,  *labTextMax = nullptr,  *labTextMin = nullptr,    *lab6 = nullptr,  *lab7 = nullptr,   *lab8 = nullptr,   *labBG = nullptr;
    QLabel        *labSelect = nullptr;
    QLabel *labMinScale = nullptr, *labMaxScale = nullptr, *labScaleSpan = nullptr;
    QPushButton   *pbSelect = nullptr;

    QSpinBox      *sb6 = nullptr,  *sbSize = nullptr,  *sbTextMax = nullptr,  *sbTextMin = nullptr;
    QPushButton   *pb7 = nullptr, *pb8 = nullptr, *pbBG = nullptr;

    QListWidget      *box7 = nullptr;

    QLabel *lab51 = nullptr, *lab52 = nullptr, *lab53 = nullptr, *lab54 = nullptr;
    QSlider *sl51 = nullptr, *sl52 = nullptr, *sl53 = nullptr, *sl54 = nullptr, *sl55 = nullptr, *sl56 = nullptr;
    QPushButton *pbReset5 = nullptr;
    QTextCodec *codec = nullptr, *codecSXF = nullptr;

    //-------6.Convert Coordinate System---------
    QComboBox* comboDestinationCoo = nullptr;
    QComboBox* comboDestinationElipsoid = nullptr;
    QLineEdit* lineCurrentCoo = nullptr;
    QLineEdit* lineCurrentElipsoid = nullptr;
    QPushButton* btnConvertCoo = nullptr;
    QPushButton* btnResetCoo = nullptr;
    QTableWidget* cooParamTable = nullptr;

    /***************************/
    MPtCoo* CooArray = nullptr;
    MStArray* parnames = nullptr;
    MStArray* pardata = nullptr;
    std::vector<int> workCooArray;
    std::vector<int> workDatumArray;
    /***************************/

    bool simple, visible,  signature,  transparent,  navigator, findIn, filter;  // current data
    MColor  cLine,  cFiller,  cText;
    int iFillerTransparent;//прозрачность заливки объектов в слое
    int iLineTransparent;//прозрачность заливки объектов в слое

    int  id1, id2, id4;

    int  wLine,   wText;
    int  wLine0,  wText0;
    int  tabID;
    QTreeWidgetItem  *node1 = nullptr, *node2 = nullptr, *node3 = nullptr, *node4 = nullptr;
    QStringList    objMapList;
    vector<int>    cntMapList;
    QStringList    objectLayerList;

    QString currentZnak;

    // ------ Private  map project parameters  ------

    QString utpFile = QString();
    QString klsFile = QString();
    QString mtxFile = QString();
    QString mtx2File = QString();
    QString mtx3File = QString();

    int     textMin, textMax, objSize;
    MColor  cGround;
    QString layerLine,  visibleLine,  signatureLine, navigatorLine, schemeLine;     //transparentLine,

    QStringList    loadFileList,  loadFlagList;
    QStringList    objLoadList,   objDrawList;
    QStringList    layerNameList, layerNameFilterList;

    bool actBack;
    int p1,p2,p3,p4,p5,p6;

    MRect zoom;
    int mgrsTilelevel = 5;      ///< pow(base, mgrsTilelevel) - step mgrs grid (0 - 5)
    //for importing from SXF-files

    bool htmlFlag;
    bool isNewMap;
    int GetTreeItemDepth( QTreeWidgetItem *item);
    bool isEnabledByGB; //The logical variable that shows if actions that are incompatible with graph building are enabled.
    //for importing from SXF-files
    QStringList  codeSXF;
    QStringList  cmpList, cmfList, klsList;
};
#endif

