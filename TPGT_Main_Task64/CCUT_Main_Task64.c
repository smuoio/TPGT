/**
* \file CCUT_Main_Task64.c
* \brief: In this file there´s the main function of the task 64.
* \author Salvatore Muoio
*/


#include "TPGT_Main_Task64.h"
#include "CCUT_Main_Task64_public.h"
#include "Generic_api.h"
#include "Time_Def.h"
#include "Mvb_Input64_impl.h"
#include "Gpp_Handle.h"
#include "Ccuo_Handle.h"
#include "Cesa_Handle.h"
#include "SpeedHandle.h"
#include "DebugTask64.h"
#include "StartPhase.h"
#include "ETCS.h"
#include "TractionBrake.h"
#include "Hmi_Handle.h"
#include "Ccuo_Handle.h"
#include "Diag_Handle.h"

/**
*\brief: main of task 64 ms
*/
void TPGT_Main_Task64(TYPE_TPGT_MAIN_TASK64_IF *interface)
{
	e_result lChk = TPG_SUCCESS;
	lChk |= checkOutputArg((TYPE_TPGT_MAIN_TASK64_IF*)interface);
	if(TPG_SUCCEEDED(lChk))
	{
		if(interface->Ready == TRUE)
		{
			/*variables used to check out the data from shm*/
			UINT8 lIsEvcMaster = 0x0U;
			UINT8 lIsStmMaster = 0x0U;

			/* read the data in the shared memory*/
			sm_recv_evc(EVC_RECV, GetApplEvcData(), &lIsEvcMaster);
			sm_recv_stm(STM_RECV, GetApplStmData(), &lIsStmMaster);
			/*assign to static variable the buffer from CCUO*/
			Ccuo_Handle_input(&interface->oCCUO_CCUT);
			/*assign to static variable the buffer from CCUO*/
			/* hmi_recv*/
			/* function call to framework debug for the shared memory data*/
			DebugTask64(interface);
			/* application functions */
			Cesa_Handle(interface);
			StartPahseHandle();
			/* Update the gpp outputs */
			Gpp_Handle(&interface->TSG_ETCS_MMI_DYN);
			/* application functions */
			ETCS_Handle(interface);
			SpeedHandle(interface);
			TractionBrake_Handle(interface);
			/* application functions */
			Hmi_Handle(interface);
			/*update tha outputs via IP*/
			Ccuo_Handle_output(interface);
			/*diagnostic handle*/
			Diag_Handle(interface);
			/* write the data in the shared memory*/
			sm_send_evc_sts(EVC_STS_SEND, GetApplEvcSts(), 1);
			sm_send_stm_sts(STM_STS_SEND, GetApplStmSts(), 1);
			/* write the data in tha application structure to send
			to output component MVB_output...*/
		}/*if*/
	}/*if chk*/
}


void TPGT_Main_Task64_init(void)
{
	/* trace shared memory --initialized the frame */
	SET_TRACE(2, TPG_64, 0);
	SET_TRACE(3, TPG_64, 0);
	/* call init functions of all componet of task 64 ms*/
	Mvb_Input64_init();
	GPP_Handle_init();
	Speed_init();
	StartPhase_init();
	Hmi_Handle_init();
	Ccuo_Handle_Init();
	CesaInit();
}
