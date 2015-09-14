#include "renderMesh.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Rasterizer.hpp"

const float kProjPlaneDist = 320.f;
const float halfscreenwidth = 640.f / 2.f;
const float halfscreenheight = 480.f / 2.f;

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat, std::vector<BufferedVertice>& buff)
{
    const unsigned * idxs = mesh.getIndices();
    unsigned idxc = mesh.getIndexCount();
    arma::vec4 av;

    for(int i = 0; i < idxc; ++i)
    {
        const Vertex& a = mesh.getVertex(idxs[i]);

        av(0) = a.position.x;
        av(1) = a.position.y;
        av(2) = a.position.z;
        av(3) = 1.f;
        av = mat * av;

        av(0) = av(0) * (kProjPlaneDist / std::fabs(av(2))) + halfscreenwidth;
        av(1) = av(1) * (kProjPlaneDist / std::fabs(av(2))) + halfscreenheight;

        raster.addVertex(av(0), av(1), a.color, av(2), a.u, a.v);
    }//for

}