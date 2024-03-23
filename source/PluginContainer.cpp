#include <wups_backend/PluginContainer.h>


using namespace WUPSBackend;

PluginContainer::PluginContainer(PluginData data, PluginMetaInformation metaInfo) : pluginData(std::move(data)),
                                                                                    metaInformation(std::move(metaInfo)) {
}

[[nodiscard]] const PluginMetaInformation &PluginContainer::getMetaInformation() const {
    return this->metaInformation;
}

[[nodiscard]] const PluginData &PluginContainer::getPluginData() const {
    return pluginData;
}
