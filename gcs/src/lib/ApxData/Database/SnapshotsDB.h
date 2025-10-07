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
#ifndef SnapshotsDB_H
#define SnapshotsDB_H

#include <Database/DatabaseSession.h>
//#include <Dictionary/DictMission.h>
#include <QtCore>

/**
 * @brief The SnapshotsDB class
 */
class SnapshotsDB : public DatabaseSession
{
    Q_OBJECT
public:
    /**
     * @brief SnapshotsDB
     * @param parent
     * @param sessionName
     */
    explicit SnapshotsDB(QObject *parent, QString sessionName);
};

/**
 * @brief The DBReqSnapshots class
 */
class DBReqSnapshots : public DatabaseRequest
{
    Q_OBJECT
public:
    /**
     * @brief DBReqSnapshots
     */
    explicit DBReqSnapshots();
};

/**
 * @brief The DBReqSnapshotsFindSnapshot class
 */
class DBReqSnapshotsFindSnapshot : public DBReqSnapshots
{
    Q_OBJECT
public:
    explicit DBReqSnapshotsFindSnapshot(double lat, double lon)
        : DBReqSnapshots()
        , snapshotsID(0)
        , lat(lat)
        , lon(lon)
    {}
    bool run(QSqlQuery &query);
    //result
    quint64 snapshotsID;
    QString snapshot;

private:
    double lat;
    double lon;
signals:
    void snapshotFound(quint64 snapshotID, QString snapshot);
};

/**
 * @brief The DBReqSnapshotsSaveSnapshot class
 */
class DBReqSnapshotsSaveSnapshot : public DBReqSnapshots
{
    Q_OBJECT
public:
    explicit DBReqSnapshotsSaveSnapshot(QString title,
                                       QString descr,
                                       QString vehicle,
                                       double lat,
                                       double lon,
                                       int alt,
                                       double lat_scr,
                                       double lon_scr,
                                       QString date,
                                       QString time,
                                       QString filename,
                                       QByteArray file,
                                       QByteArray file_preview,
                                       quint64 key = 0)
        : DBReqSnapshots()
        , title(title)
        , descr(descr)
        , vehicle(vehicle)
        , lat(lat)
        , lon(lon)
        , alt(alt)
        , lat_scr(lat_scr)
        , lon_scr(lon_scr)
        , date(date)
        , time(time)
        , filename(filename)
        , file(file)
        , file_preview(file_preview)
        , key(key)
    {}

private:
    QString title;
    QString descr;
    QString vehicle;
    double lat;
    double lon;
    int alt;
    double lat_scr;
    double lon_scr;
    QString date;
    QString time;
    QString filename;
    QByteArray file;
    QByteArray file_preview;
    quint64 key;

protected:
    bool run(QSqlQuery &query);
signals:
    void snapshotAdded(QString title);
    void snapshotModified(QString title);
};


class DBReqSnapshotsRemoveSnapshot : public DBReqSnapshots
{
    Q_OBJECT
public:
    explicit DBReqSnapshotsRemoveSnapshot(quint64 key)
        : DBReqSnapshots()
        , key(key)
    {}

private:
    quint64 key;

protected:
    bool run(QSqlQuery &query);
signals:
    void snapshotRemoved();
};

#endif
