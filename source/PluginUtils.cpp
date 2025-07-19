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
#include "logger.h"
#include "utils.h"
#include "wups_backend/api.h"
#include <cstring>
#include <memory>

namespace WUPSBackend {

const char *GetStatusStr(PluginBackendApiErrorType status) {
    return WUPSBackend_GetStatusStr(status);
}

namespace PluginUtils {
static std::optional<PluginMetaInformation> getMetaInformation(const wups_backend_plugin_information &info, PluginBackendApiErrorType &err) {
    if (info.plugin_information_version != WUPS_BACKEND_PLUGIN_INFORMATION_VERSION) {
        err = PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION;
        DEBUG_FUNCTION_LINE_ERR("Version mismatch");
        return {};
    }
    return PluginMetaInformation(info.name,
                                 info.author,
                                 info.version,
                                 info.license,
                                 info.buildTimestamp,
                                 info.description,
                                 info.storageId,
                                 info.size);
}

std::optional<PluginMetaInformation> getMetaInformationForBuffer(char *buffer, size_t size, PluginBackendApiErrorType &err, PluginBackendPluginParseError &parseErr) {
    wups_backend_plugin_information info = {};
    if ((err = WUPSBackend_GetPluginMetaInformationByBuffer(&info, buffer, size, &parseErr)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to load meta infos for buffer %p with size %08X", buffer, size);
        return {};
    }
    return getMetaInformation(info, err);
}

std::optional<PluginMetaInformation> getMetaInformationForPath(const std::string &path, PluginBackendApiErrorType &err, PluginBackendPluginParseError &parseErr) {
    wups_backend_plugin_information info = {};
    if ((err = WUPSBackend_GetPluginMetaInformationByPath(&info, path.c_str(), &parseErr)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to load meta infos for %s", path.c_str());
        return {};
    }
    return getMetaInformation(info, err);
}

std::optional<PluginContainer> getPluginForPath(const std::string &path, PluginBackendApiErrorType &err, PluginBackendPluginParseError &parseErr) {
    auto metaInfoOpt = getMetaInformationForPath(path, err, parseErr);
    if (!metaInfoOpt) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get MetaInformation for path %s", path.c_str());
        return {};
    }

    wups_backend_plugin_data_handle dataHandle;
    if ((err = WUPSBackend_LoadPluginAsDataByPath(&dataHandle, path.c_str())) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("WUPSLoadPluginAsDataByPath failed for path %s", path.c_str());
        return {};
    }

    return PluginContainer(PluginData(dataHandle), std::move(metaInfoOpt.value()));
}

std::optional<PluginContainer> getPluginForBuffer(char *buffer, size_t size, PluginBackendApiErrorType &err, PluginBackendPluginParseError &parseErr) {
    auto metaInfoOpt = getMetaInformationForBuffer(buffer, size, err, parseErr);
    if (!metaInfoOpt) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get MetaInformation for buffer %p (%d bytes)", buffer, size);
        return {};
    }

    wups_backend_plugin_data_handle dataHandle;
    if ((err = WUPSBackend_LoadPluginAsDataByBuffer(&dataHandle, buffer, size)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("WUPSLoadPluginAsDataByBuffer failed for buffer %p (%d bytes)", buffer, size);
        return {};
    }

    return PluginContainer(PluginData(dataHandle), std::move(metaInfoOpt.value()));
}

std::vector<PluginContainer> getLoadedPlugins(PluginBackendApiErrorType &err) {
    std::vector<PluginContainer> result;

    PluginBackendApiErrorType err2;
    uint32_t maxSize;
    if ((err2 = WUPSBackend_GetNumberOfLoadedPlugins(&maxSize)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_WARN("Failed to get number of loaded plugins: %s", WUPSBackend_GetStatusStr(err2));
        maxSize = 120;
    }

    auto handles = make_unique_nothrow<wups_backend_plugin_container_handle[]>(maxSize);
    if (!handles) {
        err = PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC;
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return result;
    }
    uint32_t realSize = 0;

    for (uint32_t i = 0; i < maxSize; i++) {
        handles[i] = 0xFFFFFFFF;
    }

    uint32_t plugin_information_version = 0;
    if ((err = WUPSBackend_GetLoadedPlugins(handles.get(), maxSize, &realSize, &plugin_information_version)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("WUPSGetLoadedPlugins: Failed");
        return result;
    }
    if (realSize == 0 || plugin_information_version != WUPS_BACKEND_PLUGIN_INFORMATION_VERSION) {
        err = PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION;
        DEBUG_FUNCTION_LINE_ERR("realSize is 0 or version mismatch");
        return result;
    }

    auto dataHandles = make_unique_nothrow<wups_backend_plugin_data_handle[]>(realSize);
    if (!dataHandles) {
        err = PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC;
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return result;
    }

    if ((err = WUPSBackend_GetPluginDataForContainerHandles(handles.get(), dataHandles.get(), realSize)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get plugin data");
        return result;
    }

    auto information = make_unique_nothrow<wups_backend_plugin_information[]>(realSize);
    if (!information) {
        err = PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC;
        DEBUG_FUNCTION_LINE_ERR("Not enough memory");
        return result;
    }
    if ((err = WUPSBackend_GetMetaInformation(handles.get(), information.get(), realSize)) != PLUGIN_BACKEND_API_ERROR_NONE) {
        DEBUG_FUNCTION_LINE_ERR("Failed to get meta information for handles");
        return result;
    }

    for (uint32_t i = 0; i < realSize; i++) {
        if (information[i].plugin_information_version != WUPS_BACKEND_PLUGIN_INFORMATION_VERSION) {
            err = PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION;
            DEBUG_FUNCTION_LINE_ERR("Skip, wrong struct version.");
            return {};
        }
        result.emplace_back(PluginData(dataHandles[i]), PluginMetaInformation(information[i].name,
                                                                              information[i].author,
                                                                              information[i].version,
                                                                              information[i].license,
                                                                              information[i].buildTimestamp,
                                                                              information[i].description,
                                                                              information[i].storageId,
                                                                              information[i].size));
    }

    return result;
}

PluginBackendApiErrorType LoadAndLinkOnRestart(const std::vector<PluginContainer> &plugins) {
    uint32_t dataSize = plugins.size();
    auto handles      = make_unique_nothrow<wups_backend_plugin_data_handle[]>(dataSize);
    if (!handles) {
        return PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC;
    }
    int i = 0;
    for (const auto &plugin : plugins) {
        const auto &handle = plugin.getPluginData().getHandle();
        if (handle == 0) {
            return PLUGIN_BACKEND_API_INVALID_HANDLE;
        } else {
            handles[i] = handle;
            i++;
        }
    }

    return WUPSBackend_LoadAndLinkByDataHandle(handles.get(), dataSize);
}
} // namespace PluginUtils
} // namespace WUPSBackend
