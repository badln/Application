#ifndef VEC2_H
#define VEC2_H

#include "main.h"

class Vector2 {
public:
    double e[2];

    Vector2() : e{ 0,0 } {}
    Vector2(double x) : e{ x,x } {}
    Vector2(double e0, double e1) : e{ e0, e1 } {}
    
    Vector2(glm::vec2 x) : e { x.x, x.y } {}
    glm::vec2 glm() { return glm::vec2(e[0], e[1]); }

    std::string str() { return "(" + std::to_string(e[0]) + ", " + std::to_string(e[1]) + ")"; }

    static Vector2 zero() { return Vector2(0); }
    static Vector2 one() { return Vector2(1); }

    double x() const { return e[0]; }
    double r() const { return e[0]; }

    double y() const { return e[1]; }
    double g() const { return e[1]; }

    Vector2 operator-() const { return Vector2(-e[0], -e[1]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vector2& operator+=(const Vector2& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

    Vector2& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        return *this;
    }

    Vector2& operator/=(double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1];
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vector2& v) {
    return out << v.e[0] << ' ' << v.e[1];
}

inline Vector2 operator+(const Vector2& u, const Vector2& v) {
    return Vector2(u.e[0] + v.e[0], u.e[1] + v.e[1]);
}

inline Vector2 operator-(const Vector2& u, const Vector2& v) {
    return Vector2(u.e[0] - v.e[0], u.e[1] - v.e[1]);
}

inline Vector2 operator*(const Vector2& u, const Vector2& v) {
    return Vector2(u.e[0] * v.e[0], u.e[1] * v.e[1]);
}

inline Vector2 operator*(double t, const Vector2& v) {
    return Vector2(t * v.e[0], t * v.e[1]);
}

inline Vector2 operator*(const Vector2& v, double t) {
    return t * v;
}

inline Vector2 operator/(const Vector2& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector2& u, const Vector2& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1];
}

inline Vector2 cross(const Vector2& u, const Vector2& v) {
    return Vector2(u.e[0] * v.e[1] - u.e[1] * v.e[0],
        u.e[1] * v.e[0] - u.e[0] * v.e[1]);
}

inline Vector2 unit_vector(const Vector2& v) {
    return v / v.length();
}

#endif
