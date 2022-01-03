/*****************************************************************************
* Copyright 2021 MCA All rights reserved.
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

#ifndef __MCA_CTI_API_H__
#define __MCA_CTI_API_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_CTI_PIN_LEN     4

typedef struct
{
    MCA_U8 id_len;
    MCA_U8 stb_id[16];
    MCA_U8 sc_id[16];
}MCA_CtiSecurityInfo_t;

typedef struct
{
    MCA_CHAR verify_start_time[8];
    MCA_U8 work_period[2];
    MCA_U8 mother_ua[5];
}MCA_CtiChildCardInfo_t;

typedef struct
{
    MCA_CHAR szCardNumber[24];
    MCA_U8 scstatus;    /*  
                            0: correct smart card status.
                            1: without smart card.
                            2: can`t read smart card.
                            5: smart card communication error.
                        */
    MCA_U8 usedstatus;  /* 0:Active; 1:Suspend; 2:Inactive */
    MCA_U8 systemid[2];
    MCA_U8 ecmsubsysid[2];
    MCA_U8 emmsubsysid[2];
    MCA_CHAR cosversion[9];
    MCA_CHAR cardlabel[17];
    MCA_CHAR issuetime[17];
    MCA_CHAR expiretime[17];
    MCA_U8 parentalrating;
    MCA_U8 groupcontrol;
    MCA_U8 antimovectrl[3];
    MCA_U8 smartcard_type;
    MCA_U8 smartcard_matching;
    MCA_U8 stb_ird_matching;
    MCA_CtiChildCardInfo_t childCard;
    MCA_CtiSecurityInfo_t chip;
}MCA_CtiSmcInfo_t;

typedef struct
{
    MCA_U32 status_code;
    MCA_U8 id[3];
    MCA_CHAR label[17];
    MCA_U8 usedstatus;
    MCA_U8 credit_deposit[4];
    MCA_CHAR credit_date[11];
    MCA_U8 prog_right[32];
    MCA_CHAR prog_right_start_date[11];
    MCA_CHAR prog_right_end_date[11];
}MCA_CtiPPidInfo_t;

typedef struct
{
    MCA_U8 count;
    MCA_CtiPPidInfo_t info[8];
}MCA_CtiPPidNotify_t;

typedef struct
{
    MCA_CHAR  m_pin[MCA_CTI_PIN_LEN]; /* "0000" */
    MCA_U8    m_au8PPID_ID[3];
}MCA_CtiPPidDetailRequest_t;

typedef struct
{
    MCA_U8    m_au8PPID_ID[3];
}MCA_CtiPPVRequest_t;

typedef struct
{
    MCA_U8 start_number[4];
    MCA_U8 end_number[4];
}MCA_CtiPPVNumber_t;

typedef struct
{
    MCA_U8 page;
    MCA_U8 prog_count;
    MCA_CtiPPVNumber_t ppv[32];
}MCA_CtiPPVListNotify_t;

typedef struct
{
    MCA_U8    m_au8PPID_ID[3];
}MCA_CtiIPPVRequest_t;

typedef struct
{
    MCA_CHAR prog_number[4];
}MCA_CtiIPPVNumber_t;

typedef struct
{
    MCA_U8 page;
    MCA_U8 prog_count;
    MCA_CtiIPPVNumber_t ippv[32];
}MCA_CtiIPPVListNotify_t;

typedef struct
{
    MCA_CHAR  m_Old[MCA_CTI_PIN_LEN]; /* "0000" */
    MCA_CHAR  m_New[MCA_CTI_PIN_LEN];
}MCA_CtiChangePIN_t;

typedef struct
{
    MCA_CHAR  m_pin[MCA_CTI_PIN_LEN]; /* "0000" */
}MCA_CtiPIN_t;

typedef struct
{
    MCA_CHAR m_PIN[MCA_CTI_PIN_LEN]; /* "0000" */
    MCA_U8   m_u8MatRatLevel;       /* Valid Values: 4 ~ 18 */
}MCA_CtiRating_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_CTI_API_H__
