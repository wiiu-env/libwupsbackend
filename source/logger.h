#pragma once
#include <coreinit/debug.h>
#include <cstring>

#define __FILENAME__ ({                            \
    const char *filename = __FILE__;               \
    const char *pos      = strrchr(filename, '/'); \
    if (!pos) pos = strrchr(filename, '\\');       \
    pos ? pos + 1 : filename;                      \
})

#define DEBUG_FUNCTION_LINE_ERR(FMT, ARGS...)                                                                                               \
    do {                                                                                                                                    \
        OSReport("[(%s)%18s][%23s]%30s@L%04d: ## ERROR ## " FMT "\n", "M", "libwupsbackend", __FILENAME__, __FUNCTION__, __LINE__, ##ARGS); \
    } while (0)
