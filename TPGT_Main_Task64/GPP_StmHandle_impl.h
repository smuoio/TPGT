/**
* \file Gpp_StmHandle_impl.h
* \brief the file manages the application functions used to create the GPP STM packet
*        to send via MVB. The cration of this packet is managed in the
*        application task
* \author Muoio Salvatore
*/

#ifndef __GPP_STM_HANDLE_IMPL_H__
#define __GPP_STM_HANDLE_IMPL_H__

/**
* \fn void reset_stm_index_packet2write(void)
* \brief The function reset all index of packet to write
*/
void reset_stm_index_packet2write(void);
/**
* \fn void Create_StmVersionNumber(void)
* \brief The function reports the echo of STM-1
*/
void Create_StmVersionNumber(void);
/**
* \fn void Create_MmiStmButtonEventReport(void)
* \brief The function reports the echo of STM-34
*/
void Create_MmiStmButtonEventReport(void);
/**********************interface***************/
/**
* \fn BOOLEAN8 isReceivedSTM32(void)
* \brief interface funtion
* \return TRUE recv STM-32, FALSE otherwise
*/
BOOLEAN8 isReceivedSTM32(void);
/**
* \fn BOOLEAN8 isReceivedSTM35(void)
* \brief interface funtion
* \return TRUE recv STM-35, FALSE otherwise
*/
BOOLEAN8 isReceivedSTM35(void);
/**
* \fn BOOLEAN8 isReceivedSTM46(void)
* \brief interface funtion
* \return TRUE recv STM-46, FALSE otherwise
*/
BOOLEAN8 isReceivedSTM46(void);
/**
* \fn BOOLEAN8 isReceivedSTM15(void)
* \brief interface funtion
* \return TRUE recv STM-15, FALSE otherwise
*/
BOOLEAN8 isReceivedSTM15(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the 058 sw number.
*/
UINT8 GetMmiStmN058Vermajor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the 058 sw number.
*/
UINT8 GetMmiStmN058Vermid(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the 058 sw number.
*/
UINT8 GetMmiStmN058Verminor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the 035 sw number.
*/
UINT8 GetMmiStmN035Vermajor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the 035 sw number.
*/
UINT8 GetMmiStmN035Vermid(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the 035 sw number.
*/
UINT8 GetMmiStmN035Verminor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the sw number.
*/
UINT8 GetMmiStmNSrsVermajor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface funtion
* \return the value the sw number.
*/
UINT8 GetMmiStmNSrsVerminor(void);




#endif
