/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_main_util.c
 *
 *  \author     W. Landwehr
 *
 *  \date       03.11.2011
 *
 *  \brief      Utility functions for gpp
 *
 *****************************************************************************
 */


/*-------------------------------------------------------------------------------------------------
 * gpp_main_util.c
 *  Utility functions for gpp_main.c
 *
 * Copyright (c) 2011 by Bombardier Transportation (Signal) Germany GmbH
 *
 * HISTORY (creation: 24.06.2009, based on last 4xxx version)
 *  03.11.2011 W. Landwehr  fra_005#772  Minor GPP adaptations (TSGT print-out channal)
 *  10.10.2011 W. Landwehr  fra_005#754  Minor GPP adaptations (print-outs etc.)
 *  07.10.2011 W. Landwehr  fra_005#747  GPP upgrade regarding ATP activated cabin
 *  11.05.2011 W. Landwehr  adaptation for 5000, moved function
 *-------------------------------------------------------------------------------------------------
 */


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <rts_api.h>

#include "gpp_version.h"
#include "gpp_main.h"
#include "gpp_main_util.h"

/*lint -e921*/ /*allow explicit casting*/

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/



/*******************************************************************************
 * Public Functions
 ******************************************************************************/

void gpp_set_cabin(UINT8 cabin)
{
   /*------------------------------------------------------------------------------------
    * This function sets the data environment for the selected cabin, cabin1 = default.
    * REMARK:
    *  for DCPU only  o n e  cabin (GPP queue) will be used. In case of received packets
    *  from channel 2 (SAP 5/9) this function will be called to switch to cabin 2, but
    *  due to increasing the configured amount of cabins (=1) the instance pointer remains
    *  on the default value.
    *
    * Input: cabin --> cabin to sent data to (0 < cabin <= number of cabins)
    *------------------------------------------------------------------------------------
    */
   if ((cabin > 0) && (cabin <= gpp_nr_of_cabins))
   {
      p_cabin = &gpp_data_cabin[cabin - 1];
      gpp_cabin = cabin;
   }
}


void gpp_trace_enable(UINT8 on_off)
{
   /*------------------------------------------------------------------------------------------
    * This function enables/disables the GPP MVB Trace.:
    * <tsgm gpp show/n>
    *
    * INPUT    val     tracing [on, off] [1/2, 0]
    *-----------------------------------------------------------------------------------------
    */
    /*B2DOS00003086
   SINT8* txt[3] = {(SINT8*)"disabled", (SINT8*)"enabled", (SINT8*)"time stamp"} ;
   */

   if ((on_off == 0) || (on_off == 1) || (on_off == 2))
   {
      gpp_trace_enabled = on_off;
   }
   /*lint -save -e534*/ /*it is not important if print was successful and we can ignore return value*/
   /*B2DOS00003086
   OS_IO_PRINTF((SINT8*)"\nGPP trace %s", txt[gpp_trace_enabled]);
   */
   /*lint -restore*/
}



void gpp_event_dummy(UINT8 modul_nr, UINT16 line_nr,
                     const SINT8* p_txt,
                     UINT16 p1, UINT16 p2, UINT16 p3, UINT16 p4)
{
   /* substitute for du_gpp_event(..).
    * Dummy function to solve fct. ptrs for emergency (not connected by CORE)
    */
   static OS_STR_TIME_POSIX time;
   static OS_STR_TIME_ANSI ansi_time;
   static UINT16 msec;
   static UINT16 event_nr = (UINT16)0;
   static SINT8*           p_modul_name;

   if (p_txt == NULL)
   {
      return;
   }

   if      (modul_nr == gpp0_module_nr)
   {
      p_modul_name = (SINT8*)"gpp_main.c";
   }
   else if (modul_nr == gpp1_module_nr)
   {
      p_modul_name = (SINT8*)"gpp_qm_tsg2mmi.c";
   }
   else
   {
      p_modul_name = (SINT8*)"gpp_qm_mmi2tsg.c";
   }

   event_nr++;
   os_c_get(&time);
   os_c_localtime(time.sec, &ansi_time);
   msec = (UINT16) ((time.nanosec / 1000u) / 1000u);

   /*lint -save -e534*/ /*it is not important if print was successful and we can ignore return value*/
   OS_IO_PRINTF((SINT8*)"\nGPP event[%3.3u] %2.2u %2.2u:%2.2u:%2.2u:%3.3u %s(%d)\n\t  ", event_nr,
                ansi_time.tm_mday, ansi_time.tm_hour, ansi_time.tm_min, ansi_time.tm_sec, msec,
                p_modul_name, line_nr);

   OS_IO_PRINTF(p_txt, p1, p2, p3, p4);

   OS_IO_PRINTF((SINT8*)"\n");
   /*lint -restore*/
}



/*******************************************************************************
 * Private Functions
 ******************************************************************************/


/* eof */
