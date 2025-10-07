/**
\file    FILE        : mgrsgridform.h

\brief   DESCRIPTION : Implements widget on status bar with coordinate system settings
*/

#ifndef MGRSGRIDFORM_H
#define MGRSGRIDFORM_H

#include <QWidget>
#include <QStatusBar>

inline void initMGRSResource(){Q_INIT_RESOURCE(mgrs);}

namespace Ui {
class MGRSGridForm;
class SearchForm;
}
class FloatingWidget;
namespace CooSysTransformation
{
    const int g_iBLH = 0;   ///< index of BLH coordinates
    const int g_iMGRS = 1;  ///< index of MGRS coordinates

    const QString g_strBLH = QObject::tr("BLH");    ///< name of BLH coordinates
    const QString g_strMGRS = QObject::tr("MGRS");  ///< name of MGRS coordinates
};

/**
        CLASS       : SActCooSystemParams
\brief  DESCRIPTION : Structure to store the data about active coordinate system
*/
struct SActCooSystemParams
{
    int m_iLevelIndex;  ///< current precision index
    int m_iActCooSystem;///< current coordinate system index

    SActCooSystemParams()
        : m_iLevelIndex(0),
          m_iActCooSystem(CooSysTransformation::g_iBLH)
    {

    }

    /// DESCRIPTION : Transforms from BLH to MGRS
    QString toMGRS(const double& lon, const double& lat) const;

    /// DESCRIPTION : Transforms from MGRS to BLH
    static void toGeo(const QString& mgrs, double& lon, double& lat, bool centerp) ;
};

/**
        CLASS       : MGRSGridForm
\brief  DESCRIPTION : Implements widget on status bar with coordinate system settings
*/
class MGRSGridForm : public QWidget
{
    Q_OBJECT

public:
    explicit MGRSGridForm(QStatusBar *status,QWidget *parent = nullptr);
    ~MGRSGridForm();

    void ShowOnlySerchButton();
    /// DESCRIPTION : Changes current point
    void changePoint(const double  & x,const double  & y);

    /// DESCRIPTION : Returns current parameters
    SActCooSystemParams params() const
    {
        return m_data;
    }

private:
    Ui::MGRSGridForm *ui;       ///< form
    FloatingWidget * searchWidget;

    double xPoint =-90.0,yPoint=90.0;               ///< current point ( полюс поумолчанию)
    SActCooSystemParams m_data; ///< current parameters of coordinate system

    /// DESCRIPTION : Updates label
    void updateCoords();

signals:

    /// DESCRIPTION : Emitted to update coordinates according to current parameters
    void signalChangeCoords(const SActCooSystemParams& );

    /// DESCRIPTION : Emitted to find coord at the map
    void signalSearch(const double & lat,const double & lon);

    /// DESCRIPTION : Emitted to remove coord from the map
    void signalDeletFromMap();


public slots:
    /// DESCRIPTION : Called to change precision
    void slotChangeLevel(int index);

    void slotFindPoint(const QString &point, int actCooSystem);
private slots:
    /// DESCRIPTION : Called to change coordinates
    void slotChangeCoords(int);


};

class SearchForm: public QWidget
{
     Q_OBJECT
public:
    SearchForm(QWidget *parent = nullptr);
    ~SearchForm();
signals:
    void signalFindPoint(const QString & point, int actCooSystem);
    void signalClearPoint();
private slots:
    void slotCheckEnteredText(const QString &);
    void slotSearchButtonClicked(bool);
private:

    Ui::SearchForm *ui;       ///< form
    QString strPoint;
    int m_iActCooSystem;
};

#endif // MGRSGRIDFORM_H
