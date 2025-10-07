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
#include "SnapshotsDB.h"
#include "Database.h"
#include <App/AppDirs.h>

/**
 * @brief SnapshotsDB::SnapshotsDB
 * @param parent
 * @param sessionName
 */
SnapshotsDB::SnapshotsDB(QObject *parent, QString sessionName)
    : DatabaseSession(parent, AppDirs::db().absoluteFilePath("snapshots.db"), sessionName)
{

    new DBReqMakeTable(this,
                       "Snapshots",
                       QStringList() << "key INTEGER PRIMARY KEY NOT NULL"
                                     << "title TEXT"
                                     << "descr TEXT"
                                     << "vehicle TEXT"
                                     << "lat REAL NOT NULL"
                                     << "lon REAL NOT NULL"
                                     << "alt INTEGER"
                                     << "lat_scr REAL"
                                     << "lon_scr REAL"
                                     << "date TEXT"
                                     << "time TEXT"
                                     << "filename TEXT NOT NULL"
                                     << "file TEXT NOT NULL"
                                     << "file_preview TEXT NOT NULL");

    new DBReqMakeIndex(this, "Snapshots", "filename", true);
   // new DBReqMakeIndex(this, "Snapshots", "lat,lon", true);

}

/**
 * @brief DBReqSnapshots::DBReqSnapshots
 */
DBReqSnapshots::DBReqSnapshots()
    : DatabaseRequest(Database::instance()->snapshots){

}

/**
 * @brief DBReqSnapshotsFindSnapshot::run
 * @param query
 * @return
 */
bool DBReqSnapshotsFindSnapshot::run(QSqlQuery &query)
{
    if (!(std::isnan(lat) || std::isnan(lon) || lat == 0.0 || lon == 0.0)) {
        query.prepare("SELECT (((?-lat)*(?-lat)*?)+((?-lon)*(?-lon))) AS dist, * FROM Snapshots"
                      " WHERE lat!=0 AND lon!=0 AND dist<2"
                      " ORDER BY dist ASC LIMIT 1");
        query.addBindValue(lat);
        query.addBindValue(lat);
        query.addBindValue(std::pow(std::cos(qDegreesToRadians(lat)), 2));
        query.addBindValue(lon);
        query.addBindValue(lon);
        if (!query.exec())
            return false;
        if (query.next()) {
            snapshotsID = query.value("key").toULongLong();
            snapshot = query.value("title").toString();
        }
    }
    emit snapshotFound(snapshotsID, snapshot);
    return true;
}

/**
 * @brief DBReqSnapshotsSaveSnapshot::run
 * @param query
 * @return
 */
bool DBReqSnapshotsSaveSnapshot::run(QSqlQuery &query)
{
    if (std::isnan(lat) || std::isnan(lon) || lat == 0.0 || lon == 0.0 ||
        std::isnan(lat_scr) || std::isnan(lon_scr) || lat_scr == 0.0 || lon_scr == 0.0)
        return false;
    if (!key) {
        //create snapshot record
        query.prepare("INSERT INTO Snapshots"
                      " (title,descr,vehicle,lat,lon,alt,lat_scr,lon_scr,date,time,filename,file,file_preview) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(title);
        query.addBindValue(descr);
        query.addBindValue(vehicle);
        query.addBindValue(lat);
        query.addBindValue(lon);
        query.addBindValue(alt);
        query.addBindValue(lat_scr);
        query.addBindValue(lon_scr);
        query.addBindValue(date);
        query.addBindValue(time);
        query.addBindValue(filename);
        query.addBindValue(QVariant(file));
        query.addBindValue(QVariant(file_preview));

        if (!query.exec())
            return false;
        emit snapshotAdded(title);
    } else {
        //update snapshot record
        query.prepare("UPDATE Snapshots SET"
                      " title=?,descr=?,vehicle=?,"
                      " lat=?,lon=?,alt=?,lat_scr=?,lon_scr=?,filename=?,file=?,file_preview=?"
                      " WHERE key=?");
        query.addBindValue(title);
        query.addBindValue(descr);
        query.addBindValue(vehicle);
        query.addBindValue(lat);
        query.addBindValue(lon);
        query.addBindValue(alt);
        query.addBindValue(lat_scr);
        query.addBindValue(lon_scr);
        query.addBindValue(filename);
        query.addBindValue(QVariant(file));
        query.addBindValue(QVariant(file_preview));
        query.addBindValue(key);
        if (!query.exec())
            return false;
        emit snapshotModified(title);
    }
    return true;
}
bool DBReqSnapshotsRemoveSnapshot::run(QSqlQuery &query)
{
    query.prepare("DELETE FROM Snapshots WHERE key=?");
    query.addBindValue(key);
    if (!query.exec())
        return false;
    emit snapshotRemoved();
    return true;
}

