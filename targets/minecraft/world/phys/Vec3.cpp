#include "minecraft/world/phys/Vec3.h"

#include <cmath>
#include <format>
#include <optional>
#include <string>

#include "minecraft/world/phys/AABB.h"

Vec3 Vec3::vectorTo(const Vec3& p) const { return {p.x - x, p.y - y, p.z - z}; }

Vec3 Vec3::normalize() const {
    double dist = std::sqrt(x * x + y * y + z * z);
    if (dist < 0.0001) return {0, 0, 0};

    return {x / dist, y / dist, z / dist};
}

double Vec3::dot(const Vec3& p) const { return x * p.x + y * p.y + z * p.z; }

Vec3 Vec3::cross(const Vec3& p) const {
    return {y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x};
}

Vec3 Vec3::add(double x, double y, double z) const {
    return {this->x + x, this->y + y, this->z + z};
}

double Vec3::distanceTo(const Vec3& p) const {
    double xd = p.x - x;
    double yd = p.y - y;
    double zd = p.z - z;
    return std::sqrt(xd * xd + yd * yd + zd * zd);
}

double Vec3::distanceToSqr(const Vec3& p) const {
    double xd = p.x - x;
    double yd = p.y - y;
    double zd = p.z - z;
    return xd * xd + yd * yd + zd * zd;
}

double Vec3::distanceToSqr(const double x2, const double y2,
                           const double z2) const {
    double xd = x2 - x;
    double yd = y2 - y;
    double zd = z2 - z;
    return xd * xd + yd * yd + zd * zd;
}

Vec3 Vec3::scale(const double l) const { return {x * l, y * l, z * l}; }

double Vec3::length() const { return sqrt(x * x + y * y + z * z); }

std::optional<Vec3> Vec3::clipX(const Vec3& b, const double xt) const {
    double xd = b.x - x;
    double yd = b.y - y;
    double zd = b.z - z;

    if (xd * xd < 0.0000001f) return std::nullopt;

    double d = (xt - x) / xd;
    if (d < 0 || d > 1) return std::nullopt;

    return Vec3{x + xd * d, y + yd * d, z + zd * d};
}

std::optional<Vec3> Vec3::clipY(const Vec3& b, const double yt) const {
    double xd = b.x - x;
    double yd = b.y - y;
    double zd = b.z - z;

    if (yd * yd < 0.0000001f) return std::nullopt;

    double d = (yt - y) / yd;
    if (d < 0 || d > 1) return std::nullopt;

    return Vec3{x + xd * d, y + yd * d, z + zd * d};
}

std::optional<Vec3> Vec3::clipZ(const Vec3& b, const double zt) const {
    double xd = b.x - x;
    double yd = b.y - y;
    double zd = b.z - z;

    if (zd * zd < 0.0000001f) return std::nullopt;

    double d = (zt - z) / zd;
    if (d < 0 || d > 1) return std::nullopt;

    return Vec3{x + xd * d, y + yd * d, z + zd * d};
}

std::string Vec3::toString() const {
    return std::format("({},{},{})", x, y, z);
}

Vec3 Vec3::lerp(const Vec3& v, const double a) const {
    return {x + (v.x - x) * a, y + (v.y - y) * a, z + (v.z - z) * a};
}

void Vec3::xRot(const float degs) {
    double _cos = cos(degs);  // 4J - cos/sin were floats but seems pointless
                              // wasting precision here
    double _sin = sin(degs);

    double xx = x;
    double yy = y * _cos + z * _sin;
    double zz = z * _cos - y * _sin;

    x = xx;
    y = yy;
    z = zz;
}

void Vec3::yRot(const float degs) {
    double _cos = cos(degs);  // 4J - cos/sin were floats but seems pointless
                              // wasting precision here
    double _sin = sin(degs);

    double xx = x * _cos + z * _sin;
    double yy = y;
    double zz = z * _cos - x * _sin;

    x = xx;
    y = yy;
    z = zz;
}

void Vec3::zRot(const float degs) {
    double _cos = cos(degs);  // 4J - cos/sin were floats but seems pointless
                              // wasting precision here
    double _sin = sin(degs);

    double xx = x * _cos + y * _sin;
    double yy = y * _cos - x * _sin;
    double zz = z;

    x = xx;
    y = yy;
    z = zz;
}

// Returns 0 if this point is within the box
// Otherwise returns the distance to the box
// TODO: rewrite this function
double Vec3::distanceTo(AABB* box) {
    if (box->contains(*this)) return 0;

    double xd = 0, yd = 0, zd = 0;

    if (x < box->x0)
        xd = box->x0 - x;
    else if (x > box->x1)
        xd = x - box->x1;

    if (y < box->y0)
        yd = box->y0 - y;
    else if (y > box->y1)
        yd = y - box->y1;

    if (z < box->z0)
        zd = box->z0 - z;
    else if (z > box->z1)
        zd = z - box->z1;

    return sqrt(xd * xd + yd * yd + zd * zd);
}

Vec3 Vec3::closestPointOnLine(const Vec3& p1, const Vec3& p2) const {
    Vec3 diff = {x - p1.x, y - p1.y, z - p1.z};
    Vec3 dir = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    float dot1 = diff.dot(dir);

    if (dot1 <= 0.0f) return p1;

    float dot2 = dir.dot(dir);

    if (dot2 <= dot1) return p2;

    float t = dot1 / dot2;

    return {p1.x + t * dir.x, p1.y + t * dir.y, p1.z + t * dir.z};
}

double Vec3::distanceFromLine(const Vec3& p1, const Vec3& p2) const {
    Vec3 closestPoint = closestPointOnLine(p1, p2);
    Vec3 diff{x - closestPoint.x, y - closestPoint.y, z - closestPoint.z};
    return diff.length();
}
