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

#include <string>
#include <vector>
#include <optional>

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

    [[nodiscard]] const std::string &getId() const {
        return this->description;
    }

    [[nodiscard]] size_t getSize() const {
        return this->size;
    }

    PluginMetaInformation(const std::string& name,
                          const std::string& author,
                          const std::string& version,
                          const std::string& license,
                          const std::string& buildtimestamp,
                          const std::string& description,
                          const std::string& id,
                          size_t size);

private:
    PluginMetaInformation() = default;

    void setName(const std::string &name_) {
        this->name = name_;
    }

    void setAuthor(const std::string &author_) {
        this->author = author_;
    }

    void setVersion(const std::string &version_) {
        this->version = version_;
    }

    void setLicense(const std::string &license_) {
        this->license = license_;
    }

    void setBuildTimestamp(const std::string &buildtimestamp_) {
        this->buildtimestamp = buildtimestamp_;
    }

    void setDescription(const std::string &description_) {
        this->description = description_;
    }

    void setId(const std::string &id_) {
        this->id = id_;
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
    std::string id;
    size_t size{};
};
