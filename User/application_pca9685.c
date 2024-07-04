/*
 * application_pca9685.c
 *
 *  Created on: 2024��7��4��
 *      Author: Admin
 */

#include "application_pca9685.h"

propeller_params default_params = {
    .pwm_freq_offset = -2,
    .front_right = {
        .channel = 0,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .front_left = {
        .channel = 1,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .center_right = {
        .channel = 2,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .center_left = {
        .channel = 3,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .back_right = {
        .channel = 4,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .back_left = {
        .channel = 5,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
};


void config_handle(uint8_t *rxbuf, uint8_t *txbuf)
{
    // ��һ���ֽ�ѡ�񲿷�
    uint8_t section = rxbuf[1];
    // �ڶ����ֽ�ѡ�����
    uint8_t param = rxbuf[2];
    // �������ֽ�д�������
    uint32_t *value = &rxbuf[3];//3456

    // ����ṹ��ָ������
    propeller_attr *motors[] = {
        &default_params.front_right, &default_params.front_left,
        &default_params.center_right, &default_params.center_left,
        &default_params.back_right, &default_params.back_left
    };

    if (section == 0) { // pwm_freq_offset
        default_params.pwm_freq_offset = (int8_t)*value;
    }
    else if (section >= 1 && section <= 6) {
        propeller_attr *motor = motors[section - 1];

        switch (param) {
            case 0:
                motor->channel = (int16_t)*value;
                break;
            case 1:
                motor->enabled = (bool)*value;
                break;
            case 2:
                motor->reversed = (bool)*value;
                break;
            case 3:
                motor->deadzone_upper = (int16_t)*value;
                break;
            case 4:
                motor->deadzone_lower = (int16_t)*value;
                break;
            case 5:
                motor->power_positive = (float)*value;
                break;
            case 6:
                motor->power_negative = (float)*value;
                break;
        }
    }
    *txbuf=*rxbuf;
}

// 限幅函数实现
float clamp(float value, float min, float max) {
    return value < min ? min : (value > max ? max : value);
}

