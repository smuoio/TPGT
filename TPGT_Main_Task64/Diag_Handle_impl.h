/**
* \file Diag_Handle_impl.h
* \brief the file implements the private diagnostic functions
* \author Muoio Salvatore
*/

#ifndef __DIAG_HANDLE_IMPL_H__
#define __DIAG_HANDLE_IMPL_H__

/**
* \struct t_DiagLegacy
* \brief diagnostic information used only in the diag_handle component
*/
typedef struct
{
  INT32 SDTDiagCCUO2CCUT; /*!< Diagnostic from SDT layer CCUO2CCUT */
  INT32 SDTDiagCCUT2CCUO; /*!< Diagnostic from SDT layer CCUT2CCUO */
  INT32 SDTDiagCCUT2HMI; /*!< Diagnostic from SDT layer CCUT2HMI */
}t_DiagLegacy;

/**
* \fn t_DiagLegacy *GetDiagLegacy(void)
* \brief the function return the address to the structure
*/
t_DiagLegacy *GetDiagLegacy(void);

/**
* \fn void CommEvent_Handle(void)
* \brief the function manages the communication events
*/
void CommEvent_Handle(void);
/**
* \fn void SpeedEvent_Handle(void)
* \brief the function manages the speed events
*/
void SpeedEvent_Handle(void);





#endif
