/**
* \file CcutGpp_Appl.c
* \brief the file manages the api public of GPP used in the project*
* \author Muoio Salvatore
*/

#include "gpp_main.h"
#include "CcutGpp_Appl.h"
#include "CcutGpp_impl.h"
#include "GPP_Interface.h"
#include "GPP_EvcStmPublic.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
* \var t_imported_TsgMmi_telegram Tg
* \brief variable to the static objec t_imported_TsgMmi_telegram
*/
t_imported_TsgMmi_telegram Tg;
/**
* \var static UINT8 ReadBuffFromMvb[1200 + 2u]
* \brief static buffer used to store the data via GPP
*/
static UINT8 ReadBuffFromMvb[1200 + 2u];
/**
* \var static UINT16 lLengthDataRead
* \brief static variable used for the length data read
*/
static UINT16 lLengthDataRead = 0U;

/**
*\var static BOOLEAN8 sDataInitialized
*\brief static and global variable used for the trigger, when the sw component
*       is initialized
*/
static BOOLEAN8 sDataInitialized = FALSE;
INT32           (*fp_os_io_printf)        (const SINT8* format, ... );



/*****
* void GetGPPData(BOOLEAN8 aDebug)
*****/
UINT8 GetGPPData(BOOLEAN8 aDebug)
{
		UINT8 lRet = 0x0U;/*local variable used to return*/
		/*Invalidate all buffers*/
		memset(ReadBuffFromMvb, 0, sizeof(ReadBuffFromMvb));
		/*reset index evc and stm!!!!*/
		reset_index_evc_packet();
		reset_index_stm_packet();
		/*Gpp_InvalidateData(&lDE);*/
		/* should use directly the MVB buffer whitout local copy????!!!*/
		if(sDataInitialized == TRUE)
		{
			/*Read Data from MVB*/
			if (!gpp_mmi_queue_overflow(1)) /*check the GPP queue state*/
			{
				gpp_set_cabin(1);        /*set the cabin number */
				gpp_read_data_packets(); /*read data cyclic*/
				gpp_conn_state();        /*update connection state cyclic*/
			  lLengthDataRead = gpp_qm_read_tsg_data(&ReadBuffFromMvb[0], sizeof(ReadBuffFromMvb));
				if(GPP_PacketConsistency(lLengthDataRead) == GPP_SUCCESS)
				/*if((lLengthDataRead > 0) && (lLengthDataRead < 1200))*/
				/*(!GPP_ReadTSGQueue(&lLengthDataRead))*/
				{
					/*pFffisMsgData = &ReadBuffFromMvb[0];*/
					ae_write(
										AE_POSITION,
										"GPP get");
					/*check the consistency of incomaing packet*/
					/*if(GPP_PacketConsistency(lLengthDataRead) == GPP_SUCCESS)*/

						GPP_ReadPacket(&ReadBuffFromMvb[0]); /*read-parser the packet of incoming*/
				}
			}/*if*/
			else
			{
				/*mon_broadcast_printf("[GetGPPData]::queue overflow!!!\n");*/
				GPP_Prepare2StorageData();
				lRet |= 1;
			}
		}
		{
			/*debug */
			if(aDebug == TRUE)
			{
				UINT32 ii = 28; /* counter*/
				mon_broadcast_printf("Input first dataset used to GPP communication\n");
				for(; ii < 32; ii++)
					mon_broadcast_printf("Tg.TPG_TSG_MMI_F1_P1[%d] = %X\n", ii, Tg.TPG_TSG_MMI_F1_P1[ii]);
			}
		}/*body*/
		return(lRet);
}

/*****
* void PutGPPData(BOOLEAN8 aDebug)
*****/
UINT8 PutGPPData(BOOLEAN8 aDebug)
{
	UINT8 lRet = 0x0U;/*local variable used to return*/
	/* TODO !!!!*/
	/* working with one cabin*/
	if(sDataInitialized == TRUE)
	{
		/*check data to send !!!*/
		if (!gpp_mmi_queue_overflow(1)) /*check the GPP queue state*/
		{
			gpp_set_cabin(1);        /*set the cabin number */
			/*TODO*/
			/*gpp_qm_store_mmi_data(&buffapplication, nidm, numbytes)*/
			GPP_StoreMmiData(aDebug);
			/* store data mmi */
			gpp_qm_read_mmi_data();  /*put one mmi_packet_queue element to thr GPP layer*/
			gpp_write_data_packets();/*send that element via GPP to the MMI(n data subsets)*/
		}
		else
		{
			GPP_Prepare2StorageData();
			lRet |= 1;
		}
	}/*if*/
	return(lRet);
}

BOOLEAN8 GPP_isDataInitialized(void)
{
	/**
	*\var BOOLEAN8 lDataInitialized
	*\brief static and local variable used as toogle
	*/
	static BOOLEAN8 lDataInitialized = FALSE;
	BOOLEAN8 lOut = FALSE;
	if((lDataInitialized == FALSE) & (sDataInitialized == TRUE))
	{
		/* sw component initialized*/
		lOut = TRUE;
	}
	/*update the local toogle*/
	lDataInitialized = sDataInitialized;
	return(lOut);
}


void GPP_SetDataInitialized(void)
{
	/*Preparation to storage the data*/
	if(sDataInitialized == FALSE)
	{
		mon_broadcast_printf("Init GPP Comm::\n");
		gpp_init_ports(CABIN, CTRL_LOCAL_PORT, DATA_PORT_1, DATA_PORT_2, CTRL_REMOTE_PORT);
		GPP_Prepare2StorageData();
		sDataInitialized = TRUE;
	}
}

void CcutGpp_init(void)
{
	gpp_init_ports(CABIN, CTRL_LOCAL_PORT, DATA_PORT_1, DATA_PORT_2, CTRL_REMOTE_PORT);
	/*GPP_Prepare2StorageData();*/
	/* initialized the interfaces */
	Interface_Init();
	/*initialized the pointer funtion*/
	GPP_StoreMmiDataInit();
	fp_os_io_printf = (void*)mon_broadcast_printf;
	/* init debug variables*/
	SET_TRACE(1, TPG_64, 0);
}

void GPP_CopyLocal(BYTE *pCtrlLoc, BYTE *pDataPort1, BYTE *pDataPort2)
{
	if((pCtrlLoc != 0) && (pDataPort1 != 0) && (pDataPort2 != 0))
	{
		/* copy local telegrams */
		/*CopyTelegramEntryLocal(pCtrlLoc, pDataPort1, pDataPort2);*/
		memcpy(Tg.TPG_TSG_MMI_F1_P1, pCtrlLoc,   32);
		memcpy(Tg.TPG_TSG_MMI_F1_P2, pDataPort1, 32);
		memcpy(Tg.TPG_TSG_MMI_F1_P3, pDataPort2, 32);
	}/*if */
}



e_result GPP_UpdateOut(BYTE *p2Out)
{
	e_result lOut = TPG_SUCCESS;
	lOut |= checkOutputArg((BYTE*)p2Out);
	if(TPG_SUCCEEDED(lOut))
	{
		if(sDataInitialized == TRUE)
		{
			/*remote copy*/
			memcpy(p2Out, Tg.TPG_MMI_TSG_F1, MAX_LEN_PORT);
		}
	}/*if*/
	return(lOut);
}

#ifdef __cplusplus
}
#endif
