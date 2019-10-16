/**
* \file Cesa_Handle.c
* \brief the file manages the application functions for the CESA requirements
* \author Muoio Salvatore
*/
#include "Generic_api.h"
#include "Cesa_Handle.h"
#include "CESA_SV.h"

/**
*\var static UINT32 sCesaTimeout
**\brief static variable used to set the timeout
*/
static UINT32 sCesaTimeout = 0x0U;

void CesaInit(void)
{
  /* init cesa supervision */
  TYPE_SVINIT empty;
  CESASupervisionInit(&empty);

}

void Cesa_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  if(interface != 0)
  {
    isIntTestAct(&interface->CesaTestStart);
    /*copy timeout used to send to CCUO*/
    sCesaTimeout = interface->CesaRemainOpTime;
  }/*if*/
}

UINT32 GetCesaTimeOut(void)
{
  return(sCesaTimeout);
}
