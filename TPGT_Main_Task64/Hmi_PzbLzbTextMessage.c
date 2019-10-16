/**
* \file Hmi_PzbLzbTextMessage.c
* \brief the file manages the private functions of Pzb/LZB
*        text message to hmi(STM38/STM39)
* \author Muoio Salvatore
*/


#include "Generic_api.h"
#include "GPP_Handle_Public.h"
#include "Etcs_public.h"
#include "Hmi_PzbLzbTextMessage.h"

/**
* \def #define MAX_MESSAGE_PZBLZB
* \brief maximmum value of message stored
*/

#define MAX_MESSAGE_PZBLZB 3
/**
* \var static TPT_FixedTextMessage sMsg
* \brief static variable used to store the STM-38 message
*/
static TPT_FixedTextMessage sMsg[MAX_MESSAGE_PZBLZB];
/**
* \fn static BOOLEAN8 FindTextId(TPT_FixedTextMessage *pMsg, UINT8 aTextId)
* \brief The function checks the new message in the storage
* \param pMsg pointer to structure TPT_FixedTextMessage
* \param aTextId new text id
*/
static BOOLEAN8 FindTextId(TPT_FixedTextMessage *pMsg, UINT8 aTextId)
{
  return((pMsg->TextId == aTextId) ? TRUE : FALSE);
}
/**
* \fn static BOOLEAN8 isReserved(UINT16 aStmXAttribute)
* \brief The function creates the frame to HMI about the evc8 text message
* \param aStmXAttribute attribute value of message
* \param pBitMask pointer output
* \return TRUE reserved, false not reserved
*/
static BOOLEAN8 isReserved(UINT16 aStmXAttribute)
{
  BOOLEAN8 lOut = TRUE;
  if((aStmXAttribute&0x200U) != 0x0){
	   if((aStmXAttribute&0xC0)!= 0xC0)
		   lOut = FALSE;
  }
  return(lOut);
}
/**
* \fn static UINT8 SetColor(UINT16 aXAttribute)
* \brief The function sets the frame color
* \param aXAttribute attribute value of message
* \return the color value
*/
static UINT8 SetColor(UINT16 aXAttribute)
{
  return((aXAttribute)&7);
}

/**
* \fn static UINT8 LookUpTableFlashing(UINT16 aXAttribute)
* \brief The function checks the flashing status of text message
* \param aXAttribute attribute value of message
* \return flashing status
*/
static UINT8 LookUpTableFlashing(UINT16 aXAttribute)
{
    static const UINT8 Table[] = {NO_FALSHING, SLOW_FLASHING, FAST_FLASHING,
                                 NO_FALSHING, COUNTER_SLOW_FLASHING,
                                 COUNTER_FAST_FLASHING};
    return(Table[aXAttribute]);
}
/**
* \fn static void StoreAttributeMessage(TPT_FixedTextMessage *pMsg,
*                                       UINT8 aStmNidXMsg)
* \brief The function stores the message with attribute
* \param pMsg pointer to the structure TPT_FixedTextMessage
* \param aStmNidXMsg Nid value message
*/
static void StoreAttributeMessage(TPT_FixedTextMessage *pMsg, UINT8 aStmNidXMsg)
{
  if(pMsg != 0)
  {
    UINT16 lXAttribute = GetStm38MmiStmXAttribute();
    /*check the attribute of text message
     check the last bit(bit 9)  */
    if(isReserved(lXAttribute) ==  FALSE)
    {
	     INT16 i              = 0;
	     INT16 textIdIndex    = 0;
	     BOOLEAN8 textIdFound = FALSE;
       for (; i < MAX_MESSAGE_PZBLZB; i++)
       {
         if (FindTextId(&sMsg[i], aStmNidXMsg) == TRUE)
         {
           textIdIndex = i;
           textIdFound = (BOOLEAN8)TRUE;
           break;
         }
       }

       if (textIdFound == FALSE)
       {
		     sMsg[2] = sMsg[1];
		     sMsg[1] = sMsg[0];
         memset(&sMsg[0], 0, sizeof(TPT_FixedTextMessage));
         textIdIndex = (UINT8)0u;
       }
       sMsg[textIdIndex].TextId          = aStmNidXMsg;
       sMsg[textIdIndex].TextColor       = SetColor(lXAttribute);
       sMsg[textIdIndex].BackgroundColor = SetColor(lXAttribute>>3);
       sMsg[textIdIndex].TextFlashing    = LookUpTableFlashing(lXAttribute>>6);
    }
  }/*if*/
}



void Hmi_StorePzbLzbMessage(DS_CCUT_HMI_TRN * const pCmd)
{
 if(pCmd != 0)
 {
    /*get data from STM-39*/
	if(isReceivedSTM38() == TRUE)
	{
    /*plausibility check on length*/
		if(GetStm38MmiStmLTxt() != 0)
		{
			/*store the info!!!*/
			StoreAttributeMessage(sMsg, (GetStm38MmiStmNid()));
		}
	}
 }/*if pls pointer!!!*/
}



void Hmi_CancelPzbLzbMessage(DS_CCUT_HMI_TRN * const pCmd)
{
  if(pCmd != 0){
    if((isReceivedSTM39() == TRUE)){
        if(GetStm39MmiStmNid() == sMsg[0].TextId)
        {
          sMsg[0] = sMsg[1];
          sMsg[1] = sMsg[2];
        }
        else if ((GetStm39MmiStmNid() == sMsg[1].TextId))
        {
		      sMsg[1] = sMsg[2];
	      }
        memset(&sMsg[2], 0, sizeof(TPT_FixedTextMessage));
    }
  }/*if pls pointer!!!*/
}

void Hmi_PzbLzbMessage(DS_CCUT_HMI_TRN * const pCmd)
{
  if(pCmd != 0)
  {
    /*check if etcs is isolated*/
    BOOLEAN8 lIsEtcsIsolated = FALSE;
    BOOLEAN8 lIsScreenDel    = FALSE;
    isEtcsIsolated(&lIsEtcsIsolated);
    isScreenDel(&lIsScreenDel);
    if(lIsEtcsIsolated == FALSE)
    {
      if((GetStmModus() == STM_MODUS_DATA_AVAILABLE) && (lIsScreenDel == FALSE))
      {
        Hmi_CancelPzbLzbMessage(pCmd);
        Hmi_StorePzbLzbMessage(pCmd);
      }
      else
      {
    		/*set default value*/
         memset(&sMsg[0], 0, sizeof(TPT_FixedTextMessage));
         memset(&sMsg[1], 0, sizeof(TPT_FixedTextMessage));
         memset(&sMsg[2], 0, sizeof(TPT_FixedTextMessage));
      }
    }
    else
    {
      /*set default value*/
       memset(&sMsg[0], 0, sizeof(TPT_FixedTextMessage));
       memset(&sMsg[1], 0, sizeof(TPT_FixedTextMessage));
       memset(&sMsg[2], 0, sizeof(TPT_FixedTextMessage));
    }
    /*set data to HMI*/
    pCmd->TPT_FixedTextMessage1 = sMsg[0];
    pCmd->TPT_FixedTextMessage2 = sMsg[1];
    pCmd->TPT_FixedTextMessage3 = sMsg[2];

  }/*if*/
}
