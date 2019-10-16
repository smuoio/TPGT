/**
* \file TractionBrake.c
* \brief the file manages the traction and brake commands
* \author Muoio Salvatore
*/
#include "Generic_api.h"
#include "Ccuo_Handle_Public.h"
#include "TractionBrake.h"
#include "TractionBrake_impl.h"


void TractionBrake_init(void)
{
  SetTime(GetTimeSound(), (T300ms/TASK64));
}

void TractionBrake_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  if(interface != 0)
  {
    /*BOOLEAN8 lIsCabActive = FALSE;
    isCabActive(&lIsCabActive);
    if(lIsCabActive == TRUE)*/
     {
       MaxHadCurr_Handle(&(interface->TSG_ETCS_MMI_DYN), &(interface->TSG_P2),
                         &(interface->CCUT2CCUO));
       EmergencyBrake_Handle(&(interface->TSG_P0), &(interface->CCUT2CCUO));

       TractionLock_Handle(&(interface->TSG_P0), &(interface->Vce_Tract_1),
                           &(interface->Vce_Tract_2));
       Cab_Handle(&(interface->Vce_Cab1_N), &(interface->Vce_Cab1_A));
     }
  }/*if*/
}


BOOLEAN8 GetEBZugbeeinflussung(void)
{
  return(Zugbeeinflussung);
}
