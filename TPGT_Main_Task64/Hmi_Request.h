/**
* \file Hmi_Request.h
* \brief the file manages the private functions of hmi regarding
*        the request of drive
* \author Muoio Salvatore
*/


#ifndef __HMI_REQUEST_H__
#define __HMI_REQUEST_H__

#include "Hmi_Handle_impl.h"

/**
* \enum e_TpgCommand
* \brief definition of values regarding the message id of TPG_Command
*/
typedef enum
{
  NO_TPG_COMMAND      = 0,  /*!<Value no message id*/
  EDIT_LZB_PZB_DATA   = 7,  /*!<Value EDIT_LZB_PZB_DATA*/
  TEST_SEL_BY_DRIVER  = 16, /*!<Value TEST_SEL_BY_DRIVER*/
  MESSAGE_ACK_REQUEST = 19, /*!<Value MESSAGE_ACK_REQUEST*/
  VIEW_LZB_PZB_DATA   = 21, /*!<Value VIEW_LZB_PZB_DATA*/
}e_TpgCommand;

/**
* \enum }e_TptSelectedTest;
* \brief definition of values regarding the message id of TPT_SelectedTest
*/
typedef enum
{
  NO_SELECTED, /*!<Value no test selected*/
  PZB_SELECTED,/*!<Value PZB_SELECTED*/
  BRAKE_TEST,  /*!<Value BRAKE_TEST*/
}e_TptSelectedTest;
/**
* \fn e_result EnableZDERequest(const TPG_Commands * const pCmd,
*                               DS_CCUT_HMI_TRN *const pout);
* \brief the function checks if the drives pressed the ZDE button, enables the request
* \param pCmd const pointer to the const struct TPG_Commands, pointer used
*        only to read the data
* \param pout const pointer to the output structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableZDERequest(const TPG_Commands * const pCmd, DS_CCUT_HMI_TRN *const pout);
/**
* \fn e_result EnableZDRequest(const TPG_Commands * const pCmd, DS_CCUT_HMI_TRN *const pout)
* \brief the function checks if the drives pressed the ZD button, enables the request
* \param pCmd const pointer to the const struct TPG_Commands, pointer used
*        only to read the data
* \param pout const pointer to the output structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableZDRequest(const TPG_Commands * const pCmd, DS_CCUT_HMI_TRN *const pout);
/**
* \fn e_result EnableSBVRequest(const TPG_Commands * const pCmd,
*                               const TPT_SelectedTest *const pSelTest,
*                               DS_CCUT_HMI_TRN *const pout)
* \brief the function checks if the drives pressed the SB_V button, enables the request
* \param pCmd const pointer to the const struct TPT_SelectedTest, pointer used
*        only to read the data
* \param pSelTest  const pointer to the const struct TPG_Commands, pointer used
*        only to read the data
* \param pout const pointer to the output structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableSBVRequest(const TPG_Commands * const pCmd,
                          const TPT_SelectedTest *const pSelTest,
                          DS_CCUT_HMI_TRN *const pout);
/**
* \fn e_result EnableBlockSTMemoryRequest(const TPT_JRUCommand * const pCmd,
*                                    const JRU_STATUS * const pJruSts,
*                                    MMI_AUX_OUT *const pMmiAuxOut);
* \brief the function checks if the drives pressed the SB_V button, enables the request
* \param pCmd const pointer to the const struct TPT_SelectedTest, pointer used
*        only to read the data
* \param pJruSts  const pointer to the const struct JRU_STATUS, pointer used
*        only to read the data
* \param pMmiAuxOut const pointer to the output structure MMI_AUX_OUT
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableBlockSTMemoryRequest(const TPT_JRUCommand * const pCmd,
                                    const JRU_STATUS_IP * const pJruSts,
                                    MMI_AUX_OUT *const pMmiAuxOut);
/**
* \fn e_result EnableNewTrainNumberRequest(const TPT_Uint32Valid * const pCmd)
* \brief the function checks if the the request about the new train number request
* \param pCmd const pointer to the const struct TPT_Uint32Valid, pointer used
*        only to read the data
* \param pout const pointer to the output structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableNewTrainNumberRequest(const TPT_Uint32Valid * const pCmd);
/**
* \fn e_result EnableDriverIdRequest(const TPT_Uint32Valid * const pCmd)
* \brief the function checks if the new Driver Id request is enable
* \param pCmd const pointer to the const struct TPT_DriverId, pointer used
*        only to read the data
* \param pout const pointer to the output structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableDriverIdRequest(const TPT_Uint32Valid * const pCmd);
/**
* \fn e_result EnableDriverMessageAckdRequest(const TPG_Commands * const pCmd,
*                                             const TPT_TextMessageApproval *const pMsg)
* \brief the function sends to Ebicab the logical aknowledge or not Acknowledge
* \param pCmd const pointer to the const struct TPG_Commands, pointer used
*        only to read the data
* \param pMsg const pointer to the TPT_TextMessageApproval structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableDriverMessageAckdRequest(const TPG_Commands * const pCmd,
                                        const TPT_TextMessageApproval *const pMsg);
/**
* \fn e_result EnableButtonEventReport(const TPG_Commands * const pCmd,
*                                      const TPT_SelectedTest *const pTst)
* \brief the function sends to Ebicab the request of STM 34
* \param pCmd const pointer to the const struct TPG_Commands, pointer used
*        only to read the data
* \param pTst const pointer to the TPT_SelectedTest structure
* \return false if the pCmd id out of range, viceversa TRUE
*/
e_result EnableButtonEventReport(const TPG_Commands * const pCmd,
                                 const TPT_SelectedTest *const pTst);
/**
* \fn BOOLEAN8 CheckMaskRequestBusy(void)
* \brief the function checks if there are request for the mask
* \return TRUE if mask request is busy, FALSE viceversa
*/
BOOLEAN8 CheckMaskRequestBusy(void);
/**
* \fn void Hmi_Request_init(void)
* \brief the init function of HMI Request sw component
*/
void Hmi_Request_init(void);

#endif
