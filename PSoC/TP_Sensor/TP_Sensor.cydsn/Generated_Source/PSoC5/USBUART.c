/*******************************************************************************
* File Name: USBUART.c
* Version 2.70
*
* Description:
*  API for USBFS Component.
*
* Note:
*  Many of the functions use endpoint number.  RAM arrays are sized with 9
*  elements so they are indexed directly by epNumber.  The SIE and ARB
*  registers are indexed by variations of epNumber - 1.
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <CyDmac.h>
#include "USBUART.h"
#include "USBUART_pvt.h"
#include "USBUART_hid.h"
#if(USBUART_DMA1_REMOVE == 0u)
    #include "USBUART_ep1_dma.h"
#endif   /*  USBUART_DMA1_REMOVE */
#if(USBUART_DMA2_REMOVE == 0u)
    #include "USBUART_ep2_dma.h"
#endif   /*  USBUART_DMA2_REMOVE */
#if(USBUART_DMA3_REMOVE == 0u)
    #include "USBUART_ep3_dma.h"
#endif   /*  USBUART_DMA3_REMOVE */
#if(USBUART_DMA4_REMOVE == 0u)
    #include "USBUART_ep4_dma.h"
#endif   /*  USBUART_DMA4_REMOVE */
#if(USBUART_DMA5_REMOVE == 0u)
    #include "USBUART_ep5_dma.h"
#endif   /*  USBUART_DMA5_REMOVE */
#if(USBUART_DMA6_REMOVE == 0u)
    #include "USBUART_ep6_dma.h"
#endif   /*  USBUART_DMA6_REMOVE */
#if(USBUART_DMA7_REMOVE == 0u)
    #include "USBUART_ep7_dma.h"
#endif   /*  USBUART_DMA7_REMOVE */
#if(USBUART_DMA8_REMOVE == 0u)
    #include "USBUART_ep8_dma.h"
#endif   /*  USBUART_DMA8_REMOVE */
#if((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u))
    #include "USBUART_EP_DMA_Done_isr.h"
    #include "USBUART_EP8_DMA_Done_SR.h"
    #include "USBUART_EP17_DMA_Done_SR.h"
#endif /* ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u)) */


/***************************************
* Global data allocation
***************************************/

uint8 USBUART_initVar = 0u;
#if(USBUART_EP_MM != USBUART__EP_MANUAL)
    uint8 USBUART_DmaChan[USBUART_MAX_EP];
    uint8 USBUART_DmaTd[USBUART_MAX_EP];
#endif /*  USBUART_EP_MM */
#if((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u))
    static uint8 clearInDataRdyStatus = USBUART_ARB_EPX_CFG_DEFAULT;
    uint8 USBUART_DmaNextTd[USBUART_MAX_EP];
    const uint8 USBUART_epX_TD_TERMOUT_EN[USBUART_MAX_EP] =
    {   0u,
        USBUART_ep1_TD_TERMOUT_EN,
        USBUART_ep2_TD_TERMOUT_EN,
        USBUART_ep3_TD_TERMOUT_EN,
        USBUART_ep4_TD_TERMOUT_EN,
        USBUART_ep5_TD_TERMOUT_EN,
        USBUART_ep6_TD_TERMOUT_EN,
        USBUART_ep7_TD_TERMOUT_EN,
        USBUART_ep8_TD_TERMOUT_EN
    };
    volatile uint16 USBUART_inLength[USBUART_MAX_EP];
    const uint8 *USBUART_inDataPointer[USBUART_MAX_EP];
    volatile uint8 USBUART_inBufFull[USBUART_MAX_EP];
#endif /* ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u)) */


/*******************************************************************************
* Function Name: USBUART_Start
********************************************************************************
*
* Summary:
*  This function initialize the USB SIE, arbiter and the
*  endpoint APIs, including setting the D+ Pullup
*
* Parameters:
*  device: Contains the device number of the desired device descriptor.
*          The device number can be found in the Device Descriptor Tab of
*          "Configure" dialog, under the settings of desired Device Descriptor,
*          in the "Device Number" field.
*  mode: The operating voltage. This determines whether the voltage regulator
*        is enabled for 5V operation or if pass through mode is used for 3.3V
*        operation. Symbolic names and their associated values are given in the
*        following table.
*       USBUART_3V_OPERATION - Disable voltage regulator and pass-thru
*                                       Vcc for pull-up
*       USBUART_5V_OPERATION - Enable voltage regulator and use
*                                       regulator for pull-up
*       USBUART_DWR_VDDD_OPERATION - Enable or Disable voltage
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*  The USBUART_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time USBUART_Start() is called.
*  This allows for component Re-Start without unnecessary re-initialization
*  in all subsequent calls to the USBUART_Start() routine.
*  If re-initialization of the component is required the variable should be set
*  to zero before call of UART_Start() routine, or the user may call
*  USBUART_Init() and USBUART_InitComponent() as done
*  in the USBUART_Start() routine.
*
* Side Effects:
*   This function will reset all communication states to default.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Start(uint8 device, uint8 mode) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(USBUART_initVar == 0u)
    {
        USBUART_Init();
        USBUART_initVar = 1u;
    }
    USBUART_InitComponent(device, mode);
}


/*******************************************************************************
* Function Name: USBUART_Init
********************************************************************************
*
* Summary:
*  Initialize component's hardware. Usually called in USBUART_Start().
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_Init(void) 
{
    uint8 enableInterrupts;
    #if(USBUART_EP_MM != USBUART__EP_MANUAL)
        uint16 i;
    #endif   /*  USBUART_EP_MM != USBUART__EP_MANUAL */

    enableInterrupts = CyEnterCriticalSection();

    /* Enable USB block  */
    USBUART_PM_ACT_CFG_REG |= USBUART_PM_ACT_EN_FSUSB;
    /* Enable USB block for Standby Power Mode */
    USBUART_PM_STBY_CFG_REG |= USBUART_PM_STBY_EN_FSUSB;

    /* Enable core clock */
    USBUART_USB_CLK_EN_REG = USBUART_USB_CLK_ENABLE;

    USBUART_CR1_REG = USBUART_CR1_ENABLE_LOCK;

    /* ENABLING USBIO PADS IN USB MODE FROM I/O MODE */
    /* Ensure USB transmit enable is low (USB_USBIO_CR0.ten). - Manual Transmission - Disabled */
    USBUART_USBIO_CR0_REG &= ((uint8)(~USBUART_USBIO_CR0_TEN));
    CyDelayUs(0u);  /*~50ns delay */
    /* Disable the USBIO by asserting PM.USB_CR0.fsusbio_pd_n(Inverted)
    *  high. This will have been set low by the power manger out of reset.
    *  Also confirm USBIO pull-up disabled
    */
    USBUART_PM_USB_CR0_REG &= ((uint8)(~(USBUART_PM_USB_CR0_PD_N |
                                                  USBUART_PM_USB_CR0_PD_PULLUP_N)));

    /* Select iomode to USB mode*/
    USBUART_USBIO_CR1_REG &= ((uint8)(~USBUART_USBIO_CR1_IOMODE));

    /* Enable the USBIO reference by setting PM.USB_CR0.fsusbio_ref_en.*/
    USBUART_PM_USB_CR0_REG |= USBUART_PM_USB_CR0_REF_EN;
    /* The reference will be available 1 us after the regulator is enabled */
    CyDelayUs(1u);
    /* OR 40us after power restored */
    CyDelayUs(40u);
    /* Ensure the single ended disable bits are low (PRT15.INP_DIS[7:6])(input receiver enabled). */
    USBUART_DM_INP_DIS_REG &= ((uint8)(~USBUART_DM_MASK));
    USBUART_DP_INP_DIS_REG &= ((uint8)(~USBUART_DP_MASK));

    /* Enable USBIO */
    USBUART_PM_USB_CR0_REG |= USBUART_PM_USB_CR0_PD_N;
    CyDelayUs(2u);
    /* Set the USBIO pull-up enable */
    USBUART_PM_USB_CR0_REG |= USBUART_PM_USB_CR0_PD_PULLUP_N;

    /* Write WAx */
    CY_SET_REG8(USBUART_ARB_RW1_WA_PTR,     0u);
    CY_SET_REG8(USBUART_ARB_RW1_WA_MSB_PTR, 0u);

    #if(USBUART_EP_MM != USBUART__EP_MANUAL)
        /* Init transfer descriptor. This will be used to detect the DMA state - initialized or not. */
        for (i = 0u; i < USBUART_MAX_EP; i++)
        {
            USBUART_DmaTd[i] = DMA_INVALID_TD;
            #if ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u))
                USBUART_DmaNextTd[i] = DMA_INVALID_TD;
            #endif /* ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u)) */
        }
    #endif   /*  USBUART_EP_MM != USBUART__EP_MANUAL */

    CyExitCriticalSection(enableInterrupts);


    /* Set the bus reset Interrupt. */
    (void) CyIntSetVector(USBUART_BUS_RESET_VECT_NUM,   &USBUART_BUS_RESET_ISR);
    CyIntSetPriority(USBUART_BUS_RESET_VECT_NUM, USBUART_BUS_RESET_PRIOR);

    /* Set the SOF Interrupt. */
    #if(USBUART_SOF_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_SOF_VECT_NUM,   &USBUART_SOF_ISR);
        CyIntSetPriority(USBUART_SOF_VECT_NUM, USBUART_SOF_PRIOR);
    #endif   /*  USBUART_SOF_ISR_REMOVE */

    /* Set the Control Endpoint Interrupt. */
    (void) CyIntSetVector(USBUART_EP_0_VECT_NUM,   &USBUART_EP_0_ISR);
    CyIntSetPriority(USBUART_EP_0_VECT_NUM, USBUART_EP_0_PRIOR);

    /* Set the Data Endpoint 1 Interrupt. */
    #if(USBUART_EP1_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_1_VECT_NUM,   &USBUART_EP_1_ISR);
        CyIntSetPriority(USBUART_EP_1_VECT_NUM, USBUART_EP_1_PRIOR);
    #endif   /*  USBUART_EP1_ISR_REMOVE */

    /* Set the Data Endpoint 2 Interrupt. */
    #if(USBUART_EP2_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_2_VECT_NUM,   &USBUART_EP_2_ISR);
        CyIntSetPriority(USBUART_EP_2_VECT_NUM, USBUART_EP_2_PRIOR);
    #endif   /*  USBUART_EP2_ISR_REMOVE */

    /* Set the Data Endpoint 3 Interrupt. */
    #if(USBUART_EP3_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_3_VECT_NUM,   &USBUART_EP_3_ISR);
        CyIntSetPriority(USBUART_EP_3_VECT_NUM, USBUART_EP_3_PRIOR);
    #endif   /*  USBUART_EP3_ISR_REMOVE */

    /* Set the Data Endpoint 4 Interrupt. */
    #if(USBUART_EP4_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_4_VECT_NUM,   &USBUART_EP_4_ISR);
        CyIntSetPriority(USBUART_EP_4_VECT_NUM, USBUART_EP_4_PRIOR);
    #endif   /*  USBUART_EP4_ISR_REMOVE */

    /* Set the Data Endpoint 5 Interrupt. */
    #if(USBUART_EP5_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_5_VECT_NUM,   &USBUART_EP_5_ISR);
        CyIntSetPriority(USBUART_EP_5_VECT_NUM, USBUART_EP_5_PRIOR);
    #endif   /*  USBUART_EP5_ISR_REMOVE */

    /* Set the Data Endpoint 6 Interrupt. */
    #if(USBUART_EP6_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_6_VECT_NUM,   &USBUART_EP_6_ISR);
        CyIntSetPriority(USBUART_EP_6_VECT_NUM, USBUART_EP_6_PRIOR);
    #endif   /*  USBUART_EP6_ISR_REMOVE */

     /* Set the Data Endpoint 7 Interrupt. */
    #if(USBUART_EP7_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_7_VECT_NUM,   &USBUART_EP_7_ISR);
        CyIntSetPriority(USBUART_EP_7_VECT_NUM, USBUART_EP_7_PRIOR);
    #endif   /*  USBUART_EP7_ISR_REMOVE */

    /* Set the Data Endpoint 8 Interrupt. */
    #if(USBUART_EP8_ISR_REMOVE == 0u)
        (void) CyIntSetVector(USBUART_EP_8_VECT_NUM,   &USBUART_EP_8_ISR);
        CyIntSetPriority(USBUART_EP_8_VECT_NUM, USBUART_EP_8_PRIOR);
    #endif   /*  USBUART_EP8_ISR_REMOVE */

    #if((USBUART_EP_MM != USBUART__EP_MANUAL) && (USBUART_ARB_ISR_REMOVE == 0u))
        /* Set the ARB Interrupt. */
        (void) CyIntSetVector(USBUART_ARB_VECT_NUM,   &USBUART_ARB_ISR);
        CyIntSetPriority(USBUART_ARB_VECT_NUM, USBUART_ARB_PRIOR);
    #endif   /*  USBUART_EP_MM != USBUART__EP_MANUAL */

}


/*******************************************************************************
* Function Name: USBUART_InitComponent
********************************************************************************
*
* Summary:
*  Initialize the component, except for the HW which is done one time in
*  the Start function.  This function pulls up D+.
*
* Parameters:
*  device: Contains the device number of the desired device descriptor.
*          The device number can be found in the Device Descriptor Tab of
*          "Configure" dialog, under the settings of desired Device Descriptor,
*          in the "Device Number" field.
*  mode: The operating voltage. This determines whether the voltage regulator
*        is enabled for 5V operation or if pass through mode is used for 3.3V
*        operation. Symbolic names and their associated values are given in the
*        following table.
*       USBUART_3V_OPERATION - Disable voltage regulator and pass-thru
*                                       Vcc for pull-up
*       USBUART_5V_OPERATION - Enable voltage regulator and use
*                                       regulator for pull-up
*       USBUART_DWR_VDDD_OPERATION - Enable or Disable voltage
*                         regulator depend on Vddd Voltage configuration in DWR.
*
* Return:
*   None.
*
* Global variables:
*   USBUART_device: Contains the device number of the desired device
*       descriptor. The device number can be found in the Device Descriptor Tab
*       of "Configure" dialog, under the settings of desired Device Descriptor,
*       in the "Device Number" field.
*   USBUART_transferState: This variable used by the communication
*       functions to handle current transfer state. Initialized to
*       TRANS_STATE_IDLE in this API.
*   USBUART_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_InitComponent(uint8 device, uint8 mode) 
{
    /* Initialize _hidProtocol variable to comply with
    *  HID 7.2.6 Set_Protocol Request:
    *  "When initialized, all devices default to report protocol."
    */
    #if defined(USBUART_ENABLE_HID_CLASS)
        uint8 i;

        for (i = 0u; i < USBUART_MAX_INTERFACES_NUMBER; i++)
        {
            USBUART_hidProtocol[i] = USBUART_PROTOCOL_REPORT;
        }
    #endif /* USBUART_ENABLE_HID_CLASS */

    /* Enable Interrupts. */
    CyIntEnable(USBUART_BUS_RESET_VECT_NUM);
    CyIntEnable(USBUART_EP_0_VECT_NUM);
    #if(USBUART_EP1_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_1_VECT_NUM);
    #endif   /*  USBUART_EP1_ISR_REMOVE */
    #if(USBUART_EP2_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_2_VECT_NUM);
    #endif   /*  USBUART_EP2_ISR_REMOVE */
    #if(USBUART_EP3_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_3_VECT_NUM);
    #endif   /*  USBUART_EP3_ISR_REMOVE */
    #if(USBUART_EP4_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_4_VECT_NUM);
    #endif   /*  USBUART_EP4_ISR_REMOVE */
    #if(USBUART_EP5_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_5_VECT_NUM);
    #endif   /*  USBUART_EP5_ISR_REMOVE */
    #if(USBUART_EP6_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_6_VECT_NUM);
    #endif   /*  USBUART_EP6_ISR_REMOVE */
    #if(USBUART_EP7_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_7_VECT_NUM);
    #endif   /*  USBUART_EP7_ISR_REMOVE */
    #if(USBUART_EP8_ISR_REMOVE == 0u)
        CyIntEnable(USBUART_EP_8_VECT_NUM);
    #endif   /*  USBUART_EP8_ISR_REMOVE */
    #if((USBUART_EP_MM != USBUART__EP_MANUAL) && (USBUART_ARB_ISR_REMOVE == 0u))
        /* usb arb interrupt enable */
        USBUART_ARB_INT_EN_REG = USBUART_ARB_INT_MASK;
        CyIntEnable(USBUART_ARB_VECT_NUM);
    #endif   /*  USBUART_EP_MM != USBUART__EP_MANUAL */

    /* Arbiter configuration for DMA transfers */
    #if(USBUART_EP_MM != USBUART__EP_MANUAL)
        #if(USBUART_EP_MM == USBUART__EP_DMAMANUAL)
            USBUART_ARB_CFG_REG = USBUART_ARB_CFG_MANUAL_DMA;
        #endif   /*  USBUART_EP_MM == USBUART__EP_DMAMANUAL */
        #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
            /*Set cfg cmplt this rises DMA request when the full configuration is done */
            USBUART_ARB_CFG_REG = USBUART_ARB_CFG_AUTO_DMA | USBUART_ARB_CFG_AUTO_MEM;
            #if(USBUART_EP_DMA_AUTO_OPT == 0u)
                /* Init interrupt which handles verification of the successful DMA transaction */
                USBUART_EP_DMA_Done_isr_StartEx(USBUART_EP_DMA_DONE_ISR);
                USBUART_EP17_DMA_Done_SR_InterruptEnable();
                USBUART_EP8_DMA_Done_SR_InterruptEnable();
            #endif /* USBUART_EP_DMA_AUTO_OPT == 0u */
        #endif   /*  USBUART_EP_MM == USBUART__EP_DMAAUTO */
    #endif   /*  USBUART_EP_MM != USBUART__EP_MANUAL */

    USBUART_transferState = USBUART_TRANS_STATE_IDLE;

    /* USB Locking: Enabled, VRegulator: depend on mode or DWR Voltage configuration*/
    switch(mode)
    {
        case USBUART_3V_OPERATION:
            USBUART_CR1_REG = USBUART_CR1_ENABLE_LOCK;
            break;
        case USBUART_5V_OPERATION:
            USBUART_CR1_REG = USBUART_CR1_ENABLE_LOCK | USBUART_CR1_REG_ENABLE;
            break;
        default:   /*USBUART_DWR_VDDD_OPERATION */
            #if(USBUART_VDDD_MV < USBUART_3500MV)
                USBUART_CR1_REG = USBUART_CR1_ENABLE_LOCK;
            #else
                USBUART_CR1_REG = USBUART_CR1_ENABLE_LOCK | USBUART_CR1_REG_ENABLE;
            #endif /*  USBUART_VDDD_MV < USBUART_3500MV */
            break;
    }

    /* Record the descriptor selection */
    USBUART_device = device;

    /* Clear all of the component data */
    USBUART_configuration = 0u;
    USBUART_interfaceNumber = 0u;
    USBUART_configurationChanged = 0u;
    USBUART_deviceAddress  = 0u;
    USBUART_deviceStatus = 0u;

    USBUART_lastPacketSize = 0u;

    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBUART_EP0_CR_PTR, USBUART_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_CR0_PTR, USBUART_CR0_ENABLE);

    /* Workaround for PSOC5LP */
    CyDelayCycles(1u);

    /* Finally, Enable d+ pullup and select iomode to USB mode*/
    CY_SET_REG8(USBUART_USBIO_CR1_PTR, USBUART_USBIO_CR1_USBPUEN);
}


/*******************************************************************************
* Function Name: USBUART_ReInitComponent
********************************************************************************
*
* Summary:
*  This function reinitialize the component configuration and is
*  intend to be called from the Reset interrupt.
*
* Parameters:
*  None.
*
* Return:
*   None.
*
* Global variables:
*   USBUART_device: Contains the device number of the desired device
*        descriptor. The device number can be found in the Device Descriptor Tab
*       of "Configure" dialog, under the settings of desired Device Descriptor,
*       in the "Device Number" field.
*   USBUART_transferState: This variable used by the communication
*       functions to handle current transfer state. Initialized to
*       TRANS_STATE_IDLE in this API.
*   USBUART_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_lastPacketSize initialized to 0;
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_ReInitComponent(void) 
{
    /* Initialize _hidProtocol variable to comply with HID 7.2.6 Set_Protocol
    *  Request: "When initialized, all devices default to report protocol."
    */
    #if defined(USBUART_ENABLE_HID_CLASS)
        uint8 i;

        for (i = 0u; i < USBUART_MAX_INTERFACES_NUMBER; i++)
        {
            USBUART_hidProtocol[i] = USBUART_PROTOCOL_REPORT;
        }
    #endif /* USBUART_ENABLE_HID_CLASS */

    USBUART_transferState = USBUART_TRANS_STATE_IDLE;

    /* Clear all of the component data */
    USBUART_configuration = 0u;
    USBUART_interfaceNumber = 0u;
    USBUART_configurationChanged = 0u;
    USBUART_deviceAddress  = 0u;
    USBUART_deviceStatus = 0u;

    USBUART_lastPacketSize = 0u;


    /*  ACK Setup, Stall IN/OUT */
    CY_SET_REG8(USBUART_EP0_CR_PTR, USBUART_MODE_STALL_IN_OUT);

    /* Enable the SIE with an address 0 */
    CY_SET_REG8(USBUART_CR0_PTR, USBUART_CR0_ENABLE);

}


/*******************************************************************************
* Function Name: USBUART_Stop
********************************************************************************
*
* Summary:
*  This function shuts down the USB function including to release
*  the D+ Pullup and disabling the SIE.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*   USBUART_configuration: Contains current configuration number
*       which is set by the Host using SET_CONFIGURATION request.
*       Initialized to zero in this API.
*   USBUART_deviceAddress: Contains current device address. This
*       variable is initialized to zero in this API. Host starts to communicate
*      to device with address 0 and then set it to whatever value using
*      SET_ADDRESS request.
*   USBUART_deviceStatus: initialized to 0.
*       This is two bit variable which contain power status in first bit
*       (DEVICE_STATUS_BUS_POWERED or DEVICE_STATUS_SELF_POWERED) and remote
*       wakeup status (DEVICE_STATUS_REMOTE_WAKEUP) in second bit.
*   USBUART_configurationChanged: This variable is set to one after
*       SET_CONFIGURATION request and cleared in this function.
*   USBUART_intiVar variable is set to zero
*
*******************************************************************************/
void USBUART_Stop(void) 
{

    #if(USBUART_EP_MM != USBUART__EP_MANUAL)
        USBUART_Stop_DMA(USBUART_MAX_EP);     /* Stop all DMAs */
    #endif   /*  USBUART_EP_MM != USBUART__EP_MANUAL */

    /* Disable the SIE */
    USBUART_CR0_REG &= (uint8)(~USBUART_CR0_ENABLE);
    /* Disable the d+ pullup */
    USBUART_USBIO_CR1_REG &= (uint8)(~USBUART_USBIO_CR1_USBPUEN);
    /* Disable USB in ACT PM */
    USBUART_PM_ACT_CFG_REG &= (uint8)(~USBUART_PM_ACT_EN_FSUSB);
    /* Disable USB block for Standby Power Mode */
    USBUART_PM_STBY_CFG_REG &= (uint8)(~USBUART_PM_STBY_EN_FSUSB);

    /* Disable the reset and EP interrupts */
    CyIntDisable(USBUART_BUS_RESET_VECT_NUM);
    CyIntDisable(USBUART_EP_0_VECT_NUM);
    #if(USBUART_EP1_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_1_VECT_NUM);
    #endif   /*  USBUART_EP1_ISR_REMOVE */
    #if(USBUART_EP2_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_2_VECT_NUM);
    #endif   /*  USBUART_EP2_ISR_REMOVE */
    #if(USBUART_EP3_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_3_VECT_NUM);
    #endif   /*  USBUART_EP3_ISR_REMOVE */
    #if(USBUART_EP4_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_4_VECT_NUM);
    #endif   /*  USBUART_EP4_ISR_REMOVE */
    #if(USBUART_EP5_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_5_VECT_NUM);
    #endif   /*  USBUART_EP5_ISR_REMOVE */
    #if(USBUART_EP6_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_6_VECT_NUM);
    #endif   /*  USBUART_EP6_ISR_REMOVE */
    #if(USBUART_EP7_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_7_VECT_NUM);
    #endif   /*  USBUART_EP7_ISR_REMOVE */
    #if(USBUART_EP8_ISR_REMOVE == 0u)
        CyIntDisable(USBUART_EP_8_VECT_NUM);
    #endif   /*  USBUART_EP8_ISR_REMOVE */

    /* Clear all of the component data */
    USBUART_configuration = 0u;
    USBUART_interfaceNumber = 0u;
    USBUART_configurationChanged = 0u;
    USBUART_deviceAddress  = 0u;
    USBUART_deviceStatus = 0u;
    USBUART_initVar = 0u;

}


/*******************************************************************************
* Function Name: USBUART_CheckActivity
********************************************************************************
*
* Summary:
*  Returns the activity status of the bus.  Clears the status hardware to
*  provide fresh activity status on the next call of this routine.
*
* Parameters:
*  None.
*
* Return:
*  1 - If bus activity was detected since the last call to this function
*  0 - If bus activity not was detected since the last call to this function
*
*******************************************************************************/
uint8 USBUART_CheckActivity(void) 
{
    uint8 r;

    r = CY_GET_REG8(USBUART_CR1_PTR);
    CY_SET_REG8(USBUART_CR1_PTR, (r & ((uint8)(~USBUART_CR1_BUS_ACTIVITY))));

    return((r & USBUART_CR1_BUS_ACTIVITY) >> USBUART_CR1_BUS_ACTIVITY_SHIFT);
}


/*******************************************************************************
* Function Name: USBUART_GetConfiguration
********************************************************************************
*
* Summary:
*  Returns the current configuration setting
*
* Parameters:
*  None.
*
* Return:
*  configuration.
*
*******************************************************************************/
uint8 USBUART_GetConfiguration(void) 
{
    return(USBUART_configuration);
}


/*******************************************************************************
* Function Name: USBUART_IsConfigurationChanged
********************************************************************************
*
* Summary:
*  Returns the clear on read configuration state. It is usefull when PC send
*  double SET_CONFIGURATION request with same configuration number.
*
* Parameters:
*  None.
*
* Return:
*  Not zero value when new configuration has been changed, otherwise zero is
*  returned.
*
* Global variables:
*   USBUART_configurationChanged: This variable is set to one after
*       SET_CONFIGURATION request and cleared in this function.
*
*******************************************************************************/
uint8 USBUART_IsConfigurationChanged(void) 
{
    uint8 res = 0u;

    if(USBUART_configurationChanged != 0u)
    {
        res = USBUART_configurationChanged;
        USBUART_configurationChanged = 0u;
    }

    return(res);
}


/*******************************************************************************
* Function Name: USBUART_GetInterfaceSetting
********************************************************************************
*
* Summary:
*  Returns the alternate setting from current interface
*
* Parameters:
*  uint8 interfaceNumber, interface number
*
* Return:
*  Alternate setting.
*
*******************************************************************************/
uint8  USBUART_GetInterfaceSetting(uint8 interfaceNumber)
                                                    
{
    return(USBUART_interfaceSetting[interfaceNumber]);
}


/*******************************************************************************
* Function Name: USBUART_GetEPState
********************************************************************************
*
* Summary:
*  Returned the state of the requested endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*
* Return:
*  State of the requested endpoint.
*
*******************************************************************************/
uint8 USBUART_GetEPState(uint8 epNumber) 
{
    return(USBUART_EP[epNumber].apiEpState);
}


/*******************************************************************************
* Function Name: USBUART_GetEPCount
********************************************************************************
*
* Summary:
*  This function supports Data Endpoints only(EP1-EP8).
*  Returns the transfer count for the requested endpoint.  The value from
*  the count registers includes 2 counts for the two byte checksum of the
*  packet.  This function subtracts the two counts.
*
* Parameters:
*  epNumber: Data Endpoint Number.
*            Valid values are between 1 and 8.
*
* Return:
*  Returns the current byte count from the specified endpoint or 0 for an
*  invalid endpoint.
*
*******************************************************************************/
uint16 USBUART_GetEPCount(uint8 epNumber) 
{
    uint8 ri;
    uint16 result = 0u;

    if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP))
    {
        ri = ((epNumber - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);

        result = (uint8)(CY_GET_REG8((reg8 *)(USBUART_SIE_EP1_CNT0_IND + ri)) &
                          USBUART_EPX_CNT0_MASK);
        result = (result << 8u) | CY_GET_REG8((reg8 *)(USBUART_SIE_EP1_CNT1_IND + ri));
        result -= USBUART_EPX_CNTX_CRC_COUNT;
    }
    return(result);
}


#if(USBUART_EP_MM != USBUART__EP_MANUAL)


    /*******************************************************************************
    * Function Name: USBUART_InitEP_DMA
    ********************************************************************************
    *
    * Summary:
    *  This function allocates and initializes a DMA channel to be used by the
    *  USBUART_LoadInEP() or USBUART_ReadOutEP() APIs for data
    *  transfer.
    *
    * Parameters:
    *  epNumber: Contains the data endpoint number.
    *            Valid values are between 1 and 8.
    *  *pData: Pointer to a data array that is related to the EP transfers.
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_InitEP_DMA(uint8 epNumber, const uint8* pData)
                                                                    
    {
        uint16 src;
        uint16 dst;
        #if (CY_PSOC3)                  /* PSoC 3 */
            src = HI16(CYDEV_SRAM_BASE);
            dst = HI16(CYDEV_PERIPH_BASE);
            pData = pData;
        #else                           /* PSoC 5 */
            if((USBUART_EP[epNumber].addr & USBUART_DIR_IN) != 0u )
            {   /* for the IN EP source is the SRAM memory buffer */
                src = HI16(pData);
                dst = HI16(CYDEV_PERIPH_BASE);
            }
            else
            {   /* for the OUT EP source is the SIE register */
                src = HI16(CYDEV_PERIPH_BASE);
                dst = HI16(pData);
            }
        #endif  /*  C51 */
        switch(epNumber)
        {
            case USBUART_EP1:
                #if(USBUART_DMA1_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep1_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA1_REMOVE */
                break;
            case USBUART_EP2:
                #if(USBUART_DMA2_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep2_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA2_REMOVE */
                break;
            case USBUART_EP3:
                #if(USBUART_DMA3_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep3_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA3_REMOVE */
                break;
            case USBUART_EP4:
                #if(USBUART_DMA4_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep4_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA4_REMOVE */
                break;
            case USBUART_EP5:
                #if(USBUART_DMA5_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep5_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA5_REMOVE */
                break;
            case USBUART_EP6:
                #if(USBUART_DMA6_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep6_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA6_REMOVE */
                break;
            case USBUART_EP7:
                #if(USBUART_DMA7_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep7_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA7_REMOVE */
                break;
            case USBUART_EP8:
                #if(USBUART_DMA8_REMOVE == 0u)
                    USBUART_DmaChan[epNumber] = USBUART_ep8_DmaInitialize(
                        USBUART_DMA_BYTES_PER_BURST, USBUART_DMA_REQUEST_PER_BURST, src, dst);
                #endif   /*  USBUART_DMA8_REMOVE */
                break;
            default:
                /* Do not support EP0 DMA transfers */
                break;
        }
        if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP))
        {
            USBUART_DmaTd[epNumber] = CyDmaTdAllocate();
            #if ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u))
                USBUART_DmaNextTd[epNumber] = CyDmaTdAllocate();
            #endif /*  ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u)) */

        }
    }


    /*******************************************************************************
    * Function Name: USBUART_Stop_DMA
    ********************************************************************************
    *
    * Summary: Stops and free DMA
    *
    * Parameters:
    *  epNumber: Contains the data endpoint number or
    *           USBUART_MAX_EP to stop all DMAs
    *
    * Return:
    *  None.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void USBUART_Stop_DMA(uint8 epNumber) 
    {
        uint8 i;
        i = (epNumber < USBUART_MAX_EP) ? epNumber : USBUART_EP1;
        do
        {
            if(USBUART_DmaTd[i] != DMA_INVALID_TD)
            {
                (void) CyDmaChDisable(USBUART_DmaChan[i]);
                CyDmaTdFree(USBUART_DmaTd[i]);
                USBUART_DmaTd[i] = DMA_INVALID_TD;
            }
            #if ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u))
                if(USBUART_DmaNextTd[i] != DMA_INVALID_TD)
                {
                    CyDmaTdFree(USBUART_DmaNextTd[i]);
                    USBUART_DmaNextTd[i] = DMA_INVALID_TD;
                }
            #endif /* ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u)) */
            i++;
        }while((i < USBUART_MAX_EP) && (epNumber == USBUART_MAX_EP));
    }

#endif /*  USBUART_EP_MM != USBUART__EP_MANUAL */


#if ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u))


    /*******************************************************************************
    * Function Name: USBUART_LoadNextInEP
    ********************************************************************************
    *
    * Summary:
    *  This internal function is used for IN endpoint DMA reconfiguration in
    *  Auto DMA mode.
    *
    * Parameters:
    *  epNumber: Contains the data endpoint number.
    *  mode:   0 - Configure DMA to send the the rest of data.
    *          1 - Configure DMA to repeat 2 last bytes of the first burst.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void USBUART_LoadNextInEP(uint8 epNumber, uint8 mode) 
    {
        reg16 *convert;

        if(mode == 0u)
        {
            /* Configure DMA to send the the rest of data */
            /* CyDmaTdSetConfiguration API is optimised to change only transfer length and configure TD */
            convert = (reg16 *) &CY_DMA_TDMEM_STRUCT_PTR[USBUART_DmaTd[epNumber]].TD0[0u];
            /* Set transfer length */
            CY_SET_REG16(convert, USBUART_inLength[epNumber] - USBUART_DMA_BYTES_PER_BURST);
            /* CyDmaTdSetAddress API is optimized to change only source address */
            convert = (reg16 *) &CY_DMA_TDMEM_STRUCT_PTR[USBUART_DmaTd[epNumber]].TD1[0u];
            CY_SET_REG16(convert, LO16((uint32)USBUART_inDataPointer[epNumber] +
                                            USBUART_DMA_BYTES_PER_BURST));
            USBUART_inBufFull[epNumber] = 1u;
        }
        else
        {
            /* Configure DMA to repeat 2 last bytes of the first burst. */
            /* CyDmaTdSetConfiguration API is optimised to change only transfer length and configure TD */
            convert = (reg16 *) &CY_DMA_TDMEM_STRUCT_PTR[USBUART_DmaTd[epNumber]].TD0[0u];
            /* Set transfer length */
            CY_SET_REG16(convert, USBUART_DMA_BYTES_REPEAT);
            /* CyDmaTdSetAddress API is optimized to change only source address */
            convert = (reg16 *) &CY_DMA_TDMEM_STRUCT_PTR[USBUART_DmaTd[epNumber]].TD1[0u];
            CY_SET_REG16(convert,  LO16((uint32)USBUART_inDataPointer[epNumber] +
                                   USBUART_DMA_BYTES_PER_BURST - USBUART_DMA_BYTES_REPEAT));
        }

        /* CyDmaChSetInitialTd API is optimised to init TD */
        CY_DMA_CH_STRUCT_PTR[USBUART_DmaChan[epNumber]].basic_status[1u] = USBUART_DmaTd[epNumber];
    }
#endif /* ((USBUART_EP_MM == USBUART__EP_DMAAUTO) && (USBUART_EP_DMA_AUTO_OPT == 0u)) */


/*******************************************************************************
* Function Name: USBUART_LoadInEP
********************************************************************************
*
* Summary:
*  Loads and enables the specified USB data endpoint for an IN transfer.
*
* Parameters:
*  epNumber: Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  *pData: A pointer to a data array from which the data for the endpoint space
*          is loaded.
*  length: The number of bytes to transfer from the array and then send as a
*          result of an IN request. Valid values are between 0 and 512.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_LoadInEP(uint8 epNumber, const uint8 pData[], uint16 length)
                                                                        
{
    uint8 ri;
    reg8 *p;
    #if(USBUART_EP_MM == USBUART__EP_MANUAL)
        uint16 i;
    #endif /*  USBUART_EP_MM == USBUART__EP_MANUAL */

    if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP))
    {
        ri = ((epNumber - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);
        p = (reg8 *)(USBUART_ARB_RW1_DR_IND + ri);

        #if(USBUART_EP_MM != USBUART__EP_DMAAUTO)
            /* Limits length to available buffer space, auto MM could send packets up to 1024 bytes */
            if(length > (USBUART_EPX_DATA_BUF_MAX - USBUART_EP[epNumber].buffOffset))
            {
                length = USBUART_EPX_DATA_BUF_MAX - USBUART_EP[epNumber].buffOffset;
            }
        #endif /*  USBUART_EP_MM != USBUART__EP_DMAAUTO */

        /* Set the count and data toggle */
        CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT0_IND + ri),
                            (length >> 8u) | (USBUART_EP[epNumber].epToggle));
        CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CNT1_IND + ri),  length & 0xFFu);

        #if(USBUART_EP_MM == USBUART__EP_MANUAL)
            if(pData != NULL)
            {
                /* Copy the data using the arbiter data register */
                for (i = 0u; i < length; i++)
                {
                    CY_SET_REG8(p, pData[i]);
                }
            }
            USBUART_EP[epNumber].apiEpState = USBUART_NO_EVENT_PENDING;
            /* Write the Mode register */
            CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_EP[epNumber].epMode);
        #else
            /* Init DMA if it was not initialized */
            if (USBUART_DmaTd[epNumber] == DMA_INVALID_TD)
            {
                USBUART_InitEP_DMA(epNumber, pData);
            }
        #endif /*  USBUART_EP_MM == USBUART__EP_MANUAL */

        #if(USBUART_EP_MM == USBUART__EP_DMAMANUAL)
            USBUART_EP[epNumber].apiEpState = USBUART_NO_EVENT_PENDING;
            if ((pData != NULL) && (length > 0u))
            {
                /* Enable DMA in mode2 for transferring data */
                (void) CyDmaChDisable(USBUART_DmaChan[epNumber]);
                (void) CyDmaTdSetConfiguration(USBUART_DmaTd[epNumber], length, CY_DMA_DISABLE_TD,
                                                                                 TD_TERMIN_EN | TD_INC_SRC_ADR);
                (void) CyDmaTdSetAddress(USBUART_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
                /* Enable the DMA */
                (void) CyDmaChSetInitialTd(USBUART_DmaChan[epNumber], USBUART_DmaTd[epNumber]);
                (void) CyDmaChEnable(USBUART_DmaChan[epNumber], 1u);
                /* Generate DMA request */
                * (reg8 *)(USBUART_ARB_EP1_CFG_IND + ri) |= USBUART_ARB_EPX_CFG_DMA_REQ;
                * (reg8 *)(USBUART_ARB_EP1_CFG_IND + ri) &= ((uint8)(~USBUART_ARB_EPX_CFG_DMA_REQ));
                /* Mode register will be written in arb ISR after DMA transfer complete */
            }
            else
            {
                /* When zero-length packet - write the Mode register directly */
                CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_EP[epNumber].epMode);
            }
        #endif /*  USBUART_EP_MM == USBUART__EP_DMAMANUAL */

        #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
            if (pData != NULL)
            {
                /* Enable DMA in mode3 for transferring data */
                (void) CyDmaChDisable(USBUART_DmaChan[epNumber]);
            #if (USBUART_EP_DMA_AUTO_OPT == 0u)
                USBUART_inLength[epNumber] = length;
                USBUART_inDataPointer[epNumber] = pData;
                /* Configure DMA to send the data only for the first burst */
                (void) CyDmaTdSetConfiguration(USBUART_DmaTd[epNumber],
                    (length > USBUART_DMA_BYTES_PER_BURST) ? USBUART_DMA_BYTES_PER_BURST : length,
                    USBUART_DmaNextTd[epNumber], TD_TERMIN_EN | TD_INC_SRC_ADR);
                (void) CyDmaTdSetAddress(USBUART_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
                /* The second TD will be executed only when the first one fails.
                *  The intention of this TD is to generate NRQ interrupt
                *  and repeat 2 last bytes of the first burst.
                */
                (void) CyDmaTdSetConfiguration(USBUART_DmaNextTd[epNumber], 1u,
                                               USBUART_DmaNextTd[epNumber],
                                               USBUART_epX_TD_TERMOUT_EN[epNumber]);
                /* Configure DmaNextTd to clear Data ready status */
                (void) CyDmaTdSetAddress(USBUART_DmaNextTd[epNumber],  LO16((uint32)&clearInDataRdyStatus),
                                                                LO16((uint32)(USBUART_ARB_EP1_CFG_IND + ri)));
            #else /* Configure DMA to send all data*/
                (void) CyDmaTdSetConfiguration(USBUART_DmaTd[epNumber], length,
                                               USBUART_DmaTd[epNumber], TD_TERMIN_EN | TD_INC_SRC_ADR);
                (void) CyDmaTdSetAddress(USBUART_DmaTd[epNumber],  LO16((uint32)pData), LO16((uint32)p));
            #endif /* USBUART_EP_DMA_AUTO_OPT == 0u */    

                /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
                (void) CyDmaClearPendingDrq(USBUART_DmaChan[epNumber]);
                /* Enable the DMA */
                (void) CyDmaChSetInitialTd(USBUART_DmaChan[epNumber], USBUART_DmaTd[epNumber]);
                (void) CyDmaChEnable(USBUART_DmaChan[epNumber], 1u);
            }
            else
            {
                USBUART_EP[epNumber].apiEpState = USBUART_NO_EVENT_PENDING;
                if(length > 0u)
                {
                #if (USBUART_EP_DMA_AUTO_OPT == 0u)
                    USBUART_inLength[epNumber] = length;
                    USBUART_inBufFull[epNumber] = 0u;
                    (void) CyDmaChDisable(USBUART_DmaChan[epNumber]);
                    /* Configure DMA to send the data only for the first burst */
                    (void) CyDmaTdSetConfiguration(
                        USBUART_DmaTd[epNumber], (length > USBUART_DMA_BYTES_PER_BURST) ?
                        USBUART_DMA_BYTES_PER_BURST : length,
                        USBUART_DmaNextTd[epNumber], TD_TERMIN_EN | TD_INC_SRC_ADR );
                    (void) CyDmaTdSetAddress(USBUART_DmaTd[epNumber],
                                             LO16((uint32)USBUART_inDataPointer[epNumber]), LO16((uint32)p));
                    /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
                    (void) CyDmaClearPendingDrq(USBUART_DmaChan[epNumber]);
                    /* Enable the DMA */
                    (void) CyDmaChSetInitialTd(USBUART_DmaChan[epNumber], USBUART_DmaTd[epNumber]);
                    (void) CyDmaChEnable(USBUART_DmaChan[epNumber], 1u);
                #endif /* (USBUART_EP_DMA_AUTO_OPT == 0u) */

                    /* Set Data ready status, This will generate DMA request */
                    #ifndef USBUART_MANUAL_IN_EP_ARM
                        * (reg8 *)(USBUART_ARB_EP1_CFG_IND + ri) |= USBUART_ARB_EPX_CFG_IN_DATA_RDY;
                    #endif  /* USBUART_MANUAL_IN_EP_ARM */
                    /* Mode register will be written in arb ISR(In Buffer Full) after first DMA transfer complete */
                }
                else
                {
                    /* When zero-length packet - write the Mode register directly */
                    CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_EP[epNumber].epMode);
                }
            }
        #endif /*  USBUART_EP_MM == USBUART__EP_DMAAUTO */
    }
}


/*******************************************************************************
* Function Name: USBUART_ReadOutEP
********************************************************************************
*
* Summary:
*  Read data from an endpoint.  The application must call
*  USBUART_GetEPState to see if an event is pending.
*
* Parameters:
*  epNumber: Contains the data endpoint number.
*            Valid values are between 1 and 8.
*  pData: A pointer to a data array from which the data for the endpoint space
*         is loaded.
*  length: The number of bytes to transfer from the USB Out endpoint and loads
*          it into data array. Valid values are between 0 and 1023. The function
*          moves fewer than the requested number of bytes if the host sends
*          fewer bytes than requested.
*
* Returns:
*  Number of bytes received, 0 for an invalid endpoint.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint16 USBUART_ReadOutEP(uint8 epNumber, uint8 pData[], uint16 length)
                                                                        
{
    uint8 ri;
    reg8 *p;
    #if(USBUART_EP_MM == USBUART__EP_MANUAL)
        uint16 i;
    #endif /*  USBUART_EP_MM == USBUART__EP_MANUAL */
    #if(USBUART_EP_MM != USBUART__EP_DMAAUTO)
        uint16 xferCount;
    #endif /*  USBUART_EP_MM != USBUART__EP_DMAAUTO */

    if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP) && (pData != NULL))
    {
        ri = ((epNumber - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);
        p = (reg8 *)(USBUART_ARB_RW1_DR_IND + ri);

        #if(USBUART_EP_MM != USBUART__EP_DMAAUTO)
            /* Determine which is smaller the requested data or the available data */
            xferCount = USBUART_GetEPCount(epNumber);
            if (length > xferCount)
            {
                length = xferCount;
            }
        #endif /*  USBUART_EP_MM != USBUART__EP_DMAAUTO */

        #if(USBUART_EP_MM == USBUART__EP_MANUAL)
            /* Copy the data using the arbiter data register */
            for (i = 0u; i < length; i++)
            {
                pData[i] = CY_GET_REG8(p);
            }

            /* (re)arming of OUT endpoint */
            USBUART_EnableOutEP(epNumber);
        #else
            /*Init DMA if it was not initialized */
            if(USBUART_DmaTd[epNumber] == DMA_INVALID_TD)
            {
                USBUART_InitEP_DMA(epNumber, pData);
            }

        #endif /*  USBUART_EP_MM == USBUART__EP_MANUAL */

        #if(USBUART_EP_MM == USBUART__EP_DMAMANUAL)
            /* Enable DMA in mode2 for transferring data */
            (void) CyDmaChDisable(USBUART_DmaChan[epNumber]);
            (void) CyDmaTdSetConfiguration(USBUART_DmaTd[epNumber], length, CY_DMA_DISABLE_TD,
                                                                                TD_TERMIN_EN | TD_INC_DST_ADR);
            (void) CyDmaTdSetAddress(USBUART_DmaTd[epNumber],  LO16((uint32)p), LO16((uint32)pData));
            /* Enable the DMA */
            (void) CyDmaChSetInitialTd(USBUART_DmaChan[epNumber], USBUART_DmaTd[epNumber]);
            (void) CyDmaChEnable(USBUART_DmaChan[epNumber], 1u);

            /* Generate DMA request */
            * (reg8 *)(USBUART_ARB_EP1_CFG_IND + ri) |= USBUART_ARB_EPX_CFG_DMA_REQ;
            * (reg8 *)(USBUART_ARB_EP1_CFG_IND + ri) &= ((uint8)(~USBUART_ARB_EPX_CFG_DMA_REQ));
            /* Out EP will be (re)armed in arb ISR after transfer complete */
        #endif /*  USBUART_EP_MM == USBUART__EP_DMAMANUAL */

        #if(USBUART_EP_MM == USBUART__EP_DMAAUTO)
            /* Enable DMA in mode3 for transferring data */
            (void) CyDmaChDisable(USBUART_DmaChan[epNumber]);
            (void) CyDmaTdSetConfiguration(USBUART_DmaTd[epNumber], length, USBUART_DmaTd[epNumber],
                                                                                TD_TERMIN_EN | TD_INC_DST_ADR);
            (void) CyDmaTdSetAddress(USBUART_DmaTd[epNumber],  LO16((uint32)p), LO16((uint32)pData));

            /* Clear Any potential pending DMA requests before starting the DMA channel to transfer data */
            (void) CyDmaClearPendingDrq(USBUART_DmaChan[epNumber]);
            /* Enable the DMA */
            (void) CyDmaChSetInitialTd(USBUART_DmaChan[epNumber], USBUART_DmaTd[epNumber]);
            (void) CyDmaChEnable(USBUART_DmaChan[epNumber], 1u);
            /* Out EP will be (re)armed in arb ISR after transfer complete */
        #endif /*  USBUART_EP_MM == USBUART__EP_DMAAUTO */

    }
    else
    {
        length = 0u;
    }

    return(length);
}


/*******************************************************************************
* Function Name: USBUART_EnableOutEP
********************************************************************************
*
* Summary:
*  This function enables an OUT endpoint.  It should not be
*  called for an IN endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Return:
*   None.
*
* Global variables:
*  USBUART_EP[epNumber].apiEpState - set to NO_EVENT_PENDING
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_EnableOutEP(uint8 epNumber) 
{
    uint8 ri;

    if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP))
    {
        ri = ((epNumber - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);
        USBUART_EP[epNumber].apiEpState = USBUART_NO_EVENT_PENDING;
        /* Write the Mode register */
        CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_EP[epNumber].epMode);
    }
}


/*******************************************************************************
* Function Name: USBUART_DisableOutEP
********************************************************************************
*
* Summary:
*  This function disables an OUT endpoint.  It should not be
*  called for an IN endpoint.
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_DisableOutEP(uint8 epNumber) 
{
    uint8 ri ;

    if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP))
    {
        ri = ((epNumber - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);
        /* Write the Mode register */
        CY_SET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri), USBUART_MODE_NAK_OUT);
    }
}


/*******************************************************************************
* Function Name: USBUART_Force
********************************************************************************
*
* Summary:
*  Forces the bus state
*
* Parameters:
*  bState
*    USBUART_FORCE_J
*    USBUART_FORCE_K
*    USBUART_FORCE_SE0
*    USBUART_FORCE_NONE
*
* Return:
*  None.
*
*******************************************************************************/
void USBUART_Force(uint8 bState) 
{
    CY_SET_REG8(USBUART_USBIO_CR0_PTR, bState);
}


/*******************************************************************************
* Function Name: USBUART_GetEPAckState
********************************************************************************
*
* Summary:
*  Returns the ACK of the CR0 Register (ACKD)
*
* Parameters:
*  epNumber: Endpoint Number
*            Valid values are between 1 and 8.
*
* Returns
*  0 if nothing has been ACKD, non-=zero something has been ACKD
*
*******************************************************************************/
uint8 USBUART_GetEPAckState(uint8 epNumber) 
{
    uint8 ri;
    uint8 cr = 0u;

    if((epNumber > USBUART_EP0) && (epNumber < USBUART_MAX_EP))
    {
        ri = ((epNumber - USBUART_EP1) << USBUART_EPX_CNTX_ADDR_SHIFT);
        cr = CY_GET_REG8((reg8 *)(USBUART_SIE_EP1_CR0_IND + ri)) & USBUART_MODE_ACKD;
    }

    return(cr);
}


/*******************************************************************************
* Function Name: USBUART_SetPowerStatus
********************************************************************************
*
* Summary:
*  Sets the device power status for reporting in the Get Device Status
*  request
*
* Parameters:
*  powerStatus: USBUART_DEVICE_STATUS_BUS_POWERED(0) - Bus Powered,
*               USBUART_DEVICE_STATUS_SELF_POWERED(1) - Self Powered
*
* Return:
*   None.
*
* Global variables:
*  USBUART_deviceStatus - set power status
*
* Reentrant:
*  No.
*
*******************************************************************************/
void USBUART_SetPowerStatus(uint8 powerStatus) 
{
    if (powerStatus != USBUART_DEVICE_STATUS_BUS_POWERED)
    {
        USBUART_deviceStatus |=  USBUART_DEVICE_STATUS_SELF_POWERED;
    }
    else
    {
        USBUART_deviceStatus &=  ((uint8)(~USBUART_DEVICE_STATUS_SELF_POWERED));
    }
}


#if (USBUART_MON_VBUS == 1u)

    /*******************************************************************************
    * Function Name: USBUART_VBusPresent
    ********************************************************************************
    *
    * Summary:
    *  Determines VBUS presence for Self Powered Devices.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  1 if VBUS is present, otherwise 0.
    *
    *******************************************************************************/
    uint8 USBUART_VBusPresent(void) 
    {
        return((0u != (CY_GET_REG8(USBUART_VBUS_PS_PTR) & USBUART_VBUS_MASK)) ? 1u : 0u);
    }

#endif /* USBUART_MON_VBUS */


/*******************************************************************************
* Function Name: USBUART_RWUEnabled
********************************************************************************
*
* Summary:
*  Returns TRUE if Remote Wake Up is enabled, otherwise FALSE
*
* Parameters:
*   None.
*
* Return:
*  TRUE -  Remote Wake Up Enabled
*  FALSE - Remote Wake Up Disabled
*
* Global variables:
*  USBUART_deviceStatus - checked to determine remote status
*
*******************************************************************************/
uint8 USBUART_RWUEnabled(void) 
{
    uint8 result = USBUART_FALSE;
    if((USBUART_deviceStatus & USBUART_DEVICE_STATUS_REMOTE_WAKEUP) != 0u)
    {
        result = USBUART_TRUE;
    }

    return(result);
}


/* [] END OF FILE */
