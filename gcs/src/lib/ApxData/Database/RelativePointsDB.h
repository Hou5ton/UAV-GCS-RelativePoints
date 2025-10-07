#ifndef RELATIVEPOINTSDB_H
#define RELATIVEPOINTSDB_H

#include <Database/DatabaseSession.h>
#include <QtCore>

class RelativePointsDB : public DatabaseSession
{
    Q_OBJECT
public:
    RelativePointsDB(QObject *parent, QString sessionName);
};

/**
 * @brief The DBReqRelativePoints class
 */
class DBReqRelativePoints : public DatabaseRequest
{
    Q_OBJECT
public:
    /**
     * @brief DBReqRelativePoints
     */
    explicit DBReqRelativePoints();
};

/**
 * @brief The DBReqRelativePointsFindSnapshot class
 */
class DBReqRelativePointsFind : public DBReqRelativePoints
{
    Q_OBJECT
public:
    explicit DBReqRelativePointsFind(double lat, double lon)
        : DBReqRelativePoints()
        , rpID(0)
        , lat(lat)
        , lon(lon)
    {}
    bool run(QSqlQuery &query);
    //result
    quint64 rpID;
    QString title;

private:
    double lat;
    double lon;
signals:
    void relativePointFound(quint64 rpID, QString title);
};

/**
 * @brief The DBReqRelativePointsSave class
 */
class DBReqRelativePointsSave : public DBReqRelativePoints
{
    Q_OBJECT
public:
    explicit DBReqRelativePointsSave(QString title,
                                       QString descr,
                                       double lat,
                                       double lon,
                                       int alt,
                                       int disp_on_map,
                                       quint64 key = 0)
        : DBReqRelativePoints()
        , title(title)
        , descr(descr)
        , lat(lat)
        , lon(lon)
        , alt(alt)
        , disp_on_map(disp_on_map)
        , key(key)
    {}

private:
    QString title;
    QString descr;
    double lat;
    double lon;
    int alt;
    int disp_on_map;
    quint64 key;

protected:
    bool run(QSqlQuery &query);
signals:
    void relativePointAdded(QString title);
    void relativePointModified(QString title);
};


class DBReqRelativePointsRemove : public DBReqRelativePoints
{
    Q_OBJECT
public:
    explicit DBReqRelativePointsRemove(quint64 key)
        : DBReqRelativePoints()
        , key(key)
    {}

private:
    quint64 key;

protected:
    bool run(QSqlQuery &query);
signals:
    void relativePointRemoved();
};


#endif // RELATIVEPOINTSDB_H
