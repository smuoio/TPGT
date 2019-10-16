/**
* \file Etcs_public.h
* \brief the file manages the all public functions of the ETCS/STM functions
* \author Muoio Salvatore
*/

#ifndef __ETCS_PUBLIC_H__
#define __ETCS_PUBLIC_H__

/**
* \enum e_StmModus
* \brief This value gives the actual status stm modus
*/
typedef enum
{
  STM_MODUS_UNKNOWN,        /*!<UNKNOWN         */
  STM_MODUS_DATA_AVAILABLE, /*!<DATA_AVAILABLE  */
  STM_MODUS_HOT_STANDBY,    /*!< HOT_STANDBY    */
}e_StmModus;

/**
* \fn e_StmModus GetStmModus(void)
* \brief the function returns the actual stm modus
* \return the value follows the enumeartion e_StmModus
*/
e_StmModus GetStmModus(void);
/**
* \fn UINT8 GetETCSLevel(const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn)
* \brief The function returns the ETCS LEVEL
* \param pEtcsMmiDyn interface to the object TSG_ETCS_MMI_DYN
* \return   default(15) and 0..14
*/
UINT8 GetETCSLevel(const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn);
/**
* \fn UINT8 GetETCSMode(const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn)
* \brief The function returns the ETCS MODE
* \param pEtcsMmiDyn pointer to the object TSG_ETCS_MMI_DYN
* \return   default(15) and 0..14
*/
UINT8 GetETCSMode(const TSG_ETCS_MMI_DYN * const pEtcsMmiDyn);

#endif
