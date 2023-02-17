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

        PluginMetaInformation(std::string name,
                              std::string author,
                              std::string version,
                              std::string license,
                              std::string buildtimestamp,
                              std::string description,
                              std::string storageId,
                              size_t size);

    private:
        PluginMetaInformation() = default;

        void setName(std::string name_) {
            this->name = std::move(name_);
        }

        void setAuthor(std::string author_) {
            this->author = std::move(author_);
        }

        void setVersion(std::string version_) {
            this->version = std::move(version_);
        }

        void setLicense(std::string license_) {
            this->license = std::move(license_);
        }

        void setBuildTimestamp(std::string buildtimestamp_) {
            this->buildtimestamp = std::move(buildtimestamp_);
        }

        void setDescription(std::string description_) {
            this->description = std::move(description_);
        }

        void setStorageId(std::string storageId_) {
            this->storageId = std::move(storageId_);
        }

        void setSize(size_t size_) {
            this->size = size_;
        }

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