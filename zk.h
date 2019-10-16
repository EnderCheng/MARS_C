#ifndef MARS_ZK_H
#define MARS_ZK_H

#include <vector>
#include "pairing.h"
#include "groth.h"

struct receipt
{
    GrothSig_1 vcred;
    GrothSig_2 tcred;
    G1 vk, attr_1, attr_2;
    G2 info;
};

struct Proof{
    G2 P_1;
    G2 P_2;
    G2 P_3;

    GT e_1;
    GT e_2;
    GT e_3;
    GT e_4;

    G2 e_p_1;
    G2 e_p_2;
    G1 e_a_1;
    G1 e_a_2;

    Big s_f_1;
    Big s_f_2;
    Big s_x;

    G1 t_g_3i;
    G2 t_g_4i;
    G2 t_vk_u;
    G1 t_Q_1;
    G1 t_Q_2;

    G1 a_1;
    G1 a_2;

    vector<GT> c1,c2,c3,c4,c5,c6,c7,c8,c10,c11,e_wits;
    vector<G1> t1,t2,t3,t4,t9,t10,t11,tcred_r, tcred_r_x,tcred_r_x_tmp,t_wits;
    vector<G2> t5,t6,t7,t8,t12,vcred_r;


    GT u_1;
    GT u_2;
    G2 u_sig_r;
    G1 t_S;
    G1 t_T;
    GT e_wit_u;
    G2 t_wit_u;
};

Proof prove(G2 rho, G2 theta, G2 h, G1 Gamma, Big f_1, Big f_2,
            G1 g_3i, G2 g_4i, G1 Q_1, G1 Q_2, G2 P_1, G2 P_2, G2 P_3, vector<receipt> receipts,
            G2 vk_u, GrothSig_1 sig_msg, vector<G1> wits_v, G2 wit_u);
bool verify(G2 mpk, G2 rho, G2 theta, G2 h, G1 Gamma, G2 P_1, G2 P_2, G2 P_3, Proof pi, G1 msg);

#endif //MARS_ZK_H
