/**
* \file Hmi_TextMessage.c
* \brief the file manages the private functions of EVC-8 text message to hmi
* \author Muoio Salvatore
*/

#include "Hmi_TextMessage.h"
#include "Hmi_TextMessage_Public.h"
#include "GPP_Handle_Public.h"
#include "Ccuo_Handle_Public.h"
#include "Generic_api.h"

t_StoreTextMessage StoreMessage[NUM_MAX_MESSAGE];
/**
* \var static INT32 index_message
* \brief static variable used set the index of message
*/
static INT32 index_message = 0;
/**
* \var static UINT32 lNewEvc8Text
* \brief static variable used set the NewEvc8Text
*/
static UINT32 lNewEvc8Text = 0U;

/**
* \fn static INT32 GetIndexMessage(void)
* \brief The function returns the value of variable of index messages
*/
static INT32 GetIndexMessage(void)
{
  return(index_message);
}

/**
* \fn static void set_newevc8txtbit(UINT16 aMmiQTxt, UINT32 *pBitMask)
* \brief The function creates the frame to HMI about the evc8 text message
* \param aMmiQTxt q text from evc-8
* \param pBitMask pointer output
*/
static void set_newevc8txtbit(UINT16 aMmiQTxt, UINT32 *pBitMask)
{
  /*!!!*/
  if(pBitMask != 0)
  {
    switch (aMmiQTxt)
       {
          case 260:
             {
                *pBitMask       = SET_EVC8_260_BIT;
                break;
             }
          case 261:
             {
                *pBitMask       = SET_EVC8_261_BIT;
                break;
             }
          case 270:
             {
                *pBitMask       = SET_EVC8_270_BIT;
                break;
             }
          case 271:
             {
                *pBitMask       = SET_EVC8_271_BIT;
                break;
             }
          case 272:
             {
                *pBitMask       = SET_EVC8_272_BIT;
                break;
             }
          case 514:
             {
                *pBitMask       = SET_EVC8_514_BIT;
                break;
             }
          case 515:
             {
                *pBitMask       = SET_EVC8_515_BIT;
                break;
             }
          case 516:
             {
                *pBitMask       = SET_EVC8_516_BIT;
                break;
             }
          case 517:
             {
                *pBitMask       = SET_EVC8_517_BIT;
                break;
             }
          case 519:
             {
                *pBitMask       = SET_EVC8_519_BIT;
                break;
             }
          case 521:
             {
                *pBitMask       = SET_EVC8_521_BIT;
                break;
             }
          case 524:
             {
                *pBitMask       = SET_EVC8_524_BIT;
                break;
             }
          case 525:
             {
                *pBitMask       = SET_EVC8_525_BIT;
                break;
             }
          case 526:
             {
                *pBitMask       = SET_EVC8_526_BIT;
                break;
             }
          case 527:
             {
                *pBitMask       = SET_EVC8_527_BIT;
                break;
             }
          case 528:
             {
                *pBitMask       = SET_EVC8_528_BIT;
                break;
             }
          case 535:
             {
                *pBitMask       = SET_EVC8_535_BIT;
                break;
             }
          case 536:
             {
                *pBitMask       = SET_EVC8_536_BIT;
                break;
             }
          default:
             {
                /*no mapping possible*/
                *pBitMask       = 0u;
             }
       }/*switch*/
     }/*if*/
}

static void isMMITextinTheDatabase(UINT8 aIText)
{
	UINT32 lBitMask     = 0U;
    INT32 ii = 0;
	  for (; ii < GetIndexMessage(); ii++)
	  {
			if ((StoreMessage[ii].MmiIText) == aIText)
			{
				mon_broadcast_printf("<isMMITextinTheDatabase: StoreMessage[ii].MmiIText %d>\n",StoreMessage[ii].MmiIText);
				mon_broadcast_printf("<isMMITextinTheDatabase:aIText %d>\n",aIText);
				/*index_message = index_message - 1;*/
        DecrementIndex(&index_message);
				/*reset the evc8 bitmask to hmi*/
				mon_broadcast_printf("<isMMITextinTheDatabase:StoreMessage[ii].MmiQText %d>\n",StoreMessage[ii].MmiQText);
				set_newevc8txtbit(StoreMessage[ii].MmiQText, &lBitMask);
				mon_broadcast_printf("<isMMITextinTheDatabase:lBitMask %x>\n",lBitMask);
				lNewEvc8Text = lNewEvc8Text&~lBitMask;
				mon_broadcast_printf("<isMMITextinTheDatabase:lNewEvc8Text %x>\n",lNewEvc8Text);
				mon_broadcast_printf("<isMMITextinTheDatabase:index_message %d>\n",index_message);
				memset(&StoreMessage[ii], 0, sizeof(t_StoreTextMessage));
				break;
			}
	  }
}


void Hmi_StoreMessage(UINT8 *pIText)
{
  if(pIText != 0){
  if(isReceivedEVC8() == TRUE)/*EVC8 Received*/
  {

    UINT8 lMmiQTextCriteria = GetEvc8MmiQTextCriteria();

    if((lMmiQTextCriteria == Keep_Text_After_Ack)       ||
       (lMmiQTextCriteria == Add_Informative_Text))
    {
	  /*check if the MMIText is present use the new*/
	  UINT8 lMmiIText = GetEvc8MmiIText();
    mon_broadcast_printf("<Hmi_StoreMessage: lMmiIText %d>\n",lMmiIText);
	  isMMITextinTheDatabase(lMmiIText);
      /*store the packet!!!*/
      if(GetIndexMessage() <= NUM_MAX_MESSAGE)
      {
        mon_broadcast_printf("store the packet:index_message = %d\n", index_message);
        memcpy(&StoreMessage[GetIndexMessage()], &GetApplEvcData()->MmiDriverMessage,
               sizeof(t_StoreTextMessage));
		    /*index_message = index_message + 1;*/
        IncrementIndex(&index_message);
        mon_broadcast_printf("store the packet post:index_message = %d\n", index_message);
      }
    }
    else if((lMmiQTextCriteria == Remove_Text_after_Ack)||
            (lMmiQTextCriteria == Remove_Text_after_Ack_Nak))
              *pIText = GetEvc8MmiIText();
  }/*evc8 rec*/
}
}

BOOLEAN8 Get_NoSetSBVRequest(void)
{
  BOOLEAN8 lOut = FALSE;
  UINT16 lMmiQTextCriteria = GetEvc8MmiQText();
  lOut |= (lMmiQTextCriteria == Unable_start_Brake_Test);
  lOut |= (lMmiQTextCriteria == Brake_Test_in_Progress);
  lOut |= (lMmiQTextCriteria == Brake_Test_failed_new_Test);
  return(lOut);
}

void Hmi_DeleteMessage(UINT8 aTxtId)
{
  UINT8 lMmiQTextCriteria = GetEvc8MmiQTextCriteria();
  BOOLEAN8 bFound = FALSE;

  if(lMmiQTextCriteria == Remove_This_text)
  {
    /*check the message to delete*/
    if(GetIndexMessage() <= NUM_MAX_MESSAGE)
    {
      INT32 ii = 0;
	  for (; ii < GetIndexMessage(); ii++)
	  {
		    if (!bFound)
		    {
			       if ((StoreMessage[ii].MmiIText) == aTxtId)
			       {
				           bFound = (BOOLEAN8)TRUE;
		                 mon_broadcast_printf("Hmi_DeleteMessage:index_message = %d\n", index_message);
             }
		    }
		    else
		    {
			       memcpy(&StoreMessage[ii - 1], &StoreMessage[ii], sizeof(t_StoreTextMessage));

			       if (ii == ((GetIndexMessage()) - 1))
			       {
				           memset(&StoreMessage[ii], 0, sizeof(t_StoreTextMessage));
			       }
		    }
	  }/*for*/
	  if (bFound)
		  DecrementIndex(&index_message);
    }/*plausibility check!!!*/
  }/*if*/
}

void Hmi_OutputTxtMsg(DS_CCUT_HMI_TRN *const pHmiTrn)
{
  if(pHmiTrn != 0)
  {
    INT32 ii = GetIndexMessage();
    BOOLEAN8 lIsScreenDel = FALSE;
	UINT32 lBitMask     = 0U;
    UINT8 lMmiQTextCriteria = GetEvc8MmiQTextCriteria();
    /*UINT32 lNewEvc8Text = pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text&~lBitMask;*/
    isScreenDel(&lIsScreenDel);
	if(isReceivedEVC8() == TRUE){
    for(; ii >= 0; ii--){
      set_newevc8txtbit(StoreMessage[ii].MmiQText, &lBitMask);

      /*mon_broadcast_printf("<: set_newevc8txtbit %d %d %d %x>\n",
                           ii,
                           GetEvc8MmiIText(),
                           StoreMessage[ii].MmiQText,
                           lBitMask);*/

    switch (lMmiQTextCriteria) {
      case Keep_Text_After_Ack :
        if(lIsScreenDel == FALSE)
        {
          /*set mask Message Ack*/
          SetMaskIdRequest(TxtMsgForConfirm);
          pHmiTrn->TPT_RequestedDialog.Acknowledgment  = 2;
          lNewEvc8Text = lNewEvc8Text|lBitMask;
          pHmiTrn->TPT_ApproveTextMessage.TextId       = GetEvc8MmiQText();
          /*newevc8 and textid*/
          /*mon_broadcast_printf("<Hmi_OutputTxtMsg: Keep_Text_After_Ack %x %x %d %x>\n",
                               lNewEvc8Text,
                               lBitMask,
                               pHmiTrn->TPT_ApproveTextMessage.TextId,
                               pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text);*/
        }
      break;
      case Remove_Text_after_Ack:
      if(lIsScreenDel == FALSE)
      {
        /*set mask Message Ack*/
        SetMaskIdRequest(TxtMsgForConfirm);
        pHmiTrn->TPT_RequestedDialog.Acknowledgment = 2;
        /* textid*/
        pHmiTrn->TPT_ApproveTextMessage.TextId = GetEvc8MmiQText();

        /*mon_broadcast_printf("<Hmi_OutputTxtMsg: Remove_Text_after_Ack %x %x %d %x>\n",
                              lNewEvc8Text,
                              ~lBitMask,
                              pHmiTrn->TPT_ApproveTextMessage.TextId,
                              pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text);*/
      }
      break;
      case Remove_Text_after_Ack_Nak:
      if(lIsScreenDel == FALSE)
      {
        /*set mask Message Ack*/
        SetMaskIdRequest(TxtMsgForConfirm);
        pHmiTrn->TPT_RequestedDialog.Acknowledgment = 4;
        /* textid*/
        pHmiTrn->TPT_ApproveTextMessage.TextId = GetEvc8MmiQText();

        /*mon_broadcast_printf("<Hmi_OutputTxtMsg: Remove_Text_after_Ack_Nak %x %x %d %x>\n",
                              lNewEvc8Text,
                              lBitMask,
                              pHmiTrn->TPT_ApproveTextMessage.TextId,
                              pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text);*/
      }
      break;
      case Add_Informative_Text:
      if(lIsScreenDel == FALSE)
      {
        pHmiTrn->TPT_RequestedDialog.Acknowledgment = 0;
        /* newevc8*/
        lNewEvc8Text = lNewEvc8Text|lBitMask;

        /*mon_broadcast_printf("<Hmi_OutputTxtMsg: Add_Informative_Text %x %x %d >\n",
                              lNewEvc8Text,
                              lBitMask,
                              pHmiTrn->TPT_ApproveTextMessage.TextId);*/
      }
      break;
      case Remove_This_text:
      if(lIsScreenDel == FALSE)
      {
        if(GetEvc8MmiIText() == StoreMessage[ii].MmiIText){
          pHmiTrn->TPT_RequestedDialog.Acknowledgment = 0;
          SetMaskIdRequest(NavigationTask);

          /* newevc8*/
          lNewEvc8Text = lNewEvc8Text&~lBitMask;

          lMmiQTextCriteria = 255;
        }

       /* mon_broadcast_printf("<Hmi_OutputTxtMsg: Remove_This_text %x %x %d %x>\n",
                              lNewEvc8Text,
                              ~lBitMask,
                              pHmiTrn->TPT_ApproveTextMessage.TextId,
                              pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text);*/
      }
      break;
      default:
      break;
    }/*switch*/
  }/*for*/

  pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text = lNewEvc8Text;
  mon_broadcast_printf("<Hmi_OutputTxtMsg: NewEvc8Text %x >\n",
                              pHmiTrn->TPT_NonAckDriverMessage.NewEvc8Text);
  	  lBitMask = 0U;

  }/*if*/
  }
}

/**
* \fn static void Hmi_DataOutAck(DS_CCUT_HMI_TRN *const pHmiTrn, UINT8 sIText)
* \brief The function creates the frame to HMI about the evc8 text message
* \param pHmiTrn pointer to the structure DS_CCUT_HMI_TRN
* \param sIText Mmi I Text id stored
*/
static void Hmi_DataOutAck(DS_CCUT_HMI_TRN *const pHmiTrn, UINT8 sIText)
{
  if(pHmiTrn != 0)
  {
    BOOLEAN8 lIsScreenDel = FALSE;
    UINT8 lMmiQTextCriteria = GetEvc8MmiQTextCriteria();
    isScreenDel(&lIsScreenDel);
	if(isReceivedEVC8() == TRUE){
    switch ( lMmiQTextCriteria ) {
      case Remove_Text_after_Ack:
      if(lIsScreenDel == FALSE)
      {
        /*set mask Message Ack*/
        SetMaskIdRequest(TxtMsgForConfirm);
        pHmiTrn->TPT_RequestedDialog.Acknowledgment = 2;
        /* textid*/
        pHmiTrn->TPT_ApproveTextMessage.TextId = GetEvc8MmiQText();

        /*mon_broadcast_printf("<Hmi_DataOutAck: Remove_Text_after_Ack %d>\n",
                              pHmiTrn->TPT_ApproveTextMessage.TextId);*/
      }
      break;
      case Remove_Text_after_Ack_Nak:
      if(lIsScreenDel == FALSE)
      {
        /*set mask Message Ack*/
        SetMaskIdRequest(TxtMsgForConfirm);
        pHmiTrn->TPT_RequestedDialog.Acknowledgment = 4;
        /* textid*/
        pHmiTrn->TPT_ApproveTextMessage.TextId = GetEvc8MmiQText();

        /*mon_broadcast_printf("<Hmi_DataOutAck: Remove_Text_after_Ack_Nak %d>\n",
                              pHmiTrn->TPT_ApproveTextMessage.TextId);*/
      }
      break;
      case Remove_This_text:
      if(lIsScreenDel == FALSE)
      {
        if(GetEvc8MmiIText() == sIText){
          pHmiTrn->TPT_RequestedDialog.Acknowledgment = 0;
          SetMaskIdRequest(NavigationTask);

          /*mon_broadcast_printf("<Hmi_DataOutAck: Remove_This_text %d>\n",
                              GetEvc8MmiIText());*/

        }/*if*/
      }
      break;
    }/*switch*/
  }/*if*/
  }/*if*/
}

void Hmi_DataTransferPlaceHolder(TPT_NonAckDriverMessage * const pMsg)
{
  if(pMsg != 0)
  {
    UINT16 lMmiQTextCriteria = GetEvc8MmiQText();
    switch ( lMmiQTextCriteria ) {
      case Brake_Test_Timeout_hours:
        pMsg->ParamEvc8_525 = lMmiQTextCriteria;
        break;
      case New_ATP_Power_up_hours:
        pMsg->ParamEvc8_535 = lMmiQTextCriteria;
        break;
      default:
        pMsg->ParamEvc8_525 = 0;
        pMsg->ParamEvc8_535 = 0;
      break;
    }/*switch*/
  }/*if*/
}

void Hmi_HandleTextMessage(DS_CCUT_HMI_TRN *const pHmiTrn)
{
  static UINT8 sIText = 0;
  /*store message*/

  Hmi_StoreMessage(&sIText);
  Hmi_DeleteMessage(GetEvc8MmiIText());
  Hmi_OutputTxtMsg(pHmiTrn);
  Hmi_DataOutAck(pHmiTrn, sIText);
  Hmi_DataTransferPlaceHolder(&pHmiTrn->TPT_NonAckDriverMessage);
}
