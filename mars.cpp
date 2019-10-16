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


    auto begin = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
//    cout << "Time:" << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;
    int num_vendors = 10;
    int num_users = 1;

    /******************Setup******************/
    //Trust Authority
    begin = chrono::high_resolution_clock::now();
    initialize_generators();
    initialize_groth_params();
    Big order = pfc.order();
    Big gamma, x_1, x_2;
    pfc.random(gamma);
    G1 Gamma = pfc.mult(g1, gamma);
    pfc.random(x_1);
    pfc.random(x_2);
    G2 h, rho, theta;
    pfc.random(h);
    rho = pfc.mult(h, inverse(x_1, order));
    theta = pfc.mult(h, inverse(x_2, order));
    end = chrono::high_resolution_clock::now();
    cout << "TA Setup Duration:"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;


    //Rating Platform
    begin = chrono::high_resolution_clock::now();
    initialize_accu_params();
    Big msk;
    pfc.random(msk);
    G2 mpk = pfc.mult(g2, msk);
    end = chrono::high_resolution_clock::now();
    cout << "Rating Platform Setup Duration:"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << endl;

    /******************Registration******************/

    //Rating Platform -> Vendor
    begin = chrono::high_resolution_clock::now();
    vector<RegVendor> vendors;
    for (int i = 0; i < num_vendors; i++) {
        GrothKey_2 v_keys = groth_gen_2();
        G1 v_attr_1, v_attr_2;
        pfc.random(v_attr_1);
        v_attr_2 = accu_params.g_1[i];
        acc_gen_1(i + 1);
        G1 M_1[3];
        M_1[0] = v_keys.vk;
        M_1[1] = v_attr_1;
        M_1[2] = v_attr_2;
        GrothSig_1 vcred = groth_sign_1(M_1, msk, 3);
        RegVendor new_vendor;
        new_vendor.vcred = vcred;
        new_vendor.v_keys = v_keys;
        new_vendor.v_attr_1 = v_attr_1;
        new_vendor.v_attr_2 = v_attr_2;
        vendors.push_back(new_vendor);
    }
    end = chrono::high_resolution_clock::now();
    cout << "Vendor Registration Duration:"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_vendors << endl;


    //Rating Platform -> User
    begin = chrono::high_resolution_clock::now();
    vector<RegUser> users;
    for (int i = 0; i < num_users; i++) {
        G2 g_4i = accu_params.g_4[i];
        acc_gen_2(i + 1);
        G2 ID_i = pfc.mult(g2, inverse(gamma + pow(accu_params.beta, i + 1, order), order));
        GrothKey_1 u_keys = groth_gen_1();
        RegUser new_user;
        new_user.g_i = g_4i;
        new_user.ID_i = ID_i;
        new_user.u_keys = u_keys;
        users.push_back(new_user);
    }
    end = chrono::high_resolution_clock::now();
    cout << "User Registration Duration:"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_users << endl;

    /******************Online Transaction******************/

    //vendors
    begin = chrono::high_resolution_clock::now();
    vector<receipt> receipts;
    for (int j = 0; j < num_users; j++) {
        G2 vk = users[j].u_keys.vk;
        G2 g_i = users[j].g_i;
        for (int i = 0; i < num_vendors; i++) {
            G2 info;
            pfc.random(info);
            G2 M_u[3];
            M_u[0] = vk;
            M_u[1] = info;
            M_u[2] = g_i;
            GrothSig_2 tcred = groth_sign_2(M_u, vendors[i].v_keys.sk, 3);
            receipt rept;
            rept.vcred = vendors[i].vcred;
            rept.tcred = tcred;
            rept.vk = vendors[i].v_keys.vk;
            rept.attr_1 = vendors[i].v_attr_1;
            rept.attr_2 = vendors[i].v_attr_2;
            rept.info = info;
            receipts.push_back(rept);
        }
    }
    end = chrono::high_resolution_clock::now();
    cout << "Transaction Duration (Vendor):"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / (num_users * num_vendors)
         << endl;
    /******************Anonymous Rating******************/

    //User
    begin = chrono::high_resolution_clock::now();
    int num_repts = receipts.size();
    for (int j = 0; j < num_users; j++) {
        G2 vk = users[j].u_keys.vk;
        G2 g_i = users[j].g_i;
        for (int i = 0; i < num_repts; i++) {
            GrothSig_1 vcred = receipts[i].vcred;
            GrothSig_2 tcred = receipts[i].tcred;
            G1 M_1[3];
            M_1[0] = receipts[i].vk;
            M_1[1] = receipts[i].attr_1;
            M_1[2] = receipts[i].attr_2;
            bool rst_1 = groth_verify_1(M_1, mpk, vcred, 3);
            if (!rst_1)
                cout << "Vendor Auth Fail" << endl;

            G2 M_u[3];
            M_u[0] = vk;
            M_u[1] = receipts[i].info;
            M_u[2] = g_i;

            bool rst_2 = groth_verify_2(M_u, receipts[i].vk, tcred, 3);
            if (!rst_2)
                cout << "Transaction Auth Fail" << endl;
        }
    }
    end = chrono::high_resolution_clock::now();
    cout << "Transaction Duration (User):"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / (num_repts * num_users)
         << endl;


    /******************Anonymous Rating******************/

    G1 msg;
    pfc.random(msg);

    //User
    begin = chrono::high_resolution_clock::now();
    vector<Proof> proofs;
    for (int j = 0; j < num_users; j++) {
        G2 ID_i = users[j].ID_i;
        G2 vk = users[j].u_keys.vk;
        G2 g_4i = users[j].g_i;
        Big sk = users[j].u_keys.sk;
        vector<G1> wits_v;
        G1 M_rev[1];
        M_rev[0] = msg;
        GrothSig_1 sig_msg = groth_sign_1(M_rev, sk, 1);
        for (int i = 0; i < num_repts; i++) {
            G1 wit_v = wit_gen_1(i + 1);
            wits_v.push_back(wit_v);
        }
        G2 wit_u = wit_gen_2(j + 1);
        Big f_1, f_2;
        pfc.random(f_1);
        pfc.random(f_2);
        G1 g_3i = accu_params.g_3[j];

        G2 P_1 = pfc.mult(rho, f_1);
        G2 P_2 = pfc.mult(theta, f_2);
        G2 P_3 = ID_i + pfc.mult(h, f_1 + f_2);
        G1 Q_1 = pfc.mult(g_3i, f_1);
        G1 Q_2 = pfc.mult(g_3i, f_2);

        Proof pi = prove(rho, theta, h, Gamma, f_1, f_2, g_3i, g_4i, Q_1, Q_2, P_1, P_2, P_3, receipts,
                         vk, sig_msg, wits_v, wit_u);
        proofs.push_back(pi);
    }
    end = chrono::high_resolution_clock::now();
    cout << "Rating Duration (User):"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_users << endl;

    //Rating Platform
    begin = chrono::high_resolution_clock::now();
    for (int j = 0; j < num_users; j++) {
        bool rst = verify(mpk, rho, theta, h, Gamma, proofs[j].P_1, proofs[j].P_2, proofs[j].P_3, proofs[j], msg);
        if (!rst)
            cout << "Proof Error" << endl;
    }
    end = chrono::high_resolution_clock::now();
    cout << "Rating Duration (Rating Platform):"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_users << endl;

    /******************Revocation******************/
    begin = chrono::high_resolution_clock::now();
    for (int j = 0; j < num_vendors; j++) {
        acc_revoke_1(j + 1);
    }
    end = chrono::high_resolution_clock::now();
    cout << "Vendor Revocation Duration:"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_vendors << endl;

    begin = chrono::high_resolution_clock::now();
    for (int j = 0; j < num_users; j++) {
        acc_revoke_2(j + 1);
    }
    end = chrono::high_resolution_clock::now();
    cout << "User Revocation Duration:"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_users << endl;

    /******************Updating******************/


    /******************Tracing******************/
    begin = chrono::high_resolution_clock::now();
    for (int j = 0; j < num_users; j++) {
        G2 ID_i = users[j].ID_i;
        G2 ID = proofs[j].P_3 + (-(pfc.mult(proofs[j].P_1, x_1) + pfc.mult(proofs[j].P_2, x_2)));
        if (ID != ID_i)
            cout << "Identity Recover Error" << endl;
    }
    end = chrono::high_resolution_clock::now();
    cout << "Tracing Duration (TA):"
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / num_users << endl;
    return 0;

}

