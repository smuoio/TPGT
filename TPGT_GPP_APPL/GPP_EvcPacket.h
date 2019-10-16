/**
* \file GPP_EvcPacket.h
* \brief the file manages the functions private of GPP to extract the packets EVC
* \author Muoio Salvatore
*/

#ifndef __GPP_EVC_PACKET_H__
#define __GPP_EVC_PACKET_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "usertypes.h"

/**
*\fn void GPP_ExtractEvcPacket(UINT8 aNidStm)
*\brief the function manages the state machine used to call the function to
*        extract the packets
*\param aNidStm Nid_STM used for plausibility
*/
void GPP_ExtractEvcPacket(UINT8 aNidStm);
/**
*\fn void GPP_BuildEvcPacket(UINT8* const pTg)
*\brief the function manages the functions used to create the packets to put out
*\param pTg pointer to out raw data
*/
void GPP_BuildEvcPacket(UINT8* const pTg);
#ifdef __cplusplus
}
#endif

#endif
