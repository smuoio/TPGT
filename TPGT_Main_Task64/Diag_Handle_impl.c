/**
* \file Diag_Handle_impl.c
* \brief the file implements the private diagnostic functions
* \author Muoio Salvatore
*/
#include "Comm_Util.h"
#include "Diag_Handle_impl.h"
#include "Diag_Handle_Public.h"
#include "SpeedHandle_Public.h"

/**
*\var static t_EventList EventList
**\brief static variable to the event list object
*/
static t_EventList EventList;

/**
*\var static t_DiagLegacy DiagLegacy
**\brief static variable to the event list object
*/
static t_DiagLegacy DiagLegacy;


t_EventList *GetEventList(void)
{
  return(&EventList);
}

t_DiagLegacy *GetDiagLegacy(void)
{
  return(&DiagLegacy);
}


/**
* \fn static void SetCommEvents(UINT8 ds, BOOLEAN8 *Event)
* \brief static function used to set the failure of communication(MVB & ETH)
* \param ds is the number if of the dataset
* \param *Event boolean pointer to the variable to set
* \return null
*/
static void SetCommEvents(UINT8 ds, BOOLEAN8 *Event)
{
  if(Event != 0)
  {
    e_StsComm lStsConn = CONNECTED;
    lStsConn |= CheckConn(GetBusLife(ds));
    if(lStsConn != CONNECTED)
      *Event = TRUE;
  }/*if*/
}

/**
* \fn static void SetSDTEvents(INT32 adiag, BOOLEAN8 *Event)
* \brief static function used to set the failure of SDT communication
* \param adiag is the diag from SDT layer
* \param *Event boolean pointer to the variable to set
* \return null
*/
static void SetSDTEvents(INT32 adiag, BOOLEAN8 *Event)
{
  if(Event != 0)
  {
    if(adiag != DIAG_OK)
      *Event = TRUE;
  }/*if*/
}


void CommEvent_Handle(void)
{
  /*SetCommEvents*/
  /*MVB diag*/
  SetCommEvents(P0,   &GetEventList()->CommList.CommMVB.F_TSG_P0);
  SetCommEvents(P1,   &GetEventList()->CommList.CommMVB.F_TSG_P1);
  SetCommEvents(P2,   &GetEventList()->CommList.CommMVB.F_TSG_P2);
  SetCommEvents(P3,   &GetEventList()->CommList.CommMVB.F_TSG_P3);
  SetCommEvents(ETCS, &GetEventList()->CommList.CommMVB.F_TSG_ETCS);
  SetCommEvents(STM,  &GetEventList()->CommList.CommMVB.F_TSG_STM);
  SetCommEvents(JRU,  &GetEventList()->CommList.CommMVB.F_JRU);
  /*ETH diag*/
  SetCommEvents(CCUO, &GetEventList()->CommList.CommEth.F_Ccuo2Ccut);
  SetCommEvents(HMI,  &GetEventList()->CommList.CommEth.F_HMI1);
  SetCommEvents(HMI2, &GetEventList()->CommList.CommEth.F_HMI2);
  /*SDT Diag*/
  SetSDTEvents(GetDiagLegacy()->SDTDiagCCUO2CCUT,
               &GetEventList()->CommList.CommEth.F_SDTCCCUO);
  SetSDTEvents(GetDiagLegacy()->SDTDiagCCUT2CCUO,
               &GetEventList()->CommList.CommEth.F_SDTCCCUT);
  SetSDTEvents(GetDiagLegacy()->SDTDiagCCUT2HMI,
               &GetEventList()->CommList.CommEth.F_SDTHMI);
}

void SpeedEvent_Handle(void)
{
  GetEventList()->F_Speed = GetSpeedSelected()->SpeedStatus;
}
