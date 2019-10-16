/**
* \file CcutGpp_Appl.h
* \brief the file manages the api public of GPP used in the project*
* \author Muoio Salvatore
*/


#ifndef __CCUTGPP_APPL_H__
#define __CCUTGPP_APPL_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "CCUT_Util.h"
#include "GPP_Types.h"
#include "usertypes.h"


/**
*\def CABIN
*\brief define how many cabs in the train
*/
#define CABIN 1
/**
*\def CTRL_LOCAL_PORT
*\brief MVB port address of local control and data port
*/
#define CTRL_LOCAL_PORT 810
/**
*\def DATA_PORT_1
*\brief MVB port address of the 1st data port
*/
#define DATA_PORT_1 811
/**
*\def DATA_PORT_2
*\brief MVB port address of the 2st data port
*/
#define DATA_PORT_2 812
/**
*\def CTRL_REMOTE_PORT
*\brief MVB port address of remote control and data port
*/
#define CTRL_REMOTE_PORT 820

/**
* \class t_imported_TsgMmi_telegram
* \brief local structure with the gpp telegram imported
*/
typedef struct
{
	UINT8 TPG_TSG_MMI_F1_P1[MAX_LEN_PORT]; /*!< local copy of input dataset TPG_TSG_MMI_F1_P1*/
	UINT8 TPG_TSG_MMI_F1_P2[MAX_LEN_PORT]; /*!< local copy of input dataset TPG_TSG_MMI_F1_P2*/
	UINT8 TPG_TSG_MMI_F1_P3[MAX_LEN_PORT]; /*!< local copy of input dataset TPG_TSG_MMI_F1_P3*/
	UINT8 TPG_MMI_TSG_F1[MAX_LEN_PORT];    /*!< local copy of output dataset TPG_TSG_MMI_F1_P1*/
}t_imported_TsgMmi_telegram;

/**
* \var t_imported_TsgMmi_telegram Tg
* \brief variable to the static objec t_imported_TsgMmi_telegram
*/
extern t_imported_TsgMmi_telegram Tg;

/**
*\fn void CcutGpp_init(void)
*\brief init function
*/
void CcutGpp_init(void);
/**
*\fn void GPP_CopyLocal(BYTE *pCtrlLoc, BYTE *pDataPort1, BYTE *pDataPort2)
*\brief function copy the control/data port local
*\param pCtrlLoc pointer to MVB port address of local control and data port.
*\param pDataPort1 pointer to MVB port address of the 1st local data port.
*\param pDataPort2 pointer to MVB port address of the 2st local data port.
*/
void GPP_CopyLocal(BYTE *pCtrlLoc, BYTE *pDataPort1, BYTE *pDataPort2);
/**
*\fn BOOLEAN8 GPP_isDataInitialized(void)
*\brief the function checks if the sw component is initialized
*/
BOOLEAN8 GPP_isDataInitialized(void);
/**
*\fn GPP_SetDataInitialized
*\brief the function sets the sw component initialized
*/
void GPP_SetDataInitialized(void);
/**
*\fn UINT8 GetGPPData(BOOLEAN8 aDebug)
*\brief function wrapper that manages the functions to read on MVB via GPP
*\param aDebug value used to enable the print of debug.
*/
UINT8 GetGPPData(BOOLEAN8 aDebug);
/**
*\fn UINT8 PutGPPData(BOOLEAN8 aDebug)
*\brief function wrapper that manages the functions to write on MVB via GPP
*\param aDebug value used to enable the print of debug.
*/
UINT8 PutGPPData(BOOLEAN8 aDebug);
/**
*\fn e_result GPP_UpdateOut(BYTE *p2Out)
*\brief the function update the output dataset 
*\param p2Out pointer used to write the output
*/
e_result GPP_UpdateOut(BYTE *p2Out);

#ifdef __cplusplus
}
#endif

#endif
