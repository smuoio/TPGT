/**
* \file ETCS.h
* \brief the file manages the all requirements regarding the ETCS/STM functions
* \author Muoio Salvatore
*/

#ifndef __ETCS_H__
#define __ETCS_H__

#include "TPGT_Main_Task64.h"
#include "Etcs_public.h"


/**
* \fn BOOLEAN8 ComputeStm_Modus(e_StmModus *aStmModus,
*                         const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn,
*                         const TSG_STM_MMI_DYN  * const pStmMmiDyn);
* \brief the function computes the stm modus
* \param aStmModus, global variable used in the interface function
* \param pEtcsMmiDyn, pointer to the object  TSG_ETCS_MMI_DYN
* \param pStmMmiDyn,  pointer to the object  TSG_STM_MMI_DYN
* \return TRUE if the pointer are addressed, FALSE viceversa
*/
BOOLEAN8 ComputeStm_Modus(e_StmModus *aStmModus,
                          const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn,
                          const TSG_STM_MMI_DYN  * const pStmMmiDyn);
/**
* \fn void ETCS_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
* \brief main function of application data regarding the the STM Modus
* \param interface pointer to the object TYPE_TPGT_MAIN_TASK64_IF
*/
void ETCS_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface);
/**
* \fn void ETC_Isolation(MMI_AUX_OUT * const pMMiAuxOut)
* \brief
* \param pMMiAuxOut pointer to the object MMI_AUX_OUT
*/
void ETC_Isolation(MMI_AUX_OUT * const pMMiAuxOut);


#endif
