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

#ifndef __MCA_APP_COMMON_H__
#define __MCA_APP_COMMON_H__

#ifdef __cplusplus
extern "C " {
#endif

extern MCA_CHAR aszCASName[][16];
extern MCA_CHAR aszCASMessage[][44];

#define MCA_APP_DBG(fmt, args...)   \
    do{    \
        mca_printf("----mca app->[%s]Line%d: ", __FUNCTION__, __LINE__);    \
        mca_printf(fmt, ##args);    \
    }while(0)

MCA_S32 mca_app_common_init(MCA_VOID);
MCA_S32 mca_app_query_smc_info(MCA_VOID);
MCA_S32 mca_app_query_subscription_info(MCA_VOID);
MCA_S32 mca_app_check_pin(MCA_VOID);
MCA_S32 mca_app_change_pin(MCA_VOID);
MCA_S32 mca_app_set_maturity_rating(MCA_VOID);
MCA_S32 mca_app_view_parental_channel(MCA_VOID);
MCA_S32 mca_app_query_ppv_info(MCA_VOID);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_APP_COMMON_H__

