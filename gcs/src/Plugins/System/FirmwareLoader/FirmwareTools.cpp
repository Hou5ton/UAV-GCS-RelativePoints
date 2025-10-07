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
#include "FirmwareTools.h"
#include "Firmware.h"
#include "Releases.h"
#include <App/App.h>
#include <App/AppLog.h>
//=============================================================================
FirmwareTools::FirmwareTools(Firmware *firmware)
    : Fact(firmware, "tools", tr("Tools"), tr("Maintenance tools"), Action | IconOnly)
{
    setIcon("wrench");

    f_initialize = new Initialize(firmware, this);
    f_format = new Format(firmware, this);
}
//=============================================================================
