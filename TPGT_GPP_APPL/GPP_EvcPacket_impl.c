/**
* \file GPP_EvcPacket_impl.c
* \brief the file manages the functions private of GPP to extract the EVC packets
*        the funcions are customized for EBICAB2000 ver.P8
* \author Muoio Salvatore
*/

#include "GPP_EvcPacket_impl.h"
#include "GPP_EvcPacket_Types.h"
#include "GPP_Plausibility.h"

/**
* \def SIZE_FFFIS_PACKET_HEADER
* \brief size static of FFFIS Packet Header
*/
#define SIZE_FFFIS_PACKET_HEADER 3

/* variables*/
/**
* \var static t_evc_data evc_data
* \brief static object to the structure t_evc_data
*/
static t_evc_data evc_data;
/**
* \var static t_evc_sts evc_sts
* \brief static object to the structure t_evc_sts
*/
static t_evc_sts  evc_sts;
/**
* \fn static void update_offset(UINT32 *pOff, UINT32 val)
* \brief the function update the offset
* \param pOff pointer to the offeset
* \param val value used to update
*/
static void update_offset(UINT32 *pOff, UINT32 val)
{
	if(pOff != 0)
		*pOff = *pOff + val;
}
/**************************** functions interface ***************/
t_evc_data *GetEvcData(void)
{
		return(&evc_data);
}

t_evc_sts *GetEvcSts(void)
{
	return(&evc_sts);
}

/**************************** READ functions ***************/
void Create_MmiStartAtp_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiStartAtp *lMMiStartAtp = (t_MmiStartAtp*)&(GetEvcData()->MmiStartAtp);
	if((lFFFISPacket != 0) && (lMMiStartAtp != 0))
	{
		/**
		* \var static UINT16 lLen
		* \brief length of packet, the length for this packet is 8bits
		*/
		static const UINT16 lLen = 4;
		/*plausibility check of length packet*/
		if(GPP_CheckLenEvcPcktData(lLen, &lFFFISPacket->EvcHeader.MmiLPacket[0]) == GPP_SUCCESS)
		{
			/*copy data*/
			lMMiStartAtp->MmiMStartReq = lFFFISPacket->PktData[0];
			/*set event!!!!*/
			GetEvcData()->evc_packet[Index_MmiStartAtp] = 1;
			/* print for debug !!!*/
			ae_write(
								AE_POSITION,
								"Create_MmiStartAtp_");
		}
	}/*if*/
}

void Create_MmiStatus_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiStatus *lMMiStatus = (t_MmiStatus*)&(GetEvcData()->MmiStatus);
	if((lFFFISPacket != 0) && (lMMiStatus != 0))
	{
		/**
		* \var static UINT16 lLen
		* \brief length of packet, the length for this packet data + header(3 bytes) is 72 bits
		*/
		static const UINT16 lLen = 9;
		/*plausibility check of length packet*/
		if(GPP_CheckLenEvcPcktData(lLen, &lFFFISPacket->EvcHeader.MmiLPacket[0]) == GPP_SUCCESS)
		{
			lMMiStatus->MmiMAdhesion = lFFFISPacket->PktData[0];
			/*using the bitfield I mustn't convert*/
			lMMiStatus->CabData.Data = (lFFFISPacket->PktData[1]);
			CopyandSwap32(&lMMiStatus->MmiNidOperation, &lFFFISPacket->PktData[2]);
			GetEvcData()->evc_packet[Index_MmiStatus] = 1;
		}/*if*/
	}/*if*/
}

void Create_MmiDriverMessage_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiDriverMessage *lMmiDriverMessage = (t_MmiDriverMessage*)&(GetEvcData()->MmiDriverMessage);
	if((lFFFISPacket != 0) && (lMmiDriverMessage != 0))
	{
		UINT16 len = 0x48U; /*length with init value different to zero*/
		UINT16 lOffset = 0; /*offset*/
		UINT16 ii,jj;       /*counters*/
		/*the length for this packet data + header(3 bytes) is 64 + N_TEXT*8 bits.*/
		/*TODO plausibility chek of length!!!???*/
		lMmiDriverMessage->MmiIText = lFFFISPacket->PktData[0];
		mon_broadcast_printf("EVC_8 lFFFISPacket->PktData[0] = %X\n", lFFFISPacket->PktData[0]);
		/*using the bitfield I mustn't convert*/
		lMmiDriverMessage->MmiQ.Data = (lFFFISPacket->PktData[1]);
		mon_broadcast_printf("EVC_8 lFFFISPacket->PktData[1] = %X\n", lFFFISPacket->PktData[1]);
		CopyandSwap16(&lMmiDriverMessage->MmiQText, &lFFFISPacket->PktData[2]);
		mon_broadcast_printf("EVC_8 lFFFISPacket->PktData[2] = %X\n", lFFFISPacket->PktData[2]);
		mon_broadcast_printf("EVC_8 lFFFISPacket->PktData[3] = %X\n", lFFFISPacket->PktData[3]);
		lMmiDriverMessage->MmiNText = lFFFISPacket->PktData[4];
		mon_broadcast_printf("EVC_8 lFFFISPacket->PktData[4] = %X\n", lFFFISPacket->PktData[4]);
		memcpy(lMmiDriverMessage->MmiXText, &lFFFISPacket->PktData[5], ((lMmiDriverMessage->MmiNText)*8));
		lOffset = (5 + ((lMmiDriverMessage->MmiNText)*8));
		lMmiDriverMessage->MmiNMsgBlock = lFFFISPacket->PktData[lOffset];
		mon_broadcast_printf("EVC_8 MmiNMsgBlock = %X\n", lFFFISPacket->PktData[lOffset]);
		jj = lOffset + 1;
		for(ii = 0; ii < lMmiDriverMessage->MmiNMsgBlock; ii++)
		{
			memcpy(&lMmiDriverMessage->MsgBlock[ii].MmiITextBegin, &lFFFISPacket->PktData[jj], sizeof(UINT8));
			jj+=sizeof(UINT8);
			memcpy(&lMmiDriverMessage->MsgBlock[ii].MmiITextEnd, &lFFFISPacket->PktData[jj], sizeof(UINT8));
			jj+=sizeof(UINT8);
		}/*for*/
		len = len + (lMmiDriverMessage->MmiNText*8) + (lMmiDriverMessage->MmiNMsgBlock*16);/*final length*/
		GetEvcData()->evc_packet[Index_MmiDriverMessage] = 1;/*TODO write macro!!!!*/
	}/*if*/
}

void Create_MmiSetTimeAtp_(const t_FFFISPacket * const lFFFISPacket)
{
 t_MmiSetTimeAtp *lMmiSetTimeAtp = (t_MmiSetTimeAtp*)&(GetEvcData()->MmiSetTimeAtp);
 if((lFFFISPacket != 0) && (lMmiSetTimeAtp != 0))
 {
	 /*the length for this packet data + header(3 bytes) is 64 bits.*/
	 /*TODO plausibility chek of length!!!???*/
	 CopyandSwap32(&lMmiSetTimeAtp->MmiTUtc, &lFFFISPacket->PktData[0]);
	 lMmiSetTimeAtp->MmiTZoneOffset = lFFFISPacket->PktData[4];
	 GetEvcData()->evc_packet[Index_MmiSetTimeAtp] = 1;
	}/*if*/
}

void Create_MmiTrackDescription_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiTrackDescription *lMmiTrackDescription = (t_MmiTrackDescription*)&(GetEvcData()->MmiTrackDescription);
	if((lFFFISPacket != 0) && (lMmiTrackDescription != 0))
	{
		/*the length for this packet data + header(3 bytes) is 72+Summ(n_mrsp*48)
		+ Summ(n_gradient*48) bits.*/
 	 /*TODO plausibility chek of length!!!???*/
	 INT16 ii = 0; /* local counter */
	 INT16 jj = 3; /* local counter */
	 CopyandSwap16((UINT16*)&lMmiTrackDescription->MmiVMrspCurr, &lFFFISPacket->PktData[0]);
	 lMmiTrackDescription->MmiNMrsp = lFFFISPacket->PktData[2];
	 /*copy in the t_MmiMrsp structure*/
	 for(; ii < lMmiTrackDescription->MmiNMrsp; ii++)
	 {
		 CopyandSwap16((UINT16*)&lMmiTrackDescription->MmiMrsp[ii].MmiVMrsp, &lFFFISPacket->PktData[jj]);
		 jj+=sizeof(UINT16);
		 CopyandSwap32((UINT32*)&lMmiTrackDescription->MmiMrsp[ii].MmiOMrsp, &lFFFISPacket->PktData[jj]);
		 jj+=sizeof(UINT32);
	 }/*for*/
	 CopyandSwap16((UINT16*)&lMmiTrackDescription->MmiGGradientCurr, &lFFFISPacket->PktData[jj]);
	 jj+=sizeof(UINT16);
	 lMmiTrackDescription->MmiNGradient = lFFFISPacket->PktData[jj];
	 jj++;
	 /*copy in the t_MmiGradient structure*/
	 for(ii = 0; ii < lMmiTrackDescription->MmiNGradient; ii++)
	 {
		 CopyandSwap16((UINT16*)&lMmiTrackDescription->MmiGradient[ii].MmiGGradient, &lFFFISPacket->PktData[jj]);
		 jj+=sizeof(UINT16);
		 CopyandSwap32((UINT32*)&lMmiTrackDescription->MmiGradient[ii].MmiOGradient, &lFFFISPacket->PktData[jj]);
		 jj+=sizeof(UINT32);
	 }/*for*/
	 GetEvcData()->evc_packet[Index_MmiTrackDescription] = 1;
	}/*if*/
}

/**
* \fn static void Copy_LocalCurrTrainData(t_MmiCurrentTrainData *lMmiCurrentTrainData, const UINT8 *const lPtrData)
* \brief The function copy loca current data in the packet
* \param lMmiCurrentTrainData pointer to t_MmiCurrentTrainData strcut
* \param lPtrData pointeer to data
*/
static void Copy_LocalCurrTrainData(t_MmiCurrentTrainData *lMmiCurrentTrainData, const UINT8 *const lPtrData)
{
	if((lMmiCurrentTrainData != 0) && (lPtrData != 0))
	{
		lMmiCurrentTrainData->MmiMAirtight  = lPtrData[0];
	 	lMmiCurrentTrainData->MmiNDsguarant = lPtrData[1];
	 	lMmiCurrentTrainData->MmiNDeguarant = lPtrData[2];
	 	CopyandSwap16(&lMmiCurrentTrainData->MmiTBrakeSb, &lPtrData[3]);
	 	CopyandSwap16(&lMmiCurrentTrainData->MmiTBrakeEb, &lPtrData[5]);
	 	CopyandSwap16(&lMmiCurrentTrainData->MmiTCutOffTraction, &lPtrData[7]);
	 	CopyandSwap16(&lMmiCurrentTrainData->MmiAMaxAcc, &lPtrData[9]);
	 	CopyandSwap16(&lMmiCurrentTrainData->MmiAMaxDec, &lPtrData[11]);
	 	lMmiCurrentTrainData->MmiQS.Data  = lPtrData[13];
	 	lMmiCurrentTrainData->MmiQB.Data  = lPtrData[14];
	  /* check the data */
	  if(lMmiCurrentTrainData->MmiQB.BData.ByPerc)
	  {
		  lMmiCurrentTrainData->MmiQBDP1.MmiMBrakePosition = lPtrData[15];
		  lMmiCurrentTrainData->MmiQBDP1.MmiMBrakePerc     = lPtrData[16];
	  }/*if*/
	  else
	  {
		  lMmiCurrentTrainData->MmiQBDP0.MmiMNumUnitTrainset = lPtrData[18];
		  lMmiCurrentTrainData->MmiQBDP0.MmiMNumIsolBrake    = lPtrData[19];
		}/*else*/
	}/*if*/
}

void Create_MmiCurrentTrainData_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiCurrentTrainData *lMmiCurrentTrainData = (t_MmiCurrentTrainData*)&(GetEvcData()->MmiCurrentTrainData);
	if((lFFFISPacket != 0) && (lMmiCurrentTrainData != 0))
	{
		UINT8 lSizeMmiTraction = 0;/*size of Mmi Traction*/
		/*the length for this packet data + header(3 bytes) is 256+ MMI_N_TRACTION*8.*/
 	 /*TODO plausibility chek of length!!!???*/
	 CopyandSwap16(&lMmiCurrentTrainData->MmiMDataEditEnable, &lFFFISPacket->PktData[0]);
	 CopyandSwap16(&lMmiCurrentTrainData->MmiNcTrain, &lFFFISPacket->PktData[2]);
	 CopyandSwap16(&lMmiCurrentTrainData->MmiLTrain, &lFFFISPacket->PktData[4]);
	 CopyandSwap16(&lMmiCurrentTrainData->MmiVMaxTrain, &lFFFISPacket->PktData[6]);
	 lMmiCurrentTrainData->MmiMLoadingGuage = lFFFISPacket->PktData[8];
	 lMmiCurrentTrainData->MmiMAxleLoad     = lFFFISPacket->PktData[9];
	 lMmiCurrentTrainData->MmiNTraction     = lFFFISPacket->PktData[10];
	 /*copy of MMI M TRACTION*/
	 lSizeMmiTraction = lMmiCurrentTrainData->MmiNTraction*8;
	 memcpy(lMmiCurrentTrainData->MmiMTraction, &lFFFISPacket->PktData[11], lSizeMmiTraction);
	 Copy_LocalCurrTrainData(lMmiCurrentTrainData, &lFFFISPacket->PktData[11+lSizeMmiTraction]);
	 GetEvcData()->evc_packet[Index_MmiCurrentTrainData] = 1;
	}/*if*/
}

void Create_MmiFailureReportAtp_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiFailureReportAtp *lMmiFailureReportAtp = (t_MmiFailureReportAtp*)&(GetEvcData()->MmiFailureReportAtp);
	if((lFFFISPacket != 0) && (lMmiFailureReportAtp != 0))
	{
		/*the length for this packet data + header(3 bytes) is 72+ MMI_N_TEXT*8.*/
 	 /*TODO plausibility chek of length!!!???*/
	 lMmiFailureReportAtp->MmiQFailureClass = lFFFISPacket->PktData[0];
	 lMmiFailureReportAtp->MmiFailure.Data  = lFFFISPacket->PktData[1];
	 CopyandSwap16(&lMmiFailureReportAtp->MmiIFailureNUmber, &lFFFISPacket->PktData[2]);
	 lMmiFailureReportAtp->MmiCriteria.Data = lFFFISPacket->PktData[4];
	 lMmiFailureReportAtp->MmiNText         = lFFFISPacket->PktData[5];
	 memcpy((UINT8*)lMmiFailureReportAtp->MmiXText, &lFFFISPacket->PktData[6], lMmiFailureReportAtp->MmiNText*8);
	 GetEvcData()->evc_packet[Index_MmiFailureReportAtp] = 1;
 }/*if*/
}

void Create_MmiEnableDriverRequest_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiEnableDriverRequest *lMmiEnableDriverRequest =
	                          (t_MmiEnableDriverRequest*)&(GetEvcData()->MmiEnableDriverRequest);
	if((lFFFISPacket != 0) && (lMmiEnableDriverRequest != 0))
	{
		/**
		* \var static UINT16 lLen
		* \brief length of packet
		*/
		static const UINT16 lLen = 7;
		/*plausibility check of length packet*/
		if(GPP_CheckLenEvcPcktData(lLen, &lFFFISPacket->EvcHeader.MmiLPacket[0]) == GPP_SUCCESS)
		{
			mon_broadcast_printf("lFFFISPacket->PktData[0] = %d\n", lFFFISPacket->PktData[0]);
			mon_broadcast_printf("lFFFISPacket->PktData[1] = %d\n", lFFFISPacket->PktData[1]);
			mon_broadcast_printf("lFFFISPacket->PktData[2] = %d\n", lFFFISPacket->PktData[2]);
			mon_broadcast_printf("lFFFISPacket->PktData[3] = %d\n", lFFFISPacket->PktData[3]);
			CopyandSwap32(&lMmiEnableDriverRequest->MmiQRequestEnable, &lFFFISPacket->PktData[0]);
			GetEvcData()->evc_packet[Index_MmiEnableDriverRequest] = 1;
		}/*if*/
	}/*if*/
}

void Create_MmiCurrentDriverData_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiCurrentDriverData *lMmiCurrentDriverData =
	                        (t_MmiCurrentDriverData*)&(GetEvcData()->MmiCurrentDriverData);
	if((lMmiCurrentDriverData != 0) && (lFFFISPacket != 0))
	{
		/**
		* \var static UINT16 lLen
		* \brief length of packet,
		*/
		static const UINT16 lLen = 13;
		if(GPP_CheckLenEvcPcktData(lLen, &lFFFISPacket->EvcHeader.MmiLPacket[0]) == GPP_SUCCESS)
		{
			/*add the copy of data*/
			CopyandSwap32(&lMmiCurrentDriverData->MmiNidDriver, &lFFFISPacket->PktData[0]);
			CopyandSwap32(&lMmiCurrentDriverData->MmiNidOperation, &lFFFISPacket->PktData[4]);
			lMmiCurrentDriverData->MmiNidLanguage  = lFFFISPacket->PktData[8];
			lMmiCurrentDriverData->MmiQDriver.Data = lFFFISPacket->PktData[9];
			GetEvcData()->evc_packet[Index_MmiCurrentDriverData] = 1;
		}
	}
}


void Create_MMiSpecificStmDeRequest_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MMiSpecificStmDeRequest *lMMiSpecificStmDeRequest =
	                          (t_MMiSpecificStmDeRequest*)&(GetEvcData()->MMiSpecificStmDeRequest);
	if((lFFFISPacket != 0) && (lMMiSpecificStmDeRequest != 0))
	{
		UINT32 lOffset = 15U; /*offset used to compute the data*/
		UINT16 ii, jj; /*index*/
		UINT16 lLength = 0; /*length of packet in bits*/
		memcpy(&lLength, &lFFFISPacket->EvcHeader.MmiLPacket, sizeof(UINT16));
		/*copy the data and after check the length*/
		lMMiSpecificStmDeRequest->MmiNidStm     = lFFFISPacket->PktData[0];
		lMMiSpecificStmDeRequest->uMmiStmQ.Data = lFFFISPacket->PktData[1];
		for(ii = 0; ii < lMMiSpecificStmDeRequest->uMmiStmQ.StmQ.Iter; ii++)
		{
			/*lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiNidStm      = lFFFISPacket->PktData[index++];
			lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmNidData  = lFFFISPacket->PktData[index++];
			lMMiSpecificStmDeRequest->Evc25StmIter1[ii].uMmiStmML.Data = lFFFISPacket->PktData[index++];*/
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8, &lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiNidStm);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset+8, 8, &lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmNidData);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset+16, 10, (UINT8*)&lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiXAttribute);
/*!!! to check*/			/*lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiXAttribute = HTON_UINT16(lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiXAttribute);*/
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset+26, 5, (UINT8*)&lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiLCaption);
			update_offset(&lOffset, 31);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8*lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiLCaption, lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmXCaption);
			update_offset(&lOffset, (8 * lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiLCaption));
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8, &lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmLValue);
			update_offset(&lOffset, 8);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8*lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmLValue, lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmXValue);
			update_offset(&lOffset, (8*lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmLValue));
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 5, &lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmNIter);
			update_offset(&lOffset, 5);
			/*copy of mmi stm x value*/
			for(jj = 0; jj < lMMiSpecificStmDeRequest->Evc25StmIter1[ii].MmiStmNIter; jj++)
			{
				UINT8 lSize = 0; /*local size*/
				create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8, &lMMiSpecificStmDeRequest->Evc25StmIter1[ii].Evc25StmIter2[jj].MmiStmLValue);
				update_offset(&lOffset, 8);
				lSize = (8*lMMiSpecificStmDeRequest->Evc25StmIter1[ii].Evc25StmIter2[jj].MmiStmLValue);
				create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, lSize, lMMiSpecificStmDeRequest->Evc25StmIter1[ii].Evc25StmIter2[jj].MmiStmXValue);
				update_offset(&lOffset, lSize);
			}/*second layer for*/
		}/*main layer for*/
		/**/
		lOffset = (lOffset + 7) - ((lOffset + 7) % 8);
		if(lLength != lOffset)
			GetEvcData()->diag_evc[Index_MMiSpecificStmDeRequest] = 1;
	 GetEvcData()->evc_packet[Index_MMiSpecificStmDeRequest] = 1;
	}/*if*/
}

void Create_MmiSpecificStmDwValues_(const t_FFFISPacket * const lFFFISPacket)
{
	t_MmiSpecificStmDwValues *lMmiSpecificStmDwValues =
	                          (t_MmiSpecificStmDwValues*)&(GetEvcData()->MmiSpecificStmDwValues);
	if((lFFFISPacket != 0) && (lMmiSpecificStmDwValues != 0))
	{
		UINT32 lOffset = 14; /*offset used to compute the data*/
		UINT16 ii; /*index*/
		UINT16 lLength = 0; /*length of packet in bits*/
		memcpy(&lLength, &lFFFISPacket->EvcHeader.MmiLPacket, sizeof(UINT16));
		/*copy the data and after check the length*/
		lMmiSpecificStmDwValues->MmiNidStm       = lFFFISPacket->PktData[0];
		lMmiSpecificStmDwValues->uEvc26Iter.Data = lFFFISPacket->PktData[1];
		for(ii = 0; ii < lMmiSpecificStmDwValues->uEvc26Iter.StmIter.Iter; ii++)
		{
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8, &lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiNidStm);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset+8, 8, &lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiStmNidData);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset+16, 10, (UINT8*)&lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiXAttribute);
/*!!! to check			lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiXAttribute = HTON_UINT16(lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiXAttribute);*/
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset+26, 5, (UINT8*)&lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiLCaption);
			update_offset(&lOffset, 31);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8*lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiLCaption, lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiStmXCaption);
			/*add the pls check about max number...to do for each iter!!!*/
			update_offset(&lOffset, (8 * lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiLCaption));
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8, &lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiStmLValue);
			update_offset(&lOffset, 8);
			create_localdump((UINT8*)lFFFISPacket->PktData, lOffset, 8*lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiStmLValue, lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiStmXValue);
			update_offset(&lOffset, (8*lMmiSpecificStmDwValues->Evc26StmIter[ii].MmiStmLValue));
		}/*for*/
			lOffset = (lOffset + 7) - ((lOffset + 7) % 8);
			if(lLength != lOffset)
				GetEvcData()->diag_evc[Index_MmiSpecificStmDwValues] = 1;
			GetEvcData()->evc_packet[Index_MmiSpecificStmDwValues] = 1;
	}
}


/* funtions used to write */
/**
* \fn static INT16 WriteLength(UINT8 *const pTg, UINT16 aLen)
* \brief The function write the length of message in the data to out
* \param pTg pointer to data
* \param aLen length of message
* \return 1 OK, 0 not OK
*/
static INT16 WriteLength(UINT8 *const pTg, UINT16 aLen)
{
	INT16 lOut = 0;
	if(pTg != 0)
	{
		pTg[0] = (UINT8)(aLen >> 5);
		pTg[1] = (UINT8)(aLen << 3);
		lOut = 1;
	}/*if*/
	return(lOut);
}

void Build_MmiStartMmi_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiStartMmi *lMmiMStartMmi = (t_MmiStartMmi*)&GetEvcSts()->MmiStartMmi;
	if((lFFFISMessage != 0) && (lMmiMStartMmi != 0))
	{
		/*t_FFFISPacket *lFFFISPacket = (t_FFFISPacket *)&lFFFISMessage->FFFISPacket;*/
		/* contents of both memory block are not equal, the first block of
			 memory is valid */
		{
			/* length of EVC packet is fixed */
			UINT16 len = 0x108U; /*33 bytes!!!*/
			/* create the length length of data in bits */
			UINT16 length = (len)  +
			                ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) + /* length of FFFIS packet header in bits */
							        ConvByte2Bits(sizeof(UINT8)); 			/* length of END of stream in bits */
			WriteLength(&lFFFISMessage->Buff[1], length);
			/* create the nid packet */
			lFFFISMessage->Buff[0] = MMI_EVC;
			/* create the evc packet header !!!*/
			lFFFISMessage->Buff[3] = 100U;
			/* convert the lenth in big endian and copy in the packet*/
			/* print for debug !!!*/
			/*len = HTON_UINT16(len);*/
			ae_write(
								AE_POSITION,
								"[Build_MmiStartMmi_]::len = %d\n",len);
			memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
			/* copy the data in the packet used to write */
			memcpy((UINT8*)&lFFFISMessage->Buff[6], (t_MmiStartMmi*)lMmiMStartMmi, sizeof(t_MmiStartMmi));
			/* add the end of stream */
			lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
			/*add the field of Message Header */
			lFFFISMessage->NidStm   = NID_EVC;
			lFFFISMessage->NumBytes = ConvBit2Byte(length);
			mon_broadcast_printf("[Build_MmiStartMmi_]::createBuild_MmiStartMmi_\n");
		}/*if*/
	}/*if*/
}

void Build_MmiStatusReport_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiStatusReport *lMmiStatusReport = (t_MmiStatusReport*)&GetEvcSts()->MmiStatusReport;
	if((lFFFISMessage != 0) && (lMmiStatusReport != 0))
	{
		/*t_FFFISPacket *lFFFISPacket = (t_FFFISPacket *)&lFFFISMessage->FFFISPacket;*/
		/* contents of both memory block are not equal, the first block of
			 memory is valid */
		{
			/* length of EVC packet is fixed */
			UINT16 len = 0x28U;
			/* create the length length of data in bits */
			UINT16 length = (len)  +
			                ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) + /* length of FFFIS packet header in bits */
											ConvByte2Bits(sizeof(UINT8)); 			    /* length of END of stream in bits */
			WriteLength(&lFFFISMessage->Buff[1], length);
			/* create the nid packet */
			lFFFISMessage->Buff[0] = MMI_EVC;
			/* create the evc packet header !!!*/
			lFFFISMessage->Buff[3] = 102U;
			/* convert the lenth in big endian and copy in the packet*/
			/*len = HTON_UINT16(len);*/
			memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
			/* copy the application data in the packet data!!!*/
			memcpy((UINT8*)&lFFFISMessage->Buff[6], (t_MmiStatusReport*)lMmiStatusReport, sizeof(t_MmiStatusReport));
			/* add the end of stream */
			lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
			/*add the field of Message Header*/
			lFFFISMessage->NidStm   = NID_EVC;
			lFFFISMessage->NumBytes = ConvBit2Byte(length);
			mon_broadcast_printf("Build_MmiStatusReport_\n");
		}/*if*/
	}/*if*/
}

void Build_MmiRequest_(t_FFFISMessage * const lFFFISMessage)
{
	UINT8 *lMmiRequest = (UINT8*)&GetEvcSts()->MmiRequest;
	if((lFFFISMessage != 0) && (lMmiRequest != 0))
	{
		{
			/* length of EVC packet is fixed */
			UINT16 len = 0x20U;
			/* create the nid packet
			lFFFISMessage->Buff[0] = MMI_EVC;*/
			UINT16 length = (len)  +
			                ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) + /* length of FFFIS packet header in bits */
							        ConvByte2Bits(sizeof(UINT8)); 			/* length of END of stream in bits */
			WriteLength(&lFFFISMessage->Buff[1], length);
			/*create the length length of data in bits */
			lFFFISMessage->Buff[0] = MMI_EVC;
			/* create the evc packet header !!!*/
			lFFFISMessage->Buff[3]= 101U;
			/* convert the lenth in big endian and copy in the packet*/
			/*len = HTON_UINT16(len);*/
			memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
			/* copy the application data in the packet data!!!*/
			memcpy((UINT8*)&lFFFISMessage->Buff[6], (t_MmiRequest*)lMmiRequest, sizeof(t_MmiRequest));
			/*add the field of Message Header*/
			lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
			lFFFISMessage->NidStm   = NID_EVC;
			lFFFISMessage->NumBytes = ConvBit2Byte(length);
		}/*if*/
	}/*if*/
}

void Build_MmiNewDriverData_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiNewDriverData *lMmiNewDriverData =
                      (t_MmiNewDriverData*)&GetEvcSts()->MmiNewDriverData;
	if((lFFFISMessage != 0)&&(lMmiNewDriverData != 0))
	{
		UINT16 len = 0x68U;
		/*create the length length of data in bits */
		UINT16 length = (len)  +
										ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) + /* length of FFFIS packet header in bits */
										ConvByte2Bits(sizeof(UINT8)); /* length of END of stream in bits */
		WriteLength(&lFFFISMessage->Buff[1], length);
		/* create the nid packet*/
		lFFFISMessage->Buff[0] = MMI_EVC;
		/* create the evc packet header !!!*/
		lFFFISMessage->Buff[3]= 104U;
		/* convert the lenth in big endian and copy in the packet*/
		/*len = HTON_UINT16(len);*/
		memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
		memcpy(&lFFFISMessage->Buff[6], (t_MmiNewDriverData*)lMmiNewDriverData, sizeof(t_MmiNewDriverData));
		/*memcpy(&lFFFISMessage->Buff[6], &lMmiNewDriverData->MmiNidDriver, 4);
		memcpy(&lFFFISMessage->Buff[10], &lMmiNewDriverData->MmiNidOperation, 4);
		lFFFISMessage->Buff[14] = lMmiNewDriverData->MmiNidLanguage;
		lFFFISMessage->Buff[15] = lMmiNewDriverData->MmiQDriver.Data;*/
		lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
		lFFFISMessage->NidStm   = NID_EVC;
		lFFFISMessage->NumBytes = ConvBit2Byte(length);
		mon_broadcast_printf("Build_MmiNewDriverData_\n");
	}
}

void Build_MmiSpecificStmDataToStm_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiSpecificStmDataToStm *lMmiSpecificStmDataToStm =
	                          (t_MmiSpecificStmDataToStm*)&GetEvcSts()->MmiSpecificStmDataToStm;
	if((lFFFISMessage != 0)&&(lMmiSpecificStmDataToStm != 0))
	{
		UINT16 len = 0x25U;
		UINT16 length = 0U;
		UINT32 lOffset = 56U;
		UINT16 jj = 0;/*index*/
		len = (len)+(lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter * 24);
		for(; jj < lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter; jj++)
		{
			len = len + (lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmLValue*8);
		}/*for to know the length*/
		len = ((len + 7) - ((len + 7)%8));
		length = len + ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) + /* length of FFFIS packet header in bits */
									 ConvByte2Bits(sizeof(UINT8)); /* length of END of stream in bits */

		lFFFISMessage->Buff[0] = MMI_EVC;
		WriteLength(&lFFFISMessage->Buff[1], length);
		lFFFISMessage->Buff[3]= 123U;
		memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
		lFFFISMessage->Buff[6] = lMmiSpecificStmDataToStm->MmiNiStm;
		lFFFISMessage->Buff[7] = lMmiSpecificStmDataToStm->uMmiStmNIter.Data;
		mon_broadcast_printf("lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter = %d\n", lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter);
		/*write_localdump(&lMmiSpecificStmDataToStm->MmiStmNIter, lOffset, 5, &lFFFISMessage->Buff[0]);*/
		update_offset(&lOffset, 5);
		for(jj = 0; jj <lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter; jj++)
		{
			write_localdump(&lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiNidStm, lOffset, 8, &lFFFISMessage->Buff[0]);
			write_localdump(&lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmNidData, lOffset+8, 8, &lFFFISMessage->Buff[0]);
			write_localdump(&lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmLValue, lOffset+16, 8, &lFFFISMessage->Buff[0]);
			update_offset(&lOffset, 24);
			write_localdump(&lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmXValue[0], lOffset,
			                 8*lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmLValue,
											 &lFFFISMessage->Buff[0]);
			mon_broadcast_printf("lMmiSpecificStmDataToStm->Evc123StmIter[%d].MmiStmXValue[0] = %c\n", jj, lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmXValue[0]);
			update_offset(&lOffset, (8*lMmiSpecificStmDataToStm->Evc123StmIter[jj].MmiStmLValue));
		}/*for data*/
		lFFFISMessage->NidStm   = NID_EVC;
		lFFFISMessage->NumBytes = ConvBit2Byte(length);
		lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
	mon_broadcast_printf("Build_MmiSpecificStmDataToStm_\n");
	}
}

void Build_MmiDriverMessageAck_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiDriverMessageAck *lMmiDriverMessageAck =
	                      (t_MmiDriverMessageAck*)&GetEvcSts()->MmiDriverMessageAck;
	if((lFFFISMessage != 0) && (lMmiDriverMessageAck != 0))
	{
		{
			/* length of EVC packet is fixed */
			UINT16 len = 0x28U;
			/* create the nid packet
			lFFFISMessage->Buff[0] = MMI_EVC;*/
			UINT16 length = (len)  +
			                ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) +     /* length of FFFIS packet header in bits */
							        ConvByte2Bits(sizeof(UINT8)); /* length of END of stream in bits */
			WriteLength(&lFFFISMessage->Buff[1], length);
			/*create the length length of data in bits */
			lFFFISMessage->Buff[0] = MMI_EVC;
			/* create the evc packet header !!!*/
			lFFFISMessage->Buff[3]= 111U;
			/* convert the lenth in big endian and copy in the packet*/
			/*len = HTON_UINT16(len);*/
			memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
			/* copy the application data in the packet data!!!*/
			memcpy((UINT8*)&lFFFISMessage->Buff[6],
			       (t_MmiDriverMessageAck*)lMmiDriverMessageAck,
						 sizeof(t_MmiDriverMessageAck));
			/*add the field of Message Header*/
			lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
			lFFFISMessage->NidStm   = NID_EVC;
			lFFFISMessage->NumBytes = ConvBit2Byte(length);
			mon_broadcast_printf("Build_MmiDriverMessageAck_[111]\n");
		}/*if*/
	}/*if*/
}

void Build_MmiSetTimeMmi_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiSetTime *lMmiSetTime =
	                      (t_MmiSetTime*)&GetEvcSts()->MmiSetTime;
	if((lFFFISMessage != 0) && (lMmiSetTime != 0))
	{
		{
			/* length of EVC packet is fixed */
			UINT16 len = 0x40U;
			/* create the nid packet
			lFFFISMessage->Buff[0] = MMI_EVC;*/
			UINT16 length = (len)  +
			                ConvByte2Bits(SIZE_FFFIS_PACKET_HEADER) +     /* length of FFFIS packet header in bits */
							        ConvByte2Bits(sizeof(UINT8)); /* length of END of stream in bits */
			WriteLength(&lFFFISMessage->Buff[1], length);
			/*create the length length of data in bits */
			lFFFISMessage->Buff[0] = MMI_EVC;
			/* create the evc packet header !!!*/
			lFFFISMessage->Buff[3]= 109U;
			/* convert the lenth in big endian and copy in the packet*/
			/*len = HTON_UINT16(len);*/
			memcpy((UINT8 *)&lFFFISMessage->Buff[4], (UINT16 *)&len, sizeof(UINT16));
			/* copy the application data in the packet data!!!*/
			memcpy((UINT8*)&lFFFISMessage->Buff[6],
			       (t_MmiSetTime*)lMmiSetTime,
						 sizeof(t_MmiSetTime));
			/*add the field of Message Header*/
			lFFFISMessage->NidStm   = NID_EVC;
			lFFFISMessage->NumBytes = ConvBit2Byte(length);
			lFFFISMessage->Buff[ConvBit2Byte(length)-1] = 0xFF;
			mon_broadcast_printf("Build_MmiDriverMessageAck_[109]\n");
		}/*if*/
	}/*if*/
}
