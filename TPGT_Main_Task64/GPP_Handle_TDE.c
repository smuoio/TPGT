/**
* \file Gpp_handle_TDE.c
* \brief the file manages the application functions used for the Train Data Entry
*        The cration of these functions is managed in the
*        application task
* \author Muoio Salvatore
*/

#include "Task64_Def.h"
#include "GPP_Handle_Public.h"


/************* EVC25 ****************/
BOOLEAN8 isReceivedEVC25(void)
{
  return((GetApplEvcData()->evc_packet[index_evc_25] == 1) ? TRUE : FALSE);
}

BOOLEAN8 isEVC25IterNull(void)
{
  BOOLEAN8 lOut = FALSE;
  if(isReceivedEVC25() == TRUE)
  {
    if(GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter == 0)
      lOut = TRUE;
  }
  return(lOut);
}

static BOOLEAN8 FindElementEvc25_PZBLZB(UINT8 aIndex, UINT8 aNidStm)
{
  BOOLEAN8 lOut = FALSE;
  UINT16 ii = 0;
  for(; ii < aIndex; ii++)
  {
    UINT8 lMmiNidStm = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiNidStm;
    if((aNidStm == PZB_LZB) && (lMmiNidStm == PZB_LZB))
    {
      lOut = TRUE;
      break;
    }
  }
  return(lOut);
}

static BOOLEAN8 isEVC25PcktRcvd(UINT8 *isEvc25Rcvd)
{
  BOOLEAN8 lOut = FALSE;
  if(isEvc25Rcvd != 0)
  {
    /*check received evc25*/
    if(isReceivedEVC25() == TRUE){
    UINT8 lIter = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter;
    UINT8 lNidStm = GetApplEvcData()->MMiSpecificStmDeRequest.MmiNidStm;
    UINT8 lFollowing = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Following;
    if(lIter != 0)
    {
      if((FindElementEvc25_PZBLZB(lIter, lNidStm) == TRUE) || (lFollowing))
        *isEvc25Rcvd = 1;
    }
    else
    {
    /*if((FindElementEvc25_PZBLZB(lIter, lNidStm) == FALSE) && (!lFollowing))*/
        *isEvc25Rcvd = 2;
    }
    lOut = TRUE;
  }
  }/*if*/
  return(lOut);
}

UINT8 GetEvc25Zda(UINT8 *pZda)
{
  UINT8 lisEvc25Rcvd = 0U;
  if(pZda != 0)
  {
    isEVC25PcktRcvd(&lisEvc25Rcvd);
    if(lisEvc25Rcvd == 1)
    {
      UINT8 lIter = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter;
      UINT8 ii = 0x0U;
      for(; ii < lIter; ii++)
      {
        if(GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNidData == MMI_STM_NID_DATA_ZDA)
        {
          *pZda = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[0];
          break;
        }/*if*/
      }/*for*/
    }
  }/*if*/
  return(lisEvc25Rcvd);
}

void GetEvc25Bra(UINT8 *pBra)
{
  UINT8 lisEvc25Rcvd = 0U;
  if(pBra != 0){
  isEVC25PcktRcvd(&lisEvc25Rcvd);
  if(lisEvc25Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNidData == MMI_STM_NID_DATA_BRA)
      {
        *pBra = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[0];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

void GetEvc25Brh(UINT8* pBrh0, UINT8* pBrh1, UINT8* pBrh2)
{
  UINT8 lisEvc25Rcvd = 0U;
  if((pBrh0 != 0) && (pBrh1 != 0) && (pBrh2 != 0)){
  isEVC25PcktRcvd(&lisEvc25Rcvd);
  if(lisEvc25Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNidData == MMI_STM_NID_DATA_BRH)
      {
        *pBrh0 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[0];
        *pBrh1 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[1];
        *pBrh2 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[2];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

void GetEvc25Vmz(UINT8* pVmz0, UINT8* pVmz1, UINT8* pVmz2)
{
  UINT8 lisEvc25Rcvd = 0U;
  if((pVmz0 != 0) && (pVmz1 != 0) && (pVmz2 != 0)){
  isEVC25PcktRcvd(&lisEvc25Rcvd);
  if(lisEvc25Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNidData == MMI_STM_NID_DATA_VMZ)
      {
        *pVmz0 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[0];
        *pVmz1 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[1];
        *pVmz2 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[2];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

void GetEvc25Zl(UINT8* pZl0, UINT8* pZl1, UINT8* pZl2)
{
  UINT8 lisEvc25Rcvd = 0U;
  if((pZl0 != 0) && (pZl1 != 0) && (pZl2 != 0)){
  isEVC25PcktRcvd(&lisEvc25Rcvd);
  if(lisEvc25Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmNidData == MMI_STM_NID_DATA_ZL)
      {
        *pZl0 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[0];
        *pZl1 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[1];
        *pZl2 = GetApplEvcData()->MMiSpecificStmDeRequest.Evc25StmIter1[ii].MmiStmXValue[2];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

BOOLEAN8 GetEvc25Feedback(void)
{
  BOOLEAN8 lOut = FALSE;
  if(isReceivedEVC25() == TRUE)
  {
    if((GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Following == 0) &&
      (GetApplEvcData()->MMiSpecificStmDeRequest.uMmiStmQ.StmQ.Iter == 0))
      lOut = TRUE;
  }
  return(lOut);
}


/************* EVC26 ****************/

BOOLEAN8 isReceivedEVC26(void)
{
  return((GetApplEvcData()->evc_packet[index_evc_26] == 1) ? TRUE : FALSE);
}

static BOOLEAN8 FindElementEvc26_PZBLZB(UINT8 aIndex, UINT8 aNidStm)
{
  BOOLEAN8 lOut = FALSE;
  UINT16 ii = 0;
  for(; ii < aIndex; ii++)
  {
    UINT8 lMmiNidStm = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiNidStm;
    if((aNidStm == PZB_LZB) && (lMmiNidStm == PZB_LZB))
    {
      lOut = TRUE;
      break;
    }
  }
  return(lOut);
}


static BOOLEAN8 isEVC26PcktRcvd(UINT8 *isEvc26Rcvd)
{
  BOOLEAN8 lOut = FALSE;
  if(isEvc26Rcvd != 0)
  {
    if(GetApplEvcData()->evc_packet[11] == 1){
    UINT8 lIter = GetApplEvcData()->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter;
    UINT8 lNidStm = GetApplEvcData()->MmiSpecificStmDwValues.MmiNidStm;
    UINT8 lFollowing = GetApplEvcData()->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Following;
    if(lIter != 0)
    {
      if((FindElementEvc26_PZBLZB(lIter, lNidStm) == TRUE) || (lFollowing))
        *isEvc26Rcvd = 1;
    }
    else
    {
      if((FindElementEvc26_PZBLZB(lIter, lNidStm) == TRUE) && (!lFollowing))
        *isEvc26Rcvd = 2;
    }
    lOut = TRUE;
  }
  }/*if*/
  return(lOut);
}

void GetEvc26Bra(UINT8 *pBra)
{
  UINT8 lisEvc26Rcvd = 0U;
  if(pBra != 0){
  isEVC26PcktRcvd(&lisEvc26Rcvd);
  if(lisEvc26Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmNidData == MMI_STM_NID_DATA_BRA)
      {
        *pBra = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[0];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

void GetEvc26Brh(UINT8* pBrh0, UINT8* pBrh1, UINT8* pBrh2)
{
  UINT8 lisEvc26Rcvd = 0U;
  if((pBrh0 != 0) && (pBrh1 != 0) && (pBrh2 != 0)){
  isEVC26PcktRcvd(&lisEvc26Rcvd);
  if(lisEvc26Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmNidData == MMI_STM_NID_DATA_BRH)
      {
        *pBrh0 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[0];
        *pBrh1 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[1];
        *pBrh2 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[2];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

void GetEvc26Vmz(UINT8* pVmz0, UINT8* pVmz1, UINT8* pVmz2)
{
  UINT8 lisEvc26Rcvd = 0U;
  if((pVmz0 != 0) && (pVmz1 != 0) && (pVmz2 != 0)){
  isEVC26PcktRcvd(&lisEvc26Rcvd);
  if(lisEvc26Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmNidData == MMI_STM_NID_DATA_VMZ)
      {
        *pVmz0 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[0];
        *pVmz1 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[1];
        *pVmz2 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[2];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}

void GetEvc26Zl(UINT8* pZl0, UINT8* pZl1, UINT8* pZl2)
{
  UINT8 lisEvc26Rcvd = 0U;
  if((pZl0 != 0) && (pZl1 != 0) && (pZl2 != 0)){
  isEVC26PcktRcvd(&lisEvc26Rcvd);
  if(lisEvc26Rcvd == 1)
  {
    UINT8 lIter = GetApplEvcData()->MmiSpecificStmDwValues.uEvc26Iter.StmIter.Iter;
    UINT8 ii = 0x0U;
    for(; ii < lIter; ii++)
    {
      if(GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmNidData == MMI_STM_NID_DATA_ZL)
      {
        *pZl0 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[0];
        *pZl1 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[1];
        *pZl2 = GetApplEvcData()->MmiSpecificStmDwValues.Evc26StmIter[ii].MmiStmXValue[2];
        break;
      }/*if*/
    }/*for*/
  }/*evc data incoming*/
  }/*if check pointer*/
}
