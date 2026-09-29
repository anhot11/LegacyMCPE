#pragma once

#include <optional>
#include <string>

#include "Vec3.h"

class HitResult;
class Vec3;

class AABB {
public:
    double x0, y0, z0;
    double x1, y1, z1;

    AABB() {}
    AABB(double x0, double y0, double z0, double x1, double y1, double z1);

    AABB expand(double xa, double ya, double za) const;
    AABB grow(double xa, double ya, double za) const;
    AABB minmax(const AABB& other) const;
    double clipXCollide(const AABB& c, double xa) const;
    double clipYCollide(const AABB& c, double ya) const;
    double clipZCollide(const AABB& c, double za) const;
    bool intersects(const AABB& c) const;
    AABB move(double xa, double ya, double za) const;
    bool intersects(double x02, double y02, double z02, double x12, double y12,
                    double z12) const;
    bool contains(const Vec3& p) const;
    bool containsIncludingLowerBound(const Vec3& p) const;  // 4J Added
    double getSize() const;
    AABB shrink(double xa, double ya, double za) const;
    HitResult* clip(const Vec3& a, const Vec3& b) const;
    bool containsX(const std::optional<Vec3>& v) const;
    bool containsY(const std::optional<Vec3>& v) const;
    bool containsZ(const std::optional<Vec3>& v) const;
    std::string toString() const;
};
