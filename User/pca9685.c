/*
 * pca9685.c
 *
 *  Created on: 2024��4��24��
 *      Author: Admin
 */

#include <debug.h>
#include <driver_pca9685.h>
#include <pca9685.h>
#include <stdio.h>
#include <stdarg.h>
#include "interface_pca9685.h"

//#define I2CT_FLAG_TIMEOUT         ((uint32_t)(100000))
//
static pca9685_handle_t gs_handle;        /**< pca9685 handle */
//
//static __IO uint32_t  I2CTimeout;

///*
// *  I2C1
// * B6:SCL
// * B7:SDA
// */
//uint8_t pca9685_interface_iic_init(void)
//{
//        GPIO_InitTypeDef GPIO_InitStructure =
//        { 0 };
//        I2C_InitTypeDef I2C_InitTSturcture =
//        { 0 };
//
//        RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
//        RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE);
//
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_Init( GPIOB, &GPIO_InitStructure);
//
//        I2C_InitTSturcture.I2C_ClockSpeed = 400000;
//        I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
//        I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
//        I2C_InitTSturcture.I2C_OwnAddress1 = 0x01;
//        I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
//        I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//        I2C_Init( I2C1, &I2C_InitTSturcture);
//
//        I2C_Cmd( I2C1, ENABLE);
//
//        I2C_AcknowledgeConfig( I2C1, ENABLE);
//
//        return 0;
//}
//
//uint8_t pca9685_interface_iic_deinit(void)
//{
//    I2C_DeInit(I2C1);
//
//    return 0;
//}
//
//uint8_t pca9685_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
//{
//    I2CTimeout = I2CT_FLAG_TIMEOUT;
//        while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ) if ((--I2CTimeout) == 0) return 1;
//
//        I2C_GenerateSTART( I2C1, ENABLE);
//
//        I2CTimeout = I2CT_FLAG_TIMEOUT;
//        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) ) if ((--I2CTimeout) == 0) return 2;
//
//        I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Transmitter);
//
//        I2CTimeout = I2CT_FLAG_TIMEOUT;
//        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ) if ((--I2CTimeout) == 0) return 3;
//
//        I2C_SendData( I2C1, reg);
//
//        for (uint8_t i = 0; i < len; i++)
//        {
//            I2CTimeout = I2CT_FLAG_TIMEOUT;
//            while (!I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) if ((--I2CTimeout) == 0) return 4;
//
//            I2C_SendData( I2C1, buf[i]);
//        }
//
//        I2CTimeout = I2CT_FLAG_TIMEOUT;
//        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ) if ((--I2CTimeout) == 0) return 5;
//
//        I2C_GenerateSTOP( I2C1, ENABLE);
//
//        return 0;
//}
//
//uint8_t pca9685_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
//{
//    I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ) if ((--I2CTimeout) == 0) return 1;
//
//       I2C_GenerateSTART( I2C1, ENABLE);
//
//       I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) ) if ((--I2CTimeout) == 0) return 1;
//
//       I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Transmitter);
//
//       I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) ) if ((--I2CTimeout) == 0) return 1;
//
//       I2C_SendData( I2C1, reg);
//
//       I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) ) if ((--I2CTimeout) == 0) return 1;
//
//       I2C_GenerateSTOP( I2C1, ENABLE);
//
//       //----------------------------------------------------------------------
//       I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET ) if ((--I2CTimeout) == 0) return 1;
//
//       I2C_GenerateSTART( I2C1, ENABLE);
//
//       I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) ) if ((--I2CTimeout) == 0) return 1;
//
//       I2C_Send7bitAddress( I2C1, addr, I2C_Direction_Receiver);
//
//       I2CTimeout = I2CT_FLAG_TIMEOUT;
//       while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) ) if ((--I2CTimeout) == 0) return 1;
//
//       while(len--)
//       {
//           if(len==0)
//           {
//               I2C_AcknowledgeConfig(I2C1, DISABLE);
//           }
//           else
//           {
//               I2C_AcknowledgeConfig(I2C1, ENABLE);
//           }
//
//           I2CTimeout = I2CT_FLAG_TIMEOUT;
//           while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) if ((--I2CTimeout) == 0) return 1;
//
//           *buf = I2C_ReceiveData(I2C1);
//           buf++;
//       }
//
//       I2C_GenerateSTOP(I2C1, ENABLE);
//
//       return 0;
//}
//
//
//
//
///*
// * ʹ�ܽţ��͵�ƽʹ��
// * oe:B5
// */
//uint8_t pca9685_interface_oe_init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure =
//    { 0 };
//
//    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init( GPIOB, &GPIO_InitStructure);
//
//    return 0;
//}
//
//uint8_t pca9685_interface_oe_deinit(void)
//{
//
//    return 0;
//}
//
//uint8_t pca9685_interface_oe_write(uint8_t value)
//{
//    GPIO_WriteBit(GPIOB, GPIO_Pin_5, value);
//    return 0;
//}
//
//void pca9685_interface_delay_ms(uint32_t ms)
//{
//    vTaskDelay(ms/portTICK_PERIOD_MS);
//
//}

uint8_t pca9685_basic_init(pca9685_address_t addr, uint16_t hz)
{
    uint8_t res;
        uint8_t reg;

        /* link interface function */
        DRIVER_PCA9685_LINK_INIT(&gs_handle, pca9685_handle_t);
        DRIVER_PCA9685_LINK_IIC_INIT(&gs_handle, pca9685_interface_iic_init);
        DRIVER_PCA9685_LINK_IIC_DEINIT(&gs_handle, pca9685_interface_iic_deinit);
        DRIVER_PCA9685_LINK_IIC_READ(&gs_handle, pca9685_interface_iic_read);
        DRIVER_PCA9685_LINK_IIC_WEITE(&gs_handle, pca9685_interface_iic_write);
        DRIVER_PCA9685_LINK_OE_GPIO_INIT(&gs_handle, pca9685_interface_oe_init);
        DRIVER_PCA9685_LINK_OE_GPIO_DEINIT(&gs_handle, pca9685_interface_oe_deinit);
        DRIVER_PCA9685_LINK_OE_GPIO_WRITE(&gs_handle, pca9685_interface_oe_write);
        DRIVER_PCA9685_LINK_DELAY_MS(&gs_handle, pca9685_interface_delay_ms);
        DRIVER_PCA9685_LINK_DEBUG_PRINT(&gs_handle, pca9685_interface_debug_print);

        /* set addr pin */
        res = pca9685_set_addr_pin(&gs_handle, addr);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set addr pin failed.\n");

            return 1;
        }

        /* pca9685 init */
        res = pca9685_init(&gs_handle);
        if (res != 0)
        {
            printf("pca9685: init failed:%d\r\n",res);

            return 1;
        }

        /* inactive */
        res = pca9685_set_active(&gs_handle, PCA9685_BOOL_FALSE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set active failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set sleep mode */
        res = pca9685_set_sleep_mode(&gs_handle, PCA9685_BOOL_TRUE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set sleep mode failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set frequency */
        res = pca9685_output_frequency_convert_to_register(&gs_handle, PCA9685_OSCILLATOR_INTERNAL_FREQUENCY, hz, (uint8_t *)&reg);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: output frequency convert to register failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set pre scale */
        res = pca9685_set_prescaler(&gs_handle, reg);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set pre scale failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* disable external clock pin */
        res = pca9685_set_external_clock_pin(&gs_handle, PCA9685_BOOL_FALSE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set external clock pin failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* enable auto increment */
        res = pca9685_set_register_auto_increment(&gs_handle, PCA9685_BOOL_TRUE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set register auto increment failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set respond sub address 1 */
        res = pca9685_set_respond_subaddress_1(&gs_handle, PCA9685_BASIC_DEFAULT_RESPOND_SUBADDRESS_1);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set respond sub address 1 failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set respond sub address 2 */
        res = pca9685_set_respond_subaddress_2(&gs_handle, PCA9685_BASIC_DEFAULT_RESPOND_SUBADDRESS_2);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set respond sub address 2 failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set respond sub address 3 */
        res = pca9685_set_respond_subaddress_3(&gs_handle, PCA9685_BASIC_DEFAULT_RESPOND_SUBADDRESS_2);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set respond sub address 3 failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set respond all call */
        res = pca9685_set_respond_all_call(&gs_handle, PCA9685_BASIC_DEFAULT_RESPOND_ALL_CALL);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set respond all call failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set output invert */
        res = pca9685_set_output_invert(&gs_handle, PCA9685_BASIC_DEFAULT_OUTPUT_INVERT);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set output invert failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* stop output change */
        res = pca9685_set_output_change(&gs_handle, PCA9685_BASIC_DEFAULT_OUTPUT_CHANGE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set output change failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set output driver */
        res = pca9685_set_output_driver(&gs_handle, PCA9685_BASIC_DEFAULT_OUTPUT_DRIVER);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set output driver failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set output disable type */
        res = pca9685_set_output_disable_type(&gs_handle, PCA9685_OUTPUT_DISABLE_TYPE_HIGH_IMPEDANCE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set output disable type failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set sub address 1 */
        res = pca9685_set_subaddress_1(&gs_handle, PCA9685_BASIC_DEFAULT_SUBADDRESS_1);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set sub address 1 failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set sub address 2 */
        res = pca9685_set_subaddress_2(&gs_handle, PCA9685_BASIC_DEFAULT_SUBADDRESS_2);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set sub address 2 failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set sub address 3 */
        res = pca9685_set_subaddress_3(&gs_handle, PCA9685_BASIC_DEFAULT_SUBADDRESS_3);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set sub address 3 failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set all call address */
        res = pca9685_set_all_call_address(&gs_handle, PCA9685_BASIC_DEFAULT_ALL_CALL_ADDRESS);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set all call address failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* set sleep mode */
        res = pca9685_set_sleep_mode(&gs_handle, PCA9685_BOOL_FALSE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set sleep mode failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        /* active */
        res = pca9685_set_active(&gs_handle, PCA9685_BOOL_TRUE);
        if (res != 0)
        {
            pca9685_interface_debug_print("pca9685: set active failed.\n");
            (void)pca9685_deinit(&gs_handle);

            return 1;
        }

        return 0;
}

/**
 * @brief     basic example write
 * @param[in] channel is the pwm channel
 * @param[in] delay_percent is the start delay percent
 * @param[in] high_duty_cycle_percent is the pwm high duty cycle percent
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      0.0 <= delay_percent + high_duty_cycle_percent <= 100.0
 *            0.0 <= delay_percent <= 100.0
 *            0.0 <= high_duty_cycle_percent <= 100.0
 */
uint8_t pca9685_basic_write(pca9685_channel_t channel, float delay_percent, float high_duty_cycle_percent)
{
    uint8_t res;
    uint16_t on_count, off_count;

    /* convert data */
    res = pca9685_pwm_convert_to_register(&gs_handle, delay_percent, high_duty_cycle_percent, (uint16_t *)&on_count, (uint16_t *)&off_count);
    if (res != 0)
    {
        return 1;
    }

    /* write channel */
    res = pca9685_write_channel(&gs_handle, channel, on_count, off_count);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

///**
// * @brief     interface print format data
// * @param[in] fmt is the format data
// * @note      none
// */
//void pca9685_interface_debug_print(const char *const fmt, ...)
//{
//    va_list temp_va;
//    va_start(temp_va, fmt);
//    va_end(temp_va);
//}










