/*
 * pca9685.h
 *
 *  Created on: 2024Äê4ÔÂ24ÈÕ
 *      Author: Admin
 */

#ifndef USER_PCA9685_H_
#define USER_PCA9685_H_

#include "driver_pca9685.h"


#define PCA9685_BASIC_DEFAULT_RESPOND_SUBADDRESS_1        PCA9685_BOOL_FALSE                                /**< disable */
#define PCA9685_BASIC_DEFAULT_RESPOND_SUBADDRESS_2        PCA9685_BOOL_FALSE                                /**< disable */
#define PCA9685_BASIC_DEFAULT_RESPOND_SUBADDRESS_2        PCA9685_BOOL_FALSE                                /**< disable */
#define PCA9685_BASIC_DEFAULT_RESPOND_ALL_CALL            PCA9685_BOOL_FALSE                                /**< disable */
#define PCA9685_BASIC_DEFAULT_OUTPUT_INVERT               PCA9685_BOOL_FALSE                                /**< disable */
#define PCA9685_BASIC_DEFAULT_OUTPUT_CHANGE               PCA9685_OUTPUT_CHANGE_STOP                        /**< stop change */
#define PCA9685_BASIC_DEFAULT_OUTPUT_DRIVER               PCA9685_OUTPUT_DRIVER_TOTEM_POLE                  /**< totem pole */
#define PCA9685_BASIC_DEFAULT_OUTPUT_DISABLE_TYPE         PCA9685_OUTPUT_DISABLE_TYPE_HIGH_IMPEDANCE        /**< high impedance */
#define PCA9685_BASIC_DEFAULT_ALL_CALL_ADDRESS            0xE0                                              /**< 0xE0 */
#define PCA9685_BASIC_DEFAULT_SUBADDRESS_1                0xE2                                              /**< 0xE2 */
#define PCA9685_BASIC_DEFAULT_SUBADDRESS_2                0xE4                                              /**< 0xE4 */
#define PCA9685_BASIC_DEFAULT_SUBADDRESS_3                0xE8                                              /**< 0xE8 */

//void pca9685_interface_debug_print(const char *const fmt, ...);
//uint8_t pca9685_interface_iic_init(void);
//uint8_t pca9685_iic_deinit(void);
//uint8_t pca9685_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);
//uint8_t pca9685_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);
//uint8_t pca9685_interface_oe_init(void);
//uint8_t pca9685_interface_oe_write(uint8_t value);
//void pca9685_interface_delay_ms(uint32_t ms);
uint8_t pca9685_basic_init(pca9685_address_t addr, uint16_t hz);
uint8_t pca9685_basic_deinit(void);
uint8_t pca9685_basic_write(pca9685_channel_t channel, float delay_percent, float high_duty_cycle_percent);

#endif /* USER_PCA9685_H_ */
