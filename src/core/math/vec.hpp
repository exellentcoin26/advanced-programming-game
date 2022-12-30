#pragma once

#ifndef GAME_SRC_MATH_VEC_H
#define GAME_SRC_MATH_VEC_H

#include "core/utils/types.hpp"

#include <array>
#include <cmath>
#include <exception>
#include <ostream>

namespace math {

struct DivisionByZero : std::exception {
    inline const char* what() const noexcept override { return "attempted to divide by zero"; }
};

class Vec2 {
public:
    /// Creates a new `Vec2` instance with both elements set to 0.
    inline Vec2() = default;
    /// Creates a new `Vec2` instance using the given elements.
    inline Vec2(f64 x, f64 y) : elements({x, y}) {}
    /// Creates a new `Vec2` instance from the given array.
    inline Vec2(std::array<f64, 2> v) : elements(v) {}
    inline Vec2(const Vec2& v) : elements(v.elements) {}

    ~Vec2() = default;

    /// Returns the x component of the `Vec2`.
    inline f64 get_x() const { return this->elements[0]; }
    /// Returns the y component of the `Vec2`.
    inline f64 get_y() const { return this->elements[1]; }

    /// Replaces the y component of the `Vec2`.
    inline Vec2& xn(f64 y) {
        this->elements[1] = y;
        return *this;
    }

    /// Replaces the x component of the `Vec2`.
    inline Vec2& ny(f64 x) {
        this->elements[0] = x;
        return *this;
    }

    /// Adds two `Vec2` instances and returns the result.
    inline Vec2 operator+(const Vec2& rhs) const {
        return {this->elements[0] + rhs.elements[0], this->elements[1] + rhs.elements[1]};
    }

    /// Subtrackts two `Vec2` instances and returns the result.
    inline Vec2 operator-(const Vec2& rhs) const {
        return {this->elements[0] - rhs.elements[0], this->elements[1] - rhs.elements[1]};
    }

    /// Multiplies the `Vec2` instance with factor.
    inline Vec2 operator*(f64 rhs) const { return {this->elements[0] * rhs, this->elements[1] * rhs}; }

    /// Divides the `Vec2` instance with the factor.
    /// Note: Throws `DivisionByZero` exception when `rhs` is equal to 0.
    inline Vec2 operator/(f64 rhs) const {
        if (rhs == 0)
            throw DivisionByZero();
        return {this->elements[0] / rhs, this->elements[1] / rhs};
    }

    /// Calculates inner product of the `Vec2` instances.
    inline f64 operator*(const Vec2& rhs) const {
        return this->elements[0] * rhs.elements[0] + this->elements[1] * rhs.elements[1];
    }

    /// Negates both axes of the `Vec2` instance.
    inline Vec2 operator-() const { return {-this->elements[0], -this->elements[1]}; }

    /// Returns the length of the `Vec2` instance.
    f64 length() const {
        return std::sqrt(this->elements[0] * this->elements[0] + this->elements[1] * this->elements[1]);
    }

    /// Normalizes the `Vec2` instance, resulting in it having length 1.
    inline Vec2& normalize() {
        const f64 length = this->length();

        if (length == 0)
            throw DivisionByZero();

        this->elements[0] /= length;
        this->elements[1] /= length;

        return *this;
    }

    /// Returns the normalized `Vec2` without modifying this instance.
    inline Vec2 norm() const { return Vec2(*this).normalize(); }

    /// Determines whether two `Vec2` instances are equal.
    inline bool operator==(const Vec2& rhs) const {
        return this->elements[0] == rhs.elements[0] && this->elements[1] == rhs.elements[1];
    }

    /// Determines whether the components of the `Vec2` are equal to the elemenst of the array.
    inline bool operator==(const std::array<f64, 2>& rhs) const {
        return this->elements[0] == rhs[0] && this->elements[1] == rhs[1];
    }

    /// Determines whether two `Vec2` instances are not equal
    inline bool operator!=(const Vec2& rhs) const { return !this->operator==(rhs); }

    /// Determines whether the components of the `Vec2` are not equal to the elements of the array.
    inline bool operator!=(const std::array<f64, 2>& rhs) const { return !this->operator==(rhs); }

    /// Adds a `Vec2` to this instance.
    inline Vec2& operator+=(const Vec2& rhs) {
        this->elements[0] += rhs.elements[0];
        this->elements[1] += rhs.elements[1];

        return *this;
    }

    /// Subtrackts a `Vec2` from this instance.
    inline Vec2& operator-=(const Vec2& rhs) { return this->operator+=(-rhs); }

    /// Multiplies this instance with the factor.
    inline Vec2& operator*=(f64 rhs) {
        this->elements[0] *= rhs;
        this->elements[1] *= rhs;

        return *this;
    }

    /// Divides this instance by the factor.
    /// Note: Throws a `DivisionByZero` exception when factor is 0.
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
