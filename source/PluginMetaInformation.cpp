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
#include <optional>
#include <utility>

PluginMetaInformation::PluginMetaInformation(std::string name,
                                             std::string author,
                                             std::string version,
                                             std::string license,
                                             std::string buildtimestamp,
                                             std::string description,
                                             std::string storageId,
                                             size_t size) {
    this->name           = std::move(name);
    this->author         = std::move(author);
    this->size           = size;
    this->buildtimestamp = std::move(buildtimestamp);
    this->description    = std::move(description);
    this->license        = std::move(license);
    this->version        = std::move(version);
    this->storageId      = std::move(storageId);
}
