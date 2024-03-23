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

#include <optional>
#include <string>
#include <vector>

namespace WUPSBackend {
class PluginMetaInformation {
public:
    [[nodiscard]] const std::string &getName() const {
        return name;
    }

    [[nodiscard]] const std::string &getAuthor() const {
        return this->author;
    }

    [[nodiscard]] const std::string &getVersion() const {
        return this->version;
    }

    [[nodiscard]] const std::string &getLicense() const {
        return this->license;
    }

    [[nodiscard]] const std::string &getBuildTimestamp() const {
        return this->buildtimestamp;
    }

    [[nodiscard]] const std::string &getDescription() const {
        return this->description;
    }

    [[nodiscard]] const std::string &getStorageId() const {
        return this->storageId;
    }

    [[nodiscard]] size_t getSize() const {
        return this->size;
    }

    PluginMetaInformation(std::string_view name,
                          std::string_view author,
                          std::string_view version,
                          std::string_view license,
                          std::string_view buildtimestamp,
                          std::string_view description,
                          std::string_view storageId,
                          size_t size);

private:
    PluginMetaInformation() = default;

    std::string name;
    std::string author;
    std::string version;
    std::string license;
    std::string buildtimestamp;
    std::string description;
    std::string storageId;
    size_t size{};
};
} // namespace WUPSBackend