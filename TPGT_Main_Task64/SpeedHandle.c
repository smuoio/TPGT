/**
* \file SpeedHandle.c
* \brief the file manages the application functions used for the speed
* \author Muoio Salvatore
*/

#include "CCUT_Util.h"
#include "Generic_api.h"
#include "SpeedHandle.h"
#include "SpeedHandle_impl.h"
#include "Ccuo_Handle_Public.h"

/**
* \fn static void CurrentSpeed(TYPE_TPGT_MAIN_TASK64_IF *interface)
* \brief the function manages the current speed from Ebicab
* \param interface pointer to the object TYPE_TPGT_MAIN_TASK64_IF
*/
static void CurrentSpeed(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  e_result lChk = TPG_SUCCESS;
  static BOOLEAN8 isTsgSpeedValid = FALSE;
  static BOOLEAN8 isMmiSpeedValid = FALSE;
  lChk |= checkOutputArg((TYPE_TPGT_MAIN_TASK64_IF*)interface);
  if(TPG_SUCCEEDED(lChk))
  {
    /*Check TSG Speed valid*/
    isTSGSpeedValid(&(interface->TSG_P0), &isTsgSpeedValid);
    /*Check MMI DYN Speed valid*/
    isMMISpeedValid(&(interface->TSG_ETCS_MMI_DYN), &isMmiSpeedValid);
    /*SelectSpeed*/
    SelectSpeed(isTsgSpeedValid, isMmiSpeedValid, interface->TSG_P0.Vreal,
                interface->TSG_ETCS_MMI_DYN.MMI_V_Train);
    /*TODO*/
  }/*if*/
}


void SpeedHandle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
    e_result lChk = TPG_SUCCESS;
    lChk |= checkOutputArg((TYPE_TPGT_MAIN_TASK64_IF*)interface);
    if(TPG_SUCCEEDED(lChk))
    {
      BOOLEAN8 lIsCabActive = FALSE;
      isCabActive(&lIsCabActive);
	  /*if(GetCcuoCabActive() == CAB_OCCUPIED)*/
      {
        /* CurrentSpeed*/
        CurrentSpeed(interface);
        CheckSpeedError(&(interface->TSG_P0), &(interface->TSG_ETCS_MMI_DYN));
        SetSpeed2EbiCab(&interface->Vce_Aux_Out, &interface->Mmi_Aux_Out);
      }
    }/*if*/
}

void Speed_init(void)
{
  SetTime(GetTimeSpeed(),    (T5S/TASK64));
  SetTime(GetTimeTSGSpeed(), (T1S/TASK64));
  SetTime(GetTimeMMISpeed(), (T1S/TASK64));
  CrcInit();
}
