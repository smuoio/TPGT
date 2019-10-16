/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_qm_tsg2mmi.h
 *
 *  \author     W. Landwehr
 *
 *  \date       08.07.2011
 *
 *  \brief      Export prototypes and externals for GPP Services TSG to MMI.
 *
 *****************************************************************************
 */


/*------------------------------------------------------------------------------------*/
/**
@file gpp_qm_tsg2mmi.h
Export prototypes and externals for GPP Services TSG to MMI.

@ingroup gpp

@par Copyright
 (c) 2010 by Bombardier Transportation (Signal) Germany GmbH (creation: 02.10.2006)

@par HISTORY (creation: 24.06.2009, based on last 4xxx version)
08.07.2010 W. Landwehr:  fra_005#xxx: adaptation for 5000, moved function
**/
/*------------------------------------------------------------------------------------*/


#ifndef GPP_QM_TSG2MMI_H
#define GPP_QM_TSG2MMI_H


/*******************************************************************************
 * Public Functions
 ******************************************************************************/

/*!
 *  \fn     void gpp_mmi_qm_init(void)
 *
 *  \brief  This function initializes the queue for data packages from TSG to MMI.
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
 *          - OUT: gpp1_module_nr
 *
 */
extern void           gpp_mmi_qm_init(void);

#endif /* GPP_QM_TSG2MMI_H */

/* eof */
