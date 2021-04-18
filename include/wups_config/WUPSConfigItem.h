/****************************************************************************
 * Copyright (C) 2021 Maschell
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
#include "../../../WiiUPluginSystem/include/wups_config/config_imports.h"

class WUPSConfigItem {
public:
    /**
        Sets the display name of this WUPSConfigItem
        This is the value which will be shown in the configuration menu.
    **/
    void setDisplayName(const std::string &_displayName) const {
        WUPSConfigItem_SetDisplayName(this->handle, _displayName.c_str());
    }

    /**
        \return Returns the display name of this WUPSConfigItem
    **/
    [[nodiscard]] std::string getDisplayName() const {
        char buf[256];
        if (WUPSConfigItem_GetDisplayName(this->handle, buf, sizeof(buf)) == 0) {
            return buf;
        }
        return "[ERROR]";
    }

    /**
        Sets the config ID name of this WUPSConfigItem.
        This config ID is used to persist the configuration values and needs
        to be unique in the context of this WUPSConfig.
        Items in different categories are NOT allowed to have the config ID.
    **/
    virtual void setConfigID(const std::string &_configID) {
        WUPSConfigItem_SetConfigID(this->handle, _configID.c_str());
    }

    /**
        \return Returns the configID of this WUPSConfigItem.
    **/
    [[nodiscard]] virtual std::string getConfigID() {
        char buf[256];
        if (WUPSConfigItem_GetConfigID(this->handle, buf, sizeof(buf)) == 0) {
            return buf;
        }
        return "[ERROR]";
    }

    /**
        Returns a string that displays the current value.
        This string is shown next to the display name when the cursor is NOT on this item
    **/
    [[nodiscard]] virtual std::string getCurrentValueDisplay() = 0;

    /**
        Returns a string that displays the current value when selected.
        This string is shown next to the display name when the cursor IS on this item
    **/
    [[nodiscard]] virtual std::string getCurrentValueSelectedDisplay() = 0;

    /**
        Is called when the cursor enters or leaves the item.
        When the cursor enters the item, "isSelected" will be true.
        When the cursor leaves the item, "isSelected" will be false.
    **/
    virtual void onSelected(bool isSelected) = 0;

    /**
        Is called when a button is pressed while the cursor on this item.
        See the WUPSConfigButtons enum for possible values.
    **/
    virtual void onButtonPressed(WUPSConfigButtons buttons) = 0;

    /**
        When the cursor is on this item, the configuration menu asks this item
        if it's allowed to leave it.
        If it returns true, the item can be leaved.
        It it returns false, leaves is not allowed.
    **/
    [[nodiscard]] virtual bool isMovementAllowed() = 0;

    /**
        Restores the default value
    **/
    virtual void restoreDefault() = 0;

    /**
        Call callback with with current value.
        This function will be called whenever this item should call it's (optional) given
        callback with the current value.
        Returns true if a valid callback could be called
        Returns false if no callback was called (e.g. callback was NULL)
    **/
    virtual bool callCallback() = 0;

    virtual WUPSConfigItem* cloneOnHeap() = 0;

    virtual ~WUPSConfigItem() = default;

    void setHandle(WUPSConfigItemHandle _handle) {
        this->handle = _handle;
    }

    [[nodiscard]] WUPSConfigItemHandle getHandle() const {
        return this->handle;
    }

private:
    WUPSConfigItemHandle handle{};
};
