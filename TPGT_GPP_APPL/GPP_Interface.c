/**
* \file GPP_Interface.c
* \brief the file manages the interface with any device of signalling that it uses
*        the link layer MVB and Protocol layer GPP
* \author Muoio Salvatore
*/

#include "GPP_Interface.h"
#include "GPP_EvcPacket_impl.h"
#include "GPP_StmPacket_impl.h"

/**
* \def invoke
* \brief macro invoke (inline function)
*/
#define invoke(f, a)\
        if((f != 0)&&(a!=0))\
        f(a);

/**
* \struct t_EvcInterface
* \brief interface of EVC/STM functions. This interface was implemented to habe
*        the same implementatin for differents ebicab and different implementation. 
*        The use of a function pointer can achieve
*        inheritance of a method from a superclass.
*/
typedef struct
{
  void (*_Create_MmiStartAtp)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiStatus)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiSetTimeAtp)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiTrackDescription)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiCurrentTrainData)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiDriverMessage)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiFailureReportAtp)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiEnableDriverRequest)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MMiSpecificStmDeRequest)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiSpecificStmDwValues)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Create_MmiCurrentDriverData)(const t_FFFISPacket * const lFFFISPacket);
  void (*_Build_MmiStartMmi)(t_FFFISMessage * const lFFFISMessage);
  void (*_Build_MmiStatusReport)(t_FFFISMessage * const lFFFISMessage);
  void (*_Build_MmiRequest)(t_FFFISMessage * const lFFFISMessage);
  void (*_Build_MmiNewDriverData)(t_FFFISMessage * const lFFFISMessage);
  void (*_Build_MmiDriverMessageAck)(t_FFFISMessage * const lFFFISMessage);
  void (*_Build_MmiSpecificStmDataToStm)(t_FFFISMessage * const lFFFISMessage);
  void (*_Create_VersionNumber)(const t_StmPacket * const lStmPacket);
  void (*_Create_StateReport)(const t_StmPacket * const lStmPacket);
  void (*_Create_ButtonRequest)(const t_StmPacket * const lStmPacket);
  void (*_Create_MmiStmTextMessage)(const t_StmPacket * const lStmPacket);
  void (*_Create_MmiStmDeleteTextMessage)(const t_StmPacket * const lStmPacket);
  void (*_Create_MmiStmSoundCmd)(const t_StmPacket * const lStmPacket);
  void (*_Create_MmiStmIndicatorRequest)(const t_StmPacket * const lStmPacket);
  void (*_Build_StmVersionNumber)(t_FFFISMessage * const lFFFISMessage);
  void (*_Build_MmiStmButtonEventReport)(t_FFFISMessage * const lFFFISMessage);
  void(*_Build_MmiSetTimeMmi)(t_FFFISMessage * const lFFFISMessage);
}t_EvcInterface;

/**
* \var t_EvcInterface gEvcInterface
* \brief static object to interface Evc
*/
static t_EvcInterface gEvcInterface;
/**
* \fn static t_EvcInterface *GetEvcInterface(void)
* \brief the function returns the adrress of static interface
*/

static t_EvcInterface *GetEvcInterface(void)
{
  return(&gEvcInterface);
}

void Interface_Init(void)
{
  /* check which ebicab is selected*/
  /* WhoAmIn()*/
  /* now fix ebicab2000*/
  GetEvcInterface()->_Create_MmiStartAtp = &Create_MmiStartAtp_;
  GetEvcInterface()->_Create_MmiStatus = &Create_MmiStatus_;
  GetEvcInterface()->_Create_MmiSetTimeAtp = &Create_MmiSetTimeAtp_;
  GetEvcInterface()->_Create_MmiTrackDescription = &Create_MmiTrackDescription_;
  GetEvcInterface()->_Create_MmiCurrentTrainData = &Create_MmiCurrentTrainData_;
  GetEvcInterface()->_Create_MmiDriverMessage = &Create_MmiDriverMessage_;
  GetEvcInterface()->_Create_MmiFailureReportAtp = &Create_MmiFailureReportAtp_;
  GetEvcInterface()->_Create_MmiEnableDriverRequest = &Create_MmiEnableDriverRequest_;
  GetEvcInterface()->_Create_MMiSpecificStmDeRequest = &Create_MMiSpecificStmDeRequest_;
  GetEvcInterface()->_Create_MmiCurrentDriverData = &Create_MmiCurrentDriverData_;
  GetEvcInterface()->_Build_MmiStartMmi = &Build_MmiStartMmi_;
  GetEvcInterface()->_Build_MmiRequest  = &Build_MmiRequest_;
  GetEvcInterface()->_Build_MmiStatusReport = &Build_MmiStatusReport_;
  GetEvcInterface()->_Create_VersionNumber = &Create_VersionNumber_;
  GetEvcInterface()->_Create_StateReport = &Create_StateReport_;
  GetEvcInterface()->_Create_ButtonRequest = &Create_ButtonRequest_;
  GetEvcInterface()->_Create_MmiSpecificStmDwValues = &Create_MmiSpecificStmDwValues_;
  GetEvcInterface()->_Create_MmiStmTextMessage = &Create_MmiStmTextMessage_;
  GetEvcInterface()->_Create_MmiStmDeleteTextMessage = &Create_MmiStmDeleteTextMessage_;
  GetEvcInterface()->_Create_MmiStmSoundCmd = &Create_MmiStmSoundCmd_;
  GetEvcInterface()->_Create_MmiStmIndicatorRequest = &Create_MmiStmIndicatorRequest_;
  GetEvcInterface()->_Build_MmiNewDriverData = &Build_MmiNewDriverData_;
  GetEvcInterface()->_Build_MmiDriverMessageAck = &Build_MmiDriverMessageAck_;
  GetEvcInterface()->_Build_MmiSpecificStmDataToStm = &Build_MmiSpecificStmDataToStm_;
  GetEvcInterface()->_Build_StmVersionNumber = &Build_StmVersionNumber_;
  GetEvcInterface()->_Build_MmiStmButtonEventReport = &Build_MmiStmButtonEventReport_;
  GetEvcInterface()->_Build_MmiSetTimeMmi = &Build_MmiSetTimeMmi_;
}
void Create_MmiStartAtp(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiStartAtp, lFFFISPacket);
}
void Create_MmiStatus(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiStatus, lFFFISPacket);
}
void Create_MmiSetTimeAtp(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiSetTimeAtp, lFFFISPacket);
}
void Create_MmiTrackDescription(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiTrackDescription, lFFFISPacket);
}
void Create_MmiCurrentTrainData(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiCurrentTrainData, lFFFISPacket);
}
void Create_MmiDriverMessage(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiDriverMessage, lFFFISPacket);
}
void Create_MmiFailureReportAtp(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiFailureReportAtp, lFFFISPacket);
}
void Create_MmiEnableDriverRequest(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiEnableDriverRequest, lFFFISPacket);
}
void Create_MmiCurrentDriverData(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiCurrentDriverData, lFFFISPacket);
}
void Create_MMiSpecificStmDeRequest(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MMiSpecificStmDeRequest, lFFFISPacket);
}
void Create_MmiSpecificStmDwValues(const t_FFFISPacket * const lFFFISPacket)
{
  invoke(GetEvcInterface()->_Create_MmiSpecificStmDwValues, lFFFISPacket);
}
void Build_MmiStartMmi(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_MmiStartMmi, lFFFISMessage);
}
void Build_MmiStatusReport(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_MmiStatusReport, lFFFISMessage);
}
void Create_VersionNumber(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_VersionNumber, lStmPacket);
}
void Create_StateReport(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_StateReport, lStmPacket);
}
void Create_ButtonRequest(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_ButtonRequest, lStmPacket);
}
void Create_MmiStmTextMessage(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_MmiStmTextMessage, lStmPacket);
}
void Create_MmiStmDeleteTextMessage(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_MmiStmDeleteTextMessage, lStmPacket);
}
void Create_MmiStmSoundCmd(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_MmiStmSoundCmd, lStmPacket);
}
void Create_MmiStmIndicatorRequest(const t_StmPacket * const lStmPacket)
{
  invoke(GetEvcInterface()->_Create_MmiStmIndicatorRequest, lStmPacket);
}
void Build_MmiRequest(t_FFFISMessage * const lFFFISMessage)
{
	invoke(GetEvcInterface()->_Build_MmiRequest, lFFFISMessage);
}
void Build_MmiNewDriverData(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_MmiNewDriverData, lFFFISMessage);
}
void Build_MmiDriverMessageAck(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_MmiDriverMessageAck, lFFFISMessage);
}
void Build_MmiSpecificStmDataToStm(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_MmiSpecificStmDataToStm, lFFFISMessage);
}
void Build_StmVersionNumber(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_StmVersionNumber, lFFFISMessage);
}
void Build_MmiStmButtonEventReport(t_FFFISMessage * const lFFFISMessage)
{
	invoke(GetEvcInterface()->_Build_MmiStmButtonEventReport, lFFFISMessage);
}
void Build_MmiSetTimeMmi(t_FFFISMessage * const lFFFISMessage)
{
  invoke(GetEvcInterface()->_Build_MmiSetTimeMmi, lFFFISMessage);
}
