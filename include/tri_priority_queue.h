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

        bool Remove(int triangle);

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

        class PriorityQueueInternal : public std::priority_queue<TriWithError, std::vector<TriWithError>>
        {
        public:
            bool remove(const TriWithError& e)
            {
                auto it = std::find_if(this->c.begin(), this->c.end(), [&](auto candidate) {
                    return e.Triangle == candidate.Triangle; });

                if (it != this->c.end())
                {
                    this->c.erase(it);
                    std::make_heap(this->c.begin(), this->c.end(), this->comp);
                    return true;
                }
                return false;
            }
        };

        PriorityQueueInternal m_queue;
    };
}

#endif /* TRI_PRIORITY_QUEUE_H */
