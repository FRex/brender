/* 
 * File:   Vertex.hpp
 * Author: frex
 *
 * Created on May 28, 2015, 11:43 PM
 */

#ifndef VERTEX_HPP
#define	VERTEX_HPP

class Vector3
{
public:

    Vector3() : x(0.f), y(1.f), z(0.f) { }

    Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
    float x, y, z;
};

class Vertex
{
public:

    Vertex(Vector3 position, unsigned color) : position(position), color(color), u(0.f), v(0.f) { }

    Vertex(Vector3 position, unsigned color, float u, float v) : position(position), color(color), u(u), v(v) { }

    Vertex(Vector3 position, Vector3 normal, unsigned color, float u = 0.f, float v = 0.f) : position(position), normal(normal), color(color), u(u), v(v) { }

    Vector3 position;
    Vector3 normal;
    unsigned color;
    float u;
    float v;
};

#endif	/* VERTEX_HPP */

