/**
* \file Generic_api.h
* \brief the file manages the application functions API used in the
*        application task
* \author Muoio Salvatore
*/

#ifndef __GENERIC_API_H__
#define __GENERIC_API_H__

#include "TPGT_Main_Task64.h"
#include "Gpp_Handle_Public.h"


/**
* \fn BOOLEAN8 isEbiCabAlive(BOOLEAN8 *isAlive)
* \brief The function detects whether the connections via MVB with EbiCab is
*        stable
* \param isAlive pointer is true if the connection is stable, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isEbiCabAlive(BOOLEAN8 *isAlive);
/**
* \fn BOOLEAN8 isETCSAlive(BOOLEAN8 *isAlive)
* \brief The function detects whether the STM connections via MVB with EbiCab is
*        stable
* \param isAlive pointer is true if the connection is stable, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isETCSAlive(BOOLEAN8 *isAlive);
/**
* \fn BOOLEAN8 isEbiCabOn(BOOLEAN8 *isOn)
* \brief The function detects whether the Ebicab is On or Off. The check is done
*        monitoring the MIO signals sent from CCUO
* \param isOn pointer is true if the ebicab is on, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isEbiCabOn(BOOLEAN8 *isOn);
/**
* \fn BOOLEAN8 isEtcsIsolated(BOOLEAN8 *isIsolated)
* \brief The function detects whether the Etcs is Isolated or not. The check is done
*        monitoring the status signals sent from CCUO.
* \param isIsolated pointer is true if the ebicab is isolated, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isEtcsIsolated(BOOLEAN8 *isIsolated);
/**
* \fn BOOLEAN8 isPzbIsolated(BOOLEAN8 *isIsolated)
* \brief The function detects whether the Pzb is Isolated or not. The check is done
*        monitoring the status signals sent from CCUO.
* \param isIsolated pointer is true if the ebicab is isolated, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isPzbIsolated(BOOLEAN8 *isIsolated);
/**
* \fn BOOLEAN8 isLzbIsolated(BOOLEAN8 *isIsolated)
* \brief The function detects whether the Lzb is Isolated or not. The check is done
*        monitoring the status signals sent from CCUO.
* \param isIsolated pointer is true if the ebicab is isolated, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isLzbIsolated(BOOLEAN8 *isIsolated);
/**
* \fn BOOLEAN8 isHmiAlive(BOOLEAN8 *isAlive)
* \brief The function detects whether the connections via IPT Com with HMI is
*        stable
* \param isAlive pointer is true if the connection is stable, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isHmiAlive(BOOLEAN8 *isAlive);

/**
* \fn BOOLEAN8 isCabActive(UINT8 *isActive)
* \brief The function detects whether the cabcar is active with all informations
*        from CCUO and EBICAB
* \param isActive pointer:0 No Cabin active, 1 Cabin 1 is active
*                         2 Cabin 2 is active, 3 spare
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isCabActive(UINT8 *isActive);
/**
* \fn static e_MmiMMmiStatus SetTcmsSts2EbiCab(void)
* \brief The function detects the status of TCMS to set the EVC 102
* \return  e_MmiMStartReq value
*/

e_MmiMMmiStatus SetTcmsSts2EbiCab(void);
/**
* \fn BOOLEAN8 isStandStill(BOOLEAN8 *aIsStandStill)
* \brief The function detects whether the train is in standstill
* \param aIsStandStill pointer is true if the train in standstill, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isStandStill(BOOLEAN8 *aIsStandStill);
/**
* \fn BOOLEAN8 isCcuoAlive(BOOLEAN8 *isAlive)
* \brief The function detects whether the CCUO is active on the Eth bus
* \param isAlive pointer is true if the connection is stable, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isCcuoAlive(BOOLEAN8 *isAlive);
/**
* \fn BOOLEAN8 isAtpDownAlarmActive(BOOLEAN8 *isAtpDwnActive)
* \brief The function detects whether the ATP alarm is set or reset
* \param isAtpDwnActive pointer is true if the alarm is set, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isAtpDownAlarmActive(BOOLEAN8 *isAtpDwnActive);
/**
* \fn BOOLEAN8 isScreenDel(BOOLEAN8 *isScreenDel)
* \brief The function detects if the screen is deleted
* \param isScreenDel pointer is true if the alarm is set, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isScreenDel(BOOLEAN8 *isScreenDel);
/**
* \fn BOOLEAN8 isIntTestAct(BOOLEAN8 *isAct)
* \brief The function detects whether the integrity test is active
* \param isAct pointer is true if the connection is stable, viceversa FALSE
* \return TRUE if pointer is addressed, viceversa false
*/
BOOLEAN8 isIntTestAct(BOOLEAN8 *isAct);
/**
* \fn BOOLEAN8 isTrainSpeedValid(void)
* \brief The function detects if all speeds(EbiCab and DBC are valid)
* \return TRUE if trainspeed valid, false viceversa
*/
BOOLEAN8 isTrainSpeedValid(void);

#endif
