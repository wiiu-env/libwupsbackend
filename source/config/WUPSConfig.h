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
        char buf[256];
        if (WUPSConfigCategory_GetName(this->handle, buf, sizeof(buf)) == 0) {
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
    std::optional<WUPSConfigCategory *> addCategory(const std::string &categoryName) {
        WUPSConfigCategoryHandle catHandle;
        if (WUPSConfig_AddCategoryByName(this->handle, categoryName.c_str(), &catHandle) < 0) {
            return {};
        }
        auto *curCat = new WUPSConfigCategory(catHandle);
        return curCat;
    }

    /**
        \brief  Adds a given WUPSConfigCategory to this WUPSConfig.
                The category will be added to the end of the list.
                This class holds responsibility for deleting the created instance.

        \param category: The category that will be added to this config.

        \return On success, the inserted category will be returned.
                On error NULL will be returned. In this case the caller still has the responsibility
                for deleting the WUPSConfigCategory instance.
    **/
    bool addCategory(WUPSConfigCategory *category) {
        if (WUPSConfig_AddCategory(this->handle, category->getHandle()) == 0) {
            return true;
        }
        return false;
    }

    static std::optional<WUPSConfig *> Create(const std::string &name) {
        WUPSConfigHandle handle;
        if (WUPSConfig_Create(&handle, name.c_str()) == 0) {
            return new WUPSConfig(handle);
        }
        return {};
    }


    static void Destroy(WUPSConfig *&config) {

    }

private:
    explicit WUPSConfig(WUPSConfigHandle _handle) : handle(_handle) {
    }

    ~WUPSConfig() = default;

    const WUPSConfigHandle handle;
    std::string name;
};
