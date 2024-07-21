/*
 * sensor.h
 *
 *  Created on: 2024��6��22��
 *      Author: 25016
 */

#ifndef USER_SENSOR_H_
#define USER_SENSOR_H_

#include "debug.h"

/* ʱ�� */
struct STime
{
    unsigned char ucYear;
    unsigned char ucMonth;
    unsigned char ucDay;
    unsigned char ucHour;
    unsigned char ucMinute;
    unsigned char ucSecond;
    unsigned short usMiliSecond;
};

/* ���ٶ� */
struct SAcc
{
    short a[3];
    short T;
};

/* ���ٶ� */
struct SGyro
{
    short w[3];
    short T;
};

/* �Ƕ� */
struct SAngle
{
    short angle[6];
    short T;
};

/* �ų� */
struct SMag
{
    short h[3];
    short T;
};

/* ����״̬ */
struct SDStatus
{
    short sDStatus[4];
};

/* ��ѹ/���� */
struct SPress
{
    long lPressure;
    long lAltitude;
};

/* ��γ�� */
struct SLonLat
{
    long lLon;
    long lLat;
};

/* GPS */
struct SGPSV
{
    short sGPSHeight;
    short sGPSYaw;
    long lGPSVelocity;
};

/* ��Ԫ�� */
struct SQ
{
    short q[4];
};

/* JY901 ԭʼ���� */
typedef struct
{
    struct STime stcTime;
    struct SAcc stcAcc;
    struct SGyro stcGyro;
    struct SAngle stcAngle;
    struct SMag stcMag;
    struct SDStatus stcDStatus;
    struct SPress stcPress;
    struct SLonLat stcLonLat;
    struct SGPSV stcGPSV;
    struct SQ stcQ;

} jy901_raw_t;

 //x��y��z ��ά����
typedef struct
{
    float x;
    float y;
    float z;

} Vector3f;

//x��y��z ��ά����
typedef struct
{
    short x;
    short y;
    short z;

} Vector3s;

/* JY901 ��ʵ���� */
typedef struct
{
    Vector3f acc;     //���ٶ�
    Vector3f gyro;    //���ٶ�
    Vector3f speed;   //����
    Vector3s mag;     //�ų�
    float pressure;   // ����ѹ
    float altitude;   // ���θ߶�
    float roll;        //�����
    float pitch;       //������
    float yaw;         //�����
    float temperature; //�¶�

} jy901_t;

extern jy901_t jy901;
extern int ms5837_depth;
extern int ms5837_temperature;

void depth_sensor_cope_data(uint8_t data);
void jy901_cope_data(uint8_t data);
void jy901_convert(void);
void depth_data_convert(void);

#endif /* USER_SENSOR_H_ */
