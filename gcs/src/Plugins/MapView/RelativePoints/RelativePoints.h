#ifndef RELATIVEPOINTS_H
#define RELATIVEPOINTS_H
#include <Fact/Fact.h>
#include "LookupRelativePoints.h"
#include "RelativePointEdit.h"

class RelativePoints : public Fact
{
    Q_OBJECT
    Q_PROPERTY(qint64 currentKey
               READ currentKey
               WRITE setCurrentKey
               NOTIFY currentKeyChanged)
public:
    explicit RelativePoints(Fact *parent = nullptr);

    Q_INVOKABLE void editRelativePoint(QVariantMap item);
    Q_INVOKABLE void createEditor(QVariantMap item);
    Q_INVOKABLE void destroyEditor(QVariantMap item);

    qint64 currentKey() const;
    void setCurrentKey(const qint64 &v);

private:
    LookupRelativePoints *f_lookup;
    RelativePointEdit* f_add;
    RelativePointEdit* f_add_;
    qint64 m_currentKey = 0;

signals:
     void relativePointEdited(QVariantMap item);
     void relativePointDisplaySteteChanged(QVariantMap item);
     void moveMap(int key);
     void currentKeyChanged();

private slots:
    /**
     * @brief appLoaded
     */
    void appLoaded();

    /**
     * @brief syncEditorFromModel
     */
    void syncEditorFromModel();
    void on_editorCreated(QVariantMap item);
    void on_map_move(quint64 recordID);
    void on_trigger_record(quint64 recordID);
    void on_delete_record(quint64 recordID);
    void on_edit_record(QVariantMap item);
    void dbAddRelativePoint(QVariantMap item);
    void dbUpdateRelativePoint(QVariantMap item);
    void dbRemoveRelativePoint(QVariantMap item);
};

#endif // RELATIVEPOINTS_H
