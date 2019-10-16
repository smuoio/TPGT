/**
* \file GPP_EvcPacket.c
* \brief the file manages the functions private of GPP to extract the packets EVC
* \author Muoio Salvatore
*/


#ifdef __cplusplus
extern "C"
{
#endif

#include "GPP_EvcPacket.h"
#include "GPP_EvcPacket_Types.h"
#include "GPP_Interface.h"
#include "Shared_Memory.h"
#include "GPP_EvcStmPublic.h"

/**
* \fn static void Create_Dummy(const t_FFFISPacket * const lFFFISPacket)
* \brief dummy function
* \param lFFFISPacket pointer to the FFFIS packet
*/
static void Create_Dummy(const t_FFFISPacket * const lFFFISPacket)
{;}
/**
* \fn static void Build_Dummy(t_FFFISMessage * const lFFFISMessage)
* \brief dummy function
* \param lFFFISMessage pointer to the t_FFFISMessage packet
*/
static void Build_Dummy(t_FFFISMessage * const lFFFISMessage)
{;}
/**
* \fn static UINT8 get_packet2write(UINT8 ii)
* \brief the function extract the id to write from the database
* \param ii index
*/
static UINT8 get_packet2write(UINT8 ii)
{
  UINT8 lOut = 0xFF;/*local output*/
  /*t_evc_sts *lEvcSts = GetEvcSts();*/
	if((GetEvcSts()->Pack2Write[ii] != 0xFF) && (GetEvcSts()->Pack2Write[ii] != 0))
	{
	   lOut = GetEvcSts()->Pack2Write[ii];
	}/*if*/
  return(lOut);
}

/**
* \typedef typedef void (*handler_evc)(const t_FFFISPacket * const lFFFISPacket)
* \brief state machine used to extract the data from parser
*/
typedef void (*handler_evc)(const t_FFFISPacket * const lFFFISPacket);
/**
* \typedef typedef void (*handler_evc_sts)(t_FFFISMessage *const lFFFISMEssage);
* \brief state machine used to write the data
*/
typedef void (*handler_evc_sts)(t_FFFISMessage *const lFFFISMEssage);
/**
* \brief static tab of functions used to extract the data from EVC packet
*/
const  handler_evc jumpEvcTabFun[]= {Create_MmiStartAtp,Create_Dummy,
	                                   Create_MmiStatus,Create_MmiSetTimeAtp,
									                   Create_MmiTrackDescription,Create_Dummy,
									                   Create_MmiCurrentTrainData,Create_Dummy,
									                   Create_MmiDriverMessage,
									                   Create_MmiFailureReportAtp,Create_Dummy,
									                   Create_Dummy,Create_Dummy,Create_Dummy,
									                   Create_MmiCurrentDriverData,Create_Dummy,
                                     Create_Dummy,Create_Dummy,Create_Dummy,
                                     Create_Dummy,Create_Dummy,
									                   Create_MmiEnableDriverRequest,Create_Dummy,
                                     Create_Dummy,Create_Dummy,
                                     Create_MMiSpecificStmDeRequest, Create_MmiSpecificStmDwValues,
                                     Create_Dummy, Create_Dummy, Create_Dummy, Create_Dummy,
                                     Create_Dummy, Create_Dummy};


/**
* \brief static tab of functions used to write the data from CCUT packet
*/
const handler_evc_sts jumpEvcStsTabFun[] = {Build_MmiStartMmi,
                                            Build_MmiRequest,
                                            Build_MmiStatusReport,
                                            Build_Dummy,
                                            Build_MmiNewDriverData,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_MmiDriverMessageAck,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,Build_Dummy,
                                            Build_Dummy,
                                            Build_MmiSpecificStmDataToStm};

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
*\fn static BOOLEAN8 CheckPacket(UINT8 aMmiPacket)
*\brief the function implements the plausibility check of evc packets number
*\param aMmiPacket is the number of packet
*\return TRUE if the packet is in the range, otherwise false
*/
static BOOLEAN8 CheckPacket(UINT8 aMmiPacket)
{
  return((aMmiPacket <= 30) ? TRUE : FALSE);
}

void GPP_ExtractEvcPacket(UINT8 aNidStm)
{
	/* use the packet FFFIS created*/
	t_FFFISPacket *lFFFISPacket = GetFFFISPacket();
	if(lFFFISPacket != 0)
	{
    /*collect information to extract the packet*/
		if(aNidStm == NID_EVC)
		{
			UINT8 lMMiPacket = (UINT8)(lFFFISPacket->EvcHeader.MmiMPacket);
      /*add check !!!*/
      if(CheckPacket(lMMiPacket) == TRUE)
			   jumpEvcTabFun[lMMiPacket](lFFFISPacket);
		}/*if*/
	}/*if*/
}

void GPP_BuildEvcPacket(UINT8 * const pTg)
{
	UINT16 ii  = 0U; /*local counter*/
  const INT32 size = 1200; /*size const*/
	t_FFFISMessage lFFFISMEssage[NUM_PACKET2WRITE];
	/*I have to use the FFFISMessage t_FFFISMessage*/
	for(; ii < NUM_PACKET2WRITE; ii++)
	{
		UINT8 lNid = 0U;
		memset(&lFFFISMEssage[ii], 0, sizeof(t_FFFISMessage));
		/*read the id of packet to write*/
		lNid = LookUpTableNid(get_packet2write(ii));
		if((lNid != 0xFF) && (lNid <= 23))
		{
			jumpEvcStsTabFun[lNid](&lFFFISMEssage[ii]);
			_store_mmi_data((UINT8*)&lFFFISMEssage[ii].Buff[0],
                            lFFFISMEssage[ii].NidStm,
                            lFFFISMEssage[ii].NumBytes);
		}/*if*/
	}/*for*/
  memcpy((UINT8*)pTg, (t_FFFISMessage*)&lFFFISMEssage[0], size);
}

void reset_index_evc_packet(void)
{
  memset(&(GetEvcData()->evc_packet[0]), 0, sizeof(GetEvcData()->evc_packet));
  memset(&(GetEvcData()->diag_evc[0]),   0, sizeof(GetEvcData()->diag_evc));
  /*reset also in the shared memory*/
  sm_reset_recv_evc_index();
}


#ifdef __cplusplus
}
#endif
