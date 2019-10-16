/**
* \file Hmi_TextMessage_Public.h
* \brief the file manages the public functions to get and set data from to CCUO.
* \author Muoio Salvatore
*/

#ifndef __HMIM_TEXTMESSAGE_PUBLIC_H__
#define __HMIM_TEXTMESSAGE_PUBLIC_H__

/**
* \fn BOOLEAN8 Get_NoSetSBVRequest(void)
* \brief The function returns the condition to avoid to send the feedback to EBiCab
* \param TRUE if the is active the request of SB, viceversa FALSE
*/
BOOLEAN8 Get_NoSetSBVRequest(void);

#endif
