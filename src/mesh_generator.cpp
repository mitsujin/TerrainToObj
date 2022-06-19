#include "mesh_generator.h"
#include "height_map.h"

namespace TerrainToObj
{
    MeshGenerator::MeshGenerator(std::unique_ptr<HeightMap> heightmap, float maxError, int maxTriangles, int maxPoints)
    : m_heightmap(std::move(heightmap))
    , m_maxError(maxError)
    , m_maxTriangles(maxTriangles)
    , maxPoints(maxPoints)
    {

    }

    void MeshGenerator::Generate()
    {
        // Add points for all corners
        const int x0 = 0;
        const int y0 = 0;
        const int x1 = m_heightmap->GetWidth() - 1;
        const int y1 = m_heightmap->GetHeight() - 1;
        const int p0 = AddPoint(Int2(x0, y0));
        const int p1 = AddPoint(Int2(x1, y0));
        const int p2 = AddPoint(Int2(x0, y1));
        const int p3 = AddPoint(Int2(x1, y1));

        // Add the intial triangles
        const int t0 = AddTriangle(p3, p0, p2);
        AddTriangle(p0, p3, p1);
    }

    Mesh MeshGenerator::GetMesh(const Float3& meshScale)
    {
        Mesh mesh;
        const float upScale = meshScale.z();

        mesh.Points.reserve(m_points.size());
        mesh.Triangles.reserve(m_triangles.size());

        const int h = m_heightmap->GetHeight() - 1;
        for (auto& p : m_points)
        {
            mesh.Points.emplace_back(p.x(), h - p.y(), m_heightmap->HeightAt(p.x(), p.y()) * upScale);
        }

        int triCount = m_triangles.size() / 3;
        for (int i = 0; i < triCount; i++)
        {
            mesh.Triangles.emplace_back(
                m_triangles[i*3 + 0], 
                m_triangles[i*3 + 1], 
                m_triangles[i*3 + 2]);
        }

        return mesh;
    }
    
    int MeshGenerator::AddPoint(const Int2& point)
    {
        const int i = m_points.size();
        m_points.push_back(point);
        return i;
    }

    int MeshGenerator::AddTriangle(int a, int b, int c)
    {
        const int index = m_triangles.size();
        m_triangles.push_back(a);
        m_triangles.push_back(b);
        m_triangles.push_back(c);

        return index;
    }
}
