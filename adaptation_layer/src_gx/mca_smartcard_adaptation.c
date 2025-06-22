#include "gxtype.h"
#include "gxsecure/gxsci_api.h"

#include "mca_adaptation_files.h"

#define GX_SMC_DBG(fmt, args...)        \
    do{    \
    	mca_printf("----gx smc>[%s]Line%d: ", __FUNCTION__, __LINE__);    \
    	mca_printf(fmt, ##args);    \
    }while(0)

#define GX_SMC_ERR(fmt, args...)        \
    do{    \
    	mca_printf("\033[1;40;31m");    \
    	mca_printf("\n################MS SMC ERROR################\n");    \
    	mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__);    \
    	mca_printf(fmt, ##args);    \
    	mca_printf("\n############################################\n\n");    \
    	mca_printf("\033[0m\n");\
    }while(0)

#define GX_SMC_MODULE_NAME  "gx.smc"

static MCA_HANDLE   g_hGxSmcSemaphore   = MCA_INVALID_HANDLE;
static MCA_BOOL     g_b8SmcIsInserted   = MCA_FALSE;
static MCASMCStatusCallback g_cbSmcCb   = NULL;

static MCA_VOID gx_smc_capture_semaphore(MCA_VOID)
{
    if (MCA_INVALID_HANDLE == g_hGxSmcSemaphore)
    {
        GX_SMC_ERR("Invalid semaphore handle.");
        return ;
    }

    MCA_OS_SemLock(g_hGxSmcSemaphore);
}

static MCA_VOID gx_smc_release_semaphore(MCA_VOID)
{
    if (MCA_INVALID_HANDLE == g_hGxSmcSemaphore)
    {
        GX_SMC_ERR("Invalid semaphore handle.");
        return ;
    }

    MCA_OS_SemUnlock(g_hGxSmcSemaphore);
}

static MCA_VOID *gx_smartcard_task(MCA_VOID *pParam)
{
    MCA_SMC_STATUS_t enMcaStatus;

    int32_t         s32GxRet;
    GxSciCardStatus enGxPreStatus = GXSCI_CARD_OUT;
    GxSciCardStatus enGxNewStatus;

    while (1)
    {
        mca_sleep(200);

        gx_smc_capture_semaphore();
        if (NULL == g_cbSmcCb)
        {
            GX_SMC_ERR("callback is 0x%x", g_cbSmcCb);
            gx_smc_release_semaphore();
            mca_sleep(400);
            continue;
        }
        gx_smc_release_semaphore();

        s32GxRet = GxSci_GetStatus(&enGxNewStatus);
        if (s32GxRet != 0)
        {
            GX_SMC_ERR("GxSci_GetStatus(...) = %d", s32GxRet);
            continue;
        }
        if (enGxPreStatus == enGxNewStatus)
        {
            continue;
        }

        gx_smc_capture_semaphore();
        switch (enGxNewStatus)
        {
            case GXSCI_CARD_IN:
                enMcaStatus = MCA_SMC_IN; g_b8SmcIsInserted = MCA_TRUE; break;
            case GXSCI_CARD_OUT:
                enMcaStatus = MCA_SMC_OUT;g_b8SmcIsInserted = MCA_FALSE;break;
            default:
                enMcaStatus = MCA_SMC_UNKNOWN;g_b8SmcIsInserted = MCA_FALSE;break;
        }
        enGxPreStatus = enGxNewStatus;
        gx_smc_release_semaphore();

        g_cbSmcCb(0, enMcaStatus);
    }

    return NULL;
}

static MCA_VOID gx_configurate_smartcard(MCA_VOID)
{
    int32_t         s32GxRet;
    GxSciParams     stGxParam;
    GxSciTimeParams stGxTimeParam;

    mca_memset(&stGxParam, 0x00, sizeof(stGxParam));
    stGxParam.protocol                = DISABLE_REPEAT_WHEN_ERR;
    stGxParam.stop_len                = GXSCI_STOPLEN_0BIT;
    stGxParam.io_conv                 = GXSCI_DATA_CONV_DIRECT;
    stGxParam.parity                  = GXSCI_PARITY_EVEN;
    stGxParam.detect                  = GXSCI_HIGH_LEVEL;
    stGxParam.vcc                     = GXSCI_HIGH_LEVEL;
    stGxParam.default_etu             = 372;
    stGxParam.auto_etu                = 0;
    stGxParam.auto_io_conv_and_parity = 1;
    s32GxRet = GxSci_Setup(&stGxParam);
    if (s32GxRet != 0)
    {
        GX_SMC_ERR("GxSci_Setup(...) = %d", s32GxRet);
        return;
    }

    mca_memset(&stGxTimeParam, 0x00, sizeof(stGxTimeParam));
    stGxTimeParam.clock    = 3571200;
    stGxTimeParam.egt      = 7440;
    stGxTimeParam.etu      = 372;
    stGxTimeParam.tgt      = 0;
    stGxTimeParam.twdt     = 7142400;
    stGxTimeParam.wdt      = 16740;
    stGxTimeParam.protocol = GXSCI_PROTCL_T0;
    s32GxRet = GxSci_Config(&stGxTimeParam);
    if (s32GxRet != 0)
    {
        GX_SMC_ERR("GxSci_Config(...) = %d", s32GxRet);
        return;
    }
}

MCA_S32 mca_smartcard_init(MCA_VOID)
{
    MCA_S32         s32Ret;    
    MCA_HANDLE      hTask;

    int32_t         s32GxRet;
    GxSciParams     stGxParam;

    s32GxRet = GxSci_Open(GXSCI_HIGH_LEVEL, GXSCI_HIGH_LEVEL);
    if (s32GxRet != 0)
    {
        GX_SMC_ERR("GxSci_Open(%d, %d) = %d", GXSCI_HIGH_LEVEL, GXSCI_HIGH_LEVEL, s32GxRet);
        return MCA_FAILURE;
    }

    mca_memset(&stGxParam, 0x00, sizeof(stGxParam));
    stGxParam.protocol                = DISABLE_REPEAT_WHEN_ERR;
    stGxParam.stop_len                = GXSCI_STOPLEN_0BIT;
    stGxParam.io_conv                 = GXSCI_DATA_CONV_DIRECT;
    stGxParam.parity                  = GXSCI_PARITY_ODD;
    stGxParam.detect                  = GXSCI_LOW_LEVEL;
    stGxParam.vcc                     = GXSCI_LOW_LEVEL;
    stGxParam.default_etu             = 625;
    stGxParam.auto_etu                = 1;
    stGxParam.auto_io_conv_and_parity = 1;
    s32GxRet = GxSci_Setup(&stGxParam);
    if (s32GxRet != 0)
    {
        GX_SMC_ERR("GxSci_Setup(...) = %d", s32GxRet);
        return MCA_FAILURE;
    }

    s32Ret = MCA_OS_SemCreate(&g_hGxSmcSemaphore, GX_SMC_MODULE_NAME, 1);
    if (MCA_SUCCESS != s32Ret)
    {
        GX_SMC_ERR("MCA_OS_SemCreate(...) = 0x%x.", s32Ret);
        return MCA_FAILURE;
    }
    s32Ret = MCA_OS_TaskCreate(&hTask, GX_SMC_MODULE_NAME, (MCA_TASK_FUNC_T)gx_smartcard_task,  \
                                NULL, NULL, 18 * 1024, MCA_TASK_PRIORITY_NORMAL);
    if (MCA_SUCCESS != s32Ret)
    {
        GX_SMC_ERR("MCA_OS_TaskCreate(...) = 0x%x.", s32Ret);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_reset(MCA_U8 slot, MCA_U8 *atr, MCA_U8 *pu8AtrLen)
{
    MCA_U8  i;

    int32_t s32GxRet;
    uint8_t au8GxATR[255];
    int32_t u32AtrLen;

    if ((NULL == atr) || (NULL == pu8AtrLen))
    {
        GX_SMC_ERR("Bad Param: atr = 0x%x, pu8AtrLen = 0x%x!\n", atr, pu8AtrLen);
        return MCA_FAILURE;
    }

    for (i = 0; i < 2; i++)
    {
        s32GxRet = GxSci_Reset(au8GxATR, 255, &u32AtrLen);
        if (s32GxRet >= 0)
        {            
            GxSciTimeParams stGxParams;
        
            mca_memcpy(atr, au8GxATR, u32AtrLen);
            *pu8AtrLen = (MCA_U8)u32AtrLen;

            mca_memset(&stGxParams, 0x00, sizeof(stGxParams));
            s32GxRet = GxSci_AnalyseAtr(au8GxATR, u32AtrLen, &stGxParams);
            if (s32GxRet < 0)
            {
                GX_SMC_ERR("GxSci_AnalyseAtr(...) = %d", s32GxRet);
            }
            else
            {
                mca_printf("clock   : %u\n", stGxParams.clock);
                mca_printf("etu     : %u\n", stGxParams.etu);
                mca_printf("egt     : %u\n", stGxParams.egt);
                mca_printf("tgt     : %u\n", stGxParams.tgt);
                mca_printf("wdt     : %u\n", stGxParams.wdt);
                mca_printf("twdt    : %u\n", stGxParams.twdt);
                mca_printf("protocol: %u\n", stGxParams.protocol);
            }

            gx_configurate_smartcard();            
            
            return MCA_SUCCESS;
        }
    
        mca_sleep(10);
    }

    GX_SMC_ERR("GxSci_Reset(...) fail \n");

    return MCA_FAILURE;    
}

MCA_BOOL mca_smartcard_is_inserted(MCA_U8 slot)
{
    MCA_BOOL b8Ret;
    
    gx_smc_capture_semaphore();
    b8Ret = g_b8SmcIsInserted;
    gx_smc_release_semaphore();

    return b8Ret;
}

MCA_S32 mca_smartcard_register_status(MCA_U8 slot, MCASMCStatusCallback cb)
{
    gx_smc_capture_semaphore();
    g_cbSmcCb = cb;
    gx_smc_release_semaphore();

    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_unregister_status(MCA_U8 slot, MCASMCStatusCallback cb)
{
    gx_smc_capture_semaphore();
    g_cbSmcCb = NULL;
    gx_smc_release_semaphore();

    return MCA_SUCCESS;
}

static uint8_t g_au8RxBuff[2048];    
MCA_S32 mca_smartcard_T0_command(MCA_U8 slot, MCA_U8 *pu8Send, MCA_U8 u8SendLen, MCA_U8 *pu8Rev, MCA_U16 *pu16RevLen)
{
    int32_t s32GxRet;
    size_t  RxLen;
    uint8_t SW1, SW2;

    if ((NULL == pu8Send) || (u8SendLen < 5) || (NULL == pu8Rev) || (NULL == pu16RevLen))
    {
        GX_SMC_ERR("Bad Param: pu8Send = 0x%x, u8SendLen = %d, pu8Rev = 0x%x, pu16RevLen = 0x%x.", pu8Send, u8SendLen, pu8Rev, pu16RevLen);
        return MCA_FAILURE;
    }

    gx_smc_capture_semaphore();
    s32GxRet = GxSci_Apdu(pu8Send, u8SendLen, g_au8RxBuff, &RxLen, &SW1, &SW2);
    if (0 != s32GxRet)
    {
        GX_SMC_ERR("GxSci_Apdu(...) = %d", s32GxRet);
        gx_smc_release_semaphore();
        return MCA_FAILURE;
    }

    *pu16RevLen = RxLen + 2;
    if (RxLen != 0)
    {
        mca_memcpy(pu8Rev, g_au8RxBuff, RxLen);
    }
    pu8Rev[RxLen]   = SW1;
    pu8Rev[RxLen+1] = SW2;

    gx_smc_release_semaphore();    
    
    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_T14_command(MCA_U8 slot, MCA_U8 *pu8Send, MCA_U8 u8SendLen, MCA_U8 *pu8Rev, MCA_U16 *pu16RevLen)
{
    int32_t s32GxRet;

    if ((NULL == pu8Send) || (0 == u8SendLen) || (NULL == pu8Rev) || (NULL == pu16RevLen))
    {
        GX_SMC_ERR("Bad Param: pu8Send = 0x%x, u8SendLen = %d, pu8Rev = 0x%x, pu16RevLen = 0x%x.", pu8Send, u8SendLen, pu8Rev, pu16RevLen);
        return MCA_FAILURE;
    }

    gx_smc_capture_semaphore();
    s32GxRet = GxSci_Write(pu8Send,u8SendLen,5000);
    if (s32GxRet < 0)
    {
        GX_SMC_ERR("GxSci_Write(...) = %d", s32GxRet);
        gx_smc_release_semaphore();
        return MCA_FAILURE;
    }

    s32GxRet = GxSci_Read(g_au8RxBuff, 2048, 5000);
    if (s32GxRet < 0)
    {
        GX_SMC_ERR("GxSci_Read(...) = %d", s32GxRet);
        gx_smc_release_semaphore();
        return MCA_FAILURE;
    }

    mca_memcpy(pu8Rev, g_au8RxBuff, s32GxRet);
    *pu16RevLen = s32GxRet;

    gx_smc_release_semaphore();    
    
    return MCA_SUCCESS;
}


