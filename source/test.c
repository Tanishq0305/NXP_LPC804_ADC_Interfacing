/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019, 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_adc.h"
#include "fsl_clock.h"
#include "fsl_power.h"

#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_ADC_BASE                  ADC
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER 7U
#define DEMO_ADC_IRQ_ID                ADC_SEQA_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC      ADC_SEQA_IRQHandler
#define DEMO_ADC_CLOCK_SOURCE          kCLOCK_Fro
#define DEMO_ADC_CLOCK_DIVIDER         1U


#define SEG_A_PORT BOARD_INITPINS_segA_PORT
#define SEG_A_PIN  BOARD_INITPINS_segA_PIN

#define SEG_B_PORT BOARD_INITPINS_segB_PORT
#define SEG_B_PIN  BOARD_INITPINS_segB_PIN

#define SEG_C_PORT BOARD_INITPINS_segC_PORT
#define SEG_C_PIN  BOARD_INITPINS_segC_PIN

#define SEG_D_PORT BOARD_INITPINS_segD_PORT
#define SEG_D_PIN  BOARD_INITPINS_segD_PIN

#define SEG_E_PORT BOARD_INITPINS_segE_PORT
#define SEG_E_PIN  BOARD_INITPINS_segE_PIN

#define SEG_F_PORT BOARD_INITPINS_segF_PORT
#define SEG_F_PIN  BOARD_INITPINS_segF_PIN

#define SEG_G_PORT BOARD_INITPINS_segG_PORT
#define SEG_G_PIN  BOARD_INITPINS_segG_PIN

#define DIG_1_PORT BOARD_INITPINS_Dig1_PORT
#define DIG_1_PIN  BOARD_INITPINS_Dig1_PIN
#define DIG_1_MASK BOARD_INITPINS_Dig1_GPIO_PIN_MASK

#define DIG_2_PORT BOARD_INITPINS_Dig2_PORT
#define DIG_2_PIN  BOARD_INITPINS_Dig2_PIN
#define DIG_2_MASK BOARD_INITPINS_Dig2_GPIO_PIN_MASK

#define DIG_3_PORT BOARD_INITPINS_Dig3_PORT
#define DIG_3_PIN  BOARD_INITPINS_Dig3_PIN
#define DIG_3_MASK BOARD_INITPINS_Dig3_GPIO_PIN_MASK

#define DIG_4_PORT BOARD_INITPINS_Dig4_PORT
#define DIG_4_PIN  BOARD_INITPINS_Dig4_PIN
#define DIG_4_MASK BOARD_INITPINS_Dig4_GPIO_PIN_MASK






static adc_result_info_t gAdcResultInfoStruct;
adc_result_info_t *volatile gAdcResultInfoPtr = &gAdcResultInfoStruct;
volatile bool gAdcConvSeqAIntFlag;
const uint32_t g_Adc_12bitFullRange = 4096U;







volatile uint32_t g_systickCounter;

/***
 * Code
 ****/
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}




//Display Function for 7-segment LED
void display(int num){

    //Displaying 1
	if(num == 1){
		GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 0u << SEG_A_PIN);
		GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 0u << SEG_D_PIN);
		GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 0u << SEG_E_PIN);
		GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 0u << SEG_F_PIN);
		GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 0u << SEG_G_PIN);

//		SysTick_DelayTicks(1000U);
	}

    // Displaying-2
	else if(num==2){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
	//        GPIO_PortMaskedWrite(GPIO, SEG_D_PORT, 1u <<SEG_D_PIN);
//	    SysTick_DelayTicks(1000U);
	}


    //Displaying-3
	else if(num==3){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
//	    SysTick_DelayTicks(1000U);
	}


    //Displaying-4
	else if(num==4){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
//	    SysTick_DelayTicks(1000U);
	}

    //Displaying-5
	else if(num ==5){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
//	    SysTick_DelayTicks(1000U);
	}

    //Displaying-6
	else if(num==6){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
//	    SysTick_DelayTicks(1000U);

	}

    //Displaying-7
	else if(num==7){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 0);
//	    SysTick_DelayTicks(1000U);
	}

    //Displaying-8
	else if(num == 8){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
//	    SysTick_DelayTicks(1000U);
	}


    //Displaying-6
	else if(num == 9){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 1);
//	    SysTick_DelayTicks(1000U);
	}

    //Displaying-0
	else if (num == 0){
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 0);
//	    SysTick_DelayTicks(1000U);
	}
	else{
	    GPIO_PinWrite(GPIO, SEG_A_PORT, SEG_A_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_B_PORT, SEG_B_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_C_PORT, SEG_C_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_D_PORT, SEG_D_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_E_PORT, SEG_E_PIN, 1);
	    GPIO_PinWrite(GPIO, SEG_F_PORT, SEG_F_PIN, 0);
	    GPIO_PinWrite(GPIO, SEG_G_PORT, SEG_G_PIN, 0);
//	    SysTick_DelayTicks(1000U);
	}


}










/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void ADC_Configuration(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    /* Attach 12 MHz clock to USART0 (debug console) */
    CLOCK_Select(BOARD_DEBUG_USART_CLK_ATTACH);

   GPIO_PortInit(GPIO, SEG_A_PORT);
   GPIO_PortInit(GPIO, SEG_B_PORT );
   GPIO_PortInit(GPIO, SEG_C_PORT );
   GPIO_PortInit(GPIO, SEG_D_PORT );
   GPIO_PortInit(GPIO, SEG_E_PORT);
   GPIO_PortInit(GPIO, SEG_F_PORT);
   GPIO_PortInit(GPIO,SEG_G_PORT );
   GPIO_PortInit(GPIO, DIG_1_PORT );
   GPIO_PortInit(GPIO, DIG_2_PORT  );

    /* Board pin init */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Attach FRO clock to ADC0. */
    CLOCK_Select(kADC_Clk_From_Fro);

    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1U);
    /* Power on ADC. */
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    PRINTF("ADC interrupt example.\r\n");
    if (SysTick_Config(SystemCoreClock / 1000U))
       {
           while (1)
           {
           }
       }

#if !(defined(FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC) && FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC)
    uint32_t frequency = 0U;
    /* Calibration after power up. */
#if defined(FSL_FEATURE_ADC_HAS_CALIB_REG) && FSL_FEATURE_ADC_HAS_CALIB_REG
    DEMO_ADC_BASE->CTRL |= ADC_CTRL_BYPASSCAL_MASK;
    frequency = CLOCK_GetFreq(kCLOCK_BusClk);
    if (true == ADC_DoOffsetCalibration(DEMO_ADC_BASE, frequency))
#else
#if defined(SYSCON_ADCCLKDIV_DIV_MASK)
    frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
#else
    frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE);
#endif /* SYSCON_ADCCLKDIV_DIV_MASK */
    if (true == ADC_DoSelfCalibration(DEMO_ADC_BASE, frequency))
#endif /* FSL_FEATURE_ADC_HAS_CALIB_REG */
    {
        PRINTF("ADC Calibration Done.\r\n");
    }
    else
    {
        PRINTF("ADC Calibration Failed.\r\n");
    }
#if defined(FSL_FEATURE_ADC_CALIBRATION_CLOCK_LOWER_THAN_30MHz) && FSL_FEATURE_ADC_CALIBRATION_CLOCK_LOWER_THAN_30MHz
    ReInitSystemclock();
#endif
#endif /* FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC */

    /* Configure the ADC as basic polling mode. */
    ADC_Configuration();

    /* Enable the interrupt. */
    /* Enable the interrupt the for sequence A done. */
    ADC_EnableInterrupts(DEMO_ADC_BASE, kADC_ConvSeqAInterruptEnable);
    NVIC_EnableIRQ(DEMO_ADC_IRQ_ID);

    PRINTF("Configuration Done.\r\n");

#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    PRINTF("ADC Full Range: %d\r\n", g_Adc_12bitFullRange);
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL */
    while (1)
    {
        GETCHAR();
        gAdcConvSeqAIntFlag = false;
        ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);

        while (!gAdcConvSeqAIntFlag)
        {
        }
        PRINTF("gAdcResultInfoStruct.result        = %d\r\n", gAdcResultInfoStruct.result);
        PRINTF("gAdcResultInfoStruct.channelNumber = %d\r\n", gAdcResultInfoStruct.channelNumber);
        PRINTF("gAdcResultInfoStruct.overrunFlag   = %d\r\n", gAdcResultInfoStruct.overrunFlag ? 1U : 0U);
        PRINTF("\r\n");


        //Below is the code for splitting every digit from the number
        int num = gAdcResultInfoStruct.result;
        int num1[4]={0,0,0,0};

           int i=0;
           while(num > 0) //do till num greater than  0
           {
               int mod = num % 10;  //split last digit from number

               num1[i]=mod;
               i++;


               num = num / 10;    //divide num by 10. num /= 10 also a valid one
           }



           //A loop for displaying the readings on LED Display for 2-3 Seconds
           int count =0;
           while(count<100){

           GPIO_PortSet(GPIO, DIG_1_PORT,DIG_1_MASK);
           GPIO_PortSet(GPIO, DIG_2_PORT, DIG_2_MASK);
           GPIO_PortSet(GPIO, DIG_3_PORT, DIG_3_MASK);
           GPIO_PortSet(GPIO, DIG_4_PORT,DIG_4_MASK);

           GPIO_PinWrite(GPIO, DIG_1_PORT, DIG_1_PIN, 0u << DIG_1_PIN );
           display(num1[3]);
           SysTick_DelayTicks(5U);
           GPIO_PortSet(GPIO,DIG_1_PORT, DIG_1_MASK);

           GPIO_PinWrite(GPIO, DIG_2_PORT, DIG_2_PIN, 0u << DIG_2_PIN );
           display(num1[2]);
           SysTick_DelayTicks(5U);
           GPIO_PortSet(GPIO,DIG_2_PORT, DIG_2_MASK);

           GPIO_PinWrite(GPIO, DIG_3_PORT, DIG_3_PIN, 0u << DIG_3_PIN );
           display(num1[1]);
           SysTick_DelayTicks(5U);
           GPIO_PortSet(GPIO,DIG_3_PORT, DIG_3_MASK);

           GPIO_PinWrite(GPIO, DIG_4_PORT, DIG_4_PIN, 0u << DIG_4_PIN );
           display(num1[0]);
           SysTick_DelayTicks(5U);
           GPIO_PortSet(GPIO,DIG_4_PORT, DIG_4_MASK);

           count++;
           }

    }





}

/*
 * ISR for ADC conversion sequence A done.
 */
void DEMO_ADC_IRQ_HANDLER_FUNC(void)
{
    if (kADC_ConvSeqAInterruptFlag == (kADC_ConvSeqAInterruptFlag & ADC_GetStatusFlags(DEMO_ADC_BASE)))
    {
        ADC_GetChannelConversionResult(DEMO_ADC_BASE, DEMO_ADC_SAMPLE_CHANNEL_NUMBER, gAdcResultInfoPtr);
        ADC_ClearStatusFlags(DEMO_ADC_BASE, kADC_ConvSeqAInterruptFlag);
        gAdcConvSeqAIntFlag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}

/*
 * Configure the ADC as normal converter in polling mode.
 */
void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;

/* Configure the converter. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) & FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE
    adcConfigStruct.clockMode = kADC_ClockSynchronousMode; /* Using sync clock source. */
#endif                                                     /* FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE */
    adcConfigStruct.clockDividerNumber = DEMO_ADC_CLOCK_DIVIDER;
#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    adcConfigStruct.resolution = kADC_Resolution12bit;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL) & FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL
    adcConfigStruct.enableBypassCalibration = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_CTRL_TSAMP
    adcConfigStruct.sampleTimeNumber = 0U;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_TSAMP */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE) & FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE
    adcConfigStruct.enableLowPowerMode = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE */
#if defined(FSL_FEATURE_ADC_HAS_TRIM_REG) & FSL_FEATURE_ADC_HAS_TRIM_REG
    adcConfigStruct.voltageRange = kADC_HighVoltageRange;
#endif /* FSL_FEATURE_ADC_HAS_TRIM_REG */
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

#if !(defined(FSL_FEATURE_ADC_HAS_NO_INSEL) && FSL_FEATURE_ADC_HAS_NO_INSEL)
    /* Use the temperature sensor input to channel 0. */
    ADC_EnableTemperatureSensor(DEMO_ADC_BASE, true);
#endif /* FSL_FEATURE_ADC_HAS_NO_INSEL. */

    /* Enable channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER's conversion in Sequence A. */
#if defined(FSL_FEATURE_ADC_HAS_SEQ_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_SEQ_CTRL_TSAMP
    adcConvSeqConfigStruct.seqSampleTimeNumber = 0U;
#endif /* FSL_FEATURE_ADC_HAS_SEQ_CTRL_TSAMP */
    adcConvSeqConfigStruct.channelMask =
        (1U << DEMO_ADC_SAMPLE_CHANNEL_NUMBER); /* Includes channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER. */
    adcConvSeqConfigStruct.triggerMask      = 0U;
    adcConvSeqConfigStruct.triggerPolarity  = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode    = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(DEMO_ADC_BASE, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(DEMO_ADC_BASE, true); /* Enable the conversion sequence A. */
    /* Clear the result register. */
    ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);
    while (!ADC_GetChannelConversionResult(DEMO_ADC_BASE, DEMO_ADC_SAMPLE_CHANNEL_NUMBER, &gAdcResultInfoStruct))
    {
    }
    ADC_GetConvSeqAGlobalConversionResult(DEMO_ADC_BASE, &gAdcResultInfoStruct);
}
