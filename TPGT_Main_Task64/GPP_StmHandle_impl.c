/**
* \file Gpp_StmHandle_impl.c
* \brief the file manages the application functions used to create the GPP STM packet
*        to send via MVB. The cration of this packet is managed in the
*        application task
* \author Muoio Salvatore
*/
#include "Task64_Def.h"
#include "GPP_Handle_Public.h"
#include "GPP_StmHandle_impl.h"
#include "Hmi_Request_Public.h"
#include "Ccuo_Handle_Public.h"

/**
* \var UINT16 index_packet2write
* \brief static index used in the shared memory to write the packet
*/
static UINT16 index_packet2write = 0x0U;
/**
* \fn static void set_packet2write(UINT16 aStmMPacket)
* \brief set the packet to send
* \param aStmMPacket number of packet
*/
static void set_packet2write(UINT16 aStmMPacket)
{
   /*write the MMi id in the bufer Pack2Write*/
  GetApplStmSts()->Pack2Write[index_packet2write++] = aStmMPacket;
}

void reset_stm_index_packet2write(void)
{
  /*reset the index of packet */
  index_packet2write = 0x0U;
  memset(&(GetApplStmSts()->Pack2Write[0]), 0, sizeof(GetApplStmSts()->Pack2Write));
}


void Create_StmVersionNumber(void)
{
  t_VersionNumber *lVersionNumber =
                  (t_VersionNumber *)&GetApplStmSts()->VersionNumber;
  if(lVersionNumber != 0)
  {
    if(isReceivedSTM1() == TRUE)
    {
      const UINT16 STM_1 = 1;
      /*echo data!!!*/
      lVersionNumber->MmiStmN058Vermajor = GetApplStmData()->VersionNumber.MmiStmN058Vermajor;
      lVersionNumber->MmiStmN058Vermid   = GetApplStmData()->VersionNumber.MmiStmN058Vermid;
      lVersionNumber->MmiStmN058Verminor = GetApplStmData()->VersionNumber.MmiStmN058Verminor;
      lVersionNumber->MmiStmN035Vermajor = GetApplStmData()->VersionNumber.MmiStmN035Vermajor;
      lVersionNumber->MmiStmN035Vermid   = GetApplStmData()->VersionNumber.MmiStmN035Vermid;
      lVersionNumber->MmiStmN035Verminor = GetApplStmData()->VersionNumber.MmiStmN035Verminor;
      lVersionNumber->MmiStmNSrsVermajor = GetApplStmData()->VersionNumber.MmiStmNSrsVermajor;
      lVersionNumber->MmiStmNSrsVerminor = GetApplStmData()->VersionNumber.MmiStmNSrsVerminor;
      set_packet2write(STM_1);/*index 0 == STM1*/
      mon_broadcast_printf("build STM 1\n");
    }
  }
}

void Create_MmiStmButtonEventReport(void)
{
	t_MmiStmButtonEventReport *lMmiStmButtonEventReport =
	(t_MmiStmButtonEventReport*)&GetApplStmSts()->MmiStmButtonEventReport;

	if(lMmiStmButtonEventReport != 0){
		const UINT16 STM_34 = 34;
		/*if request to write*/
		UINT8 lRequest = is_STM34_requested();
		if(lRequest != 0)
		{
      UINT32 lUtcTime = GetUtcTime();
			lMmiStmButtonEventReport->MmiStmNIter                     = 1;
			lMmiStmButtonEventReport->Stm34Iter[0].MmiNidStm          = 9;
			lMmiStmButtonEventReport->Stm34Iter[0].MmiStmNidButton    = 1;
			lMmiStmButtonEventReport->Stm34Iter[0].MmiStmQButton      = GetStm34QButton();
			lMmiStmButtonEventReport->Stm34Iter[0].MmiStmTButtonEvent = lUtcTime;
			set_packet2write(STM_34);/*index STM34*/
			mon_broadcast_printf("build STM 34\n");
		}
	}
}

/*****+ interface and api *******/
BOOLEAN8 isReceivedSTM1(void)
{
  return((GetApplStmData()->stm_packet[index_stm_1] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedSTM15(void)
{
  return((GetApplStmData()->stm_packet[index_stm_15] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedSTM32(void)
{
  return((GetApplStmData()->stm_packet[index_stm_32] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedSTM35(void)
{
  return((GetApplStmData()->stm_packet[index_stm_35] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedSTM38(void)
{
  return((GetApplStmData()->stm_packet[index_stm_38] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedSTM39(void)
{
  return((GetApplStmData()->stm_packet[index_stm_39] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedSTM46(void)
{
  return((GetApplStmData()->stm_packet[index_stm_46] == 1) ? TRUE : FALSE);
}



UINT8 GetMmiStmN058Vermajor(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmN058Vermajor);
}
UINT8 GetMmiStmN058Vermid(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmN058Vermid);
}
UINT8 GetMmiStmN058Verminor(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmN058Verminor);
}
UINT8 GetMmiStmN035Vermajor(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmN035Vermajor);
}
UINT8 GetMmiStmN035Vermid(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmN035Vermid);
}
UINT8 GetMmiStmN035Verminor(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmN035Verminor);
}
UINT8 GetMmiStmNSrsVermajor(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmNSrsVermajor);
}
UINT8 GetMmiStmNSrsVerminor(void)
{
  return(GetApplStmData()->VersionNumber.MmiStmNSrsVerminor);
}

UINT8 GetStm38MmiStmNid(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedSTM38() == TRUE)
  {
    lOut = GetApplStmData()->MmiStmTextMessage.MmiStmNidXMessage;
  }
  return(lOut);
}

UINT8 GetStm38MmiStmLTxt(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedSTM38() == TRUE)
  {
    if(GetApplStmData()->MmiStmTextMessage.MmiStmLText <= 40)
       lOut = GetApplStmData()->MmiStmTextMessage.MmiStmLText;
  }
  return(lOut);
}

UINT16 GetStm38MmiStmXAttribute(void)
{
  static UINT16 lOut = 0U;
  if(isReceivedSTM38() == TRUE)
  {
    lOut = GetApplStmData()->MmiStmTextMessage.MmiStmMXAttribute;
  }
  return(lOut);
}

UINT8 GetStm39MmiStmNid(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedSTM39() == TRUE)
    lOut = GetApplStmData()->MmiStmNidXMessage;
  return(lOut);
}

UINT8 GetStm46MmiStmNIter(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedSTM46() == TRUE)
    lOut = GetApplStmData()->MmiStmSoundCmd.MmiStmNIter.StmIter.Iter;
  return(lOut);
}

BOOLEAN8 GetStm46ZwangbremsungOnOff(UINT8 max_index, BOOLEAN8 *isSound)
{
  BOOLEAN8 lOut = FALSE;
  if(isSound != 0){
  if(isReceivedSTM46() == TRUE)
  {
    UINT8 ii = 0U;
    for(; ii < max_index; ii++)
    {
      if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == ZWANGSBREMSUNG) &&
        (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == 2))
        *isSound = TRUE;
      else if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == ZWANGSBREMSUNG) &&
              (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == STOP_SOUND))
        *isSound = FALSE;
    }/*for*/
  }
  lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 GetStm46ZugbeeinflussungOnOff(UINT8 max_index, BOOLEAN8 *isSound)
{
  BOOLEAN8 lOut = FALSE;
  const UINT8 ActiveSoundZugbeeInflussung = 2;
  if(isSound != 0){
  if(isReceivedSTM46() == TRUE)
  {
    UINT8 ii = 0U;
    for(; ii < max_index; ii++)
    {
      if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == ZUGBEEINFLUSSUNG) &&
        (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == ActiveSoundZugbeeInflussung))
        *isSound = TRUE;
      else if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == ZUGBEEINFLUSSUNG) &&
              (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == STOP_SOUND))
        *isSound = FALSE;
    }/*for*/
  }
  lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 GetStm46SchnarreOnOff(UINT8 max_index, UINT8 *isSound)
{
  BOOLEAN8 lOut = FALSE;
  if(isSound != 0){
  if(isReceivedSTM46() == TRUE)
  {
    UINT8 ii = 0U;
    for(; ii < max_index; ii++)
    {
      if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == SCHNARRE) &&
        (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == 2)           &&
        (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNIter  == 1))
        *isSound = 1;
      else if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == SCHNARRE) &&
              (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == 2)          &&
              (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNIter  == 2))
        *isSound = 2;
      else{
        if((GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound == SCHNARRE) &&
           (GetApplStmData()->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound == STOP_SOUND))
           *isSound = 0;
      }/*else*/
    }/*for*/
  }/*if*/
  lOut = TRUE;
  }
  return(lOut);
}


UINT8 GetStm35MmiStmNIter(void)
{
  UINT8 lOut = 0;
  if(isReceivedSTM35() == TRUE)
    lOut = GetApplStmData()->MmiStmIndicatorRequest.MmiStmNIter.StmIter.Iter;
  return(lOut);
}

UINT8 GetStm35MmiNidIndpos(UINT8 index)
{
  static UINT8 lOut = 0;
  if(isReceivedSTM35() == TRUE)
    lOut = GetApplStmData()->MmiStmIndicatorRequest.Stm35Iter1[index].MmiStmNidIndpos;
  return(lOut);
}

UINT8 GetStm35MmiNidIcon(UINT8 index)
{
  static UINT8 lOut = 0;
  if(isReceivedSTM35() == TRUE)
    lOut = GetApplStmData()->MmiStmIndicatorRequest.Stm35Iter1[index].MmiStmNidIcon;
  return(lOut);
}

UINT8 GetStm35MmiNidIcator(UINT8 index)
{
  static UINT8 lOut = 0;
  if(isReceivedSTM35() == TRUE)
    lOut = GetApplStmData()->MmiStmIndicatorRequest.Stm35Iter1[index].MmiStmNidIndicator;
  return(lOut);
}
UINT16 GetStm35MmiStmNidAttrib(UINT8 index)
{
  static UINT16 lOut = 0;
  if(isReceivedSTM35() == TRUE)
    lOut = GetApplStmData()->MmiStmIndicatorRequest.Stm35Iter1[index].MmiSstmMIndAttrib;
  return(lOut);
}


e_StmStateReport GetStmNidStateReport(void)
{
  static e_StmStateReport lOut = STATE_IDLE;
  if(isReceivedSTM15() == TRUE)
    lOut = GetApplStmData()->StateReport;
  return(lOut);
}

BOOLEAN8 GetButtonRequestTestLzbPzb(void)
{
  static BOOLEAN8 lRes = FALSE;
  if(isReceivedSTM32() == TRUE){
  UINT8 lIter = GetApplStmData()->ButtonRequest.MmiStmNIter.StmIter.Iter;
  if(lIter > 0)
  {
    UINT8 ii = 0U;
    for(; ii < lIter; ii++)
    {
      UINT16 lMMiStmMButAttrib = (GetApplStmData()->ButtonRequest.Data[ii].MMiStmMButAttrib &
                                  MASK_ATTRIBUT_LZBPZB_TEST);
      if((GetApplStmData()->ButtonRequest.Data[ii].MmiNidStm     == PZB_LZB) &&
         (GetApplStmData()->ButtonRequest.Data[ii].MmiStmButton  == 1)       &&
         (GetApplStmData()->ButtonRequest.Data[ii].MMiStmNidIcon == 1)       &&
         (lMMiStmMButAttrib == MASK_ATTRIBUT_LZBPZB_TEST))
         {
           lRes = TRUE;
           break;
         }
    }/*for*/
  }/*if*/
}
  return(lRes);
}
