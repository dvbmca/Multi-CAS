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

#ifndef __MCA_DESCRAMBLER_ADAPTATION_H__
#define __MCA_DESCRAMBLER_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

MCA_S32 mca_descrambler_create(MCA_DMX_ID_t id, MCA_HANDLE *phChannel);
MCA_S32 mca_descrambler_set_pid(MCA_HANDLE hChannel, MCA_U16 pid);
MCA_S32 mca_descrambler_set_odd_cw (MCA_HANDLE hChannel, MCA_U8 *key, MCA_U8 len);
MCA_S32 mca_descrambler_set_even_cw(MCA_HANDLE hChannel, MCA_U8 *key, MCA_U8 len);
MCA_S32 mca_descrambler_destroy(MCA_HANDLE hChannel);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_DESCRAMBLER_ADAPTATION_H__

