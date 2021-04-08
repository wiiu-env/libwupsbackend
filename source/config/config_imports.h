#include <cstdint>
#include <wups_config/config.h>

extern int32_t WUPSConfigItem_Create(WUPSConfigItemHandle *out, const char *configID, const char *displayName, WUPSConfigCallbacks_t callbacks, void* context);

extern int32_t WUPSConfigItem_SetDisplayName(WUPSConfigItemHandle handle, const char *displayName);

extern int32_t WUPSConfigItem_GetDisplayName(WUPSConfigItemHandle handle, char *out_buf, int32_t out_len);

extern int32_t WUPSConfigItem_SetConfigID(WUPSConfigItemHandle handle, const char *configID);

extern int32_t WUPSConfigItem_GetConfigID(WUPSConfigItemHandle handle, char *out_buf, int32_t out);


extern int32_t WUPSConfig_Create(WUPSConfigHandle *out, const char *name);

extern int32_t WUPSConfig_GetName(WUPSConfigHandle handle, char *out_buf, int32_t out_len);

extern int32_t WUPSConfig_AddCategoryByName(WUPSConfigHandle handle, const char *categoryName, WUPSConfigCategoryHandle *out);

extern int32_t WUPSConfig_AddCategory(WUPSConfigHandle handle, WUPSConfigCategoryHandle category);

/*
extern int32_t WUPSConfig_GetCategoryCount(WUPSConfigHandle handle, int32_t *category_count);

extern int32_t WUPSConfig_GetCategories(WUPSConfigHandle handle, WUPSConfigCategoryHandle *categories_out, int32_t categories_out_size);
 */


extern int32_t WUPSConfigCategory_Create(WUPSConfigCategoryHandle *out, const char *name);

extern int32_t WUPSConfigCategory_GetName(WUPSConfigCategoryHandle handle, char *out_buf, int32_t out_len);

extern int32_t WUPSConfigCategory_AddItem(WUPSConfigCategoryHandle handle, WUPSConfigItemHandle item_Handle);
