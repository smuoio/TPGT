/**
* \file CcutGpp_impl.h
* \brief the file manages the functions private of GPP Api
* \author Muoio Salvatore
*/


#ifndef __MVBGPP_INPUT64_IMPL_H__
#define __MVBGPP_INPUT64_IMPL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "CCUT_Util.h"
#include "GPP_Types.h"

/**
* \struct t_RowDataEntry
* \brief Row data structure used to read and write the data via GPP
*/
typedef struct
{
	BYTE DataInp[MAX_LEN_GPP_PACK + 2]; /*!< local copy of input  */
	BYTE DataOut[MAX_LEN_GPP_PACK + 2]; /*!< local copy of output */
}t_RowDataEntry;

/**
* \fn void Gpp_InvalidateData(t_RowDataEntry * const pData)
* \brief function used to invalidate the data
* \param *pData object to row data
*/
void Gpp_InvalidateData(t_RowDataEntry * const pData);
/**
* \fn t_RowDataEntry *GetDataEntry(void)
* \brief function used as interface of object t_RowDataEntry
* \return the static address of object
*/
t_RowDataEntry *GetDataEntry(void);
/**
* \fn gpp_result GPP_PacketConsistency(UINT16 aPcktLen)
* \brief function check the consistency of the incoming packet MMI->TSG
* \param aPcktLen pointer to remote control port
* \return 0 success or number and type of error
*/
gpp_result GPP_PacketConsistency(UINT16 aPcktLen);
/**
* \fn void GPP_Prepare2StorageData(void)
* \brief function store the address of MVB ports using the apii function gpp_init
*/
void GPP_Prepare2StorageData(void);
/**
* \fn e_result GPP_ReadTSGQueue(UINT16 *pLenData)
* \brief function reads the frame in raw data
* \param pLenData length of raw frame
* \return 0 success or number and type of error
*/
e_result GPP_ReadTSGQueue(UINT16 *pLenData);
/**
* \fn void GPP_ReadPacket(UINT8 *pBuff)
* \brief the function reads the packet via GPP, checkin the NID is possible
*        exatract the pachet EVC or STM. This functin is the main to read and
*        extract
* \param *pBuff pointer to the buffer
*/
void GPP_ReadPacket(UINT8 *pBuff);
/**
* \fn void GPP_StoreMmiDataInit(void)
* \brief the function init used to assigne the address to pointer function
*/
void GPP_StoreMmiDataInit(void);
/**
* \fn void GPP_StoreMmiData(BOOLEAN8 aDebug)
* \brief the function is main function used to store the packet to put out in the
*        queue of GPP protocol
* \param aDebug, external signal to enable the debug
*/
void GPP_StoreMmiData(BOOLEAN8 aDebug);
/**
* \fn e_result CopyTelegramEntryLocal(BYTE *ptrds1, BYTE *ptrds2, BYTE *ptrds3)
* \brief function used to copy the local control and local port for the GPP
* \param ptrds1: pointer to local control port
* \param ptrds2: pointer to loca data port
* \param ptrds3: pointer to loca data port
* \return 0 success or number and type of error
*/
e_result CopyTelegramEntryLocal(BYTE *ptrds1, BYTE *ptrds2, BYTE *ptrds3);
/**
* \fn BOOLEAN8 CheckData2Send(void)
* \brief the function checks the data before to send
*/
BOOLEAN8 CheckData2Send(void);
#ifdef __cplusplus
}
#endif


#endif
