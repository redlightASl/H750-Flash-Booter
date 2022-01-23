#include <board.h>
#include <rtthread.h>
#include "drv_common.h"
#define DBG_TAG "board"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

void system_clock_config(int target_freq_mhz)
{
//    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//    /** Supply configuration update enable
//    */
//    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
//    /** Configure the main internal regulator output voltage
//    */
//    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
//
//    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
//    /** Initializes the CPU, AHB and APB busses clocks
//    */
//    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
//    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//    RCC_OscInitStruct.PLL.PLLM = 4;
//    RCC_OscInitStruct.PLL.PLLN = 60;
//    RCC_OscInitStruct.PLL.PLLP = 2;
//    RCC_OscInitStruct.PLL.PLLQ = 2;
//    RCC_OscInitStruct.PLL.PLLR = 2;
//    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
//    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
//    RCC_OscInitStruct.PLL.PLLFRACN = 0;
//    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//    {
//        Error_Handler();
//    }
//    /** Initializes the CPU, AHB and APB busses clocks
//    */
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
//                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
//    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
//    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
//
//    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//    {
//        Error_Handler();
//    }


    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
      RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

      /** Supply configuration update enable
      */
      HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
      /** Configure the main internal regulator output voltage
      */
      __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

      while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
      /** Initializes the RCC Oscillators according to the specified parameters
      * in the RCC_OscInitTypeDef structure.
      */
      RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
      RCC_OscInitStruct.HSEState = RCC_HSE_ON;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
      RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
      RCC_OscInitStruct.PLL.PLLM = 5;
      RCC_OscInitStruct.PLL.PLLN = 192;
      RCC_OscInitStruct.PLL.PLLP = 2;
      RCC_OscInitStruct.PLL.PLLQ = 2;
      RCC_OscInitStruct.PLL.PLLR = 2;
      RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
      RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
      RCC_OscInitStruct.PLL.PLLFRACN = 0;
      if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      {
        Error_Handler();
      }
      /** Initializes the CPU, AHB and APB buses clocks
      */
      RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                  |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
      RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
      RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
      RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
      RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
      RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
      RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
      RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

      if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
      {
        Error_Handler();
      }
}
int clock_information(void)
{
    LOG_D("System Clock information");
    LOG_D("SYSCLK_Frequency = %d", HAL_RCC_GetSysClockFreq());
    LOG_D("HCLK_Frequency   = %d", HAL_RCC_GetHCLKFreq());
    LOG_D("PCLK1_Frequency  = %d", HAL_RCC_GetPCLK1Freq());
    LOG_D("PCLK2_Frequency  = %d", HAL_RCC_GetPCLK2Freq());

    return RT_EOK;
}
INIT_BOARD_EXPORT(clock_information);

void clk_init(char *clk_source, int source_freq, int target_freq)
{
    /*
     * Use SystemClock_Config generated from STM32CubeMX for clock init
     * system_clock_config(target_freq);
     */
    extern void SystemClock_Config(void);
    SystemClock_Config();
}
