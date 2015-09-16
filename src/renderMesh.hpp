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

    BufferedVertice(float x, float y, float z, unsigned c, float u, float v) :
    x(x), y(y), z(z), c(c), u(u), v(v) { }

    float x, y, z;
    unsigned c;
    float u, v;
};

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat, std::vector<BufferedVertice>& buff, bool cull);

#endif	/* RENDER_MESH_HPP */

