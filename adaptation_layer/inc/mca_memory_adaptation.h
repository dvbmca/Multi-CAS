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

#ifndef __MCA_MEMORY_ADAPTATION_H__
#define __MCA_MEMORY_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

/*************************************************************************************
    Function:   mca_malloc
    Params:     size: Memory size asked for.
    Return:     The pointer to the allocated memory, 0 if no memoy available.
    Desc:       Allocates memory.
*************************************************************************************/
MCA_VOID *mca_malloc(MCA_U32 size);

/*************************************************************************************
    Function:   mca_free
    Params:     data: Data to free.
    Return:     No return vaue.
    Desc:       Frees a previous allocated memory.
*************************************************************************************/
MCA_VOID  mca_free(MCA_VOID *pMem);

/*************************************************************************************
    Function:   mca_memcpy
    Params:     pDest: Destination buffer.
                pSrc: Origin buffer.
                len: Bytes to copy.
    Return:     Destination buffer.
    Desc:       refer to "void *memcpy(void *destin, void *source, unsigned n)".
*************************************************************************************/
MCA_VOID *mca_memcpy(MCA_VOID *pDest, const MCA_VOID *pSrc, MCA_U32 len);

/*************************************************************************************
    Function:   mca_memset
    Params:     pMem: Destination buffer.
                ch: Character.
                len: Bytes to set.
    Return:     
    Desc:       refer to "void *memset(void *s, int ch, size_t n)".
*************************************************************************************/
MCA_VOID *mca_memset(MCA_VOID *pMem, MCA_U8 ch, MCA_U32 len);

/*************************************************************************************
    Function:   mca_memcmp
    Params:
                
    Return:
    Desc:       refer to "int memcmp(const void *str1, const void *str2, size_t n)".
*************************************************************************************/
MCA_S32 mca_memcmp(const MCA_VOID *pMem1, const MCA_VOID *pMem2, MCA_U32 len);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_MEMORY_ADAPTATION_H__

