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
#ifndef Snapshots_H
#define Snapshots_H
//=============================================================================
#include "LookupSnapshots.h"
#include "SnapshotEdit.h"
#include "filevalidator.h"
#include <Fact/Fact.h>
#include <QtCore>
#include <QtLocation>
#include <datepicker.h>
#include <datepickerpopup.h>
#include <datepickerhumanreadableformater.h>
#include <datepickerstandardformater.h>
//=============================================================================
class Snapshots : public Fact
{
    Q_OBJECT

public:
    explicit Snapshots(Fact *parent = nullptr);

    Q_PROPERTY(QString image_path READ image_path WRITE setImagePath NOTIFY imagePathChanged)
    Q_PROPERTY(QString image_path_preview READ image_path_preview WRITE setImagePathPreview NOTIFY imagePathPreviewChanged)
    //Q_PROPERTY(double screenshot_lon READ screenshot_lon WRITE setSCRLon NOTIFY screenshotLonChanged)
   // Q_PROPERTY(int screenshot_alt READ screenshot_alt WRITE setSCRAlt NOTIFY screenshotAltChanged)

    Q_INVOKABLE void createEditorFromVideo(QString,QByteArray,QByteArray,QString,double,double,int);
    Q_INVOKABLE void createEditor(QVariantMap item);
    Q_INVOKABLE void destroyEditor(QVariantMap item);
    Q_INVOKABLE void moveSnapshot(QVariantMap item);
    Q_INVOKABLE QString getFilterDescription(QVariantMap item);

    LookupSnapshots *f_lookup;
    DatePicker *date_picker;
    QString image_path();
    void setImagePath(QString);
    QString image_path_preview();
    void setImagePathPreview(QString);
    /*QString sort_order_default();
    QString sort_criteria_default();*/
    void init_date_picker(QString qsBeg, QString qsEnd);
    bool createDirectory(QString path);

private:
    QSettings *m_settings;
    FileValidator* m_file_validator = nullptr;
    QString m_settings_group = "apx.tools.snapshots";
    QString m_settings_editor = "editor";
    QString m_settings_editor_name = "editor_name";
    QString m_settings_file_manager = "file_manager";
    QString m_settings_file_manager_name = "file_manager_name";
    QString m_settings_filter_start = "filter_start";
    QString m_settings_filter_finish = "filter_finish";
    SnapshotEdit *f_edit;
    Fact *f_open;
    Fact *f_file_manager;
    Fact *f_snapshot_sorting;
    Fact *f_snapshot_filter;
    Fact *f_sort_criteria;
    Fact *f_sort_order;
    Fact *f_default_date_sorting;
    Fact *f_default_dist_sorting;
    Fact *f_apply_sorting;
    Fact *f_default_filter_period;
    Fact *f_default_filter_entire_period;
    Fact *f_default_filter_distance;
    Fact *f_filter_date;
    Fact *f_filter_distance;
    Fact *f_apply_filter;
    QProcess pImageViewer;
    QProcess pOpenFolder;
    QString m_editor_path = "/usr/bin/gwenview";
    QString m_file_manager_path = "/usr/bin/dolphin";
    QString m_image_editor_name = "gwenview";
    QString m_file_manager_name = "dolphin";
    QString m_env_path = "/usr/bin/env";
    QString m_unset_path = "--unset=LD_LIBRARY_PATH";
    QString m_image_path;
    QString m_image_path_preview;
    QString m_sort_order_default_dist = tr("Ascending");
    QString m_sort_criteria_default_dist = tr("Distance");
    QString m_sort_order_default_date = tr("Descending");
    QString m_sort_criteria_default_date = tr("Date");
    QString m_sort_order = m_sort_order_default_dist;
    QString m_sort_criteria = m_sort_criteria_default_dist;
    int m_filter_distance_value_default = 40000;
    QString m_filter_distance_unit_default = tr("km");
    QTime m_filter_time_beg_default = QTime(0, 0, 0);
    QTime m_filter_time_end_default = QTime(23, 59, 59);
    QDate m_filter_date_beg_default = QDate::currentDate(); //QDate::currentDate().addDays(-1)
    QDate m_filter_date_end_default = QDate::currentDate();
    QDate m_filter_date_range_beg   = QDate(1970, 1, 1);
    QDateTime m_filter_period_beg;
    QDateTime m_filter_period_end;

    QTime m_filter_time_beg_lowest = QTime();
    QTime m_filter_time_end_lowest = QTime(23, 59, 59);
    QDate m_filter_date_beg_lowest = QDate();
    QDate m_filter_date_end_lowest = QDate::currentDate();

    QStringList st_order;
    QStringList st_criteria;
    bool on_start_sorting = true;
    bool on_start_filter = true;
    

    void reloadEditor(QString path);
    void reloadFileManager(QString path);
    bool parceFilterLabel(QString label,
                          QDateTime beg_def,
                          QDateTime end_def,
                          int *distance,
                          QDateTime* beg,
                          QDateTime* end,
                          bool* use_period) const;

private slots:
    void appLoaded();
    void on_map_move(quint64 recordID);
    void on_trigger_record(quint64 recordID);
    void on_lowest_date_timeChanged(QDateTime date);
    void on_delete_record(quint64 recordID);
    void on_record_position(quint64 recordID);
    void syncEditorFromModel();
    void onOpenTriggered();
    void onFileManagerTriggered();
    void on_sort_order_changed();
    void on_sort_criteria_changed();
    void on_default_date_sorting();
    void on_default_dist_sorting();
    void on_apply_sorting();
    void on_apply_filter();
    void on_filter_distance_changed();
    void on_default_filter_period();
    void on_default_filter_entire_period();
    void on_default_filter_distance();
    void on_filter_date_changed();
    void on_filter_date_trigered();
    void on_editorCreated(QVariantMap item);
    void on_snapshot_sorting();
    void on_snapshot_filter();

    void onRemove();
    void dbAddSnapshot(QVariantMap item);
    void dbRemoveSnapshot(QVariantMap item);
    //void moveBackSnapshot(QVariantMap item);
    void dbMoveSnapshot(QVariantMap item);
    void fileRemoveSnapshot(QVariantMap item);
    void fileRemovePreview(QVariantMap item);
    void dbUpdateSnapshot(QVariantMap item);
    void showImage(QVariantMap item);
    void pEditorFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void pOpenFolderFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void pEditorError(QProcess::ProcessError error);
    void pOpenFolderError(QProcess::ProcessError error);
    void onItemTriggered(QVariantMap item);
    void on_date_picker_editing_finished();
    void on_date_picker_canceling_finished();
    void showFolder(QString qsPath);
    void on_restore_filter(int distance,
                        QDateTime beg,
                        QDateTime end,
                         bool use_period);

signals:
     void imagePathChanged();
     void imagePathPreviewChanged();
     void sort_order_changed();
     void sort_criteria_changed();
     void restore_filter(int distance,
                         QDateTime beg,
                         QDateTime end,
                         bool use_period);
     void sig_snapshot_filter();
     void sig_filter_date_changed();
     void moveMap(int key);
};
//=============================================================================
#endif
