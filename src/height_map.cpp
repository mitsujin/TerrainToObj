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

    HeightMap::HeightMap(int size, int width)
    {
        m_segmentWidth = width;
        m_segmentSide = std::max(1, (int)sqrt(size));
        m_width = m_segmentSide * width;
        m_height = m_segmentSide * width;
        m_stride = m_segmentWidth * m_segmentWidth * m_segmentSide;
        m_data.resize(size * width * width);
    }

    void HeightMap::SetData(int index, const std::string& file)
    {
        std::vector<float> dataF;
        int c = 0;
        int width;
        int height;
        auto* data = stbi_load_16(file.c_str(), &width, &height,&c, 1);
        if (data)
        {
            std::cout << "Loaded: " << index << std::endl;
            const int dimensions = width * height;
            const float m = 1.0f / 65535.0f;
            dataF.resize(dimensions);
            for (int i = 0; i < dimensions; i++) {
                dataF[i] = data[i] * m;
            }
            free(data);
        }
        else
        {
            std::cout << data << std::endl;
            std::cerr << "Failed to load image" << std::endl;
        }

        int x = index % m_segmentSide;
        int y = index / m_segmentSide;

        int startIndex = m_stride * y + x * m_segmentWidth;
        int lineStride = m_segmentWidth * m_segmentSide;

        for (int z = 0; z < m_segmentWidth; z++)
        {
            int destIndex = startIndex + z * lineStride;
            int sourceIndex = z * m_segmentWidth;
            memcpy(&m_data[destIndex], &dataF[sourceIndex], sizeof(float) * m_segmentWidth);
        }
    }

    std::tuple<Int2, float> HeightMap::FindTriangle(const Int2& p0, 
        const Int2& p1, const Int2& p2)
    {
        const auto edge = [](
            const auto& a, const auto& b, const auto& c)
        {
            return (b.x() - c.x()) * (a.y() - c.y()) - (b.y() - c.y()) * (a.x() - c.x());
        };

        const auto min = Min(Min(p0, p1), p2);
        const auto max = Max(Max(p0, p1), p2);

        int w00 = edge(p1, p2, min);
        int w01 = edge(p2, p0, min);
        int w02 = edge(p0, p1, min);
        const int a01 = p1.y() - p0.y();
        const int b01 = p0.x() - p1.x();
        const int a12 = p2.y() - p1.y();
        const int b12 = p1.x() - p2.x();
        const int a20 = p0.y() - p2.y();
        const int b20 = p2.x() - p0.x();

        const float a = edge(p0, p1, p2);
        const float z0 = HeightAt(p0) / a;
        const float z1 = HeightAt(p1) / a;
        const float z2 = HeightAt(p2) / a;

        // iterate over pixels in bounding box
        float maxError = 0;
        Int2 maxPoint(0, 0);
        for (int y = min.y(); y <= max.y(); y++) {
            // compute starting offset
            int dx = 0;
            if (w00 < 0 && a12 != 0) {
                dx = std::max(dx, -w00 / a12);
            }
            if (w01 < 0 && a20 != 0) {
                dx = std::max(dx, -w01 / a20);
            }
            if (w02 < 0 && a01 != 0) {
                dx = std::max(dx, -w02 / a01);
            }

            int w0 = w00 + a12 * dx;
            int w1 = w01 + a20 * dx;
            int w2 = w02 + a01 * dx;

            bool wasInside = false;

            for (int x = min.x() + dx; x <= max.x(); x++) {
                // check if inside triangle
                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    wasInside = true;

                    // compute z using barycentric coordinates
                    const float z = z0 * w0 + z1 * w1 + z2 * w2;
                    const float dz = std::abs(z - HeightAt(x, y));
                    if (dz > maxError) {
                        maxError = dz;
                        maxPoint = Int2(x, y);
                    }
                } else if (wasInside) {
                    break;
                }

                w0 += a12;
                w1 += a20;
                w2 += a01;
            }

            w00 += b12;
            w01 += b20;
            w02 += b01;
        }

        if (maxPoint == p0 || maxPoint == p1 || maxPoint == p2) {
            maxError = 0;
        }

        return { maxPoint, maxError };
    }
}
