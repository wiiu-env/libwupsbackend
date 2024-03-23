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

#include "wups_backend/PluginMetaInformation.h"

#include <cstring>
#include <utility>

using namespace WUPSBackend;

PluginMetaInformation::PluginMetaInformation(std::string_view name,
                                             std::string_view author,
                                             std::string_view version,
                                             std::string_view license,
                                             std::string_view buildtimestamp,
                                             std::string_view description,
                                             std::string_view storageId,
                                             size_t size) {
    this->name           = name;
    this->author         = author;
    this->size           = size;
    this->buildtimestamp = buildtimestamp;
    this->description    = description;
    this->license        = license;
    this->version        = version;
    this->storageId      = storageId;
}
