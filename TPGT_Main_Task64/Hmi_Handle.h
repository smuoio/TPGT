/**
* \file Hmi_Handle.h
* \brief the file manages the application requirements of hmi.
* \author Muoio Salvatore
*/


#ifndef __HMI_HANDLE_H__
#define __HMI_HANDLE_H__

#include "TPGT_Main_Task64.h"

/**
* \fn void Hmi_Handle_input(const DS_HMI_CCUT *const pin)
* \brief main function of application data input from HMI.
* \param pin interface pointer to struct DS_HMI_CCUT
* \return null
*/
void Hmi_Handle_input(const DS_HMI_CCUT *const pin);

/**
* \fn void Hmi_Handle_output(TYPE_TPGT_MAIN_TASK64_IF *interface);
* \brief main function of application data output to HMI.
* \param interface pointer to struct TYPE_TPGT_MAIN_TASK64_IF
* \return null
*/
void Hmi_Handle_output(TYPE_TPGT_MAIN_TASK64_IF *interface);
/**
* \fn void Hmi_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
* \brief main function of HMI functions.
* \param interface pointer to struct TYPE_TPGT_MAIN_TASK64_IF
* \return null
*/
void Hmi_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface);
/**
* \fn void Hmi_Handle_init(void)
* \brief init  function of HMI functions.
* \return null
*/
void Hmi_Handle_init(void);
/**
* \fn BOOLEAN8 GetAtpDownAlrmShow(void)
* \brief the funvtion return the variable AtpDownAlrmShow
* \return boolean value
*/
BOOLEAN8 GetAtpDownAlrmShow(void);

#endif
