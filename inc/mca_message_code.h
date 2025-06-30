/*****************************************************************************
* Copyright 2025 MCA All rights reserved.
*
* Athour:   dvbmca@protonmail.com
*
* History:  
*
*
* This software is subject to updates, revisions,
* and extensions by MCA. Use of this software shall be permitted only
* pursuant to the terms and conditions of the license agreement
* between the licensee and MCA.
*****************************************************************************/

#ifndef __MCA_MESSAGE_CODE_H__
#define __MCA_MESSAGE_CODE_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef enum{
    MCA_M00_FTA_SERVICE,                /* M00 FTA Service. */
    MCA_M01_NO_SMC,                     /* M01 Please insert smart card. */
    MCA_M02_UNKNOWN_SMC,                /* M02 Unknown smart card. */
    MCA_M03_CHECKING_SMC,               /* M03 Checking smart card. */
    MCA_M04_NO_SERVICE,                 /* M04 No Service Avaliable. */
    MCA_M05_SLAVE_CARD_VALID,           /* M05 Slave Card Valid. */
    MCA_M06_SMC_READY,                  /* M06 Smart card is ready. */
    MCA_M07_SLAVE_CARD_INVALID,         /* M07 Slave Card Invalid. */
    MCA_M08_MASTER_CARD_INVALID,        /* M08 Master Card Invalid. */
    MCA_M09,
    MCA_M10_SCRAMBLED_SERVICE,          /* M10 Service Is Currently Scrambled. */
    MCA_M11_MISMATCHING_REGION,         /* M11 Not Allowed In This Region. */
    MCA_M12_SMC_PAIRING_FAIL,           /* M12 Smart card is not pairing to this STB. */
    MCA_M13_LOW_PARENTAL_CONTROL_LEVEL, /* M13 Parental Control Lock. */
    MCA_M14_CW_ERROR,                   /* M14 CW Error. */
    MCA_M15_NO_AUTHORIZATION,           /* M15 No Authorization. */
    MCA_M16_DESCRAMBLING,               /* M16 Descrambling works well. */
    MCA_M17_SLAVE_CARD_EXPIRE,          /* M17 Slave Card Expire. */
    MCA_M18,                            /* M18 User View Query */
    MCA_M19,                            /* M19 Regional Control. */
    MCA_M20_IPPV_SERVICE,               /* M20 IPPV Service. */
    MCA_M21,                            /* M21 Program Authorization is Paused. */
    MCA_M22,                            /* M22 Smart card is not active. */
    MCA_M23,
    MCA_M24,
    MCA_M25,
    MCA_M26,
    MCA_M27,
    MCA_M28,
    MCA_M29,
    MCA_M30,
    MCA_M31,
    MCA_M32,
    MCA_M33,
    MCA_M34,
    MCA_M35,
    MCA_M36,
    MCA_M37,
    MCA_M38,
    MCA_M39_SWITCH_CAS,                 /* M39 Switch CAS */
    MCA_M40,
    MCA_M41,
    MCA_M42,
    MCA_M43,
    MCA_M44,
    MCA_M45,
    MCA_M46,
    MCA_M47,
    MCA_M48,
    MCA_M49,
    MCA_M50_IPPV_ON_DEMAND_SUCCESS,     /* M50 IPPV on demand Success. */
    MCA_M51_IPPV_PASSWORD_Error,        /* M51 IPPV Password Error. */
    MCA_M52_IPPV_PROGRAM_IS_OVERDUE,    /* M52 IPPV program is Overdue. */
    MCA_M53_IPPV_BALANCE_IS_NOT_ENOUGH, /* M53 IPPV balance is not enough. */
    MCA_M54_IPPV_MODE_IS_NOT_SUPPORTED, /* M54 IPPV Mode is not supported. */
    MCA_M55_SET_IPPV_ON_DEMAND_FAIL,    /* M55 Set IPPV on demand Fail. */
    MCA_M56_IPPV_END_SUCCESS,           /* M56 IPPV End Success. */
    MCA_M57_IPPV_END_FAIL,              /* M57 IPPV End Fail. */
    MCA_MXX_UNKNOWN
}MCA_MESSAGE_CODE_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_MESSAGE_CODE_H__

