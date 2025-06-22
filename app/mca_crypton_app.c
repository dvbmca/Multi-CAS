#include <stdio.h>

#include "mca_adaptation_files.h"
#include "mca_app_common.h"

static void mca_crypton_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_CRYPTON != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_crypton_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_CryptonSmcInfo_t *pstScInfo = (MCA_CryptonSmcInfo_t *)pData;

    if (MCA_CAS_CRYPTON != enCASType)
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
    mca_printf(">> Card Number : %08u\n", pstScInfo->m_u32CardNumber);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

MCA_S32 mca_crypton_app_init(MCA_VOID)
{
    if (MCA_CAS_CRYPTON != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE,   mca_crypton_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,       mca_crypton_app_smc_info_callback);

    return MCA_SUCCESS;
}

