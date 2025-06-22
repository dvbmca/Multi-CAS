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

#ifndef __MCA_CRYPTOGUARD_API_H__
#define __MCA_CRYPTOGUARD_API_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_CG_SUB_COUNT_MAX    64
#define MCA_CG_PIN_LEN          4

typedef struct
{    
    MCA_U32  m_u32CardNumber;
    MCA_U32  m_u32IrdNumber;    /* 00990000001 */
    MCA_U16  m_u16ProtocolVer;
    MCA_U16  m_u16CASystemID;
    MCA_U8   m_u8MaturityLevel;/* For the STB valid setting are from 5 to 20.
                                        13: Parental Guidance
                                        18: Adult
                                        20: Erotic
                               */
}MCA_CryptoGuardSmcInfo_t;

typedef struct
{
    MCA_DATE_t  m_stDateFrom;
    MCA_DATE_t  m_stDateTo;
    MCA_U8      m_au8Field[4];
}MCA_CryptoGuardChannelSub_t;

typedef struct
{
    MCA_DATE_t  m_stDateFrom;
    MCA_DATE_t  m_stDateTo;
    MCA_U32     m_u32ProgNum;
}MCA_CryptoGuardEventSub_t;

typedef struct
{
    MCA_U8                      m_u8ChannelSubCnt;
    MCA_U8                      m_au8Resv[3];
    MCA_CryptoGuardChannelSub_t m_astChannel[MCA_CG_SUB_COUNT_MAX];
    MCA_U8                      m_u8EventSubCnt;
    MCA_U8                      m_au8Resv1[3];
    MCA_CryptoGuardEventSub_t   m_astEvent[MCA_CG_SUB_COUNT_MAX];
}MCA_CryptoGuardSub_t;

typedef struct
{
    MCA_CHAR  m_pin[MCA_CG_PIN_LEN]; /* "0000" */
}MCA_CryptoGuardPIN_t;

typedef struct
{
    MCA_CHAR  m_Old[MCA_CG_PIN_LEN]; /* "0000" */
    MCA_CHAR  m_New[MCA_CG_PIN_LEN];
}MCA_CryptoGuardChangePIN_t;

typedef struct
{
    MCA_CHAR m_PIN[MCA_CX_PIN_LEN]; /* "0000" */
    MCA_U8   m_u8MatRatLevel;       /* For the STB valid setting are from 5 to 20.
                                        13: Parental Guidance
                                        18: Adult
                                        20: Erotic
                                    */
    MCA_U8   m_u8Res;
    MCA_U16  m_u16Res;
}MCA_CryptoGuardRating_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_CRYPTOGUARD_API_H__

