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
#ifndef AppSettings_H
#define AppSettings_H
//=============================================================================
#include <App/AppDirs.h>
#include <ApxMisc/DelayedEvent.h>
#include <Fact/Fact.h>
#include <QtCore>
//=============================================================================
class AppSettings : public Fact
{
    Q_OBJECT
public:
    explicit AppSettings(Fact *parent);

    Q_INVOKABLE void setDefaults(double lat, double lon, double zoom);
    static AppSettings *instance() { return _instance; }

    Fact *f_interface;
    Fact *f_graphics;
    Fact *f_application;

private:
    static AppSettings *_instance;
    DelayedEvent scaleEvent;
};
//=============================================================================
#endif
