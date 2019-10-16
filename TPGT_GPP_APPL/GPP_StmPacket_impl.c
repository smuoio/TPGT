/**
* \file GPP_stmPacket_impl.c
* \brief the file manages the functions private of GPP to extract the packets STM
* \author Muoio Salvatore
*/
#include "GPP_StmPacket_impl.h"
#include "GPP_StmPacket_Types.h"
#include "GPP_Plausibility.h"

/* variables*/
static t_stm_data stm_data;
static t_stm_sts  stm_sts;
/**************************** functions ***************/
t_stm_data *GetStmData(void)
{
		return(&stm_data);
}

t_stm_sts *GetStmSts(void)
{
	return(&stm_sts);
}


void Create_VersionNumber_(const t_StmPacket * const lStmPacket)
{
  t_VersionNumber *lVersionNumber = (t_VersionNumber*)&GetStmData()->VersionNumber;
  if((lStmPacket != 0) && (lVersionNumber != 0))
  {
    /* length of packet is 85bits valid + 3bits used for the padding
       in the legth, the total is 88bits or 11 bytes */
    /* copy data */
    static const UINT16 cLen = 11; /* static value from specific */
    if(GPP_CheckLenEvcPcktData(cLen, lStmPacket->Header.Packet) == GPP_SUCCESS)
    {
			/* each data is shifted of 3bits, beacese the length is on 13 bits.
			   the size of header is 21 bits*/
      lVersionNumber->MmiStmN058Vermajor = (lStmPacket->PktData[0]);
      lVersionNumber->MmiStmN058Vermid   = (lStmPacket->PktData[1]);
      lVersionNumber->MmiStmN058Verminor = (lStmPacket->PktData[2]);
      lVersionNumber->MmiStmN035Vermajor = (lStmPacket->PktData[3]);
      lVersionNumber->MmiStmN035Vermid   = (lStmPacket->PktData[4]);
      lVersionNumber->MmiStmN035Verminor = (lStmPacket->PktData[5]);
      lVersionNumber->MmiStmNSrsVermajor = (lStmPacket->PktData[6]);
      lVersionNumber->MmiStmNSrsVerminor = (lStmPacket->PktData[7]);
	    GetStmData()->stm_packet[Index_VersionNumber] = 1;/*stm-1*/
	    mon_broadcast_printf("Create_VersionNumber_\n");
    }/*if*/
  }/*if*/
}

void Create_StateReport_(const t_StmPacket * const lStmPacket)
{
  UINT8 *lStateReport = (UINT8*)&GetStmData()->StateReport;
  if((lStmPacket != 0) && (lStateReport != 0))
  {
    static const UINT16 cLen = 4;/* static value from specific */
		UINT8 lOffset = 0U;
		/* read the second byte*/
    if(GPP_CheckLenEvcPcktData(cLen, lStmPacket->Header.Packet) == GPP_SUCCESS)
    {
      create_localdump((UINT8*)lStmPacket->PktData, lOffset, 4, lStateReport);
			GetStmData()->stm_packet[Index_StateReport] = 1;/*stm-15*/
			mon_broadcast_printf("[Create_StateReport_]::lStateReport = %X\n", *lStateReport);
    }/*if*/
  }/*if*/
}

void Create_ButtonRequest_(const t_StmPacket * const lStmPacket)
{
		t_ButtonRequest *lButtonRequest = (t_ButtonRequest*)&GetStmData()->ButtonRequest;
		if((lStmPacket != 0) && (lButtonRequest != 0))
		{
			UINT16 ii = 0x0U; /* index used in the loop*/
			UINT32 lOffset = 0U;/*21U;*/ /*offset used to compute the data*/
			UINT16 lLength = 0x0U;/*variable used to copy the legth*/
			memcpy(&lLength, &lStmPacket->Header.Packet, sizeof(UINT16));
			/* length dynamic::!!! TODO no plausibility???!!!!*/
			lButtonRequest->MmiStmNIter.Data = lStmPacket->PktData[0];
			/* loop */
			if(lButtonRequest->MmiStmNIter.StmIter.Iter <= 10){
				lOffset+=5;
			for(;ii < lButtonRequest->MmiStmNIter.StmIter.Iter; ii++)
			{
				create_localdump((UINT8*)lStmPacket->PktData, lOffset,    8, &lButtonRequest->Data[ii].MmiNidStm );
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+8,  8, &lButtonRequest->Data[ii].MmiStmButton );
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+16, 4, &lButtonRequest->Data[ii].MmiStmNidButPos );
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+20, 8, &lButtonRequest->Data[ii].MMiStmNidIcon );
				create_localdump_16((UINT8*)lStmPacket->PktData, lOffset+28, 10, (UINT16*)&lButtonRequest->Data[ii].MMiStmMButAttrib );
				/*lButtonRequest->Data[ii].MMiStmMButAttrib = HTON_UINT16(lButtonRequest->Data[ii].MMiStmMButAttrib);*/
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+38, 5, &lButtonRequest->Data[ii].MmiStmLCaption );
				lOffset+=43;
				create_localdump((UINT8*)lStmPacket->PktData, lOffset, 8*lButtonRequest->Data[ii].MmiStmLCaption,
				                 &lButtonRequest->Data[ii].MmiStmXCaption[0]);
        lOffset+=(8*lButtonRequest->Data[ii].MmiStmLCaption);
			}/*for*/
			if(lLength != lOffset)
				GetStmData()->diag_stm[Index_ButtonRequest] = 1;
			GetStmData()->stm_packet[Index_ButtonRequest] = 1;/*stm-32*/
		}/*if plausibility*/
		}/*if*/
}

void Create_MmiStmTextMessage_(const t_StmPacket * const lStmPacket)
{
	t_MmiStmTextMessage *lMmiStmTextMessage = (t_MmiStmTextMessage*)&GetStmData()->MmiStmTextMessage;
	if((lStmPacket != 0) && (lMmiStmTextMessage != 0))
	{
		UINT32 lOffset = 0U;
		UINT16 lLength = 0x0U;  /*variable used to copy the legth*/

		memcpy(&lLength, &lStmPacket->Header.Packet, sizeof(UINT16));
		lMmiStmTextMessage->MmiStmNidXMessage = lStmPacket->PktData[0];
		create_localdump_16((UINT8*)lStmPacket->PktData, lOffset+8,  10, (UINT16*)&lMmiStmTextMessage->MmiStmMXAttribute);
		create_localdump((UINT8*)lStmPacket->PktData, lOffset+18, 1, &lMmiStmTextMessage->MmiStmQAck);
		create_localdump((UINT8*)lStmPacket->PktData, lOffset+19, 8, &lMmiStmTextMessage->MmiStmLText);
		lOffset += 27;
		if(lMmiStmTextMessage->MmiStmLText <= 40)
		{
			create_localdump((UINT8*)lStmPacket->PktData, lOffset,
		                 	8*lMmiStmTextMessage->MmiStmLText,
										 	&lMmiStmTextMessage->MmiStmXText[0]);
			lOffset += 8*lMmiStmTextMessage->MmiStmLText;
		}/*pls check nunm of char*/
		if(lLength != lOffset)
			GetStmData()->diag_stm[Index_MmiStmTextMessage] = 1;
		GetStmData()->stm_packet[Index_MmiStmTextMessage] = 1;/*stm-38*/
	}
}

void Create_MmiStmDeleteTextMessage_(const t_StmPacket * const lStmPacket)
{
	UINT8 *lMmiStmNidXMessage = (UINT8*)&GetStmData()->MmiStmNidXMessage;
	if((lStmPacket != 0) && (lMmiStmNidXMessage != 0))
	{
		*lMmiStmNidXMessage = lStmPacket->PktData[0];
		GetStmData()->stm_packet[Index_MmiStmDeleteTextMessage] = 1;/*stm-39*/
	}/*if */
}

void Create_MmiStmSoundCmd_(const t_StmPacket * const lStmPacket)
{
	t_MmiStmSoundCmd *lMmiStmSoundCmd =
	                 (t_MmiStmSoundCmd*)&GetStmData()->MmiStmSoundCmd;
	if((lMmiStmSoundCmd != 0) && (lStmPacket != 0))
	{
		UINT16 ii, jj; /* index used in the loop*/
		UINT32 lOffset = 0U;/*21U;*/ /*offset used to compute the data*/
		UINT16 lLength = 0x0U;/*variable used to copy the legth*/
		memcpy(&lLength, &lStmPacket->Header.Packet, sizeof(UINT16));
		lMmiStmSoundCmd->MmiStmNIter.Data = lStmPacket->PktData[0];
		if(lMmiStmSoundCmd->MmiStmNIter.StmIter.Iter <= 2)
		{
			lOffset+=5;
			for(ii = 0;ii < lMmiStmSoundCmd->MmiStmNIter.StmIter.Iter; ii++)
			{
				create_localdump((UINT8*)lStmPacket->PktData, lOffset, 8, &lMmiStmSoundCmd->Stm46Iter1[ii].MmiNidStm);
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+8, 8, &lMmiStmSoundCmd->Stm46Iter1[ii].MmiStmNidSound);
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+16, 2, &lMmiStmSoundCmd->Stm46Iter1[ii].MmiStmQSound);
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+18, 5, &lMmiStmSoundCmd->Stm46Iter1[ii].MmiStmNIter);
				lOffset+=23;
				if(lMmiStmSoundCmd->Stm46Iter1[ii].MmiStmNIter <= 31)
				{
					for(jj = 0; jj < lMmiStmSoundCmd->Stm46Iter1[ii].MmiStmNIter; jj++)
					{
						create_localdump((UINT8*)lStmPacket->PktData, lOffset, 8,
						                 &lMmiStmSoundCmd->Stm46Iter1[ii].Stm46Iter2[jj].MmiStmMFreq);
					  create_localdump((UINT8*)lStmPacket->PktData, lOffset+8, 8,
  					                 &lMmiStmSoundCmd->Stm46Iter1[ii].Stm46Iter2[jj].MmiStmTSound);
						lOffset+=16;
					}/*for jj*/
				}/*if pls index*/
			}/*for ii*/
		}/*if pls index*/
		if(lLength != lOffset)
			GetStmData()->diag_stm[Index_MmiStmSoundCmd] = 1;
	 GetStmData()->stm_packet[Index_MmiStmSoundCmd] = 1;/*stm-46*/
	}/*if pls*/
}

void Create_MmiStmIndicatorRequest_(const t_StmPacket * const lStmPacket)
{
	t_MmiStmIndicatorRequest * lMmiStmIndicatorRequest =
	(t_MmiStmIndicatorRequest*)&GetStmData()->MmiStmIndicatorRequest;
	if((lMmiStmIndicatorRequest != 0) && (lStmPacket != 0)){
		UINT16 ii      = 0U;   	/* index used in the loop*/
		UINT32 lOffset = 0U;  	/*offset used to compute the data*/
		UINT16 lLength = 0x0U;	/*variable used to copy the legth*/
		memcpy(&lLength, &lStmPacket->Header.Packet, sizeof(UINT16));
		lMmiStmIndicatorRequest->MmiStmNIter.Data = lStmPacket->PktData[0];
		if(lMmiStmIndicatorRequest->MmiStmNIter.StmIter.Iter <= 24){
			lOffset+=5;
			for(; ii <= lMmiStmIndicatorRequest->MmiStmNIter.StmIter.Iter; ii++){
				create_localdump((UINT8*)lStmPacket->PktData, lOffset, 8,
				                 &lMmiStmIndicatorRequest->Stm35Iter1[ii].MmiNidStm);
 			  create_localdump((UINT8*)lStmPacket->PktData, lOffset+8, 8,
				                 &lMmiStmIndicatorRequest->Stm35Iter1[ii].MmiStmNidIndicator);
				create_localdump((UINT8*)lStmPacket->PktData, lOffset+16, 5,
				 								 &lMmiStmIndicatorRequest->Stm35Iter1[ii].MmiStmNidIndpos);
			  create_localdump((UINT8*)lStmPacket->PktData, lOffset+21, 8,
				 								 &lMmiStmIndicatorRequest->Stm35Iter1[ii].MmiStmNidIcon);
			  create_localdump_16((UINT8*)lStmPacket->PktData, lOffset+29, 10,
								 				 (UINT16*)&lMmiStmIndicatorRequest->Stm35Iter1[ii].MmiSstmMIndAttrib);
			  create_localdump((UINT8*)lStmPacket->PktData, lOffset+39, 5,
								 				 (UINT8*)&lMmiStmIndicatorRequest->Stm35Iter1[ii].MmimStmLCaption);
				lOffset+=44;
				if (lMmiStmIndicatorRequest->Stm35Iter1[ii].MmimStmLCaption <= 12) {
					/* code */
					create_localdump((UINT8*)lStmPacket->PktData, lOffset,
					                 8*lMmiStmIndicatorRequest->Stm35Iter1[ii].MmimStmLCaption,
					 								 &lMmiStmIndicatorRequest->Stm35Iter1[ii].MmiStmXCaption[0]);
					lOffset+=8*lMmiStmIndicatorRequest->Stm35Iter1[ii].MmimStmLCaption;
					}/*if*/
			}/*for ii*/
			if(lLength != lOffset)
				GetStmData()->diag_stm[Index_MmiStmSoundCmd] = 1;
		}
		GetStmData()->stm_packet[Index_MmiStmIndicatorRequest] = 1;/*stm-35*/
		mon_broadcast_printf("[STM 35]::Create_MmiStmIndicatorRequest_\n");
	}
}

/**** build ****/
static INT16 StmWriteLength(UINT8 *const pTg, UINT16 aLen)
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



void Build_StmVersionNumber_(t_FFFISMessage * const lFFFISMessage)
{
	t_VersionNumber *lVersionNumber = (t_VersionNumber *)&GetStmSts()->VersionNumber;
	if(lVersionNumber != 0)
	{
		UINT16 length = 0x55U;/*length */
		UINT32 lOffset = 0x8U; /*offset*/
		lFFFISMessage->Buff[0] = 1;
		StmWriteLength(&lFFFISMessage->Buff[1], length);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+13, &lVersionNumber->MmiStmN058Vermajor, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+21, &lVersionNumber->MmiStmN058Vermid, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+29, &lVersionNumber->MmiStmN058Verminor, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+37, &lVersionNumber->MmiStmN035Vermajor, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+45, &lVersionNumber->MmiStmN035Vermid, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+53, &lVersionNumber->MmiStmN035Verminor, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+61, &lVersionNumber->MmiStmNSrsVermajor, 0, 8);
		CopyBitsInv( &lFFFISMessage->Buff[0], lOffset+69, &lVersionNumber->MmiStmNSrsVerminor, 0, 8);
		write_localdump(&lVersionNumber->MmiStmNSrsVerminor, lOffset+69, 8, &lFFFISMessage->Buff[0]);
		lFFFISMessage->NumBytes = ConvBit2Byte(length);/*add the two byte of header*/
		mon_broadcast_printf("Build_StmVersionNumber_\n");
	}/*if*/
}

void Build_MmiStmButtonEventReport_(t_FFFISMessage * const lFFFISMessage)
{
	t_MmiStmButtonEventReport *lMmiStmButtonEventReport =
	(t_MmiStmButtonEventReport *)&GetStmSts()->MmiStmButtonEventReport;
	if((lFFFISMessage != 0) && (lMmiStmButtonEventReport !=0 ))
	{
		UINT32 lOffset = 8U;  /*offset used to compute the data*/
		UINT16 lLength = 0x1A; /*variable used to copy the legth*/
		UINT16 ii = 0;
		UINT32 lEvent = lMmiStmButtonEventReport->Stm34Iter[0].MmiStmTButtonEvent;
		lLength = lLength + lMmiStmButtonEventReport->MmiStmNIter*49;
		lFFFISMessage->Buff[0] = 34;
		StmWriteLength(&lFFFISMessage->Buff[1], lLength);
    CopyBitsInv(&lFFFISMessage->Buff[0],  lOffset+13, (UINT8*)&(lMmiStmButtonEventReport->MmiStmNIter), 3, 5);
		mon_broadcast_printf("Iter = %X\n",lMmiStmButtonEventReport->MmiStmNIter);
		mon_broadcast_printf("lLength = %X\n",lLength);
		if(lMmiStmButtonEventReport->MmiStmNIter < 31)
		{
			lOffset+=18;
			for(; ii <=lMmiStmButtonEventReport->MmiStmNIter;ii++)
			{
				CopyBitsInv(&lFFFISMessage->Buff[0], lOffset,   &lMmiStmButtonEventReport->Stm34Iter[ii].MmiNidStm, 0, 8);
				CopyBitsInv(&lFFFISMessage->Buff[0], lOffset+8,  &lMmiStmButtonEventReport->Stm34Iter[ii].MmiStmNidButton, 0, 8);
				CopyBitsInv(&lFFFISMessage->Buff[0], lOffset+16, &lMmiStmButtonEventReport->Stm34Iter[ii].MmiStmQButton, 7, 1);
				CopyBitsInv(&lFFFISMessage->Buff[0], lOffset+17, (UINT8*)&lEvent, 0, 32);
        lOffset+=49;
			}/*for*/
		}/*if*/
		lFFFISMessage->NumBytes = ConvBit2Byte(lLength);/*add the two byte of header*/
		for(ii = 0; ii < lFFFISMessage->NumBytes; ii++)
			mon_broadcast_printf("lFFFISMessage->Buff[%d] = %d\n", ii, lFFFISMessage->Buff[ii]);

		mon_broadcast_printf("Build_MmiStmButtonEventReport_\n");
	}/**/
}
