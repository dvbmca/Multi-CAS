#include <stdio.h>

#include "mca_adaptation_files.h"
#include "mca_app_common.h"

MCA_CHAR mca_app_date_2_string(MCA_DATE_t stDate)
{
    static MCA_CHAR szString[24];

    snprintf(szString, sizeof(szString), \
                "%04d-%d-%d %02d:%02d:%02d", \
                stDate.m_Year, stDate.m_Month, \
                stDate.m_Day,  stDate.m_Hour,  \
                stDate.m_Min,  stDate.m_Second);

    return szString;
}

static void mca_sumavision_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_SUMAVISION != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_sumavision_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_SumavisionCardInfo_t *pstScInfo = (MCA_SumavisionCardInfo_t *)pData;

    if (MCA_CAS_SUMAVISION != enCASType)
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
    mca_printf(">> Smart card inner ID : %u\n", pstScInfo->m_u32CardID);
    mca_printf(">> CAS Module Version  : %s\n", pstScInfo->m_szCASMouleVer);
    mca_printf(">> CAS Supplier        : %s\n", pstScInfo->m_szCASSupplier);
    mca_printf(">> COS Version         : %u\n", pstScInfo->m_u32COSVer);

    if (((-1) == pstScInfo->m_u32AreaCode) || (MCA_SumavisionAreaUnknown == pstScInfo->m_enAreaStatus))
    {
        mca_printf(">> Area code          : \n");
        mca_printf(">> Area status        : \n");
        mca_printf(">> Area time          : \n");
    }
    else
    {
        mca_printf(">> Area code          : %u\n", pstScInfo->m_u32AreaCode);
        switch (pstScInfo->m_enAreaStatus)
        {
            case MCA_SumavisionAreaLock:
                mca_printf(">> Area status        : %s\n", "Lock");
                break;
            case MCA_SumavisionAreaUnlock:
                mca_printf(">> Area status        : %s\n", "Unlock");
                break;
            default:
                mca_printf(">> Area status        : %s\n", " ");
                break;
        }
        mca_printf(">> Area time          : %s\n", mca_app_date_2_string(pstScInfo->m_stAreaTime));
    }

    if (MCA_SumavisionCardUnknown == pstScInfo->m_enCardType)
    {        
        mca_printf(">> Card Type           : %s\n", " ");
    }
    else
    {        
        mca_printf(">> Card Type           : %s\n", (MCA_SumavisionCardMaster == pstScInfo->m_enCardType) ? "Master" : "Slave");
        if (MCA_SumavisionCardSlave == pstScInfo->m_enCardType)
        {            
            mca_printf(">> Master Card ID      : %u\n", pstScInfo->m_u32MasterCardID);
        }
    }

    switch (pstScInfo->m_enPINStatus)
    {
        case MCA_SumavisionPINLock:
            mca_printf(">> PIN status         : %s\n", "PIN Lock");
            break;
        case MCA_SumavisionPINUnlock:
            mca_printf(">> PIN status         : %s\n", "PIN Unlock");
            break;
        default:
            mca_printf(">> PIN status         : %s\n", " ");
            break;
    }
    if (0xFF == pstScInfo->m_u8Rating)
    {
        mca_printf(">> Rating             : \n");
    }
    else
    {
        mca_printf(">> Rating             : %u\n", pstScInfo->m_u8Rating);
    }    
    mca_printf(">> Work time          : %s\n", pstScInfo->m_szWorkTime);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_sumavision_app_sub_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U32                          i, j;
    MCA_SumavisionOperator_t        *pstScSubInfo = (MCA_SumavisionOperator_t *)pData;
    MCA_SumavisionOperatorInfo_t    *pstNode      = NULL;
    MCA_SumavisionSubscription_t    *pstEntitle   = NULL;

    if (MCA_CAS_SUMAVISION != enCASType)
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
        pstNode = &(pstScSubInfo->m_astInfo[i]);
        mca_printf("%d      %s                  %d\n", pstNode->m_u16ID, pstNode->m_szName, pstNode->m_u8Count);      
        for (j = 0; j < pstNode->m_u8Count; j++)
        {
            pstEntitle = pstNode->m_pstEntitle + j;
        
            mca_printf("  Index %d:\n", j);
            mca_printf("  Entitle name:%s\n", pstEntitle->m_szName);
            mca_printf("  Entitle time:%s\n", mca_app_date_2_string(pstEntitle->m_stEntitleTime));
            mca_printf("  Start   time:%s\n", mca_app_date_2_string(pstEntitle->m_stStartTime));
            mca_printf("  End     time:%s\n", mca_app_date_2_string(pstEntitle->m_stEndTime));
        }        
        mca_printf("\n");
    }
    mca_printf("\n");
}

MCA_S32 mca_sumavision_app_init(MCA_VOID)
{
    if (MCA_CAS_SUMAVISION != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE,   mca_sumavision_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,       mca_sumavision_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_SUB_INFO,       mca_sumavision_app_sub_callback);

    return MCA_SUCCESS;
}

