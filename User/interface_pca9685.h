/*
 * interface_pca9685.h
 *
 *  Created on: 2024Äê6ÔÂ12ÈÕ
 *      Author: Admin
 */

#ifndef USER_INTERFACE_PCA9685_H_
#define USER_INTERFACE_PCA9685_H_

#include "debug.h"

/**
 * @defgroup pca9685_interface_driver pca9685 interface driver function
 * @brief    pca9685 interface driver modules
 * @ingroup  pca9685_driver
 * @{
 */

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t pca9685_interface_iic_init(void);

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t pca9685_interface_iic_deinit(void);

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
uint8_t pca9685_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);

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
uint8_t pca9685_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief  interface oe init
 * @return status code
 *         - 0 success
 *         - 1 clock init failed
 * @note   none
 */
uint8_t pca9685_interface_oe_init(void);

/**
 * @brief  interface oe deinit
 * @return status code
 *         - 0 success
 *         - 1 clock deinit failed
 * @note   none
 */
uint8_t pca9685_interface_oe_deinit(void);

/**
 * @brief     interface oe write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t pca9685_interface_oe_write(uint8_t value);

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void pca9685_interface_delay_ms(uint32_t ms);

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void pca9685_interface_debug_print(const char *const fmt, ...);

/**
 * @}
 */

#endif /* USER_INTERFACE_PCA9685_H_ */
