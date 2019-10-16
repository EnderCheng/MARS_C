#ifndef MARS_PAIRING_H
#define MARS_PAIRING_H

#define MR_PAIRING_BN
#define AES_SECURITY 192

#include "pairing_3.h"
#include<set>

struct GrothParams{
    G1 y_1[3];
    G2 y_2[3];
};

struct AccuParams{
    G2 acc_U1;
    G1 acc_U2;
    Big alpha, beta;
    GT eta_U1, eta_U2;
    G1 g_1[200];
    G2 g_2[200];
    G1 g_3[200];
    G2 g_4[200];
    set<int> U1;
    set<int> U2;
};

extern PFC pfc;
extern G1 g1;
extern G2 g2;
extern GrothParams g_params;
extern AccuParams accu_params;


void initialize_generators();
void initialize_groth_params();
void initialize_accu_params();

#endif //MARS_PAIRING_H
