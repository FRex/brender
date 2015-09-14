#ifndef RENDER_MESH_HPP
#define	RENDER_MESH_HPP

#include <armadillo>
#include <vector>

class Rasterizer;
class Mesh;

class BufferedVertice
{
public:

    BufferedVertice() { }

    BufferedVertice(int x, int y, unsigned c, float d, float u, float v) :
    x(x), y(y), c(c), d(d), u(u), v(v) { }

    int x, y;
    unsigned c;
    float d, u, v;
};

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat, std::vector<BufferedVertice>& buff);

#endif	/* RENDER_MESH_HPP */

