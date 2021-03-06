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

    BufferedVertice(float x, float y, float z, unsigned c, float u, float v, float nx, float ny, float nz) :
    x(x), y(y), z(z), c(c), u(u), v(v), nx(nx), ny(ny), nz(nz) { }

    float x, y, z;
    float nx, ny, nz;
    unsigned c;
    float u, v;
};

Vector3 cross(float ax, float ay, float az, float bx, float by, float bz);
bool cw(const BufferedVertice& v0, const BufferedVertice& v1, const BufferedVertice& v2);

#endif	/* CULLING_HPP */

