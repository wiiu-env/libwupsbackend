/****************************************************************************
 * Copyright (C) 2018-2021 Maschell
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

#include <wups/config.h>
#include <string>
#include <vector>
#include <optional>
#include "WUPSConfigCategory.h"

class WUPSConfig {
public:
    /**
        \return Returns the name of this WUPSConfig
    **/
    [[nodiscard]] std::string getName() const {
        if (isDestroyed) {
            return "DESTROYED";
        }
        char buf[256];
        if (WUPSConfig_GetName(this->handle, buf, sizeof(buf)) == 0) {
            return buf;
        }
        return "ERROR";
    }

    /**
        \brief  Creates a new WUPSCategory add its to this WUPSConfig.
                The category will be added to the end of the list.
                This class holds responsibility for deleting the created instance.

        \param categoryName: The name of the category that will be created.

        \return On success, the created and inserted category will be returned.
    **/
    [[nodiscard]] std::optional<WUPSConfigCategory> addCategory(const std::string &categoryName) const {
        WUPSConfigCategoryHandle catHandle;
        if (WUPSConfig_AddCategoryByName(this->handle, categoryName.c_str(), &catHandle) < 0) {
            return {};
        }
        return WUPSConfigCategory(catHandle);
    }

    /**
        \brief  Adds a given WUPSConfigCategory to this WUPSConfig.
                The category will be added to the end of the list.
                The given category will be **always** deleted by the WUPSConfig

        \param category: The category that will be added to this config.

        \return On success, the inserted category will be returned.
    **/
    bool addCategory(WUPSConfigCategory &category) const {
        if (WUPSConfig_AddCategory(this->handle, category.getHandle()) == 0) {
            return true;
        }
        category.Destroy();
        return false;
    }

    static std::optional<WUPSConfig> Create(const std::string &name) {
        WUPSConfigHandle handle;
        if (WUPSConfig_Create(&handle, name.c_str()) == 0) {
            return WUPSConfig(handle);
        }
        return {};
    }

    void Destroy() {
        if (isDestroyed || this->handle == 0) {
            isDestroyed = true;
            return;
        }

        if (WUPSConfig_Destroy(this->handle) == 0) {
            isDestroyed = true;
        };
    }

    WUPSConfig(const WUPSConfig &other) = default;

    ~WUPSConfig() = default;

    [[nodiscard]] WUPSConfigHandle getHandle() const {
        return handle;
    }

private:
    explicit WUPSConfig(WUPSConfigHandle _handle) : handle(_handle) {
    }

    const WUPSConfigHandle handle;
    bool isDestroyed = false;
    std::string name;
};
