#include "RelativePointsDB.h"
#include "Database.h"
#include <App/AppDirs.h>

/**
 * @brief RelativePointsDB::RelativePointsDB
 * @param parent
 * @param sessionName
 */
RelativePointsDB::RelativePointsDB(QObject *parent, QString sessionName)
    : DatabaseSession(parent, AppDirs::db().absoluteFilePath("relativepoints.db"), sessionName)
{

    new DBReqMakeTable(this,
                       "RelativePoints",
                       QStringList() << "key INTEGER PRIMARY KEY NOT NULL"
                                     << "title TEXT"
                                     << "descr TEXT"
                                     << "lat REAL NOT NULL"
                                     << "lon REAL NOT NULL"
                                     << "alt INTEGER"
                                     << "disp_on_map INTEGER");

    new DBReqMakeIndex(this, "RelativePoints", "lat,lon", true);

}

/**
 * @brief DBReqRelativePoints::DBReqRelativePoints
 */
DBReqRelativePoints::DBReqRelativePoints()
    : DatabaseRequest(Database::instance()->relativePoints){

}

/**
 * @brief DBReqRelativePointsFindSnapshot::run
 * @param query
 * @return
 */
bool DBReqRelativePointsFind::run(QSqlQuery &query)
{
    if (!(std::isnan(lat) || std::isnan(lon) || lat == 0.0 || lon == 0.0)) {
        query.prepare("SELECT (((?-lat)*(?-lat)*?)+((?-lon)*(?-lon))) AS dist, * FROM RelativePoints"
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
            rpID = query.value("key").toULongLong();
            title = query.value("title").toString();
        }
    }
    emit relativePointFound(rpID, title);
    return true;
}

/**
 * @brief DBReqRelativePointsSave::run
 * @param query
 * @return
 */
bool DBReqRelativePointsSave::run(QSqlQuery &query)
{
    if (std::isnan(lat) || std::isnan(lon) || lat == 0.0 || lon == 0.0)
        return false;
    if (!key) {
        //create record
        query.prepare("INSERT INTO RelativePoints"
                      " (title,descr,lat,lon,alt,disp_on_map) VALUES(?,?,?,?,?,?)");
        query.addBindValue(title);
        query.addBindValue(descr);
        query.addBindValue(lat);
        query.addBindValue(lon);
        query.addBindValue(alt);
        query.addBindValue(disp_on_map);

        if (!query.exec())
            return false;
        emit relativePointAdded(title);
    } else {
        //update record
        query.prepare("UPDATE RelativePoints SET"
                      " title=?,descr=?,"
                      " lat=?,lon=?,alt=?,disp_on_map=?"
                      " WHERE key=?");
        query.addBindValue(title);
        query.addBindValue(descr);
        query.addBindValue(lat);
        query.addBindValue(lon);
        query.addBindValue(alt);
        query.addBindValue(disp_on_map);
        query.addBindValue(key);
        if (!query.exec())
            return false;
        emit relativePointModified(title);
    }
    return true;
}
bool DBReqRelativePointsRemove::run(QSqlQuery &query)
{
    query.prepare("DELETE FROM RelativePoints WHERE key=?");
    query.addBindValue(key);
    if (!query.exec())
        return false;
    emit relativePointRemoved();
    return true;
}
