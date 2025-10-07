#include "gstplayer.h"

#include <Vehicles/Vehicle.h>
#include <Vehicles/VehicleMandala.h>
#include <Vehicles/Vehicles.h>

#include <App/App.h>
#include <App/AppDirs.h>
#include <App/AppLog.h>
#include <QCameraInfo>
#include <QVideoSurfaceFormat>
#include <QtQml>
#include "imagesaver.h"

using namespace std::placeholders;

GstPlayer::GstPlayer(Fact *parent)
    : Fact(parent,
           QString(PLUGIN_NAME).toLower(),
           tr("Video"),
           tr("Camera link and streaming"),
           Group,
           "video")
    , m_frameCnt(0)
    , m_defaultPresetName("video_settings")
    , m_defaultPresetTitle("Video settings")
    , m_currentPreset(nullptr)
{
    if (!AppDirs::images().exists())
        AppDirs::images().mkpath(AppDirs::images().absolutePath());
    if (!AppDirs::video().exists())
        AppDirs::video().mkpath(AppDirs::video().absolutePath());

    App::instance()->engine()->rootContext()->setContextProperty("gstplayer",this);

    qmlRegisterType<GstPlayer>("GstPlayer", 1, 0, "GstPlayer");

    saver = new ImageSaver();

    m_settings = AppPrefs::instance()->settings();

    f_tune = new Fact(this, "tune", tr("Tune"), tr("Video stream settings"), Group, "tune");

    f_sources = new Fact(f_tune,
                         "sources",
                         tr("Sources"),
                         tr("Video sources selection"),
                         Group,
                         "settings-box");

    m_sourcesGroup = AppRoot::instance()->f_tools->path() + "." + f_sources->path();

    if (!m_settings->childGroups().contains(m_sourcesGroup))
        createDefaultPreset();
    else
        loadPresets();

    f_addPreset = new Fact(f_sources,
                           "add_source",
                           tr("Add source"),
                           tr("Add source"),
                           Action | Apply,
                           "plus-circle");


    f_active = new Fact(f_tune,
                        "running",
                        tr("Active"),
                        tr("Receive video stream"),
                        Bool,
                        "video-input-antenna");

    f_record
        = new Fact(f_tune, "record", tr("Record"), tr("Save stream to file"), Bool, "record-rec");

    f_recParams = new Fact(f_tune,
                           "rec_params",
                           tr("Record configuration"),
                           tr("Encoder parameters"),
                           Group,
                           "tune");
    f_recCompression = new Fact(f_recParams,
                                "rec_compression",
                                tr("Compression level"),
                                tr("Bigger is better, but more resources required"),
                                PersistentValue | Int,
                                "zip-box");
    f_recCompression->setDefaultValue(1);
    f_recCompression->setMin(1);
    f_recCompression->setMax(9);
    f_recQuality = new Fact(f_recParams,
                            "rec_quality",
                            tr("Quality level"),
                            tr("Bigger is better, but more disk space required"),
                            PersistentValue | Int,
                            "quality-high");
    f_recQuality->setDefaultValue(10);
    f_recQuality->setMin(1);
    f_recQuality->setMax(10);

    f_reencoding = new Fact(f_recParams,
                            "reencoding",
                            tr("Record overlay"),
                            tr("Video reencoding"),
                            Bool | PersistentValue,
                            "film");

    f_recMaxSizeTime = new Fact(f_recParams,
                                "rec_maxsizetime",
                                tr("Split video"),
                                tr("Split video into segments"),
                                PersistentValue | Enum,
                                "scissors-cutting");
    f_recMaxSizeTime->setEnumStrings({tr("Disable"), tr("10 minutes"), tr("30 minutes")});
    f_recMaxSizeTime->setDefaultValue(0);

    f_lowLatency = new Fact(f_tune,
                            "low_latency",
                            tr("Low latency"),
                            tr("Disable timestamp synchronization"),
                            Bool | PersistentValue,
                            "speedometer");
    f_lowLatency->setDefaultValue(true);

    f_viewMode = new Fact(f_tune,
                          "view_mode",
                          tr("Fit to view"),
                          tr("Scale video to fit view"),
                          Enum | PersistentValue,
                          "fit-to-page-outline");
    f_viewMode->setEnumStrings({tr("Fit"), tr("Scale"), tr("Full")});
    f_viewMode->setDefaultValue(tr("Full"));

    // overlay
    f_overlay = new Fact(f_tune,
                         "overlay",
                         tr("Overlay"),
                         tr("Show additional info on video"),
                         Group,
                         "image-plus");
    Fact *f;

    f = new Fact(f_overlay, "aim", tr("Aim"), "", Enum | PersistentValue, "crosshairs");
    f->setEnumStrings({tr("none"), tr("crosshair"), tr("rectangle")});
    f->setDefaultValue(tr("rectangle"));

    f = new Fact(f_overlay,
                 "gimbal_yaw_var",
                 tr("Gimbal yaw"),
                 tr("Gimbal yaw position variable"),
                 Mandala | PersistentValue);
    f->setDefaultValue("cam_yaw");

    f = new Fact(f_overlay,
                 "gimbal_pitch_var",
                 tr("Gimbal pitch"),
                 tr("Gimbal pitch position variable"),
                 Mandala | PersistentValue);
    f->setDefaultValue("cam_pitch");

    // controls
    f_controls = new Fact(f_tune,
                          "controls",
                          tr("Controls"),
                          tr("Enable cam controls"),
                          Group | Bool | PersistentValue,
                          "camera-control");
    f_controls->setDefaultValue("true");

    f = new Fact(f_controls,
                 "control_x",
                 tr("Control X"),
                 tr("Horizontal axis control variable"),
                 Mandala | PersistentValue);
    f->setDefaultValue("camcmd_yaw");
    f = new Fact(f_controls,
                 "control_sx",
                 tr("Span X"),
                 tr("Horizontal axis span"),
                 Int | PersistentValue);
    f->setDefaultValue(180);

    f = new Fact(f_controls,
                 "control_y",
                 tr("Control Y"),
                 tr("Vertical axis control variable"),
                 Mandala | PersistentValue);
    f->setDefaultValue("camcmd_pitch");
    f = new Fact(f_controls,
                 "control_sy",
                 tr("Span Y"),
                 tr("Vertical axis span"),
                 Int | PersistentValue);
    f->setDefaultValue(90);

    f = new Fact(f_controls,
                 "rev_y",
                 tr("Reverse Y"),
                 tr("Invert vertical control"),
                 Bool | PersistentValue);

    f = new Fact(f_controls,
                 "rev_zoom",
                 tr("Reverse zoom"),
                 tr("Invert zoom by mouse wheel"),
                 Bool | PersistentValue);

    f_screenshotBinding = new Fact(f_tune,
                                   "sshot_binding",
                                   tr("Screenshot binding"),
                                   tr("Screenshot binding to mandala variable"),
                                   Mandala | PersistentValue,
                                   "link-plus");
    f_screenshotBinding->setDefaultValue("VM32");

    f_LatBinding = new Fact(f_tune,
                            "slat_binding",
                            tr("Screenshot Latitude binding"),
                            tr("Screenshot Latitude binding to mandala variable"),
                            Mandala | PersistentValue,
                            "link-plus");
    f_LatBinding->setDefaultValue("gps_lat");

    f_LonBinding = new Fact(f_tune,
                           "slon_binding",
                           tr("Screenshot Longitude binding"),
                           tr("Screenshot Longitude binding to mandala variable"),
                           Mandala | PersistentValue,
                           "link-plus");
   f_LonBinding->setDefaultValue("gps_lon");

    f_AltBinding = new Fact(f_tune,
                            "salt_binding",
                            tr("Screenshot Altitude binding"),
                            tr("Screenshot Altitude binding to mandala variable"),
                            Mandala | PersistentValue,
                            "link-plus");
    f_AltBinding->setDefaultValue("gps_hmsl");

    // controls menu
    f_tools = new Fact(f_tune, "tools", tr("Tools"), tr("Camera tools"), Action, "camera-plus");
    new Fact(f_tools, "power_payload");
    new Fact(f_tools, "cam_mode");
    new Fact(f_tools, "cam_ch");
    new Fact(f_tools, "cam_zoom");
    new Fact(f_tools, "cam_focus");
    new Fact(f_tools, "cambias_roll");
    new Fact(f_tools, "cambias_pitch");
    new Fact(f_tools, "cambias_yaw");
    new Fact(f_tools, "cam_opt_PF");
    new Fact(f_tools, "cam_opt_NIR");
    new Fact(f_tools, "cam_opt_DSP");
    new Fact(f_tools, "cam_opt_FMI");
    new Fact(f_tools, "cam_opt_FM");
    new Fact(f_tools, "cam_opt_laser");
    new Fact(f_tools, "cam_src");
    new Fact(f_tools, "cams");
    new Fact(f_tools, "cam_ctrb_shtr");
    new Fact(f_tools, "cam_ctrb_arm");
    new Fact(f_tools, "cam_ctrb_rec");
    new Fact(f_tools, "cam_ctrb_zin");
    new Fact(f_tools, "cam_ctrb_zout");
    new Fact(f_tools, "cam_ctrb_aux");
    new Fact(f_tools, "cam_tperiod");

    connect(Vehicles::instance(), &Vehicles::vehicleSelected, this, &GstPlayer::vehicleSelected);
    m_vehicle = Vehicles::instance()->current();
    vehicleSelected(m_vehicle);

    if(m_vehicle){
        m_screenshotFact = m_vehicle->f_mandala->factByName(
            f_screenshotBinding->defaultValue().toString());

        m_LatBinding = m_vehicle->f_mandala->factByName(
                    f_LatBinding->defaultValue().toString());
        m_LonBinding = m_vehicle->f_mandala->factByName(
                    f_LonBinding->defaultValue().toString());
        m_AltBinding = m_vehicle->f_mandala->factByName(
                    f_AltBinding->defaultValue().toString());
    }


    connect(f_addPreset, &Fact::triggered, this, &GstPlayer::onCreatePreset);
    connect(&m_videoThread, &VideoThread::frameReceived, this, &GstPlayer::onFrameReceived);
    connect(&m_videoThread, &VideoThread::errorOccured, this, &GstPlayer::onErrorOccured);
    connect(&m_reconnectTimer, &QTimer::timeout, this, &GstPlayer::onReconnectTimerTimeout);
    connect(f_active, &Fact::valueChanged, this, &GstPlayer::onActiveValueChanged);
    connect(f_record, &Fact::valueChanged, this, &GstPlayer::onRecordValueChanged);
    connect(f_recQuality, &Fact::valueChanged, this, &GstPlayer::onQualityLevelChanged);
    connect(f_recCompression, &Fact::valueChanged, this, &GstPlayer::onCompressionLevelChanged);
    connect(f_recMaxSizeTime, &Fact::valueChanged, this, &GstPlayer::onMaxSizeTimeChanged);
    connect(f_screenshotBinding, &Fact::valueChanged, this, &GstPlayer::onScreenshotBindingChanged);
    connect(f_LatBinding, &Fact::valueChanged, this, &GstPlayer::onLatBindingChanged);
    connect(f_LonBinding, &Fact::valueChanged, this, &GstPlayer::onLonBindingChanged);
    connect(f_AltBinding, &Fact::valueChanged, this, &GstPlayer::onAltBindingChanged);

    connect(saver,&ImageSaver::imageReady,this,&GstPlayer::onImageReady);

    m_connection = QObject::connect(m_screenshotFact,
                                    &VehicleMandalaFact::valueChanged,
                                    this,
                                    &GstPlayer::onScreenshotBindingValueChanged);

    m_reconnectTimer.setInterval(RECONNECT_TIMEOUT);
    m_reconnectTimer.setSingleShot(true);

    overlay = new QmlOverlay(this);
    m_videoThread.setOverlayCallback(std::bind(&QmlOverlay::cb_drawOverlay, overlay, _1));

    App::instance()->engine()->jsexec("function btn_Snapshot() { apx.tools.videostreaming.snapshot(); }");

    loadQml(QString("qrc:/%1/VideoPlugin.qml").arg(PLUGIN_NAME));

    onCompressionLevelChanged();
    onQualityLevelChanged();
    onMaxSizeTimeChanged();
}

GstPlayer::~GstPlayer()
{
    if (m_videoThread.isRunning())
        stop();
}

void GstPlayer::vehicleSelected(Vehicle *vehicle)
{
    if(m_vehicle)
        m_vehicle = nullptr;
    m_vehicle = vehicle;
    VehicleMandala *m = vehicle->f_mandala;
    for (int i = 0; i < f_tools->size(); ++i) {
        Fact *f = f_tools->child(i);
        f->bind(m->child(f->name()));
    }
}

GstPlayer::ConnectionState GstPlayer::getConnectionState() const
{
    return m_connectionState;
}

void GstPlayer::snapshot() const
{
   // QElapsedTimer timer;
   // timer.start();

    QString qsFileName = getCurrentDateTime();

    QImage image = m_lastFrame.copy();
    QImage img  = overlay->getSnapshotOverlay(image.size());
    if (!img.isNull()) {
        QPainter painter(&image);
        painter.drawImage(QPoint(0, 0), img);
    }

    if(m_vehicle){
        double lat = m_vehicle->f_mandala->factByName(m_LatBinding->name())->value().toDouble();
        double lon = m_vehicle->f_mandala->factByName(m_LonBinding->name())->value().toDouble();
        int alt = m_vehicle->f_mandala->factByName(m_AltBinding->name())->value().toInt();

        saver->requestImageSave(image,
                                qsFileName,
                                getMediaFileName(mtImage,qsFileName),
                                getMediaFileName(mtPreview,qsFileName),
                                lat,
                                lon,
                                alt);

    } else{
        apxMsg() << tr("Invalid vehicle");
    }
   // apxMsg() << QString("timer: %1").arg(timer.elapsed());
}

QString GstPlayer::getCurrentDateTime()
{
     QString currentDateTime = QDateTime::currentDateTime().toString("dd_MM_yyyyThh_mm_ss_zzz");
     return  currentDateTime;
}

QString GstPlayer::getMediaFileName(MediaType type,QString currentDateTime)
{
    QString base;
    QString filename;

    if (type == mtImage) {
        base = AppDirs::images().absolutePath();
    } else if (type == mtVideo) {
        base = AppDirs::video().absoluteFilePath(QDateTime::currentDateTime().toString("yyyy_MM_dd"));
    } else if (type == mtBlank) {
        base = AppDirs::images().absolutePath();
    } else if (type == mtPreview) {
        base = AppDirs::previews().absolutePath();
    }

    if (type == mtVideo)
        filename = QString("%1/%05d_%2.%3").arg(base, currentDateTime, getMediaFileExtension(type));
    else if(type != mtBlank)
        filename = QString("%1/%2.%3").arg(base, currentDateTime, getMediaFileExtension(type));
    else
        filename = QString("%1/%2").arg(base, currentDateTime);

    return filename;
}
QString GstPlayer::getMediaFileExtension(MediaType type)
{
    QString qsExt = "";
    if (type == mtImage) {
        qsExt = "png";
    }if (type == mtPreview) {
        qsExt = "png";
    } else if (type == mtVideo) {
        qsExt = "mp4";
    }
    return qsExt;
}


void GstPlayer::setConnectionState(GstPlayer::ConnectionState cs)
{
    if (m_connectionState != cs) {
        m_connectionState = cs;
        emit connectionStateChanged();
    }
}

void GstPlayer::play()
{
    setFrameCnt(0);
    setConnectionState(STATE_CONNECTING);
    QString uri = m_currentPreset->inputToUri();
    m_videoThread.setUri(uri);
    m_videoThread.setLowLatency(f_lowLatency->value().toBool());
    m_videoThread.setReencoding(f_reencoding->value().toBool());
    m_videoThread.start();
    m_reconnectTimer.start();
}

void GstPlayer::stop()
{
    m_videoThread.stop();
    if (!m_videoThread.wait(THREAD_STOP_TIMEOUT)) {
        onErrorOccured("VideoThread stop timeout, try to force stop...");
        m_videoThread.terminate();
    }

    QImage splash(m_lastFrame.size(), QImage::Format_RGB32);
    splash.fill(Qt::black);
    onFrameReceived(splash);

    setConnectionState(STATE_UNCONNECTED);
    m_reconnectTimer.stop();
}

void GstPlayer::stopAndPlay()
{
    stop();
    play();
}

void GstPlayer::onFrameReceived(const QImage &image)
{
    m_reconnectTimer.start();

    if (getConnectionState() == STATE_CONNECTING)
        setConnectionState(STATE_CONNECTED);

    setFrameCnt(frameCnt() + 1);
    m_lastFrame = image;
    if (m_videoSurface) {
        if (image.size() != m_videoSurface->surfaceFormat().frameSize())
            m_videoSurface->stop();

        if (!m_videoSurface->isActive())
            m_videoSurface->start(QVideoSurfaceFormat(image.size(), QVideoFrame::Format_RGB32));

        QVideoFrame frame(image);
        if (!m_videoSurface->present(frame))
            onErrorOccured("Can't present frame on surface");
    }
}

void GstPlayer::onActiveValueChanged()
{
    bool active = f_active->value().toBool();
    if (active)
        play();
    else
        stop();
}

void GstPlayer::onRecordValueChanged()
{
    bool record = f_record->value().toBool();
    m_videoThread.setRecording(record);
}

void GstPlayer::onCompressionLevelChanged()
{
    m_videoThread.setCompressionLevel(f_recCompression->value().toInt());
}

void GstPlayer::onQualityLevelChanged()
{
    m_videoThread.setQualityLevel(f_recQuality->value().toInt());
}

void GstPlayer::onMaxSizeTimeChanged()
{
    int value = f_recMaxSizeTime->value().toInt();
    int minutes = 0;

    switch (value) {
    case 1: { minutes = 10; break; }
    case 2: { minutes = 30; break; }
    default: { break; }
    }

    unsigned long long nanoseconds = minutes * 60;
    nanoseconds *= 1000;
    nanoseconds *= 1000;
    nanoseconds *= 1000;

    m_videoThread.setMaxSizeTime(nanoseconds);
}

void GstPlayer::onErrorOccured(const QString &error) const
{
    apxMsgW() << error;
}

void GstPlayer::onReconnectTimerTimeout()
{
    onErrorOccured("Connection timeout");
    stop();
    play();
}

void GstPlayer::onScreenshotBindingChanged()
{
    if(m_vehicle){
        m_screenshotFact = m_vehicle->f_mandala->factByName(f_screenshotBinding->text());
        QObject::disconnect(m_connection);
        m_connection = QObject::connect(m_screenshotFact,
                                        &VehicleMandalaFact::valueChanged,
                                        this,
                                        &GstPlayer::onScreenshotBindingValueChanged);
    }
}
void GstPlayer::onLatBindingChanged()
{
    if(m_vehicle)
        m_LatBinding = m_vehicle->f_mandala->factByName(f_LatBinding->text());
}
void GstPlayer::onLonBindingChanged()
{
    if(m_vehicle)
        m_LonBinding = m_vehicle->f_mandala->factByName(f_LonBinding->text());
}
void GstPlayer::onAltBindingChanged()
{
    if(m_vehicle)
        m_AltBinding = m_vehicle->f_mandala->factByName(f_AltBinding->text());
}
void GstPlayer::onImageReady(QString fileName,
                             QByteArray qbaFile,
                             QByteArray qbaFilePrev,
                             double lat,
                             double lon,
                             int alt,
                             bool state,
                             QString error)
{
    if(state){
         apxMsg() << tr("Screenshot") +
                     " " +
                     fileName +
                     " " +
                      tr("saved");
         setFileName(fileName);
         setFile(qbaFile);
         setFilePrev(qbaFilePrev);
         setSCRLat(lat);
         setSCRLon(lon);
         setSCRAlt(alt);

         emit finishedScreenshotSaving();

    }else{
        QString qs = tr("Wrong Screenshot");
        if(error != ""){
            qs += " (";
            qs += error;
            qs += ")";
        }
        apxMsg() << qs;
    }
}

void GstPlayer::onScreenshotBindingValueChanged()
{
    auto value = m_screenshotFact->value().toDouble();
    if (value > 0) {
        snapshot();
    }
}

quint64 GstPlayer::frameCnt() const
{
    return m_frameCnt;
}
void GstPlayer::setFrameCnt(quint64 v)
{
    if (m_frameCnt == v)
        return;
    m_frameCnt = v;
    emit frameCntChanged();
}

void GstPlayer::createDefaultPreset()
{
    auto sourceName = m_defaultPresetName + "1";
    auto sourceTitle = m_defaultPresetTitle + "#1";

    m_defaultPreset = new VideoSettings(f_sources, sourceName, sourceTitle);
    m_defaultPreset->f_removeSettings->setEnabled(false);

    setPresetConnections(m_defaultPreset);

    m_settings->beginGroup(m_sourcesGroup);
    m_settings->setValue(sourceName, 1);
    setCurrentPreset(m_defaultPreset);
    m_settings->endGroup();
}

void GstPlayer::createPreset()
{
    auto n = f_sources->size() + 1;
    auto sourceName = m_defaultPresetName + QString("%1").arg(n);
    auto sourceTitle = m_defaultPresetTitle + QString("#%1").arg(n);

    m_settings->beginGroup(m_sourcesGroup);
    if (m_settings->contains(sourceName)) {
        while (m_settings->contains(sourceName)) {
            sourceName = m_defaultPresetName + QString("%1").arg(++n);
            sourceTitle = m_defaultPresetTitle + QString("#%1").arg(n);
        }
    }

    auto source = new VideoSettings(f_sources, sourceName, sourceTitle);
    setPresetConnections(source);

    m_settings->setValue(sourceName, 0);
    m_settings->endGroup();

    // Enabling remove button in the active source if it disabled
    if (!m_currentPreset->f_removeSettings->enabled())
        m_currentPreset->f_removeSettings->setEnabled(true);
}

void GstPlayer::loadPresets()
{
    m_settings->beginGroup(m_sourcesGroup);
    auto keys = m_settings->childKeys();

    for (auto const &k : keys) {
        // Default title if the source description has not been changed
        auto sourceTitle = QString("Video settings#")
                           + k.right(k.size() - m_defaultPresetName.size());
        auto source = new VideoSettings(f_sources, k, sourceTitle);
        setPresetConnections(source);

        if (m_settings->value(k).toInt() > 0) {
            if (!m_currentPreset)
                setCurrentPreset(source);
        }

        // Disable remove button if only one source and set it active
        if (keys.size() < 2) {
            source->f_removeSettings->setEnabled(false);

            if (m_settings->value(k).toInt() < 1) {
                m_settings->setValue(k, 1);
                setCurrentPreset(source);
            }
        }
    }

    // If something is wrong with the settings values
    if (!m_currentPreset) {
        setActiveFirstPreset();
    }

    m_settings->endGroup();
}

void GstPlayer::setActiveFirstPreset()
{
    auto activePreset = qobject_cast<VideoSettings *>(f_sources->child(0));
    m_settings->setValue(activePreset->name(), 1);
    setCurrentPreset(activePreset);
}

void GstPlayer::setCurrentPreset(VideoSettings *videosettings)
{
    if (!videosettings) {
        apxMsgW() << "Can't set current settings";
        return;
    }

    m_currentPreset = videosettings;
    m_currentPreset->setActive(true);
}

void GstPlayer::setPresetConnections(const VideoSettings *videosettings)
{
    connect(videosettings, &VideoSettings::selectSettings, this, &GstPlayer::onSelectPreset);
    connect(videosettings, &VideoSettings::removeSettings, this, &GstPlayer::onRemovePreset);
}

void GstPlayer::onCreatePreset()
{
    createPreset();
}

void GstPlayer::onSelectPreset(VideoSettings *videosettings)
{
    if (!videosettings) {
        apxMsgW() << "Can't select and save settings";
        return;
    }

    m_currentPreset->setActive(false);
    setCurrentPreset(videosettings);

    m_settings->beginGroup(m_sourcesGroup);
    auto keys = m_settings->childKeys();
    for (auto const &k : keys)
        if (m_settings->value(k) != 0)
            m_settings->setValue(k, 0);

    m_settings->setValue(m_currentPreset->name(), 1);
    m_settings->endGroup();

    if (f_active->value().toBool())
        stopAndPlay();

    f_sources->trigger();
}

void GstPlayer::onRemovePreset(VideoSettings *videosettings)
{
    if (!videosettings) {
        apxMsgW() << "Can't remove settings";
        return;
    }

    auto name = videosettings->name();

    videosettings->remove();

    auto str = m_sourcesGroup + "." + name;
    m_settings->beginGroup(str);
    m_settings->remove("");
    m_settings->endGroup();

    m_settings->beginGroup(m_sourcesGroup);
    if (m_settings->value(name).toInt()) {
        setActiveFirstPreset();
        if (f_active->value().toBool())
            f_active->setValue(false);
    }
    m_settings->remove(name);

    // Disable remove button if only one source remains
    if (f_sources->size() < 2)
        m_currentPreset->f_removeSettings->setEnabled(false);

    m_settings->endGroup();
}
QString GstPlayer::getSnapshotName()
{
    return getCurrentDateTime();
}
QString GstPlayer::file_name()
{
    return m_file_name;
}
QByteArray GstPlayer::file()
{
    return m_file;
}

QByteArray GstPlayer::file_prev()
{
   return m_file_prev;
}
void GstPlayer::setFileName(QString &v)
{
    m_file_name = v;
    emit fileNameChanged();
}
void GstPlayer::setFile(QByteArray &v)
{
    m_file = v;
    emit fileChanged();
}
void GstPlayer::setFilePrev(QByteArray &v){
    m_file_prev = v;
    emit filePrevChanged();
}

double GstPlayer::screenshot_lat()
{
     return m_screenshot_lat;
}
void GstPlayer::setSCRLat(double &v)
{
    m_screenshot_lat = v;
    emit screenshotLatChanged();
}

double GstPlayer::screenshot_lon()
{
    return m_screenshot_lon;
}
void GstPlayer::setSCRLon(double &v)
{
    m_screenshot_lon = v;
    emit screenshotLonChanged();
}
int GstPlayer::screenshot_alt()
{
    return m_screenshot_alt;
}
void GstPlayer::setSCRAlt(int &v)
{
    m_screenshot_alt = v;
    emit screenshotAltChanged();
}

/*void GstPlayer::getSnapshotParams()
{
    QVariantMap map;
    map.insert("filename",getCurrentDateTime());
    map.insert("lon","10");
    map.insert("lat","20");
    map.insert("hmsl","30");


    QMetaObject::invokeMethod(view.rootObject(),
                              "getSnapshotParams",
                              Q_ARG(QVariant, QVariant::fromValue(map)));


}*/
