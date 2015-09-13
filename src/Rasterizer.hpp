#ifndef RASTERIZER_HPP
#define	RASTERIZER_HPP

#include <vector>

enum ERENDER_MODE
{
    ERM_TEXTURES = 0,
    ERM_COLORS,
    ERM_COLORS_TEXTURES,
    ERM_UV_RED_BLUE,
    
    ERENDER_MODE_COUNT //keep last
};

class Rasterizer
{
public:
    Rasterizer(void * pixels);
    ~Rasterizer();
    void rasterize();
    void clear();
    void addVertex(int x, int y, unsigned color, float depth, float u, float v);
    void toggleRenderMode();

private:
    void setPixel(int x, int y, unsigned color, float depth);
    unsigned getTexel(float u, float v) const;

    class Vertex2
    {
    public:
        int x, y;
        unsigned color;
        float depth;
        float u;
        float v;
    };

    std::vector<Vertex2> m_vertices;
    unsigned * m_pixels;
    float m_depthbuffer[640 * 480];
    unsigned char * m_texture;
    int m_texx;
    int m_texy;
    int m_mode;
};

#endif	/* RASTERIZER_HPP */

