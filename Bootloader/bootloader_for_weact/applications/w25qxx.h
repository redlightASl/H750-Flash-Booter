/**
 * @file w25qxx.h
 * @brief boot from rt-thread w25qxx driver
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
#ifndef QSPI_w25q64_H
#define QSPI_w25q64_H

#include "stm32h7xx_hal.h"
#include "board.h"

 /* Chip Info */
#define QSPI_W25Qxx_OK                       0          // W25Qxx通信正常
#define W25Qxx_ERROR_INIT                   -1          // 初始化错误
#define W25Qxx_ERROR_WriteEnable            -2          // 写使能错误
#define W25Qxx_ERROR_AUTOPOLLING            -3          // 轮询等待错误，无响应
#define W25Qxx_ERROR_Erase                  -4          // 擦除错误
#define W25Qxx_ERROR_TRANSMIT               -5          // 传输错误
#define W25Qxx_ERROR_MemoryMapped           -6          // 内存映射模式错误

#define W25Qxx_CMD_EnableReset              0x66        // 使能复位
#define W25Qxx_CMD_ResetDevice              0x99        // 复位器件
#define W25Qxx_CMD_JedecID                  0x9F        // JEDEC ID
#define W25Qxx_CMD_WriteEnable              0X06        // 写使能

#define W25Qxx_CMD_SectorErase              0x20        // 扇区擦除，4K字节， 参考擦除时间 45ms
#define W25Qxx_CMD_BlockErase_32K           0x52        // 块擦除，  32K字节，参考擦除时间 120ms
#define W25Qxx_CMD_BlockErase_64K           0xD8        // 块擦除，  64K字节，参考擦除时间 150ms
#define W25Qxx_CMD_ChipErase                0xC7        // 整片擦除，参考擦除时间 20S

#define W25Qxx_CMD_QuadInputPageProgram     0x32        // 1-1-4模式下(1线指令1线地址4线数据)，页编程指令，参考写入时间 0.4ms
#define W25Qxx_CMD_FastReadQuad_IO          0xEB        // 1-4-4模式下(1线指令4线地址4线数据)，快速读取指令

#define W25Qxx_CMD_ReadStatus_REG1          0X05        // 读状态寄存器1
#define W25Qxx_Status_REG1_BUSY             0x01        // 读状态寄存器1的第0位（只读），Busy标志位，当正在擦除/写入数据/写命令时会被置1
#define W25Qxx_Status_REG1_WEL              0x02        // 读状态寄存器1的第1位（只读），WEL写使能标志位，该标志位为1时，代表可以进行写操作

#define W25Qxx_PageSize                     256         // 页大小，256字节
#define W25Qxx_FlashSize                    0x800000    // W25Q64大小，8M字节
#define W25Qxx_FLASH_ID                     0xEF4017    // W25Q64 JEDEC ID
#define W25Qxx_ChipErase_TIMEOUT_MAX        100000U     // 超时等待时间，W25Q64整片擦除所需最大时间是100S
#define W25Qxx_Mem_Addr                     0x90000000  // 内存映射模式的地址

/* GPIO FF Configuration */
#define  QUADSPI_CLK_PIN                            GPIO_PIN_2                              // QUADSPI_CLK 引脚
#define QUADSPI_CLK_PORT                            GPIOB                                   // QUADSPI_CLK 引脚端口
#define QUADSPI_CLK_AF                              GPIO_AF9_QUADSPI                        // QUADSPI_CLK IO口复用
#define     GPIO_QUADSPI_CLK_ENABLE                 __HAL_RCC_GPIOB_CLK_ENABLE()            // QUADSPI_CLK 引脚时钟

#define  QUADSPI_BK1_NCS_PIN                        GPIO_PIN_6                              // QUADSPI_BK1_NCS 引脚
#define QUADSPI_BK1_NCS_PORT                        GPIOB                                   // QUADSPI_BK1_NCS 引脚端口
#define QUADSPI_BK1_NCS_AF                          GPIO_AF10_QUADSPI                       // QUADSPI_BK1_NCS IO口复用
#define     GPIO_QUADSPI_BK1_NCS_ENABLE             __HAL_RCC_GPIOB_CLK_ENABLE()            // QUADSPI_BK1_NCS 引脚时钟

#define  QUADSPI_BK1_IO0_PIN                        GPIO_PIN_11                             // QUADSPI_BK1_IO0 引脚
#define QUADSPI_BK1_IO0_PORT                        GPIOD                                   // QUADSPI_BK1_IO0 引脚端口
#define QUADSPI_BK1_IO0_AF                          GPIO_AF9_QUADSPI                        // QUADSPI_BK1_IO0 IO口复用
#define     GPIO_QUADSPI_BK1_IO0_ENABLE             __HAL_RCC_GPIOD_CLK_ENABLE()            // QUADSPI_BK1_IO0 引脚时钟

#define  QUADSPI_BK1_IO1_PIN                        GPIO_PIN_2                              // QUADSPI_BK1_IO1 引脚
#define QUADSPI_BK1_IO1_PORT                        GPIOE                                   // QUADSPI_BK1_IO1 引脚端口
#define QUADSPI_BK1_IO1_AF                          GPIO_AF9_QUADSPI                        // QUADSPI_BK1_IO1 IO口复用
#define     GPIO_QUADSPI_BK1_IO1_ENABLE             __HAL_RCC_GPIOE_CLK_ENABLE()            // QUADSPI_BK1_IO1 引脚时钟

#define  QUADSPI_BK1_IO2_PIN                        GPIO_PIN_12                             // QUADSPI_BK1_IO2 引脚
#define QUADSPI_BK1_IO2_PORT                        GPIOD                                   // QUADSPI_BK1_IO2 引脚端口
#define QUADSPI_BK1_IO2_AF                          GPIO_AF9_QUADSPI                        // QUADSPI_BK1_IO2 IO口复用
#define     GPIO_QUADSPI_BK1_IO2_ENABLE             __HAL_RCC_GPIOD_CLK_ENABLE()            // QUADSPI_BK1_IO2 引脚时钟

#define  QUADSPI_BK1_IO3_PIN                        GPIO_PIN_13                             // QUADSPI_BK1_IO3 引脚
#define QUADSPI_BK1_IO3_PORT                        GPIOD                                   // QUADSPI_BK1_IO3 引脚端口
#define QUADSPI_BK1_IO3_AF                          GPIO_AF9_QUADSPI                        // QUADSPI_BK1_IO3 IO口复用
#define     GPIO_QUADSPI_BK1_IO3_ENABLE             __HAL_RCC_GPIOD_CLK_ENABLE()            // QUADSPI_BK1_IO3 引脚时钟

/* Functions */
int8_t  QSPI_W25Qxx_Init(void);
int8_t  QSPI_W25Qxx_Reset(void);
uint32_t QSPI_W25Qxx_ReadID(void);
int8_t  QSPI_W25Qxx_MemoryMappedMode(void);

int8_t  QSPI_W25Qxx_SectorErase(uint32_t SectorAddress);
int8_t  QSPI_W25Qxx_BlockErase_32K(uint32_t SectorAddress);
int8_t  QSPI_W25Qxx_BlockErase_64K(uint32_t SectorAddress);
int8_t  QSPI_W25Qxx_ChipErase(void);

int8_t  QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
int8_t  QSPI_W25Qxx_WriteBuffer(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
int8_t  QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
#endif






