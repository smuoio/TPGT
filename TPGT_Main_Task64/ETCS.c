/**
* \file ETCS.c
* \brief the file manages the all requirements regarding the ETCS/STM functions
* \author Muoio Salvatore
*/

#include "CCUT_Util.h"
#include "Task64_Def.h"
#include "Gpp_Handle_Public.h"
#include "Generic_api.h"
#include "ETCS.h"


/**
* \var static e_StmModus sStsModus
* \brief variable for the STM MODUS
*/
static e_StmModus sStsModus = STM_MODUS_UNKNOWN;

void ETCS_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  if(interface != 0)
  {
    /* function computes the STM Modus*/
    ComputeStm_Modus(&sStsModus,
                     &(interface->TSG_ETCS_MMI_DYN),
                     &(interface->TSG_STM_MMI_DYN));
  }/*if*/
}



BOOLEAN8 ComputeStm_Modus(e_StmModus *aStmModus,
                            const TSG_ETCS_MMI_DYN * const pETCS_MMI_DYN,
                            const TSG_STM_MMI_DYN  * const pSTM_MMI_DYN)
{
  e_result lOut = TPG_SUCCESS;
	lOut |= checkOutputArg((TSG_ETCS_MMI_DYN*)pETCS_MMI_DYN);
	lOut |= checkOutputArg((TSG_STM_MMI_DYN*)pSTM_MMI_DYN);
  lOut |= checkOutputArg((e_StmModus*)aStmModus);
	if(TPG_SUCCEEDED(lOut))
  {
    /*check condition to set the output aStmModus*/
    /*read the data from STM15 in the shared memory using the interface function*/
    if(((pETCS_MMI_DYN->MMI_M_Mode == NL)          ||
       (pETCS_MMI_DYN->MMI_M_Mode == SN))          &&
       (pETCS_MMI_DYN->MMI_NID_STM_DA == PZB_LZB)  &&
       (pSTM_MMI_DYN->NID_STM == PZB_LZB)          &&
       ((pETCS_MMI_DYN->MMI_M_Info&1) == 1)        &&
       (GetStmNidStateReport() == DATA_AVAILABLE))
        *aStmModus = STM_MODUS_DATA_AVAILABLE;
    else if((GetStmNidStateReport() == HOT_STANDBY) &&
            ((pETCS_MMI_DYN->MMI_M_Info&2) == 2)    &&
            (pETCS_MMI_DYN->MMI_NID_STM_HS == PZB_LZB))
        *aStmModus = STM_MODUS_HOT_STANDBY;
    else
      *aStmModus = STM_MODUS_UNKNOWN;
  }/*if*/
  return(lOut);
}

UINT8 GetETCSMode(const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn)
{
  BOOLEAN8 lIsIsolated = FALSE;
  UINT8 lRes = 0xF;/*default value*/
  /*check if the ebicab is not isolated*/
  isEtcsIsolated(&lIsIsolated);
  if(lIsIsolated == FALSE)
    lRes = pEtcsMmiDyn->MMI_M_Mode;
  return(lRes);
}

UINT8 GetETCSLevel(const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn)
{
  /* init default value*/
  BOOLEAN8 lIsIsolated    = FALSE;
  BOOLEAN8 lIsEbiCabAlive = FALSE;
  UINT8 lRes = 0xF;
  /*check if the ebicab is not isolated*/
  if(pEtcsMmiDyn != 0){
    isEtcsIsolated(&lIsIsolated);
    isEbiCabAlive(&lIsEbiCabAlive);
    if((lIsIsolated == FALSE) && (lIsEbiCabAlive == TRUE))
      lRes = pEtcsMmiDyn->MMI_M_Level;
    }
  return(lRes);
}

void ETC_Isolation(MMI_AUX_OUT * const pMMiAuxOut)
{
  BOOLEAN8 lIsIsolated = FALSE;
  isEtcsIsolated(&lIsIsolated);
}

e_StmModus GetStmModus(void)
{
  return(sStsModus);
}
