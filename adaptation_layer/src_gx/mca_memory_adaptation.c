#include "gxcore.h"

#include "mca_adaptation_files.h"

#define GX_MEM_DBG(fmt, args...)    \
    do{    \
        mca_printf("[Gx.INFO ][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

#define GX_MEM_ERR(fmt, args...)    \
    do{    \
        mca_printf("[Gx.ERROR][%s L%d]:", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

MCA_VOID *mca_malloc(MCA_U32 u32Len)
{
    MCA_VOID *pMem = NULL;

    if (0 == u32Len)
    {
        GX_MEM_ERR("Bad Parameter: Length is ZERO!\n");
        return NULL;
    }    
    
    pMem = (MCA_VOID *)GxCore_Malloc(u32Len);
    if (NULL == pMem)
    {
        GX_MEM_ERR("GxCore_Malloc(0x%x) failed!\n", (unsigned int)u32Len);
        return NULL;
    }

    return pMem;
}

MCA_VOID  mca_free(MCA_VOID *pMem)
{
    if (NULL == pMem)
    {
        GX_MEM_ERR("Bad Parameter: Pointer is NULL!\n");
        return;
    }

    GxCore_Free(pMem);
}

MCA_VOID *mca_memcpy(MCA_VOID *pDest, const MCA_VOID *pSrc, MCA_U32 len)
{
    if ((NULL == pDest) || (NULL == pSrc) || (0 == len) || (pDest == pSrc))
    {
        GX_MEM_ERR("Bad Parameter: pDest is 0x%x, pSrc is 0x%x, len is 0x%x(%d)!\n",   \
                    (unsigned int)pDest, (unsigned int)pSrc, (unsigned int)len, (unsigned int)len);
        return NULL;
    }
    if ((((MCA_U32)pDest > (MCA_U32)pSrc) && (((MCA_U32)pSrc+len) > (MCA_U32)pDest))
        || (((MCA_U32)pSrc > (MCA_U32)pDest) && (((MCA_U32)pDest+len) > (MCA_U32)pSrc)))
    {
        GX_MEM_ERR("Bad Parameter: The memory areas must not overlap, pDest = 0x%x, pSrc = 0x%x, len = 0x%x\n", pDest, pSrc, len);
        return NULL;
    }

    return memcpy(pDest, pSrc, len);
}

MCA_VOID *mca_memset(MCA_VOID *pMem, MCA_U8 ch, MCA_U32 len)
{
    if ((NULL == pMem) || (0 == len))
    {
        GX_MEM_ERR("Bad Parameter: pMem = 0x%x, len = 0x%x(%d)!\n", (unsigned int)pMem, len, len);
        return NULL;
    }

    return memset(pMem, ch, len);
}

MCA_S32 mca_memcmp(const MCA_VOID *pMem1, const MCA_VOID *pMem2, MCA_U32 len)
{
    if ((NULL == pMem1) || (NULL == pMem2) || (0 == len))
    {
        GX_MEM_ERR("Bad Parameter: pMem1 = 0x%x, pMem2 = 0x%x, len = 0x%x(%d)!\n", (unsigned int)pMem1, (unsigned int)pMem2, len, len);
    }

    return memcmp(pMem1, pMem2, len);
}

