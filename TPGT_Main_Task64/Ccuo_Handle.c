/**
* \file Ccuo_Handle.c
* \brief the file manages the application functions to get and set data from to CCUO.
* \author Muoio Salvatore
*/

#include "Ccuo_Handle.h"
#include "Generic_api.h"
#include "Etcs_public.h"
#include "Ccuo_Handle_impl.h"
#include "Ccuo_Handle_Public.h"


/**
* \var static CCUO_CCUT sCcuo
* \brief static variable used for the interface function
*/
static CCUO_CCUT sCcuo;
/**
* \var static UINT16 sMaskIdRqst
* \brief static variable used to set the number of mask id
*/
static UINT16 sMaskIdRqst = 0xFFFF;
/**
* \fn static void Ccuo_input(const CCUO_CCUT * const pCcuo)
* \brief static function used to read the data from CCUO in a static buffer
* \param pCcuo pointer to input from CCUO
* \return null
*/
static void Ccuo_input(const CCUO_CCUT * const pCcuo)
{
	if(pCcuo != 0) /*check if the pointer is valid*/
	{
		/* copy in the local buffer*/
		memcpy(&sCcuo, pCcuo, sizeof(CCUO_CCUT));
	}/*if*/
}

void Ccuo_Handle_input(const CCUO_CCUT * const pin)
{
  if(pin != 0)
  {
	/*copy the data*/
    Ccuo_input(pin);
  }
}

void Ccuo_Handle_output(TYPE_TPGT_MAIN_TASK64_IF *pout)
{
	BOOLEAN8 lIsEtcsIsolated = FALSE;
	UINT8 lM_Mode = GetETCSMode(&pout->TSG_ETCS_MMI_DYN);
	if(pout != 0)
	{
		isEtcsIsolated(&lIsEtcsIsolated);
		WheelDiam(&pout->CCUT2CCUO, &pout->TSG_P1);
		SetSpeedSelected(&pout->CCUT2CCUO, GetSpeedSelected());
		ReportBrakeTest(&pout->CCUT2CCUO, &pout->TSG_P0);
		EnbaleMask2Vso(&pout->CCUT2CCUO, &sMaskIdRqst);
		/* update the diagnostic information*/
		UpdateDiagEvent(&pout->CCUT2CCUO);
		/*copy data to VCE*/
		pout->Vce_Tract_1.LifeSign = GetCcuoLifeSign();
		pout->Vce_Tract_2.LifeSign = GetCcuoLifeSign();
		/*copy data to GPS*/
		pout->Vce_Gps.GPS_latitude  = GetCcuoGpsLat();
		pout->Vce_Gps.GPS_longitude = GetCcuoGpsLon();
		pout->Vce_Gps.GPS_altitude  = GetCcuoGpsAlt();
		pout->Vce_Gps.GPS_speed     = GetCcuoGpsSpeed();
		pout->Vce_Gps.GPS_direction = GetCcuoGpsDir();
		pout->Vce_Gps.GPS_satelites = GetCcuoGpsSat();
		/*copy data to AUX*/
		pout->Vce_Aux_Out.ETCS_IS   = lIsEtcsIsolated;
		pout->Mmi_Aux_Out.ETCS_IS   = lIsEtcsIsolated;
		SetTime2EbiCab(&pout->Vce_Aux_Out, &pout->Mmi_Aux_Out, lM_Mode);
		CesaTestFeedback(&pout->CCUT2CCUO);

	}
}

void Ccuo_Handle_Init(void)
{
	/*used a time of 10 seconds*/
	SetTime(GetTimeMaskId(), (T10S/TASK64));
}

void SetMaskIdRequest(e_MaskNumId aMaskIdRqst)
{
	sMaskIdRqst = aMaskIdRqst;
}

UINT16 GetMaskIdRequest(void)
{
	return(sMaskIdRqst);
}

/***** interface *****/
UINT16 GetCcuoLifeSign(void)
{
	return(sCcuo.ICcuoLifesign);
}

UINT8 GetCcuoCabActive(void)
{
  UINT8 lCabActive     = sCcuo.ICabAct;
  BOOLEAN8 lCabActiveV = sCcuo.ICabActV;
  return((lCabActiveV == TRUE) ? lCabActive : 0);
}

BOOLEAN8 GetCcuoEbiCabOn(void)
{
	BOOLEAN8 lEbiPwr  = sCcuo.IEbiPwr;
	BOOLEAN8 lEbiPwrV = sCcuo.IEbiPwrV;
	return((lEbiPwrV == TRUE) ? lEbiPwr : TRUE);
}

UINT8 GetCcuoHmiAlive(void)
{
	UINT8 lHmiAlive     = sCcuo.IHmiAlive;
  BOOLEAN8 lHmiAliveV = sCcuo.IHmiAliveV;
  return((lHmiAliveV == TRUE) ? lHmiAlive : 0);
}

UINT8 GetCcuoEtcsIsolated(void)
{
	static UINT8 lEtcDisturbAct;
  BOOLEAN8 lEtcDisturbActV = sCcuo.IEtcDisturbActV;
  return((lEtcDisturbActV == TRUE) ?
	       (lEtcDisturbAct = sCcuo.IEtcDisturbAct):
				 lEtcDisturbAct);
}

UINT8 GetCcuoLzbIsolated(void)
{
	UINT8 lLzbDisturbAct     = sCcuo.ILzbDisturbAct;
  BOOLEAN8 lLzbDisturbActV = sCcuo.ILzbDisturbActV;
  return((lLzbDisturbActV == TRUE) ? lLzbDisturbAct : 0);
}

UINT8 GetCcuoPzbIsolated(void)
{
	UINT8 lPzbDisturbAct     = sCcuo.IPzbDisturbAct;
  BOOLEAN8 lPzbDisturbActV = sCcuo.IPzbDisturbActV;
  return((lPzbDisturbActV == TRUE) ? lPzbDisturbAct : 0);
}

e_MaskMfdId GetCcuoHmiMfd(void)
{
	UINT8 lHmiMfd     = sCcuo.IHmiMfd;
  BOOLEAN8 lHmiMfdV = sCcuo.IHmiMfdV;
  return((lHmiMfdV == TRUE) ? lHmiMfd : 0);
}

BOOLEAN8 GetCcuoTractAct(void)
{
	BOOLEAN8 lTracAct  = sCcuo.ITracAct;
	BOOLEAN8 lTracActV = sCcuo.ITracActV;
	return((lTracActV == TRUE) ? lTracAct : TRUE);
}

BOOLEAN8 GetCcuoPrkBrkAct(void)
{
	static BOOLEAN8 lPrkBrkAct = FALSE;
	BOOLEAN8 lPrkBrkActV = sCcuo.IPrkBrkActV;
	return((lPrkBrkActV == TRUE) ?
	       (lPrkBrkAct  = sCcuo.IPrkBrkAct):
				 lPrkBrkAct);
}

BOOLEAN8 GetCcuoSleepN(void)
{
	BOOLEAN8 lSleepN = sCcuo.ISleepN;
	BOOLEAN8 lSleepNV = sCcuo.ISleepNV;
	return((lSleepNV == TRUE) ? lSleepN : FALSE);
}

BOOLEAN8 GetCcuoSleepA(void)
{
	BOOLEAN8 lSleepA  = sCcuo.ISleepA;
	BOOLEAN8 lSleepAV = sCcuo.ISleepAV;
	return((lSleepAV == TRUE) ? lSleepA : TRUE);
}

BOOLEAN8 GetCcuoSifaVlv1(void)
{
	BOOLEAN8 lValLck  = sCcuo.IValLck1;
	BOOLEAN8 lValLckV = sCcuo.IValLck1V;
	return((lValLckV == TRUE) ? lValLck : TRUE);
}

BOOLEAN8 GetCcuoSifaVlv2(void)
{
	BOOLEAN8 lValLck  = sCcuo.IValLck2;
	BOOLEAN8 lValLckV = sCcuo.IValLck2V;
	return((lValLckV == TRUE) ? lValLck : TRUE);
}

BOOLEAN8 GetCcuoBrkTestPoss(void)
{
	BOOLEAN8 lBrkTestPoss  = sCcuo.IBrkTestPoss;
	BOOLEAN8 lBrkTestPossV = sCcuo.IBrkTestPossV;
	return((lBrkTestPossV == TRUE) ? lBrkTestPoss : FALSE);
}

UINT16 GetCcuoBrPipePressure(void)
{
	UINT16 lBrPiPePress     = sCcuo.IBrPipePressure;
	BOOLEAN8 lBrPiPePressV = sCcuo.IBrPipePressureV;
	if(lBrPiPePressV == TRUE)
	{
		if(lBrPiPePress > 10)
			lBrPiPePress = 10;
	}
	else
		lBrPiPePress = 0xFFFF;
	return(lBrPiPePress);
}

BOOLEAN8 GetCcuoAllPntDwn(void)
{
	static BOOLEAN8 lAllPntDwn = FALSE;
	BOOLEAN8 lAllPntDwnV = sCcuo.IAllPantoDwnV;
	return ((lAllPntDwnV == TRUE) ?
		      (lAllPntDwn = sCcuo.IAllPantoDwn) :
				  lAllPntDwn);
}

UINT32 GetCcuoTrnNumber(void)
{
	UINT32   lTrnNumber = sCcuo.IDefTrnNum;
	BOOLEAN8 lDefTrnNumV = sCcuo.IDefTrnNumV;
	return((lDefTrnNumV == TRUE) ? lTrnNumber : 0);
}

/**
* \fn static UINT32 create_tail(void)
* \brief static function creates the tail number used to compute the vehicle number
*        to send to the ebicab. the function uses some digit of the
*        vehicle id
* \return tail number
*/
static UINT32 create_tail(void)
{
	UINT32 lNum = ((sCcuo.IDigit09*100) + (sCcuo.IDigit10*10) + (sCcuo.IDigit11));
	return(lNum);
}

UINT32 CreateVehicleTypeNumber(void)
{
	UINT32 lVehicleType = 668000U;
	BOOLEAN8 lUicNumV   = sCcuo.IUicNumV;
	lVehicleType = lVehicleType + create_tail();
	return((lUicNumV == TRUE) ? lVehicleType : 0);
}

UINT32 GetCcuoDriverId(void)
{
	UINT32 lDriverId = sCcuo.IDriverId;
	BOOLEAN8 lDriverIdV = sCcuo.IDriverIdV;
	return((lDriverIdV == TRUE) ? lDriverId : 0);

}

BOOLEAN8 GetCcuoDirFwN(void)
{
	BOOLEAN8 lDirFwN  = sCcuo.IDirFwN;
	BOOLEAN8 lDirFwNV = sCcuo.IDirFwNV;
	return((lDirFwNV == TRUE) ? lDirFwN : FALSE);
}

BOOLEAN8 GetCcuoDirFwA(void)
{
	BOOLEAN8 lDirFwA  = sCcuo.IDirFwA;
	BOOLEAN8 lDirFwAV = sCcuo.IDirFwAV;
	return((lDirFwAV == TRUE) ? lDirFwA : FALSE);
}

BOOLEAN8 GetCcuoDirBwN(void)
{
	BOOLEAN8 lDirBwN  = sCcuo.IDirBwN;
	BOOLEAN8 lDirBwNV = sCcuo.IDirBwNV;
	return((lDirBwNV == TRUE) ? lDirBwN : FALSE);
}

BOOLEAN8 GetCcuoDirBwA(void)
{
	BOOLEAN8 lDirBwA  = sCcuo.IDirBwA;
	BOOLEAN8 lDirBwAV = sCcuo.IDirBwAV;
	return((lDirBwAV == TRUE) ? lDirBwA : FALSE);
}

BOOLEAN8 GetCcuoStandStill(void)
{
	BOOLEAN8 lStandStillV = sCcuo.IStandStillV;
	BOOLEAN8 lStandStill  = sCcuo.IStandStill;
	return((lStandStillV == TRUE) ? lStandStill : TRUE);
}

UINT16 GetCcuoSpeed(void)
{
	BOOLEAN8 lVelocityV = sCcuo.IVelocityV;
	UINT16 lVelocity    = sCcuo.IVelocity;
	return((lVelocityV == TRUE) ? lVelocity : 0xFFFF);
}

BOOLEAN8 GetUTCTimeV(void)
{
	BOOLEAN8 lUtcTimeV  = sCcuo.IUtcTimeV;
	BOOLEAN8 lUtcOffV   = sCcuo.IUtcOffsV;
	BOOLEAN8 lDaySavTiV = sCcuo.IDaySavTiV;
	return ((lUtcTimeV & lUtcOffV & lDaySavTiV) ? TRUE : FALSE);
}

MWT_REAL GetCcuoGpsLat(void)
{
	BOOLEAN8 lGpsLatV = sCcuo.IGpsLatV;
	static MWT_REAL lGpsLat = 0;
	return((lGpsLatV == TRUE) ?
		     (lGpsLat = sCcuo.IGpsLat) :
				 lGpsLat);
}

MWT_REAL GetCcuoGpsLon(void)
{
	BOOLEAN8 lGpsLonV = sCcuo.IGpsLonV;
	static MWT_REAL lGpsLon = 0;
	if(lGpsLonV == TRUE)
		lGpsLon = sCcuo.IGpsLon;
	return(lGpsLon);
}

MWT_REAL GetCcuoGpsSpeed(void)
{
	BOOLEAN8 lGpsSpeedV = sCcuo.IGpsSpeedV;
	static MWT_REAL lGpsSpeed = 0;
	if(lGpsSpeedV == TRUE)
		lGpsSpeed = sCcuo.IGpsSpeed;
	return(lGpsSpeed);
}

MWT_REAL GetCcuoGpsAlt(void)
{
	BOOLEAN8 lGpsAltV = sCcuo.IGpsAltV;
	static MWT_REAL lGpsAlt = 0;
	if(lGpsAltV == TRUE)
		lGpsAlt = sCcuo.IGpsAlt;
	return(lGpsAlt);
}

MWT_REAL GetCcuoGpsDir(void)
{
	BOOLEAN8 lGpsDirV = sCcuo.IGpsDirV;
	static MWT_REAL lGpsDir = 0;
	if(lGpsDirV == TRUE)
		lGpsDir = sCcuo.IGpsDir;
	return(lGpsDir);
}

MWT_REAL GetCcuoGpsSat(void)
{
	BOOLEAN8 lGpsNumSatsV = sCcuo.IGpsNumSatsV;
	static MWT_REAL lGpsNumSats = 0;
	if(lGpsNumSatsV == TRUE)
		lGpsNumSats = sCcuo.IGpsNumSats;
	return(lGpsNumSats);
}

UINT32 GetUtcTime(void)
{
  return((UINT32)(sCcuo.IUtcTime));
}

UINT8 GetCcuoKeyAct(void)
{
	UINT8 lCmdKeyAct = sCcuo.ICmdKeyAct;
	BOOLEAN8 lCmdKeyActV = sCcuo.ICmdKeyActV;
	return((lCmdKeyActV == TRUE) ? lCmdKeyAct : 0);
}

BOOLEAN8 GetCcuoKeyFreeN(void)
{
	BOOLEAN8 lFreeKeyActN  = sCcuo.IFreeKeyActN;
	BOOLEAN8 lFreeKeyActNV = sCcuo.IFreeKeyActNV;
	return((lFreeKeyActNV == TRUE) ? lFreeKeyActN : FALSE);
}

BOOLEAN8 GetCcuoKeyFreeA(void)
{
	BOOLEAN8 lFreeKeyActA  = sCcuo.IFreeKeyActA;
	BOOLEAN8 lFreeKeyActAV = sCcuo.IFreeKeyActAV;
	return((lFreeKeyActAV == TRUE) ? lFreeKeyActA : FALSE);
}

BOOLEAN8 GetCcuoLzbNotKeyActN(void)
{
	BOOLEAN8 lLzbNotKeyActN  = sCcuo.ILzbNotKeyActN;
	BOOLEAN8 lLzbNotKeyActNV = sCcuo.ILzbNotKeyActNV;
	return((lLzbNotKeyActNV == TRUE) ? lLzbNotKeyActN : FALSE);
}

BOOLEAN8 GetCcuoLzbNotKeyActA(void)
{
	BOOLEAN8 lLzbNotKeyActA  = sCcuo.ILzbNotKeyActA;
	BOOLEAN8 lLzbNotKeyActAV = sCcuo.ILzbNotKeyActAV;
	return((lLzbNotKeyActAV == TRUE) ? lLzbNotKeyActA : FALSE);
}

UINT8 GetCcuoVigKeyAct(void)
{
	UINT8 lVigiKeyAct     = sCcuo.IVigiKeyAct;
	BOOLEAN8 lVigiKeyActV = sCcuo.IVigiKeyActV;
	return((lVigiKeyActV == TRUE) ? lVigiKeyAct : 0);
}

UINT8 GetCcuoIntegrityTest(void)
{
	BOOLEAN8 lIntTestV = sCcuo.IIntTestComV;
	BOOLEAN8 lIntTest  = sCcuo.IIntTestCom;
	return((lIntTestV == TRUE) ? lIntTest : FALSE);
}

BOOLEAN8 GetDaySavTi(void)
{
	BOOLEAN8 lDaySavTiV = sCcuo.IDaySavTiV;
	BOOLEAN8 lDaySavTi  = sCcuo.IDaySavTi;
	return((lDaySavTiV == TRUE) ? lDaySavTi : FALSE);
}

INT8 GetUtcOffs(void)
{
	BOOLEAN8 lUtcOffsV = sCcuo.IUtcOffsV;
	INT8     lUtcOffs  = sCcuo.IUtcOffs;
	return((lUtcOffsV == TRUE) ? lUtcOffs : 0);
}
