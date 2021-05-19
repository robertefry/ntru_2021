
#include "NTRU/NTRU_Util.hh"

#include <gtest/gtest.h>

TEST(NTRU_UTIL, REDUCE)
{
    ntru::Poly<int> const poly { 2, 3, 5, 7, 11, 13, 17 };
    {
        auto const result = ntru::NTRU_Reduce(5,poly);
        auto const expected = ntru::Poly<int>{ 2, 3, 0, 2, 1, 3, 2 };
        EXPECT_EQ(result, expected);
    }
    {
        auto const result = ntru::NTRU_Reduce(3,5,poly);
        auto const expected = ntru::Poly<int>{ 1, 4, 3 };
        EXPECT_EQ(result, expected);
    }
}

TEST(NTRU_UTIL, CENTER_LIFT)
{
    ntru::Poly<int> const poly { 2, 3, 5, 7, 11, 13, 17 };
    {
        auto const result = ntru::NTRU_CenterLift(5,poly);
        auto const expected = ntru::Poly<int>{ 2, -2, 0, 2, 1, -2, 2 };
        EXPECT_EQ(result, expected);
    }
}

TEST(NTRU_UTIL, GCD_INTEGER)
{
    {
        auto const result = ntru::NTRU_GCD(84,120);
        auto const expected = 12;
        EXPECT_EQ(result, expected);
    }
    {
        auto const [x,y] = ntru::NTRU_ExGCD(84,120);
        EXPECT_EQ(x,+3);
        EXPECT_EQ(y,-2);
    }
}

TEST(NTRU_UTIL, POLY_DIVISION)
{
    auto const poly_a = ntru::Poly<int>{2,0,1,1,3};
    auto const poly_b = ntru::Poly<int>{1,2,1};
    auto const poly_q = ntru::Poly<int>{3,0,3};
    auto const poly_r = ntru::Poly<int>{4,4};
    {
        auto result = poly_b * poly_q + poly_r;
        result = ntru::NTRU_Reduce(5,5,result);
        EXPECT_EQ(result,poly_a);
    }
    {
        auto const [r,q] = ntru::NTRU_DivisionRQ(5,poly_a,poly_b);
        EXPECT_EQ(q, poly_q);
        EXPECT_EQ(r, poly_r);
    }
}

TEST(NTRU_UTIL, INVERSE)
{
    auto const poly_a = ntru::Poly<int>{1,4,2};
    {
        auto const quotient_gcd = ntru::NTRU_QuotientGCD(3,5,poly_a);
        EXPECT_EQ(quotient_gcd,ntru::Poly<int>{1});
    }
    {
        auto const has_inverse = ntru::NTRU_HasInverse(3,5,poly_a);
        EXPECT_TRUE(has_inverse);
    }
    {
        auto const expected = ntru::Poly<int>{2,0,1};
        auto const inverse = ntru::NTRU_GetInverse(3,5,poly_a);
        EXPECT_EQ(inverse,expected);
    }

    for (int a = 0; a < 5; ++a) for (int b = 0; b < 5; ++b) for (int c = 0; c < 5; ++c)
        for (int d = 0; d < 5; ++d) for (int e = 0; e < 5; ++e)
    {
        auto const poly_a = ntru::Poly<int>{a,b,c,d,e};
        size_t count = 0;

        if (ntru::NTRU_HasInverse(5,11,poly_a))
        {
            count += 1;
            auto const poly_b = ntru::NTRU_GetInverse(5,11,poly_a);
            auto const poly_c = ntru::NTRU_Reduce(5,11,poly_a*poly_b);
            EXPECT_EQ(poly_c,ntru::Poly<int>{1});
        }
    }
}
