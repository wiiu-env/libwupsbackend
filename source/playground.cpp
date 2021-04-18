#include <config/WUPSConfigItemBoolean.h>
#include <config/WUPSConfigCategory.h>
#include "config/WUPSConfig.h"

bool isRumbleActivated();

void rumbleChanged(WUPSConfigItemBoolean *item, bool newValue) {
}

WUPSConfigHandle getConfig() {
    auto config = WUPSConfig::Create("HID to VPAD");
    if (!config.has_value()) {
        return 0;
    }
    std::optional<WUPSConfigCategory> catOther = config->addCategory("Other");
    if (!catOther.has_value()) {
        WUPSConfig::Destroy(config.value());
        return 0;
    }
    if (!catOther->addItem(WUPSConfigItemBoolean::Create("rumble", "Enable rumble", isRumbleActivated(), rumbleChanged).value())) {
        WUPSConfig::Destroy(config.value());
        return 0;
    }
    return config.value().getHandle();
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
