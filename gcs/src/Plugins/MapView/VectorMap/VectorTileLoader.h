/*
 * Copyright (C) 2011 Aliaksei Stratsilatau <sa@uavos.com>
 *
 * This file is part of the UAV Open System Project
 *  http://www.uavos.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 *
 */
#ifndef VectorTileLoader_H
#define VectorTileLoader_H
#include "VectorMapsDB.h"
#include <KartaInterface.h>
#include <mconfig.h>
#include <Fact/Fact.h>
#include <QtCore>
#include <QtNetwork>
#include <QGeoCoordinate>
//=============================================================================


class VectorTileLoader : public Fact
{
    Q_OBJECT
    Q_ENUMS(MapID)

    Q_PROPERTY(int requestCount READ requestCount NOTIFY requestCountChanged)

public:
    VectorTileLoader(Fact *parent = nullptr);
    ~VectorTileLoader();

    static VectorTileLoader *instance() { return _instance; }


    enum MapID {
        GoogleHybrid,
        GoogleSatellite,
    };
    Q_ENUM(MapID)

    static inline quint64 uid(quint8 type, quint8 level, quint32 x, quint32 y)
    {
        return ((quint64) type << 56) | ((quint64) level << 48) | ((quint64) x << 24) | (quint64) y;
    }
    static inline quint8 type(quint64 uid) { return (uid >> 56) & (((quint64) 1 << 8) - 1); }
    static inline quint8 level(quint64 uid) { return (uid >> 48) & (((quint64) 1 << 8) - 1); }
    static inline quint32 x(quint64 uid) { return (uid >> 24) & (((quint64) 1 << 24) - 1); }
    static inline quint32 y(quint64 uid) { return uid & (((quint64) 1 << 24) - 1); }
    static inline quint64 dbHash(quint64 uid) { return uid & (((quint64) 1 << 56) - 1); }

protected:
    void updateStatus();

private:
    static VectorTileLoader *_instance;
    VectorMapsDB *db;

    //downloader
    //QNetworkAccessManager *net;
    //QByteArray userAgent;
    //QString language;
    Fact *f_prefs;
    Fact *f_loadExtraMap;
    Fact *f_loadSXF;
    QString versionMaps;    
    QHash<QNetworkReply *, quint64> reqMap;

    QSharedPointer<MConfig> m_config;
    QSharedPointer<MKartaInterface> m_ki;
    bool initMap();
    static void loadMapLog(const std::string &msg);
    void convertCooSys(MCoo *from, MCoo *to, MKartaInterface *ki);

    QList<quint64> pendingDownloads;
    QList<quint64> downloads;


    bool checkImage(const QByteArray &data);

    int requestCount() { return reqMap.size(); }
    QString getMapPath(const int& scale, const QString& sheetName);

    void slotLoadMapInPoint();
    bool isExtraMapLoaded(const QGeoCoordinate &clickPoint);
private slots:
    void render(quint64 uid);
    void createSettingsFact();
    void loadExtraMap();

    void updateLoadExtraMapAction();
    void changeToolsVisibility();
    void updateLayersFact();
    void layerVisibilityUpdate();
    void loadSXF();
public slots:
    void loadTile(quint64 uid);
    void loadCancel(quint64 uid);
    void abort();

signals:
    void tileLoaded(quint64 uid, QByteArray data);
    void tileError(quint64 uid, QString errorString);
    void requestEngieClearCache();
    void requestCountChanged();
    void mapFilesChanged();
};
//=============================================================================
#endif
