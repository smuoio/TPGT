/**
* \file Hmi_PzbLzbIcon.c
* \brief the file manages the private functions of Pzb/Lzb Icon
* \author Muoio Salvatore
*/

#include "Generic_api.h"
#include "GPP_Handle_public.h"
#include "Etcs_Public.h"
#include "Hmi_PzbLzbIcon.h"

/**
* \fn static UINT8 LookUpTableFlashing(UINT8 aFlashing)
* \brief The function checks the status of flashing of icon
* \param aFlashing value of message
* \return status of flashing
*/
static UINT8 LookUpTableFlashing(UINT8 aFlashing)
{
  const UINT8 Tab[] = {FLASHING_NONE,FLASHING_SLOW,FLASHING_FAST,
                      FLASHING_INVALID,FLASHING_COUNTERPHASE_SLOW,
                      FLASHING_COUNTERPHASE_FAST,
                      FLASHING_INVALID,FLASHING_INVALID};
  return(Tab[aFlashing]);
}

/**
* \fn static UINT8 GetFlashing(UINT16 aAttrib)
* \brief The function returns the status of flashing of icon
* \param aAttrib attribute value of message
* \return status of flashing
*/
static UINT8 GetFlashing(UINT16 aAttrib)
{
  UINT8 lFlashing = 0;
  if(((aAttrib>>9)&1) != 0){
    lFlashing = LookUpTableFlashing((aAttrib>>6)&3);
  }
  return(lFlashing);
}

/**
* \fn static UINT8 GetFrameColor(UINT16 aAttrib)
* \brief The function returns the status of frame color of icon
* \param aAttrib attribute value of message
* \return status of color
*/
static UINT8 GetFrameColor(UINT16 aAttrib)
{
  UINT8 lFrameColor = 0;
  if(((aAttrib>>9)&1) != 0){
    lFrameColor = (aAttrib>>3)&7;
  }
  return(lFrameColor);
}

/**
* \fn static void pzblzbicon_default(DS_CCUT_HMI_TRN *const pCmd)
* \brief The function sets the default value of 7 icons
* \param pCmd pointer to the structure DS_CCUT_HMI_TRN
*/
static void pzblzbicon_default(DS_CCUT_HMI_TRN *const pCmd)
{
  if(pCmd != 0){
    pCmd->TPT_StmIndicator1.SymbolId       = 0;
    pCmd->TPT_StmIndicator1.IndicatorId    = 0;
    pCmd->TPT_StmIndicator1.FrameColor     = 0;
    pCmd->TPT_StmIndicator1.SymbolFlashing = FLASHING_NONE;
    pCmd->TPT_StmIndicator2.SymbolId       = 0;
    pCmd->TPT_StmIndicator2.IndicatorId    = 0;
    pCmd->TPT_StmIndicator2.FrameColor     = 0;
    pCmd->TPT_StmIndicator2.SymbolFlashing = FLASHING_NONE;
    pCmd->TPT_StmIndicator3.SymbolId       = 0;
    pCmd->TPT_StmIndicator3.IndicatorId    = 0;
    pCmd->TPT_StmIndicator3.FrameColor     = 0;
    pCmd->TPT_StmIndicator3.SymbolFlashing = FLASHING_NONE;
    pCmd->TPT_StmIndicator4.SymbolId       = 0;
    pCmd->TPT_StmIndicator4.IndicatorId    = 0;
    pCmd->TPT_StmIndicator4.FrameColor     = 0;
    pCmd->TPT_StmIndicator4.SymbolFlashing = FLASHING_NONE;
    pCmd->TPT_StmIndicator5.SymbolId       = 0;
    pCmd->TPT_StmIndicator5.IndicatorId    = 0;
    pCmd->TPT_StmIndicator5.FrameColor     = 0;
    pCmd->TPT_StmIndicator5.SymbolFlashing = FLASHING_NONE;
    pCmd->TPT_StmIndicator6.SymbolId       = 0;
    pCmd->TPT_StmIndicator6.IndicatorId    = 0;
    pCmd->TPT_StmIndicator6.FrameColor     = 0;
    pCmd->TPT_StmIndicator6.SymbolFlashing = FLASHING_NONE;
    pCmd->TPT_StmIndicator7.SymbolId       = 0;
    pCmd->TPT_StmIndicator7.IndicatorId    = 0;
    pCmd->TPT_StmIndicator7.FrameColor     = 0;
    pCmd->TPT_StmIndicator7.SymbolFlashing = FLASHING_NONE;
  }
}

/**
* \fn static void RemoveIcon(UINT8 NidIcon, UINT16 NidAttrib,
*                            UINT8 *const pSymbolId, UINT8 *const pFrameColor,
*                            UINT8 *const pFlashing)
* \brief The function resets the icon in according with attribute value
* \param NidIcon index icon
* \param NidAttrib attribute
* \param pSymbolId pointer SymbolId
* \param pFrameColor pointer frame color
* \param pFlashing pointer flashing
*/
static void RemoveIcon(UINT8 NidIcon, UINT16 NidAttrib, UINT8 *const pSymbolId,
                      UINT8 *const pFrameColor, UINT8 *const pFlashing)
{
	if(NidIcon == 0)
	{
		*pFrameColor = 0;
		*pFlashing   = 0;
	}
	if(NidAttrib == 0)
		*pSymbolId = 0;
}

/**
* \fn static void CopyOutputIcons(DS_CCUT_HMI_TRN *const pCmd,
                            const t_PzbLzbIndicator * const pPzbLzbIndicator)
* \brief The function copy the local strcture of icons in the output to the Hmi
* \param pCmd output pointer to the struct DS_CCUT_HMI_TRN
* \param pPzbLzbIndicator pointer to local structure
*/
static void CopyOutputIcons(DS_CCUT_HMI_TRN *const pCmd,
                            const t_PzbLzbIndicator * const pPzbLzbIndicator)
{
  if((pCmd != 0)&&(pPzbLzbIndicator != 0))
  {
    pCmd->TPT_StmIndicator1 = pPzbLzbIndicator->TPT_StmIndicator1;
    pCmd->TPT_StmIndicator2 = pPzbLzbIndicator->TPT_StmIndicator2;
    pCmd->TPT_StmIndicator3 = pPzbLzbIndicator->TPT_StmIndicator3;
    pCmd->TPT_StmIndicator4 = pPzbLzbIndicator->TPT_StmIndicator4;
    pCmd->TPT_StmIndicator5 = pPzbLzbIndicator->TPT_StmIndicator5;
    pCmd->TPT_StmIndicator6 = pPzbLzbIndicator->TPT_StmIndicator6;
    pCmd->TPT_StmIndicator7 = pPzbLzbIndicator->TPT_StmIndicator7;
  }

}


static t_PzbLzbIndicator PzbLzbIndicator;
static t_PzbLzbIndicator *GetPzbLzbIndicator(void)
{
  return(&PzbLzbIndicator);
}

void Hmi_PzbLzbIcon(DS_CCUT_HMI_TRN *const pCmd)
{
  UINT8 ii = 0; /*index cycle*/
  if(pCmd != 0){
    BOOLEAN8 lIsAlive = FALSE;
    /*check if ebicab is alive*/
    isEbiCabAlive(&lIsAlive);
    if((lIsAlive == TRUE)){
      for(; ii <= GetStm35MmiStmNIter(); ii++)
      {
        switch(GetStm35MmiNidIndpos(ii))
        {
          case 1:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator1.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator1.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator1.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator1.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator1.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator1.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator1.SymbolFlashing);
          break;
          case 2:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator2.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator2.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator2.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator2.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator2.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator2.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator2.SymbolFlashing);
          break;
          case 3:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator3.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator3.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator3.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator3.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator3.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator3.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator3.SymbolFlashing);
          break;
          case 4:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator4.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator4.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator4.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator4.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator4.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator4.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator4.SymbolFlashing);
          break;
          case 5:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator5.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator5.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator5.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator5.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator5.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator5.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator5.SymbolFlashing);
          break;
          case 6:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator6.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator6.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator6.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator6.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator6.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator6.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator6.SymbolFlashing);
          break;
          case 7:
          /**/
          GetPzbLzbIndicator()->TPT_StmIndicator7.SymbolId       = GetStm35MmiNidIcon(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator7.IndicatorId    = GetStm35MmiNidIcator(ii);
          GetPzbLzbIndicator()->TPT_StmIndicator7.FrameColor     = GetFrameColor(GetStm35MmiStmNidAttrib(ii));
          GetPzbLzbIndicator()->TPT_StmIndicator7.SymbolFlashing = GetFlashing(GetStm35MmiStmNidAttrib(ii));
		      RemoveIcon(GetStm35MmiNidIcon(ii),
		                 GetStm35MmiStmNidAttrib(ii),
		                 &GetPzbLzbIndicator()->TPT_StmIndicator7.SymbolId,
					           &GetPzbLzbIndicator()->TPT_StmIndicator7.FrameColor,
					           &GetPzbLzbIndicator()->TPT_StmIndicator7.SymbolFlashing);
          break;
          default:
          break;
        }/*switch*/
      }/*for*/
      /**/
      {
        if(GetStmModus() != STM_MODUS_UNKNOWN) /*copy in the output*/
          CopyOutputIcons(pCmd, GetPzbLzbIndicator());
      }
    }/*if*/
    else
      pzblzbicon_default(pCmd);
  }/*if pls*/
}

void Hmi_ResetPzbLzbIcon(void)
{
  /*reset the structure*/
    memset(GetPzbLzbIndicator(), 0, sizeof(t_PzbLzbIndicator));
}
