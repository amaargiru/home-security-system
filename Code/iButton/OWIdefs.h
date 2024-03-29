// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* \li File:               OWIdefs.h
* \li Compiler:           IAR EWAAVR 3.20a
* \li Support mail:       avr@atmel.com
*
* \li Supported devices:  All AVRs.
*
* \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
*
*
* \li Description:        Common definitions used for all implementations.
*
*                         $Revision: 1.7 $
*                         $Date: Thursday, August 19, 2004 14:27:16 UTC $
****************************************************************************/

#ifndef _OWI_DEFS_H_
#define _OWI_DEFS_H_

#include <ioavr.h>

/****************************************************************************
 ROM commands
****************************************************************************/
#define     OWI_ROM_READ    0x33    //!< READ ROM command code.
#define     OWI_ROM_SKIP    0xcc    //!< SKIP ROM command code.
#define     OWI_ROM_MATCH   0x55    //!< MATCH ROM command code.
#define     OWI_ROM_SEARCH  0xf0    //!< SEARCH ROM command code.

/****************************************************************************
 Return codes
****************************************************************************/
#define     OWI_ROM_SEARCH_FINISHED     0x00    //!< Search finished return code.
#define     OWI_ROM_SEARCH_FAILED       0xff    //!< Search failed return code.

#endif
