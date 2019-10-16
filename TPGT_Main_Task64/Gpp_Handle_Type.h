/**
* \file Gpp_Handle_Type.h
* \brief the file manages the application type, struct and enum used in
*        Gpp_handle component
* \author Muoio Salvatore
*/

#ifndef __GPP_HANDLE_TYPE_H__
#define __GPP_HANDLE_TYPE_H__

/**
* \enum e_MmiMStartReq
* \brief This variable indicates the meaning of the MMI_START_ATP
*        packet (EVC-0)
*/
typedef enum
{
    VERSION_INFO_REQ,          /*!<Version info request            */
    IDLE_STATE,                /*!<Go to Idle state                */
    MMI_NOT_SUPPORTED,         /*!<Error: MMI type not supported   */
    INCOMPATIBLE_IF_VERSIONS,  /*!<Error: Incompatible IF versions */
    INCOMPATIBLE_SW_VERSIONS,  /*!<Error: Incompatible SW versions */
    MMI_DEFAULT =0xFF,         /*!<Default value */
}e_MmiMStartReq;

/**
* \enum e_MmiMMmiStatus
* \brief This value gives the actual status of the MMI
*/
typedef enum
{
  UNKNOWN,       /*!<UNKNOWN       */
  FAILURE,       /*!<FAILURE       */
  IDLE,          /*!<IDLE          */
  ACTIVE,        /*!<ACTIVE        */
  SPARE,         /*!<SPARE         */
  ATP_DOWN_NACK, /*!<ATP DOWN NACK */
  ATP_DOWN_ACK,  /*!<ATP DOWN ACK  */
}e_MmiMMmiStatus;

/**
* \enum e_StmStateReport
* \brief This value gives the report of STM NID State
*/
typedef enum
{
  STATE_IDLE = 0,
  POWER_ON = 1,
  CONFIGURATION,
  DATA_ENTRY,
  COLD_STANDBY,
  HOT_STANDBY = 6,
  DATA_AVAILABLE,
  STM_FAILURE,
}e_StmStateReport;

/**
* \enum e_StmSound
* \brief This value gives the actual status of the MMI
*/
typedef enum
{
  STOP_SOUND,       /*!<Stop any sound        */
  ZUGBEEINFLUSSUNG, /*!<ZUGBEEINFLUSSUNG brake sound */
  SCHNARRE,         /*!<Schnarre sound       */
  ZWANGSBREMSUNG,   /*!<Energency brake sound */
}e_StmSound;

typedef enum
{
  index_evc_0 = 0,
  index_evc_2,
  index_evc_3,
  index_evc_8,
  index_evc_4,
  index_evc_6,
  index_evc_9,
  index_evc_10,
  index_evc_14,
  index_evc_21,
  index_evc_25,
  index_evc_26
}e_evc_index;

typedef enum
{
  index_stm_1  = 0,
  index_stm_15,
  index_stm_32,
  index_stm_38,
  index_stm_39,
  index_stm_46,
  index_stm_35,

}e_stm_index;

#endif
