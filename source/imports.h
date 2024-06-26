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

#pragma once

#include "wups_backend/import_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

extern PluginBackendApiErrorType WUPSLoadAndLinkByDataHandle(const wups_backend_plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size);

extern PluginBackendApiErrorType WUPSDeletePluginData(const wups_backend_plugin_data_handle *plugin_data_handle_list, uint32_t plugin_data_handle_list_size);

extern PluginBackendApiErrorType WUPSLoadPluginAsDataByPath(wups_backend_plugin_data_handle *output, const char *path);

extern PluginBackendApiErrorType WUPSLoadPluginAsDataByBuffer(wups_backend_plugin_data_handle *output, char *buffer, size_t size);

extern PluginBackendApiErrorType WUPSGetPluginMetaInformationByPath(wups_backend_plugin_information *output, const char *path);

extern PluginBackendApiErrorType WUPSGetPluginMetaInformationByBuffer(wups_backend_plugin_information *output, char *buffer, size_t size);

extern PluginBackendApiErrorType WUPSGetPluginDataForContainerHandles(const wups_backend_plugin_container_handle *plugin_container_handle_list, const wups_backend_plugin_data_handle *plugin_data_list, uint32_t buffer_size);

extern PluginBackendApiErrorType WUPSGetMetaInformation(const wups_backend_plugin_container_handle *plugin_container_handle_list, wups_backend_plugin_information *plugin_information_list, uint32_t buffer_size);

extern PluginBackendApiErrorType WUPSGetLoadedPlugins(const wups_backend_plugin_container_handle *io_handles, uint32_t buffer_size, uint32_t *outSize, uint32_t *plugin_information_version);

#ifdef __cplusplus
}
#endif