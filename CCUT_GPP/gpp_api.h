/*******************************************************************************
 *
 * Copyright (C)
 * Bombardier Transportation GmbH, Hennigsdorf 2011
 * all rights reserved
 *
 * project Do2010, SBB
 *
 ******************************************************************************/

/************************ Doxygen file information ****************************/
/*								       */
/*! \file     gpp_api.h
 * \brief     This include file delivers the API relevant content of gpp
 *
 * \author    M. Kuenzel
 * \version   0.0.0.8
 * \date      06.01.2016
 * \copyright Bombardier Transportation GmbH, Hennigsdorf 2011
 * \todo      none
 * \warning   none
 */
/************************ end  Doxygen file information ***********************/
/*                                                                            */
/***************************** change history **********************************/
/*!
    \version 0.0.0.4
    \date    2016/01/06
    \author  MKU (CEE/ETID)
    \brief   B2DOS00008560 

    \version 0.0.0.3
    \date    2011/11/03
    \author  W. Landwehr
    \brief   fra_005#772  Minor GPP adaptations (TSGT print-out channal)

    \version 0.0.0.2
    \date    2011/10/07
    \author  W. Landwehr
    \brief   fra_005#747  GPP upgrade regarding ATP activated cabin

    \version 0.0.0.1
    \date    2011/05/11
    \author  W. Landwehr
    \brief   adaptation for 5000, moved function

 ******************************************************************************/

#ifndef GPP_API_H  /* avoid multible include of this file */
#define GPP_API_H  /* avoid multible include if this file */

/*******************************************************************************
 * Defines  (D O N' T   C H A N G E  !!)
 ******************************************************************************/

/* the following defines the current GPP scope.
*/
#define MAX_GPP_NR_OF_CABINS   2  /* maximum cabins handled by the current GPP sodtware */
#define GPP_F1ST_CABIN         1  /* don't change ! ! ! (always 1) */

/* nr. of send ports*/
#define GPP_NR_PORTS_MMI_TSG   1  /* this is the LOCAL parameter of gpp_init() in the MMI/TESTER or remote */
#define GPP_NR_PORTS_TSG_MMI   3  /* this is the LOCAL parameter of gpp_init() in the TSG */

/* maximum packet length of a gpp data set */
#define MAX_GPP_PACKET_LENGTH  1200

/* IDs for diagnosis queue entries via cdp_diag_queue_entry_gpp(..)
*/
#define K_GPP_INIT             0x07
#define G_GPP_INIT             0x77
#define K_GPP_CONNECTION       0x08
#define G_GPP_CONNECTION       0x80
#define O_GPP_QUEUE_TSG_MMI    0x99

/* substitute the os_io_printf by using this function pointer which contains the
 * original os_io_printf function or the socket based tsg_printf function
 * (usable for functions which needs another output channel)
*/
#define OS_IO_PRINTF           fp_os_io_printf
/*#define OS_IO_PRINTF           os_io_printf*/

/*******************************************************************************
 * Storage
 ******************************************************************************/

   /* symbol could be made static */
   /*lint -e765*/

/* to connect the du_event and trace functions
   (if no TSG available these pointers are set to GPP internal dummy functions)
*/
extern UINT8 (*fp_du_get_module_number)(SINT8* p_mn  /* __FILE__ */);
extern void          (*fp_du_gpp_event)        (UINT8 modul_nr, UINT16 lnr, SINT8* p_text,
      UINT16 p1, UINT16 p2, UINT16 p3, UINT16 p4);

/* to substitute print out function for TSGT own channel*/

extern INT32           (*fp_os_io_printf)        (const SINT8* format, ... );

/* to connect several functions
(if no TSG available, these functions are not called)
*/
extern void          (*fp_du_port_data)        (UINT16 port_addr, UINT16* port_data);
extern void          (*fp_gpp_send_text)       (UINT8 cabin, SINT8* p_msg);
extern void          (*fp_gpp_diag_entry)      (UINT16 entry_nr, UINT8 p1, UINT8 p2, UINT16 err_nr);


/*******************************************************************************
* Public Functions for GPP main function
******************************************************************************/

/*!
 *  \fn     void gpp_init_ports(UINT8 cabin, UINT16 ctrll_port, UINT16 data_port1, UINT16 data_port2, UINT16 ctrlr_port)
 *
 *  \brief  This function sets the data environment for the selected cabin, cabin1 = default.
 *          Attension: The 1st function to call - before gpp_init()!
 *
 *  \param  cabin - cabin to set ports to (0 < cabin <= number of cabins)
 *          - [=0] function do nothing
 *          - [1..MAX_GPP_NR_OF_CABINS] function works normal
 *          - [MAX_GPP_NR_OF_CABINS..255] function do nothing
 *  \param  ctrll_port - number of control port from TSG to HMI
 *          - [0..65535] function works normal
 *  \param  data_port1 - number of data port 1 from TSG to HMI
 *          - [0..65535] function works normal
 *  \param  data_port2 - number of data port 2 from TSG to HMI
 *          - [0..65535] function works normal
 *  \param  ctrlr_port - number of control port from HMI to TSG
 *          - [0..65535] function works normal
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - OUT: gpp_port_ctrll
 *          - OUT: gpp_port_data1
 *          - OUT: gpp_port_data2
 *          - OUT: gpp_port_ctrlr
 *          - OUT: gpp_nr_of_cabins
 *
 */
extern void           gpp_init_ports(UINT8 cabin,
                             UINT16 ctrll_port, UINT16 data_port1, UINT16 data_port2,
                             UINT16 ctrlr_port);
/*!
 *  \fn     void gpp_init(UINT16 nr_of_local_ports, UINT16 nr_of_remote_ports, BYTE* ctrll_port_cab1, BYTE* data_port1_cab1, BYTE* data_port2_cab1, BYTE* ctrlr_port_cab1, BYTE* ctrll_port_cab2, BYTE* data_port1_cab2, BYTE* data_port2_cab2, BYTE* ctrlr_port_cab2)
 *
 *  \brief  This function initializes all cabin environments,
 *          sets the module number and prepares storage variables.
 *
 *  \param  nr_of_local_ports - number of ports from MMI to TSG
 *          - [=0] function do nothing
 *          - [1..MAX_PORTS_GPP] function works normal
 *          - [MAX_PORTS_GPP..255] function do nothing
 *  \param  nr_of_remote_ports - number of ports from TSG to MMI
 *          - [=0] function do nothing
 *          - [1..MAX_PORTS_GPP] function works normal
 *          - [MAX_PORTS_GPP..255] function do nothing
 *  \param  ctrll_port_cab1 - pointer to MVB buffer of control port from TSG to HMI of cabin 1
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  data_port1_cab1 - pointer to MVB buffer of data port 1 from TSG to HMI of cabin 1
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  data_port2_cab1 - pointer to MVB buffer of data port 2 from TSG to HMI of cabin 1
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  ctrlr_port_cab1 - pointer to MVB buffer of control port from HMI to TSG of cabin 1
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  ctrll_port_cab2 - pointer to MVB buffer of control port from TSG to HMI of cabin 2
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  data_port1_cab2 - pointer to MVB buffer of data port 1 from TSG to HMI of cabin 2
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  data_port2_cab2 - pointer to MVB buffer of data port 2 from TSG to HMI of cabin 2
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *  \param  ctrlr_port_cab2 - pointer to MVB buffer of control port from HMI to TSG of cabin 2
 *          - [=0] function returns without doing anything
 *          - [>0] function working normal
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: gpp_port_ctrll
 *          - IN: gpp_port_data1
 *          - IN: gpp_port_data2
 *          - IN: gpp_port_ctrlr
 *          - IN: fp_gpp_diag_entry
 *          - OUT: fp_os_io_printf
 *          - OUT: fp_du_gpp_event
 *          - OUT: gpp0_module_nr
 *          - OUT: gpp_data_cabin
 *          - OUT: nr_of_gpp_local_ports
 *          - OUT: nr_of_gpp_remote_ports
 *          - OUT: max_entries_send
 *          - OUT: max_entries_receive
 *          - OUT: gpp_nr_of_cabins
 *          - OUT: gpp_seq_trace
 *          - OUT: p_cabin
 *          - OUT: gpp_cabin
 */
extern void gpp_init(UINT16 nr_of_local_ports, UINT16 nr_of_remote_ports,
      BYTE* ctrll_port_cab1, BYTE* data_port1_cab1, BYTE* data_port2_cab1, BYTE* ctrlr_port_cab1,
      BYTE* ctrll_port_cab2, BYTE* data_port1_cab2, BYTE* data_port2_cab2, BYTE* ctrlr_port_cab2);

/*!
 *  \fn     void gpp_conn_state(void)
 *
 *  \brief  This function controls the connection state with MMI.
 *          It is called cyclic by adm_main() after gpp_read_data_packets() in each cycle.
 *
 *  \param  none 
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: p_cabin
 *          - IN: fp_gpp_diag_entry
 *          - IN/OUT: gpp_data_cabin
 *
 */
extern void           gpp_conn_state(void);

/*!
 *  \fn     void gpp_write_data_packets(void)
 *
 *  \brief  This function contains the GPP state machine to transmit the application data via MVB
 *          to the destination device. In each cycle one data subset is sent to the remote device.
 *          This will be repeated until the whole application data stream is transmitted.
 *
 *  \param  none 
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: p_cabin
 *          - IN/OUT: gpp_data_cabin
 */
extern void           gpp_write_data_packets(void);

/*!
 *  \fn     void gpp_read_data_packets(void)
 *
 *  \brief  This function reads the incoming data from the port set and stores it into a receive
 *          buffer. After receiving the complete application data stream it will be stored into the
 *          tsg_packet_queue by calling the function gpp_qm_store_tsg_data.
 *
 *  \param  none 
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: p_cabin
 *          - IN/OUT: gpp_data_cabin
 */
extern void           gpp_read_data_packets(void);


/*******************************************************************************
 * Public Functions for GPP main util function
 ******************************************************************************/

/*!
 *  \fn     void gpp_set_cabin(UINT8 cabin)
 *
 *  \brief  This function sets the data environment for the selected cabin, cabin1 = default.
 *
 *  Note for DCPU only  o n e  cabin (GPP queue) will be used. In case of received packets
 *  from channel 2 (SAP 5/9) this function will be called to switch to cabin 2, but
 *  due to increasing the configured amount of cabins (=1) the instance pointer remains
 *  on the default value.
 *
 *  \param  cabin - cabin to sent data to (0 < cabin <= number of cabins)
 *          - [=0] function do nothing
 *          - [1..gpp_nr_of_cabins] function works normal
 *          - [gpp_nr_of_cabins..255] function do nothing
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: gpp_nr_of_cabins
 *          - IN: gpp_data_cabin
 *          - OUT: p_cabin
 *          - OUT: gpp_cabin
 *
 */
extern void           gpp_set_cabin(UINT8 cabin);

/*!
 *  \fn     void gpp_trace_enable(UINT8 on_off)
 *
 *  \brief  This function enables/disables the GPP MVB Trace.:
 *          <tsgm gpp show/n>
 *
 *  \param  on_off - tracing [on, off] [1/2, 0]
 *          - [=0] function disable tracing of GPP
 *          - [=1] function enable tracing of GPP
 *          - [=2] function enable tracing of GPP with additional time stamps
 *          - [3..255] function do nothing
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - OUT: gpp_trace_enabled
 */
extern void           gpp_trace_enable(UINT8 on_off);


/*******************************************************************************
 * Public Functions for GPP queue manager  TSG -> MMI
 ******************************************************************************/

/*!
 *  \fn     UINT16 gpp_qm_store_mmi_data(const UINT8* p_packet, UINT8 nid_stm, UINT16 length)
 *
 *  \brief  This function stores one packet into the remote (MMI) packet queue.
 *
 *  \param  p_packet - pointer onto the field with application data
 *          - [=0] function returns with GPP_ERR
 *          - [>0] function working normal
 *  \param  nid_stm - identifier of the sender (STM, ATP ,TSG)
 *          - [0..65535] function works normal
 *  \param  length - number of bytes of the application data
 *          - [=0] function do nothing
 *          - [1..MAX_GPP_PACKET_LENGTH] function works normal
 *          - [MAX_GPP_PACKET_LENGTH..65535] function do nothing
 *
 *  \return GPP_OK 
 *          - packet stored
 *  \return GPP_ERR 
 *          - invalid packet length (0 || MAX_GPP_PACKET_LENGTH), no queue element empty or NULL pointer as parameter
 *  \return GPP_NO_CONNECTION 
 *          - the GPP connection is not yet established
 *
 *  \note   global variables used:
 *          - IN: fp_gpp_send_text
 *          - IN: gpp_trace_enabled
 *          - IN: max_entries_send
 *          - IN: p_cabin
 *          - IN/OUT: gpp_data_cabin
 */
extern UINT16 gpp_qm_store_mmi_data(const UINT8* p_packet, UINT8 nid_stm, UINT16 length);

/*!
 *  \fn     void gpp_qm_read_mmi_data(void)
 *
 *  \brief  This function checks the GPP state whether a transmission is finished. In this case
 *          a possibly new data stream from the MMI queue is read and stored into the internal
 *          send buffer for transmission to the remote (MMI) by calling the function
 *          gpp_write_data_packets().
 *
 *  \param  none 
 *
 *  \return none
 *
 *  \note   global variables used:
 *          - IN: gpp_trace_enabled
 *          - IN: max_entries_send
 *          - IN: p_cabin
 *          - IN/OUT: gpp_data_cabin
 */
extern void gpp_qm_read_mmi_data(void);

/*!
 *  \fn     UINT8  gpp_mmi_queue_overflow(UINT8 cabin) 
 *
 *  \brief  This function returns the queue overflow flag.
 *
 *  \param  cabin -  cabin instance 1/2 (0 < cabin <= number of cabins)
 *          - [=0] function do nothing
 *          - [1..gpp_nr_of_cabins] function works normal
 *          - [gpp_nr_of_cabins..255] function do nothing
 *
 *  \return TRUE
 *          - overflow flag is set
 *  \return FALSE
 *          - overflow flag is not set
 *
 *  \note   global variables used:
 *          - IN: gpp_nr_of_cabins
 *          - IN: gpp_data_cabin
 *
 *  \note   static variables used:
 *          - IN/OUT: event_brake
 */
extern UINT8  gpp_mmi_queue_overflow(UINT8 cabin);

/*******************************************************************************
 * Public Functions for GPP queue manager  MMI -> TSG
 ******************************************************************************/

/*!
 *  \fn     UINT16 gpp_qm_read_tsg_data(UINT8* p_packet, UINT16 max_packet_length)
 *
 *  \brief  This function reads one packet from the local (TSG) queue and returns the packet lenght.
 *
 *  \param  p_packet - pointer to the data field, the data field returns the readed packet
 *          - [=0] function returns with 0 and do nothing
 *          - [>0] function working normal
 *  \param  max_packet_length - maximal length of the data field
 *          - [0..65535] function working normal
 *
 *  \return [=0] error or no packet found
 *  \return [1..65535] Packet length
 *
 *  \note   global variables used
 *          - IN: gpp_trace_enabled
 *          - IN: max_entries_receive
 *          - IN: p_cabin
 *          - IN/OUT: gpp_data_cabin
 */
extern UINT16 gpp_qm_read_tsg_data(UINT8* p_packet, UINT16 max_packet_length);

/*******************************************************************************
 * for export to du_event.c
 ******************************************************************************/
extern UINT8  gpp0_module_nr;         /* gpp_main.c */
extern UINT8  gpp01_module_nr;        /* gpp_main_util.c */
extern UINT8  gpp1_module_nr;         /* gpp_qm_tsg2mmi.c */
extern UINT8  gpp2_module_nr;         /* gpp_qm_mmi2tsg.c */

#endif /* GPP_API_H / avoid multible include of this file */
/*EOF*/
