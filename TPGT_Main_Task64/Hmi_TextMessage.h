/**
* \file Hmi_TextMessage.h
* \brief the file manages the private functions of EVC-8 text message to hmi
* \author Muoio Salvatore
*/


#ifndef __HMI_TEXT_MESSAGE_H__
#define __HMI_TEXT_MESSAGE_H__

/*#include "usertypes.h"*/
#include "CCUT_Util.h"

/**
* \def NUM_MAX_MESSAGE
* \brief Value of maximmum message
*/
#define NUM_MAX_MESSAGE 50
/**
* \def SET_EVC8_260_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_260_BIT  0x00000001u
/**
* \def SET_EVC8_261_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_261_BIT  0x00000002u
/**
* \def SET_EVC8_270_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_270_BIT  0x00000004u
/**
* \def SET_EVC8_271_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_271_BIT  0x00000008u
/**
* \def SET_EVC8_272_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_272_BIT  0x00000010u
/**
* \def SET_EVC8_514_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_514_BIT  0x00000080u
/**
* \def SET_EVC8_515_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_515_BIT  0x00000100u
/**
* \def SET_EVC8_516_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_516_BIT  0x00000200u
/**
* \def SET_EVC8_517_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_517_BIT  0x00000400u
/**
* \def SET_EVC8_519_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_519_BIT  0x00000800u
/**
* \def SET_EVC8_521_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_521_BIT  0x00002000u
/**
* \def SET_EVC8_524_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_524_BIT  0x00004000u
/**
* \def SET_EVC8_525_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_525_BIT  0x00008000u
/**
* \def SET_EVC8_526_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_526_BIT  0x00010000u
/**
* \def SET_EVC8_527_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_527_BIT  0x00020000u
/**
* \def SET_EVC8_528_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_528_BIT  0x00040000u
/**
* \def SET_EVC8_535_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_535_BIT  0x00080000u
/**
* \def SET_EVC8_536_BIT
* \brief Value of EVC8
*/
#define SET_EVC8_536_BIT  0x00100000u

/**
* \enum u_MmiQTextCriteria
* \brief description of criteria of text described in the EVC 8 packet
*/
typedef enum
{
  Keep_Text_After_Ack,   /*!< Add text with ack prompt, keep text after ack*/
  Remove_Text_after_Ack, /*!< Add text with ack prompt, remove text after ack*/
  Remove_Text_after_Ack_Nak,/*!< Add text with ack/nak prompt, remove text after ack/nak*/
  Add_Informative_Text,     /*!< Add informative text*/
  Remove_This_text, /*!<Remove this text. Text to remove is defined by MMI_I_TEXT.*/
}u_MmiQTextCriteria;

/**
* \enum u_MmiQText
* \brief description of value of Mmi Q Text
*/
typedef enum
{
  Perform_Brake_test         = 514,
  Unable_start_Brake_Test    = 515,
  Brake_Test_in_Progress     = 516,
  Brake_Test_failed_new_Test = 517,
  Brake_Test_Timeout_hours   = 525,
  New_ATP_Power_up_hours     = 535,
}u_MmiQText;

/**
* \struct t_StoreTextMessage
* \brief local structure of message used for the store
*/
typedef struct
{
  UINT8  MmiIText;      /*!< Text Identifier */
  UINT8  MmiQ;          /*!< Current applied Q Text struct */
  UINT16 MmiQText;      /*!< Predefined text message */
  UINT8  MmiNText;      /*!< Length of the plain text message*/
  UINT8  MmiXText[255]; /*!< Plain text message*/
}t_StoreTextMessage;

/**
* \fn void Hmi_StoreMessage(UINT8 *pIText)
* \brief The function store the message evc-8 in a local copy
* \param pIText MmiIText of EVC-8 to store
*/
void Hmi_StoreMessage(UINT8 *pIText);
/**
* \fn void Hmi_DeleteMessage(UINT8 aTxtId)
* \brief The function delete the message evc-8 from the local copy
* \param aTxtId text id of messsage that we want delete
*/
void Hmi_DeleteMessage(UINT8 aTxtId);
/**
* \fn void Hmi_HandleTextMessage(DS_CCUT_HMI_TRN *const pHmiTrn)
* \brief The main function of engine of EVC 8 message
* \param pHmiTrn pointer to the HMI structure
*/
void Hmi_HandleTextMessage(DS_CCUT_HMI_TRN *const pHmiTrn);
/**
* \fn void Hmi_OutputTxtMsg(DS_CCUT_HMI_TRN *const pHmiTrn)
* \brief The function write the text message to HMI
* \param pHmiTrn pointer to the HMI structure
* \param sIText I Text only if the q criteria is 0,1,2
*/
void Hmi_OutputTxtMsg(DS_CCUT_HMI_TRN *const pHmiTrn);
/**
* \fn void Hmi_DataTransferPlaceHolder(TPT_NonAckDriverMessage * const pMsg)
* \brief The function manages some field of EVC8 message
* \param pMsg pointer to the TPT_NonAckDriverMessage structure
*/
void Hmi_DataTransferPlaceHolder(TPT_NonAckDriverMessage * const pMsg);


#endif
