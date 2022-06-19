#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <vector>
#include <memory>
#include <vec2.h>
#include <vec3.h>

namespace TerrainToObj
{
    class HeightMap;

    struct Mesh
    {
        std::vector<Float3> Points;
        std::vector<Int3> Triangles;
    };

    class MeshGenerator
    {
    public:
        MeshGenerator(std::unique_ptr<HeightMap> heightmap, float maxError, int maxTriangles, int maxPoints);
        void Generate();
        Mesh GetMesh(const Float3& meshScale);

    private:
        std::unique_ptr<HeightMap> m_heightmap;
        float m_maxError = 0.0f;
        int m_maxTriangles = 1000;
        int maxPoints = 1000;

        int AddPoint(const Int2& point);
        int AddTriangle(int a, int b, int c);

        std::vector<Int2> m_points;
        std::vector<int> m_triangles;
    };
}

#endif /* MESH_GENERATOR_H */
