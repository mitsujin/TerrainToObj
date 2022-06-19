#include "height_map.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

namespace TerrainToObj
{
    HeightMap::HeightMap(const std::string& file)
    {
        int c = 0;
        std::cout << file << std::endl;
        auto* data = stbi_load_16(file.c_str(), &m_width, &m_height,&c, 1);
        if (data)
        {
            const int dimensions = m_width * m_height;
            const float m = 1.0f / 65535.0f;
            m_data.resize(dimensions);
            for (int i = 0; i < dimensions; i++) {
                m_data[i] = data[i] * m;
            }
            free(data);
        }
        else
        {
            std::cout << data << std::endl;
            std::cerr << "Failed to load image" << std::endl;
        }
    }

    std::tuple<int, float> HeightMap::FindTriangle(const Int2& p0, 
        const Int2& p1, const Int2& p2)
    {
        const auto edge = [](
            const auto& a, const auto& b, const auto& c)
        {
            return (b.x() - c.x()) * (a.y() - c.y()) - (b.y() - c.y()) * (a.x() - c.x());
        };


    }
}
