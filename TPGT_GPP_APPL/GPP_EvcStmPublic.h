/**
* \file GPP_EvcStmPublic.h
* \brief the file manages the functions public of Evc and Stm called in first layer of lib
* \author Muoio Salvatore
*/

#ifndef __GPP_EVC_STM_PUBLIC_H__
#define __GPP_EVC_STM_PUBLIC_H__

/**
* \fn void reset_index_evc_packet(void)
* \brief the function reset the index of evc packet incoming
*/
void reset_index_evc_packet(void);
/**
* \fn static UINT8 get_packet2write(UINT8 ii)
* \brief the function reset the index of stm packet incoming
*/
void reset_index_stm_packet(void);

#endif
