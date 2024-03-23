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
#include "logger.h"
#include "wups_backend/api.h"
#include <coreinit/debug.h>

using namespace WUPSBackend;

PluginData::PluginData(uint32_t handle) {
    this->mHandle = handle;
}

PluginData::PluginData(PluginData &&src) noexcept : mHandle(src.mHandle) {
    src.mHandle = {};
}

PluginData::~PluginData() {
    if (mHandle != 0) {
        if (WUPSBackend_DeletePluginData(&mHandle, 1) != PLUGIN_BACKEND_API_ERROR_NONE) {
            DEBUG_FUNCTION_LINE_ERR("Failed to delete plugin data");
        }
    }
}

PluginData &PluginData::operator=(PluginData &&src) noexcept {
    if (this != &src) {
        this->mHandle = src.mHandle;
        src.mHandle   = {};
    }
    return *this;
}

uint32_t PluginData::getHandle() const {
    return mHandle;
}
