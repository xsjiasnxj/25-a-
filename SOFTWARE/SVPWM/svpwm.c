//
// Created by MHR on 2023-6-11.
//

#include "svpwm.h"
#include "transform.h"


void svpwm_init(svpwmtype *svpwmtype1, float udc, float Ts)
{
    svpwmtype1->svpwm_udc = udc;
    svpwmtype1->svpwm_Ts = Ts;
    svpwmtype1->svpwm_Tx = 0;
    svpwmtype1->svpwm_Ty = 0;
    svpwmtype1->svpwm_T0 = 0;
    svpwmtype1->svpwm_ua = 0;
    svpwmtype1->svpwm_ub = 0;
    svpwmtype1->svpwm_uc = 0;
    svpwmtype1->svpwm_ualpha = 0;
    svpwmtype1->svpwm_ubeta = 0;
    svpwmtype1->svpwm_n = 1;
}

void svpwm_update(svpwmtype *svpwmtype1, float ualpha, float ubeta)
{
    svpwmtype1->svpwm_ualpha = ualpha;
    svpwmtype1->svpwm_ubeta = ubeta;
    svpwm_sector(svpwmtype1);
    gen_svpwm(svpwmtype1);
}

void svpwm_sector(svpwmtype *svpwmtype1)
{
    float u1 = svpwmtype1->svpwm_ubeta;
    float u2 = sqrt3_2 * svpwmtype1->svpwm_ualpha - 0.5f * svpwmtype1->svpwm_ubeta;
    float u3 = -1.0f * sqrt3_2 * svpwmtype1->svpwm_ualpha - 0.5f * svpwmtype1->svpwm_ubeta;
    unsigned char A;
    unsigned char B;
    unsigned char C;
    if (u1 > 0)
    {
        A = 1;
    }
    else
    {
        A = 0;
    }
    if (u2 > 0)
    {
        B = 1;
    }
    else
    {
        B = 0;
    }
    if (u3 > 0)
    {
        C = 1;
    }
    else
    {
        C = 0;
    }
    svpwmtype1->svpwm_n = A + 2 * B + 4 * C;
}

void gen_svpwm(svpwmtype *svpwmtype1)
{
    switch (svpwmtype1->svpwm_n)
    {
        case 3:
        {
            svpwmtype1->svpwm_Tx =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (sqrt3_2 * svpwmtype1->svpwm_ualpha - 0.5f * svpwmtype1->svpwm_ubeta) /
                    svpwmtype1->svpwm_udc;
            svpwmtype1->svpwm_Ty =
                    sqrt3 * svpwmtype1->svpwm_Ts * svpwmtype1->svpwm_ubeta / svpwmtype1->svpwm_udc;
            if(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty>svpwmtype1->svpwm_Ts)
            {
                svpwmtype1->svpwm_Tx = (svpwmtype1->svpwm_Tx*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
                svpwmtype1->svpwm_Ty = (svpwmtype1->svpwm_Ty*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
            }
            svpwmtype1->svpwm_ua = (svpwmtype1->svpwm_Ts - svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_ub = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_uc = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx + svpwmtype1->svpwm_Ty) / 4.0f;
            break;
        }
        case 1:
        {
            svpwmtype1->svpwm_Tx =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (0.5f * svpwmtype1->svpwm_ubeta - sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            svpwmtype1->svpwm_Ty =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (0.5f * svpwmtype1->svpwm_ubeta + sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            if(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty>svpwmtype1->svpwm_Ts)
            {
                svpwmtype1->svpwm_Tx = (svpwmtype1->svpwm_Tx*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
                svpwmtype1->svpwm_Ty = (svpwmtype1->svpwm_Ty*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
            }
            svpwmtype1->svpwm_ua = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_ub = (svpwmtype1->svpwm_Ts - svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_uc = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx + svpwmtype1->svpwm_Ty) / 4.0f;
            break;
        }
        case 5:
        {
            svpwmtype1->svpwm_Tx =
                    sqrt3 * svpwmtype1->svpwm_Ts * svpwmtype1->svpwm_ubeta / svpwmtype1->svpwm_udc;
            svpwmtype1->svpwm_Ty =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (-0.5f * svpwmtype1->svpwm_ubeta - sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            if(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty>svpwmtype1->svpwm_Ts)
            {
                svpwmtype1->svpwm_Tx = (svpwmtype1->svpwm_Tx*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
                svpwmtype1->svpwm_Ty = (svpwmtype1->svpwm_Ty*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
            }
            svpwmtype1->svpwm_ua = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx + svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_ub = (svpwmtype1->svpwm_Ts - svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_uc = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            break;
        }
        case 4:
        {
            svpwmtype1->svpwm_Tx =
                    sqrt3 * svpwmtype1->svpwm_Ts * -1.0f * svpwmtype1->svpwm_ubeta / svpwmtype1->svpwm_udc;
            svpwmtype1->svpwm_Ty =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (0.5f * svpwmtype1->svpwm_ubeta - sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            if(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty>svpwmtype1->svpwm_Ts)
            {
                svpwmtype1->svpwm_Tx = (svpwmtype1->svpwm_Tx*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
                svpwmtype1->svpwm_Ty = (svpwmtype1->svpwm_Ty*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
            }
            svpwmtype1->svpwm_ua = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx + svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_ub = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_uc = (svpwmtype1->svpwm_Ts - svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            break;
        }
        case 6:
        {
            svpwmtype1->svpwm_Tx =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (-0.5f * svpwmtype1->svpwm_ubeta - sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            svpwmtype1->svpwm_Ty =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (-0.5f * svpwmtype1->svpwm_ubeta + sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            if(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty>svpwmtype1->svpwm_Ts)
            {
                svpwmtype1->svpwm_Tx = (svpwmtype1->svpwm_Tx*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
                svpwmtype1->svpwm_Ty = (svpwmtype1->svpwm_Ty*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
            }
            svpwmtype1->svpwm_ua = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_ub = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx + svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_uc = (svpwmtype1->svpwm_Ts - svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            break;
        }
        case 2:
        {
            svpwmtype1->svpwm_Tx =
                    sqrt3 * svpwmtype1->svpwm_Ts *
                    (0.5f * svpwmtype1->svpwm_ubeta + sqrt3_2 * svpwmtype1->svpwm_ualpha) /
                    svpwmtype1->svpwm_udc;
            svpwmtype1->svpwm_Ty =
                    sqrt3 * -1.0f * svpwmtype1->svpwm_Ts * svpwmtype1->svpwm_ubeta / svpwmtype1->svpwm_udc;
            if(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty>svpwmtype1->svpwm_Ts)
            {
                svpwmtype1->svpwm_Tx = (svpwmtype1->svpwm_Tx*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
                svpwmtype1->svpwm_Ty = (svpwmtype1->svpwm_Ty*svpwmtype1->svpwm_Ts)/(svpwmtype1->svpwm_Tx+svpwmtype1->svpwm_Ty);
            }
            svpwmtype1->svpwm_ua = (svpwmtype1->svpwm_Ts - svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_ub = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx + svpwmtype1->svpwm_Ty) / 4.0f;
            svpwmtype1->svpwm_uc = (svpwmtype1->svpwm_Ts + svpwmtype1->svpwm_Tx - svpwmtype1->svpwm_Ty) / 4.0f;
            break;
        }
    }
}