#include "AltitudeMatrix.h"

#include <QFileDialog>

#include <App/AppDirs.h>
#include <App/AppLog.h>
#include <Demming/MBmhFILE.h>
#include <MString.h>
#include <BaseCl.h>
#include <Id.h>

AltitudeMatrix *AltitudeMatrix::_instance = nullptr;
AltitudeMatrix::AltitudeMatrix(Fact *parent)
    : Fact(parent, "altitudematrix", tr("Altitude Matrix"), tr("No matrix loaded"), Group, "earth-box")
{
    _instance = this;

    f_bmhFileName = new Fact(this, "bmh_filename", tr("BMH file"), tr("Select BMH file..."), Const | PersistentValue, "import");
    connect(f_bmhFileName, &Fact::triggered, this, &AltitudeMatrix::onBMHFileNameTriggered);
    connect(f_bmhFileName, &Fact::valueChanged, this, &AltitudeMatrix::onBMHFileNameValueChanged);

    f_bmhFileName->loadPresistentValue();
    QString bmhFileName = f_bmhFileName->value().toString();
    if (!bmhFileName.isEmpty()) load(bmhFileName);
}

bool AltitudeMatrix::load(const QString &fileName)
{
    if(bmhFile)
        delete bmhFile;

    bmhFile = new MBmhFILE(fileName.toStdString());
    if (!bmhFile) {
        setDescr(tr("No matrix loaded"));
        return false;
    }

    setDescr(fileName);
    return true;
}

bool AltitudeMatrix::isLoaded() const
{
    return bmhFile;
}

double AltitudeMatrix::getAltitudeAt(const QGeoCoordinate &p)
{
    if (!bmhFile) return 0;

    int alt = 0;
    MPoint point(p.longitude(), p.latitude());

    if (bmhFile && p.latitude() != 0.0 && p.longitude() != 0.0) {
        alt = bmhFile->GetHeight(point);

        if (alt == BMH_OUTOFBOUNDS || alt == BMH_NOVALUE)
            alt = 0;
    }

    return alt;
}

void AltitudeMatrix::onBMHFileNameTriggered()
{
    QString fileName = f_bmhFileName->value().toString();

    QDir defaultPath = AppDirs::db();
    if (!fileName.isEmpty())
        defaultPath = QDir(f_bmhFileName->value().toString());

    QString path = QFileDialog::getOpenFileName(nullptr,
                                                tr("Select BMH file"),
                                                defaultPath.absolutePath(),
                                                "BMH (*.bmh)");

    if (path.isEmpty()) return;

    f_bmhFileName->setValue(path);
}

void AltitudeMatrix::onBMHFileNameValueChanged()
{
    QString path = f_bmhFileName->value().toString();
    if (path.isEmpty()) return;

    load(path);
}
