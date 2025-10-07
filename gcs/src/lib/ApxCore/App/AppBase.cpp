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
#include "AppBase.h"
#include "AppDirs.h"
#include "AppLog.h"
#include "AppRoot.h"
#include "Fact/Fact.h"

#include <version.h>
#include <QIcon>
#include <QFont>
#include <QFontMetrics>
//=============================================================================
#ifdef Q_OS_MAC
#include <mach-o/arch.h>
static QByteArray getCpuId()
{
    const NXArchInfo *info = NXGetLocalArchInfo();
    return QByteArray(reinterpret_cast<const char *>(info), sizeof(NXArchInfo));
}
#elif defined Q_OS_LINUX
static void getCpuIdArray(unsigned int *p, unsigned int ax)
{
    __asm __volatile("movl %%ebx, %%esi\n\t"
                     "cpuid\n\t"
                     "xchgl %%ebx, %%esi"
                     : "=a"(p[0]), "=S"(p[1]), "=c"(p[2]), "=d"(p[3])
                     : "0"(ax));
}
static QByteArray getCpuId()
{
    unsigned int info[4] = {0, 0, 0, 0};
    getCpuIdArray(info, 0);
    return QByteArray(reinterpret_cast<const char *>(info), sizeof(info));
}
#elif defined Q_OS_WIN
#include <machine_id.h>
static QByteArray getCpuId()
{
    unsigned int info[4] = {0, 0, 0, 0};
    __cpuid(info, 0);
    return QByteArray(reinterpret_cast<const char *>(info), sizeof(info));
}
#endif
//=============================================================================
AppBase *AppBase::_instance = nullptr;
//=============================================================================
AppBase::AppBase(int &argc, char **argv, const QString &name)
    : QApplication(argc, argv)
{
    _instance = this;

    setObjectName("application");

    QCommandLineParser parser;
    QCommandLineOption appName(QStringList() << "n" << "appname",
                                   QCoreApplication::translate("main", "Set application name <name>."),
                                   QCoreApplication::translate("main", "name"));
    parser.addOption(appName);
    parser.parse(QCoreApplication::arguments());
    QString applicationName = parser.value(appName);

    QCoreApplication::setOrganizationName("agat");
    QCoreApplication::setOrganizationDomain("agat.by");
    QCoreApplication::setApplicationName(applicationName.isEmpty() ? name : applicationName);
    QCoreApplication::setApplicationVersion(VERSION);
    QSettings::setDefaultFormat(QSettings::IniFormat);

    // app constants
    m_version = VERSION;
    apxConsole() << tr("Version").append(":") << m_version;
    m_branch = BRANCH;
    apxConsole() << tr("Branch").append(":") << m_branch;

    m_hash = GIT_HASH;
    m_time = GIT_TIME;
    m_year = GIT_YEAR;

    // identity
    m_hostname = QSysInfo::machineHostName();

    // machine ID
    QByteArray uid = QSysInfo::machineUniqueId();
    if (uid.isEmpty()) {
        uid = getCpuId();
    }
    uid.append(m_hostname.toUtf8());
    //m_machineUID=uid.toHex().toUpper();
    m_machineUID = QCryptographicHash::hash(uid, QCryptographicHash::Sha1).toHex().toUpper();

    // guess user name
    QString sname = "user";
    foreach (QString s, QProcess::systemEnvironment()) {
        if (!s.startsWith("USER"))
            continue;
        s = s.mid(s.indexOf('=') + 1).trimmed();
        if (s.isEmpty())
            break;
        sname = s;
        break;
    }
    m_username = sname;

    // check dry run
    QSettings sx;
    QString lastVer = sx.value("version").toString();
    m_dryRun = lastVer != version();
    if (m_dryRun) {
        apxConsole() << tr("First time run of this version");
        connect(this, &QCoreApplication::aboutToQuit, this, []() {
            QSettings().setValue("version", version());
        });
    }

    // check last segfault
    if (m_dryRun) {
        m_segfault = false;
    } else {
        m_segfault = sx.value("segfault").toString() != version();
        if (m_segfault) {
            apxConsoleW() << tr("Application didn't exit properly");
        }
    }
    sx.remove("segfault");
    connect(this, &QCoreApplication::aboutToQuit, this, []() {
        QSettings().setValue("segfault", version());
    });

    // check installation
    m_installed = false;
    QStringList destPaths(QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation));
    QStringList appPaths;
    QDir appDir(QCoreApplication::applicationDirPath());
    appPaths << appDir.absolutePath();
#if defined Q_OS_LINUX
    const QString appImage(qEnvironmentVariable("APPIMAGE"));
    if (!appImage.isEmpty() && QFile::exists(appImage)) {
        m_bundlePath = appImage;
        appPaths << appImage;
    }
    const QString destApp(QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation))
                              .absoluteFilePath("Applications"));
    destPaths << destApp;
    m_installDir = destApp;
#elif defined Q_OS_MACOS
    m_installDir = "/Applications";
    QDir bundleDir(appDir);
    bundleDir.cdUp();
    bundleDir.cdUp();
    if (bundleDir.dirName().endsWith(".app"))
        m_bundlePath = bundleDir.absolutePath();
#endif

    //    qDebug() << appPaths;
    //    qDebug() << destPaths;
    //    qDebug() << bundlePath();
    //    qDebug() << installDir();
    QString bundleName = QFileInfo(m_bundlePath).completeBaseName();
    if (!(bundleName.contains('-') || bundleName.contains('.'))) {
        for (auto p : destPaths) {
            if (m_installed)
                break;
            for (auto a : appPaths) {
                if (!a.startsWith(p))
                    continue;
                m_installed = true;
                break;
            }
        }
    }

    if (!m_installed) {
        apxConsoleW() << tr("Application is not installed");
    }
}
//=============================================================================
QString AppBase::aboutString()
{
    QStringList lines;
    lines << QString("%1: %2 %3")
                 .arg(tr("Version"))
                 .arg(QCoreApplication::applicationName())
                 .arg(version());
    lines << QString("%1: '%2' %3").arg(tr("Branch")).arg(branch()).arg(git_time());
    lines << QString("Qt: %1 (%2, %3 bit)")
                 .arg(QLatin1String(qVersion()))
                 .arg(compilerString())
                 .arg(QString::number(QSysInfo::WordSize));
    lines << QString("%1: %2 %3")
                 .arg(tr("Built"))
                 .arg(QLatin1String(__DATE__))
                 .arg(QLatin1String(__TIME__));
    lines << QString("%1: %2").arg(tr("Hash")).arg(git_hash());
    lines << QString("%1: %2 (%3@%4)")
                 .arg(tr("Machine"))
                 .arg(machineUID())
                 .arg(username())
                 .arg(hostname());

    const auto tools = AppRoot::instance()->f_tools;
    QList<Fact*> lstChildren = tools->findChildren<Fact*>();
    bool first = true;
    foreach(Fact* pChild, lstChildren) {
        Fact* version = nullptr;
        Fact* branch = nullptr;
        Fact* date = nullptr;
        Fact* time = nullptr;
        QString name = pChild->name();
        version = pChild->findChild("version",false);
        if(version){
            branch = pChild->findChild("branch");
            date = pChild->findChild("date");
            time = pChild->findChild("time");
            if( branch && date && time){
                if(first){
                    lines << QString("%1:").arg(tr("Plugins"));
                    first = false;
                }
                QFont font = QApplication::font();
                QFontMetrics metrics(font);
                int width = metrics.horizontalAdvance(tr("Plugins:"));
                QString blankText = QString(" ").repeated(width / metrics.horizontalAdvance(" "));
                lines <<  QString("%1 %2 %3 '%4' %5 %6").arg(blankText)
                          .arg(pChild->name())
                          .arg(version->value().toString()).
                          arg(branch->value().toString()).
                          arg(date->value().toString()).
                          arg(time->value().toString());
            }
        }

    }

    return lines.join('\n');
}
QString AppBase::compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang ") + QString::number(__clang_major__) + QLatin1Char('.')
           + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC ") + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER > 1999)
        return QLatin1String("MSVC <unknown>");
    if (_MSC_VER >= 1910)
        return QLatin1String("MSVC 2017");
    if (_MSC_VER >= 1900)
        return QLatin1String("MSVC 2015");
#else
    return QLatin1String("<unknown compiler>");
#endif
}
//=============================================================================
bool AppBase::install()
{
    if (installed()) {
        apxMsgW() << tr("Already installed");
        return false;
    }
    if (installDir().isEmpty()) {
        apxMsgW() << tr("Installation path is missing");
        return false;
    }
    if (!QFile::exists(bundlePath())) {
        apxMsgW() << tr("Bundle not exist").append(':') << bundlePath();
        return false;
    }
    apxMsg() << tr("Installing application").append("...");
    bool rv = false;

#if defined Q_OS_LINUX
    QFileInfo fiBundle(bundlePath());
    QString bundleFileName(fiBundle.fileName());
    QString bundleBaseName(fiBundle.baseName());
    QString bundleExt(fiBundle.suffix());
    if (bundleBaseName.contains('-')) {
        bundleBaseName = bundleBaseName.left(bundleBaseName.indexOf('-'));
        bundleFileName = bundleBaseName + "." + bundleExt;
    }
    QDir(installDir()).mkpath(".");
    QFileInfo fiBundleDest(QDir(installDir()).absoluteFilePath(bundleFileName));
    qDebug() << "cp" << fiBundleDest.absoluteFilePath();
    int execrv = QProcess::execute("cp",
                                   QStringList()
                                       << "-af" << bundlePath() << fiBundleDest.absoluteFilePath());
    rv = execrv == 0;
    if (rv) {
        //copy desktop files
        for (auto fi : QDir(AppDirs::res().absoluteFilePath("../applications"))
                           .entryInfoList(QStringList() << "*.desktop")) {
            QFileInfo fiDest(
                QDir(QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation))
                    .absoluteFilePath(fi.fileName()));
            qDebug() << "cp desktop" << fiDest.absoluteFilePath();
            if (fiDest.exists())
                QFile::remove(fiDest.absoluteFilePath());
            QDir(fiDest.absolutePath()).mkpath(".");
            QFile::copy(fi.absoluteFilePath(), fiDest.absoluteFilePath());

            //fix Exec path
            QSettings *sx = new QSettings(fiDest.absoluteFilePath(), QSettings::IniFormat);
            sx->beginGroup("Desktop Entry");
            sx->setValue("Exec", fiBundleDest.absoluteFilePath());
            sx->sync();
            delete sx;
            QFile f(fiDest.absoluteFilePath());
            f.open(QIODevice::Text | QIODevice::ReadOnly);
            QString data = f.readAll();
            f.close();
            data.replace("%20", " ");
            f.open(QFile::WriteOnly | QFile::Truncate);
            QTextStream out(&f);
            out << data;
            f.close();
        }
        AppDirs::copyPath(AppDirs::res().absoluteFilePath("../icons"),
                          QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                              + "/icons");
        //restart
        QProcess::startDetached(fiBundleDest.absoluteFilePath());
        exit();
    }

#elif defined Q_OS_MACOS

    apxMsgW() << tr("Manually move application to '%1' folder").arg(installDir());

#endif
    if (rv) {
        apxMsg() << tr("Application successfully installed");
    } else {
        apxMsgW() << tr("Application is not installed");
    }
    return rv;
}
//=============================================================================
