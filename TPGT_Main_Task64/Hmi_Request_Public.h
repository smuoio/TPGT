/**
* \file Hmi_Request_Public.h
* \brief the file manages the public functions of hmi regarding
*        the request of drive, used in other sw component(translation unit)
* \author Muoio Salvatore
*/
#ifndef __HMI_REQUEST_PUBLIC_H__
#define __HMI_REQUEST_PUBLIC_H__


/**
* \enum e_Evc101Request
* \brief value of EVC 101
*/
typedef enum
{
  EVC101_ZDE_REQUEST               = 3, /*!< Status of ZDE request */
  EVC101_ZDE_CANCEL                = 4, /*!< Status of ZDE cancel */
  EVC101_DRIVER_ID_REQUEST         = 20, /*!< Status of Driver Id request */
  EVC101_ZD_REQUEST                = 21, /*!< Status of ZD request */
  EVC101_SBV_REQUEST               = 22, /*!< Status of brake test request */
  EVC101_TRAIN_NUMBER_REQUEST      = 30, /*!< Status of Train Number Id request */
  EVC101_EXIT_TRAIN_NUMBER_REQUEST = 31, /*!< Status of exit of  Train Number Id request*/
  EVC101_EXIT_DRIVER_ID_REQUEST    = 34, /*!< Status of exit of  Driver Id request*/
}e_Evc101Request;

/**
* \enum e_EVC104Request
* \brief value of EVC 104
*/
typedef enum
{
	EVC_104_REQUEST_UNKNOWN, /*!< No request set */
	EVC_104_REQUEST_SET,     /*!< Request set */
}e_EVC104Request;

/**
* \enum e_EVC123Request
* \brief value of EVC 123
*/
typedef enum
{
	EVC_123_REQUEST_UNKNOWN, /*!< No request set */
	EVC_123_REQUEST_SET,     /*!< Request set */
	EVC_123_REQUEST_CLOSE,   /*!< Request close */
}e_EVC123Request;


/**
* \enum e_DrvRqst
* \brief value of bitmask of request
*/
typedef enum
{
  DrvRqst_NO ,           /*!<no request from the driver*/
  DrvRqst_ZDE,           /*!<request train data entry*/
  DrvRqst_ZD ,           /*!<request train data view */
  DrvRqst_SBV,           /*!< requets test data view */
  DRVRqst_ChnTrainNumber, /*!<request change train number*/
  DRVRqst_DrvId,          /*!<request Direver Id*/
}e_DrvRqst;

e_DrvRqst GetDrvRqst(void);
/**
* \fn UINT8 is_EVC101_requested(void)
* \brief the function checks is active one request to enable the message 101
* \return null
*/
UINT8 is_EVC101_requested(void);
/**
* \fn UINT8 is_EVC104_requested(void)
* \brief the function checks is active one request to enable the message 104
* \return null
*/
UINT8 is_EVC104_requested(void);
/**
* \fn UINT8 is_EVC123_requested(void)
* \brief the function checks is active one request to enable the message 123
* \return null
*/
UINT8 is_EVC123_requested(void);
/**
* \fn UINT8 is_EVC11_requested(void)
* \brief the function checks is active one request to enable the message 123
* \return null
*/
UINT8 is_EVC111_requested(void);
/**
* \fn void is_STM34_requested(UINT8 aRequest)
* \brief the function checks is active one request to enable the message 34
*/
UINT8 is_STM34_requested(void);
/**
* \fn void SetEvc123Request(UINT8 aRequest)
* \brief the function sete the request of ?acket evc 123
*/
void SetEvc123Request(UINT8 aRequest);
/**
* \fn UINT8 SetEvc101Request(void)
* \brief the function set the request of packet evc 101
* \return null
*/
void SetEvc101Request(UINT8 aRequest);
/**
* \fn void SetEvc104Request(UINT8 aRequest)
* \brief the function set the request of packet evc-104
* \return null
*/
void SetEvc104Request(UINT8 aRequest);
/**
* \fn void ResetDrvRqst(void)
* \brief the function reset the driver request
* \return null
*/
void ResetDrvRqst(void);
/**
* \fn UINT16 GetHmiTxtMsgId(void)
* \brief the function returns the TextID from the HMI
* \return null
*/
UINT16 GetHmiTxtMsgId(void);
/**
* \fn UINT8 GetHmiTxtMsgAck(void)
* \brief the function returns the logical aknowlodge from HMI
* \return null
*/
UINT8 GetHmiTxtMsgAck(void);
/**
* \fn UINT8 GetStm34QButton(void)
* \brief the function returns the logical aknowlodge from HMI
* \return null
*/
UINT8 GetStm34QButton(void);
/**
* \fn void SetPendingDisplay(BOOLEAN8 aValue)
* \brief the function set the criable sZugBesyWait
* \return null
*/
void SetPendingDisplay(BOOLEAN8 aValue);

#endif
