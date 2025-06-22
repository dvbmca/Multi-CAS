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

#ifndef __MCA_USB_H__
#define __MCA_USB_H__

#include "mca_type.h"

#ifdef __cplusplus
extern "C " {
#endif

typedef enum
{
    MCA_USB_EVENT_DUMP_STB_ID = 0,
}MCA_USB_EVENT_t;

/*************************************************************************************
    status: Refer to mca_error_code.h
            MCA_SUCCESS
            MCA_FAILURE
            MCA_E07_ALREADY_EXIST
    pData : When enEvtType is MCA_USB_EVENT_DUMP_STB_ID, it is pointer of STB ID.
*************************************************************************************/
typedef void (*MCA_USB_EVENT_CB)(MCA_USB_EVENT_t enEvtType, MCA_S32 status, MCA_VOID *pData);

MCA_S32 MCA_USB_RegisterEvent(MCA_USB_EVENT_t type, MCA_USB_EVENT_CB cb);

/*************************************************************************************
    Function:   MCA_USB_DumpSTBID
    Params:     
    Return:     
    Desc:       When HDD/USB-Pen is ready, call it to dump STB ID into HDD/USB-Pen.
*************************************************************************************/
MCA_S32 MCA_USB_DumpSTBID(MCA_VOID);

#ifdef __cplusplus
}
#endif

#endif    //__MCA_USB_H__

