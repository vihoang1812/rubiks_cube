
#include "mesh.h"
#include "graphics.h"

Mesh::Mesh(const mat<float>& vertexData, const vec<uint>& indexData) {
    for(auto row : vertexData) {
        for(auto e : row) {
            this->vertexData.push_back(e);        
        }
    }

    for(auto e : indexData) this->indexData.push_back(e);
    id = Graphics::getInstance().registerMesh(
        getVertexDataSize(), &(getVertexData())[0],
        getIndexDataSize(), &(getIndexData())[0]);
}

uint Mesh::getVertexDataSize() const {
    return vertexData.size() * sizeof(float);
}

uint Mesh::getIndexDataSize() const {
    return indexData.size() * sizeof(uint);
}

vec<float> Mesh::getVertexData() const {
    return vertexData;
}
vec<uint> Mesh::getIndexData() const {
    return indexData;
}

uint Mesh::getNumIndicies() const{
    return indexData.size();
}

uint Mesh::getId() const{
    return id;
}
