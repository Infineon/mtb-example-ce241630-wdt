/*******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty Application Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
********************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* WDT time out for reset mode, in milliseconds. */
#define WDT_TIME_OUT_MS          5000

/* Ignore bits calculated for timeout between 4s to 6s. 
 * Note: User needs to re-calculate this value for different 
 *       timeouts other than 4 to 6s */
#define IGNORE_BITS_ABOVE        15

#define ENABLE_BLOCKING_FUNCTION 0

/* Min and Max Timout values achived for each IGNORE_BITS_ABOVE 
 * IGNORE_BITS = 4 min_timeout_ms = 2 max_timeout_ms = 2
 * IGNORE_BITS = 5 min_timeout_ms = 4 max_timeout_ms = 5
 * IGNORE_BITS = 6 min_timeout_ms = 8 max_timeout_ms = 11
 * IGNORE_BITS = 7 min_timeout_ms = 16 max_timeout_ms = 23
 * IGNORE_BITS = 8 min_timeout_ms = 32 max_timeout_ms = 46
 * IGNORE_BITS = 9 min_timeout_ms = 63 max_timeout_ms = 93
 * IGNORE_BITS = 10 min_timeout_ms = 126 max_timeout_ms = 187
 * IGNORE_BITS = 11 min_timeout_ms = 251 max_timeout_ms = 375
 * IGNORE_BITS = 12 min_timeout_ms = 501 max_timeout_ms = 750
 * IGNORE_BITS = 13 min_timeout_ms = 1001 max_timeout_ms = 1500
 * IGNORE_BITS = 14 min_timeout_ms = 2001 max_timeout_ms = 3000
 * IGNORE_BITS = 15 min_timeout_ms = 4001 max_timeout_ms = 6000
 * IGNORE_BITS = 16 min_timeout_ms = 8001 max_timeout_ms = 12000
 * IGNORE_BITS = 17 min_timeout_ms = 16001 max_timeout_ms = 24000
 * IGNORE_BITS = 18 min_timeout_ms = 32001 max_timeout_ms = 48000
 * IGNORE_BITS = 19 min_timeout_ms = 64001 max_timeout_ms = 96000
 * IGNORE_BITS = 20 min_timeout_ms = 128001 max_timeout_ms = 192000
 * IGNORE_BITS = 21 min_timeout_ms = 256001 max_timeout_ms = 384000
 * IGNORE_BITS = 22 min_timeout_ms = 512001 max_timeout_ms = 768000
 * IGNORE_BITS = 23 min_timeout_ms = 1024001 max_timeout_ms = 1536000
 * IGNORE_BITS = 24 min_timeout_ms = 2048001 max_timeout_ms = 3072000
 * IGNORE_BITS = 25 min_timeout_ms = 4096001 max_timeout_ms = 6144000
 * IGNORE_BITS = 26 min_timeout_ms = 8192001 max_timeout_ms = 12288000
 * IGNORE_BITS = 27 min_timeout_ms = 16384001 max_timeout_ms = 24576000
 * IGNORE_BITS = 28 min_timeout_ms = 32768001 max_timeout_ms = 49152000
 * IGNORE_BITS = 29 min_timeout_ms = 65536001 max_timeout_ms = 98304000
 * IGNORE_BITS = 30 min_timeout_ms = 131072001 max_timeout_ms = 196608000
 * IGNORE_BITS = 31 min_timeout_ms = 262144001 max_timeout_ms = 393216000 */

/* Match count =  Desired interrupt interval in ms x ILO Frequency in Hz */
#define WDT_MATCH_COUNT(wdt_time_out_ms, ignore_bits_above) \
                        (((wdt_time_out_ms * 32768ULL) / 1000ULL) - \
                            (2ULL * (1ULL << (ignore_bits_above + 1))) - 1ULL)

/*******************************************************************************
* Global Variables
*******************************************************************************/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void initialize_wdt();

/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CPU. It...
*    1.
*    2.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

#if defined (CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Check the reason for device restart */
    if(CY_SYSLIB_RESET_HWWDT == Cy_SysLib_GetResetReason())
    {
        /* It's WDT reset event - blink LED twice */
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
        Cy_SysLib_Delay(100);
        Cy_GPIO_Set(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
        Cy_SysLib_Delay(200);
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
        Cy_SysLib_Delay(100);
        Cy_GPIO_Set(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
    }
    else
    {
        /* It's Power-On reset or XRES event - blink LED once */
        Cy_GPIO_Clr(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
        Cy_SysLib_Delay(100);
        Cy_GPIO_Set(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
        Cy_SysLib_Delay(100);
    }

    /* Clears the reset cause register */
    Cy_SysLib_ClearResetReason();

    /* Initialize WDT */
    initialize_wdt();

    /* Enable global interrupts */
    __enable_irq();

    for (;;)
    {
#if (ENABLE_BLOCKING_FUNCTION)
         while(1);
#else
         /* Reset WDT */
         Cy_WDT_ClearWatchdog();

        /* Constant delay of 1000ms */
        Cy_SysLib_Delay(1000);

        /* Invert the state of LED */
        Cy_GPIO_Inv(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
#endif

    }
}

/*******************************************************************************
* Function Name: initialize_wdt
********************************************************************************
* Summary:
* This function initializes the WDT block
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void initialize_wdt()
{
   /* Step 1- Unlock WDT */
   Cy_WDT_Unlock();

   /* Step 2- Write the ignore bits */
   Cy_WDT_SetMatchBits(IGNORE_BITS_ABOVE);

   /* Step 3- Write match value */
   Cy_WDT_SetMatch(WDT_MATCH_COUNT(WDT_TIME_OUT_MS, IGNORE_BITS_ABOVE));

   /* Step 4- Clear match event interrupt, if any */
   Cy_WDT_ClearInterrupt();

   /* Step 5- Enable WDT */
   Cy_WDT_Enable();

   /* Step 6- Lock WDT configuration */
   Cy_WDT_Lock();
}
/* [] END OF FILE */
