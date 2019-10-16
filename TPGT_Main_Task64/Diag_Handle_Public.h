/**
* \file Diag_Handle_Public.h
* \brief the file implements the private diagnostic functions
* \author Muoio Salvatore
*/

#ifndef __DIAG_HANDLE_PUBLIC_H__
#define __DIAG_HANDLE_PUBLIC_H__

/**
* \def DIAG_OK
* \brief SDT diagnostic ok
*/
#define DIAG_OK 0x0

/**
* \struct t_CommEth
* \brief field of Eth communication list
*/
typedef struct
{
  BOOLEAN8 F_Ccuo2Ccut;   /*!< failure of dataset CCUO_CCUT*/
  BOOLEAN8 F_HMI1;        /*!< failure of dataset HMI1_CCUT*/
  BOOLEAN8 F_HMI2;        /*!< failure of dataset HMI2_CCUT*/
  BOOLEAN8 F_SDTCCCUO;    /*!< failure SDT communication CCUO2CCUT*/
  BOOLEAN8 F_SDTCCCUT;    /*!< failure SDT communication CCUT2CCUO*/
  BOOLEAN8 F_SDTHMI;      /*!< failure SDT communication CCUT2HMI*/
}t_CommEth;

/**
* \struct t_CommMVB
* \brief field of Mvb communication list
*/
typedef struct
{
  BOOLEAN8 F_TSG_P0;   /*!< failure of dataset TSG_P0*/
  BOOLEAN8 F_TSG_P1;   /*!< failure of dataset TSG_P1*/
  BOOLEAN8 F_TSG_P2;   /*!< failure of dataset TSG_P2*/
  BOOLEAN8 F_TSG_P3;   /*!< failure of dataset TSG_P3*/
  BOOLEAN8 F_TSG_STM;  /*!< failure of dataset TSG_STM*/
  BOOLEAN8 F_TSG_ETCS; /*!< failure of dataset TSG_ETCS*/
  BOOLEAN8 F_JRU;      /*!< failure of dataset JRU_STATUSS*/
}t_CommMVB;
/**
* \struct t_CommList
* \brief field of communication list
*/
typedef struct
{
  t_CommMVB CommMVB; /*!< MVB list of failure*/
  t_CommEth CommEth; /*!< Eth list of failure*/
}t_CommList;
/**
* \struct t_EventList
* \brief field of Even list
*/
typedef struct
{
  t_CommList CommList;        /*!< ommunication list */
  BOOLEAN8  F_Speed;          /*!< Speed Failure */
  BOOLEAN8  F_CesaSupervisor; /*!< Cesa Failure */
}t_EventList;

/**
* \fn t_EventList *GetEventList(void)
* \brief the function returns the address of the object Event list
* \return address of t_EventList
*/
t_EventList *GetEventList(void);


#endif
