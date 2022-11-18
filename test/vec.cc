#include "doctest/doctest.h"

#include "../src/math/vec.h"

TEST_SUITE("vec2") {
    using namespace math;

    TEST_CASE("constructors") {
        const Vec2 a;
        CHECK_EQ(a, Vec2{0, 0});

        const Vec2 b{2.0, 3.0};
        CHECK_EQ(b, Vec2{2, 3});

        const Vec2 c{{1.0, 3.0}};
        CHECK_EQ(c, Vec2{1, 3});

        const Vec2 d{b};
        CHECK_EQ(d, Vec2{2, 3});
    }

    TEST_CASE("plus") {
        const Vec2 a{2.0, 3.0};
        const Vec2 b{3.0, 2.5};

        CHECK_EQ(a + b, Vec2{5, 5.5});
    }

    TEST_CASE("minus") {
        const Vec2 a{2.0, 3.0};
        const Vec2 b{3.0, 2.5};

        CHECK_EQ(a - b, Vec2{-1.0, 0.5});
    }

    TEST_CASE("multiply") {
        const Vec2 a{2.0, 3.0};
        const f64 b{4.0};

        CHECK_EQ(a * b, Vec2{8.0, 12.0});
        CHECK_EQ(b * a, Vec2{8.0, 12.0});
    }

    TEST_CASE("divide") {
        const Vec2 a{2.0, 3.0};
        const f64 b{2.0};

        CHECK_EQ(a / b, Vec2{1.0, 1.5});
    }

    TEST_CASE("divide_by_zero") {
        const Vec2 a{2.0, 3.0};
        const f64 b{0.0};

        CHECK_THROWS_AS(a / b, DivisionByZero);
    }

    TEST_CASE("inner") {
        const Vec2 a{2.0, 3.0};
        const Vec2 b{3.0, 2.5};

        CHECK_EQ(a * b, 13.5);
    }

    TEST_CASE("negate") {
        const Vec2 a{2.0, 3.0};
        const Vec2 b = -a;

        CHECK_EQ(b, Vec2{-2.0, -3.0});
    }

    TEST_CASE("length") {
        const Vec2 a{2.0, 3.0};

        CHECK_EQ(a.length(), std::sqrt(13.0));
    }

    TEST_CASE("norm_and_normalize") {
        Vec2 a{2.0, 3.0};

        CHECK_EQ(a.normalize(), Vec2(2.0 / std::sqrt(13.0), 3.0 / std::sqrt(13.0)));
    }

    TEST_CASE("norm_and_normalize_divide_by_zero") {
        Vec2 a;

        CHECK_THROWS_AS(a.normalize(), DivisionByZero);
    }

    TEST_CASE("plus_equal") {
        Vec2 a{2.0, 3.0};
        const Vec2 b{3.0, 2.0};

        CHECK_EQ(a += b, Vec2{5.0, 5.0});
    }

    TEST_CASE("min_equal") {
        Vec2 a{2.0, 3.0};
        const Vec2 b{3.0, 2.0};

        CHECK_EQ(a -= b, Vec2{-1.0, 1.0});
    }

    TEST_CASE("multiply_equal") {
        Vec2 a{2.0, 3.0};
        const f64 b{2.0};

        CHECK_EQ(a *= b, Vec2{4.0, 6.0});
    }

    TEST_CASE("divide_equal") {
        Vec2 a{2.0, 3.0};
        const f64 b{2.0};

        CHECK_EQ(a /= b, Vec2{1.0, 1.5});
    }

    TEST_CASE("divide_equal_by_zero") {
        Vec2 a{2.0, 3.0};
        const f64 b{0.0};

        CHECK_THROWS_AS(a /= b, DivisionByZero);
    }
}
