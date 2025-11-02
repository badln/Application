#ifndef VEC4_H
#define VEC4_H

#include "main.h"

class Vector4 {
public:
    double e[4];

    Vector4() : e{ 0,0,0,0 } {}
    Vector4(double x) : e{ x, x, x, x } {}
    Vector4(double e0, double e1, double e2, double e3) : e{ e0, e1, e2, e3 } {}

    Vector4(glm::vec4 x) : e{ x.x, x.y, x.z, x.w } {}
    glm::vec4 glm() { return glm::vec4(e[0], e[1], e[2], e[3]); }

    std::string str() { return "(" + std::to_string(e[0]) + ", " + std::to_string(e[1]) + ", " + std::to_string(e[2]) + ", " + std::to_string(e[3]) + ")"; }

    static Vector4 zero() { return Vector4(0); }
    static Vector4 one() { return Vector4(1); }

    double x() const { return e[0]; }
    double r() const { return e[0]; }

    double y() const { return e[1]; }
    double g() const { return e[1]; }

    double z() const { return e[2]; }
    double b() const { return e[2]; }
    double width() const { return e[2]; }

    double w() const { return e[3]; }
    double a() const { return e[3]; }
    double height() const { return e[3]; }

    Vector4 operator-() const { return Vector4(-e[0], -e[1], -e[2], -e[3]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vector4& operator+=(const Vector4& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        e[3] += v.e[3];
        return *this;
    }

    Vector4& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        e[3] *= t;
        return *this;
    }

    Vector4& operator/=(double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3];
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vector4& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector4 operator+(const Vector4& u, const Vector4& v) {
    return Vector4(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2], u.e[3] + v[3]);
}

inline Vector4 operator-(const Vector4& u, const Vector4& v) {
    return Vector4(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2], u[3] - v[3]);
}

inline Vector4 operator*(const Vector4& u, const Vector4& v) {
    return Vector4(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2], u[3] * v[3]);
}

inline Vector4 operator*(double t, const Vector4& v) {
    return Vector4(t * v.e[0], t * v.e[1], t * v.e[2], t * v[3]);
}

inline Vector4 operator*(const Vector4& v, double t) {
    return t * v;
}

inline Vector4 operator/(const Vector4& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector4& u, const Vector4& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2]
        + u.e[3] * v.e[3];
}

inline Vector4 unit_vector(const Vector4& v) {
    return v / v.length();
}

using Rect = Vector4;
using Colour = Vector4;

#endif