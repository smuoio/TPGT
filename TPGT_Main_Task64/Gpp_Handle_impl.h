
/**
* \file Gpp_handle_impl.h
* \brief the file manages the application functions used to create the GPP packet
*        to send via MVB. The cration of this packet is managed in the
*        application task
* \author Muoio Salvatore
*/

#ifndef __GPP_HANDLE_IMPL_H__
#define __GPP_HANDLE_IMPL_H__

/*#include "usertypes.h"*/
#include "Time_Util.h"
/**
* \fn void reset_index_packet2write(void)
* \brief The function reset all index of packet to write
*/
void reset_index_packet2write(void);
/**
* \fn void Create_MmiMStartMmi(void)
* \brief The function creates packet EVC-100 to send via MVB
*/
void Create_MmiMStartMmi(void);
/**
* \fn void Create_MmiStatusReport(const TSG_ETCS_MMI_DYN *pTsgEtcs)
* \brief The function creates packet EVC-102 to send via MVB
*/
void Create_MmiStatusReport(const TSG_ETCS_MMI_DYN *pTsgEtcs);
/**
* \fn void Create_MmiRequest(void)
* \brief The function creates packet EVC-101 to send via MVB
*/
void Create_MmiRequest(void);
/**
* \fn void Create_MmiSpecifiStmDatatoStm(void)
* \brief The function creates packet EVC-123 to send via MVB
*/
void Create_MmiSpecifiStmDatatoStm(void);
/**
* \fn void Create_MmiNewDriverData(void)
* \brief The function creates packet EVC-104 to send via MVB
*/
void Create_MmiNewDriverData(void);
/**
* \fn void Create_MmiDriverMessageAck(void)
* \brief The function creates packet EVC-111 to send via MVB
*/
void Create_MmiDriverMessageAck(void);
/**
* \fn void Create_MmiSetTimeMmi(void)
* \brief The function creates packet EVC-109 to send via MVB
*/
void Create_MmiSetTimeMmi(void);
/**
* \fn t_Time *GetTimeEvc102(void)
* \brief the function returns the address to the class t_time
*/
t_Time *GetTimeEvc102(void);

#endif
