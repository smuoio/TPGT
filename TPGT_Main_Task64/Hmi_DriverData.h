/**
* \file Hmi_DriverData.h
* \brief the file manages the private functions of hmi
* \author Muoio Salvatore
*/

#ifndef __HMI_DRIVER_DATA_H__
#define __HMI_DRIVER_DATA_H__

#include "CCUT_Util.h"

/**
* \enum e_QDriverData
* \brief value of MMI Q Driver Data of EVC 14
*/
typedef enum
{
  Window_Driver_Id    = 0, /*!< Open Data Entry Window Driver Identity */
  Window_Train_Num_Id = 1, /*!< Open Data Entry Window Train running number */
  ProvideChange_Window_Diver_Id = 2, /*!< Provide button for Train Running Number Change within
                                         Data Entry Window Driver Identity (if Bit 1=1) */
}e_QDriverData;

/**
* \fn BOOLEAN8 Hmi_ChangeDriverId(CCUT_CCUO *const pOut, const TPT_Uint32Valid *pInp)
* \brief the function manages the driver id request
* \param pOut pointer to the structure CCUT_CCUO
* \pram pInp pointer to the struct of output to HMI
* \return TRUE if the pointer is addressed otherwise false
*/
BOOLEAN8 Hmi_ChangeDriverId(CCUT_CCUO *const pOut, const TPT_Uint32Valid *pInp);
/**
* \fn BOOLEAN8 Hmi_ChangeTrainNumber(CCUT_CCUO *const pOut, const TPT_Uint32Valid *pInp)
* \brief the function manages the driver id request
* \param pOut pointer to the structure CCUT_CCUO
* \pram pInp pointer to the struct of output to HMI
* \return TRUE if the pointer is addressed otherwise false
*/
BOOLEAN8 Hmi_ChangeTrainNumber(CCUT_CCUO *const pOut, const TPT_Uint32Valid *pInp);
/**
* \fn void Hmi_Handle_DriverData(CCUT_CCUO *const pOut, const DS_HMI_CCUT *pInp)
* \brief the function manages all requirements regarding the driver data
* \param pOut pointer to the structure CCUT_CCUO
* \pram pInp pointer to the struct of output to HMI
* \return TRUE if the pointer is addressed otherwise false
*/
void Hmi_Handle_DriverData(CCUT_CCUO *const pOut, const DS_HMI_CCUT *pInp);






#endif
