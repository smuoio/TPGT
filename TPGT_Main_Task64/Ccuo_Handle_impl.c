/**
* \file Ccuo_Handle_impl.c
* \brief the file manages the private functions of ccuo
* \author Muoio Salvatore
*/

#include "Ccuo_Handle_impl.h"
#include "Ccuo_Handle_Public.h"
#include "Diag_Handle_Public.h"
#include "Hmi_Request_Public.h"
#include "Hmi_TextMessage_Public.h"
#include "Cesa_Handle.h"
#include "StartPhase.h"
#include "Generic_api.h"
#include "Diag_Handle_Public.h"

/**
* \var static t_Time TimeMaskId
* \brief timer used to detect the change mask
*/
static t_Time TimeMaskId = {0,0};
/**
* \var static BOOLEAN8 sBrakeTestAct
* \brief variable used to detect the end of brake test
*/
static BOOLEAN8 sBrakeTestAct = FALSE;

/**
* \fn static void SetWheelDiam(CCUT_CCUO * const pCcut, UINT16 WheelDiam1, UINT16 WheelDiam2)
* \brief static function to set the field of CCUT_CCUO struct
* \param pCcut pointer to output to CCUO, struct CCUT_CCUO
* \param WheelDiam1 value of wheel diamenter 1
* \param WheelDiam1 value of wheel diamenter 2
* \return null
*/
static void SetWheelDiam(CCUT_CCUO * const pCcut, UINT16 WheelDiam1, UINT16 WheelDiam2)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((CCUT_CCUO*)pCcut);/*check pointer valid*/
  if(TPG_SUCCEEDED(lChk))
  {
    pCcut->IWheelDiam1  = WheelDiam1;
    pCcut->IWheelDiam2  = WheelDiam2;
    pCcut->IWheelDiam1V = TRUE;
    pCcut->IWheelDiam2V = TRUE;
  }/*if*/
}

/**
* \fn static INT8 SetUtcOffset(void)
* \brief static function to set the offset
* \return the value to send to EbiCab
*/
static INT8 SetUtcOffset(void)
{
  INT8 lOffset = GetUtcOffs();
  return((GetDaySavTi() == FALSE) ? lOffset : lOffset+4);
}

void SetTime2EbiCab(VCE_AUX_OUT * const pVce, MMI_AUX_OUT * const pMmi,
                    const UINT8 aMmiMode)
{
  const UINT8 SB_STANDBY = 6;
  pVce->UTC_valid  = FALSE;
  pMmi->UTC_valid  = FALSE;
  UINT32 lUtcTime = GetUtcTime();
  if((GetUTCTimeV() == TRUE) && (aMmiMode != SB_STANDBY))
  {
    /*set the time*/
    memcpy(&pMmi->UTC_time, &lUtcTime, sizeof(UINT32));
    memcpy(&pVce->UTC_time, &lUtcTime, sizeof(UINT32));
    /*set the offset*/
    pMmi->UTC_offset = SetUtcOffset();
    pVce->UTC_offset = SetUtcOffset();
    pVce->UTC_valid  = TRUE;
    pMmi->UTC_valid  = TRUE;
  }
}

e_result WheelDiam(CCUT_CCUO * const pCcut, const TSG_P1 * const pTsgP1)
{
  e_result lChk = TPG_SUCCESS;
  /*check pointer valid*/
  lChk |= checkOutputArg((CCUT_CCUO*)pCcut);
  lChk |= checkOutputArg((TSG_P1*)pTsgP1);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lisEbiCabAlive = FALSE;
    isEbiCabAlive(&lisEbiCabAlive);
    SetWheelDiam(pCcut, 0, 0);/*default!!!*/
    if(lisEbiCabAlive == TRUE)
    {
      SetWheelDiam(pCcut, pTsgP1->WheelDiam1, pTsgP1->WheelDiam2);
    }
  }
  return(lChk);
}

e_result SetSpeedSelected(CCUT_CCUO * const pCcut, t_SpeedSelected *aSpeedSel)
{
  e_result lChk = TPG_SUCCESS;
  /*check pointer valid*/
  lChk |= checkOutputArg((CCUT_CCUO*)pCcut);
  lChk |= checkOutputArg((t_SpeedSelected*)aSpeedSel);
  if(TPG_SUCCEEDED(lChk))
  {
    pCcut->IEbiCurSpdV = GetSpeedSelected()->Validity;
    pCcut->IEbiCurSpd  = GetSpeedSelected()->Speed;
  }/*if*/
  return(lChk);
}

/**
* \fn static UINT16 checkmaskid(UINT16 *pCurrMask)
* \brief static function to manage the maskid od HMI
* \return the value of maskid to send to CCUO
*/
static UINT16 checkmaskid(UINT16 *pCurrMask)
{
  if(pCurrMask != 0)
  {
    if((*pCurrMask == TrainMenu) || (*pCurrMask == BasicMask))
    {
      /*check time*/
      if(UpdateTime(&TimeMaskId))
      {
        *pCurrMask = NavigationTask;
        /*reset sme request in pending*/
        ResetDrvRqst();
      }
    }
    else
    {
      /*reset time*/
      ResetTime(&TimeMaskId);
    }
  }
  return(*pCurrMask);
}

void EnbaleMask2Vso(CCUT_CCUO * const pCcut, UINT16 *pMaskIdRqst)
{
  if((pCcut != 0) && (pMaskIdRqst !=0))
  {
/*    BOOLEAN8 lIsCabActiv = FALSE;
    isCabActive(&lIsCabActiv);*/
    /*check the end of phase*/
    if((GetStartPahseStatus() == StartPhase_End) &&
       (GetCcuoCabActive() == CAB_OCCUPIED))
    {
      /*set the mask number to the output structure*/
      /*if(pCcut->CMaskIdReq != *pMaskIdRqst)*/
        pCcut->CMaskIdReq  = checkmaskid(pMaskIdRqst);
        pCcut->CMaskIdReqV = TRUE;
      /**pMaskIdRqst = 0;*/
    }
    else
    {
      /*set invalid the validity*/
	  pCcut->CMaskIdReq  = NavigationTask;
	  *pMaskIdRqst       = NavigationTask;
      pCcut->CMaskIdReqV = FALSE;
    }
  }/*if*/
}

e_result ReportBrakeTest(CCUT_CCUO * const pCcut, const TSG_P0 * const pTsgP0)
{
  e_result lChk = TPG_SUCCESS;
  /*check pointer valid*/
  lChk |= checkOutputArg((CCUT_CCUO*)pCcut);
  lChk |= checkOutputArg((TSG_P0*)pTsgP0);
  if(TPG_SUCCEEDED(lChk))
  {
    if((GetDrvRqst() == DrvRqst_SBV)    &&
       ((Get_NoSetSBVRequest() == TRUE) ||
       (FallingEdge(pTsgP0->BrakeTestAct, &sBrakeTestAct) == TRUE)))
    {
      ResetDrvRqst(); /*reset request of brake test*/
      SetMaskIdRequest(TrainMenu);
    }/*if*/
    /*the copy is done always*/
    SetBoolean((BOOLEAN8*)&pCcut->IBrkTestAct, pTsgP0->BrakeTestAct);
    SetBoolean((BOOLEAN8*)&pCcut->IBrkTestActV, TRUE);
  }/*if*/
  return(lChk);
}

static void CancelMenuCheck(void)
{
  UINT8 lIsStandStill = FALSE;
  isStandStill(&lIsStandStill);
  if((GetCcuoHmiMfd() == CheckMenu) && (lIsStandStill == FALSE))
    SetMaskIdRequest(BasicMask);
}

t_Time *GetTimeMaskId(void)
{
  return(&TimeMaskId);
}


e_result CesaTestFeedback(CCUT_CCUO * const pCcut)
{
  e_result lChk = TPG_SUCCESS;
  /*check pointer valid*/
  lChk |= checkOutputArg((CCUT_CCUO*)pCcut);
  if(TPG_SUCCEEDED(lChk))
  {
    pCcut->IIntTestTimeout  = GetCesaTimeOut();
    pCcut->ICcutSupervisErr = GetEventList()->F_CesaSupervisor;
    pCcut->IIntTestTimeoutV = TRUE;
  }
  return(lChk);
}

e_result UpdateDiagEvent(CCUT_CCUO * const pCcut)
{
  e_result lChk = TPG_SUCCESS;
  /*check pointer valid*/
  lChk |= checkOutputArg((CCUT_CCUO*)pCcut);
  if(TPG_SUCCEEDED(lChk))
  {
	 if(GetStartPahseStatus() == StartPhase_End)
	 {
		pCcut->IComProbEbiTsgP0   = GetEventList()->CommList.CommMVB.F_TSG_P0;
		pCcut->IComProbEbiTsgP1   = GetEventList()->CommList.CommMVB.F_TSG_P1;
		pCcut->IComProbEbiTsgP2   = GetEventList()->CommList.CommMVB.F_TSG_P2;
		pCcut->IComProbEbiTsgP3   = GetEventList()->CommList.CommMVB.F_TSG_P3;
		pCcut->IComProbEbiTsgEtcs = GetEventList()->CommList.CommMVB.F_TSG_ETCS;
		pCcut->IComProbEbiTsgStm  = GetEventList()->CommList.CommMVB.F_TSG_STM;
		pCcut->IComProbCcuo       = GetEventList()->CommList.CommEth.F_Ccuo2Ccut;
		pCcut->IComProbHMI        = GetEventList()->CommList.CommEth.F_HMI1;
		pCcut->CTpgtToDiagV       = TRUE;
	 }
  }
  return(lChk);
}
