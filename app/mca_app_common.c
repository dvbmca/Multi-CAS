#include "mca_adaptation_files.h"

extern MCA_U16 g_u16CryptoGuardInterfaceVersion;

/* Refer to MCA_CAS_TYPE_t */
const MCA_CHAR aszCASName[MCA_CAS_AMOUNT][16] = {
    "CTI",
    "Conax",
    "Griffin",
    "CryptoGuard",
    "SoChuang",
    "DVCrypt",
    "Crypton",
    "ABV",
    "Topreal",
    "Gospell",
    "Sumavision",
    "TVCAS",
    "VeriGuard"
};

/* Refer to MCA_MESSAGE_CODE_t */
const MCA_CHAR aszCASMessage[][44] = {
    "M00 FTA Service.",
    "M01 Please insert smart card.",
    "M02 Unknown smart card.",
    "M03 Checking smart card.",
    "M04 No Service Avaliable.",
    "M05 Slave Card Valid.",
    "M06 Smart card is ready.",
    "M07 Slave Card Invalid.",
    "M08 Master Card Invalid.",
    "M09 ***.",
    "M10 Service Is Currently Scrambled.",
    "M11 Not Allowed In This Region.",
    "M12 Smart card is not pairing to this STB.",
    "M13 Parental Control Lock.",
    "M14 CW Error.",
    "M15 No Authorization.",
    "M16 Descrambling works well.",
    "M17 Slave Card Expire.",
    "M18 User View Query",
    "M19 Regional Control.",
    "M20 IPPV Service.",
    "M21 Program Authorization is Paused.",
    "M22 Smart card is not active.",
    "M23 ",
    "M24 Smart card freeze.",
    "M25 ",
    "M26 ",
    "M27 ",
    "M28 ",
    "M29 ",
    "M30 ", "M31 ", "M32 ", "M33 ", "M34 ", "M35 ", "M36 ", "M37 ", "M38 ",
    "M39 Switch CAS",
    "M40 ", "M41 ", "M42 ", "M43 ", "M44 ", "M45 ", "M46 ", "M47 ", "M48 ", "M49 ",
    "M50 IPPV On Demand Success.", 
    "M51 IPPV Password Error.", 
    "M52 ",
    "M53 IPPV balance is not enough.",
    "M54 ", "M55 ", "M56 ", "M57 ",    
    "MXX Unknown!"
};

MCA_S32 mca_cti_app_init(MCA_VOID);
MCA_S32 mca_conax_app_init(MCA_VOID);
MCA_S32 mca_griffin_app_init(MCA_VOID);
MCA_S32 mca_cryptoguard_app_init(MCA_VOID);
MCA_S32 mca_sc_app_init(MCA_VOID);
MCA_S32 mca_crypton_app_init(MCA_VOID);
MCA_S32 mca_sumavision_app_init(MCA_VOID);
MCA_S32 mca_tvcas_app_init(MCA_VOID);
MCA_S32 mca_veriguard_app_init(MCA_VOID);

static MCA_VOID mca_authorization_expires_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_BOOL *pb8Status = (MCA_BOOL *)pData;

    mca_printf("\n%s Authorization Expires: %s. >>\n", aszCASName[enCASType], (MCA_TRUE == *pb8Status) ? "Yes" : "No");
}

static MCA_VOID mca_ecm_emm_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_EcmEmm_Info_t *pstInfo = (MCA_EcmEmm_Info_t *)pData;
    if (NULL == pstInfo)
    {
        return;
    }

    mca_printf("\n%s New %s is coming: Demux=%d, PID=0x%x Counter=%lu. >>\n", \
                                aszCASName[enCASType], \
                                (MCA_EVENT_ECM_INFO == enEvtType) ? "ECM" : "EMM", \
                                pstInfo->m_enDmx, \
                                pstInfo->m_pid, \
                                pstInfo->m_u32Counter);
}

static MCA_VOID mca_ts_module_progress(MCA_TS_EVENT_t enEvtType, MCA_VOID *pParam)
{
    if (MCA_TS_EVENT_PACKAGE == enEvtType)
    {
        MCA_PackageCb_t *pstData = (MCA_PackageCb_t *)pParam;
    
        if (NULL != pParam)
        {
            mca_printf("TS Package: File ID=0x%08X, PID=0x%04X, Status=0x%x, Percent=%d% >\n\n",   \
                            pstData->m_u32FileID, pstData->m_PID, pstData->m_s32Status, pstData->m_u8Percent);
        }
    }
    else if (MCA_TS_EVENT_DOWNLOAD == enEvtType)
    {
        MCA_DownloadCb_t *pstData = (MCA_DownloadCb_t *)pParam;

        if (NULL != pParam)
        {
            mca_printf("TS Download: File ID=0x%08X, Status=0x%x, Percent=%d%, New Version=%u, Mem=0x%x, Mem Size=%u >\n\n",   \
                            pstData->m_u32FileID, pstData->m_s32Status, pstData->m_u8Percent, pstData->m_u8NewVersion, \
                            pstData->m_pu8Mem, pstData->m_u32MemSize);           
        }
    }
}

static MCA_VOID mca_usb_dump_callback(MCA_USB_EVENT_t enEvtType, MCA_S32 status, MCA_VOID *pData)
{
    MCA_CHAR *pStr = (MCA_CHAR *)pData;

    if ((enEvtType != MCA_USB_EVENT_DUMP_STB_ID) || (NULL == pStr))
    {
        return;
    }

    if (MCA_SUCCESS == status)
    {        
        mca_printf("Dump STB ID[%s] to USB-Pen successfully!\n", pStr);
    }
    else if (MCA_E07_ALREADY_EXIST == status)
    {        
        mca_printf("STB ID[%s] is already exist!\n", pStr);
    }
    else
    {        
        mca_printf("Dump STB ID[%s] to USB-Pen fail!\n", pStr);
    }

    // Call sync(...)
}

MCA_S32 mca_app_common_init(MCA_VOID)
{
    MCA_S32 s32Ret;

    MCA_RegisterEvent(MCA_EVENT_AUTHORIZATION_EXPIRES, mca_authorization_expires_callback);
    MCA_RegisterEvent(MCA_EVENT_ECM_INFO, mca_ecm_emm_info_callback);
    MCA_RegisterEvent(MCA_EVENT_EMM_INFO, mca_ecm_emm_info_callback);
    MCA_TS_RegisterEvent(MCA_TS_EVENT_PACKAGE,  mca_ts_module_progress);
    MCA_TS_RegisterEvent(MCA_TS_EVENT_DOWNLOAD, mca_ts_module_progress);
    MCA_USB_RegisterEvent(MCA_USB_EVENT_DUMP_STB_ID, mca_usb_dump_callback);

    switch (MCA_GetCASType())
    {
        case MCA_CAS_CTI:
            s32Ret = mca_cti_app_init();

        case MCA_CAS_CONAX:
            s32Ret = mca_conax_app_init();
            break;

        case MCA_CAS_GRIFFIN:
            s32Ret = mca_griffin_app_init();
            break;
            
        case MCA_CAS_CRYPTOGUARD:
            s32Ret = mca_cryptoguard_app_init();
            break;

        case MCA_CAS_SOCHUANG:
            s32Ret = mca_sc_app_init();
            break;

        case MCA_CAS_CRYPTON:
            s32Ret = mca_crypton_app_init();
            break;

        case MCA_CAS_SUMAVISION:
            s32Ret = mca_sumavision_app_init();
            break;

        case MCA_CAS_TVCAS:
            s32Ret = mca_tvcas_app_init();
            break;

        case MCA_CAS_VERIGUARD:
            s32Ret = mca_veriguard_app_init();
            break;

        default:
            s32Ret = MCA_FAILURE;
            break;
    }

    return s32Ret;
}

MCA_S32 mca_app_query_smc_info(MCA_VOID)
{
    MCA_S32 s32Ret;

    s32Ret = MCA_QueryEvent(MCA_EVENT_SMC_INFO, NULL);

    return s32Ret;
}

MCA_S32 mca_app_query_subscription_info(MCA_VOID)
{
    MCA_S32 s32Ret;

    s32Ret = MCA_QueryEvent(MCA_EVENT_SUB_INFO, NULL);

    return s32Ret;
}

MCA_S32 mca_app_query_subscription_detail_info(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret;

    if (MCA_CAS_CTI == enType)
    {
        MCA_CtiPPidDetailRequest_t stInfo;

        stInfo.m_pin[0] = stInfo.m_pin[1] = stInfo.m_pin[2] = stInfo.m_pin[3] = '0';
        stInfo.m_au8PPID_ID[0] = 0x00;
        stInfo.m_au8PPID_ID[1] = 0x00;
        stInfo.m_au8PPID_ID[2] = 0x10;    
        s32Ret = MCA_QueryEvent(MCA_EVENT_CTI_SUB_DETAIL_INFO, &stInfo);
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }

    return s32Ret;
}

MCA_S32 mca_app_check_pin(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret = MCA_SUCCESS;

    if (MCA_CAS_CTI == enType)
    {
        MCA_CtiPIN_t stCtiPIN;        
        stCtiPIN.m_pin[0] = stCtiPIN.m_pin[1] = stCtiPIN.m_pin[2] = stCtiPIN.m_pin[3] = '0';
        s32Ret = MCA_QueryEvent(MCA_EVENT_CHECK_PIN, &stCtiPIN);
    }
    else if (MCA_CAS_CONAX == enType)
    {
        MCA_ConaxPIN_t stCxPIN;    
        stCxPIN.m_pin[0] = stCxPIN.m_pin[1] = stCxPIN.m_pin[2] = stCxPIN.m_pin[3] = '0';
        s32Ret = MCA_QueryEvent(MCA_EVENT_CHECK_PIN, &stCxPIN);
    }
    else if (MCA_CAS_CRYPTOGUARD == enType)
    {
        if (g_u16CryptoGuardInterfaceVersion >= 0x0112)
        {
            MCA_CryptoGuardPIN_t stCgPIN;    
            stCgPIN.m_pin[0] = stCgPIN.m_pin[1] = stCgPIN.m_pin[2] = stCgPIN.m_pin[3] = '0';
            s32Ret = MCA_QueryEvent(MCA_EVENT_CHECK_PIN, &stCgPIN);
        }
        else
        {
            mca_printf("Card`s interface version(%d.%02d) does not support this feature!\n", \
                            g_u16CryptoGuardInterfaceVersion>>8, \
                            g_u16CryptoGuardInterfaceVersion&0xFF);
        }
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }

    return s32Ret;
}

MCA_S32 mca_app_change_pin(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret = MCA_SUCCESS;

    if (MCA_CAS_CTI == enType)
    {
        MCA_CtiChangePIN_t stCtiPIN;
        stCtiPIN.m_Old[0] = stCtiPIN.m_Old[1] = stCtiPIN.m_Old[2] = stCtiPIN.m_Old[3] = '0';
        stCtiPIN.m_New[0] = stCtiPIN.m_New[1] = stCtiPIN.m_New[2] = stCtiPIN.m_New[3] = '0';
        s32Ret = MCA_QueryEvent(MCA_EVENT_CHANGE_PIN, &stCtiPIN);
    }
    else if (MCA_CAS_CONAX == enType)
    {
        MCA_ConaxChangePIN_t stCxPIN;

        stCxPIN.m_Old[0] = stCxPIN.m_Old[1] = stCxPIN.m_Old[2] = stCxPIN.m_Old[3] = '0';
        stCxPIN.m_New[0] = stCxPIN.m_New[1] = stCxPIN.m_New[2] = stCxPIN.m_New[3] = '0';
        s32Ret = MCA_QueryEvent(MCA_EVENT_CHANGE_PIN, &stCxPIN);
    }
    else if (MCA_CAS_CRYPTOGUARD == enType)
    {
        if (g_u16CryptoGuardInterfaceVersion >= 0x0112)
        {
            MCA_CryptoGuardChangePIN_t stCgPIN;

            stCgPIN.m_Old[0] = stCgPIN.m_Old[1] = stCgPIN.m_Old[2] = stCgPIN.m_Old[3] = '0';
            stCgPIN.m_New[0] = stCgPIN.m_New[1] = stCgPIN.m_New[2] = stCgPIN.m_New[3] = '0';
            s32Ret = MCA_QueryEvent(MCA_EVENT_CHANGE_PIN, &stCgPIN);
        }
        else
        {
            mca_printf("Card`s interface version(%d.%02d) does not support this feature!\n", \
                            g_u16CryptoGuardInterfaceVersion>>8, \
                            g_u16CryptoGuardInterfaceVersion&0xFF);
        }
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }

    return s32Ret;
}

MCA_S32 mca_app_set_maturity_rating(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret = MCA_SUCCESS;

    if (MCA_CAS_CTI == enType)
    {
        MCA_CtiRating_t stCtiRating;

        stCtiRating.m_PIN[0] = stCtiRating.m_PIN[1] = stCtiRating.m_PIN[2] = stCtiRating.m_PIN[3] = '0';
        stCtiRating.m_u8MatRatLevel = 18;
        s32Ret = MCA_QueryEvent(MCA_EVENT_SET_MATURITY_RATING, &stCtiRating);
    }
    else if (MCA_CAS_CONAX == enType)
    {
        MCA_ConaxRating_t stCxRating;

        stCxRating.m_PIN[0] = stCxRating.m_PIN[1] = stCxRating.m_PIN[2] = stCxRating.m_PIN[3] = '0';
        stCxRating.m_u8MatRatLevel = 4;
        s32Ret = MCA_QueryEvent(MCA_EVENT_SET_MATURITY_RATING, &stCxRating);
    }
    else if (MCA_CAS_CRYPTOGUARD == enType)
    {
        if (g_u16CryptoGuardInterfaceVersion >= 0x0112)
        {
            MCA_CryptoGuardRating_t stCgRating;

            stCgRating.m_PIN[0] = stCgRating.m_PIN[1] = stCgRating.m_PIN[2] = stCgRating.m_PIN[3] = '0';
            stCgRating.m_u8MatRatLevel = 5;
            s32Ret = MCA_QueryEvent(MCA_EVENT_SET_MATURITY_RATING, &stCgRating);
        }
        else
        {
            mca_printf("Card`s interface version(%d.%02d) does not support this feature!\n", \
                            g_u16CryptoGuardInterfaceVersion>>8, \
                            g_u16CryptoGuardInterfaceVersion&0xFF);
        }
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }

    return s32Ret;
}

MCA_S32 mca_app_view_parental_channel(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret;

    if (MCA_CAS_CTI == enType)
    {
        MCA_CtiPIN_t stCtiPIN;
        stCtiPIN.m_pin[0] = stCtiPIN.m_pin[1] = stCtiPIN.m_pin[2] = stCtiPIN.m_pin[3] = '0';
        s32Ret = MCA_QueryEvent(MCA_EVENT_VIEW_PARENTAL_CHANNEL, &stCtiPIN);
    }
    else if (MCA_CAS_CONAX == enType)
    {
        MCA_ConaxPIN_t stCxPIN;
        stCxPIN.m_pin[0] = stCxPIN.m_pin[1] = stCxPIN.m_pin[2] = stCxPIN.m_pin[3] = '0';
        s32Ret = MCA_QueryEvent(MCA_EVENT_VIEW_PARENTAL_CHANNEL, &stCxPIN);
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }

    return s32Ret;
}

MCA_S32 mca_app_ippv_buy(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret;

    if (MCA_CAS_CTI == enType)
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }
    else if (MCA_CAS_SOCHUANG == enType)
    {
        MCA_SochuangPIN_t stScPIN;
        stScPIN.m_pin[0] = stScPIN.m_pin[1] = stScPIN.m_pin[2] = stScPIN.m_pin[3] = stScPIN.m_pin[4] = stScPIN.m_pin[5] = '6';
        s32Ret = MCA_QueryEvent(MCA_EVENT_IPPV_BUY, (MCA_VOID *)&stScPIN);
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }

    return s32Ret;
}

MCA_S32 mca_app_switch_cas(MCA_BOOL b8Yes)
{
    return MCA_QueryEvent(MCA_EVENT_SWITCH_CAS, &b8Yes);
}

MCA_S32 mca_app_unpair(MCA_VOID)
{
    MCA_CAS_TYPE_t  enType = MCA_GetCASType();
    MCA_S32         s32Ret;

    if (MCA_CAS_CTI == enType)
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }
    else if ((MCA_CAS_CONAX == enType) || (MCA_CAS_TVCAS == enType))
    {
        s32Ret = MCA_QueryEvent(MCA_EVENT_UNPAIR, NULL);
    }
    else
    {
        mca_printf("\n Not support yet! >>>>>>\n");
        s32Ret = MCA_FAILURE;
    }    

    return s32Ret;
}

