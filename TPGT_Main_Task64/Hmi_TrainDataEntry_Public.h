/**
* \file Hmi_TrainDataEntry_Public.h
* \brief the file manages the public functions of hmi for the train data entry
* \author Muoio Salvatore
*/


#ifndef __HMI_TRAIN_DATA_ENTRY_PUBLIC_H__
#define __HMI_TRAIN_DATA_ENTRY_PUBLIC_H__

/**
* \fn UINT8 GetBrakeTypeAscii(void)
* \brief the function returns the value of BrakeType_Ascii
* \return null
*/
UINT8 GetBrakeTypeAscii(void);
/**
* \fn UINT8 *GetBrakePercentageAscii(void)
* \brief the function returns the address of BrakePercentage_Ascii
* \return null
*/
UINT8 *GetBrakePercentageAscii(void);
/**
* \fn UINT8 *GetTrainLengthAscii(void)
* \brief the function returns the address of TrainLength_Ascii
* \return null
*/
UINT8 *GetTrainLengthAscii(void);
/**
* \fn UINT8 *GetVmaxAscii(void)
* \brief the function returns the address of Vmax_Ascii
* \return null
*/
UINT8 *GetVmaxAscii(void);
/**
* \fn UINT8 GetLastEvc25Zda(void)
* \brief the function returns the last value of TrainDataText,
*  used in the packet EVC 123
* \return null
*/
UINT8 GetLastEvc25Zda(void);



#endif
