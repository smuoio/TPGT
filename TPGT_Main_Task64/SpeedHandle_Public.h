/**
* \file SpeedHandle_Public.h
* \brief the file manages the public structures and interfaces used in the speed component
* \author Muoio Salvatore
*/

#ifndef __SPEED_HANDLE_PUBLIC_H__
#define __SPEED_HANDLE_PUBLIC_H__

/**
* \def ACTUAL_SPEED_NOT_AVAILABLE
* \brief Value not available
*/
#define ACTUAL_SPEED_NOT_AVAILABLE 0xFFFF
/**
* \struct t_SpeedSelected
* \brief Local structure contains the local speed and validity to send to CCUO
*/
typedef struct
{
  INT16    Speed;       /*!< Speed Selected*/
  BOOLEAN8 Validity;    /*!< Speed Validity*/
  UINT8    SpeedStatus; /*!< MMI Speed data Consistency*/
}t_SpeedSelected;


/**
* \fn UINT8 GetSpeedStatus(void)
* \brief the function returns the speed status
*/
UINT8 GetSpeedStatus(void);
/**
* \fn t_SpeedSelected *GetSpeedSelected(void)
* \brief The interface function returns the address of structure t_SpeedSelected
* \return the address of the static variable SpeedSelected
*/
t_SpeedSelected *GetSpeedSelected(void);

#endif
