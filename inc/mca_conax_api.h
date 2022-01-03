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

#ifndef __MCA_CONAX_API_H__
#define __MCA_CONAX_API_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_CX_CARD_NUMBER_LEN_MAX      22
#define MCA_CX_SUB_COUNT_MAX            40
#define MCA_CX_SUB_LABLE_LEN_MAX        42
#define MCA_CX_PIN_LEN                  4
#define MCA_CX_EVENT_COUNT_MAX          100
#define MCA_CX_EVENT_LABLE_LEN_MAX      32
#define MCA_CX_PURSE_COUNT_MAX          100
#define MCA_CX_PURSE_LABLE_LEN_MAX      30
#define MCA_CX_DEBIT_COUNT_MAX          100
#define MCA_CX_DEBIT_LABLE_LEN_MAX      32
#define MCA_CX_CREDIT_COUNT_MAX         100
#define MCA_CX_CREDIT_LABLE_LEN_MAX     32

typedef struct
{
    MCA_U8      m_u8InterfaceVersion;
    MCA_CHAR    m_szCardNumber[MCA_CX_CARD_NUMBER_LEN_MAX];
    MCA_U8      m_u8NumberOfSessions;
    MCA_U16     m_u16CountryIndicatorValue;
    MCA_U16     m_u16CASysID;
    /*
        m_u8MatRatLevel
        0x01 : G - For the family
        0x02 : PG - With parental guidance
        0x04 : A - Over 18
        0x08 : X - Erotic
        Other values : Reserved for future use
    */
    MCA_U8      m_u8MatRatLevel;
    MCA_U8      m_u8Res;
    MCA_U16     m_u16Res;
}MCA_ConaxSmcInfo_t;

typedef struct
{
    MCA_U16     m_u16SubscriptionRef;
    MCA_CHAR    m_szLable[MCA_CX_SUB_LABLE_LEN_MAX];
    MCA_DATE_t  m_astStart[2];
    MCA_DATE_t  m_astEnd[2];
    MCA_U32     m_au32Entitlement[2];
}MCA_ConaxSubscriptionNode_t;

typedef struct
{
    MCA_U16                     m_u16Count;
    MCA_U16                     m_u16Res;
    MCA_ConaxSubscriptionNode_t m_astEntries[MCA_CX_SUB_COUNT_MAX];
}MCA_ConaxSubscription_t;

typedef struct
{
    MCA_CHAR  m_Old[MCA_CX_PIN_LEN]; /* "0000" */
    MCA_CHAR  m_New[MCA_CX_PIN_LEN];
}MCA_ConaxChangePIN_t;

typedef struct
{
    MCA_CHAR  m_pin[MCA_CX_PIN_LEN]; /* "0000" */
}MCA_ConaxPIN_t;

typedef struct
{
    MCA_CHAR m_PIN[MCA_CX_PIN_LEN]; /* "0000" */
    MCA_U8   m_u8MatRatLevel;       /* Valid Values: 0x01/0x02/0x04/0x08 */
    MCA_U8   m_u8Res;
    MCA_U16  m_u16Res;
}MCA_ConaxRating_t;

typedef struct
{
    MCA_U32     m_u32EventTag;
    MCA_CHAR    m_szLable[MCA_CX_EVENT_LABLE_LEN_MAX];
    MCA_DATE_t  m_stAuthorisationStart;
    MCA_DATE_t  m_stAuthorisationEnd;
    MCA_U8      m_u8AuthType;   /*  
                                    0x00 : m_u16MinutesLeft
                                    0x02 : m_u16CreditsLeft
                                */
    MCA_U8      m_au8Res[3];
    MCA_U16     m_u16MinutesLeft;
    MCA_U16     m_u16CreditsLeft;
}MCA_ConaxEventNode_t;

typedef struct
{
    MCA_U16                 m_u16Count;
    MCA_U16                 m_u16Res;
    MCA_ConaxEventNode_t    m_astEvent[MCA_CX_EVENT_COUNT_MAX];
}MCA_ConaxEvent_t;

typedef struct
{
    MCA_U16     m_u16PurseRef;
    MCA_CHAR    m_szLable[MCA_CX_PURSE_LABLE_LEN_MAX];
    MCA_U32     m_u32Balance;
}MCA_ConaxPurseNode_t;

typedef struct
{
    MCA_U16                 m_u16Count;
    MCA_U16                 m_u16Res;
    MCA_ConaxPurseNode_t    m_astPurse[MCA_CX_PURSE_COUNT_MAX];
}MCA_ConaxPurse_t;

typedef struct
{
    MCA_U32     m_u32EventTag;
    MCA_U32     m_u32ConsumedTokens;
    MCA_DATE_t  m_stLastAccessTime;
    MCA_CHAR    m_szLable[MCA_CX_DEBIT_LABLE_LEN_MAX];
}MCA_ConaxDebitNode_t;

typedef struct
{
    MCA_U16                 m_u16Count;
    MCA_U16                 m_u16Res;
    MCA_ConaxDebitNode_t    m_astDebit[MCA_CX_DEBIT_COUNT_MAX];
}MCA_ConaxDebit_t;

typedef struct
{
    MCA_U32     m_u32CreditTokens;
    MCA_CHAR    m_szLable[MCA_CX_CREDIT_LABLE_LEN_MAX];
}MCA_ConaxCreditNode_t;

typedef struct
{
    MCA_U16                 m_u16Count;
    MCA_U16                 m_u16Res;
    MCA_ConaxCreditNode_t   m_astCredit[MCA_CX_CREDIT_COUNT_MAX];
}MCA_ConaxCredit_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_CONAX_API_H__

