/**
* \file GPP_Types.h
* \brief the file manages thetypedef struct and define common
* \author Muoio Salvatore
*/


#ifndef __GPP_TYPES_H__
#define __GPP_TYPES_H__

#include "usertypes.h"


/**
* \def MMI_EVC
* \brief value for MMI->EVC
*/
#define MMI_EVC 214

/**
* \def NID_TSG
* \brief STM ID from/to TSG
*/
#define NID_TSG 253
/**
* \def NID_EVC
* \brief STM ID from/to EVC
*/
#define NID_EVC 254
/**
* \def MAX_LEN_GPP_PACK
* \brief maximum length of packet gpp
*/
#define MAX_LEN_GPP_PACK 1200
/**
* \def MAX_LEN_PORT
* \brief maximum length of port
*/
#define MAX_LEN_PORT 32
/**
* \def END_OF_STREAM
* \brief end of stream delimiter
*/
#define END_OF_STREAM 0xFF
/**
* \def NUM_PACKET2WRITE
* \brief number of packet to write via GPP
*/
#define NUM_PACKET2WRITE 5
/**
* \def NUM_STMPACKET2WRITE
* \brief number of packet to write via GPP
*/
#define NUM_STMPACKET2WRITE 2

/**
* \enum gpp_result
* \brief description of result of generic check.
*/
typedef enum
{
  GPP_SUCCESS               = 0x00U,  /*!< It is returned when the function operated correctly.*/
  GPP_BAD_PARAM             = 0x01U,  /*!< Flag set whenever an invalid input argument is provided to the function.*/
  GPP_BAD_PACKET_MAX_LENGTH = 0x02U,  /*!< Flag set when the validity value provided from the caller is not belonging to the t_validity enumeration.*/
  GPP_BAD_PACKET_MIN_LENGTH = 0x20U,  /*!< Flag set when the bit length of the application data is not less or equal to the packet length.*/
  GPP_BAD_FRAME_LENGTH      = 0x40U,  /*!< Flag set when the frame lenght is not equal to the received packet length.*/
  GPP_BAD_NID_STM           = 0x80U,  /*!< Flag set when the frame lenght is not equal to the received packet length.*/
  GPP_BAD_EVC_PLS           = 0x100U, /*!< Flag set when the EVC NID STM doesn´t support the right Nid Packet.*/
  GPP_BAD_LENGTH            = 0x200U, /*!< Flag set when the the length of packet is too short.*/
  GPP_BAD_END_STREAMING     = 0x400U, /*!< Flag set when the the end of streamin is not correct.*/
  GPP_BAD_EVC_DATA_LEN      = 0x800U, /*!< Flag set when the the length of each data packet is not correct.*/
} gpp_result;

/**
* \enum e_IndexEvcRec
* \brief description of index of evc packet received
*/
typedef enum
{
  Index_MmiStartAtp, /*!<EVC-0*/
  Index_MmiStatus,   /*!<EVC-2*/
  Index_MmiSetTimeAtp,   /*!< EVC-3*/
  Index_MmiDriverMessage, /*!<EVC-8*/
  Index_MmiTrackDescription,
  Index_MmiCurrentTrainData,
  Index_MmiFailureReportAtp,
  Index_MmiEchoedTrainData,
  Index_MmiCurrentDriverData,/*!<EVC-14*/
  Index_MmiEnableDriverRequest,/*!<EVC-21*/
  Index_MMiSpecificStmDeRequest,/*!<EVC-25*/
  Index_MmiSpecificStmDwValues,/*!<EVC-26*/
}e_IndexEvcRec;

/**
* \enum e_IndexStmRec
* \brief description of index of stm packet received
*/
typedef enum
{
  Index_VersionNumber, /*!<STM-1*/
  Index_StateReport,   /*!<STM-15*/
  Index_ButtonRequest, /*!<STM-32*/
  Index_MmiStmTextMessage,/*!<STM-38*/
  Index_MmiStmDeleteTextMessage, /*!<STM-39*/
  Index_MmiStmSoundCmd,/*!<STM-46*/
  Index_MmiStmIndicatorRequest,/*!<STM-35*/
}e_IndexStmRec;


#endif
