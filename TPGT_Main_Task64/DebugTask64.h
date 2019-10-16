/**
* \file DebugTask64.h
* \brief the file implements the functions used to debug/print the variables
*        used in the task 64
* \author Muoio Salvatore
*/


#ifndef __DEBUG_TASK_64_H__
#define __DEBUG_TASK_64_H__

#include "TPGT_Main_Task64.h"
#include "CCUT_Util.h"
/**
* \fn e_result DebugTask64(const TYPE_TPGT_MAIN_TASK64_IF * const interface)
* \brief wrapper function for debug the evc and stm message
* \param interface to the structure of task 64ms
* \return OK or type of error
*/
e_result DebugTask64(const TYPE_TPGT_MAIN_TASK64_IF * const interface);

#endif
