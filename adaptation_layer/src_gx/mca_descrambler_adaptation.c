#include "module/app_descrambler_api.h"

#include "mca_adaptation_files.h"

#ifndef E_INVALID_HANDLE
#define E_INVALID_HANDLE        (0)
#endif

#define GX_DSC_DBG(fmt, args...)    \
    do{    \
        mca_printf("[Gx.INFO ][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define GX_DSC_ERR(fmt, args...)    \
    do{    \
        mca_printf("[Gx.ERROR][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

MCA_S32 mca_descrambler_create(MCA_DMX_ID_t id, MCA_HANDLE *phChannel)
{
    handle_t hGxDes = E_INVALID_HANDLE;

    if (NULL == phChannel)
    {
        GX_DSC_ERR("Bad Parameter: Descrambler handle is NULL!\n");
        return MCA_FAILURE;
    }

    hGxDes = GxDescrmb_Open(id);
    if (E_INVALID_HANDLE == hGxDes)
    {
        GX_DSC_ERR("GxDescrmb_Open(%d) = E_INVALID_HANDLE!\n", id);
        return MCA_FAILURE;
    }

    *phChannel = (MCA_HANDLE)hGxDes;

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_set_pid(MCA_HANDLE hChannel, MCA_U16 pid)
{
    if (GxDescrmb_SetStreamPID((handle_t)hChannel, pid) < 0)
    {
        GX_DSC_ERR("GxDescrmb_SetStreamPID(...) fail!\n");
        return MCA_FAILURE;
    }
    
    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_set_odd_cw (MCA_HANDLE hChannel, MCA_U8 *key, MCA_U8 len)
{
    if (NULL == key)
    {
        GX_DSC_ERR("Bad Parameter: Key is NULL!\n");
        return MCA_FAILURE;
    }

    if (GxDescrmb_SetOddKey((handle_t)hChannel, key, len) < 0)
    {
        GX_DSC_ERR("GxDescrmb_SetOddKey(...) fail!\n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_set_even_cw(MCA_HANDLE hChannel, MCA_U8 *key, MCA_U8 len)
{
    if (NULL == key)
    {
        GX_DSC_ERR("Bad Parameter: Key is NULL!\n");
        return MCA_FAILURE;
    }

    if (GxDescrmb_SetEvenKey((handle_t)hChannel, key, len) < 0)
    {
        GX_DSC_ERR("GxDescrmb_SetEvenKey(...) fail!\n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 mca_descrambler_destroy(MCA_HANDLE hChannel)
{
    handle_t hGxDes;
    
    hGxDes = (handle_t)hChannel;
   
    if (GxDescrmb_Close(hGxDes) < 0)
    {
        GX_DSC_ERR("GxDescrmb_Close() fail!\n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

