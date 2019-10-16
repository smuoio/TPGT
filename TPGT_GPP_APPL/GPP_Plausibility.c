/**
* \file GPP_Plausibility.c
* \brief the file manages the functions private of GPP regarding the plausibility of packet
* \author Muoio Salvatore
*/

#include "GPP_Plausibility.h"
#include "CCUT_Util.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define GPP_MIN_LENGTH 0x2U
#define GPP_MAX_LENGTH 0x4B0

gpp_result GPP_CheckPacketLength(UINT16 aPcktLen)
{
	gpp_result lRes = GPP_SUCCESS;
	if (aPcktLen > GPP_MAX_LENGTH)
		lRes = GPP_BAD_PACKET_MAX_LENGTH;
	else if(aPcktLen < GPP_MIN_LENGTH)
		lRes = GPP_BAD_PACKET_MIN_LENGTH;
	return (lRes);
}

gpp_result GPP_CheckFrameLength(UINT16 aPcktLen, const UINT8 *const pTg)
{
	const INT16 L_MSG = 0x1;
	gpp_result lOut = GPP_BAD_PARAM;
	if(pTg != 0)
	{
		lOut = GPP_BAD_FRAME_LENGTH;
		if(pTg[L_MSG] == aPcktLen)
			lOut = GPP_SUCCESS;
	}
	return(lOut);
}

gpp_result GPP_CheckEvcNid(UINT8 aNidStm, UINT8 aNidPacket)
{
	gpp_result lOut = GPP_SUCCESS;
	const INT16 EvcMmi = 213;
	const INT16 MmiEvc = 214;
	if(aNidStm == NID_TSG)
	{
		if((aNidPacket != EvcMmi) &&
		   (aNidPacket != MmiEvc))
			lOut = GPP_BAD_EVC_PLS;
	}/*if*/
	return(lOut);
}


gpp_result GPP_CheckEOStream(UINT8 aEOStr)
{
	const UINT8 EndOfStream = 0xFF;
	/* check if the input is the end of streamin delimiter */
	return((aEOStr == EndOfStream) ? GPP_SUCCESS : GPP_BAD_END_STREAMING);
}

gpp_result GPP_CheckLenEvcPcktData(const UINT16 aLenPckt, const UINT8 *const aLen)
{
	UINT16 lLen = 0x0U;
	if(aLen != 0)
	{
		memcpy((UINT16*)&lLen, (UINT8*)aLen, sizeof(UINT16));
		/* convert bits in byte*/
		lLen = ConvBit2Byte(lLen);
	}
	/* compare and return the result*/
	return ((aLenPckt == lLen) ? GPP_SUCCESS : GPP_BAD_EVC_DATA_LEN);
}

gpp_result GPP_CheckStmFrame(UINT8 *aPartLen, UINT8 aLenMsg)
{
	gpp_result lOut = GPP_BAD_PARAM;
	const UINT16 message_header = 0x2U;
	/* the lengths are in byte */
	if(aPartLen != 0)
	{
		if((aLenMsg - *aPartLen - message_header) <= 1)
		{
			/* we have to exit from the while, because the last byte is used for
		   	the padding*/
		 	*aPartLen = aLenMsg;
		}/**/
		lOut = GPP_SUCCESS;
	}/**/
	return(lOut);
}

#ifdef __cplusplus
}
#endif
