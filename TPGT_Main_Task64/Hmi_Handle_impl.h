/**
* \file Hmi_Handle_impl.h
* \brief the file manages the private functions of hmi
* \author Muoio Salvatore
*/

#ifndef __HMI_HANDLE_IMPL_H__
#define __HMI_HANDLE_IMPL_H__

#include "CCUT_Util.h"

extern BOOLEAN8 AtpDownAlrmShow;

/**
* \fn e_result BlockData(const JRU_STATUS_IP * const aJruSts,
                         DS_CCUT_HMI_TRN * const aCcutHmiTrn)
* \brief main private function used to set "Data storage"
*        the visualization on Hmi
* \param aJruSts input pointer to struct JRU_STATUS
* \param aCcutHmiTrn output pointer to struct DS_CCUT_HMI_TRN
* \return the result of check pointer
*/
e_result BlockData(const JRU_STATUS_IP * const aJruSts,
                   DS_CCUT_HMI_TRN * const aCcutHmiTrn);
/**
* \fn e_result AtpDownAlarm(const DS_HMI_CCUT * const pin,
*                          DS_CCUT_HMI_TRN *const pout)
* \brief the function manages the atpdown sound
* \param pin const pointer to the DS_HMI_CCUT structure
* \param pout pointer to the Output structure
* \return the result of check pointer
*/
e_result AtpDownAlarm(const DS_HMI_CCUT * const pin, DS_CCUT_HMI_TRN *const pout);
/**
* \fn e_result isZDEButtonVisble(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out)
* \brief the function enable to show the button ZDE_V
* \param pout const pointer to the telegram to the HMI
* \param *Out logic output
* \return the result of check pointer
*/
e_result isZDEButtonVisble(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out);
/**
* \fn e_result isZDButtonVisible(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out)
* \brief the function enable to show the button ZDE_V
* \param pout const pointer to the telegram to the HMI
* \param *Out logic output
* \return the result of check pointer
*/
e_result isZDButtonVisible(DS_CCUT_HMI_TRN *const pout,BOOLEAN8 *Out);
/**
* \fn e_result EnableTestButton(DS_CCUT_HMI_TRN *const pout)
* \brief the function enable to show the button Pruefen_V(ZD_V)
* \param pout const pointer to the telegram to the HMI
* \return the result of check pointer
*/
e_result EnableTestButton(DS_CCUT_HMI_TRN *const pout);
/**
* \fn e_result isTestBWGButtonVisible(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out)
* \brief the function enable to show the button SBV_V
* \param pout const pointer to the telegram to the HMI
* \param *Out logic output
* \return the result of check pointer
*/
e_result isTestBWGButtonVisible(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out);
/**
* \fn e_result EnableTestPzbLzbButton(DS_CCUT_HMI_TRN *const pout)
* \brief the function enable to show the button ZugBesy_V
* \param pout const pointer to the telegram to the HMI
* \return the result of check pointer
*/
e_result EnableTestPzbLzbButton(DS_CCUT_HMI_TRN *const pout);
e_result EnableDelScreen(DS_CCUT_HMI_TRN *const pout);
/**
* \fn e_result EnableAllowedSpeed(const TSG_STM_MMI_DYN *const pTsgStm,
                                 DS_CCUT_HMI_TRN * const pCcutHmiTrn);
* \brief the function sends to HMI the the allowed speed
* \param pCcutHmiTrn const pointer to the telegram to the HMI
* \param pTsgStm const pointer to process data from ebicab
* \return the result of check pointer
*/
e_result EnableAllowedSpeed(const TSG_STM_MMI_DYN *const pTsgStm,
                            DS_CCUT_HMI_TRN * const pCcutHmiTrn);
/**
* \fn e_result EnableTargetSpeed(const TSG_STM_MMI_DYN *const pTsgStm,
                                 DS_CCUT_HMI_TRN * const pCcutHmiTrn);
* \brief the function sends to HMI the the target speed
* \param pCcutHmiTrn const pointer to the telegram to the HMI
* \param pTsgStm const pointer to process data from ebicab
* \return the result of check pointer
*/
e_result EnableTargetSpeed(const TSG_STM_MMI_DYN *const pTsgStm,
                           DS_CCUT_HMI_TRN * const pCcutHmiTrn);
/**
* \fn e_result EnableTargetDistance(const TSG_STM_MMI_DYN *const pTsgStm,
                                    DS_CCUT_HMI_TRN * const pCcutHmiTrn);
* \brief the function sends to HMI the the target distance
* \param pCcutHmiTrn const pointer to the telegram to the HMI
* \param pTsgStm const pointer to process data from ebicab
* \return the result of check pointer
*/
e_result EnableTargetDistance(const TSG_STM_MMI_DYN *const pTsgStm,
                           DS_CCUT_HMI_TRN * const pCcutHmiTrn);
/**
* \fn e_result Speed2Hmi(DS_CCUT_HMI_TRN * const pCcutHmiTrn)
* \brief the function sends to HMI the speed and the validity
* \param pCcutHmiTrn const pointer to the telegram to the HMI
* \return the result of check pointer
*/
e_result Speed2Hmi(DS_CCUT_HMI_TRN * const pCcutHmiTrn);
/**
* \fn e_result EnableTestPzbLzbButton(TPT_VersionInfo *const pOut,
                                     const TSG_P3 * const pTsgP3)
* \brief the function enable to show the versions number
* \param pout const pointer to the telegram to the HMI
* \param pTsgP3 const pointer to the input from EbiCab
* \return the result of check pointer
*/
e_result EnableSwVersion(TPT_IVersionInfo *const pOut,
                         const TSG_P3 * const pTsgP3);
#endif
