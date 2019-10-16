/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2011 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       rts_api.h
 *
 *  \author     Bjarne Jensen
 *
 *  \date       25.01.2011
 *
 *  \brief      Application Programming Interface header file
 *
 *****************************************************************************
 */


/******************************************************************************
 *  COPYRIGHT    : (c) 2004 Bombardier Transportation
 ******************************************************************************
 *  PROJECT      : CSS2
 *
 *  MODULE       : rts_api.h
 *
 *  ABSTRACT     : Application Programming Interface header file
 *
 ******************************************************************************
 *  HISTORY      :
 *   $Log: /CSS/CSSLib/COMMON/api/rts_api.h $
 *
 *  9     11-01-25 15:40 Bjarne Jensen
 *  CSS_CR_CSS3_238: Make sure rts_api.h is all an app needs.
 *  Added all API files to rts_com.h.
 *  Added define O_CSS in order to handle nvram_api.h.
 *
 *  8     06-02-09 13:17 Peter Brander
 *  Headers according to coding convention
 *
 *  7     05-11-09 16:15 Christer Olsson
 *  Including rts_dev.h device specific API
 *
 *  6     05-10-27 16:16 Mattias Rehnman
 *  Make O_DC default for non 68k targets.
 *
 *  5     05-09-05 16:26 Peter Brander
 *  Erliar included temporary solution deleted.
 *
 *  4     05-05-11 15:51 Thomas Clevström
 *  Define O_DC only if it is not defined in the make environment,
 *
 *  3     05-05-11 13:39 Peter Brander
 *  A temporary solution is added. Shall be deleted when Mitrac Tool is
 *  adapted for CSS2
 *
 *  2     05-05-10 15:04 Peter Brander
 *  Include file rts_dev.h deleted
 *
 *  1     05-05-10 14:58 Peter Brander
 *  Copy from QND
 *
 *  1     05-01-12 14:38 Thomas Clevström
 *  Quick-and-Dirty port of CSS for DCU2.
 *  Mix of CSS 1.12, NG-VTCU and some CSS2.
*
*  3     02-09-18 9:37 Thomas Clevström
*  Minor cleanup and complements to enable auto-generation of contents
*  file.
*
*  2     97-06-27 9.24 Anders Wallin
*  Header added
*
*  1     96-11-25 14.59 Anders Wallin
*  First version
*
******************************************************************************/


#ifndef O_DC
#if !(CPU==CPU32)
#define O_DC
#endif
#endif

#ifndef O_CSS
/* Needed by some generic library API files */
#define O_CSS
#endif

/*****************************************************************************
*   INCLUDES
*/
#include "rts.h"
#include "rts_com.h"
#include "rts_dev.h"

/*****************************************************************************
*   DEFINES
*/

/*******************************************************************************
* GLOBAL CONSTANTS
*/

/*****************************************************************************
*   TYPEDEFS
*/

/*****************************************************************************
*   GLOBAL VARIABLES
*/

/*****************************************************************************
*   GLOBAL FUNCTION DECLARATIONS
*/
