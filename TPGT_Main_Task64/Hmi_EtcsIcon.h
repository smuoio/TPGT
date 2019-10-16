/**
* \file Hmi_EtcsIcon.h
* \brief the file manages the private functions of Etcs Icon
* \author Muoio Salvatore
*/

#ifndef __HMI_ETCS_ICON_H__
#define __HMI_ETCS_ICON_H__

#include "usertypes.h"
/**
* \enum e_EtcsLevel
* \brief field type of Etcs Level
*/
typedef enum
{
  LEVEL_0, /*!< LEVEL 0 */
  LEVEL_1, /*!< LEVEL 1 */
}e_EtcsLevel;
/**
* \enum e_MmiBrakeSts
* \brief field type of emergency brake status from Ebicab via PD-MVB
*/
typedef enum
{
  NO_EB_SB        = 0,  /*!< IDLE */
  SB_RELEASE_IMP  = 2,  /*!< SB release impossible */
  SB_RELEASE_POSS,      /*!< SB release possible */
  EB_RELEASE_IMP  = 8,  /*!< EB release impossible */
  EB_RELEASE_POSS = 12, /*!< EB release possible */
}e_MmiBrakeSts;

/**
* \enum e_BrakeStsVisible
* \brief field type of visible or not of BrakeStatus sambol
*/
typedef enum
{
  BRAKESTATUS_NOT_VISIBLE, /*!< Not visible */
  BRAKESTATUS_VISIBLE,     /*!< visible */
}e_BrakeStsVisible;
/**
* \enum e_EtcsSymbolId
* \brief field type of Etcs Symbol id
*/
typedef enum
{
  SYMBOLID_NONE,    /*!< Symbol id none */
  SYMBOLID_LEVEL_0, /*!< Symbol id level 0 */
  SYMBOLID_LEVEL_1, /*!< Symbol id level 1 */
  SYMBOLID_LEVEL_STM_LINEAR_PUNCTUAL, /*!< Symbol id level stm linear and punctual */
  SYMBOLID_LEVEL_STM_PUNCTUAL, /*!< Symbol id level stm punctual */
  SYMBOLID_LEVEL_STM_LINEARLY, /*!< Symbol id level stm linear */
  SYMBOLID_LEVEL_2, /*!< Symbol id level 2 */
}e_EtcsSymbolId;

/**
* \enum e_MmiMode
* \brief field type of MMI mode
*/
typedef enum
{
  MMI_M_MODE_FULL_SUPERVISION,  /*!< full supervision */
  MMI_M_MODE_ON_SIGHT,          /*!< on sigth */
  MMI_M_MODE_STAFF_RESPONSABLE, /*!< staff responsable */
  MMI_M_MODE_SHUNTING,          /*!< shunting */
  MMI_M_MODE_UNFITTED,          /*!< unfitted */
  MMI_M_MODE_SLEEPING,          /*!< sleeping */
  MMI_M_MODE_STANDBY,           /*!< standby */
  MMI_M_MODE_TRIP,              /*!< trip */
  MMI_M_MODE_POST_TRIP,         /*!< post trip */
  MMI_M_MODE_SYSTEM_FAILURE,    /*!< system failure */
  MMI_M_MODE_ISOLATION,         /*!< isolation */
  MMI_M_MODE_NON_LEADING,       /*!< non leading */
  MMI_M_MODE_STM_EUROPEAN,      /*!< stm european */
  MMI_M_MODE_STM_NATIONAL,      /*!< stm national */
  MMI_M_MODE_REVERSING,         /*!< reversing */
  MMI_M_MODE_UNKNOWN            /*!< unknown */
}e_MmiMode;

/**
* \fn void Hmi_EtcsIconLevel(DS_CCUT_HMI_TRN *const pCmd,const TSG_P2 *const pTsgP2,
*                            const TSG_ETCS_MMI_DYN *const pEtcs)
* \brief The function smanages the icon level
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
* \param pTsgP2 input pointer to the struct TSG_P2
* \param pEtcs output pointer to the struct TSG_ETCS_MMI_DYN
*/
void Hmi_EtcsIconLevel(DS_CCUT_HMI_TRN *const pCmd, const TSG_P2 *const pTsgP2,
                       const TSG_ETCS_MMI_DYN *const pEtcs);
/**
* \fn void Hmi_EtcsIconMode(DS_CCUT_HMI_TRN *const pCmd,const TSG_ETCS_MMI_DYN *const pEtcs)
* \brief The function manages the icone of Etcs
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
* \param pEtcs input pointer to the struct TSG_ETCS_MMI_DYN
*/
void Hmi_EtcsIconMode(DS_CCUT_HMI_TRN *const pCmd,
                      const TSG_ETCS_MMI_DYN *const pEtcs);

/**
* \fn void Hmi_EtcsIcon(DS_CCUT_HMI_TRN *const pCmd,const TSG_P2 *const pTsgP2,
*                            const TSG_ETCS_MMI_DYN *const pEtcs)
* \brief The function smanages the icon level
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
* \param pTsgP2 input pointer to the struct TSG_P2
* \param pEtcs output pointer to the struct TSG_ETCS_MMI_DYN
*/
void Hmi_EtcsIcon(DS_CCUT_HMI_TRN *const pCmd, const TSG_P2 *const pTsgP2,
                  const TSG_ETCS_MMI_DYN *const pEtcs);
#endif
