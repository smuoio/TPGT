/**
* \file GPP_StmPacket_types.h
* \brief the file manages the application structures of the packet STM
* \author Muoio Salvatore
*/

#ifndef __GPP_STM_PACKET_TYPES_H__
#define __GPP_STM_PACKET_TYPES_H__

#include "usertypes.h"
/**
*\def MAX_STM_READ_QUEUE
*\brief max value of STM queue
*/
#define MAX_STM_READ_QUEUE 7
/**
*\def MAX_STM_WRITE_QUEUE
*\brief max value of STM queue
*/
#define MAX_STM_WRITE_QUEUE 2
/**
* \struct t_StmIter
* \brief the structure has the generic field iteration used in the stm packet
*/
typedef struct
{
  UINT8 Iter  : 5; /*!< Iter*/
  UINT8 Spare : 3; /*!< Spare*/
}t_StmIter;
/**
* \union u_StmIter
* \brief the structure has the generic field iteration used in the stm packet
*/
union u_StmIter
{
  t_StmIter StmIter; /*!< stm structure*/
  UINT8 Data;        /*!< Data*/
};
/**
* \struct t_VersionNumber
* \brief the structure contains implicity the connection request from the
*        Stm or the connection confirm from the ETCS Onboard and provides
*        also the version number, packet STM-1
*/
typedef struct
{
  UINT8 MmiStmN058Vermajor; /*!< application layer number, major number*/
  UINT8 MmiStmN058Vermid;   /*!< application layer number, middle number*/
  UINT8 MmiStmN058Verminor; /*!< application layer number, minor number*/
  UINT8 MmiStmN035Vermajor; /*!< FFFIS STM layer compatibility number, major number*/
  UINT8 MmiStmN035Vermid;   /*!< FFFIS STM layer compatibility number, middle number*/
  UINT8 MmiStmN035Verminor; /*!< FFFIS STM layer compatibility number, minor number*/
  UINT8 MmiStmNSrsVermajor; /*!< major version number*/
  UINT8 MmiStmNSrsVerminor; /*!< minor version number*/
}t_VersionNumber;


/**
* \struct t_BRData
* \brief the update visual state of buttons
*/
typedef struct
{
  UINT8  MmiNidStm; /*!< Stm identity used to point to the correspondig palette for the button and icon*/
  UINT8  MmiStmButton; /*!< functional identity of button from button palette given by NID_STM*/
  UINT8  MmiStmNidButPos;/*!< button position on MMI on 4 bits*/
  UINT8  MMiStmNidIcon;/*!< identity of button icon to be displayed*/
  UINT16 MMiStmMButAttrib;/*!< attributes of the button*/
  UINT8  MmiStmLCaption;/*!< length of X_CAPTION on 5 bits*/
  UINT8  MmiStmXCaption[12];/*!< caption text byte string*/
}t_BRData;
/**
* \struct t_StateReport
* \brief the structure indicates to the ERTMS/ETCS the STM state
*        STM-32
*/
typedef struct
{
  union u_StmIter MmiStmNIter; /*!< Number of buttons to be update value on 5 bits*/
  t_BRData Data[10]; /*!< strcut with data for each buttons*/
}t_ButtonRequest;

/**
* \struct t_MmiStmTextMessage
* \brief the structure has the fields the packet STM-38
*/
typedef struct
{
  UINT8  MmiStmNidXMessage;/*!<Sequence number of given text messages  */
  UINT16 MmiStmMXAttribute;/*!<Attributes of text  */
  UINT8  MmiStmQAck;       /*!<Acknowledgement qualifier  */
  UINT8  MmiStmLText;      /*!<Number of characters in text message  */
  UINT8  MmiStmXText[40];  /*!<Text character  */
}t_MmiStmTextMessage;

/**
* \struct t_Stm46Iter2
* \brief the structure second iteraction  STM-46
*/
typedef struct
{
  UINT8 MmiStmMFreq; /*!<Frequency of a segment */
  UINT8 MmiStmTSound;/*!<Duration of a segment */
}t_Stm46Iter2;
/**
* \struct t_Stm46Iter1
* \brief the structure first iteraction  STM-46
*/
typedef struct
{
  UINT8 MmiNidStm;            /*!<Identity of sound palette */
  UINT8 MmiStmNidSound;       /*!<Identifier of sound */
  UINT8 MmiStmQSound;         /*!<Continuous/ Not continuous/ Stopped */
  UINT8 MmiStmNIter;          /*!<Number of segments of sound */
  t_Stm46Iter2 Stm46Iter2[31];/*!<second iteration 46 */
}t_Stm46Iter1;

/**
* \struct t_MmiStmSoundCmd
* \brief the structure has the fields the packet STM-46
*/
typedef struct
{
  union u_StmIter MmiStmNIter; /*!<Number of sounds to be generated  */
  t_Stm46Iter1 Stm46Iter1[2];  /*!<first iteration 46  */
}t_MmiStmSoundCmd;
/**
* \struct t_Stm35Iter1
* \brief the structure has the fields the packet STM-35
*/
typedef struct
{
  UINT8  MmiNidStm;         /*!<STM identity */
  UINT8  MmiStmNidIndicator;/*!<Functional identity of indicator */
  UINT8  MmiStmNidIndpos;   /*!<Indicator position on MMI */
  UINT8  MmiStmNidIcon;     /*!<Identity of icon */
  UINT16 MmiSstmMIndAttrib; /*!<Attributes of the indicator */
  UINT8  MmimStmLCaption;   /*!<Length of X_CAPTION */
  UINT8  MmiStmXCaption[12];/*!<Caption text byte string. */
}t_Stm35Iter1;

/**
* \struct t_MmiStmIndicatorRequest
* \brief the structure has the fields the packet STM-35
*/
typedef struct
{
  union u_StmIter MmiStmNIter;/*!<Number of Indicator fields */
  t_Stm35Iter1 Stm35Iter1[24];/*!<iteration */
}t_MmiStmIndicatorRequest;

/**
* \struct t_Stm34Iter
* \brief the structure has the fields the packet STM-34
*/
typedef struct
{
	UINT8  MmiNidStm;         /*!< STM identity */
	UINT8  MmiStmNidButton;   /*!< Functional identity of button */
	UINT8  MmiStmQButton;     /*!< Button event */
	UINT32 MmiStmTButtonEvent;/*!< Event timestamp */
}t_Stm34Iter;
/**
* \struct t_MmiStmButtonEventReport
* \brief the structure has the fields the packet STM-34
*/
typedef struct
{
	/*union u_StmIter MmiStmNIter;*/        /*!< Number of events being reported */
  UINT8 MmiStmNIter; /*!< Number of iter */
	t_Stm34Iter Stm34Iter[31]; /*!< stm 34 iter */
}t_MmiStmButtonEventReport;


/**
* \struct t_stm_data
* \brief the structure has the fields of all stm packet received
*        this structure is used also in the shared memory
*/
typedef struct
{
  UINT8               stm_packet[MAX_STM_READ_QUEUE];  /*maximum value of queue*/
  UINT8               diag_stm[MAX_STM_READ_QUEUE];    /*diag value of queue*/
  t_VersionNumber     VersionNumber;              /*!< STM 1 */
  UINT8               StateReport;                /*!< STM 15*/
  t_ButtonRequest     ButtonRequest;              /*!< STM 32*/
  t_MmiStmTextMessage MmiStmTextMessage;          /*!< STM 38*/
  UINT8               MmiStmNidXMessage;          /*!< STM 39*/
  t_MmiStmSoundCmd    MmiStmSoundCmd;             /*!< STM 46*/
  t_MmiStmIndicatorRequest MmiStmIndicatorRequest;/*!< STM 35*/
}t_stm_data;

/**
* \fn t_stm_data *GetStmData(void)
* \brief interface function to stm data
* \return the static address of the object t_stm_data*/

t_stm_data *GetStmData(void);

/**
* \struct t_stm_sts
* \brief the structure has the fields of all evc packet to trasmit
*        this structure is used also in the shared memory
*/
typedef struct
{
  UINT8             Pack2Write[MAX_STM_WRITE_QUEUE];/*maximum value of queue*/
  t_VersionNumber   VersionNumber;  /*!< STM 1*/
  t_MmiStmButtonEventReport MmiStmButtonEventReport; /*!< STM 34*/
}t_stm_sts;


/**
* \fn t_stm_sts *GetStmSts(void)
* \brief interface function to evc status
* \return the static addressof the object t_stm_sts
*/
t_stm_sts *GetStmSts(void);


#endif
