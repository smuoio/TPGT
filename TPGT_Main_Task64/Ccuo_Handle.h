/**
* \file Ccuo_Handle.h
* \brief the file manages the application functions to get and set data from to CCUO.
* \author Muoio Salvatore
*/

#ifndef __CCUO_HANDLE_H__
#define __CCUO_HANDLE_H__

#include "TPGT_Main_Task64.h"
/**
* \fn void Ccuo_Handle_input(const CCUO_CCUT * const pin)
* \brief main function of application data from CCUO.
* \param pin pointer to input from TYPE_TPGT_MAIN_TASK64_IF
* \return null
*/
void Ccuo_Handle_input(const CCUO_CCUT * const pin);
/**
* \fn void Ccuo_Handle_output(TYPE_TPGT_MAIN_TASK64_IF *pout)
* \brief main function of application data to CCUO.
* \param pin pointer to input from TYPE_TPGT_Main_Task64_IF
* \return null
*/
void Ccuo_Handle_output(TYPE_TPGT_MAIN_TASK64_IF *pout);
/**
* \fn void Ccuo_Handle_Init(void)
* \brief init function.
* \return null
*/
void Ccuo_Handle_Init(void);

#endif
