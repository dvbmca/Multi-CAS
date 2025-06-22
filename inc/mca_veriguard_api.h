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

#ifndef __MCA_VERIGUARD_API_H__
#define __MCA_VERIGUARD_API_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef struct
{
    MCA_U8      m_szSerialNumber[12];
    MCA_U32     m_u32PPUA;
    MCA_BOOL    m_b8Status;    
    MCA_U8      m_szStartDate[20];
    MCA_U8      m_szExpiredDate[20];
    MCA_U32     m_u32AccessCertification;
    MCA_U8      m_szProviderName[16];
    MCA_U32     m_u32SharedAddress;
    MCA_U16     m_u16ProviderCHID;
    MCA_U16     m_u16ProviderCAID;
}MCA_VeriGuardCardInfo_t;

typedef enum
{
    MCA_VG_COMMAND_NONE     = 0,
    MCA_VG_COMMAND_REBOOT   = 1,
    MCA_VG_COMMAND_SHUTDOWN = 2,
    MCA_VG_COMMAND_MUTE     = 3,
    MCA_VG_COMMAND_STB_INFO = 4,
}MCA_VeriGuardCmd_t;

typedef struct
{
    MCA_U8              m_szProviderName[16];
    MCA_U8              m_szSerialNumber[12];
    MCA_U8              m_szContent[48];
    MCA_U8              m_u8Timeount;
    MCA_VeriGuardCmd_t  m_emCommand;
}MCA_VeriGuardOSD_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_VERIGUARD_API_H__

