/**
* \file Cesa_Handle.h
* \brief the file manages the application functions for the CESA requirements
* \author Muoio Salvatore
*/

#ifndef __CESA_HANDLE_H__
#define __CESA_HANDLE_H__

#include "TPGT_Main_Task64.h"

/**
* \fn void Cesa_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
* \brief main function of application CESA.
* \param interface pointer to interface TYPE_TPGT_MAIN_TASK64_IF component
* \return null
*/
void Cesa_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface);
/**
* \fn UINT32 GetCesaTimeOut(void);
* \brief the function returns the value of timeout
* \return null
*/
UINT32 GetCesaTimeOut(void);
/**
* \fn void CesaInit(void)
* \brief the function initialize the cesa structure
* \return null
*/
void CesaInit(void);
#endif
