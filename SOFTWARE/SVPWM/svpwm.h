//
// Created by MHR on 2023-6-11.
//

#ifndef SVPWM_SVPWM_H
#define SVPWM_SVPWM_H
typedef struct
{
    float svpwm_udc;
    float svpwm_Ts;
    float svpwm_Tx;
    float svpwm_Ty;
    float svpwm_T0;
    float svpwm_ua;
    float svpwm_ub;
    float svpwm_uc;
    float svpwm_ualpha;
    float svpwm_ubeta;
    unsigned char svpwm_n;

} svpwmtype;
void svpwm_init(svpwmtype *svpwmtype1, float udc, float Ts);

void svpwm_sector(svpwmtype *svpwmtype1);

void svpwm_update(svpwmtype *svpwmtype1, float ualpha, float ubeta);

void gen_svpwm(svpwmtype *svpwmtype1);

#endif //SVPWM_SVPWM_H
