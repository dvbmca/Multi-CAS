#include "MsCommon.h"
#include "MsOS.h"
#include "drvSC.h"

#include "mca_adaptation_files.h"

#define MS_SMC_DBG(fmt, args...)        \
    do{    \
    	mca_printf("[Mstar.INFO ][%s L%d]:", __FUNCTION__, __LINE__);    \
    	mca_printf(fmt, ##args);    \
    }while(0)

#define MS_SMC_ERR(fmt, args...)        \
    do{    \
    	mca_printf("[Mstar.ERROR][%s L%d]:", __FUNCTION__, __LINE__);    \
    	mca_printf(fmt, ##args);    \
    }while(0)

#define MSTAR_DEFAULT_SC_ID    0

static MCA_BOOL g_b8SmcIsInserted       = MCA_FALSE;
static MCASMCStatusCallback g_cbSmcCb   = NULL;

static void mstar_smartcard_notify(MS_U8 u8SCID, SC_Event eEvent)
{
    MCA_SMC_STATUS_t enStatus;

    switch (eEvent)
    {
        case E_SC_EVENT_IN:     enStatus = MCA_SMC_IN;      break;
        case E_SC_EVENT_OUT:    enStatus = MCA_SMC_OUT;     break;
        default:                enStatus = MCA_SMC_UNKNOWN; break;
    }

    MS_SMC_DBG("u8SCID = %d, eEvent = %d!\n\n", u8SCID, eEvent);

    g_b8SmcIsInserted = (MCA_SMC_IN == enStatus) ? MCA_TRUE : MCA_FALSE;
    if (g_cbSmcCb)
    {
        g_cbSmcCb(MSTAR_DEFAULT_SC_ID, enStatus);
    }
}

MCA_S32 mca_smartcard_init(MCA_VOID)
{
    SC_Param stSCParam;
    SC_Result enErrCode;

    stSCParam.u8Protocal    = 0;
    stSCParam.eCardClk      = E_SC_CLK_4P5M;
    stSCParam.u8UartMode    = (SC_UART_CHAR_8 | SC_UART_STOP_2 | SC_UART_PARITY_EVEN);
    stSCParam.u16ClkDiv     = 372;
    stSCParam.eVccCtrl      = E_SC_VCC_CTRL_LOW;
    stSCParam.pfOCPControl  = NULL;
    enErrCode = MDrv_SC_Open(MSTAR_DEFAULT_SC_ID, 0, &stSCParam, mstar_smartcard_notify);
    if (enErrCode != E_SC_OK)
    {
        MS_SMC_ERR("MDrv_SC_Open(0) = 0x%x!\n", enErrCode);
        return MCA_FAILURE;
    }
    MsOS_DelayTask(200);
    MDrv_SC_Activate(MSTAR_DEFAULT_SC_ID);

    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_reset(MCA_U8 slot, MCA_U8 *atr, MCA_U8 *pu8AtrLen)
{
    MCA_U8      i;
    SC_Result   enRet;
    MS_U8       au8ATR[SC_ATR_LEN_MAX];
    MS_U16      u16ATRLen;
    MS_U8       au8ATRHistory[SC_HIST_LEN_MAX];
    MS_U16      u16ATRHistoryLen = 200;

    if ((NULL == atr) || (NULL == pu8AtrLen))
    {
        MS_SMC_ERR("Bad Parameter: atr = 0x%x, pu8AtrLen = 0x%x!\n", atr, pu8AtrLen);
        return MCA_FAILURE;
    }

    for (i = 0; i < 2; i++)
    {
        u16ATRLen = SC_ATR_LEN_MAX;
        enRet = MDrv_SC_Reset_ATR(MSTAR_DEFAULT_SC_ID, NULL, au8ATR, &u16ATRLen, au8ATRHistory, &u16ATRHistoryLen);
        if (E_SC_OK == enRet)
        {
            mca_memcpy(atr, au8ATR, u16ATRLen);
            *pu8AtrLen = (MCA_U8)u16ATRLen; 
            return MCA_SUCCESS;
        }
        MsOS_DelayTask(10);        
    }

    MS_SMC_ERR("MDrv_SC_Reset_ATR(...) = 0x%x!\n", enRet);
    return MCA_FAILURE;    
}

MCA_BOOL mca_smartcard_is_inserted(MCA_U8 slot)
{
    return g_b8SmcIsInserted;
}

MCA_S32 mca_smartcard_register_status(MCA_U8 slot, MCASMCStatusCallback cb)
{
    g_cbSmcCb = cb;

    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_unregister_status(MCA_U8 slot, MCASMCStatusCallback cb)
{
    g_cbSmcCb = NULL;

    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_T0_command(MCA_U8 slot, MCA_U8 *pu8Send, MCA_U8 u8SendLen, MCA_U8 *pu8Rev, MCA_U16 *pu16RevLen)
{
    if ((NULL == pu8Send) || (0 == u8SendLen) || (NULL == pu8Rev) || (NULL == pu16RevLen))
    {
        MS_SMC_ERR("Bad Parameter: pu8Send = 0x%x, u8SendLen = %d, pu8Rev = 0x%x, pu16RevLen = 0x%x!\n", \
                                                                pu8Send, u8SendLen, pu8Rev, pu16RevLen);
        return MCA_FAILURE;
    }
    
    if (E_SC_OK != MDrv_SC_T0_SendRecv(MSTAR_DEFAULT_SC_ID, pu8Send, (MS_U16)u8SendLen, pu8Rev, pu16RevLen))
    {
        MS_SMC_ERR("MDrv_SC_T0_SendRecv(...) error!\n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 mca_smartcard_T14_command(MCA_U8 slot, MCA_U8 *pu8Send, MCA_U8 u8SendLen, MCA_U8 *pu8Rev, MCA_U16 *pu16RevLen)
{
    if ((NULL == pu8Send) || (0 == u8SendLen) || (NULL == pu8Rev) || (NULL == pu16RevLen))
    {
        MS_SMC_ERR("Bad Parameter: pu8Send = 0x%x, u8SendLen = %d, pu8Rev = 0x%x, pu16RevLen = 0x%x!\n", \
                                                            pu8Send, u8SendLen, pu8Rev, pu16RevLen);
        return MCA_FAILURE;
    }

    if (E_SC_OK != MDrv_SC_CTI_T14_SendRecv(MSTAR_DEFAULT_SC_ID, pu8Send, (MS_U16)u8SendLen, pu8Rev, pu16RevLen))
    {
        MS_SMC_ERR("MDrv_SC_T0_SendRecv(...) error!\n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

