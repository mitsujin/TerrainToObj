#include "tri_priority_queue.h"

namespace TerrainToObj
{
    void TriPriorityQueue::Push(int triangle, float error)
    {
        m_queue.emplace(triangle, error);
    }

    int TriPriorityQueue::Pop() 
    {
        if (!m_queue.empty())
        {
            auto tri = m_queue.top();
            m_queue.pop();
            return tri.Triangle;
        }
        return -1;
    }
}
