#include "Rasterizer.hpp"
#include <algorithm>
#include <float.h>
#include <cstdio>
#include <cassert>

Rasterizer::Rasterizer(void* pixels) :
m_pixels(static_cast<unsigned*>(pixels)) {
    //    addVertex(0, 0, 0xff0000, 2.f);
    //    addVertex(100, 0, 0xff00, 2.f);
    //    addVertex(50, 100, 0xff, 2.f);
    //
    //    addVertex(50, 0, 0xff0000, 0.f);
    //    addVertex(150, 0, 0xff00, 10.f);
    //    addVertex(100, 100, 0xff, 10.f);
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

void adjustToView(int& x1, int& y1, int& x2, int& y2)
{
    x1 = std::max(0, std::min(x1, 640 - 1));
    y1 = std::max(0, std::min(y1, 480 - 1));
    x2 = std::max(0, std::min(x2, 640 - 1));
    y2 = std::max(0, std::min(y2, 480 - 1));
}

void Rasterizer::rasterize()
{
    addVertex(50, 0, 0xff0000, 100000.f);
    addVertex(150, 0, 0xff00, 100000.f);
    addVertex(100, 100, 0xff, 100000.f);


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

        int x2 = m_vertices[i + 1].x;
        int y2 = m_vertices[i + 1].y;
        unsigned c2 = m_vertices[i + 1].color;
        float d2 = m_vertices[i + 1].depth;

        int x3 = m_vertices[i + 2].x;
        int y3 = m_vertices[i + 2].y;
        unsigned c3 = m_vertices[i + 2].color;
        float d3 = m_vertices[i + 2].depth;

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
                    const unsigned c = mix3colors(c1, w1, c2, w2, c3, w3);
                    const float depth = d1 * w1 + d2 * w2 + d3 * w3;
                    setPixel(x, y, c, depth);
                }//inside
            }//for y
        }//for x
    }//for each triangle

    //    float mind = m_depthbuffer[0];
    //    float maxd = -1000.f;
    //    for(int i = 0; i < 640 * 480; ++i)
    //    {
    //        mind = std::min(mind, m_depthbuffer[i]);
    //        if(m_depthbuffer[i] < FLT_MAX)
    //            maxd = std::max(maxd, m_depthbuffer[i]);
    //    }
    //    std::printf("min, max = %f, %f\n", mind, maxd);
}

void Rasterizer::clear()
{
    m_vertices.clear();
}

void Rasterizer::addVertex(int x, int y, unsigned color, float depth)
{
    Vertex2 v;
    v.x = x;
    v.y = y;
    v.color = color;
    v.depth = depth;
    m_vertices.push_back(v);
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
