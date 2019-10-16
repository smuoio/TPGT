/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2010 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_main_util.h
 *
 *  \author     W. Landwehr
 *
 *  \date       29.06.2010
 *
 *  \brief      Export prototypes and externals for GPP main utilities.
 *
 *****************************************************************************
 */

/*------------------------------------------------------------------------------------*/
/**
@file gpp_main_util.h
Export prototypes and externals for GPP main utilities.

@ingroup gpp

@par Copyright
 (c) 2010 by Bombardier Transportation (Signal) Germany GmbH

@par HISTORY (creation: 24.06.2009, based on last 4xxx version)
29.06.2010 W. Landwehr: fra_005#xxx: adaptation for 5000
**/
/*------------------------------------------------------------------------------------*/


#ifndef GPP_MAIN_UTIL_H
#define GPP_MAIN_UTIL_H



/*******************************************************************************
 * Public Functions for GPP main function
 ******************************************************************************/

/*!
 *  \fn     void gpp_event_dummy(UINT8 modul_nr, UINT16 line_nr, const SINT8* p_txt, UINT16 p1, UINT16 p2, UINT16 p3, UINT16 p4)
 *
 *  \brief  Dummy function to solve fct. ptrs for emergency (not connected by CORE)
 *
 *  \param  modul_nr - an gpp modul number that is used for logging
 *          - [=gpp0_module_nr] function set p_modul_name to "gpp_main.c"
 *          - [=gpp1_module_nr] function set p_modul_name to "gpp_qm_tsg2mmi.c"
 *          - [!=gpp0_module_nr AND !=gpp1_module_nr] function set p_modul_name to "gpp_qm_mmi2tsg.c"
 *  \param  line_nr - a code line number that is used for logging
 *          - [0..65535] function works normal
 *  \param  p_txt - a pointer to a zero terminated string that is used vor logging
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  p1 - parameter number that is used for logging
 *          - [0..65535] function works normal
 *  \param  p2 - parameter number that is used for logging
 *          - [0..65535] function works normal
 *  \param  p3 - parameter number that is used for logging
 *          - [0..65535] function works normal
 *  \param  p4 - parameter number that is used for logging
 *          - [0..65535] function works normal
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: gpp0_module_nr
 *          - IN: gpp1_module_nr
 *
 */
extern void          gpp_event_dummy(UINT8 modul_nr, UINT16 line_nr,
                                     const SINT8* p_txt,
                                     UINT16 p1, UINT16 p2, UINT16 p3, UINT16 p4);

/* the GPP application interface used in GPP only */

#endif /* GPP_MAIN_UTIL_H */

/* eof */
