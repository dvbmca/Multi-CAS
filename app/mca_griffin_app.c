#include "mca_adaptation_files.h"
#include "mca_app_common.h"

static void mca_griffin_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_GRIFFIN != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_griffin_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_GriffinSmcInfo_t *pstGfInfo = (MCA_GriffinSmcInfo_t *)pData;

    if (MCA_CAS_GRIFFIN != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstGfInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s smart card information: >>\n", aszCASName[enCASType]);
    mca_printf(">> Serial Number   : %s\n", pstGfInfo->m_szSerialNumber);
    mca_printf(">> CAS ID          : 0x%04X\n", pstGfInfo->m_u16CASID);
    mca_printf(">> Maturity Rating : %d\n", pstGfInfo->m_u8MaturityRating);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_griffin_app_sub_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U32 i;
    MCA_GriffinSubscription_t *pstGfSubInfo = (MCA_GriffinSubscription_t *)pData;

    if (MCA_CAS_GRIFFIN != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstGfSubInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }
    
    mca_printf("\n%s subscription information: count = %d>>\n", aszCASName[enCASType], pstGfSubInfo->m_u16Sum);
    for (i = 0; i < pstGfSubInfo->m_u16Sum; i++)
    {
        mca_printf(">> Subscription %02d: 0x%04x.\n", i, pstGfSubInfo->m_au16Entries[i]);
    }
    mca_printf("\n");
}

MCA_S32 mca_griffin_app_init(MCA_VOID)
{
    if (MCA_CAS_GRIFFIN != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE, mca_griffin_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,     mca_griffin_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_SUB_INFO,     mca_griffin_app_sub_callback);

    return MCA_SUCCESS;
}

