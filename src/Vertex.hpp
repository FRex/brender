/* 
 * File:   Vertex.hpp
 * Author: frex
 *
 * Created on May 28, 2015, 11:43 PM
 */

#ifndef VERTEX_HPP
#define	VERTEX_HPP

#include <SFML/System/Vector3.hpp>
#include <SFML/System/Vector2.hpp>

typedef sf::Vector3f Vector3;
typedef sf::Vector2f Vector2;

class Vertex
{
public:

    Vertex(Vector3 position, unsigned color) : position(position), color(color), u(0.f), v(0.f) { }

    Vertex(Vector3 position, unsigned color, float u, float v) : position(position), color(color), u(u), v(v) { }

    Vector3 position;
    unsigned color;
    float u;
    float v;
};

#endif	/* VERTEX_HPP */

