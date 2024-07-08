#include "debug.h"
#include "stdlib.h"
#include "application_pca9685.h"

void rov_depth_control(motor_power_req *req, double z)
{

       if (z >= 0.03)      // 上升使用正向动力 positive
       {
           req->center_left  = z * default_params.center_left.power_positive *
                                   default_params.center_left.enabled ;
           req->center_right = z * default_params.center_right.power_positive *
                                   default_params.center_right.enabled;
           printf("\r\ncenter_L: %d----------center_R: %d\r\n",(int)(req->center_left*100),(int)(req->center_right*100));
       }
       else if(z <= 0.03) // 上升使用反向动力 negative
       {
           req->center_left  = z * default_params.center_left.power_negative *
                                                  default_params.center_left.enabled ;
           req->center_right = z * default_params.center_right.power_negative *
                                                  default_params.center_right.enabled;
       }

}

/**
 * @brief  推进器水平控制
 * @param  推进器推力结构体
 * @param  摇杆
 */
void horizontal_control(motor_power_req *req, double x, double y, double z, double r)
{
    static float propeller_Direction_Down_Y = -0.5f;
    static float propeller_Direction_Up_Y   = -0.5f;

    if (abs(y) < 0.03)  y  = 0;
    if (abs(x) < 0.03)  x  = 0;
    if (abs(r) < 0.03)  r = 0;

                                //2 * 反转 * 使能 *
                                //((手柄x * 正向动力百分比 + 手柄y 推进器安装方向 *
                                //反向动力百分比 * 2 ) - 手柄yaw
    req->back_left  = 2  * default_params.back_left.enabled *
                                (
                                    + x * default_params.back_left.power_positive
                                    + y * propeller_Direction_Down_Y * default_params.back_left.power_negative
                                    - r
                                );
    req->back_left = clamp(req->back_left, -1.0f, 1.0f);


    req->back_right = 2  * default_params.back_right.enabled *
                                (
                                    + x * default_params.back_right.power_positive
                                    + y * propeller_Direction_Down_Y * default_params.back_right.power_negative
                                    - r
                                 );
    req->back_right = clamp(req->back_right, -1.0f, 1.0f);


    req->front_left    = 2  * default_params.front_left.enabled *
                                (
                                    + x * default_params.front_left.power_positive
                                    + y * propeller_Direction_Up_Y * default_params.front_left.power_negative
                                    - r
                                );
    req->front_left = clamp(req->front_left, -1.0f, 1.0f);


    req->center_right   = 2 * default_params.center_right.enabled *
                                (
                                    + x * default_params.center_right.power_positive
                                    + y * propeller_Direction_Up_Y * default_params.center_right.power_negative
                                    - r
                                );
    req->center_right = clamp(req->center_right, -1.0f, 1.0f);

}

motor_power_req uvm_manual_ctrl( double x, double y, double z, double r)
{
    motor_power_req req = {0};
    //深度
    rov_depth_control(&req,z);
    //水平
    horizontal_control(&req,x,y,z,r);

    return req;
}
