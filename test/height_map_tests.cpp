#include "gtest/gtest.h"
#include <height_map.h>

namespace TerrainToObj
{
    TEST(HeightMapTests, TestLoadFile)
    {
        HeightMap map("./data/heightmap.png");
        
        ASSERT_EQ(2048, map.GetWidth());
        ASSERT_EQ(2048, map.GetHeight());
    }
}
