#include <wups_backend/PluginContainer.h>


using namespace WUPSBackend;


PluginContainer::PluginContainer(std::shared_ptr<PluginData> data, std::shared_ptr<PluginMetaInformation> metaInfo) : pluginData(std::move(data)),
                                                                                                                      metaInformation(std::move(metaInfo)) {
}

[[nodiscard]] const std::shared_ptr<PluginMetaInformation> &PluginContainer::getMetaInformation() const {
    return this->metaInformation;
}

[[nodiscard]] const std::shared_ptr<PluginData> &PluginContainer::getPluginData() const {
    return pluginData;
}
