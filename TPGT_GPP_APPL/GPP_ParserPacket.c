/**
* \file GPP_ParserPacket.c
* \brief the file manages the functions private of GPP to parser the packets
* \author Muoio Salvatore
*/

#include "GPP_ParserPacket.h"
#include "GPP_Plausibility.h"
#include "GPP_EvcPacket.h"
#include "GPP_StmPacket.h"
#include "Shared_Memory.h"
#include "CCUT_Util.h"

/* static variables*/
/**
*\var t_FFFISPacket FFFISPacket
*\brief object used to build the FFFISPacket. The packet is used for the parser
*/
static t_FFFISPacket FFFISPacket;
/**
*\var t_StmPacket StmPacket
*\brief object used to build the StmPacket. The packet is used for the parser
*/
static t_StmPacket StmPacket;

/* static public interface !!!*/

t_FFFISPacket *GetFFFISPacket(void)
{
	return(&FFFISPacket);
}

t_StmPacket *GetStmPacket(void)
{
	return(&StmPacket);
}

/**
*\fn static UINT16 GetPacketLen(UINT8 *pLen)
*\brief the function convert in bit the data in the packet. The data are in two bytes
*\param pLen pointer to input buffer
*\return the legth of packet
*/
static UINT16 GetPacketLen(UINT8 *pLen)
{
	UINT16 lLeng = 0;
	if(pLen != 0)
	{
		/* the length of packet is in bits*/
		memcpy((UINT16*)&lLeng,(UINT8*)(pLen), sizeof(UINT16));
		/*lLeng = pLen[1];
		lLeng = (lLeng << 8)|pLen[0];*/

	}/*if*/
	return(lLeng);
}
/**
*\fn static void Create_NidPacket(const UINT8* const pNidPacket, UINT8 * const aNidPacket)
*\brief the function returns the NID packet
*\param pNidPacket pointer to input buffer
*\param aNidPacket pointer to the copy buffer used to extract the packet
*/
static void Create_NidPacket(const UINT8* const pNidPacket, UINT8 * const aNidPacket)
{
	if((pNidPacket != 0) && (aNidPacket != 0))
	{
		*aNidPacket = *pNidPacket;
		ae_write(
							AE_POSITION,
							"aNidPacket = %d", *aNidPacket);
	}/*if*/
}

/**
*\fn static void Create_LenPacket(const UINT8* const pLPacket, UINT8 * const aLPacket)
*\brief the function returns the length of packet in bits. The length is on 13 bits
*\param pLPacket pointer to input buffer
*\param aLPacket pointer to the copy buffer used to extract the packet
*/
static void Create_LenPacket(const UINT8* const pLPacket, UINT8 * const aLPacket)
{
	if((pLPacket != 0) && (aLPacket != 0))
	{
		UINT16 lPacket = 0x0U;
		/* the data valid are only 13 bits: the length of packet is in bits*/
		lPacket = ((pLPacket[0] << 5) | (pLPacket[1]>>3));
		memcpy((UINT8*)aLPacket, (UINT16*)(&lPacket), sizeof(UINT16));
		ae_write(
							AE_POSITION,
							"lPacket = %d", lPacket);
	}/*if*/
}

gpp_result GPP_GetNidStm(const UINT8 * const pTg, UINT8 * const pNidStm)
{
	gpp_result lOut = GPP_BAD_PARAM;
	if((pTg != 0) && (pNidStm != 0)) /* plausibility chek pointers */
	{
		*pNidStm = pTg[0];
		lOut = GPP_SUCCESS;
		ae_write(
							AE_POSITION,
							"*pNidStm = %d", *pNidStm);

	}
	return (lOut);
}

gpp_result GPP_GetLMsgStm(const UINT8 * const pTg, UINT8 * const pLMsg)
{
	gpp_result lOut = GPP_BAD_PARAM;
	if((pTg != 0) && (pLMsg != 0)) /* plausibility chek pointers */
	{
		*pLMsg = *pTg;
		lOut = GPP_SUCCESS;
	}
	return (lOut);
}
/****** EVC/TSG Packet *********/
/**
*\fn static void Create_MmiHeader(const UINT8 * const pTg)
*\brief the function creates the header of EVC packet
*\param pTg pointer to input buffer
*/
static void Create_MmiHeader(const UINT8 * const pTg)
{
	t_FFFISPacket * const lFFFISPacket = GetFFFISPacket();
	if((pTg != 0) && (lFFFISPacket != 0) )
	{
		UINT16 lLenMmiPkt = 0x0U;
		/* create mmi_m_packet*/
		lFFFISPacket->EvcHeader.MmiMPacket = pTg[0];
		ae_write(
			AE_POSITION,
			"MmiMPacket = %d", lFFFISPacket->EvcHeader.MmiMPacket);

		memcpy(&lLenMmiPkt, &pTg[1], sizeof(lLenMmiPkt));
		/*convert in little endian*/
		/*lLenMmiPkt = NTOH_UINT16(lLenMmiPkt); make non sense because the board is a power PC--> BigEndian!!!!*/
		/* copy in the struct the length of MMI packet*/
		memcpy(&lFFFISPacket->EvcHeader.MmiLPacket, &lLenMmiPkt, sizeof(lLenMmiPkt));
		ae_write(
							AE_POSITION,
							"lLenMmiPkt after = %d", lLenMmiPkt);
	}/*if*/
}

/**
*\fn static gpp_result Create_EvcPacketData(const UINT8 *const pTg, UINT32 *pBuffIndex)
*\brief the function copy the data from the raw buffer in a specific buffer, used
*       to extraxt the data.
*\param pTg pointer to input buffer
*\param pBuffIndex pointer to index of raw buffer
*\return 0 success or number and type of error
*/
static gpp_result Create_EvcPacketData(const UINT8 *const pTg, UINT32 *pBuffIndex)
{
	gpp_result lOut = GPP_BAD_PARAM;
	const INT16 MinLength = 3; /* in byte*/
	const UINT16 ErrLen    = 0xFFFF;
	t_FFFISPacket * const lFFFISPacket = GetFFFISPacket();
	if((pTg != 0) && (lFFFISPacket != 0))
	{
		/* length in bits converted in byte*/
		UINT16 lPacketLength = ConvBit2Byte(GetPacketLen(&lFFFISPacket->EvcHeader.MmiLPacket[0]));
		lOut = GPP_BAD_LENGTH;
		/*plausibility of length*/
		if((lPacketLength >= MinLength) && (lPacketLength != ErrLen))
		{
			/* create copy datat in FFISPacket */
			/* for copy the data I have to skip the mmi_m_packet and mmi_L_packet.
			   the value of length includes also the field before */
			memcpy(lFFFISPacket->PktData, &pTg[3], (lPacketLength - sizeof(lFFFISPacket->EvcHeader)));
			*pBuffIndex+= lPacketLength;
			lOut = GPP_SUCCESS;
		}/*if*/
	}/*if*/
	return(lOut);
}

/**
*\fn static gpp_result Create_EOStream(const UINT8 *const pTg)
*\brief the function check if the next data is the end of streaming.
*\param pTg pointer to input buffer
*\return 0 success or number and type of error
*/
static gpp_result Create_EOStream(const UINT8 *const pTg)
{
	gpp_result lOut = GPP_BAD_PARAM;
	t_FFFISPacket *lFFFISPacket = GetFFFISPacket();
	if((pTg != 0) && (lFFFISPacket != 0))
	{
		UINT8 lEOStr = 0x0U;
		lEOStr = *pTg;
		lOut = GPP_SUCCESS;
		ae_write(
							AE_POSITION,
							"lEOStr = %d", lEOStr);

		if(GPP_CheckEOStream(lEOStr) == GPP_SUCCESS)
		{
			lFFFISPacket->EOStream = lEOStr;
		}
	}/*if*/
	return(lOut);
}


void GPP_CreateEvcPacket(const UINT8 *const pTg, UINT8 aNidStm)
{
	/* create header packet !!!*/
	t_FFFISPacket *lFFFISPacket = GetFFFISPacket();
	if((pTg != 0) && (lFFFISPacket != 0))
	{
		/*clean packet*/
		memset(lFFFISPacket, 0, sizeof(t_FFFISPacket));
		/*reset shared memory data input*/
		Create_NidPacket((UINT8*)&pTg[0], (UINT8*)(&lFFFISPacket->Header.NidPacket));
		Create_LenPacket((UINT8*)&pTg[1], (UINT8*)(&lFFFISPacket->Header.Packet));
		/* check the EVC plausibility*/
		if (GPP_CheckEvcNid(aNidStm, lFFFISPacket->Header.NidPacket) == GPP_SUCCESS)
		{
			/* in the row data we could have more than EVC packet.
			   we should open a while o parser the buffer in input till the end
				 of Stream Delimiter */
			UINT32 lBuffIndex = 3;
			gpp_result lChk = GPP_SUCCESS;
			/*loop to catch all data!!!*/
			while (lFFFISPacket->EOStream != END_OF_STREAM) {
				/* Create the MMI Header!!!*/
				Create_MmiHeader((UINT8*)&pTg[lBuffIndex]);
				/* Create the EVC Data and write the new start of point*/
				lChk|= Create_EvcPacketData((UINT8*)&pTg[lBuffIndex], &lBuffIndex);
				/* Create End of Stream Delimiter */
				/* check end of stream!!!*/
				lChk|= Create_EOStream((UINT8*)&pTg[lBuffIndex]);
				/*Extract the packet*/
				ae_write(
									AE_POSITION,
									"lFFFISPacket->EOStream = %d", lFFFISPacket->EOStream);
				/*TODO*/
				if(!lChk)
					GPP_ExtractEvcPacket(aNidStm);
				else
					break;
		  }/*while*/
			/* write the data in he shared memory*/
			sm_send_evc(EVC_SEND, GetEvcData(), 1);
		}/*if*/
	}/*if*/
}

/******STM Packet *********/
/* static functions STM !!!*/
/**
*\fn static void StmUpdateCtrLen(UINT8 *pLen, UINT8 aLenMsg)
*\brief the function update the length of buffer and check the length.
*\param pLen pointer to partial length of buffer
*\param aLenMsg global length of buffer
*/
static void StmUpdateCtrLen(UINT8 *pLen, UINT8 aLenMsg)
{
	t_StmPacket *lStmPacket = GetStmPacket();
	/* get length of packet*/
	if((pLen != 0) && (lStmPacket != 0))
	{
		UINT8 len = ConvBit2Byte(GetPacketLen(&lStmPacket->Header.Packet[0]));
		*pLen = *pLen + len;
		GPP_CheckStmFrame(pLen, aLenMsg);
	}/*if*/
}
/**
*\fn static UINT8 create_stmsinglepack(UINT8 p, UINT8 a)
*\brief the function create the single stm packet
*\param p actual data
*\param a next data
*\return the data to store
*/
static UINT8 create_stmsinglepack(UINT8 p, UINT8 a)
{
	return((((p&0x7)<<5) | (a>>3)));
}
/**
*\fn static gpp_result Create_StmPacketData(const UINT8 *const pTg, UINT32 *pIndex)
*\brief the function create the stm packet
*\param pTg pointer to the buffer
*\param pIndex index of buffer
*\return 0 success or number and type of error
*/
static gpp_result Create_StmPacketData(const UINT8 *const pTg, UINT32 *pIndex)
{
	gpp_result lOut = GPP_BAD_PARAM;
	const INT16 MinLength = 3; /* in byte*/
	const UINT16 ErrLen   = 0xFFFF;
  t_StmPacket *lStmPacket = GetStmPacket();
	/* get length of packet in byte*/
	UINT16 len = ConvBit2Byte(GetPacketLen(&lStmPacket->Header.Packet[0]));
	if((len >= MinLength) && (len != ErrLen))
	{
		UINT16 ii = 0;
		for(;ii < len-sizeof(lStmPacket->Header);ii++)
		{
			lStmPacket->PktData[ii] = create_stmsinglepack(pTg[ii], pTg[ii+1]);
		}/*for*/
		/*memcpy((UINT8*)&lStmPacket->PktData[0], (UINT8*)pTg, len-sizeof(lStmPacket->Header));*/
		*pIndex+=len;
		lOut = GPP_SUCCESS;
	}/*if*/
 	return(lOut);
}

void GPP_CreateStmPacket(const UINT8 *const pTg, UINT8 aLenMsg)
{
	t_StmPacket *lStmPacket = GetStmPacket();
	if((pTg != 0) && (lStmPacket != 0))
	{
		UINT32 lBuffIndex = 0;
		UINT8 lLenMsg = 0x0U;
		/* open the while cycle*/
		/* control of cycle on legth of packet */
		/*reset numebr of packet*/
		while (lLenMsg < aLenMsg) {
			/* code */
			/* Create the STM Header*/
			memset(lStmPacket, 0, sizeof(t_StmPacket));
			Create_NidPacket((UINT8*)&pTg[lBuffIndex], (UINT8*)(&lStmPacket->Header.NidPacket));
			Create_LenPacket((UINT8*)&pTg[lBuffIndex+1], (UINT8*)(&lStmPacket->Header.Packet));
			/* copy data in structure*/
			/* TODO*/
			Create_StmPacketData((UINT8*)&pTg[lBuffIndex+2], &lBuffIndex);
			/* Check the end of stream*/
			/* Extract the stm packet */
			StmUpdateCtrLen(&lLenMsg, aLenMsg);
			GPP_ExtractStmPacket();
		}
		/* write the data in he shared memory*/
		sm_send_stm(STM_SEND, GetStmData(), 1);
	}/*if*/
}
/* packet to put out*/
static BOOLEAN8 check_evc_packet(void)
{
	INT32 ii = 0;
	BOOLEAN8 lFind = FALSE;
	for(; ii < 5; ii++)
	{
		if(GetEvcSts()->Pack2Write[ii] != 0)
		{
			lFind = TRUE;
			break;
		}
	}/*for*/
	return(lFind);
}

BOOLEAN8 GPP_CreateEvcStsPacket(UINT8* const pTg)
{
	BOOLEAN8 lOut = FALSE;
	if(pTg != 0)
	{
		UINT8 lIsEvcStsMaster = 0;
		/* read the shared memory and store the data in a static object evc_sts */
		sm_recv_evc_sts(EVC_STS_RECV, GetEvcSts(), &lIsEvcStsMaster);
		/* create the packet and stored */
		if(check_evc_packet() == TRUE)
		{
			GPP_BuildEvcPacket(pTg);
			lOut = TRUE;
		}
	}/*if*/
	return(lOut);
}

static BOOLEAN8 check_stm_packet(void)
{
	INT32 ii = 0;
	BOOLEAN8 lFind = FALSE;
	for(; ii < 1; ii++)
	{
		if(GetStmSts()->Pack2Write[ii] != 0)
		{
			lFind = TRUE;
			break;
		}
	}/*for*/
	return(lFind);
}

void GPP_CreateStmStsPacket(UINT8* const pTg, UINT8 aNidStm)
{
	if(pTg != 0)
	{
		UINT8 lIsStmStsMaster = 0;
		/* read the shared memory and store the data in a static object evc_sts */
		sm_recv_stm_sts(STM_STS_RECV, GetStmSts(), &lIsStmStsMaster);
		/* create the packet and stored */
		if(check_stm_packet() == TRUE)
			GPP_BuildStmPacket(pTg, aNidStm);
	}/*if*/

}


BOOLEAN8 CheckPackets(void)
{
	UINT8 lIsEvcStsMaster = 0;
	BOOLEAN8 lRet = FALSE;
	INT32 ii = 0;
	/* read the shared memory and store the data in a static object evc_sts */
	sm_recv_evc_sts(EVC_STS_RECV, GetEvcSts(), &lIsEvcStsMaster);
	for(; ii < sizeof(GetEvcSts()->Pack2Write); ii++)
	{
		if((GetEvcSts()->Pack2Write[ii] != 0xFF) && (GetEvcSts()->Pack2Write[ii] != 0x0))
		{
			/*there is at least one packet*/
			mon_broadcast_printf("GetEvcSts()->Pack2Write[%d] = %d\n", ii, GetEvcSts()->Pack2Write[ii]);
			lRet = TRUE;
		}
	}
	return(lRet);
}
