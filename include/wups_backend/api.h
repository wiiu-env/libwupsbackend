#pragma once
#include "import_defines.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

PluginBackendApiErrorType WUPSBackend_InitLibrary();

PluginBackendApiErrorType WUPSBackend_DeInitLibrary();

const char *WUPSBackend_GetStatusStr(PluginBackendApiErrorType status);

PluginBackendApiErrorType WUPSBackend_GetSectionInformationForPlugin(plugin_container_handle handle, plugin_section_info *plugin_section_list, uint32_t buffer_size, uint32_t *out_count);

PluginBackendApiErrorType WUPSBackend_GetApiVersion(WUPSBackendAPIVersion *outVersion);

PluginBackendApiErrorType WUPSBackend_GetNumberOfLoadedPlugins(uint32_t *out_count);

PluginBackendApiErrorType WUPSBackend_WillReloadPluginsOnNextLaunch(bool *out);

PluginBackendApiErrorType WUPSBackend_GetSectionMemoryAddresses(plugin_container_handle handle, void **textAddress, void **dataAddress);

PluginBackendApiErrorType WUPSBackend_LoadAndLinkByDataHandle(const plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size);

PluginBackendApiErrorType WUPSBackend_DeletePluginData(const plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size);

PluginBackendApiErrorType WUPSBackend_LoadPluginAsDataByPath(plugin_data_handle *output, const char *path);

PluginBackendApiErrorType WUPSBackend_LoadPluginAsDataByBuffer(plugin_data_handle *output, char *buffer, size_t size);

PluginBackendApiErrorType WUPSBackend_WUPSGetPluginMetaInformationByPath(plugin_information *output, const char *path);

PluginBackendApiErrorType WUPSBackend_GetPluginMetaInformationByBuffer(plugin_information *output, char *buffer, size_t size);

PluginBackendApiErrorType WUPSBackend_GetPluginDataForContainerHandles(const plugin_container_handle *plugin_container_handle_list, const plugin_data_handle *plugin_data_list, uint32_t buffer_size);

PluginBackendApiErrorType WUPSBackend_GetMetaInformation(const plugin_container_handle *plugin_container_handle_list, plugin_information *plugin_information_list, uint32_t buffer_size);

PluginBackendApiErrorType WUPSBackend_GetLoadedPlugins(const plugin_container_handle *io_handles, uint32_t buffer_size, uint32_t *outSize, uint32_t *plugin_information_version);

#ifdef __cplusplus
}
#endif