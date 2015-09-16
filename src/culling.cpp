#include "culling.hpp"

Vector3 cross(float ax, float ay, float az, float bx, float by, float bz)
{
    return Vector3(ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx);
}

float dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool cw(const BufferedVertice& v0, const BufferedVertice& v1, const BufferedVertice& v2)
{
    const Vector3 n = cross(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z, v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
    return dot(Vector3(v0.x, v0.y, v0.z), n) >= 0.f;
}
