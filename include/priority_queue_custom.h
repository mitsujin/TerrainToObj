#ifndef PRIORITY_QUEUE_CUSTOM_H
#define PRIORITY_QUEUE_CUSTOM_H
#include <vector>

namespace TerrainToObj
{
    class PriorityQueue
    {
    public:
        void Add()
        {
            m_errors.push_back(0);
            m_queueIndices.push_back(-1);
        }

        void Push(int t, float error);
        int Pop();
        int PopBack();
        bool Remove(int t);
        int Size() const { return m_queue.size(); }

        float Error() { return m_errors[m_queue[0]]; }

        bool Less(int i, int j) const;
        void Swap(int i, int j);
        void Up(int j0);
        bool Down(int i0, int n);

        std::vector<int> m_queue;
        std::vector<int> m_queueIndices;
        std::vector<float> m_errors;
    };
}

#endif /* PRIORITY_QUEUE_CUSTOM_H */
