#ifndef VEC4_H
#define VEC4_H

#include "main.h"
#define glClearColorV4(object) glClearColor(object.x, object.y, object.z, object.w)
#include "GLMmath.h"

class Vector4 {
public:
    double x, y, z, w;

    Vector4() { x = 0; y = 0; z = 0; w = 0; }
    Vector4(double i) { x = i; y = i; z = i; w = i; }
    Vector4(double ix, double iy, double iz, double iw) { x = ix; y = iy; z = iz; w = iw; }
    Vector4(Vector3 xy, double iz, double iw) { x = xy.x; y = xy.y; z = iz; w = iw; }
    Vector4(Vector3 xyz, double iw) { x = xyz.x; y = xyz.y; z = xyz.z; w = iw; }

    Vector4(glm::vec4 i) { x = i.x; y = i.y; z = i.z; w = i.w; }
    glm::vec4 glm() { return glm::vec4(x, y, z, w); }

    std::string str() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ")"; }

    static Vector4 zero() { return Vector4(0); }
    static Vector4 one() { return Vector4(1); }

    Vector3 xyz() const { return Vector3(x, y, z); }
    Vector2 xy() const { return Vector2(x, y); }

    static Vector4 Distance(Vector4 to, Vector4 from) {
        double X = (pow(from.x, (double)2) - pow(to.x, (double)2));
        double Y = (pow(from.y, (double)2) - pow(to.y, (double)2));
        double Z = (pow(from.z, (double)2) - pow(to.z, (double)2));
        double W = (pow(from.w, (double)2) - pow(to.w, (double)2));

        return sqrt(X + Y + Z + W);
    }


    static Vector4* Normalize(Vector4* v) {
        float mag = sqrt(
            pow(v->x, 2) +
            pow(v->y, 2) +
            pow(v->z, 2) +
            pow(v->w, 3)
        );
        v->x = v->x / mag;
        v->y = v->y / mag;
        v->z = v->z / mag;
        v->w = v->w / mag;
        return v;
    }

    Vector4 Normalized() {
        Vector4 v = *this;
        float mag = sqrt(
            pow(v.x, 2) +
            pow(v.y, 2) +
            pow(v.z, 2) + 
            pow(v.w, 3)
        );
        v.x = v.x / mag;
        v.y = v.y / mag;
        v.z = v.z / mag;
        v.w = v.w / mag;
        return v;
    }

    double Length() {
        Vector4 v = *this;
        double mag = sqrt(
            (v.x * v.x) +
            (v.y * v.y) +
            (v.z * v.z) +
            (v.w * v.w)
        );
        return mag;
    }


    Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
    double operator[](int i) const { 
        switch (i) {
        case 0:
            return x;
            break;
        case 1:
            return y;
            break;
        case 2:
            return z;
            break;
        case 3:
            return w;
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
        case 2:
            return z;
            break;
        case 3:
            return w;
            break;
        default:
            throw ERROR_INDEX_OUT_OF_BOUNDS;
            return x;
            break;
        }
    }

    Vector4& operator+=(const Vector4& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }
    Vector4& operator-=(const Vector4& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    Vector4& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        w *= t;
        return *this;
    }

    Vector4& operator/=(double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return x * x + y * y + z * z + w * w;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vector4& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vector4 operator+(const Vector4& u, const Vector4& v) {
    return Vector4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v[3]);
}

inline Vector4 operator-(const Vector4& u, const Vector4& v) {
    return Vector4(u.x - v.x, u.y - v.y, u.z - v.z, u[3] - v[3]);
}

inline Vector4 operator*(const Vector4& u, const Vector4& v) {
    return Vector4(u.x * v.x, u.y * v.y, u.z * v.z, u[3] * v[3]);
}

inline Vector4 operator*(double t, const Vector4& v) {
    return Vector4(t * v.x, t * v.y, t * v.z, t * v[3]);
}

inline Vector4 operator*(const Vector4& v, double t) {
    return t * v;
}

inline Vector4 operator/(const Vector4& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector4& u, const Vector4& v) {
    return u.x * v.x
        + u.y * v.y
        + u.z * v.z
        + u.w * v.w;
}

inline Vector4 unit_vector(const Vector4& v) {
    return v / v.length();
}

using Rect = Vector4;
using Viewport = Vector4;
using Colour = Vector4;

#endif