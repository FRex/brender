#include "Rasterizer.hpp"
#include <cmath>
#include <algorithm>
#include <float.h>
#include <cstdio>
#include <cassert>
#include "stb_image.h"

using std::modf;

Rasterizer::Rasterizer(void* pixels) :
m_pixels(static_cast<unsigned*>(pixels))
{
    int n;
    m_texture = stbi_load("texture.png", &m_texx, &m_texy, &n, 3);
    m_mode = ERM_TEXTURES;
}

Rasterizer::~Rasterizer()
{
    stbi_image_free(m_texture);
}

static inline float crossProduct(float ax, float ay, float bx, float by)
{
    return ax * by - ay * bx;
}

static inline int min(int a, int b, int c)
{
    return std::min(a, std::min(b, c));
}

static inline int max(int a, int b, int c)
{
    return std::max(a, std::max(b, c));
}

static inline void sepcolor(unsigned c, int& r, int& g, int& b)
{
    r = (c >> 16) & 0xff;
    g = (c >> 8) & 0xff;
    b = (c >> 0) & 0xff;
}

static inline unsigned mix3colors(unsigned c1, float v1, unsigned c2, float v2, unsigned c3, float v3)
{
    int r1, g1, b1;
    int r2, g2, b2;
    int r3, g3, b3;
    sepcolor(c1, r1, g1, b1);
    sepcolor(c2, r2, g2, b2);
    sepcolor(c3, r3, g3, b3);
    int rf = r1 * v1 + r2 * v2 + r3 * v3;
    int gf = g1 * v1 + g2 * v2 + g3 * v3;
    int bf = b1 * v1 + b2 * v2 + b3 * v3;
    return (rf << 16) + (gf << 8) + bf;
}

static inline unsigned mul2colors(unsigned c1, unsigned c2)
{
    int r1, g1, b1;
    int r2, g2, b2;
    sepcolor(c1, r1, g1, b1);
    sepcolor(c2, r2, g2, b2);
    int rf = (r1 * r2) / 255;
    int gf = (g1 * g2) / 255;
    int bf = (b1 * b2) / 255;
    assert(0 <= rf && rf <= 255);
    assert(0 <= gf && gf <= 255);
    assert(0 <= bf && bf <= 255);
    return (rf << 16) + (gf << 8) + bf;
}

static void adjustToView(int& x1, int& y1, int& x2, int& y2)
{
    x1 = std::max(0, std::min(x1, 640 - 1));
    y1 = std::max(0, std::min(y1, 480 - 1));
    x2 = std::max(0, std::min(x2, 640 - 1));
    y2 = std::max(0, std::min(y2, 480 - 1));
}

static unsigned rgbf(float r, float g, float b)
{
    const int rf = std::max(0, std::min<int>(255, r * 255.f));
    const int gf = std::max(0, std::min<int>(255, g * 255.f));
    const int bf = std::max(0, std::min<int>(255, b * 255.f));
    return (rf << 16) + (gf << 8) + bf;
}

static float normalizeTCoords(float x)
{
    float u;
    x = modf(x, &u);
    return std::fabs(x);
}

void Rasterizer::rasterize()
{
    for(int i = 0; i < 640 * 480; ++i)
        m_depthbuffer[i] = FLT_MAX;

    for(int i = 0; i < 640 * 480; ++i)
        m_pixels[i] = 0x202020; //very dark gray

    for(int i = 0; i < (int)m_vertices.size(); i += 3)
    {
        int x1 = m_vertices[i].x;
        int y1 = m_vertices[i].y;
        unsigned c1 = m_vertices[i].color;
        float d1 = m_vertices[i].depth;
        float u1 = m_vertices[i].u;
        float v1 = m_vertices[i].v;

        int x2 = m_vertices[i + 1].x;
        int y2 = m_vertices[i + 1].y;
        unsigned c2 = m_vertices[i + 1].color;
        float d2 = m_vertices[i + 1].depth;
        float u2 = m_vertices[i + 1].u;
        float v2 = m_vertices[i + 1].v;

        int x3 = m_vertices[i + 2].x;
        int y3 = m_vertices[i + 2].y;
        unsigned c3 = m_vertices[i + 2].color;
        float d3 = m_vertices[i + 2].depth;
        float u3 = m_vertices[i + 2].u;
        float v3 = m_vertices[i + 2].v;

        //don't render triangles that might have been incorrectly
        //casted due to being behind the camera
        if(d1 < 0.f || d2 < 0.f || d3 < 0.f)
            continue;

        int maxX = max(x1, x2, x3);
        int minX = min(x1, x2, x3);
        int maxY = max(y1, y2, y3);
        int minY = min(y1, y2, y3);


        //clip
        adjustToView(minX, minY, maxX, maxY);

        for(int x = minX; x <= maxX; ++x)
        {
            for(int y = minY; y <= maxY; ++y)
            {
                const float w2 = crossProduct(x - x1, y - y1, x3 - x1, y3 - y1) / crossProduct(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
                const float w3 = crossProduct(x2 - x1, y2 - y1, x - x1, y - y1) / crossProduct(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
                if((w2 >= 0) && (w3 >= 0) && (w2 + w3 <= 1))
                {
                    const float w1 = 1.f - w2 - w3;
                    const unsigned color = mix3colors(c1, w1, c2, w2, c3, w3);
                    const float depth = d1 * w1 + d2 * w2 + d3 * w3;
                    const float u = normalizeTCoords(u1 * w1 + u2 * w2 + u3 * w3);
                    const float v = normalizeTCoords(v1 * w1 + v2 * w2 + v3 * w3);
                    const unsigned texel = getTexel(u, v);
                    switch(m_mode)
                    {
                        case ERM_COLORS:
                            setPixel(x, y, color, depth);
                            break;
                        case ERM_TEXTURES:
                            setPixel(x, y, texel, depth);
                            break;
                        case ERM_COLORS_TEXTURES:
                            setPixel(x, y, mul2colors(color, texel), depth);
                            break;
                        case ERM_UV_RED_BLUE:
                            setPixel(x, y, rgbf(u, 0.f, v), depth);
                            break;
                    }//switch m_mode
                }//inside
            }//for y
        }//for x
    }//for each triangle
}

void Rasterizer::clear()
{
    m_vertices.clear();
}

void Rasterizer::addVertex(int x, int y, unsigned color, float depth, float u, float v)
{
    Vertex2 vert;
    vert.x = x;
    vert.y = y;
    vert.color = color;
    vert.depth = depth;
    vert.u = u;
    vert.v = v;
    m_vertices.push_back(vert);
}

void Rasterizer::setPixel(int x, int y, unsigned color, float depth)
{
    if(depth < 0.f)
        return;

    assert(0 <= x && x < 640 && 0 <= y && y < 480);
    if(m_depthbuffer[x + y * 640] > depth)
    {
        m_pixels[x + y * 640] = color;
        m_depthbuffer[x + y * 640] = depth;
    }
}

unsigned Rasterizer::getTexel(float u, float v) const
{
    const int x = std::max(0, std::min<int>(round(u * m_texx), m_texx - 1));
    const int y = std::max(0, std::min<int>(round(v * m_texy), m_texy - 1));
    const unsigned char * tex = &m_texture[3 * (x + m_texx * y)];
    return (tex[0] << 16) + (tex[1] << 8) + tex[2];
}

void Rasterizer::toggleRenderMode()
{
    m_mode = (m_mode + 1) % ERENDER_MODE_COUNT;
}
