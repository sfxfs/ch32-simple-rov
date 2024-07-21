/*
 * application_pca9685.c
 *
 *  Created on: 2024��7��4��
 *      Author: Admin
 */

#include "application_pca9685.h"

propeller_params default_params = {
    .pwm_freq_offset = -3,
    .front_right = {
        .channel = 7,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .front_left = {
        .channel = 4,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .center_right = {
        .channel = 6,
        .enabled = true,
        .reversed = true,
        .deadzone_upper = 32,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .center_left = {
        .channel = 3,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -32,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .back_right = {
        .channel = 5,
        .enabled = true,
        .reversed = false,
        .deadzone_upper = 25,
        .deadzone_lower = -25,
        .power_positive = 0.5,
        .power_negative = 0.5,
    },
    .back_left = {
        .channel = 2,
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
    union {
            uint32_t value;
            uint8_t bytes[4];
        } data;
        data.bytes[0] = rxbuf[6];
        data.bytes[1] = rxbuf[5];
        data.bytes[2] = rxbuf[4];
        data.bytes[3] = rxbuf[3];

    uint32_t value = data.value;////3456
    printf("section:%d -- param:%d -- value:%d \r\n",section,param,value);

    // ����ṹ��ָ������
    propeller_attr *motors[] = {
        &default_params.front_right, &default_params.front_left,
        &default_params.center_right, &default_params.center_left,
        &default_params.back_right, &default_params.back_left
    };

    if (section == 0) { // pwm_freq_offset
        default_params.pwm_freq_offset = (int16_t)value;
        sprintf(txbuf,"pwm_freq_offset has been chaged: %d\r\n",value);
    }
    else if (section >= 1 && section <= 6) {
        propeller_attr *motor = motors[section - 1];

        switch (param) {
            case 0:
                motor->channel = (int16_t)value;
                sprintf(txbuf,"channel has been chaged: %d\n",default_params.front_right.channel);
                break;
            case 1:
                motor->enabled = (bool)value;
                sprintf(txbuf,"enabled has been chaged: %d\n",motor->enabled);
                break;
            case 2:
                motor->reversed = (bool)value;
                sprintf(txbuf,"reversed has been chaged: %d\n",motor->reversed);
                break;
            case 3:
                motor->deadzone_upper = (int16_t)value;
                sprintf(txbuf,"deadzone_upper has been chaged: %d\n",motor->deadzone_upper);
                break;
            case 4:
                motor->deadzone_lower = (int16_t)value;
                sprintf(txbuf,"deadzone_lower has been chaged: %d\n",motor->deadzone_lower);
                break;
            case 5:
                motor->power_positive = (float)value;
                sprintf(txbuf,"power_positive has been chaged: %f\n",motor->power_positive);
                break;
            case 6:
                motor->power_negative = (float)value;
                sprintf(txbuf,"power_negative has been chaged: %f\n",motor->power_negative);
                break;
        }
    }
}

// 限幅函数实现
float clamp(float value, float min, float max) {
    return value < min ? min : (value > max ? max : value);
}

