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
    MCA_M00_FTA_SERVICE,                /* M00 FTA Service. */                          // �Ǽ��ܽ�Ŀ
    MCA_M01_NO_SMC,                     /* M01 Please insert smart card. */             // ��������ܿ�
    MCA_M02_UNKNOWN_SMC,                /* M02 Unknown smart card. */                   // �޷�ʶ�����ܿ�
    MCA_M03_CHECKING_SMC,               /* M03 Checking smart card. */                  // ����У�����ܿ�
    MCA_M04_NO_SERVICE,                 /* M04 No Service Avaliable. */                 // �޽�Ŀ
    MCA_M05_SLAVE_CARD_VALID,           /* M05 Slave Card Valid. */                     // �ӿ���֤�ɹ�
    MCA_M06_SMC_READY,                  /* M06 Smart card is ready. */                  // ���ܿ�OK
    MCA_M07_SLAVE_CARD_INVALID,         /* M07 Slave Card Invalid. */                   // �ӿ���֤ʧ��
    MCA_M08_MASTER_CARD_INVALID,        /* M08 Master Card Invalid. */                  // ĸ����֤ʧ��
    MCA_M09,
    MCA_M10_SCRAMBLED_SERVICE,          /* M10 Service Is Currently Scrambled. */       // �޷�ʶ��ļ��ܽ�Ŀ    
    MCA_M11_MISMATCHING_REGION,         /* M11 Not Allowed In This Region. */           // �����벻ƥ��
    MCA_M12_SMC_PAIRING_FAIL,           /* M12 Smart card is not pairing to this STB. */// ������ƥ��    
    MCA_M13_LOW_PARENTAL_CONTROL_LEVEL, /* M13 Parental Control Lock. */                // ��ĸ������̫��    
    MCA_M14_CW_ERROR,                   /* M14 CW Error. */                             // �����ִ���    
    MCA_M15_NO_AUTHORIZATION,           /* M15 No Authorization. */                     // û�н�����Ȩ    
    MCA_M16_DESCRAMBLING,               /* M16 Descrambling works well. */              // �ɹ�����    
    MCA_M17_SLAVE_CARD_EXPIRE,          /* M17 Slave Card Expire. */                    // �ӿ���֤����    
    MCA_M18,                            /* M18 User View Query */                       // �û�����/׼������    
    MCA_M19,                            /* M19 Regional Control. */                     // �������/׼������    
    MCA_M20_IPPV_SERVICE,               /* M20 IPPV Service. */                         // IPPV��Ŀ    
    MCA_M21,                            /* M21 Program Authorization is Paused. */      // ���ܿ���Ŀ����Ȩ����ͣ    
    MCA_M22,                            /* M22 Smart card is not active. */             // ���ܿ�����δ����״̬
    MCA_M23,                                                                            // ���ܿ�������Ч��
    MCA_M24,                                                                            // ���ܿ���Ȩ����ͣ
    MCA_M25,                                                                            // ����ECM channelʧ��
    MCA_M26,                                                                            // ����ͨ������ʧ��
    MCA_M27,                                                                            // ����ʱ���ܿ�����
    MCA_M28,                                                                            // ��Ŀ״̬����
    MCA_M29,                                                                            // ����ECM filterʧ��
    MCA_M30,                                                                            // pid�л�ʧ��
    MCA_M31,
    MCA_M32,
    MCA_M33,
    MCA_M34,
    MCA_M35,
    MCA_M36,
    MCA_M37,
    MCA_M38,
    MCA_M39_SWITCH_CAS,                 /* M39 Switch CAS */
    MCA_M40,                                                                            // �û�׼�������óɹ�
    MCA_M41,                                                                            // �û�׼�������ݶ�ȡ�ɹ�
    MCA_M42,                                                                            // �û�׼��������ʱ�����
    MCA_M43,                                                                            // �û�׼��������ʱ�䳬��
    MCA_M44,                                                                            // �û�׼��������ͨ������
    MCA_M45,                                                                            // �û�׼��������ģʽ����
    MCA_M46,                                                                            // �û�׼��������PIN�����
    MCA_M47,                                                                            // �û�׼��������ʧ��
    MCA_M48,
    MCA_M49,
    MCA_M50_IPPV_ON_DEMAND_SUCCESS,     /* M50 IPPV on demand Success. */               // IPPV�㲥�ɹ�
    MCA_M51_IPPV_PASSWORD_Error,        /* M51 IPPV Password Error. */                  // IPPV�������
    MCA_M52_IPPV_PROGRAM_IS_OVERDUE,    /* M52 IPPV program is Overdue. */              // IPPV��Ŀ����
    MCA_M53_IPPV_BALANCE_IS_NOT_ENOUGH, /* M53 IPPV balance is not enough. */           // IPPV�㲥����
    MCA_M54_IPPV_MODE_IS_NOT_SUPPORTED, /* M54 IPPV Mode is not supported. */           // IPPV�㲥��ʽ��֧��
    MCA_M55_SET_IPPV_ON_DEMAND_FAIL,    /* M55 Set IPPV on demand Fail. */              // IPPV�㲥ʧ��
    MCA_M56_IPPV_END_SUCCESS,           /* M56 IPPV End Success. */                     // IPPV�����ɹ�
    MCA_M57_IPPV_END_FAIL,              /* M57 IPPV End Fail. */                        // IPPV����ʧ��
    MCA_MXX_UNKNOWN
}MCA_MESSAGE_CODE_t;

#ifdef __cplusplus
}
#endif

#endif    //__MCA_MESSAGE_CODE_H__

