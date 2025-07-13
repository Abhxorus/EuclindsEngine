#include "CVector2.h"

// Constructores
CVector2D::CVector2D() : x(0.f), y(0.f) {}
CVector2D::CVector2D(float xVal, float yVal) : x(xVal), y(yVal) {}

// Operadores
CVector2D CVector2D::operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
}
CVector2 CVector2::operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
}
CVector2 CVector2::operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
}
CVector2 CVector2::operator/(float scalar) const {
    return CVector2(x / scalar, y / scalar);
}
bool CVector2::operator==(const CVector2& other) const {
    return x == other.x && y == other.y;
}

float& CVector2::operator[](int index) {
    return (index == 0) ? x : y;
}
const float& CVector2::operator[](int index) const {
    return (index == 0) ? x : y;
}

// Funciones geométricas
float CVector2::length() const {
    return std::sqrt(x * x + y * y);
}
float CVector2::lengthSquared() const {
    return x * x + y * y;
}
float CVector2::dot(const CVector2& other) const {
    return x * other.x + y * other.y;
}
float CVector2::cross(const CVector2& other) const {
    return x * other.y - y * other.x;
}
CVector2 CVector2::normalized() const {
    float len = length();
    return (len == 0.f) ? CVector2(0.f, 0.f) : (*this) / len;
}
void CVector2::normalize() {
    float len = length();
    if (len != 0.f) {
        x /= len;
        y /= len;
    }
    else {
        x = y = 0.f;
    }
}

// Utilidades
float CVector2::distance(const CVector2& a, const CVector2& b) {
    return (b - a).length();
}
CVector2 CVector2::lerp(const CVector2& a, const CVector2& b, float t) {
    return a + (b - a) * std::clamp(t, 0.f, 1.f);
}
CVector2 CVector2::zero() {
    return CVector2(0.f, 0.f);
}
CVector2 CVector2::one() {
    return CVector2(1.f, 1.f);
}

// Debug
void CVector2::print() const {
    std::cout << "CVector2(" << x << ", " << y << ")" << std::endl;
}
