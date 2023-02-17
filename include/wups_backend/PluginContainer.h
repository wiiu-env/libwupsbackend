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

#pragma once

#include <cstdint>
#include <memory>
#include <utility>

#include "PluginData.h"
#include "PluginMetaInformation.h"

namespace WUPSBackend {
    class PluginContainer {

    public:
        PluginContainer(std::shared_ptr<PluginData> data, std::shared_ptr<PluginMetaInformation> metaInfo) : pluginData(std::move(data)),
                                                                                                             metaInformation(std::move(metaInfo)) {
        }

        [[nodiscard]] const std::shared_ptr<PluginMetaInformation> &getMetaInformation() const {
            return this->metaInformation;
        }

        [[nodiscard]] const std::shared_ptr<PluginData> &getPluginData() const {
            return pluginData;
        }

        const std::shared_ptr<PluginData> pluginData;
        const std::shared_ptr<PluginMetaInformation> metaInformation;
    };
} // namespace WUPSBackend