/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2010 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       mqa_api.h
 *
 *  \author     W. Landwehr
 *
 *  \date       29.06.2010
 *
 *  \brief      Exports for MVB application interface for quick TM access.
 *
 *****************************************************************************
 */


/*------------------------------------------------------------------------------------*/
/**
@file mqa_api.h
Exports for MVB application interface for quick TM access.

@ingroup mqa_quick_access

@par Copyright
 (c) 2010 by Bombardier Transportation (Signal) Germany GmbH

@par HISTORY (creation: 25.01.2010, separated from latest 4xxx)
 29.06.2010 W. Landwehr: new
**/
/*------------------------------------------------------------------------------------*/
#include "mwt_types.h"
#include "usertypes.h"
#ifndef MQA_API_H
#define MQA_API_H


/*******************************************************************************
 * Defines
 ******************************************************************************/
#define IX_UNDEF      0xFFFF


/*******************************************************************************
 * Storage
 ******************************************************************************/

/* for adm_info_logging.c */
extern UINT16 (*fp_mqa_ir_get_time_entry)(OS_STR_TIME_POSIX time[], UINT16 max_entries);


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* INIT  */

/*!
 *  \fn     UINT16 mqa_get_port_index(UINT16 port_addr)
 *
 *  \brief  This function shall deliver the port index onto the internal TM address table
 *          for a quick port access.
 *
 *  \param  port_addr - the MVB port address to look for
 *          - [0..809] funtion return 0 
 *          - [=810] funtion return 1
 *          - [=811] funtion return 2
 *          - [=812] funtion return 3
 *          - [=813] funtion return 5
 *          - [=814] funtion return 6
 *          - [=815] funtion return 7
 *          - [816..819] funtion return 0 
 *          - [=820] funtion return 4
 *          - [=821] funtion return 8
 *          - [820..65535] funtion return 0 
 *
 *  \return [1..8] = port index
 *  \return [=0] - port not found
 *
 */
extern UINT16  mqa_get_port_index(UINT16 port_addr);

/*!
 *  \fn     void mqa_init_port_adresses_cab1(BYTE* pP1, BYTE* pP2, BYTE* pP3, BYTE* pF1)
 *
 *  \brief  This function initializes cabin 1.
 *
 *  \param  pP1 - pointer to MVB buffer of control port from TSG to HMI of cabin 1
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *  \param  pP2 - pointer to MVB buffer of data port 1 from TSG to HMI of cabin 1
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *  \param  pP3 - pointer to MVB buffer of data port 2 from TSG to HMI of cabin 1
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *  \param  pF1 - pointer to MVB buffer of control port from HMI to TSG of cabin 1
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - OUT: pMMI_TSG_F1
 *          - OUT: pTSG_MMI_F1_P1
 *          - OUT: pTSG_MMI_F1_P2
 *          - OUT: pTSG_MMI_F1_P3
 */
extern void mqa_init_port_adresses_cab1(BYTE* pP1, BYTE* pP2, BYTE* pP3, BYTE* pF1);

/*!
 *  \fn     void mqa_init_port_adresses_cab2(BYTE* pP1, BYTE* pP2, BYTE* pP3,BYTE* pF1)
 *
 *  \brief  This function initializes cabin 2.
 *
 *  \param  pP1 - pointer to MVB buffer of control port from TSG to HMI of cabin 2
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *  \param  pP2 - pointer to MVB buffer of data port 1 from TSG to HMI of cabin 2
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *  \param  pP3 - pointer to MVB buffer of data port 2 from TSG to HMI of cabin 2
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *  \param  pF1 - pointer to MVB buffer of control port from HMI to TSG of cabin 2
 *          - [=0] function do nothing
 *          - [>0] function working normal
 *
 *  \return none 
 *
 *  \note   global variables used:
 *          - OUT: pMMI_TSG_F2
 *          - OUT: pTSG_MMI_F2_P1
 *          - OUT: pTSG_MMI_F2_P2
 *          - OUT: pTSG_MMI_F2_P3
 */
extern void mqa_init_port_adresses_cab2(BYTE* pP1, BYTE* pP2, BYTE* pP3,BYTE* pF1);

/* R/W   */
/*!
 *  \fn     UINT16* mqa_get_sink_port_tm_addr_by_index(UINT16 ix)
 *
 *  \brief  This function shall deliver the pointer onto the TM port address range of the valid page.
 *
 *  \param  ix -  the port index onto the internal TM address table
 *          - [=0] funtion return NULL
 *          - [=1] funtion return pointer to pTSG_MMI_F1_P1
 *          - [=2] funtion return pointer to pTSG_MMI_F1_P2
 *          - [=3] funtion return pointer to pTSG_MMI_F1_P3
 *          - [=4] funtion return pointer to pMMI_TSG_F1
 *          - [=5] funtion return pointer to pTSG_MMI_F2_P1
 *          - [=6] funtion return pointer to pTSG_MMI_F2_P2
 *          - [=7] funtion return pointer to pTSG_MMI_F2_P3
 *          - [=8] funtion return pointer to pMMI_TSG_F2
 *          - [9..65535] funtion return NULL
 *
 *  \return  pointer onto the port data range in TM or NULL if index not exsits in table
 *
 *  \note   global variables used:
 *          - IN: pMMI_TSG_F1
 *          - IN: pTSG_MMI_F1_P1
 *          - IN: pTSG_MMI_F1_P2
 *          - IN: pTSG_MMI_F1_P3
 *          - IN: pMMI_TSG_F2
 *          - IN: pTSG_MMI_F2_P1
 *          - IN: pTSG_MMI_F2_P2
 *          - IN: pTSG_MMI_F2_P3
 */
extern UINT16* mqa_get_sink_port_tm_addr_by_index(UINT16 ix);

/*!
 *  \fn     UINT16* mqa_get_srce_port_tm_addr_by_index(UINT16 ix)
 *
 *  \brief  This function shall deliver the pointer onto the TM port address range of the valid page.
 *
 *  \param  ix - the port index onto the internal TM address table
 *          - [=0] funtion return NULL
 *          - [=1] funtion return pointer to pTSG_MMI_F1_P1
 *          - [=2] funtion return pointer to pTSG_MMI_F1_P2
 *          - [=3] funtion return pointer to pTSG_MMI_F1_P3
 *          - [=4] funtion return pointer to pMMI_TSG_F1
 *          - [=5] funtion return pointer to pTSG_MMI_F2_P1
 *          - [=6] funtion return pointer to pTSG_MMI_F2_P2
 *          - [=7] funtion return pointer to pTSG_MMI_F2_P3
 *          - [=8] funtion return pointer to pMMI_TSG_F2
 *          - [9..65535] funtion return NULL
 *
 *  \return  pointer onto the port data range in TM or NULL if index not exsits in table
 *
 *  \note   global variables used:
 *          - IN: pMMI_TSG_F1
 *          - IN: pTSG_MMI_F1_P1
 *          - IN: pTSG_MMI_F1_P2
 *          - IN: pTSG_MMI_F1_P3
 *          - IN: pMMI_TSG_F2
 *          - IN: pTSG_MMI_F2_P1
 *          - IN: pTSG_MMI_F2_P2
 *          - IN: pTSG_MMI_F2_P3
 */
extern UINT16* mqa_get_srce_port_tm_addr_by_index(UINT16 ix);

#endif /* MQA_API_H */

/* eof */

