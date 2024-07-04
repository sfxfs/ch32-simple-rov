/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_pca9685_interface_template.c
 * @brief     driver pca9685 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-02-20
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/02/20  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "interface_pca9685.h"

#include <debug.h>
#include <driver_pca9685.h>
#include <pca9685.h>
#include <stdio.h>
#include <stdarg.h>

static __IO uint32_t  I2CTimeout;

#define I2CT_FLAG_TIMEOUT         ((uint32_t)(100000))

//static pca9685_handle_t gs_handle;        /**< pca9685 handle */


//#define PCA9685_I2C_DEV "/dev/i2c-0"
//#define PCA9685_GPIO_NUM 203 // PG11 = ( 7 - 1) * 32 + 11


/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 *         B6 - SCL
 *         B7 - SDA
 * @note   none
 */
uint8_t pca9685_interface_iic_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure =
        { 0 };
        I2C_InitTypeDef I2C_InitTSturcture =
        { 0 };

        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init( GPIOB, &GPIO_InitStructure);

        I2C_InitTSturcture.I2C_ClockSpeed = 400000;
        I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
        I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
        I2C_InitTSturcture.I2C_OwnAddress1 = 0x01;
        I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
        I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
        I2C_Init( I2C1, &I2C_InitTSturcture);

        I2C_Cmd( I2C1, ENABLE);

        I2C_AcknowledgeConfig( I2C1, ENABLE);

        return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t pca9685_interface_iic_deinit(void)
{
    I2C_DeInit(I2C1);

    return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t pca9685_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    I2CTimeout = I2CT_FLAG_TIMEOUT;
        while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ) if ((--I2CTimeout) == 0) return 1;

        I2C_GenerateSTART( I2C1, ENABLE);

        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) ) if ((--I2CTimeout) == 0) return 2;

        I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Transmitter);

        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ) if ((--I2CTimeout) == 0) return 3;

        I2C_SendData( I2C1, reg);

        for (uint8_t i = 0; i < len; i++)
        {
            I2CTimeout = I2CT_FLAG_TIMEOUT;
            while (!I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) if ((--I2CTimeout) == 0) return 4;

            I2C_SendData( I2C1, buf[i]);
        }

        I2CTimeout = I2CT_FLAG_TIMEOUT;
        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ) if ((--I2CTimeout) == 0) return 5;

        I2C_GenerateSTOP( I2C1, ENABLE);

        return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t pca9685_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ) if ((--I2CTimeout) == 0) return 1;

       I2C_GenerateSTART( I2C1, ENABLE);

       I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) ) if ((--I2CTimeout) == 0) return 1;

       I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Transmitter);

       I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ) if ((--I2CTimeout) == 0) return 1;

       I2C_SendData( I2C1, reg);

       I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ) if ((--I2CTimeout) == 0) return 1;

       I2C_GenerateSTOP( I2C1, ENABLE);

       //----------------------------------------------------------------------
       I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ) if ((--I2CTimeout) == 0) return 1;

       I2C_GenerateSTART( I2C1, ENABLE);

       I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) ) if ((--I2CTimeout) == 0) return 1;

       I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Receiver);

       I2CTimeout = I2CT_FLAG_TIMEOUT;
       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) ) if ((--I2CTimeout) == 0) return 1;

       while(len--)
       {
           if(len==0)
           {
               I2C_AcknowledgeConfig(I2C1, DISABLE);
           }
           else
           {
               I2C_AcknowledgeConfig(I2C1, ENABLE);
           }

           I2CTimeout = I2CT_FLAG_TIMEOUT;
           while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) if ((--I2CTimeout) == 0) return 1;

           *buf = I2C_ReceiveData(I2C1);
           buf++;
       }

       I2C_GenerateSTOP(I2C1, ENABLE);

       return 0;
}

/*
 * 使能脚，低电平使能
 * oe:B5
 */
uint8_t pca9685_interface_oe_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure =
    { 0 };

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure);

    return 0;
}

/**
 * @brief  interface oe deinit
 * @return status code
 *         - 0 success
 *         - 1 clock deinit failed
 * @note   none
 */
uint8_t pca9685_interface_oe_deinit(void)
{

    return 0;
}

/**
 * @brief     interface oe write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t pca9685_interface_oe_write(uint8_t value)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_5, value);
    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void pca9685_interface_delay_ms(uint32_t ms)
{
    Delay_Ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void pca9685_interface_debug_print(const char *const fmt, ...)
{
    va_list temp_va;
    va_start(temp_va, fmt);
    va_end(temp_va);
}

