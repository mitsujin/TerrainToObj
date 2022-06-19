#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

namespace TerrainToObj
{
    template <typename T>
    class Vec3
    {
    public:
        Vec3() : m_x(0), m_y(0), m_z(0) {}
        Vec3(T x, T y, T z)
        : m_x(x)
        , m_y(y)
        , m_z(z) {}

        inline T x() const { return m_x; }
        inline T y() const { return m_y; }
        inline T z() const { return m_z; } inline T r() const { return m_x; }
        inline T g() const { return m_y; }
        inline T b() const { return m_z; }

        inline Vec3 operator-() const { return Vec3(-m_x, -m_y, -m_z); }

        inline T operator[] (int i) const { return m_data[i]; }

        inline Vec3<T> operator+(const Vec3& o) const
        {
            return Vec3(m_x + o.m_x, m_y + o.m_y, m_z + o.m_z);
        }

        inline Vec3& operator += (const Vec3& o)
        {
            m_x += o.m_x;
            m_y += o.m_y;
            m_z += o.m_z;

            return *this;
        }

        inline Vec3& operator -= (const Vec3& o)
        {
            m_x -= o.m_x;
            m_y -= o.m_y;
            m_z -= o.m_z;

            return *this;
        }

        inline Vec3& operator /= (const Vec3& o)
        {
            m_x /= o.m_x;
            m_y /= o.m_y;
            m_z /= o.m_z;

            return *this;
        }

        inline Vec3& operator *= (T t)
        {
            m_x *= t;
            m_y *= t;
            m_z *= t;
            return *this;
        }

        inline Vec3& operator *= (const Vec3& o)
        {
            m_x *= o.m_x;
            m_y *= o.m_y;
            m_z *= o.m_z;

            return *this;
        }


        inline Vec3& operator /= (T t)
        {
            m_x /= t;
            m_y /= t;
            m_z /= t;
            return *this;
        }

        inline Vec3 operator-(Vec3 o) const { return Vec3(m_x-o.m_x, m_y-o.m_y, m_z-o.m_z); }
        inline Vec3 operator*(T t) const { return Vec3(m_x*t, m_y*t, m_z*t); }
        inline Vec3 operator/(T t) const { return Vec3(m_x/t, m_y/t, m_z/t); }

        inline T Length() const
        {
            return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
        }

        inline T LengthSq() const 
        {
            return m_x * m_x + m_y * m_y + m_z * m_z;
        }

        inline Vec3<T> Normalize() const
        {
            return *this / Length();
        }

        template <typename U>
        friend Vec3<U> operator * (U t, Vec3<U> o);

        template <typename U>
        friend U Dot(const Vec3<U>& a, const Vec3<U>& b);

        template <typename U>
        friend Vec3<U> Cross(const Vec3<U>& a, const Vec3<U>& b);

        template <typename U>
        friend Vec3<U> operator * (Vec3<U> a, Vec3<U> b);

    private:
        union
        {
            struct
            {
                T m_x;
                T m_y;
                T m_z;
            };
            struct
            {
                T m_r;
                T m_g;
                T m_b;
            };
            T m_data[3];
        };
    };

    /*
    template <typename T>
    inline Vec3<T> operator + (const Vec3<T>& a, const Vec3<T>& b)
    {
        return Vec3<T>(m_x + b.m_x, m_y + b.m_y, m_z + b.m_z);
    }
    */
    template <typename T>
    Vec3<T> operator * (T t, Vec3<T> o)
    {
        return Vec3<T>(t*o.m_x, t*o.m_y, t*o.m_z);
    }
    
    template <typename T>
    Vec3<T> operator * (Vec3<T> a, Vec3<T> b)
    {
        return Vec3<T>(a.m_x*b.m_x, a.m_y*b.m_y, a.m_z*b.m_z);
    }

    template <typename T>
    T Dot(const Vec3<T>& a, const Vec3<T>& b)
    {
        return a.m_x * b.m_x + a.m_y*b.m_y + a.m_z*b.m_z;
    }

    template <typename T>
    Vec3<T> Cross(const Vec3<T>& a, const Vec3<T>& b)
    {
        return Vec3<T>(
            (a.m_y * b.m_z - a.m_z * b.m_y), 
            (-(a.m_x*b.m_z - a.m_z * b.m_x)),
            (a.m_x*b.m_y - a.m_y*b.m_x));
    }

    using Float3 = Vec3<float>;
    using Double3 = Vec3<double>;
    using Int3 = Vec3<int>;
}

#endif /* VEC3_H */
