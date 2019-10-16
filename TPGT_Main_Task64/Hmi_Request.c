/**
* \file Hmi_Request.c
* \brief the file manages the private functions of hmi regarding
*        the request of drive
* \author Muoio Salvatore
*/

#include "Time_Util.h"
#include "Task64_Def.h"
#include "Ccuo_Handle_Public.h"
#include "Gpp_Handle_Public.h"
#include "Hmi_Request.h"
#include "Hmi_Request_Public.h"
#include "Hmi_TextMessage_Public.h"

/**
* \var static UINT8 sEVC101Request
* \brief static variable used for the EVC-101 request
*/
static UINT8     sEVC101Request = 0x0U;
/**
* \var static UINT8 sEVC104Request
* \brief static variable used for the EVC-104 request
*/
static UINT8     sEVC104Request = 0x0U;
/**
* \var static UINT8     sEVC111Request
* \brief static variable used for the EVC-111 request
*/
static UINT8     sEVC111Request = 0x0U;
/**
* \var static UINT8     sEVC123Request
* \brief static variable used for the EVC-123 request
*/
static UINT8     sEVC123Request = 0x0U;
/**
* \var static UINT8     sSTM34Request
* \brief static variable used for the STM-34 request
*/
static UINT8     sSTM34Request  = 0x0U;
/**
* \var static UINT16     sEVC111TextID
* \brief static variable used for text id of EVC-111
*/
static UINT16    sEVC111TextID  = 0x0U;
/**
* \var static UINT8     sEVC111Approval
* \brief static variable used for the approval of EVC-111
*/
static UINT8     sEVC111Approval= 0x0U;
/**
* \var static UINT8     sSTM34QButton
* \brief static variable used for the STM-34
*/
static UINT8     sSTM34QButton  = 0x0U;
/**
* \var static e_DrvRqst     sDriverRequest
* \brief static variable used for the EVC-101 request
*/
static e_DrvRqst sDriverRequest = 0x0U;
/**
* \var static BOOLEAN8     sZugBesyWait
* \brief static variable used for set if the mask is in waiting
*/
static BOOLEAN8  sZugBesyWait   = FALSE;
/**
* \var static t_Time TimeMask
* \brief static variable used for set the time to check the status of mask id
*/
static t_Time TimeMask = {0,0};

void SetPendingDisplay(BOOLEAN8 aValue)
{
	sZugBesyWait = aValue;
}

BOOLEAN8 CheckMaskRequestBusy(void)
{
	if(sZugBesyWait == TRUE)
	{
		if((UpdateTime(&TimeMask)))
		{
			sZugBesyWait   = FALSE;
			sDriverRequest = DrvRqst_NO;
			SetMaskIdRequest(TrainMenu);
			ResetTime(&TimeMask);
		}
	}
	else
		ResetTime(&TimeMask);
	return(sZugBesyWait);
}


e_result EnableNewTrainNumberRequest(const TPT_Uint32Valid * const pCmd)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TPT_Uint32Valid*)pCmd);
  if(TPG_SUCCEEDED(lChk))
  {
		UINT32 lChangeTrianNumber = GetEVC21EnableDriverRequest()&ENABLE_CHANGE_TRAIN_NUMBER;
		if(((lChangeTrianNumber == ENABLE_CHANGE_TRAIN_NUMBER) && !sDriverRequest &&
		    (pCmd->Valid == TRUE))) /*add the and condition about counter*/
    {
      sEVC101Request = EVC101_TRAIN_NUMBER_REQUEST;/*request active*/
      sDriverRequest = DRVRqst_ChnTrainNumber;     /*request train id*/
      lChk = TRUE;
    }
  }
  return(lChk);
}

e_result EnableDriverIdRequest(const TPT_Uint32Valid * const pCmd)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TPT_Uint32Valid*)pCmd);
  if(TPG_SUCCEEDED(lChk))
  {
		UINT32 lDriverId = GetEVC21EnableDriverRequest()&ENABLE_MMIM_Q_REQUEST;
		if(((lDriverId == ENABLE_MMIM_Q_REQUEST) && !sDriverRequest &&
		    (pCmd->Valid == TRUE))) /*add the and condition about counter*/
    {
      sEVC101Request = EVC101_DRIVER_ID_REQUEST;/*request active*/
      sDriverRequest = DRVRqst_DrvId;           /*request train id*/
      lChk = TRUE;
    }
  }
  return(lChk);
}


e_result EnableZDERequest(const TPG_Commands * const pCmd, DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  lChk |= checkOutputArg((TPG_Commands*)pCmd);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lButtonVis = FALSE;
    isZDEButtonVisble(pout, &lButtonVis);
    if((pCmd->MessageId == EDIT_LZB_PZB_DATA) && !sDriverRequest && lButtonVis)
    {
      sEVC101Request = EVC101_ZDE_REQUEST;/*request active*/
      sDriverRequest = DrvRqst_ZDE;
      sZugBesyWait   = TRUE;/*set mask id*/
			SetMaskIdRequest(DisplayPending);
      lChk = TRUE;
    }
  }
  return(lChk);
}

e_result EnableZDRequest(const TPG_Commands * const pCmd, DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  lChk |= checkOutputArg((TPG_Commands*)pCmd);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lButtonVis = FALSE;
    isZDButtonVisible(pout, &lButtonVis);
    if((pCmd->MessageId == VIEW_LZB_PZB_DATA) && !sDriverRequest && lButtonVis)
    {
      sEVC101Request = EVC101_ZD_REQUEST;
      sDriverRequest = DrvRqst_ZD;
      sZugBesyWait   = TRUE;
      SetMaskIdRequest(DisplayPending);
      lChk = TRUE;
    }
  }/*if*/
  return(lChk);
}

e_result EnableSBVRequest(const TPG_Commands * const pCmd,
                          const TPT_SelectedTest *const pSelTest,
                          DS_CCUT_HMI_TRN *const pout)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TPG_Commands*)pCmd);
  lChk |= checkOutputArg((TPT_SelectedTest*)pSelTest);
  lChk |= checkOutputArg((DS_CCUT_HMI_TRN*)pout);
  if(TPG_SUCCEEDED(lChk))
  {
    BOOLEAN8 lButtonVis = FALSE;
		BOOLEAN8 lNoSetSBVRequest = Get_NoSetSBVRequest();
    isTestBWGButtonVisible(pout, &lButtonVis);
    if((pCmd->MessageId == TEST_SEL_BY_DRIVER) &&
        (pSelTest->SelectedTest == BRAKE_TEST) &&
        !sDriverRequest && lButtonVis && (lNoSetSBVRequest == FALSE))
        {
          sEVC101Request = EVC101_SBV_REQUEST;
          sDriverRequest = DrvRqst_SBV;
          /*sZugBesyWait   = TRUE;
					SetMaskIdRequest(TrainMenu);
					*/
        }/*if*/
  }
  return(lChk);
}

e_result EnableBlockSTMemoryRequest(const TPT_JRUCommand * const pCmd,
                                    const JRU_STATUS_IP * const pJruSts,
                                    MMI_AUX_OUT *const pMmiAuxOut)
{
  e_result lChk = TPG_SUCCESS;
  lChk |= checkOutputArg((TPT_JRUCommand*)pCmd);
  lChk |= checkOutputArg((JRU_STATUS*)pJruSts);
  lChk |= checkOutputArg((MMI_AUX_OUT*)pMmiAuxOut);
  if(TPG_SUCCEEDED(lChk))
  {
    static UINT8 sPassword[4] = {'1','2','3','4'};
    pMmiAuxOut->STMem_Stop = FALSE;
    if((memcmp(pCmd->Password, sPassword, sizeof(sPassword)) == 0) &&
       (pJruSts->STblocked == FALSE))
    {
      pMmiAuxOut->STMem_Stop = TRUE;
    }
  }/*if*/
  return(lChk);
}

e_result EnableDriverMessageAckdRequest(const TPG_Commands * const pCmd,
                                        const TPT_TextMessageApproval *const pMsg)
{
	e_result lChk = TPG_SUCCESS;
	lChk |= checkOutputArg((TPG_Commands*)pCmd);
	lChk |= checkOutputArg((TPT_TextMessageApproval*)pMsg);
	if(TPG_SUCCEEDED(lChk))
  {
		if(pCmd->MessageId == MESSAGE_ACK_REQUEST)
		{
			if(GetMaskIdRequest() == TxtMsgForConfirm) {
			/*set the request*/
			sEVC111Request = 1;
			/*set the dataof EVC 111 packet*/
			/*sEVC111TextID   = pMsg->TextId;*/
			sEVC111Approval = pMsg->Approval;
			SetMaskIdRequest(NavigationTask);
			ResetDrvRqst();
			}
		}/*if*/
  }
	return(lChk);
}

e_result EnableButtonEventReport(const TPG_Commands * const pCmd,
                                 const TPT_SelectedTest *const pTst)
{
	e_result lChk = TPG_SUCCESS;
	static BOOLEAN8 lPressed = FALSE;
	lChk |= checkOutputArg((TPG_Commands*)pCmd);
	lChk |= checkOutputArg((TPT_TextMessageApproval*)pTst);
	if(TPG_SUCCEEDED(lChk))
	{
		if((pCmd->MessageId == TEST_SEL_BY_DRIVER) &&
		   (pTst->SelectedTest == PZB_SELECTED) &&
		   (lPressed == FALSE)){
			sSTM34Request = 1;
			sSTM34QButton = 0;
			lPressed = TRUE;
		}
		else if((pCmd->MessageId == 0) &&
		        (pTst->SelectedTest == NO_SELECTED) &&
	          (lPressed == TRUE)){
			sSTM34Request = 1;
			sSTM34QButton = 1;
			lPressed = FALSE;
			SetMaskIdRequest(TrainMenu);
		}
	}/*if*/
	return(lChk);
}

void Hmi_Request_init(void)
{
	SetTime(&TimeMask, (T40S/TASK64));
}

BOOLEAN8 GetZugBesyWait(void)
{
	return(sZugBesyWait);
}

e_DrvRqst GetDrvRqst(void)
{
	return(sDriverRequest);
}

void ResetDrvRqst(void)
{
	sDriverRequest = DrvRqst_NO;
}

void SetEvc104Request(UINT8 aRequest)
{
	sEVC104Request = aRequest;
}

void SetEvc101Request(UINT8 aRequest)
{
	sEVC101Request = aRequest;
}

void SetEvc123Request(UINT8 aRequest)
{
	sEVC123Request = aRequest;
}

UINT16 GetHmiTxtMsgId(void)
{
	return(sEVC111TextID);
}

UINT8 GetHmiTxtMsgAck(void)
{
	return(sEVC111Approval);
}

UINT8 GetStm34QButton(void)
{
	return(sSTM34QButton);
}

/******* request handle *******/
static UINT8 reset_and_return(UINT8 *value)
{
  UINT8 lCurrVal = *value;
  *value = 0;
  return(lCurrVal);
}

UINT8 is_EVC101_requested(void)
{
  return(reset_and_return(&sEVC101Request));
}

UINT8 is_EVC104_requested(void)
{
  return(reset_and_return(&sEVC104Request));
}

UINT8 is_EVC111_requested(void)
{
  return(reset_and_return(&sEVC111Request));
}

UINT8 is_EVC123_requested(void)
{
  return(reset_and_return(&sEVC123Request));
}
UINT8 is_STM34_requested(void)
{
	return(reset_and_return(&sSTM34Request));
}
