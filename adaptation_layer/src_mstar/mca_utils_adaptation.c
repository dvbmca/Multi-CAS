#include "MsCommon.h"
#include "MsOS.h"
//#include "drvPWSV.h"

#include "mca_adaptation_files.h"

#define MS_UTL_DBG(fmt, args...)		\
    do{	\
        mca_printf("----ms utils>[%s]Line%d: ", __FUNCTION__, __LINE__);	\
        mca_printf(fmt, ##args);	\
    }while(0)

#define MS_UTL_ERR(fmt, args...)		\
    do{	\
        mca_printf("\033[1;40;31m");	\
        mca_printf("\n################MS UTILS ERROR################\n");	\
        mca_printf("[%s]Line%d:\n", __FUNCTION__, __LINE__);	\
        mca_printf(fmt, ##args);	\
        mca_printf("\n##############################################\n\n");	\
        mca_printf("\033[0m\n");\
    }while(0)

static MCA_BOOL g_b8GetChipIDSuccess = MCA_FALSE;
static MCA_U8   g_au8ChipID[8];

MCA_S32 mca_get_stb_id(MCA_U32* pu32Len, MCA_U8* pu8StbID)
{
    MCA_U16 u16OriVal;
    MCA_U16 u16Val;

    if ((NULL == pu32Len) || (NULL == pu8StbID))
    {
        MS_UTL_ERR("Bad Param: pu32Len = 0x%x, pu8StbID = 0x%x!\n", pu32Len, pu8StbID);
        return MCA_FAILURE;
    }

    if (MCA_TRUE == g_b8GetChipIDSuccess)
    {
        mca_memcpy(pu8StbID, g_au8ChipID, sizeof(g_au8ChipID));
        *pu32Len = 8;
        return MCA_SUCCESS;
    }

    u16OriVal = MDrv_Read2Byte(0x2002);
    MDrv_Write2Byte(0x2002, 0x08);
    u16Val = MDrv_Read2Byte(0x2034);
    g_au8ChipID[0] = (MCA_U8)(u16Val>>8);
    g_au8ChipID[1] = (MCA_U8)(u16Val&0xFF);
    u16Val = MDrv_Read2Byte(0x2036);
    g_au8ChipID[2] = (MCA_U8)(u16Val>>8);
    g_au8ChipID[3] = (MCA_U8)(u16Val&0xFF);
    u16Val = MDrv_Read2Byte(0x2038);
    g_au8ChipID[4] = (MCA_U8)(u16Val>>8);
    g_au8ChipID[5] = (MCA_U8)(u16Val&0xFF);
    u16Val = MDrv_Read2Byte(0x203A);
    g_au8ChipID[6] = (MCA_U8)(u16Val>>8);
    g_au8ChipID[7] = (MCA_U8)(u16Val&0xFF);
    MDrv_Write2Byte(0x2002, u16OriVal);

    mca_memcpy(pu8StbID, g_au8ChipID, sizeof(g_au8ChipID));
    *pu32Len = 8;
    g_b8GetChipIDSuccess = MCA_TRUE;
    return MCA_SUCCESS;
}

