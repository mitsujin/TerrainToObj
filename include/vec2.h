#ifndef VEC2_H
#define VEC2_H

#include <math.h>
#include <stdlib.h>
#include <iostream>

namespace TerrainToObj
{
    template <typename T>
    class Vec2
    {
    public:
        Vec2() : m_x(0), m_y(0) {}
        Vec2(T x, T y)
        : m_x(x)
        , m_y(y)
        {}

        inline T x() const { return m_x; }
        inline T y() const { return m_y; }

        inline Vec2 operator-() const { return Vec2(-m_x, -m_y); }

        inline T operator[] (int i) const { return m_data[i]; }

        inline Vec2<T> operator+(const Vec2& o) const
        {
            return Vec2(m_x + o.m_x, m_y + o.m_y);
        }

        inline Vec2& operator += (const Vec2& o)
        {
            m_x += o.m_x;
            m_y += o.m_y;

            return *this;
        }

        inline Vec2& operator -= (const Vec2& o)
        {
            m_x -= o.m_x;
            m_y -= o.m_y;

            return *this;
        }

        inline Vec2& operator /= (const Vec2& o)
        {
            m_x /= o.m_x;
            m_y /= o.m_y;

            return *this;
        }

        inline Vec2& operator *= (T t)
        {
            m_x *= t;
            m_y *= t;
            return *this;
        }

        inline Vec2& operator *= (const Vec2& o)
        {
            m_x *= o.m_x;
            m_y *= o.m_y;

            return *this;
        }


        inline Vec2& operator /= (T t)
        {
            m_x /= t;
            m_y /= t;
            return *this;
        }

        inline Vec2 operator-(Vec2 o) const { return Vec2(m_x-o.m_x, m_y-o.m_y); }
        inline Vec2 operator*(T t) const { return Vec2(m_x*t, m_y*t); }
        inline Vec2 operator/(T t) const { return Vec2(m_x/t, m_y/t); }

        inline T Length() const
        {
            return sqrt(m_x * m_x + m_y * m_y);
        }

        inline T LengthSq() const 
        {
            return m_x * m_x + m_y * m_y;
        }

        inline Vec2<T> Normalize() const
        {
            return *this / Length();
        }

        template <typename U>
        friend Vec2<U> operator * (U t, Vec2<U> o);

        template <typename U>
        friend U Dot(const Vec2<U>& a, const Vec2<U>& b);

        template <typename U>
        friend Vec2<U> operator * (Vec2<U> a, Vec2<U> b);

    private:
        union
        {
            struct
            {
                T m_x;
                T m_y;
            };
            struct
            {
                T m_r;
                T m_g;
            };
            T m_data[2];
        };
    };

    /*
    template <typename T>
    inline Vec2<T> operator + (const Vec2<T>& a, const Vec2<T>& b)
    {
        return Vec2<T>(m_x + b.m_x, m_y + b.m_y, m_z + b.m_z);
    }
    */
    template <typename T>
    Vec2<T> operator * (T t, Vec2<T> o)
    {
        return Vec2<T>(t*o.m_x, t*o.m_y);
    }
    
    template <typename T>
    Vec2<T> operator * (Vec2<T> a, Vec2<T> b)
    {
        return Vec2<T>(a.m_x*b.m_x, a.m_y*b.m_y);
    }

    template <typename T>
    T Dot(const Vec2<T>& a, const Vec2<T>& b)
    {
        return a.m_x * b.m_x + a.m_y*b.m_y;
    }

    using Float2 = Vec2<float>;
    using Double2 = Vec2<double>;
}

#endif /* VEC2_H */
