/**
* \file Generic_api.c
* \brief the file manages the application functions API used in the
*        application task
* \author Muoio Salvatore
*/

#include "Generic_api.h"
#include "StartPhase.h"
#include "Comm_Util.h"
#include "Ccuo_Handle_Public.h"
#include "Etcs_public.h"
#include "Hmi_Handle.h"
#include "Task64_Def.h"
#include "SpeedHandle_Public.h"

BOOLEAN8 isEbiCabAlive(BOOLEAN8 *isAlive)
{
  /*output used for the debug*/
  BOOLEAN8 lOut = FALSE;
  if(isAlive != 0)
  {
    e_StsComm lStsConn = CONNECTED;
    /* detect the connection*/
    *isAlive = FALSE;
    lStsConn |= CheckConn(GetBusLife(P0));  /*P0 dataset*/
    lStsConn |= CheckConn(GetBusLife(P1));  /*P1 dataset*/
    lStsConn |= CheckConn(GetBusLife(P2));  /*P2 dataset*/
    lStsConn |= CheckConn(GetBusLife(P3));  /*P3 dataset*/
    /*lStsConn |= CheckConn(GetBusLife(ETCS));ETCS dataset*/
    lStsConn |= CheckConn(GetBusLife(STM)); /*STM dataset*/
    if(lStsConn == CONNECTED)
        *isAlive = TRUE;
    lOut = TRUE;
  }/*if*/
  return(lOut);
}

BOOLEAN8 isETCSAlive(BOOLEAN8 *isAlive)
{
  /*output used for the debug*/
  BOOLEAN8 lOut = FALSE;
  if(isAlive != 0)
  {
    e_StsComm lStsConn = CONNECTED;
    /* detect the connection*/
    *isAlive = FALSE;
    lStsConn |= CheckConn(GetBusLife(ETCS)); /*STM dataset*/
    if(lStsConn == CONNECTED)
        *isAlive = TRUE;
    lOut = TRUE;
  }/*if*/
  return(lOut);
}

BOOLEAN8 isEbiCabOn(BOOLEAN8 *isOn)
{
  BOOLEAN8 lOut = FALSE;
  if(isOn != 0)
  {
    /* the function GetCcuoEbiCabOn is in negative logic,
      isOn is in positive logic */
    *isOn = !GetCcuoEbiCabOn();
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isEtcsIsolated(BOOLEAN8 *isIsolated)
{
  BOOLEAN8 lOut = FALSE;
  if(isIsolated != 0)
  {
    /*TODO*/
    if(GetCcuoEtcsIsolated() == SIGN_NOT_ACTIVE)
      *isIsolated = FALSE;
    else if(GetCcuoEtcsIsolated() == SIGN_ACTIVE)
      *isIsolated = TRUE;
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isHmiAlive(BOOLEAN8 *isAlive)
{
  BOOLEAN8 lOut = FALSE;
  if(isAlive != 0)
  {
    /*TODO check if HMI is connected*/
    if((((GetCcuoHmiAlive() ==  HMI1_ALIVE)         ||
         (GetCcuoHmiAlive() ==  HMI2_ALIVE)         ||
         (GetCcuoHmiAlive() ==  HMI3_ALIVE))        &&
       ((CheckConn(GetBusLife(HMI)) == CONNECTED)   ||
       (CheckConn(GetBusLife(HMI2)) == CONNECTED)))/* ||
       (GetCcuoCabActive() == TRUE)*/) /*hmi connection check*/
      *isAlive = TRUE;
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isCabActive(UINT8 *isActive)
{
  BOOLEAN8 lOut = FALSE;
  if(isActive != 0)
  {
    /* GetCabActiveSts() from CCUO !!! */
    UINT8 lCcuoCabActive = GetCcuoCabActive();
    UINT8 lEvcCabActive  = GetEvcActiveCab();/* from EVC-2 !!! */
    if(((lEvcCabActive == 1) || (lEvcCabActive == 2)) && (lCcuoCabActive == CAB_OCCUPIED))
      *isActive = TRUE;
    /*TODO check if the Cab is active*/
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isStandStill(BOOLEAN8 *aIsStandStill)
{
  BOOLEAN8 lOut = FALSE;
  if(aIsStandStill != 0)
  {
    /*TODO check the data from CCUO and we could read also the speed???*/
    *aIsStandStill = GetCcuoStandStill();
    lOut = TRUE;
  }/*if*/
  return(lOut);
}

/**
* \enum e_TcmsStatus
* \brief status of TCMS
*/
typedef enum
{
    TCMS_ISOLATION,   /*!< TCMS Isolated status*/
    TCMS_STARTING_UP, /*!< TCMS starting up status*/
    TCMS_IDLE,        /*!< TCMS Idle status */
    TCMS_ACTIVE,      /*!< TCMS Active status */
    TCMS_FAILURE,     /*!< TCMS Failure status */
}e_TcmsStatus;


/**
* \fn static e_TcmsStatus TCMSStatus(void)
* \brief The function detects the status of TCMS
* \return   ISOLATION(0),FAILURE(4), IDLE(2), ACTIVE(3) and STARTING_UP(1)
*/
static e_TcmsStatus TCMSStatus(void)
{
  static e_TcmsStatus lSts = TCMS_ISOLATION;
  BOOLEAN8 lIsHmiAlive     = FALSE;
  BOOLEAN8 lIsEbiCabOn     = FALSE;
  BOOLEAN8 lIsEtcsIsolated = TRUE;
  BOOLEAN8 lIsCabActive    = FALSE;
  isHmiAlive(&lIsHmiAlive);
  isEbiCabOn(&lIsEbiCabOn);
  isEtcsIsolated(&lIsEtcsIsolated);
  isCabActive(&lIsCabActive);
  switch(lSts)
  {
    case TCMS_ISOLATION:
    if((lIsEbiCabOn == TRUE)      &&
       (lIsEtcsIsolated == FALSE) &&
       (GetStartPahseStatus() != StartPhase_End))
    {
       lSts = TCMS_STARTING_UP;
       mon_broadcast_printf("TCMS_STARTING_UP\n");
    }
    break;
    case TCMS_STARTING_UP:
    if (GetEvcMmiStartReq() == IDLE_STATE) {
      /* code */
      lSts = TCMS_IDLE;
      mon_broadcast_printf("IDLE\n");
    }
    else if (((GetEvcMmiStartReq()   == MMI_NOT_SUPPORTED)        ||
             (GetEvcMmiStartReq()    == INCOMPATIBLE_IF_VERSIONS) ||
             (GetEvcMmiStartReq()    == INCOMPATIBLE_SW_VERSIONS)))
    {
        lSts = TCMS_FAILURE;
        mon_broadcast_printf("TCMS_FAILURE\n");
    }
    break;
    case TCMS_IDLE:
    if(lIsCabActive == TRUE)
    {
            lSts = TCMS_ACTIVE;
            mon_broadcast_printf("TCMS_ACTIVE\n");
    }
    else if(GetEvcMmiStartReq() == VERSION_INFO_REQ){
      lSts = TCMS_STARTING_UP;
      mon_broadcast_printf("TCMS_STARTING_UP\n");
    }
    break;
    case TCMS_ACTIVE:
    {
      if(lIsCabActive == FALSE)
      {
        lSts = TCMS_IDLE;
        mon_broadcast_printf("TCMS_IDLE\n");
      }
      else if(GetEvcMmiStartReq() == VERSION_INFO_REQ)
      {
        lSts = TCMS_STARTING_UP;
        mon_broadcast_printf("TCMS_STARTING_UP\n");
      }
    }
    break;
    default:
    lSts = TCMS_ISOLATION; /*default value*/
    break;
  }
  return(lSts);
}

/**
* \var static BOOLEAN8 sAtpDownAlrmActive
* \brief static variable used to detect the falling edge of AtpDownAlarm
*/
static BOOLEAN8 sAtpDownAlrmActive = FALSE;

e_MmiMMmiStatus SetTcmsSts2EbiCab(void)
{
  e_MmiMMmiStatus lRes   = IDLE;
  BOOLEAN8 lIsAtpDownAlrmActive = FALSE;
  e_TcmsStatus lTcmsSts         = TCMSStatus();
  isAtpDownAlarmActive(&lIsAtpDownAlrmActive);
  if(lTcmsSts == TCMS_ACTIVE)
  {
    /*if (FallingEdge(lIsAtpDownAlrmActive, &sAtpDownAlrmActive) == FALSE)
      lRes = ACTIVE;*/
    lRes = ACTIVE;
    if(lIsAtpDownAlrmActive == TRUE)
    {
         if(FallingEdge(GetAtpDownAlrmShow(), &sAtpDownAlrmActive) == TRUE)
         {
             lRes = ATP_DOWN_NACK;
         }
         else if(RisingEdge(GetAtpDownAlrmShow(), &sAtpDownAlrmActive) == TRUE)
         {
             lRes = ATP_DOWN_ACK;
         }
    }
  }
  else
  {
      if(lTcmsSts == TCMS_FAILURE)
        lRes = FAILURE;
  }
  mon_broadcast_printf("SetTcmsSts2EbiCab_ = %d\n", lRes);
  return(lRes);
}

BOOLEAN8 isCcuoAlive(BOOLEAN8 *isAlive)
{
  BOOLEAN8 lOut = FALSE;
  if(isAlive != 0)
  {
    e_StsComm lStsConn = CONNECTED;
    lStsConn |= CheckConn(GetBusLife(CCUO));  /*CCUO->CCUT dataset*/
    if(lStsConn == CONNECTED)
        *isAlive = TRUE;
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isPzbIsolated(BOOLEAN8 *isIsolated)
{
  BOOLEAN8 lOut = FALSE;
  if(isIsolated != 0)
  {
    if(GetCcuoPzbIsolated() == SIGN_NOT_ACTIVE)
      *isIsolated = FALSE;
    else if(GetCcuoPzbIsolated() == SIGN_ACTIVE)
      *isIsolated = TRUE;
    lOut = TRUE;
  }
  return(lOut);
}
BOOLEAN8 isLzbIsolated(BOOLEAN8 *isIsolated)
{
  BOOLEAN8 lOut = FALSE;
  if(isIsolated != 0)
  {
    if(GetCcuoLzbIsolated() == SIGN_NOT_ACTIVE)
      *isIsolated = FALSE;
    else if(GetCcuoLzbIsolated() == SIGN_ACTIVE)
      *isIsolated = TRUE;
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isAtpDownAlarmActive(BOOLEAN8 *isAtpDwnActive)
{
  BOOLEAN8 lOut = FALSE;
  if(isAtpDwnActive != 0)
  {
    BOOLEAN8 lisAlive     = FALSE;
    BOOLEAN8 lisIsolated  = FALSE;
    isETCSAlive(&lisAlive);/*check if the Etcs dataset is alive*/
    isEtcsIsolated(&lisIsolated);/*check if the Etcs is isolated*/
    if((lisAlive == FALSE) && (lisIsolated == FALSE) &&
       (GetCcuoCabActive() == CAB_OCCUPIED) &&
       (GetStartPahseStatus() == StartPhase_End))
    {
      /* set local variable bATPDown*/
      *isAtpDwnActive = TRUE;
    }/*if*/
    lOut = TRUE;
  }/*if*/
  return(lOut);
}

BOOLEAN8 isScreenDel(BOOLEAN8 *isScreenDel)
{
  BOOLEAN8 lOut = FALSE;
  /*e_TcmsStatus aTcmsSts = TCMSStatus();*/
  if(isScreenDel != 0)
  {
    if((GetStmModus() != STM_MODUS_DATA_AVAILABLE) &&
       (GetStartPahseStatus() != StartPhase_End) /*&&
     (aTcmsSts != TCMS_ACTIVE)*/)
       *isScreenDel = TRUE;
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isIntTestAct(BOOLEAN8 *isAct)
{
  BOOLEAN8 lOut = FALSE;
  if(isAct != 0)
  {
    *isAct = GetCcuoIntegrityTest();
    lOut = TRUE;
  }
  return(lOut);
}

BOOLEAN8 isTrainSpeedValid(void)
{
  return(((GetCcuoSpeed() != ACTUAL_SPEED_NOT_AVAILABLE) ||
           (GetSpeedSelected()->Validity == TRUE)) ? TRUE : FALSE);
}
