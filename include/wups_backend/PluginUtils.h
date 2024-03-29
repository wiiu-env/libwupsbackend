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

#include "PluginContainer.h"
#include "PluginMetaInformation.h"
#include <optional>

namespace WUPSBackend {
    class PluginUtils {
    public:
        static std::optional<std::unique_ptr<PluginMetaInformation>> getMetaInformationForBuffer(char *buffer, size_t size);

        static std::optional<std::unique_ptr<PluginMetaInformation>> getMetaInformationForPath(const std::string &path);

        static std::vector<std::unique_ptr<PluginContainer>> getLoadedPlugins(uint32_t maxSize);

        static std::optional<std::unique_ptr<PluginContainer>> getPluginForPath(const std::string &path);

        static std::optional<std::unique_ptr<PluginContainer>> getPluginForBuffer(char *buffer, size_t size);

        static int32_t LoadAndLinkOnRestart(const std::vector<std::unique_ptr<PluginContainer>> &plugins);
    };
} // namespace WUPSBackend