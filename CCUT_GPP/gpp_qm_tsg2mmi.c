/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_qm_tsg2mmi.c
 *
 *  \author     W. Landwehr
 *
 *  \date       03.10.2011
 *
 *  \brief      Queue manager for packets from TSG to MMI.
 *  To transmit a data package to the MMI usually more than one task cycle will be
 *  needed (depends on the length of a package). Thus a queueing mechanism is
 *  necessary to grant that all incoming packets will be sent in the same sequence
 *  like they are received.
 *
 *****************************************************************************
 */

/*-------------------------------------------------------------------------------------------------
 * gpp_qm_tsg2mmi.c
 *  Queue manager for packets from TSG to MMI.
 *  To transmit a data package to the MMI usually more than one task cycle will be
 *  needed (depends on the length of a package). Thus a queueing mechanism is
 *  necessary to grant that all incoming packets will be sent in the same sequence
 *  like they are received.
 *
 * Copyright (c) 2011 by Bombardier Transportation (Signal) Germany GmbH
 *
 * HISTORY (creation: 24.06.2009, based on last 4xxx version)
 *  03.11.2011 W. Landwehr  fra_005#772  Minor GPP adaptations (TSGT print-out channal)
 *  11.10.2011 W. Landwehr  fra_005#754  Minor GPP adaptations (print-outs etc.)
 *  06.10.2011 W. Landwehr  fra_005#747  GPP upgrade regarding ATP activated cabin
 *  22.06.2011 W. Landwehr  fra_005#625: test switch check upgraded
 *  10.05.2011 W. Landwehr  fra_005#xxx: adaptation for 5000, moved function
 *-------------------------------------------------------------------------------------------------
 */


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <rts_api.h>

#include "gpp_main.h"        /* gpp data types and constants */
#include "gpp_main_util.h"   /* gpp utilities */
#include "gpp_qm_tsg2mmi.h"  /* gpp api defines and prototypes */

/*lint -e921*/ /*allow explicit casting*/

/*******************************************************************************
 * Storage
 ******************************************************************************/
UINT8  gpp1_module_nr = (UINT8)MN_GPP_TSG2MMI;



/*******************************************************************************
 * Public Functions
 ******************************************************************************/
void gpp_mmi_qm_init(void)
{
   /*-----------------------------------------------------------------------------------------
    * This function initializes the queue for data packages from TSG to MMI.
    * It runs only one time, called by the init task.
    * It gets the module number for event logging and resets internal
    * data storage with default values.
    *-----------------------------------------------------------------------------------------
    */
   UINT16 i = (UINT16)0;
   UINT16 j = (UINT16)0;

   if (fp_du_get_module_number)
   {
      /* from TSG */
      gpp1_module_nr = fp_du_get_module_number((SINT8*)__FILE__);
   }

   for (i = (UINT16)0; i < (UINT16)MAX_PACKETS_QUEUE_TSG_MMI; i++)
   {
      for (j = (UINT16)0; j < (UINT16)MAX_GPP_PACKET_LENGTH; j++)
      {
         p_cabin->mmi_packet_queue.element[i].data[j] = (UINT8)0;
      }
      p_cabin->mmi_packet_queue.element[i].length = (UINT16)0;
      p_cabin->mmi_packet_queue.element[i].transmit_state = (UINT16)TRANSMITTED;
   }
   p_cabin->mmi_packet_queue.num_stored = (UINT16)0;
   p_cabin->mmi_packet_queue.max_stored = (UINT16)0;
   p_cabin->mmi_packet_queue.num_lost = (UINT16)0;
   p_cabin->mmi_packet_queue.cycle_cnt = (UINT16)0;
   p_cabin->mmi_packet_queue.entries = (UINT16)0;
   p_cabin->mmi_packet_queue.i_store = (UINT16)0;
   p_cabin->mmi_packet_queue.i_read = (UINT16)(MAX_PACKETS_QUEUE_TSG_MMI - 1);
   p_cabin->mmi_packet_queue.diag_err = (UINT16)0;
}



UINT16 gpp_qm_store_mmi_data(const UINT8* p_packet, UINT8 nid_stm, UINT16 length)
{
   /*-----------------------------------------------------------------------------------------
    * This function stores one packet into the remote (MMI) packet queue.
    *
    * IN         p_packet --> pointer onto the field with application data
    *            nid_stm  --> identifier of the sender (STM, ATP ,TSG)
    *            length   --> number of bytes of the application data
    *
    * RETURN     GPP_OK       -->  packet stored
    *            GPP_ERR      -->  invalid packet length (0 || MAX_GPP_PACKET_LENGTH),
    *                              or no queue element empty
    *            GPP_NO_CONNECTION --> the GPP connection is not yet established
    *-----------------------------------------------------------------------------------------
    */
   UINT16 ret = (UINT16)GPP_OK;
   UINT16 i = (UINT16)0;
   UINT16 j = (UINT16)0;

   if (p_packet == NULL)
   {
      return (UINT16)GPP_ERR;
   }

   if (p_cabin->gpp_conn_state_local == GPP_CONN_RUNNING)
   {
      /* this connection is running */
      i = p_cabin->mmi_packet_queue.i_store;

      if (p_cabin->mmi_packet_queue.element[i].transmit_state != TRANSMITTED)
      {
         /* The queue is filled (the oldest is not yet transmitted) !
            err msg only three times
         */
         p_cabin->mmi_packet_queue.num_lost++;

         if (p_cabin->mmi_packet_queue.num_lost < 4)
         {
            GPP1_EVENT_3("E713: MMI[%u] queue limit=%u exceeded (lost=%u)",
                         gpp_cabin, MAX_PACKETS_QUEUE_TSG_MMI, p_cabin->mmi_packet_queue.num_lost);
         }

         if (!p_cabin->mmi_packet_queue.diag_err)
         {
            GPP1_EVENT_1("E714: MMI[%u] queue overflow --> ATP/MMI  D I S C O N N E C T", gpp_cabin);
            if (fp_gpp_diag_entry)
            {
               fp_gpp_diag_entry(O_GPP_QUEUE_TSG_MMI, gpp_cabin, 0, 714);  /* DIAG entry */
            }
            p_cabin->mmi_packet_queue.diag_err = (UINT16)TRUE;
         }
         /* disable processing within the frame */
         p_cabin->mmi_queue_overflow = (UINT8)TRUE;
         ret = (UINT16)GPP_ERR;
      }
      else
      {
         /* empty queue element, occupy it if valid length */
         if ((length > MAX_GPP_PACKET_LENGTH) || (length == 0))
         {
            GPP1_EVENT_4("E715: TSG->MMI(%u), msg length invalid (%u byte), nid_stm=%u, p[0]='%2.2x'",
                         gpp_cabin, length, nid_stm, p_packet[0]);
            ret = (UINT16)GPP_ERR;
         }
         else
         {
            /* supplement of the header structure

            |<------------------------FFFIS message----------------------->|
            |                 |<----------- FFFIS packet 1:1 ------------->|
            |                 |                          |MMI-pack|        |
            +--------+--------+--------+-----------------+---  ---+--------+
            |11111110| length |11010101|   l_packet  |xxx|  ...   |11111111|
            +--------+--------+--------+-----------------+---  ---+--------+
              FE=254             D6=213       |        |      |       |
                |        |         |          |        |      |       +---- telegram end
                |        |         |          |        |      +------------ MMI content
                |        |         |          |        +----------+-------- 3 padding bits
                |        |         |          +-------------------+-------- packet length in bit
                |        |         +------------------------------+-------- NID=214: MMI->TSG
                |        +----------------------------------------+-------- message length in byte
                +-------------------------------------------------+-------- NID_STM
                                                                  |
                                                             to supplement
            */
            p_cabin->mmi_packet_queue.element[i].data[0] = nid_stm;  /* 254, 253, 10 */
            p_cabin->mmi_packet_queue.element[i].data[1] = (UINT8)(length + 2); /* number of all bytes */

            for (j = (UINT16)0; j < length; j++)
            {
               p_cabin->mmi_packet_queue.element[i].data[j + 2] = p_packet[j];
            }
            p_cabin->mmi_packet_queue.element[i].length = (UINT16)(length + 2);
            p_cabin->mmi_packet_queue.element[i].transmit_state = (UINT16)IN_QUEUE;

            if (p_cabin->mmi_packet_queue.entries < max_entries_send)
            {
               p_cabin->mmi_packet_queue.entries++;
            }
            p_cabin->mmi_packet_queue.num_stored++; /* for statistics */
            p_cabin->mmi_packet_queue.cycle_cnt++; /* for statistics */

            if (p_cabin->mmi_packet_queue.cycle_cnt >= STATISTIC_CYCLE_COUNTER)
            {
               /* the new average value */
               p_cabin->mmi_packet_queue.max_stored = p_cabin->mmi_packet_queue.num_stored;
               p_cabin->mmi_packet_queue.cycle_cnt = (UINT16)0;
            }
            else
            {
               if (p_cabin->mmi_packet_queue.max_stored < p_cabin->mmi_packet_queue.num_stored)
               {
                  p_cabin->mmi_packet_queue.max_stored = p_cabin->mmi_packet_queue.num_stored;
               }
            }

            i++;
            /*if (i >= MAX_PACKETS_QUEUE_TSG_MMI) {i = 0;}*/
            if (i >= max_entries_send)
            {
               i = (UINT16)0;
            }
            p_cabin->mmi_packet_queue.i_store = i;

            if (gpp_trace_enabled == 1)
            {
               GPP1_EVENT_4("H713: TSG->MMI (msg queued), i_store=%u, i_read=%u, len=%u, entries=%u",
                            p_cabin->mmi_packet_queue.i_store, p_cabin->mmi_packet_queue.i_read,
                            p_cabin->mmi_packet_queue.element[i].length, p_cabin->mmi_packet_queue.entries);
            }

            /* check MMI queue level:
               In case of reached warning level display a warn msg. on the MMI, Monitor and AE log.
            */
            if (p_cabin->mmi_packet_queue.num_stored > WARNING_LIMIT_QUEUE_TSG_MMI)
            {
               if (p_cabin->warn_limit == 0)
               {
                  p_cabin->warn_limit = p_cabin->mmi_packet_queue.num_stored;
                  GPP1_EVENT_1("H714: warning! MMI queue reached limit of %u entries", p_cabin->mmi_packet_queue.num_stored);

                  /* do a recursive call of this function to store a MMI hint */
                  if (fp_gpp_send_text)
                  {
                     fp_gpp_send_text(gpp_cabin, (SINT8*)"H715: TSG-MMI: GPP Queue reached warning limit");
                  }
               }
            }
            else if (p_cabin->mmi_packet_queue.num_stored < (WARNING_LIMIT_QUEUE_TSG_MMI - 10))
            {
               /* reset warning hint disable */
               p_cabin->warn_limit = (UINT16)0;
            }
            else
            {
               /*do nothing*/
            }
         }
      }
   }
   else
   {
      /* received for inactive cabin: should not happen
         (fra_005#150: suppress msg. E716 before connection established)
         */
      ret = (UINT16)GPP_NO_CONNECTION;
   }
   return ret;
} /* gpp_qm_store_mmi_data */



void gpp_qm_read_mmi_data(void)
{
   /*-----------------------------------------------------------------------------------------
    * This function checks the GPP state whether a transmission is finished. In this case
    * a possibly new data stream from the MMI queue is read and stored into the internal
    * send buffer for transmission to the remote (MMI) by calling the function
    * gpp_write_data_packets().
    *-----------------------------------------------------------------------------------------
    */
   UINT16 k = (UINT16)0u;
   UINT16 j = (UINT16)0u;
   UINT16 i = (UINT16)0u;
   
   i = p_cabin->mmi_packet_queue.i_read;

   if (p_cabin->gpp_enabled)
   {
      if (p_cabin->gpp_conn_state_local == GPP_CONN_RUNNING)
      {
         p_cabin->warn_limit2 = (UINT8)FALSE; /* enable message output */

         if (p_cabin->gpp_work_state == GPP_WAIT_FOR_CMD)
         {
            /* no current transmission or last just finished
            */
            i = p_cabin->mmi_packet_queue.i_read;

            if (p_cabin->mmi_packet_queue.element[i].transmit_state == SENDING)
            {
               /* this was just sent, now finished
               */
               p_cabin->mmi_packet_queue.element[i].transmit_state = (UINT16)TRANSMITTED;
            }

            /* check queue if there is any further element to transmit
            */
            for (j = (UINT16)0; j < max_entries_send; j++)
            {
               i++; /* next */
               if (i >= max_entries_send)
               {
                  i = (UINT16)0;
               }
               if (p_cabin->mmi_packet_queue.element[i].transmit_state == IN_QUEUE)
               {
                  /* found a victim...
                  */
                  if (p_cabin->mmi_packet_queue.element[i].length &&
                        (p_cabin->mmi_packet_queue.element[i].length <= MAX_GPP_PACKET_LENGTH))
                  {
                     /* range valid, fill the send buffer
                     */
                     for (k = (UINT16)0; k < p_cabin->mmi_packet_queue.element[i].length; k++)
                     {
                        p_cabin->gpp_send_buf[k] = p_cabin->mmi_packet_queue.element[i].data[k];
                     }
                     p_cabin->gpp_send_buf_size = p_cabin->mmi_packet_queue.element[i].length;
                     p_cabin->gpp_work_state = (UINT8)GPP_START_CMD;

                     /* queue element transfered to GPP write buffer
                     */
                     p_cabin->mmi_packet_queue.element[i].transmit_state = (UINT16)SENDING;
                     p_cabin->mmi_packet_queue.i_read = i;
                     p_cabin->mmi_packet_queue.num_stored--; /* for statistics */

                     if (gpp_trace_enabled == 1)
                     {
                        GPP1_EVENT_4("H716: TSG->MMI (msg read), i_store=%u, i_read=%u, len=%u, entries=%u",
                                     p_cabin->mmi_packet_queue.i_store, p_cabin->mmi_packet_queue.i_read,
                                     p_cabin->mmi_packet_queue.element[i].length, p_cabin->mmi_packet_queue.entries);
                     }
                  }
                  else
                  {
                     p_cabin->mmi_packet_queue.element[i].transmit_state = (UINT16)TRANSMITTED; /* reject */
                     p_cabin->mmi_packet_queue.i_read = i;
                     p_cabin->mmi_packet_queue.num_stored--; /* for statistics */

                     GPP1_EVENT_2("E702: GPP(cab=%u): buf_size to large = %u", gpp_cabin, p_cabin->mmi_packet_queue.element[i].length);
                     GPP1_EVENT_2("E717: TSG->MMI[%u] element %u rejected", gpp_cabin, i);
                  }
                  break;
               }
            } /* for */
         } /* if GPP_WAIT_FOR_CMD */
      }
      else
      {
         if (!p_cabin->warn_limit2)
         {
            GPP1_EVENT_1("H702: GPP(cab=%u): MMI not yet connected", gpp_cabin);
            p_cabin->warn_limit2 = (UINT8)TRUE;
         }
      }
   }
} /* gpp_qm_read_mmi_data */



UINT8 gpp_mmi_queue_overflow(UINT8 cabin)
{
   /*-----------------------------------------------------------------------------------------
    * This function returns the queue overflow flag.
    *
    * IN      cabin    cabin instance 1/2 (0 < cabin <= number of cabins)
    *-----------------------------------------------------------------------------------------
    */
   UINT8 ret = (UINT8)TRUE;
   static UINT16 event_brake = (UINT16)0;

   if ((cabin > 0) && (cabin <= gpp_nr_of_cabins))
   {
      ret = gpp_data_cabin[cabin - 1].mmi_queue_overflow;
   }
   else
   {
      if (event_brake < 5)
      {
         GPP1_EVENT_1("E712: wrong MMI nr: %u --> Queue Overflow", cabin);
         event_brake++;
      }
   }
   return ret;
} /*gpp_mmi_queue_overflow*/

/* eof */
