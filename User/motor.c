#include "motor.h"

#include <debug.h>
#include <pca9685.h>

#define CALL_FOR_ALL_PROPELLER(function) \
    function(front_left) \
    function(front_right) \
    function(center_left) \
    function(center_right) \
    function(back_left) \
    function(back_right)

#define us2percent(us) (((us)*100.0)/20000.0) // 一个周期 20 ms
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt))) //限幅函数

static int per_motor_init(propeller_attr *attr)
{
    if (attr->enabled == true)
        if (pca9685_basic_write(attr->channel, 0.0f, us2percent(PROPELLER_DUTY_MID)) != 0)
            return -1;
        else
            return 0;
    else
        if (pca9685_basic_write(attr->channel, 0.0f, 0.0f) != 0)
            return -1;
        else
            return 0;
}

int uvm_motor_init(propeller_params *cfg)
{
    printf("pca9685 init starting.");
    if (pca9685_basic_init(PCA9685_ADDRESS_A000000, 50 + cfg->pwm_freq_offset) != 0)
    {
        return -1;
    }
    printf("pca9685 init finished.");
    int ret = 0;

    #define PWM_COTROLLER_WRITE(propeller) ret += per_motor_init(&cfg->propeller);
    CALL_FOR_ALL_PROPELLER(PWM_COTROLLER_WRITE);
    #undef PWM_COTROLLER_WRITE

    printf("motor init finished.");
//    if (ret != 0)
//    {
//        pca9685_basic_deinit();
//        return -2;
//    }
    return 0;
}

int uvm_motor_deinit()
{
    return -(pca9685_basic_deinit());
}

static int per_motor_write(propeller_attr *attr, float power_percent)
{
    if (attr->enabled == true)
    {
        int16_t duty_offset = 0;
        if (power_percent != 0)
        {
            duty_offset = PROPELLER_DUTY_OFFSET_MAX * constrain(power_percent, -attr->power_negative, attr->power_positive);
            duty_offset += duty_offset > 0 ? attr->deadzone_upper : attr->deadzone_lower;
            duty_offset *= attr->reversed == true ? -1 : 1;
        }
        if (pca9685_basic_write(attr->channel, 0.0f, us2percent(PROPELLER_DUTY_MID + duty_offset)) != 0)
            return -1;
        else
            return 0;
    }
    return 0;
}

int uvm_motor_write(propeller_params *cfg, motor_power_req req)
{
    printf("motor power req:%d %d %d %d %d %d", (int)(req.back_left*10), (int)(req.back_right*10), (int)(req.center_left*10), (int)(req.center_right*10), (int)(req.front_left*10), (int)(req.front_right*10));
    int ret = 0;

    #define PWM_COTROLLER_WRITE(propeller) ret += per_motor_write(&cfg->propeller, req.propeller);
    CALL_FOR_ALL_PROPELLER(PWM_COTROLLER_WRITE);
    #undef PWM_COTROLLER_WRITE

    if (ret != 0)
    {
        printf("one of pca9685 channel write failed.");
        return -1;
    }
    return 0;
}
