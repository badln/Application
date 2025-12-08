#ifndef VEC2_H
#define VEC2_H

#include "main.h"
#include "GLMmath.h"

class Vector2 {
public:
    
    double x, y;

    Vector2() { x = 0; y = 0; }
    Vector2(double i) { x = i; y = i; }
    Vector2(double ix, double iy) { x = ix; y = iy; }
    
    Vector2(glm::vec2 i) { x = i.x; y = i.y; }
    glm::vec2 glm() { return glm::vec2(x, y); }

    std::string str() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

    static Vector2 zero() { return Vector2(0); }
    static Vector2 one() { return Vector2(1); }

    static Vector2 Distance(Vector2 to, Vector2 from) {
        double X = (pow(from.x, (double)2) - pow(to.x, (double)2));
        double Y = (pow(from.y, (double)2) - pow(to.x, (double)2));

        return sqrt(X + Y);
    }
    static Vector2 RotateAround(Vector2 point, Vector2 origin, float theta) {
        double s = sin(theta);
        double c = cos(theta);

        point -= origin;

        Vector2 r;

        r.x = point.x * c - point.y * s;
        r.y = point.x * s + point.y * c;

        r += origin;

        return r;
    }

    static Vector2* Normalize(Vector2* v) {
        float mag = sqrt(
            pow(v->x, 2) +
            pow(v->y, 2)
        );
        v->x = v->x / mag;
        v->y = v->y / mag;
        return v;
    }

    Vector2 Normalized() {
        Vector2 v = *this;
        float mag = sqrt(
            pow(v.x, 2) +
            pow(v.y, 2)
        );
        v.x = v.x / mag;
        v.y = v.y / mag;
        return v;
    }
    double Length() {
        Vector2 v = *this;
        double mag = sqrt(
            (v.x * v.x) +
            (v.y * v.y)
        );
        return mag;
    }

    Vector2 operator-() const { return Vector2(-x, -y); }
    double operator[](int i) const {
        switch (i) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        default:
            throw ERROR_INDEX_OUT_OF_BOUNDS;
            return x;
            break;
        }
    }
    double& operator[](int i) { 
        switch (i) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        default:
            throw ERROR_INDEX_OUT_OF_BOUNDS;
            return x;
            break;
        }
    }

    Vector2& operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2& operator*=(double t) {
        x *= t;
        y *= t;
        return *this;
    }

    Vector2& operator/=(double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return x * x + y * y;
    }
};
inline std::ostream& operator<<(std::ostream& out, const Vector2& v) {
    return out << v.x << ' ' << v.y;
}

inline Vector2 operator+(const Vector2& u, const Vector2& v) {
    return Vector2(u.x + v.x, u.y + v.y);
}

inline Vector2 operator-(const Vector2& u, const Vector2& v) {
    return Vector2(u.x - v.x, u.y - v.y);
}

inline Vector2 operator*(const Vector2& u, const Vector2& v) {
    return Vector2(u.x * v.x, u.y * v.y);
}

inline Vector2 operator*(double t, const Vector2& v) {
    return Vector2(t * v.x, t * v.y);
}

inline Vector2 operator*(const Vector2& v, double t) {
    return t * v;
}

inline Vector2 operator/(const Vector2& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector2& u, const Vector2& v) {
    return u.x * v.x
        + u.y * v.y;
}

inline Vector2 cross(const Vector2& u, const Vector2& v) {
    return Vector2(u.x * v.y - u.y * v.x,
        u.y * v.x - u.x * v.y);
}

inline Vector2 unit_vector(const Vector2& v) {
    return v / v.length();
}

#endif
