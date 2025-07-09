#pragma once

#include "Prerequisitos.h"

class CVector3 {
public:
    float x, y, z;

    CVector3() : x(0), y(0), z(0) {}
    CVector3(float x, float y, float z) : x(x), y(y), z(z) {}

    CVector3 operator+(const CVector3& other) const {
        return CVector3(x + other.x, y + other.y, z + other.z);
    }

    CVector3 operator-(const CVector3& other) const {
        return CVector3(x - other.x, y - other.y, z - other.z);
    }

    CVector3 operator*(float scalar) const {
        return CVector3(x * scalar, y * scalar, z * scalar);
    }

    CVector3 operator/(float scalar) const {
        return CVector3(x / scalar, y / scalar, z / scalar);
    }

    CVector3& operator+=(const CVector3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    CVector3& operator-=(const CVector3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    CVector3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    CVector3& operator/=(float scalar) {
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    bool operator==(const CVector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const CVector3& other) const {
        return !(*this == other);
    }

    float& operator[](int index) {
        switch (index) {
        case 0: return x;
        case 1: return y;
        default: return z;
        }
    }

    const float& operator[](int index) const {
        switch (index) {
        case 0: return x;
        case 1: return y;
        default: return z;
        }
    }

    float lengthSquare() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt(lengthSquare());
    }

    float dot(const CVector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    CVector3 normalized() const {
        float len = length();
        return (len == 0.f) ? CVector3(0.f, 0.f, 0.f) : (*this) / len;
    }

    void normalize() {
        float len = length();
        if (len != 0.f) {
            x /= len; y /= len; z /= len;
        }
        else {
            x = y = z = 0.f;
        }
    }

    inline static float distance(const CVector3& a, const CVector3& b) {
        return (b - a).lengthSquare();
    }

    inline static CVector3 lerp(const CVector3& a, const CVector3& b, float t) {
        return a + (b - a) * t;
    }

    inline static CVector3 zero() {
        return CVector3(0.f, 0.f, 0.f);
    }

    inline static CVector3 one() {
        return CVector3(1.f, 1.f, 1.f);
    }

    friend std::ostream& operator<<(std::ostream& os, const CVector3& v) {
        os << "CVector3(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
}; #pragma once
