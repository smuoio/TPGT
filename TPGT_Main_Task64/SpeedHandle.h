/**
* \file SpeedHandle.h
* \brief the file manages the application functions used for the speed
* \author Muoio Salvatore
*/

#ifndef __SPEED_HANDLE_H__
#define __SPEED_HANDLE_H__

#include "TPGT_Main_Task64.h"
/**
* \fn void Speed_init(void)
* \brief the function initializes the counter
*/
void Speed_init(void);
/**
* \fn void SpeedHandle(TYPE_TPGT_MAIN_TASK64_IF *interface)
* \brief the function manages all stuff regarding the speed
* \param interface pointer to the object TYPE_TPGT_MAIN_TASK64_IF
*/
void SpeedHandle(TYPE_TPGT_MAIN_TASK64_IF *interface);





#endif
