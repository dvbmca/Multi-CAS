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

#ifndef __MCA_BASIC_API_H__
#define __MCA_BASIC_API_H__

#include "mca_type.h"
#include "mca_message_code.h"
#include "mca_error_code.h"

#include "mca_cti_api.h"
#include "mca_conax_api.h"
#include "mca_griffin_api.h"
#include "mca_cryptoguard_api.h"
#include "mca_sochuang_api.h"

#ifdef __cplusplus
extern "C " {
#endif

typedef enum
{
    MCA_TRACE_NONE      = 0,
    MCA_TRACE_BASIC     = 1,
    MCA_TRACE_DEBUG     = 3,
}MCA_TraceLevel_t;

typedef enum
{
    MCA_CAS_UNKNOWN = 0,
    MCA_CAS_CTI,
    MCA_CAS_CONAX,
    MCA_CAS_GRIFFIN,
    MCA_CAS_CRYPTOGUARD,
    MCA_CAS_SOCHUANG,
    MCA_CAS_DVCRYPT,
    MCA_CAS_CRYPTON,
    MCA_CAS_ABV,
    MCA_CAS_TOPREAL,
    MCA_CAS_GOSPELL,
}MCA_CAS_TYPE_t;

typedef struct
{
    MCA_TraceLevel_t    m_enLogLevel;
    MCA_CAS_TYPE_t      m_enCASType;
}MCA_Param_t;

typedef enum
{
    MCA_EVENT_MESSAGE_CODE = 0,
    MCA_EVENT_SMC_INFO,
    MCA_EVENT_SUB_INFO,
    MCA_EVENT_CTI_SUB_DETAIL_INFO,
    MCA_EVENT_CTI_PPV_LIST,
    MCA_EVENT_CTI_IPPV_LIST,
    MCA_EVENT_CHANGE_PIN,
    MCA_EVENT_CHECK_PIN,
    MCA_EVENT_SET_MATURITY_RATING,
    MCA_EVENT_VIEW_PARENTAL_CHANNEL,
    MCA_EVENT_CONAX_EVENT_STATUS,
    MCA_EVENT_CONAX_PURSE_STATUS,
    MCA_EVENT_CONAX_DEBIT_STATUS,
    MCA_EVENT_CONAX_CREDIT_STATUS,
    MCA_EVENT_IPPV,
    MCA_EVENT_IPPV_BUY,
    MCA_EVENT_UNKNOWN
}MCA_EVENT_TYPE_t;

typedef void (*MCA_EVENT_CALLBACK)(MCA_CAS_TYPE_t enCASType, MCA_EVENT_TYPE_t enEvtType, MCA_VOID *pData);

MCA_S32   MCA_Init(const MCA_Param_t *const pstData);
MCA_CHAR* MCA_Version(MCA_VOID);
MCA_S32   MCA_PostPMT(MCA_DMX_ID_t id, MCA_U8 *Section, MCA_U16 len);
MCA_S32   MCA_PostCAT(MCA_DMX_ID_t id, MCA_U8 *Section, MCA_U16 len);

MCA_S32   MCA_RegisterEvent(MCA_EVENT_TYPE_t type, MCA_EVENT_CALLBACK cb);
MCA_S32   MCA_QueryEvent(MCA_EVENT_TYPE_t type, MCA_VOID *pData);

MCA_CAS_TYPE_t MCA_GetCASType(MCA_VOID);

#ifdef __cplusplus
}
#endif

#endif    //__MCA_BASIC_API_H__

