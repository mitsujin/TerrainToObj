#include "gtest/gtest.h"
#include <vec3.h>

namespace TerrainToObj
{
    typedef ::testing::Types<float, double> Vec3Types;

    template <typename T>
    class Vec3Tests : public testing::Test {};

    TYPED_TEST_SUITE(Vec3Tests, Vec3Types);

    TYPED_TEST(Vec3Tests, TestConstructor)
    {
        Vec3<TypeParam> v;
        ASSERT_FLOAT_EQ(0.0, v.x());    
        ASSERT_FLOAT_EQ(0.0, v.y());    
        ASSERT_FLOAT_EQ(0.0, v.z());    
    }

    TYPED_TEST(Vec3Tests, TestAdd)
    {
        Vec3<TypeParam> a(1, 2, 3);
        Vec3<TypeParam> b(4, 5, 6);

        auto c = a + b;
        ASSERT_FLOAT_EQ(5.0, c.x());    
        ASSERT_FLOAT_EQ(7.0, c.y());    
        ASSERT_FLOAT_EQ(9.0, c.z());    
    }

    TYPED_TEST(Vec3Tests, TestMultiplyValue)
    {
        Vec3<TypeParam> a(1, 2, 3);
        TypeParam t = 10.0;
        auto c = a * t;

        ASSERT_FLOAT_EQ(10.0, c.x());    
        ASSERT_FLOAT_EQ(20.0, c.y());    
        ASSERT_FLOAT_EQ(30.0, c.z());    

        c = t * a;
        ASSERT_FLOAT_EQ(10.0, c.x());    
        ASSERT_FLOAT_EQ(20.0, c.y());    
        ASSERT_FLOAT_EQ(30.0, c.z());    
    }

}
