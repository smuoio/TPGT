/**
* \file Diag_Handle.h
* \brief the file implements the diagnostic of CCUT
* \author Muoio Salvatore
*/

#ifndef __DIAG_HANDLE_H__
#define __DIAG_HANDLE_H__

#include "CCUT_Util.h"
#include "TPGT_Main_Task64.h"


/**
* \fn void Diag_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface);
* \brief the function manages all diagnstic events
* \param interface pointer to sw component interface
*/
void Diag_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface);
/**
* \fn void CesaEvent_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
* \brief the function manages the cesa events
* \param interface pointer to interface sw component
*/
void CesaEvent_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface);

#endif
