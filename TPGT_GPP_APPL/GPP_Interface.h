/**
* \file GPP_Interface.h
* \brief the file manages the interface with any device of signalling that it uses
*        the link layer MVB and Protocol layer GPP
* \author Muoio Salvatore
*/

#ifndef __GPP_EVC_INTERFACE_H__
#define __GPP_EVC_INTERFACE_H__

#include "GPP_ParserPacket.h"

/**
* \fn void Create_MmiStartAtp(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Start Atp(EVC-0)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiStartAtp(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiStatus(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Status(EVC-2)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiStatus(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiSetTimeAtp(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Set Time Atp(EVC-3)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiSetTimeAtp(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiTrackDescription(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Track Description(EVC-4)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiTrackDescription(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiTrackDescription(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Track Description(EVC-6)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiCurrentTrainData(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiDriverMessage(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Driver Message(EVC-8)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiDriverMessage(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiFailureReportAtp(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Driver Message(EVC-8)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiFailureReportAtp(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiEnableDriverRequest(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Enable Driver Request(EVC-21)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiEnableDriverRequest(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MMiSpecificStmDeRequest(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Specific Stm De Request(EVC-25)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MMiSpecificStmDeRequest(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiSpecificStmDwValues(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Specific De Request(EVC-26)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiSpecificStmDwValues(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiCurrentDriverData(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Specific (EVC-14)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiCurrentDriverData(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiStmIndicatorRequest(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the sound palettes by STM(STM-35)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmIndicatorRequest(const t_StmPacket * const lStmPacket);
/**
* \fn void Build_MmiStartMmi(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the MMI Start MMI(EVC-100) and after store the
*        packet in the queue to put out
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiStartMmi(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiStatusReport(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the MMI Status Report MMI(EVC-102) and after store the
*        packet in the queue to put out
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiStatusReport(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Interface_Init(void)
* \brief The function initializes the interfaces
*/
void Interface_Init(void);
/**
* \fn void Create_VersionNumber(const t_StmPacket * const lStmPacket)
* \brief The function creates the version number (STM-1)
* \param lStmPacket pointer to the Stm packet
*/
void Create_VersionNumber(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_StateReport(const t_StmPacket * const lStmPacket)
* \brief The function creates the version number (STM-15)
* \param lStmPacket pointer to the Stm packet
*/
void Create_StateReport(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_ButtonRequest(const t_StmPacket * const lStmPacket);
* \brief The function creates or update the visual states
*        of buttons by STM(STM-32)
* \param lStmPacket pointer to the Stm packet
*/
void Create_ButtonRequest(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmTextMessage(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the visual states
*        of buttons by STM(STM-38)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmTextMessage(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmDeleteTextMessage(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the visual states
*        of buttons by STM(STM-39)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmDeleteTextMessage(const t_StmPacket * const lStmPacket);
/**
* \fn void Create_MmiStmSoundCmd(const t_StmPacket * const lStmPacket)
* \brief The function creates or update the sound palettes by STM(STM-46)
* \param lStmPacket pointer to the Stm packet
*/
void Create_MmiStmSoundCmd(const t_StmPacket * const lStmPacket);


void Build_MmiRequest(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiNewDriverData(t_FFFISMessage * const lFFFISMessage);
* \brief The function creates or update the vthe new data from the driver
* \param lStmPacket pointer to the Stm packet
*/
void Build_MmiNewDriverData(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiDriverMessageAck(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the acknowlodge or not acknowlodge (EVC-111)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiDriverMessageAck(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiSpecificStmDataToStm(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the packet  (EVC-123)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiSpecificStmDataToStm(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_StmVersionNumber(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the packet  (EVC-123)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_StmVersionNumber(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiStmButtonEventReport_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates or update the sound palettes by STM(STM-34)
* \param lStmPacket pointer to the Stm packet
*/
void Build_MmiStmButtonEventReport(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiSetTimeMmi(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the packet  (EVC-109)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiSetTimeMmi(t_FFFISMessage * const lFFFISMessage);
#endif
