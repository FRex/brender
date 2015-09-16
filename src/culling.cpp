#include "culling.hpp"
#include <cmath>

Vector3 cross(float ax, float ay, float az, float bx, float by, float bz)
{
    return Vector3(ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx);
}

float dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float len(const BufferedVertice& v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float len(Vector3 v)
{
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float cw(const BufferedVertice& v0, const BufferedVertice& v1, const BufferedVertice& v2)
{
    const float x10 = v1.x - v0.x;
    const float y10 = v1.y - v0.y;
    const float z10 = v1.z - v0.z;
    const float x20 = v2.x - v0.x;
    const float y20 = v2.y - v0.y;
    const float z20 = v2.z - v0.z;
    const Vector3 n = cross(x10, y10, z10, x20, y20, z20);
    return dot(Vector3(v0.x, v0.y, v0.z), n) / (len(v0) * len(n));
}
