#pragma once
#include "Prerequisitos.h"

namespace EngineMathLib {

    /**
     * @brief Useful mathematical constant: PI
     */
    const double PI = 3.14159265358979323846;

    /**
     * @brief Euler's number (e)
     */
    const double EULER = 2.71828182845904523536;

    const double EPSILON = 1e-6;

    // === BASIC FUNCTIONS ===

    /**
     * @brief Square root using Newton-Raphson method.
     *
     * Computes the approximate square root of x using 20 iterations of the
     * Newton-Raphson method. Returns NaN if x is negative.
     *
     * @param x Value to calculate the square root of.
     * @return Approximate square root or NaN if x < 0.
     */
    inline double
        sqrt(double x) {
        if (x < 0.0) return NAN; // NaN simulation for negative square root
        double guess = x / 2.0;
        for (int i = 0; i < 20; ++i) {
            guess = (guess + x / guess) / 2.0;
        }
        return guess;
    }

    /**
     * @brief Returns the square of x.
     *
     * @param x Value to square.
     * @return Square of x.
     */
    inline double
        square(double x) {
        return x * x;
    }

    /**
     * @brief Returns the cube of x.
     *
     * @param x Value to cube.
     * @return Cube of x.
     */
    inline double
        cube(double x) {
        return x * x * x;
    }

    /**
     * @brief Calculates base raised to exponent (approximation).
     *
     * Approximates the power function base^exponent using exp and log.
     * Returns NaN for invalid inputs such as 0^0, 0^negative, or negative bases.
     *
     * @param base Base value.
     * @param exponent Exponent value.
     * @return Result of base raised to exponent.
     */
    inline double
        power(double base, double exponent) {
        if (base == 0.0) {
            if (exponent <= 0.0) return NAN; // Undefined (0^0 or 0^negative)
            return 0.0;
        }
        if (exponent == 0.0) return 1.0;
        if (base < 0.0) return NAN; // Simplification: no negative bases handled

        return exp(exponent * log(base));
    }

    /**
     * @brief Absolute value of x.
     *
     * @param x Input value.
     * @return Absolute value of x.
     */
    inline double
        abs(double x) {
        return x < 0 ? -x : x;
    }

    /**
     * @brief Maximum of a and b.
     *
     * @param a First value.
     * @param b Second value.
     * @return Maximum of a and b.
     */
    inline double
        EMax(double a, double b) {
        return a > b ? a : b;
    }

    /**
     * @brief Minimum of a and b.
     *
     * @param a First value.
     * @param b Second value.
     * @return Minimum of a and b.
     */
    inline double
        EMin(double a, double b) {
        return a < b ? a : b;
    }

    /**
     * @brief Rounds x to the nearest integer.
     *
     * @param x Value to round.
     * @return Rounded integer.
     */
    inline int
        round(double x) {
        return (x >= 0.0) ? int(x + 0.5) : int(x - 0.5);
    }

    /**
     * @brief Returns the greatest integer less than or equal to x.
     *
     * @param x Input value.
     * @return Floor of x.
     */
    inline int
        floor(double x) {
        int i = int(x);
        return (x < 0.0 && x != i) ? i - 1 : i;
    }

    /**
     * @brief Returns the smallest integer greater than or equal to x.
     *
     * @param x Input value.
     * @return Ceiling of x.
     */
    inline int
        ceil(double x) {
        int i = int(x);
        return (x > 0.0 && x != i) ? i + 1 : i;
    }

    /**
     * @brief Floating-point absolute value.
     *
     * @param x Input value.
     * @return Absolute value of x.
     */
    inline double
        fabs(double x) { return x < 0 ? -x : x; }

    /**
     * @brief Modulo operation for real numbers.
     *
     * @param a Dividend.
     * @param b Divisor.
     * @return Remainder of a divided by b.
     */
    inline double
        mod(double a, double b) {
        while (a >= b) a -= b;
        while (a < 0) a += b;
        return a;
    }

    /**
     * @brief Exponential function approximation (e^x).
     *
     * @param x Exponent value.
     * @return Approximate value of e^x.
     */
    inline double
        exp(double x) {
        double result = 1.0;
        double term = 1.0;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= x / n++;
            result += term;
        }
        return result;
    }

    /**
     * @brief Natural logarithm approximation (ln x).
     *
     * @param x Input value (must be > 0).
     * @return Approximate natural logarithm of x.
     */
    inline double
        log(double x) {
        if (x <= 0) return NAN;
        double y = (x - 1) / (x + 1);
        double result = 0.0;
        double term = y;
        int n = 1;
        while (fabs(term) > EPSILON) {
            result += term / (2 * n - 1);
            term *= y * y;
            ++n;
        }
        return 2 * result;
    }

    /**
     * @brief Base-10 logarithm approximation (log₁₀).
     *
     * @param x Input value.
     * @return Approximate base-10 logarithm of x.
     */
    inline double
        log10(double x) {
        const double ln10 = 2.302585093;
        return log(x) / ln10;
    }

    // === TRIGONOMETRY FUNCTIONS ===

    /**
     * @brief Converts degrees to radians.
     *
     * @param degrees Angle in degrees.
     * @return Angle in radians.
     */
    inline double
        radians(double degrees) {
        return degrees * (PI / 180.0);
    }

    /**
     * @brief Converts radians to degrees.
     *
     * @param radians Angle in radians.
     * @return Angle in degrees.
     */
    inline double
        degrees(double radians) {
        return radians * (180.0 / PI);
    }

    /**
     * @brief Sine function using Taylor series.
     *
     * @param x Angle in radians.
     * @return Approximate sine of x.
     */
    inline double
        sin(double x) {
        x = mod(x, 2 * PI);
        double result = x;
        double term = x;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= -x * x / ((2 * n) * (2 * n + 1));
            result += term;
            ++n;
        }
        return result;
    }

    /**
     * @brief Cosine function using Taylor series.
     *
     * @param x Angle in radians.
     * @return Approximate cosine of x.
     */
    inline double
        cos(double x) {
        x = mod(x, 2 * PI);
        double result = 1.0;
        double term = 1.0;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= -x * x / ((2 * n - 1) * (2 * n));
            result += term;
            ++n;
        }
        return result;
    }

    /**
     * @brief Tangent function calculated as sin(x)/cos(x).
     *
     * @param x Angle in radians.
     * @return Approximate tangent of x, or INFINITY if cos(x) == 0.
     */
    inline double
        tan(double x) {
        double s = sin(x);
        double c = cos(x);
        return c != 0 ? s / c : INFINITY; // Infinity if cos is 0
    }

    /**
     * @brief Inverse sine approximation.
     *
     * @param x Input value in range [-1, 1].
     * @return Approximate arcsin of x or NaN if out of range.
     */
    inline double
        asin(double x) {
        if (x < -1 || x > 1) return NAN; // NaN
        double result = x;
        double term = x;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= (2.0 * n - 1) * (2.0 * n - 1) * x * x
                / ((2.0 * n) * (2.0 * n + 1));
            result += term;
            ++n;
        }
        return result;
    }

    /**
     * @brief Inverse cosine, calculated from asin.
     *
     * @param x Input value.
     * @return Approximate arccos of x.
     */
    inline double
        acos(double x) {
        return PI / 2 - asin(x);
    }

    /**
     * @brief Inverse tangent approximation.
     *
     * @param x Input value.
     * @return Approximate arctan of x.
     */
    inline double
        atan(double x) {
        double result = x;
        double term = x;
        int n = 1;
        while (fabs(term) > EPSILON) {
            term *= -x * x * (2.0 * n - 1) / (2.0 * n + 1);
            result += term;
            ++n;
        }
        return result;
    }

    /**
     * @brief Hyperbolic sine.
     *
     * @param x Input value.
     * @return Hyperbolic sine of x.
     */
    inline double
        sinh(double x) {
        return (exp(x) - exp(-x)) / 2.0;
    }

    /**
     * @brief Hyperbolic cosine.
     *
     * @param x Input value.
     * @return Hyperbolic cosine of x.
     */
    inline double
        cosh(double x) {
        return (exp(x) + exp(-x)) / 2.0;
    }

    /**
     * @brief Hyperbolic tangent.
     *
     * @param x Input value.
     * @return Hyperbolic tangent of x.
     */
    inline double
        tanh(double x) {
        double e2x = exp(2 * x);
        return (e2x - 1) / (e2x + 1);
    }

    // === GEOMETRIC FUNCTIONS ===

    /**
     * @brief Area of a circle.
     *
     * @param radius Radius of the circle.
     * @return Area of the circle.
     */
    inline double
        circleArea(double radius) {
        return PI * radius * radius;
    }

    /**
     * @brief Circumference of a circle.
     *
     * @param radius Radius of the circle.
     * @return Circumference of the circle.
     */
    inline double
        circleCircumference(double radius) {
        return 2 * PI * radius;
    }

    /**
     * @brief Area of a rectangle.
     *
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     * @return Area of the rectangle.
     */
    inline double
        rectangleArea(double width, double height) {
        return width * height;
    }

    /**
     * @brief Perimeter of a rectangle.
     *
     * @param width Width of the rectangle.
     * @param height Height of the rectangle.
     * @return Perimeter of the rectangle.
     */
    inline  double
        rectanglePerimeter(double width, double height) {
        return 2 * (width + height);
    }

    /**
     * @brief Area of a triangle.
     *
     * @param base Base length.
     * @param height Height of the triangle.
     * @return Area of the triangle.
     */
    inline double
        triangleArea(double base, double height) {
        return 0.5 * base * height;
    }

    /**
     * @brief Euclidean distance between two points.
     *
     * @param x1 X coordinate of the first point.
     * @param y1 Y coordinate of the first point.
     * @param x2 X coordinate of the second point.
     * @param y2 Y coordinate of the second point.
     * @return Distance between the two points.
     */
    inline double
        distance(double x1, double y1, double x2, double y2) {
        double dx = x2 - x1;
        double dy = y2 - y1;
        return sqrt(dx * dx + dy * dy);
    }

    // === OTHER USEFUL FEATURES ===

    /**
     * @brief Linear interpolation between a and b.
     *
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation parameter (0 <= t <= 1).
     * @return Interpolated value.
     */
    inline double
        lerp(double a, double b, double t) {
        return a + (b - a) * t;
    }

    /**
     * @brief Factorial of a non-negative integer.
     *
     * @param n Non-negative integer.
     * @return Factorial of n or NaN if n < 0.
     */
    inline double
        factorial(int n) {
        if (n < 0) return NAN; // NaN
        double result = 1.0;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    /**
     * @brief Compares two floating-point values with epsilon tolerance.
     *
     * @param a First value.
     * @param b Second value.
     * @param epsilon Tolerance value (default 1e-6).
     * @return True if values are approximately equal within epsilon.
     */
    inline bool
        approxEqual(double a, double b, double epsilon = 1e-6) {
        return abs(a - b) < epsilon;
    }

}