#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include <vector>
#include <memory>
#include <vec2.h>
#include <vec3.h>

#include "tri_priority_queue.h"

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
        int TriCount() const { return m_queue.Size(); }
        void Update();

        std::unique_ptr<HeightMap> m_heightmap;
        float m_maxError = 0.0f;
        int m_maxTriangles = 1000;
        int maxPoints = 1000;

        int AddPoint(const Int2& point);
        int AddTriangle(int a, int b, int c, int ab, int bc, int ca);
        int SetTriangle(int index, int a, int b, int c, int ab, int bc, int ca);
        void LinkEdges(int index, int ab, int bc, int ca);

        void ProcessPending();
        void SatisfyDelaunay(int a);
        void RemoveFromQueue(int t);


        inline void AddToPending(int triangleFirstIndex)
        {
            const int t = triangleFirstIndex / 3;
            m_pendingTriangles.push_back(t);
        }

        std::vector<Int2> m_points;
        std::vector<int> m_triangles;
        std::vector<int> m_halfEdges;

        std::vector<Int2> m_candidatePoints;

        std::vector<int> m_pendingTriangles; // To be processed
        TriPriorityQueue m_queue;
    };
}

#endif /* MESH_GENERATOR_H */
