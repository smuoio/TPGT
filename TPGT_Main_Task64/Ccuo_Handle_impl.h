/**
* \file Ccuo_Handle_impl.h
* \brief the file manages the private functions of ccuo
* \author Muoio Salvatore
*/

#ifndef __CCUO_HANDLE_IMPL_H__
#define __CCUO_HANDLE_IMPL_H__

#include "CCUT_Util.h"
#include "Time_Util.h"
#include "usertypes.h"
#include "SpeedHandle_Public.h"

/**
* \fn e_result WheelDiam(CCUT_CCUO * const pCcut, const TSG_P1 * const pTsgP1)
* \brief function used to set the wheel diamenter to send to CCUO
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \param pTsgP1 pointer to input from MVB, struct TSG_P1
* \return OK or type of error
*/
e_result WheelDiam(CCUT_CCUO * const pCcut, const TSG_P1 * const pTsgP1);
/**
* \fn e_result SetSpeedSelected(CCUT_CCUO * const pCcut, t_SpeedSelected *aSpeedSel)
* \brief function used to set the speed selected to send to CCUO
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \param aSpeedSel pointer to local struct t_SpeedSelected,
*        used in the speed component
* \return OK or type of error
*/
e_result SetSpeedSelected(CCUT_CCUO * const pCcut, t_SpeedSelected *aSpeedSel);
/**
* \fn e_result ReportBrakeTest(CCUT_CCUO * const pCcut, const TSG_P0 * const pTsgP0)
* \brief function used to set the speed selected to send to CCUO
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \param pTsgP0 pointer to local struct TSG_P0,
*        used in the speed component
* \return OK or type of error
*/
e_result ReportBrakeTest(CCUT_CCUO * const pCcut, const TSG_P0 * const pTsgP0);
/**
* \fn void EnbaleMask2Vso(CCUT_CCUO * const pCcut, UINT16 *pMaskIdRqst)
* \brief The function set the maskid to HMI via CCUO
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \param pMmi pointer to struct MMI_AUX_OUT
*/
void EnbaleMask2Vso(CCUT_CCUO * const pCcut, UINT16 *pMaskIdRqst);
/**
* \fn void SetTime2EbiCab(VCE_AUX_OUT * const pVce, MMI_AUX_OUT * const pMmi,
                          const UINT8 aMmiMode)
* \brief The function set the speed to EbiCab via process data
* \param pVce pointer to struct VCE_AUX_OUT
* \param pMaskIdRqst pointer to maskid to set
* \param aMmiMode const value
*/
void SetTime2EbiCab(VCE_AUX_OUT * const pVce, MMI_AUX_OUT * const pMmi,
                    const UINT8 aMmiMode);
/**
* \fn e_result CesaTestFeedback(CCUT_CCUO * const pCcut)
* \brief function used to set the CESA feedbacks to CCUO
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \return OK or type of error
*/
e_result CesaTestFeedback(CCUT_CCUO * const pCcut);
/**
* \fn e_result UpdateDiagEvent(CCUT_CCUO * const pCcut)
* \brief function update the diagnostic information on the frame from CCUT -> CCUO
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \return OK or type of error
*/
e_result UpdateDiagEvent(CCUT_CCUO * const pCcut);
/**
* \fn t_Time *GetTimeMaskId(void)
* \brief function returns the timer struct used to manage the mask id
* \return the pointer addressed
*/
t_Time *GetTimeMaskId(void);

#endif
