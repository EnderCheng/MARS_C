#include "pairing.h"

PFC pfc(AES_SECURITY);
G1 g1;
G2 g2;
GrothParams g_params;
AccuParams accu_params;

void initialize_generators()
{
    pfc.random(g1);
    pfc.random(g2);
}

void initialize_groth_params()
{
    for(int i=0;i<3;i++)
    {
        pfc.random(g_params.y_1[i]);
        pfc.random(g_params.y_2[i]);
    }
}

void initialize_accu_params()
{
    Big order = pfc.order();
    pfc.random(accu_params.alpha);
    pfc.random(accu_params.beta);
    accu_params.acc_U1 = pfc.mult(g2,Big(0));
    accu_params.acc_U2 = pfc.mult(g1,Big(0));

    GT eta = pfc.pairing(g2,g1);
    accu_params.eta_U1 = pfc.power(eta,pow(accu_params.alpha,101,order));
    accu_params.eta_U2 = pfc.power(eta,pow(accu_params.beta,101,order));
    for(int i=1;i<=200;i++)
    {
        if(i!=101)
        {
            accu_params.g_1[i-1] = pfc.mult(g1,pow(accu_params.alpha,i,order));
            accu_params.g_3[i-1] = pfc.mult(g1,pow(accu_params.beta,i,order));
        }

    }
    for(int i=1;i<=200;i++)
    {
        if(i!=101)
        {
            accu_params.g_2[i - 1] = pfc.mult(g2, pow(accu_params.alpha, i, order));
            accu_params.g_4[i - 1] = pfc.mult(g2, pow(accu_params.beta, i, order));
        }
    }
}