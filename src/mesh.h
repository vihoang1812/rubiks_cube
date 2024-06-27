#ifndef MESH_H
#define MESH_H

#include <vector>
#include "common_alias.h"

class Mesh {
private:
    vec<float> vertexData;
    vec<uint> indexData;
    uint id;
public:
    Mesh() = default;
    Mesh(const mat<float>& vertexData, const vec<uint>& indexData);
    uint getVertexDataSize() const;
    uint getIndexDataSize() const;
    vec<float> getVertexData() const;
    vec<uint> getIndexData() const;
    uint getNumIndicies() const;
    uint getId() const;
};

#endif