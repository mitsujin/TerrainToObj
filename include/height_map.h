#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H

namespace TerrainToObj
{
    class HeightMap
    {
    public:
        HeightMap(const std::string& file);

        float HeightAt(int x, int y) const; 
        
    };
}

#endif /* HEIGHT_MAP_H */
