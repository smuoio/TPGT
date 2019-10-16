/**
* \file StartPhase.c
* \brief the file manages the start up phase synchronisation
* \author Muoio Salvatore
*/
/*#include "usertypes.h"*/
#include "Time_Util.h"
#include "Generic_api.h"
#include "GPP_Handle_Public.h"
#include "Comm_Util.h"
#include "StartPhase.h"


/**
* \var static e_StartPhase_fsm sStartPhase
* \brief variable used for the state of startphase state machine
*/
static e_StartPhase_fsm sStartPhase = StartPahse_Idle;
/**
* \var static t_Time TimeStartPhase
* \brief timer used tpo detect the end state of startphase state machine
*/
static t_Time TimeStartPhase = {0,0};

void StartPhase_init(void)
{
  /* set the timeout */
  SetTime(&TimeStartPhase, (T5S/TASK64));
}

void StartPahseHandle(void)
{
  switch(sStartPhase)
  {
    case StartPahse_Idle:
      /*do action*/
      /* read the version info of EVC-0*/
      if(GetEvcMmiStartReq() == VERSION_INFO_REQ)
      {
        /*transition*/
        sStartPhase = StartPahse_Init;
        mon_broadcast_printf("[StartPhase]::Idle->Init\n");
      }
    break;
    case StartPahse_Init:
    {
       BOOLEAN8 lIsOn       = FALSE;
	   BOOLEAN8 lIsEtcsISol = FALSE;
       /* do action*/
       /* check statu of EbiCab */
  	   isEtcsIsolated(&lIsEtcsISol);
       isEbiCabOn(&lIsOn);
       if((lIsOn == TRUE)  && (lIsEtcsISol == FALSE) &&
          (GetEvcMmiStartReq() == VERSION_INFO_REQ))
       {
            /*transition*/
            sStartPhase = StartPhase_Counter;
            ResetTime(&TimeStartPhase);
            mon_broadcast_printf("[StartPhase]::Init->Counter\n");
       }
    }
    break;
    case StartPhase_Counter:
    {
        BOOLEAN8 lIsOn          = FALSE;
        BOOLEAN8 lIsEbicabAlive = FALSE;
        BOOLEAN8 lIsHmiAlive    = FALSE;
        BOOLEAN8 lIsCcuoAlive   = FALSE;
        /*do action*/
        isEbiCabOn(&lIsOn);
        isEbiCabAlive(&lIsEbicabAlive);
        isHmiAlive(&lIsHmiAlive);
        isCcuoAlive(&lIsCcuoAlive);
        /*check the communication*/
        if((lIsEbicabAlive == TRUE) &&
           (lIsHmiAlive == TRUE)    &&
           (lIsCcuoAlive == TRUE))
        {
          sStartPhase = StartPhase_End;
          mon_broadcast_printf("[StartPhase]::Counter->End\n");
        }
        else
        {
          /*counter the time*/
          if(UpdateTime(&TimeStartPhase))
          {
            /*transition*/
            /*sStartPhase = StartPhase_Error;*/
			sStartPhase = StartPhase_End;
            ResetTime(&TimeStartPhase);
            mon_broadcast_printf("[StartPhase]::Counter->Error\n");
          }
        }
        /* check off conditions*/
        if(lIsOn == FALSE)
          /*transition*/
          sStartPhase = StartPahse_Init;
      }
    break;
    case StartPhase_End:
    {
        BOOLEAN8 lIsOn          = FALSE;
		BOOLEAN8 lIsEtcsISol    = FALSE;
		BOOLEAN8 lIsEbicabAlive = FALSE;
        /*do action*/
        isEbiCabOn(&lIsOn);
		isEtcsIsolated(&lIsEtcsISol);
		isEbiCabAlive(&lIsEbicabAlive);
        /* check off conditions*/
        if((lIsOn == FALSE)      || 
           (lIsEtcsISol == TRUE) || 
           (lIsEbicabAlive == FALSE) /*|| (GetEvcMmiStartReq() == VERSION_INFO_REQ)*/)
        {  /*transition*/
          sStartPhase = StartPahse_Init;
          mon_broadcast_printf("[StartPhase]::End->Init\n");
        }
    }
    break;
    case StartPhase_Error:
    {
        BOOLEAN8 lIsEbicabAlive = FALSE;
        BOOLEAN8 lIsHmiAlive    = FALSE;
        BOOLEAN8 lIsCcuoAlive   = FALSE;
      /*do action*/
        isEbiCabAlive(&lIsEbicabAlive);
        isHmiAlive(&lIsHmiAlive);
        isCcuoAlive(&lIsCcuoAlive);
		/*mon_broadcast_printf("[StartPhase]::Error->Counter>lIsEbicabAlive = %d\n",lIsEbicabAlive);
		mon_broadcast_printf("[StartPhase]::Error->Counter>lIsHmiAlive = %d\n",lIsHmiAlive);
		mon_broadcast_printf("[StartPhase]::Error->Counter>lIsCcuoAlive = %d\n",lIsCcuoAlive);*/
        if(/*(GetEvcMmiStartReq() == VERSION_INFO_REQ) &&*/
           (lIsEbicabAlive == TRUE)                  &&
           (lIsHmiAlive == TRUE)                     &&
           (lIsCcuoAlive == TRUE))
        {
          sStartPhase = StartPhase_Counter;
          mon_broadcast_printf("[StartPhase]::Error->Counter\n");
        }
    }
    break;
    default:
    break;
  }
}

e_StartPhase_fsm GetStartPahseStatus(void)
{
  return(sStartPhase);
}
