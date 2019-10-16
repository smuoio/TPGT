/**
* \file GPP_ParserPacket.h
* \brief the file manages the functions private of GPP to parser the packets
* \author Muoio Salvatore
*/

#ifndef __GPP_PARSER_PACKET_H__
#define __GPP_PARSER_PACKET_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "GPP_Types.h"

typedef UINT8 (L_Packet)[2];
typedef UINT8 (PktData_)[MAX_LEN_GPP_PACK];
/**
* \struct t_Header
*\brief structure Header of FFFIS Message
*/
typedef struct
{
	UINT8    NidPacket; /*!< type Nid Stm*/
	L_Packet Packet;    /*!< type of length of the FFFIS packet in bits*/
}t_Header;
/**
* \struct t_EvcPktHeader
*\brief structure Header of FFFIS Packet
*/
typedef struct
{
	UINT8    MmiMPacket; /*!< type of EVC packet number*/
	L_Packet MmiLPacket; /*!< type of EVC packet length in bits*/
}t_EvcPktHeader;

/**
* \struct t_FFFISPacket
*\brief structure FFFIS packet(trasmitted via GPP),
*       originated by the EVC and routed via the TSG to the MMI
*/
typedef struct
{
	t_Header       Header;    /*!< FFFIS Packet Header */
	t_EvcPktHeader EvcHeader; /*!< EVC Packet Header */
	PktData_       PktData;   /*!< EVC Packet data */
	UINT8          EOStream;  /*!< End of stream Delimiter */
}t_FFFISPacket;

/**
* \struct t_StmPacket
*\brief structure STM Data packet contains several data packet.
*/
typedef struct
{
	t_Header       Header;  /*!< STM Packet Header */
	PktData_       PktData; /*!< STM Packet data */
}t_StmPacket;

/**
* \struct t_FFFISMessage
* \brief structure FFFIS message(trasmitted via GPP),
*       originated by the EVC and routed via the TSG to the MMI
*/
typedef struct
{
	UINT8  NidStm;    /*!< Nid stm*/
	UINT16 NumBytes;  /*!< Numbers of byte*/
	BYTE  Buff[1202];/*!< data*/
}t_FFFISMessage;


/**
* \fn gpp_result GPP_GetNidStm(const UINT8 * const pTg, UINT8 * const pNidStm)
* \brief function returns the value of NID_STM located in the FFFIS Message Header
* \param pTg: const pointer to the const object UINT8, is the ppointer to the raw data
* \param pNidStm: pointer used to store the value of NID_STM
* \return 0 success or number and type of error
*/
gpp_result GPP_GetNidStm(const UINT8 * const pTg, UINT8 * const pNidStm);
/**
* \fn gpp_result GPP_GetLMsgStm(const UINT8 * const pTg, UINT8 * const pLMsg)
* \brief function returns the length of the whole message in byte
*        located in the FFFIS Message Header
* \param pTg: const pointer to the const object UINT8, is the ppointer to the raw data
* \param pLMsg: pointer used to store the value of lngth
* \return 0 success or number and type of error
*/
gpp_result GPP_GetLMsgStm(const UINT8 * const pTg, UINT8 * const pLMsg);
/**
* \fn void GPP_CreateEvcPacket(const UINT8 *const pTg, UINT8 aNidStm)
* \brief function that read the raw data and creates a packet
*       used to parser the EVC/TSG data.
* \param pTg: const pointer to the raw data
* \param aNidStm: value of NID_STM, is used for the plausibility
* \return 0 success or number and type of error
*/
void GPP_CreateEvcPacket(const UINT8 *const pTg, UINT8 aNidStm);
/**
* \fn void GPP_CreateStmPacket(const UINT8 *const pTg, UINT8 aLenMsg)
* \brief function that read the raw data and creates a packet
*       used to parser the STM data.
* \param pTg: const pointer to the raw data
* \param aLenMsg: length of frame data
* \return 0 success or number and type of error
*/
void GPP_CreateStmPacket(const UINT8 *const pTg, UINT8 aLenMsg);
/**
* \fn BOOLEAN8 GPP_CreateEvcStsPacket(UINT8* const pTg)
* \brief function that read the data from the shared memory and store these
*       packet using the api of tsg: gpp_qm_store_mmi_data.
* \param pTg: const pointer to the raw data
* \return 0 success or number and type of error
*/
BOOLEAN8 GPP_CreateEvcStsPacket(UINT8* const pTg);
/**
* \fn void GPP_CreateStmStsPacket(UINT8* const pTg, UINT8 aNidStm)
* \brief function that read the data from the shared memory and store these
*       packet using the api of tsg: gpp_qm_store_mmi_data.
* \param pTg: const pointer to the raw data
* \return 0 success or number and type of error
*/
void GPP_CreateStmStsPacket(UINT8* const pTg, UINT8 aNidStm);

/* interface functions*/
/**
* \fn t_FFFISPacket *GetFFFISPacket(void)
* \brief function used to point the interface EVC
* \return the address of object t_FFFISPacket
*/
t_FFFISPacket *GetFFFISPacket(void);
/**
* \fn t_StmPacket *GetStmPacket(void)
* \brief function used to point the interface STM
* \return the address of object t_StmPacket
*/
t_StmPacket *GetStmPacket(void);
/**
* \fn UINT16 (*_store_mmi_data)(UINT8 *pTg, UINT8 nid_stm, UINT16 len)
* \brief pointer function addressed to api of gpp:gpp_qm_store_mmi_data
*       this function is called after the engine has created the püacket to put out
* \param pTg pointer of application packet to put out
* \param nid_stm identifier of sender(STM, TSG, ATP)
* \param len numebr of byte of application data
* \return the code of error
*/
UINT16 (*_store_mmi_data)(UINT8 *pTg, UINT8 nid_stm, UINT16 len);

BOOLEAN8 CheckPackets(void);

#ifdef __cplusplus
}
#endif

#endif
