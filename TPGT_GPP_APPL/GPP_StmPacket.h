/**
* \file GPP_StmPacket.h
* \brief the file manages the functions private of GPP to extract the packets STM
* \author Muoio Salvatore
*/

#ifndef __GPP_STMPACKET_H__
#define __GPP_STMPACKET_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "usertypes.h"
/**
*\enum e_Stm_Function
*\brief enumeration of stm function used in lookup LookUpTaable
*/
typedef enum
{
  STM_Dummy, /*!<stm dummy */
  STM_VersionNumber,/*!<stm version number */
  STM_StateReport,/*!<stm state report */
  STM_ButtonRequest,/*!<stm button request */
  STM_MmiStmTextMessage,/*!<stm text message */
  STM_MmiStmDeleteTextMessage,/*!<stm delete text message */
  STM_MmiStmSoundCmd,/*!<stm sound */
  STM_MmiStmIndicatorRequest,/*!<stm icons */
}e_Stm_Function;


/**
*\fn void GPP_ExtractStmPacket(void)
*\brief the function manages the state machine used to call the function to
*        extract the packets
*/
void GPP_ExtractStmPacket(void);
/**
*\fn void GPP_BuildStmPacket(UINT8* const pTg, UINT8 aNidStm)
*\brief the function manages the functions used to create the packets to put out
*\param pTg pointer to out raw data
*/
void GPP_BuildStmPacket(UINT8* const pTg, UINT8 aNidStm);

#ifdef __cplusplus
}
#endif






#endif
