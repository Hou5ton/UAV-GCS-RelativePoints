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
#include "AppSettings.h"
#include <App/App.h>
#include <App/AppDirs.h>
//=============================================================================
AppSettings *AppSettings::_instance = nullptr;
AppSettings::AppSettings(Fact *parent)
    : Fact(parent,
           "settings",
           tr("Preferences"),
           tr("Application settings"),
           Group | FlatModel,
           "settings")
{
    _instance = this;

    f_interface = new Fact(this, "interface", tr("Interface"), "", Section);
    f_graphics = new Fact(this, "graphics", tr("Graphics"), "", Section);
    f_application = new Fact(this, "application", tr("Application"), "", Section);


    Fact *item;
    item = new Fact(f_interface,
                    "lang",
                    tr("Language"),
                    tr("Interface localization"),
                    Text | PersistentValue);
    QStringList st;
    st << "default";
    st.append(App::instance()->languages());
    item->setEnumStrings(st);

    item = new Fact(f_interface,
                    "coordinate_system",
                    tr("Coordinate system"),
                    "",
                    Enum | PersistentValue);
    item->setEnumStrings({tr("WGS84"), tr("SK42")});
    item->setDefaultValue(0);

    item = new Fact(f_graphics,
                    "scale",
                    tr("Scale"),
                    tr("Fonts scale [-1..+1]"),
                    Float | PersistentValue);
    item->setDefaultValue(1.0);
    //item->setPrecision(1);
    //item->setMin(0.5);
    //item->setMax(2.0);
    scaleEvent.setInterval(1000);
    connect(item, &Fact::valueChanged, &scaleEvent, &DelayedEvent::schedule);
    connect(&scaleEvent, &DelayedEvent::triggered, this, [item]() {
        App::instance()->setScale(item->value().toDouble());
    });
    App::instance()->setScale(item->value().toDouble());

    item = new Fact(f_graphics,
                    "opengl",
                    tr("Accelerate graphics"),
                    tr("Enable OpenGL graphics when supported"),
                    Enum | PersistentValue);
    st.clear();
    st << tr("default");
    st << "OpenGL";
    st << "OpenGL ES 2.0";
    st << "OpenVG";
    item->setEnumStrings(st);

    item = new Fact(f_graphics,
                    "smooth",
                    tr("Smooth animations"),
                    tr("Enable animations and antialiasing"),
                    Bool | PersistentValue);
    item->setDefaultValue(true);

    item = new Fact(f_graphics,
                    "antialiasing",
                    tr("Antialiasing"),
                    tr("Enable antialiasing"),
                    Enum | PersistentValue);
    st.clear();
    st << tr("off");
    st << tr("minimum");
    st << tr("maximum");
    item->setDefaultValue(st.at(2));
    item->setEnumStrings(st);

    item = new Fact(f_graphics,
                    "effects",
                    tr("Effects"),
                    tr("Graphical effects level"),
                    Enum | PersistentValue);
    item->setEnumStrings(st);
    item->setDefaultValue(st.at(2));

    Fact *f = new Fact(f_graphics, "test", tr("Highlight instruments"), "", Bool);
    f->setValue(false);
    f->setVisible(false);

    item = new Fact(f_interface,
                    "lat",
                    tr("Latitude"),
                    tr("Default Latitude"),
                    Float | PersistentValue);
    item->setDefaultValue(53.529167);
    // do not display
    item->setVisible(false);

    item = new Fact(f_interface,
                    "lon",
                    tr("Longitude"),
                    tr("Default Longitude"),
                    Float | PersistentValue);
    item->setDefaultValue(28.045);
    // do not display
    item->setVisible(false);

    item = new Fact(f_interface,
                    "zoom",
                    tr("Zoom level"),
                    tr("Zoom level"),
                    Float | PersistentValue);
    item->setDefaultValue(16);
    // do not display
    item->setVisible(false);

    App::jsync(this);

    App::jsexec(
        QString("ui.__defineGetter__('%1', function(){ return apx.settings.graphics.%1.value; });")
            .arg("smooth"));
    App::jsexec(
        QString("ui.__defineGetter__('%1', function(){ return apx.settings.graphics.%1.value; });")
            .arg("antialiasing"));
    App::jsexec(
        QString("ui.__defineGetter__('%1', function(){ return apx.settings.graphics.%1.value; });")
            .arg("effects"));
    App::jsexec(
        QString("ui.__defineGetter__('%1', function(){ return apx.settings.graphics.%1.value; });")
            .arg("test"));
}
void AppSettings::setDefaults(double lat, double lon, double zoom)
{
    if(f_interface){
        auto lat_ = f_interface->child("lat");
        auto lon_ = f_interface->child("lon");
        auto zoom_ = f_interface->child("zoom");
        if(lat_){
            lat_->setValue(lat);
        }
        if(lon_){
            lon_->setValue(lon);
        }
        if(zoom_){
            zoom_->setValue(zoom);
        }
    }
}
//=============================================================================
