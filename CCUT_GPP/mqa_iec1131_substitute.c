/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2010 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       mqa_iec1131_substitute.c
 *
 *  \author     W. Landwehr
 *
 *  \date       28.09.2010
 *
 *  \brief      Implement MVB application interface for quick TM access.
 *
 *****************************************************************************
 */


/*-----------------------------------------------------------------------------------
 mqa_iec1131_substitute.c

 Copyright
 (c) 2010 by Bombardier Transportation (Signal) Germany GmbH (creation: 28.09.2010)

 HISTORY (creation: 28.09.2010
 28.09.2010 W. Landwehr: new
 ------------------------------------------------------------------------------------
*/


/************************************************************************************



 ==>   Siddhant Tomar and Alen Pljesa  (for internal use only !)




 U S I N G    of     M Q A    S u b s t i t u t i o n s    with    G P P   5.0.0.0

 General
 - if possible on your target use only the   libgpp.a  and the  gpp_api.h
   (otherwise build, based on the delivered sources)
 - to substitute MQA use the delivered original  mqa_api.h  and
   this example comleted by your Interface arrays (see above)
 - use the logical steps of GPP using shown in GPP_EXAMPLE.C


 Precondition
 - the correct MVB access (with handling the internal port-buffer pages should be
   done completely before you do the port access by using the MQA substitutions.
 - this also affects the MVB write function.

 That means the run sequence must be

 1. MVB read access is done by the TCN level and the received data are available
    in the according interface buffer
 2. the GPP read (gpp_read_data_packets(..) according the example in GPP_EXAMPLE.C
    is done
 3. any internal processing with the received ports takes place
    any internal processing to fill the write ports takes place
 4. the GPP write (gpp_write_data_packets(..) according the example in GPP_EXAMPLE.C
    is done.
 5. The standard MVB port write is done by the TCN level

 The TCN stack should not run as a parallel task if it is possible. Otherwise the
 time behaviour is not predictable and it may need a longer run time to get a
 consistant set of data.

 Pay attention
 - based on a GPP port cycle of 64 ms a data transaction (send from TSG to MMI and
   receiving the response) is estimated with 200 ms (based on former done
   mesurements). That means the logical content of a port set should not change in
   a frequency that is shorter than this time.

 To run your Test
 - start both devices and do the initialisation steps as shown inGPP_EXAMPLE.C or
   in a own init task.
 - Then let run the cyclic part to send and receive data (use the queueing to avoid
   loss of data, at last in your application, if the behaviour of sending is not
   predictable/cyclic and thus asynchronous).
   Otherwise fill your send buffer as you want.



************************************************************************************/
#include "mwt_types.h"
#include "usertypes.h"

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
/*lint -e921*/ /*allow explicit casting*/

#ifdef insideMTPE

static BYTE* pMMI_TSG_F1 = (BYTE*)NULL;
static BYTE* pTSG_MMI_F1_P1 = (BYTE*)NULL;
static BYTE* pTSG_MMI_F1_P2 = (BYTE*)NULL;
static BYTE* pTSG_MMI_F1_P3 = (BYTE*)NULL;
static BYTE* pMMI_TSG_F2 = (BYTE*)NULL;
static BYTE* pTSG_MMI_F2_P1 = (BYTE*)NULL;
static BYTE* pTSG_MMI_F2_P2 = (BYTE*)NULL;
static BYTE* pTSG_MMI_F2_P3 = (BYTE*)NULL;

void mqa_init_port_adresses_cab1(BYTE* pP1, BYTE* pP2, BYTE* pP3, BYTE* pF1)
{
   if ((pP1==NULL) || (pP2==NULL) || (pP3==NULL) || (pF1==NULL))
   {
      return;
   }

   pMMI_TSG_F1 = pF1;
   pTSG_MMI_F1_P1 = pP1;
   pTSG_MMI_F1_P2 = pP2;
   pTSG_MMI_F1_P3 = pP3;
}

void mqa_init_port_adresses_cab2(BYTE* pP1, BYTE* pP2, BYTE* pP3, BYTE* pF1)
{
   if ((pP1==NULL) || (pP2==NULL) || (pP3==NULL) || (pF1==NULL))
   {
      return;
   }

   pMMI_TSG_F2 = pF1;
   pTSG_MMI_F2_P1 = pP1;
   pTSG_MMI_F2_P2 = pP2;
   pTSG_MMI_F2_P3 = pP3;
}

UINT16 mqa_get_port_index(UINT16 port_addr)
{
   /*
   ------------------------------------------------------------------------------------
   This function shall deliver the port index onto the internal TM address table
   for a quick port access.

   Input parameter:
     port_addr           the MVB port address to look for

   Return value:
     index               0 = port not found
   ------------------------------------------------------------------------------------
   */
   UINT16 ix = (UINT16)0;

   switch (port_addr)
   {
      /* work with fix known port addresses
         (may be substituted by any own algorithm)

        this is used in GPP:
        gpp_init_ports(1, 810, 811, 812, 820);
        gpp_init_ports(2, 813, 814, 815, 821);

        */
   case 810:
   {
      ix = (UINT16)1;
      break;
   }
   case 811:
   {
      ix = (UINT16)2;
      break;
   }
   case 812:
   {
      ix = (UINT16)3;
      break;
   }
   case 820:
   {
      ix = (UINT16)4;
      break;
   }

   case 813:
   {
      ix = (UINT16)5;
      break;
   }
   case 814:
   {
      ix = (UINT16)6;
      break;
   }
   case 815:
   {
      ix = (UINT16)7;
      break;
   }
   case 821:
   {
      ix = (UINT16)8;
      break;
   }

   default :
   {
      ix = (UINT16)0;
   }
   }

   return ix;
} /* mqa_get_port_index */


UINT16* mqa_get_srce_port_tm_addr_by_index(UINT16 ix)
{
   /*
   ------------------------------------------------------------------------------------------
   This function shall deliver the pointer onto the TM port address range of the valid page.

   Input parameter:
     ix                     the port index onto the internal TM address table
   Return value:
     p_tm                   pointer onto the port data range in TM
   ------------------------------------------------------------------------------------------
   */
   UINT16* p_srce = (UINT16*)NULL;

   switch (ix)
   {
   case 1: /* 810 */
   {
      p_srce = (UINT16*)pTSG_MMI_F1_P1;
      break;
   }
   case 2: /* 811 */
   {
      p_srce = (UINT16*)pTSG_MMI_F1_P2;
      break;
   }
   case 3: /* 812 */
   {
      p_srce = (UINT16*)pTSG_MMI_F1_P3;
      break;
   }
   case 4: /* 820 */
   {
      p_srce = (UINT16*)pMMI_TSG_F1;
      break;
   }

   case 5: /* 813 */
   {
      p_srce = (UINT16*)pTSG_MMI_F2_P1;
      break;
   }
   case 6: /* 814 */
   {
      p_srce = (UINT16*)pTSG_MMI_F2_P2;
      break;
   }
   case 7: /* 815 */
   {
      p_srce = (UINT16*)pTSG_MMI_F2_P3;
      break;
   }
   case 8: /* 821 */
   {
      p_srce = (UINT16*)pMMI_TSG_F2;
      break;
   }

   default :
   {
      p_srce = (UINT16*)NULL;
   }
   }
   return p_srce;
}



UINT16* mqa_get_sink_port_tm_addr_by_index(UINT16 ix)
{
   /*
   ------------------------------------------------------------------------------------------
   This function shall deliver the pointer onto the TM port address range of the valid page.

   Input parameter:
     ix                     the port index onto the internal TM address table
   Return value:
     p_tm                   pointer onto the port data range in TM
   ------------------------------------------------------------------------------------------
   */
   UINT16* p_sink = (UINT16*)NULL;

   switch (ix)
   {
   case 1: /* 810 */
   {
      p_sink = (UINT16*)pTSG_MMI_F1_P1;
      break;
   }
   case 2: /* 811 */
   {
      p_sink = (UINT16*)pTSG_MMI_F1_P2;
      break;
   }
   case 3: /* 812 */
   {
      p_sink = (UINT16*)pTSG_MMI_F1_P3;
      break;
   }
   case 4: /* 820 */
   {
      p_sink = (UINT16*)pMMI_TSG_F1;
      break;
   }

   case 5: /* 813 */
   {
      p_sink = (UINT16*)pTSG_MMI_F2_P1;
      break;
   }
   case 6: /* 814 */
   {
      p_sink = (UINT16*)pTSG_MMI_F2_P2;
      break;
   }
   case 7: /* 815 */
   {
      p_sink = (UINT16*)pTSG_MMI_F2_P3;
      break;
   }
   case 8: /* 821 */
   {
      p_sink = (UINT16*)pMMI_TSG_F2;
      break;
   }

   default :
   {
      p_sink = (UINT16*)NULL;
   }
   }
   return p_sink;
}

#endif
/*
#undef insideMTPE
*/
