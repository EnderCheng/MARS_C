#include <iostream>
#include<ctime>

#include "pairing.h"
#include "groth.h"
#include "accu.h"
#include "zk.h"

int main() {
    time_t seed;
    time(&seed);
    irand((long) seed);

    cout << "Test MARS" << endl;
    initialize_generators();
    initialize_groth_params();
    initialize_accu_params();

/***Test Groth***/

//    GrothKey_1 key_1 = groth_gen_1();
//    GrothKey_2 key_2 = groth_gen_2();
//
//    G1 M_1[3];
//    G2 M_2[3];
//    for(int i=0;i<3;i++)
//    {
//        pfc.random(M_1[i]);
//        pfc.random(M_2[i]);
//    }
//
//    GrothSig_1 sig_1 = groth_sign_1(M_1,key_1.sk,3);
//    GrothSig_2 sig_2 = groth_sign_2(M_2,key_2.sk,3);
//
//    auto begin = chrono::high_resolution_clock::now();
//    bool rst_1 = groth_verify_1(M_1,key_1.vk,sig_1,3);
//    auto end = chrono::high_resolution_clock::now();
//    cout << "Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;
//
//    bool rst_2 = groth_verify_2(M_2,key_2.vk,sig_2,3);
//
//    if(rst_1 == true)
//        cout << "Sig_1 Verification Success" << endl;
//    else
//        cout << "Sig_1 Verification Fail" << endl;
//
//    if(rst_2 == true)
//        cout << "Sig_2 Verification Success" << endl;
//    else
//        cout << "Sig_2 Verification Fails" << endl;
//
//    GrothSig_1 sig_1_new = groth_randomize_1(sig_1,3);
//    GrothSig_2 sig_2_new = groth_randomize_2(sig_2,3);
//
//    bool rst_1_new = groth_verify_1(M_1,key_1.vk,sig_1_new,3);
//    bool rst_2_new = groth_verify_2(M_2,key_2.vk,sig_2_new,3);
//
//    if(rst_1_new == true)
//        cout << "Randomized Sig_1 Verification Success" << endl;
//    else
//        cout << "Randomized Sig_1 Verification Fail" << endl;
//
//    if(rst_2_new == true)
//        cout << "Randomized Sig_2 Verification Success" << endl;
//    else
//        cout << "Randomized Sig_2 Verification Fails" << endl;

/***Test Accumulator***/

//    acc_gen_1(1);
//    acc_gen_2(1);
//
//    G1 g_1_1 = accu_params.g_1[0];
//    G2 g_4_1 = accu_params.g_4[0];
//
//    G1 wit_1 = wit_gen_1(1);
//    G2 wit_2 = wit_gen_2(1);
//
//    rst_1 = acc_verify_1(g_1_1,wit_1);
//    rst_2 = acc_verify_2(g_4_1,wit_2);
//
//    if(rst_1 == true)
//        cout << "Accumulator 1 Success" << endl;
//    else
//        cout << "Accumulator 1 Fail" << endl;
//
//    if(rst_2 == true)
//        cout << "Accumulator 2 Success" << endl;
//    else
//        cout << "Accumulator 2 Fail" << endl;



/***Test Zeroknowlegde***/

    Big order = pfc.order();
    Big gamma,msk,x_1,x_2,f_1,f_2;
    pfc.random(gamma);
    pfc.random(msk);
    pfc.random(f_1);
    pfc.random(f_2);
    pfc.random(x_1);
    pfc.random(x_2);
    G1 Gamma = pfc.mult(g1,gamma);
    G2 mpk = pfc.mult(g2,msk);
    G2 h,rho,theta;
    pfc.random(h);
    rho = pfc.mult(h, inverse(x_1, order));
    theta = pfc.mult(h, inverse(x_2, order));
    G1 g_3i = accu_params.g_3[0];
    G2 g_4i = accu_params.g_4[0];
    acc_gen_2(1);
    G2 ID_i = pfc.mult(g2,inverse(gamma+pow(accu_params.beta,1,order),order));

    G2 P_1 = pfc.mult(rho,f_1);
    G2 P_2 = pfc.mult(theta,f_2);
    G2 P_3 = ID_i+pfc.mult(h,f_1+f_2);
    G1 Q_1 = pfc.mult(g_3i,f_1);
    G1 Q_2 = pfc.mult(g_3i,f_2);

    //vendor_1
    GrothKey_2 v_1_keys = groth_gen_2();
    G1 v_1_attr_1,v_1_attr_2;
    pfc.random(v_1_attr_1);
    v_1_attr_2 = accu_params.g_1[0];
    acc_gen_1(1);
    G1 M_1[3];
    M_1[0] = v_1_keys.vk;
    M_1[1] = v_1_attr_1;
    M_1[2] = v_1_attr_2;
    GrothSig_1 vcred_1 = groth_sign_1(M_1,msk,3);

    //vendor_1
    GrothKey_2 v_2_keys = groth_gen_2();
    G1 v_2_attr_1,v_2_attr_2;
    pfc.random(v_2_attr_1);
    v_2_attr_2 = accu_params.g_1[1];
    acc_gen_1(2);
    G1 M_2[3];
    M_2[0] = v_2_keys.vk;
    M_2[1] = v_2_attr_1;
    M_2[2] = v_2_attr_2;
    GrothSig_1 vcred_2 = groth_sign_1(M_2,msk,3);

    //user
    GrothKey_1 u_keys = groth_gen_1();

    //vendor_1_user
    G2 info_v_1;
    pfc.random(info_v_1);
    G2 M_u_1[3];
    M_u_1[0] = u_keys.vk;
    M_u_1[1] = info_v_1;
    M_u_1[2] = g_4i;
    GrothSig_2 tcred_1 = groth_sign_2(M_u_1,v_1_keys.sk,3);

    //vendor_2_user
    G2 info_v_2;
    pfc.random(info_v_2);
    G2 M_u_2[3];
    M_u_2[0] = u_keys.vk;
    M_u_2[1] = info_v_2;
    M_u_2[2] = g_4i;
    GrothSig_2 tcred_2 = groth_sign_2(M_u_2,v_2_keys.sk,3);

    receipt rept_1;
    rept_1.vcred = vcred_1;
    rept_1.tcred = tcred_1;
    rept_1.vk = v_1_keys.vk;
    rept_1.attr_1 = v_1_attr_1;
    rept_1.attr_2 = v_1_attr_2;
    rept_1.info = info_v_1;

    receipt rept_2;
    rept_2.vcred = vcred_2;
    rept_2.tcred = tcred_2;
    rept_2.vk = v_2_keys.vk;
    rept_2.attr_1 = v_2_attr_1;
    rept_2.attr_2 = v_2_attr_2;
    rept_2.info = info_v_2;

    vector<receipt> receipts;
    receipts.push_back(rept_1);
    receipts.push_back(rept_2);

    G1 msg;
    pfc.random(msg);
    G1 M_rev[1];
    M_rev[0] = msg;
    GrothSig_1 sig_msg = groth_sign_1(M_rev,u_keys.sk,1);

    G1 w_v_1 = wit_gen_1(1);
    G1 w_v_2 = wit_gen_1(2);
    vector<G1> wits_v;
    wits_v.push_back(w_v_1);
    wits_v.push_back(w_v_2);

    G2 wit_u = wit_gen_2(1);

    Proof pi = prove(rho, theta, h, Gamma, f_1, f_2,g_3i, g_4i, Q_1, Q_2, P_1, P_2, P_3, receipts,
            u_keys.vk, sig_msg, wits_v, wit_u);
    bool rst = verify(mpk,rho,theta,h,Gamma,P_1,P_2,P_3,pi,msg);

    cout << "verification:" << rst << endl;

}