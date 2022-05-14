#pragma once
#include <coreinit/debug.h>
#include <cstring>

#define __FILENAME_X__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILENAME__   (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILENAME_X__)

#define DEBUG_FUNCTION_LINE_ERR(FMT, ARGS...)                                                                                               \
    do {                                                                                                                                    \
        OSReport("[(%s)%18s][%23s]%30s@L%04d: ## ERROR ## " FMT "\n", "M", "libwupsbackend", __FILENAME__, __FUNCTION__, __LINE__, ##ARGS); \
    } while (0)
