/**
* \file DebugTask64.c
* \brief the file impplements the functions used to debug/print the variables
*        used in the task 64
* \author Muoio Salvatore
*/

#include "DebugTask64.h"
#include "Gpp_Handle.h"

/**
* \fn static void PrintEvcData(void)
* \brief the function print the EVC data of shared memory incoming
*/
static void PrintEvcData(void)
{
  e_result lChk = TPG_SUCCESS;
  t_evc_data *lEvcData = GetApplEvcData();
  t_evc_sts *lEvcSts   = GetApplEvcSts();
  lChk |= checkOutputArg((t_evc_data*)lEvcData);
	if(TPG_SUCCEEDED(lChk))
  {
    if(GetApplEvcData()->evc_packet[0] == 1)
    {
      mon_broadcast_printf("*** Packet EVC 0 ***\n");
      mon_broadcast_printf(" MmiMStartReq = %X\n",lEvcData->MmiStartAtp.MmiMStartReq);
    }
    if(GetApplEvcData()->evc_packet[1] == 1)
    {
      mon_broadcast_printf("*** Packet EVC 2 ***\n");
      mon_broadcast_printf(" _MmiMActiveCabin_ = %X\n",lEvcData->MmiStatus.CabData.Evc2Sts.MmiMActiveCabin);
      mon_broadcast_printf(" MmiMActiveCabin_Data = %X\n",lEvcData->MmiStatus.CabData.Data);
      mon_broadcast_printf("*** Packet EVC 102 ***\n");
      mon_broadcast_printf(" MmiMMode = %X\n",lEvcSts->MmiStatusReport.CurrMmi.MmiMMode);
      mon_broadcast_printf(" MmiMMmiStatus = %X\n",lEvcSts->MmiStatusReport.CurrMmi.MmiMMmiStatus);
    }
    if(GetApplEvcData()->evc_packet[9] == 1)
    {
      mon_broadcast_printf("*** Packet EVC 21 ***\n");
      mon_broadcast_printf(" MmiQRequestEnable = %X\n",lEvcData->MmiEnableDriverRequest.MmiQRequestEnable);
    }

    if(GetApplEvcData()->evc_packet[8] == 1)
    {
      mon_broadcast_printf("*** Packet EVC 14 ***\n");
      mon_broadcast_printf(" Driver_Data = %X\n",
                             lEvcData->MmiCurrentDriverData.MmiQDriver.Driver.Driver_Data);
      mon_broadcast_printf(" MmiNidDriver = %X\n",
                            lEvcData->MmiCurrentDriverData.MmiNidDriver);
      mon_broadcast_printf(" MmiNidOperation = %X\n",
                            lEvcData->MmiCurrentDriverData.MmiNidOperation);
      mon_broadcast_printf(" MmiNidLanguage = %X\n",
                            lEvcData->MmiCurrentDriverData.MmiNidLanguage);
    }
    if(GetApplEvcData()->evc_packet[3] == 1)
    {
      mon_broadcast_printf("*** Packet EVC 8 ***\n");
      mon_broadcast_printf("MmiIText = %X\n", lEvcData->MmiDriverMessage.MmiIText);
      mon_broadcast_printf("MmiQText = %X\n", lEvcData->MmiDriverMessage.MmiQText);
      mon_broadcast_printf("MmimQTextCriteria = %X\n",
                           lEvcData->MmiDriverMessage.MmiQ.Text.MmimQTextCriteria);
    }
    if(GetApplEvcData()->evc_packet[10] == 1)
    {
      UINT16 ii = 0;
      mon_broadcast_printf("*** Packet EVC 25 ***\n");
      mon_broadcast_printf("MmiNidStm = %X\n", lEvcData->MMiSpecificStmDeRequest.MmiNidStm);
      mon_broadcast_printf("Iter = %X\n", lEvcData->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter);
      mon_broadcast_printf("Following = %X\n", lEvcData->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Following);
      for(; ii < lEvcData->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter; ii++)
      {
        mon_broadcast_printf("MmiNidStm = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiNidStm);
        mon_broadcast_printf("MmiStmNidData = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNidData);
        mon_broadcast_printf("MmiXAttribute = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiXAttribute);
        mon_broadcast_printf("MmiLCaption = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiLCaption);
        mon_broadcast_printf("MmiStmLValue = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmLValue);
        mon_broadcast_printf("MmiStmNIter = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNIter);
        mon_broadcast_printf("Zda = %X\n", lEvcData->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[0]);
      }/*for*/
    }
    if(GetApplEvcData()->evc_packet[11] == 1)
    {
      UINT16 ii = 0;
      mon_broadcast_printf("*** Packet EVC 26 ***\n");
      mon_broadcast_printf("MmiNidStm = %X\n", lEvcData->MmiSpecificStmDwValues.MmiNidStm);
      mon_broadcast_printf("Iter = %X\n", lEvcData->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter);
      mon_broadcast_printf("Following = %X\n", lEvcData->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Following);
      for(; ii < lEvcData->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter; ii++)
      {
        mon_broadcast_printf("MmiNidStm = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiNidStm);
        mon_broadcast_printf("MmiStmNidData = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmNidData);
        mon_broadcast_printf("MmiXAttribute = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiXAttribute);
        mon_broadcast_printf("MmiLCaption = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiLCaption);
        mon_broadcast_printf("MmiStmLValue = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmLValue);
        mon_broadcast_printf("MmiStmNIter = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmNIter);
        mon_broadcast_printf("Zda = %X\n", lEvcData->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[0]);
      }/*for*/
    }
  }/*if*/
}

/**
* \fn static void PrintStmData(void)
* \brief the function print the STM data of shared memory incoming
*/
static void PrintStmData(void)
{
  e_result lChk = TPG_SUCCESS;
  t_stm_data *lStmData = GetApplStmData();
  lChk |= checkOutputArg((t_stm_data*)lStmData);
	if(TPG_SUCCEEDED(lChk))
  {
    if(GetApplStmData()->stm_packet[0] == 1)
    {
      mon_broadcast_printf("*** STM  1 ***\n");
      mon_broadcast_printf("MmiStmN058Vermajor = %X\n",lStmData->VersionNumber.MmiStmN058Vermajor);
      mon_broadcast_printf("MmiStmN058Vermid = %X\n",lStmData->VersionNumber.MmiStmN058Vermid);
      mon_broadcast_printf("MmiStmN058Verminor = %X\n",lStmData->VersionNumber.MmiStmN058Verminor);
      mon_broadcast_printf("MmiStmN035Vermajor = %X\n",lStmData->VersionNumber.MmiStmN035Vermajor);
      mon_broadcast_printf("MmiStmN035Vermid = %X\n",lStmData->VersionNumber.MmiStmN035Vermid);
      mon_broadcast_printf("MmiStmN035Verminor = %X\n",lStmData->VersionNumber.MmiStmN035Verminor);
      mon_broadcast_printf("MmiStmNSrsVermajor = %X\n",lStmData->VersionNumber.MmiStmNSrsVermajor);
      mon_broadcast_printf("MmiStmNSrsVerminor = %X\n",lStmData->VersionNumber.MmiStmNSrsVerminor);
    }
    if(GetApplStmData()->stm_packet[1] == 1)
    {
      mon_broadcast_printf("*** STM  15 ***\n");
      mon_broadcast_printf("StateReport = %X\n",lStmData->StateReport);
    }
    if(GetApplStmData()->stm_packet[6] == 1)
    {
      mon_broadcast_printf("*** STM  35 ***\n");
      mon_broadcast_printf("MmiStmNIter = %X\n",lStmData->MmiStmIndicatorRequest.MmiStmNIter.StmIter.Iter);
      {
        INT32 ii = 0;
        for(; ii < lStmData->MmiStmIndicatorRequest.MmiStmNIter.StmIter.Iter; ii++)
        {
          mon_broadcast_printf("MmiStmNidIndicator[%d] = %X\n", ii, lStmData->MmiStmIndicatorRequest.Stm35Iter1[ii].MmiStmNidIndicator);
          mon_broadcast_printf("MmiStmNidIndpos[%d] = %X\n", ii, lStmData->MmiStmIndicatorRequest.Stm35Iter1[ii].MmiStmNidIndpos);
          mon_broadcast_printf("MmiStmNidIcon[%d] = %x\n", ii, lStmData->MmiStmIndicatorRequest.Stm35Iter1[ii].MmiStmNidIcon);
          mon_broadcast_printf("Attrib[%d] = %x\n", ii, lStmData->MmiStmIndicatorRequest.Stm35Iter1[ii].MmiSstmMIndAttrib);
        }
      }
    }
	if(GetApplStmData()->stm_packet[index_stm_39] == 1)
	{
      mon_broadcast_printf("*** STM  39 ***\n");
      mon_broadcast_printf("TextId = %X\n",lStmData->MmiStmNidXMessage);
	}
  if(GetApplStmData()->stm_packet[index_stm_46] == 1)
	{
      UINT8 ii = 0;
      mon_broadcast_printf("*** STM  46 ***\n");
      mon_broadcast_printf("Iter 46 = %X\n",lStmData->MmiStmSoundCmd.MmiStmNIter.StmIter.Iter);
      for(; ii <lStmData->MmiStmSoundCmd.MmiStmNIter.StmIter.Iter; ii++)
      {
        mon_broadcast_printf("MmiStmNidSound[%X] = %X\n", ii, lStmData->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmNidSound);
        mon_broadcast_printf("MmiStmQSound[%X] = %X\n", ii, lStmData->MmiStmSoundCmd.Stm46Iter1[ii].MmiStmQSound);
      }
	}
 }/*if*/
}


e_result DebugTask64(const TYPE_TPGT_MAIN_TASK64_IF * const interface)
{
	e_result lOut = TPG_SUCCESS;
	lOut |= checkOutputArg((TYPE_TPGT_MAIN_TASK64_IF*)interface);
	if(TPG_SUCCEEDED(lOut))
	{
		if(interface->EnablePrintEvc)
		{
			/* print all EVC packet */
			TRACE(1, 2, " ", PrintEvcData());
		}
		if(interface->EnablePrintStm)
		{
			/* print all STM packet */
			TRACE(1, 3, "", PrintStmData())
		}
	}/*if*/
	return(lOut);
}
