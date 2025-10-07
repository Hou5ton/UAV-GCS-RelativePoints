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
#include "Snapshots.h"
#include <App/App.h>
#include <Database/SnapshotsDB.h>
#include <QFileDialog>
#include <KartaInterface.h>
#include <App/AppGcs.h>
#include "sys/wait.h"
#include <QMessageBox>
const QString sWarningCaption = QObject::tr("Warning");
//=============================================================================
Snapshots::Snapshots(Fact *parent)
    : Fact(parent, QString(PLUGIN_NAME).toLower(), tr("Screenshots"), tr("Screenshots on map"))
    , f_edit(nullptr)
{
    m_file_validator = new FileValidator(this);

    if(createDirectory(AppDirs::images().absolutePath())){
       setImagePath("file://" + AppDirs::images().absolutePath() + "/");
    }else{
        apxMsg() <<  tr("Can't create directory") <<  AppDirs::images().absolutePath();
    }
    if(createDirectory(AppDirs::previews().absolutePath())){
        setImagePathPreview("file://" + AppDirs::previews().absolutePath() + "/");
    }else{
        apxMsg() <<  tr("Can't create directory") <<  AppDirs::previews().absolutePath();
    }

    connect(this, &Snapshots::sig_filter_date_changed, this, &Snapshots::on_filter_date_changed);

    m_settings = AppPrefs::instance()->settings();

    f_open = new Fact(this, "open", tr("Image viewer"), tr("Select image viewer"));
    f_open->setIcon("import");
    connect(f_open, &Fact::triggered, this, &Snapshots::onOpenTriggered);

    f_file_manager = new Fact(this, "file_manager", tr("File Manager"), tr("Select file manager"));
    f_file_manager->setIcon("import");
    connect(f_file_manager, &Fact::triggered, this, &Snapshots::onFileManagerTriggered);


    setIcon("monitor-screenshot");
    connect(App::instance(), &App::loadingFinished, this, &Snapshots::appLoaded);

    connect(this,&Snapshots::restore_filter,this,&Snapshots::on_restore_filter);
    connect(this, &Snapshots::sig_snapshot_filter, this, &Snapshots::on_snapshot_filter);

    f_snapshot_filter = new Fact(this,
                                   "snapshot_filter",
                                   tr("Filter"),
                                   tr("Filter"),
                                   /*Section,*/FactBase::Flags(NoFlags),
                                   "filter");

    connect(f_snapshot_filter, &Fact::triggered, this, &Snapshots::on_snapshot_filter);

    f_snapshot_sorting = new Fact(this,
                                   "snapshot_sorting",
                                   tr("Sorting"),
                                   tr("Sorting"),
                                   /*Section,*/FactBase::Flags(NoFlags),
                                   "sort");

    connect(f_snapshot_sorting, &Fact::triggered, this, &Snapshots::on_snapshot_sorting);

    f_sort_criteria = new Fact(f_snapshot_sorting,
                      "snapshot_sort_criteria",
                      tr("Sorting Criteria"),
                      tr("Sorting Criteria"),
                      Enum | PersistentValue | SystemSettings);
   // f_sort_criteria->setIcon("sort-variant");
    st_criteria << tr("Date") << tr("Distance");
    f_sort_criteria->setEnumStrings(st_criteria);
    if (st_criteria.isEmpty())
        f_sort_criteria->setEnabled(false);
    else {
        f_sort_criteria->setEnabled(true);
    }
    f_sort_criteria->setDefaultValue(st_criteria.indexOf(m_sort_criteria));
   connect(f_sort_criteria, &Fact::valueChanged, this, &Snapshots::on_sort_criteria_changed);


    f_sort_order = new Fact(f_snapshot_sorting,
                      "snapshot_sort_order",
                      tr("Sorting Order"),
                      tr("Sorting Order"),
                      Enum | PersistentValue | SystemSettings);
   // f_sort_order->setIcon("sort-variant");

    st_order << tr("Ascending") << tr("Descending");
    f_sort_order->setEnumStrings(st_order);
    if (st_order.isEmpty())
        f_sort_order->setEnabled(false);
    else {
        f_sort_order->setEnabled(true);
    }
    f_sort_order->setDefaultValue(st_order.indexOf(m_sort_order));
    connect(f_sort_order, &Fact::valueChanged, this, &Snapshots::on_sort_order_changed);

    f_default_date_sorting = new Fact(f_snapshot_sorting,
                        "default_date_sorting",
                        tr("Default Date"),
                        tr("Set default date sorting"),
                        Action /*| CloseOnTrigger*/ | ShowDisabled | IconOnly,
                         "calendar");

    connect(f_default_date_sorting, &Fact::triggered, this, &Snapshots::on_default_date_sorting);
    f_default_date_sorting->setEnabled(false);

    f_default_dist_sorting = new Fact(f_snapshot_sorting,
                        "default_dist_sorting",
                        tr("Default Distance"),
                        tr("Set default distance sorting"),
                        Action /*| CloseOnTrigger*/ | ShowDisabled | IconOnly,
                        "map-marker-distance");


    connect(f_default_dist_sorting, &Fact::triggered, this, &Snapshots::on_default_dist_sorting);
    f_default_dist_sorting->setEnabled(false);

   f_apply_sorting = new Fact(f_snapshot_sorting,
                       "apply_sorting",
                       tr("Apply"),
                       tr("Apply sorting"),
                        Action | Apply | CloseOnTrigger | ShowDisabled | IconOnly);

   connect(f_apply_sorting, &Fact::triggered, this, &Snapshots::on_apply_sorting);


    f_filter_date = new Fact(f_snapshot_filter,
                             "filter_date",
                             tr("Date time period"),
                             tr("Snapshots date time period"),
                             Bool | PersistentValue);

    f_filter_date->setDefaultValue(false);
    connect(f_filter_date, &Fact::valueChanged, this, &Snapshots::on_filter_date_changed);
    connect(f_filter_date, &Fact::triggered, this, &Snapshots::on_filter_date_trigered);

    f_filter_distance = new Fact(f_snapshot_filter,
                          "filter_distance",
                          tr("Distance"),
                          tr("Distance"),
                          Int | PersistentValue);
    f_filter_distance->setUnits(m_filter_distance_unit_default);
    f_filter_distance->setMin(1);
    f_filter_distance->setDefaultValue(m_filter_distance_value_default);

    connect(f_filter_distance, &Fact::valueChanged, this, &Snapshots::on_filter_distance_changed);

    f_default_filter_entire_period = new Fact(f_snapshot_filter,
                                      "default_filter_entire_period",
                                      tr("For the entire time period"),
                                      tr("Set filter for the entire time period"),
                                      Action /*| CloseOnTrigger*/ | ShowDisabled | IconOnly,
                                      "calendar-multiple");

    connect(f_default_filter_entire_period, &Fact::triggered, this, &Snapshots::on_default_filter_entire_period);


    f_default_filter_period = new Fact(f_snapshot_filter,
                        "default_filter_period",
                        tr("Default Period"),
                        tr("Set default filter period"),
                        Action /*| CloseOnTrigger*/ | ShowDisabled | IconOnly,
                        "calendar");

    connect(f_default_filter_period, &Fact::triggered, this, &Snapshots::on_default_filter_period);

    f_default_filter_distance = new Fact(f_snapshot_filter,
                        "default_filter_distance",
                        tr("Default Distance"),
                        tr("Set default distance"),
                        Action /*| CloseOnTrigger*/ | ShowDisabled | IconOnly,
                        "map-marker-distance");

    connect(f_default_filter_distance, &Fact::triggered, this, &Snapshots::on_default_filter_distance);

    f_apply_filter = new Fact(f_snapshot_filter,
                        "apply_filter",
                        tr("Apply"),
                        tr("Apply filter"),
                         Action | Apply | CloseOnTrigger | ShowDisabled | IconOnly);

    connect(f_apply_filter, &Fact::triggered, this, &Snapshots::on_apply_filter);


    f_lookup = new LookupSnapshots(this/*,f_title*/);
    connect(f_lookup->dbModel(),
            &DatabaseLookupModel::synced,
            this,
            &Snapshots::syncEditorFromModel);

    connect(f_lookup, &LookupSnapshots::recordTriggered, this, &Snapshots::on_trigger_record);
    connect(f_lookup, &LookupSnapshots::recordMapMove, this, &Snapshots::on_map_move);
    connect(f_lookup, &LookupSnapshots::recordDeleteted, this, &Snapshots::on_delete_record);
   // connect(f_lookup, &LookupSnapshots::itemTriggered, this, &Snapshots::onItemTriggered);
    connect(f_lookup, &LookupSnapshots::snapshotShow, this, &Snapshots::showImage);
    connect(f_lookup, &LookupSnapshots::snapshotMove, this, &Snapshots::moveSnapshot/*dbMoveSnapshot*/);
    connect(f_lookup, &LookupSnapshots::editorCreated, this, &Snapshots::on_editorCreated);
    connect(f_lookup, &LookupSnapshots::folderOpened, this, &Snapshots::showFolder);
    connect(f_lookup, &LookupSnapshots::lowest_date_timeChanged, this, &Snapshots::on_lowest_date_timeChanged);

    m_settings->beginGroup(m_settings_group);
    QString editor_path = m_settings->value(m_settings_editor).toString();
    if(editor_path != "")
        m_editor_path = editor_path;
    QString image_editor_name = m_settings->value(m_settings_editor_name).toString();
    if(image_editor_name != "")
        m_image_editor_name = image_editor_name;

    QString file_manager_path = m_settings->value(m_settings_file_manager).toString();
    if(file_manager_path != "")
        m_file_manager_path = file_manager_path;

    QString file_manager_name = m_settings->value(m_settings_file_manager_name).toString();
    if(file_manager_name != "")
        m_file_manager_name = file_manager_name;

    init_date_picker(m_settings->value(m_settings_filter_start).toString(),
                     m_settings->value(m_settings_filter_finish).toString());

     m_settings->endGroup();
    if(m_editor_path != ""){
        reloadEditor(m_editor_path);
    }
    if(m_file_manager_path != ""){
        reloadFileManager(m_file_manager_path);
    }

    on_sort_order_changed();
    on_sort_criteria_changed();
    emit sig_filter_date_changed();
    on_filter_distance_changed();
    on_default_filter_period();

    loadQml("qrc:/" PLUGIN_NAME "/SnapshotsPlugin.qml");
}
bool Snapshots::createDirectory(QString path)
{
    QString filePath = path;
    QDir dir(filePath);
    if (!dir.exists()){
        if(dir.mkpath(".")){
             apxMsg() <<  tr("Create directory") <<  path;
             return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}

void Snapshots::init_date_picker(QString qsBeg, QString qsEnd)
{
    date_picker = new DatePicker();
    //DatePickerHumanReadableFormater *formater = new DatePickerHumanReadableFormater();
    DatePickerStandardFormater *formater = new DatePickerStandardFormater();

    // setup period delimeters in date string representation
    //    formater->setDateFromWord(QString::null);
    //    formater->setDateToWord("-");
   // formater->setSpecialDayWordShown(false);
    date_picker->setFormater(formater);
    date_picker->setAllowedPickerTypes(PeriodType);

    // editable datepicker shows calendar popup on mouse click
    date_picker->setEditable(true);

    // enabling time inputs (enabled by default)
    date_picker->setTimeEditable(true);

    date_picker->setTimeInputFormat("hh:mm:ss");

    // date picker label can be customized in usual way
    //Q_UNUSED(date_picker->label());

    if(qsBeg != "" && qsEnd != ""){
        QDateTime qdtBeg = QDateTime::fromString(qsBeg);
        QDateTime qdtEnd = QDateTime::fromString(qsEnd);
        date_picker->setDatePeriod(qdtBeg.date(), qdtEnd.date());
        date_picker->setTimePeriod(qdtBeg.time(), qdtEnd.time());
    }else{
        date_picker->setDatePeriod(m_filter_date_beg_default, m_filter_date_end_default);
        date_picker->setTimePeriod(m_filter_time_beg_default, m_filter_time_end_default);
    }
    m_filter_period_beg = date_picker->dateTimePeriodBegin();
    m_filter_period_end = date_picker->dateTimePeriodEnd();

    // set initial time
    date_picker->setRange(m_filter_date_range_beg, m_filter_date_end_default);

    connect(date_picker,&DatePicker::editingFinished,this, &Snapshots::on_date_picker_editing_finished);
    connect(date_picker,&DatePicker::cancelingFinished,this, &Snapshots::on_date_picker_canceling_finished);

}

/*QString Snapshots::sort_order_default()
{
    return m_sort_order_default;
}
QString Snapshots::sort_criteria_default()
{
    return m_sort_criteria_default;
}*/

 QString Snapshots::image_path()
 {
     return m_image_path;
 }
 QString Snapshots::image_path_preview()
 {
     return m_image_path_preview;
 }
 void Snapshots::setImagePath(QString image_path)
 {
     m_image_path = image_path;
     emit imagePathChanged();
 }
void Snapshots::setImagePathPreview(QString image_path)
{
    m_image_path_preview = image_path;
    emit imagePathPreviewChanged();
}
void Snapshots::onOpenTriggered()
{
    QString qsHome = m_editor_path;
    if(m_editor_path == "")
        qsHome = QDir::homePath();
    QString path = QFileDialog::getOpenFileName(nullptr,
                                                tr("Select image viewer"),
                                                qsHome,
                                                "");
    if (!path.isEmpty()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);

        reloadEditor(path);
    }
}

void Snapshots::onFileManagerTriggered()
{
    QString qsHome = m_file_manager_path;
    if(m_file_manager_path == "")
        qsHome = QDir::homePath();
    QString path = QFileDialog::getOpenFileName(nullptr,
                                                tr("Select file manager"),
                                                qsHome,
                                                "");
    if (!path.isEmpty()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);

        reloadFileManager(path);
    }
}

void Snapshots::reloadEditor(QString path)
{

    //pImageViewer.setProgram(path);
    pImageViewer.setProgram(m_env_path);

    connect(&pImageViewer,
            QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &Snapshots::pEditorFinished);

    connect(&pImageViewer,
            QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
            this,
            &Snapshots::pEditorError);

    m_settings->beginGroup(m_settings_group);
    m_settings->setValue(m_settings_editor, path);
    QStringList qsl = path.split("/");
    if(qsl.size()){
        m_settings->setValue(m_settings_editor_name, qsl.at(qsl.size()-1));
    }
    m_settings->endGroup();
    f_open->setDescr(path);
   // setDescr(path);

}

void Snapshots::reloadFileManager(QString path)
{

    //pOpenFolder.setProgram(path);
    pOpenFolder.setProgram(m_env_path);

    connect(&pOpenFolder,
            QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            &Snapshots::pOpenFolderFinished);

    connect(&pOpenFolder,
            QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
            this,
            &Snapshots::pOpenFolderError);

    m_settings->beginGroup(m_settings_group);
    m_settings->setValue(m_settings_file_manager, path);
    QStringList qsl = path.split("/");
    if(qsl.size()){
        m_settings->setValue(m_settings_file_manager_name, qsl.at(qsl.size()-1));
    }
    m_settings->endGroup();
    f_file_manager->setDescr(path);

}
void Snapshots::appLoaded()
{
    /*SnapshotEdit *f = new SnapshotEdit(this, "snapshot", tr("Snapshot"), "", QVariantMap());
    f->setIcon("monitor-screenshot");*/

   /* Fact *fSnapshot = AppRoot::instance()->findChild("apx.tools.videostreaming");
    if (!fSnapshot)
        return;*/

}
void Snapshots::on_sort_order_changed()
{
    if(f_sort_order->text() == m_sort_order_default_date  &&
            f_sort_criteria->text() == m_sort_criteria_default_date){
        f_default_date_sorting->setEnabled(false);
    }else{
        f_default_date_sorting->setEnabled(true);
    }

    if(f_sort_order->text() == m_sort_order_default_dist  &&
            f_sort_criteria->text() == m_sort_criteria_default_dist){
        f_default_dist_sorting->setEnabled(false);
    }else{
        f_default_dist_sorting->setEnabled(true);
    }
    if(on_start_sorting){
        on_apply_sorting();
    }
}
void Snapshots::on_sort_criteria_changed()
{
    if(f_sort_order->text() == m_sort_order_default_date  &&
       f_sort_criteria->text() == m_sort_criteria_default_date){
        f_default_date_sorting->setEnabled(false);
    }else{
        f_default_date_sorting->setEnabled(true);
    }

    if(f_sort_order->text() == m_sort_order_default_dist  &&
       f_sort_criteria->text() == m_sort_criteria_default_dist){
        f_default_dist_sorting->setEnabled(false);
    }else{
        f_default_dist_sorting->setEnabled(true);
    }
    if(on_start_sorting){
        on_apply_sorting();
    }
}
void Snapshots::on_filter_date_changed()
{
    //FactData *fact = qobject_cast<FactData *>(sender());
    //Snapshots *snap = qobject_cast<Snapshots *>(sender());

    bool bRet1 = date_picker->datePeriodBegin() == m_filter_date_beg_default;
    bool bRet2 = date_picker->datePeriodEnd() == m_filter_date_end_default;
    bool bRet3 = date_picker->timePeriodBegin() == m_filter_time_beg_default;
    bool bRet4 = date_picker->timePeriodEnd() == m_filter_time_end_default;

    if((bRet1 && bRet2 && bRet3 && bRet4) ||
            !f_filter_date->value().toBool()){
        f_default_filter_period->setEnabled(false);
    }else{
        f_default_filter_period->setEnabled(true);
    }

    bRet1 = date_picker->datePeriodBegin() == m_filter_date_beg_lowest;
    bRet2 = date_picker->datePeriodEnd() == m_filter_date_end_lowest;
    bRet3 = date_picker->timePeriodBegin() == m_filter_time_beg_lowest;
    bRet4 = date_picker->timePeriodEnd() == m_filter_time_end_lowest;

    if(m_filter_date_beg_lowest.isValid() &&
       m_filter_date_end_lowest.isValid() &&
       m_filter_time_beg_lowest.isValid() &&
            m_filter_time_end_lowest.isValid()){
        if((bRet1 && bRet2 && bRet3 && bRet4) ||
                !f_filter_date->value().toBool()){
            f_default_filter_entire_period->setEnabled(false);
        }else{
            f_default_filter_entire_period->setEnabled(true);
        }
    }

    if(on_start_filter){
        on_apply_filter();
    }else{
        if(f_filter_date->value().toBool()){
            f_filter_date->setDescr(date_picker->label()->text());
        }else{
            f_filter_date->setDescr(tr("No used"));
        }
    }
}
void Snapshots::on_filter_date_trigered()
{
    if(f_filter_date->value().toBool()){
        if(!date_picker->isVisible()){
            QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
            int x = (screenGeometry.width() - date_picker->width()) / 2;
            int y = (screenGeometry.height() - date_picker->height()) / 2;
            date_picker->move(x, y);
            date_picker->show();
        }
    }else{
        f_filter_date->setDescr(tr("No used"));
    }
}
void Snapshots::on_filter_distance_changed()
{
    if(f_filter_distance->value() == m_filter_distance_value_default &&
            f_filter_distance->units() == m_filter_distance_unit_default){
        f_default_filter_distance->setEnabled(false);
    }else{
        f_default_filter_distance->setEnabled(true);
    }
    if(on_start_filter){
        on_apply_filter();
    }
}
void Snapshots::on_trigger_record(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);
    //emit moveMap(item.value("key").toInt());
    createEditor(item);
}
void Snapshots::on_map_move(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);
    emit moveMap(item.value("key").toInt());
}
void Snapshots::on_lowest_date_timeChanged(QDateTime date)
{
    if(m_filter_time_beg_lowest != date.time() ||
        m_filter_date_beg_lowest != date.date()){

        m_filter_time_beg_lowest = date.time();
        m_filter_date_beg_lowest = date.date();
        emit sig_filter_date_changed();
    }

}

void Snapshots::on_record_position(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);
    if(f_lookup->selectedItems().contains(i)){
        f_lookup->triggerItem(item,true);
    }else{
       f_lookup->triggerItem(item,false);
    }
}

void Snapshots::on_delete_record(quint64 recordID)
{
    int i = f_lookup->dbModel()->indexOf("key", recordID);
    if (i < 0) {
        return;
    }
    QVariantMap item = f_lookup->dbModel()->get(i);

   /* QString qsWarning =  tr("You are going to detete the screenshot");
    qsWarning += " ";
    qsWarning +=item["title"].toString();
    qsWarning += "\n";
    qsWarning += tr("Are you sure?");

    if(QMessageBox::warning(nullptr,
                            sWarningCaption,
                            qsWarning,
                            tr("Yes"),
                            tr("No"),
                            QString(),
                            0,
                            1)) return;*/

   // removeLocalFiles(item);
    dbRemoveSnapshot(item);
}
void Snapshots::on_default_dist_sorting()
{
   f_sort_order->setValue(st_order.indexOf(m_sort_order_default_dist));
   f_sort_criteria->setValue(st_criteria.indexOf(m_sort_criteria_default_dist));
  // on_apply_sorting();
}
void Snapshots::on_default_date_sorting()
{
    f_sort_order->setValue(st_order.indexOf(m_sort_order_default_date));
    f_sort_criteria->setValue(st_criteria.indexOf(m_sort_criteria_default_date));
  // on_apply_sorting();
}
void Snapshots::on_snapshot_sorting()
{
    on_start_sorting = false;
    QString prev = f_snapshot_sorting->descr();
    QStringList qsl = prev.split(" ");
    if(qsl.size() == 2){
        f_sort_criteria->setValue(st_criteria.indexOf(qsl.at(0)));
        f_sort_order->setValue(st_order.indexOf(qsl.at(1)));
    }
}
bool Snapshots::parceFilterLabel(QString label,
                                 QDateTime beg_def,
                                 QDateTime end_def,
                      int *distance,
                      QDateTime* beg,
                      QDateTime* end,
                      bool* use_period) const
{
    bool bRet = true;
    *use_period = true;
    QStringList qsl = label.split(tr("Distance"));
    if(qsl.size() == 2){
        QStringList qslDist = qsl.at(1).split(" ");
        if(qslDist.size() == 2 &&
                distance){
            *distance = qslDist.at(0).toInt();
        }else if(qslDist.size() == 3 &&
                 distance){
            *distance = qslDist.at(1).toInt();
        }else{
            bRet = false;
            return bRet;
        }

        QString qsDate = qsl.at(0);
        if(qsDate.size()){
           QStringList qslDate = qsDate.split(" ");
           if(qslDate.size() == 11){
               QString date_beg = qslDate.at(1) + " " +
                              qslDate.at(2) + " " +
                              qslDate.at(3) + " " +
                              qslDate.at(4);

               QString date_end = qslDate.at(6) + " " +
                              qslDate.at(7) + " " +
                              qslDate.at(8) + " " +
                              qslDate.at(9);

               if(beg && end){
                   *beg = QDateTime::fromString(date_beg,"d MM yyyy hh:mm:ss");
                   *end = QDateTime::fromString(date_end,"d MM yyyy hh:mm:ss");
               }else{
                    bRet = false;
               }
           }
        }else{
            *use_period = false;
            if(beg && end){
                *beg = beg_def;
                *end = end_def;
            }else{
                bRet = false;
            }
        }
        /*qsDate.remove(tr("From"));
        QStringList qslDate = qsDate.split(tr("to"));
        if(qslDate.size() == 2 &&
                beg &&
                end){
            *beg = QDateTime::fromString(qslDate.at(0)," d MM yyyy hh:mm:ss ");
            *end = QDateTime::fromString(qslDate.at(1)," d MM yyyy hh:mm:ss ");
        }else{
            if(qsDate == ""){
                *use_period = false;
                *beg = beg_def;
                *end = end_def;
            }else{
                bRet = false;
                return bRet;
            }
        }*/
    }else{
        bRet = false;
        return bRet;
    }
    return bRet;
}

void Snapshots::on_restore_filter(int distance,
                    QDateTime beg,
                    QDateTime end,
                    bool use_period)
{
    f_filter_distance->setValue(distance);
    date_picker->setDatePeriod(beg.date(), end.date());
    date_picker->setTimePeriod(beg.time(), end.time());

    if(f_filter_date->value().toBool() != use_period){
        f_filter_date->setValue(use_period);
    }else{
        f_filter_date->setDescr(date_picker->label()->text());
    }
    /*if(f_filter_date->value().toBool()){
        f_filter_date->setDescr(date_picker->label()->text());
    }else{
        f_filter_date->setDescr(tr("No used"));
    }*/
}
void Snapshots::on_snapshot_filter()
{
   // Fact *fact = qobject_cast<Fact *>(sender());
    int distance;
    bool use_period;
    QDateTime beg;
    QDateTime end;

    on_start_filter = false;

    bool bRet = parceFilterLabel(f_snapshot_filter->descr(),
                                 /*date_picker->dateTimePeriodBegin()*/m_filter_period_beg,
                                 /*date_picker->dateTimePeriodEnd()*/m_filter_period_end,
                                 &distance,
                                 &beg,
                                 &end,
                                 &use_period);
    if(bRet){
       emit restore_filter(distance,beg,end,use_period);
    }
}
void Snapshots::on_apply_sorting()
{
    //f_lookup->updSorting();

    f_lookup->set_sort_order(f_sort_order->text());
    f_lookup->set_sort_criteria(f_sort_criteria->text());

    f_lookup->set_sort_order_value(f_sort_order->value().toInt());
    f_lookup->set_sort_criteria_value(f_sort_criteria->value().toInt());

    f_snapshot_sorting->setDescr(f_sort_criteria->text() +
                                  " " +
                                  f_sort_order->text());
}
void Snapshots::on_apply_filter()
{

    f_lookup->set_filter_date_begin(date_picker->dateTimePeriodBegin());
    f_lookup->set_filter_date_end(date_picker->dateTimePeriodEnd());
    f_lookup->set_filter_date_enabled(f_filter_date->value().toBool());
    f_lookup->set_filter_distance(f_filter_distance->text());

    if(f_filter_date->value().toBool()){
        f_filter_date->setDescr(date_picker->label()->text());
    }else{
        f_filter_date->setDescr(tr("No used"));
    }

    if(f_filter_date->value().toBool()){
        f_snapshot_filter->setDescr(date_picker->label()->text() +
                                    " " +
                                    tr("Distance") +
                                    " " +
                                    f_filter_distance->text() +
                                    " " +
                                    f_filter_distance->units());
    }else{
        f_snapshot_filter->setDescr(tr("Distance") +
                                    " " +
                                    f_filter_distance->text() +
                                    " " +
                                    f_filter_distance->units());
    }

    QDateTime qdt_beg  = date_picker->dateTimePeriodBegin();
    QDateTime qdt_end  = date_picker->dateTimePeriodEnd();

    m_filter_period_beg = qdt_beg;
    m_filter_period_end = qdt_end;

    m_settings->beginGroup(m_settings_group);
    m_settings->setValue(m_settings_filter_start, qdt_beg.toString());
    m_settings->setValue(m_settings_filter_finish, qdt_end.toString());
    m_settings->endGroup();
}

void Snapshots::on_default_filter_period()
{
    date_picker->setDatePeriod(m_filter_date_beg_default, m_filter_date_end_default);
    date_picker->setTimePeriod(m_filter_time_beg_default, m_filter_time_end_default);
    if(f_filter_date->value().toBool()){
        f_filter_date->setDescr(date_picker->label()->text());
    }else{
        f_filter_date->setDescr(tr("No used"));
    }
    //f_default_filter_period->setEnabled(false);
    //on_apply_filter();
    emit sig_filter_date_changed();
}

void Snapshots::on_default_filter_entire_period()
{
    date_picker->setDatePeriod(m_filter_date_beg_lowest, m_filter_date_end_lowest);
    date_picker->setTimePeriod(m_filter_time_beg_lowest, m_filter_time_end_lowest);
    if(f_filter_date->value().toBool()){
        f_filter_date->setDescr(date_picker->label()->text());
    }else{
        f_filter_date->setDescr(tr("No used"));
    }
    //f_default_filter_entire_period->setEnabled(false);
    //on_apply_filter();
    emit sig_filter_date_changed();
}

void Snapshots::on_default_filter_distance()
{
    f_filter_distance->setValue(f_filter_distance->defaultValue());
   // on_apply_filter();
}

//=============================================================================
void Snapshots::createEditorFromVideo(QString text,
                                      QByteArray file,
                                      QByteArray file_prev,
                                      QString vehicle,
                                      double lat,
                                      double lon,
                                      int alt)
{
    QVariantMap modelData;
    modelData["vehicle"] = vehicle;
    modelData["filename"] = text;
    modelData["lat"] = lat;
    modelData["lon"] = lon;
    modelData["alt"] = alt;
    modelData["lat_scr"] = lat;
    modelData["lon_scr"] = lon;

    QDateTime dateTime = QDateTime::fromString(text,"dd_MM_yyyyThh_mm_ss_zzz");
    QString time = dateTime.time().toString("hh:mm:ss.zzz");
    modelData["time"] = time;
    QString date = dateTime.date().toString("yyyy MMM d");
    modelData["date"] = date;
    modelData["title"] = date + " " + time;
    modelData["file"] = file;
    modelData["file_preview"] = file_prev;

    if(f_edit){
        f_edit->remove();
        f_edit = nullptr;
    }

    QString qsKey = modelData.value("key").toString();
    QString qsDescr = tr("Edit screenshot")+ ":" + modelData.value("key").toString();

    f_edit = new SnapshotEdit(this, "edit", tr("Screenshot"), qsDescr, modelData);
    f_edit->setIcon("settings");

    connect(f_edit, &SnapshotEdit::removed, this, &Snapshots::onRemove);
    connect(f_edit, &SnapshotEdit::removeTriggered, this, &Snapshots::dbRemoveSnapshot);
    connect(f_edit, &SnapshotEdit::snapshotEdited, this, &Snapshots::dbUpdateSnapshot);
    connect(f_lookup->dbModel(),
            &DatabaseLookupModel::itemEdited,
            f_edit,
            &SnapshotEdit::updateFromEditedModelData);

    f_edit->setVisible(false);

    f_edit->f_title->setValue(text);
    f_edit->f_latitude->setValue(lat);
    f_edit->f_longitude->setValue(lon);
    f_edit->f_alt_label->setValue(alt);

    f_edit->f_latitude_scr->setValue(lat);
    f_edit->f_longitude_scr->setValue(lon);
    f_edit->f_alt->setValue(alt);

    f_edit->f_use_label_coo->setValue(false);

    dbAddSnapshot(modelData);
    App::jsync(this);

}
void Snapshots::moveSnapshot(QVariantMap item)
{
     int i = f_lookup->dbModel()->indexOf("key", item.value("key").toULongLong());
     if (i < 0) {
         return;
     }
     if(f_lookup->dbModel()->get(i) != item){
         createEditor(item);
         f_lookup->dbModel()->set(i,item);
     }
}
QString Snapshots::getFilterDescription(QVariantMap item)
{
    QString qRet = item.value("title").toString();
    if(f_lookup->filter() != ""){
        if(item.value("vehicle").toString().contains(f_lookup->filter())){
          qRet =  tr("Vehicle") + ": " + item.value("vehicle").toString();
        }else if(item.value("filename").toString().contains(f_lookup->filter())){
            qRet = tr("File name") + ": " + item.value("filename").toString();
        }else if(AppRoot::latToString(item.value("lat_scr").toDouble()).contains(f_lookup->filter())){
            qRet = tr("Latitude") + ": " + AppRoot::latToString(item.value("lat_scr").toDouble());
        }else if(AppRoot::lonToString(item.value("lon_scr").toDouble()).contains(f_lookup->filter())){
            qRet = tr("Longitude") + ": " + AppRoot::lonToString(item.value("lon_scr").toDouble());
        }else if(item.value("alt").toString().contains(f_lookup->filter())){
            qRet = tr("Altitude") + ": " + item.value("alt").toString();
        }
    }
    return qRet;
}

void Snapshots::on_editorCreated(QVariantMap item)
{
    createEditor(item);
    f_edit->trigger(item);
}

void Snapshots::createEditor(QVariantMap item)
{
    if(item.size()){
        if(f_edit /*&& f_edit->modelData != item*/)
            destroyEditor(/*QVariantMap()*//*item*/f_edit->modelData);

        QString qsKey = item.value("key").toString();
        QString qsDescr = tr("Edit screenshot")+ ":" + item.value("key").toString();

        f_edit = new SnapshotEdit(this, "edit", tr("Screenshot"), qsDescr, item);
        f_edit->setIcon("settings");

        connect(f_edit, &SnapshotEdit::removed, this, &Snapshots::onRemove);
        connect(f_edit, &SnapshotEdit::removeTriggered, this, &Snapshots::dbRemoveSnapshot);
        connect(f_edit, &SnapshotEdit::snapshotEdited, this, &Snapshots::dbUpdateSnapshot);
        connect(f_edit, &SnapshotEdit::snapshotShow, this, &Snapshots::showImage);
        connect(f_edit, &SnapshotEdit::snapshotShow, &pImageViewer, &QProcess::terminate);
        connect(f_edit, &SnapshotEdit::snapshotMove, this, &Snapshots::moveSnapshot/*dbMoveSnapshot*/);
        connect(f_lookup->dbModel(),
                &DatabaseLookupModel::itemEdited,
                f_edit,
                &SnapshotEdit::updateFromEditedModelData);

        f_edit->setVisible(false);

        App::jsync(this);
    }
}
void Snapshots::destroyEditor(QVariantMap item)
{
    //Q_UNUSED(item)
    if (!f_edit)
        return;

    if (f_edit->modelData.value("key").toULongLong() != item.value("key").toULongLong()){
         qDebug()<<f_edit->modelData.value("key").toString()<<":"<<item.value("key").toString();
    }

    disconnect(f_edit, &SnapshotEdit::removed, this, &Snapshots::onRemove);
    disconnect(f_edit, &SnapshotEdit::removeTriggered, this, &Snapshots::dbRemoveSnapshot);
    disconnect(f_edit, &SnapshotEdit::snapshotEdited, this, &Snapshots::dbUpdateSnapshot);
    disconnect(f_edit, &SnapshotEdit::snapshotShow, this, &Snapshots::showImage);
    disconnect(f_edit, &SnapshotEdit::snapshotShow, &pImageViewer, &QProcess::terminate);
    disconnect(f_edit, &SnapshotEdit::snapshotMove, this, &Snapshots::moveSnapshot);
    disconnect(f_lookup->dbModel(),
            &DatabaseLookupModel::itemEdited,
            f_edit,
            &SnapshotEdit::updateFromEditedModelData);

    f_edit->remove();
    f_edit = nullptr;

    App::jsync(this);

}
void Snapshots::syncEditorFromModel()
{
    if (!f_edit)
        return;
    if(f_edit->modelData.value("key").toString() == ""){    //on add
        destroyEditor(f_edit->modelData);
        return;
    }else{

       // qDebug()<<f_edit->modelData.value("key").toString();

        int i = f_lookup->dbModel()->indexOf("key", f_edit->modelData.value("key"));
        if (i < 0) {
            destroyEditor(f_edit->modelData);
            return;
        }
        QVariantMap vm = f_lookup->dbModel()->get(i);
        MPoint geoCoord_scr(vm.value("lon_scr").toDouble(), vm.value("lat_scr").toDouble());
        auto sk42coord_scr = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord_scr);
        vm["sk42x_scr"] = std::round(sk42coord_scr.y);
        vm["sk42y_scr"] = std::round(sk42coord_scr.x);

        MPoint geoCoord(vm.value("lon").toDouble(), vm.value("lat").toDouble());
        auto sk42coord = AppGcs::instance()->mapInterface()->geoWgs84ToGaussSk42(geoCoord);
        vm["sk42x"] = std::round(sk42coord.y);
        vm["sk42y"] = std::round(sk42coord.x);
        f_edit->setModelData(vm);
    }
}
//=============================================================================
//=============================================================================
void Snapshots::onRemove()
{
    if(f_edit){
        f_edit = nullptr;
        //destroyEditor(f_edit->modelData);
    }

}
void Snapshots::dbAddSnapshot(QVariantMap item)
{
    //qDebug()<<item;
    DBReqSnapshotsSaveSnapshot *req = new DBReqSnapshotsSaveSnapshot(item.value("title").toString(),
                                                           item.value("descr").toString(),
                                                           item.value("vehicle").toString(),
                                                           item.value("lat").toDouble(),
                                                           item.value("lon").toDouble(),
                                                           item.value("alt").toInt(),
                                                           item.value("lat_scr").toDouble(),
                                                           item.value("lon_scr").toDouble(),
                                                           item.value("date").toString(),
                                                           item.value("time").toString(),
                                                           item.value("filename").toString(),
                                                           item.value("file").toByteArray(),
                                                           item.value("file_preview").toByteArray()
                                                           );
    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseSnapshotLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqSnapshotsSaveSnapshot::snapshotAdded,
        this,
        [](QString filename) { apxMsg() << tr("Screenshot data added").append(':') << filename; },
        Qt::QueuedConnection);
    req->exec();
}
//=============================================================================
void Snapshots::dbRemoveSnapshot(QVariantMap item)
{
    destroyEditor(item);
    //qDebug()<<item;
    quint64 key = item.value("key").toULongLong();
    if (!key)
        return;
    fileRemoveSnapshot(item);
    fileRemovePreview(item);

    DBReqSnapshotsRemoveSnapshot *req = new DBReqSnapshotsRemoveSnapshot(key);
    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseSnapshotLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqSnapshotsRemoveSnapshot::snapshotRemoved,
        this,
        []() { apxMsg() << tr("Screenshot data removed"); },
        Qt::QueuedConnection);
    req->exec();
}
void Snapshots::fileRemoveSnapshot(QVariantMap item)
{
     QString key = item.value("filename").toString();
     QString qsPath = AppDirs::images().absolutePath();
     QString filename = QString("%1/%2.%3").arg(qsPath, key, "png");
     QFile file(filename);
     if(file.remove())
         apxMsg() << tr("Screenshot removed");


}
void Snapshots::fileRemovePreview(QVariantMap item)
{
     QString key = item.value("filename").toString();
     QString qsPath = AppDirs::previews().absolutePath();
     QString filename = QString("%1/%2.%3").arg(qsPath, key, "png");
     QFile file(filename);
     if(file.remove())
         apxMsg() << tr("Preview removed");


}
//=============================================================================
void Snapshots::dbUpdateSnapshot(QVariantMap item)
{
    //qDebug()<<item;
    quint64 key = item.value("key").toULongLong();
    if (!key)
        return;
    DBReqSnapshotsSaveSnapshot *req = new DBReqSnapshotsSaveSnapshot(item.value("title").toString(),
                                                           item.value("descr").toString(),
                                                           item.value("vehicle").toString(),
                                                           item.value("lat").toDouble(),
                                                           item.value("lon").toDouble(),
                                                           item.value("alt").toInt(),
                                                           item.value("lat_scr").toDouble(),
                                                           item.value("lon_scr").toDouble(),
                                                           item.value("date").toString(),
                                                           item.value("time").toString(),
                                                           item.value("filename").toString(),
                                                           item.value("file").toByteArray(),
                                                           item.value("file_preview").toByteArray(),
                                                           key);

    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseSnapshotLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqSnapshotsSaveSnapshot::snapshotModified,
        this,
        [](QString title) { apxMsg() << tr("Screenshot data updated").append(':') << title; },
        Qt::QueuedConnection);
    req->exec();
}

void Snapshots::dbMoveSnapshot(QVariantMap item)
{
    //qDebug()<<item;
    quint64 key = item.value("key").toULongLong();
    if (!key)
        return;
    DBReqSnapshotsSaveSnapshot *req = new DBReqSnapshotsSaveSnapshot(item.value("title").toString(),
                                                           item.value("descr").toString(),
                                                           item.value("vehicle").toString(),
                                                           item.value("lat").toDouble(),
                                                           item.value("lon").toDouble(),
                                                           item.value("alt").toInt(),
                                                           item.value("lat_scr").toDouble(),
                                                           item.value("lon_scr").toDouble(),
                                                           item.value("date").toString(),
                                                           item.value("time").toString(),
                                                           item.value("filename").toString(),
                                                           item.value("file").toByteArray(),
                                                           item.value("file_preview").toByteArray(),
                                                           key);
    connect(req,
            &DatabaseRequest::finished,
            f_lookup,
            &DatabaseSnapshotLookup::defaultLookup,
            Qt::QueuedConnection);
    connect(
        req,
        &DBReqSnapshotsSaveSnapshot::snapshotModified,
        this,
        [](QString title) { apxMsg() << tr("Screenshot moved").append(':') << title; },
        Qt::QueuedConnection);
    req->exec();
}

/*void Snapshots::moveBackSnapshot(QVariantMap item)
{

    int i = f_lookup->dbModel()->indexOf("key", item.value("key").toULongLong());
    if (i < 0) {
        return;
    }
    if(f_lookup->dbModel()->get(i) != item){
        createEditor(item);
        f_lookup->dbModel()->set(i,item);
    }

}*/
void Snapshots::onItemTriggered(QVariantMap item)
{
    Q_UNUSED(item);
}

void Snapshots::pEditorFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    QProcess *process = qobject_cast<QProcess *>(sender());
    if(process){
        QStringList qsl = process->arguments();
    }
   apxMsg() << m_image_editor_name << exitCode;
}
void Snapshots::pOpenFolderFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitStatus);
    QProcess *process = qobject_cast<QProcess *>(sender());
    if(process){
        QStringList qsl = process->arguments();
    }
   apxMsg() << "dolphin" << exitCode;
}
void Snapshots::pEditorError(QProcess::ProcessError error)
{
   apxMsg() << m_image_editor_name << error;
   apxMsg() << pImageViewer.errorString();
}
void Snapshots::pOpenFolderError(QProcess::ProcessError error)
{
    apxMsg() << "dolphin" << error;
    apxMsg() << pOpenFolder.errorString();
}

void Snapshots::on_date_picker_editing_finished()
{
    DatePicker *picker = qobject_cast<DatePicker *>(sender());
    if(picker){
        f_filter_date->setDescr(picker->label()->text());
        if((date_picker->datePeriodBegin() == m_filter_date_beg_default  &&
            date_picker->datePeriodEnd() == m_filter_date_end_default  &&
            date_picker->timePeriodBegin() == m_filter_time_beg_default &&
            date_picker->timePeriodEnd() == m_filter_time_end_default) ||
                !f_filter_date->value().toBool()){
            f_default_filter_period->setEnabled(false);
        }else{
            f_default_filter_period->setEnabled(true);
        }
    }
}

void Snapshots::on_date_picker_canceling_finished()
{
    DatePicker *picker = qobject_cast<DatePicker *>(sender());
    if(picker){
       emit sig_snapshot_filter();
    }
}

void Snapshots::showImage(QVariantMap item)
{
    QString key = item.value("filename").toString();
    QString qsPath = AppDirs::images().absolutePath();

    QString filename = QString("%1/%2.%3").arg(qsPath, key, "png");

    if(!QFile::exists(filename))
    {
        QByteArray ba = item.value("file").toByteArray();
        QBuffer buffer(&ba);
        QImageReader reader(&buffer,"PNG");
        QImage image = reader.read();
        if(image.save(filename))
             apxMsg() <<  tr("Restore screenshot from database");
    }

    QStringList args;
    //pImageViewer.kill();
    args << m_unset_path << m_image_editor_name << filename;
    pImageViewer.setArguments(args);
    pImageViewer.startDetached();
   // pImageViewer.start();
}
void Snapshots::showFolder(QString qsPath)
{
    QString path = qsPath + "/";
    if(QDir(path).exists())
    {
       QStringList args;
       /*QString qsProg = pOpenFolder.program();
       if(qsProg.contains("/dolphin")){
            args << "--select" << path;
       }else{
           args << path;
       }*/

       args << m_unset_path << m_file_manager_name /*<< "--select"*/ << path;

       pOpenFolder.setArguments(args);
       pOpenFolder.startDetached();
    }
}

//=============================================================================
