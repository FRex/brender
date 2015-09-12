#include "Mesh.hpp"

Mesh::Mesh() : m_primtype(EMPT_LINE) { }

void Mesh::clear()
{
    m_primtype = EMPT_LINE;
    m_indices.clear();
    m_vertices.clear();
}

void Mesh::addVertex(const Vertex& vertex)
{
    m_vertices.push_back(vertex);
}

void Mesh::addIndex(unsigned a)
{
    m_indices.push_back(a);
}

void Mesh::addLine(unsigned a, unsigned b)
{
    addIndex(a);
    addIndex(b);
}

void Mesh::addTriangle(unsigned a, unsigned b, unsigned c)
{
    addIndex(a);
    addIndex(b);
    addIndex(c);
}

unsigned Mesh::getVertexCount() const
{
    return m_vertices.size();
}

const Vertex& Mesh::getVertex(unsigned idx) const
{
    return m_vertices[idx];
}

const Vertex * Mesh::getVertices() const
{
    return &m_vertices[0];
}

unsigned Mesh::getIndexCount() const
{
    return m_indices.size();
}

unsigned Mesh::getIndex(unsigned idx) const
{
    return m_indices[idx];
}

const unsigned* Mesh::getIndices() const
{
    return &m_indices[0];
}

void Mesh::setPrimitiveType(EMESH_PRIMITIVE_TYPE type)
{
    m_primtype = type;
}

EMESH_PRIMITIVE_TYPE Mesh::getPrimitiveType() const
{
    return m_primtype;
}
