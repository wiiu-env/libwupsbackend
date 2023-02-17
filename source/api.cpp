#include "wups_backend/api.h"
#include "imports.h"
#include "logger.h"
#include "wups_backend/import_defines.h"
#include <coreinit/debug.h>
#include <coreinit/dynload.h>

static OSDynLoad_Module sModuleHandle = nullptr;

static PluginBackendApiErrorType (*sWUPSGetAPIVersion)(WUPSBackendAPIVersion *) = nullptr;
static PluginBackendApiErrorType (*sWUPSGetSectionInformationForPlugin)(
        plugin_container_handle handle,
        plugin_section_info *plugin_section_list,
        uint32_t buffer_size,
        uint32_t *out_count) = nullptr;

static PluginBackendApiErrorType (*sWUPSGetNumberOfLoadedPlugins)(uint32_t *out)       = nullptr;
static PluginBackendApiErrorType (*sWUPSWillReloadPluginsOnNextLaunch)(bool *out)      = nullptr;
static PluginBackendApiErrorType (*sWUPSGetSectionMemoryAddresses)(plugin_container_handle handle,
                                                                   void **textAddress,
                                                                   void **dataAddress) = nullptr;

static bool sLibInitDone = false;

static WUPSBackendAPIVersion sWUPSAPIVersion = WUPS_BACKEND_MODULE_API_VERSION_ERROR;

const char *WUPSBackend_GetStatusStr(PluginBackendApiErrorType status) {
    switch (status) {
        case PLUGIN_BACKEND_API_ERROR_NONE:
            return "PLUGIN_BACKEND_API_ERROR_NONE";
        case PLUGIN_BACKEND_API_ERROR_INVALID_SIZE:
            return "PLUGIN_BACKEND_API_ERROR_INVALID_SIZE";
        case PLUGIN_BACKEND_API_ERROR_INVALID_ARG:
            return "PLUGIN_BACKEND_API_ERROR_INVALID_ARG";
        case PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC:
            return "PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC";
        case PLUGIN_BACKEND_API_ERROR_FILE_NOT_FOUND:
            return "PLUGIN_BACKEND_API_ERROR_FILE_NOT_FOUND";
        case PLUGIN_BACKEND_API_INVALID_HANDLE:
            return "PLUGIN_BACKEND_API_INVALID_HANDLE";
        case PLUGIN_BACKEND_API_ERROR_MODULE_NOT_FOUND:
            return "PLUGIN_BACKEND_API_ERROR_MODULE_NOT_FOUND";
        case PLUGIN_BACKEND_API_ERROR_MODULE_MISSING_EXPORT:
            return "PLUGIN_BACKEND_API_ERROR_MODULE_MISSING_EXPORT";
        case PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION:
            return "PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION";
        case PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED:
            return "PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED";
        case PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND:
            return "PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND";
    }
    return "PLUGIN_BACKEND_API_ERROR_UNKNOWN_ERROR";
}

PluginBackendApiErrorType WUPSBackend_InitLibrary() {
    if (sLibInitDone) {
        return PLUGIN_BACKEND_API_ERROR_NONE;
    }
    if (OSDynLoad_Acquire("homebrew_wupsbackend", &sModuleHandle) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("OSDynLoad_Acquire failed.");
        return PLUGIN_BACKEND_API_ERROR_MODULE_NOT_FOUND;
    }

    if (OSDynLoad_FindExport(sModuleHandle, FALSE, "WUPSGetAPIVersion", (void **) &sWUPSGetAPIVersion) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_ERR("FindExport WUPSGetAPIVersion failed.");
        return PLUGIN_BACKEND_API_ERROR_MODULE_MISSING_EXPORT;
    }

    if (OSDynLoad_FindExport(sModuleHandle, FALSE, "WUPSWillReloadPluginsOnNextLaunch", (void **) &sWUPSWillReloadPluginsOnNextLaunch) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_WARN("FindExport WUPSWillReloadPluginsOnNextLaunch failed.");
        sWUPSWillReloadPluginsOnNextLaunch = nullptr;
    }

    if (OSDynLoad_FindExport(sModuleHandle, FALSE, "WUPSGetNumberOfLoadedPlugins", (void **) &sWUPSGetNumberOfLoadedPlugins) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_WARN("FindExport WUPSGetNumberOfLoadedPlugins failed.");
        sWUPSGetNumberOfLoadedPlugins = nullptr;
    }

    if (OSDynLoad_FindExport(sModuleHandle, FALSE, "WUPSGetSectionMemoryAddresses", (void **) &sWUPSGetSectionMemoryAddresses) != OS_DYNLOAD_OK) {
        DEBUG_FUNCTION_LINE_WARN("FindExport WUPSGetSectionMemoryAddresses failed.");
        sWUPSGetSectionMemoryAddresses = nullptr;
    }

    auto res = WUPSBackend_GetApiVersion(&sWUPSAPIVersion);
    if (res != PLUGIN_BACKEND_API_ERROR_NONE) {
        sWUPSAPIVersion = WUPS_BACKEND_MODULE_API_VERSION_ERROR;
        return PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION;
    }

    sLibInitDone = true;
    return PLUGIN_BACKEND_API_ERROR_NONE;
}

PluginBackendApiErrorType WUPSBackend_DeInitLibrary() {
    if (sLibInitDone) {
        sWUPSGetAPIVersion = nullptr;
        sWUPSAPIVersion    = WUPS_BACKEND_MODULE_API_VERSION_ERROR;
        OSDynLoad_Release(sModuleHandle);
        sModuleHandle = nullptr;
        sLibInitDone  = false;
    }
    return PLUGIN_BACKEND_API_ERROR_NONE;
}

PluginBackendApiErrorType WUPSBackend_GetApiVersion(WUPSBackendAPIVersion *outVersion) {
    if (sWUPSGetAPIVersion == nullptr) {
        if (OSDynLoad_Acquire("homebrew_wupsbackend", &sModuleHandle) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("OSDynLoad_Acquire failed.");
            return PLUGIN_BACKEND_API_ERROR_MODULE_NOT_FOUND;
        }

        if (OSDynLoad_FindExport(sModuleHandle, FALSE, "WUPSGetAPIVersion", (void **) &sWUPSGetAPIVersion) != OS_DYNLOAD_OK) {
            DEBUG_FUNCTION_LINE_WARN("FindExport WUPSGetAPIVersion failed.");
            return PLUGIN_BACKEND_API_ERROR_MODULE_MISSING_EXPORT;
        }
    }

    return reinterpret_cast<decltype(&WUPSBackend_GetApiVersion)>(sWUPSGetAPIVersion)(outVersion);
}

PluginBackendApiErrorType WUPSBackend_GetSectionInformationForPlugin(plugin_container_handle handle, plugin_section_info *plugin_section_list, uint32_t buffer_size, uint32_t *out_count) {
    if (sWUPSAPIVersion == WUPS_BACKEND_MODULE_API_VERSION_ERROR) {
        return PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED;
    }
    if (sWUPSGetSectionInformationForPlugin == nullptr || sWUPSAPIVersion < 2) {
        return PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND;
    }

    if (handle == 0) {
        return PLUGIN_BACKEND_API_ERROR_INVALID_ARG;
    }

    return reinterpret_cast<decltype(&WUPSBackend_GetSectionInformationForPlugin)>(sWUPSGetSectionInformationForPlugin)(handle, plugin_section_list, buffer_size, out_count);
}

PluginBackendApiErrorType WUPSBackend_GetNumberOfLoadedPlugins(uint32_t *out_count) {
    if (sWUPSAPIVersion == WUPS_BACKEND_MODULE_API_VERSION_ERROR) {
        return PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED;
    }
    if (sWUPSGetNumberOfLoadedPlugins == nullptr || sWUPSAPIVersion < 2) {
        return PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND;
    }

    if (out_count == nullptr) {
        return PLUGIN_BACKEND_API_ERROR_INVALID_ARG;
    }

    return reinterpret_cast<decltype(&WUPSBackend_GetNumberOfLoadedPlugins)>(sWUPSGetNumberOfLoadedPlugins)(out_count);
}

PluginBackendApiErrorType WUPSBackend_WillReloadPluginsOnNextLaunch(bool *out) {
    if (sWUPSAPIVersion == WUPS_BACKEND_MODULE_API_VERSION_ERROR) {
        return PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED;
    }
    if (sWUPSWillReloadPluginsOnNextLaunch == nullptr || sWUPSAPIVersion < 2) {
        return PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND;
    }

    if (out == nullptr) {
        return PLUGIN_BACKEND_API_ERROR_INVALID_ARG;
    }

    return reinterpret_cast<decltype(&WUPSBackend_WillReloadPluginsOnNextLaunch)>(sWUPSWillReloadPluginsOnNextLaunch)(out);
}

PluginBackendApiErrorType WUPSBackend_GetSectionMemoryAddresses(plugin_container_handle handle, void **textAddress, void **dataAddress) {
    if (sWUPSAPIVersion == WUPS_BACKEND_MODULE_API_VERSION_ERROR) {
        return PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED;
    }
    if (sWUPSGetSectionMemoryAddresses == nullptr || sWUPSAPIVersion < 2) {
        return PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND;
    }

    if (handle == 0 || textAddress == nullptr || dataAddress == nullptr) {
        return PLUGIN_BACKEND_API_ERROR_INVALID_ARG;
    }

    return reinterpret_cast<decltype(&WUPSBackend_GetSectionMemoryAddresses)>(sWUPSGetSectionMemoryAddresses)(handle, textAddress, dataAddress);
}

PluginBackendApiErrorType WUPSBackend_LoadAndLinkByDataHandle(const plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size) {
    return WUPSLoadAndLinkByDataHandle(plugin_data_handle_list, plugin_data_handle_list_size);
}

PluginBackendApiErrorType WUPSBackend_DeletePluginData(const plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size) {
    return WUPSDeletePluginData(plugin_data_handle_list, plugin_data_handle_list_size);
}

PluginBackendApiErrorType WUPSBackend_LoadPluginAsData(GetPluginInformationInputType inputType, const char *path, char *buffer, size_t size, plugin_data_handle *out) {
    return WUPSLoadPluginAsData(inputType, path, buffer, size, out);
}

PluginBackendApiErrorType WUPSBackend_LoadPluginAsDataByPath(plugin_data_handle *output, const char *path) {
    return WUPSLoadPluginAsDataByPath(output, path);
}

PluginBackendApiErrorType WUPSBackend_LoadPluginAsDataByBuffer(plugin_data_handle *output, char *buffer, size_t size) {
    return WUPSLoadPluginAsDataByBuffer(output, buffer, size);
}

PluginBackendApiErrorType WUPSBackend_GetPluginMetaInformation(GetPluginInformationInputType inputType, const char *path, char *buffer, size_t size, plugin_information *output) {
    return WUPSGetPluginMetaInformation(inputType, path, buffer, size, output);
}

PluginBackendApiErrorType WUPSBackend_WUPSGetPluginMetaInformationByPath(plugin_information *output, const char *path) {
    return WUPSGetPluginMetaInformationByPath(output, path);
}

PluginBackendApiErrorType WUPSBackend_GetPluginMetaInformationByBuffer(plugin_information *output, char *buffer, size_t size) {
    return WUPSGetPluginMetaInformationByBuffer(output, buffer, size);
}

PluginBackendApiErrorType WUPSBackend_GetPluginDataForContainerHandles(const plugin_container_handle *plugin_container_handle_list, const plugin_data_handle *plugin_data_list, uint32_t buffer_size) {
    return WUPSGetPluginDataForContainerHandles(plugin_container_handle_list, plugin_data_list, buffer_size);
}

PluginBackendApiErrorType WUPSBackend_GetMetaInformation(const plugin_container_handle *plugin_container_handle_list, plugin_information *plugin_information_list, uint32_t buffer_size) {
    return WUPSGetMetaInformation(plugin_container_handle_list, plugin_information_list, buffer_size);
}

PluginBackendApiErrorType WUPSBackend_GetLoadedPlugins(const plugin_container_handle *io_handles, uint32_t buffer_size, uint32_t *outSize, uint32_t *plugin_information_version) {
    return WUPSGetLoadedPlugins(io_handles, buffer_size, outSize, plugin_information_version);
}
