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

#ifndef __MCA_TS_H__
#define __MCA_TS_H__

#include "mca_type.h"

#ifdef __cplusplus
extern "C " {
#endif

typedef enum
{
    MCA_TYPE_FILE,
    MCA_TYPE_MEMORY,
}MCA_TYPE_t;    /* Input/Output type: File, Memory and so on */

typedef struct
{
    MCA_U16  m_PID;
    MCA_U8   m_u8Version:5;
    MCA_U8   m_u8Reserved:3;
    MCA_U8   m_u8Reserved2;

    MCA_U32  m_u32FileID;   /* Should be greater than 0. */

    MCA_CHAR m_szInput[100];
    MCA_CHAR m_szOutput[100];
}MCA_PackageTs_t;

typedef struct
{
    MCA_U16  m_PID;
    MCA_U8   m_u8CurVersion:5;
    MCA_U8   m_u8Reserved:3;
    MCA_U8   m_u8Reserved2;

    MCA_U32  m_u32FileID;   /* Should be greater than 0. */
}MCA_DownloadTs_t;

typedef enum
{
    MCA_TS_EVENT_PACKAGE    = 0,
    MCA_TS_EVENT_DOWNLOAD   = 1,
}MCA_TS_EVENT_t;

typedef struct
{    
    MCA_U32 m_u32FileID;
    MCA_S32 m_s32Status; /* mca_error_code.h */
    MCA_U16 m_PID;
    MCA_U8  m_u8Percent;
    MCA_U8  m_u8Reserved;
}MCA_PackageCb_t;

typedef struct
{    
    MCA_U32 m_u32FileID;
    MCA_S32 m_s32Status;
    MCA_U8  m_u8Percent;
    MCA_U8  m_u8NewVersion;
    MCA_U16 m_u16Reserved;
    MCA_U8 *m_pu8Mem;
    MCA_U32 m_u32MemSize;
}MCA_DownloadCb_t;

typedef void (*MCA_TS_EVENT_CB)(MCA_TS_EVENT_t enEvtType, MCA_VOID *pParam);

/*************************************************************************************
    Function:
    Params:     
    Return:     
    Desc:
*************************************************************************************/
MCA_S32 MCA_TS_RegisterEvent(MCA_TS_EVENT_t type, MCA_TS_EVENT_CB cb);

/*************************************************************************************
    Function:
    Params:     
    Return:     
    Desc:
*************************************************************************************/
MCA_S32 MCA_TS_Package(const MCA_PackageTs_t * const pstParam);

/*************************************************************************************
    Function:
    Params:     
    Return:     
    Desc:
*************************************************************************************/
MCA_S32 MCA_TS_Download(const MCA_DownloadTs_t * const pstParam);

/*************************************************************************************
    Function:
    Params:     
    Return:     
    Desc:
*************************************************************************************/
MCA_S32 MCA_TS_Decrypt(MCA_PID pid, MCA_U8 *au8EvenKey, MCA_U8 *au8OddKey, MCA_U8 *ts, MCA_U32 len);

/*************************************************************************************
    Function:
    Params:     
    Return:     
    Desc:
*************************************************************************************/
MCA_S32 MCA_TS_Encrypt(MCA_PID pid, MCA_U8 *key, MCA_BOOL b8Even, MCA_U8 *ts, MCA_U32 len);

#ifdef __cplusplus
}
#endif

#endif    //__MCA_TS_H__

