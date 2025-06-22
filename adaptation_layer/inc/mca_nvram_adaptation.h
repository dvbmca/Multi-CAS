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

#ifndef __MCA_NVM_ADAPTATION_H__
#define __MCA_NVM_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef enum
{
    MCA_NVM_COM     = 0,
    MCA_NVM_CTI,
    MCA_NVM_CONAX,
    MCA_NVM_GRIFFIN,
    MCA_NVM_CRYPTOGUARD,
    MCA_NVM_SOCHUANG,
    MCA_NVM_DVCRYPT,
    MCA_NVM_CRYPTON,
    MCA_NVM_ABV,
    MCA_NVM_TOPREAL,
    MCA_NVM_GOSPELL,
    MCA_NVM_SUMAVISION,
    MCA_NVM_VERIGUARD,
    MCA_NVM_AMOUNT,    
    MCA_NVM_NONE    = 0xFF,
}MCA_NVM_TYPE_t;

typedef struct
{
    MCA_U32 m_u32BoxID;
    MCA_U8  m_au8SystemGlobalKey[16];
    MCA_U8  m_au8ManufacturerGlobalKey[16];
    MCA_U8  m_au8GroupKey[16];
    MCA_U8  m_au8UniqueKey[16];
    MCA_U8  m_au8CardlessGroupKey[16];
    MCA_U8  m_au8CardlessUniqueKey[16];
} MCA_NVM_CRYPTOGUARD_KEY_t;

/*************************************************************************************
    Function:   mca_nvram_init
    Params:     None
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Initializes the NVRAM adaptation layer.
                The default value should be 0xFF.
                The length of COM NVRAM is 3, Other are 64K.
                When STB factory setting, should not change data in NVRAM.
*************************************************************************************/
MCA_S32 mca_nvram_init(MCA_VOID);

/*************************************************************************************
    Function:   mca_nvram_write
    Params:     enType: 
                u32Offset:
                pu8WriteBuff:
                u32Len:
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Each enType has its own independent NVRAM ares.
                Should save different enType in different ares.
*************************************************************************************/
MCA_S32 mca_nvram_write(MCA_NVM_TYPE_t enType, MCA_U32 u32Offset, MCA_U8 *pu8WriteBuff, MCA_U32 u32Len);

/*************************************************************************************
    Function:   mca_nvram_read
    Params:     enType:
                u32Offset:
                pu8ReadBuff:
                u32Len:
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Each enType has its own independent NVRAM ares.
                Should read data from its own corresponding area.
*************************************************************************************/
MCA_S32 mca_nvram_read (MCA_NVM_TYPE_t enType, MCA_U32 u32Offset, MCA_U8 *pu8ReadBuff, MCA_U32 u32Len);

/*************************************************************************************
    Function:   mca_nvram_read_cryptoguard_key
    Params:     pstKey:
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       
*************************************************************************************/
MCA_S32 mca_nvram_read_cryptoguard_key(MCA_NVM_CRYPTOGUARD_KEY_t *pstKey);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_NVM_ADAPTATION_H__

