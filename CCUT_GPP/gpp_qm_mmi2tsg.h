/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_qm_mmi2tsg.h
 *
 *  \author     W. Landwehr
 *
 *  \date       27.04.2011
 *
 *  \brief      Export prototypes and externals for GPP Services MMI to TSG.
 *
 *****************************************************************************
 */


/*------------------------------------------------------------------------------------*/
/**
@file gpp_qm_mmi2tsg.h
Export prototypes and externals for GPP Services MMI to TSG.

@ingroup gpp

@par Copyright
 (c) 2011 by Bombardier Transportation (Signal) Germany GmbH (creation: 02.10.2006)

@par HISTORY (creation: 24.06.2009, based on last 4xxx version)
27.04.2011 W. Landwehr:  fra_005#xxx: adaptation for 5000
**/
/*------------------------------------------------------------------------------------*/

#ifndef GPP_QM_MMI2TSG_H
#define GPP_QM_MMI2TSG_H



/*******************************************************************************
 * Public Functions
 ******************************************************************************/

/*!
 *  \fn     void gpp_tsg_qm_init(void)
 *
 *  \brief  This function initializes the queue for data packages send from MMI to TSG.
 *          It runs only one time, called by the init task.
 *          It gets the module number for event logging and resets internal
 *          data storage with default values.
 *
 *  \param  none
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: fp_du_get_module_number
 *          - IN: p_cabin
 *          - OUT: gpp_data_cabin
 *          - OUT: gpp2_module_nr
 */
extern void           gpp_tsg_qm_init(void);

/*!
 *  \fn     UINT16 gpp_qm_store_tsg_data(const UINT8* p_packet, UINT16 length)
 *
 *  \brief  This function stores one packet from MMI into the TSG packet queue.
 *
 *  \param  p_packet - pointer to the data field
 *          - [=0] function do nothing and returns GPP_ERR
 *          - [>0] function working normal
 *  \param  length - number of bytes
 *          - [=0] function do nothing and returns GPP_ERR
 *          - [1..MAX_GPP_NR_OF_CABINS] function works normal and returns GPP_OK
 *          - [MAX_GPP_NR_OF_CABINS..255] function do nothing and returns GPP_ERR
 *
 *  \return GPP_OK 
 *          - packet stored
 *  \return GPP_ERR 
 *          - invalid packet length (0 || MAX_GPP_PACKET_LENGTH) or parameter NULL
 *
 *  \note   global variables used:
 *          - IN: gpp_trace_enabled
 *          - IN: p_cabin
 *          - IN: max_entries_receive
 *          - IN/OUT: gpp_data_cabin
 */
extern UINT16         gpp_qm_store_tsg_data(const UINT8* p_packet, UINT16 length);

#endif /* GPP_QM_MMI2TSG_H */

/* eof */

