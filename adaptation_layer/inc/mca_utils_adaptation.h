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

#ifndef __MCA_UTILS_ADAPTATION_H__
#define __MCA_UTILS_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef struct
{
    MCA_U16 m_u16PmtPid;
    MCA_U16 m_u16ProgramNumber;
    MCA_U16 m_u16TSID;
    MCA_U16 m_u16VPid;
    MCA_U16 m_u16APid;
} MCA_SERVICE_t;

MCA_S32     mca_get_stb_id(MCA_U32* pu32Len, MCA_U8* pu8StbID);
MCA_S32     mca_get_current_service(MCA_DMX_ID_t id, MCA_SERVICE_t *pstSv);
MCA_CHAR*   mca_get_usb_root_path(MCA_VOID);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_UTILS_ADAPTATION_H__

