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

#ifndef __MCA_DEMUX_ADAPTATION_H__
#define __MCA_DEMUX_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_DMX_FILTER_MAX_DEPTH    16

typedef enum
{
    MCA_DMX_CHAN_TYPE_UNKNOWN = 0,
    MCA_DMX_CHAN_TYPE_SECTION,
    MCA_DMX_CHAN_TYPE_PES,
}MCA_DMX_CHAN_TYPE_t;

typedef struct
{
    MCA_DMX_ID_t    m_DmxId;
    MCA_HANDLE      m_hChannelHandle;
    MCA_HANDLE      m_hFilterHandle;

    MCA_U32         m_u32Len;
    MCA_U8         *m_u8Section;
} MCA_DMX_CB_DATA_t;

typedef void (*MCA_DMX_CB_t)(MCA_DMX_CB_DATA_t *pstData);

typedef struct 
{
    MCA_DMX_CHAN_TYPE_t m_enType;
    MCA_U32             m_u32BuffSize;
    MCA_BOOL            m_bCRC;
    MCA_U16             m_pid;

    MCA_DMX_CB_t        m_cb;
} MCA_DMX_CHAN_ATTR_t;

typedef struct 
{
    MCA_U8  m_au8Mask [MCA_DMX_FILTER_MAX_DEPTH];
    MCA_U8  m_au8Match[MCA_DMX_FILTER_MAX_DEPTH];
    MCA_U8  m_u8Depth;
} MCA_DMX_FILTER_t;

MCA_S32 mca_dmx_init(MCA_VOID);

MCA_S32 mca_dmx_channel_create (MCA_DMX_ID_t id, const MCA_DMX_CHAN_ATTR_t *pstChAttr, MCA_HANDLE *phChannel);
MCA_S32 mca_dmx_channel_destory(MCA_HANDLE  hChannel);

MCA_S32 mca_dmx_channel_enable (MCA_HANDLE  hChannel);
MCA_S32 mca_dmx_channel_disable(MCA_HANDLE  hChannel);

MCA_S32 mca_dmx_channel_set_pid(MCA_HANDLE  hChannel, MCA_U16 pid);
MCA_S32 mca_dmx_channel_register_callback(MCA_HANDLE  hChannel, MCA_DMX_CB_t cb);

MCA_S32 mca_dmx_filter_create (MCA_HANDLE hChannel, const MCA_DMX_FILTER_t *pstFltInfo, MCA_HANDLE *phFilter);
MCA_S32 mca_dmx_filter_set    (MCA_HANDLE hFilter, const MCA_DMX_FILTER_t *pstFltInfo);
MCA_S32 mca_dmx_filter_destory(MCA_HANDLE hFilter);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_DEMUX_ADAPTATION_H__

