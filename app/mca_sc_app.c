#include <stdio.h>

#include "mca_adaptation_files.h"
#include "mca_app_common.h"

static void mca_sc_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_SOCHUANG != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_sc_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_SochuangBaseInfo_t *pstScInfo = (MCA_SochuangBaseInfo_t *)pData;

    if (MCA_CAS_SOCHUANG != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstScInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s smart card information:    >>\n", aszCASName[enCASType]);
    mca_printf(">> Card Number   : %s\n", pstScInfo->m_szCardNumber);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_sc_app_sub_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U32                          i;
    MCA_SochuangSubscription_t      *pstScSubInfo = (MCA_SochuangSubscription_t *)pData;
    MCA_SochuangSubscriptionNode_t  *pstNode      = NULL;

    if (MCA_CAS_SOCHUANG != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstScSubInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n%s subscription information: count = %d>>\n", aszCASName[enCASType], pstScSubInfo->m_u8Count);
    for (i = 0; i < pstScSubInfo->m_u8Count; i++)
    {
        pstNode = &(pstScSubInfo->m_astEntries[i]);
        mca_printf(">> Index %d.\n", i);
        mca_printf(">> ID   = %d.\n", pstNode->m_u16ID);
        mca_printf(">> %04d.%02d.%02d - %04d.%02d.%02d\n",  pstNode->m_astStart.m_Year,     \
                                                            pstNode->m_astStart.m_Month,    \
                                                            pstNode->m_astStart.m_Day,      \
                                                            pstNode->m_astEnd.m_Year,       \
                                                            pstNode->m_astEnd.m_Month,      \
                                                            pstNode->m_astEnd.m_Day);
        mca_printf("\n");
    }
    mca_printf("\n");
}

static void mca_sc_app_ippv_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_SochuangIPPV_t *pstScIppvInfo = (MCA_SochuangIPPV_t *)pData;

    if (MCA_CAS_SOCHUANG != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstScIppvInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s IPPV information:    >>\n", aszCASName[enCASType]);
    mca_printf(">> Program Number : %d\n", pstScIppvInfo->m_u16ProgramNum);
    mca_printf(">> Price          : %d\n", pstScIppvInfo->m_u32Price);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

/* ============================ */
/*             IPPV             */
/* Program number: xxxx         */
/* Program name  : xxxx         */
/* Price         : xxxx         */

/*         * * * * * *          */
/* ============================ */
/* Please input PIN to buy!     */
/* ============================ */
}

static void mca_sc_app_ippv_buy_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U8   szError[20];
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_SOCHUANG != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == ps32Status)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    switch (*ps32Status)
    {
        case MCA_SUCCESS:
            snprintf(szError, 20, "%s", "Success");
            break;
        case MCA_E02_WRONG_PIN:
            snprintf(szError, 20, "%s", "Wrong PIN");
            break;
        case MCA_E05_INSUFFICIENT_BALANCE:
            snprintf(szError, 20, "%s", "Insufficient Balance");
            break;
        default:
            snprintf(szError, 20, "%s", "Fail");
            break;
    }

    MCA_APP_DBG("%s IPPV Buy reply: %s>>\n", aszCASName[enCASType], szError);
}

MCA_S32 mca_sc_app_init(MCA_VOID)
{
    if (MCA_CAS_SOCHUANG != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE,   mca_sc_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,       mca_sc_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_SUB_INFO,       mca_sc_app_sub_callback);
    MCA_RegisterEvent(MCA_EVENT_IPPV,           mca_sc_app_ippv_callback);
    MCA_RegisterEvent(MCA_EVENT_IPPV_BUY,       mca_sc_app_ippv_buy_callback);

    return MCA_SUCCESS;
}

