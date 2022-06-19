#ifndef TRI_PRIORITY_QUEUE_H
#define TRI_PRIORITY_QUEUE_H

#include <queue>

namespace TerrainToObj
{
    class TriPriorityQueue
    {
    public:
        void Push(int triangle, float error);
        int Pop();
        int Size() const { return m_queue.size(); }

    private:
        struct TriWithError
        {
            TriWithError(int tri, float error) : Triangle(tri), Error(error) {}
            int Triangle = 0;
            float Error = 0.0f;
            bool operator < (const TriWithError& e) const
            {
                return Error < e.Error;
            }
        };

        std::priority_queue<TriWithError> m_queue;
    };
}

#endif /* TRI_PRIORITY_QUEUE_H */
