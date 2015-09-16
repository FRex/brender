#include "renderMesh.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Rasterizer.hpp"
#include "culling.hpp"

const float kProjPlaneDist = 320.f;
const float halfscreenwidth = 640.f / 2.f;
const float halfscreenheight = 480.f / 2.f;

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat, std::vector<BufferedVertice>& buff, bool cull)
{
    const unsigned * idxs = mesh.getIndices();
    const unsigned idxc = mesh.getIndexCount();
    const unsigned vc = mesh.getVertexCount();
    arma::vec4 av;

    buff.clear();

    for(int i = 0; i < vc; ++i)
    {
        const Vertex& a = mesh.getVertex(i);

        av(0) = a.position.x;
        av(1) = a.position.y;
        av(2) = a.position.z;
        av(3) = 1.f;
        av = mat * av;

        av(0) = av(0) * (kProjPlaneDist / std::fabs(av(2))) + halfscreenwidth;
        av(1) = av(1) * (kProjPlaneDist / std::fabs(av(2))) + halfscreenheight;

        buff.push_back(BufferedVertice(av(0), av(1), av(2), a.color, a.u, a.v));

    }//for

    int culled = 0;
    for(int i = 0; (i + 2) < idxc; i += 3)
    {
        const BufferedVertice& a = buff[idxs[i]];
        const BufferedVertice& b = buff[idxs[i + 1]];
        const BufferedVertice& c = buff[idxs[i + 2]];
        const float cos = cw(a, b, c);
        unsigned color = (cos * 255.f);
        if(cull && cos > 0.f)
        {
            ++culled;

            raster.addVertex(a.x, a.y, color, a.z, a.u, a.v);
            raster.addVertex(b.x, b.y, color, b.z, b.u, b.v);
            raster.addVertex(c.x, c.y, color, c.z, c.u, c.v);
            continue;
        }

        color <<= 8;
        raster.addVertex(a.x, a.y, color, a.z, a.u, a.v);
        raster.addVertex(b.x, b.y, color, b.z, b.u, b.v);
        raster.addVertex(c.x, c.y, color, c.z, c.u, c.v);

        //        raster.addVertex(a.x, a.y, a.c, a.z, a.u, a.v);
        //        raster.addVertex(b.x, b.y, b.c, b.z, b.u, b.v);
        //        raster.addVertex(c.x, c.y, c.c, c.z, c.u, c.v);
    }//for
    std::printf("CULLED: %d\n", culled);
}