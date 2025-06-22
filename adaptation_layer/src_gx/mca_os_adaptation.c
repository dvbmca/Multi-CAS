#include "gxos/gxcore_os_core.h"
#include "gxos/gxcore_os.h"
#include <common/gxpm.h>

#include "mca_adaptation_files.h"

#define GX_OS_ERR(fmt, args...) \
    do{ \
        mca_printf("\033[1;40;31m"); \
        mca_printf("\n################MS OS ERROR#################\n"); \
        mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__); \
        mca_printf(fmt, ##args); \
        mca_printf("\n############################################\n\n"); \
        mca_printf("\033[0m\n"); \
    }while(0)

#define MCA_MSG_CMD_SIZE    (4 * sizeof(MCA_U32))

MCA_S32 MCA_OS_TaskCreate(MCA_HANDLE *const pTaskHandle, const MCA_CHAR *szName, MCA_TASK_FUNC_T task_func, MCA_VOID *pParam, MCA_VOID *pStack, MCA_U32 u32StackSize, MCA_TASK_PRIORITY_E enPri)
{
    int32_t  s32GxRet;
    handle_t hGxThreadID;

    if ((NULL == pTaskHandle) || (NULL == szName) || (NULL == task_func) || !((enPri>=MCA_TASK_PRIORITY_LOWEST)&&(enPri<MCA_TASK_PRIORITY_BUTT)))
    {
        GX_OS_ERR("Bad Param: pTaskHandle = 0x%x, szName = 0x%x, task_func = 0x%x, enPri = %d",   \
                                (unsigned int)pTaskHandle, szName,  \
                                (unsigned int)task_func, (unsigned int)enPri);
        return MCA_FAILURE;
    }

    s32GxRet = GxCore_ThreadCreate(szName, &hGxThreadID, (void *)task_func, pParam, 1024*10, GXOS_DEFAULT_PRIORITY);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_ThreadCreate(...) = %d\n", s32GxRet);
        return MCA_FAILURE;
    }

    *pTaskHandle = (MCA_HANDLE)hGxThreadID;

    return MCA_SUCCESS;
}

MCA_VOID mca_reboot(MCA_VOID)
{
    GxCore_Reboot();
}

MCA_VOID mca_sleep(const MCA_U32 msecs)
{
    GxCore_ThreadDelay(msecs);
}

MCA_U32  mca_timestamp(MCA_VOID)
{
    int32_t s32GxRet;
    GxTime  stLocalTime;

    s32GxRet = GxCore_GetTickTime(&stLocalTime);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_GetLocalTime(...) = %d\n", s32GxRet);
        return 0;
    }

    return stLocalTime.seconds*1000+stLocalTime.microsecs/1000;
}

MCA_S32 MCA_OS_QueueCreate(MCA_HANDLE *const pHandle, const MCA_CHAR *szName, const MCA_U32 u32MaxMsgCout)
{
    int32_t  s32GxRet;
    handle_t hGxQueue;

    if ((NULL == pHandle) || (NULL == szName))
    {
        GX_OS_ERR("Bad Param: pHandle = 0x%x, szName = 0x%x\n", (unsigned int)pHandle, szName);
        return MCA_FAILURE;
    }

    s32GxRet = GxCore_QueueCreate(&hGxQueue, u32MaxMsgCout, MCA_MSG_CMD_SIZE);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_QueueCreate(...) = %d\n", s32GxRet);
        return MCA_FAILURE;
    }

    *pHandle = (MCA_HANDLE)hGxQueue;

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_QueueSend(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize)
{
    int32_t  s32GxRet;
    
    if (NULL == pMsg)
    {
        GX_OS_ERR("Bad Param: pMsg is NULL\n");
        return MCA_FAILURE;
    }

    s32GxRet = GxCore_QueuePut((handle_t)handle, pMsg, u32MsgSize, 10);
    if (s32GxRet < 0)
    {
        GX_OS_ERR("GxCore_QueuePut(...) = %d\n", s32GxRet);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_QueueReceive(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize)
{
    int32_t  s32GxRet;
    uint32_t u32Copied;

    if (NULL == pMsg)
    {
        GX_OS_ERR("Bad Param: pMsg is NULL\n");
        return MCA_FAILURE;
    }

    s32GxRet = GxCore_QueueGet((handle_t)handle, pMsg, u32MsgSize, &u32Copied, 0xffffffff/*Forever*/);
    if (s32GxRet < 0)
    {
        GX_OS_ERR("GxCore_QueuePut(...) = %d\n", s32GxRet);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_QueueReceiveTimeout(const MCA_HANDLE handle, MCA_VOID *const pMsg, const MCA_U32 u32MsgSize, const MCA_U32 u32Ms)
{
    int32_t  s32GxRet;
    uint32_t u32Copied;

    if (NULL == pMsg)
    {
        GX_OS_ERR("Bad Param: pMsg is NULL\n");
        return MCA_FAILURE;
    }

    s32GxRet = GxCore_QueueGet((handle_t)handle, pMsg, u32MsgSize, &u32Copied, u32Ms);
    if (s32GxRet < 0)
    {
        if (-8 == s32GxRet)
        {
            return MCA_E06_TIMEOUT;
        }
        GX_OS_ERR("GxCore_QueuePut(...) = %d\n", s32GxRet);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemCreate(MCA_HANDLE *const pSemHandle, const MCA_CHAR *szName, const MCA_U32 u32InitCount)
{
    int32_t  s32GxRet;
    handle_t hGxSem;

    if ((NULL == pSemHandle) || (NULL == szName))
    {
        GX_OS_ERR("Bad Param: pSemHandle is 0x%x, szName = 0x%x!\n", (unsigned long)pSemHandle, (unsigned long)szName);
        return MCA_FAILURE;
    }

    s32GxRet = GxCore_SemCreate(&hGxSem, u32InitCount);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_SemCreate(...) = %d\n", s32GxRet);
        *pSemHandle = MCA_INVALID_HANDLE;
        return MCA_FAILURE;
    }

    *pSemHandle = (MCA_HANDLE)hGxSem;

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemLock(const MCA_HANDLE handle)
{
    int32_t  s32GxRet;
    handle_t hGxSem = (handle_t)handle;

    if (hGxSem < 0)
    {
        GX_OS_ERR("Bad Param: hGxSem(%d) < 0\n", hGxSem);
        return MCA_FAILURE;
    }
    
    s32GxRet = GxCore_SemWait(hGxSem);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_SemWait(...) = %d \n", s32GxRet);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemLockTimeout(const MCA_HANDLE handle, MCA_U32 ms)
{
    int32_t  s32GxRet;
    handle_t hGxSem = (handle_t)handle;

    if (hGxSem < 0)
    {
        GX_OS_ERR("Bad Param: hGxSem(%d) < 0\n", hGxSem);
        return MCA_FAILURE;
    }
    
    s32GxRet = GxCore_SemTimedWait(hGxSem, ms);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_SemTimedWait(...) = %d \n", s32GxRet);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_S32 MCA_OS_SemUnlock(const MCA_HANDLE handle)
{
    int32_t  s32GxRet;
    handle_t hGxSem = (handle_t)handle;

    if (hGxSem < 0)
    {
        GX_OS_ERR("Bad Param: hGxSem(%d) < 0\n", hGxSem);
        return MCA_FAILURE;
    }
    
    s32GxRet = GxCore_SemPost(hGxSem);
    if (GXCORE_SUCCESS != s32GxRet)
    {
        GX_OS_ERR("GxCore_SemPost(...) = %d \n", s32GxRet);
        return MCA_FAILURE;
    }

    return MCA_SUCCESS;
}

MCA_VOID  mca_printf(MCA_CHAR *format, ...)
{
    int      ir;
    va_list  ap;
    MCA_CHAR szPrint[100];
    MCA_U32  len;

    len = sizeof(szPrint);
    if (format != NULL)
    {
        mca_memset(szPrint, 0x00, len);
        va_start(ap, format);
        ir = vsnprintf(szPrint, len, format, ap);
        va_end(ap);

        printf("%s", szPrint);
    }
}

