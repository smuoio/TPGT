/**
* \file GPP_StmPacket.c
* \brief the file manages the functions private of GPP to extract the packets STM
* \author Muoio Salvatore
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include "GPP_StmPacket.h"
#include "GPP_Interface.h"
#include "GPP_StmPacket_Types.h"
#include "Shared_Memory.h"
#include "GPP_EvcStmPublic.h"

/**
*\fn static void Create_Dummy(const t_FFFISPacket * const lFFFISPacket)
*\brief dummy function
*\param lStmPacket pointer to the Stm packet
*/
static void Create_Dummy(const t_StmPacket * const lStmPacket)
{;}
/**
* \fn static void Build_Dummy(t_FFFISMessage * const lFFFISMessage)
* \brief dummy function
* \param lFFFISMessage pointer to the t_FFFISMessage packet
*/
static void Build_Dummy(t_FFFISMessage * const lFFFISMessage)
{;}
/**
*\brief state machine used to extract the data from parser
*/
typedef void (*handler_stm)(const t_StmPacket * const lStmPacket);
/**
* \brief state machine used to write the data
*/
typedef void (*handler_stm_sts)(t_FFFISMessage *const lFFFISMEssage);

/**
*\brief static tab of functions used to extract the data from EVC packet
*/
const  handler_stm jumpStmTabFun[]= {Create_Dummy, Create_VersionNumber,
                                     Create_StateReport,
                                     Create_ButtonRequest,
                                     Create_MmiStmTextMessage,
                                     Create_MmiStmDeleteTextMessage,
                                     Create_MmiStmSoundCmd,
                                     Create_MmiStmIndicatorRequest};


/**
* \brief static tab of functions used to write the data from CCUT packet
*/
const handler_stm_sts jumpStmStsTabFun[] = {Build_Dummy, Build_StmVersionNumber};
/**
*\fn static UINT8 LookUpTableNid(UINT8 aNid)
*\brief the function return a state of finite state machine
*\param aNid id of packet
*/
static UINT8 LookUpTableNid(UINT8 aNid)
{
  /* tab working in progress... is possible to add new state*/
  static const UINT8 Tab[] = {0,STM_VersionNumber,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              STM_StateReport,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                              0,STM_ButtonRequest,0,0,
                              STM_MmiStmIndicatorRequest,0,0,
                              STM_MmiStmTextMessage,
                              STM_MmiStmDeleteTextMessage,0,0,0,0,0,0,
                              STM_MmiStmSoundCmd};
  return(Tab[aNid]);
}

void GPP_ExtractStmPacket(void)
{
	/* use the packet FFFIS created*/
	t_StmPacket *lStmPacket = GetStmPacket();
  /*reset stm_packet*/
	if(lStmPacket != 0)
	{
		/*collect information to extract the packet*/
		{
			UINT8 lNidPckt = lStmPacket->Header.NidPacket;
      /*if(lNidPckt <= 6)*/
			jumpStmTabFun[LookUpTableNid(lNidPckt)](lStmPacket);
		}/*if*/
	}/*if*/
}

void reset_index_stm_packet(void)
{
  memset(&GetStmData()->stm_packet[0], 0, sizeof(GetStmData()->stm_packet));
  memset(&GetStmData()->diag_stm[0],   0, sizeof(GetStmData()->diag_stm));
  /*reset also in the shared memory*/
  sm_reset_recv_stm_index();
}

/**
* \fn static UINT8 get_packet2write(UINT8 ii)
* \brief the function extract the id to write from the database
* \param ii index
*/
static UINT8 get_packet2write(UINT8 ii)
{
  UINT8 lOut = 0xFF;
	if((GetStmSts()->Pack2Write[ii] != 0xFF) && (GetStmSts()->Pack2Write[ii] != 0))
	{
	     lOut = GetStmSts()->Pack2Write[ii];
	}/*if*/
  return(lOut);
}

void GPP_BuildStmPacket(UINT8* const pTg, UINT8 aNidStm)
{
  UINT16 ii  = 0U;
  const INT32 size = 1200;
	t_FFFISMessage lFFFISMEssage[NUM_STMPACKET2WRITE];
	/*I have to use the FFFISMessage t_FFFISMessage*/
	for(; ii < NUM_STMPACKET2WRITE; ii++)
	{
		UINT8 lNid = 0U;
		memset(&lFFFISMEssage[ii], 0, sizeof(t_FFFISMessage));
		/*read the id of packet to write*/
		lNid = get_packet2write(ii);
		if((lNid != 0xFF) && (lNid <= NUM_STMPACKET2WRITE))
		{
			jumpStmStsTabFun[lNid](&lFFFISMEssage[ii]);
      lFFFISMEssage[ii].NidStm = aNidStm;
			_store_mmi_data((UINT8*)&lFFFISMEssage[ii].Buff[0],
                            lFFFISMEssage[ii].NidStm,
                            lFFFISMEssage[ii].NumBytes);
		}/*if*/
	}/*for*/
  memcpy((UINT8*)pTg, (t_FFFISMessage*)&lFFFISMEssage[0], size);
}



#ifdef __cplusplus
}
#endif
