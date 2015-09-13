#include "makeMesh.hpp"
#include "Mesh.hpp"

void makeCubeMesh(Mesh& mesh)
{
    const float kCubeSide = 150.f;

    const unsigned colors[8] = {
        0xff0000, //0 = red
        0x00ff00, //1 = green
        0x0000ff, //2 = blue
        0xffff00, //3 = yellow

        0xff00ff, //4 = magenta
        0x00ffff, //5 = cyan
        0xffffff, //6 = white
        0x000000, //7 = black
    };

    mesh.addVertex(Vertex(Vector3(-kCubeSide, -kCubeSide, -kCubeSide), colors[0])); //0
    mesh.addVertex(Vertex(Vector3(-kCubeSide, -kCubeSide, kCubeSide), colors[1])); //1
    mesh.addVertex(Vertex(Vector3(-kCubeSide, kCubeSide, -kCubeSide), colors[2])); //2
    mesh.addVertex(Vertex(Vector3(-kCubeSide, kCubeSide, kCubeSide), colors[3])); //3

    mesh.addVertex(Vertex(Vector3(kCubeSide, -kCubeSide, -kCubeSide), colors[4])); //4
    mesh.addVertex(Vertex(Vector3(kCubeSide, -kCubeSide, kCubeSide), colors[5])); //5
    mesh.addVertex(Vertex(Vector3(kCubeSide, kCubeSide, -kCubeSide), colors[6])); //6
    mesh.addVertex(Vertex(Vector3(kCubeSide, kCubeSide, kCubeSide), colors[7])); //7

    mesh.setPrimitiveType(EMPT_TRIANGLE);
    //up && down
    //    mesh.addTriangle(3, 6, 2);
    //    mesh.addTriangle(3, 7, 6);
    //    mesh.addTriangle(0, 1, 5);
    //    mesh.addTriangle(0, 4, 5);

    if(1)
    {
        //sides
        mesh.addTriangle(0, 2, 4);
        mesh.addTriangle(6, 4, 2);

        mesh.addTriangle(6, 5, 4);
        mesh.addTriangle(6, 7, 5);

        mesh.addTriangle(7, 1, 5);
        mesh.addTriangle(7, 3, 1);

        mesh.addTriangle(3, 0, 1);
        mesh.addTriangle(3, 2, 0);
    }
}
