/**
* \file Gpp_handle.h
* \brief the file manages the application the interface functions with the
*        the appluication functions used to write/read the GPP data. The use
*        of thiese functions is in the application task
* \author Muoio Salvatore
*/


#ifndef __GPP_HANDLE_H__
#define __GPP_HANDLE_H__

/*#include "usertypes.h"*/
#include "GPP_Handle_Public.h"
/**
* \fn void InvalidateDataOut(void)
* \brief the function invalidate the data used to write on MVB. Each cycle this
*        function is called to invalidate all data
*/

void InvalidateDataOut(void);
/**
* \fn void Gpp_Handle(void);
* \brief main function of application data from/to Gpp. The function manages
*        the function to create the frame to output and data inputs used
*        for the requirements
* \return null
*/
void Gpp_Handle(const TSG_ETCS_MMI_DYN *pTsgEtcs);
/**
* \fn void GPP_Handle_init(void)
* \brief main function initializes the timer and other
* \return null
*/
void GPP_Handle_init(void);

#endif
