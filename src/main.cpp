#include <SDL.h>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "Rasterizer.hpp"
#include "Mesh.hpp"

#include "renderMesh.hpp"
#include "makeMesh.hpp"

arma::mat44 scale(float x, float y, float z)
{
    arma::mat44 ret = arma::zeros(4u, 4u);
    ret(0, 0) = x;
    ret(1, 1) = y;
    ret(2, 2) = z;
    ret(3, 3) = 1.f;
    return ret;
}

arma::mat44 translate(float x, float y, float z)
{
    arma::mat44 ret = arma::zeros(4u, 4u);
    ret(0, 0) = 1.f;
    ret(1, 1) = 1.f;
    ret(2, 2) = 1.f;
    ret(3, 3) = 1.f;
    ret(0, 3) = x;
    ret(1, 3) = y;
    ret(2, 3) = z;
    return ret;
}

arma::mat44 rotateY(float y)
{
    arma::mat44 ret = arma::zeros(4u, 4u);
    ret(0, 0) = 1.f;
    ret(1, 1) = 1.f;
    ret(2, 2) = 1.f;
    ret(3, 3) = 1.f;
    ret(0, 0) = std::cos(y);
    ret(0, 2) = -std::sin(y);
    ret(2, 0) = std::sin(y);
    ret(2, 2) = std::cos(y);
    return ret;
}

arma::mat44 rotateX(float y)
{
    arma::mat44 ret = arma::zeros(4u, 4u);
    ret(0, 0) = 1.f;
    ret(1, 1) = 1.f;
    ret(2, 2) = 1.f;
    ret(3, 3) = 1.f;
    ret(1, 1) = std::cos(y);
    ret(1, 2) = -std::sin(y);
    ret(2, 1) = std::sin(y);
    ret(2, 2) = std::cos(y);
    return ret;
}

arma::mat44 rotateZ(float y)
{
    arma::mat44 ret = arma::zeros(4u, 4u);
    ret(0, 0) = 1.f;
    ret(1, 1) = 1.f;
    ret(2, 2) = 1.f;
    ret(3, 3) = 1.f;
    ret(0, 0) = std::cos(y);
    ret(0, 1) = -std::sin(y);
    ret(1, 0) = std::sin(y);
    ret(1, 1) = std::cos(y);
    return ret;
}

const float pi = 3.1415f;

unsigned rndrgb()
{
    const int r = std::rand() & 0xff;
    const int g = std::rand() & 0xff;
    const int b = std::rand() & 0xff;
    return (r << 16)+(g << 8) + b;
}

static unsigned rgbf(float r, float g, float b)
{
    r = std::fabs(r);
    g = std::fabs(g);
    b = std::fabs(b);
    const int rf = std::max(0, std::min<int>(255, r * 255.f));
    const int gf = std::max(0, std::min<int>(255, g * 255.f));
    const int bf = std::max(0, std::min<int>(255, b * 255.f));
    return (rf << 16) + (gf << 8) + bf;
}

void loadMesh(Mesh& mesh, const char * filename, int usernd, unsigned color)
{
    std::printf("Loading file: %s\n", filename);
    std::ifstream file(filename);
    mesh.clear();
    float scale;
    unsigned icount, vcount;
    unsigned idx;
    float x, y, z;
    float u, v;
    file >> scale >> icount >> vcount;
    Vector3 normal;

    for(int i = 0; i < icount; ++i)
    {
        file >> idx;
        mesh.addIndex(idx);
    }

    for(int i = 0; i < vcount; ++i)
    {
        file >> x >> y >> z;
        file >> normal.x >> normal.y >> normal.z;
        file >> u >> v;
        if(usernd)
        {
            mesh.addVertex(Vertex(Vector3(scale * x, scale * y, scale * z), rndrgb(), u, v));
        }
        else if(color == 0x0)
        {
            mesh.addVertex(Vertex(Vector3(scale * x, scale * y, scale * z), normal, rgbf(x, y, z), u, v));
        }
        else
        {
            mesh.addVertex(Vertex(Vector3(scale * x, scale * y, scale * z), normal, color, u, v));
        }
    }
    std::printf("vertices, indices, scale = (%u, %u, %f)\n", vcount, icount, scale);
}

const char * modenames[] = {
    "ERM_TEXTURES",
    "ERM_COLORS",
    "ERM_COLORS_TEXTURES",
    "ERM_UV_RED_BLUE",
    "ERM_NORMALS",
    "ERM_LIGHT",
    "ERM_LIGHT_COLOR",
    "ERM_LIGHT_COLOR_TEXTURE",
};
const char * cullnames[] = {
    "ECM_BACKFACE",
    "ECM_FRONTFACE",
    "ECM_NONE",
};

const char * tostr(bool b)
{
    return b?"true":"false";
}

int main(int argc, char ** argv)
{
    std::srand(std::time(0x0));

    Mesh mesh;
    const char * texname = 0x0;
    makeCubeMesh(mesh);

    if(argc != 1 && argc != 5)
    {
        std::fprintf(stderr, "Usage: %s mesh rnd/not color texture\n", argv[0]);
        return 1;
    }
    if(argc == 5)
    {
        loadMesh(mesh, argv[1], 0 == std::strcmp(argv[2], "rnd"), std::strtol(argv[3], 0x0, 16));
        texname = argv[4];
    }


    SDL_Window * win = NULL;
    SDL_Surface * sur = NULL;
    int cull = 0;
    int run = 1;
    SDL_Event eve;
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("BRender", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    sur = SDL_GetWindowSurface(win);

    std::vector<BufferedVertice> buffer;
    Rasterizer bras(sur->pixels);
    if(texname)
        bras.loadTexture(texname);

    printf("%s\n", SDL_GetPixelFormatName(sur->format->format));

    float tx = 0.f;
    float ty = 0.f;
    float tz = 500.f;
    float rotx = 0.f;
    float roty = 0.f;
    float rotz = 0.f;
    float sx = 1.f;
    float sy = 1.f;
    float sz = 1.f;

    while(run)
    {
        while(SDL_PollEvent(&eve))
        {
            if(eve.type == SDL_QUIT)
            {
                run = 0;
                break;
            }
            if(eve.type == SDL_KEYDOWN)
            {
                switch(eve.key.keysym.sym)
                {
                    case 'w':
                        rotx += pi / 12.f;
                        break;
                    case 's':
                        rotx -= pi / 12.f;
                        break;
                    case 'a':
                        rotz += pi / 12.f;
                        break;
                    case 'd':
                        rotz -= pi / 12.f;
                        break;
                    case 'q':
                        roty += pi / 12.f;
                        break;
                    case 'e':
                        roty -= pi / 12.f;
                        break;
                    case 'i':
                        tz += 10.f;
                        break;
                    case 'k':
                        tz -= 10.f;
                        break;
                    case 'j':
                        tx -= 10.f;
                        break;
                    case 'l':
                        tx += 10.f;
                        break;
                    case 'u':
                        ty += 10.f;
                        break;
                    case 'o':
                        ty -= 10.f;
                        break;
                    case ' ':
                        bras.toggleRenderMode();
                        break;
                    case 'x':
                        bras.toggleSkipDepth();
                        break;
                    case 'c':
                        cull = (cull + 1) % ECULLING_MODE_COUNT;
                        break;
                    case 't':
                        sx += 0.1f;
                        break;
                    case 'g':
                        sx -= 0.1f;
                        break;
                    case 'f':
                        sz += 0.1f;
                        break;
                    case 'h':
                        sz -= 0.1f;
                        break;
                    case 'r':
                        sy += 0.1f;
                        break;
                    case 'y':
                        sy -= 0.1f;
                        break;
                }
            }
        }//while sdl poll event eve


        std::printf("R: %f, %f, %f\n", rotx, roty, rotz);
        std::printf("T: %f, %f, %f\n", tx, ty, tz);
        std::printf("S: %f, %f, %f\n", sx, sy, sz);
        std::printf("Mode, z-skip, cull: %s, %s, %s\n", modenames[bras.getRenderMode()], tostr(bras.getSkipDetph()), cullnames[cull]);

        bras.clear();

        arma::mat44 rotmat = rotateZ(rotz) * rotateY(roty) * rotateX(rotx) * scale(sx, sy, sz);

        draw(bras, mesh, translate(tx, ty, tz) * rotmat, rotmat, buffer, static_cast<ECULLING_MODE>(cull));


        bras.rasterize();
        SDL_UpdateWindowSurface(win);
        SDL_Delay(25); //40 fps
    }


    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

