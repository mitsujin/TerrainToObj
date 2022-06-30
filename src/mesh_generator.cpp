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

        const auto done = [this]() {
            const float e = m_queue.Error();
            if (e <= m_maxError) {
                return true;
            }
            if (m_maxTriangles > 0 && TriCount() >= m_maxTriangles) {
                return true;
            }

            return e == 0;
        };

        while (!done()) 
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

                SatisfyDelaunay(t0 + 1);
                SatisfyDelaunay(t1 + 2);
                return;
            }

            const int b0 = b - b % 3;
            const int bl = b0 + (b + 2) % 3;
            const int br = b0 + (b + 1) % 3;
            const int p1 = m_triangles[bl];
            const int hbl = m_halfEdges[bl];
            const int hbr = m_halfEdges[br];

            RemoveFromQueue(b / 3);

            const int t0 = SetTriangle(a0, p0, pr, pn, har, -1, -1);
            const int t1 = SetTriangle(b0, pr, p1, pn, hbr, -1, t0 + 1);
            const int t2 = AddTriangle(p1, pl, pn, hbl, -1, t1 + 1);
            const int t3 = AddTriangle(pl, p0, pn, hal, t0 + 2, t2 + 1);

            SatisfyDelaunay(t0);
            SatisfyDelaunay(t1);
            SatisfyDelaunay(t2);
            SatisfyDelaunay(t3);
        };

        if (collinear(a, b, p)) {
            handleCollinear(pn, e0);
        } else if (collinear(b, c, p)) {
            handleCollinear(pn, e1);
        } else if (collinear(c, a, p)) {
            handleCollinear(pn, e2);
        } else {
            const int h0 = m_halfEdges[e0];
            const int h1 = m_halfEdges[e1];
            const int h2 = m_halfEdges[e2];

            const int t0 = SetTriangle(e0, p0, p1, pn, h0, -1, -1);
            const int t1 = AddTriangle(p1, p2, pn, h1, -1, t0 + 1);
            const int t2 = AddTriangle(p2, p0, pn, h2, t0 + 2, t1 + 1);

            SatisfyDelaunay(t0);
            SatisfyDelaunay(t1);
            SatisfyDelaunay(t2);
        }

        ProcessPending();
    }

    void MeshGenerator::SatisfyDelaunay(int a)
    {
        // if the pair of triangles doesn't satisfy the Delaunay condition
        // (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
        // then do the same check/flip recursively for the new pair of triangles
        //
        //           pl                    pl
        //          /||\                  /  \
        //       al/ || \bl            al/    \a
        //        /  ||  \              /      \
        //       /  a||b  \    flip    /___ar___\
        //     p0\   ||   /p1   =>   p0\---bl---/p1
        //        \  ||  /              \      /
        //       ar\ || /br             b\    /br
        //          \||/                  \  /
        //           pr                    pr
        //
        const auto inCircle = [](
            const Int2& a, const Int2& b, const Int2& c,
            const Int2& p)
        {
            const int64_t dx = a.x() - p.x();
            const int64_t dy = a.y() - p.y();
            const int64_t ex = b.x() - p.x();
            const int64_t ey = b.y() - p.y();
            const int64_t fx = c.x() - p.x();
            const int64_t fy = c.y() - p.y();
            const int64_t ap = dx * dx + dy * dy;
            const int64_t bp = ex * ex + ey * ey;
            const int64_t cp = fx * fx + fy * fy;
            return dx*(ey*cp-bp*fy)-dy*(ex*cp-bp*fx)+ap*(ex*fy-ey*fx) < 0;
        };

        const int b = m_halfEdges[a];

        if (b < 0) {
            return;
        }

        const int a0 = a - a % 3;
        const int b0 = b - b % 3;
        const int al = a0 + (a + 1) % 3;
        const int ar = a0 + (a + 2) % 3;
        const int bl = b0 + (b + 2) % 3;
        const int br = b0 + (b + 1) % 3;
        const int p0 = m_triangles[ar];
        const int pr = m_triangles[a];
        const int pl = m_triangles[al];
        const int p1 = m_triangles[bl];

        if (!inCircle(m_points[p0], m_points[pr], m_points[pl], m_points[p1])) {
            return;
        }

        const int hal = m_halfEdges[al];
        const int har = m_halfEdges[ar];
        const int hbl = m_halfEdges[bl];
        const int hbr = m_halfEdges[br];

        RemoveFromQueue(a / 3);
        RemoveFromQueue(b / 3);

        const int t0 = SetTriangle(a0, p0, p1, pl, -1, hbl, hal);
        const int t1 = SetTriangle(b0, p1, p0, pr, t0, har, hbr);

        SatisfyDelaunay(t0 + 1);
        SatisfyDelaunay(t1 + 2);
    }

    Mesh MeshGenerator::GetMesh(const Float3& meshScale)
    {
        Mesh mesh;
        const float upScale = meshScale.z();

        mesh.Points.reserve(m_points.size());
        mesh.Triangles.reserve(TriCount());

        const int h = m_heightmap->GetHeight() - 1;
        for (auto& p : m_points)
        {
            mesh.Points.emplace_back(p.x(), h - p.y(), m_heightmap->HeightAt(p.x(), p.y()) * upScale);
        }

        int triCount = TriCount();
        while (TriCount() > 0)
        {
            int t = m_queue.Pop();
            mesh.Triangles.emplace_back(
                m_triangles[t*3 + 0], 
                m_triangles[t*3 + 1], 
                m_triangles[t*3 + 2]);
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
        if (index < 0)
        {
            return AddTriangle(a, b, c, ab, bc, ca);
        }

        m_triangles[index + 0] = a;
        m_triangles[index + 1] = b;
        m_triangles[index + 2] = c;

        m_halfEdges[index + 0] = ab;
        m_halfEdges[index + 1] = bc;
        m_halfEdges[index + 2] = ca;

        LinkEdges(index, ab, bc, ca);
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
        m_queue.Add();

        LinkEdges(index, ab, bc, ca);
        AddToPending(index);

        return index;
    }

    void MeshGenerator::LinkEdges(int index, int ab, int bc, int ca)
    {
        if (ab >= 0) 
        {
            m_halfEdges[ab] = index + 0;
        }

        if (bc >= 0) 
        {
            m_halfEdges[bc] = index + 1;
        }

        if (ca >= 0) 
        {
            m_halfEdges[ca] = index + 2;
        }
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

    void MeshGenerator::RemoveFromQueue(int t)
    {
        //m_queue.Remove(t);
        //if m
        if (!m_queue.Remove(t))
        {
            const auto it = std::find(m_pendingTriangles.begin(), m_pendingTriangles.end(), t);
            if (it != m_pendingTriangles.end())
            {
                std::swap(*it, m_pendingTriangles.back());
                m_pendingTriangles.pop_back();
            }
        }
        /*
        else
        {
            m_queue.Remove(t);
        }
        */
    }
}
