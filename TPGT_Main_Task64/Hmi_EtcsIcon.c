/**
* \file Hmi_EtcsIcon.c
* \brief the file manages the private functions of Etcs Icon
* \author Muoio Salvatore
*/

#include "Generic_api.h"
#include "Etcs_public.h"
#include "Hmi_EtcsIcon.h"

/**
* \fn static UINT8 LookUpTableSymbolId(UINT8 aLevel)
* \brief The function is a look up table and return the level, starting
*        from the MMI_M_Level from process data
* \param aLevel MMI_M_Level, value read from the process data
* \return value of look up table
*/
static UINT8 LookUpTableSymbolId(UINT8 aLevel)
{
  const UINT8 Tab[] = {SYMBOLID_LEVEL_0,
                       SYMBOLID_LEVEL_STM_LINEAR_PUNCTUAL,
                       SYMBOLID_LEVEL_1,
                       SYMBOLID_LEVEL_2,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE,
                       SYMBOLID_NONE};
  return(Tab[aLevel]);
}


void Hmi_EtcsIconLevel(DS_CCUT_HMI_TRN *const pCmd,
                       const TSG_P2 *const pTsgP2,
                       const TSG_ETCS_MMI_DYN *const pEtcs)
{
  if((pCmd != 0) && (pTsgP2 != 0) && (pEtcs != 0))
  {
    BOOLEAN8 lIsScreenDel = FALSE;
    isScreenDel(&lIsScreenDel);
    /*check stm level condition */
    /*STM Level*/
    pCmd->TPT_EtcsLevel.SymbolId = SYMBOLID_NONE;
    if(lIsScreenDel == FALSE){
      /*set symbolid*/
      if((GetStmModus() == STM_MODUS_DATA_AVAILABLE) && (pTsgP2->LZBBetriebsart >= 8))
        pCmd->TPT_EtcsLevel.SymbolId = SYMBOLID_LEVEL_STM_LINEARLY;
      else if((GetStmModus() == STM_MODUS_DATA_AVAILABLE) && (pTsgP2->LZBBetriebsart < 8))
        pCmd->TPT_EtcsLevel.SymbolId = SYMBOLID_LEVEL_STM_PUNCTUAL;
      else if((GetETCSLevel(pEtcs) == LEVEL_1) && (GetStmModus() != STM_MODUS_DATA_AVAILABLE))
        pCmd->TPT_EtcsLevel.SymbolId = SYMBOLID_LEVEL_STM_LINEAR_PUNCTUAL;
      else
      {
        pCmd->TPT_EtcsLevel.SymbolId = LookUpTableSymbolId(GetETCSLevel(pEtcs));
      }
    }/*if cehck screen*/
  }/*if pls*/
}
/**
* \fn static void set_emerbrake_sts(UINT8 aMmiBrkSts, TPT_ButtonIndicator *pBrkSts)
* \brief The function sets the emergency icon on the HMI
* \param aMmiBrkSts MMI_Brake_Status, read from the process data
* \param pBrkSts pointer to the output structure
*/
static void set_emerbrake_sts(UINT8 aMmiBrkSts, TPT_ButtonIndicator *pBrkSts)
{
  if (pBrkSts != 0) {
    UINT8 lPossible   = (aMmiBrkSts & EB_RELEASE_POSS);
    UINT8 lImpossible = (aMmiBrkSts & EB_RELEASE_IMP);
    /* code */
    pBrkSts->SymbolId = BRAKESTATUS_NOT_VISIBLE;
    if ((lImpossible == EB_RELEASE_IMP) | (lPossible == EB_RELEASE_POSS)) {
      /* code */
      pBrkSts->SymbolId = BRAKESTATUS_VISIBLE;
    }/*if*/
  }/*if pls*/
}
/**
* \fn static UINT8 LookUpTableMMIMode(UINT8 aMmiMode)
* \brief The function is a look up table and return the MMIMode
* \param aMmiMode MMI_M_Mode, value read from the process data
* \return value of look up table
*/
static UINT8 LookUpTableMMIMode(UINT8 aMmiMode)
{
  const UINT8 MMI_M_MODE_NONE = 0x0U;
  const UINT8 Tab[] = {MMI_M_MODE_NONE, MMI_M_MODE_NONE, MMI_M_MODE_NONE,
                       MMI_M_MODE_NONE, MMI_M_MODE_NONE, MMI_M_MODE_NONE,
                       MMI_M_MODE_STANDBY,
                       MMI_M_MODE_NONE, MMI_M_MODE_NONE,
                       MMI_M_MODE_SYSTEM_FAILURE,
                       MMI_M_MODE_NONE, MMI_M_MODE_NONE, MMI_M_MODE_NONE,
                       MMI_M_MODE_STM_NATIONAL,
                       MMI_M_MODE_NONE, MMI_M_MODE_NONE};
  return(Tab[aMmiMode]);
}


void Hmi_EtcsIconMode(DS_CCUT_HMI_TRN *const pCmd,
                      const TSG_ETCS_MMI_DYN *const pEtcs)
{
  const UINT8 None = 0;
  BOOLEAN8 lIsEtcsIsolated = FALSE;
  BOOLEAN8 lIsScreenDel    = FALSE;
  BOOLEAN8 lIsAlive = FALSE;
  if((pCmd != 0) && (pEtcs != 0)){
    /*check the status of Etcs*/
    isEtcsIsolated(&lIsEtcsIsolated);
    isScreenDel(&lIsScreenDel);
    isEbiCabAlive(&lIsAlive);
    if((lIsEtcsIsolated == FALSE) && (lIsScreenDel == FALSE)){
      /**/
        pCmd->TPT_EtcsMode.SymbolId = LookUpTableMMIMode(pEtcs->MMI_M_Mode);
        set_emerbrake_sts(pEtcs->MMI_Brake_Status, &pCmd->TPT_BrakeStatus);
      }
      else if((lIsEtcsIsolated == TRUE) && (lIsScreenDel == TRUE))
        pCmd->TPT_EtcsMode.SymbolId = MMI_M_MODE_ISOLATION;
	else
		pCmd->TPT_EtcsMode.SymbolId = None;
  }/*if pls*/
}

void Hmi_EtcsIcon(DS_CCUT_HMI_TRN *const pCmd, const TSG_P2 *const pTsgP2,
                  const TSG_ETCS_MMI_DYN *const pEtcs)
{
  if ((pCmd != 0) && (pTsgP2 != 0) && (pEtcs != 0)) {
    /* code */
    Hmi_EtcsIconLevel(pCmd, pTsgP2, pEtcs);
    Hmi_EtcsIconMode(pCmd, pEtcs);
  }
}
