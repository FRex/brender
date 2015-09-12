#ifndef MESH_HPP
#define	MESH_HPP

#include <vector>
#include "Vertex.hpp"

enum EMESH_PRIMITIVE_TYPE
{
    EMPT_LINE ,
    EMPT_TRIANGLE,
    
    EMESH_PRIMITIVE_TYPE_COUNT
};

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

    void setPrimitiveType(EMESH_PRIMITIVE_TYPE type);
    EMESH_PRIMITIVE_TYPE getPrimitiveType() const;
    
private:
    EMESH_PRIMITIVE_TYPE m_primtype;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned> m_indices;

};

#endif	/* MESH_HPP */

