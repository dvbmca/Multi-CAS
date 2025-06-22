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

#ifndef __MCA_SOCHUANG_API_H__
#define __MCA_SOCHUANG_API_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_SC_CARD_NUMBER_LEN_MAX  17

#define MCA_SC_SUB_COUNT_MAX        20

#define MCA_SC_PIN_LEN              6

typedef struct
{
    MCA_CHAR    m_szCardNumber[MCA_SC_CARD_NUMBER_LEN_MAX];
    MCA_U8      m_u8RegionID;
}MCA_SochuangBaseInfo_t;

typedef struct
{
    MCA_U16     m_u16ID;
    MCA_U8      m_au8Res[2];
    MCA_DATE_t  m_astStart;
    MCA_DATE_t  m_astEnd;
}MCA_SochuangSubscriptionNode_t;

typedef struct
{
    MCA_U8                          m_u8Count;
    MCA_U8                          m_au8Res[3];
    MCA_SochuangSubscriptionNode_t  m_astEntries[MCA_SC_SUB_COUNT_MAX];
}MCA_SochuangSubscription_t;

typedef struct
{
    MCA_CHAR  m_pin[MCA_SC_PIN_LEN]; /* "666666" */
}MCA_SochuangPIN_t;

typedef struct
{
    MCA_U16 m_u16ProgramNum;
    MCA_U16 m_u16Res;
    MCA_U32 m_u32Price;
}MCA_SochuangIPPV_t;

typedef struct
{
    MCA_U8  m_u8Position; /* 0:Top; 1:Bottom */
    MCA_U8  m_u8FontType;
    MCA_U8  m_u8FontSize;
    MCA_U8  m_u8Times;
    MCA_U32 m_u32BackgroundColor;
    MCA_U32 m_u32FontColor;
    MCA_U32 m_u32ContentLen;
    MCA_U8  m_au8Content[256];
}MCA_SochuangOSD_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_SOCHUANG_API_H__

