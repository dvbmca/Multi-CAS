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

#ifndef __MCA_OS_ADAPTATION_H__
#define __MCA_OS_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef MCA_VOID* (*MCA_TASK_FUNC_T)(MCA_VOID * pParam);

typedef enum
{
    MCA_TASK_PRIORITY_LOWEST    = 0,
    MCA_TASK_PRIORITY_LOW       = 29,
    MCA_TASK_PRIORITY_NORMAL    = 49,
    MCA_TASK_PRIORITY_HIGH      = 69,
    MCA_TASK_PRIORITY_HIGHEST   = 99,
    MCA_TASK_PRIORITY_BUTT      = 100,
} MCA_TASK_PRIORITY_E;

MCA_S32 MCA_OS_TaskCreate(MCA_HANDLE *const pTaskHandle, const MCA_CHAR *szName,    \
                            MCA_TASK_FUNC_T task_func, MCA_VOID *pParam,    \
                            MCA_VOID *pStack,   \
                            MCA_U32 u32StackSize, MCA_TASK_PRIORITY_E enPri);
/*************************************************************************************
    Function:   mca_sleep
    Params:     msecs: Milliseconds to wait.
    Return:     No return value.
    Desc:       Sleeps msecs milliseconds.
*************************************************************************************/
MCA_VOID mca_sleep(const MCA_U32 msecs);

/*************************************************************************************
    Function:   mca_timestamp
    Params:     
    Return:     
    Desc:       
*************************************************************************************/
MCA_U32  mca_timestamp(MCA_VOID);

MCA_S32 MCA_OS_QueueCreate(MCA_HANDLE *const pQueHandle, const MCA_CHAR *szName, const MCA_U32 u32MaxMsgCout);
MCA_S32 MCA_OS_QueueSend(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize);
MCA_S32 MCA_OS_QueueReceive(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize);

MCA_S32 MCA_OS_SemCreate(MCA_HANDLE *const pSemHandle, const MCA_CHAR *szName, const MCA_U32 u32InitCount);
MCA_S32 MCA_OS_SemLock(const MCA_HANDLE handle);
MCA_S32 MCA_OS_SemLockTimeout(const MCA_HANDLE handle, MCA_U32 ms);
MCA_S32 MCA_OS_SemUnlock(const MCA_HANDLE handle);
MCA_S32 MCA_OS_SemDestroy(const MCA_HANDLE handle);

MCA_VOID  mca_printf(MCA_CHAR *format, ...);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_OS_ADAPTATION_H__

