#include "mca_adaptation_files.h"
#include "mca_app_common.h"

MCA_U16 g_u16CryptoGuardInterfaceVersion;

static void mca_cryptoguard_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_CRYPTOGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_cryptoguard_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_CryptoGuardSmcInfo_t *pstGfInfo = (MCA_CryptoGuardSmcInfo_t *)pData;

    if (MCA_CAS_CRYPTOGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstGfInfo)
    {
        MCA_APP_DBG("pstGfInfo is NULL.\n");
        return;
    }

    g_u16CryptoGuardInterfaceVersion = pstGfInfo->m_u16ProtocolVer;

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("About CA                            >>\n");
    mca_printf(">> Interface version: %d.%02d\n",   pstGfInfo->m_u16ProtocolVer>>8, pstGfInfo->m_u16ProtocolVer&0xFF);
    mca_printf(">> Card number      : %011u\n",     pstGfInfo->m_u32CardNumber);
    mca_printf(">> Box id           : %011u\n",     pstGfInfo->m_u32IrdNumber);
    mca_printf(">> CA System ID     : %04X\n",      pstGfInfo->m_u16CASystemID);
    mca_printf(">> CA Versiion      : 2.12\n");
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_cryptoguard_app_sub_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U32                 i;
    MCA_U32                 cnt;
    MCA_CryptoGuardSub_t   *pstCgSubInfo = (MCA_CryptoGuardSub_t *)pData;

    if (MCA_CAS_CRYPTOGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstCgSubInfo)
    {
        MCA_APP_DBG("pstCgSubInfo is NULL.\n");
        return;
    }
    
    mca_printf("\nSubscription: >>\n");
    for (i = 0; i < pstCgSubInfo->m_u8ChannelSubCnt; i++)
    {
        mca_printf(">> %02d Expire date %04d-%02d-%02d %02X %02X %02X %02X\n",    \
                                i/2,   \
                                pstCgSubInfo->m_astChannel[i].m_stDateTo.m_Year,  \
                                pstCgSubInfo->m_astChannel[i].m_stDateTo.m_Month, \
                                pstCgSubInfo->m_astChannel[i].m_stDateTo.m_Day,   \
                                pstCgSubInfo->m_astChannel[i].m_au8Field[0],    \
                                pstCgSubInfo->m_astChannel[i].m_au8Field[1],    \
                                pstCgSubInfo->m_astChannel[i].m_au8Field[2],    \
                                pstCgSubInfo->m_astChannel[i].m_au8Field[3]);
    }
    if (0 == pstCgSubInfo->m_u8ChannelSubCnt)
    {
        mca_printf(">> No subscriptions.\n");
    }
    mca_printf("\n");

    mca_printf("\nPay-per-view: >>\n");
    cnt = 0;
    for (i = 0; i < pstCgSubInfo->m_u8EventSubCnt; i++)
    {
        if (pstCgSubInfo->m_astEvent[i].m_u32ProgNum != 0)
        {
            mca_printf(">> Subscribed to program number: %d\n", pstCgSubInfo->m_astEvent[i].m_u32ProgNum);
            cnt++;
        }
    }
    if (0 == cnt)
    {
        mca_printf(">> No subscriptions.\n");
    }
    mca_printf("\n");
}

static void mca_cryptoguard_app_check_pin_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CRYPTOGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == ps32Status)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    MCA_APP_DBG("%s check PIN reply: %s>>\n", aszCASName[enCASType], (MCA_SUCCESS == (*ps32Status))?"Success":"Fail");
}

static void mca_cryptoguard_app_change_pin_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CRYPTOGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == ps32Status)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    MCA_APP_DBG("%s change PIN reply: %s>>\n", aszCASName[enCASType], (MCA_SUCCESS == (*ps32Status))?"Success":"Fail");
}

static void mca_cryptoguard_app_set_maturity_rating_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CRYPTOGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == ps32Status)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    MCA_APP_DBG("%s set maturity rating reply: %s>>\n", aszCASName[enCASType], (MCA_SUCCESS == (*ps32Status))?"Success":"Fail");
}

MCA_S32 mca_cryptoguard_app_init(MCA_VOID)
{
    if (MCA_CAS_CRYPTOGUARD != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE,       mca_cryptoguard_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,           mca_cryptoguard_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_SUB_INFO,           mca_cryptoguard_app_sub_callback);
    MCA_RegisterEvent(MCA_EVENT_CHECK_PIN,          mca_cryptoguard_app_check_pin_callback);
    MCA_RegisterEvent(MCA_EVENT_CHANGE_PIN,         mca_cryptoguard_app_change_pin_callback);
    MCA_RegisterEvent(MCA_EVENT_SET_MATURITY_RATING,mca_cryptoguard_app_set_maturity_rating_callback);

    return MCA_SUCCESS;
}

