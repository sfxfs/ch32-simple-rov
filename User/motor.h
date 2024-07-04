/*
 * control.h
 *
 *  Created on: 2024Äê4ÔÂ24ÈÕ
 *      Author: Admin
 */

#ifndef USER_MOTOR_H_
#define USER_MOTOR_H_

#define PROPELLER_DUTY_MID 1500 // us
#define PROPELLER_DUTY_OFFSET_MAX 500 // us

#include <stdbool.h>
#include "debug.h"
//#include "stdint.h"



typedef struct
{
    bool reversed;
    bool enabled;
    int16_t channel;
    int16_t deadzone_upper; // us
    int16_t deadzone_lower;
    float power_positive; // -1.0 ~ 1.0
    float power_negative;
} propeller_attr;

typedef struct
{
    int16_t pwm_freq_offset;
    propeller_attr front_right;
    propeller_attr front_left;
    propeller_attr center_right;
    propeller_attr center_left;
    propeller_attr back_right;
    propeller_attr back_left;
} propeller_params;

typedef struct
{
    float front_right; // -1.0 ~ 1.0
    float front_left;
    float center_right;
    float center_left;
    float back_right;
    float back_left;
} motor_power_req;

int uvm_motor_init(propeller_params *cfg);
int uvm_motor_deinit(void);
int uvm_motor_write(propeller_params *cfg, motor_power_req req);

#endif /* USER_MOTOR_H_ */
