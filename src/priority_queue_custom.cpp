#include "priority_queue_custom.h"

namespace TerrainToObj
{
    void PriorityQueue::Push(int t, float error)
    {
        m_errors[t] = error;
        int i = m_queue.size();
        m_queueIndices[t] = i;
        m_queue.push_back(t);
        Up(i);
    }

    int PriorityQueue::Pop()
    {
        const int n = m_queue.size() - 1;
        Swap(0, n);
        Down(0, n);
        return PopBack();
    }

    int PriorityQueue::PopBack() 
    {
        const int t = m_queue.back();
        m_queue.pop_back();
        m_queueIndices[t] = -1;
        return t;
    }

    bool PriorityQueue::Remove(int t)
    {
        const int i = m_queueIndices[t];
        if (i < 0) 
        {
            return false;
        }

        const int n = m_queue.size() - 1;
        if (n != i)
        {
            Swap(i, n);
            if (!Down(i, n)) 
            {
                Up(i);
            }
        }
        PopBack();
        return true;
    }

    bool PriorityQueue::Less(int i, int j) const
    {
        return -m_errors[m_queue[i]] < -m_errors[m_queue[j]];
    }

    void PriorityQueue::Swap(int i, int j) 
    {
        const int pi = m_queue[i];
        const int pj = m_queue[j];
        m_queue[i] = pj;
        m_queue[j] = pi;
        m_queueIndices[pi] = j;
        m_queueIndices[pj] = i;
    }

    void PriorityQueue::Up(int j0)
    {
        int j = j0;
        while (1) {
            int i = (j - 1) / 2;
            if (i == j || !Less(j, i)) {
                break;
            }
            Swap(i, j);
            j = i;
        }
    }

    bool PriorityQueue::Down(int i0, int n)
    {
        int i = i0;
        while (1) {
            const int j1 = 2 * i + 1;
            if (j1 >= n || j1 < 0) {
                break;
            }
            const int j2 = j1 + 1;
            int j = j1;
            if (j2 < n && Less(j2, j1)) {
                j = j2;
            }
            if (!Less(j, i)) {
                break;
            }
            Swap(i, j);
            i = j;
        }
        return i > i0;
    }
}

