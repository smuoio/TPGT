/**
* \file SpeedHandle_impl.h
* \brief the file manages the private functions used in the speed component
* \author Muoio Salvatore
*/

#ifndef __SPEED_HANDLE_IMPL_H__
#define __SPEED_HANDLE_IMPL_H__

#include "usertypes.h"
#include "Time_Util.h"

/**
* \enum e_SrcSpeed
* \brief definition of values regarding the source of the speed
*/
typedef enum
{
    NO_SRC_SPEED     = 0x0U, /*!<Value NO_SRC_SPEED*/
    SRC_SPEED_EBICAB,        /*!<Value SRC_SPEED_EBICAB*/
    SRC_SPEED_DBC,           /*!<Value SRC_SPEED_DBC*/
}e_SrcSpeed;

/**
* \enum e_ErrorSpeed
* \brief definition of values regarding the error speed between the speeds of
*        Ebicab
*/
typedef enum
{
    NO_ERROR_SPEED     = 0x0U, /*!<Value NO_ERROR_SPEED*/
    ERROR_SPEED_EBICAB = 0x1U, /*!<Value ERROR_SPEED_EBICAB*/
}e_ErrorSpeed;

/**
* \fn static BOOLEAN8 isSpeedCrcValid(INT16 Speed, UINT8 Life, UINT8 const CrcSpeed)
* \brief The function detects if the CRC for the speed is valid
* \param Speed speed from EbiCab via MVB bus
* \param Life counter of data set
* \param CrcSpeed value of CRC from Ebicab used to compare with crc calculated
* \return TRUE if crc is the valid, false is not valid
*/
BOOLEAN8 isSpeedCrcValid(INT16 Speed, UINT8 Life, UINT8 const CrcSpeed);
/**
* \fn BOOLEAN8 isTSGSpeedValid(const TSG_P0 *const pTSG_P0, BOOLEAN8 *isTsgSpeedValid)
* \brief The function checks if TSG Speed is valid
* \param pTSG_P0 pointer to the local structure mapped from MVB Bus
* \param isTsgSpeedValid output, true if the tsg speed is valid, false if is not valid
* \return TRUE if the pointers are addressed, false are not addressed
*/
BOOLEAN8 isTSGSpeedValid(const TSG_P0 *const pTSG_P0, BOOLEAN8 *isTsgSpeedValid);
/**
* \fn BOOLEAN8 isMMISpeedValid(const TSG_ETCS_MMI_DYN *const pTSG_ETCS_MMI_DYN, BOOLEAN8 *isMmiSpeedValid)
* \brief The function checks if MMI_DYN Speed is valid
* \param pTSG_ETCS_MMI_DYN pointer to the local structure mapped from MVB Bus
* \param isMmiSpeedValid output, true if the mmi dyn speed is valid, false if is not valid
* \return TRUE if the pointers are addressed, false are not addressed
*/
BOOLEAN8 isMMISpeedValid(const TSG_ETCS_MMI_DYN *const pTSG_ETCS_MMI_DYN, BOOLEAN8 *isMmiSpeedValid);
/**
* \fn void SelectSpeed(BOOLEAN8 isTsgSpeedValid, BOOLEAN8 isMmiSpeedValid, INT16 aVReal, INT16 aMmiVTrn)
* \brief The function select the speed  to use
* \param isTsgSpeedValid validity of Tsg Speed
* \param isMmiSpeedValid validity of Mmis Speed
* \param aVReal tsg Speed value
* \param aMmiVTrn Mmi Speed value
* \return TRUE if the pointers are addressed, false are not addressed
*/
void SelectSpeed(BOOLEAN8 isTsgSpeedValid, BOOLEAN8 isMmiSpeedValid, INT16 aVReal,
                 INT16 aMmiVTrn);
/**
* \fn BOOLEAN8 CheckSpeedError(const TSG_P0 *const pTSG_P0,
                              const TSG_ETCS_MMI_DYN *const pTSG_ETCS_MMI_DYN)
* \brief The function checks if there are some errors to detect the right speed to use
* \param pTSG_P0 pointer to the local structure mapped from MVB Bus
* \param pTSG_ETCS_MMI_DYN pointer to the local structure mapped from MVB Bus
* \return TRUE if the pointers are addressed, false are not addressed
*/
BOOLEAN8 CheckSpeedError(const TSG_P0 *const pTSG_P0,
                         const TSG_ETCS_MMI_DYN *const pTSG_ETCS_MMI_DYN);
/**
* \fn t_Time *GetTimeSpeed(void)
* \brief the function returns the address to the class t_time
*/
t_Time *GetTimeSpeed(void);
/**
* \fn t_Time *GetTimeTSGSpeed(void)
* \brief the function returns the address to the class t_time
*/
t_Time *GetTimeTSGSpeed(void);
/**
* \fn t_Time *GetTimeMMISpeed(void)
* \brief the function returns the address to the class t_time
*/
t_Time *GetTimeMMISpeed(void);
/**
* \fn void SetSpeed2EbiCab(VCE_AUX_OUT * const pVce, MMI_AUX_OUT * const pMmi)
* \brief The function set the speed to EbiCab via process data
* \param pVce pointer to struct VCE_AUX_OUT
* \param pMmi pointer to struct MMI_AUX_OUT
*/
void SetSpeed2EbiCab(VCE_AUX_OUT * const pVce, MMI_AUX_OUT * const pMmi);

#endif
