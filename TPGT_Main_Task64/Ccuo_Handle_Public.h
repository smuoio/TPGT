/**
* \file Ccuo_Handle_Public.h
* \brief the file manages the public functions to get and set data from to CCUO.
* \author Muoio Salvatore
*/

#ifndef __CCUO_HANDLE_PUBLIC_H__
#define __CCUO_HANDLE_PUBLIC_H__
/**
* \enum e_MaskNumId
* \brief mask id number
*/
typedef enum
{
  BasicMask          = 50,     /*!< BasicMask*/
  TrainDataOneColumn = 53,     /*!< TrainDataOneColumn*/
  TrainDataInput     = 54,     /*!< TrainDataInput*/
  TrainDataStatic    = 55,     /*!< TrainDataStatic*/
  TrainDataFinal     = 65,     /*!< TrainDataFinal*/
  TrainDataChkScreen = 66,     /*!< TrainDataChkScreen*/
  DisplayPending     = 67,     /*!< DisplayPending*/
  TxtMsgForConfirm   = 68,     /*!< TxtMsgForConfirm*/
  TrainMenu          = 69,     /*!< TrainMenu*/
  MainMenu           = 70,     /*!< MainMenu*/
  TrainZugBesy       = 77,     /*!< TrainZugBesy*/
  NavigationTask     = 0xFFFF  /*!< free Navigation*/
}e_MaskNumId;

/**
* \enum e_MaskMfdId
* \brief MFD mask id number
*/typedef enum
{
  GrundBild = 50, /*!< GrundBild mask*/
  CheckMenu = 76, /*!< CheckMenu mask*/
}e_MaskMfdId;

/**
* \enum e_CcuoCabActive
* \brief Cab status 
*/typedef enum
{
  CAB_UNKNOWN,      /*!< cab status unknown*/
  CAB_OCCUPIED,     /*!< cab status occupied*/
  CAB_NOT_OCCUPIED, /*!< cab status not occupied*/
}e_CcuoCabActive;

/**
* \fn void SetMaskIdRequest(e_MaskNumId aMaskIdRqst);
* \brief the function sets the request of maskid to VSO
*/
void SetMaskIdRequest(e_MaskNumId aMaskIdRqst);
/**
* \fn UINT16 GetMaskIdRequest(void)
* \brief the function return the request set of maskid to VSO
*/
UINT16 GetMaskIdRequest(void);
/**
* \fn UINT8 GetCcuoCabActive(void)
* \brief the function return the status of cab active from CCUO
* \return the value of variable if thevalidity is true otherwise 0
*/UINT8 GetCcuoCabActive(void);
/**
* \fn BOOLEAN8 GetCcuoEbiCabOn(void)
* \brief the function return the status of EbiCab is powered
* \return the value FALSE is On and TRUE if is not On
*/
BOOLEAN8 GetCcuoEbiCabOn(void);
/**
* \fn UINT8 GetCcuoHmiAlive(void)
* \brief the function return the status of Hmi1 and Hmi2
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT8 GetCcuoHmiAlive(void);
/**
* \fn UINT8 GetCcuoEtcsIsolated(void)
* \brief the function return the status of disturb switch
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT8 GetCcuoEtcsIsolated(void);
/**
* \fn UINT8 GetCcuoLzbIsolated(void)
* \brief the function return the status of disturb switch
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT8 GetCcuoLzbIsolated(void);
/**
* \fn UINT8 GetCcuoPzbIsolated(void)
* \brief the function return the status of disturb switch
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT8 GetCcuoPzbIsolated(void);
/**
* \fn BOOLEAN8 GetCcuoTractAct(void)
* \brief the function return the status of Traction
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoTractAct(void);
/**
* \fn BOOLEAN8 GetCcuoPrkBrkAct(void)
* \brief the function return the status of Parking Brake
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoPrkBrkAct(void);
/**
* \fn BOOLEAN8 GetCcuoSleepN(void)
* \brief the function return the status of Sleeping Status
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoSleepN(void);
/**
* \fn BOOLEAN8 GetCcuoSleepA(void)
* \brief the function return the status of Sleeping Antivalent
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoSleepA(void);
/**
* \fn BOOLEAN8 GetCcuoSifaVlv1(void)
* \brief the function return the status of Sifa Valve 1
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoSifaVlv1(void);
/**
* \fn BOOLEAN8 GetCcuoSifaVlv2(void)
* \brief the function return the status of Sifa Valve 2
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoSifaVlv2(void);
/**
* \fn BOOLEAN8 GetCcuoBrkTestPoss(void)
* \brief the function return the status of Brake Test
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoBrkTestPoss(void);
/**
* \fn UINT16 GetCcuoBrPipePressure(void)
* \brief the function return the value of Brake Pipe Pressure
* \return the value of variable if thevalidity is true otherwise 0xFF
*/
UINT16 GetCcuoBrPipePressure(void);
/**
* \fn BOOLEAN8 GetCcuoAllPntDwn(void)
* \brief the function return the status of all pantographs
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoAllPntDwn(void);
/**
* \fn UINT32 GetCcuoTrnNumber(void)
* \brief the function return the train number
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT32 GetCcuoTrnNumber(void);
/**
* \fn UINT32 CreateVehicleTypeNumber(void);
* \brief the function creates the vehicle type number
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT32 CreateVehicleTypeNumber(void);
/**
* \fn UINT32 GetCcuoDriverId(void)
* \brief the function return the drive id number
* \return the value of variable if thevalidity is true otherwise 0
*/
UINT32 GetCcuoDriverId(void);
/**
* \fn BOOLEAN8 GetCcuoDirFwN(void)
* \brief the function return the status of forward direction N
* \return the value of variable if thevalidity is true otherwise 0
*/
BOOLEAN8 GetCcuoDirFwN(void);
/**
* \fn BOOLEAN8 GetCcuoDirFwA(void)
* \brief the function return the status of forward direction A
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoDirFwA(void);
/**
* \fn BOOLEAN8 GetCcuoDirBwN(void)
* \brief the function return the status of backward direction N
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoDirBwN(void);
/**
* \fn BOOLEAN8 GetCcuoDirBwA(void)
* \brief the function return the status of backward direction A
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoDirBwA(void);

e_MaskMfdId GetCcuoHmiMfd(void);
/**
* \fn BOOLEAN8 GetCcuoStandStill(void)
* \brief the function return the status of stand Still
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoStandStill(void);
/**
* \fn UINT16 GetCcuoSpeed(void);
* \brief the function return the speed
* \return the value of variable if the validity is true otherwise 0
*/
UINT16 GetCcuoSpeed(void);
/**
* \fn BOOLEAN8 GetUTCTimeV(void)
* \brief the function return the status of validity ofIUtcTimeV, IUtcOffsV,
 *       and IDaySavTiV;
* \return the value of variable if the validity is true otherwise FALSE
*/
BOOLEAN8 GetUTCTimeV(void);
/**
* \fn UINT32 GetUtcTime(void)
* \brief the function return the the UTC time. This function is used with
*        the result of GetUTCTimeV
* \return the value of UTC time or the default value
*/
UINT32 GetUtcTime(void);
/**
* \fn BOOLEAN8 GetDaySavTi(void)
* \brief the function return the status of IDaySavTi
* \return the value of variable if the validity is true otherwise FALSE
*/
BOOLEAN8 GetDaySavTi(void);
/**
* \fn INT8 GetUtcOffs(void)
* \brief the function return the status of IUtcOffs
* \return the value of variable if the validity is true otherwise FALSE
*/
INT8 GetUtcOffs(void);
/**
* \fn MWT_REAL GetCcuoGpsLat(void);
* \brief the function return the gps lat
* \return the value the actual value or the last
*/
MWT_REAL GetCcuoGpsLat(void);
/**
* \fn MWT_REAL GetCcuoGpsLon(void);
* \brief the function return the gps lon.
* \return the value the actual value or the last
*/
MWT_REAL GetCcuoGpsLon(void);
/**
* \fn MWT_REAL GetCcuoGpsSpeed(void);
* \brief the function return the gps speed.
* \return the value the actual value or the last
*/
MWT_REAL GetCcuoGpsSpeed(void);
/**
* \fn MWT_REAL GetCcuoGpsAlt(void);
* \brief the function return the gps alt.
* \return the value the actual value or the last
*/
MWT_REAL GetCcuoGpsAlt(void);
/**
* \fn MWT_REAL GetCcuoGpsDir(void);
* \brief the function return the gps dir.
* \return the value the actual value or the last
*/
MWT_REAL GetCcuoGpsDir(void);
/**
* \fn MWT_REAL GetCcuoGpsSat(void);
* \brief the function return the gps sat.
* \return the value the actual value or the last
*/
MWT_REAL GetCcuoGpsSat(void);
/**
* \fn UINT8 GetCcuoKeyAct(void);
* \brief the function return the Activation Status of Command key
* \return the value of variable if the validity is true otherwise 0
*/
UINT8 GetCcuoKeyAct(void);
/**
* \fn BOOLEAN8 GetCcuoKeyFreeN(void);
* \brief the function return the Activation Status of Freitaste key
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoKeyFreeN(void);
/**
* \fn BOOLEAN8 GetCcuoKeyFreeA(void);
* \brief the function return the Antivalent Activation Status of Freitaste key
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoKeyFreeA(void);
/**
* \fn BOOLEAN8 GetCcuoLzbNotKeyActN(void);
* \brief the function return the Activation Status of Nothalt key
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoLzbNotKeyActN(void);
/**
* \fn BOOLEAN8 GetCcuoLzbNotKeyActA(void);
* \brief the function return the Antivalent Activation Status of Nothalt key
* \return the value of variable if the validity is true otherwise 0
*/
BOOLEAN8 GetCcuoLzbNotKeyActA(void);
/**
* \fn UINT8 GetCcuoVigKeyAct(void);
* \brief the function return the Activation Status of Wachsamtaste
* \return the value of variable if the validity is true otherwise 0
*/
UINT8 GetCcuoVigKeyAct(void);
/**
* \fn UINT8 GetCcuoIntegrityTest(void);
* \brief the function return the Activation Status of Integrity tets
* \return the value of variable if the validity is true otherwise 0
*/
UINT8 GetCcuoIntegrityTest(void);
/**
* \fn UINT8 GetCcuoLifeSign(void);
* \brief the function return the lifesign counter
* \return the value of variable if the validity is true otherwise 0
*/
UINT16 GetCcuoLifeSign(void);



#endif
