
#ifndef __HH_NTRU_UTIL
#define __HH_NTRU_UTIL

#include "NTRU_Keys.hh"
#include "NTRU_Poly.hh"

#include <array>
#include <algorithm>
#include <numeric>

namespace ntru
{

    template <typename Tp>
    inline Poly<Tp> NTRU_GenTrinomial(size_t degree, size_t d1, size_t d2)
    {
        std::array<size_t,3> queue { d2, degree-d1-d2, d1 };

        std::vector<Tp> coeffs;
        coeffs.reserve(degree);

        while (queue[0]+queue[1]+queue[2] != 0)
        {
            auto const random = rand() % 3;
            if (queue[random] > 0) {
                queue[random] -= 1;
                coeffs.push_back(random-1);
            }
        }
        return Poly<Tp>{std::move(coeffs)};
    }

    template <typename Tp>
    Poly<Tp> NTRU_Reduce(Tp const& modulo, Poly<Tp> const& poly)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < poly.size(); ++i)
        {
            result[i] = poly[i] % modulo;
            if (result[i] < 0) result[i] += modulo;
        }
        return result;
    }

    template <typename Tp>
    Poly<Tp> NTRU_Reduce(size_t degree, Tp const& modulo, Poly<Tp> const& poly)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < poly.size(); ++i)
        {
            size_t const index = i % degree;
            result[index] = (result[index] + poly[i]) % modulo;
            if (result[index] < 0) result[index] += modulo;
        }
        return result;
    }

    template <typename Tp>
    Poly<Tp> NTRU_CenterLift(Tp const& modulo, Poly<Tp> const& poly)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < poly.size(); ++i)
        {
            result[i] = poly[i] % modulo;
            if (result[i] > modulo / 2) result[i] -= modulo;
        }
        return result;
    }

    template <typename Tp>
    Tp NTRU_GCD(Tp const& a, Tp const& b)
    {
        if (a < b) return NTRU_GCD(b,a);

        std::array<Tp,2> rn = { a, b };

        while (rn[1] != 0)
        {
            rn = { rn[1], rn[0] % rn[1] };
        }

        if (rn[0] < 0) rn[0] = -rn[0];
        return rn[0];
    }

    template <typename Tp>
    std::array<Tp,2> NTRU_ExGCD(Tp const& a, Tp const& b)
    {
        if (a < b)
        {
            auto result = NTRU_ExGCD(b,a);
            return {result[1],result[0]};
        }

        std::array<std::array<Tp,2>,2> sn = { 1, 0, 0, 1 };
        std::array<Tp,2> rn = { a, b };

        while (rn[1] != 0)
        {
            auto const qm = rn[0] / rn[1];
            sn[0] = { sn[0][0] - qm * sn[1][0], sn[0][1] - qm * sn[1][1] };
            sn = { sn[1], sn[0] };
            rn = { rn[1], rn[0] % rn[1] };
        }

        if (rn[0] < 0) sn[0] = { -sn[0][1], -sn[0][0] };
        return sn[0];
    }

    template <typename Tp>
    std::array<Poly<Tp>,2> NTRU_DivisionRQ(Tp const& modulo, Poly<Tp> const& poly1, Poly<Tp> const& poly2)
    {
        Poly<Tp> divisor = poly2;
        Poly<Tp> remainder = poly1;
        Poly<Tp> quotient;

        while (remainder.order() >= divisor.order())
        {
            auto const [x,y] = NTRU_ExGCD(modulo,divisor.back());
            auto coeff = (remainder.back() * y) % modulo;
            if (coeff < 0) coeff += modulo;
            if (coeff == 0) break;

            auto const power = remainder.order() - divisor.order();
            auto const term = Poly<Tp>{coeff} <<= power;
            remainder = NTRU_Reduce(modulo, remainder -= term * divisor);
            quotient = NTRU_Reduce(modulo, quotient += term);
        }
        return {remainder,quotient};
    }

    template <typename Tp>
    Poly<Tp> NTRU_GetQuotient(size_t degree)
    {
        return (Poly<Tp>{1} <<= degree) - Poly<Tp>{1};
    }

    template <typename Tp>
    Poly<Tp> NTRU_QuotientGCD(size_t degree, Tp const& modulo, Poly<Tp> const& poly)
    {
        auto const quotient = NTRU_GetQuotient<Tp>(degree);
        std::array<Poly<Tp>,2> rn = { quotient, NTRU_Reduce(degree,modulo,poly) };

        while (rn[1] != Poly<Tp>{0})
        {
            auto const [rm,qm] = NTRU_DivisionRQ(modulo,rn[0],rn[1]);
            rn = { rn[1], rm };
        }
        return NTRU_Reduce(degree,modulo,rn[0]);
    }

    template <typename Tp>
    bool NTRU_HasInverse(size_t degree, Tp const& modulo, Poly<Tp> const& poly)
    {
        return NTRU_QuotientGCD(degree,modulo,poly) == Poly<Tp>{1};
    }

    template <typename Tp>
    Poly<Tp> NTRU_GetInverse(size_t degree, Tp const& modulo, Poly<Tp> const& poly)
    {
        std::array<std::array<Poly<Tp>,2>,2> sn = {
            Poly<Tp>{1}, Poly<Tp>{0}, Poly<Tp>{0}, Poly<Tp>{1}
        };
        std::array<Poly<Tp>,2> rn = {
            NTRU_GetQuotient<Tp>(degree), NTRU_Reduce(degree,modulo,poly)
        };

        while (rn[1] != Poly<Tp>{0})
        {
            auto const [rm,qm] = NTRU_DivisionRQ(modulo,rn[0],rn[1]);
            sn[0] = { sn[0][0] - qm * sn[1][0], sn[0][1] - qm * sn[1][1] };
            sn = { sn[1], sn[0] };
            rn = { rn[1], rm };
        }

        if (rn[0] < Poly<Tp>{0}) sn[0] = { -sn[0][1], -sn[0][0] };
        return NTRU_Reduce(degree,modulo,sn[0][1]);
    }

    template <typename Tp>
    bool NTRU_IsValid(NTRU_Seed<Tp> const& seed)
    {
        bool valid = true;
        valid &= seed.q > (Tp)(6*seed.d+1) * seed.p;
        valid &= seed.N >= (size_t)(2*seed.d + 1);
        return valid;
    }

    template <typename Tp>
    bool NTRU_IsValid(size_t d1, size_t d2, Poly<Tp> const& poly)
    {
        bool valid = true;
        valid &= d1 == (size_t)std::count(poly.coeffs().begin(),poly.coeffs().end(),1);
        valid &= d2 == (size_t)std::count(poly.coeffs().begin(),poly.coeffs().end(),-1);
        valid &= d1 - d2 == (size_t)std::accumulate(poly.coeffs().begin(),poly.coeffs().end(),0);
        return valid;
    }

    template <typename Tp>
    bool NTRU_IsValid(NTRU_Seed<Tp> const& seed, NTRU_Basis<Tp> const& basis)
    {
        bool valid = true;
        valid &= NTRU_IsValid(seed.d,seed.d,basis.poly_g);
        valid &= NTRU_IsValid(seed.d+1,seed.d,basis.poly_f);
        valid &= NTRU_HasInverse(seed.N,seed.p,basis.poly_f);
        valid &= NTRU_HasInverse(seed.N,seed.q,basis.poly_f);
        return valid;
    }

} // namespace ntru

#endif // __HH_NTRU_UTIL
