
#ifndef __HH_NTRU
#define __HH_NTRU

#include "NTRU_Poly.hh"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Definitions
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

namespace ntru
{

    template <typename Tp>
    struct NTRU_Seed
    {
        size_t N, d;
        Tp p, q;
    };

    template <typename Tp>
    struct NTRU_Basis
    {
        Poly<Tp> poly_f, poly_g;
    };

    template <typename Tp>
    struct NTRU_PubKey
    {
        NTRU_Seed<Tp> seed;
        Poly<Tp> poly_h;
    };

    template <typename Tp>
    struct NTRU_PrvKey
    {
        NTRU_Seed<Tp> seed;
        Poly<Tp> poly_f, poly_Fp;
    };

    template <typename Tp>
    struct NTRU_KeyPair
    {
        NTRU_PubKey<Tp> key_pub;
        NTRU_PrvKey<Tp> key_prv;
    };

} // namespace ntru

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Standard Extensions
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <iosfwd>

namespace std
{

    template <typename Tp, typename Ch>
    basic_ostream<Ch>& operator<<(basic_ostream<Ch>& ost, ntru::NTRU_Seed<Tp> const& seed)
    {
        return ost << '{' << seed.N << ',' << seed.d << ',' << seed.p << ',' << seed.q << '}';
    }

    template <typename Tp, typename Ch>
    basic_ostream<Ch>& operator<<(basic_ostream<Ch>& ost, ntru::NTRU_Basis<Tp> const& basis)
    {
        return ost << '{' << basis.poly_f << "," << basis.poly_g << '}';
    }

} // namespace std

#endif // __HH_NTRU
