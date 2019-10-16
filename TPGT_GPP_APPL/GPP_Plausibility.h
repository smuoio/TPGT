/**
* \file GPP_Plausibility.h
* \brief the file manages the functions private of GPP regarding the plausibility of packet
* \author Muoio Salvatore
*/


#ifndef __GPP_PLAUSIBILITY_H__
#define __GPP_PLAUSIBILITY_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include "GPP_Types.h"


/**
* \fn gpp_result GPP_CheckPacketLength(UINT16 aPcktLen);
* \brief function checks the length of whole raw buffer
* \param aPcktLen: length read from the packet received
* \return 0 success or number and type of error
*/
gpp_result GPP_CheckPacketLength(UINT16 aPcktLen);
/**
* \fn gpp_result GPP_CheckFrameLength(UINT16 aPcktLen, const UINT8 *const pTg);
* \brief function implements the plausibility of length of whole message, in
 *       FFFIS Message header
* \param aPcktLen: id to be sure to manage specific data
* \param pTg: pointer to FFFIS packet
* \return 0 success or number and type of error
*/
gpp_result GPP_CheckFrameLength(UINT16 aPcktLen, const UINT8 *const pTg);
/**
* \fn gpp_result GPP_CheckEvcNid(UINT8 aNidStm, UINT8 aNidPacket)
* \brief function implements the plausibility of NID_Packet
* \param aNidStm: id of FFFIS MEssage Header
* \param aNidPacket: id of FFFIS packet header
* \return 0 success or number and type of error
*/
gpp_result GPP_CheckEvcNid(UINT8 aNidStm, UINT8 aNidPacket);
/**
* \fn gpp_result GPP_CheckEOStream(UINT8 aEOStr);
* \brief function implements the plausibility of end of of stream delimeter
* \param aEOStr: byte read after the evc packet data
* \return 0 success or number and type of error
*/
gpp_result GPP_CheckEOStream(UINT8 aEOStr);
/**
* \fn gpp_result GPP_CheckLenEvcPcktData(const UINT16 aLenPckt, const UINT8 *const aLen)
* \brief function implements the plausibility of length for each data packet
* \param aLenPckt: length fixed for each packet
* \param aLen: length extract from the packet
* \return 0 success or number and type of error
*/
gpp_result GPP_CheckLenEvcPcktData(const UINT16 aLenPckt, const UINT8 *const aLen);
/**
* \fn gpp_result GPP_CheckStmFrame(UINT8 *aPartLen, UINT8 aLenMsg);
* \brief function implements the plausibility of length of stm frame
* \param *aPartLen: partial length, is the update after each packet extract
* \param aLenMsg: length of stm frame
* \return 0 success or number and type of error
*/
gpp_result GPP_CheckStmFrame(UINT8 *aPartLen, UINT8 aLenMsg);

#endif

#ifdef __cplusplus
}
#endif
