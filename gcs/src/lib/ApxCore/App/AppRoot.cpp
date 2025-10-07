﻿/*
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
#include "AppRoot.h"
#include "AppWindow.h"
#include <App/App.h>
#include <App/AppLog.h>
#include <QtCore>
//=============================================================================
AppRoot *AppRoot::_instance = nullptr;
AppRoot::AppRoot(QObject *parent)
    : Fact(nullptr, "apx", tr("Application"), QCoreApplication::applicationName(), Root | FlatModel)
{
    _instance = this;
    if (parent)
        setParent(parent);

    f_settings = new AppSettings(this);

    createTools();
}
//=============================================================================
void AppRoot::sound(const QString &v)
{
    App::sound(v);
}
//=============================================================================
void AppRoot::createTools()
{
    //plugins management facts
    f_tools = new Fact(this, "tools", tr("Tools"), tr("Application tools"), Group | Const);
    f_tools->setIcon("dialpad");
    f_tools->setVisible(false);

    f_controls = new Fact(f_tools,
                          "controls",
                          tr("Controls"),
                          tr("Instruments and controls"),
                          Group | Const);
    f_controls->setIcon("animation");
    f_controls->setVisible(false);

    f_windows = new Fact(this, "windows", tr("Windows"), tr("Application windows"), Group | Const);
    f_windows->setIcon("monitor");
    f_windows->setVisible(false);

    App::jsync(this);

    f_pluginsSettings = new Fact(f_settings->f_application,
                                 "plugins",
                                 tr("Plugins"),
                                 tr("Application PlugIns"),
                                 Group | Const);
    App::jsync(f_settings);
}
//=============================================================================
void AppRoot::addToolPlugin(AppPlugin *plugin)
{
    Fact *f = qobject_cast<Fact *>(plugin->control);
    if (!f)
        return;
    if (f->parentFact())
        return;
    f->setParentFact(f_tools);
    f->setSection(plugin->section);
    f_tools->setVisible(true);
    App::jsync(f_tools);
}
//=============================================================================
void AppRoot::addWindowPlugin(AppPlugin *plugin)
{
    new AppWindow(f_windows, plugin);
    f_windows->setVisible(true);
    App::jsync(f_windows);
}
//=============================================================================
void AppRoot::addControlPlugin(AppPlugin *plugin)
{
    Fact *f = qobject_cast<Fact *>(plugin->control);
    if (!f)
        return;
    f->setParentFact(f_controls);
    f_controls->setVisible(true);
    App::jsync(f_controls);
}
//=============================================================================
void AppRoot::updateProgress(Fact *fact)
{
    if (!progressList.contains(fact)) {
        progressList.append(fact);
    }
    int total = 0;
    int cnt = 0;
    for (int i = 0; i < progressList.size(); ++i) {
        auto f = progressList.at(i);
        int v = f ? f->progress() : -1;
        if (v < 0) {
            progressList.removeAt(i--);
            continue;
        }
        cnt++;
        total += v;
    }
    int v = cnt > 0 ? total / cnt : -1;
    if (m_progress == v)
        return;
    m_progress = v;
    emit progressChanged();
}
//=============================================================================
//=============================================================================
// utils library
//=============================================================================
QString AppRoot::latToString(double v)
{
    double a = std::abs(v);
    double a_m = 60 * (a - std::floor(a)), a_s = 60 * (a_m - std::floor(a_m)),
           a_ss = 100 * (a_s - std::floor(a_s));
    return QString("%1 %2%3%4'%5.%6\"")
        .arg((v >= 0) ? 'N' : 'S') //
        .arg(static_cast<quint16>(std::floor(a)), 2, 10, QChar('0'))
        .arg(QChar(176))
        .arg(static_cast<quint16>(std::floor(a_m)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_s)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_ss)), 2, 10, QChar('0'));
}
QString AppRoot::lonToString(double v)
{
    double a = std::abs(v);
    double a_m = 60 * (a - std::floor(a)), a_s = 60 * (a_m - std::floor(a_m)),
           a_ss = 100 * (a_s - std::floor(a_s));
    return QString("%1 %2%3%4'%5.%6\"")
        .arg((v >= 0) ? 'E' : 'W') //
        .arg(static_cast<quint16>(std::floor(a)), 2, 10, QChar('0'))
        .arg(QChar(176))
        .arg(static_cast<quint16>(std::floor(a_m)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_s)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(a_ss)), 2, 10, QChar('0'));
}
QString AppRoot::latToStringDouble(double v)
{
    double a = std::abs(v);
    int i_a = a;
    double rest = (a - i_a)*10000;
    return QString("%1 %2.%3")
        .arg((v >= 0) ? 'N' : 'S') //
        .arg(static_cast<quint16>(std::floor(a)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(rest)), 4, 10, QChar('0'));
}
QString AppRoot::lonToStringDouble(double v)
{
    double a = std::abs(v);
    int i_a = a;
    double rest = (a - i_a)*10000;

    return QString("%1 %2.%3")
        .arg((v >= 0) ? 'E' : 'W') //
        .arg(static_cast<quint16>(std::floor(a)), 2, 10, QChar('0'))
        .arg(static_cast<quint16>(std::floor(rest)), 4, 10, QChar('0'));
}
double AppRoot::latFromString(QString s)
{
    if (s.isEmpty()) return 0.0;
    bool ok;
    int i;
    s = s.simplified();
    if (QString("NS").contains(s.at(0))) {
        bool bN = s.at(0) == 'N';
        s = s.remove(0, 1).trimmed();
        i = s.indexOf(QChar(176));
        double deg = s.left(i).toDouble(&ok);
        if (!ok)
            return 0;
        s = s.remove(0, i + 1).trimmed();
        i = s.indexOf('\'');
        double min = s.left(i).toDouble(&ok);
        if (!ok)
            return 0;
        s = s.remove(0, i + 1).trimmed();
        i = s.indexOf('\"');
        double sec = s.left(i).toDouble(&ok);
        if (!ok)
            return 0;
        deg = deg + min / 60.0 + sec / 3600.0;
        return bN ? deg : -deg;
    }
    return s.toDouble();
}
double AppRoot::lonFromString(QString s)
{
    if (s.isEmpty()) return 0.0;
    s = s.simplified();
    if (QString("EW").contains(s.at(0)))
        s[0] = (s.at(0) == 'E') ? 'N' : 'S';
    return latFromString(s);
}
double AppRoot::latFromStringDouble(QString s)
{
    bool ok;
    if(s != ""){
        s = s.simplified();
        if (QString("NS").contains(s.at(0))) {
            bool bN = s.at(0) == 'N';
            s = s.remove(0, 1).trimmed();
            double deg = s.toDouble(&ok);
            if (!ok){
                return 0;
            }
            return bN ? deg : -deg;
        }
        return s.toDouble();
    }else{
       return 0;
    }
}
double AppRoot::lonFromStringDouble(QString s)
{
    if(s != ""){
        s = s.simplified();
        if (QString("EW").contains(s.at(0)))
            s[0] = (s.at(0) == 'E') ? 'N' : 'S';
        return latFromStringDouble(s);
    }else{
        return 0;
    }
}

QString AppRoot::distanceToString(uint v, bool units)
{
    QString s, su = tr("km");
    if (v >= 1000000)
        s = QString("%1").arg(v / 1000.0, 0, 'f', 0);
    else if (v >= 1000)
        s = QString("%1").arg(v / 1000.0, 0, 'f', 1);
    else {
        s = QString("%1").arg(static_cast<ulong>(v));
        su = tr("m");
    }
    if (units)
        s.append(su);
    return s;
}
QString AppRoot::timeToString(quint64 v, bool seconds)
{
    //if(v==0)return "--:--";
    qint64 d = v / (24 * 60 * 60);
    const char *sf = seconds ? "hh:mm:ss" : "hh:mm";
    const int i = static_cast<int>(v);
    if (d <= 0)
        return QString("%1").arg(QTime(0, 0, 0).addSecs(i).toString(sf));
    return QString("%1%2%3").arg(d).arg(tr("d", "days")).arg(QTime(0, 0, 0).addSecs(i).toString(sf));
}
QString AppRoot::dateToString(quint64 v)
{
    QDateTime d = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(v));
    return d.toString("dd/MM/yy hh:mm:ss t");
}
QString AppRoot::timemsToString(quint64 v)
{
    quint64 ts = v / 1000;
    QString s;
    if (ts == 0)
        s = timeToString(1, false) + ":00";
    else
        s = timeToString(ts, true);
    s += QString(".%1").arg(v % 1000, 3, 10, QLatin1Char('0'));
    return s;
}
quint64 AppRoot::timeFromString(QString s)
{
    quint64 t = 0;
    s = s.trimmed().toLower();
    if (s.contains('d')) {
        QString ds = s.left(s.indexOf('d')).trimmed();
        s = s.remove(0, s.indexOf('d') + 1).trimmed();
        bool ok = false;
        double dv = ds.toDouble(&ok);
        if (ok && dv > 0)
            t += static_cast<quint64>(std::floor(dv * (24.0 * 60.0 * 60.0)));
    }
    if (s.contains('h')) {
        QString ds = s.left(s.indexOf('h')).trimmed();
        s = s.remove(0, s.indexOf('h') + 1).trimmed();
        bool ok = false;
        double dv = ds.toDouble(&ok);
        if (ok && dv > 0)
            t += static_cast<quint64>(std::floor(dv * (60.0 * 60.0)));
    }
    if (s.contains('m')) {
        QString ds = s.left(s.indexOf('m')).trimmed();
        s = s.remove(0, s.indexOf('m') + 1).trimmed();
        bool ok = false;
        double dv = ds.toDouble(&ok);
        if (ok && dv > 0)
            t += static_cast<quint64>(std::floor(dv * (60.0)));
    }
    if (s.contains('s')) {
        QString ds = s.left(s.indexOf('s')).trimmed();
        s = s.remove(0, s.indexOf('s') + 1).trimmed();
        bool ok = false;
        double dv = ds.toDouble(&ok);
        if (ok && dv > 0)
            t += static_cast<quint64>(std::floor(dv));
        s.clear();
    }
    if (s.contains(':')) {
        QString ds = s.left(s.indexOf(':')).trimmed();
        s = s.remove(0, s.indexOf(':') + 1).trimmed();
        bool ok = false;
        double dv = ds.toDouble(&ok);
        if (ok && dv > 0)
            t += static_cast<quint64>(std::floor(dv * (60.0 * 60.0)));
        if (s.contains(':')) {
            QString ds = s.left(s.indexOf(':')).trimmed();
            s = s.remove(0, s.indexOf(':') + 1).trimmed();
            bool ok = false;
            double dv = ds.toDouble(&ok);
            if (ok && dv > 0)
                t += static_cast<quint64>(std::floor(dv * (60.0)));
        } else {
            bool ok = false;
            double dv = s.toDouble(&ok);
            if (ok && dv > 0)
                t += static_cast<quint64>(std::floor(dv * (60.0)));
            s.clear();
        }
    }
    if (!s.isEmpty()) {
        bool ok = false;
        double dv = s.toDouble(&ok);
        if (ok && dv > 0)
            t += static_cast<quint64>(std::floor(dv));
    }
    return t;
}
//=============================================================================
QString AppRoot::capacityToString(quint64 v)
{
    QString s, su;
    if (v >= (1024 * 1024 * 1024)) {
        s = QString("%1").arg(v / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
        su = "GB";
    } else if (v >= (1024 * 1024)) {
        s = QString("%1").arg(v / (1024.0 * 1024.0), 0, 'f', 1);
        su = "MB";
    } else {
        s = QString("%1").arg(v / (1024.0), 0, 'f', 0);
        su = "kB";
    }
    s.append(su);
    return s;
}
//=============================================================================
double AppRoot::limit(double v, double min, double max)
{
    if (v < min)
        return min;
    if (v > max)
        return max;
    return v;
}
double AppRoot::angle(double v)
{
    const double span = 180.0;
    const double dspan = span * 2.0;
    return v - std::floor(v / dspan + 0.5) * dspan;
}
double AppRoot::angle360(double v)
{
    while (v < 0)
        v += 360.0;
    while (v >= 360.0)
        v -= 360.0;
    return v;
}
double AppRoot::angle90(double v)
{
    const double span = 90.0;
    const double dspan = span * 2.0;
    return v - std::floor(v / dspan + 0.5) * dspan;
}
//=============================================================================
QPointF AppRoot::rotate(const QPointF &p, double a)
{
    const double psi_r = qDegreesToRadians(a);
    double cos_theta = std::cos(psi_r);
    double sin_theta = std::sin(psi_r);
    return QPointF(p.x() * cos_theta + p.y() * sin_theta, p.y() * cos_theta - p.x() * sin_theta);
}
//=============================================================================
QPointF AppRoot::seriesBounds(const QVariantList &series)
{
    //qDebug()<<v;
    double min = 0, max = 0;
    for (int i = 0; i < series.size(); ++i) {
        double v = series.at(i).toDouble();
        if (i == 0) {
            min = max = v;
        } else {
            if (min > v)
                min = v;
            if (max < v)
                max = v;
        }
    }
    return QPointF(min, max);
}
//=============================================================================
