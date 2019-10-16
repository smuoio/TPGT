/**
* \file GPP_EvcPacket_types.h
* \brief the file manages the application structures of the packet EVC
* \author Muoio Salvatore
*/


#ifndef __GPP_EVC_PACKET_TYPES_H__
#define __GPP_EVC_PACKET_TYPES_H__

#include "usertypes.h"
/**
*\def MAX_EVC_READ_QUEUE
*\brief max value of EVC queue
*/
#define MAX_EVC_READ_QUEUE 12
/**
*\def MAX_EVC_WRITE_QUEUE
*\brief max value of EVC queue
*/
#define MAX_EVC_WRITE_QUEUE 7

/**
* \struct t_MmiStartAtp
* \brief field of packet EVC-0
*/
typedef struct
{
  UINT8 MmiMStartReq; /*!< type of request from ETCS and MMI*/
}t_MmiStartAtp;

/**
* \struct t_Evc2Sts
* \brief structure with all bitfield of packet Evc2Sts
*/
typedef struct
{
  UINT8 MmiMVehicleReady: 1; /*!< Status of external vehicle I/O */
  UINT8 Spare           : 4;
  UINT8 MmiMOverrideEOA : 1; /*!< Override EOA is activated */
  UINT8 MmiMActiveCabin : 2; /*!< Identity of active cabin */
}t_Evc2Sts;
/**
* \union u_CabData
* \brief union used to manage the bitfield and whole data
*/
union u_CabData
{
  t_Evc2Sts Evc2Sts;
  UINT8     Data;
};
/**
* \struct t_MmiStatus
* \brief the structure has the fields of packet EVC-2
*/
typedef struct
{
  UINT8           MmiMAdhesion;    /*!<Current applied adhesion coefficient*/
  union u_CabData CabData;         /*!<Current applied status of cab */
  UINT32          MmiNidOperation; /*!<Train running number*/
}t_MmiStatus;

/**
* \struct t_MmiSetTimeAtp
* \brief field of packet EVC-3
*/
typedef struct
{
  UINT32 MmiTUtc;       /*!<UTC time*/
  UINT8 MmiTZoneOffset; /*!<Time zone offset*/
}t_MmiSetTimeAtp;


/**
* \struct t_MmiMrsp
* \brief field of speed EVC-4
*/
typedef struct
{
  INT16 MmiVMrsp;  /*!<New speed value k*16*/
  INT32 MmiOMrsp;  /*!<Start position of new speed value k*32*/
}t_MmiMrsp;
/**
* \struct t_MmiGradient
* \brief field of gradient EVC-4
*/
typedef struct
{
  INT16 MmiGGradient;  /*!<New gradient value k*16*/
  INT32 MmiOGradient;  /*!<Start position of new gradient value k*32 */
}t_MmiGradient;
/**
* \struct t_MmiTrackDescription
* \brief field of packet EVC-4
*/
typedef struct
{
  INT16 MmiVMrspCurr;       /*!<Current speed value*/
  UINT8 MmiNMrsp;           /*!<Number of speed information field*/
  t_MmiMrsp MmiMrsp[30];
  INT16 MmiGGradientCurr;   /*!<Current gradient value*/
  UINT8 MmiNGradient;       /*!<Number of gradient information field*/
  t_MmiGradient MmiGradient[30];
}t_MmiTrackDescription;


/**
* \struct t_MmiQSpec
* \brief field of packet t_MmiQSpec
*/
typedef struct
{
  UINT8 TrainData : 1;  /*!<Qualifier for special train data*/
  UINT8 Spare     : 7;  /*!<Spare*/
}t_MmiQSpec;
/**
* \union u_MmiQSpec
* \brief union used to manage the bitfield and whole data
*/
union u_MmiQSpec
{
	t_MmiQSpec Spec;
	UINT8 Data;
};
/**
* \struct t_MmiQBData
* \brief field of packet t_MmiQBData
*/
typedef struct
{
  UINT8 ByPerc : 1; /*!<Qualifier for brake data(Q_BDP in the right column)*/
  UINT8 Spare  : 7; /*!<Spare*/
}t_MmiQBData;
/**
* \union t_MmiQBDP0
* \brief * \brief union used to manage the bitfield and whole data
*/
union u_MmiQBData
{
	t_MmiQBData BData;
	UINT8 Data;
};
/**
* \struct t_MmiQBDP1
* \brief field of packet t_MmiQBDP1
*/
typedef struct
{
  UINT8 MmiMBrakePosition; /*!<Brake position*/
  UINT8 MmiMBrakePerc;     /*!<Brake Percentage*/
  UINT8 Spare;             /*!<Spare*/
}t_MmiQBDP1;
/**
* \struct t_MmiQBDP0
* \brief field of packet t_MmiQBDP0
*/
typedef struct
{
  UINT8 MmiMNumUnitTrainset; /*!<Number of unit trainsets*/
  UINT8 MmiMNumIsolBrake; /*!<Number of isolated brakes*/
  UINT8 Spare; /*!<Spare*/
}t_MmiQBDP0;

/**
* \struct t_MmiCurrentTrainData
* \brief field of packet EVC-6, sporadic from ETCS Onboard
*/
typedef struct
{
  UINT16 MmiMDataEditEnable;  /*!<Edit enable bits*/
  UINT16 MmiNcTrain;  /*!<Train category*/
  UINT16 MmiLTrain;  /*!<Max train length*/
  UINT16 MmiVMaxTrain;  /*!<Max. train speed*/
  UINT8  MmiMLoadingGuage;  /*!<Max. loading gauge of the train */
  UINT8  MmiMAxleLoad;  /*!<Max. axle load of the train*/
  UINT8  MmiNTraction;  /*!<Number of traction type asets defined*/
  UINT8  MmiMTraction[20]; /*!<Traction type of the train k*8*/
  UINT8  MmiMAirtight;  /*!<Train equipped with airtight system*/
  UINT8  MmiNDsguarant;  /*!<Number of SB dec. intervals */
  UINT8  MmiNDeguarant;  /*!<Number of EB deceleration intervals*/
  UINT16 MmiTBrakeSb;  /*!<Time to get defined SB deceleration*/
  UINT16 MmiTBrakeEb; /*!<Time to get defined EB deceleration*/
  UINT16 MmiTCutOffTraction;  /*!<Traction cutt off time*/
  UINT16 MmiAMaxAcc; /*!<Max train acceleration*/
  UINT16 MmiAMaxDec; /*!<Max train deceleration*/
  union u_MmiQSpec MmiQS;
  union u_MmiQBData MmiQB;
  t_MmiQBDP1 MmiQBDP1;
  t_MmiQBDP0 MmiQBDP0;
}t_MmiCurrentTrainData;

/**
* \struct t_Text
* \brief field of packet Text
*/
typedef struct
{
  UINT8 MmimQTextClass    : 1; /*!< Text Class */
  UINT8 MmimQTextCriteria : 3; /*!< Tells MMI what to do with exit */
  UINT8 MmiCharSet        : 2; /*!< Character set for X_TEXT */
  UINT8 Spare             : 2; /*!< Spare */
}t_Text;
/**
* \union u_MmiQ
* \brief union used to manage the bitfield and whole data
*/
union u_MmiQ
{
  t_Text   Text; /*!< bitfield Text structure */
  UINT8    Data; /*!< whole data */
};

typedef struct
{
  UINT8 MmiITextBegin; /*!<First index number of message block k*/
  UINT8 MmiITextEnd;   /*!<Last index number of message block k*/
}t_MsgBlock;

/**
* \struct t_MmiDriverMessage
* \brief the structure has the fields of packet EVC-8
*/
typedef struct
{
  UINT8         MmiIText;      /*!< Text Identifier */
  union u_MmiQ  MmiQ;          /*!< Current applied Q Text struct */
  UINT16        MmiQText;      /*!< Predefined text message */
  UINT8         MmiNText;      /*!< Length of the plain text message*/
  UINT8         MmiXText[255]; /*!< Plain text message*/
  UINT8         MmiNMsgBlock;  /*!<Number of message blocks*/
  t_MsgBlock    MsgBlock[4];   /*!<structure of message blocks*/
}t_MmiDriverMessage;

/**
* \struct t_MmiQFail
* \brief the structure in bit field
*/
typedef struct
{
  UINT8 MmiQFailureSeverity : 4; /*!< Mmi Q Failure severity */
  UINT8 MmiIUnit            : 4; /*!< Mmi I Unit */
}t_MmiQFail;
/**
* \union u_MmiFailure
* \brief union used to manage the bitfield and whole data
*/
union u_MmiFailure
{
  t_MmiQFail Fail;
  UINT8      Data;
};

/**
* \struct t_Criteria
* \brief the structure in bit field
*/
typedef struct
{
  UINT8 Spare               : 4; /*!< spare */
  UINT8 MmiQFailureCriteria : 4; /*!< Mmi Q Failure Criteria */
}t_Criteria;
/**
* \union u_MmiCriteria
* \brief union used to manage the bitfield and whole data
*/
union u_MmiCriteria
{
  t_Criteria Criteria;
  UINT8      Data;
};
/**
* \struct t_MmiFailureReportAtp
* \brief the structure has the fields of packet EVC-9
*/
typedef struct
{
  UINT8  MmiQFailureClass; /*!< Failure struct */
  union  u_MmiFailure MmiFailure;
  UINT16 MmiIFailureNUmber; /*!< Internal failure number */
  union  u_MmiCriteria MmiCriteria;
  UINT8  MmiNText; /*!< Length of the plain text message */
  UINT8  MmiXText[255]; /*!< Plain text message */
}t_MmiFailureReportAtp;

/**
* \struct t_MmiEchoedTrainData
* \brief the structure has the fields of packet EVC-10
*/
typedef struct
{
  union  u_MmiQBData MmiQB; /*!< Qualifier for brake */
  UINT8  Spare;
  UINT8  MmiMNumIsolBrake; /*!< Number of isolated brakes */
  UINT8  MmiMNumUnitTrainset;/*!< Number of unit trainset */
  union  u_MmiQSpec MmiQS;
  UINT16 MmiAMaxDec;  /*!< Max train deceleration */
  UINT16 MmiAMaxAcc; /*!< max train acceleration */
  UINT16 MmiTCutOffTraction; /*!< Traction cut off time */
  UINT16 MmiTBrakeEb; /*!< Time to get defined EB deceleration */
  UINT16 MmiTBrakeSb; /*!< Time to get defined SB deceleration */
  UINT8  MmiNDeguarant; /*!< Number of EB deceleration intervals */
  UINT8  MmiNDsguarant; /*!< Time to get defined EB deceleration */
  UINT8  MmiMAirtight; /*!< Train equipped with airtight system */
  UINT8  MmiNTraction; /*!< Number of traction type sets defined */
  UINT8  MmiMTraction[20];/*!< Traction type of the train */
  UINT8  MmiMAxleLoad; /*!< Max. axle load of the train */
  UINT8  MmiMLoadingGuage; /*!< max. loading gauge of the train */
  UINT16 MmiVMaxTrain; /*!< Max train speed */
  UINT16 MmiLTrain; /*!< Max train speed */
  UINT16 MmiNcTrain; /*!< Train category */
  UINT16 MmiMDataEditEnable; /*!< Edit enable bits */
}t_MmiEchoedTrainData;


typedef struct
{
  UINT8 Driver_Data      : 3; /*!< Data Entry Windows and Enable Control */
  UINT8 Driver_Data_View : 1; /*!< Qualifier to indicate Driver Data View */
  UINT8 Spare            : 4;
}t_Driver;
/**
* \union u_MmiQDriver
* \brief union used to manage the bitfield and whole data
*/
union u_MmiQDriver
{
  t_Driver Driver;
  UINT8    Data;
};
/**
* \struct t_MmiCurrentDriverData
* \brief the structure has the fields of packet EVC-14
*/
typedef struct
{
  UINT32 MmiNidDriver; /*!< Current driver identity number*/
  UINT32 MmiNidOperation;/*!< Train running number*/
  UINT8  MmiNidLanguage;/*!< Currently stored language code*/
  union u_MmiQDriver MmiQDriver;
}t_MmiCurrentDriverData;


/**
* \struct t_MmiEnableDriverRequest
* \brief the structure has the fields of packet EVC-21
*/
typedef struct
{
  UINT32 MmiQRequestEnable; /*!< requests that shall be accessible by the driver */
}t_MmiEnableDriverRequest;


/*************EVC25*************/
/**
* \struct t_Evc25StmIter2
* \brief the structure is the second iteration of main structure
*/
typedef struct
{
  UINT8 MmiStmLValue;    /*!< Length of X_VALUE*/
  UINT8 MmiStmXValue[10];/*!< Data for pick-up list value */
}t_Evc25StmIter2;

/**
* \struct t_MmiStmML
* \brief the structure manages the attribute
*/
typedef struct
{
  UINT16 LCaption  :5; /*!<Length of X_CAPTION for data label*/
  UINT16 XAttribute:10;/*!<Attribute for text string of the data*/
  UINT16 Spare     :1; /*!<Spare*/
}t_MmiStmML;
/**
* \union u_MmiStmML
* \brief union used to manage the bitfield and whole data
*/
union u_MmiStmML
{
  t_MmiStmML MmiStmML; /*!<structure attribute*/
  UINT16 Data;         /*!<Data*/
};

/**
* \struct t_Evc25StmIter1
* \brief iteration field of packet EVC-25
*/
typedef struct
{
  UINT8            MmiNidStm;/*!< STM identity  */
  UINT8            MmiStmNidData;/*!< Identifier of a Specific STM Data */
  /*union u_MmiStmML uMmiStmML;*/
  UINT16           MmiXAttribute;/*!<Attribute for text string of the datac*/
  UINT8            MmiLCaption;/*!<Length of X_CAPTION for data labelc*/
  UINT8            MmiStmXCaption[20];/*!< Data label caption text stringc*/
  UINT8            MmiStmLValue; /*!<Length of X_Value */
  UINT8            MmiStmXValue[10];/*!<Data value caption text bytestring */
  UINT8            MmiStmNIter; /*!<Maximum iteration data pick-up list values */
  t_Evc25StmIter2  Evc25StmIter2[16];
}t_Evc25StmIter1;
/**
* \struct t_StmQ
* \brief biet field of first part of EVC 25
*/
typedef struct
{
  UINT8 Driverint : 1; /*!< Need for driver intervention or not. */
  UINT8 Following : 1; /*!< Indicate a following request */
  UINT8 Iter      : 5; /*!< Maximum iteration data [0..5] */
  UINT8 Spare     : 1;
}t_StmQ;
/**
* \union u_MmiStmQ
* \brief union used to manage the bitfield and whole data
*/
union u_MmiStmQ
{
  t_StmQ StmQ; /*!< Identifier t_StmQ */
  UINT8  Data; /*!< Data */
};
/**
* \struct t_MMiSpecificStmDeRequest
* \brief field of packet EVC-25
*/
typedef struct
{
  UINT8 MmiNidStm;                  /*!< Identifier MmiNidStm */
  union u_MmiStmQ uMmiStmQ;         /*!< Identifier union u_MmiStmQ */
  t_Evc25StmIter1 Evc25StmIter1[4]; /*!< Identifier iteration1 */
}t_MMiSpecificStmDeRequest;

/**
* \struct t_Evc26StmIter
* \brief iteration field of packet EVC-26
*/
typedef struct
{
  UINT8            MmiNidStm;/*!< STM identity  */
  UINT8            MmiStmNidData;/*!< Identifier of a Specific STM Data */
  /*union u_MmiStmML uMmiStmML;*/
  UINT16           MmiXAttribute;/*!<Attribute for text string of the datac*/
  UINT8            MmiLCaption;/*!<Length of X_CAPTION for data labelc*/
  UINT8            MmiStmXCaption[20];/*!< Data label caption text stringc*/
  UINT8            MmiStmLValue; /*!<Length of X_Value */
  UINT8            MmiStmXValue[10];/*!<Data value caption text bytestring */
  UINT8            MmiStmNIter; /*!<Maximum iteration data pick-up list values */
}t_Evc26StmIter;

/**
* \struct t_StmIter
* \brief the structure has the generic field iteration used in the stm packet
*/
typedef struct
{
  UINT8 Following  : 1; /*!< Iter*/
  UINT8 Iter       : 5; /*!< Spare*/
  UINT8 Spare      : 2;
}t_Evc26Iter;
/**
* \union u_StmIter
* \brief the structure has the generic field iteration used in the stm packet
*/
union u_Evc26Iter
{
  t_Evc26Iter StmIter; /*!< stm structure*/
  UINT8 Data;        /*!< Data*/
};
/**
* \struct t_MmiSpecificStmDwValues
* \brief field of packet EVC-26
*/
typedef struct
{
  UINT8 MmiNidStm;                /*!<Identity of the STM that sent the data*/
  union u_Evc26Iter uEvc26Iter;
  t_Evc26StmIter Evc26StmIter[4]; /*!< Identifier iteration1 */
}t_MmiSpecificStmDwValues;


/**
* \struct t_evc_data
* \brief the structure has the fields of all evc packet received
*        this structure is used also in the shared memory
*/
typedef struct
{
  UINT8                     evc_packet[MAX_EVC_READ_QUEUE]; /*maximum value of queue*/
  UINT8                     diag_evc[MAX_EVC_READ_QUEUE];   /*diag value of queue*/
  t_MmiStartAtp 		        MmiStartAtp;             /*!< EVC-0 */
  t_MmiStatus   	          MmiStatus;               /*!< EVC-2 */
  t_MmiSetTimeAtp           MmiSetTimeAtp;           /*!< EVC-3*/
  t_MmiDriverMessage        MmiDriverMessage;        /*!< EVC-8*/
  t_MmiTrackDescription     MmiTrackDescription;     /*!<EVC-4*/
  t_MmiCurrentTrainData     MmiCurrentTrainData;     /*!<EVC-6*/
  t_MmiFailureReportAtp     MmiFailureReportAtp;     /*!<EVC-9*/
  t_MmiEchoedTrainData      MmiEchoedTrainData;      /*!< EVC-10 */
  t_MmiCurrentDriverData    MmiCurrentDriverData;    /*!< EVC-14 */
  t_MmiEnableDriverRequest  MmiEnableDriverRequest;  /*!< EVC-21 */
  t_MMiSpecificStmDeRequest MMiSpecificStmDeRequest; /*!< EVC-25 */
  t_MmiSpecificStmDwValues  MmiSpecificStmDwValues;  /*!< EVC-26 */
}t_evc_data;


/**
* \struct t_MmiStartMmi
* \brief the scructure manages the data of EVC-100. Data to put via MVB
*/
typedef struct
{
  UINT8 MmiMStartInfo;         /*!< MMI start-up info*/
  UINT8 MmiMStartStatus;       /*!< MMI start-up status*/
  UINT8 MmiMType;              /*!< MMI type*/
  UINT8 MmiMifVer[3];          /*!< MMI IF version*/
  UINT8 MmiMSwVer[3];          /*!< MMI SW version*/
  UINT8 MmiNidEngine2[3]; /*!<Vehicle identity from Train Data Set Configuration file*/
  UINT8 MmiTFixedTrainsetVDate[10]; /*!<Creation date from Train Data Set Configuration file*/
  UINT8 MmiTFixedTrainsetVTime[8]; /*!<Creation time from Train Data Set Configuration file*/
}t_MmiStartMmi;

/* EVC 101*/
/**
* \struct t_MmiRequest
* \brief the scructure manages the data of EVC-101. Data to put via MVB
*/
typedef struct
{
	UINT8 Request; /*!< Request */
}t_MmiRequest;
/**
* \struct t_CurrMmi
* \brief this packet contains the fields of current ATP and curret health status
*/
typedef struct
{
  UINT8 MmiMMmiStatus : 4;   /*!< the current health status of MMI*/
  UINT8 MmiMMode      : 4;   /*!< the current ATP mode as indicated by the MMI*/
}t_CurrMmi;
/**
* \struct t_MmiStatus
* \brief this packet contains the fields of speed consistency and active cabin
*/
typedef struct
{
  UINT8 MmiMactiveCabin : 2; /*!< Identity of active cabin */
  UINT8 MmiMSpeedSts    : 2; /*!< MMI Speed data consistency */
  UINT8 Spare           : 4; /*!< spare field */
}t_SpeedCabin;
/**
* \struct t_MmiStatusReport
* \brief this packet shall be sent cyclically as an alive
*        and status signal EVC-102
*/
typedef struct
{
  t_CurrMmi    CurrMmi;   /*!< Current informations  */
  t_SpeedCabin SpeedCabin; /*!< status information */
}t_MmiStatusReport;



/**
* \struct t_MmiNewDriverData
* \brief This packet shall be sent when the driver updates or
*        confirms the driver identity number or other
*        operational information. The content is the same as in packet 14
*        and status signal EVC-104
*/
typedef struct
{
  UINT32 MmiNidDriver;    /*!< Current driver identity number */
  UINT32 MmiNidOperation; /*!< Train running number */
  UINT8  MmiNidLanguage;  /*!< Currently stored language code*/
  union u_MmiQDriver MmiQDriver;/*!< Bit mask to tag changed driver data*/
}t_MmiNewDriverData;
/**
* \struct t_StmNIter
* \brief This packet manages the value of iteration
*/
typedef struct
{
	UINT8 Iter: 5; /*!< iter */
	UINT8 Spare:3; /*!< spare */
}t_StmNIter;
/**
* \union u_StmNIter
* \brief the structure has the generic field iteration used in the stm packet
*/
union u_StmNIter
{
	t_StmNIter StmNIter; /*!< iter structure*/
	UINT8 Data;          /*!< data */
};
/**
* \struct t_Evc123StmIter
* \brief This packet manages the iteration of EVC123 packet
*/
typedef struct
{
  UINT8 MmiNidStm;        /*!< STM identity*/
  UINT8 MmiStmNidData;    /*!< Identifier of a Specific STM*/
  UINT8 MmiStmLValue;     /*!< Length of X_VALUE*/
  UINT8 MmiStmXValue[255];/*!< Data value caption text*/
}t_Evc123StmIter;

/**
* \struct t_MmiSpecificStmDataToStm
* \brief This packet is used when the MMI returns specific data
*        requested from the STM. EVC -123
*/
typedef struct
{
  UINT8 MmiNiStm;
  /*UINT8 MmiStmNIter;*/
  union u_StmNIter uMmiStmNIter;
  t_Evc123StmIter Evc123StmIter[5];
}t_MmiSpecificStmDataToStm;

/**
* \struct t_MmiQAck
* \brief Logical Acknowledge or Not
 *       Acknowledge (EVC-111)
*/
typedef struct
{
  UINT8 MmiQAck : 4; /*!< Logical Acknowledge or Not Acknowledge */
  UINT8 Spare   : 4; /*!< byte alignment */
}t_MmiQAck;

/**
* \struct t_MmiDriverMessageAck
* \brief This packet shall be sent as a positive/negative response,
*        if required, on the “Driver Message” packet
*        from the ETCS Onboard (EVC-111)
*/
typedef struct
{
  UINT8 MmiIText;    /*!< Identifier of the acknowledged text */
  t_MmiQAck MmiqAck; /*!< Logical Acknowledge or Not Acknowledge */
}t_MmiDriverMessageAck;

/**
* \struct t_MmiSetTime
* \brief packet EVC-109
*/
typedef struct
{
  UINT32 Utc;        /*!< UTC time*/
  UINT8 Zone_Offset; /*!< Time Zone Offset*/
}t_MmiSetTime;


/**
* \struct t_evc_sts
* \brief the structure has the fields of all evc packet to trasmit
*        this structure is used also in the shared memory
*/
typedef struct
{
  UINT8             Pack2Write[MAX_EVC_WRITE_QUEUE];/*maximum value of queue*/
  t_MmiStartMmi     MmiStartMmi;             /*!< EVC-100 */
  UINT8             MmiRequest;              /*!< EVC 101 */
  t_MmiStatusReport MmiStatusReport;         /*!< EVC 102 */
  t_MmiNewDriverData MmiNewDriverData;       /*!< EVC 104 */
  t_MmiDriverMessageAck MmiDriverMessageAck; /*!< EVC 111 */
  t_MmiSpecificStmDataToStm MmiSpecificStmDataToStm; /*!<EVC 123*/
  t_MmiSetTime              MmiSetTime; /*!<EVC 109*/
}t_evc_sts;

/**
* \fn t_evc_sts *GetEvcSts(void)
* \brief interface function to evc status
* \return the static addressof the object t_evc_sts
*/
t_evc_sts *GetEvcSts(void);
/**
* \fn t_evc_data *GetEvcData(void)
* \brief interface function to evc data
* \return the static addressof the object t_evc_data
*/
t_evc_data *GetEvcData(void);


#endif
