/**
* \file StartPhase.h
* \brief the file manages the start up phase synchronisation
* \author Muoio Salvatore
*/
#ifndef __STAART_PHASE_H__
#define __STAART_PHASE_H__


/**
* \enum e_StartPhase_fsm
* \brief This variable indicates the state of the finite state machines
*        used to manage the start pahse requirement
*/
typedef enum
{
  StartPahse_Idle,     /*!<Idle State     */
  StartPahse_Init,     /*!<Init State      */
  StartPhase_Counter,  /*!<Counter State   */
  StartPhase_End,      /*!<End Phase State */
  StartPhase_Error,    /*!<End Phase State */
}e_StartPhase_fsm;
/**
* \fn void StartPhase_init(void)
* \brief the function initializes the counter
*/
void StartPhase_init(void);
/**
* \fn void StartPahseHandle(void)
* \brief the function manages all state machines regarding the start pahse
* \param interface pointer to the object TYPE_TPGT_Main_Task64_IF
*/
void StartPahseHandle(void);
/**
* \fn e_StartPhase_fsm GetStartPahseStatus(void)
* \brief the function returns the status of startühase finite state machine
* \return the enum e_StartPhase_fsm
*/
e_StartPhase_fsm GetStartPahseStatus(void);

#endif
