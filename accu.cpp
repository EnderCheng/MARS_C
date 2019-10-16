#include "accu.h"

void acc_gen_1(int i)
{
    accu_params.U1.insert(i);
    accu_params.acc_U1 = accu_params.acc_U1 + accu_params.g_2[100+1-i-1];
}

void acc_gen_2(int i)
{
    accu_params.U2.insert(i);
    accu_params.acc_U2 = accu_params.acc_U2 + accu_params.g_3[100+1-i-1];
}

void acc_revoke_1(int i)
{
    accu_params.U1.erase(i);
    G2 tmp = accu_params.acc_U1 +(-accu_params.g_2[100+1-i-1]);
    accu_params.acc_U1 = tmp;
}

void acc_revoke_2(int i)
{
    accu_params.U2.erase(i);
    G1 tmp= accu_params.acc_U2 +(- accu_params.g_3[100+1-i-1]);
    accu_params.acc_U2 = tmp;
}


G1 wit_gen_1(int j)
{
    Big order = pfc.order();
    G1 wit = pfc.mult(g1, Big(0));
    set<int>::iterator it;
    for (it = accu_params.U1.begin(); it != accu_params.U1.end(); it++) {
        int i = *it;
        if (i != j) {
            wit = wit + accu_params.g_1[100 + 1 - i + j - 1];
        }
    }

    return wit;
}

G2 wit_gen_2(int j)
{
    //Big order = pfc.order();
    G2 wit = pfc.mult(g2, Big(0));
    set<int>::iterator it;
    for (it = accu_params.U2.begin(); it != accu_params.U2.end(); it++) {
        int i = *it;
        if (i != j) {
            wit = wit + accu_params.g_4[100 + 1 - i + j - 1];
        }
    }

    return wit;
}

bool acc_verify_1(G1 g_1_i,G1 wit_1)
{
    GT test = pfc.pairing(accu_params.acc_U1, g_1_i)/pfc.pairing(g2, wit_1);
    if(test != accu_params.eta_U1)
        return false;
    else
        return true;
}

bool acc_verify_2(G2 g_4_i,G2 wit_2)
{
    GT test = pfc.pairing(g_4_i, accu_params.acc_U2)/pfc.pairing(wit_2,g1);
    if(test != accu_params.eta_U2)
        return false;
    else
        return true;
}
