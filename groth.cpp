#include "groth.h"

GrothKey_1 groth_gen_1()
{
    G2 vk;
    Big sk;
    GrothKey_1 key;

    pfc.random(sk);
    vk = pfc.mult(g2,sk);

    key.vk = vk;
    key.sk = sk;

    return key;
}

GrothKey_2 groth_gen_2()
{
    G1 vk;
    Big sk;
    GrothKey_2 key;

    pfc.random(sk);
    vk = pfc.mult(g1,sk);

    key.vk = vk;
    key.sk = sk;

    return key;
}

GrothSig_1 groth_sign_1(G1 M[], Big sk, int len)
{
    Big order=pfc.order();
    GrothSig_1 sig;
    Big r;
    pfc.random(r);
    sig.R = pfc.mult(g2,r);
    Big inv_r = inverse(r, order);
    sig.S = pfc.mult((g_params.y_1[0]+pfc.mult(g1,sk)),inv_r);
    for(int i=0;i<len;i++)
    {
        sig.T[i] = pfc.mult(pfc.mult(g_params.y_1[i],sk)+M[i],inv_r);
    }
    return sig;
}

GrothSig_2 groth_sign_2(G2 M[], Big sk, int len)
{
    Big order=pfc.order();
    GrothSig_2 sig;
    Big r;
    pfc.random(r);
    sig.R = pfc.mult(g1,r);
    Big inv_r = inverse(r, order);
    sig.S = pfc.mult((g_params.y_2[0]+pfc.mult(g2,sk)),inv_r);
    for(int i=0;i<len;i++)
    {
        sig.T[i] = pfc.mult(pfc.mult(g_params.y_2[i],sk)+M[i],inv_r);
    }
    return sig;
}

bool groth_verify_1(G1 M[], G2 vk, GrothSig_1 sig, int len)
{
    pfc.precomp_for_pairing(sig.R);
    pfc.precomp_for_pairing(g2);
    pfc.precomp_for_pairing(vk);

    GT left = pfc.pairing(sig.R, sig.S);
    GT right = pfc.pairing(g2, g_params.y_1[0])*pfc.pairing(vk,g1);
    if(left != right)
        return false;
    for(int i=0;i<len;i++)
    {
        left = pfc.pairing(sig.R, sig.T[i]);
        right = pfc.pairing(vk,g_params.y_1[i])*pfc.pairing(g2,M[i]);
        if(left != right)
            return false;
    }
    return true;
}

bool groth_verify_2(G2 M[], G1 vk, GrothSig_2 sig, int len)
{
    GT left = pfc.pairing(sig.S, sig.R);
    GT right = pfc.pairing(g_params.y_2[0], g1)*pfc.pairing(g2,vk);
    if(left != right)
        return false;
    for(int i=0;i<len;i++)
    {
        left = pfc.pairing(sig.T[i], sig.R);
        right = pfc.pairing(g_params.y_2[i],vk)*pfc.pairing(M[i],g1);
        if(left != right)
            return false;
    }
    return true;
}

GrothSig_1 groth_randomize_1(GrothSig_1 sig, int len)
{
    Big order=pfc.order();
    GrothSig_1 sig_new;
    Big r;
    pfc.random(r);
    Big inv_r = inverse(r, order);
    sig_new.R = pfc.mult(sig.R, r);
    sig_new.S = pfc.mult(sig.S, inv_r);
    for(int i=0;i<len;i++)
    {
        sig_new.T[i] = pfc.mult(sig.T[i], inv_r);
    }
    return sig_new;
}

GrothSig_2 groth_randomize_2(GrothSig_2 sig, int len)
{
    Big order=pfc.order();
    GrothSig_2 sig_new;
    Big r;
    pfc.random(r);
    Big inv_r = inverse(r, order);
    sig_new.R = pfc.mult(sig.R, r);
    sig_new.S = pfc.mult(sig.S, inv_r);
    for(int i=0;i<len;i++)
    {
        sig_new.T[i] = pfc.mult(sig.T[i], inv_r);
    }
    return sig_new;
}