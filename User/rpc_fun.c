/*
 * rpc_fun.c
 *
 *  Created on: 2024��6��11��
 *      Author: ssfxfss
 */

#include "debug.h"
#include "cJSON.h"
#include "rpc_cjson.h"
#include "sensor.h"
#include "motor.h"
#include "pca9685.h"
#include "application_pca9685.h"
#include "control.h"
#include <stdlib.h>
#include <string.h>

#define us2percent(us) (((us)*100.0)/20000.0) // 一个周期 20 ms
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) //限幅函数

/**
 * @brief ��ȡ json �ڶ�Ӧֵ
 * @param params cjson�ṹ��
 * @param str ֵ��Ӧ�ַ���
 * @return ��õĸ���������û�л�ȡ����Ϊ 0
 */
static double cjson_value_analysis_double(cJSON *params,const char *str)
{
    cJSON* cjson_temp = NULL;
    double value;
    cjson_temp = cJSON_GetObjectItem(params, str);
    if (cjson_temp != NULL)      //���û�յ��򷵻�
    {
        value = cjson_temp->valuedouble;
    }
    else value = 0;
    return value;
}

//---------------------------------------------------------------------------------

static int rpc_manual_ctrl(double x, double y, double z, double r)
{
    printf("manual ctrl cmd recv: x|%lf y|%lf z|%lf r|%lf\r\n", x, y, z, r);
    uvm_motor_write(&default_params, uvm_manual_ctrl(x, y, z, r));
    return 0;
}

static cJSON *get_rov_info()
{
    static char temp_str[20] = {0};
    cJSON *cjson_info = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_info, "Model", "CH32");
//    sprintf(temp_str, "%.02f", ms5837_temperature);
//    cJSON_AddStringToObject(cjson_info, "Temp", temp_str);
    jy901_convert();
    sprintf(temp_str, "%.02f", jy901.yaw);
    cJSON_AddStringToObject(cjson_info, "Yaw", temp_str);
    depth_data_convert();
    sprintf(temp_str, "%.02f", ms5837_depth);
    cJSON_AddStringToObject(cjson_info, "Depth", temp_str);
    return cjson_info;
}

/**
 * @brief ����get_rov_info����
 * @param ctx ��λ��info->sensor��ָ��
 * @return Cjson object
 */
cJSON *info_handler(jrpc_context *ctx, cJSON *params, cJSON *id)
{
    return get_rov_info();
}

/**
  * @brief  机械爪控制的rpc函数
  * @param  *params json参数
  * @param  *id  客户端标识
  * @return  1：接收成功；0：接收失败
  */
cJSON *catcher(jrpc_context *ctx, cJSON *params, cJSON *id)
{
    int arm_catch;
    static float cur = 1000.0f;
    cur = constrain(cur,500,2500);

    if (params == NULL) return cJSON_CreateNull();
    arm_catch = params->child->valuedouble;
    //pca9685 control -- 通道1 -- 机械臂
    if (arm_catch > 0) pca9685_basic_write(1, 0.0f,constrain(us2percent(cur+=25),0,100));
    else if (arm_catch < 0) pca9685_basic_write(1, 0.0f,constrain(us2percent(cur-=25),0,100));
    else {
        pca9685_basic_write(1, 0.0f,us2percent(cur));
    }
    //
    return cJSON_CreateNull();
}

cJSON *empty_handler(jrpc_context *ctx, cJSON *params, cJSON *id)
{
    return cJSON_CreateNull();
}

cJSON *manual_ctrl(jrpc_context *ctx, cJSON *params, cJSON *id)
{
    if (rpc_manual_ctrl(cjson_value_analysis_double(params, "x"), cjson_value_analysis_double(params, "y"), cjson_value_analysis_double(params, "z"), cjson_value_analysis_double(params, "rot")) != 0)
    {
        ctx->error_code = JRPC_INTERNAL_ERROR;
        ctx->error_message = strdup("Write to pwm controller failed.");
    }
    return cJSON_CreateNull();
}

int rpc_add_all_handler(rpc_handle_t *handle)
{
    int ret = 0;
    ret += rpc_add_method(handle, info_handler, "get_info", NULL);
    ret += rpc_add_method(handle, catcher, "catch", NULL);
    ret += rpc_add_method(handle, empty_handler, "light", NULL);
    ret += rpc_add_method(handle, empty_handler, "set_direction_locked", NULL);
    ret += rpc_add_method(handle, empty_handler, "set_depth_locked", NULL);

    ret += rpc_add_method(handle, manual_ctrl, "move", NULL);
    return ret;
}
