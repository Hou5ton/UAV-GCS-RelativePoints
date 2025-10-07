#ifndef VIDEOSETTINGS_H
#define VIDEOSETTINGS_H

#include <Fact/Fact.h>
#include <QAbstractVideoSurface>
#include <QtCore>

class VideoSettings : public Fact
{
    Q_OBJECT

public:
    enum SourceType { stUri, stRtsp, stTcp, stUdp, stWebcam };
    enum CodecType { ctH264, ctH265, ctRawJpeg2000 };

    explicit VideoSettings(Fact *parent = nullptr,
                           const QString &name = QString(),
                           const QString &title = QString());

    Fact *f_description;
    Fact *f_sourceType;

    Fact *f_uriInput;
    Fact *f_rtspInput;
    Fact *f_rtspTcpForce;
    Fact *f_tcpInput;
    Fact *f_tcpPortInput;
    Fact *f_udpAddress;
    Fact *f_udpPort;
    Fact *f_udpCodecInput;
    Fact *f_webcamInput;

    Fact *f_selectSettings;
    Fact *f_removeSettings;

    QString inputToUri();

private:
    QStringList getAvailableWebcams();

private slots:
    void onSourceTypeChanged();
    void onDescriptionChanged();
    void onRemoveSettings();
    void onSelectSettings();
    void onErrorOccured(const QString &error) const;

signals:
    void selectSettings(VideoSettings *videosettings);
    void removeSettings(VideoSettings *videosettings);
};

#endif //VIDEOSETTINGS_H
