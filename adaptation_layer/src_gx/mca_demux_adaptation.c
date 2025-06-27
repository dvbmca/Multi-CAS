#include "module/app_demux_api.h"

#include "mca_adaptation_files.h"

#ifndef E_INVALID_HANDLE
#define E_INVALID_HANDLE        (0)
#endif

#define GX_DMX_DBG(fmt, args...)    \
    do{    \
        mca_printf("[Gx.INFO ][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define GX_DMX_ERR(fmt, args...)    \
    do{    \
        mca_printf("[Gx.ERROR][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define GX_DMX_MODULE_NAME      "gx.demux.adapt"
#define GX_SECTION_BUF_SIZE     4096
#define GX_DMX_CH_NUM_MAX       (MCA_U16)(8)
#define GX_FLT_NUM_MAX          (MCA_U16)(12)

typedef struct 
{
    MCA_BOOL            m_bUsed;

    MCA_DMX_ID_t        m_Demux;
    
    handle_t            m_hGxChannel;             // From Gx API
    handle_t            m_ahGxFlt[GX_FLT_NUM_MAX];// From Gx API

    MCA_DMX_CHAN_ATTR_t m_stChanInfo;
} GX_CHANNEL_t;

static MCA_HANDLE   g_hGxDmxSemaphore;
static GX_CHANNEL_t g_astGxChannel[GX_DMX_CH_NUM_MAX];

static MCA_VOID gx_dmx_sem_wait(MCA_VOID)
{
    if (MCA_INVALID_HANDLE == g_hGxDmxSemaphore)
    {
        GX_DMX_ERR("Bad Parameter: Invalid semaphore handle!\n");
        return ;
    }

    MCA_OS_SemLock(g_hGxDmxSemaphore);
}

static MCA_VOID gx_dmx_sem_post(MCA_VOID)
{
    if (MCA_INVALID_HANDLE == g_hGxDmxSemaphore)
    {
        GX_DMX_ERR("Bad Parameter: Invalid semaphore handle!\n");
        return ;
    }

    MCA_OS_SemUnlock(g_hGxDmxSemaphore);
}

static MCA_VOID *gx_dmx_task(MCA_VOID *pParam)
{
    MCA_U16  slot, j;
    int32_t  s32GxRet;
    uint8_t *pu8Section = NULL;
    uint32_t u32Len;

    handle_t          hFilter;
    MCA_DMX_CB_t      cb;
    MCA_DMX_CB_DATA_t stCbData;

    pu8Section = mca_malloc(GX_SECTION_BUF_SIZE);
    if (NULL == pu8Section)
    {
        GX_DMX_ERR("mca_malloc(%d) Error! \n", GX_SECTION_BUF_SIZE);
        return NULL;
    }

    while (1)
    {
        mca_sleep(10);

        gx_dmx_sem_wait();

        for (slot = 0; slot < GX_DMX_CH_NUM_MAX; slot++)
        {
            cb = g_astGxChannel[slot].m_stChanInfo.m_cb;
        
            if ((MCA_TRUE == g_astGxChannel[slot].m_bUsed) && \
                (E_INVALID_HANDLE != g_astGxChannel[slot].m_hGxChannel) && \
                (cb != NULL))
            {
                for (j = 0; j < GX_FLT_NUM_MAX; j++)
                {
                    hFilter = g_astGxChannel[slot].m_ahGxFlt[j];
                    if (E_INVALID_HANDLE != hFilter)
                    {
                        s32GxRet = GxDemux_QueryAndGetData(hFilter, pu8Section, GX_SECTION_BUF_SIZE, &u32Len);
                        if ((s32GxRet >= 0) && (u32Len > 0))
                        {
                            mca_memset(&stCbData, 0x00, sizeof(stCbData));
                            stCbData.m_DmxId          = g_astGxChannel[slot].m_Demux;
                            stCbData.m_hChannelHandle = slot;
                            stCbData.m_hFilterHandle  = (MCA_HANDLE)(slot<<16 | j);
                            stCbData.m_u32Len         = u32Len;
                            stCbData.m_u8Section      = pu8Section;
                            gx_dmx_sem_post();
                            cb(&stCbData);
                            gx_dmx_sem_wait();
                        }                        
                    }
                }
            }
        }

        gx_dmx_sem_post();
    }

    mca_free(pu8Section);
    pu8Section = NULL;
    return NULL;
}

MCA_S32 mca_dmx_init(MCA_VOID)
{
    MCA_U16     i, j;
    MCA_S32     s32Ret;
    MCA_HANDLE  hTask;

    if (GxDemux_Init() < 0)
    {
        GX_DMX_ERR("GxDemux_Init(...) fail!\n");
        return MCA_FAILURE;
    }

    s32Ret = MCA_OS_TaskCreate(&hTask, GX_DMX_MODULE_NAME, (MCA_TASK_FUNC_T)gx_dmx_task, \
                                        NULL, NULL, 18 * 1024, MCA_TASK_PRIORITY_NORMAL);
    if (MCA_SUCCESS != s32Ret)
    {
        GX_DMX_ERR("MCA_OS_TaskCreate(...) = 0x%x!\n", s32Ret);
    }
    
    s32Ret = MCA_OS_SemCreate(&g_hGxDmxSemaphore, GX_DMX_MODULE_NAME, 1);
    if (MCA_SUCCESS != s32Ret)
    {
        GX_DMX_ERR("MCA_OS_SemCreate(...) = 0x%x!\n", s32Ret);
    }

    mca_memset(g_astGxChannel, 0x00, sizeof(GX_CHANNEL_t) * GX_DMX_CH_NUM_MAX);
    for (i = 0; i < GX_DMX_CH_NUM_MAX; i++)
    {
        g_astGxChannel[i].m_bUsed      = MCA_FALSE;
        g_astGxChannel[i].m_Demux      = MCA_INVALID_DEMUX_ID;
        g_astGxChannel[i].m_hGxChannel = E_INVALID_HANDLE;

        for (j = 0; j < GX_FLT_NUM_MAX; j++)
        {
            g_astGxChannel[i].m_ahGxFlt[j] = E_INVALID_HANDLE;            
        }
        
        g_astGxChannel[i].m_stChanInfo.m_enType     = MCA_DMX_CHAN_TYPE_UNKNOWN;
        g_astGxChannel[i].m_stChanInfo.m_u32BuffSize= 0;
        g_astGxChannel[i].m_stChanInfo.m_bCRC       = MCA_FALSE;
        g_astGxChannel[i].m_stChanInfo.m_pid        = MCA_INVALID_PID;
        g_astGxChannel[i].m_stChanInfo.m_cb         = NULL;        
    }
    
    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_create(MCA_DMX_ID_t id, const MCA_DMX_CHAN_ATTR_t *pstChAttr, MCA_HANDLE *phChannel)
{
    MCA_U16  slot;
    handle_t hGxChannel;

    if ((NULL == pstChAttr) || (NULL == phChannel) || (pstChAttr->m_pid > MCA_INVALID_PID))
    {
        GX_DMX_ERR("Bad Parameter: pstChAttr = 0x%x, phChannel = 0x%x!\n", pstChAttr, phChannel);
        return MCA_FAILURE;
    }

    *phChannel = MCA_INVALID_HANDLE;

    gx_dmx_sem_wait();

    for (slot = 0; slot < GX_DMX_CH_NUM_MAX; slot++)
    {
        if (MCA_FALSE == g_astGxChannel[slot].m_bUsed)
        {
            break;
        }
    }
    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Pool is full!\n");        
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    hGxChannel = GxDemux_ChannelAllocate(id, pstChAttr->m_pid);
    if ((E_INVALID_HANDLE == hGxChannel) || (-1 == hGxChannel))
    { 
        GX_DMX_ERR("GxDemux_ChannelAllocate(%d, 0x%x) = %d\n", id, pstChAttr->m_pid, hGxChannel);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    g_astGxChannel[slot].m_bUsed      = MCA_TRUE;
    g_astGxChannel[slot].m_Demux      = id;
    g_astGxChannel[slot].m_hGxChannel = hGxChannel;
    mca_memcpy(&(g_astGxChannel[slot].m_stChanInfo), pstChAttr, sizeof(MCA_DMX_CHAN_ATTR_t)); 

    gx_dmx_sem_post(); 

    *phChannel = (MCA_HANDLE)slot;

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_destory(MCA_HANDLE  hChannel)
{
    int32_t s32GxRet;
    MCA_U16 slot = (MCA_U16)hChannel;    
    MCA_U16 j;

    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d)\n", slot, GX_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    if (MCA_FALSE == g_astGxChannel[slot].m_bUsed)
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    for (j = 0; j < GX_FLT_NUM_MAX; j++)
    {
        if (E_INVALID_HANDLE != g_astGxChannel[slot].m_ahGxFlt[j])
        {
            s32GxRet = GxDemux_FilterFree(g_astGxChannel[slot].m_ahGxFlt[j]);
            if (s32GxRet < 0)
            {
                GX_DMX_ERR("GxDemux_FilterFree(...) = %d!\n", s32GxRet);
            }
            g_astGxChannel[slot].m_ahGxFlt[j] = E_INVALID_HANDLE;
        }
    }

    if (E_INVALID_HANDLE != g_astGxChannel[slot].m_hGxChannel)
    {
        s32GxRet = GxDemux_ChannelFree(g_astGxChannel[slot].m_hGxChannel);
        if (s32GxRet < 0)
        {
            GX_DMX_ERR("GxDemux_ChannelFree(...) = %d!\n", s32GxRet);
        }        
        g_astGxChannel[slot].m_hGxChannel = E_INVALID_HANDLE;
    }

    g_astGxChannel[slot].m_bUsed      = MCA_FALSE;
    g_astGxChannel[slot].m_Demux      = MCA_INVALID_DEMUX_ID;
    
    g_astGxChannel[slot].m_stChanInfo.m_enType      = MCA_DMX_CHAN_TYPE_UNKNOWN;
    g_astGxChannel[slot].m_stChanInfo.m_u32BuffSize = 0;
    g_astGxChannel[slot].m_stChanInfo.m_bCRC        = MCA_FALSE;
    g_astGxChannel[slot].m_stChanInfo.m_pid         = MCA_INVALID_PID;
    g_astGxChannel[slot].m_stChanInfo.m_cb          = NULL;

    gx_dmx_sem_post();     

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_enable (MCA_HANDLE  hChannel)
{
    int32_t s32GxRet;
    MCA_U16 slot = (MCA_U16)hChannel;
    MCA_U16 j;

    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d)!\n", slot, GX_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    if (MCA_FALSE == g_astGxChannel[slot].m_bUsed)
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }    

    s32GxRet = GxDemux_ChannelEnable(g_astGxChannel[slot].m_hGxChannel);
    if (s32GxRet < 0)
    {
        GX_DMX_ERR("GxDemux_ChannelEnable(%d) = %d!\n", g_astGxChannel[slot].m_hGxChannel, s32GxRet);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }
    for (j = 0; j < GX_FLT_NUM_MAX; j++)
    {
        if (E_INVALID_HANDLE != g_astGxChannel[slot].m_ahGxFlt[j])
        {
            s32GxRet = GxDemux_FilterEnable(g_astGxChannel[slot].m_ahGxFlt[j]);
            if (s32GxRet < 0)
            {
                GX_DMX_ERR("GxDemux_FilterEnable(...) = %d!\n", s32GxRet);
            }
        }
    }

    gx_dmx_sem_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_disable(MCA_HANDLE  hChannel)
{
    int32_t s32GxRet;
    MCA_U16 slot = (MCA_U16)hChannel;
    MCA_U16 j;

    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d)!\n", slot, GX_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    if (MCA_FALSE == g_astGxChannel[slot].m_bUsed)
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    for (j = 0; j < GX_FLT_NUM_MAX; j++)
    {
        if (E_INVALID_HANDLE != g_astGxChannel[slot].m_ahGxFlt[j])
        {
            s32GxRet = GxDemux_FilterDisable(g_astGxChannel[slot].m_ahGxFlt[j]);
            if (s32GxRet < 0)
            {
                GX_DMX_ERR("GxDemux_FilterDisable(...) = %d!\n", s32GxRet);
            }
        }
    }

    s32GxRet = GxDemux_ChannelDisable(g_astGxChannel[slot].m_hGxChannel);
    if (s32GxRet < 0)
    {
        GX_DMX_ERR("GxDemux_ChannelDisable(%d) = %d!\n", g_astGxChannel[slot].m_hGxChannel, s32GxRet);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    gx_dmx_sem_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_set_pid(MCA_HANDLE  hChannel, MCA_U16 pid)
{
    int32_t s32GxRet;
    MCA_U16 slot = (MCA_U16)hChannel;

    if (pid > MCA_INVALID_PID)
    {
        GX_DMX_ERR("Bad Parameter: pid(0x%x) > MCA_INVALID_PID(0x%x)!\n", pid, MCA_INVALID_PID);
        return MCA_FAILURE;
    }

    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d)!\n", slot, GX_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    if (MCA_FALSE == g_astGxChannel[slot].m_bUsed)
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    s32GxRet = GxDemux_ChannelSetPID(g_astGxChannel[slot].m_hGxChannel, pid, TRUE);// ToCheck: TRUE
    if (s32GxRet < 0)
    {
        GX_DMX_ERR("GxDemux_ChannelSetPID(%d, 0x%x, TRUE) = %d!\n", g_astGxChannel[slot].m_hGxChannel, \
                                                                   pid, s32GxRet);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }
    
    g_astGxChannel[slot].m_stChanInfo.m_pid = pid;    

    gx_dmx_sem_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_register_callback(MCA_HANDLE  hChannel, MCA_DMX_CB_t cb)
{
    MCA_U16 slot = (MCA_U16)hChannel;

    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d)!\n", slot, GX_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    if (MCA_FALSE == g_astGxChannel[slot].m_bUsed)
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    g_astGxChannel[slot].m_stChanInfo.m_cb = cb;

    gx_dmx_sem_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_filter_create(MCA_HANDLE hChannel, const MCA_DMX_FILTER_t *pstFltInfo, MCA_HANDLE *phFilter)
{
    MCA_U16  j;
    int32_t  s32GxRet;
    MCA_U16  slot = (MCA_U16)hChannel;
    handle_t hGxFilter;

    if ((NULL == pstFltInfo) || (NULL == phFilter))
    {
        GX_DMX_ERR("Bad Parameter: pstFltInfo = 0x%x, phFilter = 0x%x!\n", pstFltInfo, phFilter);
        return MCA_FAILURE;
    }

    *phFilter = MCA_INVALID_HANDLE;
    if (slot >= GX_DMX_CH_NUM_MAX)
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d)!\n", slot, GX_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    if ((MCA_FALSE == g_astGxChannel[slot].m_bUsed) || \
        (E_INVALID_HANDLE == g_astGxChannel[slot].m_hGxChannel))
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    for (j = 0; j < GX_FLT_NUM_MAX; j++)
    {
        if (E_INVALID_HANDLE == g_astGxChannel[slot].m_ahGxFlt[j])
        {
            break;
        }
    }
    if (j >= GX_FLT_NUM_MAX)
    {
        GX_DMX_ERR("Filter Pool is full!\n");
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    hGxFilter = GxDemux_FilterAllocate(g_astGxChannel[slot].m_hGxChannel);
    if ((E_INVALID_HANDLE == hGxFilter) || (-1 == hGxFilter))
    {
        GX_DMX_ERR("GxDemux_FilterAllocate(%d) = %d!\n", g_astGxChannel[slot].m_hGxChannel, hGxFilter);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    if (0 != pstFltInfo->m_u8Depth)
    {
        s32GxRet = GxDemux_FilterSetup(hGxFilter, \
                                       pstFltInfo->m_au8Match, pstFltInfo->m_au8Mask,\
                                       TRUE, \
                                       (MCA_TRUE==g_astGxChannel[slot].m_stChanInfo.m_bCRC)?TRUE:FALSE, \
                                       FALSE, pstFltInfo->m_u8Depth);
        if (s32GxRet < 0)
        {
            GX_DMX_ERR("GxDemux_FilterSetup(...) = %d!\n", s32GxRet);
        }
    }

    g_astGxChannel[slot].m_ahGxFlt[j] = hGxFilter;

    *phFilter = (MCA_HANDLE)(slot<<16 | j);

    gx_dmx_sem_post();
    
    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_filter_set(MCA_HANDLE hFilter, const MCA_DMX_FILTER_t *pstFltInfo)
{
    int32_t  s32GxRet;
    MCA_U16  slot;
    MCA_U16  j;
    handle_t hGxFilter;
    
    slot = (MCA_U16)(hFilter >> 16);
    j    = (MCA_U16)(hFilter & 0xFFFF);

    if (NULL == pstFltInfo)
    {
        GX_DMX_ERR("Bad Parameter: Filter handle is NULL!\n");
        return MCA_FAILURE;
    }

    if ((slot >= GX_DMX_CH_NUM_MAX) || (j >= GX_FLT_NUM_MAX))
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d), j(%d) >= GX_FLT_NUM_MAX(%d)\n", \
                                                    slot, GX_DMX_CH_NUM_MAX, \
                                                    j, GX_FLT_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    hGxFilter = g_astGxChannel[slot].m_ahGxFlt[j];

    if ((MCA_FALSE == g_astGxChannel[slot].m_bUsed) || \
        (E_INVALID_HANDLE == g_astGxChannel[slot].m_hGxChannel) ||
        (E_INVALID_HANDLE == hGxFilter))
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    s32GxRet = GxDemux_FilterSetup(hGxFilter, \
                                   pstFltInfo->m_au8Match, pstFltInfo->m_au8Mask, \
                                   TRUE, \
                                   (MCA_TRUE==g_astGxChannel[slot].m_stChanInfo.m_bCRC)?TRUE:FALSE, \
                                   FALSE, \
                                   pstFltInfo->m_u8Depth);
    if (s32GxRet < 0)
    {
        GX_DMX_ERR("GxDemux_FilterSetup(...) = %d!\n", s32GxRet);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    gx_dmx_sem_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_filter_destory(MCA_HANDLE hFilter)
{
    int32_t  s32GxRet;
    MCA_U16  slot;
    MCA_U16  j;
    handle_t hGxFilter;
    
    slot = (MCA_U16)(hFilter >> 16);
    j    = (MCA_U16)(hFilter & 0xFFFF);

    if ((slot >= GX_DMX_CH_NUM_MAX) || (j >= GX_FLT_NUM_MAX))
    {
        GX_DMX_ERR("Bad Parameter: slot(%d) >= GX_DMX_CH_NUM_MAX(%d), j(%d) >= GX_FLT_NUM_MAX(%d)\n", \
                                                    slot, GX_DMX_CH_NUM_MAX, \
                                                    j, GX_FLT_NUM_MAX);
        return MCA_FAILURE;
    }

    gx_dmx_sem_wait();

    hGxFilter = g_astGxChannel[slot].m_ahGxFlt[j];

    if ((MCA_FALSE == g_astGxChannel[slot].m_bUsed) || \
        (E_INVALID_HANDLE == g_astGxChannel[slot].m_hGxChannel) ||
        (E_INVALID_HANDLE == hGxFilter))
    {
        GX_DMX_ERR("This demux channel[%d] is not running!\n", slot);
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    s32GxRet = GxDemux_FilterFree(hGxFilter);
    if (s32GxRet < 0)
    {
        GX_DMX_ERR("GxDemux_FilterFree(%d) = %d!\n", hGxFilter, s32GxRet);
        g_astGxChannel[slot].m_ahGxFlt[j] = E_INVALID_HANDLE;
        gx_dmx_sem_post();
        return MCA_FAILURE;
    }

    g_astGxChannel[slot].m_ahGxFlt[j] = E_INVALID_HANDLE;
    
    gx_dmx_sem_post();
    
    return MCA_SUCCESS;
}

