#ifndef RENDER_MESH_HPP
#define	RENDER_MESH_HPP

#include <armadillo>
#include <vector>
#include "culling.hpp"

class Rasterizer;
class Mesh;

enum ECULLING_MODE
{
    ECM_BACKFACE = 0,
    ECM_FRONTFACE,
    ECM_NONE,

    ECULLING_MODE_COUNT
};

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat, std::vector<BufferedVertice>& buff, ECULLING_MODE cull);

#endif	/* RENDER_MESH_HPP */

