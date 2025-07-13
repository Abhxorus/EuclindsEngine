#pragma once

#include "Prerequisitos.h"

/**
 * @class Vec2D
 * @brief 2D vector class with common vector operations and utilities.
 */
class CVector2D {
public:
    float xCoord; ///< X coordinate component of the vector
    float yCoord; ///< Y coordinate component of the vector

    /**
     * @brief Default constructor initializes vector to (0,0).
     */
    CVector2D() : xCoord(0.0f), yCoord(0.0f) {}

    /**
     * @brief Constructor initializes vector with specified components.
     * @param x X component value.
     * @param y Y component value.
     */
    CVector2D(float x, float y) : xCoord(x), yCoord(y) {}

    /**
     * @brief Vector addition operator.
     * @param v Vector to add.
     * @return Resulting vector after addition.
     */
    CVector2D operator+(const CVector2D& v) const {
        return CVector2D(xCoord + v.xCoord, yCoord + v.yCoord);
    }

    /**
     * @brief Vector subtraction operator.
     * @param v Vector to subtract.
     * @return Resulting vector after subtraction.
     */
    CVector2D operator-(const CVector2D& v) const {
        return CVector2D(xCoord - v.xCoord, yCoord - v.yCoord);
    }

    /**
     * @brief Scalar multiplication operator.
     * @param factor Scalar value to multiply by.
     * @return Scaled vector.
     */
    CVector2D operator*(float factor) const {
        return CVector2D(xCoord * factor, yCoord * factor);
    }

    /**
     * @brief Scalar division operator.
     * @param divisor Scalar value to divide by.
     * @return Scaled vector.
     * @warning Division by zero will result in infinity or NaN.
     */
    CVector2D operator/(float divisor) const {
        return CVector2D(xCoord / divisor, yCoord / divisor);
    }

    /**
     * @brief Equality comparison operator.
     * @param v Vector to compare with.
     * @return True if vectors are exactly equal.
     */
    bool operator==(const CVector2D& v) const {
        return xCoord == v.xCoord && yCoord == v.yCoord;
    }

    /**
     * @brief Component access by index.
     * @param idx Index of component (0 for x, 1 for y).
     * @return Reference to the requested component.
     * @warning No bounds checking is performed.
     */
    float& operator[](int idx) {
        return (idx == 0) ? xCoord : yCoord;
    }

    /**
     * @brief Const component access by index.
     * @param idx Index of component (0 for x, 1 for y).
     * @return Const reference to the requested component.
     * @warning No bounds checking is performed.
     */
    const float& operator[](int idx) const {
        return (idx == 0) ? xCoord : yCoord;
    }

    /**
     * @brief Calculates the magnitude (length) of the vector.
     * @return Length of the vector.
     */
    float magnitude() const {
        return std::sqrt(xCoord * xCoord + yCoord * yCoord);
    }

    /**
     * @brief Calculates the squared magnitude of the vector.
     * @return Squared length of the vector.
     * @note Useful for comparisons to avoid sqrt operations.
     */
    float magnitudeSquared() const {
        return xCoord * xCoord + yCoord * yCoord;
    }

    /**
     * @brief Computes the dot product with another vector.
     * @param v Vector to compute dot product with.
     * @return Dot product result.
     */
    float dotProduct(const CVector2D& v) const {
        return xCoord * v.xCoord + yCoord * v.yCoord;
    }

    /**
     * @brief Computes the 2D cross product (perpendicular dot product).
     * @param v Vector to compute cross product with.
     * @return Cross product result (scalar in 2D).
     * @note This returns the magnitude of the 3D cross product.
     */
    float crossProduct(const CVector2D& v) const {
        return xCoord * v.yCoord - yCoord * v.xCoord;
    }

    /**
     * @brief Returns a normalized copy of the vector.
     * @return Unit vector in the same direction.
     * @note Returns zero vector if length is zero.
     */
    CVector2D unit() const {
        float mag = magnitude();
        return (mag == 0.0f) ? CVector2D(0.0f, 0.0f) : (*this) / mag;
    }

    /**
     * @brief Normalizes this vector in-place.
     * @note Converts to zero vector if length is zero.
     */
    void normalize() {
        float mag = magnitude();
        if (mag != 0.0f) {
            xCoord /= mag;
            yCoord /= mag;
        }
        else {
            xCoord = yCoord = 0.0f;
        }
    }

    /**
     * @brief Computes distance between two points.
     * @param a First point vector.
     * @param b Second point vector.
     * @return Distance between the points.
     */
    static float distanceBetween(const CVector2D& a, const CVector2D& b) {
        return (b - a).magnitude();
    }

    /**
     * @brief Linearly interpolates between two vectors.
     * @param start Starting vector.
     * @param end Ending vector.
     * @param t Interpolation factor (0.0 to 1.0).
     * @return Interpolated vector.
     */
    static CVector2D interpolate(const CVector2D& start, const CVector2D& end, float t) {
        return start + (end - start) * t;
    }

    /**
     * @brief Creates a zero vector.
     * @return Vector (0, 0).
     */
    static CVector2D zeroVector() {
        return CVector2D(0.0f, 0.0f);
    }

    /**
     * @brief Creates a one vector.
     * @return Vector (1, 1).
     */
    static CVector2D oneVector() {
        return CVector2D(1.0f, 1.0f);
    }

    /**
     * @brief Prints the vector to standard output.
     */
    void print() const {
        std::cout << "CVector2D(" << xCoord << ", " << yCoord << ")" << std::endl;
    }
};