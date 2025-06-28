#include "mca_adaptation_files.h"

static MCA_U8 g_au8NVMCom[64*1024];
static MCA_U8 g_au8NVM[64*1024];

MCA_S32 mca_nvram_init(MCA_VOID)
{
    return MCA_SUCCESS;
}

MCA_S32 mca_nvram_write(MCA_NVM_TYPE_t enType, MCA_U32 u32Offset, MCA_U8 *pu8WriteBuff, MCA_U32 u32Len)
{
    if (MCA_NVM_COM == enType)
    {
        mca_memcpy(&g_au8NVMCom[u32Offset], pu8WriteBuff, u32Len);
    }
    else
    {
        mca_memcpy(&g_au8NVM[u32Offset], pu8WriteBuff, u32Len);
    }    

    return MCA_SUCCESS;
}

MCA_S32 mca_nvram_read(MCA_NVM_TYPE_t enType, MCA_U32 u32Offset, MCA_U8 *pu8ReadBuff, MCA_U32 u32Len)
{
    if (MCA_NVM_COM == enType)
    {
        mca_memcpy(pu8ReadBuff, &g_au8NVMCom[u32Offset], u32Len);
    }
    else
    {
        mca_memcpy(pu8ReadBuff, &g_au8NVM[u32Offset], u32Len);
    }

    return MCA_SUCCESS;
}

MCA_S32 mca_nvram_read_cryptoguard_key(MCA_NVM_CRYPTOGUARD_KEY_t *pstKey)
{
    return MCA_FAILURE;
}

