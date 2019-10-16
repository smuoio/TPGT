/**
* \file Gpp_handle.c
* \brief the file manages the application the interface functions with the
*        the appluication functions used to write/read the GPP data. The use
*        of thiese functions is in the application task
* \author Muoio Salvatore
*/

#include "Comm_Util.h"
#include "Gpp_Handle.h"
#include "GPP_Handle_Public.h"
#include "GPP_Handle_impl.h"
#include "GPP_StmHandle_impl.h"
#include "Generic_api.h"

/* static variables */

/**
*\var t_evc_data sEvcData
*\brief static variable to object evc
*/
static t_evc_data sEvcData;
/**
*\var t_evc_data sStmData
*\brief static variable to object stm
*/
static t_stm_data sStmData;
/**
*\var static t_evc_status sEvcSts
*\brief static variable to object evc sts
*/
static t_evc_sts sEvcSts;
/**
*\var static t_stm_sts sStmSts
*\brief static variable to object evc sts
*/
static t_stm_sts sStmSts;
/* interface functions */
t_evc_data *GetApplEvcData(void)
{
  return(&sEvcData);
}

t_stm_data *GetApplStmData(void)
{
  return(&sStmData);
}

t_evc_sts *GetApplEvcSts(void)
{
  return(&sEvcSts);
}

t_stm_sts *GetApplStmSts(void)
{
  return(&sStmSts);
}

/* application function !!! */
void InvalidateDataOut(void)
{
	  /* reset static index of buffer packet2write*/
	  reset_index_packet2write();
    reset_stm_index_packet2write();
}
/**
*\var static BOOLEAN8 sCabActive
*\brief static variable used to detect the rising edge
*       of the signal cab occupied
*/
static BOOLEAN8 sCabActive = FALSE;

void Gpp_Handle(const TSG_ETCS_MMI_DYN *pTsgEtcs)
{
  if(pTsgEtcs != 0){
  BOOLEAN8 lIsAlive        = FALSE;
  BOOLEAN8 lIsCabActiv     = FALSE;
  BOOLEAN8 lIsEtcsIsolated = FALSE;
  isEbiCabAlive(&lIsAlive);
  isCabActive(&lIsCabActiv);
  isEtcsIsolated(&lIsEtcsIsolated);

  /* invalidate all data to output */
  InvalidateDataOut();
  /* create the application data of EVC-100 */
  Create_MmiMStartMmi();
  /* create the application data each 250 ms EVC-102 */
  Create_MmiStatusReport(pTsgEtcs);
  /*STM -1*/
  Create_StmVersionNumber();
  /**
  *\brief:check if the ebicab is alive
  *       the control is done checking the MVB life of process data
  */
  if(lIsAlive == TRUE)
  {
	  /*EVC-101*/
    Create_MmiRequest();
    /*EVC-104*/
    Create_MmiNewDriverData();
    /*EVC-111*/
    Create_MmiDriverMessageAck();
    /*EVC-123*/
    Create_MmiSpecifiStmDatatoStm();
	/*STM-34*/
	Create_MmiStmButtonEventReport();
    /*EVC-109*/
    if(RisingEdge(lIsCabActiv, &sCabActive) == TRUE)
      Create_MmiSetTimeMmi();
  }/*if*/
}/*if*/
}

void GPP_Handle_init(void)
{
    /*set up the time for each 250 ms*/
    SetTime(GetTimeEvc102(), (T1S/TASK64));
}
