#pragma once

#include <optional>
#include <string>

class AABB;

class Vec3 {
public:
    double x, y, z;

    constexpr Vec3() = default;
    constexpr Vec3(const double x, const double y, const double z)
        : x(x), y(y), z(z) {}

    Vec3 vectorTo(const Vec3& p) const;
    Vec3 normalize() const;
    double dot(const Vec3& p) const;
    Vec3 cross(const Vec3& p) const;
    Vec3 add(double x, double y, double z) const;
    double distanceTo(const Vec3& p) const;
    double distanceToSqr(const Vec3& p) const;
    double distanceToSqr(double x2, double y2, double z2) const;
    Vec3 scale(double l) const;
    double length() const;
    std::optional<Vec3> clipX(const Vec3& b, double xt) const;
    std::optional<Vec3> clipY(const Vec3& b, double yt) const;
    std::optional<Vec3> clipZ(const Vec3& b, double zt) const;
    std::string toString() const;
    Vec3 lerp(const Vec3& v, double a) const;
    void xRot(float degs);
    void yRot(float degs);
    void zRot(float degs);

    // 4J Added
    double distanceTo(AABB* box);

    Vec3 closestPointOnLine(const Vec3& p1, const Vec3& p2) const;
    double distanceFromLine(const Vec3& p1, const Vec3& p2) const;

    constexpr bool operator==(const Vec3& rhs) const {
        return x == rhs.x and y == rhs.y and z == rhs.z;
    }
};
