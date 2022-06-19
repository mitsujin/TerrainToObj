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
        const int t0 = AddTriangle(p3, p0, p2, -1, -1, -1);
        AddTriangle(p0, p3, p1, t0, -1, -1);
        ProcessPending();

        while (TriCount() < m_maxTriangles)
        {
            Update();
        }
    }

    void MeshGenerator::Update()
    {
        // Get the triangle with the highest error
        const int t = m_queue.Pop();

        const int e0 = t * 3 + 0;
        const int e1 = t * 3 + 1;
        const int e2 = t * 3 + 2;

        const int p0 = m_triangles[e0];
        const int p1 = m_triangles[e1];
        const int p2 = m_triangles[e2];

        const Int2 a = m_points[p0];
        const Int2 b = m_points[p1];
        const Int2 c = m_points[p2];
        const Int2 p = m_candidatePoints[t];

        const int pn = AddPoint(p);

        const auto collinear = [](
            const auto& p0, const auto& p1, const auto& p2)
        {
            return (p1.y()-p0.y())*(p2.x()-p1.x()) == (p2.y()-p1.y())*(p1.x()-p0.x());
        };

        const auto handleCollinear = [this](const int pn, const int a) {
            const int a0 = a - a % 3;
            const int al = a0 + (a + 1) % 3;
            const int ar = a0 + (a + 2) % 3;
            const int p0 = m_triangles[ar];
            const int pr = m_triangles[a];
            const int pl = m_triangles[al];
            const int hal = m_halfEdges[al];
            const int har = m_halfEdges[ar];

            const int b = m_halfEdges[a];

            if (b < 0) {
                const int t0 = SetTriangle(a0, pn, p0, pr, -1, har, -1);
                const int t1 = AddTriangle(p0, pn, pl, t0, -1, hal);

                Validate(t0 + 1);
                Validate(t1 + 2);
                return;
            }

            const int b0 = b - b % 3;
            const int bl = b0 + (b + 2) % 3;
            const int br = b0 + (b + 1) % 3;
            const int p1 = m_triangles[bl];
            const int hbl = m_halfEdges[bl];
            const int hbr = m_halfEdges[br];

        };
    }

    void MeshGenerator::Validate(int a)
    {

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

    int MeshGenerator::SetTriangle(int index, int a, int b, int c, int ab, int bc, int ca)
    {
        m_triangles[index + 0] = a;
        m_triangles[index + 1] = b;
        m_triangles[index + 2] = c;

        m_halfEdges[index + 0] = ab;
        m_halfEdges[index + 1] = bc;
        m_halfEdges[index + 2] = ca;

        AddToPending(index);

        return index;
    }

    int MeshGenerator::AddTriangle(int a, int b, int c, int ab, int bc, int ca)
    {
        const int index = m_triangles.size();
        m_triangles.push_back(a);
        m_triangles.push_back(b);
        m_triangles.push_back(c);

        m_halfEdges.push_back(ab);
        m_halfEdges.push_back(bc);
        m_halfEdges.push_back(ca);

        m_candidatePoints.emplace_back(0,0);

        AddToPending(index);

        return index;
    }

    void MeshGenerator::ProcessPending()
    {
        for (auto t : m_pendingTriangles)
        {
            // compare this triangle with the heighmap to determine error
            auto [candidate, error] = m_heightmap->FindTriangle(
                m_points[m_triangles[t*3+0]],
                m_points[m_triangles[t*3+1]],
                m_points[m_triangles[t*3+2]]);

            m_candidatePoints[t] = candidate;
            m_queue.Push(t, error);
        }

        m_pendingTriangles.clear();
    }
}
