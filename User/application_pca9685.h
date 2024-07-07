/*
 * application_pca9685.h
 *
 *  Created on: 2024��7��4��
 *      Author: Admin
 */

#ifndef USER_APPLICATION_PCA9685_H_
#define USER_APPLICATION_PCA9685_H_

#include "debug.h"
#include "motor.h"

extern propeller_params default_params;

float clamp(float value, float min, float max);

void config_handle(uint8_t *rxbuf, uint8_t *txbuf);


#endif /* USER_APPLICATION_PCA9685_H_ */
