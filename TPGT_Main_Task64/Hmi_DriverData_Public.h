/**
* \file Hmi_DriverData_Public.h
* \brief the file manages the public functions of hmi for the driver data
* \author Muoio Salvatore
*/


#ifndef __HMI_DRIVER_DATA_PUBLIC_H__
#define __HMI_DRIVER_DATA_PUBLIC_H__

/**
* \fn UINT32 GetMmiNidDriver(void)
* \brief the function returns the static value Mmi Nid Driver
* \return value
*/
UINT32 GetMmiNidDriver(void);
/**
* \fn UINT32 GetMmiNidOperation(void)
* \brief the function returns the static value Mmi Nid Operation
* \return value
*/
UINT32 GetMmiNidOperation(void);
/**
* \fn UINT8 GetMmiQDriverDataConfirmed(void)
* \brief the function returns the static value Mmi Q Driver Data
* \return value
*/
UINT8 GetMmiQDriverDataConfirmed(void);
#endif
