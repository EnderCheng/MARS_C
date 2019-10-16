#include "zk.h"
#include <map>

Proof prove(G2 rho, G2 theta, G2 h, G1 Gamma, Big f_1, Big f_2,
            G1 g_3i, G2 g_4i, G1 Q_1, G1 Q_2, G2 P_1, G2 P_2, G2 P_3, vector<receipt> receipts,
            G2 vk_u, GrothSig_1 sig_msg, vector<G1> wits_v, G2 wit_u) {
    Big order = pfc.order();
    G1 inv_gamma = -Gamma;

    G1 a_1 = pfc.mult(inv_gamma, f_1);
    G1 a_2 = pfc.mult(inv_gamma, f_2);

    Big f_1_tmp, f_2_tmp, x, x_tmp;
    pfc.random(f_1_tmp);
    pfc.random(f_2_tmp);
    pfc.random(x);
    pfc.random(x_tmp);
    G1 g_3i_tmp, Q_1_tmp, Q_2_tmp;
    G2 g_4i_tmp, vk_u_tmp;
    pfc.random(g_3i_tmp);
    pfc.random(g_4i_tmp);
    pfc.random(Q_1_tmp);
    pfc.random(Q_2_tmp);
    pfc.random(vk_u_tmp);

    G2 e_p_1 = pfc.mult(rho, f_1_tmp);
    G2 e_p_2 = pfc.mult(theta, f_2_tmp);
    G1 e_a_1 = pfc.mult(inv_gamma, f_1_tmp);
    G1 e_a_2 = pfc.mult(inv_gamma, f_2_tmp);

    pfc.precomp_for_pairing(P_1);
    pfc.precomp_for_pairing(P_2);
    pfc.precomp_for_pairing(P_3);
    pfc.precomp_for_pairing(g2);

    pfc.precomp_for_pairing(rho);
    pfc.precomp_for_pairing(theta);
    pfc.precomp_for_pairing(h);
    pfc.precomp_for_pairing(g_4i_tmp);

    GT e_1 = pfc.pairing(P_1, g_3i_tmp) / pfc.pairing(rho, Q_1_tmp);
    GT e_2 = pfc.pairing(P_2, g_3i_tmp) / pfc.pairing(theta, Q_2_tmp);
    GT e_3 = pfc.pairing(P_3, g_3i_tmp) / pfc.pairing(h, Q_1_tmp + Q_2_tmp);
    GT e_4 = pfc.pairing(g2, g_3i_tmp) / pfc.pairing(g_4i_tmp, g1);

    pfc.start_hash();
    pfc.add_to_hash(e_p_1);
    pfc.add_to_hash(e_p_2);
    pfc.add_to_hash(e_a_1);
    pfc.add_to_hash(e_a_2);
    pfc.add_to_hash(e_1);
    pfc.add_to_hash(e_2);
    pfc.add_to_hash(e_3);
    pfc.add_to_hash(e_4);
    Big ch = pfc.finish_hash_to_group();

    Big s_f_1 = (f_1_tmp + modmult(ch, f_1, order)) % order; //4G_1 exp + 2G_2 exp + 8 PAIR
    Big s_f_2 = (f_2_tmp + modmult(ch, f_2, order)) % order;
    Big s_x = (x_tmp + modmult(ch, x, order)) % order;

    G1 t_g_3i = g_3i_tmp + pfc.mult(g_3i, ch);
    G2 t_g_4i = g_4i_tmp + pfc.mult(g_4i, ch);
    G2 t_vk_u = vk_u_tmp + pfc.mult(vk_u, ch);
    G1 t_Q_1 = Q_1_tmp + pfc.mult(Q_1, ch);
    G1 t_Q_2 = Q_2_tmp + pfc.mult(Q_2, ch);

    vector<GT> c1,c2,c3,c4,c5,c6,c7,c8,c10,c11,e_wits;
    vector<G1> t1,t2,t3,t4,t9,t10,t11,tcred_r, tcred_r_x,tcred_r_x_tmp,t_wits;
    vector<G2> t5,t6,t7,t8,t12,vcred_r;

    pfc.precomp_for_pairing(g_params.y_2[0]);
    pfc.precomp_for_pairing(g_params.y_2[1]);
    pfc.precomp_for_pairing(g_params.y_2[2]);
    for (int i = 0; i < receipts.size(); ++i) {

        GrothSig_1 vcred = receipts[i].vcred;
        GrothSig_2 tcred = receipts[i].tcred;

        vcred = groth_randomize_1(vcred, 3);
        tcred = groth_randomize_2(tcred, 3);

        G1 vk_v = receipts[i].vk;
        G1 attr_1 = receipts[i].attr_1;
        G1 attr_2 = receipts[i].attr_2;
        G2 info = receipts[i].info;

        G1 tcred_R_x = pfc.mult(tcred.R, x);
        tcred_r_x.push_back(tcred_R_x);
        G1 tcred_R_x_tmp = pfc.mult(tcred.R, x_tmp);
        tcred_r_x_tmp.push_back(tcred_R_x_tmp);

        G1 vcred_S_tmp, vcred_T1_tmp, vcred_T2_tmp, vcred_T3_tmp;
        pfc.random(vcred_S_tmp);
        pfc.random(vcred_T1_tmp);
        pfc.random(vcred_T2_tmp);
        pfc.random(vcred_T3_tmp);

        G2 tcred_S_tmp, tcred_T1_tmp, tcred_T2_tmp, tcred_T3_tmp;
        pfc.random(tcred_S_tmp);
        pfc.random(tcred_T1_tmp);
        pfc.random(tcred_T2_tmp);
        pfc.random(tcred_T3_tmp);

        G1 vk_v_tmp, attr_1_tmp, attr_2_tmp;
        G2 info_tmp;
        pfc.random(vk_v_tmp);
        pfc.random(attr_1_tmp);
        pfc.random(attr_2_tmp);
        pfc.random(info_tmp);

        pfc.precomp_for_pairing(tcred.S);
        pfc.precomp_for_pairing(tcred_S_tmp);
        pfc.precomp_for_pairing(vcred.R);
        pfc.precomp_for_pairing(tcred_T1_tmp);
        pfc.precomp_for_pairing(tcred_T2_tmp);
        pfc.precomp_for_pairing(tcred_T3_tmp);

        GT c_10 = pfc.pairing(tcred.S, tcred_R_x);
        c10.push_back(c_10);

        GT c_11 = pfc.pairing(tcred_S_tmp, tcred_R_x);
        c11.push_back(c_11);

        GT c_1 = pfc.pairing(vcred.R, vcred_S_tmp);
        c1.push_back(c_1);
        GT c_2 = pfc.pairing(vcred.R, vcred_T1_tmp) / pfc.pairing(g2, vk_v_tmp);
        c2.push_back(c_2);
        GT c_3 = pfc.pairing(vcred.R, vcred_T2_tmp) / pfc.pairing(g2, attr_1_tmp);
        c3.push_back(c_3);
        GT c_4 = pfc.pairing(vcred.R, vcred_T3_tmp) / pfc.pairing(g2, attr_2_tmp);
        c4.push_back(c_4);

        GT c_5 = pfc.pairing(tcred_S_tmp, tcred.R)  / pfc.pairing(g2, vk_v_tmp);
        c5.push_back(c_5);
        GT c_6 = (pfc.pairing(tcred_T1_tmp, tcred.R) / pfc.pairing(g_params.y_2[0], vk_v_tmp)) /
                 pfc.pairing(vk_u_tmp, g1);
        c6.push_back(c_6);
        GT c_7 = (pfc.pairing(tcred_T2_tmp, tcred.R) / pfc.pairing(g_params.y_2[1], vk_v_tmp)) /
                 pfc.pairing(info_tmp, g1);
        c7.push_back(c_7);
        GT c_8 = (pfc.pairing(tcred_T3_tmp, tcred.R) / pfc.pairing(g_params.y_2[2], vk_v_tmp)) /
                 pfc.pairing(g_4i_tmp, g1);
        c8.push_back(c_8);

        G1 t_1 = vcred_S_tmp + pfc.mult(vcred.S, ch);
        t1.push_back(t_1);
        G1 t_2 = vcred_T1_tmp + pfc.mult(vcred.T[0], ch);
        t2.push_back(t_2);
        G1 t_3 = vcred_T2_tmp + pfc.mult(vcred.T[1], ch);
        t3.push_back(t_3);
        G1 t_4 = vcred_T3_tmp + pfc.mult(vcred.T[2], ch);
        t4.push_back(t_4);

        G2 t_5 = tcred_S_tmp + pfc.mult(tcred.S, ch);
        t5.push_back(t_5);
        G2 t_6 = tcred_T1_tmp + pfc.mult(tcred.T[0], ch);
        t6.push_back(t_6);
        G2 t_7 = tcred_T2_tmp + pfc.mult(tcred.T[1], ch);
        t7.push_back(t_7);
        G2 t_8 = tcred_T3_tmp + pfc.mult(tcred.T[2], ch);
        t8.push_back(t_8);

        G1 t_9 = vk_v_tmp + pfc.mult(vk_v, ch);
        t9.push_back(t_9);
        G1 t_10 = attr_1_tmp + pfc.mult(attr_1, ch);
        t10.push_back(t_10);
        G1 t_11 = attr_2_tmp + pfc.mult(attr_2, ch);
        t11.push_back(t_11);
        G2 t_12 = info_tmp + pfc.mult(info, ch);
        t12.push_back(t_12);

        vcred_r.push_back(vcred.R);
        tcred_r.push_back(tcred.R);

        G1 wit_v = wits_v[i];
        G1 wit_v_tmp;
        pfc.random(wit_v_tmp);
        GT e_wit = pfc.pairing(accu_params.acc_U1, attr_2_tmp) / pfc.pairing(g2, wit_v_tmp);
        e_wits.push_back(e_wit);
        G1 t_wit;
        if(wit_v == pfc.mult(g1, Big(0)))
        {
            t_wit = wit_v_tmp + wit_v;
        }
        else
        {
            t_wit = wit_v_tmp + pfc.mult(wit_v, ch);
        }
        t_wits.push_back(t_wit);


    }

    //

    GrothSig_1 sig_msg_r = groth_randomize_1(sig_msg, 1);
    G1 u_S_tmp, u_T_tmp;
    pfc.random(u_S_tmp);
    pfc.random(u_T_tmp);

    G1 t_S = u_S_tmp + pfc.mult(sig_msg_r.S, ch);
    G1 t_T = u_T_tmp + pfc.mult(sig_msg_r.T[0], ch);

    pfc.precomp_for_pairing(sig_msg_r.R);
    pfc.precomp_for_pairing(vk_u_tmp);

    GT u_1 = pfc.pairing(sig_msg_r.R, u_S_tmp) / pfc.pairing(vk_u_tmp, g1);
    GT u_2 = pfc.pairing(sig_msg_r.R, u_T_tmp) / pfc.pairing(vk_u_tmp, g_params.y_1[0]);

    G2 wit_u_tmp;
    pfc.random(wit_u_tmp);
    GT e_wit_u = pfc.pairing(g_4i_tmp, accu_params.acc_U2) / pfc.pairing(wit_u_tmp, g1);
    G2 t_wit_u;
    G2 tmp_wit_0 = pfc.mult(g2, Big(0));
    if(wit_u == tmp_wit_0)
    {
        t_wit_u = wit_u_tmp + wit_u;
    }
    else
    {
        t_wit_u = wit_u_tmp + pfc.mult(wit_u, ch);
    }

    //store proof
    Proof pi;
    pi.P_1 = P_1;
    pi.P_2 = P_2;
    pi.P_3 = P_3;

    pi.e_1 = e_1;
    pi.e_2 = e_2;
    pi.e_3 = e_3;
    pi.e_4 = e_4;

    pi.e_p_1 = e_p_1;
    pi.e_p_2 = e_p_2;
    pi.e_a_1 = e_a_1;
    pi.e_a_2 = e_a_2;

    pi.s_f_1 = s_f_1;
    pi.s_f_2 = s_f_2;
    pi.s_x = s_x;

    pi.t_g_3i = t_g_3i;
    pi.t_g_4i = t_g_4i;
    pi.t_vk_u = t_vk_u;
    pi.t_Q_1 = t_Q_1;
    pi.t_Q_2 = t_Q_2;

    pi.a_1 = a_1;
    pi.a_2 = a_2;

    pi.c1 = c1;
    pi.c2 = c2;
    pi.c3 = c3;
    pi.c4 = c4;
    pi.c5 = c5;
    pi.c6 = c6;
    pi.c7 = c7;
    pi.c8 = c8;
    pi.c10 = c10;
    pi.c11 = c11;
    pi.e_wits = e_wits;
    pi.t1 = t1;
    pi.t2 = t2;
    pi.t3 = t3;
    pi.t4 = t4;
    pi.t5 = t5;
    pi.t6 = t6;
    pi.t7 = t7;
    pi.t8 = t8;
    pi.t9 = t9;
    pi.t10 = t10;
    pi.t11 = t11;
    pi.t12 = t12;
    pi.vcred_r = vcred_r;
    pi.tcred_r = tcred_r;
    pi.t_wits = t_wits;
    pi.tcred_r_x_tmp = tcred_r_x_tmp;
    pi.tcred_r_x = tcred_r_x;

    pi.u_1 = u_1;
    pi.u_2 = u_2;
    pi.u_sig_r = sig_msg_r.R;
    pi.t_S = t_S;
    pi.t_T = t_T;
    pi.e_wit_u = e_wit_u;
    pi.t_wit_u = t_wit_u;

    return pi;


}

bool verify(G2 mpk, G2 rho, G2 theta, G2 h, G1 Gamma, G2 P_1, G2 P_2, G2 P_3, Proof pi, G1 msg) {
    pfc.start_hash();
    pfc.add_to_hash(pi.e_p_1);
    pfc.add_to_hash(pi.e_p_2);
    pfc.add_to_hash(pi.e_a_1);
    pfc.add_to_hash(pi.e_a_2);
    pfc.add_to_hash(pi.e_1);
    pfc.add_to_hash(pi.e_2);
    pfc.add_to_hash(pi.e_3);
    pfc.add_to_hash(pi.e_4);
    Big ch = pfc.finish_hash_to_group();

    G2 left = pfc.mult(rho, pi.s_f_1);
    G2 right = pi.e_p_1 + pfc.mult(P_1, ch);

    if (left != right) {
        cout << "1" << endl;
        return false;
    }

    left = pfc.mult(theta, pi.s_f_2);
    right = pi.e_p_2 + pfc.mult(P_2, ch);

    if (left != right) {
        cout << "2" << endl;
        return false;
    }

    Big order = pfc.order();
    pfc.precomp_for_pairing(P_1);
    GT left_t = pfc.pairing(P_1, pi.t_g_3i) / pfc.pairing(rho, pi.t_Q_1);
    GT right_t = pi.e_1;

    if (left_t != right_t) {
        cout << "3" << endl;
        return false;
    }
    pfc.precomp_for_pairing(P_2);
    left_t = pfc.pairing(P_2, pi.t_g_3i) / pfc.pairing(theta, pi.t_Q_2);
    right_t = pi.e_2;

    if (left_t != right_t) {
        cout << "4" << endl;
        return false;
    }

    G1 inv_gamma = -Gamma;

    G1 left_1 = pfc.mult(inv_gamma, pi.s_f_1);
    G1 right_1 = pi.e_a_1 + pfc.mult(pi.a_1, ch);

    if (left_1 != right_1) {
        cout << "5" << endl;
        return false;
    }

    left_1 = pfc.mult(inv_gamma, pi.s_f_2);
    right_1 = pi.e_a_2 + pfc.mult(pi.a_2, ch);

    if (left_1 != right_1) {
        cout << "6" << endl;
        return false;
    }
    pfc.precomp_for_pairing(g2);
    pfc.precomp_for_pairing(P_3);
    pfc.precomp_for_pairing(h);
    GT tmp = ((pfc.pairing(g2, g1) / pfc.pairing(P_3, Gamma)) / pfc.pairing(h, pi.a_1)) / pfc.pairing(h, pi.a_2);

    left_t = ((pfc.pairing(P_3, pi.t_g_3i) / pfc.pairing(h, pi.t_Q_1)) / pfc.pairing(h, pi.t_Q_2)) / pfc.power(tmp, ch);
    right_t = pi.e_3;

    if (left_t != right_t) {
        cout << "7" << endl;
        return false;
    }

    pfc.precomp_for_pairing(pi.t_g_4i);
    left_t = pfc.pairing(g2, pi.t_g_3i) / pfc.pairing(pi.t_g_4i, g1);
    right_t = pi.e_4;

    if (left_t != right_t) {
        cout << "8" << endl;
        return false;
    }

    pfc.precomp_for_pairing(mpk);
    pfc.precomp_for_pairing(pi.t_vk_u);
    pfc.precomp_for_pairing(g_params.y_2[0]);
    pfc.precomp_for_pairing(g_params.y_2[1]);
    pfc.precomp_for_pairing(g_params.y_2[2]);


    GT tmp_2 = pfc.pairing(mpk, g_params.y_1[0]);
    GT tmp_3 = pfc.pairing(mpk, g_params.y_1[1]);
    GT tmp_4 = pfc.pairing(mpk, g_params.y_1[2]);
    GT tmp_5 = pfc.pairing(g_params.y_2[0], g1);
    GT tmp_6 = pfc.pairing(g2, g_params.y_1[0]);
    GT tmp_1 = tmp_6 * pfc.pairing(mpk, g1);




    for (int i = 0; i < pi.c1.size(); ++i) {

        left_1 = pfc.mult(pi.tcred_r[i],pi.s_x);
        right_1 =pi.tcred_r_x_tmp[i]+pfc.mult(pi.tcred_r_x[i],ch);

        if (left_1 != right_1) {
            cout << "error R" << endl;
            return false;
        }

        left_t = pfc.pairing(pi.t5[i],pi.tcred_r_x[i]) / pfc.power(pi.c10[i], ch);
        right_t = pi.c11[i];

        if (left_t != right_t) {
            cout << "error (R,S)" << endl;
            return false;
        }


        left_t = (pfc.pairing(accu_params.acc_U1, pi.t11[i]) / pfc.pairing(g2, pi.t_wits[i])) /
                 pfc.power(accu_params.eta_U1, ch);
        right_t = pi.e_wits[i];

        if (left_t != right_t) {
            cout << "wit error" << endl;
            return false;
        }

        pfc.precomp_for_pairing(pi.vcred_r[i]);

        left_t = pfc.pairing(pi.vcred_r[i], pi.t1[i]) / pfc.power(tmp_1, ch);
        right_t = pi.c1[i];

        if (left_t != right_t) {
            cout << "9" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.vcred_r[i], pi.t2[i]) / pfc.pairing(g2, pi.t9[i])) / pfc.power(tmp_2, ch);
        right_t = pi.c2[i];

        if (left_t != right_t) {
            cout << "10" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.vcred_r[i], pi.t3[i]) / pfc.pairing(g2, pi.t10[i])) / pfc.power(tmp_3, ch);
        right_t = pi.c3[i];

        if (left_t != right_t) {
            cout << "11" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.vcred_r[i], pi.t4[i]) / pfc.pairing(g2, pi.t11[i])) / pfc.power(tmp_4, ch);
        right_t = pi.c4[i];

        if (left_t != right_t) {
            cout << "12" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.t5[i], pi.tcred_r[i]) / pfc.pairing(g2, pi.t9[i])) / pfc.power(tmp_5, ch);
        right_t = pi.c5[i];

        if (left_t != right_t) {
            cout << "13" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.t6[i], pi.tcred_r[i]) / pfc.pairing(g_params.y_2[0], pi.t9[i])) /
                 pfc.pairing(pi.t_vk_u, g1);
        right_t = pi.c6[i];

        if (left_t != right_t) {
            cout << "14" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.t7[i], pi.tcred_r[i]) / pfc.pairing(g_params.y_2[1], pi.t9[i])) /
                 pfc.pairing(pi.t12[i], g1);
        right_t = pi.c7[i];

        if (left_t != right_t) {
            cout << "15" << endl;
            return false;
        }

        left_t = (pfc.pairing(pi.t8[i], pi.tcred_r[i]) / pfc.pairing(g_params.y_2[2], pi.t9[i])) /
                 pfc.pairing(pi.t_g_4i, g1);  //
        right_t = pi.c8[i];

        if (left_t != right_t) {
            cout << "16" << endl;
            return false;
        }

    }

    left_t = (pfc.pairing(pi.u_sig_r, pi.t_S) / pfc.pairing(pi.t_vk_u, g1)) / pfc.power(tmp_6, ch);
    right_t = pi.u_1;

    if (left_t != right_t) {
        cout << "17" << endl;
        return false;
    }

    tmp = pfc.pairing(g2, msg);

    left_t = (pfc.pairing(pi.u_sig_r, pi.t_T) / pfc.pairing(pi.t_vk_u, g_params.y_1[0])) / pfc.power(tmp, ch);
    right_t = pi.u_2;

    if (left_t != right_t) {
        cout << "18" << endl;
        return false;
    }

    int num = pi.c1.size();

    for (int k_1 = 0; k_1 < num; k_1++) {
        for (int k_2 = k_1 + 1; k_2 < num; k_2++) {
            if (k_1 != k_2) {
                left_t = pi.c10[k_1];
                right_t = pi.c10[k_2];
                if (left_t == right_t) {
                    cout << "19" << endl;
                    return false;
                }
            }
        }
    }

    //4G_1 + 4G_2 + 4G_T+26PAIR + X(2G_1 + 0G_2 + 7G_T+21PAIR)

    //2x+4 G1 + 4 G_2 + 7X+4 G_T + 21X+26 PAIR

    left_t = (pfc.pairing(pi.t_g_4i, accu_params.acc_U2) / pfc.pairing(pi.t_wit_u, g1)) /
             pfc.power(accu_params.eta_U2, ch);
    right_t = pi.e_wit_u;

    if (left_t != right_t) {
        cout << "wit error user" << endl;
        return false;
    }

    return true;
}
