#pragma once

#include <stdint.h>

#define WUPS_CONFIG_BUTTON_NONE     0
#define WUPS_CONFIG_BUTTON_LEFT     (1<<0)
#define WUPS_CONFIG_BUTTON_RIGHT    (1<<1)
#define WUPS_CONFIG_BUTTON_UP       (1<<2)
#define WUPS_CONFIG_BUTTON_DOWN     (1<<3)
#define WUPS_CONFIG_BUTTON_A        (1<<4)
#define WUPS_CONFIG_BUTTON_B        (1<<5)
#define WUPS_CONFIG_BUTTON_ZL       (1<<6)
#define WUPS_CONFIG_BUTTON_ZR       (1<<7)
#define WUPS_CONFIG_BUTTON_L        (1<<8)
#define WUPS_CONFIG_BUTTON_R        (1<<9)
typedef int32_t WUPSConfigButtons;

typedef struct {
    int32_t (*getCurrentValueDisplay)(void * context, char *out_buf, int32_t out_size);

    int32_t (*getCurrentValueSelectedDisplay)(void * context, char *out_buf, int32_t out_size);

    void (*onSelected)(void * context, bool isSelected);

    void (*restoreDefault)(void * context);

    bool (*isMovementAllowed)(void * context);

    bool (*callCallback)(void * context);

    void (*onButtonPressed)(void * context, WUPSConfigButtons button);
} WUPSConfigCallbacks_t;

typedef uint32_t WUPSConfigItemHandle;
typedef uint32_t WUPSConfigHandle;
typedef uint32_t WUPSConfigCategoryHandle;