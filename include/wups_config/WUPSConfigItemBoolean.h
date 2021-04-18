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
#include <optional>
#include <wups/config.h>
#include "WUPSConfigItem.h"
#include "WUPSConfigItemWrapper.h"

class WUPSConfigItemBoolean;

typedef void (*BooleanValueChangedCallback)(WUPSConfigItemBoolean *, bool);

class WUPSConfigItemBoolean : public WUPSConfigItem {
public:
    ~WUPSConfigItemBoolean() override;

    /**
        Sets the name with will be displayed as "true" value
        \param name of the "true" value
    **/
    void setTrueValueName(const std::string &_trueValName) {
        this->trueValName = _trueValName;
    }

    /**
        Sets the name with will be displayed as "false" value
        \param name of the "false" value
    **/
    void setFalseValueName(const std::string &_falseValName) {
        this->falseValName = _falseValName;
    }

    /**
        Toggles the value. When it was true, it now false, when it was false, it's now true.
        Call the callback with the new value.
    **/
    virtual void toggleValue() {
        value = !value;
    }

    [[nodiscard]] std::string getCurrentValueDisplay() override {
        if (value) {
            return std::string("  ").append(trueValName);
        } else {
            return std::string("  ").append(falseValName);
        }
    }

    [[nodiscard]] std::string getCurrentValueSelectedDisplay() override {
        if (value) {
            return std::string("  ").append(trueValName).append(" >");
        } else {
            return std::string("< ").append(falseValName);
        }
    }

    void onSelected(bool isSelected) override {

    }

    void onButtonPressed(WUPSConfigButtons buttons) override {
        if ((buttons & WUPS_CONFIG_BUTTON_A) == WUPS_CONFIG_BUTTON_A) {
            toggleValue();
        } else if (buttons & WUPS_CONFIG_BUTTON_LEFT && !value) {
            toggleValue();
        } else if ((buttons & WUPS_CONFIG_BUTTON_RIGHT) && value) {
            toggleValue();
        }
    }

    bool isMovementAllowed() override {
        return true;
    }

    void restoreDefault() override {
        this->value = this->defaultValue;
    }

    bool callCallback() override {
        if (callback != nullptr) {
            callback(this, this->value);
            return true;
        }
        return false;
    }
    WUPSConfigItem* cloneOnHeap() override {
        auto res = new WUPSConfigItemBoolean(this->defaultValue, this->callback);
        res->setHandle(this->getHandle());
        return res;
    }

    static std::optional<WUPSConfigItemBoolean> Create(const std::string &configID, const std::string &displayName, bool defaultValue, BooleanValueChangedCallback callback) {
        WUPSConfigItemBoolean item(defaultValue, callback);

        if (WUPSConfigItemWrapper::Create(configID, displayName, item) == 0) {
            return item;
        } else {
            return {};
        }
    }

private:
    WUPSConfigItemBoolean(bool defaultValue, BooleanValueChangedCallback callback) {
        this->defaultValue = defaultValue;
        this->value = defaultValue;
        this->callback = callback;
    }

    BooleanValueChangedCallback callback = nullptr;
    bool value;
    bool defaultValue;
    std::string trueValName = "on";
    std::string falseValName = "off";
};
