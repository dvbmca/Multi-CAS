#include <stdio.h>
#include "mca_adaptation_files.h"
#include "mca_app_common.h"

static void mca_conax_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_CONAX != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_conax_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_ConaxSmcInfo_t *pstCxInfo = (MCA_ConaxSmcInfo_t *)pData;

    if (MCA_CAS_CONAX != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstCxInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }
    
    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s smart card information: >>\n", aszCASName[enCASType]);
    mca_printf(">> Interface Version : 0x%02X\n", pstCxInfo->m_u8InterfaceVersion);
    mca_printf(">> Card Number       : %s\n",     pstCxInfo->m_szCardNumber);
    mca_printf(">> NumberOfSessions  : %d\n",     pstCxInfo->m_u8NumberOfSessions);
    mca_printf(">> Language          : %d\n",     pstCxInfo->m_u16CountryIndicatorValue);
    mca_printf(">> CA_SYS_ID         : 0x%04X\n", pstCxInfo->m_u16CASysID);
    mca_printf(">> MAT_RAT_LEVEL     : 0x%02X\n", pstCxInfo->m_u8MatRatLevel);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_conax_app_sub_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U32 i;
    MCA_ConaxSubscription_t     *pstCxSubInfo = (MCA_ConaxSubscription_t *)pData;
    MCA_ConaxSubscriptionNode_t *pstNode      = NULL;

    if (MCA_CAS_CONAX != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstCxSubInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }
    
    mca_printf("\n%s subscription information: count = %d>>\n", aszCASName[enCASType], pstCxSubInfo->m_u16Count);
    for (i = 0; i < pstCxSubInfo->m_u16Count; i++)
    {
        pstNode = &(pstCxSubInfo->m_astEntries[i]);
        mca_printf(">> Index %d.\n", i);
        mca_printf(">> Subscription Ref = %d.\n", pstNode->m_u16SubscriptionRef);
        mca_printf(">> Lable            = %s.\n", pstNode->m_szLable);
        mca_printf(">> %04d.%02d.%02d - %04d.%02d.%02d %08X\n", pstNode->m_astStart[0].m_Year,  \
                                                                pstNode->m_astStart[0].m_Month, \
                                                                pstNode->m_astStart[0].m_Day,   \
                                                                pstNode->m_astEnd[0].m_Year,    \
                                                                pstNode->m_astEnd[0].m_Month,   \
                                                                pstNode->m_astEnd[0].m_Day,     \
                                                                pstNode->m_au32Entitlement[0]);
        mca_printf(">> %04d.%02d.%02d - %04d.%02d.%02d %08X\n", pstNode->m_astStart[1].m_Year,  \
                                                                pstNode->m_astStart[1].m_Month, \
                                                                pstNode->m_astStart[1].m_Day,   \
                                                                pstNode->m_astEnd[1].m_Year,    \
                                                                pstNode->m_astEnd[1].m_Month,   \
                                                                pstNode->m_astEnd[1].m_Day,     \
                                                                pstNode->m_au32Entitlement[1]);
        mca_printf("\n");
    }
    mca_printf("\n");
}

static void mca_conax_app_change_pin_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CONAX != enCASType)
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

static void mca_conax_app_check_pin_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CONAX != enCASType)
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

static void mca_conax_app_set_maturity_rating_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    char     szError[20];
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CONAX != enCASType)
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
        case MCA_E03_WRONG_MATURITY_RATING:
            snprintf(szError, 20, "%s", "Wrong Rating Value");
            break;
        default:
            snprintf(szError, 20, "%s", "Fail");
            break;
    }

    MCA_APP_DBG("%s set maturity rating reply: %s>>\n", aszCASName[enCASType], szError);
}

static void mca_conax_app_view_parental_channel_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (MCA_CAS_CONAX != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == ps32Status)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    MCA_APP_DBG("%s View Parental Channel Reply: %s>>\n", aszCASName[enCASType], (MCA_SUCCESS == (*ps32Status))?"Success":"Fail");
}

static MCA_VOID mca_conax_app_unpair_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    char     szError[20];
    MCA_S32 *ps32Status = (MCA_S32 *)pData;

    if (NULL == ps32Status)
    {
        return;
    }

    switch (*ps32Status)
    {
        case MCA_SUCCESS:
            snprintf(szError, 20, "%s", "Success");
            break;
        case MCA_E08_NOT_SUPPORT:
            snprintf(szError, 20, "%s", "Not Support");
            break;
        default:
            snprintf(szError, 20, "%s", "Fail");
            break;
    }

    mca_printf("%s unpair reply: %s>>\n", aszCASName[enCASType], szError);
}

MCA_S32 mca_conax_app_init(MCA_VOID)
{
    if (MCA_CAS_CONAX != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE,           mca_conax_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,               mca_conax_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_SUB_INFO,               mca_conax_app_sub_callback);
    MCA_RegisterEvent(MCA_EVENT_CHANGE_PIN,             mca_conax_app_change_pin_callback);
    MCA_RegisterEvent(MCA_EVENT_CHECK_PIN,              mca_conax_app_check_pin_callback);
    MCA_RegisterEvent(MCA_EVENT_SET_MATURITY_RATING,    mca_conax_app_set_maturity_rating_callback);
    MCA_RegisterEvent(MCA_EVENT_VIEW_PARENTAL_CHANNEL,  mca_conax_app_view_parental_channel_callback);
    MCA_RegisterEvent(MCA_EVENT_UNPAIR,                 mca_conax_app_unpair_callback);

    return MCA_SUCCESS;
}

