#ifndef GSTPLAYER_H
#define GSTPLAYER_H

#include "QmlOverlay.h"
#include "videosettings.h"
#include "videothread.h"
#include <Vehicles/VehicleMandalaFact.h>

#include <Fact/Fact.h>
#include <QAbstractVideoSurface>
#include <QtCore>


class Vehicle;
class ImageSaver;

class GstPlayer : public Fact
{
    Q_OBJECT
    Q_PROPERTY(QAbstractVideoSurface *videoSurface MEMBER m_videoSurface)
    Q_PROPERTY(ConnectionState connectionState READ getConnectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(quint64 frameCnt READ frameCnt NOTIFY frameCntChanged)
    Q_PROPERTY(QString file_name READ file_name WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QByteArray file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QByteArray file_prev READ file_prev WRITE setFilePrev NOTIFY filePrevChanged)
    Q_PROPERTY(double screenshot_lat READ screenshot_lat WRITE setSCRLat NOTIFY screenshotLatChanged)
    Q_PROPERTY(double screenshot_lon READ screenshot_lon WRITE setSCRLon NOTIFY screenshotLonChanged)
    Q_PROPERTY(int screenshot_alt READ screenshot_alt WRITE setSCRAlt NOTIFY screenshotAltChanged)

public:
    static const int THREAD_STOP_TIMEOUT = 500;
    static const int RECONNECT_TIMEOUT = 15000;
    enum ConnectionState { STATE_UNCONNECTED, STATE_CONNECTING, STATE_CONNECTED };
    enum MediaType { mtImage, mtPreview, mtVideo ,mtBlank};

    Q_ENUM(ConnectionState)
    explicit GstPlayer(Fact *parent = nullptr);
    ~GstPlayer() override;

    Fact *f_tune;

    Fact *f_sources;
    Fact *f_addPreset;
    Fact *f_savePresets;

    Fact *f_active;
    Fact *f_record;

    Fact *f_recParams;
    Fact *f_reencoding;
    Fact *f_recCompression;
    Fact *f_recQuality;
    Fact *f_recMaxSizeTime;

    Fact *f_lowLatency;
    Fact *f_viewMode;
    Fact *f_overlay;
    Fact *f_controls;
    Fact *f_tools;
    Fact *f_screenshotBinding;

    Fact *f_LatBinding;
    Fact *f_LonBinding;
    Fact *f_AltBinding;

    ConnectionState getConnectionState() const;
    quint64 frameCnt() const;
    void setFrameCnt(quint64 v);

    QString file_name();
    void setFileName(QString &v);

    QByteArray file();
    void setFile(QByteArray &v);

    QByteArray file_prev();
    void setFilePrev(QByteArray &v);

    double screenshot_lat();
    void setSCRLat(double &v);

    double screenshot_lon();
    void setSCRLon(double &v);

    int screenshot_alt();
    void setSCRAlt(int &v);


    Q_INVOKABLE void snapshot() const;
    Q_INVOKABLE QString getSnapshotName();


    static QString getCurrentDateTime();
    static QString getMediaFileName(MediaType type,QString currentDateTime);
    static QString getMediaFileExtension(MediaType type);

private:
    QQmlEngine *m_qmlEngine = nullptr;
    ImageSaver* saver = nullptr;
    QAbstractVideoSurface *m_videoSurface = nullptr;
    VideoThread m_videoThread;
    QImage m_lastFrame;
    ConnectionState m_connectionState = STATE_UNCONNECTED;
    QTimer m_reconnectTimer;
    quint64 m_frameCnt;
    Vehicle *m_vehicle = nullptr;
    VehicleMandalaFact *m_screenshotFact;
    VehicleMandalaFact *m_LatBinding;
    VehicleMandalaFact *m_LonBinding;
    VehicleMandalaFact *m_AltBinding;

    QString m_file_name = "";
    QByteArray m_file = "";
    QByteArray m_file_prev = "";
    double m_screenshot_lat = 0;
    double m_screenshot_lon = 0;
    int m_screenshot_alt = 0;

    QMetaObject::Connection m_connection;
    const QString m_defaultPresetName;
    const QString m_defaultPresetTitle;
    QString m_sourcesGroup;
    QSettings *m_settings;

    QmlOverlay *overlay;
    VideoSettings *m_defaultPreset;
    VideoSettings *m_currentPreset;

    void setConnectionState(ConnectionState cs);

    void play();
    void stop();
    void createDefaultPreset();
    void createPreset();
    void loadPresets();
    void setActiveFirstPreset();
    void setCurrentPreset(VideoSettings *videosettings);
    void setPresetConnections(const VideoSettings *videosettings);



private slots:
    void stopAndPlay();

    void onFrameReceived(const QImage &image);
    void onActiveValueChanged();
    void onRecordValueChanged();
    void onCompressionLevelChanged();
    void onQualityLevelChanged();
    void onMaxSizeTimeChanged();
    void onErrorOccured(const QString &error) const;
    void onReconnectTimerTimeout();
    void onScreenshotBindingChanged();
    void onLatBindingChanged();
    void onLonBindingChanged();
    void onAltBindingChanged();
    void onScreenshotBindingValueChanged();

    void vehicleSelected(Vehicle *vehicle);

    void onCreatePreset();
    void onSelectPreset(VideoSettings *videosettings);
    void onRemovePreset(VideoSettings *videosettings);
    void onImageReady(QString fileName,
                      QByteArray qbaFile,
                      QByteArray qbaFilePrev,
                      double lat,
                      double lon,
                      int alt,
                      bool state,
                      QString error);

signals:
    void connectionStateChanged();
    void frameCntChanged();
    void fileNameChanged();
    void fileChanged();
    void filePrevChanged();
    void screenshotLatChanged();
    void screenshotLonChanged();
    void screenshotAltChanged();
    void overlayNumbersChanged(); //fwd from qml only
    void finishedScreenshotSaving();
};

#endif //GSTPLAYER_H
