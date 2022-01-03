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

#ifndef __MCA_SMARTCARD_ADAPTATION_H__
#define __MCA_SMARTCARD_ADAPTATION_H__

#ifdef __cplusplus
extern "C " {
#endif

typedef enum
{
    MCA_SMC_IN      = 0,
    MCA_SMC_OUT     = 1,
    MCA_SMC_UNKNOWN = 2
}MCA_SMC_STATUS_t;

/*************************************************************************************
    Function:   MCASMCStatusCallback
    Params:     slot: Slot id.
                status: Smartcard status.
    Return:     No return value.
    Desc:       Smartcard status callback type.
*************************************************************************************/
typedef void (*MCASMCStatusCallback) (MCA_U8 slot, MCA_SMC_STATUS_t status);

/*************************************************************************************
    Function:   mca_smartcard_init
    Params:     None.
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Initialices the smartcard adaptation layer.
*************************************************************************************/
MCA_S32  mca_smartcard_init(MCA_VOID);

/*************************************************************************************
    Function:   mca_smartcard_reset
    Params:     slot: Slot id.
                atr: Allocated buffer where the ATR is returned.
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Resets the smartcard.
*************************************************************************************/
MCA_S32  mca_smartcard_reset(MCA_U8 slot, MCA_U8 *atr, MCA_U8 *pu8AtrLen);

/*************************************************************************************
    Function:   mca_smartcard_is_inserted
    Params:     slot: Slot id.
    Return:     MCA_TRUE if inserted, MCA_FALSE if not inserted.
    Desc:       Returns if the smartcard is or not inserted.
*************************************************************************************/
MCA_BOOL mca_smartcard_is_inserted(MCA_U8 slot);

/*************************************************************************************
    Function:   mca_smartcard_register_status
    Params:     slot: Slot id.
                cb: Callback to be called for each new status.
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Registers to receive smartcard status changes notifications..
*************************************************************************************/
MCA_S32 mca_smartcard_register_status(MCA_U8 slot, MCASMCStatusCallback cb);

/*************************************************************************************
    Function:   mca_smartcard_unregister_status
    Params:     slot: Slot id.
                cb: Callback to unregister.
    Return:     MCA_SUCCESS if OK, other if error.
    Desc:       Unregisters to receive smartcard status changes notifications..
*************************************************************************************/
MCA_S32 mca_smartcard_unregister_status(MCA_U8 slot, MCASMCStatusCallback cb);

/*************************************************************************************
    Function:   mca_smartcard_T0_command
    Params:     slot      : Slot id.
                pu8Send   : Command data.
                u8SendLen : Data length.                
                pu8Rev    : Allocated buffer to store the answer.
                pu16RevLen: In parameter specifies the answer length expected.
    Return:     MCA_SUCCESS if OK.
    Desc:       Sends a command to the smartcard.
*************************************************************************************/
MCA_S32 mca_smartcard_T0_command(MCA_U8 slot, MCA_U8 *pu8Send, MCA_U8 u8SendLen, MCA_U8 *pu8Rev, MCA_U16 *pu16RevLen);

/*************************************************************************************
    Function:   mca_smartcard_T14_command
    Params:     slot      : Slot id.
                pu8Send   : Command data.
                u8SendLen : Data length.                
                pu8Rev    : Allocated buffer to store the answer.
                pu16RevLen: In parameter specifies the answer length expected.
    Return:     MCA_SUCCESS if OK.
    Desc:       Sends a command to the smartcard.
*************************************************************************************/
MCA_S32 mca_smartcard_T14_command(MCA_U8 slot, MCA_U8 *pu8Send, MCA_U8 u8SendLen, MCA_U8 *pu8Rev, MCA_U16 *pu16RevLen);

#ifdef __cplusplus
}
#endif

#endif  //__MCA_SMARTCARD_ADAPTATION_H__

