/****************************************************************************
 * Copyright (C) 2019,2020 Maschell
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

PluginMetaInformation::PluginMetaInformation(const std::string &name_,
                                             const std::string &author_,
                                             const std::string &version_,
                                             const std::string &license_,
                                             const std::string &buildtimestamp_,
                                             const std::string &description_,
                                             const std::string &storageId_,
                                             size_t size_) {
    this->name           = name_;
    this->author         = author_;
    this->size           = size_;
    this->buildtimestamp = buildtimestamp_;
    this->description    = description_;
    this->license        = license_;
    this->version        = version_;
    this->storageId      = storageId_;
}
