/**
* \file Hmi_Sound.h
* \brief the file manages the private functions of Sound
*         (STM46)
* \author Muoio Salvatore
*/

#ifndef __HMI_SOUND_H__
#define __HMI_SOUND_H__

#include "usertypes.h"
/**
* \fn void Hmi_Sound(DS_CCUT_HMI_TRN *const pCmd)
* \brief The function manages the sound starting from the STM 46,
*        after that these messages must send to Hmi
*        as paramenters of TPT_StmSound
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
*/
void Hmi_Sound(DS_CCUT_HMI_TRN *const pCmd);



#endif
