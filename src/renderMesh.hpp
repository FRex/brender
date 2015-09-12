#ifndef RENDER_MESH_HPP
#define	RENDER_MESH_HPP

#include <armadillo>

class Rasterizer;
class Mesh;

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat);

#endif	/* RENDER_MESH_HPP */

