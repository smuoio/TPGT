/**
* \file Hmi_Handle_impl.c
* \brief the file manages the private functions of hmi
* \author Muoio Salvatore
*/
#include "Task64_Def.h"
#include "Generic_api.h"
#include "GPP_Handle_Public.h"
#include "Ccuo_Handle_Public.h"
#include "Etcs_public.h"
#include "StartPhase.h"
#include "SpeedHandle_Public.h"
#include "Hmi_Handle_impl.h"

/**
* \struct t_Speedlimit
* \brief field of speed range
*/
typedef struct
{
  INT32 Min; /*!<Min speed value*/
  INT32 Max; /*!<Max speed value*/
}t_Speedlimit;

t_Speedlimit SpeedLimit;

BOOLEAN8 AtpDownAlrmShow = FALSE;
/**
* \fn static void set_speedlimit(INT32 aMin, INT32 aMax)
* \brief the function sets the minimum and maximum speed value
* \param aMin minimum value
* \param aMax maximum value
*/
static void set_speedlimit(INT32 aMin, INT32 aMax)
{
  SpeedLimit.Min = aMin;
  SpeedLimit.Max = aMax;
}
/**
* \fn static void SetBlockData(DS_CCUT_HMI_TRN * const pCcutHmiTrn,
*                              BOOLEAN8 Val)
* \brief the function sets value of JRU Status to HMI
* \param pCcutHmiTrn pointer to the DS_CCUT_HMI_TRN structure
* \param Val boolean value
*/
static void SetBlockData(DS_CCUT_HMI_TRN * const pCcutHmiTrn,
                         BOOLEAN8 Val)
{
  if(pCcutHmiTrn != 0)
  {
    pCcutHmiTrn->TPT_JRUStatus.Locked     = Val;
    pCcutHmiTrn->TPT_ButtonOptions.JRU_V  = TRUE;
  }/*if*/
}


e_result BlockData(const JRU_STATUS_IP * const aJruSts,
                   DS_CCUT_HMI_TRN * const aCcutHmiTrn)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((JRU_STATUS_IP*)aJruSts);
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)aCcutHmiTrn);
  if(TPG_SUCCEEDED(lChk))
  {
    SetBlockData(aCcutHmiTrn, FALSE);
    if(aJruSts->STblocked == TRUE)
    {
      SetBlockData(aCcutHmiTrn, TRUE);
    }/*if*/
  }/*if*/
  return(lChk);
}
/**
* \fn static void SetAtpDwnAlarm(DS_CCUT_HMI_TRN *const pout,
*                                unsigned char AtpDownId,,
                                UINT32 aNewEvc8Txt)
* \brief the function sets ATP Down and text
* \param pout pointer to the DS_CCUT_HMI_TRN structure
* \param AtpDownId text id
* \param aNewEvc8Txt NewEvc8Txt
*/
static void SetAtpDwnAlarm(DS_CCUT_HMI_TRN *const pout, unsigned char AtpDownId,
                          UINT32 aNewEvc8Txt)
{
  /*ATP_DOWN_ALARM*/
  if(pout != 0)
  {
    pout->TPT_NonAckDriverMessage.NewEvc8Text = aNewEvc8Txt;
    pout->TPT_StmSound.ATPDown                = AtpDownId;
    pout->TPT_RequestedDialog.Acknowledgment  = 2;
  }/*if*/
}

e_result AtpDownAlarm(const DS_HMI_CCUT * const pin, DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_HMI_CCUT*)pin);
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lIsAtpDwnActive = FALSE;
	  BOOLEAN8 lIsAlive        = FALSE;
    static BOOLEAN8 sTrigger = FALSE;
	  isEbiCabAlive(&lIsAlive);
    isAtpDownAlarmActive(&lIsAtpDwnActive);
    if(lIsAtpDwnActive == TRUE)
    {
      if(RisingEdge(lIsAtpDwnActive, &sTrigger) == TRUE)
      {
        /*no driver action on the hmi*/
        SetAtpDwnAlarm(pout, TEXT_ATP_DOWN_ALARM, ATP_DOWN_ALARM);
	      pout->TPT_ApproveTextMessage.TextId  = 1;
        SetBoolean(&AtpDownAlrmShow, TRUE);
        /*set the mask id*/
        SetMaskIdRequest(TxtMsgForConfirm);
      }
      else if((pin->TPT_Commands.MessageId == HMI_MSGID_TEXT_CONFIRMED) &&
              (pin->TPT_TextMessageApproval.Approval == HMI_APPROVED))
      {
        /*enter here only after pressed the softkey*/
        SetAtpDwnAlarm(pout, QUIT_ATP_DOWN_ALARM, ATP_DOWN_ALARM);
        SetBoolean(&AtpDownAlrmShow, FALSE);
      }
    }
    else
	 {
    sTrigger = FALSE;
	  if(lIsAlive == FALSE)
	  {
		    SetAtpDwnAlarm(pout, QUIT_ATP_DOWN_ALARM, QUIT_ATP_DOWN_ALARM);
		    SetMaskIdRequest(NavigationTask);
		    AtpDownAlrmShow = FALSE;
	  }
	 }
  }
  return(lChk);
}

/**
* \fn static void SetVisibleButton(BOOLEAN8 *const pButton, BOOLEAN8 aValue)
* \brief generic function used to set the name of  button on the hmi
* \param pButton button to set or reset.
* \param aValue value (TRUE show, FALSE hide)
*/
static void SetVisibleButton(BOOLEAN8 *const pButton, BOOLEAN8 aValue)
{
  if(pButton != 0)
  {
    *pButton = aValue;/*set the button*/
  }/*if*/
}

static BOOLEAN8 isButtonVisible(const BOOLEAN8 *const pButton)
{
  return ((pButton !=0 ) ? (*pButton): FALSE);
}

e_result isZDEButtonVisble(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  lChk |= checkOutputArg((BOOLEAN8*)Out);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lIsStandStill;
    UINT32 lStartTrainDataEntry = (GetEVC21EnableDriverRequest()&ENABLE_START_TRAIN_DATA_ENTRY);
    isStandStill(&lIsStandStill);
    {
      BOOLEAN8 lRes = ((lIsStandStill == TRUE) &&
                       (lStartTrainDataEntry == ENABLE_START_TRAIN_DATA_ENTRY));
      SetVisibleButton(&pout->TPT_ButtonOptions.ZDE_V, lRes);
      *Out = lRes;
    }/*body*/
  }/*if*/
  return(lChk);
}

e_result isZDButtonVisible(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  lChk |= checkOutputArg((BOOLEAN8*)Out);
  if(TPG_SUCCEEDED(lChk))
  {
    UINT32 lStartTrainDataView = (GetEVC21EnableDriverRequest()&ENABLE_START_TRAIN_DATA_VIEW);
    SetVisibleButton(&pout->TPT_ButtonOptions.ZD_V,
                     (lStartTrainDataView == ENABLE_START_TRAIN_DATA_VIEW));
    *Out = (lStartTrainDataView == ENABLE_START_TRAIN_DATA_VIEW);
  }/*if*/
  return(lChk);
}

e_result isTestBWGButtonVisible(DS_CCUT_HMI_TRN *const pout, BOOLEAN8 *Out)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  lChk |= checkOutputArg((BOOLEAN8*)Out);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lIsStandStill;
    isStandStill(&lIsStandStill);
    UINT32 lEnableStartBrakeTest = (GetEVC21EnableDriverRequest()&ENABLE_START_BRAKE_TEST);
	  {
		    BOOLEAN8 lRes = ((lIsStandStill == TRUE) &&
                         (lEnableStartBrakeTest == ENABLE_START_BRAKE_TEST));
		    SetVisibleButton(&pout->TPT_ButtonOptions.PruefenSBV_V,
                         lRes);
		    *Out = (lRes);
	  }/*body*/
  }/*if*/
  return(lChk);
}

e_result EnableTestPzbLzbButton(DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lIsStandStill;
    isStandStill(&lIsStandStill);
    {
      BOOLEAN8 lRes = (lIsStandStill & GetButtonRequestTestLzbPzb());
      /*set zugbesy button!!!*/
      SetVisibleButton(&pout->TPT_ButtonOptions.PruefenZugBesy_V, lRes);
    }/*body*/
  }/*if*/
  return(lChk);
}
/*Prufen_V*/

e_result EnableTestButton(DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lIsStandStill;
    BOOLEAN8 lBWGButton    = isButtonVisible(&pout->TPT_ButtonOptions.PruefenSBV_V);
    BOOLEAN8 lPzbLzbButton = isButtonVisible(&pout->TPT_ButtonOptions.PruefenZugBesy_V);
    isStandStill(&lIsStandStill);
    {
      BOOLEAN8 lRes = (lIsStandStill & (lBWGButton | lPzbLzbButton));
      SetVisibleButton(&pout->TPT_ButtonOptions.Pruefen_V, lRes);
    }/*body*/
  }
  return(lChk);
}

e_result EnableDelScreen(DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lIsScreenDel = FALSE;
    isScreenDel(&lIsScreenDel);
    if(lIsScreenDel == TRUE)
    {
       pout->TPT_NonAckDriverMessage.NewEvc8Text = 0;
       /*pout->TPT_ApproveTextMessage.TextId       = 0;*/
    }/*if*/
  }/*if*/
  return(lChk);
}
/**
* \fn static BOOLEAN8 isSpeedPermitted(INT16 aSpeed)
* \brief the function checks if the speed is in the range
* \param aSpeed speed value
*/
static BOOLEAN8 isSpeedPermitted(INT16 aSpeed)
{
  return(((aSpeed>=SpeedLimit.Min)&&(aSpeed<=SpeedLimit.Max)) ? TRUE :FALSE);
}

e_result EnableAllowedSpeed(const TSG_STM_MMI_DYN *const pTsgStm,
                            DS_CCUT_HMI_TRN * const pCcutHmiTrn)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TSG_STM_MMI_DYN*)pTsgStm);
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pCcutHmiTrn);
  if(TPG_SUCCEEDED(lChk))
  {
    INT16 lQIndiCate = ((pTsgStm->Q_IND_SCALE >> 3)&1);
    pCcutHmiTrn->TPT_PermittedSpeed.Visible = FALSE;
    set_speedlimit(0, 600);
    if((GetSpeedSelected()->Validity == TRUE)        &&
       (GetStmModus() == STM_MODUS_DATA_AVAILABLE)   &&
       (isSpeedPermitted(pTsgStm->V_PERMIT) == TRUE) &&
       (lQIndiCate == 0))
    {
      pCcutHmiTrn->TPT_PermittedSpeed.Value   = pTsgStm->V_PERMIT;
      pCcutHmiTrn->TPT_PermittedSpeed.Visible = TRUE;
    }
  }
  return(lChk);
}

e_result EnableTargetSpeed(const TSG_STM_MMI_DYN *const pTsgStm,
                           DS_CCUT_HMI_TRN * const pCcutHmiTrn)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TSG_STM_MMI_DYN*)pTsgStm);
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pCcutHmiTrn);
  if(TPG_SUCCEEDED(lChk))
  {
    UINT16 lSpeed = 0U;
    UINT8 l7Seg[4];
    INT16 lQIndiCate = ((pTsgStm->Q_IND_SCALE >> 4)&1);
    pCcutHmiTrn->TPT_TargetSpeed.Visible    = FALSE;
    pCcutHmiTrn->TPT_AddtnlSpeedInf.Visible = FALSE;
    set_speedlimit(0, 120);
    if((GetSpeedSelected()->Validity == TRUE)        &&
       (GetStmModus() == STM_MODUS_DATA_AVAILABLE)   &&
       (isSpeedPermitted(pTsgStm->V_TARGET) == TRUE) &&
       (lQIndiCate == 0))
    {
      lSpeed = pTsgStm->V_TARGET*5;
      pCcutHmiTrn->TPT_TargetSpeed.Value   = lSpeed;
      pCcutHmiTrn->TPT_TargetSpeed.Visible = TRUE;
      /*added the lines to enable the three digit */
      ConvertNumericU16ValTo7Seg(lSpeed,
                                &l7Seg[0],
                                sizeof(l7Seg));
      memcpy(&pCcutHmiTrn->TPT_AddtnlSpeedInf.Value[0],
             &l7Seg[0],
             sizeof(l7Seg));
      pCcutHmiTrn->TPT_AddtnlSpeedInf.Visible = TRUE;
    }
  }
  return(lChk);
}

/**
* \fn static UINT8 LookUpTableScale(UINT8 aValue)
* \brief loouptable
* \param aValue
* \return value in Tab
*/
static UINT8 LookUpTableScale(UINT8 aValue)
{
  static const UINT8 Tab[] = {1,10,100,0,0,0,0,0,0,0,0,0,0,0,0,0};
  return(Tab[aValue]);
}

e_result EnableTargetDistance(const TSG_STM_MMI_DYN *const pTsgStm,
                           DS_CCUT_HMI_TRN * const pCcutHmiTrn)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TSG_STM_MMI_DYN*)pTsgStm);
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pCcutHmiTrn);
  if(TPG_SUCCEEDED(lChk))
  {
      UINT8 l7Seg[5];
      INT16 lQIndiCate = ((pTsgStm->Q_IND_SCALE >> 5)&1);
      INT16 lQIndScale = LookUpTableScale((pTsgStm->Q_IND_SCALE >> 14)&3);
      pCcutHmiTrn->TPT_TargetDistance.Visible = FALSE;
      if((GetSpeedSelected()->Validity == TRUE)        &&
         (GetStmModus() == STM_MODUS_DATA_AVAILABLE)   &&
         (lQIndiCate == 0) && (lQIndScale != 3))
      {
        UINT32 lDistNum = (pTsgStm->D_TARGET*lQIndScale)/10;
        ConvertNumericU32To7Seg(lDistNum, &l7Seg[0], sizeof(l7Seg));
        pCcutHmiTrn->TPT_TargetDistance.ValueNumeric   = pTsgStm->D_TARGET;
        memcpy(&pCcutHmiTrn->TPT_TargetDistance.ValueDigital[0],
               &l7Seg[0],
               sizeof(l7Seg));
        pCcutHmiTrn->TPT_TargetDistance.Visible = TRUE;
      }
  }
  return(lChk);
}

e_result EnableSwVersion(TPT_IVersionInfo *const pOut, const TSG_P3 * const pTsgP3)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TPT_IVersionInfo*)pOut);
  if(TPG_SUCCEEDED(lChk))
  {
    /*check packet enbale!!!!*/
    if(isReceivedSTM1() == TRUE)
    {
      /*copy version to HMI*/
      /*pOut->E2K_SysRelP1    = 0x1530;*/
      pOut->E2K_SysRelValid = TRUE;
      pOut->E2K_MMIIfP1     = 32;
      pOut->E2K_STMAppP1    = GetMmiStmN058Vermajor();
      pOut->E2K_STMAppP2    = GetMmiStmN058Vermid();
      pOut->E2K_STMAppP3    = GetMmiStmN058Verminor();
      pOut->E2K_STMFFFISP1  = GetMmiStmN035Vermajor();
      pOut->E2K_STMFFFISP2  = GetMmiStmN035Vermid();
      pOut->E2K_STMFFFISP3  = GetMmiStmN035Verminor();
      pOut->E2K_STMSRS26P1  = GetMmiStmNSrsVermajor();
      pOut->E2K_STMSRS26P2  = GetMmiStmNSrsVerminor();
      pOut->E2K_SysRelP1    = pTsgP3->SystemRelease;
      (pTsgP3->SystemRelease == 0) ? (pOut->E2K_SysRelValid = FALSE) :
                                     (pOut->E2K_SysRelValid = TRUE);
    }/*if*/
  }
  return(lChk);
}


e_result Speed2Hmi(DS_CCUT_HMI_TRN * const pCcutHmiTrn)
{
 e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pCcutHmiTrn);
  if(TPG_SUCCEEDED(lChk))
  {
	  UINT16 lSpeed      = 0x0U;
	  UINT16 lSpeedV     = 0x0U;
    BOOLEAN8 lHmiAlive = FALSE;
    isHmiAlive(&lHmiAlive);
	  /*SCR 18541*/
	  if((lHmiAlive == TRUE) &&
       (isTrainSpeedValid() == TRUE))
	  {
		  /*speed !!!*/
		  lSpeed  = ((GetSpeedSelected()->Speed + 50)/100);
		  /*validity*/
		  lSpeedV = 0xFFFF;
	  }/*if*/
	  pCcutHmiTrn->TPT_SELSpeed.TrainSpeed  = lSpeed;
	  pCcutHmiTrn->TPT_SELSpeed.TrainSpeedV = lSpeedV;
  }/*if*/
  return(lChk);
}
