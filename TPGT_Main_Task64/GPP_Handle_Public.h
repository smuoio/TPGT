/**
* \file Gpp_Handle_Public.h
* \brief the file manages the application the interface functions with the
*        the appluication functions used to write/read the GPP data. The use
*        of thiese functions is in the application task
* \author Muoio Salvatore
*/


#ifndef __GPP_HANDLE_PUBLIC_H__
#define __GPP_HANDLE_PUBLIC_H__

#include "Shared_Memory.h"
#include "GPP_Handle_Type.h"

/**
* \fn t_evc_sts *GetApplEvcSts(void)
* \brief interface function to the structure t_evc_status
* \return the address to the object t_evc_status
*/
t_evc_sts *GetApplEvcSts(void);
/**
* \fn t_evc_data *GetApplEvcData(void)
* \brief interface function to the structure t_evc_data
* \return the address to the object t_evc_data
*/
t_evc_data *GetApplEvcData(void);
/**
* \fn t_stm_data *GetApplStmData(void)
* \brief interface function to the structure t_stm_data
* \return the address to the object t_stm_data
*/
t_stm_data *GetApplStmData(void);
/**
* \fn t_stm_sts *GetApplStmSts(void)
* \brief interface function to the structure t_stm_sts
* \return the address to the object t_stm_sts
*/
t_stm_sts *GetApplStmSts(void);
/**
* \fn UINT8 GetEvcActiveCab(void)
* \brief interface function
* \return the value of field Active Cab Status of EVC-2
*/
UINT8 GetEvcActiveCab(void);
/**
* \fn e_MmiMStartReq GetEvcMmiStartReq(void)
* \brief interface function
* \return the value of field MMMI Start Req status of EVC-0
*  Info request(0), Idle state(1), Mmi type not supported(2)
*  Incompatible IF versions(3), Incompatible sw versions(4)
*/
e_MmiMStartReq GetEvcMmiStartReq(void);
/**
* \fn e_StmStateReport GetStmNidStateReport(void)
* \brief interface function
* \return the value of field Nid State Report of STM-15
*  Info poweron(1), configuration(2), dataentry(3)
*  coldstanby(4), hotstandby(6), dataavailable(7)
*  failure(8)
*/
e_StmStateReport GetStmNidStateReport(void);
/**
* \fn UINT32 GetEVC21EnableDriverRequest(void)
* \brief interface function(EVC-21)
* \return the value the controls variable driver requests.
*/
UINT32 GetEVC21EnableDriverRequest(void);
/**
* \fn BOOLEAN8 GetButtonRequestTestLzbPzb(void)
* \brief interface function
* \return the value the controls variable driver requests.
*/
BOOLEAN8 GetButtonRequestTestLzbPzb(void);
/**
* \fn UINT8 GetMmiQDriverData(void)
* \brief interface function
* \return the value the controls variable driver requests.
*/
UINT8 GetMmiQDriverData(void);
/**
* \fn UINT8 GetMmiNidLanguage(void)
* \brief interface function
* \return the value the controls variable nid language.
*/
UINT8 GetMmiNidLanguage(void);
/**
* \fn BOOLEAN8 isReceivedSTM1(void)
* \brief interface function
* \return the value of STM 1 .
*/
BOOLEAN8 isReceivedSTM1(void);
/**
* \fn BOOLEAN8 isReceivedSTM38(void)
* \brief interface function
* \return TRUE recv STM-38, FALSE otherwise
*/
BOOLEAN8 isReceivedSTM38(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the 058 sw number.
*/
UINT8 GetMmiStmN058Vermajor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the 058 sw number.
*/
UINT8 GetMmiStmN058Vermid(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the 058 sw number.
*/
UINT8 GetMmiStmN058Verminor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the 035 sw number.
*/
UINT8 GetMmiStmN035Vermajor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the 035 sw number.
*/
UINT8 GetMmiStmN035Vermid(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the 035 sw number.
*/
UINT8 GetMmiStmN035Verminor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the sw number.
*/
UINT8 GetMmiStmNSrsVermajor(void);
/**
* \fn UINT8 GetMmiStmN058Vermajor(void)
* \brief interface function
* \return the value the sw number.
*/
UINT8 GetMmiStmNSrsVerminor(void);
/**
* \fn UINT8 GetEvc8MmiQTextCriteria(void)
* \brief interface function
* \return the value the mmi q text criteria number.
*/
UINT8 GetEvc8MmiQTextCriteria(void);
/**
* \fn UINT16 GetEvc8MmiQText(void)
* \brief interface function
* \return the value the mmi q text criteria.
*/
UINT16 GetEvc8MmiQText(void);
/**
* \fn UINT8 GetEvc8MmiIText(void)
* \brief interface function
* \return the value the mmi i text
*/
UINT8 GetEvc8MmiIText(void);
/**
* \fn UINT8 GetEvc8MmiXText(void)
* \brief interface function
* \return the value first byte of the mmi x text
*/
UINT8 GetEvc8MmiXText(void);
/**
* \fn UINT8 GetEvc8MmiNText(void)
* \brief interface function
* \return the value first byte of the mmi n text
*/
UINT8 GetEvc8MmiNText(void);
/**
* \fn UINT8 GetStm38MmiStmNid(void)
* \brief interface function
* \return the value first byte of the mmi n text
*/
UINT8 GetStm38MmiStmNid(void);
/**
* \fn UINT8 GetStm38MmiStmLTxt(void)
* \brief interface function
* \return the value first byte of the mmi n text
*/
UINT8 GetStm38MmiStmLTxt(void);
/**
* \fn UIN16 GetStm38MmiStmXAttribute(void)
* \brief interface function
* \return the value of attribute of text
*/
UINT16 GetStm38MmiStmXAttribute(void);
/**
* \fn UINT8 GetStm39MmiStmNid(void)
* \brief interface function
* \return the value first byte of the mmi n text
*/
UINT8 GetStm39MmiStmNid(void);
/**
* \fn BOOLEAN8 isReceivedSTM39(void)
* \brief interface function
* \return TRUE recv STM-39, FALSE otherwise
*/
BOOLEAN8 isReceivedSTM39(void);
/**
* \fn UINT8 isReceivedEVC0(void)
* \brief interface function
* \return TRUE recv EVC-0, FALSE otherwise
*/
BOOLEAN8 isReceivedEVC0(void);
/**
* \fn UINT8 isReceivedEVC2(void)
* \brief interface function
* \return TRUE recv EVC-2, FALSE otherwise
*/
BOOLEAN8 isReceivedEVC2(void);
/**
* \fn UINT8 isReceivedEVC8(void)
* \brief interface function
* \return TRUE recv EVC-8, FALSE otherwise
*/
BOOLEAN8 isReceivedEVC8(void);
/**
* \fn UINT8 isReceivedEVC114(void)
* \brief interface function
* \return TRUE recv EVC-14, FALSE otherwise
*/
BOOLEAN8 isReceivedEVC14(void);
/**
* \fn UINT8 isReceivedEVC25(void)
* \brief interface function
* \return TRUE recv EVC-25, FALSE otherwise
*/
BOOLEAN8 isReceivedEVC25(void);
BOOLEAN8 isEVC25IterNull(void);
/**
* \fn UINT8 isReceivedEVC26(void)
* \brief interface function
* \return TRUE recv EVC-26, FALSE otherwise
*/
BOOLEAN8 isReceivedEVC26(void);
/**
* \fn UINT8 GetStm46MmiStmNidSound(void)
* \brief interface function
* \return the value of mmi stm nid sound
*/
UINT8 GetStm46MmiStmNidSound(void);
/**
* \fn UINT8 GetStm46MmiStmQSound(void)
* \brief interface function
* \return the value of mmi stm q sound
*/
UINT8 GetStm46MmiStmQSound(void);
/**
* \fn UINT8 GetStm46MmiStmNIter(void)
* \brief interface function
* \return the value of mmi stm iter
*/
UINT8 GetStm46MmiStmNIter(void);
/**
* \fn BOOLEAN8 GetStm46ZwangbremsungOnOff(UINT8 max_index, BOOLEAN8 *isSound)
* \brief interface function
* \param max_index of for
* \param isSound pointer output TRUE sound on, FALSE sound off
* \return true if the pointer addressed, false viceversa
*/
BOOLEAN8 GetStm46ZwangbremsungOnOff(UINT8 max_index, BOOLEAN8 *isSound);
/**
* \fn BOOLEAN8 GetStm46ZugbeeinflussungOnOff(UINT8 max_index, BOOLEAN8 *isSound)
* \brief interface function
* \param max_index of for
* \param isSound pointer output TRUE sound on, FALSE sound off
* \return true if the pointer addressed, false viceversa
*/
BOOLEAN8 GetStm46ZugbeeinflussungOnOff(UINT8 max_index, BOOLEAN8 *isSound);
/**
* \fn BOOLEAN8 GetStm46SchnarreOnOff(UINT8 max_index, UINT8 *isSound)
* \brief interface function
* \param max_index of for
* \param isSound pointer output 1 sound on, 0 sound off, 2 sound intermittend
* \return true if the pointer addressed, false viceversa
*/
BOOLEAN8 GetStm46SchnarreOnOff(UINT8 max_index, UINT8 *isSound);
/**
* \fn UINT8 GetStm35MmiStmNIter(void)
* \brief interface function
* \return the value of mmi stm nid iter
*/
UINT8 GetStm35MmiStmNIter(void);
/**
* \fn UINT8 GetStm35MmiNidIndpos(UINT8 index)
* \brief interface function
* \return the value of mmi stm nid inpos
*/
UINT8 GetStm35MmiNidIndpos(UINT8 index);
/**
* \fn UINT8 GetStm35MmiNidIcon(UINT8 index)
* \brief interface function
* \return the value of mmi stm nid inpos
*/
UINT8 GetStm35MmiNidIcon(UINT8 index);
/**
* \fn UINT8 GetStm35MmiNidIcator(UINT8 index)
* \brief interface function
* \return the value of mmi stm nid inpos
*/
UINT8 GetStm35MmiNidIcator(UINT8 index);
/**
* \fn UINT16 GetStm35MmiStmNidAttrib(UINT8 index)
* \brief interface function
* \return the value of mmi stm nid inpos
*/
UINT16 GetStm35MmiStmNidAttrib(UINT8 index);
/**
* \fn UINT8 GetEvc25Zda(UINT8 *pZda)
* \brief the functions return the value Zda from the EVC-25 packet
* \param pZda pointer to output
* \return if the packet is received
*/
UINT8 GetEvc25Zda(UINT8 *pZda);
/**
* \fn void void GetEvc25Bra(UINT8 *pBra)
* \param pBra pointer to output
* \brief the functions return the value of Brake type
*/
void GetEvc25Bra(UINT8 *pBra);
/**
* \fn void GetEvc25Brh(UINT8* pBrh0, UINT8* pBrh1, UINT8* pBrh2)
* \brief the functions return the value brake percentage in three digit
* \param pBrh0 pointer to output
* \param pBrh1 pointer to output
* \param pBrh2 pointer to output
*/
void GetEvc25Brh(UINT8* pBrh0, UINT8* pBrh1, UINT8* pBrh2);
/**
* \fn void GetEvc25Vmz(UINT8* pVmz0, UINT8* pVmz1, UINT8* pVmz2)
* \brief the functions return the value maximum speed in three digit
* \param pVmz0 pointer to output
* \param pVmz1 pointer to output
* \param pVmz2 pointer to output
*/
void GetEvc25Vmz(UINT8* pVmz0, UINT8* pVmz1, UINT8* pVmz2);
/**
* \fn void GetEvc25Zl(UINT8* pZl0, UINT8* pZl1, UINT8* pZl2)
* \brief the functions return the value train length in three digit
* \param pZl0 pointer to output
* \param pZl1 pointer to output
* \param pZl2 pointer to output
*/
void GetEvc25Zl(UINT8* pZl0, UINT8* pZl1, UINT8* pZl2);
/**
* \fn void void GetEvc26Bra(UINT8 *pBra)
* \brief the functions return the value of Brake type evc-26 packet
* \param pBra pointer to output
*/
void GetEvc26Bra(UINT8 *pBra);
/**
* \fn void GetEvc26Brh(UINT8* pBrh0, UINT8* pBrh1, UINT8* pBrh2)
* \brief the functions return the value brake percentage in three digit
* \param pBrh0 pointer to output
* \param pBrh1 pointer to output
* \param pBrh2 pointer to output
*/
void GetEvc26Brh(UINT8* pBrh0, UINT8* pBrh1, UINT8* pBrh2);
/**
* \fn void GetEvc26Vmz(UINT8* pVmz0, UINT8* pVmz1, UINT8* pVmz2)
* \brief the functions return the value maximum speed in three digit
* \param pVmz0 pointer to output
* \param pVmz1 pointer to output
* \param pVmz2 pointer to output
*/
void GetEvc26Vmz(UINT8* pVmz0, UINT8* pVmz1, UINT8* pVmz2);
/**
* \fn void GetEvc26Zl(UINT8* pZl0, UINT8* pZl1, UINT8* pZl2)
* \brief the functions return the value train length in three digit
* \param pZl0 pointer to output
* \param pZl1 pointer to output
* \param pZl2 pointer to output
*/
void GetEvc26Zl(UINT8* pZl0, UINT8* pZl1, UINT8* pZl2);
/**
* \fn BOOLEAN8 GetEvc25Feedback(void)
* \brief the functions return the feedback about the traindata entry,
*        the feedback is used to close with a messagedata evc 123
* \return TRUE feedback positve, viceversa FALSE
*/
BOOLEAN8 GetEvc25Feedback(void);


#endif
