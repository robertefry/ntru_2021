
#ifndef __HH_NTRU_
#define __HH_NTRU_

#include "NTRU/NTRU_Poly.hh"
#include "NTRU/NTRU_Keys.hh"
#include "NTRU/NTRU_Util.hh"

#include <cstdlib>
#include <tuple>

namespace ntru
{

    inline void NTRU_Init(unsigned int seed)
    {
        srand(seed);
    }

    template <typename Tp>
    inline NTRU_Basis<Tp> NTRU_GenBasis(NTRU_Seed<Tp> const& seed)
    {
        NTRU_Basis<Tp> basis;

        do
        {
            basis = { NTRU_GenTrinomial<Tp>(seed.N,seed.d+1,seed.d), NTRU_GenTrinomial<Tp>(seed.N,seed.d,seed.d) };
        }
        while (not NTRU_IsValid(seed,basis));

        return basis;
    }

    template <typename Tp>
    inline NTRU_KeyPair<Tp> NTRU_GenKeys(NTRU_Seed<Tp> const& seed, NTRU_Basis<Tp> const& basis)
    {
        auto const poly_Fp = NTRU_GetInverse(seed.N,seed.p,basis.poly_f);
        auto const poly_Fq = NTRU_GetInverse(seed.N,seed.q,basis.poly_f);
        auto const poly_h = NTRU_Reduce(seed.N,seed.q,poly_Fq*basis.poly_g);

        auto const key_pub = NTRU_PubKey<Tp>{ seed, poly_h };
        auto const key_prv = NTRU_PrvKey<Tp>{ seed, basis.poly_f, poly_Fp };
        return { key_pub, key_prv };
    }

    template <typename Tp>
    inline Poly<Tp> NTRU_Encrypt(NTRU_PubKey<Tp> const& key_pub, Poly<Tp> const& message)
    {
        auto const poly_r = NTRU_GenTrinomial<Tp>(key_pub.seed.N,key_pub.seed.d,key_pub.seed.d);

        auto const poly_e = key_pub.seed.p * key_pub.poly_h * poly_r + message;
        return NTRU_Reduce(key_pub.seed.N,key_pub.seed.q,poly_e);
    }

    template <typename Tp>
    inline Poly<Tp> NTRU_Decrypt(NTRU_PrvKey<Tp> const& key_prv, Poly<Tp> const& message)
    {
        auto poly_a = key_prv.poly_f * message;
        poly_a = NTRU_Reduce(key_prv.seed.N,key_prv.seed.q,poly_a);
        poly_a = NTRU_CenterLift(key_prv.seed.q,poly_a);

        auto poly_b = key_prv.poly_Fp * poly_a;
        return NTRU_Reduce(key_prv.seed.N,key_prv.seed.p,poly_b);
    }

} // namespace ntru

#endif // __HH_NTRU_
