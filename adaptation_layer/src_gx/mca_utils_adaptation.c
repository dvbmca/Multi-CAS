#include "gxsecure/gxfuse_api.h"

#include "mca_adaptation_files.h"

#define GX_UTL_DBG(fmt, args...)		\
    do{	\
        mca_printf("----gx utils>[%s]Line%d: ", __FUNCTION__, __LINE__);	\
        mca_printf(fmt, ##args);	\
    }while(0)

#define GX_UTL_ERR(fmt, args...)		\
    do{	\
        mca_printf("\033[1;40;31m");	\
        mca_printf("\n################MS UTILS ERROR################\n");	\
        mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__);	\
        mca_printf(fmt, ##args);	\
        mca_printf("\n##############################################\n\n");	\
        mca_printf("\033[0m\n");\
    }while(0)

#define GX_CHIP_ID_LEN  8

static MCA_BOOL g_b8GetChipIDSuccess = MCA_FALSE;
static MCA_U8   g_au8ChipID[GX_CHIP_ID_LEN];

MCA_S32 mca_get_stb_id(MCA_U32* pu32Len, MCA_U8* pu8StbID)
{
    if ((NULL == pu32Len) || (NULL == pu8StbID))
    {
        GX_UTL_ERR("Bad Param: pu32Len = 0x%x, pu8StbID = 0x%x!\n", pu32Len, pu8StbID);
        return MCA_FAILURE;
    }

    if (MCA_TRUE == g_b8GetChipIDSuccess)
    {
        mca_memcpy(pu8StbID, g_au8ChipID, GX_CHIP_ID_LEN);
        *pu32Len = GX_CHIP_ID_LEN;
        return MCA_SUCCESS;
    }

    if (GX_CHIP_ID_LEN != GxFuse_GetCSSN(g_au8ChipID, GX_CHIP_ID_LEN))
    {
        *pu8StbID = 0;
        GX_UTL_ERR("GxFuse_GetCSSN(...) Error!\n");
        return MCA_FAILURE;
    }    

    mca_memcpy(pu8StbID, g_au8ChipID, GX_CHIP_ID_LEN);
    *pu32Len = GX_CHIP_ID_LEN;
    g_b8GetChipIDSuccess = MCA_TRUE;

    return MCA_SUCCESS;
}

MCA_S32 mca_get_current_service(MCA_DMX_ID_t id, MCA_SERVICE_t *pstSv)
{
    GX_UTL_ERR("NOT FIT!\n");
    return MCA_FAILURE;
}

MCA_CHAR*   mca_get_usb_root_path(MCA_VOID)
{
    return "/mnt/usb01/";
}

