/**
* \file Hmi_TrainDataEntry.h
* \brief the file manages the private functions of hmi for the train data entry
* \author Muoio Salvatore
*/

#ifndef __HMI_TRAIN_DATA_ENTRY_H__
#define __HMI_TRAIN_DATA_ENTRY_H__

#include "Hmi_Handle_impl.h"
/**
* \enum e_ZdeBild
* \brief field  mask of train data entry
*/
typedef enum
{
  Zde_Idle     = 0, /*!<Idle Mask */
  Zde_KtrlBild = 1, /*!<Train Data View mask */
  Zde_ZugDtEin = 2, /*!<Train Data Entry mask */
  Zde_Confirm  = 4, /*!<Confirm Mask */
  Zde_TestFun  = 8, /*!<Test Mask */
  Zde_Note     = 16,/*!<Note Mask */
}e_ZdeBild;

/**
* \enum e_FmsSts
* \brief field of state machine
*/
typedef enum
{
  FMS_STS_KONTROL_BILD,     /*!<Kontrol bild Mask */
  FMS_STS_TRAIN_DATA_ENTRY, /*!<Train data entry Mask */
  FMS_STS_NOTE = 3,         /*!<Train data entry note Mask */
  FMS_STS_CONFIRM,          /*!<train data entry confirmation Mask */
  FMS_STS_TEST_FUN = 12,    /*!<Train data entry test fun Mask */
  FMS_STS_UNKNOWN = 255,    /*!<unknown Mask */
}e_FmsSts;
/**
* \fn BOOLEAN8 ExitMaskZDEKontrollBild(const TPG_Commands *const pCmd,
                                 e_ZdeBild *const pZdeBildState);
* \brief The function detects the conditions to exit from The ZDEKontrollBild
* \param pCmd pointer to the structure TPG_Commands
* \param pZdeBildState pointer to enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 ExitMaskZDEKontrollBild(const TPG_Commands *const pCmd,
                                e_ZdeBild *const pZdeBildState);
/**
* \fn BOOLEAN8 ExitMaskZDEZugDatenEingabe(const TPG_Commands * const pCmd,
                                    const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                                    e_ZdeBild *const pZdeBildState);
* \brief The function detects the conditions to exit from The ZDEZugDatenEingabe
* \param pCmd pointer to the structure TPG_Commands
* \param pLzbPzb pointer to the structure TPT_LZBPZBTrainDataInput
* \param pZdeBildState pointer to enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 ExitMaskZDEZugDatenEingabe(const TPG_Commands * const pCmd,
                                    const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                                    e_ZdeBild *const pZdeBildState);

/**
* \fn BOOLEAN8 ExitMaskZDENote(const TPG_Commands * const pCmd,
                               const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                               e_ZdeBild *const pZdeBildState);
* \brief The function detects the conditions to exit from The ZDENote
* \param pCmd pointer to the structure TPG_Commands
* \param pLzbPzb pointer to the structure TPT_LZBPZBTrainDataInput
* \param pZdeBildState pointer to enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 ExitMaskZDENote(const TPG_Commands * const pCmd,
                         const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                         e_ZdeBild *const pZdeBildState);

/**
* \fn BOOLEAN8 ExitMaskZDEConfirm(UINT8 aEvc25Rvcd, e_ZdeBild *const pZdeBildState)
* \brief The function detects the conditions to exit from the ZDEConfirm
* \param aEvc25Rvcd status of EVC-25 packet
* \param pZdeBildState pointer to enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 ExitMaskZDEConfirm(UINT8 aEvc25Rvcd, e_ZdeBild *const pZdeBildState);
/**
* \fn BOOLEAN8 ExitMaskZDETestFun(const TPG_Commands * const pCmd,
                            e_ZdeBild *const pZdeBildState);
* \brief The function detects the conditions to exit from The ZDETestFun
* \param pCmd pointer to the structure TPG_Commands
* \param pZdeBildState pointer to enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 ExitMaskZDETestFun(const TPG_Commands * const pCmd,
                            e_ZdeBild *const pZdeBildState);
/**
* \fn BOOLEAN8 ExitMaskZDETestFun(const TPG_Commands * const pCmd,
                                  e_ZdeBild *const pZdeBildState)
* \brief The function detects the conditions to exit from the ZDETestFun
* \param pCmd pointer to the structure TPG_Commands
* \param pZdeBildState pointer to enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 AbortTraindDataEntry(const TPG_Commands *const pCmd,
                              const TPT_LZBPZBTrainDataInput * const pLzbPzb);
/**
* \fn BOOLEAN8 ShowEffectiveValueTrainDataEntry(TPT_LZBPZBTrainDataDisplay * const pOut,
                                               e_ZdeBild ZdeBildState)
* \brief The function shows the train data entry Data
* \param pOut pointer to the structure TPT_LZBPZBTrainDataDisplay
* \param ZdeBildState variable to the enum structure e_ZdeBild
* \return TRUE if the pointers are addressed, FLASE viceversa
*/
BOOLEAN8 ShowEffectiveValueTrainDataEntry(TPT_LZBPZBTrainDataDisplay * const pOut,
                                          e_ZdeBild ZdeBildState);
/**
* \fn BOOLEAN8 SetValueTrainDataEntry(TPT_LZBPZBTrainDataEdit *const pOut,
                                      e_ZdeBild ZdeBildState);
* \brief The function detects the conditions to exit from The ZDENote
* \param pOut pointer to the structure TPT_LZBPZBTrainDataEdit
* \param ZdeBildState variable to the enum structure e_ZdeBild
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 SetValueTrainDataEntry(TPT_LZBPZBTrainDataEdit *const pOut,
                                e_ZdeBild ZdeBildState);
/**
* \fn BOOLEAN8 TrasmitData(const TPG_Commands *const pCmd,
                    const TPT_LZBPZBTrainDataInput *const pTdi);

* \brief The function creates the data for the packet EVC-123
* \param pCmd pointer to the structure TPG_Commands
* \param pTdi pointer to the structure TPT_LZBPZBTrainDataInput
* \return TRUE if the conditions are fullfilled, FLASE viceversa
*/
BOOLEAN8 TrasmitData(const TPG_Commands *const pCmd,
                    const TPT_LZBPZBTrainDataInput *const pTdi);
/**
* \fn void MaskZDEHandle(const TPG_Commands * const pCmd,
*                        const TPT_LZBPZBTrainDataInput * const pLzbPzb,
*                        DS_CCUT_HMI_TRN * const pOut)
* \brief The function manages the Requirements of start of Mission
* \param pCmd output pointer to the struct TPG_Commands
* \param pLzbPzb input pointer to the struct TPT_LZBPZBTrainDataInput
* \param pOut output pointer to the struct DS_CCUT_HMI_TRN
*/
void MaskZDEHandle(const TPG_Commands * const pCmd,
                   const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                   DS_CCUT_HMI_TRN * const pOut);
/**
* \fn void MaskZDHandle(const TPG_Commands * const pCmd,
*                        const TPT_LZBPZBTrainDataInput * const pLzbPzb,
*                        DS_CCUT_HMI_TRN * const pOut)
* \brief The function manages the Requirements of train data view
* \param pCmd output pointer to the struct TPG_Commands
* \param pLzbPzb input pointer to the struct TPT_LZBPZBTrainDataInput
* \param pOut output pointer to the struct DS_CCUT_HMI_TRN
*/
void MaskZDHandle(const TPG_Commands * const pCmd,
                  const TPT_LZBPZBTrainDataInput * const pLzbPzb,
                  DS_CCUT_HMI_TRN * const pOut);

/**
* \fn BOOLEAN8 ShowValueTrainDataView(TPT_LZBPZBTrainDataDisplay * const pOut)
* \brief The function manages set the data to HMI for the function
*        train data views(ZDView)
* \param pOut output pointer to the struct TPT_LZBPZBTrainDataEdit
* \return TRUE pointer addressed, FALSE viceversa
*/
BOOLEAN8 ShowValueTrainDataView(TPT_LZBPZBTrainDataDisplay * const pOut);

#endif
