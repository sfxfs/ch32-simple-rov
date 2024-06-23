/*
 * rpc_fun.c
 *
 *  Created on: 2024年6月11日
 *      Author: ssfxfss
 */

#include "debug.h"
#include "cJSON.h"
#include "rpc_cjson.h"
#include "sensor.h"
#include <stdlib.h>
#include <string.h>

extern jy901_t jy901;
extern float ms5837_temperature;
extern float ms5837_depth;

/**
 * @brief 获取 json 内对应值
 * @param params cjson结构体
 * @param str 值对应字符串
 * @return 获得的浮点数，若没有获取到则为 0
 */
static double cjson_value_analysis_double(cJSON *params,const char *str)
{
    cJSON* cjson_temp = NULL;
    double value;
    cjson_temp = cJSON_GetObjectItem(params, str);
    if (cjson_temp != NULL)      //如果没收到则返回
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
    // pwm write...
    return 0;
}

static cJSON *get_rov_info()
{
    static char temp_str[20] = {0};
    cJSON *cjson_info = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_info, "Model", "CH32");
    sprintf(temp_str, "%.02f", ms5837_temperature);
    cJSON_AddStringToObject(cjson_info, "Temp", temp_str);
    sprintf(temp_str, "%.02f", jy901.yaw);
    cJSON_AddStringToObject(cjson_info, "Yaw", temp_str);
    sprintf(temp_str, "%.02f", ms5837_depth);
    cJSON_AddStringToObject(cjson_info, "Depth", temp_str);
    return cjson_info;
}

/**
 * @brief 链接get_rov_info函数
 * @param ctx 下位机info->sensor的指针
 * @return Cjson object
 */
cJSON *info_handler(jrpc_context *ctx, cJSON *params, cJSON *id)
{
    return get_rov_info();
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
    ret += rpc_add_method(handle, empty_handler, "catch", NULL);
    ret += rpc_add_method(handle, empty_handler, "light", NULL);
    ret += rpc_add_method(handle, empty_handler, "set_direction_locked", NULL);
    ret += rpc_add_method(handle, empty_handler, "set_depth_locked", NULL);

    ret += rpc_add_method(handle, manual_ctrl, "move", NULL);
    return ret;
}
