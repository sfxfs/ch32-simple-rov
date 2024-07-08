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


void depth_sensor_cope_data(uint8_t data)
{
    static uint8_t rxBuffer[30] = {0}; // ���ݰ� T=xx.xx D=xx.xx\r\n
    static uint8_t rxCount = 0;        // ���ռ���

    rxBuffer[rxCount++] = data; // ���յ������ݴ��뻺������

    if (rxBuffer[0] != 'T')
    {
        // ����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
        rxCount = 0; // ��ջ�����
        return;
    }
    if (data != '\n')
        return; // û�е�����β���򷵻�

    rxBuffer[rxCount] = '\0';
    /**** Do work ****/

    char *temp_head = strchr(rxBuffer, '=');
    if (temp_head++ != NULL)
        ms5837_temperature = atof(temp_head);

    char *depth_head = strchr(temp_head, '=');
    if (depth_head++ != NULL)
        ms5837_depth = atof(depth_head);

    /**** End up ****/
    rxCount = 0; // ��ջ�����
}

static void jy901_convert(uint8_t which, jy901_t *jy901)
{
    // ѡ�����ݰ�
    switch (which)
    {
        case 0x51:
        {
            jy901->acc.x = (jy901_raw.stcAcc.a[0] / 32768.0f*16); // 32768*16
            jy901->acc.y = (jy901_raw.stcAcc.a[1] / 32768.0f*16);
            jy901->acc.z = (jy901_raw.stcAcc.a[2] / 32768.0f*16);
            jy901->temperature = (jy901_raw.stcAcc.T / 100.0f);
            //printf("acc %0.2f %0.2f %0.2f  %0.2fC\n", jy901->acc.x, jy901->acc.y, jy901->acc.z, jy901->temperature);
        }
        break;
        case 0x52:
        {
            jy901->gyro.x = (float)jy901_raw.stcGyro.w[0] / 2048 * 125; // 32768*2000
            jy901->gyro.y = (float)jy901_raw.stcGyro.w[1] / 2048 * 125;
            jy901->gyro.z = (float)jy901_raw.stcGyro.w[2] / 2048 * 125;
            //printf("gyro %0.2f %0.2f %0.2f\n", jy901->gyro.x, jy901->gyro.y, jy901->gyro.z);
        }
        break;
        case 0x53:
        {
            jy901->roll = (((jy901_raw.stcAngle.angle[1]<<8)|(jy901_raw.stcAngle.angle[0])) / 32768.0f*180); // 32768*180;
            jy901->pitch= (((jy901_raw.stcAngle.angle[3]<<8)|(jy901_raw.stcAngle.angle[2])) / 32768.0f*180);
            jy901->yaw  = (((jy901_raw.stcAngle.angle[5]<<8)|(jy901_raw.stcAngle.angle[4])) / 32768.0f*180);
            //printf("angle %0.2f %0.2f %0.2f\n", jy901->roll, jy901->pitch, jy901->yaw);
        }
        break;
        case 0x54:
        {
            jy901->mag.x = jy901_raw.stcMag.h[0];
            jy901->mag.y = jy901_raw.stcMag.h[1];
            jy901->mag.z = jy901_raw.stcMag.h[2];
            //printf("mag %d %d %d\n", jy901->mag.x, jy901->mag.y, jy901->mag.z);
        }
        break;
        case 0x56: // ��ѹֵ
        {
            jy901->pressure = jy901_raw.stcPress.lPressure;
            jy901->altitude = jy901_raw.stcPress.lAltitude;
            //printf("mag %d %d %d\n", jy901->mag.x, jy901->mag.y, jy901->mag.z);
        }
        break;
        default:
        break;
    }
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
    for (i = 0; i < JY901_PACKET_LENGTH - 1; i++)
        rxCheck += rxBuffer[i]; //У��λ�ۼ�

    if (rxCheck == rxBuffer[JY901_PACKET_LENGTH - 1]) // �ж����ݰ�У���Ƿ���ȷ
    {

        // �ж��������������ݣ�Ȼ���俽������Ӧ�Ľṹ���У���Щ���ݰ���Ҫͨ����λ���򿪶�Ӧ������󣬲��ܽ��յ�������ݰ�������
        switch (rxBuffer[1])
        {
        case 0x50: // �������� ��ȥ��ͷ�����ݳ���λ
            memcpy(&jy901_raw.stcTime, &rxBuffer[2], 8);
            break;
        case 0x51:
            memcpy(&jy901_raw.stcAcc, &rxBuffer[2], 8);
            break;
        case 0x52:
            memcpy(&jy901_raw.stcGyro, &rxBuffer[2], 8);
            break;
        case 0x53:
            memcpy(&jy901_raw.stcAngle, &rxBuffer[2], 8);
            jy901_raw.stcAngle.angle[0] =  rxBuffer[2];
            jy901_raw.stcAngle.angle[1] =  rxBuffer[3];
            jy901_raw.stcAngle.angle[2] =  rxBuffer[4];
            jy901_raw.stcAngle.angle[3] =  rxBuffer[5];
            jy901_raw.stcAngle.angle[4] =  rxBuffer[6];
            jy901_raw.stcAngle.angle[5] =  rxBuffer[7];
            break;
        case 0x54:
            memcpy(&jy901_raw.stcMag, &rxBuffer[2], 8);
            break;
        case 0x55:
            memcpy(&jy901_raw.stcDStatus, &rxBuffer[2], 8);
            break;
        case 0x56:
            memcpy(&jy901_raw.stcPress, &rxBuffer[2], 8);
            break;
        case 0x57:
            memcpy(&jy901_raw.stcLonLat, &rxBuffer[2], 8);
            break;
        case 0x58:
            memcpy(&jy901_raw.stcGPSV, &rxBuffer[2], 8);
            break;
        case 0x59:
            memcpy(&jy901_raw.stcQ, &rxBuffer[2], 8);
            break;
        }
        /* JY901 ����ת�� */
        jy901_convert(rxBuffer[1], &jy901);
    }
    rxCount = 0; // ��ջ�����
    rxCheck = 0; // У��λ����
}
