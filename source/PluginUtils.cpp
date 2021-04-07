/****************************************************************************
 * Copyright (C) 2019,2020 Maschell
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

#include <cstring>

#include "wups_backend/PluginUtils.h"
#include "imports.h"

std::optional<PluginMetaInformation> PluginUtils::getMetaInformationForBuffer(char *buffer, size_t size) {
    plugin_information info;
    memset(&info, 0, sizeof(info));
    if (WUPSGetPluginMetaInformationByBuffer(&info, buffer, size)) {
        // DEBUG_FUNCTION_LINE("Failed to load meta infos for buffer %08X with size %08X\n", buffer, size);
        return std::nullopt;
    }

    PluginMetaInformation metaInfo(info.name,
                                   info.author,
                                   info.version,
                                   info.license,
                                   info.buildTimestamp,
                                   info.description,
                                   info.id,
                                   info.size);

    return metaInfo;
}

std::optional<PluginMetaInformation> PluginUtils::getMetaInformationForPath(const std::string &path) {
    plugin_information info;
    memset(&info, 0, sizeof(info));
    if (WUPSGetPluginMetaInformationByPath(&info, path.c_str()) != PLUGIN_BACKEND_API_ERROR_NONE) {
        // DEBUG_FUNCTION_LINE("Failed to load meta infos for %s\n", path.c_str());
        return std::nullopt;
    }
    PluginMetaInformation metaInfo(info.name,
                                   info.author,
                                   info.version,
                                   info.license,
                                   info.buildTimestamp,
                                   info.description,
                                   info.id,
                                   info.size);
    return metaInfo;
}

std::optional<PluginContainer> PluginUtils::getPluginForPath(const std::string &path) {
    auto metaInfoOpt = PluginUtils::getMetaInformationForPath(path);
    if (!metaInfoOpt) {
        return std::nullopt;
    }

    plugin_data_handle dataHandle;
    if (WUPSLoadPluginAsDataByPath(&dataHandle, path.c_str()) != PLUGIN_BACKEND_API_ERROR_NONE) {
        // DEBUG_FUNCTION_LINE("Failed to load data");
        return std::nullopt;
    }

    return PluginContainer(PluginData(dataHandle), metaInfoOpt.value(), PLUGIN_BACKEND_API_ERROR_NONE);
}

std::optional<PluginContainer> PluginUtils::getPluginForBuffer(char *buffer, size_t size) {
    auto metaInfoOpt = PluginUtils::getMetaInformationForBuffer(buffer, size);
    if (!metaInfoOpt) {
        return std::nullopt;
    }

    plugin_data_handle dataHandle;
    if (WUPSLoadPluginAsDataByBuffer(&dataHandle, buffer, size) != PLUGIN_BACKEND_API_ERROR_NONE) {
        // DEBUG_FUNCTION_LINE("Failed to load data");
        return std::nullopt;
    }
    return PluginContainer(PluginData(dataHandle), metaInfoOpt.value(), 0);
}


std::vector<PluginContainer> PluginUtils::getLoadedPlugins(uint32_t maxSize) {
    std::vector<PluginContainer> result;
    auto *handles = (plugin_container_handle *) malloc(maxSize * sizeof(plugin_container_handle));
    if (handles == nullptr) {
        return result;
    }
    uint32_t realSize = 0;

    for (uint32_t i = 0; i < maxSize; i++) {
        handles[i] = 0xFFFFFFFF;
    }

    if (WUPSGetLoadedPlugins(handles, maxSize, &realSize) != PLUGIN_BACKEND_API_ERROR_NONE) {
        free(handles);
        // DEBUG_FUNCTION_LINE("Failed");
        return result;
    }
    if (realSize == 0) {
        free(handles);
        // DEBUG_FUNCTION_LINE("realsize is 0");
        return result;
    }

    auto *dataHandles = (plugin_data_handle *) malloc(realSize * sizeof(plugin_data_handle));
    if (!dataHandles) {
        free(handles);
        return result;
    }

    if (WUPSGetPluginDataForContainerHandles(handles, dataHandles, realSize) != PLUGIN_BACKEND_API_ERROR_NONE) {
        free(handles);
        free(dataHandles);
        // DEBUG_FUNCTION_LINE("Failed to get plugin data");
        return result;
    }

    auto *information = (plugin_information *) malloc(realSize * sizeof(plugin_information));
    if (!information) {
        free(handles);
        free(dataHandles);
        return result;
    }
    if (WUPSGetMetaInformation(handles, information, realSize) != PLUGIN_BACKEND_API_ERROR_NONE) {
        free(handles);
        free(dataHandles);
        free(information);
        // DEBUG_FUNCTION_LINE("Failed to get meta information for handles");
        return result;
    }

    for (uint32_t i = 0; i < realSize; i++) {
        PluginMetaInformation metaInfo(information[i].name,
                                       information[i].author,
                                       information[i].version,
                                       information[i].license,
                                       information[i].buildTimestamp,
                                       information[i].description,
                                       information[i].id,
                                       information[i].size);
        PluginData pluginData((uint32_t) dataHandles[i]);
        result.emplace_back(pluginData, metaInfo, handles[i]);
    }

    free(handles);
    free(dataHandles);
    free(information);

    return result;
}

void PluginUtils::destroyPluginContainer(PluginContainer &plugin) {
    std::vector<PluginContainer> list;
    list.push_back(plugin);
    return destroyPluginContainer(list);
}

void PluginUtils::destroyPluginContainer(std::vector<PluginContainer> &plugins) {
    uint32_t containerSize = plugins.size();
    uint32_t dataSize = containerSize;
    plugin_container_handle container_handles[containerSize];
    plugin_data_handle data_handles[dataSize];

    uint32_t cntC = 0;
    uint32_t cntD = 0;
    for (auto &plugin: plugins) {
        if (plugin.getHandle() != 0) {
            container_handles[cntC] = plugin.getHandle();
            cntC++;
        } else {
            containerSize--;
        }
        if (plugin.pluginData.getHandle() != 0) {
            data_handles[cntD] = plugin.pluginData.getHandle();
            cntD++;
        } else {
            dataSize--;
        }
    }
    WUPSDeletePluginContainer(container_handles, containerSize);
    WUPSDeletePluginData(data_handles, dataSize);
}

int32_t PluginUtils::LoadAndLinkOnRestart(std::vector<PluginContainer> &plugins) {
    uint32_t dataSize = plugins.size();
    plugin_data_handle handles[dataSize];
    int i = 0;
    for (auto &plugin: plugins) {
        plugin_data_handle handle = plugin.getPluginData().getHandle();
        if (handle == 0) {
            dataSize--;
        } else {
            handles[i] = handle;
            i++;
        }
    }

    return WUPSLoadAndLinkByDataHandle(handles, dataSize);;
}
