#ifndef ALTITUDEMATRIX_H
#define ALTITUDEMATRIX_H

#include <QGeoCoordinate>

#include <Fact/Fact.h>

class MBmhFILE;

class AltitudeMatrix : public Fact
{
    Q_OBJECT
public:
    explicit AltitudeMatrix(Fact *parent);

    static AltitudeMatrix *instance() { return _instance; }

    Fact *f_bmhFileName;

    bool load(const QString &fileName);
    Q_INVOKABLE bool isLoaded(void) const;

    Q_INVOKABLE double getAltitudeAt(const QGeoCoordinate &p);

private:
    static AltitudeMatrix *_instance;

    MBmhFILE *bmhFile = nullptr;

private slots:
    void onBMHFileNameTriggered();
    void onBMHFileNameValueChanged();
};

#endif // ALTITUDEMATRIX_H
