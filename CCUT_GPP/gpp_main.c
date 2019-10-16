/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2013 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_main.c
 *
 *  \author     K. Zingler
 *
 *  \date       13.03.2013
 *
 *  \brief      GPP Services to send larger data packets than 32 byte to the MMI.
 *              Contains all application interfaces and the transmission state machine.
 *
 *****************************************************************************
 */


/*-------------------------------------------------------------------------------------------------
 * gpp_main.c
 *  GPP Services to send larger data packets than 32 byte to the MMI.
 *  Contains all application interfaces and the transmission state machine.
 *
 * Copyright (c) 2011 by Bombardier Transportation (Signal) Germany GmbH
 *-------------------------------------------------------------------------------------------------
 * B A S I C S
 * GPP Services are used to send larger data packets than 32 byte to the MMI.
 *
 * Interface
 *     There are 3 GPP ports to send the data from TSG to MMI
 *     and one port to get the acknowledge from the MMI to recognize the correct transmission.
 *     Each port is 32 byte long.
 *
 * Basics
 *     To check the transmission a sequence number in the range from 0 to 7 will be used.
 *     This seq. nr. returns the MMI to the TSG as acknowledge.
 *     Each of  the 3 send ports contains as last byte the seq. nr.
 *     In addition the 1st port contains further control information as there be the packet size
 *     (that means the complete length of the data to be transmitted),
 *     and further more a packet index to recognize the position of the currently
 *     transmitted data package within the complete data package.
 *     This index is a multiple of 90 bytes (the data length of 3 ports).
 *     If index e.g. is 2 the data packet being send has byte position 180-270 within the
 *     complete package.
 *     If the complete data package is less than 90 byte, or the last sub packet is less then
 *     90 byte, the remaining bytes of the 3-port set will be filled with 0 (zero) values.
 *
 * Turbo-Access-Solution
 *     (This is without using TCN-Stack or MTPE-Terminals)
 *     All needed address pointer for quick access will be builded within the initialization.
 *     By using this table a direct mapping of the MVB ports is possible.
 *     The port data will be copied into a work-version into the MVB-Database.
 *     Access to the process data (such as boolean, vars) then happens easy and comfortable
 *     with a simple structure access.
 *
 * Remark
 *     The current SW version is realized for ONE cabin (selected over port definition)
 *     If there should be TWO cabins via MMI at the same time available it must be
 *     clearyfied how the processing must be handled: only one at time or both parallel.
 *
 *     WARNING: This code is not writen for be ported to a different CPU with different
 *              byte-ordering in a WORD (UINT16)
 *
 * Requirements
 *     The input requirements for this module are contained in the following documents:
 *     - [MMI_IS] - 3NGM004010 - MMI Interface Specification
 *-------------------------------------------------------------------------------------------------
 * HISTORY (creation: 24.06.2009, based on last 4xxx version)
 *  13.03.2013 K. Zingler                Resend ack frame after receiving nwe frame in case of waiting for ack as well
 *  08.03.2012 W. Landwehr  fra_005#873  Update GPP lib (minor change due to test env.)
 *  07.11.2011 W. Landwehr  fra_005#772  Minor GPP adaptations (TSGT print-out channel)
 *  07.10.2011 W. Landwehr  fra_005#747  GPP upgrade regarding ATP activated cabin
 *  22.06.2011 W. Landwehr  fra_005#625  test switch check upgraded
 *  28.01.2011 W. Landwehr  adaptation for 5000, moved function
 *-------------------------------------------------------------------------------------------------
 */


/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <rts_api.h>
#include <mqa_api.h>

#include "gpp_main.h"
#include "gpp_main_util.h"
#include "gpp_qm_tsg2mmi.h"
#include "gpp_qm_mmi2tsg.h"

#define OFFSET_1_PORT  0
#define OFFSET_2_PORT  (PORT_SIZE_GPP / 2)
#define OFFSET_3_PORT  ((PORT_SIZE_GPP + PORT_SIZE_GPP) / 2)
#define PORT_LOGGING(PORT, BUFFER) { \
    if (p_cabin->gpp_logging_enabled && fp_du_port_data) {fp_du_port_data(PORT, (UINT16 *) BUFFER);} \
  }




/*******************************************************************************
 * Defines
 ******************************************************************************/

/* this defines the current GPP scope:
   maximum number of served cabins -> MMI channels
   (may be increased by a software upgrade if necessary)
   (not the project specific number of cabins !! These are defined in mdu_port_nvs.h)
  */
#define MAX_GPP_NR_OF_CABINS    2


/** size of a GPP ports
    1 - always used maximum MVB port size -> 32
    2 - maximum number of ports currently = 3 -> increasing needs SW update !!
    */
#define PORT_SIZE_GPP           32
#define MAX_PORTS_GPP           3
#define MAX_RECV_BUF_LNG        (PORT_SIZE_GPP * MAX_PORTS_GPP)

/* Connection initialization states
*/
#define GPP_CONN_IDLE           9
#define GPP_CONN_GET_READY      0
#define GPP_CONN_READY          1
#define GPP_CONN_START          2
#define GPP_CONN_RUNNING        3

/* Packet types to fill */
#define SUB_PACKETS_APPLICATION 201
#define SUB_PACKETS_CONNECTION  202

#define TRANSMIT 1111
#define RECEIVE  2222


/*******************************************************************************
 * Storage
 ******************************************************************************/
/*lint -e921*/ /*allow explicit casting*/

UINT8    gpp_nr_of_cabins = (UINT8)0;  /* configured number of cabins: set by gpp_init_ports(..) */
UINT8    atp_active_cabin;      /* from ATP 213.evc2.mmi_m_active_cabin */

UINT16   nr_of_gpp_local_ports;
UINT16   nr_of_gpp_remote_ports;
UINT16   max_entries_receive;   /* depends on the direction */
UINT16   max_entries_send;      /* depends on the direction */

s_gpp_main_data  gpp_data_cabin[MAX_GPP_NR_OF_CABINS]; /* data instance for all cabins */
s_gpp_main_data* p_cabin = &gpp_data_cabin[0];         /* current switched data instance */
UINT8    gpp_cabin = (UINT8)0;                        /* current switched cabin: 1 - gpp_nr_of_cabins */

/* seq/ack tracing GPP protocol (only for the selected cabin )
*/
s_gpp_seq_trace  gpp_seq_trace[MAX_SEQ_TRACE];
UINT16   gpp_seq_trace_curr_entry = (UINT16)0;

UINT8    gpp_trace_enabled = (UINT8)0;    /* also for gpp_qm_mmi2tsg.c gpp_qm_tsg2mmi.c */

UINT8    gpp0_module_nr = (UINT8)MN_GPP_MAIN; /* default if no TSG is used */

/* to connect the du_event and trace functions
   (if no TSG available these pointers are set to GPP internal dummy functions)
*/
UINT8 (*fp_du_get_module_number)(SINT8* p_mn  /* __FILE__ */);
void          (*fp_du_gpp_event)        (UINT8 modul_nr, UINT16 lnr,
      SINT8* p_text,
      UINT16 p1, UINT16 p2, UINT16 p3, UINT16 p4);


/* to connect several functions
   (if no TSG available, these functions are not called)
*/
void          (*fp_du_port_data)        (UINT16 port_addr, UINT16* port_data);
void          (*fp_gpp_send_text)       (UINT8 cabin, SINT8* p_msg);
void          (*fp_gpp_diag_entry)      (UINT16 entry_nr,
      UINT8 p1, UINT8 p2, UINT16 err_nr);

/* for MVB address import from NVS part
*/
static UINT16 gpp_port_ctrll[MAX_GPP_NR_OF_CABINS];
static UINT16 gpp_port_data1[MAX_GPP_NR_OF_CABINS];
static UINT16 gpp_port_data2[MAX_GPP_NR_OF_CABINS];
static UINT16 gpp_port_ctrlr[MAX_GPP_NR_OF_CABINS];


/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*!
 *  \fn     void  gpp_remote_print_time(UINT16 dir)
 *
 *  \brief  Displays a time stamp as TRACE support
 *
 *  \param  dir - direction TRANSMIT/RECEIVE
 *
 *  \return none
 *
 */
static void  gpp_remote_print_time(UINT16 dir);

/*!
 *  \fn     UINT8 gpp_init_cabin_environment(void)
 *
 *  \brief  This function performs the mvb database internal (NVS specific) initialization:
 *  - set TS addresses,
 *  - reset tables
 *  - build tables for quick TM access
 *  - start msg
 *
 *  \param  none
 *
 *  \return function succesful TRUE/FALSE
 *
 */
static UINT8 gpp_init_cabin_environment(void);

/*!
 *  \fn     void gpp_fill_sub_packets(UINT8 type)
 *
 *  \brief  The sub packets will be filled withe the current stream of the
 *          application data packet.
 *
 *  \param  type - 1 = with appl. data, 2 = with connection data
 *
 *  \return none
 *
 */
static void  gpp_fill_sub_packets(UINT8 type);

/*!
 *  \fn     void gpp_resend_sub_packets(void)
 *
 *  \brief  The sub packets will be filled withe the actual stream of the
 *          application data packet a second time, but with new ack value.
 *
 *  \param  none
 *
 *  \return none
 *
 */
static void  gpp_resend_sub_packets(void);

/*!
 *  \fn     void gpp_fill_app_packets(UINT16* p_ctrl, UINT16* p_data_1, UINT16* p_data_2)
 *
 *  \brief  The sub packets will be filled withe the current stream of the
 *          application data packet.
 *
 *  \param  p_ctrl    -      ptr. onto area of ctrl port
 *  \param  p_data_1  -      ptr. onto area of data port 1
 *  \param  p_data_2  -      ptr. onto area of data port 2
 *
 *  \return none 
 *
 */
static void  gpp_fill_app_packets(UINT16* p_ctrl, UINT16* p_data_1, UINT16* p_data_2);

/*!
 *  \fn     void gpp_resend_app_packets(UINT16* p_ctrl, UINT16* p_data_1, UINT16* p_data_2)
 *
 *  \brief  The sub packets will be filled withe the actuial stream of the
 *          application data packet a second time.
 *
 *  \param  p_ctrl    -      ptr. onto area of ctrl port
 *  \param  p_data_1  -      ptr. onto area of data port 1
 *  \param  p_data_2  -      ptr. onto area of data port 2
 *
 *  \return none 
 *
 */
static void  gpp_resend_app_packets(UINT16* p_ctrl, UINT16* p_data_1, UINT16* p_data_2);

/*!
 *  \fn     void gpp_fill_conn_packets(UINT16* p_ctrl, UINT16* p_data_1, UINT16* p_data_2)
 *
 *  \brief  The sub packets will be filled with zero and the connection state
 *          within the control port. (there will be no seqnr incrementation !)
 *
 *  \param  p_ctrl    -      ptr. onto area of ctrl port
 *  \param  p_data_1  -      ptr. onto area of data port 1
 *  \param  p_data_2  -      ptr. onto area of data port 2
 *
 *  \return none 
 *
 */
static void  gpp_fill_conn_packets(UINT16* p_ctrl, UINT16* p_data_1, UINT16* p_data_2);



/*******************************************************************************
 * Public Functions
 ******************************************************************************/
void gpp_init_ports(UINT8 cabin,
                    UINT16 ctrll_port, UINT16 data_port1, UINT16 data_port2,
                    UINT16 ctrlr_port)
{
   /*----------------------------------------------------------------------------------------
    * This function set the project specific GPP port addresses for internal use.
    * (The 1st function to call - before gpp_init)
    *----------------------------------------------------------------------------------------
    */
   if (cabin && (cabin <= MAX_GPP_NR_OF_CABINS))
   {
      /* the port addresses */
      gpp_port_ctrll[cabin - 1] = ctrll_port;
      gpp_port_data1[cabin - 1] = data_port1;
      gpp_port_data2[cabin - 1] = data_port2;
      gpp_port_ctrlr[cabin - 1] = ctrlr_port;
      gpp_nr_of_cabins = cabin;
   }
}



void gpp_init(UINT16 nr_of_local_ports, UINT16 nr_of_remote_ports,
              BYTE* ctrll_port_cab1, BYTE* data_port1_cab1, BYTE* data_port2_cab1, BYTE* ctrlr_port_cab1,
              BYTE* ctrll_port_cab2, BYTE* data_port1_cab2, BYTE* data_port2_cab2, BYTE* ctrlr_port_cab2)
{
   /*----------------------------------------------------------------------------------------
    * This function initializes all cabin environments,
    * sets the module number and prepares storage variables.
    *----------------------------------------------------------------------------------------
    */
   UINT8 i = (UINT8)0; 
   UINT8 q = (UINT8)0;
   UINT8 ok_cnt = (UINT8)0;

   if ((ctrll_port_cab1 == NULL)||
       (data_port1_cab1 == NULL)||
       (data_port2_cab1 == NULL)||
       (ctrlr_port_cab1 == NULL)||
       (ctrll_port_cab2 == NULL)||
       (data_port1_cab2 == NULL)||
       (data_port2_cab2 == NULL)||
       (ctrlr_port_cab2 == NULL)
       )
   {
      return;
   }

   /* check the pointer validity
      (should be initialized in APP frame before the call of gpp_init() takes place)
      
   if (fp_os_io_printf == NULL)
   {
      fp_os_io_printf = (void*) os_io_printf;
   }*/

   if (fp_du_gpp_event == NULL)
   {
      fp_du_gpp_event = (void *)gpp_event_dummy;
   }

   if (fp_du_get_module_number)
   {
      gpp0_module_nr = fp_du_get_module_number((SINT8*)__FILE__);
   }
   else
   {
      /* without module nr. ne du_event()
      */
      fp_du_gpp_event = (void *)gpp_event_dummy;
   }

   /* reset the cabin instances */
   memset(&gpp_data_cabin[0], 0, sizeof(s_gpp_main_data));
   memset(&gpp_data_cabin[1], 0, sizeof(s_gpp_main_data));

   /* Plausibility check of ports
   */
   if (nr_of_local_ports  && (nr_of_local_ports  <= MAX_PORTS_GPP) &&
         nr_of_remote_ports && (nr_of_remote_ports <= MAX_PORTS_GPP))
   {
      /* for GPP internal use */
      nr_of_gpp_local_ports = nr_of_local_ports;
      nr_of_gpp_remote_ports = nr_of_remote_ports;
      if (nr_of_gpp_local_ports > 1)
      {
         /* original: TSG->MMI */
         max_entries_send = (UINT16)MAX_PACKETS_QUEUE_TSG_MMI;
         max_entries_receive = (UINT16)MAX_PACKETS_QUEUE_MMI_TSG;
      }
      else
      {
         /* original: MMI->TSG */
         max_entries_send = (UINT16)MAX_PACKETS_QUEUE_MMI_TSG;
         max_entries_receive = (UINT16)MAX_PACKETS_QUEUE_TSG_MMI;
      }

      if ((gpp_nr_of_cabins == 0) || (gpp_nr_of_cabins > MAX_GPP_NR_OF_CABINS))
      {
         /* no definition in NVS part means max. cabin number */
         gpp_nr_of_cabins = (UINT8)MAX_GPP_NR_OF_CABINS; /* export to other modules */
      }

      if (fp_gpp_diag_entry)
      {
         /* reset DIAG entries */
         fp_gpp_diag_entry(G_GPP_INIT, 0, 0, 718);
         fp_gpp_diag_entry(G_GPP_CONNECTION, 0, 0, 719);
      }

      memset(gpp_seq_trace, 0, ((UINT32)MAX_SEQ_TRACE * sizeof(MAX_SEQ_TRACE)));

      for (i = (UINT8)0u; i < gpp_nr_of_cabins; i++)
      {
         if (i == 0)
         {
            mqa_init_port_adresses_cab1(ctrll_port_cab1, data_port1_cab1, data_port2_cab1, ctrlr_port_cab1);
         }
         else
         {
            mqa_init_port_adresses_cab2(ctrll_port_cab2, data_port1_cab2, data_port2_cab2, ctrlr_port_cab2);
         }
         if (gpp_port_ctrll[i] && gpp_port_data1[i] && gpp_port_data2[i] && gpp_port_ctrlr[i])
         {
            /* all ports set to any address
            */
            if (nr_of_gpp_local_ports > 1)
            {
               /* LOCAL version (3 srce, 1 sink port) */
               gpp_data_cabin[i].port_ctrll = gpp_port_ctrll[i];
               gpp_data_cabin[i].port_data1 = gpp_port_data1[i];
               gpp_data_cabin[i].port_data2 = gpp_port_data2[i];
               gpp_data_cabin[i].port_ctrlr = gpp_port_ctrlr[i];
            }
            else
            {
               /* REMOTE version (1 srce, 3 sink ports) */
               gpp_data_cabin[i].port_ctrll = gpp_port_ctrlr[i];
               gpp_data_cabin[i].port_ctrlr = gpp_port_ctrll[i];
               gpp_data_cabin[i].port_data1 = gpp_port_data1[i];
               gpp_data_cabin[i].port_data2 = gpp_port_data2[i];
            }

            gpp_set_cabin(i + 1); /* set the cabin instance */

            q = gpp_init_cabin_environment(); /* GPP core for cabin[i] */

            if (q)
            {
               ok_cnt++; /* ok */
               gpp_mmi_qm_init(); /* TSG -> MMI queue for cabin[i] */
               gpp_tsg_qm_init(); /* MMI -> TSG queue for cabin[i] */
               gpp_data_cabin[i].gpp_enabled = (UINT8)TRUE;
               gpp_data_cabin[i].mmi_new_data_from_mmi = (UINT8)FALSE;
            }
         }
      }
      if (ok_cnt == gpp_nr_of_cabins)
      {
         /* all channels ok, set cabin 1 after init = default */
         p_cabin = &gpp_data_cabin[0];
         gpp_cabin = (UINT8)1;
         GPP0_EVENT_0("H700: GPP init done");
      }
      else
      {
         /* all or nothing: nothing */
         GPP0_EVENT_2("E701: *** GPP only %u of %u cabins initialized ***", ok_cnt, gpp_nr_of_cabins);

         if (fp_gpp_diag_entry)
         {
            fp_gpp_diag_entry(K_GPP_INIT, gpp_nr_of_cabins, ok_cnt, 721);
         }

         gpp_nr_of_cabins = (UINT8)0;
         gpp_cabin = (UINT8)0;
      }
   }
   else
   {
      GPP0_EVENT_2("E723: init GPP, invalid port number: srce=%u, sink=%u (1..3)", nr_of_local_ports, nr_of_remote_ports);
   }
} /*gpp_init*/


void gpp_conn_state(void)
{
   /*----------------------------------------------------------------------------------------
    * This function controls the connection state with MMI.
    * It is called cyclic by adm_main() after gpp_read_data_packets() in each cycle.
    *----------------------------------------------------------------------------------------
    */
   UINT8 conn_state = (UINT8)GPP_CONN_UNDEFINED;

   if (p_cabin->gpp_enabled)
   {
      switch (p_cabin->gpp_conn_state_local)
      {
      case GPP_CONN_GET_READY:
      {
         if ((p_cabin->gpp_conn_state_remote == GPP_CONN_GET_READY) || (p_cabin->gpp_conn_state_remote == GPP_CONN_READY))
         {
            p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_READY;
            p_cabin->gpp_logging_enabled = (UINT16)2;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION);  /* send conn state */
            GPP0_EVENT_1("H703: GPP_CONN_READY (cab=%u)", gpp_cabin);
         }
         break;
      }

      case GPP_CONN_READY:
      {

         if ((p_cabin->gpp_conn_state_remote == GPP_CONN_READY) || (p_cabin->gpp_conn_state_remote == GPP_CONN_START))
         {
            p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_START;
            p_cabin->gpp_logging_enabled = (UINT16)2;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION); /* send conn state */
            GPP0_EVENT_1("H704: GPP_CONN_START (cab=%u)", gpp_cabin);
         }
         else if (p_cabin->gpp_conn_state_remote != GPP_CONN_GET_READY)
         {
            p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_GET_READY; /* not expected state */
            p_cabin->gpp_logging_enabled = (UINT16)2;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION); /* send conn state */
            GPP0_EVENT_1("H705: GPP_CONN_GET_READY (cab=%u)", gpp_cabin);
         }
         else
         {
            /*do nothing*/
         }
         break;
      }

      case GPP_CONN_START:
      {

         if ((p_cabin->gpp_conn_state_remote == GPP_CONN_START) || (p_cabin->gpp_conn_state_remote == GPP_CONN_RUNNING))
         {
            p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_RUNNING;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION); /* send conn state */
            p_cabin->gpp_work_state = (UINT8)GPP_WAIT_FOR_CMD; /* abort any running transmission */
            GPP0_EVENT_1("H706: GPP connection established for cabin %u", gpp_cabin);
            if (p_cabin->disconnected)
            {
               /* reset DIAG entry */
               if (fp_gpp_diag_entry)
               {
                  fp_gpp_diag_entry(G_GPP_CONNECTION, 0, 0, 719);
               }
               p_cabin->disconnected = (UINT8)FALSE;
            }
         }
         else if (p_cabin->gpp_conn_state_remote != GPP_CONN_READY)
         {
            /* not expected state, try reconnect */
            p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_GET_READY;
            p_cabin->gpp_logging_enabled = (UINT16)2;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION);  /* send conn state */
            GPP0_EVENT_1("H707: GPP_CONN_GET_READY (cab=%u)", gpp_cabin);
         }
         else
         {
            /*do nothing*/
         }
         break;
      }

      case GPP_CONN_RUNNING:
      {
         if ((p_cabin->gpp_conn_state_remote != GPP_CONN_RUNNING) && (p_cabin->gpp_conn_state_remote != GPP_CONN_START))
         {
            p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_GET_READY;  /* not expected state */
            p_cabin->gpp_work_state = (UINT8)GPP_WAIT;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION); /* send conn state */
            GPP0_EVENT_1("H708: GPP disconnected (cab=%u)", gpp_cabin);
            if (fp_gpp_diag_entry)
            {
               fp_gpp_diag_entry(K_GPP_CONNECTION, 0, 0, 722);
            }
            p_cabin->disconnected = (UINT8)TRUE; /* to recognize reset DIAG entry */
         }
         break;
      }

      default:
      {
         /* wait until init is ready */
         /*if is allways TRUE ??? -> comment out for LINT
         if (conn_state == GPP_CONN_UNDEFINED)
         {
         */
         conn_state = p_cabin->gpp_conn_state_local;
         GPP0_EVENT_1("H717: unknown connecion state = %u (0-3)", conn_state);
         /*
         }
         */
         break;
      }
      }
   }
} /* gpp_conn_state */



void gpp_write_data_packets(void)
{
   /*----------------------------------------------------------------------------------------
    * This function contains the GPP state machine to transmit the application data via MVB
    * to the destination device. In each cycle one data subset is sent to the remote device.
    * This will be repeated until the whole application data stream is transmitted.
    *----------------------------------------------------------------------------------------
    */
   if ((p_cabin->gpp_enabled) && (p_cabin->gpp_conn_state_local == GPP_CONN_RUNNING))
   {
      switch (p_cabin->gpp_work_state)
      {
      case GPP_WAIT_FOR_CMD:
      {
         /* wait until data come in to send... */
         if (p_cabin->mmi_new_data_from_mmi)
         {
            /* received from remote with new seqnr. -> send ack. to remote
               (seqnr remote = ack_to_remote)
            */
            p_cabin->mmi_new_data_from_mmi = (UINT8)FALSE;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION);  /* seq/ack as lifesign handshake */
         }
         break;
      }

      case GPP_START_CMD:
      {
         /* prepare all for RUN and start 1st send */
         if (p_cabin->gpp_ack_from_remote == p_cabin->gpp_seq_from_tsg)
         {
            p_cabin->gpp_packet_ix = (UINT8)0;  /* start byte stream 0...n */
            p_cabin->gpp_send_buf_ix = (UINT16)0;
            p_cabin->gpp_last_send_buf_ix = (UINT16)0;
            p_cabin->gpp_size_remain = p_cabin->gpp_send_buf_size;
            p_cabin->gpp_last_size_remain = p_cabin->gpp_send_buf_size;
            p_cabin->mmi_new_data_from_mmi = (UINT8)FALSE;
            gpp_fill_sub_packets(SUB_PACKETS_APPLICATION);
            p_cabin->gpp_work_state = (UINT8)GPP_RUN_CMD;

            gpp_remote_print_time(TRANSMIT); /* start time stamp */

            if (gpp_trace_enabled == 1)
            {
               GPP0_EVENT_4("H709: GPP(%u) start send, l=%u, seq=%u, ack_to_rem=%u",
                            gpp_cabin, p_cabin->gpp_send_buf_size,
                            p_cabin->gpp_seq_from_tsg, p_cabin->gpp_seq_from_remote);
            }
         }
         else if (p_cabin->mmi_new_data_from_mmi)
         {
            /* received from remote with new seqnr. -> send ack. to remote
               (seqnr remote = ack_to_remote)
            */
            p_cabin->mmi_new_data_from_mmi = (UINT8)FALSE;
            gpp_fill_sub_packets(SUB_PACKETS_CONNECTION);  /* seq/ack as lifesign handshake */
         }
         else
         {
            /*do nothing*/
         }
         break;
      }

      case GPP_RUN_CMD:
      {
         /* GPP send n x sub-packets
         */
         if (p_cabin->gpp_ack_from_remote == p_cabin->gpp_seq_from_tsg)
         {
            if (p_cabin->gpp_size_remain)
            {
               /* ack correct, yet data available, next sub packet */
               p_cabin->gpp_packet_ix++;
               p_cabin->mmi_new_data_from_mmi = (UINT8)FALSE;
               gpp_fill_sub_packets(SUB_PACKETS_APPLICATION);
            }
            else
            {
               /* Transmission finished */
               p_cabin->gpp_work_state = (UINT8)GPP_WAIT_FOR_CMD; /* -> default */
               if (gpp_trace_enabled == 1)
               {
                  GPP0_EVENT_4("H710: GPP(%u) end send, l=%u, seq=%u, ack_to_rem=%u",
                               gpp_cabin, p_cabin->gpp_send_buf_size,
                               p_cabin->gpp_seq_from_tsg, p_cabin->gpp_seq_from_remote);
               }
            }
         }
         else if (p_cabin->mmi_new_data_from_mmi)
         {
            /* received from remote with new seqnr. -> send ack. to remote, but use old frame a second time
               (seqnr remote = ack_to_remote)
            */
            p_cabin->mmi_new_data_from_mmi = (UINT8)FALSE;
            gpp_resend_sub_packets();  /*  update ack in last Application frame */
            /* First quick fix, can destroy longer messages: gpp_fill_sub_packets(SUB_PACKETS_CONNECTION); */  /* seq/ack as lifesign handshake */
         }
         else
         {
            /*do nothing*/
         }
         break;
      }

      default:
      {
         /* GPP_WAIT do nothing */
         break;
      }
      } /* switch */
   }
} /*gpp_write_data_packets*/



void gpp_read_data_packets(void)
{
   /*----------------------------------------------------------------------------------------
    * This function reads the incoming data from the port set and stores it into a receive
    * buffer. After receiving the complete application data stream it will be stored into the
    * tsg_packet_queue by calling the function gpp_qm_store_tsg_data.
    *----------------------------------------------------------------------------------------
    */


   static UINT8  gpp_recv_buf[MAX_RECV_BUF_LNG];

   UINT8  packet_index = (UINT8)0;
   UINT8  seq_ctrl, seq_data_1, seq_data_2 = (UINT8)0;
   UINT16 sum_max_bytes_to_copy = (UINT16)0;
   UINT16* p_ctrlr = (UINT16*)NULL;
   UINT16* p_data1 = (UINT16*)NULL;
   UINT16* p_data2 = (UINT16*)NULL;
   UINT8  mmi_data_changed = (UINT8)FALSE;
   UINT16 i = (UINT16)0;
   UINT16 gpp_store_result = (UINT16)GPP_ERR;

   if (p_cabin->gpp_enabled)
   {
      memset(gpp_recv_buf, 0, MAX_RECV_BUF_LNG);

      /*------------------------------------------------

        copy data into work buffer and get control info

        ----------------------------------------------*/

      /* get TM pointers for quick access from table */
      p_ctrlr = mqa_get_sink_port_tm_addr_by_index(p_cabin->ix_ctrlr);

      /* copy all data into work buffer word by word (TM has no byte access)  */
      for (i = (UINT16)0; i < (PORT_SIZE_GPP / 2); i++)
      {
         ((UINT16*) gpp_recv_buf)[i + OFFSET_1_PORT] = p_ctrlr[i];
      }
      sum_max_bytes_to_copy = (UINT16)(PORT_SIZE_GPP - 4); /* for ctrl port */

      seq_ctrl = (UINT8)((gpp_recv_buf[PORT_SIZE_GPP - 1] & 0xE0) >> 5);
      seq_data_1 = seq_ctrl;
      seq_data_2 = seq_ctrl;

      /* control info from remote */
      packet_index                 =  gpp_recv_buf[PORT_SIZE_GPP - 2];
      p_cabin->gpp_conn_state_remote  =  (UINT8)(gpp_recv_buf[PORT_SIZE_GPP - 1] & 0x03);
      p_cabin->gpp_seq_from_remote = (UINT8)((gpp_recv_buf[PORT_SIZE_GPP - 1] & 0xE0) >> 5);
      p_cabin->gpp_ack_from_remote = (UINT8)((gpp_recv_buf[PORT_SIZE_GPP - 1] & 0x1C) >> 2);
      p_cabin->mmi_recv_data_size  = *((UINT16*) &gpp_recv_buf[PORT_SIZE_GPP - 4]);

      if (nr_of_gpp_remote_ports > 1)
      {
         /* more than 1 receive port */
         p_data1 = mqa_get_sink_port_tm_addr_by_index(p_cabin->ix_data1);
         for (i = (UINT16)0; i < (PORT_SIZE_GPP / 2); i++)
         {
            ((UINT16*) gpp_recv_buf)[i + OFFSET_2_PORT] = p_data1[i];
         }
         sum_max_bytes_to_copy = (UINT16)(sum_max_bytes_to_copy + (UINT16)(PORT_SIZE_GPP - 1)); /* for ctrl + data_1 port */
         seq_data_1 = gpp_recv_buf[(PORT_SIZE_GPP + PORT_SIZE_GPP) - 1];
      }

      if (nr_of_gpp_remote_ports > 2)
      {
         /* more than 2 receive ports (max. = 3) */
         p_data2 = mqa_get_sink_port_tm_addr_by_index(p_cabin->ix_data2);
         for (i = (UINT16)0; i < (PORT_SIZE_GPP / 2); i++)
         {
            ((UINT16*) gpp_recv_buf)[i + OFFSET_3_PORT] = p_data2[i];
         }
         sum_max_bytes_to_copy = (UINT16)(sum_max_bytes_to_copy + (UINT16)(PORT_SIZE_GPP - 1)); /* for ctrl + data_1 + data_2 port */
         seq_data_2 = gpp_recv_buf[(PORT_SIZE_GPP + PORT_SIZE_GPP + PORT_SIZE_GPP) - 1];
      }

      /* statistics */
      if ((atp_active_cabin == gpp_cabin) || (atp_active_cabin == 0) || (gpp_nr_of_cabins == 1))
      {
         os_c_get(&gpp_seq_trace[gpp_seq_trace_curr_entry].recv_time);
         gpp_seq_trace[gpp_seq_trace_curr_entry].seq_from_remote = p_cabin->gpp_seq_from_remote;
         gpp_seq_trace[gpp_seq_trace_curr_entry].ack_from_remote = p_cabin->gpp_ack_from_remote;
      }

      /*------------------------------------------------

        check plausibility

        ----------------------------------------------*/
      if ((seq_ctrl == seq_data_1) && (seq_ctrl == seq_data_2))
      {
         /* incoming ports are consistent, check change */
         if (p_cabin->gpp_seq_from_remote != p_cabin->gpp_seq_from_remote_old)
         {
            /* new data from remote have arrieved if the seq. nr. has changed and all
               3 ports are consistent, recognized by the same seq. of them
            */
            p_cabin->mmi_new_data_from_mmi = (UINT8)TRUE; /* change, used in gpp_write_data_packets() */
            p_cabin->gpp_seq_from_remote_old = p_cabin->gpp_seq_from_remote;

            if (p_cabin->mmi_recv_data_size > 0)
            {
               if (p_cabin->mmi_recv_data_size <= MAX_GPP_PACKET_LENGTH)
               {
                  mmi_data_changed = (UINT8)TRUE; /* valid data, copy necessary */
               }
               else
               {
                  GPP0_EVENT_3("E705: cabin %u, received data length > %u, l=%u, invalid data",
                               gpp_cabin, MAX_GPP_PACKET_LENGTH, p_cabin->mmi_recv_data_size);
               }
            }
         }
      }
      else
      {
         /* inkonsist data due to asynchronious access at the MVBC. (next cycle may be consistent) */
         if (gpp_trace_enabled == 1)
         {
            GPP0_EVENT_4("H711: GPP(%u): seqnr. inconsist., ports: ctrl=%u, data1=%u, data2=%u",
                         gpp_cabin, seq_ctrl, seq_data_1, seq_data_2);
         }
      }

      /*------------------------------------------------

        collect data from work buffer into the receive
        buffer (n sub packets).

        ----------------------------------------------*/
      if (mmi_data_changed)
      {
         if (p_cabin->gpp_packet_index_expected != packet_index)
         {
            if (p_cabin->unexpected_cnt > 20)
            {
               /* corrupt data received, lack of packet index
               */
               GPP0_EVENT_4("E706: (cab=%u) wrong packet index, pi=%u, exp.=%u, ix=%u, invalid data",
                            gpp_cabin, packet_index, p_cabin->gpp_packet_index_expected, p_cabin->mmi_recv_data_ix);
               p_cabin->unexpected_cnt = (UINT8)0;
               p_cabin->gpp_packet_index_expected = (UINT8)0;
               p_cabin->mmi_recv_data_ix = (UINT16)0;
            }
            else
            {
               p_cabin->unexpected_cnt++;
            }
         }
         else
         {
            /* copy the whole received data. This may be a set of n sub-packets. The length of a
               sub-packet depends on the configured amount of receive ports. Possible values are
               sum_max_bytes_to_copy: 28 for 1 port, 59 for 2 ports, 90 for 3 ports

               1. sub packet <= sum_max_bytes_to_copy     one packet: end recognized by while(...)
               2. sub packet >  sum_max_bytes_to_copy     n packets received by sub-ndizes,
                                                          end of last packet recognized by while(..)
            */
            i = (UINT16)0; /* current recv buffer index */
            p_cabin->unexpected_cnt = (UINT8)0;

            while (p_cabin->mmi_recv_data_ix < p_cabin->mmi_recv_data_size)
            {
               /* check if more than one recv port 28, 28+31, 28+31+31 */
               if ((i < 28) || ((i > 31) && (i < 63)) || ((i > 63) && (i < 95)))
               {
                  p_cabin->mmi_recv_data[p_cabin->mmi_recv_data_ix] = gpp_recv_buf[i];
                  p_cabin->mmi_recv_data_ix++;
               }
               else if ((p_cabin->mmi_recv_data_size >= sum_max_bytes_to_copy) && (i >= sum_max_bytes_to_copy))
               {
                  /* end of one sub-packet of n sub-packets */
                  p_cabin->gpp_packet_index_expected++; /* next sub packet */

                  break; /* sub packet finished */
               }
               else
               {
                  /*do nothing*/
               }
               i++; /* next */
            }

            if (p_cabin->mmi_recv_data_size == p_cabin->mmi_recv_data_ix)
            {
               /* D a t a  c o m p l e t e
                  store MMI data into queue for NVS work through
                  (store each telegram. Used due to check life sign of MMI.
                   life sign is the change of seq/ack)
               */
               gpp_remote_print_time(RECEIVE); /* end time stamp */

               gpp_store_result = gpp_qm_store_tsg_data(p_cabin->mmi_recv_data, p_cabin->mmi_recv_data_size);
               /*not longer ignoring return value for LINT*/
               if(gpp_store_result != GPP_OK)
               {
                  GPP0_EVENT_1("E888: gpp_qm_store_tsg_data returning error %d",
                      gpp_store_result);
               }

               p_cabin->gpp_packet_index_expected = (UINT8)0;
               p_cabin->mmi_recv_data_ix = (UINT16)0;

               /* LOGGING if enabled */
               if (p_cabin->gpp_logging_enabled)
               {
                  p_cabin->gpp_logging_enabled--;

                  /* record logging in connection mode only or if changed content */
                  if (fp_du_port_data)
                  {
                     fp_du_port_data(p_cabin->port_ctrlr, p_ctrlr);

                     if (nr_of_gpp_remote_ports > 1)
                     {
                        fp_du_port_data(p_cabin->port_data1, p_data1);
                     }
                     if (nr_of_gpp_remote_ports > 2)
                     {
                        fp_du_port_data(p_cabin->port_data2, p_data2);
                     }
                  }
               }
            } /* if data changed */
         }
      }
   } /* if gpp enabled */
} /* gpp_read_data_packets */



/*******************************************************************************
 * Private Functions
 ******************************************************************************/
void gpp_remote_print_time(UINT16 dir)
{
   /*-------------------------------------------------------------
    * Displays a time stamp as TRACE support
    *-------------------------------------------------------------
   */
   OS_STR_TIME_POSIX time;
   UINT16 msec;

   if (gpp_trace_enabled == 2)
   {
      os_c_get(&time);
      msec = (UINT16)((UINT16)(time.sec * 1000u) + (UINT16)((time.nanosec / 1000u) / 1000u));

      /* ! ! ! don't change the format string ! ! !
      */
      if (dir == TRANSMIT)
      {
         /*lint -save -e534*/ /*it is not important if print was successful and we can ignore return value*/
         OS_IO_PRINTF((SINT8*)" \nts tx %d", msec);
      }
      if (dir == RECEIVE)
      {
         OS_IO_PRINTF((SINT8*)" \nts rx %d", msec);
         /*lint -restore*/
      }
   }
}



UINT8 gpp_init_cabin_environment(void)
{
   /*--------------------------------------------------------------------------------------
    * This function performs the mvb database internal (NVS specific) initialization:
    *  - set TS addresses,
    *  - reset tables
    *  - build tables for quick TM access
    *  - start msg
    *--------------------------------------------------------------------------------------
    */
   UINT16 sink_port_addr_cnt;
   UINT16 srce_port_addr_cnt;
   INT32 j;
   UINT8 q;

   p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_IDLE;
   p_cabin->gpp_conn_state_remote = (UINT8)GPP_CONN_IDLE;
   p_cabin->gpp_work_state = (UINT8)GPP_WAIT;
   p_cabin->gpp_packet_index_expected = (UINT8)0;
   p_cabin->unexpected_cnt = (UINT8)0;
   p_cabin->mmi_recv_data_size = (UINT16)0;
   p_cabin->mmi_recv_data_ix = (UINT16)0;
   p_cabin->gpp_mmi_invalid_counter = (UINT16)0;
   p_cabin->gpp_size_remain = (UINT16)0;

   p_cabin->gpp_seq_from_tsg = (UINT8)0; /* endless rotation 0..7 */
   p_cabin->gpp_seq_from_remote = (UINT8)0;
   p_cabin->gpp_ack_from_remote = (UINT8)0;

   p_cabin->gpp_seq_from_remote_old = (UINT8)0;

   /* send and recv buffer reset */
   for (j = 0; j < MAX_GPP_PACKET_LENGTH; j++)
   {
      p_cabin->mmi_recv_data[j] = (UINT8)0;
      p_cabin->gpp_send_buf[j] = (UINT8)0;
   }

   /* Prepare the quick MVB TM access
      (get the access index and set port names) */
   sink_port_addr_cnt = (UINT16)0;
   srce_port_addr_cnt = (UINT16)0;

   if (nr_of_gpp_local_ports > 1)
   {
      /* TSG version (1 sink, 3 srce ports) */
      p_cabin->ix_ctrlr = mqa_get_port_index(p_cabin->port_ctrlr);

      p_cabin->ix_ctrll = mqa_get_port_index(p_cabin->port_ctrll);
      p_cabin->ix_data1 = mqa_get_port_index(p_cabin->port_data1);
      p_cabin->ix_data2 = mqa_get_port_index(p_cabin->port_data2);

      if (p_cabin->ix_ctrlr != IX_UNDEF)
      {
         sink_port_addr_cnt++;
      }

      if (p_cabin->ix_ctrll != IX_UNDEF)
      {
         srce_port_addr_cnt++;
      }

      if (p_cabin->ix_data1 != IX_UNDEF)
      {
         srce_port_addr_cnt++;
      }

      if (p_cabin->ix_data2 != IX_UNDEF)
      {
         srce_port_addr_cnt++;
      }
   }
   else
   {
      /* REMOTE version (1 srce, 3 sink ports) */
      p_cabin->ix_ctrll = mqa_get_port_index(p_cabin->port_ctrll);

      p_cabin->ix_ctrlr = mqa_get_port_index(p_cabin->port_ctrlr);
      p_cabin->ix_data1 = mqa_get_port_index(p_cabin->port_data1);
      p_cabin->ix_data2 = mqa_get_port_index(p_cabin->port_data2);

      if (p_cabin->ix_ctrlr != IX_UNDEF)
      {
         srce_port_addr_cnt++;
      }

      if (p_cabin->ix_ctrll != IX_UNDEF)
      {
         sink_port_addr_cnt++;
      }

      if (p_cabin->ix_data1 != IX_UNDEF)
      {
         sink_port_addr_cnt++;
      }

      if (p_cabin->ix_data2 != IX_UNDEF)
      {
         sink_port_addr_cnt++;
      }
   }

   if ((srce_port_addr_cnt == nr_of_gpp_local_ports) && (sink_port_addr_cnt == nr_of_gpp_remote_ports))
   {
      q = (UINT8)TRUE; /* ok */

      p_cabin->gpp_conn_state_local = (UINT8)GPP_CONN_GET_READY; /* ready to establish GPP connection */
      gpp_fill_sub_packets(SUB_PACKETS_CONNECTION); /* fill all with zero */
   }
   else
   {
      q = (UINT8)FALSE; /* error */

      if (srce_port_addr_cnt != nr_of_gpp_local_ports)
      {
         GPP0_EVENT_3("E703: init GPP cabin %u, srce=%u(%u), check MTPE/MVB config",
                      gpp_cabin, srce_port_addr_cnt, nr_of_gpp_local_ports);
      }
      if (sink_port_addr_cnt != nr_of_gpp_remote_ports)
      {
         GPP0_EVENT_3("E704: init GPP cabin %u, sink=%u(%u), check MTPE/MVB config",
                      gpp_cabin, sink_port_addr_cnt, nr_of_gpp_remote_ports);
      }
   }
   return q;
} /* gpp_init_cabin_environment */



void gpp_fill_app_packets(UINT16* p_ctrl,
                          UINT16* p_data_1,
                          UINT16* p_data_2)
{
   /*------------------------------------------------------------------------------------
    * The sub packets will be filled withe the current stream of the
    * application data packet.
    *
    * IN          *p_ctrl          ptr. onto area of ctrl port
    *             *p_data_1        ptr. onto area of data port 1
    *             *p_data_2        ptr. onto area of data port 2
    *------------------------------------------------------------------------------------
    */


   UINT16 i = (UINT16)0;
   UINT8 port_buf1[32]; /* copy buffer, due to only word access at TM */
   UINT8 port_buf2[32];
   UINT8 port_buf3[32];

   /* fill the CTRL port */
   ((UINT16*)port_buf1)[14] = p_cabin->gpp_send_buf_size;
   port_buf1[30] = p_cabin->gpp_packet_ix;

   /* increment sequence counter. That means data to send */
   p_cabin->gpp_seq_from_tsg++;
   if (p_cabin->gpp_seq_from_tsg > 7)
   {
      p_cabin->gpp_seq_from_tsg = (UINT8)0;
   }

   port_buf1[31] = (UINT8)((UINT8)(p_cabin->gpp_seq_from_tsg << 5) |
                           (UINT8)(p_cabin->gpp_seq_from_remote << 2) | p_cabin->gpp_conn_state_local);

   /* statistics */
   if ((atp_active_cabin == gpp_cabin) || (atp_active_cabin == 0) || (gpp_nr_of_cabins == 1))
   {
      gpp_seq_trace[gpp_seq_trace_curr_entry].seq_to_remote = p_cabin->gpp_seq_from_tsg;
      gpp_seq_trace[gpp_seq_trace_curr_entry].ack_to_remote = p_cabin->gpp_seq_from_remote;
      gpp_seq_trace_curr_entry++;
      if (gpp_seq_trace_curr_entry >= MAX_SEQ_TRACE)
      {
         gpp_seq_trace_curr_entry = (UINT16)0;
      }
   }

   /* store actual start values for resending in gpp_resend_app_packets() */
   p_cabin->gpp_last_send_buf_ix = p_cabin->gpp_send_buf_ix;
   p_cabin->gpp_last_size_remain = p_cabin->gpp_size_remain;

   for (i = (UINT16)0; i < (UINT16)28; i++)
   {
      if (p_cabin->gpp_size_remain)
      {
         port_buf1[i] = p_cabin->gpp_send_buf[p_cabin->gpp_send_buf_ix];
         p_cabin->gpp_send_buf_ix++;
         p_cabin->gpp_size_remain--;
      }
      else
      {
         port_buf1[i] = (UINT8)0;
      }
   }

   if (p_data_1)
   {
      /* fill the DATA_1 port */
      port_buf2[31] = p_cabin->gpp_seq_from_tsg; /* only seq. nr. no further ! */

      for (i = (UINT16)0; i < (UINT16)31; i++)
      {
         if (p_cabin->gpp_size_remain)
         {
            port_buf2[i] = p_cabin->gpp_send_buf[p_cabin->gpp_send_buf_ix];
            p_cabin->gpp_send_buf_ix++;
            p_cabin->gpp_size_remain--;
         }
         else
         {
            port_buf2[i] = (UINT8)0;
         }
      }
   }

   if (p_data_2)
   {
      /* fill the DATA_2 port */
      port_buf3[31] = p_cabin->gpp_seq_from_tsg; /* only seq. nr. no further ! */

      for (i = (UINT16)0; i < (UINT16)31; i++)
      {
         if (p_cabin->gpp_size_remain)
         {
            port_buf3[i] = p_cabin->gpp_send_buf[p_cabin->gpp_send_buf_ix];
            p_cabin->gpp_send_buf_ix++;
            p_cabin->gpp_size_remain--;
         }
         else
         {
            port_buf3[i] = (UINT8)0;
         }
      }
   }

   /* copy all to the valid TM port area by word access!
      (afterwards MVBC does the real stuff)
   */
   for (i = (UINT16)0; i < (UINT16)16; i++)
   {
      p_ctrl[i] = ((UINT16*) port_buf1)[i];
   }
   PORT_LOGGING(p_cabin->port_ctrll, port_buf1);

   if (p_data_1)
   {
      for (i = (UINT16)0; i < (UINT16)16; i++)
      {
         p_data_1[i] = ((UINT16*) port_buf2)[i];
      }
      PORT_LOGGING(p_cabin->port_data1, port_buf2);
   }

   if (p_data_2)
   {
      for (i = (UINT16)0; i < (UINT16)16; i++)
      {
         p_data_2[i] = ((UINT16*) port_buf3)[i];
      }
      PORT_LOGGING(p_cabin->port_data2, port_buf3);
   }

   if (p_cabin->gpp_logging_enabled)
   {
      p_cabin->gpp_logging_enabled--;
   }
} /* gpp_fill_app_packets */



void gpp_resend_app_packets(UINT16* p_ctrl,
                            UINT16* p_data_1,
                            UINT16* p_data_2)
{
   /*------------------------------------------------------------------------------------
    * The sub packets will be filled withe the actuial stream of the
    * application data packet a second time.
    *
    * IN          *p_ctrl          ptr. onto area of ctrl port
    *             *p_data_1        ptr. onto area of data port 1
    *             *p_data_2        ptr. onto area of data port 2
    *------------------------------------------------------------------------------------
    */


   UINT16 i = (UINT16)0;
   UINT8 port_buf1[32]; /* copy buffer, due to only word access at TM */
   UINT8 port_buf2[32];
   UINT8 port_buf3[32];

   /* fill the CTRL port */
   ((UINT16*)port_buf1)[14] = p_cabin->gpp_send_buf_size;
   port_buf1[30] = p_cabin->gpp_packet_ix;

   /* do not increment sequence counter! Only update ack! */
   port_buf1[31] = (UINT8)((UINT8)(p_cabin->gpp_seq_from_tsg << 5) |
                           (UINT8)(p_cabin->gpp_seq_from_remote << 2) | p_cabin->gpp_conn_state_local);

   /* statistics */
   if ((atp_active_cabin == gpp_cabin) || (atp_active_cabin == 0) || (gpp_nr_of_cabins == 1))
   {
      gpp_seq_trace[gpp_seq_trace_curr_entry].seq_to_remote = p_cabin->gpp_seq_from_tsg;
      gpp_seq_trace[gpp_seq_trace_curr_entry].ack_to_remote = p_cabin->gpp_seq_from_remote;
      gpp_seq_trace_curr_entry++;
      if (gpp_seq_trace_curr_entry >= MAX_SEQ_TRACE)
      {
         gpp_seq_trace_curr_entry = (UINT16)0;
      }
   }

   /* restore start values for resending */
   p_cabin->gpp_send_buf_ix = p_cabin->gpp_last_send_buf_ix;
   p_cabin->gpp_size_remain = p_cabin->gpp_last_size_remain;

   for (i = (UINT16)0; i < (UINT16)28; i++)
   {
      if (p_cabin->gpp_size_remain)
      {
         port_buf1[i] = p_cabin->gpp_send_buf[p_cabin->gpp_send_buf_ix];
         p_cabin->gpp_send_buf_ix++;
         p_cabin->gpp_size_remain--;
      }
      else
      {
         port_buf1[i] = (UINT8)0;
      }
   }

   if (p_data_1)
   {
      /* fill the DATA_1 port */
      port_buf2[31] = p_cabin->gpp_seq_from_tsg; /* only seq. nr. no further ! */

      for (i = (UINT16)0; i < (UINT16)31; i++)
      {
         if (p_cabin->gpp_size_remain)
         {
            port_buf2[i] = p_cabin->gpp_send_buf[p_cabin->gpp_send_buf_ix];
            p_cabin->gpp_send_buf_ix++;
            p_cabin->gpp_size_remain--;
         }
         else
         {
            port_buf2[i] = (UINT8)0;
         }
      }
   }

   if (p_data_2)
   {
      /* fill the DATA_2 port */
      port_buf3[31] = p_cabin->gpp_seq_from_tsg; /* only seq. nr. no further ! */

      for (i = (UINT16)0; i < (UINT16)31; i++)
      {
         if (p_cabin->gpp_size_remain)
         {
            port_buf3[i] = p_cabin->gpp_send_buf[p_cabin->gpp_send_buf_ix];
            p_cabin->gpp_send_buf_ix++;
            p_cabin->gpp_size_remain--;
         }
         else
         {
            port_buf3[i] = (UINT8)0;
         }
      }
   }

   /* copy all to the valid TM port area by word access!
      (afterwards MVBC does the real stuff)
   */
   for (i = (UINT16)0; i < (UINT16)16; i++)
   {
      p_ctrl[i] = ((UINT16*) port_buf1)[i];
   }
   PORT_LOGGING(p_cabin->port_ctrll, port_buf1);

   if (p_data_1)
   {
      for (i = (UINT16)0; i < (UINT16)16; i++)
      {
         p_data_1[i] = ((UINT16*) port_buf2)[i];
      }
      PORT_LOGGING(p_cabin->port_data1, port_buf2);
   }

   if (p_data_2)
   {
      for (i = (UINT16)0; i < (UINT16)16; i++)
      {
         p_data_2[i] = ((UINT16*) port_buf3)[i];
      }
      PORT_LOGGING(p_cabin->port_data2, port_buf3);
   }

   if (p_cabin->gpp_logging_enabled)
   {
      p_cabin->gpp_logging_enabled--;
   }
} /* gpp_resend_app_packets */



void gpp_fill_conn_packets(UINT16* p_ctrl,
                           UINT16* p_data_1,
                           UINT16* p_data_2)
{
   /*------------------------------------------------------------------------------------
    * The sub packets will be filled with zero and the connection state
    * within the control port. (there will be no seqnr incrementation !)
    *
    * IN          *p_ctrl          ptr. onto area of ctrl port
    *             *p_data_1        ptr. onto area of data port 1
    *             *p_data_2        ptr. onto area of data port 2
    *------------------------------------------------------------------------------------
    */
   UINT16 i = (UINT16)0;
   UINT8 port_buf[32]; /* copy buffer, due to only word access at TM */

   /* fill the CTRL port */
   memset(port_buf, 0x11, 32);
   ((UINT16*)port_buf)[14] = (UINT16)0;
   port_buf[30] = p_cabin->gpp_packet_ix;

   /* No increment of sequence counter. That means no data to send */
   port_buf[31] = (UINT8)((UINT8)(p_cabin->gpp_seq_from_tsg << 5) |
                          (UINT8)(p_cabin->gpp_seq_from_remote << 2) | p_cabin->gpp_conn_state_local);

   /* statistics */
   if ((atp_active_cabin == gpp_cabin) || (atp_active_cabin == 0) || (gpp_nr_of_cabins == 1))
   {
      gpp_seq_trace[gpp_seq_trace_curr_entry].seq_to_remote = p_cabin->gpp_seq_from_tsg;
      gpp_seq_trace[gpp_seq_trace_curr_entry].ack_to_remote = p_cabin->gpp_seq_from_remote;
      gpp_seq_trace_curr_entry++;
      if (gpp_seq_trace_curr_entry >= MAX_SEQ_TRACE)
      {
         gpp_seq_trace_curr_entry = (UINT16)0;
      }
   }

   /* copy all to the valid TM port area by word access!
      (afterwards MVBC does the real stuff)
   */
   for (i = (UINT16)0; i < (UINT16)16; i++)
   {
      p_ctrl[i] = ((UINT16*) port_buf)[i];
   }
   PORT_LOGGING(p_cabin->port_ctrll, port_buf);

   if (p_data_1)
   {
      /* sequence in data1 and data2 */
      memset(port_buf, 0x22, 32);
      port_buf[31] = p_cabin->gpp_seq_from_tsg; /* only seq. nr. no further ! */
      for (i = (UINT16)0; i < (UINT16)16; i++)
      {
         p_data_1[i] = ((UINT16*) port_buf)[i];
      }
   }

   if (p_data_2)
   {
      memset(port_buf, 0x33, 32);
      port_buf[31] = p_cabin->gpp_seq_from_tsg; /* only seq. nr. no further ! */
      for (i = (UINT16)0; i < (UINT16)16; i++)
      {
         p_data_2[i] = ((UINT16*) port_buf)[i];
      }
   }
} /* gpp_fill_conn_packets */



void gpp_fill_sub_packets(UINT8 type)
{
   /*------------------------------------------------------------------------------------
    * The sub packets will be filled withe the current stream of the
    * application data packet.
    *
    * IN          type             1 = with appl. data, 2 = with connection data
    *------------------------------------------------------------------------------------
    */
   UINT16* p_ctrll = 0;
   UINT16* p_data1 = 0;
   UINT16* p_data2 = 0;

   /* get the current TM pointer for the port triple */
   p_ctrll = mqa_get_srce_port_tm_addr_by_index(p_cabin->ix_ctrll);

   if (nr_of_gpp_local_ports > 1)
   {
      p_data1 = mqa_get_srce_port_tm_addr_by_index(p_cabin->ix_data1);
   }

   if (nr_of_gpp_local_ports > 2)
   {
      p_data2 = mqa_get_srce_port_tm_addr_by_index(p_cabin->ix_data2);
   }

   if (type == SUB_PACKETS_APPLICATION)
   {
      /* a p p l i c a t i o n  data */
      gpp_fill_app_packets(p_ctrll, p_data1, p_data2);
   }
   else
   {
      /* c o n n e c t i o n  data */
      gpp_fill_conn_packets(p_ctrll, p_data1, p_data2);
   }

} /* gpp_fill_sub_packets */



void gpp_resend_sub_packets(void)
{
   /*------------------------------------------------------------------------------------
    * The sub packets will be filled withe the actual stream of the
    * application data packet a second time, but with new ack value.
    *------------------------------------------------------------------------------------
    */
   UINT16* p_ctrll = 0;
   UINT16* p_data1 = 0;
   UINT16* p_data2 = 0;

   /* get the current TM pointer for the port triple */
   p_ctrll = mqa_get_srce_port_tm_addr_by_index(p_cabin->ix_ctrll);
   if (nr_of_gpp_local_ports > 1)
   {
      p_data1 = mqa_get_srce_port_tm_addr_by_index(p_cabin->ix_data1);
   }

   if (nr_of_gpp_local_ports > 2)
   {
      p_data2 = mqa_get_srce_port_tm_addr_by_index(p_cabin->ix_data2);
   }

   /* resend a p p l i c a t i o n  data */
   gpp_resend_app_packets(p_ctrll, p_data1, p_data2);

} /* gpp_resend_sub_packets */

/* eof */
