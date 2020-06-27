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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GetPluginInformationInputType {
    PLUGIN_INFORMATION_INPUT_TYPE_PATH = 0,
    PLUGIN_INFORMATION_INPUT_TYPE_BUFFER = 1,
} GetPluginInformationInputType;

typedef uint32_t plugin_container_handle;
typedef uint32_t plugin_data_handle;

/* plugin_information message */
typedef struct __attribute__((__packed__)) plugin_information {
    char name[256];
    char author[256];
    char buildTimestamp[256];
    char description[256];
    char license[256];
    char version[256];
    size_t size;
} plugin_information;

extern int32_t WUPSLoadAndLinkByDataHandle(const plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size);

extern int32_t WUPSDeletePluginContainer(const plugin_container_handle *handle_list, uint32_t handle_list_size);

extern int32_t WUPSDeletePluginData(const plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size);

extern int32_t WUPSLoadPluginAsData(GetPluginInformationInputType inputType, const char *path, char *buffer, size_t size, plugin_data_handle *out);

extern int32_t WUPSLoadPluginAsDataByPath(plugin_data_handle *output, const char *path);

extern int32_t WUPSLoadPluginAsDataByBuffer(plugin_data_handle *output, char *buffer, size_t size);

extern int32_t WUPSGetPluginMetaInformation(GetPluginInformationInputType inputType, const char *path, char *buffer, size_t size, plugin_information *output);

extern int32_t WUPSGetPluginMetaInformationByPath(plugin_information *output, const char *path);

extern int32_t WUPSGetPluginMetaInformationByBuffer(plugin_information *output, char *buffer, size_t size);

extern int32_t WUPSGetPluginDataForContainerHandles(const plugin_container_handle *plugin_container_handle_list, const plugin_data_handle *plugin_data_list, uint32_t buffer_size);

extern int32_t WUPSGetMetaInformation(const plugin_container_handle *plugin_container_handle_list, plugin_information *plugin_information_list, uint32_t buffer_size);

extern int32_t WUPSGetLoadedPlugins(const plugin_container_handle *io_handles, uint32_t buffer_size, uint32_t *outSize);

#ifdef __cplusplus
}
#endif