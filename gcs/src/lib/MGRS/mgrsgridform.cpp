#include <iostream>
#include "mgrsgridform.h"
#include "mgrs.h"
#include "ui_mgrsgridform.h"
#include "ui_searchForm.h"
#include <QAction>
#include <QProcessEnvironment>
#include "floatingwidget.h"

const QString g_strDegrees = "\u00B0";
const QString g_strMinutes = "\u2032";
const QString g_strSeconds = "\u2033";

bool convertStrToGeo(const QString & strCoord, double & retVal)
{
    if(strCoord.isEmpty())
        return false;
    const QString strDeg = strCoord.section(g_strDegrees, 0, 0);
    QString strRemain = strCoord.section(g_strDegrees, 1, 1);
    const int deg = strDeg.toInt();
    QString str1 = strRemain.section(g_strMinutes, 0, 0);
    const int min = str1.toInt();
    QString str2 = strRemain.section(g_strMinutes, 1, 1);
    const int sec = str2.section(g_strSeconds, 0, 0).toInt();
    QByteArray direction = str2.section(g_strSeconds, 1, 1).toUtf8();

    retVal = (double) (deg + (double)min/60.0 + (double)sec/3600.0);
    if(direction=="S" || direction=="W" )
        retVal*=(-1);
    return true;
}

MGRSGridForm::MGRSGridForm(QStatusBar *status, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MGRSGridForm),   
    m_data()
{
    initMGRSResource();
    ui->setupUi(this);
    searchWidget = new FloatingWidget(parent);
    SearchForm * w = new SearchForm(parent);
    searchWidget->setAnchorWidget(w);
    searchWidget->setAnchorPoint(FloatingWidget::BottomRight);
    searchWidget->setAnchorWidgetPoint(FloatingWidget::TopRight);
    searchWidget->setVisible(false);
    w->setVisible(false);
    connect (w,&SearchForm::signalFindPoint,this,&MGRSGridForm::slotFindPoint);
    connect (w,&SearchForm::signalClearPoint,this,&MGRSGridForm::signalDeletFromMap);

    connect(ui->searchButton,&QPushButton::toggled,[=](bool checked){

        ui->searchButton->setChecked(checked);
        if(checked)
        {
            searchWidget->setVisible(true);
            w->setVisible(true);
        }
        else
        {
            searchWidget->setVisible(false);
            w->setVisible(false);
        }
    });


    if(status) status->addPermanentWidget(this);
    ui->comboBox->setCurrentIndex(m_data.m_iLevelIndex);
    (void)connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChangeLevel(int)));
    slotChangeLevel(m_data.m_iLevelIndex);
    ui->comboBox_coosys->addItem(CooSysTransformation::g_strBLH, CooSysTransformation::g_iBLH);
    ui->comboBox_coosys->addItem(CooSysTransformation::g_strMGRS, CooSysTransformation::g_iMGRS);  
    (void)connect(ui->comboBox_coosys, SIGNAL(currentIndexChanged(int)), SLOT(slotChangeCoords(int)));
    ui->comboBox_coosys->setCurrentIndex(m_data.m_iActCooSystem);
    slotChangeCoords(m_data.m_iActCooSystem);

}
void MGRSGridForm::slotFindPoint(const  QString & point, int actCooSystem){
        if(point.isEmpty())
            return;
        double lat,lon;
        switch(actCooSystem){
        case(CooSysTransformation::g_iMGRS):
            SActCooSystemParams::toGeo(point,lon,lat,true);
            break;
        case(CooSysTransformation::g_iBLH):
            QStringList coords = point.split(" ");
            if(coords.size()==2)
            {
            if(!convertStrToGeo(coords.at(0),lat))
                            return;
            if(!convertStrToGeo(coords.at(1),lon))
                            return;
            }
            else
                return;
            break;

        }
        emit signalSearch(lat,lon);
    }
MGRSGridForm::~MGRSGridForm()
{
    std::cout<<"~MGRSGridForm()"<<std::endl;
    delete ui;
}

void MGRSGridForm::ShowOnlySerchButton()
{
    ui->comboBox->setVisible(false);
    ui->comboBox_coosys->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    setWindowFlag(Qt::FramelessWindowHint);
    setStyleSheet("border: none;");


}

void MGRSGridForm::changePoint(const double  & x,const double  & y)
{
    xPoint = x;
    yPoint = y;
    updateCoords();
}

void MGRSGridForm::updateCoords()
{
    ui->label_2->setText(m_data.toMGRS(xPoint, yPoint));
}


void MGRSGridForm::slotChangeLevel(int index)
{
    m_data.m_iLevelIndex = index;
    updateCoords();  
    emit signalChangeCoords(m_data);    
}


void MGRSGridForm::slotChangeCoords(int index)
{
    m_data.m_iActCooSystem = index;   
    emit signalChangeCoords(m_data);
}

QString SActCooSystemParams::toMGRS(const double &lon, const double &lat) const
{   
    return QString::fromStdString(MGRS::convertFromGeo(lon,lat, m_iLevelIndex));
}

void SActCooSystemParams::toGeo(const QString &mgrs,
                                double &lon, double &lat,
                                bool centerp)
{   
    std::string mgrsStr = mgrs.toStdString();
    MGRS::convertToGeo(mgrsStr, lon,lat, centerp); 
}

SearchForm::SearchForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchForm),strPoint(QString()),
    m_iActCooSystem(CooSysTransformation::g_iBLH)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    connect(ui->lineEdit,&QLineEdit::textChanged,this,&SearchForm::slotCheckEnteredText);
    connect(ui->pushButton,&QPushButton::clicked,this,&SearchForm::slotSearchButtonClicked);
    auto list = ui->lineEdit->children();
    for(QObject *obj : list)
    {
        QAction *a = qobject_cast<QAction*>(obj);
        if (a)
        {
            connect(a, &QAction::triggered,
                    [this]()
            {
                ui->lineEdit->setText(tr(""));
                ui->pushButton->setEnabled(false);
                emit signalClearPoint();
            });
            break;
        }
    }

}
SearchForm::~SearchForm()
{
    delete ui;
}

void SearchForm::slotCheckEnteredText(const QString & str)
{
   static QString strExpMGRS("(^[1-9]|^[1-5][0-9]|^60){1,2}([^ABIOYZabioyz0-9]{1})([^IOio0-9]{2})([0-9]{2}|[0-9]{4}|[0-9]{6}|[0-9]{8}|[0-9]{10})$");
   static QString strExpBLH = QString("([1-9]|[9][0]|[0-8][0-9])°([0-9]|[1-5][0-9])′(\\d|[1-5][0-9])″([NS]){1}\\s(\\d|[1][8][0]|[1][0-7][0-9]|[1-9][0-9])°(\\d|[1-5][0-9])′(\\d|[1-5][0-9])″([EW]){1}");

   static QRegExp  expMGRS(strExpMGRS) ;
   static QRegExp  expBLH(strExpBLH) ;
   if(expMGRS.exactMatch(str))
   {
       m_iActCooSystem = CooSysTransformation::g_iMGRS;
       strPoint = str;
       ui->pushButton->setEnabled(true);
   }
   else if(expBLH.exactMatch(str))
   {
       m_iActCooSystem = CooSysTransformation::g_iBLH;
       strPoint = str;
       ui->pushButton->setEnabled(true);
   }
   else
       ui->pushButton->setEnabled(false);

}

void SearchForm::slotSearchButtonClicked(bool )
{
    emit signalFindPoint(strPoint,m_iActCooSystem);
}
