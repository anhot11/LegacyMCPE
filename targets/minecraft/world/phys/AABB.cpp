// package net.minecraft.world.phys;

// import java->util.ArrayList;
// import java->util.List;

// TODO: use brace initialization everywhere

#include "minecraft/world/phys/AABB.h"

#include <algorithm>
#include <format>
#include <optional>
#include <string>

#include "HitResult.h"
#include "minecraft/world/phys/Vec3.h"

AABB::AABB(double x0, double y0, double z0, double x1, double y1, double z1) {
    this->x0 = x0;
    this->y0 = y0;
    this->z0 = z0;
    this->x1 = x1;
    this->y1 = y1;
    this->z1 = z1;
}

AABB AABB::expand(double xa, double ya, double za) const {
    double _x0 = x0;
    double _y0 = y0;
    double _z0 = z0;
    double _x1 = x1;
    double _y1 = y1;
    double _z1 = z1;

    if (xa < 0) _x0 += xa;
    if (xa > 0) _x1 += xa;

    if (ya < 0) _y0 += ya;
    if (ya > 0) _y1 += ya;

    if (za < 0) _z0 += za;
    if (za > 0) _z1 += za;

    return {_x0, _y0, _z0, _x1, _y1, _z1};
}

AABB AABB::grow(const double xa, const double ya, const double za) const {
    double _x0 = x0 - xa;
    double _y0 = y0 - ya;
    double _z0 = z0 - za;
    double _x1 = x1 + xa;
    double _y1 = y1 + ya;
    double _z1 = z1 + za;

    return {_x0, _y0, _z0, _x1, _y1, _z1};
}

AABB AABB::minmax(const AABB& other) const {
    double _x0 = std::min(x0, other.x0);
    double _y0 = std::min(y0, other.y0);
    double _z0 = std::min(z0, other.z0);
    double _x1 = std::max(x1, other.x1);
    double _y1 = std::max(y1, other.y1);
    double _z1 = std::max(z1, other.z1);

    return {_x0, _y0, _z0, _x1, _y1, _z1};
}

double AABB::clipXCollide(const AABB& c, double xa) const {
    if (c.y1 <= y0 || c.y0 >= y1) return xa;
    if (c.z1 <= z0 || c.z0 >= z1) return xa;

    if (xa > 0 && c.x1 <= x0) {
        double max = x0 - c.x1;
        if (max < xa) xa = max;
    }

    if (xa < 0 && c.x0 >= x1) {
        double max = x1 - c.x0;
        if (max > xa) xa = max;
    }

    return xa;
}

double AABB::clipYCollide(const AABB& c, double ya) const {
    if (c.x1 <= x0 || c.x0 >= x1) return ya;
    if (c.z1 <= z0 || c.z0 >= z1) return ya;

    if (ya > 0 && c.y1 <= y0) {
        double max = y0 - c.y1;
        if (max < ya) ya = max;
    }

    if (ya < 0 && c.y0 >= y1) {
        double max = y1 - c.y0;
        if (max > ya) ya = max;
    }

    return ya;
}

double AABB::clipZCollide(const AABB& c, double za) const {
    if (c.x1 <= x0 || c.x0 >= x1) return za;
    if (c.y1 <= y0 || c.y0 >= y1) return za;

    if (za > 0 && c.z1 <= z0) {
        double max = z0 - c.z1;
        if (max < za) za = max;
    }

    if (za < 0 && c.z0 >= z1) {
        double max = z1 - c.z0;
        if (max > za) za = max;
    }

    return za;
}

bool AABB::intersects(const AABB& c) const {
    if (c.x1 <= x0 || c.x0 >= x1) return false;
    if (c.y1 <= y0 || c.y0 >= y1) return false;
    if (c.z1 <= z0 || c.z0 >= z1) return false;
    return true;
}

AABB AABB::move(const double xa, const double ya, const double za) const {
    return {
        x0 + xa, y0 + ya, z0 + za,

        x1 + xa, y1 + ya, z1 + za,
    };
}

bool AABB::intersects(const double x02, const double y02, const double z02,
                      const double x12, const double y12,
                      const double z12) const {
    if (x12 <= x0 || x02 >= x1) return false;
    if (y12 <= y0 || y02 >= y1) return false;
    if (z12 <= z0 || z02 >= z1) return false;

    return true;
}

bool AABB::contains(const Vec3& p) const {
    if (p.x <= x0 || p.x >= x1) return false;
    if (p.y <= y0 || p.y >= y1) return false;
    if (p.z <= z0 || p.z >= z1) return false;

    return true;
}

// 4J Added
bool AABB::containsIncludingLowerBound(const Vec3& p) const {
    if (p.x < x0 || p.x >= x1) return false;
    if (p.y < y0 || p.y >= y1) return false;
    if (p.z < z0 || p.z >= z1) return false;
    return true;
}

double AABB::getSize() const {
    const double xs = x1 - x0;
    const double ys = y1 - y0;
    const double zs = z1 - z0;

    return (xs + ys + zs) / 3.0f;
}

AABB AABB::shrink(const double xa, const double ya, const double za) const {
    double _x0 = x0 + xa;
    double _y0 = y0 + ya;
    double _z0 = z0 + za;
    double _x1 = x1 - xa;
    double _y1 = y1 - ya;
    double _z1 = z1 - za;

    return {_x0, _y0, _z0, _x1, _y1, _z1};
}

HitResult* AABB::clip(const Vec3& a, const Vec3& b) const {
    auto xh0 = a.clipX(b, x0);
    auto xh1 = a.clipX(b, x1);

    auto yh0 = a.clipY(b, y0);
    auto yh1 = a.clipY(b, y1);

    auto zh0 = a.clipZ(b, z0);
    auto zh1 = a.clipZ(b, z1);

    if (!containsX(xh0)) xh0 = std::nullopt;
    if (!containsX(xh1)) xh1 = std::nullopt;
    if (!containsY(yh0)) yh0 = std::nullopt;
    if (!containsY(yh1)) yh1 = std::nullopt;
    if (!containsZ(zh0)) zh0 = std::nullopt;
    if (!containsZ(zh1)) zh1 = std::nullopt;

    std::optional<Vec3> closest = std::nullopt;

    if (xh0.has_value() && (!closest.has_value() ||
                            a.distanceToSqr(*xh0) < a.distanceToSqr(*closest)))
        closest = xh0;

    if (xh1.has_value() && (!closest.has_value() ||
                            a.distanceToSqr(*xh1) < a.distanceToSqr(*closest)))
        closest = xh1;

    if (yh0.has_value() && (!closest.has_value() ||
                            a.distanceToSqr(*yh0) < a.distanceToSqr(*closest)))
        closest = yh0;

    if (yh1.has_value() && (!closest.has_value() ||
                            a.distanceToSqr(*yh1) < a.distanceToSqr(*closest)))
        closest = yh1;

    if (zh0.has_value() && (!closest.has_value() ||
                            a.distanceToSqr(*zh0) < a.distanceToSqr(*closest)))
        closest = zh0;

    if (zh1.has_value() && (!closest.has_value() ||
                            a.distanceToSqr(*zh1) < a.distanceToSqr(*closest)))
        closest = zh1;

    if (!closest.has_value()) return nullptr;

    int face = -1;

    if (closest == xh0) face = 4;
    if (closest == xh1) face = 5;
    if (closest == yh0) face = 0;
    if (closest == yh1) face = 1;
    if (closest == zh0) face = 2;
    if (closest == zh1) face = 3;

    return new HitResult(0, 0, 0, face, *closest);
}

bool AABB::containsX(const std::optional<Vec3>& v) const {
    if (!v.has_value()) return false;
    return v->y >= y0 && v->y <= y1 && v->z >= z0 && v->z <= z1;
}

bool AABB::containsY(const std::optional<Vec3>& v) const {
    if (!v.has_value()) return false;
    return v->x >= x0 && v->x <= x1 && v->z >= z0 && v->z <= z1;
}

bool AABB::containsZ(const std::optional<Vec3>& v) const {
    if (!v.has_value()) return false;
    return v->x >= x0 && v->x <= x1 && v->y >= y0 && v->y <= y1;
}

std::string AABB::toString() const {
    return std::format("box[{}, {}, {}, {}, {}, {}]", x0, y0, z0, x1, y1, z1);
}
