#include "renderMesh.hpp"
#include "Vertex.hpp"
#include "Mesh.hpp"
#include "Rasterizer.hpp"

const float kProjPlaneDist = 320.f;
const float halfscreenwidth = 640.f / 2.f;
const float halfscreenheight = 480.f / 2.f;

static inline Vector3 cross(Vector3 a, Vector3 p)
{
    return Vector3(a.y * p.z - a.z * p.y, a.z * p.x - a.x * p.z, a.x * p.y - a.y * p.x);
}

static inline float dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline bool cw(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
    const Vector3 n = cross(v1.position - v0.position, v2.position - v0.position);
    return (-dot(v0.position, n)) >= 0.f;
}

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

        buff.push_back(BufferedVertice(av(0), av(1), a.color, av(2), a.u, a.v));

    }//for

    for(int i = 0; (i + 2) < idxc; i += 3)
    {
        {
            const Vertex& a = mesh.getVertex(idxs[i]);
            const Vertex& b = mesh.getVertex(idxs[i + 1]);
            const Vertex& c = mesh.getVertex(idxs[i + 2]);
            if(cull && cw(a, b, c))
                continue;
        }
        {
            const BufferedVertice& a = buff[idxs[i]];
            const BufferedVertice& b = buff[idxs[i + 1]];
            const BufferedVertice& c = buff[idxs[i + 2]];
            raster.addVertex(a.x, a.y, a.c, a.d, a.u, a.v);
            raster.addVertex(b.x, b.y, b.c, b.d, b.u, b.v);
            raster.addVertex(c.x, c.y, c.c, c.d, c.u, c.v);
        }
    }//for
}