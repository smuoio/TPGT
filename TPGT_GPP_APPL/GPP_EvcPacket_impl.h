/**
* \file GPP_EvcPacket_impl.h
* \brief the file manages the functions private of GPP to extract the packets EVC
*        the funcions are customized for EBICAB2000 ver.P8
* \author Muoio Salvatore
*/

#ifndef __GPP_EVC_PACKET_IMPL_H__
#define __GPP_EVC_PACKET_IMPL_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "CCUT_Util.h"
#include "GPP_ParserPacket.h"



/****** FUNCTIONS ******/

/**
* \fn void Create_MmiStartAtp_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Start Atp(EVC-0)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiStartAtp_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiStatus_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Status(EVC-2)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiStatus_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiSetTimeAtp_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Set Time Atp(EVC-3)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiSetTimeAtp_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiTrackDescription_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Track Description(EVC-4)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiTrackDescription_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiTrackDescription_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Track Description(EVC-6)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiCurrentTrainData_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiDriverMessage_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Driver Message(EVC-8)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiDriverMessage_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiFailureReportAtp_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Driver Message(EVC-8)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiFailureReportAtp_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiCurrentDriverData_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Enable Driver Request(EVC-21)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiCurrentDriverData_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiEnableDriverRequest_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Enable Driver Request(EVC-21)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiEnableDriverRequest_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MMiSpecificStmDeRequest_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Specific De Request(EVC-25)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MMiSpecificStmDeRequest_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Create_MmiSpecificStmDwValues_(const t_FFFISPacket * const lFFFISPacket)
* \brief The function creates the MMI Specific De Request(EVC-26)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Create_MmiSpecificStmDwValues_(const t_FFFISPacket * const lFFFISPacket);
/**
* \fn void Build_MmiStartMmi_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the MMI Start MMI(EVC-100) and after store the
*        packet in the queue to put out
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiStartMmi_(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiStatusReport_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the MMI Status Report MMI(EVC-102) and after store the
*        packet in the queue to put out
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiStatusReport_(t_FFFISMessage * const lFFFISMessage);

void Build_MmiRequest_(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiNewDriverData_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the new data from the driver MMI(EVC-104) and after store the
*        packet in the queue to put out
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiNewDriverData_(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiDriverMessageAck_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the acknowlodge or not acknowlodge (EVC-111)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiDriverMessageAck_(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiSpecificStmDataToStm_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the packet  (EVC-123)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiSpecificStmDataToStm_(t_FFFISMessage * const lFFFISMessage);
/**
* \fn void Build_MmiSetTimeMmi_(t_FFFISMessage * const lFFFISMessage)
* \brief The function creates the sends the packet  (EVC-109)
* \param lFFFISPacket pointer to the FFFIS packet
*/
void Build_MmiSetTimeMmi_(t_FFFISMessage * const lFFFISMessage);

#ifdef __cpluplus
}
#endif

#endif
