#ifndef MARS_GROTH_H
#define MARS_GROTH_H

#include "pairing.h"

struct GrothSig_1{
    G2 R;
    G1 S;
    G1 T[3];
};

struct GrothSig_2{
    G1 R;
    G2 S;
    G2 T[3];
};

struct GrothKey_1{
    G2 vk;
    Big sk;
};

struct GrothKey_2{
    G1 vk;
    Big sk;
};

struct RegVendor{
    GrothSig_1 vcred;
    GrothKey_2 v_keys;
    G1 v_attr_1,v_attr_2;
};

struct RegUser{
    G2 g_i, ID_i;
    GrothKey_1 u_keys;
};

GrothKey_1 groth_gen_1();
GrothKey_2 groth_gen_2();

GrothSig_1 groth_sign_1(G1 M[], Big sk, int len);
GrothSig_2 groth_sign_2(G2 M[], Big sk, int len);

bool groth_verify_1(G1 M[], G2 vk, GrothSig_1 sig, int len);
bool groth_verify_2(G2 M[], G1 vk, GrothSig_2 sig, int len);

GrothSig_1 groth_randomize_1(GrothSig_1 sig, int len);
GrothSig_2 groth_randomize_2(GrothSig_2 sig, int len);

#endif //MARS_GROTH_H
