/**
* \file SpeedHandle_impl.c
* \brief the file manages the private functions used in the speed component
* \author Muoio Salvatore
*/

#include "Comm_Util.h"
#include "SpeedHandle_impl.h"
#include "SpeedHandle_Public.h"
#include "Ccuo_Handle_Public.h"


/**
* \var static t_Time TimeSpeed
* \brief timer used to detect the error
*/
static t_Time TimeSpeed = {0,0};
/**
* \var static t_Time TimeTsgSpeed
* \brief timer used to detect the TSG error
*/
static t_Time TimeTsgSpeed = {0,0};
/**
* \var static t_Time TimeMmiSpeed
* \brief timer used to detect the MMI error
*/
static t_Time TimeMmiSpeed = {0,0};
/**
* \struct t_SpeedRange
* \brief structure used to set low anh high limit
*/
typedef struct
{
    INT16 Low;  /*!< Low range*/
    INT16 High; /*!< High range*/
}t_SpeedRange;
/**
* \var static t_SpeedLimit SpeedLimit
* \brief static variable used to set the limit of range
*/
static t_SpeedRange SpeedRange;
/**
* \var static t_SpeedSelected SpeedSelected
* \brief object to public structure
*/
static t_SpeedSelected SpeedSelected = {0x0, FALSE, 0x0};

t_SpeedSelected *GetSpeedSelected(void)
{
  return(&SpeedSelected);
}

t_Time *GetTimeSpeed(void)
{
  return(&TimeSpeed);
}

t_Time *GetTimeTSGSpeed(void)
{
  return(&TimeTsgSpeed);
}

t_Time *GetTimeMMISpeed(void)
{
  return(&TimeMmiSpeed);
}

UINT8 GetSpeedStatus(void)
{
  return(SpeedSelected.SpeedStatus);
}
/**
* \fn static BOOLEAN8 SpeedPlausibilityCheck(INT16 aSpeed1, INT16 aSpeed2)
* \brief The function implements the plausibility check between two speeds,
*        the function checks if the difference is greater than 5 km/h.
*        The value is in absolute value
* \param aSpeed1 speed 1
* \param aSpeed2 speed2
* \return TRUE if the difference is greater than 5Km7, false viceversa.
*/
static BOOLEAN8 SpeedPlausibilityCheck(INT16 aSpeed1, INT16 aSpeed2)
{
  const int DiffSpeed = 139;/* 5km/h in cm/s*/
  return((abs(aSpeed1 - aSpeed2) >= DiffSpeed) ? FALSE : TRUE);
}
/**
* \fn static BOOLEAN8 isSpeedValid(INT16 aSpeed)
* \brief The function implements the plausibility check of speed
* \param aSpeed speed
* \return TRUE if the speed is valid, false viceversa.
*/
static BOOLEAN8 isSpeedValid(INT16 aSpeed)
{
  return (((aSpeed >= SpeedRange.Low) && (aSpeed <= SpeedRange.High)) ? TRUE : FALSE);
}
/**
* \fn static void SetRange(INT16 aLow, INT16 aHigh)
* \brief The functionset the range of speed
* \param aLow low range of speed
* \param aHigh high range of speed
*/
static void SetRange(INT16 aLow, INT16 aHigh)
{
  SpeedRange.Low  = aLow;
  SpeedRange.High = aHigh;
}
/**
* \fn static void SetSpeed(UINT16 Speed)
* \brief The function set field of SpeedSelected structure, speed value and
*        validity value
* \param Speed speed value
* \param SrcSpeed which device manages the speed, 1 is EbiCab, 2 is Dbc
*/
static void SetSpeed(UINT16 Speed, UINT8 SrcSpeed)
{
  GetSpeedSelected()->Validity = FALSE;
  if(Speed != ACTUAL_SPEED_NOT_AVAILABLE)
  {
    GetSpeedSelected()->Speed    = (INT16)Speed;
    if(SrcSpeed == SRC_SPEED_EBICAB)
      GetSpeedSelected()->Validity = TRUE;
  }/*if*/
}

BOOLEAN8 isSpeedCrcValid(INT16 Speed, UINT8 Life, UINT8 const CrcSpeed)
{
  UINT8 data[3];
  UINT8 lCrc;
  data[0] = (Speed>>8);
  data[1] = (Speed & (0x00FF));
  data[2] = Life;
  /*create the local crc*/
  lCrc = CrcNative(data, 3);
  /* plausibility check of Crc */
  return((lCrc == CrcSpeed) ? TRUE : FALSE);
}

BOOLEAN8 isTSGSpeedValid(const TSG_P0 *const pTSG_P0, BOOLEAN8 *isTsgSpeedValid)
{
  BOOLEAN8 lOut = FALSE; /*used for futuire debug*/
  if((isTsgSpeedValid != 0) && (pTSG_P0 != 0))
  {
    lOut = TRUE;
    SetRange(-32768, 32767);
    /*check if the speed is in the range*/
    /*if(isSpeedValid(pTSG_P0->Vreal) == TRUE)*/
    if((CheckConn(GetBusLife(P0)) == CONNECTED)      && /*dataset P0 Connected*/
       (isSpeedCrcValid(pTSG_P0->Vreal,
                        pTSG_P0->LifeSign,
                        pTSG_P0->CRC_Vreal) == TRUE) && /*Speed CRC valid*/
        (pTSG_P0->ValidityVreal == TRUE)             && /*SpeedValidity valid*/
        (isSpeedValid(pTSG_P0->Vreal) == TRUE))
    {
          *isTsgSpeedValid = TRUE;
          ResetTime(&TimeTsgSpeed);
    }
    else
    {
      /*check time out elapsed*/
      if(UpdateTime(&TimeTsgSpeed))
        *isTsgSpeedValid = FALSE;
    }
  }/*if*/
  return(lOut);
}

BOOLEAN8 isMMISpeedValid(const TSG_ETCS_MMI_DYN *const pTSG_ETCS_MMI_DYN, BOOLEAN8 *isMmiSpeedValid)
{
  BOOLEAN8 lOut = FALSE; /*used for futuire debug*/
  if((isMmiSpeedValid != 0) && (pTSG_ETCS_MMI_DYN != 0))
  {
    lOut = TRUE;
    SetRange(0, 16666);
    /*dataset ETCS Connected*/
    if((CheckConn(GetBusLife(ETCS)) == CONNECTED)                    &&
       /*Speed CRC valid*/
       (isSpeedCrcValid(pTSG_ETCS_MMI_DYN->MMI_V_Train,
                        pTSG_ETCS_MMI_DYN->MMI_M_Counter,
                        pTSG_ETCS_MMI_DYN->MMI_CRC_V_Train) == TRUE) &&
       /*check if the speed is in the range*/
       (isSpeedValid(pTSG_ETCS_MMI_DYN->MMI_V_Train) == TRUE))
    {
        *isMmiSpeedValid = TRUE;
        ResetTime(&TimeMmiSpeed);
    }
    else
    {
      /*check time out elapsed*/
      if(UpdateTime(&TimeMmiSpeed))
        *isMmiSpeedValid = FALSE;
    }
  }/*if*/
  return(lOut);
}

void SelectSpeed(BOOLEAN8 isTsgSpeedValid, BOOLEAN8 isMmiSpeedValid,
                 INT16 aVReal, INT16 aMmiVTrn)
{
    INT16 lVReal = abs(aVReal);
    /* Set the default value*/
    SetSpeed(ACTUAL_SPEED_NOT_AVAILABLE, NO_SRC_SPEED);
    /* select the speed to use!!!!*/
    if(GetSpeedStatus() == NO_ERROR_SPEED) /*no speed data error*/
    {
      if(isTsgSpeedValid & ((isMmiSpeedValid &
         isGreaterThan(lVReal, aMmiVTrn)) | ~isMmiSpeedValid))
      {
        /* select TSG speed !!!!*/
        SetSpeed(Convertcms2dmh(lVReal), SRC_SPEED_EBICAB);
      }/*if*/
      else if(isMmiSpeedValid & ((isTsgSpeedValid &
              isGreaterThan(aMmiVTrn, lVReal)) | ~isTsgSpeedValid))
      {
        /* select MMI Speed !!!!*/
        SetSpeed(Convertcms2dmh(aMmiVTrn), SRC_SPEED_EBICAB);
      }/*elseif*/
    }
	  else
	  {
		    /*TODO set DBC speed*/
		    SetSpeed(GetCcuoSpeed(), SRC_SPEED_DBC);
	  }/*else*/
}

/**
* \fn static void SetSpeedStatus(UINT8 aSpeedStatus)
* \brief The function write the speed status in the field of t_SpeedSelected
*        strcuture
* \param aSpeedStatus value of feedback, 0 default, 1 error
*/
static void SetSpeedStatus(UINT8 aSpeedStatus)
{
  GetSpeedSelected()->SpeedStatus = aSpeedStatus;
}

BOOLEAN8 CheckSpeedError(const TSG_P0 *const pTSG_P0,
                         const TSG_ETCS_MMI_DYN *const pTSG_ETCS_MMI_DYN)
{
  BOOLEAN8 lOut      = FALSE;
  BOOLEAN8 lSpeedVal = TRUE;
  if((pTSG_P0 != 0) && (pTSG_ETCS_MMI_DYN != 0))
  {
    UINT8 lSpeedStatus = NO_ERROR_SPEED;
    if((pTSG_P0->ValidityVreal == FALSE) || (pTSG_ETCS_MMI_DYN->MMI_V_Train == (-1)))
    {
      if((UpdateTime(&TimeSpeed)))
        lSpeedStatus = ERROR_SPEED_EBICAB; /*Speed data error*/
    }/*if*/
    else
    {
      SetRange(0, 16666);
      lSpeedVal &= (isSpeedValid(pTSG_ETCS_MMI_DYN->MMI_V_Train) == TRUE);
      SetRange(-32768, 32767);
      lSpeedVal &= ((pTSG_P0->ValidityVreal == TRUE) & (isSpeedValid(pTSG_P0->Vreal) == TRUE));
      if(lSpeedVal == TRUE)
      {
          if(SpeedPlausibilityCheck(abs(pTSG_P0->Vreal), pTSG_ETCS_MMI_DYN->MMI_V_Train) == FALSE)
          {
            /*plausibility check failed*/
            if((UpdateTime(&TimeSpeed)))
              lSpeedStatus = ERROR_SPEED_EBICAB;/*Speed data error*/
          }/*if*/
          else
            ResetTime(&TimeSpeed); /*reset the counter when all conditions are ok*/
      }/* if all speed are valid*/
    }/*else*/
    /*Set the mmi speed data consistency*/
    SetSpeedStatus(lSpeedStatus);
    lOut = TRUE;/*pointer addressed!!!!*/
  }
  return(lOut);
}

void SetSpeed2EbiCab(VCE_AUX_OUT * const pVce, MMI_AUX_OUT * const pMmi)
{
  if((pVce != 0) && (pMmi != 0))
  {
    UINT16 lSpeed = GetCcuoSpeed();
    pVce->VCE_Speed_valid = FALSE;
    pMmi->MMI_Speed_valid = FALSE;
    if(lSpeed != ACTUAL_SPEED_NOT_AVAILABLE)
    {
      Convertdmh2cms(lSpeed);
      pVce->VCE_Speed_valid = TRUE;
      pMmi->MMI_Speed_valid = TRUE;
    }
    pVce->VCE_Speed = lSpeed; /* check the right conversion!!!*/
    pMmi->MMI_Speed = lSpeed; /* check the right conversion!!!*/
  }/*if*/
}
