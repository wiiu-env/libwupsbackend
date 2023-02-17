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

#include <stddef.h>
#include <stdint.h>

typedef enum GetPluginInformationInputType {
    PLUGIN_INFORMATION_INPUT_TYPE_PATH   = 0,
    PLUGIN_INFORMATION_INPUT_TYPE_BUFFER = 1,
} GetPluginInformationInputType;

typedef uint32_t plugin_container_handle;
typedef uint32_t plugin_data_handle;
typedef uint32_t WUPSBackendAPIVersion;

#define WUPS_BACKEND_MODULE_API_VERSION       0x00000002
#define WUPS_BACKEND_MODULE_API_VERSION_ERROR 0xFFFFFFFF

#define PLUGIN_INFORMATION_VERSION            0x00000002

/* plugin_information message */
typedef struct plugin_information {
    uint32_t plugin_information_version;
    char name[256];
    char author[256];
    char buildTimestamp[256];
    char description[256];
    char license[256];
    char version[256];
    char storageId[256];
    size_t size;
} plugin_information;

#define PLUGIN_SECTION_INFORMATION_VERSION 0x00000001

typedef struct plugin_section_info {
    uint32_t plugin_section_info_version;
    char name[32];
    void *address;
    uint32_t size;
} plugin_section_info;

typedef enum PluginBackendApiErrorType {
    PLUGIN_BACKEND_API_ERROR_NONE                  = 0,
    PLUGIN_BACKEND_API_ERROR_INVALID_SIZE          = 0xFFFFFFFF,
    PLUGIN_BACKEND_API_ERROR_INVALID_ARG           = 0xFFFFFFFE,
    PLUGIN_BACKEND_API_ERROR_FAILED_ALLOC          = 0xFFFFFFFD,
    PLUGIN_BACKEND_API_ERROR_FILE_NOT_FOUND        = 0xFFFFFFFC,
    PLUGIN_BACKEND_API_INVALID_HANDLE              = 0xFFFFFFFB,
    PLUGIN_BACKEND_API_ERROR_MODULE_NOT_FOUND      = 0xFFFFFFFA,
    PLUGIN_BACKEND_API_ERROR_MODULE_MISSING_EXPORT = 0xFFFFFFF9,
    PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_VERSION   = 0xFFFFFFF8,
    PLUGIN_BACKEND_API_ERROR_LIB_UNINITIALIZED     = 0xFFFFFFF7,
    PLUGIN_BACKEND_API_ERROR_UNSUPPORTED_COMMAND   = 0xFFFFFFF6,
} PluginBackendApiErrorType;
