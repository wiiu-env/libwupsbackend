#include <config/WUPSConfigItemBoolean.h>
#include <config/WUPSConfigCategory.h>
#include "config/WUPSConfig.h"
/*
bool isRumbleActivated();

void rumbleChanged(WUPSConfigItemBoolean *item, bool newValue) {
}

WUPSConfig *getConfig() {
    auto config = WUPSConfig::Create("HID to VPAD");
    if (!config.has_value()) {
        return nullptr;
    }
    std::optional<WUPSConfigCategory *> catOther = config.value()->addCategory("Other");
    if (!catOther.has_value()) {
        WUPSConfig::Destroy(config.value());
        return nullptr;
    }
    if (!catOther.value()->addItem(WUPSConfigItemBoolean::Create("rumble", "Enable rumble", isRumbleActivated(), rumbleChanged))) {
        WUPSConfig::Destroy(config.value());
        return nullptr;
    }
    return config.value();
}

WUPSConfig *getConfig1() {
    auto config = WUPSConfig::Create("HID to VPAD");

    auto catOther = config.value()->addCategory("Other");

    catOther.value()->addItem(WUPSConfigItemBoolean::Create("rumble", "Enable rumble", isRumbleActivated(), rumbleChanged));

    return config.value();
}

bool isRumbleActivated() {
    return false;
}
*/