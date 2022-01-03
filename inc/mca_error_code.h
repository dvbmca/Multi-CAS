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

#ifndef __MCA_ERROR_CODE_H__
#define __MCA_ERROR_CODE_H__

#ifdef __cplusplus
extern "C " {
#endif

#define MCA_E01_UNKNOWN                 0x80000001
#define MCA_E02_WRONG_PIN               0x80000002
#define MCA_E03_WRONG_MATURITY_RATING   0x80000003
#define MCA_E04_WRONG_PPID              0x80000004
#define MCA_E05_INSUFFICIENT_BALANCE    0x80000005

#ifdef __cplusplus
}
#endif

#endif    //__MCA_ERROR_CODE_H__

