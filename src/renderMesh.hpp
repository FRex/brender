#ifndef RENDER_MESH_HPP
#define	RENDER_MESH_HPP

#include <armadillo>
#include <vector>
#include "culling.hpp"

class Rasterizer;
class Mesh;

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat, std::vector<BufferedVertice>& buff, bool cull);

#endif	/* RENDER_MESH_HPP */

