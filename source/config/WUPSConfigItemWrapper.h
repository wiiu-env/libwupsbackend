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

#include <wups_config/config.h>
#include "config_imports.h"

class WUPSConfigItemWrapper {
public:

    static int32_t getCurrentValueDisplay(void *context, char *out_buf, int32_t out_size) {
        if (context == nullptr || out_buf == nullptr) {
            return -1;
        }
        auto *ptr = static_cast<WUPSConfigItem *>(context);
        std::string result = ptr->getCurrentValueDisplay();
        snprintf(out_buf, out_size, "%s", result.c_str());
        return 0;
    }

    static int32_t getCurrentValueSelectedDisplay(void *context, char *out_buf, int32_t out_size) {
        if (context == nullptr || out_buf == nullptr) {
            return -1;
        }
        auto *ptr = static_cast<WUPSConfigItem *>(context);
        std::string result = ptr->getCurrentValueSelectedDisplay();
        snprintf(out_buf, out_size, "%s", result.c_str());
        return 0;
    }

    static bool callCallback(void *context) {
        if (context == nullptr) {
            return false;
        }
        auto *ptr = static_cast<WUPSConfigItem *>(context);
        return ptr->callCallback();
    }

    static bool isMovementAllowed(void *context) {
        if (context == nullptr) {
            return false;
        }
        auto *ptr = static_cast<WUPSConfigItem *>(context);
        return ptr->isMovementAllowed();
    }

    static void onButtonPressed(void *context, WUPSConfigButtons buttons) {
        if (context == nullptr) {
            return;
        }
        auto *ptr = static_cast<WUPSConfigItem *>(context);
        ptr->onButtonPressed(buttons);
    }

    static void restoreDefault(void *context) {
        if (context == nullptr) {
            return;
        }
        auto *ptr = static_cast<WUPSConfigItem *>(context);
        ptr->restoreDefault();
    }

    static bool Create(const std::string &_configID, const std::string &_displayName, WUPSConfigItem &item) {
        WUPSConfigCallbacks_t callbacks = {};
        callbacks.getCurrentValueDisplay = &WUPSConfigItemWrapper::getCurrentValueDisplay;
        callbacks.callCallback = &WUPSConfigItemWrapper::callCallback;
        callbacks.getCurrentValueSelectedDisplay = &WUPSConfigItemWrapper::getCurrentValueSelectedDisplay;
        callbacks.isMovementAllowed = &WUPSConfigItemWrapper::isMovementAllowed;
        callbacks.onButtonPressed = &WUPSConfigItemWrapper::onButtonPressed;
        callbacks.restoreDefault = &WUPSConfigItemWrapper::restoreDefault;
        WUPSConfigItemHandle handle;
        if (WUPSConfigItem_Create(&handle, _configID.c_str(), _displayName.c_str(), callbacks, (void *) &item) == 0) {
            item.setHandle(handle);
            return true;
        }
        return false;
    }
};
