/*!
 *****************************************************************************
 *  COPYRIGHT   : (c) 2012 Bombardier Transportation BTPC
 *****************************************************************************
 *
 *  \file       gpp_version.h
 *
 *  \author     W. Landwehr
 *
 *  \date       12.04.2012
 *
 *  \brief      Contains the version string according the CSS rules with following assembly:
 *
 *   e.p.v.r
 *   | | | |
 *   | | | +----  release, increment if any minor update or fix takes place
 *   | | +------  version, increment if any major update takes place
 *   | +--------  project id: 0 = for all projects
 *   +----------  ETCS release, increment at change of the main TRACK
 *
 *****************************************************************************
 */


/**************************************************************************************************
 * gpp_version.h
 * Copyright (c) 2011 by Bombardier Transportation GmbH
 * --------------------------------------------------------------------------------
 * DESCRIPTION
 * Contains the version string according the CSS rules with following assembly:
 *
 *   e.p.v.r
 *   | | | |
 *   | | | +----  release, increment if any minor update or fix takes place
 *   | | +------  version, increment if any major update takes place
 *   | +--------  project id: 0 = for all projects
 *   +----------  ETCS release, increment at change of the main TRACK
 * --------------------------------------------------------------------------------
 * HISTORY
 * 5.88.0.5  17.04.2012 W. Landwehr  fra_005#906  Upgrade MTPE and other Equipment for CSS 1.14.6
 * 5.88.0.4  08.03.2012 W. Landwehr  fra_005#873  Update GPP lib (minor change due to test env.)
 * 5.88.0.3  07.11.2011 W. Landwehr  fra_005#772  Minor GPP adaptations (TSGT print-out channel)
 * 5.88.0.2  10.10.2011 W. Landwehr  fra_005#747  GPP upgrade regarding ATP activated cabin
 * 5.88.0.1  22.06.2011 W. Landwehr  fra_005#625: test switch check upgraded
 * 5.88.0.0  26.05.2011 W. Landwehr  avoid cyclic hint H702
 * 5.00.0.0  29.06.2010 W. Landwehr  adaptation for 5xxx, MAX_GPP_PACKET_LENGTH set to 1200
 **************************************************************************************************
 */
#ifndef GPP_VERSION_H
#define GPP_VERSION_H


#define VERSION_GPP  "5.88.0.5"


#endif /* GPP_VERSION_H */

/* eof */
