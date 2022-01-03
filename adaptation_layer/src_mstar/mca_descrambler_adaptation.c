#include "MsCommon.h"
#include "MsOS.h"
#include "drvDSCMB.h"

#include "mca_adaptation_files.h"

#define MS_DSC_DBG(fmt, args...)    \
    do{    \
        mca_printf("----ms dsc>[%s]Line%d: ", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define MS_DSC_ERR(fmt, args...)    \
    do{    \
        mca_printf("\033[1;40;31m");    \
        mca_printf("\n################MS DSC ERROR################\n");    \
        mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
        mca_printf("\n############################################\n\n");    \
        mca_printf("\033[0m\n");\
    }while(0)

MCA_S32 mca_descrambler_create(MCA_DMX_ID_t id, MCA_HANDLE *phChannel)
{
    MS_U32 u32MsDsc;

    if (NULL == phChannel)
    {
        MS_DSC_ERR("Bad Param: phChannel = NULL \n");
        return MCA_FAILURE;
    }

    u32MsDsc = MDrv_DSCMB_FltAlloc();
    if (DRV_DSCMB_FLT_NULL == u32MsDsc)
    {
        MS_DSC_ERR("MDrv_DSCMB_FltAlloc() = 0x%x Failed.", u32MsDsc);
        *phChannel = MCA_INVALID_HANDLE;
        return MCA_FAILURE;
    }

    *phChannel = (MCA_HANDLE)u32MsDsc;

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_set_pid(MCA_HANDLE hChannel, MCA_U16 pid)
{
    if (FALSE == MDrv_DSCMB_FltConnectPid((MS_U32)hChannel, pid))
    {
        MS_DSC_ERR("MDrv_DSCMB_FltConnectPid() = FALSE");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_set_odd_cw (MCA_HANDLE hChannel, MCA_U8 *key, MCA_U8 len)
{
    if (NULL == key)
    {
        MS_DSC_ERR("Bad Param: key = NULL \n");
        return MCA_FAILURE;
    }
    key[3] = (key[0] + key[1] + key[2]) % 256;
    key[7] = (key[4] + key[5] + key[6]) % 256;

    MDrv_DSCMB_FltTypeSet((MS_U32)hChannel, E_DSCMB_TYPE_CSA);
    MDrv_DSCMB_FltKeySet((MS_U32)hChannel, E_DSCMB_KEY_ODD, (MS_U8 *)key);

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_set_even_cw(MCA_HANDLE hChannel, MCA_U8 *key, MCA_U8 len)
{
    if (NULL == key)
    {
        MS_DSC_ERR("Bad Param: key = NULL \n");
        return MCA_FAILURE;
    }
    key[3] = (key[0] + key[1] + key[2]) % 256;
    key[7] = (key[4] + key[5] + key[6]) % 256;

    MDrv_DSCMB_FltTypeSet((MS_U32)hChannel, E_DSCMB_TYPE_CSA);
    MDrv_DSCMB_FltKeySet((MS_U32)hChannel, E_DSCMB_KEY_EVEN, (MS_U8 *)key);

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_destroy(MCA_HANDLE hChannel)
{
    MS_U32 u32MsDsc;
    
    u32MsDsc = (MS_U32)hChannel;
   
    if (FALSE == MDrv_DSCMB_FltFree(u32MsDsc))
    {
        MS_DSC_ERR("MDrv_DSCMB_FltFree() = FALSE");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

