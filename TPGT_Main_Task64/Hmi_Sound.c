/**
* \file Hmi_Sound.c
* \brief the file manages the private functions of Sound
*         (STM46)
* \author Muoio Salvatore
*/
#include "Generic_api.h"
#include "GPP_handle_Public.h"
#include "Etcs_Public.h"
#include "TractionBrake.h"
#include "Hmi_Sound.h"

/**
* \fn static BOOLEAN8 isSoundEBActive(BOOLEAN8 *isSoundEB)
* \brief The function detects if is active the emergency brake sound
* \param isSoundEB output pointer
* \return TRUE if the pointer is addressed, FALSE viceversa
*/
static BOOLEAN8 isSoundEBActive(BOOLEAN8 *isSoundEB)
{
  BOOLEAN8 lOut = FALSE;
  if(isSoundEB != 0)
  {
    if(GetStmModus() != STM_MODUS_UNKNOWN)
    {
        GetStm46ZwangbremsungOnOff(GetStm46MmiStmNIter(), isSoundEB);
    }/*if*/
    lOut = TRUE;
  }/*if pls*/
  return(lOut);
}

/**
* \fn static BOOLEAN8 isSoundZugBeeActive(BOOLEAN8 *isSoundZugBee)
* \brief The function detects if is active the Zugbeeinflussung sound
* \param isSoundZugBee output pointer
* \return TRUE if the pointer is addressed, FALSE viceversa
*/
static BOOLEAN8 isSoundZugBeeActive(BOOLEAN8 *isSoundZugBee)
{
  BOOLEAN8 lOut = FALSE;
  if(isSoundZugBee != 0){
    if(GetStmModus() != STM_MODUS_UNKNOWN){
      GetStm46ZugbeeinflussungOnOff(GetStm46MmiStmNIter(), isSoundZugBee);
    }/*if*/
    lOut = TRUE;
  }/*if pls*/
  return(lOut);
}

/**
* \fn static BOOLEAN8 isSoundSchnarreActive(UINT8 *SoundSchnarre)
* \brief The function detects if is active the Schnarre sound
* \param SoundSchnarre output pointer
* \return TRUE if the pointer is addressed, FALSE viceversa
*/
static BOOLEAN8 isSoundSchnarreActive(UINT8 *SoundSchnarre)
{
  BOOLEAN8 lOut = FALSE;
  if(SoundSchnarre != 0){
    if(GetStmModus() != STM_MODUS_UNKNOWN){
      GetStm46SchnarreOnOff(GetStm46MmiStmNIter(), SoundSchnarre);
    }/*if*/
    lOut = TRUE;
  }/*if pls*/
  return(lOut);
}



void Hmi_Sound(DS_CCUT_HMI_TRN *const pCmd)
{
  BOOLEAN8 lIsScreenDel            = FALSE;
  static BOOLEAN8 lIsSoundEB       = FALSE;
  static BOOLEAN8 lIsSoundZugBee   = FALSE;
  static UINT8    lIsSoundSchnarre = 0U;
  if(pCmd != 0)
  {
    isScreenDel(&lIsScreenDel);
    isSoundEBActive(&lIsSoundEB);
    isSoundZugBeeActive(&lIsSoundZugBee);
    isSoundSchnarreActive(&lIsSoundSchnarre);
    pCmd->TPT_StmSound.Zwangsbremsung   = 0;
    pCmd->TPT_StmSound.Zugbeeinflussung = 0;
    pCmd->TPT_StmSound.Schnarre         = 0;
    if((GetStmModus() == STM_MODUS_DATA_AVAILABLE) && (lIsScreenDel == FALSE))
    {
       if(lIsSoundEB == TRUE)
        pCmd->TPT_StmSound.Zwangsbremsung = 1;
      else
      {
        pCmd->TPT_StmSound.Schnarre = lIsSoundSchnarre;
        if((lIsSoundZugBee == TRUE) ||
           (GetEBZugbeeinflussung() == TRUE))
               pCmd->TPT_StmSound.Zugbeeinflussung = 1;
      }
    }
	else
	{
		lIsSoundEB       = FALSE;
		lIsSoundZugBee   = FALSE;
		lIsSoundSchnarre = 0U;		
	}
  }
}
