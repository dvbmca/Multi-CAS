#include "mca_adaptation_files.h"

/* Refer to MCA_CAS_TYPE_t */
const MCA_CHAR aszCASName[][16] = {
    "Unknown CAS",
    "CTI",
    "Conax",
    "Griffin",
    "CryptoGuard",
    "SoChuang",
    "DVCrypt",
    "Crypton",
    "ABV",
    "Topreal"
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
    "M30 ", "M31 ", "M32 ", "M33 ", "M34 ", "M35 ", "M36 ", "M37 ", "M38 ", "M39 ",
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

MCA_S32 mca_app_common_init(MCA_VOID)
{
    MCA_S32 s32Ret;

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
    MCA_S32         s32Ret;

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
    MCA_S32         s32Ret;

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
    MCA_S32         s32Ret;

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


