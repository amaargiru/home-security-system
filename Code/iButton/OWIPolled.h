// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
*
* Atmel Corporation
*
* \li File:               OWIPolled.h
* \li Compiler:           IAR EWAAVR 3.20a
* \li Support mail:       avr@atmel.com
*
* \li Supported devices:  All AVRs.
*
* \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
*
*
* \li Description:        Defines used in the polled 1-Wire(R) driver.
*
*                         $Revision: 1.7 $
*                         $Date: Thursday, August 19, 2004 14:27:18 UTC $
****************************************************************************/

#ifndef _OWI_POLLED_H_
#define _OWI_POLLED_H_

#include "OWIdefs.h"
#include "OWIDeviceSpecific.h"

/*****************************************************************************
 User defines
*****************************************************************************/
// Uncomment one of the two following lines to choose between 
// software only and UART driver.
#define     OWI_SOFTWARE_DRIVER    
//#define     OWI_UART_DRIVER

/*****************************************************************************
 The following defines only has an effect on the software only driver.
*****************************************************************************/
/*! \brief  CPU clock frequency.
 *
 *  This define is used to calculate delays when the software only driver
 *  is used. The CPU frequency must be at least 2.170 MHz to be able to
 *  generate the shortest delays.
 */
#define     CPU_FREQUENCY   8.000

 /*! \brief  Use internal pull-up resistor on 1-Wire buses.
  *
  *  If this symbol is defined, the internal pull-up resister on the GPIO pins
  *  of the AVR will be used to generate the necessary pull-up on the bus. If
  *  an external pull-up resistor is used, uncomment this define.
  */
#define     OWI_USE_INTERNAL_PULLUP

// Port configuration registers for 1-Wire buses.
// Make sure that all three registers belong to the same port.
#define     OWI_PORT        PORTA   //!< 1-Wire PORT Data register.
#define     OWI_PIN         PINA    //!< 1-Wire Input pin register.
#define     OWI_DDR         DDRA    //!< 1-Wire Data direction register.

/*****************************************************************************
 Other defines
*****************************************************************************/
// Pin bitmasks.
#define     OWI_PIN_3       0x08

/*****************************************************************************
 Timing parameters
*****************************************************************************/

#define     OWI_DELAY_OFFSET_CYCLES    13   //!< Timing delay when pulling bus low and releasing bus.

// Bit timing delays in clock cycles (= us*clock freq in MHz).
#define     OWI_DELAY_A_STD_MODE    ((6   * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_B_STD_MODE    ((64  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_C_STD_MODE    ((60  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_D_STD_MODE    ((10  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_E_STD_MODE    ((9   * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_F_STD_MODE    ((55  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_H_STD_MODE    ((480 * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_I_STD_MODE    ((70  * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)
#define     OWI_DELAY_J_STD_MODE    ((410 * CPU_FREQUENCY) - OWI_DELAY_OFFSET_CYCLES)

#endif
