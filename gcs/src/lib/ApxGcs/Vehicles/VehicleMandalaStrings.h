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

QT_TRANSLATE_NOOP("MandalaUnits", "deg");
QT_TRANSLATE_NOOP("MandalaUnits", "deg/s");

QT_TRANSLATE_NOOP("MandalaUnits", "m");
QT_TRANSLATE_NOOP("MandalaUnits", "m/s");
QT_TRANSLATE_NOOP("MandalaUnits", "m/s2");
QT_TRANSLATE_NOOP("MandalaUnits", "m/s^2");

QT_TRANSLATE_NOOP("MandalaUnits", "a.u.");

QT_TRANSLATE_NOOP("MandalaUnits", "1/min");
QT_TRANSLATE_NOOP("MandalaUnits", "rpm");

QT_TRANSLATE_NOOP("MandalaUnits", "Lift/Drag");

QT_TRANSLATE_NOOP("MandalaUnits", "kPa");

QT_TRANSLATE_NOOP("MandalaUnits", "l");
QT_TRANSLATE_NOOP("MandalaUnits", "l/h");

QT_TRANSLATE_NOOP("MandalaUnits", "V");
QT_TRANSLATE_NOOP("MandalaUnits", "v");
QT_TRANSLATE_NOOP("MandalaUnits", "A");

QT_TRANSLATE_NOOP3("MandalaUnits", "C", "C");

QT_TRANSLATE_NOOP("MandalaUnits", "atm");

QT_TRANSLATE_NOOP3("MandalaUnits", "K", "K");

QT_TRANSLATE_NOOP("MandalaUnits", "number");

QT_TRANSLATE_NOOP("MandalaUnits", "ms");
QT_TRANSLATE_NOOP("MandalaUnits", "sec");

#define MIDX(aname, adescr, ...) (void) QT_TRANSLATE_NOOP3("MandalaVars", adescr, #aname);

#define MVAR(atype, aname, adescr, ...) (void) QT_TRANSLATE_NOOP3("MandalaVars", adescr, #aname);

#define MBIT(atype, aname, adescr, ...) \
    (void) QT_TRANSLATE_NOOP3("MandalaVars", adescr, #atype "_" #aname); \
    (void) QT_TRANSLATE_NOOP3("MandalaVars", #aname, #atype "_" #aname);

#include "../../../../../lib/Mandala/flat/MandalaTemplate.h"
