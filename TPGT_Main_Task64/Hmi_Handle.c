/**
* \file Hmi_Handle.c
* \brief the file manages the application requirements of hmi.
* \author Muoio Salvatore
*/
#include "Generic_api.h"
#include "Ccuo_Handle_Public.h"
#include "Hmi_Handle.h"
#include "Hmi_Handle_impl.h"
#include "Hmi_Request.h"
#include "Hmi_Request_Public.h"
#include "Hmi_TrainDataEntry.h"
#include "Hmi_DriverData.h"
#include "Hmi_TextMessage.h"
#include "Hmi_PzbLzbTextMessage.h"
#include "Hmi_Sound.h"
#include "Hmi_EtcsIcon.h"
#include "Hmi_PzbLzbIcon.h"

/**
*\var static DS_HMI_CCUT sHmi1
*\brief static variable used for the interface function
*/
static DS_HMI_CCUT sHmi1;

void Hmi_Handle_input(const DS_HMI_CCUT *const pin)
{
  if(pin != 0)
  {
    /*copy structure*/
    memcpy(&sHmi1, pin, sizeof(DS_HMI_CCUT));
  }/*if*/
}

void Hmi_Handle_output(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  const UINT16 cBlockData = 0x18U;
  if(interface != 0)
  {
    DS_CCUT_HMI_TRN * const lCcutHmiTrn = (DS_CCUT_HMI_TRN*)(&interface->soCCUT_HMI_TRN);
    BOOLEAN8 lIsHmiAlive = FALSE;
    UINT8 lCcuoCabActive = GetCcuoCabActive();
    isHmiAlive(&lIsHmiAlive);
    /*functions to set the data to hmi*/
    if((lIsHmiAlive == TRUE) && (lCcuoCabActive == CAB_OCCUPIED)){
    BlockData(&interface->JRU_STATUS, lCcutHmiTrn);
    AtpDownAlarm(&interface->oHMI1_CCUT, lCcutHmiTrn);
    /*button train data input */
    EnableZDERequest(&interface->oHMI1_CCUT.TPT_Commands,lCcutHmiTrn);
    EnableZDRequest(&interface->oHMI1_CCUT.TPT_Commands, lCcutHmiTrn);
    EnableSBVRequest(&interface->oHMI1_CCUT.TPT_Commands,
                     &interface->oHMI1_CCUT.TPT_SelectedTest,
                     lCcutHmiTrn);
    EnableBlockSTMemoryRequest(&interface->oHMI1_CCUT.TPT_JRUCommand,
                               &interface->JRU_STATUS,
                               &interface->Mmi_Aux_Out);
    EnableNewTrainNumberRequest(&interface->oHMI1_CCUT.TPT_TrainId);
    EnableDriverIdRequest(&interface->oHMI1_CCUT.TPT_DriverId);
    EnableDriverMessageAckdRequest(&interface->oHMI1_CCUT.TPT_Commands,
	                               &interface->oHMI1_CCUT.TPT_TextMessageApproval);
    EnableButtonEventReport(&interface->oHMI1_CCUT.TPT_Commands,
                            &interface->oHMI1_CCUT.TPT_SelectedTest);
	  EnableTestPzbLzbButton(lCcutHmiTrn);
    EnableTestButton(lCcutHmiTrn);
	   /*Train Data Entry handle mask*/
	  MaskZDEHandle(&interface->oHMI1_CCUT.TPT_Commands,
	                &interface->oHMI1_CCUT.TPT_LZBPZBTrainDataInput, lCcutHmiTrn);
    MaskZDHandle(&interface->oHMI1_CCUT.TPT_Commands,
                  &interface->oHMI1_CCUT.TPT_LZBPZBTrainDataInput, lCcutHmiTrn);
    Hmi_Handle_DriverData(&interface->CCUT2CCUO, &interface->oHMI1_CCUT);
    /*check the time of mask*/
    CheckMaskRequestBusy();
    EnableDelScreen(lCcutHmiTrn);
    /*Dynamic*/
    EnableAllowedSpeed(&interface->TSG_STM_MMI_DYN, lCcutHmiTrn);
    EnableTargetSpeed(&interface->TSG_STM_MMI_DYN, lCcutHmiTrn);
    EnableTargetDistance(&interface->TSG_STM_MMI_DYN, lCcutHmiTrn);
	  Speed2Hmi(lCcutHmiTrn);
    /*text message*/
    EnableSwVersion(&lCcutHmiTrn->TPT_IVersionInfo, &interface->TSG_P3);
    Hmi_HandleTextMessage(lCcutHmiTrn);
    Hmi_PzbLzbMessage(lCcutHmiTrn);
    Hmi_Sound(lCcutHmiTrn);
    Hmi_EtcsIcon(lCcutHmiTrn, &interface->TSG_P2, &interface->TSG_ETCS_MMI_DYN);
    Hmi_PzbLzbIcon(lCcutHmiTrn);
    SetBoolean((BOOLEAN8*)&interface->KSPMem_Block,
               (interface->oHMI1_CCUT.TPT_Commands.MessageId == cBlockData));
    }
    else
    {
	    memset(lCcutHmiTrn, 0, sizeof(DS_CCUT_HMI_TRN));
      Hmi_ResetPzbLzbIcon();
      /*reset request */
      ResetDrvRqst();
    }/*else*/
  }/*if*/
}

void Hmi_Handle(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
  if(interface != 0)
  {
    Hmi_Handle_input(&interface->oHMI1_CCUT);

    Hmi_Handle_output(interface);
  }
}


void Hmi_Handle_init(void)
{
	Hmi_Request_init();
}

BOOLEAN8 GetAtpDownAlrmShow(void)
{
  return(AtpDownAlrmShow);
}
