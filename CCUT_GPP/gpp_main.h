/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_main.h
 *
 *  \author     W. Landwehr
 *
 *  \date       03.11.2011
 *
 *  \brief      Export constants, types, prototypes and externals for GPP Services.
 *
 *****************************************************************************
 */

/*-------------------------------------------------------------------------------------------------
 * gpp_main.h
 *  Export constants, types, prototypes and externals for GPP Services.
 *
 * Copyright (c) 2011 by Bombardier Transportation (Signal) Germany GmbH
 *
 * HISTORY (creation: 24.06.2009, based on last 4xxx version)
 *  03.11.2011 W. Landwehr  fra_005#772  Minor GPP adaptations (TSGT print-out channal)
 *  10.10.2011 W. Landwehr  fra_005#754  Minor GPP adaptations (print-outs etc.)
 *  06.10.2011 W. Landwehr  fra_005#747  GPP upgrade regarding ATP activated cabin
 *  10.05.2011 W. Landwehr  fra_005#xxx: adaptation for 5000, moved function
 *-------------------------------------------------------------------------------------------------
 */
#ifndef GPP_MAIN_H
#define GPP_MAIN_H

#include "mwt_types.h"
#include "usertypes.h"

#include "gpp_api.h"
/******************************************************************************
 * Defines MACROS
 *****************************************************************************/

/* event macros for TSG event logging */

/* event macros for TSG event logging */
#define GPP0_EVENT_0(TEXT)                 {;}
#define GPP0_EVENT_1(TEXT, P1)             {;}
#define GPP0_EVENT_2(TEXT, P1, P2)         {;}
#define GPP0_EVENT_3(TEXT, P1, P2, P3)     {;}
#define GPP0_EVENT_4(TEXT, P1, P2, P3, P4) {;}
#define GPP1_EVENT_0(TEXT)                 {;}
#define GPP1_EVENT_1(TEXT, P1)             {;}
#define GPP1_EVENT_2(TEXT, P1, P2)         {;}
#define GPP1_EVENT_3(TEXT, P1, P2, P3)     {;}
#define GPP1_EVENT_4(TEXT, P1, P2, P3, P4) {;}
#define GPP2_EVENT_0(TEXT)                 {;}
#define GPP2_EVENT_1(TEXT, P1)             {;}
#define GPP2_EVENT_2(TEXT, P1, P2)         {;}
#define GPP2_EVENT_3(TEXT, P1, P2, P3)     {;}
#define GPP2_EVENT_4(TEXT, P1, P2, P3, P4) {;}

/* event macros for TSG event logging */
/*
#define GPP0_EVENT_0(TEXT)                 {fp_du_gpp_event(gpp0_module_nr, __LINE__, TEXT,  0,  0,  0,  0);}
#define GPP0_EVENT_1(TEXT, P1)             {fp_du_gpp_event(gpp0_module_nr, __LINE__, TEXT, P1,  0,  0,  0);}
#define GPP0_EVENT_2(TEXT, P1, P2)         {fp_du_gpp_event(gpp0_module_nr, __LINE__, TEXT, P1, P2,  0,  0);}
#define GPP0_EVENT_3(TEXT, P1, P2, P3)     {fp_du_gpp_event(gpp0_module_nr, __LINE__, TEXT, P1, P2, P3,  0);}
#define GPP0_EVENT_4(TEXT, P1, P2, P3, P4) {fp_du_gpp_event(gpp0_module_nr, __LINE__, TEXT, P1, P2, P3, P4);}
#define GPP1_EVENT_0(TEXT)                 {fp_du_gpp_event(gpp1_module_nr, __LINE__, TEXT,  0,  0,  0,  0);}
#define GPP1_EVENT_1(TEXT, P1)             {fp_du_gpp_event(gpp1_module_nr, __LINE__, TEXT, P1,  0,  0,  0);}
#define GPP1_EVENT_2(TEXT, P1, P2)         {fp_du_gpp_event(gpp1_module_nr, __LINE__, TEXT, P1, P2,  0,  0);}
#define GPP1_EVENT_3(TEXT, P1, P2, P3)     {fp_du_gpp_event(gpp1_module_nr, __LINE__, TEXT, P1, P2, P3,  0);}
#define GPP1_EVENT_4(TEXT, P1, P2, P3, P4) {fp_du_gpp_event(gpp1_module_nr, __LINE__, TEXT, P1, P2, P3, P4);}
#define GPP2_EVENT_0(TEXT)                 {fp_du_gpp_event(gpp2_module_nr, __LINE__, TEXT,  0,  0,  0,  0);}
#define GPP2_EVENT_1(TEXT, P1)             {fp_du_gpp_event(gpp2_module_nr, __LINE__, TEXT, P1,  0,  0,  0);}
#define GPP2_EVENT_2(TEXT, P1, P2)         {fp_du_gpp_event(gpp2_module_nr, __LINE__, TEXT, P1, P2,  0,  0);}
#define GPP2_EVENT_3(TEXT, P1, P2, P3)     {fp_du_gpp_event(gpp2_module_nr, __LINE__, TEXT, P1, P2, P3,  0);}
#define GPP2_EVENT_4(TEXT, P1, P2, P3, P4) {fp_du_gpp_event(gpp2_module_nr, __LINE__, TEXT, P1, P2, P3, P4);}
*/
#define PRINT_GPP_NR_AND_CABIN { \
  os_io_printf("\nGPP configured CABINs=%u, active cabin: GPP=%u, ATP=%u (by EVC2)", \
               gpp_nr_of_cabins, gpp_cabin, atp_active_cabin); \
}



/******************************************************************************
 * API Defines
 *****************************************************************************/

/* IDs for diagnosis queue entries via cdp_diag_queue_entry_gpp(..)
*/
#define K_GPP_INIT           0x07
#define G_GPP_INIT           0x77
#define K_GPP_CONNECTION     0x08
#define G_GPP_CONNECTION     0x80
#define O_GPP_QUEUE_TSG_MMI  0x99

#define IN_QUEUE             'q'
#define TRANSMITTED          't'
#define SENDING              's'
#define RETRIEVED            'r'

/******************************************************************************
 * Defines
 *****************************************************************************/

/* substitute module numbers for stand alone GPP
*/
#define MN_GPP_MAIN             0
#define MN_GPP_TSG2MMI          1
#define MN_GPP_MMI2TSG          2

/* Connection initialization states */
#define GPP_CONN_IDLE           9
#define GPP_CONN_GET_READY      0
#define GPP_CONN_READY          1
#define GPP_CONN_START          2
#define GPP_CONN_RUNNING        3
#define GPP_CONN_UNDEFINED      77

/* GPP transmission states */
#define GPP_WAIT                77
#define GPP_WAIT_FOR_CMD        11
#define GPP_START_CMD           22
#define GPP_RUN_CMD             33

/* delivered returns by gpp_write() and gpp_state()*/
#define GPP_OK                   0
#define GPP_ERR                  (GPP_OK + 99)  /* invalid parameter */
#define GPP_RUNNING              (GPP_OK + 1)
#define GPP_READY_TO_SEND        (GPP_OK + 2)
#define GPP_NO_CONNECTION        (GPP_OK + 3)

#define GPP_MAX_BUF_LENGTH       256       /* data packet, limited by Profibus telegram length */

/* maximum packet length of a gpp data set */
#define MAX_GPP_PACKET_LENGTH    1200      /* old 242, 600 */
/* REMARK
    longest packet is TSG-2 (Track Conditions and NF.
    mmi_n_trackc = 65 ->  l = 32 + 32 + 65(32 + 8 +32) = 4744 => 4744/8 = 593 byte
    */

#define MAX_PACKETS_IN_QUEUE         90                      /* length to define the GPP queue */
#define WARNING_LIMIT_QUEUE_TSG_MMI  30                      /* warning queue level for queue TSG-REMOTE */
#define MAX_PACKETS_QUEUE_TSG_MMI    MAX_PACKETS_IN_QUEUE    /* length of the queue TSG->REMOTE */
#define MAX_PACKETS_QUEUE_MMI_TSG    5                       /* length of the queue REMOTE->TSG */

#define STATISTIC_CYCLE_COUNTER     600    /* one minute */



/******************************************************************************
 * Typedefs
 *****************************************************************************/

/* structure to store sequence tracing */
typedef struct
{
   UINT8 seq_from_remote;   /* MMI -> TSG (from read fct.) */
   UINT8 ack_to_remote;     /* TSG -> MMI (from send fct.) */
   UINT8 seq_to_remote;     /* TSG -> MMI (from send fct.) */
   UINT8 ack_from_remote;   /* MMI -> TSG (from read fct.) */
   OS_STR_TIME_POSIX recv_time;     /* time stamp at receiving GPP data */
} s_gpp_seq_trace;

#define MAX_SEQ_TRACE  30


/* packet record for queuing TSG-App -> MMI and vice versa */
typedef struct
{
   UINT16 transmit_state;              /* IN_QUEUE, TRANSMITED, SENDING */
   UINT16 length;                      /* used number of bytes within the data field */
   UINT8  nid_stm;                     /* FFFFIS header: 254 from EVC, 253 from tsg; 10 from STM */
   UINT8  data[MAX_GPP_PACKET_LENGTH]; /* data field */
} s_packet_element;


/* packet queuing TSG-MMI and vice versa
   Buffer will be used as ring buffer.
   storing:    the index store_element points of the next element that will be
               used for storing an incoming packet. This element schould be empty
               recognized by packet_element.length = 0. Otherwise an error is
               reported.
   read/write: the index send_element points to the current handled element. If
               the packet_element.length = 0, this element is completely
               transmitted and the next element can be worked off.
               If the next send_element = store_element, the queue is empty.
*/
typedef struct
{
   UINT16 entries;
   UINT16 cycle_cnt;   /* for statistic average */
   UINT16 num_stored;  /* for statistic average */
   UINT16 num_lost;    /* for statistic */
   UINT16 max_stored;  /* for statistic maximum in 600 cycles */
   UINT16 diag_err;    /* flag to check whether a DIAG msg was sent */
   UINT16 i_store;     /* points to the element for next entry */
   UINT16 i_read;      /* points to the current handled element read/write */
   s_packet_element element[MAX_PACKETS_IN_QUEUE]; /* data content to send */
} s_packet_queue;


/* Structure containing all GPP data */
typedef struct
{
   /* data for quick access to TM
   */
   UINT16 ix_ctrll, ix_data1, ix_data2, ix_ctrlr;
   UINT16 port_ctrll, port_data1, port_data2, port_ctrlr;

   /* Queuing
      mmi_packet_queue for packets from TSG to MMI
      tsg_packet_queue for packets from MMI to TSG
   */
   s_packet_queue mmi_packet_queue;
   s_packet_queue tsg_packet_queue;

   /* GPP api function input */
   UINT8  gpp_send_buf[MAX_GPP_PACKET_LENGTH];
   UINT16 gpp_send_buf_size;

   UINT8  mmi_recv_data[MAX_GPP_PACKET_LENGTH];  /*< receiver buffer */
   UINT16 mmi_recv_data_size;                    /*< length of packet size */
   UINT16 mmi_recv_data_ix;                      /*< current index */
   UINT8  mmi_new_data_from_mmi;                 /*< to recognize incoming new data from remote */

   /* GPP state machine work data */
   UINT8  gpp_packet_index_expected;

   UINT8  gpp_work_state;       /* state of machine */
   /* control the data transmission by */
   UINT16 gpp_send_buf_ix;      /* current index */
   UINT16 gpp_last_send_buf_ix; /* last index */
   UINT8  gpp_packet_ix;        /* packet index */
   UINT16 gpp_size_remain;      /* the remaining data to send */
   UINT16 gpp_last_size_remain; /* the last remaining data to send */

   UINT8  gpp_conn_state_local; /* 0,1,2,3           '......cc'*/
   UINT8  gpp_conn_state_remote;/* 0,1,2,3           '......cc'*/
   UINT8  gpp_seq_from_tsg;     /*                     TSGout 'sss.....'*/
   UINT8  gpp_ack_from_remote;  /* MMIin  '...aaa..'                    */
   UINT8  gpp_seq_from_remote;  /* MMIin  'sss.....' = TSGout '...aaa..'*/
   UINT8  gpp_seq_from_remote_old;

   /* for data logging */
   UINT16 gpp_logging_enabled;
   UINT16 gpp_mmi_invalid_counter;

   /* several data */
   UINT8  unexpected_cnt;
   UINT8  gpp_enabled;
   UINT8  disconnected;  /* flag for DIAG entry set/reset */
   UINT8  mmi_queue_overflow; /*< set in ncase that MMI queue overflow */

   /* limitation for event output: Exx, Hxxx */
   UINT16 warn_limit;
   UINT8  warn_limit2;
   UINT8  warn_limit3;
   UINT8  warn_limit4;
} s_gpp_main_data;



/******************************************************************************
 * storage (global)
 *****************************************************************************/
extern UINT8  gpp_nr_of_cabins;       /* configured number of cabins */
extern UINT8  atp_active_cabin;       /* from ATP 213.evc2.mmi_m_active_cabin */

extern UINT16 nr_of_gpp_local_ports;
extern UINT16 nr_of_gpp_remote_ports;
extern UINT16 max_entries_receive;
extern UINT16 max_entries_send;

extern s_gpp_main_data  gpp_data_cabin[];     /* data instance for all cabins */
extern s_gpp_main_data* p_cabin;              /* current switched data instance */
extern UINT8    gpp_cabin;            /* current switched cabin: 1 - gpp_nr_of_cabins */

extern UINT8  gpp_trace_enabled;      /* also for gpp_qm_mmi2tsg.c gpp_qm_tsg2mmi.c */
extern UINT8  gpp_remote_print;       /* print out for the remote (e.g. TESTER) */

/* for export to du_event.c
*/
extern UINT8  gpp0_module_nr;         /* gpp_main.c */
extern UINT8  gpp1_module_nr;         /* gpp_qm_tsg2mmi.c */
extern UINT8  gpp2_module_nr;         /* gpp_qm_mmi2tsg.c */

/* seq/ack tracing GPP protocol (only for the selected cabin ) */
extern s_gpp_seq_trace gpp_seq_trace[];
extern UINT16  gpp_seq_trace_curr_entry;


/* to connect the du_event and trace functions
   (if no TSG available these pointers are set to GPP internal dummy functions)
*/
extern UINT8 (*fp_du_get_module_number)(SINT8* p_mn  /* __FILE__ */);
extern void          (*fp_du_gpp_event)        (UINT8 modul_nr, UINT16 lnr,
      SINT8* p_text,
      UINT16 p1, UINT16 p2, UINT16 p3, UINT16 p4);


/* to connect several functions
   (if no TSG available, these functions are not called)
*/
extern void          (*fp_du_port_data)        (UINT16 port_addr, UINT16* port_data);
extern void          (*fp_gpp_send_text)       (UINT8 cabin, SINT8* p_msg);
extern void          (*fp_gpp_diag_entry)      (UINT16 entry_nr,
      UINT8 p1, UINT8 p2, UINT16 err_nr);


/******************************************************************************
 * Public Functions (GPP internal)
 *****************************************************************************/

/* the GPP application interface used in CORE-part (TSG) */
extern void gpp_set_cabin(UINT8 cabin);


#endif /* GPP_MAIN_H */

/* eof */
