/**
 * @file main.c
 * @brief boot from rt-thread
 * 从art-pi的bootloader工程与qboot组件工程借鉴而来的
 * stm32h750 通用片外spi flash bootloader
 * @author RedlightASl (dddbbbdd@foxmail.com)
 * @version 1.0
 * @date 2022-01-23
 * 
 * @copyright Copyright (c) 2022  RedlightASl
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date    <th>Version  <th>Author  <th>Description
 * <tr><td>2022-01-23  <td>1.0      <td>wangh   <td>Content
 * </table>
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_common.h>
#include "w25qxx.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define VECT_TAB_OFFSET      0x00000000UL
#define APPLICATION_ADDRESS  (uint32_t)0x90000000

typedef void (*pFunction)(void);
pFunction JumpToApplication;

int main(void)
{
    rt_kprintf("Ready to Init SPI FLASH!\r\n");
    QSPI_W25Qxx_Init();
    QSPI_W25Qxx_MemoryMappedMode();

    rt_kprintf("Prepare for Jumping!\r\n");

//    SCB_DisableICache();
//    SCB_DisableDCache();
    SysTick->CTRL = 0;

    JumpToApplication = (pFunction)(*(__IO uint32_t *)(APPLICATION_ADDRESS + 4));
    __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);
    rt_kprintf("Jumping to Application!\r\n");
    JumpToApplication();

    return RT_EOK;
}


