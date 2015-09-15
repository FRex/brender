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

static void adjustToView(float& x1, float& y1, float& x2, float& y2)
{
    x1 = std::max(0.f, std::min(x1, 639.f));
    y1 = std::max(0.f, std::min(y1, 479.f));
    x2 = std::max(0.f, std::min(x2, 639.f));
    y2 = std::max(0.f, std::min(y2, 479.f));
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

static void decomposeColorF(unsigned c, float& r, float& g, float& b)
{
    b = (c & 0xff) / 255.f;
    c >>= 8;
    g = (c & 0xff) / 255.f;
    c >>= 8;
    r = (c & 0xff) / 255.f;
}

void Rasterizer::rasterize()
{
    for(int i = 0; i < 640 * 480; ++i)
        m_depthbuffer[i] = FLT_MAX;

    for(int i = 0; i < 640 * 480; ++i)
        m_pixels[i] = 0x202020; //very dark gray

    float x1, y1, r1, g1, b1, d1, d1i, u1, v1;
    float x2, y2, r2, g2, b2, d2, d2i, u2, v2;
    float x3, y3, r3, g3, b3, d3, d3i, u3, v3;
    float maxX, maxY, minX, minY;
    float x, y;
    float w1, w2, w3;
    float r, g, b, depth, depthi, u, v;
    for(int i = 0; i < (int)m_vertices.size(); i += 3)
    {
        x1 = m_vertices[i].x;
        y1 = m_vertices[i].y;
        r1 = m_vertices[i].r;
        g1 = m_vertices[i].g;
        b1 = m_vertices[i].b;
        d1 = m_vertices[i].depth;
        u1 = m_vertices[i].u;
        v1 = m_vertices[i].v;
        d1i = 1.f / m_vertices[i].depth;

        x2 = m_vertices[i + 1].x;
        y2 = m_vertices[i + 1].y;
        r2 = m_vertices[i + 1].r;
        g2 = m_vertices[i + 1].g;
        b2 = m_vertices[i + 1].b;
        d2 = m_vertices[i + 1].depth;
        u2 = m_vertices[i + 1].u;
        v2 = m_vertices[i + 1].v;
        d2i = 1.f / m_vertices[i + 1].depth;

        x3 = m_vertices[i + 2].x;
        y3 = m_vertices[i + 2].y;
        r3 = m_vertices[i + 2].r;
        g3 = m_vertices[i + 2].g;
        b3 = m_vertices[i + 2].b;
        d3 = m_vertices[i + 2].depth;
        u3 = m_vertices[i + 2].u;
        v3 = m_vertices[i + 2].v;
        d3i = 1.f / m_vertices[i + 2].depth;

        //don't render triangles that might have been incorrectly
        //casted due to being behind the camera
        if(d1 < 0.f || d2 < 0.f || d3 < 0.f)
            continue;

        maxX = max(x1, x2, x3);
        minX = min(x1, x2, x3);
        maxY = max(y1, y2, y3);
        minY = min(y1, y2, y3);


        //clip
        adjustToView(minX, minY, maxX, maxY);

        for(x = minX; x <= maxX; x += 1.f)
        {
            for(y = minY; y <= maxY; y += 1.f)
            {
                //                const float w2 = crossProduct(x - x1, y - y1, x3 - x1, y3 - y1) / crossProduct(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
                //                const float w3 = crossProduct(x2 - x1, y2 - y1, x - x1, y - y1) / crossProduct(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
                //unrolled:
                w2 = ((x - x1)*(y3 - y1)-(y - y1)*(x3 - x1)) / ((x2 - x1)*(y3 - y1)-(y2 - y1)*(x3 - x1));
                w3 = ((x2 - x1)*(y - y1)-(y2 - y1)*(x - x1)) / ((x2 - x1)*(y3 - y1)-(y2 - y1)*(x3 - x1));
                if((w2 >= 0.f) && (w3 >= 0.f) && (w2 + w3 <= 1.f))
                {
                    w1 = 1.f - w2 - w3;
                    depth = d1 * w1 + d2 * w2 + d3 * w3;
                    depthi = d1i * w1 + d2i * w2 + d3i * w3;
                    if(canSetPixel(x, y, depth))
                    {
                        r = (r1 * w1 + r2 * w2 + r3 * w3) / depthi;
                        g = (g1 * w1 + g2 * w2 + g3 * w3) / depthi;
                        b = (b1 * w1 + b2 * w2 + b3 * w3) / depthi;
                        u = normalizeTCoords(u1 * w1 + u2 * w2 + u3 * w3) / depthi;
                        v = normalizeTCoords(v1 * w1 + v2 * w2 + v3 * w3) / depthi;
                        switch(m_mode)
                        {
                            case ERM_COLORS:
                                setPixel(x, y, rgbf(r, g, b), depth);
                                break;
                            case ERM_TEXTURES:
                                setPixel(x, y, getTexel(u, v), depth);
                                break;
                            case ERM_COLORS_TEXTURES:
                                setPixel(x, y, mul2colors(rgbf(r, g, b), getTexel(u, v)), depth);
                                break;
                            case ERM_UV_RED_BLUE:
                                setPixel(x, y, rgbf(u, 0.f, v), depth);
                                break;
                        }//switch m_mode
                    }//can set pixel
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
    decomposeColorF(color, vert.r, vert.g, vert.b);
    vert.r /= depth;
    vert.g /= depth;
    vert.b /= depth;
    vert.depth = depth;
    vert.u = u / depth;
    vert.v = v / depth;
    m_vertices.push_back(vert);
}

bool Rasterizer::canSetPixel(int x, int y, float depth)
{
    if(depth < 0.f)
        return false;

    assert(0 <= x && x < 640 && 0 <= y && y < 480);
    if(m_depthbuffer[x + y * 640] > depth)
        return true;

    return false;
}

void Rasterizer::setPixel(int x, int y, unsigned color, float depth)
{
    assert(0 <= x && x < 640 && 0 <= y && y < 480);
    m_pixels[x + y * 640] = color;
    m_depthbuffer[x + y * 640] = depth;
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

int Rasterizer::getRenderMode()
{
    return m_mode;
}
