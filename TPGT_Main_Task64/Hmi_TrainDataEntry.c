/**
* \file Hmi_TrainDataEntry.c
* \brief the file manages the private functions of hmi for the train data entry
* \author Muoio Salvatore
*/

#include "Task64_Def.h"
#include "GPP_Handle_Public.h"
#include "Ccuo_Handle_Public.h"
#include "Generic_api.h"
#include "Hmi_Request_Public.h"
#include "Hmi_TrainDataEntry.h"
#include "Hmi_TrainDataEntry_Public.h"

/**
* \var static t_EventTrigger EVC101_EventTrigger
* \brief static variable used to close the loop to abort the train data entry
*/
static t_EventTrigger EVC101_EventTrigger = {FALSE, FALSE};
/**
* \var static t_EventTrigger EVC123_EventTrigger
* \brief static variable used to send the EVC123 packet
*/
static t_EventTrigger EVC123_EventTrigger = {FALSE, FALSE};
/**
* \var static UINT8 sBrakeType_Ascii
* \brief static variable used to convert the brake value in ASCII
*/
static UINT8 sBrakeType_Ascii = 0;
/**
* \var static UINT8 sBrakePercentage_Ascii[3]
* \brief static variable used to convert the brake percentage value in ASCII
*/
static UINT8 sBrakePercentage_Ascii[3];
/**
* \var static UINT8 sTrainLength_Ascii[3]
* \brief static variable used to convert the train length value in ASCII
*/
static UINT8 sTrainLength_Ascii[3];
/**
* \var static UINT8 sVmax_Ascii[3]
* \brief static variable used to convert the maximum speed value in ASCII
*/
static UINT8 sVmax_Ascii[3];
/**
* \var static UINT8 sZda
* \brief static Zda variable, value of EVC-25 packet, used to know the mask
*/
static UINT8 sZda        = 0U;
/**
* \var static UINT8 sCheckMask
* \brief static variable used to store the number of mask
*/
static UINT8 sCheckMask = 0U;

/******* interface function ************/
UINT8 GetBrakeTypeAscii(void)
{
  return(sBrakeType_Ascii);
}

UINT8 *GetBrakePercentageAscii(void)
{
  return(&sBrakePercentage_Ascii[0]);
}

UINT8 *GetTrainLengthAscii(void)
{
  return(&sTrainLength_Ascii[0]);
}

UINT8 *GetVmaxAscii(void)
{
  return(&sVmax_Ascii[0]);
}

UINT8 GetLastEvc25Zda(void)
{
	return(sZda);
}

/**
* \var static BOOLEAN8 sConfirmed
* \brief static variable used to detect the rising edge
*        to set the sSend123 variable

static BOOLEAN8 sConfirmed = FALSE;*/
/**
* \fn static BOOLEAN8 CheckFeedBackEVC123(void)
* \brief The function detects when the CCUt have to send to EbiCab
*        the feedback about the train data entry function

static BOOLEAN8 CheckFeedBackEVC123(UINT8 lEvc25Rcvd)
{
	BOOLEAN8 lOut = FALSE;
	BOOLEAN8 lSequ2Confirm = (lEvc25Rcvd == 2);
	if(RisingEdge(lSequ2Confirm, &sConfirmed) == TRUE)
	{
		sSend123 = TRUE;
		mon_broadcast_printf("SET CLOSE SEQUENCE\n");
    lOut = TRUE;
	}
	return(lOut);
}*/


BOOLEAN8 ExitMaskZDEKontrollBild(const TPG_Commands *const pCmd,
                                 e_ZdeBild *const pZdeBildState)
{
  BOOLEAN8 lShow = FALSE;
  if((pCmd != 0)&&(pZdeBildState != 0))
  {
      UINT32 lEnableExitTDE = (GetEVC21EnableDriverRequest()&ENABLE_EXIT_TRAIN_DATA_ENTRY);
      if((lEnableExitTDE == ENABLE_EXIT_TRAIN_DATA_ENTRY) &&
         (pCmd->MessageId == HMI_MSG_ZDE_KONTROLL_CANCEL))
       {
         lShow = TRUE;
         /*set request mask number zug besy*/
         SetMaskIdRequest(TrainMenu);
         /*reset status*/
         *pZdeBildState &= ~Zde_KtrlBild;
       }/*if*/
  }/*if*/
  return(lShow);
}

BOOLEAN8 ExitMaskZDEZugDatenEingabe(const TPG_Commands * const pCmd,
                                    const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                                    e_ZdeBild *const pZdeBildState)
{
  UINT32 lEnableExitTDE = (GetEVC21EnableDriverRequest()&ENABLE_EXIT_TRAIN_DATA_ENTRY);
  BOOLEAN8 lShow = FALSE;

  if((pCmd != 0) && (pLzbPzb != 0))
  {
    lShow = TRUE;

    if(((GetDrvRqst() == DrvRqst_ZDE)                &&
       ((pCmd->MessageId == HMI_MSGID_EINGABE_CANCEL)||
       (pCmd->MessageId == HMI_MSGID_WARTE_CANCEL))))
    {/* Cancel button was pressed? yes */

        if((lEnableExitTDE == ENABLE_EXIT_TRAIN_DATA_ENTRY) &&
           (pLzbPzb->UserApproval == HMI_CANCELLED))
        {/* Cancel EVC-21 bit 6 was received? yes */

            /*set request mask number zug besy*/
            *pZdeBildState &= ~Zde_ZugDtEin;
        }/*if*/
    }
    else
    {
      if((GetDrvRqst() == DrvRqst_ZDE)                   &&
         (pCmd->MessageId == HMI_MSGID_ZUGADTEN_EINGABE) &&
         (pLzbPzb->UserApproval == HMI_APPROVED))
      {/* Train-Info-Enty ready? yes */

        /*reset request*/
        *pZdeBildState &= ~Zde_ZugDtEin;
      }
    }
  }/*if*/
  return(lShow);
}

BOOLEAN8 ExitMaskZDENote(const TPG_Commands * const pCmd,
                         const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                         e_ZdeBild *const pZdeBildState)
{
  BOOLEAN8 lShow = FALSE;
	if((pCmd != 0) && (pLzbPzb != 0))
	{
    lShow = TRUE;
    UINT32 lEnableExitTDE = (GetEVC21EnableDriverRequest()&ENABLE_EXIT_TRAIN_DATA_ENTRY);
		if((lEnableExitTDE == ENABLE_EXIT_TRAIN_DATA_ENTRY) &&
       (pCmd->MessageId == HMI_MSGID_EINGABE_CANCEL) &&
       (pLzbPzb->UserApproval == HMI_CANCELLED))
		{
			/*set request mask number zug besy*/
      SetMaskIdRequest(TrainMenu);
      *pZdeBildState &= ~Zde_Note;
		}/*if*/
    else
    {
      if((pCmd->MessageId == HMI_MSGID_ZUGADTEN_EINGABE) &&
         (pLzbPzb->UserApproval == HMI_APPROVED))
      /*reset request*/
       *pZdeBildState &= ~Zde_Note;
    }
	}
	return(lShow);
}

BOOLEAN8 ExitMaskZDEConfirm(UINT8 aEvc25Rvcd, e_ZdeBild *const pZdeBildState)
{
	BOOLEAN8 lShow = FALSE;
	if(aEvc25Rvcd == 2)
	{
		/*set request zugbesy */
    SetMaskIdRequest(TrainMenu);
    *pZdeBildState &= ~Zde_Confirm;
    SetEvc123Request(EVC_123_REQUEST_CLOSE);
	}/*if*/
  lShow = TRUE;
	return(lShow);
}

BOOLEAN8 ExitMaskZDETestFun(const TPG_Commands * const pCmd,
                            e_ZdeBild *const pZdeBildState)
{
	BOOLEAN8 lShow = FALSE;
	if(pCmd != 0)
	{
			lShow = TRUE;
			/*set request mask number*/
			if(pCmd->MessageId == HMI_MSG_ZD_ANZEIGE_CANCEL)
			{
				/* set request zugbesy */
        SetMaskIdRequest(TrainMenu);
        *pZdeBildState &= ~Zde_TestFun;
			}/*if*/
	}/*if*/
	return(lShow);
}


BOOLEAN8 ShowEffectiveValueTrainDataEntry(TPT_LZBPZBTrainDataDisplay * const pOut,
                                          e_ZdeBild ZdeBildState)
{
  const int TDE_SHOW_OK = 0x1F;
  BOOLEAN8 lOut = FALSE;
  if(pOut != 0)
  {
    if((ZdeBildState&TDE_SHOW_OK) != 0)
    {
      /*set the data to display in the HMI*/
      static UINT8 lZda    = 0U;
      static UINT8 lBra    = 0U;
      static UINT8 lBrh[3] = {0,0,0};
	    static UINT8 lVmz[3] = {0,0,0};
	    static UINT8 lZl[3]  = {0,0,0};
	    if(isReceivedEVC25() == TRUE){
        GetEvc25Zda(&lZda);
        GetEvc25Bra(&lBra);
        GetEvc25Brh(&lBrh[0], &lBrh[1], &lBrh[2]);
        GetEvc25Vmz(&lVmz[0], &lVmz[1], &lVmz[2]);
        GetEvc25Zl(&lZl[0],  &lZl[1],   &lZl[2]);
	     }
      pOut->BrakeType  = lBra;
      pOut->TextId     = lZda+1;
      memcpy((UINT8*)(&pOut->BrakePercentage[0]),(UINT8*)&lBrh[0], sizeof(lBrh));
      memcpy((UINT8*)(&pOut->Vmax[0]),(UINT8*)&lVmz[0], sizeof(lVmz));
      memcpy((UINT8*)(&pOut->TrainLength[0]),(UINT8*)&lZl[0], sizeof(lZl));
    }/*if*/
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 SetValueTrainDataEntry(TPT_LZBPZBTrainDataEdit *const pOut,
                                e_ZdeBild ZdeBildState)
{
  BOOLEAN8 lOut = FALSE;
  const int TD_SEND_OK = 0xF;
  if(pOut != 0)
  {
    if((ZdeBildState&TD_SEND_OK) != 0)
    {
      UINT16 l7SegToNum = 0U;
      static UINT8 lBra = 0U;
      static UINT8 lBrh[3] = {0,0,0};
	    static UINT8 lVmz[3] = {0,0,0};
	    static UINT8 lZl[3]  = {0,0,0};
	    if(isReceivedEVC25() == TRUE){
        /*GetEvc25Zda(&lZda);*/
        GetEvc25Bra(&lBra);
        GetEvc25Brh(&lBrh[0], &lBrh[1], &lBrh[2]);
        GetEvc25Vmz(&lVmz[0], &lVmz[1], &lVmz[2]);
        GetEvc25Zl(&lZl[0],  &lZl[1],   &lZl[2]);
	     }
      /*set the data to edit in the HMI*/
      /*we have to convert in 7 segment*/
      Convert7SegArr2NumUint16(&lBra, 1, &l7SegToNum);
      pOut->BrakeType = l7SegToNum;
      Convert7SegArr2NumUint16(&lBrh[0], 3, &l7SegToNum);
      pOut->BrakePercentage = l7SegToNum;
      Convert7SegArr2NumUint16(&lVmz[0], 3, &l7SegToNum);
      pOut->Vmax = l7SegToNum;
      Convert7SegArr2NumUint16(&lZl[0], 3, &l7SegToNum);
      pOut->TrainLength = l7SegToNum;
    }
    lOut = TRUE;
  }
  return(lOut);
}


BOOLEAN8 TrasmitData(const TPG_Commands *const pCmd,
                     const TPT_LZBPZBTrainDataInput *const pTdi)
{
  /*static BOOLEAN8 sTrig = FALSE;*/
  BOOLEAN8 lOut = FALSE;
  if((pCmd != 0)&&(pTdi != 0))
  {
    if (((GetMaskIdRequest() == TrainDataInput)           &&
         (pCmd->MessageId == HMI_MSGID_ZUGADTEN_EINGABE)  &&
         (pTdi->UserApproval == HMI_APPROVED))            ||
        ((GetMaskIdRequest() == TrainDataFinal)           &&
         (pCmd->MessageId == HMI_MSG_ZDE_BESTAETIGUNG)    &&
         (pTdi->UserApproval == HMI_APPROVED))            ||
         (isEVC25IterNull() == TRUE))
    {
      /*set the value in ascii*/
      ConvertNumericU16ValToAscii(pTdi->BrakeType, &sBrakeType_Ascii, 1);
      ConvertNumericU16ValToAscii(pTdi->BrakePercentage, &sBrakePercentage_Ascii[0], 3);
      ConvertNumericU16ValToAscii(pTdi->TrainLength, &sTrainLength_Ascii[0], 3);
      ConvertNumericU16ValToAscii(pTdi->Vmax, &sVmax_Ascii[0], 3);
      /*SetEvcRequest123*/
      if(GetTriggerStatus(&EVC123_EventTrigger) == FALSE)
      /*if(sTrig == FALSE)*/{
        /*sTrig = TRUE;*/
        SetEvc123Request(EVC_123_REQUEST_SET);
        SetTrigger(&EVC123_EventTrigger);
        mon_broadcast_printf("[TrasmimtData]::set_evc_123\n");
      }
    }
    else
	  {
      /*sTrig = FALSE;*/
      ResetTrigger(&EVC123_EventTrigger);
      /*check if evc 25 is received*/
      if(isReceivedEVC25() == TRUE)
      {
        /*reply the evc_25 with EVC_123*/
        if((GetMaskIdRequest() != TrainDataInput) &&
           (GetMaskIdRequest() != TrainDataFinal))
        {
          UINT8 lBra = 0U;
          UINT8 lBrh[3] = {0,0,0};
  	      UINT8 lVmz[3] = {0,0,0};
  	      UINT8 lZl[3]  = {0,0,0};
          GetEvc25Bra(&lBra);
          GetEvc25Brh(&lBrh[0], &lBrh[1], &lBrh[2]);
          GetEvc25Vmz(&lVmz[0], &lVmz[1], &lVmz[2]);
          GetEvc25Zl(&lZl[0],  &lZl[1],   &lZl[2]);
          /*copy the data*/
          sBrakeType_Ascii = lBra;
          memcpy(&sBrakePercentage_Ascii[0], &lBrh[0], 3);
          memcpy(&sTrainLength_Ascii[0], &lZl[0], 3);
          memcpy(&sVmax_Ascii[0], &lVmz[0], 3);
          SetEvc123Request(EVC_123_REQUEST_SET);
          mon_broadcast_printf("[TrasmimtData::reply]::set_evc_123\n");
        }
      }
	  }
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 AbortTraindDataEntry(const TPG_Commands *const pCmd,
                              const TPT_LZBPZBTrainDataInput * const pLzbPzb)
{
  /*static BOOLEAN8 lSendEVC101 = FALSE;*/
  BOOLEAN8 lOut = FALSE;
  if((pCmd != 0)&&(pLzbPzb != 0)){
  BOOLEAN8 lIsEbicabAlive = FALSE;
  UINT32 lEnableTDEntry = (GetEVC21EnableDriverRequest()&ENABLE_START_TRAIN_DATA_ENTRY);
  UINT32 lEnableExitTDE = (GetEVC21EnableDriverRequest()&ENABLE_EXIT_TRAIN_DATA_ENTRY);
  UINT32 lEnableTDView  = (GetEVC21EnableDriverRequest()&ENABLE_START_TRAIN_DATA_VIEW);
  /*check condition*/
  isEbiCabAlive(&lIsEbicabAlive);
  if(GetDrvRqst() == DrvRqst_ZDE)
  {
    if((GetTriggerStatus(&EVC101_EventTrigger) == FALSE)&&
        /*(lEnableExitTDE == ENABLE_EXIT_TRAIN_DATA_ENTRY)&&*/
                    ((pCmd->MessageId == HMI_MSGID_EINGABE_CANCEL)
                     ||(pCmd->MessageId == HMI_MSGID_WARTE_CANCEL)
                     ||((pLzbPzb->UserApproval == HMI_CANCELLED)&&((pCmd->MessageId == HMI_MSG_ZDE_BESTAETIGUNG)
                                                                   ||(pCmd->MessageId == HMI_MSG_ZDE_KONTROLL_CANCEL)
                                                                   ||(pCmd->MessageId == HMI_MSGID_EINGABE_CANCEL)))))
    {
      /*reset Driver request ZDE
      sSend123 = FALSE;*/
      /*ResetDrvRqst();*/
      /*EVC_123.MMI_STM_N_ITER = 0*/
      SetEvc123Request(EVC_123_REQUEST_CLOSE);
      /*SetMaskIdRequest(TrainZugBesy);*/

      mon_broadcast_printf("Abort::ZDE::EVC_123::0\n");

      /* unlock EVC101 sending */
      SetTrigger(&EVC101_EventTrigger);
      /*lSendEVC101 = TRUE;*/
    }
    else if((GetTriggerStatus(&EVC101_EventTrigger) == TRUE)&&
       /*(lEnableExitTDE != ENABLE_EXIT_TRAIN_DATA_ENTRY)&&*/
                    ((pCmd->MessageId == HMI_MSGID_EINGABE_CANCEL)
                     ||(pCmd->MessageId == HMI_MSGID_WARTE_CANCEL)
                     ||((pLzbPzb->UserApproval == HMI_CANCELLED)&&((pCmd->MessageId == HMI_MSG_ZDE_BESTAETIGUNG)
                                                                   ||(pCmd->MessageId == HMI_MSG_ZDE_KONTROLL_CANCEL)
                                                                   ||(pCmd->MessageId == HMI_MSGID_EINGABE_CANCEL)))))
    {
        ResetDrvRqst();
        SetMaskIdRequest(NavigationTask);

        /* send EVC101 request */
        SetEvc101Request(EVC101_ZDE_CANCEL);
        mon_broadcast_printf("Abort::ZDE::EVC_101::4\n");

        /* lock EVC101 sending */
        /*lSendEVC101 = FALSE;*/
        ResetTrigger(&EVC101_EventTrigger);
    }
    else if((lIsEbicabAlive == FALSE) ||
            (lEnableTDEntry != ENABLE_START_TRAIN_DATA_ENTRY))
    {
      ResetDrvRqst();
      SetMaskIdRequest(NavigationTask);
      mon_broadcast_printf("Abort::ZDE::1\n");

      /* lock EVC101 sending */
      /*lSendEVC101 = FALSE;*/
      ResetTrigger(&EVC101_EventTrigger);
    }
  }
  /*if is active ZD request*/
  if(GetDrvRqst() == DrvRqst_ZD)
  {
   if((pCmd->MessageId == HMI_MSG_ZD_ANZEIGE_CANCEL)  ||
      (lEnableTDView != ENABLE_START_TRAIN_DATA_VIEW) ||
      (lIsEbicabAlive == FALSE))
   {
     /*reset Driver request ZD*/
     ResetDrvRqst();
     SetMaskIdRequest(NavigationTask);
     mon_broadcast_printf("Abort::ZD::1\n");
   }
  }
  lOut = TRUE;
}
return(lOut);
}

/**
* \fn static UINT8 LookUpTableStsFms(UINT8 aIndex)
* \brief The function detects the status of state machine from Zda value
* \param aIndex, Zda value
*/
static UINT8 LookUpTableStsFms(UINT8 aIndex)
{
  static const UINT8 Table[] = {FMS_STS_KONTROL_BILD,
                                FMS_STS_TRAIN_DATA_ENTRY,
                                FMS_STS_TRAIN_DATA_ENTRY,
                                FMS_STS_NOTE,
                                FMS_STS_CONFIRM,
                                FMS_STS_NOTE,
                                FMS_STS_NOTE,
                                FMS_STS_NOTE,
                                FMS_STS_UNKNOWN,
                                FMS_STS_TRAIN_DATA_ENTRY,
                                FMS_STS_NOTE,
                                FMS_STS_UNKNOWN,
                                FMS_STS_TEST_FUN,
                                FMS_STS_TEST_FUN,
                                FMS_STS_TEST_FUN,
                                FMS_STS_NOTE};
  return(Table[aIndex]);
}

/**
* \fn static void CheckMaskZDE(UINT8 aZda, e_ZdeBild *sZdeBildState)
* \brief The function checks which mask id should be set
* \param aZda, Zda value
* \param sZdeBildState pointer to the enum structure e_ZdeBild
*/
static void CheckMaskZDE(UINT8 aZda, e_ZdeBild *sZdeBildState)
{
  UINT8 ii = LookUpTableStsFms(aZda);

  switch(ii)
  {
    case FMS_STS_KONTROL_BILD:
    {
      /*Show Kontrolbild*/
      SetMaskIdRequest(TrainDataOneColumn);
      *sZdeBildState |= Zde_KtrlBild;
      SetPendingDisplay(FALSE);
      sCheckMask = Zde_KtrlBild;
    }
    break;
    case FMS_STS_TRAIN_DATA_ENTRY:
    {
      /*Show Train Data Entry*/
      SetMaskIdRequest(TrainDataInput);
      *sZdeBildState |= Zde_ZugDtEin;
      SetPendingDisplay(FALSE);
      sCheckMask = Zde_ZugDtEin;
    }
    break;
    case FMS_STS_NOTE:
    {
      /*Show ZDENote*/
      SetMaskIdRequest(TrainDataStatic);
      *sZdeBildState |= Zde_Note;
      SetPendingDisplay(FALSE);
      sCheckMask = Zde_Note;
      /*Get EVC25 data and echo via evc 123*/
    }
    break;
    case FMS_STS_CONFIRM:
      /*Show ZDEConfirm*/
      SetMaskIdRequest(TrainDataFinal);
      *sZdeBildState |= Zde_Confirm;
      SetPendingDisplay(FALSE);
      sCheckMask = Zde_Confirm;
    break;
    case FMS_STS_TEST_FUN:
      /*Show ZDETestFun*/
      SetMaskIdRequest(TrainDataChkScreen);
      *sZdeBildState |= Zde_TestFun;
      SetPendingDisplay(FALSE);
      sCheckMask = Zde_TestFun;
    break;
    default:
    break;
  }
}


void MaskZDEHandle(const TPG_Commands * const pCmd,
                   const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                   DS_CCUT_HMI_TRN * const pOut)
{
  static e_ZdeBild sZdeBildState = Zde_Idle;
  if((pCmd != 0) && (pLzbPzb != 0) && (pOut != 0)){
  UINT8 lEvc25Rcvd = GetEvc25Zda(&sZda);
  /*check if there is the abort of Train Data Entry*/
  AbortTraindDataEntry(pCmd, pLzbPzb);
  /*finite state machine*/
  /*request to enable the train data mask*/
  if(GetDrvRqst() == DrvRqst_ZDE)
  {
    if(isReceivedEVC25() == TRUE){
    CheckMaskZDE(sZda, &sZdeBildState);
    }
    /*check the condition to exit from the mask*/
    switch(sCheckMask)
    {
      case Zde_KtrlBild:
        ExitMaskZDEKontrollBild(pCmd, &sZdeBildState);
      break;
      case Zde_ZugDtEin:
        ExitMaskZDEZugDatenEingabe(pCmd, pLzbPzb,&sZdeBildState);
      break;
      case Zde_Confirm:
        ExitMaskZDEConfirm(lEvc25Rcvd,&sZdeBildState);
      break;
      case Zde_TestFun:
        ExitMaskZDETestFun(pCmd,&sZdeBildState);
      break;
      case Zde_Note:
        ExitMaskZDENote(pCmd, pLzbPzb,&sZdeBildState);
      break;
      default:
      break;
    }/*switch*/
  /*}if request*/
  /*send data to HMI, data from Ebicab!!!!*/
  ShowEffectiveValueTrainDataEntry(&pOut->TPT_LZBPZBTrainDataDisplay, sZdeBildState);
  SetValueTrainDataEntry(&pOut->TPT_LZBPZBTrainDataEdit, sZdeBildState);
  TrasmitData(pCmd, pLzbPzb);
  }
}
}

/*ZD_view*/
BOOLEAN8 ShowValueTrainDataView(TPT_LZBPZBTrainDataDisplay * const pOut)
{
  BOOLEAN8 lOut = FALSE;
  const UINT8 TextIdTrainDataView = 7;
  if(pOut != 0)
  {
      /*set the data to display in the HMI*/
      static UINT8 lBra = 0U;
      static UINT8 lBrh[3] = {0,0,0};
	    static UINT8 lVmz[3] = {0,0,0};
	    static UINT8 lZl[3]  = {0,0,0};
      UINT8 l7Seg[3]       = {0,0,0};

	    if(isReceivedEVC26() == TRUE){
        /*GetEvc25Zda(&lZda);*/
        GetEvc26Bra(&lBra);
        GetEvc26Brh(&lBrh[0], &lBrh[1], &lBrh[2]);
        GetEvc26Vmz(&lVmz[0], &lVmz[1], &lVmz[2]);
        GetEvc26Zl(&lZl[0],  &lZl[1],   &lZl[2]);
	     }
      /*set the data to edit in the HMI*/
      /*we have to convert in 7 segment*/
      ConvertAsciiTo7Seg(&lBra, 1, &l7Seg[0], 1);
      pOut->BrakeType = l7Seg[0];
      ConvertAsciiTo7Seg(&lBrh[0], 3, &l7Seg[0], 3);
      memcpy((UINT8*)(&pOut->BrakePercentage[0]), &l7Seg[0], sizeof(l7Seg));
      ConvertAsciiTo7Seg(&lVmz[0], 3, &l7Seg[0], 3);
      memcpy((UINT8*)(&pOut->Vmax[0]), &l7Seg[0], sizeof(l7Seg));
      ConvertAsciiTo7Seg(&lZl[0], 3, &l7Seg[0], 3);
      memcpy((UINT8*)(&pOut->TrainLength[0]), &l7Seg[0], sizeof(l7Seg));
      pOut->TextId = TextIdTrainDataView;
    lOut = TRUE;
  }
  return(lOut);
}

void MaskZDHandle(const TPG_Commands * const pCmd,
                   const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                   DS_CCUT_HMI_TRN * const pOut)
{
  if((pCmd != 0) && (pLzbPzb != 0) && (pOut != 0)){
  /*check if there is the abort of Train Data Entry*/
  AbortTraindDataEntry(pCmd, pLzbPzb);
  /*finite state machine*/
  /*request to enable the train data mask*/
  if((GetDrvRqst() == DrvRqst_ZD) && (isReceivedEVC26() == TRUE))
  {
    SetMaskIdRequest(TrainDataChkScreen);
    SetPendingDisplay(FALSE);
    ShowValueTrainDataView(&pOut->TPT_LZBPZBTrainDataDisplay);
    /*check the condition to exit from the mask*/
    mon_broadcast_printf("MaskZDHandle\n");
  }/*if request*/
}
}
