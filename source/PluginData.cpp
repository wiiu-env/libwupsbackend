/****************************************************************************
 * Copyright (C) 2019-2022 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include "wups_backend/PluginData.h"
#include "imports.h"
#include "logger.h"
#include <coreinit/debug.h>

PluginData::PluginData(uint32_t handle) {
    this->handle = handle;
}

PluginData::~PluginData() {
    if (handle != 0) {
        if (WUPSDeletePluginData(&handle, 1) != PLUGIN_BACKEND_API_ERROR_NONE) {
            DEBUG_FUNCTION_LINE_ERR("### ERROR ###: Failed to delete plugin data");
        }
    }
}