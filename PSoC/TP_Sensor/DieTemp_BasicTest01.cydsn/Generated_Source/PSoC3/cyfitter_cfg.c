/*******************************************************************************
* FILENAME: cyfitter_cfg.c
* PSoC Creator 3.0 SP2
*
* Description:
* This file is automatically generated by PSoC Creator with device 
* initialization code.  Except for the user defined sections in
* CyClockStartupError(), this file should not be modified.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include <cytypes.h>
#include <cydevice_trm.h>
#include <cyfitter.h>
#include <CyLib.h>
#include <cyfitter_cfg.h>

#define CY_NEED_CYCLOCKSTARTUPERROR 1


#if defined(__C51__) || defined(__CX51__)
	#define CYPACKED
	#define CYPACKED_ATTR
	#define CYALIGNED
	#define DMAINITSEC
    #define CY_CFG_UNUSED
	#define CY_CFG_MEMORY_BARRIER() do { } while (0)



	#define CYMEMZERO(a,c) cymemzero((a),(c))
	#define CYCONFIGCPY(d,s,c) cyconfigcpy((c),(s),(d))
	#define CYCONFIGCPYCODE(d,s,c) cyconfigcpycode((c),(s),(d))
#else
	#error Unsupported toolchain
#endif



/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u

#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* `#START CyClockStartupError` */

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

    /* `#END` */

    /* If nothing else, stop here since the clocks have not started         */
    /* correctly.                                                           */
    while(1) {}
}
#endif

/* IOPINS0_2 Address: CYREG_PRT2_DM0 Size (bytes): 8 */
#define BS_IOPINS0_2_VAL ((const uint8 CYFAR *)0x00080000u)


/*******************************************************************************
* Function Name: cfg_dma_init
********************************************************************************
* Summary:
*  This function is used to copy device configuration data from constant values
*  stored in memory into the proper chip configuration registers using DMA.  It
*  pulls data out of the CFG_TD_LISTs to know exactly how/what to configure.
*
* Parameters:  
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_dma_init(void);
static void cfg_dma_init(void)
{
	/* TD structure for big-endian systems. Requires 2-byte endian swap */
	CYPACKED struct td_t {
		uint16 size;
		uint8 flags;
		uint8 next_td_ptr;
		uint16 src;
		uint16 dest;
	} CYPACKED_ATTR;

	/* PHUB_CH1_CFGMEM_CFG0/1 initialization data */
	DMAINITSEC static const uint8 CYCODE CH1_ADDR_DATA0[] = { 0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u };

	/* PHUB_CH2_CFGMEM_CFG0/1 initialization data */
	DMAINITSEC static const uint8 CYCODE CH2_ADDR_DATA0[] = { 0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x01u, 0x00u };

	/* Initialization data for each DMA channel's TD registers */
	DMAINITSEC static const uint8 CYCODE DMA_TD_START_CMD[] = { 0x01u, 0x00u, 0x00u, 0x00u, 0x01u };

	CYALIGNED static uint32 CYXDATA DMA_ZERO_VAL;

	/* This table holds all of the initialization data necessary for the DMA
	   process to transfer data from the source location to the destination. */
	DMAINITSEC static const struct td_t CYCODE CFG_TD_LIST0[] = {
		/* Size, Flags, Next, Source, Destination */
		{ 8u, 0x23u, 0x02u, (uint16)(uint32)(CH1_ADDR_DATA0), (uint16)(CYREG_PHUB_CFGMEM1_CFG0) },	/* TD 1 */
		{ 8u, 0x23u, 0x03u, (uint16)(uint32)(CH2_ADDR_DATA0), (uint16)(CYREG_PHUB_CFGMEM2_CFG0) },	/* TD 2 */
		{ 5u, 0x23u, 0x04u, (uint16)(uint32)(DMA_TD_START_CMD), (uint16)(CYREG_PHUB_CH1_BASIC_CFG) },	/* TD 3 */
		{ 5u, 0x23u, 0xFEu, (uint16)(uint32)(DMA_TD_START_CMD), (uint16)(CYREG_PHUB_CH2_BASIC_CFG) },	/* TD 4 */
		{ 32u, 0x22u, 0x06u, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYREG_PRT0_DR) },	/* TD 5 */
		{ 64u, 0x22u, 0x07u, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYREG_PRT3_DR) },	/* TD 6 */
		{ 16u, 0x22u, 0x08u, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYREG_PRT12_DR) },	/* TD 7 */
		{ 16u, 0x22u, 0xFEu, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYREG_PRT15_DR) },	/* TD 8 */
		{ 4095u, 0x22u, 0x0Au, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYDEV_UCFG_B0_P0_U0_BASE) },	/* TD 9 */
		{ 1u, 0x22u, 0x0Bu, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYDEV_UCFG_B0_P7_ROUTE_BASE + 0x00000080u) },	/* TD 10 */
		{ 2048u, 0x22u, 0x0Cu, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYDEV_UCFG_B1_P2_U0_BASE) },	/* TD 11 */
		{ 2560u, 0x22u, 0x0Du, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYDEV_UCFG_DSI0_BASE) },	/* TD 12 */
		{ 512u, 0x22u, 0x0Eu, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYDEV_UCFG_DSI12_BASE) },	/* TD 13 */
		{ 32u, 0x22u, 0xFEu, (uint16)(uint32)(&DMA_ZERO_VAL), (uint16)(CYREG_BCTL0_MDCLK_EN) },	/* TD 14 */
	};

	DMA_ZERO_VAL = 0u;
	CY_SET_REG8((void CYXDATA *)CYREG_PHUB_CFGMEM0_CFG0, 0x10u);	/* set burstcnt */
	CY_SET_REG8((void CYXDATA *)CYREG_PHUB_CH0_BASIC_CFG, 0x01u);	/* enable ch0 */
	CY_SET_REG8((void CYXDATA *)(CYREG_PHUB_CH0_BASIC_STATUS+1u), 0x00u);	/* set first TD to 0 */
	CY_SET_REG16((void CYXDATA *)CYREG_PHUB_TDMEM0_ORIG_TD0, 14u*8u);	/* transfer size */
	CY_SET_REG8((void CYXDATA *)(CYREG_PHUB_TDMEM0_ORIG_TD0+2u), 0x01u);	/* set next TD to 1 */
	CY_SET_REG8((void CYXDATA *)(CYREG_PHUB_TDMEM0_ORIG_TD0+3u), 0xA3u);	/* set TD flags */
	CY_SET_REG16((void CYXDATA *)CYREG_PHUB_TDMEM0_ORIG_TD1, (uint16)((uint32)CFG_TD_LIST0 & 0x0000FFFFu));	/* set td0's src addr */
	CY_SET_REG16((void CYXDATA *)(CYREG_PHUB_TDMEM0_ORIG_TD1+2u), CYDEV_PHUB_TDMEM1_BASE);	/* set td0's dst addr */
	CY_SET_REG16((void CYXDATA *)CYREG_PHUB_CFGMEM0_CFG1, (uint16)(CYDEV_FLS_BASE >> 16));	/* set ch0's src high addr */
	CY_SET_REG16((void CYXDATA *)(CYREG_PHUB_CFGMEM0_CFG1+2u), (uint16)(CYDEV_PHUB_TDMEM1_BASE >> 16));	/* set ch0's dst high addr */
	CY_SET_REG8((void CYXDATA *)(CYREG_PHUB_CH1_BASIC_STATUS+1u), 0x05u);	/* ch1 first TD: 5 */
	CY_SET_REG8((void CYXDATA *)(CYREG_PHUB_CH2_BASIC_STATUS+1u), 0x09u);	/* ch2 first TD: 9 */

	CY_CFG_MEMORY_BARRIER();
	CY_SET_REG8((void CYXDATA *)CYREG_PHUB_CH0_ACTION, 0x01u); /* cpu_req */
	CY_CFG_MEMORY_BARRIER();

	/* Wait for all DMA transfers to complete */
	while(0u != (CY_GET_REG8((void CYXDATA *)CYREG_PHUB_CH0_BASIC_STATUS) & 0x01u)){}
	while(0u != (CY_GET_REG8((void CYXDATA *)CYREG_PHUB_CH1_BASIC_STATUS) & 0x01u)){}
	while(0u != (CY_GET_REG8((void CYXDATA *)CYREG_PHUB_CH2_BASIC_STATUS) & 0x01u)){}


}
/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*  Performs the initialization of all of the clocks in the device based on the
*  settings in the Clock tab of the DWR.  This includes enabling the requested
*  clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void ClockSetup(void);
static void ClockSetup(void)
{
	uint32 timeout;
	uint8 pllLock;


	/* Configure ILO based on settings from Clock DWR */
	CY_SET_REG8((void CYXDATA *)(CYREG_SLOWCLK_ILO_CR0), 0x02u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_CR), 0x08u);

	/* Configure IMO based on settings from Clock DWR */
	CY_SET_REG8((void CYXDATA *)(CYREG_FASTCLK_IMO_CR), 0x03u);
	CY_SET_REG8((void CYXDATA *)(CYREG_IMO_TR1), (CY_GET_XTND_REG8((void CYFAR *)CYREG_FLSHID_CUST_TABLES_IMO_3MHZ)));

	/* Configure PLL based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_FASTCLK_PLL_P), 0x0008u);
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_FASTCLK_PLL_CFG0), 0x1251u);
	/* Wait up to 250us for the PLL to lock */
	pllLock = 0u;
	for (timeout = 250u / 10u; (timeout > 0u) && (pllLock != 0x03u); timeout--)
	{ 
		pllLock = 0x03u & ((uint8)((uint8)pllLock << 1) | ((CY_GET_REG8((void CYXDATA *)CYREG_FASTCLK_PLL_SR) & 0x01u) >> 0));
		CyDelayCycles(10u * 48u); /* Delay 10us based on 48MHz clock */
	}
	/* If we ran out of time the PLL didn't lock so go to the error function */
	if (timeout == 0u)
	{
		CyClockStartupError(CYCLOCKSTART_PLL_ERROR);
	}

	/* Configure Bus/Master Clock based on settings from Clock DWR */
	CY_SET_XTND_REG16((void CYFAR *)(CYREG_CLKDIST_MSTR0), 0x0100u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_MSTR0), 0x07u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_BCFG0), 0x00u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_BCFG2), 0x48u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_MSTR0), 0x00u);
	CY_SET_REG8((void CYXDATA *)(CYREG_CLKDIST_LD), 0x02u);
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	uint8 bg_xover_inl_trim = CY_GET_XTND_REG8((void CYFAR *)(CYREG_FLSHID_MFG_CFG_BG_XOVER_INL_TRIM + 1u));
	CY_SET_REG8((void CYXDATA *)(CYREG_BG_DFT0), (bg_xover_inl_trim & 0x07u));
	CY_SET_REG8((void CYXDATA *)(CYREG_BG_DFT1), ((bg_xover_inl_trim >> 4) & 0x0Fu));
	CY_SET_REG8((void CYXDATA *)CYREG_PUMP_CR0, 0x44u);
}


/*******************************************************************************
* Function Name: SetAnalogRoutingPumps
********************************************************************************
*
* Summary:
* Enables or disables the analog pumps feeding analog routing switches.
* Intended to be called at startup, based on the Vdda system configuration;
* may be called during operation when the user informs us that the Vdda voltage
* crossed the pump threshold.
*
* Parameters:
*  enabled - 1 to enable the pumps, 0 to disable the pumps
*
* Return:
*  void
*
*******************************************************************************/
void SetAnalogRoutingPumps(uint8 enabled)
{
	uint8 regValue = CY_GET_REG8((void CYXDATA *)CYREG_PUMP_CR0);
	if (enabled != 0u)
	{
		regValue |= 0x00u;
	}
	else
	{
		regValue &= (uint8)~0x00u;
	}
	CY_SET_REG8((void CYXDATA *)CYREG_PUMP_CR0, regValue);
}

#define CY_AMUX_UNUSED CYREG_BOOST_SR


/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:  
*   void
*
* Return:
*   void
*
*******************************************************************************/

void cyfitter_cfg(void)
{
	/* Enable/Disable Debug functionality based on settings from System DWR */
	CY_SET_XTND_REG8((void CYFAR *)CYREG_MLOGIC_DEBUG, (CY_GET_XTND_REG8((void CYFAR *)CYREG_MLOGIC_DEBUG) | 0x05u));

	{
		cfg_dma_init();

		/* Enable digital routing */
		CY_SET_XTND_REG8((void CYFAR *)CYREG_BCTL0_BANK_CTL, CY_GET_XTND_REG8((void CYFAR *)CYREG_BCTL0_BANK_CTL) | 0x02u);
		CY_SET_XTND_REG8((void CYFAR *)CYREG_BCTL1_BANK_CTL, CY_GET_XTND_REG8((void CYFAR *)CYREG_BCTL1_BANK_CTL) | 0x02u);

		/* Enable UDB array */
		CY_SET_REG8((void CYXDATA *)CYREG_PM_ACT_CFG0, CY_GET_REG8((void CYXDATA *)CYREG_PM_ACT_CFG0) | 0x40u);
		CY_SET_REG8((void CYXDATA *)CYREG_PM_AVAIL_CR2, CY_GET_REG8((void CYXDATA *)CYREG_PM_AVAIL_CR2) | 0x10u);
	}

	/* Perform second pass device configuration. These items must be configured in specific order after the regular configuration is done. */
	CYCONFIGCPY((void CYFAR *)(CYREG_PRT2_DM0), (const void CYFAR *)(BS_IOPINS0_2_VAL), 8u);

	/* Switch Boost to the precision bandgap reference from its internal reference */
	CY_SET_REG8((void CYXDATA *)CYREG_BOOST_CR2, (CY_GET_REG8((void CYXDATA *)CYREG_BOOST_CR2) | 0x08u));
	if (CYDEV_CHIP_REV_ACTUAL < 5u)
	{
		CY_SET_REG8((void CYXDATA *)CYREG_PWRSYS_WAKE_TR2, 0x3Au);
	}


	/* Set Flash Cycles based on max possible frequency in case a glitch occurs during ClockSetup(). */
	CY_SET_REG8((void CYXDATA *)(CYREG_CACHE_CR), (((CYDEV_INSTRUCT_CACHE_ENABLED) != 0) ? 0xF1u : 0xC0u));
	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();
	/* Set Flash Cycles based on newly configured 24.00MHz Bus Clock. */
	CY_SET_REG8((void CYXDATA *)(CYREG_CACHE_CR), (((CYDEV_INSTRUCT_CACHE_ENABLED) != 0) ? 0xB1u : 0x80u));

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

	/* Configure alternate active mode */
	CYCONFIGCPY((void CYFAR *)CYDEV_PM_STBY_BASE, (const void CYFAR *)CYDEV_PM_ACT_BASE, 14u);
	CY_SET_REG8((void CYXDATA *)CYREG_PM_STBY_CFG0, CY_GET_REG8((void CYXDATA *)CYREG_PM_STBY_CFG0) & (uint8)~0x02u);	/* Disable CPU */
}
