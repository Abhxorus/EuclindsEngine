#pragma once
#include "../Utilities/EngineMath.h"
#include <ostream>
#include <SFML/System/Vector2.hpp>

namespace EngineMathLib
{
    /**
     * @class CVector2
     * @brief Represents a 2D vector with basic operations.
     */
    class
        CVector2 {
    public:
        float x, y; /**< The x and y components of the vector */

    public:
        static sf::Vector2f cVector2ToSF(const CVector2& v) {
            return sf::Vector2f(v.x, v.y);
        }
        static CVector2 sfToCVector2(const sf::Vector2f& v) {
            return CVector2(v.x, v.y);
        }

        
        operator sf::Vector2f() const {
            return sf::Vector2f(x, y);
        }
        /**
         * @brief Default constructor, initializes vector to (0,0).
         */
        CVector2() : x(0), y(0) {}

        /**
         * @brief Constructor with initial values.
         * @param x The x component.
         * @param y The y component.
         */
        CVector2(float x, float y) : x(x), y(y) {}

        /**
         * @brief Adds two vectors.
         * @param other Vector to add.
         * @return Resulting vector from the addition.
         */
        CVector2
            operator+(const CVector2& other) const {
            return CVector2(x + other.x, y + other.y);
        }

        /**
         * @brief Subtracts two vectors.
         * @param other Vector to subtract.
         * @return Resulting vector from the subtraction.
         */
        CVector2
            operator-(const CVector2& other) const {
            return CVector2(x - other.x, y - other.y);
        }

        /**
         * @brief Multiplies the vector by a scalar.
         * @param scalar Scalar multiplier.
         * @return New scaled vector.
         */
        CVector2
            operator*(float scalar) const {
            return CVector2(x * scalar, y * scalar);
        }

        /**
         * @brief Divides the vector by a scalar.
         * @param scalar Scalar divisor.
         * @return New scaled vector.
         */
        CVector2
            operator/(float scalar) const {
            return CVector2(x / scalar, y / scalar);
        }

        /**
         * @brief Adds and assigns another vector.
         * @param other Vector to add.
         * @return Reference to the modified vector.
         */
        CVector2&
            operator+=(const CVector2& other) {
            x += other.x; y += other.y;
            return *this;
        }

        /**
         * @brief Subtracts and assigns another vector.
         * @param other Vector to subtract.
         * @return Reference to the modified vector.
         */
        CVector2&
            operator-=(const CVector2& other) {
            x -= other.x; y -= other.y;
            return *this;
        }

        /**
         * @brief Multiplies and assigns a scalar.
         * @param scalar Scalar multiplier.
         * @return Reference to the modified vector.
         */
        CVector2&
            operator*=(float scalar) {
            x *= scalar; y *= scalar;
            return *this;
        }

        /**
         * @brief Divides and assigns a scalar.
         * @param scalar Scalar divisor.
         * @return Reference to the modified vector.
         */
        CVector2&
            operator/=(float scalar) {
            x /= scalar; y /= scalar;
            return *this;
        }

        /**
         * @brief Compares equality with another vector.
         * @param other Vector to compare.
         * @return True if both vectors are approximately equal.
         */
        bool
            operator==(const CVector2& other) const {
            return EngineMathLib::approxEqual(x, other.x)
                && EngineMathLib::approxEqual(y, other.y);
        }

        /**
         * @brief Compares inequality with another vector.
         * @param other Vector to compare.
         * @return True if vectors are not equal.
         */
        bool
            operator!=(const CVector2& other) const {
            return !(*this == other);
        }

        /**
         * @brief Access vector components by index.
         * @param index Index (0 for x, 1 for y).
         * @return Reference to the corresponding component.
         */
        float&
            operator[](int index) {
            return (index == 0) ? x : y;
        }

        /**
         * @brief Access vector components by index (const).
         * @param index Index (0 for x, 1 for y).
         * @return Const reference to the corresponding component.
         */
        const float&
            operator[](int index) const {
            return (index == 0) ? x : y;
        }

        /**
         * @brief Squared length of the vector.
         * @return x² + y².
         */
        float
            lengthSquare() const {
            return x * x + y * y;
        }

        /**
         * @brief Length (magnitude) of the vector.
         * @return Square root of the squared length.
         */
        float
            length() const {
            return (float)EngineMathLib::sqrt(lengthSquare());
        }

        /**
         * @brief Dot product with another vector.
         * @param other Another vector.
         * @return Dot product (x1*x2 + y1*y2).
         */
        float
            dot(const CVector2& other) const {
            return x * other.x + y * other.y;
        }

        /**
         * @brief Cross product in 2D (scalar value).
         * @param other Another vector.
         * @return Scalar cross product result.
         */
        float
            cross(const CVector2& other) const {
            return x * other.y - y * other.x;
        }

        /**
         * @brief Normalized vector (magnitude 1).
         * @return New normalized vector.
         */
        CVector2
            normalized() const {
            float len = length();
            return (len == 0.f) ? CVector2(0.f, 0.f) : (*this) / len;
        }

        /**
         * @brief Normalizes the current vector (modifies it).
         */
        void
            normalize() {
            float len = length();
            if (len != 0.f) {
                x /= len; y /= len;
            }
            else {
                x = y = 0.f;
            }
        }

        /**
         * @brief Distance between two vectors.
         * @param a First vector.
         * @param b Second vector.
         * @return Euclidean distance between a and b.
         */
        inline static float
            distance(const CVector2& a, const CVector2& b) {
            return (float)EngineMathLib::sqrt((b - a).lengthSquare());
        }

        /**
         * @brief Linear interpolation between two vectors.
         * @param a Start vector.
         * @param b End vector.
         * @param t Interpolation parameter [0,1].
         * @return Interpolated vector.
         */
        inline static CVector2
            lerp(const CVector2& a, const CVector2& b, float t) {
            return a + (b - a) * t;
        }

        /**
         * @brief Zero vector (0,0).
         * @return Vector with zero components.
         */
        inline static CVector2
            zero() {
            return CVector2(0.f, 0.f);
        }

        /**
         * @brief Vector with ones (1,1).
         * @return Vector with components equal to one.
         */
        inline static CVector2
            one() {
            return CVector2(1.f, 1.f);
        }

        /**
         * @brief Sets the position (components x,y).
         * @param position Vector with the desired position.
         */
        void
            setPosition(const CVector2& position) {
            x = position.x;
            y = position.y;
        }

        /**
         * @brief Moves the vector by adding an offset.
         * @param offset Offset vector.
         */
        void
            move(const CVector2& offset) {
            x += offset.x;
            y += offset.y;
        }

        /**
         * @brief Sets scale by multiplying by factors.
         * @param factors Vector with scale factors.
         */
        void
            setScale(const CVector2& factors) {
            x *= factors.x;
            y *= factors.y;
        }

        /**
         * @brief Scales the vector by multiplying by factors.
         * @param factors Vector with scale factors.
         */
        void
            scale(const CVector2& factors) {
            x *= factors.x;
            y *= factors.y;
        }

        /**
         * @brief Sets the origin (components x,y).
         * @param origin Vector with desired origin.
         */
        void
            setOrigin(const CVector2& origin) {
            x = origin.x;
            y = origin.y;
        }

        /**
         * @brief Output stream operator.
         * @param os Output stream.
         * @param v Vector to print.
         * @return Reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const CVector2& v) {
            os << "CVector2(" << v.x << ", " << v.y << ")";
            return os;
        }


    };
}