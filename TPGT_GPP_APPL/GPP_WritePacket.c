/**
* \file GPP_WritePacket.c
* \brief the file manages the functions used to create the packet to output
* \author Muoio Salvatore
*/
#include "GPP_StmPacket.h"
#include "GPP_Interface.h"
#include "GPP_StmPacket_Types.h"

#include "GPP_EvcPacket.h"
#include "GPP_EvcPacket_Types.h"
#include "GPP_Interface.h"

#include "Shared_Memory.h"
#include "GPP_WritePacket.h"
#include "GPP_Types.h"

/**
*\def MAX_EVC_FUNCTION_SIZE
*\brief max number of function in the function Tab
*/
#define MAX_EVC_FUNCTION_SIZE 23
/**
*\def MAX_STM_FUNCTION_SIZE
*\brief max number of function in the function Tab
*/
#define MAX_STM_FUNCTION_SIZE 34
/**
*\def MAX_SIZE_FFFIS_MESSAGE
*\brief max size of array of FFFISMessage
*/
#define MAX_SIZE_FFFIS_MESSAGE 30
/**
* \fn static void Build_Dummy(t_FFFISMessage * const lFFFISMessage)
* \brief dummy function
* \param lFFFISMessage pointer to the t_FFFISMessage packet
*/
static void Build_Dummy(t_FFFISMessage * const lFFFISMessage)
{;}
/**
* \brief state machine used to write the data
*/
typedef void (*handler_evc_sts)(t_FFFISMessage *const lFFFISMEssage);
/**
* \brief state machine used to write the data
*/
typedef void (*handler_stm_sts)(t_FFFISMessage *const lFFFISMEssage);

/**
* \brief static tab of functions used to write the data from CCUT packet
*/
const handler_evc_sts _jumpEvcStsTabFun[] = {Build_MmiStartMmi,
                                            Build_MmiRequest,
                                            Build_MmiStatusReport,
                                            Build_Dummy,
                                            Build_MmiNewDriverData,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_MmiSetTimeMmi,Build_Dummy,
                                            Build_MmiDriverMessageAck,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,
                                            Build_MmiSpecificStmDataToStm};

/**
* \brief static tab of functions used to write the data from CCUT packet
*/
const handler_stm_sts _jumpStmStsTabFun[] = {Build_Dummy, Build_StmVersionNumber,
                                             Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,Build_Dummy,
											 Build_Dummy,Build_Dummy,
                                             Build_MmiStmButtonEventReport};



/**
*\fn static UINT8 LookUpTableNid(UINT8 aNid)
*\brief the function return a state of finite state machine
*\param aNid id of packet
*/
static UINT8 LookUpTableNid(UINT8 aNid)
{
  /* tab working in progress... is possible to add new state*/
  return(((aNid!=0xFF)&&(aNid >= 100)) ? (aNid - 100) : 0xFF);
}

/**
* \fn static UINT8 get_packet2write(UINT8 aPckt, UINT8 ii)
* \brief the function extract the id to write from the database
* \param aPckt is the packet to write, 1 EVC, 2 STM
* \param ii index
*\ return the index valid
*/
static UINT8 get_packet2write(UINT8 aPckt, UINT8 ii)
{
  UINT8 lOut = 0xFF;
  if(aPckt == EVC_Packet_to_Write)
  {
	   if((GetEvcSts()->Pack2Write[ii] != 0xFF) && (GetEvcSts()->Pack2Write[ii] != 0))
	   {
	      lOut = GetEvcSts()->Pack2Write[ii];
	   }/*if*/
  }
  else if(aPckt == STM_Packet_to_Write)
  {
    if((GetStmSts()->Pack2Write[ii] != 0xFF) && (GetStmSts()->Pack2Write[ii] != 0))
  	{
  	     lOut = GetStmSts()->Pack2Write[ii];
  	}/*if*/
  }
  return(lOut);
}


void GPP_CreatePacket2Write(UINT8 aNidStm)
{
  UINT8 lIsEvcStsMaster = 0;
  UINT8 lIsStmStsMaster = 0;
  /* read the shared memory and store the data in a static object evc_sts */
  sm_recv_stm_sts(STM_STS_RECV, GetStmSts(), &lIsStmStsMaster);
  /* read the shared memory and store the data in a static object evc_sts */
  sm_recv_evc_sts(EVC_STS_RECV, GetEvcSts(), &lIsEvcStsMaster);
  /* call functions used to add the packet in the queue to store*/
  GPP_BuildPacket2Queue(aNidStm);
}



void GPP_BuildPacket2Queue(UINT8 aNidStm)
{
  INT32 index_message = 0;
  INT32 ii = 0; /*index used in the for*/
  /*const INT32 size = 1200;*/
	t_FFFISMessage lFFFISMEssage[MAX_SIZE_FFFIS_MESSAGE];
  /*reset packet!!!*/
  memset(&lFFFISMEssage[0], 0, sizeof(t_FFFISMessage));
	/*I have to use the FFFISMessage t_FFFISMessage*/
  /*EVC packet to write*/
	for(ii = 0; ii < NUM_PACKET2WRITE; ii++)
	{
		UINT8 lNid = 0U;
		/*read the id of packet to write*/
		lNid = LookUpTableNid(get_packet2write(EVC_Packet_to_Write, ii));
		if((lNid != 0xFF) && (lNid <= MAX_EVC_FUNCTION_SIZE))
		{
			_jumpEvcStsTabFun[lNid](&lFFFISMEssage[index_message++]);
    }/**/
  }/*for evc packet*/
  /*STM packet to write*/
  for(ii = 0; ii < NUM_STMPACKET2WRITE; ii++)
	{
		UINT8 lNid = 0U;
		/*read the id of packet to write*/
		lNid = get_packet2write(STM_Packet_to_Write, ii);
		if((lNid != 0xFF) && (lNid <= MAX_STM_FUNCTION_SIZE))
		{
			_jumpStmStsTabFun[lNid](&lFFFISMEssage[index_message]);
      lFFFISMEssage[index_message].NidStm = 9;
      index_message++;
    }
  }/*for stm*/
  /* add the packets in queue*/
  for(ii = 0; ii < index_message; ii++)
  {
    _store_mmi_data((UINT8*)&lFFFISMEssage[ii].Buff[0],
                          lFFFISMEssage[ii].NidStm,
                          lFFFISMEssage[ii].NumBytes);
  }/*for*/
  /*copy in the debug buffer!!!*/
}
