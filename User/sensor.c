/*
 * sensor.c
 *
 *  Created on: 2024��6��22��
 *      Author: 25016
 */

#include "sensor.h"

#include <string.h>
#include <stdlib.h>

#define JY901_PACKET_LENGTH 11   // JY901   ���ݰ�����

static jy901_raw_t jy901_raw;
jy901_t jy901;
float ms5837_temperature;
float ms5837_depth;

char depth_data[30];

volatile int jy901_data_update;
volatile int depth_data_update;

void depth_data_convert()
{
    if (depth_data_update != 1)
        return;

    char *temp_head = strchr(depth_data, 'D');
    if (temp_head++ != NULL)
        ms5837_depth = atoi(temp_head);

    char *depth_head = strchr(temp_head, 'T');
    if (depth_head++ != NULL)
        ms5837_temperature = atoi(depth_head);

    depth_data_update = 0;
}

void depth_sensor_cope_data(uint8_t data)
{
    static uint8_t rxBuffer[30] = {0}; // ���ݰ� T=xx.xx D=xx.xx\r\n
    static uint8_t rxCount = 0;        // ���ռ���

    rxBuffer[rxCount++] = data; // ���յ������ݴ��뻺������

    if (rxBuffer[0] != 'P')
    {
        // ����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
        rxCount = 0; // ��ջ�����
        return;
    }
    if (data != '\n')
        return; // û�е�����β���򷵻�

    rxBuffer[rxCount] = '\0';
    /**** Do work ****/

    depth_data_update = 1;
    memcpy(depth_data, rxBuffer, strlen(rxBuffer)+1);

    /**** End up ****/
    rxCount = 0; // ��ջ�����
}

void jy901_convert()
{
    if (jy901_data_update != 1)
        return;

    jy901.roll = (((jy901_raw.stcAngle.angle[1]<<8)|(jy901_raw.stcAngle.angle[0])) / 32768.0f*180); // 32768*180;
    jy901.pitch= (((jy901_raw.stcAngle.angle[3]<<8)|(jy901_raw.stcAngle.angle[2])) / 32768.0f*180);
    jy901.yaw  = (((jy901_raw.stcAngle.angle[5]<<8)|(jy901_raw.stcAngle.angle[4])) / 32768.0f*180);

    jy901_data_update = 0;
}

void jy901_cope_data(uint8_t data)
{
    static uint8_t i;
    static uint8_t rxBuffer[20] = {0}; // ���ݰ�
    static uint8_t rxCheck = 0;        // βУ����
    static uint8_t rxCount = 0;        // ���ռ���

    rxBuffer[rxCount++] = data; // ���յ������ݴ��뻺������

    if (rxBuffer[0] != 0x55)
    {
        // ����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
        rxCount = 0; // ��ջ�����
        return;
    }
    if (rxCount < JY901_PACKET_LENGTH)
        return; // ���ݲ���11�����򷵻�

    /*********** ֻ�н�����11���ֽ����� �Ż�������³��� ************/
    if (rxBuffer[1] == 0x53)
    {
        for (i = 0; i < JY901_PACKET_LENGTH - 1; i++)
            rxCheck += rxBuffer[i]; //У��λ�ۼ�

        if (rxCheck == rxBuffer[JY901_PACKET_LENGTH - 1]) // �ж����ݰ�У���Ƿ���ȷ
        {
            jy901_data_update = 1;
            memcpy(&jy901_raw.stcAngle.angle[0], &rxBuffer[2], 6);
        }
    }
    rxCount = 0; // ��ջ�����
    rxCheck = 0; // У��λ����
}
