#include "videosettings.h"

#include <App/App.h>
#include <App/AppDirs.h>
#include <App/AppLog.h>
#include <QCameraInfo>
#include <QVideoSurfaceFormat>
#include <QtQml>

using namespace std::placeholders;

VideoSettings::VideoSettings(Fact *parent, const QString &name, const QString &title)
    : Fact(parent, name, title, tr("Video sources settings"), Group, "settings")
{
    f_description = new Fact(this,
                             "description",
                             tr("Description"),
                             tr("Source name"),
                             Text | PersistentValue,
                             "rename-box");
    f_description->setDefaultValue(this->title());

    f_sourceType = new Fact(this,
                            "source_type",
                            tr("Source"),
                            tr("Source type"),
                            Enum | PersistentValue);
    f_sourceType->setEnumStrings({"URI", "RTSP", "TCP", "UDP", "Webcam"});

    f_rtspInput = new Fact(this, "rtsp_input", tr("URL"), tr("rtsp://<..>"), Text | PersistentValue);
    f_rtspTcpForce = new Fact(this,
                              "rtspforcetcp_input",
                              tr("Force tcp"),
                              "",
                              Bool | PersistentValue);
    f_tcpInput = new Fact(this, "tcp_input", tr("IP"), tr("IP address"), Text | PersistentValue);
    f_tcpPortInput = new Fact(this,
                              "tcpport_input",
                              tr("Port"),
                              tr("Port number"),
                              Int | PersistentValue);

    f_udpAddress = new Fact(this,
                            "udp_address",
                            tr("Address"),
                            tr("IP Address"),
                            Text | PersistentValue);
    f_udpAddress->setDefaultValue("0.0.0.0");

    f_udpPort = new Fact(this, "udp_port", tr("Port"), tr("Port number"), Int | PersistentValue);
    f_udpCodecInput = new Fact(this, "udpcodec_input", tr("Codec"), "", Enum | PersistentValue);
    f_udpCodecInput->setEnumStrings({"H264", "H265", "Raw-JPEG2000"});

    f_webcamInput = new Fact(this, "webcam_input", tr("Webcam"), "", Enum | PersistentValue);
    f_webcamInput->setEnumStrings(getAvailableWebcams());

    f_uriInput = new Fact(this,
                          "uri_input",
                          tr("URI"),
                          tr("rtsp://<..>, file://<..>, etc."),
                          Text | PersistentValue);
    f_uriInput->setDefaultValue("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov");

    f_selectSettings = new Fact(this,
                                "select_save",
                                tr("Select"),
                                tr("Select settings"),
                                Action | Apply | CloseOnTrigger);
    f_removeSettings = new Fact(this,
                                "remove",
                                tr("Remove"),
                                tr("Remove settings"),
                                Action | Remove | ShowDisabled);

    connect(f_sourceType, &Fact::valueChanged, this, &VideoSettings::onSourceTypeChanged);
    connect(f_description, &Fact::valueChanged, this, &VideoSettings::onDescriptionChanged);
    connect(f_removeSettings, &Fact::triggered, this, &VideoSettings::onRemoveSettings);
    connect(f_selectSettings, &Fact::triggered, this, &VideoSettings::onSelectSettings);

    onSourceTypeChanged();
}

QString VideoSettings::inputToUri()
{
    QString result;
    if (f_sourceType->value().toInt() == stUri) {
        result = f_uriInput->value().toString();
    } else if (f_sourceType->value().toInt() == stRtsp) {
        QString value = f_rtspInput->value().toString();
        if (!value.contains("rtspt://") && f_rtspTcpForce->value().toBool()) {
            if (value.indexOf("://") == -1)
                result = "rtspt://" + value;
            else
                result = "rtspt://" + value.remove(0, value.indexOf("://") + 3);
        } else if (!value.contains("://"))
            result = "rtsp://" + value;
        else
            result = value;
    } else if (f_sourceType->value().toInt() == stTcp) {
        QString host = f_tcpInput->value().toString();
        int port = f_tcpPortInput->value().toInt();
        result = QString("tcp://%1:%2").arg(host).arg(port);
    } else if (f_sourceType->value().toInt() == stUdp) {
        QString address = f_udpAddress->value().toString();
        int port = f_udpPort->value().toInt();
        QString codec;
        if (f_udpCodecInput->value().toInt() == ctH264)
            codec = "h264";
        else if (f_udpCodecInput->value().toInt() == ctH265)
            codec = "h265";
        else if (f_udpCodecInput->value().toInt() == ctRawJpeg2000)
            codec = "raw-jpeg2000";
        QString uri = QString("udp://%1:%2?codec=%3").arg(address).arg(port).arg(codec);
        result = uri;
    } else if (f_sourceType->value().toInt() == stWebcam) {
#ifdef Q_OS_LINUX
        QString camDescr = f_webcamInput->enumText(f_webcamInput->value().toInt());
        auto cameras = QCameraInfo::availableCameras();
        auto res = std::find_if(cameras.begin(), cameras.end(), [camDescr](auto c) {
            return camDescr == c.description();
        });
        if (res == cameras.end()) {
            onErrorOccured("Can't find webcam");
        } else {
            result = QString("v4l2://%1").arg(res->deviceName());
        }
#endif
#ifdef Q_OS_MAC
        result = QString("avf://index%1").arg(f_webcamInput->value().toInt());
#endif
    }
    return result;
}

QStringList VideoSettings::getAvailableWebcams()
{
    auto cameras = QCameraInfo::availableCameras();
    QStringList ids;
    std::transform(cameras.begin(), cameras.end(), std::back_inserter(ids), [](auto c) {
        return c.description();
    });
    return ids;
}

void VideoSettings::onSourceTypeChanged()
{
    f_uriInput->setVisible(false);
    f_rtspInput->setVisible(false);
    f_rtspTcpForce->setVisible(false);
    f_tcpInput->setVisible(false);
    f_tcpPortInput->setVisible(false);
    f_udpAddress->setVisible(false);
    f_udpPort->setVisible(false);
    f_udpCodecInput->setVisible(false);
    f_webcamInput->setVisible(false);

    if (f_sourceType->value().toInt() == stUri)
        f_uriInput->setVisible(true);
    else if (f_sourceType->value().toInt() == stRtsp) {
        f_rtspInput->setVisible(true);
        f_rtspTcpForce->setVisible(true);
    } else if (f_sourceType->value().toInt() == stTcp) {
        f_tcpInput->setVisible(true);
        f_tcpPortInput->setVisible(true);
    } else if (f_sourceType->value().toInt() == stUdp) {
        f_udpAddress->setVisible(true);
        f_udpPort->setVisible(true);
        f_udpCodecInput->setVisible(true);
    } else if (f_sourceType->value().toInt() == stWebcam) {
        f_webcamInput->setEnumStrings(getAvailableWebcams());
        f_webcamInput->setVisible(true);
    }
}

void VideoSettings::onDescriptionChanged()
{
    this->setTitle(f_description->value().toString());
}

void VideoSettings::onRemoveSettings()
{
    emit removeSettings(this);
}

void VideoSettings::onSelectSettings()
{
    emit selectSettings(this);
}

void VideoSettings::onErrorOccured(const QString &error) const
{
    apxMsgW() << error;
}
