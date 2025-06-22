#include <stdio.h>
#include "mca_adaptation_files.h"
#include "mca_app_common.h"

static void mca_cti_app_message_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_MESSAGE_CODE_t enCode;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    mca_memcpy(&enCode, pData, sizeof(enCode));

    enCode = (enCode>=MCA_MXX_UNKNOWN) ? MCA_MXX_UNKNOWN : enCode;

    MCA_APP_DBG("%s: Event:%d, Message:%s\n", aszCASName[enCASType], enEvtType, aszCASMessage[enCode]);
}

static void mca_cti_app_smc_info_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{    
    MCA_CtiSmcInfo_t *pstCtiInfo = (MCA_CtiSmcInfo_t *)pData;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstCtiInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("%s smart card information: >>\n", aszCASName[enCASType]);
    mca_printf(">> Card Number       : %s\n", pstCtiInfo->szCardNumber);
    
    if (0 == pstCtiInfo->scstatus)      mca_printf(">> Status            : Correct (%d).\n", pstCtiInfo->scstatus);
    else if (1 == pstCtiInfo->scstatus) mca_printf(">> Status            : Without smart card (%d).\n", pstCtiInfo->scstatus);
    else if (2 == pstCtiInfo->scstatus) mca_printf(">> Status            : Can`t read smart card (%d).\n", pstCtiInfo->scstatus);
    else if (5 == pstCtiInfo->scstatus) mca_printf(">> Status            : Smart card communication error (%d).\n", pstCtiInfo->scstatus);
    else                                mca_printf(">> Status            : Unknown (%d).\n", pstCtiInfo->scstatus);
    
    if (0 == pstCtiInfo->usedstatus)        mca_printf(">> Used status       : Active (%d).\n", pstCtiInfo->usedstatus);
    else if (1 == pstCtiInfo->usedstatus)   mca_printf(">> Used status       : Suspend (%d).\n", pstCtiInfo->usedstatus);
    else if (2 == pstCtiInfo->usedstatus)   mca_printf(">> Used status       : Inactive (%d).\n", pstCtiInfo->usedstatus);
    else                                    mca_printf(">> Used status       : Unknown (%d).\n", pstCtiInfo->usedstatus);
    
    mca_printf(">> System ID         : 0x%02x%02x\n", pstCtiInfo->systemid[0], pstCtiInfo->systemid[1]);
    mca_printf(">> ECM Sub System ID : 0x%02x%02x\n", pstCtiInfo->ecmsubsysid[0], pstCtiInfo->ecmsubsysid[1]);
    mca_printf(">> EMM Sub System ID : 0x%02x%02x\n", pstCtiInfo->emmsubsysid[0], pstCtiInfo->emmsubsysid[1]);
    mca_printf(">> COS Version       : %s\n", pstCtiInfo->cosversion);
    mca_printf(">> Card label        : %s\n", pstCtiInfo->cardlabel);
    mca_printf(">> Issue - Expire:   : %s - %s\n", pstCtiInfo->issuetime, pstCtiInfo->expiretime);
    mca_printf(">> Parental Rating   : %d\n", pstCtiInfo->parentalrating);
    mca_printf(">> Group Control     : %d\n", pstCtiInfo->groupcontrol);
    mca_printf(">> Anti Move Control : 0x%02x%02x%02x\n", pstCtiInfo->antimovectrl[0], pstCtiInfo->antimovectrl[1], pstCtiInfo->antimovectrl[2]);
    
    if (0 == pstCtiInfo->smartcard_type)        mca_printf(">> Card Type         : Mother (%d).\n", pstCtiInfo->smartcard_type);
    else if (1 == pstCtiInfo->smartcard_type)   mca_printf(">> Card Type         : Son (%d).\n", pstCtiInfo->smartcard_type);
    else if (2 == pstCtiInfo->smartcard_type)   mca_printf(">> Card Type         : Error (%d)\n", pstCtiInfo->smartcard_type);
    else                                        mca_printf(">> Card Type         : Unknown (%d).\n", pstCtiInfo->smartcard_type);
    
    if (0 == pstCtiInfo->smartcard_matching)        mca_printf(">> Card Match        : No (%d).\n", pstCtiInfo->smartcard_matching);
    else if (1 == pstCtiInfo->smartcard_matching)   mca_printf(">> Card Match        : Yes (%d).\n", pstCtiInfo->smartcard_matching);
    else if (5 == pstCtiInfo->smartcard_matching)   mca_printf(">> Card Match        : None (%d).\n", pstCtiInfo->smartcard_matching);
    else                                            mca_printf(">> Card Match        : Unknown (%d).\n", pstCtiInfo->smartcard_matching);
    
    if (2 == pstCtiInfo->stb_ird_matching)      mca_printf(">> IRD Match         : No (%d).\n", pstCtiInfo->stb_ird_matching);
    else if (3 == pstCtiInfo->stb_ird_matching) mca_printf(">> IRD Match         : Yes (%d).\n", pstCtiInfo->stb_ird_matching);
    else                                        mca_printf(">> IRD Match         : Unknown (%d).\n", pstCtiInfo->stb_ird_matching);
    
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_cti_app_sub_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    MCA_U32              i;
    MCA_CtiPPidNotify_t *pstCtiSubInfo = (MCA_CtiPPidNotify_t *)pData;
    MCA_CtiPPidInfo_t   *pstCtiNode    = NULL;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstCtiSubInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }
    
    mca_printf("\n%s PPID information: >>\n", aszCASName[enCASType]);
    for (i = 0; i < pstCtiSubInfo->count; i++)
    {
        pstCtiNode = &(pstCtiSubInfo->info[i]);
        mca_printf(">> PPID %d: ID: 0x%02x%02x%02x, Label: %s, Status: %s(%d)\n",  \
                            i,  \
                            pstCtiNode->id[0], pstCtiNode->id[1], pstCtiNode->id[2],    \
                            pstCtiNode->label,
                            (0==pstCtiNode->usedstatus)?"Active":"Pause", pstCtiNode->usedstatus);
    }
    mca_printf("\n");    
}
    
static void mca_cti_app_sub_detail_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{    
    MCA_CtiPPidInfo_t  *pstCtiPPIDInfo = (MCA_CtiPPidInfo_t *)pData;
    MCA_U8             *pu8Right;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }
    if (NULL == pstCtiPPIDInfo)
    {
        MCA_APP_DBG("Parameters is NULL.\n");
        return;
    }

    mca_printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    mca_printf("CTI PPID Detail information:        >>\n");
    if (MCA_SUCCESS == pstCtiPPIDInfo->status_code)             mca_printf(">> Status Code      : Success (0x%x).\n",       pstCtiPPIDInfo->status_code);
    else if (MCA_E02_WRONG_PIN == pstCtiPPIDInfo->status_code)  mca_printf(">> Status Code      : Wrong PID (0x%x).\n",     pstCtiPPIDInfo->status_code);
    else if (MCA_E04_WRONG_PPID == pstCtiPPIDInfo->status_code) mca_printf(">> Status Code      : Wrong PPID (0x%x).\n",    pstCtiPPIDInfo->status_code);
    else                                                        mca_printf(">> Status Code      : Unknown Error (0x%x).\n", pstCtiPPIDInfo->status_code);
    
    mca_printf(">> ID               : 0x%02x%02x%02x\n", pstCtiPPIDInfo->id[0], pstCtiPPIDInfo->id[1], pstCtiPPIDInfo->id[2]);
    mca_printf(">> Label            : %s\n", pstCtiPPIDInfo->label);
    mca_printf(">> Status           : %s (%d)\n", (pstCtiPPIDInfo->usedstatus==0)?"Active":"Pause", pstCtiPPIDInfo->usedstatus);
    mca_printf(">> Credit Deposit   : %d Fen\n", (pstCtiPPIDInfo->credit_deposit[0]<<24) + \
                                                (pstCtiPPIDInfo->credit_deposit[1]<<16) + \
                                                (pstCtiPPIDInfo->credit_deposit[2]<<8) + \
                                                (pstCtiPPIDInfo->credit_deposit[3]));
    pu8Right = pstCtiPPIDInfo->prog_right;
    mca_printf(">> Program Right    : \n");
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[0], pu8Right[1], pu8Right[2], pu8Right[3]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[4], pu8Right[5], pu8Right[6], pu8Right[7]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[8], pu8Right[9], pu8Right[10], pu8Right[11]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[12], pu8Right[13], pu8Right[14], pu8Right[15]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[16], pu8Right[17], pu8Right[18], pu8Right[19]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[20], pu8Right[21], pu8Right[22], pu8Right[23]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[24], pu8Right[25], pu8Right[26], pu8Right[27]);
    mca_printf(">>                    0x %02x %02x %02x %02x\n", pu8Right[28], pu8Right[29], pu8Right[30], pu8Right[31]);
    mca_printf(">> Start Date       : %s\n", pstCtiPPIDInfo->prog_right_start_date);
    mca_printf(">> End   Date       : %s\n", pstCtiPPIDInfo->prog_right_end_date);
    mca_printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
}

static void mca_cti_app_check_pin_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    char     szError[20];
    MCA_S32 *ps32Status;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    ps32Status = (MCA_S32 *)pData;
    switch (*ps32Status)
    {
        case MCA_SUCCESS:
            snprintf(szError, 20, "%s", "Success");
            break;
        case MCA_E02_WRONG_PIN:
            snprintf(szError, 20, "%s", "Wrong PIN");
            break;
        default:
            snprintf(szError, 20, "%s", "Fail");
            break;
    }

    MCA_APP_DBG("%s check PIN reply: %s>>\n", aszCASName[enCASType], szError);
}

static void mca_cti_app_change_pin_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    char     szError[20];
    MCA_S32 *ps32Status;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    ps32Status = (MCA_S32 *)pData;
    switch (*ps32Status)
    {
        case MCA_SUCCESS:
            snprintf(szError, 20, "%s", "Success");
            break;
        case MCA_E02_WRONG_PIN:
            snprintf(szError, 20, "%s", "Wrong PIN");
            break;
        default:
            snprintf(szError, 20, "%s", "Fail");
            break;
    }

    MCA_APP_DBG("%s Change PIN reply: %s>>\n", aszCASName[enCASType], szError);
}

static void mca_cti_app_set_maturity_rating_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    char     szError[20];
    MCA_S32 *ps32Status;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    ps32Status = (MCA_S32 *)pData;
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

static void mca_cti_app_view_parental_channel_callback(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData)
{
    char     szError[20];
    MCA_S32 *ps32Status;

    if (MCA_CAS_CTI != enCASType)
    {
        MCA_APP_DBG("CAS Type does not match!");
        return ;
    }

    ps32Status = (MCA_S32 *)pData;
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

    MCA_APP_DBG("%s View Parental Channel Reply: %s>>\n", aszCASName[enCASType], szError);
}

MCA_S32 mca_cti_app_init(MCA_VOID)
{
    if (MCA_CAS_CTI != MCA_GetCASType())
    {
        MCA_APP_DBG("CAS Type does not match!");
        return MCA_FAILURE;
    }

    MCA_RegisterEvent(MCA_EVENT_MESSAGE_CODE,           mca_cti_app_message_callback);
    MCA_RegisterEvent(MCA_EVENT_SMC_INFO,               mca_cti_app_smc_info_callback);
    MCA_RegisterEvent(MCA_EVENT_SUB_INFO,               mca_cti_app_sub_callback);
    MCA_RegisterEvent(MCA_EVENT_CTI_SUB_DETAIL_INFO,    mca_cti_app_sub_detail_callback);
    MCA_RegisterEvent(MCA_EVENT_CHECK_PIN,              mca_cti_app_check_pin_callback);
    MCA_RegisterEvent(MCA_EVENT_CHANGE_PIN,             mca_cti_app_change_pin_callback);
    MCA_RegisterEvent(MCA_EVENT_SET_MATURITY_RATING,    mca_cti_app_set_maturity_rating_callback);
    MCA_RegisterEvent(MCA_EVENT_VIEW_PARENTAL_CHANNEL,  mca_cti_app_view_parental_channel_callback);

    return MCA_SUCCESS;
}

