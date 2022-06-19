#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

#include <vector>
#include "vec2.h"

namespace TerrainToObj
{
    class HeightMap
    {
    public:
        HeightMap(const std::string& file);

        inline float HeightAt(int x, int y) const
        {
            return m_data[y * m_width + x];
        }

        inline float HeightAt(const Int2& p) const 
        {
            return m_data[p.y() * m_width + p.x()];
        }

        float GetWidth() const { return m_width; }
        float GetHeight() const { return m_height; }

        std::tuple<Int2, float> FindTriangle(const Int2& p0, 
            const Int2& p1, const Int2& p2);

    private:

        std::vector<float> m_data; // heightmap data
        int m_width = 0;
        int m_height = 0;
    };
}

#endif /* HEIGHT_MAP_H */
