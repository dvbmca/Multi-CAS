#include "MsCommon.h"
#include "MsOS.h"
#include "apiDMX.h"

#include "mca_adaptation_files.h"

#define MS_DMX_DBG(fmt, args...)    \
    do{    \
        mca_printf("[Mstar.INFO ][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define MS_DMX_ERR(fmt, args...)    \
    do{    \
        mca_printf("[Mstar.ERROR][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define MS_DMX_MODULE_NAME          "mca.demux.adapt"

#define MS_DMX_NEW_CALLBACK_DATA    0x98788800
#define MS_SECTION_BUF_SIZE         4096

#define MS_DMX_CH_NUM_MAX   8
#define MS_FLT_NUM_MAX      16
#define MS_INVALID_FLT_ID   0xFFFF

typedef struct 
{
    MCA_U8  m_au8Mask [MCA_DMX_FILTER_MAX_DEPTH];
    MCA_U8  m_au8Match[MCA_DMX_FILTER_MAX_DEPTH];
    MCA_U8  m_u8Depth;

    MCA_U8  m_u8Res;
    MCA_U16 m_u16HwFltID;
    MCA_U8 *m_pu8SecBuff;
} MS_FILTER_t;

typedef struct 
{
    MCA_BOOL            m_bUsed;

    MCA_DMX_CHAN_ATTR_t m_stChanInfo;

    MS_FILTER_t         m_astFltInfo[MS_FLT_NUM_MAX];

} MS_CHANNEL_t;

static MCA_HANDLE   g_hMstarDmxQueue;
static MCA_HANDLE   g_hMstarDmxSemaphore;
static MS_CHANNEL_t g_astMstarChannel[MS_DMX_CH_NUM_MAX];

static void mstar_dmx_filter_notify(MS_U8 u8FltId, DMX_EVENT enEvent);

static MCA_VOID mstar_dmx_wait(MCA_VOID)
{
    if (MCA_INVALID_HANDLE == g_hMstarDmxSemaphore)
    {
        MS_DMX_ERR("Bad Parameter: Invalid semaphore handle!\n");
        return ;
    }

    MCA_OS_SemLock(g_hMstarDmxSemaphore);
}

static MCA_VOID mstar_dmx_post(MCA_VOID)
{
    if (MCA_INVALID_HANDLE == g_hMstarDmxSemaphore)
    {
        MS_DMX_ERR("Bad Parameter: Invalid semaphore handle!\n");
        return ;
    }

    MCA_OS_SemUnlock(g_hMstarDmxSemaphore);
}

static MCA_VOID mstar_reset_channel_info(MCA_DMX_CHAN_ATTR_t *pstChan)
{
    if (NULL == pstChan)
    {
        MS_DMX_ERR("Bad Parameter: Pointer is NULL!\n");
        return;
    }
    
    pstChan->m_enType       = MCA_DMX_CHAN_TYPE_UNKNOWN;
    pstChan->m_u32BuffSize  = 0;
    pstChan->m_bCRC         = MCA_FALSE;
    pstChan->m_pid          = MCA_INVALID_PID;
    pstChan->m_cb           = NULL;
}

static MCA_VOID mstar_reset_filter_info(MS_FILTER_t *pstFlt)
{
    if (NULL == pstFlt)
    {
        MS_DMX_ERR("Bad Parameter: Pointer is NULL!\n");
        return;
    }

    mca_memset(pstFlt->m_au8Mask,  0x00, MCA_DMX_FILTER_MAX_DEPTH);
    mca_memset(pstFlt->m_au8Match, 0x00, MCA_DMX_FILTER_MAX_DEPTH);
    pstFlt->m_u8Depth = 0;
    if (pstFlt->m_u16HwFltID != MS_INVALID_FLT_ID)
    {
        MApi_DMX_Stop (pstFlt->m_u16HwFltID);
        MApi_DMX_Close(pstFlt->m_u16HwFltID);
        pstFlt->m_u16HwFltID = MS_INVALID_FLT_ID;
    }

    if (pstFlt->m_pu8SecBuff)
    {
        mca_free(pstFlt->m_pu8SecBuff);
        pstFlt->m_pu8SecBuff = NULL;
    }
}

static MCA_VOID *mca_dmx_task(MCA_VOID *pParam)
{
    MCA_U16             i, j;
    MCA_S32             s32Ret;
    MCA_U32             au32Msg[4];
    MCA_U32             u32MsgLen;
    MCA_U8              u8FltID;
    MCA_DMX_CB_t        cb;
    MCA_DMX_CB_DATA_t   stCbData;
    MCA_U8             *pu8Section;
    MS_U32              u32ActualSize;
    MS_U32              u32RmnSize;

    pu8Section = mca_malloc(MS_SECTION_BUF_SIZE);
    if (NULL == pu8Section)
    {
        MS_DMX_ERR("mca_malloc(0x%x) Failed!\n", MS_SECTION_BUF_SIZE);
        return NULL;
    }       

    u32MsgLen = sizeof(au32Msg);
    while (1)
    {
        mca_sleep(5);
        mca_memset(au32Msg, 0x00, u32MsgLen);        
        s32Ret = MCA_OS_QueueReceive(g_hMstarDmxQueue, au32Msg, u32MsgLen);
        if ((MCA_SUCCESS == s32Ret) && (MS_DMX_NEW_CALLBACK_DATA == au32Msg[0]))
        {        
            cb      = NULL;
            u8FltID = (MCA_U8)au32Msg[1];

            mstar_dmx_wait();

            for (i = 0; i < MS_DMX_CH_NUM_MAX; i++)
            {
                for (j = 0; (MCA_TRUE == g_astMstarChannel[i].m_bUsed)&&(j < MS_FLT_NUM_MAX); j++)
                {
                    if (u8FltID == g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID)
                    {
                        stCbData.m_DmxId            = 0;
                        stCbData.m_hChannelHandle   = i;
                        stCbData.m_hFilterHandle    = (i<<16) | j;
                        cb = g_astMstarChannel[i].m_stChanInfo.m_cb;
                        break;
                    }
                }                
            }

            mstar_dmx_post();
            
            if (cb)
            {
                if (DMX_FILTER_STATUS_OK != MApi_DMX_CopyData(u8FltID, pu8Section, MS_SECTION_BUF_SIZE, \
                                                                    &u32ActualSize, &u32RmnSize, NULL))
                {
                    MS_DMX_ERR("MApi_DMX_CopyData Error!\n");
                    continue;
                }

                stCbData.m_u32Len    = u32ActualSize;
                stCbData.m_u8Section = pu8Section;
                cb(&stCbData);
            }
        }
    }

    mca_free(pu8Section);
    pu8Section = NULL;

    return NULL;
}

MCA_S32 mca_dmx_init(MCA_VOID)
{
    MCA_U32     i, j;
    MCA_S32     s32Ret;
    MCA_HANDLE  hTask, hQue;

    s32Ret = MCA_OS_TaskCreate(&hTask, MS_DMX_MODULE_NAME, (MCA_TASK_FUNC_T)mca_dmx_task, NULL, NULL, \
                                                                18 * 1024, MCA_TASK_PRIORITY_NORMAL);
    if (MCA_SUCCESS != s32Ret)
    {
        MS_DMX_ERR("MCA_OS_TaskCreate(...) = 0x%x!\n", s32Ret);
    }
    s32Ret = MCA_OS_QueueCreate(&hQue, MS_DMX_MODULE_NAME, 100);
    if (MCA_SUCCESS != s32Ret)
    {
        MS_DMX_ERR("MCA_OS_QueueCreate(...) = 0x%x!\n", s32Ret);
    }
    g_hMstarDmxQueue = hQue;
    
    s32Ret = MCA_OS_SemCreate(&g_hMstarDmxSemaphore, MS_DMX_MODULE_NAME, 1);
    if (MCA_SUCCESS != s32Ret)
    {
        MS_DMX_ERR("MCA_OS_SemCreate(...) = 0x%x!\n", s32Ret);
    }

    mca_memset(g_astMstarChannel, 0x00, sizeof(MS_CHANNEL_t) * MS_DMX_CH_NUM_MAX);
    for (i = 0; i < MS_DMX_CH_NUM_MAX; i++)
    {
        g_astMstarChannel[i].m_bUsed = MCA_FALSE;
        mstar_reset_channel_info(&g_astMstarChannel[i].m_stChanInfo);

        for (j = 0; j < MS_FLT_NUM_MAX; j++)
        {
            mca_memset(g_astMstarChannel[i].m_astFltInfo[j].m_au8Mask,  0x00, MCA_DMX_FILTER_MAX_DEPTH);
            mca_memset(g_astMstarChannel[i].m_astFltInfo[j].m_au8Match, 0x00, MCA_DMX_FILTER_MAX_DEPTH);
            g_astMstarChannel[i].m_astFltInfo[j].m_u8Depth      = 0;
            g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID   = MS_INVALID_FLT_ID;
            g_astMstarChannel[i].m_astFltInfo[j].m_pu8SecBuff   = NULL;
        }
    }
    
    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_create(MCA_DMX_ID_t id, const MCA_DMX_CHAN_ATTR_t *pstChAttr, MCA_HANDLE *phChannel)
{
    MCA_U16 i;

    if ((NULL == pstChAttr) || (NULL == phChannel))
    {
        MS_DMX_ERR("Bad Parameter: pstChAttr = 0x%x, phChannel = 0x%x!\n", pstChAttr, phChannel);
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    for (i = 0; i < MS_DMX_CH_NUM_MAX; i++)
    {
        if (MCA_FALSE == g_astMstarChannel[i].m_bUsed)
        {
            mca_memcpy(&(g_astMstarChannel[i].m_stChanInfo), pstChAttr, sizeof(MCA_DMX_CHAN_ATTR_t));
            g_astMstarChannel[i].m_bUsed = MCA_TRUE;
            break;
        }
    }

    mstar_dmx_post();

    if (i >= MS_DMX_CH_NUM_MAX)
    {
        *phChannel = MCA_INVALID_HANDLE;
        return MCA_FAILURE;
    }
    *phChannel = i;
        
    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_destory(MCA_HANDLE  hChannel)
{
    MCA_U16 i, j;

    i = (MCA_U16)hChannel;

    if (i >= MS_DMX_CH_NUM_MAX)
    {
        MS_DMX_ERR("Bad Parameter: i >= MS_DMX_CH_NUM_MAX!\n");
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    if (MCA_FALSE == g_astMstarChannel[i].m_bUsed)
    {
        MS_DMX_ERR("This demux channel is already descotried!\n");
    }
    g_astMstarChannel[i].m_bUsed = MCA_FALSE;
    mstar_reset_channel_info(&g_astMstarChannel[i].m_stChanInfo);
    for (j = 0; j < MS_FLT_NUM_MAX; j++)
    {
        mstar_reset_filter_info(&(g_astMstarChannel[i].m_astFltInfo[j]));
    }

    mstar_dmx_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_enable (MCA_HANDLE  hChannel)
{
    MCA_U16 i, j;

    i = (MCA_U16)hChannel;
    if (i >= MS_DMX_CH_NUM_MAX)
    {
        MS_DMX_ERR("Bad Parameter: i >= MS_DMX_CH_NUM_MAX!\n");
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    if (MCA_FALSE == g_astMstarChannel[i].m_bUsed)
    {
        MS_DMX_ERR("Channel is not used!\n");
        mstar_dmx_post();
        return MCA_FAILURE;
    }
    for (j = 0; j < MS_FLT_NUM_MAX; j++)
    {
        if (MS_INVALID_FLT_ID != g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID)
        {        
            MApi_DMX_Start(g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID);
        }
    }

    mstar_dmx_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_disable(MCA_HANDLE  hChannel)
{
    MCA_U16 i, j;

    i = (MCA_U16)hChannel;
    if (i >= MS_DMX_CH_NUM_MAX)
    {
        MS_DMX_ERR("Bad Parameter: i >= MS_DMX_CH_NUM_MAX!\n");
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    if (MCA_FALSE == g_astMstarChannel[i].m_bUsed)
    {
        MS_DMX_ERR("Channel is not used!\n");
        mstar_dmx_post();
        return MCA_FAILURE;
    }
    for (j = 0; j < MS_FLT_NUM_MAX; j++)
    {
        if (g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID != MS_INVALID_FLT_ID)
        {
            MApi_DMX_Stop(g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID);
        }
    }

    mstar_dmx_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_set_pid(MCA_HANDLE  hChannel, MCA_U16 pid)
{
    MCA_U16 i, j;

    i = (MCA_U16)hChannel;
    if ((i >= MS_DMX_CH_NUM_MAX) || (pid > MCA_INVALID_PID))
    {
        MS_DMX_ERR("Bad Parameter: i(%d) >= MS_DMX_CH_NUM_MAX(%d), pid(0x%x) > MCA_INVALID_PID(0x%x)!\n",   \
                                                            i, MS_DMX_CH_NUM_MAX, pid, MCA_INVALID_PID);
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    if (MCA_FALSE == g_astMstarChannel[i].m_bUsed)
    {
        MS_DMX_ERR("Channel is not used!\n");
        mstar_dmx_post();
        return MCA_FAILURE;
    }

    g_astMstarChannel[i].m_stChanInfo.m_pid = pid;

    for (j = 0; j < MS_FLT_NUM_MAX; j++)
    {
        if (MS_INVALID_FLT_ID != g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID)
        {       
            if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID, &pid, TRUE))
            {
                MS_DMX_ERR("MApi_DMX_Pid(0x%x, 0x%x, TRUE) Faile!\n", g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID, pid);
            }
        }
    }

    mstar_dmx_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_channel_register_callback(MCA_HANDLE  hChannel, MCA_DMX_CB_t cb)
{
    MCA_U16 i;

    i = (MCA_U16)hChannel;
    if (i >= MS_DMX_CH_NUM_MAX)
    {
        MS_DMX_ERR("Bad Parameter: i >= MS_DMX_CH_NUM_MAX!\n");
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    if (MCA_FALSE == g_astMstarChannel[i].m_bUsed)
    {
        MS_DMX_ERR("Channel is not used!\n");
        mstar_dmx_post();
        return MCA_FAILURE;
    }

    g_astMstarChannel[i].m_stChanInfo.m_cb = cb;

    mstar_dmx_post();

    return MCA_SUCCESS;
}

static void mstar_dmx_filter_notify(MS_U8 u8FltId, DMX_EVENT enEvent)
{
    if (enEvent == DMX_EVENT_DATA_READY)    
    {
        MCA_U32  au32Msg[4];
    
        au32Msg[0] = MS_DMX_NEW_CALLBACK_DATA;
        au32Msg[1] = (MCA_U32)u8FltId;
        au32Msg[2] = au32Msg[3] = 0;

        MCA_OS_QueueSend(g_hMstarDmxQueue, au32Msg, sizeof(au32Msg));
    }
    else if(enEvent == DMX_EVENT_BUF_OVERFLOW)
    {
        MS_DMX_DBG("u8FltId = 0x%x, DMX_EVENT_DATA_READY!\n", u8FltId);
    
        MApi_DMX_Stop (u8FltId);
        MApi_DMX_Start(u8FltId);
    }
    else
    {
        MS_DMX_ERR("Unknown event [%d]!\n", enEvent);
    }
}

MCA_S32 mca_dmx_filter_create(MCA_HANDLE hChannel, const MCA_DMX_FILTER_t *pstFltInfo, MCA_HANDLE *phFilter)
{
    MCA_U16         i, j;
    MS_U8           u8NewFlt = 0xFF;
    MS_U32          u32BuffSize;
    MS_U8*          pu8Buffer= NULL;
    DMX_Flt_info    stMsFlt;
    DMX_FILTER_TYPE u32MsFltType;
    MS_U8           au8Match[DMX_SECT_FLT_DEPTH];
    MS_U8           au8Mask [DMX_SECT_FLT_DEPTH];
    MS_U8           au8NMask[DMX_SECT_FLT_DEPTH];

    i = (MCA_U32)hChannel;

    if ((NULL == pstFltInfo) || (NULL == phFilter) || (i >= MS_DMX_CH_NUM_MAX))
    {
        MS_DMX_ERR("Bad Parameter: pstFltInfo = 0x%x, phFilter = 0x%x, i(%d) >= MS_DMX_CH_NUM_MAX(%d)!\n", \
                                                                pstFltInfo, phFilter, i, MS_DMX_CH_NUM_MAX);
        return MCA_FAILURE;
    }

    mstar_dmx_wait();

    for (j = 0; j < MS_FLT_NUM_MAX; j++)
    {
        if (MS_INVALID_FLT_ID == g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID)
        {
            break;
        }
    }
    if (j >= MS_FLT_NUM_MAX)
    {
        MS_DMX_ERR("There is no free filter!\n");
        goto ms_error;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Open(DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE, &u8NewFlt))
    {
        MS_DMX_ERR("MApi_DMX_Open Error!\n");
        goto ms_error;
    }    
    
    u32BuffSize = (g_astMstarChannel[i].m_stChanInfo.m_u32BuffSize) << 1;
    pu8Buffer   = mca_malloc(u32BuffSize);
    if (NULL == pu8Buffer)
    {
        MS_DMX_ERR("mca_malloc(0x%x) Failed!\n", u32BuffSize);
        goto ms_error;
    }

    if (DMX_FILTER_STATUS_OK != MApi_DMX_Pid(u8NewFlt, &g_astMstarChannel[i].m_stChanInfo.m_pid, TRUE))
    {
        MS_DMX_ERR("MApi_DMX_Pid Fail!\n");
        goto ms_error;
    }
    
    u32MsFltType = DMX_FILTER_TYPE_SECTION | DMX_FILTER_SOURCE_TYPE_LIVE;
    stMsFlt.Info.SectInfo.SectBufAddr   = (MS_U32)MsOS_VA2PA((MS_U32)pu8Buffer);
    stMsFlt.Info.SectInfo.SectBufSize   = u32BuffSize;
    if (MCA_TRUE == g_astMstarChannel[i].m_stChanInfo.m_bCRC)
    {
        stMsFlt.Info.SectInfo.SectMode = DMX_SECT_MODE_CONTI | DMX_SECT_MODE_CRCCHK;
    }
    else
    {
        stMsFlt.Info.SectInfo.SectMode = DMX_SECT_MODE_CONTI;
    }
    //stMsFlt.Info.SectInfo.SectMode      = DMX_SECT_MODE_ONESHOT;
    stMsFlt.Info.SectInfo.Event         = DMX_EVENT_DATA_READY | DMX_EVENT_CB_SELF | DMX_EVENT_BUF_OVERFLOW;
    stMsFlt.Info.SectInfo.pNotify       = mstar_dmx_filter_notify;
    if (DMX_FILTER_STATUS_OK != MApi_DMX_Info(u8NewFlt, &stMsFlt, &u32MsFltType, TRUE))
    {
        MS_DMX_ERR("MApi_DMX_Info(...) Error!\n");
        goto ms_error;
    }
    MApi_DMX_Stop(u8NewFlt);

    if (pstFltInfo->m_u8Depth != 0)
    {
        mca_memset(au8Match, 0x00, DMX_SECT_FLT_DEPTH);
        mca_memset(au8Mask,  0x00, DMX_SECT_FLT_DEPTH);
        mca_memset(au8NMask, 0x00, DMX_SECT_FLT_DEPTH);

        mca_memcpy(au8Match, pstFltInfo->m_au8Match, pstFltInfo->m_u8Depth);
        mca_memcpy(au8Mask,  pstFltInfo->m_au8Mask,  pstFltInfo->m_u8Depth);
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SectPatternSet(u8NewFlt, au8Match, au8Mask, au8NMask, DMX_SECT_FLT_DEPTH))
        {
            MS_DMX_ERR("MApi_DMX_SectPatternSet(...) Error!\n");
            goto ms_error;
        }
    }

    mca_memcpy(g_astMstarChannel[i].m_astFltInfo[j].m_au8Mask,  au8Mask,  DMX_SECT_FLT_DEPTH);
    mca_memcpy(g_astMstarChannel[i].m_astFltInfo[j].m_au8Match, au8Match, DMX_SECT_FLT_DEPTH);
    g_astMstarChannel[i].m_astFltInfo[j].m_u8Depth      = pstFltInfo->m_u8Depth;
    g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID   = u8NewFlt;
    g_astMstarChannel[i].m_astFltInfo[j].m_pu8SecBuff   = pu8Buffer;

    *phFilter = (hChannel<<16) | j;

    mstar_dmx_post();

    return MCA_SUCCESS;

ms_error:
    if (u8NewFlt != 0xFF)
    {
        MApi_DMX_Close(u8NewFlt);
    }
    if (pu8Buffer)
    {
        mca_free(pu8Buffer);
        pu8Buffer = NULL;
    }
    *phFilter = MCA_INVALID_HANDLE;
    mstar_dmx_post();
    
    return MCA_FAILURE;
}

MCA_S32 mca_dmx_filter_set(MCA_HANDLE hFilter, const MCA_DMX_FILTER_t *pstFltInfo)
{
    MCA_U16 i, j;
    MS_U8   au8Match[DMX_SECT_FLT_DEPTH];
    MS_U8   au8Mask [DMX_SECT_FLT_DEPTH];
    MS_U8   au8NMask[DMX_SECT_FLT_DEPTH];

    if (NULL == pstFltInfo)
    {
        MS_DMX_ERR("Bad Parameter: Pointer is NULL!\n");
        return MCA_FAILURE;
    }

    i = hFilter >> 16;
    j = hFilter & 0xFF;

    mstar_dmx_wait();

    if (pstFltInfo->m_u8Depth != 0)
    {
        mca_memset(au8Match, 0x00, DMX_SECT_FLT_DEPTH);
        mca_memset(au8Mask,  0x00, DMX_SECT_FLT_DEPTH);
        mca_memset(au8NMask, 0x00, DMX_SECT_FLT_DEPTH);

        mca_memcpy(au8Match, pstFltInfo->m_au8Match, pstFltInfo->m_u8Depth);
        mca_memcpy(au8Mask,  pstFltInfo->m_au8Mask,  pstFltInfo->m_u8Depth);
        if (DMX_FILTER_STATUS_OK != MApi_DMX_SectPatternSet(g_astMstarChannel[i].m_astFltInfo[j].m_u16HwFltID,   \
                                                                au8Match, au8Mask, au8NMask, DMX_SECT_FLT_DEPTH))
        {
            MS_DMX_ERR("MApi_DMX_SectPatternSet(...) Error!\n");
            mstar_dmx_post();
            return MCA_FAILURE;
        }
    }

    mstar_dmx_post();

    return MCA_SUCCESS;
}

MCA_S32 mca_dmx_filter_destory(MCA_HANDLE hFilter)
{
    MCA_U16 i, j;

    i = hFilter >> 16;
    j = hFilter & 0xFF;

    mstar_dmx_wait();
    
    mstar_reset_filter_info(&(g_astMstarChannel[i].m_astFltInfo[j]));

    mstar_dmx_post();

    return MCA_SUCCESS;
}

