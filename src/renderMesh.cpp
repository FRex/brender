#include "renderMesh.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Rasterizer.hpp"

const float kProjPlaneDist = 320.f;

void draw(Rasterizer& raster, const Mesh& mesh, const arma::mat44& mat)
{
    const unsigned * idxs = mesh.getIndices();
    unsigned idxc = mesh.getIndexCount();

    for(int i = 2; i < idxc; i += 3)
    {
        const Vertex& a = mesh.getVertex(idxs[i - 2]);
        const Vertex& b = mesh.getVertex(idxs[i - 1]);
        const Vertex& c = mesh.getVertex(idxs[i - 0]);

        arma::vec4 av;
        av(0) = a.position.x;
        av(1) = a.position.y;
        av(2) = a.position.z;
        av(3) = 1.f;

        arma::vec4 bv;
        bv(0) = b.position.x;
        bv(1) = b.position.y;
        bv(2) = b.position.z;
        bv(3) = 1.f;

        arma::vec4 cv;
        cv(0) = c.position.x;
        cv(1) = c.position.y;
        cv(2) = c.position.z;
        cv(3) = 1.f;

        av = mat * av;
        bv = mat * bv;
        cv = mat * cv;

        const float screenwidth = 640.f;
        const float screenheight = 480.f;

        av(0) = av(0) * (kProjPlaneDist / std::fabs(av(2))) + screenwidth / 2.f;
        av(1) = av(1) * (kProjPlaneDist / std::fabs(av(2))) + screenheight / 2.f;

        bv(0) = bv(0) * (kProjPlaneDist / std::fabs(bv(2))) + screenwidth / 2.f;
        bv(1) = bv(1) * (kProjPlaneDist / std::fabs(bv(2))) + screenheight / 2.f;

        cv(0) = cv(0) * (kProjPlaneDist / std::fabs(cv(2))) + screenwidth / 2.f;
        cv(1) = cv(1) * (kProjPlaneDist / std::fabs(cv(2))) + screenheight / 2.f;

        raster.addVertex(av(0), av(1), a.color, av(2), a.u, a.v);
        raster.addVertex(bv(0), bv(1), b.color, bv(2), b.u, b.v);
        raster.addVertex(cv(0), cv(1), c.color, cv(2), c.u, c.v);
    }//for

}