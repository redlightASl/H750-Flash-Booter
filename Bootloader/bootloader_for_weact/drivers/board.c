/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-21     RealThread   first version
 */

#include <rtthread.h>
#include <board.h>
#include <drv_common.h>

RT_WEAK void rt_hw_board_init()
{
    extern void hw_board_init(char *clock_src, int32_t clock_src_freq, int32_t clock_target_freq);

    /* Heap initialization */
#if defined(RT_USING_HEAP)
    rt_system_heap_init((void *) HEAP_BEGIN, (void *) HEAP_END);
#endif

    hw_board_init(BSP_CLOCK_SOURCE, BSP_CLOCK_SOURCE_FREQ_MHZ, BSP_CLOCK_SYSTEM_FREQ_MHZ);

    /* Set the shell console output device */
#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

}

/**
* @brief QSPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hqspi: QSPI handle pointer
* @retval None
*/
//void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//  if(hqspi->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspInit 0 */
//
//  /* USER CODE END QUADSPI_MspInit 0 */
//  /** Initializes the peripherals clock
//  */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
//    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Peripheral clock enable */
//    __HAL_RCC_QSPI_CLK_ENABLE();
//
//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_GPIOD_CLK_ENABLE();
//    /**QUADSPI GPIO Configuration
//    PE2     ------> QUADSPI_BK1_IO2
//    PB2     ------> QUADSPI_CLK
//    PD11     ------> QUADSPI_BK1_IO0
//    PD12     ------> QUADSPI_BK1_IO1
//    PD13     ------> QUADSPI_BK1_IO3
//    PB6     ------> QUADSPI_BK1_NCS
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_6;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//    HAL_I2CEx_EnableFastModePlus(SYSCFG_PMCR_I2C_PB6_FMP);
//
//  /* USER CODE BEGIN QUADSPI_MspInit 1 */
//
//  /* USER CODE END QUADSPI_MspInit 1 */
//  }
//
//}

