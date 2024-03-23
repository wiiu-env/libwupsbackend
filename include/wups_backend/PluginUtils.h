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

const char *GetStatusStr(PluginBackendApiErrorType err);

namespace PluginUtils {

std::optional<PluginMetaInformation> getMetaInformationForBuffer(char *buffer, size_t size, PluginBackendApiErrorType &err);

std::optional<PluginMetaInformation> getMetaInformationForPath(const std::string &path, PluginBackendApiErrorType &err);

std::vector<PluginContainer> getLoadedPlugins(PluginBackendApiErrorType &err);

std::optional<PluginContainer> getPluginForPath(const std::string &path, PluginBackendApiErrorType &err);

std::optional<PluginContainer> getPluginForBuffer(char *buffer, size_t size, PluginBackendApiErrorType &err);

PluginBackendApiErrorType LoadAndLinkOnRestart(const std::vector<PluginContainer> &plugins);

} // namespace PluginUtils
} // namespace WUPSBackend