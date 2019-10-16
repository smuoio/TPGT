/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_qm_mmi2tsg.c
 *
 *  \author     W. Landwehr
 *
 *  \date       07.11.2011
 *
 *  \brief      Queue manager for packets from MMI to TSG.
 *  To transmit a data package to the TSG it may be possible that more than one
 *  task cycle will be needed (depends on the length of a package).
 *  Thus a queueing mechanism is  necessary to grant that all incoming packets will
 *  be sent in the same sequence like they are received.
 *
 *****************************************************************************
 */

/*-------------------------------------------------------------------------------------------------
 * gpp_qm_mmi2tsg.c
 *  Queue manager for packets from MMI to TSG.
 *  To transmit a data package to the TSG it may be possible that more than one
 *  task cycle will be needed (depends on the length of a package).
 *  Thus a queueing mechanism is  necessary to grant that all incoming packets will
 *  be sent in the same sequence like they are received.
 *
 * Copyright (c) 2011 by Bombardier Transportation (Signal) Germany GmbH (creation: 27.10.2005)
 *
 * HISTORY (creation: 24.06.2009, based on last 4xxx version)
 *  07.11.2011 W. Landwehr  fra_005#772  Minor GPP adaptations (TSGT print-out channal)
 *  10.10.2011 W. Landwehr  fra_005#754  Minor GPP adaptations (print-outs etc.)
 *  02.03.2011 W. Landwehr  fra_005#xxx: adaptation for 5000
 *-------------------------------------------------------------------------------------------------
 */


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <rts_api.h>

#include "gpp_main.h"          /* gpp data types and constants */
#include "gpp_main_util.h"     /* gpp utilities */
#include "gpp_qm_mmi2tsg.h"    /* gpp api defines and prototypes */

/*lint -e921*/ /*allow explicit casting*/


/*******************************************************************************
 * Storage
 ******************************************************************************/
UINT8  gpp2_module_nr = (UINT8)MN_GPP_MMI2TSG;



/*******************************************************************************
 * Public Functions
 ******************************************************************************/
void gpp_tsg_qm_init(void)
{
   /*------------------------------------------------------------------------------------
    * This function initializes the queue for data packages send from MMI to TSG.
    * It runs only one time, called by the init task.
    * It gets the module number for event logging and resets internal
    * data storage with default values.
    *------------------------------------------------------------------------------------
    */
   UINT16 i = (UINT16)0;
   UINT16 j = (UINT16)0;

   if (fp_du_get_module_number)
   {
      /* from TSG */
      gpp2_module_nr = fp_du_get_module_number((SINT8*)__FILE__);
   }

   for (i = (UINT16)0; i < (UINT16)MAX_PACKETS_QUEUE_MMI_TSG; i++)
   {
      for (j = (UINT16)0; j < (UINT16)MAX_GPP_PACKET_LENGTH; j++)
      {
         p_cabin->tsg_packet_queue.element[i].data[j] = (UINT8)0;
      }
      p_cabin->tsg_packet_queue.element[i].length = (UINT16)0;
      p_cabin->tsg_packet_queue.element[i].transmit_state = (UINT16)RETRIEVED;
   }
   p_cabin->tsg_packet_queue.num_stored = (UINT16)0;
   p_cabin->tsg_packet_queue.max_stored = (UINT16)0;
   p_cabin->tsg_packet_queue.entries = (UINT16)0;
   p_cabin->tsg_packet_queue.i_store = (UINT16)0;
   p_cabin->tsg_packet_queue.i_read = (UINT16)(MAX_PACKETS_QUEUE_MMI_TSG - 1);
}



UINT16 gpp_qm_store_tsg_data(const UINT8* p_packet, UINT16 length)
{
   /*------------------------------------------------------------------------------------
    * This function stores one packet from MMI into the TSG packet queue.
    *
    * IN/OUT    p_packet      pointer to the data field
    * IN        length        number of bytes
    *
    * RETURN    GPP_OK        packet stored
    *           GPP_ERR       invalid packet length (0 || MAX_GPP_PACKET_LENGTH),
    *------------------------------------------------------------------------------------
    */
   UINT16 ret = (UINT16)GPP_OK;
   UINT16 ix = (UINT16)0;
   UINT16 j = (UINT16)0;

   if (p_packet == NULL)
   {
      return (UINT16)GPP_ERR;
   }

   ix = p_cabin->tsg_packet_queue.i_store;

   /* occupy queue element if valid length */
   if ((length > MAX_GPP_PACKET_LENGTH) || (length == 0))
   {
      GPP2_EVENT_4("E707: MMI->TSG(%u), msg length invalid (%u byte), nid_stm=%u, nid=%u",
                   gpp_cabin, length, p_packet[0], p_packet[2]);
      ret = (UINT16)GPP_ERR;
   }
   else
   {
      for (j = (UINT16)0; j < length; j++)
      {
         p_cabin->tsg_packet_queue.element[ix].data[j] = p_packet[j];
      }
      p_cabin->tsg_packet_queue.element[ix].length = length;
      p_cabin->tsg_packet_queue.element[ix].transmit_state = (UINT16)IN_QUEUE;

      if (p_cabin->tsg_packet_queue.entries < max_entries_receive)
      {
         p_cabin->tsg_packet_queue.entries++;
      }
      p_cabin->tsg_packet_queue.num_stored++; /* for statistics */
      p_cabin->tsg_packet_queue.cycle_cnt++; /* for statistics */

      if (p_cabin->tsg_packet_queue.cycle_cnt >= STATISTIC_CYCLE_COUNTER)
      {
         /* the new average value */
         p_cabin->tsg_packet_queue.max_stored = p_cabin->tsg_packet_queue.num_stored;
         p_cabin->tsg_packet_queue.cycle_cnt = (UINT16)0;
      }
      else
      {
         if (p_cabin->tsg_packet_queue.max_stored < p_cabin->tsg_packet_queue.num_stored)
         {
            p_cabin->tsg_packet_queue.max_stored = p_cabin->tsg_packet_queue.num_stored;
         }
      }

      if (gpp_trace_enabled == 2)
      {
         if (p_cabin->tsg_packet_queue.element[ix].length)
         {        
            /* ! ! ! don't change the format string ! ! !  */
            /*lint -save -e534*/ /*it is not important if print was successful and we can ignore return value*/
            OS_IO_PRINTF((SINT8*)" \nGPP,%X", gpp_cabin);
            for (j = (UINT16)0; j < p_cabin->tsg_packet_queue.element[ix].length; j++)
            {
               OS_IO_PRINTF((SINT8*)",%2.2X", p_cabin->tsg_packet_queue.element[ix].data[j]);
            }
            /*lint -restore*/
         }
      }

      if (gpp_trace_enabled == 1)
      {
         /* to check, wether all incoming packets are received */
         GPP2_EVENT_1("H712: MMI->TSG (msg queued), i_store=%u", p_cabin->tsg_packet_queue.i_store);
      }

      ix++;
      if (ix >= max_entries_receive)
      {
         ix = (UINT16)0;
      }
      p_cabin->tsg_packet_queue.i_store = ix;
   }
   return ret;
} /* gpp_qm_store_tsg_data */



UINT16 gpp_qm_read_tsg_data(UINT8* p_packet, UINT16 max_packet_length)
{
   /*------------------------------------------------------------------------------------
    * This function reads one packet from the loacl (TSG) queue and returns the packet lenght.
    *
    * IN/OUT   p_packet[]      pointer to the data field
    *
    * RETURN   Packet length
    *------------------------------------------------------------------------------------
    */
   UINT16 i = (UINT16)0;
   UINT16 j = (UINT16)0;
   UINT16 k = (UINT16)0;
   UINT16 length = (UINT16)0;
   UINT16 l_bit = (UINT16)0;
   UINT16 l_byte = (UINT16)0;

   if (p_packet == NULL)
   {
      return (UINT16)0;
   }

   i = p_cabin->tsg_packet_queue.i_read;
   length = (UINT16)0; /* default = no packet found or error */

   /* check queue if there is any further element to transmit
   */
   for (j = (UINT16)0; j < max_entries_receive; j++)
   {
      /* next */ i++;
      if (i >= max_entries_receive)
      {
         i = (UINT16)0;
      }

      if (p_cabin->tsg_packet_queue.element[i].transmit_state == IN_QUEUE)
      {
         /* found a victim...
            STM packet length in bit and byte
         */
         l_bit = (UINT16)(*((UINT16*) &p_cabin->tsg_packet_queue.element[i].data[3]) >> 3);

#ifdef RUNNING_ON_INTEL_MACHINE
         /* low-high byte swap */
         l_bit = p_cabin->tsg_packet_queue.element[i].data[3] << 8;
         l_bit |= p_cabin->tsg_packet_queue.element[i].data[4];
         l_bit = l_bit >> 3;
#endif

         l_byte = (UINT16)((l_bit + 7) / 8);

         /* at first check the plausibility of incomming data stream
         */
         if (p_cabin->tsg_packet_queue.element[i].length > max_packet_length)
         {
            GPP2_EVENT_2("E708: data from MMI exceeds limit of input buffer (input=%u, lng=%u)",
                         max_packet_length, p_cabin->tsg_packet_queue.element[i].length);
         }
         else if (p_cabin->tsg_packet_queue.element[i].data[1] != p_cabin->tsg_packet_queue.element[i].length)
         {
            GPP2_EVENT_3("E709: data from MMI(%u), msg length != recv. length (%u/%u) ",
                         gpp_cabin, p_cabin->tsg_packet_queue.element[i].data[1],
                         p_cabin->tsg_packet_queue.element[i].length);
         }
         /*B2DOS00004108: the check of the STM message length and against lenght of first STM packet is removed*/
         else
         {
            for (k = (UINT16)0; k < p_cabin->tsg_packet_queue.element[i].length; k++)
            {
               p_packet[k] = p_cabin->tsg_packet_queue.element[i].data[k];
            }
            length = p_cabin->tsg_packet_queue.element[i].length;

            if (gpp_trace_enabled == 1)
            {
               GPP2_EVENT_4("E711: data from MMI(%u), wrong packet length=%u (byte=%u/%u) ",
                            p_cabin->tsg_packet_queue.i_store, p_cabin->tsg_packet_queue.i_read,
                            p_cabin->tsg_packet_queue.element[i].length, p_cabin->tsg_packet_queue.entries);
            }
         }

         /* if valid or not, entry is not longer usable
         */
         p_cabin->tsg_packet_queue.i_read = i;
         p_cabin->tsg_packet_queue.num_stored--; /* for statistics */
         p_cabin->tsg_packet_queue.element[i].transmit_state = (UINT16)RETRIEVED;
      } /* if IN_QUEUE */
   }
   return length;
} /* gpp_qm_read_tsg_data */


/* eof */
