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

#ifndef __MCA_TYPE_H__
#define __MCA_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long  MCA_U64;
typedef long long           MCA_S64;

typedef unsigned int    MCA_U32;
typedef signed int      MCA_S32;

typedef unsigned short  MCA_U16;
typedef signed short    MCA_S16;

typedef unsigned char   MCA_U8;
typedef signed char     MCA_S8;
typedef char            MCA_CHAR;
typedef unsigned char   MCA_BOOL;


typedef MCA_U32         MCA_HANDLE;
typedef MCA_U16         MCA_PID;

typedef MCA_U32         MCA_TIMESTAMP;

typedef MCA_U8          MCA_DMX_ID_t;

#define MCA_TRUE         1
#define MCA_FALSE        0

#ifndef NULL
#ifdef __cplusplus
#define NULL            0
#else
#define NULL            ((void *)0)
#endif
#endif

/*typedef void          MCA_VOID;*/
#define MCA_VOID         void

#define MCA_SUCCESS      0
#define MCA_FAILURE      (-1)

#define MCA_INVALID_HANDLE   (0xffffffff)
#define MCA_INVALID_LCN      (0xffff)

#define MCA_INVALID_PID          (0x1FFF)
#define MCA_IS_INVALID_PID(pid)  ((MCA_PID)(pid) >= MCA_INVALID_PID)

#define MCA_INVALID_ID          (0xffffffff)
#define MCA_INVALID_DEMUX_ID    (0xff)

typedef struct
{
    MCA_U16 m_Year;
    MCA_U8  m_Month;
    MCA_U8  m_Day;
    MCA_U8  m_Hour;
    MCA_U8  m_Min;
    MCA_U8  m_Second;
    MCA_U8  m_Res;
}MCA_DATE_t;

#ifdef __cplusplus
}
#endif

#endif

