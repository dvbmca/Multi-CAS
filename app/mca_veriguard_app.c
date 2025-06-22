#include <stdio.h>
#include "mca_adaptation_files.h"
#include "mca_app_common.h"

static void mca_veriguard_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_VeriGuardCardInfo_t *pstVgInfo = (MCA_VeriGuardCardInfo_t *)pData;

    if (MCA_CAS_VERIGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstVgInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }
    
    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s smart card information: >>\n", aszCASName[enCASType]);
    mca_printf(">> Serial Number        : %s\n",       pstVgInfo->m_szSerialNumber);
    mca_printf(">> PPUA                 : %08X\n",     pstVgInfo->m_u32PPUA);
    mca_printf(">> Status               : %s\n",       (MCA_TRUE==pstVgInfo->m_b8Status)?"Active":"Not Active");
    mca_printf(">> Start Date           : %s\n",       pstVgInfo->m_szStartDate);
    mca_printf(">> End   Date           : %s\n",       pstVgInfo->m_szExpiredDate);
    mca_printf(">> Access Certification : %08X\n",     pstVgInfo->m_u32AccessCertification);
    mca_printf(">> Provider Name        : %s\n",       pstVgInfo->m_szProviderName);
    mca_printf(">> Shared Address       : %08X\n",     pstVgInfo->m_u32SharedAddress);
    mca_printf(">> Provider CHID        : %04X\n",     pstVgInfo->m_u16ProviderCHID);
    mca_printf(">> Provider CAID        : %04X\n",     pstVgInfo->m_u16ProviderCAID);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_veriguard_app_osd_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_VeriGuardOSD_t *pstVgOSDInfo = (MCA_VeriGuardOSD_t *)pData;

    if (MCA_CAS_VERIGUARD != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstVgOSDInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s Pop Message\n",               aszCASName[enCASType]);
    mca_printf(">> Provider Name    : %s\n",     pstVgOSDInfo->m_szProviderName);
    mca_printf(">> Serial Number    : %s\n",     pstVgOSDInfo->m_szSerialNumber);
    mca_printf(">> Message          : %s\n",     pstVgOSDInfo->m_szContent);
    mca_printf(">> Timeout          : %u\n",     pstVgOSDInfo->m_u8Timeount);
    switch (pstVgOSDInfo->m_emCommand)
    {
        case MCA_VG_COMMAND_NONE:
            mca_printf(">> Command 00       : No Command.\n");
            break;
        case MCA_VG_COMMAND_REBOOT:
            mca_printf(">> Command 01       : Reboot.\n");
            break;
        case MCA_VG_COMMAND_SHUTDOWN:
            mca_printf(">> Command 02       : Shutdown.\n");
            break;
        case MCA_VG_COMMAND_MUTE:
            mca_printf(">> Command 03       : Mute.\n");
            break;
        case MCA_VG_COMMAND_STB_INFO:
            mca_printf(">> Command 04       : Display the stb information menu.\n");
            break;
        default:
            mca_printf(">> Command XX       : Unknown Command.\n");
            break;
    }    
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

MCA_S32 mca_veriguard_app_init(MCA_VOID)
{
    if (MCA_CAS_VERIGUARD != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_SMC_INFO, mca_veriguard_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_OSD,      mca_veriguard_app_osd_callback);

    return MCA_SUCCESS;
}

