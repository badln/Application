#ifndef VEC3_H
#define VEC3_H

#include "main.h"
#include "GLMmath.h"

class Vector3 {
public:
  
    double x, y, z;

    Vector3() { x = 0; y = 0; z = 0; }
    Vector3(double i) { x = i; y = i; z = i; }
    Vector3(double ix, double iy, double iz) { x = ix; y = iy; z = iz; }
    Vector3(Vector2 xy, double iz) { x = xy.x, y = xy.y, z = iz; }

    Vector3(glm::vec3 i) { x = i.x, y = i.x; z = i.z; }
    Vector3(glm::vec4 i) { x = i.x, y = i.x; z = i.z; }
    
    glm::vec3 glm() { return glm::vec3(x, y, z); }

    std::string str() { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }

    static Vector3 zero() { return Vector3(0); }
    static Vector3 one() { return Vector3(1); }

    static Vector3 XAxis() { return Vector3(1, 0, 0); }
    static Vector3 YAxis() { return Vector3(0, 1, 0); }
    static Vector3 ZAxis() { return Vector3(0, 0, 1); }

    Vector2 xy() const { return Vector2(x, y); }

    /// <summary>
    /// Rotates the vector by angle (in degrees) in the axis given.
    /// </summary>
    /// <param name="angle"></param>
    /// <param name="axis"></param>
    /// <returns></returns>
    Vector3 RotateAngleAxis(float angle, Vector3 axis) {
        
        glm::mat4 m = glm::rotate(glm::mat4(1), (float)glm::radians(angle), axis.glm());
        float pitch, yaw, roll;
        glm::extractEulerAngleYXZ(m, yaw, pitch, roll);

        x += glm::degrees(pitch);
        y += glm::degrees(yaw);
        z += glm::degrees(roll);
        return *this;
    }


    static float Distance(Vector3 to, Vector3 from) {
        double X = (pow(from.x, (double)2) - pow(to.x, (double)2));
        double Y = (pow(from.y, (double)2) - pow(to.y, (double)2));
        double Z = (pow(from.z, (double)2) - pow(to.z, (double)2));

        return sqrt(X + Y + Z);
    }


    Vector3 Normalize() {
        *this = Normalized();
    }

    Vector3 Normalized() {
        Vector3 v = *this;
        double len = this->length();
        v.x /= len;
        v.y /= len;
        v.z /= len;
        return v;
    }

    Vector3 operator-() const { return Vector3(-x, -y, -z); }
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
        default:
            throw ERROR_INDEX_OUT_OF_BOUNDS;
            return x;
            break;
        }
    }

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator*=(double t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    Vector3& operator/=(double t) {
        return *this *= 1 / t;
    }
    bool operator!=(const Vector3& v) {
        if (v.x == x)
            return false;
        if (v.y == y)
            return false;
        if (v.z == z)
            return false;

        return true; 
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return x * x + y * y + z * z;
    }

};

inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
    return Vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) {
    return Vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) {
    return Vector3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vector3 operator*(double t, const Vector3& v) {
    return Vector3(t * v.x, t * v.y, t * v.z);
}

inline Vector3 operator*(const Vector3& v, double t) {
    return t * v;
}

inline Vector3 operator/(const Vector3& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector3& u, const Vector3& v) {
    return u.x * v.x
        + u.y * v.y
        + u.z * v.z;
}

inline Vector3 cross(const Vector3& u, const Vector3& v) {
    return Vector3(u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x);
}

inline Vector3 unit_vector(const Vector3& v) {
    return v / v.length();
}

using ColourRGB = Vector3;

#endif