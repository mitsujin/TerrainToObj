#ifndef OBJ_WRITER_H
#define OBJ_WRITER_H

#include <string>
#include <vector>
#include <vec3.h>

namespace TerrainToObj
{
    class ObjWriter
    {
    public:
        static bool Write(const std::string& file, 
            const std::vector<Float3>& points, 
            const std::vector<Int3>& triangles);
    };
}

#endif /* OBJ_WRITER_H */
