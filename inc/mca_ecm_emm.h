/*****************************************************************************
* Copyright 2023 MCA All rights reserved.
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

#ifndef __MCA_ECM_EMM_H__
#define __MCA_ECM_EMM_H__

#include "mca_type.h"

#ifdef __cplusplus
extern "C " {
#endif

typedef struct
{
    MCA_DMX_ID_t m_enDmx;
    MCA_U16      m_pid;
    MCA_U32      m_u32Counter;
}MCA_EcmEmm_Info_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_ECM_EMM_H__

