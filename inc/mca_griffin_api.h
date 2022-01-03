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

#ifndef __MCA_GRIFFIN_API_H__
#define __MCA_GRIFFIN_API_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_GF_SUB_COUNT_MAX    40
#define MCA_GF_PIN_LEN          4

typedef struct
{
    MCA_CHAR m_szSerialNumber[20];
    MCA_U16  m_u16CASID;
    /*
        m_u8MatRatLevel
        0x00 : General Audience
        0x01 : Parental Guidance
        0x02 : Adult
        0x03 : Erotic
    */
    MCA_U8   m_u8MaturityRating;
    MCA_U8   m_u8Res;
}MCA_GriffinSmcInfo_t;

typedef struct
{
    MCA_U16 m_u16Sum;
    MCA_U16 m_u16Res;
    MCA_U16 m_au16Entries[MCA_GF_SUB_COUNT_MAX]; /* Display as hexadecimal format. for example: 0xABCD */
}MCA_GriffinSubscription_t;

typedef struct
{
    MCA_CHAR  m_Old[MCA_GF_PIN_LEN]; /* "0000" */
    MCA_CHAR  m_New[MCA_GF_PIN_LEN];
}MCA_GriffinChangePIN_t;

typedef struct
{
    MCA_CHAR  m_pin[MCA_GF_PIN_LEN]; /* "0000" */
}MCA_GriffinPIN_t;

typedef struct
{
    MCA_CHAR m_PIN[MCA_GF_PIN_LEN]; /* "0000" */
    MCA_U8   m_u8MaturityRating;    /* Valid Values: 0x00/0x01/0x02/0x03 */
    MCA_U8   m_u8Res;
    MCA_U16  m_u16Res;
}MCA_GriffinRating_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_GRIFFIN_API_H__

