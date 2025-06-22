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

#ifndef __MCA_SUMAVISION_API_H__
#define __MCA_SUMAVISION_API_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_SUMA_CAS_MODULE_VER_LEN_MAX     16
#define MCA_SUMA_CAS_SUPPLIER_LEN_MAX       24
#define MCA_SUMA_WORK_TIME_LEN_MAX          16

#define MCA_SUMA_OPERATOR_NAME_LEN_MAX      21

#define MCA_SUMA_ENTITLE_NAME_LEN_MAX       24


typedef enum
{
    MCA_SumavisionPINLock   = 0,
    MCA_SumavisionPINUnlock = 1,
    MCA_SumavisionPINUnknown= 0xFF,
}MCA_SumavisionPINLockStatus_t;

typedef enum
{
    MCA_SumavisionAreaLock   = 0,
    MCA_SumavisionAreaUnlock = 1,
    MCA_SumavisionAreaUnknown= 0xFF,
}MCA_SumavisionAreaLockStatus_t;

typedef enum
{
    MCA_SumavisionCardMaster = 0,
    MCA_SumavisionCardSlave  = 1,
    MCA_SumavisionCardUnknown= 0xFF,
}MCA_SumavisionCardType_t;

typedef struct
{
    MCA_U32     m_u32CardID;
    MCA_CHAR    m_szCASMouleVer[MCA_SUMA_CAS_MODULE_VER_LEN_MAX];
    MCA_CHAR    m_szCASSupplier[MCA_SUMA_CAS_SUPPLIER_LEN_MAX];
    MCA_U32     m_u32COSVer;

    MCA_U32     m_u32AreaCode;
    MCA_SumavisionAreaLockStatus_t  m_enAreaStatus:8;
    MCA_U8      m_au8Reserve1[3];
    MCA_DATE_t  m_stAreaTime;

    MCA_SumavisionCardType_t        m_enCardType:8;
    MCA_U8      m_au8Reserve2[3];
    MCA_U32     m_u32MasterCardID;
    
    MCA_SumavisionPINLockStatus_t   m_enPINStatus:8;
    MCA_U8      m_u8Rating;
    MCA_U8      m_u16Reserved;
    MCA_U8      m_u8Reserve3;
    MCA_CHAR    m_szWorkTime[MCA_SUMA_WORK_TIME_LEN_MAX];
}MCA_SumavisionCardInfo_t;

typedef struct
{
    MCA_CHAR    m_szName[MCA_SUMA_ENTITLE_NAME_LEN_MAX];
    MCA_DATE_t  m_stEntitleTime;
    MCA_DATE_t  m_stStartTime;
    MCA_DATE_t  m_stEndTime;
}MCA_SumavisionSubscription_t;

typedef struct
{
    MCA_U16                         m_u16ID;
    MCA_CHAR                        m_szName[MCA_SUMA_OPERATOR_NAME_LEN_MAX];
    MCA_U8                          m_u8Count;
    MCA_SumavisionSubscription_t   *m_pstEntitle;
}MCA_SumavisionOperatorInfo_t;

typedef struct
{
    MCA_U8                          m_u8Count;
    MCA_U8                          m_au8Reserve[3];
    MCA_SumavisionOperatorInfo_t    m_astInfo[5];
}MCA_SumavisionOperator_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_SUMAVISION_API_H__

