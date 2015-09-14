#ifndef MESH_HPP
#define	MESH_HPP

#include <vector>
#include "Vertex.hpp"

class Mesh
{
public:
    Mesh();
    
    void clear();
    
    void addVertex(const Vertex& vertex);
    void addIndex(unsigned a);
    void addLine(unsigned a, unsigned b);
    void addTriangle(unsigned a, unsigned b, unsigned c);

    unsigned getVertexCount() const;
    const Vertex& getVertex(unsigned idx) const;
    const Vertex * getVertices() const;

    unsigned getIndexCount() const;
    unsigned getIndex(unsigned idx) const;
    const unsigned * getIndices() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned> m_indices;

};

#endif	/* MESH_HPP */

