/**
* \file GPP_stmPacket_impl.h
* \brief the file manages the functions private of GPP to extract the packets STM
* \author Muoio Salvatore
*/

#ifndef __GPP_STMPACKET_IMPL_H__
#define __GPP_STMPACKET_IMPL_H__


#include "CCUT_Util.h"
#include "GPP_ParserPacket.h"

/****** FUNCTIONS ******/
/**
* \fn void Create_VersionNumber_(const t_StmPacket * const lStmPacket)
* \brief The function creates the version number (STM-1)
* \param lStmPacket pointer to the Stm packet
*/
void Create_VersionNumber_(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_StateReport_(const t_StmPacket * const lStmPacket)
* \brief The function creates the version number (STM-15)
* \param lStmPacket pointer to the Stm packet
*/
void Create_StateReport_(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_ButtonRequest_(const t_StmPacket * const lStmPacket);
* \brief The function creates or update the visual states
*        of buttons by STM(STM-32)
* \param lStmPacket pointer to the Stm packet
*/
void Create_ButtonRequest_(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmTextMessage_(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the visual states
*        of buttons by STM(STM-38)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmTextMessage_(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmDeleteTextMessage(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the visual states
*        of buttons by STM(STM-39)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmDeleteTextMessage_(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmSoundCmd_(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the sound palettes by STM(STM-46)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmSoundCmd_(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmIndicatorRequest_(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the sound palettes by STM(STM-35)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmIndicatorRequest_(const t_StmPacket * const lStmPacket);
/**
* \fn void Build_StmVersionNumber_(const t_StmPacket * const lStmPacket)
* \brief The function creates the version number (STM-1)
* \param lStmPacket pointer to the Stm packet
*/
void Build_StmVersionNumber_(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiStmButtonEventReport_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates or update the sound palettes by STM(STM-34)
* \param lStmPacket pointer to the Stm packet
*/
void Build_MmiStmButtonEventReport_(t_FFFISMessage * const lFFFISMessage);






#endif
