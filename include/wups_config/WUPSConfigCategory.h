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
#include <wups/config.h>
#include "WUPSConfigItem.h"

class WUPSConfigCategory {
    friend class WUPSConfig;
public:
    /**
        \return Returns the name of this WUPSConfigCategory
    **/
    [[nodiscard]] std::string getName() const {
        if (isDestroyed) {
            return "ERROR";
        }
        char buf[256];
        if (WUPSConfigCategory_GetName(this->handle, buf, sizeof(buf)) == 0) {
            return buf;
        }
        return "ERROR";
    }

    /**
        \brief  Adds a given WUPSConfigItem to this WUPSConfigCategory.
                The item will be added to the end of the list.
                This class holds responsibility for deleting the created instance.

        \param item: The item that will be added to this config.

        \return On success, true will be returned.
                On error false will be returned. In this case the caller still has the responsibility
                for deleting the WUPSConfigItem instance.
    **/
    [[nodiscard]] bool addItem(const WUPSConfigItem &item) const {
        if (isDestroyed) {
            return false;
        }
        if (WUPSConfigCategory_AddItem(this->handle, item.getHandle())) {
            return true;
        }
        return false;
    }

    static WUPSConfigCategory *Create(const std::string &name) {
        WUPSConfigCategoryHandle handle;
        if (WUPSConfigCategory_Create(&handle, name.c_str()) == 0) {
            return new WUPSConfigCategory(handle);
        }
        return nullptr;
    }


    [[nodiscard]] WUPSConfigItemHandle getHandle() const {
        return this->handle;
    }


    void Destroy() {
        if (isDestroyed || this->handle == 0) {
            isDestroyed = true;
            return;
        }
        if (WUPSConfigCategory_Destroy(this->handle) == 0) {
            isDestroyed = true;
        };
    }

    WUPSConfigCategory(const WUPSConfigCategory &cat) = default;

    ~WUPSConfigCategory() = default;

private:
    explicit WUPSConfigCategory(WUPSConfigCategoryHandle _handle) : handle(_handle) {
    }

private:
    bool isDestroyed = false;
    const WUPSConfigCategoryHandle handle;
};
