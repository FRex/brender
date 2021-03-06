#ifndef RASTERIZER_HPP
#define	RASTERIZER_HPP

#include <vector>

enum ERENDER_MODE
{
    ERM_TEXTURES = 0,
    ERM_COLORS,
    ERM_COLORS_TEXTURES,
    ERM_UV_RED_BLUE,
    ERM_NORMALS,
    ERM_LIGHT,
    ERM_LIGHT_COLOR,
    ERM_LIGHT_COLOR_TEXTURE,
    
    ERENDER_MODE_COUNT //keep last
};

class Rasterizer
{
public:
    Rasterizer(void * pixels);
    ~Rasterizer();
    void rasterize();
    void clear();
    void addVertex(int x, int y, unsigned color, float depth, float u, float v, float nx, float ny, float nz);
    void toggleRenderMode();
    int getRenderMode();
    void toggleSkipDepth();
    bool getSkipDetph() const;
    void loadTexture(const char * tex);

private:
    bool canSetPixel(int x, int y, float depth);
    void setPixel(int x, int y, unsigned color, float depth);
    unsigned getTexel(float u, float v) const;

    class Vertex2
    {
    public:
        float x, y;
        float r, g, b;
        float depth;
        float u;
        float v;
        float nx, ny, nz;
    };

    std::vector<Vertex2> m_vertices;
    unsigned * m_pixels;
    float m_depthbuffer[640 * 480];
    unsigned char * m_texture;
    int m_texx;
    int m_texy;
    int m_mode;
    bool m_skipdepth;
};

#endif	/* RASTERIZER_HPP */

