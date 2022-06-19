#include "gtest/gtest.h"
#include <vec2.h>

namespace TerrainToObj
{
    typedef ::testing::Types<float, double> Vec2Types;

    template <typename T>
    class Vec2Tests : public testing::Test {};

    TYPED_TEST_SUITE(Vec2Tests, Vec2Types);

    TYPED_TEST(Vec2Tests, TestConstructor)
    {
        Vec2<TypeParam> v;
        ASSERT_FLOAT_EQ(0.0, v.x());    
        ASSERT_FLOAT_EQ(0.0, v.y());    
    }

    TYPED_TEST(Vec2Tests, TestAdd)
    {
        Vec2<TypeParam> a(1, 2);
        Vec2<TypeParam> b(4, 5);

        auto c = a + b;
        ASSERT_FLOAT_EQ(5.0, c.x());    
        ASSERT_FLOAT_EQ(7.0, c.y());    
    }

    TYPED_TEST(Vec2Tests, TestMultiplyValue)
    {
        Vec2<TypeParam> a(1, 2);
        TypeParam t = 10.0;
        auto c = a * t;

        ASSERT_FLOAT_EQ(10.0, c.x());    
        ASSERT_FLOAT_EQ(20.0, c.y());    

        c = t * a;
        ASSERT_FLOAT_EQ(10.0, c.x());    
        ASSERT_FLOAT_EQ(20.0, c.y());    
    }

}
