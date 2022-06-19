#include "gtest/gtest.h"
#include <tri_priority_queue.h>

namespace TerrainToObj
{
    TEST(TriPriorityQueueTests, TestPushing)
    {
        TriPriorityQueue queue;
        queue.Push(1, 1.0f);
        queue.Push(2, 2.0f);
        queue.Push(3, 3.0f);
        
        ASSERT_EQ(3, queue.Size());
    }

    TEST(TriPriorityQueueTests, TestOrder)
    {
        TriPriorityQueue queue;
        queue.Push(1, 1.0f);
        queue.Push(2, 2.0f);
        queue.Push(3, 3.0f);
        queue.Push(0, 1.0f);

        ASSERT_EQ(3, queue.Pop());
        ASSERT_EQ(2, queue.Pop());
        ASSERT_EQ(1, queue.Pop());
        ASSERT_EQ(0, queue.Pop());
    }

    TEST(TriPriorityQueueTests, TestRemove)
    {
        TriPriorityQueue queue;
        queue.Push(1, 1.0f);

        ASSERT_TRUE(queue.Remove(1));
    }

    TEST(TriPriorityQueueTests, TestCantRemoveEmpty)
    {
        TriPriorityQueue queue;

        ASSERT_FALSE(queue.Remove(1));
    }

    TEST(TriPriorityQueueTests, TestRemovePreservesOrder)
    {
        TriPriorityQueue queue;
        queue.Push(1, 1.0f);
        queue.Push(2, 2.0f);
        queue.Push(3, 3.0f);
        queue.Push(0, 1.0f);

        ASSERT_TRUE(queue.Remove(3));
        ASSERT_EQ(2, queue.Pop());
    }
}
