
#ifndef __HH_NTRU_POLY
#define __HH_NTRU_POLY

#include <vector>
#include <compare>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Definition
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

namespace ntru
{

    template <typename Tp>
    class Poly
    {
    public:
        explicit Poly() = default;
        virtual ~Poly() = default;

        Poly(std::initializer_list<Tp>);
        Poly(std::vector<Tp> const&);
        Poly(std::vector<Tp>&&);

    public:
        Poly& trim() const;

        auto coeffs() const -> std::vector<Tp> const& { return m_Coefficients; }
        auto coeffs() -> std::vector<Tp>& { return m_Coefficients; }
        auto size() const -> size_t { return m_Coefficients.size(); }

        auto order() const -> size_t;
        auto front() const -> Tp const& { return m_Coefficients[0]; }
        auto back() const -> Tp const& { return m_Coefficients[order()]; }

        auto operator[](size_t) const -> Tp const&;
        auto operator[](size_t) -> Tp&;

        Poly& operator<<=(size_t);
        Poly& operator>>=(size_t);

        Poly& operator+=(Poly const&);
        Poly& operator-=(Poly const&);
        Poly& operator*=(Poly const&);

    private:
        mutable std::vector<Tp> m_Coefficients{};
    };

} // namespace ntru

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Implementation
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

namespace ntru
{

    template <typename Tp>
    Poly<Tp>::Poly(std::initializer_list<Tp> args)
        : m_Coefficients{args}
    {
    }

    template <typename Tp>
    Poly<Tp>::Poly(std::vector<Tp> const& args)
        : m_Coefficients{args}
    {
    }

    template <typename Tp>
    Poly<Tp>::Poly(std::vector<Tp>&& args)
        : m_Coefficients{std::move(args)}
    {
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::trim() const
    {
        if (m_Coefficients.size())
        {
            m_Coefficients.erase(m_Coefficients.begin()+order(),m_Coefficients.end());
        }
        return *this;
    }

    template <typename Tp>
    size_t Poly<Tp>::order() const
    {
        for (size_t i = m_Coefficients.size() - 1; i < m_Coefficients.size(); --i)
        {
            if (m_Coefficients[i] != 0) return i;
        }
        return 0;
    }

    template <typename Tp>
    auto Poly<Tp>::operator[](size_t index) const -> Tp const&
    {
        if (index >= m_Coefficients.size()) m_Coefficients.resize(index+1);
        return m_Coefficients[index];
    }

    template <typename Tp>
    auto Poly<Tp>::operator[](size_t index) -> Tp&
    {
        if (index >= m_Coefficients.size()) m_Coefficients.resize(index+1);
        return m_Coefficients[index];
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator<<=(size_t power)
    {
        m_Coefficients.insert(m_Coefficients.begin(),power,Tp{});
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator>>=(size_t power)
    {
        if (m_Coefficients.size() > power)
        {
            m_Coefficients.erase(m_Coefficients.begin(),m_Coefficients.begin()+power);
        } else {
            m_Coefficients.resize(0);
        }
        return *this;
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator+=(Poly<Tp> const& other)
    {
        return *this = (*this + other);
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator-=(Poly<Tp> const& other)
    {
        return *this = (*this - other);
    }

    template <typename Tp>
    Poly<Tp>& Poly<Tp>::operator*=(Poly<Tp> const& other)
    {
        return *this = (*this * other);
    }

} // namespace ntru

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Non-Member Extensions
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

namespace ntru
{

    template <typename Tp>
    bool operator==(Poly<Tp> const& poly1, Poly<Tp> const& poly2)
    {
        for (size_t i = 0; i < std::max(poly1.size(),poly2.size()); ++i)
        {
            if (poly1[i] != poly2[i]) return false;
        }
        return true;
    }

    template <typename Tp>
    auto operator<=>(Poly<Tp> const& poly1, Poly<Tp> const& poly2)
    {
        using order_type = std::compare_three_way_result<Tp>::type;

        if (poly1.order() > poly2.order()) return poly1.back() <=> 0;
        if (poly1.order() < poly2.order()) return 0 <=> poly2.back();
        size_t order = std::max(poly1.order(),poly2.order());

        for (size_t i = order; i <= order; --i)
        {
            if (poly1[i] < poly2[i]) return order_type::less;
            if (poly1[i] > poly2[i]) return order_type::greater;
        }
        return order_type::equivalent;
    }

    template <typename Tp>
    auto operator>>(Poly<Tp> const& poly, size_t power)
    {
        return Poly<Tp>{poly} >>= power;
    }

    template <typename Tp>
    auto operator<<(Poly<Tp> const& poly, size_t power)
    {
        return Poly<Tp>{poly} <<= power;
    }

    template <typename Tp>
    auto operator+(Poly<Tp> const& poly)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < poly.size(); ++i)
        {
            result[i] = +poly[i];
        }
        return result;
    }

    template <typename Tp>
    auto operator-(Poly<Tp> const& poly)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < poly.size(); ++i)
        {
            result[i] = -poly[i];
        }
        return result;
    }

    template <typename Tp>
    auto operator+(Poly<Tp> const& poly1, Poly<Tp> const& poly2)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < std::max(poly1.size(),poly2.size()); ++i)
        {
            result[i] = poly1[i] + poly2[i];
        }
        return result;
    }

    template <typename Tp>
    auto operator+(Tp const& value, Poly<Tp> const& poly)
    {
        return Poly<Tp>{value} + poly;
    }

    template <typename Tp>
    auto operator+(Poly<Tp> const& poly, Tp const& value)
    {
        return poly + Poly<Tp>{value};
    }

    template <typename Tp>
    auto operator-(Poly<Tp> const& poly1, Poly<Tp> const& poly2)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < std::max(poly1.size(),poly2.size()); ++i)
        {
            result[i] = poly1[i] - poly2[i];
        }
        return result;
    }

    template <typename Tp>
    auto operator-(Tp const& value, Poly<Tp> const& poly)
    {
        return Poly<Tp>{value} - poly;
    }

    template <typename Tp>
    auto operator-(Poly<Tp> const& poly, Tp const& value)
    {
        return poly - Poly<Tp>{value};
    }

    template <typename Tp>
    auto operator*(Poly<Tp> const& poly1, Poly<Tp> const& poly2)
    {
        Poly<Tp> result;

        for (size_t i = 0; i < poly1.size(); ++i)
        {
            for (size_t j = 0; j < poly2.size(); ++j)
            {
                result[i+j] += poly1[i] * poly2[j];
            }
        }
        return result;
    }

    template <typename Tp>
    auto operator*(Tp const& value, Poly<Tp> const& poly)
    {
        return Poly<Tp>{value} * poly;
    }

    template <typename Tp>
    auto operator*(Poly<Tp> const& poly, Tp const& value)
    {
        return poly * Poly<Tp>{value};
    }

} // namespace ntru

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// Standard Extensions
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <iosfwd>

namespace std
{

    template <typename Tp, typename Ch>
    basic_ostream<Ch>& operator<<(basic_ostream<Ch>& ost, ntru::Poly<Tp> const& poly)
    {
        ost << '[' << poly[0];

        for (size_t i = 1; i < poly.size(); ++i)
        {
            ost << ',' << poly[i];
        }
        return ost << ']';
    }

} // namespace std

#endif // __HH_NTRU_POLY
