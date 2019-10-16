/**
* \file Hmi_DriverData.c
* \brief the file manages the private functions of hmi
* \author Muoio Salvatore
*/

#include "Hmi_DriverData.h"
#include "Hmi_DriverData_Public.h"
#include "Ccuo_Handle_Public.h"
#include "Gpp_Handle_Public.h"
#include "Hmi_Request_Public.h"

/**
* \var static UINT32 sMmiNidDriver
* \brief static variable used to set the driver id number
*/
static UINT32 sMmiNidDriver                = 0x0U;
/**
* \var static UINT32 sMmiNidOperation
* \brief static variable used to set the train id number
*/
static UINT32 sMmiNidOperation             = 0x0U;
/**
* \var static UINT8  sMmiQDriverData
* \brief static variable used to set the confirmation
*/
static UINT8  sMmiQDriverData              = 0x0U;
/**
* \var static BOOLEAN8 sCloseDriverIdRequest
* \brief static variable used to check the falling edge to close the request
*/
static BOOLEAN8 sCloseDriverIdRequest      = FALSE;
/**
* \var static BOOLEAN8 sCloseTrainNumberIdRequest
* \brief static variable used to check the falling edge to close the request
*/
static BOOLEAN8 sCloseTrainNumberIdRequest = FALSE;


/**
* \fn static void SetNidDriver(UINT32 aNidDriver)
* \brief The function sets sets the field of Driver Id number
* \param aNidDriver driver id number
*/
static void SetNidDriver(UINT32 aNidDriver)
{
  sMmiNidDriver = aNidDriver;
}

/**
* \fn static void SetNidOperation(UINT32 aNidOperation)
* \brief The function sets sets the field of Train number Id
* \param aNidOperation driver id number
*/
static void SetNidOperation(UINT32 aNidOperation)
{
  /*convert the number in special coding!!!*/
  UINT32 lTmp = 0x0U;
  Uint32NumToUint32Bcd(aNidOperation, &lTmp);
  sMmiNidOperation = lTmp;
}

/**
* \fn static void SetMmiQDriverDataConfirmed(const UINT8 aValue)
* \brief The function sets sets the field MMI_Q_DRIVER_DATA
* \param aNidDriver driver id number
*/
static void SetMmiQDriverDataConfirmed(const UINT8 aValue)
{
  sMmiQDriverData = aValue;
}

/**
* \fn static UINT8 CheckEnableControlDataEntry(e_QDriverData DataEntry)
* \brief The function sets checks Data Entry Windows and Enable Control
         within those windows.
* \param DataEntry bit mask
*/
static UINT8 CheckEnableControlDataEntry(e_QDriverData DataEntry)
{
  /* read the MMI Q Driver data from the EVC 14*/
  UINT8 lMmiQDriverData = GetMmiQDriverData();
  return(((lMmiQDriverData>>DataEntry)&1));
}



BOOLEAN8 Hmi_ChangeDriverId(CCUT_CCUO *const pOut, const TPT_Uint32Valid *pInp)
{
  BOOLEAN8 lOut = FALSE;
  const UINT8 Driverid = 5;
  if((pOut != 0) && (pInp != 0))
  {
    /*UINT8 lMmiQDriverData = GetMmiQDriverData();Evc14->Mmi_Q_Driver_Data*/
    lOut = TRUE;
    if(CheckEnableControlDataEntry(ProvideChange_Window_Diver_Id))/*(((lMmiQDriverData>>2)&1))*/
    {
      if((pInp->Valid == TRUE))
      {
        pOut->CDrvIdReq  = 0;
        pOut->CDrvIdReqV = TRUE;
        /*set EVC_104.MMI_NID_DRIVER = Fahrernummer.IDriverId*/
        SetNidDriver(pInp->Value);
        /*sMmiNidDriver = pInp->Value;*/
        SetBoolean(&sCloseDriverIdRequest, TRUE);
        SetEvc104Request(EVC_104_REQUEST_SET);
        SetMmiQDriverDataConfirmed(Driverid);
        mon_broadcast_printf("Hmi_ChangeDriverId::Hmi()\n");
      }
      else if((GetCcuoDriverId() == 0) && (pInp->Valid == FALSE))
      {
        pOut->CDrvIdReq  = TRUE;
        pOut->CDrvIdReqV = TRUE;
        mon_broadcast_printf("Hmi_ChangeDriverId::GetCcuoDriverId == 0\n");
      }
    }
    else
    {

        if((GetDrvRqst() == DRVRqst_DrvId) &&
           FallingEdge(pInp->Valid, &sCloseDriverIdRequest) == TRUE)
        {
          /* set the evc 101 with cancel driver id request field*/
          SetEvc101Request(EVC101_EXIT_DRIVER_ID_REQUEST);
          /*reset request id*/
          ResetDrvRqst();
          mon_broadcast_printf("Hmi_ChangeDriverId::Close request\n");
        }
    }
  }/*if*/
  return(lOut);
}

BOOLEAN8 Hmi_ChangeTrainNumber(CCUT_CCUO *const pOut, const TPT_Uint32Valid *pInp)
{
  BOOLEAN8 lOut = FALSE;
  const UINT8 TrainNumberid = 2;
  if((pOut != 0)&& (pInp != 0))
  {
    /*UINT8 lMmiQDriverData = GetMmiQDriverData();Evc14->Mmi_Q_Driver_Data*/
    lOut = TRUE;
    if(CheckEnableControlDataEntry(Window_Train_Num_Id))/*((lMmiQDriverData>>1)&1)*/
    {
      if(pInp->Valid == TRUE)
      {
        /*ResetDrvRqst();*/
        pOut->CTrnNumReq  = FALSE;
        pOut->CTrnNumReqV = TRUE;
        /*EVC_104.MMI_NID_OPERATION = HMI_CCUT.TPT_TrainId.Value*/
        SetNidOperation(pInp->Value);
        SetMmiQDriverDataConfirmed(TrainNumberid);
        /*sMmiNidOperation = pInp->Value;*/
        SetBoolean(&sCloseTrainNumberIdRequest, TRUE);
        SetEvc104Request(EVC_104_REQUEST_SET);
        mon_broadcast_printf("Hmi_ChangeTrainNumber::Hmi\n");
      }
      else if((GetCcuoTrnNumber() == 0) && (pInp->Valid == FALSE))
      {
        pOut->CTrnNumReq  = TRUE;
        pOut->CTrnNumReqV = TRUE;
      }
    }/*if*/
    else
    {
      if((GetDrvRqst() == DRVRqst_ChnTrainNumber) &&
          FallingEdge(pInp->Valid, &sCloseTrainNumberIdRequest) == TRUE)
      {
        /* set the evc 101 with cancel train number id request field*/
        SetEvc101Request(EVC101_EXIT_TRAIN_NUMBER_REQUEST);
        /*reset request id*/
        ResetDrvRqst();
        mon_broadcast_printf("Hmi_ChangeTrainNumber::Close request\n");
      }
    }
  }/*if*/
  return(lOut);
}

UINT32 GetMmiNidDriver(void)
{
  return(sMmiNidDriver);
}

UINT32 GetMmiNidOperation(void)
{
  return(sMmiNidOperation);
}

UINT8 GetMmiQDriverDataConfirmed(void)
{
  return(sMmiQDriverData);
}

void Hmi_Handle_DriverData(CCUT_CCUO *const pOut, const DS_HMI_CCUT *pInp)
{
  if((pOut != 0) && (pInp !=0 ))
  {
    Hmi_ChangeDriverId(pOut, &pInp->TPT_DriverId);
    Hmi_ChangeTrainNumber(pOut, &pInp->TPT_TrainId);
  }
}
