#include <stdarg.h>
#include "MsCommon.h"
#include "MsOS.h"
#include "drvUART.h"
#include "drvWDT.h"

#include "mca_adaptation_files.h"

#define MS_OS_ERR(fmt, args...) \
    do{    \
        mca_printf("\033[1;40;31m");    \
        mca_printf("\n################MS OS ERROR#################\n");    \
        mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
        mca_printf("\n############################################\n\n");    \
        mca_printf("\033[0m\n");    \
    }while(0)

#define MCA_MSG_CMD_SIZE    (4 * sizeof(MCA_U32))

extern MS_S32 gs32NonCachedPoolID;

MCA_S32 MCA_OS_TaskCreate(MCA_HANDLE *const pTaskHandle, const MCA_CHAR *szName,    \
                            MCA_TASK_FUNC_T task_func, MCA_VOID *pParam,    \
                            MCA_VOID *pStack,   \
                            MCA_U32 u32StackSize, MCA_TASK_PRIORITY_E enPri)
{
    MS_S32  s32MsTaskID;
    void    *pStackBuff;

    if ((NULL == pTaskHandle) || (NULL == szName) || (NULL == task_func) || !((enPri>=MCA_TASK_PRIORITY_LOWEST)&&(enPri<MCA_TASK_PRIORITY_BUTT)))
    {
        MS_OS_ERR("Bad Param: pTaskHandle = 0x%x, szName = 0x%x, task_func = 0x%x, enPri = %d",   \
                                (unsigned int)pTaskHandle, szName,  \
                                (unsigned int)task_func, (unsigned int)enPri);
        return MCA_FAILURE;
    }

/*    mca_printf("<IN> [%s]Line%d: pTaskHandle = 0x%x, szName = %s, task_func = 0x%x, u32StackSize = 0x%x, enPri = %d\n", \
                __FUNCTION__, __LINE__, \
                pTaskHandle, szName, task_func, u32StackSize, enPri);*/

    if (u32StackSize < 8192)
    {
        u32StackSize = 8192;
    }
    pStackBuff = MsOS_AllocateMemory(u32StackSize, gs32NonCachedPoolID);
    if (NULL == pStackBuff)
    {
        MS_OS_ERR("MsOS_AllocateMemory(...) = NULL\n");
        return MCA_FAILURE;
    }

    s32MsTaskID = MsOS_CreateTask((TaskEntry)task_func, (MS_U32)NULL, E_TASK_PRI_MEDIUM, TRUE, pStackBuff, (MS_U32)u32StackSize, (char *)szName);
    if (s32MsTaskID < 0)
    {
        MsOS_FreeMemory(pStackBuff, gs32NonCachedPoolID);
        MS_OS_ERR("MsOS_CreateTask(...) = %d\n", s32MsTaskID);
        return MCA_FAILURE;
    }

    *pTaskHandle = (MCA_HANDLE)s32MsTaskID;

 //   mca_printf("<OU> [%s]Line%d: *pTaskHandle = 0x%x\n", __FUNCTION__, __LINE__, *pTaskHandle);

    return MCA_SUCCESS;
}

MCA_VOID mca_reboot(MCA_VOID)
{
    MDrv_WDT_SetTimer_ms(E_WDT_DBGLV_ALL, 10);
    MDrv_WDT_SetIntTimer(E_WDT_DBGLV_ALL, 5);
}

MCA_VOID mca_sleep(const MCA_U32 msecs)
{
    MsOS_DelayTask((MCA_U32)msecs);
}

MCA_U32  mca_timestamp(MCA_VOID)
{
    return MsOS_GetSystemTime();
}

MCA_S32 MCA_OS_QueueCreate(MCA_HANDLE *const pHandle, const MCA_CHAR *szName, const MCA_U32 u32MaxMsgCout)
{
    MS_S32 s32MsQueueID;

    if ((NULL == pHandle) || (NULL == szName))
    {
        MS_OS_ERR("Bad Param: pHandle = 0x%x, szName = 0x%x\n", (unsigned int)pHandle, szName);
        return MCA_FAILURE;
    }

//    mca_printf("<IN> [%s]Line%d: pHandle = 0x%x, szName = %s\n", __FUNCTION__, __LINE__, pHandle, szName);

    s32MsQueueID = MsOS_CreateQueue(NULL,
                                  MCA_MSG_CMD_SIZE * 64,
                                  E_MSG_FIXED_SIZE,
                                  MCA_MSG_CMD_SIZE,
                                  E_MSOS_FIFO,
                                  (char *)szName);
    if (s32MsQueueID < 0)
    {
        MS_OS_ERR("MsOS_CreateQueue(...) = %d\n", s32MsQueueID);
        return MCA_FAILURE;
    }

    *pHandle = (MCA_HANDLE)s32MsQueueID;
//    mca_printf("<OU> [%s]Line%d: *pHandle = 0x%x\n", __FUNCTION__, __LINE__, *pHandle);

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_QueueSend(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize)
{
//    mca_printf("<IN> [%s]Line%d: handle = 0x%x, pMsg = 0x%x, u32MsgSize = %d\n", __FUNCTION__, __LINE__, handle, pMsg, u32MsgSize);

    if (NULL == pMsg)
    {
        MS_OS_ERR("Bad Param: pMsg is NULL\n");
        return MCA_FAILURE;
    }

    if (FALSE == MsOS_SendToQueue((MS_S32)handle, pMsg, u32MsgSize, 10))
    {
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_QueueReceive(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize)
{
    MS_BOOL b8Status;
    MS_U32  u32ActualSize;

//    mca_printf("<IN> [%s]Line%d: handle = 0x%x, pMsg = 0x%x, u32MsgSize = %d\n", __FUNCTION__, __LINE__, handle, pMsg, u32MsgSize);

    if (NULL == pMsg)
    {
        MS_OS_ERR("Bad Param: pMsg is NULL\n");
        return MCA_FAILURE;
    }

    b8Status = MsOS_RecvFromQueue((MS_S32)handle, pMsg, u32MsgSize, &u32ActualSize, MSOS_WAIT_FOREVER);
    if ((FALSE == b8Status) || (u32ActualSize != u32MsgSize))
    {
        MS_OS_ERR("MsOS_RecvFromQueue(...) = %d, u32ActualSize = %d, u32MsgSize = %d\n", b8Status, u32ActualSize, u32MsgSize);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_QueueReceiveTimeout(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize, const MCA_U32 u32Ms)
{
    MS_BOOL b8Status;
    MS_U32  u32ActualSize;

//    mca_printf("<IN> [%s]Line%d: handle = 0x%x, pMsg = 0x%x, u32MsgSize = %d\n", __FUNCTION__, __LINE__, handle, pMsg, u32MsgSize);

    if (NULL == pMsg)
    {
        MS_OS_ERR("Bad Param: pMsg is NULL\n");
        return MCA_FAILURE;
    }

    b8Status = MsOS_RecvFromQueue((MS_S32)handle, pMsg, u32MsgSize, &u32ActualSize, u32Ms);
    if ((FALSE == b8Status) || (u32ActualSize != u32MsgSize))
    {
        MS_OS_ERR("MsOS_RecvFromQueue(...) = %d, u32ActualSize = %d, u32MsgSize = %d\n", b8Status, u32ActualSize, u32MsgSize);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemCreate(MCA_HANDLE *const pSemHandle, const MCA_CHAR *szName, const MCA_U32 u32InitCount)
{
    MS_S32 s32MsSem;

    if ((NULL == pSemHandle) || (NULL == szName))
    {
        MS_OS_ERR("Bad Param: pSemHandle is 0x%x, szName = 0x%x!\n", (unsigned long)pSemHandle, (unsigned long)szName);
        return MCA_FAILURE;
    }

//    mca_printf("<IN> [%s]Line%d: pSemHandle = 0x%x szName = %s, u32InitCount = %d\n", __FUNCTION__, __LINE__, pSemHandle, szName, u32InitCount);

    s32MsSem = MsOS_CreateSemaphore((MS_U32)u32InitCount, E_MSOS_FIFO, (char *)szName);
    if (s32MsSem < 0)
    {
        MS_OS_ERR("MsOS_CreateSemaphore(...) = %d\n", s32MsSem);
        *pSemHandle = MCA_INVALID_HANDLE;
        return MCA_FAILURE;
    }

    *pSemHandle = (MCA_HANDLE)s32MsSem;
 //   mca_printf("<OU> [%s]Line%d: *pSemHandle = 0x%x\n", __FUNCTION__, __LINE__, *pSemHandle);

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemLock(const MCA_HANDLE handle)
{
    MS_BOOL b8Ret;
    MS_S32  s32MsSem = (MS_S32)handle;

//    mca_printf("[%s]Line%d: handle = 0x%x\n", __FUNCTION__, __LINE__, handle);

    if (s32MsSem < 0)
    {
        MS_OS_ERR("Bad Param: s32MsSem(%d) < 0\n", s32MsSem);
        return MCA_FAILURE;
    }
    
    b8Ret = MsOS_ObtainSemaphore(s32MsSem, MSOS_WAIT_FOREVER);
    if (FALSE == b8Ret)
    {
        MS_OS_ERR("MsOS_ObtainSemaphore(...) = FALSE \n");
        return MCA_FAILURE;
    }
    
    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemLockTimeout(const MCA_HANDLE handle, MCA_U32 ms)
{
    MS_BOOL b8Ret;
    MS_S32  s32MsSem = (MS_S32)handle;

//    mca_printf("[%s]Line%d: handle = 0x%x\n", __FUNCTION__, __LINE__, handle);

    if (s32MsSem < 0)
    {
        MS_OS_ERR("Bad Param: s32MsSem(%d) < 0\n", s32MsSem);
        return MCA_FAILURE;
    }
    
    b8Ret = MsOS_ObtainSemaphore(s32MsSem, ms);
    if (FALSE == b8Ret)
    {
    #if 0
        /* If timeout, API will return FALSE. */
        MS_OS_ERR("MsOS_ObtainSemaphore(..., %d) = FALSE \n", ms);
        return MCA_FAILURE;
    #endif
    }
    
    return MCA_SUCCESS;
}


MCA_S32 MCA_OS_SemUnlock(const MCA_HANDLE handle)
{
    MS_BOOL b8Ret;
    MS_S32  s32MsSem = (MS_S32)handle;

//    mca_printf("[%s]Line%d: handle = 0x%x\n", __FUNCTION__, __LINE__, handle);

    if (s32MsSem < 0)
    {
        MS_OS_ERR("Bad Param: s32MsSem(%d) < 0\n", s32MsSem);
        return MCA_FAILURE;
    }

    b8Ret = MsOS_ReleaseSemaphore(s32MsSem);
    if (FALSE == b8Ret)
    {
        MS_OS_ERR("MsOS_ObtainSemaphore(...) = FALSE \n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemDestroy(const MCA_HANDLE handle)
{
    MS_BOOL b8Ret;
    MS_S32  s32MsSem = (MS_S32)handle;

//    mca_printf("[%s]Line%d: handle = 0x%x\n", __FUNCTION__, __LINE__, handle);

    if (s32MsSem < 0)
    {
        MS_OS_ERR("Bad Param: s32MsSem(%d) < 0\n", s32MsSem);
        return MCA_FAILURE;
    }

    b8Ret = MsOS_DeleteSemaphore(s32MsSem);
    if (FALSE == b8Ret)
    {
        MS_OS_ERR("MsOS_DeleteSemaphore(...) = FALSE \n");
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_VOID  mca_printf(MCA_CHAR *format, ...)
{
#if 0
    int i, ir;
    va_list ap;
    MCA_CHAR szPrint[100];

    if (format != NULL)
    {
        va_start(ap, format);
        ir = vsnprintf(szPrint, 100, format, ap);
        va_end(ap);

        for (i = 0; i < ir; i++)
        {
            if (szPrint[i] == '\n')
            {
                MDrv_UART_PutChar('\r');
            }
            MDrv_UART_PutChar(szPrint[i]);
        }
    }
#else
    printf(format);
#endif
}

