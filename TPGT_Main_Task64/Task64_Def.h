/**
* \file Task64_Def.h
* \brief the file contains all the define used in the task 64 ms
* \author Muoio Salvatore
*/
#ifndef __TASK64_DEF_H__
#define __TASK64_DEF_H__
/**
* \def NO_BT_ACTIVE
* \brief Value NO_BT_ACTIVE of Brake Test
*/
#define NO_BT_ACTIVE 0
/**
* \def TCO_CMD
* \brief Value TCO_CMD traction lock
*/
#define TCO_CMD 1
/**
* \def EB_CMD
* \brief Value EB_CMD emergency brake command
*/
#define EB_CMD 1
/**
* \def LEVEL_STM
* \brief Value LEVEL_STM
*/
#define LEVEL_STM 1
/**
* \def LZB_SUPERVISION
* \brief Value  LZB_SUPERVISION
*/
#define  LZB_SUPERVISION 1

/**
* \def MMI_STM_NID_DATA_BRA
* \brief Value MMI_STM_NID_DATA_BRA
*/
#define MMI_STM_NID_DATA_BRA 1
/**
* \def MMI_STM_NID_DATA_BRH
* \brief Value MMI_STM_NID_DATA_BRH
*/
#define MMI_STM_NID_DATA_BRH 2
/**
* \def MMI_STM_NID_DATA_ZL
* \brief Value MMI_STM_NID_DATA_ZL
*/
#define MMI_STM_NID_DATA_ZL 3
/**
* \def MMI_STM_NID_DATA_VMZ
* \brief Value MMI_STM_NID_DATA_VMZ
*/
#define MMI_STM_NID_DATA_VMZ 4
/**
* \def MMI_STM_NID_DATA_ZDA
* \brief Value MMI_STM_NID_DATA_ZDA
*/
#define MMI_STM_NID_DATA_ZDA 5
/**
* \def PZB_LZB
* \brief Value PZB_LZB
*/
#define PZB_LZB 9
/**
* \def NL
* \brief Value NL
*/
#define NL      11
/**
* \def SN
* \brief Value SN
*/
#define SN      13
/**
* \def HMI1_ALIVE
* \brief Value HMI1
*/
#define HMI1_ALIVE 2
/**
* \def HMI2_ALIVE
* \brief Value HMI2
*/
#define HMI2_ALIVE 3
/**
* \def HMI3_ALIVE
* \brief Value HMI3
*/
#define HMI3_ALIVE 4
/**
* \def ACTIVE
* \brief Value active ETC/Lzb and Pzb
*/
#define SIGN_ACTIVE 1
/**
* \def ACTIVE
* \brief Value not active ETC/Lzb and Pzb
*/
#define SIGN_NOT_ACTIVE 2
/**
* \def TEXT_ATP_DOWN_ALARM
* \brief Value of Atp Down Alarm
*/
#define TEXT_ATP_DOWN_ALARM 0x1
/**
* \def ATP_DOWN_ALARM
* \brief Value of Atp Down Alarm
*/
#define ATP_DOWN_ALARM 0x800000U
/**
* \def QUIT_ATP_DOWN_ALARM
* \brief Value of Quit Atp Down Alarm
*/
#define QUIT_ATP_DOWN_ALARM 0x0
/**
* \enum e_Evc21
* \brief value of bitmask of MMI_Q_REQUEST_ENABLE_32
*        data of EVC-21
*/
typedef enum
{
    ENABLE_START_TRAIN_DATA_ENTRY = 0x10LU, /*!<Value of Enable Start Train Data Entry*/
    ENABLE_EXIT_TRAIN_DATA_ENTRY  = 0x20LU, /*!<Value of Enable Exit Train Data Entry*/
    ENABLE_START_TRAIN_DATA_VIEW  = 0x40LU, /*!<Enable Start Train Data View*/
    ENABLE_MMIM_Q_REQUEST         = 0x80LU, /*!<Value of Enable Mmi Q Request Enable*/
    ENABLE_CHANGE_TRAIN_NUMBER    = 0x100LU, /*!<Value of Enable Change Train Number*/
    ENABLE_START_BRAKE_TEST       = 0x00400000LU,/*!<Value of Value of Enable Start Brake Test*/
}e_Evc21;

/**
* \enum e_HmiApproval
* \brief value of bitmask Hmi Approval
*/
typedef enum
{
  HMI_UNKNOWN,       /*!<Value HMI Unknown */
  HMI_APPROVED,      /*!<Value Hmi approved */
  HMI_CANCELLED = 3, /*!<Value Hmi cancelled*/
}e_HmiApproval;

/**
* \enum e_HmiMsgId
* \brief value of Message ID Hmi
*/
typedef enum
{
  MHI_MSGID_UNKNOWN               = 0, /*!<Value unknowny*/
  HMI_MSGID_EINGABE_CANCEL        = 8, /*!<Value Train Data Entry cancel*/
  HMI_MSGID_ZUGADTEN_EINGABE      = 9, /*!<Value Train Data Entry enable*/
  HMI_MSGID_TEXT_CONFIRMED        = 19,/*!<Value Train Data Entry Text Confirmed*/
  HMI_MSG_ZDE_KONTROLL_CANCEL     = 22,/*!<Value Train Data Entry Control cancel*/
  HMI_MSG_ZDE_BESTAETIGUNG        = 25,/*!<Value Train Data Entry set*/
  HMI_MSG_ZD_ANZEIGE_CANCEL       = 26,/*!<Value Train Data View cancel*/
  HMI_MSGID_WARTE_CANCEL          = 27,/*!<Value Train Data Entry wait cancel*/
}e_HmiMsgId;
/**
* \def MASK_ATTRIBUT_LZBPZB_TEST
* \brief Value of Mask attribute PruefenZugBesy_V stm-32
*/
#define MASK_ATTRIBUT_LZBPZB_TEST 0x200U





#endif
