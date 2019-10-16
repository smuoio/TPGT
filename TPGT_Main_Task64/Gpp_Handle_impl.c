/**
* \file Gpp_handle_impl.c
* \brief the file manages the application functions used to create the GPP packet
*        to send via MVB. The cration of this packet is managed in the
*        application task
* \author Muoio Salvatore
*/
#include "Task64_Def.h"
#include "GPP_Handle_Public.h"
#include "GPP_Handle_impl.h"
#include "Generic_api.h"
#include "StartPhase.h"
#include "SpeedHandle_Public.h"
#include "Hmi_Request_Public.h"
#include "Hmi_TrainDataEntry_Public.h"
#include "Hmi_DriverData_Public.h"
#include "Ccuo_Handle_Public.h"

/**
* \var static UINT8 sIfVer[3]
* \brief interface version to send to Ebicab
*/
static UINT8 sIfVer[3] = {0x04,0x02,0x00};
/**
* \var static UINT8 sSwVer[3]
* \brief sw version to send to Ebicab
*/
static UINT8 sSwVer[3] = {0xFF,0xFF,0xFF};
/**
* \var static UINT16 index_packet2write
* \brief index of packet that the TCMS writes
*/
static UINT16 index_packet2write = 0x0U;
/**
* \var static t_Time TimeEVC102
* \brief timer used to send EVC-102
*/
static t_Time TimeEVC_102 = {0,0};
/**
* \fn static void set_packet2write(UINT16 aMMiMPacket)
* \brief function used to set the trigger og evc to send
* \param aMMiMPacket index of packet.
*/
static void set_packet2write(UINT16 aMMiMPacket)
{
   /*write the MMi id in the bufer Pack2Write*/
  GetApplEvcSts()->Pack2Write[index_packet2write++] = aMMiMPacket;
}

void reset_index_packet2write(void)
{
  /*reset the index of packet */
  index_packet2write = 0x0U;
  memset(&(GetApplEvcSts()->Pack2Write[0]), 0, sizeof(GetApplEvcSts()->Pack2Write));
}

/*creates packet EVC-100*/
void Create_MmiMStartMmi(void)
{
  BOOLEAN8 lIsHmiAlive = FALSE;

  /*check if the HMI is alive !!!*/
  isHmiAlive(&lIsHmiAlive);
  /*hmi doesn't provide the life sign, so we use the info, cab is occupied!!!*/
  /* request from ebicab, read in the shared memory */
  /*check if the ebicab sent the signal !!!*/
  if(/*(lIsHmiAlive == TRUE) &&*/
     /*(GetStartPahseStatus() == StartPhase_End)   &&  startup phase terminated*/
     (GetEvcMmiStartReq() == VERSION_INFO_REQ) && /*version info still in request*/
     (isReceivedEVC0() == TRUE)) /*received EVC-0*/
  {
    /**
    * \brief pointer addressed to the structure in the Shared Memory
    */
    t_MmiStartMmi *lMmiMStartMmi = (t_MmiStartMmi*)&(GetApplEvcSts()->MmiStartMmi);
    if(lMmiMStartMmi != 0)
    {
      /* create data evc-100*/
      lMmiMStartMmi->MmiMStartInfo   = 0x0U; /*Version info response*/
      lMmiMStartMmi->MmiMStartStatus = 0x0U; /*MMI is OK*/
      lMmiMStartMmi->MmiMType        = 0x0U; /*ERTMS/ETCS MMI*/
      memcpy(lMmiMStartMmi->MmiMifVer, sIfVer, sizeof(sIfVer));
      memcpy(lMmiMStartMmi->MmiMSwVer, sSwVer, sizeof(sSwVer));
      /*set Vehicle identity number provided by Train Data Set Configuration file.*/
      /*set Train Data Sets not configured*/
      memset(&lMmiMStartMmi->MmiNidEngine2[0], 0,
            sizeof(lMmiMStartMmi->MmiNidEngine2));
      /*set default value, sent if MMI_NID_ENGINE_2==0
       the default value is yyyy-mm-dd*/
      lMmiMStartMmi->MmiTFixedTrainsetVDate[0] = 'y';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[1] = 'y';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[2] = 'y';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[3] = 'y';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[4] = '-';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[5] = 'm';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[6] = 'm';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[7] = '-';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[8] = 'd';
      lMmiMStartMmi->MmiTFixedTrainsetVDate[9] = 'd';
      /*set default value, sent if MMI_NID_ENGINE_2==0
       the default value is hh:mm:ss*/
      lMmiMStartMmi->MmiTFixedTrainsetVTime[0] = 'h';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[1] = 'h';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[2] = ':';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[3] = 'm';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[4] = 'm';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[5] = ':';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[6] = 's';
      lMmiMStartMmi->MmiTFixedTrainsetVTime[7] = 's';
	    /* set the id of packet to write*/
	    set_packet2write(100);
      mon_broadcast_printf("Create_MmiMStartMmi - packet[100]\n");
    }/*if*/
  }/*if*/
}

/**
* \fn static UINT8 GetTsgEtcsMmiMMode(const TSG_ETCS_MMI_DYN *pTsgEtcs)
* \brief function returns the process data value of MMI_M_Mode
* \param pTsgEtcs pointer to the object TSG_ETCS_MMI_DYN.
* \return value of MMI_M_Mode
*/
static UINT8 GetTsgEtcsMmiMMode(const TSG_ETCS_MMI_DYN *pTsgEtcs)
{
  UINT8 lOut = 0xAU;
  if(pTsgEtcs != 0)
  {
     lOut = pTsgEtcs->MMI_M_Mode;
  }/*if*/
  return(lOut);
}


/*creates packet EVC-102*/
void Create_MmiStatusReport(const TSG_ETCS_MMI_DYN *pTsgEtcs)
{
  BOOLEAN8 lIsHmiAlive     = FALSE;
  UINT8 lEvc0GoToIdle      = 0U;
  t_MmiStatusReport * lMmiStatusReport = (t_MmiStatusReport*)&(GetApplEvcSts()->MmiStatusReport);
  isHmiAlive(&lIsHmiAlive);
  if(lMmiStatusReport != 0)
  /*if(GetApplEvcSts()->evc_packet[Index_MmiStatus])*/ /*EVC-2 received!!!!*/
  {
    lEvc0GoToIdle = GetEvcMmiStartReq();
	/*if(lIsHmiAlive == TRUE)*//*HMI alive and cab occupied!!!*/
	  if((lEvc0GoToIdle == IDLE_STATE) && (lIsHmiAlive == TRUE))
    {
      /*start the timer*/
      if(UpdateTime(&TimeEVC_102))
      {
        ResetTime(&TimeEVC_102); /*reset the counter*/
        /*start the timer!!!!!*/
        /*check if the HMI is alive !!!*/
        /*if(GetEvcMmiStartReq() == VERSION_INFO_REQ)*/
        {
          /* set the active cabin */
          lMmiStatusReport->SpeedCabin.MmiMactiveCabin = GetEvcActiveCab();
		      mon_broadcast_printf("EVC 102 MmiMactiveCabin = %d\n", lMmiStatusReport->SpeedCabin.MmiMactiveCabin);
          /* set the speed status */
          lMmiStatusReport->SpeedCabin.MmiMSpeedSts    = GetSpeedStatus();
          /* set the mmi mode */
          /*TODO add the real status of leuchtmelder, Req. 4.2.12.2 leuchtmelder Mode*/
          lMmiStatusReport->CurrMmi.MmiMMode           = GetTsgEtcsMmiMMode(pTsgEtcs);
          mon_broadcast_printf("EVC 102 MmiMMode = %d\n", lMmiStatusReport->CurrMmi.MmiMMode);
          /* set the mmi status */
          lMmiStatusReport->CurrMmi.MmiMMmiStatus      = SetTcmsSts2EbiCab();
          mon_broadcast_printf("EVC 102 MmiMMmiStatus = %d\n", lMmiStatusReport->CurrMmi.MmiMMmiStatus);
          /* set the id of packet to write*/
          set_packet2write(102);
	        mon_broadcast_printf("[Packet]::VERSION_INFO_REQ->IDLE_STATE::102\n");
        }
      }
    }/*if*/
   }/*if hmi*/
}

/*creates packet EVC-101*/
void Create_MmiRequest(void)
{
  UINT8 *lMmiRequest = (UINT8*)&(GetApplEvcSts()->MmiRequest);
  if(lMmiRequest != 0)
  {
    /*check if is active one request!!!*/
    UINT8 lRequest = is_EVC101_requested();
    /* set the active cabin */
    /* plausibility of request (MMI_M_REQUEST)*/
    if(lRequest > 0)
    {
      *lMmiRequest = lRequest;
      /* set the id of packet to write*/
 	    set_packet2write(101);
    }/*if*/
  }/*if*/
}

/* create the EVC-111*/
void Create_MmiDriverMessageAck(void)
{
  /**/
  t_MmiDriverMessageAck *lMmiDriverMessageAck =
                        (t_MmiDriverMessageAck*)&(GetApplEvcSts()->MmiDriverMessageAck);
  if(lMmiDriverMessageAck != 0)
  {
    /*check the request*/
    UINT8 lRequest = is_EVC111_requested();
    if(lRequest == 1)
    {
      /*create the packet!!!*/
      lMmiDriverMessageAck->MmiIText        = GetEvc8MmiIText();/* GetHmiTxtMsgId();*//*EVC-8 I_Text*/
      lMmiDriverMessageAck->MmiqAck.MmiQAck = GetHmiTxtMsgAck();
      set_packet2write(111);
      mon_broadcast_printf("[Packet]::111\n");
    }/*if*/
  }/*if*/
}
/*creates packet EVC-123*/
void Create_MmiSpecifiStmDatatoStm(void)
{
  t_MmiSpecificStmDataToStm *lMmiSpecificStmDataToStm =
                             (t_MmiSpecificStmDataToStm*)&GetApplEvcSts()->MmiSpecificStmDataToStm;
  UINT8 lEvc123Request = is_EVC123_requested();
  if(lMmiSpecificStmDataToStm != 0)
  {
    if(lEvc123Request == 1)/*filled data*/
    {
    /*check request evc 123*/
    lMmiSpecificStmDataToStm->MmiNiStm = 9;
    lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter = 5;
    /*iteraction*/
    lMmiSpecificStmDataToStm->Evc123StmIter[0].MmiNidStm     = 9;
    lMmiSpecificStmDataToStm->Evc123StmIter[0].MmiStmNidData = 1;
    lMmiSpecificStmDataToStm->Evc123StmIter[0].MmiStmLValue  = 1;
    lMmiSpecificStmDataToStm->Evc123StmIter[0].MmiStmXValue[0]  = GetBrakeTypeAscii();

    lMmiSpecificStmDataToStm->Evc123StmIter[1].MmiNidStm     = 9;
    lMmiSpecificStmDataToStm->Evc123StmIter[1].MmiStmNidData = 2;
    lMmiSpecificStmDataToStm->Evc123StmIter[1].MmiStmLValue  = 3;
    memcpy(&lMmiSpecificStmDataToStm->Evc123StmIter[1].MmiStmXValue[0],
           GetBrakePercentageAscii(), 3);
    /*lMmiSpecificStmDataToStm->Evc123StmIter[1].MmiStmXValue[1]  = Valorein ascii;*/
    /*lMmiSpecificStmDataToStm->Evc123StmIter[1].MmiStmXValue[2]  = Valorein ascii;*/
    lMmiSpecificStmDataToStm->Evc123StmIter[2].MmiNidStm     = 9;
    lMmiSpecificStmDataToStm->Evc123StmIter[2].MmiStmNidData = 3;
    lMmiSpecificStmDataToStm->Evc123StmIter[2].MmiStmLValue  = 3;
    memcpy(&lMmiSpecificStmDataToStm->Evc123StmIter[2].MmiStmXValue[0],
           GetTrainLengthAscii(), 3);
    /*lMmiSpecificStmDataToStm->Evc123StmIter[2].MmiStmXValue[1]  = Valorein ascii;*/
    /*lMmiSpecificStmDataToStm->Evc123StmIter[2].MmiStmXValue[2]  = Valorein ascii;*/
    lMmiSpecificStmDataToStm->Evc123StmIter[3].MmiNidStm     = 9;
    lMmiSpecificStmDataToStm->Evc123StmIter[3].MmiStmNidData = 4;
    lMmiSpecificStmDataToStm->Evc123StmIter[3].MmiStmLValue  = 3;
    memcpy(&lMmiSpecificStmDataToStm->Evc123StmIter[3].MmiStmXValue[0],
           GetVmaxAscii(), 3);
    /*lMmiSpecificStmDataToStm->Evc123StmIter[3].MmiStmXValue[1]  = Valorein ascii;*/
    /*lMmiSpecificStmDataToStm->Evc123StmIter[3].MmiStmXValue[2]  = Valorein ascii;*/
    lMmiSpecificStmDataToStm->Evc123StmIter[4].MmiNidStm     = 9;
    lMmiSpecificStmDataToStm->Evc123StmIter[4].MmiStmNidData = 5;
    lMmiSpecificStmDataToStm->Evc123StmIter[4].MmiStmLValue  = 1;
    lMmiSpecificStmDataToStm->Evc123StmIter[4].MmiStmXValue[0]  = GetLastEvc25Zda();
    set_packet2write(123);
    mon_broadcast_printf("Create_MmiSpecifiStmDatatoStm.Request == 1\n");
   }/*if request*/
   else if(lEvc123Request == 2)
   {
     /*reset data*/
     lMmiSpecificStmDataToStm->uMmiStmNIter.StmNIter.Iter = 0;
     set_packet2write(123);
     mon_broadcast_printf("Create_MmiSpecifiStmDatatoStm.Request == 2\n");
   }
  }
}

/*EVC-104*/
void Create_MmiNewDriverData(void)
{
  t_MmiNewDriverData *lMmiNewDriverData =
                     (t_MmiNewDriverData*)&GetApplEvcSts()->MmiNewDriverData;
  if(lMmiNewDriverData != 0)
  {
    UINT8 lEvc104Request = is_EVC104_requested();
    if(lEvc104Request != 0)
    {
      lMmiNewDriverData->MmiNidDriver    = GetMmiNidDriver();
      lMmiNewDriverData->MmiNidOperation = GetMmiNidOperation();
      lMmiNewDriverData->MmiNidLanguage  = GetMmiNidLanguage();
      lMmiNewDriverData->MmiQDriver.Driver.Driver_Data = GetMmiQDriverDataConfirmed();
      set_packet2write(104);
      mon_broadcast_printf("Create_MmiNewDriverData\n");
    }
  }/*if pointer*/
}

/*EVC-109*/
void Create_MmiSetTimeMmi(void)
{
  t_MmiSetTime *lMmiSetTime = (t_MmiSetTime*)&GetApplEvcSts()->MmiSetTime;
  if(lMmiSetTime != 0)
  {
    UINT32 lUtcTime = GetUtcTime();
    memcpy(&lMmiSetTime->Utc, &lUtcTime, sizeof(UINT32));
    lMmiSetTime->Zone_Offset = 0U;
    set_packet2write(109);
    mon_broadcast_printf("EVC::109::Create_MmiSetTimeMmi\n");
  }
}

/***** public functions *****/
BOOLEAN8 isReceivedEVC0(void)
{
	return((GetApplEvcData()->evc_packet[index_evc_0] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedEVC2(void)
{
  return((GetApplEvcData()->evc_packet[index_evc_2] == 1) ? TRUE : FALSE);
}

static BOOLEAN8 isReceivedEVC21(void)
{
	return((GetApplEvcData()->evc_packet[index_evc_21] == 1) ? TRUE : FALSE);
}
/**
* \var static BOOLEAN8 sEbiCabAlive
* \brief variable used to compute the falling edge of mvb communication
*/
static BOOLEAN8 sEbiCabAlive = FALSE;

UINT8 GetEvcActiveCab(void)
{
  static UINT8 lOut = 0U; /*no cabin active!!!*/
  BOOLEAN8 lIsEtcsIsolated = FALSE;
  BOOLEAN8 lIsAlive        = FALSE;
  isEtcsIsolated(&lIsEtcsIsolated);
  isEbiCabAlive(&lIsAlive);
  if(FallingEdge(lIsAlive, &sEbiCabAlive) == TRUE)
	  lOut = 0;
  else {
  if(lIsEtcsIsolated == FALSE)
  {
	/*pointer to shared memory !!!*/
	if (isReceivedEVC2() == TRUE)
		/*store the last value whe the data is available*/
		lOut = GetApplEvcData()->MmiStatus.CabData.Evc2Sts.MmiMActiveCabin;
  }
  else
	  lOut = 0U;
  }
  return(lOut);
}

BOOLEAN8 isReceivedEVC8(void)
{
  return((GetApplEvcData()->evc_packet[index_evc_8] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isReceivedEVC14(void)
{
  return((GetApplEvcData()->evc_packet[index_evc_14] == 1) ? TRUE : FALSE);
}

e_MmiMStartReq GetEvcMmiStartReq(void)
{
  static e_MmiMStartReq lOut = MMI_DEFAULT;
  BOOLEAN8 lIsEtcsIsolated = FALSE;
  BOOLEAN8 lIsAlive        = FALSE;
  isEtcsIsolated(&lIsEtcsIsolated);
  isEbiCabAlive(&lIsAlive);
  if(FallingEdge(lIsAlive, &sEbiCabAlive) == TRUE)
	  lOut = MMI_DEFAULT;
  else
  {
    if(lIsEtcsIsolated == FALSE)
    {
	     /*pointer to shared memory !!!*/
	      if (isReceivedEVC0() == TRUE) /*Index_MmiStartAtp == 0*/
		        lOut = (GetApplEvcData()->MmiStartAtp.MmiMStartReq);
    }
    else
	    lOut = MMI_DEFAULT;
  }
  return(lOut);
}


UINT32 GetEVC21EnableDriverRequest(void)
{
  static UINT32 lOut = 0U;
  if(isReceivedEVC21() == TRUE) /*Index_MmiEnableDriverRequest == 9*/
  {
    /*store the last value whe the data is available*/
    lOut = GetApplEvcData()->MmiEnableDriverRequest.MmiQRequestEnable;
  }
  return(lOut);
}


UINT16 GetEvc8MmiQText(void)
{
  static UINT16 lOut = 0;
  if(isReceivedEVC8() == TRUE)
    lOut = GetApplEvcData()->MmiDriverMessage.MmiQText;
  return(lOut);
}

UINT8 GetEvc8MmiQTextCriteria(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedEVC8() == TRUE)
    lOut = (GetApplEvcData()->MmiDriverMessage.MmiQ.Text.MmimQTextCriteria);
  return(lOut);
}

UINT8 GetEvc8MmiIText(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedEVC8() == TRUE)
    lOut = (GetApplEvcData()->MmiDriverMessage.MmiIText);
  return(lOut);
}

UINT8 GetEvc8MmiXText(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedEVC8() == TRUE)
    lOut = (GetApplEvcData()->MmiDriverMessage.MmiXText[0]);
  return(lOut);
}

UINT8 GetEvc8MmiNText(void)
{
  static UINT8 lOut = 0U;
  if(isReceivedEVC8() == TRUE)
    lOut = (GetApplEvcData()->MmiDriverMessage.MmiNText);
  return(lOut);
}

UINT8 GetMmiNidLanguage(void)
{
  UINT8 lOut = 0;
  if(isReceivedEVC14() == TRUE)
  {
    lOut = GetApplEvcData()->MmiCurrentDriverData.MmiNidLanguage;
  }
  return(lOut);
}

UINT8 GetMmiQDriverData(void)
{
  UINT8 lOut = 0U;
  if(isReceivedEVC14() == TRUE)
  {
    lOut = GetApplEvcData()->MmiCurrentDriverData.MmiQDriver.Driver.Driver_Data;
  }
  return(lOut);
}

t_Time *GetTimeEvc102(void)
{
  return(&TimeEVC_102);
}
