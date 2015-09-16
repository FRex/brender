/* 
 * File:   culling.hpp
 * Author: frex
 *
 * Created on September 16, 2015, 3:53 AM
 */

#ifndef CULLING_HPP
#define	CULLING_HPP

#include "Vertex.hpp"

class BufferedVertice
{
public:

    BufferedVertice() { }

    BufferedVertice(float x, float y, float z, unsigned c, float u, float v) :
    x(x), y(y), z(z), c(c), u(u), v(v) { }

    float x, y, z;
    unsigned c;
    float u, v;
};

Vector3 cross(float ax, float ay, float az, float bx, float by, float bz);
float dot(Vector3 a, Vector3 b);
bool cw(const BufferedVertice& v0, const BufferedVertice& v1, const BufferedVertice& v2);

#endif	/* CULLING_HPP */

