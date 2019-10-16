/**
* \file CcutGpp_impl.c
* \brief the file manages the functions private of GPP Api
* \author Muoio Salvatore
*/
#include "gpp_main.h"
#include "CcutGpp_Appl.h"
#include "CcutGpp_impl.h"
#include "GPP_Plausibility.h"
#include "GPP_ParserPacket.h"
#include "GPP_WritePacket.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
* \var t_RowDataEntry DataEntry
* \brief variable to the static objec t_RowDataEntry
*/
static t_RowDataEntry DataEntry;

/* public interface */
t_RowDataEntry *GetDataEntry(void)
{
	return(&DataEntry);
}


e_result CopyTelegramEntryLocal(BYTE *ptrds1, BYTE *ptrds2, BYTE *ptrds3)
{
	e_result lOut = TPG_SUCCESS;
	lOut |= checkOutputArg((BYTE*)ptrds1);
	lOut |= checkOutputArg((BYTE*)ptrds2);
	lOut |= checkOutputArg((BYTE*)ptrds3);
	if(TPG_SUCCEEDED(lOut))
	{
		/*local copy */
		memcpy(Tg.TPG_TSG_MMI_F1_P1, ptrds1, 32);
		memcpy(Tg.TPG_TSG_MMI_F1_P2, ptrds2, 32);
		memcpy(Tg.TPG_TSG_MMI_F1_P3, ptrds3, 32);
	}/*if*/
	return(lOut);
}



/**
* \var t_RowDataEntry sInvalidDate
* \brief static object used to invalidate the data
*/
static t_RowDataEntry sInvalidDate = {{0}};
void Gpp_InvalidateData(t_RowDataEntry * const pData)
{
	/*t_RowDataEntry *lInvalidData = &sInvalidDate;*/
	if(pData != 0)
	{
		/* reset only the data input*/
		/*memcpy(pData->DataInp, lInvalidData->DataInp, sizeof(lInvalidData->DataInp));*/
	}/*if*/
}

gpp_result GPP_PacketConsistency(UINT16 aPcktLen)
{
	gpp_result lRes = GPP_SUCCESS;
	if(aPcktLen > 0)
	{
		mon_broadcast_printf("aPcktLen = %X\n",aPcktLen);
		/* plausibility check of the data entry packets */
		lRes |= GPP_CheckPacketLength(aPcktLen);
		mon_broadcast_printf("lRes_GPP_CheckPacketLength= %X\n",lRes);
		/*lRes |= GPP_CheckFrameLength(aPcktLen, (UINT8*)&aReadBuffFromMvb[0]);
		mon_broadcast_printf("lRes_GPP_CheckFrameLength = %X\n",lRes);*/
	}
	else
		lRes = GPP_BAD_PARAM;
	return(lRes);
}

void GPP_Prepare2StorageData(void)
{
	/**
	* \var static UINT8 lDummyPort[32]
	* \brief dummy variable used for the empty fields
	*/
	static UINT8 lDummyPort[32];
	memset(lDummyPort, 0, sizeof(lDummyPort));
	/*gpp_init_ports(CABIN, CTRL_LOCAL_PORT, DATA_PORT_1, DATA_PORT_2, CTRL_REMOTE_PORT);*/
	/* call the gpp_init with the local Tg addressed to MVB ports*/
	gpp_init(GPP_NR_PORTS_MMI_TSG, GPP_NR_PORTS_TSG_MMI,
		     &Tg.TPG_TSG_MMI_F1_P1[0],
			 &Tg.TPG_TSG_MMI_F1_P2[0],
			 &Tg.TPG_TSG_MMI_F1_P3[0],
			 &Tg.TPG_MMI_TSG_F1[0],
			 &lDummyPort[0],
			 &lDummyPort[0],
			 &lDummyPort[0],
			 &lDummyPort[0]);
	gpp_trace_enable(1);
}


e_result GPP_ReadTSGQueue(UINT16 *pLenData)
{
	e_result lOut = TPG_SUCCESS;
	lOut |= checkOutputArg((UINT16*)pLenData);
	if(TPG_SUCCEEDED(lOut))
	{
		/*invalidate the data entry before the read*/
		/*Gpp_InvalidateData(lTg);*/
		/* read the data and copy in the DataInp array*/
		/**pLenData = gpp_qm_read_tsg_data((UINT8*)&(lTg->DataInp), sizeof(lTg->DataInp));*/
		/**pLenData = gpp_qm_read_tsg_data(
                               (UINT8*) &aReadBuffFromMvb, sizeof(aReadBuffFromMvb));*/
		/*TRACE(0, 1, " EVC Data Incoming\n", PrintEvcInput());*/
		/*mon_broadcast_printf("*pLenData = %d\n", *pLenData);*/
		/*mon_broadcast_printf("aReadBuffFromMvb[0] = %d\n", aReadBuffFromMvb[0]);*/
	}/*if*/
	return(lOut);
}
/**
* \var static UINT8 sNidStm
* \brief variable regarding the nation id stm
*/
static 	UINT8 sNidStm  = 0x0U;

void GPP_ReadPacket(UINT8 *pBuff)
{
	UINT8 lMessage = 0x0U;
	if(pBuff != 0){
	/*pLBuff = pBuff;*/
	/*get the nid_stm of FFFIS Message Header*/
	GPP_GetNidStm((UINT8*)&pBuff[0], &sNidStm);
	/*get the length of whole message in bytes, FFFIS Message Header*/
	GPP_GetLMsgStm((UINT8*)&pBuff[1], &lMessage);
	mon_broadcast_printf("sNidStm = %X\n", sNidStm);
	if((sNidStm == NID_TSG) || (sNidStm == NID_EVC))
	{
		/* create a Evc Message FFFIS Packet*/
		GPP_CreateEvcPacket((UINT8*)&pBuff[2], sNidStm);
		/* extract FFIS data in file GPP_ParserPacket!!!!*/
	}/*if*/
	else
	{
		/* create the stm packet*/
		GPP_CreateStmPacket((UINT8*)&pBuff[2], lMessage);
		/* extract STM data in file GPP_ParserPacket!!!!*/
	}/*else*/
}
}
/**
* \fn static UINT16 store_mmi_data(UINT8 *pTg, UINT8 nid_stm, UINT16 len)
* \brief pointer function call to api of gpp:gpp_qm_store_mmi_data
*       this function is called after the engine has created the püacket to put out
* \param pTg pointer of application packet to put out
* \param nid_stm identifier of sender(STM, TSG, ATP)
* \param len numebr of byte of application data
* \return GPP_OK is ok, GPP_ERROR or GPP_NO_CONNECTION
*/
static UINT16 store_mmi_data(UINT8 *ptg, UINT8 nid_stm, UINT16 len)
{

	UINT16 err = gpp_qm_store_mmi_data((UINT8*)ptg, nid_stm, len);
	return(err);
}

void GPP_StoreMmiData(BOOLEAN8 aDebug)
{
	t_RowDataEntry *lTg = GetDataEntry();
	/*
	if(GPP_CreateEvcStsPacket((UINT8*)&lTg->DataOut[0]) == FALSE)
	{
		GPP_CreateStmStsPacket((UINT8*)&lTg->DataOut[0], sNidStm);
	}*/
	/*new function used to add the signal in queue*/
	GPP_CreatePacket2Write(sNidStm);
	{
		/*Debug*/
		if(aDebug == TRUE)
		{
			UINT32 ii = 0;
			mon_broadcast_printf("Data Output :: GPP communication\n");
			for(; ii < 32; ii++)
				mon_broadcast_printf("lTg->DataOut[%d] = %X\n", ii, lTg->DataOut[ii]);
		}
	}/*body*/
}

void GPP_StoreMmiDataInit(void)
{
	/* check which ebicab is present */
	/* assigned the pointer (just one time)*/
	_store_mmi_data = &store_mmi_data;
}

BOOLEAN8 CheckData2Send(void)
{
	/*there are some packets???*/
	return(CheckPackets());
}

#ifdef __cplusplus
}
#endif
