#include "MsCommon.h"
#include "MsOS.h"

#include "mca_adaptation_files.h"

#define MS_MEM_DBG(fmt, args...)    \
    do{    \
        mca_printf("----ms mem>[%s]Line%d: ", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define MS_MEM_ERR(fmt, args...)    \
    do{    \
        mca_printf("\033[1;40;31m");    \
        mca_printf("\n################MS MEM ERROR################\n");    \
        mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
        mca_printf("\n############################################\n\n");    \
        mca_printf("\033[0m\n");\
    }while(0)

extern MS_S32 gs32NonCachedPoolID;

MCA_VOID *mca_malloc(MCA_U32 u32Len)
{
    MCA_VOID *pMem;

    if (0 == u32Len)
    {
        MS_MEM_ERR("Bad Param: u32Len is 0!");
        return NULL;
    }    
    
    pMem = (MCA_VOID *)MsOS_AllocateMemory(u32Len, gs32NonCachedPoolID);
    if (NULL == pMem)
    {
        MS_MEM_ERR("MsOS_AllocateMemory(0x%x, 0x%x) failed\n", (unsigned int)u32Len, gs32NonCachedPoolID);
        return NULL;
    }

    return pMem;
}

MCA_VOID  mca_free(MCA_VOID *pMem)
{
    MS_BOOL b8Ret;

    if (NULL == pMem)
    {
        MS_MEM_ERR("Bad Param: pvMemory = NULL \n");
        return;
    }

    b8Ret = MsOS_FreeMemory(pMem, gs32NonCachedPoolID);
    if (FALSE == b8Ret)
    {
        MS_MEM_ERR("MsOS_FreeMemory(...) = FALSE \n");
    }
}

MCA_VOID *mca_memcpy(MCA_VOID *pDest, const MCA_VOID *pSrc, MCA_U32 len)
{
    if ((NULL == pDest) || (NULL == pSrc) || (0 == len) || (pDest == pSrc))
    {
        MS_MEM_ERR("Bad Param: pDest is 0x%x, pSrc is 0x%x, len is 0x%x(%d)!\n",   \
                    (unsigned int)pDest, (unsigned int)pSrc, (unsigned int)len, (unsigned int)len);
        return NULL;
    }
    if ((((MCA_U32)pDest > (MCA_U32)pSrc) && (((MCA_U32)pSrc+len) > (MCA_U32)pDest))
        || (((MCA_U32)pSrc > (MCA_U32)pDest) && (((MCA_U32)pDest+len) > (MCA_U32)pSrc)))
    {
        MS_MEM_ERR("Bad Param: The memory areas must not overlap, pDest = 0x%x, pSrc = 0x%x, len = 0x%x\n", pDest, pSrc, len);
        return NULL;
    }

    return memcpy(pDest, pSrc, len);
}

MCA_VOID *mca_memset(MCA_VOID *pMem, MCA_U8 ch, MCA_U32 len)
{
    if ((NULL == pMem) || (0 == len))
    {
        MS_MEM_ERR("Bad Param: pMem = 0x%x, len = 0x%x(%d)!\n", (unsigned int)pMem, len, len);
        return NULL;
    }

    return memset(pMem, ch, len);
}

MCA_S32 mca_memcmp(const MCA_VOID *pMem1, const MCA_VOID *pMem2, MCA_U32 len)
{
    if ((NULL == pMem1) || (NULL == pMem2) || (0 == len))
    {
        MS_MEM_ERR("Bad Param: pMem1 = 0x%x, pMem2 = 0x%x, len = 0x%x(%d)!\n", (unsigned int)pMem1, (unsigned int)pMem2, len, len);
    }

    return memcmp(pMem1, pMem2, len);
}

