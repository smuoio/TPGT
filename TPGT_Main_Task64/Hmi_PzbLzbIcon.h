/**
* \file Hmi_PzbLzbIcon.h
* \brief the file manages the private functions of Pzb/Lzb Icon
* \author Muoio Salvatore
*/

#ifndef __HMI_PZBLZB_ICON_H__
#define __HMI_PZBLZB_ICON_H__

#include "usertypes.h"
/**
* \enum e_PzbLzbFlashing
* \brief enunmeration of type flashing
*/
typedef enum
{
  FLASHING_INVALID,          /*!< Invalid flashing*/
  FLASHING_NONE,             /*!< Indicator No flashing*/
  FLASHING_SLOW,             /*!< Indicator Slow flashing*/
  FLASHING_FAST,             /*!< Indicator Fast flashing*/
  FLASHING_COUNTERPHASE_SLOW,/*!< Indicator Counter Slow flashing*/
  FLASHING_COUNTERPHASE_FAST,/*!< Indicator Counter Fast flashing*/
}e_PzbLzbFlashing;

/**
* \strcut t_PzbLzbIndicator
* \brief structure with fields used from the icons
*/
typedef struct
{
  TPT_ButtonIndicator TPT_StmIndicator1; /*!< Icon 1*/
  TPT_ButtonIndicator TPT_StmIndicator2; /*!< Icon 2*/
  TPT_ButtonIndicator TPT_StmIndicator3; /*!< Icon 3*/
  TPT_ButtonIndicator TPT_StmIndicator4; /*!< Icon 4*/
  TPT_ButtonIndicator TPT_StmIndicator5; /*!< Icon 5*/
  TPT_ButtonIndicator TPT_StmIndicator6; /*!< Icon 6*/
  TPT_ButtonIndicator TPT_StmIndicator7; /*!< Icon 7*/
}t_PzbLzbIndicator;

/**
* \fn void Hmi_PzbLzbIcon(DS_CCUT_HMI_TRN *const pCmd)
* \brief The function sets the icons starting from the STM 35,
*        after that these messages must send to Hmi
*        as paramenters of TPT_StmIndicator x,
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
*/
void Hmi_PzbLzbIcon(DS_CCUT_HMI_TRN *const pCmd);
/**
* \fn void Hmi_ResetPzbLzbIcon(void)
* \brief The function resets the local varable when the cab is not occupied
*/
void Hmi_ResetPzbLzbIcon(void);

#endif
