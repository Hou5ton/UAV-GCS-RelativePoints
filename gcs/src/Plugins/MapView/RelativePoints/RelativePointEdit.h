#ifndef RELATIVEPOINTEDIT_H
#define RELATIVEPOINTEDIT_H
//=============================================================================
#include <Fact/Fact.h>
#include "LookupRelativePoints.h"
#include <QtCore>
#include <QtLocation>
class LookupRelativePoints;
//=============================================================================
class RelativePointEdit : public Fact
{
    Q_OBJECT

public:
    explicit RelativePointEdit(Fact *parent,
                      const QString &name,
                      const QString &title,
                      const QString &descr,
                      QVariantMap modelData);

    Fact *f_title;
    Fact *f_descr;

    Fact *f_latitude;
    Fact *f_longitude;
   /* Fact *f_altitude;
    Fact *f_altitude_matrix;*/

    Fact *f_sk42x;
    Fact *f_sk42y;

    Fact *a_add;
    Fact *a_edit;
    Fact *a_remove;

    Fact *f_coordinateSystem;

    LookupRelativePoints *f_relativePoints;

    QVariantMap modelData;
    void setModelData(QVariantMap v);

private:
    bool blockUpdateItemData;

private slots:
    void saveToModelData();
    void lookupRelativePoints();

    void onCoordinateSystemChanged();

public slots:
    void reset();
    void loadFromModelData();
    void updateFromEditedModelData(int i, QVariantMap v);

signals:
    void addTriggered(QVariantMap item);
    void removeTriggered(QVariantMap item);

    void relativePointEdited(QVariantMap item);
};
//=============================================================================
#endif
