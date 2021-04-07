/****************************************************************************
 * Copyright (C) 2019-2021 Maschell
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

#pragma once

#include <cstdint>

#include "PluginMetaInformation.h"
#include "PluginData.h"

class PluginContainer {

public:
    PluginContainer(const PluginData &data, PluginMetaInformation metaInfo, uint32_t handle) : pluginData(data), metaInformation(std::move(metaInfo)) {
        this->handle = handle;
    }

    [[nodiscard]] uint32_t getHandle() const {
        return this->handle;
    }

    [[nodiscard]] const PluginMetaInformation &getMetaInformation() const {
        return this->metaInformation;
    }

    [[nodiscard]] const PluginData &getPluginData() const {
        return pluginData;
    }

    const PluginData pluginData;
    const PluginMetaInformation metaInformation;
    uint32_t handle = 0;
};