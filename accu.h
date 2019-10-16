#ifndef MARS_ACCU_H
#define MARS_ACCU_H

#include "pairing.h"

void acc_gen_1(int i);
void acc_gen_2(int i);
void acc_revoke_1(int i);
void acc_revoke_2(int i);
G1 wit_gen_1(int j);
G2 wit_gen_2(int j);
bool acc_verify_1(G1 g_1_i,G1 wit_1);
bool acc_verify_2(G2 g_4_i,G2 wit_2);

#endif //MARS_ACCU_H
