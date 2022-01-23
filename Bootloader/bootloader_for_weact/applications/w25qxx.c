/**
 * @file w25qxx.c
 * @brief boot from rt-thread w25qxx driver
 * @author RedlightASl (dddbbbdd@foxmail.com)
 * @version 1.0
 * @date 2022-01-23
 *
 * @copyright Copyright (c) 2022  RedlightASl
 *
 * @note
 * 函数均使用HAL库函数实现
 * 无DMA或中断
 *
 * @par 修改日志:
 * <table>
 * <tr><th>Date    <th>Version  <th>Author  <th>Description
 * <tr><td>2022-01-23  <td>1.0      <td>wangh   <td>Content
 * </table>
 */
#include "w25qxx.h"
#include <drv_common.h>

QSPI_HandleTypeDef hqspi;

/**
 * @brief Init QSPI Pins
 * @param  hqspi            spi handler
 */
void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if (hqspi->Instance == QUADSPI)
    {
        __HAL_RCC_QSPI_CLK_ENABLE();

        GPIO_QUADSPI_CLK_ENABLE;
        GPIO_QUADSPI_BK1_NCS_ENABLE;
        GPIO_QUADSPI_BK1_IO0_ENABLE;
        GPIO_QUADSPI_BK1_IO1_ENABLE;
        GPIO_QUADSPI_BK1_IO2_ENABLE;
        GPIO_QUADSPI_BK1_IO3_ENABLE;

        /******************************************************
        PB2     ------> QUADSPI_CLK
        PB6     ------> QUADSPI_BK1_NCS
        PD11    ------> QUADSPI_BK1_IO0
        PD12    ------> QUADSPI_BK1_IO1
        PE2     ------> QUADSPI_BK1_IO2
        PD13    ------> QUADSPI_BK1_IO3
        *******************************************************/

        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        GPIO_InitStruct.Pin = QUADSPI_CLK_PIN;
        GPIO_InitStruct.Alternate = QUADSPI_CLK_AF;
        HAL_GPIO_Init(QUADSPI_CLK_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = QUADSPI_BK1_NCS_PIN;
        GPIO_InitStruct.Alternate = QUADSPI_BK1_NCS_AF;
        HAL_GPIO_Init(QUADSPI_BK1_NCS_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = QUADSPI_BK1_IO0_PIN;
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO0_AF;
        HAL_GPIO_Init(QUADSPI_BK1_IO0_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = QUADSPI_BK1_IO1_PIN;
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO1_AF;
        HAL_GPIO_Init(QUADSPI_BK1_IO1_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = QUADSPI_BK1_IO2_PIN;
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO2_AF;
        HAL_GPIO_Init(QUADSPI_BK1_IO2_PORT, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = QUADSPI_BK1_IO3_PIN;
        GPIO_InitStruct.Alternate = QUADSPI_BK1_IO3_AF;
        HAL_GPIO_Init(QUADSPI_BK1_IO3_PORT, &GPIO_InitStruct);
    }
}

/**
 * @brief Init QSPI Config
 */
void MX_QUADSPI_Init(void)
{
    hqspi.Instance = QUADSPI;

    hqspi.Init.ClockPrescaler = 1;

    hqspi.Init.FifoThreshold = 32;
    hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
    hqspi.Init.FlashSize = 22;
    hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
    hqspi.Init.ClockMode = QSPI_CLOCK_MODE_3;
    hqspi.Init.FlashID = QSPI_FLASH_ID_1;
    hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;

    HAL_QSPI_Init(&hqspi); // 初始化配置
}

/**
 * @brief Init W25Qxx
 * @return int8_t
 */
int8_t QSPI_W25Qxx_Init(void)
{
    uint32_t    Device_ID;

    MX_QUADSPI_Init();
    QSPI_W25Qxx_Reset();
    Device_ID = QSPI_W25Qxx_ReadID();
    if (Device_ID == W25Qxx_FLASH_ID)
    {
        rt_kprintf("W25Q64 OK,flash ID:%X\r\n", Device_ID);
        return QSPI_W25Qxx_OK;
    }
    else
    {
        rt_kprintf("W25Q64 ERROR!!!!!  ID:%X\r\n", Device_ID);
        return W25Qxx_ERROR_INIT;
    }
}

/**
 * @brief Automatically Poll check Mem Flags
 * @return int8_t QSPI_W25Qxx_OK ; W25Qxx_ERROR_AUTOPOLLING
 */
int8_t QSPI_W25Qxx_AutoPollingMemReady(void)
{
    QSPI_CommandTypeDef     s_command;
    QSPI_AutoPollingTypeDef s_config;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.Instruction = W25Qxx_CMD_ReadStatus_REG1;

    s_config.Match = 0;
    s_config.MatchMode = QSPI_MATCH_MODE_AND;
    s_config.Interval = 0x10;
    s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
    s_config.StatusBytesSize = 1;
    s_config.Mask = W25Qxx_Status_REG1_BUSY;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;

}

/**
 * @brief Reset W25Qxx
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_INIT
 */
int8_t QSPI_W25Qxx_Reset(void)
{
    QSPI_CommandTypeDef s_command;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.Instruction = W25Qxx_CMD_EnableReset;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_INIT;
    }

    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    s_command.Instruction = W25Qxx_CMD_ResetDevice;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_INIT;
    }
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief Check SPI Flash ID
 * @return uint32_t W25Qxx_ID
 */
uint32_t QSPI_W25Qxx_ReadID(void)
{
    QSPI_CommandTypeDef s_command;
    uint8_t QSPI_ReceiveBuff[3];
    uint32_t    W25Qxx_ID;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.DummyCycles = 0;
    s_command.NbData = 3;
    s_command.Instruction = W25Qxx_CMD_JedecID;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        // return W25Qxx_ERROR_INIT;
    }
    if (HAL_QSPI_Receive(&hqspi, QSPI_ReceiveBuff, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        // return W25Qxx_ERROR_TRANSMIT;
    }
    W25Qxx_ID = (QSPI_ReceiveBuff[0] << 16) | (QSPI_ReceiveBuff[1] << 8) | QSPI_ReceiveBuff[2];

    return W25Qxx_ID;
}

/**
 * @brief set SPI Flash as MemoryMappedMode
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_WriteEnable
 * @note read only when using MemoryMappedMode
 */
int8_t QSPI_W25Qxx_MemoryMappedMode(void)
{
    QSPI_CommandTypeDef      s_command;
    QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_4_LINES;
    s_command.DataMode = QSPI_DATA_4_LINES;
    s_command.DummyCycles = 6;
    s_command.Instruction = W25Qxx_CMD_FastReadQuad_IO;

    s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    s_mem_mapped_cfg.TimeOutPeriod = 0;

    QSPI_W25Qxx_Reset();
    if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
    {
        return W25Qxx_ERROR_MemoryMapped;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief W25Qxx Write enable
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_WriteEnable
 */
int8_t QSPI_W25Qxx_WriteEnable(void)
{
    QSPI_CommandTypeDef     s_command;
    QSPI_AutoPollingTypeDef s_config;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.Instruction = W25Qxx_CMD_WriteEnable;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    s_config.Match = 0x02;
    s_config.Mask = W25Qxx_Status_REG1_WEL;
    s_config.MatchMode = QSPI_MATCH_MODE_AND;
    s_config.StatusBytesSize = 1;
    s_config.Interval = 0x10;
    s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

    s_command.Instruction = W25Qxx_CMD_ReadStatus_REG1;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.NbData = 1;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief W25Qxx Erase Sectors
 * @param  SectorAddress    the Address you want to erase
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_Erase
 * @note 4KB every time
 */
int8_t QSPI_W25Qxx_SectorErase(uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.Address = SectorAddress;
    s_command.Instruction = W25Qxx_CMD_SectorErase;

    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;
    }
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief W25Qxx Erase Blocks
 * @param  SectorAddress    the Address you want to erase
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_Erase
 * @note 32KB every time
 */
int8_t QSPI_W25Qxx_BlockErase_32K(uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.Address = SectorAddress;
    s_command.Instruction = W25Qxx_CMD_BlockErase_32K;

    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;
    }
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief W25Qxx Erase Blocks
 * @param  SectorAddress    the Address you want to erase
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_Erase
 * @note 64KB every time
 */
int8_t QSPI_W25Qxx_BlockErase_64K(uint32_t SectorAddress)
{
    QSPI_CommandTypeDef s_command;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.Address = SectorAddress;
    s_command.Instruction = W25Qxx_CMD_BlockErase_64K;

    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;
    }
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief Erase all Chip
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_Erase
 */
int8_t QSPI_W25Qxx_ChipErase(void)
{
    QSPI_CommandTypeDef s_command;
    QSPI_AutoPollingTypeDef s_config;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_NONE;
    s_command.DataMode = QSPI_DATA_NONE;
    s_command.DummyCycles = 0;
    s_command.Instruction = W25Qxx_CMD_ChipErase;

    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_Erase;
    }

    s_config.Match = 0;
    s_config.MatchMode = QSPI_MATCH_MODE_AND;
    s_config.Interval = 0x10;
    s_config.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;
    s_config.StatusBytesSize = 1;
    s_config.Mask = W25Qxx_Status_REG1_BUSY;

    s_command.Instruction = W25Qxx_CMD_ReadStatus_REG1;
    s_command.DataMode = QSPI_DATA_1_LINE;
    s_command.NbData = 1;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, W25Qxx_ChipErase_TIMEOUT_MAX) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief Write Pages
 * @param  pBuffer          Data
 * @param  WriteAddr        Address
 * @param  NumByteToWrite   Data Length 256Bytes max
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_TRANSMIT
 */
int8_t QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    QSPI_CommandTypeDef s_command;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_1_LINE;
    s_command.DataMode = QSPI_DATA_4_LINES;
    s_command.DummyCycles = 0;
    s_command.NbData = NumByteToWrite;
    s_command.Address = WriteAddr;
    s_command.Instruction = W25Qxx_CMD_QuadInputPageProgram;

    if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;
    }
    if (HAL_QSPI_Transmit(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;
    }

    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}

/**
 * @brief Write W25Qxx from Data Buffer
 * @param  pBuffer          Data Buffer
 * @param  WriteAddr        Address
 * @param  Size             Data Length
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_TRANSMIT
 */
int8_t QSPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{
    uint32_t end_addr, current_size, current_addr;
    uint8_t* write_data;

    current_size = W25Qxx_PageSize - (WriteAddr % W25Qxx_PageSize);

    if (current_size > Size)
    {
        current_size = Size;
    }

    current_addr = WriteAddr;
    end_addr = WriteAddr + Size;
    write_data = pBuffer;

    do
    {
        if (QSPI_W25Qxx_WritePage(write_data, current_addr, current_size) != QSPI_W25Qxx_OK)
        {
            return W25Qxx_ERROR_TRANSMIT;
        }

        else
        {
            current_addr += current_size;
            write_data += current_size;
            current_size = ((current_addr + W25Qxx_PageSize) > end_addr) ? (end_addr - current_addr) : W25Qxx_PageSize;
        }
    } while (current_addr < end_addr);

    return QSPI_W25Qxx_OK;

}

/**
 * @brief Read W25Qxx from Data Buffer
 * @param  pBuffer          Data Buffer
 * @param  ReadAddr         Address
 * @param  NumByteToRead    Data Length
 * @return int8_t QSPI_W25Qxx_OK;W25Qxx_ERROR_TRANSMIT
 */
int8_t QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    QSPI_CommandTypeDef s_command;

    s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    s_command.AddressSize = QSPI_ADDRESS_24_BITS;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DdrMode = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
    s_command.AddressMode = QSPI_ADDRESS_4_LINES;
    s_command.DataMode = QSPI_DATA_4_LINES;
    s_command.DummyCycles = 6;
    s_command.NbData = NumByteToRead;
    s_command.Address = ReadAddr;
    s_command.Instruction = W25Qxx_CMD_FastReadQuad_IO;

    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;
    }
    if (HAL_QSPI_Receive(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;
    }
    if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return QSPI_W25Qxx_OK;
}
