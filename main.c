//*****************************************************************************
//
// Application Name     - Blinky
// Application Overview - The objective of this application is to showcase the 
//                        GPIO control using Driverlib api calls. The LEDs 
//                        connected to the GPIOs on the LP are used to indicate 
//                        the GPIO output. The GPIOs are driven high-low 
//                        periodically in order to turn on-off the LEDs.
//
//*****************************************************************************

//****************************************************************************
//
//! \addtogroup blinky
//! @{
//
//****************************************************************************

// Standard includes
#include <stdio.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// Common interface includes
#include "gpio_if.h"

#include "pinmux.h"

#define APPLICATION_VERSION     "1.1.1"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES                           
//*****************************************************************************
void LEDBlinkyRoutine();
static void BoardInit(void);



void LEDBlinkyRoutine()
{
    //
    // Toggle the lines initially to turn off the LEDs.
    // The values driven are as required by the LEDs on the LP.
    //
	unsigned long delay = 8000000;

    while(1)
    {


        MAP_UtilsDelay(delay);
        GPIOPinWrite(GPIOA3_BASE,0x40,0x40);	// Set pin 53, GPIO30 (Set pin to logic 1)
        MAP_UtilsDelay(delay);
        GPIOPinWrite(GPIOA0_BASE,0x40,0x40);	// Set pin 61, GPIO6 (Set pin to logic 1)
        MAP_UtilsDelay(delay);
        GPIOPinWrite(GPIOA0_BASE,0x20,0x20);	// Set pin 60, GPIO5 (Set pin to logic 1)
        MAP_UtilsDelay(delay);
        GPIOPinWrite(GPIOA3_BASE,0x40,0);		// Reset PIN 53, GPIO30 (Set pin to logic 0)
        MAP_UtilsDelay(delay);
        GPIOPinWrite(GPIOA0_BASE,0x40,0);		// Reset pin 61, GPIO6	(Set pin to logic 0)
        MAP_UtilsDelay(delay);
        GPIOPinWrite(GPIOA0_BASE,0x20,0);		// Reset pin 60, GPIO5	(Set pin to logic 0)
    }

}
//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}
//****************************************************************************
//
//! Main function
//!
//! \param none
//! 
//! This function  
//!    1. Invokes the LEDBlinkyTask
//!
//! \return None.
//
//****************************************************************************
int
main()
{
    //
    // Initialize Board configurations
    //
    BoardInit();
    
    //
    // Power on the corresponding GPIO port B for 9,10,11.
    // Set up the GPIO lines to mode 0 (GPIO)
    //
    PinMuxConfig();

    //
    //Enable VCC_BRD
    //
    GPIOPinWrite(GPIOA0_BASE,0x80,0x80); 	//Set  Pin 62 to 1. Enable power to the booster

    //
    // Start the LEDBlinkyRoutine
    //
    LEDBlinkyRoutine();
    return 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
