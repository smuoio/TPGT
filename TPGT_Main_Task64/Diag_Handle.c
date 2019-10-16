/**
* \file Diag_Handle.c
* \brief the file implements the diagnostic of CCUT
* \author Muoio Salvatore
*/

#include "Diag_Handle.h"
#include "Diag_Handle_impl.h"
#include "Diag_Handle_Public.h"
#include "Comm_Util.h"


/**
* \fn static e_result ResetEventList(t_EventList *pEventList)
* \brief static function used to reset all the failures
* \param *Event to the t_EventList struct
* \return TRUE of FALSE
*/
static e_result ResetEventList(t_EventList *pEventList)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((t_EventList*)pEventList);
  if(TPG_SUCCEEDED(lChk))
  {
    /*reset all fields*/
    memset(pEventList, 0, sizeof(t_EventList));
  }/*if*/
  return(lChk);
}

/**
* \fn static e_result ResetEventList(t_EventList *pEventList)
* \brief static function used to reset all the failures
* \param *Event to the t_EventList struct
* \return TRUE of FALSE
*/
static void Copy2Local(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  if(interface != 0)
  {
    /*copy*/
    GetDiagLegacy()->SDTDiagCCUO2CCUT = interface->iCCUT_CCUO_diag;
    GetDiagLegacy()->SDTDiagCCUT2CCUO = interface->oCCUT_CCUO_diag;
    GetDiagLegacy()->SDTDiagCCUT2HMI  = interface->CCUT_HMI_TRN_diag;
  }
}


void Diag_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  /*reset all diagnosti events*/
  if(interface != 0)
  {
    ResetEventList(GetEventList());
    Copy2Local(interface);
    /*CommEvents_Handle*/
    CommEvent_Handle();
    /*SpeedEvent_Handle*/
    SpeedEvent_Handle();
    CesaEvent_Handle(interface);
  }
}

void CesaEvent_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  BOOLEAN8 lFailure = FALSE;
  if(interface != 0)
  {
    lFailure |= interface->CesaErrTempOverheat;
    lFailure |= interface->CesaErrTempSensor;
    lFailure |= interface->CesaErrCcuoHwType;
    lFailure |= interface->CesaErrCcuoHwVersion;
    lFailure |= interface->CesaErrEeprom;
    lFailure |= interface->CesaErrHWClock;
    GetEventList()->F_CesaSupervisor = lFailure;
  }
}
