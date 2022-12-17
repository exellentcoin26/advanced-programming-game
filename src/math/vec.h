#pragma once

#include <ostream>
#ifndef GAME_SRC_MATH_VEC_H
#define GAME_SRC_MATH_VEC_H

#include "../utils/types.h"

#include <array>
#include <cmath>
#include <exception>

namespace math {

struct DivisionByZero : std::exception {
    inline const char* what() const noexcept override { return "attempted to divide by zero"; }
};

class Vec2 {
public:
    inline Vec2() = default;
    inline Vec2(f64 x, f64 y) : elements({x, y}) {}
    inline Vec2(std::array<f64, 2> v) : elements(v) {}
    inline Vec2(const Vec2& v) : elements(v.elements) {}

    ~Vec2() = default;

    inline f64 get_x() const { return this->elements[0]; }
    inline f64 get_y() const { return this->elements[1]; }

    inline Vec2& xn(f64 y) {
        this->elements[1] = y;
        return *this;
    }

    inline Vec2& ny(f64 x) {
        this->elements[0] = x;
        return *this;
    }

    inline Vec2 operator+(const Vec2& rhs) const {
        return {this->elements[0] + rhs.elements[0], this->elements[1] + rhs.elements[1]};
    }

    inline Vec2 operator-(const Vec2& rhs) const {
        return {this->elements[0] - rhs.elements[0], this->elements[1] - rhs.elements[1]};
    }

    inline Vec2 operator*(f64 rhs) const { return {this->elements[0] * rhs, this->elements[1] * rhs}; }

    inline Vec2 operator/(f64 rhs) const {
        if (rhs == 0)
            throw DivisionByZero();
        return {this->elements[0] / rhs, this->elements[1] / rhs};
    }

    inline f64 operator*(const Vec2& rhs) const {
        return this->elements[0] * rhs.elements[0] + this->elements[1] * rhs.elements[1];
    }

    inline Vec2 operator-() const { return {-this->elements[0], -this->elements[1]}; }

    f64 length() const {
        return std::sqrt(this->elements[0] * this->elements[0] + this->elements[1] * this->elements[1]);
    }

    inline Vec2& normalize() {
        const f64 length = this->length();

        if (length == 0)
            throw DivisionByZero();

        this->elements[0] /= length;
        this->elements[1] /= length;

        return *this;
    }

    inline Vec2 norm() const { return Vec2(*this).normalize(); }

    inline bool operator==(const Vec2& rhs) const {
        return this->elements[0] == rhs.elements[0] && this->elements[1] == rhs.elements[1];
    }

    inline bool operator==(const std::array<f64, 2>& rhs) const {
        return this->elements[0] == rhs[0] && this->elements[1] == rhs[1];
    }

    inline bool operator!=(const Vec2& rhs) const { return !this->operator==(rhs); }

    inline bool operator!=(const std::array<f64, 2>& rhs) const { return !this->operator==(rhs); }

    inline Vec2& operator+=(const Vec2& rhs) {
        this->elements[0] += rhs.elements[0];
        this->elements[1] += rhs.elements[1];

        return *this;
    }

    inline Vec2& operator-=(const Vec2& rhs) { return this->operator+=(-rhs); }

    inline Vec2& operator*=(f64 rhs) {
        this->elements[0] *= rhs;
        this->elements[1] *= rhs;

        return *this;
    }

    inline Vec2& operator/=(f64 rhs) {
        if (rhs == 0)
            throw DivisionByZero();

        this->elements[0] /= rhs;
        this->elements[1] /= rhs;

        return *this;
    }

    friend inline Vec2 operator*(f64 lhs, const Vec2& rhs);

    friend inline std::ostream& operator<<(std::ostream& os, const Vec2& v);

private:
    std::array<f64, 2> elements{};
};

inline Vec2 operator*(f64 lhs, const Vec2& rhs) { return {rhs.elements[0] * lhs, rhs.elements[1] * lhs}; }

inline std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << '{' << v.elements[0] << ", " << v.elements[1] << '}';
    return os;
}

} // namespace math

#endif // GAME_SRC_MATH_VEC_H
