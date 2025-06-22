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

#ifndef __MCA_MESSAGE_CODE_H__
#define __MCA_MESSAGE_CODE_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef enum
{
    MCA_M00_FTA_SERVICE,                /* M00 FTA Service. */                          // 非加密节目
    MCA_M01_NO_SMC,                     /* M01 Please insert smart card. */             // 请插入智能卡
    MCA_M02_UNKNOWN_SMC,                /* M02 Unknown smart card. */                   // 无法识别智能卡
    MCA_M03_CHECKING_SMC,               /* M03 Checking smart card. */                  // 正在校验智能卡
    MCA_M04_NO_SERVICE,                 /* M04 No Service Avaliable. */                 // 无节目
    MCA_M05_SLAVE_CARD_VALID,           /* M05 Slave Card Valid. */                     // 子卡认证成功
    MCA_M06_SMC_READY,                  /* M06 Smart card is ready. */                  // 智能卡OK
    MCA_M07_SLAVE_CARD_INVALID,         /* M07 Slave Card Invalid. */                   // 子卡认证失败
    MCA_M08_MASTER_CARD_INVALID,        /* M08 Master Card Invalid. */                  // 母卡验证失败
    MCA_M09,
    MCA_M10_SCRAMBLED_SERVICE,          /* M10 Service Is Currently Scrambled. */       // 无法识别的加密节目    
    MCA_M11_MISMATCHING_REGION,         /* M11 Not Allowed In This Region. */           // 区域码不匹配
    MCA_M12_SMC_PAIRING_FAIL,           /* M12 Smart card is not pairing to this STB. */// 机卡不匹配    
    MCA_M13_LOW_PARENTAL_CONTROL_LEVEL, /* M13 Parental Control Lock. */                // 父母锁级别太低    
    MCA_M14_CW_ERROR,                   /* M14 CW Error. */                             // 控制字错误    
    MCA_M15_NO_AUTHORIZATION,           /* M15 No Authorization. */                     // 没有解密授权    
    MCA_M16_DESCRAMBLING,               /* M16 Descrambling works well. */              // 成功解扰    
    MCA_M17_SLAVE_CARD_EXPIRE,          /* M17 Slave Card Expire. */                    // 子卡认证过期    
    MCA_M18,                            /* M18 User View Query */                       // 用户禁播/准播控制    
    MCA_M19,                            /* M19 Regional Control. */                     // 区域禁播/准播控制    
    MCA_M20_IPPV_SERVICE,               /* M20 IPPV Service. */                         // IPPV节目    
    MCA_M21,                            /* M21 Program Authorization is Paused. */      // 智能卡节目商授权被暂停    
    MCA_M22,                            /* M22 Smart card is not active. */             // 智能卡处于未激活状态
    MCA_M23,                                                                            // 智能卡超出有效期
    MCA_M24,                                                                            // 智能卡授权被暂停
    MCA_M25,                                                                            // 分配ECM channel失败
    MCA_M26,                                                                            // 解扰通道分配失败
    MCA_M27,                                                                            // 解扰时智能卡错误
    MCA_M28,                                                                            // 节目状态错误
    MCA_M29,                                                                            // 分配ECM filter失败
    MCA_M30,                                                                            // pid切换失败
    MCA_M31,
    MCA_M32,
    MCA_M33,
    MCA_M34,
    MCA_M35,
    MCA_M36,
    MCA_M37,
    MCA_M38,
    MCA_M39_SWITCH_CAS,                 /* M39 Switch CAS */
    MCA_M40,                                                                            // 用户准禁播设置成功
    MCA_M41,                                                                            // 用户准禁播数据读取成功
    MCA_M42,                                                                            // 用户准禁播设置时间错误
    MCA_M43,                                                                            // 用户准禁播控制时间超长
    MCA_M44,                                                                            // 用户准禁播设置通道错误
    MCA_M45,                                                                            // 用户准禁播设置模式错误
    MCA_M46,                                                                            // 用户准禁播设置PIN码错误
    MCA_M47,                                                                            // 用户准禁播设置失败
    MCA_M48,
    MCA_M49,
    MCA_M50_IPPV_ON_DEMAND_SUCCESS,     /* M50 IPPV on demand Success. */               // IPPV点播成功
    MCA_M51_IPPV_PASSWORD_Error,        /* M51 IPPV Password Error. */                  // IPPV密码错误
    MCA_M52_IPPV_PROGRAM_IS_OVERDUE,    /* M52 IPPV program is Overdue. */              // IPPV节目过期
    MCA_M53_IPPV_BALANCE_IS_NOT_ENOUGH, /* M53 IPPV balance is not enough. */           // IPPV点播金额不足
    MCA_M54_IPPV_MODE_IS_NOT_SUPPORTED, /* M54 IPPV Mode is not supported. */           // IPPV点播方式不支持
    MCA_M55_SET_IPPV_ON_DEMAND_FAIL,    /* M55 Set IPPV on demand Fail. */              // IPPV点播失败
    MCA_M56_IPPV_END_SUCCESS,           /* M56 IPPV End Success. */                     // IPPV结束成功
    MCA_M57_IPPV_END_FAIL,              /* M57 IPPV End Fail. */                        // IPPV结束失败
    MCA_MXX_UNKNOWN
}MCA_MESSAGE_CODE_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_MESSAGE_CODE_H__

