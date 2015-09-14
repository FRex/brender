#include <SDL.h>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <vector>

#include "Rasterizer.hpp"
#include "Mesh.hpp"

#include "renderMesh.hpp"
#include "makeMesh.hpp"

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

const float pi = 3.1415f; // ^_^...

void loadMesh(Mesh& mesh, const char * filename)
{
    std::printf("Loading file: %s\n", filename);
    std::ifstream file(filename);
    mesh.clear();
    mesh.setPrimitiveType(EMPT_TRIANGLE);
    float scale;
    float x, y, z;
    unsigned c = 0u;
    file >> scale;
    while(file >> x >> y >> z)
    {
        mesh.addVertex(Vertex(Vector3(scale * x, scale * y, scale * z), 0xff0000));
        mesh.addIndex(c++);
    }
    std::printf("%u total vertices, scale is %f\n", c, scale);
}

int main(int argc, char ** argv)
{
    SDL_Window * win = NULL;
    SDL_Surface * sur = NULL;
    int run = 1;
    SDL_Event eve;
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("BRender", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    sur = SDL_GetWindowSurface(win);

    Rasterizer bras(sur->pixels);
    printf("%s\n", SDL_GetPixelFormatName(sur->format->format));

    Mesh mesh;
    if(argc < 2)
    {
        makeCubeMesh(mesh);
    }
    else
    {
        loadMesh(mesh, argv[1]);
    }
    float tx = 0.f;
    float ty = 0.f;
    float tz = 500.f;
    float rotx = 0.f;
    float roty = 0.f;
    float rotz = 0.f;


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
                }
            }
        }//while sdl poll event eve


        std::printf("R: %f, %f, %f\n", rotx, roty, rotz);
        std::printf("T: %f, %f, %f\n", tx, ty, tz);

        bras.clear();

        draw(bras, mesh, translate(tx, ty, tz) * rotateZ(rotz) * rotateY(roty) * rotateX(rotx));


        bras.rasterize();
        SDL_UpdateWindowSurface(win);
        SDL_Delay(25); //40 fps
    }


    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

