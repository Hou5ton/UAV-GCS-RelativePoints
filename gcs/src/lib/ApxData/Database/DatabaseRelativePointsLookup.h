#ifndef DATABASERELATIVEPOINTSLOOKUP_H
#define DATABASERELATIVEPOINTSLOOKUP_H

#include "DatabaseLookupModel.h"
#include "DatabaseRequest.h"
#include <Fact/Fact.h>
#include <QtCore>

class DatabaseRelativePointsLookup : public Fact
{
    Q_OBJECT
    Q_PROPERTY(DatabaseLookupModel *dbModel READ dbModel CONSTANT)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool filterEnabled READ filterEnabled WRITE setFilterEnabled NOTIFY filterEnabledChanged)

public:
    explicit DatabaseRelativePointsLookup(Fact *parent,
                                          const QString &name,
                                          const QString &title,
                                          const QString &descr,
                                          DatabaseSession *db,
                                          FactBase::Flags flags = FactBase::Flags(Group));

    Q_INVOKABLE void query(const QString &queryString,
                           const QVariantList &bindValues = QVariantList());
    Q_INVOKABLE void query(DatabaseRequest *req);

protected:
    DatabaseSession *db;
    DatabaseRequest::Records records;
    DatabaseLookupModel::ItemsList recordsItems;

    virtual bool fixItemDataThr(QVariantMap *item);
    virtual void setRecordsCount(quint64 v);
    virtual void setSelectedItem(quint64 v, bool bState);
    virtual void setQuantity(int value);
    virtual void setQuantityMap(int value);
    virtual int getSortOrder();
    virtual QString getFilter();

    QTimer modelSyncTimer;
    QMutex loadMutex;

protected slots:
    void reloadQueryResults();
    void loadRecordsItems(DatabaseLookupModel::ItemsList list);
    virtual void loadItems();


public slots:
    virtual void defaultLookup();
    void loadQueryResults(DatabaseRequest::Records records);
    void triggerItem(QVariantMap modelData,bool isChecked);
    void clickItem(QVariantMap modelData, bool isChecked);
    void showRelativePoint(QVariantMap modelData);
    void editRelativePoint(QVariantMap modelData);
    void createEditor(QVariantMap modelData);

signals:
    void itemTriggered(QVariantMap modelData,bool isChecked);
    void itemClicked(QVariantMap modelData,bool isChecked);
    void relativePointShow(QVariantMap modelData);
    void  relativePointEdit(QVariantMap modelData);
    void editorCreated(QVariantMap modelData);

    //internal loading
    void _itemsLoaded(DatabaseLookupModel::ItemsList list);


private:
     DatabaseLookupModel *m_dbModel;
     QString m_filter;
     bool m_filterEnabled;


public:
     DatabaseLookupModel *dbModel() const;
     QString filter() const;
     void setFilter(const QString &v);
     bool filterEnabled() const;
     void setFilterEnabled(const bool &v);

signals:
    void filterChanged();
    void filterEnabledChanged();
};

#endif // DATABASERELATIVEPOINTSLOOKUP_H
