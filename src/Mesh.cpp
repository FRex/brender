#include "Mesh.hpp"

Mesh::Mesh() { }

void Mesh::clear()
{
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
