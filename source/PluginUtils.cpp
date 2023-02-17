/****************************************************************************
 * Copyright (C) 2019-2022 Maschell
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

#include "wups_backend/PluginUtils.h"
#include "imports.h"
#include "logger.h"
#include "utils.h"
#include <cstring>
#include <memory>

using namespace WUPSBackend;

std::optional<std::unique_ptr<PluginMetaInformation>> getMetaInformation(const plugin_information &info) {
    if (info.plugin_information_version != PLUGIN_INFORMATION_VERSION) {
        DEBUG_FUNCTION_LINE_ERR("Version mismatch");
        return {};
    }
    auto res = make_unique_nothrow<PluginMetaInformation>(info.name,
                                                          info.author,
                                                          info.version,
                                                          info.license,
                                                          info.buildTimestamp,
                                                          info.description,
                                                          info.storageId,
                                                          info.size);
    if (!res) {
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return {};
    }
    return res;
}

std::optional<std::unique_ptr<PluginMetaInformation>> PluginUtils::getMetaInformationForBuffer(char *buffer, size_t size) {
    plugin_information info = {};
    if (WUPSGetPluginMetaInformationByBuffer(&info, buffer, size) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to load meta infos for buffer %08X with size %08X", buffer, size);
        return {};
    }

    return getMetaInformation(info);
}

std::optional<std::unique_ptr<PluginMetaInformation>> PluginUtils::getMetaInformationForPath(const std::string &path) {
    plugin_information info = {};
    if (WUPSGetPluginMetaInformationByPath(&info, path.c_str()) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to load meta infos for %s", path.c_str());
        return {};
    }

    return getMetaInformation(info);
}

std::optional<std::unique_ptr<PluginContainer>> PluginUtils::getPluginForPath(const std::string &path) {
    auto metaInfoOpt = PluginUtils::getMetaInformationForPath(path);
    if (!metaInfoOpt) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get MetaInformation for path %s", path.c_str());
        return {};
    }

    plugin_data_handle dataHandle;
    if (WUPSLoadPluginAsDataByPath(&dataHandle, path.c_str()) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("WUPSLoadPluginAsDataByPath failed for path %s", path.c_str());
        return {};
    }

    auto pluginData = make_shared_nothrow<PluginData>(dataHandle);
    if (!pluginData) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate PluginData");
        return {};
    }

    auto pluginContainer = make_unique_nothrow<PluginContainer>(std::move(pluginData), std::move(metaInfoOpt.value()));
    if (!pluginContainer) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate PluginContainer");
        return {};
    }

    return pluginContainer;
}

std::optional<std::unique_ptr<PluginContainer>> PluginUtils::getPluginForBuffer(char *buffer, size_t size) {
    auto metaInfoOpt = PluginUtils::getMetaInformationForBuffer(buffer, size);
    if (!metaInfoOpt) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get MetaInformation for buffer %08X (%d bytes)", buffer, size);
        return {};
    }

    plugin_data_handle dataHandle;
    if (WUPSLoadPluginAsDataByBuffer(&dataHandle, buffer, size) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("WUPSLoadPluginAsDataByBuffer failed for buffer %08X (%d bytes)", buffer, size);
        return {};
    }

    auto pluginData = make_shared_nothrow<PluginData>(dataHandle);
    if (!pluginData) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate PluginData");
        return {};
    }

    auto pluginContainer = make_unique_nothrow<PluginContainer>(std::move(pluginData), std::move(metaInfoOpt.value()));
    if (!pluginContainer) {
        DEBUG_FUNCTION_LINE_ERR("Failed to allocate PluginContainer");
        return {};
    }

    return pluginContainer;
}


std::vector<std::unique_ptr<PluginContainer>> PluginUtils::getLoadedPlugins(uint32_t maxSize) {
    std::vector<std::unique_ptr<PluginContainer>> result;

    auto handles = make_unique_nothrow<plugin_container_handle[]>(maxSize);
    if (!handles) {
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return result;
    }
    uint32_t realSize = 0;

    for (uint32_t i = 0; i < maxSize; i++) {
        handles[i] = 0xFFFFFFFF;
    }

    uint32_t plugin_information_version = 0;

    if (WUPSGetLoadedPlugins(handles.get(), maxSize, &realSize, &plugin_information_version) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("WUPSGetLoadedPlugins: Failed");
        return result;
    }
    if (realSize == 0 || plugin_information_version != PLUGIN_INFORMATION_VERSION) {
        DEBUG_FUNCTION_LINE_ERR("realSize is 0 or version mismatch");
        return result;
    }

    auto dataHandles = make_unique_nothrow<plugin_data_handle[]>(realSize);
    if (!dataHandles) {
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return result;
    }

    if (WUPSGetPluginDataForContainerHandles(handles.get(), dataHandles.get(), realSize) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get plugin data");
        return result;
    }

    auto information = make_unique_nothrow<plugin_information[]>(realSize);
    if (!information) {
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return result;
    }
    if (WUPSGetMetaInformation(handles.get(), information.get(), realSize) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get meta information for handles");
        return result;
    }

    for (uint32_t i = 0; i < realSize; i++) {
        if (information[i].plugin_information_version != PLUGIN_INFORMATION_VERSION) {
            DEBUG_FUNCTION_LINE_ERR("Skip, wrong struct version.");
            continue;
        }
        auto metaInfo = make_shared_nothrow<PluginMetaInformation>(information[i].name,
                                                                   information[i].author,
                                                                   information[i].version,
                                                                   information[i].license,
                                                                   information[i].buildTimestamp,
                                                                   information[i].description,
                                                                   information[i].storageId,
                                                                   information[i].size);

        if (!metaInfo) {
            DEBUG_FUNCTION_LINE_ERR("Skip, failed to allocate MetaInformation");
            continue;
        }

        auto pluginData = std::make_shared<PluginData>((uint32_t) dataHandles[i]);
        if (!pluginData) {
            DEBUG_FUNCTION_LINE_ERR("Skip, failed to allocate PluginData");
            continue;
        }

        auto pluginContainer = make_unique_nothrow<PluginContainer>(std::move(pluginData), std::move(metaInfo));
        if (!pluginContainer) {
            DEBUG_FUNCTION_LINE_ERR("Skip, failed to allocate PluginContainer");
            continue;
        }

        result.push_back(std::move(pluginContainer));
    }

    return result;
}

int32_t PluginUtils::LoadAndLinkOnRestart(const std::vector<std::unique_ptr<PluginContainer>> &plugins) {
    uint32_t dataSize = plugins.size();
    plugin_data_handle handles[dataSize];
    int i = 0;
    for (auto &plugin : plugins) {
        plugin_data_handle handle = plugin->getPluginData()->getHandle();
        if (handle == 0) {
            dataSize--;
        } else {
            handles[i] = handle;
            i++;
        }
    }

    return WUPSLoadAndLinkByDataHandle(handles, dataSize);
}
