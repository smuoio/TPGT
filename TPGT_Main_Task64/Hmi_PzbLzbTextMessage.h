/**
* \file Hmi_PzbLzbTextMessage.h
* \brief the file manages the private functions of Pzb/LZB
*        text message to hmi(STM38/STM39)
* \author Muoio Salvatore
*/


#ifndef __HMI_PZBLZB_TEXT_MESSAGE_H__
#define __HMI_PZBLZB_TEXT_MESSAGE_H__


#include "usertypes.h"
/**
* \enum e_Flashing
* \brief field type of flashing
*/
typedef enum
{
  NO_FALSHING = 1,      /*!<No flashing */
  SLOW_FLASHING,        /*!<Slow flashing */
  FAST_FLASHING,        /*!<Fast flashing */
  COUNTER_SLOW_FLASHING,/*!<Counter phase flashing slow */
  COUNTER_FAST_FLASHING,/*!<Counter phase flashing fast*/
}e_Flashing;

/**
* \fn void Hmi_StorePzbLzbMessage(DS_CCUT_HMI_TRN *const pCmd)
* \brief The function store the STM 38 message, These messages must send to Hmi
*        as paramenters of TPT_FixedTextMessage1, TPT_FixedTextMessage2 and
*        TPT_FixedTextMessage3
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
*/
void Hmi_StorePzbLzbMessage(DS_CCUT_HMI_TRN *const pCmd);
/**
* \fn void Hmi_CancelPzbLzbMessage(DS_CCUT_HMI_TRN *const pCmd)
* \brief The function cancel the message startin from the STM 39,
*        after that these messages must send to Hmi
*        as paramenters of TPT_FixedTextMessage1, TPT_FixedTextMessage2 and
*        TPT_FixedTextMessage3
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
*/
void Hmi_CancelPzbLzbMessage(DS_CCUT_HMI_TRN * const pCmd);
/**
* \fn void Hmi_CancelPzbLzbMessage(DS_CCUT_HMI_TRN *const pCmd)
* \brief The function cancel the message startin from the STM 39,
*        after that these messages must send to Hmi
*        as paramenters of TPT_FixedTextMessage1, TPT_FixedTextMessage2 and
*        TPT_FixedTextMessage3
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
*/
void Hmi_PzbLzbMessage(DS_CCUT_HMI_TRN * const pCmd);

#endif
