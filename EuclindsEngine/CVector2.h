#pragma once

#include "Prerequisitos.h"

class CVector2 {
public:
    float x, y;

    // Constructores
    CVector2();
    CVector2(float x, float y);

    // Operadores
    CVector2 operator+(const CVector2& other) const;
    CVector2 operator-(const CVector2& other) const;
    CVector2 operator*(float scalar) const;
    CVector2 operator/(float scalar) const;
    bool operator==(const CVector2& other) const;

    float& operator[](int index);
    const float& operator[](int index) const;

    // Funciones geométricas
    float length() const;
    float lengthSquared() const;
    float dot(const CVector2& other) const;
    float cross(const CVector2& other) const;

    CVector2 normalized() const;
    void normalize();

    // Utilidades
    static float distance(const CVector2& a, const CVector2& b);
    static CVector2 lerp(const CVector2& a, const CVector2& b, float t);
    static CVector2 zero();
    static CVector2 one();

    // Debug
    void print() const;
};